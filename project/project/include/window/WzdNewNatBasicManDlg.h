/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : zxx
** Date         : 2013-11-26
** Name         : WzdNatBasicManDlg.h
** Version      : 1.0
** Description  : 向导页面对话框
** Modify Record:
***********************************************************************/

//该类为管理时间和夏令时的类

#ifndef _WZD_NEW_NAT_BASICMAN_DLG_H_
#define _WZD_NEW_NAT_BASICMAN_DLG_H_

#include "Dialog.h"
#include "WzdNatPageDlg.h"
#include "WzdNewNatTimeDlg.h"
#include "WzdNatDSTDlg.h"
#include "WzdNatLanguageSetDlg.h"
#include "TabCtrl.h"
#include "CheckCtrl.h"
using namespace GUI;

class CWzdNewNatBasicManDlg : public CWzdNatPageDlg
{
public:
	CWzdNewNatBasicManDlg();
	~CWzdNewNatBasicManDlg();
	unsigned long CheckValue();
	//设置向导信息
	void SetWzdInfo(NAT_WIZARD_INFO* pWzdInfo);
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
	CWzdNewNatTimeDlg			m_wzdNatTimeDlg;
	CWzdNatDSTDlg		    m_wzdNatDSTDlg;
	CWzdNatLanguageSetDlg	m_wzdNatLanguageSetDlg;

	WZDPAGE_ITEM		m_curPage;
	NAT_WIZARD_INFO  *  m_pWzdInfo;
};

#endif

