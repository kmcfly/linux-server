#include "ScrollDialog.h"

using namespace GUI;

GUI::CScrollDialog::CScrollDialog()
{
	m_ctrlType = GUI::WND_TYPE_SCROLL_DIALOG;

	m_cur_pos.x = -1;
	m_cur_pos.y = -1;

	m_color.backgroud = COLOR_DLG_CHILD_BK;
	m_color.border = COLOR_LINE_GRAY;
	m_color.mouse_over = COLOR_SELECT;
	m_color.wnd_border = COLOR_LINE_GRAY;

	memset(&m_size,0,sizeof(ITEM_SIZE));
	m_size.dlg_border = 1;
}

GUI::CScrollDialog::~CScrollDialog()
{

}

void GUI::CScrollDialog::OnInitial()
{
	GUI::CDialog::OnInitial();
	m_wnd.Create( GetRandWndID(), m_size.wnd_left, m_size.wnd_top, m_rect.Width()-m_size.wnd_left-m_size.wnd_right, m_rect.Height()-m_size.wnd_top-m_size.wnd_bottom, this, WND_CHILD | WND_VISIBLE, 0, 0 );

	//m_wnd.SetSizeOffset( 80,80);
	//m_wnd.SetViewMaxSize(100,100);

	//for ( int i = 0; i<20;i ++ )
	//{
	//	GUI::CRect rect(0,i * 80,120,i*80 + m_length[BTN_HEIGHT]);
	//	GUI::CWnd * pBtn = m_wnd.AddWnd(WND_TYPE_BUTTON,GetRandWndID(),rect,0,1 );
	//	char str[128] = {0};
	//	sprintf(str,"%d",i+1);
	//	pBtn->SetCaption(str,false);
	//}
}

void GUI::CScrollDialog::OnPostInitial()
{
	m_cur_pos.x = -1;
	m_cur_pos.y = -1;
	m_scroll_x = 0;
	m_scroll_y = 0;
	m_init_scroll_x = false;
	m_init_scroll_y = false;

	unsigned long nx, ny;
	unsigned long maxx,maxy;
	int maxCx,maxCy;

	/*
	缺陷:重新计算wnd和滚动条位置,必须预先给滚动条预留出足够的空间。
	窗口移动裁剪可能将此处处于误差的中的控件错误的裁剪掉。
	*/

	m_wnd.GetPageCount( nx,ny);
	m_wnd.GetViewMaxSize(maxCx,maxCy);
	m_wnd.GetMaxPageCount( maxCx, maxCy,maxx,maxy);

	unsigned long wndCx = m_rect.Width()-m_size.wnd_left-m_size.wnd_right;
	unsigned long wndCy = m_rect.Height()-m_size.wnd_top-m_size.wnd_bottom;

	bool reSizeX = ( maxy > ny ) ? true : false ;//垂直方向上需要滚动条,表示需要调整X方向的窗口
	bool reSizeY = ( maxx > nx ) ? true : false ;//水平方向上需要滚动条,表示需要调整Y方向的窗口

	//计算wnd窗口大小
	if( reSizeX )
	{
		wndCx -= m_length[SCROLL_WIDTH] + 2;
	}

	if( reSizeY )
	{
		wndCy -= m_length[SCROLL_WIDTH] + 2;
	}

	//wnd 窗口可以调整位置后引起,nx,ny计算误差,设定wnd窗口后,重新计算nx,ny
	m_wnd.SetWndPos( m_size.wnd_left,m_size.wnd_top,wndCx,wndCy);
	m_wnd.GetPageCount( nx,ny);

	//创建滚动条
	if( reSizeX )
	{
		m_scroll_v.Create(GetRandWndID(),m_size.wnd_left+wndCx+1,m_size.wnd_top+1,wndCy-2,this,0,1,false);
		m_scroll_v.SetRange(0,maxy - ny);
		m_init_scroll_y = true;
	}

	if( reSizeY )
	{
		m_scroll_h.Create(GetRandWndID(),m_size.wnd_left+1,m_size.wnd_top+wndCy+1,wndCx-2,this,1,0,true);
		m_scroll_h.SetRange(0,maxx - nx);
		m_init_scroll_x = true;
	}

	ScrollViewTo(m_scroll_x,m_scroll_y,false);
	m_wnd.CreateWndFocusMap();
}

void GUI::CScrollDialog::OnDestroy()
{
	GUI::CDialog::OnDestroy();

	m_posDrawBorder.clear();

	m_cur_pos.x = -1;
	m_cur_pos.y = -1;
}

void GUI::CScrollDialog::OnPaint(PAINT_STATUS status)
{
	unsigned short off_left = (m_size.wnd_left-m_size.dlg_border>0)?(m_size.wnd_left-m_size.dlg_border):0;
	unsigned short off_top = (m_size.wnd_top-m_size.dlg_border>0)?(m_size.wnd_top-m_size.dlg_border):0;
	unsigned short off_right = (m_size.wnd_right-m_size.dlg_border>0)?(m_size.wnd_right-m_size.dlg_border):0;
	unsigned short off_bottom = (m_size.wnd_bottom-m_size.dlg_border>0)?(m_size.wnd_bottom-m_size.dlg_border):0;

	GUI::CWnd::DrawRect(m_rect.m_left, m_rect.m_top, m_rect.Width(), m_rect.Height(), m_color.backgroud);
	GUI::CWnd::DrawRectBorder(m_rect.m_left+off_left, m_rect.m_top+off_top, m_rect.Width()-off_left-off_right, m_rect.Height()-off_top-off_bottom, m_color.border, m_size.dlg_border);

	std::vector<POS_INDEX>::iterator it;
	for ( it = m_posDrawBorder.begin(); it != m_posDrawBorder.end(); it++ )
	{
		int xView = -1, yView = -1;
		if( m_wnd.GetCurViewMap(it->x,it->y,xView,yView) )
		{
			//处于可视范围内,则绘制
			m_wnd.DrawViewBorder( xView,yView,m_color.wnd_border);
		}
	}
}

void GUI::CScrollDialog::OnErase(PAINT_STATUS status)
{
	GUI::CDialog::OnErase(status);
/*
	if( PAINT_STATUS_FOCUS == status &&
		(m_cur_pos.x >= 0 && m_cur_pos.y >= 0) )
	{
		m_wnd.DrawViewBorder(m_cur_pos.x,m_cur_pos.y,m_color.mouse_over,m_size.mouse_over_offset);
	}
*/
}

bool GUI::CScrollDialog::CreateWndFocusMap()
{
	return m_wnd.CreateWndFocusMap();
}

GUI::CWnd* GUI::CScrollDialog::AddWnd(WND_TYPE wndType, unsigned long ctrlID, const GUI::CRect& rect, int xFocus,int yFocus, int index )
{
	return m_wnd.AddWnd(wndType,ctrlID,rect,xFocus,yFocus,index);
}

GUI::CScrollWnd& GUI::CScrollDialog::GetScrollWnd()
{
	return m_wnd;
}

void GUI::CScrollDialog::AddDrawBorderPos( int x, int y )
{
	POS_INDEX index;
	index.x = x;
	index.y = y;

	m_posDrawBorder.push_back( index );
}

void GUI::CScrollDialog::ClearAllDrawBorderPos()
{
	m_posDrawBorder.clear();
}


void GUI::CScrollDialog::SetCurScrollPos(int x,int y,bool bRefresh/*=true*/)
{
	if( m_init_scroll_x )
	{
		x = (x <= m_scroll_h.GetRangeMin())? m_scroll_h.GetRangeMin():x;
		x = (x >= m_scroll_h.GetRangeMax())?m_scroll_h.GetRangeMax():x;
		m_scroll_h.SetCurPos(x,false);
	}
	else
	{
		x = 0;
	}

	if( m_init_scroll_y )
	{
		y = (y <= m_scroll_v.GetRangeMin())? m_scroll_v.GetRangeMin():y;
		y = (y >= m_scroll_v.GetRangeMax())?m_scroll_v.GetRangeMax():y;
		m_scroll_v.SetCurPos(y,false);
	}
	else
	{
		y = 0;
	}

	ScrollViewTo(x,y,bRefresh);
}

void GUI::CScrollDialog::GetCurScrollPos(int& x, int& y)
{
	x = m_scroll_x;
	y = m_scroll_y;
}

void GUI::CScrollDialog::ScrollViewTo(int xIndex,int yIndex,bool bRefresh/*=true*/)
{
	m_scroll_x = xIndex;
	m_scroll_y = yIndex;

	m_wnd.ScrollViewTo( m_scroll_x, m_scroll_y);

	if( bRefresh )
	{
		Repaint();
	}
}

void GUI::CScrollDialog::SetScrollWndPos(int left, int top, int right, int bottom)
{
	m_size.wnd_left = left;
	m_size.wnd_top = top;
	m_size.wnd_right = right;
	m_size.wnd_bottom = bottom;
}

void GUI::CScrollDialog::SetDlgBorder(int border)
{
	m_size.dlg_border = border;
}

BEGIN_MSG_MAP(GUI::CScrollDialog, GUI::CDialog)
	ON_MSG_EX(m_scroll_v.GetID(), KEY_VIRTUAL_MSG_DEFAULT_TAG, OnScrollchangeV)
	ON_MSG_EX(m_scroll_v.GetID(),KEY_VIRTUAL_ENTER, OnScrollchangeV)
	ON_MSG_EX(m_scroll_v.GetID(),KEY_VIRTUAL_DRAG, OnScrollchangeV)
	ON_MSG_EX(m_scroll_v.GetID(),KEY_VIRTUAL_BLUP, OnScrollchangeV)
	ON_MSG_EX(m_scroll_h.GetID(), KEY_VIRTUAL_MSG_DEFAULT_TAG, OnScrollchangeH)	
	ON_MSG_EX(m_scroll_h.GetID(), KEY_VIRTUAL_ENTER, OnScrollchangeH)
	ON_MSG_EX(m_scroll_h.GetID(), KEY_VIRTUAL_DRAG, OnScrollchangeH)
	ON_MSG_EX(m_scroll_h.GetID(), KEY_VIRTUAL_BLUP, OnScrollchangeH)
	ON_MSG_EX(CTRLID_NULL, KEY_VIRTUAL_MOVE,	OnMove)
	ON_MSG_EX(CTRLID_NULL, KEY_VIRTUAL_DEC,	OnAdd)
	ON_MSG_EX(CTRLID_NULL, KEY_VIRTUAL_ADD,	OnDec)
	//ON_MSG_EX(CTRLID_NULL, KEY_VIRTUAL_UP,		OnMove)
	//ON_MSG_EX(CTRLID_NULL, KEY_VIRTUAL_DOWN,	OnMove)
END_MSG_MAP()

unsigned long GUI::CScrollDialog::OnScrollchangeV(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	//int pos = keyID & (~KEY_VIRTUAL_MSG_DEFAULT_TAG);
	//printf("v pos=%d\r\n",pos);
	int pos = m_scroll_v.GetPos();

	ScrollViewTo(m_scroll_x,pos,true);
	//m_scroll_y = pos;
	//m_wnd.ScrollViewTo( m_scroll_x, m_scroll_y);

	//Repaint();

	return KEY_VIRTUAL_MSG_STOP; 
}

unsigned long GUI::CScrollDialog::OnScrollchangeH(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	//int pos = keyID & (~KEY_VIRTUAL_MSG_DEFAULT_TAG);
	//printf("h pos=%d\r\n",pos);
	int pos = m_scroll_v.GetPos();

	ScrollViewTo(pos,m_scroll_y,true);
	//m_scroll_x = pos;
	//m_wnd.ScrollViewTo( m_scroll_x, m_scroll_y );

	//Repaint();

	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long GUI::CScrollDialog::OnMove(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	/*
	int  x_view = xPos;
	int y_view = yPos;
	ScreenToClient( x_view, y_view );

	int xIndex = m_cur_pos.x,yIndex = m_cur_pos.y;
	if( m_wnd.GetCurViewIndex(x_view,y_view,xIndex,yIndex) )
	{
		if( xIndex != m_cur_pos.x ||
			yIndex != m_cur_pos.y )
		{
			m_cur_pos.x = xIndex;
			m_cur_pos.y = yIndex;

			//printf("(%s,line=%d)pos.x=%d,pos.y=%d\r\n",__FILE__,__LINE__,m_cur_pos.x,m_cur_pos.y);

			Repaint();
		}
	}
	else
	{
		int pre_x = m_cur_pos.x;
		int pre_y = m_cur_pos.y;

		m_cur_pos.x = -1;
		m_cur_pos.y = -1;

		if( xIndex != pre_x || yIndex != pre_y )
		{
			Repaint();
		}
	}
	*/
	return KEY_VIRTUAL_MSG_CONTINUE;
}

unsigned long GUI::CScrollDialog::OnAdd(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	if( m_init_scroll_y && m_scroll_y + 1 <= m_scroll_v.GetRangeMax() )
	{
		SetCurScrollPos(m_scroll_x, m_scroll_y + 1, true);
	}
	return KEY_VIRTUAL_MSG_CONTINUE;
}


unsigned long GUI::CScrollDialog::OnDec(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	if( m_init_scroll_y && m_scroll_y - 1 >= m_scroll_v.GetRangeMin() )
	{
		SetCurScrollPos(m_scroll_x, m_scroll_y - 1, true);
	}

	return KEY_VIRTUAL_MSG_CONTINUE;
}
