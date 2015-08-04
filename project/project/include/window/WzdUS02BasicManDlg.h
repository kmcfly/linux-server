/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : zxx
** Date         : 2012-09-18
** Name         : WzdUS02BasicManDlg.h
** Version      : 1.0
** Description  : 向导页面对话框
** Modify Record:
***********************************************************************/

//该类为管理时间和夏令时的类

#ifndef _WZD_US02_BASICMAN_DLG_H_
#define _WZD_US02_BASICMAN_DLG_H_

#include "Dialog.h"
#include "WzdUS02PageDlg.h"
#include "WzdUS02TimeDlg.h"
#include "WzdUS02DSTDlg.h"
#include "WzdUS02LanguageSetDlg.h"
#include "TabCtrl.h"
#include "CheckCtrl.h"
using namespace GUI;

class CWzdUS02BasicManDlg : public CWzdUS02PageDlg
{
public:
	CWzdUS02BasicManDlg();
	~CWzdUS02BasicManDlg();
	unsigned long CheckValue();
	//设置向导信息
	void SetWzdInfo(US02_WzdInfo* pWzdInfo);
	void InitCtrlInfo();
	//显示向导数据
	void ShowWzdData(bool bRefresh);

	//页面完成读取向导信息
	virtual void GetWzdData(bool bNext = true);

	void OnLeavePage();
	
	bool GetCurrZoneTimeInfo(ZONE_TIME_INFO& zoneTimeInfo, int zone);
public:
	enum WZDPAGE_ITEM
	{
		PAGE_LANGUAGE,
		PAGE_TIME,
		PAGE_DST
	};

protected:
	void OnInitial();
	void OnDestroy();
protected:
	DECLARE_MSG_MAP()
	unsigned long  OnCheck();
protected:
	GUI::CTabCtrl			m_tabCtrl;
	GUI::CCheckCtrl			m_checkCtrl;				//下次是否显示向导
	CWzdUS02TimeDlg			m_wzdUS02TimeDlg;
	CWzdUS02DSTDlg		    m_wzdUS02DSTDlg;
	CWzdUS02LanguageSetDlg	m_wzdUS02LanguageSetDlg;

	WZDPAGE_ITEM		m_curPage;
	US02_WzdInfo*		m_pWzdInfo;
};

#endif

