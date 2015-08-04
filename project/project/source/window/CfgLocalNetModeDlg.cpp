#include "CfgLocalNetModeDlg.h"
#include "Button.h"
#include "EditCtrl.h"
#include "CheckCtrl.h"
#include "IPAddressCtrl.h"
#include "CfgNetworkExDlg.h"
CCfgLocalNetModeDlg::CCfgLocalNetModeDlg()
{
	memset(&m_dataInfo,0,sizeof(SETUPDATAINFO));
	m_GetIpMode.bGetIpModeAuto = true;
	m_GetIpMode.bGetIpModePPPOE = true;
}

CCfgLocalNetModeDlg::~CCfgLocalNetModeDlg()
{

}


void CCfgLocalNetModeDlg::OnInitial()
{
	m_style |= WND_NO_TITLE;
	GUI::CDialog::OnInitial();
	//////////////////////////////////////////////////////////////////////////
	unsigned long x = 0,y = 0;
	unsigned long width = m_rect.Width(), height = m_rect.Height();

	GUI::CListCtrl& list = m_listCtrl;
	GUI::CCheckCtrl* pCheck = NULL;
	GUI::CIPAddressCtrl* pIP = NULL;
	GUI::CEditCtrl*	 pEdit = NULL;
	GUI::CButton* pBtn = NULL;

	char str[128] = {0};
	int row =0;
	unsigned short offsetEdge = m_length[SCROLL_WIDTH] + 2;
	if( GUI::CWnd::m_width >= 1024 && GUI::CWnd::m_height >= 768 )
	{
		offsetEdge = 4;
	}

	list.Create(GetRandWndID(),x,y,width,height,this,0,0,false);
	list.AddColumn(NULL,width/2);
	list.AddColumn(NULL,width/2-offsetEdge);

	
	//row = 0;
	list.AddItem( ROW_GETIP_DHCP, 0, m_pStrTable->String(E_STR_ID_IP_AUTO) );
	pCheck = dynamic_cast<GUI::CCheckCtrl*>( list.AddItem( ROW_GETIP_DHCP, 1, WND_TYPE_CHECK ) );
	assert( pCheck );

	// row = 1
	row += 1;
	list.AddItem( ROW_STATIC_IP, 0, m_pStrTable->String(E_STR_ID_IP_ADDRESS) );
	pIP = dynamic_cast<GUI::CIPAddressCtrl*>( list.AddItem( ROW_STATIC_IP, 1, WND_TYPE_IP) );
	assert( pIP );

	// row = 2
	row += 1;
	list.AddItem( ROW_SUB_NET_MASK, 0, m_pStrTable->String( E_STR_ID_NET_MASK ) );
	pIP = dynamic_cast<GUI::CIPAddressCtrl*>( list.AddItem( ROW_SUB_NET_MASK, 1, WND_TYPE_IP) );
	assert( pIP );

	// row = 3
	row += 1;
	list.AddItem( ROW_GATEWAY, 0, m_pStrTable->String( E_STR_ID_GATEWAY ) );
	pIP = dynamic_cast<GUI::CIPAddressCtrl*>( list.AddItem( ROW_GATEWAY, 1, WND_TYPE_IP) );
	assert( pIP );

	// row = 4
	row += 1;
	list.AddItem( ROW_DNS_1, 0, m_pStrTable->String( E_STR_ID_PREFERRED_DNS_SERVER ) );
	pIP = dynamic_cast<GUI::CIPAddressCtrl*>( list.AddItem( ROW_DNS_1, 1, WND_TYPE_IP) );
	assert( pIP );

	// row = 5
	row += 1;
	list.AddItem( ROW_DNS_2, 0, m_pStrTable->String( E_STR_ID_ALTERNATE_DNS_SERVER ) );
	pIP = dynamic_cast<GUI::CIPAddressCtrl*>( list.AddItem( ROW_DNS_2, 1, WND_TYPE_IP) );
	assert( pIP );

	// row = 6
	row += 1;
	list.AddItem( ROW_PPPOE_ENABLE, 0, m_pStrTable->String( E_STR_ID_PPPOE ) );
	pCheck = dynamic_cast<GUI::CCheckCtrl*>( list.AddItem( ROW_PPPOE_ENABLE, 1, WND_TYPE_CHECK ) );
	assert( pCheck );

	// row = 7
	row += 1;
	list.AddItem( ROW_PPPOE_USER_NAME, 0, m_pStrTable->String( E_STR_ID_USER_NAME ) );
	pEdit = dynamic_cast<GUI::CEditCtrl*>( list.AddItem( ROW_PPPOE_USER_NAME, 1, WND_TYPE_EDIT) );
	assert( pEdit );

	// row = 8
	row += 1;
	list.AddItem( ROW_PPPOE_PASSWORD, 0, m_pStrTable->String( E_STR_ID_PASSWORD ) );
	pEdit = dynamic_cast<GUI::CEditCtrl*>( list.AddItem( ROW_PPPOE_PASSWORD, 1, WND_TYPE_EDIT) );
	assert( pEdit );
	pEdit->SetPassword( true );

	// row = 9
	row += 1;
	pBtn = dynamic_cast<GUI::CButton*>(list.AddItem(ROW_NET_TEST,1,WND_TYPE_BUTTON,m_length[BTN_WIDTH],m_length[BTN_HEIGHT]));
	assert(pBtn);
	pBtn->SetCaption(m_pStrTable->String(E_STR_ID_TEST), false);

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

	// end
	list.CreateListFocusMap();
	m_bAuto = false;

}

void CCfgLocalNetModeDlg::OnPaint(PAINT_STATUS status)
{

}

void CCfgLocalNetModeDlg::RefreshView(bool bRefresh)
{
	GUI::CListCtrl& list = m_listCtrl;
	switch(m_dataInfo.cfgID)
	{
		case NCFG_ITEM_NETWORK_ADDR_INFO:
		{
			GUI::CCheckCtrl* pCheck = dynamic_cast<GUI::CCheckCtrl*>( list.GetItemWnd(ROW_GETIP_DHCP, 1) );
			switch ( static_cast<GETIP_MODE>(m_dataInfo.network_info.bDHCP))
			{
				case GETIP_MODE_STATICIP:
					pCheck->SetCheck( false, bRefresh );					
					m_bAuto = false;
					break;
				case GETIP_MODE_DHCP:
					{						
						pCheck->SetCheck( true, bRefresh );

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

						GUI::CIPAddressCtrl* pIP = dynamic_cast<GUI::CIPAddressCtrl*>(list.GetItemWnd(ROW_STATIC_IP,1));
						assert(pIP);
						pIP->SetIP(networkStatusInfo.IP,bRefresh);

						pIP = dynamic_cast<GUI::CIPAddressCtrl*>(list.GetItemWnd(ROW_SUB_NET_MASK,1));
						assert(pIP);
						pIP->SetIP(networkStatusInfo.subMask,bRefresh);

						pIP = dynamic_cast<GUI::CIPAddressCtrl*>(list.GetItemWnd(ROW_GATEWAY,1));
						assert(pIP);
						pIP->SetIP(networkStatusInfo.gateway,bRefresh);

						pIP = dynamic_cast<GUI::CIPAddressCtrl*>( list.GetItemWnd(ROW_DNS_1, 1) );
						assert( pIP );
						pIP->SetIP( networkStatusInfo.DNS1, bRefresh );

						pIP = dynamic_cast<GUI::CIPAddressCtrl*>( list.GetItemWnd(ROW_DNS_2, 1) );
						assert( pIP );
						pIP->SetIP( networkStatusInfo.DNS2, bRefresh );

						m_bAuto = true;

						if (networkStatusInfo.IP  != m_dataInfo.network_info.staticIP || networkStatusInfo.subMask  != m_dataInfo.network_info.netMask || networkStatusInfo.gateway  != m_dataInfo.network_info.gateway)
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
							pData=NULL;
						}
						CheckEnableUI(bRefresh, REFRASH_UI_AUTOIP);
						return;						
					}
					break;					
				default:
					assert(false);
			}
			GUI::CIPAddressCtrl* pIP = dynamic_cast<GUI::CIPAddressCtrl*>(list.GetItemWnd(ROW_STATIC_IP,1));
			assert(pIP);
			pIP->SetIP(m_dataInfo.network_info.staticIP,bRefresh);

			pIP = dynamic_cast<GUI::CIPAddressCtrl*>(list.GetItemWnd(ROW_SUB_NET_MASK,1));
			assert(pIP);
			pIP->SetIP(m_dataInfo.network_info.netMask,bRefresh);

			pIP = dynamic_cast<GUI::CIPAddressCtrl*>(list.GetItemWnd(ROW_GATEWAY,1));
			assert(pIP);
			pIP->SetIP(m_dataInfo.network_info.gateway,bRefresh);

			CheckEnableUI(bRefresh, REFRASH_UI_AUTOIP);
			break;

		}
		case NCFG_ITEM_DNS_1:
		{			
			if (m_bAuto)
			{
				return;
			}
			GUI::CIPAddressCtrl* pIp = dynamic_cast<GUI::CIPAddressCtrl*>( list.GetItemWnd(ROW_DNS_1, 1) );
			pIp->SetIP( m_dataInfo.pUlong1, bRefresh );
			break;
		}
		case NCFG_ITEM_DNS_2:
		{
			if (m_bAuto)
			{
				return;
			}
			GUI::CIPAddressCtrl* pIp = dynamic_cast<GUI::CIPAddressCtrl*>( list.GetItemWnd(ROW_DNS_2, 1) );
			assert( pIp );
			pIp->SetIP( m_dataInfo.pUlong2, bRefresh );
			break;
		}
		case NCFG_ITEM_PPPOE_INFO:
		{
			GUI::CCheckCtrl* pCheck = dynamic_cast<GUI::CCheckCtrl*>( list.GetItemWnd(ROW_PPPOE_ENABLE, 1) );
			assert( pCheck );
			pCheck->SetCheck(m_dataInfo.pPPPOE.bEnable, bRefresh );

			GUI::CEditCtrl* pEdit = dynamic_cast<GUI::CEditCtrl*>(list.GetItemWnd(ROW_PPPOE_USER_NAME,1));
			assert(pEdit);
			pEdit->SetMaxStringLen(m_dataInfo.maxUserLen);
			pEdit->SetCaption(m_dataInfo.pPPPOE.user,bRefresh);

			pEdit = dynamic_cast<GUI::CEditCtrl*>(list.GetItemWnd(ROW_PPPOE_PASSWORD,1));
			assert(pEdit);
			pEdit->SetMaxStringLen(m_dataInfo.maxPassLen);
			pEdit->SetCaption(m_dataInfo.pPPPOE.password,bRefresh);

			CheckEnableUI(bRefresh, REFRASH_UI_PPPOE);
			break;
		}
		case NCFG_ITEM_SWITCH_INFO:
		{
			if(m_poeNum > 0)
			{
				//assert(len == sizeof(NET_SWITCH_INFO));
				//const NET_SWITCH_INFO *pInfo =  reinterpret_cast<const NET_SWITCH_INFO*>(pData);
				GUI::CIPAddressCtrl *piP = dynamic_cast<GUI::CIPAddressCtrl*>( list.GetItemWnd(ROW_POESWITCHIP, 1) );
				assert(piP);
				piP -> SetIP(m_dataInfo.switchIP,bRefresh);
				piP = dynamic_cast<GUI::CIPAddressCtrl*>( list.GetItemWnd(ROW_POESWITCH_SUBNETMASK, 1) );
				assert(piP);
				piP -> SetIP(m_dataInfo.netMask,bRefresh);
			}
		}
		break;
		default:
			break;

	}

}

void CCfgLocalNetModeDlg::CheckEnableUI( bool bRefresh, REFRASH_UI_NUM uiNum )
{

	GUI::CCheckCtrl* pCheck = NULL;

	switch( uiNum )
	{
	case REFRASH_UI_AUTOIP:
		pCheck = dynamic_cast<GUI::CCheckCtrl*>(m_listCtrl.GetItemWnd( ROW_GETIP_DHCP, 1 ));
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
		pCheck = dynamic_cast<GUI::CCheckCtrl*>(m_listCtrl.GetItemWnd( ROW_PPPOE_ENABLE, 1 ));
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

	default:
		assert( false ); 
		break;
	}

	DoUpdateUI( m_GetIpMode , bRefresh , uiNum );
}
void CCfgLocalNetModeDlg::DoUpdateUI( GET_IP_MODE& getIpMode, bool bRefresh,REFRASH_UI_NUM uiNum)
{
	switch( uiNum )
	{
	case REFRASH_UI_AUTOIP:
	case REFRASH_UI_PPPOE:

		//静态IP和DNS
		m_listCtrl.GetItemWnd(ROW_STATIC_IP,1)->SetEnable((!m_GetIpMode.bGetIpModeAuto)&&(!m_GetIpMode.bGetIpModePPPOE),bRefresh);
		m_listCtrl.GetItemWnd(ROW_SUB_NET_MASK,1)->SetEnable((!m_GetIpMode.bGetIpModeAuto)&&(!m_GetIpMode.bGetIpModePPPOE),bRefresh);
		m_listCtrl.GetItemWnd(ROW_GATEWAY,1)->SetEnable((!m_GetIpMode.bGetIpModeAuto)&&(!m_GetIpMode.bGetIpModePPPOE),bRefresh);
		m_listCtrl.GetItemWnd(ROW_DNS_1,1)->SetEnable((!m_GetIpMode.bGetIpModeAuto)&&(!m_GetIpMode.bGetIpModePPPOE),bRefresh);
		m_listCtrl.GetItemWnd(ROW_DNS_2,1)->SetEnable((!m_GetIpMode.bGetIpModeAuto)&&(!m_GetIpMode.bGetIpModePPPOE),bRefresh);

		//PPPOE
		m_listCtrl.GetItemWnd(ROW_PPPOE_USER_NAME,1)->SetEnable(m_GetIpMode.bGetIpModePPPOE,bRefresh);
		m_listCtrl.GetItemWnd(ROW_PPPOE_PASSWORD,1)->SetEnable(m_GetIpMode.bGetIpModePPPOE,bRefresh);

		//check
		dynamic_cast<GUI::CCheckCtrl*>(m_listCtrl.GetItemWnd(ROW_GETIP_DHCP,1))->SetCheck(m_GetIpMode.bGetIpModeAuto,bRefresh);
		dynamic_cast<GUI::CCheckCtrl*>(m_listCtrl.GetItemWnd(ROW_PPPOE_ENABLE,1))->SetCheck(m_GetIpMode.bGetIpModePPPOE,bRefresh);
		break;
	default:
		assert( false ); 
		break;
	}
}
void CCfgLocalNetModeDlg::OnModifyNetModeData(unsigned long cfgId)
{
	CCfgNetworkExDlg *pcfgNetWorkExDlg = dynamic_cast<CCfgNetworkExDlg*>(GetParent());
	assert(pcfgNetWorkExDlg);//
	pcfgNetWorkExDlg->SetDataInfo(m_dataInfo);
	pcfgNetWorkExDlg->OnModifyNetModeData(cfgId);
	
}

unsigned long CCfgLocalNetModeDlg::GetErrorTextNum()
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



unsigned long CCfgLocalNetModeDlg::CheckDhcpParam()
{
	unsigned char *pData = NULL;
	int dataLen = 0;
	NET_PROTOCOL netCmdRequest;
	MESSAGE_DATA msgData;
	memset( &msgData, 0, sizeof( msgData ) );

	GUI::CListCtrl& list = m_listCtrl;


	m_bCheckParam = true;								//设置该标记，屏蔽外部输入

	//check 标志
	GUI::CCheckCtrl* pCheck = dynamic_cast<GUI::CCheckCtrl*>( list.GetItemWnd( ROW_GETIP_DHCP, 1) );
	bool bUseDHCP = pCheck->IsCheck();
	pCheck = dynamic_cast<GUI::CCheckCtrl*>( list.GetItemWnd( ROW_PPPOE_ENABLE, 1) );
	bool bUsePppoe = pCheck->IsCheck();

	if( ( !bUseDHCP ) && ( !bUsePppoe ) )
	{
		//使用静态IP
		pData = new unsigned char [sizeof( NETWORK_ADDR_INFO )];
		memset( pData, 0, sizeof( NETWORK_ADDR_INFO ) );
		NETWORK_ADDR_INFO *pNetworkAddrInfo = reinterpret_cast<NETWORK_ADDR_INFO *>( pData );
		pNetworkAddrInfo->bDHCP = bUseDHCP;

		GUI::CIPAddressCtrl* pIP = dynamic_cast<GUI::CIPAddressCtrl*>(list.GetItemWnd(ROW_STATIC_IP,1));
		assert(pIP);
		pNetworkAddrInfo->staticIP = pIP->GetIP();

		pIP = NULL;
		pIP = dynamic_cast<GUI::CIPAddressCtrl*>(list.GetItemWnd(ROW_SUB_NET_MASK,1));
		assert(pIP);
		pNetworkAddrInfo->netMask = pIP->GetIP();

		pIP = NULL;
		pIP = dynamic_cast<GUI::CIPAddressCtrl*>(list.GetItemWnd(ROW_GATEWAY,1));
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

		GUI::CEditCtrl* pEdit = dynamic_cast<GUI::CEditCtrl*>(list.GetItemWnd(ROW_PPPOE_USER_NAME,1));
		assert(pEdit);
		strcpy( pPppoeInfo->user, pEdit->GetCaption().c_str() );

		pEdit = dynamic_cast<GUI::CEditCtrl*>(list.GetItemWnd(ROW_PPPOE_PASSWORD,1));
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

unsigned long CCfgLocalNetModeDlg::CheckValue()
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
	return 0;
}

BEGIN_MSG_MAP(CCfgLocalNetModeDlg, GUI::CDialog)
	ON_MSG_EX(m_listCtrl.GetID(), KEY_VIRTUAL_MSG_DEFAULT_TAG, OnClickList)
END_MSG_MAP()

unsigned long CCfgLocalNetModeDlg::OnClickList(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	GUI::CListCtrl& list = m_listCtrl; 
	unsigned short row = (keyID >> 16)&0x7fff;	//行
	unsigned short col = keyID&0xffff;			//列

	// 是否使用自动分配IP
	if( ( ROW_GETIP_DHCP == row && 1 == col) )
	{
		CheckEnableUI( true , REFRASH_UI_AUTOIP );
	}

	// 是否启用PPPOE
	if( ROW_PPPOE_ENABLE == row && 1 == col )
	{
		CheckEnableUI( true, REFRASH_UI_PPPOE );
	}

	//测试网络
	if( ROW_NET_TEST == row && 1 == col )
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
	case ROW_POESWITCHIP:
	case ROW_POESWITCH_SUBNETMASK:
		m_StatusIPCheckOK = CHECK_STATUS_NOCHECK;
		break;
	default:
		break;
	}

	switch (row)
	{
	case ROW_GETIP_DHCP:
		{
			m_dataInfo.cfgID = NCFG_ITEM_NETWORK_ADDR_INFO;
			const GUI::CCheckCtrl* pCheck = dynamic_cast<const GUI::CCheckCtrl*>( list.GetItemWnd(ROW_GETIP_DHCP, 1) );
			assert( pCheck );
			if( pCheck->IsCheck() )
			{
				m_dataInfo.network_info.bDHCP = 1;//true
			}
			else
			{
				m_dataInfo.network_info.bDHCP = 0;//false
			}
			OnModifyNetModeData(NCFG_ITEM_NETWORK_ADDR_INFO);
			return KEY_VIRTUAL_MSG_CONTINUE;
			break;
		}
	case ROW_STATIC_IP:
		{
			m_dataInfo.cfgID = NCFG_ITEM_NETWORK_ADDR_INFO;
			const GUI::CIPAddressCtrl* pIP = dynamic_cast<const GUI::CIPAddressCtrl*>(list.GetItemWnd(ROW_STATIC_IP,1));
			assert(pIP);
			m_dataInfo.network_info.staticIP = pIP->GetIP();
			OnModifyNetModeData(NCFG_ITEM_NETWORK_ADDR_INFO);
			return KEY_VIRTUAL_MSG_CONTINUE;
			break;
		}
	case ROW_SUB_NET_MASK:
		{
			m_dataInfo.cfgID = NCFG_ITEM_NETWORK_ADDR_INFO;
			const GUI::CIPAddressCtrl* pIP = dynamic_cast<const GUI::CIPAddressCtrl*>(list.GetItemWnd(ROW_SUB_NET_MASK,1));
			assert(pIP);
			m_dataInfo.network_info.netMask = pIP->GetIP();
			OnModifyNetModeData(NCFG_ITEM_NETWORK_ADDR_INFO);
			return KEY_VIRTUAL_MSG_CONTINUE;
			break;
		}
	case ROW_GATEWAY:
		{
			m_dataInfo.cfgID = NCFG_ITEM_NETWORK_ADDR_INFO;
			const GUI::CIPAddressCtrl* pIP = dynamic_cast<const GUI::CIPAddressCtrl*>(list.GetItemWnd(ROW_GATEWAY,1));
			assert(pIP);
			m_dataInfo.network_info.gateway = pIP->GetIP();
			OnModifyNetModeData(NCFG_ITEM_NETWORK_ADDR_INFO);
			return KEY_VIRTUAL_MSG_CONTINUE;
			break;
		}
	case ROW_DNS_1:
		{
			m_dataInfo.cfgID = NCFG_ITEM_DNS_1;
			const GUI::CIPAddressCtrl* pIp = dynamic_cast<const GUI::CIPAddressCtrl*>( list.GetItemWnd(ROW_DNS_1, 1) );
			assert( pIp );
			m_dataInfo.pUlong1 = pIp->GetIP();
			OnModifyNetModeData(NCFG_ITEM_DNS_1);
			return KEY_VIRTUAL_MSG_CONTINUE;
			break;
		}
	case ROW_DNS_2:
		{
			m_dataInfo.cfgID = NCFG_ITEM_DNS_2;
			const GUI::CIPAddressCtrl* pIp = dynamic_cast<const GUI::CIPAddressCtrl*>( list.GetItemWnd(ROW_DNS_2, 1) );
			assert( pIp );
			m_dataInfo.pUlong2 = pIp->GetIP();
			OnModifyNetModeData(NCFG_ITEM_DNS_2);
			return KEY_VIRTUAL_MSG_CONTINUE;
			break;
		}
	case ROW_PPPOE_ENABLE:
		{
			m_dataInfo.cfgID = NCFG_ITEM_PPPOE_INFO;
			const GUI::CCheckCtrl* pCheck = dynamic_cast<const GUI::CCheckCtrl*>( list.GetItemWnd(ROW_PPPOE_ENABLE, 1) );
			assert( pCheck );
			if( pCheck->IsCheck() )
			{
				m_dataInfo.pPPPOE.bEnable = 1;
			}
			else
			{
				m_dataInfo.pPPPOE.bEnable = 0;
			}
			OnModifyNetModeData(NCFG_ITEM_PPPOE_INFO);
			return KEY_VIRTUAL_MSG_CONTINUE;
			break;
		}
	case ROW_PPPOE_USER_NAME:
		{
			m_dataInfo.cfgID = NCFG_ITEM_PPPOE_INFO;
			const GUI::CEditCtrl* pEditUser = dynamic_cast<const GUI::CEditCtrl*>(list.GetItemWnd(ROW_PPPOE_USER_NAME,1));
			assert(pEditUser);
			strcpy(m_dataInfo.pPPPOE.user,pEditUser->GetCaption().c_str());
			OnModifyNetModeData(NCFG_ITEM_PPPOE_INFO);
			return KEY_VIRTUAL_MSG_CONTINUE;
			break;
		}
	case ROW_PPPOE_PASSWORD:
		{	
			m_dataInfo.cfgID = NCFG_ITEM_PPPOE_INFO;
			const GUI::CEditCtrl* pEditPass = dynamic_cast<const GUI::CEditCtrl*>(list.GetItemWnd(ROW_PPPOE_PASSWORD,1));
			assert(pEditPass);
			strcpy(m_dataInfo.pPPPOE.password,pEditPass->GetCaption().c_str());

			OnModifyNetModeData(NCFG_ITEM_PPPOE_INFO);
			return KEY_VIRTUAL_MSG_CONTINUE;
			break;
		}
	case ROW_POESWITCHIP:
		{
			m_dataInfo.cfgID = NCFG_ITEM_SWITCH_INFO;
			const GUI::CIPAddressCtrl* pEditSwitch = dynamic_cast<const GUI::CIPAddressCtrl*>(list.GetItemWnd(ROW_POESWITCHIP,1));
			m_dataInfo.switchIP=pEditSwitch->GetIP();
			OnModifyNetModeData(NCFG_ITEM_SWITCH_INFO);
			return KEY_VIRTUAL_MSG_CONTINUE;
			break;
		}
	case ROW_POESWITCH_SUBNETMASK:
		{
			const GUI::CIPAddressCtrl* pEditSwitchMask = dynamic_cast<const GUI::CIPAddressCtrl*>(list.GetItemWnd(ROW_POESWITCH_SUBNETMASK,1));
			m_dataInfo.netMask=pEditSwitchMask->GetIP();
			OnModifyNetModeData(NCFG_ITEM_SWITCH_INFO);
			return KEY_VIRTUAL_MSG_CONTINUE;
			break;
		}
	default:
		break;
	}
	
	return KEY_VIRTUAL_MSG_STOP;

}
