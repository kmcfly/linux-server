/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : lufen
** Date         : 2010-04-19
** Name         : CfgNetWIFIIPInfoDlg.h
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/

#ifndef _CFG_NET_WIFI_IP_INFO_DLG_H_
#define _CFG_NET_WIFI_IP_INFO_DLG_H_

#include "ListCtrl.h"
#include "CfgDlg.h"

using namespace GUI;

class CCfgNetWIFIIpInfoDlg : public CCfgDlg
{
public:
	CCfgNetWIFIIpInfoDlg();
	~CCfgNetWIFIIpInfoDlg();
	void SetwifiIpInfo(WIFI_CONFIG* wifiIpInfo);
	void GetwifiIpInfo(WIFI_CONFIG* wifiIpInfo);

protected:
	void OnInitial();
	void OnPostInitial();

protected:
	unsigned long OnApply();

	DECLARE_MSG_MAP()
	unsigned long OnClickList(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);

protected:
	GUI::CListCtrl	m_cfgListwifiIp;
	WIFI_CONFIG     m_cfgwifi;

	enum LIST_ROW
	{
		ROW_STATIC_IP_MODE =0 ,
		ROW_STATIC_IP = 1,
		ROW_STATIC_SUB_NET_MASK = 2,
		ROW_STATIC_GATEWAY =3,
		ROW_STATIC_DNS_1 = 4,

	};
};




#endif

