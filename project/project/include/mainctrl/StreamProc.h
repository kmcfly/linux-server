/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : yuanshiwei
** Date         : 2009-05-27
** Name         : StreamProc.h
** Version      : 1.0
** Description  :
** Modify Record:
1:
***********************************************************************/
#ifndef _STREAM_PROC_H_
#define _STREAM_PROC_H_
#include <list>
#include "LocalDevice.h"
#include "RecordMan.h"
#include "StreamRecordManEx.h"
#if defined (__ENVIRONMENT_LINUX__) && defined (__DVR_BASIC__)
#include "SmtpMan.h"
#endif
#include "MessageMan.h"

#if defined(__DVD_BURN_REC__) && defined (__ENVIRONMENT_LINUX__)
#include "DispatcherMan.h"
#endif

#ifdef __DVR_ULTIMATE__
#include "NetDeviceManager.h"
#endif

#ifdef __DVR_ULTIMATE__
typedef struct _buffer_frames
{
	ext_dvr_ip_in_buffer_param		bufferInfo;
	std::list<CFrameData *>			*pFrameList;
}BUFFER_FRAMES;

typedef struct _chip_decode_param
{
	long				busyCount;
}CHIP_DECODE_PARAM;
#endif

class CStreamProc
{
public:
	CStreamProc();
	~CStreamProc();

	bool Initial(unsigned char videoInputNum, unsigned char netVideoInputNum, unsigned char audioInputNum, \
				CReclogManEX *pReclogMan, CRecordMan *pRecMan ,CStreamRecordManEx *pStreamRecManEx);
	void Quit();

	bool Start();
	void Stop();
    	
	void Action(bool bAction);
	void ClearData();

	void SetSecondStreamEncode(unsigned char chnn,  const ENCODE_INFO & encode);
	void SetSecondStreamAuto(bool bAuto);
	void ChangeSecondStreamCH(ULONGLONG CH, ULONGLONG CH_mobile, unsigned long secondAllSendNum, ULONGLONG mobileHigh);
	void SetSecondSteamRateLimit(unsigned long limitRate);

	void SecondStreamMaxRateForFirstStreamNum(unsigned long limitRateForFirstNum, unsigned long limitAllRateForFirstNum);

	void SetFirstStreamEncode(unsigned char chnn, const ENCODE_INFO_EX & encode);
	void SetFirstStreamLimit(unsigned long limitFrameRate, unsigned long limitBitRate);

	//开启关闭对讲
	bool StartTalk();
	void WouldStopTalk();
	void StopTalk();

	void SetNetVideoFRate(unsigned char chnn, long frameRate, unsigned long bitRate);
protected:
private:
	//
	static	RESULT WINAPI StreamThread(LPVOID lpParameter);
	void StreamProc();
	
	static RESULT WINAPI SecondStreamThread(LPVOID lpParameter);
	void SecondStreamProc();

	static RESULT WINAPI Net_StreamThread(LPVOID lpParameter);
	void Net_StreamProc();

	static RESULT WINAPI Net_SecondStreamThread(LPVOID lpParameter);
	void Net_SecondStreamProc();

	static RESULT WINAPI Net_MinorStreamThread(LPVOID lpParameter);
	void Net_MinorStreamProc();

	static RESULT WINAPI Net_JPEGStreamThread(LPVOID lpParameter);
	void Net_JPEGStreamProc();

	static RESULT WINAPI TalkStreamThread(LPVOID lpParameter);
	void TalkStreamProc();

#ifdef __DVR_BASIC__
	static RESULT WINAPI JPEGStreamThread(LPVOID lpParameter);
	void JPEGStreamProc();
#endif

	void ReleaseFrameInfo(unsigned char chnn);
	void ReleaseSecondFrameInfo( unsigned char chnn );
#ifdef __DVR_BASIC__
	void ReleaseJPEGFrameInfo(unsigned char chnn);
#endif
#ifdef __DVR_ULTIMATE__
	void ReleaseFrameInfoNet(unsigned char decCard);
	void ReleaseFrameInfoNetMinor(unsigned char decCard);
	void ReleaseFrameInfoNetSecond();
	void ReleaseFrameInfoNetJpeg();
	void ChangeRecStatusForNetStatus(ULONGLONG &ullDisconnectCH);
#endif
	void ReleaseTalkFrame();

	void SendAVFormat(unsigned char chnn, bool bAudio = false);

	void SetSecondStreamEncodeAttrib();

	LONGLONG GetNetRelativeTime(LONGLONG & lgmTime, LONGLONG & lrTime, LONGLONG ngmTime);
	//////////////////////////////////////////////////////////////////////////
	typedef struct _video_info
	{
		unsigned short width;
		unsigned short height;
	}VIDEO_INFO;

	//////////////////////////////////////////////////////////////////////////
	bool			*m_pbSendAudioFormat;		//表示已经发送音频格式

	bool			m_bStream;					//主码流线程运行标记（包括音频流）
	PUB_thread_t	m_stream_ID;				//主码流线程ID
	bool            m_bStreamAction;
	CPUB_Lock       m_streamLock;

	bool			m_bSecondStream;			//子码流线程运行标记
	PUB_thread_t	m_hSecondStreamID;			//子码流线程ID
	bool            m_bSecondStreamAction;
	CPUB_Lock       m_secondStreamLock;

#ifdef __DVR_BASIC__
	bool			m_bJPEGStream;			    //
	PUB_thread_t	m_hJPEGStreamID;			//
	bool            m_bJPEGStreamAction;
	CPUB_Lock       m_JPEGStreamLock;
#endif

	bool			m_bStreamNet;				//网络码流线程运行标记（包括音频流）
	PUB_thread_t	m_streamNet_ID;				//网络码流线程ID
	bool            m_bNetStreamAction;
	CPUB_Lock       m_netStreamLock;
	bool			m_bNetMinorStreamAction;
	CPUB_Lock		m_netMinorStreamLock;

	bool			m_bStreamNetSecond;
	PUB_thread_t	m_streamNetSecond_ID;

	bool			m_bStreamNetMinor;
	PUB_thread_t	m_streamNetMinor_ID;

	bool			m_bJPEGNetStream;			    //
	PUB_thread_t	m_hJPEGNetStreamID;			//
	bool            m_bJPEGNetStreamAction;
	CPUB_Lock       m_JPEGNetStreamLock;

	bool			m_bTalkStream;				//对讲线程运行标记
	PUB_thread_t	m_streamTalkID;				//对讲线程ID
	bool			m_bWouldStopTalk;			//true表示将要关闭对讲，不再抓取对讲数据
	CPUB_Lock       m_talkStreamLock;
	
	VIDEO_INFO		*m_pVideoInfoMain;
	VIDEO_INFO		*m_pVideoInfoSub;
	//////////////////////////////////////////////////////////////////////////
	unsigned char	m_videoInputNum;
	unsigned char	m_netVideoInputNum;
	unsigned char	m_localVideoInputNum;
	unsigned char	m_audioInputNum;
	CLocalDevice	*m_pLocalDevice;
#ifdef __DVR_ULTIMATE__
	CNetDeviceManager *m_pNetDeviceMan;
#endif
	CRecordMan		*m_pRecMan;
	CStreamRecordManEx *m_pStreamRecManEx;
#if defined(__DVD_BURN_REC__) && defined (__ENVIRONMENT_LINUX__)
	CDispatcherMan	*m_pDvdWriteMan;
#endif
	CMessageMan		*m_pMsgMan;

	bool			m_bSecondStreamAuto;
	ULONGLONG		m_secondStreamCH;
	ULONGLONG		m_secondStreamCH_Mobile;
	unsigned long   m_secondSendAllNum;
	unsigned long   m_secondRateLimit;
	ENCODE_INFO		*m_pSecondStreamEncodeInfo;
	ENCODE_INFO_EX	*m_pFirstStreamEncodeInfo;
	//////////////////////////////////////////////////////////////////////////
	std::list<CFrameData *>	*m_pFrameList;				//主码流
	std::list<CFrameData *> *m_pSecondFrameList;		//子码流
	std::list<CFrameData *> m_talkFrameList;			//对讲数据
#ifdef __DVR_BASIC__
	std::list<CFrameData *> *m_pJPEGFrameList;			//jpeg码流
#endif

#ifdef __DVR_ULTIMATE__
	std::list<BUFFER_FRAMES>	*m_pBufferFramesList;
	std::list<BUFFER_FRAMES>	*m_pMinorBufferFramesList;
	std::list<BUFFER_FRAMES>	m_secondBufferFramesList;
	std::list<BUFFER_FRAMES>	m_jpegBufferFramesList;
	unsigned char		m_decCardNum;
	//////////////////////////////////////////////////////////////////////////
	BITMAPINFOHEADER	*m_pBitmapInfo;
	WAVEFORMATEX		*m_pWaveFormat;
	BITMAPINFOHEADER	*m_pMinorBitmapInfo;
	WAVEFORMATEX		*m_pMinorWaveFormat;
	BITMAPINFOHEADER	*m_pSecondBitmapInfo;

	long				*m_pFrameRate;
	LONGLONG			*m_pAudioFrameTime;
	bool				*m_pbFrameRateChanged;
	long				*m_pVORate;
#endif

	unsigned long   m_secondLimitRateForFirstStreamNum;
	unsigned long   m_secondLimitAllRateForFirstStreamNum;
//zxx新加
	public:
		bool StartHxhtTalk();
		void WouldStopHxhtTalk();
		void StopHxhtTalk();
	private:
		static RESULT WINAPI TalkHxhtThread(LPVOID lpParameter);
		void TalkHxhtProc();
	private:
		bool m_haveHxht;//互信互通线程标记
		bool m_bHxhtWouldStop;
		CPUB_Lock  m_talkHxhtLock;
		PUB_thread_t	m_haveHxhtID;

	LONGLONG	m_nmgmTime;
	LONGLONG	m_nmrelativeTime;
	LONGLONG	m_nsgmTime;
	LONGLONG	m_nsrelativeTime;
};
#endif //_STREAM_PROC_H_
