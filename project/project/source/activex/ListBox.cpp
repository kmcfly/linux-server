#include "ListBox.h"
#include "CtrlID.h"
#include "ScrollDialog.h"

using namespace GUI;

GUI::CListBox::CListBox() : m_itemCount(0)
, m_topRow(0)
, m_bDrawLine(true)
, m_bSingleSelect(false)
, m_curSelectItem(LISTBOX_INVALID_ITEM)
{
	m_ctrlType = GUI::WND_TYPE_LIST_BOX;
}

GUI::CListBox::~CListBox()
{

}

//////////////////////////////////////////////////////////////////////////
BEGIN_MSG_MAP(GUI::CListBox, GUI::CWnd)
	ON_MSG_EX(CTRLID_NULL, KEY_VIRTUAL_ENTER, OnEnter)
	//ON_MSG_EX(CTRLID_NULL, KEY_VIRTUAL_ADD, OnEnter)
	//ON_MSG_EX(CTRLID_NULL, KEY_VIRTUAL_DEC, OnEnter)
END_MSG_MAP()
//////////////////////////////////////////////////////////////////////////
unsigned long GUI::CListBox::OnEnter(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	//处理选中操作
	if (m_bSingleSelect)
	{
		int selItem = (yPos - m_rect.m_top) / m_length[LIST_ITEM_HEIGHT] + m_topRow;
		if ((selItem >= m_topRow) && (selItem < (m_topRow + GetPageSize())) && (selItem < m_itemCount))
		{
			m_curSelectItem = selItem;
			Repaint();
		}
	}
	//处理列表中的控件响应
	if (m_itemMap.empty())
	{
		return KEY_VIRTUAL_MSG_CONTINUE;
	}

	unsigned long msg = KEY_VIRTUAL_MSG_DEFAULT_TAG;

	unsigned short bottomRow = GetBottomRow(m_topRow);
	LISTBOX_ITEM_MAP::iterator iter;
	int colCount = m_colWidthVector.size();
	unsigned long rowBits = 0;
	for (unsigned long row = m_topRow; row <= bottomRow; ++row)
	{
		rowBits = static_cast<unsigned long>(row) << 16;
		for (unsigned long col = 0; col < colCount; ++col)
		{
			iter = m_itemMap.find(rowBits | col);
			if (iter != m_itemMap.end())
			{
				if (ctrlID == iter->second->GetID())
				{
					return (msg + (rowBits | col));
				}
			}
		}
	}

	return keyID;
}

bool GUI::CListBox::NextItem()
{
	int row = m_topRow + 1;
	if (row <= (m_itemCount - GetPageSize()))
	{
		SetTopRow(row);
		return true;
	}
	else
	{
		return false;
	}
}

bool GUI::CListBox::PrevItem()
{
	int row = m_topRow - 1;
	if (row >= 0)
	{
		SetTopRow(row);
		return true;
	}
	else
	{
		return false;
	}
}

void GUI::CListBox::OnDestroy()
{
	LISTBOX_ITEM_MAP::iterator iter;
	for (iter = m_itemMap.begin(); iter != m_itemMap.end(); ++iter)
	{
		if (NULL != iter->second)
		{
			delete iter->second;
			iter->second = NULL;
		}
	}

	m_colWidthVector.clear();
	m_itemMap.clear();
	m_itemMapStr.clear();

	m_itemCount = 0;
	m_topRow = 0;
	m_curSelectItem = LISTBOX_INVALID_ITEM;
}

unsigned long GUI::CListBox::GetKey(unsigned short row, unsigned short col) const
{
	unsigned long _row = static_cast<unsigned long>(row) << 16;
	unsigned long _col = static_cast<unsigned long>(col);
	return (_row | _col);
}

unsigned short GUI::CListBox::GetCol(unsigned long key) const
{
	return static_cast<unsigned short>(key & 0xffff);
}

unsigned short GUI::CListBox::GetRow(unsigned long key) const
{
	return static_cast<unsigned short>( (key >> 16) & 0xffff );
}

bool GUI::CListBox::IsValid(unsigned short row, unsigned short col) const
{
	return (row < m_itemCount) && (col < m_colWidthVector.size());
}

int GUI::CListBox::GetPageSize() const
{
	return m_rect.Height() / m_length[LIST_ITEM_HEIGHT];
}

int GUI::CListBox::GetPageCount() const
{
	int pageSize = GetPageSize();
	assert(pageSize > 0);
	return m_itemCount / pageSize + 1;
}

int GUI::CListBox::AddColumn(int width)
{
	m_colWidthVector.push_back(width);
	return m_colWidthVector.size() - 1;
}

GUI::CWnd*  GUI::CListBox::AddItem(unsigned short row, unsigned short col, WND_TYPE wndType, unsigned short cx /*= -1*/, unsigned short cy /*= -1*/ )
{
	assert(col < m_colWidthVector.size());
	if (row >= m_itemCount)
	{
		m_itemCount = row + 1;
	}

	GUI::CWnd* pWnd = CreateSubWnd( row, col, wndType, cx, cy );
	if (NULL != pWnd)
	{
		unsigned long key = GetKey(row, col);
		m_itemMap[key] = pWnd;

		if (row >= GetPageSize())
		{
			pWnd->Show(false, false);
		}
		return pWnd;
	} 
	else
	{
		return NULL;
	}
}

void GUI::CListBox::AddItem(unsigned short row, unsigned short col, const char* pStr, bool bRefresh /* = false */)
{
	assert(col < m_colWidthVector.size());
	if (row >= m_itemCount)
	{
		m_itemCount = row + 1;
	}
	
	unsigned long key = GetKey(row, col);
	m_itemMapStr[key] = pStr;

	if (bRefresh)
	{
		Repaint();
	}
}

void GUI::CListBox::RemoveItem(unsigned short row, bool bRefresh)
{
	if (m_itemCount < 0)
	{
		return;
	}
	assert(row >= 0);
	assert(row < m_itemCount);
	
	{
		LISTBOX_ITEM_STRING_MAP::iterator iter;
		for (iter = m_itemMapStr.begin(); iter != m_itemMapStr.end(); )
		{
			if (GetRow(iter->first) == row)
			{
				LISTBOX_ITEM_STRING_MAP::iterator iter_temp = iter;
				++iter;
				m_itemMapStr.erase(iter_temp);
			}
			else
			{
				++iter;
			}
			
		}
	}

	{
		LISTBOX_ITEM_MAP::iterator iter;
		for (iter = m_itemMap.begin(); iter != m_itemMap.end(); )
		{
			if (GetRow(iter->first) == row)
			{
				assert(NULL != iter->second);
				iter->second->Show(false, false);
				iter->second->Destroy();
				delete iter->second;

				LISTBOX_ITEM_MAP::iterator iter_temp = iter;
				++iter;
				m_itemMap.erase(iter_temp);
			}
			else
			{
				++iter;
			}
		}
	}

	if (m_itemCount - 1 == row)
	{
		m_itemCount--;
		if (m_curSelectItem >= m_itemCount)
		{
			m_curSelectItem = LISTBOX_INVALID_ITEM;
		}
	}	
	
	if (bRefresh)
	{
		Repaint();
	}
}

void GUI::CListBox::RemoveAllItem(bool bRefresh)
{
	LISTBOX_ITEM_MAP::iterator iter;
	for (iter = m_itemMap.begin(); iter != m_itemMap.end(); iter++)
	{
		assert(NULL != iter->second);
		iter->second->Show(false, false);
		iter->second->Destroy();
		delete iter->second;
		iter->second = NULL;
	}


	m_itemMap.clear();
	m_itemMapStr.clear();

	m_itemCount = 0;
	m_topRow = 0;
	m_curSelectItem = LISTBOX_INVALID_ITEM;

	if (bRefresh)
	{
		Repaint();
	}
}

GUI::CWnd* GUI::CListBox::CreateSubWnd(unsigned short row, unsigned short col, WND_TYPE wndType, unsigned short cx /*= -1*/, unsigned short cy /*= -1*/ )
{
	int x = 0;
	int y = 0;
	int w = 0, h = 0;

	bool useDefault = true;
	if( cx > 0 && cy > 0 ) useDefault = false;

	if( useDefault )
	{
		w = m_colWidthVector[col] - m_length[LIST_ITEM_XPOS];
	}
	else
	{
		w = cx >= (m_colWidthVector[col] - m_length[LIST_ITEM_XPOS]) ? (m_colWidthVector[col] - m_length[LIST_ITEM_XPOS]) : cx;
	}

	if (!CalculateItemPos(row, col, x, y))
	{
		assert(false);
		return NULL;
	}

	if (m_rect.m_top + y > m_rect.m_bottom)
	{
		y = 0;
	}

	switch (wndType)
	{
	case WND_TYPE_STATIC:
		{
			//assert(false);
			if( useDefault )
			{
				h = m_length[LIST_ITEM_HEIGHT] - 2 *m_length[LIST_ITEM_YPOS];
			}
			else
			{
				h = (m_length[LIST_ITEM_HEIGHT] - 2 *m_length[LIST_ITEM_YPOS]) >= cy ? cy : (m_length[LIST_ITEM_HEIGHT] - 2 *m_length[LIST_ITEM_YPOS]);
			}
			GUI::CStatic* pWnd = new GUI::CStatic;
			pWnd->Create(GUI::CWnd::GetRandWndID(), 
				x + m_length[LIST_ITEM_XPOS], 
				y + m_length[LIST_ITEM_YPOS], 
				w/*m_colWidthVector[col] - m_length[LIST_ITEM_XPOS]*/, 
				h/*m_length[LIST_ITEM_HEIGHT]*/, 
				this, NULL);
			pWnd->SetBKColor( COLOR_DLG_CHILD_BK );
			return pWnd;
		}
		break;
	case WND_TYPE_BUTTON:
		{
			if( useDefault )
			{
				h = m_length[BTN_HEIGHT];
			}
			else
			{
				h = m_length[BTN_HEIGHT] >= cy ? cy : m_length[BTN_HEIGHT];
			}
			GUI::CButton* pWnd = new GUI::CButton;
			pWnd->Create(GUI::CWnd::GetRandWndID(), 
				x + m_length[LIST_ITEM_XPOS], 
				y + m_length[LIST_ITEM_YPOS] - 1,
				w/*m_colWidthVector[col] - m_length[LIST_ITEM_XPOS]*/, 
				h/*m_length[BTN_HEIGHT]*/, 
				this, 
				col+1, 
				row+1, 
				NULL);
			return pWnd;
		}
		break;
	case WND_TYPE_RADIO:
		{
			if( useDefault )
			{
				h = m_length[RADIO_HEIGHT];
			}
			else
			{
				h = m_length[RADIO_HEIGHT] >= cy ? cy : m_length[RADIO_HEIGHT];
			}
			GUI::CRadioCtrl* pWnd = new GUI::CRadioCtrl;
			pWnd->Create(GUI::CWnd::GetRandWndID(),
									x + m_length[LIST_ITEM_XPOS], 
									y + m_length[LIST_ITEM_YPOS],
									w-m_length[LIST_ITEM_XPOS]/*m_colWidthVector[col] - m_length[LIST_ITEM_XPOS]*/,
									h/*m_length[EDIT_HEIGHT]*/, 
									this, 
									col+1, 
									row+1,NULL);
			return pWnd;
		}
		break;
	case WND_TYPE_EDIT:
		{
			if( useDefault )
			{
				h = m_length[EDIT_HEIGHT];
			}
			else
			{
				h = m_length[EDIT_HEIGHT] >= cy ? cy : m_length[EDIT_HEIGHT];
			}
			GUI::CEditCtrl* pWnd = new GUI::CEditCtrl;
			pWnd->Create(GUI::CWnd::GetRandWndID(),
				x + m_length[LIST_ITEM_XPOS], 
				y + m_length[LIST_ITEM_YPOS],
				w-m_length[LIST_ITEM_XPOS]/*m_colWidthVector[col] - m_length[LIST_ITEM_XPOS]*/,
				h/*m_length[EDIT_HEIGHT]*/, 
				this, 
				col+1, 
				row+1);
			return pWnd;
		}
		break;
	case WND_TYPE_NUM_EDIT:
		{
			if( useDefault )
			{
				h = m_length[EDIT_HEIGHT];
			}
			else
			{
				h = m_length[EDIT_HEIGHT] >= cy ? cy : m_length[EDIT_HEIGHT];
			}
			GUI::CNumEditCtrl* pWnd = new GUI::CNumEditCtrl;
			pWnd->Create(GUI::CWnd::GetRandWndID(),
				x + m_length[LIST_ITEM_XPOS], 
				y + m_length[LIST_ITEM_YPOS],
				w-m_length[LIST_ITEM_XPOS]/*m_colWidthVector[col] - m_length[LIST_ITEM_XPOS]*/,
				h/*m_length[EDIT_HEIGHT]*/, 
				this, 
				col+1, 
				row+1);
			return pWnd;
		}
		break;
	case WND_TYPE_CHECK:
		{
			if( useDefault )
			{
				h = m_length[CHECK_HEIGHT];
			}
			else
			{
				h = m_length[CHECK_HEIGHT] >= cy ? cy : m_length[CHECK_HEIGHT];
			}
			GUI::CCheckCtrl* pWnd = new GUI::CCheckCtrl;
			pWnd->Create(GUI::CWnd::GetRandWndID(), 
				x + m_length[LIST_ITEM_XPOS], 
				y + m_length[LIST_ITEM_YPOS],
				w/*m_colWidthVector[col] - m_length[LIST_ITEM_XPOS]*/, 
				h/*m_length[CHECK_HEIGHT]*/, 
				this, 
				col+1,
				row+1);
			return pWnd;
		}
		break;
	case WND_TYPE_COMBO:
		{
			if( useDefault )
			{
				h = m_length[COMBO_HEIGHT];
			}
			else
			{
				h = m_length[COMBO_HEIGHT] >= cy ? cy : m_length[COMBO_HEIGHT];
			}
			GUI::CComboCtrl* pWnd = new GUI::CComboCtrl;
			pWnd->Create(GUI::CWnd::GetRandWndID(), 
				x + m_length[LIST_ITEM_XPOS], 
				y + m_length[LIST_ITEM_YPOS], 
				w - m_length[LIST_ITEM_XPOS]/*m_colWidthVector[col] - m_length[LIST_ITEM_XPOS]*/, 
				h/*m_length[COMBO_HEIGHT]*/, 
				this, 
				col+1, 
				row+1);
			return pWnd;
		}
		break;
	case WND_TYPE_DATE:
		{
			if( useDefault )
			{
				h = m_length[DATE_CTRL_HEIGHT];
			}
			else
			{
				h = m_length[DATE_CTRL_HEIGHT] >= cy ? cy : m_length[DATE_CTRL_HEIGHT];
			}
			GUI::CDateCtrl* pWnd = new GUI::CDateCtrl;
			pWnd->Create(GUI::CWnd::GetRandWndID(), 
				x + m_length[LIST_ITEM_XPOS], 
				y + m_length[LIST_ITEM_YPOS], 
				w - 4, 
				h, 
				this, 
				col+1, 
				row+1);
			return pWnd;
		}
		break;
	case WND_TYPE_TIME:
		{
			if( useDefault )
			{
				h = m_length[TIME_CTRL_HEIGHT];
			}
			else
			{
				h = m_length[TIME_CTRL_HEIGHT] >= cy ? cy : m_length[TIME_CTRL_HEIGHT];
			}
			GUI::CTimeCtrl* pWnd = new GUI::CTimeCtrl;
			pWnd->Create(GUI::CWnd::GetRandWndID(), 
				x + m_length[LIST_ITEM_XPOS], 
				y + m_length[LIST_ITEM_YPOS], 
				w - 4, 
				h, 
				this, 
				col+1, 
				row+1);
			return pWnd;
		}
		break;
	case WND_TYPE_IP:
		{
		/*	if( useDefault ){
				h = m_length[IP_CTRL_HEIGHT];
			}else{
				h = m_length[IP_CTRL_HEIGHT] >= cy ? cy : m_length[IP_CTRL_HEIGHT];
			}*/
			GUI::CIPAddressCtrl* pWnd = new GUI::CIPAddressCtrl;
			pWnd->Create(GUI::CWnd::GetRandWndID(), 
				x + m_length[LIST_ITEM_XPOS], 
				y + m_length[LIST_ITEM_YPOS], 
				0, 
				h, 
				this, 
				col+1, 
				row+1);
			return pWnd;
		}
		break;
	case WND_TYPE_PROGRESS:
		{
			if( useDefault ){
				h = m_length[IP_CTRL_HEIGHT];
			}else{
				h = m_length[IP_CTRL_HEIGHT] >= cy ? cy : m_length[IP_CTRL_HEIGHT];
			}
			GUI::CProgressCtrl* pWnd = new GUI::CProgressCtrl;
			pWnd->Create(GUI::CWnd::GetRandWndID(), 
				x + m_length[LIST_ITEM_XPOS], 
				y + m_length[LIST_ITEM_YPOS], 
				w - 4, 
				h, 
				this, 
				col+1, 
				row+1);
			return pWnd;
		}
		break;
	case WND_TYPE_PICTURE:
		{
			GUI::CPictureCtrl* pWnd = new GUI::CPictureCtrl;
			pWnd->Create(GUI::CWnd::GetRandWndID(), 
				x + m_length[LIST_ITEM_XPOS], 
				y + m_length[LIST_ITEM_YPOS], 
				this, 
				BMP_BTN_PLAY_AUDIO,
				true);
			return pWnd;
		}
		break;
#if defined (__CUSTOM_IPV6__)
	case WND_TYPE_IPV6:
		{
			if( useDefault )
			{
				h = m_length[IP_CTRL_HEIGHT];
			}
			else
			{
				h = m_length[IP_CTRL_HEIGHT] >= cy ? cy : m_length[IP_CTRL_HEIGHT];
			}
			GUI::CIPV6Ctrl* pWnd = new GUI::CIPV6Ctrl;
			if(m_width<=800)
			{
				printf("%s width=%d\n",__FILE__,m_colWidthVector[col] - m_length[LIST_ITEM_XPOS]-15);
				pWnd->Create(GUI::CWnd::GetRandWndID(), 
					x + m_length[LIST_ITEM_XPOS], 
					y + m_length[LIST_ITEM_YPOS], 
					m_colWidthVector[col] - m_length[LIST_ITEM_XPOS]-2, 
					h, 
					this, 
					col+1, 
					row+1);

			}
			else
			{
				printf("%s width=%d\n",__FILE__,0);
				pWnd->Create(GUI::CWnd::GetRandWndID(), 
					x + m_length[LIST_ITEM_XPOS], 
					y + m_length[LIST_ITEM_YPOS], 
					0, 
					h, 
					this, 
					col+1, 
					row+1);

			}
			
			return pWnd;
		}
		break;
	case WND_TYPE_HEX_NUM_EDIT:
		{
			if( useDefault )
			{
				h = m_length[EDIT_HEIGHT];
			}
			else
			{
				h = m_length[EDIT_HEIGHT] >= cy ? cy : m_length[EDIT_HEIGHT];
			}
			GUI::CHexNumEditCtrl* pWnd = new GUI::CHexNumEditCtrl;
			pWnd->Create(GUI::CWnd::GetRandWndID(),
				x + m_length[LIST_ITEM_XPOS], 
				y + m_length[LIST_ITEM_YPOS],
				w-m_length[LIST_ITEM_XPOS],
				h, 
				this, 
				col+1, 
				row+1);
			return pWnd;
		}
		break;
#endif
#if defined(__CUSTOM_BR27__) || defined(__AUTOREG__)
	case WND_TYPE_EDITSTATIC:
		{
			if( useDefault )
			{
				h = m_length[EDIT_HEIGHT];
			}
			else
			{
				h = m_length[EDIT_HEIGHT] >= cy ? cy : m_length[EDIT_HEIGHT];
			}
			GUI::CEditStaticCtrl* pWnd = new GUI::CEditStaticCtrl;
			pWnd->Create(GUI::CWnd::GetRandWndID(),
				x + m_length[LIST_ITEM_XPOS], 
				y + m_length[LIST_ITEM_YPOS],
				w-m_length[LIST_ITEM_XPOS],
				h,
				this, 
				col+1, 
				row+1);
			return pWnd;
		}
		break;
#endif
	default:
		assert(false);
		break;
	}

	return NULL;
}

bool GUI::CListBox::CalculateItemPos(unsigned short row, unsigned short col, int& x, int& y) const
{
	if (!IsValid(row, col))
	{
		return false;
	}

	y = row * m_length[LIST_ITEM_HEIGHT];
	x = 0;
	for (unsigned short _col = 0; _col < col; ++_col)
	{
		x += m_colWidthVector[_col];
	}
	
	return true;
}

bool GUI::CListBox::CreateListFocusMap()
{
	return CreateFocusMap();
}

void GUI::CListBox::SetTopRow(unsigned short topRow)
{
	assert(topRow < m_itemCount);
	unsigned short bottomRow = GetBottomRow(m_topRow);
	//隐藏当前
	ShowRow(m_topRow, bottomRow, false);
	//移动并显示指定项
	bottomRow = GetBottomRow(topRow);
	OffsetRow(topRow, bottomRow, 0);
	ShowRow(topRow, bottomRow, true);

	m_topRow = topRow;
	Repaint();
}

unsigned short GUI::CListBox::GetBottomRow(unsigned short topRow) const
{
	assert(topRow < m_itemCount);

	int pageSize = GetPageSize();
	unsigned short rowBottom = topRow + pageSize - 1;
	if (m_itemCount - 1 < rowBottom)//如果超出
	{
		rowBottom = m_itemCount - 1;
	}
	return rowBottom;
}

bool GUI::CListBox::GetPageRow(int page, unsigned short & beginRow, unsigned short & endRow) const
{
	beginRow = 0;
	endRow = 0;
	if (m_itemMap.empty())
	{
		return false;
	}
	
	
	int pageSize = GetPageSize();
	int pageCount = GetPageCount();
	if (page >= pageCount)
	{
		assert(false);
		return false;
	}

	beginRow = page * pageSize;
	
	if (pageCount - 1 == page)//尾页
	{
		endRow = GetItemCount() - 1;
	} 
	else
	{
		endRow = beginRow + pageSize - 1;
	}

	return true;
}

void GUI::CListBox::ShowRow(unsigned short beginRow, unsigned short endRow, bool bShow)
{
	assert(endRow >= beginRow);

	LISTBOX_ITEM_MAP::iterator iter;
	int colCount = m_colWidthVector.size();
	unsigned long rowBits = 0;
	for (unsigned long row = beginRow; row <= endRow; ++row)
	{
		rowBits = static_cast<unsigned long>(row) << 16;
		for (unsigned long col = 0; col < colCount; ++col)
		{
			iter = m_itemMap.find(rowBits | col);
			if (iter != m_itemMap.end())
			{
				iter->second->Show(bShow, false);
			}
		}
	}
}

void GUI::CListBox::OffsetRow(unsigned short beginRow, unsigned short endRow, int rowOffset)
{
	assert(endRow >= beginRow);
	GUI::CRect rect;

	LISTBOX_ITEM_MAP::iterator iter;
	int colCount = m_colWidthVector.size();
	unsigned long rowBits = 0;
	for (unsigned long row = beginRow; row <= endRow; ++row)
	{
		rowBits = static_cast<unsigned long>(row) << 16;
		for (unsigned long col = 0; col < colCount; ++col)
		{
			iter = m_itemMap.find(rowBits | col);
			if (iter != m_itemMap.end())
			{
				rect = iter->second->GetWindowRect();
				int x = rect.m_left;
				int y = m_rect.m_top + static_cast<int>(rowOffset + row - beginRow)*m_length[LIST_ITEM_HEIGHT] + m_length[LIST_ITEM_YPOS];
				
				iter->second->MoveWnd(x, y);

				/*if (col == 3)
				{
					TRACE("X:%d, Y:%d \n", iter->second->GetWindowRect().m_left, iter->second->GetWindowRect().m_top);
				}*/
			}
		}
	}
}

void GUI::CListBox::OnPaint(PAINT_STATUS status)
{
	//清除背景
	GUI::CWnd::DrawRect(m_rect.m_left, m_rect.m_top, m_rect.Width(), m_rect.Height(), COLOR_DLG_CHILD_BK);
	unsigned long color = COLOR_LINE_GRAY;
	//画颜色背景
	if (!m_itemColorMap.empty())
	{
		unsigned short endRow = GetBottomRow(m_topRow);
		unsigned short beginRow = m_topRow;

		LISTBOX_COLOR_MAP::const_iterator iter;

		int colCount = m_colWidthVector.size();
		unsigned long rowBits = 0;

		int x = m_rect.m_left;
		int y = m_rect.m_top;
		for (unsigned long row = beginRow; row <= endRow; ++row)
		{
			rowBits = static_cast<unsigned long>(row) << 16;
			x = m_rect.m_left;

			for (unsigned long col = 0; col < colCount; ++col)
			{
				iter = m_itemColorMap.find(rowBits | col);
				if (iter != m_itemColorMap.end())
				{
					GUI::CWnd::DrawRect(x, y,  m_colWidthVector[col], m_length[LIST_ITEM_HEIGHT], iter->second);	
				}
				x += m_colWidthVector[col];
			}

			y += m_length[LIST_ITEM_HEIGHT];
		}
	}
	//话选中行
	if (m_bSingleSelect)
	{
		int pageSize = GetPageSize();
		if ((m_curSelectItem >= m_topRow) && (m_curSelectItem < m_topRow + pageSize))
		{
			int y = m_rect.m_top + (m_curSelectItem - m_topRow) * m_length[LIST_ITEM_HEIGHT];
			GUI::CWnd::DrawRect(m_rect.m_left, y, m_rect.Width(), m_length[LIST_ITEM_HEIGHT], COLOR_SELECT);
		}
	}
	
	if (m_bDrawLine)
	{
		//画列
		int x = m_rect.m_left;

		if (!m_colWidthVector.empty())
		{
			LISTBOX_COL_INFO::const_iterator iter;
			for (iter = m_colWidthVector.begin(); iter != m_colWidthVector.end() - 1; ++iter)
			{
				x += (*iter);
				GUI::CWnd::DrawLine(x, m_rect.m_top, x, m_rect.m_bottom, color);
			}
		}

		//画行
		int y = m_rect.m_top + m_length[LIST_ITEM_HEIGHT];
		while (y < m_rect.m_bottom)
		{
			GUI::CWnd::DrawLine(m_rect.m_left, y, m_rect.m_right, y, color);
			y += m_length[LIST_ITEM_HEIGHT];
		}
	}	

	//画指定行
	if (!m_itemLineMap.empty())
	{
		int y = m_rect.m_top + m_length[LIST_ITEM_HEIGHT];
		int row = m_topRow;
		LISTBOX_COLOR_MAP::const_iterator iter;
		while (y < m_rect.m_bottom)
		{
			iter = m_itemLineMap.find(row ++ );
			if (iter != m_itemLineMap.end())
			{
				GUI::CWnd::DrawLine(m_rect.m_left, y, m_rect.m_right, y, color, iter->second);
			}
			y += m_length[LIST_ITEM_HEIGHT];
		}
	}

//	GUI::CWnd::DrawLine(m_rect.m_left, m_rect.m_bottom, m_rect.m_right, m_rect.m_bottom, color, 2);//最下面的横条

	//显示静态字符串
	{
		if (!m_itemMapStr.empty()) 
		{
			unsigned short endRow = GetBottomRow(m_topRow);
			unsigned short beginRow = m_topRow;

			LISTBOX_ITEM_STRING_MAP::const_iterator iter;

			int colCount = m_colWidthVector.size();
			unsigned long rowBits = 0;
			
			int x = m_rect.m_left + m_length[LIST_ITEM_XPOS];
			int y = m_rect.m_top + m_length[LIST_ITEM_YPOS];
			for (unsigned long row = beginRow; row <= endRow; ++row)
			{
				rowBits = static_cast<unsigned long>(row) << 16;
				x = m_rect.m_left + m_length[LIST_ITEM_XPOS];

				for (unsigned long col = 0; col < colCount; ++col)
				{
					iter = m_itemMapStr.find(rowBits | col);
					if (iter != m_itemMapStr.end())
					{
						GUI::CWnd::DrawText(iter->second.c_str(), iter->second.length(), x, y, m_colWidthVector[col]- 2 * m_length[LIST_ITEM_XPOS], m_length[LIST_ITEM_HEIGHT]-m_length[LIST_ITEM_YPOS]);	
					}
					x += m_colWidthVector[col];
				}
				
				y += m_length[LIST_ITEM_HEIGHT];
			}
		}
	}
	//画边框
	GUI::CWnd::DrawRectBorder(m_rect.m_left, m_rect.m_top, m_rect.Width(), m_rect.Height(), color, 2);
}

GUI::CWnd* GUI::CListBox::GetItemWnd(unsigned short row, unsigned short col)
{
	unsigned long key = GetKey(row, col);
	LISTBOX_ITEM_MAP::iterator iter = m_itemMap.find(key);
	if (iter != m_itemMap.end())
	{
		return iter->second;
	} 
	else
	{
		return NULL;
	}
}


const GUI::CWnd* GUI::CListBox::GetItemWnd(unsigned short row, unsigned short col) const
{
	unsigned long key = GetKey(row, col);
	LISTBOX_ITEM_MAP::const_iterator iter = m_itemMap.find(key);
	if (iter != m_itemMap.end())
	{
		return iter->second;
	} 
	else
	{
		return NULL;
	}
}

std::string GUI::CListBox::GetItemText(unsigned short row, unsigned short col)
{
	unsigned long key = GetKey(row, col);
	LISTBOX_ITEM_STRING_MAP::iterator iter = m_itemMapStr.find(key);
	if (iter != m_itemMapStr.end())
	{
		return iter->second;
	} 
	else
	{
		return NULL;
	}
}

void GUI::CListBox::SetItemColor(unsigned short row, unsigned short col, unsigned long color)
{
	assert(col < m_colWidthVector.size());

	unsigned long key = GetKey(row, col);
	m_itemColorMap[key] = color;
}

void GUI::CListBox::SetRowColor(unsigned short row, unsigned long color)
{
	int col_count = m_colWidthVector.size();
	for (int i = 0; i < col_count; ++i)
	{
		SetItemColor(row, i, color);
	}
}

void GUI::CListBox::DrawLine(unsigned short row, unsigned long width)
{
	int col_count = m_colWidthVector.size();
	m_itemLineMap[row] = width;
}

void GUI::CListBox::SetCurSingleSelect(int item, bool bRefresh)
{
	assert(item >= 0);
	if(item < m_itemCount)
	{
		m_curSelectItem = item;
		if (bRefresh)
		{
			Repaint();
		}
	}	
}
