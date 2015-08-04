///////////////////////////////////////////////////////////////////////////////////////////////
//
/*
*
*FTP ���ݴ��䴦��data transfer process DTP
*�������͡�����FTP����
*���յ����ݰ���:
*				1 �����Ӧ�𣬱���ls�����Ӧ��
*				2 �ļ����ݵĽ���
*���͵����ݰ���:
*				1 �ļ����ݵķ���
*/
#ifndef _FTP_DTP_H_
#define _FTP_DTP_H_

#include "TVTSock.h"
#include "FtpType.h"
#include "FtpDataProc.h"
#include "SSLControl.h"

class CFtpDTP
{
public:
	CFtpDTP();
	~CFtpDTP();

	bool InitialStream();
	bool Initial(const char *pServerIP, unsigned short port, CFtpDataProc *pFtpDataProc, unsigned long totalDataLen = 0, bool bUseSSL = false);
	void Quit();

	bool Start();
	void Stop();

	bool IsDataProcOver();
private:
	static	PUB_THREAD_RESULT PUB_THREAD_CALL FtpDataProcThread(void *pParam);
	void FtpDataProc();

private:
	bool				m_bHasInitial;

	CTVTSock		*	m_pTVTSock;					//�������ݵ��׽ӿ�
	CFtpDataProc	*	m_pDataProc;				//���ݴ���
	CSSLClient		*	m_pSSLClient;				//ʹ��SSL��������ʱm_pSSLControl��ΪNULL

	unsigned long		m_totalDataLen;				//Ҫ�������ݵĳ���
	unsigned long		m_hadOperateDataLen;		//�Ѿ���������ݳ���

	PUB_thread_t		m_ftpDataProcID;
	bool				m_bFtpDataProc;

	CPUB_Lock			m_dataProclock;
	bool				m_bDataProcOver;
};

#endif /*_FTP_DTP_H_*/
///////////////////////////////////////////////////////////////////////////////////////////////
