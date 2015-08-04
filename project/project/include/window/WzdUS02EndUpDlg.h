/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : ZXX
** Date         : 2012-09-25
** Name         : WzdUS02EndUpDlg.h
** Version      : 1.0
** Description  : 向导页面对话框
** Modify Record:
***********************************************************************/

#ifndef _WZD_US02_ENDUP_DLG_H_
#define  _WZD_US02_ENDUP_DLG_H_
#include "Dialog.h"
#include "WzdUS02PageDlg.h"
#include "RichTextCtrl.h"
#include "CheckCtrl.h"
using namespace GUI;



//////////////////////////////////////////////////////////////////////////
// * 向导 最后页面
//////////////////////////////////////////////////////////////////////////
class CWzdUS02EndUpDlg : public  CWzdUS02PageDlg
{
public:
	CWzdUS02EndUpDlg();

	~CWzdUS02EndUpDlg();

	void SetTipDisk(const std::string& str);

	void SetWzdInfo(US02_WzdInfo* pWzdInfo);

	void ShowWzdData(bool bRefresh);
protected:
	void OnInitial();
	virtual void InitCtrlInfo();
protected:
	DECLARE_MSG_MAP()
	unsigned long OnCheck();
protected:
	GUI::CRichTextCtrl m_richTextCtrl;
	GUI::CCheckCtrl	   m_checkCtrl;
	US02_WzdInfo       * m_pWzdInfo;
};

#endif



