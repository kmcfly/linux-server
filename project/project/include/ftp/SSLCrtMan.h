/////////////////////////////////////////////////////////////////////
/*
*数字证书管理。在使用SSL通信时数字证书用于服务器端证明自己的身份、给SSL客户端提供公钥信息。这个公钥用于客户端和服务器端一些重要数据的协商
*功能:	
*		1:初始时生成一个经过CA签名的数字证书。
			初始时，每次都生成私钥文件，其中存放RSA的公钥和私钥，公钥和私钥是SSL生成的，每次都不一样。
			私钥文件要加密保存，加密方式为3DES，加密密钥有用户输入。程序每次使用私钥文件都需要正确的加密密钥。
*		2:能够获取数字证书路径
*		3:能够获取私钥文件路径
*		4:能够获取加密私钥文件的密码路径。
*		SSL服务器初始时需要2，3，4中获取的信息。
*/

#ifndef _SSL_CRT_MAN_H_
#define _SSL_CRT_MAN_H_

#include "SWL_TypeDefine.h"
#include "SSLType.h"

#include <openssl/ssl.h>
#include <openssl/err.h>
#include <stdio.h>
#include <stdlib.h>
#include <openssl/x509.h>
#include <openssl/x509v3.h>
#include <openssl/err.h>
#include <openssl/pem.h>


struct cert_entry
{
	unsigned char key[100];
	unsigned char value[100];
};

#define DAYS_TILL_EXPIRE	365
#define EXPIRE_SECS			(60*60*24*DAYS_TILL_EXPIRE)		//签名证书有效期为一年
#define ENTRY_COUNT			6
#define EXT_COUNT			5
#define PW_MIN_LENGTH		4

const char PASSWORD[]	= "123456";
const char PAS_LEN		= strlen(PASSWORD);

class CSSLCrtMan
{
public:
	static CSSLCrtMan * Instance();
	~CSSLCrtMan();

	bool Initial(const char *pCACertFile=CA_CERT_FILE, const char *pCAKeyFile=CA_KEY_FILE, const char *pCAPass=PASSWORD, unsigned long pasLen=PAS_LEN, const char *pServerReqFile=SERVER_REQ_CERT_FILE, const char *pServerCertFile=SERVER_CERT_FILE, const char *pServerKeyFile=SERVER_KEY_FILE);
	void Quit();
	
	const char * GetServerCrtFilePath();
	const char * GetServerKeyFilePath();
	const char * GetServerKey(unsigned long & keyLen);

private:
	CSSLCrtMan();

private:
	bool CreatePrivateKeyFile(const char *pPrivatKeyFilePath);
	bool CreateCertFile(const char *pPriKeyFile, char *pPass, const char *pCertFilePath);
	bool SignCertFile(const char *pReqCrtFile, const char *pCACrtFile, const char *pCAKeyFile, const char *pCAPsss, unsigned long pasLen);

	bool GenPassword(char *pBuf, unsigned long &bufLen);
	//static int GenrsaCB(int p, int n, BN_GENCB *pCB);
	static int PasswordCallback(char * pBuf, int bufsiz, int a, char * pKey);

private:

	bool	m_bHasInitial;

	static	CSSLCrtMan	*	m_pSSLCrtMan;

	char				*	m_pServerReqCrtFile;			//存放申请的证书路径，这个文件没有经过CA签名处理
	char				*	m_pServerCrtFile;				//存储服务器证书路径
	char				*	m_pServerKeyFile;				//存储服务器私钥密码文件路径
	char					m_serverKey[128];				//加密密钥文件的3des密码
	unsigned long			m_serverKeyLen;

	char				*	m_pCAPassword;					//CA用于签名的密钥

	static	BIO			*	m_pBIOOut;						//错误输出
};
#endif	/*_SSL_CRT_MAN_H_*/
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
