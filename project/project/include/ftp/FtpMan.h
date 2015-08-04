////////////////////////////////////////////////////////////////////////////////////
//
/*
*
*FTP管理部分
*检测有哪些数据传输已经结束,如果数据传输结束就销毁使用的资源
*
*/

#ifndef _FTP_MAN_H_
#define _FTP_MAN_H_

#include "FtpPI.h"

const unsigned long MAX_FTP_PI_COUNT = 128;

typedef struct _ftp_pi_info_
{
	CFtpPI		*	pFtpPi;
	SWL_socket_t	sock;
	unsigned long	time;		
}FTP_PI_INFO;

class CFtpMan
{
public:
	static CFtpMan *Instance();
	~CFtpMan();
	
	bool Initial();
	void Quit();

	bool Start();
	void Stop();

	bool AddFtpPI(CFtpPI *pFtpPI);
	int  CurPICount();

private:
	CFtpMan();

	FTP_PI_INFO * FindFtpPi(SWL_socket_t sockfd);

	static	PUB_THREAD_RESULT PUB_THREAD_CALL FtpManProcThread(void *pParam);
	void FtpManProc();

private:
	bool						m_bHasInitial;

	static CFtpMan		*		m_pFtpMan;

	PUB_thread_t				m_ftpManProcID;
	bool						m_bFtpManProc;

	std::list<FTP_PI_INFO *>	m_ftpPIInfoList;
	CPUB_Lock					m_ftpPIInfoListLock;
};

#endif /*_FTP_MAN_H_*/
////////////////////////////////////////////////////////////////////////////////////
