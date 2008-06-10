/*
 * TreoChineseMate.h
 *
 * header file for TreoChineseMate
 *
 * This wizard-generated code is based on code adapted from the
 * stationery files distributed as part of the Palm OS SDK 4.0.
 *
 * Copyright (c) 1999-2000 Palm, Inc. or its subsidiaries.
 * All rights reserved.
 */
 
#ifndef TREOCHINESEMATE_H_
#define TREOCHINESEMATE_H_

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
	Boolean				isInstalled;
	UInt16				cardNo;
	LocalID				dbId;
} ModuleInfomationType;

/*typedef struct
{
	Char				inputFileName[256];
	Char				outputFileName[256];
	
	FileRef				inputFileRef;
	FileRef				outputFileRef;
	
	UInt32				width;
	UInt32				height;
	UInt32				rowBytes;
	UInt32				colorFormat;
	UInt32				fileFormat;
	
	Boolean				encoding;
	Boolean				fitToScreen;
	
	PalmCodecSession	session;
} ImageInfo;*/

/*********************************************************************
 * Global variables
 *********************************************************************/


/*********************************************************************
 * Internal Constants
 *********************************************************************/

#define APPLICATION_CREATOR					'CNFE'
#define APPLICATION_PREF_ID					0x02
#define APPLICATION_PREF_VER				0x02

#define SHOW_DETAIL				0x00
#define NO_DETAIL				0x01
#define SHOW_CARD				0x02

#define KEY_HARD2_SAVE_CONTACT				0x00
#define KEY_HARD2_OPEN_CONTACT				0x01

#define KEY_HARD3_SYSTEM_MESSAGE			0x00
#define KEY_HARD3_EASYPHONE_MESSAGE			0x01

#define TREO_600							0x00
#define TREO_650							0x01
#define TREO_680							0x02
#define CENTRO								0x03
#define NOT_A_TREO							0xFF
#define LASTNAME_MODE						0x00
#define FIRSTNAME_MODE						0x01

#define NATIVE_KEYDOWN_EVENT				0x0400

#define INVAILD_FORM						0x00
#define IN_PHONE_FORM						0x01
#define IN_SMS_FORM_FIELD					0x02
#define IN_FOCUSED_FIELD					0x03
#define IN_CONTACT_FORM						0x04
#define EVENT_SHOULD_BE_CLEAR				0x05

#define NO_ACTION							0x00
#define EASY_PHONE_LONG_PRESS_ACTIVE		0x01
#define EASY_PHONE_PHONE_UI_ACTIVE			0x02
#define EASY_INPUT_KEY_ACTIVE				0x03
#define EASY_INPUT_SMS_FIELD_ACTIVE			0x04
#define EASY_CONTACT_ACTIVE					0x05
#define EASY_CONTACT_NEW_CONTACT			0x06

#define LAUNCH_CODE_ACTION					sysAppLaunchCmdCustomBase + 0x1030
#define PHONE_FORM_ID_TREO600				7000
#define PHONE_FORM_ID_TREO650				7000
#define PHONE_FORM_ID_TREO680				25225
#define PHONE_FORM_ID_CENTRO				25225
#define PHONE_FORM_OBJECT_NUMBER_TREO600	45
#define PHONE_FORM_OBJECT_NUMBER_TREO650	66
#define PHONE_FORM_OBJECT_NUMBER_TREO680	27
#define PHONE_FORM_OBJECT_NUMBER_CENTRO		26

#define SMS_FORM_ID_TREO600					3100
#define SMS_FORM_ID_TREO650					8500
#define SMS_FORM_ID_TREO680					8600
#define SMS_FORM_ID_CENTRO					8600
#define SMS_FORM_OBJECT_NUMBER_TREO600		20
#define SMS_FORM_OBJECT_NUMBER_TREO650		146
#define SMS_FORM_OBJECT_NUMBER_TREO680		58
#define SMS_FORM_OBJECT_NUMBER_CENTRO		68
#define SMS_FORM_FIELD_INDEX_TREO600		5
#define SMS_FORM_FIELD_INDEX_TREO650		16
#define SMS_FORM_FIELD_INDEX_TREO680		9
#define SMS_FORM_FIELD_INDEX_CENTRO			9

#define CONTACT_FORM_ID_TREO600				1000
#define CONTACT_FORM_ID_TREO650				1100
#define CONTACT_FORM_ID_TREO680				25206
#define CONTACT_FORM_ID_CENTRO				25206
#define CONTACT_FORM_OBJECT_NUMBER_TREO600	13
#define CONTACT_FORM_OBJECT_NUMBER_TREO650	23
#define CONTACT_FORM_OBJECT_NUMBER_TREO680	24
#define CONTACT_FORM_OBJECT_NUMBER_CENTRO	24
#define CONTACT_FORM_TABLE_INDEX_TREO600	3
#define CONTACT_FORM_TABLE_INDEX_TREO650	3
#define CONTACT_FORM_TABLE_INDEX_TREO680	18
#define CONTACT_FORM_TABLE_INDEX_CENTRO		18

#define EXIT_FOR_NOTHING		0x01
#define EXIT_FOR_DIAL			0x02
#define EXIT_FOR_MESSAGE		0x03
#define EXIT_FOR_SYSTEM_MESSAGE	0x04
#define EXIT_FOR_EMAIL			0x05
#define EXIT_FOR_DIRECT_DIAL	0x06
#define EXIT_FOR_ADD_CONTACT	0x07
#define EXIT_FOR_OPEN_CONTACT	0x08
#define EXIT_FOR_RETURN_CONTACT	0x09

#define STATIC_MODIFIERS	(	shiftKeyMask      | \
								capsLockMask      | \
								numLockMask       | \
								commandKeyMask    | \
								optionKeyMask     | \
								controlKeyMask    | \
								poweredOnKeyMask  | \
								appEvtHookKeyMask | \
								libEvtHookKeyMask | \
								willSendUpKeyMask   \
							)
#define DYNAMIC_MODIFIERS	(	autoRepeatKeyMask | \
								doubleTapKeyMask    \
							)
#define	byteSwap16(n)			( ((((unsigned int) n) << 8) & 0xFF00) | ((((unsigned int) n) >> 8) & 0x00FF) )
#define byteSwap32(n)			(((((UInt32) n) << 24) & 0xFF000000) | \
								 ((((UInt32) n) <<  8) & 0x00FF0000) | \
								 ((((UInt32) n) >>  8) & 0x0000FF00) | \
								 ((((UInt32) n) >>  24) & 0x000000FF))
#define getObjectPtr(f, o)		FrmGetObjectPtr(f, FrmGetObjectIndex(f, o))

#endif /* TREOCHINESEMATE_H_ */
