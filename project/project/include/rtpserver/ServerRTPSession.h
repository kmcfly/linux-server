/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : CC
** Date         : 2013-01-14
** Version      : 1.0
** Description  : 处理会话相关信息，包括发送上层传过来的RTP数据、接收RTSP数据
**				: 
** Modify Record:
1:
***********************************************************************/
#ifndef __SERVER_RTP_SESSION__
#define __SERVER_RTP_SESSION__

#include "ServerRTPPacket.h"
#include "RTPSock.h"
#include "ServerRTSP.h"
#include <map>
#include <list>
#ifndef WIN32
#include <sys/time.h>
#include <unistd.h>
#include <time.h>
#include <sys/utsname.h>
#include <openssl/md5.h>
#endif

typedef void (*SIP_CALLBACK)(int ChannelID);
typedef struct
{
	int				SessionID;//区分其他请求媒体的标识
	int				Channel;//通道号
	unsigned long	ClientID;//供PutMsgToServer使用
	int				MessageID;//NET_DATA_SEARCH中的StreamID 以供MessageMan区分回放流
	int				StreamType;//0为现场，1为回放 ,2为下载
	int				PlaybackScale;//2表示2倍速	-2表示0.5倍速
	int 			TransMethod;//UDP TCP
	int				MediaType;//h264为96 98 ，G711为8
	unsigned long	SSRC;
	SWL_socket_t	SocketFD;//若为TCP方式，则把已经建立好的socket的file descriptor传过来
	unsigned short 	RemoteRTPPort;//传输RTP数据的远端端口
	unsigned short 	LocalRTPPort;//传输RTP数据的本地端口
	unsigned short 	RemoteRTCPPort;//传输RTCP数据的远端端口
	unsigned short 	LocalRTCPPort;//传输RTCP数据的本地端口
	unsigned long	RemoteIp;//远端IP，struct sockaddr_in.sin_addr.s_addr格式
	SIP_CALLBACK	pSipCallBack;
}RTPMediaInfo;

class CServerRTPSession
{

	typedef struct _server_rtp_stream_info_
	{
		unsigned long	DiscardPos;
		unsigned long	SSRC;
	}SERVER_RTP_STREAM_INFO;

public:
	CServerRTPSession(struct sockaddr_in clientAddr, SWL_socket_t rtspSockfd);
	CServerRTPSession(RTPMediaInfo *pMediaInfo);
	~CServerRTPSession();


	int Quit();
	bool SetMediaInfo(unsigned short ServerRTPPort);//待加媒体参数，现在默认
	unsigned char GetChannel();
	void GetRTPPacket(CRTPPacket *pPacket, unsigned short streamType);
	int SendRTPPacket();
	int RecvMsg();
	int SendMsg(SWL_socket_t &fd);
	int SendRTCP();

	SWL_socket_t GetSocketFd();
	SWL_socket_t GetRTPSocketFd();
	void SendWaitTimeInc() {m_SendWaitTimes++;}
	void SendWaitTimeClear() {m_SendWaitTimes = 0;}
	unsigned long GetSendWaitTime() {return m_SendWaitTimes;}
	int GetServerRTPPort();
	bool IsSendOverUDP();
	int GetSessionID() {return m_SessionID;}
	unsigned long GetClientID() {return m_ClientID;}
	int GetMessageID() {return m_MessageID;}
	bool IsPlayBack() {return m_bIsPlayBack;}
	int CallCallBack(int channel);
	void SetPause(bool bPause);
	void SetScale(int Scale);

private:
	void Initial();
	int HandleRecvMsg();
	int HandleRTCP();
	bool DeleteOneFrame();
	bool CreateRTPPort(unsigned short ServerPort, unsigned short *ClientPort, unsigned long DestIP, int num);
	bool CreateSR(int type);
	void CreateSDES(char *pBuf, unsigned long &len, unsigned long csrc);
#ifndef WIN32
	unsigned long md_32(char *string, int len);
	unsigned int random32(int type);
#endif

private:
	CPUB_Lock				m_Lock;
	std::map<unsigned short, std::list<CRTPPacket*>*>		m_mapPacket;
	std::map<unsigned short, SERVER_RTP_STREAM_INFO>		m_mapStreamInfo;
	CServerRTSP				*m_pRTSP;
	int						m_SessionID;
	bool					m_bIsPlayBack;
	unsigned long			m_ClientID;
	int						m_MessageID;//NET_DATA_SEARCH中的StreamID 以供MessageMan区分回放流

//媒体信息
	float					m_FrameRate;

//会话信息
	CRTPSock				*m_pUDPSock[MAX_RTSP_MEDIA_COUNT*2];
	CRTPSock				*m_pRTSPSock;
	int						m_VideoType;
	int						m_AudioType;//用-1来指示无音频
	bool					m_bIsUDP;
	unsigned char			m_Channel;

//数据收发
	unsigned char			*m_pRecvBuf;
	unsigned char			m_RecvLen;
	unsigned char			*m_pSendBuf;
	unsigned long			m_SendLen;
//控制相关
	bool					m_bRTSPWaitSend;
	bool					m_bIsStartTransPacket;
	unsigned long			m_RandSeed;
	bool					m_bIsDiscardEnd;
	unsigned long			m_SendWaitTimes;
	bool					m_bIsFirstStart;//控制第一帧为关键帧

//RTCP相关
	RTCP_SENDER_REPORTS		*m_pSR[MAX_RTSP_MEDIA_COUNT];
	unsigned long			m_LastNTPSec[MAX_RTSP_MEDIA_COUNT];
	unsigned long			m_LastNTPMicroSec[MAX_RTSP_MEDIA_COUNT];
	unsigned long			m_LastPacketTimeStamp[MAX_RTSP_MEDIA_COUNT];
	unsigned long			m_SenderPacketCount[MAX_RTSP_MEDIA_COUNT];
	unsigned long			m_SenderOctetCount[MAX_RTSP_MEDIA_COUNT];

	SIP_CALLBACK 			m_pSipCallBack;

//SIP添加
	bool				m_bPause;
	unsigned long		m_LastTime;
	unsigned long		m_ThisTime;
	bool				m_bCHeckMark;
	int					m_Scale;

//for test
FILE					*m_file;
	unsigned char		*m_pTestBuf;
	unsigned long		m_TestBufLen;
	unsigned long		m_TestBufPos;
};

#endif
