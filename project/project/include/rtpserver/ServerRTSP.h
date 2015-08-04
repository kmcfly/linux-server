/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : CC
** Date         : 2013-01-15
** Version      : 1.0
** Description  : RTSP协议相关，只针对服务器，处理客户端发送来的RTSP消息
** Modify Record:
1:
***********************************************************************/
#ifndef __SERVER_RTSP__
#define __SERVER_RTSP__

#include <string.h>
#include <string>
#include "RTSPHeader.h"
#include "RTPHeader.h"
#include "RTSPStatusCode.h"
#include "RTSP_Common.h"
#include "HttpDigestAuth.h"

const char RTSP_SERVER[] = "Tongwei Video Technology Co.,Ltd.";
const char RTSP_PUBLIC[] = "OPTIONS, DESCRIBE, SETUP, TEARDOWN, PLAY, GET_PARAMETER";

const char RTSP_DATE_DAY_ABBR[][4] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
const char RTSP_DATE_MONTH_ABBR[][5] = {"Jan", "Feb", "Mar", "Apr", "May", "June", "July", "Aug", "Sept", "Oct", "Nov", "Dec"};

const char WWW_AUTHENTICATION_REALM[] = "TVT";
const int RTSP_ROUND_TRIP_TIME = 500000 * 2;

class CServerRTSP
{
public:
	explicit CServerRTSP(int VideoType, int AudioType, int rtpPort, float FrameRate);
	~CServerRTSP();
	int HandleMsg(const char *pMsg, char *pBuf, unsigned long &Len);
	int GetStatus();
	int GetChannel();
	unsigned short GetUDPServerPort();
	unsigned short *GetUDPClientPort();
	bool IsTransOverUDP();
	int GetSetUpCount();

private:
	int AnalyseRequestLine(const char *pRTSPMsgHeader);
	int GetRTSPMethodType(const char *pMethod);
	int HandleOption(const char *pMsg, char *pBuf, unsigned long &Len);
	int HandleDescribe(const char *pMsg, char *pBuf, unsigned long &Len);
	int HandleSetup(const char *pMsg, char *pBuf, unsigned long &Len);
	int HandlePlay(const char *pMsg, char *pBuf, unsigned long &Len);
	int HandleTeardown(const char *pMsg, char *pBuf, unsigned long &Len);

	void CreateRTSPOptionsReplayMsg(unsigned long statusCode, char *pBuf, unsigned long &Len);
	void CreateRTSPDescribeReplayMsg(unsigned long statusCode, char *pBuf, unsigned long &Len);
	void CreateRTSPSetupReplayMsg(unsigned long statusCode, char *pBuf, unsigned long &Len);
	void CreateRTSPTeardownReplayMsg(unsigned long statusCode, char *pBuf, unsigned long &Len);
	void CreateRTSPPlayReplayMsg(unsigned long statusCode, char *pBuf, unsigned long &Len);
	void CreateRTSPCommonMsg(unsigned long statusCode, char *pBuf, unsigned long &Len);

	bool CheckCSeq(const char *pMsg);
	void NonceGenerator();
	bool CheckAuthentication(const char *pMsg, const char *pMethod);
	void CreateAuthenticationMsg(char *pBuf, unsigned long &Len);
	bool FindAccount(const char *pUserName, char *pPassWd);

private:
//rtsp 请求端相关
	char				m_method[20];
	char				m_rtspURL[64];
	char				m_rtspVersion[12];
	

//会话相关
	RTSP_SESSION_STATUS	m_CurSessionStatus;
	unsigned long		m_Cseq;
	unsigned short		m_SetupTimes;
	std::string			m_SessionID;

	int					m_TimeOut;
	RTSP_DATE_TIME		m_LastRequestTime;
	bool				m_bIsUDP;
	unsigned short		m_Port;
	unsigned short		m_UDPServerPort;
	unsigned short		m_UDPPort[4];

//媒体参数相关
	unsigned char		m_Channel;
	int					m_VideoType;
	int					m_AudioType;//用-1来指示无音频
	float				m_FrameRate;

	CPUB_Lock			m_Lock;

//www-authentication
	HASHHEX				m_CurrNonce;
	bool				m_bUseAuthentication;
	char				m_CurUserName[TVT_MAX_TITLE_LEN+4];
	char				m_CurPasswd[TVT_MAX_TITLE_LEN+4];
};

#endif
