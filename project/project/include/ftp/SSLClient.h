//////////////////////////////////////////////////////////////////////////////////////
#ifndef _SSL_CLIENT_H_
#define _SSL_CLIENT_H_

#include "SWL_TypeDefine.h"
#include <openssl/ssl.h>
#include <openssl/err.h>
#include "SSLType.h"


class CSSLClient
{
public:
	CSSLClient();
	~CSSLClient();

	bool Initial();
	void Quit();

	//设置加密套件，SSL支持的加密套件查看头文件SSLType.h
	bool SSLSetCipher(const char *pCipherName);
	//int	 SSLBindSock(SWL_socket_t sockfd);
	int	 SSLConnect(SWL_socket_t sockfd);
	void SSLShutdown();

	int	 SSLRead(void *pBuf, int bufLen);
	int	 SSLWrite(const void * pDataBuf, int dataLen);

protected:
private:
	void FreeRsource();
	int SSL_CTX_use_PrivateKey_file_pass(SSL_CTX *ctx,const char *pFileName,char *pPass);
	static unsigned long GetThreadId();
	static void pthreads_locking_callback(int mode, int type, const char *file, int line);
	
private:
	static unsigned long s_objectCount;
	static CPUB_Lock s_Lock;
	static bool s_bHasInitial;

	//That's the main SSL/TLS structure which is created by a server or client per established connection
	SSL					*m_pSSL;

	X509				*m_pServerCert;
	

};
#endif	/*_SSL_CLIENT_H_*/
//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
