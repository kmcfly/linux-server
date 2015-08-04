/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : ZXX
** Date         : 2012-09-24
** Name         : WzdUS02GetRouterIpDlg.cpp
** Version      : 1.0
** Description  : 向导页面对话框
** Modify Record:
***********************************************************************/
#include "WzdUS02GetRouterIpDlg.h"
#include "MessageMan.h"
CWzdUS02GetRouterIpDlg::CWzdUS02GetRouterIpDlg()
{
	memset(m_RouterIp,0,sizeof(m_RouterIp));
}

CWzdUS02GetRouterIpDlg::~CWzdUS02GetRouterIpDlg()
{

}

void CWzdUS02GetRouterIpDlg::SetWzdInfo(US02_WzdInfo* pWzdInfo)
{
	m_pWzdInfo = pWzdInfo;
}

void CWzdUS02GetRouterIpDlg::ShowWzdData(bool bRefresh)
{
	GUI::CButton *pButton = NULL;
	string str;
	char ipText[32]={0};
	bool bret = false;

	if(1 == m_pWzdInfo->ShowWzd)
	{
		m_checkCtrl.SetCheck(true,false);
	}
	else
	{
		m_checkCtrl.SetCheck(false,false);
	}

	pButton =  dynamic_cast<GUI::CButton*>(GetTopestWnd()->GetCtrl(CTRLID_DEFAULT_BTN_OK));
	bret = GetRouterIp(ipText);

	if(bret)
	{
		pButton->SetEnable(true,false);
		strcpy(m_RouterIp,ipText);
		str.append(m_pStrTable->String(E_STR_ID_US02_WZD_CHECKIPSUCC));
		str.append("\\n");
		str.append(m_pStrTable->String(E_STR_ID_US02_WZD_NINETIP01));
		str.append(ipText);
		str.append("\\n");
		str.append(m_pStrTable->String(E_STR_ID_US02_WZD_NINETIP02));
		str.append("\\n");
		str.append(m_pStrTable->String(E_STR_ID_US02_WZD_SIXTIP02));
		str.append("\\n");
		str.append(m_pStrTable->String(E_STR_ID_US02_WZD_SIXTIP03));
		str.append("\\n");
		str.append("http://");
		str.append(ipText);
		char port[5]={0};
		snprintf(port,sizeof(port),":%d",m_pWzdInfo->httpPort);
		str.append(port);
	}
	else
	{
		pButton->SetEnable(false,false);
		str.append(m_pStrTable->String(E_STR_ID_US02_WZD_CHECKIPERROR));
	}
	m_richTextCtrl.SetText(str,false);
}

void CWzdUS02GetRouterIpDlg::GetWzdData(bool bNext)
{
	strcpy(m_pWzdInfo->RouterIp,m_RouterIp);
}

void CWzdUS02GetRouterIpDlg::OnInitial()
{
	CWzdUS02PageDlg::OnInitial();
	unsigned long x = m_length[DLG_OFFSET_SUB_LEFT];
	unsigned long y = m_length[DLG_OFFSET_SUB_TOP];

	unsigned long cx = m_rect.Width() -  2 * x;
	unsigned long cy = m_rect.Height() -  2 * y;

	m_richTextCtrl.Create(GetRandWndID(), x, y, cx, cy, this);

	x = m_length[DLG_OFFSET_SUB_LEFT];
	y = m_rect.Height() - 3 * m_length[DLG_OFFSET_SUB_TOP];

	cx = m_rect.Width() -  2 * x;
	cy = m_length[CHECK_HEIGHT];

	m_checkCtrl.Create(GetRandWndID(),x,y,cx,cy,this,0,1);
	m_checkCtrl.SetCaption(m_pStrTable->String(E_STR_ID_US02_WZD_OPENAGAIN),false,false);
}

void CWzdUS02GetRouterIpDlg::OnDestroy()
{

}

bool CWzdUS02GetRouterIpDlg::GetRouterIp(char *pIP)
{
	MESSAGE_DATA msgData;
	memset(&msgData, 0, sizeof(msgData));
	CMessageMan::Instance()->SendMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_ROUTERIP, NULL,0, msgData);
	
	char *pTmp = reinterpret_cast<char *>(msgData.pData+sizeof(PACKCMD));
	strcpy(pIP,pTmp);
	PACKCMD *pCmd = reinterpret_cast<PACKCMD *>(msgData.pData);
	if( CMD_REPLY_GETROUTERIP_SUCC == static_cast<NET_PROTOCOL>(pCmd->cmdType))
	{
		CMessageMan::Instance()->ReleaseMsgBuff(msgData);
		return true;
	}
	
	CMessageMan::Instance()->ReleaseMsgBuff(msgData);
	return false;
}

BEGIN_MSG_MAP(CWzdUS02GetRouterIpDlg, CWzdUS02PageDlg)
	ON_MSG(m_checkCtrl.GetID(), KEY_VIRTUAL_ENTER, OnCheck)
END_MSG_MAP()

unsigned long  CWzdUS02GetRouterIpDlg::OnCheck()
{
	if(m_checkCtrl.IsCheck())
	{
		m_pWzdInfo->ShowWzd = 1;
	}
	else
	{
		m_pWzdInfo->ShowWzd = 0;
	}
	return KEY_VIRTUAL_STOP;
}
