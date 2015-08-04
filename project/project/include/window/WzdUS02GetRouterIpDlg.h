/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : ZXX
** Date         : 2012-09-24
** Name         : WzdUS02GetRouterIpDlg.h
** Version      : 1.0
** Description  : 向导页面对话框
** Modify Record:
***********************************************************************/

#ifndef _WZD_US02_GETROUTERIP_DLG_H_
#define _WZD_US02_GETROUTERIP_DLG_H_
#include "Dialog.h"
#include "WzdUS02PageDlg.h"
#include "RichTextCtrl.h"
#include "CheckCtrl.h"
using namespace GUI;

class CWzdUS02GetRouterIpDlg:public CWzdUS02PageDlg
{
public:
	CWzdUS02GetRouterIpDlg();
	~CWzdUS02GetRouterIpDlg();
	
	void SetWzdInfo(US02_WzdInfo* pWzdInfo);
	void ShowWzdData(bool bRefresh);
	void GetWzdData(bool bNext);
protected:
	void OnInitial();
	void OnDestroy();
protected:
	DECLARE_MSG_MAP()
	unsigned long OnCheck();
private:
	bool GetRouterIp(char *pIP);
protected:
	GUI::CRichTextCtrl	m_richTextCtrl;
	US02_WzdInfo*		m_pWzdInfo;
private:
	GUI::CCheckCtrl	    m_checkCtrl;				//下次是否显示向导
	char				m_RouterIp[32];
};
#endif
//end

