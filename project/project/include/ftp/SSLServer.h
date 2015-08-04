/////////////////////////////////////////////////////////////////////////////////
#ifndef _SSL_SERVER_H_
#define _SSL_SERVER_H_

/*
*ssl ������
*����:	�������տͻ��˵�ssl��������
*1:	SSL�ͻ��˿���ѡ��ͬ����Կ������ʽ����֤��ʽ�����ݼ��ܷ�ʽ����ϢժҪ��ʽ��
*2:	Ŀǰ֧�ֵ���Կ������ʽΪRSA����֤��ʽΪRSA��
*3:	���ݼ��ܷ�ʽ������ѡȡ����ΪAES(128,256),Camellia(256),DES(56),3DES(168),
*	SEED(128),IDEA(128),RC2(40),RC4(40,128)��
*	����������Ϊ��Կ���ȣ�����AES����֧��128Ϊ��256λ����Կ��
*	��ͬ�ļ����㷨����Կ����Խ����Կ���ƽ�Ŀ�����ԽС
*4: ��ϢժҪ(MAC)ΪSHA
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

	//�������Ϊ֤���ļ�·����������Կ�ļ�·��
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
