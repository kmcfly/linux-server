/*******************************************************************************
** Copyright (c) Tongwei Video Technology Co.,Ltd. All rights reserved.				
** AUTHOR		:	CC	
** DATE			:	2013-04-17
** Version		:	
** DESCRIPTION	:使用openssl库，选择摘要算法，生成摘要
** Modify Record:
*******************************************************************************/

#ifndef __DIGEST_GEN__
#define __DIGEST_GEN__

#include "openssl/evp.h"
#include "Base64.h"
#include "RTSP_Common.h"

//const unsigned short NONCE_LEN = 20;
//const unsigned short CREATE_LEN = 20;

class CDigestGen
{
public:
	CDigestGen();
	~CDigestGen();

	bool HMAC_SHA1(const unsigned char *pText, int TextLen,const unsigned char *pKey, int KeyLen,unsigned char *pResult, unsigned int &ResultLen);
	
	bool Base64_HMAC_SHA1(const unsigned char *pText, int TextLen,const unsigned char *pKey, int KeyLen,unsigned char *pResult, unsigned int &ResultLen);

	bool GetDigest(const char *pDigestMethod, const void *pMsg, unsigned long MsgLen, unsigned char *pResult, unsigned int &ResultLen);

	bool GetWSSPassWdDiget(const char *pPassWd, unsigned int PassWdLen, \
		unsigned char *pResult, unsigned int &ResultLen, char *pNonce, char *pCreate,
		ONVIF_PUBLIC_UTC_TIME *aDevUtcTime = NULL);

private:
	//pDigestMethod 可以是 "SHA"  "SHA1" "MD5" 等
	bool Initial(const char *pDigestMethod);
	bool NonceGen(unsigned char *pNonce, int Nonce_Num);
	bool CreateGen(char *pCreate, ONVIF_PUBLIC_UTC_TIME *aDevUtcTime = NULL);

private:
	bool			m_IsInital;

	EVP_MD_CTX		*m_MdCtx;
	const EVP_MD	*m_Md;
	CBase64			m_Base64Obj;
};


#endif
