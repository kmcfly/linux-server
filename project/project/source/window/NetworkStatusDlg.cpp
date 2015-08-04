/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : ysw
** Date         : 2009-08-07
** Name         : NetworkStatusDlg.cpp
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/
#include "NetworkStatusDlg.h"
#include "MessageMan.h"
#include "NetInterface.h"
#include "WirelessManage.h"
//////////////////////////////////////////////////////////////////////////
CNetworkStatusDlg::CNetworkStatusDlg()
{

}

CNetworkStatusDlg::~CNetworkStatusDlg()
{

}

//////////////////////////////////////////////////////////////////////////
void CNetworkStatusDlg::OnInitial()
{
	NETWORK_STATUS_INFO networkStatusInfo;
	memset( &networkStatusInfo, 0, sizeof(NETWORK_STATUS_INFO));

	GUI::CDialog::OnInitial();

#ifdef __ENVIRONMENT_LINUX__
	MESSAGE_DATA msgData;
	CMessageMan::Instance()->SendMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_NETWORK_INFO, NULL, 0, msgData);

	networkStatusInfo = *reinterpret_cast<NETWORK_STATUS_INFO *>(msgData.pData+sizeof(PACKCMD));

	CMessageMan::Instance()->ReleaseMsgBuff(msgData);
#endif
	//////////////////////////////////////////////////////////////////////////
	unsigned short x = m_length[DLG_OFFSET_SUB_LEFT], y = m_length[DLG_OFFSET_SUB_TOP];
	unsigned short width = m_rect.Width() - 2 * m_length[DLG_OFFSET_SUB_LEFT];
	unsigned short height = m_rect.Height() - y - m_length[DLG_OFFSET_SUB_BOTTOM];

	m_networkInfoList.Create(GUI::CWnd::GetRandWndID(), x, y, width, height, this, 0, 1, false);
	m_networkInfoList.AddColumn(NULL, width/2);
	m_networkInfoList.AddColumn(NULL, width/2);

	//////////////////////////////////////////////////////////////////////////
	char str[512] = {0};
	m_networkInfoList.AddItem(0, 0, m_pStrTable->String(E_STR_ID_HTTP_PORT));
	sprintf(str,"%d",networkStatusInfo.httpPort);
	m_networkInfoList.AddItem(0, 1, str);

	m_networkInfoList.AddItem(1, 0, m_pStrTable->String(E_STR_ID_SERVER_PORT));
	sprintf(str, "%d", networkStatusInfo.serverPort);
	m_networkInfoList.AddItem(1, 1, str);

	m_networkInfoList.AddItem(2, 0, m_pStrTable->String(E_STR_ID_IP_ADDRESS));
	sprintf(str, "%03d.%03d.%03d.%03d", (networkStatusInfo.IP&0xff), (networkStatusInfo.IP>>8)&0xff, (networkStatusInfo.IP>>16)&0xff, (networkStatusInfo.IP>>24)&0xff);
	m_networkInfoList.AddItem(2, 1, str);

	m_networkInfoList.AddItem(3, 0, m_pStrTable->String(E_STR_ID_NET_MASK));
	sprintf(str, "%03d.%03d.%03d.%03d", (networkStatusInfo.subMask&0xff), (networkStatusInfo.subMask>>8)&0xff, (networkStatusInfo.subMask>>16)&0xff, (networkStatusInfo.subMask>>24)&0xff);
	m_networkInfoList.AddItem(3, 1, str);

	m_networkInfoList.AddItem(4, 0, m_pStrTable->String(E_STR_ID_GATEWAY));
	sprintf(str, "%03d.%03d.%03d.%03d", (networkStatusInfo.gateway&0xff), (networkStatusInfo.gateway>>8)&0xff, (networkStatusInfo.gateway>>16)&0xff, (networkStatusInfo.gateway>>24)&0xff);
	m_networkInfoList.AddItem(4, 1, str);

	m_networkInfoList.AddItem(5, 0, m_pStrTable->String(E_STR_ID_PREFERRED_DNS_SERVER));
	sprintf(str, "%03d.%03d.%03d.%03d", (networkStatusInfo.DNS1&0xff), (networkStatusInfo.DNS1>>8)&0xff, (networkStatusInfo.DNS1>>16)&0xff, (networkStatusInfo.DNS1>>24)&0xff);
	m_networkInfoList.AddItem(5, 1, str);

	m_networkInfoList.AddItem(6, 0, m_pStrTable->String(E_STR_ID_ALTERNATE_DNS_SERVER));
	sprintf(str, "%03d.%03d.%03d.%03d", (networkStatusInfo.DNS2&0xff), (networkStatusInfo.DNS2>>8)&0xff, (networkStatusInfo.DNS2>>16)&0xff, (networkStatusInfo.DNS2>>24)&0xff);
	m_networkInfoList.AddItem(6, 1, str);

	//连网方式
	m_networkInfoList.AddItem(7, 0, m_pStrTable->String(E_STR_ID_NETWORKAPPROACH));
	strcpy(str, m_pStrTable->String(E_STR_ID_STATIC_IP));
	if( networkStatusInfo.bDHCP )
	{
		strcpy(str, m_pStrTable->String(E_STR_ID_DHCP));
	}

	if( networkStatusInfo.bPPPoE )
	{
		strcpy(str, m_pStrTable->String(E_STR_ID_PPPOE));
	}
	m_networkInfoList.AddItem(7, 1, str);

	//当前网络状态
	m_networkInfoList.AddItem(8, 0, m_pStrTable->String(E_STR_ID_STATUS));
	if( 0 == networkStatusInfo.netstatus )
	{
		m_networkInfoList.AddItem(8, 1, m_pStrTable->String(E_STR_ID_UNCONNECTED));
	}
	else
	{
		m_networkInfoList.AddItem(8, 1, m_pStrTable->String(E_STR_ID_CONNECTED));
	}

	//DDNS的连接状态
	m_networkInfoList.AddItem(9, 0, m_pStrTable->String(E_STR_ID_DDNS));
	if (networkStatusInfo.bDDNS)
	{
		m_networkInfoList.AddItem(9, 1, m_pStrTable->String(E_STR_ID_CONNECTED));
	}
	else
	{
		m_networkInfoList.AddItem(9, 1, m_pStrTable->String(E_STR_ID_UNCONNECTED));
	}

	//设备的物理地址
	m_networkInfoList.AddItem(10, 0, m_pStrTable->String(E_STR_ID_MAC));
	sprintf(str, "%02X-%02X-%02X-%02X-%02X-%02X", networkStatusInfo.MAC[0], networkStatusInfo.MAC[1],networkStatusInfo.MAC[2],networkStatusInfo.MAC[3],networkStatusInfo.MAC[4],networkStatusInfo.MAC[5]);
	m_networkInfoList.AddItem(10, 1, str);
	
#ifdef __SUPPORT_3G__
	{
#ifdef __ENVIRONMENT_LINUX__
		m_networkInfoList.AddItem(12, 0,  m_pStrTable->String(E_STR_ID_NETWORK_WIRELESS));
		if(CWirelessManage::Instance()->GetWirelessState())
		{
			m_networkInfoList.AddItem(12, 1, m_pStrTable->String(E_STR_ID_CONNECTED) );
		}
		else
		{
			if (CWirelessManage::Instance()->Get3GEnabled())
			{
				m_networkInfoList.AddItem(12, 1, m_pStrTable->String(E_STR_ID_UNCONNECTED));
			}
			else
			{
				m_networkInfoList.AddItem(12, 1, m_pStrTable->String(E_STR_ID_UNCONNECTED));
			}
		}
		m_networkInfoList.AddItem(13, 0, m_pStrTable->String(E_STR_ID_WIRELESS_IP));
		memset(str,0, 512);
		CNetInterface::Instance()->GetIPText(str, 512, "ppp0", 4);
		m_networkInfoList.AddItem(13, 1, str);
#else
		m_networkInfoList.AddItem(12, 0,  m_pStrTable->String(E_STR_ID_NETWORK_WIRELESS));
		m_networkInfoList.AddItem(12, 1, m_pStrTable->String(E_STR_ID_UNCONNECTED));
		m_networkInfoList.AddItem(13, 0, m_pStrTable->String(E_STR_ID_WIRELESS_IP));
		memset(str,0, 512);		
		m_networkInfoList.AddItem(13, 1, str);
#endif
	}
#endif
}
