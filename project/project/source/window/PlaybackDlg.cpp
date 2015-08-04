/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : hz
** Date         : 2009-08-12
** Name         : PlaybackDlg.cpp
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/

#include "PlaybackDlg.h"
#include "Product.h"
#include "NetProtocol.h"
#include "MessageMan.h"
#include "FuncCustom.h"
#include "UserMan.h"

extern NET_LOGIN_SUCCESS_REPLY *g_p_login_succ;

//////////////////////////////////////////////////////////////////////////
CPlaybackDlg::CPlaybackDlg():m_pTipWindow(NULL)
{
	m_pFileList				= NULL;
	m_playType				= PLAYBACK_ON_TIME;
	m_playbackStartTime		= 0;
	m_playbackEndTime		= 0;
	m_playbackPlayTime		= 0;
	m_chnnBits				= 0;
	m_bChangePlayChnn		= true;

	m_maxChannelNum			= 0;
	m_maxVideoOutputNum		= 0;
#if defined(__ASYN_PLAY__)
	m_bSync					= true;
#endif

	m_ipcMaxVideoOutputNum = CProduct::Instance()->IPCMaxVideoOutNum();
}

CPlaybackDlg::~CPlaybackDlg()
{
	m_preSplitModeInfoList.RemoveAll();
	m_curSplitModeInfoList.RemoveAll();

}

void CPlaybackDlg::SetPlayInfo(unsigned long playbackStartTime, unsigned long playbackEndTime, 
							   unsigned long playbackPlayTime,  ULONGLONG chnnBits, 
							   unsigned char maxChannelNum, unsigned char videoOutPutNum, 
							   bool bChangePlayChnn, unsigned long diskOwnerIndex)
{
	assert(chnnBits != 0);
	assert(0 == m_chnnBits);
	
	ULONGLONG auth = CUserMan::Instance()->CheckAuthority(AUTH_PLAYBACK);
	unsigned char channelNum = g_p_login_succ->productInfo.videoInputNum;
	unsigned char maxVideoOutNum = g_p_login_succ->productInfo.videoOuputNum;
	unsigned char localChnnNum =  g_p_login_succ->productInfo.localVideoInputNum;
	unsigned char IpcMaxChnnNum = CProduct::Instance()->IPCMaxVideoOutNum();
	LONGLONG chnnBits_chnn = 0; 
	int numTemp = 0;
	chnnBits_chnn = ALL_CHNN_MASK(channelNum);
	chnnBits_chnn &= auth;

	for (int index = 0; index < localChnnNum; index++)
	{
		if (chnnBits_chnn & (((LONGLONG)0x01) << index))
		{
			numTemp++;	
		}
	}
	if(numTemp ==0)
	{
		videoOutPutNum = IpcMaxChnnNum;	//����ͨ��û�лط�Ȩ��  ,�ָ���ΪIPC�ط�
	}
	
	PLAY_CH_MODE maxSplitMode = GetSplitMode(videoOutPutNum);

	assert(maxSplitMode <= PLAY_CH_SIXTEEN); //���֧��16·�ԱȻط�
	
	m_maxSplitMode			= maxSplitMode;		
	m_playbackStartTime		= playbackStartTime;
	m_playbackEndTime		= playbackEndTime;
	m_playbackPlayTime		= playbackPlayTime;
	m_chnnBits				= chnnBits;
	m_maxChannelNum			= maxChannelNum;
	m_maxVideoOutputNum		= videoOutPutNum;
	m_bChangePlayChnn		= bChangePlayChnn;
	m_diskOwnerIndex		= diskOwnerIndex;
	m_playType				= PLAYBACK_ON_TIME;

	m_playerDlg.SetMaxSelChnn(m_maxChannelNum, videoOutPutNum);
	m_playerDlg.SetCurPlayChnn(m_chnnBits);
	m_playerDlg.SetDiskOwner(diskOwnerIndex);
	m_ipcMaxVideoOutputNum = CProduct::Instance()->IPCMaxVideoOutNum();
	
	if (m_ipcMaxVideoOutputNum > m_maxVideoOutputNum)
	{
		m_ipcMaxVideoOutputNum = m_maxVideoOutputNum;
	}
}

void CPlaybackDlg::SetPlayInfo(FILE_INFO_EX fileInfo, unsigned long diskOwnerIndex)
{
	//�޸ģ����ļ����¼��ط�ʱ����ʱ����ʧ�ܵġ�
	//	���е�һ��ԭ���ǣ�PlaybackDlg��cpp�е�m_maxChannelNum�����ǣ��طŵ�ͨ���ŵ�ȡֵ��Χ�������Ǳ�ʾ�����ط�·����
	//	���磺��2316ME�ϰ��ļ��طţ�����ļ���2ͨ���ġ���ôm_maxChannelNum��16������1��

	assert (NULL == m_pFileList);
	m_pFileList = new FILE_INFO_EX_LIST;
	assert (NULL != m_pFileList);

	m_pFileList->AddTail(fileInfo);

	m_maxSplitMode			= PLAY_CH_ONE;		
	m_playbackStartTime		= fileInfo.startTime;
	m_playbackEndTime		= fileInfo.endTime;
	m_playbackPlayTime		= fileInfo.startTime;
	m_chnnBits				= static_cast<ULONGLONG>(0x01) << fileInfo.chnn;
	m_maxChannelNum			= g_p_login_succ->productInfo.videoInputNum;
	m_maxVideoOutputNum		= g_p_login_succ->productInfo.videoOuputNum;
	m_bChangePlayChnn		= false;
	m_diskOwnerIndex		= diskOwnerIndex;
	m_playType				= PLAYBACK_ON_FILE;

	m_playerDlg.SetMaxSelChnn(m_maxChannelNum, m_maxVideoOutputNum);
	m_playerDlg.SetCurPlayChnn(m_chnnBits);
	m_playerDlg.SetDiskOwner(diskOwnerIndex);
	
	m_ipcMaxVideoOutputNum = CProduct::Instance()->IPCMaxVideoOutNum();
	if (m_ipcMaxVideoOutputNum > m_maxVideoOutputNum)
	{
		m_ipcMaxVideoOutputNum = m_maxVideoOutputNum;
	}
}

void CPlaybackDlg::OnInitial()
{
	assert((0 != m_chnnBits) || (NULL != m_pFileList));

	m_style |= WND_NO_TITLE;
	GUI::CDialog::OnInitial();
    
	InitialBtnStatus();
	////////////////////////////////////////////////////
	int height = 80;
	if (576 == m_height)
	{
		height += 20; 
	}

	m_playerDlg.Create(GetRandWndID(), 0, m_height-height, m_width, height, this, 0, 1);
	m_rectPlayer = m_playerDlg.GetWindowRect();

	m_bDragSlider = false;
	m_changeTime  = 0;
	m_bStopPlayMsg = false;

	// �����϶����ο�
	m_groupCtrl.Create(GetRandWndID(), 0, 0, m_width / 10, m_height / 10, this);
	m_groupCtrl.Show(false, false);

	//�Ѱ���ģʽ��Ϊ����ģʽ
	m_keyMode = KEY_MODE_PLAYBACK;

	m_bDragChnn = false;
	m_bFirstDragChnn = false;
	m_bDraged = false;

#if defined(__NEW_CUT_BACKUP__)
	m_clickchnnINfo.clickchnn=0;//����ͨ��λ����С��ͨ����
	m_clickchnnINfo.bsync=true;//Ĭ��ͬ��
	for(int i=0;i<m_maxChannelNum;i++)
	{
		if (m_chnnBits & (((LONGLONG)0x01) << i))
		{
			m_clickchnnINfo.clickchnn=i;//�ҵ���С��ͨ������������Ϊ���
			break;
		}	
	}
	//������Ϣ�ѻطź��ֳ��ĺ�����ͨ��0��

	MESSAGE_DATA msgData;
	memset(&msgData, 0, sizeof(MESSAGE_DATA));
	CMessageMan::Instance()->SendMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_LOCAL_CLIENT_PLAYBACK_CHNINIT, (unsigned char*)&(m_clickchnnINfo.clickchnn),sizeof(unsigned long), msgData);
	CMessageMan::Instance()->ReleaseMsgBuff(msgData);
	m_playerDlg.SetCurSelectChnn(m_clickchnnINfo.clickchnn);
#endif

}

void CPlaybackDlg::OnPostInitial()
{
	GUI::CDialog::OnPostInitial();

#if defined(__ASYN_PLAY__)
	for (int i = 0; i < g_p_login_succ->productInfo.localVideoInputNum; i++)
	{
		m_curStatus[i] = PLAYBACK_STATUS_STOP;
	}
#endif

	m_curPlaybackStatus = PLAYBACK_STATUS_STOP;
	m_curSplitMode      = PLAY_CH_NULL;
	m_preSplitMode      = PLAY_CH_NULL;

	KEY_VALUE_INFO keyValue;
	keyValue.type = KEY_TYPE_VIRTUAL;
	keyValue.value = KEY_VIRTUAL_PLAY;
	AddKeyValueEx(keyValue); 
}

void CPlaybackDlg::OnDestroy()
{
#if !defined(__ASYN_PLAY__)
	GUI::CDialog::OnDestroy();

	if (NULL != m_pFileList)
	{
		delete m_pFileList;
		m_pFileList = NULL;
	}

	m_chnnBits = 0;

	if (PLAYBACK_STATUS_STOP != m_curPlaybackStatus)
	{
		MESSAGE_DATA msgData;
		memset(&msgData, 0, sizeof(MESSAGE_DATA));
		CMessageMan::Instance()->SendMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_LOCAL_CLIENT_PLAYBACK_STOP, NULL, 0, msgData);

		CMessageMan::Instance()->ReleaseMsgBuff(msgData);
		m_curPlaybackStatus = PLAYBACK_STATUS_STOP;
	}

	MESSAGE_DATA msgData;
	memset(&msgData, 0, sizeof(MESSAGE_DATA));
	CMessageMan::Instance()->SendMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_CHANGE_DISPLAY_MODE, NULL, 0, msgData);
	CMessageMan::Instance()->ReleaseMsgBuff(msgData);

#if defined(__NEW_CUT_BACKUP__)
	memset(&msgData, 0, sizeof(MESSAGE_DATA));
	CMessageMan::Instance()->SendMsgDataToServer(0, LOCAL_DEVICE_ID, CMD_REQUEST_LOCAL_CLIENT_PLAYBACK_UNINIT,NULL,0, msgData);
	CMessageMan::Instance()->ReleaseMsgBuff(msgData);
#endif

	NET_LIVEAUDIO_CTRL_INFO audioCtrlInfo;
	audioCtrlInfo.type = NET_LIVE_AUDIO_RECOVER;
	CMessageMan::Instance()->PostMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_CONTROL_LIVE_AUDIO, (unsigned char*)(&audioCtrlInfo), sizeof(audioCtrlInfo));

#else
	GUI::CDialog::OnDestroy();

	if (NULL != m_pFileList)
	{
		delete m_pFileList;
		m_pFileList = NULL;
	}

	m_chnnBits = 0;

	if (m_bSync)
	{
		if (PLAYBACK_STATUS_STOP != m_curPlaybackStatus)
		{
			MESSAGE_DATA msgData;
			memset(&msgData, 0, sizeof(MESSAGE_DATA));
			CMessageMan::Instance()->SendMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_LOCAL_CLIENT_PLAYBACK_STOP, NULL, 0, msgData);

			CMessageMan::Instance()->ReleaseMsgBuff(msgData);
			m_curPlaybackStatus = PLAYBACK_STATUS_STOP;
		}
	}
	else
	{
		bool bStop = true;
		for (int i = 0; i < g_p_login_succ->productInfo.localVideoInputNum; i++)
		{
			if (m_curStatus[i] != PLAYBACK_STATUS_STOP)
			{
				bStop = false;
			}
		}
		if (!bStop)
		{
			MESSAGE_DATA msgData;
			memset(&msgData, 0, sizeof(MESSAGE_DATA));
			CMessageMan::Instance()->SendMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_LOCAL_CLIENT_PLAYBACK_STOP, NULL, 0, msgData);

			CMessageMan::Instance()->ReleaseMsgBuff(msgData);
			for (int i = 0; i < g_p_login_succ->productInfo.localVideoInputNum; i++)
			{
				m_curStatus[i] = PLAYBACK_STATUS_STOP;
			}
		}
	}
	MESSAGE_DATA msgData;
	memset(&msgData, 0, sizeof(MESSAGE_DATA));
	CMessageMan::Instance()->SendMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_CHANGE_DISPLAY_MODE, NULL, 0, msgData);
	CMessageMan::Instance()->ReleaseMsgBuff(msgData);

#if defined(__NEW_CUT_BACKUP__)
	memset(&msgData, 0, sizeof(MESSAGE_DATA));
	CMessageMan::Instance()->SendMsgDataToServer(0, LOCAL_DEVICE_ID, CMD_REQUEST_LOCAL_CLIENT_PLAYBACK_UNINIT,NULL,0, msgData);
	CMessageMan::Instance()->ReleaseMsgBuff(msgData);
#endif

	NET_LIVEAUDIO_CTRL_INFO audioCtrlInfo;
	audioCtrlInfo.type = NET_LIVE_AUDIO_RECOVER;
	CMessageMan::Instance()->PostMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_CONTROL_LIVE_AUDIO, (unsigned char*)(&audioCtrlInfo), sizeof(audioCtrlInfo));

#endif
}

void CPlaybackDlg::OnPaint(GUI::PAINT_STATUS status)
{
	//���ػ��ƣ���֤�����ֳ��ָ��ߵ�ˢ����
}

BEGIN_MSG_MAP(CPlaybackDlg, GUI::CDialog)
	ON_MSG(CTRLID_NULL, KEY_VIRTUAL_PLAY, OnClickPlayPause)
	ON_MSG(CTRLID_NULL, KEY_VIRTUAL_STOP, OnClickStop)
	ON_MSG(CTRLID_NULL, KEY_VIRTUAL_SF, OnClickNextFrame)
	ON_MSG(CTRLID_NULL, KEY_VIRTUAL_FF, OnClickFastForward)

	ON_MSG(CTRLID_NULL, KEY_VIRTUAL_REW, OnClickReward)

	ON_MSG(CTRLID_NULL, KEY_VIRTUAL_HIDE, OnHide)
	ON_MSG(CTRLID_NULL, KEY_VIRTUAL_EXIT, OnExit)
	ON_MSG(CTRLID_NULL, KEY_VIRTUAL_AUDIO, OnAudio)
	ON_MSG(CTRLID_NULL, KEY_VIRTUAL_ZOOM, OnZoom)

	ON_MSG(CTRLID_NULL, KEY_VIRTUAL_DEC, OnClickPreSection)
	ON_MSG(CTRLID_NULL, KEY_VIRTUAL_ADD, OnClickNextSection)

	ON_MSG_EX(m_playerDlg.GetID(), KEY_VIRTUAL_MSG_DEFAULT_TAG, OnDrag)
	ON_MSG_EX(CTRLID_NULL, KEY_VIRTUAL_BLUP, OnMouseUp)
	ON_MSG_EX(CTRLID_NULL, KEY_VIRTUAL_ENTER, OnEnter)
	ON_MSG_EX(CTRLID_NULL, KEY_VIRTUAL_MOVE, OnMouseMove)
	ON_MSG_EX(CTRLID_NULL, KEY_VIRTUAL_DRAG, OnMouseDrag)
	ON_MSG_KEY_RANGE(CTRLID_NULL, KEY_VIRTUAL_0, KEY_VIRTUAL_32, OnClickNumKey)
	ON_MSG_KEY_RANGE(CTRLID_NULL, KEY_VIRTUAL_1X1, KEY_VIRTUAL_4X4, OnClickSplit)

	ON_MSG_KEY_RANGE(CTRLID_NULL, KEY_VIRTUAL_FLY_LEFT_1, KEY_VIRTUAL_FLY_LEFT_8, OnFlyLeft)
	ON_MSG_KEY_RANGE(CTRLID_NULL, KEY_VIRTUAL_FLY_RIGHT_1, KEY_VIRTUAL_FLY_RIGHT_8, OnFlyRight)
	ON_MSG(CTRLID_NULL, KEY_VIRTUAL_FLY_MEDIUM, OnFlyMedium)
	ON_MSG(CTRLID_NULL, KEY_VIRTUAL_FLFLY_IN_RIGHT, OnFlyInRight)

	ON_MSG(CTRLID_NULL, KEY_VIRTUAL_SPLIT, OnNextSplit)
	ON_MSG(CTRLID_NULL, KEY_VIRTUAL_GROUP, OnNextGroup)

	ON_MSG_EX(CTRLID_NULL, KEY_VIRTUAL_DBCLICK, OnDoubleClick)

	#ifdef __CUSTOM_US02_NEW__
	ON_MSG(CTRLID_NULL, KEY_VIRTUAL_OVER, OnOver)
	ON_MSG(CTRLID_NULL, KEY_VIRTUAL_OUT, OnOut)
	#endif
	
END_MSG_MAP()

unsigned long  CPlaybackDlg::OnHide()
{
	if (m_playerDlg.IsShow())
	{
		m_playerDlg.Show(false);
		#ifdef __CUSTOM_US02_NEW__
			if(m_pTipWindow != NULL)
				{
					m_pTipWindow->Show(false, true);
				}
		#endif
	}
	else
	{
		m_playerDlg.Show(true);
	}

	GetRootWnd()->Repaint();

	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long  CPlaybackDlg::OnExit()
{
	GUI::CDialog::EndDialog(DLG_ESC);
	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CPlaybackDlg::OnAudio()
{
#ifndef __DVR_ULTIMATE__
	if (0 != (m_chnnBits & ALL_CHNN_MASK(g_p_login_succ->productInfo.audioInputNum)))
#else
	if (0 != (m_chnnBits & ALL_CHNN_MASK(g_p_login_succ->productInfo.localVideoInputNum + g_p_login_succ->productInfo.netVideoInputNum)))
#endif
	{
		//��ȡ��Ƶͨ��
		unsigned long audioChnn = m_playerDlg.GetAudioCH();
		CMessageMan::Instance()->PostMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_LOCAL_CLIENT_PLAYBACK_CHG_AUDIO, (unsigned char *)(&audioChnn), sizeof(unsigned long));
	}

	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CPlaybackDlg::OnZoom()
{
	if (PLAY_CH_ONE == m_curSplitMode)
	{
		bool blive=false;
		//��ȡ���ڵ��ֳ�ͨ������Ŀ
		NET_DISPLAY_INFO curDisp;
		memset(&curDisp, 0, sizeof(NET_DISPLAY_INFO));
		curDisp.type = NET_DISPLAY_GET_INFO;
		MESSAGE_DATA msgData;
		memset(&msgData, 0, sizeof(MESSAGE_DATA));
		CMessageMan::Instance()->SendMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_GET_DISPLAY_INFO, (unsigned char*)&curDisp, sizeof(NET_DISPLAY_INFO), msgData);
		assert(NULL != msgData.pData);
		PACKCMD *pCmd = reinterpret_cast<PACKCMD *>(msgData.pData);
		if (CMD_REPLY_CTRL_SUCC == static_cast<NET_PROTOCOL>(pCmd->cmdType))
		{
			//��ȡ��ǰ��ʾ��ͨ��
			NET_DISPLAY_INFO* pDispInfo = reinterpret_cast<NET_DISPLAY_INFO*>(msgData.pData + sizeof(PACKCMD));
		
			for(int i = 0; i < pDispInfo->data_len; i++)
			{
				if(DISPLAY_GET_SIGNAL(pDispInfo->data[i])==DISPLAY_SIGNAL_ANALOG)
				{
					//printf("livechnn=%d\n",DISPLAY_GET_CHNN(pDispInfo->data[i]));
					blive =true;
					break;
				}
			}
						
			CMessageMan::Instance()->ReleaseMsgBuff(msgData);
			if(blive)
			{
				return KEY_VIRTUAL_MSG_STOP;
			}
			
		}
		else//��ȡʧ�ܣ����ټ���
		{
			CMessageMan::Instance()->ReleaseMsgBuff(msgData);
			return KEY_VIRTUAL_MSG_STOP;
		}
	

		unsigned char chnn = 0;
		ULONGLONG chnBits = m_chnnBits;
		do
		{
			if (chnBits & (0x01 << chnn))
			{
				break;
			}

			++ chnn;
		}while(chnn < m_maxChannelNum);

		m_zoomDlg.SetCHInfo(chnn, m_chnnBits, true);
		m_zoomDlg.DoModal(this, CENTER_POS, CENTER_POS, MAX_SCREEN_X, MAX_SCREEN_Y, false);

		GetRootWnd()->Repaint();
	}

	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CPlaybackDlg::OnClickPlayPause()
{
#if !defined(__ASYN_PLAY__)
	if (PLAYBACK_STATUS_STOP == m_curPlaybackStatus)
	{
		
		assert(m_playbackEndTime > m_playbackStartTime);
		
		MESSAGE_CALLBACK  callBack;
		LOCAL_CLINET_PLAYBACK_INFO playbackInfo;

		playbackInfo.playbackType   = m_playType;
		if (m_changeTime > 0)
		{
			playbackInfo.starTime       = m_changeTime;
			playbackInfo.endTime        = m_playbackEndTime;
			playbackInfo.curTime        = m_changeTime;
		}
		else
		{
			playbackInfo.starTime       = m_playbackStartTime;
			playbackInfo.endTime        = m_playbackEndTime;
			playbackInfo.curTime        = m_playbackPlayTime;
		}

		//ͳ�Ƶ�ǰҪ�󲥷�ͨ����Ŀ��ȷ���ָ�ģʽ
		unsigned char channNum = 0;
		for (unsigned char chnn = 0; chnn < m_maxChannelNum; ++chnn)
		{
			if ((m_chnnBits & (((ULONGLONG)1) << chnn)))
			{
				++ channNum;
			}
		}

		if (PLAYBACK_ON_FILE == m_playType)
		{
			assert (NULL != m_pFileList);
			playbackInfo.pFileList = reinterpret_cast<unsigned long>(m_pFileList);
		}
		
		int type=m_playerDlg.GetSPECtype();
		
		playbackInfo.splitMode		= CPlayerDlg::SplitPBToLive(GetSplitMode(channNum),type);
		
		playbackInfo.chnnBits       = m_chnnBits;
		callBack                    = CPlaybackDlg::ProcCallBack;
		playbackInfo.pMsgCallBack   = (unsigned long )(callBack);
		playbackInfo.pCallBackObj   = (unsigned long )this;
		playbackInfo.diskOwnerIndex = m_diskOwnerIndex;
		playbackInfo.bSync          = false;

		//����ָ�ģʽ
		
		m_lastFrameTime             = 0;
		m_preFrameTime              = 0;
		m_secStartTime              = 0;
		m_secEndTime                = 0;
		m_bDragSlider               = false;
		m_bStopPlayMsg              = false;
		m_playerDlg.SetRange(0, SECTIME_LENGTH - 1);		

		MESSAGE_DATA msgData;
		memset(&msgData, 0, sizeof(MESSAGE_DATA));
		CMessageMan::Instance()->SendMsgDataToServer(0, LOCAL_DEVICE_ID, CMD_REQUEST_LOCAL_CLIENT_PLAYBACK_PLAY, (unsigned char *)&playbackInfo, sizeof(playbackInfo), msgData);

		PACKCMD *pCmd = reinterpret_cast<PACKCMD *>(msgData.pData);

		if (CMD_REPLY_LOCAL_CLIENT_PLAYBACK_PLAY_FAIL == (NET_PROTOCOL)pCmd->cmdType)
		{
			m_playerDlg.StopStatus();
			MessageBox(m_pStrTable->String(E_STR_ID_MESSAGE_46), m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK | DEFAULT_ICON_WARNING);
			OnExit();			
		}
		else 
		{
			ULONGLONG outCH = *reinterpret_cast <ULONGLONG *>(msgData.pData + sizeof(PACKCMD));
			//��Ҫ������Ŀ�����������Ϣ
			m_playerDlg.SetCurPlayChnn(outCH);
			m_playerDlg.SetAudioCH();
			SetSplitMode(outCH);
			m_curPlaybackStatus = PLAYBACK_STATUS_PLAYING;
			assert(m_playbackEndTime >= m_playbackStartTime);
			
			m_playerDlg.PlayStatus();
			m_changeTime = 0;
			m_playerDlg.SetCurSplitMode(m_curSplitMode);
		}	

		CMessageMan::Instance()->ReleaseMsgBuff(msgData);

	}
	else if ((PLAYBACK_STATUS_PLAYING == m_curPlaybackStatus) || (PLAYBACK_STATUS_FF == m_curPlaybackStatus) || (PLAYBACK_STATUS_REW == m_curPlaybackStatus)) 
	{
		
		if (m_playerDlg.IsShow() || m_playerDlg.PlayPause())
		{
			MESSAGE_DATA msgData;
			memset(&msgData, 0, sizeof(MESSAGE_DATA));
			CMessageMan::Instance()->SendMsgDataToServer(0, LOCAL_DEVICE_ID, CMD_REQUEST_LOCAL_CLIENT_PLAYBACK_PAUSE, NULL, 0, msgData);

			CMessageMan::Instance()->ReleaseMsgBuff(msgData);

			m_curPlaybackStatus = PLAYBACK_STATUS_PAUSE;
		}
	}
	else if (PLAYBACK_STATUS_PAUSE == m_curPlaybackStatus)
	{
		
		if (m_playerDlg.IsShow() || m_playerDlg.PlayPause())
		{
			if (m_changeTime > 0)
			{
				MESSAGE_DATA msgData;
				memset(&msgData, 0, sizeof(MESSAGE_DATA));
				CMessageMan::Instance()->SendMsgDataToServer(0, LOCAL_DEVICE_ID, CMD_REQUEST_LOCAL_CLIENT_PLAYBACK_CHG_TIME, (unsigned char *)&m_changeTime, sizeof(unsigned long), msgData);

				ULONGLONG outCH = *reinterpret_cast<ULONGLONG *>(msgData.pData + sizeof(PACKCMD));
				CMessageMan::Instance()->ReleaseMsgBuff(msgData);

				if(outCH == 0)
				{
					MessageBox(m_pStrTable->String(E_STR_ID_MESSAGE_46), m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK | DEFAULT_ICON_WARNING);
					OnExit();
				}
				else
				{
					m_lastFrameTime = m_changeTime;  //�޸����⣺�������������ݵ�ʱ���ᣬ�����϶���������֣�ʱ��ָ���ȵ���ԭ����λ�ã�Ȼ���������϶���λ�á�
					//ԭ��m_changeTime ��ʾ�Ƿ��϶���ʱ���ᣬ���Ϊ0����ʾû���϶�����0����ʾ�϶��ĳ��ȡ�
					//m_lastFrameTime ��ʾ��һ�λطŵ�֡ʱ�䡣�϶�������m_lastFrameTime������ǰ��֡ʱ�䣬
					//����ʱ�϶����֡��ʱ�仹û���͹����������ʱˢ��ʱ����Ļ���ʱ��ָ��ͻ�������ǰ��λ�á�������m_lastFrameTime = m_changeTime����ʱ��ָ�벻�����ȴ��µ�֡������
					m_chnnBits = outCH;
					SetSplitMode(outCH);
					m_changeTime = 0;
					m_curPlaybackStatus = PLAYBACK_STATUS_PLAYING;
				}
			}
			else
			{
				MESSAGE_DATA msgData;
				memset(&msgData, 0, sizeof(MESSAGE_DATA));
				CMessageMan::Instance()->SendMsgDataToServer(0, LOCAL_DEVICE_ID, CMD_REQUEST_LOCAL_CLIENT_PLAYBACK_RESUM, NULL, 0, msgData);

				CMessageMan::Instance()->ReleaseMsgBuff(msgData);

				m_curPlaybackStatus = PLAYBACK_STATUS_PLAYING;
			}
		}
	}
	
#else
	m_bSync = m_playerDlg.IsSync();
	m_clickchnnINfo.bsync=m_bSync;
	if (m_bSync)
	{
		if (PLAYBACK_STATUS_STOP == m_curPlaybackStatus)
		{

			assert(m_playbackEndTime > m_playbackStartTime);

			MESSAGE_CALLBACK  callBack;
			LOCAL_CLINET_PLAYBACK_INFO playbackInfo;

			playbackInfo.playbackType   = m_playType;
			if (m_changeTime > 0)
			{
				playbackInfo.starTime       = m_changeTime;
				playbackInfo.endTime        = m_playbackEndTime;
				playbackInfo.curTime        = m_changeTime;
			}
			else
			{
				playbackInfo.starTime       = m_playbackStartTime;
				playbackInfo.endTime        = m_playbackEndTime;
				playbackInfo.curTime        = m_playbackPlayTime;
			}

			//ͳ�Ƶ�ǰҪ�󲥷�ͨ����Ŀ��ȷ���ָ�ģʽ
			unsigned char channNum = 0;
			for (unsigned char chnn = 0; chnn < m_maxChannelNum; ++chnn)
			{
				if ((m_chnnBits & (((ULONGLONG)1) << chnn)))
				{
					++ channNum;
				}
			}

			if (PLAYBACK_ON_FILE == m_playType)
			{
				assert (NULL != m_pFileList);
				playbackInfo.pFileList = reinterpret_cast<unsigned long>(m_pFileList);
			}

			int type=m_playerDlg.GetSPECtype();

			playbackInfo.splitMode		= CPlayerDlg::SplitPBToLive(GetSplitMode(channNum),type);

			playbackInfo.chnnBits       = m_chnnBits;
			callBack                    = CPlaybackDlg::ProcCallBack;
			playbackInfo.pMsgCallBack   = (unsigned long )(callBack);
			playbackInfo.pCallBackObj   = (unsigned long )this;
			playbackInfo.diskOwnerIndex = m_diskOwnerIndex;
			playbackInfo.bSync			= true;

			//����ָ�ģʽ

			m_lastFrameTime             = 0;
			m_preFrameTime              = 0;
			m_secStartTime              = 0;
			m_secEndTime                = 0;
			m_bDragSlider               = false;
			m_bStopPlayMsg              = false;
			m_playerDlg.SetRange(0, SECTIME_LENGTH - 1);		

			MESSAGE_DATA msgData;
			memset(&msgData, 0, sizeof(MESSAGE_DATA));
			CMessageMan::Instance()->SendMsgDataToServer(0, LOCAL_DEVICE_ID, CMD_REQUEST_LOCAL_CLIENT_PLAYBACK_PLAY, (unsigned char *)&playbackInfo, sizeof(playbackInfo), msgData);

			PACKCMD *pCmd = reinterpret_cast<PACKCMD *>(msgData.pData);

			if (CMD_REPLY_LOCAL_CLIENT_PLAYBACK_PLAY_FAIL == (NET_PROTOCOL)pCmd->cmdType)
			{
				m_playerDlg.StopStatus();
				MessageBox(m_pStrTable->String(E_STR_ID_MESSAGE_46), m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK | DEFAULT_ICON_WARNING);
				OnExit();			
			}
			else 
			{
				ULONGLONG outCH = *reinterpret_cast <ULONGLONG *>(msgData.pData + sizeof(PACKCMD));
				//��Ҫ������Ŀ�����������Ϣ
				m_playerDlg.SetCurPlayChnn(outCH);
				m_playerDlg.SetAudioCH();
				SetSplitMode(outCH);
				m_curPlaybackStatus = PLAYBACK_STATUS_PLAYING;
				assert(m_playbackEndTime >= m_playbackStartTime);

				m_playerDlg.PlayStatus();
				m_changeTime = 0;
				m_playerDlg.SetCurSplitMode(m_curSplitMode);
			}	

			CMessageMan::Instance()->ReleaseMsgBuff(msgData);

		}
		else if ((PLAYBACK_STATUS_PLAYING == m_curPlaybackStatus) || (PLAYBACK_STATUS_FF == m_curPlaybackStatus) || (PLAYBACK_STATUS_REW == m_curPlaybackStatus)) 
		{

			if (m_playerDlg.IsShow() || m_playerDlg.PlayPause())
			{
				MESSAGE_DATA msgData;
				memset(&msgData, 0, sizeof(MESSAGE_DATA));
				m_clickchnnINfo.speed = PLAY_SPEED_PAUSE;
				CMessageMan::Instance()->SendMsgDataToServer(0, LOCAL_DEVICE_ID, CMD_REQUEST_LOCAL_CLIENT_PLAYBACK_PAUSE, (unsigned char*)(&m_clickchnnINfo), sizeof(CURCHNNINFO), msgData);

				CMessageMan::Instance()->ReleaseMsgBuff(msgData);

				m_curPlaybackStatus = PLAYBACK_STATUS_PAUSE;
			}
		}
		else if (PLAYBACK_STATUS_PAUSE == m_curPlaybackStatus)
		{
			
			if (m_playerDlg.IsShow() || m_playerDlg.PlayPause())
			{
				if (m_changeTime > 0)
				{
					MESSAGE_DATA msgData;
					PLAYBACK_CHANGETIME_INFO changeTime;
					changeTime.chnn = m_clickchnnINfo.clickchnn;
					changeTime.time = m_changeTime;
					changeTime.bsync =m_bSync;
					changeTime.bpause = true;
					memset(&msgData, 0, sizeof(MESSAGE_DATA));
		
					CMessageMan::Instance()->SendMsgDataToServer(0, LOCAL_DEVICE_ID, CMD_REQUEST_LOCAL_CLIENT_PLAYBACK_CHG_TIME, (unsigned char *)&changeTime, sizeof(PLAYBACK_CHANGETIME_INFO), msgData);

					ULONGLONG outCH = *reinterpret_cast<ULONGLONG *>(msgData.pData + sizeof(PACKCMD));
					CMessageMan::Instance()->ReleaseMsgBuff(msgData);

					if(outCH == 0)
					{
						MessageBox(m_pStrTable->String(E_STR_ID_MESSAGE_46), m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK | DEFAULT_ICON_WARNING);
						OnExit();
					}
					else
					{
						m_lastFrameTime = m_changeTime;  //�޸����⣺�������������ݵ�ʱ���ᣬ�����϶���������֣�ʱ��ָ���ȵ���ԭ����λ�ã�Ȼ���������϶���λ�á�
						//ԭ��m_changeTime ��ʾ�Ƿ��϶���ʱ���ᣬ���Ϊ0����ʾû���϶�����0����ʾ�϶��ĳ��ȡ�
						//m_lastFrameTime ��ʾ��һ�λطŵ�֡ʱ�䡣�϶�������m_lastFrameTime������ǰ��֡ʱ�䣬
						//����ʱ�϶����֡��ʱ�仹û���͹����������ʱˢ��ʱ����Ļ���ʱ��ָ��ͻ�������ǰ��λ�á�������m_lastFrameTime = m_changeTime����ʱ��ָ�벻�����ȴ��µ�֡������
						m_chnnBits = outCH;
						SetSplitMode(outCH);
						m_changeTime = 0;
						m_curPlaybackStatus = PLAYBACK_STATUS_PLAYING;
					}
				}
				else
				{
					MESSAGE_DATA msgData;
					memset(&msgData, 0, sizeof(MESSAGE_DATA));
					
					CMessageMan::Instance()->SendMsgDataToServer(0, LOCAL_DEVICE_ID, CMD_REQUEST_LOCAL_CLIENT_PLAYBACK_RESUM, (unsigned char*)(&m_clickchnnINfo), sizeof(CURCHNNINFO), msgData);

					CMessageMan::Instance()->ReleaseMsgBuff(msgData);

					m_curPlaybackStatus = PLAYBACK_STATUS_PLAYING;
				}
			}
			GetRootWnd()->Repaint();
		}
	}
	else
	{
		//��ȫ��ȫ��ͨ��ֹͣ������£���ȥ�����طš�
		bool bStop = true;
		for (int i = 0; i < g_p_login_succ->productInfo.localVideoInputNum; i++)
		{
			if (m_curStatus[i] != PLAYBACK_STATUS_STOP)
			{
				bStop = false;
			}
		}
		if (bStop)
		{
			assert(m_playbackEndTime > m_playbackStartTime);

			MESSAGE_CALLBACK  callBack;
			LOCAL_CLINET_PLAYBACK_INFO playbackInfo;

			playbackInfo.playbackType   = m_playType;
			if (m_changeTime > 0)
			{
				playbackInfo.starTime       = m_changeTime;
				playbackInfo.endTime        = m_playbackEndTime;
				playbackInfo.curTime        = m_changeTime;
			}
			else
			{
				playbackInfo.starTime       = m_playbackStartTime;
				playbackInfo.endTime        = m_playbackEndTime;
				playbackInfo.curTime        = m_playbackPlayTime;
			}

			//ͳ�Ƶ�ǰҪ�󲥷�ͨ����Ŀ��ȷ���ָ�ģʽ
			unsigned char channNum = 0;
			for (unsigned char chnn = 0; chnn < m_maxChannelNum; ++chnn)
			{
				if ((m_chnnBits & (((ULONGLONG)1) << chnn)))
				{
					++ channNum;
				}
			}

			if (PLAYBACK_ON_FILE == m_playType)
			{
				assert (NULL != m_pFileList);
				playbackInfo.pFileList = reinterpret_cast<unsigned long>(m_pFileList);
			}

			int type=m_playerDlg.GetSPECtype();

			playbackInfo.splitMode		= CPlayerDlg::SplitPBToLive(GetSplitMode(channNum),type);

			playbackInfo.chnnBits       = m_chnnBits;
			callBack                    = CPlaybackDlg::ProcCallBack;
			playbackInfo.pMsgCallBack   = (unsigned long )(callBack);
			playbackInfo.pCallBackObj   = (unsigned long )this;
			playbackInfo.diskOwnerIndex = m_diskOwnerIndex;
			playbackInfo.bSync			= false;

			//����ָ�ģʽ

			m_lastFrameTime             = 0;
			m_preFrameTime              = 0;
			m_secStartTime              = 0;
			m_secEndTime                = 0;
			m_bDragSlider               = false;
			m_bStopPlayMsg              = false;
			m_playerDlg.SetRange(0, SECTIME_LENGTH - 1);		

			MESSAGE_DATA msgData;
			memset(&msgData, 0, sizeof(MESSAGE_DATA));
			CMessageMan::Instance()->SendMsgDataToServer(0, LOCAL_DEVICE_ID, CMD_REQUEST_LOCAL_CLIENT_PLAYBACK_PLAY, (unsigned char *)&playbackInfo, sizeof(playbackInfo), msgData);

			PACKCMD *pCmd = reinterpret_cast<PACKCMD *>(msgData.pData);

			if (CMD_REPLY_LOCAL_CLIENT_PLAYBACK_PLAY_FAIL == (NET_PROTOCOL)pCmd->cmdType)
			{
				m_playerDlg.StopStatus();
				MessageBox(m_pStrTable->String(E_STR_ID_MESSAGE_46), m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK | DEFAULT_ICON_WARNING);
				OnExit();			
			}
			else 
			{
				ULONGLONG outCH = *reinterpret_cast <ULONGLONG *>(msgData.pData + sizeof(PACKCMD));
				//��Ҫ������Ŀ�����������Ϣ
				m_playerDlg.SetCurPlayChnn(outCH);
				m_playerDlg.SetAudioCH();
				SetSplitMode(outCH);
				for (int i = 0; i < g_p_login_succ->productInfo.localVideoInputNum; i++)
				{
					m_curStatus[i] = PLAYBACK_STATUS_PLAYING;
				}
				assert(m_playbackEndTime >= m_playbackStartTime);

				m_playerDlg.PlayStatus();
				m_changeTime = 0;
				m_playerDlg.SetCurSplitMode(m_curSplitMode);
			}	

			CMessageMan::Instance()->ReleaseMsgBuff(msgData);

		}
		else if ((PLAYBACK_STATUS_PLAYING == m_curStatus[m_clickchnnINfo.clickchnn]) || (PLAYBACK_STATUS_FF == m_curStatus[m_clickchnnINfo.clickchnn]) || (PLAYBACK_STATUS_REW == m_curStatus[m_clickchnnINfo.clickchnn])) 
		{

			if (m_playerDlg.IsShow() || m_playerDlg.PlayPause())
			{
				MESSAGE_DATA msgData;
				memset(&msgData, 0, sizeof(MESSAGE_DATA));
				m_clickchnnINfo.speed = PLAY_SPEED_PAUSE;
				CMessageMan::Instance()->SendMsgDataToServer(0, LOCAL_DEVICE_ID, CMD_REQUEST_LOCAL_CLIENT_PLAYBACK_PAUSE, (unsigned char*)(&m_clickchnnINfo), sizeof(CURCHNNINFO), msgData);

				CMessageMan::Instance()->ReleaseMsgBuff(msgData);

				m_curStatus[m_clickchnnINfo.clickchnn] = PLAYBACK_STATUS_PAUSE;
			}
		}
		else if (PLAYBACK_STATUS_PAUSE == m_curStatus[m_clickchnnINfo.clickchnn])
		{

			if (m_playerDlg.IsShow() || m_playerDlg.PlayPause())
			{
				if (m_changeTime > 0)
				{
					MESSAGE_DATA msgData;
					PLAYBACK_CHANGETIME_INFO changeTime;
					changeTime.chnn = m_clickchnnINfo.clickchnn;
					changeTime.bsync = m_bSync;
					changeTime.time = m_changeTime;
					changeTime.bpause = true;
					memset(&msgData, 0, sizeof(MESSAGE_DATA));
					CMessageMan::Instance()->SendMsgDataToServer(0, LOCAL_DEVICE_ID, CMD_REQUEST_LOCAL_CLIENT_PLAYBACK_CHG_TIME, (unsigned char *)&changeTime, sizeof(PLAYBACK_CHANGETIME_INFO), msgData);

					ULONGLONG outCH = *reinterpret_cast<ULONGLONG *>(msgData.pData + sizeof(PACKCMD));
					CMessageMan::Instance()->ReleaseMsgBuff(msgData);

					if(outCH == 0)
					{
						MessageBox(m_pStrTable->String(E_STR_ID_MESSAGE_46), m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK | DEFAULT_ICON_WARNING);
						OnExit();
					}
					else
					{
						m_lastFrameTime = m_changeTime;  //�޸����⣺�������������ݵ�ʱ���ᣬ�����϶���������֣�ʱ��ָ���ȵ���ԭ����λ�ã�Ȼ���������϶���λ�á�
						//ԭ��m_changeTime ��ʾ�Ƿ��϶���ʱ���ᣬ���Ϊ0����ʾû���϶�����0����ʾ�϶��ĳ��ȡ�
						//m_lastFrameTime ��ʾ��һ�λطŵ�֡ʱ�䡣�϶�������m_lastFrameTime������ǰ��֡ʱ�䣬
						//����ʱ�϶����֡��ʱ�仹û���͹����������ʱˢ��ʱ����Ļ���ʱ��ָ��ͻ�������ǰ��λ�á�������m_lastFrameTime = m_changeTime����ʱ��ָ�벻�����ȴ��µ�֡������
						m_chnnBits = outCH;
						SetSplitMode(outCH);
						m_changeTime = 0;
						m_curStatus[m_clickchnnINfo.clickchnn] = PLAYBACK_STATUS_PLAYING;
					}
				}
				else
				{
					MESSAGE_DATA msgData;
					memset(&msgData, 0, sizeof(MESSAGE_DATA));
					CMessageMan::Instance()->SendMsgDataToServer(0, LOCAL_DEVICE_ID, CMD_REQUEST_LOCAL_CLIENT_PLAYBACK_RESUM, (unsigned char*)(&m_clickchnnINfo), sizeof(CURCHNNINFO), msgData);

					CMessageMan::Instance()->ReleaseMsgBuff(msgData);

					m_curStatus[m_clickchnnINfo.clickchnn] = PLAYBACK_STATUS_PLAYING;
				}
			}
			GetRootWnd()->Repaint();
		}	
	}	

#endif
	
	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CPlaybackDlg::OnClickStop()
{
#if !defined(__ASYN_PLAY__)
	if (PLAYBACK_STATUS_STOP != m_curPlaybackStatus)
	{
		MESSAGE_DATA msgData;
		memset(&msgData, 0, sizeof(MESSAGE_DATA));
		CMessageMan::Instance()->SendMsgDataToServer(0, LOCAL_DEVICE_ID, CMD_REQUEST_LOCAL_CLIENT_PLAYBACK_STOP, NULL, 0, msgData);
		CMessageMan::Instance()->ReleaseMsgBuff(msgData);

		m_curPlaybackStatus = PLAYBACK_STATUS_STOP;
		m_playerDlg.StopStatus();
	}

	return KEY_VIRTUAL_MSG_STOP;
#else
	if (m_bSync)
	{
		if (PLAYBACK_STATUS_STOP != m_curPlaybackStatus)
		{
			MESSAGE_DATA msgData;
			memset(&msgData, 0, sizeof(MESSAGE_DATA));
			CMessageMan::Instance()->SendMsgDataToServer(0, LOCAL_DEVICE_ID, CMD_REQUEST_LOCAL_CLIENT_PLAYBACK_STOP, NULL, 0, msgData);
			CMessageMan::Instance()->ReleaseMsgBuff(msgData);

			m_curPlaybackStatus = PLAYBACK_STATUS_STOP;
			m_playerDlg.StopStatus();
		}
	 }
	else
	{
		MESSAGE_DATA msgData;
		memset(&msgData, 0, sizeof(MESSAGE_DATA));
		CMessageMan::Instance()->SendMsgDataToServer(0, LOCAL_DEVICE_ID, CMD_REQUEST_LOCAL_CLIENT_PLAYBACK_STOP, NULL, 0, msgData);
		CMessageMan::Instance()->ReleaseMsgBuff(msgData);
		for (int i = 0; i < g_p_login_succ->productInfo.localVideoInputNum; i++)
		{
			m_curStatus[i] = PLAYBACK_STATUS_STOP;
		}
		m_playerDlg.StopStatus();
	}
	

	 return KEY_VIRTUAL_MSG_STOP;
#endif
}

unsigned long CPlaybackDlg::OnClickNextFrame()
{
#if !defined(__ASYN_PLAY__)
	if (PLAYBACK_STATUS_PAUSE == m_curPlaybackStatus)
	{
		if (m_playerDlg.IsShow() || m_playerDlg.NextFrame())
		{
			MESSAGE_DATA msgData;
			memset(&msgData, 0, sizeof(MESSAGE_DATA));
			CMessageMan::Instance()->SendMsgDataToServer(0, LOCAL_DEVICE_ID, CMD_REQUEST_LOCAL_CLIENT_PLAYBACK_NEXTFRAME, NULL, 0, msgData);

			CMessageMan::Instance()->ReleaseMsgBuff(msgData);
		}
	}

	return KEY_VIRTUAL_MSG_STOP;
#else

	if (m_bSync)
	{
		if (PLAYBACK_STATUS_PAUSE == m_curPlaybackStatus)
		{
			if (m_playerDlg.IsShow() || m_playerDlg.NextFrame())
			{
				MESSAGE_DATA msgData;
				memset(&msgData, 0, sizeof(MESSAGE_DATA));
				CMessageMan::Instance()->SendMsgDataToServer(0, LOCAL_DEVICE_ID, CMD_REQUEST_LOCAL_CLIENT_PLAYBACK_NEXTFRAME, (unsigned char*)&(m_clickchnnINfo.clickchnn), sizeof(unsigned long),  msgData);

				CMessageMan::Instance()->ReleaseMsgBuff(msgData);
			}
		}
	}
	else
	{
		if (PLAYBACK_STATUS_PAUSE == m_curStatus[m_clickchnnINfo.clickchnn])
		{
			if (m_playerDlg.IsShow() || m_playerDlg.NextFrame())
			{
				MESSAGE_DATA msgData;
				memset(&msgData, 0, sizeof(MESSAGE_DATA));
				CMessageMan::Instance()->SendMsgDataToServer(0, LOCAL_DEVICE_ID, CMD_REQUEST_LOCAL_CLIENT_PLAYBACK_NEXTFRAME, (unsigned char*)&(m_clickchnnINfo.clickchnn), sizeof(unsigned long),  msgData);

				CMessageMan::Instance()->ReleaseMsgBuff(msgData);
			}
		}
	}	


	 return KEY_VIRTUAL_MSG_STOP;
#endif
}

// zhl 2010-03-05 ����ʱ�������ǰ���Ϳ��ٺ���
// �������������豸�������塢485���̡�ң�������Ĵ���ʽ��һ��
// ����Ի����������Ի�����������ʱm_playerDlg�Ի��������õ������ȱ�ˢ�£���ý���Ĳ�������Ӧ
// �������豸����ʱֻ�иý������Ӧ������Ӧ������m_playerDlg�����ϵ�����û��ˢ�¾ͻ�ã��Ӷ�����
// ������������������������������Ĳ������ǣ�0��0������������������������������豸
// ������������ʱҪ��ˢ��m_playerDlg�����ڴ�����ض���
// ��

unsigned long CPlaybackDlg::OnClickFastForward()
{
#if !defined(__ASYN_PLAY__)
	if ((PLAYBACK_STATUS_PLAYING == m_curPlaybackStatus) || (PLAYBACK_STATUS_FF == m_curPlaybackStatus))
	{
		if (m_playerDlg.IsShow() || m_playerDlg.ChangeSpeed(true))
		{
			unsigned long speed = m_playerDlg.GetCurSpeed(true);

			MESSAGE_DATA msgData;
			memset(&msgData, 0, sizeof(MESSAGE_DATA));
			CMessageMan::Instance()->SendMsgDataToServer(0, LOCAL_DEVICE_ID, CMD_REQUEST_LOCAL_CLIENT_PLAYBACK_FF, (unsigned char*)(&speed), sizeof(speed), msgData);

			CMessageMan::Instance()->ReleaseMsgBuff(msgData);

			if (PLAY_SPEED_1X == speed)
			{
				m_curPlaybackStatus = PLAYBACK_STATUS_PLAYING;
				m_playerDlg.PlayStatus();
			}
			else
			{
				m_curPlaybackStatus = PLAYBACK_STATUS_FF;
			}	
		}
	}
	
	return KEY_VIRTUAL_MSG_STOP;
#else
	if (m_bSync)
	{
		if ((PLAYBACK_STATUS_PLAYING == m_curPlaybackStatus) || (PLAYBACK_STATUS_FF == m_curPlaybackStatus))
		{
			if (m_playerDlg.IsShow() || m_playerDlg.ChangeSpeed(true))
			{
				unsigned long speed = m_playerDlg.GetCurSpeed(true);
				PLAYBACK_SPEED_INFO speedinfo;
				speedinfo.speed =speed;
				speedinfo.chnn =0xFF;
				MESSAGE_DATA msgData;
				memset(&msgData, 0, sizeof(MESSAGE_DATA));
				CMessageMan::Instance()->SendMsgDataToServer(0, LOCAL_DEVICE_ID, CMD_REQUEST_LOCAL_CLIENT_PLAYBACK_FF, (unsigned char*)(&speedinfo), sizeof(PLAYBACK_SPEED_INFO), msgData);

				CMessageMan::Instance()->ReleaseMsgBuff(msgData);

				if (PLAY_SPEED_1X == speed)
				{
					m_curPlaybackStatus = PLAYBACK_STATUS_PLAYING;
					m_playerDlg.PlayStatus();
				}
				else
				{
					m_curPlaybackStatus = PLAYBACK_STATUS_FF;
				}	
			}
			GetRootWnd()->Repaint();
		}
	 }
	else
	{
		if ((PLAYBACK_STATUS_PLAYING == m_curStatus[m_clickchnnINfo.clickchnn]) || (PLAYBACK_STATUS_FF == m_curStatus[m_clickchnnINfo.clickchnn]))
		{
			if (m_playerDlg.IsShow() || m_playerDlg.ChangeSpeed(true))
			{
				unsigned long speed = m_playerDlg.GetCurSpeed(true);
				PLAYBACK_SPEED_INFO speedinfo;
				speedinfo.speed =speed;
				speedinfo.chnn =m_clickchnnINfo.clickchnn;
				MESSAGE_DATA msgData;
				memset(&msgData, 0, sizeof(MESSAGE_DATA));
				CMessageMan::Instance()->SendMsgDataToServer(0, LOCAL_DEVICE_ID, CMD_REQUEST_LOCAL_CLIENT_PLAYBACK_FF, (unsigned char*)(&speedinfo), sizeof(PLAYBACK_SPEED_INFO), msgData);

				CMessageMan::Instance()->ReleaseMsgBuff(msgData);

				if (PLAY_SPEED_1X == speed)
				{
					m_curStatus[m_clickchnnINfo.clickchnn] = PLAYBACK_STATUS_PLAYING;
					m_playerDlg.PlayStatus();
				}
				else
				{
					m_curStatus[m_clickchnnINfo.clickchnn] = PLAYBACK_STATUS_FF;
				}
				GetRootWnd()->Repaint();
			}
		}
	 }


	  return KEY_VIRTUAL_MSG_STOP;

#endif	


}

unsigned long CPlaybackDlg::OnClickReward()
{
#if !defined(__ASYN_PLAY__)
	if ((PLAYBACK_STATUS_PLAYING == m_curPlaybackStatus) || (PLAYBACK_STATUS_REW == m_curPlaybackStatus)) 
	{
		if (m_playerDlg.IsShow() || m_playerDlg.ChangeSpeed(false))
		{
			unsigned long speed = m_playerDlg.GetCurSpeed(false);

			MESSAGE_DATA msgData;
			if (PLAY_SPEED_1X == speed)
			{
				memset(&msgData, 0, sizeof(MESSAGE_DATA));
				CMessageMan::Instance()->SendMsgDataToServer(0, LOCAL_DEVICE_ID, CMD_REQUEST_LOCAL_CLIENT_PLAYBACK_FF, (unsigned char*)(&speed), sizeof(speed), msgData);
				CMessageMan::Instance()->ReleaseMsgBuff(msgData);
				m_curPlaybackStatus = PLAYBACK_STATUS_PLAYING;
				m_playerDlg.PlayStatus();
			}
			else
			{
				memset(&msgData, 0, sizeof(MESSAGE_DATA));
				CMessageMan::Instance()->SendMsgDataToServer(0, LOCAL_DEVICE_ID, CMD_REQUEST_LOCAL_CLIENT_PLAYBACK_REW, (unsigned char*)(&speed), sizeof(speed), msgData);
				CMessageMan::Instance()->ReleaseMsgBuff(msgData);		
				m_curPlaybackStatus = PLAYBACK_STATUS_REW;
			}
		}
	}

	return KEY_VIRTUAL_MSG_STOP;
#else
	if (m_bSync)
	{
		if ((PLAYBACK_STATUS_PLAYING == m_curPlaybackStatus) || (PLAYBACK_STATUS_REW == m_curPlaybackStatus)) 
		{
			if (m_playerDlg.IsShow() || m_playerDlg.ChangeSpeed(false))
			{
				unsigned long speed = m_playerDlg.GetCurSpeed(false);
				PLAYBACK_SPEED_INFO speedinfo;
				speedinfo.speed =speed;
				speedinfo.chnn = 0xFF;
				MESSAGE_DATA msgData;
				if (PLAY_SPEED_1X == speed)
				{
					memset(&msgData, 0, sizeof(MESSAGE_DATA));
					CMessageMan::Instance()->SendMsgDataToServer(0, LOCAL_DEVICE_ID, CMD_REQUEST_LOCAL_CLIENT_PLAYBACK_FF, (unsigned char*)(&speedinfo), sizeof(PLAYBACK_SPEED_INFO), msgData);
					CMessageMan::Instance()->ReleaseMsgBuff(msgData);
					m_curPlaybackStatus = PLAYBACK_STATUS_PLAYING;
					m_playerDlg.PlayStatus();
				}
				else
				{
					memset(&msgData, 0, sizeof(MESSAGE_DATA));
					CMessageMan::Instance()->SendMsgDataToServer(0, LOCAL_DEVICE_ID, CMD_REQUEST_LOCAL_CLIENT_PLAYBACK_REW, (unsigned char*)(&speedinfo), sizeof(PLAYBACK_SPEED_INFO), msgData);
					CMessageMan::Instance()->ReleaseMsgBuff(msgData);		
					m_curPlaybackStatus = PLAYBACK_STATUS_REW;
				}
			}
			GetRootWnd()->Repaint();
		}
	}
	else
	{
		if ((PLAYBACK_STATUS_PLAYING == m_curStatus[m_clickchnnINfo.clickchnn]) || (PLAYBACK_STATUS_REW == m_curStatus[m_clickchnnINfo.clickchnn])) 
		{
			if (m_playerDlg.IsShow() || m_playerDlg.ChangeSpeed(false))
			{
				unsigned long speed = m_playerDlg.GetCurSpeed(false);
				PLAYBACK_SPEED_INFO speedinfo;
				speedinfo.speed =speed;
				speedinfo.chnn = m_clickchnnINfo.clickchnn;
				MESSAGE_DATA msgData;
				if (PLAY_SPEED_1X == speed)
				{
					memset(&msgData, 0, sizeof(MESSAGE_DATA));
					CMessageMan::Instance()->SendMsgDataToServer(0, LOCAL_DEVICE_ID, CMD_REQUEST_LOCAL_CLIENT_PLAYBACK_FF, (unsigned char*)(&speedinfo), sizeof(PLAYBACK_SPEED_INFO), msgData);
					CMessageMan::Instance()->ReleaseMsgBuff(msgData);
					m_curStatus[m_clickchnnINfo.clickchnn] = PLAYBACK_STATUS_PLAYING;
					m_playerDlg.PlayStatus();
				}
				else
				{
					memset(&msgData, 0, sizeof(MESSAGE_DATA));
					CMessageMan::Instance()->SendMsgDataToServer(0, LOCAL_DEVICE_ID, CMD_REQUEST_LOCAL_CLIENT_PLAYBACK_REW, (unsigned char*)(&speedinfo), sizeof(PLAYBACK_SPEED_INFO), msgData);
					CMessageMan::Instance()->ReleaseMsgBuff(msgData);		
					m_curStatus[m_clickchnnINfo.clickchnn] = PLAYBACK_STATUS_REW;
				}
			}
			GetRootWnd()->Repaint();
		}
	}	
	return KEY_VIRTUAL_MSG_STOP;
#endif
  
}

unsigned long CPlaybackDlg::OnClickSplit(unsigned long keyID)
{
#if !defined(__ASYN_PLAY__)
	if (PLAYBACK_STATUS_PLAYING == m_curPlaybackStatus)
	{
		if (m_playerDlg.IsShow() || m_playerDlg.ChangeCH(keyID))
		{
			LOCAL_CLINET_PB_CHG_CH_INFO info;
			int type=m_playerDlg.GetSPECtype();
			info.chnnBits = m_playerDlg.GetCurPlayChnn(info.splitMode,type);
			
			
			MESSAGE_DATA msgData;
			memset(&msgData, 0, sizeof(MESSAGE_DATA));
			CMessageMan::Instance()->SendMsgDataToServer(0, LOCAL_DEVICE_ID, CMD_REQUEST_LOCAL_CLIENT_PLAYBACK_CHG_CHNN, (unsigned char *)&info, sizeof(LOCAL_CLINET_PB_CHG_CH_INFO), msgData);

			ULONGLONG outCH = *reinterpret_cast<ULONGLONG *>(msgData.pData + sizeof(PACKCMD));
			CMessageMan::Instance()->ReleaseMsgBuff(msgData);

			if(outCH == 0)
			{
				MessageBox(m_pStrTable->String(E_STR_ID_MESSAGE_46), m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK | DEFAULT_ICON_WARNING);
				OnExit();
			}
			else
			{
				m_chnnBits = outCH;
				SetSplitMode(outCH);
#if defined(__NEW_CUT_BACKUP__)				
				for(int i=0;i<m_maxChannelNum;i++)
				{
					if (m_chnnBits & (((LONGLONG)0x01) << i))
					{
						m_clickchnnINfo.clickchnn=i;//�ҵ���С��ͨ������������Ϊ���
						break;
					}
				}
				m_playerDlg.SetCurSelectChnn(m_clickchnnINfo.clickchnn);
				memset(&msgData, 0, sizeof(MESSAGE_DATA));
				CMessageMan::Instance()->SendMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_LOCAL_CLIENT_PLAYBACK_CHNINIT, (unsigned char *)&(m_clickchnnINfo.clickchnn), sizeof(unsigned long), msgData);
				CMessageMan::Instance()->ReleaseMsgBuff(msgData);
#endif
			}		
		}
	}
	
	return KEY_VIRTUAL_MSG_STOP;

#else

	if (m_bSync)
	{
		if (PLAYBACK_STATUS_PLAYING == m_curPlaybackStatus)
		{
			if (m_playerDlg.IsShow() || m_playerDlg.ChangeCH(keyID))
			{
				LOCAL_CLINET_PB_CHG_CH_INFO info;
				int type=m_playerDlg.GetSPECtype();
				info.chnnBits = m_playerDlg.GetCurPlayChnn(info.splitMode,type);
				

				MESSAGE_DATA msgData;
				memset(&msgData, 0, sizeof(MESSAGE_DATA));
				CMessageMan::Instance()->SendMsgDataToServer(0, LOCAL_DEVICE_ID, CMD_REQUEST_LOCAL_CLIENT_PLAYBACK_CHG_CHNN, (unsigned char *)&info, sizeof(LOCAL_CLINET_PB_CHG_CH_INFO), msgData);

				ULONGLONG outCH = *reinterpret_cast<ULONGLONG *>(msgData.pData + sizeof(PACKCMD));
				CMessageMan::Instance()->ReleaseMsgBuff(msgData);

				//���ͼٵ�ͨ���Ÿ�fb,�ı��л�ʱ�򲻵����״̬
				unsigned long testchnn=0;
				CMessageMan::Instance()->SendMsgDataToServer(0, LOCAL_DEVICE_ID, CMD_REQUEST_LOCAL_CLIENT_PLAYBACK_CHNINIT, (unsigned char *)&testchnn, sizeof(unsigned long), msgData);
				CMessageMan::Instance()->ReleaseMsgBuff(msgData);

				if(outCH == 0)
				{
					MessageBox(m_pStrTable->String(E_STR_ID_MESSAGE_46), m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK | DEFAULT_ICON_WARNING);
					OnExit();
				}
				else
				{
					m_chnnBits = outCH;
					SetSplitMode(outCH);
#if defined(__NEW_CUT_BACKUP__)				
					for(int i=0;i<m_maxChannelNum;i++)
					{
						if (m_chnnBits & (((LONGLONG)0x01) << i))
						{
							m_clickchnnINfo.clickchnn=i;//�ҵ���С��ͨ������������Ϊ���
							break;
						}
					}
					m_playerDlg.SetCurSelectChnn(m_clickchnnINfo.clickchnn);
					memset(&msgData, 0, sizeof(MESSAGE_DATA));
					CMessageMan::Instance()->SendMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_LOCAL_CLIENT_PLAYBACK_CHNINIT, (unsigned char *)&m_clickchnnINfo.clickchnn, sizeof(unsigned long), msgData);
					CMessageMan::Instance()->ReleaseMsgBuff(msgData);
#endif
				}
			}
		}
	}

	
	return KEY_VIRTUAL_MSG_STOP;
#endif
}

/***************************************************************************************
������OnNextSplit()
���ܣ�����ʱ�ڷָ�ģʽ1/4/9/16֮���л�, ֻ����ƽplayerDlg.cpp����ʱ��Ч����δ����ʱ��ȻΪ�������ҹ��ܡ�
	  ֻ��1UTW�������д˰���
zhl 2010-06-07
***************************************************************************************/
unsigned long CPlaybackDlg::OnNextSplit()
{
#if !defined(__ASYN_PLAY__)
	if (PLAYBACK_STATUS_PLAYING == m_curPlaybackStatus)
	{
		if (!m_playerDlg.IsShow() && m_playerDlg.NextSplit())
		{
			LOCAL_CLINET_PB_CHG_CH_INFO info;
			int type=m_playerDlg.GetSPECtype();
			info.chnnBits = m_playerDlg.GetCurPlayChnn(info.splitMode,type);
			

			MESSAGE_DATA msgData;
			memset(&msgData, 0, sizeof(MESSAGE_DATA));
			CMessageMan::Instance()->SendMsgDataToServer(0, LOCAL_DEVICE_ID, CMD_REQUEST_LOCAL_CLIENT_PLAYBACK_CHG_CHNN, (unsigned char *)&info, sizeof(LOCAL_CLINET_PB_CHG_CH_INFO), msgData);

			ULONGLONG outCH = *reinterpret_cast<ULONGLONG *>(msgData.pData + sizeof(PACKCMD));
			CMessageMan::Instance()->ReleaseMsgBuff(msgData);

			if(outCH == 0)
			{
				MessageBox(m_pStrTable->String(E_STR_ID_MESSAGE_46), m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK | DEFAULT_ICON_WARNING);
				OnExit();
			}
			else
			{
				m_chnnBits = outCH;
				SetSplitMode(outCH);
			}
			
		}
	}

	return KEY_VIRTUAL_MSG_STOP;
#else
	if (m_bSync)
	{
		if (PLAYBACK_STATUS_PLAYING == m_curPlaybackStatus)
		{
			if (!m_playerDlg.IsShow() && m_playerDlg.NextSplit())
			{
				LOCAL_CLINET_PB_CHG_CH_INFO info;
				int type=m_playerDlg.GetSPECtype();
				info.chnnBits = m_playerDlg.GetCurPlayChnn(info.splitMode,type);
				MESSAGE_DATA msgData;
				memset(&msgData, 0, sizeof(MESSAGE_DATA));
				CMessageMan::Instance()->SendMsgDataToServer(0, LOCAL_DEVICE_ID, CMD_REQUEST_LOCAL_CLIENT_PLAYBACK_CHG_CHNN, (unsigned char *)&info, sizeof(LOCAL_CLINET_PB_CHG_CH_INFO), msgData);

				ULONGLONG outCH = *reinterpret_cast<ULONGLONG *>(msgData.pData + sizeof(PACKCMD));
				CMessageMan::Instance()->ReleaseMsgBuff(msgData);

				if(outCH == 0)
				{
					MessageBox(m_pStrTable->String(E_STR_ID_MESSAGE_46), m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK | DEFAULT_ICON_WARNING);
					OnExit();
				}
				else
				{
					m_chnnBits = outCH;
					SetSplitMode(outCH);
				}

			}
		}
	}

	 return KEY_VIRTUAL_MSG_STOP;
#endif	
}


/***************************************************************************************
������OnNextSplit()
���ܣ�����ʱ�ڵ�ǰ�ָ�ģʽ�·�ҳ, ֻ����ƽplayerDlg.cpp����ʱ��Ч����δ����ʱ��ȻΪ�������ҹ��ܡ�
	  ֻ��1UTW�������д˰���
zhl 2010-06-07
***************************************************************************************/
unsigned long CPlaybackDlg::OnNextGroup()
{
#if !defined(__ASYN_PLAY__)
	if (PLAYBACK_STATUS_PLAYING == m_curPlaybackStatus)
	{
		if (!m_playerDlg.IsShow() && m_playerDlg.NextGroup())
		{
			LOCAL_CLINET_PB_CHG_CH_INFO info;
			int type=m_playerDlg.GetSPECtype();
			info.chnnBits = m_playerDlg.GetCurPlayChnn(info.splitMode,type);
			

			MESSAGE_DATA msgData;
			memset(&msgData, 0, sizeof(MESSAGE_DATA));
			CMessageMan::Instance()->SendMsgDataToServer(0, LOCAL_DEVICE_ID, CMD_REQUEST_LOCAL_CLIENT_PLAYBACK_CHG_CHNN, (unsigned char *)&info, sizeof(LOCAL_CLINET_PB_CHG_CH_INFO), msgData);

			ULONGLONG outCH = *reinterpret_cast<ULONGLONG *>(msgData.pData + sizeof(PACKCMD));
			CMessageMan::Instance()->ReleaseMsgBuff(msgData);

			if(outCH == 0)
			{
				MessageBox(m_pStrTable->String(E_STR_ID_MESSAGE_46), m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK | DEFAULT_ICON_WARNING);
				OnExit();
			}
			else
			{
				m_chnnBits = outCH;
				SetSplitMode(outCH);
			}
			
		}
	}

	return KEY_VIRTUAL_MSG_STOP;
#else
	if (m_bSync)
	{
		if (PLAYBACK_STATUS_PLAYING == m_curPlaybackStatus)
		{
			if (!m_playerDlg.IsShow() && m_playerDlg.NextGroup())
			{
				LOCAL_CLINET_PB_CHG_CH_INFO info;
				int type=m_playerDlg.GetSPECtype();
				info.chnnBits = m_playerDlg.GetCurPlayChnn(info.splitMode,type);


				MESSAGE_DATA msgData;
				memset(&msgData, 0, sizeof(MESSAGE_DATA));
				CMessageMan::Instance()->SendMsgDataToServer(0, LOCAL_DEVICE_ID, CMD_REQUEST_LOCAL_CLIENT_PLAYBACK_CHG_CHNN, (unsigned char *)&info, sizeof(LOCAL_CLINET_PB_CHG_CH_INFO), msgData);

				ULONGLONG outCH = *reinterpret_cast<ULONGLONG *>(msgData.pData + sizeof(PACKCMD));
				CMessageMan::Instance()->ReleaseMsgBuff(msgData);

				if(outCH == 0)
				{
					MessageBox(m_pStrTable->String(E_STR_ID_MESSAGE_46), m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK | DEFAULT_ICON_WARNING);
					OnExit();
				}
				else
				{
					m_chnnBits = outCH;
					SetSplitMode(outCH);
				}

			}
		}
	}
	return KEY_VIRTUAL_MSG_STOP;
#endif
	

}

unsigned long CPlaybackDlg::OnClickPreSection()
{
#if !defined(__ASYN_PLAY__)
	tm endTm = DVRTime32ToTm(m_secStartTime);
	if ((0 == endTm.tm_hour) && (0 == endTm.tm_min) && (0 == endTm.tm_sec))
	{
		return KEY_VIRTUAL_MSG_STOP;
	}

	if ((PLAYBACK_STATUS_STOP == m_curPlaybackStatus) || (PLAYBACK_STATUS_PAUSE == m_curPlaybackStatus))
	{
		GetSectionTime(m_secStartTime - 1, m_secStartTime, m_secEndTime);
		m_changeTime = m_secStartTime;
		m_playerDlg.SetSectionTime(m_secStartTime, m_secEndTime);
		m_playerDlg.SetTime(m_changeTime, 0, true);
		return KEY_VIRTUAL_MSG_STOP;
	}

	if ((PLAYBACK_STATUS_PLAYING == m_curPlaybackStatus) || (PLAYBACK_STATUS_FF == m_curPlaybackStatus) || (PLAYBACK_STATUS_REW == m_curPlaybackStatus))
	{
		MESSAGE_DATA msgData;
		memset(&msgData, 0, sizeof(MESSAGE_DATA));
		CMessageMan::Instance()->SendMsgDataToServer(0, LOCAL_DEVICE_ID, CMD_REQUEST_LOCAL_CLIENT_PLAYBACK_PAUSE, NULL, 0, msgData);
		CMessageMan::Instance()->ReleaseMsgBuff(msgData);

		m_changeTime = m_secStartTime - SECTIME_LENGTH;
		memset(&msgData, 0, sizeof(MESSAGE_DATA));		
		CMessageMan::Instance()->SendMsgDataToServer(0, LOCAL_DEVICE_ID, CMD_REQUEST_LOCAL_CLIENT_PLAYBACK_CHG_TIME, (unsigned char *)&m_changeTime, sizeof(unsigned long), msgData);
		ULONGLONG outCH = *reinterpret_cast<ULONGLONG *>(msgData.pData + sizeof(PACKCMD));
		CMessageMan::Instance()->ReleaseMsgBuff(msgData);
		if(outCH == 0)
		{
			MessageBox(m_pStrTable->String(E_STR_ID_MESSAGE_46), m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK | DEFAULT_ICON_WARNING);
			OnExit();
		}
		else
		{
			m_chnnBits = outCH;
			SetSplitMode(outCH);
			m_changeTime = 0;
			m_curPlaybackStatus = PLAYBACK_STATUS_PLAYING;
			m_playerDlg.PlayStatus();
		}
	}


	return KEY_VIRTUAL_MSG_STOP;
#else
	tm endTm = DVRTime32ToTm(m_secStartTime);
	if ((0 == endTm.tm_hour) && (0 == endTm.tm_min) && (0 == endTm.tm_sec))
	{
		return KEY_VIRTUAL_MSG_STOP;
	}

	if (m_bSync)
	{
		if ((PLAYBACK_STATUS_STOP == m_curPlaybackStatus) || (PLAYBACK_STATUS_PAUSE == m_curPlaybackStatus))
		{
			GetSectionTime(m_secStartTime - 1, m_secStartTime, m_secEndTime);
			m_changeTime = m_secStartTime;
			m_playerDlg.SetSectionTime(m_secStartTime, m_secEndTime);
			m_playerDlg.SetTime(m_changeTime, 0, true);
			return KEY_VIRTUAL_MSG_STOP;
		}

		if ((PLAYBACK_STATUS_PLAYING == m_curPlaybackStatus) || (PLAYBACK_STATUS_FF == m_curPlaybackStatus) || (PLAYBACK_STATUS_REW == m_curPlaybackStatus))
		{
			MESSAGE_DATA msgData;
			memset(&msgData, 0, sizeof(MESSAGE_DATA));
			m_clickchnnINfo.speed = PLAY_SPEED_1X;
			CMessageMan::Instance()->SendMsgDataToServer(0, LOCAL_DEVICE_ID, CMD_REQUEST_LOCAL_CLIENT_PLAYBACK_PAUSE, (unsigned char *)&(m_clickchnnINfo), sizeof(CURCHNNINFO), msgData);
			CMessageMan::Instance()->ReleaseMsgBuff(msgData);

			m_changeTime = m_secStartTime - SECTIME_LENGTH;
			PLAYBACK_CHANGETIME_INFO changeTime;
			changeTime.chnn = m_clickchnnINfo.clickchnn;
			changeTime.time = m_changeTime;
			changeTime.bsync =m_bSync;
			changeTime.bpause = false;
			memset(&msgData, 0, sizeof(MESSAGE_DATA));		
			CMessageMan::Instance()->SendMsgDataToServer(0, LOCAL_DEVICE_ID, CMD_REQUEST_LOCAL_CLIENT_PLAYBACK_CHG_TIME, (unsigned char *)&changeTime, sizeof(PLAYBACK_CHANGETIME_INFO), msgData);
			ULONGLONG outCH = *reinterpret_cast<ULONGLONG *>(msgData.pData + sizeof(PACKCMD));
			CMessageMan::Instance()->ReleaseMsgBuff(msgData);
			if(outCH == 0)
			{
				MessageBox(m_pStrTable->String(E_STR_ID_MESSAGE_46), m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK | DEFAULT_ICON_WARNING);
				OnExit();
			}
			else
			{
				m_chnnBits = outCH;
				SetSplitMode(outCH);
				m_changeTime = 0;
				m_curPlaybackStatus = PLAYBACK_STATUS_PLAYING;
				m_playerDlg.PlayStatus();
			}
		}

	}
	else
	{
		if ((PLAYBACK_STATUS_STOP == m_curStatus[m_clickchnnINfo.clickchnn]) || (PLAYBACK_STATUS_PAUSE == m_curStatus[m_clickchnnINfo.clickchnn]))
		{
			GetSectionTime(m_secStartTime - 1, m_secStartTime, m_secEndTime);
			m_changeTime = m_secStartTime;
			m_playerDlg.SetSectionTime(m_secStartTime, m_secEndTime);
			m_playerDlg.SetTime(m_changeTime, 0, true);
			return KEY_VIRTUAL_MSG_STOP;
		}

		if ((PLAYBACK_STATUS_PLAYING == m_curStatus[m_clickchnnINfo.clickchnn]) || (PLAYBACK_STATUS_FF == m_curStatus[m_clickchnnINfo.clickchnn]) || (PLAYBACK_STATUS_REW == m_curStatus[m_clickchnnINfo.clickchnn]))
		{
			MESSAGE_DATA msgData;
			memset(&msgData, 0, sizeof(MESSAGE_DATA));
			m_clickchnnINfo.speed = PLAY_SPEED_1X;
			CMessageMan::Instance()->SendMsgDataToServer(0, LOCAL_DEVICE_ID, CMD_REQUEST_LOCAL_CLIENT_PLAYBACK_PAUSE, (unsigned char *)&m_clickchnnINfo, sizeof(CURCHNNINFO), msgData);
			CMessageMan::Instance()->ReleaseMsgBuff(msgData);

			m_changeTime = m_secStartTime - SECTIME_LENGTH;
			PLAYBACK_CHANGETIME_INFO changeTime;
			changeTime.chnn = m_clickchnnINfo.clickchnn;
			changeTime.time = m_changeTime;
			changeTime.bsync =m_bSync;
			changeTime.bpause = false;
			memset(&msgData, 0, sizeof(MESSAGE_DATA));		
			CMessageMan::Instance()->SendMsgDataToServer(0, LOCAL_DEVICE_ID, CMD_REQUEST_LOCAL_CLIENT_PLAYBACK_CHG_TIME, (unsigned char *)&changeTime, sizeof(PLAYBACK_CHANGETIME_INFO), msgData);
			ULONGLONG outCH = *reinterpret_cast<ULONGLONG *>(msgData.pData + sizeof(PACKCMD));
			CMessageMan::Instance()->ReleaseMsgBuff(msgData);
			if(outCH == 0)
			{
				MessageBox(m_pStrTable->String(E_STR_ID_MESSAGE_46), m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK | DEFAULT_ICON_WARNING);
				OnExit();
			}
			else
			{
				m_chnnBits = outCH;
				SetSplitMode(outCH);
				m_changeTime = 0;
				m_curStatus[m_clickchnnINfo.clickchnn] = PLAYBACK_STATUS_PLAYING;
				m_playerDlg.PlayStatus();
			}
		}
	}
	 return KEY_VIRTUAL_MSG_STOP;
#endif	
}

unsigned long CPlaybackDlg::OnClickNextSection()
{
#if !defined(__ASYN_PLAY__)
	tm endTm = DVRTime32ToTm(m_secEndTime);
	if ((23 == endTm.tm_hour) && (59 == endTm.tm_min) && (59 == endTm.tm_sec))
	{
		return KEY_VIRTUAL_MSG_STOP;
	}

	if ((PLAYBACK_STATUS_STOP == m_curPlaybackStatus) || (PLAYBACK_STATUS_PAUSE == m_curPlaybackStatus))
	{
		GetSectionTime(m_secEndTime + 1, m_secStartTime, m_secEndTime);
		m_changeTime = m_secStartTime;
		m_playerDlg.SetSectionTime(m_secStartTime, m_secEndTime);
		m_playerDlg.SetTime(m_changeTime, 0, true);
		return KEY_VIRTUAL_MSG_STOP;
	}

	if ((PLAYBACK_STATUS_PLAYING == m_curPlaybackStatus) || (PLAYBACK_STATUS_FF == m_curPlaybackStatus) || (PLAYBACK_STATUS_REW == m_curPlaybackStatus))
	{
		MESSAGE_DATA msgData;
		memset(&msgData, 0, sizeof(MESSAGE_DATA));
		CMessageMan::Instance()->SendMsgDataToServer(0, LOCAL_DEVICE_ID, CMD_REQUEST_LOCAL_CLIENT_PLAYBACK_PAUSE, NULL, 0, msgData);
		CMessageMan::Instance()->ReleaseMsgBuff(msgData);

		m_changeTime = m_secEndTime + 1;
		memset(&msgData, 0, sizeof(MESSAGE_DATA));		
		CMessageMan::Instance()->SendMsgDataToServer(0, LOCAL_DEVICE_ID, CMD_REQUEST_LOCAL_CLIENT_PLAYBACK_CHG_TIME, (unsigned char *)&m_changeTime, sizeof(unsigned long), msgData);
		ULONGLONG outCH = *reinterpret_cast<ULONGLONG *>(msgData.pData + sizeof(PACKCMD));
		CMessageMan::Instance()->ReleaseMsgBuff(msgData);
		if(outCH == 0)
		{
			MessageBox(m_pStrTable->String(E_STR_ID_MESSAGE_46), m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK | DEFAULT_ICON_WARNING);
			OnExit();
		}
		else
		{
			m_chnnBits = outCH;
			SetSplitMode(outCH);
			m_changeTime = 0;
			m_curPlaybackStatus = PLAYBACK_STATUS_PLAYING;	
			m_playerDlg.PlayStatus();
		}
	}
	return KEY_VIRTUAL_MSG_STOP;
#else
	tm endTm = DVRTime32ToTm(m_secEndTime);
	if ((23 == endTm.tm_hour) && (59 == endTm.tm_min) && (59 == endTm.tm_sec))
	{
		return KEY_VIRTUAL_MSG_STOP;
	}
	
	if (m_bSync)
	{
		if ((PLAYBACK_STATUS_STOP == m_curPlaybackStatus) || (PLAYBACK_STATUS_PAUSE == m_curPlaybackStatus))
		{
			GetSectionTime(m_secEndTime + 1, m_secStartTime, m_secEndTime);
			m_changeTime = m_secStartTime;
			m_playerDlg.SetSectionTime(m_secStartTime, m_secEndTime);
			m_playerDlg.SetTime(m_changeTime, 0, true);
			return KEY_VIRTUAL_MSG_STOP;
		}

		if ((PLAYBACK_STATUS_PLAYING == m_curPlaybackStatus) || (PLAYBACK_STATUS_FF == m_curPlaybackStatus) || (PLAYBACK_STATUS_REW == m_curPlaybackStatus))
		{
			MESSAGE_DATA msgData;
			memset(&msgData, 0, sizeof(MESSAGE_DATA));
			m_clickchnnINfo.speed = PLAY_SPEED_1X;
			CMessageMan::Instance()->SendMsgDataToServer(0, LOCAL_DEVICE_ID, CMD_REQUEST_LOCAL_CLIENT_PLAYBACK_PAUSE, (unsigned char *)&m_clickchnnINfo, sizeof(CURCHNNINFO), msgData);
			CMessageMan::Instance()->ReleaseMsgBuff(msgData);

			m_changeTime = m_secEndTime + 1;
			PLAYBACK_CHANGETIME_INFO changeTime;
			changeTime.chnn = m_clickchnnINfo.clickchnn;
			changeTime.bsync =m_bSync;
			changeTime.time = m_changeTime;
			changeTime.bpause = false;
			memset(&msgData, 0, sizeof(MESSAGE_DATA));		
			CMessageMan::Instance()->SendMsgDataToServer(0, LOCAL_DEVICE_ID, CMD_REQUEST_LOCAL_CLIENT_PLAYBACK_CHG_TIME, (unsigned char *)&changeTime, sizeof(PLAYBACK_CHANGETIME_INFO), msgData);
			ULONGLONG outCH = *reinterpret_cast<ULONGLONG *>(msgData.pData + sizeof(PACKCMD));
			CMessageMan::Instance()->ReleaseMsgBuff(msgData);
			if(outCH == 0)
			{
				MessageBox(m_pStrTable->String(E_STR_ID_MESSAGE_46), m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK | DEFAULT_ICON_WARNING);
				OnExit();
			}
			else
			{
				m_chnnBits = outCH;
				SetSplitMode(outCH);
				m_changeTime = 0;
				m_curPlaybackStatus = PLAYBACK_STATUS_PLAYING;	
				m_playerDlg.PlayStatus();
			}
		}
	}
	else
	{
		if ((PLAYBACK_STATUS_STOP == m_curStatus[m_clickchnnINfo.clickchnn]) || (PLAYBACK_STATUS_PAUSE == m_curStatus[m_clickchnnINfo.clickchnn]))
		{
			GetSectionTime(m_secEndTime + 1, m_secStartTime, m_secEndTime);
			m_changeTime = m_secStartTime;
			m_playerDlg.SetSectionTime(m_secStartTime, m_secEndTime);
			m_playerDlg.SetTime(m_changeTime, 0, true);
			return KEY_VIRTUAL_MSG_STOP;
		}

		if ((PLAYBACK_STATUS_PLAYING == m_curStatus[m_clickchnnINfo.clickchnn]) || (PLAYBACK_STATUS_FF == m_curStatus[m_clickchnnINfo.clickchnn]) || (PLAYBACK_STATUS_REW == m_curStatus[m_clickchnnINfo.clickchnn]))
		{
			MESSAGE_DATA msgData;
			memset(&msgData, 0, sizeof(MESSAGE_DATA));
			m_clickchnnINfo.speed = PLAY_SPEED_1X;
			CMessageMan::Instance()->SendMsgDataToServer(0, LOCAL_DEVICE_ID, CMD_REQUEST_LOCAL_CLIENT_PLAYBACK_PAUSE, (unsigned char *)&m_clickchnnINfo, sizeof(CURCHNNINFO), msgData);
			CMessageMan::Instance()->ReleaseMsgBuff(msgData);

			m_changeTime = m_secEndTime + 1;
			PLAYBACK_CHANGETIME_INFO changeTime;
			changeTime.chnn = m_clickchnnINfo.clickchnn;
			changeTime.time = m_changeTime;
			changeTime.bsync =m_bSync;
			changeTime.bpause = false;
			memset(&msgData, 0, sizeof(MESSAGE_DATA));
			CMessageMan::Instance()->SendMsgDataToServer(0, LOCAL_DEVICE_ID, CMD_REQUEST_LOCAL_CLIENT_PLAYBACK_CHG_TIME, (unsigned char *)&changeTime, sizeof(PLAYBACK_CHANGETIME_INFO), msgData);
			ULONGLONG outCH = *reinterpret_cast<ULONGLONG *>(msgData.pData + sizeof(PACKCMD));
			CMessageMan::Instance()->ReleaseMsgBuff(msgData);
			if(outCH == 0)
			{
				MessageBox(m_pStrTable->String(E_STR_ID_MESSAGE_46), m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK | DEFAULT_ICON_WARNING);
				OnExit();
			}
			else
			{
				m_chnnBits = outCH;
				SetSplitMode(outCH);
				m_changeTime = 0;
				m_curStatus[m_clickchnnINfo.clickchnn] = PLAYBACK_STATUS_PLAYING;	
				m_playerDlg.PlayStatus();
			}
		}
	}
	return KEY_VIRTUAL_MSG_STOP;
#endif

}

unsigned long CPlaybackDlg::OnMouseUp(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	m_bDragSlider = false;
	m_playerDlg.ShowTime(false);

	if(m_playerDlg.IsShow())
	{
		m_playerDlg.OnMouseLeftUp();
	}

#if 0
	if(m_bDragChnn)
	{
		if(m_groupCtrl.IsShow())
		{
			m_groupCtrl.Show(false, false);
		}

		if(m_bDraged)
		{
			ChangeDisplayByTwoPt(m_enterDragPosX, m_enterDragPosY, xPos, yPos, -1);
		}

		GetRootWnd()->Repaint();
	}
#endif

	m_bDragChnn = false;
	m_bFirstDragChnn = false;
	m_bDraged = false;

	#ifdef __CUSTOM_US02_NEW__
		if(m_pTipWindow != NULL)
		{
			m_pTipWindow->Show(false, true);
			GetRootWnd()->Repaint();
		}
	#endif

	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CPlaybackDlg::OnDrag(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
#if !defined(__ASYN_PLAY__)
	//�϶�������,��������Ϣ
	if (MESSAGE_VIRTUAL_DRAG_START == keyID)
	{
		//assert(!m_bDragSlider);
		if (!m_bDragSlider)
		{
			m_bDragSlider = true;
		}
	}
	else if (MESSAGE_VIRTUAL_DRAG_ING == keyID)
	{
		int distance = m_playerDlg.GetCurPos();
		assert((distance >= 0) && (distance < SECTIME_LENGTH));
		unsigned long time = m_secStartTime + distance;
		m_playerDlg.SetTime(time, distance, true);  
	}
	else if (MESSAGE_VIRTUAL_DRAG_END == keyID)
	{
		//assert(m_bDragSlider);
		if (m_bDragSlider)
		{
			m_bDragSlider = false;

			int distance = m_playerDlg.GetCurPos();
			assert((distance >= 0) && (distance < SECTIME_LENGTH));
			m_changeTime = m_secStartTime + distance;

			if ((PLAYBACK_STATUS_STOP == m_curPlaybackStatus) || (PLAYBACK_STATUS_PAUSE == m_curPlaybackStatus))
			{
				m_playerDlg.SetTime(m_changeTime, distance, true);
			}
			else
			{
				MESSAGE_DATA msgData;

				if ((PLAYBACK_STATUS_FF == m_curPlaybackStatus) || (PLAYBACK_STATUS_REW == m_curPlaybackStatus))
				{						
					memset(&msgData, 0, sizeof(MESSAGE_DATA));
					CMessageMan::Instance()->SendMsgDataToServer(0, LOCAL_DEVICE_ID, CMD_REQUEST_LOCAL_CLIENT_PLAYBACK_PAUSE, NULL, 0, msgData);
					CMessageMan::Instance()->ReleaseMsgBuff(msgData);				
				}

				memset(&msgData, 0, sizeof(MESSAGE_DATA));
				CMessageMan::Instance()->SendMsgDataToServer(0, LOCAL_DEVICE_ID, CMD_REQUEST_LOCAL_CLIENT_PLAYBACK_CHG_TIME, (unsigned char *)&m_changeTime, sizeof(unsigned long), msgData);
				ULONGLONG outCH = *reinterpret_cast<ULONGLONG *>(msgData.pData + sizeof(PACKCMD));
				CMessageMan::Instance()->ReleaseMsgBuff(msgData);
				if(outCH == 0)
				{
					MessageBox(m_pStrTable->String(E_STR_ID_MESSAGE_46), m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK | DEFAULT_ICON_WARNING);
					OnClickStop();
					m_changeTime = 0;
				}
				else
				{
					m_lastFrameTime = m_changeTime;
					m_chnnBits = outCH;
					SetSplitMode(outCH);
					m_changeTime = 0;
					m_curPlaybackStatus = PLAYBACK_STATUS_PLAYING;
					m_playerDlg.PlayStatus();
				}
			}	
		}
	}
	else if((MESSAGE_VIRTUAL_SPLIT_5 == keyID) | (MESSAGE_VIRTUAL_SPLIT_7 == keyID) | (MESSAGE_VIRTUAL_SPLIT_12 == keyID))//5�ָ�
	{
		return OnClickSplit(keyID);
	}
	else
	{
		
		assert(false);
	}


	return KEY_VIRTUAL_MSG_STOP;
#else
	//�϶�������,��������Ϣ
	if (MESSAGE_VIRTUAL_DRAG_START == keyID)
	{
		//assert(!m_bDragSlider);
		if (!m_bDragSlider)
		{
			m_bDragSlider = true;
		}
	}
	else if (MESSAGE_VIRTUAL_DRAG_ING == keyID)
	{
		int distance = m_playerDlg.GetCurPos();
		assert((distance >= 0) && (distance < SECTIME_LENGTH));
		unsigned long time = m_secStartTime + distance;
		m_playerDlg.SetTime(time, distance, true);  
	}
	else if (MESSAGE_VIRTUAL_DRAG_END == keyID)
	{
		//assert(m_bDragSlider);
		if (m_bDragSlider)
		{
			m_bDragSlider = false;

			int distance = m_playerDlg.GetCurPos();
			assert((distance >= 0) && (distance < SECTIME_LENGTH));
			m_changeTime = m_secStartTime + distance;

			if ((m_bSync && ((PLAYBACK_STATUS_STOP == m_curPlaybackStatus) || (PLAYBACK_STATUS_PAUSE == m_curPlaybackStatus))) \
				||(!m_bSync &&((PLAYBACK_STATUS_STOP == m_curStatus[m_clickchnnINfo.clickchnn]) || (PLAYBACK_STATUS_PAUSE == m_curStatus[m_clickchnnINfo.clickchnn]))))
			{
				m_playerDlg.SetTime(m_changeTime, distance, true);
			}
			else
			{
				MESSAGE_DATA msgData;
				PLAYBACK_CHANGETIME_INFO changeTime;
				changeTime.chnn = m_clickchnnINfo.clickchnn;
				changeTime.time = m_changeTime;
				changeTime.bpause = false;
				changeTime.bsync = m_bSync;
				if (m_bSync)
				{
					if ((m_bSync && ((PLAYBACK_STATUS_FF == m_curPlaybackStatus) || (PLAYBACK_STATUS_REW == m_curPlaybackStatus))))
					{						
						memset(&msgData, 0, sizeof(MESSAGE_DATA));
						m_clickchnnINfo.speed =PLAY_SPEED_1X;
						CMessageMan::Instance()->SendMsgDataToServer(0, LOCAL_DEVICE_ID, CMD_REQUEST_LOCAL_CLIENT_PLAYBACK_PAUSE, (unsigned char *)&m_clickchnnINfo, sizeof(CURCHNNINFO), msgData);
						CMessageMan::Instance()->ReleaseMsgBuff(msgData);				
					}
					

				}
				else
				{
					if(!m_bSync &&((m_curStatus[m_clickchnnINfo.clickchnn]==PLAYBACK_STATUS_FF)||(PLAYBACK_STATUS_REW == m_curStatus[m_clickchnnINfo.clickchnn])))
					{
						memset(&msgData, 0, sizeof(MESSAGE_DATA));
						m_clickchnnINfo.speed =PLAY_SPEED_1X;
						CMessageMan::Instance()->SendMsgDataToServer(0, LOCAL_DEVICE_ID, CMD_REQUEST_LOCAL_CLIENT_PLAYBACK_PAUSE, (unsigned char *)&m_clickchnnINfo, sizeof(CURCHNNINFO), msgData);
						CMessageMan::Instance()->ReleaseMsgBuff(msgData);	
					}
				}
				memset(&msgData, 0, sizeof(MESSAGE_DATA));
				CMessageMan::Instance()->SendMsgDataToServer(0, LOCAL_DEVICE_ID, CMD_REQUEST_LOCAL_CLIENT_PLAYBACK_CHG_TIME, (unsigned char *)&changeTime, sizeof(PLAYBACK_CHANGETIME_INFO), msgData);
				ULONGLONG outCH = *reinterpret_cast<ULONGLONG *>(msgData.pData + sizeof(PACKCMD));
				CMessageMan::Instance()->ReleaseMsgBuff(msgData);
				if(outCH == 0)
				{
					MessageBox(m_pStrTable->String(E_STR_ID_MESSAGE_46), m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK | DEFAULT_ICON_WARNING);
					OnClickStop();
					m_changeTime = 0;
				}
				else
				{
					m_lastFrameTime = m_changeTime;
					m_chnnBits = outCH;
					SetSplitMode(outCH);
					m_changeTime = 0;
					if (m_bSync)
					{
						m_curPlaybackStatus = PLAYBACK_STATUS_PLAYING;
					}
					else
					{
						m_curStatus[m_clickchnnINfo.clickchnn] = PLAYBACK_STATUS_PLAYING;
					}
					m_playerDlg.PlayStatus();
				}
			}	
		}
	}
	else if((MESSAGE_VIRTUAL_SPLIT_5 == keyID) | (MESSAGE_VIRTUAL_SPLIT_7 == keyID) | (MESSAGE_VIRTUAL_SPLIT_12 == keyID))//5�ָ�
	{
		return OnClickSplit(keyID);
	}
	else
	{		
		assert(false);
	}

	 return KEY_VIRTUAL_MSG_STOP;
#endif

}

unsigned long CPlaybackDlg::OnEnter(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	bool bPlayTool = false;

	if(m_playerDlg.IsShow())
	{
		m_playerDlg.OnEnter(xPos, yPos);

		bPlayTool = m_playerDlg.PtInPlayToolBar(xPos, yPos);
	}

#if 0
	m_bDragChnn = !bPlayTool;
	m_bFirstDragChnn = true;

	if(m_bDragChnn)
	{
		NET_DISPLAY_INFO dispInfo;
		memset(&dispInfo, 0, sizeof(NET_DISPLAY_INFO));
		if(GetCurDisplay(dispInfo, xPos, yPos, true) && (dispInfo.data_len > 0))
		{
			m_bDragChnn = true;

			m_enterDragPosX = xPos;
			m_enterDragPosY = yPos;
		}
		else
		{
			m_bDragChnn = false;
		}
	}
#endif

	m_bDraged = false;

#if defined(__NEW_CUT_BACKUP__)
	if(bPlayTool)//����ڿ������ϣ������Ӧ
	{
		return KEY_VIRTUAL_MSG_STOP;
	}


	MESSAGE_DATA msgData;
	memset(&msgData, 0, sizeof(MESSAGE_DATA));

	unsigned long pos = (xPos & 0xffff) + ((yPos & 0xffff) << 16);
	CMessageMan::Instance()->SendMsgDataToServer(0, LOCAL_DEVICE_ID, CMD_REQUEST_LOCAL_CLIENT_PLAYBACK_CHOSECHN, (unsigned char *)&pos, sizeof(unsigned long), msgData);

	PACKCMD *pCmd = reinterpret_cast<PACKCMD *>(msgData.pData);
	if (CMD_REPLY_LOCAL_CLIENT_PLAYBACK_CHOSE_CHNN_SUCC == (NET_PROTOCOL)pCmd->cmdType)
	{
		m_clickchnnINfo.clickchnn = *reinterpret_cast<unsigned long *>(msgData.pData + sizeof(PACKCMD));

		//��ѡ���ͨ���ش���m_playerDlg
		m_playerDlg.SetCurSelectChnn(m_clickchnnINfo.clickchnn);
		
	}
	CMessageMan::Instance()->ReleaseMsgBuff(msgData);
#endif
	
	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CPlaybackDlg::OnMouseMove(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	m_playerDlg.OnMouseMove(xPos, yPos);
	#ifdef __CUSTOM_US02_NEW__
		if(m_pTipWindow != NULL)
			{
				m_pTipWindow->Show(false, true);
				GetRootWnd()->Repaint();
			}
	#endif
	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CPlaybackDlg::OnMouseDrag(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	if(m_playerDlg.IsShow())
	{
		GUI::CRect rect = m_playerDlg.OnMouseDrag(xPos, yPos, false);

		#ifdef __CUSTOM_US02_NEW__
			if(m_pTipWindow != NULL)
			{
				m_pTipWindow->Show(false, true);
				GetRootWnd()->Repaint();
			}
		#endif

		if((rect.Width() > 0) && (rect.Height() > 0))
		{
			GUI::CWnd::DrawClear(rect.m_left, rect.m_top, rect.Width(), rect.Height());
			m_playerDlg.Show(true, false);

			GUI::CWnd::Refresh(rect.m_left, rect.m_top, rect.Width(), rect.Height());

			CMessageMan::Instance()->RefreshLiveTrue();
		}
	}
#if 0
	if(m_bDragChnn)
	{
		int width = m_groupCtrl.GetClientRect().Width();
		int height = m_groupCtrl.GetClientRect().Height();;

		GUI::CRect rectGroup(0, 0, width, height);

		if((xPos > width / 2) && ((xPos + width / 2) < m_width))
		{
			rectGroup.MoveToX(xPos - width / 2);
		}
		else if((xPos + width / 2) >= m_width)
		{
			rectGroup.MoveToX(m_width - width);
		}

		if((yPos > height / 2) && ((yPos + height / 2) < m_height))
		{
			rectGroup.MoveToY(yPos - height / 2);
		}
		else if((yPos + height / 2) >= m_height)
		{
			rectGroup.MoveToY(m_height - height);
		}

		if(m_bFirstDragChnn)
		{
			unsigned long offset_len = ((m_width / 30) * (m_width / 30)) + ((m_height / 30) * (m_height / 30));
			//unsigned long offset_len = 200;
			if(!IsAlmostEqual(m_lastDragPosX, m_lastDragPosY, xPos, yPos, offset_len))
			{
				//��ֹ˫����������ʧ��
				m_groupCtrl.MoveWnd(rectGroup.m_left, rectGroup.m_top);
				m_groupCtrl.Show(true, true);

				m_lastDragPosX = xPos;
				m_lastDragPosY = yPos;

				m_bFirstDragChnn = false;
				m_bDraged = true;

				CMessageMan::Instance()->RefreshLiveTrue();
			}
		}
		else
		{
			unsigned long offset_len = ((m_width / 50) * (m_width / 50)) + ((m_height / 50) * (m_height / 50));
			//unsigned long offset_len = 200;
			if(!IsAlmostEqual(m_lastDragPosX, m_lastDragPosY, xPos, yPos, offset_len))
			{
				GUI::CRect rectOld = m_groupCtrl.GetWindowRect();
				//GUI::CWnd::DrawClear(rectOld.m_left, rectOld.m_top, rectOld.Width(), rectOld.Height()); //ȥ����Ӱ

				m_groupCtrl.MoveWnd(rectGroup.m_left, rectGroup.m_top);
				m_groupCtrl.Show(true, false);

				GUI::CRect rectRefresh = rectOld.UnionRect(rectGroup);
				CWnd::Refresh(rectRefresh.m_left, rectRefresh.m_top, rectRefresh.Width(), rectRefresh.Height());

				m_bDraged = true;
				CMessageMan::Instance()->RefreshLiveTrue();

				m_lastDragPosX = xPos;
				m_lastDragPosY = yPos;
			}
		}
	}
#endif

	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CPlaybackDlg::OnClickNumKey(unsigned long keyID)
{
	if (PLAYBACK_STATUS_PLAYING == m_curPlaybackStatus)
	{
		unsigned char chnn = ( keyID & 0x3f );
		if (0 == chnn)
		{
			chnn = (10 - 1);
		}
		else
		{
			chnn -= 1;
		}

		if (chnn < m_maxChannelNum)
		{
			LOCAL_CLINET_PB_CHG_CH_INFO info;
			info.splitMode = VIEW_SPLIT_1X1;
			info.chnnBits = (ULONGLONG)(0x01) << chnn;
			
			MESSAGE_DATA msgData;
			memset(&msgData, 0, sizeof(MESSAGE_DATA));
			CMessageMan::Instance()->SendMsgDataToServer(0, LOCAL_DEVICE_ID, CMD_REQUEST_LOCAL_CLIENT_PLAYBACK_CHG_CHNN, (unsigned char *)&info, sizeof(LOCAL_CLINET_PB_CHG_CH_INFO), msgData);

			ULONGLONG outCH = *reinterpret_cast<ULONGLONG *>(msgData.pData + sizeof(PACKCMD));
			CMessageMan::Instance()->ReleaseMsgBuff(msgData);

			if(outCH == 0)
			{
				MessageBox(m_pStrTable->String(E_STR_ID_MESSAGE_46), m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK | DEFAULT_ICON_WARNING);
				OnExit();
			}
			else
			{
				m_chnnBits = outCH;
				//��Ҫ������Ŀ�����������Ϣ
				m_playerDlg.SetCurPlayChnn(m_chnnBits);
				m_playerDlg.SetAudioCH();
				SetSplitMode(outCH);
			}

			return KEY_VIRTUAL_MSG_STOP;
		}
	}

	return KEY_VIRTUAL_MSG_CONTINUE;
}

unsigned long CPlaybackDlg::OnDoubleClick(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	unsigned long totalauthCHnum = 0;

	if(m_playerDlg.IsShow() && m_playerDlg.PtInPlayToolBar(xPos, yPos))
	{
		return KEY_VIRTUAL_MSG_STOP;
	}
	
	ULONGLONG authCH = CUserMan::Instance()->CheckAuthority(AUTH_PLAYBACK);

	if(0 == authCH)
	{
		//ûȨ�ޣ�ʲô�¶�����
		return KEY_VIRTUAL_MSG_STOP;
	}
	
	ULONGLONG totalauthCH = 0;
	for(int i=0;i<g_p_login_succ->productInfo.videoOuputNum;i++)
	{
		if((authCH >> i) & (0x01))
		{
			totalauthCHnum++;
			totalauthCH  |= ((ULONGLONG)(0x01) << i);
		}
	}
	
	ULONGLONG totalauthNetCH = 0;
	unsigned long totalauthNetCHnum = 0;
	
	for(int i=0;i<m_ipcMaxVideoOutputNum;i++)
	{
		if((authCH >> (i + g_p_login_succ->productInfo.localVideoInputNum)) & (0x01))
		{
			totalauthNetCHnum++;
			totalauthNetCH  |= ((ULONGLONG)(0x01) << i);
		}
	}
	
	if (PLAYBACK_STATUS_PLAYING == m_curPlaybackStatus)
	{
		//������ʾPlayer �����λ����Player��,�򲻴���
		if( m_playerDlg.IsShow() && m_rectPlayer.PtInRect(xPos, yPos) )
		{
			return KEY_VIRTUAL_MSG_STOP; 
		}

		ULONGLONG outCH = 0;
		MESSAGE_DATA msgData;
		memset(&msgData, 0, sizeof(MESSAGE_DATA));

		if (PLAY_CH_ONE == m_curSplitMode)
		{
			unsigned char chnnNum = 0;
			LOCAL_CLINET_PB_CHG_CH_INFO info;
			memset(&info, 0, sizeof(LOCAL_CLINET_PB_CHG_CH_INFO));

			//�Ƚ����жϵ�������ֳ����滹�ǻطŻ���
			//unsigned long pos = (xPos & 0xffff) + ((yPos & 0xffff) << 16);
			//CMessageMan::Instance()->SendMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_LOCAL_CLIENT_PLAYBACK_HIT, (unsigned char *)&pos, sizeof(unsigned long), msgData);
			//PACKCMD *pCmd = reinterpret_cast<PACKCMD *>(msgData.pData);
			//if (CMD_REPLY_LOCAL_CLIENT_PLAYBACK_HIT_FAIL == (NET_PROTOCOL)pCmd->cmdType)
			//{
			//	//��������ֳ���			
			//	CMessageMan::Instance()->ReleaseMsgBuff(msgData);
			//	
			//	return KEY_VIRTUAL_MSG_STOP;
			//}
			//else
			//{
			//	//������ǻطŵģ�����������
			//	CMessageMan::Instance()->ReleaseMsgBuff(msgData);
			//}
			m_pDisplayMan = CDisplayMan::Instance();
			unsigned long chnn = INVALID_INDEX(unsigned long);

			unsigned long hitsChnn[MAX_CHNN_NUM] = {0};
			int hits = m_pDisplayMan->HitChnn(xPos, yPos, hitsChnn, MAX_CHNN_NUM);

			for (int i = 0; i < hits; i++)
			{
				if( (DISPLAY_SIGNAL_DIGIT == DISPLAY_GET_SIGNAL(hitsChnn[i])) &&
					(DISPLAY_INVALID != DISPLAY_GET_CHNN(hitsChnn[i])))
				{
					chnn = DISPLAY_GET_CHNN(hitsChnn[i]);
					break;
				}
			}
			if (INVALID_INDEX(unsigned long) == chnn)//��������ֳ����ߺ���
			{
				return KEY_VIRTUAL_MSG_STOP;
			}
			

			if (m_preSplitModeInfoList.IsEmpty())
			{		
				if(!m_curSplitModeInfoList.IsEmpty())
				{
					POS pos = m_curSplitModeInfoList.GetHeadPosition();
					PLAYBACK_CHANNEL_INFO &headInfo = m_curSplitModeInfoList.GetNext(pos);
					unsigned maxOutPutNum = g_p_login_succ->productInfo.videoOuputNum;
					info.chnnBits = totalauthCH<<((unsigned long)(headInfo.channel/maxOutPutNum)*maxOutPutNum);
					chnnNum = totalauthCHnum;
					
					if (headInfo.channel >= g_p_login_succ->productInfo.localVideoInputNum)
					{
						if (1 == m_ipcMaxVideoOutputNum)
						{
							return KEY_VIRTUAL_MSG_CONTINUE;
						}
						else
						{
							maxOutPutNum = m_ipcMaxVideoOutputNum;
							info.chnnBits = totalauthNetCH<<((unsigned long)(headInfo.channel/maxOutPutNum)*maxOutPutNum);
							chnnNum = totalauthNetCHnum;
						}
					}
				}
				else
				{
					info.chnnBits = totalauthCH;
					chnnNum = totalauthCHnum;
				}
			}
			else
			{
				chnnNum =0;
				POS pos = m_preSplitModeInfoList.GetHeadPosition();
				while (pos != NULL)
				{
					PLAYBACK_CHANNEL_INFO &channelInfo = m_preSplitModeInfoList.GetNext(pos);
					info.chnnBits |= ((ULONGLONG)1 << (channelInfo.channel));
					++ chnnNum;					
				}
				
				assert(0 != info.chnnBits);
				//���˫���е������е��໭��ʱ�����û���ҵ��໭�棬�Ͳ���
				//һ��Ĭ��ֵ
				if ((1 == chnnNum) || (0 == info.chnnBits))
				{					
					info.chnnBits = totalauthCH;
					chnnNum = totalauthCHnum;
				}
			}
			
			int type=m_playerDlg.GetSPECtype();
			
			info.splitMode = CPlayerDlg::SplitPBToLive(GetSplitMode(chnnNum),type);
			
			CMessageMan::Instance()->SendMsgDataToServer(0, LOCAL_DEVICE_ID, CMD_REQUEST_LOCAL_CLIENT_PLAYBACK_CHG_CHNN, (unsigned char *)&info, sizeof(LOCAL_CLINET_PB_CHG_CH_INFO), msgData);
		}
		else
		{
			
			unsigned long pos = (xPos & 0xffff) + ((yPos & 0xffff) << 16);
			CMessageMan::Instance()->SendMsgDataToServer(0, LOCAL_DEVICE_ID, CMD_REQUEST_LOCAL_CLIENT_PLAYBACK_CLICK, (unsigned char *)&pos, sizeof(unsigned long), msgData);
		}

		PACKCMD *pCmd = reinterpret_cast<PACKCMD *>(msgData.pData);
		outCH = *reinterpret_cast<ULONGLONG *>(msgData.pData + sizeof(PACKCMD));

		if (CMD_REPLY_LOCAL_CLIENT_PLAYBACK_CHG_CHNN_FAIL != (NET_PROTOCOL)pCmd->cmdType)
		{	
			if(outCH == 0)
			{
				MessageBox(m_pStrTable->String(E_STR_ID_MESSAGE_46), m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK | DEFAULT_ICON_WARNING);
				OnExit();
			}
			else
			{
				m_chnnBits = outCH;
				//��Ҫ������Ŀ�����������Ϣ
				m_playerDlg.SetCurPlayChnn(m_chnnBits);
				m_playerDlg.SetAudioCH();
				SetSplitMode(outCH);
			}
		}

		CMessageMan::Instance()->ReleaseMsgBuff(msgData);
		return KEY_VIRTUAL_MSG_STOP;
	}

	return KEY_VIRTUAL_MSG_CONTINUE;
}


/*
��������ʱִ�п�����ܣ����ݲ�ͬ�������Ƕȣ��ֱ�Ϊ�����X2,X4,X8,X16���٣�����ԭʱΪX1���ٵ��������� ZHL 2010-06-23
*/
unsigned long  CPlaybackDlg::OnFlyRight(unsigned long keyID)
{
	if ((PLAYBACK_STATUS_PLAYING == m_curPlaybackStatus) || (PLAYBACK_STATUS_FF == m_curPlaybackStatus))
	{
		FlyChangePlaySpeed(keyID, true);
	}
	return KEY_VIRTUAL_MSG_STOP;
}

/*
��������ʱִ�п��˹��ܣ����ݲ�ͬ�������Ƕȣ��ֱ�Ϊ���˵�X8,X16,X32���٣�����ԭʱΪX1���ٵ��������� ZHL 2010-06-23
*/
unsigned long  CPlaybackDlg::OnFlyLeft(unsigned long keyID)
{
	if ((PLAYBACK_STATUS_PLAYING == m_curPlaybackStatus) || (PLAYBACK_STATUS_REW == m_curPlaybackStatus)) 
	{
		FlyChangePlaySpeed(keyID, false);
	}
	
	return KEY_VIRTUAL_MSG_STOP;

}

/*
����ԭ�����ò����ٶ�ΪX1���ٵ��������� ZHL 2010-06-23
*/
unsigned long CPlaybackDlg::OnFlyMedium()
{
	if ((PLAYBACK_STATUS_PLAYING == m_curPlaybackStatus) || (PLAYBACK_STATUS_FF == m_curPlaybackStatus) || (PLAYBACK_STATUS_REW == m_curPlaybackStatus))
	{
		unsigned long keyID = KEY_VIRTUAL_FLY_MEDIUM;
		FlyChangePlaySpeed(keyID, true);
	}
	

	return KEY_VIRTUAL_MSG_STOP;
}

/*
��ͣʱ�����ڲ�������ִ�е�֡���� ZHL 2010-06-23
*/
unsigned long CPlaybackDlg::OnFlyInRight()
{
#if !defined(__ASYN_PLAY__)
	if (PLAYBACK_STATUS_PAUSE == m_curPlaybackStatus)
	{
		if (m_playerDlg.NextFrame())
		{
			MESSAGE_DATA msgData;
			memset(&msgData, 0, sizeof(MESSAGE_DATA));
			CMessageMan::Instance()->SendMsgDataToServer(0, LOCAL_DEVICE_ID, CMD_REQUEST_LOCAL_CLIENT_PLAYBACK_NEXTFRAME, NULL, 0, msgData);

			CMessageMan::Instance()->ReleaseMsgBuff(msgData);
		}
	}

	return KEY_VIRTUAL_MSG_STOP;
#else
	if (PLAYBACK_STATUS_PAUSE == m_curPlaybackStatus)
	{
		if (m_playerDlg.NextFrame())
		{
			MESSAGE_DATA msgData;
			memset(&msgData, 0, sizeof(MESSAGE_DATA));
			CMessageMan::Instance()->SendMsgDataToServer(0, LOCAL_DEVICE_ID, CMD_REQUEST_LOCAL_CLIENT_PLAYBACK_NEXTFRAME, (unsigned char *)&m_clickchnnINfo.clickchnn, sizeof(unsigned long), msgData);

			CMessageMan::Instance()->ReleaseMsgBuff(msgData);
		}
	}
	return KEY_VIRTUAL_MSG_STOP;
#endif

}
void  CPlaybackDlg::GetSectionTime(unsigned long time, unsigned long &secStartime, unsigned long &secEndTime)
{
	unsigned long length = SECTIME_LENGTH;
	unsigned long left = time % length;
	
	if (left > 0)
	{
		secStartime = time - left;
	}
	else
	{
		secStartime = time;
	}
	secEndTime = secStartime + length - 1;
}

void CPlaybackDlg::FlyChangePlaySpeed(unsigned long keyID, bool bForward )
{
#if !defined(__ASYN_PLAY__)
	PLAY_SPEED playSpeed = PLAY_SPEED_NONE;

	//��������
	//if( KEY_VIRTUAL_FLY_RIGHT_1 == keyID)
	//{
	//	
	//}
	//else
	if(KEY_VIRTUAL_FLY_RIGHT_2 == keyID)
	{
		playSpeed = PLAY_SPEED_2X;
		
	}
	//else if(KEY_VIRTUAL_FLY_RIGHT_3 == keyID)
	//{	
		
	//}
	else if(KEY_VIRTUAL_FLY_RIGHT_4 == keyID)
	{
		playSpeed = PLAY_SPEED_4X;
		
		
	}
	//else if(KEY_VIRTUAL_FLY_RIGHT_5 == keyID)
	//{
		
	//}
	else if(KEY_VIRTUAL_FLY_RIGHT_6 == keyID)
	{
		playSpeed = PLAY_SPEED_8X;
	}
	else if(KEY_VIRTUAL_FLY_RIGHT_7 == keyID)
	{
		playSpeed = PLAY_SPEED_16X;
	}
	//else if(KEY_VIRTUAL_FLY_RIGHT_8 == keyID)
	//{
		
	//}
	else if(KEY_VIRTUAL_FLY_MEDIUM == keyID)
	{
		//����ԭ
		playSpeed = PLAY_SPEED_1X;
	}
	//��������
	//else if(KEY_VIRTUAL_FLY_LEFT_1 == keyID)
	//{
	//	
	//}
	else if(KEY_VIRTUAL_FLY_LEFT_2 == keyID)
	{
		playSpeed = PLAY_SPEED_8X;
		
	}
	//else if(KEY_VIRTUAL_FLY_LEFT_3 == keyID)
	//{
			
	//}
	else if(KEY_VIRTUAL_FLY_LEFT_4 == keyID)
	{
		playSpeed = PLAY_SPEED_16X;
		
	}
	//else if(KEY_VIRTUAL_FLY_LEFT_5 == keyID)
	//{

	//}	
	else if(KEY_VIRTUAL_FLY_LEFT_6 == keyID)
	{
		playSpeed = PLAY_SPEED_32X;
	}	
	//else if(KEY_VIRTUAL_FLY_LEFT_7 == keyID)
	//{

	//}
	//else if(KEY_VIRTUAL_FLY_LEFT_8 == keyID)
	//{

	//}
	
	

	if( PLAY_SPEED_NONE != playSpeed )
	{
		if (m_playerDlg.SetSpeed(playSpeed, bForward))
		{
			unsigned long speed = m_playerDlg.GetCurSpeed(bForward);

			MESSAGE_DATA msgData;
			memset(&msgData, 0, sizeof(MESSAGE_DATA));
			if(bForward)
			{
				CMessageMan::Instance()->SendMsgDataToServer(0, LOCAL_DEVICE_ID, CMD_REQUEST_LOCAL_CLIENT_PLAYBACK_FF, (unsigned char*)(&speed), sizeof(speed), msgData);
				if (PLAY_SPEED_1X == speed)
				{
					m_curPlaybackStatus = PLAYBACK_STATUS_PLAYING;
					m_playerDlg.PlayStatus();
				}
				else
				{
					m_curPlaybackStatus = PLAYBACK_STATUS_FF;
				}

			}
			else
			{
				CMessageMan::Instance()->SendMsgDataToServer(0, LOCAL_DEVICE_ID, CMD_REQUEST_LOCAL_CLIENT_PLAYBACK_REW, (unsigned char*)(&speed), sizeof(speed), msgData);
				m_curPlaybackStatus = PLAYBACK_STATUS_REW;
			}

			CMessageMan::Instance()->ReleaseMsgBuff(msgData);
		}
	}

#else

    PLAY_SPEED playSpeed = PLAY_SPEED_NONE;

	
	if(KEY_VIRTUAL_FLY_RIGHT_2 == keyID)
	{
		playSpeed = PLAY_SPEED_2X;
	 	
	}	
	else if(KEY_VIRTUAL_FLY_RIGHT_4 == keyID)
	{
		playSpeed = PLAY_SPEED_4X;
		
		
	}	
	else if(KEY_VIRTUAL_FLY_RIGHT_6 == keyID)
	{
		playSpeed = PLAY_SPEED_8X;
	}
	else if(KEY_VIRTUAL_FLY_RIGHT_7 == keyID)
	{
		playSpeed = PLAY_SPEED_16X;
	}	
	else if(KEY_VIRTUAL_FLY_MEDIUM == keyID)
	{
		//����ԭ
		playSpeed = PLAY_SPEED_1X;
	}	
	else if(KEY_VIRTUAL_FLY_LEFT_2 == keyID)
	{
		playSpeed = PLAY_SPEED_8X;
		
	}	
	else if(KEY_VIRTUAL_FLY_LEFT_4 == keyID)
	{
		playSpeed = PLAY_SPEED_16X;
		
	}	
	else if(KEY_VIRTUAL_FLY_LEFT_6 == keyID)
	{
		playSpeed = PLAY_SPEED_32X;
	}	

	 if( PLAY_SPEED_NONE != playSpeed )
	{
		if (m_playerDlg.SetSpeed(playSpeed, bForward))
		{
			unsigned long speed = m_playerDlg.GetCurSpeed(bForward);
			PLAYBACK_SPEED_INFO speedinfo;
			speedinfo.speed =speed;
			speedinfo.chnn =m_clickchnnINfo.clickchnn;
			MESSAGE_DATA msgData;
			memset(&msgData, 0, sizeof(MESSAGE_DATA));
			if(bForward)
			{
				CMessageMan::Instance()->SendMsgDataToServer(0, LOCAL_DEVICE_ID, CMD_REQUEST_LOCAL_CLIENT_PLAYBACK_FF, (unsigned char*)(&speedinfo), sizeof(PLAYBACK_SPEED_INFO), msgData);
				if (PLAY_SPEED_1X == speed)
				{
					m_curPlaybackStatus = PLAYBACK_STATUS_PLAYING;
					m_playerDlg.PlayStatus();
				}
				else
				{
					m_curPlaybackStatus = PLAYBACK_STATUS_FF;
				}

			}
			else
			{
				CMessageMan::Instance()->SendMsgDataToServer(0, LOCAL_DEVICE_ID, CMD_REQUEST_LOCAL_CLIENT_PLAYBACK_REW, (unsigned char*)(&speedinfo), sizeof(PLAYBACK_SPEED_INFO), msgData);
				m_curPlaybackStatus = PLAYBACK_STATUS_REW;
			}

			CMessageMan::Instance()->ReleaseMsgBuff(msgData);
		}
	}
#endif

}

void CPlaybackDlg::OnIdle()
{
#if !defined(__ASYN_PLAY__)
	if (m_curPlaybackStatus != PLAYBACK_STATUS_STOP)
	{
		if (m_bStopPlayMsg)
		{
			OnClickStop();
			m_bStopPlayMsg = false;
		}
		else
		{			
			bool bRefreshTime = false;			
			if (PLAYBACK_STATUS_PAUSE == m_curPlaybackStatus)
			{
				//��ͣʱ������ʹ�õ�֡���Ź��ܡ����ʹ���˵�֡���ܣ��ͻ�����
				//m_lastFrameTimeʱ��ı仯����m_lastFrameTime�ı仯��ȷ���Ƿ�ˢ��ʱ����
				if (m_preFrameTime != m_lastFrameTime)
				{
					bRefreshTime = true;
				}
			}
			else if ((PLAYBACK_STATUS_FF == m_curPlaybackStatus)||(PLAYBACK_STATUS_REW == m_curPlaybackStatus))
			{
				//������˵�ˢ��ʱ�䷽�������û���϶�ʱ���ᣬֻҪm_lastFrameTime�����仯����ˢ��ʱ���ᡣ
				//�����������Կ���ʱ��Ҳ�п���Ϳ��˵�Ч����
				if ((!m_bDragSlider) && (m_preFrameTime != m_lastFrameTime))
				{
					bRefreshTime = true;
				}
			}
			else
			{
				//���������Ż������ˣ�״̬�¡����û���϶�ʱ���ᣬ��Ҫʹ��һ���ķ���ˢ��ʱ���ᡣ
				if ((!m_bDragSlider) && (m_preFrameTime != m_lastFrameTime))
				{
					bRefreshTime = true;
				}
			}
			
			//Ҫ��֤m_lastFrameTime�е�ֵ��֡��ʱ�䡣
			if (bRefreshTime && (m_lastFrameTime > 0))
			{
				if ((m_lastFrameTime >= m_secStartTime) && (m_lastFrameTime <= m_secEndTime))
				{
					unsigned long pos = m_lastFrameTime - m_secStartTime;
					assert((pos >= 0) && (pos < SECTIME_LENGTH));
					m_playerDlg.SetTime(m_lastFrameTime, pos);						
				}
				else
				{
					GetSectionTime(m_lastFrameTime, m_secStartTime, m_secEndTime);
					m_playerDlg.SetSectionTime(m_secStartTime, m_secEndTime);
				}

				m_preFrameTime = m_lastFrameTime;
			}
		}
	}
#else
	if ((m_bSync && m_curPlaybackStatus != PLAYBACK_STATUS_STOP) || (!m_bSync && m_curStatus[m_clickchnnINfo.clickchnn] != PLAYBACK_STATUS_STOP))
	{
		if (m_bStopPlayMsg)
		{
			OnClickStop();
			m_bStopPlayMsg = false;
		}
		else
		{			
			bool bRefreshTime = false;			
			if ((m_bSync && PLAYBACK_STATUS_PAUSE == m_curPlaybackStatus) || (!m_bSync && m_curStatus[m_clickchnnINfo.clickchnn] == PLAYBACK_STATUS_PAUSE))
			{
				//��ͣʱ������ʹ�õ�֡���Ź��ܡ����ʹ���˵�֡���ܣ��ͻ�����
				//m_lastFrameTimeʱ��ı仯����m_lastFrameTime�ı仯��ȷ���Ƿ�ˢ��ʱ����
				if (m_preFrameTime != m_lastFrameTime)
				{
					bRefreshTime = true;
				}
			}
			else if ((m_bSync && (PLAYBACK_STATUS_FF == m_curPlaybackStatus)||(PLAYBACK_STATUS_REW == m_curPlaybackStatus)) ||  \
				(!m_bSync && ((PLAYBACK_STATUS_FF == m_curStatus[m_clickchnnINfo.clickchnn])||(PLAYBACK_STATUS_REW == m_curStatus[m_clickchnnINfo.clickchnn]))))
			{
				//������˵�ˢ��ʱ�䷽�������û���϶�ʱ���ᣬֻҪm_lastFrameTime�����仯����ˢ��ʱ���ᡣ
				//�����������Կ���ʱ��Ҳ�п���Ϳ��˵�Ч����
				if ((!m_bDragSlider) && (m_preFrameTime != m_lastFrameTime))
				{
					bRefreshTime = true;
				}
			}
			else
			{
				//���������Ż������ˣ�״̬�¡����û���϶�ʱ���ᣬ��Ҫʹ��һ���ķ���ˢ��ʱ���ᡣ
				if ((!m_bDragSlider) && (m_preFrameTime != m_lastFrameTime))
				{
					bRefreshTime = true;
				}
			}
			
			//Ҫ��֤m_lastFrameTime�е�ֵ��֡��ʱ�䡣
			 if (bRefreshTime && (m_lastFrameTime > 0))
			{
				
				if ((m_lastFrameTime >= m_secStartTime) && (m_lastFrameTime <= m_secEndTime))
				{
					unsigned long pos = m_lastFrameTime - m_secStartTime;
					assert((pos >= 0) && (pos < SECTIME_LENGTH));
					m_playerDlg.SetTime(m_lastFrameTime, pos);						
				}
				else
				{					
					GetSectionTime(m_lastFrameTime, m_secStartTime, m_secEndTime);
					m_playerDlg.SetSectionTime(m_secStartTime, m_secEndTime);
				}
				
				m_preFrameTime = m_lastFrameTime;
			}
		}
	}

#endif

#ifdef __CUSTOM_US02_NEW__
	if(m_pTipWindow != NULL)
	{
		if(m_pTipWindow->IsShow())
		{
			unsigned long curTime = GetCurrTime32();
			if((curTime - m_pTipWindow->GetShowTime()) >= 5)
			{
				m_pTipWindow->Show(false, true);
				GetRootWnd()->Repaint();
			}
		}
	}
#endif
}



int CPlaybackDlg::ProcCallBack(void* pObject, int iMsg, void *pParam1, void *pParam2)
{
	CPlaybackDlg * pPlayBack = (CPlaybackDlg*)pObject;
	return pPlayBack->ProcCallBack(iMsg, pParam1, pParam2);
}


int CPlaybackDlg::ProcCallBack(int iMsg, void *pParam1, void *pParam2)
{
#if !defined(__ASYN_PLAY__)
	if (1 == iMsg)
	{
		m_lastFrameTime = (*reinterpret_cast<LONGLONG *>(pParam1))/1000000;
	}
	else if (2 == iMsg)
	{
		m_bStopPlayMsg = true;
	}
#else
	if (1 == iMsg)
	{
		if (!m_bSync)
		{
			if (m_clickchnnINfo.clickchnn == (*reinterpret_cast<unsigned long *>(pParam2)))
			{
				m_lastFrameTime = (*reinterpret_cast<LONGLONG *>(pParam1))/1000000;
			}
		}
		else
		{
			m_lastFrameTime = (*reinterpret_cast<LONGLONG *>(pParam1))/1000000;
		}
	}
	else if (2 == iMsg)
	{
		m_bStopPlayMsg = true;
	}
#endif
#ifdef __TDFH__
	else if (3 == iMsg)
	{
		if (m_zoomDlg.IsInZoom())
		{
			KEY_VALUE_INFO keyValue;
			keyValue.type = KEY_TYPE_VIRTUAL;
			keyValue.value = KEY_VIRTUAL_ESC;
			AddKeyValueEx(keyValue); 
		}
	}
#endif
	return 0;
}



void CPlaybackDlg::InitialBtnStatus()
{

	PLAYER_CTRL_SPEED_ITEM item;

	item.speed = PLAY_SPEED_16X;
	strcpy(item.szSpeedName, ">>16X");
	m_ctrlSpeedForwardList.AddTail(item);
	item.speed = PLAY_SPEED_8X;
	strcpy(item.szSpeedName, ">>8X");
	m_ctrlSpeedForwardList.AddTail(item);
	item.speed = PLAY_SPEED_4X;
	strcpy(item.szSpeedName, ">>4X");
	m_ctrlSpeedForwardList.AddTail(item);
	item.speed = PLAY_SPEED_2X;
	strcpy(item.szSpeedName, ">>2X");
	m_ctrlSpeedForwardList.AddTail(item);
	item.speed = PLAY_SPEED_1X;
	strcpy(item.szSpeedName, ">>1X");
	m_ctrlSpeedForwardList.AddTail(item);
	item.speed = PLAY_SPEED_1_2X;
	strcpy(item.szSpeedName, ">>1/2X");
	m_ctrlSpeedForwardList.AddTail(item);
	item.speed = PLAY_SPEED_1_4X;
	strcpy(item.szSpeedName, ">>1/4X");
	m_ctrlSpeedForwardList.AddTail(item);

	item.speed = PLAY_SPEED_1X;
	strcpy(item.szSpeedName, ">>1X");
	m_ctrlSpeedRewardList.AddTail(item);
	item.speed = PLAY_SPEED_32X;
	strcpy(item.szSpeedName, "<<32X");
	m_ctrlSpeedRewardList.AddTail(item);
	item.speed = PLAY_SPEED_16X;
	strcpy(item.szSpeedName, "<<16X");
	m_ctrlSpeedRewardList.AddTail(item);
	item.speed = PLAY_SPEED_8X;
	strcpy(item.szSpeedName, "<<8X");
	m_ctrlSpeedRewardList.AddTail(item);
	
	m_playerDlg.SetSpeedRang(&m_ctrlSpeedForwardList, &m_ctrlSpeedRewardList);

	m_playerDlg.SetMaxSplitMode(m_maxSplitMode, m_bChangePlayChnn);
}


void CPlaybackDlg::SetSplitMode(ULONGLONG chnnBits)
{
	assert(chnnBits != 0);

	unsigned char channNum = 0;
	unsigned char *pChannel = NULL;
	unsigned char index = 0;

	m_preSplitModeInfoList.RemoveAll();
	m_preSplitMode = m_curSplitMode;
	if (m_curSplitMode != PLAY_CH_NULL)
	{
		POS pos = m_curSplitModeInfoList.GetHeadPosition();
		while (pos != NULL)
		{
			PLAYBACK_CHANNEL_INFO &channelInfo = m_curSplitModeInfoList.GetNext(pos);
			m_preSplitModeInfoList.AddTail(channelInfo);
		}
	}
	m_curSplitModeInfoList.RemoveAll();
	m_curSplitMode      = PLAY_CH_NULL;

	pChannel = new unsigned char [m_maxChannelNum];	
	for (unsigned char chnn = 0; chnn < m_maxChannelNum; ++chnn)
	{
		if (chnnBits & (((ULONGLONG)1) << chnn))
		{
			pChannel[channNum] = chnn;
			++channNum;
		}
	}

	assert(channNum > 0);

	//��ȡ��Ƶͨ��
	unsigned long audioChnn = m_playerDlg.GetAudioCH();
	CMessageMan::Instance()->PostMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_LOCAL_CLIENT_PLAYBACK_CHG_AUDIO, (unsigned char *)(&audioChnn), sizeof(unsigned long));
    //end

	m_curSplitMode = GetSplitMode(channNum);
	
	assert(m_curSplitMode <= m_maxSplitMode);

	for (index = 0; index < channNum; ++index)
	{
		PLAYBACK_CHANNEL_INFO info;
		info.channel = pChannel[index];
		m_curSplitModeInfoList.AddTail(info);
	}
	
	delete [] pChannel;

	GetRootWnd()->Repaint();
}

int CPlaybackDlg::GetSplitChannelNum(PLAY_CH_MODE mode)
{
	if (PLAY_CH_ONE == mode)
	{
		return 1;
	}
	else if (PLAY_CH_FOUR == mode)
	{
		return 4;
	}
	else if (PLAY_CH_NINE == mode)
	{
		return 9;
	}
	else if (PLAY_CH_SIXTEEN == mode)
	{
		return 16;
	}
	else
	{
		assert (false);
		return 1;
	}
}

PLAY_CH_MODE CPlaybackDlg::GetSplitMode(unsigned char videoOutPutNum)
{
	assert(videoOutPutNum > 0);

	PLAY_CH_MODE mode;
	if (1 == videoOutPutNum)
	{
		mode = PLAY_CH_ONE;
	}
	else if (videoOutPutNum <= 4)
	{
		mode = PLAY_CH_FOUR;
	}
	else if (videoOutPutNum <= 6)
	{
		mode = PLAY_CH_SIX;
	}
	else if(videoOutPutNum <= 8)
	{
		mode = PLAY_CH_NINE;
	}
	else if (videoOutPutNum <= 9)
	{
		mode = PLAY_CH_NINE;
	}
	else if(videoOutPutNum <= 13)
	{
		mode = PLAY_CH_THIRTEEN;

	}
	else if (videoOutPutNum <= 16)
	{
		mode = PLAY_CH_SIXTEEN;
	}
	else
	{
		assert(false);
	}
	return mode;
}


bool CPlaybackDlg::GetCurDisplay(NET_DISPLAY_INFO& disp, unsigned short x, unsigned short y, bool bUsePt)
{
	bool ret = false;

	NET_DISPLAY_INFO inDisp;
	memset(&inDisp, 0, sizeof(NET_DISPLAY_INFO));
	inDisp.x = x;
	inDisp.y = y;

	if(bUsePt)
	{
		inDisp.type = NET_DISPLAY_GET_INFO_XY;
	}
	else
	{
		inDisp.type = NET_DISPLAY_GET_INFO;
	}

	MESSAGE_DATA msgData;
	memset(&msgData, 0, sizeof(MESSAGE_DATA));
	CMessageMan::Instance()->SendMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_GET_DISPLAY_INFO, (unsigned char*)&inDisp, sizeof(NET_DISPLAY_INFO), msgData);
	assert(NULL != msgData.pData);
	PACKCMD *pCmd = reinterpret_cast<PACKCMD *>(msgData.pData);
	if (CMD_REPLY_CTRL_SUCC == static_cast<NET_PROTOCOL>(pCmd->cmdType))
	{
		//��ȡ��ǰ��ʾ��ͨ��
		NET_DISPLAY_INFO* pDispInfo = reinterpret_cast<NET_DISPLAY_INFO*>(msgData.pData + sizeof(PACKCMD));

		memcpy(&disp, pDispInfo, sizeof(NET_DISPLAY_INFO));

		ret = true;
	}
	CMessageMan::Instance()->ReleaseMsgBuff(msgData);

	return ret;
}

void CPlaybackDlg::ChangeDisplayByTwoPt(unsigned short x1, unsigned short y1, unsigned short x2, unsigned short y2, unsigned long timeout, unsigned short bRec)
{
	NET_DISPLAY_INFO dispInfo;
	memset(&dispInfo, 0 ,sizeof(NET_DISPLAY_INFO));
	dispInfo.type = NET_DISPLAY_EXCHANGE_TWO_PT;
	dispInfo.time_out = timeout;
	dispInfo.rec = bRec;

	dispInfo.data[0] = x1;
	dispInfo.data[1] = y1;
	dispInfo.data[2] = x2;
	dispInfo.data[3] = y2;
	dispInfo.data_len = 4;

	MESSAGE_DATA msgData;
	memset(&msgData, 0, sizeof(MESSAGE_DATA));
	CMessageMan::Instance()->SendMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_CHANGE_DISPLAY_MODE, (unsigned char*)&dispInfo, sizeof(NET_DISPLAY_INFO), msgData);	
	//PACKCMD *pCmd = reinterpret_cast<PACKCMD *>(msgData.pData);
	//if (CMD_REPLY_CTRL_SUCC != static_cast<NET_PROTOCOL>(pCmd->cmdType))
	//{
	//	NET_DISPLAY_INFO* pDispInfo = reinterpret_cast<NET_DISPLAY_INFO*>(msgData.pData + sizeof(PACKCMD));
	//	memcpy(&dispInfo, pDispInfo, sizeof(NET_DISPLAY_INFO));
	//}
	CMessageMan::Instance()->ReleaseMsgBuff(msgData);
}

/***************************************************************************************
������OnPreCommand()
���ܣ����ذ�����Ϣ���жϵ�ǰ�û��Ƿ���Ȩ�޻ط�ĳͨ��¼��
				��Ҫ���ڰ������ң�����������ж�
wn	2011-07-16
***************************************************************************************/
unsigned long CPlaybackDlg::OnPreCommand(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	//--------------2011.7.16-------------
	ULONGLONG authCH = CUserMan::Instance()->CheckAuthority(AUTH_PLAYBACK);
	if ((KEY_VIRTUAL_1 <= keyID) && (KEY_VIRTUAL_32 >= keyID))
	{
		unsigned long nTemp = static_cast<unsigned long>(keyID & 0x00FF);

		if (!(static_cast<unsigned long>(authCH>>(nTemp - 1)) & 0x0001))
		{
			return KEY_VIRTUAL_MSG_STOP;
		}
	}
	return GUI::CDialog::OnPreCommand(ctrlID, keyID, xPos, yPos);
}


//////////////////////////////
#ifdef __CUSTOM_US02_NEW__
unsigned long CPlaybackDlg::OnOver()
{
	CWnd* active = GetActiveCtrl();
	if(active == NULL) return KEY_VIRTUAL_STOP;
	
	if(active != NULL)
	{
		CWnd* aactive = active->GetActiveCtrl();
		CWnd* aaactive = aactive->GetActiveCtrl();

		int x, y;
		aactive->GetFocusPos(x, y);
		if(aaactive != NULL)
		{
			aactive = aaactive;
			aaactive->GetFocusPos(x, y);
		}
		if(aactive->GetID() == CTRLID_TOOL_BAR_DLG) return KEY_VIRTUAL_STOP;
		CTRLID ctrl = aactive->GetID();
		std::string caption;
		switch(ctrl)
		{
			case CTRLID_PLAYER_PLAY_PAUSE:
				{
					caption.clear();
					if(aactive->IsEnable())
					{
						#ifndef __ASYN_PLAY__
						if(m_playerDlg.GetCurrStatus() == PLAYBACK_STATUS_PLAYING)
						{
							caption.assign(m_pStrTable->String(E_STR_ID_PLAYER_TIP_PAUSE));
						}
						else
						{
							caption.assign(m_pStrTable->String(E_STR_ID_PLAYER_TIP_PLAY));
						}
						#endif
					}			
				}
				break;
			case CTRLID_PLAYER_STOP:
				{
					caption.clear();
					if(aactive->IsEnable())
					{
						caption.assign(m_pStrTable->String(E_STR_ID_PLAYER_TIP_STOP));
					}
				}
				break;
			case CTRLID_PLAYER_NEXTFRAME:
				{
					caption.clear();
					if(aactive->IsEnable())
					{
						caption.assign(m_pStrTable->String(E_STR_ID_PLAYER_TIP_FRAME_ADVANCE));
					}
				}
				break;
			case CTRLID_PLAYER_FASTFORWARD:
				{
					caption.clear();
					if(aactive->IsEnable())
					{
						caption.assign(m_pStrTable->String(E_STR_ID_PLAYER_TIP_FAST_FORWARD));
					}
				}
				break;
			case CTRLID_PLAYER_REWARD:
				{
					caption.clear();
					if(aactive->IsEnable())
					{
						caption.assign(m_pStrTable->String(E_STR_ID_PLAYER_TIP_REWIND));
					}
				}
				break;
			case CTRLID_PLAYER_ONESPLIT:
				{
					caption.clear();
					if(aactive->IsEnable())
					{
						caption.assign(m_pStrTable->String(E_STR_ID_TOOLBAR_TIP_1X1));
					}
				}
				break;
			case CTRLID_PLAYER_FOURSPLIT:
				{
					caption.clear();
					if(aactive->IsEnable())
					{
						caption.assign(m_pStrTable->String(E_STR_ID_TOOLBAR_TIP_2X2));
					}
				}
				break;
			case CTRLID_PLAYER_SIXSPLIT:
				{
					caption.clear();
					if(aactive->IsEnable())
					{
						caption.assign(m_pStrTable->String(E_STR_ID_TOOLBAR_TIP_2X3));
					}
				}
				break;
			case CTRLID_PLAYER_NINESPLIT:
				{
					caption.clear();
					if(aactive->IsEnable())
					{
						caption.assign(m_pStrTable->String(E_STR_ID_TOOLBAR_TIP_3X3));
					}
				}
				break;
			case CTRLID_PLAYER_SIXTEENSPLIT:
				{
					caption.clear();
					if(aactive->IsEnable())
					{
						caption.assign(m_pStrTable->String(E_STR_ID_TOOLBAR_TIP_4X4));
					}
				}
				break;
			case CTRLID_PLAYER_ZOOM:
				{
					caption.clear();
					if(aactive->IsEnable())
					{
						caption.assign(m_pStrTable->String(E_STR_ID_TOOLBAR_TIP_ZOOM));
					}
				}
				break;
			case CTRLID_PLAYER_COLOR:
				{
					caption.clear();
					if(aactive->IsEnable())
					{
						caption.assign(m_pStrTable->String(E_STR_ID_TOOLBAR_TIP_COLOR));
					}
				}
				break;
			/*case CTRLID_PLAYER_BACKUP_START_TIME:
				{
					caption.clear();
					if(aactive->IsEnable())
					{
						caption.assign(m_pStrTable->String(E_STR_ID_PLAYER_TIP_START_CLIP));
					}
				}
				break;
			case CTRLID_PLAYER_BACKUP_END_TIME:
				{
					caption.clear();
					if(aactive->IsEnable())
					{
						caption.assign(m_pStrTable->String(E_STR_ID_PLAYER_TIP_END_CLIP));
					}
				}
				break;*/
			case CTRLID_PLAYER_BACKUP_SELECT_TIME:
				{
					caption.clear();
					if(aactive->IsEnable())
					{
						if(dynamic_cast<CStateButton *>(aactive)->GetCurState() == 1)
						{
							caption.assign(m_pStrTable->String(E_STR_ID_PLAYER_TIP_END_CLIP));
						}
						else
						{
							caption.assign(m_pStrTable->String(E_STR_ID_PLAYER_TIP_START_CLIP));
						}
					}
				}
				break;
			case CTRLID_PLAYER_AUDIO:
				{
					caption.clear();
					if(aactive->IsEnable())
					{
						caption.assign(m_pStrTable->String(E_STR_ID_TOOLBAR_TIP_AUDIO));
					}
				}
				break;
			/*case CTRLID_PLAYER_SNAP:
				{
					caption.clear();
					if(aactive->IsEnable())
					{
						caption.assign(m_pStrTable->String(E_STR_ID_PLAYER_TIP_STOP));
					}
				}
				break;*/
			case CTRLID_PLAYER_BACKUP:
				{
					caption.clear();
					if(aactive->IsEnable())
					{
						caption.assign(m_pStrTable->String(E_STR_ID_PLAYER_TIP_BACKUP));
					}
				}
				break;
			case CTRLID_PLAYER_LIVE:
				{
					caption.clear();
					if(aactive->IsEnable())
					{
						caption.assign(m_pStrTable->String(E_STR_ID_PLAYER_TIP_LIVE_CAMERA));
					}
				}
				break;
			case CTRLID_PLAYER_EXIT:
				{
					caption.clear();
					if(aactive->IsEnable())
					{
						caption.assign(m_pStrTable->String(E_STR_ID_PLAYER_TIP_EXIT));
					}
				}
				break;
			case CTRLID_PLAYER_HIDE:
				{
					caption.clear();
					if(aactive->IsEnable())
					{
						caption.assign(m_pStrTable->String(E_STR_ID_PLAYER_TIP_HIDE_CONTROLS));
					}
				}
				break;	
			case CTRLID_PLAYER_NEXT_SECTION:
				{
					caption.clear();
					if(aactive->IsEnable())
					{
						caption.assign(m_pStrTable->String(E_STR_ID_PLAYER_TIP_NEXT_RECORD));
					}
				}
				break;
			case CTRLID_PLAYER_PRE_SECTION:
				{
					caption.clear();
					if(aactive->IsEnable())
					{
						caption.assign(m_pStrTable->String(E_STR_ID_PLAYER_TIP_PREVIOUS_RECORD));
					}
				}
				break;
			case CTRLID_PLAYER_MOVE:
				{
					caption.clear();
					if(aactive->IsEnable())
					{
						caption.assign(m_pStrTable->String(E_STR_ID_TOOLBAR_TIP_MOVE));
					}
				}
				break;
			/*case CTRLID_PLAYER_CAM_PARA:
				{
					caption.clear();
					if(aactive->IsEnable())
					{
						caption.assign(m_pStrTable->String(E_STR_ID_TOOLBAR_TIP_CAM_PARAM));
					}
				}
				break;*/
			default:
				{	printf("ctrl=%d\n", ctrl);
					return KEY_VIRTUAL_STOP;
				}
		}
		GUI::CRect rect = aactive->GetWindowRect();

		int xx = (caption.size()+2)*10;
		x = rect.m_left + rect.Width()/2;
		
		GUI::CRect rootRect = GetWindowRect();
		if((rootRect.m_right-x) < xx/2)
		{
			x = rootRect.m_right - xx;
		}
		else
		{
			x = (x > xx/2) ? (x - xx/2) : rect.m_left;
		}
		if((ctrl == CTRLID_TOOL_BAR_SYSTEM))
		{
			//y = m_stcTime.GetWindowRect().m_top - m_length[STATIC_HEIGHT] - 10;
		}
		else
		{
			y = (rect.m_top > m_height / 2) ? (rect.m_top - m_length[STATIC_HEIGHT]) : rect.m_bottom;
			if(rect.m_top > m_height / 2)
			{
				y -= 10;
			}
		}
		if(m_pTipWindow == NULL)
		{
			m_pTipWindow = new CTipWindow();
			m_pTipWindow->Create(GetRandWndID(), x, y, xx, m_length[STATIC_HEIGHT], this, caption.c_str());
		}

		m_pTipWindow->SetShowTime(GetCurrTime32());

			//m_pTipWindow->SetWndSize(false, false, xx, m_length[STATIC_HEIGHT]);
		m_pTipWindow->SetWndPos(x, y, xx, m_length[STATIC_HEIGHT]);
		m_pTipWindow->SetCaption(caption.c_str(), false);
		m_pTipWindow->MoveWnd(x, y);


		m_pTipWindow->SetAlignText(ALIGN_TEXT_CENTER);
		m_pTipWindow->SetBKColor(0x737b7b, true);
		m_pTipWindow->SetTextColor(0xffff00);
		m_pTipWindow->SetEnable(true, false);	

		m_pTipWindow->Show(true, true);
		GetRootWnd()->Repaint();
	}
	return KEY_VIRTUAL_STOP;
}

unsigned long CPlaybackDlg::OnOut()
{

		if(m_pTipWindow != NULL)
		{
			m_pTipWindow->Show(false, true);
			GetRootWnd()->Repaint();
		}
		return KEY_VIRTUAL_STOP;
}
#endif

//end
