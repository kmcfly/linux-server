/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : ZXX
** Date         : 2012-09-19
** Name         : WzdUS02GetNetWorkSetUpDlg.h
** Version      : 1.0
** Description  : 向导管理对话框
** Modify Record:
***********************************************************************/

#include "WzdUS02GetNetWorkSetUpDlg.h"
#include "MessageMan.h"

#define IPADDRESS_FIELD(ip, field) static_cast<BYTE>(((ip>>((field)*8))<<24)>>24)
CWzdUS02GetNetWorkSetUpDlg::CWzdUS02GetNetWorkSetUpDlg()
{

}

CWzdUS02GetNetWorkSetUpDlg::~CWzdUS02GetNetWorkSetUpDlg()
{

}

void CWzdUS02GetNetWorkSetUpDlg::SetWzdInfo(US02_WzdInfo* pWzdInfo)
{
	m_pNetWzdInfo = pWzdInfo;
	memset(&(m_pNetWzdInfo->Addr_Info),0,sizeof(NETWORK_ADDR_INFO));
}

void CWzdUS02GetNetWorkSetUpDlg::InitCtrlInfo()
{
	
}

void CWzdUS02GetNetWorkSetUpDlg::ShowWzdData(bool bRefresh)
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
		str.append(m_pStrTable->String(E_STR_ID_US02_WZD_SIXTIP01));
		str.append("\\n");
		str.append(m_pStrTable->String(E_STR_ID_US02_WZD_SIXTIP02));
		str.append("\\n");
		str.append(m_pStrTable->String(E_STR_ID_US02_WZD_SIXTIP03));
		str.append("\\n");
		str.append("http://");
		str.append(ipArr);
		str.append(":");
		char port[5]={0};
		snprintf(port,sizeof(port),"%d",m_pNetWzdInfo->httpPort);
		str.append(port);
		m_netNoteRichText.SetText(str,false);
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
		str.append(m_pStrTable->String(E_STR_ID_US02_WZD_Note));
		str.append("\\n");
		str.append(m_pStrTable->String(E_STR_ID_US02_WZD_SIXERRTIP01));
		str.append("\\n");
		str.append(m_pStrTable->String(E_STR_ID_US02_WZD_SIXERRTIP02));
		str.append("\\n");
		str.append(m_pStrTable->String(E_STR_ID_US02_WZD_SIXERRTIP03));
		m_netNoteRichText.SetText(str,false);

	}
}
void CWzdUS02GetNetWorkSetUpDlg::GetWzdData(bool bNext)
{
	
}

void CWzdUS02GetNetWorkSetUpDlg::OnInitial()
{
	CWzdUS02PageDlg::OnInitial();
	GUI::CListCtrl& list = m_netList;
	unsigned short x = m_length[DLG_OFFSET_SUB_LEFT], y = m_length[DLG_OFFSET_SUB_TOP];
	unsigned short width = m_rect.Width() - 2 * m_length[DLG_OFFSET_SUB_LEFT];
	unsigned short height = 3 * m_length[LIST_ITEM_HEIGHT];
	unsigned short offsetEdge = 0;

	list.Create(GetRandWndID(), x, y, width, height, this, 0, 0, false);
	list.AddColumn(NULL,width/2);
	list.AddColumn(NULL, width / 2 );
	list.AddItem(ROW_NET_STATUS,0,m_pStrTable->String(E_STR_ID_US02_WZD_NETWORKSTATUS));
	list.AddItem(ROW_GETIP_STATUS,0,m_pStrTable->String(E_STR_ID_US02_WZD_GETIPADDRESS));
	list.AddItem(ROW_DVRIP,0,m_pStrTable->String(E_STR_ID_US02_WZD_DVRIPADDRESS));

	GUI::CStatic * pStatic = dynamic_cast<GUI::CStatic*>(list.AddItem(ROW_NET_STATUS, 1, WND_TYPE_STATIC,2*m_length[BTN_WIDTH],m_length[STATIC_HEIGHT]));
	pStatic = dynamic_cast<GUI::CStatic*>(list.AddItem(ROW_GETIP_STATUS, 1, WND_TYPE_STATIC,2*m_length[BTN_WIDTH],m_length[STATIC_HEIGHT]) );
	pStatic  = dynamic_cast<GUI::CStatic*>(list.AddItem(ROW_DVRIP, 1, WND_TYPE_STATIC,5*m_length[BTN_WIDTH],m_length[STATIC_HEIGHT]) );
	
	m_netNoteRichText.Create(GetRandWndID(),x,y+height+15,width,m_rect.Height()-height,this);
	
	x = m_length[DLG_OFFSET_SUB_LEFT];
	y = m_rect.Height() - 3 * m_length[DLG_OFFSET_SUB_TOP];

	unsigned short cx = m_rect.Width() -  2 * x;
	unsigned short cy = m_length[CHECK_HEIGHT];

	m_checkCtrl.Create(GetRandWndID(),x,y,cx,cy,this,0,1);
	m_checkCtrl.SetCaption(m_pStrTable->String(E_STR_ID_US02_WZD_OPENAGAIN),false,false);
}

void CWzdUS02GetNetWorkSetUpDlg::OnDestroy()
{
	memset(&(m_pNetWzdInfo->Addr_Info),0,sizeof(NETWORK_ADDR_INFO));

}

bool CWzdUS02GetNetWorkSetUpDlg::GetIp()
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

void CWzdUS02GetNetWorkSetUpDlg::GetStaticIp(unsigned long ip,char *pIP)
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

BEGIN_MSG_MAP(CWzdUS02GetNetWorkSetUpDlg, CWzdUS02PageDlg)
	ON_MSG(m_checkCtrl.GetID(), KEY_VIRTUAL_ENTER, OnCheck)
END_MSG_MAP()

unsigned long  CWzdUS02GetNetWorkSetUpDlg::OnCheck()
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
//end
