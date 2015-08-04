/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : hz
** Date         : 2009-08-01
** Name         : CfgAlarmManDlg.h 
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/
#ifndef _CFG_ALARM_OUT_DLG_H_
#define _CFG_ALARM_OUT_DLG_H_

#include "CfgDlg.h"
#include "ListCtrl.h"
#include "ComboCtrl.h"
#include "Static.h"
#include "Anchor.h"

#include "CfgManDlg.h"
#include "CfgManDlg.cpp"
#include "CfgScheduleChildDlg.h"
#include "CfgAlarmOutBuzzerDlg.h"

using namespace GUI;

class CCfgAlarmOutDlg : public CCfgDlg
{
public:
	CCfgAlarmOutDlg();
	~CCfgAlarmOutDlg();

protected:
	void OnInitial();

protected:
	//ÖØÐ´
	void UpdateView(bool bRefresh);
	bool ShowData(NCFG_ITEM_ID cfgID, const BYTE* pData, int len, bool bRefresh);
	bool SetData(NCFG_ITEM_ID cfgID, BYTE*& pData, int& len)  const;

protected:
	DECLARE_MSG_MAP()
	unsigned long OnClickList(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);
	unsigned long OnClickListAll(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);

protected:
	GUI::CListCtrl	m_listCtrl;

	GUI::CListCtrl m_listAll;
	GUI::CStatic   m_scAll;

	static unsigned long s_holdTime[];

protected:
	enum COL_INDEX
	{
		COL_CHANNEL,
		COL_NAME,
		COL_HOLD_TIME,
	};
};

//////////////////////////////////////////////////////////////////////////

class CCfgAlarmOutManDlg : public CCfgManDlg3<CCfgAlarmOutDlg, CCfgScheduleChildDlg, CCfgAlarmOutBuzzerDlg>
{
public:
	CCfgAlarmOutManDlg();
	~CCfgAlarmOutManDlg();
protected:
	void OnPreInitial();
	void GetItemList(CFGITEM_LIST& itemList) const;
};


//////////////////////////////////////////////////////////////////////////
// __DVR_ANCHOR__
//////////////////////////////////////////////////////////////////////////

class CCfgAlarmOutManAnchorDlg : public GUI::CDialog
{
public:
	CCfgAlarmOutManAnchorDlg();
	~CCfgAlarmOutManAnchorDlg();

protected:
	void OnInitial();

protected:
	DECLARE_MSG_MAP()

	unsigned long OnAnchorAlarmOut();
	unsigned long OnAnchorSchedule();
	unsigned long OnAnchorBuzzer();
	unsigned long OnExit();

protected:
	GUI::CAnchor		m_anAlarmOut;
	GUI::CAnchor		m_anSchedule;
	GUI::CAnchor		m_anBuzzer;
	GUI::CAnchor		m_anExit;

	CCfgAlarmOutManDlg	m_cfgAlarmOutManDlg;
};

#endif//_CFG_ALARM_OUT_DLG_H_

//end
