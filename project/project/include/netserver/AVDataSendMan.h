// AVDataSendMan.h: interface for the CAVDataSendMan class.
//
//////////////////////////////////////////////////////////////////////
/******************************************************************************
*	说明：
*	
*	回放及备份的音频视频数据统一存放在一个列表中，网络现场的数据音频视频分开存放，
*	且每个通道占用一个列表。在发送时，优先发送网络现场的音频，接着是网络现场的视
*	频，再接着是回放的数据，最后是备份的数据。
*	
******************************************************************************/

#if !defined(AFX_AVDATASENDMAN_H__2C684FC4_E768_43BE_B961_2E13790DDB6C__INCLUDED_)
#define AFX_AVDATASENDMAN_H__2C684FC4_E768_43BE_B961_2E13790DDB6C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "PUB_common.h"
#include "SWL_MultiNetComm.h"

class CFrameData;
class CFrameBuffer;

#if defined(__DVR_ULTIMATE__)
const long COMBINED_DATA_SPLIT_LEN = 128*1024;											//把帧数据分成大小为多少的小块
#elif defined(__PACKID_9__)
const long COMBINED_DATA_SPLIT_LEN = 128*1024;											//把帧数据分成大小为多少的小块
#elif defined(__PACKID_14__) || defined(__PACKID_27__)
const long COMBINED_DATA_SPLIT_LEN = 448*1024;											//把帧数据分成大小为多少的小块
#else
const long COMBINED_DATA_SPLIT_LEN = 128*1024;											//把帧数据分成大小为多少的小块
#endif
const long SUB_PACK_HEAD_LEN = sizeof(NET_COMBINED_DATA) + sizeof(PACKED_DATA_FLAG);//每个分包的头部长度
const long SUB_PACK_LEN = COMBINED_DATA_SPLIT_LEN + SUB_PACK_HEAD_LEN;				//每个分包的总大小

#define		ALL_CHANNELS		0xffffffff	//全部通道

#define		CLIENT_VIDEO_DATA	0	//视频数据
#define		CLIENT_AUDIO_DATA	1	//音频数据
#define		CLIENT_ALL_DATA		2	//音频和视频数据

typedef struct _client_video_info
{
	ULONGLONG 			ullVideobit;		//最多支持64个,从低位开始，每位表示一个通道
	ULONGLONG			ullVideoFormat;		//是否发了视频格式
	ULONGLONG			ullIFrame;			//是否发了I帧
}CLIENT_VIDEO_INFO;

typedef struct _client_audio_info
{
	ULONGLONG			ullHasAudioFormat;		//是否发了音频格式
	ULONGLONG 			ullAudioChannelbit;		//假设最多支持64个，从低位开始，每位表示一个通道
}CLIENT_AUDIO_INFO;

typedef struct _client_stream_info
{
	unsigned long			dwStreamID;			//流ID，发送的帧数据附带此信息，来区别是哪个流的
	unsigned short			bLiveStream;		//是不是现场数据
	unsigned short			bFirstFrame;		//回放或备份时，该客户端每通道是否刚开始请求数据

	union
	{
		CLIENT_VIDEO_INFO	fastVideoInfo;
		CLIENT_VIDEO_INFO	playbackInfo;
		CLIENT_VIDEO_INFO	jpegInfo;
	};
	
	CLIENT_VIDEO_INFO		slowVideoInfo;
	CLIENT_AUDIO_INFO		audioInfo;

	CFrameBuffer			**ppAudioDataList;	//音频数据list
	CFrameBuffer			**ppVideoDataList;	//视频数据list

	unsigned long			dwSendVideoIndex;	//发到几个通道的视频了
	unsigned long			dwLastDataID;		//取数据时，最后一次取数据的ID（GetFrame时获得的）

	NET_COMBINED_DATA		combinedData;		//发送组合数据时用
	NET_COMBINED_INFO		combinedInfo;		//记录要发送的组合数据

	unsigned long			vDataToSendLen;		//上次发送失败，现待发送的视频数据长度
	unsigned char			szVSendData[SUB_PACK_LEN];	//用于直接发送视频数据的缓冲区

	unsigned long			aDataToSendLen;		//上次发送失败，现待发送的音频数据长度
	unsigned char			szASendData[SUB_PACK_LEN];	//用于直接发送音频数据的缓冲区

//	unsigned long			oDataToSendLen;		//上次发送失败，现待发送的其他数据长度
//	unsigned char			szOSendData[SUB_PACK_LEN];	//用于直接发送其他数据的缓冲区
}CLIENT_STREAM_INFO;

typedef enum _send_frame_error
{
	SEND_FRAME_SUCC,
	SEND_FRAME_NEED_FORMAT,
	SEND_FRAME_NEED_IFRAME
}SEND_FRAME_ERROR;

class CAVDataSendMan
{
public:
	CAVDataSendMan(long clientID, CSWL_MultiNet *pMultiNetComm, unsigned long dwMaxVideo, unsigned long dwMaxAudio);
	virtual ~CAVDataSendMan();

	//开始一个码流
	void StartStream(bool bLiveStream, ULONGLONG ullFastVideobit,
		ULONGLONG ullSlowVideobit, ULONGLONG ullAudioChannelbit, unsigned long dwStreamID);

	//改变某个码流需要的接收的视音频通道
	void ChangeStream(ULONGLONG ullFastVideobit, ULONGLONG ullSlowVideobit,
		ULONGLONG ullAudioChannelbit, unsigned long dwStreamID);
	//停止某个码流
	void StopStream(unsigned long dwStreamID);

	//停止所有码流
	void StopAllStream();

	//把视频数据放入发送列表
	long PutVideoData(CFrameData *pFrameData, bool bLive = true);

	//把音频数据放入发送列表
	long PutAudioData(CFrameData *pFrameData, bool bLive = true);


	//把事件数据加放入发送列表
	long PutOtherData(CFrameData *pFrameData);

	//清除网络模块中未发送完的现场数据
	int	CleanLiveData(unsigned long dwStreamID, unsigned long dwDataType , unsigned long dwLiveType,
		unsigned long dwChannel = ALL_CHANNELS, bool bCleanFormat = true);

	int CleanPBData(unsigned long dwStreamID);

	//以适当的发送策略发送现场音频视频数据及回放数据
	int SendAVData();

	//得到一帧或一分包数据
	int GetAVData(unsigned char *pDestBuf, long bufSize, unsigned char *&pDataHead);

	//得到当前指定通道需要发送的数据总大小
	ULONGLONG GetStreamArrivedBytes(unsigned long channel, unsigned long frameType);

	//得到当前指定通道已发送的数据总大小
	ULONGLONG GetStreamSentBytes(unsigned long channel, unsigned long frameType);

	//得到统计数据的时间跨度
	LONGLONG GetStatisticTime(unsigned long channel);

	//重新统计以上两个量,-1表示重置所有通道的统计
	void ResetStreamBytesStatistics(unsigned long channel = ALL_CHANNELS);

	//对讲码流操作
	void StartTalkStream();
	void StopTalkStream();
	long PutTalkData(CFrameData *pFrameData);

	//JPEG流操作
	void StartJpegStream(ULONGLONG ullJpegChannelbit, unsigned long dwStreamID);
	void ChangeJpegStream(ULONGLONG ullJpegChannelbit, unsigned long dwStreamID);
	void StopJpegStream(unsigned long dwStreamID);
	long PutJpegData(CFrameData *pFrameData);

private:
	int	SendVideo(unsigned long dwStreamID);
	int	SendAudio(unsigned long dwStreamID);

	int GetVideoData(unsigned long dwStreamID, unsigned char *pDataBuf, long bufSize, unsigned char *&pDataHead);
	int GetAudioData(unsigned long dwStreamID, unsigned char *pDataBuf, long bufSize);
	int GetTalkData(unsigned char *pDataBuf, long bufSize);
	int GetJpegData(unsigned long dwStreamID, unsigned char *pDataBuf, long bufSize, unsigned char *&pDataHead);

	int SplitAndSendData(unsigned long dwStreamID, CFrameBuffer& dataList, 
		NET_COMBINED_DATA& combinedData, NET_COMBINED_INFO& combinedInfo, unsigned long& dwLastDataID);

	CLIENT_STREAM_INFO * GetStreamByID(unsigned long dwStreamID);
	
	void ResetStreamBytesStatisticsEx(unsigned long channel = ALL_CHANNELS);

private:
	long							m_clientID;
	CSWL_MultiNet					*m_pMultiNetComm;

	unsigned long	const			m_dwMaxVideo;	//该DVS有几路视频
	unsigned long	const			m_dwMaxAudio;	//该DVS有几路音频

	//现场、回放、备份数据流
	CMyList<CLIENT_STREAM_INFO *>	m_lstClientStream;
	CPUB_Lock						m_streamInfoLock;		//码流的锁
	unsigned long					m_dwDataID;				//组合数据发送时，用来计算ID

	//对讲码流
	CFrameBuffer					*m_pTalkDataList;
	bool							m_bTalkNeedFormat;
	CPUB_Lock						m_talkStreamLock;

	//Jpeg流，现在只提供一个
	CLIENT_STREAM_INFO				*m_pJpegStreamInfo;
	CPUB_Lock						m_jpegStreamLock;

	//以下成员变量用来作网络流统计
	ULONGLONG						*m_pullArrivedVideoBytes;
	ULONGLONG						*m_pullSentVideoBytes;
	ULONGLONG						*m_pullArrivedAudioBytes;
	ULONGLONG						*m_pullSentAudioBytes;

	LONGLONG						*m_pPreLiveFrameTime;
	LONGLONG						*m_pFirstLiveFrameTime;
};

#endif // !defined(AFX_AVDATASENDMAN_H__2C684FC4_E768_43BE_B961_2E13790DDB6C__INCLUDED_)
