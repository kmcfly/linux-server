///////////////////////////////////////////////////////////////////////////////////////////////
//
/*
*
*FTP 数据传输处理。data transfer process DTP
*用来发送、接收FTP数据
*接收的数据包括:
*				1 命令的应答，比如ls命令的应答
*				2 文件数据的接收
*发送的数据包括:
*				1 文件数据的发送
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

	CTVTSock		*	m_pTVTSock;					//传输数据的套接口
	CFtpDataProc	*	m_pDataProc;				//数据处理
	CSSLClient		*	m_pSSLClient;				//使用SSL传输数据时m_pSSLControl不为NULL

	unsigned long		m_totalDataLen;				//要接收数据的长度
	unsigned long		m_hadOperateDataLen;		//已经处理的数据长度

	PUB_thread_t		m_ftpDataProcID;
	bool				m_bFtpDataProc;

	CPUB_Lock			m_dataProclock;
	bool				m_bDataProcOver;
};

#endif /*_FTP_DTP_H_*/
///////////////////////////////////////////////////////////////////////////////////////////////
