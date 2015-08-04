////////////////////////////////////////////////////////////////////
#ifndef _SSL_CONTROL_H_
#define _SSL_CONTROL_H_

#include "SWL_TypeDefine.h"

#include <openssl/ssl.h>
#include <openssl/err.h>

class CSSLControl
{
public:
	CSSLControl();
	~CSSLControl();

	bool Initial();
	void Quit();

	int	 SSLBindSock(SWL_socket_t sockfd);
	int	 SSLConnect();
	void SSLShutdown();

	int	 SSLRead(void *pBuf, int bufLen);
	int	 SSLWrite(const void * pDataBuf, int dataLen);
protected:
private:
	void FreeRsource();
private:
	static bool			m_bHasInitial;
	static unsigned long m_objectCount;

	SSL_CTX			*	m_pSSLCTX;
	SSL				*	m_pSSL;
	X509			*	m_pServerCert;
	BIO				*	m_pBIO;
};
#endif  /*_SSH_CONTROL_H_*/
////////////////////////////////////////////////////////////////////
