/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : ZXX
** Date         : 2012-10-17
** Name         : WzdUS02MobiletipDlg.h
** Version      : 1.0
** Description  : 向导管理对话框
** Modify Record:
***********************************************************************/

#ifndef _WZD_US02_MOBILETIP_DLG_H_
#define _WZD_US02_MOBILETIP_DLG_H_
#include "Dialog.h"
#include "WzdUS02PageDlg.h"
#include "RichTextCtrl.h"
#include "CheckCtrl.h"
#include <string>
using namespace GUI;

class CWzdUS02MobiletipDlg:public CWzdUS02PageDlg
{
public:
	CWzdUS02MobiletipDlg();
	~CWzdUS02MobiletipDlg();
	void SetWzdInfo(US02_WzdInfo* pWzdInfo);
	void ShowWzdData(bool bRefresh);
protected:
	void OnInitial();
	void OnPostInitial();
	void OnDestroy();
protected:
	DECLARE_MSG_MAP()
	unsigned long OnCheck();
private:
	GUI::CRichTextCtrl  m_richText;
	GUI::CCheckCtrl		m_checkCtrl;				//下次是否显示向导

	US02_WzdInfo		*m_pWzdInfo;
	enum Mobile_TYPE
	{
		MOBILE_APPLE,
		MOBILE_ANDROID,
	};

};










#endif
