/*
 * EasyContact.h
 *
 * header file for EasyContact
 *
 * This wizard-generated code is based on code adapted from the
 * stationery files distributed as part of the Palm OS SDK 4.0.
 *
 * Copyright (c) 1999-2000 Palm, Inc. or its subsidiaries.
 * All rights reserved.
 */
 
#ifndef EASYCONTACT_H_
#define EASYCONTACT_H_

#include "CLib.h"

/*********************************************************************
 * Internal Structures
 *********************************************************************/
typedef struct
{
	WChar				chr;
	UInt16				keyCode;
	UInt16				modifiers;
} KeyInfoType;

typedef struct
{
	RectangleType		bounds;
	UInt16				other;
} ObjectSettingType;

typedef struct
{
	Char				bmpPath[PALM_PHOTO_MAX_PATH + 1];
	Coord				bmpWidth;
	Coord				bmpHeight;
	UInt16				bmpDensity;
	void				*bmpBitsP;
	UInt32				bmpBitsSize;
	ObjectSettingType	frmContacts;
	ObjectSettingType	lstContacts;
	ObjectSettingType	gdtDialButton;
	ObjectSettingType	fldPhoneNumber;
	ObjectSettingType	gdtPreFixTrigger;
	ObjectSettingType	gdtOperationButton;
	ObjectSettingType	gdtSMSButton;
	ObjectSettingType	gdtHelpButton;
} SkinSettingType;

typedef struct
{
	//Common
	Boolean				isShowInPref;
	Boolean				isEasyPhoneEnabled;
	Boolean				isEasyInputEnabled;
	Boolean				isEasyContactEnabled;
	Boolean				orderlySearch;
	UInt8				treoModel;
	Boolean				isInPhoneUI;
	Boolean				isInContactUI;
	UInt8				actionCode;
	UInt32				actionCreator;
	WChar				actionKey;
	UInt16				actionKeycode;
	Char				r1[32];
	
	//EasyPhone
	Char				preFix[6][21];
	KeyInfoType			customActiveKey;
	UInt16				initDelay;
	Boolean				isTempDisabled;
	Boolean				isNotAutoLaunchInPhoneUI;
	Boolean				isPhoneUIEnabled;
	UInt8				defaultField;
	UInt16				phoneFormId;
	UInt16				phoneFormObjectNumber;
	SkinSettingType		skin;
	UInt8				showDetailType;
	Boolean				shouldUseOldOperationType;
	UInt8				keyHard2Action;
	UInt8				keyHard3Action;
	Char				r2[32];
	
	//EasyInput
	WChar				easyInputActiveKey;
	Char				punc[8][9];
	UInt16				smsFormId;
	UInt16				smsFormObjectNumber;
	UInt16				smsFormFieldIndex;
	FieldType			*currentField;
	Char				r3[32];
	
	//EasyContact
	UInt8				searchMode;
	UInt16				contactFormId;
	UInt16				contactFormObjectNumber;
	UInt16				contactFormTableIndex;
	Char				r4[32];
} SettingType;

typedef struct
{
	SettingType			*settingP;
	Boolean				hasActionToPerform;
	UInt8				actionCode;
	Char				fullName[256];
	Char				phoneNumber[128];
	UInt16				recordId;
	Char				*dataP;
} ActionParameterType;

typedef struct
{
	Char				fullName[256];
	Char				phoneNumber[128];
	Boolean				shouldChangeColor;
	RectangleType		bounds;
} RecentUnitType;

typedef struct
{
	UInt8					matchType;
	Boolean					shouldChangeColor;
	RectangleType			bounds;
	UInt16					recordId;
} ContactUnitType;

typedef struct
{
	Char				cache[128];
	UInt16				refNum;
	CNLBGlobeType		*CNLBGlobeP;
	SettingType			*settingP;
	RGBColorType		nameItemRGBColor;
	RGBColorType		nameItemRGBColorR;
	RGBColorType		nameItemRGBColorP;
	RGBColorType		nameItemRGBColorS;
	RGBColorType		originalRGBColor;
	ListType			*lstContactsP;
	MemHandle			phoneNumberH;
	Char				*phoneNumberP;
	UInt16				phoneNumberLength;
	MemHandle			pyKeywordH;
	Char				*pyKeywordP;
	UInt16				pyKeywordLength;
	void				**contactList;
	MemHandle			contactListH;
	UInt16				contactCount;
	UInt16				totalContacts;
	FieldType			*fldRP[45];
	MemHandle			fldRTextH[45];
	Char				*fldRTextP[45];
	UInt16				usedField;
	UInt16				row;
	UInt16				col;
} GlobeType;

typedef struct
{
	Boolean				capsLock;
	Boolean				numLock;
	Boolean				optLock;
	UInt16				tempShift;
	Boolean				autoShifted;
} GrfStatusType;

/*********************************************************************
 * Global variables
 *********************************************************************/


/*********************************************************************
 * Internal Constants
 *********************************************************************/

#define appFileCreator			'EzCT'
#define appName					"EasyContact"
#define appVersionNum			0x01
#define appPrefID				0x00
#define appPrefVersionNum		0x01


#define TREO_680_GREEN_KEY		0x021A
#define TREO_600				0x00
#define TREO_650				0x01
#define TREO_680				0x02
#define NOT_A_TREO				0xFF

#define FORM_UPDATE_CUSTOM		1
#define NO_EXIT					0x00
#define NORMAL_RESULT			0x00
#define RECENT_RESULT			0x01
#define	PY_MATCH_RESULT			0x02
#define PHONE_MATCH_RESULT		0x03
#define EXIT_FOR_NOTHING		0x01
#define EXIT_FOR_DIAL			0x02
#define EXIT_FOR_MESSAGE		0x03
#define EXIT_FOR_SYSTEM_MESSAGE	0x04
#define EXIT_FOR_EMAIL			0x05
#define EXIT_FOR_ADD_CONTACT	0x06
#define EXIT_FOR_OPEN_CONTACT	0x07
#define EXIT_FOR_RETURN_CONTACT	0x08
#define PARAMETER_WITH_IP		0x00
#define PARAMETER_WITHOUT_IP	0x01
#define PARAMETER_FOR_EMAIL		0x02
#define PARAMETER_FOR_OPEN		0x03
#define PARAMETER_FOR_RETURN	0x04
#define MOVE_UP					0x00
#define MOVE_DOWN				0x01
#define MOVE_LEFT				0x02
#define MOVE_RIGHT				0x03
#define MOVE_BY_CLICK			0x04
#define NOT_A_NUMBER			0xFFFF
#define LASTNAME_MODE			0x00
#define FIRSTNAME_MODE			0x01

#define LAUNCH_CODE_ACTION					sysAppLaunchCmdCustomBase + 0x1030

#define byteSwap16(n) ( ((((unsigned int) n) << 8) & 0xFF00) | ((((unsigned int) n) >> 8) & 0x00FF) )
#define getObjectPtr(f, o)		FrmGetObjectPtr(f, FrmGetObjectIndex(f, o))

#endif /* EASYCONTACT_H_ */
