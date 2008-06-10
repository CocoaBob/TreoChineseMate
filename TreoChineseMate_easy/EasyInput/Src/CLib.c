#include "CLib.h"

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
Err CLibLoad(UInt16 *refNumP, UInt32 *clientContextP)
{
	UInt16	error = CNLB_RUNTIME_ERROR;
	
	if (SysLibLoad(CLibTypeID, CLibCreatorID, refNumP) == errNone)
	{
		error = CLibOpen((*refNumP), clientContextP);
	}
	
	return error;
}

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
Err CLibRemove(UInt16 refNum, UInt32 clientContext)
{
	UInt16	error = CNLB_RUNTIME_ERROR;
	
	if (CLibClose(refNum, clientContext) == errNone)
	{
		error = SysLibRemove(refNum);
	}
	
	return error;
}

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
Err initCNLBGlobeParameter(CNLBGlobeType **globe, UInt8 addressType)
{	
	//Allocate globe variable
	(*globe) = (CNLBGlobeType *)MemPtrNew(sizeof(CNLBGlobeType));
	if ((*globe) != NULL)
	{
		MemSet((*globe), sizeof(CNLBGlobeType), 0x00);
		
		//Get the PY table
		if (FtrGet(CLibCreatorID, PY_TABLE_FTR_NUMBER, (UInt32 *)&((*globe)->pyTableP)) == errNone)
		{
			//Open Contact database
			if (addressType == ADDRESS_VERSION_NEW)
			{
				(*globe)->addressDbRef = DmOpenDatabaseByTypeCreator('DATA', 'PAdd', dmModeReadOnly);
			}
			else
			{
				(*globe)->addressDbRef = DmOpenDatabaseByTypeCreator('DATA', 'addr', dmModeReadOnly);
			}
			if (DmGetLastErr())
			{
				MemPtrFree((*globe));
				return CNLB_DB_NOT_FOUND_ERROR;
			}
		}
		else
		{
			MemPtrFree((*globe));
			return CNLB_RUNTIME_ERROR;
		}
	}
	else
	{
		return CNLB_RUNTIME_ERROR;
	}
	
	return errNone;
}

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
Err constructPYKeyword(CNLBParameterType *param, Char *keyword)
{
	UInt16		i = 0;
	
	if (param)
	{
		if (*keyword)
		{
			//Loop through the keyword
			while (*keyword && i < 10)
			{
				//Get the bit field of current character
				param->pyKeyword[i] = (((UInt32)1) << ((*keyword) - 97));
				i ++;
				keyword ++;
			}
			param->keywordLength = i;
		}
		else
		{
			param->pyKeyword[0] = 0;
			param->keywordLength = 1;
		}
	}
	else
	{
		return CNLB_RUNTIME_ERROR;
	}
	
	return errNone;
}