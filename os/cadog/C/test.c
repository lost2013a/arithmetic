/*====================================================================
	Copyright (C) Rockey Tech. Co. Ltd. All rights reserved.

	File: 	ET299Edit.cpp
	
	Purpose:Sample console editor for ET299 	
======================================================================*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "et299api.h"

int g_hToken = -1;

int WaitForUserInput()
{
	int  ic;
	//
	printf("\nInput selection:");
	fflush(stdin);
	//
	ic = getchar();
	//
	fflush(stdin);
    //
    fgetc(stdin);
    //
	return ic;
}

void ShowMainCmdMenu()
{
	//HANDLE hStd=GetStdHandle(STD_OUTPUT_HANDLE); 
	//SetConsoleTextAttribute(hStd,FOREGROUND_INTENSITY|FOREGROUND_BLUE|FOREGROUND_GREEN);
	
	printf("\n\nMain menu:\n");
	printf("-------------------------------------------------------------\n");
	printf(	"  [F]ind      Open[T]oken    LED[O]n          D[A]taMenu\n"
			"  GetS[N]     GenP[I]D       GenRando[M]      Cr[Y]ptMenu\n"
			"  User[P]IN   [S]OPIN        [R]eset          Set[U]pMenu\n"
			"  LE[D]Off    [C]lose        ET29[9]          Update[L]icense\n"
			"  E[X]it\n"
			);
	//SetConsoleTextAttribute(hStd,FOREGROUND_GREEN|FOREGROUND_BLUE|FOREGROUND_RED);	
}

void ShowBanner()
{
	unsigned long version = 0;
	printf("\nRockey ET299 Editor v1.0.\nCreate at " __DATE__ ", " __TIME__ "\n\n" );

	printf( "Library version is V1.0\n");
}

void ShowErrInfo(unsigned long retval)
{
	switch(retval)
	{
	case ET_SUCCESS				    	 :
		printf("Success!\n");
		return;
	case ET_ACCESS_DENY					 :
		printf("Err: access denied, have not enough right!");
		break;
	case ET_COMMUNICATIONS_ERROR		 :
		printf("Err: have not open the device");
		break;
	case ET_INVALID_PARAMETER		     :
		printf("Err: invalid parameter!");
		break;
	case ET_NOT_SET_PID					 :
		printf("Err: have not set PID!");
		break;
	case ET_UNIT_NOT_FOUND			     :
		printf("Err: open the device fail!");
		break;
	case 0xF0                            :
		printf("Err: PIN have been locked!");
		break;
	default								 :
		{
			if(retval&0xF0)
				printf("Err: PIN is wrong!");
			else
				printf("Err: unknown error occured!");
		}
		break;
	}
	printf("(0x%X)\n",retval);
}

void ShowBinHex(unsigned char* pBin, long len)
{
	// Show 16 bytes each line.
	int  i, j ,k, kk;
	long lLines = len / 16;
	long lCharInLastLine = 0;
	if(0 != len % 16)
	{
		lCharInLastLine = len - lLines * 16;
	}

	for(i = 0; i < lLines; ++i)
	{
		for(j = 0; j < 16; ++j)
		{
			printf("%02X ", pBin[16 * i + j]);

			if(j == 7)
				printf("- ");
		}

		printf("    ");

		for(j = 0; j < 16; ++j)
		{
			if(isprint(pBin[16 * i + j]))
				printf("%c", pBin[16 * i + j]);
			else
				printf(".");
		}

		printf("\n");
	}

	if(0 != lCharInLastLine)
	{
		for(j = 0; j < lCharInLastLine; ++j)
		{
			printf("%02X ", pBin[16 * i + j]);

			if(j == 7 && lCharInLastLine > 8)
				printf("- ");
		}

		k = 0;

		k += ((16 - lCharInLastLine) * 3);

		if(lCharInLastLine <= 8)
		{
			k += 2;
		}

		for(kk = 0; kk < k; ++kk)
			printf(" ");

		printf("    ");

		for(j = 0; j < lCharInLastLine; ++j)
		{
			if(isprint(pBin[16 * i + j]))
				printf("%c", pBin[16 * i + j]);
			else
				printf(".");
		}

		printf("\n");
	}
	printf("\n");
}

void Test299()
{	
	unsigned long retval;
	int       i, len;
	unsigned char  tb;
	unsigned short tw;
	unsigned long  td;
	time_t         t;
	struct tm *    ptm;
	struct SYSTEMTIME * pst;
	//FILE*       fp;
	unsigned char buf[512];
	//
    printf("et_SetKeyEx(ET_KEY_HMAC_MD5, 1, \"FFFFFFFFFFFFFFFF\") ......");	
	retval = et_SetKeyEx(g_hToken, ET_KEY_HMAC_MD5, 1, "FFFFFFFFFFFFFFFF");
	if(retval != ET_SUCCESS)
	{
       if(retval == 1)
	   {
         printf("Error: %2X  ... Need Input SOPIN to Access\n", retval);
	   }
	   else
	   {
          printf("Error: %2X\n", retval);
	   }
	   //
	   return;
	}
	printf("OK\n");
	//===========
    printf("et_SetKeyEx(ET_KEY_TDES, 1, \"FFFFFFFFFFFFFFFF\") ......");	
	retval = et_SetKeyEx(g_hToken, ET_KEY_TDES, 1, "FFFFFFFFFFFFFFFF");
	if(retval != ET_SUCCESS)
	{
       printf("Error: %2X\n", retval);
	   return;
	}
	printf("OK\n");
	//===========
    printf("et_SetSNEx(\"1234567890ABCDEFGH\") ......");	
    retval = et_SetSNEx(g_hToken, "1234567890ABCDEFGH", 18);
    if(retval != ET_SUCCESS)
	{
       printf("Error: %2X\n", retval);
	   return;
	}
	printf("OK\n");
	//===========
	memset(buf, 0, sizeof(buf));
	len = sizeof(buf);  //必须填写缓冲区长度
    printf("et_GetSNEx() ......");
	retval = et_GetSNEx(g_hToken, buf, &len);
    if(retval != ET_SUCCESS)
	{
       printf("Error: %2X\n", retval);
	   return;
	}
	printf("OK\nSN_LEN=%d\nSN=%s\n", len, buf);
	//===========    
    memset(buf, 0, sizeof(buf));
	printf("et_RSAGenKey(RSA_KEY_LEN_1024, pucKey, priKey) ......");
	retval = et_RSAGenKey(g_hToken, RSA_KEY_LEN_1024, buf, buf+256);
    if(retval != ET_SUCCESS)
	{
       printf("Error: %2X\n", retval);
	   return;
	}
	printf("OK\n");
	printf("publicKEY:\n");
    ShowBinHex(buf, 128);
    printf("privateKEY:\n");
    ShowBinHex(buf+256, 128);
	//===========
	printf("et_RSASetKey(RSA_KEY_LEN_1024, pucKey, priKey) ......");
	retval = et_RSASetKey(g_hToken, RSA_KEY_LEN_1024, buf, buf+256);
    if(retval != ET_SUCCESS)
	{
       printf("Error: %2X\n", retval);
	   return;
	}
	printf("OK\n\n");
	//===========
    memset(buf, 0, sizeof(buf));
    for(i=0; i<128; i++)
	{
       buf[i] = i;
	}
	printf("Original Data:\n");
    ShowBinHex(buf, 128);
	//===========
	printf("et_RSAPublicEncrypt(buf, 128, ET_RSA_NO_PADDING) ......");	
	//
	len = 128;
	//
	retval = et_RSAPublicEncrypt(g_hToken, buf, &len, ET_RSA_NO_PADDING); 
	if(retval != ET_SUCCESS)
	{
       printf("Error: %2X\n", retval);
	   return;
	}
	printf("OK\n");
	//    
    printf("RSAPublicEncrypt Result:\n");
    ShowBinHex(buf, 128);
	//==========
	printf("et_RSAPrivateDecrypt(buf, 128, ET_RSA_NO_PADDING) ......");
	//
	len = 128;
	//
	retval = et_RSAPrivateDecrypt(g_hToken, buf, &len, ET_RSA_NO_PADDING); 
	if(retval != ET_SUCCESS)
	{
       printf("Error: %2X\n", retval);
	   return;
	}
	printf("OK\n");
	//
    printf("RSAPrivateDecrypt Result:\n");
    ShowBinHex(buf, 128);
	//=========
    printf("et_RSAPrivateEncrypt(buf, 128, ET_RSA_NO_PADDING) ......");
	//
	len = 128;
	//
	retval = et_RSAPrivateEncrypt(g_hToken, buf, &len, ET_RSA_NO_PADDING); 
	if(retval != ET_SUCCESS)
	{
       printf("Error: %2X\n", retval);
	   return;
	}
	printf("OK\n");
	//
    printf("RSAPrivateEncrypt Result:\n");
    ShowBinHex(buf, 128);
	//=========
    printf("et_RSAPublicDecrypt(buf, 128, ET_RSA_NO_PADDING) ......");
	//
	len = 128;
	//
	retval = et_RSAPublicDecrypt(g_hToken, buf, &len, ET_RSA_NO_PADDING); 
	if(retval != ET_SUCCESS)
	{
       printf("Error: %2X\n", retval);
	   return;
	}
	printf("OK\n");
	//
    printf("RSAPublicDecrypt Result:\n");
    ShowBinHex(buf, 128);
	//=========
    printf("et_TDesEncrypt(1, buf ,128) ......");
	//
	retval = et_TDesEncrypt(g_hToken, 1, buf, 128); 
	if(retval != ET_SUCCESS)
	{
       printf("Error: %2X\n", retval);
	   return;
	}
	printf("OK\n");
	//
    printf("TDesEncrypt Result:\n");
    ShowBinHex(buf, 128);
    //=========
    printf("et_TDesDecrypt(1, buf ,128) ......");
	//
	retval = et_TDesDecrypt(g_hToken, 1, buf, 128); 
	if(retval != ET_SUCCESS)
	{
       printf("Error: %2X\n", retval);
	   return;
	}
	printf("OK\n");
	//
    printf("TDesDecrypt Result:\n");
    ShowBinHex(buf, 128);
	//=========
	printf("et_SetTimeLicense(0, ET_EXPIRE_DATE ,\"2020-01-02 12:16:18\") ......");
	//
    pst = (struct SYSTEMTIME *) buf;
	pst->wYear   = 2020;
	pst->wMonth  = 1;
	pst->wDay    = 2;
	pst->wHour   = 12;
	pst->wMinute = 16;
    pst->wSecond = 18;
	pst->wMilliseconds = 0;
	pst->wDayOfWeek    = 0;
	//
	retval = et_SetTimeLicense(g_hToken, 0, ET_EXPIRE_DATE, buf);
	if(retval != ET_SUCCESS)
	{
       printf("Error: %2X\n", retval);
	   return;
	}
	printf("OK\n");
	//=========	
    printf("et_GetTimeLicense(0, &timeMode ,buf) ......");	
	//
	len = -1; //timeMode
	//
	retval = et_GetTimeLicense(g_hToken, 0, &len, buf);
	if(retval != ET_SUCCESS)
	{
       printf("Error: %2X\n", retval);
	   return;
	}
	printf("OK\n");
    pst = (struct SYSTEMTIME *) buf;
	printf("timeMode=%d, ExpireTime=%04d-%02d-%02d %02d:%02d:%02d\n", len, pst->wYear, pst->wMonth, pst->wDay, pst->wHour, pst->wMinute, pst->wSecond);
	//=========
    printf("Clear Time License ......");
	//
	memset(buf, 0xFF, sizeof(buf));   //全0xFF表示清除限制
	//
    retval = et_SetTimeLicense(g_hToken, 0, ET_EXPIRE_DATE, buf);
	if(retval != ET_SUCCESS)
	{
       printf("Error: %2X\n", retval);
	   return;
	}
	printf("OK\n");
	//=========
    printf("et_SetTimeLicense(0, ET_TIME_MINUTE , 120) ......");
	//
	*((int*)buf) = 120;
	//
	retval = et_SetTimeLicense(g_hToken, 0, ET_TIME_MINUTE, buf);
	if(retval != ET_SUCCESS)
	{
       printf("Error: %2X\n", retval);
	   return;
	}
	printf("OK\n");
	//=========
    printf("et_StartTimeUnit(0) ......");	
	//
	retval = et_StartTimeUnit(g_hToken, 0);	
	if(retval != ET_SUCCESS)
	{
       printf("Error: %2X\n", retval);
	   return;
	}
	printf("OK\n");
	//=========
    printf("et_StopTimeUnit(0) ......");	
	//
	retval = et_StopTimeUnit(g_hToken, 0);	
	if(retval != ET_SUCCESS)
	{
       printf("Error: %2X\n", retval);
	   return;
	}
	printf("OK\n");
	//=========
    printf("Clear Time License ......");
	//
	memset(buf, 0xFF, sizeof(buf));   //全0xFF表示清除限制
	//
    retval = et_SetTimeLicense(g_hToken, 0, ET_TIME_MINUTE, buf);
	if(retval != ET_SUCCESS)
	{
       printf("Error: %2X\n", retval);
	   return;
	}
	printf("OK\n");
    //=========  
    /*  
	printf("et_AdjustTimer() ......");	
	//
	t   = time(0);
    ptm = gmtime(&t);	
	//
	memset(buf, 0, sizeof(buf));
	//
    pst = (struct SYSTEMTIME*) buf;
	//
	pst->wSecond = ptm->tm_sec;
	pst->wMinute = ptm->tm_min;
	pst->wHour   = ptm->tm_hour;
	pst->wDay    = ptm->tm_mday;
	pst->wMonth  = ptm->tm_mon  + 1;
	pst->wYear   = ptm->tm_year + 1900;
	//
	retval = et_AdjustTimer(g_hToken, buf);
    if(retval != ET_SUCCESS)
	{
       printf("Error: %2X\n", retval);
	   return;
	}
	printf("OK\n");	
	*/
	//========
    printf("et_SetCountLicense(0, 100) ......");
	//
	retval = et_SetCountLicense(g_hToken, 0, 100, 1);  //ET_ENABLE_DECREASE = 1
    if(retval != ET_SUCCESS)
	{
       printf("Error: %2X\n", retval);
	   return;
	}
	printf("OK\n");
	//========
    printf("et_GetCountLicense(0, &count) ......");
	//
	tb = 0;
	td = 0;
	retval = et_GetCountLicense(g_hToken, 0, &td, &tb);
    if(retval != ET_SUCCESS)
	{
       printf("Error: %2X\n", retval);
	   return;
	}
	printf("OK\ncount=%d type=%d\n", td, tb);
    //========
    printf("et_GetCountLicense(0, &count) ......");
	//
	tb = 0;
	td = 0;
	retval = et_GetCountLicense(g_hToken, 0, &td, &tb);
    if(retval != ET_SUCCESS)
	{
       printf("Error: %2X\n", retval);
	   return;
	}
	printf("OK\ncount=%d type=%d\n", td, tb);
	//========
    printf("Clear CountLicense ......");
	//
	retval = et_SetCountLicense(g_hToken, 0, -1, 0);  //ET_DISABLE_DECREASE = 0  ET_ENABLE_DECREASE = 1
    if(retval != ET_SUCCESS)
	{
       printf("Error: %2X\n", retval);
	   return;
	}
	printf("OK\n");
	//========
    printf("et_GetCountLicense(0, &count) ......");
	//
	tb = 0;
	td = 0;
	retval = et_GetCountLicense(g_hToken, 0, &td, &tb);
    if(retval != ET_SUCCESS)
	{
       printf("Error: %2X\n", retval);
	   return;
	}
	printf("OK\ncount=%d type=%d\n", td, tb);
	//========
    printf("et_GetVersion() ......");
	//
	tw = 0;
	retval = et_GetVersion(g_hToken, &tw);
    if(retval != ET_SUCCESS)
	{
       printf("Error: %2X\n", retval);
	   return;
	}
	printf("OK\nCosVersion=0X%04X\n", tw);
	//========
    /*
    printf("fopen(\"et299update.inc\") ......");
    fp = fopen("et299update.inc", "rb");
	if(fp == 0)
	{
       printf("Error\n");
	   return;
	}
	//
	fread(buf, 288, 1, fp);
	fclose(fp);
	printf("OK\n");
	//========
    printf("et_CheckUpdateFile(buf, 128) ......");
	//
	retval = et_CheckUpdateFile(g_hToken, buf, 128);
    //
    if(retval != ET_SUCCESS)
	{
       printf("Error: %2X\n", retval);
	   return;
	}
	printf("OK\n");
	//========
	printf("et_RemoteUpdate() ......");
	//
	retval = et_RemoteUpdate(g_hToken, buf+128, 288 - 32 - 128);
    //
    if(retval != ET_SUCCESS)
	{
       printf("Error: %2X\n", retval);
	   return;
	}
	printf("OK\n");
*/
	//========

}

void UpdateLic()
{
	unsigned long retval;
	FILE*       fp;
	unsigned char buf[512];
    //========
    memset(buf, 0, sizeof(buf));
	//
    printf("fopen(\"et299update.inc\") ......");
    fp = fopen("et299update.inc", "rb");  //二进制只读
	if(fp == 0)
	{
       printf("Error\n");
	   return;
	}
	//
	fread(buf, 288, 1, fp);
	fclose(fp);
	printf("OK\n");
	//========
    printf("et_Verify(ET_VERIFY_USERPIN, \"FFFFFFFFFFFFFFFFetrockeyFFC5EB78\") ......");
	//
	retval = et_Verify(g_hToken, ET_VERIFY_USERPIN, "FFFFFFFFFFFFFFFFetrockeyFFC5EB78");
    //
    if(retval != ET_SUCCESS)
	{
       printf("Error: %2X\n", retval);
	   return;
	}
	printf("OK\n");
	//========
    printf("et_CheckUpdateFile(buf+32, 128) ......");
	//
	retval = et_CheckUpdateFile(g_hToken, buf+32, 128);
    //
    if(retval != ET_SUCCESS)
	{
       printf("Error: %2X\n", retval);
	   return;
	}
	printf("OK\n");
	//========
	printf("et_RemoteUpdate() ......");
	//
	retval = et_RemoteUpdate(g_hToken, buf+32+128, 288-32-128);
    //
    if(retval != ET_SUCCESS)
	{
       printf("Error: %2X\n", retval);
	   return;
	}
	printf("OK\n");	
}

void ShowSetupMenu()
{
	unsigned long retval = ET_SUCCESS;
	int  len;
	char Pin[20] = {0};
	char newPin[20] = {0};
	char soPin[20];
	char iBuf[60] = {0};
	char seed[60] = {0};
	//char soPin[16];
	unsigned char bSoPINRetries;
	unsigned char bUserPINRetries;
	unsigned char bUserReadOnly;
	char buff[16];
	//
	while(1)
	{
		//HANDLE hStd=GetStdHandle(STD_OUTPUT_HANDLE); 
		//SetConsoleTextAttribute(hStd,FOREGROUND_INTENSITY|FOREGROUND_BLUE|FOREGROUND_GREEN);
		printf("\n\nSetup menu:\n");
		printf("-------------------------------------------------------------\n");
		printf("  User[P]IN          Gen[S]OPIN          [R]esetUserPIN \n");
		printf("  Setup[T]oken       E[X]it\n\n");
		//SetConsoleTextAttribute(hStd,FOREGROUND_GREEN|FOREGROUND_BLUE|FOREGROUND_RED);
		switch(WaitForUserInput())
		{
		case 'P':
		case 'p':
			{				
				printf("\nChange User PIN\n");				
				printf("Input old User PIN <16>:");
				fflush(stdin);
				//gets(Pin);
				fgets(Pin, sizeof(Pin), stdin);
				printf("Input new User PIN <16>:");
				fflush(stdin);
				//gets(newPin);
				fgets(newPin, sizeof(newPin), stdin);
				retval = et_ChangeUserPIN(g_hToken,(unsigned char *)Pin,(unsigned char *)newPin);
				ShowErrInfo(retval);
			}
			break;
		case 'S':
		case 's':
			{
				
				soPin[16]='\0';
				
				printf("Please input seed <1-51>: ");
				fflush(stdin);
				//gets((char*)seed);
				fgets(seed, sizeof(seed), stdin);
				//
				len = strlen(seed);
				retval = et_GenSOPIN(g_hToken,len,seed,soPin);
				ShowErrInfo(retval);
				if(ET_SUCCESS == retval)
				{
					printf("The new SO PIN is : \n");
					printf("%s",(char*)soPin);
				}
			}
			break;
		case 'R':
		case 'r':
			{
				
				printf("Please input the SO PIN <16>: ");
				fflush(stdin);
				//gets(soPin);
				fgets(soPin, sizeof(soPin), stdin);
				retval = et_ResetPIN(g_hToken,(unsigned char *)soPin);
				ShowErrInfo(retval);
			}
			break;
		case 'T':
		case 't':
			{
				
				printf("Please input SO PIN retry count <0-15>:");
				fflush(stdin);
				//gets(&buff);
				fgets(buff, sizeof(buff), stdin);
				sscanf(buff,"%ld",&bSoPINRetries);

				printf("Please input User PIN retry count <0-15>:");
				fflush(stdin);
				//gets(&buff);
				fgets(buff, sizeof(buff), stdin);
				sscanf(buff,"%ld",&bUserPINRetries);

				printf("Please input user read or write data zone flags <0 or 1>:");
				fflush(stdin);
				//gets(&buff);
				fgets(buff, sizeof(buff), stdin);
				sscanf(buff,"%ld",&bUserReadOnly);
				
				retval = et_SetupToken(g_hToken,bSoPINRetries,bUserPINRetries,bUserReadOnly,0);
				ShowErrInfo(retval);
			}
			break;
		case 'X':
		case 'x':
			return;
		default:
			break;
		}
	}
}

void ShowCryptMenu()
{
	unsigned long retval = ET_SUCCESS;
	unsigned long fid1 = 0;
	unsigned long fid2 = 0;
	char tBuf[40] = {0};
	int keyIndex=0;
	unsigned char key[40] = {0};
    char text[80] = {0};
    unsigned char Dig1[20] = {0};
	unsigned char Dig2[20] = {0};
    char skey[100] = {0};
    char tmpBuf[40] = {0};
	int index = 0;
	char hkey[100] = {0};
	unsigned char tDig[20] = {0};
	//
	while(1)
	{
		//HANDLE hStd=GetStdHandle(STD_OUTPUT_HANDLE); 
		//SetConsoleTextAttribute(hStd,FOREGROUND_INTENSITY|FOREGROUND_BLUE|FOREGROUND_GREEN);
		printf("\n\nCrypt Menu:\n");
		printf("-------------------------------------------------------------\n");
		printf("  [M]akeHMACKey    [H]MAC     E[X]it\n");
		//SetConsoleTextAttribute(hStd,FOREGROUND_GREEN|FOREGROUND_BLUE|FOREGROUND_RED);
		switch(WaitForUserInput())
		{
		case 'H':
		case 'h':
			{
				printf("\nUsing key for HMAC-MD5 authentication\n\n");

				
				printf("Input index of the key <1-8>:");
				fflush(stdin);
				//gets(tBuf);
				fgets(tBuf, sizeof(tBuf), stdin);
				sscanf(tBuf,"%ld",&keyIndex);
				
				fflush(stdin);
				printf("Input random data for HMAC-MD5 authentication(MAX 51 bytes):");
				//gets(text);
				fgets(text, sizeof(text), stdin);
				
				retval = et_HMAC_MD5(g_hToken,keyIndex,strlen(text),(unsigned char *)text,Dig1);
				ShowErrInfo(retval);
				if(ET_SUCCESS!=retval)
					break;
				ShowBinHex(Dig1,16);
				
				printf("\n\nPleas input the	secret HMAC-MD5 key to verify:");
				
				fflush(stdin);
				//gets(skey);
				fgets(skey, sizeof(skey), stdin);
				retval = MD5_HMAC((unsigned char*)text,strlen((char*)text),(unsigned char*)skey,strlen(skey),key,Dig2);
				ShowErrInfo(retval);
				if(ET_SUCCESS!=retval)
					break;
				printf("Software HMAC-MD5 compute result:\n");
				ShowBinHex(Dig2,16);
				
				if(memcmp(Dig1,Dig2,16))
				{
					printf("Verify failed!");
				}
				else
				{
					printf("Verify successfully!");
				}
			}
			break;
		case 'M':
		case 'm':
			{
				printf("\n Create key files for HMAC-MD5 authentication.\n\n");
				
				printf("Input index of the key <1-8>:");
				fflush(stdin);
				//gets(tmpBuf);
				fgets(tmpBuf, sizeof(tmpBuf), stdin);
				sscanf(tmpBuf,"%ld",&index);

				printf("Input the secret HMAC-MD5 key:");
				
				fflush(stdin);
				//gets(hkey);
				fgets(hkey, sizeof(hkey), stdin);
				
				
				retval = MD5_HMAC(NULL,0,(unsigned char*)hkey,strlen(hkey),key,tDig);
				ShowErrInfo(retval);
				if(ET_SUCCESS!=retval)
					break;

				printf("HMAC-MD5 key content:\n");
				ShowBinHex(key,32);

				retval = et_SetKey(g_hToken,index,key);
				ShowErrInfo(retval);
			}
			break;
		case 'X':
		case 'x':
			return;
		default:
			break;
		}
	}
}

void ShowDataMenu()
{
	unsigned long retval = ET_SUCCESS;
    unsigned char iBuf7[100] = {0};
	unsigned long offs = 0;
	unsigned long rLen = 0;
	unsigned char rBuff[600] = {0};
	unsigned long off2 = 0;
	unsigned char iBuf8[100] = {0};
	unsigned long rLen2 = 0;
	unsigned long index = 0;
	char keyData[40];
    //
	while(1)
	{
		//HANDLE hStd=GetStdHandle(STD_OUTPUT_HANDLE); 
		//SetConsoleTextAttribute(hStd,FOREGROUND_INTENSITY|FOREGROUND_BLUE|FOREGROUND_GREEN);	
		printf("\n\nData Menu:\n");
		printf("-------------------------------------------------------------\n");
		printf(	"  [R]ead   [W]rite   Set[K]ey   E[X]it\n" );
		//SetConsoleTextAttribute(hStd,FOREGROUND_GREEN|FOREGROUND_BLUE|FOREGROUND_RED);
		switch(WaitForUserInput())
		{
		case 'R':
		case 'r':
			{
				
				printf("Input offset to begin read <0-6000>:");
				fflush(stdin);
				//gets((char*)iBuf7);
				fgets((char*)iBuf7, sizeof(iBuf7)-1, stdin);
				sscanf((char*)iBuf7,"%ld",&offs);
				printf("Input number of byte to read <0-500>:");
				
				fflush(stdin);
				//gets((char*)iBuf7);
				fgets((char*)iBuf7, sizeof(iBuf7)-1, stdin);
				sscanf((char*)iBuf7,"%ld",&rLen);
				
				retval = et_Read(g_hToken,(unsigned short)offs,(unsigned short)rLen,rBuff);
				ShowErrInfo(retval);
				if(ET_SUCCESS == retval)
				{
					printf("\n =>> %ld (0x%X) byte%s read.\n", rLen, rLen, rLen > 1 ? "s" : "");
					ShowBinHex(rBuff,rLen);
				}
			}
			break;
		case 'W':
		case 'w':
			{
				
				printf("Input offset to begin write <0-6000>:");
				fflush(stdin);
				//gets((char*)iBuf8);
				fgets((char*)iBuf8, sizeof(iBuf8)-1, stdin);
				sscanf((char*)iBuf8,"%ld",&off2);
				printf("Input data to write <0-500>:");
				fflush(stdin);
				//gets((char*)iBuf8);
				fgets((char*)iBuf8, sizeof(iBuf8)-1, stdin);
				
				rLen2 = strlen((char*)iBuf8);
				printf("\n =>> %ld byte%s to write.\n",rLen2, rLen2 > 1 ? "s" : "");
				ShowBinHex(iBuf8,rLen2);
				 
				retval = et_Write(g_hToken,(unsigned short)off2,(unsigned short)rLen2,(unsigned char *)iBuf8);
				ShowErrInfo(retval);
				if(ET_SUCCESS == retval)
				{
					printf("\n =>> %ld byte%s written successfully!",rLen2, rLen2 > 1 ? "s" : "");
				}
			}
			break;
		case 'K':
		case 'k':
			{
				
				printf("Input Key's index <1-8>:");
				fflush(stdin);
				//gets((char*)iBuf8);
				fgets((char*)iBuf8, sizeof(iBuf8)-1, stdin);
				sscanf((char*)iBuf8,"%ld",&index);
				printf("Input Key(32):");
				fflush(stdin);
				//gets(keyData);
				fgets((char*)keyData, sizeof(keyData)-1, stdin);
				retval = et_SetKey(g_hToken,index,(unsigned char *)keyData);
				ShowErrInfo(retval);
			}
			break;
		case 'X':
		case 'x':
			return;
		}
	}
}

int main(int argc, char* argv[])
{
	unsigned long retval = ET_SUCCESS;
    unsigned long PID = 0;
	int count = 0;
	char iBuf[100] = {0};
	unsigned char SN[16];
	unsigned char pid[16];
	unsigned char seed[60];	
	char Pin[20] = {0};
	char sopin[20] = {0};
	unsigned char rBuf[20] = {0};
    //
	ShowBanner();
	
	while(1)
	{
		ShowMainCmdMenu();
		switch(WaitForUserInput())
		{
		case 'F':
		case 'f':
			{				
				printf("Input PID <8>: ");
				fflush(stdin);
				//gets(iBuf);
				fgets(iBuf, sizeof(iBuf), stdin);
				//sscanf(iBuf,"%lx",&PID);
				retval = et_FindToken((unsigned char*)iBuf,&count);
				ShowErrInfo(retval);
				if(ET_SUCCESS == retval)
					printf("Find %ld ET299.\n",count);
			}
			break;
		case 'N':
		case 'n':
			{				
				retval = et_GetSN(g_hToken,SN);
				ShowErrInfo(retval);
				if(ET_SUCCESS == retval)
				{
					printf("The SN is : \n");
					ShowBinHex(SN,8);
				}
			}
			break;
		case 'I':
		case 'i':
			{
				
				pid[8]='\0';
				
				printf("Please input seed <1-51>: ");
				fflush(stdin);
				//gets(iBuf);
				fgets(iBuf, sizeof(iBuf), stdin);
				memcpy(seed,iBuf,strlen(iBuf));
				retval = et_GenPID(g_hToken,strlen(iBuf),seed,pid);
				ShowErrInfo(retval);
				if(ET_SUCCESS == retval)
				{
					printf("The new PID is : %s\n",(char*)pid);
				}
			}
			break;
		case 'T':
		case 't':
			{
				
				printf("Please input PID <8>: ");
				fflush(stdin);
				//gets(iBuf);
				fgets(iBuf, sizeof(iBuf), stdin);
				//sscanf(iBuf,"%lx",&pid);
				retval = et_OpenToken(&g_hToken,(unsigned char*)iBuf,1);
				ShowErrInfo(retval);
			}
			break;
		case 'O':
		case 'o':
			printf("\nTurn LED on:");
			retval = et_TurnOnLED(g_hToken);
			ShowErrInfo(retval);
			break;
		case 'P':
		case 'p':
			{
				
				printf("Input PIN <16>:");
				fflush(stdin);
				//gets(Pin);
				fgets(Pin, sizeof(Pin), stdin);
				retval = et_Verify(g_hToken,ET_VERIFY_USERPIN,(unsigned char *)Pin);
				ShowErrInfo(retval);
			}
			break;
		case 'A':
		case 'a':
			ShowDataMenu();
			break;
		case 'D':
		case 'd':
			printf("\nTurn LED off:");
			retval = et_TurnOffLED(g_hToken);
			ShowErrInfo(retval);
			break;
		case 'S':
		case 's':
			{
				
				printf("Input SO PIN <16>:");
				fflush(stdin);
				//gets(sopin);
				fgets(sopin, sizeof(sopin), stdin);
				retval = et_Verify(g_hToken,ET_VERIFY_SOPIN,(unsigned char*)sopin);
				ShowErrInfo(retval);
			}
			break;
		case 'M':
		case 'm':
			{
				printf("Generate random \n");
				
				retval = et_GenRandom(g_hToken,rBuf);
				ShowErrInfo(retval);
				if(ET_SUCCESS == retval)
				{
					ShowBinHex(rBuf,16);
				}
			}
			break;
		case 'Y':
		case 'y':
			ShowCryptMenu();
			break;
		case 'R':
		case 'r':
			//printf("Reset token:");
			retval = et_ResetSecurityState(g_hToken);
			ShowErrInfo(retval);
			break;
		case 'U':
		case 'u':
			ShowSetupMenu();
			break;
		case 'C':
		case 'c':
			printf("\nClose device:");
			retval = et_CloseToken(g_hToken);
			ShowErrInfo(retval);
			break;
		case '9':
			printf("Test Start \n");
			Test299();
            printf("Test End\n");
			break;	
		case 'L':
		case 'l':
			printf("Test UpdateLicense \n");
			UpdateLic();
            printf("Test End\n");
			break;	
		case 'X':
		case 'x':
			et_CloseToken(g_hToken);
			return 0;
		default:
			break;
		}
	}
}


