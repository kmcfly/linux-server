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

const unsigned long CLIENT_RTP_RECV_BUF_LEN = 256*1024;
const unsigned long CLIENT_RTP_MAX_RECV_LENS = 1536*1024;
const unsigned long CLIENT_RTP_NORMAL_RECV_LEN = 512*1024;
const unsigned long CLIENT_RTCP_RECV_BUF_LEN = 16*1024;
const unsigned long CLIENT_RTCP_SEND_BUF_LEN = 16*1024;
const unsigned char RTP_START_CODE[4] = {0x00, 0x00, 0x00, 0x01};
const unsigned long CLIENT_AUDIO_RECV_LENS = 4*1024;


typedef int (*RTP_CALLBACK)(RTSP_DATA_FRAME *pFrameData, int SeesionID, void *pParam);
typedef int (*PRTP_FUNC_PROC_CALLBACK)(RTSP_FUNC_PROC_CALLBACK_INFO* p_func_info, void *pParam );

class CClientRTP
{
public:
	CClientRTP();
	~CClientRTP();
	
	int Stop();

	bool SetUp(bool bHasVideo, bool bHasAudio, float frameRate, 
		unsigned short width, unsigned short height,
		unsigned int video_payload, unsigned int audio_payload,
		unsigned int audio_data_type );

	int GetRTPData(const char *pData,const unsigned long len, 
		unsigned char InterleavedType, bool bPrevFrameData );
	int GetRTCPData(const char *pData, unsigned long len);
	int SendRTCPData(char *pData, unsigned long &len);
	int SendUnLockRTCPData(char *pData, unsigned long &len);

	void SetCallBack(RTP_CALLBACK pCallBack, void *pParam, int TVTSessionID);
	void SetFuncProcCallback(PRTP_FUNC_PROC_CALLBACK pFuncProcCallBack, void *pParam);

	void HandleConnectionBreak();
	void HandleSystemTimeChanged();

	void SetChannelNumber( int chnn, int sub_chn );
	void TellNextDataIsError( bool bDataErr );
	bool GetIsMustResyncSystem();

	void GetLastRTPHeaderInfo(RTP_HEADER* pRTPInfo );
	int GetCurVideoPayload(){return m_video_payload;}

	unsigned long GetLastError(){return m_ulLastError;}

	void PrintRunPos(){printf("m_run_lines rtp =%d\n", m_run_lines);}

	bool SetSpsPps( const char *pData, int len );

	bool ResetObject(void);

private:
	bool ReAllocBuf(unsigned long CurDataLen, int frame_len_alloc );
	bool ReAllocForDecBuf( int curFrameLen );
	unsigned long HandleRawData(const char *pData,const unsigned long len, 
		unsigned char InterleavedChannel, bool &IsFalse, bool &bIsVideoData );//build frame
	unsigned long HandleRTCP(const char *pData, const unsigned long len, unsigned char InterleavedChannel);
	bool DistributeFrame(int streamType, bool bForceFrame = false );

	void CreateAudioRR(char *pBuf, unsigned long &len);
	void CreateVideoRR(char *pBuf, unsigned long &len);
	void CreateSDES(char *pBuf, unsigned long &len, unsigned long csrc);

	void ResetVideoBufferParamInfo();

private:
	CPUB_Lock				m_Lock;
	bool					m_bIsKeyFrame;
	bool                    m_bIsHasSpsInfo;
	unsigned long           m_iCheckSpsCount;

	bool					m_bSystemTimeChanged[MAX_RTSP_MEDIA_COUNT];

	bool                    m_bMustCurFrameIsKeyframe;

	bool                    m_bSessionTellDataError;


	unsigned long long sa_ulltm;

//����Ͷ�λ
	char					*m_pRTPRecvRam;
	char					*m_pRTPRecvBuf;
	char					*m_pRTPCurPos;
	char					*m_pRTCPRecvBuf;
	unsigned long			m_RTPRecvBufLen;
	//��Ƶ��Framedateʹ�õĿռ�
	char					*m_pRTPARecvRam;
	char					*m_pRTPARecvBuf;
	unsigned long           m_RTPAudioBufLen;
	unsigned long           m_RTPAudioFrameLens;


//����������
	bool					m_bFrameDistributeNeedWait;
	char					*m_pFrameStartPos;//һ֡�Ŀ�ʼλ��
	char					*m_pFrameEndPos;//һ֡�Ľ���λ��

//RTCP��ؼ���
	RTP_HEADER				m_LastRTPHeader[MAX_RTSP_MEDIA_COUNT];
	RTP_HEADER				m_LastButOneRTPHeader[MAX_RTSP_MEDIA_COUNT];
	unsigned long			m_LastPacketTime[MAX_RTSP_MEDIA_COUNT];
	unsigned long			m_LastButOnePacketTime[MAX_RTSP_MEDIA_COUNT];
	unsigned long			m_LastJitter[MAX_RTSP_MEDIA_COUNT];

	RTCP_HEADER				m_RTCPHeader[MAX_RTSP_MEDIA_COUNT];
	RTCP_RECEIVER_REPORTS	m_RR[MAX_RTSP_MEDIA_COUNT];
	
	unsigned long			m_LastRecvSRTimeSec[MAX_RTSP_MEDIA_COUNT];
	unsigned long			m_LastRecvSRTimeUSec[MAX_RTSP_MEDIA_COUNT];
	RTCP_SENDER_REPORTS		m_lastSR[MAX_RTSP_MEDIA_COUNT];

	bool					m_bRTCPSendControl;

//���ݻص�
	RTP_CALLBACK			m_pCallBack;
	void					*m_pParam;
	int						m_TVTSessionID;
	RTSP_DATE_TIME			m_LastFrameTime[MAX_RTSP_MEDIA_COUNT];
	bool					m_bIsFirstDistribute[MAX_RTSP_MEDIA_COUNT];
	unsigned long			m_LastVFrameTs;//��Ƶ��ʱ�����Ϊÿ��������һ������˿��Դ�RTPͷ��ֱ�ӻ�ȡ
	bool					m_bIsVFrameDistribute;
	bool                    m_bForceFrameOnSameTimestamp;

	RTSP_DATE_TIME          m_stSaveResyncTimes;


//��Ƶ��ʽ
	bool					m_bHasAudio;
	bool					m_bHasVideo;
	unsigned short			m_Width;
	unsigned short			m_Height;
	unsigned long			m_VideoTimeInc;

	unsigned int            m_video_payload;
	unsigned int            m_audio_payload;

	unsigned int            m_audio_data_type;


	FILE					*m_file;
	FILE					*m_fileAudio;

	RTSP_DATE_TIME          m_stCheckRtspDatatime;
	RTSP_DATE_TIME          m_stAudioChkFrmTime;

	int                     m_iPrintVideoSyncCnt;
	int                     m_iPrintAudioSyncCnt;

	int                     m_iChannelNum;
	int                     m_subChnnNum;

	bool                    m_bNvrIpcSystimeResync;

	int                     m_iCheckAudioFrameSendContinue;

	//���seq
	bool                    m_bFirstRtpHeader;
	RTP_HEADER              m_stSavePrevRTPHeader;

	unsigned int            m_iReallocBufProcess;

	unsigned int            m_iNonIntervalPackCnt;

	unsigned int            m_iVideoPrintCount;
	unsigned int            m_iAudioPrintCount;

	unsigned int            m_iAudioSpsFrmCnt;

	unsigned long           m_ulLastError;
	unsigned long           m_run_lines;

	char					*m_pSpsPps;
	int						m_SpsPpsLen;

	int                     m_audio_frame_data_len;

	PRTP_FUNC_PROC_CALLBACK   m_pFuncProcCallback;
	void					  *m_pFuncProcParam;

	int                       m_iRtspProcFrameNumber;

};


#endif



