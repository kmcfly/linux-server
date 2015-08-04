/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : ysw
** Date         : 2009-08-06
** Name         : NetworkStreamInfoDlg.h
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/
#ifndef _NETWORK_STREAM_INFO_DLG_H_
#define _NETWORK_STREAM_INFO_DLG_H_
#include "Dialog.h"
#include "ListCtrl.h"

using namespace GUI;

class CNetworkStreamInfoDlg : public GUI::CDialog
{
public:
	CNetworkStreamInfoDlg();
	~CNetworkStreamInfoDlg();
protected:
	//DECLARE_MSG_MAP()
	void OnInitial();
private:
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	GUI::CListCtrl	m_streamInfoList;
};
#endif //_NETWORK_STREAM_INFO_DLG_H_
