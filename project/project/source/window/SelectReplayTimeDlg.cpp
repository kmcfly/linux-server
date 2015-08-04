/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : ZYX
** Date         : 2011-05-26
** Name         : SelectReplayTimeDlg.cpp 
** Version      : 1.0
** Description  : 
** Modify Record: 
1:
***********************************************************************/
#include "SelectReplayTimeDlg.h"
#include <algorithm>

using namespace GUI;

CSelectReplayTimeDlg::CSelectReplayTimeDlg() : 
CHNN_DLG_BORDER( 8 ),CHNN_DLG_TOP( 10 ), CHNN_DLG_LEFT( 10 ), CHNN_CK_OFFSET_X( 40 ), CHNN_CK_OFFSET_Y( 4 )
{
	//CHNN_DLG_BORDER = 8,
	//CHNN_DLG_TOP	= 10,
	//CHNN_DLG_LEFT	= 10,

	//CHNN_CK_OFFSET_X = 28,
	//CHNN_CK_OFFSET_Y = 4,
	m_selTime = 5 * 60;
}

CSelectReplayTimeDlg::~CSelectReplayTimeDlg()
{

}

void CSelectReplayTimeDlg::OnInitial()
{
	m_style |= WND_NO_TITLE;
	GUI::CDialog::OnInitial();
	//////////////////////////////////////////////////////////////////////////

	int xLeft = CHNN_DLG_LEFT, yTop = CHNN_DLG_TOP;
	int cx = m_length[CHECK_WIDTH] + CHNN_CK_OFFSET_X, cy = m_length[CHECK_HEIGHT] + CHNN_CK_OFFSET_Y;
	int x = xLeft, y = yTop;


	// Create UI
	{
		y = yTop;
		/*y -= cy;*/
		CTRLID id = GUI::CWnd::GetRandWndID();
		x = xLeft;
		m_ckTime[0].Create( CTRLID_SELECT_CHNN_CHECK_BEGIN, x, y, cx - 4, cy, this, 1, 1);
		m_ckTime[0].SetCaption("2");
		x += cx;
		m_ckTime[1].Create( CTRLID_SELECT_CHNN_CHECK_BEGIN + 1, x, y, cx - 4, cy, this, 2, 1);
		m_ckTime[1].SetCaption("5");
		x += cx;
		m_ckTime[2].Create( CTRLID_SELECT_CHNN_CHECK_BEGIN + 2, x, y, cx - 4, cy, this, 3, 1);
		m_ckTime[2].SetCaption("10");
	}

	int btnWidth = 30;
	int btnHeight = m_length[BTN_HEIGHT];

	cx = CHNN_DLG_BORDER * 2 + (m_length[CHECK_WIDTH] + CHNN_CK_OFFSET_X) * 3;
	cy = CHNN_DLG_BORDER * 2 + m_length[BTN_HEIGHT] + (m_length[CHECK_HEIGHT] + CHNN_CK_OFFSET_Y);
	int xOff = CHNN_DLG_BORDER/2;
	x = cx - CHNN_DLG_BORDER - btnWidth * 2 - 4;
	y = cy - CHNN_DLG_BORDER - btnHeight + 2;

	int tipoffsetX =xLeft;
	int tipoffsetY =y;
	m_staticTip.Create(GUI::CWnd::GetRandWndID(),tipoffsetX,tipoffsetY,3*btnWidth,btnHeight,this);
	m_staticTip.SetCaption( m_pStrTable->String(E_STR_ID_MINUTES),false);
	m_staticTip.Show(true,false);

	m_btnOK.Create( GUI::CWnd::GetRandWndID(), x, y, btnWidth,btnHeight, this, 1, 2 );
	x += btnWidth + xOff;
	m_btnExit.Create( GUI::CWnd::GetRandWndID(), x, y, btnWidth,btnHeight, this, 2, 2 );

	m_btnOK.SetImage( PAINT_STATUS_NORMAL, BMP_BUTTON_DLG_OK, false );
	m_btnOK.SetImage( PAINT_STATUS_FOCUS, BMP_BUTTON_DLG_OK_F, false );

	m_btnExit.SetImage( PAINT_STATUS_NORMAL, BMP_BTN_EXIT4, false );
	m_btnExit.SetImage( PAINT_STATUS_FOCUS, BMP_BTN_EXIT4_FOCUS, false );

	////
	if (m_selTime == 2 * 60)
	{
		m_ckTime[0].SetCheck(true, false);
		m_ckTime[1].SetCheck(false, false);
		m_ckTime[2].SetCheck(false, false);
	}
	else if (m_selTime == 5 * 60)
	{
		m_ckTime[0].SetCheck(false, false);
		m_ckTime[1].SetCheck(true, false);
		m_ckTime[2].SetCheck(false, false);
	}
	else if (m_selTime == 10 * 60)
	{
		m_ckTime[0].SetCheck(false, false);
		m_ckTime[1].SetCheck(false, false);
		m_ckTime[2].SetCheck(true, false);
	}
	
}

void CSelectReplayTimeDlg::OnOutOfDlg(unsigned long keyID)
{
	GUI::CDialog::OnOutOfDlg(keyID);

	if (KEY_VIRTUAL_ENTER == keyID)
	{
		EndDialog(DLG_ESC);
	}
}

GUI::CRect CSelectReplayTimeDlg::GetDlgDefaultRect()
{
	unsigned short row = 1, col = 3;

	int cx = CHNN_DLG_BORDER * 2 + (m_length[CHECK_WIDTH] + CHNN_CK_OFFSET_X) * col;
	int cy = CHNN_DLG_BORDER * 2 + m_length[BTN_HEIGHT] + (m_length[CHECK_HEIGHT] + CHNN_CK_OFFSET_Y) * row;

	return GUI::CRect( 0 , 0 , cx , cy );
}

void CSelectReplayTimeDlg::OnPaint( PAINT_STATUS status )
{
	GUI::CDialog::OnPaint( status );

	//// border
	GUI::CWnd::DrawRectBorder( m_rect.m_left, m_rect.m_top, m_rect.Width(), m_rect.Height(), COLOR_LINE_BLACK, 1);
	GUI::CWnd::DrawRectBorder( m_rect.m_left + 1, m_rect.m_top + 1, m_rect.Width() - 2, m_rect.Height() - 2, COLOR_LINE_GRAY, 1);

	unsigned short xLeft = CHNN_DLG_LEFT, yTop = CHNN_DLG_TOP;
	unsigned short cx = m_length[CHECK_WIDTH] + CHNN_CK_OFFSET_X, cy = m_length[CHECK_HEIGHT] + CHNN_CK_OFFSET_Y;

	unsigned short rcLeft = m_rect.m_left + xLeft;
	unsigned short rcTop = m_rect.m_top + yTop;
	unsigned short xOff = 2, yOff = 4;
	unsigned short width = (m_length[CHECK_WIDTH] + CHNN_CK_OFFSET_X) * 3;
	unsigned short height =(m_length[CHECK_HEIGHT] + CHNN_CK_OFFSET_Y) * 1 ;
	unsigned short x = rcLeft - xOff, y = rcTop - yOff;

	////

	GUI::CWnd::DrawLine( x, y , x, y + height, COLOR_LINE_GRAY ); //×óÊú
	GUI::CWnd::DrawLine( x + width, y, x + width, y + height , COLOR_LINE_GRAY ); //ÓÒÊú

	////
	for ( int i = 0; i < 2; i++ )
	{
		GUI::CWnd::DrawLine( x, y, x + width , y, COLOR_LINE_GRAY );
		y += m_length[CHECK_HEIGHT] + CHNN_CK_OFFSET_Y;
	}
}

BEGIN_MSG_MAP(CSelectReplayTimeDlg, GUI::CDialog)
	ON_MSG( m_btnExit.GetID(), KEY_VIRTUAL_ENTER, OnBtnExit)
	ON_MSG( m_btnOK.GetID(), KEY_VIRTUAL_ENTER, OnBtnOK)
	ON_MSG_CTRL_RANGE( CTRLID_SELECT_CHNN_CHECK_BEGIN, CTRLID_SELECT_CHNN_CHECK_END, KEY_VIRTUAL_ENTER, OnCheckClick )
END_MSG_MAP()

unsigned long CSelectReplayTimeDlg::OnBtnExit()
{
	return OnCancel();
}

unsigned long CSelectReplayTimeDlg::OnBtnOK()
{
	if( m_ckTime[0].IsCheck() )
	{
		m_selTime = 2 * 60;
	}
	else if ( m_ckTime[1].IsCheck() )
	{
		m_selTime = 5 * 60;
	}
	else if (m_ckTime[2].IsCheck())
	{
		m_selTime = 10 * 60;
	}
	else
	{
		assert(false);
	}
	printf("182 m_selTime=%d\n",m_selTime);
	return OnOK();
}

unsigned long CSelectReplayTimeDlg::OnCheckClick( unsigned long ctrlID )
{
	int seln = ctrlID - CTRLID_SELECT_CHNN_CHECK_BEGIN;
	for (int i=0; i<3; i++)
	{
		if (i!=seln && m_ckTime[i].IsCheck())
		{
			m_ckTime[i].SetCheck(false, true);
		}
	}
	/*if (0 == seln)
	{
		m_selTime = 2* 60;
	}
	else if (1 == seln)
	{
		m_selTime = 5 * 60;
	}
	else if (2 == seln)
	{
		m_selTime = 10 * 60;
	}*/
	return KEY_VIRTUAL_MSG_STOP;
}
