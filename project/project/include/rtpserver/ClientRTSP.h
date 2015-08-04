/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : CC
** Date         : 2012-12-05
** Version      : 1.0
** Description  : RTSP协议相关，只针对客户端，只获取客户端关心的数据
** Modify Record:
1:
***********************************************************************/

#ifndef __CLIENT_RTSP__
#define __CLIENT_RTSP__

#include <string>
#include "RTPSock.h"
#include "RTPHeader.h"
#include "RTSPHeader.h"
#include "HttpDigestAuth.h"
#include "SDPParser.h"

const unsigned long RTP_CLIENT_RTSP_SEND_LEN = 1024*10;
const unsigned long RTP_CLIENT_RTSP_RECV_LEN = 1024*10;
const unsigned char MAX_RTSP_METHOD = 12;
const char RTSP_AUTHENTICATE_DIGEST[] = "Digest";
const char RTSP_AUTHENTICATE_BasicT[] = "Basic";

class CClientRTSP
{
public:
	CClientRTSP();
	~CClientRTSP();
	
	int CreateOptions(const char *pURL, const char *pUserName = NULL, const char *pPassWd = NULL);
	int HandleData(const char *pData, unsigned long len);//接收到session发来的数据
	int GetBufferData(char *pBuffer, unsigned long &len);//外部调用  获取该类buff中的数据
	RTSP_SESSION_STATUS GetStatus() {return m_CurStatus;}
	int GetCurMethod() {int ret; m_Lock.Lock(); ret = m_SendingMethod; m_Lock.UnLock();return ret;}
	int Play();
	int TearDown();
	bool IsSendBufEmpty();
	bool IsWaitingforReply() {bool ret; m_Lock.Lock();ret = m_bIsSending;m_Lock.UnLock();return ret;}
	bool IsHasAudio();
	bool IsHasVideo();
	float GetFrameRate();
	unsigned short GetWidth();
	unsigned short GetHeight();
	unsigned int GetTimeOut() {unsigned int ret;m_Lock.Lock(); ret = m_TimeOut; m_Lock.UnLock(); return ret;}

	int CreateGetParameter();
	int CreateTeardown(char *pData);
	bool IsSupportGetParameter();
private:
	int ParseData(char *pData);

	int CreateDescribe();
	int CreateSetup(STREAM_TYPE streamType);
	int CreateSetparameter();
	int CreatePlay();
	int CreateTeardown();
	int SendMsg(const char *pMsg, unsigned long msgLen);
	int GetStatusCode(char *pStatusCode);
	int GetTransportInfo(char *pRTransportInfo, unsigned long length);

private:
	CSDPParser			m_SDPPaerser;
	CPUB_Lock			m_Lock;
	int					m_SendingMethod;//当前已经发出去的指令
	RTSP_SESSION_STATUS m_CurStatus;
	bool				m_bIsSending;
	bool				m_bISTCPSession;
	bool				m_bISUnicast;
	bool				m_bIsHasAudio;
	bool				m_bIsHasVideo;
	std::string			m_sessionID;

	//数据相关
	char				*m_pRecvBuf;
	char				*m_pSendBuf;
	unsigned long		m_SendDataLen;
	unsigned long		m_recvDataLen;

	//request-line信息
	unsigned long		m_cseq;
	std::string			m_method;
	std::string			m_serverURL;
	std::string			m_rtspVersion;
	std::string			m_SetupRequest[2];//0为视频 1为音频
	unsigned long		m_interleaveChannel;
	std::string			m_agentDes;

	char				m_ContentType[64];
	unsigned int		m_ContentLen;
	unsigned char		m_PacketizationMode;
	unsigned int		m_TimeOut;

	//www-Authenticate相关
	std::string			m_realm;
	std::string			m_nonce;
	std::string			m_mode;//digest
	std::string			m_UserName;
	std::string			m_PassWd;
	std::string			m_Response;
	int					m_ncount;
	CHttpDigestAuth		m_HttpDigestAuth;
	bool				m_bAuthenticate;

	int					m_RequestTime;
	//服务器支持的指令
	char				m_ServerSupportMethod[MAX_RTSP_METHOD][15];
};

#endif
