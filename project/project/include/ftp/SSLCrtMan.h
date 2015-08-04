/////////////////////////////////////////////////////////////////////
/*
*����֤�������ʹ��SSLͨ��ʱ����֤�����ڷ�������֤���Լ�����ݡ���SSL�ͻ����ṩ��Կ��Ϣ�������Կ���ڿͻ��˺ͷ�������һЩ��Ҫ���ݵ�Э��
*����:	
*		1:��ʼʱ����һ������CAǩ��������֤�顣
			��ʼʱ��ÿ�ζ�����˽Կ�ļ������д��RSA�Ĺ�Կ��˽Կ����Կ��˽Կ��SSL���ɵģ�ÿ�ζ���һ����
			˽Կ�ļ�Ҫ���ܱ��棬���ܷ�ʽΪ3DES��������Կ���û����롣����ÿ��ʹ��˽Կ�ļ�����Ҫ��ȷ�ļ�����Կ��
*		2:�ܹ���ȡ����֤��·��
*		3:�ܹ���ȡ˽Կ�ļ�·��
*		4:�ܹ���ȡ����˽Կ�ļ�������·����
*		SSL��������ʼʱ��Ҫ2��3��4�л�ȡ����Ϣ��
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
#define EXPIRE_SECS			(60*60*24*DAYS_TILL_EXPIRE)		//ǩ��֤����Ч��Ϊһ��
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

	char				*	m_pServerReqCrtFile;			//��������֤��·��������ļ�û�о���CAǩ������
	char				*	m_pServerCrtFile;				//�洢������֤��·��
	char				*	m_pServerKeyFile;				//�洢������˽Կ�����ļ�·��
	char					m_serverKey[128];				//������Կ�ļ���3des����
	unsigned long			m_serverKeyLen;

	char				*	m_pCAPassword;					//CA����ǩ������Կ

	static	BIO			*	m_pBIOOut;						//�������
};
#endif	/*_SSL_CRT_MAN_H_*/
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
