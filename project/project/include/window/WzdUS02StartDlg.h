/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : ZXX
** Date         : 2012-09-17
** Name         : WzdUS02StartDlg.h
** Version      : 1.0
** Description  : 向导页面对话框
** Modify Record:
***********************************************************************/

#ifndef _WZD_US02_START_DLG_H_
#define _WZD_US02_START_DLG_H_
#include "Dialog.h"
#include "WzdUS02PageDlg.h"
#include "RichTextCtrl.h"
#include "CheckCtrl.h"
using namespace GUI;

//////////////////////////////////////////////////////////////////////////
// * 向导 第一页面
//////////////////////////////////////////////////////////////////////////
class CWzdUS02StartDlg : public CWzdUS02PageDlg
{
public:
	CWzdUS02StartDlg();

	~CWzdUS02StartDlg();

	//设置向导信息
	void SetWzdInfo(US02_WzdInfo* pWzdInfo);
	void ShowWzdData(bool bRefresh);
	void OnLeavePage();
	void SetTipDisk(const std::string& str);
protected:
	void OnInitial();
	virtual void InitCtrlInfo();
protected:
	DECLARE_MSG_MAP()
	unsigned long OnCheck();
protected:
	GUI::CRichTextCtrl	m_richTextCtrl;
	GUI::CCheckCtrl		m_checkCtrl;
	US02_WzdInfo        * m_pWzdInfo;
};

#endif


