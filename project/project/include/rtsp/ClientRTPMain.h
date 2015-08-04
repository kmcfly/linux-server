/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : CC
** Date         : 2012-12-05
** Version      : 1.0
** Description  : RTP/RTSP客户端的管理，向外提供接口
** Modify Record:
1:
***********************************************************************/

#ifndef __RTP_RTSP_CLIENT__
#define __RTP_RTSP_CLIENT__

#include <string>
#include <list>
#include <map>
#include "ClientRTSPSession.h"
#include "RTPSock.h"

const unsigned long RTP_CLIENT_RECV_BUF_LEN = 64*1024;
const unsigned long RTP_CLIENT_SEND_BUF_LEN = RTP_CLIENT_RTSP_SEND_LEN;
const unsigned char RTP_MAX_REQUEST_URL_NUM = 196;

#define RTP_THREAD_MAX_COUNT 1
#define RTP_THREAD_SESSID_NUM 196

#define SESSION_DEL_LIST_MAX_CNT 6


typedef struct _rtp_session_info_
{
	void				*pCallBackObj;
	int					TVTSessionID;
	int                 chnnNumber;
	//std::string			RequestURL;
	CClientRTSPSession	*pSession;
	CRTPSock			*pSock;
	char				*pRecvBuf;
	char				*pRecvPos;
	unsigned long		RecvDataLen;
	char				*pSendBuf;
	char				*pSendPos;
	unsigned long		SendDataLen;

	unsigned long       rtspSendErrorCnt;
}RTP_SESSION_INFO;

class CRtpThreadContext
{
public:

	CRtpThreadContext()
	{	
		FD_ZERO(&m_fdSet);
		m_MaxSocketFd = 0;
		memset(m_ArraySessionID, 0, sizeof(m_ArraySessionID));
		m_bRecvThreadProc = false;
		m_iSessidValueStart = 0;
		m_iSessidMaxCount = 0;
		m_iSpsRecvDataLenToSleep = 0;
	};

	~CRtpThreadContext(){};

	bool				m_bRecvThreadProc;

	fd_set				m_fdSet;
	SWL_socket_t		m_MaxSocketFd;
	CPUB_Lock			m_Lock;

	unsigned char		m_ArraySessionID[RTP_MAX_REQUEST_URL_NUM];

	unsigned int        m_iSessidValueStart;
	unsigned int        m_iSessidMaxCount;

	unsigned int        m_iSpsRecvDataLenToSleep;

	std::list<RTP_SESSION_INFO*>		m_listSessionInfo;

};

class CClientRTPMain
{
public:
	static CClientRTPMain *Instance();
	~CClientRTPMain();

	bool Start();
	bool Stop();
	int StartASession(const char *pURL, const char *pUserName, const char *pPassWd, 
		RTP_CALLBACK pCallBack, void *pParam,PGET_IPC_USED_INFO pIpcUsedInfo,
		int chl_num, int sub_chn, bool bAudioEnable, 
		unsigned long* pErrorCallback, PRTP_FUNC_PROC_CALLBACK pFuncProcCallback );
	bool StopASession(int TVTSeesionID);
	void HandleSystemTimeChanged(int TVTSeesionID);
	bool GetIsMustResyncSystem(int TVTSeesionID);

	void PrintRunPos();


private:
	CClientRTPMain();
	static PUB_THREAD_RESULT PUB_THREAD_CALL RecvThread(void *pParam);
	static PUB_THREAD_RESULT PUB_THREAD_CALL SendThread(void *pParam);

	static bool BindThreadToCpu(int pid, int whichCpu);
	
	void RecvProc();
	void SendProc();
	
	int RecvAllData(CRtpThreadContext* pThreadCon, fd_set &CurSet, int count);

	int RecvData(RTP_SESSION_INFO *pSessionInfo);
	int SendData(RTP_SESSION_INFO *pSessionInfo);
	
	void CloseAllSession(int run_pos);
	void CloseASession( CRtpThreadContext* pThreadCon, RTP_SESSION_INFO *pSessionInf, bool bIsNotifyCaller = true);
	int CloseSockErrorSession(CRtpThreadContext* pThreadCon, fd_set &CurSet, int count);

	RTP_SESSION_INFO* DelListGetItem( bool bGetSmallMem );
	bool DelListInsertItem( RTP_SESSION_INFO* pSessItem );
	
	void DeleteSessionItemMem( RTP_SESSION_INFO* pSessionInf );
	void DeleteAllExistSessionItem();

private:
	PUB_thread_t		m_hRecvThreadID[RTP_THREAD_MAX_COUNT];
	CRtpThreadContext   m_stThreadCon[RTP_THREAD_MAX_COUNT];
	CPUB_Lock           m_thread_index_lock;
	int                 m_thread_index_val;

	PUB_thread_t		m_hSendThreadID;
	bool				m_bSendThreadProc;

	bool				m_bIsStart;
	unsigned long      m_run_lines;
	unsigned long      m_run_recv_lines;


	CPUB_Lock			                m_delListLock;
	RTP_SESSION_INFO*		            m_pdelSessList[SESSION_DEL_LIST_MAX_CNT];

};

#endif
