#include <PalmOS.h>
#include <PalmOSGlue.h>
#include <68K\Hs.h>

#include "EasyPhone.h"
#include "EasyPhone_Rsc.h"

#if 0
#pragma mark -
#pragma mark Functions for Common use
#pragma mark -
#endif

static void getContactUnitById_Addr(GlobeType *globeP, UInt16 recordId)
{
	globeP->CNLBparamP->recordId = recordId;
	CLibGetMatchContactByRecordId_Addr(globeP->refNum, globeP->CNLBGlobeP, globeP->CNLBparamP, (OLD_AddrDBRecordType *)globeP->contactUnitP, SEARCH_MODE_GET);
}
static void getContactUnitById_PAdd(GlobeType *globeP, UInt16 recordId)
{
	globeP->CNLBparamP->recordId = recordId;
	CLibGetMatchContactByRecordId_PAdd(globeP->refNum, globeP->CNLBGlobeP, globeP->CNLBparamP, (AddrDBRecordType *)globeP->contactUnitP, SEARCH_MODE_GET);
}

static void freeContactList(GlobeType *globeP)
{
	UInt16	i;
	
	globeP->contactListPP = (ContactUnitType **)MemHandleLock(globeP->contactListH);
	
	//循环释放节点内存
	for (i = 0; i < globeP->contactCount; i ++)
	{
		MemPtrFree(globeP->contactListPP[i]);
	}
	
	MemHandleUnlock(globeP->contactListH);
	globeP->contactListPP = NULL;
	
	//重置节点计数器
	globeP->contactCount = 0;
	
	//重置映射表
	MemSet(globeP->listMappingP, globeP->listMappingSize, 0);
	globeP->listMappingCount = 0;
}

static UInt16 StrDigitCopy(Char * dstString, Char * srcString)
{
	UInt16 dstLength = 0;
	
	while (*srcString)
	{
		if (TxtCharIsDigit(*srcString) || (*srcString) == '+')
		{
			dstString[dstLength] = *srcString;
			dstLength ++;
		}
		srcString ++;
	}
	
	dstString[dstLength] = 0x00;

	return dstLength;
}

static Boolean isFieldTextSelected(FieldType *field)
{
	UInt16		start, end;
	
	FldGetSelection(field, &start, &end);
	
	if (start != end)
	{
		return true;
	}
	
	return false;
}

static UInt16 getPhoneLabelText(AddressPhoneLabels phoneLabel, Char *cache)
{
	switch (phoneLabel)
	{
		case workLabel:
		{
			StrCopy(cache, "(W)");
			break;
		}
		case homeLabel:
		{
			StrCopy(cache, "(H)");
			break;
		}
		case faxLabel:
		{
			StrCopy(cache, "(F)");
			break;
		}
		case otherLabel:
		{
			StrCopy(cache, "(O)");
			break;
		}
		case emailLabel:
		{
			StrCopy(cache, "(E)");
			break;
		}
		case mainLabel:
		{
			StrCopy(cache, "(M)");
			break;
		}
		case pagerLabel:
		{
			StrCopy(cache, "(P)");
			break;
		}
		case mobileLabel:
		{
			StrCopy(cache, "(M)");
			break;
		}
	}
	
	//return FntCharsWidth(globeP->cache, 3);
	return 0;
}

static UInt16 getMatchedPhoneField(Char *phoneNumberP, void *recordP, Boolean isOldType)
{
	Char		*phoneP;
	UInt16		i;
	UInt16		phoneNumberLength	= StrLen(phoneNumberP);
	
	if (phoneNumberLength > 8)
	{
		phoneNumberP += (phoneNumberLength - 8);
	}
	
	if (isOldType)
	{
		for (i = OLD_phone1; i <= OLD_phone5; i ++)
		{
			if (((OLD_AddrDBRecordType *)recordP)->fields[i])
			{
				phoneP = ((OLD_AddrDBRecordType *)recordP)->fields[i];
				phoneNumberLength = StrLen(phoneP);
				if (phoneNumberLength > 8)
				{
					phoneP += (phoneNumberLength - 8);
				}
				if (StrCompare(phoneP, phoneNumberP) == 0)
				{
					break;
				}
			}
		}
	}
	else
	{
		for (i = phone1; i <= phone7; i ++)
		{
			if (((AddrDBRecordType *)recordP)->fields[i])
			{
				phoneP = ((AddrDBRecordType *)recordP)->fields[i];
				phoneNumberLength = StrLen(phoneP);
				if (phoneNumberLength > 8)
				{
					phoneP += (phoneNumberLength - 8);
				}
				if (StrCompare(phoneP, phoneNumberP) == 0)
				{
					break;
				}
			}
		}
	}
	
	return i;
}

static UInt16 getSelectedPhoneField(Int16 itemNum, Int16 listIndex, GlobeType *globeP)
{
	if (globeP->settingP->showDetailType == NO_DETAIL)
	{
		UInt16		i;
		Int16		n	= 0;
		
		globeP->contactListPP = (ContactUnitType **)MemHandleLock(globeP->contactListH);
		
		if (globeP->settingP->treoModel == TREO_600 || globeP->settingP->treoModel == NOT_A_TREO)
		{
			getContactUnitById_Addr(globeP, globeP->contactListPP[itemNum]->recordId);
			for (i = OLD_phone1; i < OLD_phone5; i ++)
			{
				if (((OLD_AddrDBRecordType *)globeP->contactUnitP)->fields[i])
				{
					if (n == listIndex)		//找到对应的号码项
					{
						break;
					}
					else
					{
						n ++;
					}
				}
			}
		}
		else
		{
			getContactUnitById_PAdd(globeP, globeP->contactListPP[itemNum]->recordId);
			for (i = phone1; i < phone7; i ++)
			{
				if (((AddrDBRecordType *)globeP->contactUnitP)->fields[i])
				{
					if (n == listIndex)		//找到对应的号码项
					{
						break;
					}
					else
					{
						n ++;
					}
				}
			}
		}
		
		MemHandleUnlock(globeP->contactListH);
		globeP->contactListPP = NULL;
		
		return i;
	}
	
	return (UInt16)globeP->listMappingP[itemNum].field;
}

static AddressPhoneLabels getSelectedPhoneLabel(GlobeType *globeP, Int16 itemNum, UInt16 fieldNum)
{
	AddressPhoneLabels		i;
	UInt16					recordId;
	
	globeP->contactListPP = (ContactUnitType **)MemHandleLock(globeP->contactListH);
	
	if (globeP->settingP->showDetailType == NO_DETAIL)
	{
		recordId = globeP->contactListPP[itemNum]->recordId;
	}
	else
	{
		recordId = globeP->contactListPP[globeP->listMappingP[itemNum].index]->recordId;
	}
	
	if (globeP->settingP->treoModel == TREO_600 || globeP->settingP->treoModel == NOT_A_TREO)
	{
		getContactUnitById_Addr(globeP, recordId);
		i = OLD_GetPhoneLabel((OLD_AddrDBRecordType *)globeP->contactUnitP, fieldNum);
	}
	else
	{
		getContactUnitById_PAdd(globeP, recordId);
		i = GetPhoneLabel((AddrDBRecordType *)globeP->contactUnitP, fieldNum);
	}
	
	MemHandleUnlock(globeP->contactListH);
	globeP->contactListPP = NULL;
	
	return i;
}

static UInt16 getRecordByPhoneNumber(void *resultP, Char *phoneNumberP, ActionParameterType *actionParamP, GlobeType *globeP)
{
	UInt16			recordCount	= 0;
	
	if (globeP->totalContacts > 0)
	{
		//分配内存
		MemHandle		recordH		= MemHandleNew(globeP->totalContacts * 4);
		
		if (recordH)
		{
			void			**recordPP	= (void **)MemHandleLock(recordH);
			
			//临时关闭数据库连接
			DmCloseDatabase(globeP->CNLBGlobeP->addressDbRef);
			
			//查询
			if (globeP->settingP->treoModel == TREO_600 || globeP->settingP->treoModel == NOT_A_TREO)
			{
				CLibGetContacts_Addr(globeP->refNum, phoneNumberP, false, SEARCH_MODE_PHONE, (OLD_AddrDBRecordType ***)&recordPP, &recordCount);
			}
			else
			{
				CLibGetContacts_PAdd(globeP->refNum, phoneNumberP, false, SEARCH_MODE_PHONE, (AddrDBRecordType ***)&recordPP, &recordCount);
			}
			
			//恢复数据库连接
			if (globeP->settingP->treoModel == TREO_600 || globeP->settingP->treoModel == NOT_A_TREO)
			{
				globeP->CNLBGlobeP->addressDbRef = DmOpenDatabaseByTypeCreator('DATA', 'addr', dmModeReadWrite);
			}
			else
			{
				globeP->CNLBGlobeP->addressDbRef = DmOpenDatabaseByTypeCreator('DATA', 'PAdd', dmModeReadWrite);
			}
			
			//构造姓名
			if (recordCount > 0)
			{
				UInt16		i;
				
				if (globeP->settingP->treoModel == TREO_600 || globeP->settingP->treoModel == NOT_A_TREO)
				{
					//拷贝到结果
					MemMove(resultP, recordPP[0], sizeof(OLD_AddrDBRecordType));
				}
				else
				{
					//拷贝到结果
					MemMove(resultP, recordPP[0], sizeof(AddrDBRecordType));
				}
				
				for (i = 0; i < recordCount; i ++)
				{
					MemPtrFree(recordPP[i]);
				}
			}
			
			MemHandleUnlock(recordH);
			MemHandleFree(recordH);
		}
	}
	
	return recordCount;
}

static WChar keyTranslator(EventType *eventP)
{
	if (EvtKeydownIsVirtual(eventP))
	{
		if (eventP->data.keyDown.keyCode > 0)
		{
			return eventP->data.keyDown.keyCode;
		}
		else
		{
			return eventP->data.keyDown.chr;
		}
	}
	else
	{
		if (eventP->data.keyDown.chr != eventP->data.keyDown.keyCode)
		{
			if (eventP->data.keyDown.chr != keyZero)
			{
				return eventP->data.keyDown.keyCode;
			}
			else
			{
				return eventP->data.keyDown.chr;
			}
		}
		else
		{
			return eventP->data.keyDown.chr;
		}
	}
	
	return chrNull;
}

#if 0
#pragma mark -
#pragma mark Functions for Contact Form
#pragma mark -
#endif

#if 0
#pragma mark == COMMON ACTION HANDLER ==
#endif

static void buildFullName_Addr(Char *cache, OLD_AddrDBRecordType *data)
{
	MemSet(cache, 256, 0);
	
	if (data->fields[OLD_name])
	{
		StrCopy(cache, data->fields[OLD_name]);
	}
	
	if (data->fields[OLD_firstName])
	{
		StrCat(cache, data->fields[OLD_firstName]);
	}
	
	if ((UInt8)(*cache) == 0)
	{
		if (data->fields[OLD_company])
		{
			StrCopy(cache, data->fields[OLD_company]);
		}
	}
}

static void buildFullName_PAdd(Char *cache, AddrDBRecordType *data)
{
	MemSet(cache, 256, 0);
	
	if (data->fields[name])
	{
		StrCopy(cache, data->fields[name]);
	}
	
	if (data->fields[firstName])
	{
		StrCat(cache, data->fields[firstName]);
	}
	
	if ((UInt8)(*cache) == 0)
	{
		if (data->fields[company])
		{
			StrCopy(cache, data->fields[company]);
		}
	}
}

static UInt16 constructPreFix(Char *phoneNumberP, GlobeType *globeP, ActionParameterType *actionParameterP)
{
	UInt16		iPreFixLength;
	UInt16		ignoreNumber = 0;
	
	//构造不带通配符的前缀串
	MemSet(globeP->cache, 256, 0);
	iPreFixLength = StrDigitCopy(globeP->cache, globeP->preFix[globeP->currentPreFix]);
	
	//选定了可构造的前缀吗？
	if (StrCompare(globeP->preFix[globeP->currentPreFix], "-") != 0 &&
		(StrNCompare(globeP->cache, phoneNumberP, iPreFixLength) != 0))
	{
		//可构造，获取应该被忽略的电话号码数
		while (globeP->preFix[globeP->currentPreFix][iPreFixLength] == 'i')
		{
			ignoreNumber ++;
			iPreFixLength ++;
		}
		
		//构造IP号码
		StrCopy(actionParameterP->phoneNumber, globeP->cache);
	}
	
	return ignoreNumber;
}

static void getActionParameter(ActionParameterType *actionParamP, GlobeType *globeP, UInt8 exitCode)
{
	Int16			selectedListItem	= LstGetSelection(globeP->lstContactsP);
	Char			*phoneNumberP		= FldGetTextPtr(globeP->fldPhoneNumberP);
	
	//无论如何，总是要得到姓名的
	MemSet(actionParamP->fullName, 256, 0);
	if (selectedListItem != noListSelection)	//从列表框获得的号码
	{
		if (globeP->contactCount > 0)	//是联系人列表
		{
			UInt16		recordId;
			
			globeP->contactListPP = (ContactUnitType **)MemHandleLock(globeP->contactListH);
			
			if (globeP->settingP->showDetailType == NO_DETAIL)
			{
				recordId = globeP->contactListPP[selectedListItem]->recordId;
			}
			else
			{
				recordId = globeP->contactListPP[globeP->listMappingP[selectedListItem].index]->recordId;
			}
			
			//取姓名和号码标签
			MemSet(globeP->cache, 256, 0);
			if (globeP->settingP->treoModel == TREO_600 || globeP->settingP->treoModel == NOT_A_TREO)
			{
				getContactUnitById_Addr(globeP, recordId);
				buildFullName_Addr(actionParamP->fullName, globeP->contactUnitP);
				
				if (actionParamP->fullName[0] != '\0')
				{
					getPhoneLabelText(OLD_GetPhoneLabel((OLD_AddrDBRecordType *)globeP->contactUnitP, globeP->currentField), globeP->cache);
					StrCat(actionParamP->fullName, " ");
					StrCat(actionParamP->fullName, globeP->cache);
				}
			}
			else
			{
				getContactUnitById_PAdd(globeP, recordId);
				buildFullName_PAdd(actionParamP->fullName, globeP->contactUnitP);
				
				if (actionParamP->fullName[0] != '\0')
				{
					getPhoneLabelText(GetPhoneLabel((AddrDBRecordType *)globeP->contactUnitP, globeP->currentField), globeP->cache);
					StrCat(actionParamP->fullName, " ");
					StrCat(actionParamP->fullName, globeP->cache);
				}
			}
			actionParamP->recordId = recordId;
			
			MemHandleUnlock(globeP->contactListH);
			globeP->contactListPP = NULL;
		}
		else if (globeP->recentCount > 0)	//是常用联系人列表
		{
			if (StrCompare(globeP->recentListP[selectedListItem].fullName, globeP->recentListP[selectedListItem].phoneNumber) != 0)
			{
				StrCopy(actionParamP->fullName, globeP->recentListP[selectedListItem].fullName);
			}
		}
	}
	else	//手动输入的号码
	{
		void	*recordP;
		
		MemSet(globeP->cache, 256, 0);
		if (globeP->settingP->treoModel == TREO_600 || globeP->settingP->treoModel == NOT_A_TREO)
		{
			recordP = MemPtrNew(sizeof(OLD_AddrDBRecordType));
			
			if (getRecordByPhoneNumber(recordP, phoneNumberP, actionParamP, globeP) > 0)
			{
				buildFullName_Addr(actionParamP->fullName, (OLD_AddrDBRecordType*)recordP);
				if (actionParamP->fullName[0] != '\0')
				{
					getPhoneLabelText(OLD_GetPhoneLabel((OLD_AddrDBRecordType*)recordP, getMatchedPhoneField(phoneNumberP, recordP, true)), globeP->cache);
					StrCat(actionParamP->fullName, " ");
					StrCat(actionParamP->fullName, globeP->cache);
				}
			}
		}
		else
		{
			recordP = MemPtrNew(sizeof(AddrDBRecordType));
			
			if (getRecordByPhoneNumber(recordP, phoneNumberP, actionParamP, globeP) > 0)
			{
				buildFullName_PAdd(actionParamP->fullName, (AddrDBRecordType*)recordP);
				if (actionParamP->fullName[0] != '\0')
				{
					getPhoneLabelText(GetPhoneLabel((AddrDBRecordType*)recordP, getMatchedPhoneField(phoneNumberP, recordP, false)), globeP->cache);
					StrCat(actionParamP->fullName, " ");
					StrCat(actionParamP->fullName, globeP->cache);
				}
			}
		}
		
		MemPtrFree(recordP);
	}
	
	//取号码
	MemSet(actionParamP->phoneNumber, 128, 0);
	if (exitCode == EXIT_FOR_DIAL || exitCode == EXIT_FOR_MESSAGE ||
		exitCode == EXIT_FOR_SYSTEM_MESSAGE)	//需带上电话卡
	{
		StrDigitCopy(globeP->cache, (phoneNumberP + constructPreFix(phoneNumberP, globeP, actionParamP)));
	}
	else if (exitCode == EXIT_FOR_EMAIL)	//不带电话卡、不过滤文字
	{
		StrCopy(globeP->cache, phoneNumberP);
	}
	else	//不需电话卡
	{
		StrDigitCopy(globeP->cache, phoneNumberP);
	}
	StrCat(actionParamP->phoneNumber, globeP->cache);
	
	actionParamP->actionCode = exitCode;
	actionParamP->hasActionToPerform = true;
}

#if 0
#pragma mark == RECENT DB HANDLER ==
#endif

static void getRecentList(GlobeType *globeP)
{
	MemHandle		recordH;
	UInt16			i;
	
	//释放通讯记录节点
	freeContactList(globeP);
	
	//清除纪录
	MemSet(globeP->recentListP, sizeof(RecentUnitType) * 10, 0);
	
	//获取联系人列表总数
	globeP->recentCount = DmNumRecordsInCategory(globeP->recentDbRef, dmAllCategories);
	
	if (globeP->recentCount > 0)
	{
		Boolean		shouldChangeColor	= true;
		//读取各个联系人记录
		for (i = 0; i < globeP->recentCount; i ++)
		{
			//读取记录并拷贝到unit
			recordH = DmQueryRecord(globeP->recentDbRef, i);
			MemMove(&globeP->recentListP[i], MemHandleLock(recordH), sizeof(RecentUnitType));
			globeP->recentListP[i].shouldChangeColor = shouldChangeColor;
			shouldChangeColor = ! shouldChangeColor;
			MemHandleUnlock(recordH);
		}
	}
}

static void deleteRecentItem(GlobeType *globeP, UInt16 recordId)
{
	if (globeP->recentCount > 0)
	{
		//删除联系人项目
		DmRemoveRecord(globeP->recentDbRef, recordId);
	}
}

static void saveContactToRecentDB(GlobeType *globeP, ActionParameterType *actionParameterP)
{
	Boolean			isContactExist = false;
	UInt16			recordId;
	UInt16			newRecordId;
	MemHandle		recordH;
	RecentUnitType	*recordP;
	Char			*p1;
	UInt16			p1Length;
	
	//检测联系人是否已经存在
	p1 = actionParameterP->phoneNumber;
	p1Length = StrLen(actionParameterP->phoneNumber);
	if (p1Length > 8)
	{
		p1 += (p1Length - 8);
	}
	for (recordId = 0; recordId < globeP->recentCount; recordId ++)
	{
		if ((StrCompare(globeP->recentListP[recordId].fullName, actionParameterP->fullName) == 0 || 
			(StrCompare(globeP->recentListP[recordId].fullName, actionParameterP->phoneNumber) == 0 && actionParameterP->fullName[0] == '\0')) &&
			StrCompare(globeP->recentListP[recordId].phoneNumber, actionParameterP->phoneNumber) == 0)
		{
			isContactExist = true;
			break;
		}
	}
	
	if (isContactExist)
	{ //联系人已经存在
		if (recordId > 0)
		{ //有必要进行频率调整
			//计算新位置
			newRecordId = recordId / 2;
			
			//移动记录到该位置
			DmMoveRecord(globeP->recentDbRef, recordId, newRecordId);
		}
	}
	else
	{ //联系人不存在
		//获取需操作的记录
		if (globeP->recentCount == 10)
		{ //已经到达上限，修改最后一个记录
			//获取记录
			recordH = DmGetRecord(globeP->recentDbRef, 9);
			recordP = (RecentUnitType *)MemHandleLock(recordH);
			recordId = 9;
		}
		else
		{ //未到达上限，增加一个记录
			recordId = dmMaxRecordIndex;
			recordH = DmNewRecord(globeP->recentDbRef, &recordId, sizeof(RecentUnitType));
			recordP = (RecentUnitType *)MemHandleLock(recordH);
		}
		//DmSet(recordP, 0, sizeof(RecentUnitType), 0);
		
		//填入信息
		if (actionParameterP->fullName[0] != '\0')
		{
			DmWrite(recordP, 0, actionParameterP->fullName, 256);
		}
		else
		{
			DmWrite(recordP, 0, actionParameterP->phoneNumber, 128);
		}
		DmWrite(recordP, 256, actionParameterP->phoneNumber, 128);
		
		//解锁并释放记录
		MemHandleUnlock(recordH);
		DmReleaseRecord(globeP->recentDbRef, recordId, true);
		
		//移动记录到记录头
		DmMoveRecord(globeP->recentDbRef, recordId, 0);
	}
}

#if 0
#pragma mark == CONTACT DB HANDLER ==
#endif

//****************************************************************************
//读取符合关键字要求的联系人列表
//****************************************************************************
static void getContactList_Addr(CNLBGlobeType *CNLBglobeP, CNLBParameterType *CNLBparamP, void *voidGlobeP)
{
	UInt16					phoneNumberLength;
	UInt8					i;
	Boolean					shouldChangeColor	= true;
	GlobeType				*globeP				= (GlobeType *)voidGlobeP;
	Char					*phoneNumberP		= FldGetTextPtr(globeP->fldPhoneNumberP);
	
	if (globeP->totalContacts > 0)
	{
		if (phoneNumberP)
		{
			phoneNumberLength = StrLen(phoneNumberP);
		}
		else
		{
			phoneNumberLength = 0;
		}
		//构造拼音位封装
		constructPYKeyword(CNLBparamP, globeP->pyKeyword);
		
		//清除通讯记录节点
		freeContactList(globeP);
		
		//锁定列表
		globeP->contactListPP = (ContactUnitType **)MemHandleLock(globeP->contactListH);
		
		//预分配节点
		globeP->contactListPP[0] = (ContactUnitType *)MemPtrNew(sizeof(ContactUnitType));
		
		//遍历通讯记录
		CNLBparamP->recordId = 0;
		while (CNLBparamP->recordId < globeP->totalContacts)
		{
			//检查当前记录是否符合拼音
			if (CLibGetMatchContactByRecordId_Addr(globeP->refNum, CNLBglobeP, CNLBparamP, globeP->contactUnitP, SEARCH_MODE_FULLNAME) == errNone)
			{
				//找到一个拼音匹配的记录
				globeP->contactListPP[globeP->contactCount]->matchType = NORMAL_RESULT;
				globeP->contactListPP[globeP->contactCount]->recordId = CNLBparamP->recordId;
				
				if (globeP->settingP->showDetailType == NO_DETAIL)
				{
					globeP->contactListPP[globeP->contactCount]->shouldChangeColor = shouldChangeColor;
					shouldChangeColor = ! shouldChangeColor;
				}
				else
				{
					//姓名
					globeP->listMappingP[globeP->listMappingCount].index = globeP->contactCount;
					globeP->listMappingP[globeP->listMappingCount].field = 0xFF;
					globeP->listMappingP[globeP->listMappingCount].shouldChangeColor = true;
					globeP->listMappingCount ++;
					
					//号码
					for (i = OLD_phone1; i <= OLD_phone5; i ++)
					{
						if (((OLD_AddrDBRecordType *)globeP->contactUnitP)->fields[i])
						{
							globeP->listMappingP[globeP->listMappingCount].index = globeP->contactCount;
							globeP->listMappingP[globeP->listMappingCount].field = i;
							globeP->listMappingP[globeP->listMappingCount].shouldChangeColor = false;
							globeP->listMappingCount ++;
						}
					}
				}
				
				//再分配一个节点用于检索
				globeP->contactCount ++;
				globeP->contactListPP[globeP->contactCount] = (ContactUnitType *)MemPtrNew(sizeof(ContactUnitType));
			}
			else if (phoneNumberLength >= 3)//检测是否符合电话号码
			{
				for (i = OLD_phone1; i <= OLD_phone5; i ++)
				{
					if (((OLD_AddrDBRecordType *)globeP->contactUnitP)->fields[i])
					{
						if (StrStr(((OLD_AddrDBRecordType *)globeP->contactUnitP)->fields[i], phoneNumberP))
						{
							//找到一个电话号码匹配的记录
							globeP->contactListPP[globeP->contactCount]->matchType = PHONE_MATCH_RESULT;
							globeP->contactListPP[globeP->contactCount]->recordId = CNLBparamP->recordId;
							
							if (globeP->settingP->showDetailType == NO_DETAIL)
							{
								globeP->contactListPP[globeP->contactCount]->shouldChangeColor = shouldChangeColor;
								shouldChangeColor = ! shouldChangeColor;
							}
							else
							{
								//姓名
								globeP->listMappingP[globeP->listMappingCount].index = globeP->contactCount;
								globeP->listMappingP[globeP->listMappingCount].field = 0xFF;
								globeP->listMappingP[globeP->listMappingCount].shouldChangeColor = true;
								globeP->listMappingCount ++;
								
								//号码
								for (i = OLD_phone1; i <= OLD_phone5; i ++)
								{
									if (((OLD_AddrDBRecordType *)globeP->contactUnitP)->fields[i])
									{
										globeP->listMappingP[globeP->listMappingCount].index = globeP->contactCount;
										globeP->listMappingP[globeP->listMappingCount].field = i;
										globeP->listMappingP[globeP->listMappingCount].shouldChangeColor = false;
										globeP->listMappingCount ++;
									}
								}
							}
							
							//再分配一个节点用于检索
							globeP->contactCount ++;
							globeP->contactListPP[globeP->contactCount] = (ContactUnitType *)MemPtrNew(sizeof(ContactUnitType));
							break;
						}
					}
				}
			}
			refreshCNLBGlobeParameter(CNLBglobeP);
			CNLBparamP->recordId ++;
		}
		
		//释放和解锁内存
		MemPtrFree(globeP->contactListPP[globeP->contactCount]); //额外生成的节点
		MemHandleUnlock(globeP->contactListH);
		globeP->contactListPP = NULL;
	}
}
static void getContactList_PAdd(CNLBGlobeType *CNLBglobeP, CNLBParameterType *CNLBparamP, void *voidGlobeP)
{
	UInt16					phoneNumberLength;
	UInt8					i;
	Boolean					shouldChangeColor	= true;
	GlobeType				*globeP				= (GlobeType *)voidGlobeP;
	Char					*phoneNumberP		= FldGetTextPtr(globeP->fldPhoneNumberP);
	
	if (globeP->totalContacts > 0)
	{
		if (phoneNumberP)
		{
			phoneNumberLength = StrLen(phoneNumberP);
		}
		else
		{
			phoneNumberLength = 0;
		}
		
		//构造拼音位封装
		constructPYKeyword(CNLBparamP, globeP->pyKeyword);
		
		//清除通讯记录节点
		freeContactList(globeP);
		
		//锁定列表
		globeP->contactListPP = (ContactUnitType **)MemHandleLock(globeP->contactListH);
		
		//预分配节点
		globeP->contactListPP[0] = (ContactUnitType *)MemPtrNew(sizeof(ContactUnitType));
		
		//遍历通讯记录
		CNLBparamP->recordId = 0;
		while (CNLBparamP->recordId < globeP->totalContacts)
		{
			//检查当前记录是否符合拼音
			if (CLibGetMatchContactByRecordId_PAdd(globeP->refNum, CNLBglobeP, CNLBparamP, globeP->contactUnitP, SEARCH_MODE_FULLNAME) == errNone)
			{
				//找到一个拼音匹配的记录
				globeP->contactListPP[globeP->contactCount]->matchType = NORMAL_RESULT;
				globeP->contactListPP[globeP->contactCount]->recordId = CNLBparamP->recordId;
				
				if (globeP->settingP->showDetailType == NO_DETAIL)
				{
					globeP->contactListPP[globeP->contactCount]->shouldChangeColor = shouldChangeColor;
					shouldChangeColor = ! shouldChangeColor;
				}
				else
				{
					//姓名
					globeP->listMappingP[globeP->listMappingCount].index = globeP->contactCount;
					globeP->listMappingP[globeP->listMappingCount].field = 0xFF;
					globeP->listMappingP[globeP->listMappingCount].shouldChangeColor = true;
					globeP->listMappingCount ++;
					
					//号码
					for (i = phone1; i <= phone7; i ++)
					{
						if (((AddrDBRecordType *)globeP->contactUnitP)->fields[i])
						{
							globeP->listMappingP[globeP->listMappingCount].index = globeP->contactCount;
							globeP->listMappingP[globeP->listMappingCount].field = i;
							globeP->listMappingP[globeP->listMappingCount].shouldChangeColor = false;
							globeP->listMappingCount ++;
						}
					}
				}
				
				//再分配一个节点用于检索
				globeP->contactCount ++;
				globeP->contactListPP[globeP->contactCount] = (ContactUnitType *)MemPtrNew(sizeof(ContactUnitType));
			}
			else if (phoneNumberLength >= 3)//检测是否符合电话号码
			{
				for (i = phone1; i <= phone7; i ++)
				{
					if (((AddrDBRecordType *)globeP->contactUnitP)->fields[i])
					{
						if (StrStr(((AddrDBRecordType *)globeP->contactUnitP)->fields[i], phoneNumberP))
						{
							//找到一个电话号码匹配的记录
							globeP->contactListPP[globeP->contactCount]->matchType = PHONE_MATCH_RESULT;
							globeP->contactListPP[globeP->contactCount]->recordId = CNLBparamP->recordId;
							
							if (globeP->settingP->showDetailType == NO_DETAIL)
							{
								globeP->contactListPP[globeP->contactCount]->shouldChangeColor = shouldChangeColor;
								shouldChangeColor = ! shouldChangeColor;
							}
							else
							{
								//姓名
								globeP->listMappingP[globeP->listMappingCount].index = globeP->contactCount;
								globeP->listMappingP[globeP->listMappingCount].field = 0xFF;
								globeP->listMappingP[globeP->listMappingCount].shouldChangeColor = true;
								globeP->listMappingCount ++;
								
								//号码
								for (i = phone1; i <= phone7; i ++)
								{
									if (((AddrDBRecordType *)globeP->contactUnitP)->fields[i])
									{
										globeP->listMappingP[globeP->listMappingCount].index = globeP->contactCount;
										globeP->listMappingP[globeP->listMappingCount].field = i;
										globeP->listMappingP[globeP->listMappingCount].shouldChangeColor = false;
										globeP->listMappingCount ++;
									}
								}
							}
							
							//再分配一个节点用于检索
							globeP->contactCount ++;
							globeP->contactListPP[globeP->contactCount] = (ContactUnitType *)MemPtrNew(sizeof(ContactUnitType));
							break;
						}
					}
				}
			}
			refreshCNLBGlobeParameter(CNLBglobeP);
			CNLBparamP->recordId ++;
		}
		
		//释放和解锁内存
		MemPtrFree(globeP->contactListPP[globeP->contactCount]); //额外生成的节点
		MemHandleUnlock(globeP->contactListH);
		globeP->contactListPP = NULL;
	}
}

#if 0
#pragma mark == UI DISPLAY HANDLER ==
#endif

static void updateFormControlPtr(GlobeType *globeP)
{
	globeP->frmContactsP = FrmGetActiveForm();
	globeP->lstContactsP = (ListType *)getObjectPtr(globeP->frmContactsP, lstContactsId);
	globeP->fldPhoneNumberP = (FieldType *)getObjectPtr(globeP->frmContactsP, fldPhoneNumberId);
	globeP->gdtDialButtonP = (FormGadgetType *)getObjectPtr(globeP->frmContactsP, gdtDialButtonId);
	globeP->gdtPreFixTriggerP = (FormGadgetType *)getObjectPtr(globeP->frmContactsP, gdtPreFixTriggerId);
	globeP->lstPreFixP = (ListType *)getObjectPtr(globeP->frmContactsP, lstPreFixId);
	globeP->gdtOperationButtonP = (FormGadgetType *)getObjectPtr(globeP->frmContactsP, gdtOperationButtonId);
	globeP->lstOperationP = (ListType *)getObjectPtr(globeP->frmContactsP, lstOperationId);
	globeP->gdtSMSButtonP = (FormGadgetType *)getObjectPtr(globeP->frmContactsP, gdtSMSButtonId);
	globeP->lstSMSP = (ListType *)getObjectPtr(globeP->frmContactsP, lstSMSId);
	globeP->gdtHelpButtonP = (FormGadgetType *)getObjectPtr(globeP->frmContactsP, gdtHelpButtonId);
}

static void copyDefaultPhoneNumber(Int16 itemNum, GlobeType *globeP)
{
	UInt16		i;
	Char		*phoneNumberP	= NULL;
	
	//取默认号码
	if (globeP->contactCount > 0)
	{
		globeP->contactListPP = (ContactUnitType **)MemHandleLock(globeP->contactListH);
		
		if (globeP->settingP->showDetailType == NO_DETAIL)
		{
			if (globeP->settingP->treoModel == TREO_600 || globeP->settingP->treoModel == NOT_A_TREO)
			{
				getContactUnitById_Addr(globeP, globeP->contactListPP[itemNum]->recordId);
				if (globeP->settingP->defaultField == 0xFF)
				{
					for (i = OLD_phone1; i < OLD_phone5; i ++)
					{
						if (((OLD_AddrDBRecordType *)globeP->contactUnitP)->fields[i])
						{
							phoneNumberP = ((OLD_AddrDBRecordType *)globeP->contactUnitP)->fields[i];
							break;
						}
					}
				}
				else
				{
					for (i = OLD_phone1; i < OLD_phone5; i ++)
					{
						if (((OLD_AddrDBRecordType *)globeP->contactUnitP)->fields[i])
						{
							if (OLD_GetPhoneLabel((OLD_AddrDBRecordType *)globeP->contactUnitP, i) == globeP->settingP->defaultField)
							{
								phoneNumberP = ((OLD_AddrDBRecordType *)globeP->contactUnitP)->fields[i];
								break;
							}
						}
					}
					if (! phoneNumberP)
					{
						for (i = OLD_phone1; i < OLD_phone5; i ++)
						{
							if (((OLD_AddrDBRecordType *)globeP->contactUnitP)->fields[i])
							{
								phoneNumberP = ((OLD_AddrDBRecordType *)globeP->contactUnitP)->fields[i];
								break;
							}
						}
					}
				}
			}
			else
			{
				getContactUnitById_PAdd(globeP, globeP->contactListPP[itemNum]->recordId);
				if (globeP->settingP->defaultField == 0xFF)
				{
					for (i = phone1; i < phone7; i ++)
					{
						if (((AddrDBRecordType *)globeP->contactUnitP)->fields[i])
						{
							phoneNumberP = ((AddrDBRecordType *)globeP->contactUnitP)->fields[i];
							break;
						}
					}
				}
				else
				{
					for (i = phone1; i < phone7; i ++)
					{
						if (((AddrDBRecordType *)globeP->contactUnitP)->fields[i])
						{
							if (GetPhoneLabel((AddrDBRecordType *)globeP->contactUnitP, i) == globeP->settingP->defaultField)
							{
								phoneNumberP = ((AddrDBRecordType *)globeP->contactUnitP)->fields[i];
								break;
							}
						}
					}
					if (! phoneNumberP)
					{
						for (i = phone1; i < phone7; i ++)
						{
							if (((AddrDBRecordType *)globeP->contactUnitP)->fields[i])
							{
								phoneNumberP = ((AddrDBRecordType *)globeP->contactUnitP)->fields[i];
								break;
							}
						}
					}
				}
			}
		}
		else
		{
			i = globeP->listMappingP[itemNum].field;
			
			if (i != 0xFF)
			{
				if (globeP->settingP->treoModel == TREO_600 || globeP->settingP->treoModel == NOT_A_TREO)
				{
					getContactUnitById_Addr(globeP, globeP->contactListPP[globeP->listMappingP[itemNum].index]->recordId);
					phoneNumberP = ((OLD_AddrDBRecordType *)globeP->contactUnitP)->fields[globeP->listMappingP[itemNum].field];
				}
				else
				{
					getContactUnitById_PAdd(globeP, globeP->contactListPP[globeP->listMappingP[itemNum].index]->recordId);
					phoneNumberP = ((AddrDBRecordType *)globeP->contactUnitP)->fields[globeP->listMappingP[itemNum].field];
				}
			}
		}
		
		if (phoneNumberP)
		{
			globeP->currentField = i;
		}
		else
		{
			globeP->currentField = 0;
		}
		
		MemHandleUnlock(globeP->contactListH);
		globeP->contactListPP = NULL;
	}
	else if (globeP->recentCount > 0)
	{
		phoneNumberP = globeP->recentListP[itemNum].phoneNumber;
		globeP->currentField = 0;
	}
	
	//拷贝到文本框
	FldDelete(globeP->fldPhoneNumberP, 0, 128);
	
	if (phoneNumberP)
	{
		UInt16		phoneNumberLength	= StrLen(phoneNumberP);
		
		FldInsert(globeP->fldPhoneNumberP, phoneNumberP, phoneNumberLength);
		FldSetSelection(globeP->fldPhoneNumberP, 0, phoneNumberLength);
	}
}

static void setListFrameColor(Boolean shouldSetToWhite, GlobeType *globeP)
{
	if (shouldSetToWhite)
	{
		RGBColorType	newListFrameColor	= {0, 255, 255, 255};
		
		//获取原边框颜色
		UIColorGetTableEntryRGB(UIObjectFrame, &globeP->originalListFrameRGBColor);
		
		//设置成白色
		UIColorSetTableEntry(UIObjectFrame, &newListFrameColor);
	}
	else
	{
		//还原边框颜色
		UIColorSetTableEntry(UIObjectFrame, &globeP->originalListFrameRGBColor);
	}
}

//****************************************************************************
//画联系人列表（显示最近使用的联系人）
//****************************************************************************
static void drawContactListByRecent(Int16 itemNum, RectangleType *bounds, Char **itemsText)
{
	GlobeType	*globeP		=	*(GlobeType **)itemsText;
	
	//重获全局指针
	//globeP = *(GlobeType **)itemsText;
	
	//获取范围
	bounds->topLeft.x -= 2;
	bounds->extent.x += 2;
	globeP->recentListP[itemNum].bounds = (*bounds);
	
	//画记录
	if (itemNum == LstGetSelection(globeP->lstContactsP))	//选中项
	{
		//显示项目
		WinDrawTruncChars(globeP->recentListP[itemNum].fullName, StrLen(globeP->recentListP[itemNum].fullName), bounds->topLeft.x, bounds->topLeft.y, bounds->extent.x - 10);
	}
	else	//非选中项
	{
		if (globeP->recentListP[itemNum].shouldChangeColor)	//改变底色
		{
			//修改背景颜色
			WinSetBackColorRGB(&globeP->alternateRGBColor, &globeP->originalRGBColor);
			
			//画背景
			WinEraseRectangle(bounds, 0);
			
			//显示项目
			WinDrawTruncChars(globeP->recentListP[itemNum].fullName, StrLen(globeP->recentListP[itemNum].fullName), bounds->topLeft.x, bounds->topLeft.y, bounds->extent.x - 10);
			
			//恢复背景颜色
			WinSetBackColorRGB(&globeP->originalRGBColor, NULL);
		}
		else
		{
			//显示项目
			WinDrawTruncChars(globeP->recentListP[itemNum].fullName, StrLen(globeP->recentListP[itemNum].fullName), bounds->topLeft.x, bounds->topLeft.y, bounds->extent.x - 10);
		}
	}
}

static void drawContactListByContact_Addr(Int16 itemNum, RectangleType *bounds, Char **itemsText)
{
	UInt16				itemLength;
	UInt16				itemWidth;
	GlobeType			*globeP		= *(GlobeType **)itemsText;
	
	//锁定列表
	globeP->contactListPP = (ContactUnitType **)MemHandleLock(globeP->contactListH);
	
	//获取范围
	bounds->topLeft.x -= 2;
	bounds->extent.x += 2;
	
	if (globeP->settingP->showDetailType != NO_DETAIL)
	{
		//取记录
		getContactUnitById_Addr(globeP, globeP->contactListPP[globeP->listMappingP[itemNum].index]->recordId);
		
		if (globeP->listMappingP[itemNum].field == 0xFF)	//姓名
		{
			buildFullName_Addr(globeP->cache, globeP->contactUnitP);
		}
		else
		{
			StrCopy(globeP->cache, ((OLD_AddrDBRecordType *)globeP->contactUnitP)->fields[globeP->listMappingP[itemNum].field]);
			StrCat(globeP->cache, " ");
			getPhoneLabelText((OLD_AddressPhoneLabels)OLD_GetPhoneLabel((OLD_AddrDBRecordType *)globeP->contactUnitP, globeP->listMappingP[itemNum].field), (globeP->cache + StrLen(globeP->cache)));
		}
		
		itemLength = StrLen(globeP->cache);
		itemWidth = FntCharsWidth(globeP->cache, itemLength);
		
		//调整号码对齐方式
		if (globeP->listMappingP[itemNum].field == 0xFF)
		{
			if (itemWidth > bounds->extent.x - 10)
			{
				itemWidth = bounds->extent.x - 10;
			}
		}
		else
		{
			if(itemWidth < bounds->extent.x)
			{
				bounds->topLeft.x += (bounds->extent.x - itemWidth);
			}
			else
			{
				itemWidth = bounds->extent.x;
			}
		}
		
		//显示内容
		if (globeP->listMappingP[itemNum].field == 0xFF)
		{
			if (itemNum == LstGetSelection(globeP->lstContactsP))	//选中项
			{
				//显示名称
				WinDrawTruncChars(globeP->cache, itemLength, bounds->topLeft.x + 10, bounds->topLeft.y, itemWidth);
			}
			else
			{
				//修改背景颜色
				WinSetBackColorRGB(&globeP->alternateRGBColor, &globeP->originalRGBColor);
				
				//画背景
				WinEraseRectangle(bounds, 0);
				
				//显示名称
				WinDrawTruncChars(globeP->cache, itemLength, bounds->topLeft.x + 10, bounds->topLeft.y, itemWidth);
				
				//恢复背景颜色
				WinSetBackColorRGB(&globeP->originalRGBColor, NULL);
			}
			
			//画圈~~
			globeP->typeRect.topLeft.x = bounds->topLeft.x + 3;
			globeP->typeRect.topLeft.y = bounds->topLeft.y + 3;
			
			if (globeP->contactListPP[globeP->listMappingP[itemNum].index]->matchType == PHONE_MATCH_RESULT)
			{
				WinSetForeColorRGB(&globeP->phoneNumberMatchRGBColor, &globeP->originalRGBColor);
			}
			else
			{
				WinSetForeColorRGB(&globeP->pyMatchRGBColor, &globeP->originalRGBColor);
			}
			WinDrawRectangle(&globeP->typeRect, 2);
			WinSetForeColorRGB(&globeP->originalRGBColor, NULL);
		}
		else
		{
			WinDrawTruncChars(globeP->cache, itemLength, bounds->topLeft.x, bounds->topLeft.y, itemWidth);
		}
	}
	else
	{
		globeP->contactListPP[itemNum]->bounds = (*bounds);
		
		//取记录
		getContactUnitById_Addr(globeP, globeP->contactListPP[itemNum]->recordId);
		
		//构造姓名
		buildFullName_Addr(globeP->cache, globeP->contactUnitP);
		
		itemLength = StrLen(globeP->cache);
		itemWidth = bounds->extent.x - 10;
		
		//显示内容
		if (itemNum == LstGetSelection(globeP->lstContactsP))	//选中项
		{
			//显示名称
			WinDrawTruncChars(globeP->cache, itemLength, bounds->topLeft.x + 10, bounds->topLeft.y, itemWidth);
		}
		else	//非选中项
		{
			if (globeP->contactListPP[itemNum]->shouldChangeColor)	//改变背景颜色
			{
				//修改背景颜色
				WinSetBackColorRGB(&globeP->alternateRGBColor, &globeP->originalRGBColor);
				
				//画背景
				WinEraseRectangle(bounds, 0);
				
				//显示名称
				WinDrawTruncChars(globeP->cache, itemLength, bounds->topLeft.x + 10, bounds->topLeft.y, itemWidth);
				
				//恢复背景颜色
				WinSetBackColorRGB(&globeP->originalRGBColor, NULL);
			}
			else	//不改变背景颜色
			{
				//显示名称
				WinDrawTruncChars(globeP->cache, itemLength, bounds->topLeft.x + 10, bounds->topLeft.y, itemWidth);
			}
		}
		
		//画圈~~
		globeP->typeRect.topLeft.x = bounds->topLeft.x + 3;
		globeP->typeRect.topLeft.y = bounds->topLeft.y + 3;
		
		if (globeP->contactListPP[itemNum]->matchType == PHONE_MATCH_RESULT)
		{
			WinSetForeColorRGB(&globeP->phoneNumberMatchRGBColor, &globeP->originalRGBColor);
		}
		else
		{
			WinSetForeColorRGB(&globeP->pyMatchRGBColor, &globeP->originalRGBColor);
		}
		WinDrawRectangle(&globeP->typeRect, 2);
		WinSetForeColorRGB(&globeP->originalRGBColor, NULL);
	}
	
	MemHandleUnlock(globeP->contactListH);
	globeP->contactListPP = NULL;
}

static void drawContactListByContact_PAdd(Int16 itemNum, RectangleType *bounds, Char **itemsText)
{
	UInt16				itemLength;
	UInt16				itemWidth;
	GlobeType			*globeP		= *(GlobeType **)itemsText;
	
	//锁定列表
	globeP->contactListPP = (ContactUnitType **)MemHandleLock(globeP->contactListH);
	
	//获取范围
	bounds->topLeft.x -= 2;
	bounds->extent.x += 2;
	
	if (globeP->settingP->showDetailType != NO_DETAIL)
	{
		//取记录
		getContactUnitById_PAdd(globeP, globeP->contactListPP[globeP->listMappingP[itemNum].index]->recordId);
		
		if (globeP->listMappingP[itemNum].field == 0xFF)	//姓名
		{
			buildFullName_PAdd(globeP->cache, globeP->contactUnitP);
		}
		else
		{
			StrCopy(globeP->cache, ((AddrDBRecordType *)globeP->contactUnitP)->fields[globeP->listMappingP[itemNum].field]);
			StrCat(globeP->cache, " ");
			getPhoneLabelText((AddressPhoneLabels)GetPhoneLabel((AddrDBRecordType *)globeP->contactUnitP, globeP->listMappingP[itemNum].field), (globeP->cache + StrLen(globeP->cache)));
		}
		
		itemLength = StrLen(globeP->cache);
		itemWidth = FntCharsWidth(globeP->cache, itemLength);
		
		//调整号码对齐方式
		if (globeP->listMappingP[itemNum].field == 0xFF)
		{
			if (itemWidth > bounds->extent.x - 10)
			{
				itemWidth = bounds->extent.x - 10;
			}
		}
		else
		{
			if(itemWidth < bounds->extent.x)
			{
				bounds->topLeft.x += (bounds->extent.x - itemWidth);
			}
			else
			{
				itemWidth = bounds->extent.x;
			}
		}
		
		//显示内容
		if (globeP->listMappingP[itemNum].field == 0xFF)
		{
			if (itemNum == LstGetSelection(globeP->lstContactsP))	//选中项
			{
				//显示名称
				WinDrawTruncChars(globeP->cache, itemLength, bounds->topLeft.x + 10, bounds->topLeft.y, itemWidth);
			}
			else
			{
				//修改背景颜色
				WinSetBackColorRGB(&globeP->alternateRGBColor, &globeP->originalRGBColor);
				
				//画背景
				WinEraseRectangle(bounds, 0);
				
				//显示名称
				WinDrawTruncChars(globeP->cache, itemLength, bounds->topLeft.x + 10, bounds->topLeft.y, itemWidth);
				
				//恢复背景颜色
				WinSetBackColorRGB(&globeP->originalRGBColor, NULL);
			}
			
			//画圈~~
			globeP->typeRect.topLeft.x = bounds->topLeft.x + 3;
			globeP->typeRect.topLeft.y = bounds->topLeft.y + 3;
			
			if (globeP->contactListPP[globeP->listMappingP[itemNum].index]->matchType == PHONE_MATCH_RESULT)
			{
				WinSetForeColorRGB(&globeP->phoneNumberMatchRGBColor, &globeP->originalRGBColor);
			}
			else
			{
				WinSetForeColorRGB(&globeP->pyMatchRGBColor, &globeP->originalRGBColor);
			}
			WinDrawRectangle(&globeP->typeRect, 2);
			WinSetForeColorRGB(&globeP->originalRGBColor, NULL);
		}
		else
		{
			WinDrawTruncChars(globeP->cache, itemLength, bounds->topLeft.x, bounds->topLeft.y, itemWidth);
		}
	}
	else
	{
		globeP->contactListPP[itemNum]->bounds = (*bounds);
		
		//取记录
		getContactUnitById_PAdd(globeP, globeP->contactListPP[itemNum]->recordId);
		
		//构造姓名
		buildFullName_PAdd(globeP->cache, globeP->contactUnitP);
		
		itemLength = StrLen(globeP->cache);
		itemWidth = bounds->extent.x - 10;
		
		//显示内容
		if (itemNum == LstGetSelection(globeP->lstContactsP))	//选中项
		{
			//显示名称
			WinDrawTruncChars(globeP->cache, itemLength, bounds->topLeft.x + 10, bounds->topLeft.y, itemWidth);
		}
		else	//非选中项
		{
			if (globeP->contactListPP[itemNum]->shouldChangeColor)	//改变背景颜色
			{
				//修改背景颜色
				WinSetBackColorRGB(&globeP->alternateRGBColor, &globeP->originalRGBColor);
				
				//画背景
				WinEraseRectangle(bounds, 0);
				
				//显示名称
				WinDrawTruncChars(globeP->cache, itemLength, bounds->topLeft.x + 10, bounds->topLeft.y, itemWidth);
				
				//恢复背景颜色
				WinSetBackColorRGB(&globeP->originalRGBColor, NULL);
			}
			else	//不改变背景颜色
			{
				//显示名称
				WinDrawTruncChars(globeP->cache, itemLength, bounds->topLeft.x + 10, bounds->topLeft.y, itemWidth);
			}
		}
		
		//画圈~~
		globeP->typeRect.topLeft.x = bounds->topLeft.x + 3;
		globeP->typeRect.topLeft.y = bounds->topLeft.y + 3;
		
		if (globeP->contactListPP[itemNum]->matchType == PHONE_MATCH_RESULT)
		{
			WinSetForeColorRGB(&globeP->phoneNumberMatchRGBColor, &globeP->originalRGBColor);
		}
		else
		{
			WinSetForeColorRGB(&globeP->pyMatchRGBColor, &globeP->originalRGBColor);
		}
		WinDrawRectangle(&globeP->typeRect, 2);
		WinSetForeColorRGB(&globeP->originalRGBColor, NULL);
	}
	
	MemHandleUnlock(globeP->contactListH);
	globeP->contactListPP = NULL;
}

static BitmapType *getSkinBitmap(SettingType *settingP, MemHandle *bmpH)
{
	BitmapType		*bmpP;
	BitmapTypeV3	*bmpV3P	= NULL;
	
	if ((UInt8)settingP->skin.bmpPath[0] != 0)	//自定义的皮肤
	{
		UInt16			error;
		
		bmpP = BmpCreate(settingP->skin.bmpWidth, settingP->skin.bmpHeight, 16, NULL, &error);
		
		if (error == errNone)
		{
			bmpV3P = BmpCreateBitmapV3(bmpP, settingP->skin.bmpDensity, settingP->skin.bmpBitsP, NULL);
			BmpDelete(bmpP);
			BmpSetTransparentValue((BitmapType *)bmpV3P, 0x07E0);
			//BmpSetTransparentValue((BitmapType *)bmpV3P, 0x00FF00);
		}
	}
	else
	{
		if (settingP->treoModel == TREO_600)
		{
			(*bmpH) = DmGetResource('Tbmp', bmpDefaultLow);
		}
		else
		{
			(*bmpH) = DmGetResource('Tbmp', bmpDefault);
		}
		bmpV3P = (BitmapTypeV3 *) MemHandleLock((*bmpH));
	}
	
	return (BitmapType *)bmpV3P;
}

static void createContactsForm(GlobeType *globeP)
{
	Coord			x;
	Coord			y;
	UInt16			n;
	SkinSettingType	*skin		= &globeP->settingP->skin;
	
	//创建窗体
	globeP->skinBmpP = getSkinBitmap(globeP->settingP, &globeP->skinBmpH);
	globeP->frmContactsP = FrmNewForm(frmContactsId, NULL, 0, 0, 160, 160, false, 0, 0, 0);
	
	//创建联系人列表
	LstNewList((void **)&globeP->frmContactsP, lstContactsId,
				skin->lstContacts.bounds.topLeft.x,
				skin->lstContacts.bounds.topLeft.y,
				skin->lstContacts.bounds.extent.x,
				skin->lstContacts.bounds.extent.y,
				stdFont, skin->lstContacts.other, 0);
	globeP->lstContactsP = (ListType *)getObjectPtr(globeP->frmContactsP, lstContactsId);
	globeP->lstContactsIndex = FrmGetObjectIndex(globeP->frmContactsP, lstContactsId);
	globeP->lstClip = skin->lstContacts.bounds;
	globeP->lstClip.extent.y -= 2;
	
	//号码弹出列表
	globeP->phoneNumberRect.extent.x = 104;
	if (globeP->lstClip.extent.x > 106)
	{
		globeP->phoneNumberRect.topLeft.x = globeP->lstClip.topLeft.x + globeP->lstClip.extent.x - 106;
	}
	else
	{
		globeP->phoneNumberRect.topLeft.x = globeP->lstClip.topLeft.x + 50;
	}
	
	//创建拨号按钮
	globeP->gdtDialButtonP = FrmNewGadget(&globeP->frmContactsP, gdtDialButtonId,
											skin->gdtDialButton.bounds.topLeft.x,
											skin->gdtDialButton.bounds.topLeft.y,
											skin->gdtDialButton.bounds.extent.x,
											skin->gdtDialButton.bounds.extent.y);
	
	//设置关键字区域
	globeP->PYKeywordRect.topLeft.y = globeP->lstClip.topLeft.y;
	globeP->PYKeywordRect.extent.y = 12;
	
	//创建电话号码框
	globeP->fldPhoneNumberP = FldNewField((void **)&globeP->frmContactsP, fldPhoneNumberId,
											skin->fldPhoneNumber.bounds.topLeft.x,
											skin->fldPhoneNumber.bounds.topLeft.y + skin->fldPhoneNumber.bounds.extent.y / 2 - 7,
											skin->fldPhoneNumber.bounds.extent.x,
											14,
											largeFont, 128, true, false, true, false,
											rightAlign, false, false, false);
	
	//创建电话卡触发器
	globeP->gdtPreFixTriggerP = FrmNewGadget(&globeP->frmContactsP, gdtPreFixTriggerId,
											skin->gdtPreFixTrigger.bounds.topLeft.x,
											skin->gdtPreFixTrigger.bounds.topLeft.y,
											skin->gdtPreFixTrigger.bounds.extent.x,
											skin->gdtPreFixTrigger.bounds.extent.y);
	
	//创建电话卡列表
	y = skin->gdtPreFixTrigger.bounds.topLeft.y;
	n = globeP->totalPreFix * 11 + 2;
	if (skin->gdtPreFixTrigger.bounds.topLeft.y + skin->gdtPreFixTrigger.bounds.extent.y >= n)
	{
		y = skin->gdtPreFixTrigger.bounds.topLeft.y + skin->gdtPreFixTrigger.bounds.extent.y - n;
	}
	else
	{
		y = skin->gdtPreFixTrigger.bounds.topLeft.y;
	}
	LstNewList((void **)&globeP->frmContactsP, lstPreFixId,
				skin->gdtPreFixTrigger.bounds.topLeft.x,
				y, 70, n, stdFont, globeP->totalPreFix, 0);
	globeP->lstPreFixP = (ListType *)getObjectPtr(globeP->frmContactsP, lstPreFixId);
	LstSetListChoices(globeP->lstPreFixP, globeP->preFix, globeP->totalPreFix);
	FrmHideObject(globeP->frmContactsP, FrmGetObjectIndex(globeP->frmContactsP, lstPreFixId));
	
	//创建操作按钮
	globeP->gdtOperationButtonP = FrmNewGadget(&globeP->frmContactsP, gdtOperationButtonId,
											skin->gdtOperationButton.bounds.topLeft.x,
											skin->gdtOperationButton.bounds.topLeft.y,
											skin->gdtOperationButton.bounds.extent.x,
											skin->gdtOperationButton.bounds.extent.y);
	
	//创建操作列表
	x = skin->gdtOperationButton.bounds.topLeft.x + skin->gdtOperationButton.bounds.extent.x / 2 - 30;
	n = skin->frmContacts.bounds.extent.x - 60;
	if (x > n)
	{
		x = n;
	}
	if (skin->gdtOperationButton.bounds.topLeft.y + skin->gdtOperationButton.bounds.extent.y >= 24)
	{
		y = skin->gdtOperationButton.bounds.topLeft.y + skin->gdtOperationButton.bounds.extent.y - 24;
	}
	else
	{
		y = skin->gdtOperationButton.bounds.topLeft.y;
	}
	LstNewList((void **)&globeP->frmContactsP, lstOperationId,
				x,
				y, 60, 24, stdFont, 2, 0);
	globeP->lstOperationP = (ListType *)getObjectPtr(globeP->frmContactsP, lstOperationId);
	LstSetListChoices(globeP->lstOperationP, globeP->operationPP, 2);
	FrmHideObject(globeP->frmContactsP, FrmGetObjectIndex(globeP->frmContactsP, lstOperationId));
	
	//创建短讯按钮
	globeP->gdtSMSButtonP = FrmNewGadget(&globeP->frmContactsP, gdtSMSButtonId,
											skin->gdtSMSButton.bounds.topLeft.x,
											skin->gdtSMSButton.bounds.topLeft.y,
											skin->gdtSMSButton.bounds.extent.x,
											skin->gdtSMSButton.bounds.extent.y);
											
	//创建短讯列表
	x = skin->gdtSMSButton.bounds.topLeft.x + skin->gdtSMSButton.bounds.extent.x / 2 - 40;
	n -= 20;
	if (x > n)
	{
		x = n;
	}
	if (skin->gdtSMSButton.bounds.topLeft.y + skin->gdtSMSButton.bounds.extent.y >= 35)
	{
		y = skin->gdtSMSButton.bounds.topLeft.y + skin->gdtSMSButton.bounds.extent.y - 35;
	}
	else
	{
		y = skin->gdtSMSButton.bounds.topLeft.y;
	}
	LstNewList((void **)&globeP->frmContactsP, lstSMSId,
				x,
				y, 80, 35, stdFont, 3, 0);
	globeP->lstSMSP = (ListType *)getObjectPtr(globeP->frmContactsP, lstSMSId);
	LstSetListChoices(globeP->lstSMSP, globeP->SMSPP, 3);
	FrmHideObject(globeP->frmContactsP, FrmGetObjectIndex(globeP->frmContactsP, lstSMSId));
	
	//创建帮助按钮
	globeP->gdtHelpButtonP = FrmNewGadget(&globeP->frmContactsP, gdtHelpButtonId,
											skin->gdtHelpButton.bounds.topLeft.x,
											skin->gdtHelpButton.bounds.topLeft.y,
											skin->gdtHelpButton.bounds.extent.x,
											skin->gdtHelpButton.bounds.extent.y);
	globeP->typeRect.extent.x = 4;
	globeP->typeRect.extent.y = 4;
	
	//安装菜单
	FrmSetMenu(globeP->frmContactsP, menuSpecial);
}

static RectangleType *getFocusedObjectBounds(GlobeType *globeP)
{
	switch (globeP->focusedObjectId)
	{
		case lstContactsId:
		{
			return &globeP->lstClip;
			break;
		}
		case gdtDialButtonId:
		{
			return &globeP->settingP->skin.gdtDialButton.bounds;
			break;
		}
		case fldPhoneNumberId:
		{
			return &globeP->settingP->skin.fldPhoneNumber.bounds;
			break;
		}
		case gdtPreFixTriggerId:
		{
			return &globeP->settingP->skin.gdtPreFixTrigger.bounds;
			break;
		}
		case gdtOperationButtonId:
		{
			return &globeP->settingP->skin.gdtOperationButton.bounds;
			break;
		}
		case gdtSMSButtonId:
		{
			return &globeP->settingP->skin.gdtSMSButton.bounds;
			break;
		}
		case gdtHelpButtonId:
		{
			return &globeP->settingP->skin.gdtHelpButton.bounds;
			break;
		}
	}
	
	return &globeP->settingP->skin.fldPhoneNumber.bounds;
}

/*static void drawFloatPhoneNumber(Int16 itemNum, GlobeType *globeP)
{
	if (itemNum != noListSelection)
	{
		UInt16			i;
		RectangleType	*bounds;
		Char			*phoneNumberP	= NULL;
		
		if (globeP->contactCount > 0)
		{
			globeP->contactListPP = (void **)MemHandleLock(globeP->contactListH);
			
			if (globeP->settingP->treoModel == TREO_600 || globeP->settingP->treoModel == NOT_A_TREO)
			{
				for (i = OLD_phone1; i < OLD_phone5; i ++)
				{
					if (((ContactUnitType_Addr **)globeP->contactListPP)[itemNum]->data.fields[i])
					{
						phoneNumberP = ((ContactUnitType_Addr **)globeP->contactListPP)[itemNum]->data.fields[i];
						bounds = &((ContactUnitType_Addr **)globeP->contactListPP)[itemNum]->bounds;
						break;
					}
				}
			}
			else
			{
				for (i = phone1; i < phone7; i ++)
				{
					if (((ContactUnitType_PAdd **)globeP->contactListPP)[itemNum]->data.fields[i])
					{
						phoneNumberP = ((ContactUnitType_PAdd **)globeP->contactListPP)[itemNum]->data.fields[i];
						bounds = &((ContactUnitType_PAdd **)globeP->contactListPP)[itemNum]->bounds;
						break;
					}
				}
			}
			
			MemHandleUnlock(globeP->contactListH);
	globeP->contactListPP = NULL;
		}
		else
		{
			phoneNumberP = globeP->recentListP[itemNum].phoneNumber;
			bounds = &globeP->recentListP[itemNum].bounds;
		}
		
		if (phoneNumberP)
		{
			RectangleType	floatRect;
			Int16			topItem				= LstGetTopItem(globeP->lstContactsP);
			Int16			bottomItem			= topItem + globeP->settingP->skin.lstContacts.other - 1;
			UInt16			phoneNumberLength	= StrLen(phoneNumberP);
			UInt16			phoneNumberWidth	= FntCharsWidth(phoneNumberP, phoneNumberLength);
			
			//计算浮动框的宽度
			if (phoneNumberWidth > 71)
			{
				floatRect.extent.x = phoneNumberWidth + 4;
			}
			else
			{
				floatRect.extent.x = 71;
			}
			
			//计算浮动框的左x值
			if (bounds->extent.x > floatRect.extent.x)
			{
				floatRect.topLeft.x = bounds->topLeft.x + bounds->extent.x - floatRect.extent.x;
				
				if ((itemNum == topItem && topItem != 0) || (itemNum == bottomItem && bottomItem != LstGetNumberOfItems(globeP->lstContactsP) - 1))
				{
					floatRect.topLeft.x += 8;
				}
			}
			else
			{
				floatRect.topLeft.x = bounds->topLeft.x + 48;
			}
			
			//计算浮动框的左y值
			if (itemNum == bottomItem)	//是最后一行
			{
				//显示号码在该行的上部
				floatRect.topLeft.y = bounds->topLeft.y - 10;
			}
			else
			{
				//显示号码在该行的下部
				floatRect.topLeft.y = bounds->topLeft.y + 9;
			}
			
			//计算浮动框的高度
			floatRect.extent.y = 12;
			
			//更改颜色
			WinSetBackColorRGB(&globeP->highlightBGRGBColor, &globeP->originalRGBColor);
			WinSetForeColorRGB(&globeP->highlightTextRGBColor, &globeP->originalTextRGBColor);
			
			//画框
			WinEraseRectangle(&floatRect, 2);
			//WinDrawRectangleFrame(roundFrame, &floatRect);
			
			//画电话号码
			WinDrawChars(phoneNumberP, phoneNumberLength, floatRect.topLeft.x + (floatRect.extent.x - phoneNumberWidth) / 2, floatRect.topLeft.y + 1);
			
			//恢复颜色
			WinSetForeColorRGB(&globeP->originalTextRGBColor, NULL);
			WinSetBackColorRGB(&globeP->originalRGBColor, NULL);
		}
	}
}*/

static void drawPYKeyword(GlobeType *globeP)
{
	if (LstGetSelection(globeP->lstContactsP) == noListSelection && globeP->pyKeywordLength > 0)
	{
		UInt16		keywordWidth	= FntCharsWidth(globeP->pyKeyword, globeP->pyKeywordLength);
		//计算区域
		globeP->PYKeywordRect.extent.x = keywordWidth + 4;
		if (globeP->PYKeywordRect.extent.x < 20)
		{
			globeP->PYKeywordRect.extent.x = 20;
		}
		globeP->PYKeywordRect.topLeft.x = globeP->lstClip.topLeft.x + globeP->lstClip.extent.x - globeP->PYKeywordRect.extent.x - 2;
		
		//更改背景颜色
		WinSetBackColorRGB(&globeP->alternateRGBColor, &globeP->originalRGBColor);
		
		//画关键字托盘
		WinEraseRectangle(&globeP->PYKeywordRect, 2);
		WinDrawRectangleFrame(popupFrame, &globeP->PYKeywordRect);
		
		//画关键字
		WinDrawChars(globeP->pyKeyword, globeP->pyKeywordLength, globeP->PYKeywordRect.topLeft.x + (globeP->PYKeywordRect.extent.x - keywordWidth) / 2/*globeP->PYKeywordRect.topLeft.x + 2*/, globeP->PYKeywordRect.topLeft.y);
		
		//恢复背景颜色
		WinSetBackColorRGB(&globeP->originalRGBColor, NULL);
	}
}

static void updateContactsForm(GlobeType *globeP, UInt16 updateCode)
{
	UInt16			preFixLength;
	UInt16			n;
	SkinSettingType	*skin			= &globeP->settingP->skin;
	
	switch (updateCode)
	{
		case frmRedrawUpdateCode:
		{
			//画窗体
			FrmDrawForm(globeP->frmContactsP);
			WinCopyRectangle(globeP->bgBitsWinH, (WinHandle)globeP->frmContactsP, &globeP->bgRect, 0, 0, winPaint);
			WinDrawBitmap(globeP->skinBmpP, skin->frmContacts.bounds.topLeft.x,
									skin->frmContacts.bounds.topLeft.y);
			
			//画联系人列表
			setListFrameColor(true, globeP);	//设置成白色
			LstDrawList(globeP->lstContactsP);
			
			//画号码框
			FldDrawField(globeP->fldPhoneNumberP);
			
			/*//画电话卡触发器
			preFixLength = StrLen(globeP->preFix[globeP->currentPreFix]);
			n = FntCharsWidth(globeP->preFix[globeP->currentPreFix], preFixLength);
			if (n > skin->gdtPreFixTrigger.bounds.extent.x - 1)
			{
				n = skin->gdtPreFixTrigger.bounds.topLeft.x;
			}
			else
			{
				n = skin->gdtPreFixTrigger.bounds.topLeft.x + (skin->gdtPreFixTrigger.bounds.extent.x - n) - 1;
			}
			WinSetDrawMode(winOverlay);
			WinPaintChars(globeP->preFix[globeP->currentPreFix], preFixLength, n, skin->gdtPreFixTrigger.bounds.topLeft.y);*/
			break;
		}
		case FORM_UPDATE_CUSTOM:
		{
			//画联系人列表
			LstDrawList(globeP->lstContactsP);
			
			//画号码框
			FldDrawField(globeP->fldPhoneNumberP);
			
			/*//画电话卡触发器
			preFixLength = StrLen(globeP->preFix[globeP->currentPreFix]);
			WinSetDrawMode(winOverlay);
			WinPaintChars(globeP->preFix[globeP->currentPreFix], preFixLength, skin->gdtPreFixTrigger.bounds.topLeft.x + 21, skin->gdtPreFixTrigger.bounds.topLeft.y);*/
			break;
		}
		case FORM_UPDATE_REFLASH:
		{
			WinCopyRectangle(globeP->bgBitsWinH, (WinHandle)globeP->frmContactsP, &globeP->bgRect, 0, 0, winPaint);
			WinDrawBitmap(globeP->skinBmpP, skin->frmContacts.bounds.topLeft.x,
									skin->frmContacts.bounds.topLeft.y);
			
			//画联系人列表
			LstDrawList(globeP->lstContactsP);
			
			//画号码框
			FldDrawField(globeP->fldPhoneNumberP);
			
			/*//画电话卡触发器
			preFixLength = StrLen(globeP->preFix[globeP->currentPreFix]);
			WinSetDrawMode(winOverlay);
			WinPaintChars(globeP->preFix[globeP->currentPreFix], preFixLength, skin->gdtPreFixTrigger.bounds.topLeft.x + 21, skin->gdtPreFixTrigger.bounds.topLeft.y);*/
			break;
		}
		case FORM_UPDATE_REFLASH2:
		{
			WinDrawBitmap(globeP->skinBmpP, skin->frmContacts.bounds.topLeft.x,
									skin->frmContacts.bounds.topLeft.y);
			
			//画联系人列表
			LstDrawList(globeP->lstContactsP);
			
			//画号码框
			FldDrawField(globeP->fldPhoneNumberP);
			
			/*//画电话卡触发器
			preFixLength = StrLen(globeP->preFix[globeP->currentPreFix]);
			WinSetDrawMode(winOverlay);
			WinPaintChars(globeP->preFix[globeP->currentPreFix], preFixLength, skin->gdtPreFixTrigger.bounds.topLeft.x + 21, skin->gdtPreFixTrigger.bounds.topLeft.y);*/
			break;
		}
	}
	
	//画电话卡触发器
	preFixLength = StrLen(globeP->preFix[globeP->currentPreFix]);
	n = FntCharsWidth(globeP->preFix[globeP->currentPreFix], preFixLength);
	if (n > skin->gdtPreFixTrigger.bounds.extent.x - 1)
	{
		n = skin->gdtPreFixTrigger.bounds.topLeft.x;
	}
	else
	{
		n = skin->gdtPreFixTrigger.bounds.topLeft.x + (skin->gdtPreFixTrigger.bounds.extent.x - n) - 1;
	}
	WinSetDrawMode(winOverlay);
	WinPaintChars(globeP->preFix[globeP->currentPreFix], preFixLength, n, skin->gdtPreFixTrigger.bounds.topLeft.y);
	
	//显示关键字
	drawPYKeyword(globeP);
}

#if 0
#pragma mark == POPUP EVENT HANDLER ==
#endif

static UInt8 popupEventHandler(ListType *lstPopupP, GlobeType *globeP, Int16 item, Char **phoneNumberPP)
{
	EventType		event;
	Boolean			isKeyHandled		= false;
	Boolean			exit				= false;
	UInt8			actionCode			= NO_EXIT;
	Int8			selectIndex			= (Int8)LstGetSelection(lstPopupP);
	
	do
	{
		EvtGetEvent(&event, evtWaitForever);
		
		if (event.eType == keyDownEvent)
		{
			switch (keyTranslator(&event))
			{
				case chrUpArrow:	//向上，滚动列表
				case keyPageUp:
				case keyRockerUp:
				{
					if (selectIndex > 0)
					{
						selectIndex --;
						LstSetSelection(lstPopupP, selectIndex);
					}
					
					isKeyHandled = true;
					break;
				}
				case chrDownArrow:	//向下，滚动列表
				case keyPageDown:
				case keyRockerDown:
				{
					if (selectIndex < LstGetNumberOfItems(lstPopupP) - 1)
					{
						selectIndex ++;
						LstSetSelection(lstPopupP, selectIndex);
					}
					
					isKeyHandled = true;
					break;
				}
				case 317:
				{
					isKeyHandled = true;
				}
				case vchrRockerCenter:	//中间键
				{
					if ((! (event.data.keyDown.modifiers & willSendUpKeyMask)) || globeP->settingP->treoModel == TREO_600)
					{
						if (getSelectedPhoneLabel(globeP, item, getSelectedPhoneField(item, selectIndex, globeP)) == emailLabel)
						{
							actionCode = EXIT_FOR_EMAIL;
						}
						else
						{
							actionCode = EXIT_FOR_DIAL;
						}
						exit = true;
					}
					isKeyHandled = true;
					break;
				}
				case keyHard1:			//电话键
				case TREO_680_GREEN_KEY:
				{
					actionCode = EXIT_FOR_DIRECT_DIAL;
					exit = true;
					isKeyHandled = true;
					break;
				}
				case keyHard3:			//短讯键
				{
					if (getSelectedPhoneLabel(globeP, item, getSelectedPhoneField(item, selectIndex, globeP)) == emailLabel)
					{
						actionCode = EXIT_FOR_EMAIL;
					}
					else
					{
						actionCode = EXIT_FOR_SYSTEM_MESSAGE;
					}
					exit = true;
					isKeyHandled = true;
					break;
				}
				case keySpace:		//空格键
				{
					if (globeP->currentPreFix < globeP->totalPreFix - 1)
					{
						globeP->currentPreFix ++;
					}
					else
					{
						globeP->currentPreFix = 0;
					}
					WinDrawBitmap(globeP->skinBmpP, globeP->settingP->skin.frmContacts.bounds.topLeft.x,
									globeP->settingP->skin.frmContacts.bounds.topLeft.y);
					//画电话卡触发器
					WinSetDrawMode(winOverlay);
					WinPaintChars(globeP->preFix[globeP->currentPreFix], StrLen(globeP->preFix[globeP->currentPreFix]), globeP->settingP->skin.gdtPreFixTrigger.bounds.topLeft.x + 21, globeP->settingP->skin.gdtPreFixTrigger.bounds.topLeft.y);
							
					isKeyHandled = true;
					break;
				}
				default:
				{
					exit = true;
					isKeyHandled = true;
					break;
				}
			}
		}
		
		
		
		if (! isKeyHandled)
		{
			if (! SysHandleEvent(&event))
			{
				if (event.eType == lstSelectEvent)
				{
					if (event.data.lstSelect.listID == lstPopupId)
					{
						if (getSelectedPhoneLabel(globeP, item, getSelectedPhoneField(item, event.data.lstSelect.selection, globeP)) == emailLabel)
						{
							actionCode = EXIT_FOR_EMAIL;
						}
						else
						{
							actionCode = EXIT_FOR_DIAL;
						}
						exit = true;
					}
				}
				else if (event.eType == penUpEvent)
				{
					if (! RctPtInRectangle(event.data.penUp.end.x, event.data.penUp.end.y, &globeP->phoneNumberRect))
					{
						exit = true;
					}
					else
					{
						event.eType = nilEvent;
					}
				}
				else if (event.eType == penDownEvent && (! RctPtInRectangle(event.screenX, event.screenY, &globeP->phoneNumberRect)))
				{
					event.eType = nilEvent;
				}
				else
				{
					FrmDispatchEvent(&event);
				}
			}
		}
		else
		{
			MemSet(&event, sizeof(EventType), 0);
			event.eType = nilEvent;
			isKeyHandled = false;
		}
	} while (event.eType != appStopEvent && exit == false);
	
	return actionCode;
}

static void drawPopupPhoneNumberList(Int16 itemNum, RectangleType *bounds, Char **itemsText)
{
	UInt16		itemLength	= StrLen(itemsText[itemNum]);
	UInt16		itemWidth	= FntCharsWidth(itemsText[itemNum], itemLength);
	UInt16		x, width;
	
	if (itemWidth <= bounds->extent.x)
	{
		x = bounds->topLeft.x + bounds->extent.x - itemWidth;
		width = itemWidth;
	}
	else
	{
		x = bounds->topLeft.x;
		width = bounds->extent.x;
	}
	
	WinDrawTruncChars(itemsText[itemNum], itemLength, x, bounds->topLeft.y, width);
}

static UInt8 popupPhoneNumberList(Int16 index, GlobeType *globeP)
{
	UInt8			i;
	UInt8			actionCode			= NO_EXIT;
	UInt16			phoneNumberCount	= 0;
	ListType		*lstPopupP			= NULL;
	Char			**phoneNumberListPP	= MemPtrNew(28);	//最多7个电话号码
	
	globeP->contactListPP = (ContactUnitType **)MemHandleLock(globeP->contactListH);
	
	if (globeP->settingP->treoModel == TREO_600 || globeP->settingP->treoModel == NOT_A_TREO)
	{
		getContactUnitById_Addr(globeP, globeP->contactListPP[index]->recordId);
		for (i = OLD_phone1; i <= OLD_phone5; i ++)
		{
			if (((OLD_AddrDBRecordType *)globeP->contactUnitP)->fields[i])
			{
				//取号码
				phoneNumberListPP[phoneNumberCount] = (Char *)MemPtrNew(StrLen(((OLD_AddrDBRecordType *)globeP->contactUnitP)->fields[i]) + 5);
				StrCopy(phoneNumberListPP[phoneNumberCount], ((OLD_AddrDBRecordType *)globeP->contactUnitP)->fields[i]);
				
				//取标签
				getPhoneLabelText((OLD_AddressPhoneLabels)OLD_GetPhoneLabel((OLD_AddrDBRecordType *)globeP->contactUnitP, i), globeP->cache);
				StrCat(phoneNumberListPP[phoneNumberCount], " ");
				StrCat(phoneNumberListPP[phoneNumberCount], globeP->cache);
				
				phoneNumberCount ++;
			}
		}
	}
	else
	{
		getContactUnitById_PAdd(globeP, globeP->contactListPP[index]->recordId);
		for (i = phone1; i <= phone7; i ++)
		{
			if (((AddrDBRecordType *)globeP->contactUnitP)->fields[i])
			{
				//取号码
				phoneNumberListPP[phoneNumberCount] = (Char *)MemPtrNew(StrLen(((AddrDBRecordType *)globeP->contactUnitP)->fields[i]) + 5);
				StrCopy(phoneNumberListPP[phoneNumberCount], ((AddrDBRecordType *)globeP->contactUnitP)->fields[i]);
				
				//取标签
				getPhoneLabelText((AddressPhoneLabels)GetPhoneLabel((AddrDBRecordType *)globeP->contactUnitP, i), globeP->cache);
				StrCat(phoneNumberListPP[phoneNumberCount], " ");
				StrCat(phoneNumberListPP[phoneNumberCount], globeP->cache);
				
				phoneNumberCount ++;
			}
		}
	}
	
	//计算弹出列表的位置基点
	globeP->phoneNumberRect.topLeft.y = globeP->contactListPP[index]->bounds.topLeft.y + 5;
	
	MemHandleUnlock(globeP->contactListH);
	globeP->contactListPP = NULL;
	
	if (phoneNumberCount > 0)
	{
		//计算弹出列表的高度
		globeP->phoneNumberRect.extent.y = phoneNumberCount * 11 + 4;
		
		//调整位置
		globeP->phoneNumberRect.topLeft.y -= (globeP->phoneNumberRect.extent.y / 2);
		if (globeP->phoneNumberRect.topLeft.y < globeP->lstClip.topLeft.y)
		{
			globeP->phoneNumberRect.topLeft.y = globeP->lstClip.topLeft.y;
		}
		else if (globeP->phoneNumberRect.topLeft.y + globeP->phoneNumberRect.extent.y > globeP->lstClip.topLeft.y + globeP->lstClip.extent.y - 2)
		{
			globeP->phoneNumberRect.topLeft.y = globeP->lstClip.topLeft.y + globeP->lstClip.extent.y - globeP->phoneNumberRect.extent.y - 2;
		}
		
		//画列表框底
		WinEraseRectangle(&globeP->phoneNumberRect, 2);
		WinDrawRectangleFrame(popupFrame, &globeP->phoneNumberRect);
		
		//生成列表
		LstNewList((void **)&globeP->frmContactsP, lstPopupId,
					globeP->phoneNumberRect.topLeft.x + 2,
					globeP->phoneNumberRect.topLeft.y + 2,
					100,
					globeP->phoneNumberRect.extent.y - 4,
					stdFont, phoneNumberCount, 0);
		
		//更新指针
		updateFormControlPtr(globeP);
		
		//弹出列表
		lstPopupP = (ListType *)getObjectPtr(globeP->frmContactsP, lstPopupId);
		LstSetDrawFunction(lstPopupP, drawPopupPhoneNumberList);
		LstSetListChoices(lstPopupP, phoneNumberListPP, phoneNumberCount);
		LstDrawList(lstPopupP);
		LstSetSelection(lstPopupP, 0);
		
		actionCode = popupEventHandler(lstPopupP, globeP, index, phoneNumberListPP);
		
		if (actionCode != NO_EXIT)
		{
			Int16			selection			= LstGetSelection(lstPopupP);
			UInt16			phoneNumberLength	= StrLen(phoneNumberListPP[selection]) - 4;
			
			//拷贝到文本框
			FldDelete(globeP->fldPhoneNumberP, 0, 128);
			
			FldInsert(globeP->fldPhoneNumberP, phoneNumberListPP[selection], phoneNumberLength);
			FldSetSelection(globeP->fldPhoneNumberP, 0, phoneNumberLength);
			
			//记录下被选定的电话项
			globeP->currentField = getSelectedPhoneField(index, LstGetSelection(lstPopupP), globeP);
		}
		
		//清除列表
		LstEraseList(lstPopupP);
		LstSetListChoices(lstPopupP, NULL, 0);
		FrmRemoveObject(&globeP->frmContactsP, FrmGetObjectIndexFromPtr(globeP->frmContactsP, lstPopupP));
		
		//更新指针
		updateFormControlPtr(globeP);
	}
	
	for (i = 0; i < phoneNumberCount; i ++)
	{
		MemPtrFree(phoneNumberListPP[i]);
	}
	MemPtrFree(phoneNumberListPP);
	
	return actionCode;
}

#if 0
#pragma mark == KEY EVENT HANDLER ==
#endif

static void addKeyToKeyword(WChar chr, GlobeType *globeP)
{
	if (chr == keyBackspace)
	{
		if (globeP->pyKeywordLength > 0)
		{
			globeP->pyKeywordLength --;
			globeP->pyKeyword[globeP->pyKeywordLength] = '\0';
		}
	}
	else
	{
		if (globeP->pyKeywordLength < 10)
		{
			//添加到关键字
			globeP->pyKeyword[globeP->pyKeywordLength] = (Char)chr;
			globeP->pyKeywordLength ++;
		}
	}
}

static Boolean keyHandleEvent(EventType *eventP, GlobeType *globeP, UInt8 *exitCodeP, Boolean *needResetListDrawFunc)
{
	Boolean		isKeyHandled	= false;
	Boolean		shouldPopupList	= false;
	
	if (eventP->eType == keyDownEvent && globeP->isMenuNotActive)
	{
		Int16		i, n;
		UInt16		phoneNumberLength;
		UInt16		updateCode			= FORM_NO_UPDATE;
		WChar		chr					= keyTranslator(eventP);
		
		isKeyHandled = true;
		
		switch (chr)
		{
			case chrUpArrow:	//向上，滚动列表
			case keyPageUp:
			case keyRockerUp:
			{
				//获取列表项数
				n = LstGetNumberOfItems(globeP->lstContactsP);
				
				if (n > 0)
				{
					if (globeP->focusedObjectId != lstContactsId)	//焦点不在列表
					{
						//调整焦点
						globeP->focusedObjectId = lstContactsId;
						globeP->isFocusRingDrawed = false;
					}
					
					//获取当前选中项
					i = LstGetSelection(globeP->lstContactsP);
					
					//确定是否需要重绘列表
					if (i == LstGetTopItem(globeP->lstContactsP))
					{
						//在最后一行，需要重绘
						updateCode = FORM_UPDATE_CUSTOM;
					}
					
					//调整选中项
					if (i == noListSelection || i == 0)
					{
						i = n - 1;
						
						//跳转到最后一行，需要重绘
						updateCode = FORM_UPDATE_CUSTOM;
					}
					else
					{
						i --;
					}
					LstSetSelection(globeP->lstContactsP, i);
					
					//把默认号码拷贝到电话号码框
					copyDefaultPhoneNumber(i, globeP);
				}
				break;
			}
			case chrDownArrow:	//向下，滚动列表
			case keyPageDown:
			case keyRockerDown:
			{
				//获取列表项数
				n = LstGetNumberOfItems(globeP->lstContactsP);
				
				if (n > 0)
				{
					if (globeP->focusedObjectId != lstContactsId)	//焦点不在列表
					{
						//调整焦点
						globeP->focusedObjectId = lstContactsId;
						globeP->isFocusRingDrawed = false;
					}
					
					//获取当前选中项
					i = LstGetSelection(globeP->lstContactsP);
					
					//确定是否需要重绘列表
					if (i - LstGetTopItem(globeP->lstContactsP) + 1 == globeP->settingP->skin.lstContacts.other)
					{
						//在最后一行，需要重绘
						updateCode = FORM_UPDATE_CUSTOM;
					}
					
					if (i == noListSelection || i + 1 == n)
					{
						i = 0;
						
						//回溯至第一行，需要重绘
						updateCode = FORM_UPDATE_CUSTOM;
					}
					else
					{
						i ++;
					}
					LstSetSelection(globeP->lstContactsP, i);
					
					//把默认号码拷贝到电话号码框
					copyDefaultPhoneNumber(i, globeP);
				}
				
				break;
			}
			case chrLeftArrow:	//向左，移动焦点
			case keyRockerLeft:
			{
				globeP->isFocusRingDrawed = false;
				updateCode = FORM_UPDATE_CUSTOM;
				switch (globeP->focusedObjectId)
				{
					case gdtHelpButtonId:
					{
						globeP->focusedObjectId = gdtSMSButtonId;
						break;
					}
					case gdtSMSButtonId:
					{
						globeP->focusedObjectId = gdtOperationButtonId;
						break;
					}
					case gdtOperationButtonId:
					{
						globeP->focusedObjectId = gdtPreFixTriggerId;
						break;
					}
					case gdtPreFixTriggerId:
					{
						globeP->focusedObjectId = fldPhoneNumberId;
						break;
					}
					case fldPhoneNumberId:
					{
						if (isFieldTextSelected(globeP->fldPhoneNumberP))
						{
							FldSetSelection(globeP->fldPhoneNumberP, 0, 0);
							globeP->isFocusRingDrawed = true;
						}
						else if(FldGetInsPtPosition(globeP->fldPhoneNumberP) == 0)
						{
							globeP->focusedObjectId = gdtDialButtonId;
						}
						else
						{
							globeP->isFocusRingDrawed = true;
							isKeyHandled = false;
							updateCode = FORM_NO_UPDATE;
						}
						break;
					}
					case lstContactsId:
					case gdtDialButtonId:
					{
						globeP->focusedObjectId = gdtHelpButtonId;
						break;
					}
				}
				break;
			}
			case chrRightArrow:	//向右，移动焦点
			case keyRockerRight:
			{
				globeP->isFocusRingDrawed = false;
				updateCode = FORM_UPDATE_CUSTOM;
				switch (globeP->focusedObjectId)
				{
					case lstContactsId:
					case gdtHelpButtonId:
					{
						globeP->focusedObjectId = gdtDialButtonId;
						break;
					}
					case gdtDialButtonId:
					{
						globeP->focusedObjectId = fldPhoneNumberId;
						break;
					}
					case fldPhoneNumberId:
					{
						phoneNumberLength = FldGetTextLength(globeP->fldPhoneNumberP);
						
						if (isFieldTextSelected(globeP->fldPhoneNumberP))
						{
							FldSetSelection(globeP->fldPhoneNumberP, 0, 0);
							globeP->isFocusRingDrawed = true;
						}
						else if(FldGetInsPtPosition(globeP->fldPhoneNumberP) == phoneNumberLength)
						{
							globeP->focusedObjectId = gdtPreFixTriggerId;
						}
						else
						{
							globeP->isFocusRingDrawed = true;
							isKeyHandled = false;
							updateCode = FORM_NO_UPDATE;
						}
						break;
					}
					case gdtPreFixTriggerId:
					{
						globeP->focusedObjectId = gdtOperationButtonId;
						break;
					}
					case gdtOperationButtonId:
					{
						globeP->focusedObjectId = gdtSMSButtonId;
						break;
					}
					case gdtSMSButtonId:
					{
						globeP->focusedObjectId = gdtHelpButtonId;
						break;
					}
				}
				break;
			}
			case 0x0128:	//检测长按中间键
			{
				if (FldGetTextLength(globeP->fldPhoneNumberP) > 0 && globeP->settingP->showDetailType == NO_DETAIL)
				{
					(*exitCodeP) = EXIT_FOR_DIAL;
				}
				break;
			}
			case keyRockerCenter:	//中间键
			{
				if ((! (eventP->data.keyDown.modifiers & willSendUpKeyMask)) || globeP->settingP->treoModel == TREO_600)
				{
					switch (globeP->focusedObjectId)
					{
						case lstContactsId:			//打开联系人细节
						{
							if (globeP->contactCount > 0)
							{
								if (globeP->settingP->showDetailType == NO_DETAIL)
								{
									shouldPopupList = true;
								}
								else
								{
									if (globeP->listMappingP[LstGetSelection(globeP->lstContactsP)].field == 0xFF)
									{
										//打开联系人
										(*exitCodeP) = EXIT_FOR_OPEN_CONTACT;
									}
									else
									{
										(*exitCodeP) = EXIT_FOR_DIAL;
									}
								}
							}
							else if (globeP->recentCount > 0)
							{
								(*exitCodeP) = EXIT_FOR_DIAL;
							}
							break;
						}
						case gdtDialButtonId:		//拨号
						{
							if (FldGetTextLength(globeP->fldPhoneNumberP) > 0)
							{
								(*exitCodeP) = EXIT_FOR_DIAL;
							}
							break;
						}
						case fldPhoneNumberId:		//移动焦点至拨号按钮
						{
							if (isFieldTextSelected(globeP->fldPhoneNumberP))
							{
								FldSetSelection(globeP->fldPhoneNumberP, 0, 0);
							}
							else
							{
								globeP->focusedObjectId = gdtDialButtonId;
								globeP->isFocusRingDrawed = false;
							}
							updateCode = FORM_UPDATE_CUSTOM;
							break;
						}
						case gdtPreFixTriggerId:	//打开电话卡列表
						{
							HsNavRemoveFocusRing(globeP->frmContactsP);
							setListFrameColor(false, globeP);
							if (LstPopupList(globeP->lstPreFixP) != noListSelection)
							{
								globeP->currentPreFix = LstGetSelection(globeP->lstPreFixP);
							}
							setListFrameColor(true, globeP);
							updateCode = FORM_UPDATE_REFLASH;
							globeP->isFocusRingDrawed = false;
							break;
						}
						case gdtOperationButtonId:	//打开其他操作列表
						{
							if (FldGetTextLength(globeP->fldPhoneNumberP) > 0)
							{
								setListFrameColor(false, globeP);
								switch (LstPopupList(globeP->lstOperationP))
								{
									case 0:
									{
										(*exitCodeP) = EXIT_FOR_ADD_CONTACT;
										break;
									}
									case 1:
									{
										(*exitCodeP) = EXIT_FOR_OPEN_CONTACT;
										break;
									}
								}
								setListFrameColor(true, globeP);
							}
							break;
						}
						case gdtSMSButtonId:		//打开短讯操作列表
						{
							if (FldGetTextLength(globeP->fldPhoneNumberP) > 0)
							{
								setListFrameColor(false, globeP);
								switch (LstPopupList(globeP->lstSMSP))
								{
									case 0:
									{
										(*exitCodeP) = EXIT_FOR_SYSTEM_MESSAGE;
										break;
									}
									case 1:
									{
										(*exitCodeP) = EXIT_FOR_MESSAGE;
										break;
									}
									case 2:
									{
										(*exitCodeP) = EXIT_FOR_EMAIL;
										break;
									}
								}
								setListFrameColor(true, globeP);
							}
							break;
						}
						case gdtHelpButtonId:		//打开帮助
						{
							FrmHelp(stringTip);
							updateCode = FORM_UPDATE_REFLASH;
							globeP->isFocusRingDrawed = false;
							break;
						}
					}
				}
				else
				{
					isKeyHandled = false;
				}
				break;
			}
			case TREO_680_GREEN_KEY:	//直接拨号
			case keyHard1:
			{
				if (FldGetTextLength(globeP->fldPhoneNumberP) > 0)
				{
					(*exitCodeP) = EXIT_FOR_DIRECT_DIAL;
				}
				break;
			}
			case keyHard2:			//保存到联系人
			{
				if (globeP->settingP->keyHard2Action == KEY_HARD2_SAVE_CONTACT && FldGetTextLength(globeP->fldPhoneNumberP) > 0)
				{
					(*exitCodeP) = EXIT_FOR_ADD_CONTACT;
				}
				else if (LstGetSelection(globeP->lstContactsP) != noListSelection && globeP->contactCount > 0)
				{
					(*exitCodeP) = EXIT_FOR_OPEN_CONTACT;
				}
				break;
			}
			case keyHard3:			//发送短讯或电子邮件
			{
				i = LstGetSelection(globeP->lstContactsP);
				if (i != noListSelection)
				{
					if (globeP->contactCount > 0)
					{
						if (getSelectedPhoneLabel(globeP, i, getSelectedPhoneField(i, 0, globeP)) != emailLabel)	//发短讯
						{
							if (globeP->settingP->keyHard3Action == KEY_HARD3_SYSTEM_MESSAGE)
							{
								(*exitCodeP) = EXIT_FOR_SYSTEM_MESSAGE;
							}
							else
							{
								(*exitCodeP) = EXIT_FOR_MESSAGE;
							}
						}
						else	//发邮件
						{
							(*exitCodeP) = EXIT_FOR_EMAIL;
						}
					}
					else
					{
						if (globeP->settingP->keyHard3Action == KEY_HARD3_SYSTEM_MESSAGE)
						{
							(*exitCodeP) = EXIT_FOR_SYSTEM_MESSAGE;
						}
						else
						{
							(*exitCodeP) = EXIT_FOR_MESSAGE;
						}
					}
				}
				else if (FldGetTextLength(globeP->fldPhoneNumberP) > 0)
				{
					if (globeP->settingP->keyHard3Action == KEY_HARD3_SYSTEM_MESSAGE)
					{
						(*exitCodeP) = EXIT_FOR_SYSTEM_MESSAGE;
					}
					else
					{
						(*exitCodeP) = EXIT_FOR_MESSAGE;
					}
				}
				break;
			}
			case keySpace:		//空格
			{
				if (globeP->currentPreFix < globeP->totalPreFix - 1)
				{
					globeP->currentPreFix ++;
				}
				else
				{
					globeP->currentPreFix = 0;
				}
				updateCode = FORM_UPDATE_REFLASH2;
				break;
			}
			case keyBackspace:		//删除输入的字符
			{
				//取消列表的选择
				LstSetSelection(globeP->lstContactsP, noListSelection);
				
				if ((eventP->data.keyDown.modifiers & autoRepeatKeyMask))	//长按，清空所有内容
				{
					//电话号码
					n = FldGetTextLength(globeP->fldPhoneNumberP);
					if (n > 0)
					{
						FldDelete(globeP->fldPhoneNumberP, 0, n);
					}
					
					//关键字
					if (globeP->pyKeywordLength > 0)
					{
						MemSet(globeP->pyKeyword, 11, 0);
						globeP->pyKeywordLength = 0;
					}
					
					//联系人
					if (globeP->contactCount > 0)
					{
						freeContactList(globeP);
					}
					
					globeP->currentField = 0;
					updateCode = FORM_UPDATE_CUSTOM;
					globeP->focusedObjectId = fldPhoneNumberId;
				}
				else
				{
					//调整关键字，并检测是否应该退出
					n = FldGetTextLength(globeP->fldPhoneNumberP);
					if (n > 0 || globeP->pyKeywordLength > 0)
					{
						if (globeP->focusedObjectId == fldPhoneNumberId)
						{
							isKeyHandled = FldHandleEvent(globeP->fldPhoneNumberP, eventP);
						}
						else if (globeP->pyKeywordLength == 0)
						{
							globeP->focusedObjectId = fldPhoneNumberId;
							isKeyHandled = FldHandleEvent(globeP->fldPhoneNumberP, eventP);
						}
						//isKeyHandled = FldHandleEvent(globeP->fldPhoneNumberP, eventP);
						
						/*if (n > 0)
						{
							if (isFieldTextSelected(globeP->fldPhoneNumberP))
							{
								FldDelete(globeP->fldPhoneNumberP, 0, n);
								n = 0;
								if (globeP->focusedObjectId != fldPhoneNumberId)
								{
									globeP->focusedObjectId = fldPhoneNumberId;
									globeP->isFocusRingDrawed = false;
								}
							}
							else
							{
								FldDelete(globeP->fldPhoneNumberP, n - 1, n);
								n --;
							}
						}*/
						
						addKeyToKeyword(chr, globeP);
						
						if (globeP->pyKeywordLength > 0 || n >= 3)
						{
							//检索通讯录
							(*(globeP->getContactListP))(globeP->CNLBGlobeP, globeP->CNLBparamP, globeP);
						}
						else
						{
							freeContactList(globeP);
							
							if (globeP->focusedObjectId != fldPhoneNumberId && n == 0)
							{
								globeP->focusedObjectId = fldPhoneNumberId;
								globeP->isFocusRingDrawed = false;
							}
						}
						
						globeP->currentField = 0;
						updateCode = FORM_UPDATE_CUSTOM;
					}
					else
					{
						(*exitCodeP) = EXIT_FOR_NOTHING;
					}
				}
				
				(*needResetListDrawFunc) = true;
				break;
			}
			default:				//添加到关键字并进行检索
			{
				if ((chr >= keyA && chr <= keyZ) || chr == keyZero)
				{
					//取消列表的选择
					LstSetSelection(globeP->lstContactsP, noListSelection);
					
					//添加到关键字
					addKeyToKeyword(chr, globeP);
					
					if (globeP->focusedObjectId == fldPhoneNumberId)
					{
						FldHandleEvent(globeP->fldPhoneNumberP, eventP);
					}
					
					//检索通讯录
					(*(globeP->getContactListP))(globeP->CNLBGlobeP, globeP->CNLBparamP, (void *)globeP);
					
					globeP->currentField = 0;
					
					(*needResetListDrawFunc) = true;
					updateCode = FORM_UPDATE_CUSTOM;
				}
				else
				{
					isKeyHandled = false;
				}
			}
		}
		
		if (isKeyHandled)
		{
			MemSet(eventP, sizeof(EventType), 0);
			eventP->eType = nilEvent;
		}
		
		if (shouldPopupList)
		{
			(*exitCodeP) = popupPhoneNumberList(LstGetSelection(globeP->lstContactsP), globeP);
			if ((*exitCodeP) == NO_EXIT)
			{
				updateCode = FORM_UPDATE_CUSTOM;
			}
		}
		
		if (updateCode != FORM_NO_UPDATE)
		{
			FrmUpdateForm(frmContactsId, updateCode);
		}
		
		if (! globeP->isFocusRingDrawed)
		{
			if (globeP->focusedObjectId != lstContactsId)
			{
				HsNavObjectTakeFocus(globeP->frmContactsP, globeP->focusedObjectId);
			}
			else
			{
				HsNavRemoveFocusRing(globeP->frmContactsP);
			}
			
			globeP->isFocusRingDrawed = true;
		}
	}
	
	return isKeyHandled;
}

static Boolean keyHandleEvent_OLD(EventType *eventP, GlobeType *globeP, UInt8 *exitCodeP, Boolean *needResetListDrawFunc)
{
	Boolean		isKeyHandled	= false;
	Boolean		shouldPopupList	= false;
	
	if (eventP->eType == keyDownEvent && globeP->isMenuNotActive)
	{
		Int16		i, n;
		//UInt16		phoneNumberLength;
		UInt16		updateCode			= FORM_NO_UPDATE;
		WChar		chr					= keyTranslator(eventP);
		
		isKeyHandled = true;
		
		switch (chr)
		{
			case chrUpArrow:	//向上，滚动列表
			case keyPageUp:
			case keyRockerUp:
			{
				//获取列表项数
				n = LstGetNumberOfItems(globeP->lstContactsP);
				
				if (n > 0)
				{
					//获取当前选中项
					i = LstGetSelection(globeP->lstContactsP);
					
					//确定是否需要重绘列表
					if (i == LstGetTopItem(globeP->lstContactsP))
					{
						//在最后一行，需要重绘
						updateCode = FORM_UPDATE_CUSTOM;
					}
					
					//调整选中项
					if (i == noListSelection || i == 0)
					{
						i = n - 1;
						
						//跳转到最后一行，需要重绘
						updateCode = FORM_UPDATE_CUSTOM;
					}
					else
					{
						i --;
					}
					LstSetSelection(globeP->lstContactsP, i);
					
					//把默认号码拷贝到电话号码框
					copyDefaultPhoneNumber(i, globeP);
				}
				break;
			}
			case chrDownArrow:	//向下，滚动列表
			case keyPageDown:
			case keyRockerDown:
			{
				//获取列表项数
				n = LstGetNumberOfItems(globeP->lstContactsP);
				
				if (n > 0)
				{
					//获取当前选中项
					i = LstGetSelection(globeP->lstContactsP);
					
					//确定是否需要重绘列表
					if (i - LstGetTopItem(globeP->lstContactsP) + 1 == globeP->settingP->skin.lstContacts.other)
					{
						//在最后一行，需要重绘
						updateCode = FORM_UPDATE_CUSTOM;
					}
					
					if (i == noListSelection || i + 1 == n)
					{
						i = 0;
						
						//回溯至第一行，需要重绘
						updateCode = FORM_UPDATE_CUSTOM;
					}
					else
					{
						i ++;
					}
					LstSetSelection(globeP->lstContactsP, i);
					
					//把默认号码拷贝到电话号码框
					copyDefaultPhoneNumber(i, globeP);
				}
				
				break;
			}
			case chrLeftArrow:	//向左，清除所有输入
			case keyRockerLeft:
			{
				LstSetSelection(globeP->lstContactsP, noListSelection);
				updateCode = FORM_UPDATE_CUSTOM;
				
				n = FldGetTextLength(globeP->fldPhoneNumberP);
				if (n > 0)
				{
					FldDelete(globeP->fldPhoneNumberP, 0, n);
				}
				
				if (globeP->pyKeywordLength > 0)
				{
					MemSet(globeP->pyKeyword, 11, 0);
					globeP->pyKeywordLength = 0;
				}
				
				if (globeP->contactCount > 0)
				{
					freeContactList(globeP);
				}
				
				globeP->currentField = 0;
				(*needResetListDrawFunc) = true;
				break;
			}
			case chrRightArrow:	//向右，循环电话卡
			case keyRockerRight:
			{
				if (globeP->currentPreFix < globeP->totalPreFix - 1)
				{
					globeP->currentPreFix ++;
				}
				else
				{
					globeP->currentPreFix = 0;
				}
				updateCode = FORM_UPDATE_REFLASH2;
				break;
			}
			case 0x0128:	//检测长按中间键
			{
				if (FldGetTextLength(globeP->fldPhoneNumberP) > 0 && globeP->settingP->showDetailType == NO_DETAIL)
				{
					(*exitCodeP) = EXIT_FOR_DIAL;
				}
				break;
			}
			case keyRockerCenter:	//中间键
			{
				if ((! (eventP->data.keyDown.modifiers & willSendUpKeyMask)) || globeP->settingP->treoModel == TREO_600)
				{
					if (LstGetSelection(globeP->lstContactsP) != noListSelection)
					{
						if (globeP->contactCount > 0)
						{
							if (globeP->settingP->showDetailType == NO_DETAIL)
							{
								shouldPopupList = true;
							}
							else
							{
								if (globeP->listMappingP[LstGetSelection(globeP->lstContactsP)].field == 0xFF)
								{
									//打开联系人
									(*exitCodeP) = EXIT_FOR_OPEN_CONTACT;
								}
								else
								{
									(*exitCodeP) = EXIT_FOR_DIAL;
								}
							}
						}
						else if (globeP->recentCount > 0)
						{
							(*exitCodeP) = EXIT_FOR_DIAL;
						}
					}
					/*switch (globeP->focusedObjectId)
					{
						case lstContactsId:			//打开联系人细节
						{
							if (globeP->contactCount > 0)
							{
								if (globeP->settingP->showDetailType == NO_DETAIL)
								{
									shouldPopupList = true;
								}
								else
								{
									if (globeP->listMappingP[LstGetSelection(globeP->lstContactsP)].field == 0xFF)
									{
										//打开联系人
										(*exitCodeP) = EXIT_FOR_OPEN_CONTACT;
									}
									else
									{
										(*exitCodeP) = EXIT_FOR_DIAL;
									}
								}
							}
							else if (globeP->recentCount > 0)
							{
								(*exitCodeP) = EXIT_FOR_DIAL;
							}
							break;
						}
						case gdtDialButtonId:		//拨号
						{
							if (FldGetTextLength(globeP->fldPhoneNumberP) > 0)
							{
								(*exitCodeP) = EXIT_FOR_DIAL;
							}
							break;
						}
						case fldPhoneNumberId:		//移动焦点至拨号按钮
						{
							if (isFieldTextSelected(globeP->fldPhoneNumberP))
							{
								FldSetSelection(globeP->fldPhoneNumberP, 0, 0);
							}
							else
							{
								globeP->focusedObjectId = gdtDialButtonId;
								globeP->isFocusRingDrawed = false;
							}
							updateCode = FORM_UPDATE_CUSTOM;
							break;
						}
						case gdtPreFixTriggerId:	//打开电话卡列表
						{
							HsNavRemoveFocusRing(globeP->frmContactsP);
							setListFrameColor(false, globeP);
							if (LstPopupList(globeP->lstPreFixP) != noListSelection)
							{
								globeP->currentPreFix = LstGetSelection(globeP->lstPreFixP);
							}
							setListFrameColor(true, globeP);
							updateCode = FORM_UPDATE_REFLASH;
							globeP->isFocusRingDrawed = false;
							break;
						}
						case gdtOperationButtonId:	//打开其他操作列表
						{
							if (FldGetTextLength(globeP->fldPhoneNumberP) > 0)
							{
								setListFrameColor(false, globeP);
								switch (LstPopupList(globeP->lstOperationP))
								{
									case 0:
									{
										(*exitCodeP) = EXIT_FOR_ADD_CONTACT;
										break;
									}
									case 1:
									{
										(*exitCodeP) = EXIT_FOR_OPEN_CONTACT;
										break;
									}
								}
								setListFrameColor(true, globeP);
							}
							break;
						}
						case gdtSMSButtonId:		//打开短讯操作列表
						{
							if (FldGetTextLength(globeP->fldPhoneNumberP) > 0)
							{
								setListFrameColor(false, globeP);
								switch (LstPopupList(globeP->lstSMSP))
								{
									case 0:
									{
										(*exitCodeP) = EXIT_FOR_SYSTEM_MESSAGE;
										break;
									}
									case 1:
									{
										(*exitCodeP) = EXIT_FOR_MESSAGE;
										break;
									}
									case 2:
									{
										(*exitCodeP) = EXIT_FOR_EMAIL;
										break;
									}
								}
								setListFrameColor(true, globeP);
							}
							break;
						}
						case gdtHelpButtonId:		//打开帮助
						{
							FrmHelp(stringTip);
							updateCode = FORM_UPDATE_REFLASH;
							globeP->isFocusRingDrawed = false;
							break;
						}
					}*/
				}
				else
				{
					isKeyHandled = false;
				}
				break;
			}
			case TREO_680_GREEN_KEY:	//直接拨号
			case keyHard1:
			{
				if (FldGetTextLength(globeP->fldPhoneNumberP) > 0)
				{
					(*exitCodeP) = EXIT_FOR_DIRECT_DIAL;
				}
				break;
			}
			case keyHard2:			//保存或打开联系人
			{
				if (globeP->settingP->keyHard2Action == KEY_HARD2_SAVE_CONTACT && FldGetTextLength(globeP->fldPhoneNumberP) > 0)
				{
					(*exitCodeP) = EXIT_FOR_ADD_CONTACT;
				}
				else if (LstGetSelection(globeP->lstContactsP) != noListSelection && globeP->contactCount > 0)
				{
					(*exitCodeP) = EXIT_FOR_OPEN_CONTACT;
				}
				break;
			}
			case keyHard3:			//发送短讯或电子邮件
			{
				i = LstGetSelection(globeP->lstContactsP);
				if (i != noListSelection)
				{
					if (globeP->contactCount > 0)
					{
						if (getSelectedPhoneLabel(globeP, i, getSelectedPhoneField(i, 0, globeP)) != emailLabel)	//发短讯
						{
							if (globeP->settingP->keyHard3Action == KEY_HARD3_SYSTEM_MESSAGE)
							{
								(*exitCodeP) = EXIT_FOR_SYSTEM_MESSAGE;
							}
							else
							{
								(*exitCodeP) = EXIT_FOR_MESSAGE;
							}
						}
						else	//发邮件
						{
							(*exitCodeP) = EXIT_FOR_EMAIL;
						}
					}
					else
					{
						if (globeP->settingP->keyHard3Action == KEY_HARD3_SYSTEM_MESSAGE)
						{
							(*exitCodeP) = EXIT_FOR_SYSTEM_MESSAGE;
						}
						else
						{
							(*exitCodeP) = EXIT_FOR_MESSAGE;
						}
					}
				}
				else if (FldGetTextLength(globeP->fldPhoneNumberP) > 0)
				{
					if (globeP->settingP->keyHard3Action == KEY_HARD3_SYSTEM_MESSAGE)
					{
						(*exitCodeP) = EXIT_FOR_SYSTEM_MESSAGE;
					}
					else
					{
						(*exitCodeP) = EXIT_FOR_MESSAGE;
					}
				}
				break;
			}
			case keyBackspace:		//删除输入的字符
			{
				//取消列表的选择
				LstSetSelection(globeP->lstContactsP, noListSelection);
				
				//调整关键字，并检测是否应该退出
				if (FldGetTextLength(globeP->fldPhoneNumberP) > 0 || globeP->pyKeywordLength > 0)
				{
					globeP->focusedObjectId = fldPhoneNumberId;
					
					isKeyHandled = FldHandleEvent(globeP->fldPhoneNumberP, eventP);
					
					addKeyToKeyword(chr, globeP);
					
					if (globeP->pyKeywordLength > 0 || FldGetTextLength(globeP->fldPhoneNumberP) >= 3)
					{
						//检索通讯录
						(*(globeP->getContactListP))(globeP->CNLBGlobeP, globeP->CNLBparamP, globeP);
					}
					else
					{
						//显示常用联系人列表
						getRecentList(globeP);
					}
					
					globeP->currentField = 0;
					updateCode = FORM_UPDATE_CUSTOM;
				}
				else
				{
					(*exitCodeP) = EXIT_FOR_NOTHING;
				}
				
				(*needResetListDrawFunc) = true;
				break;
			}
			default:				//添加到关键字并进行检索
			{
				if (chr >= keyA && chr <= keyZ)
				{
					//取消列表的选择
					LstSetSelection(globeP->lstContactsP, noListSelection);
					
					//添加到关键字
					addKeyToKeyword(chr, globeP);
					
					globeP->focusedObjectId = fldPhoneNumberId;
					
					FldHandleEvent(globeP->fldPhoneNumberP, eventP);
					
					//检索通讯录
					(*(globeP->getContactListP))(globeP->CNLBGlobeP, globeP->CNLBparamP, (void *)globeP);
					
					globeP->currentField = 0;
					
					(*needResetListDrawFunc) = true;
					updateCode = FORM_UPDATE_CUSTOM;
				}
				else
				{
					isKeyHandled = false;
				}
			}
		}
		
		if (isKeyHandled)
		{
			MemSet(eventP, sizeof(EventType), 0);
			eventP->eType = nilEvent;
		}
		
		if (shouldPopupList)
		{
			(*exitCodeP) = popupPhoneNumberList(LstGetSelection(globeP->lstContactsP), globeP);
			if ((*exitCodeP) == NO_EXIT)
			{
				updateCode = FORM_UPDATE_CUSTOM;
			}
		}
		
		if (updateCode != FORM_NO_UPDATE)
		{
			FrmUpdateForm(frmContactsId, updateCode);
		}
		
		/*if (! globeP->isFocusRingDrawed)
		{
			if (globeP->focusedObjectId != lstContactsId)
			{
				HsNavObjectTakeFocus(globeP->frmContactsP, globeP->focusedObjectId);
			}
			else
			{
				HsNavRemoveFocusRing(globeP->frmContactsP);
			}
			
			globeP->isFocusRingDrawed = true;
		}*/
	}
	
	return isKeyHandled;
}

#if 0
#pragma mark == APPLICATION EVENT HANDLER ==
#endif

static UInt16 getSelectedControl(EventType *eventP, SkinSettingType *skinP)
{
	if (RctPtInRectangle(eventP->screenX, eventP->screenY, &skinP->gdtDialButton.bounds))
	{
		return gdtDialButtonId;
	}
	else if (RctPtInRectangle(eventP->screenX, eventP->screenY, &skinP->fldPhoneNumber.bounds))
	{
		return fldPhoneNumberId;
	}
	else if (RctPtInRectangle(eventP->screenX, eventP->screenY, &skinP->gdtPreFixTrigger.bounds))
	{
		return gdtPreFixTriggerId;
	}
	else if (RctPtInRectangle(eventP->screenX, eventP->screenY, &skinP->gdtOperationButton.bounds))
	{
		return gdtOperationButtonId;
	}
	else if (RctPtInRectangle(eventP->screenX, eventP->screenY, &skinP->gdtSMSButton.bounds))
	{
		return gdtSMSButtonId;
	}
	else if (RctPtInRectangle(eventP->screenX, eventP->screenY, &skinP->gdtHelpButton.bounds))
	{
		return gdtHelpButtonId;
	}
	
	return 0;
}

static Boolean appHandleEvent(EventType *eventP, GlobeType *globeP, UInt8 *exitCodeP, Boolean *needResetListDrawFunc)
{
	Boolean		isEventHandled		= false;
	
	switch (eventP->eType)
	{
		case frmOpenEvent:
		{
			//构造窗体
			createContactsForm(globeP);
			FrmSetActiveForm(globeP->frmContactsP);
			
			//取常用联系人列表
			getRecentList(globeP);
			
			if (globeP->settingP->actionKey != chrNull)
			{
				//添加到关键字
				if (globeP->settingP->actionKeycode >= keyA && globeP->settingP->actionKeycode <= keyZ)
				{
					addKeyToKeyword(globeP->settingP->actionKeycode, globeP);
				}
				
				//添加到号码框
				globeP->cache[0] = (Char)globeP->settingP->actionKey;
				FldInsert(globeP->fldPhoneNumberP, globeP->cache, 1);
				globeP->cache[0] = '\0';
					
				//检索通讯录
				(*(globeP->getContactListP))(globeP->CNLBGlobeP, globeP->CNLBparamP, (void *)globeP);
			}
			LstSetSelection(globeP->lstContactsP, noListSelection);
			(*needResetListDrawFunc) = true;
			FrmUpdateForm(frmContactsId, frmRedrawUpdateCode);
			
			//设置焦点
			globeP->focusedObjectId = fldPhoneNumberId;
			globeP->isFocusRingDrawed = true;
			HsNavObjectTakeFocus(globeP->frmContactsP, fldPhoneNumberId);
			/*globeP->focusedObjectId = gdtDialButtonId;
			globeP->isFocusRingDrawed = false;
			HsNavObjectTakeFocus(globeP->frmContactsP, gdtDialButtonId);*/
			
			
			isEventHandled = true;
			break;
		}
		case frmUpdateEvent:
		{
			updateContactsForm(globeP, eventP->data.frmUpdate.updateCode);
			
			isEventHandled = true;
			break;
		}
		case frmObjectFocusTakeEvent:
		{
			if (FrmDispatchEvent(eventP))
			{
				switch (globeP->focusedObjectId)
				{
					case fldPhoneNumberId:
					{
						HsGrfSetStateExt(false, false, true, false, false, false);
						if (isFieldTextSelected(globeP->fldPhoneNumberP))
						{
							HsNavDrawFocusRing(globeP->frmContactsP, globeP->focusedObjectId, hsNavFocusRingNoExtraInfo, getFocusedObjectBounds(globeP), hsNavFocusRingStyleSquare, true);
						}
						else
						{
							HsNavRemoveFocusRing(globeP->frmContactsP);
						}
						break;
					}
					case lstContactsId:
					{
						HsNavRemoveFocusRing(globeP->frmContactsP);
						break;
					}
					default:
					{
						HsNavDrawFocusRing(globeP->frmContactsP, globeP->focusedObjectId, hsNavFocusRingNoExtraInfo, getFocusedObjectBounds(globeP), hsNavFocusRingStyleRound, true);
						break;
					}
				}
			}
			
			isEventHandled = true;
			break;
		}
		case ctlSelectEvent:
		{
			break;
		}
		case winEnterEvent:
		{
			if (globeP->frmContactsP)
			{
				if ((WinHandle)globeP->frmContactsP == eventP->data.winEnter.enterWindow)
				{
					globeP->isMenuNotActive = true;
				}
			}
			break;
		}
		case menuEvent:
		{
			if (eventP->data.menu.itemID == menuItemDeleteRecent)
			{
				if (LstGetSelection(globeP->lstContactsP) != noListSelection)
				{
					setListFrameColor(false, globeP);
					if (globeP->contactCount == 0 && globeP->recentCount > 0)
					{
						if (FrmAlert(alertConfirmDelete) == 0)
						{
							deleteRecentItem(globeP, LstGetSelection(globeP->lstContactsP));
							getRecentList(globeP);
							if (globeP->recentCount > 0)
							{
								LstSetSelection(globeP->lstContactsP, 0);
							}
							else
							{
								LstSetSelection(globeP->lstContactsP, noListSelection);
							}
							(*needResetListDrawFunc) = true;
						}
					}
					else
					{
						FrmCustomAlert(alertCustomError, "无法删除选定的项目", "", "");
					}
					setListFrameColor(true, globeP);
				}
			}
			
			FrmUpdateForm(frmContactsId, FORM_UPDATE_REFLASH);
			isEventHandled = true;
			break;
		}
		case lstSelectEvent:
		{
			if (eventP->data.lstSelect.listID == lstContactsId)
			{
				if (LstGetNumberOfItems(globeP->lstContactsP) > 0)
				{
					globeP->focusedObjectId = lstContactsId;
					HsNavObjectTakeFocus(globeP->frmContactsP, lstContactsId);
					copyDefaultPhoneNumber(eventP->data.lstSelect.selection, globeP);
					HsNavRemoveFocusRing(globeP->frmContactsP);
				}
				else
				{
					LstSetSelection(globeP->lstContactsP, noListSelection);
				}
			}
			break;
		}
		case penDownEvent:
		{
			if ((! RctPtInRectangle(eventP->screenX, eventP->screenY, &globeP->settingP->skin.lstContacts.bounds)) &&
				(! RctPtInRectangle(eventP->screenX, eventP->screenY, &globeP->settingP->skin.fldPhoneNumber.bounds)))
			{
				isEventHandled = true;
			}
			break;
		}
		case penUpEvent:
		{
			if (! RctPtInRectangle(eventP->screenX, eventP->screenY, &globeP->settingP->skin.frmContacts.bounds))
			{
				isEventHandled = true;
				(*exitCodeP) = EXIT_FOR_NOTHING;
			}
			else
			{
				UInt16		updateCode			= FORM_UPDATE_CUSTOM;
				
				switch (getSelectedControl(eventP, &globeP->settingP->skin))
				{
					case gdtDialButtonId:
					{
						if (FldGetTextLength(globeP->fldPhoneNumberP) > 0)
						{
							(*exitCodeP) = EXIT_FOR_DIAL;
						}
						isEventHandled = true;
						break;
					}
					case fldPhoneNumberId:
					{
						if (! globeP->settingP->shouldUseOldOperationType)
						{
							globeP->focusedObjectId = fldPhoneNumberId;
							HsNavRemoveFocusRing(globeP->frmContactsP);
						}
						isEventHandled = true;
						break;
					}
					case gdtPreFixTriggerId:
					{
						if (! globeP->settingP->shouldUseOldOperationType)
						{
							HsNavRemoveFocusRing(globeP->frmContactsP);
							globeP->isFocusRingDrawed = false;
						}
						
						setListFrameColor(false, globeP);
						if (LstPopupList(globeP->lstPreFixP) != noListSelection)
						{
							globeP->currentPreFix = LstGetSelection(globeP->lstPreFixP);
						}
						setListFrameColor(true, globeP);
						updateCode = FORM_UPDATE_REFLASH;
						isEventHandled = true;
						break;
					}
					case gdtOperationButtonId:
					{
						if (FldGetTextLength(globeP->fldPhoneNumberP) > 0)
						{
							setListFrameColor(false, globeP);
							switch (LstPopupList(globeP->lstOperationP))
							{
								case 0:
								{
									(*exitCodeP) = EXIT_FOR_ADD_CONTACT;
									break;
								}
								case 1:
								{
									if (LstGetSelection(globeP->lstContactsP) != noListSelection && globeP->contactCount > 0)
									{
										(*exitCodeP) = EXIT_FOR_OPEN_CONTACT;
									}
									break;
								}
							}
							setListFrameColor(true, globeP);
							isEventHandled = true;
						}
						break;
					}
					case gdtSMSButtonId:
					{
						if (FldGetTextLength(globeP->fldPhoneNumberP) > 0)
						{
							setListFrameColor(false, globeP);
							switch (LstPopupList(globeP->lstSMSP))
							{
								case 0:
								{
									(*exitCodeP) = EXIT_FOR_SYSTEM_MESSAGE;
									break;
								}
								case 1:
								{
									(*exitCodeP) = EXIT_FOR_MESSAGE;
									break;
								}
								case 2:
								{
									(*exitCodeP) = EXIT_FOR_EMAIL;
									break;
								}
							}
							setListFrameColor(true, globeP);
							isEventHandled = true;
						}
						break;
					}
					case gdtHelpButtonId:
					{
						FrmHelp(stringTip);
						updateCode = FORM_UPDATE_REFLASH;
						if (! globeP->settingP->shouldUseOldOperationType)
						{
							globeP->isFocusRingDrawed = false;
						}
						isEventHandled = true;
						break;
					}
				}
				
				if (updateCode != FORM_NO_UPDATE)
				{
					FrmUpdateForm(frmContactsId, updateCode);
				}
				
				if (! globeP->isFocusRingDrawed)
				{
					if (globeP->focusedObjectId != lstContactsId)
					{
						HsNavObjectTakeFocus(globeP->frmContactsP, globeP->focusedObjectId);
					}
					else
					{
						HsNavRemoveFocusRing(globeP->frmContactsP);
					}
					
					globeP->isFocusRingDrawed = true;
				}
			}
			break;
		}
	}
	return isEventHandled;
}

#if 0
#pragma mark == MAIN EVENT HANDLER ==
#endif

static void setGlobePreFix(GlobeType *globeP, Boolean isConstruct)
{
	UInt16		i;
	
	if (isConstruct)
	{
		//分配内存
		globeP->preFix = (Char **)MemPtrNew(24);
		
		for (i = 0; i < 6; i ++)
		{
			globeP->preFix[i] = (Char *)MemPtrNew(21);
			MemSet(globeP->preFix[i], 21, 0x00);
			if (globeP->settingP->preFix[i][0])
			{
				StrCopy(globeP->preFix[i], globeP->settingP->preFix[i]);
				globeP->totalPreFix ++;
			}
		}
		
		globeP->currentPreFix = 0;
	}
	else
	{
		for (i = 0; i < 6; i ++)
		{
			MemPtrFree(globeP->preFix[i]);
		}
		MemPtrFree(globeP->preFix);
	}
}

static UInt8 frmContactEventHandler(ActionParameterType *actionParamP)
{
	EventType			event;
	GrfStatusType		grfStatus;
	UInt16				error					= errNone;
	UInt32				ctx						= 0;
	UInt8				exitCode				= NO_EXIT;
	Boolean				needResetListDrawFunc	= false;
	GlobeType			*globeP					= NULL;
	Boolean				(*keyHandleEventP)		(EventType *eventP, GlobeType *globeP, UInt8 *exitCodeP, Boolean *needResetListDrawFunc);
	
	//确定按键操作函数
	if (actionParamP->settingP->shouldUseOldOperationType)
	{
		keyHandleEventP = keyHandleEvent_OLD;
	}
	else
	{
		keyHandleEventP = keyHandleEvent;
	}
	
	//初始化globe
	globeP = (GlobeType *)MemPtrNew(sizeof(GlobeType));
	MemSet(globeP, sizeof(GlobeType), 0x00);
	globeP->settingP = actionParamP->settingP;
	setGlobePreFix(globeP, true);
	globeP->isMenuNotActive = true;
	globeP->recentListP = (RecentUnitType *)MemPtrNew((sizeof(RecentUnitType) * 10));
	
	//保存当前窗口的内容
	globeP->bgRect.topLeft.x = 0; globeP->bgRect.topLeft.y = 0;
	globeP->bgRect.extent.x = 160; globeP->bgRect.extent.y = 160;
	globeP->bgBitsWinH = WinSaveBits(&globeP->bgRect, &error);
	
	if (error == errNone)
	{
		//保存并重置Grf状态
		HsGrfGetStateExt(&grfStatus.capsLock, &grfStatus.numLock, &grfStatus.optLock, &grfStatus.tempShift, &grfStatus.autoShifted);
		HsGrfSetStateExt(false, false, false, false, false, false);
		
		//打开检索库
		if (CLibLoad(&globeP->refNum, &ctx) == errNone)
		{
			//根据通讯录数据库的类型，确定应调用的函数
			if (globeP->settingP->treoModel == TREO_600 || globeP->settingP->treoModel == NOT_A_TREO)
			{
				globeP->drawContactListByContactP = drawContactListByContact_Addr;
				globeP->getContactListP = getContactList_Addr;
				globeP->contactUnitP = MemPtrNew(sizeof(OLD_AddrDBRecordType));
				error = initCNLBGlobeParameter(&globeP->CNLBGlobeP, ADDRESS_VERSION_OLD);
			}
			else
			{
				globeP->drawContactListByContactP = drawContactListByContact_PAdd;
				globeP->getContactListP = getContactList_PAdd;
				globeP->contactUnitP = MemPtrNew(sizeof(AddrDBRecordType));
				error = initCNLBGlobeParameter(&globeP->CNLBGlobeP, ADDRESS_VERSION_NEW);
			}
			
			//预初始化全局变量，打开通讯录数据库
			if (error == errNone)
			{
				//预构造参数
				globeP->CNLBparamP = (CNLBParameterType *)MemPtrNew(sizeof(CNLBParameterType));
				MemSet(globeP->CNLBparamP, sizeof(CNLBParameterType), 0x00);
				globeP->CNLBparamP->orderlySearch = globeP->settingP->orderlySearch;
				
				//获取通讯记录总数
				globeP->totalContacts = DmNumRecordsInCategory(globeP->CNLBGlobeP->addressDbRef, dmAllCategories);
				
				//分配映射表
				globeP->listMappingSize = (globeP->totalContacts + 1) * sizeof(ListMappingType);
				globeP->listMappingP = (ListMappingType *)MemGluePtrNew(globeP->listMappingSize);
				MemSet(globeP->listMappingP, globeP->listMappingSize, 0);
				
				//分配节点指针
				globeP->contactListH = MemHandleNew(((globeP->totalContacts + 1) * 4));
				
				//打开数据库
				globeP->recentDbRef = DmOpenDatabaseByTypeCreator('RECT', appFileCreator, dmModeReadWrite);
				if (DmGetLastErr() == dmErrCantFind) //最近联系人列表库不存在
				{
					DmCreateDatabase(0, "Recent Contacts", appFileCreator, 'RECT', false);
					globeP->recentDbRef = DmOpenDatabaseByTypeCreator('RECT', appFileCreator, dmModeReadWrite);
				}
				
				//设置颜色
				globeP->pyMatchRGBColor.r = 139;
				globeP->pyMatchRGBColor.g = 174;
				globeP->pyMatchRGBColor.b = 219;
				globeP->phoneNumberMatchRGBColor.r = 245;
				globeP->phoneNumberMatchRGBColor.g = 131;
				globeP->phoneNumberMatchRGBColor.b = 131;
				globeP->alternateRGBColor.r = 220;
				globeP->alternateRGBColor.g = 220;
				globeP->alternateRGBColor.b = 220;
				UIColorGetTableEntryRGB(UIObjectSelectedFill, &globeP->highlightBGRGBColor);
				UIColorGetTableEntryRGB(UIObjectSelectedForeground, &globeP->highlightTextRGBColor);
				
				//分配列表项
				globeP->operationPP = (Char **)MemPtrNew(8);
				globeP->operationPP[0] = (Char *)MemPtrNew(9);
				StrCopy(globeP->operationPP[0], "保存号码");
				globeP->operationPP[1] = (Char *)MemPtrNew(11);
				StrCopy(globeP->operationPP[1], "打开联系人");
				
				globeP->SMSPP = (Char **)MemPtrNew(12);
				globeP->SMSPP[0] = (Char *)MemPtrNew(15);
				StrCopy(globeP->SMSPP[0], "用系统短讯发送");
				globeP->SMSPP[1] = (Char *)MemPtrNew(16);
				StrCopy(globeP->SMSPP[1], "用随手Phone发送");
				globeP->SMSPP[2] = (Char *)MemPtrNew(13);
				StrCopy(globeP->SMSPP[2], "发送电子邮件");
				
				//打开窗体
				FrmPopupForm(frmContactsId);
				
				//事件循环
				do
				{
					EvtGetEvent(&event, evtWaitForever);
					
					//设置列表绘制函数
					if (needResetListDrawFunc)
					{
						if (globeP->contactCount == 0)	//常用联系人列表
						{
							if (globeP->recentCount > 0 && globeP->pyKeywordLength == 0/* && FldGetTextLength(globeP->fldPhoneNumberP) == 0*/)
							{
								LstSetDrawFunction(globeP->lstContactsP, drawContactListByRecent);
								LstSetListChoices(globeP->lstContactsP, (Char **)&globeP, globeP->recentCount);
							}
							else
							{
								LstSetDrawFunction(globeP->lstContactsP, NULL);
								LstSetListChoices(globeP->lstContactsP, NULL, 0);
							}
						}
						else	//检索结果
						{
							LstSetDrawFunction(globeP->lstContactsP, globeP->drawContactListByContactP);
							if (globeP->settingP->showDetailType == NO_DETAIL)
							{
								LstSetListChoices(globeP->lstContactsP, (Char **)&globeP, globeP->contactCount);
							}
							else
							{
								LstSetListChoices(globeP->lstContactsP, (Char **)&globeP, globeP->listMappingCount);
							}
						}
						
						needResetListDrawFunc = false;
					}
					
					if (! (*keyHandleEventP)(&event, globeP, &exitCode, &needResetListDrawFunc))
					{
						if (! SysHandleEvent(&event))
						{
							if (! MenuHandleEvent(0, &event, &error))
							{
								if (! appHandleEvent(&event, globeP, &exitCode, &needResetListDrawFunc))
								{
									FrmDispatchEvent(&event);
								}
							}
							else
							{
								globeP->isMenuNotActive = false;
							}
						}
					}
				} while(event.eType != appStopEvent && exitCode == NO_EXIT);
				
				//构造返回参数
				if (exitCode != EXIT_FOR_NOTHING && event.eType != appStopEvent)
				{
					getActionParameter(actionParamP, globeP, exitCode);
					if (exitCode >= EXIT_FOR_DIAL && exitCode <= EXIT_FOR_DIRECT_DIAL)
					{
						saveContactToRecentDB(globeP, actionParamP);
					}
				}
				
				//退出当前窗口
				FrmReturnToForm(0);
				
				//释放列表项
				MemPtrFree(globeP->SMSPP[2]);
				MemPtrFree(globeP->SMSPP[1]);
				MemPtrFree(globeP->SMSPP[0]);
				MemPtrFree(globeP->SMSPP);
				MemPtrFree(globeP->operationPP[1]);
				MemPtrFree(globeP->operationPP[0]);
				MemPtrFree(globeP->operationPP);
				
				//恢复边框颜色
				setListFrameColor(false, globeP);
				
				//关闭数据库
				DmCloseDatabase(globeP->recentDbRef);
				
				//释放内存
				MemPtrFree(globeP->CNLBparamP);
				freeContactList(globeP);
				MemHandleFree(globeP->contactListH);
				MemPtrFree(globeP->listMappingP);
			}
			
			//关闭数据库
			DmCloseDatabase(globeP->CNLBGlobeP->addressDbRef);
			
			//释放内存
			MemPtrFree(globeP->CNLBGlobeP);
			MemPtrFree(globeP->contactUnitP);
		}
		
		//关闭检索库
		CLibRemove(globeP->refNum, ctx);
		
		//恢复Grf状态
		HsGrfSetStateExt(grfStatus.capsLock, grfStatus.numLock, grfStatus.optLock, false, false, false);
	}
	
	//释放内存
	if ((UInt8)globeP->settingP->skin.bmpPath[0] != 0)
	{
		BmpDelete(globeP->skinBmpP);
	}
	else
	{
		MemHandleUnlock(globeP->skinBmpH);
		DmReleaseResource(globeP->skinBmpH);
	}
	WinDeleteWindow(globeP->bgBitsWinH, false);
	
	MemPtrFree(globeP->recentListP);
	setGlobePreFix(globeP, false);
	MemPtrFree(globeP);
	
	return exitCode;
}

#if 0
#pragma mark -
#pragma mark Funtions for sending SMS
#pragma mark -
#endif

//****************************************************************************
//显示短讯发送状态
//****************************************************************************
static void showSendMessageStatus(const Char *messageStatusP)
{
	FormType	*frmSendSMSStatusP = NULL;
	UInt16		messageStatusLength;
	UInt16		messageStatusWidth;
	
	frmSendSMSStatusP = FrmInitForm(frmSendSMSStatus);
	FrmSetActiveForm(frmSendSMSStatusP);
	FrmDrawForm(frmSendSMSStatusP);
	messageStatusLength = StrLen(messageStatusP);
	messageStatusWidth = FntCharsWidth(messageStatusP, messageStatusLength);
	WinDrawChars(messageStatusP, messageStatusLength, 30 - (messageStatusWidth >> 1), 7);
}

//****************************************************************************
//发送短讯
//****************************************************************************
static void sendMessages(ActionParameterType *actionParameterP, Char *messageP, UInt16 messageLength)
{
	/*MsgAddRecordParamsType		*msg;
	
	msg = (MsgAddRecordParamsType *)MemPtrNew(sizeof(MsgAddRecordParamsType));
	msg->category = MsgInboxCategory;
	msg->edit = false;
	msg->signature = false;
	msg->subject = NULL;
	msg->from = NULL;
	msg->to = (Char *)MemPtrNew(StrLen(actionParameterP->phoneNumber) + 1);
	StrCopy(msg->to, actionParameterP->phoneNumber);
	MemPtrSetOwner(msg->to, 0);
	msg->replyTo = (Char *)MemPtrNew(4);
	StrCopy(msg->replyTo, "123");
	MemPtrSetOwner(msg->replyTo, 0);
	msg->body = (Char *)MemPtrNew(messageLength + 1);
	StrCopy(msg->body, messageP);
	MemPtrSetOwner(msg->body, 0);
	MemPtrSetOwner(msg, 0);
	
	//MemPtrFree(actionParameterP);
	
	LaunchWithCommand('appl', hsFileCTexter, sysAppLaunchCmdAddRecord, msg);*/
	/*SysNotifyParamType			*param	 = MemPtrNew(sizeof(SysNotifyParamType));
	HelperNotifyEventType		*details = MemPtrNew(sizeof(HelperNotifyEventType));
	HelperNotifyExecuteType		*execute = MemPtrNew(sizeof(HelperNotifyExecuteType));
	HelperServiceSMSDetailsType	*sms	 = MemPtrNew(sizeof(HelperServiceSMSDetailsType));
	
	//消息机制参数
	param->notifyType = sysNotifyHelperEvent;
	param->broadcaster = appFileCreator;
	param->notifyDetailsP = details;
	param->handled = false;
	
	//帮助机制参数
	details->version = kHelperNotifyCurrentVersion;
	details->actionCode = kHelperNotifyActionCodeExecute;
	details->data.executeP = execute;
	
	//短讯内容
	sms->version = 1;
	sms->message = (Char *)MemPtrNew(messageLength + 1);
	StrCopy(sms->message, messageP);
	MemPtrSetOwner(sms->message, 0);
	
	//帮助机制执行模式参数
	execute->serviceClassID = kHelperServiceClassIDSMS;
	execute->helperAppID = 0;
	execute->dataP = (Char *)MemPtrNew(StrLen(actionParameterP->phoneNumber) + 1);
	StrCopy(execute->dataP, actionParameterP->phoneNumber);
	execute->displayedName = (Char *)MemPtrNew(StrLen(actionParameterP->fullName) + 1);
	StrCopy(execute->displayedName, actionParameterP->fullName);
	execute->detailsP = sms;
	execute->err = errNone;
	
	MemPtrSetOwner(sms, 0);
	MemPtrSetOwner(execute->dataP, 0);
	MemPtrSetOwner(execute->displayedName, 0);
	MemPtrSetOwner(execute, 0);
	MemPtrSetOwner(details, 0);
	MemPtrSetOwner(param, 0);
	
	SysNotifyBroadcast(param);*/
	
	UInt16				refNum;
	PhnDatabaseID		messageId;
	PhnAddressHandle	addressH;
	PhnAddressList		addressList;
	
	//打开电话库
	if (HsGetPhoneLibrary(&refNum) == errNone)
	{
		//生成一个短讯记录
		messageId = PhnLibNewMessage(refNum, kMTOutgoing);
		
		//设置短讯所有者
		PhnLibSetOwner(refNum, messageId, appFileCreator);
		
		//设置短讯正文
		if (PhnLibSetText(refNum, messageId, messageP, (Int16)messageLength) == errNone)
		{
			//设置发送对象
			addressList = PhnLibNewAddressList(refNum);
			if (addressList)
			{
				addressH = PhnLibNewAddress(refNum, actionParameterP->phoneNumber, phnLibUnknownID);
				if (addressH)
				{
					PhnLibAddAddress(refNum, addressList, addressH);
					MemHandleFree(addressH);
					
					PhnLibSetAddresses(refNum, messageId, addressList);
					
					//发送短讯
					showSendMessageStatus("短讯发送中");
					if (PhnLibSendMessage(refNum, messageId, true) != errNone)
					{
						FrmReturnToForm(0);
						showSendMessageStatus("发送失败");
						SysTaskDelay((SysTicksPerSecond() >> 1));
						FrmReturnToForm(0);
					}
					else
					{
						FrmReturnToForm(0);
						showSendMessageStatus("发送成功");
						SysTaskDelay((SysTicksPerSecond() >> 1));
						FrmReturnToForm(0);
					}
				}
				
				PhnLibDisposeAddressList(refNum, addressList);
			}
		}
		
		PhnLibDeleteMessage(refNum, messageId, false);
		
		//关闭电话库
		PhnLibClose(refNum);
	}
}

//****************************************************************************
//根据文本框的变化，更新滚动条
//****************************************************************************
static void updateScrollBar(FieldType *fldContentP, ScrollBarType *srlContentP)
{
	UInt16			scrollPosition;
	UInt16			textHeight;
	UInt16			fieldHeight;
	Int16			maxValue;

	FldGetScrollValues (fldContentP, &scrollPosition, &textHeight,  &fieldHeight);

	if (textHeight > fieldHeight)
	{
		// On occasion, such as after deleting a multi-line selection of text,
		// the display might be the last few lines of a field followed by some
		// blank lines.  To keep the current position in place and allow the user
		// to "gracefully" scroll out of the blank area, the number of blank lines
		// visible needs to be added to max value.  Otherwise the scroll position
		// may be greater than maxValue, get pinned to maxvalue in SclSetScrollBar
		// resulting in the scroll bar and the display being out of sync.
		maxValue = (textHeight - fieldHeight) + FldGetNumberOfBlankLines(fldContentP);
	}
	else if (scrollPosition)
	{
		maxValue = scrollPosition;
	}
	else
	{
		maxValue = 0;
	}

	SclSetScrollBar (srlContentP, scrollPosition, 0, maxValue, fieldHeight - 1);
}

//****************************************************************************
//根据滚动条的变化，更新文本框
//****************************************************************************
static void scrollContextField(FieldType *fldContentP, ScrollBarType *srlContentP, Int16 linesToScroll, Boolean updateScrollbar)
{
	UInt16			blankLines;

	blankLines = FldGetNumberOfBlankLines(fldContentP);

	if (linesToScroll < 0)
	{
		FldScrollField (fldContentP, -linesToScroll, winUp);
	}
	else if (linesToScroll > 0)
	{
		FldScrollField (fldContentP, linesToScroll, winDown);
	}

	// If there were blank lines visible at the end of the field
	// then we need to update the scroll bar.
	if (blankLines || updateScrollbar)
	{
		ErrNonFatalDisplayIf(blankLines && linesToScroll > 0, "blank lines when scrolling winDown");

		updateScrollBar(fldContentP, srlContentP);
	}
}

//****************************************************************************
//计算短讯的文字长度及条数
//****************************************************************************
static void calculateAndDrawLengthAndCount(FieldType *fldContentP, MessageGlobeType *globeP)
{
	Char	number[maxStrIToALen];
	
	globeP->contentLength = FldGetTextLength(fldContentP);
	globeP->messageCount = (globeP->contentLength / 140);
	if ((globeP->contentLength % 140) > 0)
	{
		globeP->messageCount ++;
	}
	
	WinEraseChars(globeP->lengthAndCount, StrLen(globeP->lengthAndCount), 125, 16);
	
	StrIToA(number, globeP->contentLength);
	StrCopy(globeP->lengthAndCount, number);
	StrCat(globeP->lengthAndCount, "/");
	StrIToA(number, globeP->messageCount);
	StrCat(globeP->lengthAndCount, number);
	
	WinDrawChars(globeP->lengthAndCount, StrLen(globeP->lengthAndCount), 125, 16);
}

//****************************************************************************
//编写并发送短讯
//****************************************************************************
static void frmMessageEventHandler(ActionParameterType *actionParameterP)
{
	EventType			event;
	UInt16				exitCode = NO_EXIT;
	UInt16				error;
	FormType			*frmMessageP = NULL;
	FieldType			*fldContentP = NULL;
	FieldType			*fldSMSPhoneNumberP = NULL;
	ScrollBarType		*srlContentP = NULL;
	MessageGlobeType	*globeP;
	
	//初始化全局变量
	globeP = (MessageGlobeType *)MemPtrNew(sizeof(MessageGlobeType));
	MemSet(globeP, sizeof(MessageGlobeType), 0x00);
	
	//打开短讯编写窗口
	FrmPopupForm(frmMessage);
	
	do
	{
		//获取新事件
		EvtGetEvent(&event, evtWaitForever);
		
		if (fldContentP)
		{
			if (WinGetActiveWindow() == (WinHandle)frmMessageP)
			{
				//计算并绘制短讯字符数和条数
				calculateAndDrawLengthAndCount(fldContentP, globeP);
				
				//绘制联系人名
				FntSetFont(boldFont);
				WinDrawChars(actionParameterP->fullName, StrLen(actionParameterP->fullName), 20, 16);
				FntSetFont(stdFont);
			}
		}
		
		if (! SysHandleEvent(&event))
		{
			if (! MenuHandleEvent(0, &event, &error))
			{
				switch (event.eType)
				{
					case frmLoadEvent:
					{
						frmMessageP = FrmInitForm(frmMessage);
						FrmSetActiveForm(frmMessageP);
						break;
					}
					case frmOpenEvent:
					{
						//获取控件指针
						fldContentP = (FieldType *)getObjectPtr(frmMessageP, fldContent);
						fldSMSPhoneNumberP = (FieldType *)getObjectPtr(frmMessageP, fldSMSPhoneNumber);
						srlContentP = (ScrollBarType *)getObjectPtr(frmMessageP, srlContent);
						
						//填入电话号码
						FldInsert(fldSMSPhoneNumberP, actionParameterP->phoneNumber, StrLen(actionParameterP->phoneNumber));
						
						//绘制窗口
						FrmDrawForm(frmMessageP);
						
						//绘制联系人名
						FntSetFont(boldFont);
						WinDrawChars(actionParameterP->fullName, StrLen(actionParameterP->fullName), 20, 16);
						FntSetFont(stdFont);
						
						//设置光标
						HsNavObjectTakeFocus(frmMessageP, fldContent);
						FldGrabFocus(fldContentP);
						break;
					}
					case fldChangedEvent: //调整滚动条
					{
						if (event.data.fldChanged.fieldID == fldContent)
						{
							updateScrollBar(fldContentP, srlContentP);
						}						
						break;
					}
					case fldHeightChangedEvent: //调整滚动条
					{
						if (event.data.fldHeightChanged.fieldID == fldContent)
						{
							FldSetScrollPosition(fldContentP, event.data.fldHeightChanged.currentPos);
							updateScrollBar(fldContentP, srlContentP);
						}
						break;
					}
					case sclRepeatEvent: //滚动文本框
					{
						scrollContextField(fldContentP, srlContentP, event.data.sclRepeat.newValue - event.data.sclRepeat.value, false);
						SclHandleEvent(srlContentP, &event);
						break;
					}
					case ctlSelectEvent:
					{
						switch (event.data.ctlSelect.controlID)
						{
							case btnSendMessage:
							{
								if (FldGetTextLength(fldContentP) > 0 && FldGetTextLength(fldSMSPhoneNumberP) > 0)
								{
									sendMessages(actionParameterP, FldGetTextPtr(fldContentP), FldGetTextLength(fldContentP));
									exitCode = EXIT_FOR_NOTHING;
								}
								break;
							}
							case btnCancelMessage:
							{
								exitCode = EXIT_FOR_NOTHING;
								break;
							}
						}
						SysHandleEvent(&event);
					}
					default:
					{
						if (frmMessageP)
						{
							FrmHandleEvent(frmMessageP, &event);
						}
						break;
					}
				}
			}
		}
	} while (event.eType != appStopEvent && exitCode == NO_EXIT);
	
	//退出窗口
	FrmReturnToForm(0);
	
	//释放内存
	MemPtrFree(globeP);
}

#if 0
#pragma mark -
#endif
//****************************************************************************
//启动模块
//****************************************************************************
UInt32 PilotMain(UInt16 cmd, MemPtr cmdPBP, UInt16 launchFlags)
{
	if (cmd == LAUNCH_CODE_ACTION)
	{
		ActionParameterType		*actionParamP	= (ActionParameterType *)cmdPBP;
		void					*pIMEDataP		= NULL;
		UInt32					pIMEDataAddress;
		LocalID					dbID;
		
		//禁用掌易
		if (FtrGet('pIME', 0x01, &pIMEDataAddress) != ftrErrNoSuchFeature)
		{
			DmSearchStateType		s;
			UInt16					cardNo;
			
			//取参数
			pIMEDataP = (void *)pIMEDataAddress;
			
			//取数据库
			if (DmGetNextDatabaseByTypeCreator(true, &s, 'appl', 'pIME', true, &cardNo, &dbID) == dmErrCantFind)
			{
				if (DmGetNextDatabaseByTypeCreator(true, &s, 'panl', 'pIME', true, &cardNo, &dbID) == dmErrCantFind)
				{
					pIMEDataP = NULL;
				}
			}
			
			//禁用输入法
			if (pIMEDataP)
			{
				SysNotifyUnregister(0, dbID, sysNotifyInsPtEnableEvent, sysNotifyNormalPriority);
				SysNotifyUnregister(0, dbID, sysNotifyEventDequeuedEvent, sysNotifyNormalPriority);
			}
		}
		
		if (frmContactEventHandler(actionParamP) == EXIT_FOR_MESSAGE)
		{
			//启用掌易
			if (pIMEDataP)
			{
				MemPtrSetOwner(pIMEDataP, 0);
				SysNotifyRegister (0, dbID, sysNotifyEventDequeuedEvent, NULL, sysNotifyNormalPriority, pIMEDataP);
				SysNotifyRegister (0, dbID, sysNotifyInsPtEnableEvent, NULL, sysNotifyNormalPriority, pIMEDataP);
			}
			
			//用随手Phone发送短讯
			frmMessageEventHandler(actionParamP);
			actionParamP->hasActionToPerform = false;
		}
		else
		{
			//启用掌易
			if (pIMEDataP)
			{
				MemPtrSetOwner(pIMEDataP, 0);
				SysNotifyRegister (0, dbID, sysNotifyEventDequeuedEvent, NULL, sysNotifyNormalPriority, pIMEDataP);
				SysNotifyRegister (0, dbID, sysNotifyInsPtEnableEvent, NULL, sysNotifyNormalPriority, pIMEDataP);
			}
		}
	}

	return errNone;
}