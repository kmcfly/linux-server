/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : CC
** Date         : 2012-12-11
** Version      : 1.0
** Description  : �����ϲ�ģ�鴫������ԭʼ���ݣ�����ɿ��õ�֡����;����RTCP���ݰ����д���
** Modify Record:
1:
***********************************************************************/
#ifndef __CLIENT_RTP__
#define __CLIENT_RTP__

#include "RTPHeader.h"
#include "RTSPHeader.h"
#include "RTSP_Common.h"

const unsigned long CLIENT_RTP_RECV_BUF_LEN = 100*1024;
const unsigned long CLIENT_RTCP_RECV_BUF_LEN = 10*1024;
const unsigned long CLIENT_RTCP_SEND_BUF_LEN = 10*1024;
const unsigned char RTP_START_CODE[4] = {0x00, 0x00, 0x00, 0x01};


typedef int (*RTP_CALLBACK)(RTSP_DATA_FRAME *pFrameData, int SeesionID, void *pParam);

class CClientRTP
{
public:
	CClientRTP();
	~CClientRTP();
	
	int Stop();

	bool SetUp(bool bHasVideo, bool bHasAudio, float frameRate, unsigned short width, unsigned short height);

	int GetRTPData(const char *pData,const unsigned long len, unsigned char InterleavedType);
	int GetRTCPData(const char *pData, unsigned long len);
	int SendRTCPData(char *pData, unsigned long &len);
	void SetCallBack(RTP_CALLBACK pCallBack, void *pParam, int TVTSessionID);
	void HandleConnectionBreak();

private:
	bool ReAllocBuf(unsigned long CurDataLen);
	unsigned long HandleRawData(const char *pData,const unsigned long len, unsigned char InterleavedChannel, bool &IsFalse);//build frame
	unsigned long HandleRTCP(const char *pData, const unsigned long len, unsigned char InterleavedChannel);
	bool DistributeFrame(STREAM_TYPE streamType);

	void CreateAudioRR(char *pBuf, unsigned long &len);
	void CreateVideoRR(char *pBuf, unsigned long &len);
	void CreateSDES(char *pBuf, unsigned long &len, unsigned long csrc);

private:
	bool					m_bIsFirstFrame;
	bool					m_bIsFirstAudio;
	bool					m_bIsKeyFrame;
//����Ͷ�λ
	char					*m_pRTPRecvRam;
	char					*m_pRTPRecvBuf;
	char					*m_pRTPRecvPos;
	char					*m_pRTCPRecvBuf;
	unsigned long			m_RTPRecvBufLen;

//֡��Ϣ
	unsigned short			m_LastPacketRemainLen;


//����������

	bool					m_bCurMarker;
	bool					m_bIsSpsFound;
	bool					m_bIsLastFrameButNeedWait;
	char					*m_pFrameStartPos;//һ֡�Ŀ�ʼλ��
	char					*m_pFrameEndPos;//һ֡�Ľ���λ��

//RTP��RTCP��ص���Ϣ

	CPUB_Lock				m_Lock;
	//RTCP��ؼ���
	RTP_HEADER				m_LastRTPHeader;
	RTP_HEADER				m_LastButOneRTPHeader;
	unsigned long			m_LastPacketTime;
	unsigned long			m_LastButOnePacketTime;
	unsigned long			m_LastJitter;

	RTCP_HEADER				m_RTCPHeader;
	RTCP_HEADER				m_AudioRTCPHeader;
	RTCP_RECEIVER_REPORTS	m_RR;
	RTCP_RECEIVER_REPORTS	m_AudioRR;

	unsigned long			m_LastRecvSRTimeSec;
	unsigned long			m_LastRecvSRTimeUSec;
	RTCP_SENDER_REPORTS		m_lastSR;

	bool					m_bRTCPSendControl;

//���ݻص�
	RTP_CALLBACK			m_pCallBack;
	void					*m_pParam;
	int						m_TVTSessionID;

//��Ƶ��ʽ
	bool					m_bHasAudio;
	bool					m_bHasVideo;
	unsigned short			m_Width;
	unsigned short			m_Height;
	unsigned long			m_VideoTimeInc;
	RTSP_DATE_TIME			m_LastVideoTime;
	RTSP_DATE_TIME			m_LastAudioTime;

//for test
	FILE					*m_file;
	FILE					*m_fileAudio;

};


#endif



