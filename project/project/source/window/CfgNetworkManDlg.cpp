/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : L
** Date         : 2009-08-14
** Name         : CfgNetworkManDlg.cpp
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/

#include "CfgNetworkManDlg.h"
#include "CharCodeTable.h"
//#include "DdnsClient.h"
#include "FuncCustom.h"

extern bool g_curNet3G;

CCfgNetworkManDlg::CCfgNetworkManDlg() : m_curPage(PAGE_NETWORK),
m_initAnchorPage(PAGE_NETWORK),
m_initPageMask(0)
{
	for (int i = 0; i < PAGE_COUNT; i++)
	{
		m_szPageItem[i] = PAGE_ITEM(i);
	}
	#ifdef __SUPPORT_3G__
		m_3gLastStatus = g_curNet3G;
	#endif
}

CCfgNetworkManDlg::~CCfgNetworkManDlg()
{

}


void CCfgNetworkManDlg::OnInitial()
{
	CCfgDlg::OnInitial();
	SetImage(BMP_MEM_DLG_LARGE, false);
	SetDefaultBtn( DEFAULT_BTN_APPLY | DEFAULT_BTN_OK | DEFAULT_BTN_CANCEL);
	SetDefaultBtnCaption( DEFAULT_BTN_CANCEL, m_pStrTable->String(E_STR_ID_EXIT) );
	SetDefaultBtnCaption( DEFAULT_BTN_OK, m_pStrTable->String(E_STR_ID_APPLY) );
	SetDefaultBtnCaption( DEFAULT_BTN_APPLY, m_pStrTable->String(E_STR_ID_DEFAULT) );
	//////////////////////////////////////////////////////////////////////

	m_initPageMask= 0x1;

#ifndef __DVR_ANCHOR__
	SetCaption(m_pStrTable->String(E_STR_ID_TITLE_NETWORK), false);

	unsigned short x = m_length[DLG_OFFSET_LEFT];
	unsigned short y = m_length[TITLE_HEIGHT] + m_length[TABLE_HEIGHT] + m_length[DLG_OFFSET_TOP];
	unsigned short cx = m_rect.Width() - 2 * m_length[DLG_OFFSET_LEFT];
	unsigned short cy = m_rect.Height() - y- m_length[DLG_OFFSET_BOTTOM] - m_length[DEFAULT_BTN_BOTTOM_YPOS];

#ifdef __SUPPORT_3G__
	m_cfgNetworkExDlg.Create(GUI::CWnd::GetRandWndID(), x, y, cx, cy, this, 0,2);
#else
	m_cfgNetworkDlg.Create(GUI::CWnd::GetRandWndID(), x, y, cx, cy, this, 0,2);
#endif

//	//m_cfgNetworkVideoDlg.Create(GUI::CWnd::GetRandWndID(),x,y,cx,cy,this, 2,1);
//	m_cfgNetworkStreamDlg.Create(GUI::CWnd::GetRandWndID(), x, y, cx, cy, this, 0,3);
//#ifdef __DVR_BASIC__	
//	m_cfgNetEmailDlg.Create(GUI::CWnd::GetRandWndID(), x, y, cx, cy, this, 0,4);
//	//m_cfgNetFtpDlg.Create(GUI::CWnd::GetRandWndID(), x, y, cx, cy, this, 0, 5);
//#endif
	
#ifdef __ALARM_TO_UPDATE_FTP_VIDEO__
	m_cfgNetFtpDlg.Create(GUI::CWnd::GetRandWndID(), x, y, cx, cy, this, 0, 5);
#endif

//
//#ifdef __MOBILE_DVR__
//	m_cfgNetWifiDlg.Create(GUI::CWnd::GetRandWndID(), x, y, cx, cy, this, 0,6);
//	m_cfgNetWirelessDlg.Create(GUI::CWnd::GetRandWndID(), x, y, cx, cy, this, 0,7);
//	m_cfgNetServerDlg.Create(GUI::CWnd::GetRandWndID(), x, y, cx, cy, this, 0,8);
//#endif
//
//#ifdef __SUPPORT_HRSST__
//	m_cfgNetPhoneDlg.Create(GUI::CWnd::GetRandWndID(), x, y, cx, cy, this, 0,9);
//#endif
//
//	m_cfgNetMoreConfigDlg.Create(GUI::CWnd::GetRandWndID(), x, y, cx, cy, this, 0,10);

	m_tabCtrl.Create(GUI::CWnd::GetRandWndID(), x, m_length[TITLE_HEIGHT] + m_length[DLG_OFFSET_TOP], cx, m_length[TABLE_HEIGHT], this, 0, 1);
#ifdef __SUPPORT_3G__
	m_tabCtrl.AddItem(&m_cfgNetworkExDlg, m_pStrTable->String( E_STR_ID_NETWORK ) );
#else
	m_tabCtrl.AddItem(&m_cfgNetworkDlg, m_pStrTable->String( E_STR_ID_NETWORK ) );
#endif
	if (s_loginSuccessReply.productInfo.videoInputNum)
	{
		m_tabCtrl.AddItem(&m_cfgNetworkStreamDlg, m_pStrTable->String(E_STR_ID_NETWORK_STREAM));	
	}
	else
	{
		for (int i = 0; i < PAGE_COUNT; i++)
		{
			if (m_szPageItem[i] == PAGE_NETWORK_STREAM)
			{
				while (i + 1 < PAGE_COUNT)
				{
					m_szPageItem[i] = m_szPageItem[i + 1];
					++i;
				}
				//m_szPageItem[i] = static_cast<PAGE_ITEM>(10);
				break;
			}
		}
	}
	
#ifdef __DVR_BASIC__	
	m_tabCtrl.AddItem(&m_cfgNetEmailDlg, m_pStrTable->String(E_STR_ID_EMAIL));
//	m_tabCtrl.AddItem(&m_cfgNetFtpDlg, m_pStrTable->String(E_STR_ID_FTP));
#endif

#ifdef __ALARM_TO_UPDATE_FTP_VIDEO__
	m_tabCtrl.AddItem(&m_cfgNetFtpDlg, m_pStrTable->String(E_STR_ID_FTP));
#endif

#ifdef __MOBILE_DVR__
	m_tabCtrl.AddItem(&m_cfgNetWifiDlg, m_pStrTable->String(E_STR_ID_NETWORK_WIFI));
	m_tabCtrl.AddItem(&m_cfgNetWirelessDlg, m_pStrTable->String(E_STR_ID_NETWORK_WIRELESS));
	m_tabCtrl.AddItem(&m_cfgNetServerDlg, m_pStrTable->String(E_STR_ID_NETWORK_SERVER));
#endif
	
#ifdef __SUPPORT_HRSST__
    m_tabCtrl.AddItem(&m_cfgNetPhoneDlg, "Phone Server Settings");
#endif




#if !defined(__CUSTOM_BR27__) && !defined(__KR24_IN__)
	m_tabCtrl.AddItem(&m_cfgNetAutoReportDlg, m_pStrTable->String(E_STR_ID_SERVER));	
#endif

#if defined(__TVT_NAT_SOCK__)
	m_tabCtrl.AddItem(&m_cfgNetworkNatDlg, m_pStrTable->String(E_STR_ID_NAT));
#endif

#if defined(__TVT_PUSH_SOCK__)
	m_tabCtrl.AddItem(&m_cfgNetworkPushDlg, m_pStrTable->String(E_STR_ID_PUSH));
#endif

#if defined(__KR24_IN__)
	m_tabCtrl.AddItem(&m_cfgNetMoreConfigDlg, m_pStrTable->String(E_STR_ID_DDNS));
#else
	m_tabCtrl.AddItem(&m_cfgNetMoreConfigDlg, m_pStrTable->String( E_STR_ID_OTHERSETTINGS ) );
#endif

	m_curPage = PAGE_NETWORK;
	m_tabCtrl.SetCurItem(m_curPage , false );
	CCfgDlg::SetRefreshView( false );

#else
	unsigned short x = m_length[DLG_OFFSET_LEFT];
	unsigned short y = m_length[TITLE_HEIGHT] + m_length[DLG_OFFSET_TOP];
	unsigned short offset = m_length[DLG_OFFSET_BOTTOM];
	unsigned short cx = m_rect.Width() - 2 * m_length[DLG_OFFSET_LEFT];
	unsigned short cy = m_rect.Height() - y - offset - m_length[DEFAULT_BTN_BOTTOM_YPOS];

	switch (m_initAnchorPage)
	{
	case PAGE_NETWORK:
		SetCaption(m_pStrTable->String(E_STR_ID_TITLE_NETWORK), false);
#ifdef __SUPPORT_3G__
		m_cfgNetworkExDlg.Create(GetRandWndID(), x, y, cx, cy, this, 0,1);
#else
		m_cfgNetworkDlg.Create(GetRandWndID(), x, y, cx, cy, this, 0, 1);
#endif
		break;
	case PAGE_NETWORK_STREAM:
		SetCaption(m_pStrTable->String(E_STR_ID_TITLE_SUB_STREAM), false);
		m_cfgNetworkStreamDlg.Create(GetRandWndID(), x, y, cx, cy, this, 0, 1);
		break;
#ifdef __DVR_BASIC__
	case PAGE_NETWORK_EMAIL:
		SetCaption(m_pStrTable->String(E_STR_ID_TITLE_EMAIL), false);
		m_cfgNetEmailDlg.Create(GetRandWndID(), x, y, cx, cy, this, 0, 1);
		break;
#endif

#ifdef __ALARM_TO_UPDATE_FTP_VIDEO__
	case PAGE_NETWORK_FTP:
		SetCaption(m_pStrTable->String(E_STR_ID_FTP), false);		
		m_cfgNetFtpDlg.Create(GetRandWndID(), x, y, cx, cy, this, 0, 1);
		break;
#endif

#ifdef __MOBILE_DVR__
	case PAGE_NETWORK_WIFI:
		SetCaption(m_pStrTable->String(E_STR_ID_TITLE_WIFI_SETUP), false);
		m_cfgNetWifiDlg.Create(GetRandWndID(), x, y, cx, cy, this, 0, 1);
		break;
	case PAGE_NETWORK_WIRELESS:
		SetCaption(m_pStrTable->String(E_STR_ID_TITLE_NETWORK_WIRELESS), false);
		m_cfgNetWirelessDlg.Create(GetRandWndID(), x, y, cx, cy, this, 0, 1);
		break;
	case PAGE_NETWORK_SERVER:
		SetCaption(m_pStrTable->String(E_STR_ID_TITLE_NETWORK_SERVER), false);
		m_cfgNetServerDlg.Create(GetRandWndID(), x, y, cx, cy, this, 0, 1);
		break;
#endif
#ifdef __SUPPORT_HRSST__
	case PAGE_NETWORK_PHONE:
		SetCaption("Phone Server Settings", false);
		m_cfgNetPhoneDlg.Create(GetRandWndID(), x, y, cx, cy, this, 0, 1);
		break;
#endif
		
#if !defined(__CUSTOM_BR27__) && !defined(__KR24_IN__)
	case PAGE_NETWORK_AUTOREPORT:
		SetCaption(m_pStrTable->String(E_STR_ID_TITLE_NETWORK_SERVER), false);
		m_cfgNetAutoReportDlg.Create(GetRandWndID(), x, y, cx, cy, this, 0, 1);
		break;
#endif

#if defined(__TVT_NAT_SOCK__)
	case PAGE_NETWORK_NAT:
		SetCaption(m_pStrTable->String(E_STR_ID_NAT), false);
		m_cfgNetworkNatDlg.Create(GetRandWndID(), x, y, cx, cy, this, 0, 1);
		break;
#endif

#if defined(__TVT_PUSH_SOCK__)
	case PAGE_NETWORK_PUSH:
		SetCaption(m_pStrTable->String(E_STR_ID_PUSH), false);
		m_cfgNetworkPushDlg.Create(GetRandWndID(), x, y, cx, cy, this, 0, 1);
		break;
#endif
	case PAGE_NETWORK_MORECONFIG:
		SetCaption(m_pStrTable->String(E_STR_ID_TITLE_OTHERSETTINGS), false);
		m_cfgNetMoreConfigDlg.Create(GetRandWndID(), x, y, cx, cy, this, 0, 1);
		break;	
	default:
		assert(false);
		break;
	}

	m_curPage = m_initAnchorPage;
	CCfgDlg::SetRefreshView( false );
#endif

	////
	CreateTipWnd();
}

extern NET_LOGIN_SUCCESS_REPLY *g_p_login_succ;

void CCfgNetworkManDlg::OnPostInitial()
{
	CCfgDlg::OnPostInitial();

	if ((PAGE_NETWORK_STREAM == m_initAnchorPage) || (PAGE_NETWORK_STREAM == m_curPage))
	{
		if ( ( (0 < g_p_login_succ->productInfo.netVideoInputNum) || (!IsSupportSubCode()) )
			 && (NULL != m_pBtnDefualt) )
		{
			m_pBtnDefualt->SetEnable(false, true);
		}
	}
}

void CCfgNetworkManDlg::GetItemList(CFGITEM_LIST& itemList) const
{
	if(s_loginSuccessReply.productInfo.localVideoInputNum > 0)
	{
		itemList.push_back(NCFG_ITEM_ENCODE_MINOR);
	}

	if (0 < s_loginSuccessReply.productInfo.netVideoInputNum)
	{
		itemList.push_back(NCFG_ITEM_ENCODE_MINOR_EX);
	}
#ifdef __DVR_BASIC__
	//itemList.push_back(NCFG_ITEM_JPEG_PERIOD);	
#endif
	//if(s_loginSuccessReply.productInfo.netVideoInputNum == 0)
	{
		itemList.push_back(NCFG_ITEM_UPNP_ENABLE);
	}
	itemList.push_back(NCFG_ITEM_HTTP_PORT);
	itemList.push_back(NCFG_ITEM_SERVER_PORT);

	itemList.push_back(NCFG_ITEM_NETWORK_ADDR_INFO);
	itemList.push_back(NCFG_ITEM_DNS_1);
	itemList.push_back(NCFG_ITEM_DNS_2);

	itemList.push_back(NCFG_ITEM_DDNS_INFO);
	itemList.push_back(NCFG_ITEM_PPPOE_INFO);
#ifdef __DVR_BASIC__	
	itemList.push_back(NCFG_ITEM_EMAIL_SEND_INFO);
	itemList.push_back(NCFG_ITEM_EMAIL_RECV_INFO);
	itemList.push_back(NCFG_ITEM_EMAIL_PIC_ACC);
#endif

	//DDNS更新周期
	itemList.push_back(NCFG_ITEM_DDNS_UPDATE_CYCLE);

#ifdef __SUPPORT_HRSST__	
	itemList.push_back(NCFG_ITEM_HRSST_SERVER_IP);
	itemList.push_back(NCFG_ITEM_HRSST_SERVER_PORT);
#endif
	itemList.push_back(NCFG_ITEM_NET_WIRELESS);

	itemList.push_back(NCFG_ITEM_AUTO_REPORT);
	
	itemList.push_back(NCFG_ITEM_MINT_DDNS_SERVER_NAME);
	itemList.push_back(NCFG_ITEM_USE_3G);
#if defined (__CUSTOM_IPV6__)
	itemList.push_back(NCFG_ITEM_USE_IPV4_IPV6);
	itemList.push_back(NCFG_ITEM_NETWORK_ADDR_INFO_IPV6);
	itemList.push_back(NCFG_ITEM_DNS_1_IPV6);
	itemList.push_back(NCFG_ITEM_DNS_2_IPV6);
#endif

#ifdef __ALARM_TO_UPDATE_FTP_VIDEO__
	itemList.push_back(NCFG_ITEM_NET_FTP);
#endif

	itemList.push_back(NCFG_ITEM_NAT_SERVER);
	itemList.push_back(NCFG_ITEM_PUSH_SERVER);
	if(CProduct::Instance()->PoeInputNum() > 0)
	{
		itemList.push_back(NCFG_ITEM_SWITCH_INFO);
	}
	
	itemList.push_back(NCFG_ITEM_USE_PUSH);

#ifdef __MOBILE_DVR__
	itemList.push_back(NCFG_ITEM_NET_SERVER);
	itemList.push_back(NCFG_ITEM_WIRELESS_WIFI);
	itemList.push_back(NCFG_ITEM_WIRELESS_COMM);
#endif


	////read only
	itemList.push_back(NCFG_ITEM_PPPOE_USER_NAME_MAX_LEN);
	itemList.push_back(NCFG_ITEM_PPPOE_PASSWORD_MAX_LEN);
	itemList.push_back(NCFG_ITEM_DDNS_USER_NAME_MAX_LEN);
	itemList.push_back(NCFG_ITEM_DDNS_PASSWORD_MAX_LEN);
	itemList.push_back(NCFG_ITEM_DDNS_DOMAIN_MAX_LEN);
	itemList.push_back(NCFG_ITEM_DDNS_SERVER_INFO);

	itemList.push_back(NCFG_ITEM_ENCODE_MINOR_SUPPORT_PROPERTY);
	itemList.push_back(NCFG_ITEM_MINOR_SUPPORT_QULITY_LEVELNUM);
	itemList.push_back(NCFG_ITEM_MINOR_SUPPORT_ENCODEMODE);
	itemList.push_back(NCFG_ITEM_MINOR_SUPPORT_BITRATE_RANGE);
	itemList.push_back(NCFG_ITEM_ENCODE_SIZE_MINOR_ALL_CH);

	itemList.push_back(NCFG_ITEM_EMAIL_IMG_NUM_RANGE);
	itemList.push_back(NCFG_ITEM_EMAIL_SNAP_TIME_RANGE);
}

unsigned long CCfgNetworkManDlg::OnApply()
{
	//Load default
	CCfgDlg* pCfgDlg = GetPage(m_curPage);
	assert(pCfgDlg);
	unsigned long ret = OnLoadDefault(pCfgDlg);

	if( pCfgDlg->IsModify() )
	{
		EnableOkBtn(true,false);
	}
	else
	{
		EnableOkBtn(false,false);
	}

	ENUM_STRING_ID strID = ret ? static_cast<ENUM_STRING_ID>(ret) : E_STR_ID_TIP_00_03;
	MessageOut(strID,TIP_MSG,false);

	//一次重绘全部
	Repaint();

	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CCfgNetworkManDlg::CheckValue()
{
	CCfgDlg* pCfgDlg = GetPage(m_curPage);
	assert(pCfgDlg);

	return pCfgDlg->CheckValue();
}

//unsigned long CCfgNetworkManDlg::OnOK()
//{
//	unsigned int retval = m_cfgNetworkDlg.CheckValue();
//
//	//switch( retval )
//	//{
//	//case DDNS_IPUPDATE_OK:
//	//	MessageOut(E_STR_ID_MESSAGE_56, TIP_MSG, true );
//	//	break;
//	//case DDNS_IPUPDATE_ENAMEPWD:
//	//	MessageOut(E_STR_ID_MESSAGE_57, TIP_ERR, true );
//	//	break;
//	//case DDNS_IPUPDATE_ECONNECT:
//	//case DDNS_IPUPDATE_ENETUNREACHD:
//	//	MessageOut(E_STR_ID_MESSAGE_58, TIP_ERR, true );
//	//	break;
//	//case DDNS_IPUPDATE_EUNKNOWN:
//	//	MessageOut(E_STR_ID_MESSAGE_59, TIP_ERR, true );
//	//	break;
//	//default:
//	//	break;
//	//}
//	printf("%s():%s:%d, check value=%d\n", __FUNCTION__, __FILE__, __LINE__, retval);
//	MessageOut(E_STR_ID_TIP_00,TIP_MSG,true);
//
//	if( DDNS_IPUPDATE_OK != retval )
//	{
//		return KEY_VIRTUAL_MSG_STOP;
//	}
//	else
//	{
//		return CCfgDlg::OnOK();
//	}
//}

CCfgDlg* CCfgNetworkManDlg::GetPage(PAGE_ITEM item)
{
	CCfgDlg* pCfgDlg = NULL;
	switch (item)
	{
	case PAGE_NETWORK:
#ifdef __SUPPORT_3G__
		pCfgDlg = &m_cfgNetworkExDlg;
#else
		pCfgDlg = &m_cfgNetworkDlg;
#endif
		break;
	//case PAGE_NETWORK_VIDEO:
	//	pCfgDlg = &m_cfgNetworkVideoDlg;
	//	break;
	case PAGE_NETWORK_STREAM:
		pCfgDlg = &m_cfgNetworkStreamDlg;
		break;
#ifdef __DVR_BASIC__	
	case PAGE_NETWORK_EMAIL:
		pCfgDlg = &m_cfgNetEmailDlg;
		break;
	//case PAGE_NETWORK_FTP:
	//	pCfgDlg = &m_cfgNetFtpDlg;
	//	break;
#endif

#ifdef __ALARM_TO_UPDATE_FTP_VIDEO__
	case PAGE_NETWORK_FTP:
		pCfgDlg = &m_cfgNetFtpDlg;
		break;
#endif

#ifdef __MOBILE_DVR__
	case PAGE_NETWORK_WIRELESS:
		pCfgDlg = &m_cfgNetWirelessDlg;
		break;
	case PAGE_NETWORK_SERVER:
		pCfgDlg = &m_cfgNetServerDlg;			
		break;
	case PAGE_NETWORK_WIFI:
		pCfgDlg = &m_cfgNetWifiDlg;			
		break;
#endif
#ifdef __SUPPORT_HRSST__
	case PAGE_NETWORK_PHONE:
		pCfgDlg = &m_cfgNetPhoneDlg;
		break;
#endif

#if !defined(__CUSTOM_BR27__) && !defined(__KR24_IN__)
	case PAGE_NETWORK_AUTOREPORT:
		pCfgDlg = &m_cfgNetAutoReportDlg;
		break;
#endif

#if defined(__TVT_NAT_SOCK__)
	case PAGE_NETWORK_NAT:
		pCfgDlg = &m_cfgNetworkNatDlg;
		break;
#endif
#if defined(__TVT_PUSH_SOCK__)
	case PAGE_NETWORK_PUSH:
		pCfgDlg = &m_cfgNetworkPushDlg;
		break;
#endif

	case PAGE_NETWORK_MORECONFIG:
		pCfgDlg = &m_cfgNetMoreConfigDlg;
		break;	
	default:
		assert(false);
		break;
	}

	return pCfgDlg;
}

void CCfgNetworkManDlg::SetAnchorPage(PAGE_ITEM item)
{
	m_initAnchorPage = item;
}

BEGIN_MSG_MAP(CCfgNetworkManDlg,CCfgDlg)
#ifdef __SUPPORT_3G__
	ON_MSG(m_cfgNetworkExDlg.GetID(), KEY_VIRTUAL_ENTER, OnClickNetwork)
#else
	ON_MSG(m_cfgNetworkDlg.GetID(), KEY_VIRTUAL_ENTER, OnClickNetwork)
#endif
	ON_MSG(m_cfgNetMoreConfigDlg.GetID(), KEY_VIRTUAL_ENTER, OnClickNetMoreConfig)
#if defined(__TVT_NAT_SOCK__)
	ON_MSG(m_cfgNetworkNatDlg.GetID(),KEY_VIRTUAL_ENTER,OnClickNetworkNat)
#endif
#if defined(__TVT_PUSH_SOCK__)
	ON_MSG(m_cfgNetworkPushDlg.GetID(),KEY_VIRTUAL_ENTER,OnClickNetworkPush)
#endif
	ON_MSG(m_cfgNetworkStreamDlg.GetID(), KEY_VIRTUAL_ENTER, OnClickNetworkStream)
#ifdef __DVR_BASIC__	
	ON_MSG(m_cfgNetEmailDlg.GetID(), KEY_VIRTUAL_ENTER, OnClickNetEmail)
	//ON_MSG(m_cfgNetFtpDlg.GetID(), KEY_VIRTUAL_ENTER, OnClickNetFtp)
#endif

#ifdef __ALARM_TO_UPDATE_FTP_VIDEO__
	ON_MSG(m_cfgNetFtpDlg.GetID(), KEY_VIRTUAL_ENTER, OnClickNetFtp)
#endif

#ifdef __MOBILE_DVR__
	ON_MSG(m_cfgNetWifiDlg.GetID(), KEY_VIRTUAL_ENTER, OnClickNetWifi)
	ON_MSG(m_cfgNetWirelessDlg.GetID(), KEY_VIRTUAL_ENTER, OnClickNetWireless)
	ON_MSG(m_cfgNetServerDlg.GetID(), KEY_VIRTUAL_ENTER, OnClickNetServer)
#endif

#ifdef __SUPPORT_HRSST__
	ON_MSG(m_cfgNetPhoneDlg.GetID(), KEY_VIRTUAL_ENTER, OnClickNetPhone)
#endif

	ON_MSG(m_cfgNetAutoReportDlg.GetID(), KEY_VIRTUAL_ENTER, OnClickNetAutoReport);
END_MSG_MAP()

unsigned long CCfgNetworkManDlg::OnPreCommand(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
#ifdef __SUPPORT_3G__
	if(m_curPage == PAGE_NETWORK)
	{
		if(m_cfgNetworkExDlg.GetCurPage()== 0)//子页面PAGE_NETWORK
		{
			GUI::CButton *pBtn = dynamic_cast<GUI::CButton*>(GetCtrl(CTRLID_DEFAULT_BTN_APPLY));
			assert(pBtn != NULL);
			pBtn->SetEnable(true, true);
		}
		else if(m_cfgNetworkExDlg.GetCurPage()== 1)
		{
			GUI::CButton *pBtn = dynamic_cast<GUI::CButton*>(GetCtrl(CTRLID_DEFAULT_BTN_APPLY));
			assert(pBtn != NULL);
			pBtn->SetEnable(false, true);
		}
	}
	else
	{
		GUI::CButton *pBtn = dynamic_cast<GUI::CButton*>(GetCtrl(CTRLID_DEFAULT_BTN_APPLY));
		assert(pBtn != NULL);
		pBtn->SetEnable(true, true);
	}
#endif

#if defined(__CHIPGM__)
	if (TD_2704AS_SL == CProduct::Instance()->ProductType())
	{
		if(m_curPage == PAGE_NETWORK_STREAM)
		{
			GUI::CButton *pBtn = dynamic_cast<GUI::CButton*>(GetCtrl(CTRLID_DEFAULT_BTN_APPLY));
			assert(pBtn != NULL);
			pBtn->SetEnable(false, true);
		}
	}
#endif

	if ((ctrlID == m_tabCtrl.GetID()) && (KEY_VIRTUAL_ENTER == keyID))
	{
		if (m_curPage != m_szPageItem[m_tabCtrl.GetFocusItem()])
		{
			unsigned long ret = 0;
			if (GetPage(m_curPage)->IsModify())
			{
				ret = TrySaveChange(GetPage(m_curPage),true,false,false);
			}

			if ( ret == 0 )
			{
				m_curPage = static_cast<PAGE_ITEM>(m_szPageItem[m_tabCtrl.GetFocusItem()]);
			}
			else
			{
				Repaint();

				return KEY_VIRTUAL_MSG_STOP;
			}
		}
		
		{
			unsigned short x = m_length[DLG_OFFSET_LEFT];
			unsigned short y = m_length[TITLE_HEIGHT] + m_length[TABLE_HEIGHT] + m_length[DLG_OFFSET_TOP];
			unsigned short cx = m_rect.Width() - 2 * m_length[DLG_OFFSET_LEFT];
			unsigned short cy = m_rect.Height() - y- m_length[DLG_OFFSET_BOTTOM] - m_length[DEFAULT_BTN_BOTTOM_YPOS];

			if(s_loginSuccessReply.productInfo.videoInputNum > 0)
			{
				if(!((m_initPageMask >> PAGE_NETWORK_STREAM) & 0x1) && (PAGE_NETWORK_STREAM == m_tabCtrl.GetFocusItem()))
				{
					m_cfgNetworkStreamDlg.Create(GUI::CWnd::GetRandWndID(),x,y,cx,cy, this,0, PAGE_NETWORK_STREAM + 2);

					SetSubCfgDlg(&m_cfgNetworkStreamDlg);
					m_initPageMask |= (0x1 << PAGE_NETWORK_STREAM);
				}
				
				if ( (PAGE_NETWORK_STREAM == m_curPage) && ((0 < g_p_login_succ->productInfo.netVideoInputNum)|| (!IsSupportSubCode())) )
				{
					if (NULL != m_pBtnDefualt)
					{
						m_pBtnDefualt->SetEnable(false, true);
					}
				}
				else
				{
					if (NULL != m_pBtnDefualt)
					{
						m_pBtnDefualt->SetEnable(true, true);
					}
				}
			}
			
#ifdef __DVR_BASIC__
			if(!((m_initPageMask >> PAGE_NETWORK_EMAIL) & 0x1) && (PAGE_NETWORK_EMAIL == m_szPageItem[m_tabCtrl.GetFocusItem()]))
			{
				m_cfgNetEmailDlg.Create(GUI::CWnd::GetRandWndID(),x,y,cx,cy, this,0, PAGE_NETWORK_EMAIL + 2);

				SetSubCfgDlg(&m_cfgNetEmailDlg);
				m_initPageMask |= (0x1 << PAGE_NETWORK_EMAIL);
			}
#endif

#ifdef __SUPPORT_HRSST__
			if(!((m_initPageMask >> PAGE_NETWORK_PHONE) & 0x1) && (PAGE_NETWORK_PHONE == m_szPageItem[m_tabCtrl.GetFocusItem()]))
			{
				m_cfgNetPhoneDlg.Create(GUI::CWnd::GetRandWndID(),x,y,cx,cy, this,0, PAGE_NETWORK_PHONE + 2);

				SetSubCfgDlg(&m_cfgNetPhoneDlg);
				m_initPageMask |= (0x1 << PAGE_NETWORK_PHONE);
			}
#endif

#if !defined(__CUSTOM_BR27__) && !defined(__KR24_IN__)
			if(!((m_initPageMask >> PAGE_NETWORK_AUTOREPORT) & 0x1) && (PAGE_NETWORK_AUTOREPORT == m_szPageItem[m_tabCtrl.GetFocusItem()]))
			{
				m_cfgNetAutoReportDlg.Create(GUI::CWnd::GetRandWndID(),x,y,cx,cy, this,0, PAGE_NETWORK_AUTOREPORT + 2);

				SetSubCfgDlg(&m_cfgNetAutoReportDlg);
				m_initPageMask |= (0x1 << PAGE_NETWORK_AUTOREPORT);
			}
#endif

#if defined(__TVT_NAT_SOCK__)
			if(!((m_initPageMask >> PAGE_NETWORK_NAT) & 0x1) && (PAGE_NETWORK_NAT == m_szPageItem[m_tabCtrl.GetFocusItem()]))
			{
				m_cfgNetworkNatDlg.Create(GUI::CWnd::GetRandWndID(),x,y,cx,cy, this,0, PAGE_NETWORK_NAT + 2);

				SetSubCfgDlg(&m_cfgNetworkNatDlg);
				m_initPageMask |= (0x1 << PAGE_NETWORK_NAT);				
			}
#endif

#if defined(__TVT_PUSH_SOCK__)
			if(!((m_initPageMask >> PAGE_NETWORK_PUSH) & 0x1) && (PAGE_NETWORK_PUSH == m_szPageItem[m_tabCtrl.GetFocusItem()]))
			{
				m_cfgNetworkPushDlg.Create(GUI::CWnd::GetRandWndID(),x,y,cx,cy, this,0, PAGE_NETWORK_PUSH + 2);

				SetSubCfgDlg(&m_cfgNetworkPushDlg);
				m_initPageMask |= (0x1 << PAGE_NETWORK_PUSH);				
			}
#endif

			if(!((m_initPageMask >> PAGE_NETWORK_MORECONFIG) & 0x1) && (PAGE_NETWORK_MORECONFIG == m_szPageItem[m_tabCtrl.GetFocusItem()]))
			{
				m_cfgNetMoreConfigDlg.Create(GUI::CWnd::GetRandWndID(),x,y,cx,cy, this,0, PAGE_NETWORK_MORECONFIG + 2);

				SetSubCfgDlg(&m_cfgNetMoreConfigDlg);
				m_initPageMask |= (0x1 << PAGE_NETWORK_MORECONFIG);				
			}
		}

// 		if (m_curPage != m_szPageItem[m_tabCtrl.GetFocusItem()])
// 		{
// 			unsigned long ret = 0;
// 			if (GetPage(m_curPage)->IsModify())
// 			{
// 				ret = TrySaveChange(GetPage(m_curPage),true,false,false);
// 			}
// 
// 			if ( ret == 0 )
// 			{
// 				m_curPage = static_cast<PAGE_ITEM>(m_szPageItem[m_tabCtrl.GetFocusItem()]);
// 			}
// 			else
// 			{
// 				Repaint();
// 
// 				return KEY_VIRTUAL_MSG_STOP;
// 			}
// 		}
	}

	return keyID;
}

unsigned long CCfgNetworkManDlg::OnClickNetwork()
{
#ifdef __SUPPORT_3G__
	if(m_cfgNetworkExDlg.IsModify())
#else
	if (m_cfgNetworkDlg.IsModify())
#endif
	{
		EnableOkBtn(true, true);
	}

	return KEY_VIRTUAL_MSG_STOP;
}

//unsigned long CCfgNetworkManDlg::OnClickNetworkVideo()
//{
//	if( m_cfgNetworkVideoDlg.IsModify() )
//	{
//		EnableOkBtn(true,true);
//	}
//
//	return KEY_VIRTUAL_MSG_STOP;
//}

unsigned long CCfgNetworkManDlg::OnClickNetworkStream()
{
	if (m_cfgNetworkStreamDlg.IsModify())
	{
		EnableOkBtn(true, true);
	}

	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CCfgNetworkManDlg::OnClickNetMoreConfig()
{
	if (m_cfgNetMoreConfigDlg.IsModify())
	{
		EnableOkBtn(true, true);
	}
	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CCfgNetworkManDlg::OnClickNetworkNat()
{
	if (m_cfgNetworkNatDlg.IsModify())
	{
		EnableOkBtn(true, true);
	}
	return KEY_VIRTUAL_MSG_STOP;
}


unsigned long CCfgNetworkManDlg::OnClickNetworkPush()
{
	if (m_cfgNetworkPushDlg.IsModify())
	{
		EnableOkBtn(true, true);
	}
	return KEY_VIRTUAL_MSG_STOP;
}


#ifdef __DVR_BASIC__	
unsigned long  CCfgNetworkManDlg::OnClickNetEmail()
{
	if(m_cfgNetEmailDlg.IsModify())
	{
		EnableOkBtn(true, true);
	}
	return KEY_VIRTUAL_MSG_STOP;
}
#endif

#ifdef __ALARM_TO_UPDATE_FTP_VIDEO__
unsigned long CCfgNetworkManDlg::OnClickNetFtp()
{
	if(m_cfgNetFtpDlg.IsModify())
	{
		EnableOkBtn(true, true);
	}
	return KEY_VIRTUAL_MSG_STOP;
}
#endif


#ifdef __MOBILE_DVR__

unsigned long CCfgNetworkManDlg::OnClickNetWireless()
{
	if (m_cfgNetWirelessDlg.IsModify())
	{
		EnableOkBtn(true, true);
	}

	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CCfgNetworkManDlg::OnClickNetServer()
{
	if (m_cfgNetServerDlg.IsModify())
	{
		EnableOkBtn(true, true);
	}

	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CCfgNetworkManDlg::OnClickNetWifi()
{
	if (m_cfgNetWifiDlg.IsModify())
	{
		EnableOkBtn(true, true);
	}

	return KEY_VIRTUAL_MSG_STOP;
}

#endif

#ifdef __SUPPORT_HRSST__
unsigned long CCfgNetworkManDlg::OnClickNetPhone()
{
	if (m_cfgNetPhoneDlg.IsModify())
	{
		EnableOkBtn(true, true);
	}

	return KEY_VIRTUAL_MSG_STOP;
}
#endif

unsigned long CCfgNetworkManDlg::OnClickNetAutoReport()
{
	if (m_cfgNetAutoReportDlg.IsModify())
	{
		EnableOkBtn(true, true);
	}

	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CCfgNetworkManDlg::PostSave()
{
#if defined (__SUPPORT_3G__)
	if((m_curPage == PAGE_NETWORK) && (dynamic_cast<CCfgNetworkExDlg *>(GetPage(m_curPage))->GetCurPage() == m_cfgNetworkExDlg.PAGE_WIRELESS) && !g_curNet3G && GetLast3gStatus())
	{
		MESSAGE_DATA msgData;
		CConfigEx *temp_pConfig = CConfigEx::Instance();
		CMyList <NCFG_ITEM_ID> temp_itemList;
		unsigned char *temp_pData = NULL; 
		unsigned long temp_dataLen = 0;

		CMessageMan::Instance()->SendMsgDataToServer( LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_EFFECT_NETWORK, temp_pData, temp_dataLen, msgData );
		delete [] temp_pData;
		temp_pData = NULL;

		Repaint();
	}
	
	Set3gStatus(g_curNet3G);
#endif

	return 0;
}

//////////////////////////////////////////////////////////////////////////
// __DVR_ANCHOR__
//////////////////////////////////////////////////////////////////////////

CCfgNetworkManAnchorDlg::CCfgNetworkManAnchorDlg()
{

}

CCfgNetworkManAnchorDlg::~CCfgNetworkManAnchorDlg()
{

}

void CCfgNetworkManAnchorDlg::OnInitial()
{
	GUI::CDialog::OnInitial();
	SetImage(BMP_MEM_DLG_NORMAL, false);
	SetCaption(m_pStrTable->String(E_STR_ID_TITLE_NETWORK), false);
	//////////////////////////////////////////////////////////////////////////
	unsigned short x = 200;
	unsigned short y = m_length[TITLE_HEIGHT] + m_length[DLG_OFFSET_TOP] + 25;
	unsigned short x_off = 60, y_off = 10;
	unsigned short cx = m_length[ANCHOR_WIDTH], cy = m_length[ANCHOR_HEIGHT];

	if(CCfgNetworkManDlg::PAGE_COUNT >= 5)
	{
		x = 80;
	}

	int xFocus = 0;
	int yFocus = 1;
	int row = 0;

	row++;
	m_anNetwork.Create(GUI::CWnd::GetRandWndID(), x, y, cx, cy, this, xFocus, yFocus++, m_pStrTable->String(E_STR_ID_NETWORK));
	m_anNetwork.SetImage(BMP_ANCHOR_NETWORK, BMP_ANCHOR_NETWORK_F, BMP_ANCHOR_NETWORK);

	if(row >= 5)
	{
		row = 0;
		x = m_rect.Width() / 2 + 40;
		y = m_length[TITLE_HEIGHT] + m_length[DLG_OFFSET_TOP] + 25 - 60;
	}
	
	if (CCfgDlg::s_loginSuccessReply.productInfo.videoInputNum > 0)
	{
		row++;
		y += 60;
		m_anNetworkStream.Create(GUI::CWnd::GetRandWndID(), x, y, cx, cy, this, xFocus, yFocus++, m_pStrTable->String(E_STR_ID_NETWORK_STREAM));
		m_anNetworkStream.SetImage(BMP_ANCHOR_SUB_STREAM, BMP_ANCHOR_SUB_STREAM_F, BMP_ANCHOR_SUB_STREAM);
		if(row >= 5)
		{
			row = 0;
			x = m_rect.Width() / 2 + 40;
			y = m_length[TITLE_HEIGHT] + m_length[DLG_OFFSET_TOP] + 25 - 60;
		}
	}

#ifdef __DVR_BASIC__
	row++;
	y += 60;
	m_anNetEmail.Create(GUI::CWnd::GetRandWndID(), x, y, cx, cy, this, xFocus, yFocus++, m_pStrTable->String(E_STR_ID_EMAIL));
	m_anNetEmail.SetImage(BMP_ANCHOR_EMAIL, BMP_ANCHOR_EMAIL_F, BMP_ANCHOR_EMAIL);
	if(row >= 5)
	{
		row = 0;
		x = m_rect.Width() / 2 + 40;
		y = m_length[TITLE_HEIGHT] + m_length[DLG_OFFSET_TOP] + 25 - 60;
	}
#endif

#ifdef __ALARM_TO_UPDATE_FTP_VIDEO__
	row++;
	y += 60;
	m_anNetFtp.Create(GUI::CWnd::GetRandWndID(), x, y, cx, cy, this, xFocus, yFocus++, m_pStrTable->String(E_STR_ID_FTP));
	m_anNetFtp.SetImage(BMP_ANCHOR_FTP, BMP_ANCHOR_FTP_F, BMP_ANCHOR_FTP);
	if(row >= 5)
	{
		row = 0;
		x = m_rect.Width() / 2 + 40;
		y = m_length[TITLE_HEIGHT] + m_length[DLG_OFFSET_TOP] + 25 - 60;
	}
#endif

#ifdef __MOBILE_DVR__
	row++;
	y += 60;
	m_anNetWifi.Create(GUI::CWnd::GetRandWndID(), x, y, cx, cy, this, xFocus, yFocus++, m_pStrTable->String(E_STR_ID_NETWORK_WIFI));
	m_anNetWifi.SetImage(BMP_ANCHOR_CONFIG, BMP_ANCHOR_CONFIG_F, BMP_ANCHOR_CONFIG);
	if(row >= 5)
	{
		row = 0;
		x = m_rect.Width() / 2 + 40;
		y = m_length[TITLE_HEIGHT] + m_length[DLG_OFFSET_TOP] + 25 - 60;
	}

	row++;
	y += 60;
	m_anNetWireless.Create(GUI::CWnd::GetRandWndID(), x, y, cx, cy, this, xFocus, yFocus++, m_pStrTable->String(E_STR_ID_NETWORK_WIRELESS));
	m_anNetWireless.SetImage(BMP_ANCHOR_CONFIG, BMP_ANCHOR_CONFIG_F, BMP_ANCHOR_CONFIG);

	if(row >= 5)
	{
		row = 0;
		x = m_rect.Width() / 2 + 40;
		y = m_length[TITLE_HEIGHT] + m_length[DLG_OFFSET_TOP] + 25 - 60;
	}

	row++;
	y += 60;
	m_anNetServer.Create(GUI::CWnd::GetRandWndID(), x, y, cx, cy, this, xFocus, yFocus++, m_pStrTable->String(E_STR_ID_NETWORK_SERVER));
	m_anNetServer.SetImage(BMP_ANCHOR_CONFIG, BMP_ANCHOR_CONFIG_F, BMP_ANCHOR_CONFIG);
	if(row >= 5)
	{
		row = 0;
		x = m_rect.Width() / 2 + 40;
		y = m_length[TITLE_HEIGHT] + m_length[DLG_OFFSET_TOP] + 25 - 60;
	}
#endif

#ifdef __SUPPORT_HRSST__
	row++;
	y += 60;
	m_anNetPhone.Create(GUI::CWnd::GetRandWndID(), x, y, cx, cy, this, xFocus, yFocus++, m_pStrTable->String(E_STR_ID_OTHERSETTINGS));
	m_anNetPhone.SetImage(BMP_ANCHOR_OTHER_SETTING, BMP_ANCHOR_OTHER_SETTING_F, BMP_ANCHOR_OTHER_SETTING);
	if(row >= 5)
	{
		row = 0;
		x = m_rect.Width() / 2 + 40;
		y = m_length[TITLE_HEIGHT] + m_length[DLG_OFFSET_TOP] + 25 - 60;
	}
#endif


#if !defined(__CUSTOM_BR27__) && !defined(__KR24_IN__)
	row++;
	y += 60;
	m_anNetAutoReport.Create(GUI::CWnd::GetRandWndID(), x, y, cx, cy, this, xFocus, yFocus++, m_pStrTable->String(E_STR_ID_NETWORK_SERVER));
	m_anNetAutoReport.SetImage(BMP_ANCHOR_NET_SERVER, BMP_ANCHOR_NET_SERVER_F, BMP_ANCHOR_NET_SERVER);
	if(row >= 5)
	{
		row = 0;
		x = m_rect.Width() / 2 + 40;
		y = m_length[TITLE_HEIGHT] + m_length[DLG_OFFSET_TOP] + 25 - 60;
	}
#endif

#if defined(__TVT_NAT_SOCK__)
	row++;
	y += 60;
	m_anNetworkNat.Create(GUI::CWnd::GetRandWndID(), x, y, cx, cy, this, xFocus, yFocus++, m_pStrTable->String(E_STR_ID_NAT));
	m_anNetworkNat.SetImage(BMP_ANCHOR_OTHER_SETTING, BMP_ANCHOR_OTHER_SETTING_F, BMP_ANCHOR_OTHER_SETTING);
	if(row >= 5)
	{
		row = 0;
		x = m_rect.Width() / 2 + 40;
		y = m_length[TITLE_HEIGHT] + m_length[DLG_OFFSET_TOP] + 25 - 60;
	}
#endif

#if defined(__TVT_PUSH_SOCK__)
	row++;
	y += 60;
	m_anNetworkPush.Create(GUI::CWnd::GetRandWndID(), x, y, cx, cy, this, xFocus, yFocus++, m_pStrTable->String(E_STR_ID_PUSH));
	m_anNetworkPush.SetImage(BMP_ANCHOR_OTHER_SETTING, BMP_ANCHOR_OTHER_SETTING_F, BMP_ANCHOR_OTHER_SETTING);
	if(row >= 5)
	{
		row = 0;
		x = m_rect.Width() / 2 + 40;
		y = m_length[TITLE_HEIGHT] + m_length[DLG_OFFSET_TOP] + 25 - 60;
	}
#endif

	row++;
	y += 60;
	m_anNetMoreConfig.Create(GUI::CWnd::GetRandWndID(), x, y, cx, cy, this, xFocus, yFocus++, m_pStrTable->String(E_STR_ID_OTHERSETTINGS));
	m_anNetMoreConfig.SetImage(BMP_ANCHOR_OTHER_SETTING, BMP_ANCHOR_OTHER_SETTING_F, BMP_ANCHOR_OTHER_SETTING);
	if(row >= 5)
	{
		row = 0;
		x = m_rect.Width() / 2 + 40;
		y = m_length[TITLE_HEIGHT] + m_length[DLG_OFFSET_TOP] + 25 - 60;
	}

	row++;
	y += 60;
	m_anExit.Create(GUI::CWnd::GetRandWndID(), x, y, cx, cy, this, xFocus, yFocus++, m_pStrTable->String(E_STR_ID_EXIT));
	m_anExit.SetImage(BMP_ANCHOR_EXIT, BMP_ANCHOR_EXIT_F, BMP_ANCHOR_EXIT);
}

BEGIN_MSG_MAP(CCfgNetworkManAnchorDlg, GUI::CDialog)
	ON_MSG(m_anNetwork.GetID(), KEY_VIRTUAL_ENTER, OnAnchorNetwork)
	ON_MSG(m_anNetworkStream.GetID(), KEY_VIRTUAL_ENTER, OnAnchorNetworkStream)
	ON_MSG(m_anNetEmail.GetID(), KEY_VIRTUAL_ENTER, OnAnchorNetEmail)
#ifdef __ALARM_TO_UPDATE_FTP_VIDEO__
	ON_MSG(m_anNetFtp.GetID(), KEY_VIRTUAL_ENTER, OnAnchorNetFtp)
#endif
	ON_MSG(m_anNetWifi.GetID(), KEY_VIRTUAL_ENTER, OnAnchorNetWifi)
	ON_MSG(m_anNetWireless.GetID(), KEY_VIRTUAL_ENTER, OnAnchorNetWireless)
	ON_MSG(m_anNetServer.GetID(), KEY_VIRTUAL_ENTER, OnAnchorNetServer)
	ON_MSG(m_anNetPhone.GetID(), KEY_VIRTUAL_ENTER, OnAnchorNetPhone)
#if defined(__TVT_NAT_SOCK__)
	ON_MSG(m_anNetworkNat.GetID(), KEY_VIRTUAL_ENTER, OnAnchorNetworkNat)
#endif
#if defined(__TVT_PUSH_SOCK__)
	ON_MSG(m_anNetworkPush.GetID(), KEY_VIRTUAL_ENTER, OnAnchorNetworkPush)
#endif
	ON_MSG(m_anNetMoreConfig.GetID(), KEY_VIRTUAL_ENTER, OnAnchorNetMoreConfig)
	ON_MSG(m_anNetAutoReport.GetID(), KEY_VIRTUAL_ENTER, OnAnchorNetAutoReport)
	ON_MSG(m_anExit.GetID(), KEY_VIRTUAL_ENTER, OnExit)
END_MSG_MAP()

unsigned long CCfgNetworkManAnchorDlg::OnAnchorNetwork()
{
	m_cfgNetworkManDlg.SetAnchorPage(CCfgNetworkManDlg::PAGE_NETWORK);
	m_cfgNetworkManDlg.DoModal(this, CENTER_POS, CENTER_POS, MAX_SCREEN_X, MAX_SCREEN_Y, false);

	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CCfgNetworkManAnchorDlg::OnAnchorNetworkStream()
{
	m_cfgNetworkManDlg.SetAnchorPage(CCfgNetworkManDlg::PAGE_NETWORK_STREAM);
	m_cfgNetworkManDlg.DoModal(this, CENTER_POS, CENTER_POS, MAX_SCREEN_X, MAX_SCREEN_Y, false);

	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CCfgNetworkManAnchorDlg::OnAnchorNetEmail()
{
#ifdef __DVR_BASIC__
	m_cfgNetworkManDlg.SetAnchorPage(CCfgNetworkManDlg::PAGE_NETWORK_EMAIL);
	m_cfgNetworkManDlg.DoModal(this, CENTER_POS, CENTER_POS, MAX_SCREEN_X, MAX_SCREEN_Y, false);
#endif

	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CCfgNetworkManAnchorDlg::OnAnchorNetFtp()
{
#ifdef __ALARM_TO_UPDATE_FTP_VIDEO__
	m_cfgNetworkManDlg.SetAnchorPage(CCfgNetworkManDlg::PAGE_NETWORK_FTP);
	m_cfgNetworkManDlg.DoModal(this, CENTER_POS, CENTER_POS, MAX_SCREEN_X, MAX_SCREEN_Y, false);
#endif

	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CCfgNetworkManAnchorDlg::OnAnchorNetWifi()
{
#ifdef __MOBILE_DVR__
	m_cfgNetworkManDlg.SetAnchorPage(CCfgNetworkManDlg::PAGE_NETWORK_WIFI);
	m_cfgNetworkManDlg.DoModal(this, CENTER_POS, CENTER_POS, MAX_SCREEN_X, MAX_SCREEN_Y, false);
#endif
	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CCfgNetworkManAnchorDlg::OnAnchorNetWireless()
{
#ifdef __MOBILE_DVR__
	m_cfgNetworkManDlg.SetAnchorPage(CCfgNetworkManDlg::PAGE_NETWORK_WIRELESS);
	m_cfgNetworkManDlg.DoModal(this, CENTER_POS, CENTER_POS, MAX_SCREEN_X, MAX_SCREEN_Y, false);
#endif
	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CCfgNetworkManAnchorDlg::OnAnchorNetServer()
{
#ifdef __MOBILE_DVR__
	m_cfgNetworkManDlg.SetAnchorPage(CCfgNetworkManDlg::PAGE_NETWORK_SERVER);
	m_cfgNetworkManDlg.DoModal(this, CENTER_POS, CENTER_POS, MAX_SCREEN_X, MAX_SCREEN_Y, false);
#endif
	return KEY_VIRTUAL_MSG_STOP;
}


unsigned long CCfgNetworkManAnchorDlg::OnAnchorNetPhone()
{
#ifdef __SUPPORT_HRSST__
	m_cfgNetworkManDlg.SetAnchorPage(CCfgNetworkManDlg::PAGE_NETWORK_PHONE);
	m_cfgNetworkManDlg.DoModal(this, CENTER_POS, CENTER_POS, MAX_SCREEN_X, MAX_SCREEN_Y, false);
#endif

	return KEY_VIRTUAL_MSG_STOP;
}



unsigned long CCfgNetworkManAnchorDlg::OnAnchorNetAutoReport()
{
#if !defined(__CUSTOM_BR27__) && !defined(__KR24_IN__)
	m_cfgNetworkManDlg.SetAnchorPage(CCfgNetworkManDlg::PAGE_NETWORK_AUTOREPORT);
	m_cfgNetworkManDlg.DoModal(this, CENTER_POS, CENTER_POS, MAX_SCREEN_X, MAX_SCREEN_Y, false);
#endif

	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CCfgNetworkManAnchorDlg::OnAnchorNetMoreConfig()
{
	m_cfgNetworkManDlg.SetAnchorPage(CCfgNetworkManDlg::PAGE_NETWORK_MORECONFIG);
	m_cfgNetworkManDlg.DoModal(this, CENTER_POS, CENTER_POS, MAX_SCREEN_X, MAX_SCREEN_Y, false);

	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CCfgNetworkManAnchorDlg::OnAnchorNetworkNat()
{
#if defined(__TVT_NAT_SOCK__)
	m_cfgNetworkManDlg.SetAnchorPage(CCfgNetworkManDlg::PAGE_NETWORK_NAT);
	m_cfgNetworkManDlg.DoModal(this, CENTER_POS, CENTER_POS, MAX_SCREEN_X, MAX_SCREEN_Y, false);
#endif
	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CCfgNetworkManAnchorDlg::OnAnchorNetworkPush()
{
#if defined(__TVT_PUSH_SOCK__)
	m_cfgNetworkManDlg.SetAnchorPage(CCfgNetworkManDlg::PAGE_NETWORK_PUSH);
	m_cfgNetworkManDlg.DoModal(this, CENTER_POS, CENTER_POS, MAX_SCREEN_X, MAX_SCREEN_Y, false);
#endif
	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CCfgNetworkManAnchorDlg::OnExit()
{
	EndDialog(DLG_ESC);
	return KEY_VIRTUAL_MSG_STOP;
}

//end

