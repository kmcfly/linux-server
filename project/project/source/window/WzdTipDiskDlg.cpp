/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : L
** Date         : 2010-03-01
** Name         : WzdTipDiskDlg.cpp
** Version      : 1.0
** Description  : 磁盘相关向导提示
** Modify Record:
***********************************************************************/

#include "WzdTipDiskDlg.h"

CWzdTipDiskDlg::CWzdTipDiskDlg()
{

}

CWzdTipDiskDlg::~CWzdTipDiskDlg()
{

}

void CWzdTipDiskDlg::OnInitial()
{
	unsigned long x = m_length[DLG_OFFSET_SUB_LEFT];
	unsigned long y = m_length[DLG_OFFSET_SUB_TOP];

	unsigned long cx = m_rect.Width() -  2 * x;
	unsigned long cy = m_rect.Height() - 2 * y;

	m_richTextCtrl.Create(GetRandWndID(), x, y, cx, cy, this);
}


void CWzdTipDiskDlg::SetTipDisk(const std::string& str)
{
	m_richTextCtrl.SetText(str, false);
}

void CWzdTipDiskDlg::AppendTipDisk(const std::string& str)
{
	m_richTextCtrl.AppendText(str, false);
}
