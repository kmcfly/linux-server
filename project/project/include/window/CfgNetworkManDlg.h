/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : L
** Date         : 2009-08-14
** Name         : CfgNetworkManDlg.h
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/

#ifndef _CFG_NETWORK_MAN_DLG_H_
#define _CFG_NETWORK_MAN_DLG_H_

#include "Dialog.h"
#include "TabCtrl.h"
#include "Anchor.h"

#include "CfgDlg.h"
#include "CfgNetworkDlg.h"
#include "CfgNetMoreConfigDlg.h"
//#include "CfgNetworkVideoDlg.h"
#include "CfgNetworkStreamDlg.h"
#ifdef __DVR_BASIC__	
#include "CfgNetEmailDlg.h"
#endif

#ifdef __ALARM_TO_UPDATE_FTP_VIDEO__
#include "CfgNetworkFtpDlg.h"
#endif


#ifdef __MOBILE_DVR__
#include "CfgNetWirelessDlg.h"
#include "CfgNetServerDlg.h"
#include "CfgNetWifiDlg.h"
#endif

#ifdef __SUPPORT_HRSST__
#include "CfgNetPhoneDlg.h"
#endif

#include "CfgNetAutoReportDlg.h"

#include "CfgNetworkExDlg.h"

#include "CfgNetworkNatDlg.h"
#include "CfgNetworkPushDlg.h"

using namespace GUI;

class CCfgNetworkManDlg : public CCfgDlg
{
public:
	CCfgNetworkManDlg();
	~CCfgNetworkManDlg();

#ifdef __SUPPORT_3G__
	void Set3gStatus(bool bStatus){ m_3gLastStatus = bStatus;};
	bool GetLast3gStatus(){ return m_3gLastStatus;};
#endif 
public:
	enum PAGE_ITEM
	{
		PAGE_NETWORK,
		//PAGE_NETWORK_VIDEO,
		PAGE_NETWORK_STREAM,
#ifdef __DVR_BASIC__	
		PAGE_NETWORK_EMAIL,
#endif

#ifdef __ALARM_TO_UPDATE_FTP_VIDEO__
		PAGE_NETWORK_FTP,
#endif

#ifdef __MOBILE_DVR__
		PAGE_NETWORK_WIFI,
		PAGE_NETWORK_WIRELESS,
		PAGE_NETWORK_SERVER,
#endif
#ifdef __SUPPORT_HRSST__
		PAGE_NETWORK_PHONE,
#endif

#if !defined(__CUSTOM_BR27__) && !defined(__KR24_IN__)
		PAGE_NETWORK_AUTOREPORT,		
#endif

#if defined(__TVT_NAT_SOCK__)
		PAGE_NETWORK_NAT,
#endif

#if defined(__TVT_PUSH_SOCK__)
		PAGE_NETWORK_PUSH,
#endif

		PAGE_NETWORK_MORECONFIG,

		PAGE_COUNT,
	};

	void SetAnchorPage(PAGE_ITEM item);

protected:
	virtual void OnInitial();			//初始化

	void OnPostInitial();
	unsigned long OnApply();

	unsigned long CheckValue();

	//unsigned long OnOK();
	void GetItemList(CFGITEM_LIST& itemList) const;
	CCfgDlg* GetPage(PAGE_ITEM item);
protected:
	unsigned long OnPreCommand(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);

protected:
	DECLARE_MSG_MAP()
	unsigned long OnClickNetwork();
	unsigned long OnClickNetMoreConfig();
	unsigned long OnClickNetworkNat();
	unsigned long OnClickNetworkPush();
	//unsigned long OnClickNetworkVideo();
	unsigned long OnClickNetworkStream();
#ifdef __DVR_BASIC__	
	unsigned long OnClickNetEmail();
#endif

#ifdef __ALARM_TO_UPDATE_FTP_VIDEO__
	unsigned long OnClickNetFtp();
#endif

#ifdef __SUPPORT_HRSST__
	unsigned long OnClickNetPhone();
#endif


	unsigned long OnClickNetAutoReport();


	unsigned long PostSave();

protected:
	GUI::CTabCtrl				m_tabCtrl;
	CCfgNetworkExDlg            m_cfgNetworkExDlg;

	CCfgNetworkDlg				m_cfgNetworkDlg;

	CCfgNetMoreConfigDlg		m_cfgNetMoreConfigDlg;
	//CCfgNetworkVideoDlg		m_cfgNetworkVideoDlg;
	CCfgNetworkStreamDlg		m_cfgNetworkStreamDlg;

#ifdef __DVR_BASIC__	
	CCfgNetEmailDlg				m_cfgNetEmailDlg;

	
#endif

#ifdef __ALARM_TO_UPDATE_FTP_VIDEO__
	CCfgNetworkFtpDlg			m_cfgNetFtpDlg;
#endif

#ifdef __SUPPORT_HRSST__
	CCfgNetPhoneDlg             m_cfgNetPhoneDlg;
#endif

	PAGE_ITEM					m_curPage;
	unsigned long				m_initPageMask;			//记录已经初始化的页面
	PAGE_ITEM					m_szPageItem[PAGE_COUNT];

#ifdef __MOBILE_DVR__
	CCfgNetWirelessDlg		m_cfgNetWirelessDlg;
	CCfgNetServerDlg		m_cfgNetServerDlg;
	CCfgNetWIFIDlg			m_cfgNetWifiDlg;
	unsigned long			OnClickNetWireless();
	unsigned long			OnClickNetServer();
	unsigned long			OnClickNetWifi();
#endif

	CCfgNetAutoReportDlg	m_cfgNetAutoReportDlg;

	CCfgNetworkNatDlg		m_cfgNetworkNatDlg;
	CCfgNetworkPushDlg		m_cfgNetworkPushDlg;

	//
	PAGE_ITEM m_initAnchorPage;

	//
	#ifdef __SUPPORT_3G__
	bool	m_3gLastStatus;		//上次3g是否启用
	#endif
};


//////////////////////////////////////////////////////////////////////////
// __DVR_ANCHOR__
//////////////////////////////////////////////////////////////////////////

class CCfgNetworkManAnchorDlg : public GUI::CDialog
{
public:
	CCfgNetworkManAnchorDlg();
	~CCfgNetworkManAnchorDlg();

protected:
	void OnInitial();

protected:
	DECLARE_MSG_MAP()

	unsigned long OnAnchorNetwork();
	unsigned long OnAnchorNetworkStream();
	unsigned long OnAnchorNetEmail();
	unsigned long OnAnchorNetFtp();
	unsigned long OnAnchorNetWifi();
	unsigned long OnAnchorNetWireless();
	unsigned long OnAnchorNetServer();
	unsigned long OnAnchorNetPhone();
	unsigned long OnAnchorNetMoreConfig();
	unsigned long OnAnchorNetworkNat();
	unsigned long OnAnchorNetworkPush();
	

	unsigned long OnAnchorNetAutoReport();
	unsigned long OnExit();

protected:
	GUI::CAnchor		m_anNetwork;
	GUI::CAnchor		m_anNetworkStream;
	GUI::CAnchor		m_anNetEmail;
	GUI::CAnchor		m_anNetFtp;
	GUI::CAnchor		m_anNetWifi;
	GUI::CAnchor		m_anNetWireless;
	GUI::CAnchor		m_anNetServer;
	GUI::CAnchor        m_anNetPhone;
	GUI::CAnchor		m_anNetworkNat;
	GUI::CAnchor		m_anNetworkPush;
	GUI::CAnchor		m_anNetMoreConfig;
	GUI::CAnchor		m_anNetAutoReport;
	GUI::CAnchor		m_anNetMdvrWireless;
	GUI::CAnchor		m_anExit;

	CCfgNetworkManDlg	m_cfgNetworkManDlg;
};

#endif

//end
