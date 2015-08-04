/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : lufen
** Date         : 2010-03-29
** Name         : 
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/

#ifndef _CFG_SPEED_ALARM_DLG_H_
#define _CFG_SPEED_ALARM_DLG_H_

#include "CfgDlg.h"
#include "TabCtrl.h"
#include "Anchor.h"

//#include "Button.h"
#include "CfgSpeedAlmBasicDlg.h"
#include "CfgSpeedAlmOutputDlg.h"

using namespace GUI;

class CCfgSpeedAlarmDlg : public CCfgDlg
{
public:
	CCfgSpeedAlarmDlg();
	~CCfgSpeedAlarmDlg();

public:
	enum PAGE_ITEM
	{
		PAGE_BASIC,
		PAGE_OUTPUT,
	};

	void SetAnchorPage(PAGE_ITEM item);

protected:
	void OnInitial();
	void OnDestroy();
	unsigned long OnApply();
	unsigned long CheckValue();
	CCfgDlg* GetPage(PAGE_ITEM item);

protected: 
	DECLARE_MSG_MAP()
	unsigned long OnClickBasic();
	unsigned long OnOutputDlg();
	unsigned long OnPreCommand(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);
	void GetItemList(CFGITEM_LIST& itemList) const;

protected:
	GUI::CTabCtrl	m_tabCtrl;

	CCfgSpeedAlmBasicDlg m_cfgSpeedAlmBasicDlg;
	CCfgSpeedAlmOutputDlg m_cfgSpeedAlmOutputDlg;

	PAGE_ITEM m_curPage;

	//
	PAGE_ITEM m_initAnchorPage;
};



//////////////////////////////////////////////////////////////////////////
// __DVR_ANCHOR__
//////////////////////////////////////////////////////////////////////////

class CCfgSpeedAlarmAnchorDlg : public GUI::CDialog
{
public:
	CCfgSpeedAlarmAnchorDlg();
	~CCfgSpeedAlarmAnchorDlg();

protected:
	void OnInitial();

protected:
	DECLARE_MSG_MAP()

	unsigned long OnAnchorSpeedAlmBasic();
	unsigned long OnAnchorSpeedAlmOutput();
	unsigned long OnExit();

protected:
	GUI::CAnchor		m_anSpeedAlmBasic;
	GUI::CAnchor		m_anSpeedAlmOutput;
	GUI::CAnchor		m_anExit;

	CCfgSpeedAlarmDlg	m_cfgSpeedAlarmDlg;
};

#endif

//end
