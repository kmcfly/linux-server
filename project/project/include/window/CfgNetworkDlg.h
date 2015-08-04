/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : L
** Date         : 2009-08-14
** Name         : CfgNetworkDlg.h
** Version      : 1.0
** Description  : 
** Modify Record:
1:2010-06-10,把该页面中的DDNS配置分割出来，放在CfgNetMoreConfigDlg.cpp页面中 ZHL
***********************************************************************/

#ifndef _CFG_NETWORK_DLG_H_
#define _CFG_NETWORK_DLG_H_

#include "Dialog.h"
#include "ListCtrl.h"
#include "CfgDlg.h"
#if defined (__CUSTOM_IPV6__)
#include "IPv6Ctrl.h"
#endif
using namespace GUI;

class CCfgNetworkDlg : public CCfgDlg
{
public:
	CCfgNetworkDlg();
	~CCfgNetworkDlg();

protected:
	typedef struct _get_ip_mode_
	{
		bool bGetIpModeAuto;
		bool bGetIpModePPPOE;
	}GET_IP_MODE;

	typedef enum _refrash_ui_num
	{
		REFRASH_UI_AUTOIP  =0,
		REFRASH_UI_PPPOE   =1,
		REFRASH_UI_ALL     =2,
		REFRASH_UI_IPV6_AUTOIP =3,

	}REFRASH_UI_NUM;

	typedef enum _parameter_check_status
	{
		CHECK_STATUS_NOCHANGE	=0,					//网络参数没有改变
		CHECK_STATUS_OK			=1,					//测试成功
		CHECK_STATUS_NOCHECK	=2,					//没有进行测试			
		CHECK_STATUS_FAIL		=3,					//测试失败
	}CHECK_STATUS;

public:
	unsigned long PreCheckValue();

protected:
	virtual void OnInitial();
	//在检测IP参数正确性的时候该函数屏蔽外部输入
	unsigned long OnPreCommand(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);

	void SetTip();

protected:
	bool ShowData(NCFG_ITEM_ID cfgID, const BYTE* pData, int len, bool bRefresh);
	bool SetData(NCFG_ITEM_ID cfgID, BYTE*& pData, int& len)  const;

protected:
	void CheckEnableUI( bool bRefresh, REFRASH_UI_NUM uiNum=REFRASH_UI_ALL );
	void DoUpdateUI( GET_IP_MODE& getIpMode, bool bRefresh,REFRASH_UI_NUM uiNum);

	unsigned long GetErrorTextNum();

	unsigned long CheckDhcpParam();
	unsigned long CheckValue();

protected:
	DECLARE_MSG_MAP()
	unsigned long OnClickList(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);
private:
	bool CheckPOEIP();
protected:
	GUI::CListCtrl			m_cfgNetworkList;

	GET_IP_MODE				m_GetIpMode;
	bool					m_bCheckParam;
	CHECK_STATUS			m_StatusIPCheckOK;

	NET_CTRL_FAIL			m_netCheckResult;

	bool					m_bAuto; //是否动态IP

	int						m_poeNum;
#if defined (__CUSTOM_IPV6__)
	enum LIST_ROW
	{
		ROW_HTTP_PORT = 0,
		ROW_SERVER_PORT = 1,

		ROW_IPV4_CHECK = 2, 
		ROW_GETIP_DHCP = 3,
		ROW_STATIC_IP = 4,
		ROW_SUB_NET_MASK = 5,
		ROW_GATEWAY = 6,
		ROW_DNS_1 = 7,
		ROW_DNS_2 = 8,

		ROW_IPV6_CHECK = 9, 
		ROW_IPV6_GETIP_DHCP = 10,
		ROW_IPV6_STATIC_IP = 11,
		ROW_IPV6_SUB_NET_MASK = 12,
		ROW_IPV6_GATEWAY = 13,
		ROW_IPV6_DNS_1 = 14,
		ROW_IPV6_DNS_2 = 15,

		ROW_PPPOE_ENABLE = 17,
		ROW_PPPOE_USER_NAME = 18,
		ROW_PPPOE_PASSWORD = 19,
		ROW_NET_TEST = 20,
		ROW_POESWITCHIP = 21,
		ROW_POESWITCH_SUBNETMASK = 22,

		ROW_NUM,
	};

	enum LIST_COLUMN
	{
		COLUMN_IPV,
		COLUMN_DESCRIBE,
		COLUMN_DATA,
	};
	bool					m_bAutoIPv6;	//是否动态IPv6
	bool                    m_bAutoGetIpV6;//页面是否选取了自动获取ipv6
#else
	enum LIST_ROW
	{
		ROW_HTTP_PORT = 0,
		ROW_SERVER_PORT = 1,

		ROW_GETIP_DHCP = 3,
		ROW_STATIC_IP = 4,
		ROW_SUB_NET_MASK = 5,
		ROW_GATEWAY = 6,
		ROW_DNS_1 = 7,
		ROW_DNS_2 = 8,

		ROW_PPPOE_ENABLE = 9,
		ROW_PPPOE_USER_NAME = 10,
		ROW_PPPOE_PASSWORD = 11,
		ROW_NET_TEST = 12,
		
		ROW_POESWITCHIP = 13,
		ROW_POESWITCH_SUBNETMASK = 14,
		ROW_NUM = 15,
	};
	enum LIST_COLUMN
	{
		COLUMN_DESCRIBE,
		COLUMN_DATA,
	};
#endif
};


#endif

