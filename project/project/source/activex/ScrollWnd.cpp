
#include "ScrollWnd.h"
#include "ComboCtrl.h"
#include "CheckCtrl.h"
#include "CalendarCtrl.h"
#include "DateCtrl.h"
#include "TimeCtrl.h"
#include <algorithm>

using namespace GUI;

GUI::CScrollWnd::WND_ITEM& GUI::CScrollWnd::WND_ITEM::operator=(const GUI::CScrollWnd::WND_ITEM& item)
{
	Index = item.Index;
	Show = item.Show;
	pWnd = item.pWnd;
	Rect = item.Rect;
	return *this;
}

GUI::CScrollWnd::CScrollWnd()
{
	m_ctrlType = GUI::WND_TYPE_SCROLL_WND;

	m_xSizeOff = 0;
	m_ySizeOff = 0;
	m_xIndexOff = 0;
	m_yIndexOff = 0;

	m_maxCx = 0;
	m_maxCy = 0;
}

GUI::CScrollWnd::~CScrollWnd()
{

}

bool GUI::CScrollWnd::CreateWndFocusMap()
{
	return CreateFocusMap();
}

void GUI::CScrollWnd::OnPaint(PAINT_STATUS status)
{
	//GUI::CWnd::DrawRect(m_rect.m_left, m_rect.m_top, m_rect.Width(), m_rect.Height(), COLOR_DLG_CHILD_BK);
	//CWnd::DrawRectBorder(m_rect.m_left,m_rect.m_top,m_rect.Width(),m_rect.Height(),COLOR_LINE_GRAY,1);
}

void GUI::CScrollWnd::OnDestroy()
{
	GUI::CWnd::OnDestroy();

	std::map<unsigned long,WND_ITEM>::iterator it;
	for ( it = m_mapPosOriginally.begin();it!=m_mapPosOriginally.end();it++)
	{
		delete it->second.pWnd;
	}

	m_mapPosOriginally.clear();
}

GUI::CWnd* GUI::CScrollWnd::AddWnd(WND_TYPE wndType, unsigned long ctrlID, const GUI::CRect& rect, int xFocus,int yFocus, int index)
{
	return DoAddWnd(wndType,ctrlID,rect,xFocus,yFocus,index);
}

GUI::CWnd* GUI::CScrollWnd::AddWnd(WND_TYPE wndType, unsigned long ctrlID, const GUI::CRect& rect, int xIndex, int yIndex, int xFocus,int yFocus, unsigned long align, int index, int xOffset, int yOffset)
{
	GUI::CRect rectReal = rect;

	int x = xIndex * m_xSizeOff;
	int y = yIndex * m_ySizeOff;
	int x_offset = m_length[LIST_ITEM_XPOS], y_offset = m_length[LIST_ITEM_YPOS];

	if(align & SW_WND_OFFSET)
	{
		x_offset = xOffset;
		y_offset = yOffset;
	}
	else
	{
		if(align & SW_WND_LEFT)
		{
			x_offset = m_length[LIST_ITEM_XPOS];
		}
		else if((align & SW_WND_RIGHT) && (m_xSizeOff > rectReal.Width() + m_length[LIST_ITEM_XPOS]))
		{
			x_offset = m_xSizeOff - rectReal.Width() - m_length[LIST_ITEM_XPOS];
		}
		else if((align & SW_WND_CENTER_X) && (m_xSizeOff > rectReal.Width()))
		{
			x_offset = (m_xSizeOff - rectReal.Width()) / 2;
		}

		if(align & SW_WND_TOP)
		{
			y_offset = m_length[LIST_ITEM_YPOS];
		}
		else if((align & SW_WND_BOTTOM) && (m_ySizeOff > rectReal.Height() + m_length[LIST_ITEM_YPOS]))
		{
			y_offset = m_ySizeOff - rectReal.Height() - m_length[LIST_ITEM_YPOS];
		}
		else if((align & SW_WND_CENTER_Y) && (m_ySizeOff > rectReal.Height()))
		{
			y_offset = (m_ySizeOff - rectReal.Height()) / 2;
		}
	}

	rectReal.Offset(x + x_offset, y + y_offset);

	return DoAddWnd(wndType, ctrlID, rectReal, xFocus, yFocus, index);
}

void GUI::CScrollWnd::ResetWndRect(int index, const GUI::CRect& rect)
{
	std::map<unsigned long,WND_ITEM>::iterator it;

	for ( it = m_mapPosOriginally.begin();it!=m_mapPosOriginally.end();it++ )
	{
		if ( (it->second.Index >= 0) && (it->second.Index == index) )
		{
			it->second.Rect = rect;
			return;
		}
	}

	gui_trace_np(true, printf("CScrollWnd::ResetWndRect : index = %d.", index));
	gui_assert(false, Dump());
}

void GUI::CScrollWnd::ShowSubWnd(int index, bool show )
{
	std::map<unsigned long,WND_ITEM>::iterator it;

	for ( it = m_mapPosOriginally.begin();it!=m_mapPosOriginally.end();it++ )
	{
		if ( (it->second.Index >=0) && (it->second.Index == index) )
		{
			it->second.Show = show;
			return;
		}
	}
}

void GUI::CScrollWnd::ShowSubWndID(unsigned long ctrlID, bool show )
{
	std::map<unsigned long,WND_ITEM>::iterator it;
	it = m_mapPosOriginally.find(ctrlID);
	if( it != m_mapPosOriginally.end() )
	{
		it->second.Show = show;
	}
}

void  GUI::CScrollWnd::ScrollView(int x, int y)
{
	DoScrollWnd(x,y);
}

void GUI::CScrollWnd::ScrollViewTo(int xIndex,int yIndex)
{
	m_xIndexOff = xIndex;
	m_yIndexOff = yIndex;

	DoScrollWnd( m_xIndexOff * m_xSizeOff, m_yIndexOff * m_ySizeOff );
}

//void GUI::CScrollWnd::SetWndPos(int x, int y, int cx, int cy)
//{
//	CWnd::SetWndPos(x,y,cx,cy);
//}

void GUI::CScrollWnd::SetSizeOffset(int sizeX,int sizeY )
{
	m_xSizeOff = sizeX;
	m_ySizeOff = sizeY;
}

void GUI::CScrollWnd::SetViewMaxSize(int maxCx,int maxCy)
{
	m_maxCx = maxCx;
	m_maxCy = maxCy;
}

void GUI::CScrollWnd::GetViewMaxSize(int& cx, int& cy)
{
	cx = m_maxCx;
	cy = m_maxCy;
}

GUI::CWnd* GUI::CScrollWnd::GetWnd(int index)
{
	std::map<unsigned long,WND_ITEM>::iterator it;

	for ( it = m_mapPosOriginally.begin();it!=m_mapPosOriginally.end();it++ )
	{
		if( it->second.Index >= 0 && it->second.Index == index )
		{
			return it->second.pWnd;
		}
	}

	printf("(%s,line=%d) wnd index=%d con't find.\r\n",__FILE__,__LINE__,index);
	return NULL;
}

void GUI::CScrollWnd::GetViewPos(int xIndex,int yIndex,unsigned long& x, unsigned long& y )
{
	x = xIndex * m_xSizeOff;
	y = yIndex * m_ySizeOff;
}

void GUI::CScrollWnd::GetViewSize(int xIndex,int yIndex,unsigned long& cx, unsigned long& cy )
{
	cx = m_xSizeOff;
	cy = m_ySizeOff;
}

bool GUI::CScrollWnd::GetCurViewIndex(int x,int y,int& xIndex, int& yIndex)
{
	if( x<0 || y<0 )
	{
		return false;
	}

	int xx = x + m_xIndexOff * m_xSizeOff;
	int yy = y + m_yIndexOff * m_ySizeOff;

	int tmpX = xx / m_xSizeOff;
	int tmpY = yy / m_ySizeOff;

	unsigned long xCount = 0, yCount = 0;
	GetPageCount(xCount,yCount);
	if( tmpX > (m_xIndexOff+xCount) ||
		tmpY >= (m_yIndexOff+yCount) )
	{
		return false;
	}

	xIndex = tmpX /*- m_xIndexOff*/;
	yIndex = tmpY /*- m_yIndexOff*/;
	return true;
}

bool GUI::CScrollWnd::GetCurViewMap(int xIndex,int yIndex,int& xWnd,int& yWnd)
{

	int tmpX = xIndex - m_xIndexOff;
	int tmpY = yIndex - m_yIndexOff;

	if( tmpX < 0 || tmpY < 0 )
	{
		return false;
	}

	unsigned long xCount = 0, yCount = 0;
	GetPageCount(xCount,yCount);

	if( tmpX >= xCount || tmpY >= yCount )
	{
		return false;
	}
	
	xWnd = tmpX;
	yWnd = tmpY;
	return true;
}

void GUI::CScrollWnd::GetPageCount(unsigned long& x,unsigned long& y)
{
	GetMaxPageCount( m_rect.Width(),m_rect.Height(),x,y );
}

void GUI::CScrollWnd::GetMaxPageCount(unsigned long cx, unsigned long cy, unsigned long& x,unsigned long& y)
{
	assert( 0 != m_xSizeOff);
	assert( 0 != m_ySizeOff);

	unsigned long num_x = cx / m_xSizeOff;
	if( (cx % m_xSizeOff) > (m_xSizeOff / 5) &&
		(cx % m_xSizeOff) < (m_xSizeOff * 4/ 5) )
	{
		num_x += 1;
	}

	unsigned long num_y = cy / m_ySizeOff;
	if( (cy % m_ySizeOff) > (m_xSizeOff / 5) &&
		(cy % m_ySizeOff) < (m_xSizeOff * 4 / 5) )
	{
		num_y += 1;
	}

	x = num_x;
	y = num_y;
}

void GUI::CScrollWnd::DrawViewBorder(int xIndex, int yIndex, unsigned long color,int offset)
{
	unsigned long x, y;
	GetViewPos(xIndex,yIndex,x,y);

	unsigned long cx = m_xSizeOff;
	unsigned long cy = m_ySizeOff;

	//
	GUI::CRect rect(m_rect.m_left+x+offset,m_rect.m_top+y,m_rect.m_left+x+cx,m_rect.m_top+y+cy);
	rect.Modify(m_rect.m_left,m_rect.m_top,m_rect.Width(),m_rect.Height());
	GUI::CWnd::DrawRectBorder(rect.m_left+offset,rect.m_top+offset,rect.Width()-2*offset,rect.Height()-2*offset,color,1);
}

int GUI::CScrollWnd::GetItemWndIndex(unsigned long id)
{
	std::map<unsigned long,WND_ITEM>::iterator iter = m_mapPosOriginally.find(id);
	if( iter != m_mapPosOriginally.end() )
	{
		return iter->second.Index;
	}

	return -1;
}

unsigned long GUI::CScrollWnd::OnCommand (unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	if (KEY_VIRTUAL_MSG_STOP == CWnd::OnCommand (ctrlID, keyID, xPos, yPos)) return KEY_VIRTUAL_MSG_STOP;

	if( keyID == KEY_VIRTUAL_ENTER )
	{
		unsigned long msg = KEY_VIRTUAL_MSG_DEFAULT_TAG;

		std::map<unsigned long,WND_ITEM>::iterator it = m_mapPosOriginally.find( ctrlID );
		if( it != m_mapPosOriginally.end() )
		{
			unsigned long index = (static_cast<unsigned long>(0x7FF) << 20);
			if( it->second.Index >= 0 )
			{
				index = (static_cast<unsigned long>(it->second.Index) << 20);
			}
			unsigned long id = it->second.pWnd->GetID();

			return (msg + (index | id));
		}
	}

	return keyID;
}

GUI::CWnd* GUI::CScrollWnd::DoAddWnd(WND_TYPE wndType, unsigned long ctrlID, const GUI::CRect& rect, int xFocus,int yFocus,int index)
{
	CWnd* pWnd = NULL;

	switch ( wndType )
	{
	case WND_TYPE_STATIC:
		{
			GUI::CStatic* pSc = new GUI::CStatic();
			pSc->Create(ctrlID,0, 0, rect.Width(), rect.Height(), this);
			pWnd=pSc;
		}
		break;
	case WND_TYPE_BUTTON:
		{
			GUI::CButton* pBtn = new GUI::CButton();
			pBtn->Create(ctrlID,0, 0, rect.Width()-1, rect.Height()-1, this, xFocus, yFocus);
			pWnd=pBtn;
		}
		break;
	case WND_TYPE_CHECK:
		{
			CCheckCtrl* pCheck = new GUI::CCheckCtrl();
			pCheck->Create(ctrlID,0,0,rect.Width()-1,rect.Height()-1,this,xFocus,yFocus);
			pWnd = pCheck;
		}
		break;
	case WND_TYPE_COMBO:
		{
			GUI::CComboCtrl* pCombo = new GUI::CComboCtrl();
			pCombo->Create(ctrlID,0,0,rect.Width(),rect.Height(),this,xFocus,yFocus);
			pWnd=pCombo;
		}
		break;
	case WND_TYPE_COMBO_ITEM:
		{
			GUI::CComboItem* pItem = new GUI::CComboItem();
			pItem->Create(ctrlID,0,0,rect.Width(),rect.Height(),this,xFocus,yFocus);
			pWnd=pItem;
		}
		break;
	case WND_TYPE_CALENDAR:
		{
			GUI::CCalendarCtrl* pCalendar = new GUI::CCalendarCtrl();
			pCalendar->Create(ctrlID, 0, 0, rect.Width(), rect.Height(), this, xFocus, yFocus);
			pWnd = pCalendar;
		}
		break;
	case WND_TYPE_DATE:
		{
			GUI::CDateCtrl* pDate = new GUI::CDateCtrl();
			pDate->Create(ctrlID, 0, 0, rect.Width(), rect.Height(), this, xFocus, yFocus);
			pWnd = pDate;
		}
		break;
	case WND_TYPE_TIME:
		{
			GUI::CTimeCtrl* pTime = new GUI::CTimeCtrl();
			pTime->Create(ctrlID, 0, 0, rect.Width(), rect.Height(), this, xFocus, yFocus);
			pWnd = pTime;
		}
		break;
	case WND_TYPE_EDIT:
		{
			GUI::CEditCtrl * pEdit = new GUI::CEditCtrl();
			pEdit->Create(ctrlID, 0, 0, rect.Width(), rect.Height(), this, xFocus, yFocus);
			pWnd = pEdit;
		}
		break;
	case WND_TYPE_NUM_EDIT:
		{
			GUI::CNumEditCtrl * pNumEdit = new GUI::CNumEditCtrl();
			pNumEdit->Create(ctrlID, 0, 0, rect.Width(), rect.Height(), this, xFocus, yFocus);
			pWnd = pNumEdit;
		}
		break;
	default:
		assert(false);//如果缺少对应控件,可以按以上格式添加.
		break;
	}

	WND_ITEM item;
	item.Index = index;
	item.Show = true;
	item.pWnd = pWnd;
	item.Rect = rect;
	if( NULL != pWnd && DoAddPosOriginally( ctrlID, item ) )
	{
		return pWnd;
	}
	else
	{
		printf("(%s,line=%d) wnd id=%d redefine.\r\n",__FILE__,__LINE__,ctrlID);
		assert( false );
		return NULL;
	}
}

bool GUI::CScrollWnd::DoAddPosOriginally( unsigned long id, const WND_ITEM& item )
{
	//printf("(%s,line=%d)  wnd index=%d id =%d item id = %d.\r\n",__FILE__,__LINE__,item.Index,id,item.pWnd->GetID());
	return m_mapPosOriginally.insert( std::make_pair(id,item) ).second;
}

void GUI::CScrollWnd::DoScrollWnd(int x, int y)
{
	std::map<unsigned long,WND_ITEM>::iterator it;

	for ( it = m_mapPosOriginally.begin();it!=m_mapPosOriginally.end();it++ )
	{
		it->second.pWnd->Show(false,false);
		if( !it->second.Show ) continue;

		//移动窗口
		int xReal = m_rect.m_left + it->second.Rect.m_left - x;
		int yReal = m_rect.m_top + it->second.Rect.m_top - y;

		//严格的检查将导致控件很容易被裁剪掉
		//容许误差在4个像素之内
		GUI::CRect rect( xReal+2, yReal+2, xReal + it->second.Rect.Width()-4,yReal+it->second.Rect.Height()-4);
		if(m_showRect.PtInRect(rect.m_left,rect.m_top) && 
			m_showRect.PtInRect(rect.m_right,rect.m_bottom) )
		{
			it->second.pWnd->MoveWnd(xReal,yReal);
			it->second.pWnd->Show(true,false);
		}
	}
}
