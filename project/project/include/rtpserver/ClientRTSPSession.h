/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : CC
** Date         : 2012-12-05
** Version      : 1.0
** Description  : 整个RTSP会话相关，包括RTSP会话的建立，RTP和RTCP数据的接收，RTCP包的发送
** Modify Record:
1:
***********************************************************************/
#ifndef __CLIENT_RTSP_SESSION__
#define __CLIENT_RTSP_SESSION__

#include <string>
#include "RTPHeader.h"
#include "RTSPHeader.h"
#include "ClientRTSP.h"
#include "ClientRTP.h"

class CClientRTSPSession
{
public:
	CClientRTSPSession();
	~CClientRTSPSession();
	
	int ParseURL(char const* url, char* username, char* password, char* address,int &portNum);
	int Start(const char *pURL, const char *pUserName, const char *pPassWd);
	int Stop();

	std::string GetSessionID() {return m_SeesionID;}
	void SetCallBack(RTP_CALLBACK pCallBack, void *pParamm, int TVTSessionID);
	void HandleConnectionBreak();

	//从上层模块获取数据并分发
	int GetData(const char* pData, unsigned long len);

	//从下层模块取得数据
	int SendData(char* pData, unsigned long &len);
	int SendTearDown(char *pData);

	//控制超时
	unsigned long GetRecvWaitTime() {return m_RecvWaitTime;}
	void RecvWaitTimeInc() {m_RecvWaitTime++;}
	void ClearRecvWaitTime() {m_RecvWaitTime = 0;}

private:
	void Initial();
	int FindString(const char *pSrc, const char *pPattern, const int dataLen);

private:
	std::string			m_SeesionID;
	bool				m_bHasSetRTP;

	CClientRTSP			m_RTSP;
	CClientRTP			m_RTP;

	CPUB_Lock			m_Lock;

	RTSP_DATE_TIME		m_LastGetParaTime;
	RTSP_DATE_TIME		m_LastRTCPTime;

	unsigned long		m_RecvWaitTime;
};

#endif

