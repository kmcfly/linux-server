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
#define  MESSAGE_VIRTUAL_SPLIT_5    (KEY_VIRTUAL_MSG_DEFAULT_TAG | ((unsigned long )0x04))//6�����棬��С����
#define  MESSAGE_VIRTUAL_SPLIT_7    (KEY_VIRTUAL_MSG_DEFAULT_TAG | ((unsigned long )0x05))//8�����棬��С����
#define  MESSAGE_VIRTUAL_SPLIT_12    (KEY_VIRTUAL_MSG_DEFAULT_TAG | ((unsigned long )0x06))//13�����棬��С����

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
   
	//�ڵ���OnInitialǰ��Ҫ��ɵĶ���
	void SetCurPlayChnn(ULONGLONG curChnn);
	void SetMaxSelChnn(int chnnNum, int maxSel);//����������ѡ��Ļط�ͨ������
	void SetSpeedRang(PLAYER_CTRL_SPEED_LIST *pCtrlSpeedForward, PLAYER_CTRL_SPEED_LIST *pCtrlSpeedReward);
	void SetMaxSplitMode(PLAY_CH_MODE splitMode, bool bChangePlayChnn);
	void SetCurSplitMode(PLAY_CH_MODE curSplitMode) {m_currSplitMode = curSplitMode;};
	void SetDiskOwner(unsigned long diskOwner) { m_diskOwnerIndex = diskOwner;};

	//��ʱ������صĺ���
	void SetRange(int min, int max);
	void SetSectionTime(unsigned long startTime, unsigned long endTime);
	void SetTime(unsigned long time, unsigned long pos, bool bRefresh = true);
	int  GetCurPos();
	unsigned long GetCurTime();
	void ShowTime(bool bShow);
    
	//�ı�״̬�ĺ���
	void StopStatus();
	void PlayStatus(bool bRefresh = true);
	
	//���Ż�����ͣ
	bool PlayPause();

	//��֡����
	bool NextFrame();

	//�������ˣ���صĺ���
	bool ChangeSpeed(bool bForward);
	bool SetSpeed(PLAY_SPEED playSpeed, bool bForward);
	unsigned long GetCurSpeed(bool bForward);	

	//��ȡ����ͨ��
	bool ChangeCH(unsigned long keyID);
	ULONGLONG GetCurPlayChnn(unsigned long & splitMode,int type);

	//��ȡ��Ƶͨ�� �� �ڵ���طŻ���ʱ����Ҫ��������Ƶͨ����
	unsigned long GetAudioCH();	
	void SetAudioCH();
#if defined(__ASYN_PLAY__)
	bool IsSync(){return m_bSync;};	
#endif

	//ȡ��һ���ָ�ģʽ
	bool NextSplit();
	//ȡ�÷ָ�ģʽ�µ���һ��ͨ��
	bool NextGroup();

	static unsigned long SplitPBToLive(PLAY_CH_MODE mode,int type);
	int GetSPECtype();//��ȡ�Ƿ�������ָ�
	void SetSPECtype(int type);
	int GetPreSPECtype();//��ȡ��һ�ε�����ָ�
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
	//���š���ͣ��ֹͣ
	unsigned long OnClickPlayPause();
	unsigned long OnClickStop();

	//��֡����
	unsigned long OnClickNextFrame();

	//���������
	unsigned long OnClickFastForward();
	unsigned long OnClickReward();
	unsigned long OnClickFFEx();
	unsigned long OnClickREWEx();

	//ץͼ��ɫ�ʵ���
	unsigned long OnClickColor();
	unsigned long OnClickSnap();

	//�ָť
	unsigned long OnClickOneSplit();
	unsigned long OnClickFourSplit();
	unsigned long OnClickNineSplit();
	
	unsigned long OnClickSixteenSplit();
	unsigned long OnClickSixSplit();
	unsigned long OnClickSpecSplit();
	unsigned long OnClickLive();

	//1UTW�������ϵĸı�ָ�ģʽ���л���һ��ͨ����ť
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

	//�Ŵ���Ƶ�����ء�����
	unsigned long OnClickZoom();
	unsigned long OnClickAudio();
	unsigned long OnClickHide();
	unsigned long OnClickExit();
	unsigned long OnClickMove(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);

	//��һ�Ρ���һ�Ρ��϶�
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

	//�ָť
	GUI::CButton	m_btnOneSplit;
	GUI::CButton	m_btnFourSplit;
	GUI::CButton	m_btnNineSplit;
	GUI::CButton	m_btnSixteenSplit;
	GUI::CButton	m_btnSixSplit;
	//zxx�¼�����ָ��С���棩
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
	GUI::CSelectChnnDlg		m_selChnnDlg;		//ͨ��ѡ��Ի���
	GUI::CSpecSplitDlg      m_specSplitDlg;		//����ָ�Ի���
	int						m_maxSelChnn;		//�����ѡ��طŵ�ͨ��
	int						m_maxChnnNum;		//���ͨ��
	ULONGLONG				m_selCuPlayrChnn;	//��ǰ���ŵ�ͨ��bit
	ULONGLONG				m_selAudioCH;		//��ǰѡ�����Ƶͨ��
#if !defined(__ASYN_PLAY__)
	int						m_currStatus;
#else
	int						m_currStatus[32];
#endif
	CCfgDevColorDlg		    m_changeColorDlg;	//ɫ�ʵ��ڶԻ���

	//��ʾ���Բ�����Ƶ��ͨ����Ŀ.�˱���������ȡֵ��������ȡֵ���п��ԡ�
	//��һ�֣�m_playAudioMaxChnnNum = g_p_login_succ->productInfo.audioInputNum;
	//������ֻ��¼4·��Ƶ���豸�ϣ�����ֻ��4·����Ƶ���ʿ��Բ�����Ƶ��ͨ����ĿҲֻ����4.
	//�ڶ��У�m_playAudioMaxChnnNum = m_maxSelChnn;
	//���ǿ�����ÿ��ͨ�����ܲ�����Ƶ��������ͨ����ʱ¼��Ƶ�ˣ���ô�Ϳ��Բ��ų���������
    //������ͨ����ʱû��¼��Ƶ����ô��ʹ����������Ƶ����Ҳ���Ų��������ʿ��Բ�����Ƶ��ͨ����Ŀ�����������Ƶͨ������
	unsigned char           m_playAudioMaxChnnNum; 

	ULONGLONG				m_audioChnnMask;


	////////////////////////////////////////////////////////////
	//���ڲ���
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
	//(x1,y1) (x2,y2) ��������
	bool IsAlmostEqual(unsigned long x1, unsigned long y1, unsigned long x2, unsigned long y2, unsigned long distance);
	//(x1,y1) (x2,y2) �������ܴ�
	bool IsAlmostAbove(unsigned long x1, unsigned long y1, unsigned long x2, unsigned long y2, unsigned long distance);

protected:
	struct BTN_WND_INFO
	{
		unsigned short	width;			//Btnռ�ݵĿ��[Btn��� + ��϶]
		unsigned short	height;
		unsigned short	offsetTop;		//�����ж�������
		unsigned short  row;			//��һ��
		GUI::CWnd*		pWnd;
	};
	
	struct BTN_WND_LAYOUT
	{
		unsigned short	btnMaxWidth;	//���а�ť��Ⱥ�(�������)

		unsigned short  rightFixWidth;	//�ұ߹̶����
		unsigned short	minWidth;		//�����������С���
		unsigned short	minHeight;		//�����������С�߶�
		unsigned short	maxRow;			//�ؼ��Ų�����

		int				state;			//0�ײ�,1����,2�м�

		int				offsetTop;		//���߾���
		int				offsetRight;	//�ұ߽����

		GUI::CRect		wndRect;		//��������ʾ����
		GUI::CRect		playToolRect;	//���������Ļ��Χ
	};

	int				m_xPosLeft;			//��ʼλ��
	int				m_yPosTop;
	int				m_bmpBgWidth;		//����ͼƬ��С
	int				m_bmpBgHeight;
	int				m_btnCount;			//���ְ�ť����
	BTN_WND_INFO	m_btnWndInfo[32];	//��ť��Ϣ
	BTN_WND_LAYOUT	m_wndLayout;		//������Ϣ
	bool			m_bCvbs;			//�Ƿ���CVBS��ʾ��ʽ

	//��ꡢ�ƶ���Ϣ
	unsigned long	m_x_enter, m_y_enter;			//�����ʱλ��
	unsigned long	m_x_drag_last, m_y_drag_last;	//
	unsigned long	m_x_pos, m_y_pos;				//���λ��
	unsigned long	m_x_pos_last, m_y_pos_last;		//��һ�����λ��

	bool			m_bFirstDrag;					//��һ���϶����
	bool			m_bDrag;						//�Ƿ����϶�״̬
	int             m_spectype;                     //�ָ�����
	unsigned long   m_diskOwnerIndex;
private:

#if defined(__NEW_CUT_BACKUP__)
	unsigned long OnGetSliderStartTime();           //���п�ʼ
	unsigned long OnGetSliderEndTime();             //���н������Ѷ�Ӧ��ǰ�ļ���ʱ��ε��ļ�����������m_searchDataVector
	unsigned long BackupReplay();                   //���ݼ��кõĻط�
	void          CleanCutBackup();	
	GUI::CButton m_btnGetBackupStartTime;
	GUI::CButton m_btnGetBackupEndTime;
	GUI::CButton m_btnbackup;
	GUI::CStatic  m_tip;                             //���¼��к����ʾ	
	unsigned long m_sliderStartTime;                 //������ʼ�ͽ���ʱ��
	unsigned long m_sliderEndTime;	
	unsigned long m_startBackTime;                  //���ݿ�ʼʱ��ͽ���ʱ��
	unsigned long m_endBackTime;	
	unsigned long  m_curRedSelectChnn;  //���ѡ���ͨ����
	BACKUP_DATA_SEQ_VECTOR m_searchDataVector;
#else

	unsigned long GetSliderTime();//��ȡ������ʱ��,���й���
	unsigned long BackupReplay();//���ݼ��кõĻط�
	void recoverState();//�ط���ϻظ�ԭ����״̬

	//�طű��ݻ�ȡ��ť
	GUI::CStateButton m_btnGetBackupTime;
	GUI::CButton m_btnbackup;
	GUI::CStatic m_tip; //���¼��к����ʾ
	//������ʼ�ͽ���ʱ��
	unsigned long m_sliderStartTime;
	unsigned long m_sliderEndTime;

	//���ݿ�ʼʱ��ͽ���ʱ��
	unsigned long m_startBackTime;
	unsigned long m_endBackTime;
	int m_backCnt;//���¼��а�ť�Ĵ���
	enum
	{
		CUTSTART,
		CUTEND,
	};
	ULONGLONG m_chnn;
	bool m_bIsPlay;//�ж���ֹͣ���ǲ���
	int	 m_recoverStatus;

#endif
#if defined(__ASYN_PLAY__)
	ULONGLONG m_liveSelCH;
	bool m_bSync;	//�Ƿ�ͬ�����ţ�Ĭ��ͬ����
#endif
};

#endif//_PLAYER_DLG_H_

