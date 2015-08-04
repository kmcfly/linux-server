// PlaybackManager.cpp: implementation of the CPlaybackManager class.
//
//////////////////////////////////////////////////////////////////////

#include "PlaybackManager.h"
#include "LocalDevice.h"
#include "MessageMan.h"

#ifdef WIN32

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#endif

extern VIDEO_FORMAT g_video_format;

#define MAX_GET_BUF_NUM 5000

//#ifndef __CHIP3531__
#ifndef __NEW_PLAYBACK__
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CPlaybackManager::CPlaybackManager()
{
	m_pFileName			= NULL;
	m_playThread		= PUB_THREAD_ID_NOINIT;
	memset(&m_frameInfo, 0, sizeof(FRAME_INFO_EX));

	m_prePlayChannels	= 0;

	m_audioChannel		= INVALID_INDEX(unsigned long);

	m_evtReadOnce.CreateWaitEvent();

	m_bRun = false;
}

CPlaybackManager::~CPlaybackManager()
{
	m_evtReadOnce.Close();
}

bool CPlaybackManager::Initial(unsigned char chnnNum, unsigned char videoOutputNum, unsigned char maxFFSpeed, const char *pFilePath, CReclogManEX *pReclogManEx, unsigned long diskOwnerIndex)
{
	//assert ((0 < chnnNum) && (chnnNum <= 32));
	m_chnnNum = chnnNum;
	m_videoOutputNum = videoOutputNum;
	m_maxFFSpeed = maxFFSpeed;
	m_activeChnnNum = 0;
	m_localChnnNum = CProduct::Instance()->LocalVideoInputNum();

	m_reader.Initial(CProduct::Instance()->VideoOutputNum(), pReclogManEx, diskOwnerIndex);

	m_nPlayStatus = N_PS_STOP;

	if(pFilePath != NULL)
	{
		m_pFileName = new char [256];
		strcpy(m_pFileName, pFilePath);
	}

	m_playRate[0] = 1.0/32;
	for(int i = 1; i < sizeof(m_playRate)/sizeof(m_playRate[0]); i++)
	{
		m_playRate[i] = m_playRate[i-1] * 2;
		printf("####%s,%d,m_playRate[%d] = %f\n",__FILE__,__LINE__,i,m_playRate[i]);
	}

// #if (defined(__CHIP3520A__) || defined(__CHIP3531__)) && defined(__DVR_ULTIMATE__)
// 	//3520A�Ĳ�Ʒ�������ܲ�����Ҫ���ط�����
// 	m_playRate[PLAY_SPEED_1X] = 0.96;
// #endif


	if (TD_2704HD == CProduct::Instance()->ProductType())
	{
		if (g_video_format == VIDEO_FORMAT_NTSC)
		{
			m_playRate[PLAY_SPEED_1X] = 0.9;
			printf("####%s,%d,VIDEO_FORMAT_NTSCm_playRate[%d] = %f\n",__FILE__,__LINE__,PLAY_SPEED_1X,m_playRate[PLAY_SPEED_1X]);
		}
	}	

	m_audioChannel = INVALID_INDEX(unsigned long);

	m_bRun = false;

	return true;
}

void CPlaybackManager::Quit()
{
	m_reader.Quit();

	if(m_pFileName != NULL)
	{
		delete [] m_pFileName;
	}

	m_audioChannel = INVALID_INDEX(unsigned long);
}

bool CPlaybackManager::Play(FILE_INFO_EX_LIST *pFileList, unsigned long curTime, 
							MESSAGE_CALLBACK callBack, void *pCallBackObj,
							ULONGLONG &outCH)
{
	assert(m_nPlayStatus == N_PS_STOP);
	m_bPlayNext = false;
	m_bPlayPrev = false;
	m_playSpeed = PLAY_SPEED_1X;
	m_frameSpeedStatus = 1;
	m_forwardType = FF_TYPE_SMOOTHLY;
	m_speedCtrl.bFirstFrame = true;
	m_pMsgCallBack = callBack;
	m_pCallBackObj = pCallBackObj;
	m_bRun = false;

	//������ȡ
	if(m_reader.StartRead(pFileList, outCH, m_pFileName) == MR_RET_OK)
	{
		//��λ��ʱ��curTime
		if (MR_RET_OK == m_reader.SetCurTime(true, curTime, outCH))
		{
			ULONGLONG ullTemp = (outCH & ((((ULONGLONG)0x01) << m_localChnnNum)-1)) << (m_chnnNum-m_localChnnNum);
			ullTemp |= (outCH & ~((((ULONGLONG)0x01) << m_localChnnNum)-1)) >> m_localChnnNum;
			//�����豸
			if (CLocalDevice::Instance()->PlaybackStart(ullTemp))
			{
				m_nPlayStatus = N_PS_PLAY;
				m_playThread = PUB_CreateThread(PlayProc, this, NULL);

				if (PUB_CREATE_THREAD_FAIL != m_playThread)
				{
					m_prePlayChannels = ullTemp;

					m_activeChnnNum = 0;
					ULONGLONG chnBits = ullTemp;
					while ((0 != chnBits) && (m_activeChnnNum < m_videoOutputNum))
					{
						if (chnBits & 0x01)
						{
							++ m_activeChnnNum;
						}

						chnBits = (chnBits >> 1);
					}

					return true;
				}
			}
		}

		//û�ж�λ�ɹ�����û�������豸������Ҫֹͣ��ȡ
		m_reader.StopRead();
	}

	return false;
}

bool CPlaybackManager::Play(unsigned long startTime, unsigned long endTime, unsigned long curTime,
							ULONGLONG chnnBits,MESSAGE_CALLBACK callBack, void *pCallBackObj, 
							ULONGLONG &outCH)
{
	assert(startTime == curTime);
	m_bPlayNext = false;
	m_bPlayPrev = false;
	m_playSpeed = PLAY_SPEED_1X;
	m_frameSpeedStatus = 1;
	m_forwardType = FF_TYPE_SMOOTHLY;
	m_speedCtrl.bFirstFrame = true;
	m_pMsgCallBack = callBack;
	m_pCallBackObj = pCallBackObj;
	m_bRun = false;

	m_channelList.RemoveAll();
	for (unsigned long chnn = 0; chnn < m_chnnNum; ++chnn)
	{
		if (chnnBits & (ULONGLONG)0x01 << chnn)
		{
			m_channelList.AddTail(chnn);
		}
	}

	if(MR_RET_OK == m_reader.StartRead(startTime, endTime, m_channelList, outCH, m_pFileName))
	{
		ULONGLONG ullTemp = (outCH & ((((ULONGLONG)0x01) << m_localChnnNum)-1)) << (m_chnnNum-m_localChnnNum);
		ullTemp |= (outCH & ~((((ULONGLONG)0x01) << m_localChnnNum)-1)) >> m_localChnnNum;
		//�����豸
		if (CLocalDevice::Instance()->PlaybackStart(ullTemp))
		{
			m_nPlayStatus = N_PS_PLAY;
			m_playThread = PUB_CreateThread(PlayProc, this, NULL);

			if (PUB_CREATE_THREAD_FAIL != m_playThread)
			{
				m_prePlayChannels = ullTemp;

				m_activeChnnNum = 0;
				ULONGLONG chnBits = ullTemp;
				while ((0 != chnBits) && (m_activeChnnNum < m_videoOutputNum))
				{
					if (chnBits & 0x01)
					{
						++ m_activeChnnNum;
					}

					chnBits = (chnBits >> 1);
				}

				return true;
			}
		}
	}

	return false;
}

void CPlaybackManager::Run()
{
	m_playLock.Lock();
	m_bRun = true;
	m_playLock.Unlock();
}

void CPlaybackManager::Stop()
{
	m_playLock.Lock();

	if(m_nPlayStatus != N_PS_STOP)
	{
		m_nPlayStatus	= N_PS_STOP;
		m_bPlayNext		= false;
		m_bPlayPrev		= false;		

		//�Ƚ��������򲥷��߳��˲�������
		m_playLock.Unlock();
		PUB_ExitThread(&m_playThread, NULL);

		//�ٴ���ס������Ϊ���滹�н�����������Ҫ��Ӧ��
		//ͬ�������Է�ֹ���ڼ��ⲿ�ٴ��������Ż�������������
		m_playLock.Lock();

		CLocalDevice::Instance()->PlaybackStop();

		m_reader.StopRead();

		m_bSetTimeFinish = true;

		m_prePlayChannels = 0;

		m_channelList.RemoveAll();

		m_bRun = false;
	}

	m_playLock.Unlock();
}

void CPlaybackManager::Pause()
{
	m_playLock.Lock();

	if(m_nPlayStatus != N_PS_STOP)
	{
		m_nPlayStatus = N_PS_PAUSE;
		CLocalDevice::Instance()->PlaybackPause(true);
	}

	m_playLock.Unlock();
}

void CPlaybackManager::Resume()
{
	m_playLock.Lock();

	if(m_nPlayStatus != N_PS_STOP)
	{
		m_speedCtrl.bFirstFrame = true;
		m_nPlayStatus = N_PS_PLAY;

		CLocalDevice::Instance()->PlaybackPause(false);

		m_playSpeed = PLAY_SPEED_1X;

		m_frameSpeedStatus = 1;
		m_forwardType = FF_TYPE_SMOOTHLY;
	}
	m_playLock.Unlock();
}

bool CPlaybackManager::SetCurTime(unsigned long curTime, ULONGLONG & outCH)
{
	m_playLock.Lock();

	if(m_nPlayStatus == N_PS_PLAY)
	{
		m_nPlayStatus = N_PS_CHANGING_TIME;
		m_speedCtrl.bFirstFrame = true;

		m_playLock.Unlock();
		m_evtReadOnce.WaitForCond();
		m_playLock.Lock();

		if(MR_RET_OK == m_reader.SetCurTime((m_nPlayStatus != N_PS_REW), curTime, outCH))
		{
			m_nPlayStatus = N_PS_PLAY;
			m_playSpeed = PLAY_SPEED_1X; //����״̬�޸�Ϊ����������ô�����ٶ�ҲҪ�޸ĳ�1����
		}
		else
		{
			m_playLock.Unlock();
			return false;
		}
	}
	else if(m_nPlayStatus == N_PS_PAUSE)
	{
		m_nPlayStatus = N_PS_CHANGING_TIME;
		m_speedCtrl.bFirstFrame = true;
		if(MR_RET_OK != m_reader.SetCurTime((m_nPlayStatus != N_PS_REW), curTime, outCH))
		{
			m_playLock.Unlock();
			return false;
		}
		else
		{
			m_nPlayStatus = N_PS_PLAY;
			m_playSpeed = PLAY_SPEED_1X;

			CLocalDevice::Instance()->PlaybackPause(true);
		}
	}
	else
	{
		//do nothing
	}

	m_activeChnnNum = 0;
	ULONGLONG chnBits = outCH;
	while ((0 != chnBits) && (m_activeChnnNum < m_videoOutputNum))
	{
		if (chnBits & 0x01)
		{
			++ m_activeChnnNum;
		}

		chnBits = (chnBits >> 1);
	}

	m_frameSpeedStatus = 1;
	m_forwardType = FF_TYPE_SMOOTHLY;

	m_bSetTimeFinish = false;
	m_playLock.Unlock();

	return true;
}

bool CPlaybackManager::SetCurChannelMask(ULONGLONG ullWindowMask,  ULONGLONG & outCH)
{
	m_playLock.Lock();
	if(m_nPlayStatus == N_PS_PLAY)
	{
		m_nPlayStatus = N_PS_CHANGING_TIME;
		m_speedCtrl.bFirstFrame = true;

		m_playLock.Unlock();
		m_evtReadOnce.WaitForCond();
		m_playLock.Lock();

		if (MR_RET_OK != m_reader.SetCurChannel(ullWindowMask, outCH))
		{
			m_playLock.Unlock();
			return false;
		}
		else
		{
			ULONGLONG ullTemp = (outCH & ((((ULONGLONG)0x01) << m_localChnnNum)-1)) << (m_chnnNum-m_localChnnNum);
			ullTemp |= (outCH & ~((((ULONGLONG)0x01) << m_localChnnNum)-1)) >> m_localChnnNum;
			ULONGLONG stopCH = m_prePlayChannels & ~ullTemp;
			//��ֹͣ�豸
			//CLocalDevice::Instance()->PlaybackStop(stopCH);

			//�ٿ����豸
			CLocalDevice::Instance()->PlaybackStart(ullTemp,true);

			m_prePlayChannels = ullTemp;

			m_nPlayStatus = N_PS_PLAY;

			m_speedCtrl.bFirstFrame = true;
			m_playSpeed = PLAY_SPEED_1X;
		}
	}
	else if(m_nPlayStatus == N_PS_PAUSE)
	{
		m_nPlayStatus = N_PS_CHANGING_TIME;
		m_speedCtrl.bFirstFrame = true;

		if (MR_RET_OK != m_reader.SetCurChannel(ullWindowMask, outCH))
		{
			m_playLock.Unlock();
			return false;
		}
		else
		{
			ULONGLONG ullTemp = (outCH & ((((ULONGLONG)0x01) << m_localChnnNum)-1)) << (m_chnnNum-m_localChnnNum);
			ullTemp |= (outCH & ~((((ULONGLONG)0x01) << m_localChnnNum)-1)) >> m_localChnnNum;
			ULONGLONG stopCH = m_prePlayChannels & ~ullTemp;
			//��ֹͣ�豸
			//CLocalDevice::Instance()->PlaybackStop(stopCH);

			//�ٿ����豸
			CLocalDevice::Instance()->PlaybackStart(ullTemp, true);

			m_prePlayChannels |= ullTemp;

			m_nPlayStatus = N_PS_PAUSE;

			CLocalDevice::Instance()->PlaybackPause(true);

			m_speedCtrl.bFirstFrame = true;
		}
	}
	else
	{
		//do nothing
		ULONGLONG outCH = 0;
		m_reader.SetCurChannel(ullWindowMask, outCH);
	}

	m_bSetTimeFinish = false;

	m_activeChnnNum = 0;
	ULONGLONG chnBits = outCH;
	while ((0 != chnBits) && (m_activeChnnNum < m_videoOutputNum))
	{
		if (chnBits & 0x01)
		{
			++ m_activeChnnNum;
		}

		chnBits = (chnBits >> 1);
	}

	m_frameSpeedStatus = 1;
	m_forwardType = FF_TYPE_SMOOTHLY;

	m_playLock.Unlock();

	return true;
}

void CPlaybackManager::SetPlaySpeed(bool bForward, PLAY_SPEED playSpeed)
{
	m_playLock.Lock();

	if((m_nPlayStatus == N_PS_STOP) || (m_nPlayStatus == N_PS_PAUSE))
	{
		//�ط���ֹͣ,�������κβ���
	}
	else
	{
		m_playSpeed = playSpeed;

		if(!bForward)
		{
			m_nPlayStatus = N_PS_REW;
			assert(playSpeed >= PLAY_SPEED_8X);
		}
		else if(playSpeed != PLAY_SPEED_1X)
		{
			assert(playSpeed <= PLAY_SPEED_16X);
			m_nPlayStatus = N_PS_FF;

			switch(playSpeed)
			{
			case PLAY_SPEED_2X:
				m_frameSpeedStatus = 2;
				break;
			case PLAY_SPEED_4X:
				m_frameSpeedStatus = 4;
				break;
			case PLAY_SPEED_8X:
				m_frameSpeedStatus = 8;
				break;
			case PLAY_SPEED_16X:
				m_frameSpeedStatus = 16;
				break;
			default:
				m_frameSpeedStatus = 1;
				break;
			}

			//�����128����ʾ��Ʒ�������Լ����
			//���ǲ���Ĭ�ϣ����ܼ�·�طţ����ֻ������4������֡���롣
			if (128 == m_maxFFSpeed)
			{
				if (m_frameSpeedStatus <= 4)
				{
					m_forwardType = FF_TYPE_SMOOTHLY;	

					if (TD_2704HD == CProduct::Instance()->ProductType())
					{
						m_forwardType = FF_TYPE_JUMPING;
					}					

				}
				else
				{
					m_forwardType = FF_TYPE_JUMPING;
				}

			}
			else
			{
				if (m_playSpeed <= PLAY_SPEED_1_2X)
				{
					m_forwardType = FF_TYPE_SMOOTHLY;
				}
				else
				{
					if (m_maxFFSpeed < (m_frameSpeedStatus * m_activeChnnNum))
					{
						m_forwardType = FF_TYPE_JUMPING;
					}
					else
					{
						m_forwardType = FF_TYPE_SMOOTHLY;
					}

					if (TD_2704HD == CProduct::Instance()->ProductType())
					{
						m_forwardType = FF_TYPE_JUMPING;
					}

					//23XXSS-B��Ʒ��CVBS���ʱ�ط�1·�����֡�����ٶ�Ϊ2����
					PRODUCT_TYPE product = CProduct::Instance()->ProductType();
					if ((TD_2304SS_B == product) || (TD_2308SS_B == product) || (TD_2316SS_B == product) || 
						(TD_2304SS_C == product) || (TD_2308SS_C == product) || (TD_2316SS_C == product) ||
						(TD_2304SL_C == product) || (TD_2308SL_C == product))
					{
						if ((VIDEO_OUT_CVBS == CLocalDevice::Instance()->GetCurMainDevice()) && (m_maxFFSpeed <= (m_frameSpeedStatus * m_activeChnnNum)))
						{
							m_forwardType = FF_TYPE_JUMPING;
						}
					}

				}	
			}
		}
		else
		{
			m_nPlayStatus = N_PS_PLAY;	
			m_frameSpeedStatus = 1;
			m_forwardType = FF_TYPE_SMOOTHLY;
			m_playSpeed = PLAY_SPEED_1X;
		}

		m_speedCtrl.bFirstFrame = true;
	}

	m_playLock.Unlock();
}

void CPlaybackManager::PlayFF()
{
	m_playLock.Lock();
	if((m_nPlayStatus != N_PS_STOP) && (m_nPlayStatus != N_PS_PAUSE))
	{
		if(m_nPlayStatus != N_PS_FF)
		{
			m_nPlayStatus = N_PS_FF;
			m_playSpeed = PLAY_SPEED_2X;
		}
		else
		{
			if(m_playSpeed < PLAY_SPEED_16X)
			{
				char temp = m_playSpeed;
				++ temp;
				m_playSpeed = (PLAY_SPEED)temp;
				if(m_playSpeed == PLAY_SPEED_1X)
				{
					m_nPlayStatus = N_PS_PLAY;
				}
			}
			else
			{
				m_playSpeed = PLAY_SPEED_1X;
				m_nPlayStatus = N_PS_PLAY;
			}

			m_speedCtrl.bFirstFrame = true;
		}

		switch(m_playSpeed)
		{
		case PLAY_SPEED_2X:
			m_frameSpeedStatus = 2;
			break;
		case PLAY_SPEED_4X:
			m_frameSpeedStatus = 4;
			break;
		case PLAY_SPEED_8X:
			m_frameSpeedStatus = 8;
			break;
		case PLAY_SPEED_16X:
			m_frameSpeedStatus = 16;
			break;
		default:
			m_frameSpeedStatus = 1;
			break;
		}

		//�����128����ʾ��Ʒ�������Լ����
		//���ǲ���Ĭ�ϣ����ܼ�·�طţ����ֻ������4������֡���롣
		if (128 == m_maxFFSpeed)
		{
			if (m_frameSpeedStatus <= 4)
			{
				m_forwardType = FF_TYPE_SMOOTHLY;

				if (TD_2704HD == CProduct::Instance()->ProductType())
				{
					m_forwardType = FF_TYPE_JUMPING;
				}
			}
			else
			{
				m_forwardType = FF_TYPE_JUMPING;
			}
		}
		else
		{
			if (m_playSpeed <= PLAY_SPEED_1_2X)
			{
				m_forwardType = FF_TYPE_SMOOTHLY;
			}
			else
			{
				if (m_maxFFSpeed < (m_frameSpeedStatus * m_activeChnnNum))
				{
					m_forwardType = FF_TYPE_JUMPING;
				}
				else
				{
					m_forwardType = FF_TYPE_SMOOTHLY;
				}
				if (TD_2704HD == CProduct::Instance()->ProductType())
				{
					m_forwardType = FF_TYPE_JUMPING;
				}

				//23XXSS-B��Ʒ��CVBS���ʱ�ط�1·�����֡�����ٶ�Ϊ2����
				PRODUCT_TYPE product = CProduct::Instance()->ProductType();
				if ((TD_2304SS_B == product) || (TD_2308SS_B == product) || (TD_2316SS_B == product) || 
					(TD_2304SS_C == product) || (TD_2308SS_C == product) || (TD_2316SS_C == product) ||
					(TD_2304SL_C == product) || (TD_2308SL_C == product))
				{
					if ((VIDEO_OUT_CVBS == CLocalDevice::Instance()->GetCurMainDevice()) && (m_maxFFSpeed <= (m_frameSpeedStatus * m_activeChnnNum)))
					{
						m_forwardType = FF_TYPE_JUMPING;
					}
				}

			}			
		}
	}

	m_playLock.Unlock();
}

void CPlaybackManager::PlayRew()
{
	m_playLock.Lock();
	if((m_nPlayStatus != N_PS_STOP) && (m_nPlayStatus != N_PS_PAUSE))
	{
		if(m_nPlayStatus != N_PS_REW)
		{
			//����,����8����
			m_nPlayStatus = N_PS_REW;
			m_playSpeed = PLAY_SPEED_8X;
		}
		else
		{
			if(m_playSpeed < PLAY_SPEED_16X)
			{
				char temp = m_playSpeed;
				++temp;
				m_playSpeed = (PLAY_SPEED)temp;
			}
			else	//�ﵽ�������ٶȣ�������������
			{
				m_playSpeed = PLAY_SPEED_1X;
				m_nPlayStatus = N_PS_PLAY;
			}

			m_speedCtrl.bFirstFrame = true;
		}

		m_frameSpeedStatus = 1;
		m_forwardType = FF_TYPE_SMOOTHLY;
	}

	m_playLock.Unlock();
}

void CPlaybackManager::PlayNextFrame(bool bForward)
{
	assert (N_PS_PAUSE == m_nPlayStatus);

	if(N_PS_PAUSE == m_nPlayStatus)
	{
		if(bForward)
		{
			m_bPlayNext = true;
		}
		else
		{
			m_bPlayPrev = true;
		}
	}
}

void CPlaybackManager::SetAudioChannel(unsigned long channel)
{
	m_audioChannel = (channel >= m_localChnnNum) ? (channel - m_localChnnNum) : (channel + (m_chnnNum - m_localChnnNum));
	printf("%s,%d,chnn:%d\n", __FILE__, __LINE__, m_audioChannel);
}



RESULT WINAPI CPlaybackManager::PlayProc(void *pParam)
{
#ifdef __ENVIRONMENT_LINUX__
	pid_t pid = getpid();
	pid_t tid = syscall(__NR_gettid);
	printf("%s, %d, pid = %d, tid=%d\n", __FILE__, __LINE__, pid, tid);
#endif

	CPlaybackManager *pThis = (CPlaybackManager *)pParam;
	pThis->PlayProc();

	return 0;
}

void CPlaybackManager::PlayProc()
{
	memset(&m_frameInfo, 0, sizeof(FRAME_INFO_EX));

	LONGLONG lastShowTime = 0;
	unsigned long MultiCH = 0, chnn = 0;
	unsigned long frameTypeTemp = FRAME_TYPE_NONE;
	unsigned long lastFrameHeight = 0;
	unsigned long lastFrameWidth = 0;

	while(true)
	{
		m_playLock.Lock();

		if (!m_bRun)
		{		
			m_playLock.Unlock();
#if defined(__TD2316SES_SPEC__) || defined(__TDFH__)
			PUB_Sleep(100);
#else
			PUB_Sleep(10);
#endif
			continue;
		}

		if(m_nPlayStatus == N_PS_STOP)
		{
			m_playLock.Unlock();
			m_evtReadOnce.SetEvent();
			break;
		}

		if(m_nPlayStatus == N_PS_CHANGING_TIME)
		{
			m_playLock.Unlock();
			m_evtReadOnce.SetEvent();
			PUB_Sleep(10);
			continue;
		}

		if(m_nPlayStatus == N_PS_PAUSE)
		{
			if(!m_bPlayNext && !m_bPlayPrev && m_bSetTimeFinish)		//���û�а���һ֡��ť
			{
				m_playLock.Unlock();
				m_evtReadOnce.SetEvent();
				PUB_Sleep(200);
				continue;
			}
		}

		unsigned long time = 0;
		memset(&m_frameInfo, 0, sizeof(m_frameInfo));

		//��ȡ��һ֡��֡����;
		long ret;
		if((m_nPlayStatus == N_PS_REW) && (m_playSpeed >= PLAY_SPEED_8X))
		{
			ret = m_reader.GetNextFrameType(m_frameInfo, false);
		}
		else
		{
			ret = m_reader.GetNextFrameType(m_frameInfo, true);
		}

		if(MR_RET_OK != ret)
		{
			m_pMsgCallBack(m_pCallBackObj, 2, NULL, NULL);

			printf("%s:%s:%d, no data for read\n", __FUNCTION__, __FILE__, __LINE__);
			m_playLock.Unlock();
			m_evtReadOnce.SetEvent();
			break;
		}

		frameTypeTemp = m_frameInfo.frameType;

		{
			//��2010-01-20 15:18:00 YSW��
			//�ѻ�ȡ�ڴ�ͳһ��LocaclDevice�ڲ�ȥ����������Ƶ������Ƶ��ʽ��������һƬ�ڴ�
			//�������Ǵӽ�������ȡ�ڴ��ַ��
			m_frameInfo.channel = CHANNEL_ID(m_frameInfo.channel);
			
			if(m_frameInfo.channel >= m_localChnnNum)
			{
				m_frameInfo.channel -= m_localChnnNum;
			}
			else
			{
				m_frameInfo.channel += (m_chnnNum - m_localChnnNum);
			}

			//ͨ���豸�ӿڻ�ȡ���ݻ��������п��ܻ�ȡ���ɹ������޶��������MAX_GET_BUF_NUM��
			while(!CLocalDevice::Instance()->PlaybackGetBuf(&m_frameInfo) && (time < MAX_GET_BUF_NUM))
			{
				time++;				
				PUB_Sleep(1);
			}			

			//�Ⱥų���������ζ�Ż�ȡ������ʧ�ܣ�����Ҫ�˳��طš�
			if(time == MAX_GET_BUF_NUM)
			{
				m_pMsgCallBack(m_pCallBackObj, 2, NULL, NULL);
				m_playLock.Unlock();
				m_evtReadOnce.SetEvent();

				printf("%s,%d,not get buffer.time:%d\n",__FILE__,__LINE__,time);				

				RUN_LOG(RUN_LOG_ITEM_PB_GET_FRAME_FAIL, CRunLog::RUN_Num("CH:", m_frameInfo.channel), CRunLog::RUN_Num("Frame Type:", m_frameInfo.frameType), "Get Buffer fail");

				break;
			}
		}

		//���ݲ���״̬�����ò�ͬ�Ľӿڶ�ȡ���ݣ�8�������������ؼ�֡��������������������֡���С�
		if((m_nPlayStatus == N_PS_FF) && (FF_TYPE_JUMPING == m_forwardType))
		{
			ret = m_reader.GetKeyFrame(m_frameInfo, 1, true);
		}
		else if((m_nPlayStatus == N_PS_REW) && (m_playSpeed >= PLAY_SPEED_8X))
		{
			ret = m_reader.GetKeyFrame(m_frameInfo, 1, false);
		}
		else if((m_nPlayStatus == N_PS_PLAY) || (m_nPlayStatus == N_PS_FF) || (m_nPlayStatus == N_PS_REW))
		{
			ret = m_reader.GetNextFrame(m_frameInfo);
		}
		else if ((m_nPlayStatus == N_PS_PAUSE) && (m_bPlayNext || m_bPlayPrev || !m_bSetTimeFinish))
		{
			ret = m_reader.GetNextFrame(m_frameInfo);
		}
		else
		{
			RUN_LOG(RUN_LOG_ITEM_PB_GET_FRAME_FAIL, CRunLog::RUN_Num("Play Status:", m_nPlayStatus), CRunLog::RUN_Num("Play Speed:", m_playSpeed), "Get Next Frame fail");
		}

		//�޸����⣺�ڵ���GetNextFrameType����ʱ���õ�����Ƶ֡��������GetNextFrame��
		//GetKeyFrameʱ�õ�������Ƶ֡��
		//����������������Ƶʱ�����8��16,32�������Ŀ������ˡ�
		if (frameTypeTemp != m_frameInfo.frameType)
		{
			printf("#############################################chnn:%d,frameType error,%d,%d. keyframe:%d\n", m_frameInfo.channel, frameTypeTemp, m_frameInfo.frameType, m_frameInfo.keyFrame);
			m_frameInfo.frameType = FRAME_TYPE_NONE;
			m_frameInfo.frameIndex = 1;
			CLocalDevice::Instance()->PlaybackSetBuf(&m_frameInfo);
			m_playLock.Unlock();
			m_evtReadOnce.SetEvent();
			continue;
		}

		//��ȡ���ݳɹ�
		if(MR_RET_OK == ret)
		{
			chnn = CHANNEL_ID(m_frameInfo.channel);//�õ���ͨ���ţ����ֺ�ģ��ͳһ��ŵ�ͨ��
			if(chnn >= m_localChnnNum)
			{
				chnn -= m_localChnnNum;
			}
			else
			{
				chnn += (m_chnnNum - m_localChnnNum);
			}

			//�������Ƶ����Ƶ��ʽ����
			if(m_frameInfo.frameType == FRAME_TYPE_VIDEO_FORMAT)
			{
				m_frameInfo.frameIndex = 1;
				CLocalDevice::Instance()->PlaybackSetBuf(&m_frameInfo);
				m_playLock.Unlock();
				continue;
			}
			else if(m_frameInfo.frameType == FRAME_TYPE_AUDIO_FORMAT)
			{
				m_frameInfo.frameIndex = 1;
				CLocalDevice::Instance()->PlaybackSetBuf(&m_frameInfo);
				m_playLock.Unlock();
				continue;
			}
			else if(m_frameInfo.frameType == FRAME_TYPE_VIDEO)
			{
				m_bPlayNext = false;
				m_bPlayPrev = false;
#if defined(__TDFH__) || defined(__CHIPTI__)
				if ( lastFrameHeight && lastFrameHeight != m_frameInfo.height 
					|| lastFrameWidth && lastFrameWidth != m_frameInfo.width)
				{
					m_pMsgCallBack(m_pCallBackObj, 3, NULL, NULL);
				}

				lastFrameHeight = m_frameInfo.height;
				lastFrameWidth	= m_frameInfo.width;
#endif
				if(!m_bSetTimeFinish)
				{
					m_bSetTimeFinish = true;
				}

#ifdef __ENVIRONMENT_WIN32__
				m_frameInfo.frameAttrib = PLAY_FRAME_SHOW;
#else
				m_frameInfo.frameAttrib = PLAY_FRAME_NO_SHOW;
#endif

				if(m_speedCtrl.bFirstFrame)
				{
					m_speedCtrl.bFirstFrame = false;
					m_speedCtrl.lastShowTime = GetCurrTime();
					m_speedCtrl.lastFrameTime = m_frameInfo.time;
					MultiCH = 0x01 << chnn;
				}
				else if(0 != (MultiCH & (0x01 << chnn)))
				{
					LONGLONG diffTime = 0;
					if(m_nPlayStatus == N_PS_REW)
					{
						diffTime = static_cast<LONGLONG>((m_speedCtrl.lastFrameTime - m_frameInfo.time) / m_playRate[m_playSpeed]);
					}
					else
					{
						diffTime = static_cast<LONGLONG>((m_frameInfo.time - m_speedCtrl.lastFrameTime) / m_playRate[m_playSpeed]);
					}

					if ((0 < diffTime) && (diffTime < 3000000))
					{
						m_speedCtrl.lastShowTime += diffTime;

						while (GetCurrTime() < m_speedCtrl.lastShowTime)
						{
							PUB_Sleep(1);
						}
					}

					m_frameInfo.frameAttrib = PLAY_FRAME_SHOW;

					if (m_frameInfo.time > lastShowTime)						
					{
						if (m_frameInfo.time >= (lastShowTime + 1000000))
						{
							m_pMsgCallBack(m_pCallBackObj, 1, &m_frameInfo.time, NULL);
							lastShowTime = m_frameInfo.time;
						}
					}
					else
					{
						if (lastShowTime >= (m_frameInfo.time + 1000000))
						{
							m_pMsgCallBack(m_pCallBackObj, 1, &m_frameInfo.time, NULL);
							lastShowTime = m_frameInfo.time;
						}
					}

					m_speedCtrl.lastFrameTime = m_frameInfo.time;
					MultiCH = 0x01 << chnn;
				}
				else
				{
					MultiCH |= (0x01 << chnn);
				}

				m_frameInfo.frameIndex = m_frameSpeedStatus;
				m_frameInfo.frameIndex =(m_frameInfo.frameIndex << 16);
				m_frameInfo.frameIndex |= (FF_TYPE_JUMPING == m_forwardType) ? 1 : m_frameSpeedStatus;
				m_playLock.Unlock();

				CLocalDevice::Instance()->PlaybackSetBuf(&m_frameInfo);
			}
			else if(m_frameInfo.frameType == FRAME_TYPE_AUDIO)
			{
				if ((m_nPlayStatus != N_PS_PLAY) || (m_audioChannel != chnn))
				{
					//����ÿ�ζ�ȡ����֮ǰ�Ѿ���ȡ���˻��������Ҳ���֪����������֡�����ͣ�����
					//������ǵ�ǰ�طŵ�ͨ��������Ҫ���й��ˡ�
					m_frameInfo.frameType = FRAME_TYPE_NONE;
				}

				m_frameInfo.frameIndex = 1;
				CLocalDevice::Instance()->PlaybackSetBuf(&m_frameInfo);
				m_playLock.Unlock();

			}
			else
			{
				m_frameInfo.frameIndex = 1;
				CLocalDevice::Instance()->PlaybackSetBuf(&m_frameInfo);
				m_playLock.Unlock();				
			} 			
		}
		else
		{
			//û�����ݿɻطţ�����طŽ���
			m_frameInfo.frameIndex = 1;
			CLocalDevice::Instance()->PlaybackSetBuf(&m_frameInfo);

			m_pMsgCallBack(m_pCallBackObj, 2, NULL, NULL);
			printf("%s:%s:%d, no data for read\n", __FUNCTION__, __FILE__, __LINE__);
			m_playLock.Unlock();
			m_evtReadOnce.SetEvent();

			break;
		}

		m_evtReadOnce.SetEvent();
	}

#ifdef __ENVIRONMENT_LINUX__
	RUN_LOG(RUN_LOG_ITEM_PB_EXIT, {}, {}, "Exit Playback Proc");
#endif
}

#else //__CHIP3531__
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CPlaybackManager::CPlaybackManager()
{
	m_pFileName			= NULL;
	m_playThread		= PUB_THREAD_ID_NOINIT;
	memset(&m_frameInfo, 0, sizeof(FRAME_INFO_EX));

	m_prePlayChannels	= 0;
    
	m_audioChannel		= INVALID_INDEX(unsigned long);

	m_bRun = false;
	m_bFrameReadFromFile = false;

	memset( m_playImageChannWidth, 0, sizeof(int)*PLAYBACK_SPS_MAX_CHN_NUM);
	memset( m_playImageChannHeight, 0, sizeof(int)*PLAYBACK_SPS_MAX_CHN_NUM);
	memset( m_playImageChannFrames, 0, sizeof(int)*PLAYBACK_SPS_MAX_CHN_NUM);
	memset( m_playImageChannTmCount, 0, sizeof(int)*PLAYBACK_SPS_MAX_CHN_NUM);
	memset( m_playImageChannPreTms, 0, sizeof(LONGLONG)*PLAYBACK_SPS_MAX_CHN_NUM);

	m_bIsSepcSplit = false;
	m_primaryStreamDisplayIndex = 0;
}

CPlaybackManager::~CPlaybackManager()
{
}

bool CPlaybackManager::Initial(unsigned char chnnNum, unsigned char videoOutputNum, unsigned char maxFFSpeed, const char *pFilePath, CReclogManEX *pReclogManEx, unsigned long diskOwnerIndex)
{
	//assert ((0 < chnnNum) && (chnnNum <= 32));
	m_chnnNum = chnnNum;
	m_videoOutputNum = videoOutputNum;
	m_maxFFSpeed = maxFFSpeed;
	m_activeChnnNum = 0;
	m_localChnnNum = CProduct::Instance()->LocalVideoInputNum();

	m_reader.Initial(CProduct::Instance()->VideoOutputNum(), pReclogManEx, diskOwnerIndex);

	m_nPlayStatus = N_PS_STOP;

	if(pFilePath != NULL)
	{
		m_pFileName = new char [256];
		strcpy(m_pFileName, pFilePath);
	}

	m_playRate[0] = 1.0/32;
	for(int i = 1; i < sizeof(m_playRate)/sizeof(m_playRate[0]); i++)
	{
		m_playRate[i] = m_playRate[i-1] * 2;
		printf("####%s,%d,m_playRate[%d] = %f\n",__FILE__,__LINE__,i,m_playRate[i]);
	}

// #if (defined(__CHIP3520A__) || defined(__CHIP3531__)) && defined(__DVR_ULTIMATE__)
// 	//3520A�Ĳ�Ʒ�������ܲ�����Ҫ���ط�����
// 	m_playRate[PLAY_SPEED_1X] = 0.96;
// #endif


	if (TD_2704HD == CProduct::Instance()->ProductType())
	{
		if (g_video_format == VIDEO_FORMAT_NTSC)
		{
			m_playRate[PLAY_SPEED_1X] = 0.9;
			printf("####%s,%d,VIDEO_FORMAT_NTSCm_playRate[%d] = %f\n",__FILE__,__LINE__,PLAY_SPEED_1X,m_playRate[PLAY_SPEED_1X]);
		}
	}	

	m_audioChannel = INVALID_INDEX(unsigned long);

	m_bRun = false;

	return true;
}

void CPlaybackManager::Quit()
{
	m_reader.Quit();

	if(m_pFileName != NULL)
	{
		delete [] m_pFileName;
	}

	m_audioChannel = INVALID_INDEX(unsigned long);
}

bool CPlaybackManager::Play(FILE_INFO_EX_LIST *pFileList, unsigned long curTime, 
							MESSAGE_CALLBACK callBack, void *pCallBackObj,
							ULONGLONG &outCH)
{
	assert(m_nPlayStatus == N_PS_STOP);
	m_bPlayNext = false;
	m_bPlayPrev = false;
	m_playSpeed = PLAY_SPEED_1X;
	m_frameSpeedStatus = 1;
	m_forwardType = FF_TYPE_SMOOTHLY;
	m_speedCtrl.bFirstFrame = true;
	m_pMsgCallBack = callBack;
	m_pCallBackObj = pCallBackObj;
	m_bRun = false;

	//������ȡ
	if(m_reader.StartRead(pFileList, outCH, m_pFileName) == MR_RET_OK)
	{
		ADAPT_TO_ACTUAL_OUTPUT_ULL_CHNN(outCH);
		//��λ��ʱ��curTime
		if (MR_RET_OK == m_reader.SetCurTime(true, curTime, outCH))
		{
			ADAPT_TO_ACTUAL_OUTPUT_ULL_CHNN(outCH);
			unsigned char chnnNumAdapt = 0;
			ADAPT_TO_ACTUAL_CHNN_NUM(m_chnnNum,chnnNumAdapt);
			
			ULONGLONG ullTemp = (outCH & ((((ULONGLONG)0x01) << m_localChnnNum)-1)) << (chnnNumAdapt-m_localChnnNum);
			ullTemp |= (outCH & ~((((ULONGLONG)0x01) << m_localChnnNum)-1)) >> m_localChnnNum;
			//�����豸
			m_bFrameReadFromFile = false;
			if (CLocalDevice::Instance()->PlaybackStart(ullTemp))
			{
				m_nPlayStatus = N_PS_PLAY;
				m_playThread = PUB_CreateThread(PlayProc, this, NULL);

				if (PUB_CREATE_THREAD_FAIL != m_playThread)
				{
					m_prePlayChannels = ullTemp;

					m_activeChnnNum = 0;
					ULONGLONG chnBits = ullTemp;
					while ((0 != chnBits) && (m_activeChnnNum < m_videoOutputNum))
					{
						if (chnBits & 0x01)
						{
							++ m_activeChnnNum;
						}

						chnBits = (chnBits >> 1);
					}

					return true;
				}
			}
		}
		
		//û�ж�λ�ɹ�����û�������豸������Ҫֹͣ��ȡ
		m_reader.StopRead();
	}

	return false;
}

bool CPlaybackManager::Play(unsigned long startTime, unsigned long endTime, unsigned long curTime,
							ULONGLONG chnnBits,MESSAGE_CALLBACK callBack, void *pCallBackObj, 
							ULONGLONG &outCH)
{
	assert(startTime == curTime);
	m_bPlayNext = false;
	m_bPlayPrev = false;
	m_playSpeed = PLAY_SPEED_1X;
	m_frameSpeedStatus = 1;
	m_forwardType = FF_TYPE_SMOOTHLY;
	m_speedCtrl.bFirstFrame = true;
	m_pMsgCallBack = callBack;
	m_pCallBackObj = pCallBackObj;
	m_bRun = false;

	m_channelList.RemoveAll();
	for (unsigned long chnn = 0; chnn < m_chnnNum; ++chnn)
	{
		if (chnnBits & (ULONGLONG)0x01 << chnn)
		{
			m_channelList.AddTail(chnn);
		}
	}
 
	if(MR_RET_OK == m_reader.StartRead(startTime, endTime, m_channelList, outCH, m_pFileName))
	{
		ADAPT_TO_ACTUAL_OUTPUT_ULL_CHNN(outCH);
		
		unsigned char chnnNumAdapt = 0;
		ADAPT_TO_ACTUAL_CHNN_NUM(m_chnnNum,chnnNumAdapt);
			
		ULONGLONG ullTemp = (outCH & ((((ULONGLONG)0x01) << m_localChnnNum)-1)) << (chnnNumAdapt-m_localChnnNum);
		ullTemp |= (outCH & ~((((ULONGLONG)0x01) << m_localChnnNum)-1)) >> m_localChnnNum;
		//�����豸
		m_bFrameReadFromFile = false;
		if (CLocalDevice::Instance()->PlaybackStart(ullTemp))
		{
			m_nPlayStatus = N_PS_PLAY;
			m_playThread = PUB_CreateThread(PlayProc, this, NULL);

			if (PUB_CREATE_THREAD_FAIL != m_playThread)
			{
				m_prePlayChannels = ullTemp;

				m_activeChnnNum = 0;
				ULONGLONG chnBits = ullTemp;
				while ((0 != chnBits) && (m_activeChnnNum < m_videoOutputNum))
				{
					if (chnBits & 0x01)
					{
						++ m_activeChnnNum;
					}

					chnBits = (chnBits >> 1);
				}

				return true;
			}
			else
			{
				printf("%s:%s:%d, Play back start create thread error\n", __FUNCTION__, __FILE__, __LINE__);
			}
		}
		else
		{
			printf("%s:%s:%d, Play back start error\n", __FUNCTION__, __FILE__, __LINE__);
		}
	}

	return false;
}

void CPlaybackManager::Run()
{
	m_playLock.Lock();
	m_bRun = true;
	memset( m_playImageChannWidth, 0, sizeof(int)*PLAYBACK_SPS_MAX_CHN_NUM);
	memset( m_playImageChannHeight, 0, sizeof(int)*PLAYBACK_SPS_MAX_CHN_NUM);
	memset( m_playImageChannFrames, 0, sizeof(int)*PLAYBACK_SPS_MAX_CHN_NUM);
	memset( m_playImageChannTmCount, 0, sizeof(int)*PLAYBACK_SPS_MAX_CHN_NUM);
	memset( m_playImageChannPreTms, 0, sizeof(LONGLONG)*PLAYBACK_SPS_MAX_CHN_NUM);
	m_bFrameReadFromFile = false;
	m_playLock.UnLock();
}

void CPlaybackManager::Stop()
{
	m_playLock.Lock();

	memset( m_playImageChannWidth, 0, sizeof(int)*PLAYBACK_SPS_MAX_CHN_NUM);
	memset( m_playImageChannHeight, 0, sizeof(int)*PLAYBACK_SPS_MAX_CHN_NUM);
	memset( m_playImageChannFrames, 0, sizeof(int)*PLAYBACK_SPS_MAX_CHN_NUM);
	memset( m_playImageChannTmCount, 0, sizeof(int)*PLAYBACK_SPS_MAX_CHN_NUM);
	memset( m_playImageChannPreTms, 0, sizeof(LONGLONG)*PLAYBACK_SPS_MAX_CHN_NUM);

	if(m_nPlayStatus != N_PS_STOP)
	{
		m_nPlayStatus	= N_PS_STOP;
		m_bPlayNext		= false;
		m_bPlayPrev		= false;		

		//�Ƚ��������򲥷��߳��˲�������
		m_playLock.UnLock();
		PUB_ExitThread(&m_playThread, NULL);

		//�ٴ���ס������Ϊ���滹�н�����������Ҫ��Ӧ��
		//ͬ�������Է�ֹ���ڼ��ⲿ�ٴ��������Ż�������������
		m_playLock.Lock();

		CLocalDevice::Instance()->PlaybackStop();

		m_reader.StopRead();

		m_bSetTimeFinish = true;

		m_prePlayChannels = 0;

		m_channelList.RemoveAll();

		m_bRun = false;
	}

	m_playLock.UnLock();
}

void CPlaybackManager::Pause()
{
	m_playLock.Lock();

	if(m_nPlayStatus != N_PS_STOP)
	{
		m_nPlayStatus = N_PS_PAUSE;
		CLocalDevice::Instance()->PlaybackPause(true);
	}

	m_playLock.UnLock();
}

void CPlaybackManager::Resume()
{
	m_playLock.Lock();

	if(m_nPlayStatus != N_PS_STOP)
	{
		m_speedCtrl.bFirstFrame = true;
		m_nPlayStatus = N_PS_PLAY;

		m_playSpeed = PLAY_SPEED_1X;

		m_frameSpeedStatus = 1;
		m_forwardType = FF_TYPE_SMOOTHLY;

		CLocalDevice::Instance()->PlaybackPause(false);
	}
	m_playLock.UnLock();
}

bool CPlaybackManager::SetCurTime(unsigned long curTime, ULONGLONG & outCH)
{
	m_playLock.Lock();

	if(m_nPlayStatus == N_PS_PLAY)
	{
		m_nPlayStatus = N_PS_CHANGING_TIME;

		if(m_bFrameReadFromFile)
		{
			CLocalDevice::Instance()->PlaybackSetBuf(&m_frameInfo);
			m_bFrameReadFromFile = false;
		}

		m_speedCtrl.bFirstFrame = true;
		
		if(MR_RET_OK == m_reader.SetCurTime((m_nPlayStatus != N_PS_REW), curTime, outCH))
		{
			m_nPlayStatus = N_PS_PLAY;
			m_playSpeed = PLAY_SPEED_1X; //����״̬�޸�Ϊ����������ô�����ٶ�ҲҪ�޸ĳ�1����
		}
		else
		{
			m_playLock.UnLock();
			return false;
		}
	}
	else if(m_nPlayStatus == N_PS_PAUSE)
	{
		m_nPlayStatus = N_PS_CHANGING_TIME;

		if(m_bFrameReadFromFile)
		{
			CLocalDevice::Instance()->PlaybackSetBuf(&m_frameInfo);
			m_bFrameReadFromFile = false;
		}

		m_speedCtrl.bFirstFrame = true;
		if(MR_RET_OK != m_reader.SetCurTime((m_nPlayStatus != N_PS_REW), curTime, outCH))
		{
			m_playLock.UnLock();
			return false;
		}
		else
		{
			m_nPlayStatus = N_PS_PLAY;
			m_playSpeed = PLAY_SPEED_1X;

			CLocalDevice::Instance()->PlaybackPause(true);
		}
	}
	else
	{
		//do nothing
	}

	ADAPT_TO_ACTUAL_OUTPUT_ULL_CHNN(outCH);
	m_activeChnnNum = 0;
	ULONGLONG chnBits = outCH;
	while ((0 != chnBits) && (m_activeChnnNum < m_videoOutputNum))
	{
		if (chnBits & 0x01)
		{
			++ m_activeChnnNum;
		}

		chnBits = (chnBits >> 1);
	}

	m_frameSpeedStatus = 1;
	m_forwardType = FF_TYPE_SMOOTHLY;

	m_bSetTimeFinish = false;
	m_playLock.UnLock();

	return true;
}

bool CPlaybackManager::SetCurChannelMask(ULONGLONG ullWindowMask,  ULONGLONG & outCH)
{
	m_playLock.Lock();
	ADAPT_TO_VIRTUAL_ULL_WINDOW_MASK(ullWindowMask);
	//begin:�����������⴦������ָ�ʱ�������ô������طţ�С������С�����ط�
	if(m_bIsSepcSplit)
	{
		SET_PRIMARY_STREAM_CHNN(m_primaryStreamDisplayIndex,ullWindowMask);
	}
	//end

	memset( m_playImageChannWidth, 0, sizeof(int)*PLAYBACK_SPS_MAX_CHN_NUM);
	memset( m_playImageChannHeight, 0, sizeof(int)*PLAYBACK_SPS_MAX_CHN_NUM);
	memset( m_playImageChannFrames, 0, sizeof(int)*PLAYBACK_SPS_MAX_CHN_NUM);
	memset( m_playImageChannTmCount, 0, sizeof(int)*PLAYBACK_SPS_MAX_CHN_NUM);
	memset( m_playImageChannPreTms, 0, sizeof(LONGLONG)*PLAYBACK_SPS_MAX_CHN_NUM);

	if(m_nPlayStatus == N_PS_PLAY)
	{
		m_nPlayStatus = N_PS_CHANGING_TIME;

		if(m_bFrameReadFromFile)
		{
			CLocalDevice::Instance()->PlaybackSetBuf(&m_frameInfo);
			m_bFrameReadFromFile = false;
		}

		m_speedCtrl.bFirstFrame = true;
	
		if (MR_RET_OK != m_reader.SetCurChannel(ullWindowMask, outCH))
		{
			m_playLock.UnLock();
			return false;
		}
		else
		{
			ADAPT_TO_ACTUAL_OUTPUT_ULL_CHNN(outCH);
			unsigned char chnnNumAdapt = 0;
			ADAPT_TO_ACTUAL_CHNN_NUM(m_chnnNum,chnnNumAdapt);
		
			ULONGLONG ullTemp = (outCH & ((((ULONGLONG)0x01) << m_localChnnNum)-1)) << (chnnNumAdapt-m_localChnnNum);
			ullTemp |= (outCH & ~((((ULONGLONG)0x01) << m_localChnnNum)-1)) >> m_localChnnNum;
			ULONGLONG stopCH = m_prePlayChannels & ~ullTemp;

			//ֹͣ�豸ǰ�����û����Ҫ��ʾ��ͼ��
			if(m_bFrameReadFromFile)
			{
				CLocalDevice::Instance()->PlaybackSetBuf(&m_frameInfo);
				m_bFrameReadFromFile = false;
			}

			//��ֹͣ�豸
			//CLocalDevice::Instance()->PlaybackStop(stopCH);

			//�ٿ����豸
			CLocalDevice::Instance()->PlaybackStart(ullTemp, true);

			m_prePlayChannels = ullTemp;

			m_nPlayStatus = N_PS_PLAY;

			m_speedCtrl.bFirstFrame = true;
			m_playSpeed = PLAY_SPEED_1X;
		}
	}
	else if(m_nPlayStatus == N_PS_PAUSE)
	{
		m_nPlayStatus = N_PS_CHANGING_TIME;

		if(m_bFrameReadFromFile)
		{
			CLocalDevice::Instance()->PlaybackSetBuf(&m_frameInfo);
			m_bFrameReadFromFile = false;
		}

		m_speedCtrl.bFirstFrame = true;

		if (MR_RET_OK != m_reader.SetCurChannel(ullWindowMask, outCH))
		{
			m_playLock.UnLock();
			return false;
		}
		else
		{
			ADAPT_TO_ACTUAL_OUTPUT_ULL_CHNN(outCH);
			unsigned char chnnNumAdapt = 0;
			ADAPT_TO_ACTUAL_CHNN_NUM(m_chnnNum,chnnNumAdapt);
		
			ULONGLONG ullTemp = (outCH & ((((ULONGLONG)0x01) << m_localChnnNum)-1)) << (chnnNumAdapt-m_localChnnNum);
			ullTemp |= (outCH & ~((((ULONGLONG)0x01) << m_localChnnNum)-1)) >> m_localChnnNum;
			ULONGLONG stopCH = m_prePlayChannels & ~ullTemp;

			//ֹͣ�豸ǰ�����û����Ҫ��ʾ��ͼ��
			if(m_bFrameReadFromFile)
			{
				CLocalDevice::Instance()->PlaybackSetBuf(&m_frameInfo);
				m_bFrameReadFromFile = false;
			}

			//��ֹͣ�豸
			//CLocalDevice::Instance()->PlaybackStop(stopCH);

			//�ٿ����豸
			CLocalDevice::Instance()->PlaybackStart(ullTemp, true);

			m_prePlayChannels |= ullTemp;

			m_nPlayStatus = N_PS_PAUSE;

			CLocalDevice::Instance()->PlaybackPause(true);

			m_speedCtrl.bFirstFrame = true;
		}
	}
	else
	{
		//do nothing
		ULONGLONG outCH = 0;
		m_reader.SetCurChannel(ullWindowMask, outCH);
		ADAPT_TO_ACTUAL_OUTPUT_ULL_CHNN(outCH);
	}

	m_bSetTimeFinish = false;

	m_activeChnnNum = 0;
	ULONGLONG chnBits = outCH;
	while ((0 != chnBits) && (m_activeChnnNum < m_videoOutputNum))
	{
		if (chnBits & 0x01)
		{
			++ m_activeChnnNum;
		}

		chnBits = (chnBits >> 1);
	}

	m_channelList.RemoveAll();
	for (unsigned long chnn = 0; chnn < m_chnnNum; ++chnn)
	{
		if (ullWindowMask & (ULONGLONG)0x01 << chnn)
		{
			m_channelList.AddTail(chnn);
		}
	}
	m_frameSpeedStatus = 1;
	m_forwardType = FF_TYPE_SMOOTHLY;

	m_playLock.UnLock();

	return true;
}

void CPlaybackManager::SetSpecSplit(bool bIsSepcSplit, unsigned long primaryStreamDisplayIndex)
{
	m_bIsSepcSplit = bIsSepcSplit;
	m_primaryStreamDisplayIndex = primaryStreamDisplayIndex;
}


void CPlaybackManager::SetPlaySpeed(bool bForward, PLAY_SPEED playSpeed)
{
	m_playLock.Lock();

	if((m_nPlayStatus == N_PS_STOP) || (m_nPlayStatus == N_PS_PAUSE))
	{
		//�ط���ֹͣ,�������κβ���
	}
	else
	{
		m_playSpeed = playSpeed;

		if(!bForward)
		{
			m_nPlayStatus = N_PS_REW;
			assert(playSpeed >= PLAY_SPEED_8X);
		}
		else if(playSpeed != PLAY_SPEED_1X)
		{
			assert(playSpeed <= PLAY_SPEED_16X);
			m_nPlayStatus = N_PS_FF;

			switch(playSpeed)
			{
			case PLAY_SPEED_2X:
				m_frameSpeedStatus = 2;
				break;
			case PLAY_SPEED_4X:
				m_frameSpeedStatus = 4;
				break;
			case PLAY_SPEED_8X:
				m_frameSpeedStatus = 8;
				break;
			case PLAY_SPEED_16X:
				m_frameSpeedStatus = 16;
				break;
			default:
				m_frameSpeedStatus = 1;
				break;
			}

			//�����128����ʾ��Ʒ�������Լ����
			//���ǲ���Ĭ�ϣ����ܼ�·�طţ����ֻ������4������֡���롣
			if (128 == m_maxFFSpeed)
			{
				if (m_frameSpeedStatus <= 4)
				{
					m_forwardType = FF_TYPE_SMOOTHLY;	

					if (TD_2704HD == CProduct::Instance()->ProductType())
					{
						m_forwardType = FF_TYPE_JUMPING;
					}					
				}
				else
				{
					m_forwardType = FF_TYPE_JUMPING;
				}
			}
			else
			{
				if (m_playSpeed <= PLAY_SPEED_1_2X)
				{
					m_forwardType = FF_TYPE_SMOOTHLY;
				}
				else
				{
					if (m_maxFFSpeed < (m_frameSpeedStatus * m_activeChnnNum))
					{
						m_forwardType = FF_TYPE_JUMPING;
					}
					else
					{
						m_forwardType = FF_TYPE_SMOOTHLY;
					}
					if (TD_2704HD == CProduct::Instance()->ProductType())
					{
						m_forwardType = FF_TYPE_JUMPING;
					}

					//23XXSS-B��Ʒ��CVBS���ʱ�ط�1·�����֡�����ٶ�Ϊ2����
					PRODUCT_TYPE product = CProduct::Instance()->ProductType();
					if ((TD_2304SS_B == product) || (TD_2308SS_B == product) || (TD_2316SS_B == product) || 
						(TD_2304SS_C == product) || (TD_2308SS_C == product) || (TD_2316SS_C == product) ||
						(TD_2304SL_C == product) || (TD_2308SL_C == product))
					{
						if ((VIDEO_OUT_CVBS == CLocalDevice::Instance()->GetCurMainDevice()) && (m_maxFFSpeed <= (m_frameSpeedStatus * m_activeChnnNum)))
						{
							m_forwardType = FF_TYPE_JUMPING;
						}
					}
					
					//����Ͳ�Ʒ��������ͨ���طţ����ʱ����֡
					if ((0 < m_localChnnNum) && (m_localChnnNum < m_chnnNum))
					{
						int channelTemp = 0;
						POS pos = m_channelList.GetHeadPosition();
						while(pos)
						{
							channelTemp = m_channelList.GetNext(pos);

							if (channelTemp >= m_localChnnNum)
							{
								m_forwardType = FF_TYPE_JUMPING;
								break;
							}
						}
					}
				}
			}
		}
		else
		{
			m_nPlayStatus = N_PS_PLAY;	
			m_frameSpeedStatus = 1;
			m_forwardType = FF_TYPE_SMOOTHLY;
			m_playSpeed = PLAY_SPEED_1X;
		}

		if(m_bFrameReadFromFile)
		{
			CLocalDevice::Instance()->PlaybackSetBuf(&m_frameInfo);
			m_bFrameReadFromFile = false;
		}

		m_speedCtrl.bFirstFrame = true;
	}

	m_playLock.UnLock();
}

void CPlaybackManager::PlayFF()
{
	m_playLock.Lock();
	if((m_nPlayStatus != N_PS_STOP) && (m_nPlayStatus != N_PS_PAUSE))
	{
		if(m_nPlayStatus != N_PS_FF)
		{
			m_nPlayStatus = N_PS_FF;
			m_playSpeed = PLAY_SPEED_2X;
		}
		else
		{
			if(m_playSpeed < PLAY_SPEED_16X)
			{
				char temp = m_playSpeed;
				++ temp;
				m_playSpeed = (PLAY_SPEED)temp;
				if(m_playSpeed == PLAY_SPEED_1X)
				{
					m_nPlayStatus = N_PS_PLAY;
				}
			}
			else
			{
				m_playSpeed = PLAY_SPEED_1X;
				m_nPlayStatus = N_PS_PLAY;
			}

			if(m_bFrameReadFromFile)
			{
				CLocalDevice::Instance()->PlaybackSetBuf(&m_frameInfo);
				m_bFrameReadFromFile = false;
			}

			m_speedCtrl.bFirstFrame = true;
		}

		switch(m_playSpeed)
		{
		case PLAY_SPEED_2X:
			m_frameSpeedStatus = 2;
			break;
		case PLAY_SPEED_4X:
			m_frameSpeedStatus = 4;
			break;
		case PLAY_SPEED_8X:
			m_frameSpeedStatus = 8;
			break;
		case PLAY_SPEED_16X:
			m_frameSpeedStatus = 16;
			break;
		default:
			m_frameSpeedStatus = 1;
			break;
		}

		//�����128����ʾ��Ʒ�������Լ����
		//���ǲ���Ĭ�ϣ����ܼ�·�طţ����ֻ������4������֡���롣
		if (128 == m_maxFFSpeed)
		{
			if (m_frameSpeedStatus <= 4)
			{
				m_forwardType = FF_TYPE_SMOOTHLY;

				if (TD_2704HD == CProduct::Instance()->ProductType())
				{
					m_forwardType = FF_TYPE_JUMPING;
				}
			}
			else
			{
				m_forwardType = FF_TYPE_JUMPING;
			}
		}
		else
		{
			if (m_playSpeed <= PLAY_SPEED_1_2X)
			{
				m_forwardType = FF_TYPE_SMOOTHLY;
			}
			else
			{
				if (m_maxFFSpeed < (m_frameSpeedStatus * m_activeChnnNum))
				{
					m_forwardType = FF_TYPE_JUMPING;
				}
				else
				{
					m_forwardType = FF_TYPE_SMOOTHLY;
				}
				if (TD_2704HD == CProduct::Instance()->ProductType())
				{
					m_forwardType = FF_TYPE_JUMPING;
				}

				//23XXSS-B��Ʒ��CVBS���ʱ�ط�1·�����֡�����ٶ�Ϊ2����
				PRODUCT_TYPE product = CProduct::Instance()->ProductType();
				if ((TD_2304SS_B == product) || (TD_2308SS_B == product) || (TD_2316SS_B == product) || 
					(TD_2304SS_C == product) || (TD_2308SS_C == product) || (TD_2316SS_C == product) ||
					(TD_2304SL_C == product) || (TD_2308SL_C == product))
				{
					if ((VIDEO_OUT_CVBS == CLocalDevice::Instance()->GetCurMainDevice()) && (m_maxFFSpeed <= (m_frameSpeedStatus * m_activeChnnNum)))
					{
						m_forwardType = FF_TYPE_JUMPING;
					}
				}
			}
		}
	}

	m_playLock.UnLock();
}

void CPlaybackManager::PlayRew()
{
	m_playLock.Lock();
	if((m_nPlayStatus != N_PS_STOP) && (m_nPlayStatus != N_PS_PAUSE))
	{
		if(m_nPlayStatus != N_PS_REW)
		{
			//����,����8����
			m_nPlayStatus = N_PS_REW;
			m_playSpeed = PLAY_SPEED_8X;
		}
		else
		{
			if(m_playSpeed < PLAY_SPEED_16X)
			{
				char temp = m_playSpeed;
				++temp;
				m_playSpeed = (PLAY_SPEED)temp;
			}
			else	//�ﵽ�������ٶȣ�������������
			{
				m_playSpeed = PLAY_SPEED_1X;
				m_nPlayStatus = N_PS_PLAY;
			}

			if(m_bFrameReadFromFile)
			{
				CLocalDevice::Instance()->PlaybackSetBuf(&m_frameInfo);
				m_bFrameReadFromFile = false;
			}

			m_speedCtrl.bFirstFrame = true;
		}

		m_frameSpeedStatus = 1;
		m_forwardType = FF_TYPE_SMOOTHLY;
	}

	m_playLock.UnLock();
}

void CPlaybackManager::PlayNextFrame(bool bForward)
{
	assert (N_PS_PAUSE == m_nPlayStatus);

	if(N_PS_PAUSE == m_nPlayStatus)
	{
		if(bForward)
		{
			m_bPlayNext = true;
		}
		else
		{
			m_bPlayPrev = true;
		}
	}
}

void CPlaybackManager::SetAudioChannel(unsigned long channel)
{
	m_audioChannel = (channel >= m_localChnnNum) ? (channel - m_localChnnNum) : (channel + (m_chnnNum - m_localChnnNum));
	printf("%s,%d,chnn:%d\n", __FILE__, __LINE__, m_audioChannel);
}



RESULT WINAPI CPlaybackManager::PlayProc(void *pParam)
{
#ifdef __ENVIRONMENT_LINUX__
	pid_t pid = getpid();
	pid_t tid = syscall(__NR_gettid);
	printf("%s, %d, pid = %d, tid=%d\n", __FILE__, __LINE__, pid, tid);
#endif

	CPlaybackManager *pThis = (CPlaybackManager *)pParam;
	pThis->PlayProc();
	
	return 0;
}

void CPlaybackManager::PlayProc()
{
	memset(&m_frameInfo, 0, sizeof(FRAME_INFO_EX));

	LONGLONG lastShowTime = 0;
	unsigned long MultiCH = 0, chnn = 0;
	unsigned long frameTypeTemp = FRAME_TYPE_NONE;
	bool bNeedSleep = false;
	unsigned long getBufferTryTimes = 0;
	unsigned long lastFrameHeight = 0;
	unsigned long lastFrameWidth = 0;

	bool	bTimeCheck = false;
	LONGLONG diffTime = 0;


	while(true)
	{
		bNeedSleep = false;

		m_playLock.Lock();

		if (!m_bRun)
		{
			if(m_bFrameReadFromFile)
			{
				CLocalDevice::Instance()->PlaybackSetBuf(&m_frameInfo);
				m_bFrameReadFromFile = false;
			}
			m_playLock.UnLock();
#if defined(__TD2316SES_SPEC__) || defined(__TDFH__) 
			PUB_Sleep(100);
#else
			PUB_Sleep(10);
#endif
			continue;
		}

		if(m_nPlayStatus == N_PS_STOP)
		{
			if(m_bFrameReadFromFile)
			{
				CLocalDevice::Instance()->PlaybackSetBuf(&m_frameInfo);
				m_bFrameReadFromFile = false;
			}
			m_playLock.UnLock();
			break;
		}

		if(m_nPlayStatus == N_PS_CHANGING_TIME)
		{
			if(m_bFrameReadFromFile)
			{
				CLocalDevice::Instance()->PlaybackSetBuf(&m_frameInfo);
				m_bFrameReadFromFile = false;
			}
			m_playLock.UnLock();
			PUB_Sleep(10);
			continue;
		}

		if(m_nPlayStatus == N_PS_PAUSE)
		{
			if(!m_bPlayNext && !m_bPlayPrev && m_bSetTimeFinish)		//���û�а���һ֡��ť
			{
				if(m_bFrameReadFromFile)
				{
					CLocalDevice::Instance()->PlaybackSetBuf(&m_frameInfo);
					m_bFrameReadFromFile = false;
				}
				m_playLock.UnLock();
				PUB_Sleep(200);
				continue;
			}
		}

		if(!m_bFrameReadFromFile)
		{
			memset(&m_frameInfo, 0, sizeof(m_frameInfo));

			//��ȡ��һ֡��֡����;
			long ret;
			if((m_nPlayStatus == N_PS_REW) && (m_playSpeed >= PLAY_SPEED_8X))
			{
				ret = m_reader.GetNextFrameType(m_frameInfo, false);
			}
			else
			{
				ret = m_reader.GetNextFrameType(m_frameInfo, true);
			}

			if(MR_RET_OK != ret)
			{
				m_pMsgCallBack(m_pCallBackObj, 2, NULL, NULL);
				m_playLock.UnLock();
				break;
			}

			frameTypeTemp = m_frameInfo.frameType;

			//��2010-01-20 15:18:00 YSW��
			//�ѻ�ȡ�ڴ�ͳһ��LocaclDevice�ڲ�ȥ����������Ƶ������Ƶ��ʽ��������һƬ�ڴ�
			//�������Ǵӽ�������ȡ�ڴ��ַ��
			m_frameInfo.channel = CHANNEL_ID(m_frameInfo.channel);
	
			if(m_frameInfo.channel >= m_localChnnNum)
			{
				m_frameInfo.channel -= m_localChnnNum;
			}
			else
			{
				m_frameInfo.channel += (m_chnnNum - m_localChnnNum);
			}

			//��ȡBUFFER��������MAX_GET_BUF_NUM������ζ�Ż�ȡ������ʧ�ܣ�����Ҫ�˳��طš�
			if(getBufferTryTimes >= MAX_GET_BUF_NUM)
			{
				m_pMsgCallBack(m_pCallBackObj, 2, NULL, NULL);
				m_playLock.UnLock();

				printf("%s,%d,not get buffer.time:%d\n",__FILE__,__LINE__,time);				

				RUN_LOG(RUN_LOG_ITEM_PB_GET_FRAME_FAIL, CRunLog::RUN_Num("CH:", m_frameInfo.channel), CRunLog::RUN_Num("Frame Type:", m_frameInfo.frameType), "Get Buffer fail");

				break;
			}

			ADAPT_TO_ACTUAL_CHANNEL(m_frameInfo.channel);
			if(!CLocalDevice::Instance()->PlaybackGetBuf(&m_frameInfo))
			{
				getBufferTryTimes++;
				m_playLock.UnLock();
				bNeedSleep = true;
				goto PROC_END;
			}
			else
			{
				getBufferTryTimes = 0;
			}

			//���ݲ���״̬�����ò�ͬ�Ľӿڶ�ȡ���ݣ�8�������������ؼ�֡��������������������֡���С�
			ADAPT_TO_VIRTUAL_CHANNEL(m_frameInfo.channel);
			
			if((m_nPlayStatus == N_PS_FF) && (FF_TYPE_JUMPING == m_forwardType))
			{
				ret = m_reader.GetKeyFrame(m_frameInfo, 1, true);
			}
			else if((m_nPlayStatus == N_PS_REW) && (m_playSpeed >= PLAY_SPEED_8X))
			{
				ret = m_reader.GetKeyFrame(m_frameInfo, 1, false);
			}
			else if((m_nPlayStatus == N_PS_PLAY) || (m_nPlayStatus == N_PS_FF) || (m_nPlayStatus == N_PS_REW))
			{
				ret = m_reader.GetNextFrame(m_frameInfo);
			}
			else if ((m_nPlayStatus == N_PS_PAUSE) && (m_bPlayNext || m_bPlayPrev || !m_bSetTimeFinish))
			{
				ret = m_reader.GetNextFrame(m_frameInfo);
			}
			else
			{
				RUN_LOG(RUN_LOG_ITEM_PB_GET_FRAME_FAIL, CRunLog::RUN_Num("Play Status:", m_nPlayStatus), CRunLog::RUN_Num("Play Speed:", m_playSpeed), "Get Next Frame fail");
			}

			//�޸����⣺�ڵ���GetNextFrameType����ʱ���õ�����Ƶ֡��������GetNextFrame��
			//GetKeyFrameʱ�õ�������Ƶ֡��
			//����������������Ƶʱ�����8��16,32�������Ŀ������ˡ�
			if (frameTypeTemp != m_frameInfo.frameType)
			{
				printf("#############################################chnn:%d,frameType error,%d,%d. keyframe:%d\n", m_frameInfo.channel, frameTypeTemp, m_frameInfo.frameType, m_frameInfo.keyFrame);
				m_frameInfo.frameType = FRAME_TYPE_NONE;
				m_frameInfo.frameIndex = 1;
				ADAPT_TO_ACTUAL_CHANNEL(m_frameInfo.channel);
				CLocalDevice::Instance()->PlaybackSetBuf(&m_frameInfo);
				m_bFrameReadFromFile = false;
				m_playLock.UnLock();
				continue;
			}
			//��ȡ���ݳɹ�
			if(MR_RET_OK == ret)
			{
				if(m_frameInfo.channel >= m_localChnnNum)
				{
					m_frameInfo.channel -= m_localChnnNum;
				}
				else
				{
					m_frameInfo.channel += (m_chnnNum - m_localChnnNum);
				}
				ADAPT_TO_ACTUAL_CHANNEL(m_frameInfo.channel);
				m_bFrameReadFromFile = true;
				bTimeCheck = true;

				if (m_frameInfo.frameType == FRAME_TYPE_VIDEO)
				{
					if(!m_bSetTimeFinish)
					{
						m_bSetTimeFinish = true;
					}

					m_bPlayNext = false;
					m_bPlayPrev = false;
#if defined(__CHIP3535__) || defined(__CHIP3531__) 

#if defined(__CHIP3535__)
					if( g_video_format == VIDEO_FORMAT_NTSC && m_frameInfo.channel < PLAYBACK_SPS_MAX_CHN_NUM )
#else
					//3531����Ͳ�Ʒ
					if(0 < m_chnnNum)
#endif
					{
						int cur_chann_frame = 30;
						int save_prev_fram_cnt = m_playImageChannFrames[m_frameInfo.channel];
						LONGLONG llFrmTimesAdd;

						if( 0 != m_playImageChannPreTms[m_frameInfo.channel] )
						{
							++m_playImageChannTmCount[m_frameInfo.channel];
							if( m_playImageChannTmCount[m_frameInfo.channel] >= 30 )
							{
								llFrmTimesAdd = m_frameInfo.time - m_playImageChannPreTms[m_frameInfo.channel];
								if( llFrmTimesAdd > 0 )
								{
									cur_chann_frame = (int)((30000000)/llFrmTimesAdd);
									if( cur_chann_frame <= 7 )
									{
										cur_chann_frame = 7;
									}
									else if( cur_chann_frame <= 15 )
									{
										cur_chann_frame = 15;
									}
									else if( cur_chann_frame <= 22 )
									{
										cur_chann_frame = 22;
									}
									else if( cur_chann_frame <= 26 )
									{
										cur_chann_frame = 26;
									}
									else
									{
										cur_chann_frame = 30;
									}
								}
								else
								{
									cur_chann_frame = 15;
								}

								m_playImageChannPreTms[m_frameInfo.channel] = m_frameInfo.time;
								m_playImageChannTmCount[m_frameInfo.channel] = 0;
								m_playImageChannFrames[m_frameInfo.channel] = cur_chann_frame;
							}
						}
						else
						{
							m_playImageChannPreTms[m_frameInfo.channel] = m_frameInfo.time;
							m_playImageChannTmCount[m_frameInfo.channel] = 0;
							m_playImageChannFrames[m_frameInfo.channel] = 15;
						}

						if( m_playImageChannWidth[m_frameInfo.channel] != m_frameInfo.width 
							|| m_playImageChannHeight[m_frameInfo.channel] != m_frameInfo.height 
							|| save_prev_fram_cnt != m_playImageChannFrames[m_frameInfo.channel])
						{
							m_playImageChannWidth[m_frameInfo.channel] = m_frameInfo.width;
							m_playImageChannHeight[m_frameInfo.channel] = m_frameInfo.height;

							//�طŵ�3M��1080P��ͨ����
							unsigned char ipc_resolution_3M_num = 0;
							unsigned char ipc_resolution_1080P_num = 0;
							int cur_total_resolution = 0;
							for( int idx = 0; idx < PLAYBACK_SPS_MAX_CHN_NUM; idx++ )
							{
								if(m_playImageChannWidth[idx]==2048 && m_playImageChannHeight[idx]==1536)
								{
									ipc_resolution_3M_num++;
								}
								else if(m_playImageChannWidth[idx]==1920 && m_playImageChannHeight[idx]==1080)
								{
									ipc_resolution_1080P_num++;
								}
								cur_total_resolution += m_playImageChannWidth[idx]*m_playImageChannHeight[idx]*m_playImageChannFrames[idx];
							}

#if defined(__CHIP3535__)
							if(64 <= CProduct::Instance()->NetVideoInputNum()) //32·����
							{
								if( cur_total_resolution == 2048*1536*4*30 && ipc_resolution_3M_num == 4) // 4·3M¼��ط�
								{
									m_playRate[PLAY_SPEED_1X] = 0.5;
								}
								else if( (ipc_resolution_3M_num == 3) \
										 || (ipc_resolution_3M_num == 2 && ipc_resolution_1080P_num >= 1)) // ����3·3M¼��طš�����2·3M¼��ط�ͬʱ1·1080p�ط�
								{
									m_playRate[PLAY_SPEED_1X] = 0.67;
								}
								else if(ipc_resolution_3M_num==1 && ipc_resolution_1080P_num >= 3)
								{
									m_playRate[PLAY_SPEED_1X] = 0.85;
								}
								else
								{
									m_playRate[PLAY_SPEED_1X] = 1;
								}
							}
							else // ��������
							{
								if( cur_total_resolution > 2048*1536*2*30 + 1920*1080*2*30 )
								{
									if (32 <= CProduct::Instance()->NetVideoInputNum())
									{
										m_playRate[PLAY_SPEED_1X] = 0.75;
									}
									else
									{
										m_playRate[PLAY_SPEED_1X] = 0.85;
									}
								}
								else
								{
									m_playRate[PLAY_SPEED_1X] = 1;
								}
							}
							
#else
							if( cur_total_resolution > 2048*1536*2*30 + 1920*1080*2*30 ) 
							{
								if (0 == m_localChnnNum)
								{
									if (g_video_format == VIDEO_FORMAT_NTSC)
									{
										m_playRate[PLAY_SPEED_1X] = 0.8;
									}
								}
								else
								{
									if (g_video_format == VIDEO_FORMAT_NTSC)
									{
										m_playRate[PLAY_SPEED_1X] = 0.7;
									}
									else
									{
										m_playRate[PLAY_SPEED_1X] = 0.75;
									}
								}

							}
							else
							{
								if( 1 != m_playRate[PLAY_SPEED_1X])
								{
									//printf( "current playback rate == 1,%s,%d\n", __FILE__, __LINE__ );
								}
								m_playRate[PLAY_SPEED_1X] = 1;
							}
#endif
						}
					}
//#if defined(__CHIP3535__)
#if 0
					else if( g_video_format == VIDEO_FORMAT_PAL && m_frameInfo.channel < PLAYBACK_SPS_MAX_CHN_NUM )
					{
						int cur_chann_frame = 25;
						int save_prev_fram_cnt = m_playImageChannFrames[m_frameInfo.channel];
						LONGLONG llFrmTimesAdd;

						if( 0 != m_playImageChannPreTms[m_frameInfo.channel] )
						{
							++m_playImageChannTmCount[m_frameInfo.channel];
							if( m_playImageChannTmCount[m_frameInfo.channel] >= 25 )
							{
								llFrmTimesAdd = m_frameInfo.time - m_playImageChannPreTms[m_frameInfo.channel];
								if( llFrmTimesAdd > 0 )
								{
									cur_chann_frame = (int)((25000000)/llFrmTimesAdd);
									if( cur_chann_frame <= 10 )
									{
										cur_chann_frame = 10;
									}
									else if( cur_chann_frame <= 15 )
									{
										cur_chann_frame = 15;
									}
									else if( cur_chann_frame <= 20 )
									{
										cur_chann_frame = 20;
									}
									else if( cur_chann_frame <= 22 )
									{
										cur_chann_frame = 22;
									}
									else
									{
										cur_chann_frame = 25;
									}
								}
								else
								{
									cur_chann_frame = 15;
								}

								m_playImageChannPreTms[m_frameInfo.channel] = m_frameInfo.time;
								m_playImageChannTmCount[m_frameInfo.channel] = 0;
								m_playImageChannFrames[m_frameInfo.channel] = cur_chann_frame;
							}
						}
						else
						{
							m_playImageChannPreTms[m_frameInfo.channel] = m_frameInfo.time;
							m_playImageChannTmCount[m_frameInfo.channel] = 0;
							m_playImageChannFrames[m_frameInfo.channel] = 15;
						}

						if( m_playImageChannWidth[m_frameInfo.channel] != m_frameInfo.width 
							|| m_playImageChannHeight[m_frameInfo.channel] != m_frameInfo.height 
							|| save_prev_fram_cnt != m_playImageChannFrames[m_frameInfo.channel])
						{
							m_playImageChannWidth[m_frameInfo.channel] = m_frameInfo.width;
							m_playImageChannHeight[m_frameInfo.channel] = m_frameInfo.height;

							//�طŵ�3M��1080P��ͨ����
							unsigned char ipc_resolution_3M_num = 0;
							unsigned char ipc_resolution_1080P_num = 0;
							int cur_total_resolution = 0;
							for( int idx = 0; idx < PLAYBACK_SPS_MAX_CHN_NUM; idx++ )
							{
								if(m_playImageChannWidth[idx]==2048 && m_playImageChannHeight[idx]==1536)
								{
									ipc_resolution_3M_num++;
								}
								else if(m_playImageChannWidth[idx]==1920 && m_playImageChannHeight[idx]==1080)
								{
									ipc_resolution_1080P_num++;
								}
								cur_total_resolution += m_playImageChannWidth[idx]*m_playImageChannHeight[idx]*m_playImageChannFrames[idx];
							}

							if(32 <= CProduct::Instance()->NetVideoInputNum()) //32·����
							{
								if( cur_total_resolution == 2048*1536*4*25 && ipc_resolution_3M_num == 4) // 4·3M¼��ط�
								{
									m_playRate[PLAY_SPEED_1X] = 0.6;
									printf("set pal play speed 0.6 !\n");
								}
								else if( (ipc_resolution_3M_num == 3) \
										 || (ipc_resolution_3M_num == 2 && ipc_resolution_1080P_num >= 2)) // ����3·3M¼��طš�����2·3M¼��ط�ͬʱ1·1080p�ط�
								{
									m_playRate[PLAY_SPEED_1X] = 0.8;
									printf("set pal play speed 0.8 !\n");
								}
								else
								{
									m_playRate[PLAY_SPEED_1X] = 1;
								}
							}
							//else // ��������
							//{
							//	if( cur_total_resolution > 2048*1536*2*25 + 1920*1080*2*25 )
							//	{
							//		m_playRate[PLAY_SPEED_1X] = 0.85;
							//	}
							//	else
							//	{
							//		m_playRate[PLAY_SPEED_1X] = 1;
							//	}
							//}
						}
					}
#endif
#endif
				}
			}
		}
		
		//��ȡ���ݳɹ�
		if(m_bFrameReadFromFile)
		{
			chnn = CHANNEL_ID(m_frameInfo.channel);//�õ���ͨ���ţ����ֺ�ģ��ͳһ��ŵ�ͨ��

			//�������Ƶ����Ƶ��ʽ����
			if(m_frameInfo.frameType == FRAME_TYPE_VIDEO_FORMAT)
			{
				m_frameInfo.frameIndex = 1;
				CLocalDevice::Instance()->PlaybackSetBuf(&m_frameInfo);
				m_bFrameReadFromFile = false;
				m_playLock.UnLock();
				continue;
			}
			else if(m_frameInfo.frameType == FRAME_TYPE_AUDIO_FORMAT)
			{
				m_frameInfo.frameIndex = 1;
				CLocalDevice::Instance()->PlaybackSetBuf(&m_frameInfo);
				m_bFrameReadFromFile = false;
				m_playLock.UnLock();
				continue;
			}
			else if(m_frameInfo.frameType == FRAME_TYPE_VIDEO)
			{
				m_bPlayNext = false;
				m_bPlayPrev = false;
				//FH��Ʒ�ڷֱ��ʸı�ʱҪ�رջطţ�������쳣
#if defined(__TDFH__) || defined(__CHIPTI__)
				if ( lastFrameHeight && lastFrameHeight != m_frameInfo.height || lastFrameWidth && lastFrameWidth != m_frameInfo.width)
				{
					m_pMsgCallBack(m_pCallBackObj, 3, NULL, NULL);
				}

				lastFrameHeight = m_frameInfo.height;
				lastFrameWidth	= m_frameInfo.width;
#endif


#ifdef __ENVIRONMENT_WIN32__
				m_frameInfo.frameAttrib = PLAY_FRAME_SHOW;
#else
				m_frameInfo.frameAttrib = PLAY_FRAME_NO_SHOW;
#endif

				if(m_speedCtrl.bFirstFrame)
				{
					m_speedCtrl.bFirstFrame = false;
					m_speedCtrl.lastShowTime = GetCurrTime();
					m_speedCtrl.lastFrameTime = m_frameInfo.time;
					MultiCH = 0x01 << chnn;
				}
				else if (0 == (MultiCH & (0x01 << chnn)))
				{
					MultiCH |= (0x01 << chnn);
				}
				else if(GetCurrTime() < m_speedCtrl.lastShowTime)
				{
					m_playLock.UnLock();
					bNeedSleep = true;
					goto PROC_END;
				}
				else if((0 != (MultiCH & (0x01 << chnn))) && (true == bTimeCheck))
				{
					if(m_nPlayStatus == N_PS_REW)
					{
						diffTime = static_cast<LONGLONG>((m_speedCtrl.lastFrameTime - m_frameInfo.time) / m_playRate[m_playSpeed]);
					}
					else
					{
						diffTime = static_cast<LONGLONG>((m_frameInfo.time - m_speedCtrl.lastFrameTime) / m_playRate[m_playSpeed]);
					}

					m_frameInfo.frameAttrib = PLAY_FRAME_SHOW;
					m_frameInfo.frameIndex = m_frameSpeedStatus;
					m_frameInfo.frameIndex =(m_frameInfo.frameIndex << 16);
					m_frameInfo.frameIndex |= (FF_TYPE_JUMPING == m_forwardType) ? 1 : m_frameSpeedStatus;

					if (m_frameInfo.time > lastShowTime)
					{
						if (m_frameInfo.time >= (lastShowTime + 1000000))
						{
							m_pMsgCallBack(m_pCallBackObj, 1, &m_frameInfo.time, NULL);
							lastShowTime = m_frameInfo.time;
						}
					}
					else
					{
						if (lastShowTime >= (m_frameInfo.time + 1000000))
						{
							m_pMsgCallBack(m_pCallBackObj, 1, &m_frameInfo.time, NULL);
							lastShowTime = m_frameInfo.time;
						}
					}

					m_speedCtrl.lastFrameTime = m_frameInfo.time;

					if ((0 < diffTime) && (3000000 > diffTime))
					{
						m_speedCtrl.lastShowTime += diffTime;
					}
					else
					{
						m_speedCtrl.lastShowTime = GetCurrTime();
					}

					bTimeCheck = false;
					bNeedSleep = false;
					MultiCH = (0x01 << chnn);

					m_playLock.UnLock();

					continue;
				}
				else
				{
					MultiCH |= (0x01 << chnn);
				}

				m_frameInfo.frameIndex = m_frameSpeedStatus;
				m_frameInfo.frameIndex =(m_frameInfo.frameIndex << 16);
				m_frameInfo.frameIndex |= (FF_TYPE_JUMPING == m_forwardType) ? 1 : m_frameSpeedStatus;

				CLocalDevice::Instance()->PlaybackSetBuf(&m_frameInfo);
				m_bFrameReadFromFile = false;
				m_playLock.UnLock();

				continue;
			}
			else if(m_frameInfo.frameType == FRAME_TYPE_AUDIO)
			{
				if ((m_nPlayStatus != N_PS_PLAY) || (m_audioChannel != chnn))
				{
					//����ÿ�ζ�ȡ����֮ǰ�Ѿ���ȡ���˻��������Ҳ���֪����������֡�����ͣ�����
					//������ǵ�ǰ�طŵ�ͨ��������Ҫ���й��ˡ�
					m_frameInfo.frameType = FRAME_TYPE_NONE;
				}

				m_frameInfo.frameIndex = 1;

				CLocalDevice::Instance()->PlaybackSetBuf(&m_frameInfo);
				m_bFrameReadFromFile = false;

				m_playLock.UnLock();

				continue;
			}
			else
			{
				m_frameInfo.frameIndex = 1;

				CLocalDevice::Instance()->PlaybackSetBuf(&m_frameInfo);
				m_bFrameReadFromFile = false;

				m_playLock.UnLock();

				continue;
			}
		}
		else
		{
			//û�����ݿɻطţ�����طŽ���
			m_frameInfo.frameIndex = 1;
			m_pMsgCallBack(m_pCallBackObj, 2, NULL, NULL);
			
			CLocalDevice::Instance()->PlaybackSetBuf(&m_frameInfo);
			m_bFrameReadFromFile = false;

			m_playLock.UnLock();

			break;
		}

PROC_END:
		if(bNeedSleep)
		{
			PUB_Sleep(1);
		}
	}

#ifdef __ENVIRONMENT_LINUX__
	RUN_LOG(RUN_LOG_ITEM_PB_EXIT, {}, {}, "Exit Playback Proc");
#endif
}

#endif //__CHIP3531__

