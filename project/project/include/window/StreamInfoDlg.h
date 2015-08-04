/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : ysw
** Date         : 2009-08-08
** Name         : StreamInfoDlg.h
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/
#ifndef _STREAM_INFO_DLG_H_
#define _STREAM_INFO_DLG_H_
#include "Dialog.h"
#include "ListCtrl.h"

using namespace GUI;

class CStreamInfoDlg : public GUI::CDialog
{
public:
	CStreamInfoDlg();
	~CStreamInfoDlg();
protected:
	//DECLARE_MSG_MAP()
	void OnInitial();
private:
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	GUI::CListCtrl	m_streamInfoList;
};
#endif //_STREAM_INFO_DLG_H_
