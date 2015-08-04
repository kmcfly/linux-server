/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : ZXX
** Date         : 2013-11-25
** Name         : WzdNewNatStartDlg.h
** Version      : 1.0
** Description  : 穿透向导页面开始对话框
** Modify Record:
***********************************************************************/

#ifndef _WZD_NEW_NAT_START_DLG_H_
#define _WZD_NEW_NAT_START_DLG_H_
#include "WzdNatPageDlg.h"
#include "RichTextCtrl.h"
#include "ScrollCtrl.h"
#include "CheckCtrl.h"
using namespace GUI;

//////////////////////////////////////////////////////////////////////////
// * 向导 第一页面
//////////////////////////////////////////////////////////////////////////
class CWzdNewNatStartDlg : public CWzdNatPageDlg
{
public:
	CWzdNewNatStartDlg();
	~CWzdNewNatStartDlg();

	//设置向导信息
	void SetWzdInfo(NAT_WIZARD_INFO* pWzdInfo);
	void ShowWzdData(bool bRefresh);
	void GetWzdData(bool bNext);
	void OnLeavePage();
	
protected:
	void OnInitial();
	void OnDestroy();
	virtual void InitCtrlInfo();
	void SetTipInfo();
protected:
	DECLARE_MSG_MAP()
	unsigned long OnCheck();
	unsigned long OnScrollchange(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);
protected:
	GUI::CRichTextCtrl	m_richTextCtrl;
	GUI::CCheckCtrl		m_checkCtrl;
	GUI::CScrollCtrl	m_scroll_v;
	NAT_WIZARD_INFO     * m_pWzdInfo;
	unsigned short		m_toprow;//滚屏显示的第一行
};

#endif


