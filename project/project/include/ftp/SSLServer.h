/////////////////////////////////////////////////////////////////////////////////
#ifndef _SSL_SERVER_H_
#define _SSL_SERVER_H_

/*
*ssl 服务器
*功能:	用来接收客户端的ssl连接请求。
*1:	SSL客户端可以选择不同的密钥交换方式、认证方式、数据加密方式和信息摘要方式。
*2:	目前支持的密钥交换方式为RSA、认证方式为RSA。
*3:	数据加密方式可任意选取，可为AES(128,256),Camellia(256),DES(56),3DES(168),
*	SEED(128),IDEA(128),RC2(40),RC4(40,128)。
*	其中括号中为密钥长度，比如AES可以支持128为和256位的密钥。
*	相同的加密算法，密钥长度越长密钥被破解的可能性越小
*4: 信息摘要(MAC)为SHA
*
*/

#include "SWL_TypeDefine.h"

#include <openssl/ossl_typ.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/x509.h>

class CSSLServer
{
public:
	static CSSLServer * Instance();

	~CSSLServer();

	//输入参数为证书文件路径和秘密密钥文件路径
	bool Initial(const char *pServerCrtFile, const char *pServerKeyFile, const char *pServerKey, unsigned long keyLen);
	void Quit();

	bool SetCipher(const char *pCipherName);
	SSL * OnAccept(SWL_socket_t clientTCPSockfd);
	void  OnShutDown(SSL * pClientSSL);	
protected:
private:
	CSSLServer();
private:
	bool					m_bHasInitial;
	static CSSLServer	*	m_pSSLServer;

	SSL_CTX				*	m_psslCTX;
};
#endif	/*_SSL_SERVER_H_*/
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
