/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author			: L
** Date				: 2009-12-28
** Name			: SelectAudioDlg.cpp 
** Version			: 1.0
** Description	: 
** Modify Record:
1:
***********************************************************************/

#include "SelectAudioDlg.h"
#include "GuiAssert.h"

using namespace GUI;

CSelectAudioDlg::CSelectAudioDlg()
: CHNN_DLG_BORDER( 8 )
, CHNN_DLG_TOP( 10 )
, CHNN_DLG_LEFT( 10 )
, CHNN_CK_OFFSET_X( 28 )
, CHNN_CK_OFFSET_Y( 4 )
{
	m_chnnAudio = 0;
	m_selChnn = 0;
	m_maxChnn = 0;

	m_pCheck = NULL;
}

CSelectAudioDlg::~CSelectAudioDlg()
{
	if( m_pCheck )
	{
		delete [] m_pCheck;
		m_pCheck = NULL;
	}
}

void CSelectAudioDlg::OnInitial()
{
	m_style |= WND_NO_TITLE;
	GUI::CDialog::OnInitial();

	//////////////////////////////////////////////////////////////////////////

	gui_assert(m_maxChnn>0,printf("Please SetChnnAudio() first!\n"));

	int maxRow = m_maxChnn/4;
	maxRow = m_maxChnn%4?(maxRow+1):maxRow;

	unsigned long xLeft = CHNN_DLG_LEFT, yTop = CHNN_DLG_TOP;
	unsigned long x = xLeft,y= yTop;
	unsigned long cx = m_length[CHECK_WIDTH] + CHNN_CK_OFFSET_X;
	unsigned long cy = m_length[CHECK_HEIGHT] + CHNN_CK_OFFSET_Y;

	//创建控件类
	if( m_pCheck )
	{
		delete [] m_pCheck;
		m_pCheck = NULL;
	}
	m_pCheck = new GUI::CCheckCtrl[m_maxChnn];

	int num = 0;
	char str[32] = {0};
	y -= cy;
	unsigned long idBase = CTRLID_SELECT_CHNN_CHECK_BEGIN;//ID值是别的对话框重用的
	for (int i=0;i<CHNN_MAX;i++)
	{
		if( m_chnnAudio & (static_cast<ULONGLONG>(0x1)<<i) )
		{
			if( num%4 == 0 )
			{
				x = xLeft;
				y += cy;
			}

			m_pCheck[num].Create(idBase+num,x,y,cx,cy,this,num/4,num%4);

			sprintf(str,"%d",i+1);
			m_pCheck[num].SetCaption(str,false,false);
			m_pCheck[num].SetData(i);//此值为通道编号

			if( m_selChnn & (static_cast<ULONGLONG>(0x1)<<i) )
			{
				m_pCheck[num].SetCheck(true,false);
				CheckUI(num,false);
			}

			x += cx;
			num += 1;
		}
	}

	//最下排,btn
	int btnWidth = 30;
	int btnHeight = m_length[BTN_HEIGHT];

	GUI::CRect rect = GetDlgDefaultRect();
	int xOff = CHNN_DLG_BORDER/2;
	x = rect.Width() - CHNN_DLG_BORDER - btnWidth * 2 - 4;
	y = rect.Height() - CHNN_DLG_BORDER - btnHeight + 2;

	m_btnOK.Create( GUI::CWnd::GetRandWndID(), x, y, btnWidth,btnHeight, this, 1, CHNN_MAX_ROW );

	x += btnWidth + xOff;
	m_btnExit.Create(GetRandWndID(), x, y, btnWidth,btnHeight, this, 2, CHNN_MAX_ROW );

	m_btnOK.SetImage( PAINT_STATUS_NORMAL, BMP_BUTTON_DLG_OK, false );
	m_btnOK.SetImage( PAINT_STATUS_FOCUS, BMP_BUTTON_DLG_OK_F, false );

	m_btnExit.SetImage( PAINT_STATUS_NORMAL, BMP_BTN_EXIT4, false );
	m_btnExit.SetImage( PAINT_STATUS_FOCUS, BMP_BTN_EXIT4_FOCUS, false );

	x = xLeft;
	cx = 120;
	cy = m_length[CHECK_HEIGHT];
	m_ckNone.Create(GetRandWndID(),x,y,cx,cy,this,0,CHNN_MAX_ROW);

	m_ckNone.SetCaption(m_pStrTable->String(E_STR_ID_MUTE),false,false);

	//最后做总体检查
	CheckUI(-1,false);
}

void CSelectAudioDlg::OnDestroy()
{
	CDialog::OnDestroy();

	if( m_pCheck )
	{
		delete [] m_pCheck;
		m_pCheck = NULL;
	}

	m_maxChnn = 0;
}

void CSelectAudioDlg::OnOutOfDlg(unsigned long keyID)
{
	GUI::CDialog::OnOutOfDlg(keyID);

	if (KEY_VIRTUAL_ENTER == keyID)
	{
		EndDialog(DLG_ESC);
	}
}

void CSelectAudioDlg::OnPaint( PAINT_STATUS status )
{
	// copy from SelectChnnDlg::OnPaint

	GUI::CDialog::OnPaint( status );

	//// border
	GUI::CWnd::DrawRectBorder( m_rect.m_left, m_rect.m_top, m_rect.Width(), m_rect.Height(), COLOR_LINE_BLACK, 1);
	GUI::CWnd::DrawRectBorder( m_rect.m_left + 1, m_rect.m_top + 1, m_rect.Width() - 2, m_rect.Height() - 2, COLOR_LINE_GRAY, 1);

	unsigned short xLeft = CHNN_DLG_LEFT, yTop = CHNN_DLG_TOP;
	unsigned short cx = m_length[CHECK_WIDTH] + CHNN_CK_OFFSET_X, cy = m_length[CHECK_HEIGHT] + CHNN_CK_OFFSET_Y;

	unsigned short row = 4, col = 4;
	row = m_maxChnn/4;
	row = m_maxChnn%4?(row+1):row;

	unsigned short rcLeft = m_rect.m_left + xLeft;
	unsigned short rcTop = m_rect.m_top + yTop;
	unsigned short xOff = 2, yOff = 4;
	unsigned short width = (m_length[CHECK_WIDTH] + CHNN_CK_OFFSET_X) * col;
	unsigned short height =(m_length[CHECK_HEIGHT] + CHNN_CK_OFFSET_Y) * row ;
	unsigned short x = rcLeft - xOff, y = rcTop - yOff;

	////

	GUI::CWnd::DrawLine( x, y , x, y + height, COLOR_LINE_GRAY ); //左竖
	if( col > 4 )
	{
		GUI::CWnd::DrawLine( x + m_length[CHECK_WIDTH] + CHNN_CK_OFFSET_X/2, y, x + m_length[CHECK_WIDTH] + CHNN_CK_OFFSET_X/2, y + height , COLOR_LINE_GRAY ); //中竖
	}
	GUI::CWnd::DrawLine( x + width, y, x + width, y + height , COLOR_LINE_GRAY ); //右竖

	////
	for ( int i = 0; i < row + 1; i++ )
	{
		GUI::CWnd::DrawLine( x, y, x + width , y, COLOR_LINE_GRAY );
		y += m_length[CHECK_HEIGHT] + CHNN_CK_OFFSET_Y;
	}
}

void CSelectAudioDlg::CheckUI(int chnn,bool bRefresh /*= false*/ )
{
	gui_assert(chnn<m_maxChnn,printf("Check SetChnnAudio or OnCheckClick!\n"));
	if( chnn>=0 )
	{
		bool checked = m_pCheck[chnn].IsCheck();
		//选中有效chnn
		for (int i=0;i<m_maxChnn;i++)
		{
			if( i != chnn )
			{
				if( m_pCheck[i].IsCheck() )
				{
					m_pCheck[i].SetCheck(false,bRefresh);
				}
			}
		}

		if( (!checked) != m_ckNone.IsCheck() )
		{
			m_ckNone.SetCheck(!checked,bRefresh);
		}
	}
	else
	{
		//选中静音
		bool ckNone = m_ckNone.IsCheck();
		bool checked = false;
		for(int i=0;i<m_maxChnn;i++)
		{
			if(ckNone)
			{
				if( m_pCheck[i].IsCheck() )
				{
					m_pCheck[i].SetCheck(false,bRefresh);
				}
			}
			else
			{
				if( m_pCheck[i].IsCheck() )
				{
					checked = true;
				}
			}
		}

		if( (!ckNone) && (!checked) )
		{
			m_ckNone.SetCheck(true,bRefresh);
		}
	}
}

//////////////////////////////////////////////////////////////////////////

void CSelectAudioDlg::SetChnnAudio(ULONGLONG allAudio, ULONGLONG ckedAudio)
{
	m_chnnAudio = allAudio;
	m_selChnn = ckedAudio;

	m_maxChnn = 0;
	for (int i=0; i<CHNN_MAX; ++i)
	{
		if( m_chnnAudio & (static_cast<ULONGLONG>(0x1) << i) )
		{
			++ m_maxChnn;
		}
	}
}

GUI::CRect CSelectAudioDlg::GetDlgDefaultRect()
{
	gui_assert(m_maxChnn>0,printf("Please SetChnnAudio() first!\n"));

	unsigned short row = 4, col = 4;
	row = m_maxChnn/4;
	row = m_maxChnn%4?(row+1):row;

	int cx = CHNN_DLG_BORDER * 2 + (m_length[CHECK_WIDTH] + CHNN_CK_OFFSET_X) * col;
	int cy = CHNN_DLG_BORDER * 2 + m_length[BTN_HEIGHT] + (m_length[CHECK_HEIGHT] + CHNN_CK_OFFSET_Y) * row;

	return GUI::CRect( 0 , 0 , cx , cy );
}

//////////////////////////////////////////////////////////////////////////

BEGIN_MSG_MAP(CSelectAudioDlg, GUI::CDialog)
	ON_MSG( m_btnExit.GetID(), KEY_VIRTUAL_ENTER, OnBtnExit)
	ON_MSG( m_btnOK.GetID(), KEY_VIRTUAL_ENTER, OnBtnOK)
	ON_MSG_CTRL_RANGE( CTRLID_SELECT_CHNN_CHECK_BEGIN, CTRLID_SELECT_CHNN_CHECK_END, KEY_VIRTUAL_ENTER, OnCheckClick )
	ON_MSG(m_ckNone.GetID(),KEY_VIRTUAL_ENTER,OnCheckNone);
END_MSG_MAP()

unsigned long CSelectAudioDlg::OnBtnExit()
{
	return OnCancel();
}

unsigned long CSelectAudioDlg::OnBtnOK()
{
	m_selChnn = 0;

	if( m_ckNone.IsCheck() )
	{
		m_selChnn = 0;
	}
	else
	{
		for (int i=0;i<m_maxChnn;i++)
		{
			if(m_pCheck[i].IsCheck())
			{
				unsigned long chnn = m_pCheck[i].GetData();
				m_selChnn = static_cast<ULONGLONG>(0x1)<<chnn;
			}
		}
	}

	return OnOK();
}

unsigned long CSelectAudioDlg::OnCheckClick( unsigned long ctrlID )
{
	//unsigned long index = ctrlID - CTRLID_SELECT_CHNN_CHECK_BEGIN;

	for (int i=0;i<m_maxChnn;i++)
	{
		if( m_pCheck[i].GetID() == ctrlID )
		{
			CheckUI(i,true);
		}
	}

	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CSelectAudioDlg::OnCheckNone()
{
	CheckUI(-1,true);
	return KEY_VIRTUAL_MSG_STOP;
}
