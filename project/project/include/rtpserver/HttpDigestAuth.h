#ifndef _HTTP_DIGEST_AUTH_H_
#define _HTTP_DIGEST_AUTH_H_

#include <string>
#include "Base64.h"
#include "openssl/md5.h"
#include <assert.h>
#include <stdio.h>
#include "RTSP_Common.h"

#define HASHLEN				16
typedef char				HASH[HASHLEN];
#define HASHHEXLEN			32
typedef char				HASHHEX[HASHHEXLEN + 1];

class CHttpDigestAuth
{
public:
	CHttpDigestAuth();
	virtual ~CHttpDigestAuth();

	//bool FillDigestAuth(std::string& uri);
	void SetDigestAuth(const char* pURL, const char* pUser, const char* pPass, const char* pNonce, const char* pRealm, const char* pQop, const char* pAlg);
	
	void SetURL(const char* pURL) {m_URL = pURL;}
	void SetUsername(const char* pUser) {m_username = pUser;};
	void SetPassword(const char* pPass) {m_password = pPass;};
	void SetNonce(const char* pNonce) {m_nonce = pNonce;};
	void SetRealm(const char* pRealm) {m_realm = pRealm;};
	void SetQop(const char* pQop) { m_qop = pQop;};
	void SetAlgorithm(const char* pAlg = "MD5") {m_algorithm = pAlg;};
	void SetInit(bool bInit = true) { m_bInit = true;};

	bool GetResponse(const char *pArg, HASHHEX pResponse);

private:
	/* calculate H(A1) as per HTTP Digest spec */
	void DigestCalcHA1( const char * pszAlg,
		const char * pszUserName,
		const char * pszRealm,
		const char * pszPassword,
		const char * pszNonce,
		const char * pszCNonce,
		HASHHEX sessionKey);

	/* calculate request-digest/response-digest as per HTTP Digest spec */
	void DigestCalcResponse(HASHHEX ha1,           /* H(A1) */
		const char * pszNonce,       /* nonce from server */
		const char * pszNonceCount,  /* 8 hex digits */
		const char * pszCNonce,      /* client nonce */
		const char * pszQop,         /* qop-value: "", "auth", "auth-int" */
		const char * pszMethod,      /* method from the request */
		const char * pszDigestUri,   /* requested URL */
		HASHHEX hEntity,       /* H(entity body) if qop="auth-int" */
		HASHHEX response);     /* request-digest or response-digest */

	void CvtHex(HASH bin, HASHHEX hex);


protected:
	std::string		m_nonce;	//从服务端返回中提取
	std::string		m_realm;	//从服务端返回中提取
	std::string		m_qop;
	std::string		m_algorithm;
	std::string		m_username;
	std::string		m_password;
	std::string		m_URL;
	int				m_ncount;
	bool			m_bInit;

	CBase64			m_Base64;
};

#endif
//end
