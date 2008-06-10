#include "CLib.h"

/*
 * ClibLoad
 * װ�ز���ʼ��ͨѶ¼ƴ�����ҿ�
 *
 * ������
 *	refNumP			<->	�����UInt16ָ�룬���ڷ��ز��ҿ������
 *	clientContextP	<->	�����UInt32ָ�룬���ڷ���clientContext
 *
 * ���أ�
 *	���ɹ�װ�ز���ʼ��ʱ������errNone�����򷵻ض�Ӧ�Ĵ������
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
 * �رղ�ж��ͨѶ¼ƴ�����ҿ�
 *
 * ������
 *	refNum			->	�Ѿ��򿪵Ĳ��ҿ������
 *	clientContext	->	�Ѿ��򿪵Ĳ��ҿ��Client Context
 *
 * ���أ�
 *	���ɹ��رղ�ж�ؿ�ʱ������errNone�����򷵻ض�Ӧ�Ĵ������
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
 * ��ʼ������CLibGetMatchContactByRecordId()ʱ���ṩ��Ԥ��ʼ����������Ҫ�ɵ������ṩ
 * �ò�����ԭ����CLibGetMatchContactByRecordId()���ܱ�����ѭ������������¼�����磬
 * ���ڲ��ҿ�δ�ṩ��ȡ�ض������·���ƴ���ؼ���Ҫ���ͨѶ¼�����ֹ��ܣ������߿�������
 * ͨ��ѭ������CLibGetMatchContactByRecordId()��ʵ�ָù��ܣ����ṩ�ò������Խ�ʡÿ��
 * ����CLibGetMatchContactByRecordId()ʱ���ɿ⺯�����г�ʼ������ɵ�ʱ�俪�����ڴ���Ƭ
 *
 * ������
 *	globe			<->	�����UInt16ָ�룬���ڷ��ز��ҿ������
 *	addressType		->	ͨѶ¼�����ͣ�ADDRESS_VERSION_OLD��ADDRESS_VERSION_NEW
 *
 * ���أ�
 *	���ɹ���ʼ��ʱ������errNone�����򷵻ض�Ӧ�Ĵ������
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
 * �������ڼ�����UInt32ƴ���飬���������ڳ�ʼ������CLibGetMatchContactByRecordId()ʱ
 * ���ṩ�Ĳ���CNLBParameterType�е�ƴ����
 *
 * ������
 *	param			<->	����UInt32ƴ����
 *	keyword			->	���ڹ���ƴ�����ƴ������ĸ�ַ�����ע������ƴ������ĸ����Сд
 *
 * ���أ�
 *	���ɹ�װ�ز���ʼ��ʱ������errNone�����򷵻ض�Ӧ�Ĵ������
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