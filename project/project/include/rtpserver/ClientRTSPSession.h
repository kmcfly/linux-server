/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : CC
** Date         : 2012-12-05
** Version      : 1.0
** Description  : ����RTSP�Ự��أ�����RTSP�Ự�Ľ�����RTP��RTCP���ݵĽ��գ�RTCP���ķ���
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

	//���ϲ�ģ���ȡ���ݲ��ַ�
	int GetData(const char* pData, unsigned long len);

	//���²�ģ��ȡ������
	int SendData(char* pData, unsigned long &len);
	int SendTearDown(char *pData);

	//���Ƴ�ʱ
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

