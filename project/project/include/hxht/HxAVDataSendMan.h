#ifndef __HXAVDATASENDMAN_H__
#define __HXAVDATASENDMAN_H__

/***************************************************************************
�������һ���ͻ�����������һ·����Ƶ���ݵķ�ʽ���
***************************************************************************/
#include "HxFrameBuffer.h"
#include "SWL_TypeDefine.h"
#include "HxGlobal.h"

#define HX_DATA_PACK_LEN			TVTHX_MAX_VIDEO_PACK_SIZE

const unsigned long videoCacheBufLen	= 1024 * 1024;		//�ֳ���Ƶ���峤��
const unsigned long audioCacheBufLen	= 64   * 1024;				//�ֳ���Ƶ���峤��
const unsigned long cacheTimeLen		= 4;						//�����������ʱ�䳤�ȣ� ��λ��

typedef struct _HX_STREAM_INFO
{
	char				bHasVideoIFrame;			//�Ƿ��Ѿ�������Ƶ�ؼ�֡
	char				reserved1;

	CHxFrameBuffer		*pVideoFrameBuffer;			//��Ƶ�����б�
	CHxFrameBuffer		*pAudioFrameBuffer;			//��Ƶ�����б�

	unsigned long		dwLastDataID;				//ȡ����ʱ�����һ��ȡ���ݵ�ID��GetFrameʱ��õģ�

	struct
	{
		unsigned long	dwUsed;			//�Ƿ�����ʹ��	0 û���� 1 ʹ����
		unsigned long	dwDataID;		//��������ݵ�ID������������ÿ����������ID��ͬ
		unsigned long	dwTotalLen;		//���е���Ч���ݵ��ܳ���
		unsigned long	dwLen;			//�Ѿ�����/�����˶��ٳ���
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
	long					m_channel;				//�����������ݵ�ָ��ͨ��
	unsigned long			m_dwDataID;				//������ݷ���ʱ����������ID

	HX_STREAM_INFO			m_streamInfo;

	unsigned long			m_frameIndex;			//��Ƶ֡���
	unsigned long			m_allFrameIndex;		//����Ƶ֡���
};

#endif
