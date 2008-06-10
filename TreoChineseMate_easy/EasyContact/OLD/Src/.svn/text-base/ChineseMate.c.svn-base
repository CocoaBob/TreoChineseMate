/*
 * ChineseMate.c
 *
 * main file for ChineseMate
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
#include <common\system\palmOneNavigator.h>
#include <common\system\HsKeyCodes.h>

#include "ChineseMate.h"
#include "ChineseMate_Rsc.h"
#include "AddrDefines.h"
#include "AddressDB.h"
#include "OLD_AddrDefines.h"
#include "OLD_AddressDB.h"

/*********************************************************************
 * Internal Functions
 *********************************************************************/

const Char			*trigText_FirstName = "名(First Name)";
const Char			*trigText_LastName = "姓(Last Name)";
const Char			*trigText_Company = "公司(Company)";

static WChar GetFirstChar(DmOpenRef * dbRef, UInt16 recI)
{
	MemHandle			recH = 0;
	AddrDBRecordType	addrRec;
	WChar				firstChar = 0;
	
	AddrDBGetRecord(dbRef, recI, &addrRec, &recH);
	if (recH)
	{
		if (addrRec.fields[name])
		{
			if ((UInt8)addrRec.fields[name][0] > 128)
			{
				firstChar = ((((UInt16) addrRec.fields[name][0]) << 8) & 0xFF00) | (((UInt16) addrRec.fields[name][1]) & 0x00FF);
			}
			else
			{
				firstChar = ((UInt16) addrRec.fields[name][0]) & 0x00FF;
			}
		}
		else if (addrRec.fields[firstName])
		{
			if ((UInt8)addrRec.fields[firstName][0] > 128)
			{
				firstChar = ((((UInt16) addrRec.fields[firstName][0]) << 8) & 0xFF00) | (((UInt16) addrRec.fields[firstName][1]) & 0x00FF);
			}
			else
			{
				firstChar = ((UInt16) addrRec.fields[firstName][0]) & 0x00FF;
			}
		}
		else if (addrRec.fields[company])
		{
			if ((UInt8)addrRec.fields[company][0] > 128)
			{
				firstChar = ((((UInt16) addrRec.fields[company][0]) << 8) & 0xFF00) | (((UInt16) addrRec.fields[company][1]) & 0x00FF);
			}
			else
			{
				firstChar = ((UInt16) addrRec.fields[company][0]) & 0x00FF;
			}
		}
		else
		{
			firstChar = 0x0055; //'U'
		}
		MemHandleUnlock(recH);
	}
	
	return firstChar;
}

static WChar OLD_GetFirstChar(DmOpenRef * dbRef, UInt16 recI)
{
	MemHandle				recH = 0;
	OLD_AddrDBRecordType	addrRec;
	WChar					firstChar = 0;
	
	OLD_AddrDBGetRecord(dbRef, recI, &addrRec, &recH);
	if (recH)
	{
		if (addrRec.fields[OLD_name])
		{
			if ((UInt8)addrRec.fields[OLD_name][0] > 128)
			{
				firstChar = ((((UInt16) addrRec.fields[OLD_name][0]) << 8) & 0xFF00) | (((UInt16) addrRec.fields[OLD_name][1]) & 0x00FF);
			}
			else
			{
				firstChar = ((UInt16) addrRec.fields[OLD_name][0]) & 0x00FF;
			}
		}
		else if (addrRec.fields[OLD_firstName])
		{
			if ((UInt8)addrRec.fields[OLD_firstName][0] > 128)
			{
				firstChar = ((((UInt16) addrRec.fields[OLD_firstName][0]) << 8) & 0xFF00) | (((UInt16) addrRec.fields[OLD_firstName][1]) & 0x00FF);
			}
			else
			{
				firstChar = ((UInt16) addrRec.fields[OLD_firstName][0]) & 0x00FF;
			}
		}
		else if (addrRec.fields[company])
		{
			if ((UInt8)addrRec.fields[company][0] > 128)
			{
				firstChar = ((((UInt16) addrRec.fields[company][0]) << 8) & 0xFF00) | (((UInt16) addrRec.fields[company][1]) & 0x00FF);
			}
			else
			{
				firstChar = ((UInt16) addrRec.fields[company][0]) & 0x00FF;
			}
		}
		else
		{
			firstChar = 0x0055; //'U'
		}
		MemHandleUnlock(recH);
	}
	
	return firstChar;
}

static void GetFirstName(DmOpenRef * dbRef, UInt16 rcIndex, Char * addrFullName, ChineseMatePref *cf)
{
	MemHandle			rcHandle;
	AddrDBRecordType	addrRecord;
	const Char			* unName = "unamed\0";
	
	AddrDBGetRecord(dbRef, rcIndex, &addrRecord, &rcHandle);
	addrFullName[0] = '\0';
	addrFullName[99] = '\0';
	switch (cf->DALaunch)
	{
		case FastSearch_FirstName:
		{
			if (addrRecord.fields[firstName])
			{
				StrCopy(addrFullName, addrRecord.fields[firstName]);
			}
			break;
		}
		case FastSearch_LastName:
		{
			if (addrRecord.fields[name])
			{
				StrCopy(addrFullName, addrRecord.fields[name]);
			}
			break;
		}
	}
	if (StrLen(addrFullName) == 0)
	{
		if (addrRecord.fields[company])
		{
			StrCat(addrFullName, addrRecord.fields[company]);
		}
		else
		{
			StrCat(addrFullName, unName);
		}
	}
	if (rcHandle)
	{
		MemHandleUnlock(rcHandle);
	}
}

static void OLD_GetFirstName(DmOpenRef * dbRef, UInt16 rcIndex, Char * addrFullName, ChineseMatePref *cf)
{
	MemHandle				rcHandle;
	OLD_AddrDBRecordType	addrRecord;
	const Char				* unName = "unamed\0";
	
	OLD_AddrDBGetRecord(dbRef, rcIndex, &addrRecord, &rcHandle);
	addrFullName[0] = '\0';
	addrFullName[99] = '\0';
	switch (cf->DALaunch)
	{
		case FastSearch_FirstName:
		{
			if (addrRecord.fields[OLD_firstName])
			{
				StrCopy(addrFullName, addrRecord.fields[OLD_firstName]);
			}
			break;
		}
		case FastSearch_LastName:
		{
			if (addrRecord.fields[OLD_name])
			{
				StrCopy(addrFullName, addrRecord.fields[OLD_name]);
			}
			break;
		}
	}
	if (StrLen(addrFullName) == 0)
	{
		if (addrRecord.fields[company])
		{
			StrCat(addrFullName, addrRecord.fields[company]);
		}
		else
		{
			StrCat(addrFullName, unName);
		}
	}
	if (rcHandle)
	{
		MemHandleUnlock(rcHandle);
	}
}

/*static void GBK2PY(Char *gbkStr, Char *pyStr, Char *keyStr, DmOpenRef *dbR)
{
	UInt16		gbkSize;
	UInt16		gbkOffset;
	UInt16		pyOffset;
	WChar		curChar;
	
	gbkSize = StrLen(gbkStr);
	gbkOffset = 0;
	pyOffset = 0;
	keyStr = keyStr;
	while (gbkOffset < gbkSize)
	{
		gbkOffset += TxtGetNextChar(gbkStr, gbkOffset, &curChar);
		if (curChar > 128)
		{
			pyStr[pyOffset] = GetFirstPY(curChar, curChar, dbR);
		}
		else
		{
			if (curChar > 96 && curChar < 123)
			{
				pyStr[pyOffset] = curChar - 32;
			}
			else
			{
				pyStr[pyOffset] = curChar;
			}
		}
		pyOffset ++;
	}
	pyStr[pyOffset] = '\0';
}*/

static UInt16 isMultiPY(WChar gbkchr, Char *pychr, UInt32 *base, DmOpenRef *pydbRef)
{
	MemHandle	recordHandle;
	MemPtr		recordPtr;
	UInt16		index;

	if (gbkchr <= 128) //英文
	{
		(*pychr) = (Char)(0x00FF & gbkchr);
		return 0xFFFF;
	}
	recordHandle = DmQueryRecord(pydbRef, 0);
	recordPtr = MemHandleLock(recordHandle);
	index = ((UInt16 *)recordPtr)[gbkchr - 33088];
	if (index >= 0xFF00)
	{
		(*pychr) = (Char)(0x00FF & index);
		MemHandleUnlock(recordHandle);
		return 0xFFFF;
	}
	else //多音字
	{
		MemHandleUnlock(recordHandle);
		recordHandle = DmQueryRecord(pydbRef, 1);
		recordPtr = MemHandleLock(recordHandle);
		(*pychr) = ((Char *)recordPtr)[index];
		(*base) = (*base) * ((UInt32)StrLen((Char *)recordPtr + index));
		MemHandleUnlock(recordHandle);
		return index;
	}
}

static Boolean CharMatched(UInt16 CHChar, Char curKey, DmOpenRef *pydbRef)
{
	Char		firstPY;
	UInt16		index;
	MemHandle	recH;
	Char		*recP;
	UInt32		base = 1;

	index = isMultiPY(CHChar, &firstPY, &base, pydbRef);
	if (index == 0xFFFF)
	{
		if (firstPY == curKey)
		{
			return true;
		}
	}
	else
	{
		recH = DmQueryRecord(pydbRef, 1);
		recP = ((Char *)MemHandleLock(recH)) + index;
		while (((UInt8)(*recP)) != 0x00)
		{
			if ((*recP) == curKey)
			{
				MemHandleUnlock(recH);
				return true;
			}
			recP ++;
		}
		MemHandleUnlock(recH);
	}
	
	return false;
}

static Boolean MatchPY(Char *gbkStr, Char *pyStr, Char *keyStr, UInt16 *sampleStr, DmOpenRef *dbR)
{
	WChar		chrBuf;
	Char		**mStr;
	MemHandle	recH;
	Char		*recP;
	UInt16		multiPyCount;
	UInt16		gbklen = 0;
	UInt32		strCount = 1;
	UInt16		groupRepeatTime;
	UInt16		groupCount;
	UInt16		i = 0;
	UInt16		j = 0;
	UInt16		k = 0;
	UInt16		l = 0;
	UInt16		m = 0;
	UInt16		n = 0;
	Boolean		matched = false;
	
	while (gbkStr[i])
	{
		i += TxtGetNextChar(gbkStr, i, &chrBuf);
		sampleStr[(gbklen << 1)] = isMultiPY(chrBuf, &pyStr[gbklen], &strCount, dbR);
		gbklen ++;
	}
	if (strCount == 1) //没有多音字
	{
		recP = StrStr(pyStr, keyStr);
		if (recP)
		{
			matched = true;
		}
	}
	else
	{
		//分配内存
		mStr = MemPtrNew((strCount << 2));
		j = gbklen + 1;
		for (i = 0; i < strCount; i ++)
		{
			mStr[i] = MemPtrNew(j);
			MemSet(mStr[i], j, 0x00);
		}
		groupRepeatTime = strCount;
		//构造字符串
		for (i = 0; i < gbklen; i ++) //遍历每个字母
		{
			m = i << 1;
			if (sampleStr[m] != 0xFFFF) //多音字
			{
				recH = DmQueryRecord(dbR, 1);
				recP = ((Char *)MemHandleLock(recH)) + sampleStr[m]; //+偏移量，指向当前汉字的拼音首字母集。
				multiPyCount = StrLen(recP); //获取长度，即当前汉字的拼音首字母的个数
				groupRepeatTime = groupRepeatTime / multiPyCount; //当前字母在每一组中重复的次数
				groupCount = strCount / (groupRepeatTime * multiPyCount); //段数
				j = 0; //结果字符串数组索引
				for (k = 0; k < groupCount; k ++) //遍历每个组
				{
					for (l = 0; l < multiPyCount; l ++) //遍历当前汉字的每个拼音首字母
					{
						for (n = 0; n < groupRepeatTime; n ++) //把当前字母重复足够的次数
						{
							mStr[j][i] = recP[l];
							j ++;
						}
					}
				}
				MemHandleUnlock(recH);
			}
			else
			{
				for (j = 0; j < strCount; j ++)
				{
					mStr[j][i] = pyStr[i];
				}
			}
		}
		//比较的同时释放内存。
		for (i = 0; i < strCount; i ++)
		{
			recP = StrStr(mStr[i], keyStr);
			if (recP)
			{
				matched = true;
			}
			MemPtrFree(mStr[i]);
		}
		MemPtrFree(mStr);
	}
	return matched;
}

static void FirstNameHandler(WChar *Buf, ChineseMatePref *cf)
{
	EventType	event;
	FormType	*frmP;
	ListType	*lstP;
	Char		*fldKeyTextPtr;
	Boolean		ExitEventLoop;
	UInt16		i;
	DmOpenRef	*dbRef;
	DmOpenRef	*dbR;
	UInt32		dbCount;
	Char		*addrFullName;
	UInt16		*sStr;
	UInt16		seachOffset;
	UInt16		keyLength;
	Int16		lstSel;
	UInt16		matchC = 0;
	Char		*pyStr;
	//Char		*matched = 0;
	Char		**lstItemStr;
	Char		*bLstBuf;
	Boolean		keyHandled;
	UInt32		addrCreator;
	void		(* funcPtr)(DmOpenRef * dbRef, UInt16 rcIndex, Char * addrFullName, ChineseMatePref *cf);
	
	//初始化窗口
	frmP = FrmInitForm(frmFSForm);
	FrmDrawForm(frmP);
	FrmSetActiveForm(frmP);
	
	//Init List
	lstP = (ListType *)FrmGetObjectPtr(frmP, FrmGetObjectIndex(frmP, lstResult));
	LstSetSelection(lstP, noListSelection);
	
	//Init Field
	fldKeyTextPtr = MemPtrNew(6);
	MemSet(fldKeyTextPtr, 6, 0x00);
	WinDrawGrayLine(2, 93, 40, 93);

	//Init Buf
	addrFullName = MemPtrNew(100);
	MemSet(addrFullName, 100, 0x00);
	sStr = MemPtrNew(200);
	MemSet(sStr, 200, 0x00);
	pyStr = MemPtrNew(100);
	MemSet(pyStr, 100, 0x00);
	//Buf[0] = 0; Buf[1] = 0; Buf[2] = 0; Buf[3] = 0; Buf[4] = 0;
	MemSet(Buf, 10, 0x00);
	ExitEventLoop = false;
	
	//Init Func
	if (cf->addType == 1)
	{
		addrCreator = 'PAdd';
		funcPtr = GetFirstName;
	}
	else
	{
		addrCreator = 'addr';
		funcPtr = OLD_GetFirstName;
	}
	
	dbRef = DmOpenDatabaseByTypeCreator('DATA', addrCreator, dmModeReadOnly);
	dbR = DmOpenDatabaseByTypeCreator('CMPY', 'REiC', dmModeReadOnly);
	dbCount = DmNumRecordsInCategory(dbRef, dmAllCategories);
	lstItemStr = MemPtrNew((dbCount << 2));
	
	do 
	{
		EvtGetEvent(&event, evtWaitForever);

		keyHandled = false;
		if (event.eType == keyDownEvent)
		{
			if (EvtKeydownIsVirtual(&event) && matchC > 0)
			{
				if (IsFiveWayNavEvent(&event) && LstGetNumberOfItems(lstP) > 0)
				{
					lstSel = LstGetSelection(lstP);
					if (NavKeyPressed(&event, Up) || event.data.keyDown.chr == 11)
					{
						keyHandled = true;
						if (lstSel != noListSelection)
						{
							if (lstSel > 0)
							{
								LstSetSelection(lstP, lstSel - 1);
							}
							else
							{
								LstSetSelection(lstP, LstGetNumberOfItems(lstP) - 1);
							}
						}
						else
						{
							LstSetSelection(lstP, LstGetTopItem(lstP));
						}
					}
					else if (NavKeyPressed(&event, Down) || event.data.keyDown.chr == 12)
					{
						keyHandled = true;
						if (lstSel != noListSelection)
						{
							if (LstGetSelection(lstP) < LstGetNumberOfItems(lstP) - 1)
							{
								LstSetSelection(lstP, LstGetSelection(lstP) + 1);
							}
							else
							{
								LstSetSelection(lstP, 0);
							}
						}
						else
						{
							LstSetSelection(lstP, LstGetTopItem(lstP));
						}
					}
					else if (NavKeyPressed(&event, Select) && lstSel != noListSelection)
					{
						keyHandled = true;
						bLstBuf = LstGetSelectionText(lstP, lstSel);
						seachOffset = 0;
						i = 0;
						while (i < 5 && bLstBuf[seachOffset])
						{
							seachOffset += TxtGetNextChar(bLstBuf, seachOffset, &Buf[i]);
							if (Buf[i] >= 97 && Buf[i] <= 122)
							{
								Buf[i] -= 32;
							}
							i ++;
						}
						ExitEventLoop = true;
					}
				}
				else if (event.data.keyDown.keyCode == keyLeftAlt)
				{
					if (LstGetTopItem(lstP) - LstGetVisibleItems(lstP) >= 0)
					{
						LstSetTopItem(lstP, LstGetTopItem(lstP) - LstGetVisibleItems(lstP));
					}
					else
					{
						LstSetTopItem(lstP, 0);
					}
					LstSetSelection(lstP, noListSelection);
					LstDrawList(lstP);
				}
				else if (event.data.keyDown.keyCode == keyLeftShift)
				{
					if (LstGetTopItem(lstP) + LstGetVisibleItems(lstP) < LstGetNumberOfItems(lstP))
					{
						LstSetTopItem(lstP, LstGetTopItem(lstP) + LstGetVisibleItems(lstP));
					}
					LstSetSelection(lstP, noListSelection);
					LstDrawList(lstP);
				}
				else if (event.data.keyDown.keyCode == keyHard4)
				{
					ExitEventLoop = true;
					MemSet(&event, sizeof(EventType), 0);
					event.eType = nilEvent;
				}
			}
			else if (event.data.keyDown.chr >= 97 && event.data.keyDown.chr <= 122)
			{
				keyHandled = true;
				keyLength = StrLen(fldKeyTextPtr);
				if (keyLength < 5)
				{
					fldKeyTextPtr[keyLength] = event.data.keyDown.chr - 32;
				}
				WinDrawChars(fldKeyTextPtr, StrLen(fldKeyTextPtr), 2, 82);
				for (i = 0; i < matchC; i ++)
				{
					MemPtrFree(lstItemStr[i]);
				}
				matchC = 0;
				for (i = 0; i < dbCount; i ++)
				{
					MemSet(addrFullName, 100, 0x00);
					MemSet(sStr, 200, 0x00);
					MemSet(pyStr, 100, 0x00);
					(* funcPtr) (dbRef, i, addrFullName, cf);
					if (MatchPY(addrFullName, pyStr, fldKeyTextPtr, sStr, dbR))
					{
						lstItemStr[matchC] = MemPtrNew(StrLen(addrFullName) + 1);
						StrCopy(lstItemStr[matchC], addrFullName);
						matchC ++;
					}
				}
				if (matchC > 0)
				{
					LstSetListChoices (lstP , lstItemStr, matchC);
				}
				else
				{
					LstSetListChoices (lstP , NULL, 0);
				}
				LstDrawList(lstP);
				LstSetSelection(lstP, noListSelection);
			}
			else if (event.data.keyDown.chr == 0x0008) //退格键
			{
				keyHandled = true;
				keyLength = StrLen(fldKeyTextPtr);
				if (keyLength > 0) //最后一个字符不用删，直接退出
				{
					WinEraseChars(fldKeyTextPtr, StrLen(fldKeyTextPtr), 2, 82);
					fldKeyTextPtr[keyLength - 1] = '\0';
					WinDrawChars(fldKeyTextPtr, StrLen(fldKeyTextPtr), 2, 82);
					for (i = 0; i < matchC; i ++)
					{
						MemPtrFree(lstItemStr[i]);
					}
					matchC = 0;
					for (i = 0; i < dbCount; i ++)
					{
						MemSet(addrFullName, 100, 0x00);
						MemSet(sStr, 200, 0x00);
						MemSet(pyStr, 100, 0x00);
						(* funcPtr) (dbRef, i, addrFullName, cf);
						if (MatchPY(addrFullName, pyStr, fldKeyTextPtr, sStr, dbR))
						{
							lstItemStr[matchC] = MemPtrNew(StrLen(addrFullName) + 1);
							StrCopy(lstItemStr[matchC], addrFullName);
							matchC ++;
						}
					}
					if (matchC > 0)
					{
						LstSetListChoices (lstP , lstItemStr, matchC);
					}
					else
					{
						LstSetListChoices (lstP , NULL, 0);
					}
					LstDrawList(lstP);
					LstSetSelection(lstP, noListSelection);
				}
				else
				{
					ExitEventLoop = true;
				}
			}
			else if (event.data.keyDown.chr == 0x000A) //回车
			{
				keyHandled = true;
				keyLength = StrLen(fldKeyTextPtr);
				if (keyLength > 0)
				{
					i = 0;
					while (i < keyLength)
					{
						Buf[i] = fldKeyTextPtr[i];
						i ++;
					}
				}
				ExitEventLoop = true;
			}
			else if (event.data.keyDown.chr == 0x0020) //空格
			{
				keyHandled = true;
				if (matchC > 0 && LstGetSelection(lstP) != noListSelection)
				{
					bLstBuf = LstGetSelectionText(lstP, LstGetSelection(lstP));
					seachOffset = 0;
					i = 0;
					while (i < 5 && bLstBuf[seachOffset])
					{
						seachOffset += TxtGetNextChar(bLstBuf, seachOffset, &Buf[i]);
						if (Buf[i] >= 97 && Buf[i] <= 122)
						{
							Buf[i] -= 32;
						}
						i ++;
					}
					ExitEventLoop = true;
				}
			}
		}
		else if (event.eType == lstSelectEvent)
		{
			keyHandled = true;
			if (matchC > 0 && LstGetSelection(lstP) != noListSelection)
			{
				bLstBuf = LstGetSelectionText(lstP, LstGetSelection(lstP));
				seachOffset = 0;
				i = 0;
				while (i < 5 && bLstBuf[seachOffset])
				{
					seachOffset += TxtGetNextChar(bLstBuf, seachOffset, &Buf[i]);
					if (Buf[i] >= 97 && Buf[i] <= 122)
					{
						Buf[i] -= 32;
					}
					i ++;
				}
				ExitEventLoop = true;
			}
		}
		else if (event.eType == penDownEvent)
		{
			if (event.screenX < 0 || event.screenX > 55 || event.screenY < 0 || event.screenY > 97)
			{
				keyHandled = true;
				ExitEventLoop = true;
			}
		}
		if (! keyHandled)
		{
			if (! SysHandleEvent(&event))
			{
				FrmDispatchEvent(&event);
			}
		}
	}while (event.eType != appStopEvent && ! ExitEventLoop);
	
	DmCloseDatabase(dbRef);
	DmCloseDatabase(dbR);
	MemPtrFree(fldKeyTextPtr);
	MemPtrFree(sStr);
	MemPtrFree(addrFullName);
	MemPtrFree(pyStr);
	for (i = 0; i < matchC; i ++)
	{
		MemPtrFree(lstItemStr[i]);
	}
	MemPtrFree(lstItemStr);
	FrmEraseForm(frmP);
	FrmReturnToForm(0);
}

static UInt16 GetCurLine(UInt16 totalLen, UInt16 lineLen, UInt16 curPos)
{
	UInt16	i;
	UInt16	baseLineLen;
	
	if (curPos > totalLen)
	{
		return 0;
	}
	i = 0;
	baseLineLen = lineLen;
	while(true)
	{
		if (curPos >= lineLen)
		{
			i ++;
			lineLen += baseLineLen;
		}
		else
		{
			break;
		}
	}
	return i;
}

static UInt16 GetMaxLine(UInt16 totalLen, UInt16 lineLen)
{
	UInt16	i;
	UInt16	baseLineLen;
	
	i = 0;
	baseLineLen = lineLen;
	while(true)
	{
		if (lineLen <= totalLen)
		{
			i ++;
			lineLen += baseLineLen;
		}
		else
		{
			break;
		}
	}
	return i;
}

//百家姓
static WChar LastNameHandler(ChineseMatePref *cPref)
{
	EventType	event;
	FormType	*frmP;
	FieldType	*fldKeyPtr;
	FieldType	*fldNamePtr;
	MemHandle	fldKeyTextHandle;
	MemHandle	fldNameTextHandle;
	Char		*fldKeyTextPtr;
	Char		*fldNameTextPtr;
	Char		curKey = 0x00;
	Boolean		ExitEventLoop = false;
	DmOpenRef	*dbRef;
	DmOpenRef	*pydbRef;
	UInt16		dbCount = 0;
	WChar		curChar = 0;
	WChar		prvChar;
	UInt16		nameOffset;
	UInt16		curSel;
	UInt16		curSelEnd;
	UInt16		maxNameLen;
	Boolean		keyHandled;
	UInt16		i;
	//UInt16		startI;
	//UInt16		seachI;
	//UInt16		endI;
	UInt32		addrCreator;
	WChar		(* GetFirstCharPtr) (DmOpenRef * dbRef, UInt16 recI);
	
	frmP = FrmInitForm(frmLastNameForm);
	FrmDrawForm(frmP);
	FrmSetActiveForm(frmP);
	
	//Init Field
	fldKeyPtr = (FieldType *)FrmGetObjectPtr(frmP, FrmGetObjectIndex(frmP, fldLNKey));
	fldKeyTextHandle = MemHandleNew(2);
	if (! fldKeyTextHandle) //err check
	{
		FrmAlert(errMemError);
		return 0;
	}
	fldKeyTextPtr = (Char *)MemHandleLock(fldKeyTextHandle);
	fldKeyTextPtr[0] = 0x00; fldKeyTextPtr[1] = 0x00;
	MemHandleUnlock(fldKeyTextHandle);
	FldSetTextHandle(fldKeyPtr, fldKeyTextHandle);
	
	fldNamePtr = (FieldType *)FrmGetObjectPtr(frmP, FrmGetObjectIndex(frmP, fldName));
	fldNameTextHandle = MemHandleNew(255);
	if (! fldNameTextHandle) //err check
	{
		FrmAlert(errMemError);
		fldKeyTextHandle = FldGetTextHandle(fldKeyPtr);
		FldSetTextHandle(fldKeyPtr, NULL);
		MemHandleFree(fldKeyTextHandle);
		return 0;
	}
	fldNameTextPtr = (Char *)MemHandleLock(fldNameTextHandle);
	fldNameTextPtr[0] = 0x00; fldNameTextPtr[254] = 0x00;
	MemHandleUnlock(fldNameTextHandle);
	FldSetTextHandle(fldNamePtr, fldNameTextHandle);
	
	if (cPref->addType == 1)
	{
		addrCreator = 'PAdd';
		GetFirstCharPtr = GetFirstChar;
	}
	else
	{
		addrCreator = 'addr';
		GetFirstCharPtr = OLD_GetFirstChar;
	}
	
	WinDrawLine(16, 2, 16, 51);
	
	do
	{
		EvtGetEvent(&event, evtWaitForever);
		keyHandled = false;
		if (event.eType == keyDownEvent)
		{
			if (EvtKeydownIsVirtual(&event)) //5-Way Nav
			{
				if (IsFiveWayNavEvent(&event))
				{
					maxNameLen = FldGetTextLength(fldNamePtr);
					fldNameTextHandle = FldGetTextHandle(fldNamePtr);
					fldNameTextPtr = MemHandleLock(fldNameTextHandle);
					FldSetTextHandle(fldNamePtr, NULL);
					if (NavKeyPressed(&event, Right) && maxNameLen > 0)
					{
						keyHandled = true;
						i = FldWordWrap(fldNameTextPtr, 100);
						if (GetCurLine(maxNameLen, i, curSel) < GetCurLine(maxNameLen, i, (curSel + TxtGetNextChar(fldNameTextPtr, curSel, NULL) + 1))) //换行
						{
							curSel = GetCurLine(maxNameLen, i, curSel) * i;
							if (fldNameTextPtr[curSel] == 0x20)
							{
								curSel ++;
							}
							else if ((UInt8)fldNameTextPtr[curSel] > 128)
							{
								if (curSel > 0)
								{
									if ((UInt8)fldNameTextPtr[curSel - 1] > 128)
									{
										curSel --;
									}
								}
							}
						}
						else
						{
							curSel += TxtGetNextChar(fldNameTextPtr, curSel, NULL);
							curSel ++;
							if (curSel >= maxNameLen)
							{
								curSel --;
								curSel -= TxtGetPreviousChar(fldNameTextPtr, curSel, NULL);
								curSel = GetCurLine(maxNameLen, i, curSel) * i;
								if (fldNameTextPtr[curSel] == 0x20)
								{
									curSel ++;
								}
								else if ((UInt8)fldNameTextPtr[curSel] > 128)
								{
									if (curSel > 0)
									{
										if ((UInt8)fldNameTextPtr[curSel - 1] > 128)
										{
											curSel --;
										}
									}
								}
							}
						}
						curSelEnd = curSel + TxtGetNextChar(fldNameTextPtr, curSel, NULL);
					}
					else if (NavKeyPressed(&event, Left))
					{
						keyHandled = true;
						i = FldWordWrap(fldNameTextPtr, 100);
						if (curSel > 0)
						{
							if (GetCurLine(maxNameLen, i, curSel) > GetCurLine(maxNameLen, i, (curSel - TxtGetPreviousChar(fldNameTextPtr, curSel - 1, NULL)))) //换行
							{
								curSel = (GetCurLine(maxNameLen, i, curSel) + 1) * i - 1;
								if (curSel >= maxNameLen)
								{
									curSel = maxNameLen - 1;
									curSel -= TxtGetPreviousChar(fldNameTextPtr, curSel, NULL);
								}
								if (fldNameTextPtr[curSel] == 0x20)
								{
									curSel -= TxtGetPreviousChar(fldNameTextPtr, curSel, NULL);
								}
								else if ((UInt8)fldNameTextPtr[curSel] > 128)
								{
									if (curSel > 0)
									{
										if ((UInt8)fldNameTextPtr[curSel - 1] > 128)
										{
											curSel --;
										}
									}
								}
								curSelEnd = curSel + TxtGetNextChar(fldNameTextPtr, curSel, NULL);
							}
							else
							{
								curSel --;
								curSelEnd = curSel;
								curSel -= TxtGetPreviousChar(fldNameTextPtr, curSel, NULL);
							}
						}
						else
						{
							curSel = i - 1;
							if (curSel >= maxNameLen)
							{
								curSel = maxNameLen - 1;
								curSel -= TxtGetPreviousChar(fldNameTextPtr, curSel, NULL);
							}
							if (fldNameTextPtr[curSel] == 0x20)
							{
								curSel -= TxtGetPreviousChar(fldNameTextPtr, curSel, NULL);
							}
							else if ((UInt8)fldNameTextPtr[curSel] > 128)
							{
								if (curSel > 0)
								{
									if ((UInt8)fldNameTextPtr[curSel - 1] > 128)
									{
										curSel --;
									}
								}
							}
							curSelEnd = curSel + TxtGetNextChar(fldNameTextPtr, curSel, NULL);
						}
					}
					else if ((NavKeyPressed(&event, Up) || event.data.keyDown.chr == 11) && maxNameLen > 0)
					{
						keyHandled = true;
						i = FldWordWrap(fldNameTextPtr, 100);
						if (maxNameLen > i)
						{
							if (GetCurLine(maxNameLen, i, curSel) > 0)
							{
								curSel -= i;
								if (fldNameTextPtr[curSel] == 0x20)
								{
									curSel ++;
								}
								else if ((UInt8)fldNameTextPtr[curSel] > 128)
								{
									if (curSel > 0)
									{
										if ((UInt8)fldNameTextPtr[curSel - 1] > 128)
										{
											curSel --;
										}
									}
								}
								curSelEnd = curSel + TxtGetNextChar(fldNameTextPtr, curSel, NULL);
							}
							else //循环选择
							{
								curSel += i * GetMaxLine(maxNameLen, i);
								if (curSel >= maxNameLen)
								{
									curSel -= i; //回溯一行
								}
								if (fldNameTextPtr[curSel] == 0x20)
								{
									if (curSel > 0)
									{
										curSel --;
									}
								}
								else if ((UInt8)fldNameTextPtr[curSel] > 128)
								{
									if (curSel > 0)
									{
										if ((UInt8)fldNameTextPtr[curSel - 1] > 128)
										{
											curSel --;
										}
									}
								}
								curSelEnd = curSel + TxtGetNextChar(fldNameTextPtr, curSel, NULL);
							}
						}
					}
					else if ((NavKeyPressed(&event, Down) || event.data.keyDown.chr == 12) && maxNameLen > 0)
					{
						keyHandled = true;
						//这里的效率很低，不过考虑到操作并不多，所以不优化了。
						i = FldWordWrap(fldNameTextPtr, 100);
						if (maxNameLen > i)
						{
							if (GetCurLine(maxNameLen, i, curSel) < GetMaxLine(maxNameLen, i))
							{
								curSel += i;
								if (curSel >= maxNameLen)
								{
									curSel -= i;
									curSel -= i * GetCurLine(maxNameLen, i, curSel);
								}
								if (fldNameTextPtr[curSel] == 0x20)
								{
									if (curSel > 0)
									{
										curSel --;
									}
								}
								else if ((UInt8)fldNameTextPtr[curSel] > 128)
								{
									if (curSel > 0)
									{
										if ((UInt8)fldNameTextPtr[curSel - 1] > 128)
										{
											curSel --;
										}
									}
								}
								curSelEnd = curSel + TxtGetNextChar(fldNameTextPtr, curSel, NULL);
							}
							else //循环选择
							{
								curSel -= i * GetCurLine(maxNameLen, i, curSel);
								if ((UInt8)fldNameTextPtr[curSel] == 0x20)
								{
									if (curSel > 0)
									{
										curSel --;
									}
								}
								else if ((UInt8)fldNameTextPtr[curSel] > 128)
								{
									if (curSel > 0)
									{
										if ((UInt8)fldNameTextPtr[curSel - 1] > 128)
										{
											curSel --;
										}
									}
								}
								curSelEnd = curSel + TxtGetNextChar(fldNameTextPtr, curSel, NULL);
							}
						}
					}
					else if (NavKeyPressed(&event, Select))
					{
						ExitEventLoop = true;
						if (curSel != curSelEnd)
						{
							if (((UInt8)fldNameTextPtr[curSel]) > 128)
							{
								curChar = ((((UInt16) fldNameTextPtr[curSel]) << 8) & 0xFF00) | (((UInt16) fldNameTextPtr[curSel + 1]) & 0x00FF);
							}
							else
							{
								curChar = (((UInt16) fldNameTextPtr[curSel]) & 0x00FF);
							}
						}
						else
						{
							curChar = 0;
						}
					}
					MemHandleUnlock(fldNameTextHandle);
					FldSetTextHandle(fldNamePtr, fldNameTextHandle);
					FldDrawField(fldNamePtr);
					if (maxNameLen > 0 && ! ExitEventLoop)
					{
						FldSetSelection(fldNamePtr, curSel, curSelEnd);
					}
				}
				else if (event.data.keyDown.chr == keyHard4)
				{
					curChar = 0;
					ExitEventLoop = true;
					MemSet(&event, sizeof(EventType), 0);
					event.eType = nilEvent;
				}
			}
			else if ((event.data.keyDown.chr >= 97 && event.data.keyDown.chr <= 122) || event.data.keyDown.chr == 46)
			{
				keyHandled = true;
				//Show Key
				fldKeyTextHandle = FldGetTextHandle(fldKeyPtr);
				fldKeyTextPtr = MemHandleLock(fldKeyTextHandle);
				FldSetTextHandle(fldKeyPtr, NULL);
				if (event.data.keyDown.chr != 46)
				{
					curKey = (Char) (event.data.keyDown.chr - 32);
					fldKeyTextPtr[0] = curKey;
				}
				else
				{
					fldKeyTextPtr[0] = '.';
					curKey = '.';
				}
				fldKeyTextPtr[1] = 0x00;
				MemHandleUnlock(fldKeyTextHandle);
				FldSetTextHandle(fldKeyPtr, fldKeyTextHandle);
				FldDrawField(fldKeyPtr);
				//Seach
				//open database
				dbRef = DmOpenDatabaseByTypeCreator('DATA', addrCreator, dmModeReadOnly);
				pydbRef = DmOpenDatabaseByTypeCreator('CMPY', 'REiC', dmModeReadOnly);
				//search method
				curSel = 0;
				curSelEnd = 0;
				if (event.data.keyDown.chr != 46)
				{
					if (cPref->nativeSeach) //native search
					{
						WChar * nsBuf;
						UInt16 nbSp;
						Boolean charExist;
						nsBuf = MemPtrNew(182); //按照文本框最大255个字符的预设，其实最多只能容纳85个汉字
						MemSet(nsBuf, 182, 0x00);
						dbCount = DmNumRecordsInCategory(dbRef, dmAllCategories);
						fldNameTextHandle = FldGetTextHandle(fldNamePtr);
						fldNameTextPtr = MemHandleLock(fldNameTextHandle);
						FldSetTextHandle(fldNamePtr, NULL);
						fldNameTextPtr[0] = 0x00;
						fldNameTextPtr[254] = 0x00;
						nameOffset = 0;
						nsBuf[0] = 0;
						for (i = 0; i < dbCount; i ++)
						{
							curChar = (* GetFirstCharPtr)(dbRef, i);
							charExist = false;
							nbSp = 0;
							while (nsBuf[nbSp] != 0)
							{
								if (nsBuf[nbSp] == curChar) //该姓已存在
								{
									charExist = true;
									break;
								}
								nbSp ++;
							}
							if (curChar > 128 && (! charExist))
							{
								if (CharMatched(curChar, curKey, pydbRef) && nameOffset < 250)
								{
									fldNameTextPtr[nameOffset] = (Char) ((curChar >> 8) & 0x00FF);
									nameOffset ++;
									fldNameTextPtr[nameOffset] = (Char) (curChar & 0x00FF);
									nameOffset ++;
									fldNameTextPtr[nameOffset] = 0x20;
									nameOffset ++;
									fldNameTextPtr[nameOffset] = 0x00;
									nsBuf[nbSp] = curChar;
									nsBuf[nbSp + 1] = 0;
								}
							}
						}
						MemHandleUnlock(fldNameTextHandle);
						FldSetTextHandle(fldNamePtr, fldNameTextHandle);
						FldDrawField(fldNamePtr);
						if (nameOffset > 0)
						{
							FldSetSelection(fldNamePtr, 0, 2);
							curSel = 0;
							curSelEnd = 2;
						}
						else
						{
							curSel = 0;
							curSelEnd = 0;
						}
						MemPtrFree(nsBuf);
					}
				}
				else //non-chinese
				{
					dbCount = DmNumRecordsInCategory(dbRef, dmAllCategories);
					fldNameTextHandle = FldGetTextHandle(fldNamePtr);
					fldNameTextPtr = MemHandleLock(fldNameTextHandle);
					FldSetTextHandle(fldNamePtr, NULL);
					fldNameTextPtr[0] = 0x00;
					fldNameTextPtr[254] = 0x00;
					nameOffset = 0;
					prvChar = 0;
					for (i = 0; i < dbCount; i ++)
					{
						curChar = (* GetFirstCharPtr)(dbRef, i);
						if (curChar <= 128)
						{
							if (curChar != prvChar)
							{
								if (curChar >= 97 && curChar <= 122)
								{
									fldNameTextPtr[nameOffset] = (Char) ((curChar - 32) & 0x00FF);
								}
								else
								{
									fldNameTextPtr[nameOffset] = (Char) (curChar & 0x00FF);
								}
								nameOffset ++;
								fldNameTextPtr[nameOffset] = 0x20;
								nameOffset ++;
								fldNameTextPtr[nameOffset] = 0x00;
								prvChar = curChar;
								if (nameOffset >= 250)
								{
									break;
								}
							}
						}
					}
					MemHandleUnlock(fldNameTextHandle);
					FldSetTextHandle(fldNamePtr, fldNameTextHandle);
					FldDrawField(fldNamePtr);
					if (nameOffset > 0)
					{
						FldSetSelection(fldNamePtr, 0, 1);
						curSel = 0;
						curSelEnd = 1;
					}
				}
				//close database
				DmCloseDatabase(dbRef);
				DmCloseDatabase(pydbRef);
			}
			else if (event.data.keyDown.chr == 0x0020)
			{
				ExitEventLoop = true;
				fldNameTextHandle = FldGetTextHandle(fldNamePtr);
				fldNameTextPtr = MemHandleLock(fldNameTextHandle);
				FldSetTextHandle(fldNamePtr, NULL);
				if (curSel != curSelEnd)
				{
					if (((UInt8)fldNameTextPtr[curSel]) > 128)
					{
						curChar = ((((UInt16) fldNameTextPtr[curSel]) << 8) & 0xFF00) | (((UInt16) fldNameTextPtr[curSel + 1]) & 0x00FF);
					}
					else
					{
						curChar = (((UInt16) fldNameTextPtr[curSel]) & 0x00FF);
					}
				}
				else
				{
					curChar = 0;
				}
				MemHandleUnlock(fldNameTextHandle);
				FldSetTextHandle(fldNamePtr, fldNameTextHandle);
			}
			else if (event.data.keyDown.chr == 0x0008)
			{
				curChar = 0;
				ExitEventLoop = true;
			}
		}
		else if (event.eType == penDownEvent)
		{
			if (event.screenX < 0 || event.screenX > 120 || event.screenY < 0 || event.screenY > 55)
			{
				curChar = 0;
				ExitEventLoop = true;
			}
		}
		else
		{
			 SysHandleEvent(&event);
		}
	}while (event.eType != appStopEvent && ! ExitEventLoop);
	
	fldKeyTextHandle = FldGetTextHandle(fldKeyPtr);
	FldSetTextHandle(fldKeyPtr, NULL);
	MemHandleFree(fldKeyTextHandle);
	fldNameTextHandle = FldGetTextHandle(fldNamePtr);
	FldSetTextHandle(fldNamePtr, NULL);
	MemHandleFree(fldNameTextHandle);
	FrmEraseForm(frmP);
	FrmReturnToForm(0);
	
	return curChar;
}

static void AboutBox(FormType * frmP)
{
	FieldType	* fldP;
	EventType	event;
	Boolean		exit = false;
	const Char	* thxStr = "感谢Baba811、轻轻草语、Joey的测试和支持；感谢许润华(Treo来电秀)的无私指导；感谢wowmouse的帮助文档，以及感谢所有支持我的朋友^_^。";
	
	FrmDrawForm(frmP);
	FrmSetActiveForm(frmP);
	fldP = (FieldType *)FrmGetObjectPtr(frmP, FrmGetObjectIndex(frmP, fldThx));
	
	FldInsert(fldP, thxStr, StrLen(thxStr));
	
	do
	{
		EvtGetEvent(&event, evtWaitForever);
		
		if (event.eType == ctlSelectEvent)
		{
			if (event.data.ctlSelect.controlID == btnOK)
			{
				exit = true;
			}
		}
		else if (! SysHandleEvent(&event))
		{
			FrmDispatchEvent(&event);
		}
	} while (exit == false);
	
	FrmEraseForm(frmP);
}

static Boolean MainFormDoCommand(UInt16 command)
{
	Boolean     handled = false;
	FormType *  pForm;

	switch (command) {
		case menuAbout:
			pForm = FrmInitForm(AboutForm);
			AboutBox(pForm);
			FrmReturnToForm(0);
			handled = true;
			break;
		case menuHelp:
			FrmHelp(1000);
			break;

	}
	
	return handled;
}

static Boolean MainFormHandleEvent(EventType * eventP)
{
	Boolean				handled = false;
	FormType			*frmP;
	LocalID				dbID;
	UInt16				cardNo;
	UInt16				pSize;
	ChineseMatePref		cPref;
	ChineseMatePref		* cpf; //改为指针以便作为消息的参数（2005-11-15）。
	UInt32				cpfAddr;
	//const Char			*trigText_FirstName = "名(First Name)";
	//const Char			*trigText_LastName = "姓(Last Name)";
	//const Char			*trigText_Company = "公司(Company)";
	
	frmP = FrmGetActiveForm();
	pSize = sizeof(ChineseMatePref);
	PrefGetAppPreferences(appFileCreator, appPrefID, &cPref, &pSize, true);
	switch (eventP->eType) 
	{
		case menuEvent:
			return MainFormDoCommand(eventP->data.menu.itemID);
			break;

		case frmOpenEvent:
			
			handled = true;
			break;
			
		case frmUpdateEvent:
			/* 
			 * To do any custom drawing here, first call
			 * FrmDrawForm(), then do your drawing, and
			 * then set handled to true. 
			 */
			break;
			
		case ctlSelectEvent:
		{
			if (eventP->data.ctlSelect.controlID == trigLstSearch)
			{
				FrmShowObject(frmP, FrmGetObjectIndex(frmP, lstSearch));
			}
			else if (eventP->data.ctlSelect.controlID == btnBackToPref)
			{
				LaunchWithCommand('appl', 'pref', sysAppLaunchCmdNormalLaunch, NULL);
			}
			else if (eventP->data.ctlSelect.controlID == pbtnEnableCM)
			{
				if (FrmGetControlValue(frmP, FrmGetObjectIndex(frmP, pbtnEnableCM)) == 1)
				{
					//frmP = FrmGetActiveForm();
					SysCurAppDatabase(&cardNo, &dbID);
					
					cpf = MemPtrNew(pSize);
					PrefGetAppPreferences(appFileCreator, appPrefID, cpf, &pSize, true);
					MemPtrSetOwner(cpf, 0); //把所有者设置为系统
					cpfAddr = (UInt32) cpf;
					FtrSet(appFileCreator, 0, cpfAddr);
					
					cpf->cmActived = false;
					cpf->power = true;
					PrefSetAppPreferences(appFileCreator, appPrefID, appPrefVersionNum, cpf, pSize, true);
					SysNotifyRegister(cardNo, dbID, sysNotifyEventDequeuedEvent, NULL, sysNotifyNormalPriority, cpf);
				}
				else
				{
					if (FtrGet(appFileCreator, 0, &cpfAddr) != ftrErrNoSuchFeature)
					{
						cpf = (ChineseMatePref *) cpfAddr;
						MemPtrFree(cpf);
						FtrUnregister(appFileCreator, 0);
					}
					SysCurAppDatabase(&cardNo, &dbID);
					SysNotifyUnregister(cardNo, dbID, sysNotifyEventDequeuedEvent, sysNotifyNormalPriority);
					cPref.cmActived = false;
					cPref.power = false;
					PrefSetAppPreferences(appFileCreator, appPrefID, appPrefVersionNum, &cPref, pSize, true);
				}
				handled = true;
			}
			else if (eventP->data.ctlSelect.controlID == cbLastName)
			{
				cPref.fsMode = false;
				PrefSetAppPreferences(appFileCreator, appPrefID, appPrefVersionNum, &cPref, pSize, true);
				//frmP = FrmGetActiveForm();
				FrmSetControlValue (frmP, FrmGetObjectIndex (frmP, cbFastSearch), 0);
				handled = true;
			}
			else if (eventP->data.ctlSelect.controlID == cbFastSearch)
			{
				cPref.fsMode = true;
				PrefSetAppPreferences(appFileCreator, appPrefID, appPrefVersionNum, &cPref, pSize, true);
				//frmP = FrmGetActiveForm();
				FrmSetControlValue (frmP, FrmGetObjectIndex (frmP, cbLastName), 0);
				handled = true;
			}
			else if (eventP->data.ctlSelect.controlID == cbRigourSearch)
			{
				cPref.rigourSearch = ! cPref.rigourSearch;
				PrefSetAppPreferences(appFileCreator, appPrefID, appPrefVersionNum, &cPref, pSize, true);
			}
			break;
		}
		case lstSelectEvent:
		{
			cPref.DALaunch = eventP->data.lstSelect.selection;
			PrefSetAppPreferences(appFileCreator, appPrefID, appPrefVersionNum, &cPref, pSize, true);
			if (cPref.DALaunch == FastSearch_FirstName)
			{
				CtlSetLabel(FrmGetObjectPtr(frmP, FrmGetObjectIndex(frmP, trigLstSearch)), trigText_FirstName);
			}
			else if (cPref.DALaunch == FastSearch_LastName)
			{
				CtlSetLabel(FrmGetObjectPtr(frmP, FrmGetObjectIndex(frmP, trigLstSearch)), trigText_LastName);
			}
			else
			{
				CtlSetLabel(FrmGetObjectPtr(frmP, FrmGetObjectIndex(frmP, trigLstSearch)), trigText_Company);
			}
			FrmHideObject(frmP, FrmGetObjectIndex(frmP, lstSearch));
			FrmUpdateForm(MainForm, frmRedrawUpdateCode);
			break;
		}
	}
	
	return handled;
}

static Boolean AppHandleEvent(EventType * eventP)
{
	UInt16 formId;
	FormType * frmP;

	if (eventP->eType == frmLoadEvent)
	{
		/* Load the form resource. */
		formId = eventP->data.frmLoad.formID;
		frmP = FrmInitForm(formId);
		FrmSetActiveForm(frmP);

		/* 
		 * Set the event handler for the form.  The handler of the
		 * currently active form is called by FrmHandleEvent each
		 * time is receives an event. 
		 */
		switch (formId)
		{
			case MainForm:
				FrmSetEventHandler(frmP, MainFormHandleEvent);
				break;

		}
		return true;
	}

	return false;
}

static void AppEventLoop(void)
{
	UInt16				error;
	EventType			event;
	FormType			*frmP;
	UInt16				pSize;
	ChineseMatePref		cPref;
	ChineseMatePref		*cpf;
	UInt32				cpfAddr;
	UInt32				companyID;
	UInt32				deviceID;
	//const Char			*trigText_FirstName = "名(First Name)";
	//const Char			*trigText_LastName = "姓(Last Name)";
	//const Char			*trigText_Company = "公司(Company)";

	pSize = sizeof(ChineseMatePref);
	do
	{
		/* change timeout if you need periodic nilEvents */
		EvtGetEvent(&event, evtWaitForever);

		if (! SysHandleEvent(&event))
		{
			if (! MenuHandleEvent(0, &event, &error))
			{
				if (! AppHandleEvent(&event))
				{
					if (event.eType == frmOpenEvent)
					{
						frmP = FrmGetActiveForm();
						FrmDrawForm(frmP);
						if (PrefGetAppPreferences(appFileCreator, appPrefID, &cPref, &pSize, true) == noPreferenceFound)
						{
							cPref.dbID = 0;
							cPref.addType = 0;
							cPref.fsMode = false;
							cPref.nativeSeach = true;
							cPref.power = false;
							cPref.cmActived = false;
							cPref.rigourSearch = false;
							cPref.DALaunch = FastSearch_FirstName;
							PrefSetAppPreferences(appFileCreator, appPrefID, appPrefVersionNum, &cPref, pSize, true);
						}
						FtrGet(sysFtrCreator, sysFtrNumOEMCompanyID, &companyID);
						FtrGet(sysFtrCreator, sysFtrNumOEMDeviceID, &deviceID);
						if ((companyID == 'hspr') && (deviceID == 'H101' || deviceID == 'H201')) //Treo600
						{
							cPref.dbID = DmFindDatabase(0, "Address Book");
							PrefSetAppPreferences(appFileCreator, appPrefID, appPrefVersionNum, &cPref, pSize, true);
						}
						else //Treo650
						{
							cPref.dbID = DmFindDatabase(0, "Contacts-PAdd");
							PrefSetAppPreferences(appFileCreator, appPrefID, appPrefVersionNum, &cPref, pSize, true);
							
						}
						if ((companyID == 'hspr') && (deviceID == 'H101' || deviceID == 'H201')) //Treo600
						{
							cPref.addType = 0; //Address
						}
						else //Treo650
						{
							cPref.addType = 1; //Contact
						}
						PrefSetAppPreferences(appFileCreator, appPrefID, appPrefVersionNum, &cPref, pSize, true);
						if (cPref.fsMode)
						{
							FrmSetControlValue (frmP, FrmGetObjectIndex (frmP, cbFastSearch), 1);
						}
						else
						{
							FrmSetControlValue (frmP, FrmGetObjectIndex (frmP, cbLastName), 1);
						}
						if (cPref.power)
						{
							FrmSetControlValue (frmP, FrmGetObjectIndex (frmP, pbtnEnableCM), 1);
						}
						else
						{
							FrmSetControlValue (frmP, FrmGetObjectIndex (frmP, pbtnEnableCM), 0);
						}
						if (cPref.rigourSearch)
						{
							FrmSetControlValue (frmP, FrmGetObjectIndex (frmP, cbRigourSearch), 1);
						}
						if (cPref.DALaunch == FastSearch_FirstName)
						{
							CtlSetLabel(FrmGetObjectPtr(frmP, FrmGetObjectIndex(frmP, trigLstSearch)), trigText_FirstName);
						}
						else if (cPref.DALaunch == FastSearch_LastName)
						{
							CtlSetLabel(FrmGetObjectPtr(frmP, FrmGetObjectIndex(frmP, trigLstSearch)), trigText_LastName);
						}
						else
						{
							CtlSetLabel(FrmGetObjectPtr(frmP, FrmGetObjectIndex(frmP, trigLstSearch)), trigText_Company);
							cPref.DALaunch = FastSearch_Company;
						}
					}
					else
					{
						FrmDispatchEvent(&event);
					}
				}
			}
		}
	} while (event.eType != appStopEvent);
	
	if (FtrGet(appFileCreator, 0, &cpfAddr) != ftrErrNoSuchFeature)
	{
		cpf = (ChineseMatePref *) cpfAddr;
		PrefGetAppPreferences(appFileCreator, appPrefID, cpf, &pSize, true);
	}
}

UInt32 PilotMain(UInt16 cmd, MemPtr cmdPBP, UInt16 launchFlags)
{
	LocalID				dbID;
	UInt16				pSize;
	WChar				Buf[5];
	UInt16				i;
	UInt32				cfAddr;
	ChineseMatePref		*cf;
	EventType			*ep;
	FormType			*frmP = NULL;

	switch (cmd)
	{
		case sysAppLaunchCmdNotify:
		{
			SysNotifyParamType * notifyPtr = (SysNotifyParamType *)cmdPBP;
			if (notifyPtr->notifyType == sysNotifyEventDequeuedEvent)
			{
				ep = (EventType *)notifyPtr->notifyDetailsP;
				cf = (ChineseMatePref *)notifyPtr->userDataP;
				if (ep->eType == NativeKeyDownEvent)
				{
					WChar keyChr = (WChar) ByteSwap16(ep->data.keyDown.chr);
					if (((keyChr >= keyA && keyChr <= keyZ) || keyChr == keyPeriod))
					{
						//通讯录伴侣激活
						frmP = FrmGetActiveForm();
						if (frmP)
						{
							i = FrmGetNumberOfObjects(frmP);
							pSize = FrmGetFormId(frmP);
							if ((cf->addType == 0 && pSize == 1000 && i == 13) || (cf->addType == 1 && pSize == 1100 && i == 23))
							{
								notifyPtr->handled = true;
								SysCurAppDatabase(&i, &dbID);
								SysNotifyUnregister(0, dbID, sysNotifyEventDequeuedEvent, sysNotifyNormalPriority);
								MemSet(ep, sizeof(EventType), 0);
								ep->eType = nilEvent;
								if (cf->fsMode) //名字检索
								{
									Buf[0] = 0; Buf[1] = 0; Buf[2] = 0; Buf[3] = 0; Buf[4] = 0;
									EvtEnqueueKey(keyChr, 0, 0);
									FirstNameHandler(Buf, cf);
									i = 0;
									while (Buf[i] > 0 && i < 5)
									{
										EvtEnqueueKey(Buf[i], 0, 0);
										i ++;
									}
								}
								else //百家姓
								{
									EvtEnqueueKey(keyChr, 0, 0);
									Buf[0] = LastNameHandler(cf);
									if (Buf[0] > 0)
									{
										EvtEnqueueKey(0x0008, 0, 0);
										EvtEnqueueKey(0x0008, 0, 0);
										if (EvtEnqueueKey(Buf[0], 0, 0))
										{
											FrmAlert(errEnqueueKeyError);
										}
									}
								}
								SysNotifyRegister(0, dbID, sysNotifyEventDequeuedEvent, NULL, sysNotifyNormalPriority, cf);
							}
						}
					}
				}
			}
			break;
		}
		case sysAppLaunchCmdSystemReset:
		{
			pSize = sizeof(ChineseMatePref);
			cf = MemPtrNew(pSize);
			if (PrefGetAppPreferences(appFileCreator, appPrefID, cf, &pSize, true) == noPreferenceFound)
			{
				cf->dbID = 0;
				cf->addType = 0;
				cf->fsMode = false;
				cf->nativeSeach = true;
				cf->power = false;
				cf->cmActived = false;
				
				cf->fiKey = 3;
				cf->fiEnabled = false;
			
				cf->fcdEnabled = false;
				
				cf->dpEnabled = false;
				cf->phdbID = 0;
				cf->dpON = true;
				PrefSetAppPreferences(appFileCreator, appPrefID, appPrefVersionNum, cf, pSize, true);
			}
			MemPtrSetOwner(cf, 0);
			cfAddr = (UInt32) cf;
			FtrSet(appFileCreator, 0, cfAddr);
			SysCurAppDatabase(&i, &dbID);
			if (cf->power)
			{
				cf->cmActived = false;
				SysNotifyRegister(0, dbID, sysNotifyEventDequeuedEvent, NULL, sysNotifyNormalPriority, cf);
				PrefSetAppPreferences(appFileCreator, appPrefID, appPrefVersionNum, cf, pSize, true);
			}
			break;
		}
		case sysAppLaunchCmdNormalLaunch:
		{
			FrmGotoForm(MainForm);
			AppEventLoop();
			FrmCloseAllForms();
			break;
		}
		case 13: //Panel启动
		{
			FrmGotoForm(MainForm);
			AppEventLoop();
			FrmCloseAllForms();
			break;
		}
	}

	return errNone;
}
