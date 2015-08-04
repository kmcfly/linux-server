/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : L
** Date         : 2009-08-15
** Name         : CfgSensorManDlg.h 
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/

#ifndef _CFG_SENSOR_MAN_DLG_H_
#define _CFG_SENSOR_MAN_DLG_H_

#include "CfgDlg.h"
#include "TabCtrl.h"
#include "Anchor.h"

#include "CfgSensorDlg.h"
#include "CfgScheduleChildDlg.h"
#include "CfgSensorHandingDlg.h"

using namespace GUI;

class CCfgSensorManDlg : public CCfgDlg
{
public:
	CCfgSensorManDlg();
	~CCfgSensorManDlg();

public:
	enum PAGE_ITEM
	{
		PAGE_SENSOR,
		PAGE_HANDING,
		PAGE_SCHEDULE,
	};

	void SetAnchorPage(PAGE_ITEM item);

protected:
	void OnInitial();

	unsigned long OnApply();
	unsigned long CheckValue();
	CCfgDlg* GetPage(PAGE_ITEM item);

protected:
	unsigned long OnPreCommand(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);

protected:
	DECLARE_MSG_MAP()
	unsigned long OnClickSensor();
	unsigned long OnClickHanding();
	unsigned long OnClickSchedule();

	void GetItemList(CFGITEM_LIST& itemList) const;

protected:
	GUI::CTabCtrl			m_tabCtrl;

	CCfgSensorDlg			m_cfgSensorDlg;
	CCfgSensorHandingDlg	m_cfgSensorHandingDlg;
	CCfgScheduleChildDlg	m_cfgSensorScheduleDlg;

	PAGE_ITEM				m_curPage;
	unsigned long			m_initPageMask;			//记录已经初始化的页面


	////
	PAGE_ITEM				m_initAnchorPage;
};


//////////////////////////////////////////////////////////////////////////
// __DVR_ANCHOR__
//////////////////////////////////////////////////////////////////////////

class CCfgSensorAnchorDlg : public GUI::CDialog
{
public:
	CCfgSensorAnchorDlg();
	~CCfgSensorAnchorDlg();

protected:
	void OnInitial();

protected:
	DECLARE_MSG_MAP()

	unsigned long OnAnchorSensor();
	unsigned long OnAnchorHanding();
	unsigned long OnAnchorSchedule();
	unsigned long OnExit();

protected:
	GUI::CAnchor		m_anSensor;
	GUI::CAnchor		m_anHanding;
	GUI::CAnchor		m_anSchedule;
	GUI::CAnchor		m_anExit;

	CCfgSensorManDlg	m_cfgSensorManDlg;
};

#endif//_CFG_SENSOR_MAN_DLG_H_
