/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : ysw
** Date         : 2009-08-06
** Name         : NetworkInfoDlg.cpp
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/
#include "NetworkInfoDlg.h"

//////////////////////////////////////////////////////////////////////////
CNetworkInfoDlg::CNetworkInfoDlg()
{

}

CNetworkInfoDlg::~CNetworkInfoDlg()
{

}

//////////////////////////////////////////////////////////////////////////
void CNetworkInfoDlg::OnInitial()
{
	GUI::CDialog::OnInitial();
	SetImage(BMP_MEM_DLG_LARGE, false);
	SetCaption(m_pStrTable->String(E_STR_ID_TITLE_NETWORK_INFO), false);
/*
	//////////////////////////////////////////////////////////////////////////
	unsigned short x = m_length[DLG_OFFSET_LEFT], y = m_length[TITLE_HEIGHT]+m_length[TABLE_HEIGHT]+m_length[DLG_OFFSET_TOP];
	unsigned short cx = m_rect.Width() - 2 * m_length[DLG_OFFSET_LEFT];
	unsigned short cy = m_rect.Height() - y - 2 * m_length[DLG_OFFSET_BOTTOM];

	m_networkStatusDlg.Create(GUI::CWnd::GetRandWndID(), x, y, cx, cy, this, 0, 1);
	m_networkStreamInfoDlg.Create(GUI::CWnd::GetRandWndID(), x, y, cx, cy, this, 0, 1);

	m_tabCtrl.Create(GUI::CWnd::GetRandWndID(), x, m_length[TITLE_HEIGHT]+10, cx, m_length[TABLE_HEIGHT], this, 0, 0);
	m_tabCtrl.AddItem(&m_networkStatusDlg, m_pStrTable->String(E_STR_ID_NETWORK));
	m_tabCtrl.AddItem(&m_networkStreamInfoDlg, m_pStrTable->String(E_STR_ID_NETWORK_STREAM));

	m_tabCtrl.SetCurItem(0, false);
	*/
	unsigned short x = m_length[DLG_OFFSET_LEFT], y = m_length[TITLE_HEIGHT]+m_length[DLG_OFFSET_TOP];
	unsigned short cx = m_rect.Width() - 2 * m_length[DLG_OFFSET_LEFT];
	unsigned short cy = m_rect.Height() - y - 2 * m_length[DLG_OFFSET_BOTTOM];

	m_networkStatusDlg.Create(GUI::CWnd::GetRandWndID(), x, y, cx, cy, this, 0, 1);
}
