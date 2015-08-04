/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : zxx
** Date         : 2012-09-18
** Name         : WzdUS02ConnectTipDlg.h
** Version      : 1.0
** Description  : 向导页面对话框
** Modify Record:
***********************************************************************/

#include "WzdUS02ConnectTipDlg.h"

CWzdUS02ConnectTipDlg::CWzdUS02ConnectTipDlg()
{

}

CWzdUS02ConnectTipDlg::~CWzdUS02ConnectTipDlg()
{

}


void CWzdUS02ConnectTipDlg::OnInitial()
{
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

void CWzdUS02ConnectTipDlg::OnDestroy()
{

}

void CWzdUS02ConnectTipDlg::SetTipDisk(const std::string& str)
{
	m_richTextCtrl.SetText(str, false);
}
void CWzdUS02ConnectTipDlg::SetWzdInfo(US02_WzdInfo * pWzdInfo)
{
	m_pWzdInfo = pWzdInfo;
}

void CWzdUS02ConnectTipDlg::ShowWzdData(bool bRefresh)
{
	if(1 == m_pWzdInfo->ShowWzd)
	{
		m_checkCtrl.SetCheck(true,false);
	}
	else
	{
		m_checkCtrl.SetCheck(false,false);
	}
}

BEGIN_MSG_MAP(CWzdUS02ConnectTipDlg, CWzdUS02PageDlg)
	ON_MSG(m_checkCtrl.GetID(), KEY_VIRTUAL_ENTER, OnCheck)
END_MSG_MAP()

unsigned long  CWzdUS02ConnectTipDlg::OnCheck()
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
