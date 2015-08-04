/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : l
** Date         : 2009-08-11
** Name         : CfgBasicManDlg.h
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

#ifndef _CFG_BASIC_MAN_DLG_H_
#define _CFG_BASIC_MAN_DLG_H_

///---------------------------------------------------------------------
// Include
///---------------------------------------------------------------------
#include "CfgDlg.h"
#include "TabCtrl.h"
#include "Button.h"
#include "Anchor.h"

#include "CfgSystemDlg.h"
#include "CfgDateTimeDlg.h"
#include "CfgNewDateTimeDlg.h"
#include "CfgDaylightDlg.h"
#include "LocalDevice.h"

#ifdef __MOBILE_DVR__
#include "CfgDriverInfoDlg.h"
#endif

using namespace GUI;

///---------------------------------------------------------------------
// Predefine
///---------------------------------------------------------------------

///---------------------------------------------------------------------
// Class Define
///---------------------------------------------------------------------
class CCfgBasicManDlg : public CCfgDlg
{
public:
	CCfgBasicManDlg();
	~CCfgBasicManDlg();

public:
	enum PAGE_ITEM
	{
		PAGE_SYSTEM,
		PAGE_TIME,
#if !defined(__TW01_RILI__)
		PAGE_DST,
#endif
#ifdef __MOBILE_DVR__
		PAGE_DRIVER,
#endif
	};

	void SetAnchorPage(PAGE_ITEM item);

protected:
	void OnInitial();
	void OnDestroy();
	unsigned long OnApply();
	unsigned long CheckValue();

protected:
	CCfgDlg* GetPage(PAGE_ITEM item);

protected:
	unsigned long OnPreCommand(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);
protected:
	DECLARE_MSG_MAP()
	unsigned long OnClickSystem();
	unsigned long OnClickDateTime();
	unsigned long OnClickDst();
	unsigned long OnOK();

	void GetItemList(CFGITEM_LIST& itemList) const;

protected:
	GUI::CTabCtrl		m_tabCtrl;

	CCfgSystemDlg		m_cfgSystemDlg;
	CCfgDateTimeDlg		m_cfgDateTimeDlg;
	CCfgNewDateTimeDlg	m_cfgNewDateTimeDlg;
	CCfgDaylightDlg		m_cfgDaylightDlg;

	PAGE_ITEM			m_curPage;
	unsigned long		m_initPageMask;			//记录已经初始化的页面

	//void SavePage(PAGE_ITEM pageItem);
	//void LoadPage(PAGE_ITEM pageItem, bool bRefresh);

#ifdef __MOBILE_DVR__
	unsigned long OnDriverInfoDst();
	CCfgDriverInfoDlg   m_cfgDriverInfoDlg;
#endif
	
	//
	PAGE_ITEM			m_initAnchorPage;
};


//////////////////////////////////////////////////////////////////////////
// __DVR_ANCHOR__
//////////////////////////////////////////////////////////////////////////

class CCfgBasicManAnchorDlg : public GUI::CDialog
{
public:
	CCfgBasicManAnchorDlg();
	~CCfgBasicManAnchorDlg();

protected:
	void OnInitial();

protected:
	DECLARE_MSG_MAP()

	unsigned long OnAnchorSystem();
	unsigned long OnAnchorDateTime();
	unsigned long OnAnchorDaylight();
	unsigned long OnAnchorDriverInfo();
	unsigned long OnExit();

protected:
	GUI::CAnchor		m_anSystem;
	GUI::CAnchor		m_anDateTime;
	GUI::CAnchor		m_anDaylight;
	GUI::CAnchor		m_anDriverInfo;
	GUI::CAnchor		m_anExit;

	CCfgBasicManDlg		m_cfgBasicManDlg;
};

#endif

//end
