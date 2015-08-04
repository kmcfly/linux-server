/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : hz
** Date         : 2009-08-12
** Name         : PlayerDlg.h 
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/

#ifndef _PLAYER_DLG_H_
#define _PLAYER_DLG_H_

#include "CheckCtrl.h"
#include "Dialog.h"
#include "SliderCtrl.h"
#include "DateCtrl.h"
#include "TimeCtrl.h"
#include "Menu.h"
#include "mylist.cpp"
#include "SelectChnnDlg.h"
#include "StateButton.h"
#include "SelectAudioDlg.h"
#include "CfgDevColorDlg.h"
#include "dvrdvsdef.h"
#include "SpecSplitDlg.h"
#include "PreBackUpDlg.h"




#define  MESSAGE_VIRTUAL_DRAG_START  (KEY_VIRTUAL_MSG_DEFAULT_TAG | ((unsigned long )0x01))
#define  MESSAGE_VIRTUAL_DRAG_ING    (KEY_VIRTUAL_MSG_DEFAULT_TAG | ((unsigned long )0x02))  
#define  MESSAGE_VIRTUAL_DRAG_END    (KEY_VIRTUAL_MSG_DEFAULT_TAG | ((unsigned long )0x03))
#define  MESSAGE_VIRTUAL_SPLIT_5    (KEY_VIRTUAL_MSG_DEFAULT_TAG | ((unsigned long )0x04))//6个画面，大小画面
#define  MESSAGE_VIRTUAL_SPLIT_7    (KEY_VIRTUAL_MSG_DEFAULT_TAG | ((unsigned long )0x05))//8个画面，大小画面
#define  MESSAGE_VIRTUAL_SPLIT_12    (KEY_VIRTUAL_MSG_DEFAULT_TAG | ((unsigned long )0x06))//13个画面，大小画面

using namespace GUI;

typedef struct _player_ctrl_speed
{
	unsigned long speed;
	char          szSpeedName[10];
}PLAYER_CTRL_SPEED_ITEM;

typedef CMyList <PLAYER_CTRL_SPEED_ITEM>  PLAYER_CTRL_SPEED_LIST;
class CPlayerDlg : public GUI::CDialog
{
public:
	CPlayerDlg();
	~CPlayerDlg();
   
	//在调用OnInitial前需要完成的动作
	void SetCurPlayChnn(ULONGLONG curChnn);
	void SetMaxSelChnn(int chnnNum, int maxSel);//设置最大可以选择的回放通道个数
	void SetSpeedRang(PLAYER_CTRL_SPEED_LIST *pCtrlSpeedForward, PLAYER_CTRL_SPEED_LIST *pCtrlSpeedReward);
	void SetMaxSplitMode(PLAY_CH_MODE splitMode, bool bChangePlayChnn);
	void SetCurSplitMode(PLAY_CH_MODE curSplitMode) {m_currSplitMode = curSplitMode;};
	void SetDiskOwner(unsigned long diskOwner) { m_diskOwnerIndex = diskOwner;};

	//与时间轴相关的函数
	void SetRange(int min, int max);
	void SetSectionTime(unsigned long startTime, unsigned long endTime);
	void SetTime(unsigned long time, unsigned long pos, bool bRefresh = true);
	int  GetCurPos();
	unsigned long GetCurTime();
	void ShowTime(bool bShow);
    
	//改变状态的函数
	void StopStatus();
	void PlayStatus(bool bRefresh = true);
	
	//播放或者暂停
	bool PlayPause();

	//单帧播放
	bool NextFrame();

	//与快进（退）相关的函数
	bool ChangeSpeed(bool bForward);
	bool SetSpeed(PLAY_SPEED playSpeed, bool bForward);
	unsigned long GetCurSpeed(bool bForward);	

	//获取播放通道
	bool ChangeCH(unsigned long keyID);
	ULONGLONG GetCurPlayChnn(unsigned long & splitMode,int type);

	//获取音频通道 和 在点击回放画面时，需要先设置音频通道。
	unsigned long GetAudioCH();	
	void SetAudioCH();
#if defined(__ASYN_PLAY__)
	bool IsSync(){return m_bSync;};	
#endif

	//取下一个分割模式
	bool NextSplit();
	//取该分割模式下的下一组通道
	bool NextGroup();

	static unsigned long SplitPBToLive(PLAY_CH_MODE mode,int type);
	int GetSPECtype();//获取是否是奇异分割
	void SetSPECtype(int type);
	int GetPreSPECtype();//获取上一次的奇异分割
#if defined(__NEW_CUT_BACKUP__)
	void SetCurSelectChnn(unsigned long curChnn);
#endif

	    //
#ifndef __ASYN_PLAY__
    int GetCurrStatus(){return m_currStatus;}
#endif
    /************************************/
    
protected:
	DECLARE_MSG_MAP()
	void OnInitial();
	void OnPostInitial();
	void OnPaint(PAINT_STATUS status);
	void OnDestroy();
	unsigned long OnPreCommand(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);
	//////////////////////////////////////////////////////////////////////////
	//播放、暂停、停止
	unsigned long OnClickPlayPause();
	unsigned long OnClickStop();

	//单帧播放
	unsigned long OnClickNextFrame();

	//快进、快退
	unsigned long OnClickFastForward();
	unsigned long OnClickReward();
	unsigned long OnClickFFEx();
	unsigned long OnClickREWEx();

	//抓图、色彩调节
	unsigned long OnClickColor();
	unsigned long OnClickSnap();

	//分割按钮
	unsigned long OnClickOneSplit();
	unsigned long OnClickFourSplit();
	unsigned long OnClickNineSplit();
	
	unsigned long OnClickSixteenSplit();
	unsigned long OnClickSixSplit();
	unsigned long OnClickSpecSplit();
	unsigned long OnClickLive();

	//1UTW按键板上的改变分割模式和切换下一组通道按钮
	unsigned long OnSplit();
	unsigned long OnGroup();


	unsigned long OnClick6X6Ex();
	unsigned long OnClick5X5Ex();
	unsigned long OnClick4X4Ex();
	unsigned long OnClick3X3Ex();
	unsigned long OnClick1A5Ex();
	unsigned long OnClick2X3Ex();
	unsigned long OnClick2X2Ex();
	unsigned long OnClick1X1Ex();

	//放大、音频、隐藏、返回
	unsigned long OnClickZoom();
	unsigned long OnClickAudio();
	unsigned long OnClickHide();
	unsigned long OnClickExit();
	unsigned long OnClickMove(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);

	//上一段、下一段、拖动
	unsigned long OnClickPreSection();
	unsigned long OnClickNextSection();
	unsigned long OnDragStart(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);
	unsigned long OnDragIng(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);
    unsigned long OnDragEnd(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);

	//
	unsigned long OnShowName();
#if defined(__ASYN_PLAY__)
	unsigned long OnClickAsync();
#endif
	DLG_END_VALUE PopupLiveColorDlg( int x, int y, int cx, int cy, int chnn );


	//
	#ifdef __CUSTOM_US02_NEW__
	unsigned long OnFocus();
	unsigned long OnFocus(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);
	#endif
	//////////////////////////////////////////////////////////////////////////

private:
	enum
	{
		AUDIO_OPEN,
		AUDIO_MUTE,
	};

	enum
	{
		PLAYBACK_STOP,
		PLAYBACK_PLAYING,
		PLAYBACK_PAUSE,
		PLAYBACK_FF,
		PLAYBACK_REW
	};

	void PauseStatus();	
	void NextFrameStatus();
	void FastForwardStatus();
	void RewardStatus();

	bool OnClickSelChnn(GUI::CButton & btn, PLAY_CH_MODE splitMode, int splitChnn, ULONGLONG & selCH);

	void SetSplitBtnEnable(bool bEnable, bool bRefresh = true);
	void RefreshAudioBtn(bool bRefresh = true);
	void FindPlayStausName(bool bForwardList, unsigned long speed, char *szRetName);

	unsigned long GetNextPlaySpeed( bool bforward ,int chnnNum);

	bool OnChangeCH(ULONGLONG & CH, PLAY_CH_MODE splitMode, unsigned long n);

	//////////////////////////////////////////////////////////////////////////
	GUI::CSliderCtrl m_sliderCtrl;
	GUI::CStatic m_scTime;

	GUI::CButton m_btnPlayPause;
	GUI::CButton m_btnStop;

	GUI::CButton m_btnNextFrame;
	GUI::CCheckCtrl	m_ckShowName;
	//
	GUI::CButton m_btnFastForward;
	GUI::CButton m_btnReward;
	GUI::CButton m_btnFFEx;
	GUI::CButton m_btnREWEx;

	GUI::CMenu m_menuForward;
	GUI::CMenu m_menuReward;

#if !defined(__ASYN_PLAY__)
	unsigned long m_curForwardSpeed;
	unsigned long m_curRewardSpeed;
#else
	unsigned long m_curForwardSpeed[32];
	unsigned long m_curRewardSpeed[32];
#endif
	PLAYER_CTRL_SPEED_LIST *m_pCtrlSpeedForwardList;
	PLAYER_CTRL_SPEED_LIST *m_pCtrlSpeedRewardList;	

	//
	GUI::CStateButton	m_btnAudio;
	GUI::CButton		m_btnHide;
	GUI::CButton		m_btnExit;
	GUI::CButton		m_btnMove;

	//分割按钮
	GUI::CButton	m_btnOneSplit;
	GUI::CButton	m_btnFourSplit;
	GUI::CButton	m_btnNineSplit;
	GUI::CButton	m_btnSixteenSplit;
	GUI::CButton	m_btnSixSplit;
	//zxx新加奇异分割（大小画面）
	GUI::CButton	m_btnSpecSplit;

	
	GUI::CButton	m_btn4X4Ex;
	GUI::CButton	m_btn3X3Ex;
	
	GUI::CButton	m_btn2X3Ex;
	GUI::CButton	m_btn2X2Ex;
	GUI::CButton	m_btn1X1Ex;
	GUI::CButton	m_btnColor;
	GUI::CButton	m_btnSnap;
	GUI::CButton	m_btnZoom;
	GUI::CButton	m_btnLive;

	ULONGLONG		m_1X1CH;
	ULONGLONG		m_2X2CH;
	ULONGLONG		m_2X3CH;
	
	ULONGLONG		m_1A5CH;
	ULONGLONG       m_1A7CH;
	ULONGLONG		m_1A12CH;
	ULONGLONG		m_3X3CH;
	ULONGLONG		m_4X4CH;
	PLAY_CH_MODE	m_currSplitMode;

	PLAY_CH_MODE	m_maxSplitMode;
	bool			m_bChangePlayChnn;
	//
	GUI::CButton	m_btnPreSection;
	GUI::CButton	m_btnNextSection;
	GUI::CStatic	m_stSecStartTime;
	GUI::CStatic	m_stSecEndTime;
	GUI::CStatic	m_stPlayStatus;
#if defined(__ASYN_PLAY__)
	GUI::CCheckCtrl	m_ckSyncPlayback;
#endif



protected:
	CSelectAudioDlg			m_selAudioDlg;
	GUI::CSelectChnnDlg		m_selChnnDlg;		//通道选择对话框
	GUI::CSpecSplitDlg      m_specSplitDlg;		//奇异分割对话框
	int						m_maxSelChnn;		//最大能选择回放的通道
	int						m_maxChnnNum;		//最大通道
	ULONGLONG				m_selCuPlayrChnn;	//当前播放的通道bit
	ULONGLONG				m_selAudioCH;		//当前选择的音频通道
#if !defined(__ASYN_PLAY__)
	int						m_currStatus;
#else
	int						m_currStatus[32];
#endif
	CCfgDevColorDlg		    m_changeColorDlg;	//色彩调节对话框

	//表示可以播放音频的通道数目.此变量有两种取值，这两种取值都有可以。
	//第一种，m_playAudioMaxChnnNum = g_p_login_succ->productInfo.audioInputNum;
	//比如在只能录4路音频的设备上，由于只有4路有音频，故可以播放音频的通道数目也只能是4.
	//第二中，m_playAudioMaxChnnNum = m_maxSelChnn;
	//我们可以让每个通道都能播放音频，如果这个通道当时录音频了，那么就可以播放出声音来，
    //如果这个通道当时没有录音频，那么即使让它播放音频，它也播放不出来。故可以播放音频的通道数目可以是最大视频通道数。
	unsigned char           m_playAudioMaxChnnNum; 

	ULONGLONG				m_audioChnnMask;


	////////////////////////////////////////////////////////////
	//窗口布局
public:
	void OnEnter(unsigned long xPos, unsigned long yPos);
	void OnMouseMove(unsigned long xPos, unsigned long yPos);
	GUI::CRect OnMouseDrag(unsigned long xPos, unsigned long yPos, bool bRefresh = false);
	unsigned long OnMouseLeftUp();

	void AutoLayout(bool bRefresh = false, int xPos = -1, int yPos = -1);
	bool MovePlayToolWnd(int x, int y);
	void LayoutSubWnd();

	void SetBtnWndInfo(GUI::CWnd* pWnd, int index, unsigned short width, unsigned short height, unsigned short offsetTop = 0);

	bool PtInPlayToolBar(int xPos, int yPos);
	//(x1,y1) (x2,y2) 距离相差不大
	bool IsAlmostEqual(unsigned long x1, unsigned long y1, unsigned long x2, unsigned long y2, unsigned long distance);
	//(x1,y1) (x2,y2) 距离相差很大
	bool IsAlmostAbove(unsigned long x1, unsigned long y1, unsigned long x2, unsigned long y2, unsigned long distance);

protected:
	struct BTN_WND_INFO
	{
		unsigned short	width;			//Btn占据的宽度[Btn宽度 + 空隙]
		unsigned short	height;
		unsigned short	offsetTop;		//距离行顶部距离
		unsigned short  row;			//哪一行
		GUI::CWnd*		pWnd;
	};
	
	struct BTN_WND_LAYOUT
	{
		unsigned short	btnMaxWidth;	//所有按钮宽度和(包含间距)

		unsigned short  rightFixWidth;	//右边固定宽度
		unsigned short	minWidth;		//窗体需求的最小宽度
		unsigned short	minHeight;		//窗体需求的最小高度
		unsigned short	maxRow;			//控件排布行数

		int				state;			//0底部,1顶部,2中间

		int				offsetTop;		//顶边距离
		int				offsetRight;	//右边界距离

		GUI::CRect		wndRect;		//控制条显示区域
		GUI::CRect		playToolRect;	//控制条最大的活动范围
	};

	int				m_xPosLeft;			//起始位置
	int				m_yPosTop;
	int				m_bmpBgWidth;		//背景图片大小
	int				m_bmpBgHeight;
	int				m_btnCount;			//布局按钮个数
	BTN_WND_INFO	m_btnWndInfo[32];	//按钮信息
	BTN_WND_LAYOUT	m_wndLayout;		//布局信息
	bool			m_bCvbs;			//是否是CVBS显示方式

	//鼠标、移动信息
	unsigned long	m_x_enter, m_y_enter;			//鼠标点击时位置
	unsigned long	m_x_drag_last, m_y_drag_last;	//
	unsigned long	m_x_pos, m_y_pos;				//鼠标位置
	unsigned long	m_x_pos_last, m_y_pos_last;		//上一次鼠标位置

	bool			m_bFirstDrag;					//第一次拖动标记
	bool			m_bDrag;						//是否处于拖动状态
	int             m_spectype;                     //分割类型
	unsigned long   m_diskOwnerIndex;
private:

#if defined(__NEW_CUT_BACKUP__)
	unsigned long OnGetSliderStartTime();           //剪切开始
	unsigned long OnGetSliderEndTime();             //剪切结束并把对应当前的剪切时间段的文件搜索并存入m_searchDataVector
	unsigned long BackupReplay();                   //备份剪切好的回放
	void          CleanCutBackup();	
	GUI::CButton m_btnGetBackupStartTime;
	GUI::CButton m_btnGetBackupEndTime;
	GUI::CButton m_btnbackup;
	GUI::CStatic  m_tip;                             //按下剪切后的提示	
	unsigned long m_sliderStartTime;                 //滑条开始和结束时间
	unsigned long m_sliderEndTime;	
	unsigned long m_startBackTime;                  //备份开始时间和结束时间
	unsigned long m_endBackTime;	
	unsigned long  m_curRedSelectChnn;  //红框选择的通道号
	BACKUP_DATA_SEQ_VECTOR m_searchDataVector;
#else

	unsigned long GetSliderTime();//获取滑条的时间,剪切功能
	unsigned long BackupReplay();//备份剪切好的回放
	void recoverState();//回放完毕回复原本的状态

	//回放备份获取按钮
	GUI::CStateButton m_btnGetBackupTime;
	GUI::CButton m_btnbackup;
	GUI::CStatic m_tip; //按下剪切后的提示
	//滑条开始和结束时间
	unsigned long m_sliderStartTime;
	unsigned long m_sliderEndTime;

	//备份开始时间和结束时间
	unsigned long m_startBackTime;
	unsigned long m_endBackTime;
	int m_backCnt;//按下剪切按钮的次数
	enum
	{
		CUTSTART,
		CUTEND,
	};
	ULONGLONG m_chnn;
	bool m_bIsPlay;//判断是停止还是播放
	int	 m_recoverStatus;

#endif
#if defined(__ASYN_PLAY__)
	ULONGLONG m_liveSelCH;
	bool m_bSync;	//是否同步播放（默认同步）
#endif
};

#endif//_PLAYER_DLG_H_

