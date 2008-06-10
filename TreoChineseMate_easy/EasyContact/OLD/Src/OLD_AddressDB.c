/******************************************************************************
 *
 * Copyright (c) 1995-2002 PalmSource, Inc. All rights reserved.
 *
 * File: AddressDB.c
 *
 * Release: Palm OS 5 SDK (68K) R2.
 *
 * Description:
 *      Address Manager routines
 *
 *****************************************************************************/

#include "OLD_AddressDB.h"
//#include "AddrTools.h"
//#include "AddressRsc.h"
#include "OLD_AddrDefines.h"

#include <UIResources.h>
#include <SysUtils.h>
#include <ErrorMgr.h>
#include <StringMgr.h>
#include <TextMgr.h>
#include <PalmUtils.h>

#include <FeatureMgr.h>
#include "AddressSortLib.h"
/***********************************************************************
 *
 *   Defines
 *
 ***********************************************************************/

// Max length of a field name found in the FieldNamesStrList string list.
#define OLD_maxFieldName		31

#define OLD_LocalizedAppInfoStr	1000

// Extract the bit at position index from bitfield.  0 is the high bit.
#define OLD_BitAtPosition(pos)                ((UInt32)1 << (pos))
#define OLD_GetBitMacro(bitfield, index)      ((bitfield) & OLD_BitAtPosition(index))
#define OLD_SetBitMacro(bitfield, index)      ((bitfield) |= OLD_BitAtPosition(index))
#define OLD_RemoveBitMacro(bitfield, index)   ((bitfield) &= ~OLD_BitAtPosition(index))

#define OLD_sortKeyFieldBits   (OLD_BitAtPosition(OLD_name) | OLD_BitAtPosition(OLD_firstName) | OLD_BitAtPosition(OLD_company))

// Indexes into FieldNamesStrList string list.
enum {
	OLD_fieldNameStrListCity = 0,
	OLD_fieldNameStrListState,
	OLD_fieldNameStrListZip
};

// The following structure doesn't really exist.  The first field
// varies depending on the data present.  However, it is convient
// (and less error prone) to use when accessing the other information.
typedef struct {
	OLD_AddrOptionsType		options;        // Display by company or by name
	OLD_AddrDBRecordFlags	flags;
	UInt8				companyFieldOffset;   // Offset from firstField
	char				firstField;
} OLD_PrvAddrPackedDBRecord;


/***********************************************************************
 *
 *	Internal Functions
 *
 ***********************************************************************/
static void		OLD_PrvAddrDBUnpack(OLD_PrvAddrPackedDBRecord *src, OLD_AddrDBRecordPtr dest);

void OLD_PrvAddrDBUnpack(OLD_PrvAddrPackedDBRecord *src, OLD_AddrDBRecordPtr dest)
{
	Int16   index;
	UInt32 flags;
	char *p;


	dest->options = src->options;
	flags = src->flags.allBits;
	p = &src->firstField;


	for (index = OLD_firstAddressField; index < OLD_addressFieldsCount; index++)
	{
		// If the flag is set point to the string else NULL
		if (OLD_GetBitMacro(flags, index) != 0)
		{
			dest->fields[index] = p;
			p += StrLen(p) + 1;
		}
		else
			dest->fields[index] = NULL;
	}
}

Err OLD_AddrDBGetRecord(DmOpenRef dbP, UInt16 index, OLD_AddrDBRecordPtr recordP, MemHandle *recordH)
{
	OLD_PrvAddrPackedDBRecord *src;

	*recordH = DmQueryRecord(dbP, index);
	src = (OLD_PrvAddrPackedDBRecord *) MemHandleLock(*recordH);
	if (src == NULL)
		return dmErrIndexOutOfRange;

	OLD_PrvAddrDBUnpack(src, recordP);

	return 0;
}
