/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : l
** Date         : 2009-08-11
** Name         : CfgSystemManDlg.h
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/

/***********************************************************************
** UI View
** ---------------------------------------------------------------------
** 
** 
** 
** 
** 
***********************************************************************/

#ifndef _CFG_SYSTEM_MAN_DLG_H_
#define _CFG_SYSTEM_MAN_DLG_H_

///---------------------------------------------------------------------
// Include
///---------------------------------------------------------------------
#include "Dialog.h"
#include "Static.h"
#include "Anchor.h"

#include "CfgBasicManDlg.h"
#include "CfgLiveManDlg.h"
#include "CfgRecordManDlg.h"
#include "CfgScheduleDlg.h"
#include "CfgAlarmManDlg.h"
#include "CfgNetworkManDlg.h"
#include "CfgUserDlg.h"
#include "CfgPtzDlg.h"
#include "CfgAdvanceDlg.h"

#include "CfgNewLiveManDlg.h"
#include "CfgNewRecordManDlg.h"
#include "CfgNewAlarmManDlg.h"

#ifdef __GB28181_SERVER__
#include "CfGb28181Dlg.h"
#endif
using namespace GUI;

///---------------------------------------------------------------------
// Predefine
///---------------------------------------------------------------------

///---------------------------------------------------------------------
// Class Define
///---------------------------------------------------------------------
class CCfgSystemManDlg : public GUI::CDialog
{
public:
	CCfgSystemManDlg();
	~CCfgSystemManDlg();

public:/// 公共接口函数

protected:/// 继承函数
	void OnInitial();
	void OnDestroy();

protected:/// 功能函数
	bool CreateCtrl();

protected:/// 消息响应
	DECLARE_MSG_MAP()
	unsigned long OnBtnCfgBasic();
	unsigned long OnBtnCfgLive();
	unsigned long OnBtnCfgRecord();
	unsigned long OnBtnCfgSchedule();
	unsigned long OnBtnCfgAlarm();
	unsigned long OnBtnCfgNetwork();
	unsigned long OnBtnCfgUsers();
	unsigned long OnBtnCfgPTZ();
	unsigned long OnBtnCfgAdvance();
	unsigned long OnExit();
#ifdef __GB28181_SERVER__
	unsigned long OnBtnCfgGb28181();
#endif

protected:
	bool EnterConfig();
	void ExitConfig() const;

	bool EnterPtz();
	void ExitPtz() const;

	bool CheckAuthoritySetup();

#ifndef __DVR_ANCHOR__
	CCfgBasicManDlg			m_cfgBasicManDlg;

	#ifdef __CUSTOM_US02_NEW__
		CCfgNewLiveManDlg		m_cfgLiveManDlg;
		CCfgNewRecordManDlg		m_cfgRecordManDlg;
	#else
		CCfgLiveManDlg			m_cfgLiveManDlg;
		CCfgRecordManDlg		m_cfgRecordManDlg;
	#endif

	CCfgScheduleDlg			m_cfgScheduleDlg;
	CCfgNetworkManDlg		m_cfgNetworkManDlg;
	CCfgPtzDlg				m_cfgPtzDlg;

#else
	CCfgBasicManAnchorDlg	m_cfgBasicManDlg;
	CCfgLiveManAnchorDlg	m_cfgLiveManDlg;
	CCfgRecordManAnchorDlg	m_cfgRecordManDlg;
	CCfgScheduleAnchorDlg	m_cfgScheduleDlg;
	CCfgNetworkManAnchorDlg m_cfgNetworkManDlg;
	CCfgPtzAnchorDlg		m_cfgPtzDlg;
#endif
	#ifdef __CUSTOM_US02_NEW__
		CCfgNewAlarmManDlg		m_cfgAlarmManDlg;
	#else
		CCfgAlarmManDlg		m_cfgAlarmManDlg;
	#endif
	CCfgUserDlg			m_cfgUserDlg;
	CCfgAdvanceDlg		m_cfgAdvanceDlg;
#ifdef __GB28181_SERVER__
	CCfGb28281Dlg		m_cfgGb28181Dlg;
#endif

	enum SYSTEM_MAN_BTN_NAME
	{
		SYSTEM_MAN_BTN_BASIC,
		SYSTEM_MAN_BTN_LIVE,
		SYSTEM_MAN_BTN_RECORD,
		SYSTEM_MAN_BTN_SCHEDULE,
		SYSTEM_MAN_BTN_ALARM,
#if !defined(__TW01_RILI__)
		SYSTEM_MAN_BTN_NETWORK,
#endif

		SYSTEM_MAN_BTN_USERS,
#if !defined(__TW01_RILI__)		
		SYSTEM_MAN_BTN_PTZ,
#endif

		SYSTEM_MAN_BTN_ADVANCE,
#ifdef __GB28181_SERVER__
		SYSTEM_MAN_BTN_GB28181,
#endif
		SYSTEM_MAN_BTN_COUNT,
	};

#ifndef __DVR_ANCHOR__
	GUI::CButton	m_btn[SYSTEM_MAN_BTN_COUNT];
	GUI::CStatic	m_static[SYSTEM_MAN_BTN_COUNT];
#else
	GUI::CAnchor	m_btn[SYSTEM_MAN_BTN_COUNT];
#endif
};


#endif
