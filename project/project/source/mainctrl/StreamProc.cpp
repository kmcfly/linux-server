/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : yuanshiwei
** Date         : 2009-05-27
** Name         : StreamProc.cpp
** Version      : 1.0
** Description  :
** Modify Record:
1:
***********************************************************************/
#include "StreamProc.h"
#ifdef __NETSERVER_TVT__
#include "NetServer.h"
#endif

#ifdef __NETSERVER_HXHT__
#include "HxConnectMan.h"
#endif

#ifdef __DVR_ULTIMATE__
#include "IPDataBufferDefine.h"
#include "IPCDecDisplayMan.h"
#endif

#include "SnapPictureMan.h"
#include "SnapCapabilityMan.h"
#include "SnapMan.h"

#ifdef __CUSTOM_YUANZHENG__
#include "YZSnapMan.h"
#endif

#ifdef __SUPPORT_SNAPD1__
#include "ShanghaiSnapMan.h"
#endif

#ifdef __ALARM_TO_UPDATE_FTP_VIDEO__
#include "FTPUpdateMan.h"
#endif

#include "NetStreamAllotDevice.h"
#include "LocalSubStreamAllot.h"

#include "Product.h"

extern VIDEO_FORMAT		g_video_format;
extern bool g_bHaveSnapPic;
CStreamProc * g_pStreamProc = NULL;
extern bool				g_curNet3G;
extern bool g_preRecErrSleep; 
/************************************************************************/
/*                                public                                */
/************************************************************************/
CStreamProc::CStreamProc():m_pbSendAudioFormat(NULL), m_bStream(false), m_stream_ID(PUB_THREAD_ID_NOINIT), m_bStreamNet(false), m_streamNet_ID(PUB_THREAD_ID_NOINIT)
{
#ifdef __DVR_ULTIMATE__
	m_pBitmapInfo	= NULL;
	m_pWaveFormat	= NULL;
	m_pMinorBitmapInfo	= NULL;
	m_pMinorWaveFormat	= NULL;
	m_pSecondBitmapInfo = NULL;
	m_pNetDeviceMan = NULL;
	m_decCardNum	= 0;
#endif

	m_pVideoInfoMain	= NULL;
	m_pVideoInfoSub		= NULL;

	m_bSecondStreamAuto			= true;
	m_secondStreamCH			= 0;
	m_secondStreamCH_Mobile		= 0;
	m_secondSendAllNum          = 0;
	m_secondRateLimit           = INVALID_INDEX(unsigned long);
	m_pSecondStreamEncodeInfo	= NULL;
	m_pFirstStreamEncodeInfo	= NULL;
	m_bTalkStream				= false;
	m_bWouldStopTalk			= false;
	m_hSecondStreamID			= PUB_THREAD_ID_NOINIT;
	m_stream_ID					= PUB_THREAD_ID_NOINIT;

	m_streamNetSecond_ID		= PUB_THREAD_ID_NOINIT;
	m_bStreamNetSecond			= false;

	m_streamNetMinor_ID			= PUB_THREAD_ID_NOINIT;
	m_bStreamNetMinor			= false;

	m_secondLimitRateForFirstStreamNum = INVALID_INDEX(unsigned long);
	m_secondLimitAllRateForFirstStreamNum = INVALID_INDEX(unsigned long); 
	g_pStreamProc = this;

	m_nmgmTime = 0;
	m_nmrelativeTime = 0;
	m_nsgmTime = 0;
	m_nsrelativeTime = 0;
	
	m_bStreamAction = false;
	m_bSecondStreamAction = false;
	m_bNetStreamAction = false;
	m_bNetMinorStreamAction = false;
	m_bJPEGNetStreamAction = false;
#ifdef __DVR_BASIC__
	m_bJPEGStreamAction = false;
#endif
}

CStreamProc::~CStreamProc()
{

}

bool CStreamProc::Initial(unsigned char videoInputNum, unsigned char netVideoInputNum,  unsigned char audioInputNum, \
							CReclogManEX *pReclogMan, CRecordMan *pRecMan, CStreamRecordManEx *pStreamRecManEx)
{
	assert ((0 < videoInputNum) && (videoInputNum <= MAX_CHNN_NUM));
	assert ((0 <= netVideoInputNum) && (netVideoInputNum <= MAX_CHNN_NUM));
	assert ((0 <= audioInputNum) && (audioInputNum <= MAX_CHNN_NUM));

	m_videoInputNum	= videoInputNum;
	m_localVideoInputNum = videoInputNum - netVideoInputNum;
	m_netVideoInputNum = netVideoInputNum;
	m_audioInputNum	= audioInputNum;

	m_pLocalDevice	= CLocalDevice::Instance();
	m_pRecMan		= pRecMan;
	m_pStreamRecManEx = pStreamRecManEx;
#if defined(__DVD_BURN_REC__) && defined (__ENVIRONMENT_LINUX__)
	m_pDvdWriteMan	= CDispatcherMan::Instance();
#endif

	m_pMsgMan		= CMessageMan::Instance();

	m_pFrameList	= new std::list<CFrameData *> [m_localVideoInputNum];
	m_pSecondFrameList = new std::list<CFrameData *> [m_localVideoInputNum];
#ifdef __DVR_BASIC__
	m_pJPEGFrameList = new std::list<CFrameData *> [m_localVideoInputNum];
#endif

	m_pVideoInfoMain	= new VIDEO_INFO [m_localVideoInputNum];
	m_pVideoInfoSub		= new VIDEO_INFO [m_localVideoInputNum];
	memset(m_pVideoInfoMain, 0, sizeof(VIDEO_INFO)*m_localVideoInputNum);
	memset(m_pVideoInfoSub, 0, sizeof(VIDEO_INFO)*m_localVideoInputNum);

	m_pSecondStreamEncodeInfo = new ENCODE_INFO [m_localVideoInputNum];
	memset(m_pSecondStreamEncodeInfo, 0, sizeof(ENCODE_INFO)*m_localVideoInputNum);
	m_pFirstStreamEncodeInfo = new ENCODE_INFO_EX [m_localVideoInputNum];
	memset(m_pFirstStreamEncodeInfo, 0, sizeof(ENCODE_INFO_EX) * m_localVideoInputNum);

#ifdef __DVR_ULTIMATE__
	if (m_netVideoInputNum > 0)
	{
		m_decCardNum = CLocalDevice::Instance()->GetIPCDecChipNum();
		m_pNetDeviceMan	= CNetDeviceManager::Instance();
		m_pBufferFramesList = new std::list<BUFFER_FRAMES> [m_decCardNum];
		m_pMinorBufferFramesList = new std::list<BUFFER_FRAMES>[m_decCardNum];
		m_pBitmapInfo	= new BITMAPINFOHEADER [netVideoInputNum];
		m_pWaveFormat = new WAVEFORMATEX [netVideoInputNum];
		m_pMinorBitmapInfo	= new BITMAPINFOHEADER [netVideoInputNum];
		m_pMinorWaveFormat = new WAVEFORMATEX [netVideoInputNum];
		m_pSecondBitmapInfo = new BITMAPINFOHEADER[netVideoInputNum];
		m_pFrameRate = new long[netVideoInputNum];
		m_pVORate = new long[netVideoInputNum];
		m_pbFrameRateChanged = new bool[netVideoInputNum];
		m_pAudioFrameTime = new LONGLONG[netVideoInputNum];

		memset(m_pBitmapInfo, 0, sizeof(BITMAPINFOHEADER)*netVideoInputNum);
		memset(m_pWaveFormat, 0, sizeof(WAVEFORMATEX)*netVideoInputNum);
		memset(m_pMinorBitmapInfo, 0, sizeof(BITMAPINFOHEADER)*netVideoInputNum);
		memset(m_pMinorWaveFormat, 0, sizeof(WAVEFORMATEX)*netVideoInputNum);
		memset(m_pSecondBitmapInfo, 0, sizeof(BITMAPINFOHEADER)*netVideoInputNum);

		unsigned long videoFormat = CLocalDevice::GetVideoFormatFromFlash();
		for(int i = 0; i < m_netVideoInputNum; i++)
		{
			m_pFrameRate[i] = (VIDEO_FORMAT_PAL == videoFormat) ? 25 : 30;
			m_pAudioFrameTime[i] = 0;
			m_pbFrameRateChanged[i] = true;
			m_pVORate[i] = m_pFrameRate[i];
		}
	}
#endif

	m_pbSendAudioFormat = new bool[audioInputNum];
	for(long audioCh = 0; audioCh < audioInputNum; audioCh++)
	{
		m_pbSendAudioFormat[audioCh] = false;
	}

#ifdef __DVR_BASIC__
	m_bJPEGStream = false;
	m_hJPEGStreamID = PUB_THREAD_ID_NOINIT;
	m_bJPEGStreamAction = false;
#endif
	m_secondLimitRateForFirstStreamNum = INVALID_INDEX(unsigned long);
	m_secondLimitAllRateForFirstStreamNum = INVALID_INDEX(unsigned long); 
	//zxx新加
	m_haveHxht=false;

#ifdef __ALARM_TO_UPDATE_FTP_VIDEO__
	CFtpUpdateMan::Instance()->Initial(1024 * 2048);
#endif
	return true;
}

void CStreamProc::Quit()
{
#ifdef __ALARM_TO_UPDATE_FTP_VIDEO__
	CFtpUpdateMan::Instance()->Quit();
#endif
	g_pStreamProc = NULL;
#ifdef __DVR_ULTIMATE__
	if(NULL != m_pAudioFrameTime)
	{
		delete [] m_pAudioFrameTime;
		m_pAudioFrameTime = NULL;
	}

	if(NULL != m_pFrameRate)
	{
		delete [] m_pFrameRate;
		m_pFrameRate = NULL;
	}

	if(NULL != m_pVORate)
	{
		delete [] m_pVORate;
		m_pVORate = NULL;
	}

	if(NULL != m_pbFrameRateChanged)
	{
		delete [] m_pbFrameRateChanged;
		m_pbFrameRateChanged = NULL;
	}

#endif

	if (NULL != m_pFrameList)
	{
		std::list<CFrameData *>::iterator it;
		for (unsigned char i=0; i<m_localVideoInputNum; ++i)
		{
			while (0 < m_pFrameList[i].size())
			{
				it = m_pFrameList[i].begin();
				m_pLocalDevice->ReleaseBuf((*it)->GetFrameInfo());
				delete (*it);
				m_pFrameList[i].pop_front();
			}
		}
	}

	if (NULL != m_pSecondFrameList)
	{
		std::list<CFrameData *>::iterator it;
		for (unsigned char i=0; i<m_localVideoInputNum; ++i)
		{
			while (0 < m_pSecondFrameList[i].size())
			{
				it = m_pSecondFrameList[i].begin();
				m_pLocalDevice->ReleaseBuf((*it)->GetFrameInfo());
				delete (*it);
				m_pSecondFrameList[i].pop_front();
			}
		}
	}

#ifdef __DVR_BASIC__
	if (NULL != m_pJPEGFrameList)
	{
		std::list<CFrameData *>::iterator it;
		for (unsigned char i=0; i<m_localVideoInputNum; ++i)
		{
			while (0 < m_pJPEGFrameList[i].size())
			{
				it = m_pJPEGFrameList[i].begin();
				m_pLocalDevice->ReleaseJPEGBuf((*it)->GetFrameInfo());
				delete (*it);
				m_pJPEGFrameList[i].pop_front();
			}
		}
	}
#endif

#ifdef __DVR_ULTIMATE__
	if (m_netVideoInputNum > 0)
	{
		if (NULL != m_pBufferFramesList)
		{
			std::list<BUFFER_FRAMES>::iterator it_1;
			std::list<CFrameData *>::iterator it_2;
			for (unsigned char i=0; i<m_decCardNum; ++i)
			{
				while (0 < m_pBufferFramesList[i].size())
				{
					it_1 = m_pBufferFramesList[i].begin();
					while ((*it_1).pFrameList->size() > 0)
					{
						it_2 = (*it_1).pFrameList->begin();
						delete (*it_2);
						
						(*it_1).pFrameList->pop_front();
					}

					m_pNetDeviceMan->ReleaseDataBuffer((*it_1).bufferInfo.chipId, (*it_1).bufferInfo.pData, (*it_1).bufferInfo.dataLen);
					delete (*it_1).pFrameList;
					delete (*it_1).bufferInfo.pStatus;
					m_pBufferFramesList[i].pop_front();
				}
			}

			while (0 < m_secondBufferFramesList.size())
			{
				it_1 = m_secondBufferFramesList.begin();
				while ((*it_1).pFrameList->size() > 0)
				{
					it_2 = (*it_1).pFrameList->begin();
					delete (*it_2);
					(*it_1).pFrameList->pop_front();
				}

				m_pNetDeviceMan->ReleaseDataBuffer((*it_1).bufferInfo.chipId, (*it_1).bufferInfo.pData, (*it_1).bufferInfo.dataLen, 1);
				delete (*it_1).pFrameList;
				m_secondBufferFramesList.pop_front();
			}
		}

		if (NULL != m_pMinorBufferFramesList)
		{
			for (unsigned char i=0; i<m_decCardNum; ++i)
			{
				while (0 < m_pMinorBufferFramesList[i].size())
				{
					BUFFER_FRAMES bufferFrames = m_pMinorBufferFramesList[i].front();
					std::list<CFrameData *>::iterator it;
					while(bufferFrames.pFrameList->size() >0)
					{
						it = bufferFrames.pFrameList->begin();

						delete (*it);

						bufferFrames.pFrameList->pop_front();
					}

					m_pNetDeviceMan->ReleaseDataBuffer(bufferFrames.bufferInfo.chipId, bufferFrames.bufferInfo.pData, bufferFrames.bufferInfo.dataLen, NET_DEVICE_MINOR_STREAM);
					delete bufferFrames.pFrameList;
					delete bufferFrames.bufferInfo.pStatus;
					m_pMinorBufferFramesList[i].pop_front();
				}
			}
		}
	}
#endif

	delete [] m_pbSendAudioFormat;
	delete [] m_pFrameList;
	delete [] m_pSecondFrameList;
#ifdef __DVR_BASIC__
	delete [] m_pJPEGFrameList;
#endif
	delete [] m_pSecondStreamEncodeInfo;
	delete [] m_pFirstStreamEncodeInfo;
	delete [] m_pVideoInfoMain;
	delete [] m_pVideoInfoSub;

	m_pbSendAudioFormat = NULL;
	m_pFrameList		= NULL;
	m_pSecondFrameList	= NULL;
#ifdef __DVR_BASIC__
	m_pJPEGFrameList	= NULL;
#endif
	m_pSecondStreamEncodeInfo	= NULL;
	m_pFirstStreamEncodeInfo	= NULL;
	m_pVideoInfoMain	= NULL;
	m_pVideoInfoSub		= NULL;
#ifdef __DVR_ULTIMATE__
	delete [] m_pBufferFramesList;
	delete [] m_pMinorBufferFramesList;
	delete [] m_pBitmapInfo;
	delete [] m_pWaveFormat;
	delete [] m_pMinorBitmapInfo;
	delete [] m_pMinorWaveFormat;
	delete [] m_pSecondBitmapInfo;
	m_pBufferFramesList = NULL;
	m_pMinorBufferFramesList = NULL;
	m_pBitmapInfo	= NULL;
	m_pWaveFormat	= NULL;
	m_pMinorBitmapInfo	= NULL;
	m_pMinorWaveFormat	= NULL;
	m_pSecondBitmapInfo = NULL;
#endif
	m_secondLimitRateForFirstStreamNum = INVALID_INDEX(unsigned long);
	m_secondLimitAllRateForFirstStreamNum = INVALID_INDEX(unsigned long); 
}

bool CStreamProc::Start()
{
	//先清空底层缓存的数据
	CLocalDevice::Instance()->ClearBuffer();

	if(m_localVideoInputNum > 0)
	{
		//去掉高优先级
		//抓取录像数据的线程优先级在TD2416MD,TD2516FD采用默认值，而在其他产品上采用较高的优先级
		PRODUCT_TYPE produtType = CProduct::Instance()->ProductType();
		if ((TD_2416MD_4A == produtType) || (TD_2416MD    == produtType)
		  ||(TD_2424MD    == produtType) || (TD_2432MD_4A == produtType) 
		  ||(TD_2432MD    == produtType) || (TD_2504FD    == produtType)
		  ||(TD_2508FD_4A == produtType) || (TD_2508FD    == produtType)
		  ||(TD_2516FD_4A == produtType) || (TD_2516FD    == produtType)
		  ||(TD_2504HD    == produtType) || (TD_2508HD    == produtType)
		  ||(TD_2508HD_4A == produtType) || (TD_2516HD    == produtType)
		  ||(TD_2516HD_4A == produtType) || (TD_2504HE    == produtType)
		  ||(TD_2508HE    == produtType) || (TD_2516HE    == produtType)
		  ||(TD_2316SE_S  == produtType) || (TD_2324SE_S  == produtType)
		  ||(TD_2316CDS   == produtType) || (TD_2532HD_C  == produtType)
		  ||(TD_2616LD_S  == produtType))
		{
			printf("%s,%d,PUB_CreateThread(StreamThread, (void *)this, &m_bStream);\n",__FILE__,__LINE__);
			m_stream_ID = PUB_CreateThread(StreamThread, (void *)this, &m_bStream);
			if( PUB_CREATE_THREAD_FAIL == m_stream_ID )
			{
				printf("Initial local stream thread fail!");
				return false;
			}
		}
		else
		{
			printf("%s,%d,PUB_CreateThreadEx(StreamThread, (void *)this, &m_bStream, 50);\n",__FILE__,__LINE__);
			m_stream_ID = PUB_CreateThreadEx(StreamThread, (void *)this, &m_bStream, 50);
			if( PUB_CREATE_THREAD_FAIL == m_stream_ID )
			{
				printf("Initial local stream thread fail!");
				return false;
			}
		}
	}


	if(m_localVideoInputNum > 0)
	{
		m_hSecondStreamID = PUB_CreateThread( SecondStreamThread, (void *)this, &m_bSecondStream);
		if( PUB_CREATE_THREAD_FAIL == m_hSecondStreamID )
		{
			printf("Initial local second stream thread fail!\n");
			return false;
		}
	}


#ifdef __DVR_ULTIMATE__
	if (m_netVideoInputNum > 0)
	{
		m_streamNet_ID = PUB_CreateThread(Net_StreamThread, (void *)this, &m_bStreamNet);
		if( PUB_CREATE_THREAD_FAIL == m_streamNet_ID )
		{
			printf("Initial net stream thread fail!\n");
			return false;
		}

		m_streamNetSecond_ID = PUB_CreateThread(Net_SecondStreamThread, (void *)this, &m_bStreamNetSecond);
		if(PUB_CREATE_THREAD_FAIL == m_streamNetSecond_ID)
		{
			printf("Initial net second stream thread fail!\n");
			return false;
		}

		if(CProduct::Instance()->IPCSupportDisplayMinor())
		{
			m_streamNetMinor_ID = PUB_CreateThread(Net_MinorStreamThread, (void *)this, &m_bStreamNetMinor);
			if(PUB_CREATE_THREAD_FAIL == m_streamNetMinor_ID)
			{
				printf("Initial net minor stream thread fail!\n");
				return false;
			}
		}

		m_hJPEGNetStreamID = PUB_CreateThread(Net_JPEGStreamThread, (void *)this, &m_bJPEGNetStream);
		if(PUB_CREATE_THREAD_FAIL == m_hJPEGNetStreamID)
		{
			printf("Initial net second stream thread fail!\n");
			return false;
		}
	}
#endif

#ifdef __DVR_BASIC__
	m_hJPEGStreamID = PUB_CreateThread( JPEGStreamThread, (void *)this, &m_bJPEGStream);
	if( PUB_CREATE_THREAD_FAIL == m_hJPEGStreamID )
	{
		printf("Initial local JPEG stream thread fail!\n");
		return false;
	}
#endif
	

	m_bStreamAction = true;
	m_bSecondStreamAction = true;
	m_bNetStreamAction = true;
	m_bNetMinorStreamAction = true;
	m_bJPEGNetStreamAction = true;
#ifdef __DVR_BASIC__
	m_bJPEGStreamAction = true;
#endif
	return true;
}

void CStreamProc::Stop()
{
#ifdef __DVR_ULTIMATE__
	if (m_netVideoInputNum > 0)
	{
		if(PUB_THREAD_ID_NOINIT != m_hJPEGNetStreamID)
		{
			PUB_ExitThread(&m_hJPEGNetStreamID, &m_bJPEGNetStream);
		}
		if(PUB_THREAD_ID_NOINIT != m_streamNetMinor_ID)
		{
			PUB_ExitThread(&m_streamNetMinor_ID, &m_bStreamNetMinor);
		}
		if(PUB_THREAD_ID_NOINIT != m_streamNetSecond_ID)
		{
			PUB_ExitThread(&m_streamNetSecond_ID, &m_bStreamNetSecond);
		}
		if(PUB_THREAD_ID_NOINIT != m_streamNet_ID)
		{
			PUB_ExitThread(&m_streamNet_ID, &m_bStreamNet);
		}
	}
#endif

#ifdef __DVR_BASIC__
	if( PUB_THREAD_ID_NOINIT != m_hJPEGStreamID )
	{
		PUB_ExitThread( &m_hJPEGStreamID, &m_bJPEGStream );
	}
#endif

	if( PUB_THREAD_ID_NOINIT != m_hSecondStreamID )
	{
		PUB_ExitThread( &m_hSecondStreamID, &m_bSecondStream );
	}


	if (PUB_THREAD_ID_NOINIT != m_stream_ID)
	{		
		PUB_ExitThread(&m_stream_ID, &m_bStream);
	}

	StopTalk();
}

void CStreamProc::Action(bool bAction)
{
	m_streamLock.Lock();
	m_secondStreamLock.Lock();
	m_netStreamLock.Lock();
	m_netMinorStreamLock.Lock();
#ifdef __DVR_BASIC__
	m_JPEGStreamLock.Lock();
#endif

	if (bAction)
	{
		assert(!m_bStreamAction);
		assert(!m_bSecondStreamAction);
		assert(!m_bNetStreamAction);
		assert(!m_bNetMinorStreamAction);
#ifdef __DVR_BASIC__
		assert(!m_bJPEGStreamAction);
#endif

		m_nmgmTime = GetCurrTime();

#ifdef __ENVIRONMENT_LINUX__
		m_nmrelativeTime = GetTickCount64();
#else
		m_nmrelativeTime = GetTickCount() * 1000;
#endif

		m_nsgmTime = GetCurrTime();

#ifdef __ENVIRONMENT_LINUX__
		m_nsrelativeTime = GetTickCount64();;
#else
		m_nsrelativeTime = GetTickCount() * 1000;
#endif

	}
	else
	{
		assert(m_bStreamAction);
		assert(m_bSecondStreamAction);
		assert(m_bNetStreamAction);
		assert(m_bNetMinorStreamAction);
#ifdef __DVR_BASIC__
		assert(m_bJPEGStreamAction);
#endif
	}

	m_bStreamAction = bAction;
	m_bSecondStreamAction = bAction;
	m_bNetStreamAction = bAction;
	m_bNetMinorStreamAction = bAction;
#ifdef __DVR_BASIC__
	m_bJPEGStreamAction = bAction;
	m_JPEGStreamLock.UnLock();
#endif
	m_netMinorStreamLock.UnLock();
	m_netStreamLock.UnLock();
	m_secondStreamLock.UnLock();
	m_streamLock.UnLock();
}

void CStreamProc::ClearData()
{
	m_streamLock.Lock();
	m_secondStreamLock.Lock();
#ifdef __DVR_BASIC__
	m_JPEGStreamLock.Lock();
#endif

	if (NULL != m_pFrameList)
	{
		std::list<CFrameData *>::iterator it;
		for (unsigned char i = 0; i < m_localVideoInputNum; ++i)
		{
			while (!m_pFrameList[i].empty())
			{
				it = m_pFrameList[i].begin();
				assert (0 == (*it)->GetStatus());
				m_pLocalDevice->ReleaseBuf((*it)->GetFrameInfo());
				delete (*it);
				m_pFrameList[i].pop_front();
			}			
		}
	}

	if (NULL != m_pSecondFrameList)
	{
		std::list<CFrameData *>::iterator it;
		for (unsigned char i = 0; i < m_localVideoInputNum; ++i)
		{
			while (!m_pSecondFrameList[i].empty())
			{
				it = m_pSecondFrameList[i].begin();
				assert (0 == (*it)->GetStatus());
				m_pLocalDevice->ReleaseBuf((*it)->GetFrameInfo());
				delete (*it);
				m_pSecondFrameList[i].pop_front();
			}
		}
	}

#ifdef __DVR_BASIC__
	if (NULL != m_pJPEGFrameList)
	{
		std::list<CFrameData *>::iterator it;
		for (unsigned char i = 0; i < m_localVideoInputNum; ++i)
		{
			while (!m_pJPEGFrameList[i].empty())
			{
				it = m_pJPEGFrameList[i].begin();
				assert (0 == (*it)->GetStatus());
				m_pLocalDevice->ReleaseJPEGBuf((*it)->GetFrameInfo());
				delete (*it);
				m_pJPEGFrameList[i].pop_front();
			}
		}
	}
#endif

	memset(m_pVideoInfoMain, 0, sizeof(VIDEO_INFO)*m_localVideoInputNum);
	memset(m_pVideoInfoSub, 0, sizeof(VIDEO_INFO)*m_localVideoInputNum);

#ifdef __DVR_BASIC__
	m_JPEGStreamLock.UnLock();
#endif
	m_secondStreamLock.UnLock();
	m_streamLock.UnLock();
}

void CStreamProc::SetSecondStreamEncode(unsigned char chnn, const ENCODE_INFO & encode)
{
	assert (NULL != m_pSecondStreamEncodeInfo);
	assert (chnn < m_localVideoInputNum);

	if ((chnn < m_localVideoInputNum) && (NULL != m_pSecondStreamEncodeInfo))
	{
		m_pSecondStreamEncodeInfo[chnn] = encode;

		//由于SetSecondStreamEncodeAttrib设置帧率只关心数字，且原来网络也只有cif
		//下面是简单的改法
		if (TD_2704HD == CProduct::Instance()->ProductType())
		{
			m_pSecondStreamEncodeInfo[chnn].resolution = DVRVIDEO_SIZE_CIF;
		}

		CNetStreamAllotDevice::Instance()->SetChnnEncodeInfo(chnn, encode);
		CLocalSubStreamAllot::Instance()->SetChnnEncodeInfo(chnn, encode);
	}
}

void CStreamProc::SetSecondStreamAuto(bool bAuto)
{
	m_secondStreamLock.Lock();

	if (m_bSecondStreamAuto != bAuto)//改变了才处理
	{
		m_bSecondStreamAuto = bAuto;

		if (m_bSecondStreamAction)//当子码流激动状态才处理
		{
			if (m_bSecondStreamAuto)//要自适应，则根据当前状况做处理
			{
				SetSecondStreamEncodeAttrib();
			}
			else	//恢复到设定状态
			{
				unsigned char maxRate = 0;
				if (VIDEO_FORMAT_PAL == g_video_format)
				{
					maxRate = CProduct::Instance()->NetMaxChannelRatePal();
				}
				else if (VIDEO_FORMAT_NTSC == g_video_format)
				{
					maxRate = CProduct::Instance()->NetMaxChannelRateNtsc();
				}

				unsigned char rate = 0;
				for (int i=0; i<m_localVideoInputNum; i++)
				{
	
					if (UsedRate(m_pSecondStreamEncodeInfo[i].rate, (VIDEO_SIZE)(m_pSecondStreamEncodeInfo[i].resolution)) > maxRate)
					{
						rate = RealRate(maxRate, (VIDEO_SIZE)(m_pSecondStreamEncodeInfo[i].resolution));
					}
					else
					{
						rate = m_pSecondStreamEncodeInfo[i].rate;
					}

					//m_pLocalDevice->SetFrameRate(i, rate, false);
					m_pLocalDevice->SetFrameRateQuality(i, rate, static_cast<VIDEO_ENCODE_MODE>(m_pSecondStreamEncodeInfo[i].encodeType), m_pSecondStreamEncodeInfo[i].hBitStream, m_pSecondStreamEncodeInfo[i].quality,false);
				}

			}
		}
	}

	m_secondStreamLock.UnLock();
}

void CStreamProc::ChangeSecondStreamCH(ULONGLONG CH, ULONGLONG CH_mobile, unsigned long secondAllSendNum, ULONGLONG mobileHigh)
{
	m_secondStreamLock.Lock();

	m_secondStreamCH_Mobile = CH_mobile;
	m_secondStreamCH = CH;
	m_secondSendAllNum = secondAllSendNum;

	if (m_bSecondStreamAction)//子码流激活状态才处理
	{
		bool bLimitRate = false;
		if ((m_secondLimitRateForFirstStreamNum != INVALID_INDEX(unsigned long)) || (m_secondRateLimit != INVALID_INDEX(unsigned long)))
		{
			bLimitRate = true;
		}

		if (!CNetStreamAllotDevice::Instance()->AllotNetStream(m_secondStreamCH, m_secondStreamCH_Mobile, mobileHigh, m_secondSendAllNum, bLimitRate))
		{
			if (!CLocalSubStreamAllot::Instance()->AllotLocalSubStream(m_secondStreamCH, m_secondStreamCH_Mobile, mobileHigh, m_secondSendAllNum, bLimitRate))
			{
				m_secondStreamCH |= mobileHigh;
				SetSecondStreamEncodeAttrib();
			}
		}
	}

	m_secondStreamLock.UnLock();
}

void CStreamProc::SetSecondSteamRateLimit(unsigned long limitRate)
{
	m_secondStreamLock.Lock();
	m_secondRateLimit = limitRate;
	printf("%s,%d,m_secondRateLimit:%d\n",__FILE__,__LINE__,m_secondRateLimit);

	m_secondStreamLock.UnLock();
}

void CStreamProc::SecondStreamMaxRateForFirstStreamNum(unsigned long limitRateForFirstNum, unsigned long limitAllRateForFirstNum)
{
	m_secondStreamLock.Lock();
	m_secondLimitRateForFirstStreamNum = limitRateForFirstNum;
	m_secondLimitAllRateForFirstStreamNum = limitAllRateForFirstNum;
	printf("%s,%d,m_secondLimitRateForFirstStreamNum:%d,m_secondLimitAllRateForFirstStreamNum:%d\n",__FILE__,__LINE__,m_secondLimitRateForFirstStreamNum,m_secondLimitAllRateForFirstStreamNum);

	m_secondStreamLock.UnLock();
}

void CStreamProc::SetFirstStreamEncode(unsigned char chnn, const ENCODE_INFO_EX & encode)
{
	assert (NULL != m_pFirstStreamEncodeInfo);
	assert (chnn < m_localVideoInputNum);

	if ((chnn < m_localVideoInputNum) && (NULL != m_pSecondStreamEncodeInfo))
	{
		m_pFirstStreamEncodeInfo[chnn] = encode;
	}
}

void CStreamProc::SetFirstStreamLimit(unsigned long limitFrameRate, unsigned long limitBitRate)
{
	unsigned long maxRate = limitFrameRate;
	if (VIDEO_FORMAT_PAL == g_video_format)
	{
		maxRate = (maxRate < 25) ? maxRate : 25;
	}
	else/* if (VIDEO_FORMAT_NTSC == g_video_format)*/
	{
		maxRate = (maxRate < 30) ? maxRate : 30;
	}

	unsigned long frameRate = maxRate;
	unsigned long bitRate = limitBitRate;

	for (unsigned char i=0; i<m_localVideoInputNum; ++i)
	{
		frameRate = m_pFirstStreamEncodeInfo[i].rate;//UsedRate(m_pFirstStreamEncodeInfo[i].rate, static_cast<VIDEO_SIZE>(m_pFirstStreamEncodeInfo[i].resolution));
		frameRate = (frameRate < maxRate) ? frameRate : maxRate;

		bitRate = m_pFirstStreamEncodeInfo[i].hBitStream;

		bitRate = (bitRate < limitBitRate) ? bitRate : limitBitRate;

		m_pLocalDevice->SetFrameRateQuality(i, frameRate, \
			static_cast<VIDEO_ENCODE_MODE>(m_pFirstStreamEncodeInfo[i].encodeType), \
			m_pFirstStreamEncodeInfo[i].hBitStream, m_pFirstStreamEncodeInfo[i].quality);
	}
}

bool CStreamProc::StartTalk()
{
	if(m_bTalkStream)	//对讲独占
	{
		return false;
	}


	if(!m_pLocalDevice->TalkWayStart())
	{
		return false;
	}

	m_bWouldStopTalk = false;
	m_streamTalkID = PUB_CreateThread(TalkStreamThread, (void *)this, &m_bTalkStream);
	if(PUB_CREATE_THREAD_FAIL == m_streamTalkID)
	{
		printf("Initial talk stream thread fail!\n");
		return false;
	}

	return true;
}

void CStreamProc::WouldStopTalk()
{
	m_talkStreamLock.Lock();
	m_bWouldStopTalk = true;
	m_talkStreamLock.UnLock();
}

void CStreamProc::StopTalk()
{
	if(m_bTalkStream)
	{
		if(PUB_THREAD_ID_NOINIT != m_streamTalkID)
		{
			PUB_ExitThread(&m_streamTalkID, &m_bTalkStream);
		}

		//清除没有被网络传出的对讲数据
		ReleaseTalkFrame();
		if(!m_talkFrameList.empty())
		{
			assert(false);
		}

		m_pLocalDevice->TalkWayStop();
		m_pLocalDevice->RecoverAudioLiveChannel();
	}
}

void CStreamProc::SetNetVideoFRate(unsigned char chnn, long frameRate, unsigned long bitRate)
{
#ifdef __DVR_ULTIMATE__
	m_pFrameRate[chnn] = frameRate;
	m_pbFrameRateChanged[chnn] = true;

	unsigned long maxBitRate = CProduct::Instance()->GetDigitMaxTotalBitRate();
	unsigned long preRecordMaxLen = ((PRE_RECORD_MAX_LEN >> 10) * bitRate / (maxBitRate / m_netVideoInputNum)) << 10;

	m_pStreamRecManEx->SetPreRecDataLen(chnn + m_localVideoInputNum, preRecordMaxLen);
#endif
}

/************************************************************************/
/*                                protected                             */
/************************************************************************/


/************************************************************************/
/*                                private                               */
/************************************************************************/
RESULT WINAPI CStreamProc::StreamThread(LPVOID lpParameter)
{
#ifdef __ENVIRONMENT_LINUX__
	pid_t pid = getpid();
	pid_t tid = syscall(__NR_gettid);
	printf("%s, %d, pid = %d, tid=%d\n", __FILE__, __LINE__, pid, tid);
#endif

	assert(NULL != lpParameter);

	CStreamProc *pThis = reinterpret_cast<CStreamProc *>(lpParameter);

	pThis->StreamProc();

	return 0;
}

void CStreamProc::StreamProc()
{
	bool	bKeyFrame = false;
	FRAME_INFO_EX frameInfo;
	CFrameData *pFrameData = NULL;
	unsigned char i=0, succCount = 0, group = 4;//(4 < m_localVideoInputNum) ? (m_localVideoInputNum - 4) : 4;
	unsigned char writeSuccCount = 0, recGroup = (group < m_localVideoInputNum) ? (m_localVideoInputNum - group) : group;
	RECORD_WRITE_STATUS ret = RECORD_WRITE_FAIL;
	int	getRet = CLocalDevice::GET_FRAME_FAIL;

#ifdef __ALARM_TO_UPDATE_FTP_VIDEO__
	CFtpUpdateMan * pFtpUpdateMan = CFtpUpdateMan::Instance();
#endif

#ifdef __NETSERVER_HXHT__
	bool bSupportDoubleSteam = true;
#else
	bool bSupportDoubleSteam = CProduct::Instance()->SupportDoubleStream();
#endif

	unsigned long sleepCount = 11;
	PRODUCT_TYPE productType = CProduct::Instance()->ProductType();
	if (TD_2704HD == productType)
	{
		group = 4;
	}

	if (TD_2532HD_C == productType)
	{
		group = 4;
		sleepCount = 2;
	}

#if defined(__CHIPGM__)
	if (TD_2704AS_SL == productType)
	{
		group = 4;
		sleepCount = 5;
	}
#endif

	bool bPreMemRecord = false;
	if (PRERECORD_MODE_MEMORY == CProduct::Instance()->GetPreRecordType())
	{
		bPreMemRecord = true;
	}
	else
	{
		bPreMemRecord = false;
	}

	/*if (g_curNet3G)
	{
		bSupportDoubleSteam = false;
	}*/
	while (m_bStream)
	{
		if (g_curNet3G)
		{
			bSupportDoubleSteam = false;
		}
		else
		{
			bSupportDoubleSteam = CProduct::Instance()->SupportDoubleStream();
		}
		
		m_streamLock.Lock();
		if (!m_bStreamAction)
		{
			m_streamLock.UnLock();
			PUB_Sleep(10);
		}
		else
		{
			succCount = 0;
			writeSuccCount = 0;

			for (i=0; i<m_localVideoInputNum; ++i)
			{	
				memset(&frameInfo, 0, sizeof(FRAME_INFO_EX));
				getRet = m_pLocalDevice->GetVideo_F(i, frameInfo);

				if (CLocalDevice::GET_FRAME_SUCC == getRet)
				{
					++ succCount;

					//printf("frameInfo.frameType:%lu  frameInfo.width:%lu  frameInfo.height:%lu  m_pVideoInfoMain[%d].width:%lu  m_pVideoInfoMain[%d].height:%lu  %s %d\n", frameInfo.frameType, frameInfo.width, frameInfo.height, i, m_pVideoInfoMain[i].width, i, m_pVideoInfoMain[i].height, __FILE__, __LINE__);
					//如果图像尺寸发生变化，则需要重新送入格式
					if ((FRAME_TYPE_VIDEO == frameInfo.frameType) && \
						((m_pVideoInfoMain[i].width != frameInfo.width) || (m_pVideoInfoMain[i].height != frameInfo.height)))
					{
						m_pVideoInfoMain[i].width = frameInfo.width;
						m_pVideoInfoMain[i].height = frameInfo.height;

						SendAVFormat(i);
					}

					frameInfo.frameAttrib = LIVE_FRAME_FIRST_STREAM;

					pFrameData = new CFrameData (frameInfo, 1, CMD_REPLY_DATA_STREAM, INVALID_INDEX(unsigned long));
					//pFrameData->SetProtocol(CMD_REPLY_DATA_STREAM);
					//assert(NULL != pFrameData);
					//pFrameData->SetDataId(INVALID_INDEX(unsigned long));
					pFrameData->Add();

					//送入录像模块
					ret = m_pRecMan->DateProc(*pFrameData, bKeyFrame);
					writeSuccCount += (RECORD_WRITE_SUCC == ret) ? 1 : 0;

					//送入的是视频帧，绝对不能返回需要音频格式
					assert(ret != RECORD_WRITE_NO_WAVEFORMAT); 

#if defined(__DVD_BURN_REC__) && defined (__ENVIRONMENT_LINUX__)
					//送刻录模块
					pFrameData->Add();
					m_pDvdWriteMan->WriteData(*pFrameData);
#endif

#ifdef __ALARM_TO_UPDATE_FTP_VIDEO__
					//送入FTP模块
					pFtpUpdateMan->PutFrame(pFrameData);
#endif

					//发送视频格式
					if(ret == RECORD_WRITE_NO_BITMAPINFO)
					{
						SendAVFormat(i);
					}
					
					//加入网络队列
					if (bSupportDoubleSteam)
					{
						m_pMsgMan->PutFrame(pFrameData);
					}					

					if (bKeyFrame)	//要求关键帧
					{
						m_pLocalDevice->KeyFrame(i);
						bKeyFrame = false;
					}

					if (0 < pFrameData->Dec())
					{
						m_pFrameList[i].push_back(pFrameData);
					}
					else
					{
						m_pLocalDevice->ReleaseBuf(pFrameData->GetFrameInfo());
						delete pFrameData;
						pFrameData = NULL;
					}
				}    
				else if (CLocalDevice::FRAME_BUF_FULL == getRet)
				{
					//录像缓存满了，需要清空所有的数据
					if (bPreMemRecord)
					{
						m_pRecMan->RemoveAllDate();
					}		

#if defined(__DVD_BURN_REC__) && defined (__ENVIRONMENT_LINUX__)
					m_pDvdWriteMan->RemoveAllDate();
#endif

					m_pMsgMan->ClearFrame(LIVE_FRAME_FIRST_STREAM);

					//清除网络队列
#ifdef __NETSERVER_TVT__
					CNetServer::Instance()->ClearData(LIVE_FRAME_FIRST_STREAM);
#endif

#ifdef __NETSERVER_HXHT__
					CHxConnectMan::Instance()->ClearAllFrameData();	
#endif

#ifdef __ALARM_TO_UPDATE_FTP_VIDEO__
					pFtpUpdateMan->ClearStream();
#endif
				}

				ReleaseFrameInfo(i);

				//抓取音频数据
				memset( &frameInfo, 0, sizeof(FRAME_INFO_EX) );
				if((i < m_audioInputNum) && (CLocalDevice::GET_FRAME_SUCC == m_pLocalDevice->GetAudio( i, frameInfo )) )
				{
					if ((FRAME_TYPE_AUDIO == frameInfo.frameType) && !m_pbSendAudioFormat[i])
					{
						m_pbSendAudioFormat[i] = true;
						SendAVFormat(i, true);
					}

					frameInfo.frameAttrib |= LIVE_FRAME_FIRST_STREAM;
					pFrameData = new CFrameData( frameInfo, 1, CMD_REPLY_DATA_STREAM, INVALID_INDEX(unsigned long));
					assert( NULL != pFrameData );
					//pFrameData->SetProtocol( CMD_REPLY_DATA_STREAM );
					//pFrameData->SetDataId( INVALID_INDEX(unsigned long) );
					pFrameData->Add();

					//送到录像模块
					ret = m_pRecMan->DateProc( *pFrameData, bKeyFrame );

					//送入的是音频帧，绝对不能返回需要视频格式
					assert(ret != RECORD_WRITE_NO_BITMAPINFO);

#if defined(__DVD_BURN_REC__) && defined (__ENVIRONMENT_LINUX__)
					//音频送入刻录
					pFrameData->Add();
					m_pDvdWriteMan->WriteData(*pFrameData);
#endif

					//发送音频格式
					if (ret == RECORD_WRITE_NO_WAVEFORMAT)
					{
						SendAVFormat(i, true);
					}

					m_pMsgMan->PutFrame( pFrameData );

					//printf("ch%d, %d\n", i, pFrameData->GetStatus());
					if( 0 < pFrameData->Dec() )
					{
						m_pFrameList[i].push_back( pFrameData );
					}
					else
					{
						m_pLocalDevice->ReleaseBuf( pFrameData->GetFrameInfo() );
						delete pFrameData;
						pFrameData = NULL;
					}

				}

				ReleaseFrameInfo(i);
			}

			m_streamLock.UnLock();

			//只要有一个通道抓取不成功，或者不足recGroup个通道能录像（在录像且有文件可写）
			//则进行sleep让出CPU，因为本线程采用了实时调度的线程。
#ifndef __CHIP3515__
#ifdef __TD32SPEC__
			if ((succCount < group))
			{
				PUB_Sleep(sleepCount);
			}
#else
			if ((succCount < group) || ((writeSuccCount < succCount) && (g_preRecErrSleep)))
			{
				PUB_Sleep(sleepCount);    
			}
#endif
#else
#ifdef __TD2316SES_SPEC__
			if ((succCount < group) || ((writeSuccCount < succCount) && (g_preRecErrSleep)))
			{
				PUB_Sleep(sleepCount);
			}
#else
			PUB_Sleep(sleepCount);
#endif
#endif
		}
	}
}


RESULT WINAPI CStreamProc::SecondStreamThread(LPVOID lpParameter)
{
#ifdef __ENVIRONMENT_LINUX__
	pid_t pid = getpid();
	pid_t tid = syscall(__NR_gettid);
	printf("%s, %d, pid = %d, tid=%d\n", __FILE__, __LINE__, pid, tid);
#endif

	assert( NULL != lpParameter );
	CStreamProc *pThis = reinterpret_cast<CStreamProc*>(lpParameter);
	pThis->SecondStreamProc();

	return 0;
}


void CStreamProc::SecondStreamProc()
{

	FRAME_INFO_EX frameInfo;
	CFrameData *pFrameData = NULL;
	char chnn, succCount, group = (4 < m_localVideoInputNum) ? 4 : m_localVideoInputNum;
	int	getRet = CLocalDevice::GET_FRAME_FAIL;

	//只有2516FD才CleanBuff,
	PRODUCT_TYPE productType = CProduct::Instance()->ProductType();
    bool bCleanBuff = false;
	if ((TD_2516FD_4A == productType) || (TD_2516FD == productType))
	{
		bCleanBuff = true;
	}

	while( m_bSecondStream )
	{
		m_secondStreamLock.Lock();
		if (!m_bSecondStreamAction)
		{
			m_secondStreamLock.UnLock();
			PUB_Sleep(10);
		}
		else 
		{
			succCount = 0;

			for( chnn=0; chnn<m_localVideoInputNum; ++chnn )
			{
				memset(&frameInfo, 0, sizeof(frameInfo));
				getRet = m_pLocalDevice->GetVideo_S( chnn, frameInfo );

				if( CLocalDevice::GET_FRAME_SUCC == getRet)
				{

					assert (FRAME_TYPE_VIDEO == frameInfo.frameType);

					++ succCount;

					frameInfo.frameAttrib = LIVE_FRAME_SECOND_STREAM;
					pFrameData = new CFrameData(frameInfo, 1, CMD_REPLY_DATA_STREAM, INVALID_INDEX(unsigned long));
					assert( NULL != pFrameData );
					//pFrameData->SetProtocol( CMD_REPLY_DATA_STREAM );
					//pFrameData->SetDataId( INVALID_INDEX(unsigned long) );
					pFrameData->Add();

					//如果图像尺寸发生变化，则需要重新送入格式
					if ((m_pVideoInfoSub[chnn].width != frameInfo.width) || (m_pVideoInfoSub[chnn].height != frameInfo.height))
					{
						FRAME_INFO_EX frameFormatInfo;
						memset( &frameFormatInfo, 0, sizeof( FRAME_INFO_EX ) );

						frameFormatInfo.channel = chnn;
						frameFormatInfo.frameAttrib = LIVE_FRAME_SECOND_STREAM;
						frameFormatInfo.frameType = FRAME_TYPE_VIDEO_FORMAT;
						frameFormatInfo.length = sizeof(BITMAPINFOHEADER);
						frameFormatInfo.pData = new unsigned char[sizeof(BITMAPINFOHEADER)];
						m_pLocalDevice->GetVideoFormat( chnn, *reinterpret_cast<BITMAPINFOHEADER*>(frameFormatInfo.pData), false);
						
						BITMAPINFOHEADER * pVF = (BITMAPINFOHEADER *)frameFormatInfo.pData;
						if (frameInfo.width != pVF->biWidth || frameInfo.height != pVF->biHeight)
						{
							//高清子码流设备，如果频繁切换分辨率，比如从720->1080->CIF，会出现拿到的帧宽高为1080P的，但是从SDK获取到的宽高为CIF的，这会导致发给客户端的格式帧为CIF的
							//但是实际的分辨率为1080，从而导致客户端解码错误
							pVF->biWidth	= frameInfo.width;
							pVF->biHeight	= frameInfo.height;
						}
						CFrameData *pFrameFromatData = new CFrameData(frameFormatInfo, 0, CMD_REPLY_DATA_STREAM, INVALID_INDEX(unsigned long));
						//pFrameFromatData->SetProtocol( CMD_REPLY_DATA_STREAM);
						//pFrameFromatData->SetDataId( INVALID_INDEX(unsigned long) );
						m_pMsgMan->PutFrame( pFrameFromatData );

						m_pVideoInfoSub[chnn].width = frameInfo.width;
						m_pVideoInfoSub[chnn].height = frameInfo.height;
					}


					//加入网络队列
					m_pMsgMan->PutFrame( pFrameData );

					if( 0 < pFrameData->Dec())
					{
						m_pSecondFrameList[chnn].push_back(pFrameData);
					}
					else
					{
						m_pLocalDevice->ReleaseBuf(pFrameData->GetFrameInfo());
						delete pFrameData;
						pFrameData = NULL;
					}

				}
				if (bCleanBuff)
				{
					unsigned long frameNum = m_pMsgMan->GetFrameNum(LIVE_FRAME_SECOND_STREAM);
					if (frameNum > 2000)
					{
						printf("##############%s,%d,frameNum:%d > 2000\n",__FILE__,__LINE__,frameNum);

						//录像缓存满了，需要清空所有的数据
						//m_pRecMan->RemoveAllDate();
						m_pMsgMan->ClearFrame(LIVE_FRAME_SECOND_STREAM);

						//清除网络队列
#ifdef __NETSERVER_TVT__
						CNetServer::Instance()->ClearData(LIVE_FRAME_SECOND_STREAM);
#endif

					}

				}

				

				//获得不到码流、每次获得码流后都要释放码流空间
				ReleaseSecondFrameInfo( chnn );

			}

			m_secondStreamLock.UnLock();
#if defined(__TD2316SES_SPEC__) || defined(__TDFH__) 
			PUB_Sleep(15);
#endif
#if defined(__CHIPGM__)
			if (TD_2704AS_SL == productType)
			{
				PUB_Sleep(5);
			}
#endif
			if (succCount < group)
			{
				PUB_Sleep(1);
			}
		}
	}
}



RESULT WINAPI CStreamProc::Net_StreamThread(LPVOID lpParameter)
{
#ifdef __ENVIRONMENT_LINUX__
	pid_t pid = getpid();
	pid_t tid = syscall(__NR_gettid);
	printf("%s, %d, pid = %d, tid=%d\n", __FILE__, __LINE__, pid, tid);

#if defined (__CHIP3531__)
	BindThreadToCpu(tid, 1);
#endif

#endif

	assert(NULL != lpParameter);

	CStreamProc *pThis = reinterpret_cast<CStreamProc *>(lpParameter);

	pThis->Net_StreamProc();

	return 0;
}

void CStreamProc::Net_StreamProc()
{
#ifdef __DVR_ULTIMATE__
	unsigned long prevPrintTick = 0;
	bool	bKeyFrame = false;
	FRAME_INFO_EX frameInfo, playFrameInfo;
	memset(&frameInfo, 0, sizeof(FRAME_INFO_EX));
	DVR_IP_IN_FRM_INFO *pIPFrameInfo; 
	CFrameData *pFrameData = NULL;
	unsigned char iDecCard = 0, channel = 0;
	RECORD_WRITE_STATUS ret = RECORD_WRITE_FAIL;
	BUFFER_FRAMES bufferFrames;

	CHIP_DECODE_PARAM *pChipDecodeParam = new CHIP_DECODE_PARAM[m_decCardNum];
	memset(pChipDecodeParam, 0, sizeof(CHIP_DECODE_PARAM) * m_decCardNum);

	memset(&playFrameInfo, 0, sizeof(FRAME_INFO_EX));
	playFrameInfo.frameType = FRAME_TYPE_VIDEO;

	LONGLONG **ppFrameTime = new LONGLONG*[m_netVideoInputNum];
	for(unsigned char i = 0; i < m_netVideoInputNum; i++)
	{
		ppFrameTime[i] = new LONGLONG[8];
		memset(ppFrameTime[i], 0, sizeof(LONGLONG) * 8);
	}
	long *pFrameNum = new long[m_netVideoInputNum];
	memset(pFrameNum, 0, m_netVideoInputNum * sizeof(long));

	ULONGLONG ullDisconnectCH = 0;

	long bufferNum = 0;
	bool bGetDataAgain = false;
	bool bHasMuchDataBuffer = false;

	long maxFrameNum = (VIDEO_FORMAT_PAL == g_video_format) ? 25 : 30;

#ifdef __ENVIRONMENT_WIN32__
	m_pLocalDevice->PlaybackStart(0x0f);
#endif
	long readCursor = 0;

	m_nmgmTime = GetCurrTime();

#ifdef __ENVIRONMENT_LINUX__
	m_nmrelativeTime = GetTickCount64();
#else
	m_nmrelativeTime = GetTickCount() * 1000;
#endif

	CIPCDecDisplayMan * pIPCDecDisplayMan = CIPCDecDisplayMan::Instance();

	while (m_bStreamNet)
	{
		m_netStreamLock.Lock();
		if(!m_bNetStreamAction)
		{
			m_netStreamLock.UnLock();
			PUB_Sleep(11);
			continue;
		}

		for(iDecCard = 0; iDecCard < m_decCardNum; iDecCard++)
		{
			//整理队列。
			ReleaseFrameInfoNet(iDecCard);

			if(m_pNetDeviceMan->IsBufferFull(iDecCard))
			{
 				unsigned long curTick = GetTickCount();
// 				if((curTick - prevPrintTick) > 1000)
				{
					if(!m_pBufferFramesList[iDecCard].empty())
					{
						if(m_pBufferFramesList[iDecCard].front().pFrameList != NULL)
						{
							printf("......Buffer is full! Remove all pre-record data\n");
							
							m_pRecMan->RemoveAllDate();
							
							ReleaseFrameInfoNet(iDecCard);
							
							//清除预录像后如果还满就尝试清理网络发送队列
							if(m_pNetDeviceMan->IsBufferFull(iDecCard))
							{
								unsigned long averageRate = m_pNetDeviceMan->GetAverageRate();
								printf("......Clear netserver message! AverageRate:%d \n",averageRate);
								
								m_pMsgMan->ClearFrame(LIVE_FRAME_FIRST_STREAM);
								
#if defined(__DVD_BURN_REC__) && defined (__ENVIRONMENT_LINUX__)
								m_pDvdWriteMan->RemoveAllDate();
#endif
								//清除网络队列
#ifdef __NETSERVER_TVT__
								CNetServer::Instance()->ClearData(LIVE_FRAME_FIRST_STREAM);
#endif

#ifdef __NETSERVER_HXHT__
								CHxConnectMan::Instance()->ClearAllFrameData();	
#endif

#ifdef __ALARM_TO_UPDATE_FTP_VIDEO__
								pFtpUpdateMan->ClearStream();
#endif
								ReleaseFrameInfoNet(iDecCard);
								if(m_pNetDeviceMan->IsBufferFull(iDecCard))
								{
									printf("Release frame buffer error!\n");
								}
							}
						}
						else
						{
							printf("......Buffer is full! Because of not release by decode!\n");
						}
					}
					else
					{
						printf(".......Buffer is full! Because of get to slow!\n");
						m_pNetDeviceMan->PrintBufferInfo(iDecCard);
						
						if(prevPrintTick==0)
						{
							prevPrintTick = curTick;
						}
						else if((curTick-prevPrintTick) > 2000)
						{
							m_pNetDeviceMan->ClearDataBuffer(NET_DEVICE_MAIN_STREAM);
							prevPrintTick = curTick;
						}
					}
				}
			}

			pChipDecodeParam[iDecCard].busyCount = 0;
			list<BUFFER_FRAMES>::iterator it = m_pBufferFramesList[iDecCard].begin();
			for(; it != m_pBufferFramesList[iDecCard].end(); it++)
			{
				if(IP_BUFFER_FREE != *(*it).bufferInfo.pStatus)
				{
					pChipDecodeParam[iDecCard].busyCount++;
				}
			}
		}//end对所有芯片检查释放资源

		ChangeRecStatusForNetStatus(ullDisconnectCH);

		bGetDataAgain = false;
		do
		{
			bHasMuchDataBuffer = false;
			for(iDecCard = 0; iDecCard < m_decCardNum; iDecCard++)
			{
				bufferNum = m_pNetDeviceMan->GetBufferedNum(iDecCard);
				if(bufferNum == 0)
				{
					continue;
				}
				else if(bufferNum <= 1)
				{
					if(bGetDataAgain || (pChipDecodeParam[iDecCard].busyCount >= 8))
					{
						continue;
					}
				}
				else if(bufferNum > 2)
				{
					bHasMuchDataBuffer = true;
				}

				if(!m_pNetDeviceMan->GetDataBuffer(iDecCard, bufferFrames.bufferInfo.pData, bufferFrames.bufferInfo.dataLen))
				{
					//printf("NetStream get data failed!\n");
					continue;
				}

				bufferFrames.pFrameList = new std::list<CFrameData *>();

				for(readCursor = 0; readCursor < bufferFrames.bufferInfo.dataLen;)
				{
					assert((bufferFrames.bufferInfo.dataLen - readCursor) >= sizeof(DVR_IP_IN_FRM_INFO));
					if((bufferFrames.bufferInfo.dataLen - readCursor) < sizeof(DVR_IP_IN_FRM_INFO))
					{
						break;
					}

					pIPFrameInfo = (DVR_IP_IN_FRM_INFO *)(bufferFrames.bufferInfo.pData+readCursor);
					frameInfo.channel = pIPFrameInfo->channel;
					frameInfo.frameType = pIPFrameInfo->data_type;
					frameInfo.keyFrame = pIPFrameInfo->keyframe;
					frameInfo.length = pIPFrameInfo->len;
					frameInfo.time = pIPFrameInfo->cur_time;
					if (0 == m_localVideoInputNum)
					{
						frameInfo.relativeTime = pIPFrameInfo->relative_time;
					}
					else
					{
						frameInfo.relativeTime = GetNetRelativeTime(m_nmgmTime, m_nmrelativeTime, pIPFrameInfo->relative_time);
					}
					frameInfo.width = pIPFrameInfo->width;
					frameInfo.height = pIPFrameInfo->height;
					frameInfo.frameAttrib = LIVE_FRAME_FIRST_STREAM;

					pIPFrameInfo->channel -= m_localVideoInputNum;
					channel = CHANNEL_ID(pIPFrameInfo->channel);

					if(frameInfo.frameType == FRAME_TYPE_VIDEO)
					{
						if(frameInfo.keyFrame)
						//if(pFrameNum[channel] > maxFrameNum*2)
						{
							ULONGLONG displayCH, displayCHMinor;
							pIPCDecDisplayMan->GetDisplayCH(displayCH, displayCHMinor);
							if(displayCH & ((ULONGLONG)0x01 << channel))
							{
								long leftFrameNum = CLocalDevice::Instance()->GetDecLeftFrame(channel);

								if(m_pbFrameRateChanged[channel])
								{
									m_pbFrameRateChanged[channel] = false;
									m_pVORate[channel] = m_pFrameRate[channel];
									printf("...............change vo[%d] = %d\n", channel, m_pVORate[channel]);
								}
								else //if(pFrameNum[channel] > 1)
								{
									LONGLONG timeSpan = ppFrameTime[channel][1] - ppFrameTime[channel][0];

									if(timeSpan > 0 && ppFrameTime[channel][0] > 0 )
									{
										int frameRate = (int)(((pFrameNum[channel]-1) * 1000000 + timeSpan/2) / timeSpan);
										int prev_frmate = (int)m_pVORate[channel];
										int sys_frame = (int)maxFrameNum;

										if( sys_frame < m_pFrameRate[channel] )
										{
											sys_frame = m_pFrameRate[channel];
										}

										if( ppFrameTime[channel][4] != frameRate )
										{
											ppFrameTime[channel][4] = frameRate;

											if( ppFrameTime[channel][5] > 0 )
											{
												if( frameRate < prev_frmate )
												{
													frameRate = prev_frmate;
												}
												else if( frameRate > sys_frame )
												{
													frameRate = sys_frame;
												}
											}
											else
											{
												frameRate = sys_frame;
											}

											ppFrameTime[channel][5] = 0;

										}
										else if( ppFrameTime[channel][5] < 9 )
										{
											frameRate = sys_frame;
										}

										ppFrameTime[channel][5]++;

										if( leftFrameNum >= sys_frame/2 )
										{
											if( frameRate <= prev_frmate )
											{
												frameRate = prev_frmate + 1;
											}
										}

										if(m_pVORate[channel] != frameRate)	//恢复到正常速度
										{
											m_pVORate[channel] = frameRate;
											//printf("current ------------------2----------------------------channel %d,%d,  %d,%d,%d\n", 
											//	channel, frameRate, (int)ppFrameTime[channel][4], leftFrameNum, prev_frmate);
										}
									}
								}
							}

							pFrameNum[channel] = 1;
							ppFrameTime[channel][0] = frameInfo.time;
						}
						else
						{
							ppFrameTime[channel][1] = frameInfo.time;
							pFrameNum[channel]++;
						}
					}

					pIPFrameInfo->enc_stream_type = m_pVORate[channel];

					readCursor += sizeof(DVR_IP_IN_FRM_INFO);
					frameInfo.pData = bufferFrames.bufferInfo.pData+readCursor;
					readCursor += pIPFrameInfo->vir_len;

					if(readCursor > bufferFrames.bufferInfo.dataLen)
					{
						printf("readCursor = %d, dataLen = %d\n", readCursor, bufferFrames.bufferInfo.dataLen);
						assert(false);
					}

					if (FRAME_TYPE_VIDEO_FORMAT == frameInfo.frameType)
					{
						m_pBitmapInfo[channel] = *reinterpret_cast<BITMAPINFOHEADER *>(frameInfo.pData);
					}
					else if (FRAME_TYPE_AUDIO_FORMAT == frameInfo.frameType)
					{
						m_pWaveFormat[channel] = *reinterpret_cast<WAVEFORMATEX *>(frameInfo.pData);
					}

#ifdef __ENVIRONMENT_WIN32__
					ULONGLONG displayCH, displayCHMinor;
					pIPCDecDisplayMan->GetDisplayCH(displayCH, displayCHMinor);
					if((FRAME_TYPE_VIDEO_FORMAT == frameInfo.frameType) || ((0x01 << frameInfo.channel) & displayCH))
					{
						FRAME_INFO_EX playFrameInfo = frameInfo;
						playFrameInfo.channel -= m_localVideoInputNum;
						while(!m_pLocalDevice->PlaybackGetBuf(&playFrameInfo))
						{
							PUB_Sleep(11);
						}
						memcpy(playFrameInfo.pData, frameInfo.pData, frameInfo.length);
						playFrameInfo.length = frameInfo.length;
						m_pLocalDevice->PlaybackSetBuf(&playFrameInfo);
					}
#endif

					pFrameData = new CFrameData (frameInfo, 1, CMD_REPLY_DATA_STREAM, INVALID_INDEX(unsigned long));
					assert(NULL != pFrameData);

					pFrameData->Add();

					m_pMsgMan->PutFrame(pFrameData);

					//送入录像模块
					ret = m_pRecMan->DateProc(*pFrameData, bKeyFrame);

					if (RECORD_WRITE_NO_BITMAPINFO == ret)
					{
						frameInfo.frameType = FRAME_TYPE_VIDEO_FORMAT;
						frameInfo.length	= sizeof(BITMAPINFOHEADER);
						frameInfo.pData		= new unsigned char [frameInfo.length];
						memcpy(frameInfo.pData, &m_pBitmapInfo[channel], sizeof(BITMAPINFOHEADER));

						CFrameData *pFormat = new CFrameData (frameInfo, 0, CMD_REPLY_DATA_STREAM, INVALID_INDEX(unsigned long));
						//pFormat->SetProtocol(CMD_REPLY_DATA_STREAM);
						//pFormat->SetDataId(INVALID_INDEX(unsigned long));
						pFormat->Add();
						m_pMsgMan->PutFrame(pFormat);
						m_pRecMan->DateProc(*pFormat, bKeyFrame);
						pFormat->Dec();

						m_pRecMan->DateProc(*pFrameData, bKeyFrame);
					}
					else if (RECORD_WRITE_NO_WAVEFORMAT == ret)
					{
						frameInfo.frameType = FRAME_TYPE_AUDIO_FORMAT;
						frameInfo.length	= sizeof(WAVEFORMATEX);
						frameInfo.pData		= new unsigned char [frameInfo.length];
						memcpy(frameInfo.pData, &m_pWaveFormat[channel], sizeof(WAVEFORMATEX));

						CFrameData *pFormat = new CFrameData (frameInfo, 0, CMD_REPLY_DATA_STREAM, INVALID_INDEX(unsigned long));
						//pFormat->SetProtocol(CMD_REPLY_DATA_STREAM);
						//pFormat->SetDataId(INVALID_INDEX(unsigned long));
						pFormat->Add();
						m_pMsgMan->PutFrame(pFormat);
						m_pRecMan->DateProc(*pFormat, bKeyFrame);
						
						pFormat->Dec();

						m_pRecMan->DateProc(*pFrameData, bKeyFrame);
					}
					else
					{

					}
					
					if(frameInfo.frameType == FRAME_TYPE_AUDIO)
					{
						m_pAudioFrameTime[channel] = frameInfo.time;
					}

					pFrameData->Dec();
					bufferFrames.pFrameList->push_back(pFrameData);
				}

				bufferFrames.bufferInfo.pStatus = new long;
				*bufferFrames.bufferInfo.pStatus = IP_BUFFER_READING;
				bufferFrames.bufferInfo.chipId = iDecCard;
				//调用底层接口解码显示该数据块
				pIPCDecDisplayMan->SetDataBuffer(&bufferFrames.bufferInfo);
				m_pBufferFramesList[iDecCard].push_back(bufferFrames);

				pChipDecodeParam[iDecCard].busyCount = 0;
				list<BUFFER_FRAMES>::iterator it = m_pBufferFramesList[iDecCard].begin();
				for(; it != m_pBufferFramesList[iDecCard].end(); it++)
				{
					if(IP_BUFFER_FREE != *(*it).bufferInfo.pStatus)
					{
						pChipDecodeParam[iDecCard].busyCount++;
					}
				}
				
				ReleaseFrameInfoNet(iDecCard);
			}

			bGetDataAgain = bHasMuchDataBuffer;
		} while(bGetDataAgain);

		m_netStreamLock.UnLock();

		if (!bHasMuchDataBuffer)
		{
			PUB_Sleep(11);
		}

	}//end while

	for(iDecCard = 0; iDecCard < m_decCardNum; iDecCard++)
	{
		ReleaseFrameInfoNet(iDecCard);
	}
	
	for(unsigned char i = 0; i < m_netVideoInputNum; i++)
	{
		delete [] ppFrameTime[i];
	}
	delete [] ppFrameTime;
	delete [] pFrameNum;
	delete [] pChipDecodeParam;

#ifdef __ENVIRONMENT_WIN32___
	m_pLocalDevice->PlaybackStop(0xff);
#endif
#endif
}

RESULT WINAPI CStreamProc::Net_SecondStreamThread(LPVOID lpParameter)
{
#ifdef __ENVIRONMENT_LINUX__
	int pid = GetThisThreadId();
	printf("%s, %d, pid = %d\n", __FILE__, __LINE__, pid);

#if defined (__CHIP3531__)
	BindThreadToCpu(pid, 1);
#endif
#endif

	assert(NULL != lpParameter);

	CStreamProc *pThis = reinterpret_cast<CStreamProc *>(lpParameter);

	pThis->Net_SecondStreamProc();

	return 0;
}

void CStreamProc::Net_SecondStreamProc()
{
#ifdef __DVR_ULTIMATE__
	bool bKeyFrame = false;
	FRAME_INFO_EX frameInfo;
	memset(&frameInfo, 0, sizeof(FRAME_INFO_EX));

	DVR_IP_IN_FRM_INFO *pIPFrameInfo;
	CFrameData *pFrameData = NULL;
	unsigned char channel = 0;
	RECORD_WRITE_STATUS ret = RECORD_WRITE_FAIL;
	BUFFER_FRAMES bufferFrames;

	bool bNeedVideoFormat[64];
	memset(bNeedVideoFormat, true, sizeof(bool)*(64));

	unsigned long ulGetFrmCnt[64];
	memset(ulGetFrmCnt, true, sizeof(unsigned long) * (64));

	long bufferNum = 0;
	long readCursor = 0;

	bool bGetDataAgain = false;

	m_nsgmTime = GetCurrTime();

#ifdef __ENVIRONMENT_LINUX__
	m_nsrelativeTime = GetTickCount64();;
#else
	m_nsrelativeTime = GetTickCount() * 1000;
#endif

	while (m_bStreamNetSecond)
	{
		if(!m_bNetStreamAction)
		{
			PUB_Sleep(10);
			continue;
		}

		do
		{
			if(m_pNetDeviceMan->IsBufferFull(0,NET_DEVICE_SUB_STREAM))
			{
				m_pMsgMan->ClearFrame(LIVE_FRAME_SECOND_STREAM);
				//清除网络队列
#ifdef __NETSERVER_TVT__
				CNetServer::Instance()->ClearData(LIVE_FRAME_SECOND_STREAM);
#endif
			}
			
			//整理队列。
			ReleaseFrameInfoNetSecond();

			bGetDataAgain = false;
			bufferNum = m_pNetDeviceMan->GetBufferedNum(0, 1);

			if(bufferNum == 0)
			{
				PUB_Sleep(20);
				break;
			}
			else if(bufferNum > 2)
			{
				bGetDataAgain = true;
			}

			if(!m_pNetDeviceMan->GetDataBuffer(0, bufferFrames.bufferInfo.pData, bufferFrames.bufferInfo.dataLen, 1))
			{
				PUB_Sleep(20);
				break;
			}

			bufferFrames.pFrameList = new std::list<CFrameData *>();

			for(readCursor = 0; readCursor < bufferFrames.bufferInfo.dataLen;)
			{
				assert((bufferFrames.bufferInfo.dataLen - readCursor) >= sizeof(DVR_IP_IN_FRM_INFO));
				if((bufferFrames.bufferInfo.dataLen - readCursor) < sizeof(DVR_IP_IN_FRM_INFO))
				{
					break;
				}

				pIPFrameInfo = (DVR_IP_IN_FRM_INFO *)(bufferFrames.bufferInfo.pData+readCursor);
				frameInfo.channel = pIPFrameInfo->channel;
				frameInfo.frameType = pIPFrameInfo->data_type;
				frameInfo.keyFrame = pIPFrameInfo->keyframe;
				frameInfo.length = pIPFrameInfo->len;
				frameInfo.time = pIPFrameInfo->cur_time;
				if (0 == m_localVideoInputNum)
				{
					frameInfo.relativeTime = pIPFrameInfo->relative_time;
				}
				else
				{
					frameInfo.relativeTime = GetNetRelativeTime(m_nsgmTime, m_nsrelativeTime, pIPFrameInfo->relative_time);
				}
				frameInfo.width = pIPFrameInfo->width;
				frameInfo.height = pIPFrameInfo->height;
				frameInfo.frameAttrib = LIVE_FRAME_SECOND_STREAM;

				pIPFrameInfo->channel -= m_localVideoInputNum;
				channel = CHANNEL_ID(pIPFrameInfo->channel);

				readCursor += sizeof(DVR_IP_IN_FRM_INFO);
				frameInfo.pData = bufferFrames.bufferInfo.pData+readCursor;
				readCursor += pIPFrameInfo->vir_len;

				if(readCursor > bufferFrames.bufferInfo.dataLen)
				{
					printf("readCursor = %d, dataLen = %d\n", readCursor, bufferFrames.bufferInfo.dataLen);
					assert(false);
				}

				if (FRAME_TYPE_VIDEO_FORMAT == frameInfo.frameType)
				{
					m_pSecondBitmapInfo[channel] = *reinterpret_cast<BITMAPINFOHEADER *>(frameInfo.pData);
				}

				pFrameData = new CFrameData (frameInfo, 1, CMD_REPLY_DATA_STREAM, INVALID_INDEX(unsigned long));
				assert(NULL != pFrameData);

				if (bNeedVideoFormat[channel])
				{
					frameInfo.frameType = FRAME_TYPE_VIDEO_FORMAT;
					frameInfo.length	= sizeof(BITMAPINFOHEADER);
					frameInfo.pData		= new unsigned char [frameInfo.length];
					memcpy(frameInfo.pData, &m_pSecondBitmapInfo[channel], sizeof(BITMAPINFOHEADER));

					CFrameData *pFormat = new CFrameData (frameInfo, 0, CMD_REPLY_DATA_STREAM, INVALID_INDEX(unsigned long));
					//pFormat->SetProtocol(CMD_REPLY_DATA_STREAM);
					//pFormat->SetDataId(INVALID_INDEX(unsigned long));
					pFormat->Add();
					m_pMsgMan->PutFrame(pFormat);
					pFormat->Dec();

					bNeedVideoFormat[channel] = false;
				}

				ulGetFrmCnt[channel]++;

				m_pMsgMan->PutFrame(pFrameData);

				bufferFrames.pFrameList->push_back(pFrameData);
			}

			bufferFrames.bufferInfo.chipId = 0;
			m_secondBufferFramesList.push_back(bufferFrames);
		} while(bGetDataAgain);
	}//end while
#endif
}

RESULT WINAPI CStreamProc::Net_MinorStreamThread(LPVOID lpParameter)
{
#ifdef __ENVIRONMENT_LINUX__
	int pid = GetThisThreadId();
	printf("%s, %d, pid = %d\n", __FILE__, __LINE__, pid);
	
#if defined (__CHIP3531__)
	BindThreadToCpu(pid, 1);
#endif

#endif

	assert(NULL != lpParameter);

	CStreamProc *pThis = reinterpret_cast<CStreamProc *>(lpParameter);

	pThis->Net_MinorStreamProc();

	return 0;
}

void CStreamProc::Net_MinorStreamProc()
{
#ifdef __DVR_ULTIMATE__
	unsigned long prevPrintTick = 0;
	bool	bKeyFrame = false;
	FRAME_INFO_EX frameInfo, playFrameInfo;
	memset(&frameInfo, 0, sizeof(FRAME_INFO_EX));
	DVR_IP_IN_FRM_INFO *pIPFrameInfo; 
	CFrameData *pFrameData = NULL;
	unsigned char iDecCard = 0, channel = 0;
	BUFFER_FRAMES bufferFrames;
	RECORD_WRITE_STATUS ret = RECORD_WRITE_FAIL;
	long maxFrameNum = (VIDEO_FORMAT_PAL == g_video_format) ? 25 : 30;

	CHIP_DECODE_PARAM *pChipDecodeParam = new CHIP_DECODE_PARAM[m_decCardNum];
	memset(pChipDecodeParam, 0, sizeof(CHIP_DECODE_PARAM) * m_decCardNum);

	memset(&playFrameInfo, 0, sizeof(FRAME_INFO_EX));
	playFrameInfo.frameType = FRAME_TYPE_VIDEO;

	LONGLONG **ppFrameTime = new LONGLONG*[m_netVideoInputNum];
	for(unsigned char i = 0; i < m_netVideoInputNum; i++)
	{
		ppFrameTime[i] = new LONGLONG[8];
		memset(ppFrameTime[i], 0, sizeof(LONGLONG) * 8);
	}
	long *pFrameNum = new long[m_netVideoInputNum];
	memset(pFrameNum, 0, m_netVideoInputNum * sizeof(long));

	long bufferNum = 0;
	bool bGetDataAgain = false;
	bool bHasMuchDataBuffer = false;

#ifdef __ENVIRONMENT_WIN32__1
	m_pLocalDevice->PlaybackStart(0x0f);
#endif
	long readCursor = 0;

	ULONGLONG displayCH, displayCHMinor;
	CIPCDecDisplayMan * pIPCDecDisplayMan = CIPCDecDisplayMan::Instance();
	CLocalDevice * pLocalDevice = CLocalDevice::Instance();
	long leftFrameNum = 0;
	LONGLONG timeSpan = 0;
	bool bNeedSleep = false;

	while (m_bStreamNetMinor)
	{
		if (bNeedSleep)
		{
			PUB_Sleep(10);
			bNeedSleep = false;
		}

		m_netMinorStreamLock.Lock();
		if(!m_bNetMinorStreamAction)
		{
			m_netMinorStreamLock.UnLock();
			PUB_Sleep(10);
			continue;
		}

		bGetDataAgain = false;
		do
		{
			bHasMuchDataBuffer = false;
			for(iDecCard = 0; iDecCard < m_decCardNum; iDecCard++)
			{
				bufferNum = m_pNetDeviceMan->GetBufferedNum(iDecCard, NET_DEVICE_MINOR_STREAM);
				if(bufferNum == 0)
				{
					bNeedSleep = true;
					continue;
				}
				else if(bufferNum <= 1)
				{
					if(bGetDataAgain || (pChipDecodeParam[iDecCard].busyCount >= 8))
					{
						bNeedSleep = true;
						continue;
					}
				}
				else if(bufferNum > 2)
				{
					bHasMuchDataBuffer = true;
				}

				if(!m_pNetDeviceMan->GetDataBuffer(iDecCard, bufferFrames.bufferInfo.pData, bufferFrames.bufferInfo.dataLen, NET_DEVICE_MINOR_STREAM))
				{
					bNeedSleep = true;
					continue;
				}

				bufferFrames.pFrameList = new std::list<CFrameData *>();

				for(readCursor = 0; readCursor < bufferFrames.bufferInfo.dataLen;)
				{
					assert((bufferFrames.bufferInfo.dataLen - readCursor) >= sizeof(DVR_IP_IN_FRM_INFO));
					if((bufferFrames.bufferInfo.dataLen - readCursor) < sizeof(DVR_IP_IN_FRM_INFO))
					{
						break;
					}

					pIPFrameInfo = (DVR_IP_IN_FRM_INFO *)(bufferFrames.bufferInfo.pData+readCursor);
					frameInfo.channel = pIPFrameInfo->channel;
					frameInfo.frameType = pIPFrameInfo->data_type;
					frameInfo.keyFrame = pIPFrameInfo->keyframe;
					frameInfo.length = pIPFrameInfo->len;
					frameInfo.time = pIPFrameInfo->cur_time;
					frameInfo.relativeTime = pIPFrameInfo->relative_time;
					frameInfo.width = pIPFrameInfo->width;
					frameInfo.height = pIPFrameInfo->height;
					frameInfo.frameAttrib = LIVE_FRAME_FIRST_STREAM;

					pIPFrameInfo->channel -= m_localVideoInputNum;
					channel = CHANNEL_ID(pIPFrameInfo->channel);

					if(frameInfo.frameType == FRAME_TYPE_VIDEO)
					{
						if(frameInfo.keyFrame)
						{
							pIPCDecDisplayMan->GetDisplayCH(displayCH, displayCHMinor);
							if(displayCHMinor & ((ULONGLONG)0x01 << channel))
							{
								leftFrameNum = pLocalDevice->GetDecLeftFrame(channel);

								{
									timeSpan = ppFrameTime[channel][1] - ppFrameTime[channel][0];

									if(timeSpan > 0 && ppFrameTime[channel][0] > 0 )
									{
										int frameRate = (int)(((pFrameNum[channel]-1) * 1000000 + timeSpan/2) / timeSpan);
										int prev_frmate = (int)m_pVORate[channel];
										int sys_frame = (int)maxFrameNum;

										if( sys_frame < m_pFrameRate[channel] )
										{
											sys_frame = m_pFrameRate[channel];
										}

										if( ppFrameTime[channel][4] != frameRate )
										{
											ppFrameTime[channel][4] = frameRate;

											if( ppFrameTime[channel][5] > 0 )
											{
												if( frameRate < prev_frmate )
												{
													frameRate = prev_frmate;
												}
												else if( frameRate > sys_frame )
												{
													frameRate = sys_frame;
												}
											}
											else
											{
												frameRate = sys_frame;
											}

											ppFrameTime[channel][5] = 0;

										}
										else if( ppFrameTime[channel][5] < 9 )
										{
											frameRate = sys_frame;
										}

										ppFrameTime[channel][5]++;

										if( leftFrameNum >= sys_frame/2 )
										{
											if( frameRate <= prev_frmate )
											{
												frameRate = prev_frmate + 1;
											}
										}

										if(m_pVORate[channel] != frameRate)	//恢复到正常速度
										{
											m_pVORate[channel] = frameRate;
										}
									}
								}
							}

							pFrameNum[channel] = 1;
							ppFrameTime[channel][0] = frameInfo.time;
						}
						else
						{
							ppFrameTime[channel][1] = frameInfo.time;
							pFrameNum[channel]++;
						}
					}

					pIPFrameInfo->enc_stream_type = m_pVORate[channel];

					readCursor += sizeof(DVR_IP_IN_FRM_INFO);
					frameInfo.pData = bufferFrames.bufferInfo.pData+readCursor;
					readCursor += pIPFrameInfo->vir_len;

					if(readCursor > bufferFrames.bufferInfo.dataLen)
					{
						printf("readCursor = %d, dataLen = %d\n", readCursor, bufferFrames.bufferInfo.dataLen);
						assert(false);
					}

					if (FRAME_TYPE_VIDEO_FORMAT == frameInfo.frameType)
					{
						m_pMinorBitmapInfo[channel] = *reinterpret_cast<BITMAPINFOHEADER *>(frameInfo.pData);
					}
					else if (FRAME_TYPE_AUDIO_FORMAT == frameInfo.frameType)
					{
						m_pMinorWaveFormat[channel] = *reinterpret_cast<WAVEFORMATEX *>(frameInfo.pData);
					}

#ifdef __ENVIRONMENT_WIN32__1
					ULONGLONG displayCH, displayCHMinor;
					pIPCDecDisplayMan->GetDisplayCH(displayCH, displayCHMinor);

					if((0x01 << frameInfo.channel) & displayCHMinor)
					{
						FRAME_INFO_EX playFrameInfo = frameInfo;
						playFrameInfo.channel -= m_localVideoInputNum;
						while(!m_pLocalDevice->PlaybackGetBuf(&playFrameInfo))
						{
							PUB_Sleep(1);
						}
						memcpy(playFrameInfo.pData, frameInfo.pData, frameInfo.length);
						playFrameInfo.length = frameInfo.length;
						m_pLocalDevice->PlaybackSetBuf(&playFrameInfo);
					}
#endif

					pFrameData = new CFrameData (frameInfo, 1, CMD_REPLY_DATA_STREAM, INVALID_INDEX(unsigned long));
					assert(NULL != pFrameData);

					pFrameData->Add();

					//送入录像模块
					//printf("Minor stream process-0-0\n");
					ret = m_pStreamRecManEx->DateProc(*pFrameData, bKeyFrame);

					//printf("Minor stream process-0\n");
					if (RECORD_WRITE_NO_BITMAPINFO == ret)
					{
						//printf("Minor stream process-1\n");
						frameInfo.frameType = FRAME_TYPE_VIDEO_FORMAT;
						frameInfo.length	= sizeof(BITMAPINFOHEADER);
						frameInfo.pData		= new unsigned char [frameInfo.length];
						memcpy(frameInfo.pData, &m_pMinorBitmapInfo[channel], sizeof(BITMAPINFOHEADER));

						CFrameData *pFormat = new CFrameData (frameInfo, 0, CMD_REPLY_DATA_STREAM, INVALID_INDEX(unsigned long));
						//pFormat->SetProtocol(CMD_REPLY_DATA_STREAM);
						//pFormat->SetDataId(INVALID_INDEX(unsigned long));
						pFormat->Add();
						m_pStreamRecManEx->DateProc(*pFormat, bKeyFrame);
						//m_pMsgMan->PutFrame(pFormat);
						pFormat->Dec();

						m_pStreamRecManEx->DateProc(*pFrameData, bKeyFrame);
					}
					else if (RECORD_WRITE_NO_WAVEFORMAT == ret)
					{
						//printf("Minor stream process-2\n");
						frameInfo.frameType = FRAME_TYPE_AUDIO_FORMAT;
						frameInfo.length	= sizeof(WAVEFORMATEX);
						frameInfo.pData		= new unsigned char [frameInfo.length];
						memcpy(frameInfo.pData, &m_pMinorWaveFormat[channel], sizeof(WAVEFORMATEX));

						CFrameData *pFormat = new CFrameData (frameInfo, 0, CMD_REPLY_DATA_STREAM, INVALID_INDEX(unsigned long));
						//pFormat->SetProtocol(CMD_REPLY_DATA_STREAM);
						//pFormat->SetDataId(INVALID_INDEX(unsigned long));
						pFormat->Add();
						m_pStreamRecManEx->DateProc(*pFormat, bKeyFrame);
						//m_pMsgMan->PutFrame(pFormat);
						pFormat->Dec();

						m_pStreamRecManEx->DateProc(*pFrameData, bKeyFrame);
					}
					else
					{
						//printf("Minor stream process-3\n");
					}
					//m_pMsgMan->PutFrame(pFrameData);
						//printf("Minor stream process-4\n");

					/*if(frameInfo.frameType == FRAME_TYPE_AUDIO)
					{
						m_pAudioFrameTime[channel] = frameInfo.time;
					}*/

					pFrameData->Dec();
					bufferFrames.pFrameList->push_back(pFrameData);
				}

				bufferFrames.bufferInfo.pStatus = new long;
				*bufferFrames.bufferInfo.pStatus = IP_BUFFER_READING;
				bufferFrames.bufferInfo.chipId = iDecCard;
				//调用底层接口解码显示该数据块
				bool bRet = pIPCDecDisplayMan->SetMinorDataBuffer(&bufferFrames.bufferInfo);
				if(!bRet)
				{
					printf("SetMinorDataBuffer failed.....\n");
				}

				m_pMinorBufferFramesList[iDecCard].push_back(bufferFrames);

				ReleaseFrameInfoNetMinor(iDecCard);
			}

			bGetDataAgain = bHasMuchDataBuffer;
		} while(bGetDataAgain);

		for(iDecCard = 0; iDecCard < m_decCardNum; iDecCard++)
		{
			//整理队列。
			ReleaseFrameInfoNetMinor(iDecCard);
			
			if(m_pNetDeviceMan->IsBufferFull(iDecCard, NET_DEVICE_MINOR_STREAM))
			{
				unsigned long curTick = GetTickCount();
				//if((curTick - prevPrintTick) > 1000)
				{
					prevPrintTick = curTick;

					if(!m_pMinorBufferFramesList[iDecCard].empty())
					{
						if(m_pMinorBufferFramesList[iDecCard].front().pFrameList != NULL)
						{
							printf("Minor stream process......Buffer is full! Remove all pre-record data\n");
							
							m_pRecMan->RemoveAllDate();
							ReleaseFrameInfoNetMinor(iDecCard);
							
						}
						else
						{
							printf("Minor stream process......Buffer is full! Because of not release by decode!\n");
							m_pNetDeviceMan->PrintBufferInfo(iDecCard,NET_DEVICE_MINOR_STREAM);
						}
					}
					else
					{
						printf("Minor stream process.......Buffer is full! Because of get to slow!\n");
						if(prevPrintTick == 0)
						{
							prevPrintTick = curTick;
						}
						else if((curTick - prevPrintTick) > 1000)
						{
							m_pNetDeviceMan->ClearDataBuffer(NET_DEVICE_MINOR_STREAM);
							prevPrintTick = curTick;
						}
					}
				}
			}

			pChipDecodeParam[iDecCard].busyCount = 0;
			list<BUFFER_FRAMES>::iterator it = m_pMinorBufferFramesList[iDecCard].begin();
			for(; it != m_pMinorBufferFramesList[iDecCard].end(); it++)
			{
				if(IP_BUFFER_FREE != *(*it).bufferInfo.pStatus)
				{
					pChipDecodeParam[iDecCard].busyCount++;
				}
			}
		}//end对所有芯片检查释放资源

		m_netMinorStreamLock.UnLock();

		//PUB_Sleep(2);
	}//end while

	for(unsigned char i = 0; i < m_netVideoInputNum; i++)
	{
		delete [] ppFrameTime[i];
	}
	delete [] ppFrameTime;
	delete [] pFrameNum;
	delete [] pChipDecodeParam;

#ifdef __ENVIRONMENT_WIN32__1
	m_pLocalDevice->PlaybackStop(0xff);
#endif
#endif
}

RESULT WINAPI CStreamProc::Net_JPEGStreamThread(LPVOID lpParameter)
{
#ifdef __ENVIRONMENT_LINUX__
	int pid = GetThisThreadId();
	printf("%s, %d, pid = %d\n", __FILE__, __LINE__, pid);
#endif

	CStreamProc *pThis = (CStreamProc *)lpParameter;
	pThis->Net_JPEGStreamProc();

	return 0;
}

void CStreamProc::Net_JPEGStreamProc()
{
#ifdef __DVR_ULTIMATE__
	FRAME_INFO_EX frameInfo;
	memset(&frameInfo, 0, sizeof(FRAME_INFO_EX));
	DVR_IP_IN_FRM_INFO *pIPFrameInfo; 
	CFrameData *pFrameData = NULL;
	unsigned char channel = 0;
	BUFFER_FRAMES bufferFrames;

#if defined(__SUPPORT_SNAPD1__)
	for (int i = 0; i < m_netVideoInputNum; i++)
	{
		CSnapCapabilityMan::Instance()->StarSnapJpeg(i,false);
	}
#endif

	long bufferNum = 0;
	bool bGetDataAgain = false;
	long readCursor = 0;

	ULONGLONG ullDisconnect = m_pMsgMan->GetNetDeviceStatus();

	while (m_bJPEGNetStream)
	{
		if(!m_bJPEGNetStreamAction)
		{
			PUB_Sleep(10);
			continue;
		}

		ullDisconnect = m_pMsgMan->GetNetDeviceStatus();
		if(ullDisconnect != 0)
		{
			for(unsigned char i = 0; i < m_netVideoInputNum; i++)
			{
				if(ullDisconnect & (0x01 << m_netVideoInputNum))
				{
#ifndef WIN32
					memset(&frameInfo, 0, sizeof(FRAME_INFO_EX));
					frameInfo.channel = i;

					pFrameData = new CFrameData(frameInfo, 1);
					CSnapProc::Instance()->SnapJPEGStream(pFrameData);

					delete pFrameData;
#endif
				}
			}
		}

		do 
		{
			bGetDataAgain = false;
			bufferNum = m_pNetDeviceMan->GetBufferedNum(0, NET_DEVICE_JPEG_STREAM);

			if(bufferNum > 2)
			{
				bGetDataAgain = true;
			}

			if(!m_pNetDeviceMan->GetDataBuffer(0, bufferFrames.bufferInfo.pData, bufferFrames.bufferInfo.dataLen, NET_DEVICE_JPEG_STREAM))
			{
				PUB_Sleep(200);
				continue;
			}

			bufferFrames.pFrameList = new std::list<CFrameData *>();

			for(readCursor = 0; readCursor < bufferFrames.bufferInfo.dataLen;)
			{
				assert((bufferFrames.bufferInfo.dataLen - readCursor) >= sizeof(DVR_IP_IN_FRM_INFO));
				if((bufferFrames.bufferInfo.dataLen - readCursor) < sizeof(DVR_IP_IN_FRM_INFO))
				{
					break;
				}

				pIPFrameInfo = (DVR_IP_IN_FRM_INFO *)(bufferFrames.bufferInfo.pData+readCursor);
				frameInfo.channel = pIPFrameInfo->channel;
				frameInfo.frameType = pIPFrameInfo->data_type;
				frameInfo.keyFrame = pIPFrameInfo->keyframe;
				frameInfo.length = pIPFrameInfo->len;
				frameInfo.time = pIPFrameInfo->cur_time;
				
				frameInfo.relativeTime = pIPFrameInfo->relative_time;
				frameInfo.width = pIPFrameInfo->width;
				frameInfo.height = pIPFrameInfo->height;
				frameInfo.frameAttrib = LIVE_FRAME_JPEG;

				pIPFrameInfo->channel -= m_localVideoInputNum;
				channel = CHANNEL_ID(pIPFrameInfo->channel);

				readCursor += sizeof(DVR_IP_IN_FRM_INFO);
				frameInfo.pData = bufferFrames.bufferInfo.pData+readCursor;
				
				readCursor += pIPFrameInfo->vir_len;

				if(readCursor > bufferFrames.bufferInfo.dataLen)
				{
					printf("readCursor = %d, dataLen = %d\n", readCursor, bufferFrames.bufferInfo.dataLen);
					assert(false);
				}

				pFrameData = new CFrameData (frameInfo, 1, CMD_REPLY_DATA_STREAM, INVALID_INDEX(unsigned long));
				//pFrameData->SetProtocol(CMD_REPLY_DATA_STREAM);
				//pFrameData->SetDataId(INVALID_INDEX(unsigned long));

				assert(NULL != pFrameData);

				pFrameData->Add();

				//送入email抓图
#ifndef WIN32
				CSnapProc::Instance()->SnapJPEGStream(pFrameData);


				if (g_bHaveSnapPic)
				{
#ifdef __SUPPORT_SNAPD1__
					CShanghaiSnapMan::GetInstance()->ShanghaiAlarmManualSnapPicture(frameInfo.pData, frameInfo.length, frameInfo.channel, frameInfo.time, frameInfo.width, frameInfo.height);
#else
					CSnapPictureMan::GetInstance()->SnapPicture(frameInfo.pData, frameInfo.length, frameInfo.channel, frameInfo.time, frameInfo.width, frameInfo.height);
#endif				
				}
#ifdef __SUPPORT_SNAPD1__
				CShanghaiSnapMan::GetInstance()->ShanghaiDailySnapPicture(frameInfo.pData, frameInfo.length, frameInfo.channel,frameInfo.time, frameInfo.width, frameInfo.height);
				CShanghaiSnapMan::GetInstance()->ShanghaiManualSnapPicture(frameInfo.pData, frameInfo.length, frameInfo.channel,frameInfo.time, frameInfo.width, frameInfo.height);
				CShanghaiSnapMan::GetInstance()->ShanghaiOnDemandSnapPicture(frameInfo.pData, frameInfo.length, frameInfo.channel, frameInfo.time);
#endif

				m_pMsgMan->PutFrame(pFrameData);
#endif
				pFrameData->Dec();

				bufferFrames.pFrameList->push_back(pFrameData);
			}

			bufferFrames.bufferInfo.chipId = 0;
			m_jpegBufferFramesList.push_back(bufferFrames);
		} while(bGetDataAgain);

		//整理队列。
		ReleaseFrameInfoNetJpeg();

		PUB_Sleep(20);
	}//end while
#endif
}


RESULT WINAPI CStreamProc::TalkStreamThread(LPVOID lpParameter)
{
#ifdef __ENVIRONMENT_LINUX__
	pid_t pid = getpid();
	pid_t tid = syscall(__NR_gettid);
	printf("%s, %d, pid = %d, tid=%d\n", __FILE__, __LINE__, pid, tid);
#endif

	CStreamProc *pThis = (CStreamProc *)lpParameter;
	pThis->TalkStreamProc();

	return 0;
}

void CStreamProc::TalkStreamProc()
{
	CFrameData *pFrameData;
	FRAME_INFO_EX localFrameInfo, clientFrameInfo;

	bool bNeedFormat = true;
	bool bGet = false;

	memset(&clientFrameInfo, 0, sizeof(FRAME_INFO_EX));
	clientFrameInfo.frameType = FRAME_TYPE_TALK_AUDIO;
	clientFrameInfo.length = 840;
	while(m_bTalkStream)
	{
		m_talkStreamLock.Lock();
		if(m_bWouldStopTalk)
		{
			m_talkStreamLock.UnLock();
			break;
		}

		bGet = false;
		//首先采集本地的对讲数据
		if(bNeedFormat)
		{
			WAVEFORMATEX wfx;
			if(m_pLocalDevice->GetAudioFormat(wfx))
			{
				bNeedFormat = false;
				
				memset(&localFrameInfo, 0, sizeof(FRAME_INFO_EX));
				localFrameInfo.frameType = FRAME_TYPE_TALK_AUDIO_FORMAT;
				localFrameInfo.frameAttrib = LIVE_FRAME_TALK;
				localFrameInfo.length = sizeof(WAVEFORMATEX);
				localFrameInfo.pData = new unsigned char[localFrameInfo.length];
				memcpy(localFrameInfo.pData, &wfx, sizeof(WAVEFORMATEX));
				
				pFrameData = new CFrameData(localFrameInfo, 0, CMD_REPLY_DATA_STREAM, INVALID_INDEX(unsigned long));
				//pFrameData->SetProtocol( CMD_REPLY_DATA_STREAM);
				//pFrameData->SetDataId( INVALID_INDEX(unsigned long) );
				m_pMsgMan->PutFrame(pFrameData);
			}
		}

		if(CLocalDevice::GET_FRAME_SUCC == m_pLocalDevice->GetTalkWay(localFrameInfo))
		{
			bGet = true;

			localFrameInfo.frameType = FRAME_TYPE_TALK_AUDIO;
			localFrameInfo.frameAttrib = LIVE_FRAME_TALK;

			pFrameData = new CFrameData(localFrameInfo, 1, CMD_REPLY_DATA_STREAM, INVALID_INDEX(unsigned long));
			assert( NULL != pFrameData );
			//pFrameData->SetProtocol( CMD_REPLY_DATA_STREAM );
			//pFrameData->SetDataId( INVALID_INDEX(unsigned long) );

			pFrameData->Add();
			m_pMsgMan->PutFrame(pFrameData);

			if (0 < pFrameData->Dec())
			{
				m_talkFrameList.push_back(pFrameData);
			}
			else
			{
				m_pLocalDevice->ReleaseTalkWayBuf(pFrameData->GetFrameInfo());
				delete pFrameData;
				pFrameData = NULL;
			}
		}

		ReleaseTalkFrame();

		//以下为播放客户端发过来的对讲数据
		if(NULL == clientFrameInfo.pData)
		{
			m_pLocalDevice->TalkWayGetBuf(&clientFrameInfo);
		}
		
		if(NULL != clientFrameInfo.pData)
		{
			if(m_pMsgMan->GetTalkWayFrame(clientFrameInfo))
			{
#if defined(__TDFH__) || defined(__CHIPTI__)
				int count = clientFrameInfo.length / 324;
				for (int i=0; i<count;i++)
				{
					//FH平台的数据每帧不能大于512，
					//从客户端送来的数据每320字节会加4字节的头，这种情况是海斯平台要求的，在FH平台不要求有头
					FRAME_INFO_EX clientFrameInfoT = clientFrameInfo;
					clientFrameInfoT.pData = clientFrameInfo.pData + (i * 324 + 4);
					clientFrameInfoT.length = 320;
					m_pLocalDevice->TalkWaySetBuf(&clientFrameInfoT);	
				}
#else
				m_pLocalDevice->TalkWaySetBuf(&clientFrameInfo);
#endif

				clientFrameInfo.pData = NULL;
			}
		}

		m_talkStreamLock.UnLock();

		if(!bGet)
		{
			PUB_Sleep(10);
		}
	}	
}


#ifdef __DVR_BASIC__
RESULT WINAPI CStreamProc::JPEGStreamThread(LPVOID lpParameter)
{
#ifdef __ENVIRONMENT_LINUX__
	pid_t pid = getpid();
	pid_t tid = syscall(__NR_gettid);
	printf("%s, %d, pid = %d, tid=%d\n", __FILE__, __LINE__, pid, tid);
#endif

	assert( NULL != lpParameter );
	CStreamProc *pThis = reinterpret_cast<CStreamProc*>(lpParameter);
	pThis->JPEGStreamProc();

	return 0;
}

void CStreamProc::JPEGStreamProc()
{
	FRAME_INFO_EX frameInfo;
	CFrameData *pFrameData = NULL;
	int	getRet = CLocalDevice::GET_FRAME_FAIL;
	char count = 0;
	char videoNum = CProduct::Instance()->LocalVideoInputNum();
	int channel = 0;
	bool bSleep = false;

#if defined(__SUPPORT_SNAPD1__)
	for (int i = 0; i < m_localVideoInputNum; i++)
	{
		CSnapCapabilityMan::Instance()->StarSnapJpeg(i,false);
	}
#endif

#if defined(__SUPPORT_HONGDI__)
	for (int i = 0; i < m_localVideoInputNum; i++)
	{
		CSnapCapabilityMan::Instance()->StarSnapJpeg(i,false);
	}
#endif

#if defined(__SUPPORT_WAP__)
	for (int i = 0; i < m_localVideoInputNum; i++)
	{
		CSnapCapabilityMan::Instance()->StarSnapJpeg(i,true);
	}
#endif

	
	while( m_bJPEGStream )
	{
		m_JPEGStreamLock.Lock();
		if (!m_bJPEGStreamAction)
		{
			m_JPEGStreamLock.UnLock();
			PUB_Sleep(10);
		}
		else 
		{
			memset(&frameInfo, 0, sizeof(frameInfo));
			getRet = m_pLocalDevice->GetJPEGStream(frameInfo);

			if( CLocalDevice::GET_FRAME_SUCC == getRet)
			{		
				channel = frameInfo.channel;
				assert((channel >=0) && (channel <= videoNum));
				frameInfo.frameAttrib = LIVE_FRAME_JPEG;
				pFrameData = new CFrameData(frameInfo, 1, CMD_REPLY_DATA_STREAM, INVALID_INDEX(unsigned long));
				assert( NULL != pFrameData );
				//pFrameData->SetProtocol( CMD_REPLY_DATA_STREAM );
				//pFrameData->SetDataId( INVALID_INDEX(unsigned long) );
				pFrameData->Add();

				//送入email抓图
#if defined (__ENVIRONMENT_LINUX__) && defined (__DVR_BASIC__)
				CSnapProc::Instance()->SnapJPEGStream(pFrameData);
#endif

#if defined (__ENVIRONMENT_LINUX__) && defined (__DVR_BASIC__)
				if (g_bHaveSnapPic)
				{
#if !defined(__SUPPORT_HONGDI__)
#ifdef __SUPPORT_SNAPD1__
					
					CShanghaiSnapMan::GetInstance()->ShanghaiAlarmManualSnapPicture(frameInfo.pData, frameInfo.length, frameInfo.channel, frameInfo.time, frameInfo.width, frameInfo.height);
#else
					CSnapPictureMan::GetInstance()->SnapPicture(frameInfo.pData, frameInfo.length, frameInfo.channel, frameInfo.time, frameInfo.width, frameInfo.height);
#endif
#endif
				}
#endif

#if defined(__SUPPORT_HONGDI__)
				CSnapMan::GetInstance()->SaveSnapPicture(frameInfo.pData, frameInfo.length, 1, frameInfo.channel, frameInfo.time, frameInfo.width, frameInfo.height);
#endif

#if defined (__CUSTOM_YUANZHENG__)
				CYZSnapMan::Instance()->SnapPicture(frameInfo.pData, frameInfo.length, frameInfo.channel,frameInfo.time);
#endif

#ifdef __SUPPORT_SNAPD1__
			
			CShanghaiSnapMan::GetInstance()->ShanghaiDailySnapPicture(frameInfo.pData, frameInfo.length, frameInfo.channel,frameInfo.time, frameInfo.width, frameInfo.height);
			
			CShanghaiSnapMan::GetInstance()->ShanghaiManualSnapPicture(frameInfo.pData, frameInfo.length, frameInfo.channel,frameInfo.time, frameInfo.width, frameInfo.height);

			CShanghaiSnapMan::GetInstance()->ShanghaiOnDemandSnapPicture(frameInfo.pData, frameInfo.length, frameInfo.channel, frameInfo.time);
#endif
				//加入网络队列
				m_pMsgMan->PutFrame( pFrameData );

				if( 0 < pFrameData->Dec())
				{
					m_pJPEGFrameList[channel].push_back(pFrameData);
				}
				else
				{
					m_pLocalDevice->ReleaseJPEGBuf(pFrameData->GetFrameInfo());
					delete pFrameData;
					pFrameData = NULL;
				}
			}

			if (count < videoNum)
			{
				ReleaseJPEGFrameInfo(count);	
				count++;
				bSleep = false;
			}
			else
			{
				bSleep = true;
				count = 0;
			}


			m_JPEGStreamLock.UnLock();
#ifdef __TD2316SES_SPEC__
			PUB_Sleep(10);
#else
			if (bSleep)
			{	
#if defined(__CHIP3515__) || defined(__TDFH__) || defined(__CHIPGM__)
				PUB_Sleep(100);
#else
				PUB_Sleep(1);
#endif
			}
#endif

		}
	}
#if defined(__SUPPORT_HONGDI__)
	for (int i=0; i<m_localVideoInputNum; i++)
	{
		CSnapCapabilityMan::Instance()->StopSnapJpeg(i, false);
	}
#endif

#if defined(__SUPPORT_WAP__)
	{
		for (int i=0; i<m_localVideoInputNum; i++)
		{
			CSnapCapabilityMan::Instance()->StopSnapJpeg(i, true);
		}
	}
#endif
}
#endif

void CStreamProc::ReleaseFrameInfo(unsigned char chnn)
{
	std::list<CFrameData *>::iterator it;
	while (!m_pFrameList[chnn].empty())
	{
		it = m_pFrameList[chnn].begin();

		//printf("chnn:%d,i:%d,st:%d\n", (*it)->GetFrameInfo().channel, (*it)->GetFrameInfo().frameAttrib,(*it)->GetStatus());
		if (0 == (*it)->GetStatus())
		{
			m_pLocalDevice->ReleaseBuf((*it)->GetFrameInfo());
			delete (*it);
			m_pFrameList[chnn].pop_front();
		}
		else
		{
			break;
		}
	}
}

void CStreamProc::ReleaseSecondFrameInfo( unsigned char chnn )
{
	std::list<CFrameData *>::iterator it;
	while(!m_pSecondFrameList[chnn].empty())
	{
		it = m_pSecondFrameList[chnn].begin();
		if( 0 == (*it)->GetStatus())
		{
			m_pLocalDevice->ReleaseBuf((*it)->GetFrameInfo());
			delete (*it);
			m_pSecondFrameList[chnn].pop_front();
		}
		else
		{
			break;
		}
	}
}

#ifdef __DVR_BASIC__
void CStreamProc::ReleaseJPEGFrameInfo(unsigned char chnn)
{
	std::list<CFrameData *>::iterator it;
	while(!m_pJPEGFrameList[chnn].empty())
	{
		it = m_pJPEGFrameList[chnn].begin();
		if( 0 == (*it)->GetStatus())
		{
			m_pLocalDevice->ReleaseJPEGBuf((*it)->GetFrameInfo());
			delete (*it);
			m_pJPEGFrameList[chnn].pop_front();
		}
		else
		{
			break;
		}
	}
}
#endif

#ifdef __DVR_ULTIMATE__
void CStreamProc::ReleaseFrameInfoNet(unsigned char decCard)
{
	std::list<BUFFER_FRAMES>::iterator it_1;
	std::list<CFrameData *>::iterator it_2;

	while(!m_pBufferFramesList[decCard].empty())
	{
		it_1 = m_pBufferFramesList[decCard].begin();

		if((*it_1).pFrameList != NULL)
		{
			while(!(*it_1).pFrameList->empty())
			{
				it_2 = (*it_1).pFrameList->begin();
				if (0 == (*it_2)->GetStatus())
				{
					delete (*it_2);
					
					(*it_1).pFrameList->pop_front();
				}
				else
				{
					break;
				}
			}

			if((*it_1).pFrameList->empty())
			{
				delete (*it_1).pFrameList;
				(*it_1).pFrameList = NULL;
			}
		}

		if( (NULL == (*it_1).pFrameList) && (IP_BUFFER_FREE == *(*it_1).bufferInfo.pStatus) )
		{
			m_pNetDeviceMan->ReleaseDataBuffer((*it_1).bufferInfo.chipId, (*it_1).bufferInfo.pData, (*it_1).bufferInfo.dataLen);
			delete (*it_1).bufferInfo.pStatus;
			m_pBufferFramesList[decCard].pop_front();
		}
		else
		{
			break;
		}
	}
}

void CStreamProc::ReleaseFrameInfoNetMinor(unsigned char decCard)
{
	std::list<BUFFER_FRAMES>::iterator it_1;
	std::list<CFrameData *>::iterator it_2;

	while(!m_pMinorBufferFramesList[decCard].empty())
	{
		it_1 = m_pMinorBufferFramesList[decCard].begin();

		if((*it_1).pFrameList != NULL)
		{
			while(!((*it_1).pFrameList->empty()))
			{
				it_2 = (*it_1).pFrameList->begin();
				if (0 == (*it_2)->GetStatus())
				{
					delete (*it_2);
					(*it_1).pFrameList->pop_front();
				}
				else
				{
					break;
				}
			}

			if((*it_1).pFrameList->empty())
			{
				delete (*it_1).pFrameList;
				(*it_1).pFrameList = NULL;
			}
		}

		if( (NULL == (*it_1).pFrameList) && (IP_BUFFER_FREE == *(*it_1).bufferInfo.pStatus) )
		{
			m_pNetDeviceMan->ReleaseDataBuffer((*it_1).bufferInfo.chipId, (*it_1).bufferInfo.pData, (*it_1).bufferInfo.dataLen,NET_DEVICE_MINOR_STREAM);
			delete (*it_1).bufferInfo.pStatus;
			m_pMinorBufferFramesList[decCard].pop_front();
		}
		else
		{
			break;
		}
	}
}


void CStreamProc::ReleaseFrameInfoNetSecond()
{
	std::list<BUFFER_FRAMES>::iterator it_1;
	std::list<CFrameData *>::iterator it_2;

	while(!m_secondBufferFramesList.empty())
	{
		it_1 = m_secondBufferFramesList.begin();

		if((*it_1).pFrameList != NULL)
		{
			while(!(*it_1).pFrameList->empty())
			{
				it_2 = (*it_1).pFrameList->begin();
				if (0 == (*it_2)->GetStatus())
				{
					delete (*it_2);
					(*it_1).pFrameList->pop_front();
				}
				else
				{
					break;
				}
			}

			if((*it_1).pFrameList->empty())
			{
				delete (*it_1).pFrameList;
				(*it_1).pFrameList = NULL;
			}
		}

		if( (NULL == (*it_1).pFrameList) )
		{
			m_pNetDeviceMan->ReleaseDataBuffer((*it_1).bufferInfo.chipId, (*it_1).bufferInfo.pData, (*it_1).bufferInfo.dataLen, 1);
			m_secondBufferFramesList.pop_front();
		}
		else
		{
			break;
		}
	}
}

void CStreamProc::ReleaseFrameInfoNetJpeg()
{
	std::list<BUFFER_FRAMES>::iterator it_1;
	std::list<CFrameData *>::iterator it_2;

	while(!m_jpegBufferFramesList.empty())
	{
		it_1 = m_jpegBufferFramesList.begin();

		if((*it_1).pFrameList != NULL)
		{
			while(!(*it_1).pFrameList->empty())
			{
				it_2 = (*it_1).pFrameList->begin();
				if (0 == (*it_2)->GetStatus())
				{
					delete (*it_2);
					(*it_1).pFrameList->pop_front();
				}
				else
				{
					break;
				}
			}

			if((*it_1).pFrameList->empty())
			{
				delete (*it_1).pFrameList;
				(*it_1).pFrameList = NULL;
			}
		}

		if( (NULL == (*it_1).pFrameList) )
		{
			m_pNetDeviceMan->ReleaseDataBuffer((*it_1).bufferInfo.chipId, (*it_1).bufferInfo.pData, (*it_1).bufferInfo.dataLen, NET_DEVICE_JPEG_STREAM);
			m_jpegBufferFramesList.pop_front();
		}
		else
		{
			break;
		}
	}
}

void CStreamProc::ChangeRecStatusForNetStatus(ULONGLONG &ullDisconnectCH)
{
	ULONGLONG tempCH = ullDisconnectCH;
	ullDisconnectCH = m_pMsgMan->GetNetDeviceStatus();
	ULONGLONG newConnectCH = tempCH & ~ullDisconnectCH;
	tempCH = ullDisconnectCH & ~tempCH;
	if(0 != tempCH)
	{
		for(unsigned char chnn = m_localVideoInputNum; chnn < m_videoInputNum; chnn++)
		{
			if((tempCH >> chnn) & 0x01)
			{
				m_pStreamRecManEx->SetChannelLive(chnn, false);
			}
		}
	}
	if(0 != newConnectCH)
	{
		for(unsigned char chnn = m_localVideoInputNum; chnn < m_videoInputNum; chnn++)
		{
			if((newConnectCH >> chnn) & 0x01)
			{
				m_pStreamRecManEx->SetChannelLive(chnn, true);
			}
		}
	}
}

#endif

void CStreamProc::ReleaseTalkFrame()
{
	std::list<CFrameData *>::iterator it;
	while (! m_talkFrameList.empty())
	{
		it = m_talkFrameList.begin();

		if (0 == (*it)->GetStatus())
		{
			m_pLocalDevice->ReleaseTalkWayBuf((*it)->GetFrameInfo());
			delete (*it);
			m_talkFrameList.pop_front();
		}
		else
		{
			break;
		}
	}
}

//发送通道chnn的音视频格式
void CStreamProc::SendAVFormat(unsigned char chnn, bool bAudio/* = false*/)
{
	FRAME_INFO_EX frameInfo;
	memset( &frameInfo, 0, sizeof( FRAME_INFO_EX ) );
	bool bKeyFrame = false;

	if(!bAudio)
	{
		frameInfo.channel	= chnn;
		frameInfo.frameType = FRAME_TYPE_VIDEO_FORMAT;
		frameInfo.frameAttrib |= LIVE_FRAME_FIRST_STREAM;
		frameInfo.length	= sizeof(BITMAPINFOHEADER);
		frameInfo.pData		= new unsigned char [sizeof(BITMAPINFOHEADER)];
		m_pLocalDevice->GetVideoFormat(chnn, *reinterpret_cast<BITMAPINFOHEADER *>(frameInfo.pData));

		CFrameData *pFormat = new CFrameData (frameInfo, 0, CMD_REPLY_DATA_STREAM, INVALID_INDEX(unsigned long));
		//pFormat->SetProtocol(CMD_REPLY_DATA_STREAM);
		//pFormat->SetDataId(INVALID_INDEX(unsigned long));
		pFormat->Add();
		m_pRecMan->DateProc(*pFormat, bKeyFrame);
		m_pMsgMan->PutFrame(pFormat);
		pFormat->Dec();
	}
	else
	{
		frameInfo.channel	= chnn;
		frameInfo.frameType = FRAME_TYPE_AUDIO_FORMAT;
		frameInfo.frameAttrib |= LIVE_FRAME_FIRST_STREAM;
		frameInfo.length	= sizeof(WAVEFORMATEX);
		frameInfo.pData		= new unsigned char [sizeof(WAVEFORMATEX)];
		m_pLocalDevice->GetAudioFormat(*reinterpret_cast<WAVEFORMATEX *>(frameInfo.pData));

		CFrameData *pFormat = new CFrameData (frameInfo, CMD_REPLY_DATA_STREAM, INVALID_INDEX(unsigned long));
		//pFormat->SetProtocol(CMD_REPLY_DATA_STREAM);
		//pFormat->SetDataId(INVALID_INDEX(unsigned long));
		pFormat->Add();
		m_pRecMan->DateProc(*pFormat, bKeyFrame);
		m_pMsgMan->PutFrame(pFormat);
		pFormat->Dec();
	}
}

void CStreamProc::SetSecondStreamEncodeAttrib()
{
	unsigned long maxCopies = 0;
	unsigned long maxRate = 0;
	if (VIDEO_FORMAT_PAL == g_video_format)
	{
		maxRate = 25;
	}
	else/* if (VIDEO_FORMAT_NTSC == g_video_format)*/
	{
		maxRate = 30;
	}

	unsigned long totalRate = 0;
	for (unsigned char i=0; i<m_localVideoInputNum; ++i)
	{
		totalRate += UsedRate(m_pSecondStreamEncodeInfo[i].rate, static_cast<VIDEO_SIZE>(m_pSecondStreamEncodeInfo[i].resolution));
	}

	unsigned long num = 0, mobileNum = 0;
	for (unsigned char i=0; i<m_localVideoInputNum; ++i)
	{
		if ((m_secondStreamCH_Mobile & ((ULONGLONG)(0x01) << i)) || (m_secondStreamCH & ((ULONGLONG)(0x01) << i)))
		{
			++ num;
		}

		if ((m_secondStreamCH_Mobile & ((ULONGLONG)(0x01) << i)))
		{
			++ mobileNum;
		}
	}

	 maxCopies = CProduct::Instance()->GetSecondStreamCopies();

	 printf("%s,%d, maxCopies:%d,m_secondRateLimit:%d\n",__FILE__,__LINE__,maxCopies,m_secondRateLimit);
	 if (m_secondRateLimit != INVALID_INDEX(unsigned long))
	 {
		 printf("%s,%d, change maxCopies to %d\n",__FILE__, __LINE__,m_secondRateLimit);
		 maxCopies = m_secondRateLimit;
	 }
	 printf("%s,%d, m_secondSendAllNum:%d,maxCopies:%d, num:%d,mobileNum:%d\n",__FILE__,__LINE__,m_secondSendAllNum,maxCopies,num,mobileNum);
	 
	if ((maxCopies != INVALID_INDEX(unsigned long)) && (m_secondSendAllNum > 0))
	{
		unsigned long maxrateTemp = 0;
		unsigned long chnnMaxRate = maxRate;
		if (VIDEO_FORMAT_PAL == g_video_format)
		{
			chnnMaxRate = CProduct::Instance()->NetMaxChannelRatePal();
		}
		else
		{
			chnnMaxRate = CProduct::Instance()->NetMaxChannelRateNtsc();
		}

		maxrateTemp = maxCopies * chnnMaxRate/m_secondSendAllNum;

		unsigned long oneChnnMaxRate = 25;
		if (VIDEO_FORMAT_NTSC == g_video_format)
		{
			oneChnnMaxRate = 30;
		}

		if (maxrateTemp < 1)
		{
			maxrateTemp = 1;
		}
		else if (maxrateTemp <= oneChnnMaxRate)
		{

		}
		else
		{
			maxrateTemp = oneChnnMaxRate;
		}
		printf("%s,%d,m_secondSendAllNum:%d,maxrate:%d,maxrateTemp:%d\n",__FILE__,__LINE__,m_secondSendAllNum,maxRate,maxrateTemp);

		maxRate = maxrateTemp;	
	}
	
	if (m_secondRateLimit != INVALID_INDEX(unsigned long))
	{
		//如果是2308ME产品，只要在回放或备份时，子码流就设为1；
		PRODUCT_TYPE produtType = CProduct::Instance()->ProductType();
		if ((TD_2308ME == produtType) || (TD_2308SE == produtType) || (TD_2308CDS == produtType) || (TD_7008SB == produtType))
		{
			maxRate = 1;
		}
		printf("%s,%d,2308ME/SE ,playing or backuping , secondstream limited to %d\n",__FILE__,__LINE__,maxRate);
	}

	 if (m_secondLimitRateForFirstStreamNum != INVALID_INDEX(unsigned long))
	 {
		 if (7 == m_secondLimitRateForFirstStreamNum)
		 {
			 if (VIDEO_FORMAT_PAL == g_video_format)
			 {
				 m_secondLimitRateForFirstStreamNum -= 1;
			 }
		 }	

		 unsigned long oneChnnMaxRate = 25;
		 if (VIDEO_FORMAT_NTSC == g_video_format)
		 {
			 oneChnnMaxRate = 30;
		 }

		 if (m_secondLimitRateForFirstStreamNum < 1)
		 {
			 m_secondLimitRateForFirstStreamNum = 1;
		 }
		 else if (m_secondLimitRateForFirstStreamNum <= oneChnnMaxRate)
		 {
			 
		 }
		 else
		 {
			 m_secondLimitRateForFirstStreamNum = oneChnnMaxRate;
		 }

		 printf("%s, %d, cur second rate(%d) , secondLimit Max Rate for FirstStreamNum:%d\n",__FILE__,__LINE__,maxRate, m_secondLimitRateForFirstStreamNum);

		 if (maxRate > m_secondLimitRateForFirstStreamNum)
		 {
			 maxRate = m_secondLimitRateForFirstStreamNum;
		 }		 
	 }

	 if ((m_secondLimitAllRateForFirstStreamNum != INVALID_INDEX(unsigned long)) && (m_secondSendAllNum > 0))
	 {
		 unsigned long maxrateTemp = 0;
		 maxrateTemp = m_secondLimitAllRateForFirstStreamNum/m_secondSendAllNum;

		 unsigned long oneChnnMaxRate = 25;
		 if (VIDEO_FORMAT_NTSC == g_video_format)
		 {
			 oneChnnMaxRate = 30;
		 }

		 if (maxrateTemp < 1)
		 {
			 maxrateTemp = 1;
		 }
		 else if (maxrateTemp <= oneChnnMaxRate)
		 {

		 }
		 else
		 {
			 maxrateTemp = oneChnnMaxRate;
		 }

		 if (maxrateTemp < maxRate)
		 {
			 maxRate = maxrateTemp;	
		 }
		 printf("%s,%d,m_secondLimitAllRateForFirstStreamNum:%d,m_secondSendAllNum:%d,maxRate:%d,\n",
			   __FILE__,__LINE__,m_secondLimitAllRateForFirstStreamNum,m_secondSendAllNum,maxRate);
	 }


	unsigned long rate = maxRate;

	if(num > 0)
	{
		rate = totalRate / num;
	}

	rate = (rate > maxRate) ? maxRate : rate;


#ifdef __TD2316SES_SPEC__
	{
		PRODUCT_TYPE produtType = CProduct::Instance()->ProductType();
		if ((produtType == TD_2316SE_S) || (produtType == TD_2316CDS))
		{
			if (rate > 3)
			{
				rate = 3;
			}
		}
	}
#endif

#ifdef __CUSTOM_ZNV__
	unsigned long chnnMaxRate;
	if (VIDEO_FORMAT_PAL == g_video_format)
	{
		chnnMaxRate = CProduct::Instance()->NetMaxChannelRatePal();
	}
	else
	{
		chnnMaxRate = CProduct::Instance()->NetMaxChannelRateNtsc();
	}
#endif

	for (unsigned char i=0; i<m_localVideoInputNum; ++i)
	{
			if (m_secondStreamCH_Mobile & ((ULONGLONG)(0x01) << i))
			{
				//手机流单路未3帧,多路手机则每路1帧。
				m_pLocalDevice->SetFrameRateQuality(i, (1 == mobileNum) ? 3 : 1 , \
					static_cast<VIDEO_ENCODE_MODE>(m_pSecondStreamEncodeInfo[i].encodeType), \
					m_pSecondStreamEncodeInfo[i].hBitStream, m_pSecondStreamEncodeInfo[i].quality, false);
			}
			else if (m_secondStreamCH & ((ULONGLONG)(0x01) << i))
			{
				//if (!CProduct::Instance()->GetIsSupportSubCode())    //xian

				m_pLocalDevice->SetFrameRateQuality(i, RealRate(rate, static_cast<VIDEO_SIZE>(m_pSecondStreamEncodeInfo[i].resolution)), \
					static_cast<VIDEO_ENCODE_MODE>(m_pSecondStreamEncodeInfo[i].encodeType), \
					m_pSecondStreamEncodeInfo[i].hBitStream, m_pSecondStreamEncodeInfo[i].quality, false);
			}
			else
			{
#ifdef __CUSTOM_ZNV__

				m_pLocalDevice->SetFrameRateQuality(i, chnnMaxRate, \
					static_cast<VIDEO_ENCODE_MODE>(m_pSecondStreamEncodeInfo[i].encodeType), \
					m_pSecondStreamEncodeInfo[i].hBitStream, m_pSecondStreamEncodeInfo[i].quality, false);
				printf("__________%s_________%d_________%d \n",__FUNCTION__,__LINE__,chnnMaxRate);
#else
				m_pLocalDevice->SetFrameRateQuality(i, 1, \
					static_cast<VIDEO_ENCODE_MODE>(m_pSecondStreamEncodeInfo[i].encodeType), \
					m_pSecondStreamEncodeInfo[i].hBitStream, m_pSecondStreamEncodeInfo[i].quality, false);
				printf("__________%s_________%d_________%d \n",__FUNCTION__,__LINE__,1);
#endif
		}
	}
}


LONGLONG CStreamProc::GetNetRelativeTime(LONGLONG & lgmTime, LONGLONG & lrTime, LONGLONG ngmTime)
{
	LONGLONG nRtime = ngmTime - lgmTime + lrTime;
	if (0 >= nRtime)
	{
		lgmTime = GetCurrTime();

#ifdef __ENVIRONMENT_LINUX__
		lrTime = GetTickCount64();
#else
		lrTime = GetTickCount() * 1000;
#endif

		nRtime = lrTime;
	}

	return nRtime;
}

////zxx新加hxht语音对讲
bool CStreamProc::StartHxhtTalk()
{
	if(m_haveHxht)	//对讲独占
	{
		return false;
	}


	if(!m_pLocalDevice->TalkWayStart())
	{
		return false;
	}

	m_bHxhtWouldStop = false;
	m_haveHxhtID = PUB_CreateThread(TalkHxhtThread, (void *)this, &m_haveHxht);
	if(PUB_CREATE_THREAD_FAIL == m_haveHxhtID)
	{
		printf("Initial talk stream thread fail!\n");
		return false;
	}
	return true;
}
//
RESULT WINAPI CStreamProc::TalkHxhtThread(LPVOID lpParameter)
{
#ifdef __ENVIRONMENT_LINUX__
	pid_t pid = getpid();
	pid_t tid = syscall(__NR_gettid);
	printf("%s, %d, pid = %d, tid=%d\n", __FILE__, __LINE__, pid, tid);
#endif

	CStreamProc *pThis = (CStreamProc *)lpParameter;
	pThis->TalkHxhtProc();

	return 0;
}

void CStreamProc::TalkHxhtProc() 
{
	FRAME_INFO_EX clientFrameInfo;
	FRAME_INFO_EX localFrameInfo;

	memset(&clientFrameInfo, 0, sizeof(FRAME_INFO_EX));
	clientFrameInfo.frameType = FRAME_TYPE_TALK_AUDIO;
	clientFrameInfo.length = 840;

	while(m_haveHxht)
	{
		m_talkHxhtLock.Lock();
		if(m_bHxhtWouldStop)
		{
			m_talkHxhtLock.UnLock();
			break;
		}

		//以下为播放客户端发过来的对讲数据
		while(CLocalDevice::GET_FRAME_SUCC == m_pLocalDevice->GetTalkWay(localFrameInfo))
		{
			m_pLocalDevice->ReleaseTalkWayBuf(localFrameInfo);
		}

		if(NULL == clientFrameInfo.pData)
		{
			m_pLocalDevice->TalkWayGetBuf(&clientFrameInfo);
			
		}

		if(NULL != clientFrameInfo.pData)
		{
			if(m_pMsgMan->GetHxhtTalkWayFrame(clientFrameInfo))
			{
				m_pLocalDevice->TalkWaySetBuf(&clientFrameInfo);
				clientFrameInfo.pData = NULL;
				m_talkHxhtLock.UnLock();
			}
			else
			{
				m_talkHxhtLock.UnLock();
				PUB_Sleep(50);
			}
		}
		else
		{
			m_talkHxhtLock.UnLock();
			PUB_Sleep(10);
		}
	}
}
void CStreamProc::WouldStopHxhtTalk()
{
	m_talkStreamLock.Lock();
	m_bHxhtWouldStop = true;
	m_talkStreamLock.UnLock();
}

void CStreamProc::StopHxhtTalk()
{
	if(m_haveHxht)
	{
		if(PUB_THREAD_ID_NOINIT != m_haveHxhtID)
		{
			PUB_ExitThread(&m_haveHxhtID, &m_haveHxht);
		}
		m_pLocalDevice->TalkWayStop();
		m_pLocalDevice->RecoverAudioLiveChannel();
	}
}
//end
