/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : ZXX
** Date         : 2012-10-17
** Name         : WzdUS02DDNSConnectTipDlg.cpp
** Version      : 1.0
** Description  : 向导管理对话框
** Modify Record:
***********************************************************************/
#include "WzdUS02DDNSConnectTipDlg.h"

CWzdUS02DDNSConnectTipDlg::CWzdUS02DDNSConnectTipDlg()
{

}

CWzdUS02DDNSConnectTipDlg::~CWzdUS02DDNSConnectTipDlg()
{

}

void CWzdUS02DDNSConnectTipDlg::SetWzdInfo(US02_WzdInfo* pWzdInfo)
{
	m_pWzdInfo = pWzdInfo;

}

void CWzdUS02DDNSConnectTipDlg::ShowWzdData(bool bRefresh)
{
	string str;
	str.append(m_pStrTable->String(E_STR_ID_US02_WZD_13_01));
	str.append("\\n");
	str.append(m_pStrTable->String(E_STR_ID_US02_WZD_13_02));
	str.append("\\n");
	if(1 == m_pWzdInfo->ddnsInfo.bEnable)
	{
		if(0 != strlen(m_pWzdInfo->ddnsInfo.domain))
		{
			str.append(m_pWzdInfo->ddnsInfo.domain);
			char tmp[256]={0};
			snprintf(tmp,sizeof(tmp),":%d",m_pWzdInfo->httpPort);
			str.append(tmp);

		}
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
void CWzdUS02DDNSConnectTipDlg::OnInitial()
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

void CWzdUS02DDNSConnectTipDlg::OnDestroy()
{

}
BEGIN_MSG_MAP(CWzdUS02DDNSConnectTipDlg, CWzdUS02PageDlg)
	ON_MSG(m_checkCtrl.GetID(), KEY_VIRTUAL_ENTER, OnCheck)
END_MSG_MAP()

unsigned long  CWzdUS02DDNSConnectTipDlg::OnCheck()
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
