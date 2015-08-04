/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : hz
** Date         : 2009-08-12
** Name         : CfgScheduleDlg.h
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/

#ifndef _CFG_SCHEDULE_DLG_H_
#define _CFG_SCHEDULE_DLG_H_

#include "CfgDlg.h"
#include "TabCtrl.h"
#include "Anchor.h"

#include "CfgScheduleChildDlg.h"

using namespace GUI;

class CCfgScheduleDlg : public CCfgDlg
{
public:
	CCfgScheduleDlg();
	~CCfgScheduleDlg();

public:
	enum SCHEDULE_TYPE
	{
		SCHEDULE_RECORD = 0,
		SCHEDULE_MOTION,
		SCHEDULE_ALARM,

		SCHEDULE_COUNT,
	};

	void SetAnchorPage(SCHEDULE_TYPE item);

protected:
	void OnInitial();

	unsigned long OnApply();

	unsigned long CheckValue();

	CCfgDlg* GetPage(SCHEDULE_TYPE item);

protected:
	unsigned long OnPreCommand(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);

	DECLARE_MSG_MAP()
	void GetItemList(CFGITEM_LIST& itemList) const;

	unsigned long OnClickRecord();
	unsigned long OnClickMotion();
	unsigned long OnClickAlarm();

protected:
	GUI::CTabCtrl			m_tabCtrl;

	CCfgScheduleChildDlg	m_childDlg[SCHEDULE_COUNT];
	SCHEDULE_TYPE			m_curPage;
	unsigned long			m_initPageMask;			//记录已经初始化的页面

	//
	SCHEDULE_TYPE			m_initAnchorPage;
};


//////////////////////////////////////////////////////////////////////////
// __DVR_ANCHOR__
//////////////////////////////////////////////////////////////////////////

class CCfgScheduleAnchorDlg : public GUI::CDialog
{
public:
	CCfgScheduleAnchorDlg();
	~CCfgScheduleAnchorDlg();

protected:
	void OnInitial();

protected:
	DECLARE_MSG_MAP()

	unsigned long OnAnchorRecord();
	unsigned long OnAnchorMotion();
	unsigned long OnAnchorAlarm();
	unsigned long OnExit();

protected:
	GUI::CAnchor		m_anRecord;
	GUI::CAnchor		m_anMotion;
	GUI::CAnchor		m_anAlarm;
	GUI::CAnchor		m_anExit;

	CCfgScheduleDlg		m_cfgScheduleDlg;
};


#endif

//end
