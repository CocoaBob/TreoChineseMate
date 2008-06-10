/*
 * EasyPhone.h
 *
 * header file for EasyPhone
 *
 * This wizard-generated code is based on code adapted from the
 * stationery files distributed as part of the Palm OS SDK 4.0.
 *
 * Copyright (c) 1999-2000 Palm, Inc. or its subsidiaries.
 * All rights reserved.
 */

#include "CLib.h"

#ifndef EASYPHONE_H_
#define EASYPHONE_H_

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

/*typedef struct
{
	//OLD_AddrDBRecordType	data;
	UInt8					matchType;
	Boolean					shouldChangeColor;
	RectangleType			bounds;
	UInt16					recordId;
} ContactUnitType_Addr;*/

typedef struct
{
	UInt8					matchType;
	Boolean					shouldChangeColor;
	RectangleType			bounds;
	UInt16					recordId;
} ContactUnitType;

typedef struct
{
	UInt16					index;
	UInt8					field;
	Boolean					shouldChangeColor;
} ListMappingType;

typedef struct
{
	Char				cache[256];
	SettingType			*settingP;
	//----------------
	UInt16				refNum;
	CNLBGlobeType		*CNLBGlobeP;
	CNLBParameterType	*CNLBparamP;
	//----------------
	Char				**preFix;
	UInt8				currentPreFix;
	UInt8				totalPreFix;
	//-----------------
	RectangleType		bgRect;
	FormType			*frmContactsP;
	ListType			*lstContactsP;
	UInt16				lstContactsIndex;
	RectangleType		lstClip;
	RectangleType		PYKeywordRect;
	RectangleType		phoneNumberRect;
	FieldType			*fldPhoneNumberP;
	FormGadgetType		*gdtDialButtonP;
	FormGadgetType		*gdtPreFixTriggerP;
	ListType			*lstPreFixP;
	FormGadgetType		*gdtOperationButtonP;
	ListType			*lstOperationP;
	Char				**operationPP;
	FormGadgetType		*gdtSMSButtonP;
	ListType			*lstSMSP;
	Char				**SMSPP;
	FormGadgetType		*gdtHelpButtonP;
	WinHandle			bgBitsWinH;
	MemHandle			skinBmpH;
	BitmapType			*skinBmpP;
	RectangleType		typeRect;
	UInt16				focusedObjectId;
	Boolean				isFocusRingDrawed;
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
	ListMappingType		*listMappingP;
	UInt16				listMappingSize;
	UInt16				listMappingCount;
	//-----------------
	Char				pyKeyword[11];
	UInt16				pyKeywordLength;
	//-----------------
	void				(*drawContactListByContactP)(Int16 itemNum, RectangleType *bounds, Char **itemsText);
	void				(*getContactListP)(CNLBGlobeType *CNLBglobeP, CNLBParameterType *CNLBparamP, void *voidGlobeP);
	//-----------------
	Boolean				isMenuNotActive;
} GlobeType;

typedef struct
{
	Boolean				capsLock;
	Boolean				numLock;
	Boolean				optLock;
	UInt16				tempShift;
	Boolean				autoShifted;
} GrfStatusType;

typedef struct
{
	UInt16				contentLength;
	UInt16				messageCount;
	Char				lengthAndCount[32];
} MessageGlobeType;

/*********************************************************************
 * Global variables
 *********************************************************************/


/*********************************************************************
 * Internal Constants
 *********************************************************************/

#define appFileCreator			'EzPH'
#define appName					"EasyPhone"
#define appVersionNum			0x01
#define appPrefID				0x00
#define appPrefVersionNum		0x01

#define TREO_680_GREEN_KEY		0x021A
#define TREO_600				0x00
#define TREO_650				0x01
#define TREO_680				0x02
#define NOT_A_TREO				0xFF

#define frmContactsId			0x1030
#define lstContactsId			0x1031
#define	gdtDialButtonId			0x1032
#define fldPhoneNumberId		0x1033
#define gdtPreFixTriggerId		0x1034
#define gdtOperationButtonId	0x1035
#define gdtSMSButtonId			0x1036
#define gdtHelpButtonId			0x1037
#define lstPreFixId				0x1038
#define lstPopupId				0x1039
#define lstOperationId			0x103A
#define lstSMSId				0x103B

#define FORM_NO_UPDATE			0
#define FORM_UPDATE_CUSTOM		1
#define FORM_UPDATE_REFLASH		2
#define FORM_UPDATE_REFLASH2	3

#define NO_EXIT					0x00
#define NORMAL_RESULT			0x00
#define RECENT_RESULT			0x01
#define	PY_MATCH_RESULT			0x02
#define PHONE_MATCH_RESULT		0x03

#define SHOW_DETAIL				0x00
#define NO_DETAIL				0x01
#define SHOW_CARD				0x02

#define KEY_HARD2_SAVE_CONTACT				0x00
#define KEY_HARD2_OPEN_CONTACT				0x01

#define KEY_HARD3_SYSTEM_MESSAGE			0x00
#define KEY_HARD3_EASYPHONE_MESSAGE			0x01

#define EXIT_FOR_NOTHING		0x01
#define EXIT_FOR_DIAL			0x02
#define EXIT_FOR_MESSAGE		0x03
#define EXIT_FOR_SYSTEM_MESSAGE	0x04
#define EXIT_FOR_EMAIL			0x05
#define EXIT_FOR_DIRECT_DIAL	0x06
#define EXIT_FOR_ADD_CONTACT	0x07
#define EXIT_FOR_OPEN_CONTACT	0x08
#define EXIT_FOR_RETURN_CONTACT	0x09

#define PARAMETER_WITH_IP		0x00
#define PARAMETER_WITHOUT_IP	0x01
#define PARAMETER_FOR_EMAIL		0x02
#define PARAMETER_FOR_OPEN		0x03
#define PARAMETER_FOR_RETURN	0x04

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

#define LAUNCH_CODE_ACTION					sysAppLaunchCmdCustomBase + 0x1030

//****************************************************************************
//获取指定的窗体组件的指针
//****************************************************************************
#define getObjectPtr(f, o)		FrmGetObjectPtr(f, FrmGetObjectIndex(f, o))

#endif /* EASYPHONE_H_ */
