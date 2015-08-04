/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : ZXX
** Date         : 2011-05-26
** Name         : SpecSplitDlg.cpp
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/

#include "SpecSplitDlg.h"
#include "LocalUIMan.h"
extern NET_LOGIN_SUCCESS_REPLY *g_p_login_succ;
using namespace GUI;


CSpecSplitDlg::CSpecSplitDlg() : CHNN_DLG_BORDER( 8 ),CHNN_DLG_TOP( 10 ), CHNN_DLG_LEFT( 10 ), CHNN_CK_OFFSET_X( 40 ), CHNN_CK_OFFSET_Y( 4 )
{
	m_maxChnnNum            =0;
	m_maxSplitMode			=PLAY_CH_NULL;

	m_1A5CH					= 0x003f;
	m_1A7CH					= 0x00ff;
	m_1A12CH				=0x1fff;
	m_selCuPlayrChnn		=0;
	m_currSplitMode			=PLAY_CH_NULL;
	m_maxSelChnn			=0;
	m_playToolRect.SetRect(0,0,0,0);
	m_rcWnd.SetRect(0,0,0,0);
}

CSpecSplitDlg::~CSpecSplitDlg()
{

}
void CSpecSplitDlg::SetPlyabackParam(PLAY_CH_MODE maxSplitMode,int maxchnnNum,int maxSelChnn)
{
	m_maxSplitMode = maxSplitMode;
	m_maxChnnNum = maxchnnNum;
	m_maxSelChnn = maxSelChnn;
	
}
void CSpecSplitDlg::OnInitial()
{
	m_style |= WND_NO_TITLE;
	GUI::CDialog::OnInitial();
	m_splitType=SPEC_NONE;
	
	int xLeft=5,yTop=5;
	int cx = 24, cy = 24;//宽度和高度
	int x = xLeft, y = yTop;
	int xFocus = 0;

	if(m_maxSplitMode >= PLAY_CH_SIX)
	{
		
		m_btn1A5.Create(GetRandWndID(), x, y, cx, cy, this, xFocus++, 1);
		m_btn1A5.SetImage(PAINT_STATUS_NORMAL, BMP_BTN_PLAY_1A5, false);
		m_btn1A5.SetImage(PAINT_STATUS_FOCUS, BMP_BTN_PLAY_1A5_F, false);
		m_btn1A5.SetImage(PAINT_STATUS_DISABLE, BMP_BTN_PLAY_1A5_N, false);
		m_btn1A5.Show(true,false);
		x += 26;
		m_btn1A5Ex.Create(GetRandWndID(), x, y+5, cx, cy, this, xFocus++, 1);
		m_btn1A5Ex.SetImage(PAINT_STATUS_NORMAL, BMP_BTN_PLAY_TRIANGLE, false);
		m_btn1A5Ex.SetImage(PAINT_STATUS_FOCUS, BMP_BTN_PLAY_TRIANGLE_F, false);
		m_btn1A5Ex.SetImage(PAINT_STATUS_DISABLE, BMP_BTN_PLAY_TRIANGLE, false);
		m_btn1A5Ex.Show(true,false);
	}
	
	if(m_maxSplitMode >= PLAY_CH_EIGHT)
	{
		x += 14;
		m_btn1A7.Create(GetRandWndID(), x, y, cx, cy, this, xFocus++, 1);
		m_btn1A7.SetImage(PAINT_STATUS_NORMAL, BMP_BTN_PLAY_1A7, false);
		m_btn1A7.SetImage(PAINT_STATUS_FOCUS, BMP_BTN_PLAY_1A7_F, false);
		m_btn1A7.SetImage(PAINT_STATUS_DISABLE, BMP_BTN_PLAY_1A7_N, false);
		m_btn1A7.Show(true,false);
		x += 26;
		m_btn1A7Ex.Create(GetRandWndID(), x, y+5, cx, cy, this, xFocus++, 1);
		m_btn1A7Ex.SetImage(PAINT_STATUS_NORMAL, BMP_BTN_PLAY_TRIANGLE, false);
		m_btn1A7Ex.SetImage(PAINT_STATUS_FOCUS, BMP_BTN_PLAY_TRIANGLE_F, false);
		m_btn1A7Ex.SetImage(PAINT_STATUS_DISABLE, BMP_BTN_PLAY_TRIANGLE, false);
		m_btn1A7Ex.Show(true,false);

	}
	if(m_maxSplitMode >= PLAY_CH_NINE)
	{
		
	}
	if(m_maxSplitMode >= PLAY_CH_THIRTEEN)
	{
		x += 14;
		m_btn1A12.Create(GetRandWndID(), x, y, cx, cy, this, xFocus++, 1);
		m_btn1A12.SetImage(PAINT_STATUS_NORMAL, BMP_BTN_PLAY_1A12, false);
		m_btn1A12.SetImage(PAINT_STATUS_FOCUS, BMP_BTN_PLAY_1A12_F, false);
		m_btn1A12.SetImage(PAINT_STATUS_DISABLE, BMP_BTN_PLAY_1A12_N, false);
		m_btn1A12.Show(true,false);
		x += 26;
		m_btn1A12Ex.Create(GetRandWndID(), x, y+5, cx, cy, this, xFocus++, 1);
		m_btn1A12Ex.SetImage(PAINT_STATUS_NORMAL, BMP_BTN_PLAY_TRIANGLE, false);
		m_btn1A12Ex.SetImage(PAINT_STATUS_FOCUS, BMP_BTN_PLAY_TRIANGLE_F, false);
		m_btn1A12Ex.SetImage(PAINT_STATUS_DISABLE, BMP_BTN_PLAY_TRIANGLE, false);
		m_btn1A12Ex.Show(true,false);

	}
	
	if(m_maxSplitMode >= PLAY_CH_SIXTEEN)
	{

	}

}

void CSpecSplitDlg::OnOutOfDlg(unsigned long keyID)
{
	GUI::CDialog::OnOutOfDlg(keyID);

	if (KEY_VIRTUAL_ENTER == keyID)
	{
		EndDialog(DLG_ESC);
	}
}

void CSpecSplitDlg::OnPaint( PAINT_STATUS status )
{
	GUI::CDialog::OnPaint( status );

	//// border
	GUI::CWnd::DrawRectBorder( m_rect.m_left, m_rect.m_top, m_rect.Width(), m_rect.Height(), COLOR_LINE_BLACK, 1);
	GUI::CWnd::DrawRectBorder( m_rect.m_left + 1, m_rect.m_top + 1, m_rect.Width() - 2, m_rect.Height() - 2, COLOR_LINE_GRAY, 1);

}
GUI::CRect CSpecSplitDlg::GetDlgDefaultRect()
{
	unsigned short row = 0, col = 0;
	row=1;
	col=4;
	int cx = CHNN_DLG_BORDER * 2 + (m_length[CHECK_WIDTH] + CHNN_CK_OFFSET_X) * col;
	int cy = CHNN_DLG_BORDER + m_length[BTN_HEIGHT] + CHNN_CK_OFFSET_Y* row;

	return GUI::CRect( 0 , 0 , cx , cy );
}

bool CSpecSplitDlg::OnClickSelChnn(GUI::CButton & btn, PLAY_CH_MODE splitMode, int splitChnn, ULONGLONG & selCH)
{
	if ( m_maxSplitMode >= splitMode)
	{
		m_selChnnDlg.SetChnnCount(m_maxChnnNum, g_p_login_succ->authPlaybackCH, min(splitChnn, m_maxSelChnn), selCH);
		GUI::CRect rcBtn = btn.GetWindowRect();
		GUI::CRect rcSpecSplit = GetDlgDefaultRect();//弹出的按钮菜单的矩形
		GUI::CRect rcSel = m_selChnnDlg.GetDlgDefaultRect();
		//按钮做边界大于弹出框左边界+
		int x = (rcBtn.m_left > m_playToolRect.m_left + (rcSel.Width() - rcBtn.Width()) / 2)  ? (rcBtn.m_left - (rcSel.Width() - rcBtn.Width()) / 2) : rcBtn.m_left;
		int y=0;
		if(x + rcSel.Width() > m_playToolRect.m_right)
		{
			x = m_rect.m_right - rcSel.Width() - 1;
		}
		if(m_rcWnd.m_top + 1 > (rcSel.Height() + m_playToolRect.m_top + rcSpecSplit.Height()))
		{
			y=m_rcWnd.m_top - rcSel.Height() -rcSpecSplit.Height()- 1;
		}
		else
		{
			if(m_rcWnd.m_top + 1 > (m_playToolRect.m_top + rcSpecSplit.Height()))
			{
				//剩余的空间值够放下一个弹出菜单
				y=m_rcWnd.m_bottom + 1;
			}
			else
			{
				//不够放弹出菜单
				y=m_rcWnd.m_bottom + 1 + rcSpecSplit.Height();
			}
		}
		if( DLG_OK == m_selChnnDlg.DoModal(this, x, y, rcSel.Width(), rcSel.Height()) )
		{
			selCH = m_selChnnDlg.GetSelChnn();

			m_selCuPlayrChnn = selCH;
			m_currSplitMode	= splitMode;
			return true;
		}
		else
		{
			GetRootWnd()->Repaint();
		}
	}

	return false;
}

void CSpecSplitDlg::GetExParam(PLAY_CH_MODE &splitMode,ULONGLONG &selCH)
{
	splitMode = m_currSplitMode;
	selCH = m_selCuPlayrChnn;
}

void CSpecSplitDlg::SetPlayToolParam(GUI::CRect playToolRect,GUI::CRect rcWnd)
{
	m_playToolRect = playToolRect;
	m_rcWnd = rcWnd;
}
BEGIN_MSG_MAP(CSpecSplitDlg, GUI::CDialog)
	ON_MSG(m_btn1A5.GetID(), KEY_VIRTUAL_ENTER, OnClickBtn1A5)
	ON_MSG(m_btn1A5Ex.GetID(),KEY_VIRTUAL_ENTER,OnClickBtn1A5Ex)
	ON_MSG(m_btn1A7.GetID(), KEY_VIRTUAL_ENTER, OnClickBtn1A7)
	ON_MSG(m_btn1A7Ex.GetID(),KEY_VIRTUAL_ENTER,OnClickBtn1A7Ex)
	ON_MSG(m_btn1A12.GetID(), KEY_VIRTUAL_ENTER, OnClickBtn1A12)
	ON_MSG(m_btn1A12Ex.GetID(), KEY_VIRTUAL_ENTER, OnClickBtn1A12Ex)
END_MSG_MAP()
unsigned long CSpecSplitDlg::OnClickBtn1A5()
{
	m_splitType=SPEC_1A5;
	return OnOK();
}

unsigned long CSpecSplitDlg::OnClickBtn1A5Ex()
{
	if (OnClickSelChnn(m_btn1A5Ex, PLAY_CH_SIX, 6, m_1A5CH))
	{
		m_splitType=SPEC_1A5EX;
	}
	
	return OnOK();
}

unsigned long CSpecSplitDlg::OnClickBtn1A7()
{
	m_splitType=SPEC_1A7;
	return OnOK();
}

unsigned long CSpecSplitDlg::OnClickBtn1A7Ex()
{
	if (OnClickSelChnn(m_btn1A7Ex, PLAY_CH_EIGHT, 8, m_1A7CH))
	{
		m_splitType=SPEC_1A7EX;
	}

	return OnOK();
}
unsigned long CSpecSplitDlg::OnClickBtn1A12()
{

	m_splitType=SPEC_1A12;
	return OnOK();
	

}
unsigned long CSpecSplitDlg::OnClickBtn1A12Ex()
{
	if (OnClickSelChnn(m_btn1A12, PLAY_CH_THIRTEEN, 13, m_1A12CH))
	{
		m_splitType=SPEC_1A12EX;
	}

	return OnOK();
}
//end
