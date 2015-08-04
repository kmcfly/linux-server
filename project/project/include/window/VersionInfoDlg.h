/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : ysw
** Date         : 2009-08-08
** Name         : VersionInfoDlg.h
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/
#ifndef _VERSION_INFO_DLG_H_
#define _VERSION_INFO_DLG_H_
#include "Dialog.h"
#include "ListCtrl.h"

using namespace GUI;

class CVersionInfoDlg : public GUI::CDialog
{
public:
	CVersionInfoDlg();
	~CVersionInfoDlg();
protected:
	//DECLARE_MSG_MAP()
	void OnInitial();
private:
	enum COL_INDEX
	{
		COL_DEVICE_NAME,
#if defined(__CUSTOM_CNB__)
		COL_DEVICE_TYPE,
#endif
		COL_DEVICE_ID,
		COL_VIDEO_FORMAT,
		COL_HARDWARE_VERSION,
		COL_MCU_VERSION,
		COL_KERNEL_VERSION,
		COL_FIRMWARE_VERSION,
		COL_LAUNCH_DATE,
	};
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	GUI::CListCtrl	m_versionInfoList;
};
#endif //_VERSION_INFO_DLG_H_
