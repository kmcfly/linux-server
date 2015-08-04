/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : ysw
** Date         : 2009-08-06
** Name         : SystemInfoDlg.h
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/
#ifndef _SYSTEM_INFO_DLG_H_
#define _SYSTEM_INFO_DLG_H_
#include "Dialog.h"
#include "TabCtrl.h"
#include "VersionInfoDlg.h"
#include "StreamInfoDlg.h"

using namespace GUI;

class CSystemInfoDlg : public GUI::CDialog
{
public:
	CSystemInfoDlg();
	~CSystemInfoDlg();
protected:
	//DECLARE_MSG_MAP()
	void OnInitial();
private:
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	GUI::CTabCtrl	m_tabCtrl;

	CVersionInfoDlg	m_versionDlg;
	CStreamInfoDlg	m_streamDlg;
};
#endif //_SYSTEM_INFO_DLG_H_
