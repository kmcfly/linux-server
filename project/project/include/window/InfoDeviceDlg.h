/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : ZYX
** Date         : 2011-06-23
** Name         : InfoDeviceDlg.h
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/
#ifndef _INFO_DEVICE_DLG_H_
#define _INFO_DEVICE_DLG_H_
#include "Dialog.h"
#include "ListCtrl.h"
#include "Static.h"
#include "RadioCtrl.h"
#include "ProgressCtrl.h"

using namespace GUI;

class CInfoDeviceDlg : public GUI::CDialog
{
public:
	CInfoDeviceDlg();
	~CInfoDeviceDlg();
protected:
	//DECLARE_MSG_MAP()
	void OnInitial();
private:
	//////////////////////////////////////////////////////////////////////////
	GUI::CListCtrl	m_deviceInfoList;
	GUI::CListCtrl	m_ChanelRecList;
	GUI::CListCtrl	m_DiskList;


	GUI::CStatic	m_stcDisk;
	GUI::CStatic	m_stcChannelRec;
	//////////////////////////////////////////////////////////////////////////
};
#endif //_INFO_DEVICE_DLG_H_
