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
#define	  ET_SUCCESS				0x00			//函数执行成功
#define   ET_ACCESS_DENY            0x01			//访问被拒绝，权限不够
#define   ET_COMMUNICATIONS_ERROR   0x02			//通讯错误，没有打开设备
#define   ET_INVALID_PARAMETER      0x03			//无效的参数，参数出错
#define   ET_NOT_SET_PID            0x04			//没有设置PID
#define   ET_UNIT_NOT_FOUND 	    0x05			//打开指定的设备失败
#define   ET_HARD_ERROR             0x06			//硬件错误
#define   ET_UNKNOWN                0x07			//一般性错误
#define	  ET_PIN_ERR_MASK			0x0F			//验证PIN码掩码
#define	  ET_PIN_ERR_MAX		    0xFF            //验证PIN码最大剩余次数，如果验证错误还还回ET_PIN_ERR_MAX则表示PIN码永远不被锁死。

//新增错误码
#define	  ET_KEY_TYPE				0X10			//密钥类型错误
#define	  ET_LICENSE_TYPE			0X11			//授权类型错误
#define	  ET_LICENSE_UNINIT			0x12			//授权单元未启用，出厂时，全部授权单元都未启用，只有当设置授权后才会被启用
#define   ET_NO_LICENSE				0x13			//授权失败
#define   ET_UNKNOWN_UPDATE_FILE	0x14			//未知的升级文件格式
#define	  ET_INVALID_DATA_LEN		0x15			//非法的数据长度
#define   ET_NO_TDES_KEY			0x16			//无效的TDES密钥
#define   ET_INVALID_UPDATE_FILE	0x17			//无效的升级文件
#define	  ET_UNSUPPORTED			0x18			//不支持的功能
#define	  ET_ADJUST_TIMER			0x19			//调整系统时钟失败
#define	  ET_KEY_INDEX				0X1A			//错误的密钥索引
#define	  ET_KEY_ZERO				0X1B			//0号密钥为系统专用

//linux下新增错误码
#define   ET_TOOMUCHTHREAD		0x1C    //同一个进程中打开锁的线程数 > 100

//常量定义
#define ET299_MEMORY_SIZE			6*1024			//ET299的数据区大小 

//出厂设置的PID
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

//查找ET299，输入PID，输出找到的ET299数目
unsigned long 	et_FindToken(unsigned char* pid/*[in]*/,int * count);

//打开指定PID和索引的ET299，输入PID，索引
unsigned long 	et_OpenToken(int* pHandle,unsigned char* pid,int index);

//关闭ET299
unsigned long 	et_CloseToken(int hHandle);

//读ET299数据区
unsigned long 	et_Read(int hHandle,unsigned short offset,int Len,unsigned char* pucReadBuf);

//写ET299数据区
unsigned long 	et_Write(int hHandle,unsigned short offset,int Len,unsigned char* pucWriteBuf);

//产生PID,输入种子和长度，输出新的PID
unsigned long 	et_GenPID(int hHandle,int SeedLen,unsigned char* pucSeed,unsigned char* pid);

//产生随机数
unsigned long 	et_GenRandom(int hHandle,unsigned char* pucRandBuf);

//产生新的SoPIN，输入种子和长度，输出新的SoPIN
unsigned long 	et_GenSOPIN(int hHandle,int SeedLen,unsigned char* pucSeed, unsigned char* pucNewSoPIN);

//重置UserPIN
unsigned long 	et_ResetPIN(int hHandle,unsigned char* pucSoPIN);

//设置密钥，原ET99接口，只设置HMAC-MD5密钥，密钥长度32字节
unsigned long 	et_SetKey(int hHandle,int Keyid, unsigned char* pucKeyBuf);

//设置密钥，扩展原ET99接口，可设置TDES密钥,3DES密钥长度16字节
unsigned long 	et_SetKeyEx(int hHandle,int keyType,int Keyid, unsigned char* keyBuf);
//选择密钥类型
#define ET_KEY_HMAC_MD5			0	//选择HMAC-MD5密钥	
#define ET_KEY_TDES				1	//选择3DES密钥

//HMAC-MD5运算，输入密钥索引，数据及长度，输出16字节摘要
unsigned long 	et_HMAC_MD5(int hHandle,int keyID,int textLen,unsigned char* pucText,unsigned char *digest);

//验证PIN码
unsigned long 	et_Verify(int hHandle,int Flags,unsigned char* pucPIN);

//define pin Flags
#define ET_VERIFY_USERPIN			0
#define ET_VERIFY_SOPIN				1

//修正UserPIN，输入原UserPIN，新的UserPIN
unsigned long 	et_ChangeUserPIN(int hHandle,unsigned char* pucOldPIN,unsigned char* pucNewPIN);

//重置安全状态
unsigned long 	et_ResetSecurityState(int hHandle);//需要重置到et_logoff

//返回硬件序列号，出厂时烧入的8字节硬件序列号，用户不可更改
unsigned long 	et_GetSN(int hHandle,unsigned char* pucSN/*8bytes*/);

//配置ET2999，设置SoPIN重试次数，UserPIN重试次数，USERPIN只读属性
unsigned long 	et_SetupToken(int hHandle,unsigned char bSoPINRetries,unsigned char bUserPINRetries,unsigned char bUserReadOnly,unsigned char bBack);

#define ET_USER_WRITE_READ				0	//读写
#define ET_USER_READ_ONLY				1	//只读

//打开ET299指示灯
unsigned long 	et_TurnOnLED(int hHandle);

//关闭ET299指示灯
unsigned long 	et_TurnOffLED(int hHandle);

//获取自定义硬件序列号
unsigned long 	et_GetSNEx(int hHandle, unsigned char* pucSN,int* len);

//设置自定义硬件序列号
unsigned long 	et_SetSNEx(int hHandle,unsigned char* pucSNEx,int len);

//产生RSA密钥对，e=65537 并返回，RSA密钥对只能在此时返回，其它任何时候无法读取
unsigned long 	et_RSAGenKey(int hHandle,int keyLen,unsigned char* pBufferRSA_N, unsigned char* pBufferRSA_D);

#define RSA_KEY_LEN_1024	1024	//RSA 密钥长度为1024位
	
//设置RSA密钥对，用于RSA运算
unsigned long 	et_RSASetKey(int hHandle,int keyLen,unsigned char* pBufferRSA_N, unsigned char* pBufferRSA_D);
//设置远程升级专用RSA密钥对，用于远程升级
unsigned long 	et_RSASetUpdateKey(int hHandle,int keyLen,unsigned char* pBufferRSA_N, unsigned char* pBufferRSA_D);

#define ET_RSA_PKCS1_PADDING	1	//PKCS#1填充方式
#define ET_RSA_NO_PADDING		2	//无填充
//RSA公钥加密
unsigned long 	et_RSAPublicEncrypt(int hHandle,unsigned char* buffer,int *len,int nFillMode);

//RSA公钥解密
unsigned long 	et_RSAPublicDecrypt(int hHandle,unsigned char* buffer,int *len,int nFillMode);

//RSA私钥加密
unsigned long   et_RSAPrivateEncrypt(int hHandle,unsigned char* buffer,int* len,int nFillMode);

//RSA私钥解密
unsigned long 	et_RSAPrivateDecrypt(int hHandle,unsigned char* buffer,int* len,int nFillMode);

//3DES加密
unsigned long 	et_TDesEncrypt(int hHandle,int keyIndex,unsigned char* buffer,int len);

//3DES解密
unsigned long 	et_TDesDecrypt(int hHandle, int keyIndex,unsigned char* buffer,int len);

//设置时间授权
unsigned long 	et_SetTimeLicense(int hHandle, int unitIndex,int timeMode,unsigned char* newTime);

#define ET_EXPIRE_DATE		0	//截止日期授权
#define ET_TIME_MINUTE		1	//计时授权(单位：分)
#define ET_TIME_DAY			2	//计日授权(单位：日)

//获取时间授权
unsigned long 	et_GetTimeLicense(int hHandle, int unitIndex,int *timeMode,unsigned char* newTime);

//启动计时授权
unsigned long 	et_StartTimeUnit (int hHandle, int unitIndex);

//启动计时授权
unsigned long 	et_StopTimeUnit (int hHandle, int unitIndex);

//调整ET299系统时间
unsigned long 	et_AdjustTimer(int hHandle, unsigned char* newTime);

//设置计次授权
unsigned long 	et_SetCountLicense(int hHandle, int unitIndex,unsigned long count, unsigned char type);

//获取计次授权
unsigned long 	et_GetCountLicense(int hHandle, int unitIndex,unsigned long *count,unsigned char *type);

//检测升级文件合法性
unsigned long 	et_CheckUpdateFile(int hHandle,unsigned char* updateBuffer,int len);

//远程升级
unsigned long 	et_RemoteUpdate (int hHandle,unsigned char* updateBuffer,int len);

//由授权文件生成远程升级文件
unsigned long 	et_GenRemoteUpdateFile (int hHandle, unsigned char* updateBuffer, int len);

//获取ET299版本
unsigned long 	et_GetVersion (int hHandle, unsigned short* version);

/*{{{ MD5 HMAC Wrapper functions.*/
// 软件实现
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

