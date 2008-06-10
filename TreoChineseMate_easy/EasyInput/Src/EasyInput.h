/*
 * EasyInput.h
 *
 * header file for EasyInput
 *
 * This wizard-generated code is based on code adapted from the
 * stationery files distributed as part of the Palm OS SDK 4.0.
 *
 * Copyright (c) 1999-2000 Palm, Inc. or its subsidiaries.
 * All rights reserved.
 */
#include "CLib.h" 

#ifndef EASYINPUT_H_
#define EASYINPUT_H_

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
	Char				cache[256];
	WChar				initChr;
	UInt16				initKeyCode;
	SettingType			*settingP;
	//----------------
	UInt16				refNum;
	CNLBGlobeType		*CNLBGlobeP;
	CNLBParameterType	*CNLBparamP;
	//-----------------
	RectangleType		bgRect;
	FormType			*frmContactsP;
	ListType			*lstContactsP;
	RectangleType		lstClip;
	UInt16				lstContactsIndex;
	FieldType			*fldPhoneNumberP;
	FieldType			*fldPYKeywordP;
	RectangleType		phoneNumberRect;
	RectangleType		PYKeywordRect;
	RectangleType		typeRect;
	UInt16				focusedObjectId;
	//-----------------
	Char				pyKeyword[11];
	UInt16				pyKeywordLength;
	Char				*phoneNumberP;
	UInt16				phoneNumberLength;
	//-----------------
	RGBColorType		alternateRGBColor;
	RGBColorType		originalRGBColor;
	RGBColorType		originalTextRGBColor;
	RGBColorType		highlightTextRGBColor;
	RGBColorType		highlightBGRGBColor;
	RGBColorType		originalListFrameRGBColor;
	RGBColorType		pyMatchRGBColor;
	RGBColorType		phoneNumberMatchRGBColor;
	//-----------------
	DmOpenRef			recentDbRef;
	RecentUnitType		*recentListP;
	UInt16				recentCount;
	//-----------------
	ContactUnitType		**contactListPP;
	void				*contactUnitP;
	UInt8				currentField;
	MemHandle			contactListH;
	UInt16				contactCount;
	UInt16				totalContacts;
	//-----------------
	void				(*drawContactListByContactP)(Int16 itemNum, RectangleType *bounds, Char **itemsText);
	void				(*getContactListP)(Char *PYKeywordP, Char *phoneNumberP, CNLBGlobeType *CNLBglobeP, CNLBParameterType *CNLBparamP, void *voidGlobeP);
	void				(*updateFormControlPtrP)(void *voidGlobeP);
} GlobeType;

typedef struct
{
	SettingType			*settingP;
	Char				*resultP;
	KeyInfoType			*contactInitKeyP;
	FormType			*frmEasyInputPuncP;
	ControlType			*pbtnPuncP[8];
	ControlType			*pbtnDateP;
	ControlType			*pbtnTimeP;
	ListType			*lstQuickTextP;
	UInt16				focusedObjectId;
	Char				**quickTextPP;
	UInt16				quickTextCount;
	Char				date[maxStrIToALen];
	Char				time[maxStrIToALen];
} PuncGlobeType;

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

#define appFileCreator			'EzIP'
#define appName					"EasyInput"
#define appVersionNum			0x02
#define appPrefID				0x00
#define appPrefVersionNum		0x00

/*#define NoApp					0xFF
#define ContactsApp				0x00
#define PhoneApp				0x01
#define MsgApp					0x02

#define bgColor					0x00
#define curColor				0x01
#define defObjFrame				0x02
#define defObjFill				0x03
#define defDiagFrame			0x04
#define defDiagFill				0x05

#define daNoAction				0x00
#define daDialPad				0x01
#define daEasyInput				0x02*/

#define TREO_680_GREEN_KEY		0x021A
#define TREO_600				0x00
#define TREO_650				0x01
#define TREO_680				0x02
#define NOT_A_TREO				0xFF

#define lstPopupId							2000
#define frmContactsId						10000
#define lstContactsId						10001

#define NO_ACTION							0x00
#define EASY_PHONE_LONG_PRESS_ACTIVE		0x01
#define EASY_PHONE_PHONE_UI_ACTIVE			0x02
#define EASY_INPUT_KEY_ACTIVE				0x03
#define EASY_INPUT_SMS_FIELD_ACTIVE			0x04
#define EASY_CONTACT_ACTIVE					0x05
#define EASY_CONTACT_NEW_CONTACT			0x06

#define LAUNCH_CODE_ACTION					sysAppLaunchCmdCustomBase + 0x1030

#define FORM_NO_UPDATE			0
#define FORM_UPDATE_CUSTOM		1

#define NO_EXIT					0x00
#define NORMAL_RESULT			0x00
#define RECENT_RESULT			0x01
#define	PY_MATCH_RESULT			0x02
#define PHONE_MATCH_RESULT		0x03

#define EXIT_FOR_NOTHING		0x01
#define EXIT_FOR_ALL			0x02
#define EXIT_FOR_NAMEONLY		0x03
#define EXIT_FOR_PHONEONLY		0x04
#define EXIT_FOR_NAME_PHONE		0x05
#define EXIT_FOR_CONTACT_SEARCH	0x06

#define PARAMETER_TOTAL			0x00
#define PARAMETER_SELECT		0x01
#define PARAMETER_NAME_SELECT	0x02
#define MOVE_UP					0x00
#define MOVE_DOWN				0x01
#define MOVE_BY_CLICK			0x02
#define NOT_A_NUMBER			0xFFFF

#define NO_FOCUS				0x00
#define CONTACT_LIST			0x01
#define GET_TOTAL_BUTTON		0x02
#define GET_MORE_BUTTON			0x03
#define EXIT_BUTTON				0x04
#define PUNC_PUSHBUTTON			0x05
#define DATE_PUSHBUTTON			0x06
#define TIME_PUSHBUTTON			0x07
#define QUICK_TEXT_LIST			0x08
#define OPEN_CONTACT_BUTTON		0x09

#define ByteSwap16(n) ( ((((unsigned int) n) << 8) & 0xFF00) | ((((unsigned int) n) >> 8) & 0x00FF) )
#define getObjectPtr(f, o)		FrmGetObjectPtr(f, FrmGetObjectIndex(f, o))

#endif /* EASYINPUT_H_ */
