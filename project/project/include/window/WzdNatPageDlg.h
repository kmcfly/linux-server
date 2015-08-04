/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : ZXX
** Date         : 2013-11-25
** Name         : WzdNatPageDlg.h
** Version      : 1.0
** Description  : 穿透向导继承对话框
** Modify Record:
***********************************************************************/

#ifndef _WZD_NAT_PAGE_DLG_H_
#define _WZD_NAT_PAGE_DLG_H_

#include "Dialog.h"
#include "WndStructDef.h"

using namespace GUI;

typedef enum _DEV_SELECT_
{
	DEV_ANDROID = 0,
	DEV_APPLE	= 1,
	DEV_PC		= 2,
	DEV_MOBILE  = 3,
	NEXT_PC		= 4,
	NEXT_MOBILE = 5,
}DEV_SELECT_INDEX;

class CWzdNatPageDlg : public GUI::CDialog
{
public:
	CWzdNatPageDlg();
	~CWzdNatPageDlg();

	//此页面是否使用
	void SetUsed(bool bUsed = true);
	bool IsUsed();

	//在获取数据后设置控件信息
	virtual void InitCtrlInfo();
	//设置向导信息
	virtual void SetWzdInfo(NAT_WIZARD_INFO* pWzdInfo);

	//显示向导数据
	virtual void ShowWzdData(bool bRefresh);

	//页面完成读取向导信息
	virtual void GetWzdData(bool bNext = true);

	//离开某一页面时
	virtual void OnLeavePage();

	virtual unsigned long CheckValue();
	
protected:
	NAT_WIZARD_INFO* m_pWzdInfo;
	bool m_bUsed;
};

#endif
//end
