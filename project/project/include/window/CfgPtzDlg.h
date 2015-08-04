/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : l
** Date         : 2009-08-14
** Name         : CfgPtzDlg.h
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/

#ifndef _CFG_PTZ_DLG_H_
#define _CFG_PTZ_DLG_H_

#include "CfgDlg.h"
#include "TabCtrl.h"
#include "Anchor.h"

#include "CfgPtzSerialPortDlg.h"
#include "CfgPtzAdvanceDlg.h"

using namespace GUI;

class CCfgPtzDlg : public CCfgDlg
{
public:
	CCfgPtzDlg();
	~CCfgPtzDlg();

public:
	enum PAGE_ITEM
	{
		PAGE_SERIAL,
		PAGE_ADVANCE,
	};

	void SetAnchorPage(PAGE_ITEM item);

protected:
	void OnInitial();
	unsigned long OnApply();
	unsigned long CheckValue();
	CCfgDlg* GetPage(PAGE_ITEM item);

protected:
	unsigned long OnPreCommand(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);

	DECLARE_MSG_MAP()
	unsigned long OnClickSerial();
	unsigned long OnClickAdvance();

	void GetItemList(CFGITEM_LIST& itemList) const;

protected:
	GUI::CTabCtrl			m_tabCtrl;

	CCfgPtzSerialPortDlg	m_serialPortDlg;
	CCfgPtzAdvanceDlg		m_AdvanceDlg;

	PAGE_ITEM				m_curPage;
	unsigned long			m_initPageMask;			//记录已经初始化的页面

	//
	PAGE_ITEM				m_initAnchorPage;
};



//////////////////////////////////////////////////////////////////////////
// __DVR_ANCHOR__
//////////////////////////////////////////////////////////////////////////

class CCfgPtzAnchorDlg : public GUI::CDialog
{
public:
	CCfgPtzAnchorDlg();
	~CCfgPtzAnchorDlg();

protected:
	void OnInitial();

protected:
	DECLARE_MSG_MAP()

	unsigned long OnAnchorSerialPort();
	unsigned long OnAnchorAdvance();
	unsigned long OnExit();

protected:
	GUI::CAnchor		m_anSerialPort;
	GUI::CAnchor		m_anAdvance;
	GUI::CAnchor		m_anExit;

	CCfgPtzDlg			m_cfgPtzDlg;
};

#endif

//end
