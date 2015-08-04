#include "ListCtrl.h"
#include "CtrlID.h"

using namespace GUI;

GUI::CListCtrl::CListCtrl() : m_hasHeader(true)
{
	m_ctrlType = GUI::WND_TYPE_LIST_CTRL;
}

GUI::CListCtrl::~CListCtrl()
{

}

bool GUI::CListCtrl::Create(unsigned long ctrlID, 
							unsigned long x, 
							unsigned long y, 
							unsigned long cx, 
							unsigned long cy, 
							CWnd* pParent, 
							int xFocus, 
							int yFocus,
							bool hasHeader/* = true*/)
{
	m_hasHeader = hasHeader;

	unsigned short height = ((cy - m_length[LIST_HEAD_HEIGHT])/m_length[LIST_ITEM_HEIGHT])*m_length[LIST_ITEM_HEIGHT] + m_length[LIST_HEAD_HEIGHT] + 2;
	
	if (!m_hasHeader)
	{
		height = (cy/m_length[LIST_ITEM_HEIGHT])*m_length[LIST_ITEM_HEIGHT] + 2;
	}

	return GUI::CWnd::Create(ctrlID, x, y, cx, height, pParent, WND_CHILD | WND_VISIBLE, xFocus, yFocus);
}

void GUI::CListCtrl::OnInitial()
{
	GUI::CWnd::OnInitial();

	if (m_hasHeader)
	{
		m_header.Create(CTRLID_LISTCTRL_HEDER, 0, 0, m_rect.Width()/* - m_length[SCROLL_WIDTH]*/, m_length[LIST_HEAD_HEIGHT], this, WND_VISIBLE | WND_CHILD | WND_DISABLE_FOCUS, 0, 0);
		m_listBox.Create(CTRLID_LISTCTRL_BOX, 0, m_length[LIST_HEAD_HEIGHT], m_rect.Width() /*- CScrollCtrl::m_length[SCROLL_WIDTH]*/, m_rect.Height() - m_length[LIST_HEAD_HEIGHT], this, WND_VISIBLE|WND_CHILD, 0, 1);
	
		m_scroll_v.Create(CTRLID_LIST_CTRL_SCROLL_V, 
			m_rect.Width() - CScrollCtrl::m_length[SCROLL_WIDTH] - 2, 
			m_length[LIST_HEAD_HEIGHT] + 2, 
			m_rect.Height() - m_length[LIST_HEAD_HEIGHT] - 4, 
			this, 
			LIST_SCROLL_XFOCUS, 
			LIST_SCROLL_YFOCUS, 
			false);
	}
	else
	{
		//m_header.Create(CTRLID_LISTCTRL_HEDER, 0, 0, m_rect.Width(), m_length[LIST_HEAD_HEIGHT], this, WND_VISIBLE | WND_CHILD, 0, 0);
		m_listBox.Create(CTRLID_LISTCTRL_BOX, 0, 0, m_rect.Width()/* - CScrollCtrl::m_length[SCROLL_WIDTH]*/, m_rect.Height(), this, WND_VISIBLE|WND_CHILD, 0, 1);

		m_scroll_v.Create(CTRLID_LIST_CTRL_SCROLL_V, m_rect.Width() - CScrollCtrl::m_length[SCROLL_WIDTH] - 2, 2, m_rect.Height() - 4, this, LIST_SCROLL_XFOCUS, LIST_SCROLL_YFOCUS, false);
	}

	m_scroll_v.Show(false, false);

#if defined(__TD32SPEC__) || defined(__TD2316SES_SPEC__)
	//
	if((m_width > 800) && (m_rect.Width() * m_rect.Height() * 4) > (m_width * m_height))
	{
		//如果大分辨率  且 面积超过屏幕 1/4
		//则滑动条的中间部分会忽略部分消息
		m_scroll_v.SetIgnoreStep(true);
	}
#endif

}

void GUI::CListCtrl::OnPaint(PAINT_STATUS status)
{
	GUI::CWnd::OnPaint(status);

	if(m_scroll_v.IsShow())
	{
		unsigned short x = m_rect.m_right - CScrollCtrl::m_length[SCROLL_WIDTH] - 2;
		unsigned short y = m_rect.m_top;
		unsigned short cx = m_length[LIST_HEAD_HEIGHT] + 2;
		unsigned short cy = m_rect.Height();
		if(m_hasHeader)
		{
			y = m_rect.m_top + m_length[LIST_HEAD_HEIGHT];
			cy = m_rect.Height() - m_length[LIST_HEAD_HEIGHT];
		}
		
		GUI::CWnd::DrawRectBorder(x, y, cx, cy, COLOR_LINE_GRAY, 2, 2);
	}
}

int GUI::CListCtrl::AddColumn(const char* pStr, int width)
{
	int ret = m_listBox.AddColumn(width);

	if (m_hasHeader)
	{
		assert(NULL != pStr);
		return m_header.AddItem(pStr, width);
	}
	else
	{
		return ret;
	}
}

GUI::CWnd*  GUI::CListCtrl::AddItem(unsigned short row, unsigned short col, WND_TYPE wndType, unsigned short cx /*= -1*/, unsigned short cy /*= -1*/ )
{
	GUI::CWnd* pWnd = m_listBox.AddItem(row, col, wndType, cx, cy );
	SetScroll();	
	
	return pWnd;
}

void GUI::CListCtrl::AddItem(unsigned short row, unsigned short col, const char* pStr, bool bRefresh /* = false */)
{
	m_listBox.AddItem(row, col, pStr, bRefresh);
	SetScroll();
}

void GUI::CListCtrl::SetScroll()
{
	int count = m_listBox.GetItemCount() - m_listBox.GetPageSize();
	if (count > 0)
	{
		int box_width = m_rect.Width() - CScrollCtrl::m_length[SCROLL_WIDTH] - 2;
		int box_height = (m_hasHeader ? (m_rect.Height() - m_length[LIST_HEAD_HEIGHT]) : m_rect.Height());

		m_listBox.SetWndSize(false, false, box_width, box_height);
		m_scroll_v.SetRange(0, count);
		m_scroll_v.Show(true, false);
	}
	else
	{
		int box_width = m_rect.Width();
		int box_height = (m_hasHeader ? (m_rect.Height() - m_length[LIST_HEAD_HEIGHT]) : m_rect.Height());

		m_listBox.SetWndSize(false, false, box_width, box_height);

		m_scroll_v.Show(false, false);
	}
}

bool GUI::CListCtrl::CreateListFocusMap()
{
	return m_listBox.CreateListFocusMap();
}

BEGIN_MSG_MAP(GUI::CListCtrl, GUI::CWnd)
	//ON_MSG(CTRLID_LIST_CTRL_SCROLL_V, KEY_VIRTUAL_ENTER, OnClickScroll)
	//ON_MSG(CTRLID_LIST_CTRL_SCROLL_V, KEY_VIRTUAL_DRAG,	OnClickScroll)
	//ON_MSG(CTRLID_LIST_CTRL_SCROLL_V, KEY_VIRTUAL_UP,	OnClickScroll)
	//ON_MSG(CTRLID_LIST_CTRL_SCROLL_V, KEY_VIRTUAL_DOWN,	OnClickScroll)
	ON_MSG_EX(CTRLID_NULL, KEY_VIRTUAL_DEC,	OnAdd)
	ON_MSG_EX(CTRLID_NULL, KEY_VIRTUAL_ADD,	OnDec)
	ON_MSG_EX(CTRLID_LIST_CTRL_SCROLL_V, KEY_VIRTUAL_MSG_DEFAULT_TAG, OnScrollchange)
	ON_MSG_EX(CTRLID_LIST_CTRL_SCROLL_V, KEY_VIRTUAL_ENTER, OnScrollchange)
	ON_MSG_EX(CTRLID_LIST_CTRL_SCROLL_V, KEY_VIRTUAL_DRAG, OnScrollchange)
	ON_MSG_EX(CTRLID_LIST_CTRL_SCROLL_V, KEY_VIRTUAL_BLUP, OnScrollchange)
END_MSG_MAP()

unsigned long GUI::CListCtrl::OnScrollchange(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	//int pos = keyID & (~KEY_VIRTUAL_MSG_DEFAULT_TAG);
	int pos = m_scroll_v.GetPos();
	if (pos != m_listBox.GetTopRow())
	{
		int itemCount = m_listBox.GetItemCount();
		int pageSize = m_listBox.GetPageSize();
		if ((itemCount > pageSize) && ((pos+pageSize) <= itemCount))
		{

			if (pos >= itemCount)
			{
				pos = 0;
			}

			m_listBox.SetTopRow(pos);
		}
	}

	return KEY_VIRTUAL_MSG_STOP; 
}

//unsigned long GUI::CListCtrl::OnClickScroll()
//{
//	int pos = m_scroll_v.GetPos();
//	if (pos != m_listBox.GetTopRow())
//	{
//		int itemCount = m_listBox.GetItemCount();
//		int pageSize = m_listBox.GetPageSize();
//		if ((itemCount > pageSize) && ((pos+pageSize) <= itemCount))
//		{
//			
//			if (pos >= itemCount)
//			{
//				pos = 0;
//			}
//
//			m_listBox.SetTopRow(pos);
//		}
//	}
//
//	return KEY_VIRTUAL_MSG_STOP;
//}

GUI::CWnd* GUI::CListCtrl::GetItemWnd(unsigned short row, unsigned short col)
{
	return m_listBox.GetItemWnd(row, col);
}

const GUI::CWnd* GUI::CListCtrl::GetItemWnd(unsigned short row, unsigned short col) const
{
	return m_listBox.GetItemWnd(row, col);
}

std::string GUI::CListCtrl::GetItemText(unsigned short row, unsigned short col)
{
	return m_listBox.GetItemText(row, col);
}

unsigned long GUI::CListCtrl::OnAdd(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	if(m_listBox.NextItem())
	{
		m_scroll_v.NextPos();
	}
	return KEY_VIRTUAL_MSG_CONTINUE;
}

unsigned long GUI::CListCtrl::OnDec(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	if(m_listBox.PrevItem())
	{
		m_scroll_v.PrevPos();
	}
	return KEY_VIRTUAL_MSG_CONTINUE;
}

void GUI::CListCtrl::SetCurSingleSelect(int item, bool bRefresh)
{
	if (item > (m_listBox.GetTopRow() + m_listBox.GetPageSize() - 1))
	{
		int top_item = item - m_listBox.GetPageSize() + 1;
		if (top_item < 0)
		{
			top_item = 0;
		}
		m_listBox.SetTopRow(top_item);
		m_scroll_v.SetCurPos(top_item, bRefresh);
	}
	m_listBox.SetCurSingleSelect(item, bRefresh);
}
