/*
 * CLib.h
 * ͨѶ¼������ V1.0
 *
 * ���ߣ�REi.Laputa��2006��11��22��
 * ��Ȩ����
 * http://rei.gtyz.org
 */

#ifndef CLIB_H_
#define CLIB_H_

#include <SystemMgr.h>
#include "ContactDefines.h"

/**************************************************************************************
 * ���ݽṹ����
 **************************************************************************************/

//����CLibGetMatchContactByRecordId()ʱ���ṩ�Ĳ�����
typedef struct
{
	UInt16					recordId;			//Ҫ��ȡ��ͨѶ��¼�ļ�¼��
	UInt32					pyKeyword[10];		//���ԱȽϵ�ƴ������ĸ�ؼ��֣������
												//  constructPYKeyword()���й���
	Int16					keywordLength;		//�ؼ��ֵĳ���
	Boolean					orderlySearch;		//�Ƚ�ģʽ��
												//true  - �������ϸ�ӵ�һ���ַ���ʼ
												//        �Ƚϣ�����ؼ���"yd"������
												//        ��"�ƶ�ͨ��"�����Ҳ���"�й��ƶ�"
												//false - �������ҳ����а����ؼ��ֵ�
												//        �����
} CNLBParameterType;

//����CLibGetMatchContactByRecordId()ʱ���ṩ��Ԥ��ʼ������������������ͨ������
//initCNLBGlobeParameter()���г�ʼ����
typedef struct
{
	Char					keyword[256];		//���ڹ���ͨѶ��¼������Ϣ���ַ�������
	Int16					nextV[10];			//����KMP�㷨�Ļ���
	UInt32					pyData[256];		//����װ����keyword[]���ɵ�ƴ����Ϣ
	UInt16					pyDataLength;		//pyData[]����Ч��Ϣ�ĳ���
	DmOpenRef				addressDbRef;		//ͨѶ¼���ݿ��ָ��
	UInt32					*pyTableP;			//ƴ��ת�����ָ��
} CNLBGlobeType;

//����CLibGetContacts_PAdd()�󣬷��صĽ����Ԫ
/*typedef struct
{
	AddrDBRecordType		data;				//ͨѶ��¼�����ContactDefines.h��Palm
												//�ṩ��ͨѶ¼�ĵ�
	Char					fullName[256];		//����������
	UInt16					fullNameLength;		//���ֵĳ���
} CNLBContactType;

//����CLibGetContacts_Addr()�󣬷��صĽ����Ԫ������ͬ��
typedef struct
{
	OLD_AddrDBRecordType	data;
	Char					fullName[256];		//����������
	UInt16					fullNameLength;		//���ֵĳ���
} OLD_CNLBContactType;*/

/**************************************************************************************
 * �궨��
 **************************************************************************************/

#define CLIB_LIB_TRAP(trapNum)	SYS_TRAP(trapNum)

#define	CLibCreatorID			'CNLB'					//�ⴴ����ID
#define	CLibTypeID				sysFileTLibrary			//������

#define PY_TABLE_FTR_NUMBER		0x01

#define CNLB_RUNTIME_ERROR		0x01					//����ʱ����ͨ��ָ�ڴ����Ȳ��ɻָ��Ĵ���
#define	CNLB_NOT_MATCH_ERROR	0x02					//��ƥ����󣬸�����ƴ���ؼ�������ҵļ�¼��ƥ��
#define CNLB_DB_NOT_FOUND_ERROR	0x03					//ƴ��ת�����ݿ�δ�ҵ�
#define CNLB_OPEN_CONTACT_ERROR	0x04					//ͨѶ¼δ�ҵ�

#define SEARCH_MODE_FULLNAME	0x00					//ȫ������ģʽ
#define SEARCH_MODE_LASTNAME	0x01					//��������
#define SEARCH_MODE_FIRSTNAME	0x02					//��������
#define SEARCH_MODE_PHONE		0x03					//�������ģʽ
#define SEARCH_MODE_GET			0x04

#define ADDRESS_VERSION_OLD		0x00					//Treo600����ǰ��PalmOS��ʹ�õ����ݿ⣨AddressDB.pdb��
#define ADDRESS_VERSION_NEW		0x01					//Treo650���Ժ��PalmOS��ʹ�õ����ݿ⣨ContactDB-PAdd.pdb��

//���ڲ���ƴ��ת����ĺ�
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

//ˢ�µ���CLibGetMatchContactByRecordId()ʱ���ṩ��Ԥ��ʼ����������ѭ������
//CLibGetMatchContactByRecordId()ʱ��ÿ�ε�����Ϻ�Ӧ��ʹ�ñ���
#define refreshCNLBGlobeParameter(n)	MemSet(n, 1302, 0x00)

/**************************************************************************************
 * �⺯������
 **************************************************************************************/

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
extern Err CLibLoad(UInt16 *refNumP, UInt32 *clientContextP);

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
extern Err CLibRemove(UInt16 refNum, UInt32 clientContext);

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
extern Err initCNLBGlobeParameter(CNLBGlobeType **globe, UInt8 addressType);

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
extern Err constructPYKeyword(CNLBParameterType *param, Char *keyword);

//��������Ӧ��ֱ�ӵ���
extern Err CLibOpen(UInt16 refNum, UInt32 *clientContextP)
	CLIB_LIB_TRAP(sysLibTrapOpen);

//��������Ӧ��ֱ�ӵ���
extern Err CLibClose(UInt16 refNum, UInt32 clientContext)
	CLIB_LIB_TRAP(sysLibTrapClose);

extern Err CLibSleep(UInt16 refNum)
	CLIB_LIB_TRAP(sysLibTrapSleep);

extern Err CLibWake(UInt16 refNum)
	CLIB_LIB_TRAP(sysLibTrapWake);

/*
 * CLibGetMatchContactByRecordId_Addr
 * ��������λ�õ�ͨѶ��¼�Ƿ��������ƴ������ĸƥ�䣬��ƥ�䣬�򷵻ظ�ͨѶ��¼���ɰ�ͨѶ¼��
 *
 * ������
 *	refNum			->	������
 *	globe			->	Ԥ��ʼ������
 *	param			->	��������������ƴ������ĸ�鼰Ҫ���м�����ͨѶ��¼
 *	result			<->	���ؼ�����ƥ���ͨѶ��¼
 *
 * ���أ�
 *	���ɹ���������ͨѶ��¼ƥ���ʱ�򣬷���errNone�����򷵻ض�Ӧ�Ĵ������
 */
extern Err CLibGetMatchContactByRecordId_Addr(UInt16 refNum, CNLBGlobeType *globe, CNLBParameterType *param, OLD_AddrDBRecordType *result, UInt8 mode)
	CLIB_LIB_TRAP(sysLibTrapBase + 5);

/*
 * CLibGetMatchContactByRecordId_PAdd
 * ��������λ�õ�ͨѶ��¼�Ƿ��������ƴ������ĸƥ�䣬��ƥ�䣬�򷵻ظ�ͨѶ��¼���°�ͨѶ¼��
 *
 * ������
 *	refNum			->	������
 *	globe			->	Ԥ��ʼ������
 *	param			->	��������������ƴ������ĸ�鼰Ҫ���м�����ͨѶ��¼
 *	result			<->	���ؼ�����ƥ���ͨѶ��¼
 *
 * ���أ�
 *	���ɹ���������ͨѶ��¼ƥ���ʱ�򣬷���errNone�����򷵻ض�Ӧ�Ĵ������
 */
extern Err CLibGetMatchContactByRecordId_PAdd(UInt16 refNum, CNLBGlobeType *globe, CNLBParameterType *param, AddrDBRecordType *result, UInt8 mode)
	CLIB_LIB_TRAP(sysLibTrapBase + 6);

/*
 * CLibGetContacts_Addr
 * ��ͨѶ¼�з��ط��ϸ���ƴ������ĸ�ؼ��ִ�������ͨѶ��¼
 * ���ɰ�ͨѶ¼��
 *
 * ������
 *	refNum			->	������
 *	keyword			->	������ƴ������ĸ�ؼ��ִ�
 *	orderlySearch	->	����ģʽ�����"CNLBParameterType"��orderlySearch��˵��
 *	result			->	���ڷ��ط��ϼ���Ҫ���ͨѶ��¼
 *	contactNum		<->	���ط��ϼ���������ͨѶ��¼������
 *
 * ���أ�
 *	���ɹ�����ʱ������errNone�����򷵻ض�Ӧ�Ĵ������
 */
extern Err CLibGetContacts_Addr(UInt16 refNum, Char *keyword, Boolean orderlySearch, UInt8 mode, OLD_AddrDBRecordType ***result, UInt16 *contactNum)
	CLIB_LIB_TRAP(sysLibTrapBase + 7);

/*
 * CLibGetContacts_PAdd
 * ��ͨѶ¼�з��ط��ϸ���ƴ������ĸ�ؼ��ִ�������ͨѶ��¼
 * ���°�ͨѶ¼��
 *
 * ������
 *	refNum			->	������
 *	keyword			->	������ƴ������ĸ�ؼ��ִ�
 *	orderlySearch	->	����ģʽ�����"CNLBParameterType"��orderlySearch��˵��
 *	result			->	���ڷ��ط��ϼ���Ҫ���ͨѶ��¼
 *	contactNum		<->	���ط��ϼ���������ͨѶ��¼������
 *
 * ���أ�
 *	���ɹ�����ʱ������errNone�����򷵻ض�Ӧ�Ĵ������
 */
extern Err CLibGetContacts_PAdd(UInt16 refNum, Char *keyword, Boolean orderlySearch, UInt8 mode, AddrDBRecordType ***result, UInt16 *contactNum)
	CLIB_LIB_TRAP(sysLibTrapBase + 8);

#endif /* CLIB_H_ */
