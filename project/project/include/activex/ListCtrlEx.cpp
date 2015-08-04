#include "ListCtrlEx.h"
#include "CtrlID.h"
#include "Dialog.h"

using namespace GUI;

//////////////////////////////////////////////////////////////////////////

template <class T>
GUI::CListItemEx<T>::CListItemEx()
{

}

template <class T>
GUI::CListItemEx<T>::~CListItemEx()
{

}

template <class T>
bool GUI::CListItemEx<T>::Create(unsigned long ctrlID, unsigned long x, unsigned long y, unsigned long cx, unsigned long cy, CWnd* pParent, int xFocus, int yFocus)
{
	return GUI::CWnd::Create(ctrlID, x, y, cx, cy, pParent, WND_CHILD | WND_VISIBLE | WND_NO_IMAGE_BK, xFocus, yFocus);
}

template <class T>
void GUI::CListItemEx<T>::OnPaint(PAINT_STATUS status)
{
	//需要刷新本控件时候,通知父窗口去刷新
	GUI::CListCtrlEx<T>* pListCtrlEx = dynamic_cast<GUI::CListCtrlEx<T>*>(GetParent());
	assert(pListCtrlEx);
	pListCtrlEx->OnDrawItem(static_cast<int>(GetData()), status);

	//通知父窗口处理获取焦点事件
	if(PAINT_STATUS_FOCUS == status)
	{
		pListCtrlEx->OnFocusItem(static_cast<int>(GetData()));
	}
}

//////////////////////////////////////////////////////////////////////////

template <class T>
GUI::CListCtrlEx<T>::CListCtrlEx() : m_bSelect(false)
, m_currLine(-1)
, m_pageNum(0)
, m_curPage(0)
, m_lineNum(0)
, m_pCKSelect(NULL)
, m_pListItemVir(NULL)
, m_pData(NULL)
, m_itemNum(0)
, m_singleSelItem(-1)
, m_bEnableSingleSel(false)
, m_bItemFocusNotify(false)
{
	m_ctrlType = GUI::WND_TYPE_LIST_CTRL_EX;
}

template <class T>
GUI::CListCtrlEx<T>::~CListCtrlEx()
{

}

template <class T>
bool GUI::CListCtrlEx<T>::Create(unsigned long ctrlID, 
							unsigned long x, 
							unsigned long y, 
							unsigned long cx, 
							unsigned long cy, 
							CWnd* pParent, 
							int xFocus, 
							int yFocus,
							bool bSelect/* = false*/)
{
	m_bSelect = bSelect;

	unsigned short height = ((cy - m_length[LIST_HEAD_HEIGHT] - LIST_CTRL_BOTTOM_HEIGHT)/m_length[LIST_EX_ITEM_HEIGHT])*m_length[LIST_EX_ITEM_HEIGHT] + m_length[LIST_HEAD_HEIGHT] + LIST_CTRL_BOTTOM_HEIGHT;

	return GUI::CWnd::Create(ctrlID, x, y, cx, height, pParent, WND_CHILD | WND_VISIBLE, xFocus, yFocus);
}

template <class T>
void GUI::CListCtrlEx<T>::PutData(T * pData, unsigned long num, bool bRefresh/* = true*/)
{
	//可以为空，为空则刷新
	//assert (NULL != pData);
	//assert (num > 0);

	if (NULL != m_pData)
	{
		delete [] m_pData;
		m_pData = NULL;
	}

	m_pData = pData;
	m_itemNum = num;

	m_pageNum = (m_itemNum + m_lineNum - 1) / m_lineNum;
	m_curPage = (m_itemNum > 0) ? 1 : 0;
	m_currItem = 0;
	m_singleSelItem = -1;

	//处理CHECK框的使能和不使能
	if (m_bSelect)
	{
		unsigned long itemNum = (m_itemNum < m_lineNum) ? m_itemNum : m_lineNum;
		
		//2010-01-30 11:11:00 YSW
		//先全部清除掉选择状态，因为存在着可能处于选择状态，此时再次搜索数据后
		//再次来此刷新，那么默认状态应该为不选择状态
		for (unsigned long i=0; i<m_lineNum; ++i)
		{
			m_pCKSelect[i].SetEnable(true, false);

			m_pCKSelect[i].SetCheck(false, false);
		}
		
		////2010-01-30 11:11:00 YSW
		//没有数据项的行，应该设为不使能
		for (unsigned long i=itemNum; i<m_lineNum; ++i)
		{
			m_pCKSelect[i].SetEnable(false, false);

			m_pCKSelect[i].SetCheck(false, false);
		}

		//2010-02-08 10:38:00 YSW
		//每次都应该把全部选择置为不选择状态，然后根据是否有数据来置使能和不使能。
		m_ckAll.SetEnable(true, false);
		m_ckAll.SetCheck(false, false);

		if (0 == itemNum)//有至少一个item，则设为使能，否则为不使能。
		{
			m_ckAll.SetEnable(false, false);
		}

		//2010-03-02 15:46:00 L
		//每次都应该把选择的内容清空
		m_selectList.clear();
	}

	//处理Item项控件是否使能
	{
		unsigned long itemNum = (m_itemNum < m_lineNum) ? m_itemNum : m_lineNum;
		for (unsigned long i=0; i<m_lineNum; ++i)
		{
			m_pListItemVir[i].SetEnable(true, false);
		}

		for (unsigned long i=itemNum; i<m_lineNum; ++i)
		{
			m_pListItemVir[i].SetEnable(false, false);
		}
	}

	if (bRefresh)
	{
		Repaint();
	}
}

template <class T>
void GUI::CListCtrlEx<T>::AddItem(const T& item, bool bRefresh)
{
	unsigned long itemNum = m_itemNum + 1;
	assert(itemNum > 0);

	T* pData = new T[itemNum];
	if (NULL != m_pData)
	{
		for (int i = 0; i < m_itemNum; ++i)
		{
			pData[i] = m_pData[i];
		}
	}
	
	pData[itemNum - 1] = item;
	
	PutData(pData, itemNum, bRefresh);
}

template <class T>
void GUI::CListCtrlEx<T>::DelItem(long item, bool bRefresh)
{
	if ((item < 0) || (item >= m_itemNum))
	{
		assert(false);
		return ;
	}

	m_singleSelItem = -1;
	
	unsigned long itemNum = m_itemNum - 1;
	if (itemNum > 0)
	{
		T* pData = new T[itemNum];
		if (NULL != m_pData)
		{
			for (int srcIndex = 0, destIndex = 0; srcIndex < m_itemNum; ++srcIndex)
			{
				if (item != srcIndex)
				{
					assert(destIndex < itemNum);
					pData[destIndex] = m_pData[srcIndex];
					++destIndex;
				}		
			}
		}
		
		PutData(pData, itemNum, bRefresh);
	}
	else
	{
		PutData(NULL , 0, bRefresh);
	}	
}

template <class T>
T* GUI::CListCtrlEx<T>::GetSelectData(unsigned long & count) const
{
	count = m_selectList.size();
	if ((0 < m_itemNum) && (0 < count) && (count <= m_itemNum))
	{
		T* pSelectData = new T[count];
		int index = 0;
		for (SELECT_LIST::const_iterator iter = m_selectList.begin(); iter != m_selectList.end(); ++iter)
		{
			assert(*iter < m_itemNum);
			pSelectData[index++] = m_pData[*iter];
		}

		return pSelectData;
	} 
	else
	{
		return NULL;
	}
}

template <class T>
void GUI::CListCtrlEx<T>::OnInitial()
{
	GUI::CWnd::OnInitial();

	m_header.Create(CTRLID_LISTCTRL_HEDER, 0, 0, m_rect.Width(), m_length[LIST_HEAD_HEIGHT], this, WND_VISIBLE | WND_CHILD | WND_DISABLE_FOCUS, 0, 0);
	
	unsigned short x = m_rect.Width() - ((LIST_CTRL_BUTTON_WIDTH + 4) * 4) - LIST_CTRL_CHECK_WIDTH;
	unsigned short y = m_rect.Height() - LIST_CTRL_BOTTOM_HEIGHT + 4;

	m_lineNum = (y - m_length[LIST_HEAD_HEIGHT])/ m_length[LIST_EX_ITEM_HEIGHT];

	if (m_bSelect && (m_lineNum > 0))
	{
		m_ckAll.Create(CTRLID_LIST_EX_CK_ALL, x - 16, y, LIST_CTRL_CHECK_WIDTH, m_length[CHECK_HEIGHT], this, 0, m_lineNum+2);
		m_ckAll.SetCaption(m_pStrTable->String(E_STR_ID_ALL), true, false);
		m_ckAll.SetCheck(false, false);
		m_ckAll.SetEnable(false, false);//默认是不使能状态

		m_pCKSelect = new GUI::CCheckCtrl [m_lineNum];
		assert (NULL != m_pCKSelect);

		unsigned short yck = m_length[LIST_HEAD_HEIGHT] + 2;
		for (int i=0; i<m_lineNum; ++i)
		{
			m_pCKSelect[i].Create(CTRLID_LIST_EX_CK_SELECT_0+i, 2, yck, m_length[CHECK_WIDTH], m_length[CHECK_HEIGHT], this, 0, i+1);
			yck += m_length[LIST_EX_ITEM_HEIGHT];
			m_pCKSelect[i].SetEnable(false, false);//默认是不使能状态
		}
	}
	
	m_pListItemVir = new GUI::CListItemEx<T>[m_lineNum];
	unsigned short ywnd = m_length[LIST_HEAD_HEIGHT];
	unsigned short xwnd = (m_bSelect) ? (m_length[CHECK_WIDTH] + 4) : 0;
	unsigned short cxwnd = m_rect.Width();
	cxwnd -= (m_bSelect) ? (m_length[CHECK_WIDTH] + 4) : 0;
	unsigned short idwnd = (m_bSelect) ? 1 : 0;
	for (int i = 0; i< m_lineNum; ++i)
	{
		m_pListItemVir[i].Create(GetRandWndID(), xwnd, ywnd, cxwnd, m_length[LIST_EX_ITEM_HEIGHT], this, idwnd, i+1);
		ywnd += m_length[LIST_EX_ITEM_HEIGHT];
		m_pListItemVir[i].SetEnable(false, false);
		m_pListItemVir[i].SetData(i);
	}

	x += LIST_CTRL_CHECK_WIDTH;
	m_btnFirst.Create(CTRLID_LIST_EX_BTN_FIRST, x, y, LIST_CTRL_BUTTON_WIDTH, LIST_CTRL_BUTTON_HEIGHT, this, 1, m_lineNum+2);x+=(LIST_CTRL_BUTTON_WIDTH + 4);
	m_btnPrev.Create(CTRLID_LIST_EX_BTN_PREV, x, y, LIST_CTRL_BUTTON_WIDTH, LIST_CTRL_BUTTON_HEIGHT, this, 2, m_lineNum+2);x+=(LIST_CTRL_BUTTON_WIDTH + 4);
	m_btnNext.Create(CTRLID_LIST_EX_BTN_NEXT, x, y, LIST_CTRL_BUTTON_WIDTH, LIST_CTRL_BUTTON_HEIGHT, this, 3, m_lineNum+2);x+=(LIST_CTRL_BUTTON_WIDTH + 4);
	m_btnEnd.Create(CTRLID_LIST_EX_BTN_END, x, y, LIST_CTRL_BUTTON_WIDTH, LIST_CTRL_BUTTON_HEIGHT, this, 4, m_lineNum+2);

	////
	m_btnFirst.SetImage(PAINT_STATUS_NORMAL,BMP_LISTEX_BTN_LEFTMOST,false);
	m_btnFirst.SetImage(PAINT_STATUS_FOCUS,BMP_LISTEX_BTN_LEFTMOST_FOCUS,false);
	m_btnFirst.SetImage(PAINT_STATUS_DISABLE,BMP_LISTEX_BTN_LEFTMOST_DISABLE,false);

	m_btnPrev.SetImage(PAINT_STATUS_NORMAL,BMP_LISTEX_BTN_LEFT,false);
	m_btnPrev.SetImage(PAINT_STATUS_FOCUS,BMP_LISTEX_BTN_LEFT_FOCUS,false);
	m_btnPrev.SetImage(PAINT_STATUS_DISABLE,BMP_LISTEX_BTN_LEFT_DISABLE,false);

	m_btnNext.SetImage(PAINT_STATUS_NORMAL,BMP_LISTEX_BTN_RIGHT,false);
	m_btnNext.SetImage(PAINT_STATUS_FOCUS,BMP_LISTEX_BTN_RIGHT_FOCUS,false);
	m_btnNext.SetImage(PAINT_STATUS_DISABLE,BMP_LISTEX_BTN_RIGHT_DISABLE,false);

	m_btnEnd.SetImage(PAINT_STATUS_NORMAL,BMP_LISTEX_BTN_RIGHTMOST,false);
	m_btnEnd.SetImage(PAINT_STATUS_FOCUS,BMP_LISTEX_BTN_RIGHTMOST_FOCUS,false);
	m_btnEnd.SetImage(PAINT_STATUS_DISABLE,BMP_LISTEX_BTN_RIGHTMOST_DISABLE,false);

	if (m_bSelect)
	{
		m_header.AddItem("", m_length[CHECK_WIDTH]+4);
	}
}

template <class T>
void GUI::CListCtrlEx<T>::OnDestroy()
{
	GUI::CWnd::OnDestroy();

	m_colWidthVector.clear();
	m_selectList.clear();

	delete [] m_pCKSelect;
	m_pCKSelect = NULL;

	delete [] m_pListItemVir;
	m_pListItemVir = NULL;

	delete [] m_pData;
	m_pData = NULL;
	m_itemNum = 0;

	m_currLine = -1;
	m_pageNum = 0;
	m_curPage = 0;
	m_lineNum = 0;

	m_singleSelItem = -1;
}

template <class T>
void GUI::CListCtrlEx<T>::OnPaint(PAINT_STATUS status)
{
	//清除背景
	GUI::CWnd::DrawRect(m_rect.m_left, m_rect.m_top, m_rect.Width(), m_rect.Height(), COLOR_DLG_CHILD_BK);
/*
	//画单选底色
	if (m_bEnableSingleSel && (m_itemNum > 0) && (m_singleSelItem >= 0))
	{
		assert(m_curPage > 0);
		unsigned long begin = (m_curPage - 1)* m_lineNum;
		unsigned long end = m_curPage * m_lineNum;
		if ((m_singleSelItem >= begin) && (m_singleSelItem < end))
		{
			int x = m_rect.m_left;
			x += (m_bSelect) ? (m_length[CHECK_WIDTH] + 4) : 0;
			int y = m_rect.m_top + m_length[LIST_HEAD_HEIGHT] + (m_singleSelItem - begin) * m_length[LIST_EX_ITEM_HEIGHT];

			GUI::CWnd::DrawRect(x, y, m_rect.Width(), m_length[LIST_EX_ITEM_HEIGHT], COLOR_SELECT);
		}
	}
*/

	unsigned long color = COLOR_LINE_GRAY;
	int end = (m_rect.m_bottom - LIST_CTRL_BOTTOM_HEIGHT);

	//画列
	int x = m_rect.m_left;
	x += (m_bSelect) ? (m_length[CHECK_WIDTH] + 4) : 0;
	GUI::CWnd::DrawLine(m_rect.m_left, m_rect.m_top, m_rect.m_left, end+LIST_CTRL_BOTTOM_HEIGHT, color, 2);//最左边的竖条
	if (m_bSelect)
	{
		GUI::CWnd::DrawLine(m_rect.m_left+(m_length[CHECK_WIDTH] + 4), m_rect.m_top, m_rect.m_left+(m_length[CHECK_WIDTH] + 4), end, color, 1);
	}
	GUI::CWnd::DrawLine(m_rect.m_right-1, m_rect.m_top, m_rect.m_right-1, end+LIST_CTRL_BOTTOM_HEIGHT, color, 2);//最右边的竖条

	LISTBOX_COL_INFO::const_iterator iter;
	for (iter = m_colWidthVector.begin(); iter != m_colWidthVector.end() - 1; ++iter)
	{
		x += (*iter);
		GUI::CWnd::DrawLine(x, m_rect.m_top, x, end, color);
	}

	//画行
	int y = m_rect.m_top + m_length[LIST_HEAD_HEIGHT];// + m_length[LIST_EX_ITEM_HEIGHT];
	GUI::CWnd::DrawLine(m_rect.m_left, y, m_rect.m_right, y, color, 2);//最下面的横条

	y += m_length[LIST_EX_ITEM_HEIGHT];
	while (y < end)
	{
		GUI::CWnd::DrawLine(m_rect.m_left, y, m_rect.m_right, y, color);
		y += m_length[LIST_EX_ITEM_HEIGHT];
	}

	GUI::CWnd::DrawLine(m_rect.m_left, end, m_rect.m_right, end, color, 2);//表格下面的横条
	GUI::CWnd::DrawLine(m_rect.m_left, end+LIST_CTRL_BOTTOM_HEIGHT, m_rect.m_right, end+LIST_CTRL_BOTTOM_HEIGHT, color, 2);//最下面的横条
/*
	ShowData(status);
*/
	char pageInfo [32] = {0};
	snprintf(pageInfo, 32, "%d/%d", m_curPage, m_pageNum);
	unsigned short cx = 0, cy = 0;
	TextSize(pageInfo, strlen(pageInfo), cx, cy);
	GUI::CWnd::DrawText(pageInfo, strlen(pageInfo), m_rect.m_left+5, end + 4, cx, LIST_CTRL_BOTTOM_HEIGHT - 4);
}

template <class T>
int GUI::CListCtrlEx<T>::AddColumn(const char* pStr, int width)
{
	assert(NULL != pStr);

	m_colWidthVector.push_back(width);
	return m_header.AddItem(pStr, width);
}

template <class T>
void GUI::CListCtrlEx<T>::RefreshCheckStatus(bool bRefresh)
{
	assert(m_curPage > 0);
	int index = (m_curPage - 1)*m_lineNum;
	int end_index = index + m_lineNum - 1;
	if (end_index >= m_itemNum)
	{
		end_index = m_itemNum - 1;
	}

	for (int i = 0; i < m_lineNum; ++i)
	{
		if(m_bSelect)
		{
			//2010-01-30 11:11:00 YSW
			//清空之前需要全部设为使能才能设入选择和不选择
			m_pCKSelect[i].SetEnable(true, false);

			//
			m_pCKSelect[i].SetCheck(false, false);//先全部清空
		}

		m_pListItemVir[i].SetEnable(true, false);
	}

	//2010-01-30 11:11:00 YSW
	//检查有数据项的是否为选择状态，是则设为选择状态
	int check_index = 0;
	for (check_index = 0; (index <= end_index) && (check_index < m_lineNum); ++index, ++check_index)
	{
		if (m_bSelect && IsItemSelect(index))
		{
			m_pCKSelect[check_index].SetCheck(true, false);
		}
	}

	//2010-01-30 11:11:00 YSW
	//可能存在不足一页的情况，那么后面的那些项应该设为不使能状态
	for (; check_index < m_lineNum; ++check_index)
	{
		if(m_bSelect)
		{
			m_pCKSelect[check_index].SetEnable(false, false);
		}

		m_pListItemVir[check_index].SetEnable(false, false);
	}
}

template <class T>
bool GUI::CListCtrlEx<T>::IsItemSelect(int item) const
{
	for (SELECT_LIST::const_iterator iter = m_selectList.begin(); iter != m_selectList.end(); ++iter)
	{
		if (*iter == item)
		{
			return true;
		}
	}
	return false;
}

template <class T>
BEGIN_MSG_MAP(GUI::CListCtrlEx<T>, GUI::CWnd)
	ON_MSG(CTRLID_LIST_EX_BTN_FIRST, KEY_VIRTUAL_ENTER, OnFirstPage)
	ON_MSG(CTRLID_LIST_EX_BTN_PREV, KEY_VIRTUAL_ENTER,	OnPrevPage)
	ON_MSG(CTRLID_LIST_EX_BTN_NEXT, KEY_VIRTUAL_ENTER,	OnNextPage)
	ON_MSG(CTRLID_LIST_EX_BTN_END, KEY_VIRTUAL_ENTER,	OnEndPage)
	ON_MSG(CTRLID_LIST_EX_CK_ALL, KEY_VIRTUAL_ENTER,	OnSelAll)
	ON_MSG_CTRL_RANGE(CTRLID_LIST_EX_CK_SELECT_0, CTRLID_LIST_EX_CK_SELECT_31, KEY_VIRTUAL_ENTER, OnClickCheck)

	ON_MSG_EX(CTRLID_NULL, KEY_VIRTUAL_ENTER,	OnClickList)
	ON_MSG_EX(CTRLID_NULL, KEY_VIRTUAL_DBCLICK, OnDBClickList)
END_MSG_MAP()
//////////////////////////////////////////////////////////////////////////
template <class T>
unsigned long GUI::CListCtrlEx<T>::OnFirstPage()
{
	if ((m_itemNum > m_lineNum) && (0 != m_currItem))
	{
		m_currItem = 0;
		m_curPage = 1;
		RefreshCheckStatus(false);
		Repaint();
	}
	return KEY_VIRTUAL_MSG_STOP;
}

template <class T>
unsigned long GUI::CListCtrlEx<T>::OnPrevPage()
{
	if ((m_itemNum > m_lineNum) && (0 != m_currItem))
	{
		unsigned long i = 0;
		do 
		{
			-- m_currItem;
			++ i;
		} while((i < m_lineNum) && (0 != m_currItem));

		-- m_curPage;
		RefreshCheckStatus(false);
		Repaint();
	}

	return KEY_VIRTUAL_MSG_STOP;
}

template <class T>
unsigned long GUI::CListCtrlEx<T>::OnNextPage()
{
	if ((m_itemNum > m_lineNum) && ((m_currItem+m_lineNum) < m_itemNum))
	{
		unsigned long i = 0;
		do 
		{
			++ m_currItem;
			++ i;
		} while(i < m_lineNum);

		++ m_curPage;
		RefreshCheckStatus(false);
		Repaint();
	}

	return KEY_VIRTUAL_MSG_STOP;
}

template <class T>
unsigned long GUI::CListCtrlEx<T>::OnEndPage()
{
	if ((m_itemNum > m_lineNum) && ((m_currItem+m_lineNum) < m_itemNum))
	{
		//修改问题：按事件搜索，如果有多页数据，点击到最后一页，点击最后一条，经常会没有任何反应；
		//原因：点击最后一页按钮，把数据的序号破坏了。
		m_currItem = (m_pageNum - 1) * m_lineNum;
		m_curPage = m_pageNum;
		RefreshCheckStatus(false);
		Repaint();
	}

	return KEY_VIRTUAL_MSG_STOP;
}

template <class T>
unsigned long GUI::CListCtrlEx<T>::OnSelAll()
{
	if (0 < m_itemNum)
	{
		bool bCheckAll = m_ckAll.IsCheck();

		int check_count = m_lineNum;
		if (m_curPage == m_pageNum)
		{
			check_count = m_itemNum % m_lineNum;
			if (0 == check_count)
			{
				check_count = m_lineNum;
			}
		}

		for (int i = 0; i < check_count; ++i)
		{
			m_pCKSelect[i].SetCheck(bCheckAll, true);
		}

		m_selectList.clear();
		if (bCheckAll)
		{
			for (int i = 0; i < m_itemNum; ++i)
			{
				m_selectList.push_back(i);
			}
		}
	}
	
	return KEY_VIRTUAL_MSG_STOP;
}

template <class T>
unsigned long GUI::CListCtrlEx<T>::OnClickCheck(unsigned long ctrlID)
{
	int check_index = ctrlID - CTRLID_LIST_EX_CK_SELECT_0;
	if ((check_index >= 0) && (check_index < m_lineNum) && (check_index < m_itemNum))
	{
		int item_index = check_index + (m_curPage - 1)*m_lineNum;
		
		if (m_pCKSelect[check_index].IsCheck())
		{
			for (SELECT_LIST::const_iterator iter = m_selectList.begin(); iter != m_selectList.end(); ++iter)
			{
				if (*iter == item_index)
				{
					//2010-02-08 10:37:00 YSW
					//如果才选择却已经在选择列表中，说明有次非正常操作才是，所以增加断言。
					//assert(false);
					return KEY_VIRTUAL_MSG_STOP;
				}
			}

			m_selectList.push_back(item_index);

			//2010-02-08 10:33:00 YSW
			//如果选择了全部项，则把选择全部的CHECK置为选中状态
			if (!m_ckAll.IsCheck() && (m_itemNum == m_selectList.size()))
			{
				m_ckAll.SetCheck(true, false);
			}
			
			//如果选择全部的CHECK置为选中状态，但是当前选择的项不是全部，则置为不选状态
			if (m_ckAll.IsCheck() && (m_selectList.size() < m_itemNum))
			{
				m_ckAll.SetCheck(false, false);
			}

			return KEY_VIRTUAL_MSG_STOP;
		} 
		else
		{
			for (SELECT_LIST::iterator iter = m_selectList.begin(); iter != m_selectList.end(); ++iter)
			{
				if (*iter == item_index)
				{
					m_selectList.erase(iter);
					return KEY_VIRTUAL_MSG_STOP;
				}
			}

			return KEY_VIRTUAL_MSG_STOP;
		}
	}
	else
	{
		return KEY_VIRTUAL_MSG_STOP;
	}
}

template <class T>
unsigned long GUI::CListCtrlEx<T>::OnClickList(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	assert (KEY_VIRTUAL_ENTER == keyID);
	int hitIndex = -1;
	bool bHit = false;
	for (int i = 0; i < m_lineNum; i++ )
	{
		if(m_pListItemVir[i].GetID() == ctrlID)
		{
			bHit = true;
			hitIndex = i;
			break;
		}
	}

	if(GetID() == ctrlID)
	{
		bHit = true;
	}

	if(!bHit)
	{
		return KEY_VIRTUAL_MSG_CONTINUE;
	}

	if (m_bEnableSingleSel)//设置可单选
	{
		if( (m_width <= xPos) || (m_height <= yPos))
		{
			//按键板传来消息Enter消息
			if(hitIndex >= 0)
			{
				long item = hitIndex + (m_curPage - 1) * m_lineNum;
				if ((item >= 0) && (item < m_itemNum))
				{
					if(m_singleSelItem != item)
					{
						DrawItem(m_singleSelItem, ITEM_BG | ITEM_RC, true);
						DrawItem(item, ITEM_BG | ITEM_RC | ITEM_FOCUS | ITEM_SEL_BG, true);
					}
					m_singleSelItem = item;
				}
			}
		}
		else
		{
			//鼠标传来消息Enter消息
			long item = LineIndex(xPos, yPos);
			if(item >= 0 && item < m_lineNum)
			{
				item += (m_curPage - 1) * m_lineNum;
				if ((item >= 0) && (item < m_itemNum))
				{
					if(m_singleSelItem != item)
					{
						DrawItem(m_singleSelItem, ITEM_BG | ITEM_RC, true);
						DrawItem(item, ITEM_BG | ITEM_RC | ITEM_FOCUS | ITEM_SEL_BG, true);
					}
					m_singleSelItem = item;
				}
			}
		}
	}

	return KEY_VIRTUAL_MSG_CONTINUE;
}

template <class T>
unsigned long GUI::CListCtrlEx<T>::OnDBClickList(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	/*
		双击、屏蔽无效数据区域的消息
	*/

	for (int i = 0; i< m_lineNum; ++i)
	{
		if(m_pListItemVir[i].GetID() == ctrlID)
		{
			return KEY_VIRTUAL_MSG_CONTINUE;
		}
	}

	return KEY_VIRTUAL_MSG_STOP;
}

template <class T>
long GUI::CListCtrlEx<T>::LineIndex(unsigned long xPos, unsigned long yPos)
{
	if ((xPos < m_rect.m_left) || (m_rect.m_right < xPos))
	{
		return -1;
	}

	if (yPos < (m_rect.m_top + m_length[LIST_HEAD_HEIGHT]))
	{
		return -1;
	}

	if (yPos > (m_rect.m_bottom - LIST_CTRL_BOTTOM_HEIGHT))
	{
		return -1;
	}

	long index = (yPos - (m_rect.m_top+m_length[LIST_HEAD_HEIGHT])) / m_length[LIST_EX_ITEM_HEIGHT];

	if(index >= m_lineNum)
	{
		return -1;
	}
	else
	{
		return index;
	}
}

template <class T>
void GUI::CListCtrlEx<T>::ShowData(PAINT_STATUS status)
{
	if (m_itemNum > 0)
	{
		unsigned short x = 0;//m_rect.m_left + m_length[LIST_ITEM_XPOS];
		unsigned short y = m_rect.m_top + m_length[LIST_HEAD_HEIGHT] + m_length[LIST_ITEM_YPOS];

		unsigned long index = m_currItem;
		int i = 0;
		while((i<m_lineNum) && (index < m_itemNum))
		{
			x = m_rect.m_left + m_length[LIST_ITEM_XPOS];
			x += (m_bSelect) ? (m_length[CHECK_WIDTH] + 4) : 0;

			if ((PAINT_STATUS_FOCUS == status) && (i == m_currLine))
			{
				for (int j=0; j<m_colWidthVector.size(); ++j)
				{
					std::string & str = Text(m_pData[index], j);

					if (str.length() > 0)
					{
						GUI::CWnd::DrawText(str.c_str(), str.length(), x, y, m_colWidthVector[j]-m_length[LIST_ITEM_XPOS], m_length[LIST_EX_ITEM_HEIGHT], GUI_RGB(255, 255, 0));
					}

					x += m_colWidthVector[j];
				}
			}
			else
			{
				for (int j=0; j<m_colWidthVector.size(); ++j)
				{
					std::string & str = Text(m_pData[index], j);

					if (str.length() > 0)
					{
						GUI::CWnd::DrawText(str.c_str(), str.length(), x, y, m_colWidthVector[j]-m_length[LIST_ITEM_XPOS], m_length[LIST_EX_ITEM_HEIGHT]);
					}

					x += m_colWidthVector[j];
				}
			}

			y += m_length[LIST_EX_ITEM_HEIGHT];
			++ index;
			++ i;
		}
	}
}

template<class T>
void GUI::CListCtrlEx<T>::SetSingleSelect(long item, bool bRefresh)
{
	if (!m_bEnableSingleSel)
	{
		return;
	}

	if ((item >= 0) && (item < m_itemNum))
	{
		m_singleSelItem = item;

		m_curPage = item / m_lineNum + 1;
		m_currItem = (m_curPage - 1)* m_lineNum;
		
		RefreshCheckStatus(false);

		if (bRefresh)
		{
			Repaint();
		}
	} 
	else
	{
		assert(false);
	}
}

template<class T>
long GUI::CListCtrlEx<T>::ForEach(long param, bool bRefresh /* = false */)
{
	for (int i = 0; i < m_itemNum; ++i)
	{
		ForEach(m_pData[i], i, param);
	}

	if (bRefresh)
	{
		Repaint();
	}

	return 0;
}

template<class T>
const T* GUI::CListCtrlEx<T>::GetItem(int item) const
{
	if ((NULL != m_pData) && (item >= 0) && (item < m_itemNum))
	{
		return &(m_pData[item]);
	} 
	else
	{
		assert(false);
		return NULL;
	}
}

template<class T>
T* GUI::CListCtrlEx<T>::GetItem(int item) 
{
	if ((NULL != m_pData) && (item >= 0) && (item < m_itemNum))
	{
		return &(m_pData[item]);
	} 
	else
	{
		assert(false);
		return NULL;
	}
}


template<class T>
void GUI::CListCtrlEx<T>::CleanSelected( ) 
{
	m_ckAll.SetCheck(false, true);
	OnSelAll();
	//m_ckAll.SetCheck(false, true);
}

template<class T>
void GUI::CListCtrlEx<T>::SetSelAll( ) 
{
	if (!m_ckAll.IsCheck())
	{
		m_ckAll.SetCheck(true, true);
	}	
	OnSelAll();
	//m_ckAll.SetCheck(false, true);
}

template<class T>
void GUI::CListCtrlEx<T>::DrawItem(long item, unsigned long style, bool bRefresh)
{
	/*
		绘制单行,按ITEM_DRAW_STYLE样式绘制
		ITEM_SEL_BG:绘制单选选中行背景；ITEM_FOCUS:绘制行前景
	*/
	if( !(m_itemNum > 0) || !(item >= 0) || !(item < m_itemNum) )
	{
		return;
	}

	assert(m_curPage > 0);
	unsigned long begin = (m_curPage - 1)* m_lineNum;
	unsigned long end = m_curPage * m_lineNum;
	end = (end > m_itemNum) ? m_itemNum : end;

	if ((item >= begin) && (item < end))
	{
		unsigned long index = item - begin;

		int x_left = m_rect.m_left;
		x_left += (m_bSelect) ? (m_length[CHECK_WIDTH] + 4) : 0;
		int y_top = m_rect.m_top + m_length[LIST_HEAD_HEIGHT] + index * m_length[LIST_EX_ITEM_HEIGHT];
		int cx = (m_bSelect) ? (m_rect.Width() - m_length[CHECK_WIDTH] - 4) : m_rect.Width();

		// * 绘制背景
		if(style & ITEM_SEL_BG)
		{
			GUI::CWnd::DrawRect(x_left, y_top, cx, m_length[LIST_EX_ITEM_HEIGHT], COLOR_SELECT, false);
		}
		else if(style & ITEM_BG)
		{
			GUI::CWnd::DrawRect(x_left, y_top, cx, m_length[LIST_EX_ITEM_HEIGHT], COLOR_DLG_CHILD_BK, false);
		}

		// * 绘制线条
		unsigned long color = COLOR_LINE_GRAY;
		int y_end = y_top + LIST_CTRL_BOTTOM_HEIGHT;

		//画列
		if (m_bSelect)
		{
			GUI::CWnd::DrawLine(x_left, y_top, x_left, y_end - 2, color, 1);
		}
		else
		{
			GUI::CWnd::DrawLine(x_left, y_top, x_left, y_end - 3, color, 2);//最左边的竖条
		}

		GUI::CWnd::DrawLine(m_rect.m_right - 1, y_top, m_rect.m_right - 1, y_end - 3, color, 2);//最右边的竖条

		int x = x_left;
		LISTBOX_COL_INFO::const_iterator iter;
		for (iter = m_colWidthVector.begin(); iter != m_colWidthVector.end() - 1; ++iter)
		{
			x += (*iter);
			GUI::CWnd::DrawLine(x, y_top, x, y_end - 2, color);
		}

		//画行
		if(index <= 0)
		{
			GUI::CWnd::DrawLine(x_left, y_top, m_rect.m_right - 1, y_top, color, 2);
			GUI::CWnd::DrawLine(x_left, y_end - 2, m_rect.m_right, y_end - 2, color, 1);
		}
		else if(index >= m_lineNum - 1)
		{
			GUI::CWnd::DrawLine(x_left, y_top, m_rect.m_right, y_top, color, 1);
			GUI::CWnd::DrawLine(x_left, y_end - 3, m_rect.m_right, y_end - 3, color, 2);
		}
		else
		{
			GUI::CWnd::DrawLine(x_left, y_top, m_rect.m_right, y_top, color, 1);
			GUI::CWnd::DrawLine(x_left, y_end - 2, m_rect.m_right, y_end - 2, color, 1);
		}

		// * 绘制文字
		x = x_left + m_length[LIST_ITEM_XPOS];
		int y = y_top + m_length[LIST_ITEM_YPOS];
		for (int j = 0; j < m_colWidthVector.size(); ++j)
		{
			std::string & str = Text(m_pData[item], j);

			if (str.length() > 0)
			{
				if(style & ITEM_FOCUS)
				{
					GUI::CWnd::DrawText(str.c_str(), str.length(), x, y, m_colWidthVector[j] - 2 * m_length[LIST_ITEM_XPOS], m_length[LIST_EX_ITEM_HEIGHT], COLOR_TEXT_FOCUS);
				}
				else
				{
					GUI::CWnd::DrawText(str.c_str(), str.length(), x, y, m_colWidthVector[j] - 2 * m_length[LIST_ITEM_XPOS], m_length[LIST_EX_ITEM_HEIGHT]);
				}
			}

			x += m_colWidthVector[j];
		}

		// * 刷新
		if(bRefresh)
		{
			GUI::CWnd::Refresh(x_left, y_top, cx, m_length[LIST_EX_ITEM_HEIGHT]);
		}
	}
}

template<class T>
void GUI::CListCtrlEx<T>::OnDrawItem(int index, PAINT_STATUS status)
{
	assert(index >= 0);
	assert(index < m_lineNum);

	if(index >= m_lineNum)
	{
		return;
	}

	long begin = (m_curPage - 1)* m_lineNum;
	long end = m_curPage * m_lineNum;
	end = (end > m_itemNum) ? m_itemNum : end;

	long item = begin + static_cast<long>(index);

	unsigned long style = ITEM_BG | ITEM_RC;
	if(PAINT_STATUS_FOCUS == status)
	{
		style |= ITEM_FOCUS;
	}

	if(m_bEnableSingleSel && m_singleSelItem == item)
	{
		style |= ITEM_SEL_BG;
	}

	DrawItem(item, style, false);
}

template<class T>
void GUI::CListCtrlEx<T>::OnFocusItem(int index)
{
	/*
		现在的处理方式是：当子窗口Item获取焦点需要重绘时
	通知本处理过程
	*/

	assert(index >= 0);
	assert(index < m_lineNum);

	if(!m_bItemFocusNotify || index >= m_lineNum || index < 0 || !m_pListItemVir[index].IsShow())
	{
		return;
	}

	long begin = (m_curPage - 1)* m_lineNum;
	long end = m_curPage * m_lineNum;
	end = (end > m_itemNum) ? m_itemNum : end;

	long item = begin + static_cast<long>(index);


	//添加特殊消息,当前焦点在本窗口内,由上层得到消息并处理
	KEY_VALUE_INFO key;
	memset(&key, 0, sizeof(KEY_VALUE_INFO));

	key.type = KEY_TYPE_VIRTUAL;
	key.value = KEY_VIRTUAL_MSG_DEFAULT_TAG | static_cast<unsigned long>(item);

	GUI::CDialog::AddKeyValueEx(key);	
}

//////////////////////////////////////////////////////////////////////////

