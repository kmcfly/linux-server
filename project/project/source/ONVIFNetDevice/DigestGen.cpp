#include "DigestGen.h"
#include <string.h>
#include "rand.h"


CDigestGen::CDigestGen()
:m_MdCtx(NULL),m_Md(NULL),m_IsInital(false)
{
	OpenSSL_add_all_digests();
}

CDigestGen::~CDigestGen()
{
   //printf("**** ~CDigestGen()!\n");
}

bool CDigestGen::Initial(const char *pDigestMethod)
{

	m_Md = EVP_get_digestbyname(pDigestMethod);
	if (NULL == m_Md)
	{
		return false;
	}

	m_MdCtx = EVP_MD_CTX_create();
	if (!EVP_DigestInit_ex(m_MdCtx, m_Md, NULL))
	{
		return false;
	}
	
	m_IsInital = true;

	return true;
}

bool CDigestGen::GetDigest(const char *pDigestMethod, const void *pMsg, unsigned long MsgLen, unsigned char *pResult, unsigned int &ResultLen)
{

	if (!pMsg || !pResult)
	{
		return false;
	}

	if (!Initial(pDigestMethod))
	{
		return false;
	}

	if (!EVP_DigestUpdate(m_MdCtx, pMsg, MsgLen))
	{
		return false;
	}

	if (!EVP_DigestFinal_ex(m_MdCtx, pResult, &ResultLen))
	{
		return false;
	}
	EVP_MD_CTX_destroy(m_MdCtx);

	return true;
}


bool CDigestGen::HMAC_SHA1(const unsigned char *pText, int TextLen,const unsigned char *pKey, int KeyLen,unsigned char *pResult, unsigned int &ResultLen)
{

	if (!pText || !pKey)
	{
		return false;
	}
	
	if (!Initial("SHA1"))
	{
		return false;
	}

	unsigned char k_ipad[65] = {0};
	unsigned char k_opad[65] = {0};
	
	memcpy(k_ipad, pKey, KeyLen);
	memcpy(k_opad, pKey, KeyLen);
	for (int i=0; i<64; i++)
	{
		k_ipad[i] ^= 0x36;
		k_opad[i] ^= 0x5c;
	}

	if (!EVP_DigestUpdate(m_MdCtx, k_ipad, 64))
	{
		return false;
	}

	if (!EVP_DigestUpdate(m_MdCtx, pText, TextLen))
	{
		return false;
	}

	if (!EVP_DigestFinal_ex(m_MdCtx, pResult, &ResultLen))
	{
		return false;
	}

	if (!Initial("SHA1"))
	{
		return false;
	}

	if (!EVP_DigestUpdate(m_MdCtx, k_opad, 64))
	{
		return false;
	}
	
	if (!EVP_DigestUpdate(m_MdCtx, pResult, ResultLen))
	{
		return false;
	}

	if (!EVP_DigestFinal_ex(m_MdCtx, pResult, &ResultLen))
	{
		return false;
	}

	EVP_MD_CTX_destroy(m_MdCtx);
	
	return true;
}

bool CDigestGen::Base64_HMAC_SHA1(const unsigned char *pText, int TextLen,const unsigned char *pKey, int KeyLen,unsigned char *pResult, unsigned int &ResultLen)
{
	unsigned char tmp[512] = {0};

	if (!HMAC_SHA1(pText, TextLen, pKey, KeyLen, tmp, ResultLen))
	{
		return false;
	}

	CBase64 Base64Obj;
	ResultLen = Base64Obj.Encode(tmp, ResultLen, (char *)pResult, 0);

	return true;
}


bool CDigestGen::NonceGen(unsigned char *pNonce, int Nonce_Num)
{
	if (!pNonce)
	{
		return false;
	}
	time_t r = time(NULL);
	memcpy(pNonce, &r, 4);
	unsigned char randomNum[4];

	for (int i = 4; i < Nonce_Num; i += 4)
	{ 
		RAND_pseudo_bytes(randomNum ,4);
		memcpy(pNonce + i, &randomNum, 4);
	}
    return true;
}

bool CDigestGen::CreateGen(char *pCreate, ONVIF_PUBLIC_UTC_TIME *aDevUtcTime )
{
	char tmpBuf[32];

	memset(tmpBuf,'\0',sizeof(tmpBuf));
	if (pCreate == NULL)
	{
		return false;
	}

	RTSP_LOCAL_TIME localTime = RTSPGetLocalTime();
	if( !aDevUtcTime || 0 == aDevUtcTime->uYear || 0 == aDevUtcTime->uMonth || 0 == aDevUtcTime->uDay )
	{
		sprintf(tmpBuf, "%04d-%02d-%02dT%02d:%02d:%02d.%03dZ", localTime.year, localTime.month, localTime.mday, \
			localTime.hour, localTime.minute, localTime.second, localTime.microsecond/1000);
	}
	else
	{
		sprintf(tmpBuf, "%04d-%02d-%02dT%02d:%02d:%02d.%03dZ", aDevUtcTime->uYear, aDevUtcTime->uMonth,aDevUtcTime->uDay, \
			aDevUtcTime->uHour, aDevUtcTime->uMointue, aDevUtcTime->uSeconds, localTime.microsecond/1000);
	}

	strcpy(pCreate,tmpBuf);
	return true;
}

bool CDigestGen::GetWSSPassWdDiget(const char *pPassWd, unsigned int PassWdLen, \
					   unsigned char *pResult, unsigned int &ResultLen, char *pNonce, char *pCreate, ONVIF_PUBLIC_UTC_TIME *aDevUtcTime)
{
	if (!pPassWd || !pResult)
	{
		return false;
	}

	unsigned char tmp[128] = {0}; //test segment fault
	unsigned char Nonce[64] = {0};
	unsigned char sha1[64] = {0};
	unsigned int  sha1Len;

	unsigned int  len_nonces = 24;
	unsigned int  len_creates = 24;
    
	memset(tmp,'\0',sizeof(tmp));
	memset(Nonce,'\0',sizeof(Nonce));
	memset(sha1,'\0',sizeof(sha1));

 	NonceGen(Nonce, len_nonces);
 	CreateGen(pCreate, aDevUtcTime);

	//传出去的nonce值是经过base64编码的
	m_Base64Obj.Encode(Nonce, len_nonces, pNonce, 0);

	memcpy(tmp, Nonce, len_nonces);
	memcpy(tmp+len_nonces, pCreate, len_creates);
	memcpy(tmp+len_nonces+len_creates, pPassWd, PassWdLen);
	
  	if (!GetDigest("SHA1", tmp, PassWdLen+len_nonces+len_creates, sha1, sha1Len))
  	{
  		return false;
  	}


	ResultLen = m_Base64Obj.Encode(sha1, sha1Len, (char *)pResult, 0);

	return true;
}
