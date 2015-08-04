/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : ZXX
** Date         : 2012-10-17
** Name         : WzdUS02MobiletipDlg.cpp
** Version      : 1.0
** Description  : 向导管理对话框
** Modify Record:
***********************************************************************/

#include "WzdUS02MobiletipDlg.h"

CWzdUS02MobiletipDlg::CWzdUS02MobiletipDlg()
{

}

CWzdUS02MobiletipDlg::~CWzdUS02MobiletipDlg()
{
	
}

void CWzdUS02MobiletipDlg::SetWzdInfo(US02_WzdInfo* pWzdInfo)
{
	m_pWzdInfo = pWzdInfo;
}

void CWzdUS02MobiletipDlg::ShowWzdData(bool bRefresh)
{
	string str;
	if(MOBILE_APPLE == m_pWzdInfo->MobileDevice)
	{
		str.append(m_pStrTable->String(E_STR_ID_US02_WZD_APPTIP01));
		str.append("\\n");
		str.append(m_pStrTable->String(E_STR_ID_US02_WZD_APPTIP02));
		str.append("\\n");
		str.append(m_pStrTable->String(E_STR_ID_US02_WZD_APPTIP03));
		str.append("\\n");
		str.append(m_pStrTable->String(E_STR_ID_US02_WZD_ELEVENTTIP04));
		str.append("\\n");
		str.append(m_pStrTable->String(E_STR_ID_US02_WZD_ELEVENTTIP05));
		if(0 != strlen(m_pWzdInfo->RouterIp))
		{
			str.append(m_pWzdInfo->RouterIp);
			char tmp[256]={0};
			snprintf(tmp,sizeof(tmp),":%d",m_pWzdInfo->httpPort);
			str.append(tmp);
		}
		str.append("\\n");
		str.append(m_pStrTable->String(E_STR_ID_US02_WZD_ELEVENTTIP06));
		str.append("\\n");
		str.append(m_pStrTable->String(E_STR_ID_US02_WZD_ELEVENTTIP07));
		str.append("\\n");
		str.append(m_pStrTable->String(E_STR_ID_US02_WZD_ELEVENTTIP08));

	}
	else if(MOBILE_ANDROID == m_pWzdInfo->MobileDevice)
	{
		str.append(m_pStrTable->String(E_STR_ID_US02_WZD_ANDROIDTIP01));
		str.append("\\n");
		str.append(m_pStrTable->String(E_STR_ID_US02_WZD_ANDROIDTIP02));
		str.append("\\n");
		str.append(m_pStrTable->String(E_STR_ID_US02_WZD_ANDROIDTIP03));
		str.append("\\n");
		str.append(m_pStrTable->String(E_STR_ID_US02_WZD_ELEVENTTIP04));
		str.append("\\n");
		str.append(m_pStrTable->String(E_STR_ID_US02_WZD_ELEVENTTIP05));
		if(0 != strlen(m_pWzdInfo->RouterIp))
		{
			str.append(m_pWzdInfo->RouterIp);
			char tmp[256]={0};
			snprintf(tmp,sizeof(tmp),":%d",m_pWzdInfo->httpPort);
			str.append(tmp);
		}
		str.append("\\n");
		str.append(m_pStrTable->String(E_STR_ID_US02_WZD_ELEVENTTIP06));
		str.append("\\n");
		str.append(m_pStrTable->String(E_STR_ID_US02_WZD_ELEVENTTIP07));
		str.append("\\n");
		str.append(m_pStrTable->String(E_STR_ID_US02_WZD_ELEVENTTIP08));
	}
	m_richText.SetText(str,false);
	if(1 == m_pWzdInfo->ShowWzd)
	{
		m_checkCtrl.SetCheck(true,false);
	}
	else
	{
		m_checkCtrl.SetCheck(false,false);
	}
}

void CWzdUS02MobiletipDlg::OnInitial()
{
	CWzdUS02PageDlg::OnInitial();
	unsigned long x = m_length[DLG_OFFSET_SUB_LEFT];
	unsigned long y = m_length[DLG_OFFSET_SUB_TOP];
	unsigned long cx = m_rect.Width() -  2 * x;
	unsigned long cy = m_rect.Height() - 2 * y;

	m_richText.Create(GetRandWndID(),x,y,cx,cy,this);

	x = m_length[DLG_OFFSET_SUB_LEFT];
	y = m_rect.Height() - 3 * m_length[DLG_OFFSET_SUB_TOP];

	cx = m_rect.Width() -  2 * x;
	cy = m_length[CHECK_HEIGHT];

	m_checkCtrl.Create(GetRandWndID(),x,y,cx,cy,this,0,1);
	m_checkCtrl.SetCaption(m_pStrTable->String(E_STR_ID_US02_WZD_OPENAGAIN),false,false);

}

void CWzdUS02MobiletipDlg::OnPostInitial()
{
	

}

void CWzdUS02MobiletipDlg::OnDestroy()
{

}

BEGIN_MSG_MAP(CWzdUS02MobiletipDlg, CWzdUS02PageDlg)
	ON_MSG(m_checkCtrl.GetID(), KEY_VIRTUAL_ENTER, OnCheck)
END_MSG_MAP()


unsigned long  CWzdUS02MobiletipDlg::OnCheck()
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

//end
