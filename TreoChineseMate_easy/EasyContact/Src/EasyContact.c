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
//�ͷ�ͨѶ��¼�ڵ���ڴ�
//****************************************************************************
static void freeContactList(GlobeType *globeP)
{
	UInt16	i;
	
	if (globeP->contactListH && globeP->totalContacts > 0)
	{
		globeP->contactList = (void **)MemHandleLock(globeP->contactListH);
		
		//ѭ���ͷŽڵ��ڴ�
		for (i = 0; i < globeP->contactCount; i ++)
		{
			MemPtrFree(globeP->contactList[i]);
		}
		
		MemHandleUnlock(globeP->contactListH);
	}
	
	//���ýڵ������
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
	
	if (((UInt8)(*cache)) > 127)	//����
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
	
	if (((UInt8)(*cache)) > 127)	//����
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
//��ȡ���Ϲؼ���Ҫ�����ϵ���б�
//****************************************************************************
static void getContactList_Addr(CNLBGlobeType *CNLBglobeP, CNLBParameterType *CNLBparamP, GlobeType *globeP, UInt8 mode)
{
	OLD_AddrDBRecordType	*unit;
	
	//���ͨѶ��¼�ڵ�
	freeContactList(globeP);
	
	if (globeP->contactListH && globeP->totalContacts > 0)
	{
		globeP->contactList = (void **)MemHandleLock(globeP->contactListH);
		
		//������ʱ��Ԫ
		unit = (OLD_AddrDBRecordType *)MemPtrNew(sizeof(OLD_AddrDBRecordType));
		
		//����ͨѶ��¼
		CNLBparamP->recordId = 0;
		while (CNLBparamP->recordId < globeP->totalContacts)
		{
			//��鵱ǰ��¼�Ƿ����ƴ��
			if (CLibGetMatchContactByRecordId_Addr(globeP->refNum, CNLBglobeP, CNLBparamP, unit, mode) == errNone)
			{ //�ҵ�һ��ƥ��ļ�¼
				//���䵥Ԫ�ڴ�
				globeP->contactList[globeP->contactCount] = MemPtrNew(sizeof(OLD_AddrDBRecordType));
				
				//��ȡ��¼
				MemMove(globeP->contactList[globeP->contactCount], unit, sizeof(OLD_AddrDBRecordType));
				globeP->contactCount ++;
			}
			
			refreshCNLBGlobeParameter(CNLBglobeP);
			CNLBparamP->recordId ++;
		}
		
		//�ͷźͽ����ڴ�
		MemPtrFree(unit);
		MemHandleUnlock(globeP->contactListH);
	}
}
static void getContactList_PAdd(CNLBGlobeType *CNLBglobeP, CNLBParameterType *CNLBparamP, GlobeType *globeP, UInt8 mode)
{
	AddrDBRecordType		*unit;
	
	//���ͨѶ��¼�ڵ�
	freeContactList(globeP);
	
	if (globeP->contactListH && globeP->totalContacts > 0)
	{
		globeP->contactList = (void **)MemHandleLock(globeP->contactListH);
		
		//������ʱ��Ԫ
		unit = (AddrDBRecordType *)MemPtrNew(sizeof(AddrDBRecordType));
		
		//����ͨѶ��¼
		CNLBparamP->recordId = 0;
		while (CNLBparamP->recordId < globeP->totalContacts)
		{
			//��鵱ǰ��¼�Ƿ����Ҫ��
			if (CLibGetMatchContactByRecordId_PAdd(globeP->refNum, CNLBglobeP, CNLBparamP, unit, mode) == errNone)
			{ //�ҵ�һ��ƥ��ļ�¼
				//���䵥Ԫ�ڴ�
				globeP->contactList[globeP->contactCount] = MemPtrNew(sizeof(AddrDBRecordType));
				
				//��ȡ��¼
				MemMove(globeP->contactList[globeP->contactCount], unit, sizeof(AddrDBRecordType));
				
				globeP->contactCount ++;
			}
			refreshCNLBGlobeParameter(CNLBglobeP);
			CNLBparamP->recordId ++;
		}
		
		//�ͷźͽ����ڴ�
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
//�����������Ƿ��Ѿ��������ı�����
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
//�Ѽ������������ϰ󶨵��ı���
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
			//��������
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
			//��������
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
//���ؼ��ֺͷָ���
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
//���ø������ı���
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
		
		//����ָ���ƶ�����
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
		
		//�����������н�����ı���
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
//��ȡ�ı�������ָ��
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
//���ı�������ָ��
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
//�ټ��ռ���ģʽ
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
	
	//��ʼ��globe
	globeP = (GlobeType *)MemPtrNew(sizeof(GlobeType));
	MemSet(globeP, sizeof(GlobeType), 0x00);
	globeP->settingP = settingP;
	globeP->pyKeywordP = (Char *)MemPtrNew(11);
	MemSet(globeP->pyKeywordP, 11, 0);
	globeP->pyKeywordLength = 1;
	
	//�򿪼�����
	if (CLibLoad(&globeP->refNum, &ctx) == errNone)
	{
		//����ͨѶ¼���ݿ�����ͣ�ȷ��Ӧ���õĺ���
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
		
		//Ԥ��ʼ��ȫ�ֱ�������ͨѶ¼���ݿ�
		if (! error)
		{
			//Ԥ�������
			CNLBparamP = (CNLBParameterType *)MemPtrNew(sizeof(CNLBParameterType));
			MemSet(CNLBparamP, sizeof(CNLBParameterType), 0x00);
			CNLBparamP->orderlySearch = true;
			
			//��ȡͨѶ��¼����
			globeP->totalContacts = DmNumRecordsInCategory(CNLBglobeP->addressDbRef, dmAllCategories);
			
			//����ڵ�ָ��
			globeP->contactListH = MemHandleNew((globeP->totalContacts << 2));
			
			//�򿪴���
			FrmPopupForm(frmLastNameSearch);
			
			do
			{
				EvtGetEvent(&event, evtWaitForever);
				
				if (event.eType == keyDownEvent && frmContactP)
				{
					//��ȡ�ı���ָ�뼯
					getPointerOfFieldTextHandle(globeP);
					
					if (event.data.keyDown.chr >= keyA && event.data.keyDown.chr <= keyZ)
					{
						globeP->pyKeywordP[0] = (Char)event.data.keyDown.chr;
						
						//����ͨѶ¼
						constructPYKeyword(CNLBparamP, globeP->pyKeywordP);
						(*getContactListP)(CNLBglobeP, CNLBparamP, globeP, SEARCH_MODE_LASTNAME);
						
						//�󶨵��ı���
						bindLastNameToFieldP(globeP);
						
						//���ؼ���
						drawKeyword(globeP);
						
						//���ø������ı���
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
							case keyRockerUp: //�ϼ����ƶ���ϵ���б�ѡ����
							{
								setHighLightField(globeP, MOVE_UP);
								
								//��־�����Ѵ���
								isKeyHandled = true;
								break;
							}
							case chrDownArrow:
							case keyPageDown:
							case keyRockerDown: //�¼����ƶ���ϵ���б�ѡ����
							{
								setHighLightField(globeP, MOVE_DOWN);
								
								//��־�����Ѵ���
								isKeyHandled = true;
								break;
							}
							case chrLeftArrow:
							case vchrRockerLeft: //��������ȫ���绰�����ƴ���ؼ��֣�������ʾ�����ϵ���б�
							{
								setHighLightField(globeP, MOVE_LEFT);
								
								//��־�����Ѵ���
								isKeyHandled = true;
								break;
							}
							case chrRightArrow:
							case vchrRockerRight:
							{
								setHighLightField(globeP, MOVE_RIGHT);
								
								//��־�����Ѵ���
								isKeyHandled = true;
								break;
							}
							case vchrRockerCenter: //�м�����ȡ��ϵ����Ϣ�����߻�ȡѡ���ĺ���
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
								
								//��־�����Ѵ���
								isKeyHandled = true;
								break;
							}
							case keyBackspace: //ɾ��
							{
								exitCode = true;
								
								//��־�����Ѵ���
								isKeyHandled = true;
								break;
							}
							case keySpace: //�ո������ȡѡ��������
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
								
								//��־�����Ѵ���
								isKeyHandled = true;
								break;
							}
						}
					}
					
					//���ı���ָ�뼯
					setHandleToFieldText(globeP);
					
					//��������¼�
					if (isKeyHandled)
					{
						MemSet(&event, sizeof(EventType), 0x00);
						event.eType = nilEvent;
						isKeyHandled = false;
						
						if (exitCode == false)
						{
							//�ػ洰��
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
							case frmLoadEvent: //װ�ش���
							{
								frmContactP = FrmInitForm(frmLastNameSearch);
								FrmSetActiveForm(frmContactP);
								break;
							}
							case frmOpenEvent: //��ʾ����
							{
								//��ȡ�ı���ָ�롢�����ڴ�
								for (i = r0; i <= r44; i ++)
								{
									globeP->fldRP[i - r0] = (FieldType *)getObjectPtr(frmContactP, i);
									globeP->fldRTextH[i - r0] = MemHandleNew(3);
									globeP->fldRTextP[i - r0] = MemHandleLock(globeP->fldRTextH[i - r0]);
									MemSet(globeP->fldRTextP[i - r0], 3, 0x00);
								}
								
								//����ƴ���ؼ��ֻ���
								globeP->pyKeywordP[0] = (Char)initKey;
								
								//����ͨѶ¼
								constructPYKeyword(CNLBparamP, globeP->pyKeywordP);
								(*getContactListP)(CNLBglobeP, CNLBparamP, globeP, SEARCH_MODE_LASTNAME);
								
								//�󶨵��ı���
								bindLastNameToFieldP(globeP);
								
								//�����ı���ָ��
								setHandleToFieldText(globeP);
								
								//������
								FrmDrawForm(frmContactP);
								
								//���ؼ��ֺͷָ���
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
			
			//�ر����ݿ�
			DmCloseDatabase(CNLBglobeP->addressDbRef);
			
			//�ͷ��ڴ�
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
		
		//�رռ�����
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
//��鰴���Ƿ�������
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
//���췵�ز���
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
//����ϵ���б���ʾ���ϼ����������ϵ�ˣ�
//****************************************************************************
static void drawContactListByContact_Addr(Int16 itemNum, RectangleType *bounds, Char **itemsText)
{
	GlobeType			*globeP;
	
	//�ػ�ȫ��ָ��
	globeP = *(GlobeType **)itemsText;
	
	globeP->contactList = (void **)MemHandleLock(globeP->contactListH);
	
	buildFullName_Addr(globeP->cache, &((ContactUnitType_PAdd **)globeP->contactList)[itemNum]->data);
	
	//��ʾ����
	WinDrawTruncChars(globeP->cache, StrLen(globeP->cache), bounds->topLeft.x, bounds->topLeft.y, bounds->extent.x);

	MemHandleUnlock(globeP->contactListH);
}
static void drawContactListByContact_PAdd(Int16 itemNum, RectangleType *bounds, Char **itemsText)
{
	GlobeType			*globeP;
	
	//�ػ�ȫ��ָ��
	globeP = *(GlobeType **)itemsText;
	
	globeP->contactList = (void **)MemHandleLock(globeP->contactListH);
	
	buildFullName_PAdd(globeP->cache, &((ContactUnitType_PAdd **)globeP->contactList)[itemNum]->data);
	
	//��ʾ����
	WinDrawTruncChars(globeP->cache, StrLen(globeP->cache), bounds->topLeft.x, bounds->topLeft.y, bounds->extent.x);

	MemHandleUnlock(globeP->contactListH);
}

//****************************************************************************
//��ȡ�ı�������ָ��
//****************************************************************************
static void getPointerOfFieldTextHandleFirstName(FieldType *fldPYKeywordP, GlobeType *globeP)
{
	FldSetTextHandle(fldPYKeywordP, NULL);
	globeP->pyKeywordP = (Char *)MemHandleLock(globeP->pyKeywordH);
}

//****************************************************************************
//���ı�������ָ��
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
//���ּ���ģʽ
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
	
	//��ʼ��globe
	globeP = (GlobeType *)MemPtrNew(sizeof(GlobeType));
	MemSet(globeP, sizeof(GlobeType), 0x00);
	globeP->settingP = settingP;
	
	//�򿪼�����
	if (CLibLoad(&globeP->refNum, &ctx) == errNone)
	{
		//����ͨѶ¼���ݿ�����ͣ�ȷ��Ӧ���õĺ���
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
		
		//Ԥ��ʼ��ȫ�ֱ�������ͨѶ¼���ݿ�
		if (! error)
		{
			//Ԥ�������
			CNLBparamP = (CNLBParameterType *)MemPtrNew(sizeof(CNLBParameterType));
			MemSet(CNLBparamP, sizeof(CNLBParameterType), 0x00);
			CNLBparamP->orderlySearch = globeP->settingP->orderlySearch;
			
			//��ȡͨѶ��¼����
			globeP->totalContacts = DmNumRecordsInCategory(CNLBglobeP->addressDbRef, dmAllCategories);
			
			//����ڵ�ָ��
			globeP->contactListH = MemHandleNew((globeP->totalContacts << 2));
			
			//�򿪴���
			FrmPopupForm(frmFirstNameSearch);
			
			do
			{
				EvtGetEvent(&event, evtWaitForever);
				
				if (event.eType == keyDownEvent && frmContactP)
				{						
					//������
					if (isTheKeyValid(event.data.keyDown.chr, globeP, &numericKey))
					{ //��A-Z
						//��ȡ�ı���ָ��
						getPointerOfFieldTextHandleFirstName(fldPYKeywordP, globeP);
						
						//����ƴ���ؼ��ֻ���
						if (globeP->pyKeywordLength < 10)
						{
							globeP->pyKeywordP[globeP->pyKeywordLength] = (Char)event.data.keyDown.chr;
							globeP->pyKeywordLength ++;
							
							//����ͨѶ¼
							constructPYKeyword(CNLBparamP, globeP->pyKeywordP);
							(*getContactListP)(CNLBglobeP, CNLBparamP, globeP, SEARCH_MODE_FIRSTNAME);
							
							//����ͨѶ��¼�б�
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
						
						//�����ı���ָ��
						setHandleToFieldTextFirstName(fldPYKeywordP, globeP);
						
						//�ػ洰��
						FrmUpdateForm(frmFirstNameSearch, frmRedrawUpdateCode);
						
						//��־�����Ѵ���
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
							case keyRockerUp: //�ϼ����ƶ���ϵ���б�ѡ����
							{
								if (globeP->contactCount > 0)
								{
									//�ƶ��б�ѡ����
									performListSelectionMovement(lstContactsP, MOVE_UP, globeP);
								}
								
								//��־�����Ѵ���
								isKeyHandled = true;
								break;
							}
							case chrDownArrow:
							case keyPageDown:
							case keyRockerDown: //�¼����ƶ���ϵ���б�ѡ����
							{
								if (globeP->contactCount > 0)
								{
									//�ƶ��б�ѡ����
									performListSelectionMovement(lstContactsP, MOVE_DOWN, globeP);
								}
								
								//��־�����Ѵ���
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
							case vchrRockerCenter: //�м�����ȡ��ϵ����Ϣ�����߻�ȡѡ���ĺ���
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
							case keyBackspace: //ɾ��
							{
								if (globeP->pyKeywordLength > 0)
								{
									//��ȡ�ı���ָ��
									getPointerOfFieldTextHandleFirstName(fldPYKeywordP, globeP);
									
									//����ƴ���ؼ��ֻ���
									if (globeP->pyKeywordLength > 0)
									{
										globeP->pyKeywordLength --;
										globeP->pyKeywordP[globeP->pyKeywordLength] = '\0';
									}
									
									if (globeP->pyKeywordLength > 0)
									{
										//���¼���ͨѶ¼
										constructPYKeyword(CNLBparamP, globeP->pyKeywordP);
										(*getContactListP)(CNLBglobeP, CNLBparamP, globeP, SEARCH_MODE_FIRSTNAME);
										
										//����ͨѶ��¼�б�
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
									
									//�����ı���ָ��
									setHandleToFieldTextFirstName(fldPYKeywordP, globeP);
									
									if (! exit)
									{
										//�ػ洰��
										FrmUpdateForm(frmFirstNameSearch, frmRedrawUpdateCode);
									}
								}
								else
								{
									exit = true;
								}
								
								//��־�����Ѵ���
								isKeyHandled = true;
								break;
							}
							case keyLeftAlt: //Option�����Ϸ�ҳ
							{
								break;
							}
							case keyLeftShift: //��Shift���·�ҳ
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
					
					//��������¼�
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
							case frmLoadEvent: //װ�ش���
							{
								frmContactP = FrmInitForm(frmFirstNameSearch);
								FrmSetActiveForm(frmContactP);
								break;
							}
							case frmOpenEvent: //��ʾ����
							{
								//��ȡ�ı���ָ��
								fldPYKeywordP = (FieldType *)getObjectPtr(frmContactP, fldPYKey);
								
								//��ʼ���ı���
								globeP->pyKeywordH = MemHandleNew(11);
								globeP->pyKeywordP = (Char *)MemHandleLock(globeP->pyKeywordH);
								MemSet(globeP->pyKeywordP, 11, 0x00);
								
								//��ȡ�б�ָ��
								lstContactsP = (ListType *)getObjectPtr(frmContactP, lstContacts);
								globeP->lstContactsP = lstContactsP;
								
								if (initKey)
								{ //���г�ʼ��
									isTheKeyValid(initKey, globeP, &numericKey);
									
									//����ƴ���ؼ��ֻ���
									globeP->pyKeywordP[globeP->pyKeywordLength] = (Char)initKey;
									globeP->pyKeywordLength ++;
								}
									
								//����ͨѶ¼
								constructPYKeyword(CNLBparamP, globeP->pyKeywordP);
								(*getContactListP)(CNLBglobeP, CNLBparamP, globeP, SEARCH_MODE_FIRSTNAME);
								
								//ͨѶ��¼�б�
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
								
								//�����ı���ָ��
								setHandleToFieldTextFirstName(fldPYKeywordP, globeP);
								
								//������
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
			
			//�ر����ݿ�
			DmCloseDatabase(CNLBglobeP->addressDbRef);
			
			//�ͷ��ڴ�
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
		
		//�رռ�����
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