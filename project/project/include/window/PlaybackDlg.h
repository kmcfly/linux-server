/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : hz
** Date         : 2009-08-12
** Name         : PlaybackDlg.h 
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/

#ifndef _PLAYBACK_DLG_H_
#define _PLAYBACK_DLG_H_

#include "GroupCtrl.h"
#include "Dialog.h"
#include "PlayerDlg.h"
#include "MessageMan.h"
#include "mylist.cpp"
#include "ZoomDlg.h"
#include "DisplayMan.h"

//提示窗口
#include "TipWindow.h"


#define SECTIME_LENGTH   1800     //30*60.以秒为单位。表示时间刻度的长度。
using namespace GUI;

typedef struct _playback_channel_info
{
	int   channel;
	int   xPos;
	int   yPos;
	int   xWidth;
	int   yHeight;
}PLAYBACK_CHANNEL_INFO;

typedef CMyList <PLAYBACK_CHANNEL_INFO> PLAYBACK_CHANNEL_INFO_LIST;

class CPlaybackDlg : public GUI::CDialog
{
public:

	
	CPlaybackDlg();
	~CPlaybackDlg();

	//在回放时，外边只能调用setPlayInfo的一种形式。按文件回放还是按时间回放。
	void SetPlayInfo(unsigned long playbackStartTime, unsigned long playbackEndTime, 
		             unsigned long playbackPlayTime, ULONGLONG chnnBits,
					 unsigned char maxChannelNum, unsigned char videoOutPutNum, 
					 bool bChangePlayChnn, unsigned long diskOwnerIndex);

	//按文件回放
	void SetPlayInfo(FILE_INFO_EX fileInfo, unsigned long diskOwnerIndex);

    static int  ProcCallBack(void* pObject, int iMsg, void *pParam1, void *pParam2);

protected:
	DECLARE_MSG_MAP()
	void OnInitial();
	void OnDestroy();
	void OnPostInitial();
	void OnPaint(GUI::PAINT_STATUS status);

	unsigned long OnHide();
	unsigned long OnExit();
	unsigned long OnAudio();

	unsigned long OnZoom();
	unsigned long OnClickPlayPause();
	unsigned long OnClickStop();
	unsigned long OnClickNextFrame();
	unsigned long OnClickFastForward();
	unsigned long OnClickReward();
	unsigned long OnClickSplit(unsigned long keyID);
	
	unsigned long OnNextSplit();
	unsigned long OnNextGroup();

	unsigned long OnClickPreSection();
	unsigned long OnClickNextSection();
	void          OnIdle();
	unsigned long OnMouseUp(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);
	unsigned long OnDrag(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);
	unsigned long OnEnter(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);
	unsigned long OnMouseMove(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);
	unsigned long OnMouseDrag(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);
	unsigned long OnClickNumKey(unsigned long keyID);
	unsigned long OnDoubleClick(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);
	unsigned long OnFlyRight(unsigned long keyID);
	unsigned long OnFlyLeft(unsigned long keyID);
	unsigned long OnFlyMedium();
	unsigned long OnFlyInRight();
	int  ProcCallBack(int iMsg, void *pParam1, void *pParam2);

	void InitialBtnStatus();

	PLAY_CH_MODE GetSplitMode(unsigned char videoOutPutNum);
	void SetSplitMode(ULONGLONG chnnBits);
	int  GetSplitChannelNum(PLAY_CH_MODE mode);
	
	void GetSectionTime(unsigned long time, unsigned long &secStartime, unsigned long &secEndTime);
	
	void FlyChangePlaySpeed(unsigned long keyID, bool bForward);

	bool GetCurDisplay(NET_DISPLAY_INFO& disp, unsigned short x = 0, unsigned short y = 0, bool bUsePt = false);
	void ChangeDisplayByTwoPt(unsigned short x1, unsigned short y1, unsigned short x2, unsigned short y2, unsigned long timeout = 0, unsigned short bRec = 0x1);
	//---------------added 2011.7.16---------------------
	unsigned long OnPreCommand(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);
protected:

	//tip window
	#ifdef __CUSTOM_US02_NEW__
	unsigned long OnOver();
	unsigned long OnOut();
	#endif

	CZoomDlg			m_zoomDlg;
	CPlayerDlg			m_playerDlg;
	GUI::CRect			m_rectPlayer;//记录ToolBar位置

	FILE_INFO_EX_LIST  *m_pFileList;
	ENUM_PLAYBACK_TYPE	m_playType;
	unsigned long      m_playbackStartTime;
	unsigned long      m_playbackEndTime;
	unsigned long      m_playbackPlayTime;
	unsigned long      m_lastFrameTime;
	ULONGLONG          m_chnnBits;
	unsigned long      m_curPlaybackStatus;

	unsigned long      m_preFrameTime;
	unsigned long      m_secStartTime;
	unsigned long      m_secEndTime;
	bool               m_bDragSlider;
	unsigned long      m_changeTime;
	bool               m_bStopPlayMsg;

	PLAYBACK_CHANNEL_INFO_LIST m_preSplitModeInfoList;
	PLAY_CH_MODE               m_preSplitMode;
	PLAYBACK_CHANNEL_INFO_LIST m_curSplitModeInfoList;
	PLAY_CH_MODE               m_curSplitMode;
	PLAY_CH_MODE               m_maxSplitMode;
	unsigned char              m_maxVideoOutputNum;
	unsigned char              m_maxChannelNum;
	bool                       m_bChangePlayChnn;
	unsigned long			   m_ipcMaxVideoOutputNum;


	PLAYER_CTRL_SPEED_LIST  m_ctrlSpeedForwardList;
	PLAYER_CTRL_SPEED_LIST  m_ctrlSpeedRewardList;

	unsigned long           m_diskOwnerIndex;

	enum 
	{
		PLAYBACK_STATUS_STOP,
		PLAYBACK_STATUS_PLAYING,
		PLAYBACK_STATUS_PAUSE,
		PLAYBACK_STATUS_FF,
		PLAYBACK_STATUS_REW,
	};

	////拖动通道
	GUI::CGroupCtrl	m_groupCtrl;		
	bool			m_bDragChnn;
	bool			m_bFirstDragChnn;
	bool			m_bDraged;
	unsigned long	m_lastDragPosX;
	unsigned long	m_lastDragPosY;
	unsigned long	m_enterDragPosX;
	unsigned long	m_enterDragPosY;
private:
	CDisplayMan		*m_pDisplayMan;
#if defined(__NEW_CUT_BACKUP__)
	CURCHNNINFO m_clickchnnINfo;
#endif
	
#if defined(__ASYN_PLAY__)
	unsigned long	   m_curStatus[32];
	bool			   m_bSync;
#endif

	//提示
	CTipWindow*			m_pTipWindow;
};

#endif//_PLAYBACK_DLG_H_
