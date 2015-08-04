/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : 
** Date         : 
** Name         : 
** Version      : 
** Description  : 向导页面对话框
** Modify Record:
***********************************************************************/

#include "WzdNewNatRemoteAccessTipDlg.h"

CWzdNewNatRemoteAccessTipDlg::CWzdNewNatRemoteAccessTipDlg()
{

}

CWzdNewNatRemoteAccessTipDlg::~CWzdNewNatRemoteAccessTipDlg()
{

}


void CWzdNewNatRemoteAccessTipDlg::OnInitial()
{
	unsigned long x = m_length[DLG_OFFSET_SUB_LEFT];
	unsigned long y = m_length[DLG_OFFSET_SUB_TOP];

	unsigned long cx = m_rect.Width() -  2 * x;
	unsigned long cy = m_rect.Height() - 2 * y;

	m_richTextCtrl.Create(GetRandWndID(), x, y, cx, cy, this);

	SetTipInfo();

	x = m_length[DLG_OFFSET_SUB_LEFT];
	y = m_rect.Height() - 3 * m_length[DLG_OFFSET_SUB_TOP];

	cx = m_rect.Width() -  2 * x;
	cy = m_length[CHECK_HEIGHT];

	m_checkCtrl.Create(GetRandWndID(),x,y,cx,cy,this,0,1);
	m_checkCtrl.SetCaption(m_pStrTable->String(E_STR_ID_US02_WZD_OPENAGAIN),false,false);
}

void CWzdNewNatRemoteAccessTipDlg::OnDestroy()
{

}


void CWzdNewNatRemoteAccessTipDlg::SetWzdInfo(NAT_WIZARD_INFO * pWzdInfo)
{
	m_pWzdInfo = pWzdInfo;
}

void CWzdNewNatRemoteAccessTipDlg::ShowWzdData(bool bRefresh)
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

BEGIN_MSG_MAP(CWzdNewNatRemoteAccessTipDlg, CWzdNatPageDlg)
	ON_MSG(m_checkCtrl.GetID(), KEY_VIRTUAL_ENTER, OnCheck)
END_MSG_MAP()

unsigned long  CWzdNewNatRemoteAccessTipDlg::OnCheck()
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


void CWzdNewNatRemoteAccessTipDlg::SetTipInfo()
{
	std::string strTip;
	// strTip.append("The next step will take you through the Remote Access setup process.");
	strTip.append(m_pStrTable->String(E_STR_ID_US02_NEW_WZD_ACCESS_TIP_00));
	strTip.append("\\n");
	// strTip.append("Connect the network cable from the DVR to the Router.");
	strTip.append(m_pStrTable->String(E_STR_ID_US02_NEW_WZD_ACCESS_TIP_01));


	m_richTextCtrl.SetText(strTip, false);
	unsigned short maxTextRow = m_richTextCtrl.GetTextRow()-1;//文本显示需要多少行
	if (800 >= m_width)//800x600以上分辨率不需要滚动条
	{
		m_scroll_v.SetRange(0,maxTextRow);
		m_scroll_v.SetCurPos(0,false);
	}
}
