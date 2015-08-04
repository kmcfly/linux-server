/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : ysw
** Date         : 2009-07-22
** Name         : Disktop.h
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/
#ifndef _DISKTOP_H_
#define _DISKTOP_H_
#include "Static.h"
#include "GroupCtrl.h"
#include "Dialog.h"
#include "ToolBar.h"
#include "ToolBarMenu.h"
#include "CfgSystemManDlg.h"
#include "SearchManDlg.h"
#include "InfoManDlg.h"
#include "MainMenuDlg.h"
#include "PTZViewDlg.h"
#include "PTZLiveCtrl.h"
#include "AudioControlDlg.h"
#include "ManualAlarmDlg.h"
#include "DiskManDlg.h"
#include "DiskAdvDlg.h"
#include "SystemUpgradeDlg.h"
#include "DeviceManDlg.h"

#include "BackupDlg.h"
#include "LoginDlg.h"

#include "SelectChnnDlg.h"
#include "SelectDiskDlg.h"

#include "WndStructDef.h"
#include "WzdManDlg.h"
#include "ZoomDlg.h"
#include "FuncCustom.h"

#include "CfgLiveColorDlg.h"
#include "SetCamParaDlg.h"
#ifndef NDEBUG
#include "DebugSetupDlg.h"
#endif

#include "SelectReplayTimeDlg.h"

#ifdef __CUSTOM_US02__
#include "WzdUS02ManDlg.h"
#include "WzdNewNatManDlg.h"
#include "LocalDevice.h"
#endif


#include "WzdNatManDlg.h"

#include "TipWindow.h"

using namespace GUI;


typedef struct _channel_info
{
	int   channel;
	int   xPos;
	int   yPos;
	int   xWidth;
	int   yHeight;
}CHANNEL_INFO;

typedef CMyList <CHANNEL_INFO> CHANNEL_INFO_LIST;



class CDisktop : public GUI::CDialog
{
public:
	CDisktop();
	~CDisktop();

	void Quit();
protected:
	DECLARE_MSG_MAP()
	void OnInitial();
	void OnPostInitial();
	void OnPaint(GUI::PAINT_STATUS status);
	void OnDestroy();

	void EndDialog(DLG_END_VALUE result);
	unsigned long OnVirtualEsc(unsigned long keyID, unsigned long xPos, unsigned long yPos);
	void OnIdle();
	void OnExitProc();
	unsigned long OnPreCommand(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);
private:
	//////////////////////////////////////////////////////////////////////////
	unsigned long On6X6();
	unsigned long On5X5();
	unsigned long On4X4();
	unsigned long On1A12();
	unsigned long On1A7();
	unsigned long On3X3();
	unsigned long On1A5();
	unsigned long On2X3();
	unsigned long On2X2();
	unsigned long On1X1();
	unsigned long OnSplit();
	unsigned long OnSplitEx();
	unsigned long OnGroup();
	unsigned long OnGroupEx();

	unsigned long On6X6Ex();
	unsigned long On5X5Ex();
	unsigned long On4X4Ex();
	unsigned long On1A12EX();
	unsigned long On1A7Ex();
	unsigned long On3X3Ex();
	unsigned long On1A5Ex();
	unsigned long On2X3Ex();
	unsigned long On2X2Ex();
	unsigned long On1X1Ex();
	unsigned long OnPIP();

	//tip window
	#ifdef __CUSTOM_US02_NEW__
	unsigned long OnOver();
	unsigned long OnOut();
	#endif

	unsigned long OnInputChnnNum( unsigned long ctrlID );							//切换通道到对应按键值

	unsigned long OnMainMenu();
	unsigned long OnSearch();
	unsigned long OnBackup();
	unsigned long OnInfo();
	unsigned long OnLog();

	unsigned long OnDwell();
	unsigned long OnZoom();
	unsigned long OnAudio();
	unsigned long OnPTZ();
	unsigned long OnRec();
	unsigned long OnPlay();
	unsigned long OnShutDown();
	unsigned long OnHide();
	unsigned long OnSnap();
	unsigned long OnPlayEx();
	unsigned long OnBurn();
	unsigned long OnBurnEx();
	unsigned long OnSysInfo();
	bool OnSelectBurn(unsigned long xPos, int chnnNum, int maxSel, ULONGLONG & CH);

	unsigned long OnConfig();
	unsigned long OnManualAlarm();
	unsigned long OnDiskMan();
	unsigned long OnSysRescouces();
	unsigned long OnDeviceMan();
	unsigned long OnLogoff();

	unsigned long OnChangeColor();
	unsigned long OnChangeToolStatus();

	unsigned long OnChangeCamPara();

	unsigned long OnClickMainMenu(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);
    unsigned long OnDoubleClick(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);

	bool OnSelectCH(unsigned long xPos, int chnnNum, int maxSel, ULONGLONG & CH,unsigned long splitMode);
	bool OnChangeCH(ULONGLONG & CH, unsigned long splitMode, unsigned long n, bool bUseSubMask = true);

	//向低端切换通道
	bool OnChangeCHEx(ULONGLONG & CH, unsigned long splitMode, unsigned long n);
	//飞梭右旋
	unsigned long OnFlyRight(unsigned long keyID);
	//飞梭左旋
	unsigned long OnFlyLeft(unsigned long keyID);
	unsigned long OnFlyMedium();

	//记录鼠标移动坐标
#ifndef __DVR_MENU__
	unsigned long OnClickToolBar(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);
#endif

	unsigned long OnEnter(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);
	unsigned long OnMouseMove(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);
	unsigned long OnMouseDrag(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);
	unsigned long OnMouseLeftUp(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);

	//////////////////////////////////////////////////////////////////////////
	//检查权限，登录
	ULONGLONG CheckAuthority(USER_AUTHORITY authority, bool bReDisplay = false);

	//获取当前的显示模式
	bool GetCurDisplay(NET_DISPLAY_INFO& disp, unsigned short x = 0, unsigned short y = 0, bool bUsePt = false);
	
	//改变当前显示模式
	bool ChangeCurDisplay( unsigned long keyID );

	void ChangeDisplayByCH(ULONGLONG CH, unsigned long splitMode, int chnnMini = -1, unsigned long timeout = 0, unsigned short bRec = 0x1, unsigned short bFilter=false);
	void ChangeDisplayByPt(unsigned short x, unsigned short y, unsigned long timeout = 0, unsigned short bRec = 0x1);
	void ChangeDisplayByTwoPt(unsigned short x1, unsigned short y1, unsigned short x2, unsigned short y2, unsigned long timeout = 0, unsigned short bRec = 0x1);
	bool ChangeDisplayToDwellGroup();
	//ULONGLONG GetMainMonitorCfg();

	void HideSnapTip();
	bool EnterConfig();
	void ExitConfig() const;

	bool IsAlmostEqual(unsigned long x1, unsigned long y1, unsigned long x2, unsigned long y2, unsigned long distance);
	//////////////////////////////////////////////////////////////////////////
	void GetDiskOwer(DVR_DISK_OWNER_LIST &diskOwnerList);
	//////////////////////////////////////////////////////////////////////////
	void OnWizzard(); //向导过程
	//////////////////////////////////////////////////////////////////////////
	DLG_END_VALUE PopupLiveColorDlg( int x, int y, int cx, int cy, int chnn );
	//////////////////////////////////////////////////////////////////////////
	DLG_END_VALUE PopupLiveSetCamParaDlg(int x, int y, int cx, int cy, int chnn );
#ifdef __CUSTOM_US02__ 
	//US02专用向导
	void OnNewNatWizzard();
	void OnUS02Wizzard();
#endif


	void OnNatWizarrd();
	CMessageMan			*m_pMsgMan;
	VIDEO_FORMAT		m_videoFormat;
	//////////////////////////////////////////////////////////////////////////
	unsigned short		m_timeXPos;
	unsigned short		m_timeYPos;
	unsigned short		m_timeWidth;
	unsigned short		m_timeHeight;

	unsigned long		m_xPos;
	unsigned long		m_yPos;

	ULONGLONG			m_1X1CH;
	ULONGLONG			m_2X2CH;
	ULONGLONG			m_2X3CH;
	ULONGLONG			m_1A5CH;
	ULONGLONG			m_3X3CH;
	ULONGLONG			m_1A7CH;
	ULONGLONG			m_1A12CH;
	ULONGLONG			m_4X4CH;
	ULONGLONG			m_5X5CH;
	ULONGLONG			m_6X6CH;
	unsigned long		m_currSplitMode;

	LONGLONG			m_toolBarLastTime;
	LONGLONG			m_lastShowTime;
	LONGLONG			m_showTime;
	std::string			m_strTime;
	GUI::CStatic		m_stcTime;

#ifdef __DVR_MENU__
	CToolBarMenu		m_toolBarMenu;
#else
	GUI::CToolBar		m_toolBar;
#endif

	CTipWindow*			m_pTipWindow;

	CSearchManDlg		m_searchMan;
	CInfoManDlg			m_infoMan;
	CCfgSystemManDlg	m_cfgSystemMan;
	CPTZViewDlg			m_ptzViewDlg;
	CPTZLiveCtrl		m_ptzLiveCtrl;
	CBackupDlg			m_backupDlg;
	CDeviceManDlg		m_deviceManDlg;
	CLogInfoDlg			m_logMan;

	CManualAlarmDlg		m_manualAlarm;
	CDiskTableManDlg	m_diskMan;
	CSystemUpgradeDlg		m_update;
	CAudioControlDlg	m_audioControl;

	CMainMenuDlg		m_mainMenu;
	CLoginDlg			m_loginDlg;
	CSelectChnnDlg		m_selectChnnDlg;

	int					m_selTime;
	CSelectReplayTimeDlg m_selReplayTimeDlg;
	bool				m_bShowToolBar;
	CWzdManDlg			m_wzdManDlg;
	CZoomDlg			m_zoomDlg;

	CCfgLiveColorDlg	m_changeColorDlg;
	CSetCamParaDlg      m_setCamParaDlg;
#ifndef NDEBUG
	CDebugSetupDlg		m_debugSetupDlg;
#endif
	//////////////////////////////////////////////////////////////////////////
	//现场显示。由于其他地方可以改变现场,因此只能通过命令CMD_REQUEST_GET_DISPLAY_INFO
	//实时获取当前显示状态
	NET_DISPLAY_INFO	m_displayinfo; //此变量并没有跟随记录现场的状态,只临时使用

	//

	unsigned short		m_diskAlarmXPos;
	unsigned short		m_diskAlarmYPos;
	unsigned short		m_diskAlarmWidth;
	unsigned short		m_diskAlarmHeight;
	GUI::CStatic        m_stcDiskAlarm;

	unsigned long		m_keyFlyRightID;
	unsigned long		m_keyFlyLeftID;

	WND_STARTUP_INFO*	m_pWndStartupInfo;		//

	unsigned long		m_countSnapTip;			//抓图提示计时

	////拖动通道
	GUI::CGroupCtrl	m_groupCtrl;		
	bool			m_bDragChnn;
	bool			m_bFirstDragChnn;
	bool			m_bDraged;
	unsigned long	m_lastDragPosX;
	unsigned long	m_lastDragPosY;
	unsigned long	m_enterDragPosX;
	unsigned long	m_enterDragPosY;
	unsigned long	m_countGroupCtrl;
	bool			m_bNoMenuMsg;
	int				m_startTime;
	unsigned long	m_burning;
#ifdef __CUSTOM_US02__ 
	CWzdUS02ManDlg m_WzdUS02ManDlg;
	CWzdNewNatManDlg m_WzdNewNatManDlg;
#endif

#if 0
	CCfgDevColorDlg m_changeDevColorDlg;
#endif
	CWzdNatManDlg	m_wzdNatManDlg;
};
#endif //_DISKTOP_H_
