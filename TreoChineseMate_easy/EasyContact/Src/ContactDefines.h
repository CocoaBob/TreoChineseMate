#ifndef CONTACTDEFINES_H_
#define CONTACTDEFINES_H_
////////////////////////////////////////////////////////////////////////////////////
//
// 这个头文件是用Palm提供的通讯录库构造的，主要用于提供数据结构和一些宏。如果需要详
// 细的说明，请查阅Palm的通讯录库文档。
//
////////////////////////////////////////////////////////////////////////////////////
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
////////////////////////////////////////////////////////////////////////////////////

#define addrDBName          "ContactsDB-PAdd"   /**< Contacts DB Name */
#define addrDBType          'DATA'              /**< Contacts DB Type */
#define addrFileCAddress    'PAdd'              /**< Contacts DB Creator ID */
/** AutoFill database for the title field. */
/*@{*/
#define titleDBType                         'titl'
#define titleDBName                         "AddressTitlesDB"
/*@}*/

/** AutoFill database for the company field. */
/*@{*/
#define companyDBType                       'cmpy'
#define companyDBName                       "AddressCompaniesDB"
/*@}*/

/** AutoFill database for the city field. */
/*@{*/
#define cityDBType                          'city'
#define cityDBName                          "AddressCitiesDB"
/*@}*/

/** AutoFill database for the state field. */
/*@{*/
#define stateDBType                         'stat'
#define stateDBName                         "AddressStatesDB"
/*@}*/

/** AutoFill database for the country field. */
/*@{*/
#define countryDBType                       'cnty'
#define countryDBName                       "AddressCountriesDB"
/*@}*/

/***********************************************************************
 *  Update codes.
 *  These codes are used to determine how the List View should be
 *  redrawn on a frmUpdateEvent.
 ***********************************************************************/
/**
 * @name List View Update Codes
 */
/*@{*/
#define updateRedrawAll                     0x01
#define updateGrabFocus                     0x02
#define updateItemHide                      0x04
#define updateCategoryChanged               0x08
#define updateFontChanged                   0x0100
#define updateListViewPhoneChanged          0x20
#define updateCustomFieldLabelChanged       0x40
#define updateSelectCurrentRecord           0x80
/*@}*/

/** Special update event sent when the custom field labels are renamed. */
#define kFrmCustomUpdateEvent               firstUserEvent

/***********************************************************************
 *  Common field string literals.
 ***********************************************************************/

/**
 * @name Common String Literals and Constants
 */
/*@{*/
#define shortenedFieldString                "..."
#define shortenedFieldLength                3
#define fieldSeparatorString                ", "
#define fieldSeparatorLength                2
#define spaceBetweenNamesAndPhoneNumbers    6
#define editFirstFieldIndex                 0
#define kDialListShowInListPhoneIndex       ((UInt16)(-1))
/*@}*/

/***********************************************************************
 *  Field and field label numbers.
 ***********************************************************************/

#define numPhoneLabels          8 /**< Number of phone labels */
#define numAddressLabels        3 /**< Number of address labels */
#define numChatLabels           5 /**< Number of chat labels */

/** Total number of fields related to birthday */
#define numBirthdayFields       3

/** Total number of fields in Contacts application */
#define addrNumFields           43

/** Number of fields that are non-string */
#define addrNumNonStringFields  4

/** Number of fields that will show up in UI */
#define addrNumDisplayFields    addrNumFields - (numBirthdayFields -1)

/** Number of fields that are of type string */
#define addrNumStringFields     addrNumFields - addrNumNonStringFields

/***********************************************************************
 *  Miscellaneous.
 ***********************************************************************/

/** Record index value indicating no record in the database. */
#define noRecord                    0xffff

/** Width of the field label popup trigger arrow. */
#define popupArrowSpacing           12

/** @brief Default phone field to show in List View.
 *
 *  Current default is the mobile field. This constant is not the
 *  mobileLabel constant.  It is actually the phone field which has
 *  mobile as the defualt label when a new record is created.
 */
#define defaultShowInList           2

/** Number of possible address fields (address fields meaning as blocks of
 *  street, city, etc.) e.g.: address, address2, address3
 */
#define deliveryAddressCount        3

/** Number of address sub-fields (sub-fields meaning address, city, state,
 *  zipCode, country.)
 */
#define deliveryAddressFieldCount   5

typedef enum
{
    name,
    firstName,
    company,
    title,

    phone1,         /**< Phone fields. */
    phone2,
    phone3,
    phone4,
    phone5,
    phone6,
    phone7,

    chat1,          /**< Instant message id and service. */
    chat2,

    webpage,

    custom1,        /**< Custom fields. */
    custom2,
    custom3,
    custom4,
    custom5,
    custom6,
    custom7,
    custom8,
    custom9,

    address,        /**< Address fields. */
    city,
    state,
    zipCode,
    country,
    address2,
    city2,
    state2,
    zipCode2,
    country2,
    address3,
    city3,
    state3,
    zipCode3,
    country3,

    note,           /**< The note field must not be more than ~32K. */

    birthdayDate,   /**< Birthday info. */
    birthdayMask,   /**< Holds AddressDBBirthdayFlags type. */
    birthdayPreset,


    /***************************************************************************
     * All fields starting from picture will be added as blob
     * Each blob is: 4 bytes id, 2 bytes length and data
     * App blobs will have reserved id (1 to 64k) to keep them separate from
     * third party blobs.
     **************************************************************************/

    pictureInfo,    /**< holds picture blob: id --> 1, data  -->  2 bytes
    picture dirty flag, picture data */

    addressFieldsCount

} AddressFields;

#define firstAddressField           name    /**< First index of address field */
#define firstPhoneField             phone1  /**< First index of phone field */
#define lastPhoneField              phone7  /**< Last index of phone field */

/** Total number of phone fields */
#define numPhoneFields              (lastPhoneField - firstPhoneField + 1)

#define firstChatField              chat1   /**< First index of chat field */
#define lastChatField               chat2   /**< Last index of chat field */

#define firstWebField               webpage /**< First index of web field */
#define lastWebField                webpage /**< Last index of web field */

/**
 *  Field label sub-indices relating to the position of the field
 *  label within a popup list of labels.
 *  e.g.: the phone label popup list first selection is "Work", the
 *        second is "Home", etc.
 **/

typedef enum
{
    workLabel,
    homeLabel,
    faxLabel,
    otherLabel,
    emailLabel,
    mainLabel,
    pagerLabel,
    mobileLabel
} AddressPhoneLabels;

/**
 *  Field label sub-indices relating to the position of the field
 *  label within a popup list of labels.
 *  e.g.: the phone label popup list first selection is "Work", the
 *        second is "Home", etc.
 **/

typedef enum
{
    otherChatLabel,
    aimChatLabel,
    msnChatLabel,
    yahooChatLabel,
    icqChatLabel

} AddressChatLabels;

/**
 *  Field label sub-indices relating to the position of the field
 *  label within a popup list of labels.
 *  e.g.: the phone label popup list first selection is "Work", the
 *        second is "Home", etc.
 **/

typedef enum
{
    workAddressLabel,
    homeAddressLabel,
    otherAddressLabel
} AddressStreetAddressLabels;

/** Maximum length of a field label, excluding NULL terminator: */
#define addrLabelLength             16

/** Phone labels: */
#define numPhoneLabelsStoredFirst   numPhoneFields
#define numPhoneLabelsStoredSecond  (numPhoneLabels - numPhoneLabelsStoredFirst)

/** Labels preceding the address field groups (work, home, other): */
/*@{*/
#define Addr1FieldGroupLabel        phone1
#define Addr2FieldGroupLabel        phone2
#define Addr3FieldGroupLabel        phone4
/*@}*/

/** The first and last custom/renameable labels: */
/*@{*/
#define firstRenameableLabel        custom1
#define lastRenameableLabel         custom9
/*@}*/

/** Last label in the first set of labels: */
#define lastLabel                   addrNumDisplayFields

/** Indices to start of second set of phone, address and chat labels: */
/*@{*/
#define phoneLabelSecondStart       addrNumDisplayFields -1
#define chatLabelstart              addrNumDisplayFields
#define webLabelStart               13
/*@}*/

/** Labels for the "Add field" list of fields: */
/*@{*/
#define numAddFieldLabels           3
#define AddFieldLabelStart          addrNumDisplayFields            \
                                    + numPhoneLabelsStoredSecond    \
                                    + numChatLabels
/*@}*/

/** Total field label count: */
/*@{*/
#define numPictureFieldLabels       3
#define numFieldLabels              addrNumDisplayFields            \
                                    + numPhoneLabelsStoredSecond    \
                                    + numChatLabels                 \
                                    + numAddFieldLabels             \
                                    + numPictureFieldLabels
/*@}*/

/***********************************************************************
 *  Application Packed Record Format Related Data
 *
 *  Note: Records are stored in the database in packed format to
 *        conserve space.  When retrieving a record from the database,
 *        it is unpacked into the AddrDBRecordType.  AddrDBGetRecord()
 *        does the necessary unpacking for you.  When creating a new
 *        record, AddrDBNewRecord(), or saving a record, AddrDBChangeRecord(),
 *        the packing is handled for you.
 ***********************************************************************/

/** @brief Packed record flags.
 *
 * AddrDBRecordFlags indicates the address record bits.  It is used
 * to indicate field changes or fields contained in a packed record.
 * Since there is no 64-bit bitfield, a struct with two unsigned
 * longs is used. Please be sure to use the bit macros (defined
 * later below) to extract the bits, as the bit order may change in
 * future.
 */
typedef struct
{
    union
    {
        struct
        {
            UInt32 reserved     :4;

            UInt32 country      :1; /**< Set if record contains a country. */
            UInt32 zipCode      :1; /**< Set if record contains a zipCode. */
            UInt32 state        :1; /**< Set if record contains a state. */
            UInt32 city         :1; /**< Set if record contains a city. */
            UInt32 address      :1; /**< Set if record contains a address. */

            UInt32 custom9      :1; /**< Set if record contains a custom9. */
            UInt32 custom8      :1; /**< Set if record contains a custom8. */
            UInt32 custom7      :1; /**< Set if record contains a custom7. */
            UInt32 custom6      :1; /**< Set if record contains a custom6. */
            UInt32 custom5      :1; /**< Set if record contains a custom5. */
            UInt32 custom4      :1; /**< Set if record contains a custom4. */
            UInt32 custom3      :1; /**< Set if record contains a custom3. */
            UInt32 custom2      :1; /**< Set if record contains a custom2. */
            UInt32 custom1      :1; /**< Set if record contains a custom1. */

            UInt32 webpage      :1; /**< Set if record contains a URL. */

            UInt32 chat2        :1; /**< Set if record contains an instant
                                         message id 2. */
            UInt32 chat1        :1; /**< Set if record contains an instant
                                         message id 1. */

            UInt32 phone7       :1; /**< Set if record contains a phone7. */
            UInt32 phone6       :1; /**< Set if record contains a phone6. */
            UInt32 phone5       :1; /**< Set if record contains a phone5. */
            UInt32 phone4       :1; /**< Set if record contains a phone4. */
            UInt32 phone3       :1; /**< Set if record contains a phone3. */
            UInt32 phone2       :1; /**< Set if record contains a phone2. */
            UInt32 phone1       :1; /**< Set if record contains a phone1. */
            UInt32 title        :1; /**< Set if record contains a title. */
            UInt32 company      :1; /**< Set if record contains a company. */
            UInt32 firstName    :1; /**< Set if record contains a firstName. */
            UInt32 name         :1; /**< Set if record contains a name (bit 0).
            */

        } bits;
        UInt32 allBits;
    };
    union
    {
        struct
        {
            UInt32 reserved2        :6;
            UInt32 reserved         :12;

            UInt32 birthdayPreset   :1; /**< Set if record contains birthday
                                             preset. */
            UInt32 birthdayMask     :1; /**< Set if record contains birthday
                                             mask. */
            UInt32 birthdayDate     :1; /**< Set if record contains birthday
                                             date. */

            UInt32 note             :1; /**< Set if record contains a note
                                             handle. */

            UInt32 country3         :1; /**< Set if record contains a country3.
                                             */
            UInt32 zipCode3         :1; /**< Set if record contains a zipCode3.
                                             */
            UInt32 state3           :1; /**< Set if record contains a state3. */
            UInt32 city3            :1; /**< Set if record contains a city3. */
            UInt32 address3         :1; /**< Set if record contains a address3.
                                             */

            UInt32 country2         :1; /**< Set if record contains a country2.
                                             */
            UInt32 zipCode2         :1; /**< Set if record contains a zipCode2.
                                             */
            UInt32 state2           :1; /**< Set if record contains a state2. */
            UInt32 city2            :1; /**< Set if record contains a city2. */
            UInt32 address2         :1; /**< Set if record contains a address2.
                                             */


        } bits2;
        UInt32 allBits2;
    };
} AddrDBRecordFlags;

/***********************************************************************
 *  Application Unpacked Record Format Related Data
 ***********************************************************************/

/** @brief Label types.
 *
 * This describes the label types for those user selectable label
 * fields: phone, instant messenger, and address.
 */
typedef struct
{
    union
    {
        struct
        {
            unsigned displayPhoneForList:4; /**< The phone displayed for the
                                                 List View, corresponding to the
                                                 item in the AddressPhoneLabels
                                                 enum. */
            unsigned phone7:4;              /**< Which phone label (work, home,
                                                 fax, other, ...) for this phone
                                                 field, value is an
                                                 AddressPhoneLabels enum. */
            unsigned phone6:4;
            unsigned phone5:4;
            unsigned phone4:4;
            unsigned phone3:4;
            unsigned phone2:4;
            unsigned phone1:4;
        } phones;
        UInt32 phoneBits;
    };

    union
    {
        struct
        {
            unsigned int reserved : 4;
            unsigned int address3 : 4;  /**< Which address label (work, home,
                                             other) for this address field,
                                             value is an
                                             AddressStreetAddressLabels enum. */
            unsigned int address2 : 4;
            unsigned int address1 : 4;
        }addresses;
        UInt16 addressBits;
    };

    union
    {
        struct
        {
            unsigned int reserved:8;
            unsigned int chat2:4;       /**< Which chat label (other, aim, msn,
                                          ...) for this chat field, value is an
                                             AddressChatLabels enum. */
            unsigned int chat1:4;

        }chatIds;
        UInt16 chatBits;
    };

} AddrOptionsType;

/** @brief Birthday data flags
 *
 * On/off flags for birthday features.
 */
typedef struct
{
    unsigned short reserved     :15;
    unsigned short alarm        :1; /**< Indicates if a reminder is set for this
                                         birthday. */
}
AddressDBBirthdayFlags;

/** @brief Birthday format
 *
 * This is the format of the birthday data in the record.
 */
typedef struct
{
    DateType                birthdayDate;   /**< Contains birthdate. Year should
                                                 be considered if year mask it
                                                 set. */
    AddressDBBirthdayFlags  birthdayMask;
    UInt8                   birthdayPreset; /**< Number of days for advance
                                                 reminder. */
} BirthdayInfo;

/** @brief Picture info
 *
 * AddressDBPictureInfo is a temp struct to hold the picture data.Within the
 * address record picture is stored as blob data.
 */
typedef struct
{
    UInt16  pictureDirty;   /**< set when a picture is edited or added. */
    UInt16  pictureSize;    /**< size of the image in bytes */
    void*   pictureData;    /**< image data in jpeg format, limit is 4k */
} AddressDBPictureInfo;

/** Total number of blobs a record is allowed to have */
#define apptMaxBlobs                    10

/** @brief Format of each blob attached to a record. */
typedef struct
{
    UInt32                  creatorID;
    UInt16                  size;       /**< Excludes space used by blob ID
                                             and size. */
    void *                  content;    /**< Remainder of blob is content */
} BlobType;

typedef BlobType * BlobPtr;

/** @brief Record format
 *
 * This is the unpacked record format as used by the app.  Pointers are
 * either NULL or point to strings elsewhere on the card.  All strings
 * are NULL character terminated.
 */
typedef struct
{
    /** Display by company or by name */
    AddrOptionsType         options;

    /** Container for all fields of type string */
    Char *                  fields[addrNumStringFields];

    /** Container to hold birthday information */
    BirthdayInfo            birthdayInfo;

    /**
     * Container to hold picture information: dirty flags, followed
     * by size and the actual image */
    AddressDBPictureInfo    pictureInfo;

    /** Number of blobs the record has excluding the picture blob */
    UInt16                  numBlobs;

    /** Container to hold blobs and the metadata */
    BlobType                blobs[apptMaxBlobs];

} AddrDBRecordType;

typedef AddrDBRecordType *AddrDBRecordPtr;

/***********************************************************************
 *  Contacts Application Info Block Related Data
 ***********************************************************************/

/** Record field label string. */
typedef char addressLabel[addrLabelLength];

/** @brief Field label state
 *
 * This is used to indicate changes to record field labels. e.g.:
 * Flags are set when copying the localized field labels into the
 * fieldLabels array of the AddrAppInfoType.
 */
typedef struct
{
    union
    {
        struct
        {
            UInt32 reserved     :4;

            UInt32 country      :1; /**< Set if country label is dirty. */
            UInt32 zipCode      :1; /**< Set if zipcode label is dirty. */
            UInt32 state        :1; /**< Set if state label is dirty. */
            UInt32 city         :1; /**< Set if city label is dirty. */
            UInt32 address      :1; /**< Set if address label is dirty. */

            UInt32 custom9      :1; /**< Set if custom/renameable label is
            dirty. */
            UInt32 custom8      :1;
            UInt32 custom7      :1;
            UInt32 custom6      :1;
            UInt32 custom5      :1;
            UInt32 custom4      :1;
            UInt32 custom3      :1;
            UInt32 custom2      :1;
            UInt32 custom1      :1;

            UInt32 webpage      :1; /**< Set if webpage label is dirty. */

            UInt32 chat2        :1; /**< Set if instant message id is dirty. */
            UInt32 chat1        :1;

            UInt32 phone7       :1; /**< Set if phone label is dirty. */
            UInt32 phone6       :1;
            UInt32 phone5       :1;
            UInt32 phone4       :1;
            UInt32 phone3       :1;
            UInt32 phone2       :1;
            UInt32 phone1       :1;
            UInt32 title        :1; /**< Set if title label is dirty. */
            UInt32 company      :1; /**< Set if company label is dirty. */
            UInt32 firstName    :1; /**< Set if firstName label is dirty. */
            UInt32 name         :1; /**< Set if name label is dirty (bit 0). */

        } bits;
        UInt32 allBits;
    };
    union
    {
        struct
        {
            UInt32 reserved1        :12;
            UInt32 reserved2        :7;

            UInt32 pictureInfo      :1; /**< Set if picture label is dirty. */

            UInt32 birthdayDate     :1; /**< Set if birthday label is dirty. */

            UInt32 note             :1; /**< Set if note field is dirty. */

            UInt32 country3         :1; /**< Set if address fields are dirty. */
            UInt32 zipCode3         :1;
            UInt32 state3           :1;
            UInt32 city3            :1;
            UInt32 address3         :1;

            UInt32 country2         :1;
            UInt32 zipCode2         :1;
            UInt32 state2           :1;
            UInt32 city2            :1;
            UInt32 address2         :1;

        } bits2;
        UInt32 allBits2;
    };
} AddrDBFieldLabelsDirtyFlags;

/** @brief Miscellaneous application data
 *
 * This includes any miscellaneous application wide data.
 */
typedef struct
{
    unsigned reserved:7;
    unsigned sortByCompany  :1;
} AddrDBMisc;

/** @brief Application Info Block
 *
 * This structure is used to store info applicable to all records
 * in the database, specific to the application, inter-session (like
 * preferences), etc.
 *
 * Note: The phone field labels are stored specially.  Each phone
 *       field can use one of eight labels.  Some of those eight
 *       labels are stored as the phone field labels.  The remainder
 *       (phoneLabelsStoredAtEnd) are stored after the labels for
 *       all the fields.
 */
typedef struct
{
    /** Bitfield of categories with a different name. */
    UInt16 renamedCategories;

    char   categoryLabels[dmRecNumCategories][dmCategoryLength];

    UInt8  categoryUniqIDs[dmRecNumCategories];

    /**
     * Unique IDs generated by the device are between 0 - 127.
     * Those from the PC are 128 - 255.
     */
    UInt8  lastUniqID;

    /** From the compiler word aligning things. */
    UInt8  reserved1;

    /**
     * Whether category colors were edited since last sync.
     * Least significant bit first.
     */
    UInt16 categoryColorsEdited;

    /**
     * Added as part of the Mullet version of this application,
     * so that we can later support color categories without
     * breaking the conduits.
     */
    UInt8  categoryColors[dmRecNumCategories];

    UInt16 reserved2;

    /** Labels that have changed. */
    AddrDBFieldLabelsDirtyFlags dirtyFieldLabels;

    /** Labels displayed for record fields. */
    addressLabel                fieldLabels[numFieldLabels];

    /** Country for which the database (labels) is formatted. */
    CountryType                 country;

    UInt8                       reserved;

    AddrDBMisc                  misc;

} AddrAppInfoType;

typedef AddrAppInfoType *AddrAppInfoPtr;

/** Macro to determine whether a lookup field is for phone lookup */
#define IsPhoneLookupField(p)   (addrLookupWork <= (p) && (p) <= \
                                 addrLookupMobile)

#define firstRecordFlagBits2Field   address2

/**
 * Get a bit mask for the given bit position.
 *
 * The pos argument is used to indicate what bit position
 * should be set in the 4-byte bit mask, so a pos = 2 yields:
 * 0000 0000 0000 0010
 *
 * @param pos   Position of the bit to be set in the 4 byte
 *                  bit mask.
 * @return      A 4-byte bit mask.
 */
#define BitAtPosition(pos)  ((UInt32)1 <<                               \
                            ((pos < firstRecordFlagBits2Field) ?        \
                              pos:(pos - firstRecordFlagBits2Field)) )

/**
 * Get the record flag value for the given flag index.
 *
 * @param recFlags  A AddrDBRecordFlags struct.
 * @param index     The flag index. i.e.: the flag bit position.
 *
 * @return          The value of the record flag for the given flag index.
 */
#define GetBitMacro(recFlags, index)    ((index < firstRecordFlagBits2Field) ? \
                                        ((recFlags).allBits) & BitAtPosition(  \
                                          index):((recFlags).allBits2) &       \
                                          BitAtPosition(index))

/**
 * Set the record flag value at the given flag index.
 *
 * @param recFlags  A AddrDBRecordFlags struct.
 * @param index     The index of the flag to set. i.e.:
 *                      the bit position of the flag to set.
 *
 * @return          Nothing.
 */
#define SetBitMacro(recFlags, index)    if(index < firstRecordFlagBits2Field)\
                                          (recFlags).allBits |= BitAtPosition(index);  else \
                                          (recFlags).allBits2 |=  BitAtPosition(index)

/**
 * Clear the record flag value at the given flag index.
 * This clears or sets the flag value to zero (0).
 *
 * @param recFlags  A AddrDBRecordFlags struct.
 * @param index     The index of the flag to clear. i.e.:
 *                      the bit position of the flag to set.
 *
 * @retval          Nothing.
 */
#define RemoveBitMacro(recFlags, index) if(index < firstRecordFlagBits2Field)  \
                                        ((recFlags).allBits) &= ~BitAtPosition \
                                        (index); else                          \
                                        ((recFlags).allBits2) &=               \
                                        ~BitAtPosition(index)

#define SetBitMacroBits(bitField, index) (bitField |= BitAtPosition(index))

#define PictureFieldListStart   addrNumDisplayFields +          \
                                numPhoneLabelsStoredSecond +    \
                                numChatLabels +                 \
                                numAddFieldLabels

#define phoneLabelNone          -1

#define noPhoneField            0x0000

// <chg 20-sep-99 dia> These are phone fields that you might want to make
// a voice phone call to...
#define IsVoiceLabel(f) (((f) != P1ctFaxPhone) && ((f) != P1ctEmailPhone))

#define IsPhoneLookupField(p) (addrLookupWork <= (p) && (p) <= addrLookupMobile)

#define IsSMSLabel(f) (((f) == P1ctMobilePhone) || ((f) ==P1ctEmailPhone) ||   \
                       ((f) == P1ctOtherPhone))

#define IsSMSLabelNoEmail(f) (((f) == P1ctMobilePhone) ||   \
                              ((f) == P1ctOtherPhone))

/******************************************************************************
 * BLOB ID definition
 * Internal blob id will start from 'Bd00'.
 * Creator ids from 'Bd00 - 'Bd09' is registered for this purpose.
 ******************************************************************************/

/** picture blob id */
#define addrPictureBlobId                       'Bd00'

// Need this constant to hack the fix for a Mullet Outlook conduit bug, which is
// already shipped.
#define appInfoBlockSizeMullet  1092

#define kCallerIDMaxPhoneNumberDigits           63

// This is the number of digits in a phone number that are significant...
// Only check the last 8 chars to allow number to be stored in international
// format to still match if bufferLen is greater than key len just use the key
// length
#define kCallerIDSignificantPhoneNumberDigits   8

#define kStrLenRequiredForMatch                 6

// This structure is only for the exchange of address records.
typedef union
{
    struct
    {
        unsigned reserved       :8;

        // Typically only one of these are set

        unsigned email          :1; /**< set if data is an email address */
        unsigned fax            :1; /**< set if data is a fax */
        unsigned pager          :1; /**< set if data is a pager */
        unsigned voice          :1; /**< set if data is a phone */

        unsigned mobile         :1; /**< set if data is a mobile phone */

        // These are set in addition to other flags.

        unsigned work           :1; /**< set if phone is at work */
        unsigned home           :1; /**< set if phone is at home */

        // Set if this number is preferred over others.  May be preferred
        // over all others.  May be preferred over other emails.  One
        // preferred number should be listed next to the person's name.

        unsigned preferred      :1; /**< set if this phone is preferred (bit 0)
                                         */
    } bits;
    UInt32 allBits;
} AddrDBPhoneFlags;

typedef struct
{
    UInt16          currentCategory;
    FontID          v20NoteFont;                // For 2.0 compatibility (BGT)
    Boolean         showAllCategories;
    Boolean         saveBackup;
    Boolean         rememberLastCategory;

    // Version 3 preferences
    FontID          addrListFont;
    FontID          addrRecordFont;
    FontID          addrEditFont;
    UInt8           reserved1;
    UInt32          businessCardRecordID;
    FontID          noteFont;
    UInt8           reserved2;

    // Version 4 preferences
    Boolean         enableTapToConnect;

    // Version 5 preferences for wireless products
    Boolean         showSimCategory;

} AddrPreferenceType;

#define GetPhoneLabel(r, p) (((r)->options.phoneBits >> \
                            (((p) - firstPhoneField) << 2)) & 0xF)

#endif