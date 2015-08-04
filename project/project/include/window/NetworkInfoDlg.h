/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : ysw
** Date         : 2009-08-06
** Name         : NetworkInfoDlg.h
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/
#ifndef _NETWORK_INFO_DLG_H_
#define _NETWORK_INFO_DLG_H_
#include "Button.h"
#include "TabCtrl.h"
#include "Dialog.h"
#include "NetworkStatusDlg.h"
#include "NetworkStreamInfoDlg.h"

using namespace GUI;

class CNetworkInfoDlg : public GUI::CDialog
{
public:
	CNetworkInfoDlg();
	~CNetworkInfoDlg();
protected:
	//DECLARE_MSG_MAP()
	void OnInitial();
private:
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	GUI::CTabCtrl		m_tabCtrl;

	CNetworkStatusDlg	m_networkStatusDlg;
	CNetworkStreamInfoDlg m_networkStreamInfoDlg;
};
#endif //_NETWORK_INFO_DLG_H_
