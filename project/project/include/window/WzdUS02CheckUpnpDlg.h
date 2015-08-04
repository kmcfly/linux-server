/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : ZXX
** Date         : 2012-09-19
** Name         : WzdUS02CheckUpnpDlg.h
** Version      : 1.0
** Description  : 向导管理对话框
** Modify Record:
***********************************************************************/

#ifndef _WZD_US02_CHECKUPNP_DLG_H_
#define _WZD_US02_CHECKUPNP_DLG_H_
#include "Dialog.h"
#include "WzdUS02PageDlg.h"
#include "RichTextCtrl.h"
#include "CheckCtrl.h"
using namespace GUI;

class CWzdUS02CheckUpnpDlg : public CWzdUS02PageDlg
{
public:
	CWzdUS02CheckUpnpDlg();
	~CWzdUS02CheckUpnpDlg();
	void SetWzdInfo(US02_WzdInfo* pWzdInfo);
	//显示向导数据
	void ShowWzdData(bool bRefresh);
	//页面完成读取向导信息
	void GetWzdData(bool bNext = true);

protected:
	void OnInitial();
	void OnDestroy();
protected:
	DECLARE_MSG_MAP()
	unsigned long OnCheck();
protected:
	GUI::CRichTextCtrl m_richTextCtrl;
	US02_WzdInfo * m_pWzdInfo;
	GUI::CCheckCtrl m_checkCtrl;
private:
	bool CheckUpnp();
};


#endif