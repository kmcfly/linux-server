/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : zxx
** Date         : 2010-07-20
** Name         : WzdTipNetDlg.cpp
** Version      : 1.0
** Description  : 向导对话框
** Modify Record:
***********************************************************************/

#include "WzdTipNetDlg.h"
#include "NetProtocol.h"
#include "MessageMan.h"

CWzdTipNetDlg::CWzdTipNetDlg() : m_pWzdInfo(NULL),m_bUsed(true)
{
	m_bIpIsValid = false;
}

CWzdTipNetDlg::~CWzdTipNetDlg()
{

}

void CWzdTipNetDlg::SetUsed(bool bUsed)
{
	m_bUsed = bUsed;
}

bool CWzdTipNetDlg::IsUsed()
{
	return m_bUsed;
}

void CWzdTipNetDlg::SetWzdInfo(WIZARD_INFO* pWzdInfo)
{
	m_pWzdInfo = pWzdInfo;
}

void CWzdTipNetDlg::OnInitial()
{

	GUI::CListCtrl& list = NetSetList;
	GUI::CWnd * pWnd = NULL;
	GUI::CCheckCtrl* pCheck = NULL;
	GUI::CEditCtrl*	 pEdit = NULL;
	GUI::CNumEditCtrl* pNumEdit = NULL;
	GUI::CIPAddressCtrl* pIP = NULL;
	GUI::CButton* pBtn = NULL;

	unsigned short x = m_length[DLG_OFFSET_SUB_LEFT], y = m_length[DLG_OFFSET_SUB_TOP];
	unsigned short width = m_rect.Width() - 2 * m_length[DLG_OFFSET_SUB_LEFT];
	unsigned short height = m_rect.Height() - y - m_length[DLG_OFFSET_SUB_BOTTOM];
	unsigned short offsetEdge = 0;
	int maxListRow = height/m_length[LIST_ITEM_HEIGHT]; //List框不带头
	list.Create(GUI::CWnd::GetRandWndID(), x, y, width, height, this, 0, 0, false);
	if( ROW_NUM > maxListRow )
	{
		offsetEdge = m_length[SLIDER_WIDTH] + 2;
	}
	list.AddColumn(NULL, width * 16 / 32);
	list.AddColumn(NULL, width * 16 / 32 );
	char str[128] = {0};
	int row = 0;

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
	pNumEdit->SetRange(0,(int)65535);

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

	#if !defined(__SUPPORT_3G__) && !defined(__TW01_RILI__)
	row += 2;
	m_netTestButton = dynamic_cast<GUI::CButton*>(list.AddItem(ROW_NET_TEST,1,WND_TYPE_BUTTON,m_length[BTN_WIDTH],m_length[BTN_HEIGHT]));
	m_netTestButton->SetCaption(m_pStrTable->String(E_STR_ID_TEST), false);

	#endif
	list.CreateListFocusMap();
}

void CWzdTipNetDlg::InitCtrlInfo()
{
	GUI::CListCtrl& list = NetSetList;
	GUI::CNumEditCtrl* pEdit = dynamic_cast<GUI::CNumEditCtrl*>( list.GetItemWnd(ROW_HTTP_PORT, 1) );
	pEdit->SetValue(m_pWzdInfo->Port,false);
	pEdit = dynamic_cast<GUI::CNumEditCtrl*>( list.GetItemWnd(ROW_SERVER_PORT, 1) );
	pEdit->SetValue(m_pWzdInfo->ServerPort,false);
	GUI::CCheckCtrl* pCheck=dynamic_cast<GUI::CCheckCtrl*>( list.GetItemWnd(ROW_GETIP_DHCP, 1) );
	if(m_pWzdInfo->Addr_Info.bDHCP==GETIP_MODE_STATICIP)
		pCheck->SetCheck( false, false );
	else if(m_pWzdInfo->Addr_Info.bDHCP==GETIP_MODE_DHCP) 
		pCheck->SetCheck( true, false );
	GUI::CIPAddressCtrl* pIP = dynamic_cast<GUI::CIPAddressCtrl*>(list.GetItemWnd(ROW_STATIC_IP,1));
	assert(pIP);
	pIP->SetIP(m_pWzdInfo->Addr_Info.staticIP,false);

	pIP = dynamic_cast<GUI::CIPAddressCtrl*>(list.GetItemWnd(ROW_SUB_NET_MASK,1));
	assert(pIP);
	pIP->SetIP(m_pWzdInfo->Addr_Info.netMask,false);

	pIP = dynamic_cast<GUI::CIPAddressCtrl*>(list.GetItemWnd(ROW_GATEWAY,1));
	assert(pIP);
	pIP->SetIP(m_pWzdInfo->Addr_Info.gateway,false);
	pIP=dynamic_cast<GUI::CIPAddressCtrl*>( list.GetItemWnd(ROW_DNS_1, 1) );
	pIP->SetIP(m_pWzdInfo->DNS_MainIP,false);
	pIP=dynamic_cast<GUI::CIPAddressCtrl*>( list.GetItemWnd(ROW_DNS_2, 1) );
	pIP->SetIP(m_pWzdInfo->DNS_SubIP,false);
	//私有成员初始化
	m_port=m_pWzdInfo->Port;
	m_serverPort=m_pWzdInfo->ServerPort;
	m_serverPort=m_pWzdInfo->ServerPort;

	m_addr_Info=m_pWzdInfo->Addr_Info;
	m_DNS_MainIP=m_pWzdInfo->DNS_MainIP;
	m_DNS_SubIP=m_pWzdInfo->DNS_SubIP;		
	
}

void CWzdTipNetDlg::ShowWzdData(bool bRefresh)
{
	CheckEnableUI(bRefresh);

}

void CWzdTipNetDlg::GetWzdData(bool bNext)
{
	m_pWzdInfo->Port=m_port;
	m_pWzdInfo->ServerPort=m_serverPort;
	m_pWzdInfo->ServerPort=m_serverPort;

	m_pWzdInfo->Addr_Info=m_addr_Info;
	m_pWzdInfo->DNS_MainIP=m_DNS_MainIP;
	m_pWzdInfo->DNS_SubIP=m_DNS_SubIP;
	
}



BEGIN_MSG_MAP(CWzdTipNetDlg, CWzdPageDlg)
	ON_MSG_EX(NetSetList.GetID(), KEY_VIRTUAL_MSG_DEFAULT_TAG, OnClickList)
END_MSG_MAP()

unsigned long CWzdTipNetDlg::OnClickList(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	//判断自动获取ip复选框是不是被选中，被选中不允许修改选项
	if (IsUsed())
	{
		unsigned short row = (keyID >> 16)&0x7fff;	//行
		unsigned short col = keyID&0xffff;			//列
		GUI::CListCtrl& list = NetSetList;
		
		switch (row)
		{
			case ROW_HTTP_PORT:
				{
					GUI::CNumEditCtrl* pEdit = dynamic_cast<GUI::CNumEditCtrl*>( list.GetItemWnd(ROW_HTTP_PORT, 1) );
					assert(pEdit!=NULL);
					m_port=pEdit->GetValue();
					//m_pWzdInfo->Modified=1;
				}
				return true;
				break;
			case ROW_SERVER_PORT:
				{
					
					GUI::CNumEditCtrl* pEdit = dynamic_cast<GUI::CNumEditCtrl*>( list.GetItemWnd(ROW_SERVER_PORT, 1) );
					assert(pEdit!=NULL);
					m_serverPort=pEdit->GetValue();
					//m_pWzdInfo->Modified=1;
				}
				return true;
				break;
		case ROW_GETIP_DHCP:
				{
					const GUI::CCheckCtrl* pCheck = dynamic_cast<const GUI::CCheckCtrl*>( list.GetItemWnd(ROW_GETIP_DHCP, 1) );
					assert(pCheck!=NULL);
					if( pCheck->IsCheck() )
					{
						//m_pWzdInfo->Addr_Info.bDHCP = true;
						m_addr_Info.bDHCP = true;
					}
					else
					{
						//m_pWzdInfo->Addr_Info.bDHCP = false;
						m_addr_Info.bDHCP = false;
					}
					CheckEnableUI(false);
					//m_pWzdInfo->Modified=1;
				}
				return true;
				break;
		case ROW_STATIC_IP:
			{
				GUI::CIPAddressCtrl* pIP = dynamic_cast<GUI::CIPAddressCtrl*>(list.GetItemWnd(ROW_STATIC_IP,1));
				//m_pWzdInfo->Addr_Info.staticIP=pIP->GetIP();
				//m_pWzdInfo->Modified=1;
				m_addr_Info.staticIP=pIP->GetIP();
			}
			return true;
			break;
		case ROW_SUB_NET_MASK:
			{
				GUI::CIPAddressCtrl* pIP = dynamic_cast<GUI::CIPAddressCtrl*>(list.GetItemWnd(ROW_SUB_NET_MASK,1));
				/*m_pWzdInfo->Addr_Info.netMask=pIP->GetIP();
				m_pWzdInfo->Modified=1;*/
				m_addr_Info.netMask=pIP->GetIP();
			}
			return true;
			break;
		case ROW_GATEWAY:
			{
				GUI::CIPAddressCtrl* pIP = dynamic_cast<GUI::CIPAddressCtrl*>(list.GetItemWnd(ROW_GATEWAY,1));
				/*m_pWzdInfo->Addr_Info.gateway=pIP->GetIP();
				m_pWzdInfo->Modified=1;*/
				m_addr_Info.gateway=pIP->GetIP();
			}
			return true;
			break;
		case ROW_DNS_1:
			{
				GUI::CIPAddressCtrl* pIP = dynamic_cast<GUI::CIPAddressCtrl*>(list.GetItemWnd(ROW_DNS_1,1));
				//m_pWzdInfo->DNS_MainIP=pIP->GetIP();
				//m_pWzdInfo->Modified=1;
				m_DNS_MainIP=pIP->GetIP();
			}
			return true;
			break;
		case ROW_DNS_2:
			{
				GUI::CIPAddressCtrl* pIP = dynamic_cast<GUI::CIPAddressCtrl*>(list.GetItemWnd(ROW_DNS_2,1));
				//m_pWzdInfo->DNS_SubIP=pIP->GetIP();
				//m_pWzdInfo->Modified=1;
				m_DNS_SubIP=pIP->GetIP();
			}
			return true;
			break;
		case ROW_NET_TEST:
			{	
				MessageOut( GetTopestWnd(), E_STR_ID_PLEASE_WAIT, TIP_ERR );			
				m_bIpIsValid = true;
				m_bIpIsValid = CheckIPIfConflict();
				if(m_bIpIsValid)
				{
					MessageOut( GetTopestWnd(),static_cast<ENUM_STRING_ID>(E_STR_ID_TIP_04_05), TIP_ERR, true );
				}
				else
				{
					MessageOut( GetTopestWnd(),static_cast<ENUM_STRING_ID>(GetErrorTextNum()), TIP_ERR, true );
				}
				GetTopestWnd()->Repaint();
			}
			return true;
			break;
			default:
				return false;
				break;

		} 
		return KEY_VIRTUAL_MSG_CONTINUE;
	}

	else
	{
		return KEY_VIRTUAL_MSG_STOP;
	}

}

void CWzdTipNetDlg::CheckEnableUI(bool bRefresh)
{
	GUI::CCheckCtrl* pCheck = NULL;
	GUI::CListCtrl& list = NetSetList;
	pCheck = dynamic_cast<GUI::CCheckCtrl*>(list.GetItemWnd( ROW_GETIP_DHCP, 1 ));
	m_getIpModeAuto = pCheck->IsCheck();
	DoUpdateUI(bRefresh);
}

void CWzdTipNetDlg::DoUpdateUI(bool bRefresh)
{
	//静态IP和DNS
	NetSetList.GetItemWnd(ROW_STATIC_IP,1)->SetEnable(!m_getIpModeAuto,bRefresh);
	NetSetList.GetItemWnd(ROW_SUB_NET_MASK,1)->SetEnable(!m_getIpModeAuto,bRefresh);
	NetSetList.GetItemWnd(ROW_GATEWAY,1)->SetEnable(!m_getIpModeAuto,bRefresh);
	NetSetList.GetItemWnd(ROW_DNS_1,1)->SetEnable(!m_getIpModeAuto,bRefresh);
	NetSetList.GetItemWnd(ROW_DNS_2,1)->SetEnable(!m_getIpModeAuto,bRefresh);
		
}
bool CWzdTipNetDlg::CheckIPIfConflict()
{
	GUI::CListCtrl& list = NetSetList;
	bool bResult= false;
	unsigned char *pData = NULL;
	int dataLen = 0;
	NET_PROTOCOL netCmdRequest;
	MESSAGE_DATA msgData;

	memset( &msgData, 0, sizeof( msgData ) );

	m_bIpIsValid = true;

	const GUI::CCheckCtrl* pCheck = dynamic_cast<const GUI::CCheckCtrl*>( list.GetItemWnd(ROW_GETIP_DHCP, 1) );
	assert(pCheck!=NULL);
	if( pCheck->IsCheck() )
	{
		m_addr_Info.bDHCP = true;
		return true;
	}
	else
	{
		m_addr_Info.bDHCP = false;
	}

	//使用静态IP
	pData = new unsigned char [sizeof( NETWORK_ADDR_INFO )];
	memset( pData, 0, sizeof( NETWORK_ADDR_INFO ) );
	NETWORK_ADDR_INFO *pNetworkAddrInfo = reinterpret_cast<NETWORK_ADDR_INFO *>( pData );

	GUI::CIPAddressCtrl* pIP = dynamic_cast<GUI::CIPAddressCtrl*>(list.GetItemWnd(ROW_STATIC_IP,1));

	pNetworkAddrInfo->staticIP=pIP->GetIP();

	pIP = dynamic_cast<GUI::CIPAddressCtrl*>(list.GetItemWnd(ROW_SUB_NET_MASK,1));
	pNetworkAddrInfo->netMask=pIP->GetIP();

	pIP = dynamic_cast<GUI::CIPAddressCtrl*>(list.GetItemWnd(ROW_GATEWAY,1));
	pNetworkAddrInfo->gateway=pIP->GetIP();

	netCmdRequest = CMD_REQUEST_NETPARAMCHECK_DHCP;
	dataLen = sizeof( NETWORK_ADDR_INFO);


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
		bResult = true;
		break;
	default:
		bResult = false;
		break;
	}
	
	m_netCheckResult = *reinterpret_cast< const NET_CTRL_FAIL* >( msgData.pData + sizeof(PACKCMD) );
	
	CMessageMan::Instance()->ReleaseMsgBuff(msgData);

	return bResult;

}
unsigned long CWzdTipNetDlg::GetErrorTextNum()
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

void CWzdTipNetDlg::SetTip()
{
	GUI::CWnd* pWnd = NetSetList.GetItemWnd(ROW_NET_TEST, 1);
	if( NULL != pWnd )
	{
		AddTip(pWnd->GetID(), E_STR_ID_TIP_05_01);
	}
	else
	{
		printf("%s:%s:%d, GetItemWnd() error\n", __FUNCTION__, __FILE__, __LINE__);
	}
}

