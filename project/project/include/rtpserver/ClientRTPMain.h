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

const unsigned long RTP_CLIENT_RECV_BUF_LEN = 65535;
const unsigned long RTP_CLIENT_SEND_BUF_LEN = 10*1024;
const unsigned char RTP_MAX_REQUEST_URL_NUM = 16;

class CClientRTPMain
{
	typedef struct _rtp_session_info_
	{
		void				*pCallBackObj;
		int					TVTSessionID;
		std::string			RequestURL;
		CClientRTSPSession	*pSession;
		CRTPSock			*pSock;
		char				*pRecvBuf;
		char				*pRecvPos;
		unsigned long		RecvDataLen;
		char				*pSendBuf;
		char				*pSendPos;
		unsigned long		SendDataLen;
	}RTP_SESSION_INFO;
public:
	static CClientRTPMain *Instance();
	~CClientRTPMain();

	bool Start();
	bool Stop();
	int StartASession(const char *pURL, const char *pUserName, const char *pPassWd, RTP_CALLBACK pCallBack, void *pParam);
	bool StopASession(int TVTSeesionID);

private:
	CClientRTPMain();
	static PUB_THREAD_RESULT PUB_THREAD_CALL RecvThread(void *pParam);
	static PUB_THREAD_RESULT PUB_THREAD_CALL SendThread(void *pParam);

	void RecvProc();
	void SendProc();
	
	int RecvData(RTP_SESSION_INFO *pSessionInfo);
	int RecvAllData();
	int SendData(RTP_SESSION_INFO *pSessionInfo);
	
	void CloseAllSession();
	void CloseASession(RTP_SESSION_INFO *pSessionInf);

private:
	PUB_thread_t		m_hRecvThreadID;
	PUB_thread_t		m_hSendThreadID;

	bool				m_bRecvThreadProc;
	bool				m_bSendThreadProc;

	bool				m_bIsStart;

	fd_set				m_fdSet;
	SWL_socket_t		m_MaxSocketFd;
	CPUB_Lock			m_Lock;

	unsigned char		m_ArraySessionID[RTP_MAX_REQUEST_URL_NUM];

	std::list<RTP_SESSION_INFO*>		m_listSessionInfo;

};

#endif
