/*
[]=========================================================================[]

Copyright(C) 2008, Feitian Technologies Co., Ltd.
All rights reserved.
 
FILE:
	FT_ET299_API.h
	
DESC:
	ET299 interface and defines 
		
[]=========================================================================[]
*/

#ifndef  __FT_ET299_HEADER_H
#define  __FT_ET299_HEADER_H

#ifdef __cplusplus
extern "C" {
#endif


//Return Value Status Definition
#define	  ET_SUCCESS				0x00			//����ִ�гɹ�
#define   ET_ACCESS_DENY            0x01			//���ʱ��ܾ���Ȩ�޲���
#define   ET_COMMUNICATIONS_ERROR   0x02			//ͨѶ����û�д��豸
#define   ET_INVALID_PARAMETER      0x03			//��Ч�Ĳ�������������
#define   ET_NOT_SET_PID            0x04			//û������PID
#define   ET_UNIT_NOT_FOUND 	    0x05			//��ָ�����豸ʧ��
#define   ET_HARD_ERROR             0x06			//Ӳ������
#define   ET_UNKNOWN                0x07			//һ���Դ���
#define	  ET_PIN_ERR_MASK			0x0F			//��֤PIN������
#define	  ET_PIN_ERR_MAX		    0xFF            //��֤PIN�����ʣ������������֤���󻹻���ET_PIN_ERR_MAX���ʾPIN����Զ����������

//����������
#define	  ET_KEY_TYPE				0X10			//��Կ���ʹ���
#define	  ET_LICENSE_TYPE			0X11			//��Ȩ���ʹ���
#define	  ET_LICENSE_UNINIT			0x12			//��Ȩ��Ԫδ���ã�����ʱ��ȫ����Ȩ��Ԫ��δ���ã�ֻ�е�������Ȩ��Żᱻ����
#define   ET_NO_LICENSE				0x13			//��Ȩʧ��
#define   ET_UNKNOWN_UPDATE_FILE	0x14			//δ֪�������ļ���ʽ
#define	  ET_INVALID_DATA_LEN		0x15			//�Ƿ������ݳ���
#define   ET_NO_TDES_KEY			0x16			//��Ч��TDES��Կ
#define   ET_INVALID_UPDATE_FILE	0x17			//��Ч�������ļ�
#define	  ET_UNSUPPORTED			0x18			//��֧�ֵĹ���
#define	  ET_ADJUST_TIMER			0x19			//����ϵͳʱ��ʧ��
#define	  ET_KEY_INDEX				0X1A			//�������Կ����
#define	  ET_KEY_ZERO				0X1B			//0����ԿΪϵͳר��

//linux������������
#define   ET_TOOMUCHTHREAD		0x1C    //ͬһ�������д������߳��� > 100

//��������
#define ET299_MEMORY_SIZE			6*1024			//ET299����������С 

//�������õ�PID
#define CONST_PID                   "FFFFFFFF"

struct SYSTEMTIME {
    unsigned short wYear; 
    unsigned short wMonth; 
    unsigned short wDayOfWeek; 
    unsigned short wDay; 
    unsigned short wHour; 
    unsigned short wMinute; 
    unsigned short wSecond; 
    unsigned short wMilliseconds; 
} ; 

//����ET299������PID������ҵ���ET299��Ŀ
unsigned long 	et_FindToken(unsigned char* pid/*[in]*/,int * count);

//��ָ��PID��������ET299������PID������
unsigned long 	et_OpenToken(int* pHandle,unsigned char* pid,int index);

//�ر�ET299
unsigned long 	et_CloseToken(int hHandle);

//��ET299������
unsigned long 	et_Read(int hHandle,unsigned short offset,int Len,unsigned char* pucReadBuf);

//дET299������
unsigned long 	et_Write(int hHandle,unsigned short offset,int Len,unsigned char* pucWriteBuf);

//����PID,�������Ӻͳ��ȣ�����µ�PID
unsigned long 	et_GenPID(int hHandle,int SeedLen,unsigned char* pucSeed,unsigned char* pid);

//���������
unsigned long 	et_GenRandom(int hHandle,unsigned char* pucRandBuf);

//�����µ�SoPIN���������Ӻͳ��ȣ�����µ�SoPIN
unsigned long 	et_GenSOPIN(int hHandle,int SeedLen,unsigned char* pucSeed, unsigned char* pucNewSoPIN);

//����UserPIN
unsigned long 	et_ResetPIN(int hHandle,unsigned char* pucSoPIN);

//������Կ��ԭET99�ӿڣ�ֻ����HMAC-MD5��Կ����Կ����32�ֽ�
unsigned long 	et_SetKey(int hHandle,int Keyid, unsigned char* pucKeyBuf);

//������Կ����չԭET99�ӿڣ�������TDES��Կ,3DES��Կ����16�ֽ�
unsigned long 	et_SetKeyEx(int hHandle,int keyType,int Keyid, unsigned char* keyBuf);
//ѡ����Կ����
#define ET_KEY_HMAC_MD5			0	//ѡ��HMAC-MD5��Կ	
#define ET_KEY_TDES				1	//ѡ��3DES��Կ

//HMAC-MD5���㣬������Կ���������ݼ����ȣ����16�ֽ�ժҪ
unsigned long 	et_HMAC_MD5(int hHandle,int keyID,int textLen,unsigned char* pucText,unsigned char *digest);

//��֤PIN��
unsigned long 	et_Verify(int hHandle,int Flags,unsigned char* pucPIN);

//define pin Flags
#define ET_VERIFY_USERPIN			0
#define ET_VERIFY_SOPIN				1

//����UserPIN������ԭUserPIN���µ�UserPIN
unsigned long 	et_ChangeUserPIN(int hHandle,unsigned char* pucOldPIN,unsigned char* pucNewPIN);

//���ð�ȫ״̬
unsigned long 	et_ResetSecurityState(int hHandle);//��Ҫ���õ�et_logoff

//����Ӳ�����кţ�����ʱ�����8�ֽ�Ӳ�����кţ��û����ɸ���
unsigned long 	et_GetSN(int hHandle,unsigned char* pucSN/*8bytes*/);

//����ET2999������SoPIN���Դ�����UserPIN���Դ�����USERPINֻ������
unsigned long 	et_SetupToken(int hHandle,unsigned char bSoPINRetries,unsigned char bUserPINRetries,unsigned char bUserReadOnly,unsigned char bBack);

#define ET_USER_WRITE_READ				0	//��д
#define ET_USER_READ_ONLY				1	//ֻ��

//��ET299ָʾ��
unsigned long 	et_TurnOnLED(int hHandle);

//�ر�ET299ָʾ��
unsigned long 	et_TurnOffLED(int hHandle);

//��ȡ�Զ���Ӳ�����к�
unsigned long 	et_GetSNEx(int hHandle, unsigned char* pucSN,int* len);

//�����Զ���Ӳ�����к�
unsigned long 	et_SetSNEx(int hHandle,unsigned char* pucSNEx,int len);

//����RSA��Կ�ԣ�e=65537 �����أ�RSA��Կ��ֻ���ڴ�ʱ���أ������κ�ʱ���޷���ȡ
unsigned long 	et_RSAGenKey(int hHandle,int keyLen,unsigned char* pBufferRSA_N, unsigned char* pBufferRSA_D);

#define RSA_KEY_LEN_1024	1024	//RSA ��Կ����Ϊ1024λ
	
//����RSA��Կ�ԣ�����RSA����
unsigned long 	et_RSASetKey(int hHandle,int keyLen,unsigned char* pBufferRSA_N, unsigned char* pBufferRSA_D);
//����Զ������ר��RSA��Կ�ԣ�����Զ������
unsigned long 	et_RSASetUpdateKey(int hHandle,int keyLen,unsigned char* pBufferRSA_N, unsigned char* pBufferRSA_D);

#define ET_RSA_PKCS1_PADDING	1	//PKCS#1��䷽ʽ
#define ET_RSA_NO_PADDING		2	//�����
//RSA��Կ����
unsigned long 	et_RSAPublicEncrypt(int hHandle,unsigned char* buffer,int *len,int nFillMode);

//RSA��Կ����
unsigned long 	et_RSAPublicDecrypt(int hHandle,unsigned char* buffer,int *len,int nFillMode);

//RSA˽Կ����
unsigned long   et_RSAPrivateEncrypt(int hHandle,unsigned char* buffer,int* len,int nFillMode);

//RSA˽Կ����
unsigned long 	et_RSAPrivateDecrypt(int hHandle,unsigned char* buffer,int* len,int nFillMode);

//3DES����
unsigned long 	et_TDesEncrypt(int hHandle,int keyIndex,unsigned char* buffer,int len);

//3DES����
unsigned long 	et_TDesDecrypt(int hHandle, int keyIndex,unsigned char* buffer,int len);

//����ʱ����Ȩ
unsigned long 	et_SetTimeLicense(int hHandle, int unitIndex,int timeMode,unsigned char* newTime);

#define ET_EXPIRE_DATE		0	//��ֹ������Ȩ
#define ET_TIME_MINUTE		1	//��ʱ��Ȩ(��λ����)
#define ET_TIME_DAY			2	//������Ȩ(��λ����)

//��ȡʱ����Ȩ
unsigned long 	et_GetTimeLicense(int hHandle, int unitIndex,int *timeMode,unsigned char* newTime);

//������ʱ��Ȩ
unsigned long 	et_StartTimeUnit (int hHandle, int unitIndex);

//������ʱ��Ȩ
unsigned long 	et_StopTimeUnit (int hHandle, int unitIndex);

//����ET299ϵͳʱ��
unsigned long 	et_AdjustTimer(int hHandle, unsigned char* newTime);

//���üƴ���Ȩ
unsigned long 	et_SetCountLicense(int hHandle, int unitIndex,unsigned long count, unsigned char type);

//��ȡ�ƴ���Ȩ
unsigned long 	et_GetCountLicense(int hHandle, int unitIndex,unsigned long *count,unsigned char *type);

//��������ļ��Ϸ���
unsigned long 	et_CheckUpdateFile(int hHandle,unsigned char* updateBuffer,int len);

//Զ������
unsigned long 	et_RemoteUpdate (int hHandle,unsigned char* updateBuffer,int len);

//����Ȩ�ļ�����Զ�������ļ�
unsigned long 	et_GenRemoteUpdateFile (int hHandle, unsigned char* updateBuffer, int len);

//��ȡET299�汾
unsigned long 	et_GetVersion (int hHandle, unsigned short* version);

/*{{{ MD5 HMAC Wrapper functions.*/
// ���ʵ��
unsigned long 
MD5_HMAC(unsigned char * pucText,        /* pointer to data stream        */
		 unsigned long   ulText_Len,     /* length of data stream         */
		 unsigned char * pucKey,         /* pointer to authentication key */
		 unsigned long   ulKey_Len,      /* length of authentication key  */
		 unsigned char * pucToenKey,     /* inner hashed key and  outer hashed key */
		 unsigned char * pucDigest );    /* caller digest to be filled in */
/*}}}*/

#ifdef __cplusplus
}
#endif


#endif	//__FT_ET299_HEADER_H

