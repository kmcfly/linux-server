/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : ysw
** Date         : 2009-08-06
** Name         : OnlineUserDlg.h
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/
#ifndef _ONLINE_USER_DLG_H_
#define _ONLINE_USER_DLG_H_
#include "Button.h"
#include "CheckCtrl.h"
#include "Dialog.h"
#include "ListCtrlEx.h"
#include "UserMan.h"

using namespace GUI;

class COnlineUserDlg : public GUI::CDialog
{
public:
	COnlineUserDlg();
	~COnlineUserDlg();
protected:
	DECLARE_MSG_MAP()
	void OnInitial();
private:
	//////////////////////////////////////////////////////////////////////////
	unsigned long OnRefresh();
	unsigned long OnEvict();
	//////////////////////////////////////////////////////////////////////////
	GUI::CButton	m_btnRefresh;
	GUI::CButton	m_btnEvict;


	GUI::CListCtrlEx <NET_CLIENT_INFO_EX> m_listCtrlEx;
};
#endif //_ONLINE_USER_DLG_H_
