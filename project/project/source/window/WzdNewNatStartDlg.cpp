/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : ZXX
** Date         : 2013-11-25
** Name         : WzdNewNatStartDlg.cpp
** Version      : 1.0
** Description  : 穿透向导页面开始对话框
** Modify Record:
***********************************************************************/

#include "WzdNewNatStartDlg.h"

CWzdNewNatStartDlg::CWzdNewNatStartDlg()
{
	m_toprow = 0;
}

CWzdNewNatStartDlg::~CWzdNewNatStartDlg()
{

}

void CWzdNewNatStartDlg::SetWzdInfo(NAT_WIZARD_INFO* pWzdInfo)
{
	m_pWzdInfo = pWzdInfo;
}

void CWzdNewNatStartDlg::ShowWzdData(bool bRefresh)
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

void CWzdNewNatStartDlg::GetWzdData(bool bNext)
{

}

void CWzdNewNatStartDlg::OnLeavePage()
{

}

void CWzdNewNatStartDlg::OnDestroy()
{
	CWzdNatPageDlg::OnDestroy();
}

void CWzdNewNatStartDlg::SetTipInfo()
{
	std::string strTip;
	// strTip.append("Welcome. ");
	strTip.append(m_pStrTable->String(E_STR_ID_US02_NEW_WZD_START_00));
	strTip.append("\\n");
	// strTip.append("Start Up Wizard for:");
	strTip.append(m_pStrTable->String(E_STR_ID_US02_NEW_WZD_START_01));
	strTip.append("\\n");
	// strTip.append("1. Language");
	strTip.append(m_pStrTable->String(E_STR_ID_US02_NEW_WZD_START_02));
	strTip.append("\\n");
	// strTip.append("2. Date and Time");
	strTip.append(m_pStrTable->String(E_STR_ID_US02_NEW_WZD_START_03));
	strTip.append("\\n");
	// strTip.append("3. Password");
	strTip.append(m_pStrTable->String(E_STR_ID_US02_NEW_WZD_START_04));
	strTip.append("\\n");
	// strTip.append("4. Local Connection and Remote Access");
	strTip.append(m_pStrTable->String(E_STR_ID_US02_NEW_WZD_START_05));

	m_richTextCtrl.SetText(strTip, false);
	unsigned short maxTextRow = m_richTextCtrl.GetTextRow()-1;//文本显示需要多少行
	if (800 >= m_width)//800x600以上分辨率不需要滚动条
	{
		m_scroll_v.SetRange(0,maxTextRow);
		m_scroll_v.SetCurPos(0,false);
	}
}

void CWzdNewNatStartDlg::OnInitial()
{
	CWzdNatPageDlg::OnInitial();
	unsigned long x = m_length[DLG_OFFSET_SUB_LEFT];
	unsigned long y = m_length[DLG_OFFSET_SUB_TOP];

	unsigned long cx = m_rect.Width() -  2 * x -CScrollCtrl::m_length[SCROLL_WIDTH] -2;
	unsigned long cy = m_rect.Height() - 2 * y - m_length[CHECK_HEIGHT]*2;
	unsigned long height =  m_rect.Height() - 2 * y - m_length[CHECK_HEIGHT];

	m_richTextCtrl.Create(GetRandWndID(), x, y, cx, cy, this);
	
	if (800 >= m_width)//800x600以上分辨率不需要滚动条
	{
		m_richTextCtrl.SetScrollShow(true);
		m_scroll_v.Create(GetRandWndID(), x+cx + 2,y,cy,this,0,1,false);
	}
	
	SetTipInfo();
	x = m_length[DLG_OFFSET_SUB_LEFT];
	y = m_length[DLG_OFFSET_SUB_TOP]+height;
	cx = m_rect.Width() -  2 * x;
	cy = m_length[CHECK_HEIGHT];
	m_checkCtrl.Create(GetRandWndID(),x,y,cx,cy,this,0,2);
	m_checkCtrl.SetCaption(m_pStrTable->String(E_STR_ID_US02_WZD_OPENAGAIN),false,false);

}

void CWzdNewNatStartDlg::InitCtrlInfo()
{

}

BEGIN_MSG_MAP(CWzdNewNatStartDlg, CWzdNatPageDlg)
	ON_MSG(m_checkCtrl.GetID(), KEY_VIRTUAL_ENTER, OnCheck)
	ON_MSG_EX(m_scroll_v.GetID(), KEY_VIRTUAL_MSG_DEFAULT_TAG, OnScrollchange)
	ON_MSG_EX(m_scroll_v.GetID(),KEY_VIRTUAL_ENTER, OnScrollchange)
	ON_MSG_EX(m_scroll_v.GetID(),KEY_VIRTUAL_DRAG, OnScrollchange)
	ON_MSG_EX(m_scroll_v.GetID(),KEY_VIRTUAL_BLUP, OnScrollchange)
END_MSG_MAP()

unsigned long CWzdNewNatStartDlg::OnScrollchange(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	int pos = m_scroll_v.GetPos();
	if (pos != m_toprow)
	{
		m_toprow = pos;
		m_richTextCtrl.SetCurRow(m_toprow);
		Repaint();
	}

	return KEY_VIRTUAL_MSG_STOP; 
}

unsigned long  CWzdNewNatStartDlg::OnCheck()
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

