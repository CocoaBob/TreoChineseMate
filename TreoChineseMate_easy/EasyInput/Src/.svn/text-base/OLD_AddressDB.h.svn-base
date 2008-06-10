/******************************************************************************
 *
 * Copyright (c) 1995-2002 PalmSource, Inc. All rights reserved.
 *
 * File: AddressDB.h
 *
 * Release: Palm OS 5 SDK (68K) R2.
 *
 *****************************************************************************/

#ifndef OLD_ADDRESSDB_H
#define OLD_ADDRESSDB_H

#include "OLD_AddrDefines.h"

#include <AppLaunchCmd.h>
#include <CoreTraps.h>
#include <LocaleMgr.h>
#include <DataMgr.h>


/***********************************************************************
 *   Defines
 ***********************************************************************/

#define OLD_firstAddressField			OLD_name
#define OLD_firstPhoneField				OLD_phone1
#define OLD_lastPhoneField				OLD_phone5
#define OLD_numPhoneFields				(OLD_lastPhoneField - OLD_firstPhoneField + 1)
#define OLD_numPhoneLabelsStoredFirst	OLD_numPhoneFields
#define OLD_numPhoneLabelsStoredSecond	(OLD_numPhoneLabels - OLD_numPhoneLabelsStoredFirst)

#define OLD_firstRenameableLabel		OLD_custom1
#define OLD_lastRenameableLabel			OLD_custom4
#define OLD_lastLabel					(OLD_addressFieldsCount + OLD_numPhoneLabelsStoredSecond)

#define OLD_IsPhoneLookupField(p)		(OLD_addrLookupWork <= (p) && (p) <= OLD_addrLookupMobile)

#define OLD_addrLabelLength				16


/***********************************************************************
 *   Internal Structures
 ***********************************************************************/

typedef union
{
	struct
	{
		unsigned OLD_reserved	:13;
		unsigned OLD_note		:1;	// set if record contains a note handle
		unsigned OLD_custom4	:1;	// set if record contains a custom4
		unsigned OLD_custom3	:1;	// set if record contains a custom3
		unsigned OLD_custom2	:1;	// set if record contains a custom2
		unsigned OLD_custom1	:1;	// set if record contains a custom1
		unsigned OLD_title		:1;	// set if record contains a title
		unsigned OLD_country	:1;	// set if record contains a birthday
		unsigned OLD_zipCode	:1;	// set if record contains a birthday
		unsigned OLD_state		:1;	// set if record contains a birthday
		unsigned OLD_city		:1;	// set if record contains a birthday
		unsigned OLD_address	:1;	// set if record contains a address
		unsigned OLD_phone5		:1;	// set if record contains a phone5
		unsigned OLD_phone4		:1;	// set if record contains a phone4
		unsigned OLD_phone3		:1;	// set if record contains a phone3
		unsigned OLD_phone2		:1;	// set if record contains a phone2
		unsigned OLD_phone1		:1;	// set if record contains a phone1
		unsigned OLD_company	:1;	// set if record contains a company
		unsigned OLD_firstName	:1;	// set if record contains a firstName
		unsigned OLD_name		:1;	// set if record contains a name (bit 0)

	} bits;
	UInt32 allBits;
} OLD_AddrDBRecordFlags;

typedef union
{
	struct
	{
		unsigned OLD_reserved	 :10;
		unsigned OLD_phone8      :1;	// set if phone8 label is dirty
		unsigned OLD_phone7      :1;	// set if phone7 label is dirty
		unsigned OLD_phone6      :1;	// set if phone6 label is dirty
		unsigned OLD_note        :1;	// set if note label is dirty
		unsigned OLD_custom4     :1;	// set if custom4 label is dirty
		unsigned OLD_custom3     :1;	// set if custom3 label is dirty
		unsigned OLD_custom2     :1;	// set if custom2 label is dirty
		unsigned OLD_custom1     :1;	// set if custom1 label is dirty
		unsigned OLD_title       :1;	// set if title label is dirty
		unsigned OLD_country	 :1;	// set if country label is dirty
		unsigned OLD_zipCode	 :1;	// set if zipCode label is dirty
		unsigned OLD_state		 :1;	// set if state label is dirty
		unsigned OLD_city		 :1;	// set if city label is dirty
		unsigned OLD_address     :1;	// set if address label is dirty
		unsigned OLD_phone5      :1;	// set if phone5 label is dirty
		unsigned OLD_phone4      :1;	// set if phone4 label is dirty
		unsigned OLD_phone3      :1;	// set if phone3 label is dirty
		unsigned OLD_phone2      :1;	// set if phone2 label is dirty
		unsigned OLD_phone1      :1;	// set if phone1 label is dirty
		unsigned OLD_company     :1;	// set if company label is dirty
		unsigned OLD_firstName   :1;	// set if firstName label is dirty
		unsigned OLD_name        :1;	// set if name label is dirty (bit 0)

	} bits;
	UInt32 allBits;
} OLD_AddrDBFieldLabelsDirtyFlags;


typedef struct
{
	unsigned OLD_reserved:7;
	unsigned OLD_sortByCompany	:1;
} OLD_AddrDBMisc;

typedef enum
{
	OLD_name,
	OLD_firstName,
	OLD_company,
	OLD_phone1,
	OLD_phone2,
	OLD_phone3,
	OLD_phone4,
	OLD_phone5,
	OLD_address,
	OLD_city,
	OLD_state,
	OLD_zipCode,
	OLD_country,
	OLD_title,
	OLD_custom1,
	OLD_custom2,
	OLD_custom3,
	OLD_custom4,
	OLD_note,			// This field is assumed to be < 4K
	OLD_addressFieldsCount
} OLD_AddressFields;


// This structure is only for the exchange of address records.
typedef union
{
	struct
	{
		unsigned OLD_reserved		:8;

		// Typically only one of these are set
		unsigned OLD_email			:1;	// set if data is an email address
		unsigned OLD_fax			:1;	// set if data is a fax
		unsigned OLD_pager			:1;	// set if data is a pager
		unsigned OLD_voice			:1;	// set if data is a phone

		unsigned OLD_mobile			:1;	// set if data is a mobile phone

		// These are set in addition to other flags.
		unsigned OLD_work			:1;	// set if phone is at work
		unsigned OLD_home			:1;	// set if phone is at home

		// Set if this number is preferred over others.  May be preferred
		// over all others.  May be preferred over other emails.  One
		// preferred number should be listed next to the person's name.
		unsigned OLD_preferred   	:1;	// set if this phone is preferred (bit 0)
	} bits;
	UInt32 allBits;
} OLD_AddrDBPhoneFlags;

typedef enum
{
	OLD_workLabel,
	OLD_homeLabel,
	OLD_faxLabel,
	OLD_otherLabel,
	OLD_emailLabel,
	OLD_mainLabel,
	OLD_pagerLabel,
	OLD_mobileLabel
} OLD_AddressPhoneLabels;


typedef union
{
	struct
	{
		unsigned OLD_reserved:8;
		unsigned OLD_displayPhoneForList:4;	// The phone displayed for the list view 0 - 4
		unsigned OLD_phone5:4;				// Which phone (home, work, car, ...)
		unsigned OLD_phone4:4;
		unsigned OLD_phone3:4;
		unsigned OLD_phone2:4;
		unsigned OLD_phone1:4;
	} phones;
	UInt32 phoneBits;
} OLD_AddrOptionsType;



// AddrDBRecord.
//
// This is the unpacked record form as used by the app.  Pointers are
// either NULL or point to strings elsewhere on the card.  All strings
// are null character terminated.

typedef struct
{
	OLD_AddrOptionsType	options;        // Display by company or by name
	Char *			fields[OLD_addressFieldsCount];
} OLD_AddrDBRecordType;

typedef OLD_AddrDBRecordType *OLD_AddrDBRecordPtr;


// The labels for phone fields are stored specially.  Each phone field
// can use one of eight labels.  Part of those eight labels are stored
// where the phone field labels are.  The remainder (phoneLabelsStoredAtEnd)
// are stored after the labels for all the fields.

typedef char OLD_addressLabel[OLD_addrLabelLength];

typedef struct
{
	UInt16				OLD_renamedCategories;	// bitfield of categories with a different name
	char 					OLD_categoryLabels[dmRecNumCategories][dmCategoryLength];
	UInt8 				OLD_categoryUniqIDs[dmRecNumCategories];
	UInt8					OLD_lastUniqID;	// Uniq IDs generated by the device are between
	// 0 - 127.  Those from the PC are 128 - 255.
	UInt8					OLD_reserved1;	// from the compiler word aligning things
	UInt16				OLD_reserved2;
	OLD_AddrDBFieldLabelsDirtyFlags OLD_dirtyFieldLabels;
	OLD_addressLabel 		OLD_fieldLabels[OLD_addrNumFields + OLD_numPhoneLabelsStoredSecond];
	CountryType 		OLD_country;		// Country the database (labels) is formatted for
	UInt8 				OLD_reserved;
	OLD_AddrDBMisc			OLD_misc;
} OLD_AddrAppInfoType;

typedef OLD_AddrAppInfoType *OLD_AddrAppInfoPtr;


/************************************************************
 * Function Prototypes
 *************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

Err				OLD_AddrDBGetRecord(DmOpenRef dbP, UInt16 index, OLD_AddrDBRecordPtr recordP, MemHandle *recordH);

#ifdef __cplusplus
}
#endif

#endif // OLD_ADDRESSDB_H
