/*
 * ChineseMate.h
 *
 * header file for ChineseMate
 *
 * This wizard-generated code is based on code adapted from the
 * stationery files distributed as part of the Palm OS SDK 4.0.
 *
 * Copyright (c) 1999-2000 Palm, Inc. or its subsidiaries.
 * All rights reserved.
 */
 
#ifndef CHINESEMATE_H_
#define CHINESEMATE_H_

/*********************************************************************
 * Internal Structures
 *********************************************************************/
typedef struct
{
	LocalID		dbID;
	UInt16		addType;
	Boolean		fsMode;
	Boolean		nativeSeach;
	Boolean		power;
	Boolean		cmActived;
	
	WChar		fiKey;
	Boolean		fiEnabled;
	
	Boolean		fcdEnabled;
	
	Boolean		dpEnabled;
	LocalID		phdbID;
	Boolean		dpON;
	UInt8		DALaunch;
	Boolean		rigourSearch;
	
	UInt16		curApp;
}ChineseMatePref;

typedef struct
{
	UInt8		actionCode;
	Char		* data;
}actionType;

typedef struct
{
	UInt16		recIndex;
	UInt8		phnLabel;	
	void		*next;
}MRecordType;

typedef struct
{
	DmOpenRef	*dbRef;
	MRecordType *mRec;
	UInt16		mRecC;
	Char		*nameBuf;
	Boolean		(*lstGetSelTextPtr)(void * lparam, Int16 itemNum, UInt8 *phnType);
	Boolean		dpMode;
	ListType	*curLstP;
}dpDrawListParam;

/*********************************************************************
 * Global variables
 *********************************************************************/


/*********************************************************************
 * Internal Constants
 *********************************************************************/

#define appFileCreator			'REiC'
#define appName					"ContactMate"
#define appVersionNum			0x01
#define appPrefID				0x00
#define appPrefVersionNum		0x01

#define ByteSwap16(n) ( ((((unsigned int) n) << 8) & 0xFF00) | ((((unsigned int) n) >> 8) & 0x00FF) )

#define NativeKeyDownEvent		0x0400
#define FastSearch_FirstName	0x00
#define FastSearch_LastName		0x01
#define FastSearch_Company		0x02

#endif /* CHINESEMATE_H_ */
