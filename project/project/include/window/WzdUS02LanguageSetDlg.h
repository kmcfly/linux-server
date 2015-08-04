/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : ZXX
** Date         : 2012-10-16
** Name         : WzdUS02LanguageSetDlg.h
** Version      : 1.0
** Description  : 向导管理对话框
** Modify Record:
***********************************************************************/

#ifndef _WZD_US02_LANGUAGE_SET_DLG_H_
#define _WZD_US02_LANGUAGE_SET_DLG_H_
#include "Dialog.h"
#include "WzdUS02PageDlg.h"
#include "Static.h"
#include "ComboCtrl.h"

using namespace GUI;


class CWzdUS02LanguageSetDlg:public CWzdUS02PageDlg
{
public:
	CWzdUS02LanguageSetDlg();
	~CWzdUS02LanguageSetDlg();

	//在获取数据后设置控件信息
	void InitCtrlInfo();
	//设置向导信息
	void SetWzdInfo(US02_WzdInfo* pWzdInfo);
	
	//页面完成读取向导信息
	void GetWzdData(bool bNext = true);

protected:
	void OnInitial();
	void OnDestroy();

	//############监听语言改变
	DECLARE_MSG_MAP()
	unsigned long OnChangeLanguage();
private:
	GUI::CComboCtrl m_selectLanguageCombo;
	GUI::CStatic	m_staticTip;

	US02_WzdInfo * m_pUS02LanguageWzdInfo;

};



#endif
