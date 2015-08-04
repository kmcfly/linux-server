/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : ysw
** Date         : 2009-08-06
** Name         : SystemInfoDlg.cpp
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/
#include "SystemInfoDlg.h"

//////////////////////////////////////////////////////////////////////////
CSystemInfoDlg::CSystemInfoDlg()
{

}

CSystemInfoDlg::~CSystemInfoDlg()
{

}

//////////////////////////////////////////////////////////////////////////
void CSystemInfoDlg::OnInitial()
{
	GUI::CDialog::OnInitial();
	SetImage(BMP_MEM_DLG_LARGE, false);
	SetCaption(m_pStrTable->String(E_STR_ID_TITLE_SYSTEM), false);
/*
	//////////////////////////////////////////////////////////////////////////
	unsigned short x = m_length[DLG_OFFSET_LEFT], y = m_length[TITLE_HEIGHT]+m_length[TABLE_HEIGHT]+m_length[DLG_OFFSET_TOP];
	unsigned short cx = m_rect.Width() - 2 * m_length[DLG_OFFSET_LEFT];
	unsigned short cy = m_rect.Height() - y - 2 * m_length[DLG_OFFSET_BOTTOM];
	m_versionDlg.Create(GUI::CWnd::GetRandWndID(), x, y, cx, cy, this, 0, 0);
	m_streamDlg.Create(GUI::CWnd::GetRandWndID(), x, y, cx, cy, this, 0, 0);

	m_tabCtrl.Create(GUI::CWnd::GetRandWndID(), x, m_length[TITLE_HEIGHT]+m_length[DLG_OFFSET_TOP], cx, m_length[TABLE_HEIGHT], this, 0, 0);
	m_tabCtrl.AddItem(&m_versionDlg, m_pStrTable->String(E_STR_ID_VERSION));
	m_tabCtrl.AddItem(&m_streamDlg, m_pStrTable->String(E_STR_ID_STREAM));

	m_tabCtrl.SetCurItem(0, false);
	*/
	unsigned short x = m_length[DLG_OFFSET_LEFT], y = m_length[TITLE_HEIGHT]+m_length[DLG_OFFSET_TOP];
	unsigned short cx = m_rect.Width() - 2 * m_length[DLG_OFFSET_LEFT];
	unsigned short cy = m_rect.Height() - y - 2 * m_length[DLG_OFFSET_BOTTOM];

	m_versionDlg.Create(GUI::CWnd::GetRandWndID(), x, y, cx, cy, this, 0, 0);
}
