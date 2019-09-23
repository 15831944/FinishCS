#include "StdAfx.h"
#include "Encryption.h"

unsigned short g_nDogValue;
bool g_bHasDog=false;
bool g_bReduceDogTimes=false;

#ifdef CODEMETER

#include "wibuixap.h"
#include "codemeter.h"
#define CMULONG    unsigned long
WUPIDECLFUNC void jzlineFunction();
WUPIDECLFUNC void hzdtkFunction();
WUPIDECLFUNC void pltfFunction();
WUPIDECLFUNC void elecofferrptFunction();

bool WibuEncryption(CMULONG ulFirmCode, CMULONG ulProductCode)
{
	bool bDemoReturn = true;

	// Create an "Entry Access"
	CMACCESS cmacc;
	memset(&cmacc, 0, sizeof(cmacc));
	cmacc.mflCtrl = CM_ACCESS_NOUSERLIMIT;
	cmacc.mulFirmCode = ulFirmCode;
	cmacc.mulProductCode = ulProductCode;
	//cmacc.mcmBoxInfo = mpabCmBoxInfo[nBoxIndex];
	HCMSysEntry hcmse = CmAccess(CM_ACCESS_LOCAL_LAN, &cmacc);
	if (0 ==hcmse) {
		//log.error("Error %d in CmAccess(CM_ACCESS_LOCAL) of %d:%d.", CmGetLastErrorCode(), cmacc.mulFirmCode, cmacc.mulProductCode);
		return false;
	} else {
		//log.detailed("  Using CmContainer %d-%d. Handle: %d.", cmacc.mcmBoxInfo.musBoxMask, cmacc.mcmBoxInfo.mulSerialNumber, hcmse);
	} // if

	// set encryption parameters
	CMCRYPT2 cmcrypt2;
	memset(&cmcrypt2, 0, sizeof(cmcrypt2));
	// for encryption in CmContainer (hardware encryption): use AES, calculate CRC 
	cmcrypt2.mcmBaseCrypt.mflCtrl = CM_CRYPT_AES | CM_CRYPT_CALCCRC; 
	// random encryption code
	srand( (unsigned)time( NULL ) );

	cmcrypt2.mcmBaseCrypt.mulEncryptionCode = rand(); 

	CMBOXINFO cmboxinfo;
	memset(&cmboxinfo, 0, sizeof(cmboxinfo));
	if (0 == CmGetInfo(hcmse, CM_GEI_BOXINFO, &cmboxinfo, sizeof(cmboxinfo))) {
		//		log.error("Error %d in CmGetInfo(CM_GEI_BOXINFO).", CmGetLastErrorCode());
		CmRelease(hcmse);
		return false;
	}

	// some options for encryption
	cmcrypt2.mcmBaseCrypt.mulEncryptionCodeOptions |= CM_CRYPT_UCIGNORE; 
	// for software encryption: use AES with CBC and initkey mechanism
	CMULONG flCtrl = CM_CRYPT_AES_ENC_CBC;

	//the initkey should be set to an own value for each pair of encryption/decryption
	//initialize initkey with random values
	unsigned char initkey[CM_BLOCK_SIZE];
	int range_max =255;
	srand( (unsigned)time( NULL ) );
	for (int i=0;i< sizeof(initkey);i++) {
		unsigned char charRandValue = (unsigned char)((double)rand() / (RAND_MAX + 1) * (range_max - 0));
		initkey[i]=charRandValue;
	}
	memcpy(cmcrypt2.mabInitKey, initkey, CM_BLOCK_SIZE);

	// string to encrypt
	unsigned char szData[] = {0,2,34,244,98,4,235,89,12,57,98,0,12,5,9,32,34,12,53,76,125,179,45,94,12,46,95,39};
	CMUINT cbSequence = (CMUINT)20 /*qstrlen(szTest)*/ /*strTest.length()*/ + 1; // for easy string handling 
	char * abSequence;
	char * abSource;
	// create a sequence to encrypt
	abSequence = new char [cbSequence]; 
	abSource = new char [cbSequence]; 
	// initialize, especially the last byte
	memset(abSequence, 0, cbSequence); 
	// copy string to sequence
	memcpy(abSequence, szData , min(cbSequence, (CMUINT) 20 /*qstrlen(szTest)*/));
	// make backup of original
	memcpy(abSource, abSequence, cbSequence);
	//log.detailed("  Text to encrypt: \"%s\"",abSequence);
	// encrypt sequence
	int nRet = CmCrypt2(hcmse, flCtrl, &cmcrypt2, abSequence, cbSequence - 1);
	if (0 == nRet) {
		//log.error("Error %d in CmCrypt2().", CmGetLastErrorCode());
		bDemoReturn = false;
	} else {
		// now let's decrypt
		// remove algorithm flag
		flCtrl &= ~CM_CRYPT_AES_ENC_CBC; 
		// add flag for decryption
		flCtrl |= CM_CRYPT_AES_DEC_CBC;
		// remove calculation of CRC
		cmcrypt2.mcmBaseCrypt.mflCtrl &= ~CM_CRYPT_CALCCRC;
		// add check of CRC
		cmcrypt2.mcmBaseCrypt.mflCtrl |= CM_CRYPT_CHKCRC; 
		// decrypt sequence
		nRet = CmCrypt2(hcmse, flCtrl, &cmcrypt2, abSequence, cbSequence - 1);
		if (0 == nRet) {
			//			log.error("Error %d in CmCrypt2().", CmGetLastErrorCode());
			bDemoReturn = false;
		} else {
			//			log.detailed("  Decrypted Text:  \"%s\"",abSequence);
			// compare decrypted with original
			bool fValid;
			for (CMUINT i = 0; (i < cbSequence); i++) 
			{
				fValid = (abSequence[i] == abSource[i]);
				if (!fValid) 
				{
					//					log.error("Decrypted string is not same as source string!");
					bDemoReturn = false;
					break;
				} // if
			} // for

		} // if
	} // if

	delete [] abSequence;
	delete [] abSource;
	CmRelease(hcmse);
	// 	if (bDemoReturn) {
	// //		log.message("** EncryptionDemo finished without errors **");
	// 	}
	return bDemoReturn;
} // EncryptionDemo()
#endif


//////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef CODEMETER
#include <CodeMeter.h>
#define FIRM_CODE 101828
#define PRO_CODE 100090001
//#define PRO_CODE 100090191
HCMSysEntry mhSysEntry;
unsigned char mabSignature[CM_PUBLIC_KEY_LEN];

unsigned char * mpRandomCharArray;
enum eClick {eNum, eSimpleOperator, eError};
eClick meLastClick;

void AccessCMStick(unsigned long lFirmCode, unsigned long lProductCode) {
	// create structure for access
	CMACCESS cmaccess;
	memset(&cmaccess, 0, sizeof(CMACCESS));
	cmaccess.mflCtrl = CM_ACCESS_NOUSERLIMIT | CM_ACCESS_FORCE;
	cmaccess.mulFirmCode = lFirmCode;
	cmaccess.mulProductCode = lProductCode;
	// Access local subsystem
	mhSysEntry = CmAccess(CM_ACCESS_LOCAL_LAN, &cmaccess);
	if (NULL == mhSysEntry) {
		//SetError("","AccessCMStick: ");
	}
}
void ReleaseCMStick() {
	CmRelease(mhSysEntry);
	mhSysEntry = NULL;
	delete [] mpRandomCharArray;
}
int CalculateSignature( unsigned char *acData, int cbData )
{
	// This Function signs a string that can be checked subsequently
	unsigned char abDigest[CM_DIGEST_LEN];
	unsigned int res;
	CMAUTHENTICATE pcmAuth;
	unsigned int cbSignature = sizeof(mabSignature);

	// calculate a hash of the used data
	res = CmCalculateDigest(acData, cbData, abDigest, CM_DIGEST_LEN);

	// if no handle is available try to access
	if (NULL == mhSysEntry) {
		AccessCMStick(FIRM_CODE,PRO_CODE);
	}

	// check state of the Calculator. Exit on error.
	if (eError == meLastClick) {
		return FALSE;
	}

	// perform the current signature
	memset(&pcmAuth, 0, sizeof(pcmAuth));
	pcmAuth.mflCtrl = CM_AUTH_SECRETDATA | CM_AUTH_EXTENDED ;
	pcmAuth.mulFirmCode = FIRM_CODE;
	pcmAuth.mulProductCode = PRO_CODE;
	pcmAuth.mulKeyExtType = 0;
	pcmAuth.mulEncryptionCodeOptions = CM_CRYPT_UCIGNORE; //allows to calculate signature also if UC=0
	pcmAuth.mulFeatureCode = 0;
	memcpy( pcmAuth.mabDigest, abDigest, CM_DIGEST_LEN);
	res = CmCalculateSignature(mhSysEntry, &pcmAuth, mabSignature, cbSignature );
	char ptext[5000] = {0};
	int aa = CmGetLastErrorText(CM_AUTH_SECRETDATA | CM_AUTH_EXTENDED , ptext , 4999);
	if(res==0) {
		//SetMessage("Signature generation failed!");
		return FALSE;
	}
	return TRUE;
}

// Checks the stored data with the previously calculated signature
// 
// Parameters
//   data       data to be checked
//   signature  signature that signs the data
// Returns      true if the check was successful
int CheckSignature(unsigned char *acData, int cbData)
{
	unsigned char abPublicKey[CM_PUBLIC_KEY_LEN] = {
		0x20, 0x88, 0x9e, 0x64, 0x43, 0xc2, 0x64, 0x8b, 
		0x9d, 0xd8, 0x31, 0x3b, 0x91, 0xee, 0x81, 0x6a, 
		0xb7, 0xf2, 0xe1, 0x2b, 0x3b, 0x81, 0xe7, 0x31, 
		0x23, 0xc5, 0x77, 0x97, 0x00, 0x00, 0x00, 0x00, 
		0x90, 0x17, 0x2f, 0xde, 0x55, 0x9a, 0x14, 0x2f, 
		0xf2, 0x64, 0x4d, 0x35, 0xfb, 0x78, 0xc7, 0x38, 
		0x28, 0xa5, 0xc0, 0xab, 0x45, 0xe0, 0x22, 0xe3, 
		0x9b, 0xd9, 0xaf, 0x57, 0x00, 0x00, 0x00, 0x00};



		unsigned char abDigest[CM_DIGEST_LEN];
		unsigned int cbDigest = sizeof(abDigest);
		unsigned int res;
		CMAUTHENTICATE cmAuth;

		// calculate a hash of the used data
		res = CmCalculateDigest(acData, cbData, abDigest, CM_DIGEST_LEN);

		// Validate the signature. We are not using a CM-Stick here!
		memset(&cmAuth, 0, sizeof(cmAuth));
		cmAuth.mflCtrl = CM_AUTH_EXTENDED;
		cmAuth.mulFirmCode = FIRM_CODE;
		cmAuth.mulProductCode = PRO_CODE;
		cmAuth.mulKeyExtType = 0;
		cmAuth.mulEncryptionCodeOptions = CM_CRYPT_UCIGNORE;
		cmAuth.mulFeatureCode = 0;
		memcpy( cmAuth.mabDigest, abDigest, CM_DIGEST_LEN);
		memset( &cmAuth.mcmBoxInfo, 0, sizeof(CMBOXINFO) );

		res = CmValidateSignature(&cmAuth, mabSignature, sizeof(mabSignature), abPublicKey, sizeof(abPublicKey));
		if(res)
		{
			//SetMessage("Signature checked successful!");
		}
		else
		{
			return FALSE;
		}
		return TRUE;
}

//证书验证函数，类似于 seekdog
bool CheckLIC()
{
	//////////////////////////////////////////////////////////////////////////

	if(!CheckSignature(mpRandomCharArray, RND_ARR_LEN))
	{
		//QMessageBox::information(NULL,"提示","证书错误！");
		return false;
	}

	delete [] mpRandomCharArray;
	mpRandomCharArray = new unsigned char [RND_ARR_LEN];
	// initialize random generator
	srand((UINT)GetTickCount());
	for (int i=0; i<RND_ARR_LEN; i++) 
		mpRandomCharArray[i] = rand()*255 / RAND_MAX;

	if (!CalculateSignature(mpRandomCharArray, RND_ARR_LEN)) {
		return false;
	}

	return true;
}
//以上为codemeter证书加密内容

#endif
//////////////////////////////////////////////////////////////////////////////////////////////////////////

bool Cass_Register(TCHAR *szCode,unsigned short nType)
{
	TCHAR *ppp,*qqq;
	BYTE szPassWord[30],lpOut[30],*pb;
	unsigned long returnlen=0;
	unsigned short nDog1,nDog2,nDog3,nDog4;
	int nNumber=0;

	for (unsigned j=0; j< _tcslen(szCode); j++) {
		if ( _T('-') == szCode[j] ) nNumber++;
	}
	if ( 3 != nNumber ) 
	{
		//acutPrintf(_T("\n注册失败！！！"));
		return false;
	}

	//取得四个整数
	qqq = szCode; nDog1 = _ttoi(qqq); 
	ppp=_tcschr(qqq,_T('-')); qqq=ppp+1; nDog2 = _ttoi(qqq);
	ppp=_tcschr(qqq,_T('-')); qqq=ppp+1; nDog3 = _ttoi(qqq);
	ppp=_tcschr(qqq,_T('-')); qqq=ppp+1; nDog4 = _ttoi(qqq);

	//建立参数字符串
	nType += 100;
	S4_ConvertShort(&nType); //add at 2004.04.17
	pb=szPassWord+0; memcpy(pb,&nDog1,2);
	pb=szPassWord+2; memcpy(pb,&nDog2,2);
	pb=szPassWord+4; memcpy(pb,&nDog3,2);
	pb=szPassWord+6; memcpy(pb,&nDog4,2);
	pb=szPassWord+8; memcpy(pb,&nType,2);
	szPassWord[10] = '\0';

	bool bResult = SenseExecute("\\0001", "1050", szPassWord,
		10,lpOut,returnlen);
	if ( true == bResult && 10 == returnlen )
	{
		//acutPrintf(_T("\n注册成功！！！"));
		return true;
	}
	else
	{
		//acutPrintf(_T("\n注册失败！！！"));
		return false;
	}
}

bool InitGsmhDog()
{
	Code1(41);

	BYTE szIn[100],szOut[255];
	DWORD returnlen;
	szIn[0] = 3;
	if ( S4_SUCCESS == S4Execute(&s4_context,"1054",szIn,2,szOut,255,&returnlen) ) {
		if ( 1 != szOut[1] ) {//表示不是CASS70
			return false;
		}
	}
	else {
		return false;
	}

	unsigned short nDogTimes;
	bool bResult = Sense_Short_Short("\\0001","1001",3,nDogTimes); //读次数,3代表CASS9
	if ( false == bResult ) return false;
	if ( nDogTimes <= 0 ) return false;

	unsigned short nBanben;

	bResult = Sense_Short_Short("\\0001","1049", 52, nBanben); //查厦门竣工加密号

	if ( false == bResult ) return false;

#ifdef S4NEWNET
	bResult = Sense_Short_Short("\\0001","1004",4,g_nDogValue); //取现有数
#elif SENSE4NET
	bResult = Sense_Short_Short("\\0001","1004",4,g_nDogValue); //取现有数
#else
	bResult = Sense_Short_Short("\\0001","1003",4,g_nDogValue); //产生随机数
#endif

	if ( false == bResult ) return false;
	g_bHasDog = true;
	return true;
}

#ifndef _WIN64
bool RSA_Verify_Dog()
{
	R_RSA_PUBLIC_KEY RpubKey = {
		1024,
		{
			0xe5,0xc0,0x71,0xe0,0x51,0xfe,0xc3,0x9b,0x37,0x14,0x14,0x79,0x27,0x71,0xde,0x21,
				0x2e,0x42,0xc6,0x53,0xa8,0x96,0x12,0xf4,0x5d,0xd3,0xa7,0x1c,0xcd,0xec,0x6c,0xde,
				0x0f,0x4a,0xad,0x9d,0xf3,0xb2,0xc8,0x46,0xb6,0x68,0x73,0xf8,0x08,0x13,0x50,0x3c,
				0x21,0x57,0xd1,0x39,0x3d,0x5a,0x19,0xb8,0x4b,0xf0,0xf6,0xf1,0x62,0x42,0x87,0x2e,
				0x27,0xb9,0xbf,0xbf,0x98,0xae,0x77,0xa4,0x85,0xcd,0xc8,0xf1,0xe4,0xef,0x7d,0xd5,
				0x1c,0x41,0xf1,0x33,0x41,0xea,0xad,0x56,0x77,0xd1,0xcb,0x0c,0x86,0x66,0x92,0x91,
				0x49,0x17,0x34,0xe6,0x1d,0x93,0xe5,0x29,0x1a,0x10,0x7b,0x3e,0x92,0x56,0xf8,0x88,
				0x81,0xc4,0x32,0x21,0x06,0x68,0x0f,0x41,0x50,0x4c,0x04,0xbe,0xa4,0xae,0xe5,0xe3,
		},
		{
			0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
				0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
				0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
				0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
				0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
				0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
				0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
				0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x00,0x01
			}
	};
	BYTE inbuff[32],tmpbuff[32],outbuff[128];
	DWORD returnlen,es;

	// 初始化随机数发生器 ////////////////////////////////////////////////////
	long clockL,clockH,CLOCKS;
	if ( isRDTSCsupported() ) {
		__asm { // 读取CPU上电以来的CLOCK计数
			rdtsc;
			mov clockL, eax;
			mov clockH, edx;
		}
	} 
	else {
		clockL = GetTickCount(); // 使用GetTickCount取数据
	}
	CLOCKS = clockL;
	srandom(clockL);
	//////////////////////////////////////////////////////////////////////////

	// 这个部分是需要使用随机数的部分 ////////////////////////////////////////
	_rand(inbuff, 32);
	memcpy(tmpbuff,inbuff,32);
	es = S4Execute(&s4_context,"1055",inbuff,32,outbuff,128,&returnlen); //1096
	//////////////////////////////////////////////////////////////////////////

	// 这部分用于迷惑破解者(大部分情况下RDTSC指令用于计时)
	// 初始化随机数发生器 ////////////////////////////////////////////////////
	if (isRDTSCsupported()) {
		__asm { // 读取CPU上电以来的CLOCK计数
			rdtsc;
			mov clockL, eax;
			mov clockH, edx;
		}
		CLOCKS = (clockL - CLOCKS) / (500 * 1000);
	} else {
		clockL = GetTickCount();
		CLOCKS = clockL - CLOCKS;
	}
	srandom(clockL);
	//////////////////////////////////////////////////////////////////////////

	//认证算法
	if (es != S4_SUCCESS) return false;
	int nRet = Verify(DA_SHS, tmpbuff, 32, outbuff, 128, &RpubKey);
	if ( nRet != RE_SUCCESS ) return false;
	else return true;
}
#endif

bool Reduce_DogTimes()
{
	Code1(42);
	if ( false == g_bReduceDogTimes ) 
	{
		g_bHasDog = false;
		unsigned short nTemp;
		bool bResult = Sense_Short_Short("\\0001","1002",4,nTemp); //减次数
		if ( false == bResult ) return false;
		g_bReduceDogTimes = true;
		g_bHasDog = true;
	}
	return true;
}

bool seekdog()
{
	//return true;

	CString strPath, strText;
	strPath.Format(_T("%scass.log"), getSystemPath());

#ifndef NOSENSE
	AddLog(strPath, _T("开始"));
	Code1(43);
	if ( false == Reduce_DogTimes() ) return false;
	AddLog(strPath, _T("Reduce_DogTimes"));

	if ( false == g_bHasDog ) return false;
	AddLog(strPath, _T("g_bHasDog is true"));

	unsigned short dogdog;
	bool bResult = Sense_Short_Short("\\0001","1004",4,dogdog); //检验
	if ( false == bResult ) return false;
	AddLog(strPath, _T("bResult is true"));

	if ( dogdog != g_nDogValue ) return false;
	strText.Format(_T("dogdog: %d g_nDogValue: %d"), dogdog, g_nDogValue);
	AddLog(strPath, strText);
#endif

#ifdef CODEMETER
	if ( false == WibuEncryption(101828, 100090001) ) return false;
#endif
	AddLog(strPath, _T("结束"));

	return true;
}