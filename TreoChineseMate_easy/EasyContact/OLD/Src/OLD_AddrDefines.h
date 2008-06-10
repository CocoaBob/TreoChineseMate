/******************************************************************************
 *
 * Copyright (c) 1995-2002 PalmSource, Inc. All rights reserved.
 *
 * File: AddrDefines.h
 *
 * Release: Palm OS 5 SDK (68K) R2.
 *
 *****************************************************************************/

#ifndef OLD_ADDRDEFINES_H
#define OLD_ADDRDEFINES_H

#include <Event.h>


// Update codes, used to determine how the address list view should
// be redrawn.
#define OLD_updateRedrawAll						0x01
#define OLD_updateGrabFocus						0x02
#define OLD_updateItemHide						0x04
#define OLD_updateCategoryChanged				0x08
#define OLD_updateFontChanged					0x10
#define OLD_updateListViewPhoneChanged			0x20
#define OLD_updateCustomFieldLabelChanged		0x40
#define OLD_updateSelectCurrentRecord			0x80

// AutoFill database types and names
// Note that we prefix with "Address" to avoid name conflicts with Expense app
#define OLD_titleDBType							'titl'
#define OLD_titleDBName							"AddressTitlesDB"

#define OLD_companyDBType						'cmpy'
#define OLD_companyDBName						"AddressCompaniesDB"

#define OLD_cityDBType							'city'
#define OLD_cityDBName							"AddressCitiesDB"

#define OLD_stateDBType							'stat'
#define OLD_stateDBName							"AddressStatesDB"

#define OLD_countryDBType						'cnty'
#define OLD_countryDBName						"AddressCountriesDB"

#define OLD_addrDBName							"AddressDB"
#define OLD_addrDBType							'DATA'

#define OLD_shortenedFieldString				"..."
#define OLD_shortenedFieldLength				3
#define OLD_fieldSeparatorString				", "
#define OLD_fieldSeparatorLength				2
#define OLD_spaceBetweenNamesAndPhoneNumbers	6

#define OLD_editFirstFieldIndex					0

#define	OLD_kDialListShowInListPhoneIndex		((UInt16)(-1))

#define OLD_addrNumFields						19
#define OLD_numPhoneLabels						8

#define OLD_noRecord							0xffff

#define OLD_GetPhoneLabel(r, p)					(((r)->options.phoneBits >> (((p) - OLD_firstPhoneField) << 2)) & 0xF)

#define OLD_SetPhoneLabel(r, p, pl)				((r)->options.phoneBits = \
											((r)->options.phoneBits & ~((UInt32) 0x0000000F << (((p) - OLD_firstPhoneField) << 2))) | \
											((UInt32) pl << (((p) - OLD_firstPhoneField) << 2)))

#define OLD_kFrmCustomUpdateEvent				OLD_firstUserEvent


#endif	// ADDRDEFINES_H
