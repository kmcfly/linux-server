#include "HttpDigestAuth.h"


#ifdef __ENVIRONMENT_WIN32__
#ifndef snprintf
#define  snprintf                   _snprintf
#endif
#endif


CHttpDigestAuth::CHttpDigestAuth()
{
	ResetObject();
	m_algorithm = "MD5";
}

CHttpDigestAuth::~CHttpDigestAuth()
{

}

bool CHttpDigestAuth::ResetObject(void)
{
	m_ncount = 0;
	m_bInit = false;

	return true;
}

void CHttpDigestAuth::CvtHex(HASH bin, HASHHEX hex)
{
	unsigned short i;
	unsigned char j;

	for (i = 0; i < HASHLEN; i++)
	{
		j = (bin[i] >> 4) & 0xf;
		if(j <= 9)
		{
			hex[i * 2] = (j + '0');
		}
		else
		{
			hex[i * 2] = (j + 'a' - 10);
		}

		j = bin[i] & 0xf;

		if(j <= 9)
		{
			hex[i * 2 + 1] = (j + '0');
		}
		else
		{
			hex[i * 2 + 1] = (j + 'a' - 10);
		}
	}

	hex[HASHHEXLEN] = '\0';
}

/* calculate H(A1) as per spec */
void CHttpDigestAuth::DigestCalcHA1(const char * pszAlg, const char * pszUserName, const char * pszRealm, const char * pszPassword, const char * pszNonce, const char * pszCNonce, HASHHEX sessionKey)
{
	MD5_CTX md5Ctx;
	HASH ha1;

	MD5_Init(&md5Ctx);
	MD5_Update(&md5Ctx, pszUserName, strlen(pszUserName));
	MD5_Update(&md5Ctx, ":", 1);
	MD5_Update(&md5Ctx, pszRealm, strlen(pszRealm));
	MD5_Update(&md5Ctx, ":", 1);
	MD5_Update(&md5Ctx, pszPassword, strlen(pszPassword));
	MD5_Final((unsigned char*)ha1, &md5Ctx);

#ifdef __ENVIRONMENT_WIN32__
	if(stricmp(pszAlg, "md5-sess") == 0)
#else
	if(strcasecmp(pszAlg, "md5-sess") == 0)
#endif
	{
		MD5_Init(&md5Ctx);
		MD5_Update(&md5Ctx, ha1, HASHLEN);
		MD5_Update(&md5Ctx, ":", 1);
		MD5_Update(&md5Ctx, pszNonce, strlen(pszNonce));
		MD5_Update(&md5Ctx, ":", 1);
		MD5_Update(&md5Ctx, pszCNonce, strlen(pszCNonce));
		MD5_Final((unsigned char *)ha1, &md5Ctx);
	}

	CvtHex(ha1, sessionKey);
}

/* calculate request-digest/response-digest as per HTTP Digest spec */
void CHttpDigestAuth::DigestCalcResponse(HASHHEX ha1, const char * pszNonce, const char * pszNonceCount, const char * pszCNonce, const char * pszQop, const char * pszMethod, const char * pszDigestUri, HASHHEX hEntity, HASHHEX response)
{
	MD5_CTX md5Ctx;
	HASH ha2;
	HASH respHash;
	HASHHEX ha2Hex;

	// calculate H(A2)
	MD5_Init(&md5Ctx);
	MD5_Update(&md5Ctx, pszMethod, strlen(pszMethod));
	MD5_Update(&md5Ctx, ":", 1);
	MD5_Update(&md5Ctx, pszDigestUri, strlen(pszDigestUri));

#ifdef __ENVIRONMENT_WIN32__
	if(stricmp(pszQop, "auth-int") == 0)
#else
	if(strcasecmp(pszQop, "auth-int") == 0)
#endif
	{
		MD5_Update(&md5Ctx, ":", 1);
		MD5_Update(&md5Ctx, hEntity, HASHHEXLEN);
	}

	MD5_Final((unsigned char *)ha2, &md5Ctx);
	CvtHex(ha2, ha2Hex);

	// calculate response
	MD5_Init(&md5Ctx);
	MD5_Update(&md5Ctx, ha1, HASHHEXLEN);
	MD5_Update(&md5Ctx, ":", 1);
	MD5_Update(&md5Ctx, pszNonce, strlen(pszNonce));
	MD5_Update(&md5Ctx, ":", 1);

	if (*pszQop)
	{
		MD5_Update(&md5Ctx, pszNonceCount, strlen(pszNonceCount));
		MD5_Update(&md5Ctx, ":", 1);
		MD5_Update(&md5Ctx, pszCNonce, strlen(pszCNonce));
		MD5_Update(&md5Ctx, ":", 1);
		MD5_Update(&md5Ctx, pszQop, strlen(pszQop));
		MD5_Update(&md5Ctx, ":", 1);
	}

	MD5_Update(&md5Ctx, ha2Hex, HASHHEXLEN);
	MD5_Final((unsigned char *)respHash, &md5Ctx);
	CvtHex(respHash, response);
}

void CHttpDigestAuth::MD5Encrypt(void *pIn, int iLenIn, void *pOut, bool bUpper)
{
	MD5_CTX ctx;	
	MD5_Init (&ctx);
	MD5_Update(&ctx, (unsigned char*)pIn, iLenIn);
	MD5_Final((unsigned char*)pOut, &ctx);
}

void CHttpDigestAuth::SetDigestAuth(const char* pURL, const char* pUser, const char* pPass, const char* pNonce, const char* pRealm, const char* pQop, const char* pAlg)
{
	m_bInit = true;

	m_URL = pURL;
	m_username = pUser;
	m_password = pPass;
	m_nonce = pNonce;
	m_realm = pRealm;
	m_qop = pQop;
	m_algorithm = pAlg;

	m_ncount = 0;
}

bool CHttpDigestAuth::ComputeDigestResponse(const char *pCmd, char *pResponseOut)
{
	HASH respHash;
	char *pTmp = new char[512];

	char *pMD5Pass = new char[64];
	char *pMD5CmdUrl = new char[64];

	//md5(<username>:<realm>:<password>)
	memset(pTmp, 0, 512);
	snprintf(pTmp, 512, "%s:%s:%s", (char *)m_username.c_str(), (char *)m_realm.c_str(), (char *)m_password.c_str());
	MD5Encrypt(pTmp, strlen(pTmp), pMD5Pass, false);

	//md5(<cmd>:<url>)
	memset(pTmp, 0, 512);
	snprintf(pTmp, 512, "%s:%s", pCmd, (char *)m_URL.c_str());
	MD5Encrypt(pTmp, strlen(pTmp), pMD5CmdUrl, false);	

	//md5(md5(<username>:<realm>:<password>):<nonce>:md5(<cmd>:<url>))
	memset(pTmp, 0, 512);
	snprintf(pTmp, 512, "%s:%s:%s", pMD5Pass, (char *)m_nonce.c_str(), pMD5CmdUrl);
	MD5Encrypt(pTmp, strlen(pTmp), (void*)&respHash, false);

	CvtHex(respHash, pResponseOut);

	delete [] pTmp;
	delete [] pMD5Pass;
	delete [] pMD5CmdUrl;
	return true;
}


bool CHttpDigestAuth::GetResponse(const char *pArg, HASHHEX pResponse)
{
	if (!m_bInit)
	{
		return false;
	}

	//return ComputeDigestResponse( pArg, (char*)pResponse);

	//访问次序
	char NonceCount[9] = {0};
	snprintf(NonceCount, 9, "%.8d", ++m_ncount);

	//Cnonce-value
	char Cnonce[64] = {0};
	char pszIn[32] = {0};
	RTSP_GET_IPC_USED_INFO stIpcUtcTime;

	memset( &stIpcUtcTime, 0, sizeof(RTSP_GET_IPC_USED_INFO));
	if( m_pGetIpcUsedInfo && m_pGetIpcUsedParam )
	{
		stIpcUtcTime.cmd = 0;
		//m_pGetIpcUsedInfo( &stIpcUtcTime, m_pGetIpcUsedParam );
	}
	
	if( 0 == stIpcUtcTime.uYear || 0 == stIpcUtcTime.uMonth || 0 == stIpcUtcTime.uDay )
	{
		tm curTm = RTSPGetCurrTime().TM();
		snprintf(pszIn, 24, "%04d-%02d-%02dT%02d:%02d:%02d.%03dZ", 
			curTm.tm_year + 1900, curTm.tm_mon, curTm.tm_mday, 
			curTm.tm_hour, curTm.tm_min, curTm.tm_sec, 0);
	}
	else
	{
		snprintf(pszIn, 24, "%04d-%02d-%02dT%02d:%02d:%02d.%03dZ", stIpcUtcTime.uYear, stIpcUtcTime.uMonth,stIpcUtcTime.uDay, \
			stIpcUtcTime.uHour, stIpcUtcTime.uMointue, stIpcUtcTime.uSeconds, 999 );
	}

	int inSize = strlen(pszIn);
	int outSize = (((inSize)+2) / 3 * 4 + 1);//AV_BASE64_SIZE
	m_Base64.Encode((const unsigned char*)pszIn, inSize, Cnonce, outSize);

	//	//计算response
	HASHHEX ha1;

	DigestCalcHA1("md5", (char *)m_username.c_str(), (char *)m_realm.c_str(), (char *)m_password.c_str(), (char *)m_nonce.c_str(), Cnonce, ha1);
	DigestCalcResponse(ha1, (char *)m_nonce.c_str(), NonceCount, Cnonce, "", (char *)pArg, (char *)m_URL.c_str(), "", pResponse);

	return true;
}

bool CHttpDigestAuth::GetBasicCredentials(const char* pUser, const char* pPass, char* pResult)
{
	if (!pUser || !pPass)
	{
		return false;
	}

	std::string tmp = pUser;
	tmp.append(":");
	tmp.append(pPass);

	long tmpLen = 0;
	tmpLen = m_Base64.Encode((const unsigned char*)tmp.c_str(), tmp.length(), pResult, tmpLen);
	pResult[tmpLen] = '\0';

	return true;
}

//
//bool CHttpDigestAuth::FillDigestAuth(std::string& uri)
//{
//	if(!m_bInit)
//	{
//		return false;
//	}
//
//	std::string strMethod;
//	std::string strText;
//
//	//方法, uri
//	if(uri.find("GET /") == 0)
//	{
//		strMethod = "GET";
//
//		std::string::size_type posNext = uri.find(" HTTP/1.1\r\n");
//		if(std::string::npos != posNext)
//		{
//			std::string::size_type strLen = strlen("GET /");
//			strText = uri.substr(strLen, posNext - strLen);
//		}
//		else
//		{
//			printf("%s %d err\n", __FILE__, __LINE__);
//			return false;
//		}
//	}
//	else if(uri.find("POST /") == 0)
//	{
//		strMethod = "POST";
//
//		std::string::size_type posNext = uri.find(" HTTP/1.1\r\n");
//		if(std::string::npos != posNext)
//		{
//			std::string::size_type strLen = strlen("POST /");
//			strText = uri.substr(strLen, posNext - strLen);
//		}
//		else
//		{
//			printf("%s %d err\n", __FILE__, __LINE__);
//			return false;
//		}
//	}
//	else
//	{
//		printf("%s %d err\n", __FILE__, __LINE__);
//		return false;
//	}
//
//	//访问次序
//	char szNonceCount[9] = {0};
//	snprintf(szNonceCount, 9, "%.8d", ++m_ncount);
//
//	//cnonce-value
//	char pszCnonce[64] = {0};
//	char pszIn[32] = {0};
//	tm curTm = DVRTimeToTm(GetCurrTime());
//	snprintf(pszIn, 16, "%.4d%.2d%.2d%.2d:%.2d:%.2d", 
//			curTm.tm_year + 1900, curTm.tm_mon, curTm.tm_mday, 
//			curTm.tm_hour, curTm.tm_min, curTm.tm_sec);
//
//	int inSize = strlen(pszIn);
//	int outSize = AV_BASE64_SIZE(inSize);
//	av_base64_encode(pszCnonce, outSize, (unsigned char*)pszIn, inSize);
//
//	//计算response
//	HASHHEX ha1;
//	HASHHEX ha2 = "";
//	HASHHEX response;
//
//	DigestCalcHA1(m_algorithm.c_str(), m_username.c_str(), m_realm.c_str(), m_password.c_str(), m_nonce.c_str(), pszCnonce, ha1);
//	DigestCalcResponse(ha1, m_nonce.c_str(), szNonceCount, pszCnonce, m_qop.c_str(), strMethod.c_str(), strText.c_str(), ha2, response);
//
//	//清除结束标记
//	uri.erase(uri.find_last_not_of("\r\n") + 1);
//	uri.append("\r\n");
//
//	//最后添加验证参数
//	uri.append("Authorization: ");
// 
//	uri.append(" Digest username=\"").append(m_username).append("\"");
//	uri.append(", realm=\"").append(m_realm).append("\"");
//	uri.append(", qop=\"").append(m_qop).append("\"");
//	uri.append(", algorithm=\"").append(m_algorithm).append("\"");
//	uri.append(", uri=\"").append(strText).append("\"");
//	uri.append(", nonce=\"").append(m_nonce).append("\"");
//	uri.append(", nc=\"").append(szNonceCount).append("\"");
//	uri.append(", cnonce=\"").append(pszCnonce).append("\"");
//	uri.append(", response=\"").append(response).append("\"");
//
//	uri.append("\r\n\r\n");
//
//	return true;
//}

//end
