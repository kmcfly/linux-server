/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : ZXX
** Date         : 2013-11-26
** Name         : WzdNatLanguageSetDlg.h
** Version      : 1.0
** Description  : 向导管理对话框
** Modify Record:
***********************************************************************/

#ifndef _WZD_NAT_LANGUAGE_SET_DLG_H_
#define _WZD_NAT_LANGUAGE_SET_DLG_H_
#include "Dialog.h"
#include "WzdNatPageDlg.h"
#include "Static.h"
#include "ComboCtrl.h"
#include "RichTextCtrl.h"
#include <string>
using namespace GUI;


class CWzdNatLanguageSetDlg:public CWzdNatPageDlg
{
public:
	CWzdNatLanguageSetDlg();
	~CWzdNatLanguageSetDlg();

	//在获取数据后设置控件信息
	void InitCtrlInfo();
	//设置向导信息
	void SetWzdInfo(NAT_WIZARD_INFO* pWzdInfo);

	//页面完成读取向导信息
	void GetWzdData(bool bNext = true);

protected:
	void OnInitial();
	void OnDestroy();

	//############监听语言改变
	DECLARE_MSG_MAP()
	unsigned long  OnChangeLanguage();

private:
	GUI::CComboCtrl m_selectLanguageCombo;
	GUI::CStatic	m_staticTip;
	GUI::CRichTextCtrl	m_richTextCtrl;

	NAT_WIZARD_INFO * m_pNatLanguageWzdInfo;

};


#endif
