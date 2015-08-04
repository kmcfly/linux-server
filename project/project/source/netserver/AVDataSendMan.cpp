// AVDataSendMan.cpp: implementation of the CAVDataSendMan class.
//
//////////////////////////////////////////////////////////////////////
#include "AVDataSendMan.h"

#include "FrameData.h"
#include "FrameBuffer.h"
#include "NetProtocol.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAVDataSendMan::CAVDataSendMan(long clientID, CSWL_MultiNet *pMultiNetComm, unsigned long dwMaxVideo, unsigned long dwMaxAudio):
m_dwMaxVideo(dwMaxVideo), m_dwMaxAudio(dwMaxVideo)
{
	m_clientID = clientID;
	m_pMultiNetComm = pMultiNetComm;

	m_pTalkDataList = NULL;
	m_pJpegStreamInfo = NULL;
	
	m_dwDataID = 0;

	m_pullArrivedVideoBytes = new ULONGLONG[m_dwMaxVideo];
	m_pullSentVideoBytes	= new ULONGLONG[m_dwMaxVideo];
	m_pullArrivedAudioBytes	= new ULONGLONG[m_dwMaxAudio];
	m_pullSentAudioBytes	= new ULONGLONG[m_dwMaxAudio];
	m_pPreLiveFrameTime		= new LONGLONG[m_dwMaxVideo];
	m_pFirstLiveFrameTime	= new LONGLONG[m_dwMaxVideo];

	ResetStreamBytesStatistics();
}

CAVDataSendMan::~CAVDataSendMan()
{
	unsigned long i = 0;

	if(!m_lstClientStream.IsEmpty())
	{
		assert(false);

		CLIENT_STREAM_INFO *pStreamInfo;
		while(!m_lstClientStream.IsEmpty())
		{
			pStreamInfo = m_lstClientStream.RemoveHead();
			unsigned long audioListCount = 0;
			unsigned long videoListCount = 1;
			if(pStreamInfo->bLiveStream)
			{
				audioListCount = m_dwMaxAudio;
				videoListCount = m_dwMaxVideo;
			}
			for (i = 0; i < videoListCount; ++i) 
			{
				pStreamInfo->ppVideoDataList[i]->RemoveAll();
				delete pStreamInfo->ppVideoDataList[i];
			}
			
			for (i = 0; i < audioListCount; ++i) 
			{
				pStreamInfo->ppAudioDataList[i]->RemoveAll();
				delete pStreamInfo->ppAudioDataList[i];
			}
			
			delete [] pStreamInfo->ppAudioDataList;
			delete [] pStreamInfo->ppVideoDataList;
			
			delete pStreamInfo;
		}
	}

	delete [] m_pullArrivedVideoBytes;
	delete [] m_pullSentVideoBytes;
	delete [] m_pullArrivedAudioBytes;
	delete [] m_pullSentAudioBytes;
	delete [] m_pPreLiveFrameTime;
	delete [] m_pFirstLiveFrameTime;

//	delete m_pTalkDataList;
}

void CAVDataSendMan::StartStream(bool bLiveStream, ULONGLONG ullFastVideobit, \
								 ULONGLONG ullSlowVideobit, ULONGLONG ullAudioChannelbit, unsigned long dwStreamID)
{
	m_streamInfoLock.Lock();

	PRODUCT_TYPE productTypeTmp = CProduct::Instance()->ProductType();
	CLIENT_STREAM_INFO	*pStreamInfo = new CLIENT_STREAM_INFO;
	memset(pStreamInfo, 0, sizeof(CLIENT_STREAM_INFO));

	pStreamInfo->dwLastDataID	= INVALID_DATA_ID;
	pStreamInfo->dwStreamID		= dwStreamID;

	//����������ʼ�ĸ���Ƶ����Ƶͨ������Ϣ	
	pStreamInfo->fastVideoInfo.ullVideobit		= ullFastVideobit;
	pStreamInfo->slowVideoInfo.ullVideobit		= ullSlowVideobit;
	pStreamInfo->audioInfo.ullAudioChannelbit	= ullAudioChannelbit;
	pStreamInfo->bFirstFrame = true;
	pStreamInfo->bLiveStream = bLiveStream;
	
	if(bLiveStream)	//ÿ��ͨ��һ���б�
	{
		unsigned long i = 0;
		pStreamInfo->ppAudioDataList = new CFrameBuffer*[m_dwMaxAudio];
		for (i = 0; i < m_dwMaxAudio; ++i) 
		{
			pStreamInfo->ppAudioDataList[i] = new CFrameBuffer(NET_STREAM_AUDIO, 32*1024, 3);
		}

		pStreamInfo->ppVideoDataList = new CFrameBuffer*[m_dwMaxVideo];
		for (i = 0; i < m_dwMaxVideo; ++i) 
		{
#ifdef __DVR_ULTIMATE__
			pStreamInfo->ppVideoDataList[i] = new CFrameBuffer(NET_STREAM_VIDEO, PRE_RECORD_MAX_LEN, 3);
#else
			if (TD_2704HD == productTypeTmp)
			{
				pStreamInfo->ppVideoDataList[i] = new CFrameBuffer(NET_STREAM_VIDEO, 5*1024*1024, 3);
			}
			else if ((productTypeTmp >= TD_2704XE_L) && (productTypeTmp <= TD_2716XE_S))
			{
				pStreamInfo->ppVideoDataList[i] = new CFrameBuffer(NET_STREAM_VIDEO, 2*1024*1024, 3);
			}			
			else
			{
				pStreamInfo->ppVideoDataList[i] = new CFrameBuffer(NET_STREAM_VIDEO, 512*1024, 3);
			}
#endif
		}
	}
	else	//����ͨ������һ���б�
	{
		pStreamInfo->ppAudioDataList = new CFrameBuffer*[1];

		pStreamInfo->ppVideoDataList = new CFrameBuffer*[1];
		pStreamInfo->ppVideoDataList[0] = new CFrameBuffer(NET_STREAM_PLAYBACK, 512*1024, 3);
	}

	//������Ϣ�����ķ�������˳���ɸߵ�������
	CLIENT_STREAM_INFO *pTempStream;
	POS pos = m_lstClientStream.GetHeadPosition();
	while(pos)
	{
		pTempStream = m_lstClientStream.GetAt(pos);
		if(pTempStream->dwStreamID > dwStreamID)
		{
			m_lstClientStream.InsertBefore(pos, pStreamInfo);
			break;
		}
		m_lstClientStream.GetNext(pos);
	}
	if(pos == NULL)
	{
		m_lstClientStream.AddTail(pStreamInfo);
	}

	m_streamInfoLock.UnLock();
}
	
void CAVDataSendMan::ChangeStream(ULONGLONG ullFastVideobit, ULONGLONG ullSlowVideobit, \
								  ULONGLONG ullAudioChannelbit, unsigned long dwStreamID)
{
	m_streamInfoLock.Lock();

	CLIENT_STREAM_INFO	*pStreamInfo = GetStreamByID(dwStreamID);
	
	if (NULL != pStreamInfo)
	{
		ULONGLONG	  ullFastVideoBitOld;	//�ı�ǰ�ģ�������Ҫ������Ƶͨ��
		ULONGLONG	  ullSlowVideoBitOld;	//�ı�ǰ�ģ�������Ҫ������Ƶͨ��

		ULONGLONG	  ullAudioChannelBitOld;	//�ı�ǰ�ģ�Ҫ������Ƶͨ��

		ullFastVideoBitOld = pStreamInfo->fastVideoInfo.ullVideobit;
		ullSlowVideoBitOld = pStreamInfo->slowVideoInfo.ullVideobit;

		ullAudioChannelBitOld = pStreamInfo->audioInfo.ullAudioChannelbit;

		pStreamInfo->fastVideoInfo.ullVideobit = ullFastVideobit;
		pStreamInfo->slowVideoInfo.ullVideobit = ullSlowVideobit;
		pStreamInfo->audioInfo.ullAudioChannelbit = ullAudioChannelbit;	

		unsigned long i = 0;
		for (i = 0; i < m_dwMaxVideo; ++i) 
		{
			//���ڿ�����������ǰû�п�����ͨ����Ҫ������Ƶ��ʽ���ݣ�������I֡
			if (PUB_HasBit64(ullFastVideobit, static_cast<int>(i)))
			{
				if (!PUB_HasBit64(ullFastVideoBitOld, static_cast<int>(i))) 
				{
					//��ʾ���ͨ����û���յ�I֡
					PUB_ClearBit64(&pStreamInfo->fastVideoInfo.ullVideoFormat, static_cast<int>(i));
					PUB_ClearBit64(&pStreamInfo->fastVideoInfo.ullIFrame, static_cast<int>(i));
				}
			}
			else
			{
				//�����ǰ���ˣ�����û����ͨ�����ͷ�û���������
				if (PUB_HasBit64(ullFastVideoBitOld, static_cast<int>(i)))
				{
					PUB_ClearBit64(&pStreamInfo->fastVideoInfo.ullVideoFormat, static_cast<int>(i));
					PUB_ClearBit64(&pStreamInfo->fastVideoInfo.ullIFrame, static_cast<int>(i));
					pStreamInfo->ppVideoDataList[i]->RemoveAll();

					ResetStreamBytesStatisticsEx(i);
				}
			}

			if (PUB_HasBit64(ullSlowVideobit, static_cast<int>(i)))
			{
				if (!PUB_HasBit64(ullSlowVideoBitOld, static_cast<int>(i))) 
				{
					//��ʾ���ͨ����û���յ�I֡
					PUB_ClearBit64(&pStreamInfo->slowVideoInfo.ullVideoFormat, static_cast<int>(i));
					PUB_ClearBit64(&pStreamInfo->slowVideoInfo.ullIFrame, static_cast<int>(i));
				}
			}
			else
			{
				//�����ǰ���ˣ�����û����ͨ�����ͷ�û���������
				if (PUB_HasBit64(ullSlowVideoBitOld, static_cast<int>(i)))
				{
					PUB_ClearBit64(&pStreamInfo->slowVideoInfo.ullVideoFormat, static_cast<int>(i));
					PUB_ClearBit64(&pStreamInfo->slowVideoInfo.ullIFrame, static_cast<int>(i));
					pStreamInfo->ppVideoDataList[i]->RemoveAll();

					ResetStreamBytesStatisticsEx(i);
				}
			}		
		}	

		for (i = 0; i < m_dwMaxAudio; ++i) 
		{
			//���ڿ�����������ǰû�п�����ͨ����Ҫ������Ƶ��ʽ����
			if (PUB_HasBit64(ullAudioChannelbit, static_cast<int>(i)))
			{
				if (!PUB_HasBit64(ullAudioChannelBitOld, static_cast<int>(i))) 
				{
					//��ʾ���ͨ����û���յ�I֡
					PUB_ClearBit64(&pStreamInfo->audioInfo.ullHasAudioFormat, static_cast<int>(i));
				}
			}
			else 
			{
				if (PUB_HasBit64(ullAudioChannelBitOld, static_cast<int>(i))) 
				{
					pStreamInfo->ppAudioDataList[i]->RemoveAll();

					ResetStreamBytesStatisticsEx(i);
				}
			}
		}
	}

	m_streamInfoLock.UnLock();
}

/******************************************************************************
*
*ֹͣ����������������е������Դ
*
******************************************************************************/
void CAVDataSendMan::StopStream(unsigned long dwStreamID)
{
	unsigned long i = 0;

	m_streamInfoLock.Lock();
	CLIENT_STREAM_INFO *pStreamInfo;
	POS pos = m_lstClientStream.GetHeadPosition();
	while(pos)
	{
		pStreamInfo = m_lstClientStream.GetAt(pos);
		if(pStreamInfo->dwStreamID == dwStreamID)
		{
			m_lstClientStream.RemoveAt(pos);
			break;
		}
		m_lstClientStream.GetNext(pos);
	}
	if(pos == NULL)
	{
		//ֹͣһ��δ����������
		PUB_PRINTF_WARN("ֹͣһ��δ����������\n");
//		assert(false);
		m_streamInfoLock.UnLock();
		return;
	}

	unsigned long audioListCount = 0;
	unsigned long videoListCount = 1;
	if(pStreamInfo->bLiveStream)
	{
		audioListCount = m_dwMaxAudio;
		videoListCount = m_dwMaxVideo;
	}
	//�����Դ
	for (i = 0; i < videoListCount; ++i) 
	{
		pStreamInfo->ppVideoDataList[i]->RemoveAll();
		delete pStreamInfo->ppVideoDataList[i];
	}
		
	for (i = 0; i < audioListCount; ++i) 
	{
		pStreamInfo->ppAudioDataList[i]->RemoveAll();
		delete pStreamInfo->ppAudioDataList[i];
	}

	delete [] pStreamInfo->ppAudioDataList;
	delete [] pStreamInfo->ppVideoDataList;

	delete pStreamInfo;

	m_streamInfoLock.UnLock();

	ResetStreamBytesStatistics();
}

void CAVDataSendMan::StopAllStream()
{
	m_streamInfoLock.Lock();

	unsigned int i;
	CLIENT_STREAM_INFO *pStreamInfo;
	while(!m_lstClientStream.IsEmpty())
	{
		pStreamInfo = m_lstClientStream.RemoveHead();
		
		unsigned long audioListCount = 0;
		unsigned long videoListCount = 1;
		if(pStreamInfo->bLiveStream)
		{
			audioListCount = m_dwMaxAudio;
			videoListCount = m_dwMaxVideo;
		}
		
		for (i = 0; i < videoListCount; ++i) 
		{
			pStreamInfo->ppVideoDataList[i]->RemoveAll();
			delete pStreamInfo->ppVideoDataList[i];
		}
		
		for (i = 0; i < audioListCount; ++i) 
		{
			pStreamInfo->ppAudioDataList[i]->RemoveAll();
			delete pStreamInfo->ppAudioDataList[i];
		}
			
		delete [] pStreamInfo->ppAudioDataList;
		delete [] pStreamInfo->ppVideoDataList;

		delete pStreamInfo;
	}

	m_streamInfoLock.UnLock();

	//ֹͣ�Խ���
	m_talkStreamLock.Lock();

	if(m_pTalkDataList != NULL)
	{
		m_pTalkDataList->RemoveAll();
		delete m_pTalkDataList;
		m_pTalkDataList = NULL;
	}

	m_talkStreamLock.UnLock();

	//ֹͣJPEG��
	m_jpegStreamLock.Lock();

	if(m_pJpegStreamInfo != NULL)
	{
		for (i = 0; i < m_dwMaxVideo; ++i) 
		{
			m_pJpegStreamInfo->ppVideoDataList[i]->RemoveAll();
			delete m_pJpegStreamInfo->ppVideoDataList[i];
		}

		delete [] m_pJpegStreamInfo->ppVideoDataList;
		delete  m_pJpegStreamInfo;
		m_pJpegStreamInfo = NULL;
	}

	m_jpegStreamLock.UnLock();
}

/******************************************************************************
*
*����Ƶ���ݷ��뷢���б�
*
******************************************************************************/
long CAVDataSendMan::PutVideoData(CFrameData *pFrameData, bool bLive)
{
	bool bNeedKeyFrame = false;
	long ret = SEND_FRAME_SUCC;

	FRAME_INFO_EX &frameInfo = pFrameData->GetFrameInfo();
	CLIENT_STREAM_INFO	*pStreamInfo = NULL;
	CLIENT_VIDEO_INFO	*pVideoInfo = NULL;

	m_streamInfoLock.Lock();

	POS pos = m_lstClientStream.GetHeadPosition();
	while(pos)
	{
		pStreamInfo = m_lstClientStream.GetNext(pos);

		if(bLive)	//�ֳ�����
		{
			if(!pStreamInfo->bLiveStream)	//ֻ���뵽�ֳ���
			{
				continue;
			}
		}
		else	//�ǻط���������ID��һ��
		{
			if(pStreamInfo->dwStreamID != frameInfo.streamID)
			{
				continue;
			}
		}


		if(!pStreamInfo->bLiveStream)	//�طŻ򱸷ݵ����ݣ�Ҫ���˵��ϴλطŻ򱸷�ʱ����������
		{
			if(pStreamInfo->bFirstFrame)
			{
				if(frameInfo.frameIndex != 0)
				{
					break;
				}
				pStreamInfo->bFirstFrame = false;
			}
		}

		if(frameInfo.frameAttrib & LIVE_FRAME_SECOND_STREAM)
		{
			pVideoInfo = &pStreamInfo->slowVideoInfo;
		}
		else
		{
			pVideoInfo = &pStreamInfo->fastVideoInfo;
		}

		//�жϴ���Ƶͨ���Ƿ���
		if(!PUB_HasBit64(pVideoInfo->ullVideobit, 
			static_cast<int>(CHANNEL_ID(frameInfo.channel))))
		{
			continue;
		}
		else if (!PUB_HasBit64(pVideoInfo->ullVideoFormat, 
			static_cast<int>(CHANNEL_ID(frameInfo.channel)))) //ͨ���ǿ�����,���Ǹ�ͨ����û����ʽ
		{	
			if (FRAME_TYPE_VIDEO_FORMAT != frameInfo.frameType) 
			{
				m_streamInfoLock.UnLock();
				return SEND_FRAME_NEED_FORMAT;
			}
			else
			{
				PUB_SetBit64(&pVideoInfo->ullVideoFormat, CHANNEL_ID(frameInfo.channel));
			}				
		}
		else	//ͨ�������ˣ��������˸�ʽ
		{
			if(FRAME_TYPE_VIDEO_FORMAT == frameInfo.frameType)	//�ı�����Ƶ��ʽ��Ҫ�ȴ��ؼ�֡
			{
				PUB_ClearBit64(&pVideoInfo->ullIFrame, static_cast<int>(CHANNEL_ID(frameInfo.channel)));
			}
			else//
			{
				//��û�յ�I֡
				if(!PUB_HasBit64(pVideoInfo->ullIFrame, static_cast<int>(CHANNEL_ID(frameInfo.channel))))
				{
					//�������ͨ��Ƶ���ݣ���Ҫ�ж��Ƿ���I֡
					if (FRAME_TYPE_VIDEO == frameInfo.frameType)
					{
						if( 0 == frameInfo.keyFrame) //���ǹؼ�֡��Ҫ�ж��Ƿ��Ѿ������˹ؼ�֡
						{
							ret = SEND_FRAME_NEED_IFRAME;
							continue;
						}
						else	//�ǹؼ�֡�������Ƿ��͹��ؼ�֡��������һ��
						{
							PUB_SetBit64(&pVideoInfo->ullIFrame, CHANNEL_ID(frameInfo.channel));
						}
					}
					else
					{
						printf(" frameInfo.frameType:%d   %s %d\n", frameInfo.frameType, __FILE__, __LINE__);
						assert(false);//ǰ�治�Ǹ�ʽ�������в�����ͨ���� 
					}
				}
			}
		}

		assert(CHANNEL_ID(frameInfo.channel) < m_dwMaxVideo );
		if(pStreamInfo->bLiveStream)
		{
			pStreamInfo->ppVideoDataList[CHANNEL_ID(frameInfo.channel)]->AddFrame(pFrameData, bNeedKeyFrame);

			if((m_pPreLiveFrameTime[CHANNEL_ID(frameInfo.channel)] - pFrameData->GetFrameInfo().time) > 10*1000000)
			{
				ResetStreamBytesStatistics(CHANNEL_ID(frameInfo.channel));
			}
			m_pullArrivedVideoBytes[CHANNEL_ID(frameInfo.channel)] += pFrameData->GetFrameInfo().length;
			m_pPreLiveFrameTime[CHANNEL_ID(frameInfo.channel)] = pFrameData->GetFrameInfo().time;
		}
		else
		{
			pStreamInfo->ppVideoDataList[0]->AddFrame(pFrameData, bNeedKeyFrame);
		}

		if (bNeedKeyFrame)
		{
			PUB_ClearBit64(&pVideoInfo->ullIFrame, CHANNEL_ID(frameInfo.channel));
		}
	}

	m_streamInfoLock.UnLock();

	return ret;
}

/******************************************************************************
*
*����Ƶ���ݷ��뷢���б�
*
******************************************************************************/
long CAVDataSendMan::PutAudioData(CFrameData *pFrameData, bool bLive)
{
	assert(NULL != pFrameData);

	bool bNeedKeyFrame = false;
	long ret = SEND_FRAME_SUCC;

	FRAME_INFO_EX &frameInfo = pFrameData->GetFrameInfo();
	CLIENT_STREAM_INFO	*pStreamInfo = NULL;

	m_streamInfoLock.Lock();

	POS pos = m_lstClientStream.GetHeadPosition();
	while(pos)
	{
		pStreamInfo = m_lstClientStream.GetNext(pos);
		if(bLive)
		{
			if(!pStreamInfo->bLiveStream)
			{
				continue;
			}
		}
		else
		{
			if(pStreamInfo->dwStreamID != frameInfo.streamID)
			{
				continue;
			}
		}		

		if(!pStreamInfo->bLiveStream)	//�طŻ򱸷ݵ����ݣ�Ҫ���˵��ϴλطŻ򱸷�ʱ����������
		{
			if(pStreamInfo->bFirstFrame)
			{
				if(frameInfo.frameIndex != 0)
				{
					break;
				}
				pStreamInfo->bFirstFrame = false;
			}
		}

		//�жϴ���Ƶͨ���Ƿ���
		if(!PUB_HasBit64(pStreamInfo->audioInfo.ullAudioChannelbit, static_cast<int>(CHANNEL_ID(frameInfo.channel))))
		{
			//������ֳ�������Ҫ����
			if(pStreamInfo->bLiveStream)
			{
				continue;
			}

			//����ط���Ƶ��Ƶ���ѹر�
			if(!PUB_HasBit64(pStreamInfo->audioInfo.ullAudioChannelbit|pStreamInfo->fastVideoInfo.ullVideobit, \
				static_cast<int>(CHANNEL_ID(frameInfo.channel))))
			{
				continue;
			}
		}
		else if(!PUB_HasBit64(pStreamInfo->audioInfo.ullHasAudioFormat, 
			static_cast<int>(CHANNEL_ID(frameInfo.channel)))) //ͨ���ǿ�����,����û���յ���ʽ
		{
			if(FRAME_TYPE_AUDIO_FORMAT != frameInfo.frameType)
			{	
				m_streamInfoLock.UnLock();
				return SEND_FRAME_NEED_FORMAT;
			}
			else
			{
				PUB_SetBit64(&pStreamInfo->audioInfo.ullHasAudioFormat, CHANNEL_ID(frameInfo.channel));
			}
		}
		//�и�ʽ��Ҳ�����ٷ�һ�Σ�Ϊ���ǻط�ʱÿһ֡�����뷢�ͣ���������ڿ��ʱ���������ػظ�֡���
		//else	//ͨ���ǿ����ģ������и�ʽ��
		//{
		//	if(FRAME_TYPE_AUDIO_FORMAT == frameInfo.frameType)
		//	{		
		//		m_streamInfoLock.UnLock();
		//		return SEND_FRAME_SUCC;
		//	}
		//}
		assert(CHANNEL_ID(frameInfo.channel) < m_dwMaxAudio);
		if(pStreamInfo->bLiveStream)
		{
			pStreamInfo->ppAudioDataList[CHANNEL_ID(frameInfo.channel)]->AddFrame(pFrameData, bNeedKeyFrame);
			m_pullArrivedAudioBytes[CHANNEL_ID(frameInfo.channel)] += pFrameData->GetFrameInfo().length;
		}
		else	//�طźͱ��ݵ���Ƶ��Ƶͳһ����һ���б���
		{
			pStreamInfo->ppVideoDataList[0]->AddFrame(pFrameData, bNeedKeyFrame);
		}
	}

	m_streamInfoLock.UnLock();

	return ret;
}

long CAVDataSendMan::PutOtherData(CFrameData *pFrameData)
{
	assert(NULL != pFrameData);

	bool bNeedKeyFrame = false;

	FRAME_INFO_EX	frameInfo = pFrameData->GetFrameInfo();
	CLIENT_STREAM_INFO	*pStreamInfo = NULL;

	m_streamInfoLock.Lock();

	POS pos = m_lstClientStream.GetHeadPosition();
	while(pos)
	{
		pStreamInfo = m_lstClientStream.GetNext(pos);
		if(pStreamInfo->dwStreamID != frameInfo.streamID)
		{
			continue;
		}
		pStreamInfo->ppVideoDataList[0]->AddFrame(pFrameData, bNeedKeyFrame);
	}

	m_streamInfoLock.UnLock();

	return SEND_FRAME_SUCC;
}

/******************************************************************************
*
*�������ģ����δ�����������
*����ֵ��0�ɹ�
		 -1�����û�����������ͨ��û������
*
******************************************************************************/
int	CAVDataSendMan::CleanLiveData(unsigned long dwStreamID, unsigned long dwDataType , unsigned long dwLiveType, 
								  unsigned long dwChannel/* = ALL_CHANNELS*/, bool bCleanFormat/* = true*/)
{
	assert((dwChannel < m_dwMaxVideo) || (dwChannel < m_dwMaxAudio) || (ALL_CHANNELS == dwChannel));
	assert((CLIENT_VIDEO_DATA == dwDataType) 
		|| (CLIENT_AUDIO_DATA == dwDataType) 
		|| (CLIENT_ALL_DATA == dwDataType));

	unsigned long i = 0;
	CLIENT_VIDEO_INFO	*pVideoStreamInfo = NULL;

	m_streamInfoLock.Lock();

	CLIENT_STREAM_INFO *pStreamInfo = GetStreamByID(dwStreamID);
	if(pStreamInfo == NULL)
	{
		m_streamInfoLock.UnLock();
		return -1;
	}

	if (VIDEO_STREAM_FAST_NET == dwLiveType)		
	{		
		pVideoStreamInfo =  &pStreamInfo->fastVideoInfo ;

		//��������ȥ�ж�ͨ�������ݸò������
		//if (0 == pVideoStreamInfo->ullVideobit)//��Ӧ��������ͨ���ڴ���
		//{
		//	m_streamInfoLock.UnLock();
		//	return -1; 
		//}
	}
	else if (VIDEO_STREAM_SLOW_NET == dwLiveType) 
	{	
		pVideoStreamInfo = &pStreamInfo->slowVideoInfo;

		//��������ȥ�ж�ͨ�������ݸò������
		//if (0 == pVideoStreamInfo->ullVideobit)//��Ӧ��������ͨ���ڴ���
		//{
		//	m_streamInfoLock.UnLock();
		//	return -1; 
		//}
	}
	else
	{
		if (CLIENT_AUDIO_DATA != dwDataType)
		{
			assert(false);
		}
	}

	if (CLIENT_AUDIO_DATA == dwDataType) 
	{
		if (ALL_CHANNELS == dwChannel) 
		{
			if(bCleanFormat)
			{
				memset(&pStreamInfo->audioInfo.ullHasAudioFormat, 0, sizeof(ULONGLONG));		
			}

			for(i = 0; i < m_dwMaxAudio; ++i)
			{
				pStreamInfo->ppAudioDataList[i]->RemoveAll();
			}
		}
		else
		{	
			if(bCleanFormat)
			{
				PUB_ClearBit64(&pStreamInfo->audioInfo.ullHasAudioFormat, dwChannel);
			}

			pStreamInfo->ppAudioDataList[dwChannel]->RemoveAll();
		}
	}
	else if(CLIENT_VIDEO_DATA == dwDataType)
	{		
		if (ALL_CHANNELS == dwChannel)
		{
			if(bCleanFormat)
			{
				memset(&pVideoStreamInfo->ullVideoFormat, 0, sizeof(ULONGLONG));
			}

			memset(&pVideoStreamInfo->ullIFrame, 0, sizeof(ULONGLONG));

			for(i = 0; i < m_dwMaxVideo; ++i)
			{
				if (pVideoStreamInfo->ullVideobit & ((ULONGLONG)(0x01) << i))
				{
					pStreamInfo->ppVideoDataList[i]->RemoveAll();	
				}
			}
		}
		else
		{			
			if(bCleanFormat)
			{
				PUB_ClearBit64(&pVideoStreamInfo->ullVideoFormat, dwChannel);
			}

			PUB_ClearBit64(&pVideoStreamInfo->ullIFrame, dwChannel);

			if (pVideoStreamInfo->ullVideobit & ((ULONGLONG)(0x01) << dwChannel))
			{
				pStreamInfo->ppVideoDataList[dwChannel]->RemoveAll();	
			}	
		}
	}
	else //CLIENT_ALL_DATA
	{
		if (ALL_CHANNELS == dwChannel) 
		{
			if(bCleanFormat)
			{
				memset(&pVideoStreamInfo->ullVideoFormat, 0, sizeof(ULONGLONG));
				memset(&pStreamInfo->audioInfo.ullHasAudioFormat, 0, sizeof(ULONGLONG));
			}	

			memset(&pVideoStreamInfo->ullIFrame, 0, sizeof(ULONGLONG));		

			for(i = 0; i < m_dwMaxVideo; ++i)
			{
				if (pVideoStreamInfo->ullVideobit & ((ULONGLONG)(0x01) << i))
				{
					pStreamInfo->ppVideoDataList[i]->RemoveAll();	
				}				
			}

			for(i = 0; i < m_dwMaxAudio; ++i)
			{
				pStreamInfo->ppAudioDataList[i]->RemoveAll();	
			}
		}
		else
		{
			//�������жϣ�������Ƶ�����ͨ��������Ƶ�����ͨ������ͬ			
			if (dwChannel < m_dwMaxVideo) 
			{
				if(bCleanFormat)
				{
					PUB_ClearBit64(&pVideoStreamInfo->ullVideoFormat, dwChannel);
				}				
				PUB_ClearBit64(&pVideoStreamInfo->ullIFrame, dwChannel);

				if (pVideoStreamInfo->ullVideobit & ((ULONGLONG)(0x01) << dwChannel))
				{
					pStreamInfo->ppVideoDataList[dwChannel]->RemoveAll();
				}
			}

			if (dwChannel < m_dwMaxAudio) 
			{
				if(bCleanFormat)
				{
					PUB_ClearBit64(&pStreamInfo->audioInfo.ullHasAudioFormat, dwChannel);			
				}	

				pStreamInfo->ppAudioDataList[dwChannel]->RemoveAll();
			}			
		}
	}	

	m_streamInfoLock.UnLock();

	ResetStreamBytesStatistics(dwChannel);

	return 0;
}

int CAVDataSendMan::CleanPBData(unsigned long dwStreamID)
{
	m_streamInfoLock.Lock();

	CLIENT_STREAM_INFO *pStreamInfo = GetStreamByID(dwStreamID);
	if(pStreamInfo == NULL)
	{
		m_streamInfoLock.UnLock();
		return -1;
	}

	assert(!pStreamInfo->bLiveStream);
	if(pStreamInfo->bLiveStream)
	{
		m_streamInfoLock.UnLock();
		return -1;
	}

	pStreamInfo->ppVideoDataList[0]->RemoveAll();
	memset(&pStreamInfo->audioInfo.ullHasAudioFormat, 0, sizeof(ULONGLONG));
	memset(&pStreamInfo->fastVideoInfo.ullVideoFormat, 0, sizeof(ULONGLONG));
	memset(&pStreamInfo->fastVideoInfo.ullIFrame, 0, sizeof(ULONGLONG));

	m_streamInfoLock.UnLock();

	return 0;
}

/******************************************************************************
*
*������Ƶ��Ƶ���ݣ����ֳ���Ƶ���ȣ��ֳ���Ƶ��Σ��ط��������Ĳ��Է���
*����ֵ��0�������ݳɹ�
		 -1��������ʧ��
		 -2û�����ݿɷ���
*
******************************************************************************/
int CAVDataSendMan::SendAVData()
{
	int iRet = 0;

	m_streamInfoLock.Lock();

	CLIENT_STREAM_INFO * pStreamInfo;
	POS pos = m_lstClientStream.GetHeadPosition();
	while(pos)
	{
		pStreamInfo = m_lstClientStream.GetNext(pos);

		//������Ƶ����
		iRet = SendAudio(pStreamInfo->dwStreamID);

		//��������ݷ��ͣ�ֱ�ӷ��أ����°����ʹ���������ݷ���
		if(iRet != -2)
		{
			m_streamInfoLock.UnLock();
			return iRet;
		}

		//������Ƶ����
		iRet = SendVideo(pStreamInfo->dwStreamID);

		//��������ݷ��ͣ�ֱ�ӷ��أ����°����ʹ���������ݷ���
		if(iRet != -2)
		{
			m_streamInfoLock.UnLock();
			return iRet;
		}
	}

	m_streamInfoLock.UnLock();
	return -2;
}

/************************************************************************
*
*
*
************************************************************************/
int CAVDataSendMan::GetAVData(unsigned char *pDestBuf, long bufSize, unsigned char *&pDataHead)
{
	int iRet = 0;

	//���ȷ��ͶԽ�����
	m_talkStreamLock.Lock();
	iRet = GetTalkData(pDestBuf, bufSize);
	m_talkStreamLock.UnLock();

	//��������ݷ��ͣ�ֱ�ӷ��أ����°����ʹ���������ݷ���
	if(iRet > 0)
	{
		pDataHead = pDestBuf;
		return iRet;
	}

	m_streamInfoLock.Lock();

	CLIENT_STREAM_INFO * pStreamInfo;
	POS pos = m_lstClientStream.GetHeadPosition();
	while(pos)
	{
		pStreamInfo = m_lstClientStream.GetNext(pos);

		//������Ƶ����
		iRet = GetAudioData(pStreamInfo->dwStreamID, pDestBuf, bufSize);

		//��������ݷ��ͣ�ֱ�ӷ��أ����°����ʹ���������ݷ���
		if(iRet > 0)
		{
			pDataHead = pDestBuf;
			m_streamInfoLock.UnLock();
			return iRet;
		}

		//������Ƶ����
		iRet = GetVideoData(pStreamInfo->dwStreamID, pDestBuf, bufSize, pDataHead);

		//��������ݷ��ͣ�ֱ�ӷ��أ����°����ʹ���������ݷ���
		if(iRet > 0)
		{
			m_streamInfoLock.UnLock();
			return iRet;
		}
	}
	m_streamInfoLock.UnLock();

	m_jpegStreamLock.Lock();
	if(NULL == m_pJpegStreamInfo)
	{
		m_jpegStreamLock.UnLock();
		return 0;
	}
	iRet = GetJpegData(m_pJpegStreamInfo->dwStreamID, pDestBuf, bufSize, pDataHead);
	m_jpegStreamLock.UnLock();

	if(iRet > 0)
	{
		return iRet;
	}

	return 0;
}

/******************************************************************************
*
*							private method
*
******************************************************************************/

/******************************************************************************
*
*����Ƶ�����б���ȡ���ݷ���
*����ֵ��0�������ݳɹ�
		 -1��������ʧ��
		 -2û�����ݿɷ���
*
******************************************************************************/
int	CAVDataSendMan::SendVideo(unsigned long dwStreamID)
{
	int				iRet = 0;
	unsigned long	dwFrameBuffIndex = 0;

	//m_streamInfoLock.Lock();

	CLIENT_STREAM_INFO * pStreamInfo = GetStreamByID(dwStreamID);
	if(pStreamInfo == NULL)
	{
		assert(false);
		//m_streamInfoLock.UnLock();
		return -2;
	}

	unsigned long videoListCount = 1;
	if(pStreamInfo->bLiveStream)
	{
		videoListCount = m_dwMaxVideo;
	}
	for (dwFrameBuffIndex = 1; dwFrameBuffIndex <= videoListCount; ++dwFrameBuffIndex) 
	{		
		assert(pStreamInfo->dwSendVideoIndex <= videoListCount);

		//ͬһ��ͨ��������ͬʱ�����������
		if (!pStreamInfo->bLiveStream || PUB_HasBit64(pStreamInfo->fastVideoInfo.ullVideobit, pStreamInfo->dwSendVideoIndex)\
			|| PUB_HasBit64(pStreamInfo->slowVideoInfo.ullVideobit, pStreamInfo->dwSendVideoIndex)) 
		{
			//�����û���ϴ�ȡ����û�з��ͳ�ȥ����Ƶ����
			if(pStreamInfo->vDataToSendLen > 0)
			{
				if(pStreamInfo->combinedInfo.dwUsed == 1)	//����һ���ְ�
				{
					iRet = m_pMultiNetComm->SendData(m_clientID, pStreamInfo->szVSendData, \
						pStreamInfo->vDataToSendLen, false);

					if(iRet != 0)
					{
						iRet = -1;
					}
					else if (pStreamInfo->combinedData.dwIndex == pStreamInfo->combinedData.dwTotalPack) //���������һ���ְ�
					{
						pStreamInfo->vDataToSendLen = 0;
						pStreamInfo->combinedInfo.dwUsed = 0;
						pStreamInfo->dwLastDataID = INVALID_DATA_ID;
						iRet = 0;
					}
					else
					{
						pStreamInfo->vDataToSendLen = 0;
						pStreamInfo->combinedInfo.dwLen += pStreamInfo->combinedData.dwLen;
						iRet = 1;
					}
				}
				else	//����һ������(����С)
				{
					//������Ƶ����
					iRet = m_pMultiNetComm->SendData(m_clientID, pStreamInfo->szVSendData+SUB_PACK_HEAD_LEN, \
						pStreamInfo->vDataToSendLen, false);
					if (0 != iRet)
					{
						iRet = -1;
					}
					else
					{
						//���ͳɹ�����������������ݶ���
						pStreamInfo->vDataToSendLen = 0;
						iRet = 0;
					}
				}
			}
			else	
			{
				iRet = SplitAndSendData(dwStreamID, *(pStreamInfo->ppVideoDataList[pStreamInfo->dwSendVideoIndex]), 
					pStreamInfo->combinedData, pStreamInfo->combinedInfo, pStreamInfo->dwLastDataID);
			}

			if (0 == iRet) 
			{				
				pStreamInfo->dwSendVideoIndex = (pStreamInfo->dwSendVideoIndex + 1) % videoListCount;
				//m_streamInfoLock.UnLock();
				return 0;
			}
			else if ( 1 == iRet )
			{
				//m_streamInfoLock.UnLock();
				return 0;
			}
			else if (-2 == iRet) 
			{
				pStreamInfo->dwSendVideoIndex = (pStreamInfo->dwSendVideoIndex + 1) % videoListCount;
				continue;
			}
			else
			{
				//m_streamInfoLock.UnLock();
				return -1;
			}
		}
		else	
		{
			pStreamInfo->dwSendVideoIndex = (pStreamInfo->dwSendVideoIndex + 1) % videoListCount;
		}
	}

	//m_streamInfoLock.UnLock();

	//û�����ݿɷ���
	return -2;
}

/******************************************************************************
*
*����Ƶ�����б���ȡ���ݷ���
*����ֵ��0�������ݳɹ�
		 -1��������ʧ��
		 -2û�����ݿɷ���
*
******************************************************************************/
int	CAVDataSendMan::SendAudio(unsigned long dwStreamID)
{
	int	iRet = 0;
	bool bSameID = false;
	FRAME_INFO_EX	*pFrameInfoEx = NULL;
	unsigned long	dwAudioDataID = INVALID_DATA_ID;
	unsigned long	dwBuffLen = COMBINED_DATA_SPLIT_LEN;
	unsigned long	dwSendAudioIndex = 0;
	dwAudioDataID = INVALID_DATA_ID;
	dwBuffLen = COMBINED_DATA_SPLIT_LEN;

// 	if(m_pTalkDataList->GetFrame(dwAudioDataID, &pFrameInfoEx, m_szSendData, dwBuffLen, 0, bSameID))
// 	{
// 		if (!bSameID)
// 		{
// 			//������ȡһ����Ƶ���ò���ģ������ˣ�֤������һ���������꣬�����Ҫ���޸�
// 			assert(COMBINED_DATA_SPLIT_LEN != dwBuffLen);
// 			pFrameInfoEx->streamID = 0;	
// 			
// 			//������Ƶ����
// 			iRet = m_pTCPLink->SendSyn(m_szSendData, dwBuffLen);	
// 			if (0 != iRet) 
// 			{
// 				return 2;
// 			}
// 			else
// 			{
// 				return 0;
// 			}
// 		}
// 		else
// 		{
// 			assert(false);
// 			PUB_PRINTF_ERROR("%s %d\n",__FILE__,__LINE__);				
// 		}
// 	}	

	//����Խ�ҲҪ��ID������Ҫ��������ŵ�����ȥ
	//m_streamInfoLock.Lock();

	CLIENT_STREAM_INFO * pStreamInfo = GetStreamByID(dwStreamID);

	unsigned long audioListCount = 0;
	if(pStreamInfo->bLiveStream)
	{
		audioListCount = m_dwMaxAudio;
	}

	for(dwSendAudioIndex = 0; dwSendAudioIndex < audioListCount; ++dwSendAudioIndex)
	{
		//�����û���ϴ�ȡ����û�з��ͳ�ȥ����Ƶ����
		if(pStreamInfo->aDataToSendLen > 0)
		{
			//������Ƶ����
			iRet = m_pMultiNetComm->SendData(m_clientID, pStreamInfo->szASendData, pStreamInfo->aDataToSendLen, false);
			if (0 != iRet) 
			{
				//m_streamInfoLock.UnLock();
				return -1;
			}
			else
			{
				pStreamInfo->aDataToSendLen = 0;
				//m_streamInfoLock.UnLock();
				return 0;
			}
		}
		else if(pStreamInfo->ppAudioDataList[dwSendAudioIndex]->GetFrame(dwAudioDataID, &pFrameInfoEx, pStreamInfo->szASendData, dwBuffLen, 0, bSameID))
		{
			if (!bSameID)
			{
				//������ȡһ����Ƶ���ò���ģ������ˣ�֤������һ���������꣬�����Ҫ���޸�
				assert(COMBINED_DATA_SPLIT_LEN != dwBuffLen);
				pFrameInfoEx->streamID = pStreamInfo->dwStreamID;

				//������Ƶ����
				iRet = m_pMultiNetComm->SendData(m_clientID, pStreamInfo->szASendData, dwBuffLen, false);
				if (0 != iRet) 
				{
					pStreamInfo->aDataToSendLen = dwBuffLen;
					//m_streamInfoLock.UnLock();
					return -1;
				}
				else
				{
					//m_streamInfoLock.UnLock();
					return 0;
				}
			}
			else
			{
				assert(false);
				PUB_PRINTF_ERROR("%s %d\n",__FILE__,__LINE__);				
			}
		}	
	}
	//m_streamInfoLock.UnLock();
	return -2;
}

int CAVDataSendMan::GetVideoData(unsigned long dwStreamID, unsigned char *pDataBuf, long bufSize, unsigned char *&pDataHead)
{
	int				iRet = 0;
	unsigned long	dwFrameBuffIndex = 0;

	//m_streamInfoLock.Lock();

	CLIENT_STREAM_INFO * pStreamInfo = GetStreamByID(dwStreamID);
	if(pStreamInfo == NULL)
	{
		assert(false);
		//m_streamInfoLock.UnLock();
		return 0;
	}

	unsigned long videoListCount = 1;
	if(pStreamInfo->bLiveStream)
	{
		videoListCount = m_dwMaxVideo;
	}

	for (dwFrameBuffIndex = 1; dwFrameBuffIndex <= videoListCount; ++dwFrameBuffIndex) 
	{		
		assert(pStreamInfo->dwSendVideoIndex < videoListCount);

		//ͬһ��ͨ��������ͬʱ�����������
		if (!pStreamInfo->bLiveStream || PUB_HasBit64(pStreamInfo->fastVideoInfo.ullVideobit, pStreamInfo->dwSendVideoIndex)\
			|| PUB_HasBit64(pStreamInfo->slowVideoInfo.ullVideobit, pStreamInfo->dwSendVideoIndex)) 
		{
			bool			bSameID = false;
			FRAME_INFO_EX	*pFrameInfoEx = NULL;
			unsigned long	dwBuffLen = COMBINED_DATA_SPLIT_LEN;	
			unsigned long	dwRTDataLen = 0;						//ʵ�ʵ�����Ƶ���ݵĳ���
			unsigned long   dwTotalLen = 0;							//������Ƶ����ʱ�ã�Э��ͷ + ����ͷ + ���� ���ܳ���
			CFrameBuffer	&dataList = *(pStreamInfo->ppVideoDataList[pStreamInfo->dwSendVideoIndex]);

			if(1 == pStreamInfo->combinedInfo.dwUsed)	//�����ҷְ����ݷ���
			{
				//���㻹��Ҫ���Ͷ�������
				dwBuffLen = COMBINED_DATA_SPLIT_LEN;
				if (dataList.GetFrame( pStreamInfo->dwLastDataID, &pFrameInfoEx, pDataBuf+SUB_PACK_HEAD_LEN, dwBuffLen, pStreamInfo->combinedInfo.dwLen, bSameID))
				{
					assert(dwBuffLen <= COMBINED_DATA_SPLIT_LEN);
					if (bSameID) //���ŷ��ͷְ�����
					{
						pStreamInfo->combinedData.dwIndex += 1;			//�ְ���ż�1
						pStreamInfo->combinedData.dwLen = dwBuffLen;	

						//�ְ�����
						memcpy(pDataBuf, &PACKED_DATA_FLAG, sizeof(PACKED_DATA_FLAG));
						memcpy(pDataBuf+sizeof(PACKED_DATA_FLAG), &pStreamInfo->combinedData, sizeof(NET_COMBINED_DATA));

						//���������һ���ְ�
						if (pStreamInfo->combinedData.dwIndex == pStreamInfo->combinedData.dwTotalPack) 
						{	
							pStreamInfo->combinedInfo.dwUsed = 0;												
							pStreamInfo->dwLastDataID = INVALID_DATA_ID;
							pStreamInfo->dwSendVideoIndex = (pStreamInfo->dwSendVideoIndex + 1) % videoListCount;
						}
						else
						{
							pStreamInfo->combinedInfo.dwLen += pStreamInfo->combinedData.dwLen;
						}

						pDataHead = pDataBuf;

						//m_streamInfoLock.UnLock();
						return dwBuffLen+SUB_PACK_HEAD_LEN;
					}
					else	//��һ���µ�����
					{
						pStreamInfo->combinedInfo.dwUsed = 0; //������ߵ�����ķ���������
					}
				}
				else
				{
					pStreamInfo->dwSendVideoIndex = (pStreamInfo->dwSendVideoIndex + 1) % videoListCount;
					continue;
				}
			}
			else	//��һ��û��δ������ķְ����ݣ�������µ�����
			{
				dwBuffLen = COMBINED_DATA_SPLIT_LEN;
				pStreamInfo->dwLastDataID = INVALID_DATA_ID;
				if(!dataList.GetFrame(pStreamInfo->dwLastDataID, &pFrameInfoEx, pDataBuf+SUB_PACK_HEAD_LEN, dwBuffLen, 0, bSameID))
				{
					pStreamInfo->dwSendVideoIndex = (pStreamInfo->dwSendVideoIndex + 1) % videoListCount;
					continue;
				}
				assert(dwBuffLen <= COMBINED_DATA_SPLIT_LEN);
			}

			//����������
			pFrameInfoEx->streamID = dwStreamID;

			//Ϊ��4�ֽڶ���
			unsigned long dwResidual = pFrameInfoEx->length % 4;
			dwRTDataLen = pFrameInfoEx->length + ((dwResidual > 0) ? (4 - dwResidual) : 0);
			dwTotalLen = (reinterpret_cast<unsigned char*>(pFrameInfoEx) - (pDataBuf + SUB_PACK_HEAD_LEN)) \
				+ sizeof(FRAME_INFO_EX) + dwRTDataLen;

			if (dwTotalLen <= COMBINED_DATA_SPLIT_LEN )		//һ�ξ��ܷ��꣬����ְ�
			{
				pStreamInfo->dwLastDataID = INVALID_DATA_ID;				//�´μ����������ݷ���

				pDataHead =  pDataBuf+SUB_PACK_HEAD_LEN;


				pStreamInfo->dwSendVideoIndex = (pStreamInfo->dwSendVideoIndex + 1) % videoListCount;

				//m_streamInfoLock.UnLock();
				return dwTotalLen;
			}
			else	// ��Ҫ�ְ����ȷ�һ����
			{
				//�õ�������ݵ�������Ϣ
				pStreamInfo->combinedInfo.dwTotalLen = dwTotalLen;
				pStreamInfo->combinedInfo.dwUsed = 1;
				pStreamInfo->combinedInfo.dwDataID = ++m_dwDataID;
				pStreamInfo->combinedInfo.dwLen = 0;

				//�ܹ���Ƶ����
				if (0 == dwTotalLen % COMBINED_DATA_SPLIT_LEN) 
				{
					pStreamInfo->combinedInfo.dwTotalPack = dwTotalLen / COMBINED_DATA_SPLIT_LEN;
				}
				else
				{
					pStreamInfo->combinedInfo.dwTotalPack = dwTotalLen / COMBINED_DATA_SPLIT_LEN + 1;
				}

				//ÿ�η����������ʱ�ĸ�����Ϣ
				pStreamInfo->combinedData.dwDataID = pStreamInfo->combinedInfo.dwDataID;
				pStreamInfo->combinedData.dwIndex = 1;
				pStreamInfo->combinedData.dwLen = COMBINED_DATA_SPLIT_LEN;
				pStreamInfo->combinedData.dwTotalLen = pStreamInfo->combinedInfo.dwTotalLen;
				pStreamInfo->combinedData.dwTotalPack = pStreamInfo->combinedInfo.dwTotalPack;

				memcpy(pDataBuf, &PACKED_DATA_FLAG, sizeof(PACKED_DATA_FLAG));
				memcpy(pDataBuf+sizeof(PACKED_DATA_FLAG), &pStreamInfo->combinedData, sizeof(NET_COMBINED_DATA));

				//�����Ѿ����͵ķְ����ݳ���
				pStreamInfo->combinedInfo.dwLen = COMBINED_DATA_SPLIT_LEN;

				pDataHead = pDataBuf;

				//m_streamInfoLock.UnLock();
				return COMBINED_DATA_SPLIT_LEN+SUB_PACK_HEAD_LEN;
			}
		}
		else	
		{
			pStreamInfo->dwSendVideoIndex = (pStreamInfo->dwSendVideoIndex + 1) % videoListCount;
		}
	}

	//m_streamInfoLock.UnLock();

	//û�����ݿɷ���
	return 0;
}

int CAVDataSendMan::GetAudioData(unsigned long dwStreamID, unsigned char *pDataBuf, long bufSize)
{
	int	iRet = 0;
	bool bSameID = false;
	FRAME_INFO_EX	*pFrameInfoEx = NULL;
	unsigned long	dwAudioDataID = INVALID_DATA_ID;
	unsigned long	dwBuffLen = COMBINED_DATA_SPLIT_LEN;
	unsigned long	dwSendAudioIndex = 0;
	dwAudioDataID = INVALID_DATA_ID;
	dwBuffLen = COMBINED_DATA_SPLIT_LEN;

	//m_streamInfoLock.Lock();

	CLIENT_STREAM_INFO * pStreamInfo = GetStreamByID(dwStreamID);

	unsigned long audioListCount = 0;
	if(pStreamInfo->bLiveStream)
	{
		audioListCount = m_dwMaxAudio;
	}

	for(dwSendAudioIndex = 0; dwSendAudioIndex < audioListCount; ++dwSendAudioIndex)
	{
		if(pStreamInfo->ppAudioDataList[dwSendAudioIndex]->GetFrame(dwAudioDataID, &pFrameInfoEx, pDataBuf, dwBuffLen, 0, bSameID))
		{
			if (!bSameID)
			{
				//������ȡһ����Ƶ���ò���ģ������ˣ�֤������һ���������꣬�����Ҫ���޸�
				assert(COMBINED_DATA_SPLIT_LEN != dwBuffLen);
				pFrameInfoEx->streamID = pStreamInfo->dwStreamID;

				//������Ƶ����
				//m_streamInfoLock.UnLock();
				return dwBuffLen;
			}
			else
			{
				assert(false);
				PUB_PRINTF_ERROR("%s %d\n",__FILE__,__LINE__);				
			}
		}	
	}
	//m_streamInfoLock.UnLock();
	return 0;
}

/******************************************************************************
*
*�������ģ����δ������Ļط�����
*����ֵ��0������һ�������İ�
		 1����������һ���ְ�
		 -1����ʧ��
		 -2û�����ݷ���
*
******************************************************************************/
int CAVDataSendMan::SplitAndSendData(unsigned long dwStreamID, CFrameBuffer& dataList, 
									 NET_COMBINED_DATA& combinedData, NET_COMBINED_INFO& combinedInfo, unsigned long& dwLastDataID)
{
	bool	bSameID = false;
	int		iRet = 0;
	FRAME_INFO_EX	*pFrameInfoEx = NULL;

	unsigned long	dwBuffLen = COMBINED_DATA_SPLIT_LEN;	
	unsigned long	dwRTDataLen = 0;						//ʵ�ʵ�����Ƶ���ݵĳ���
	unsigned long   dwTotalLen = 0;							//������Ƶ����ʱ�ã�Э��ͷ + ����ͷ + ���� ���ܳ���

	CLIENT_STREAM_INFO * pStreamInfo = GetStreamByID(dwStreamID);

	if(1 == combinedInfo.dwUsed)	//�����ҷְ����ݷ���
	{
		//���㻹��Ҫ���Ͷ�������
		dwBuffLen = COMBINED_DATA_SPLIT_LEN;
		if (dataList.GetFrame(dwLastDataID, &pFrameInfoEx, pStreamInfo->szVSendData+SUB_PACK_HEAD_LEN, dwBuffLen, combinedInfo.dwLen, bSameID))
		{
			assert(dwBuffLen <= COMBINED_DATA_SPLIT_LEN);
			if (bSameID) //���ŷ��ͷְ�����
			{
				combinedData.dwIndex += 1;			//�ְ���ż�1
				combinedData.dwLen = dwBuffLen;	

				//���ͷְ�����
				memcpy(pStreamInfo->szVSendData, &PACKED_DATA_FLAG, sizeof(PACKED_DATA_FLAG));
				memcpy(pStreamInfo->szVSendData+sizeof(PACKED_DATA_FLAG), &combinedData, sizeof(NET_COMBINED_DATA));

				iRet = m_pMultiNetComm->SendData(m_clientID, pStreamInfo->szVSendData, dwBuffLen+SUB_PACK_HEAD_LEN, false);
				if (0 != iRet) 
				{
					pStreamInfo->vDataToSendLen = dwBuffLen+SUB_PACK_HEAD_LEN;
					return -1;
				}
				else
				{	
					//���������һ���ְ�
					if (combinedData.dwIndex == combinedData.dwTotalPack) 
					{					
						combinedInfo.dwUsed = 0;												
						dwLastDataID = INVALID_DATA_ID;
						return 0;
					}
					else
					{
						combinedInfo.dwLen += combinedData.dwLen;
						return 1;
					}
				}
			}
			else	//��һ���µ�����
			{
				combinedInfo.dwUsed = 0; //������ߵ�����ķ���������
			}
		}
		else
		{
			return -2;
		}
	}
	else	//��һ��û��δ������ķְ����ݣ�������µ�����
	{
		dwBuffLen = COMBINED_DATA_SPLIT_LEN;
		dwLastDataID = INVALID_DATA_ID;
		if(!dataList.GetFrame(dwLastDataID, &pFrameInfoEx, pStreamInfo->szVSendData+SUB_PACK_HEAD_LEN, dwBuffLen, 0, bSameID))
		{
			return -2;
		}
		assert(dwBuffLen <= COMBINED_DATA_SPLIT_LEN);
	}

	//����������
	pFrameInfoEx->streamID = dwStreamID;

	//Ϊ��4�ֽڶ���
	unsigned long dwResidual = pFrameInfoEx->length % 4;
	dwRTDataLen = pFrameInfoEx->length + ((dwResidual > 0) ? (4 - dwResidual) : 0);
	dwTotalLen = (reinterpret_cast<unsigned char*>(pFrameInfoEx) - (pStreamInfo->szVSendData + SUB_PACK_HEAD_LEN)) \
		+ sizeof(FRAME_INFO_EX) + dwRTDataLen;

	if (dwTotalLen <= COMBINED_DATA_SPLIT_LEN )		//һ�ξ��ܷ��꣬����ְ�
	{
		dwLastDataID = INVALID_DATA_ID;				//�´μ����������ݷ���

		//������Ƶ����
		iRet = m_pMultiNetComm->SendData(m_clientID, pStreamInfo->szVSendData+SUB_PACK_HEAD_LEN, dwTotalLen, false);	
		if (0 != iRet) 
		{
			pStreamInfo->vDataToSendLen = dwTotalLen;
			return -1;
		}
		else
		{
			return 0;
		}
	}
	else	// ��Ҫ�ְ����ȷ�һ����
	{
		//�õ�������ݵ�������Ϣ
		combinedInfo.dwTotalLen = dwTotalLen;
		combinedInfo.dwUsed = 1;
		combinedInfo.dwDataID = ++m_dwDataID;
		combinedInfo.dwLen = 0;

		//�ܹ���Ƶ����
		if (0 == dwTotalLen % COMBINED_DATA_SPLIT_LEN) 
		{
			combinedInfo.dwTotalPack = dwTotalLen / COMBINED_DATA_SPLIT_LEN;
		}
		else
		{
			combinedInfo.dwTotalPack = dwTotalLen / COMBINED_DATA_SPLIT_LEN + 1;
		}

		//ÿ�η����������ʱ�ĸ�����Ϣ
		combinedData.dwDataID = combinedInfo.dwDataID;
		combinedData.dwIndex = 1;
		combinedData.dwLen = COMBINED_DATA_SPLIT_LEN;
		combinedData.dwTotalLen = combinedInfo.dwTotalLen;
		combinedData.dwTotalPack = combinedInfo.dwTotalPack;

		memcpy(pStreamInfo->szVSendData, &PACKED_DATA_FLAG, sizeof(PACKED_DATA_FLAG));
		memcpy(pStreamInfo->szVSendData+sizeof(PACKED_DATA_FLAG), &combinedData, sizeof(NET_COMBINED_DATA));

		//���ͷְ�����
		iRet = m_pMultiNetComm->SendData(m_clientID, pStreamInfo->szVSendData, COMBINED_DATA_SPLIT_LEN+SUB_PACK_HEAD_LEN, false);
		if (0 != iRet) 
		{
			pStreamInfo->vDataToSendLen = COMBINED_DATA_SPLIT_LEN+SUB_PACK_HEAD_LEN;
			return -1;
		}
		else
		{
			//�����Ѿ����͵ķְ����ݳ���
			combinedInfo.dwLen = COMBINED_DATA_SPLIT_LEN;

			//���ͳɹ�����������������ݶ���
			return 1;
		}
	}
}

CLIENT_STREAM_INFO * CAVDataSendMan::GetStreamByID(unsigned long dwStreamID)
{
	CLIENT_STREAM_INFO * pStreamInfo = NULL;
	POS pos = m_lstClientStream.GetHeadPosition();
	while(pos)
	{
		pStreamInfo = m_lstClientStream.GetNext(pos);
		if(pStreamInfo->dwStreamID == dwStreamID)
		{
			return pStreamInfo;
		}
	}

	return NULL;
}

//frameType: FRAME_TYPE_VIDEO or FRAME_TYPE_AUDIO�� ֻͳ�������ֳ���������
ULONGLONG CAVDataSendMan::GetStreamArrivedBytes(unsigned long channel, unsigned long frameType)
{
	if(FRAME_TYPE_VIDEO == frameType)
	{
		assert(CHANNEL_ID(channel) < m_dwMaxVideo);
		if(CHANNEL_ID(channel) >= m_dwMaxVideo)
		{
			return 0;
		}

		return m_pullArrivedVideoBytes[CHANNEL_ID(channel)];
	}
	else if(FRAME_TYPE_AUDIO == frameType)
	{
		assert(CHANNEL_ID(channel) < m_dwMaxAudio);
		if(CHANNEL_ID(channel) >= m_dwMaxAudio)
		{
			return 0;
		}

		return m_pullArrivedAudioBytes[CHANNEL_ID(channel)];
	}
	else
	{
		assert(false);
		return 0;
	}
}

ULONGLONG CAVDataSendMan::GetStreamSentBytes(unsigned long channel, unsigned long frameType)
{
	if(FRAME_TYPE_VIDEO == frameType)
	{
		assert(CHANNEL_ID(channel) < m_dwMaxVideo);
		if(CHANNEL_ID(channel) >= m_dwMaxVideo)
		{
			return 0;
		}

		return m_pullSentVideoBytes[CHANNEL_ID(channel)];
	}
	else if(FRAME_TYPE_AUDIO == frameType)
	{
		assert(CHANNEL_ID(channel) < m_dwMaxAudio);
		if(CHANNEL_ID(channel) >= m_dwMaxAudio)
		{
			return 0;
		}

		return m_pullSentAudioBytes[CHANNEL_ID(channel)];
	}
	else
	{
		assert(false);
		return 0;
	}
}

LONGLONG CAVDataSendMan::GetStatisticTime(unsigned long channel)
{
	return m_pPreLiveFrameTime[CHANNEL_ID(channel)] - m_pFirstLiveFrameTime[CHANNEL_ID(channel)];
}

void CAVDataSendMan::ResetStreamBytesStatistics(unsigned long channel /* = ALL_CHANNELS */)
{
	m_streamInfoLock.Lock();

	if(channel != ALL_CHANNELS)
	{
		m_pullArrivedVideoBytes[CHANNEL_ID(channel)] = 0;
		m_pullSentVideoBytes[CHANNEL_ID(channel)] = 0;
		m_pFirstLiveFrameTime[CHANNEL_ID(channel)] = 0;
		m_pPreLiveFrameTime[CHANNEL_ID(channel)] = 0;

		if(CHANNEL_ID(channel) < m_dwMaxAudio)
		{
			m_pullArrivedAudioBytes[CHANNEL_ID(channel)] = 0;
			m_pullSentAudioBytes[CHANNEL_ID(channel)] = 0;
		}
	}
	else
	{
		int ch = 0;
		for(ch = 0; ch < m_dwMaxVideo; ch++)
		{
			m_pullArrivedVideoBytes[ch] = 0;
			m_pullSentVideoBytes[ch] = 0;
			m_pFirstLiveFrameTime[ch] = 0;
			m_pPreLiveFrameTime[ch] = 0;
		}
		for(ch = 0; ch < m_dwMaxAudio; ch++)
		{
			m_pullArrivedAudioBytes[ch] = 0;
			m_pullSentAudioBytes[ch] = 0;
		}
	}

	m_streamInfoLock.UnLock();
}

void CAVDataSendMan::ResetStreamBytesStatisticsEx(unsigned long channel/* = ALL_CHANNELS*/)
{
	if(channel != ALL_CHANNELS)
	{
		m_pullArrivedVideoBytes[CHANNEL_ID(channel)] = 0;
		m_pullSentVideoBytes[CHANNEL_ID(channel)] = 0;
		m_pFirstLiveFrameTime[CHANNEL_ID(channel)] = 0;
		m_pPreLiveFrameTime[CHANNEL_ID(channel)] = 0;

		if(CHANNEL_ID(channel) < m_dwMaxAudio)
		{
			m_pullArrivedAudioBytes[CHANNEL_ID(channel)] = 0;
			m_pullSentAudioBytes[CHANNEL_ID(channel)] = 0;
		}
	}
	else
	{
		int ch = 0;
		for(ch = 0; ch < m_dwMaxVideo; ch++)
		{
			m_pullArrivedVideoBytes[ch] = 0;
			m_pullSentVideoBytes[ch] = 0;
			m_pFirstLiveFrameTime[ch] = 0;
			m_pPreLiveFrameTime[ch] = 0;
		}
		for(ch = 0; ch < m_dwMaxAudio; ch++)
		{
			m_pullArrivedAudioBytes[ch] = 0;
			m_pullSentAudioBytes[ch] = 0;
		}
	}
}

void CAVDataSendMan::StartTalkStream()
{
	m_talkStreamLock.Lock();

	assert(NULL == m_pTalkDataList);
	if(m_pTalkDataList != NULL)
	{
		delete m_pTalkDataList;
	}

	m_pTalkDataList = new CFrameBuffer(NET_STREAM_AUDIO, 50*1024, 2);
	m_bTalkNeedFormat = true;

	m_talkStreamLock.UnLock();
}

void CAVDataSendMan::StopTalkStream()
{
	m_talkStreamLock.Lock();

	assert(NULL != m_pTalkDataList);

	if(m_pTalkDataList != NULL)
	{
		m_pTalkDataList->RemoveAll();
		delete m_pTalkDataList;
		m_pTalkDataList = NULL;
	}

	m_talkStreamLock.UnLock();
}

long CAVDataSendMan::PutTalkData(CFrameData *pFrameData)
{
	FRAME_INFO_EX & frameInfo = pFrameData->GetFrameInfo();

	//�����ܷǶԽ�����
	assert((frameInfo.frameType == FRAME_TYPE_TALK_AUDIO) || \
		(frameInfo.frameType == FRAME_TYPE_TALK_AUDIO_FORMAT));
	if((frameInfo.frameType != FRAME_TYPE_TALK_AUDIO) && (frameInfo.frameType != FRAME_TYPE_TALK_AUDIO_FORMAT))
	{
		return SEND_FRAME_SUCC;
	}


	m_talkStreamLock.Lock();

	//û�п����Խ������ݹ��˵�
	if(NULL == m_pTalkDataList)
	{
		m_talkStreamLock.UnLock();
		return SEND_FRAME_SUCC;
	}

	//�ǶԽ����ݸ�ʽ
	if(frameInfo.frameType == FRAME_TYPE_TALK_AUDIO_FORMAT)
	{
		m_bTalkNeedFormat = false;
	}
	
	//��Ҫ�Խ���ʽ
	if(m_bTalkNeedFormat)
	{
		m_talkStreamLock.UnLock();
		return SEND_FRAME_NEED_FORMAT;
	}

	//�ѶԽ����ݼ��뵽�б���
	bool bKeyFrame;
	m_pTalkDataList->AddFrame(pFrameData, bKeyFrame);

	m_talkStreamLock.UnLock();
	return SEND_FRAME_SUCC;
}

//���ض�ȡ�����ֽ���
int CAVDataSendMan::GetTalkData(unsigned char *pDataBuf, long bufSize)
{
	if(NULL == m_pTalkDataList)
	{
		return 0;
	}

	int	iRet = 0;
	bool bSameID = false;
	FRAME_INFO_EX	*pFrameInfoEx = NULL;
	unsigned long	dwAudioDataID = INVALID_DATA_ID;
	unsigned long	dwBuffLen = COMBINED_DATA_SPLIT_LEN;
	
	m_talkStreamLock.Lock();

	if((NULL != m_pTalkDataList) && m_pTalkDataList->GetFrame(dwAudioDataID, &pFrameInfoEx, pDataBuf, dwBuffLen, 0, bSameID))
	{
		if (!bSameID)
		{
			//������ȡһ����Ƶ���ò���ģ������ˣ�֤������һ���������꣬�����Ҫ���޸�
			assert(COMBINED_DATA_SPLIT_LEN != dwBuffLen);
			pFrameInfoEx->streamID = 0;

			//������Ƶ����
			m_talkStreamLock.UnLock();
			return dwBuffLen;
		}
		else
		{
			assert(false);
			PUB_PRINTF_ERROR("%s %d\n",__FILE__,__LINE__);			
		}
	}

	m_talkStreamLock.UnLock();

	return 0;
}

void CAVDataSendMan::StartJpegStream(ULONGLONG ullJpegChannelbit, unsigned long dwStreamID)
{
	m_jpegStreamLock.Lock();

	m_pJpegStreamInfo = new CLIENT_STREAM_INFO;
	memset(m_pJpegStreamInfo, 0, sizeof(CLIENT_STREAM_INFO));

	m_pJpegStreamInfo->dwLastDataID	= INVALID_DATA_ID;
	m_pJpegStreamInfo->dwStreamID	= dwStreamID;

	//����������ʼ�ĸ���Ƶ����Ƶͨ������Ϣ	
	m_pJpegStreamInfo->jpegInfo.ullVideobit		= ullJpegChannelbit;
	m_pJpegStreamInfo->bFirstFrame = true;

	m_pJpegStreamInfo->ppVideoDataList = new CFrameBuffer*[m_dwMaxVideo];
	for (unsigned long i = 0; i < m_dwMaxVideo; ++i) 
	{
		m_pJpegStreamInfo->ppVideoDataList[i] = new CFrameBuffer(NET_STREAM_VIDEO, 512*1024, 5);
	}

	m_jpegStreamLock.UnLock();
}

void CAVDataSendMan::ChangeJpegStream(ULONGLONG ullJpegChannelbit, unsigned long dwStreamID)
{
	m_jpegStreamLock.Lock();

	if (NULL != m_pJpegStreamInfo)
	{
		ULONGLONG	  ullJpegBitOld;	//�ı�ǰ��JPEGͨ��

		ullJpegBitOld = m_pJpegStreamInfo->jpegInfo.ullVideobit;
		m_pJpegStreamInfo->jpegInfo.ullVideobit = ullJpegChannelbit;

		unsigned long i = 0;
		for (i = 0; i < m_dwMaxVideo; ++i)
		{
			//�����ǰ���ˣ�����û����ͨ�����ͷ�û���������
			if (PUB_HasBit64(ullJpegBitOld, static_cast<int>(i)) && !PUB_HasBit64(ullJpegChannelbit, static_cast<int>(i)))
			{
				m_pJpegStreamInfo->ppVideoDataList[i]->RemoveAll();
			}
		}
	}

	m_jpegStreamLock.UnLock();
}

void CAVDataSendMan::StopJpegStream(unsigned long dwStreamID)
{
	unsigned long i = 0;

	m_jpegStreamLock.Lock();

	if(m_pJpegStreamInfo == NULL)
	{
		//ֹͣһ��δ����������
		m_jpegStreamLock.UnLock();
		return;
	}

	//�����Դ
	for (i = 0; i < m_dwMaxVideo; ++i) 
	{
		m_pJpegStreamInfo->ppVideoDataList[i]->RemoveAll();
		delete m_pJpegStreamInfo->ppVideoDataList[i];
	}
	delete [] m_pJpegStreamInfo->ppVideoDataList;

	delete m_pJpegStreamInfo;
	m_pJpegStreamInfo = NULL;

	m_jpegStreamLock.UnLock();
}

long CAVDataSendMan::PutJpegData(CFrameData *pFrameData)
{
	bool bNeedKeyFrame = false;

	FRAME_INFO_EX &frameInfo = pFrameData->GetFrameInfo();
	if(frameInfo.frameType != FRAME_TYPE_JPEG)
	{
		return SEND_FRAME_SUCC;
	}

	m_jpegStreamLock.Lock();

	if(NULL == m_pJpegStreamInfo)
	{
		m_jpegStreamLock.UnLock();
		return SEND_FRAME_SUCC;
	}

	frameInfo.streamID = m_pJpegStreamInfo->dwStreamID;

	//�жϴ�JPEGͨ���Ƿ���
	if(!PUB_HasBit64(m_pJpegStreamInfo->jpegInfo.ullVideobit, 	static_cast<int>(CHANNEL_ID(frameInfo.channel))))
	{
		m_jpegStreamLock.UnLock();
		return SEND_FRAME_SUCC;
	}

	assert(CHANNEL_ID(frameInfo.channel) < m_dwMaxVideo );
	m_pJpegStreamInfo->ppVideoDataList[CHANNEL_ID(frameInfo.channel)]->AddFrame(pFrameData, bNeedKeyFrame);

	m_jpegStreamLock.UnLock();

	return SEND_FRAME_SUCC;
}

int CAVDataSendMan::GetJpegData(unsigned long dwStreamID, unsigned char *pDataBuf, long bufSize, unsigned char *&pDataHead)
{
	int				iRet = 0;
	unsigned long	dwFrameBuffIndex = 0;

	if(NULL == m_pJpegStreamInfo)
	{
		return 0;
	}

	for (dwFrameBuffIndex = 1; dwFrameBuffIndex <= m_dwMaxVideo; ++dwFrameBuffIndex) 
	{		
		assert(m_pJpegStreamInfo->dwSendVideoIndex < m_dwMaxVideo);

		if (PUB_HasBit64(m_pJpegStreamInfo->jpegInfo.ullVideobit, m_pJpegStreamInfo->dwSendVideoIndex)) 
		{
			bool			bSameID = false;
			FRAME_INFO_EX	*pFrameInfoEx = NULL;
			unsigned long	dwBuffLen = COMBINED_DATA_SPLIT_LEN;	
			unsigned long	dwRTDataLen = 0;						//ʵ�ʵ�����Ƶ���ݵĳ���
			unsigned long   dwTotalLen = 0;							//������Ƶ����ʱ�ã�Э��ͷ + ����ͷ + ���� ���ܳ���
			CFrameBuffer	&dataList = *(m_pJpegStreamInfo->ppVideoDataList[m_pJpegStreamInfo->dwSendVideoIndex]);

			if(1 == m_pJpegStreamInfo->combinedInfo.dwUsed)	//�����ҷְ����ݷ���
			{
				//���㻹��Ҫ���Ͷ�������
				dwBuffLen = COMBINED_DATA_SPLIT_LEN;
				if (dataList.GetFrame( m_pJpegStreamInfo->dwLastDataID, &pFrameInfoEx, pDataBuf+SUB_PACK_HEAD_LEN, dwBuffLen, \
					m_pJpegStreamInfo->combinedInfo.dwLen, bSameID))
				{
					assert(dwBuffLen <= COMBINED_DATA_SPLIT_LEN);
					if (bSameID) //���ŷ��ͷְ�����
					{
						m_pJpegStreamInfo->combinedData.dwIndex += 1;			//�ְ���ż�1
						m_pJpegStreamInfo->combinedData.dwLen = dwBuffLen;	

						//�ְ�����
						memcpy(pDataBuf, &PACKED_DATA_FLAG, sizeof(PACKED_DATA_FLAG));
						memcpy(pDataBuf+sizeof(PACKED_DATA_FLAG), &m_pJpegStreamInfo->combinedData, sizeof(NET_COMBINED_DATA));

						//���������һ���ְ�
						if (m_pJpegStreamInfo->combinedData.dwIndex == m_pJpegStreamInfo->combinedData.dwTotalPack) 
						{	
							m_pJpegStreamInfo->combinedInfo.dwUsed = 0;												
							m_pJpegStreamInfo->dwLastDataID = INVALID_DATA_ID;
							m_pJpegStreamInfo->dwSendVideoIndex = (m_pJpegStreamInfo->dwSendVideoIndex + 1) % m_dwMaxVideo;
						}
						else
						{
							m_pJpegStreamInfo->combinedInfo.dwLen += m_pJpegStreamInfo->combinedData.dwLen;
						}

						pDataHead = pDataBuf;

						return dwBuffLen+SUB_PACK_HEAD_LEN;
					}
					else	//��һ���µ�����
					{
						m_pJpegStreamInfo->combinedInfo.dwUsed = 0; //������ߵ�����ķ���������
					}
				}
				else
				{
					m_pJpegStreamInfo->dwSendVideoIndex = (m_pJpegStreamInfo->dwSendVideoIndex + 1) % m_dwMaxVideo;
					continue;
				}
			}
			else	//��һ��û��δ������ķְ����ݣ�������µ�����
			{
				dwBuffLen = COMBINED_DATA_SPLIT_LEN;
				m_pJpegStreamInfo->dwLastDataID = INVALID_DATA_ID;
				if(!dataList.GetFrame(m_pJpegStreamInfo->dwLastDataID, &pFrameInfoEx, pDataBuf+SUB_PACK_HEAD_LEN, dwBuffLen, 0, bSameID))
				{
					m_pJpegStreamInfo->dwSendVideoIndex = (m_pJpegStreamInfo->dwSendVideoIndex + 1) % m_dwMaxVideo;
					continue;
				}
				assert(dwBuffLen <= COMBINED_DATA_SPLIT_LEN);
			}

			//����������
			pFrameInfoEx->streamID = dwStreamID;

			//Ϊ��4�ֽڶ���
			unsigned long dwResidual = pFrameInfoEx->length % 4;
			dwRTDataLen = pFrameInfoEx->length + ((dwResidual > 0) ? (4 - dwResidual) : 0);
			dwTotalLen = (reinterpret_cast<unsigned char*>(pFrameInfoEx) - (pDataBuf + SUB_PACK_HEAD_LEN)) \
				+ sizeof(FRAME_INFO_EX) + dwRTDataLen;

			if (dwTotalLen <= COMBINED_DATA_SPLIT_LEN )		//һ�ξ��ܷ��꣬����ְ�
			{
				m_pJpegStreamInfo->dwLastDataID = INVALID_DATA_ID;				//�´μ����������ݷ���

				pDataHead =  pDataBuf+SUB_PACK_HEAD_LEN;


				m_pJpegStreamInfo->dwSendVideoIndex = (m_pJpegStreamInfo->dwSendVideoIndex + 1) % m_dwMaxVideo;

				return dwTotalLen;
			}
			else	// ��Ҫ�ְ����ȷ�һ����
			{
				//�õ�������ݵ�������Ϣ
				m_pJpegStreamInfo->combinedInfo.dwTotalLen = dwTotalLen;
				m_pJpegStreamInfo->combinedInfo.dwUsed = 1;
				m_pJpegStreamInfo->combinedInfo.dwDataID = ++m_dwDataID;
				m_pJpegStreamInfo->combinedInfo.dwLen = 0;

				//�ܹ���Ƶ����
				if (0 == dwTotalLen % COMBINED_DATA_SPLIT_LEN) 
				{
					m_pJpegStreamInfo->combinedInfo.dwTotalPack = dwTotalLen / COMBINED_DATA_SPLIT_LEN;
				}
				else
				{
					m_pJpegStreamInfo->combinedInfo.dwTotalPack = dwTotalLen / COMBINED_DATA_SPLIT_LEN + 1;
				}

				//ÿ�η����������ʱ�ĸ�����Ϣ
				m_pJpegStreamInfo->combinedData.dwDataID = m_pJpegStreamInfo->combinedInfo.dwDataID;
				m_pJpegStreamInfo->combinedData.dwIndex = 1;
				m_pJpegStreamInfo->combinedData.dwLen = COMBINED_DATA_SPLIT_LEN;
				m_pJpegStreamInfo->combinedData.dwTotalLen = m_pJpegStreamInfo->combinedInfo.dwTotalLen;
				m_pJpegStreamInfo->combinedData.dwTotalPack = m_pJpegStreamInfo->combinedInfo.dwTotalPack;

				memcpy(pDataBuf, &PACKED_DATA_FLAG, sizeof(PACKED_DATA_FLAG));
				memcpy(pDataBuf+sizeof(PACKED_DATA_FLAG), &m_pJpegStreamInfo->combinedData, sizeof(NET_COMBINED_DATA));

				//�����Ѿ����͵ķְ����ݳ���
				m_pJpegStreamInfo->combinedInfo.dwLen = COMBINED_DATA_SPLIT_LEN;

				pDataHead = pDataBuf;

				return COMBINED_DATA_SPLIT_LEN+SUB_PACK_HEAD_LEN;
			}
		}
		else	
		{
			m_pJpegStreamInfo->dwSendVideoIndex = (m_pJpegStreamInfo->dwSendVideoIndex + 1) % m_dwMaxVideo;
		}
	}

	//û�����ݿɷ���
	return 0;
}

