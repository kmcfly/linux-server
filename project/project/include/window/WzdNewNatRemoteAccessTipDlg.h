/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : 
** Name         : 
** Version      : 
** Description  : 向导页面对话框
** Modify Record:
***********************************************************************/

#ifndef _WZD_NEW_NAT_REMOTEACCESS_TIP_DLG_H_
#define _WZD_NEW_NAT_REMOTEACCESS_TIP_DLG_H_

#include "Dialog.h"
#include "RichTextCtrl.h"
#include "WzdNatPageDlg.h"
#include "ScrollCtrl.h"
#include "CheckCtrl.h"
using namespace GUI;

class CWzdNewNatRemoteAccessTipDlg : public CWzdNatPageDlg
{
public:
	CWzdNewNatRemoteAccessTipDlg();
	~CWzdNewNatRemoteAccessTipDlg();

	void SetWzdInfo(NAT_WIZARD_INFO * pWzdInfo);
	void ShowWzdData(bool bRefresh);

protected:
	void OnInitial();
	void OnDestroy();
	void SetTipInfo();
protected:
	DECLARE_MSG_MAP()
	unsigned long OnCheck();
protected:
	GUI::CRichTextCtrl	m_richTextCtrl;
	GUI::CCheckCtrl		m_checkCtrl;				//下次是否显示向导
	GUI::CScrollCtrl	m_scroll_v;
	NAT_WIZARD_INFO	    * m_pWzdInfo;
};


#endif
