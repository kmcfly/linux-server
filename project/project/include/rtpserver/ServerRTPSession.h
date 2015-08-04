/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : CC
** Date         : 2013-01-14
** Version      : 1.0
** Description  : ����Ự�����Ϣ�����������ϲ㴫������RTP���ݡ�����RTSP����
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
	int				SessionID;//������������ý��ı�ʶ
	int				Channel;//ͨ����
	unsigned long	ClientID;//��PutMsgToServerʹ��
	int				MessageID;//NET_DATA_SEARCH�е�StreamID �Թ�MessageMan���ֻط���
	int				StreamType;//0Ϊ�ֳ���1Ϊ�ط� ,2Ϊ����
	int				PlaybackScale;//2��ʾ2����	-2��ʾ0.5����
	int 			TransMethod;//UDP TCP
	int				MediaType;//h264Ϊ96 98 ��G711Ϊ8
	unsigned long	SSRC;
	SWL_socket_t	SocketFD;//��ΪTCP��ʽ������Ѿ������õ�socket��file descriptor������
	unsigned short 	RemoteRTPPort;//����RTP���ݵ�Զ�˶˿�
	unsigned short 	LocalRTPPort;//����RTP���ݵı��ض˿�
	unsigned short 	RemoteRTCPPort;//����RTCP���ݵ�Զ�˶˿�
	unsigned short 	LocalRTCPPort;//����RTCP���ݵı��ض˿�
	unsigned long	RemoteIp;//Զ��IP��struct sockaddr_in.sin_addr.s_addr��ʽ
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
	bool SetMediaInfo(unsigned short ServerRTPPort);//����ý�����������Ĭ��
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
	int						m_MessageID;//NET_DATA_SEARCH�е�StreamID �Թ�MessageMan���ֻط���

//ý����Ϣ
	float					m_FrameRate;

//�Ự��Ϣ
	CRTPSock				*m_pUDPSock[MAX_RTSP_MEDIA_COUNT*2];
	CRTPSock				*m_pRTSPSock;
	int						m_VideoType;
	int						m_AudioType;//��-1��ָʾ����Ƶ
	bool					m_bIsUDP;
	unsigned char			m_Channel;

//�����շ�
	unsigned char			*m_pRecvBuf;
	unsigned char			m_RecvLen;
	unsigned char			*m_pSendBuf;
	unsigned long			m_SendLen;
//�������
	bool					m_bRTSPWaitSend;
	bool					m_bIsStartTransPacket;
	unsigned long			m_RandSeed;
	bool					m_bIsDiscardEnd;
	unsigned long			m_SendWaitTimes;
	bool					m_bIsFirstStart;//���Ƶ�һ֡Ϊ�ؼ�֡

//RTCP���
	RTCP_SENDER_REPORTS		*m_pSR[MAX_RTSP_MEDIA_COUNT];
	unsigned long			m_LastNTPSec[MAX_RTSP_MEDIA_COUNT];
	unsigned long			m_LastNTPMicroSec[MAX_RTSP_MEDIA_COUNT];
	unsigned long			m_LastPacketTimeStamp[MAX_RTSP_MEDIA_COUNT];
	unsigned long			m_SenderPacketCount[MAX_RTSP_MEDIA_COUNT];
	unsigned long			m_SenderOctetCount[MAX_RTSP_MEDIA_COUNT];

	SIP_CALLBACK 			m_pSipCallBack;

//SIP���
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
