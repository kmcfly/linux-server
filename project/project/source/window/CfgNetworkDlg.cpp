/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : L
** Date         : 2009-08-14
** Name         : CfgNetworkDlg.cpp
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/

#include "CfgNetworkDlg.h"

CCfgNetworkDlg::CCfgNetworkDlg()
{
	m_GetIpMode.bGetIpModeAuto = true;
	m_GetIpMode.bGetIpModePPPOE = true;
#if defined (__CUSTOM_IPV6__)
		m_bAutoGetIpV6 = false;//
#endif
		m_poeNum = 0;
}

CCfgNetworkDlg::~CCfgNetworkDlg()
{

}


void CCfgNetworkDlg::OnInitial()
{
	CCfgDlg::OnInitial();
	//////////////////////////////////////////////////////////////////////////

	GUI::CListCtrl& list = m_cfgNetworkList;
	GUI::CWnd * pWnd = NULL;
	GUI::CCheckCtrl* pCheck = NULL;
	GUI::CEditCtrl*	 pEdit = NULL;
	GUI::CNumEditCtrl* pNumEdit = NULL;
	GUI::CIPAddressCtrl* pIP = NULL;
	GUI::CButton* pBtn = NULL;

#if defined (__CUSTOM_IPV6__)
	GUI::CIPV6Ctrl *pIPv6 = NULL;
#endif

	unsigned short x = m_length[DLG_OFFSET_SUB_LEFT], y = m_length[DLG_OFFSET_SUB_TOP];
	unsigned short width = m_rect.Width() - 2 * m_length[DLG_OFFSET_SUB_LEFT];
	unsigned short height = m_rect.Height() - y - m_length[DLG_OFFSET_SUB_BOTTOM];

	unsigned short offsetEdge = 0;
	int maxListRow = height/m_length[LIST_ITEM_HEIGHT]; //List框不带头
	if( ROW_NUM > maxListRow )
	{
		offsetEdge = m_length[SLIDER_WIDTH] + 2;
	}

	list.Create(GUI::CWnd::GetRandWndID(), x, y, width, height, this, 0, 0, false);
#if defined (__CUSTOM_IPV6__)
	list.AddColumn(NULL, width * 3 / 32);
	list.AddColumn(NULL, width * 13 / 32);
	list.AddColumn(NULL, width * 16 / 32 - offsetEdge);
#else
	list.AddColumn(NULL, width * 16 / 32);
	list.AddColumn(NULL, width * 16 / 32 - offsetEdge);
#endif
	////
	char str[128] = {0};
	int row = 0;
#if defined (__CUSTOM_IPV6__)
	// row = 0
	list.AddItem( ROW_HTTP_PORT, COLUMN_DESCRIBE, m_pStrTable->String( E_STR_ID_HTTP_PORT ) );
	pNumEdit = dynamic_cast<GUI::CNumEditCtrl*>( list.AddItem( ROW_HTTP_PORT, COLUMN_DATA, WND_TYPE_NUM_EDIT) );
	assert( pNumEdit );
	pNumEdit->SetMaxStringLen( 5 );
	pNumEdit->SetRange(0,65535);

	// row = 1
	row += 1;
	list.AddItem( ROW_SERVER_PORT, COLUMN_DESCRIBE, m_pStrTable->String(E_STR_ID_SERVER_PORT) );
	pNumEdit = dynamic_cast<GUI::CNumEditCtrl*>( list.AddItem( ROW_SERVER_PORT, COLUMN_DATA, WND_TYPE_NUM_EDIT) );
	assert( pNumEdit );
	pNumEdit->SetMaxStringLen( 5 );
	pNumEdit->SetRange(0,(int)65535);

	//IPV4
	// row = 2
	row += 1;
	list.AddItem(ROW_IPV4_CHECK, COLUMN_IPV, "IPV4");
	//目前先不显示ipv4和ipv6勾选框
	//pCheck = dynamic_cast<GUI::CCheckCtrl*>(list.AddItem(ROW_IPV4_CHECK, COLUMN_DATA, WND_TYPE_CHECK));
	//assert(pCheck);

	// row = 3
	row += 1;
	list.AddItem( ROW_GETIP_DHCP, COLUMN_DESCRIBE, m_pStrTable->String(E_STR_ID_IP_AUTO) );
	pCheck = dynamic_cast<GUI::CCheckCtrl*>( list.AddItem( ROW_GETIP_DHCP, COLUMN_DATA, WND_TYPE_CHECK ) );
	assert( pCheck );
	//pCheck->SetCheck( true, false );


	// row = 4
	row += 1;
	list.AddItem( ROW_STATIC_IP, COLUMN_DESCRIBE, m_pStrTable->String(E_STR_ID_IP_ADDRESS) );
	pIP = dynamic_cast<GUI::CIPAddressCtrl*>( list.AddItem( ROW_STATIC_IP, COLUMN_DATA, WND_TYPE_IP) );
	assert( pIP );

	// row = 5
	row += 1;
	list.AddItem( ROW_SUB_NET_MASK, COLUMN_DESCRIBE, m_pStrTable->String( E_STR_ID_NET_MASK ) );
	pIP = dynamic_cast<GUI::CIPAddressCtrl*>( list.AddItem( ROW_SUB_NET_MASK, COLUMN_DATA, WND_TYPE_IP) );
	assert( pIP );

	// row = 6
	row += 1;
	list.AddItem( ROW_GATEWAY, COLUMN_DESCRIBE, m_pStrTable->String( E_STR_ID_GATEWAY ) );
	pIP = dynamic_cast<GUI::CIPAddressCtrl*>( list.AddItem( ROW_GATEWAY, COLUMN_DATA, WND_TYPE_IP) );
	assert( pIP );

	// row = 7
	row += 1;
	list.AddItem( ROW_DNS_1, COLUMN_DESCRIBE, m_pStrTable->String( E_STR_ID_PREFERRED_DNS_SERVER ) );
	pIP = dynamic_cast<GUI::CIPAddressCtrl*>( list.AddItem( ROW_DNS_1, COLUMN_DATA, WND_TYPE_IP) );
	assert( pIP );

	// row = 8
	row += 1;
	list.AddItem( ROW_DNS_2, COLUMN_DESCRIBE, m_pStrTable->String( E_STR_ID_ALTERNATE_DNS_SERVER ) );
	pIP = dynamic_cast<GUI::CIPAddressCtrl*>( list.AddItem( ROW_DNS_2, COLUMN_DATA, WND_TYPE_IP) );
	assert( pIP );

	//IPV6
	//row = 9
	row += 1;
	list.AddItem(ROW_IPV6_CHECK, COLUMN_IPV, "IPV6");
	//目前先不显示ipv4和ipv6勾选框
	//pCheck = dynamic_cast<GUI::CCheckCtrl*>(list.AddItem(ROW_IPV6_CHECK, COLUMN_DATA, WND_TYPE_CHECK));
	//assert(pCheck);

	// row = 10
	row += 1;
	//ipv6自动获得ip先隐藏起来
	//list.AddItem( ROW_IPV6_GETIP_DHCP, COLUMN_DESCRIBE, m_pStrTable->String(E_STR_ID_IP_AUTO) );
	//pCheck = dynamic_cast<GUI::CCheckCtrl*>( list.AddItem( ROW_IPV6_GETIP_DHCP, COLUMN_DATA, WND_TYPE_CHECK ) );
	//assert( pCheck );

	// row = 11
	unsigned long cx = width/2 - m_length[LIST_ITEM_XPOS]-m_length[BTN_WIDTH]/2;
	
	row += 1;
	list.AddItem( ROW_IPV6_STATIC_IP, COLUMN_DESCRIBE, m_pStrTable->String(E_STR_ID_IP_ADDRESS) );
	
	if(m_width <= 800)
	{
		
		pEdit = dynamic_cast<GUI::CEditCtrl*>( list.AddItem( ROW_IPV6_STATIC_IP, COLUMN_DATA, WND_TYPE_EDIT) );
		assert( pEdit );
	}
	else
	{
		pIPv6 = dynamic_cast<GUI::CIPV6Ctrl*>( list.AddItem( ROW_IPV6_STATIC_IP, COLUMN_DATA, WND_TYPE_IPV6) );
		assert( pIPv6 );
	}
	

	// row = 12
	row += 1;
	list.AddItem( ROW_IPV6_SUB_NET_MASK, COLUMN_DESCRIBE, m_pStrTable->String( E_STR_ID_NET_MASK ) );

	pNumEdit  = dynamic_cast<GUI::CNumEditCtrl*>(list.AddItem( ROW_IPV6_SUB_NET_MASK, COLUMN_DATA, WND_TYPE_NUM_EDIT,cx) );
	assert( pNumEdit  );
	pNumEdit->SetMaxStringLen( 5 );
	pNumEdit->SetRange(0,65535);

	// row = 13
	row += 1;
	list.AddItem( ROW_IPV6_GATEWAY, COLUMN_DESCRIBE, m_pStrTable->String( E_STR_ID_GATEWAY ) );
	if(m_width <= 800)
	{
		pEdit = dynamic_cast<GUI::CEditCtrl*>( list.AddItem( ROW_IPV6_GATEWAY, COLUMN_DATA, WND_TYPE_EDIT) );
		assert( pEdit );
	}
	else
	{
		pIPv6 = dynamic_cast<GUI::CIPV6Ctrl*>( list.AddItem( ROW_IPV6_GATEWAY, COLUMN_DATA, WND_TYPE_IPV6) );
		assert( pIP );
	}
	

	// row = 14
	row += 1;
	//list.AddItem( ROW_IPV6_DNS_1, COLUMN_DESCRIBE, m_pStrTable->String( E_STR_ID_PREFERRED_DNS_SERVER ) );
	//临时隐藏起来
	//pIPv6 = dynamic_cast<GUI::CIPV6Ctrl*>( list.AddItem( ROW_IPV6_DNS_1, COLUMN_DATA, WND_TYPE_IPV6) );
	//assert( pIP );

	// row = 15
	row += 1;
	//list.AddItem( ROW_IPV6_DNS_2, COLUMN_DESCRIBE, m_pStrTable->String( E_STR_ID_ALTERNATE_DNS_SERVER ) );
	//临时隐藏起来
	//pIPv6 = dynamic_cast<GUI::CIPV6Ctrl*>( list.AddItem( ROW_IPV6_DNS_2, COLUMN_DATA, WND_TYPE_IPV6) );
	//assert( pIP );

	
	
	// row = 17
	row += 2;
	list.AddItem( ROW_PPPOE_ENABLE, COLUMN_DESCRIBE, m_pStrTable->String( E_STR_ID_PPPOE ) );
	pCheck = dynamic_cast<GUI::CCheckCtrl*>( list.AddItem( ROW_PPPOE_ENABLE, COLUMN_DATA, WND_TYPE_CHECK ) );
	assert( pCheck );

	// row = 18
	row += 1;
	list.AddItem( ROW_PPPOE_USER_NAME, COLUMN_DESCRIBE, m_pStrTable->String( E_STR_ID_USER_NAME ) );
	pEdit = dynamic_cast<GUI::CEditCtrl*>( list.AddItem( ROW_PPPOE_USER_NAME, COLUMN_DATA, WND_TYPE_EDIT) );
	assert( pEdit );

	// row = 19
	row += 1;
	list.AddItem( ROW_PPPOE_PASSWORD, COLUMN_DESCRIBE, m_pStrTable->String( E_STR_ID_PASSWORD ) );
	pEdit = dynamic_cast<GUI::CEditCtrl*>( list.AddItem( ROW_PPPOE_PASSWORD, COLUMN_DATA, WND_TYPE_EDIT) );
	assert( pEdit );
	pEdit->SetPassword( true );

	// row = 20
	row += 1;
	pBtn = dynamic_cast<GUI::CButton*>(list.AddItem(ROW_NET_TEST, COLUMN_DATA,WND_TYPE_BUTTON,m_length[BTN_WIDTH],m_length[BTN_HEIGHT]));
	assert(pBtn);
	pBtn->SetCaption(m_pStrTable->String(E_STR_ID_TEST), false);
#else
	// row = 0
	list.AddItem( ROW_HTTP_PORT, 0, m_pStrTable->String( E_STR_ID_HTTP_PORT ) );
	pNumEdit = dynamic_cast<GUI::CNumEditCtrl*>( list.AddItem( ROW_HTTP_PORT, 1, WND_TYPE_NUM_EDIT) );
	assert( pNumEdit );
	pNumEdit->SetMaxStringLen( 5 );
	pNumEdit->SetRange(0,65535);

	// row = 1
	row += 1;
	list.AddItem( ROW_SERVER_PORT, 0, m_pStrTable->String(E_STR_ID_SERVER_PORT) );
	pNumEdit = dynamic_cast<GUI::CNumEditCtrl*>( list.AddItem( ROW_SERVER_PORT, 1, WND_TYPE_NUM_EDIT) );
	assert( pNumEdit );
	pNumEdit->SetMaxStringLen( 5 );
	pNumEdit->SetRange(0, (int)65535);

	// row = 2
	row += 1;

	// row = 3
	row += 1;
	list.AddItem( ROW_GETIP_DHCP, 0, m_pStrTable->String(E_STR_ID_IP_AUTO) );
	pCheck = dynamic_cast<GUI::CCheckCtrl*>( list.AddItem( ROW_GETIP_DHCP, 1, WND_TYPE_CHECK ) );
	assert( pCheck );
	//pCheck->SetCheck( true, false );


	// row = 4
	row += 1;
	list.AddItem( ROW_STATIC_IP, 0, m_pStrTable->String(E_STR_ID_IP_ADDRESS) );
	pIP = dynamic_cast<GUI::CIPAddressCtrl*>( list.AddItem( ROW_STATIC_IP, 1, WND_TYPE_IP) );
	assert( pIP );

	// row = 5
	row += 1;
	list.AddItem( ROW_SUB_NET_MASK, 0, m_pStrTable->String( E_STR_ID_NET_MASK ) );
	pIP = dynamic_cast<GUI::CIPAddressCtrl*>( list.AddItem( ROW_SUB_NET_MASK, 1, WND_TYPE_IP) );
	assert( pIP );

	// row = 6
	row += 1;
	list.AddItem( ROW_GATEWAY, 0, m_pStrTable->String( E_STR_ID_GATEWAY ) );
	pIP = dynamic_cast<GUI::CIPAddressCtrl*>( list.AddItem( ROW_GATEWAY, 1, WND_TYPE_IP) );
	assert( pIP );

	// row = 7
	row += 1;
	list.AddItem( ROW_DNS_1, 0, m_pStrTable->String( E_STR_ID_PREFERRED_DNS_SERVER ) );
	pIP = dynamic_cast<GUI::CIPAddressCtrl*>( list.AddItem( ROW_DNS_1, 1, WND_TYPE_IP) );
	assert( pIP );

	// row = 8
	row += 1;
	list.AddItem( ROW_DNS_2, 0, m_pStrTable->String( E_STR_ID_ALTERNATE_DNS_SERVER ) );
	pIP = dynamic_cast<GUI::CIPAddressCtrl*>( list.AddItem( ROW_DNS_2, 1, WND_TYPE_IP) );
	assert( pIP );

	// row = 9
	row += 1;
	list.AddItem( ROW_PPPOE_ENABLE, 0, m_pStrTable->String( E_STR_ID_PPPOE ) );
	pCheck = dynamic_cast<GUI::CCheckCtrl*>( list.AddItem( ROW_PPPOE_ENABLE, 1, WND_TYPE_CHECK ) );
	assert( pCheck );

	// row = 10
	row += 1;
	list.AddItem( ROW_PPPOE_USER_NAME, 0, m_pStrTable->String( E_STR_ID_USER_NAME ) );
	pEdit = dynamic_cast<GUI::CEditCtrl*>( list.AddItem( ROW_PPPOE_USER_NAME, 1, WND_TYPE_EDIT) );
	assert( pEdit );

	// row = 11
	row += 1;
	list.AddItem( ROW_PPPOE_PASSWORD, 0, m_pStrTable->String( E_STR_ID_PASSWORD ) );
	pEdit = dynamic_cast<GUI::CEditCtrl*>( list.AddItem( ROW_PPPOE_PASSWORD, 1, WND_TYPE_EDIT) );
	assert( pEdit );
	pEdit->SetPassword( true );

	// row = 12
	row += 1;
	pBtn = dynamic_cast<GUI::CButton*>(list.AddItem(ROW_NET_TEST,1,WND_TYPE_BUTTON,m_length[BTN_WIDTH],m_length[BTN_HEIGHT]));
	assert(pBtn);
	pBtn->SetCaption(m_pStrTable->String(E_STR_ID_TEST), false);

	//row = 13;
	m_poeNum = CProduct::Instance()->PoeInputNum();
	if(m_poeNum > 0)
	{
		row += 1;
		list.AddItem( ROW_POESWITCHIP, 0, m_pStrTable->String( E_STR_ID_SWITCHIP ));
		pIP = dynamic_cast<GUI::CIPAddressCtrl*>( list.AddItem( ROW_POESWITCHIP, 1, WND_TYPE_IP) );

		row += 1;
		list.AddItem( ROW_POESWITCH_SUBNETMASK, 0, m_pStrTable->String( E_STR_ID_SWITCHSUBNETMASK ));
		pIP = dynamic_cast<GUI::CIPAddressCtrl*>( list.AddItem( ROW_POESWITCH_SUBNETMASK, 1, WND_TYPE_IP) );
	}
#endif
	// end

	list.CreateListFocusMap();

	m_bCheckParam = false;
	m_StatusIPCheckOK = CHECK_STATUS_NOCHANGE;
	m_bAuto = false;
#if defined (__CUSTOM_IPV6__)
	m_bAutoIPv6=false;
#endif
}

unsigned long CCfgNetworkDlg::OnPreCommand(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	if( m_bCheckParam )
	{
		//在检测PPPOE参数的时候屏蔽其他消息响应
		return KEY_VIRTUAL_MSG_STOP;
	}
	else
	{
		return CCfgDlg::OnPreCommand( ctrlID, keyID ,xPos, yPos );
	}

}

void CCfgNetworkDlg::SetTip()
{
	GUI::CWnd* pWnd = m_cfgNetworkList.GetItemWnd(ROW_NET_TEST, COLUMN_DATA);
	if( NULL != pWnd )
	{
		AddTip(pWnd->GetID(), E_STR_ID_TIP_05_01);
	}
	else
	{
		printf("%s:%s:%d, GetItemWnd() error\n", __FUNCTION__, __FILE__, __LINE__);
		assert(false);
	}
}

bool CCfgNetworkDlg::ShowData(NCFG_ITEM_ID cfgID, const BYTE* pData, int len, bool bRefresh)
{
	assert(NULL != pData);
	assert(len > 0);
	GUI::CListCtrl& list = m_cfgNetworkList;

	BYTE* pTempData = NULL;
	unsigned long tempDataLen = 0;

	switch (cfgID)
	{
	case NCFG_ITEM_HTTP_PORT:
		{
			assert( len == sizeof(unsigned long));
			const unsigned long* pValue = reinterpret_cast<const unsigned long*>(pData);
			GUI::CNumEditCtrl* pEdit = dynamic_cast<GUI::CNumEditCtrl*>( list.GetItemWnd(ROW_HTTP_PORT, COLUMN_DATA) );
			assert( pEdit );
			pEdit->SetValue( *pValue, bRefresh );
		}
		return true;
		break;
	case NCFG_ITEM_SERVER_PORT:
		{
			assert( len == sizeof(unsigned long));
			const unsigned long* pValue = reinterpret_cast<const unsigned long*>(pData);
			GUI::CNumEditCtrl* pEdit = dynamic_cast<GUI::CNumEditCtrl*>( list.GetItemWnd(ROW_SERVER_PORT, COLUMN_DATA) );
			assert( pEdit );
			pEdit->SetValue( *pValue, bRefresh );
		}
		return true;
		break;
	case NCFG_ITEM_NETWORK_ADDR_INFO:
		{
			assert( len == sizeof(NETWORK_ADDR_INFO) );
			const NETWORK_ADDR_INFO* pValue = reinterpret_cast<const NETWORK_ADDR_INFO*>(pData);
			GUI::CCheckCtrl* pCheck = dynamic_cast<GUI::CCheckCtrl*>( list.GetItemWnd(ROW_GETIP_DHCP, COLUMN_DATA) );
			assert( pCheck );
			switch ( static_cast<GETIP_MODE>(pValue->bDHCP))
			{
			case GETIP_MODE_STATICIP:
				pCheck->SetCheck( false, bRefresh );
				m_bAuto = false;
				break;
			case GETIP_MODE_DHCP:
				{
					NETWORK_STATUS_INFO networkStatusInfo;
					memset( &networkStatusInfo, 0, sizeof(NETWORK_STATUS_INFO));
					MESSAGE_DATA msgData;
					memset(&msgData, 0, sizeof(msgData));
					CMessageMan::Instance()->SendMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_NETWORK_INFO, NULL, 0, msgData);

					networkStatusInfo = *reinterpret_cast<NETWORK_STATUS_INFO *>(msgData.pData+sizeof(PACKCMD));

					pCheck->SetCheck( true, bRefresh );
					if (networkStatusInfo.IP == 0)
					{
						CMessageMan::Instance()->ReleaseMsgBuff(msgData);
						break;
					}

					CMessageMan::Instance()->ReleaseMsgBuff(msgData);

					GUI::CIPAddressCtrl* pIP = dynamic_cast<GUI::CIPAddressCtrl*>(list.GetItemWnd(ROW_STATIC_IP,COLUMN_DATA));
					assert(pIP);
					pIP->SetIP(networkStatusInfo.IP,bRefresh);

					pIP = dynamic_cast<GUI::CIPAddressCtrl*>(list.GetItemWnd(ROW_SUB_NET_MASK,COLUMN_DATA));
					assert(pIP);
					pIP->SetIP(networkStatusInfo.subMask,bRefresh);

					pIP = dynamic_cast<GUI::CIPAddressCtrl*>(list.GetItemWnd(ROW_GATEWAY,COLUMN_DATA));
					assert(pIP);
					pIP->SetIP(networkStatusInfo.gateway,bRefresh);

					pIP = dynamic_cast<GUI::CIPAddressCtrl*>( list.GetItemWnd(ROW_DNS_1, COLUMN_DATA) );
					assert( pIP );
					pIP->SetIP( networkStatusInfo.DNS1, bRefresh );

					pIP = dynamic_cast<GUI::CIPAddressCtrl*>( list.GetItemWnd(ROW_DNS_2, COLUMN_DATA) );
					assert( pIP );
					pIP->SetIP( networkStatusInfo.DNS2, bRefresh );
					
					m_bAuto = true;
					/*if (networkStatusInfo.IP  != pValue->staticIP || networkStatusInfo.subMask  != pValue->netMask || networkStatusInfo.gateway  != pValue->gateway)
					{
						
						long dataLen = sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD) + sizeof(NETWORK_ADDR_INFO);
						unsigned char *pData = new unsigned char[dataLen];
						NCFG_BLOCK_HEAD *pBlockHead = (NCFG_BLOCK_HEAD *)pData;
						NCFG_ITEM_HEAD *pItemHead = (NCFG_ITEM_HEAD *)(pData + sizeof(NCFG_BLOCK_HEAD));
						pBlockHead->biSize = sizeof(NCFG_BLOCK_HEAD);
						pBlockHead->ItemNum = 1;
						pBlockHead->netcfgver = NCFG_VERSION;
						pItemHead[0].itemID = NCFG_ITEM_NETWORK_ADDR_INFO;
						pItemHead[0].len = sizeof(NETWORK_ADDR_INFO);
						pItemHead[0].num = 1;
						pItemHead[0].subLen = sizeof(NETWORK_ADDR_INFO);
						NETWORK_ADDR_INFO * pNetWorkInfo = (NETWORK_ADDR_INFO *)(pData+sizeof(NCFG_BLOCK_HEAD)+sizeof(NCFG_ITEM_HEAD));
						pNetWorkInfo->bDHCP = true;
						pNetWorkInfo->staticIP = networkStatusInfo.IP;
						pNetWorkInfo->netMask = networkStatusInfo.subMask;
						pNetWorkInfo->gateway = networkStatusInfo.gateway;
						memset(&msgData, 0, sizeof(msgData));
						CMessageMan::Instance()->SendMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_CFG_SET, const_cast<BYTE*>(pData), dataLen, msgData);
						CMessageMan::Instance()->ReleaseMsgBuff(msgData);
						pItemHead[0].itemID = NCFG_ITEM_DNS_1;
						pItemHead[0].len = sizeof(unsigned long);
						pItemHead[0].subLen = sizeof(unsigned long);
						unsigned long * pDns1 = (unsigned long *)(pData+sizeof(NCFG_BLOCK_HEAD)+sizeof(NCFG_ITEM_HEAD));
						*pDns1 = networkStatusInfo.DNS1;
						dataLen = sizeof(NCFG_BLOCK_HEAD)+sizeof(NCFG_ITEM_HEAD)+sizeof(unsigned long);
						memset(&msgData, 0, sizeof(msgData));
						CMessageMan::Instance()->SendMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_CFG_SET, const_cast<BYTE*>(pData), dataLen, msgData);
						CMessageMan::Instance()->ReleaseMsgBuff(msgData);
						pItemHead[0].itemID = NCFG_ITEM_DNS_2;
						pItemHead[0].len = sizeof(unsigned long);
						pItemHead[0].subLen = sizeof(unsigned long);
						unsigned long * pDns2 = (unsigned long *)(pData+sizeof(NCFG_BLOCK_HEAD)+sizeof(NCFG_ITEM_HEAD));
						*pDns2 = networkStatusInfo.DNS2;
						dataLen = sizeof(NCFG_BLOCK_HEAD)+sizeof(NCFG_ITEM_HEAD)+sizeof(unsigned long);
						memset(&msgData, 0, sizeof(msgData));
						CMessageMan::Instance()->SendMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_CFG_SET, const_cast<BYTE*>(pData), dataLen, msgData);
						CMessageMan::Instance()->ReleaseMsgBuff(msgData);
						delete [] pData;
						pData = NULL;
					}*/
					
					CheckEnableUI(bRefresh, REFRASH_UI_AUTOIP);
					
					return true;
				}
				break;
			default:
				assert(false);
			}
		
			GUI::CIPAddressCtrl* pIP = dynamic_cast<GUI::CIPAddressCtrl*>(list.GetItemWnd(ROW_STATIC_IP,COLUMN_DATA));
			assert(pIP);
			pIP->SetIP(pValue->staticIP,bRefresh);
			
			pIP = dynamic_cast<GUI::CIPAddressCtrl*>(list.GetItemWnd(ROW_SUB_NET_MASK,COLUMN_DATA));
			assert(pIP);
			pIP->SetIP(pValue->netMask,bRefresh);
			
			pIP = dynamic_cast<GUI::CIPAddressCtrl*>(list.GetItemWnd(ROW_GATEWAY,COLUMN_DATA));
			assert(pIP);
			pIP->SetIP(pValue->gateway,bRefresh);
			
			CheckEnableUI(bRefresh, REFRASH_UI_AUTOIP);
		}
		return true;
		break;
	case NCFG_ITEM_DNS_1:
		{
			if (m_bAuto)
			{
				return true;
			}
			assert( len == sizeof(unsigned long) );
			const unsigned long* pUlong = reinterpret_cast<const unsigned long*>(pData);
			GUI::CIPAddressCtrl* pIp = dynamic_cast<GUI::CIPAddressCtrl*>( list.GetItemWnd(ROW_DNS_1, COLUMN_DATA) );
			assert( pIp );
			pIp->SetIP( *pUlong, bRefresh );
		}
		return true;
		break;
	case NCFG_ITEM_DNS_2:
		{
			if (m_bAuto)
			{
				return true;
			}
			assert( len == sizeof(unsigned long) );
			const unsigned long* pUlong = reinterpret_cast<const unsigned long*>(pData);
			GUI::CIPAddressCtrl* pIp = dynamic_cast<GUI::CIPAddressCtrl*>( list.GetItemWnd(ROW_DNS_2, COLUMN_DATA) );
			assert( pIp );
			pIp->SetIP( *pUlong, bRefresh );
		}
		return true;
		break;
	case NCFG_ITEM_PPPOE_INFO:
		{
			assert( len == sizeof(PPPOE_INFO) );
			CCfgDlg::GetConfig(NCFG_ITEM_PPPOE_USER_NAME_MAX_LEN,&pTempData, tempDataLen);
			assert(tempDataLen==sizeof(unsigned long));
			unsigned long maxUserLen = *reinterpret_cast<unsigned long*>(pTempData);

			CCfgDlg::GetConfig(NCFG_ITEM_PPPOE_PASSWORD_MAX_LEN,&pTempData,tempDataLen);
			assert(tempDataLen==sizeof(unsigned long));
			unsigned long maxPassLen = *reinterpret_cast<unsigned long*>(pTempData);

			const PPPOE_INFO* pPPPOE = reinterpret_cast<const PPPOE_INFO*>(pData);
			GUI::CCheckCtrl* pCheck = dynamic_cast<GUI::CCheckCtrl*>( list.GetItemWnd(ROW_PPPOE_ENABLE, COLUMN_DATA) );
			assert( pCheck );
			pCheck->SetCheck( pPPPOE->bEnable, bRefresh );

			GUI::CEditCtrl* pEdit = dynamic_cast<GUI::CEditCtrl*>(list.GetItemWnd(ROW_PPPOE_USER_NAME,COLUMN_DATA));
			assert(pEdit);
			pEdit->SetMaxStringLen(maxUserLen);
			pEdit->SetCaption(pPPPOE->user,bRefresh);
		
			pEdit = dynamic_cast<GUI::CEditCtrl*>(list.GetItemWnd(ROW_PPPOE_PASSWORD,COLUMN_DATA));
			assert(pEdit);
			pEdit->SetMaxStringLen(maxPassLen);
			pEdit->SetCaption(pPPPOE->password,bRefresh);

			CheckEnableUI(bRefresh, REFRASH_UI_PPPOE);
		}
		return true;
		break;
#if defined (__CUSTOM_IPV6__)
	case NCFG_ITEM_USE_IPV4_IPV6:
		{
			//ipv4和ipv6目前选全部启用默认配置
			//assert(len == sizeof(USE_IPV4_IPV6));
			//const USE_IPV4_IPV6 *pValue = reinterpret_cast<const USE_IPV4_IPV6*>(pData);
			//GUI::CCheckCtrl *pCheck = dynamic_cast<GUI::CCheckCtrl*>(list.GetItemWnd(ROW_IPV4_CHECK, COLUMN_DATA));
			//assert(pCheck);
			//pCheck->SetCheck(pValue->bEnableIPv4, bRefresh);

			//pCheck = dynamic_cast<GUI::CCheckCtrl*>(list.GetItemWnd(ROW_IPV6_CHECK, COLUMN_DATA));
			//assert(pCheck);
			//pCheck->SetCheck(pValue->bEnableIPv6, bRefresh);
		}
		return true;
		break;
	case NCFG_ITEM_NETWORK_ADDR_INFO_IPV6:
		{
			assert(len == sizeof(NETWORK_ADDR_INFO_IPv6));
			const NETWORK_ADDR_INFO_IPv6* pValue = reinterpret_cast<const NETWORK_ADDR_INFO_IPv6*>(pData);
			//GUI::CCheckCtrl* pCheck = dynamic_cast<GUI::CCheckCtrl*>( list.GetItemWnd(ROW_IPV6_GETIP_DHCP, COLUMN_DATA) );
			assert( pCheck );
			switch ( static_cast<GETIP_MODE>(pValue->useDHCP))
			{
			case GETIP_MODE_STATICIP:
				{
					//pCheck->SetCheck( false, bRefresh );
					m_bAutoIPv6 = false;
				}
				break;
			case GETIP_MODE_DHCP:
				{
					//pCheck->SetCheck( true, bRefresh );
					m_bAutoIPv6 = true;
					//动态获取IPv6的IP
				}
				break;
			default:
				break;
			}
			
			{
				if(m_width > 800)
				{
					GUI::CIPV6Ctrl* pIPv6 = dynamic_cast<GUI::CIPV6Ctrl*>(list.GetItemWnd(ROW_IPV6_STATIC_IP,COLUMN_DATA));
					assert(pIPv6);
					pIPv6->SetIP(pValue->IP, sizeof(pValue->IP), bRefresh);
				}
				else
				{
					GUI::CEditCtrl *pEdit = dynamic_cast<GUI::CEditCtrl*>(list.GetItemWnd(ROW_IPV6_STATIC_IP,COLUMN_DATA));
					assert(pIPv6);
					pEdit->SetMaxStringLen( sizeof(pValue->IP) );
					pEdit->SetCaption( (char *)(pValue->IP), bRefresh);
				}
			

				GUI::CNumEditCtrl *pSubMask =  dynamic_cast<GUI::CNumEditCtrl*>(list.GetItemWnd(ROW_IPV6_SUB_NET_MASK, COLUMN_DATA));
				assert(pSubMask);			
				pSubMask->SetValue( pValue->subnetMask, bRefresh );
				
				if(m_width > 800)
				{
					GUI::CIPV6Ctrl *pIPv6 = dynamic_cast<GUI::CIPV6Ctrl*>(list.GetItemWnd(ROW_IPV6_GATEWAY,COLUMN_DATA));
					assert(pIPv6);
					pIPv6->SetIP(pValue->gateway, sizeof(pValue->gateway), bRefresh);
				}
				else
				{
					GUI::CEditCtrl *pEdit = dynamic_cast<GUI::CEditCtrl*>(list.GetItemWnd(ROW_IPV6_GATEWAY,COLUMN_DATA));
					assert(pIPv6);
					pEdit->SetMaxStringLen( sizeof(pValue->IP) );
					pEdit->SetCaption( (char *)(pValue->gateway), bRefresh);
				}
			
			}
		}
		return true;
		break;
	case NCFG_ITEM_DNS_1_IPV6:
		{
			//临时隐藏
			/*if (m_bAutoIPv6)
			{
				return true;
			}
			//assert(len == sizeof(unsigned char[40]));
			//const unsigned char *pChar = reinterpret_cast<const unsigned char*>(pData);
			//GUI::CIPV6Ctrl* pIPv6 = dynamic_cast<GUI::CIPV6Ctrl*>( list.GetItemWnd(ROW_IPV6_DNS_1, COLUMN_DATA) );
			//assert( pIPv6 );
			//pIPv6->SetIP( pChar, len, bRefresh );*/
		}
		return true;
		break;
	case NCFG_ITEM_DNS_2_IPV6:
		{
			//临时隐藏
			/*if (m_bAutoIPv6)
			{
				return true;
			}
			//assert(len == sizeof(unsigned char[40]));
			//const unsigned char *pChar = reinterpret_cast<const unsigned char*>(pData);
			//GUI::CIPV6Ctrl* pIPv6 = dynamic_cast<GUI::CIPV6Ctrl*>( list.GetItemWnd(ROW_IPV6_DNS_2, COLUMN_DATA) );
			//assert( pIPv6 );
			//pIPv6->SetIP( pChar, len, bRefresh );*/
		}
		return true;
		break;
#endif
	case NCFG_ITEM_SWITCH_INFO:
		{
			if(m_poeNum > 0)
			{
				assert(len == sizeof(NET_SWITCH_INFO));
				const NET_SWITCH_INFO *pInfo =  reinterpret_cast<const NET_SWITCH_INFO*>(pData);
				GUI::CIPAddressCtrl *piP = dynamic_cast<GUI::CIPAddressCtrl*>( list.GetItemWnd(ROW_POESWITCHIP, COLUMN_DATA) );
				assert(piP);
				piP -> SetIP(pInfo->switchIP,bRefresh);
				piP = dynamic_cast<GUI::CIPAddressCtrl*>( list.GetItemWnd(ROW_POESWITCH_SUBNETMASK, COLUMN_DATA) );
				assert(piP);
				piP -> SetIP(pInfo ->netMask,bRefresh);
			}
		}
		return true;
		break;
	default:
		break;
	}

	return false;
}

bool CCfgNetworkDlg::SetData(NCFG_ITEM_ID cfgID, BYTE*& pData, int& len)  const
{
	assert(NULL != pData);
	assert(len > 0);
	
	const GUI::CListCtrl& list = m_cfgNetworkList;

	switch (cfgID)
	{
	case NCFG_ITEM_HTTP_PORT:
		{
			assert( len == sizeof(unsigned long));
			unsigned long* pValue = reinterpret_cast<unsigned long*>(pData);
			const GUI::CNumEditCtrl* pEdit = dynamic_cast<const GUI::CNumEditCtrl*>( list.GetItemWnd(ROW_HTTP_PORT, COLUMN_DATA) );
			assert( pEdit );
			*pValue = pEdit->GetValue();
		}
		return true;
		break;
	case NCFG_ITEM_SERVER_PORT:
		{
			assert( len == sizeof(unsigned long));
			unsigned long* pValue = reinterpret_cast<unsigned long*>(pData);
			const GUI::CNumEditCtrl* pEdit = dynamic_cast<const GUI::CNumEditCtrl*>( list.GetItemWnd(ROW_SERVER_PORT, COLUMN_DATA) );
			assert( pEdit );
			*pValue = pEdit->GetValue();
		}
		return true;
		break;
	case NCFG_ITEM_NETWORK_ADDR_INFO:
		{
			assert( len == sizeof(NETWORK_ADDR_INFO) );
			NETWORK_ADDR_INFO* pValue = reinterpret_cast<NETWORK_ADDR_INFO*>(pData);
			const GUI::CCheckCtrl* pCheck = dynamic_cast<const GUI::CCheckCtrl*>( list.GetItemWnd(ROW_GETIP_DHCP, COLUMN_DATA) );
			assert( pCheck );
			if( pCheck->IsCheck() )
			{
				pValue->bDHCP = true;
			}
			else
			{
				pValue->bDHCP = false;
			}

			const GUI::CIPAddressCtrl* pIP = dynamic_cast<const GUI::CIPAddressCtrl*>(list.GetItemWnd(ROW_STATIC_IP,COLUMN_DATA));
			assert(pIP);
			pValue->staticIP = pIP->GetIP();

			pIP = dynamic_cast<const GUI::CIPAddressCtrl*>(list.GetItemWnd(ROW_SUB_NET_MASK,COLUMN_DATA));
			assert(pIP);
			pValue->netMask = pIP->GetIP();

			pIP = dynamic_cast<const GUI::CIPAddressCtrl*>(list.GetItemWnd(ROW_GATEWAY,COLUMN_DATA));
			assert(pIP);
			pValue->gateway = pIP->GetIP();
		}
		return true;
		break;
	case NCFG_ITEM_DNS_1:
		{
			assert( len == sizeof(unsigned long) );
			unsigned long* pUlongLong = reinterpret_cast<unsigned long*>(pData);
			const GUI::CIPAddressCtrl* pIp = dynamic_cast<const GUI::CIPAddressCtrl*>( list.GetItemWnd(ROW_DNS_1, COLUMN_DATA) );
			assert( pIp );
			*pUlongLong = pIp->GetIP();
		}
		return true;
		break;
	case NCFG_ITEM_DNS_2:
		{
			assert( len == sizeof(unsigned long) );
			unsigned long* pUlongLong = reinterpret_cast<unsigned long*>(pData);
			const GUI::CIPAddressCtrl* pIp = dynamic_cast<const GUI::CIPAddressCtrl*>( list.GetItemWnd(ROW_DNS_2, COLUMN_DATA) );
			assert( pIp );
			*pUlongLong = pIp->GetIP();
		}
		return true;
		break;
	case NCFG_ITEM_PPPOE_INFO:
		{
			assert( len == sizeof(PPPOE_INFO) );
			PPPOE_INFO* pValue = reinterpret_cast<PPPOE_INFO*>(pData);
			const GUI::CCheckCtrl* pCheck = dynamic_cast<const GUI::CCheckCtrl*>( list.GetItemWnd(ROW_PPPOE_ENABLE, COLUMN_DATA) );
			assert( pCheck );
			if( pCheck->IsCheck() )
			{
				pValue->bEnable = 1;
			}
			else
			{
				pValue->bEnable = 0;
			}

			const GUI::CEditCtrl* pEditUser = dynamic_cast<const GUI::CEditCtrl*>(list.GetItemWnd(ROW_PPPOE_USER_NAME,COLUMN_DATA));
			assert(pEditUser);
			strcpy(pValue->user,pEditUser->GetCaption().c_str());

			const GUI::CEditCtrl* pEditPass = dynamic_cast<const GUI::CEditCtrl*>(list.GetItemWnd(ROW_PPPOE_PASSWORD,COLUMN_DATA));
			assert(pEditPass);
			strcpy(pValue->password,pEditPass->GetCaption().c_str());
		}
		return true;
		break;
#if defined (__CUSTOM_IPV6__)
	case NCFG_ITEM_USE_IPV4_IPV6:
		{
			//目前ipv4和ipv6启用都采用默认配置
		/*	assert(len == sizeof(USE_IPV4_IPV6));
			USE_IPV4_IPV6 *pUseIPv = reinterpret_cast<USE_IPV4_IPV6*>(pData);
			const GUI::CCheckCtrl *pCheck = dynamic_cast<const GUI::CCheckCtrl*>(list.GetItemWnd(ROW_IPV4_CHECK, COLUMN_DATA));
			assert(pCheck);
			pCheck->IsCheck()?(pUseIPv->bEnableIPv4 = 1):(pUseIPv->bEnableIPv4 = 0);

			pCheck = dynamic_cast<const GUI::CCheckCtrl*>(list.GetItemWnd(ROW_IPV6_CHECK, COLUMN_DATA));
			assert(pCheck);
			pCheck->IsCheck()?(pUseIPv->bEnableIPv6 = 1):(pUseIPv->bEnableIPv6 = 0);*/
		}
		return true;
		break;
	case NCFG_ITEM_NETWORK_ADDR_INFO_IPV6:
		{
			assert(len == sizeof(NETWORK_ADDR_INFO_IPv6));
			NETWORK_ADDR_INFO_IPv6 *pNetWorkIPv6 = reinterpret_cast<NETWORK_ADDR_INFO_IPv6*>(pData);
			//const GUI::CCheckCtrl *pCheck = dynamic_cast<const GUI::CCheckCtrl*>(list.GetItemWnd(ROW_IPV6_GETIP_DHCP, COLUMN_DATA));
			//assert(pCheck);
			//pCheck->IsCheck()?(pNetWorkIPv6->useDHCP = 1):(pNetWorkIPv6->useDHCP = 0);
			
			if(m_width > 800)
			{
				const GUI::CIPV6Ctrl* pIPv6 = dynamic_cast<const GUI::CIPV6Ctrl*>(list.GetItemWnd(ROW_IPV6_STATIC_IP, COLUMN_DATA));
				assert(pIPv6);
				pIPv6->GetIP(pNetWorkIPv6->IP, sizeof(pNetWorkIPv6->IP));
			}
			else
			{
				const GUI::CEditCtrl* pIpV6Edit = dynamic_cast<const GUI::CEditCtrl*>(list.GetItemWnd(ROW_IPV6_STATIC_IP, COLUMN_DATA));
				assert(pIpV6Edit);
				strcpy((char *)(pNetWorkIPv6->IP),pIpV6Edit->GetCaption().c_str());
				
			}

			const GUI::CNumEditCtrl *pIPEdit = dynamic_cast<const GUI::CNumEditCtrl*>(list.GetItemWnd(ROW_IPV6_SUB_NET_MASK, COLUMN_DATA));
			assert(pIPEdit);
			pNetWorkIPv6->subnetMask = pIPEdit->GetValue();
			
			if(m_width > 800)
			{
				const GUI::CIPV6Ctrl*pIPv6 = dynamic_cast<const GUI::CIPV6Ctrl*>(list.GetItemWnd(ROW_IPV6_GATEWAY, COLUMN_DATA));
				assert(pIP);
				pIPv6->GetIP(pNetWorkIPv6->gateway, sizeof(pNetWorkIPv6->gateway));
			}
			else
			{
				const GUI::CEditCtrl* pIpV6Edit = dynamic_cast<const GUI::CEditCtrl*>(list.GetItemWnd(ROW_IPV6_GATEWAY, COLUMN_DATA));
				assert(pIpV6Edit);
				strcpy((char *)(pNetWorkIPv6->gateway),pIpV6Edit->GetCaption().c_str());
			}
			
		}
		return true;
		break;
	case NCFG_ITEM_DNS_1_IPV6:
		{
			//临时隐藏
		/*	assert(len == sizeof(unsigned char[40]));
			unsigned char *pChar = reinterpret_cast<unsigned char*>(pData);
			const GUI::CIPV6Ctrl* pIPv6 = dynamic_cast<const GUI::CIPV6Ctrl*>( list.GetItemWnd(ROW_IPV6_DNS_1, COLUMN_DATA) );
			assert( pIPv6 );
			pIPv6->GetIP(pChar, len);*/
		}
		return true;
		break;
	case NCFG_ITEM_DNS_2_IPV6:
		{
			//临时隐藏
		/*	assert( len == sizeof(unsigned char[40]) );
			unsigned char *pChar = reinterpret_cast<unsigned char*>(pData);
			const GUI::CIPV6Ctrl* pIPv6 = dynamic_cast<const GUI::CIPV6Ctrl*>( list.GetItemWnd(ROW_IPV6_DNS_2, COLUMN_DATA) );
			assert( pIPv6 );
			pIPv6->GetIP(pChar, len);*/
		}
		return true;
		break;
#endif
	case NCFG_ITEM_SWITCH_INFO:
		{
			if(m_poeNum > 0)
			{
				assert( len == sizeof(NET_SWITCH_INFO) );
				NET_SWITCH_INFO *pinfo = reinterpret_cast< NET_SWITCH_INFO *>(pData);
				const GUI::CIPAddressCtrl* pIP = dynamic_cast<const GUI::CIPAddressCtrl*>( list.GetItemWnd(ROW_POESWITCHIP, COLUMN_DATA) );
				assert( pIP );
				pinfo->switchIP = pIP->GetIP();
				pIP = dynamic_cast<const GUI::CIPAddressCtrl*>( list.GetItemWnd(ROW_POESWITCH_SUBNETMASK, COLUMN_DATA) );
				pinfo->netMask = pIP->GetIP();
			}
		}
		return true;
		break;
	default:
		break;
	}

	return false;
}

void CCfgNetworkDlg::CheckEnableUI( bool bRefresh, REFRASH_UI_NUM uiNum )
{
	GUI::CCheckCtrl* pCheck = NULL;

	switch( uiNum )
	{
	case REFRASH_UI_AUTOIP:
		pCheck = dynamic_cast<GUI::CCheckCtrl*>(m_cfgNetworkList.GetItemWnd( ROW_GETIP_DHCP, COLUMN_DATA ));
		if( pCheck )
		{
			m_GetIpMode.bGetIpModeAuto = pCheck->IsCheck();
			if( m_GetIpMode.bGetIpModeAuto )
			{
				m_GetIpMode.bGetIpModePPPOE = false;
			}
		}
		else
		{	
			assert( false );
		}
		break;
	case REFRASH_UI_PPPOE:
		pCheck = dynamic_cast<GUI::CCheckCtrl*>(m_cfgNetworkList.GetItemWnd( ROW_PPPOE_ENABLE, COLUMN_DATA ));
		if( pCheck )
		{
			m_GetIpMode.bGetIpModePPPOE = pCheck->IsCheck();
			if( m_GetIpMode.bGetIpModePPPOE )
			{
				m_GetIpMode.bGetIpModeAuto = false;
			}
		}
		else
		{	
			assert( false );
		}
		break;
#if defined (__CUSTOM_IPV6__)
	case REFRASH_UI_IPV6_AUTOIP:
		//pCheck = dynamic_cast<GUI::CCheckCtrl*>(m_cfgNetworkList.GetItemWnd( ROW_IPV6_GETIP_DHCP, COLUMN_DATA ));
		//m_bAutoGetIpV6 = pCheck->IsCheck();
		break;
#endif
	default:
		assert( false ); 
		break;
	}
	DoUpdateUI( m_GetIpMode , bRefresh , uiNum );

}

void CCfgNetworkDlg::DoUpdateUI( GET_IP_MODE& getIpMode , bool bRefresh, REFRASH_UI_NUM uiNum )
{
	switch( uiNum )
	{
	case REFRASH_UI_AUTOIP:
	case REFRASH_UI_PPPOE:

		//静态IP和DNS
		m_cfgNetworkList.GetItemWnd(ROW_STATIC_IP,COLUMN_DATA)->SetEnable((!m_GetIpMode.bGetIpModeAuto)&&(!m_GetIpMode.bGetIpModePPPOE),bRefresh);
		m_cfgNetworkList.GetItemWnd(ROW_SUB_NET_MASK,COLUMN_DATA)->SetEnable((!m_GetIpMode.bGetIpModeAuto)&&(!m_GetIpMode.bGetIpModePPPOE),bRefresh);
		m_cfgNetworkList.GetItemWnd(ROW_GATEWAY,COLUMN_DATA)->SetEnable((!m_GetIpMode.bGetIpModeAuto)&&(!m_GetIpMode.bGetIpModePPPOE),bRefresh);
		m_cfgNetworkList.GetItemWnd(ROW_DNS_1,COLUMN_DATA)->SetEnable((!m_GetIpMode.bGetIpModeAuto)&&(!m_GetIpMode.bGetIpModePPPOE),bRefresh);
		m_cfgNetworkList.GetItemWnd(ROW_DNS_2,COLUMN_DATA)->SetEnable((!m_GetIpMode.bGetIpModeAuto)&&(!m_GetIpMode.bGetIpModePPPOE),bRefresh);

		//PPPOE
		m_cfgNetworkList.GetItemWnd(ROW_PPPOE_USER_NAME,COLUMN_DATA)->SetEnable(m_GetIpMode.bGetIpModePPPOE,bRefresh);
		m_cfgNetworkList.GetItemWnd(ROW_PPPOE_PASSWORD,COLUMN_DATA)->SetEnable(m_GetIpMode.bGetIpModePPPOE,bRefresh);

		//check
		dynamic_cast<GUI::CCheckCtrl*>(m_cfgNetworkList.GetItemWnd(ROW_GETIP_DHCP,COLUMN_DATA))->SetCheck(m_GetIpMode.bGetIpModeAuto,bRefresh);
		dynamic_cast<GUI::CCheckCtrl*>(m_cfgNetworkList.GetItemWnd(ROW_PPPOE_ENABLE,COLUMN_DATA))->SetCheck(m_GetIpMode.bGetIpModePPPOE,bRefresh);
		break;

#if defined (__CUSTOM_IPV6__)
	case REFRASH_UI_IPV6_AUTOIP:
		m_cfgNetworkList.GetItemWnd(ROW_IPV6_STATIC_IP,COLUMN_DATA)->SetEnable(!m_bAutoGetIpV6,bRefresh);
		m_cfgNetworkList.GetItemWnd(ROW_IPV6_SUB_NET_MASK,COLUMN_DATA)->SetEnable(!m_bAutoGetIpV6,bRefresh);
		m_cfgNetworkList.GetItemWnd(ROW_IPV6_GATEWAY,COLUMN_DATA)->SetEnable(!m_bAutoGetIpV6,bRefresh);
		//临时隐藏
		//m_cfgNetworkList.GetItemWnd(ROW_IPV6_DNS_1,COLUMN_DATA)->SetEnable(!m_bAutoGetIpV6,bRefresh);
		//m_cfgNetworkList.GetItemWnd(ROW_IPV6_DNS_2,COLUMN_DATA)->SetEnable(!m_bAutoGetIpV6,bRefresh);
		break;
#endif
	default:
		assert( false ); 
		break;
	}
}



BEGIN_MSG_MAP(CCfgNetworkDlg, GUI::CDialog)
	ON_MSG_EX(m_cfgNetworkList.GetID(), KEY_VIRTUAL_MSG_DEFAULT_TAG, OnClickList)
END_MSG_MAP()

unsigned long CCfgNetworkDlg::OnClickList(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	unsigned short row = (keyID >> 16)&0x7fff;	//行
	unsigned short col = keyID&0xffff;			//列

	// 是否使用自动分配IP
	if( ( ROW_GETIP_DHCP == row && COLUMN_DATA == col) )
	{
		CheckEnableUI( true , REFRASH_UI_AUTOIP );
	}
#if defined (__CUSTOM_IPV6__)
	// 是否使用IPV6自动分配IP
	if( ( ROW_IPV6_GETIP_DHCP == row && COLUMN_DATA == col) )
	{
		CheckEnableUI( true , REFRASH_UI_IPV6_AUTOIP );
	}
#endif
	// 是否启用PPPOE
	if( ROW_PPPOE_ENABLE == row && COLUMN_DATA == col )
	{
		CheckEnableUI( true, REFRASH_UI_PPPOE );
	}

	//测试网络
	if( ROW_NET_TEST == row && COLUMN_DATA == col )
	{
		unsigned long errNum = CheckDhcpParam();

		MessageOut( GetTopestWnd(),static_cast<ENUM_STRING_ID>(errNum), TIP_ERR, false );
	
		GetTopestWnd()->Repaint();
	}

	switch (row)
	{
	case ROW_GETIP_DHCP:
	case ROW_STATIC_IP:
	case ROW_SUB_NET_MASK:
	case ROW_GATEWAY:
		m_StatusIPCheckOK = CHECK_STATUS_NOCHECK;
		break;
	case ROW_PPPOE_ENABLE:
	case ROW_PPPOE_USER_NAME:
	case ROW_PPPOE_PASSWORD:
		m_StatusIPCheckOK = CHECK_STATUS_NOCHECK;
		break;
	default:
		break;
	}

	if (!IsModify())
	{
		switch (row)
		{
		case ROW_HTTP_PORT:
			if (IsChange(NCFG_ITEM_HTTP_PORT))
			{
				SetModify(true);
			}
			return KEY_VIRTUAL_MSG_CONTINUE;
			break;
		case ROW_SERVER_PORT:
			if(IsChange( NCFG_ITEM_SERVER_PORT ))
			{
				SetModify(true);
			}
			return KEY_VIRTUAL_MSG_CONTINUE;
			break;
#if defined (__CUSTOM_IPV6__)
		/*case ROW_IPV4_CHECK:
			if(IsChange(NCFG_ITEM_USE_IPV4_IPV6))
			{
				SetModify(true);
			}
			return KEY_VIRTUAL_MSG_CONTINUE;*/
#endif
		case ROW_GETIP_DHCP:
		case ROW_STATIC_IP:
		case ROW_SUB_NET_MASK:
		case ROW_GATEWAY:
			if(IsChange(NCFG_ITEM_NETWORK_ADDR_INFO))
			{
				SetModify(true);
			}
			return KEY_VIRTUAL_MSG_CONTINUE;
			break;
		case ROW_DNS_1:
			if(IsChange(NCFG_ITEM_DNS_1))
			{
				SetModify(true);
			}
			return KEY_VIRTUAL_MSG_CONTINUE;
			break;
		case ROW_DNS_2:
			if(IsChange(NCFG_ITEM_DNS_2))
			{
				SetModify(true);
			}
			return KEY_VIRTUAL_MSG_CONTINUE;
			break;
		case ROW_PPPOE_ENABLE:
		case ROW_PPPOE_USER_NAME:
		case ROW_PPPOE_PASSWORD:
			if(IsChange(NCFG_ITEM_PPPOE_INFO))
			{
				SetModify(true);
			}
			return KEY_VIRTUAL_MSG_CONTINUE;
			break;
#if defined (__CUSTOM_IPV6__)
		/*case ROW_IPV6_CHECK:
			if(IsChange(NCFG_ITEM_USE_IPV4_IPV6))
			{
				SetModify(true);
			}
			return KEY_VIRTUAL_MSG_CONTINUE;
			break;*/ 
		//case ROW_IPV6_GETIP_DHCP:
		case ROW_IPV6_STATIC_IP:
		case ROW_IPV6_SUB_NET_MASK:
		case ROW_IPV6_GATEWAY:
			if(IsChange(NCFG_ITEM_NETWORK_ADDR_INFO_IPV6))
			{
				SetModify(true);
			}
			return KEY_VIRTUAL_MSG_CONTINUE;
			break;
		//临时隐藏
		/*case ROW_IPV6_DNS_1:
			if(IsChange(NCFG_ITEM_DNS_1_IPV6))
			{
				SetModify(true);
			}
			return KEY_VIRTUAL_MSG_CONTINUE;
			break;
		case ROW_IPV6_DNS_2:
			if(IsChange(NCFG_ITEM_DNS_2_IPV6))
			{
				SetModify(true);
			}
			return KEY_VIRTUAL_MSG_CONTINUE;
			break; */
#endif
		case ROW_POESWITCHIP:
		case ROW_POESWITCH_SUBNETMASK:
			if(IsChange(NCFG_ITEM_SWITCH_INFO))
			{
				SetModify(true);
			}
			return KEY_VIRTUAL_MSG_CONTINUE;
			break;
		default:
			break;
		}
	}

	return KEY_VIRTUAL_MSG_STOP;
}

bool CCfgNetworkDlg::CheckPOEIP()
{	
	unsigned long poeIP,poeSubnetMask,ip,subNetMask,poeNetTag,netTag = 0;
	GUI::CIPAddressCtrl *pIP = dynamic_cast<GUI::CIPAddressCtrl*>(m_cfgNetworkList.GetItemWnd(ROW_POESWITCHIP,COLUMN_DATA));
	poeIP = pIP -> GetIP();
	pIP = dynamic_cast<GUI::CIPAddressCtrl*>(m_cfgNetworkList.GetItemWnd(ROW_POESWITCH_SUBNETMASK,COLUMN_DATA));
	poeSubnetMask = pIP -> GetIP();

	//
	GUI::CCheckCtrl* pCheck = dynamic_cast<GUI::CCheckCtrl*>( m_cfgNetworkList.GetItemWnd(ROW_GETIP_DHCP, COLUMN_DATA) );
	if(pCheck->IsCheck())
	{
		NETWORK_STATUS_INFO networkStatusInfo;
		memset( &networkStatusInfo, 0, sizeof(NETWORK_STATUS_INFO));
		
		MESSAGE_DATA msgData;
		memset(&msgData, 0, sizeof(msgData));
		CMessageMan::Instance()->SendMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_DTNETWORK, NULL , 0, msgData);
		networkStatusInfo = *reinterpret_cast<NETWORK_STATUS_INFO *>(msgData.pData+sizeof(PACKCMD));
		ip = networkStatusInfo.IP;
		subNetMask = networkStatusInfo.subMask;
		CMessageMan::Instance()->ReleaseMsgBuff(msgData);
	}
	else
	{
		m_bAuto = false;
		pIP = dynamic_cast<GUI::CIPAddressCtrl*>(m_cfgNetworkList.GetItemWnd(ROW_STATIC_IP,COLUMN_DATA));
		ip = pIP -> GetIP();
		pIP = dynamic_cast<GUI::CIPAddressCtrl*>(m_cfgNetworkList.GetItemWnd(ROW_SUB_NET_MASK,COLUMN_DATA));
		subNetMask = pIP ->GetIP();
	}

	poeNetTag = poeIP & poeSubnetMask & subNetMask;
	netTag = ip & subNetMask & subNetMask;
	if(netTag == poeNetTag)
	{
		return false;
	}

	return true;
}

unsigned long CCfgNetworkDlg::GetErrorTextNum()
{
	ENUM_STRING_ID strID = E_STR_ID_NULL;
	
	switch( m_netCheckResult )
	{
	case NETERR_NETPARAMCHECK_OK:						//测试通过
		strID = E_STR_ID_TIP_04_05;
		break;
	case NETERR_NETPARAMCHECK_ENAMEPWD:					//用户名或者密码错误
		strID = E_STR_ID_MESSAGE_37;
		break;
	case NETERR_NETPARAMCHECK_ECONNECT:					//网络未连接,未插网线
		strID = E_STR_ID_MESSAGE_58;
		break;
	case NETERR_NETPARMACHECK_ENETUNREACHD:				//网络不可达
		strID = E_STR_ID_MESSAGE_58;
		break;
	case NETERR_NETPARAMCHECK_ETIMEOUT:					//网络连接超时
		strID = E_STR_ID_MESSAGE_87;
		break;
	case NETERR_NETPARAMCHECK_EUNKNOWN:					//未知错误	
		strID = E_STR_ID_MESSAGE_59;
		break;
	case NETERR_NETPARAMCHECK_EIPCONFLICT:				//静态设置的IP冲突
		strID = E_STR_ID_MESSAGE_27;
		break;
	case NETERR_NETPARAMCHECK_EIPORSUBMASK:				//IP或者子网掩码设置出错
		strID = E_STR_ID_MESSAGE_23;
		break;
	case NETERR_NETPARAMCHECK_ENONICDEVICE:				//未发现指定的网络接口
		strID = E_STR_ID_MESSAGE_71;
		break;
	case NETERR_NETPARAMCHECK_EUSERNAMEEMPTY:			//用户名不能为空
		strID = E_STR_ID_MESSAGE_89;
		break;
	default:
		strID = E_STR_ID_MESSAGE_58;
		break;
	}

	return strID;
}



unsigned long CCfgNetworkDlg::CheckDhcpParam()
{
	unsigned char *pData = NULL;
	int dataLen = 0;
	NET_PROTOCOL netCmdRequest;
	MESSAGE_DATA msgData;
	memset( &msgData, 0, sizeof( msgData ) );

	GUI::CListCtrl& list = m_cfgNetworkList;
	
	
	m_bCheckParam = true;								//设置该标记，屏蔽外部输入

	//check 标志
	GUI::CCheckCtrl* pCheck = dynamic_cast<GUI::CCheckCtrl*>( list.GetItemWnd( ROW_GETIP_DHCP, COLUMN_DATA) );
	bool bUseDHCP = pCheck->IsCheck();
	pCheck = dynamic_cast<GUI::CCheckCtrl*>( list.GetItemWnd( ROW_PPPOE_ENABLE, COLUMN_DATA) );
	bool bUsePppoe = pCheck->IsCheck();

	if( ( !bUseDHCP ) && ( !bUsePppoe ) )
	{
		//使用静态IP
		pData = new unsigned char [sizeof( NETWORK_ADDR_INFO )];
		memset( pData, 0, sizeof( NETWORK_ADDR_INFO ) );
		NETWORK_ADDR_INFO *pNetworkAddrInfo = reinterpret_cast<NETWORK_ADDR_INFO *>( pData );
		pNetworkAddrInfo->bDHCP = bUseDHCP;

		GUI::CIPAddressCtrl* pIP = dynamic_cast<GUI::CIPAddressCtrl*>(list.GetItemWnd(ROW_STATIC_IP,COLUMN_DATA));
		assert(pIP);
		pNetworkAddrInfo->staticIP = pIP->GetIP();
		
		pIP = NULL;
		pIP = dynamic_cast<GUI::CIPAddressCtrl*>(list.GetItemWnd(ROW_SUB_NET_MASK,COLUMN_DATA));
		assert(pIP);
		pNetworkAddrInfo->netMask = pIP->GetIP();
	
		pIP = NULL;
		pIP = dynamic_cast<GUI::CIPAddressCtrl*>(list.GetItemWnd(ROW_GATEWAY,COLUMN_DATA));
		assert(pIP);
		pNetworkAddrInfo->gateway = pIP->GetIP();
	
		netCmdRequest = CMD_REQUEST_NETPARAMCHECK_DHCP;
		dataLen = sizeof( NETWORK_ADDR_INFO);
	}
	else if( bUseDHCP )
	{
	//使用动态IP
		pData = new unsigned char [sizeof( NETWORK_ADDR_INFO )];
		memset( pData, 0, sizeof( NETWORK_ADDR_INFO ) );
		NETWORK_ADDR_INFO *pNetworkAddrInfo = reinterpret_cast<NETWORK_ADDR_INFO *>( pData );
		pNetworkAddrInfo->bDHCP = bUseDHCP;

		netCmdRequest = CMD_REQUEST_NETPARAMCHECK_DHCP;
		dataLen = sizeof( NETWORK_ADDR_INFO);
	}
	else if( bUsePppoe )
	{
	//使用PPPOE
		pData = new unsigned char [sizeof( PPPOE_INFO )];
		memset( pData, 0, sizeof( PPPOE_INFO ) );	
		PPPOE_INFO *pPppoeInfo = reinterpret_cast< PPPOE_INFO * >( pData );
		pPppoeInfo->bEnable = bUsePppoe;

		GUI::CEditCtrl* pEdit = dynamic_cast<GUI::CEditCtrl*>(list.GetItemWnd(ROW_PPPOE_USER_NAME,COLUMN_DATA));
		assert(pEdit);
		strcpy( pPppoeInfo->user, pEdit->GetCaption().c_str() );

		pEdit = dynamic_cast<GUI::CEditCtrl*>(list.GetItemWnd(ROW_PPPOE_PASSWORD,COLUMN_DATA));
		assert(pEdit);
		strcpy( pPppoeInfo->password, pEdit->GetCaption().c_str() );

		netCmdRequest = CMD_REQUEST_NETPARAMCHECK_PPPOE;
		dataLen = sizeof( PPPOE_INFO );

	}
	
	//

	MessageOut( GetTopestWnd(), E_STR_ID_PLEASE_WAIT, TIP_ERR );

	CMessageMan::Instance()->SendMsgDataToServer( LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, netCmdRequest, pData, dataLen, msgData );
	
	if( NULL !=  pData )
	{
		delete [] pData;
		pData = NULL;
	}

	assert( msgData.pData );
	PACKCMD *pCmd = reinterpret_cast<PACKCMD *>(msgData.pData);

	switch( static_cast<NET_PROTOCOL>(pCmd->cmdType) )
	{
	case CMD_REPLY_NETPARAMCHECK_DHCP_SUCC:
	case CMD_REPLY_NETPARAMCHECK_PPPOE_SUCC:
		{
			m_StatusIPCheckOK = CHECK_STATUS_OK;
		}
		break;
	case CMD_REPLY_NETPARAMCHECK_DHCP_FAIL:
	case CMD_REPLY_NETPARAMCHECK_PPPOE_FAIL:
		{
			m_StatusIPCheckOK = CHECK_STATUS_FAIL;
		}
		break;
	default:
		assert( 0 );
		m_netCheckResult = NETERR_NETPARAMCHECK_EUNKNOWN;
		break;
	}

	m_netCheckResult = *reinterpret_cast< const NET_CTRL_FAIL* >( msgData.pData + sizeof(PACKCMD) );
	CMessageMan::Instance()->ReleaseMsgBuff(msgData);


	m_bCheckParam = false;
	
	return GetErrorTextNum();
}

unsigned long CCfgNetworkDlg::CheckValue()
{

	//修改IP后保存时的提示		
	switch(m_StatusIPCheckOK)
	{
	case CHECK_STATUS_NOCHANGE:							//网络参数没有改变
	case CHECK_STATUS_OK:								//测试成功
		break;

	case CHECK_STATUS_NOCHECK:							//没有进行测试, 要先测试， 如果通过就会保存
		{

			unsigned long errNum = CheckDhcpParam();

			if( (E_STR_ID_TIP_04_05 == errNum))
			{
				//IP相关信息检测成功
				m_StatusIPCheckOK = CHECK_STATUS_OK;
				errNum = E_STR_ID_NULL;
				break;
			}
			return errNum;
		}
	case CHECK_STATUS_FAIL:								//测试失败		提示：测试失败，请重新设定
		return E_STR_ID_MESSAGE_110;
		break;

	default:
		assert(false);
		break;
	}

	//如果改变了HTTP或者SERVER的端口，需要检查端口是否有效。
	//端口无效的情况为：1两个端口相同 2:使用了系统默认的端口
	if(IsChange(NCFG_ITEM_HTTP_PORT) || IsChange(NCFG_ITEM_SERVER_PORT)) 
	{
		unsigned long httpPort = dynamic_cast<const GUI::CNumEditCtrl*>( m_cfgNetworkList.GetItemWnd(ROW_HTTP_PORT, COLUMN_DATA) )->GetValue();
		unsigned long serverPort = dynamic_cast<const GUI::CNumEditCtrl*>( m_cfgNetworkList.GetItemWnd(ROW_SERVER_PORT, COLUMN_DATA) )->GetValue();
		if( (httpPort == serverPort))
		{
			return E_STR_ID_MESSAGE_28;
		}
	}
	//poe需要检查是不是和非poe在同一个网段内
	{
		if(m_poeNum > 0)
		{
			//if(IsChange(NCFG_ITEM_SWICH_INFO))
			{
				if(!CheckPOEIP())
				{
					return  E_STR_ID_TIP_POEIP_CHECK;
				}
			}

		}				
	}
	return 0;
}

unsigned long CCfgNetworkDlg::PreCheckValue()
{
	if(IsChange(NCFG_ITEM_SWITCH_INFO))
	{
		return E_STR_ID_CHANGE_SWITCHIP;
	}
	
	return 0;
}

