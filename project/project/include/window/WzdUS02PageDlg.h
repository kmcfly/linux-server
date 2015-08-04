/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : ZXX
** Date         : 2012-09-17
** Name         : WzdUS02PageDlg.h
** Version      : 1.0
** Description  : 向导对话框
** Modify Record:
***********************************************************************/

#ifndef _WZD_US02_PAGE_DLG_H_
#define _WZD_US02_PAGE_DLG_H_

#include "Dialog.h"
#include "WndStructDef.h"

using namespace GUI;

class CWzdUS02PageDlg : public GUI::CDialog
{
public:
	CWzdUS02PageDlg();
	~CWzdUS02PageDlg();

	//此页面是否使用
	void SetUsed(bool bUsed = true);
	bool IsUsed();

	//在获取数据后设置控件信息
	virtual void InitCtrlInfo();
	//设置向导信息
	virtual void SetWzdInfo(US02_WzdInfo* pWzdInfo);

	//显示向导数据
	virtual void ShowWzdData(bool bRefresh);

	//页面完成读取向导信息
	virtual void GetWzdData(bool bNext = true);

	//离开某一页面时
	virtual void OnLeavePage();
	
	virtual unsigned long CheckValue();
protected:
	US02_WzdInfo* m_pWzdInfo;
	bool m_bUsed;
};

#endif

