/*
 * EasyInput.c
 *
 * main file for EasyInput
 *
 * This wizard-generated code is based on code adapted from the
 * stationery files distributed as part of the Palm OS SDK 4.0.
 *
 * Copyright (c) 1999-2000 Palm, Inc. or its subsidiaries.
 * All rights reserved.
 */
 
#include <PalmOS.h>
#include <68K\Hs.h>
#include <common\system\palmOneNavigator.h>
#include <common\system\HsKeyCodes.h>
#include "EasyInput.h"
#include "EasyInput_Rsc.h"

#if 0
#pragma mark -
#pragma mark Functions for Common use
#pragma mark -
#endif

#if 0
#pragma mark == MISC FUNCTIONS ==
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
}

static UInt16 getPhoneLabelText(AddressPhoneLabels phoneLabel, GlobeType *globeP)
{
	switch (phoneLabel)
	{
		case workLabel:
		{
			StrCopy(globeP->cache, "(W)");
			break;
		}
		case homeLabel:
		{
			StrCopy(globeP->cache, "(H)");
			break;
		}
		case faxLabel:
		{
			StrCopy(globeP->cache, "(F)");
			break;
		}
		case otherLabel:
		{
			StrCopy(globeP->cache, "(O)");
			break;
		}
		case emailLabel:
		{
			StrCopy(globeP->cache, "(E)");
			break;
		}
		case mainLabel:
		{
			StrCopy(globeP->cache, "(M)");
			break;
		}
		case pagerLabel:
		{
			StrCopy(globeP->cache, "(P)");
			break;
		}
		case mobileLabel:
		{
			StrCopy(globeP->cache, "(M)");
			break;
		}
	}
	
	return FntCharsWidth(globeP->cache, 3);
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

static AddressPhoneLabels getSelectedPhoneLabel(GlobeType *globeP, Int16 itemNum, UInt16 fieldNum)
{
	AddressPhoneLabels		i;
	
	globeP->contactListPP = (ContactUnitType **)MemHandleLock(globeP->contactListH);
	
	if (globeP->settingP->treoModel == TREO_600 || globeP->settingP->treoModel == NOT_A_TREO)
	{
		getContactUnitById_Addr(globeP, globeP->contactListPP[itemNum]->recordId);
		i = OLD_GetPhoneLabel((OLD_AddrDBRecordType *)globeP->contactUnitP, fieldNum);
	}
	else
	{
		getContactUnitById_PAdd(globeP, globeP->contactListPP[itemNum]->recordId);
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

static WChar numericKeyTranslator(UInt16 keyCode, SettingType *settingP)
{
	switch ((WChar)keyCode)
	{
		case keyE:
		{
			return keyOne;
			break;
		}
		case keyR:
		{
			return keyTwo;
			break;
		}
		case keyT:
		{
			return keyThree;
			break;
		}
		case keyD:
		{
			return keyFour;
			break;
		}
		case keyF:
		{
			return keyFive;
			break;
		}
		case keyG:
		{
			return keySix;
			break;
		}
		case keyX:
		{
			return keySeven;
			break;
		}
		case keyC:
		{
			return keyEight;
			break;
		}
		case keyV:
		{
			return keyNine;
			break;
		}
		case hsKeySymbol:
		{
			if (settingP->treoModel == TREO_600)
			{
				return keyZero;
			}
			break;
		}
	}
	
	return (WChar)keyCode;
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
#pragma mark == CONTACT COMMON ACTION HANDLER ==
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

static void getActionParameter(Char *resultP, GlobeType *globeP, UInt8 exitCode)
{
	UInt16			i;
	Int16			selectedListItem	= LstGetSelection(globeP->lstContactsP);
	
	if (selectedListItem != noListSelection)
	{
		if (globeP->contactCount > 0)
		{
			globeP->contactListPP = (ContactUnitType **)MemHandleLock(globeP->contactListH);
			
			MemSet(globeP->cache, 256, 0);
			if (exitCode != EXIT_FOR_PHONEONLY)
			{
				if (globeP->settingP->treoModel == TREO_600 || globeP->settingP->treoModel == NOT_A_TREO)
				{
					getContactUnitById_Addr(globeP, globeP->contactListPP[selectedListItem]->recordId);
					buildFullName_Addr(resultP, globeP->contactUnitP);
				}
				else
				{
					getContactUnitById_PAdd(globeP, globeP->contactListPP[selectedListItem]->recordId);
					buildFullName_PAdd(resultP, globeP->contactUnitP);
				}
			}
			
			if (globeP->settingP->treoModel == TREO_600 || globeP->settingP->treoModel == NOT_A_TREO)
			{
				switch (exitCode)
				{
					case EXIT_FOR_ALL:
					{
						for (i = OLD_phone1; i <= OLD_phone5; i ++)
						{
							if (((OLD_AddrDBRecordType *)globeP->contactUnitP)->fields[i])
							{
								StrCat(resultP, "\n");
								StrCat(resultP, ((OLD_AddrDBRecordType *)globeP->contactUnitP)->fields[i]);
								StrCat(resultP, " ");
								getPhoneLabelText(OLD_GetPhoneLabel((OLD_AddrDBRecordType *)globeP->contactUnitP, i), globeP);
								StrCat(resultP, globeP->cache);
							}
						}
						break;
					}
					case EXIT_FOR_NAME_PHONE:
					{
						StrCat(resultP, "\n");
						StrCat(resultP, ((OLD_AddrDBRecordType *)globeP->contactUnitP)->fields[globeP->currentField]);
						StrCat(resultP, " ");
						getPhoneLabelText(OLD_GetPhoneLabel((OLD_AddrDBRecordType *)globeP->contactUnitP, globeP->currentField), globeP);
						StrCat(resultP, globeP->cache);
						break;
					}
					case EXIT_FOR_PHONEONLY:
					{
						StrCopy(resultP, ((OLD_AddrDBRecordType *)globeP->contactUnitP)->fields[globeP->currentField]);
						StrCat(resultP, " ");
						getPhoneLabelText(OLD_GetPhoneLabel((OLD_AddrDBRecordType *)globeP->contactUnitP, globeP->currentField), globeP);
						StrCat(resultP, globeP->cache);
						break;
					}
				}
			}
			else
			{
				switch (exitCode)
				{
					case EXIT_FOR_ALL:
					{
						for (i = phone1; i <= phone7; i ++)
						{
							if (((AddrDBRecordType *)globeP->contactUnitP)->fields[i])
							{
								StrCat(resultP, "\n");
								StrCat(resultP, ((AddrDBRecordType *)globeP->contactUnitP)->fields[i]);
								StrCat(resultP, " ");
								getPhoneLabelText(GetPhoneLabel((AddrDBRecordType *)globeP->contactUnitP, i), globeP);
								StrCat(resultP, globeP->cache);
							}
						}
						break;
					}
					case EXIT_FOR_NAME_PHONE:
					{
						StrCat(resultP, "\n");
						StrCat(resultP, ((AddrDBRecordType *)globeP->contactUnitP)->fields[globeP->currentField]);
						StrCat(resultP, " ");
						getPhoneLabelText(GetPhoneLabel((AddrDBRecordType *)globeP->contactUnitP, globeP->currentField), globeP);
						StrCat(resultP, globeP->cache);
						break;
					}
					case EXIT_FOR_PHONEONLY:
					{
						StrCopy(resultP, ((AddrDBRecordType *)globeP->contactUnitP)->fields[globeP->currentField]);
						StrCat(resultP, " ");
						getPhoneLabelText(GetPhoneLabel((AddrDBRecordType *)globeP->contactUnitP, globeP->currentField), globeP);
						StrCat(resultP, globeP->cache);
						break;
					}
				}
			}
			
			MemHandleUnlock(globeP->contactListH);
			globeP->contactListPP = NULL;
		}
	}
}

#if 0
#pragma mark == CONTACT DB HANDLER ==
#endif

//****************************************************************************
//读取符合关键字要求的联系人列表
//****************************************************************************
static void getContactList_Addr(Char *PYKeywordP, Char *phoneNumberP, CNLBGlobeType *CNLBglobeP, CNLBParameterType *CNLBparamP, void *voidGlobeP)
{
	GlobeType				*globeP				= (GlobeType *)voidGlobeP;
	UInt16					phoneNumberLength	= StrLen(phoneNumberP);
	Boolean					shouldChangeColor	= true;
	UInt8					theMatchedField;
	
	//构造拼音位封装
	constructPYKeyword(CNLBparamP, PYKeywordP);
	
	//清除通讯记录节点
	freeContactList(globeP);
	
	//锁定列表
	globeP->contactListPP = (ContactUnitType **)MemHandleLock(globeP->contactListH);
	
	//预分配节点
	globeP->contactListPP[0] = MemPtrNew(sizeof(ContactUnitType));
	
	//遍历通讯记录
	CNLBparamP->recordId = 0;
	while (CNLBparamP->recordId < globeP->totalContacts)
	{
		//检查当前记录是否符合拼音
		if (CLibGetMatchContactByRecordId_Addr(globeP->refNum, CNLBglobeP, CNLBparamP, (OLD_AddrDBRecordType *)globeP->contactUnitP, SEARCH_MODE_FULLNAME) == errNone)
		{
			//找到一个拼音匹配的记录
			globeP->contactListPP[globeP->contactCount]->matchType = NORMAL_RESULT;
			globeP->contactListPP[globeP->contactCount]->shouldChangeColor = shouldChangeColor;
			globeP->contactListPP[globeP->contactCount]->recordId = CNLBparamP->recordId;
			shouldChangeColor = ! shouldChangeColor;
			
			//再分配一个节点用于检索
			globeP->contactCount ++;
			globeP->contactListPP[globeP->contactCount] = MemPtrNew(sizeof(ContactUnitType));
		}
		else if (phoneNumberLength >= 3)//检测是否符合电话号码
		{
			for (theMatchedField = OLD_phone1; theMatchedField <= OLD_phone5; theMatchedField ++)
			{
				if (((OLD_AddrDBRecordType *)globeP->contactUnitP)->fields[theMatchedField])
				{
					if (StrStr(((OLD_AddrDBRecordType *)globeP->contactUnitP)->fields[theMatchedField], phoneNumberP))
					{
						//找到一个电话号码匹配的记录
						globeP->contactListPP[globeP->contactCount]->matchType = PHONE_MATCH_RESULT;
						globeP->contactListPP[globeP->contactCount]->shouldChangeColor = shouldChangeColor;
						globeP->contactListPP[globeP->contactCount]->recordId = CNLBparamP->recordId;
						shouldChangeColor = ! shouldChangeColor;
						
						//再分配一个节点用于检索
						globeP->contactCount ++;
						globeP->contactListPP[globeP->contactCount] = MemPtrNew(sizeof(ContactUnitType));
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
static void getContactList_PAdd(Char *PYKeywordP, Char *phoneNumberP, CNLBGlobeType *CNLBglobeP, CNLBParameterType *CNLBparamP, void *voidGlobeP)
{
	GlobeType				*globeP				= (GlobeType *)voidGlobeP;
	UInt16					phoneNumberLength	= StrLen(phoneNumberP);
	Boolean					shouldChangeColor	= true;
	UInt8					theMatchedField;
	
	//构造拼音位封装
	constructPYKeyword(CNLBparamP, PYKeywordP);
	
	//清除通讯记录节点
	freeContactList(globeP);
	
	//锁定列表
	globeP->contactListPP = (ContactUnitType **)MemHandleLock(globeP->contactListH);
	
	//预分配节点
	globeP->contactListPP[0] = MemPtrNew(sizeof(ContactUnitType));
	
	//遍历通讯记录
	CNLBparamP->recordId = 0;
	while (CNLBparamP->recordId < globeP->totalContacts)
	{
		//检查当前记录是否符合拼音
		if (CLibGetMatchContactByRecordId_PAdd(globeP->refNum, CNLBglobeP, CNLBparamP, (AddrDBRecordType *)globeP->contactUnitP, SEARCH_MODE_FULLNAME) == errNone)
		{
			//找到一个拼音匹配的记录
			globeP->contactListPP[globeP->contactCount]->matchType = NORMAL_RESULT;
			globeP->contactListPP[globeP->contactCount]->shouldChangeColor = shouldChangeColor;
			globeP->contactListPP[globeP->contactCount]->recordId = CNLBparamP->recordId;
			shouldChangeColor = ! shouldChangeColor;
			
			//再分配一个节点用于检索
			globeP->contactCount ++;
			globeP->contactListPP[globeP->contactCount] = MemPtrNew(sizeof(ContactUnitType));
		}
		else if (phoneNumberLength >= 3)//检测是否符合电话号码
		{
			for (theMatchedField = phone1; theMatchedField <= phone7; theMatchedField ++)
			{
				if (((AddrDBRecordType *)globeP->contactUnitP)->fields[theMatchedField])
				{
					if (StrStr(((AddrDBRecordType *)globeP->contactUnitP)->fields[theMatchedField], phoneNumberP))
					{
						//找到一个电话号码匹配的记录
						globeP->contactListPP[globeP->contactCount]->matchType = PHONE_MATCH_RESULT;
						globeP->contactListPP[globeP->contactCount]->shouldChangeColor = shouldChangeColor;
						globeP->contactListPP[globeP->contactCount]->recordId = CNLBparamP->recordId;
						shouldChangeColor = ! shouldChangeColor;
						
						//再分配一个节点用于检索
						globeP->contactCount ++;
						globeP->contactListPP[globeP->contactCount] = MemPtrNew(sizeof(ContactUnitType));
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

#if 0
#pragma mark == CONTACT LIST DISPLAY HANDLER ==
#endif

static void drawContactListByContact_Addr(Int16 itemNum, RectangleType *bounds, Char **itemsText)
{
	GlobeType			*globeP		= *(GlobeType **)itemsText;
	
	//锁定列表
	globeP->contactListPP = (ContactUnitType **)MemHandleLock(globeP->contactListH);
	
	//构造姓名
	MemSet(globeP->cache, 256, 0);
	getContactUnitById_Addr(globeP, globeP->contactListPP[itemNum]->recordId);
	buildFullName_Addr(globeP->cache, (OLD_AddrDBRecordType *)globeP->contactUnitP);
	
	//获取范围
	bounds->topLeft.x -= 2;
	bounds->extent.x += 2;
	globeP->contactListPP[itemNum]->bounds = (*bounds);
	
	//显示指定单元的内容
	if (itemNum == LstGetSelection(globeP->lstContactsP))	//选中项
	{
		//显示名称
		WinDrawTruncChars(globeP->cache, StrLen(globeP->cache), bounds->topLeft.x + 10, bounds->topLeft.y, bounds->extent.x - 10);
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
			WinDrawTruncChars(globeP->cache, StrLen(globeP->cache), bounds->topLeft.x + 10, bounds->topLeft.y, bounds->extent.x - 10);
			
			//恢复背景颜色
			WinSetBackColorRGB(&globeP->originalRGBColor, NULL);
		}
		else	//不改变背景颜色
		{
			//显示名称
			WinDrawTruncChars(globeP->cache, StrLen(globeP->cache), bounds->topLeft.x + 10, bounds->topLeft.y, bounds->extent.x - 10);
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
	
	MemHandleUnlock(globeP->contactListH);
	globeP->contactListPP = NULL;
}

static void drawContactListByContact_PAdd(Int16 itemNum, RectangleType *bounds, Char **itemsText)
{
	GlobeType			*globeP		= (GlobeType *)(*itemsText);
	
	//锁定列表
	globeP->contactListPP = (ContactUnitType **)MemHandleLock(globeP->contactListH);
	
	//构造姓名
	MemSet(globeP->cache, 256, 0);
	getContactUnitById_PAdd(globeP, globeP->contactListPP[itemNum]->recordId);
	buildFullName_PAdd(globeP->cache, (AddrDBRecordType *)globeP->contactUnitP);
	
	//获取范围
	bounds->topLeft.x -= 2;
	bounds->extent.x += 2;
	globeP->contactListPP[itemNum]->bounds = (*bounds);
	
	//显示指定单元的内容
	if (itemNum == LstGetSelection(globeP->lstContactsP))	//选中项
	{
		//显示名称
		WinDrawTruncChars(globeP->cache, StrLen(globeP->cache), bounds->topLeft.x + 10, bounds->topLeft.y, bounds->extent.x - 10);
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
			WinDrawTruncChars(globeP->cache, StrLen(globeP->cache), bounds->topLeft.x + 10, bounds->topLeft.y, bounds->extent.x - 10);
			
			//恢复背景颜色
			WinSetBackColorRGB(&globeP->originalRGBColor, NULL);
		}
		else	//不改变背景颜色
		{
			//显示名称
			WinDrawTruncChars(globeP->cache, StrLen(globeP->cache), bounds->topLeft.x + 10, bounds->topLeft.y, bounds->extent.x - 10);
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
	
	MemHandleUnlock(globeP->contactListH);
	globeP->contactListPP = NULL;
}

#if 0
#pragma mark == POPUP EVENT HANDLER ==
#endif

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

static UInt8 popupEventHandler(ListType *lstPopupP, GlobeType *globeP, Int16 item, Char **phoneNumberPP)
{
	EventType		event;
	Boolean			exit				= false;
	UInt8			actionCode			= NO_EXIT;
	Boolean			isKeyHandled		= false;
	Int8			selectIndex			= LstGetSelection(lstPopupP);
	
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
						if (selectIndex == 0)
						{
							actionCode = EXIT_FOR_ALL;
						}
						else
						{
							actionCode = EXIT_FOR_NAME_PHONE;
						}
						exit = true;
					}
					isKeyHandled = true;
					break;
				}
				case keyReturn:			//回车
				{
					if (selectIndex > 0)
					{
						actionCode = EXIT_FOR_PHONEONLY;
						exit = true;
					}
					isKeyHandled = true;
					break;
				}
				default:
				{
					exit = true;
					isKeyHandled = true;
					FrmUpdateForm(frmEasyInputContacts, frmRedrawUpdateCode);
					break;
				}
			}
		}
		
		
		
		if (! isKeyHandled)
		{
			if (! SysHandleEvent(&event))
			{
				FrmDispatchEvent(&event);
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
	
	if (itemNum == 0)
	{
		x = bounds->topLeft.x + (bounds->extent.x - itemWidth) / 2;
		width = itemWidth;
	}
	else
	{
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
	}
	
	WinDrawTruncChars(itemsText[itemNum], itemLength, x, bounds->topLeft.y, width);
}

static UInt8 popupPhoneNumberList(Int16 index, GlobeType *globeP)
{
	UInt8			i;
	UInt8			actionCode			= NO_EXIT;
	ListType		*lstPopupP			= NULL;
	Char			**phoneNumberListPP	= MemPtrNew(32);	//7个电话号码 + 1个选项
	UInt16			phoneNumberCount	= 1;
	
	globeP->contactListPP = (ContactUnitType **)MemHandleLock(globeP->contactListH);
	
	phoneNumberListPP[0] = (Char *)MemPtrNew(19);
	StrCopy(phoneNumberListPP[0], "=====选择全部=====");
	
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
				getPhoneLabelText((OLD_AddressPhoneLabels)OLD_GetPhoneLabel((OLD_AddrDBRecordType *)globeP->contactUnitP, i), globeP);
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
				getPhoneLabelText((AddressPhoneLabels)GetPhoneLabel((AddrDBRecordType *)globeP->contactUnitP, i), globeP);
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
	
	if (phoneNumberCount > 1)
	{
		//计算弹出列表的高度
		globeP->phoneNumberRect.extent.y = phoneNumberCount * 11 + 4;
		
		//调整位置
		globeP->phoneNumberRect.topLeft.y -= (globeP->phoneNumberRect.extent.y / 2);
		if (globeP->phoneNumberRect.topLeft.y < globeP->lstClip.topLeft.y + 1)
		{
			globeP->phoneNumberRect.topLeft.y = globeP->lstClip.topLeft.y + 1;
		}
		else if (globeP->phoneNumberRect.topLeft.y + globeP->phoneNumberRect.extent.y > globeP->lstClip.topLeft.y + globeP->lstClip.extent.y - 2)
		{
			globeP->phoneNumberRect.topLeft.y = globeP->lstClip.topLeft.y + globeP->lstClip.extent.y - globeP->phoneNumberRect.extent.y - 2;
		}
		
		//画列表框底
		WinEraseRectangle(&globeP->phoneNumberRect, 2);
		WinDrawRectangleFrame(popupFrame, &globeP->phoneNumberRect);
		
		setListFrameColor(true, globeP);
		
		//生成列表
		LstNewList((void **)&globeP->frmContactsP, lstPopupId,
					globeP->phoneNumberRect.topLeft.x + 2,
					globeP->phoneNumberRect.topLeft.y + 2,
					100,
					globeP->phoneNumberRect.extent.y - 4,
					stdFont, phoneNumberCount, 0);
		globeP->updateFormControlPtrP((void *)globeP);
		
		lstPopupP = (ListType *)getObjectPtr(globeP->frmContactsP, lstPopupId);
		LstSetDrawFunction(lstPopupP, drawPopupPhoneNumberList);
		LstSetListChoices(lstPopupP, phoneNumberListPP, phoneNumberCount);
		LstDrawList(lstPopupP);
		LstSetSelection(lstPopupP, 0);
		
		actionCode = popupEventHandler(lstPopupP, globeP, index, phoneNumberListPP);
		
		setListFrameColor(false, globeP);
		
		//记录下被选定的电话项
		i = LstGetSelection(lstPopupP);
		if (i > 0)
		{
			globeP->currentField = getSelectedPhoneField(index, i - 1, globeP);
		}
		else
		{
			globeP->currentField = 0;
		}
		
		//删除列表
		LstEraseList(lstPopupP);
		LstSetListChoices(lstPopupP, NULL, 0);
		FrmRemoveObject(&globeP->frmContactsP, FrmGetObjectIndexFromPtr(globeP->frmContactsP, lstPopupP));
		globeP->updateFormControlPtrP((void *)globeP);
	}
	
	for (i = 0; i < phoneNumberCount; i ++)
	{
		MemPtrFree(phoneNumberListPP[i]);
	}
	MemPtrFree(phoneNumberListPP);
	
	return actionCode;
}

#if 0
#pragma mark -
#pragma mark FUNCTIONS FOR EASY INPUT CONTACTS
#pragma mark -
#endif

#if 0
#pragma mark == MISC HANDLER ==
#endif

static void updateFormControlPtr(void *voidGlobeP)
{
	((GlobeType *)voidGlobeP)->frmContactsP = FrmGetActiveForm();
	((GlobeType *)voidGlobeP)->lstContactsP = (ListType *)getObjectPtr(((GlobeType *)voidGlobeP)->frmContactsP, lstContacts);
	((GlobeType *)voidGlobeP)->fldPYKeywordP = (FieldType *)getObjectPtr(((GlobeType *)voidGlobeP)->frmContactsP, fldPYKeyword);
	((GlobeType *)voidGlobeP)->fldPhoneNumberP = (FieldType *)getObjectPtr(((GlobeType *)voidGlobeP)->frmContactsP, fldPhoneNumber);
}

#if 0
#pragma mark == KEY EVENT HANDLER ==
#endif

static void addKeyToPhoneNumber(WChar chr, GlobeType *globeP)
{
	MemHandle		phoneNumberH		= FldGetTextHandle(globeP->fldPhoneNumberP);
	Char			*phoneNumberP		= (Char *)MemHandleLock(phoneNumberH);
	UInt16			phoneNumberLength	= StrLen(phoneNumberP);
	
	FldSetTextHandle(globeP->fldPhoneNumberP, NULL);
	
	if (chr == keyBackspace)
	{
		if (phoneNumberLength > 0)
		{
			phoneNumberP[phoneNumberLength - 1] = '\0';
		}
	}
	else
	{
		if (phoneNumberLength < 10)
		{
			//添加到关键字
			phoneNumberP[phoneNumberLength] = (Char)chr;
			phoneNumberP[phoneNumberLength + 1] = '\0';
		}
	}
	
	MemHandleUnlock(phoneNumberH);
	FldSetTextHandle(globeP->fldPhoneNumberP, phoneNumberH);
}

static void addKeyToKeyword(WChar chr, GlobeType *globeP)
{
	MemHandle		PYKeywordH		= FldGetTextHandle(globeP->fldPYKeywordP);
	Char			*PYKeywordP		= (Char *)MemHandleLock(PYKeywordH);
	UInt16			PYKeywordLength	= StrLen(PYKeywordP);
	
	FldSetTextHandle(globeP->fldPYKeywordP, NULL);
	
	if (chr == keyBackspace)
	{
		if (PYKeywordLength > 0)
		{
			PYKeywordP[PYKeywordLength - 1] = '\0';
		}
	}
	else
	{
		if (PYKeywordLength < 10)
		{
			//添加到关键字
			PYKeywordP[PYKeywordLength] = (Char)chr;
			PYKeywordP[PYKeywordLength + 1] = '\0';
		}
	}
	
	MemHandleUnlock(PYKeywordH);
	FldSetTextHandle(globeP->fldPYKeywordP, PYKeywordH);
}

static Boolean contactKeyHandleEvent(EventType *eventP, GlobeType *globeP, UInt8 *exitCodeP, Boolean *needResetListDrawFunc)
{
	Boolean		isKeyHandled		= false;
	Boolean		shouldPopupList		= false;
	Boolean		isFocusRingDrawed	= true;
	
	if (eventP->eType == keyDownEvent)
	{
		Int16		i, n;
		UInt16		updateCode			= FORM_UPDATE_CUSTOM;
		WChar		chr					= keyTranslator(eventP);
		
		isKeyHandled = true;
		
		switch (chr)
		{
			case chrUpArrow:	//向上，滚动列表
			case keyPageUp:
			case keyRockerUp:
			{
				updateCode = FORM_NO_UPDATE;
				
				//获取列表项数
				n = LstGetNumberOfItems(globeP->lstContactsP);
				
				if (n > 0)
				{
					if (globeP->focusedObjectId != lstContacts)	//焦点不在列表
					{
						//调整焦点
						globeP->focusedObjectId = lstContacts;
						isFocusRingDrawed = false;
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
				}
				break;
			}
			case chrDownArrow:	//向下，滚动列表
			case keyPageDown:
			case keyRockerDown:
			{
				updateCode = FORM_NO_UPDATE;
				
				//获取列表项数
				n = LstGetNumberOfItems(globeP->lstContactsP);
				
				if (n > 0)
				{
					if (globeP->focusedObjectId != lstContacts)	//焦点不在列表
					{
						//调整焦点
						globeP->focusedObjectId = lstContacts;
						isFocusRingDrawed = false;
					}
					
					//获取当前选中项
					i = LstGetSelection(globeP->lstContactsP);
					
					//确定是否需要重绘列表
					if (i - LstGetTopItem(globeP->lstContactsP) + 1 == LstGetNumberOfItems(globeP->lstContactsP))
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
				}
				
				break;
			}
			case chrLeftArrow:	//向左，移动焦点
			case keyRockerLeft:
			{
				isFocusRingDrawed = false;
				switch (globeP->focusedObjectId)
				{
					case btnExitEasyInputContact:
					{
						globeP->focusedObjectId = btnEasyInputContactHelp;
						break;
					}
					case btnEasyInputContactHelp:
					{
						globeP->focusedObjectId = lstContacts;
						break;
					}
				}
				break;
			}
			case chrRightArrow:	//向右，移动焦点
			case keyRockerRight:
			{
				isFocusRingDrawed = false;
				switch (globeP->focusedObjectId)
				{
					case lstContacts:
					{
						globeP->focusedObjectId = btnEasyInputContactHelp;
						LstSetSelection(globeP->lstContactsP, noListSelection);
						break;
					}
					case btnEasyInputContactHelp:
					{
						globeP->focusedObjectId = btnExitEasyInputContact;
						break;
					}
				}
				break;
			}
			case keyRockerCenter:	//中间键
			{
				if (((! (eventP->data.keyDown.modifiers & willSendUpKeyMask)) || globeP->settingP->treoModel == TREO_600) &&
					globeP->focusedObjectId == lstContacts && globeP->contactCount > 0)
				{	//打开联系人细节
					
					shouldPopupList = true;
				}
				else
				{
					isKeyHandled = false;
				}
				break;
			}
			case keyReturn:			//回车键
			{
				if (globeP->contactCount > 0 && LstGetSelection(globeP->lstContactsP) != noListSelection)
				{
					(*exitCodeP) = EXIT_FOR_NAMEONLY;
				}
				break;
			}
			case keyBackspace:		//删除输入的字符
			{
				//调整关键字，并检测是否应该退出
				if (FldGetTextLength(globeP->fldPhoneNumberP) > 0 || FldGetTextLength(globeP->fldPYKeywordP) > 0)
				{
					addKeyToPhoneNumber(chr, globeP);
					addKeyToKeyword(chr, globeP);
					
					//检索通讯录
					(*(globeP->getContactListP))(FldGetTextPtr(globeP->fldPYKeywordP), FldGetTextPtr(globeP->fldPhoneNumberP), globeP->CNLBGlobeP, globeP->CNLBparamP, globeP);
					
					globeP->currentField = 0;
					globeP->focusedObjectId = lstContacts;
					
					isFocusRingDrawed = false;
				}
				else
				{
					(*exitCodeP) = EXIT_FOR_NOTHING;
					updateCode = FORM_NO_UPDATE;
				}
				
				(*needResetListDrawFunc) = true;
				break;
			}
			default:				//添加到关键字并进行检索
			{
				chr = numericKeyTranslator(eventP->data.keyDown.chr, globeP->settingP);
				
				if ((chr >= keyZero && chr <= keyNine) ||
					(eventP->data.keyDown.keyCode >= keyA && eventP->data.keyDown.keyCode <= keyZ))
				{
					if (chr >= keyZero && chr <= keyNine)
					{
						addKeyToPhoneNumber(chr, globeP);
					}
					addKeyToKeyword(eventP->data.keyDown.keyCode, globeP);
					
					//检索通讯录
					(*(globeP->getContactListP))(FldGetTextPtr(globeP->fldPYKeywordP), FldGetTextPtr(globeP->fldPhoneNumberP), globeP->CNLBGlobeP, globeP->CNLBparamP, (void *)globeP);
					
					globeP->currentField = 0;
					globeP->focusedObjectId = lstContacts;
					
					(*needResetListDrawFunc) = true;
					isFocusRingDrawed = false;
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
			if ((*exitCodeP) != NO_EXIT)
			{
				updateCode = FORM_NO_UPDATE;
			}
		}
		
		if (updateCode != FORM_NO_UPDATE)
		{
			FrmUpdateForm(frmEasyInputContacts, updateCode);
		}
		
		if (! isFocusRingDrawed)
		{
			HsNavObjectTakeFocus(globeP->frmContactsP, globeP->focusedObjectId);
			if (globeP->focusedObjectId != lstContacts)
			{
				LstSetSelection(globeP->lstContactsP, noListSelection);
			}
		}
	}
	
	return isKeyHandled;
}

#if 0
#pragma mark == APPLICATION EVENT HANDLER ==
#endif

static Boolean contactAppHandleEvent(EventType *eventP, GlobeType *globeP, UInt8 *exitCodeP, Boolean *needResetListDrawFunc)
{
	Boolean		isEventHandled		= false;
	
	switch (eventP->eType)
	{
		case frmOpenEvent:
		{
			MemHandle		fldTextH;
			Char			*fldTextP;
			//构造窗体
			globeP->frmContactsP = FrmInitForm(frmEasyInputContacts);
			FrmSetActiveForm(globeP->frmContactsP);
			
			globeP->fldPYKeywordP = (FieldType *)getObjectPtr(globeP->frmContactsP, fldPYKeyword);
			fldTextH = MemHandleNew(11);
			fldTextP = (Char *)MemHandleLock(fldTextH);
			MemSet(fldTextP, 11, 0);
			MemHandleUnlock(fldTextH);
			FldSetTextHandle(globeP->fldPYKeywordP, fldTextH);
			
			globeP->fldPhoneNumberP = (FieldType *)getObjectPtr(globeP->frmContactsP, fldPhoneNumber);
			fldTextH = MemHandleNew(256);
			fldTextP = (Char *)MemHandleLock(fldTextH);
			MemSet(fldTextP, 256, 0);
			MemHandleUnlock(fldTextH);
			FldSetTextHandle(globeP->fldPhoneNumberP, fldTextH);
			
			globeP->lstContactsP = (ListType *)getObjectPtr(globeP->frmContactsP, lstContacts);
			FrmGetObjectBounds(globeP->frmContactsP, FrmGetObjectIndex(globeP->frmContactsP, lstContacts), &globeP->lstClip);
			
			globeP->phoneNumberRect.extent.x = 104;
			if (globeP->lstClip.extent.x > 106)
			{
				globeP->phoneNumberRect.topLeft.x = globeP->lstClip.topLeft.x + globeP->lstClip.extent.x - 106;
			}
			else
			{
				globeP->phoneNumberRect.topLeft.x = globeP->lstClip.topLeft.x + 50;
			}
			globeP->typeRect.extent.x = 4;
			globeP->typeRect.extent.y = 4;
			
			if (globeP->initChr != chrNull)
			{
				//添加到关键字
				if (globeP->initKeyCode >= keyA && globeP->initKeyCode <= keyZ)
				{
					addKeyToKeyword(globeP->initKeyCode, globeP);
				}
				
				//添加到号码框
				if (globeP->initChr >= keyZero && globeP->initChr <= keyNine)
				{
					addKeyToPhoneNumber(globeP->initChr, globeP);
				}
			}
			
			//检索通讯录
			(*(globeP->getContactListP))(FldGetTextPtr(globeP->fldPYKeywordP), FldGetTextPtr(globeP->fldPhoneNumberP), globeP->CNLBGlobeP, globeP->CNLBparamP, (void *)globeP);
			
			if (globeP->contactCount > 0)
			{
				globeP->focusedObjectId = lstContacts;
			}
			else
			{
				globeP->focusedObjectId = btnExitEasyInputContact;
			}
			
			(*needResetListDrawFunc) = true;
			FrmUpdateForm(frmEasyInputContacts, frmRedrawUpdateCode);
			
			//设置焦点
			HsNavObjectTakeFocus(globeP->frmContactsP, globeP->focusedObjectId);
			
			isEventHandled = true;
			break;
		}
		case frmUpdateEvent:
		{
			if (eventP->data.frmUpdate.updateCode == frmRedrawUpdateCode)
			{
				FrmDrawForm(globeP->frmContactsP);
			}
			else
			{
				FldDrawField(globeP->fldPYKeywordP);
				FldDrawField(globeP->fldPhoneNumberP);
				LstDrawList(globeP->lstContactsP);
			}
			
			isEventHandled = true;
			break;
		}
		case ctlSelectEvent:
		{
			switch (eventP->data.ctlSelect.controlID)
			{
				case btnEasyInputContactHelp:
				{
					FrmHelp(stringEasyInputContactTip);
					FrmUpdateForm(frmEasyInputContacts, frmRedrawUpdateCode);
					break;
				}
				case btnExitEasyInputContact:
				{
					(*exitCodeP) = EXIT_FOR_NOTHING;
					break;
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

static void freeFieldTextHandle(GlobeType *globeP)
{
	MemHandle		fldTextH;
	
	fldTextH = FldGetTextHandle(globeP->fldPhoneNumberP);
	FldSetTextHandle(globeP->fldPhoneNumberP, NULL);
	MemHandleFree(fldTextH);
	
	fldTextH = FldGetTextHandle(globeP->fldPYKeywordP);
	FldSetTextHandle(globeP->fldPYKeywordP, NULL);
	MemHandleFree(fldTextH);
}

static void easyInputContactHandler(WChar initChr, UInt16 initKeyCode, Char *resultP, SettingType *settingP)
{
	EventType			event;
	UInt16				error					= errNone;
	UInt32				ctx						= 0;
	UInt8				exitCode				= NO_EXIT;
	Boolean				needResetListDrawFunc	= false;
	GlobeType			*globeP					= NULL;
	
	//初始化globe
	globeP = (GlobeType *)MemPtrNew(sizeof(GlobeType));
	MemSet(globeP, sizeof(GlobeType), 0x00);
	globeP->settingP = settingP;
	globeP->initChr = initChr;
	globeP->initKeyCode = initKeyCode;
	globeP->updateFormControlPtrP = updateFormControlPtr;
	
	//打开检索库
	if (CLibLoad(&globeP->refNum, &ctx) == errNone)
	{
		//根据通讯录数据库的类型，确定应调用的函数
		if (globeP->settingP->treoModel == TREO_600 || globeP->settingP->treoModel == NOT_A_TREO)
		{
			globeP->drawContactListByContactP = drawContactListByContact_Addr;
			globeP->getContactListP = getContactList_Addr;
			error = initCNLBGlobeParameter(&globeP->CNLBGlobeP, ADDRESS_VERSION_OLD);
			globeP->contactUnitP = MemPtrNew(sizeof(OLD_AddrDBRecordType));
		}
		else
		{
			globeP->drawContactListByContactP = drawContactListByContact_PAdd;
			globeP->getContactListP = getContactList_PAdd;
			error = initCNLBGlobeParameter(&globeP->CNLBGlobeP, ADDRESS_VERSION_NEW);
			globeP->contactUnitP = MemPtrNew(sizeof(AddrDBRecordType));
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
			
			//分配节点指针
			globeP->contactListH = MemHandleNew(((globeP->totalContacts + 1) * 4));
			
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
			
			//打开窗体
			FrmPopupForm(frmEasyInputContacts);
			
			//事件循环
			do
			{
				EvtGetEvent(&event, evtWaitForever);
				
				//设置列表绘制函数
				if (needResetListDrawFunc)
				{
					if (globeP->contactCount == 0)	//常用联系人列表
					{
						LstSetDrawFunction(globeP->lstContactsP, NULL);
						LstSetListChoices(globeP->lstContactsP, NULL, 0);
						LstSetSelection(globeP->lstContactsP, noListSelection);
					}
					else	//检索结果
					{
						LstSetDrawFunction(globeP->lstContactsP, globeP->drawContactListByContactP);
						LstSetListChoices(globeP->lstContactsP, (Char **)&globeP, globeP->contactCount);
						LstSetSelection(globeP->lstContactsP, 0);
					}
					
					needResetListDrawFunc = false;
				}
				
				if (! contactKeyHandleEvent(&event, globeP, &exitCode, &needResetListDrawFunc))
				{
					if (! SysHandleEvent(&event))
					{
						if (! MenuHandleEvent(0, &event, &error))
						{
							if (! contactAppHandleEvent(&event, globeP, &exitCode, &needResetListDrawFunc))
							{
								FrmDispatchEvent(&event);
							}
						}
					}
				}
			} while(event.eType != appStopEvent && exitCode == NO_EXIT);
			
			//构造返回参数
			if (exitCode != EXIT_FOR_NOTHING && event.eType != appStopEvent)
			{
				getActionParameter(resultP, globeP, exitCode);
			}
			
			freeFieldTextHandle(globeP);
			
			//退出当前窗口
			FrmReturnToForm(0);
			
			//释放内存
			MemPtrFree(globeP->CNLBparamP);
			freeContactList(globeP);
			MemHandleFree(globeP->contactListH);
			MemPtrFree(globeP->contactUnitP);
		}
		
		//关闭数据库
		DmCloseDatabase(globeP->CNLBGlobeP->addressDbRef);
		
		//释放内存
		MemPtrFree(globeP->CNLBGlobeP);
	}
	
	//关闭检索库
	CLibRemove(globeP->refNum, ctx);
	
	MemPtrFree(globeP);
}

#if 0
#pragma mark -
#pragma mark FUNCTIONS FOR EASY INPUT PUNC
#pragma mark -
#endif

#if 0
#pragma mark == PUNC COMMON FUNCTIONS ==
#endif

static UInt32 updateDateTime(UInt32 prevSecond, PuncGlobeType *globeP)
{
	DateTimeType	now;
	Char			second[maxStrIToALen];

	MemSet(globeP->date, maxStrIToALen, 0);
	MemSet(globeP->time, maxStrIToALen, 0);
	TimSecondsToDateTime(TimGetSeconds(), &now);
	DateToAscii(now.month, now.day, now.year, dfYMDWithDashes, globeP->date);
	TimeToAscii(now.hour, now.minute, tfColon24h, globeP->time);
	StrIToA(second, (UInt32)now.second);
	StrCat(globeP->time, ":");
	StrCat(globeP->time, second);
	
	CtlSetLabel(globeP->pbtnDateP, globeP->date);
	CtlSetLabel(globeP->pbtnTimeP, globeP->time);
	
	return 0;
}

#if 0
#pragma mark == PUNC KEY EVENT HANDLER ==
#endif

static Boolean puncKeyHandleEvent(EventType *eventP, PuncGlobeType *globeP, UInt8 *exitCodeP, Char *resultP)
{
	Boolean			isKeyHandled		= false;
	Boolean			shouldChangeFocus	= false;
	
	if (globeP->frmEasyInputPuncP && eventP->eType == keyDownEvent)
	{
		Int16			listSelectedItem	= LstGetSelection(globeP->lstQuickTextP);
		WChar			chr					= keyTranslator(eventP);
		
		switch (chr)
		{
			case chrUpArrow:	//向上
			case keyRockerUp:
			{
				if (globeP->focusedObjectId == lstQuickText)
				{
					if (listSelectedItem > 0)
					{
						listSelectedItem --;
					}
					else
					{
						listSelectedItem = noListSelection;
						globeP->focusedObjectId = pbtnDate;
						shouldChangeFocus = true;
					}
					LstSetSelection(globeP->lstQuickTextP, listSelectedItem);
				}
				else
				{
					switch (globeP->focusedObjectId)
					{
						case pbtnDate:
						{
							globeP->focusedObjectId = pbtnPunc4;
							break;
						}
						case pbtnTime:
						{
							globeP->focusedObjectId = pbtnPunc6;
							break;
						}
						case btnOpenEasyInputContact:
						case btnEasyInputPuncHelp:
						case btnExitEasyInputPunc:
						{
							if (globeP->quickTextCount > 0)
							{
								globeP->focusedObjectId = lstQuickText;
								LstSetSelection(globeP->lstQuickTextP, globeP->quickTextCount - 1);
							}
							else
							{
								globeP->focusedObjectId = pbtnDate;
							}
							break;
						}
						default:
						{
							if (globeP->focusedObjectId > pbtnPunc3)
							{
								globeP->focusedObjectId -= 4;
							}
							break;
						}
					}
					shouldChangeFocus = true;
				}
				isKeyHandled = true;
				break;
			}
			case chrDownArrow:	//向下
			case keyRockerDown:
			{
				if (globeP->focusedObjectId == lstQuickText)
				{
					if (listSelectedItem < globeP->quickTextCount - 1)
					{
						listSelectedItem ++;
					}
					else
					{
						listSelectedItem = noListSelection;
						globeP->focusedObjectId = btnOpenEasyInputContact;
						shouldChangeFocus = true;
					}
					LstSetSelection(globeP->lstQuickTextP, listSelectedItem);
				}
				else
				{
					switch (globeP->focusedObjectId)
					{
						case pbtnDate:
						case pbtnTime:
						{
							if (globeP->quickTextCount > 0)
							{
								globeP->focusedObjectId = lstQuickText;
								LstSetSelection(globeP->lstQuickTextP, 0);
							}
							else
							{
								globeP->focusedObjectId = btnOpenEasyInputContact;
							}
							break;
						}
						default:
						{
							if (globeP->focusedObjectId <= pbtnPunc3)
							{
								globeP->focusedObjectId += 4;
							}
							else if (globeP->focusedObjectId <= pbtnPunc5)
							{
								globeP->focusedObjectId = pbtnDate;
							}
							else if (globeP->focusedObjectId <= pbtnPunc7)
							{
								globeP->focusedObjectId = pbtnTime;
							}
							break;
						}
					}
					shouldChangeFocus = true;
				}
				isKeyHandled = true;
				break;
			}
			case chrLeftArrow:		//向左
			case keyRockerLeft:
			{
				if (globeP->focusedObjectId > pbtnPunc0)
				{
					if (globeP->focusedObjectId == lstQuickText)
					{
						LstSetSelection(globeP->lstQuickTextP, noListSelection);
					}
					globeP->focusedObjectId --;
					if (globeP->focusedObjectId == lstQuickText)
					{
						if (globeP->quickTextCount == 0)
						{
							globeP->focusedObjectId --;
						}
						else
						{
							LstSetSelection(globeP->lstQuickTextP, globeP->quickTextCount - 1);
						}
					}
					shouldChangeFocus = true;
				}
				
				isKeyHandled = true;
				break;
			}
			case chrRightArrow:		//向右
			case keyRockerRight:
			{
				if (globeP->focusedObjectId < btnExitEasyInputPunc)
				{
					if (globeP->focusedObjectId == lstQuickText)
					{
						LstSetSelection(globeP->lstQuickTextP, noListSelection);
					}
					globeP->focusedObjectId ++;
					if (globeP->focusedObjectId == lstQuickText)
					{
						if (globeP->quickTextCount == 0)
						{
							globeP->focusedObjectId ++;
						}
						else
						{
							LstSetSelection(globeP->lstQuickTextP, 0);
						}
					}
					shouldChangeFocus = true;
				}
				
				isKeyHandled = true;
				break;
			}
			case keyRockerCenter:
			{
				if ((! (eventP->data.keyDown.modifiers & willSendUpKeyMask)) || globeP->settingP->treoModel == TREO_600)
				{
					if (globeP->focusedObjectId == lstQuickText)
					{
						(*exitCodeP) = EXIT_FOR_NOTHING;
						StrCopy(globeP->resultP, globeP->quickTextPP[listSelectedItem]);
						
						isKeyHandled = true;
					}
				}
				break;
			}
			case keyBackspace:
			{
				(*exitCodeP) = EXIT_FOR_NOTHING;
				break;
			}
			default:
			{
				chr = numericKeyTranslator(eventP->data.keyDown.chr, globeP->settingP);
				if ((eventP->data.keyDown.keyCode >= keyA && eventP->data.keyDown.keyCode <= keyZ) ||
					(chr >= keyZero && chr <= keyNine))
				{
					(*exitCodeP) = EXIT_FOR_CONTACT_SEARCH;
					globeP->contactInitKeyP->chr = chr;
					globeP->contactInitKeyP->keyCode = eventP->data.keyDown.keyCode;
					globeP->contactInitKeyP->modifiers = 0;
				}
				break;
			}
		}
		
		if (isKeyHandled)
		{
			MemSet(eventP, sizeof(EventType), 0);
			eventP->eType = nilEvent;
		}
		
		if (shouldChangeFocus)
		{
			HsNavObjectTakeFocus(globeP->frmEasyInputPuncP, globeP->focusedObjectId);
		}
	}
	
	return isKeyHandled;
}

#if 0
#pragma mark == PUNC APPLICATION EVENT HANDLER ==
#endif

static Char **getQuickText(UInt16 *quickTextCount)
{
	UInt16		i;
	MemHandle	recordH;
	Char		**result = NULL;
	DmOpenRef	dbRef = DmOpenDatabaseByTypeCreator('SMqt', 'HsCh', dmModeReadOnly);
	
	if (DmGetLastErr() == errNone)
	{
		(*quickTextCount) = DmNumRecordsInCategory(dbRef, dmAllCategories);
		
		if ((*quickTextCount) > 0)
		{
			result = MemPtrNew((*quickTextCount) * 4);
			for (i = 0; i < (*quickTextCount); i ++)
			{
				recordH = DmQueryRecord(dbRef, i);
				result[i] = (Char *)MemPtrNew(MemHandleSize(recordH));
				StrCopy(result[i], (Char *)MemHandleLock(recordH));
				MemHandleUnlock(recordH);
			}
		}
		
		DmCloseDatabase(dbRef);
	}
	else
	{
		(*quickTextCount) = 0;
	}

	return result;
}

static Boolean puncAppHandleEvent(EventType *eventP, PuncGlobeType *globeP, UInt8 *exitCodeP, Char *resultP)
{
	UInt16			i;
	Boolean			isEventHandled		= false;
	
	switch (eventP->eType)
	{
		case frmOpenEvent:	//打开窗体
		{
			//初始化
			globeP->frmEasyInputPuncP = FrmInitForm(frmEasyInputPunc);
			FrmSetActiveForm(globeP->frmEasyInputPuncP);
			
			//符号
			for (i = 0; i < 8; i ++)
			{
				globeP->pbtnPuncP[i] = (ControlType *)getObjectPtr(globeP->frmEasyInputPuncP, (pbtnPunc0 + i));
				CtlSetLabel(globeP->pbtnPuncP[i], globeP->settingP->punc[i]);
			}
			
			//日期时间
			globeP->pbtnDateP = (ControlType *)getObjectPtr(globeP->frmEasyInputPuncP, pbtnDate);
			globeP->pbtnTimeP = (ControlType *)getObjectPtr(globeP->frmEasyInputPuncP, pbtnTime);
			updateDateTime(0, globeP);
			
			//快捷文字
			globeP->lstQuickTextP = (ListType *)getObjectPtr(globeP->frmEasyInputPuncP, lstQuickText);
			globeP->quickTextPP = getQuickText(&globeP->quickTextCount);
			if (globeP->quickTextCount > 0)
			{
				LstSetListChoices(globeP->lstQuickTextP, globeP->quickTextPP, globeP->quickTextCount);
			}
			LstSetSelection(globeP->lstQuickTextP, noListSelection);
			
			//画窗体
			FrmDrawForm(globeP->frmEasyInputPuncP);
			
			//设置焦点
			FrmSetNavState(globeP->frmEasyInputPuncP, kFrmNavStateFlagsObjectFocusMode);
			globeP->focusedObjectId = pbtnPunc0;
			HsNavObjectTakeFocus(globeP->frmEasyInputPuncP, globeP->focusedObjectId);
			
			isEventHandled = true;
			break;
		}
		case frmObjectFocusTakeEvent:
		{
			if (FrmDispatchEvent(eventP))
			{
				//清除被选定的按钮
				for (i = 0; i < 8; i ++)
				{
					CtlSetValue(globeP->pbtnPuncP[i], 0);
				}
				CtlSetValue(globeP->pbtnDateP, 0);
				CtlSetValue(globeP->pbtnTimeP, 0);
				
				if (eventP->data.frmObjectFocusTake.objectID != lstQuickText)
				{
					//选定按钮
					if ((eventP->data.frmObjectFocusTake.objectID >= pbtnPunc0 && eventP->data.frmObjectFocusTake.objectID <= pbtnPunc7) ||
						eventP->data.frmObjectFocusTake.objectID == pbtnDate || eventP->data.frmObjectFocusTake.objectID == pbtnTime)
					{
						CtlSetValue(FrmGetObjectPtr(globeP->frmEasyInputPuncP, FrmGetObjectIndex(globeP->frmEasyInputPuncP, eventP->data.frmObjectFocusTake.objectID)), 1);
					}
				}
			}
			
			isEventHandled = true;
			break;
		}
		case ctlSelectEvent:
		{
			switch (eventP->data.ctlSelect.controlID)
			{
				case btnExitEasyInputPunc:
				{
					(*exitCodeP) = EXIT_FOR_NOTHING;
					break;
				}
				case btnEasyInputPuncHelp:
				{
					FrmHelp(stringEasyInputPuncTip);
					FrmUpdateForm(frmEasyInputPunc, frmRedrawUpdateCode);
					break;
				}
				case btnOpenEasyInputContact:
				{
					(*exitCodeP) = EXIT_FOR_CONTACT_SEARCH;
					globeP->contactInitKeyP->chr = chrNull;
					globeP->contactInitKeyP->keyCode = 0;
					globeP->contactInitKeyP->modifiers = 0;
					/*//退出当前窗体
					LstSetListChoices(globeP->lstQuickTextP, NULL, 0);
					FrmReturnToForm(0);
					globeP->frmEasyInputPuncP = NULL;
					
					//打开联系人查找窗体
					frmContactEventHandler(chrNull, 0, resultP, globeP->settingP);
					
					//返回
					if (resultP[0] != '\0') //直接退出
					{
						(*shouldExit) = true;
					}
					else //重新打开
					{
						FrmReturnToForm(0);
						FrmPopupForm(frmEasyInputPunc);
					}*/
					break;
				}
				default:
				{
					(*exitCodeP) = EXIT_FOR_NOTHING;
					StrCopy(globeP->resultP, CtlGetLabel(FrmGetObjectPtr(globeP->frmEasyInputPuncP, FrmGetObjectIndex(globeP->frmEasyInputPuncP, eventP->data.ctlSelect.controlID))));
					break;
				}
			}
			
			isEventHandled = true;
			break;
		}
		case lstSelectEvent:
		{
			(*exitCodeP) = EXIT_FOR_NOTHING;
			StrCopy(globeP->resultP, globeP->quickTextPP[eventP->data.lstSelect.selection]);
			
			isEventHandled = true;
			break;
		}
	}
	
	return isEventHandled;
}

#if 0
#pragma mark == PUNC MAIN EVENT HANDLER ==
#endif

static UInt8 easyInputPuncHandler(Char *resultP, SettingType *settingP, KeyInfoType *contactInitKeyP)
{
	GrfStatusType		grfStatus;
	EventType			event;
	PuncGlobeType		*globeP		= MemPtrNew(sizeof(PuncGlobeType));
	UInt8				exitCode	= NO_EXIT;
	UInt32				now			= TimGetSeconds();
	UInt32				oneSecond	= SysTicksPerSecond();
	
	//保存并重置Grf状态
	HsGrfGetStateExt(&grfStatus.capsLock, &grfStatus.numLock, &grfStatus.optLock, &grfStatus.tempShift, &grfStatus.autoShifted);
	HsGrfSetStateExt(false, false, false, false, false, false);
	
	MemSet(globeP, sizeof(PuncGlobeType), 0);
	globeP->settingP = settingP;
	globeP->resultP = resultP;
	globeP->contactInitKeyP = contactInitKeyP;
	
	FrmPopupForm(frmEasyInputPunc);
	
	do
	{
		EvtGetEvent(&event, oneSecond);
		
		if (globeP->frmEasyInputPuncP)
		{
			now = updateDateTime(now, globeP);
		}
		
		if (! puncKeyHandleEvent(&event, globeP, &exitCode, resultP))
		{
			if (! SysHandleEvent(&event))
			{
				if (! puncAppHandleEvent(&event, globeP, &exitCode, resultP))
				{
					FrmDispatchEvent(&event);
				}
			}
		}
	} while (event.eType != appStopEvent && exitCode == NO_EXIT);
	
	FrmReturnToForm(0);
	
	//恢复Grf状态
	HsGrfSetStateExt(grfStatus.capsLock, grfStatus.numLock, grfStatus.optLock, false, false, false);
	
	//释放内存
	if (globeP->quickTextCount > 0)
	{
		UInt16		i;
		
		for (i = 0; i < globeP->quickTextCount; i ++)
		{
			MemPtrFree(globeP->quickTextPP[i]);
		}
		MemPtrFree(globeP->quickTextPP);
	}
	MemPtrFree(globeP);
	
	return exitCode;
}

#if 0
#pragma mark -
#pragma mark FUNCTIONS FOR EASY INPUT SMS CONTACT SEARCH
#pragma mark -
#endif

#if 0
#pragma mark == MISC HANDLER ==
#endif

static void SMSGetActionParameter(Char *resultP, GlobeType *globeP)
{
	Int16			selectedListItem	= LstGetSelection(globeP->lstContactsP);
	
	if (selectedListItem != noListSelection)
	{
		if (globeP->contactCount > 0)
		{
			globeP->contactListPP = (ContactUnitType **)MemHandleLock(globeP->contactListH);
			
			if (globeP->settingP->treoModel == TREO_600 || globeP->settingP->treoModel == NOT_A_TREO)
			{
				getContactUnitById_Addr(globeP, globeP->contactListPP[selectedListItem]->recordId);
				StrCopy(resultP, ((OLD_AddrDBRecordType *)globeP->contactUnitP)->fields[globeP->currentField]);
			}
			else
			{
				getContactUnitById_PAdd(globeP, globeP->contactListPP[selectedListItem]->recordId);
				StrCopy(resultP, ((AddrDBRecordType *)globeP->contactUnitP)->fields[globeP->currentField]);
			}
			
			MemHandleUnlock(globeP->contactListH);
			globeP->contactListPP = NULL;
		}
		else
		{
			StrCopy(resultP, globeP->recentListP[selectedListItem].phoneNumber);
		}
	}
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

static void saveContactToRecentDB(GlobeType *globeP)
{
	Int16			selectedListItem	= LstGetSelection(globeP->lstContactsP);
	
	//获取姓名和号码
	if (selectedListItem != noListSelection)
	{
		Boolean			isContactExist = false;
		UInt16			recordId;
		UInt16			newRecordId;
		MemHandle		recordH;
		RecentUnitType	*recordP;
		Char			*p1;
		UInt16			p1Length;
		Char			*fullNameP		= MemPtrNew(256);
		Char			*phoneNumberP	= MemPtrNew(128);
		
		if (globeP->contactCount > 0)
		{
			globeP->contactListPP = (ContactUnitType **)MemHandleLock(globeP->contactListH);
			
			MemSet(fullNameP, 256, 0);
			MemSet(phoneNumberP, 128, 0);
			if (globeP->settingP->treoModel == TREO_600 || globeP->settingP->treoModel == NOT_A_TREO)
			{
				getContactUnitById_Addr(globeP, globeP->contactListPP[selectedListItem]->recordId);
				buildFullName_Addr(fullNameP, (OLD_AddrDBRecordType *)globeP->contactUnitP);
				StrCopy(phoneNumberP, ((OLD_AddrDBRecordType *)globeP->contactUnitP)->fields[globeP->currentField]);
				getPhoneLabelText(OLD_GetPhoneLabel((OLD_AddrDBRecordType *)globeP->contactUnitP, globeP->currentField), globeP);
			}
			else
			{
				getContactUnitById_PAdd(globeP, globeP->contactListPP[selectedListItem]->recordId);
				buildFullName_PAdd(fullNameP, (AddrDBRecordType *)globeP->contactUnitP);
				StrCopy(phoneNumberP, ((AddrDBRecordType *)globeP->contactUnitP)->fields[globeP->currentField]);
				getPhoneLabelText(GetPhoneLabel((AddrDBRecordType *)globeP->contactUnitP, globeP->currentField), globeP);
			}
			StrCat(fullNameP, " ");
			StrCat(fullNameP, globeP->cache);
			
			MemHandleUnlock(globeP->contactListH);
			globeP->contactListPP = NULL;
		}
		else
		{
			StrCopy(fullNameP, globeP->recentListP[selectedListItem].fullName);
			StrCopy(phoneNumberP, globeP->recentListP[selectedListItem].phoneNumber);
		}
		
		//检测联系人是否已经存在
		p1 = phoneNumberP;
		p1Length = StrLen(phoneNumberP);
		if (p1Length > 8)
		{
			p1 += (p1Length - 8);
		}
		for (recordId = 0; recordId < globeP->recentCount; recordId ++)
		{
			if (StrCompare(globeP->recentListP[recordId].fullName, fullNameP) == 0 && StrCompare(globeP->recentListP[recordId].phoneNumber, phoneNumberP) == 0)
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
			
			//填入信息
			DmWrite(recordP, 0, fullNameP, 256);
			DmWrite(recordP, 256, phoneNumberP, 128);
			
			//解锁并释放记录
			MemHandleUnlock(recordH);
			DmReleaseRecord(globeP->recentDbRef, recordId, true);
			
			//移动记录到记录头
			DmMoveRecord(globeP->recentDbRef, recordId, 0);
		}
		
		MemPtrFree(phoneNumberP);
		MemPtrFree(fullNameP);
	}
}

#if 0
#pragma mark == UI DISPLAY EVENT HANDLER ==
#endif

/*static void drawPYKeyword(GlobeType *globeP)
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
		WinDrawChars(globeP->pyKeyword, globeP->pyKeywordLength, globeP->PYKeywordRect.topLeft.x + (globeP->PYKeywordRect.extent.x - keywordWidth) / 2, globeP->PYKeywordRect.topLeft.y);
		
		//恢复背景颜色
		WinSetBackColorRGB(&globeP->originalRGBColor, NULL);
	}
}*/

static void SMSUpdateFormControlPtr(void *voidGlobeP)
{
	((GlobeType *)voidGlobeP)->frmContactsP = FrmGetActiveForm();
	((GlobeType *)voidGlobeP)->lstContactsP = (ListType *)getObjectPtr(((GlobeType *)voidGlobeP)->frmContactsP, lstContactsId);
}

static void createContactList(FieldType *fldSMSPhoneNumberP, GlobeType *globeP)
{
	UInt16		numberOfItems;
	UInt16		i;
	
	//计算列表大小
	FldGetBounds(fldSMSPhoneNumberP, &globeP->lstClip);
	globeP->lstClip.topLeft.x -= 29;
	globeP->lstClip.extent.x += 28;
	globeP->lstClip.topLeft.y += (globeP->lstClip.extent.y + 1);
	numberOfItems = (130 - globeP->lstClip.topLeft.y) / 11;
	if (numberOfItems < 3)
	{
		numberOfItems = 3;
		globeP->lstClip.topLeft.y = 95;
	}
	globeP->lstClip.extent.y = numberOfItems * 11 + 2;
	
	//创建列表
	globeP->frmContactsP = FrmNewForm(frmContactsId, NULL,
									globeP->lstClip.topLeft.x - 1,
									globeP->lstClip.topLeft.y - 1,
									globeP->lstClip.extent.x + 2,
									globeP->lstClip.extent.y,
									false, 0, 0, 0);
	globeP->lstClip.topLeft.x = 1;
	globeP->lstClip.topLeft.y = 1;
	i = LstNewList((void **)&globeP->frmContactsP, lstContactsId,
				1,
				1,
				globeP->lstClip.extent.x,
				globeP->lstClip.extent.y,
				stdFont, numberOfItems, 0);
	
	globeP->lstContactsP = (ListType *)getObjectPtr(globeP->frmContactsP, lstContactsId);
	
	FrmSetActiveForm(globeP->frmContactsP);
}

static void drawContactListByRecent(Int16 itemNum, RectangleType *bounds, Char **itemsText)
{
	GlobeType	*globeP;
	
	//重获全局指针
	globeP = *(GlobeType **)itemsText;
	
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

static void drawPYAndPhone(GlobeType *globeP)
{
	if (/*LstGetSelection(globeP->lstContactsP) == noListSelection &&*/
		globeP->pyKeywordLength > 0 || globeP->phoneNumberLength > 0)
	{
		UInt16		keywordWidth		= FntCharsWidth(globeP->pyKeyword, globeP->pyKeywordLength);
		UInt16		phoneNumberWidth	= FntCharsWidth(globeP->phoneNumberP, globeP->phoneNumberLength);
		
		//计算区域
		if (keywordWidth > phoneNumberWidth)
		{
			globeP->PYKeywordRect.extent.x = keywordWidth + 4;
		}
		else
		{
			globeP->PYKeywordRect.extent.x = phoneNumberWidth + 4;
		}
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
		if (globeP->pyKeywordLength > 0)
		{
			WinDrawChars(globeP->pyKeyword, globeP->pyKeywordLength, globeP->PYKeywordRect.topLeft.x + (globeP->PYKeywordRect.extent.x - keywordWidth) / 2/*globeP->PYKeywordRect.topLeft.x + 2*/, globeP->PYKeywordRect.topLeft.y);
		}
		if (globeP->phoneNumberLength > 0)
		{
			WinDrawChars(globeP->phoneNumberP, globeP->phoneNumberLength, globeP->PYKeywordRect.topLeft.x + (globeP->PYKeywordRect.extent.x - phoneNumberWidth) / 2/*globeP->PYKeywordRect.topLeft.x + 2*/, globeP->PYKeywordRect.topLeft.y + 12);
		}
		
		//恢复背景颜色
		WinSetBackColorRGB(&globeP->originalRGBColor, NULL);
	}
}

#if 0
#pragma mark == POPUP EVENT HANDLER ==
#endif

static void SMSSetListFrameColor(Boolean shouldSetToWhite, GlobeType *globeP)
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

static UInt8 SMSPopupEventHandler(ListType *lstPopupP, GlobeType *globeP, Int16 item, Char **phoneNumberPP)
{
	EventType		event;
	//UInt16			phoneNumberLength;
	Boolean			exit				= false;
	UInt8			actionCode			= NO_EXIT;
	Int8			selectIndex			= LstGetSelection(lstPopupP);
	Boolean			isKeyHandled		= false;
	
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
						actionCode = EXIT_FOR_PHONEONLY;
						exit = true;
					}
					isKeyHandled = true;
					break;
				}
				default:
				{
					exit = true;
					isKeyHandled = true;
					FrmUpdateForm(frmContactsId, frmRedrawUpdateCode);
					break;
				}
			}
		}
		
		
		
		if (! isKeyHandled)
		{
			if (! SysHandleEvent(&event))
			{
				FrmDispatchEvent(&event);
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

static void SMSDrawPopupPhoneNumberList(Int16 itemNum, RectangleType *bounds, Char **itemsText)
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

static UInt8 SMSPopupPhoneNumberList(Int16 index, GlobeType *globeP)
{
	UInt8			i;
	UInt8			actionCode			= NO_EXIT;
	ListType		*lstPopupP			= NULL;
	Char			**phoneNumberListPP	= MemPtrNew(28);	//7个电话号码
	UInt16			phoneNumberCount	= 0;
	
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
				getPhoneLabelText((OLD_AddressPhoneLabels)OLD_GetPhoneLabel((OLD_AddrDBRecordType *)globeP->contactUnitP, i), globeP);
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
				getPhoneLabelText((AddressPhoneLabels)GetPhoneLabel((AddrDBRecordType *)globeP->contactUnitP, i), globeP);
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
		if (globeP->phoneNumberRect.topLeft.y < globeP->lstClip.topLeft.y + 1)
		{
			globeP->phoneNumberRect.topLeft.y = globeP->lstClip.topLeft.y + 1;
		}
		else if (globeP->phoneNumberRect.topLeft.y + globeP->phoneNumberRect.extent.y > globeP->lstClip.topLeft.y + globeP->lstClip.extent.y - 2)
		{
			globeP->phoneNumberRect.topLeft.y = globeP->lstClip.topLeft.y + globeP->lstClip.extent.y - globeP->phoneNumberRect.extent.y - 2;
		}
		
		//画列表框底
		WinEraseRectangle(&globeP->phoneNumberRect, 2);
		WinDrawRectangleFrame(popupFrame, &globeP->phoneNumberRect);
		
		SMSSetListFrameColor(true, globeP);
		
		//生成列表
		LstNewList((void **)&globeP->frmContactsP, lstPopupId,
					globeP->phoneNumberRect.topLeft.x + 2,
					globeP->phoneNumberRect.topLeft.y + 2,
					100,
					globeP->phoneNumberRect.extent.y - 4,
					stdFont, phoneNumberCount, 0);
		globeP->updateFormControlPtrP((void *)globeP);
		
		lstPopupP = (ListType *)getObjectPtr(globeP->frmContactsP, lstPopupId);
		LstSetDrawFunction(lstPopupP, SMSDrawPopupPhoneNumberList);
		LstSetListChoices(lstPopupP, phoneNumberListPP, phoneNumberCount);
		LstDrawList(lstPopupP);
		LstSetSelection(lstPopupP, 0);
		
		actionCode = SMSPopupEventHandler(lstPopupP, globeP, index, phoneNumberListPP);
		
		SMSSetListFrameColor(false, globeP);
		
		//记录下被选定的电话项
		i = LstGetSelection(lstPopupP);
		if (i >= 0)
		{
			globeP->currentField = getSelectedPhoneField(index, i, globeP);
		}
		else
		{
			globeP->currentField = 0;
		}
		
		//删除列表
		LstEraseList(lstPopupP);
		LstSetListChoices(lstPopupP, NULL, 0);
		FrmRemoveObject(&globeP->frmContactsP, FrmGetObjectIndexFromPtr(globeP->frmContactsP, lstPopupP));
		globeP->updateFormControlPtrP((void *)globeP);
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

static void SMSAddKeyToPhoneNumber(WChar chr, GlobeType *globeP)
{
	if (chr == keyBackspace)
	{
		if (globeP->phoneNumberLength > 0)
		{
			globeP->phoneNumberLength --;
			globeP->phoneNumberP[globeP->phoneNumberLength] = '\0';
		}
	}
	else
	{
		if (globeP->phoneNumberLength < 10)
		{
			//添加到关键字
			globeP->phoneNumberP[globeP->phoneNumberLength] = (Char)chr;
			globeP->phoneNumberLength ++;
			globeP->phoneNumberP[globeP->phoneNumberLength] = '\0';
		}
	}
}

static void SMSAddKeyToKeyword(WChar chr, GlobeType *globeP)
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
			globeP->pyKeyword[globeP->pyKeywordLength] = '\0';
		}
	}
}

static Boolean SMSKeyHandleEvent(EventType *eventP, GlobeType *globeP, UInt8 *exitCodeP, Boolean *needResetListDrawFunc)
{
	Boolean		isKeyHandled		= false;
	Boolean		shouldPopupList		= false;
	Boolean		isFocusRingDrawed	= true;
	
	if (eventP->eType == keyDownEvent)
	{
		Int16		i, n;
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
					
					if (i == noListSelection || i == 0)
					{
						i = n - 1;
					}
					else
					{
						i --;
					}
					LstSetSelection(globeP->lstContactsP, i);
					updateCode = FORM_UPDATE_CUSTOM;
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
					
					if (i == noListSelection || i + 1 == n)
					{
						i = 0;
					}
					else
					{
						i ++;
					}
					LstSetSelection(globeP->lstContactsP, i);
					updateCode = FORM_UPDATE_CUSTOM;
				}
				
				break;
			}
			case chrLeftArrow:	//向左，退出
			case keyRockerLeft:
			{
				(*exitCodeP) = EXIT_FOR_NOTHING;
				break;
			}
			case chrRightArrow:	//向右，退出
			case keyRockerRight:
			{
				(*exitCodeP) = EXIT_FOR_NOTHING;
				break;
			}
			case keyRockerCenter:	//中间键
			{
				if ((! (eventP->data.keyDown.modifiers & willSendUpKeyMask)) || globeP->settingP->treoModel == TREO_600)
				{
					if (globeP->contactCount > 0)
					{
						//打开联系人细节
						shouldPopupList = true;
					}
					else if (globeP->recentCount > 0)
					{
						//返回常用联系人
						(*exitCodeP) = EXIT_FOR_PHONEONLY;
					}
				}
				else
				{
					isKeyHandled = false;
				}
				break;
			}
			case keyBackspace:		//删除输入的字符
			{
				//调整关键字，并检测是否应该退出
				if (globeP->phoneNumberLength > 0 || globeP->pyKeywordLength > 0)
				{
					SMSAddKeyToPhoneNumber(chr, globeP);
					SMSAddKeyToKeyword(chr, globeP);
					
					if ((globeP->phoneNumberLength > 0 || globeP->pyKeywordLength > 0)/* || globeP->recentCount == 0*/)
					{
						//检索通讯录
						(*(globeP->getContactListP))(globeP->pyKeyword, globeP->phoneNumberP, globeP->CNLBGlobeP, globeP->CNLBparamP, globeP);
					}
					
					globeP->currentField = 0;
					updateCode = frmRedrawUpdateCode;
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
				chr = numericKeyTranslator(eventP->data.keyDown.chr, globeP->settingP);
				
				if ((chr >= keyZero && chr <= keyNine) ||
					(eventP->data.keyDown.keyCode >= keyA && eventP->data.keyDown.keyCode <= keyZ))
				{
					if (chr >= keyZero && chr <= keyNine)
					{
						SMSAddKeyToPhoneNumber(chr, globeP);
					}
					SMSAddKeyToKeyword(eventP->data.keyDown.keyCode, globeP);
					
					//检索通讯录
					(*(globeP->getContactListP))(globeP->pyKeyword, globeP->phoneNumberP, globeP->CNLBGlobeP, globeP->CNLBparamP, (void *)globeP);
					
					globeP->currentField = 0;
					updateCode = frmRedrawUpdateCode;
					
					(*needResetListDrawFunc) = true;
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
			(*exitCodeP) = SMSPopupPhoneNumberList(LstGetSelection(globeP->lstContactsP), globeP);
		}
		
		if (updateCode != FORM_NO_UPDATE)
		{
			FrmUpdateForm(frmContactsId, updateCode);
		}
	}
	
	return isKeyHandled;
}

#if 0
#pragma mark == APPLICATION EVENT HANDLER ==
#endif

static Boolean SMSAppHandleEvent(EventType *eventP, GlobeType *globeP, UInt8 *exitCodeP, Boolean *needResetListDrawFunc)
{
	Boolean		isEventHandled		= false;
	
	switch (eventP->eType)
	{
		case frmOpenEvent:
		{
			//构造窗体
			createContactList(globeP->fldPhoneNumberP, globeP);
			
			FrmGetObjectBounds(globeP->frmContactsP, FrmGetObjectIndex(globeP->frmContactsP, lstContactsId), &globeP->lstClip);
			globeP->phoneNumberRect.extent.x = 104;
			if (globeP->lstClip.extent.x > 106)
			{
				globeP->phoneNumberRect.topLeft.x = globeP->lstClip.topLeft.x + globeP->lstClip.extent.x - 106;
			}
			else
			{
				globeP->phoneNumberRect.topLeft.x = globeP->lstClip.topLeft.x + 50;
			}
			globeP->PYKeywordRect.topLeft.y = globeP->lstClip.topLeft.y;
			globeP->PYKeywordRect.extent.y = 24;
			globeP->typeRect.extent.x = 4;
			globeP->typeRect.extent.y = 4;
			
			//取常用联系人列表
			getRecentList(globeP);
			
			if (globeP->recentCount == 0)
			{
				//检索通讯录
				(*(globeP->getContactListP))(globeP->pyKeyword, globeP->phoneNumberP, globeP->CNLBGlobeP, globeP->CNLBparamP, (void *)globeP);
			}
			
			(*needResetListDrawFunc) = true;
			
			FrmUpdateForm(frmContactsId, frmRedrawUpdateCode);
			
			isEventHandled = true;
			break;
		}
		case frmUpdateEvent:
		{
			FrmDrawForm(globeP->frmContactsP);
			
			if (eventP->data.frmUpdate.updateCode != FORM_UPDATE_CUSTOM)
			{
				drawPYAndPhone(globeP);
			}
			
			isEventHandled = true;
			break;
		}
	}
	return isEventHandled;
}

#if 0
#pragma mark == MAIN EVENT HANDLER ==
#endif

static void easyInputSMSContactSearchHandler(FieldType *fldSMSPhoneNumberP, Char *resultP, SettingType *settingP)
{
	EventType			event;
	GrfStatusType		grfStatus;
	UInt16				error					= errNone;
	UInt32				ctx						= 0;
	UInt8				exitCode				= NO_EXIT;
	Boolean				needResetListDrawFunc	= false;
	GlobeType			*globeP					= NULL;
	
	//初始化globe
	globeP = (GlobeType *)MemPtrNew(sizeof(GlobeType));
	MemSet(globeP, sizeof(GlobeType), 0x00);
	globeP->settingP = settingP;
	globeP->recentListP = (RecentUnitType *)MemPtrNew((sizeof(RecentUnitType) * 10));
	globeP->phoneNumberP = (Char *)MemPtrNew(128);
	MemSet(globeP->phoneNumberP, 128, 0);
	globeP->updateFormControlPtrP = SMSUpdateFormControlPtr;
	globeP->fldPhoneNumberP = fldSMSPhoneNumberP;
	
	
	//打开检索库
	if (CLibLoad(&globeP->refNum, &ctx) == errNone)
	{
		//保存并重置Grf状态
		HsGrfGetStateExt(&grfStatus.capsLock, &grfStatus.numLock, &grfStatus.optLock, &grfStatus.tempShift, &grfStatus.autoShifted);
		HsGrfSetStateExt(false, false, false, false, false, false);
		
		//根据通讯录数据库的类型，确定应调用的函数
		if (globeP->settingP->treoModel == TREO_600 || globeP->settingP->treoModel == NOT_A_TREO)
		{
			globeP->drawContactListByContactP = drawContactListByContact_Addr;
			globeP->getContactListP = getContactList_Addr;
			error = initCNLBGlobeParameter(&globeP->CNLBGlobeP, ADDRESS_VERSION_OLD);
			globeP->contactUnitP = MemPtrNew(sizeof(OLD_AddrDBRecordType));
		}
		else
		{
			globeP->drawContactListByContactP = drawContactListByContact_PAdd;
			globeP->getContactListP = getContactList_PAdd;
			error = initCNLBGlobeParameter(&globeP->CNLBGlobeP, ADDRESS_VERSION_NEW);
			globeP->contactUnitP = MemPtrNew(sizeof(AddrDBRecordType));
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
			
			//分配节点指针
			globeP->contactListH = MemHandleNew(((globeP->totalContacts + 1) * 4));
			
			//打开数据库
			globeP->recentDbRef = DmOpenDatabaseByTypeCreator('RECT', appFileCreator, dmModeReadWrite);
			if (DmGetLastErr() == dmErrCantFind) //最近联系人列表库不存在
			{
				DmCreateDatabase(0, "Ezi Recent Contacts", appFileCreator, 'RECT', false);
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
			
			//画列表
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
						if (globeP->recentCount > 0 && (globeP->phoneNumberLength == 0 && globeP->pyKeywordLength == 0))
						{
							LstSetDrawFunction(globeP->lstContactsP, drawContactListByRecent);
							LstSetListChoices(globeP->lstContactsP, (Char **)&globeP, globeP->recentCount);
							LstSetSelection(globeP->lstContactsP, 0);
						}
						else
						{
							LstSetDrawFunction(globeP->lstContactsP, NULL);
							LstSetListChoices(globeP->lstContactsP, NULL, 0);
							LstSetSelection(globeP->lstContactsP, noListSelection);
						}
					}
					else	//检索结果
					{
						LstSetDrawFunction(globeP->lstContactsP, globeP->drawContactListByContactP);
						LstSetListChoices(globeP->lstContactsP, (Char **)&globeP, globeP->contactCount);
						LstSetSelection(globeP->lstContactsP, 0);
					}
					
					needResetListDrawFunc = false;
				}
				
				if (! SMSKeyHandleEvent(&event, globeP, &exitCode, &needResetListDrawFunc))
				{
					if (! SysHandleEvent(&event))
					{
						if (! MenuHandleEvent(0, &event, &error))
						{
							if (! SMSAppHandleEvent(&event, globeP, &exitCode, &needResetListDrawFunc))
							{
								FrmDispatchEvent(&event);
							}
						}
					}
				}
			} while(event.eType != appStopEvent && exitCode == NO_EXIT);
			
			//构造返回参数
			if (exitCode != EXIT_FOR_NOTHING && event.eType != appStopEvent)
			{
				SMSGetActionParameter(resultP, globeP);
				saveContactToRecentDB(globeP);
			}
			
			//清除列表
			FrmReturnToForm(0);
			
			//关闭数据库
			DmCloseDatabase(globeP->recentDbRef);
			
			//释放内存
			MemPtrFree(globeP->phoneNumberP);
			MemPtrFree(globeP->CNLBparamP);
			freeContactList(globeP);
			MemHandleFree(globeP->contactListH);
			MemPtrFree(globeP->contactUnitP);
		}
		
		//关闭数据库
		DmCloseDatabase(globeP->CNLBGlobeP->addressDbRef);
		
		//释放内存
		MemPtrFree(globeP->CNLBGlobeP);
		
		//恢复Grf状态
		HsGrfSetStateExt(grfStatus.capsLock, grfStatus.numLock, grfStatus.optLock, false, false, false);
	}
	
	//关闭检索库
	CLibRemove(globeP->refNum, ctx);
	MemPtrFree(globeP->recentListP);
	MemPtrFree(globeP);
}

#if 0
#pragma mark -
#endif

UInt32 PilotMain(UInt16 cmd, MemPtr cmdPBP, UInt16 launchFlags)
{
	if (cmd == LAUNCH_CODE_ACTION)
	{
		ActionParameterType		*actionParamP	= (ActionParameterType *)cmdPBP;
		Char					*resultP = (Char *)MemPtrNew(1024);
			
		MemSet(resultP, 1024, 0x00);
		
		switch (actionParamP->settingP->actionCode)
		{
			case EASY_INPUT_KEY_ACTIVE:
			{
				FormType	*currentForm = FrmGetActiveForm();
				
				if (currentForm)
				{
					UInt16		objectIndex = FrmGetFocus(currentForm);
					UInt16		objectType = FrmGetObjectType(currentForm, objectIndex);
					FieldType	*currentField = NULL;
					
					if (objectType == frmFieldObj)
					{
						currentField = (FieldType *)FrmGetObjectPtr(currentForm, objectIndex);
					}
					else if (objectType == frmTableObj)
					{
						TableType		*currentTable = (TableType *)FrmGetObjectPtr(currentForm, objectIndex);
						
						currentField = TblGetCurrentField(currentTable);
					}
					
					if (currentField)
					{
						KeyInfoType		key	= {0, 0, 0};
						
						while (easyInputPuncHandler(resultP, actionParamP->settingP, &key) == EXIT_FOR_CONTACT_SEARCH)
						{
							easyInputContactHandler(key.chr, key.keyCode, resultP, actionParamP->settingP);
							if (resultP[0] != '\0')
							{
								break;
							}
						}
						if (StrLen(resultP) > 0 && (FldGetMaxChars(currentField) - FldGetTextLength(currentField)) > StrLen(resultP))
						{
							FldInsert(currentField, resultP, StrLen(resultP));
						}
					}
				}
				
				break;
			}
			case EASY_INPUT_SMS_FIELD_ACTIVE:
			{
				FormType	*currentForm = FrmGetActiveForm();
				
				if (currentForm)
				{
					FieldType		*currentField = (FieldType *)FrmGetObjectPtr(currentForm, FrmGetFocus(currentForm));
					
					if (currentField)
					{
						easyInputSMSContactSearchHandler(currentField, resultP, actionParamP->settingP);
						
						if (StrLen(resultP) > 0)
						{
							FldInsert(currentField, resultP, StrLen(resultP));
						}
					}
				}
				
				break;
			}
		}
		
		MemPtrFree(resultP);
	}

	return errNone;
}