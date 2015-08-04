#include "CfgNetWIFIIPInfoDlg.h"

CCfgNetWIFIIpInfoDlg::CCfgNetWIFIIpInfoDlg()
{
	
}

CCfgNetWIFIIpInfoDlg::~CCfgNetWIFIIpInfoDlg()
{

}

void CCfgNetWIFIIpInfoDlg::OnInitial()
{
	GUI::CListCtrl& list = m_cfgListwifiIp;
	GUI::CWnd * pWnd = NULL;
	GUI::CCheckCtrl* pCheck = NULL;
	GUI::CIPAddressCtrl* pIP = NULL;

	SetImage(BMP_MEM_DLG_LARGE, false);
	SetCaption(m_pStrTable->String(E_STR_ID_TITLE_IP_SET), false);

	SetDefaultBtn(DEFAULT_BTN_APPLY | DEFAULT_BTN_CANCEL );
	SetDefaultBtnCaption( DEFAULT_BTN_CANCEL, m_pStrTable->String( E_STR_ID_EXIT ) );
	SetDefaultBtnCaption( DEFAULT_BTN_APPLY, m_pStrTable->String(E_STR_ID_OK) );

	unsigned short x = m_length[DLG_OFFSET_LEFT];
	unsigned short y = m_length[TITLE_HEIGHT] + m_length[TABLE_HEIGHT] + m_length[DLG_OFFSET_TOP];
	unsigned short offset = m_length[DLG_OFFSET_BOTTOM];
	unsigned short width = m_rect.Width() - 2 * m_length[DLG_OFFSET_LEFT];
	unsigned short height = m_rect.Height() - y- offset - m_length[DEFAULT_BTN_BOTTOM_YPOS];


	list.Create(GUI::CWnd::GetRandWndID(), x, y, width, height, this, 0, 1, false);
	list.AddColumn(NULL, width/2);
	list.AddColumn(NULL, width/2 - 4 );
	
	//static ip
	list.AddItem( ROW_STATIC_IP_MODE, 0, m_pStrTable->String(E_STR_ID_IP_AUTO) );
	pCheck = dynamic_cast<GUI::CCheckCtrl*>( list.AddItem( ROW_STATIC_IP_MODE, 1, WND_TYPE_CHECK ) );
	pCheck->SetCheck(false,true);
	assert(pCheck);

	list.AddItem( ROW_STATIC_IP, 0, m_pStrTable->String(E_STR_ID_IP_ADDRESS) );
	pIP = dynamic_cast<GUI::CIPAddressCtrl*>( list.AddItem( ROW_STATIC_IP, 1, WND_TYPE_IP) );
	assert( pIP );

	list.AddItem( ROW_STATIC_SUB_NET_MASK, 0, m_pStrTable->String( E_STR_ID_NET_MASK ) );
	pIP = dynamic_cast<GUI::CIPAddressCtrl*>( list.AddItem( ROW_STATIC_SUB_NET_MASK, 1, WND_TYPE_IP) );
	assert( pIP );

	list.AddItem( ROW_STATIC_GATEWAY, 0, m_pStrTable->String( E_STR_ID_GATEWAY ) );
	pIP = dynamic_cast<GUI::CIPAddressCtrl*>( list.AddItem( ROW_STATIC_GATEWAY, 1, WND_TYPE_IP) );
	assert( pIP );

	list.AddItem( ROW_STATIC_DNS_1, 0, m_pStrTable->String( E_STR_ID_PREFERRED_DNS_SERVER ) );
	pIP = dynamic_cast<GUI::CIPAddressCtrl*>( list.AddItem( ROW_STATIC_DNS_1, 1, WND_TYPE_IP) );
	assert( pIP );
	
	////dhcp ip 
	//list.AddItem( ROW_DHCP_IP_MODE, 0, m_pStrTable->String(E_STR_ID_STATIC_IP) );
	//pCheck = dynamic_cast<GUI::CCheckCtrl*>( list.AddItem( ROW_DHCP_IP_MODE, 1, WND_TYPE_CHECK ) );
	//pCheck->SetCheck(false,true);
	//assert(pCheck);

	//list.AddItem( ROW_DHCP_IP, 0, m_pStrTable->String(E_STR_ID_IP_ADDRESS) );
	//pIP = dynamic_cast<GUI::CIPAddressCtrl*>( list.AddItem( ROW_DHCP_IP, 1, WND_TYPE_IP) );
	//assert( pIP );

	//list.AddItem( ROW_DHCP_SUB_NET_MASK, 0, m_pStrTable->String( E_STR_ID_NET_MASK ) );
	//pIP = dynamic_cast<GUI::CIPAddressCtrl*>( list.AddItem( ROW_DHCP_SUB_NET_MASK, 1, WND_TYPE_IP) );
	//assert( pIP );

	//list.AddItem( ROW_DHCP_GATEWAY, 0, m_pStrTable->String( E_STR_ID_GATEWAY ) );
	//pIP = dynamic_cast<GUI::CIPAddressCtrl*>( list.AddItem( ROW_DHCP_GATEWAY, 1, WND_TYPE_IP) );
	//assert( pIP );

	//list.AddItem( ROW_DHCP_DNS_1, 0, m_pStrTable->String( E_STR_ID_PREFERRED_DNS_SERVER ) );
	//pIP = dynamic_cast<GUI::CIPAddressCtrl*>( list.AddItem( ROW_DHCP_DNS_1, 1, WND_TYPE_IP) );
	//assert( pIP );

	list.CreateListFocusMap();

	//memset(&m_cfgwifi, 0, sizeof(WIFI_CONFIG));
}

void CCfgNetWIFIIpInfoDlg::OnPostInitial()
{
	CDialog::OnPostInitial();
	GUI::CListCtrl& list = m_cfgListwifiIp;
	GUI::CCheckCtrl* pCheck = NULL;
	GUI::CIPAddressCtrl* pIP = NULL;

	pCheck = dynamic_cast<GUI::CCheckCtrl*>( list.GetItemWnd(ROW_STATIC_IP_MODE, 1) );
	if(m_cfgwifi.IPMode == 0)
	{
		pCheck->SetCheck(false,true);

		pIP = dynamic_cast<GUI::CIPAddressCtrl*>(list.GetItemWnd(ROW_STATIC_IP,1));
		assert(pIP);
		pIP->SetIP(m_cfgwifi.staticIP,true);

		pIP = dynamic_cast<GUI::CIPAddressCtrl*>(list.GetItemWnd(ROW_STATIC_SUB_NET_MASK,1));
		assert(pIP);
		pIP->SetIP(m_cfgwifi.subMask,true);

		pIP = dynamic_cast<GUI::CIPAddressCtrl*>(list.GetItemWnd(ROW_STATIC_GATEWAY,1));
		assert(pIP);
		pIP->SetIP(m_cfgwifi.gateway,true);

		pIP = dynamic_cast<GUI::CIPAddressCtrl*>(list.GetItemWnd(ROW_STATIC_DNS_1,1));
		assert(pIP);
		pIP->SetIP(m_cfgwifi.DNSServer,true);

	}
	else if(m_cfgwifi.IPMode == 1)
	{
		pCheck->SetCheck(true,true);
		if(!pCheck->IsCheck())
		{
			list.GetItemWnd(ROW_STATIC_IP,1)->SetEnable(false,true);
			list.GetItemWnd(ROW_STATIC_SUB_NET_MASK,1)->SetEnable(false,true);
			list.GetItemWnd(ROW_STATIC_GATEWAY,1)->SetEnable(false,true);
			list.GetItemWnd(ROW_STATIC_DNS_1,1)->SetEnable(false,true);
		}
	}
}

void CCfgNetWIFIIpInfoDlg::GetwifiIpInfo(WIFI_CONFIG *wifiIpInfo)
{
	wifiIpInfo[0].IPMode = m_cfgwifi.IPMode;
	wifiIpInfo[0].staticIP = m_cfgwifi.staticIP;
	wifiIpInfo[0].subMask = m_cfgwifi.subMask;
	wifiIpInfo[0].gateway = m_cfgwifi.gateway;
	wifiIpInfo[0].DNSServer = m_cfgwifi.DNSServer;
}

void CCfgNetWIFIIpInfoDlg::SetwifiIpInfo(WIFI_CONFIG *wifiIpInfo)
{
	m_cfgwifi.IPMode = wifiIpInfo[0].IPMode;
	m_cfgwifi.staticIP = wifiIpInfo[0].staticIP;
	m_cfgwifi.subMask = wifiIpInfo[0].subMask;
	m_cfgwifi.gateway = wifiIpInfo[0].gateway;
	m_cfgwifi.DNSServer = wifiIpInfo[0].DNSServer;
}

unsigned long CCfgNetWIFIIpInfoDlg::OnApply()
{
	return CDialog::OnOK();
}

BEGIN_MSG_MAP(CCfgNetWIFIIpInfoDlg, CCfgDlg)
	ON_MSG_EX(m_cfgListwifiIp.GetID(), KEY_VIRTUAL_MSG_DEFAULT_TAG, OnClickList)
END_MSG_MAP()

unsigned long CCfgNetWIFIIpInfoDlg::OnClickList(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	unsigned short row = (keyID >> 16)&0x7fff;	
	unsigned short col = keyID&0xffff;			

	GUI::CListCtrl& list = m_cfgListwifiIp;
	GUI::CCheckCtrl* pCheck = NULL;
	GUI::CIPAddressCtrl* pIP = NULL;

	if((ROW_STATIC_IP_MODE == row) && (1 == col))
	{
		pCheck = dynamic_cast<GUI::CCheckCtrl*>(list.GetItemWnd(ROW_STATIC_IP_MODE,1));
		m_cfgwifi.IPMode = pCheck->IsCheck()?1:0;
		if(m_cfgwifi.IPMode)
		{
			list.GetItemWnd(ROW_STATIC_IP,1)->SetEnable(false,true);
			list.GetItemWnd(ROW_STATIC_SUB_NET_MASK,1)->SetEnable(false,true);
			list.GetItemWnd(ROW_STATIC_GATEWAY,1)->SetEnable(false,true);
			list.GetItemWnd(ROW_STATIC_DNS_1,1)->SetEnable(false,true);
		}
		else
		{
			list.GetItemWnd(ROW_STATIC_IP,1)->SetEnable(true,true);
			list.GetItemWnd(ROW_STATIC_SUB_NET_MASK,1)->SetEnable(true,true);
			list.GetItemWnd(ROW_STATIC_GATEWAY,1)->SetEnable(true,true);
			list.GetItemWnd(ROW_STATIC_DNS_1,1)->SetEnable(true,true);
		}
	}
	if((ROW_STATIC_IP == row) && (1 == col))
	{
		pIP =dynamic_cast<GUI::CIPAddressCtrl*>(list.GetItemWnd(ROW_STATIC_IP,1));
		m_cfgwifi.staticIP = pIP->GetIP();
	}
	if((ROW_STATIC_SUB_NET_MASK == row) && ( 1 == col))
	{
		pIP =dynamic_cast<GUI::CIPAddressCtrl*>(list.GetItemWnd(ROW_STATIC_SUB_NET_MASK,1));
		m_cfgwifi.subMask = pIP->GetIP();
	}
	if((ROW_STATIC_GATEWAY == row) && (1 == col))
	{
		pIP =dynamic_cast<GUI::CIPAddressCtrl*>(list.GetItemWnd(ROW_STATIC_GATEWAY,1));
		m_cfgwifi.gateway = pIP->GetIP();
	}
	if((ROW_STATIC_DNS_1 == row) && (1 == col))
	{
		pIP =dynamic_cast<GUI::CIPAddressCtrl*>(list.GetItemWnd(ROW_STATIC_DNS_1,1));
		m_cfgwifi.DNSServer = pIP->GetIP();
	}

	return KEY_VIRTUAL_MSG_STOP;
}
