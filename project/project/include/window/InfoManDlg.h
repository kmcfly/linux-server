/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : ysw
** Date         : 2009-08-06
** Name         : InfoManDlg.h
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/
#ifndef _INFO_MAN_DLG_H_
#define _INFO_MAN_DLG_H_
#include "Static.h"
#include "Button.h"
#include "Dialog.h"
#include "Anchor.h"

#include "SystemInfoDlg.h"
#include "EventInfoDlg.h"
#include "LogInfoDlg.h"
#include "NetworkInfoDlg.h"
#include "OnlineUserDlg.h"
#include "InfoDeviceDlg.h"

#ifdef __SUPPORT_SHOW_QRCODE__
#include "CPrintQRCodeDlg.h"
#endif

#ifdef __SUPPORT_APP_INFO_QRCODE__
#include "AppInfoQrCodeDlg.h"
#endif
using namespace GUI;
const int verNum = 3;//3.0µÄ°æ±¾ºÅ
class CInfoManDlg : public GUI::CDialog
{
public:
	CInfoManDlg();
	~CInfoManDlg();
protected:
	DECLARE_MSG_MAP()
	void OnInitial();
private:
	//////////////////////////////////////////////////////////////////////////
	unsigned long OnSystem();
	unsigned long OnEvent();
	unsigned long OnLog();
	unsigned long OnNetwork();
	unsigned long OnOnlineUser();
	unsigned long OnDevice();
	unsigned long OnExit();
	unsigned long OnShowQrCode();
	unsigned long OnShowAppInfoQrCode();
	//////////////////////////////////////////////////////////////////////////

#ifndef __DVR_ANCHOR__
	GUI::CStatic m_stcSystem;
	GUI::CStatic m_stcEvent;
	GUI::CStatic m_stcLog;
	GUI::CStatic m_stcNetwork;
	GUI::CStatic m_stcOnlineUser;
	GUI::CStatic m_stcDevice;
	GUI::CStatic m_stcExit;
	GUI::CStatic m_stcqrCode;
	GUI::CStatic m_stcsnCode;
	GUI::CStatic m_stcAppInfoQrCode;

	GUI::CButton m_btnSystem;
	GUI::CButton m_btnEvent;
	GUI::CButton m_btnLog;
	GUI::CButton m_btnNetwork;
	GUI::CButton m_btnOnlineUser;
	GUI::CButton m_btnDevice;
	GUI::CButton m_btnExit;
	GUI::CButton m_btnqrCode;
	GUI::CButton m_btnsnCode;
	GUI::CButton m_btnAppInfo;
#else
	GUI::CAnchor		m_btnSystem;
	GUI::CAnchor		m_btnEvent;
	GUI::CAnchor		m_btnLog;
	GUI::CAnchor		m_btnNetwork;
	GUI::CAnchor		m_btnOnlineUser;
	GUI::CAnchor		m_btnDevice;
	GUI::CAnchor		m_btnExit;
	GUI::CAnchor		m_btnqrCode;
	GUI::CAnchor		m_btnsnCode;
	GUI::CAnchor		m_btnAppInfo;
#endif

	//////////////////////////////////////////////////////////////////////////
	CSystemInfoDlg	m_systemDlg;
	CEventInfoDlg	m_eventDlg;
	CLogInfoDlg		m_logDlg;
	CNetworkInfoDlg	m_networkDlg;
	COnlineUserDlg	m_onlineUserDlg;
	CInfoDeviceDlg	m_infoDeviceDlg;
#ifdef __SUPPORT_SHOW_QRCODE__
		CPrintQRCodeDlg	m_printQRCodeDlg;
#endif

#ifdef __SUPPORT_APP_INFO_QRCODE__
	CAppInfoQrCodeDlg *m_appInfoQrCodeDlg;
#endif


};
#endif //_INFO_MAN_DLG_H_
