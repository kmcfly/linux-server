#include "SetupIPCNetworkDlg.h"
#include "Product.h"
#include "RegNetDeviceLog.h"
#include "NetProtocol.h"
#include "IPDataBufferDefine.h"
#include "dvrdvsdefine.h"
#include "NdLibMan.h"
#include "MessageMan.h"
#include "Base64Coder.h"

#ifndef __ENVIRONMENT_WIN32__
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif

CSetupIpcNetworkDlg::CSetupIpcNetworkDlg(DISCOVER_DEVICE_INFO & ipcDeviceInfo)
{
	m_ipcDeviceInfo = ipcDeviceInfo;
	m_netdeviceInfo.curIpAddr = inet_addr(m_ipcDeviceInfo.szAddr);
	m_netdeviceInfo.newIpaddr = inet_addr(m_ipcDeviceInfo.szAddr);
	m_netdeviceInfo.newNetmask = m_ipcDeviceInfo.netmask;
	m_netdeviceInfo.newGateway = m_ipcDeviceInfo.gateway;
	memcpy(m_netdeviceInfo.szID, ipcDeviceInfo.szID, sizeof(ipcDeviceInfo.szID));
	
#if defined(__CUSTOM_PL14__)
	snprintf(m_netdeviceInfo.userName, sizeof(m_netdeviceInfo.userName), "root");
#else
	snprintf(m_netdeviceInfo.userName, sizeof(m_netdeviceInfo.userName), "admin");
#endif
	memset(m_netdeviceInfo.password, 0, sizeof(m_netdeviceInfo.password));
}

CSetupIpcNetworkDlg::~CSetupIpcNetworkDlg()
{

}


extern NET_LOGIN_SUCCESS_REPLY *g_p_login_succ;
//////////////////////////////////////////////////////////////////////////

void CSetupIpcNetworkDlg::OnInitial()
{
	GUI::CDialog::OnInitial();
	SetImage(BMP_MEM_DLG_LARGE, false);

	char str[256] = {0};
	snprintf(str, 256, "%s", m_pStrTable->String(E_STR_ID_SETUP));
	SetCaption(str, false);

	SetDefaultBtn( DEFAULT_BTN_OK | DEFAULT_BTN_CANCEL );
	//////////////////////////////////////////////////////////////////////////
	GUI::CListCtrl& list = m_listCtrl;
	GUI::CIPAddressCtrl* pIP = NULL;
	GUI::CIPAddressCtrl* pNetmask = NULL;
	GUI::CIPAddressCtrl* pGateway = NULL;

	GUI::CEditCtrl* pEditUserName = NULL;
	GUI::CEditCtrl* pEditPassword = NULL;

	unsigned short x = m_length[DLG_OFFSET_LEFT], y = m_length[TITLE_HEIGHT] + m_length[DLG_OFFSET_TOP];
	unsigned short width = m_rect.Width() - 2 * m_length[DLG_OFFSET_LEFT];
	unsigned short height = m_rect.Height() - y - m_length[DEFAULT_BTN_BOTTOM_YPOS] - m_length[DLG_OFFSET_BOTTOM];

	list.Create( GUI::CWnd::GetRandWndID(), x, y, width, height, this, 0, 1, false );
	list.AddColumn( NULL , width / 2 );
	list.AddColumn( NULL , width / 2 - 2 );

	int row = 0;

	//////////////////////////////////////////////////////////////////////////
	// row = 0 IP地址
	row = ROW_IP_ADDRESS;
	list.AddItem( row, 0, m_pStrTable->String(E_STR_ID_IP));
	pIP = dynamic_cast<GUI::CIPAddressCtrl*>(list.AddItem(row, 1, WND_TYPE_IP));
	assert(pIP);
	pIP->SetIP(m_netdeviceInfo.newIpaddr, false);

	//////////////////////////////////////////////////////////////////////////
	// row = 1 子网掩码
	row = ROW_NETMASK;
	list.AddItem(row, 0, m_pStrTable->String(E_STR_ID_NET_MASK));
	pNetmask = dynamic_cast<GUI::CIPAddressCtrl*>(list.AddItem(row , 1, WND_TYPE_IP));
	assert( pNetmask );
	pNetmask->SetIP(m_netdeviceInfo.newNetmask, false);

	//////////////////////////////////////////////////////////////////////////
	// row = 2 网关
	row = ROW_GATEWAY;
	list.AddItem(row, 0, m_pStrTable->String(E_STR_ID_GATEWAY));
	pGateway = dynamic_cast<GUI::CIPAddressCtrl*>( list.AddItem(row, 1, WND_TYPE_IP));
	assert(pGateway);
	pGateway->SetIP(m_netdeviceInfo.newGateway, false);

	//////////////////////////////////////////////////////////////////////////
	// row = 3 用户名
	row = ROW_USER_NAME;
	list.AddItem( row, 0, m_pStrTable->String(E_STR_ID_USER_NAME));
	pEditUserName = dynamic_cast<GUI::CEditCtrl*>(list.AddItem(row, 1, WND_TYPE_EDIT));
	assert(pEditUserName);
	pEditUserName->SetMaxStringLen( 36 );
	pEditUserName->SetCaption(m_netdeviceInfo.userName, false);
	pEditUserName->SetEnable(false, false);
	
	//////////////////////////////////////////////////////////////////////////
	// row = 4 密码
	row = ROW_PASSWORD;
	list.AddItem(row, 0, m_pStrTable->String(E_STR_ID_PASSWORD));
	pEditPassword = dynamic_cast<GUI::CEditCtrl*>(list.AddItem(row, 1, WND_TYPE_EDIT));
	assert( pEditPassword );
	pEditPassword->SetMaxStringLen( 36 );
	pEditPassword->SetCaption(m_netdeviceInfo.password, false);
	pEditPassword->SetPassword( true );
	pEditPassword->SetEnable(true, false);

	ShowLocalNetWorkInfo();

	list.CreateListFocusMap();
}

unsigned long CSetupIpcNetworkDlg::OnOK()
{
	if (DoCheckUIValue())
	{
		//发送请求修改IPC ip信息的命令到IPC
		CMessageMan::Instance()->PostMsgDataToServer(0, LOCAL_DEVICE_ID, CMD_REQUEST_SET_NET_DEVICE_INFO, (unsigned char *)&m_netdeviceInfo, sizeof(m_netdeviceInfo));

		PUB_Sleep(2000);
	}
	
	//return KEY_VIRTUAL_MSG_STOP; 
	return GUI::CDialog::OnOK();
}


bool CSetupIpcNetworkDlg::DoCheckUIValue()
{
	GUI::CListCtrl& list = m_listCtrl;
	GUI::CIPAddressCtrl* pIP = dynamic_cast<GUI::CIPAddressCtrl*>(m_listCtrl.GetItemWnd(ROW_IP_ADDRESS, 1));
	assert(NULL != pIP);

	GUI::CIPAddressCtrl* pNetmask = dynamic_cast<GUI::CIPAddressCtrl*>(m_listCtrl.GetItemWnd(ROW_NETMASK, 1));
	assert(NULL != pNetmask);

	GUI::CIPAddressCtrl* pGateway = dynamic_cast<GUI::CIPAddressCtrl*>(m_listCtrl.GetItemWnd(ROW_GATEWAY, 1));
	assert(NULL != pGateway);

	GUI::CEditCtrl* pEditUserName = dynamic_cast<GUI::CEditCtrl*>(m_listCtrl.GetItemWnd(ROW_USER_NAME, 1));
	assert(NULL != pEditUserName);

	GUI::CEditCtrl* pEditPassword = dynamic_cast<GUI::CEditCtrl*>(m_listCtrl.GetItemWnd(ROW_PASSWORD, 1));
	assert(NULL != pEditPassword);

	m_netdeviceInfo.newIpaddr = pIP->GetIP();
	m_netdeviceInfo.newNetmask = pNetmask->GetIP();
	m_netdeviceInfo.newGateway = pGateway->GetIP();

	strcpy(m_netdeviceInfo.userName, pEditUserName->GetCaption().c_str());
	strcpy(m_netdeviceInfo.password, pEditPassword->GetCaption().c_str());

	CBase64Coder base64Coder;
	base64Coder.b64encode((unsigned char *)m_netdeviceInfo.password);
	if (0 == strlen(m_netdeviceInfo.userName))
	{
		return false;
	}

	return true;
}


BEGIN_MSG_MAP(CSetupIpcNetworkDlg, GUI::CDialog)
	ON_MSG_EX(m_listCtrl.GetID(), KEY_VIRTUAL_MSG_DEFAULT_TAG, OnClickList)
END_MSG_MAP()

unsigned long CSetupIpcNetworkDlg::OnClickList(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	unsigned short row = (keyID >> 16)&0x7fff;	//行
	unsigned short col = keyID&0xffff;			//列

	if (1 != col)
	{
		return KEY_VIRTUAL_MSG_STOP;
	}
	else
	{
		GUI::CListCtrl& list = m_listCtrl;
		GUI::CIPAddressCtrl* pIP = NULL;
		GUI::CIPAddressCtrl* pNetmask = NULL;
		GUI::CIPAddressCtrl* pGateway = NULL;

		GUI::CEditCtrl* pEditUserName = NULL;
		GUI::CEditCtrl* pEditPassword = NULL;

		switch (row)
		{
			case ROW_IP_ADDRESS:
				{
					pIP = dynamic_cast<GUI::CIPAddressCtrl*>(m_listCtrl.GetItemWnd(ROW_IP_ADDRESS, 1));
					break;
				}
			case ROW_NETMASK:
				{
					pNetmask = dynamic_cast<GUI::CIPAddressCtrl*>(m_listCtrl.GetItemWnd(ROW_NETMASK, 1));
					break;
				}
			case ROW_GATEWAY:
				{
					pGateway = dynamic_cast<GUI::CIPAddressCtrl*>(m_listCtrl.GetItemWnd(ROW_GATEWAY, 1));
					break;
				}
			case ROW_USER_NAME:
				{
					pEditUserName = dynamic_cast<GUI::CEditCtrl*>(m_listCtrl.GetItemWnd(ROW_USER_NAME, 1));
					break;
				}
			case ROW_PASSWORD:
				{
					pEditPassword = dynamic_cast<GUI::CEditCtrl*>(m_listCtrl.GetItemWnd(ROW_PASSWORD, 1));
					break;
				}
			default:
				{
					break;
				}
		}
	}

	return KEY_VIRTUAL_MSG_STOP;
}

void CSetupIpcNetworkDlg::ShowLocalNetWorkInfo()
{
	NETWORK_STATUS_INFO networkStatusInfo;
	memset( &networkStatusInfo, 0, sizeof(NETWORK_STATUS_INFO));

	//获取网络状态
#ifdef __ENVIRONMENT_LINUX__
	MESSAGE_DATA msgData;
	CMessageMan::Instance()->SendMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_NETWORK_INFO, NULL, 0, msgData);

	networkStatusInfo = *reinterpret_cast<NETWORK_STATUS_INFO *>(msgData.pData+sizeof(PACKCMD));

	CMessageMan::Instance()->ReleaseMsgBuff(msgData);
#else
	networkStatusInfo.IP = (155 << 24) | (3 << 16) | (168 << 8) | (192);
	networkStatusInfo.subMask = (0 << 24) | (255 << 16) | (255 << 8) | (255);
	networkStatusInfo.gateway = (1 << 24) | (3 << 16) | (168 << 8) | (192);
#endif

	//////////////////////////////////////////////////////////////////////////
	char str[512] = {0};

	m_listCtrl.AddItem(ROW_LOCAL_IP, 0, m_pStrTable->String(E_STR_ID_LOCAL_IP));
	sprintf(str, "%03d.%03d.%03d.%03d", (networkStatusInfo.IP&0xff), (networkStatusInfo.IP>>8)&0xff, (networkStatusInfo.IP>>16)&0xff, (networkStatusInfo.IP>>24)&0xff);
	m_listCtrl.AddItem(ROW_LOCAL_IP, 1, str);

	m_listCtrl.AddItem(ROW_LOCAL_NETMASK, 0, m_pStrTable->String(E_STR_ID_LOCAL_NET_MASK));
	sprintf(str, "%03d.%03d.%03d.%03d", (networkStatusInfo.subMask&0xff), (networkStatusInfo.subMask>>8)&0xff, (networkStatusInfo.subMask>>16)&0xff, (networkStatusInfo.subMask>>24)&0xff);
	m_listCtrl.AddItem(ROW_LOCAL_NETMASK, 1, str);

	m_listCtrl.AddItem(ROW_LOCAL_GATEWAY, 0, m_pStrTable->String(E_STR_ID_LOCAL_GATEWAY));
	sprintf(str, "%03d.%03d.%03d.%03d", (networkStatusInfo.gateway&0xff), (networkStatusInfo.gateway>>8)&0xff, (networkStatusInfo.gateway>>16)&0xff, (networkStatusInfo.gateway>>24)&0xff);
	m_listCtrl.AddItem(ROW_LOCAL_GATEWAY, 1, str);
}

