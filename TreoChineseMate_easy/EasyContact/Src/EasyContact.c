/*
 * EasyContact.c
 *
 * main file for EasyContact
 *
 * This wizard-generated code is based on code adapted from the
 * stationery files distributed as part of the Palm OS SDK 4.0.
 *
 * Copyright (c) 1999-2000 Palm, Inc. or its subsidiaries.
 * All rights reserved.
 */
 
#include <PalmOS.h>
#include <Helper.h>
#include <HelperServiceClass.h>
#include <68K\Hs.h>

#include "EasyContact.h"
#include "EasyContact_Rsc.h"

//****************************************************************************
//释放通讯记录节点的内存
//****************************************************************************
static void freeContactList(GlobeType *globeP)
{
	UInt16	i;
	
	if (globeP->contactListH && globeP->totalContacts > 0)
	{
		globeP->contactList = (void **)MemHandleLock(globeP->contactListH);
		
		//循环释放节点内存
		for (i = 0; i < globeP->contactCount; i ++)
		{
			MemPtrFree(globeP->contactList[i]);
		}
		
		MemHandleUnlock(globeP->contactListH);
	}
	
	//重置节点计数器
	globeP->contactCount = 0;
}

static void buildFullName_Addr(Char *cache, OLD_AddrDBRecordType *data)
{
	MemSet(cache, 128, 0);
	
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
	MemSet(cache, 128, 0);
	
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

static void getLastName_Addr(Char *cache, OLD_AddrDBRecordType *data)
{
	buildFullName_Addr(cache, data);
	
	if (((UInt8)(*cache)) > 127)	//中文
	{
		cache += 2;
	}
	else
	{
		cache ++;
	}
	
	MemSet(cache, StrLen(cache), 0);
}
static void getLastName_PAdd(Char *cache, AddrDBRecordType *data)
{
	buildFullName_PAdd(cache, data);
	
	if (((UInt8)(*cache)) > 127)	//中文
	{
		cache += 2;
	}
	else
	{
		cache ++;
	}
	
	MemSet(cache, StrLen(cache), 0);
}

//****************************************************************************
//读取符合关键字要求的联系人列表
//****************************************************************************
static void getContactList_Addr(CNLBGlobeType *CNLBglobeP, CNLBParameterType *CNLBparamP, GlobeType *globeP, UInt8 mode)
{
	OLD_AddrDBRecordType	*unit;
	
	//清除通讯记录节点
	freeContactList(globeP);
	
	if (globeP->contactListH && globeP->totalContacts > 0)
	{
		globeP->contactList = (void **)MemHandleLock(globeP->contactListH);
		
		//分配临时单元
		unit = (OLD_AddrDBRecordType *)MemPtrNew(sizeof(OLD_AddrDBRecordType));
		
		//遍历通讯记录
		CNLBparamP->recordId = 0;
		while (CNLBparamP->recordId < globeP->totalContacts)
		{
			//检查当前记录是否符合拼音
			if (CLibGetMatchContactByRecordId_Addr(globeP->refNum, CNLBglobeP, CNLBparamP, unit, mode) == errNone)
			{ //找到一个匹配的记录
				//分配单元内存
				globeP->contactList[globeP->contactCount] = MemPtrNew(sizeof(OLD_AddrDBRecordType));
				
				//获取记录
				MemMove(globeP->contactList[globeP->contactCount], unit, sizeof(OLD_AddrDBRecordType));
				globeP->contactCount ++;
			}
			
			refreshCNLBGlobeParameter(CNLBglobeP);
			CNLBparamP->recordId ++;
		}
		
		//释放和解锁内存
		MemPtrFree(unit);
		MemHandleUnlock(globeP->contactListH);
	}
}
static void getContactList_PAdd(CNLBGlobeType *CNLBglobeP, CNLBParameterType *CNLBparamP, GlobeType *globeP, UInt8 mode)
{
	AddrDBRecordType		*unit;
	
	//清除通讯记录节点
	freeContactList(globeP);
	
	if (globeP->contactListH && globeP->totalContacts > 0)
	{
		globeP->contactList = (void **)MemHandleLock(globeP->contactListH);
		
		//分配临时单元
		unit = (AddrDBRecordType *)MemPtrNew(sizeof(AddrDBRecordType));
		
		//遍历通讯记录
		CNLBparamP->recordId = 0;
		while (CNLBparamP->recordId < globeP->totalContacts)
		{
			//检查当前记录是否符合要求
			if (CLibGetMatchContactByRecordId_PAdd(globeP->refNum, CNLBglobeP, CNLBparamP, unit, mode) == errNone)
			{ //找到一个匹配的记录
				//分配单元内存
				globeP->contactList[globeP->contactCount] = MemPtrNew(sizeof(AddrDBRecordType));
				
				//获取记录
				MemMove(globeP->contactList[globeP->contactCount], unit, sizeof(AddrDBRecordType));
				
				globeP->contactCount ++;
			}
			refreshCNLBGlobeParameter(CNLBglobeP);
			CNLBparamP->recordId ++;
		}
		
		//释放和解锁内存
		MemPtrFree(unit);
		MemHandleUnlock(globeP->contactListH);
	}
}

static void clearFieldText(GlobeType *globeP)
{
	UInt16		i = 0;
	
	while (*globeP->fldRTextP[i])
	{
		globeP->fldRTextP[i][0] = 0;
		globeP->fldRTextP[i][1] = 0;
		i ++;
	}
}

//****************************************************************************
//检查给定的姓是否已经存在于文本框集中
//****************************************************************************
static Boolean isLastNameExist(Char *nameToTest, GlobeType *globeP)
{
	UInt16		i;
	
	for (i = 0; i < 45; i ++)
	{
		if (*(globeP->fldRTextP[i]))
		{
			if (globeP->fldRTextP[i][0] == nameToTest[0] && globeP->fldRTextP[i][1] == nameToTest[1])
			{
				return true;
			}
		}
		else
		{
			break;
		}
	}
	
	return false;
}

//****************************************************************************
//把检索出来的姓氏绑定到文本框集
//****************************************************************************
static void bindLastNameToField_Addr(GlobeType *globeP)
{
	UInt16		i = 0;
	UInt16		j = 0;
	
	globeP->usedField = 0;
	
	if (globeP->totalContacts > 0)
	{
		clearFieldText(globeP);
		
		globeP->contactList = (void **)MemHandleLock(globeP->contactListH);
		
		while (i < globeP->contactCount && j < 45)
		{
			//构造姓氏
			getLastName_Addr(globeP->cache, ((OLD_AddrDBRecordType **)globeP->contactList)[i]);
			
			if (! isLastNameExist(globeP->cache, globeP))
			{
				StrNCopy(globeP->fldRTextP[j], globeP->cache, 2);
				j ++;
				globeP->usedField ++;
			}
			
			i ++;
		}
		
		if (j < 45)
		{
			for ( ; j < 45; j ++)
			{
				globeP->fldRTextP[j][0] = 0x00; globeP->fldRTextP[j][1] = 0x00;
			}
		}
		
		MemHandleUnlock(globeP->contactListH);
	}
}
static void bindLastNameToField_PAdd(GlobeType *globeP)
{
	UInt16		i = 0;
	UInt16		j = 0;
	
	globeP->usedField = 0;
	if (globeP->totalContacts > 0)
	{
		clearFieldText(globeP);
		
		globeP->contactList = (void **)MemHandleLock(globeP->contactListH);
		
		while (i < globeP->contactCount && j < 45)
		{
			//构造姓氏
			getLastName_PAdd(globeP->cache, ((AddrDBRecordType **)globeP->contactList)[i]);
			
			if (! isLastNameExist(globeP->cache, globeP))
			{
				StrNCopy(globeP->fldRTextP[j], globeP->cache, 2);
				j ++;
				globeP->usedField ++;
			}
			
			i ++;
		}
		
		if (j < 45)
		{
			for ( ; j < 45; j ++)
			{
				globeP->fldRTextP[j][0] = 0x00; globeP->fldRTextP[j][1] = 0x00;
			}
		}
		
		MemHandleUnlock(globeP->contactListH);
	}
}

//****************************************************************************
//画关键字和分隔线
//****************************************************************************
static void drawKeyword(GlobeType *globeP)
{
	RectangleType	r;
	WChar			chr;
	UInt16			chrWidth;
	FontID			currentFont;
	
	r.topLeft.x = 2; r.topLeft.y = 2; r.extent.x = 13; r.extent.y = 20;
	WinEraseRectangle(&r, 0);
	
	if (globeP->pyKeywordP[0] >= 97 && globeP->pyKeywordP[0] <= 122)
	{
		chr = globeP->pyKeywordP[0] - 32;
	}
	else
	{
		chr = globeP->pyKeywordP[0];
	}
	
	currentFont = FntSetFont(largeBoldFont);
	
	chrWidth = FntCharWidth(chr);
	
	WinDrawChar(chr, 7 - (chrWidth > 1), 2);
	
	FntSetFont(currentFont);
	
	WinDrawLine(20, 2, 20, 58);
}

//****************************************************************************
//设置高亮的文本框
//****************************************************************************
static void setHighLightField(GlobeType *globeP, UInt8 move)
{
	UInt16		maxRow = 0;
	UInt16		maxLastRowCol = 0;
	UInt16		i = globeP->usedField;
	
	if (globeP->contactCount > 0)
	{
		while (i > 9)
		{
			maxRow ++;
			i -= 9;
		}
		maxLastRowCol = i - 1;
		
		//根据指令移动行列
		switch (move)
		{
			case MOVE_UP:
			{
				if (globeP->row > 0)
				{
					globeP->row --;
				}
				else
				{
					globeP->row = maxRow;
				}
				
				break;
			}
			case MOVE_DOWN:
			{
				if (globeP->row < maxRow)
				{
					globeP->row ++;
				}
				else
				{
					globeP->row = 0;
				}
				
				break;
			}
			case MOVE_LEFT:
			{
				if (globeP->col > 0)
				{
					globeP->col --;
				}
				else
				{
					globeP->col = 8;
				}
				
				break;
			}
			case MOVE_RIGHT:
			{
				if (globeP->row == maxRow)
				{
					if (globeP->col < maxLastRowCol)
					{
						globeP->col ++;
					}
					else
					{
						globeP->col = 0;
					}
				}
				else
				{
					if (globeP->col < 8)
					{
						globeP->col ++;
					}
					else
					{
						globeP->col = 0;
					}
				}
				break;
			}
		}
		
		//调整行列至有结果的文本框
		do
		{
			if (*globeP->fldRTextP[(globeP->row * 9) + globeP->col])
			{
				break;
			}
			else
			{
				if (globeP->col > 0)
				{
					globeP->col --;
				}
				else
				{
					if (globeP->row > 0)
					{
						globeP->row --;
						globeP->col = 8;
					}
				}
			}
		} while (globeP->row > 0 || globeP->col > 0);
	}
}

//****************************************************************************
//获取文本框内容指针
//****************************************************************************
static void getPointerOfFieldTextHandle(GlobeType *globeP)
{
	UInt16		i;
	
	for (i = r0; i <= r44; i ++)
	{
		FldSetTextHandle(globeP->fldRP[i - r0], NULL);
		globeP->fldRTextP[i - r0] = (Char *)MemHandleLock(globeP->fldRTextH[i - r0]);
	}
}

//****************************************************************************
//绑定文本框内容指针
//****************************************************************************
static void setHandleToFieldText(GlobeType *globeP)
{
	UInt16		i;
	
	for (i = r0; i <= r44; i ++)
	{
		MemHandleUnlock(globeP->fldRTextH[i - r0]);
		FldSetTextHandle(globeP->fldRP[i - r0], globeP->fldRTextH[i - r0]);
	}
}

//****************************************************************************
//百家姓检索模式
//****************************************************************************
static WChar lastNameSearchEventHandler(SettingType *settingP, WChar initKey)
{
	EventType			event;
	UInt16				error;
	UInt16				i;
	UInt32				ctx = 0;
	WChar				result = 0;
	Boolean				exitCode = false;
	Boolean				isKeyHandled = false;
	FormType			*frmContactP = NULL;
	GlobeType			*globeP = NULL;
	WChar				key;
	CNLBGlobeType		*CNLBglobeP = NULL;
	CNLBParameterType	*CNLBparamP = NULL;
	void				(*bindLastNameToFieldP)(GlobeType *globeP);
	void				(*getContactListP)(CNLBGlobeType *CNLBglobeP, CNLBParameterType *CNLBparamP, GlobeType *globeP, UInt8 mode);
	
	//初始化globe
	globeP = (GlobeType *)MemPtrNew(sizeof(GlobeType));
	MemSet(globeP, sizeof(GlobeType), 0x00);
	globeP->settingP = settingP;
	globeP->pyKeywordP = (Char *)MemPtrNew(11);
	MemSet(globeP->pyKeywordP, 11, 0);
	globeP->pyKeywordLength = 1;
	
	//打开检索库
	if (CLibLoad(&globeP->refNum, &ctx) == errNone)
	{
		//根据通讯录数据库的类型，确定应调用的函数
		if (globeP->settingP->treoModel == TREO_600 || globeP->settingP->treoModel == NOT_A_TREO)
		{
			bindLastNameToFieldP = bindLastNameToField_Addr;
			getContactListP = getContactList_Addr;
			error = initCNLBGlobeParameter(&CNLBglobeP, ADDRESS_VERSION_OLD);
		}
		else
		{
			bindLastNameToFieldP = bindLastNameToField_PAdd;
			getContactListP = getContactList_PAdd;
			error = initCNLBGlobeParameter(&CNLBglobeP, ADDRESS_VERSION_NEW);
		}
		
		//预初始化全局变量，打开通讯录数据库
		if (! error)
		{
			//预构造参数
			CNLBparamP = (CNLBParameterType *)MemPtrNew(sizeof(CNLBParameterType));
			MemSet(CNLBparamP, sizeof(CNLBParameterType), 0x00);
			CNLBparamP->orderlySearch = true;
			
			//获取通讯记录总数
			globeP->totalContacts = DmNumRecordsInCategory(CNLBglobeP->addressDbRef, dmAllCategories);
			
			//分配节点指针
			globeP->contactListH = MemHandleNew((globeP->totalContacts << 2));
			
			//打开窗体
			FrmPopupForm(frmLastNameSearch);
			
			do
			{
				EvtGetEvent(&event, evtWaitForever);
				
				if (event.eType == keyDownEvent && frmContactP)
				{
					//获取文本框指针集
					getPointerOfFieldTextHandle(globeP);
					
					if (event.data.keyDown.chr >= keyA && event.data.keyDown.chr <= keyZ)
					{
						globeP->pyKeywordP[0] = (Char)event.data.keyDown.chr;
						
						//检索通讯录
						constructPYKeyword(CNLBparamP, globeP->pyKeywordP);
						(*getContactListP)(CNLBglobeP, CNLBparamP, globeP, SEARCH_MODE_LASTNAME);
						
						//绑定到文本框
						bindLastNameToFieldP(globeP);
						
						//画关键字
						drawKeyword(globeP);
						
						//设置高亮的文本框
						globeP->row = 0;
						globeP->col = 0;
						
						isKeyHandled = true;
					}
					else
					{
						if (EvtKeydownIsVirtual(&event) && event.data.keyDown.keyCode > 0)
						{
							key = event.data.keyDown.keyCode;
						}
						else
						{
							key = event.data.keyDown.chr;
						}
						switch (key)
						{
							case chrUpArrow:
							case keyPageUp:
							case keyRockerUp: //上键，移动联系人列表选择项
							{
								setHighLightField(globeP, MOVE_UP);
								
								//标志按键已处理
								isKeyHandled = true;
								break;
							}
							case chrDownArrow:
							case keyPageDown:
							case keyRockerDown: //下键，移动联系人列表选择项
							{
								setHighLightField(globeP, MOVE_DOWN);
								
								//标志按键已处理
								isKeyHandled = true;
								break;
							}
							case chrLeftArrow:
							case vchrRockerLeft: //左键，清除全部电话号码和拼音关键字，重新显示最近联系人列表
							{
								setHighLightField(globeP, MOVE_LEFT);
								
								//标志按键已处理
								isKeyHandled = true;
								break;
							}
							case chrRightArrow:
							case vchrRockerRight:
							{
								setHighLightField(globeP, MOVE_RIGHT);
								
								//标志按键已处理
								isKeyHandled = true;
								break;
							}
							case vchrRockerCenter: //中键，获取联系人信息，或者获取选定的号码
							{
								if (! (event.data.keyDown.modifiers & willSendUpKeyMask))
								{
									if (globeP->usedField > 0)
									{
										i = globeP->row * 9 + globeP->col;
										if (((UInt8)globeP->fldRTextP[i][0]) <= 127)
										{
											result = (WChar)globeP->fldRTextP[i][0];
										}
										else
										{
											MemMove(&result, globeP->fldRTextP[i], 2);
										}
									}
									
									exitCode = true;
								}
								
								//标志按键已处理
								isKeyHandled = true;
								break;
							}
							case keyBackspace: //删除
							{
								exitCode = true;
								
								//标志按键已处理
								isKeyHandled = true;
								break;
							}
							case keySpace: //空格键，获取选定的姓氏
							{
								if (globeP->usedField > 0)
								{
									i = globeP->row * 9 + globeP->col;
									if (((UInt8)globeP->fldRTextP[i][0]) <= 127)
									{
										result = (WChar)globeP->fldRTextP[i][0];
									}
									else
									{
										MemMove(&result, globeP->fldRTextP[i], 2);
									}
								}
								
								exitCode = true;
								
								//标志按键已处理
								isKeyHandled = true;
								break;
							}
						}
					}
					
					//绑定文本框指针集
					setHandleToFieldText(globeP);
					
					//清除按键事件
					if (isKeyHandled)
					{
						MemSet(&event, sizeof(EventType), 0x00);
						event.eType = nilEvent;
						isKeyHandled = false;
						
						if (exitCode == false)
						{
							//重绘窗体
							FrmUpdateForm(frmLastNameSearch, frmRedrawUpdateCode);
						}
					}
					else
					{
						if (! SysHandleEvent(&event))
						{
							if (! MenuHandleEvent(0, &event, &error))
							{
								FrmHandleEvent(frmContactP, &event);
							}
						}
					}
				}
				else if (! SysHandleEvent(&event))
				{
					if (! MenuHandleEvent(0, &event, &error))
					{
						switch (event.eType)
						{
							case frmLoadEvent: //装载窗体
							{
								frmContactP = FrmInitForm(frmLastNameSearch);
								FrmSetActiveForm(frmContactP);
								break;
							}
							case frmOpenEvent: //显示窗体
							{
								//获取文本框指针、赋予内存
								for (i = r0; i <= r44; i ++)
								{
									globeP->fldRP[i - r0] = (FieldType *)getObjectPtr(frmContactP, i);
									globeP->fldRTextH[i - r0] = MemHandleNew(3);
									globeP->fldRTextP[i - r0] = MemHandleLock(globeP->fldRTextH[i - r0]);
									MemSet(globeP->fldRTextP[i - r0], 3, 0x00);
								}
								
								//置入拼音关键字缓存
								globeP->pyKeywordP[0] = (Char)initKey;
								
								//检索通讯录
								constructPYKeyword(CNLBparamP, globeP->pyKeywordP);
								(*getContactListP)(CNLBglobeP, CNLBparamP, globeP, SEARCH_MODE_LASTNAME);
								
								//绑定到文本框
								bindLastNameToFieldP(globeP);
								
								//设置文本框指针
								setHandleToFieldText(globeP);
								
								//画窗体
								FrmDrawForm(frmContactP);
								
								//画关键字和分隔线
								drawKeyword(globeP);
								
								if (globeP->contactCount > 0)
								{
									FldSetSelection(globeP->fldRP[0], 0, FldGetTextLength(globeP->fldRP[0]));
								}
								
								break;
							}
							case frmUpdateEvent:
							{
								if (frmContactP)
								{
									for (i = 0; i < 45; i ++)
									{
										FldDrawField(globeP->fldRP[i]);
									}
									drawKeyword(globeP);
									
									if (globeP->contactCount > 0)
									{
										FldSetSelection(globeP->fldRP[(globeP->row * 9) + globeP->col], 0, FldGetTextLength(globeP->fldRP[(globeP->row * 9) + globeP->col]));
									}
								}
								break;
							}
							default:
							{
								if (frmContactP)
								{
									FrmHandleEvent(frmContactP, &event);
								}
								break;
							}
						}
					}
				}
			}while(event.eType != appStopEvent && exitCode == false);
			
			//关闭数据库
			DmCloseDatabase(CNLBglobeP->addressDbRef);
			
			//释放内存
			for (i = r0; i <= r44; i ++)
			{
				FldSetTextHandle(globeP->fldRP[i - r0], NULL);
				MemHandleFree(globeP->fldRTextH[i - r0]);
			}
			MemPtrFree(CNLBparamP);
			MemPtrFree(CNLBglobeP);
			freeContactList(globeP);
			if (globeP->contactListH && globeP->totalContacts > 0)
			{
				MemHandleFree(globeP->contactListH);
			}
			FrmReturnToForm(0);
		}
		
		//关闭检索库
		CLibRemove(globeP->refNum, ctx);
	}
	else
	{
		FrmAlert(alertLoadPYLibraryError);
	}
	
	MemPtrFree(globeP->pyKeywordP);
	MemPtrFree(globeP);

	return result;
}

//****************************************************************************
//检查按键是否是数字
//****************************************************************************
/*static Boolean isTheKeyValid(WChar theKey, GlobeType *globeP, WChar *numericKey)
{
	switch (theKey)
	{
		case keyE:
		{
			(*numericKey) = keyOne;
			break;
		}
		case keyR:
		{
			(*numericKey) = keyTwo;
			break;
		}
		case keyT:
		{
			(*numericKey) = keyThree;
			break;
		}
		case keyD:
		{
			(*numericKey) = keyFour;
			break;
		}
		case keyF:
		{
			(*numericKey) = keyFive;
			break;
		}
		case keyG:
		{
			(*numericKey) = keySix;
			break;
		}
		case keyX:
		{
			(*numericKey) = keySeven;
			break;
		}
		case keyC:
		{
			(*numericKey) = keyEight;
			break;
		}
		case keyV:
		{
			(*numericKey) = keyNine;
			break;
		}
		case keyZero:
		{
			(*numericKey) = keyZero;
			break;
		}
		default:
		{
			if (globeP->settingP->treoModel == TREO_600 && theKey == hsKeySymbol)
			{
				(*numericKey) = keyZero;
			}
			else if (theKey >= keyZero && theKey <= keyNine)
			{
				(*numericKey) = theKey;
			}
			else if (theKey >= keyA && theKey <= keyZ)
			{
				(*numericKey) = NOT_A_NUMBER;
			}
			else
			{
				return false;
			}
			break;
		}
	}
	
	return true;
}

//****************************************************************************
//构造返回参数
//****************************************************************************
static Err getActionParameter(GlobeType *globeP, Char **resultP)
{
	Int16		currentListSelection = LstGetSelection(globeP->lstContactsP);
	
	if (currentListSelection != noListSelection)
	{
		globeP->contactList = (void **)MemHandleLock(globeP->contactListH);
		
		if (globeP->settingP->treoModel == TREO_600 || globeP->settingP->treoModel == NOT_A_TREO)
		{
			(*resultP) = (Char *)MemPtrNew(((OLD_CNLBContactType **)globeP->contactList)[currentListSelection]->fullNameLength);
			StrCopy((*resultP), ((OLD_CNLBContactType **)globeP->contactList)[currentListSelection]->fullName);
		}
		else
		{
			(*resultP) = (Char *)MemPtrNew(((CNLBContactType **)globeP->contactList)[currentListSelection]->fullNameLength);
 			StrCopy((*resultP), ((CNLBContactType **)globeP->contactList)[currentListSelection]->fullName);
		}
		
		MemHandleUnlock(globeP->contactListH);
	}
	else
	{
		return 1;
	}
	
	return errNone;
}

//****************************************************************************
//画联系人列表（显示符合检索结果的联系人）
//****************************************************************************
static void drawContactListByContact_Addr(Int16 itemNum, RectangleType *bounds, Char **itemsText)
{
	GlobeType			*globeP;
	
	//重获全局指针
	globeP = *(GlobeType **)itemsText;
	
	globeP->contactList = (void **)MemHandleLock(globeP->contactListH);
	
	buildFullName_Addr(globeP->cache, &((ContactUnitType_PAdd **)globeP->contactList)[itemNum]->data);
	
	//显示名称
	WinDrawTruncChars(globeP->cache, StrLen(globeP->cache), bounds->topLeft.x, bounds->topLeft.y, bounds->extent.x);

	MemHandleUnlock(globeP->contactListH);
}
static void drawContactListByContact_PAdd(Int16 itemNum, RectangleType *bounds, Char **itemsText)
{
	GlobeType			*globeP;
	
	//重获全局指针
	globeP = *(GlobeType **)itemsText;
	
	globeP->contactList = (void **)MemHandleLock(globeP->contactListH);
	
	buildFullName_PAdd(globeP->cache, &((ContactUnitType_PAdd **)globeP->contactList)[itemNum]->data);
	
	//显示名称
	WinDrawTruncChars(globeP->cache, StrLen(globeP->cache), bounds->topLeft.x, bounds->topLeft.y, bounds->extent.x);

	MemHandleUnlock(globeP->contactListH);
}

//****************************************************************************
//获取文本框内容指针
//****************************************************************************
static void getPointerOfFieldTextHandleFirstName(FieldType *fldPYKeywordP, GlobeType *globeP)
{
	FldSetTextHandle(fldPYKeywordP, NULL);
	globeP->pyKeywordP = (Char *)MemHandleLock(globeP->pyKeywordH);
}

//****************************************************************************
//绑定文本框内容指针
//****************************************************************************
static void setHandleToFieldTextFirstName(FieldType *fldPYKeywordP, GlobeType *globeP)
{
	MemHandleUnlock(globeP->pyKeywordH);
	FldSetTextHandle(fldPYKeywordP, globeP->pyKeywordH);
}

static void performListSelectionMovement(ListType *lstContactsP, UInt8 movement, GlobeType *globeP)
{
	Int16		currentSelection = LstGetSelection(lstContactsP);
	
	switch (movement)
	{
		case MOVE_UP:
		{
			if (currentSelection == noListSelection || currentSelection == 0)
			{
				currentSelection = globeP->contactCount - 1;
			}
			else
			{
				currentSelection --;
			}
			
			break;
		}
		case MOVE_DOWN:
		{
			if (currentSelection == noListSelection || currentSelection == globeP->contactCount - 1)
			{
				currentSelection = 0;
			}
			else
			{
				currentSelection ++;
			}
			
			break;
		}
	}
	
	LstSetSelection(lstContactsP, currentSelection);
}

//****************************************************************************
//名字检索模式
//****************************************************************************
static Char *firstNameSearchEventHandler(SettingType *settingP, WChar initKey)
{
	Char				*resultP = NULL;
	EventType			event;
	UInt16				error;
	UInt32				ctx = 0;
	Boolean				exit = false;
	Boolean				isKeyHandled = false;
	FormType			*frmContactP = NULL;
	FieldType			*fldPYKeywordP = NULL;
	ListType			*lstContactsP = NULL;
	GlobeType			*globeP = NULL;
	WChar				key;
	WChar				numericKey = NOT_A_NUMBER;
	CNLBGlobeType		*CNLBglobeP = NULL;
	CNLBParameterType	*CNLBparamP = NULL;
	void				(*drawContactListByContactP)(Int16 itemNum, RectangleType *bounds, Char **itemsText);
	void				(*getContactListP)(CNLBGlobeType *CNLBglobeP, CNLBParameterType *CNLBparamP, GlobeType *globeP, UInt8 mode);
	
	//初始化globe
	globeP = (GlobeType *)MemPtrNew(sizeof(GlobeType));
	MemSet(globeP, sizeof(GlobeType), 0x00);
	globeP->settingP = settingP;
	
	//打开检索库
	if (CLibLoad(&globeP->refNum, &ctx) == errNone)
	{
		//根据通讯录数据库的类型，确定应调用的函数
		if (globeP->settingP->treoModel == TREO_600 || globeP->settingP->treoModel == NOT_A_TREO)
		{
			drawContactListByContactP = drawContactListByContact_Addr;
			getContactListP = getContactList_Addr;
			error = initCNLBGlobeParameter(&CNLBglobeP, ADDRESS_VERSION_OLD);
		}
		else
		{
			drawContactListByContactP = drawContactListByContact_PAdd;
			getContactListP = getContactList_PAdd;
			error = initCNLBGlobeParameter(&CNLBglobeP, ADDRESS_VERSION_NEW);
		}
		
		//预初始化全局变量，打开通讯录数据库
		if (! error)
		{
			//预构造参数
			CNLBparamP = (CNLBParameterType *)MemPtrNew(sizeof(CNLBParameterType));
			MemSet(CNLBparamP, sizeof(CNLBParameterType), 0x00);
			CNLBparamP->orderlySearch = globeP->settingP->orderlySearch;
			
			//获取通讯记录总数
			globeP->totalContacts = DmNumRecordsInCategory(CNLBglobeP->addressDbRef, dmAllCategories);
			
			//分配节点指针
			globeP->contactListH = MemHandleNew((globeP->totalContacts << 2));
			
			//打开窗体
			FrmPopupForm(frmFirstNameSearch);
			
			do
			{
				EvtGetEvent(&event, evtWaitForever);
				
				if (event.eType == keyDownEvent && frmContactP)
				{						
					//处理按键
					if (isTheKeyValid(event.data.keyDown.chr, globeP, &numericKey))
					{ //是A-Z
						//获取文本框指针
						getPointerOfFieldTextHandleFirstName(fldPYKeywordP, globeP);
						
						//置入拼音关键字缓存
						if (globeP->pyKeywordLength < 10)
						{
							globeP->pyKeywordP[globeP->pyKeywordLength] = (Char)event.data.keyDown.chr;
							globeP->pyKeywordLength ++;
							
							//检索通讯录
							constructPYKeyword(CNLBparamP, globeP->pyKeywordP);
							(*getContactListP)(CNLBglobeP, CNLBparamP, globeP, SEARCH_MODE_FIRSTNAME);
							
							//重置通讯记录列表
							LstSetDrawFunction(lstContactsP, drawContactListByContactP);
							LstSetListChoices(lstContactsP, (Char **)&globeP, globeP->contactCount);
							if (globeP->contactCount > 0)
							{
								LstSetSelection(lstContactsP, 0);
							}
							else
							{
								LstSetSelection(lstContactsP, noListSelection);
							}
						}
						
						//设置文本框指针
						setHandleToFieldTextFirstName(fldPYKeywordP, globeP);
						
						//重绘窗口
						FrmUpdateForm(frmFirstNameSearch, frmRedrawUpdateCode);
						
						//标志按键已处理
						isKeyHandled = true;
					}
					else
					{
						if (EvtKeydownIsVirtual(&event))
						{
							if (event.data.keyDown.keyCode > 0)
							{
								key = event.data.keyDown.keyCode;
							}
							else
							{
								key = event.data.keyDown.chr;
							}
						}
						else
						{
							key = event.data.keyDown.chr;
						}
						switch (key)
						{
							case chrUpArrow:
							case keyPageUp:
							case keyRockerUp: //上键，移动联系人列表选择项
							{
								if (globeP->contactCount > 0)
								{
									//移动列表选择项
									performListSelectionMovement(lstContactsP, MOVE_UP, globeP);
								}
								
								//标志按键已处理
								isKeyHandled = true;
								break;
							}
							case chrDownArrow:
							case keyPageDown:
							case keyRockerDown: //下键，移动联系人列表选择项
							{
								if (globeP->contactCount > 0)
								{
									//移动列表选择项
									performListSelectionMovement(lstContactsP, MOVE_DOWN, globeP);
								}
								
								//标志按键已处理
								isKeyHandled = true;
								break;
							}
							case chrLeftArrow:
							case keyRockerLeft:
							{
								exit = true;
								
								break;
							}
							case chrRightArrow:
							case keyRockerRight:
							{
								exit = true;
								
								break;
							}
							case vchrRockerCenter: //中键，获取联系人信息，或者获取选定的号码
							{
								if (! (event.data.keyDown.modifiers & willSendUpKeyMask))
								{
									if (getActionParameter(globeP, &resultP) == errNone)
									{
										exit = true;
									}
									
									isKeyHandled = true;
								}
								
								break;
							}
							case keyBackspace: //删除
							{
								if (globeP->pyKeywordLength > 0)
								{
									//获取文本框指针
									getPointerOfFieldTextHandleFirstName(fldPYKeywordP, globeP);
									
									//置入拼音关键字缓存
									if (globeP->pyKeywordLength > 0)
									{
										globeP->pyKeywordLength --;
										globeP->pyKeywordP[globeP->pyKeywordLength] = '\0';
									}
									
									if (globeP->pyKeywordLength > 0)
									{
										//重新检索通讯录
										constructPYKeyword(CNLBparamP, globeP->pyKeywordP);
										(*getContactListP)(CNLBglobeP, CNLBparamP, globeP, SEARCH_MODE_FIRSTNAME);
										
										//重置通讯记录列表
										LstSetDrawFunction(lstContactsP, drawContactListByContactP);
										LstSetListChoices(lstContactsP, (Char **)&globeP, globeP->contactCount);
										if (globeP->contactCount > 0)
										{
											LstSetSelection(lstContactsP, 0);
										}
										else
										{
											LstSetSelection(lstContactsP, noListSelection);
										}
									}
									else
									{
										exit = true;
									}
									
									//设置文本框指针
									setHandleToFieldTextFirstName(fldPYKeywordP, globeP);
									
									if (! exit)
									{
										//重绘窗口
										FrmUpdateForm(frmFirstNameSearch, frmRedrawUpdateCode);
									}
								}
								else
								{
									exit = true;
								}
								
								//标志按键已处理
								isKeyHandled = true;
								break;
							}
							case keyLeftAlt: //Option键，上翻页
							{
								break;
							}
							case keyLeftShift: //左Shift，下翻页
							{
								break;
							}
							default:
							{
								//
								break;
							}
						}
					}
					
					//清除按键事件
					if (isKeyHandled)
					{
						MemSet(&event, sizeof(EventType), 0x00);
						event.eType = nilEvent;
						isKeyHandled = false;
					}
					else
					{
						if (! SysHandleEvent(&event))
						{
							if (! MenuHandleEvent(0, &event, &error))
							{
								FrmHandleEvent(frmContactP, &event);
							}
						}
					}
				}
				else if (! SysHandleEvent(&event))
				{
					if (! MenuHandleEvent(0, &event, &error))
					{
						switch (event.eType)
						{
							case frmLoadEvent: //装载窗体
							{
								frmContactP = FrmInitForm(frmFirstNameSearch);
								FrmSetActiveForm(frmContactP);
								break;
							}
							case frmOpenEvent: //显示窗体
							{
								//获取文本框指针
								fldPYKeywordP = (FieldType *)getObjectPtr(frmContactP, fldPYKey);
								
								//初始化文本框
								globeP->pyKeywordH = MemHandleNew(11);
								globeP->pyKeywordP = (Char *)MemHandleLock(globeP->pyKeywordH);
								MemSet(globeP->pyKeywordP, 11, 0x00);
								
								//获取列表指针
								lstContactsP = (ListType *)getObjectPtr(frmContactP, lstContacts);
								globeP->lstContactsP = lstContactsP;
								
								if (initKey)
								{ //带有初始键
									isTheKeyValid(initKey, globeP, &numericKey);
									
									//置入拼音关键字缓存
									globeP->pyKeywordP[globeP->pyKeywordLength] = (Char)initKey;
									globeP->pyKeywordLength ++;
								}
									
								//检索通讯录
								constructPYKeyword(CNLBparamP, globeP->pyKeywordP);
								(*getContactListP)(CNLBglobeP, CNLBparamP, globeP, SEARCH_MODE_FIRSTNAME);
								
								//通讯记录列表
								LstSetDrawFunction(lstContactsP, drawContactListByContactP);
								LstSetListChoices(lstContactsP, (Char **)&globeP, globeP->contactCount);
								if (globeP->contactCount > 0)
								{
									LstSetSelection(lstContactsP, 0);
								}
								else
								{
									LstSetSelection(lstContactsP, noListSelection);
								}
								
								//设置文本框指针
								setHandleToFieldTextFirstName(fldPYKeywordP, globeP);
								
								//画窗体
								FrmDrawForm(frmContactP);
								
								break;
							}
							default:
							{
								if (frmContactP)
								{
									FrmHandleEvent(frmContactP, &event);
								}
								break;
							}
						}
					}
				}
			}while(event.eType != appStopEvent && exit == false);
			
			//关闭数据库
			DmCloseDatabase(CNLBglobeP->addressDbRef);
			
			//释放内存
			FldSetTextHandle(fldPYKeywordP, NULL);
			MemPtrFree(CNLBparamP);
			MemPtrFree(CNLBglobeP);
			freeContactList(globeP);
			if (globeP->contactListH && globeP->totalContacts > 0)
			{
				MemHandleFree(globeP->contactListH);
			}
			MemHandleFree(globeP->pyKeywordH);
			FrmReturnToForm(0);
		}
		
		//关闭检索库
		CLibRemove(globeP->refNum, ctx);
	}
	else
	{
		FrmAlert(alertLoadPYLibraryError);
	}
	
	MemPtrFree(globeP);
	
	return resultP;
}*/

static void EnqueueResultToEvent(Char *resultP)
{
	WChar		chr;
	UInt16		i = 0;
	UInt16		resultLength = StrLen(resultP);
	
	while (i < resultLength && i < 3)
	{
		i += TxtGetNextChar(resultP, i, &chr);
		EvtEnqueueKey(chr, chr, 0);
	}
}

UInt32 PilotMain(UInt16 cmd, MemPtr cmdPBP, UInt16 launchFlags)
{
	if (cmd == LAUNCH_CODE_ACTION)
	{
		ActionParameterType		*actionParamP	= (ActionParameterType *)cmdPBP;
		
		switch (actionParamP->settingP->searchMode)
		{
			case LASTNAME_MODE:
			{
				WChar	chr = lastNameSearchEventHandler(actionParamP->settingP, actionParamP->settingP->actionKey);
				
				if (chr > 0)
				{
					EvtEnqueueKey(keyBackspace, keyBackspace, 0);
					EvtEnqueueKey(keyBackspace, keyBackspace, 0);
					EvtEnqueueKey(chr, chr, 0);
				}
				
				break;
			}
			case FIRSTNAME_MODE:
			{
				/*Char	*resultP = firstNameSearchEventHandler(settingP, settingP->actionKey);
				
				if (resultP)
				{
					EvtEnqueueKey(keyBackspace, keyBackspace, 0);
					EvtEnqueueKey(keyBackspace, keyBackspace, 0);
					EvtEnqueueKey(keyBackspace, keyBackspace, 0);
					EvtEnqueueKey(keyBackspace, keyBackspace, 0);
					EnqueueResultToEvent(resultP);
				}*/
				
				break;
			}
		}
	}

	return errNone;
}