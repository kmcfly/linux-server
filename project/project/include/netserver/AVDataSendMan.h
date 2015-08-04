// AVDataSendMan.h: interface for the CAVDataSendMan class.
//
//////////////////////////////////////////////////////////////////////
/******************************************************************************
*	˵����
*	
*	�طż����ݵ���Ƶ��Ƶ����ͳһ�����һ���б��У������ֳ���������Ƶ��Ƶ�ֿ���ţ�
*	��ÿ��ͨ��ռ��һ���б��ڷ���ʱ�����ȷ��������ֳ�����Ƶ�������������ֳ�����
*	Ƶ���ٽ����ǻطŵ����ݣ�����Ǳ��ݵ����ݡ�
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
const long COMBINED_DATA_SPLIT_LEN = 128*1024;											//��֡���ݷֳɴ�СΪ���ٵ�С��
#elif defined(__PACKID_9__)
const long COMBINED_DATA_SPLIT_LEN = 128*1024;											//��֡���ݷֳɴ�СΪ���ٵ�С��
#elif defined(__PACKID_14__) || defined(__PACKID_27__)
const long COMBINED_DATA_SPLIT_LEN = 448*1024;											//��֡���ݷֳɴ�СΪ���ٵ�С��
#else
const long COMBINED_DATA_SPLIT_LEN = 128*1024;											//��֡���ݷֳɴ�СΪ���ٵ�С��
#endif
const long SUB_PACK_HEAD_LEN = sizeof(NET_COMBINED_DATA) + sizeof(PACKED_DATA_FLAG);//ÿ���ְ���ͷ������
const long SUB_PACK_LEN = COMBINED_DATA_SPLIT_LEN + SUB_PACK_HEAD_LEN;				//ÿ���ְ����ܴ�С

#define		ALL_CHANNELS		0xffffffff	//ȫ��ͨ��

#define		CLIENT_VIDEO_DATA	0	//��Ƶ����
#define		CLIENT_AUDIO_DATA	1	//��Ƶ����
#define		CLIENT_ALL_DATA		2	//��Ƶ����Ƶ����

typedef struct _client_video_info
{
	ULONGLONG 			ullVideobit;		//���֧��64��,�ӵ�λ��ʼ��ÿλ��ʾһ��ͨ��
	ULONGLONG			ullVideoFormat;		//�Ƿ�����Ƶ��ʽ
	ULONGLONG			ullIFrame;			//�Ƿ���I֡
}CLIENT_VIDEO_INFO;

typedef struct _client_audio_info
{
	ULONGLONG			ullHasAudioFormat;		//�Ƿ�����Ƶ��ʽ
	ULONGLONG 			ullAudioChannelbit;		//�������֧��64�����ӵ�λ��ʼ��ÿλ��ʾһ��ͨ��
}CLIENT_AUDIO_INFO;

typedef struct _client_stream_info
{
	unsigned long			dwStreamID;			//��ID�����͵�֡���ݸ�������Ϣ�����������ĸ�����
	unsigned short			bLiveStream;		//�ǲ����ֳ�����
	unsigned short			bFirstFrame;		//�طŻ򱸷�ʱ���ÿͻ���ÿͨ���Ƿ�տ�ʼ��������

	union
	{
		CLIENT_VIDEO_INFO	fastVideoInfo;
		CLIENT_VIDEO_INFO	playbackInfo;
		CLIENT_VIDEO_INFO	jpegInfo;
	};
	
	CLIENT_VIDEO_INFO		slowVideoInfo;
	CLIENT_AUDIO_INFO		audioInfo;

	CFrameBuffer			**ppAudioDataList;	//��Ƶ����list
	CFrameBuffer			**ppVideoDataList;	//��Ƶ����list

	unsigned long			dwSendVideoIndex;	//��������ͨ������Ƶ��
	unsigned long			dwLastDataID;		//ȡ����ʱ�����һ��ȡ���ݵ�ID��GetFrameʱ��õģ�

	NET_COMBINED_DATA		combinedData;		//�����������ʱ��
	NET_COMBINED_INFO		combinedInfo;		//��¼Ҫ���͵��������

	unsigned long			vDataToSendLen;		//�ϴη���ʧ�ܣ��ִ����͵���Ƶ���ݳ���
	unsigned char			szVSendData[SUB_PACK_LEN];	//����ֱ�ӷ�����Ƶ���ݵĻ�����

	unsigned long			aDataToSendLen;		//�ϴη���ʧ�ܣ��ִ����͵���Ƶ���ݳ���
	unsigned char			szASendData[SUB_PACK_LEN];	//����ֱ�ӷ�����Ƶ���ݵĻ�����

//	unsigned long			oDataToSendLen;		//�ϴη���ʧ�ܣ��ִ����͵��������ݳ���
//	unsigned char			szOSendData[SUB_PACK_LEN];	//����ֱ�ӷ����������ݵĻ�����
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

	//��ʼһ������
	void StartStream(bool bLiveStream, ULONGLONG ullFastVideobit,
		ULONGLONG ullSlowVideobit, ULONGLONG ullAudioChannelbit, unsigned long dwStreamID);

	//�ı�ĳ��������Ҫ�Ľ��յ�����Ƶͨ��
	void ChangeStream(ULONGLONG ullFastVideobit, ULONGLONG ullSlowVideobit,
		ULONGLONG ullAudioChannelbit, unsigned long dwStreamID);
	//ֹͣĳ������
	void StopStream(unsigned long dwStreamID);

	//ֹͣ��������
	void StopAllStream();

	//����Ƶ���ݷ��뷢���б�
	long PutVideoData(CFrameData *pFrameData, bool bLive = true);

	//����Ƶ���ݷ��뷢���б�
	long PutAudioData(CFrameData *pFrameData, bool bLive = true);


	//���¼����ݼӷ��뷢���б�
	long PutOtherData(CFrameData *pFrameData);

	//�������ģ����δ��������ֳ�����
	int	CleanLiveData(unsigned long dwStreamID, unsigned long dwDataType , unsigned long dwLiveType,
		unsigned long dwChannel = ALL_CHANNELS, bool bCleanFormat = true);

	int CleanPBData(unsigned long dwStreamID);

	//���ʵ��ķ��Ͳ��Է����ֳ���Ƶ��Ƶ���ݼ��ط�����
	int SendAVData();

	//�õ�һ֡��һ�ְ�����
	int GetAVData(unsigned char *pDestBuf, long bufSize, unsigned char *&pDataHead);

	//�õ���ǰָ��ͨ����Ҫ���͵������ܴ�С
	ULONGLONG GetStreamArrivedBytes(unsigned long channel, unsigned long frameType);

	//�õ���ǰָ��ͨ���ѷ��͵������ܴ�С
	ULONGLONG GetStreamSentBytes(unsigned long channel, unsigned long frameType);

	//�õ�ͳ�����ݵ�ʱ����
	LONGLONG GetStatisticTime(unsigned long channel);

	//����ͳ������������,-1��ʾ��������ͨ����ͳ��
	void ResetStreamBytesStatistics(unsigned long channel = ALL_CHANNELS);

	//�Խ���������
	void StartTalkStream();
	void StopTalkStream();
	long PutTalkData(CFrameData *pFrameData);

	//JPEG������
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

	unsigned long	const			m_dwMaxVideo;	//��DVS�м�·��Ƶ
	unsigned long	const			m_dwMaxAudio;	//��DVS�м�·��Ƶ

	//�ֳ����طš�����������
	CMyList<CLIENT_STREAM_INFO *>	m_lstClientStream;
	CPUB_Lock						m_streamInfoLock;		//��������
	unsigned long					m_dwDataID;				//������ݷ���ʱ����������ID

	//�Խ�����
	CFrameBuffer					*m_pTalkDataList;
	bool							m_bTalkNeedFormat;
	CPUB_Lock						m_talkStreamLock;

	//Jpeg��������ֻ�ṩһ��
	CLIENT_STREAM_INFO				*m_pJpegStreamInfo;
	CPUB_Lock						m_jpegStreamLock;

	//���³�Ա����������������ͳ��
	ULONGLONG						*m_pullArrivedVideoBytes;
	ULONGLONG						*m_pullSentVideoBytes;
	ULONGLONG						*m_pullArrivedAudioBytes;
	ULONGLONG						*m_pullSentAudioBytes;

	LONGLONG						*m_pPreLiveFrameTime;
	LONGLONG						*m_pFirstLiveFrameTime;
};

#endif // !defined(AFX_AVDATASENDMAN_H__2C684FC4_E768_43BE_B961_2E13790DDB6C__INCLUDED_)
