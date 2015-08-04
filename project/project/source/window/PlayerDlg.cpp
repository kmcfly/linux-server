/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : hz
** Date         : 2009-08-12
** Name         : PlayerDlg.cpp
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/

#include "PlayerDlg.h"
#include "Product.h"
#include "GuiAssert.h"

#include "FuncCustom.h"

#include "NetProtocol.h"
#include "MessageMan.h"

#include "UserMan.h"
#include "BackUpInfoDlg.h"
#include "CutKeepLog.h"
extern NET_LOGIN_SUCCESS_REPLY *g_p_login_succ;
extern int MAX_BACKUP_SEQ;

//////////////////////////////////////////////////////////////////////////
CPlayerDlg::CPlayerDlg()
{
#if defined(__NEW_CUT_BACKUP__)
	m_curRedSelectChnn      = INVALID_INDEX(unsigned long);
#endif
	m_pCtrlSpeedForwardList = NULL;
	m_pCtrlSpeedRewardList  = NULL;

#if !defined(__ASYN_PLAY__)
	m_curForwardSpeed       = PLAY_SPEED_1X;
	m_curRewardSpeed        = PLAY_SPEED_1X;
	m_currStatus			= PLAYBACK_STOP;
#else
	for (int i = 0; i < g_p_login_succ->productInfo.localVideoInputNum; i++)
	{
		m_curForwardSpeed[i]       = PLAY_SPEED_1X;
		m_curRewardSpeed[i]        = PLAY_SPEED_1X;
		m_currStatus[i]	    	   = PLAYBACK_STOP;
	}
	m_liveSelCH	=0;
	m_bSync = true;
#endif
	m_bChangePlayChnn       = true;
	m_maxSelChnn			= 0;
	m_maxChnnNum			= 0;
	m_selCuPlayrChnn		= 0x1;
	m_selAudioCH			= 0;

	m_1X1CH					= 0x0001;
	m_2X2CH					= 0x000f;
	m_2X3CH					= 0x003f;
	m_1A5CH					= 0x003f;
	m_1A7CH					= 0x00ff;
	m_1A12CH				= 0x1fff;
	m_3X3CH					= 0x01ff;
	m_4X4CH					= 0xffff;

	m_currSplitMode			= PLAY_CH_NULL;

	m_bmpBgWidth = 0;
	m_bmpBgHeight = 0;

	m_xPosLeft = -1;
	m_yPosTop = -1;
	m_diskOwnerIndex = 0;
		
}

CPlayerDlg::~CPlayerDlg()
{
	m_pCtrlSpeedForwardList = NULL;
	m_pCtrlSpeedRewardList  = NULL;

#if !defined(__ASYN_PLAY__)
	m_curForwardSpeed       = 0;
	m_curRewardSpeed        = 0;
#endif

}


void CPlayerDlg::SetMaxSelChnn(int chnnNum, int maxSel) 
{
	m_maxChnnNum = chnnNum;

	m_maxSelChnn = maxSel;
	   
	m_playAudioMaxChnnNum = g_p_login_succ->productInfo.videoInputNum;

	m_audioChnnMask = ALL_CHNN_MASK(g_p_login_succ->productInfo.audioInputNum);
	for(long chnn = g_p_login_succ->productInfo.localVideoInputNum; chnn < g_p_login_succ->productInfo.videoInputNum; chnn++)
	{
		m_audioChnnMask |= ((ULONGLONG)0x01) << chnn;
	}

	if(maxSel == 8)
	{
		//当16路机器,最大8路回放时,调整3x3模式的初值
		m_3X3CH = 0x00ff;
	}
}

void CPlayerDlg::SetCurPlayChnn(ULONGLONG curChnn)
{
	m_selCuPlayrChnn = curChnn;

#if defined(__ASYN_PLAY__)
	for(int i = 0; i < 32; i++)
	{
		if (curChnn & (0x01 << i))
		{
			m_currStatus[i] = PLAYBACK_PLAYING;
		}
	}
#endif

}

void CPlayerDlg::SetMaxSplitMode(PLAY_CH_MODE splitMode, bool bChangePlayChnn)
{
	m_maxSplitMode = splitMode;
    m_bChangePlayChnn = bChangePlayChnn;

//	m_currSplitMode	= splitMode;
}

void CPlayerDlg::SetSpeedRang(PLAYER_CTRL_SPEED_LIST *pCtrlSpeedForward, PLAYER_CTRL_SPEED_LIST *pCtrlSpeedReward)
{
	assert(m_pCtrlSpeedForwardList == NULL);
	assert(m_pCtrlSpeedRewardList == NULL);

	m_pCtrlSpeedForwardList = pCtrlSpeedForward;
	m_pCtrlSpeedRewardList  = pCtrlSpeedReward;

}

unsigned long CPlayerDlg::GetAudioCH()
{
	if (0 == m_selAudioCH)
	{
		return INVALID_INDEX(unsigned long);
	}
	else
	{
		for (unsigned char i=0; i<m_maxChnnNum; ++i)
		{
			if ( ((ULONGLONG)(0x01) << i) & (m_selAudioCH))
			{
				return i;
			}
		}

		return INVALID_INDEX(unsigned long);
	}
}

void CPlayerDlg::OnInitial()
{
	assert(m_pCtrlSpeedForwardList != NULL);
	assert(m_pCtrlSpeedRewardList != NULL);
#if defined(__NEW_CUT_BACKUP__)
	m_searchDataVector.clear();
#else
	m_recoverStatus=0;
	m_backCnt=0;//按下剪切按钮的次数
#endif
	m_style |= WND_CHILD;
	GUI::CDialog::OnInitial();
	////////////////////////////////////////////////////
	m_sliderStartTime=0;
	m_sliderEndTime=0;
	m_startBackTime=0;
	m_endBackTime=0;
	m_spectype=SPEC_NONE;
	
	int x = 3;
	int y = 3;
	int width = 24;
	int height = 24;
	int offset_x = 8;
	int xFocus = 0;
	int showNameFocus = 0;

	//2010-03-08 19:50:00 YSW
	//由于某些监视器不能完整地显示整个输出，但接这样的监视器时，就会出现最下面的工具条不能看全的情况
	//此时需要特殊处理一下
	if (800 > m_width)
	{
		x += 40; 
	}

	m_btnPlayPause.Create(CTRLID_PLAYER_PLAY_PAUSE, x, y, width, height, this, xFocus++, 1);
	m_btnPlayPause.SetImage(PAINT_STATUS_NORMAL, BMP_BTN_PLAY, false);
	m_btnPlayPause.SetImage(PAINT_STATUS_FOCUS, BMP_BTN_PLAY_FOCUS, false);
	m_btnPlayPause.SetImage(PAINT_STATUS_DISABLE, BMP_BTN_PLAY_DISABLE, false);

	x += 30;
	m_btnStop.Create(CTRLID_PLAYER_STOP, x, y, width, height, this, xFocus++, 1);
	m_btnStop.SetImage(PAINT_STATUS_NORMAL, BMP_BTN_STOP, false);
	m_btnStop.SetImage(PAINT_STATUS_FOCUS, BMP_BTN_STOP_FOCUS, false);
	m_btnStop.SetImage(PAINT_STATUS_DISABLE, BMP_BTN_STOP_DISABLE, false);

	x += 30;
	m_btnNextFrame.Create(CTRLID_PLAYER_NEXTFRAME, x, y, width, height, this, xFocus++, 1);
	m_btnNextFrame.SetImage(PAINT_STATUS_NORMAL, BMP_BTN_NEXT_FRAME, false);
	m_btnNextFrame.SetImage(PAINT_STATUS_FOCUS, BMP_BTN_NEXT_FRAME_FOCUS, false);
	m_btnNextFrame.SetImage(PAINT_STATUS_DISABLE, BMP_BTN_NEXT_FRAME_DISABLE, false);

	x += 30;
	m_btnFastForward.Create(CTRLID_PLAYER_FASTFORWARD, x, y, width, height, this, xFocus++, 1);
	m_btnFastForward.SetImage(PAINT_STATUS_NORMAL, BMP_BTN_FAST_FORWARD, false);
	m_btnFastForward.SetImage(PAINT_STATUS_FOCUS, BMP_BTN_FAST_FORWARD_FOCUS, false);
	m_btnFastForward.SetImage(PAINT_STATUS_DISABLE, BMP_BTN_FAST_FORWARD_DISABLE, false);
	x += 28;
	m_btnFFEx.Create(CTRLID_PLAYER_FFEX, x, y+5, width, height, this, xFocus++, 1);
	m_btnFFEx.SetImage(PAINT_STATUS_NORMAL, BMP_BTN_PLAY_TRIANGLE, false);
	m_btnFFEx.SetImage(PAINT_STATUS_FOCUS, BMP_BTN_PLAY_TRIANGLE_F, false);
	m_btnFFEx.SetImage(PAINT_STATUS_DISABLE, BMP_BTN_PLAY_TRIANGLE, false);

	x += 14;
	m_btnReward.Create(CTRLID_PLAYER_REWARD, x, y, width, height, this, xFocus++, 1);
	m_btnReward.SetImage(PAINT_STATUS_NORMAL, BMP_BTN_REWARD, false);
	m_btnReward.SetImage(PAINT_STATUS_FOCUS, BMP_BTN_REWARD_FOCUS, false);
	m_btnReward.SetImage(PAINT_STATUS_DISABLE, BMP_BTN_REWARD_DISABLE, false);
	x += 28;
	m_btnREWEx.Create(CTRLID_PLAYER_REWEX, x, y+5, width, height, this, xFocus++, 1);
	m_btnREWEx.SetImage(PAINT_STATUS_NORMAL, BMP_BTN_PLAY_TRIANGLE, false);
	m_btnREWEx.SetImage(PAINT_STATUS_FOCUS, BMP_BTN_PLAY_TRIANGLE_F, false);
	m_btnREWEx.SetImage(PAINT_STATUS_DISABLE, BMP_BTN_PLAY_TRIANGLE, false);

	//分割
	//1X1
	x += 30;
	m_btnOneSplit.Create(CTRLID_PLAYER_ONESPLIT, x, y, width, height, this, xFocus++, 1);
	m_btnOneSplit.SetImage(PAINT_STATUS_NORMAL, BMP_BTN_PLAY_1X1, false);
	m_btnOneSplit.SetImage(PAINT_STATUS_FOCUS, BMP_BTN_PLAY_1X1_F, false);
	m_btnOneSplit.SetImage(PAINT_STATUS_DISABLE, BMP_BTN_PLAY_1X1_N, false);
	m_btnOneSplit.Show(false, false);
	x += 26;
	m_btn1X1Ex.Create(CTRLID_PLAYER_1X1EX, x, y+5, width, height, this, xFocus++, 1);
	m_btn1X1Ex.SetImage(PAINT_STATUS_NORMAL, BMP_BTN_PLAY_TRIANGLE, false);
	m_btn1X1Ex.SetImage(PAINT_STATUS_FOCUS, BMP_BTN_PLAY_TRIANGLE_F, false);
	m_btn1X1Ex.SetImage(PAINT_STATUS_DISABLE, BMP_BTN_PLAY_TRIANGLE, false);
	m_btn1X1Ex.Show(false, false);

	//2X2
	x += 14;
	m_btnFourSplit.Create(CTRLID_PLAYER_FOURSPLIT, x, y, width, height, this, xFocus++, 1);
	m_btnFourSplit.SetImage(PAINT_STATUS_NORMAL, BMP_BTN_PLAY_2X2, false);
	m_btnFourSplit.SetImage(PAINT_STATUS_FOCUS, BMP_BTN_PLAY_2X2_F, false);
	m_btnFourSplit.SetImage(PAINT_STATUS_DISABLE, BMP_BTN_PLAY_2X2_N, false);
	m_btnFourSplit.Show(false, false);
	x += 26;
	m_btn2X2Ex.Create(CTRLID_PLAYER_2X2EX, x, y+5, width, height, this, xFocus++, 1);
	m_btn2X2Ex.SetImage(PAINT_STATUS_NORMAL, BMP_BTN_PLAY_TRIANGLE, false);
	m_btn2X2Ex.SetImage(PAINT_STATUS_FOCUS, BMP_BTN_PLAY_TRIANGLE_F, false);
	m_btn2X2Ex.SetImage(PAINT_STATUS_DISABLE, BMP_BTN_PLAY_TRIANGLE, false);
	m_btn2X2Ex.Show(false, false);

	//2X3
	x += 14;
	m_btnSixSplit.Create(CTRLID_PLAYER_SIXSPLIT,x,y,width,height,this, xFocus++,1);
	m_btnSixSplit.SetImage(PAINT_STATUS_NORMAL, BMP_BTN_PLAY_2X3, false);
	m_btnSixSplit.SetImage(PAINT_STATUS_FOCUS, BMP_BTN_PLAY_2X3_F, false);
	m_btnSixSplit.SetImage(PAINT_STATUS_DISABLE, BMP_BTN_PLAY_2X3_N, false);
	m_btnSixSplit.Show(false, false);
	x += 26;
	m_btn2X3Ex.Create(CTRLID_PLAYER_2X3EX, x, y+5, width, height, this, xFocus++, 1);
	m_btn2X3Ex.SetImage(PAINT_STATUS_NORMAL, BMP_BTN_PLAY_TRIANGLE, false);
	m_btn2X3Ex.SetImage(PAINT_STATUS_FOCUS, BMP_BTN_PLAY_TRIANGLE_F, false);
	m_btn2X3Ex.SetImage(PAINT_STATUS_DISABLE, BMP_BTN_PLAY_TRIANGLE, false);
	m_btn2X3Ex.Show(false, false);

	//3X3
	x += 14;
	m_btnNineSplit.Create(CTRLID_PLAYER_NINESPLIT, x, y, width, height, this, xFocus++, 1);
	m_btnNineSplit.SetImage(PAINT_STATUS_NORMAL, BMP_BTN_PLAY_3X3, false);
	m_btnNineSplit.SetImage(PAINT_STATUS_FOCUS, BMP_BTN_PLAY_3X3_F, false);
	m_btnNineSplit.SetImage(PAINT_STATUS_DISABLE, BMP_BTN_PLAY_3X3_N, false);
	m_btnNineSplit.Show(false, false);
	x += 26;
	m_btn3X3Ex.Create(CTRLID_PLAYER_3X3EX, x, y+5, width, height, this, xFocus++, 1);
	m_btn3X3Ex.SetImage(PAINT_STATUS_NORMAL, BMP_BTN_PLAY_TRIANGLE, false);
	m_btn3X3Ex.SetImage(PAINT_STATUS_FOCUS, BMP_BTN_PLAY_TRIANGLE_F, false);
	m_btn3X3Ex.SetImage(PAINT_STATUS_DISABLE, BMP_BTN_PLAY_TRIANGLE, false);
	m_btn3X3Ex.Show(false, false);

	//4X4
	x += 14;
	m_btnSixteenSplit.Create(CTRLID_PLAYER_SIXTEENSPLIT, x, y, width, height, this,xFocus++, 1);
	m_btnSixteenSplit.SetImage(PAINT_STATUS_NORMAL, BMP_BTN_PLAY_4X4, false);
	m_btnSixteenSplit.SetImage(PAINT_STATUS_FOCUS, BMP_BTN_PLAY_4X4_F, false);
	m_btnSixteenSplit.SetImage(PAINT_STATUS_DISABLE, BMP_BTN_PLAY_4X4_N, false);
	m_btnSixteenSplit.Show(false, false);
	x += 26;
	m_btn4X4Ex.Create(CTRLID_PLAYER_4X4EX, x, y+5, width, height, this, xFocus++, 1);
	m_btn4X4Ex.SetImage(PAINT_STATUS_NORMAL, BMP_BTN_PLAY_TRIANGLE, false);
	m_btn4X4Ex.SetImage(PAINT_STATUS_FOCUS, BMP_BTN_PLAY_TRIANGLE_F, false);
	m_btn4X4Ex.SetImage(PAINT_STATUS_DISABLE, BMP_BTN_PLAY_TRIANGLE, false);
	m_btn4X4Ex.Show(false, false);

	//所有的奇异按键
	if( m_maxSplitMode >= PLAY_CH_SIX )
	{
		x+=14;
		m_btnSpecSplit.Create(CTRLID_PLAYER_SPECSPLIT,x,y,width,height,this, xFocus++,1);
		m_btnSpecSplit.SetImage(PAINT_STATUS_NORMAL, BMP_BTN_POPSPLIT,false);
		m_btnSpecSplit.SetImage(PAINT_STATUS_FOCUS, BMP_BTN_POPSPLIT_F, false);
		m_btnSpecSplit.SetImage(PAINT_STATUS_DISABLE, BMP_BTN_POPSPLIT_N, false);
		m_btnSpecSplit.Show(false, false);
	}
	
	int maxLiveChnn = 0;

	//现场通道
	if(IsSupportLiPb(maxLiveChnn))
	{
		x += 30;
		m_btnLive.Create(CTRLID_PLAYER_LIVE, x, y, width, height, this, xFocus++, 1);
		m_btnLive.SetImage(PAINT_STATUS_NORMAL, BMP_BTN_PLAY_LIVE, false);
		m_btnLive.SetImage(PAINT_STATUS_FOCUS, BMP_BTN_PLAY_LIVE_F, false);
		m_btnLive.SetImage(PAINT_STATUS_DISABLE, BMP_BTN_PLAY_LIVE_N, false);
	}

	//按照最大通道处理分割按钮的显示与否
	if (m_bChangePlayChnn)
	{	
		if (m_maxSplitMode >= PLAY_CH_ONE)
		{
			m_btnOneSplit.Show(true, false);
			if (m_maxChnnNum > 1)
			{
				m_btn1X1Ex.Show(true, false);
			}
		}


		if(m_maxSplitMode >= PLAY_CH_FOUR)
		{
			m_btnFourSplit.Show(true, false);			
			if (m_maxChnnNum > 4)
			{
				m_btn2X2Ex.Show(true, false);
			}
		}


		if( m_maxSplitMode >= PLAY_CH_SIX )
		{
			m_btnSixSplit.Show(true,false);	
			m_btnSpecSplit.Show(true, false);
			if(m_maxChnnNum > 6)
			{
				m_btn2X3Ex.Show(true, false);
			}	
		}

		if(m_maxSplitMode >= PLAY_CH_NINE)
		{
			m_btnNineSplit.Show(true, false);
			if(m_maxChnnNum>9)
			{
				m_btn3X3Ex.Show(true, false);
			}
			
		}
		
		if(m_maxSplitMode >= PLAY_CH_SIXTEEN)
		{
			m_btnSixteenSplit.Show(true, false);
			
			if (m_maxChnnNum > 16)
			{
				m_btn4X4Ex.Show(true, false);
			}
		}
	}

	x += 40;
	m_btnZoom.Create(CTRLID_PLAYER_ZOOM, x, y, width, height, this,xFocus++, 1);
	m_btnZoom.SetImage(PAINT_STATUS_NORMAL, BMP_BTN_ZOOM, false);
	m_btnZoom.SetImage(PAINT_STATUS_FOCUS, BMP_BTN_ZOOM_FOCUS, false);
	m_btnZoom.SetImage(PAINT_STATUS_DISABLE, BMP_BTN_ZOOM_DISABLE, false);

	if(IsSupportChangePlayBackColor())
	{
		x +=30;
		m_btnColor.Create(CTRLID_PLAYER_COLOR, x, y, width, height, this, xFocus++, 1);
		m_btnColor.SetImage(PAINT_STATUS_NORMAL, BMP_BTN_SET_COLOR,false);
		m_btnColor.SetImage(PAINT_STATUS_FOCUS, BMP_BTN_SET_COLOR_F, false);
		m_btnColor.SetImage(PAINT_STATUS_DISABLE, BMP_BTN_SET_COLOR_N, false);
	}

	/*if (IsSupportSnapPic())
	{
	x += 40;
	m_btnSnap.Create(CTRLID_PLAYER_SNAP, x, y, width, height, this, 18, 1);
	m_btnSnap.SetImage(PAINT_STATUS_NORMAL, BMP_BTN_SNAP, false);
	m_btnSnap.SetImage(PAINT_STATUS_FOCUS, BMP_BTN_SNAP_F, false);
	m_btnSnap.SetImage(PAINT_STATUS_DISABLE, BMP_BTN_SNAP_N, false);
	}*/
	
#if defined(__NEW_CUT_BACKUP__)
	showNameFocus = xFocus + 3;

	if (800 < m_width)
	{
		x += 35;
		unsigned short cx = 0, cy = 0;
		GUI::CWnd::TextSize(m_pStrTable->String(E_STR_ID_CAMERA_NAME), strlen(m_pStrTable->String(E_STR_ID_CAMERA_NAME)), cx, cy);
		m_ckShowName.Create(GetRandWndID(), x, y, 2*m_length[CHECK_WIDTH]+cx, m_length[CHECK_HEIGHT], this, showNameFocus, 1);
		m_ckShowName.SetCaption(m_pStrTable->String(E_STR_ID_CAMERA_NAME), false, false);
	}
	//默认状态为start状态
	x+=m_length[BTN_WIDTH];	
	
	m_btnGetBackupStartTime.Create(CTRLID_PLAYER_BACKUP_START_TIME, x, y, width, height, this, xFocus++, 1);
	m_btnGetBackupStartTime.SetImage(PAINT_STATUS_NORMAL,BMP_BTN_CUTSTART,false);
	m_btnGetBackupStartTime.SetImage(PAINT_STATUS_FOCUS,BMP_BTN_CUTSTART_F,false);
	m_btnGetBackupStartTime.SetImage(PAINT_STATUS_DISABLE, BMP_BTN_CUTSTART_N,false);
	m_btnGetBackupStartTime.SetEnable(true,false);

	x+=m_length[BTN_WIDTH];
	m_btnGetBackupEndTime.Create(CTRLID_PLAYER_BACKUP_END_TIME, x, y, width, height, this, xFocus++, 1);
	m_btnGetBackupEndTime.SetImage(PAINT_STATUS_NORMAL,BMP_BTN_CUTEND,false);
	m_btnGetBackupEndTime.SetImage(PAINT_STATUS_FOCUS,BMP_BTN_CUTEND_F,false);
	m_btnGetBackupEndTime.SetImage(PAINT_STATUS_DISABLE, BMP_BTN_CUTEND_N,false);
	m_btnGetBackupEndTime.SetEnable(false,false);

	x+=m_length[BTN_WIDTH];
	m_btnbackup.Create(CTRLID_PLAYER_BACKUP, x, y, width, height, this, xFocus++, 1);
	m_btnbackup.SetImage(PAINT_STATUS_NORMAL,BMP_BTN_BACKUPCOPY,false);
	m_btnbackup.SetImage(PAINT_STATUS_FOCUS,BMP_BTN_BACKUPCOPY_F,false);
	m_btnbackup.SetImage(PAINT_STATUS_DISABLE, BMP_BTN_BACKUPCOPY_N,false);

#if !defined(__ASYN_PLAY__)
	m_btnbackup.SetEnable(true,false);
#endif
#else
	showNameFocus = xFocus + 2;

	if (800 < m_width)
	{
		x += 35;
		unsigned short cx = 0, cy = 0;
		GUI::CWnd::TextSize(m_pStrTable->String(E_STR_ID_CAMERA_NAME), strlen(m_pStrTable->String(E_STR_ID_CAMERA_NAME)), cx, cy);
		m_ckShowName.Create(GetRandWndID(), x, y, 2*m_length[CHECK_WIDTH]+cx, m_length[CHECK_HEIGHT], this, showNameFocus, 1);
		m_ckShowName.SetCaption(m_pStrTable->String(E_STR_ID_CAMERA_NAME), false, false);
	}
	//默认状态为start状态
	x+=m_length[BTN_WIDTH];
	
	m_btnGetBackupTime.Create(CTRLID_PLAYER_BACKUP_SELECT_TIME, x, y, width, height, this, xFocus++, 1);
	m_btnGetBackupTime.SetStateImage(CUTSTART, PAINT_STATUS_NORMAL,BMP_BTN_CUTSTART);
	m_btnGetBackupTime.SetStateImage(CUTSTART, PAINT_STATUS_FOCUS,BMP_BTN_CUTSTART_F);
	m_btnGetBackupTime.SetStateImage(CUTSTART, PAINT_STATUS_DISABLE, BMP_BTN_CUTSTART_N);
	m_btnGetBackupTime.SetStateImage(CUTEND, PAINT_STATUS_NORMAL,BMP_BTN_CUTEND);
	m_btnGetBackupTime.SetStateImage(CUTEND, PAINT_STATUS_FOCUS,BMP_BTN_CUTEND_F);
	m_btnGetBackupTime.SetStateImage(CUTEND, PAINT_STATUS_DISABLE,BMP_BTN_CUTEND_N);
	m_btnGetBackupTime.SetEnable(true,false);

	x+=m_length[BTN_WIDTH];
	m_btnbackup.Create(CTRLID_PLAYER_BACKUP, x, y, width, height, this, xFocus++, 1);
	m_btnbackup.SetImage(PAINT_STATUS_NORMAL,BMP_BTN_BACKUPCOPY,false);
	m_btnbackup.SetImage(PAINT_STATUS_FOCUS,BMP_BTN_BACKUPCOPY_F,false);
	m_btnbackup.SetImage(PAINT_STATUS_DISABLE, BMP_BTN_BACKUPCOPY_N,false);

	m_btnbackup.SetEnable(false,false);
	m_btnGetBackupTime.SetCurState(CUTSTART, false);
#endif

	xFocus++;
	//end
	//////////////////////////////////////////////////////////////////////////
	x = m_rect.Width() - width - 10;
	if (800 > m_width)
	{
		x -= 40; 
	}

	width = 30;
	int nIndexTmp = xFocus + 3;

	m_btnMove.Create(CTRLID_PLAYER_MOVE, x, y, width, height, this, nIndexTmp--, 1);
	m_btnMove.SetImage(PAINT_STATUS_NORMAL, BMP_BTN_PLAY_MOVE, false);
	m_btnMove.SetImage(PAINT_STATUS_FOCUS, BMP_BTN_PLAY_MOVE_FOCUS, false);
	m_btnMove.SetImage(PAINT_STATUS_DISABLE, BMP_BTN_PLAY_MOVE, false);
	
	x -= (width + 5);
	m_btnExit.Create(CTRLID_PLAYER_EXIT, x, y, width, height, this, nIndexTmp--, 1);
	m_btnExit.SetImage(PAINT_STATUS_NORMAL, BMP_BTN_EXIT2, false);
	m_btnExit.SetImage(PAINT_STATUS_FOCUS, BMP_BTN_EXIT2_FOCUS, false);
	m_btnExit.SetImage(PAINT_STATUS_DISABLE, BMP_BTN_EXIT2, false);

	x -= (width + 5);
	m_btnHide.Create(CTRLID_PLAYER_HIDE, x, y, width, height, this, nIndexTmp--, 1);
	m_btnHide.SetImage(PAINT_STATUS_NORMAL, BMP_BTN_PLAY_HIDE, false);
	m_btnHide.SetImage(PAINT_STATUS_FOCUS, BMP_BTN_PLAY_HIDE_FOCUS, false);
	m_btnHide.SetImage(PAINT_STATUS_DISABLE, BMP_BTN_PLAY_HIDE, false);

	x -= (width + 5);
	m_btnAudio.Create(CTRLID_PLAYER_AUDIO,x,y,width,height,this,nIndexTmp--,1);
	//状态AUDIO_OPEN,有声音; 状态AUDIO_MUTE,无声音
	m_btnAudio.SetStateImage(AUDIO_OPEN, PAINT_STATUS_NORMAL, BMP_BTN_PLAY_AUDIO);
	m_btnAudio.SetStateImage(AUDIO_OPEN, PAINT_STATUS_FOCUS, BMP_BTN_PLAY_AUDIO_F);
	m_btnAudio.SetStateImage(AUDIO_OPEN, PAINT_STATUS_DISABLE, BMP_BTN_PLAY_AUDIO_N);
	m_btnAudio.SetStateImage(AUDIO_MUTE, PAINT_STATUS_NORMAL, BMP_BTN_PLAY_AUDIO_M);
	m_btnAudio.SetStateImage(AUDIO_MUTE, PAINT_STATUS_FOCUS, BMP_BTN_PLAY_AUDIO_M_F);
	m_btnAudio.SetStateImage(AUDIO_MUTE, PAINT_STATUS_DISABLE, BMP_BTN_PLAY_AUDIO_N);
	
	xFocus += 4;
	//////////////////////////////////////////////////////////////////////////
	x = 3;

	if (800 > m_width)
	{
		x += 40; 
	}
	if(m_width<=800)
	{
		//开始时间
		y += height + 3;
		const char* strTextStart = "8888/88/88 88:88:88";
		unsigned short strCxStart = 0, strCyStart = 20;
		GUI::CWnd::TextSize(strTextStart, strlen(strTextStart), strCxStart, strCyStart);

		x+=2;
		width = strCxStart + 4;
		height = m_length[STATIC_HEIGHT];
		m_stSecStartTime.Create(GetRandWndID(), x, y, width, m_length[STATIC_HEIGHT],this);
		m_stSecStartTime.SetBKColor(COLOR_DLG_CHILD_BK);

		x+=width;
		width = 24;
		height = 24;
		m_btnPreSection.Create(CTRLID_PLAYER_PRE_SECTION, x, y, width, height, this, 0, 2);
		m_btnPreSection.SetImage(PAINT_STATUS_NORMAL, BMP_BTN_REWARD, false);
		m_btnPreSection.SetImage(PAINT_STATUS_FOCUS, BMP_BTN_REWARD_FOCUS, false);
		m_btnPreSection.SetImage(PAINT_STATUS_DISABLE, BMP_BTN_REWARD, false);

		const char* strTextEnd = "88:88:88";
		unsigned short strCxEnd = 0, strCyEnd = 20;
		GUI::CWnd::TextSize(strTextEnd, strlen(strTextEnd), strCxEnd, strCyEnd);
		x+=width;
		unsigned long sliderWidth=m_width-x-width*2-strCxEnd;
		m_sliderCtrl.Create(GetRandWndID(), x, y, sliderWidth, this,1, 2);
		m_sliderCtrl.SetCurPos(0,false);

		x += sliderWidth + 3;
		width = 24;
		height = 24;
		m_btnNextSection.Create(CTRLID_PLAYER_NEXT_SECTION, x, y, width, height, this, 2, 2);
		m_btnNextSection.SetImage(PAINT_STATUS_NORMAL, BMP_BTN_FAST_FORWARD, false);
		m_btnNextSection.SetImage(PAINT_STATUS_FOCUS, BMP_BTN_FAST_FORWARD_FOCUS, false);
		m_btnNextSection.SetImage(PAINT_STATUS_DISABLE, BMP_BTN_FAST_FORWARD, false);

		//末尾时间
		x+=width;
		m_stSecEndTime.Create(GetRandWndID(), x, y, strCxEnd, m_length[STATIC_HEIGHT], this);
		m_stSecEndTime.SetBKColor(COLOR_DLG_CHILD_BK);

		//下一段
		y += height-3;
		width = 70;	
		x=5;
		m_scTime.Create(GetRandWndID(), x, y, width, m_length[STATIC_HEIGHT], this);
		m_scTime.SetBKColor(COLOR_DLG_CHILD_BK);
		m_scTime.Show(false,false);

		x = x + width + 10;
		width = 70;
		m_stPlayStatus.Create(GetRandWndID(), x, y, width, m_length[STATIC_HEIGHT], this);
		m_stPlayStatus.SetBKColor(COLOR_DLG_CHILD_BK);

		x =x+24;
		const char* str = "88:88:88 PM-88:88:88 PM";
		unsigned short strCx = 0, strCy = 20;
		GUI::CWnd::TextSize(str, strlen(str), strCx, strCy);
		
		m_tip.Create(GetRandWndID(), x, y, strCx, m_length[STATIC_HEIGHT], this);
		m_tip.SetBKColor(COLOR_DLG_CHILD_BK);
		m_tip.Show(true,false);

		unsigned short cx = 0, cy = 0;
		GUI::CWnd::TextSize(m_pStrTable->String(E_STR_ID_CAMERA_NAME), strlen(m_pStrTable->String(E_STR_ID_CAMERA_NAME)), cx, cy);
		m_ckShowName.Create(GetRandWndID(), x+strCx+10, y, 2*m_length[CHECK_WIDTH]+cx, m_length[CHECK_HEIGHT], this, showNameFocus, 2);
		m_ckShowName.SetCaption(m_pStrTable->String(E_STR_ID_CAMERA_NAME), false, false);

#if defined(__ASYN_PLAY__)
		GUI::CWnd::TextSize("Async Play", strlen("Async Play"), cx, cy);
		x += strCx+10 + cx + 2*m_length[CHECK_WIDTH]+20;
		m_ckSyncPlayback.Create(GetRandWndID(), x, y, 2*m_length[CHECK_WIDTH]+cx, m_length[CHECK_HEIGHT], this, showNameFocus+1, 1);
		m_ckSyncPlayback.SetCaption("Async Play", false);
		m_ckSyncPlayback.SetCheck(false, false);
		m_ckSyncPlayback.SetEnable(true, false);
#endif
	}
	else
	{

		y += height + 3;
		width = 24;
		height = 24;
		m_btnPreSection.Create(CTRLID_PLAYER_PRE_SECTION, x, y, width, height, this, 0, 2);
		m_btnPreSection.SetImage(PAINT_STATUS_NORMAL, BMP_BTN_REWARD, false);
		m_btnPreSection.SetImage(PAINT_STATUS_FOCUS, BMP_BTN_REWARD_FOCUS, false);
		m_btnPreSection.SetImage(PAINT_STATUS_DISABLE, BMP_BTN_REWARD, false);

		x += width + 3;
		width = m_width - (x*2);
		
		m_sliderCtrl.Create(GetRandWndID(), x, y, width, this,1, 2);

		x += width + 3;
		width = 24;
		height = 24;
		m_btnNextSection.Create(CTRLID_PLAYER_NEXT_SECTION, x, y, width, height, this, 2, 2);
		m_btnNextSection.SetImage(PAINT_STATUS_NORMAL, BMP_BTN_FAST_FORWARD, false);
		m_btnNextSection.SetImage(PAINT_STATUS_FOCUS, BMP_BTN_FAST_FORWARD_FOCUS, false);
		m_btnNextSection.SetImage(PAINT_STATUS_DISABLE, BMP_BTN_FAST_FORWARD, false);

		x = 3;
		if (800 > m_width)
		{
			x += 40; 
		}

		const char* strText = "8888/88/88 88:88:88";
		unsigned short strCx = 0, strCy = 20;
		GUI::CWnd::TextSize(strText, strlen(strText), strCx, strCy);

		y += height;
		width = strCx + 4;
		height = m_length[STATIC_HEIGHT];
		m_stSecStartTime.Create(GetRandWndID(), x, y, width, m_length[STATIC_HEIGHT],this);
		m_stSecStartTime.SetBKColor(COLOR_DLG_CHILD_BK);

		width = 70;	
		x = m_width/2 - width;
		m_scTime.Create(GetRandWndID(), x, y, width, m_length[STATIC_HEIGHT], this);
		m_scTime.SetBKColor(COLOR_DLG_CHILD_BK);
		m_scTime.Show(false,false);

		if (m_width < 800)
		{
			x = x + 70;
		}
		else
		{
			x = x + width + 10;
		}
		width = 70;
		m_stPlayStatus.Create(GetRandWndID(), x, y, width, m_length[STATIC_HEIGHT], this);
		m_stPlayStatus.SetBKColor(COLOR_DLG_CHILD_BK);


		width = 70;
		x = m_width - 3 - width;
		if (800 > m_width)
		{
			x -= 40; 
		}
		m_stSecEndTime.Create(GetRandWndID(), x, y, width, m_length[STATIC_HEIGHT], this);
		m_stSecEndTime.SetBKColor(COLOR_DLG_CHILD_BK);

		x=m_width/2+2*width;
		const char* str = "88:88:88 PM-88:88:88 PM";
		strCx = 0, strCy = 20;
		GUI::CWnd::TextSize(str, strlen(str), strCx, strCy);
		
		m_tip.Create(GetRandWndID(), x, y, strCx, m_length[STATIC_HEIGHT], this);
		m_tip.SetBKColor(COLOR_DLG_CHILD_BK);
		m_tip.Show(true,false);

#if defined(__ASYN_PLAY__)
		unsigned short cx = 0, cy = 0;
		x += strCx + 2*m_length[BTN_WIDTH];
		GUI::CWnd::TextSize("Async Play", strlen("Async Play"), cx, cy);
		m_ckSyncPlayback.Create(GetRandWndID(), x, y, 2*m_length[CHECK_WIDTH]+cx, m_length[CHECK_HEIGHT], this, xFocus++, 1);
		m_ckSyncPlayback.SetCaption("Async Play", false);
		m_ckSyncPlayback.SetCheck(false, false);
		m_ckSyncPlayback.SetEnable(true, false);
#endif
		  
	}
		
		//////////////////////////////////////////////////////////////////////////
		//设置菜单项
		int item  = 0;
		POS pos = m_pCtrlSpeedForwardList->GetHeadPosition();
		while (pos != NULL)
		{
			PLAYER_CTRL_SPEED_ITEM &speeditem = m_pCtrlSpeedForwardList->GetAt(pos);
			item = m_menuForward.AddItem(speeditem.szSpeedName);
			m_menuForward.SetItemData(item, speeditem.speed);
			m_pCtrlSpeedForwardList->GetNext(pos);
		}

		pos = m_pCtrlSpeedRewardList->GetHeadPosition();
		while (pos != NULL)
		{
			PLAYER_CTRL_SPEED_ITEM &speeditem = m_pCtrlSpeedRewardList->GetAt(pos);
			item = m_menuReward.AddItem(speeditem.szSpeedName);
			m_menuReward.SetItemData(item, speeditem.speed);
			m_pCtrlSpeedRewardList->GetNext(pos);
		}

		///////////////////////////////////////////////////////////////////////////
		SetAudioCH();
		PlayStatus(false);
#if defined(__NEW_CUT_BACKUP__)
		//读取/mnt/mtd/cutKeeplog.log 里面的记录
		CCutKeepLog cutKeepLog;		
		if (cutKeepLog.OpenKeepFile())
		{
			cutKeepLog.ReadKeepFile(&m_searchDataVector);			
			cutKeepLog.Quit();
		}	
#endif
#if defined(__ASYN_PLAY__)
		if(m_bSync)
		{
			m_btnbackup.SetEnable(true,false);
		}
		else
		{
			m_btnbackup.SetEnable(false,false);
		}
#endif
}

void CPlayerDlg::OnPostInitial()
{
	GUI::CDialog::OnPostInitial();

	////////////////////////////////////////////////////
	{
		{
			m_x_enter = 0; m_y_enter = 0;
			m_x_drag_last = 0, m_y_drag_last = 0;
			m_x_pos = 0, m_y_pos = 0;
			m_x_pos_last = 0, m_y_pos_last = 0;

			m_bFirstDrag = false;
			m_bDrag = false;
		}

		//初始化布局信息
		m_btnCount = 0;
		memset(&m_btnWndInfo, 0, sizeof(m_btnWndInfo));
		memset(&m_wndLayout, 0, sizeof(BTN_WND_LAYOUT));
		m_bCvbs = (800 > m_width) ? true : false;

		if(m_bCvbs)
		{
			m_wndLayout.playToolRect.SetRect(40, 40, m_width - 40 - 1, m_height - 40 - 1);
		}
		else
		{
			m_wndLayout.playToolRect.SetRect(0, 0, m_width - 1, m_height - 1);	
		}

		unsigned short width = 24;
		unsigned short height = 24;
		unsigned short width_triangle = 10;

		unsigned short offset_x = 4;

		//
		SetBtnWndInfo(&m_btnPlayPause, m_btnCount++, width + offset_x, height, 0);
		SetBtnWndInfo(&m_btnStop, m_btnCount++, width + offset_x, height, 0);
		SetBtnWndInfo(&m_btnNextFrame, m_btnCount++, width + offset_x, height, 0);
		SetBtnWndInfo(&m_btnFastForward, m_btnCount++, width + offset_x, height, 0);
		SetBtnWndInfo(&m_btnFFEx, m_btnCount++, width_triangle + offset_x, height, 3);
		SetBtnWndInfo(&m_btnReward, m_btnCount++, width + offset_x, height, 0);
		SetBtnWndInfo(&m_btnREWEx, m_btnCount++, width_triangle + offset_x, height, 3);
		
		//通道
		if (m_bChangePlayChnn)
		{	
			if (m_maxSplitMode >= PLAY_CH_ONE)
			{
				SetBtnWndInfo(&m_btnOneSplit, m_btnCount++, width + offset_x, height, 0);
				//m_btnOneSplit.Show(true, false);
				if (m_maxChnnNum > 1)
				{
					SetBtnWndInfo(&m_btn1X1Ex, m_btnCount++, width_triangle + offset_x, height, 3);
					//m_btn1X1Ex.Show(true, false);
				}
			}


			if(m_maxSplitMode >= PLAY_CH_FOUR)
			{
				SetBtnWndInfo(&m_btnFourSplit, m_btnCount++, width + offset_x, height, 0);
				
				//m_btnFourSplit.Show(true, false);			
				if (m_maxChnnNum > 4)
				{
					SetBtnWndInfo(&m_btn2X2Ex, m_btnCount++, width_triangle + offset_x, height, 3);
					//m_btn2X2Ex.Show(true, false);
				}
				
			}


			if( m_maxSplitMode >= PLAY_CH_SIX )
			{
				SetBtnWndInfo(&m_btnSixSplit, m_btnCount++, width + offset_x, height, 0);
				if(m_maxChnnNum > 6)
				{
					SetBtnWndInfo(&m_btn2X3Ex, m_btnCount++, width_triangle + offset_x, height, 3);
				}		
				
			}

			if(m_maxSplitMode >= PLAY_CH_NINE)
			{
				SetBtnWndInfo(&m_btnNineSplit, m_btnCount++, width + offset_x, height, 0);
				//m_btnNineSplit.Show(true, false);	
				if (m_maxChnnNum > 9)
				{
					SetBtnWndInfo(&m_btn3X3Ex, m_btnCount++, width_triangle + offset_x, height, 3);
					//m_btn3X3Ex.Show(true, false);
				}		
			}

			if(m_maxSplitMode >= PLAY_CH_SIXTEEN)
			{
				SetBtnWndInfo(&m_btnSixteenSplit, m_btnCount++, width + offset_x, height, 0);
				//m_btnSixteenSplit.Show(true, false);
				if(m_maxChnnNum > 16)
				{
					SetBtnWndInfo(&m_btn4X4Ex, m_btnCount++, width_triangle + offset_x, height, 3);
				}
				
			}
		}
		if(m_maxSplitMode >= PLAY_CH_SIX )
		{
			SetBtnWndInfo(&m_btnSpecSplit, m_btnCount++, width + offset_x, height, 0);
		}
		
		int maxLiveChnn = 0;
		if(IsSupportLiPb(maxLiveChnn))
		{
			SetBtnWndInfo(&m_btnLive, m_btnCount++, width + offset_x, height, 0);
		}
	
		SetBtnWndInfo(&m_btnZoom, m_btnCount++, width + offset_x, height, 0);
		if(IsSupportChangePlayBackColor())
		{
			SetBtnWndInfo(&m_btnColor, m_btnCount++, width + offset_x, height, 0);
		}

#if defined(__NEW_CUT_BACKUP__)
		SetBtnWndInfo(&m_btnGetBackupStartTime, m_btnCount++, width + offset_x, height, 0);
		SetBtnWndInfo(&m_btnGetBackupEndTime, m_btnCount++, width + offset_x, height, 0);
		SetBtnWndInfo(&m_btnbackup, m_btnCount++, width + offset_x, height, 0);
#else
		SetBtnWndInfo(&m_btnGetBackupTime, m_btnCount++, width + offset_x, height, 0);
		SetBtnWndInfo(&m_btnbackup, m_btnCount++, width + offset_x, height, 0);
#endif
		unsigned short width_exit_btn = 30;
		unsigned short height_exit_btn = 25;
		unsigned short require_width = 0;

		//音频,最小化,关闭,移动
		m_wndLayout.rightFixWidth = 8 + (width_exit_btn + 3) * 4;

		for(int i = 0; i < m_btnCount; i++)
		{
			require_width += m_btnWndInfo[i].width;
		}
		m_wndLayout.btnMaxWidth = require_width;
		m_wndLayout.maxRow = 2;

		m_wndLayout.minWidth = m_wndLayout.btnMaxWidth + m_wndLayout.rightFixWidth;
		m_wndLayout.minHeight = height * 2 + m_length[STATIC_HEIGHT] + 3 * 2 + 8;

		//背景图大小
		CWnd::GetImageSize(BMP_BTN_PLAY_BG, m_bmpBgWidth, m_bmpBgHeight);

		//移动按钮中心距离顶和右边界距离
		m_wndLayout.offsetTop = 5 + width / 2;
		m_wndLayout.offsetRight = 5 + height / 2;

		AutoLayout(false, m_xPosLeft, m_yPosTop);
	}
}

void CPlayerDlg::OnPaint(PAINT_STATUS status)
{
#if !defined(__NEW_CUT_BACKUP__)
	if(m_chnn!=m_selCuPlayrChnn)
	{
		//切换的话，备份变灰
		m_backCnt=0;
		m_endBackTime=0;
		m_startBackTime=0;
		m_chnn=0;
		m_btnGetBackupTime.SetCurState(CUTSTART,false);
		m_btnbackup.SetEnable(false,true);
		string stm1;
		stm1.clear();
		m_tip.SetCaption(stm1,false);
		m_tip.Show(false,false);
		
	}
#endif

	CDrawManager drawMan;
	const unsigned char* pData = drawMan.Stretch(BMP_BTN_PLAY_BG, m_rect.Width(), m_rect.Height(), m_bmpBgWidth / 2, m_bmpBgHeight / 2, NULL, true);

	int len = drawMan.GetDataLen();
	if (NULL != pData)
	{
		GUI::CWnd::DrawImage(pData, len, m_rect.m_left, m_rect.m_top, m_rect.Width(), m_rect.Width(), m_rect.Height());
	}
	
}

void CPlayerDlg::OnDestroy()
{
	GUI::CDialog::OnDestroy();

	m_menuForward.DeleteAllItem();
	m_menuReward.DeleteAllItem();

	m_pCtrlSpeedForwardList = NULL;
	m_pCtrlSpeedRewardList  = NULL;
#if !defined(__ASYN_PLAY__)
	m_currStatus = PLAYBACK_STOP;
#else
	for (int i = 0; i < g_p_login_succ->productInfo.localVideoInputNum; i++)
	{
		m_curForwardSpeed[i]       = PLAY_SPEED_1X;
		m_curRewardSpeed[i]        = PLAY_SPEED_1X;
		m_currStatus[i]	    	   = PLAYBACK_STOP;
	}
#endif
#if defined(__NEW_CUT_BACKUP__)
	m_searchDataVector.clear();
#endif
}

void CPlayerDlg::StopStatus()
{
	m_btnPlayPause.SetImage(PAINT_STATUS_NORMAL, BMP_BTN_PLAY, false);
	m_btnPlayPause.SetImage(PAINT_STATUS_FOCUS, BMP_BTN_PLAY_FOCUS, false);
	m_btnPlayPause.SetImage(PAINT_STATUS_DISABLE, BMP_BTN_PLAY_DISABLE, false);

#if defined(__ASYN_PLAY__)
	m_ckSyncPlayback.SetEnable(true, false);
#endif
	m_btnPlayPause.SetEnable(true, false);
	m_btnStop.SetEnable(false, false);
	m_btnNextFrame.SetEnable(false, false);

	m_btnFastForward.SetEnable(false, false);
	m_btnReward.SetEnable(false, false);
	m_btnFFEx.SetEnable(false, false);
	m_btnREWEx.SetEnable(false, false);

	SetSplitBtnEnable(false, false);

	m_btnLive.SetEnable(false, false);
	
#if !defined(__ASYN_PLAY__)
	m_curForwardSpeed       = PLAY_SPEED_1X;
	m_curRewardSpeed        = PLAY_SPEED_1X;
#else
	for (int i = 0; i < g_p_login_succ->productInfo.localVideoInputNum; i++)
	{
		m_curForwardSpeed[i]       = PLAY_SPEED_1X;
		m_curRewardSpeed[i]        = PLAY_SPEED_1X;
		m_currStatus[i]	    	   = PLAYBACK_STOP;
	}
#endif

	m_sliderCtrl.SetCurPos(0, false);

	m_stPlayStatus.SetCaption("", false);	
	m_ckShowName.SetCheck(false, false);
	OnShowName();
	m_tip.Show(false,false);
#if !defined(__ASYN_PLAY__)
	m_currStatus = PLAYBACK_STOP;
#endif
	//zxx新加
#if defined(__NEW_CUT_BACKUP__)
	CleanCutBackup();
#else
	m_backCnt=0;
	m_endBackTime=0;
	m_startBackTime=0;
	m_chnn=0;
	m_btnGetBackupTime.SetCurState(CUTSTART,false);
	m_btnGetBackupTime.SetEnable(false,false);
	m_btnbackup.SetEnable(false,false);
#endif
	Repaint();
}

void CPlayerDlg::PlayStatus(bool bRefresh/* = true*/)
{
	m_btnPlayPause.SetImage(PAINT_STATUS_NORMAL, BMP_BTN_PAUSE, false);
	m_btnPlayPause.SetImage(PAINT_STATUS_FOCUS, BMP_BTN_PAUSE_FOCUS, false);
	m_btnPlayPause.SetImage(PAINT_STATUS_DISABLE, BMP_BTN_PAUSE_DISABLE, false);

#if defined(__ASYN_PLAY__)
	m_ckSyncPlayback.SetEnable(false, false);
#endif
	m_btnPlayPause.SetEnable(true, false);
	m_btnStop.SetEnable(true, false);
	m_btnNextFrame.SetEnable(false, false);

	m_btnFastForward.SetEnable(true, false);
	m_btnReward.SetEnable(true, false);
	m_btnFFEx.SetEnable(true, false);
	m_btnREWEx.SetEnable(true, false);

#if !defined(__ASYN_PLAY__)
	SetSplitBtnEnable(true, false);

	m_btnLive.SetEnable(true, false);

	m_curForwardSpeed       = PLAY_SPEED_1X;
	m_curRewardSpeed        = PLAY_SPEED_1X;
#else
	if (m_bSync)
	{
		SetSplitBtnEnable(true, false);
		m_btnLive.SetEnable(true, false);
		m_btnGetBackupStartTime.SetEnable(true, false);
		m_btnGetBackupEndTime.SetEnable(true, false);		
		for (int i = 0; i < g_p_login_succ->productInfo.localVideoInputNum; i++)
		{
			m_curForwardSpeed[i]       = PLAY_SPEED_1X;
			m_curRewardSpeed[i]        = PLAY_SPEED_1X;
			m_currStatus[i]	    	   = PLAYBACK_PLAYING;
		}
	}
	else
	{
		SetSplitBtnEnable(false, false);
		m_btnZoom.SetEnable(false, false);
		m_btnLive.SetEnable(false, false);
		m_btnGetBackupStartTime.SetEnable(false, false);
		m_btnGetBackupEndTime.SetEnable(false, false);		
		m_curForwardSpeed[m_curRedSelectChnn]       = PLAY_SPEED_1X;
		m_curRewardSpeed[m_curRedSelectChnn]        = PLAY_SPEED_1X;
		m_currStatus[m_curRedSelectChnn]	    	   = PLAYBACK_PLAYING;
	}
#endif
	m_stPlayStatus.SetCaption(">>1X", false);

#if !defined(__ASYN_PLAY__)
	m_currStatus = PLAYBACK_PLAYING;
#endif
	if(bRefresh)
	{
		Repaint();
	}


}

void CPlayerDlg::PauseStatus()
{
	m_btnPlayPause.SetImage(PAINT_STATUS_NORMAL, BMP_BTN_PLAY, false);
	m_btnPlayPause.SetImage(PAINT_STATUS_FOCUS, BMP_BTN_PLAY_FOCUS, false);
	m_btnPlayPause.SetImage(PAINT_STATUS_DISABLE, BMP_BTN_PLAY_DISABLE, false);

#if defined(__ASYN_PLAY__)
	m_ckSyncPlayback.SetEnable(false, false);
#endif
	m_btnPlayPause.SetEnable(true, false);
	m_btnStop.SetEnable(true, false);
	m_btnNextFrame.SetEnable(true, false);

	m_btnFastForward.SetEnable(false, false);
	m_btnReward.SetEnable(false, false);
	m_btnFFEx.SetEnable(false, false);
	m_btnREWEx.SetEnable(false, false);

	SetSplitBtnEnable(false, false);

	m_btnLive.SetEnable(false, false);

	m_stPlayStatus.SetCaption("||");

#if !defined(__ASYN_PLAY__)
	m_currStatus = PLAYBACK_PAUSE;
#else
	if (m_bSync)
	{
		for (int i = 0; i < g_p_login_succ->productInfo.localVideoInputNum; i++)
		{
			m_curForwardSpeed[i]       = PLAY_SPEED_NONE;
			m_curRewardSpeed[i]        = PLAY_SPEED_NONE;
			m_currStatus[i]	    	   = PLAYBACK_PAUSE;
		}
	}
	else
	{
		m_curForwardSpeed[m_curRedSelectChnn]       = PLAY_SPEED_NONE;
		m_curRewardSpeed[m_curRedSelectChnn]        = PLAY_SPEED_NONE;
		m_currStatus[m_curRedSelectChnn] = PLAYBACK_PAUSE;
	}
#endif

	Repaint();
}


void CPlayerDlg::NextFrameStatus()
{
	m_btnPlayPause.SetImage(PAINT_STATUS_NORMAL, BMP_BTN_PLAY, false);
	m_btnPlayPause.SetImage(PAINT_STATUS_FOCUS, BMP_BTN_PLAY_FOCUS, false);
	m_btnPlayPause.SetImage(PAINT_STATUS_DISABLE, BMP_BTN_PLAY_DISABLE, false);

	m_btnPlayPause.SetEnable(true, false);
	m_btnStop.SetEnable(true, false);
	m_btnNextFrame.SetEnable(true, false);

	m_btnFastForward.SetEnable(false, false);
	m_btnReward.SetEnable(false, false);
	m_btnFFEx.SetEnable(false, false);
	m_btnREWEx.SetEnable(false, false);

	SetSplitBtnEnable(false, false);

	m_btnLive.SetEnable(false, false);

	Repaint();
}

void CPlayerDlg::FastForwardStatus()
{
	char str[16] = {0};

	m_btnPlayPause.SetImage(PAINT_STATUS_NORMAL, BMP_BTN_PAUSE, false);
	m_btnPlayPause.SetImage(PAINT_STATUS_FOCUS, BMP_BTN_PAUSE_FOCUS, false);
	m_btnPlayPause.SetImage(PAINT_STATUS_DISABLE, BMP_BTN_PAUSE_DISABLE, false);

	m_btnPlayPause.SetEnable(true);
	m_btnStop.SetEnable(true);
	m_btnNextFrame.SetEnable(false);

	m_btnFastForward.SetEnable(true);
	m_btnReward.SetEnable(false);
	m_btnFFEx.SetEnable(true);
	m_btnREWEx.SetEnable(false);

	SetSplitBtnEnable(false);

#if !defined(__ASYN_PLAY__)
	FindPlayStausName(true, m_curForwardSpeed, str);
	m_stPlayStatus.SetCaption(str, false);

	m_btnLive.SetEnable(false, false);

	m_currStatus = PLAYBACK_FF;
#else	
	if(m_bSync)
	{
		for (int i = 0; i < g_p_login_succ->productInfo.localVideoInputNum; i++)
		{
			memset(str,0,sizeof(str));
			FindPlayStausName(true, m_curForwardSpeed[i], str);
			m_stPlayStatus.SetCaption(str, false);
			m_currStatus[i] = PLAYBACK_FF;
		}
	}
	else
	{
		FindPlayStausName(true, m_curForwardSpeed[m_curRedSelectChnn], str);
		m_stPlayStatus.SetCaption(str, false);
		m_currStatus[m_curRedSelectChnn] = PLAYBACK_FF;
	}
	m_btnLive.SetEnable(false, false);
#endif

	Repaint();
}

void CPlayerDlg::RewardStatus()
{
	char str[16] = {0};

	m_btnPlayPause.SetImage(PAINT_STATUS_NORMAL, BMP_BTN_PAUSE, false);
	m_btnPlayPause.SetImage(PAINT_STATUS_FOCUS, BMP_BTN_PAUSE_FOCUS, false);
	m_btnPlayPause.SetImage(PAINT_STATUS_DISABLE, BMP_BTN_PAUSE_DISABLE, false);

	m_btnPlayPause.SetEnable(true, false);
	m_btnStop.SetEnable(true, false);
	m_btnNextFrame.SetEnable(false, false);

	m_btnFastForward.SetEnable(false, false);
	m_btnReward.SetEnable(true, false);
	m_btnFFEx.SetEnable(false, false);
	m_btnREWEx.SetEnable(true, false);

	SetSplitBtnEnable(false, false);

	m_btnLive.SetEnable(false, false);

#if !defined(__ASYN_PLAY__)
	FindPlayStausName(false, m_curRewardSpeed, str);
	m_stPlayStatus.SetCaption(str, false);

	m_currStatus = PLAYBACK_REW;
#else
	if(m_bSync)
	{
		for (int i = 0; i < g_p_login_succ->productInfo.localVideoInputNum; i++)
		{
			FindPlayStausName(false, m_curRewardSpeed[i], str);
			m_stPlayStatus.SetCaption(str, false);
			m_currStatus[i] = PLAYBACK_REW;
		}
	}
	else
	{
		FindPlayStausName(false, m_curRewardSpeed[m_curRedSelectChnn], str);
		m_stPlayStatus.SetCaption(str, false);
		m_currStatus[m_curRedSelectChnn] = PLAYBACK_REW;
	}
#endif

	Repaint();
}

bool CPlayerDlg::OnClickSelChnn(GUI::CButton & btn, PLAY_CH_MODE splitMode, int splitChnn, ULONGLONG & selCH)
{
	if ( m_bChangePlayChnn && (m_maxSplitMode >= splitMode))
	{
		unsigned long ipcMaxVideoOutNum = CProduct::Instance()->IPCMaxVideoOutNum();
		if( g_p_login_succ->productInfo.localVideoInputNum && ipcMaxVideoOutNum )
		{
			if(((PLAY_CH_ONE == splitMode) && (0 < ipcMaxVideoOutNum)) || ((PLAY_CH_FOUR == splitMode) && (4 <= ipcMaxVideoOutNum)))
			{
				m_selChnnDlg.SetChnnCount(m_maxChnnNum, g_p_login_succ->authPlaybackCH, min(splitChnn, m_maxSelChnn), selCH);
			}
			else
			{
				m_selChnnDlg.SetChnnCount(g_p_login_succ->productInfo.localVideoInputNum, g_p_login_succ->authPlaybackCH, min(splitChnn, m_maxSelChnn), selCH);
			}
		}
		else
		{
			if(splitChnn <= ipcMaxVideoOutNum)
			{
				m_selChnnDlg.SetChnnCount(m_maxChnnNum, g_p_login_succ->authPlaybackCH, min(splitChnn, m_maxSelChnn), selCH);
			}
			else
			{
				m_selChnnDlg.SetChnnCount(g_p_login_succ->productInfo.localVideoInputNum, g_p_login_succ->authPlaybackCH, min(splitChnn, m_maxSelChnn), selCH);
			}
		}

		GUI::CRect rcBtn = btn.GetWindowRect();
		GUI::CRect rcWnd = GetWindowRect();
		GUI::CRect rcSel = m_selChnnDlg.GetDlgDefaultRect();
		int x = (rcBtn.m_left > m_wndLayout.playToolRect.m_left + (rcSel.Width() - rcBtn.Width()) / 2)  ? (rcBtn.m_left - (rcSel.Width() - rcBtn.Width()) / 2) : rcBtn.m_left;
		if(x + rcSel.Width() > m_wndLayout.playToolRect.m_right)
		{
			x = m_wndLayout.playToolRect.m_right - rcSel.Width() - 1;
		}
		int y = (rcWnd.m_top + 1 > (rcSel.Height() + m_wndLayout.playToolRect.m_top)) ? (rcWnd.m_top - rcSel.Height() - 1) : (rcWnd.m_bottom + 1);

		if( DLG_OK == m_selChnnDlg.DoModal(this, x, y, rcSel.Width(), rcSel.Height()) )
		{
			selCH = m_selChnnDlg.GetSelChnn();

			m_selCuPlayrChnn = selCH;
			m_currSplitMode	= splitMode;

			SetAudioCH();

			GetRootWnd()->Repaint();
			return true;
		}
		else
		{
			GetRootWnd()->Repaint();
		}
	}

	return false;
}


/////////
BEGIN_MSG_MAP(CPlayerDlg, GUI::CDialog)
	//播放、暂停、停止
	ON_MSG(m_btnPlayPause.GetID(), KEY_VIRTUAL_ENTER, OnClickPlayPause)
	ON_MSG(m_btnStop.GetID(), KEY_VIRTUAL_ENTER, OnClickStop)
	ON_MSG(CTRLID_NULL, KEY_VIRTUAL_PLAY, OnClickPlayPause)
	ON_MSG(CTRLID_NULL, KEY_VIRTUAL_STOP, OnClickStop)

	#ifdef __CUSTOM_US02_NEW__
	ON_MSG(m_btnPlayPause.GetID(), KEY_VIRTUAL_MOVE, OnFocus)
	ON_MSG(m_btnStop.GetID(), KEY_VIRTUAL_MOVE, OnFocus)
	#endif

	//单帧播放
	ON_MSG(m_btnNextFrame.GetID(), KEY_VIRTUAL_ENTER, OnClickNextFrame)
	ON_MSG(CTRLID_NULL, KEY_VIRTUAL_SF, OnClickNextFrame)

	#ifdef __CUSTOM_US02_NEW__
	ON_MSG(m_btnNextFrame.GetID(), KEY_VIRTUAL_MOVE, OnFocus)
	#endif

	//快进、快退
	ON_MSG(m_btnFFEx.GetID(), KEY_VIRTUAL_ENTER, OnClickFFEx)
	ON_MSG(m_btnREWEx.GetID(), KEY_VIRTUAL_ENTER, OnClickREWEx)

	ON_MSG(m_btnFastForward.GetID(), KEY_VIRTUAL_ENTER, OnClickFastForward)
	ON_MSG(m_btnReward.GetID(), KEY_VIRTUAL_ENTER, OnClickReward)
	ON_MSG(CTRLID_NULL, KEY_VIRTUAL_FF, OnClickFastForward)
	ON_MSG(CTRLID_NULL, KEY_VIRTUAL_REW, OnClickReward)

	#ifdef __CUSTOM_US02_NEW__
	ON_MSG(m_btnFFEx.GetID(), KEY_VIRTUAL_MOVE, OnFocus)
	ON_MSG(m_btnREWEx.GetID(), KEY_VIRTUAL_MOVE, OnFocus)

	ON_MSG(m_btnFastForward.GetID(), KEY_VIRTUAL_MOVE, OnFocus)
	ON_MSG(m_btnReward.GetID(), KEY_VIRTUAL_MOVE, OnFocus)
	#endif

	//抓图、色彩调节
	ON_MSG(m_btnColor.GetID(), KEY_VIRTUAL_ENTER, OnClickColor)
	ON_MSG(m_btnSnap.GetID(), KEY_VIRTUAL_ENTER, OnClickSnap)

	#ifdef __CUSTOM_US02_NEW__
	ON_MSG(m_btnColor.GetID(), KEY_VIRTUAL_MOVE, OnFocus)
	ON_MSG(m_btnSnap.GetID(), KEY_VIRTUAL_MOVE, OnFocus)
	#endif

	//分割
	ON_MSG(m_btn1X1Ex.GetID(),KEY_VIRTUAL_ENTER,OnClick1X1Ex)
	ON_MSG(m_btn2X2Ex.GetID(),KEY_VIRTUAL_ENTER,OnClick2X2Ex)
	
	ON_MSG(m_btn2X3Ex.GetID(),KEY_VIRTUAL_ENTER,OnClick2X3Ex)
	ON_MSG(m_btn3X3Ex.GetID(),KEY_VIRTUAL_ENTER,OnClick3X3Ex)
	ON_MSG(m_btn4X4Ex.GetID(),KEY_VIRTUAL_ENTER,OnClick4X4Ex)

	ON_MSG(m_btnOneSplit.GetID(), KEY_VIRTUAL_ENTER, OnClickOneSplit)
	ON_MSG(m_btnFourSplit.GetID(), KEY_VIRTUAL_ENTER, OnClickFourSplit)
	ON_MSG(m_btnSixSplit.GetID(), KEY_VIRTUAL_ENTER, OnClickSixSplit)
	ON_MSG(m_btnNineSplit.GetID(), KEY_VIRTUAL_ENTER, OnClickNineSplit)
	
	ON_MSG(m_btnSixteenSplit.GetID(), KEY_VIRTUAL_ENTER, OnClickSixteenSplit)
	ON_MSG(m_btnSpecSplit.GetID(),KEY_VIRTUAL_ENTER,OnClickSpecSplit)
	ON_MSG(m_btnLive.GetID(), KEY_VIRTUAL_ENTER, OnClickLive)

	#ifdef __CUSTOM_US02_NEW__
	ON_MSG(m_btn1X1Ex.GetID(), KEY_VIRTUAL_MOVE, OnFocus)
	ON_MSG(m_btn2X2Ex.GetID(), KEY_VIRTUAL_MOVE, OnFocus)
	ON_MSG(m_btn2X3Ex.GetID(),KEY_VIRTUAL_MOVE, OnFocus)
	ON_MSG(m_btn3X3Ex.GetID(),KEY_VIRTUAL_MOVE, OnFocus)
	ON_MSG(m_btn4X4Ex.GetID(),KEY_VIRTUAL_MOVE, OnFocus)

	ON_MSG(m_btnOneSplit.GetID(), KEY_VIRTUAL_MOVE, OnFocus)
	ON_MSG(m_btnFourSplit.GetID(), KEY_VIRTUAL_MOVE, OnFocus)
	ON_MSG(m_btnSixSplit.GetID(), KEY_VIRTUAL_MOVE, OnFocus)
	ON_MSG(m_btnNineSplit.GetID(), KEY_VIRTUAL_MOVE, OnFocus)
	
	ON_MSG(m_btnSixteenSplit.GetID(), KEY_VIRTUAL_MOVE, OnFocus)
	ON_MSG(m_btnSpecSplit.GetID(),KEY_VIRTUAL_MOVE, OnFocus)
	ON_MSG(m_btnLive.GetID(), KEY_VIRTUAL_MOVE, OnFocus)
	#endif

	//音频、隐藏、返回
	ON_MSG(m_btnZoom.GetID(),KEY_VIRTUAL_ENTER,OnClickZoom)
	ON_MSG(m_btnAudio.GetID(),KEY_VIRTUAL_ENTER,OnClickAudio)
	ON_MSG(m_btnHide.GetID(), KEY_VIRTUAL_ENTER, OnClickHide)
	ON_MSG(m_btnExit.GetID(), KEY_VIRTUAL_ENTER, OnClickExit)
	ON_MSG_EX(m_btnMove.GetID(), KEY_VIRTUAL_ENTER, OnClickMove)
	ON_MSG(CTRLID_NULL, KEY_VIRTUAL_AUDIO,OnClickAudio)
	ON_MSG(CTRLID_NULL, KEY_VIRTUAL_HIDE, OnClickHide)
	ON_MSG(CTRLID_NULL, KEY_VIRTUAL_EXIT, OnClickExit)

	#ifdef __CUSTOM_US02_NEW__
	ON_MSG(m_btnZoom.GetID(), KEY_VIRTUAL_MOVE, OnFocus)
	ON_MSG(m_btnMove.GetID(), KEY_VIRTUAL_MOVE, OnFocus)
	ON_MSG(m_btnAudio.GetID(),KEY_VIRTUAL_MOVE, OnFocus)
	ON_MSG(m_btnHide.GetID(), KEY_VIRTUAL_MOVE, OnFocus)
	ON_MSG(m_btnExit.GetID(), KEY_VIRTUAL_MOVE, OnFocus)
	#endif

	//上一段、下一段、拖动
	ON_MSG(m_btnPreSection.GetID(), KEY_VIRTUAL_ENTER, OnClickPreSection)
	ON_MSG(m_btnNextSection.GetID(),KEY_VIRTUAL_ENTER, OnClickNextSection)
	ON_MSG(CTRLID_NULL, KEY_VIRTUAL_PRE_SECTION, OnClickPreSection)
	ON_MSG(CTRLID_NULL, KEY_VIRTUAL_NEXT_SECTION, OnClickNextSection)
	ON_MSG_EX(m_sliderCtrl.GetID(), KEY_VIRTUAL_ENTER, OnDragStart)
	ON_MSG_EX(m_sliderCtrl.GetID(), KEY_VIRTUAL_DRAG, OnDragIng)
	ON_MSG_EX(m_sliderCtrl.GetID(), KEY_VIRTUAL_BLUP,  OnDragEnd)
	
	#ifdef __CUSTOM_US02_NEW__
	ON_MSG(m_btnPreSection.GetID(), KEY_VIRTUAL_MOVE, OnFocus)
	ON_MSG(m_btnNextSection.GetID(), KEY_VIRTUAL_MOVE, OnFocus)
	ON_MSG(m_ckShowName.GetID(), KEY_VIRTUAL_MOVE, OnFocus)
	#endif


	ON_MSG(m_ckShowName.GetID(), KEY_VIRTUAL_ENTER, OnShowName)
#if defined(__NEW_CUT_BACKUP__)
	ON_MSG(m_btnGetBackupStartTime.GetID(), KEY_VIRTUAL_ENTER, OnGetSliderStartTime)
	//ON_MSG(CTRLID_NULL, KEY_VIRTUAL_CUT, OnGetSliderStartTime)
	ON_MSG(m_btnGetBackupEndTime.GetID(), KEY_VIRTUAL_ENTER, OnGetSliderEndTime)
	//ON_MSG(CTRLID_NULL, KEY_VIRTUAL_CUT, OnGetSliderEndTime)
	ON_MSG(m_btnbackup.GetID(),KEY_VIRTUAL_ENTER,BackupReplay)
	//ON_MSG(CTRLID_NULL, KEY_VIRTUAL_BACKUP, BackupReplay)

	#ifdef __CUSTOM_US02_NEW__
	ON_MSG(m_btnGetBackupStartTime.GetID(), KEY_VIRTUAL_MOVE, OnFocus)
	ON_MSG(m_btnGetBackupEndTime.GetID(), KEY_VIRTUAL_MOVE, OnFocus)
	ON_MSG(m_btnbackup.GetID(), KEY_VIRTUAL_MOVE, OnFocus)
	#endif
#else
	ON_MSG(m_btnGetBackupTime.GetID(), KEY_VIRTUAL_ENTER, GetSliderTime)
	ON_MSG(CTRLID_NULL, KEY_VIRTUAL_CUT, GetSliderTime)
	ON_MSG(m_btnbackup.GetID(),KEY_VIRTUAL_ENTER,BackupReplay)
	ON_MSG(CTRLID_NULL, KEY_VIRTUAL_BACKUP, BackupReplay)

	#ifdef __CUSTOM_US02_NEW__
	ON_MSG(m_btnGetBackupTime.GetID(), KEY_VIRTUAL_MOVE, OnFocus)
	ON_MSG(m_btnbackup.GetID(), KEY_VIRTUAL_MOVE, OnFocus)
	#endif
#endif
#if defined(__ASYN_PLAY__)
	ON_MSG(m_ckSyncPlayback.GetID(), KEY_VIRTUAL_ENTER, OnClickAsync);
	#ifdef __CUSTOM_US02_NEW__
	ON_MSG(m_ckSyncPlayback.GetID(), KEY_VIRTUAL_MOVE, OnFocus)
	#endif
#endif
END_MSG_MAP()

unsigned long CPlayerDlg::OnPreCommand(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	if ((KEY_VIRTUAL_1X1 <= keyID) && (keyID <= KEY_VIRTUAL_6X6))
	{
		return KEY_VIRTUAL_MSG_STOP;
	}
	else
	{
		return keyID;
	}
}
////////////////////////////播放、暂停、停止////////////////////////////////////////
unsigned long CPlayerDlg::OnClickPlayPause()
{
#if !defined(__NEW_CUT_BACKUP__)
	m_btnGetBackupTime.SetEnable(true,false);
#endif
	if (BMP_BTN_PLAY == m_btnPlayPause.GetImage(PAINT_STATUS_NORMAL))
	{
		PlayStatus();
		return KEY_VIRTUAL_PLAY;
	} 
	else
	{
		PauseStatus();
		return KEY_VIRTUAL_PLAY;
	}
}

unsigned long CPlayerDlg::OnClickStop()
{
	StopStatus();
	
	return KEY_VIRTUAL_STOP;
}
//播放、暂停、停止//end

////////////////////////////单帧播放///////////////////////////////////////
unsigned long CPlayerDlg::OnClickNextFrame()
{
#if !defined(__ASYN_PLAY__)
	if (PLAYBACK_PAUSE == m_currStatus)
	{
		NextFrameStatus();
		return KEY_VIRTUAL_SF;
	}
	else
	{
		return KEY_VIRTUAL_MSG_STOP;
	}
#else
	if (PLAYBACK_PAUSE == m_currStatus[m_curRedSelectChnn])
	{
		NextFrameStatus();
		return KEY_VIRTUAL_SF;
	}
	else
	{
		return KEY_VIRTUAL_MSG_STOP;
	}
#endif
}
//单帧播放 //end

//////////////////////////快进、快退///////////////////////////////////////////
unsigned long CPlayerDlg::OnClickFastForward()
{
#if !defined(__ASYN_PLAY__)
	if (PLAYBACK_FF == m_currStatus)
	{
		//如果正在快进，则换个倍速
		m_curForwardSpeed = GetNextPlaySpeed(true,0);
	}
	else if (PLAYBACK_REW == m_currStatus)
	{
		//如果正在快进，则拦截消息，正在快进时不响应快退。
		return KEY_VIRTUAL_MSG_STOP;
	}
	else if (PLAYBACK_PLAYING == m_currStatus)
	{
		//如果正在播放或者暂停，则从2倍速开始
		m_curForwardSpeed = PLAY_SPEED_2X;
	}
	else
	{
		//如果正在暂停，则拦截消息
		return KEY_VIRTUAL_MSG_STOP;
	}

	FastForwardStatus();
	return KEY_VIRTUAL_FF;
#else
	if(m_bSync)
	{
		for (int i = 0; i < g_p_login_succ->productInfo.localVideoInputNum; i++)
		{
			if (PLAYBACK_FF == m_currStatus[i])
			{
				//如果正在快进，则换个倍速
				m_curForwardSpeed[i] = GetNextPlaySpeed(true,i);
			}
			else if (PLAYBACK_REW == m_currStatus[i])
			{
				//如果正在快进，则拦截消息，正在快进时不响应快退。
				return KEY_VIRTUAL_MSG_STOP;
			}
			else if (PLAYBACK_PLAYING == m_currStatus[i])
			{
				//如果正在播放或者暂停，则从2倍速开始
				m_curForwardSpeed[i] = PLAY_SPEED_2X;
			}
			else
			{
				//如果正在暂停，则拦截消息
				return KEY_VIRTUAL_MSG_STOP;
			}
		}
	}
	else
	{
		printf("m_curRedSelectChnn=%d,m_currStatus[m_curRedSelectChnn]=%d\n",m_curRedSelectChnn,m_currStatus[m_curRedSelectChnn]);
		if (PLAYBACK_FF == m_currStatus[m_curRedSelectChnn])
		{
			//如果正在快进，则换个倍速
			m_curForwardSpeed[m_curRedSelectChnn] = GetNextPlaySpeed(true,m_curRedSelectChnn);
		}
		else if (PLAYBACK_REW == m_currStatus[m_curRedSelectChnn])
		{
			//如果正在快进，则拦截消息，正在快进时不响应快退。
			return KEY_VIRTUAL_MSG_STOP;
		}
		else if (PLAYBACK_PLAYING == m_currStatus[m_curRedSelectChnn])
		{
			//如果正在播放或者暂停，则从2倍速开始
			m_curForwardSpeed[m_curRedSelectChnn] = PLAY_SPEED_2X;
		}
		else
		{
			//如果正在暂停，则拦截消息
			return KEY_VIRTUAL_MSG_STOP;
		}

	}
	FastForwardStatus();
	return KEY_VIRTUAL_FF;
#endif


}

unsigned long CPlayerDlg::OnClickReward()
{
#if !defined(__ASYN_PLAY__)
	if (PLAYBACK_REW == m_currStatus)
	{
		//如果正在快退，则换个倍速
		m_curRewardSpeed = GetNextPlaySpeed(false,0);
	}
	else if (PLAYBACK_FF == m_currStatus)
	{
		//如果正在快进，则拦截消息，正在快退时不响应快进。
		return KEY_VIRTUAL_MSG_STOP;
	}
	else if (PLAYBACK_PLAYING == m_currStatus)
	{
		//如果正在播放或者暂停，则从8倍速快退开始
		m_curRewardSpeed = PLAY_SPEED_8X;
	}
	else
	{
		//如果正在暂停，则拦截消息
		return KEY_VIRTUAL_MSG_STOP;
	}

	RewardStatus();
	return KEY_VIRTUAL_REW;

#else
	if(m_bSync)
	{
		for (int i = 0; i < g_p_login_succ->productInfo.localVideoInputNum; i++)
		{
			if (PLAYBACK_REW == m_currStatus[i])
			{
				//如果正在快退，则换个倍速
				m_curRewardSpeed[i] = GetNextPlaySpeed(false,i);
			}
			else if (PLAYBACK_FF == m_currStatus[i])
			{
				//如果正在快进，则拦截消息，正在快退时不响应快进。
				return KEY_VIRTUAL_MSG_STOP;
			}
			else if (PLAYBACK_PLAYING == m_currStatus[i])
			{
				//如果正在播放或者暂停，则从8倍速快退开始
				m_curRewardSpeed[i] = PLAY_SPEED_8X;
			}
			else
			{
				//如果正在暂停，则拦截消息
				return KEY_VIRTUAL_MSG_STOP;
			}
		}
	}
	else
	{
		if (PLAYBACK_REW == m_currStatus[m_curRedSelectChnn])
		{
			//如果正在快退，则换个倍速
			m_curRewardSpeed[m_curRedSelectChnn] = GetNextPlaySpeed(false,m_curRedSelectChnn);
		}
		else if (PLAYBACK_FF == m_currStatus[m_curRedSelectChnn])
		{
			//如果正在快进，则拦截消息，正在快退时不响应快进。
			return KEY_VIRTUAL_MSG_STOP;
		}
		else if (PLAYBACK_PLAYING == m_currStatus[m_curRedSelectChnn])
		{
			//如果正在播放或者暂停，则从8倍速快退开始
			m_curRewardSpeed[m_curRedSelectChnn] = PLAY_SPEED_8X;
		}
		else
		{
			//如果正在暂停，则拦截消息
			return KEY_VIRTUAL_MSG_STOP;
		}
	}
	RewardStatus();
	return KEY_VIRTUAL_REW;
#endif
}

DLG_END_VALUE CPlayerDlg::PopupLiveColorDlg( int x, int y, int cx, int cy, int chnn )
{
	
	DLG_END_VALUE retVal;


	retVal = m_changeColorDlg.DoModal( this, x , y, cx, cy,NULL);

	return retVal;
}

unsigned long CPlayerDlg::OnClickColor()
{	
	//
	unsigned short xOff = m_width - CCfgDevColorDlg::GetWidth() - CCfgDevColorDlg::CFG_DLG_RIGHT_X;
	if (m_width < 800)
	{
		xOff -= 40;
	}

	if (m_width < 800)
	{
		xOff -= 40;
	}
	

	MESSAGE_DATA msgData;
	memset(&msgData, 0, sizeof(MESSAGE_DATA));
	CMessageMan::Instance()->SendMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_GET_DEV_COLOR, NULL, 0, msgData);
	unsigned long color = *reinterpret_cast<unsigned long *>(msgData.pData + sizeof(PACKCMD));
	CMessageMan::Instance()->ReleaseMsgBuff(msgData);

	m_changeColorDlg.SetDevColorInfo(color);
	PopupLiveColorDlg(xOff, CENTER_POS,m_changeColorDlg.GetWidth(), m_changeColorDlg.GetHeight(), -1);	

	GetRootWnd()->Repaint();

	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CPlayerDlg::OnClickSnap()
{
	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CPlayerDlg::OnClickFFEx()
{
#if !defined(__ASYN_PLAY__)
	unsigned short menuHeight = m_menuForward.GetHeight();
	unsigned short menuWidth = m_menuForward.GetWidth();
	GUI::CRect rect = m_btnFFEx.GetWindowRect();
	GUI::CRect wndRet = GetWindowRect();
	int x = (rect.m_left + menuWidth > m_width) ? (m_width - menuWidth - 1) : (rect.m_left);
	int y = (wndRet.m_top > menuHeight + 40) ? (wndRet.m_top - menuHeight) :  (wndRet.m_bottom + 1);
	int ret = m_menuForward.PopUp(x, y, &m_btnFFEx);
	if (-1 != ret)
	{
		m_curForwardSpeed = m_menuForward.GetItemData(ret);
		FastForwardStatus();
		return KEY_VIRTUAL_FF;
	} 
	else
	{
		return KEY_VIRTUAL_MSG_STOP;
	}
#else
	unsigned short menuHeight = m_menuForward.GetHeight();
	unsigned short menuWidth = m_menuForward.GetWidth();
	GUI::CRect rect = m_btnFFEx.GetWindowRect();
	GUI::CRect wndRet = GetWindowRect();
	int x = (rect.m_left + menuWidth > m_width) ? (m_width - menuWidth - 1) : (rect.m_left);
	int y = (wndRet.m_top > menuHeight + 40) ? (wndRet.m_top - menuHeight) :  (wndRet.m_bottom + 1);
	int ret = m_menuForward.PopUp(x, y, &m_btnFFEx);
	if (-1 != ret)
	{
		if(m_bSync)
		{
			for (int i = 0; i < g_p_login_succ->productInfo.localVideoInputNum; i++)
			{
				m_curForwardSpeed[i] = m_menuForward.GetItemData(ret);
			}
		}
		else
		{
			m_curForwardSpeed[m_curRedSelectChnn] = m_menuForward.GetItemData(ret);
		}
		FastForwardStatus();
		return KEY_VIRTUAL_FF;
	} 
	else
	{
		return KEY_VIRTUAL_MSG_STOP;
	}
#endif
}

unsigned long CPlayerDlg::OnClickREWEx()
{
#if !defined(__ASYN_PLAY__)

	unsigned short menuHeight = m_menuReward.GetHeight();
	unsigned short menuWidth = m_menuReward.GetWidth();
	GUI::CRect rect = m_btnREWEx.GetWindowRect();
	GUI::CRect wndRet = GetWindowRect();
	int x = (rect.m_left + menuWidth > m_width) ? (m_width - menuWidth - 1) : (rect.m_left);
	int y = (wndRet.m_top > menuHeight + 40) ? (wndRet.m_top - menuHeight) :  (wndRet.m_bottom + 1);
	int ret = m_menuReward.PopUp(x, y, &m_btnREWEx);
	if (-1 != ret)
	{
		m_curRewardSpeed = m_menuReward.GetItemData(ret);
		RewardStatus();
		return KEY_VIRTUAL_REW;
	} 
	else
	{
		return KEY_VIRTUAL_MSG_STOP;
	}
#else
	unsigned short menuHeight = m_menuReward.GetHeight();
	unsigned short menuWidth = m_menuReward.GetWidth();
	GUI::CRect rect = m_btnREWEx.GetWindowRect();
	GUI::CRect wndRet = GetWindowRect();
	int x = (rect.m_left + menuWidth > m_width) ? (m_width - menuWidth - 1) : (rect.m_left);
	int y = (wndRet.m_top > menuHeight + 40) ? (wndRet.m_top - menuHeight) :  (wndRet.m_bottom + 1);
	int ret = m_menuReward.PopUp(x, y, &m_btnREWEx);
	if (-1 != ret)
	{
		if(m_bSync)
		{
			for (int i = 0; i < g_p_login_succ->productInfo.localVideoInputNum; i++)
			{
				m_curRewardSpeed[i] = m_menuReward.GetItemData(ret);
			}
		}
		else
		{
			m_curRewardSpeed[m_curRedSelectChnn] = m_menuReward.GetItemData(ret);
		}
		RewardStatus();
		return KEY_VIRTUAL_REW;
	} 
	else
	{
		return KEY_VIRTUAL_MSG_STOP;
	}
#endif
}
//快进、快退//end

////////////////////////////////放大、音频、隐藏、返回///////////////////////////////////
unsigned long CPlayerDlg::OnClickZoom()
{
	return KEY_VIRTUAL_ZOOM;
}

unsigned long CPlayerDlg::OnClickAudio()
{
	//如果不包含音频通道，则不必要响应了。
	if (0 == (m_selCuPlayrChnn & m_audioChnnMask))
	{
		return KEY_VIRTUAL_MSG_STOP;
	}

	//先检查是否是单通道
	int numChnn = 0;
	for ( int i=0; i<64; i++)
	{
		if( m_selCuPlayrChnn & (static_cast<ULONGLONG>(0x1)<<i) )
		{
			numChnn += 1;
		}
	}

	//当前通道数目为1时，单击音频按钮则是切换静音和开音。
	if( numChnn <= 1 )
	{
		int state = m_btnAudio.GetCurState();
		if( state)
		{
			//非静音
			m_btnAudio.SetCurState(AUDIO_OPEN, true);

			m_selAudioCH = m_selCuPlayrChnn;
		}
		else
		{
			m_btnAudio.SetCurState(AUDIO_MUTE, true);

			m_selAudioCH = static_cast<ULONGLONG>(0x0);
		}

		return KEY_VIRTUAL_AUDIO;
	}

	//多通道弹出通道音频选择
	GUI::CRect btnRect = m_btnAudio.GetWindowRect();
	GUI::CRect wndRect = GetWindowRect();
	
	//能选择的通道应该根据当前系统支持的最大音频通道数目（目前都是把音频通道从零开始不间断地和视频通道绑定）
    ULONGLONG curAudioRange = m_selCuPlayrChnn & m_audioChnnMask;
	m_selAudioDlg.SetChnnAudio(curAudioRange, m_selAudioCH);
	GUI::CRect rect = m_selAudioDlg.GetDlgDefaultRect();
	int x = (btnRect.m_left > m_wndLayout.playToolRect.m_left + (rect.Width() - btnRect.Width()) / 2)  ? (btnRect.m_left - (rect.Width() - btnRect.Width()) / 2) : btnRect.m_left;
	if(x + rect.Width() > m_wndLayout.playToolRect.m_right)
	{
		x = m_wndLayout.playToolRect.m_right - rect.Width() - 1;
	}
	int y = (wndRect.m_top + 1 > (rect.Height() + m_wndLayout.playToolRect.m_top)) ? (wndRect.m_top - rect.Height() - 1) : (wndRect.m_bottom + 1);
	if( DLG_OK == m_selAudioDlg.DoModal(this,x,y,rect.Width(),rect.Height()) )
	{
		m_selAudioCH = m_selAudioDlg.GetSelChnn();

		gui_trace(true, printf("sel chnn = 0x%X.\n", m_selAudioCH));

		if(0 != m_selAudioCH)
		{
			m_btnAudio.SetCurState(AUDIO_OPEN, true);
		}
		else
		{
			//表示静音
			m_btnAudio.SetCurState(AUDIO_MUTE, true);
		}

		GetRootWnd()->Repaint();

		return KEY_VIRTUAL_AUDIO;
	}
	else
	{
		GetRootWnd()->Repaint();

		return KEY_VIRTUAL_MSG_STOP;
	}
}


unsigned long CPlayerDlg::OnClickHide()
{
	return KEY_VIRTUAL_HIDE;
}

unsigned long CPlayerDlg::OnClickExit()
{
	return KEY_VIRTUAL_EXIT;
}

unsigned long CPlayerDlg::OnClickMove(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	if(PtInPlayToolBar(xPos, yPos))
	{
		m_bDrag = true;
		m_bFirstDrag = true;

		m_x_enter = xPos;
		m_y_enter = yPos;
	}

	return KEY_VIRTUAL_MSG_STOP;
}

///////////////////////////////分割按钮/////////////////////////////////////
unsigned long CPlayerDlg::OnClickOneSplit()
{
	if (OnChangeCH(m_1X1CH, PLAY_CH_ONE, 1))
	{
		return KEY_VIRTUAL_1X1;
	}

	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CPlayerDlg::OnClickFourSplit()
{
	if (OnChangeCH(m_2X2CH, PLAY_CH_FOUR, 4))
	{
		return KEY_VIRTUAL_2X2;
	}

	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CPlayerDlg::OnClickSixSplit()
{
	m_spectype=SPEC_NONE;
	if (OnChangeCH(m_2X3CH, PLAY_CH_SIX, 6))
	{
		return KEY_VIRTUAL_2X3;
	}

	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CPlayerDlg::OnClickNineSplit()
{
	m_spectype=SPEC_NONE;
	if (OnChangeCH(m_3X3CH, PLAY_CH_NINE, 9))
	{
		return KEY_VIRTUAL_3X3;
	}

	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CPlayerDlg::OnClickSixteenSplit()
{
	if (OnChangeCH(m_4X4CH, PLAY_CH_SIXTEEN, 16))
	{
		return KEY_VIRTUAL_4X4;
	}

	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CPlayerDlg::OnClickLive()
{
	int maxLiveChnn = 0;
	if(!IsSupportLiPb(maxLiveChnn))
	{
		return KEY_VIRTUAL_MSG_STOP;
	}

	ULONGLONG authCH = CUserMan::Instance()->CheckAuthority(AUTH_LIVE);
	ULONGLONG authSet= GetMainMonitorCfg();

	unsigned long ipcMaxVideoOutNum = CProduct::Instance()->IPCMaxVideoOutNum();
	unsigned long localVideoNum = g_p_login_succ->productInfo.localVideoInputNum;
	unsigned long iMaxChnCount = m_maxChnnNum;

	if( ipcMaxVideoOutNum && localVideoNum )
	{
		iMaxChnCount = localVideoNum;
	}

	authCH &= authSet;
	ULONGLONG selCH = 0;
#if defined(__ASYN_PLAY__)
	selCH = m_liveSelCH;
#endif
	int playbackChnn = 0;
	for(int i = 0; i < iMaxChnCount; i++)
	{
		if(m_selCuPlayrChnn & ((ULONGLONG)0x1 << i))
		{
			playbackChnn++;
		}
	}

	int maxSel = 0;
	if(36 > playbackChnn)
	{
		maxSel = 36 - playbackChnn;	//6X6 限制
		maxSel = (maxSel > maxLiveChnn) ? maxLiveChnn : maxSel; //最大通道限制
	}

	if((authCH != 0) && (maxSel > 0))
	{
		m_selChnnDlg.SetChnnCount(iMaxChnCount, authCH, maxSel, selCH);
		m_selChnnDlg.SetAllChnnEnable(false);
		m_selChnnDlg.SetAutoSel(false);

		GUI::CRect rcBtn = m_btnLive.GetWindowRect();
		GUI::CRect rcWnd = GetWindowRect();
		GUI::CRect rcSel = m_selChnnDlg.GetDlgDefaultRect();
		int x = (rcBtn.m_left > m_wndLayout.playToolRect.m_left + (rcSel.Width() - rcBtn.Width()) / 2)  ? (rcBtn.m_left - (rcSel.Width() - rcBtn.Width()) / 2) : rcBtn.m_left;
		if(x + rcSel.Width() > m_wndLayout.playToolRect.m_right)
		{
			x = m_wndLayout.playToolRect.m_right - rcSel.Width() - 1;
		}
		int y = (rcWnd.m_top + 1 > (rcSel.Height() + m_wndLayout.playToolRect.m_top)) ? (rcWnd.m_top - rcSel.Height() - 1) : (rcWnd.m_bottom + 1);

		if( DLG_OK == m_selChnnDlg.DoModal(this, x, y, rcSel.Width(), rcSel.Height()) )
		{
			selCH = m_selChnnDlg.GetSelChnn();
#if defined(__ASYN_PLAY__)
			m_liveSelCH = selCH;
#endif
			if(0 != selCH)
			{
				bool bGetDispOK = false;

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
					//获取当前显示的通道
					NET_DISPLAY_INFO* pDispInfo = reinterpret_cast<NET_DISPLAY_INFO*>(msgData.pData + sizeof(PACKCMD));

					//memcpy(&curDisp, pDispInfo, sizeof(NET_DISPLAY_INFO));

					memset(&curDisp, 0, sizeof(NET_DISPLAY_INFO));
					curDisp.type = NET_DISPLAY_CHANGE_DATA_PACK;
					curDisp.rec = 1;
					curDisp.time_out = -1;
					
					int index = 0;
					int viewIndex = 0;

					//回放通道
					for(int i = 0; i < pDispInfo->data_len; i++)
					{
						if(DISPLAY_LAYER_DIGIT == DISPLAY_GET_LAYER(pDispInfo->data[i]))
						{		
							curDisp.data[index++] = DISPLAY_PACK(DISPLAY_GET_LAYER(pDispInfo->data[i]), DISPLAY_GET_SIGNAL(pDispInfo->data[i]), viewIndex++, DISPLAY_GET_CHNN(pDispInfo->data[i]));
						}
					}

					//现场通道
					for(int i = 0; i < iMaxChnCount; i++)
					{
						if(selCH & ((ULONGLONG)0x1 << i))
						{
							curDisp.data[index++] = DISPLAY_PACK(DISPLAY_LAYER_ANALOG, DISPLAY_SIGNAL_ANALOG, viewIndex++, i);
						}
					}

					curDisp.data_len = index;

					bGetDispOK = (curDisp.data_len > 0) ? true : false;
				}
				CMessageMan::Instance()->ReleaseMsgBuff(msgData);

				if(bGetDispOK)
				{
					LIVESTRUCT livestruct;
					unsigned long	splitMode=0;
					int type=GetSPECtype();
					livestruct.chnnBits = GetCurPlayChnn(splitMode,type);
					livestruct.curDisp =curDisp;
					memset(&msgData, 0, sizeof(MESSAGE_DATA));
					CMessageMan::Instance()->SendMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_LOCAL_CLIENT_PLAYBACK_LIVE, (unsigned char*)&livestruct, sizeof(LIVESTRUCT), msgData);
					CMessageMan::Instance()->ReleaseMsgBuff(msgData);
				}
			}
		}

#if defined(__NEW_CUT_BACKUP__)		
		//切换后重新把红框设置为初始位置
		for(int i=0;i<iMaxChnCount;i++)
		{
			if (m_selCuPlayrChnn & (((LONGLONG)0x01) << i))
			{
				m_curRedSelectChnn=i;//找到最小的通道，把它设置为红框
				break;
			}	
		}
		MESSAGE_DATA msgData;
		memset(&msgData, 0, sizeof(MESSAGE_DATA));
		CMessageMan::Instance()->SendMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_LOCAL_CLIENT_PLAYBACK_CHNINIT, (unsigned char*)&m_curRedSelectChnn,sizeof(unsigned long), msgData);
		CMessageMan::Instance()->ReleaseMsgBuff(msgData);
#endif

		
		GetRootWnd()->Repaint();
	}

	return KEY_VIRTUAL_MSG_STOP;
}

/***************************************************************************************
函数：OnSplit()
功能：播放时在分割模式1/4/9/16之间切换
		只有1UTW按键板有此按键
zhl 2010-06-07
修改：
	1：切换到下一个分割模式时要检测设备是否支持该分割模式，如果不支持就回到1X1分割模式。 ZHL 2010-07-16
***************************************************************************************/
unsigned long CPlayerDlg::OnSplit()
{
	ULONGLONG *pNextChnn = NULL;
	PLAY_CH_MODE nextSplitMode = PLAY_CH_NULL;
	unsigned long displayCount = 0;


	switch(m_currSplitMode)
	{
	case PLAY_CH_ONE:
		{
			if (1 < g_p_login_succ->productInfo.videoOuputNum)
			{
				pNextChnn = &m_2X2CH;
				nextSplitMode = PLAY_CH_FOUR;
				displayCount = 4;
			} 
			else
			{
				return KEY_VIRTUAL_MSG_STOP;
			}
		}
		break;
	case PLAY_CH_FOUR:
		{
			if (4 < g_p_login_succ->productInfo.videoOuputNum)
			{
				pNextChnn = &m_2X3CH;
				nextSplitMode = PLAY_CH_SIX;
				displayCount = 6;
			} 
			else
			{
				pNextChnn = &m_1X1CH;
				nextSplitMode = PLAY_CH_ONE;
				displayCount = 1;
			}

		}
		break;
	case PLAY_CH_SIX:
		{
			if (6 < g_p_login_succ->productInfo.videoOuputNum)
			{
				pNextChnn = &m_3X3CH;
				nextSplitMode = PLAY_CH_NINE;
				displayCount = (8 < g_p_login_succ->productInfo.videoInputNum) ? 9 : 8;
			} 
			else
			{
				pNextChnn = &m_1X1CH;
				nextSplitMode = PLAY_CH_ONE;
				displayCount = 1;
			}
		}
		break;
	case PLAY_CH_NINE:
		{
			if (8 < g_p_login_succ->productInfo.videoOuputNum)
			{
				pNextChnn = &m_4X4CH;
				nextSplitMode = PLAY_CH_SIXTEEN;
				displayCount = 16;
			} 
			else
			{
				pNextChnn = &m_1X1CH;
				nextSplitMode = PLAY_CH_ONE;
				displayCount = 1;
			}
		}
		break;
	case PLAY_CH_SIXTEEN:
		{
			if (16 < g_p_login_succ->productInfo.videoOuputNum)
			{
				pNextChnn = &m_1X1CH;
				nextSplitMode = PLAY_CH_ONE;
				displayCount = 1;
			} 
			else
			{
				pNextChnn = &m_1X1CH;
				nextSplitMode = PLAY_CH_ONE;
				displayCount = 1;
			}
		}
		break;
	default:
		{
			assert(false);
			pNextChnn = &m_1X1CH;
			nextSplitMode = PLAY_CH_ONE;
			displayCount = 1;
		}
		break;
	}

	if (OnChangeCH(*pNextChnn, nextSplitMode, displayCount))
	{
		return KEY_VIRTUAL_SPLIT;
	}

	return KEY_VIRTUAL_MSG_STOP;
}

/***************************************************************************************
函数：OnGroup()
功能：播放时画面切换，在当分割模式下翻页
		只有1UTW按键板有此按键
zhl 2010-06-07
***************************************************************************************/
unsigned long CPlayerDlg::OnGroup()
{
	ULONGLONG *pCurrChnn = NULL;
	PLAY_CH_MODE currSplitMode = PLAY_CH_NULL;
	unsigned long displayCount = 0;

	switch(m_currSplitMode)
	{
	case PLAY_CH_ONE:
		{
			pCurrChnn = &m_1X1CH;
			currSplitMode = PLAY_CH_ONE;
			displayCount = 1;
		}
		break;
	case PLAY_CH_FOUR:
		{
			pCurrChnn = &m_2X2CH;
			currSplitMode = PLAY_CH_FOUR;
			displayCount = 4;
		}
		break;
	case PLAY_CH_SIX:
		{
			pCurrChnn = &m_2X3CH;
			currSplitMode = PLAY_CH_SIX;
			displayCount = 6;
		}
		break;
	case PLAY_CH_NINE:
		{
			pCurrChnn = &m_3X3CH;
			currSplitMode = PLAY_CH_NINE;
			displayCount = (8 < g_p_login_succ->productInfo.videoInputNum) ? 9 : 8;
		}
		break;
	case PLAY_CH_SIXTEEN:
		{
			pCurrChnn = &m_4X4CH;
			currSplitMode = PLAY_CH_SIXTEEN;
			displayCount = 16;
		}
		break;

	default:
		{
			pCurrChnn = &m_1X1CH;
			currSplitMode = PLAY_CH_ONE;
			displayCount = 1;
		}
		break;
	}


	if (OnChangeCH(*pCurrChnn, currSplitMode, displayCount))
	{
		return KEY_VIRTUAL_GROUP;
	}

	return KEY_VIRTUAL_MSG_STOP;
}


unsigned long CPlayerDlg::OnClick1X1Ex()
{
	if (OnClickSelChnn(m_btn1X1Ex, PLAY_CH_ONE, 1, m_1X1CH))
	{
		return KEY_VIRTUAL_1X1;
	}

	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CPlayerDlg::OnClick2X2Ex()
{
	if (OnClickSelChnn(m_btn2X2Ex, PLAY_CH_FOUR, 4, m_2X2CH))
	{
		return KEY_VIRTUAL_2X2;
	}

	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CPlayerDlg::OnClick2X3Ex()
{
	m_spectype=SPEC_NONE;
	if (OnClickSelChnn(m_btn2X3Ex, PLAY_CH_SIX, 6, m_2X3CH))
	{
		return KEY_VIRTUAL_2X3;
	}

	return KEY_VIRTUAL_MSG_STOP;	
}

unsigned long CPlayerDlg::OnClick3X3Ex()
{
	if (OnClickSelChnn(m_btn3X3Ex, PLAY_CH_NINE, 9, m_3X3CH))
	{
		return KEY_VIRTUAL_3X3;
	}

	return KEY_VIRTUAL_MSG_STOP;	
}

unsigned long CPlayerDlg::OnClick4X4Ex()
{
	if (OnClickSelChnn(m_btn4X4Ex, PLAY_CH_SIXTEEN, 16, m_4X4CH))
	{
		return KEY_VIRTUAL_4X4;
	}

	return KEY_VIRTUAL_MSG_STOP;	
}

//分割结束

///////////////////////上一段、下一段、拖动/////////////////////////////////////////
unsigned long CPlayerDlg::OnClickPreSection()
{
	return KEY_VIRTUAL_DEC;
}

unsigned long CPlayerDlg::OnClickNextSection()
{
	return KEY_VIRTUAL_ADD;
}

unsigned long CPlayerDlg::OnDragStart(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	if ((MESSAGE_VIRTUAL_DRAG_ING == keyID)	|| (MESSAGE_VIRTUAL_DRAG_END == keyID))
	{
		return keyID;
	}
	else
	{
		m_scTime.Show(true);
		return MESSAGE_VIRTUAL_DRAG_START;
	}	
}

unsigned long CPlayerDlg::OnDragIng(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{	
	if ((MESSAGE_VIRTUAL_DRAG_START == keyID) || (MESSAGE_VIRTUAL_DRAG_END == keyID))
	{
		return keyID;
	}
	else
	{
		return MESSAGE_VIRTUAL_DRAG_ING;
	}	
}


unsigned long CPlayerDlg::OnDragEnd(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{

	if ((MESSAGE_VIRTUAL_DRAG_ING == keyID)	|| (MESSAGE_VIRTUAL_DRAG_START == keyID))
	{
		return keyID;
	}
	else
	{
		m_scTime.Show(false);
		return MESSAGE_VIRTUAL_DRAG_END;
	}	
}
//上一段、下一段、拖动//end

//
unsigned long CPlayerDlg::OnShowName()
{
	ULONGLONG CH = 0;
	if (m_ckShowName.IsCheck())
	{
		CH = ALL_CHNN_MASK(m_maxChnnNum);
	}

	MESSAGE_DATA msgData;
	memset(&msgData, 0, sizeof(MESSAGE_DATA));
	CMessageMan::Instance()->SendMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_LOCAL_CLIENT_PLAYBACK_CAM_NAME, reinterpret_cast<unsigned char *>(&CH), sizeof(ULONGLONG), msgData);
	CMessageMan::Instance()->ReleaseMsgBuff(msgData);

	GetRootWnd()->Repaint();

	return KEY_VIRTUAL_MSG_STOP;
}

#if defined(__ASYN_PLAY__)
unsigned long CPlayerDlg::OnClickAsync()
{
	if (m_ckSyncPlayback.IsCheck())
	{
		m_bSync = false;
		m_btnbackup.SetEnable(false,false);
	}
	else
	{
		m_bSync = true;
		m_btnbackup.SetEnable(true,false);
	}
	Repaint();
	return KEY_VIRTUAL_MSG_STOP;
}
#endif

//
void CPlayerDlg::SetTime(unsigned long time, unsigned long pos, bool bRefresh)
{
	std::string strTm;
	CTimeCtrl::GetTime32Text(strTm, time, CTimeCtrl::SECTION_HOUR|CTimeCtrl::SECTION_MIN|CTimeCtrl::SECTION_SEC);
	m_scTime.SetCaption(strTm, bRefresh);
	m_sliderCtrl.SetCurPos(pos, bRefresh);	
}


void CPlayerDlg::SetRange(int min, int max)
{	
	m_sliderCtrl.SetRange(min, max);		
}

void CPlayerDlg::SetSectionTime(unsigned long startTime, unsigned long endTime)
{
	m_sliderStartTime=startTime;
	m_sliderEndTime=endTime;
	std::string strTm;

	CTimeCtrl::GetTime32Text(strTm, startTime);
	m_stSecStartTime.SetCaption(strTm);

	CTimeCtrl::GetTime32Text(strTm, endTime, CTimeCtrl::SECTION_HOUR|CTimeCtrl::SECTION_MIN|CTimeCtrl::SECTION_SEC);
	m_stSecEndTime.SetCaption(strTm);
}

int CPlayerDlg::GetCurPos()
{
	return m_sliderCtrl.GetPos();
}

void CPlayerDlg::ShowTime(bool bShow)
{
	m_scTime.Show(false);
}

bool CPlayerDlg::PlayPause()
{
	if (BMP_BTN_PLAY == m_btnPlayPause.GetImage(PAINT_STATUS_NORMAL))
	{

		PlayStatus();
	} 
	else
	{
		PauseStatus();
	}

	return true;
}

bool CPlayerDlg::NextFrame()
{
#if !defined(__ASYN_PLAY__)
	if (PLAYBACK_PAUSE == m_currStatus)
	{
		NextFrameStatus();
		return true;
	}
	else
	{
		return false;
	}
#else
	if (PLAYBACK_PAUSE == m_currStatus[m_curRedSelectChnn])
	{
		NextFrameStatus();
		return true;
	}
	else
	{
		return false;
	}
#endif
}

bool CPlayerDlg::ChangeSpeed(bool bForward)
{
#if !defined(__ASYN_PLAY__)
	if (bForward)
	{
		if (PLAYBACK_FF == m_currStatus)
		{
			//如果正在快进，则换个倍速
			m_curForwardSpeed = GetNextPlaySpeed(true,0);
		}
		else if (PLAYBACK_REW == m_currStatus)
		{
			//如果正在快退，则拦截消息，正在快进时不响应快退。
			return false;
		}
		else
		{
			//播放或者暂停，则从2倍速开始
			m_curForwardSpeed = PLAY_SPEED_2X;
		}

		FastForwardStatus();
	}
	else
	{
		if (PLAYBACK_REW == m_currStatus)
		{
			//如果正在快退，则换个倍速
			m_curRewardSpeed = GetNextPlaySpeed(false,0);
		}
		else if (PLAYBACK_FF == m_currStatus)
		{
			//如果正在快退，则拦截消息，正在快退时不响应快进。
			return false;
		}
		else
		{
			//播放或者暂停，则从2倍速开始
			m_curRewardSpeed = PLAY_SPEED_8X;
		}

		RewardStatus();
	}

	return true;
#else
	if(m_bSync)
	{
		if (bForward)
		{
			for (int i = 0; i < g_p_login_succ->productInfo.localVideoInputNum; i++)
			{
				if (PLAYBACK_FF == m_currStatus[i])
				{
					//如果正在快进，则换个倍速
					m_curForwardSpeed[i] = GetNextPlaySpeed(true,i);
				}
				else if (PLAYBACK_REW == m_currStatus[i])
				{
					//如果正在快退，则拦截消息，正在快进时不响应快退。
					return false;
				}
				else
				{
					//播放或者暂停，则从2倍速开始
					m_curForwardSpeed[i] = PLAY_SPEED_2X;
				}
			}
			FastForwardStatus();
		}
		else
		{
			for (int i = 0; i < g_p_login_succ->productInfo.localVideoInputNum; i++)
			{
				if (PLAYBACK_REW == m_currStatus[i])
				{
					//如果正在快退，则换个倍速
					m_curRewardSpeed[i] = GetNextPlaySpeed(false,i);
				}
				else if (PLAYBACK_FF == m_currStatus[i])
				{
					//如果正在快退，则拦截消息，正在快退时不响应快进。
					return false;
				}
				else
				{
					//播放或者暂停，则从2倍速开始
					m_curRewardSpeed[i] = PLAY_SPEED_8X;
				}
			}
			RewardStatus();
		}
	}
	else
	{
		if (bForward)
		{
			if (PLAYBACK_FF == m_currStatus[m_curRedSelectChnn])
			{
				//如果正在快进，则换个倍速
				m_curForwardSpeed[m_curRedSelectChnn] = GetNextPlaySpeed(true,m_curRedSelectChnn);
			}
			else if (PLAYBACK_REW == m_currStatus[m_curRedSelectChnn])
			{
				//如果正在快退，则拦截消息，正在快进时不响应快退。
				return false;
			}
			else
			{
				//播放或者暂停，则从2倍速开始
				m_curForwardSpeed[m_curRedSelectChnn] = PLAY_SPEED_2X;
			}

			FastForwardStatus();
		}
		else
		{
			if (PLAYBACK_REW == m_currStatus[m_curRedSelectChnn])
			{
				//如果正在快退，则换个倍速
				m_curRewardSpeed[m_curRedSelectChnn] = GetNextPlaySpeed(false,m_curRedSelectChnn);
			}
			else if (PLAYBACK_FF == m_currStatus[m_curRedSelectChnn])
			{
				//如果正在快退，则拦截消息，正在快退时不响应快进。
				return false;
			}
			else
			{
				//播放或者暂停，则从2倍速开始
				m_curRewardSpeed[m_curRedSelectChnn] = PLAY_SPEED_8X;
			}

			RewardStatus();
		}
	}
	

	return true;
#endif
}


bool CPlayerDlg::SetSpeed(PLAY_SPEED playSpeed, bool bForward)
{
#if !defined(__ASYN_PLAY__)
	if (bForward)
	{
		if (PLAYBACK_REW == m_currStatus && PLAY_SPEED_1X != playSpeed)
		{
			//如果正在快退并且不是要改变到正常播放，则拦截消息，正在快进时不响应快退。
			return false;
		}
		else
		{
			m_curForwardSpeed = playSpeed;
		}

		FastForwardStatus();
	}
	else
	{
		if (PLAYBACK_FF == m_currStatus)
		{
			//如果正在快进，则拦截消息，正在快进时不响应快退消息。
			return false;
		}
		else
		{
			m_curRewardSpeed = playSpeed;
		}

		RewardStatus();
	}

	return true;

#else
	if (bForward)
	{
		if (PLAYBACK_REW == m_currStatus[m_curRedSelectChnn] && PLAY_SPEED_1X != playSpeed)
		{
			//如果正在快退并且不是要改变到正常播放，则拦截消息，正在快进时不响应快退。
			return false;
		}
		else
		{
			m_curForwardSpeed[m_curRedSelectChnn] = playSpeed;
		}

		FastForwardStatus();
	}
	else
	{
		if (PLAYBACK_FF == m_currStatus[m_curRedSelectChnn])
		{
			//如果正在快进，则拦截消息，正在快进时不响应快退消息。
			return false;
		}
		else
		{
			m_curRewardSpeed[m_curRedSelectChnn] = playSpeed;
		}

		RewardStatus();
	}

	return true;
#endif
}

unsigned long CPlayerDlg::GetCurSpeed(bool bForward)
{
#if !defined(__ASYN_PLAY__)
	if (bForward)
	{
		return m_curForwardSpeed;
	}
	else
	{
		return m_curRewardSpeed;
	}
#else
	if (bForward)
	{
		return m_curForwardSpeed[m_curRedSelectChnn];
	}
	else
	{
		return m_curRewardSpeed[m_curRedSelectChnn];
	}
#endif
}

bool CPlayerDlg::ChangeCH(unsigned long keyID)
{
	if (KEY_VIRTUAL_1X1 == keyID)
	{
		return OnChangeCH(m_1X1CH, PLAY_CH_ONE, 1);
	}
	else if (KEY_VIRTUAL_2X2 == keyID)
	{
		return OnChangeCH(m_2X2CH, PLAY_CH_FOUR, 4);
	}
	else if (KEY_VIRTUAL_2X3 == keyID)
	{
		return OnChangeCH(m_2X3CH, PLAY_CH_SIX, 6);
	}
	else if (KEY_VIRTUAL_3X3 == keyID)
	{
		return OnChangeCH(m_3X3CH, PLAY_CH_NINE, 9);
	}
	else if (KEY_VIRTUAL_4X4 == keyID)
	{
		return OnChangeCH(m_4X4CH, PLAY_CH_SIXTEEN, 16);
	}
	else if(MESSAGE_VIRTUAL_SPLIT_5 == keyID)
	{
		return OnChangeCH(m_1A5CH, PLAY_CH_SIX, 6);
	}
	else
	{
		return OnChangeCH(m_1X1CH, PLAY_CH_ONE, 1);
	}
}

void CPlayerDlg::SetSplitBtnEnable(bool bEnable, bool bRefresh/* = true*/)
{
	m_btnOneSplit.SetEnable(bEnable, bRefresh);
	m_btnFourSplit.SetEnable(bEnable, bRefresh);
	m_btnSixSplit.SetEnable(bEnable, bRefresh);
	m_btnNineSplit.SetEnable(bEnable, bRefresh);
	m_btnSixteenSplit.SetEnable(bEnable, bRefresh);
	m_btnSpecSplit.SetEnable(bEnable, bRefresh);

	m_btn1X1Ex.SetEnable(bEnable, bRefresh);
	m_btn2X2Ex.SetEnable(bEnable, bRefresh);
	m_btn2X3Ex.SetEnable(bEnable, bRefresh);
	m_btn3X3Ex.SetEnable(bEnable, bRefresh);
	//m_btn1A5Ex.SetEnable(bEnable, bRefresh);
}

void CPlayerDlg::RefreshAudioBtn(bool bRefresh/* = true*/)
{
	if (0 != m_selAudioCH)
	{
		//非无效音频通道组合，则设置为不静音
		m_btnAudio.SetCurState(AUDIO_OPEN, false);

		//如果为不使能，则应该设为使能
		if (!m_btnAudio.IsEnable())
		{
			m_btnAudio.SetEnable(true, false);
		}
	}
	else
	{
		//非无效音频通道组合，则设置为不静音
		m_btnAudio.SetCurState(AUDIO_MUTE, false);

		if (m_btnAudio.IsEnable())
		{
			m_btnAudio.SetEnable(false, false);
		}
	}

	if (bRefresh)
	{
		m_btnAudio.Repaint();
	}
}

void CPlayerDlg::FindPlayStausName(bool bForwardList, unsigned long speed, char *szRetName)
{
	PLAYER_CTRL_SPEED_LIST * pSpeedList = NULL;
	if (bForwardList)
	{
		pSpeedList = m_pCtrlSpeedForwardList;
	}
	else
	{
		pSpeedList = m_pCtrlSpeedRewardList;
	}

	if (NULL != pSpeedList)
	{
		POS pos = pSpeedList->GetHeadPosition();
		while (NULL != pos)
		{
			PLAYER_CTRL_SPEED_ITEM &speeditem = pSpeedList->GetAt(pos);
			if (speeditem.speed == speed)
			{
				sprintf(szRetName, "%s", speeditem.szSpeedName);
				printf("@@@@@@@@@@  SzRetName=%s\n",szRetName);
				return;
			}				
			pSpeedList->GetNext(pos);
		}	
	}

	if (bForwardList)
	{
		sprintf(szRetName, ">>");
	}
	else
	{
		sprintf(szRetName, "<<");
	}
	return;	
}

/******************************************************************************************************************

函数：	GetNextPlaySpeed( bool bforward )
功能：	获得快进当前播放速度的下一个值
参数：
		1：	bforward = true 表示寻找快进的速度
			bforward = false 表示寻找快退的速度
		2: int chnnNum  通道号---同步的时候值为对应通道值，异步时候值为m_curRedSelectChnn；
返回值：
		
*******************************************************************************************************************/
unsigned long CPlayerDlg::GetNextPlaySpeed( bool bforward,int chnnNum)
{
	PLAYER_CTRL_SPEED_LIST * pSpeedList = NULL;
	unsigned long playSpeed = 0;

#if !defined(__ASYN_PLAY__)

	if (bforward)
	{
		pSpeedList = m_pCtrlSpeedForwardList;
		playSpeed  = m_curForwardSpeed;
	}
	else
	{
		pSpeedList = m_pCtrlSpeedRewardList;
		playSpeed  = m_curRewardSpeed; 
	}

#else
		if (bforward)
		{
			pSpeedList = m_pCtrlSpeedForwardList;
			playSpeed  = m_curForwardSpeed[chnnNum];
		}
		else
		{
			pSpeedList = m_pCtrlSpeedRewardList;
			playSpeed  = m_curRewardSpeed[chnnNum]; 
		}
#endif

	if ( ( NULL == pSpeedList ) || ( 0 == pSpeedList->GetCount() ) )
	{
		//快进和快退的链表都不能为空
		assert( false );
		return -1;
	}

	POS pos = pSpeedList->GetTailPosition( );
	assert( NULL != pos );

	while( NULL != pos )	
	{
		PLAYER_CTRL_SPEED_ITEM &speeditem = pSpeedList->GetPrev( pos );
		if ( speeditem.speed == playSpeed )
		{
			break;
		}				
	}

	if( ( NULL == pos ) && ( NULL != pSpeedList->GetTailPosition( ) ) )
	{
		//要找的是结尾的数据
		pos = pSpeedList->GetTailPosition( );
	}
	else if( NULL == pSpeedList->GetTailPosition( ) )
	{
		// pSpeedList为空时
		assert( false );
		return -1;
	}

	return pSpeedList->GetAt( pos ).speed;
}

void CPlayerDlg::SetAudioCH()
{
	assert(m_playAudioMaxChnnNum > 0);	

	//如果切换了视频通道，则需要把音频通道做一次刷新检查。
	ULONGLONG  selAudioCHRange = (m_selCuPlayrChnn & m_audioChnnMask);
	if (selAudioCHRange > 0)
	{
		if (0 == (m_selAudioCH & selAudioCHRange))
		{
			for (unsigned char i=0; i < m_maxChnnNum; ++i)
			{
				if ( ((ULONGLONG)(0x01) << i) & (selAudioCHRange))
				{
					m_selAudioCH = (ULONGLONG)(0x01) << i;
					break;								
				}
			}
		}
	}
	else
	{
		m_selAudioCH = selAudioCHRange;
	}

	RefreshAudioBtn(false);
}

bool CPlayerDlg::NextSplit()
{
	if( KEY_VIRTUAL_SPLIT == OnSplit() )
	{
		return true;
	}
	else
	{
		return false;
	}
}    
bool CPlayerDlg::NextGroup()
{
	if( KEY_VIRTUAL_GROUP == OnGroup() )
	{
		return true;
	}
	else
	{
		return false;
	}
	
}
bool CPlayerDlg::OnChangeCH(ULONGLONG & CH, PLAY_CH_MODE splitMode, unsigned long n)
{
	if (n > m_maxSelChnn)
	{
		n = m_maxSelChnn;
	}

	unsigned long ipcMaxVideoOutNum = CProduct::Instance()->IPCMaxVideoOutNum();
	unsigned long channelNum = g_p_login_succ->productInfo.localVideoInputNum;
	if( ipcMaxVideoOutNum && channelNum )
	{
		if(((PLAY_CH_ONE == splitMode) && (0 < ipcMaxVideoOutNum)) || ((PLAY_CH_FOUR == splitMode) && (4 <= ipcMaxVideoOutNum)))
		{
			channelNum += g_p_login_succ->productInfo.netVideoInputNum;
		}
	}
	else
	{
		if(n <= ipcMaxVideoOutNum)
		{
			channelNum += g_p_login_succ->productInfo.netVideoInputNum;
		}
	}

	if ( m_bChangePlayChnn && (m_maxSplitMode >= splitMode))
	{
		assert (0 != g_p_login_succ->authPlaybackCH);

		ULONGLONG chnnBits = CH;

		if (m_currSplitMode == splitMode)
		{
			do 
			{
				if (0 == chnnBits)
				{
					chnnBits = ALL_CHNN_MASK(n);
				}
				else
				{
					chnnBits = CH_ROL(chnnBits, ALL_CHNN_MASK(channelNum), ALL_CHNN_MASK(n), n);
					chnnBits &= ((ULONGLONG)1<<channelNum)-1;
				}
			} while(0 == (chnnBits & g_p_login_succ->authPlaybackCH));	
		}
		else
		{
			if (TD_2708AS_PL != CProduct::Instance()->ProductType())
			{
				while(0 == (chnnBits & g_p_login_succ->authPlaybackCH))    //0x2  -> 0x01ff
				{
					if (0 == chnnBits)
					{
						chnnBits = ALL_CHNN_MASK(n);
					}
					else
					{
						chnnBits = CH_ROL(chnnBits, ALL_CHNN_MASK(channelNum), ALL_CHNN_MASK(n), n);
						chnnBits &= ((ULONGLONG)1<<channelNum)-1;
					}
				}
			}
			else
			{
				if (PLAY_CH_ONE == splitMode)
				{
					chnnBits = 0x1;
				}
				else if (PLAY_CH_FOUR == splitMode)
				{
					chnnBits = 0xf;
				}
				else if (PLAY_CH_SIX == splitMode)
				{
					chnnBits = 0x3f;
				}
				else if (PLAY_CH_EIGHT == splitMode)
				{
					chnnBits = 0xff;  
				}
				else if (PLAY_CH_NINE == splitMode)
				{
					chnnBits = 0xff;
				}	
			}

			m_currSplitMode = splitMode;
		}

		if ((chnnBits & g_p_login_succ->authPlaybackCH) != m_selCuPlayrChnn)
		{
			m_selCuPlayrChnn = (chnnBits & g_p_login_succ->authPlaybackCH);
			CH = chnnBits;

			SetAudioCH();
			GetRootWnd()->Repaint();

			return true;
		}
		else
		{
#if !defined(__ASYN_PLAY__)
				//相同分割数但是分割方法不一样
				m_selCuPlayrChnn = (chnnBits & g_p_login_succ->authPlaybackCH);
				CH = chnnBits;
				return true;
#else
			 if(m_spectype!=SPEC_NONE)
			 {
				 //相同分割数但是分割方法不一样
				 m_selCuPlayrChnn = (chnnBits & g_p_login_succ->authPlaybackCH);
				 CH = chnnBits;
				 return true;
			 }
#endif
		}
	}

	return false;
}

ULONGLONG CPlayerDlg::GetCurPlayChnn(unsigned long & splitMode,int type)
{
	splitMode = SplitPBToLive(m_currSplitMode,type); 
	return m_selCuPlayrChnn;
}

unsigned long CPlayerDlg::SplitPBToLive(PLAY_CH_MODE mode,int type)
{
	if (PLAY_CH_ONE == mode)
	{
		return VIEW_SPLIT_1X1;
	}
	else if (PLAY_CH_FOUR == mode)
	{
		return VIEW_SPLIT_2X2;
	}
	else if (PLAY_CH_SIX == mode)
	{
		if(type == SPEC_NONE)
		{
			
			return VIEW_SPLIT_2X3;
		}
		else if(type == SPEC_1A5)
		{
		
			return VIEW_SPLIT_1A5;
		}
	}
	else if(PLAY_CH_EIGHT == mode)
	{
		
		return VIEW_SPLIT_1A7;
	}
	else if (PLAY_CH_NINE == mode)
	{
		if(type == SPEC_NONE)
		{
			return VIEW_SPLIT_3X3;
		}
		else if(type == SPEC_1A7)
		{
			return VIEW_SPLIT_1A7;
		}
	}
	else if(PLAY_CH_THIRTEEN == mode)
	{
		
		return VIEW_SPLIT_1A12;
	}
	else if (PLAY_CH_SIXTEEN == mode)
	{
		return VIEW_SPLIT_4X4;
	}
	else
	{
		return VIEW_SPLIT_1X1;
	}
}

//////////////////////////////////////////////////////////////////////////
// 窗口布局
//////////////////////////////////////////////////////////////////////////

void CPlayerDlg::OnEnter(unsigned long xPos, unsigned long yPos)
{

}

void CPlayerDlg::OnMouseMove(unsigned long xPos, unsigned long yPos)
{
	m_x_pos_last = m_x_pos;
	m_y_pos_last = m_y_pos;

	m_x_pos = xPos;
	m_y_pos = yPos;
}

GUI::CRect CPlayerDlg::OnMouseDrag(unsigned long xPos, unsigned long yPos, bool bRefresh)
{
	GUI::CRect retRect(0, 0, 0, 0);

	if(m_bDrag)
	{
		if(m_bFirstDrag)
		{
			m_x_drag_last = m_x_enter;
			m_y_drag_last = m_y_enter;

			m_bFirstDrag = false;
		}

		GUI::CRect oldRect = m_wndLayout.wndRect;

		unsigned long offset_len = ((m_width / 50) * (m_width / 50)) + ((m_height / 50) * (m_height / 50));
		//unsigned long offset_len = 200;
		if(!IsAlmostEqual(m_x_drag_last, m_y_drag_last, xPos, yPos, offset_len))
		{
			if(MovePlayToolWnd(xPos, yPos))
			{
				retRect = oldRect.UnionRect(m_wndLayout.wndRect);

				m_x_drag_last = xPos;
				m_y_drag_last = yPos;
			}

		}
	}

	return retRect;
}

unsigned long CPlayerDlg::OnMouseLeftUp()
{
	if(m_bDrag)
	{
		m_bDrag = false;
		m_bFirstDrag = false;

		return KEY_VIRTUAL_MSG_CONTINUE;
	}

	return KEY_VIRTUAL_MSG_STOP;
}

void CPlayerDlg::AutoLayout(bool bRefresh, int xPos, int yPos)
{
	if(xPos < 0 || yPos < 0)
	{
		//默认位置, 最下排
		m_wndLayout.state = 0;
		m_wndLayout.wndRect.SetRect(m_wndLayout.playToolRect.m_left, m_wndLayout.playToolRect.m_bottom - m_wndLayout.minHeight, m_wndLayout.playToolRect.m_right, m_wndLayout.playToolRect.m_bottom);
	}
	else if(yPos < m_wndLayout.playToolRect.m_top + 10)
	{
		//最上面
		m_wndLayout.state = 1;
		m_wndLayout.wndRect.SetRect(m_wndLayout.playToolRect.m_left, m_wndLayout.playToolRect.m_top, m_wndLayout.playToolRect.m_right, m_wndLayout.playToolRect.m_top + m_wndLayout.minHeight);
	}
	else if(yPos + m_wndLayout.minHeight + 10 > m_wndLayout.playToolRect.m_bottom)
	{
		//最下排
		m_wndLayout.state = 0;
		m_wndLayout.wndRect.SetRect(m_wndLayout.playToolRect.m_left, m_wndLayout.playToolRect.m_bottom - m_wndLayout.minHeight, m_wndLayout.playToolRect.m_right, m_wndLayout.playToolRect.m_bottom);
	}
	else
	{
		m_wndLayout.state = 2;

		int width = m_wndLayout.btnMaxWidth + m_wndLayout.rightFixWidth + m_ckShowName.GetWindowRect().Width();

		if(width > m_wndLayout.playToolRect.Width())
		{
			width = m_wndLayout.btnMaxWidth + m_wndLayout.rightFixWidth;
			if(width > m_wndLayout.playToolRect.Width())
			{
				width = m_wndLayout.playToolRect.Width();
			}
		}

		m_wndLayout.wndRect.SetRect(xPos, yPos, xPos + width, yPos + m_wndLayout.minHeight);
	}

	SetWndPos(m_wndLayout.wndRect.m_left, m_wndLayout.wndRect.m_top, m_wndLayout.wndRect.Width(), m_wndLayout.wndRect.Height());

	LayoutSubWnd();
}

bool CPlayerDlg::MovePlayToolWnd(int x, int y)
{
	int x_left = 0, y_top = 0;

	x_left = (x + m_wndLayout.offsetRight - m_wndLayout.wndRect.Width());
	y_top = (y - m_wndLayout.offsetTop);

	if(y_top < m_wndLayout.playToolRect.m_top + 10)
	{
		//顶
		y_top = m_wndLayout.playToolRect.m_top;

		AutoLayout(false, m_wndLayout.playToolRect.m_left, y_top);

		return true;
	}

	if(y_top + m_wndLayout.wndRect.Height() + 10 > m_wndLayout.playToolRect.m_bottom)
	{
		//底
		y_top = m_wndLayout.playToolRect.m_bottom - m_wndLayout.wndRect.Height();

		AutoLayout(false, m_wndLayout.playToolRect.m_left, y_top);

		return true;
	}

	if(m_wndLayout.state == 2)
	{
		if(x_left + m_wndLayout.wndRect.Width() + 10 > m_wndLayout.playToolRect.m_right)
		{
			x_left = m_wndLayout.playToolRect.m_right - m_wndLayout.wndRect.Width();
		}

		if(x_left < m_wndLayout.playToolRect.m_left + 10)
		{
			x_left = m_wndLayout.playToolRect.m_left;
		}


		m_wndLayout.wndRect.MoveToXY(x_left, y_top);
		MoveWnd(x_left, y_top);
	}
	else
	{
		AutoLayout(false, m_wndLayout.playToolRect.m_top, y_top);

		if(m_wndLayout.state == 2)
		{
			x_left = (x + m_wndLayout.offsetRight - m_wndLayout.wndRect.Width());
			if(x_left + m_wndLayout.wndRect.Width() + 10 > m_wndLayout.playToolRect.m_right)
			{
				x_left = m_wndLayout.playToolRect.m_right - m_wndLayout.wndRect.Width();
			}

			if(x_left < m_wndLayout.playToolRect.m_left + 10)
			{
				x_left = m_wndLayout.playToolRect.m_left;
			}

			m_wndLayout.wndRect.MoveToXY(x_left, y_top);
			MoveWnd(x_left, y_top);
		}
	}

	return true;
}

void CPlayerDlg::LayoutSubWnd()
{
	int x_left = m_rect.m_left + 5, y_top = m_rect.m_top + 5;
	int width = 24, height = 24;
	int offset_x = 6;

	int x = x_left, y = y_top;

	//按钮区
	for(int i = 0; i < m_btnCount; i++)
	{
		if(m_btnWndInfo[i].pWnd)
		{
			m_btnWndInfo[i].pWnd->MoveWnd(x, y + m_btnWndInfo[i].offsetTop);
		}

		x += m_btnWndInfo[i].width;
	}

	//右边固定按钮
	int btn_width = 30;
	x = m_rect.m_right - btn_width - 6;
	m_btnMove.MoveWnd(x, y);

	x = x - btn_width - 3;
	m_btnExit.MoveWnd(x, y);

	x = x - btn_width - 3;
	m_btnHide.MoveWnd(x, y);

	x = x - btn_width - 3;
	m_btnAudio.MoveWnd(x, y);

	if(m_width<=800)//分辨率小于800
	{
		//显示时间提示字符
		x = x_left + 3;
		y += height + 3;
		m_stSecStartTime.MoveWnd(x, y);

		const char* strTextStart = "8888/88/88 88:88:88";
		unsigned short strCxStart = 0, strCyStart =0;
		GUI::CWnd::TextSize(strTextStart, strlen(strTextStart), strCxStart, strCyStart);
		x+=strCxStart+4;
		m_btnPreSection.MoveWnd(x, y);

		x +=width+5;//m_btnPreSection长度
		const char* strTextEnd = "88:88:88";
		unsigned short strCxEnd = 0, strCyEnd =0;
		GUI::CWnd::TextSize(strTextEnd, strlen(strTextEnd), strCxEnd, strCyEnd);
		unsigned long sliderWidth=m_rect.Width()-x-width-strCxEnd-12;
		m_sliderCtrl.SetWndPos(x-x_left+5, height + 7 + (height - m_length[SLIDER_WIDTH]) / 2,sliderWidth, m_length[SLIDER_WIDTH]);

		x += sliderWidth;
		m_btnNextSection.MoveWnd(x, y);

		//显示末尾提示时间
		x+=width+3;
		m_stSecEndTime.MoveWnd(x, y);

		////显示最后一行的状态和通道名

		y += height;

		//cvbs把剪切和备份按钮放到第三行
#if defined(__NEW_CUT_BACKUP__)
		 if((m_width<800)&&(m_maxChnnNum >= 16))
		{
			x=x_left+width;
			 m_btnGetBackupStartTime.MoveWnd(x,y);
			x+=width+5;
			m_btnGetBackupEndTime.MoveWnd(x,y);
			x+=width+5;
			m_btnbackup.MoveWnd(x,y);
		}
#else
		if((m_width<800)&&(m_maxChnnNum >= 16))
		{
			x=x_left+width;
			m_btnGetBackupTime.MoveWnd(x,y);
			x+=width+5;
			m_btnbackup.MoveWnd(x,y);
		}
#endif
			
#if !defined(__ASYN_PLAY__)
		x = x_left + (m_rect.Width() -m_scTime.GetClientRect().Width()- m_stPlayStatus.GetClientRect().Width() - 6) / 2-50;
#else
		x=x_left+m_scTime.GetClientRect().Width()+10;
#endif
		m_stPlayStatus.MoveWnd(x, y);

#if !defined(__ASYN_PLAY__)
		int offset=x+m_length[BTN_HEIGHT]+35;
		m_tip.MoveWnd(offset,y);

		x = x - m_scTime.GetClientRect().Width() - 3;
		m_scTime.MoveWnd(x, y);

#else
		x=x_left+m_scTime.GetClientRect().Width()+10+m_stPlayStatus.GetClientRect().Width()+10;
		m_tip.MoveWnd(x,y);

		int offset=x_left+5;
		m_scTime.MoveWnd(offset, y);
#endif

		const char* strtip = "88:88:88 PM-88:88:88 PM";
		unsigned short tipCxStart = 0, tipCyStart =0;
		GUI::CWnd::TextSize(strtip, strlen(strtip), tipCxStart, tipCyStart);
		GUI::CRect rect = m_stPlayStatus.GetWindowRect();
		
#if !defined(__ASYN_PLAY__)
		m_ckShowName.MoveWnd(rect.m_right+tipCxStart,y);
#else
		m_ckShowName.MoveWnd(rect.m_right+tipCxStart+25,y);
		m_ckSyncPlayback.MoveWnd(rect.m_right+tipCxStart+25+m_ckShowName.GetClientRect().Width()+15, y);
#endif		

	}
	else
	{
		//拖动条
		y += height + 3;
		x = x_left + 3;
		m_btnPreSection.MoveWnd(x, y);

		x += width + 3;
		m_sliderCtrl.SetWndPos(width + 8, height + 7 + (height - m_length[SLIDER_WIDTH]) / 2, m_rect.Width() - (width + 8) * 2, m_length[SLIDER_WIDTH]);

		x = m_rect.m_right - width - 6;
		m_btnNextSection.MoveWnd(x, y);

		//字符提示
		y += height + 3;
		x = x_left + 2;

		m_stSecStartTime.MoveWnd(x, y);

		x = m_rect.m_right - m_stSecEndTime.GetClientRect().Width() - 7;
		m_stSecEndTime.MoveWnd(x, y);
		
		int scTimeX = x_left+m_stSecStartTime.GetClientRect().Width()+15;
		m_scTime.MoveWnd(scTimeX, y);
		
		x=x_left+m_scTime.GetClientRect().Width()+m_stSecStartTime.GetClientRect().Width()+25;
		m_stPlayStatus.MoveWnd(x, y);

		x += m_stPlayStatus.GetClientRect().Width()+10;
		m_tip.MoveWnd(x,y);
#if !defined(__ASYN_PLAY__)
		//int offset=x+m_length[BTN_HEIGHT]+35;
		//m_tip.MoveWnd(offset,y);
		//x = x - m_scTime.GetClientRect().Width() - 6;
		//m_scTime.MoveWnd(x, y);

		//显示通道名
		if(m_wndLayout.btnMaxWidth + m_wndLayout.rightFixWidth + m_ckShowName.GetClientRect().Width() < m_wndLayout.wndRect.Width())
		{
			
			m_ckShowName.MoveWnd(x_left + m_wndLayout.btnMaxWidth + 3, y_top + 2);
		}
		else
		{
			GUI::CRect rect = m_stPlayStatus.GetWindowRect();
			m_ckShowName.MoveWnd(rect.m_right, rect.m_top);
		}
#else
		//int offset=x+m_length[BTN_HEIGHT]+35;
	//	int tipoffset=x;
		//m_tip.MoveWnd(offset,y);
	//	x = x - m_scTime.GetClientRect().Width() - 6;
	//	m_scTime.MoveWnd(x, y);

		//显示通道名
		if(m_wndLayout.btnMaxWidth + m_wndLayout.rightFixWidth + m_ckShowName.GetClientRect().Width()< m_wndLayout.wndRect.Width())
		{
			m_ckShowName.MoveWnd(x_left + m_wndLayout.btnMaxWidth + 3, y_top + 2);
			char* strtip = "88:88:88 PM-88:88:88 PM";
			unsigned short tipCxStart = 0, tipCyStart =0;
			GUI::CWnd::TextSize(strtip, strlen(strtip), tipCxStart, tipCyStart);
			m_ckSyncPlayback.MoveWnd(x+tipCxStart+m_length[CHECK_WIDTH],y);
		
		}
		else
		{
			char* strtip = "Async Play";
			unsigned short tipCxStart = 0, tipCyStart =0;
			GUI::CWnd::TextSize(strtip, strlen(strtip), tipCxStart, tipCyStart);
			GUI::CRect rect = m_stPlayStatus.GetWindowRect();
			m_ckShowName.MoveWnd(rect.m_right, rect.m_top);
			m_ckSyncPlayback.MoveWnd(rect.m_right-tipCxStart-10, y);
			
		}
		
#endif
	}
	
}

void CPlayerDlg::SetBtnWndInfo(GUI::CWnd* pWnd, int index, unsigned short width, unsigned short height, unsigned short offsetTop)
{
	gui_assert(index < (sizeof(m_btnWndInfo) / sizeof(m_btnWndInfo[0])), printf("param error.index = %d.\n", index));
	
	m_btnWndInfo[index].pWnd = pWnd;
	m_btnWndInfo[index].width = width;
	m_btnWndInfo[index].height = height;
	m_btnWndInfo[index].offsetTop = offsetTop;
}

bool CPlayerDlg::PtInPlayToolBar(int xPos, int yPos)
{
	return m_wndLayout.wndRect.PtInRect(xPos, yPos);
}

bool CPlayerDlg::IsAlmostEqual(unsigned long x1, unsigned long y1, unsigned long x2, unsigned long y2, unsigned long distance)
{
	unsigned long x = (x1 >= x2) ? (x1-x2) : (x2-x1);
	unsigned long y = (y1 >= y2) ? (y1-y2) : (y2-y1);

	if(((x*x) + (y*y)) <= distance)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool CPlayerDlg::IsAlmostAbove(unsigned long x1, unsigned long y1, unsigned long x2, unsigned long y2, unsigned long distance)
{
	unsigned long x = (x1>=x2)?(x1-x2):(x2-x1);
	unsigned long y = (y1>=y2)?(y1-y2):(y2-y1);

	if(((x*x) + (y*y)) >= distance)
	{
		return true;
	}
	else
	{
		return false;
	}
}

#if defined(__NEW_CUT_BACKUP__)
#if !defined(__ASYN_PLAY__)
unsigned long CPlayerDlg::OnGetSliderStartTime()
{
	if((m_currStatus==PLAYBACK_PLAYING)||(m_currStatus==PLAYBACK_FF)||(m_currStatus==PLAYBACK_REW)||(m_currStatus==PLAYBACK_PAUSE))
	{		
		printf("xxxxxxxxxxxxxxxx%s,%d,m_maxChnnNum:%d,m_curRedSelectChnn:%d\n",__FILE__,__LINE__,m_maxChnnNum,m_curRedSelectChnn);
		if ((m_curRedSelectChnn >= 0) && (m_curRedSelectChnn < m_maxChnnNum))
		{
			//获取时间
			unsigned long slidermin = m_sliderCtrl.GetRangeMin();
			unsigned long slidermax = m_sliderCtrl.GetRangeMax();
			unsigned long slidercur = m_sliderCtrl.GetPos();
			unsigned long sliderlen = slidermax - slidermin;
			unsigned long curlen    = slidercur - slidermin;
			unsigned long time      = m_sliderEndTime - m_sliderStartTime;
			unsigned long tmptime   = curlen*time/sliderlen;
			unsigned long curtime   = tmptime + m_sliderStartTime;

			m_startBackTime = curtime;

			//转化后字符提示显示在界面上,a按键要变图像
			m_btnGetBackupEndTime.SetEnable(true,false);

			string stm1;		
			CTimeCtrl::GetTime32Text(stm1,m_startBackTime,CTimeCtrl::SECTION_HOUR|CTimeCtrl::SECTION_MIN|CTimeCtrl::SECTION_SEC);	
			m_tip.SetCaption(stm1,false);
			m_tip.Show(true,false);
		}
		else
		{
			CleanCutBackup();
		}
	}
	Repaint();
	return KEY_VIRTUAL_MSG_STOP;

}
unsigned long CPlayerDlg::OnGetSliderEndTime()
{
	m_btnGetBackupEndTime.SetEnable(false,false);
	if((m_currStatus==PLAYBACK_PLAYING)||(m_currStatus==PLAYBACK_FF)||(m_currStatus==PLAYBACK_REW)||(m_currStatus==PLAYBACK_PAUSE))
	{
		if ((m_curRedSelectChnn >= 0) && (m_curRedSelectChnn < m_maxChnnNum))
		{		
			//获取时间
			unsigned long slidermin,slidermax,slidercur;
			slidermin=m_sliderCtrl.GetRangeMin();
			slidermax=m_sliderCtrl.GetRangeMax();
			slidercur=m_sliderCtrl.GetPos();
			unsigned long sliderlen=slidermax-slidermin;
			unsigned long curlen=slidercur-slidermin;
			unsigned long time=m_sliderEndTime-m_sliderStartTime;
			unsigned long tmptime=curlen*time/sliderlen;
			unsigned long curtime=tmptime+m_sliderStartTime;
			if(curtime<=m_startBackTime)
			{
				//滑条往后拉，交换开始时间和结束时间
				m_endBackTime=m_startBackTime;
				m_startBackTime=curtime;
			}
			else
			{
				m_endBackTime=curtime;
			}
			
			//结束时间与开始时间间隔小于1分钟（60s）,补齐一分钟
			if(m_endBackTime-m_startBackTime<60)
			{
				m_endBackTime=m_startBackTime+60;
			}
			
			if (m_endBackTime > GetCurrTime32())
			{
				printf("%s,%d,endTime big CurrTime\n",__FILE__,__LINE__);
				CleanCutBackup();
			}
			else
			{
				string stm1,stm2,sum;
				stm1.clear();
				CTimeCtrl::GetTime32Text(stm1,m_startBackTime,CTimeCtrl::SECTION_HOUR|CTimeCtrl::SECTION_MIN|CTimeCtrl::SECTION_SEC);
				stm2.clear();
				CTimeCtrl::GetTime32Text(stm2,m_endBackTime,CTimeCtrl::SECTION_HOUR|CTimeCtrl::SECTION_MIN|CTimeCtrl::SECTION_SEC);
				sum.clear();
				sum=stm1+"-"+stm2;
				m_tip.SetCaption(sum,false);
				m_tip.Show(true,false);
				
				//将获得的时间发送消息获取文件信息存入m_searchDataVector
				MESSAGE_DATA msgData;
				NET_DATA_SEARCH searchCond;
				searchCond.searchType  = DATA_SEARCH_TYPE_FILE;
				searchCond.videoCH     = (ULONGLONG)0x01 << m_curRedSelectChnn; 
				searchCond.audioCH     = 0;
				searchCond.startTime   = m_startBackTime;
				searchCond.endTime     = m_endBackTime;
				searchCond.dataOwnerby = m_diskOwnerIndex;
				PACKCMD *pCmd = NULL;
				memset(&msgData, 0, sizeof(MESSAGE_DATA));
				CMessageMan::Instance()->SendMsgDataToServer(0, LOCAL_DEVICE_ID, CMD_REQUEST_DATA_SEARCH, (unsigned char *)&searchCond, sizeof(NET_DATA_SEARCH), msgData);
				assert ((sizeof(PACKCMD) + sizeof(NET_DATA_INFO)) <= msgData.dataLen);
				FILE_INFO_EX *pDate = (FILE_INFO_EX *)(msgData.pData + sizeof(PACKCMD) + sizeof(NET_DATA_INFO));
				int num = (msgData.dataLen - sizeof(PACKCMD) - sizeof(NET_DATA_INFO))/sizeof(FILE_INFO_EX);
				printf("search time:\n");
				PrintfBit32Time(m_startBackTime);
				PrintfBit32Time(m_endBackTime);
				printf("####%s,%d,num:%d\n",__FILE__,__LINE__,num);
				if (num > 0)
				{					
					for (int count = num - 1; count >= 0; --count)
					{
						BACKUP_DATA_SEQ     backupTmp;
						memset(&backupTmp, 0, sizeof(BACKUP_DATA_SEQ));
						backupTmp.changeTime=GetCurrTime32();
						backupTmp.hasCheck = 0;
						backupTmp.seq = MAX_BACKUP_SEQ;
						backupTmp.hasCalculate = 0;
						backupTmp.backupinfo.fileInfo = pDate[count];
						if(backupTmp.backupinfo.fileInfo.startTime<m_startBackTime)
						{
							backupTmp.backupinfo.fileInfo.startTime=m_startBackTime;
						}
						if(backupTmp.backupinfo.fileInfo.endTime>m_endBackTime)
						{
							backupTmp.backupinfo.fileInfo.endTime=m_endBackTime;
						}
						backupTmp.backupinfo.hasBackupAudio = 0;
						backupTmp.hasAudio = 0;
						printf("##count:%d,chn:%d,fileDisk:%d,fileIndex:%d,time:",count,backupTmp.backupinfo.fileInfo.chnn,backupTmp.backupinfo.fileInfo.diskIndex,backupTmp.backupinfo.fileInfo.fileIndex);
						PrintfBit32Time(backupTmp.backupinfo.fileInfo.startTime);
						
						m_searchDataVector.push_back(backupTmp);
					}
				}
				CMessageMan::Instance()->ReleaseMsgBuff(msgData);
				//刷新现场
				CMessageMan::Instance()->RefreshLiveTrue();
			}
		}
		else
		{
			CleanCutBackup();
		}		
	}
	Repaint();
	return KEY_VIRTUAL_MSG_STOP;
}
unsigned long CPlayerDlg::BackupReplay()
{	
	//开始备份前暂停播放
	int returnStatus = m_currStatus;
	if ((PLAYBACK_PLAYING == returnStatus) || (PLAYBACK_FF == returnStatus) || (PLAYBACK_REW == returnStatus))
	{
		PauseStatus();
		MESSAGE_DATA msgData;
		memset(&msgData, 0, sizeof(MESSAGE_DATA));
		CMessageMan::Instance()->SendMsgDataToServer(0, LOCAL_DEVICE_ID, CMD_REQUEST_LOCAL_CLIENT_PLAYBACK_PAUSE, NULL, 0, msgData);
		CMessageMan::Instance()->ReleaseMsgBuff(msgData);
	}
	
	ULONGLONG auth = CUserMan::Instance()->CheckAuthority(AUTH_BACKUP);
	for(vector<BACKUP_DATA_SEQ>::iterator it = m_searchDataVector.begin();it != m_searchDataVector.end();)
	{
		if(0 == (auth & (0x01 <<(it->backupinfo.fileInfo.chnn))))
		{
			it = m_searchDataVector.erase(it);
		}
		else
		{
			++it;
		}
	}
	CPreBackUpDlg preBackupDlg;
	preBackupDlg.SetSEARCHData(&m_searchDataVector);
	preBackupDlg.DoModal();
	//备份完毕后把按钮重新设置为不可用
	CleanCutBackup();

	if ((PLAYBACK_PLAYING == returnStatus) || (PLAYBACK_FF == returnStatus) || (PLAYBACK_REW == returnStatus))
	{	
		//回放恢复
		MESSAGE_DATA msgData;
		memset(&msgData, 0, sizeof(MESSAGE_DATA));
		CMessageMan::Instance()->SendMsgDataToServer(0, LOCAL_DEVICE_ID, CMD_REQUEST_LOCAL_CLIENT_PLAYBACK_RESUM, NULL, 0, msgData);
		CMessageMan::Instance()->ReleaseMsgBuff(msgData);
					
		if(returnStatus == PLAYBACK_PLAYING)
		{
			PlayStatus();
		}
		else if(returnStatus == PLAYBACK_FF)
		{
			memset(&msgData, 0, sizeof(MESSAGE_DATA));
			CMessageMan::Instance()->SendMsgDataToServer(0, LOCAL_DEVICE_ID, CMD_REQUEST_LOCAL_CLIENT_PLAYBACK_FF, (unsigned char*)(&m_curForwardSpeed), sizeof(m_curForwardSpeed), msgData);
			CMessageMan::Instance()->ReleaseMsgBuff(msgData);
			FastForwardStatus();
		}
		else if(returnStatus== PLAYBACK_REW)
		{
			memset(&msgData, 0, sizeof(MESSAGE_DATA));
			CMessageMan::Instance()->SendMsgDataToServer(0, LOCAL_DEVICE_ID, CMD_REQUEST_LOCAL_CLIENT_PLAYBACK_REW, (unsigned char*)(&m_curRewardSpeed), sizeof(m_curRewardSpeed), msgData);
			CMessageMan::Instance()->ReleaseMsgBuff(msgData);
			RewardStatus();
		}
		CMessageMan::Instance()->RefreshLiveTrue();
	}	
	
	Repaint();
	return KEY_VIRTUAL_MSG_STOP;
}
#else
unsigned long CPlayerDlg::OnGetSliderStartTime()
{
	if((m_currStatus[m_curRedSelectChnn]==PLAYBACK_PLAYING)||(m_currStatus[m_curRedSelectChnn]==PLAYBACK_FF)||(m_currStatus[m_curRedSelectChnn]==PLAYBACK_REW)||(m_currStatus[m_curRedSelectChnn]==PLAYBACK_PAUSE))
	{		
		printf("xxxxxxxxxxxxxxxx%s,%d,m_maxChnnNum:%d,m_curRedSelectChnn:%d\n",__FILE__,__LINE__,m_maxChnnNum,m_curRedSelectChnn);
		if ((m_curRedSelectChnn >= 0) && (m_curRedSelectChnn < m_maxChnnNum))
		{
			//获取时间
			unsigned long slidermin = m_sliderCtrl.GetRangeMin();
			unsigned long slidermax = m_sliderCtrl.GetRangeMax();
			unsigned long slidercur = m_sliderCtrl.GetPos();
			unsigned long sliderlen = slidermax - slidermin;
			unsigned long curlen    = slidercur - slidermin;
			unsigned long time      = m_sliderEndTime - m_sliderStartTime;
			unsigned long tmptime   = curlen*time/sliderlen;
			unsigned long curtime   = tmptime + m_sliderStartTime;

			m_startBackTime = curtime;

			//转化后字符提示显示在界面上,a按键要变图像
			m_btnGetBackupEndTime.SetEnable(true,false);

			string stm1;		
			CTimeCtrl::GetTime32Text(stm1,m_startBackTime,CTimeCtrl::SECTION_HOUR|CTimeCtrl::SECTION_MIN|CTimeCtrl::SECTION_SEC);	
			m_tip.SetCaption(stm1,false);
			m_tip.Show(true,false);
		}
		else
		{
			CleanCutBackup();
		}
	}
	Repaint();
	return KEY_VIRTUAL_MSG_STOP;

}
unsigned long CPlayerDlg::OnGetSliderEndTime()
{
	m_btnGetBackupEndTime.SetEnable(false,false);
	if((m_currStatus[m_curRedSelectChnn]==PLAYBACK_PLAYING)||(m_currStatus[m_curRedSelectChnn]==PLAYBACK_FF)||(m_currStatus[m_curRedSelectChnn]==PLAYBACK_REW)||(m_currStatus[m_curRedSelectChnn]==PLAYBACK_PAUSE))
	{
		if ((m_curRedSelectChnn >= 0) && (m_curRedSelectChnn < m_maxChnnNum))
		{
			//获取时间
			unsigned long slidermin,slidermax,slidercur;
			slidermin=m_sliderCtrl.GetRangeMin();
			slidermax=m_sliderCtrl.GetRangeMax();
			slidercur=m_sliderCtrl.GetPos();
			unsigned long sliderlen=slidermax-slidermin;
			unsigned long curlen=slidercur-slidermin;
			unsigned long time=m_sliderEndTime-m_sliderStartTime;
			unsigned long tmptime=curlen*time/sliderlen;
			unsigned long curtime=tmptime+m_sliderStartTime;
			if(curtime<=m_startBackTime)
			{
				//滑条往后拉，交换开始时间和结束时间
				m_endBackTime=m_startBackTime;
				m_startBackTime=curtime;
			}
			else
			{
				m_endBackTime=curtime;
			}

			//结束时间与开始时间间隔小于1分钟（60s）,补齐一分钟
			if(m_endBackTime-m_startBackTime<60)
			{
				m_endBackTime=m_startBackTime+60;
			}
			if (m_endBackTime > GetCurrTime32())
			{
				printf("%s,%d,endTime big CurrTime\n",__FILE__,__LINE__);
				CleanCutBackup();
			}
			else
			{
				string stm1,stm2,sum;
				stm1.clear();
				CTimeCtrl::GetTime32Text(stm1,m_startBackTime,CTimeCtrl::SECTION_HOUR|CTimeCtrl::SECTION_MIN|CTimeCtrl::SECTION_SEC);
				stm2.clear();
				CTimeCtrl::GetTime32Text(stm2,m_endBackTime,CTimeCtrl::SECTION_HOUR|CTimeCtrl::SECTION_MIN|CTimeCtrl::SECTION_SEC);
				sum.clear();
				sum=stm1+"-"+stm2;
				m_tip.SetCaption(sum,false);
				m_tip.Show(true,false);

				//将获得的时间发送消息获取文件信息存入m_searchDataVector
				MESSAGE_DATA msgData;
				NET_DATA_SEARCH searchCond;
				searchCond.searchType  = DATA_SEARCH_TYPE_FILE;
				searchCond.videoCH     = (ULONGLONG)0x01 << m_curRedSelectChnn; 
				searchCond.audioCH     = 0;
				searchCond.startTime   = m_startBackTime;
				searchCond.endTime     = m_endBackTime;
				searchCond.dataOwnerby = m_diskOwnerIndex;
				PACKCMD *pCmd = NULL;
				memset(&msgData, 0, sizeof(MESSAGE_DATA));
				CMessageMan::Instance()->SendMsgDataToServer(0, LOCAL_DEVICE_ID, CMD_REQUEST_DATA_SEARCH, (unsigned char *)&searchCond, sizeof(NET_DATA_SEARCH), msgData);
				assert ((sizeof(PACKCMD) + sizeof(NET_DATA_INFO)) <= msgData.dataLen);
				FILE_INFO_EX *pDate = (FILE_INFO_EX *)(msgData.pData + sizeof(PACKCMD) + sizeof(NET_DATA_INFO));
				int num = (msgData.dataLen - sizeof(PACKCMD) - sizeof(NET_DATA_INFO))/sizeof(FILE_INFO_EX);
				printf("search time:\n");
				PrintfBit32Time(m_startBackTime);
				PrintfBit32Time(m_endBackTime);
				printf("####%s,%d,num:%d\n",__FILE__,__LINE__,num);
				if (num > 0)
				{					
					for (int count = num - 1; count >= 0; --count)
					{
						BACKUP_DATA_SEQ     backupTmp;
						memset(&backupTmp, 0, sizeof(BACKUP_DATA_SEQ));
						backupTmp.changeTime=GetCurrTime32();
						backupTmp.hasCheck = 0;
						backupTmp.seq = MAX_BACKUP_SEQ;
						backupTmp.hasCalculate = 0;
						backupTmp.backupinfo.fileInfo = pDate[count];
						if(backupTmp.backupinfo.fileInfo.startTime<m_startBackTime)
						{
							backupTmp.backupinfo.fileInfo.startTime=m_startBackTime;
						}
						if(backupTmp.backupinfo.fileInfo.endTime>m_endBackTime)
						{
							backupTmp.backupinfo.fileInfo.endTime=m_endBackTime;
						}
						backupTmp.backupinfo.hasBackupAudio = 0;
						backupTmp.hasAudio = 0;
						printf("##count:%d,chn:%d,fileDisk:%d,fileIndex:%d,time:",count,backupTmp.backupinfo.fileInfo.chnn,backupTmp.backupinfo.fileInfo.diskIndex,backupTmp.backupinfo.fileInfo.fileIndex);
						PrintfBit32Time(backupTmp.backupinfo.fileInfo.startTime);

						m_searchDataVector.push_back(backupTmp);
					}
				}
				CMessageMan::Instance()->ReleaseMsgBuff(msgData);
				//刷新现场
				CMessageMan::Instance()->RefreshLiveTrue();
			}
		}
		else
		{
			CleanCutBackup();
		}		
	}
	Repaint();
	return KEY_VIRTUAL_MSG_STOP;
}
unsigned long CPlayerDlg::BackupReplay()
{	
	//开始备份前暂停播放
	int returnStatus = m_currStatus[m_curRedSelectChnn];
	if ((PLAYBACK_PLAYING == returnStatus) || (PLAYBACK_FF == returnStatus) || (PLAYBACK_REW == returnStatus))
	{
		PauseStatus();
		MESSAGE_DATA msgData;
		memset(&msgData, 0, sizeof(MESSAGE_DATA));
		CMessageMan::Instance()->SendMsgDataToServer(0, LOCAL_DEVICE_ID, CMD_REQUEST_LOCAL_CLIENT_PLAYBACK_PAUSE, NULL, 0, msgData);
		CMessageMan::Instance()->ReleaseMsgBuff(msgData);
	}

	ULONGLONG auth = CUserMan::Instance()->CheckAuthority(AUTH_BACKUP);
	for(vector<BACKUP_DATA_SEQ>::iterator it = m_searchDataVector.begin();it != m_searchDataVector.end();)
	{
		if(0 == (auth & (0x01 <<(it->backupinfo.fileInfo.chnn))))
		{
			it = m_searchDataVector.erase(it);
		}
		else
		{
			++it;
		}
	}
	CPreBackUpDlg preBackupDlg;
	preBackupDlg.SetSEARCHData(&m_searchDataVector);
	preBackupDlg.DoModal();
	//备份完毕后把按钮重新设置为不可用
	CleanCutBackup();

	if ((PLAYBACK_PLAYING == returnStatus) || (PLAYBACK_FF == returnStatus) || (PLAYBACK_REW == returnStatus))
	{	
		//回放恢复
		MESSAGE_DATA msgData;
		memset(&msgData, 0, sizeof(MESSAGE_DATA));
		CMessageMan::Instance()->SendMsgDataToServer(0, LOCAL_DEVICE_ID, CMD_REQUEST_LOCAL_CLIENT_PLAYBACK_RESUM, NULL, 0, msgData);
		CMessageMan::Instance()->ReleaseMsgBuff(msgData);

		if(returnStatus == PLAYBACK_PLAYING)
		{
			PlayStatus();
		}
		else if(returnStatus == PLAYBACK_FF)
		{
			memset(&msgData, 0, sizeof(MESSAGE_DATA));
			CMessageMan::Instance()->SendMsgDataToServer(0, LOCAL_DEVICE_ID, CMD_REQUEST_LOCAL_CLIENT_PLAYBACK_FF, (unsigned char*)(&m_curForwardSpeed), sizeof(m_curForwardSpeed), msgData);
			CMessageMan::Instance()->ReleaseMsgBuff(msgData);
			FastForwardStatus();
		}
		else if(returnStatus== PLAYBACK_REW)
		{
			memset(&msgData, 0, sizeof(MESSAGE_DATA));
			CMessageMan::Instance()->SendMsgDataToServer(0, LOCAL_DEVICE_ID, CMD_REQUEST_LOCAL_CLIENT_PLAYBACK_REW, (unsigned char*)(&m_curRewardSpeed), sizeof(m_curRewardSpeed), msgData);
			CMessageMan::Instance()->ReleaseMsgBuff(msgData);
			RewardStatus();
		}
		CMessageMan::Instance()->RefreshLiveTrue();
	}	

	Repaint();
	return KEY_VIRTUAL_MSG_STOP;
}
#endif
#else
unsigned long CPlayerDlg::GetSliderTime()
{
	if((m_currStatus==PLAYBACK_PLAYING)||(m_currStatus==PLAYBACK_FF)||(m_currStatus==PLAYBACK_REW)||(m_currStatus==PLAYBACK_PAUSE))
	{
		string stm1,stm2,sum;
		m_backCnt++;
		//获取时间
		unsigned long slidermin,slidermax,slidercur;
		slidermin=m_sliderCtrl.GetRangeMin();
		slidermax=m_sliderCtrl.GetRangeMax();
		slidercur=m_sliderCtrl.GetPos();
		unsigned long sliderlen=slidermax-slidermin;
		unsigned long curlen=slidercur-slidermin;
		unsigned long time=m_sliderEndTime-m_sliderStartTime;
		unsigned long tmptime=curlen*time/sliderlen;
		unsigned long curtime=tmptime+m_sliderStartTime;
		
		if(m_backCnt==1)//第一次按下
		{
			m_chnn=m_selCuPlayrChnn;
			m_startBackTime=curtime;
			//转化后字符提示显示在界面上,a按键要变图像
			m_btnGetBackupTime.SetCurState(CUTEND,false);
			stm1.clear();
			CTimeCtrl::GetTime32Text(stm1,m_startBackTime,CTimeCtrl::SECTION_HOUR|CTimeCtrl::SECTION_MIN|CTimeCtrl::SECTION_SEC);	
			m_tip.SetCaption(stm1,false);
			m_tip.Show(true,false);
		}
		else if(m_backCnt==2)
		{
			
			m_btnbackup.SetEnable(true,true);
			m_btnGetBackupTime.SetCurState(CUTSTART,false);
			if(curtime<=m_startBackTime)
			{
				//滑条往后拉，交换开始时间和结束时间
				m_endBackTime=m_startBackTime;
				m_startBackTime=curtime;
			}
			else
			{
				m_endBackTime=curtime;
				m_btnbackup.SetEnable(true,false);
			}
		
			//结束时间与开始时间间隔小于1分钟（60s）,补齐一分钟
			if(m_endBackTime-m_startBackTime<60)
			{
				m_endBackTime=m_startBackTime+60;
			}
			
			{
				stm1.clear();
				CTimeCtrl::GetTime32Text(stm1,m_startBackTime,CTimeCtrl::SECTION_HOUR|CTimeCtrl::SECTION_MIN|CTimeCtrl::SECTION_SEC);
				stm2.clear();
				CTimeCtrl::GetTime32Text(stm2,m_endBackTime,CTimeCtrl::SECTION_HOUR|CTimeCtrl::SECTION_MIN|CTimeCtrl::SECTION_SEC);
				sum.clear();
				sum=stm1+"-"+stm2;
				
				m_tip.SetCaption(sum,false);
				m_tip.Show(true,false);

			}
		}
		else//大于2，说明按了3次剪切按钮，前2次选定的内容无效了，以最后一次选择的内容为m_startBackTime
		{
			m_btnGetBackupTime.SetCurState(CUTEND,false);
			m_btnbackup.SetEnable(false,true);
			m_endBackTime=0;
			m_backCnt=1;
			m_startBackTime=curtime;
			stm1.clear();
			CTimeCtrl::GetTime32Text(stm1,m_startBackTime,CTimeCtrl::SECTION_HOUR|CTimeCtrl::SECTION_MIN|CTimeCtrl::SECTION_SEC);	
			m_tip.SetCaption(stm1,false);
			m_tip.Show(true,false);
		}
	}
	
	Repaint();
	return KEY_VIRTUAL_MSG_STOP;
}
unsigned long CPlayerDlg::BackupReplay()
{
	//先检查权限，有的话进行备份，没有发送警告
	ULONGLONG auth = CUserMan::Instance()->CheckAuthority(AUTH_BACKUP);
	if(auth !=0)
	{
		ULONGLONG selCuPlayrChnn=m_selCuPlayrChnn&auth;
		
		assert(m_backCnt==2);
		//进行备份,发送消息给mainframe
		//备份当前看的通道数目的文件
		LOCAL_CLIENT_BACKUP_INFO backupInfo;
		MESSAGE_DATA msgData;
		PACKCMD *pCmd = NULL;
		unsigned long cmdType;
		bool bHaveVideoPlay = false;
		unsigned long freeSpace = 0;
		POS pos = NULL;

		DVR_DISK_INFO *pPartionInfo = NULL;
		DVR_DISK_INFO_LIST partionInfoList;
		int partionNum = 0;
		m_btnbackup.SetEnable(false,false);
		m_recoverStatus=m_currStatus;

		//开始备份前暂停播放
		PauseStatus();
		memset(&msgData, 0, sizeof(MESSAGE_DATA));
		CMessageMan::Instance()->SendMsgDataToServer(0, LOCAL_DEVICE_ID, CMD_REQUEST_LOCAL_CLIENT_PLAYBACK_PAUSE, NULL, 0, msgData);
		CMessageMan::Instance()->ReleaseMsgBuff(msgData);

		//获取需要的文件数据
		NET_DATA_SEARCH searchCond;
		searchCond.searchType  = DATA_SEARCH_TYPE_FILE;
		searchCond.videoCH     = selCuPlayrChnn;  
		searchCond.audioCH     = 0;
		searchCond.startTime   = m_startBackTime;
		searchCond.endTime     = m_endBackTime;
		searchCond.dataOwnerby = m_diskOwnerIndex;

		memset(&msgData, 0, sizeof(MESSAGE_DATA));
		CMessageMan::Instance()->SendMsgDataToServer(0, LOCAL_DEVICE_ID, CMD_REQUEST_DATA_SEARCH, (unsigned char *)&searchCond, sizeof(NET_DATA_SEARCH), msgData);
		assert ((sizeof(PACKCMD) + sizeof(NET_DATA_INFO)) <= msgData.dataLen);
		FILE_INFO_EX *pDateBackup = (FILE_INFO_EX *)(msgData.pData + sizeof(PACKCMD) + sizeof(NET_DATA_INFO));
		int num = (msgData.dataLen - sizeof(PACKCMD) - sizeof(NET_DATA_INFO))/sizeof(FILE_INFO_EX);

		if (0 == num)
		{
			CMessageMan::Instance()->ReleaseMsgBuff(msgData);
			MessageBox(m_pStrTable->String(E_STR_ID_MESSAGE_36), m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK | DEFAULT_ICON_WARNING);
			m_btnbackup.SetEnable(false,true);
			m_startBackTime=0;
			m_endBackTime=0;
			m_backCnt=0;
			string clean;
			clean.clear();
			m_tip.SetCaption(clean,false);
			m_tip.Show(false,true);

			recoverState();
			return KEY_VIRTUAL_MSG_STOP;
		} 

		FILE_INFO_EX_BACKUP *pFileInfoBackup = new FILE_INFO_EX_BACKUP[num];
		for (int count = 0; count < num; ++count)
		{
			pFileInfoBackup[count].fileInfo = pDateBackup[count];
			pFileInfoBackup[count].hasBackupAudio = 1;
		}
		CMessageMan::Instance()->ReleaseMsgBuff(msgData);

		memset(&msgData, 0, sizeof(MESSAGE_DATA));
		//查找硬件设备:uSB， 硬盘硬盘，光驱	
		partionInfoList.RemoveAll();
		//查找所有的热拔插设备的分区信息
		CMessageMan::Instance()->SendMsgDataToServer(0, LOCAL_DEVICE_ID, CMD_REQUEST_DISK_GET_REMOVABLE_PARTION, NULL, 0, msgData);
		pPartionInfo = (DVR_DISK_INFO *)(msgData.pData + sizeof(PACKCMD));
		partionNum = (msgData.dataLen - sizeof(PACKCMD))/sizeof(DVR_DISK_INFO);
		if (partionNum > 0)
		{
			for (int i = 0; i < partionNum; ++i)
			{	
				DVR_DISK_INFO &tmp =	pPartionInfo[i];
				partionInfoList.AddTail(tmp);
			}
		}
		CMessageMan::Instance()->ReleaseMsgBuff(msgData);

		//获取光驱的信息 
		memset(&msgData, 0, sizeof(MESSAGE_DATA));
		CMessageMan::Instance()->SendMsgDataToServer(0, LOCAL_DEVICE_ID, CMD_REQUEST_DISK_GET_CD_INFO, NULL, 0, msgData);
		pPartionInfo = (DVR_DISK_INFO *)(msgData.pData + sizeof(PACKCMD));
		partionNum = (msgData.dataLen - sizeof(PACKCMD))/sizeof(DVR_DISK_INFO);
		if (partionNum > 0)
		{
			for (int i = 0; i < partionNum; ++i)
			{	
				DVR_DISK_INFO tmp =	pPartionInfo[i];
				partionInfoList.AddTail(tmp);
			}
		}
		CMessageMan::Instance()->ReleaseMsgBuff(msgData);

		if (0 == partionInfoList.GetCount())
		{
			MessageBox(m_pStrTable->String(E_STR_ID_MESSAGE_07),m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK | DEFAULT_ICON_WARNING);
			m_btnbackup.SetEnable(true,true);
			recoverState();
			delete []pFileInfoBackup;
			return KEY_VIRTUAL_MSG_STOP;
		}

		CBackUpInfoDlg backUpInfoDlg;

		backUpInfoDlg.SetInitialInfo(&partionInfoList, pFileInfoBackup, num, searchCond.startTime, searchCond.endTime,m_diskOwnerIndex);
		backUpInfoDlg.DoModal(this);

		//备份完毕后把按钮重新设置为不可用
		m_startBackTime=0;
		m_endBackTime=0;
		m_backCnt=0;
		string clean;
		clean.clear();
		m_tip.SetCaption(clean,false);
		m_tip.Show(false,true);
		delete []pFileInfoBackup;
		recoverState();
	}
	else
	{
		MessageBox(m_pStrTable->String(E_STR_ID_MESSAGE_78), m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK | DEFAULT_ICON_WARNING);
		return KEY_VIRTUAL_MSG_STOP;	
	}

	
	
	return KEY_VIRTUAL_MSG_STOP;
}

void CPlayerDlg::recoverState()//回放完毕回复原本的状态
{
	PLAY_SPEED speed=PLAY_SPEED_NONE;
	MESSAGE_DATA msgData;
	if(m_recoverStatus== PLAYBACK_PLAYING)
	{
		//回放恢复
		memset(&msgData, 0, sizeof(MESSAGE_DATA));
		CMessageMan::Instance()->SendMsgDataToServer(0, LOCAL_DEVICE_ID, CMD_REQUEST_LOCAL_CLIENT_PLAYBACK_RESUM, NULL, 0, msgData);
		CMessageMan::Instance()->ReleaseMsgBuff(msgData);
		PlayStatus();

	}
	else if(m_recoverStatus== PLAYBACK_PAUSE)
	{
		memset(&msgData, 0, sizeof(MESSAGE_DATA));
		CMessageMan::Instance()->SendMsgDataToServer(0, LOCAL_DEVICE_ID, CMD_REQUEST_LOCAL_CLIENT_PLAYBACK_PAUSE, NULL, 0, msgData);
		CMessageMan::Instance()->ReleaseMsgBuff(msgData);
		PauseStatus();
	}
	else if(m_recoverStatus== PLAYBACK_FF)
	{
		memset(&msgData, 0, sizeof(MESSAGE_DATA));
		CMessageMan::Instance()->SendMsgDataToServer(0, LOCAL_DEVICE_ID, CMD_REQUEST_LOCAL_CLIENT_PLAYBACK_RESUM, NULL, 0, msgData);
		CMessageMan::Instance()->ReleaseMsgBuff(msgData);
		CMessageMan::Instance()->SendMsgDataToServer(0, LOCAL_DEVICE_ID, CMD_REQUEST_LOCAL_CLIENT_PLAYBACK_FF, (unsigned char*)(&m_curForwardSpeed), sizeof(m_curForwardSpeed), msgData);
		CMessageMan::Instance()->ReleaseMsgBuff(msgData);
		FastForwardStatus();
	}
	else if(m_recoverStatus== PLAYBACK_REW)
	{
		memset(&msgData, 0, sizeof(MESSAGE_DATA));
		CMessageMan::Instance()->SendMsgDataToServer(0, LOCAL_DEVICE_ID, CMD_REQUEST_LOCAL_CLIENT_PLAYBACK_RESUM, NULL, 0, msgData);
		CMessageMan::Instance()->ReleaseMsgBuff(msgData);
		CMessageMan::Instance()->SendMsgDataToServer(0, LOCAL_DEVICE_ID, CMD_REQUEST_LOCAL_CLIENT_PLAYBACK_REW, (unsigned char*)(&m_curRewardSpeed), sizeof(m_curRewardSpeed), msgData);
		CMessageMan::Instance()->ReleaseMsgBuff(msgData);
		RewardStatus();
	}
	CMessageMan::Instance()->RefreshLiveTrue();
}

#endif

int CPlayerDlg::GetSPECtype()
{
	return m_spectype;
}





unsigned long CPlayerDlg::OnClickSpecSplit()
{
	//按下按键后的反应，弹出对话框
	
	GUI::CRect rcBtn = m_btnSpecSplit.GetWindowRect();
	GUI::CRect rcWnd = GetWindowRect();
	GUI::CRect rcSel = m_specSplitDlg.GetDlgDefaultRect();
	int x = (rcBtn.m_left > m_wndLayout.playToolRect.m_left + (rcSel.Width() - rcBtn.Width()) / 2)  ? (rcBtn.m_left - (rcSel.Width() - rcBtn.Width()) / 2) : rcBtn.m_left;
	if(x + rcSel.Width() > m_wndLayout.playToolRect.m_right)
	{
		x = m_wndLayout.playToolRect.m_right - rcSel.Width() - 1;
	}
	
	int y = (rcWnd.m_top + 1 > (rcSel.Height() + m_wndLayout.playToolRect.m_top)) ? (rcWnd.m_top - rcSel.Height()- 1) : (rcWnd.m_bottom + 1);

	unsigned long ipcMaxVideoOutNum = CProduct::Instance()->IPCMaxVideoOutNum();
	if( g_p_login_succ->productInfo.localVideoInputNum && ipcMaxVideoOutNum )
	{
		m_specSplitDlg.SetPlyabackParam(m_maxSplitMode, g_p_login_succ->productInfo.localVideoInputNum, m_maxSelChnn);
	}
	else
	{
		m_specSplitDlg.SetPlyabackParam(m_maxSplitMode,m_maxChnnNum, m_maxSelChnn);
	}
	
	m_specSplitDlg.SetPlayToolParam(m_wndLayout.playToolRect,rcWnd);
	if( DLG_OK == m_specSplitDlg.DoModal(this, x, y, rcSel.Width(), rcSel.Height()) )
	{
		m_spectype=m_specSplitDlg.GetSPlitType();
		
		switch(m_spectype)
		{
			case SPEC_1A5:
				{
					
					if (OnChangeCH(m_1A5CH, PLAY_CH_SIX, 6))
					{
						
						return MESSAGE_VIRTUAL_SPLIT_5;
					}
					break;
				}
			case SPEC_1A5EX:
				{
					{
						m_specSplitDlg.GetExParam(m_currSplitMode,m_selCuPlayrChnn);
						SetAudioCH();
						m_spectype=SPEC_1A5;
						return MESSAGE_VIRTUAL_SPLIT_5;
					}
					break;
				}
			case SPEC_1A7:
				{
					
					if (OnChangeCH(m_1A7CH, PLAY_CH_EIGHT, 8))
					{
						
						return MESSAGE_VIRTUAL_SPLIT_7;
					}
					break;
				}
			case SPEC_1A7EX:
				{
					{
						m_specSplitDlg.GetExParam(m_currSplitMode,m_selCuPlayrChnn);
						SetAudioCH();
						m_spectype=SPEC_1A7;
						return MESSAGE_VIRTUAL_SPLIT_7;
					}
					break;
					
				}
			case SPEC_1A12:
				{
					if (OnChangeCH(m_1A12CH, PLAY_CH_THIRTEEN, 13))
					{
					
						return MESSAGE_VIRTUAL_SPLIT_12;
					}
					break;
				}
			case SPEC_1A12EX:
				{
					{
						m_specSplitDlg.GetExParam(m_currSplitMode,m_selCuPlayrChnn);
						SetAudioCH();
						m_spectype=SPEC_1A12;
						return MESSAGE_VIRTUAL_SPLIT_12;
					}
					break;
				}
			case SPEC_NONE:
				{
					break;
				}
			default:
				{
					break;
				}
		}
	}
	else
	{
		GetRootWnd()->Repaint();
	}

	return KEY_VIRTUAL_MSG_STOP;
} 
#if defined(__NEW_CUT_BACKUP__)
void CPlayerDlg::SetCurSelectChnn(unsigned long curChnn)
{
	if ((curChnn >= 0) && (curChnn < m_maxChnnNum))
	{
		if (m_curRedSelectChnn != curChnn)
		{
			m_curRedSelectChnn = curChnn;
			CleanCutBackup();
			Repaint();
		}
#if defined(__ASYN_PLAY__)
		switch(m_currStatus[m_curRedSelectChnn])
		{
		case PLAYBACK_STOP:
			StopStatus();
			break;
		case PLAYBACK_PLAYING:
			PlayStatus();
			break;
		case PLAYBACK_FF:
			FastForwardStatus();
			break;
		case PLAYBACK_REW:
			RewardStatus();
			break;
		case PLAYBACK_PAUSE:
			PauseStatus();
			break;
		default:
			break;
		}
#endif
	}
	else
	{
		m_curRedSelectChnn =  INVALID_INDEX(unsigned long);
		CleanCutBackup();
		Repaint();
	}
}


void CPlayerDlg::CleanCutBackup()
{
	m_btnGetBackupStartTime.SetEnable(true,false);
	m_btnGetBackupEndTime.SetEnable(false,false);
#if !defined(__ASYN_PLAY__)
	m_btnbackup.SetEnable(true,false);
#else
	if(m_bSync)
	{
		m_btnbackup.SetEnable(true,false);
	}
	else
	{
		m_btnbackup.SetEnable(false,false);
	}
#endif
	string stm1;
	stm1.clear();
	m_tip.SetCaption(stm1,false);
	m_tip.Show(false,false);
}
#endif


#ifdef __CUSTOM_US02_NEW__
unsigned long CPlayerDlg::OnFocus()
{
	CWnd* active = GetActiveCtrl();
	if(active != NULL)
	{
		return KEY_VIRTUAL_OVER;
	}
	else
	{
		return KEY_VIRTUAL_OUT;
	}

}
#endif


#ifdef __CUSTOM_US02_NEW__
unsigned long CPlayerDlg::OnFocus(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	CWnd* active = GetActiveCtrl();
	if(active != NULL)
	{
		return KEY_VIRTUAL_OVER;
	}
	else
	{
		return KEY_VIRTUAL_OUT;
	}
}
#endif

//end

