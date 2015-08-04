/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : ZXX
** Date         : 2012-09-19
** Name         : WzdUS02CheckUpnpDlg.cpp
** Version      : 1.0
** Description  : 向导管理对话框
** Modify Record:
***********************************************************************/

#include "WzdUS02CheckUpnpDlg.h"
#include "MessageMan.h"
CWzdUS02CheckUpnpDlg::CWzdUS02CheckUpnpDlg()
{

}

CWzdUS02CheckUpnpDlg::~CWzdUS02CheckUpnpDlg()
{

}

void CWzdUS02CheckUpnpDlg::SetWzdInfo(US02_WzdInfo* pWzdInfo)
{
	m_pWzdInfo = pWzdInfo;
}

void CWzdUS02CheckUpnpDlg::GetWzdData(bool bNext)
{
	
}

void CWzdUS02CheckUpnpDlg::ShowWzdData(bool bRefresh)
{
	GUI::CButton *pButton = NULL;
	bool bCheckUpnp = false;
	string str;
	if(1 == m_pWzdInfo->ShowWzd)
	{
		m_checkCtrl.SetCheck(true,false);
	}
	else
	{
		m_checkCtrl.SetCheck(false,false);
	}


	for(int i=0;i<5;i++)
	{
		if(CheckUpnp())
		{
			bCheckUpnp = true;
			break;
		}
	}
	
	pButton =  dynamic_cast<GUI::CButton*>(GetTopestWnd()->GetCtrl(CTRLID_DEFAULT_BTN_OK));
	if(NULL != pButton)
	{
		pButton->SetEnable(bCheckUpnp,false);
	}
	
	str.clear();
	
	if(bCheckUpnp)
	{
		str.append(m_pStrTable->String(E_STR_ID_US02_WZD_CHECKUPNPSUCC));
		str.append("\\n");
		str.append(m_pStrTable->String(E_STR_ID_US02_WZD_SEVENTIP01));
		str.append("\\n");
		str.append(m_pStrTable->String(E_STR_ID_US02_WZD_SEVENTIP02));
		str.append("\\n");
	}
	else
	{
		str.append(m_pStrTable->String(E_STR_ID_US02_WZD_CHECKUPNPFAIL));
		str.append("\\n");
		str.append(m_pStrTable->String(E_STR_ID_US02_WZD_Note));
		str.append("\\n");
		str.append(m_pStrTable->String(E_STR_ID_US02_WZD_SEVENERRTIP01));
		str.append("\\n");
		str.append(m_pStrTable->String(E_STR_ID_US02_WZD_SEVENERRTIP02));
		str.append("\\n");
		str.append(m_pStrTable->String(E_STR_ID_US02_WZD_SEVENERRTIP03));
	}
		m_richTextCtrl.SetText(str,false);
}

void CWzdUS02CheckUpnpDlg::OnInitial()
{
	CWzdUS02PageDlg::OnInitial();
	unsigned long x = m_length[DLG_OFFSET_SUB_LEFT];
	unsigned long y = m_length[DLG_OFFSET_SUB_TOP];

	unsigned long cx = m_rect.Width() -  2 * x;
	unsigned long cy = m_rect.Height() - 2 * y;

	m_richTextCtrl.Create(GetRandWndID(), x, y, cx, cy, this);

	x = m_length[DLG_OFFSET_SUB_LEFT];
	y = m_rect.Height() - 3 * m_length[DLG_OFFSET_SUB_TOP];

	cx = m_rect.Width() -  2 * x;
	cy = m_length[CHECK_HEIGHT];

	m_checkCtrl.Create(GetRandWndID(),x,y,cx,cy,this,0,1);
	m_checkCtrl.SetCaption(m_pStrTable->String(E_STR_ID_US02_WZD_OPENAGAIN),false,false);
	
}

void CWzdUS02CheckUpnpDlg::OnDestroy()
{
	
}

bool CWzdUS02CheckUpnpDlg::CheckUpnp()
{
	MESSAGE_DATA msgData;
	memset(&msgData, 0, sizeof(msgData));
	unsigned long staticIp= m_pWzdInfo->Addr_Info.staticIP;
	CMessageMan::Instance()->SendMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_NETPARAMCHECK_UPNP, reinterpret_cast<unsigned char *>(&staticIp),sizeof(unsigned long), msgData);
	
	unsigned long ret = *reinterpret_cast<unsigned long *>(msgData.pData+sizeof(PACKCMD));
	if(0 == ret)
	{
		CMessageMan::Instance()->ReleaseMsgBuff(msgData);
		return true;
	}
	
	CMessageMan::Instance()->ReleaseMsgBuff(msgData);
	return false;
}
BEGIN_MSG_MAP(CWzdUS02CheckUpnpDlg, CWzdUS02PageDlg)
	ON_MSG(m_checkCtrl.GetID(), KEY_VIRTUAL_ENTER, OnCheck)
END_MSG_MAP()

unsigned long  CWzdUS02CheckUpnpDlg::OnCheck()
{
	if(m_checkCtrl.IsCheck())
	{
		m_pWzdInfo ->ShowWzd =1;
	}
	else
	{
		m_pWzdInfo ->ShowWzd =0;
	}
	return KEY_VIRTUAL_STOP;
}
