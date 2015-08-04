/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : xzw
** Date         : 2011-6-28
** Name         : PlaybackManager.cpp
** Version      : 3.1.9
** Description  :
** Modify Record:
1:
***********************************************************************/


#include "PlaybackManagerAsyn.h"
#include "MessageMan.h"
extern NET_LOGIN_SUCCESS_REPLY *g_p_login_succ;
extern VIDEO_FORMAT g_video_format;

#define MAX_GET_BUF_NUM 5000

//#ifndef __CHIP3531__
#ifndef __NEW_PLAYBACK__
 CPlaybackManagerAsyn::CPlaybackManagerAsyn()
{
	m_pFileName			= NULL;
	m_pPlayStatus		= NULL;
	m_pforwardType		= NULL;
	m_pframeSpeedStatus = NULL;
	m_playThread		= PUB_THREAD_ID_NOINIT;
	memset(&m_frameInfo, 0, sizeof(FRAME_INFO_EX));
	m_prePlayChannels	= 0;
	m_audioChannel		= INVALID_INDEX(unsigned long);
	m_bRun = false;
	m_bSync = true;
}

CPlaybackManagerAsyn::~CPlaybackManagerAsyn()
{

}

/*******************************************************************************************************************************
函数功能：回放功能初始化
参数说明：chnnNum 回放通道数
videoOutputNum 视频输出通道数
maxFFSpeed 最大帧率速度
pFilePath 回放录像文件路径
pReclogManEx 管理回放录像日志的类
diskOwnerIndex 录像所在的磁盘编号
bSync 是否同步播放录像
返回值  ：成功为true 
失败为false
********************************************************************************************************************************/
bool CPlaybackManagerAsyn::Initial(unsigned char chnnNum, unsigned char videoOutputNum, unsigned char maxFFSpeed, const char *pFilePath, CReclogManEX *pReclogManEx, unsigned long diskOwnerIndex, bool bSync)
{
	m_chnnNum = chnnNum;
	m_videoOutputNum = videoOutputNum;
	m_maxFFSpeed = maxFFSpeed;
	m_activeChnnNum = 0;
	m_bSync = bSync;

	if (pFilePath != NULL)
	{
		m_pFileName = new char[256];
		strcpy(m_pFileName, pFilePath);
	}

	if (!bSync)
	{
		m_asyncReader.Initial(m_chnnNum, pReclogManEx, diskOwnerIndex);

		m_pPlayStatus = new PLAY_STATE[m_chnnNum];
		m_pforwardType = new unsigned char[m_chnnNum];
		m_pframeSpeedStatus = new unsigned char[m_chnnNum];

		for (int chnn = 0; chnn < m_chnnNum; chnn ++)
		{
			m_pPlayStatus[chnn] = N_PS_STOP;
			m_pforwardType[chnn] = FF_TYPE_SMOOTHLY;
			m_pframeSpeedStatus[chnn] = 1;
			m_bFirstFrame[chnn] =  false;
		}
	}
	else
	{
		m_syncReader.Initial(m_chnnNum, pReclogManEx, diskOwnerIndex);

		m_nPlayStatus = N_PS_STOP;
		m_forwardType = FF_TYPE_SMOOTHLY;
		m_frameSpeedStatus = 1;
	}

	m_playRate[5] = 1;
	for(int i = 0; i < 5; i++)
	{
		m_playRate[i+6] = m_playRate[i+5] * 2;
		m_playRate[4-i] = m_playRate[i+6] + (0x1 << 8);
		printf("####%s,%d,m_playRate[%d] = %f\n",__FILE__,__LINE__,i,m_playRate[i]);
	}

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

/*******************************************************************************************************************************
函数功能：按启动时间回放
参数说明：startTime 录像文件开始时间
endTime 录像文件结束时间
curTime 当前播放时间
chnnBits 需要播发通道掩码
callBack 回调函数
pCallBackObj 回调指针
outCH 成功播放通道的掩码
返回值  ：成功为true 
失败为false
********************************************************************************************************************************/
bool CPlaybackManagerAsyn::Play(unsigned long startTime, unsigned long endTime, unsigned long curTime, \
								ULONGLONG chnnBits,MESSAGE_CALLBACK callBack, void *pCallBackObj, ULONGLONG &outCH)
{
	CHANNEL_LIST channelList;
	m_pMsgCallBack = callBack;
	m_pCallBackObj = pCallBackObj;

	channelList.RemoveAll();
	for (unsigned long chnn = 0; chnn < m_chnnNum; chnn ++)
	{
		if (chnnBits & ((ULONGLONG)0x01 << chnn))
		{
			channelList.AddTail(chnn);
		}
	}

	if (m_bSync)
	{
		m_bPlayNext = false;
		m_bPlayPrev = false;
		m_playSpeed = PLAY_SPEED_1X;
		m_frameSpeedStatus = 1;
		m_forwardType = FF_TYPE_SMOOTHLY;
		m_speedCtrl.bFirstFrame = true;

		if(MR_RET_OK == m_syncReader.StartRead(startTime, endTime, channelList, outCH, m_pFileName))
		{
			//启动设备
			if (CLocalDevice::Instance()->PlaybackStart(outCH))
			{
				m_nPlayStatus = N_PS_PLAY;
				m_playThread = PUB_CreateThread(SyncPlayProc, this, NULL);

				if (PUB_CREATE_THREAD_FAIL != m_playThread)
				{
					m_prePlayChannels = outCH;

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

					return true;
				}
			}
		}
	}
	else
	{
		if (CAsyncReader::MR_RET_OK == m_asyncReader.StartRead(startTime, endTime, channelList, outCH, m_pFileName))
		{
			if (CLocalDevice::Instance()->PlaybackStart(outCH))
			{
				for (unsigned long chnn = 0; chnn < m_chnnNum; chnn++)
				{
					if (outCH & ((ULONGLONG)0x01 << chnn))
					{
						m_pPlayStatus[chnn] = N_PS_PLAY;
						m_bFirstFrame[chnn] = true;
						m_asyncReader.SetForward(chnn, true);
						m_asyncReader.SetJumping(chnn, false);
						m_asyncReader.SetPlayNext(chnn, false);
					}
				}
				m_playThread = PUB_CreateThread(AsyncPlayProc, this, NULL);
				if (PUB_THREAD_ID_NOINIT != m_playThread)
				{
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
					return true;
				}
			}
		}
	}

	return false;
}

/*******************************************************************************************************************************
函数功能：退出回放进程
参数说明：
返回值  ：
********************************************************************************************************************************/
void CPlaybackManagerAsyn::Quit()
{
	if (m_bSync)
	{
		m_syncReader.Quit();
	}
	else
	{
		m_asyncReader.Quit();

		if (m_pPlayStatus != NULL)
		{
			delete [] m_pPlayStatus;
		}
		if (m_pforwardType != NULL)
		{
			delete [] m_pforwardType;
		}
		if (m_pframeSpeedStatus != NULL)
		{
			delete [] m_pframeSpeedStatus;
		}
	}

	if (m_pFileName != NULL)
	{
		delete [] m_pFileName;
	}

	m_audioChannel = INVALID_INDEX(unsigned long);
}


/*******************************************************************************************************************************
函数功能：启动回放线程
参数说明：
返回值  ：
********************************************************************************************************************************/
void CPlaybackManagerAsyn::Run()
{
	m_playLock.Lock();
	m_bRun = true;
	m_playLock.Unlock();
}


/*******************************************************************************************************************************
函数功能：回放停止
参数说明：
返回值  ：
********************************************************************************************************************************/
void CPlaybackManagerAsyn::Stop()
{
	m_playLock.Lock();
	if (m_bSync)
	{
		if(m_nPlayStatus != N_PS_STOP)
		{
			m_nPlayStatus	= N_PS_STOP;
			m_bPlayNext		= false;
			m_bPlayPrev		= false;		

			//先解锁，否则播放线程退不出来。
			m_playLock.Unlock();
			PUB_ExitThread(&m_playThread, NULL);

			//再次锁住，是因为后面还有解锁动作，需要对应。
			//同样还可以防止这期间外部再次启动播放或者其他操作。
			m_playLock.Lock();
			CLocalDevice::Instance()->PlaybackStop();
			m_syncReader.StopRead();

			m_bSetTimeFinish = true;
			m_prePlayChannels = 0;
			m_channelList.RemoveAll();
			m_bRun = false;
		}

	}
	else
	{
		for (int chnn = 0; chnn < m_chnnNum; chnn++)
		{
			m_pPlayStatus[chnn] = N_PS_STOP;
		}
		m_playLock.Unlock();

		PUB_ExitThread(&m_playThread, NULL);
		m_playLock.Lock();

		CLocalDevice::Instance()->PlaybackStop();
		m_asyncReader.StopRead();
		m_bRun = false;
	}
	m_playLock.Unlock();

}
/*******************************************************************************************************************************
函数功能：回放暂停
参数说明：chnn为需要暂停的通道。如果chnn为PLAYBACK_ALL_CHNN，则表示暂停所有的通道
brealPause-如果是真，表示是真正的暂停，这样必须把画面停住，如果是假，则是快进或者回退按上下帧按钮时候发的，这时候不停止画面，仅仅设置停止状态
返回值  ：无
********************************************************************************************************************************/
void CPlaybackManagerAsyn::Pause(unsigned long chnn,bool brealPause)
{
	m_playLock.Lock();
	if (m_bSync)
	{
		if (m_nPlayStatus != N_PS_STOP)
		{
			m_nPlayStatus = N_PS_PAUSE;
		}
	
		CLocalDevice::Instance()->PlaybackPause(true);
	}
	else
	{
		if (m_pPlayStatus[chnn] != N_PS_STOP)
		{
			m_pPlayStatus[chnn] = N_PS_PAUSE;
			m_asyncReader.SetPause(chnn, true);
			m_asyncReader.SetJumping(chnn, false);
			m_asyncReader.SetForward(chnn, true);
			m_asyncReader.SetPlayNext(chnn,false);
			m_asyncReader.SetRealPause(brealPause);
		}

		CLocalDevice::Instance()->PlaybackPause(chnn, true);
	}
	m_playLock.Unlock();
}
/*******************************************************************************************************************************
函数功能：回复到正常播放
参数说明：chnn为需要暂停的通道。如果chnn为PLAYBACK_ALL_CHNN，则表示暂停所有的通道
返回值  ：无
********************************************************************************************************************************/
void CPlaybackManagerAsyn::Resume(unsigned long chnn)
{
	m_playLock.Lock();
	if (m_bSync)
	{
		if(m_nPlayStatus != N_PS_STOP)
		{
			m_speedCtrl.bFirstFrame = true;
			m_nPlayStatus = N_PS_PLAY;

			m_playSpeed = PLAY_SPEED_1X;

			m_frameSpeedStatus = 1;
			m_forwardType = FF_TYPE_SMOOTHLY;
		}

		CLocalDevice::Instance()->PlaybackPause(false);
	}
	else
	{
		if (m_pPlayStatus[chnn] != N_PS_STOP)
		{
			m_pPlayStatus[chnn] = N_PS_PLAY;
			m_bFirstFrame[chnn] = true;
			m_asyncReader.SetPlaySpeed(chnn, m_playRate[PLAY_SPEED_1X]);
			m_asyncReader.SetPause(chnn, false);
			m_asyncReader.SetJumping(chnn, false);
			m_asyncReader.SetForward(chnn, true);
			m_asyncReader.SetPlayNext(chnn, true);
			m_pframeSpeedStatus[chnn] = 1;
			m_pforwardType[chnn] = FF_TYPE_SMOOTHLY;

			CLocalDevice::Instance()->PlaybackPause(chnn, false);
		}
	}
	m_playLock.Unlock();
}
/*******************************************************************************************************************************
函数功能：定位时间播放
参数说明：curTime 定位到的播放时间
outCH 成功定位播放的通道
chnn  定位时间播放的通道
返回值  ：成功返回true
失败返回false
********************************************************************************************************************************/
bool CPlaybackManagerAsyn::SetCurTime(unsigned long curTime, ULONGLONG & outCH, unsigned long chnn)
{
	m_playLock.Lock();

	if (m_bSync)
	{
		if(m_nPlayStatus == N_PS_PLAY)
		{
			m_nPlayStatus = N_PS_CHANGING_TIME;
			m_speedCtrl.bFirstFrame = true;


			if(MR_RET_OK == m_syncReader.SetCurTime((m_nPlayStatus != N_PS_REW), curTime, outCH))
			{
				m_nPlayStatus = N_PS_PLAY;
				m_playSpeed = PLAY_SPEED_1X; //播放状态修改为是正常，那么播放速度也要修改成1倍速
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
			if(MR_RET_OK != m_syncReader.SetCurTime((m_nPlayStatus != N_PS_REW), curTime, outCH))
			{
				m_playLock.Unlock();
				return false;
			}
			else
			{
				m_nPlayStatus = N_PS_PLAY;
				m_playSpeed = PLAY_SPEED_1X;

				CLocalDevice::Instance()->PlaybackPause(false);
			}
		}
		else
		{
			//do nothing
		}
		m_frameSpeedStatus = 1;
		m_forwardType = FF_TYPE_SMOOTHLY;

		m_bSetTimeFinish = false;

	}
	else
	{
		if(m_pPlayStatus[chnn] == N_PS_PLAY)
		{
			//定位新的时间播放的时候需要设置立即播放下一帧		
			if(CAsyncReader::MR_RET_OK == m_asyncReader.SetCurTime((m_pPlayStatus[chnn] != N_PS_REW), curTime, outCH, chnn))
			{
				m_pPlayStatus[chnn] = N_PS_PLAY;
				m_asyncReader.SetPlaySpeed(chnn, m_playRate[PLAY_SPEED_1X]); //播放状态修改为是正常，那么播放速度也要修改成1倍速
				m_asyncReader.SetPlayNext(chnn, true);
				m_bFirstFrame[chnn] = true;
			}
			else
			{
				m_playLock.Unlock();
				return false;
			}
		}
		else if(m_pPlayStatus[chnn] == N_PS_PAUSE)
		{
			if(CAsyncReader::MR_RET_OK != m_asyncReader.SetCurTime((m_pPlayStatus[chnn] != N_PS_REW), curTime, outCH, chnn))
			{
				m_playLock.Unlock();
				return false;
			}
			else
			{
				m_pPlayStatus[chnn] = N_PS_PLAY;
				m_asyncReader.SetPlaySpeed(chnn, m_playRate[PLAY_SPEED_1X]);
				m_asyncReader.SetPlayNext(chnn, true);
				m_asyncReader.SetPause(chnn, false);
				m_bFirstFrame[chnn] = true;

				CLocalDevice::Instance()->PlaybackPause(chnn, false);
			}
		}
		else
		{
			//do nothing
		}

		m_pframeSpeedStatus[chnn] = 1;
		m_pforwardType[chnn] = FF_TYPE_SMOOTHLY;
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

	m_playLock.Unlock();

	return true;
}
/*******************************************************************************************************************************
函数功能：退出回放进程
参数说明：
返回值  ：
********************************************************************************************************************************/
bool CPlaybackManagerAsyn::SetCurChannelMask(ULONGLONG ullWindowMask, ULONGLONG & outCH)
{
	m_playLock.Lock();

	if (m_bSync)
	{
		if(m_nPlayStatus == N_PS_PLAY)
		{
			m_nPlayStatus = N_PS_CHANGING_TIME;
			m_speedCtrl.bFirstFrame = true;

			if (MR_RET_OK != m_syncReader.SetCurChannel(ullWindowMask, outCH))
			{
				m_playLock.Unlock();
				return false;
			}
			else
			{
				ULONGLONG stopCH = m_prePlayChannels & ~outCH;
				//先停止设备
				CLocalDevice::Instance()->PlaybackStop(stopCH);

				//再开启设备
				CLocalDevice::Instance()->PlaybackStart(outCH);

				m_prePlayChannels = outCH;

				m_nPlayStatus = N_PS_PLAY;

				m_speedCtrl.bFirstFrame = true;
				m_playSpeed = PLAY_SPEED_1X;
			}
		}
		else if(m_nPlayStatus == N_PS_PAUSE)
		{
			m_nPlayStatus = N_PS_CHANGING_TIME;
			m_speedCtrl.bFirstFrame = true;

			if (MR_RET_OK != m_syncReader.SetCurChannel(ullWindowMask, outCH))
			{
				m_playLock.Unlock();
				return false;
			}
			else
			{
				ULONGLONG stopCH = m_prePlayChannels & ~outCH;
				//先停止设备
				CLocalDevice::Instance()->PlaybackStop(stopCH);

				//再开启设备
				CLocalDevice::Instance()->PlaybackStart(outCH);

				m_prePlayChannels |= outCH;

				m_nPlayStatus = N_PS_PAUSE;
				
				CLocalDevice::Instance()->PlaybackPause(true);

				m_speedCtrl.bFirstFrame = true;
			}
		}
		else
		{
			//do nothing
			ULONGLONG outCH = 0;
			m_syncReader.SetCurChannel(ullWindowMask, outCH);
		}

		m_frameSpeedStatus = 1;
		m_forwardType = FF_TYPE_SMOOTHLY;
		m_bSetTimeFinish = false;
	}
	else
	{
		if (CAsyncReader::MR_RET_OK != m_asyncReader.SetCurChannel(ullWindowMask, outCH))
		{
			m_playLock.Unlock();
			return false;
		}
		else
		{
			ULONGLONG stopCH = m_prePlayChannels & ~outCH;
			//先停止设备
			CLocalDevice::Instance()->PlaybackStop(stopCH);

			//再开启设备
			CLocalDevice::Instance()->PlaybackStart(outCH);

			m_prePlayChannels = outCH;
			for (int chnn = 0; chnn <  m_chnnNum; chnn++)
			{
				if (m_pPlayStatus[chnn] == N_PS_PLAY)
				{
					m_pPlayStatus[chnn] = N_PS_PLAY;
				}
				else if(m_pPlayStatus[chnn] == N_PS_PAUSE)
				{
					m_pPlayStatus[chnn] = N_PS_PAUSE;
				}
				m_asyncReader.SetPlaySpeed(chnn, m_playRate[PLAY_SPEED_1X]);
				m_asyncReader.SetPlayNext(chnn, true);
				m_pframeSpeedStatus[chnn] = 1;
				m_pforwardType[chnn] = FF_TYPE_SMOOTHLY;

				CLocalDevice::Instance()->PlaybackPause(chnn, true);
			}

		}
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

	m_playLock.Unlock();

	return true;
}
/*******************************************************************************************************************************
函数功能：回放控制
参数说明：bForward 向前播放为true，向后播放时false
playSpeed 播放速度
chnn 要控制的通道
返回值  ：无
********************************************************************************************************************************/
void CPlaybackManagerAsyn::SetPlaySpeed(bool bForward, PLAY_SPEED playSpeed, unsigned long chnn)
{

	m_playLock.Lock();

	if (m_bSync)
	{
		if((m_nPlayStatus == N_PS_STOP) || (m_nPlayStatus == N_PS_PAUSE))
		{
			//回放已停止,不进行任何操作
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

				//如果是128，表示产品不受这个约束。
				//就是采用默认，不管几路回放，最大只能做到4倍数逐帧解码。
				if (128 == m_maxFFSpeed)
				{
					if (m_frameSpeedStatus <= 4)
					{
						m_forwardType = FF_TYPE_SMOOTHLY;					
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

	}
	else
	{
		if((m_pPlayStatus[chnn] == N_PS_STOP) || (m_pPlayStatus[chnn] == N_PS_PAUSE))
		{
			//回放已停止,不进行任何操作
		}
		else
		{
			m_bFirstFrame[chnn] = true;
			m_asyncReader.SetPlayNext(chnn, true);
			if(!bForward)
			{
				m_pPlayStatus[chnn] = N_PS_REW;
				m_asyncReader.SetForward(chnn, false);
				m_asyncReader.SetJumping(chnn, true);
				m_asyncReader.SetPlaySpeed(chnn, m_playRate[playSpeed]);
				assert(playSpeed >= PLAY_SPEED_8X);
			}
			else if(playSpeed != PLAY_SPEED_1X)
			{
				assert(playSpeed <= PLAY_SPEED_16X);
				m_pPlayStatus[chnn] = N_PS_FF;
				m_asyncReader.SetPlaySpeed(chnn, m_playRate[playSpeed]);
				switch(playSpeed)
				{
				case PLAY_SPEED_2X:
					m_pframeSpeedStatus[chnn] = 2;
					break;
				case PLAY_SPEED_4X:
					m_pframeSpeedStatus[chnn] = 4;
					break;
				case PLAY_SPEED_8X:
					m_pframeSpeedStatus[chnn] = 8;
					break;
				case PLAY_SPEED_16X:
					m_pframeSpeedStatus[chnn] = 16;
					break;
				default:
					m_pframeSpeedStatus[chnn] = 1;
					break;
				}

				//如果是128，表示产品不受这个约束。
				//就是采用默认，不管几路回放，最大只能做到4倍数逐帧解码。
				if (128 == m_maxFFSpeed)
				{
					if (m_pframeSpeedStatus[chnn] <= 4)
					{
						m_pforwardType[chnn] = FF_TYPE_SMOOTHLY;	
						m_asyncReader.SetJumping(chnn , false);
					}
					else
					{
						m_pforwardType[chnn] = FF_TYPE_JUMPING;
						m_asyncReader.SetJumping(chnn, true);
					}
				}
				else
				{
					if (playSpeed <= PLAY_SPEED_1_2X)
					{
						m_pforwardType[chnn] = FF_TYPE_SMOOTHLY;
						m_asyncReader.SetJumping(chnn, false);
					}
					else
					{
						if (m_maxFFSpeed < (m_pframeSpeedStatus[chnn] * m_activeChnnNum))
						{
							m_pforwardType[chnn] = FF_TYPE_JUMPING;
							m_asyncReader.SetJumping(chnn, true);
						}
						else
						{
							m_pforwardType[chnn] = FF_TYPE_SMOOTHLY;
							m_asyncReader.SetJumping(chnn, false);
						}
					}	
				}
			}
			else
			{
				m_pPlayStatus[chnn] = N_PS_PLAY;
				m_bFirstFrame[chnn] = true;
				m_pframeSpeedStatus[chnn] = 1;
				m_pforwardType[chnn] = FF_TYPE_SMOOTHLY;
				m_asyncReader.SetPlaySpeed(chnn, m_playRate[PLAY_SPEED_1X]);
				m_asyncReader.SetJumping(chnn, false);
				m_asyncReader.SetForward(chnn, true);
			}
		}

	}

	m_playLock.Unlock();
}
/*******************************************************************************************************************************
函数功能：单帧播放时播放下一帧
参数说明：bForward 向前还是向后播放
chnn 为播放通道
返回值  ：无
********************************************************************************************************************************/
void CPlaybackManagerAsyn::PlayNextFrame(bool bForward, unsigned long chnn)
{
	m_playLock.Lock();
	if(m_bSync)
	{
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
	else
	{
		if (m_pPlayStatus[chnn] == N_PS_PAUSE)
		{
			m_asyncReader.SetPlayNext(chnn, true);
		}
	}
	m_playLock.Unlock();
}

/*******************************************************************************************************************************
函数功能：设置音频通道
参数说明：channel 通道号
返回值  ：无
********************************************************************************************************************************/
void CPlaybackManagerAsyn::SetAudioChannel(unsigned long channel)
{
	printf("%s,%d,chnn:%d\n",__FILE__,__LINE__,channel);
	m_audioChannel = channel;
}
/*******************************************************************************************************************************
函数功能：回放线程
参数说明：
返回值  ：无
********************************************************************************************************************************/
RESULT WINAPI CPlaybackManagerAsyn::SyncPlayProc(void *pParam)
{
#ifdef __ENVIRONMENT_LINUX__
	pid_t pid = getpid();
	printf("%s, %d, pid = %d\n", __FILE__, __LINE__, pid);
#endif

	CPlaybackManagerAsyn *pThis = (CPlaybackManagerAsyn *)pParam;
	pThis->SyncPlayProc();

	return 0;
}

RESULT WINAPI CPlaybackManagerAsyn::AsyncPlayProc(void *pParam)
{
#ifdef __ENVIRONMENT_LINUX__
	pid_t pid = getpid();
	pid_t tid = syscall(__NR_gettid);
	printf("%s, %d, pid = %d, tid=%d\n", __FILE__, __LINE__, pid, tid);
#endif

	CPlaybackManagerAsyn *pThis = (CPlaybackManagerAsyn *)pParam;
	pThis->AsyncPlayProc();

	return 0;
}
/*******************************************************************************************************************************
函数功能：多通道异步回放线程
参数说明：无
返回值  ：无
********************************************************************************************************************************/
void CPlaybackManagerAsyn::AsyncPlayProc()
{
	memset(&m_frameInfo, 0, sizeof(FRAME_INFO_EX));
	unsigned long frameTypeTemp = FRAME_TYPE_NONE;
	int time = 0;
	unsigned long chn;

	while (true)
	{
		m_playLock.Lock();

		if (!m_bRun)
		{
			m_playLock.Unlock();
			PUB_Sleep(10);
			continue;
		}

		bool bStop = true;
		for (int chnn = 0; chnn < m_chnnNum; chnn++)
		{
			if (m_pPlayStatus[chnn] != N_PS_STOP)
			{
				bStop = false;
			}
		}

		if (bStop)
		{
			m_playLock.Unlock();
			break;
		}

		long ret;
		unsigned long currReaderID = 0;
		unsigned long currReaderIDTmp = 0;
		bool bneedchange=false;
		ret = m_asyncReader.GetNextFrameType(m_frameInfo, currReaderIDTmp,bneedchange);
		if(ret == CAsyncReader::MR_RET_OK)
		{

		}
		else if(ret == CAsyncReader::MR_RET_ALLCHNN_PAUSE)
		{
			m_playLock.Unlock();
			continue;
		}
		else
		{
			m_pMsgCallBack(m_pCallBackObj, 2, NULL, NULL);
			m_playLock.Unlock();
			break;
		}


		frameTypeTemp = m_frameInfo.frameType;
		{
			//（2010-01-20 15:18:00 YSW）
			//把获取内存统一到LocaclDevice内部去。发现是视频或者音频格式，则申请一片内存
			//其他都是从解码器获取内存地址。
			m_frameInfo.channel = CHANNEL_ID(m_frameInfo.channel);
			//通过设备接口获取数据缓冲区，有可能获取不成功，则限定其次数（MAX_GET_BUF_NUM）
			while(!CLocalDevice::Instance()->PlaybackGetBuf(&m_frameInfo) && (time < MAX_GET_BUF_NUM))
			{
				time++;				
				PUB_Sleep(1);
			}			
			//等号成立，则意味着获取缓冲区失败，则需要退出回放。 
			if(time == MAX_GET_BUF_NUM)
			{

				m_pMsgCallBack(m_pCallBackObj, 2, NULL, NULL);
				m_playLock.Unlock();
				printf("%s,%d,not get buffer.time:%d,chnn=%d\n",__FILE__,__LINE__,time,m_frameInfo.channel);				

				RUN_LOG(RUN_LOG_ITEM_PB_GET_FRAME_FAIL, CRunLog::RUN_Num("CH:", m_frameInfo.channel), CRunLog::RUN_Num("Frame Type:", m_frameInfo.frameType), "Get Buffer fail");

				break;
			}
		}

		ret = m_asyncReader.GetFrame(m_frameInfo, currReaderID);
		//修改问题：在调用GetNextFrameType函数时，得到是音频帧，但是在GetNextFrame或
		//GetKeyFrame时得到的是视频帧。
		//重新条件：播放音频时，点击8（16,32）倍数的快进或快退。
		if ((frameTypeTemp != m_frameInfo.frameType) || (currReaderID != currReaderIDTmp))
		{
			printf("#############################################chnn:%d,frameType error,%d,%d. keyframe:%d\n", m_frameInfo.channel, frameTypeTemp, m_frameInfo.frameType, m_frameInfo.keyFrame);
			m_frameInfo.frameType = FRAME_TYPE_NONE;
			m_frameInfo.frameIndex = 1;
			CLocalDevice::Instance()->PlaybackSetBuf(&m_frameInfo);
			m_playLock.Unlock();
			continue;
		}

		//读取数据成功
		if (CAsyncReader::MR_RET_OK == ret)
		{

			if(bneedchange)//修改提前是因为获取的frametype有可能是音频，此时也需要设置BaseShowTime
			{
				m_asyncReader.SetBaseShowTime(currReaderID, GetCurrTime());
			}
			chn = CHANNEL_ID(m_frameInfo.channel);//得到其通道号，数字和模拟统一编号的通道

			//如果是视频和音频格式数据
			if(m_frameInfo.frameType == FRAME_TYPE_VIDEO_FORMAT)
			{
				m_frameInfo.frameIndex = 1;
				CLocalDevice::Instance()->PlaybackSetBuf(&m_frameInfo);
				m_asyncReader.SetPlayNext(chn, true);
				m_playLock.Unlock();
				continue;
			}
			else if(m_frameInfo.frameType == FRAME_TYPE_AUDIO_FORMAT)
			{
				m_frameInfo.frameIndex = 1;
				CLocalDevice::Instance()->PlaybackSetBuf(&m_frameInfo);
				m_asyncReader.SetPlayNext(chn, true);
				m_playLock.Unlock();
				continue;
			}
			else if(m_frameInfo.frameType == FRAME_TYPE_VIDEO)
			{
#ifdef __ENVIRONMENT_WIN32__
				m_frameInfo.frameAttrib = PLAY_FRAME_SHOW;
#else
				m_frameInfo.frameAttrib = PLAY_FRAME_NO_SHOW;
#endif
				if (m_bFirstFrame[chn])
				{

					m_asyncReader.SetBaseFrameTime(currReaderID, m_frameInfo.time);
					m_asyncReader.SetBaseShowTime(currReaderID, GetCurrTime());
					m_asyncReader.SetLastReadFrameTime(currReaderID,m_frameInfo.time);
					m_bFirstFrame[chn] = false;
				}
				else
				{

					LONGLONG diffTime = 0;
					do
					{
						diffTime = m_asyncReader.GetShowTime() - GetCurrTime();
						if ((0 < diffTime) && (diffTime < 3000000))
						{
							PUB_Sleep(1);
						}
					}while(diffTime > 0);
					m_pMsgCallBack(m_pCallBackObj, 1, &m_frameInfo.time,  &m_frameInfo.channel);
				}

				m_asyncReader.SetLastReadFrameTime(currReaderID,m_frameInfo.time);
				m_frameInfo.frameAttrib = PLAY_FRAME_SHOW;				
				m_frameInfo.frameIndex = (FF_TYPE_JUMPING == m_pforwardType[chn]) ? 1 : m_pframeSpeedStatus[chn];;
				m_playLock.Unlock();
				CLocalDevice::Instance()->PlaybackSetBuf(&m_frameInfo);
			}
			else if(m_frameInfo.frameType == FRAME_TYPE_AUDIO)
			{
				if ((m_pPlayStatus[chn] != N_PS_PLAY) || (m_audioChannel != chn))
				{
					//由于每次读取数据之前已经获取到了缓冲区，且并不知道即将来的帧的类型，所以
					//如果不是当前回放地通道，则需要进行过滤。
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
			//没有数据可回放，宣告回放结束
			m_frameInfo.frameIndex = 1;
			CLocalDevice::Instance()->PlaybackSetBuf(&m_frameInfo);

			m_pMsgCallBack(m_pCallBackObj, 2, NULL, NULL);
			m_playLock.Unlock();

			break;
		}
	}
}

/*******************************************************************************************************************************
函数功能：多通道异步回放线程
参数说明：无
返回值  ：无
********************************************************************************************************************************/
void CPlaybackManagerAsyn::SyncPlayProc()
{
	memset(&m_frameInfo, 0, sizeof(FRAME_INFO_EX));

	LONGLONG lastShowTime = 0;
	unsigned long MultiCH = 0, chnn = 0;
	unsigned long frameTypeTemp = FRAME_TYPE_NONE;

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
			break;
		}

		if(m_nPlayStatus == N_PS_CHANGING_TIME)
		{
			m_playLock.Unlock();
			PUB_Sleep(10);
			continue;
		}

		if(m_nPlayStatus == N_PS_PAUSE)
		{
			if(!m_bPlayNext && !m_bPlayPrev && m_bSetTimeFinish)		//如果没有按下一帧按钮
			{
				m_playLock.Unlock();
				PUB_Sleep(200);
				continue;
			}
		}

		unsigned long time = 0;
		memset(&m_frameInfo, 0, sizeof(m_frameInfo));

		//获取下一帧的帧类型;
		long ret;
		if((m_nPlayStatus == N_PS_REW) && (m_playSpeed >= PLAY_SPEED_8X))
		{
			ret = m_syncReader.GetNextFrameType(m_frameInfo, false);
		}
		else
		{
			ret = m_syncReader.GetNextFrameType(m_frameInfo, true);
		}

		if(MR_RET_OK != ret)
		{
			m_pMsgCallBack(m_pCallBackObj, 2, NULL, NULL);
			m_playLock.Unlock();
			break;
		}

		frameTypeTemp = m_frameInfo.frameType;

		{
			//（2010-01-20 15:18:00 YSW）
			//把获取内存统一到LocaclDevice内部去。发现是视频或者音频格式，则申请一片内存
			//其他都是从解码器获取内存地址。
			m_frameInfo.channel = CHANNEL_ID(m_frameInfo.channel);

			//通过设备接口获取数据缓冲区，有可能获取不成功，则限定其次数（MAX_GET_BUF_NUM）
			while(!CLocalDevice::Instance()->PlaybackGetBuf(&m_frameInfo) && (time < MAX_GET_BUF_NUM))
			{
				time++;	

				PUB_Sleep(1);
			}			

			//等号成立，则意味着获取缓冲区失败，则需要退出回放。
			if(time == MAX_GET_BUF_NUM)
			{
				m_pMsgCallBack(m_pCallBackObj, 2, NULL, NULL);
				m_playLock.Unlock();

				printf("%s,%d,not get buffer.time:%d\n",__FILE__,__LINE__,time);				

				RUN_LOG(RUN_LOG_ITEM_PB_GET_FRAME_FAIL, CRunLog::RUN_Num("CH:", m_frameInfo.channel), CRunLog::RUN_Num("Frame Type:", m_frameInfo.frameType), "Get Buffer fail");

				break;
			}
		}

		//根据播放状态来调用不同的接口读取数据，8倍数以上则跳关键帧，其他的则是走正常的帧序列。
		if((m_nPlayStatus == N_PS_FF) && (FF_TYPE_JUMPING == m_forwardType))
		{
			ret = m_syncReader.GetKeyFrame(m_frameInfo, 1, true);
		}
		else if((m_nPlayStatus == N_PS_REW) && (m_playSpeed >= PLAY_SPEED_8X))
		{
			ret = m_syncReader.GetKeyFrame(m_frameInfo, 1, false);
		}
		else if((m_nPlayStatus == N_PS_PLAY) || (m_nPlayStatus == N_PS_FF) || (m_nPlayStatus == N_PS_REW))
		{
			ret = m_syncReader.GetNextFrame(m_frameInfo);
		}
		else if ((m_nPlayStatus == N_PS_PAUSE) && (m_bPlayNext || m_bPlayPrev || !m_bSetTimeFinish))
		{
			ret = m_syncReader.GetNextFrame(m_frameInfo);
		}
		else
		{
			RUN_LOG(RUN_LOG_ITEM_PB_GET_FRAME_FAIL, CRunLog::RUN_Num("Play Status:", m_nPlayStatus), CRunLog::RUN_Num("Play Speed:", m_playSpeed), "Get Next Frame fail");
		}

		//修改问题：在调用GetNextFrameType函数时，得到是音频帧，但是在GetNextFrame或
		//GetKeyFrame时得到的是视频帧。
		//重新条件：播放音频时，点击8（16,32）倍数的快进或快退。
		if (frameTypeTemp != m_frameInfo.frameType)
		{
			printf("#############################################chnn:%d,frameType error,%d,%d. keyframe:%d\n", m_frameInfo.channel, frameTypeTemp, m_frameInfo.frameType, m_frameInfo.keyFrame);
			m_frameInfo.frameType = FRAME_TYPE_NONE;
			m_frameInfo.frameIndex = 1;
			CLocalDevice::Instance()->PlaybackSetBuf(&m_frameInfo);
			m_playLock.Unlock();
			continue;
		}

		//读取数据成功
		if(MR_RET_OK == ret)
		{
			chnn = CHANNEL_ID(m_frameInfo.channel);//得到其通道号，数字和模拟统一编号的通道

			//如果是视频和音频格式数据
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
						if (m_playRate[m_playSpeed] >> 8)
						{
							diffTime = static_cast<LONGLONG>((m_speedCtrl.lastFrameTime - m_frameInfo.time) * (m_playRate[m_playSpeed] & 0xff));
						}
						else
						{
							diffTime = static_cast<LONGLONG>((m_speedCtrl.lastFrameTime - m_frameInfo.time) / m_playRate[m_playSpeed]);
						}

					}
					else
					{
						if (m_playRate[m_playSpeed] >> 8)
						{
							diffTime = static_cast<LONGLONG>((m_frameInfo.time - m_speedCtrl.lastFrameTime) * (m_playRate[m_playSpeed] & 0xff));
						}
						else
						{
							diffTime = static_cast<LONGLONG>((m_frameInfo.time - m_speedCtrl.lastFrameTime) / m_playRate[m_playSpeed]);
						}
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

				m_frameInfo.frameIndex = (FF_TYPE_JUMPING == m_forwardType) ? 1 : m_frameSpeedStatus;
				CLocalDevice::Instance()->PlaybackSetBuf(&m_frameInfo);
				m_playLock.Unlock();
			}
			else if(m_frameInfo.frameType == FRAME_TYPE_AUDIO)
			{
				if ((m_nPlayStatus != N_PS_PLAY) || (m_audioChannel != chnn))
				{
					//由于每次读取数据之前已经获取到了缓冲区，且并不知道即将来的帧的类型，所以
					//如果不是当前回放地通道，则需要进行过滤。
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
			//没有数据可回放，宣告回放结束
			m_frameInfo.frameIndex = 1;
			CLocalDevice::Instance()->PlaybackSetBuf(&m_frameInfo);

			m_pMsgCallBack(m_pCallBackObj, 2, NULL, NULL);
			m_playLock.Unlock();
			break;
		}
	}

#ifdef __ENVIRONMENT_LINUX__
	RUN_LOG(RUN_LOG_ITEM_PB_EXIT, {}, {}, "Exit Playback Proc");
#endif

}
#else //__CHIP3531__
CPlaybackManagerAsyn::CPlaybackManagerAsyn()
{
	m_pFileName			= NULL;
	m_pPlayStatus		= NULL;
	m_pforwardType		= NULL;
	m_pframeSpeedStatus = NULL;
	m_playThread		= PUB_THREAD_ID_NOINIT;
	memset(&m_frameInfo, 0, sizeof(FRAME_INFO_EX));
	m_prePlayChannels	= 0;
	m_audioChannel		= INVALID_INDEX(unsigned long);
	m_bRun = false;
	m_bSync = true;
	m_bFrameReadFromFile = false;
}

CPlaybackManagerAsyn::~CPlaybackManagerAsyn()
{

}

/*******************************************************************************************************************************
函数功能：回放功能初始化
参数说明：chnnNum 回放通道数
		  videoOutputNum 视频输出通道数
		  maxFFSpeed 最大帧率速度
		  pFilePath 回放录像文件路径
		  pReclogManEx 管理回放录像日志的类
		  diskOwnerIndex 录像所在的磁盘编号
		  bSync 是否同步播放录像
返回值  ：成功为true 
		  失败为false
********************************************************************************************************************************/
bool CPlaybackManagerAsyn::Initial(unsigned char chnnNum, unsigned char videoOutputNum, unsigned char maxFFSpeed, const char *pFilePath, CReclogManEX *pReclogManEx, unsigned long diskOwnerIndex, bool bSync)
{
	m_chnnNum = chnnNum;
	m_videoOutputNum = videoOutputNum;
	m_maxFFSpeed = maxFFSpeed;
	m_activeChnnNum = 0;
	m_bSync = bSync;

	if (pFilePath != NULL)
	{
		m_pFileName = new char[256];
		strcpy(m_pFileName, pFilePath);
	}
	
	if (!bSync)
	{
		m_asyncReader.Initial(m_chnnNum, pReclogManEx, diskOwnerIndex);

		m_pPlayStatus = new PLAY_STATE[m_chnnNum];
		m_pforwardType = new unsigned char[m_chnnNum];
		m_pframeSpeedStatus = new unsigned char[m_chnnNum];

		for (int chnn = 0; chnn < m_chnnNum; chnn ++)
		{
			m_pPlayStatus[chnn] = N_PS_STOP;
			m_pforwardType[chnn] = FF_TYPE_SMOOTHLY;
			m_pframeSpeedStatus[chnn] = 1;
			m_bFirstFrame[chnn] =  false;
		}
	}
	else
	{
		m_syncReader.Initial(m_chnnNum, pReclogManEx, diskOwnerIndex);

		m_nPlayStatus = N_PS_STOP;
		m_forwardType = FF_TYPE_SMOOTHLY;
		m_frameSpeedStatus = 1;
	}

	m_playRate[5] = 1;
	for(int i = 0; i < 5; i++)
	{
		m_playRate[i+6] = m_playRate[i+5] * 2;
		m_playRate[4-i] = m_playRate[i+6] + (0x1 << 8);
		printf("####%s,%d,m_playRate[%d] = %f\n",__FILE__,__LINE__,i,m_playRate[i]);
	}

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

/*******************************************************************************************************************************
函数功能：按启动时间回放
参数说明：startTime 录像文件开始时间
		  endTime 录像文件结束时间
		  curTime 当前播放时间
		  chnnBits 需要播发通道掩码
		  callBack 回调函数
		  pCallBackObj 回调指针
		  outCH 成功播放通道的掩码
返回值  ：成功为true 
		  失败为false
********************************************************************************************************************************/
bool CPlaybackManagerAsyn::Play(unsigned long startTime, unsigned long endTime, unsigned long curTime, \
		  ULONGLONG chnnBits,MESSAGE_CALLBACK callBack, void *pCallBackObj, ULONGLONG &outCH)
{
	CHANNEL_LIST channelList;
	m_pMsgCallBack = callBack;
	m_pCallBackObj = pCallBackObj;

	channelList.RemoveAll();
	for (unsigned long chnn = 0; chnn < m_chnnNum; chnn ++)
	{
		if (chnnBits & ((ULONGLONG)0x01 << chnn))
		{
			channelList.AddTail(chnn);
		}
	}
	
	if (m_bSync)
	{
		m_bPlayNext = false;
		m_bPlayPrev = false;
		m_playSpeed = PLAY_SPEED_1X;
		m_frameSpeedStatus = 1;
		m_forwardType = FF_TYPE_SMOOTHLY;
		m_speedCtrl.bFirstFrame = true;

		if(MR_RET_OK == m_syncReader.StartRead(startTime, endTime, channelList, outCH, m_pFileName))
		{
			//启动设备
			m_bFrameReadFromFile = false;
			if (CLocalDevice::Instance()->PlaybackStart(outCH))
			{
				m_nPlayStatus = N_PS_PLAY;
				m_playThread = PUB_CreateThread(SyncPlayProc, this, NULL);

				if (PUB_CREATE_THREAD_FAIL != m_playThread)
				{
					m_prePlayChannels = outCH;

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

					return true;
				}
			}
		}
	}
	else
	{
		if (CAsyncReader::MR_RET_OK == m_asyncReader.StartRead(startTime, endTime, channelList, outCH, m_pFileName))
		{
			m_bFrameReadFromFile = false;
			if (CLocalDevice::Instance()->PlaybackStart(outCH))
			{
				for (unsigned long chnn = 0; chnn < m_chnnNum; chnn++)
				{
					if (outCH & ((ULONGLONG)0x01 << chnn))
					{
						m_pPlayStatus[chnn] = N_PS_PLAY;
						m_bFirstFrame[chnn] = true;
						m_asyncReader.SetForward(chnn, true);
						m_asyncReader.SetJumping(chnn, false);
						m_asyncReader.SetPlayNext(chnn, false);
					}
				}
				m_playThread = PUB_CreateThread(AsyncPlayProc, this, NULL);
				if (PUB_THREAD_ID_NOINIT != m_playThread)
				{
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
					return true;
				}
			}
		}
	}
	
	return false;
}

/*******************************************************************************************************************************
函数功能：退出回放进程
参数说明：
返回值  ：
********************************************************************************************************************************/
void CPlaybackManagerAsyn::Quit()
{
	if (m_bSync)
	{
		m_syncReader.Quit();
	}
	else
	{
		m_asyncReader.Quit();

		if (m_pPlayStatus != NULL)
		{
			delete [] m_pPlayStatus;
		}
		if (m_pforwardType != NULL)
		{
			delete [] m_pforwardType;
		}
		if (m_pframeSpeedStatus != NULL)
		{
			delete [] m_pframeSpeedStatus;
		}
	}

	if (m_pFileName != NULL)
	{
		delete [] m_pFileName;
	}

	m_audioChannel = INVALID_INDEX(unsigned long);
}


/*******************************************************************************************************************************
函数功能：启动回放线程
参数说明：
返回值  ：
********************************************************************************************************************************/
void CPlaybackManagerAsyn::Run()
{
	m_playLock.Lock();
	m_bRun = true;
	m_bFrameReadFromFile = false;
	m_playLock.UnLock();
}


/*******************************************************************************************************************************
函数功能：回放停止
参数说明：
返回值  ：
********************************************************************************************************************************/
void CPlaybackManagerAsyn::Stop()
{
	m_playLock.Lock();
	if (m_bSync)
	{
		if(m_nPlayStatus != N_PS_STOP)
		{
			m_nPlayStatus	= N_PS_STOP;
			m_bPlayNext		= false;
			m_bPlayPrev		= false;		

			//先解锁，否则播放线程退不出来。
			m_playLock.UnLock();
			PUB_ExitThread(&m_playThread, NULL);

			//再次锁住，是因为后面还有解锁动作，需要对应。
			//同样还可以防止这期间外部再次启动播放或者其他操作。
			m_playLock.Lock();
			CLocalDevice::Instance()->PlaybackStop();
			m_syncReader.StopRead();

			m_bSetTimeFinish = true;
			m_prePlayChannels = 0;
			m_channelList.RemoveAll();
			m_bRun = false;
		}

	}
	else
	{
		for (int chnn = 0; chnn < m_chnnNum; chnn++)
		{
			m_pPlayStatus[chnn] = N_PS_STOP;
		}
		m_playLock.UnLock();

		PUB_ExitThread(&m_playThread, NULL);
		m_playLock.Lock();

		CLocalDevice::Instance()->PlaybackStop();
		m_asyncReader.StopRead();
		m_bRun = false;
	}
	m_playLock.UnLock();

}
/*******************************************************************************************************************************
函数功能：回放暂停
参数说明：chnn为需要暂停的通道。如果chnn为PLAYBACK_ALL_CHNN，则表示暂停所有的通道
		 brealPause-如果是真，表示是真正的暂停，这样必须把画面停住，如果是假，则是快进或者回退按上下帧按钮时候发的，这时候不停止画面，仅仅设置停止状态
返回值  ：无
********************************************************************************************************************************/
void CPlaybackManagerAsyn::Pause(unsigned long chnn,bool brealPause)
{
	m_playLock.Lock();
	if (m_bSync)
	{
		if (m_nPlayStatus != N_PS_STOP)
		{
			m_nPlayStatus = N_PS_PAUSE;
		}
	}
	else
	{
		if (m_pPlayStatus[chnn] != N_PS_STOP)
		{
			m_pPlayStatus[chnn] = N_PS_PAUSE;
			m_asyncReader.SetPause(chnn, true);
			m_asyncReader.SetJumping(chnn, false);
			m_asyncReader.SetForward(chnn, true);
			m_asyncReader.SetPlayNext(chnn,false);
			m_asyncReader.SetRealPause(brealPause);
		}
	}
	m_playLock.UnLock();
}
/*******************************************************************************************************************************
函数功能：回复到正常播放
参数说明：chnn为需要暂停的通道。如果chnn为PLAYBACK_ALL_CHNN，则表示暂停所有的通道
返回值  ：无
********************************************************************************************************************************/
void CPlaybackManagerAsyn::Resume(unsigned long chnn)
{
	m_playLock.Lock();
	if (m_bSync)
	{
		if(m_nPlayStatus != N_PS_STOP)
		{
			m_speedCtrl.bFirstFrame = true;
			m_nPlayStatus = N_PS_PLAY;

			m_playSpeed = PLAY_SPEED_1X;

			m_frameSpeedStatus = 1;
			m_forwardType = FF_TYPE_SMOOTHLY;
		}
	}
	else
	{
		if (m_pPlayStatus[chnn] != N_PS_STOP)
		{
			m_pPlayStatus[chnn] = N_PS_PLAY;
			m_bFirstFrame[chnn] = true;
			m_asyncReader.SetPlaySpeed(chnn, m_playRate[PLAY_SPEED_1X]);
			m_asyncReader.SetPause(chnn, false);
			m_asyncReader.SetJumping(chnn, false);
			m_asyncReader.SetForward(chnn, true);
			m_asyncReader.SetPlayNext(chnn, true);
			m_pframeSpeedStatus[chnn] = 1;
			m_pforwardType[chnn] = FF_TYPE_SMOOTHLY;
		}
	}
	m_playLock.UnLock();
}
/*******************************************************************************************************************************
函数功能：定位时间播放
参数说明：curTime 定位到的播放时间
		  outCH 成功定位播放的通道
		  chnn  定位时间播放的通道
返回值  ：成功返回true
          失败返回false
********************************************************************************************************************************/
bool CPlaybackManagerAsyn::SetCurTime(unsigned long curTime, ULONGLONG & outCH, unsigned long chnn)
{
	m_playLock.Lock();

	if (m_bSync)
	{
		if(m_nPlayStatus == N_PS_PLAY)
		{
			m_nPlayStatus = N_PS_CHANGING_TIME;
			m_speedCtrl.bFirstFrame = true;

			ReleaseReadedBuf();

			if(MR_RET_OK == m_syncReader.SetCurTime((m_nPlayStatus != N_PS_REW), curTime, outCH))
			{
				m_nPlayStatus = N_PS_PLAY;
				m_playSpeed = PLAY_SPEED_1X; //播放状态修改为是正常，那么播放速度也要修改成1倍速
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
			m_speedCtrl.bFirstFrame = true;
			ReleaseReadedBuf();
			if(MR_RET_OK != m_syncReader.SetCurTime((m_nPlayStatus != N_PS_REW), curTime, outCH))
			{
				m_playLock.UnLock();
				return false;
			}
			else
			{
				m_nPlayStatus = N_PS_PLAY;
				m_playSpeed = PLAY_SPEED_1X;
			}
		}
		else
		{
			//do nothing
		}
		m_frameSpeedStatus = 1;
		m_forwardType = FF_TYPE_SMOOTHLY;

		m_bSetTimeFinish = false;

	}
	else
	{
		if(m_pPlayStatus[chnn] == N_PS_PLAY)
		{
			ReleaseReadedBuf();
			//定位新的时间播放的时候需要设置立即播放下一帧		
			if(CAsyncReader::MR_RET_OK == m_asyncReader.SetCurTime((m_pPlayStatus[chnn] != N_PS_REW), curTime, outCH, chnn))
			{
				m_pPlayStatus[chnn] = N_PS_PLAY;
				m_asyncReader.SetPlaySpeed(chnn, m_playRate[PLAY_SPEED_1X]); //播放状态修改为是正常，那么播放速度也要修改成1倍速
				m_asyncReader.SetPlayNext(chnn, true);
				m_bFirstFrame[chnn] = true;
			}
			else
			{
				m_playLock.UnLock();
				return false;
			}
		}
		else if(m_pPlayStatus[chnn] == N_PS_PAUSE)
		{
			ReleaseReadedBuf();
			if(CAsyncReader::MR_RET_OK != m_asyncReader.SetCurTime((m_pPlayStatus[chnn] != N_PS_REW), curTime, outCH, chnn))
			{
				m_playLock.UnLock();
				return false;
			}
			else
			{
				m_pPlayStatus[chnn] = N_PS_PLAY;
				m_asyncReader.SetPlaySpeed(chnn, m_playRate[PLAY_SPEED_1X]);
				m_asyncReader.SetPlayNext(chnn, true);
				m_asyncReader.SetPause(chnn, false);
				m_bFirstFrame[chnn] = true;
			}
		}
		else
		{
			//do nothing
		}

		m_pframeSpeedStatus[chnn] = 1;
		m_pforwardType[chnn] = FF_TYPE_SMOOTHLY;
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

	m_playLock.UnLock();

	return true;
}
/*******************************************************************************************************************************
函数功能：退出回放进程
参数说明：
返回值  ：
********************************************************************************************************************************/
bool CPlaybackManagerAsyn::SetCurChannelMask(ULONGLONG ullWindowMask, ULONGLONG & outCH)
{
	m_playLock.Lock();

	if (m_bSync)
	{
		if(m_nPlayStatus == N_PS_PLAY)
		{
			ReleaseReadedBuf();
			m_nPlayStatus = N_PS_CHANGING_TIME;
			m_speedCtrl.bFirstFrame = true;

			if (MR_RET_OK != m_syncReader.SetCurChannel(ullWindowMask, outCH))
			{
				m_playLock.UnLock();
				return false;
			}
			else
			{
				ReleaseReadedBuf();
				ULONGLONG stopCH = m_prePlayChannels & ~outCH;
				//先停止设备
				CLocalDevice::Instance()->PlaybackStop(stopCH);

				//再开启设备
				CLocalDevice::Instance()->PlaybackStart(outCH);

				m_prePlayChannels = outCH;

				m_nPlayStatus = N_PS_PLAY;

				m_speedCtrl.bFirstFrame = true;
				m_playSpeed = PLAY_SPEED_1X;
			}
		}
		else if(m_nPlayStatus == N_PS_PAUSE)
		{
			ReleaseReadedBuf();
			m_nPlayStatus = N_PS_CHANGING_TIME;
			m_speedCtrl.bFirstFrame = true;

			if (MR_RET_OK != m_syncReader.SetCurChannel(ullWindowMask, outCH))
			{
				m_playLock.UnLock();
				return false;
			}
			else
			{
				ReleaseReadedBuf();
				ULONGLONG stopCH = m_prePlayChannels & ~outCH;
				//先停止设备
				CLocalDevice::Instance()->PlaybackStop(stopCH);

				//再开启设备
				CLocalDevice::Instance()->PlaybackStart(outCH);

				m_prePlayChannels |= outCH;

				m_nPlayStatus = N_PS_PAUSE;

				m_speedCtrl.bFirstFrame = true;
			}
		}
		else
		{
			//do nothing
			ULONGLONG outCH = 0;
			m_syncReader.SetCurChannel(ullWindowMask, outCH);
		}

		m_frameSpeedStatus = 1;
		m_forwardType = FF_TYPE_SMOOTHLY;
		m_bSetTimeFinish = false;
	}
	else
	{
		ReleaseReadedBuf();
		if (CAsyncReader::MR_RET_OK != m_asyncReader.SetCurChannel(ullWindowMask, outCH))
		{
			m_playLock.UnLock();
			return false;
		}
		else
		{
			ReleaseReadedBuf();
			ULONGLONG stopCH = m_prePlayChannels & ~outCH;
			//先停止设备
			CLocalDevice::Instance()->PlaybackStop(stopCH);

			//再开启设备
			CLocalDevice::Instance()->PlaybackStart(outCH);

			m_prePlayChannels = outCH;
			for (int chnn = 0; chnn <  m_chnnNum; chnn++)
			{
				if (m_pPlayStatus[chnn] == N_PS_PLAY)
				{
					m_pPlayStatus[chnn] = N_PS_PLAY;
				}
				else if(m_pPlayStatus[chnn] == N_PS_PAUSE)
				{
					m_pPlayStatus[chnn] = N_PS_PAUSE;
				}
				m_asyncReader.SetPlaySpeed(chnn, m_playRate[PLAY_SPEED_1X]);
				m_asyncReader.SetPlayNext(chnn, true);
				m_pframeSpeedStatus[chnn] = 1;
				m_pforwardType[chnn] = FF_TYPE_SMOOTHLY;
			}

		}
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

	m_playLock.UnLock();

	return true;
}
/*******************************************************************************************************************************
函数功能：回放控制
参数说明：bForward 向前播放为true，向后播放时false
		  playSpeed 播放速度
		  chnn 要控制的通道
返回值  ：无
********************************************************************************************************************************/
void CPlaybackManagerAsyn::SetPlaySpeed(bool bForward, PLAY_SPEED playSpeed, unsigned long chnn)
{
	
	m_playLock.Lock();

	if (m_bSync)
	{
		if((m_nPlayStatus == N_PS_STOP) || (m_nPlayStatus == N_PS_PAUSE))
		{
			//回放已停止,不进行任何操作
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

				//如果是128，表示产品不受这个约束。
				//就是采用默认，不管几路回放，最大只能做到4倍数逐帧解码。
				if (128 == m_maxFFSpeed)
				{
					if (m_frameSpeedStatus <= 4)
					{
						m_forwardType = FF_TYPE_SMOOTHLY;					
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

			ReleaseReadedBuf();
			m_speedCtrl.bFirstFrame = true;
		}

	}
	else
	{
		if((m_pPlayStatus[chnn] == N_PS_STOP) || (m_pPlayStatus[chnn] == N_PS_PAUSE))
		{
			//回放已停止,不进行任何操作
		}
		else
		{
			ReleaseReadedBuf();
			m_bFirstFrame[chnn] = true;
			m_asyncReader.SetPlayNext(chnn, true);
			if(!bForward)
			{
				m_pPlayStatus[chnn] = N_PS_REW;
				m_asyncReader.SetForward(chnn, false);
				m_asyncReader.SetJumping(chnn, true);
				m_asyncReader.SetPlaySpeed(chnn, m_playRate[playSpeed]);
				assert(playSpeed >= PLAY_SPEED_8X);
			}
			else if(playSpeed != PLAY_SPEED_1X)
			{
				assert(playSpeed <= PLAY_SPEED_16X);
				m_pPlayStatus[chnn] = N_PS_FF;
				m_asyncReader.SetPlaySpeed(chnn, m_playRate[playSpeed]);
				switch(playSpeed)
				{
				case PLAY_SPEED_2X:
					m_pframeSpeedStatus[chnn] = 2;
					break;
				case PLAY_SPEED_4X:
					m_pframeSpeedStatus[chnn] = 4;
					break;
				case PLAY_SPEED_8X:
					m_pframeSpeedStatus[chnn] = 8;
					break;
				case PLAY_SPEED_16X:
					m_pframeSpeedStatus[chnn] = 16;
					break;
				default:
					m_pframeSpeedStatus[chnn] = 1;
					break;
				}

				//如果是128，表示产品不受这个约束。
				//就是采用默认，不管几路回放，最大只能做到4倍数逐帧解码。
				if (128 == m_maxFFSpeed)
				{
					if (m_pframeSpeedStatus[chnn] <= 4)
					{
						m_pforwardType[chnn] = FF_TYPE_SMOOTHLY;	
						m_asyncReader.SetJumping(chnn , false);
					}
					else
					{
						m_pforwardType[chnn] = FF_TYPE_JUMPING;
						m_asyncReader.SetJumping(chnn, true);
					}
				}
				else
				{
					if (playSpeed <= PLAY_SPEED_1_2X)
					{
						m_pforwardType[chnn] = FF_TYPE_SMOOTHLY;
						m_asyncReader.SetJumping(chnn, false);
					}
					else
					{
						if (m_maxFFSpeed < (m_pframeSpeedStatus[chnn] * m_activeChnnNum))
						{
							m_pforwardType[chnn] = FF_TYPE_JUMPING;
							m_asyncReader.SetJumping(chnn, true);
						}
						else
						{
							m_pforwardType[chnn] = FF_TYPE_SMOOTHLY;
							m_asyncReader.SetJumping(chnn, false);
						}
					}	
				}
			}
			else
			{
				m_pPlayStatus[chnn] = N_PS_PLAY;
				m_bFirstFrame[chnn] = true;
				m_pframeSpeedStatus[chnn] = 1;
				m_pforwardType[chnn] = FF_TYPE_SMOOTHLY;
				m_asyncReader.SetPlaySpeed(chnn, m_playRate[PLAY_SPEED_1X]);
				m_asyncReader.SetJumping(chnn, false);
				m_asyncReader.SetForward(chnn, true);
			}
		}

	}
	
	m_playLock.UnLock();
}
/*******************************************************************************************************************************
函数功能：单帧播放时播放下一帧
参数说明：bForward 向前还是向后播放
		  chnn 为播放通道
返回值  ：无
********************************************************************************************************************************/
void CPlaybackManagerAsyn::PlayNextFrame(bool bForward, unsigned long chnn)
{
	m_playLock.Lock();
	if(m_bSync)
	{
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
	else
	{
		if (m_pPlayStatus[chnn] == N_PS_PAUSE)
		{
			m_asyncReader.SetPlayNext(chnn, true);
		}
	}
	m_playLock.UnLock();
}

/*******************************************************************************************************************************
函数功能：设置音频通道
参数说明：channel 通道号
返回值  ：无
********************************************************************************************************************************/
void CPlaybackManagerAsyn::SetAudioChannel(unsigned long channel)
{
	printf("%s,%d,chnn:%d\n",__FILE__,__LINE__,channel);
	m_audioChannel = channel;
}
/*******************************************************************************************************************************
函数功能：回放线程
参数说明：
返回值  ：无
********************************************************************************************************************************/
RESULT WINAPI CPlaybackManagerAsyn::SyncPlayProc(void *pParam)
{
#ifdef __ENVIRONMENT_LINUX__
	pid_t pid = getpid();
	printf("%s, %d, pid = %d\n", __FILE__, __LINE__, pid);
#endif

	CPlaybackManagerAsyn *pThis = (CPlaybackManagerAsyn *)pParam;
	pThis->SyncPlayProc();

	return 0;
}

RESULT WINAPI CPlaybackManagerAsyn::AsyncPlayProc(void *pParam)
{
#ifdef __ENVIRONMENT_LINUX__
	pid_t pid = getpid();
	pid_t tid = syscall(__NR_gettid);
	printf("%s, %d, pid = %d, tid=%d\n", __FILE__, __LINE__, pid, tid);
#endif

	CPlaybackManagerAsyn *pThis = (CPlaybackManagerAsyn *)pParam;
	pThis->AsyncPlayProc();

	return 0;
}
/*******************************************************************************************************************************
函数功能：多通道异步回放线程
参数说明：无
返回值  ：无
********************************************************************************************************************************/
void CPlaybackManagerAsyn::AsyncPlayProc()
{
	memset(&m_frameInfo, 0, sizeof(FRAME_INFO_EX));
	unsigned long frameTypeTemp = FRAME_TYPE_NONE;
	unsigned long chn;
	bool bNeedSleep = false;
	unsigned long getBufferTryTimes = 0;
	unsigned long lastFrameHeight = 0;
	unsigned long lastFrameWidth = 0;

	while (true)
	{
		bNeedSleep = false;

		m_playLock.Lock();
		if (!m_bRun)
		{
			ReleaseReadedBuf();
			m_playLock.UnLock();
			PUB_Sleep(10);
			continue;
		}
		
		bool bStop = true;
		for (int chnn = 0; chnn < m_chnnNum; chnn++)
		{
			if (m_pPlayStatus[chnn] != N_PS_STOP)
			{
				bStop = false;
			}
		}

		if (bStop)
		{
			ReleaseReadedBuf();
			m_playLock.UnLock();
			break;
		}


		long ret;
		unsigned long currReaderID = 0;
		unsigned long currReaderIDTmp = 0;
		bool bneedchange=false;
		if (!m_bFrameReadFromFile)
		{
			memset(&m_frameInfo, 0, sizeof(m_frameInfo));
			ret = m_asyncReader.GetNextFrameType(m_frameInfo, currReaderIDTmp,bneedchange);
			if(ret == CAsyncReader::MR_RET_OK)
			{

			}
			else if(ret == CAsyncReader::MR_RET_ALLCHNN_PAUSE)
			{
				m_playLock.UnLock();
				continue;
			}
			else
			{
				m_pMsgCallBack(m_pCallBackObj, 2, NULL, NULL);
				m_playLock.UnLock();
				break;
			}


			frameTypeTemp = m_frameInfo.frameType;

			//（2010-01-20 15:18:00 YSW）
			//把获取内存统一到LocaclDevice内部去。发现是视频或者音频格式，则申请一片内存
			//其他都是从解码器获取内存地址。
			m_frameInfo.channel = CHANNEL_ID(m_frameInfo.channel);
			//获取BUFFER次数超过MAX_GET_BUF_NUM，则意味着获取缓冲区失败，则需要退出回放。
			if(getBufferTryTimes >= MAX_GET_BUF_NUM)
			{
				m_pMsgCallBack(m_pCallBackObj, 2, NULL, NULL);
				m_playLock.UnLock();

				printf("%s,%d,not get buffer.time:%d\n",__FILE__,__LINE__,time);				

				RUN_LOG(RUN_LOG_ITEM_PB_GET_FRAME_FAIL, CRunLog::RUN_Num("CH:", m_frameInfo.channel), CRunLog::RUN_Num("Frame Type:", m_frameInfo.frameType), "Get Buffer fail");

				break;
			}

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

			ret = m_asyncReader.GetFrame(m_frameInfo, currReaderID);
			//修改问题：在调用GetNextFrameType函数时，得到是音频帧，但是在GetNextFrame或
			//GetKeyFrame时得到的是视频帧。
			//重新条件：播放音频时，点击8（16,32）倍数的快进或快退。
			if ((frameTypeTemp != m_frameInfo.frameType) || (currReaderID != currReaderIDTmp))
			{
				printf("#############################################chnn:%d,frameType error,%d,%d. keyframe:%d\n", m_frameInfo.channel, frameTypeTemp, m_frameInfo.frameType, m_frameInfo.keyFrame);
				m_frameInfo.frameType = FRAME_TYPE_NONE;
				m_frameInfo.frameIndex = 1;
				CLocalDevice::Instance()->PlaybackSetBuf(&m_frameInfo);
				m_bFrameReadFromFile = false;
				m_playLock.UnLock();
				continue;
			}

			//读取数据成功
			if(MR_RET_OK == ret)
			{
				m_bFrameReadFromFile = true;
			}
		}
		
		//读取数据成功
		if (m_bFrameReadFromFile)
		{
			if(bneedchange)//修改提前是因为获取的frametype有可能是音频，此时也需要设置BaseShowTime
			{
				m_asyncReader.SetBaseShowTime(currReaderID, GetCurrTime());
			}
			chn = CHANNEL_ID(m_frameInfo.channel);//得到其通道号，数字和模拟统一编号的通道

			//如果是视频和音频格式数据
			if(m_frameInfo.frameType == FRAME_TYPE_VIDEO_FORMAT)
			{
				m_frameInfo.frameIndex = 1;
				CLocalDevice::Instance()->PlaybackSetBuf(&m_frameInfo);
				m_bFrameReadFromFile = false;
				m_asyncReader.SetPlayNext(chn, true);
				m_playLock.UnLock();
				continue;
			}
			else if(m_frameInfo.frameType == FRAME_TYPE_AUDIO_FORMAT)
			{
				m_frameInfo.frameIndex = 1;
				CLocalDevice::Instance()->PlaybackSetBuf(&m_frameInfo);
				m_bFrameReadFromFile = false;
				m_asyncReader.SetPlayNext(chn, true);
				m_playLock.UnLock();
				continue;
			}
			else if(m_frameInfo.frameType == FRAME_TYPE_VIDEO)
			{
#if defined(__TDFH__) || defined(__CHIPTI__)
				if ( lastFrameHeight && lastFrameHeight != m_frameInfo.height 
					|| lastFrameWidth && lastFrameWidth != m_frameInfo.width)
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
				m_frameInfo.frameAttrib = PLAY_FRAME_SHOW;				
				m_frameInfo.frameIndex = (FF_TYPE_JUMPING == m_pforwardType[chn]) ? 1 : m_pframeSpeedStatus[chn];

				if (m_bFirstFrame[chn])
				{
					m_asyncReader.SetBaseFrameTime(currReaderID, m_frameInfo.time);
					m_asyncReader.SetBaseShowTime(currReaderID, GetCurrTime());
					m_asyncReader.SetLastReadFrameTime(currReaderID,m_frameInfo.time);
					m_bFirstFrame[chn] = false;
				}
				else if(GetCurrTime() < m_asyncReader.GetShowTime())
				{
					m_playLock.UnLock();
					bNeedSleep = true;
					goto PROC_END;
				}
				else
				{
					LONGLONG diffTime = m_asyncReader.GetShowTime() - GetCurrTime();
					if ((0 < diffTime) && (diffTime < 3000000))
					{
						m_playLock.UnLock();
						bNeedSleep = true;
						goto PROC_END;
					}
					m_pMsgCallBack(m_pCallBackObj, 1, &m_frameInfo.time,  &m_frameInfo.channel);
				}
				
				m_asyncReader.SetLastReadFrameTime(currReaderID,m_frameInfo.time);
				//m_frameInfo.frameAttrib = PLAY_FRAME_SHOW;				
				//m_frameInfo.frameIndex = (FF_TYPE_JUMPING == m_pforwardType[chn]) ? 1 : m_pframeSpeedStatus[chn];
				m_pMsgCallBack(m_pCallBackObj, 1, &m_frameInfo.time,  &m_frameInfo.channel);
				CLocalDevice::Instance()->PlaybackSetBuf(&m_frameInfo);
				m_bFrameReadFromFile = false;

				m_playLock.UnLock();
			}
			else if(m_frameInfo.frameType == FRAME_TYPE_AUDIO)
			{
				if ((m_pPlayStatus[chn] != N_PS_PLAY) || (m_audioChannel != chn))
				{
					//由于每次读取数据之前已经获取到了缓冲区，且并不知道即将来的帧的类型，所以
					//如果不是当前回放地通道，则需要进行过滤。
					m_frameInfo.frameType = FRAME_TYPE_NONE;
				}

				m_frameInfo.frameIndex = 1;
				CLocalDevice::Instance()->PlaybackSetBuf(&m_frameInfo);
				m_bFrameReadFromFile = false;
				m_playLock.UnLock();
			}
			else
			{
				m_frameInfo.frameIndex = 1;
				CLocalDevice::Instance()->PlaybackSetBuf(&m_frameInfo);
				m_bFrameReadFromFile = false;
				m_playLock.UnLock();				
			}
		}
		else
		{
			//没有数据可回放，宣告回放结束
			m_frameInfo.frameIndex = 1;
			CLocalDevice::Instance()->PlaybackSetBuf(&m_frameInfo);
			m_bFrameReadFromFile = false;
			m_pMsgCallBack(m_pCallBackObj, 2, NULL, NULL);
			m_playLock.UnLock();

			break;
		}

PROC_END:
		if(bNeedSleep)
		{
			PUB_Sleep(0);
		}
	}
}

/*******************************************************************************************************************************
函数功能：多通道异步回放线程
参数说明：无
返回值  ：无
********************************************************************************************************************************/
void CPlaybackManagerAsyn::SyncPlayProc()
{
	memset(&m_frameInfo, 0, sizeof(FRAME_INFO_EX));

	LONGLONG lastShowTime = 0;
	unsigned long MultiCH = 0, chnn = 0;
	unsigned long frameTypeTemp = FRAME_TYPE_NONE;
	bool bNeedSleep = false;
	unsigned long getBufferTryTimes = 0;
	unsigned long lastFrameHeight = 0;
	unsigned long lastFrameWidth = 0;

	while(true)
	{
		bNeedSleep = false;
		m_playLock.Lock();
		if (!m_bRun)
		{
			ReleaseReadedBuf();
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
			ReleaseReadedBuf();
			m_playLock.UnLock();
			break;
		}

		if(m_nPlayStatus == N_PS_CHANGING_TIME)
		{
			ReleaseReadedBuf();
			m_playLock.UnLock();
			PUB_Sleep(10);
			continue;
		}

		if(m_nPlayStatus == N_PS_PAUSE)
		{
			if(!m_bPlayNext && !m_bPlayPrev && m_bSetTimeFinish)		//如果没有按下一帧按钮
			{
				ReleaseReadedBuf();
				m_playLock.UnLock();
				PUB_Sleep(200);
				continue;
			}
		}

		//取下一帧
		if (!m_bFrameReadFromFile)
		{
			memset(&m_frameInfo, 0, sizeof(m_frameInfo));

			//获取下一帧的帧类型;
			long ret;
			if((m_nPlayStatus == N_PS_REW) && (m_playSpeed >= PLAY_SPEED_8X))
			{
				ret = m_syncReader.GetNextFrameType(m_frameInfo, false);
			}
			else
			{
				ret = m_syncReader.GetNextFrameType(m_frameInfo, true);
			}

			if(MR_RET_OK != ret)
			{
				m_pMsgCallBack(m_pCallBackObj, 2, NULL, NULL);
				m_playLock.UnLock();
				break;
			}

			frameTypeTemp = m_frameInfo.frameType;
			//（2010-01-20 15:18:00 YSW）
			//把获取内存统一到LocaclDevice内部去。发现是视频或者音频格式，则申请一片内存
			//其他都是从解码器获取内存地址。
			m_frameInfo.channel = CHANNEL_ID(m_frameInfo.channel);

			//等号成立，则意味着获取缓冲区失败，则需要退出回放。
			if(getBufferTryTimes > MAX_GET_BUF_NUM)
			{
				m_pMsgCallBack(m_pCallBackObj, 2, NULL, NULL);
				m_playLock.UnLock();

				printf("%s,%d,not get buffer.time:%d\n",__FILE__,__LINE__,time);				

				RUN_LOG(RUN_LOG_ITEM_PB_GET_FRAME_FAIL, CRunLog::RUN_Num("CH:", m_frameInfo.channel), CRunLog::RUN_Num("Frame Type:", m_frameInfo.frameType), "Get Buffer fail");

				break;
			}

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

			//根据播放状态来调用不同的接口读取数据，8倍数以上则跳关键帧，其他的则是走正常的帧序列。
			if((m_nPlayStatus == N_PS_FF) && (FF_TYPE_JUMPING == m_forwardType))
			{
				ret = m_syncReader.GetKeyFrame(m_frameInfo, 1, true);
			}
			else if((m_nPlayStatus == N_PS_REW) && (m_playSpeed >= PLAY_SPEED_8X))
			{
				ret = m_syncReader.GetKeyFrame(m_frameInfo, 1, false);
			}
			else if((m_nPlayStatus == N_PS_PLAY) || (m_nPlayStatus == N_PS_FF) || (m_nPlayStatus == N_PS_REW))
			{
				ret = m_syncReader.GetNextFrame(m_frameInfo);
			}
			else if ((m_nPlayStatus == N_PS_PAUSE) && (m_bPlayNext || m_bPlayPrev || !m_bSetTimeFinish))
			{
				ret = m_syncReader.GetNextFrame(m_frameInfo);
			}
			else
			{
				RUN_LOG(RUN_LOG_ITEM_PB_GET_FRAME_FAIL, CRunLog::RUN_Num("Play Status:", m_nPlayStatus), CRunLog::RUN_Num("Play Speed:", m_playSpeed), "Get Next Frame fail");
			}

			//修改问题：在调用GetNextFrameType函数时，得到是音频帧，但是在GetNextFrame或
			//GetKeyFrame时得到的是视频帧。
			//重新条件：播放音频时，点击8（16,32）倍数的快进或快退。
			if (frameTypeTemp != m_frameInfo.frameType)
			{
				printf("#############################################chnn:%d,frameType error,%d,%d. keyframe:%d\n", m_frameInfo.channel, frameTypeTemp, m_frameInfo.frameType, m_frameInfo.keyFrame);
				m_frameInfo.frameType = FRAME_TYPE_NONE;
				m_frameInfo.frameIndex = 1;
				CLocalDevice::Instance()->PlaybackSetBuf(&m_frameInfo);
				m_bFrameReadFromFile = false;
				m_playLock.UnLock();
				continue;
			}

			//读取数据成功
			if(MR_RET_OK == ret)
			{
				m_bFrameReadFromFile = true;
			}
		}

		//读取数据成功
		if(m_bFrameReadFromFile)
		{
			chnn = CHANNEL_ID(m_frameInfo.channel);//得到其通道号，数字和模拟统一编号的通道

			//如果是视频和音频格式数据
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
				else if (GetCurrTime() < m_speedCtrl.lastShowTime)
				{
					m_playLock.UnLock();
					bNeedSleep = true;
					goto PROC_END;
				}
				else if(0 != (MultiCH & (0x01 << chnn)))
				{
					LONGLONG diffTime = 0;
					if(m_nPlayStatus == N_PS_REW)
					{
						if (m_playRate[m_playSpeed] >> 8)
						{
							diffTime = static_cast<LONGLONG>((m_speedCtrl.lastFrameTime - m_frameInfo.time) * (m_playRate[m_playSpeed] & 0xff));
						}
						else
						{
							diffTime = static_cast<LONGLONG>((m_speedCtrl.lastFrameTime - m_frameInfo.time) / m_playRate[m_playSpeed]);
						}

					}
					else
					{
						if (m_playRate[m_playSpeed] >> 8)
						{
							diffTime = static_cast<LONGLONG>((m_frameInfo.time - m_speedCtrl.lastFrameTime) * (m_playRate[m_playSpeed] & 0xff));
						}
						else
						{
							diffTime = static_cast<LONGLONG>((m_frameInfo.time - m_speedCtrl.lastFrameTime) / m_playRate[m_playSpeed]);
						}
					}

					m_frameInfo.frameAttrib = PLAY_FRAME_SHOW;
					m_frameInfo.frameIndex = (FF_TYPE_JUMPING == m_forwardType) ? 1 : m_frameSpeedStatus;

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
					MultiCH = 0;

					//diffTime = (diffTime <= 0) ? 0 : diffTime;
					diffTime = (diffTime >= 8000000) ? 8000000 : diffTime;

					m_speedCtrl.lastShowTime += diffTime;

					m_playLock.UnLock();
					//bNeedSleep = true;
					bNeedSleep = false;
					goto PROC_END;

// 					if(GetCurrTime() < m_speedCtrl.lastShowTime)
// 					{
// 
// 					}
// 					else
// 					{
// 						MultiCH = 0x01 << chnn;
// 					}
				}
				else
				{
					MultiCH |= (0x01 << chnn);
				}

				m_frameInfo.frameAttrib = PLAY_FRAME_SHOW;
				m_frameInfo.frameIndex = (FF_TYPE_JUMPING == m_forwardType) ? 1 : m_frameSpeedStatus;
				CLocalDevice::Instance()->PlaybackSetBuf(&m_frameInfo);
				m_bFrameReadFromFile = false;
				m_playLock.UnLock();
			}
			else if(m_frameInfo.frameType == FRAME_TYPE_AUDIO)
			{
				if ((m_nPlayStatus != N_PS_PLAY) || (m_audioChannel != chnn))
				{
					//由于每次读取数据之前已经获取到了缓冲区，且并不知道即将来的帧的类型，所以
					//如果不是当前回放地通道，则需要进行过滤。
					m_frameInfo.frameType = FRAME_TYPE_NONE;
				}

				m_frameInfo.frameIndex = 1;
				CLocalDevice::Instance()->PlaybackSetBuf(&m_frameInfo);
				m_bFrameReadFromFile = false;
				m_playLock.UnLock();

			}
			else
			{
				m_frameInfo.frameIndex = 1;
				CLocalDevice::Instance()->PlaybackSetBuf(&m_frameInfo);
				m_bFrameReadFromFile = false;
				m_playLock.UnLock();				
			} 			
		}
		else
		{
			//没有数据可回放，宣告回放结束
			m_frameInfo.frameIndex = 1;
			CLocalDevice::Instance()->PlaybackSetBuf(&m_frameInfo);
			m_bFrameReadFromFile = false;

			m_pMsgCallBack(m_pCallBackObj, 2, NULL, NULL);
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

