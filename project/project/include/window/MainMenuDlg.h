/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : hz
** Date         : 2009-08-06
** Name         : MainMenuDlg.h  
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/
#ifndef _MAIN_MENU_DLG_H_
#define _MAIN_MENU_DLG_H_
#include "Dialog.h"
#include "Static.h"
#include "Anchor.h"
#include "PictureCtrl.h"
#ifdef __SUPPORT_SNAPD1__
#include "CharCodeTable.h"
#include <string>
#endif
using namespace GUI;

class CMainMenuDlg : public GUI::CDialog
{
public:
	CMainMenuDlg();
	~CMainMenuDlg();
protected:
	DECLARE_MSG_MAP()
	void OnInitial();
	void OnPostInitial();

	//////////////////////////////////////////////////////////////////////////
	unsigned long OnBtnConfig();
	unsigned long OnBtnSearch();
	unsigned long OnBtnBackup();
	unsigned long OnBtnInfo();
	unsigned long OnBtnManualAlarm();
	unsigned long OnBtnDiskMan();
	unsigned long OnBtnSysResources();
	unsigned long OnBtnDeviceMan();
	unsigned long OnBtnLogoff();
	unsigned long OnBtnShutdown();

	unsigned long OnBtnExit();
#ifdef __SUPPORT_SNAPD1__
	unsigned long OnBtnShanghaiPTMan();
#endif
private:
	bool CreateCtrl();
	//////////////////////////////////////////////////////////////////////////
#ifndef __DVR_ANCHOR__
	GUI::CButton		m_btn[10];
	GUI::CStatic		m_static[10];
#else
	GUI::CAnchor		m_btn[10];
#endif

#ifdef __SUPPORT_SNAPD1__
	GUI::CButton		m_btnShanghaiMan;
	GUI::CStatic		m_staticShanghaiMan;
#endif

#if defined(__DVR_LOGO__) || defined(__ENVIRONMENT_WIN32__)
	GUI::CPictureCtrl	m_logoPicCtrl;
#endif
	//////////////////////////////////////////////////////////////////////////
};
#endif //_MAIN_MENU_DLG_H_
