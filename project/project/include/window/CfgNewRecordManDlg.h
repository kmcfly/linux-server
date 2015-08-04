/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : L
** Date         : 2009-08-15
** Name         : CfgRecordManDlg.h
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/

#ifndef _CFG_NEW_RECORD_MAN_DLG_H_
#define _CFG_NEW_RECORD_MAN_DLG_H_

#include "Dialog.h"
#include "TabCtrl.h"
#include "Anchor.h"
#include "CfgDlg.h"

#include "CfgRecordDlg.h"
#include "CfgRecordTimeDlg.h"
#include "CfgRecordStreamDlg.h"
#include "CfgRecordRecycleDlg.h"
#include "CfgRecOsdDlg.h"
#include "CfgRecordSnapDlg.h"

#include "CfgMotionDlg.h"
#include "CfgCoverDlg.h"

#include "CfgNewMotionDlg.h"

using namespace GUI;


class CCfgNewRecordManDlg : public CCfgDlg
{
public:
	CCfgNewRecordManDlg();
	~CCfgNewRecordManDlg();

public:
	enum PAGE_ITEM
	{
		PAGE_RECORD,
		PAGE_RECORD_STREAM,
		PAGE_MOTION,
		PAGE_COVER,
		PAGE_RECORD_TIME,		
		PAGE_RECORD_OSD,
		PAGE_RECORD_RECYCLE,
		PAGE_RECORD_SNAP,
		PAGE_DAILY,		//日常图像配置
		PAGE_ALARM,	    //报警图像配置

	};

	void SetAnchorPage(PAGE_ITEM item);

protected:
	void OnInitial();
	unsigned long OnApply();
	unsigned long OnOK();

	unsigned long CheckValue();

	CCfgDlg* GetPage(PAGE_ITEM item);

	void GetItemList(CFGITEM_LIST& itemList) const;

	bool SaveConfig();
protected:
	unsigned long OnPreCommand(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);

protected:
	DECLARE_MSG_MAP()
	unsigned long OnClickRecord();
	unsigned long OnclickMotion();
	unsigned long OnClickCover();
	unsigned long OnClickRecordTime();
	unsigned long OnClickRecordStream();
	unsigned long OnClickRecordRecycle();
	unsigned long OnClickRecordOSD();
	unsigned long OnClickRecordSnap();
protected:

	GUI::CTabCtrl			m_tabCtrl;
	CCfgRecordDlg			m_cfgRecordDlg;
	CSubCfgMotionManDlg		m_cfgMotionManDlg;

	CCfgCoverDlg			m_cfgCoverDlg;
	CCfgRecordStreamDlg		m_cfgRecordStreamDlg;	
	CCfgRecordTimeDlg		m_cfgRecordTimeDlg;
	CCfgRecOsdDlg			m_cfgRecordOsdDlg;
	CCfgRecordRecycleDlg	m_cfgRecordRecycleDlg;	
	CCfgRecordSnapDlg		m_cfgRecordSnapDlg;


	PAGE_ITEM				m_curPage;
	unsigned long			m_initPageMask;			//记录已经初始化的页面

	//
	PAGE_ITEM				m_initAnchorPage;

};


//////////////////////////////////////////////////////////////////////////
// __DVR_ANCHOR__
//////////////////////////////////////////////////////////////////////////

class CCfgNewRecordManAnchorDlg : public GUI::CDialog
{
public:
	CCfgNewRecordManAnchorDlg();
	~CCfgNewRecordManAnchorDlg();

protected:
	void OnInitial();

protected:
	DECLARE_MSG_MAP()

	unsigned long OnAnchorRecord();
	unsigned long OnAnchorRecordStream();
	unsigned long OnAnchorRecordTime();
	unsigned long OnAnchorRecordOSD();
	unsigned long OnAnchorRecordRecycle();
	unsigned long OnAnchorRecordSnap();
	unsigned long OnExit();

protected:
	GUI::CAnchor		m_anRecord;
	GUI::CAnchor		m_anRecordStream;
	GUI::CAnchor		m_anRecordTime;
	GUI::CAnchor		m_anRecordOSD;
	GUI::CAnchor		m_anRecordRecycle;
	GUI::CAnchor		m_anRecordSnap;
	GUI::CAnchor		m_anExit;

	CCfgNewRecordManDlg	m_cfgRecordManDlg;
};

#endif
