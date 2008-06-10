/*
 * CLib.h
 * 通讯录检索库 V1.0
 *
 * 作者：REi.Laputa，2006年11月22日
 * 版权所有
 * http://rei.gtyz.org
 */

#ifndef CLIB_H_
#define CLIB_H_

#include <SystemMgr.h>
#include "ContactDefines.h"

/**************************************************************************************
 * 数据结构定义
 **************************************************************************************/

//调用CLibGetMatchContactByRecordId()时需提供的参数。
typedef struct
{
	UInt16					recordId;			//要获取的通讯记录的记录号
	UInt32					pyKeyword[10];		//用以比较的拼音首字母关键字，需调用
												//  constructPYKeyword()进行构造
	Int16					keywordLength;		//关键字的长度
	Boolean					orderlySearch;		//比较模式：
												//true  - 函数将严格从第一个字符开始
												//        比较，比如关键字"yd"可以找
												//        到"移动通信"，但找不到"中国移动"
												//false - 函数将找出所有包含关键字的
												//        结果。
} CNLBParameterType;

//调用CLibGetMatchContactByRecordId()时需提供的预初始化参数，本参数可以通过函数
//initCNLBGlobeParameter()进行初始化。
typedef struct
{
	Char					keyword[256];		//用于构造通讯记录查找信息的字符串缓存
	Int16					nextV[10];			//用于KMP算法的缓存
	UInt32					pyData[256];		//用于装载由keyword[]生成的拼音信息
	UInt16					pyDataLength;		//pyData[]中有效信息的长度
	DmOpenRef				addressDbRef;		//通讯录数据库的指针
	UInt32					*pyTableP;			//拼音转换表的指针
} CNLBGlobeType;

//调用CLibGetContacts_PAdd()后，返回的结果单元
/*typedef struct
{
	AddrDBRecordType		data;				//通讯记录，详见ContactDefines.h或Palm
												//提供的通讯录文档
	Char					fullName[256];		//完整的名字
	UInt16					fullNameLength;		//名字的长度
} CNLBContactType;

//调用CLibGetContacts_Addr()后，返回的结果单元，内容同上
typedef struct
{
	OLD_AddrDBRecordType	data;
	Char					fullName[256];		//完整的名字
	UInt16					fullNameLength;		//名字的长度
} OLD_CNLBContactType;*/

/**************************************************************************************
 * 宏定义
 **************************************************************************************/

#define CLIB_LIB_TRAP(trapNum)	SYS_TRAP(trapNum)

#define	CLibCreatorID			'CNLB'					//库创建者ID
#define	CLibTypeID				sysFileTLibrary			//库类型

#define PY_TABLE_FTR_NUMBER		0x01

#define CNLB_RUNTIME_ERROR		0x01					//运行时错误，通常指内存错误等不可恢复的错误
#define	CNLB_NOT_MATCH_ERROR	0x02					//不匹配错误，给定的拼音关键字与查找的记录不匹配
#define CNLB_DB_NOT_FOUND_ERROR	0x03					//拼音转换数据库未找到
#define CNLB_OPEN_CONTACT_ERROR	0x04					//通讯录未找到

#define SEARCH_MODE_FULLNAME	0x00					//全名检索模式
#define SEARCH_MODE_LASTNAME	0x01					//仅检索姓
#define SEARCH_MODE_FIRSTNAME	0x02					//仅检索名
#define SEARCH_MODE_PHONE		0x03					//号码检索模式
#define SEARCH_MODE_GET			0x04

#define ADDRESS_VERSION_OLD		0x00					//Treo600及以前的PalmOS中使用的数据库（AddressDB.pdb）
#define ADDRESS_VERSION_NEW		0x01					//Treo650及以后的PalmOS中使用的数据库（ContactDB-PAdd.pdb）

//用于操作拼音转换表的宏
#define PRONUNCIATION_COUNT		0x0FFFFFFF
#define PY_A					0x00000001
#define PY_B					0x00000002
#define PY_C					0x00000004
#define PY_D					0x00000008
#define PY_E					0x00000010
#define PY_F					0x00000020
#define PY_G					0x00000040
#define PY_H					0x00000080
#define PY_I					0x00000100
#define PY_J					0x00000200
#define PY_K					0x00000400
#define PY_L					0x00000800
#define PY_M					0x00001000
#define PY_N					0x00002000
#define PY_O					0x00004000
#define PY_P					0x00008000
#define PY_Q					0x00010000
#define PY_R					0x00020000
#define PY_S					0x00040000
#define PY_T					0x00080000
#define PY_U					0x00100000
#define PY_V					0x00200000
#define PY_W					0x00400000
#define PY_X					0x00800000
#define PY_Y					0x01000000
#define PY_Z					0x02000000

//刷新调用CLibGetMatchContactByRecordId()时需提供的预初始化参数，当循环调用
//CLibGetMatchContactByRecordId()时，每次调用完毕后都应该使用本宏
#define refreshCNLBGlobeParameter(n)	MemSet(n, 1302, 0x00)

/**************************************************************************************
 * 库函数定义
 **************************************************************************************/

/*
 * ClibLoad
 * 装载并初始化通讯录拼音查找库
 *
 * 参数：
 *	refNumP			<->	给予的UInt16指针，用于返回查找库的引用
 *	clientContextP	<->	给予的UInt32指针，用于返回clientContext
 *
 * 返回：
 *	当成功装载并初始化时，返回errNone；否则返回对应的错误代码
 */
extern Err CLibLoad(UInt16 *refNumP, UInt32 *clientContextP);

/*
 * ClibRemove
 * 关闭并卸载通讯录拼音查找库
 *
 * 参数：
 *	refNum			->	已经打开的查找库的引用
 *	clientContext	->	已经打开的查找库的Client Context
 *
 * 返回：
 *	当成功关闭并卸载库时，返回errNone；否则返回对应的错误代码
 */
extern Err CLibRemove(UInt16 refNum, UInt32 clientContext);

/*
 * initCNLBGlobeParameter
 * 初始化调用CLibGetMatchContactByRecordId()时需提供的预初始化参数。需要由调用者提供
 * 该参数的原因，是CLibGetMatchContactByRecordId()可能被用于循环检索多条记录（比如，
 * 由于查找库未提供获取特定分类下符合拼音关键字要求的通讯录集这种功能，调用者可能自行
 * 通过循环调用CLibGetMatchContactByRecordId()来实现该功能），提供该参数可以节省每次
 * 调用CLibGetMatchContactByRecordId()时，由库函数进行初始化所造成的时间开销和内存碎片
 *
 * 参数：
 *	globe			<->	给予的UInt16指针，用于返回查找库的引用
 *	addressType		->	通讯录的类型，ADDRESS_VERSION_OLD或ADDRESS_VERSION_NEW
 *
 * 返回：
 *	当成功初始化时，返回errNone；否则返回对应的错误代码
 */
extern Err initCNLBGlobeParameter(CNLBGlobeType **globe, UInt8 addressType);

/*
 * constructPYKeyword
 * 构造用于检索的UInt32拼音组，本函数用于初始化调用CLibGetMatchContactByRecordId()时
 * 需提供的参数CNLBParameterType中的拼音组
 *
 * 参数：
 *	param			<->	返回UInt32拼音组
 *	keyword			->	用于构造拼音组的拼音首字母字符串，注意所有拼音首字母必须小写
 *
 * 返回：
 *	当成功装载并初始化时，返回errNone；否则返回对应的错误代码
 */
extern Err constructPYKeyword(CNLBParameterType *param, Char *keyword);

//本函数不应该直接调用
extern Err CLibOpen(UInt16 refNum, UInt32 *clientContextP)
	CLIB_LIB_TRAP(sysLibTrapOpen);

//本函数不应该直接调用
extern Err CLibClose(UInt16 refNum, UInt32 clientContext)
	CLIB_LIB_TRAP(sysLibTrapClose);

extern Err CLibSleep(UInt16 refNum)
	CLIB_LIB_TRAP(sysLibTrapSleep);

extern Err CLibWake(UInt16 refNum)
	CLIB_LIB_TRAP(sysLibTrapWake);

/*
 * CLibGetMatchContactByRecordId_Addr
 * 检索给定位置的通讯记录是否与给定的拼音首字母匹配，若匹配，则返回该通讯记录（旧版通讯录）
 *
 * 参数：
 *	refNum			->	库引用
 *	globe			->	预初始化参数
 *	param			->	检索参数，包含拼音首字母组及要进行检索的通讯记录
 *	result			<->	返回检索到匹配的通讯记录
 *
 * 返回：
 *	当成功检索，且通讯记录匹配的时候，返回errNone；否则返回对应的错误代码
 */
extern Err CLibGetMatchContactByRecordId_Addr(UInt16 refNum, CNLBGlobeType *globe, CNLBParameterType *param, OLD_AddrDBRecordType *result, UInt8 mode)
	CLIB_LIB_TRAP(sysLibTrapBase + 5);

/*
 * CLibGetMatchContactByRecordId_PAdd
 * 检索给定位置的通讯记录是否与给定的拼音首字母匹配，若匹配，则返回该通讯记录（新版通讯录）
 *
 * 参数：
 *	refNum			->	库引用
 *	globe			->	预初始化参数
 *	param			->	检索参数，包含拼音首字母组及要进行检索的通讯记录
 *	result			<->	返回检索到匹配的通讯记录
 *
 * 返回：
 *	当成功检索，且通讯记录匹配的时候，返回errNone；否则返回对应的错误代码
 */
extern Err CLibGetMatchContactByRecordId_PAdd(UInt16 refNum, CNLBGlobeType *globe, CNLBParameterType *param, AddrDBRecordType *result, UInt8 mode)
	CLIB_LIB_TRAP(sysLibTrapBase + 6);

/*
 * CLibGetContacts_Addr
 * 从通讯录中返回符合给定拼音首字母关键字串的所有通讯记录
 * （旧版通讯录）
 *
 * 参数：
 *	refNum			->	库引用
 *	keyword			->	给定的拼音首字母关键字串
 *	orderlySearch	->	查找模式，详见"CNLBParameterType"中orderlySearch的说明
 *	result			->	用于返回符合检索要求的通讯记录
 *	contactNum		<->	返回符合检索条件的通讯记录的总数
 *
 * 返回：
 *	当成功检索时，返回errNone；否则返回对应的错误代码
 */
extern Err CLibGetContacts_Addr(UInt16 refNum, Char *keyword, Boolean orderlySearch, UInt8 mode, OLD_AddrDBRecordType ***result, UInt16 *contactNum)
	CLIB_LIB_TRAP(sysLibTrapBase + 7);

/*
 * CLibGetContacts_PAdd
 * 从通讯录中返回符合给定拼音首字母关键字串的所有通讯记录
 * （新版通讯录）
 *
 * 参数：
 *	refNum			->	库引用
 *	keyword			->	给定的拼音首字母关键字串
 *	orderlySearch	->	查找模式，详见"CNLBParameterType"中orderlySearch的说明
 *	result			->	用于返回符合检索要求的通讯记录
 *	contactNum		<->	返回符合检索条件的通讯记录的总数
 *
 * 返回：
 *	当成功检索时，返回errNone；否则返回对应的错误代码
 */
extern Err CLibGetContacts_PAdd(UInt16 refNum, Char *keyword, Boolean orderlySearch, UInt8 mode, AddrDBRecordType ***result, UInt16 *contactNum)
	CLIB_LIB_TRAP(sysLibTrapBase + 8);

#endif /* CLIB_H_ */
