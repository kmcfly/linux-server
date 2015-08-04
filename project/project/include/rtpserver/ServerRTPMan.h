/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : CC
** Date         : 2013-01-14
** Version      : 1.0
** Description  : 创建接收和发送客户端消息的线程，并监听客户端的连接请求
**				: 向外提供接口，处理传进来的frameData数据，转换为RTP格式
** Modify Record:
1:
***********************************************************************/
#ifndef __SERVER_RTP_MAN__
#define __SERVER_RTP_MAN__

#include "SWL_ListenProcEx.h"
#include "ServerRTPSession.h"
#include "ServerRTSP.h"
#include "FrameData.h"
#include "ServerRTPPacket.h"
#include "RTSP_Common.h"
#include "MessageMan.h"
#include "NetProtocol.h"
#include "PS_Packet_Packaging.h"

// #ifdef TEST_JRTP
//  #include "rtpsession.h"
//  #include "rtpsessionparams.h"
//  #include "rtptransmitter.h"
//  #include "rtpudpv4transmitter.h"
//  #include "rtpipv4address.h"
//  #include "rtptimeutilities.h"
//  #include "rtppacket.h"
// #endif


class CServerRTPMan
{
public:
	static CServerRTPMan* Instance();
	~CServerRTPMan();

	bool Start();
	bool Stop();
	int StartTransRTP(RTPMediaInfo *pMediaInfo);
	int StopTransRTP(int SessionID);
	int ChangePlaybackScale(int SessionID, int PlaybackScale);//0:暂停  2:快进2倍速 0.5:0.5倍速

	bool Initial(unsigned short MaxConnectNum, unsigned short rtspPort = RTSP_PORT, unsigned short rtpPort = RTP_UDP_START_PORT);

	int GetFrameData(CFrameData *pFrameData, bool IsPlayBack = false);
	void SetAFrameRate(unsigned long frameRate, unsigned short channel);
	void SetVFrameRate(unsigned long frameRate, unsigned short channel);
	
	void dump(const void *p, unsigned long Len);

private:
	CServerRTPMan();
	static int AcceptLinkCallback(CLIENT_SOCK_INFO *pClientSockInfo, void *pParam1, void *pParam2);
	int	OnGetAcceptLink(CLIENT_SOCK_INFO *pClientSockInfo);
	static	PUB_THREAD_RESULT PUB_THREAD_CALL SendThread(void *pParam);
	static	PUB_THREAD_RESULT PUB_THREAD_CALL RecvThread(void *pParam);
	static	PUB_THREAD_RESULT PUB_THREAD_CALL RTCPThread(void *pParam);
	static	PUB_THREAD_RESULT PUB_THREAD_CALL PlayBackThread(void *pParam);
	void SendProc();
	void RecvProc();
	void RTCPProc();
	void PlayBackProc();
	int RecvMsg(fd_set &CurSet,unsigned short ValidNum);
	int SendPacketAndMsg(fd_set &CurSet,unsigned short ValidNum);
	int SendPacket(fd_set &CurSet,unsigned short ValidNum);
	int SendMsg(fd_set &CurSet,unsigned short ValidNum);

	bool AddSession(CServerRTPSession* pServerRTPSession, SWL_socket_t rtspSocketFd, SWL_socket_t rtpSocketFd);
	bool DeleteSession(CServerRTPSession* pServerRTPSession);

	inline int FrameDataToRTPPacket(CFrameData *pFrameData);
	inline int FrameDataToRTPPacketRefBy3550(CFrameData *pFrameData, bool IsPlayBack = false);//直接使用RFC3550的方式打包
	bool PreAllocPacLst(unsigned char channel);
	bool RellocPacLst(unsigned char Channel);
	bool IsStartCode(const unsigned char *pData);
	const unsigned char *NextStartCode(const unsigned char *pData, unsigned long dataLen);

	int CreateRFC3550Packet(unsigned short channel, const unsigned char *pRTPData, unsigned long dataLen, bool bIsKeyFrame, unsigned long TimeStamp, bool mark, unsigned short payloadType = RTP_PAYLOAD_TYPE_H264, bool bIsPaused = false);
	int CreateRTPPacket(unsigned short channel, const unsigned char *pRTPData, unsigned long dataLen, bool bIsKeyFrame, unsigned long TimeStamp);
	int CreateRTPspsPacket(unsigned short channel, const unsigned char *pRTPData, unsigned long dataLen, bool bIsKeyFrame, unsigned long TimeStamp);
	int CreateRTPFUPacket(unsigned short channel, const NALU_HEADER *pNaluHeader, const unsigned char *pNaluData, unsigned long dataLen, bool bIsKeyFrame, unsigned long TimeStamp);
	int CreateAudioPacket(unsigned short channel, const unsigned char *pData, unsigned long dataLen, unsigned long TimeStamp);
	void DistributePacket(CRTPPacket *pPacket, unsigned long channel);
	bool SaveFrameData(CFrameData * pFrameData, unsigned short channel);	
	void CheckFrameRate(CFrameData * pFrameData);
	void ReleasePacket(unsigned char channel);
	void CloseAllSession();
	unsigned short GetRTPServerPort();
	void ClearRTPServerPort(unsigned short port);
	int SundaySearch(char *pSrc, char *pPattern, unsigned long PatternLen, unsigned long SrcLen);
	int PutMsgToServer(unsigned long ClientID, int StreamID, unsigned long DataType);
	bool GetSessionInfo(unsigned short channel, unsigned long &ClientID, int &StreamID);

private:
//初始化相关
	bool				m_bHasInitial;
	unsigned short		m_RTSPPort;
	unsigned short		*m_pRTPPort;//保存UDP方式的服务端的端口号
	unsigned short		m_RTPStartPort;
	unsigned short		m_MaxConnectNum;
	unsigned short		m_VideoWidth;
	unsigned short		m_VideoHeight;

	
//线程相关
	CSWL_ListenProcEx	*m_pListenProc;
	PUB_thread_t		m_hSendThreadID;
	PUB_thread_t		m_hRecvThreadID;
	PUB_thread_t		m_hRTCPThreadID;
	PUB_thread_t		m_hPlayBackThreadID;
	bool				m_bSendThreadProc;
	bool				m_bRecvThreadProc;
	bool				m_bRTCPThreadProc;
	bool				m_bPlayBackProc;
	CPUB_Lock			m_Lock;

//socket
	fd_set				m_FdSetRTSP;
	SWL_socket_t		m_MaxFdRTSP;

	fd_set				m_FdSetRTP;
	SWL_socket_t		m_MaxFdRTP;

	unsigned long		m_SendTimeOutNum;

//数据以及通道相关
	std::list<CFrameData*>								m_listFrameData;
	std::list<CServerRTPSession*>						m_listSession;
	std::map<unsigned long, std::list<CRTPPacket*>*>	m_mapRTPPacket;//map 的键值是通道号
	std::map<unsigned long, std::list<CRTPPacket*>*>	m_mapRTPPacketFree;
	unsigned long		m_FrameDataLimitSize[MAX_STREAM_NUM];
	unsigned short		m_ASequenceNum[MAX_STREAM_NUM];
	unsigned short		m_VSequenceNum[MAX_STREAM_NUM];
	RTSP_DATE_TIME		m_LastFrameDateTime[MAX_RTSP_MEDIA_COUNT][MAX_STREAM_NUM];
	unsigned long		m_LastFrameTs[MAX_RTSP_MEDIA_COUNT][MAX_STREAM_NUM];
	bool				m_bRequestedThisChannel[MAX_STREAM_NUM];
	
//回放控制
	bool				m_bIsPackedH264[MAX_STREAM_NUM];//use pack method according to RFC 3984 or RFC6184
	int					m_Scale[MAX_CH_NUM];
	LONGLONG			m_LastFrameTime[MAX_CH_NUM];
	LONGLONG			m_ThisFrameTime[MAX_CH_NUM];
	unsigned long		m_FrameCountPerMsg[MAX_CH_NUM];
	unsigned long		m_LastFrameIndex[MAX_CH_NUM];
	RTSP_DATE_TIME		m_LastMsgTime[MAX_CH_NUM];
	int					m_StarSpeedControl[MAX_CH_NUM];//刚启动时控制发送快点
	bool				m_bIsPaused[MAX_CH_NUM];
//
	FILE				*m_file;
	unsigned char		*m_pTestBuf;
	unsigned long		m_TestBufLen;
	unsigned long		m_TestBufPos;

	
	CPSPackaging *m_pPsPackt;

//国标ps打包 加个缓冲缓存音频  当前只加一个通道
	char				*m_pPsBuf;
	unsigned long		m_pPsBufLen;

	//jrtplib::RTPSession			m_RTPSession;
};


#endif

