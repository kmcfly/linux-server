/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : hz
** Date         : 2009-08-01
** Name         : SelectChnnDlg.cpp 
** Version      : 1.0
** Description  : 
** Modify Record: 2009-8-28
1:
***********************************************************************/
#include "SelectChnnDlg.h"
#include "FuncCustom.h"
#include <algorithm>

using namespace GUI;

CSelectChnnDlg::CSelectChnnDlg() : m_uiLayOutInited( false ), 
CHNN_DLG_BORDER( 8 ),CHNN_DLG_TOP( 10 ), CHNN_DLG_LEFT( 10 ), CHNN_CK_OFFSET_X( 40 ), CHNN_CK_OFFSET_Y( 4 )
{
	//CHNN_DLG_BORDER = 8,
	//CHNN_DLG_TOP	= 10,
	//CHNN_DLG_LEFT	= 10,

	//CHNN_CK_OFFSET_X = 28,
	//CHNN_CK_OFFSET_Y = 4,

	m_uiLayOutInited = false;
	m_bUsedMiniMap = false;
	m_chnnMiniMap = -1;

	m_bAutoSel = true;
	m_bAllChnnEnable = true;

	memset( &m_uiLayOut, 0, sizeof(m_uiLayOut) );
}

CSelectChnnDlg::~CSelectChnnDlg()
{

}

void CSelectChnnDlg::OnInitial()
{
	m_style |= WND_NO_TITLE;
	GUI::CDialog::OnInitial();
	//////////////////////////////////////////////////////////////////////////

	assert( m_uiLayOutInited );// 必须先设置 SetChnnCount(~,~)

	int xLeft = CHNN_DLG_LEFT, yTop = CHNN_DLG_TOP;
	int cx = m_length[CHECK_WIDTH] + CHNN_CK_OFFSET_X, cy = m_length[CHECK_HEIGHT] + CHNN_CK_OFFSET_Y;
	int x = xLeft, y = yTop;

	GUI::CCheckCtrl* pCheck[CHNN_MAX_ROW][5] = { NULL };

	// Init Map
	{
		for ( int j = 0; j < CHNN_MAX_ROW; j++ )
		{
			if( m_uiLayOut[j][0] )
			{
				pCheck[j][0] = &m_ckHeader[j];
			}
			for ( int i = 1; i < 5; i++ )
			{
				if( m_uiLayOut[j][i] ) 
				{
					pCheck[j][i] = &m_ckChnn[j * 4 + i - 1];
				}
			}
		}
	}

	// Create UI
	{
		y = yTop;
		y -= cy;
		CTRLID id = GUI::CWnd::GetRandWndID();
		for ( int j = 0; j < CHNN_MAX_ROW; j++ )
		{
			x = xLeft;
			y += cy;
			for ( int i = 0; i < 5; i ++ )
			{
				if( pCheck[j][i] ) 
				{
					pCheck[j][i]->Create( CTRLID_SELECT_CHNN_CHECK_BEGIN + j * 5 + i, x, y, cx - 4, cy, this, i + 1, j + 1);
					pCheck[j][i]->SetCheck( false, false );
					x += cx;
				}
			}
		}
	}

	int btnWidth = 30;
	int btnHeight = m_length[BTN_HEIGHT];

	GUI::CRect rect = GetDlgDefaultRect();
	int xOff = CHNN_DLG_BORDER/2;
	x = rect.Width() - CHNN_DLG_BORDER - btnWidth * 2 - 4;
	y = rect.Height() - CHNN_DLG_BORDER - btnHeight + 2;

	m_btnOK.Create( GUI::CWnd::GetRandWndID(), x, y, btnWidth,btnHeight, this, 1, CHNN_MAX_ROW );

	x += btnWidth + xOff;
	m_btnExit.Create( GUI::CWnd::GetRandWndID(), x, y, btnWidth,btnHeight, this, 2, CHNN_MAX_ROW );

	m_btnOK.SetImage( PAINT_STATUS_NORMAL, BMP_BUTTON_DLG_OK, false );
	m_btnOK.SetImage( PAINT_STATUS_FOCUS, BMP_BUTTON_DLG_OK_F, false );

	m_btnExit.SetImage( PAINT_STATUS_NORMAL, BMP_BTN_EXIT4, false );
	m_btnExit.SetImage( PAINT_STATUS_FOCUS, BMP_BTN_EXIT4_FOCUS, false );

	////
	char str[32];
	int localChnnCount = CProduct::Instance()->LocalVideoInputNum();
	for ( int i = 0; i < m_chnnCount; i++ )
	{
		sprintf( str, "%d" , i + 1 );
		m_ckChnn[i].SetCaption( str , false ,false);
	}

	ULONGLONG CH = 0;
	int localVideoInputNum = 0, netVideoInputNum = 0;
	int maxChnn = 0, effectChnn = 0;

	IsSupportCH(CH, &effectChnn, &localVideoInputNum, &netVideoInputNum);

	maxChnn = localVideoInputNum + netVideoInputNum;
	// Init data
	int num = 0;
	for ( int i = 0; i < CHNN_MAX; i++ )
	{
		//2010-02-04 13:40:00 YSW
		if((0 == (CH & ((ULONGLONG)(0x01) << i))) && !m_bAllChnnEnable)
		{
			//无此通道
			m_ckChnn[i].SetEnable(false, false);
		}
		else if(0 == (m_authCH & ((ULONGLONG)(0x01) << i)))
		{
			//不具备权限，则设置为不使能。
			m_ckChnn[i].SetEnable(false, false);
		}
		else if( m_chnnSelect & ( ULONGLONG(1) << i ) )
		{
			//优先使用外部指定的选择项
			//具有权限且为选择状态，则应该设置为使能。
			if (!m_ckChnn[i].IsEnable())
			{
				m_ckChnn[i].SetEnable(true, false);
			}

			m_ckChnn[i].SetCheck( true, false);
			RefreshSelRec( i, true, false );

			++ num;
		}
		else
		{
			//具有权限，则应该设置为使能。
			if (!m_ckChnn[i].IsEnable())
			{
				m_ckChnn[i].SetEnable(true, false);
			}
		}
	}

	if(m_bAutoSel && (m_maxSel != 1))
	{
		//2010-02-04 13:50:00 YSW
		//再次检查一遍,如果还未到默认最大选择数目,则补选一部分.
		for ( int i = 0; (i < m_chnnCount) && (num < m_maxSel); i++ )
		{
			//必须是具有权限，且未被选择。
			if (m_ckChnn[i].IsEnable() && !m_ckChnn[i].IsCheck())
			{
				m_ckChnn[i].SetCheck( true, false);
				RefreshSelRec( i, true, false );

				++ num;	

				m_chnnSelect |= ((ULONGLONG)(0x01) << i);
			}
		}
	}

	for( int i = 0; i < CHNN_MAX_ROW; i++ )
	{
		if( m_uiLayOut[i][0] )
		{
			if( (!m_ckChnn[ i * 4 + 0 ].IsEnable() )&& (!m_ckChnn[ i * 4 + 1 ].IsEnable() )&&
				(!m_ckChnn[ i * 4 + 2 ].IsEnable() )&&(!m_ckChnn[ i * 4 + 3 ].IsEnable() ))
			{
				m_ckHeader[ i ].SetEnable( false, false);
			}
		}
	}
	CheckUI( false );

	////

	if(m_bUsedMiniMap)
	{
		x = CHNN_DLG_BORDER;
		y = rect.Height() - CHNN_DLG_BORDER - btnHeight + 2;

		m_btnMiniMap.Create(GetRandWndID(), x, y, btnWidth, btnHeight, this, 0, CHNN_MAX_ROW );

		m_btnMiniMap.SetImage( PAINT_STATUS_NORMAL, BMP_BUTTON_MINI_MAP, false );
		m_btnMiniMap.SetImage( PAINT_STATUS_FOCUS, BMP_BUTTON_MINI_MAP_F, false );

		unsigned short strCx = rect.Width() - 2 * CHNN_DLG_BORDER - btnWidth * 2 - btnWidth - 16;
		m_scMiniMap.Create(GetRandWndID(), x + btnWidth + 8, y + 2, strCx, m_length[STATIC_HEIGHT], this);
		m_scMiniMap.SetBKColor(COLOR_DLG_CHILD_BK, true);

		RefreshMiniCaption(false);
	}
}

void CSelectChnnDlg::SetChnnCount(int count,  ULONGLONG authCH, int maxSel /*= 1*/, ULONGLONG chnnBit /*= 0*/)
{
	assert(count > 0);
	assert( count <= CHNN_MAX );
	assert( maxSel > 0 && maxSel <= count );

	int row = (count+3) / 4;
	//assert(0 == count % 4);
	assert( row <= CHNN_MAX_ROW );

	char header = 0;
	if( maxSel >= 4 ) header = 1;

	memset( &m_uiLayOut, 0, sizeof(m_uiLayOut) );

	int chl_num = 0;
	for ( int j = 0; j < row; j++ )
	{
		m_uiLayOut[j][0] = header;
		for ( int i = 1; i < 5; i++ )
		{
			chl_num++;
			if( chl_num <= count )
			{
				m_uiLayOut[j][i] = 1;
			}
		}
	}

	m_authCH = authCH;

	m_uiLayOutInited = true;
	m_chnnSelRec.clear();
	m_chnnSelect = (chnnBit & m_authCH);

	m_chnnCount = count;
	m_maxSel = maxSel;
}

void CSelectChnnDlg::SetMiniMap(bool bUsed, int miniChnn)
{
	m_bUsedMiniMap = bUsed;
	m_chnnMiniMap = miniChnn;
}

GUI::CRect CSelectChnnDlg::GetDlgDefaultRect()
{
	unsigned short row = 0, col = 0;
	GetRowCol( row, col );

	int cx = CHNN_DLG_BORDER * 2 + (m_length[CHECK_WIDTH] + CHNN_CK_OFFSET_X) * col;
	int cy = CHNN_DLG_BORDER * 2 + m_length[BTN_HEIGHT] + (m_length[CHECK_HEIGHT] + CHNN_CK_OFFSET_Y) * row;

	return GUI::CRect( 0 , 0 , cx , cy );
}

void CSelectChnnDlg::OnOutOfDlg(unsigned long keyID)
{
	GUI::CDialog::OnOutOfDlg(keyID);

	if (KEY_VIRTUAL_ENTER == keyID)
	{
		m_uiLayOutInited = false;
		EndDialog(DLG_ESC);
	}
}

void CSelectChnnDlg::OnPaint( PAINT_STATUS status )
{
	GUI::CDialog::OnPaint( status );

	//// border
	GUI::CWnd::DrawRectBorder( m_rect.m_left, m_rect.m_top, m_rect.Width(), m_rect.Height(), COLOR_LINE_BLACK, 1);
	GUI::CWnd::DrawRectBorder( m_rect.m_left + 1, m_rect.m_top + 1, m_rect.Width() - 2, m_rect.Height() - 2, COLOR_LINE_GRAY, 1);

	unsigned short xLeft = CHNN_DLG_LEFT, yTop = CHNN_DLG_TOP;
	unsigned short cx = m_length[CHECK_WIDTH] + CHNN_CK_OFFSET_X, cy = m_length[CHECK_HEIGHT] + CHNN_CK_OFFSET_Y;

	unsigned short row = 0, col = 0;
	GetRowCol( row, col );

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


BEGIN_MSG_MAP(CSelectChnnDlg, GUI::CDialog)
	ON_MSG( m_btnExit.GetID(), KEY_VIRTUAL_ENTER, OnBtnExit)
	ON_MSG( m_btnOK.GetID(), KEY_VIRTUAL_ENTER, OnBtnOK)
	ON_MSG_CTRL_RANGE( CTRLID_SELECT_CHNN_CHECK_BEGIN, CTRLID_SELECT_CHNN_CHECK_END, KEY_VIRTUAL_ENTER, OnCheckClick )
	ON_MSG( m_btnMiniMap.GetID(), KEY_VIRTUAL_ENTER, OnBtnMiniMap)
END_MSG_MAP()

unsigned long CSelectChnnDlg::OnBtnExit()
{
	m_uiLayOutInited = false;
	return OnCancel();
}

unsigned long CSelectChnnDlg::OnBtnOK()
{
	m_chnnSelect = 0;
	for ( int i = 0; i < m_chnnCount; i++ )
	{
		if( m_ckChnn[i].IsCheck() )
		{
			m_chnnSelect |= ( ULONGLONG(1) << i );
		}
	}

	m_uiLayOutInited = false;
	return OnOK();
}

unsigned long CSelectChnnDlg::OnChnnClick( unsigned long ctrlID )
{
	for ( int i = 0; i < m_chnnCount; i ++ )
	{
		if( ctrlID == m_ckChnn[i].GetID() )
		{
			RefreshSelRec( i, m_ckChnn[i].IsCheck(), true );

			if(m_bUsedMiniMap && (m_chnnMiniMap == i))
			{
// 				m_chnnMiniMap++;
// 
// 				if(m_chnnMiniMap >= m_chnnCount)
// 				{
// 					m_chnnMiniMap = 0;
// 				}
				m_chnnMiniMap = -1;

				RefreshMiniCaption(true);
			}
		}
	}
	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CSelectChnnDlg::OnHeaderClick( unsigned long ctrlID )
{
	for ( int i = 0; i < CHNN_MAX_ROW; i ++ )
	{
		if( ctrlID == m_ckHeader[i].GetID() )
		{
			for ( int j = 0; j < 4; j ++ )
			{
				if( m_ckHeader[i].IsCheck() )
				{
					if (m_ckChnn[i*4 + j].IsEnable())
					{
						RefreshSelRec( i*4 + j, false, false );
						m_ckChnn[i*4 + j].SetCheck( true, true);
						RefreshSelRec( i*4 + j, true, true );
					}
				}
				else
				{
					RefreshSelRec( i*4 + j, false, false );
					m_ckChnn[i*4 + j].SetCheck( false, true);
				}
			}
		}
	}

	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CSelectChnnDlg::OnCheckClick( unsigned long ctrlID )
{
	OnChnnClick( ctrlID );

	OnHeaderClick( ctrlID );

	CheckUI( true );

	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CSelectChnnDlg::OnBtnMiniMap()
{
	CSelectChnnDlg selectDlg;
	selectDlg.SetAllChnnEnable(false);

	ULONGLONG chnn = 0;
	if(m_chnnMiniMap >= 0)
	{
		chnn = ULONGLONG(0x1 << m_chnnMiniMap);
	}

	ULONGLONG selChnn = 0;
	for ( int i = 0; i < m_chnnCount; i++ )
	{
		if( m_ckChnn[i].IsCheck() )
		{
			selChnn |= ( ULONGLONG(1) << i );
		}
	}

	ULONGLONG authCH = m_authCH;
	authCH &= (~selChnn);
	selectDlg.SetChnnCount(m_chnnCount, authCH, 1, chnn);
	selectDlg.SetMiniMap(false, -1);
	selectDlg.SetAutoSel(false);

	GUI::CRect rect = m_btnMiniMap.GetWindowRect();
	GUI::CRect chnnRect = selectDlg.GetDlgDefaultRect();
	unsigned short x = (rect.m_left > chnnRect.Width()/2) ? (rect.m_left - chnnRect.Width()/2) : rect.m_left;
	unsigned short y = (rect.m_top > m_height / 2) ? (rect.m_top - chnnRect.Height()) : rect.m_bottom;
	
	if(DLG_OK == selectDlg.DoModal(NULL, x, y, chnnRect.Width(), chnnRect.Height()))
	{
		ULONGLONG CH = selectDlg.GetSelChnn();

		m_chnnMiniMap = -1;
		for(int i = 0; i < m_chnnCount; i++)
		{
			if(CH & ULONGLONG(0x1 << i))
			{
				m_chnnMiniMap = i;
				break;
			}
		}
	}

	RefreshMiniCaption(true);

	return KEY_VIRTUAL_MSG_STOP;
}

void CSelectChnnDlg::RefreshSelRec( int chnn, bool checked, bool bRefresh /*= true*/ )
{
	std::list<unsigned int>::iterator it;
	if( checked )
	{
		if( m_maxSel < m_chnnSelRec.size() + 1 )
		{
			it = m_chnnSelRec.begin();
			if( it != m_chnnSelRec.end() )
			{
				int pre = *it;
				m_ckChnn[pre].SetCheck( false, bRefresh );
				m_chnnSelRec.pop_front();
			}
		}
		m_chnnSelRec.push_back( chnn );
	}
	else
	{
		it = std::find( m_chnnSelRec.begin(), m_chnnSelRec.end(), chnn );
		if( it != m_chnnSelRec.end() ) 
		{
			m_chnnSelRec.erase( it );
		}
	}
}

void CSelectChnnDlg::CheckUI( bool bRefresh /*= false*/ )
{
	for( int i = 0; i < CHNN_MAX_ROW; i++ )
	{
		if (m_ckHeader[ i ].IsEnable())
		{
			if( m_uiLayOut[i][0] )
			{
				if( (m_ckChnn[ i * 4 + 0 ].IsCheck() || !m_ckChnn[ i * 4 + 0 ].IsEnable() )&& 
					(m_ckChnn[ i * 4 + 1 ].IsCheck() || !m_ckChnn[ i * 4 + 1 ].IsEnable() )&&
					(m_ckChnn[ i * 4 + 2 ].IsCheck() || !m_ckChnn[ i * 4 + 2 ].IsEnable() )&&
					(m_ckChnn[ i * 4 + 3 ].IsCheck() || !m_ckChnn[ i * 4 + 3 ].IsEnable() ))
				{
					m_ckHeader[ i ].SetCheck( true, bRefresh );
				}
				else
				{
					m_ckHeader[ i ].SetCheck( false, bRefresh );
				}
			}
		}
	}
}

void CSelectChnnDlg::GetRowCol( unsigned short& row, unsigned short& col )
{
	assert( m_uiLayOutInited );

	int tmpRow = 0, tmpCol = 4;
	if( m_uiLayOut[0][0] ) 
	{
		tmpCol = 5;
	}
	for ( int i = 0; i < CHNN_MAX_ROW; i++ )
	{
		if( m_uiLayOut[i][1] ) 
		{
			tmpRow = i + 1;
		}
	}

	row = tmpRow;
	col = tmpCol;
}

void CSelectChnnDlg::RefreshMiniCaption(bool bRefresh)
{
	if(m_bUsedMiniMap)
	{
		if(m_chnnMiniMap >= 0)
		{
			char strChnn[128] = {0};

			snprintf(strChnn, 128, "%s:%d", m_pStrTable->String(E_STR_ID_CH), m_chnnMiniMap + 1);
			m_scMiniMap.SetCaption(strChnn, bRefresh);	
		}
		else
		{
			m_scMiniMap.SetCaption(m_pStrTable->String(E_STR_ID_NONE), bRefresh);
		}
	}
}

//end
