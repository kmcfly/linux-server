/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : L
** Date         : 2009-08-19
** Name         : ProgressDlg.cpp
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/

#include "ProgressDlg.h"

CProgressDlg::CProgressDlg()
{
	m_xOffLeft = 4;
	m_xOffRight = 4;
}

CProgressDlg::~CProgressDlg()
{

}


void CProgressDlg::SetDlgStyle( )
{

}

void CProgressDlg::SetPercent( int percet , bool bRefresh )
{
	m_progress.SetPercent( percet, bRefresh );
}

void CProgressDlg::EndProgress()
{
	OnCancel();
}

void CProgressDlg::OnInitial()
{
	CDialog::OnInitial();
	m_style |= WND_NO_TITLE;

	//////////////////////////////////////////////////////////////////////////

	unsigned short cx = m_rect.Width() - m_xOffLeft - m_xOffRight;
	unsigned short cy = 24;
	unsigned short x = (m_rect.Width() - cx) /2, y = (m_rect.Height() - cy)/2;

	m_progress.Create( GUI::CWnd::GetRandWndID(), x, y, cx, cy, this );
}

void CProgressDlg::OnPaint(PAINT_STATUS status)
{

}
