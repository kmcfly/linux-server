#ifndef __HXAVDATASENDMAN_H__
#define __HXAVDATASENDMAN_H__

/***************************************************************************
本类针对一个客户端连接请求一路音视频数据的方式设计
***************************************************************************/
#include "HxFrameBuffer.h"
#include "SWL_TypeDefine.h"
#include "HxGlobal.h"

#define HX_DATA_PACK_LEN			TVTHX_MAX_VIDEO_PACK_SIZE

const unsigned long videoCacheBufLen	= 1024 * 1024;		//现场视频缓冲长度
const unsigned long audioCacheBufLen	= 64   * 1024;				//现场音频缓冲长度
const unsigned long cacheTimeLen		= 4;						//缓冲数据最大时间长度， 单位秒

typedef struct _HX_STREAM_INFO
{
	char				bHasVideoIFrame;			//是否已经发了视频关键帧
	char				reserved1;

	CHxFrameBuffer		*pVideoFrameBuffer;			//视频数据列表
	CHxFrameBuffer		*pAudioFrameBuffer;			//音频数据列表

	unsigned long		dwLastDataID;				//取数据时，最后一次取数据的ID（GetFrame时获得的）

	struct
	{
		unsigned long	dwUsed;			//是否正被使用	0 没有用 1 使用了
		unsigned long	dwDataID;		//本组合数据的ID，组合数据里的每个包的数据ID相同
		unsigned long	dwTotalLen;		//所有的有效数据的总长度
		unsigned long	dwLen;			//已经接收/发送了多少长度
		unsigned long	continuePackNum;
	}combinedInfo;
}HX_STREAM_INFO;

typedef enum _hx_send_frame_error
{
	HX_SEND_FRAME_SUCC,
	HX_SEND_FRAME_NEED_IFRAME,
}HX_SEND_FRAME_ERROR;

class CHxAVDataSendMan
{
public:
	CHxAVDataSendMan(long channel, bool bLive, HX_VIDEO_VERSION videoVersion = HX_VIDEO_VERSION_TYPE_ONE);
	~CHxAVDataSendMan(void);

	bool StartStream();
	void StopStream();

	long PutVideoData(CFrameData *pFrameData);
	long PutAudioData(CFrameData *pFrameData);

	long GetAVData(unsigned char *pDataBuf, long bufSize, TVTHX_EXT_VIDEO_INFO *pExtVideoInfo);
	bool IsEndFrame();
	
	void ClearData(bool bClearVideo, bool bClearAudio);

	bool IsLive(){return m_bLive;}
	void SetVideoVersion(HX_VIDEO_VERSION videoVersion){m_videoVersion = videoVersion;}

private:
	long GetVideoData(unsigned char *pDataBuf, long bufSize, TVTHX_EXT_VIDEO_INFO *pExtVideoInfo);
	long GetAudioData(unsigned char *pDataBuf, long bufSize, TVTHX_EXT_VIDEO_INFO *pExtVideoInfo);

	void GetTimeVal(timeval *pTimeVal);

private:
	bool					m_bLive;
	HX_VIDEO_VERSION		m_videoVersion;
	long					m_channel;				//本对象发送数据的指定通道
	unsigned long			m_dwDataID;				//组合数据发送时，用来计算ID

	HX_STREAM_INFO			m_streamInfo;

	unsigned long			m_frameIndex;			//视频帧序号
	unsigned long			m_allFrameIndex;		//音视频帧序号
};

#endif
