/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : ysw
** Date         : 2009-08-07
** Name         : NetworkStatusDlg.h
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/
#ifndef _NETWORK_STATUS_DLG_H_
#define _NETWORK_STATUS_DLG_H_
#include "Dialog.h"
#include "ListCtrl.h"

using namespace GUI;

class CNetworkStatusDlg : public GUI::CDialog
{
public:
	CNetworkStatusDlg();
	~CNetworkStatusDlg();
protected:
	//DECLARE_MSG_MAP()
	void OnInitial();
private:
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	GUI::CListCtrl	m_networkInfoList;
};

#endif//_NETWORK_STATUS_DLG_H_
