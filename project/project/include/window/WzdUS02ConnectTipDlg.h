/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : zxx
** Date         : 2012-09-18
** Name         : WzdUS02ConnectTipDlg.h
** Version      : 1.0
** Description  : 向导页面对话框
** Modify Record:
***********************************************************************/

#ifndef _WZD_US02_CONNECTTIP_DLG_H_
#define _WZD_US02_CONNECTTIP_DLG_H_

#include "Dialog.h"
#include "RichTextCtrl.h"
#include "WzdUS02PageDlg.h"
#include "CheckCtrl.h"
using namespace GUI;

class CWzdUS02ConnectTipDlg : public CWzdUS02PageDlg
{
public:
	CWzdUS02ConnectTipDlg();
	~CWzdUS02ConnectTipDlg();
	void SetTipDisk(const std::string& str);
	void SetWzdInfo(US02_WzdInfo * pWzdInfo);
	void ShowWzdData(bool bRefresh);
	
protected:
	void OnInitial();
	void OnDestroy();
protected:
	DECLARE_MSG_MAP()
	unsigned long OnCheck();
protected:
	GUI::CRichTextCtrl	m_richTextCtrl;
	GUI::CCheckCtrl		m_checkCtrl;				//下次是否显示向导
	US02_WzdInfo	    * m_pWzdInfo;
};


#endif
