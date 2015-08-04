/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : hz
** Date         : 2009-08-01
** Name         : CfgTriggerDlg.h 
** Version      : 1.0
** Description  : 
** Modify Record: 2010-01-20
1:
***********************************************************************/
#ifndef _CFG_TRIGGER_DLG_H_
#define _CFG_TRIGGER_DLG_H_

#include "Dialog.h"
#include "TabCtrl.h"

#include "CfgDlg.h"
#include "CfgCruiseDlg.h"

#include "CfgTriggerBasicDlg.h"
#include "CfgTriggerToRecordDlg.h"
#include "CfgTriggerToPtzDlg.h"

#ifdef __ALARM_TO_UPDATE_FTP_VIDEO__
#include "CfgTriggerToFtpDlg.h"
#endif

#include "WndStructDef.h"

using namespace GUI;

class CCfgTriggerDlg : public CCfgDlg
{
public:
	CCfgTriggerDlg();
	~CCfgTriggerDlg();

public:
	enum PAGE_ITEM
	{
		PAGE_BASIC				= 0,
		PAGE_TO_RECORD,
		PAGE_TO_PTZ,
		PAGE_TO_FTP,
		PAGE_NULL				= ~(0x0),
	};

	enum PAGE_SHOW
	{
		PAGE_BASIC_SHOW				= 0x0001,
		PAGE_TO_RECORD_SHOW			= 0x0002,
		PAGE_TO_PTZ_SHOW			= 0x0004,
		PAGE_TO_FTP_SHOW			= 0X0008,
		PAGE_ALL_SHOW				= ~(0x0),
	};

public:
	void SetTriggerInfo(const ALARM_TRIGGER_INFO& triggerInfo, unsigned char chnn);
	void GetTriggerInfo(ALARM_TRIGGER_INFO& triggerInfo);
	void SetShowPage(unsigned long showPage) { m_showPage = showPage; }

	void UpdateView(bool bRefresh);

	void GetItemList(CFGITEM_LIST& itemList) const;

protected:
	void OnInitial();
	void OnPostInitial();
	void OnDestroy();

	unsigned long OnApply();

protected:
	GUI::CTabCtrl m_tabCtrl;

	CCfgTriggerBasicDlg			m_cfgTriggerBasicDlg;
	CCfgTriggerToRecordDlg		m_cfgTriggerToRecordDlg;
	CCfgTriggerToPtzDlg			m_cfgTriggerToPtzDlg;

#ifdef __ALARM_TO_UPDATE_FTP_VIDEO__
	CCfgTriggerToFtpDlg			m_cfgTriggerToFtpDlg;
#endif

	PAGE_ITEM		m_curPage;
	unsigned long	m_showPage;	//记录需要显示的页面

protected:
	unsigned char		m_channel;
	ALARM_TRIGGER_INFO	m_alarmTriggerInfo; //报警输出信息
};

#endif//_CFG_TRIGGER_DLG_H_

