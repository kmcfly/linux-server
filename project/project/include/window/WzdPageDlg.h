/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : L
** Date         : 2010-02-26
** Name         : WzdPageDlg.h
** Version      : 1.0
** Description  : 向导对话框
** Modify Record:
***********************************************************************/

#ifndef _WZD_PAGE_DLG_H_
#define _WZD_PAGE_DLG_H_

#include "Dialog.h"
#include "WndStructDef.h"

using namespace GUI;

class CWzdPageDlg : public GUI::CDialog
{
public:
	CWzdPageDlg();
	~CWzdPageDlg();

	//此页面是否使用
	void SetUsed(bool bUsed = true);
	bool IsUsed();

	//在获取数据后设置控件信息
	virtual void InitCtrlInfo();
	//设置向导信息
	virtual void SetWzdInfo(WIZARD_INFO* pWzdInfo);

	//显示向导数据
	virtual void ShowWzdData(bool bRefresh);

	//页面完成读取向导信息
	virtual void GetWzdData(bool bNext = true);

	//离开某一页面时
	virtual void OnLeavePage();

protected:
	WIZARD_INFO* m_pWzdInfo;
	bool m_bUsed;
};

#endif

