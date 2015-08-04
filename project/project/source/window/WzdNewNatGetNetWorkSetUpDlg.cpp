/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : ZXX
** Date         : 2013-11-26
** Name         : WzdNewNatGetNetWorkSetUpDlg.h
** Version      : 1.0
** Description  : 向导管理对话框
** Modify Record:
***********************************************************************/

#include "WzdNewNatGetNetWorkSetUpDlg.h"
#include "MessageMan.h"

#define IPADDRESS_FIELD(ip, field) static_cast<BYTE>(((ip>>((field)*8))<<24)>>24)
CWzdNewNatGetNetWorkSetUpDlg::CWzdNewNatGetNetWorkSetUpDlg()
{
	m_toprow = 0;
	m_bEnableNatFlag = true;
}

CWzdNewNatGetNetWorkSetUpDlg::~CWzdNewNatGetNetWorkSetUpDlg()
{

}

void CWzdNewNatGetNetWorkSetUpDlg::SetWzdInfo(NAT_WIZARD_INFO* pWzdInfo)
{
	m_pNetWzdInfo = pWzdInfo;
	
}

void CWzdNewNatGetNetWorkSetUpDlg::InitCtrlInfo()
{

}

void CWzdNewNatGetNetWorkSetUpDlg::ShowWzdData(bool bRefresh)
{
	GUI::CButton *pButton = NULL;
	bool bret = false;

	if(1 == m_pNetWzdInfo->ShowWzd)
	{
		m_checkCtrl.SetCheck(true,false);
	}
	else
	{
		m_checkCtrl.SetCheck(false,false);
	}


	//设置键的状态
	pButton =  dynamic_cast<GUI::CButton*>(GetTopestWnd()->GetCtrl(CTRLID_DEFAULT_BTN_OK));
	//获取并显示ip
	GUI::CListCtrl& list = m_netList;
	GUI::CStatic * pStatic = NULL;
	bret = GetIp();
	if(NULL != pButton)
	{
		if(bret)
		{
			pButton->SetEnable(true,false);
		}
		else
		{
			pButton->SetEnable(false,false);
		}

	}
	if(bret)
	{
		pStatic = dynamic_cast<GUI::CStatic*>(list.GetItemWnd(ROW_NET_STATUS, 1));
		pStatic->SetCaption(m_pStrTable->String(E_STR_ID_US02_WZD_DONE),false);
		pStatic = dynamic_cast<GUI::CStatic*>(list.GetItemWnd(ROW_GETIP_STATUS, 1));
		pStatic->SetCaption(m_pStrTable->String(E_STR_ID_US02_WZD_DONE),false);
		pStatic = dynamic_cast<GUI::CStatic*>(list.GetItemWnd(ROW_DVRIP, 1));
		char ipArr[32]={0};
		GetStaticIp(m_pNetWzdInfo->Addr_Info.staticIP,ipArr);
		pStatic ->SetCaption(ipArr,false);
		string str;
		// str.append("Checking Network IP address......Success");
		str.append(m_pStrTable->String(E_STR_ID_US02_NEW_WZD_NETWORK_00));
		str.append("\\n");
		// str.append("Your DVR is connected to your network.");
		str.append(m_pStrTable->String(E_STR_ID_US02_NEW_WZD_NETWORK_01));
		str.append("\\n");
		// str.append("The local IP address for this DVR is ");
		str.append(m_pStrTable->String(E_STR_ID_US02_NEW_WZD_NETWORK_02));
		str.append(" http://");
		str.append(ipArr);
		str.append(":");
		char port[5]={0};
		snprintf(port,sizeof(port),"%d",m_pNetWzdInfo->httpPort);
		str.append(port);
		str.append("\\n");
		// str.append("If you will be connecting to your DVR away from your local network (over the Internet), please click Next to continue with the Q-See Scan N'VIEW.");
		str.append(m_pStrTable->String(E_STR_ID_US02_NEW_WZD_NETWORK_03));
		str.append("\\n");
		str.append(" \\n");
		str.append(" \\n");
		// str.append("If you will ONLY be connecting to your DVR through your local network, select Local Connection Only below and then click Next to continue. Otherwise, just click Next to continue.");
		str.append(m_pStrTable->String(E_STR_ID_US02_NEW_WZD_NETWORK_04));
		m_netNoteRichText.SetText(str,false);
		unsigned short maxTextRow = m_netNoteRichText.GetTextRow()-1;//文本显示需要多少行
		if (800 >= m_width)//800x600以上分辨率不需要滚动条
		{
			m_scroll_v.SetRange(0,maxTextRow);
			m_scroll_v.SetCurPos(0,false);
		}
		//成功,读取穿透的状态，把页面选择的默认值和穿透状态挂钩
		m_rdNat.Show(true,false);
		m_rdLocal.Show(true,false);
		m_bEnableNatFlag = m_pNetWzdInfo->natInfo.bStartNat;
		if(m_bEnableNatFlag)
		{
			m_rdNat.SetCheck(true,false);
		}
		else
		{
			m_rdLocal.SetCheck(true,false);
		}
		
	}
	else
	{
		pStatic = dynamic_cast<GUI::CStatic*>(list.GetItemWnd(ROW_NET_STATUS, 1));
		pStatic->SetCaption(m_pStrTable->String(E_STR_ID_US02_WZD_FAILED),false);
		pStatic = dynamic_cast<GUI::CStatic*>(list.GetItemWnd(ROW_GETIP_STATUS, 1));
		pStatic->SetCaption(m_pStrTable->String(E_STR_ID_US02_WZD_FAILED),false);
		pStatic = dynamic_cast<GUI::CStatic*>(list.GetItemWnd(ROW_DVRIP, 1));
		pStatic->SetCaption(m_pStrTable->String(E_STR_ID_US02_WZD_FAILED),false);

		string str;
		// str.append("Checking Network IP address......Failure");
		str.append(m_pStrTable->String(E_STR_ID_NAT_WZD_NETWORK_03));
		str.append("\\n");
		// str.append("Your DVR is not connected to your network.");
		str.append(m_pStrTable->String(E_STR_ID_NAT_WZD_NETWORK_04));
		str.append("\\n");
		// str.append("TROUBLESHOOTING:");
		str.append(m_pStrTable->String(E_STR_ID_NAT_WZD_NETWORK_05));
		str.append("\\n");
		// str.append("Please ensure that the Network Cable is properly connecting your DVR to your Router.");
		str.append(m_pStrTable->String(E_STR_ID_NAT_WZD_NETWORK_06));
		str.append("\\n");
		// str.append("Please make sure that your Router has DHCP enabled. ");
		str.append(m_pStrTable->String(E_STR_ID_NAT_WZD_NETWORK_07));
		str.append("\\n");
		// str.append("(Consult your service provider or your router's manual).");
		str.append(m_pStrTable->String(E_STR_ID_NAT_WZD_NETWORK_08));
		str.append("\\n");
		// str.append("Alternative connection methods may be found on the Remote Monitoring Quick Start poster. ");
		// str.append("Additional resources may be found in Section 1.7 of the Remote Monitoring Guide on the CD. ");
		str.append(m_pStrTable->String(E_STR_ID_NAT_WZD_NETWORK_09));
		str.append(m_pStrTable->String(E_STR_ID_NAT_WZD_NETWORK_0a));
		str.append("\\n");
		// str.append("Click on Prev to go back and retry your connection. ");
		str.append(m_pStrTable->String(E_STR_ID_NAT_WZD_NETWORK_0b));
		m_netNoteRichText.SetText(str,false);
		unsigned short maxTextRow = m_netNoteRichText.GetTextRow()-1;//文本显示需要多少行
		if (800 >= m_width)//800x600以上分辨率不需要滚动条
		{
			m_scroll_v.SetRange(0,maxTextRow);
			m_scroll_v.SetCurPos(0,false);
		}


	}
}
void CWzdNewNatGetNetWorkSetUpDlg::GetWzdData(bool bNext)
{
	
}

bool CWzdNewNatGetNetWorkSetUpDlg::GetNatEnaleFlag()
{
	return m_bEnableNatFlag;
}
void CWzdNewNatGetNetWorkSetUpDlg::OnInitial()
{
	CWzdNatPageDlg::OnInitial();
	GUI::CListCtrl& list = m_netList;
	unsigned short x = m_length[DLG_OFFSET_SUB_LEFT], y = m_length[DLG_OFFSET_SUB_TOP];
	
	unsigned short listheight = 3 * m_length[LIST_ITEM_HEIGHT];
	unsigned short Richwidth = m_rect.Width() - 2 * m_length[DLG_OFFSET_SUB_LEFT] -CScrollCtrl::m_length[SCROLL_WIDTH] -5;
	unsigned short RichHeight = m_rect.Height()-listheight-m_length[CHECK_HEIGHT]-m_length[DLG_OFFSET_SUB_TOP]*2-5-m_length[CHECK_HEIGHT] - 2*m_length[BTN_HEIGHT];

	list.Create(GetRandWndID(), x, y, Richwidth, listheight, this, 0, 0, false);
	list.AddColumn(NULL,Richwidth/2);
	list.AddColumn(NULL, Richwidth / 2 );
	list.AddItem(ROW_NET_STATUS,0,m_pStrTable->String(E_STR_ID_US02_WZD_NETWORKSTATUS));
	list.AddItem(ROW_GETIP_STATUS,0,m_pStrTable->String(E_STR_ID_US02_WZD_GETIPADDRESS));
	list.AddItem(ROW_DVRIP,0,m_pStrTable->String(E_STR_ID_US02_WZD_DVRIPADDRESS));

	GUI::CStatic * pStatic = dynamic_cast<GUI::CStatic*>(list.AddItem(ROW_NET_STATUS, 1, WND_TYPE_STATIC,2*m_length[BTN_WIDTH],m_length[STATIC_HEIGHT]));
	pStatic = dynamic_cast<GUI::CStatic*>(list.AddItem(ROW_GETIP_STATUS, 1, WND_TYPE_STATIC,2*m_length[BTN_WIDTH],m_length[STATIC_HEIGHT]) );
	pStatic  = dynamic_cast<GUI::CStatic*>(list.AddItem(ROW_DVRIP, 1, WND_TYPE_STATIC,5*m_length[BTN_WIDTH],m_length[STATIC_HEIGHT]) );
	y = y+listheight+5;
	m_netNoteRichText.Create(GetRandWndID(),x,y,Richwidth-CScrollCtrl::m_length[SCROLL_WIDTH]-2,RichHeight,this);
	if (800 >= m_width)//800x600以上分辨率不需要滚动条
	{
		m_netNoteRichText.SetScrollShow(true);
		m_scroll_v.Create(GetRandWndID(), x+Richwidth -CScrollCtrl::m_length[SCROLL_WIDTH]-2,y,RichHeight-5,this,0,1,false);
	}
	
	x = m_length[DLG_OFFSET_SUB_LEFT]+Richwidth/2 - m_length[BTN_WIDTH];
	y += RichHeight+m_length[CHECK_HEIGHT];
	m_rdNat.Create(GetRandWndID(),x,y,5*m_length[BTN_WIDTH],m_length[RADIO_HEIGHT],this,0,2,&m_rdGroup);
	y += m_length[RADIO_HEIGHT];
	m_rdLocal.Create(GetRandWndID(),x,y,2*m_length[BTN_WIDTH],m_length[RADIO_HEIGHT],this,0,3,&m_rdGroup);
	m_rdGroup.AddRadio(&m_rdNat);
	m_rdGroup.AddRadio(&m_rdLocal);
	//m_rdNat.SetCaption("Local Connection with Remote Access",false,false);
	m_rdNat.SetCaption(m_pStrTable->String(E_STR_ID_US02_NEW_WZD_NETWORK_05), false, false);
	m_rdNat.SetData(NET_NAT);
	//m_rdLocal.SetCaption("Local Connection",false,false);
	m_rdLocal.SetCaption(m_pStrTable->String(E_STR_ID_US02_NEW_WZD_NETWORK_06), false, false);
	m_rdLocal.SetData(NET_LOCAL);
	m_rdNat.Show(false,false);
	m_rdLocal.Show(false,false);

	x = m_length[DLG_OFFSET_SUB_LEFT];
	y += m_length[RADIO_HEIGHT]+5;
	unsigned short cx = m_rect.Width() -  2 * x;
	unsigned short cy = m_length[CHECK_HEIGHT];
	m_checkCtrl.Create(GetRandWndID(),x,y,cx,cy,this,0,4);
	m_checkCtrl.SetCaption(m_pStrTable->String(E_STR_ID_US02_WZD_OPENAGAIN),false,false);
	
}

void CWzdNewNatGetNetWorkSetUpDlg::OnDestroy()
{
	
}

bool CWzdNewNatGetNetWorkSetUpDlg::GetIp()
{
	NETWORK_STATUS_INFO networkStatusInfo;
	memset( &networkStatusInfo, 0, sizeof(NETWORK_STATUS_INFO));
	MESSAGE_DATA msgData;
	memset(&msgData, 0, sizeof(msgData));
	CMessageMan::Instance()->SendMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_NETWORK_INFO, NULL, 0, msgData);

	networkStatusInfo = *reinterpret_cast<NETWORK_STATUS_INFO *>(msgData.pData+sizeof(PACKCMD));

	if (networkStatusInfo.IP == 0)
	{
		CMessageMan::Instance()->ReleaseMsgBuff(msgData);
		return false;
	}

	if(0 == networkStatusInfo.netstatus)
	{
		CMessageMan::Instance()->ReleaseMsgBuff(msgData);
		return false;
	}

	CMessageMan::Instance()->ReleaseMsgBuff(msgData);
	m_pNetWzdInfo->Addr_Info.bDHCP =1;
	m_pNetWzdInfo->Addr_Info.staticIP = networkStatusInfo.IP;
	m_pNetWzdInfo->Addr_Info.gateway = networkStatusInfo.gateway;
	m_pNetWzdInfo->Addr_Info.netMask = networkStatusInfo.subMask;
	return true;
}

void CWzdNewNatGetNetWorkSetUpDlg::GetStaticIp(unsigned long ip,char *pIP)
{
	char str[32]= {0};

	unsigned char ch;
	for(int i = 0; i < 4; i++)
	{
		ch = IPADDRESS_FIELD(ip, i);
		if(3 != i)
		{
			snprintf(str+strlen(str), 32, "%d.", ch);
		}
		else
		{
			snprintf(str+strlen(str), 32, "%d", ch);
		}
	}
	strcpy(pIP,str);
}

BEGIN_MSG_MAP(CWzdNewNatGetNetWorkSetUpDlg, CWzdNatPageDlg)
	ON_MSG(m_checkCtrl.GetID(), KEY_VIRTUAL_ENTER, OnCheck)
	ON_MSG_EX(m_scroll_v.GetID(), KEY_VIRTUAL_MSG_DEFAULT_TAG, OnScrollchange)
	ON_MSG_EX(m_scroll_v.GetID(),KEY_VIRTUAL_ENTER, OnScrollchange)
	ON_MSG_EX(m_scroll_v.GetID(),KEY_VIRTUAL_DRAG, OnScrollchange)
	ON_MSG_EX(m_scroll_v.GetID(),KEY_VIRTUAL_BLUP, OnScrollchange)
	ON_MSG(m_rdNat.GetID(),KEY_VIRTUAL_ENTER,OnClickRadioCtrl)
	ON_MSG(m_rdLocal.GetID(),KEY_VIRTUAL_ENTER,OnClickRadioCtrl)
END_MSG_MAP()

unsigned long  CWzdNewNatGetNetWorkSetUpDlg::OnCheck()
{
	if(m_checkCtrl.IsCheck())
	{
		m_pNetWzdInfo->ShowWzd = 1;
	}
	else
	{
		m_pNetWzdInfo->ShowWzd = 0;
	}
	return KEY_VIRTUAL_STOP;
}

unsigned long CWzdNewNatGetNetWorkSetUpDlg::OnScrollchange(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	int pos = m_scroll_v.GetPos();
	if (pos != m_toprow)
	{
		m_toprow = pos;
		m_netNoteRichText.SetCurRow(m_toprow);
		Repaint();
	}

	return KEY_VIRTUAL_MSG_STOP; 
}

unsigned long CWzdNewNatGetNetWorkSetUpDlg::OnClickRadioCtrl()
{
	GUI::CRadioCtrl* pRadioCtrl = m_rdGroup.GetCheckRadio();
	NET_SELECT click_select= static_cast<NET_SELECT>(pRadioCtrl->GetData());

	switch(click_select)
	{
	case NET_NAT:
		{
			m_bEnableNatFlag = true;
			m_rdNat.SetCheck(true,false);
		}
		break;
	case NET_LOCAL:
		{
			m_bEnableNatFlag = false;
			m_rdLocal.SetCheck(true,false);
		}
		break;
	default:
		{
			assert(false);
			m_bEnableNatFlag = true;
			m_rdNat.SetCheck(true,false);
		}
		break;
	}
	//读取页面的选择，并保存到m_pNetWzdInfo
	m_pNetWzdInfo->natInfo.bStartNat = m_bEnableNatFlag;
	return KEY_VIRTUAL_MSG_CONTINUE;
}
//end
