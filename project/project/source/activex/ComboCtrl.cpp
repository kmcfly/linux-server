#include "ComboCtrl.h"

using namespace GUI;

//////////////////////////////////////////////////////////////////////////
//组合框弹出菜单项目
//////////////////////////////////////////////////////////////////////////
CComboItem::CComboItem() : m_bSel(false)
{
	m_ctrlType = GUI::WND_TYPE_COMBO_ITEM;
}

CComboItem::~CComboItem()
{

}

bool CComboItem::Create(unsigned long ctrlID, unsigned long x, unsigned long y, unsigned long cx, unsigned long cy,
						CWnd* pParent, int xFocus, int yFocus)
{
	unsigned long style = WND_CHILD | WND_VISIBLE | WND_NO_BORDER | WND_NO_IMAGE_BK;

	if (CWnd::Create(ctrlID, x, y, cx, cy, pParent, style, xFocus, yFocus))
	{
		return true;
	}
	else
	{
		return false;
	}
}

void CComboItem::OnPaint(PAINT_STATUS status)
{
	if( m_bSel )
	{
		GUI::CWnd::DrawRect(m_rect.m_left, m_rect.m_top, m_rect.Width(), m_rect.Height(), COLOR_SELECT);
	}
	else
	{
		GUI::CWnd::DrawRect(m_rect.m_left, m_rect.m_top, m_rect.Width(), m_rect.Height(), COLOR_EDIT);
	}

	unsigned short str_cx = 0;
	unsigned short str_cy = 0;
	if (!m_strCaption.empty())
	{
		int text_x = m_rect.m_left+m_length[EDIT_XPOS];
		int text_y = m_rect.m_top+1;

		if( m_bSel )
		{
			text_x += 1;
			text_y += 1;
		}
	
		GUI::CWnd::DrawText(m_strCaption.c_str(), m_strCaption.length(), text_x, text_y, m_rect.Width(), m_rect.Height(), COLOR_TEXT_NORMAL);
	} 
}

bool CComboItem::SetFocus(bool bFocus, bool bRefresh)
{
	return GUI::CWnd::SetFocus(bFocus, false);
}

//////////////////////////////////////////////////////////////////////////
//组合框弹出菜单
//////////////////////////////////////////////////////////////////////////
CComboMenu::CComboMenu() : m_cbItemVec(NULL)
, m_pageCount(-1)
, m_curSel(-1)
, m_bInit(false)
{
	m_ctrlType = GUI::WND_TYPE_COMBO_MENU;

	SetBackgroundColor(COLOR_EDIT);
	SetBorderColor(COLOR_SELECT);

	SetMouseOverOffset(1);
}

CComboMenu::~CComboMenu()
{

}

void CComboMenu::OnInitial()
{
	m_style |= WND_NO_TITLE;
	GUI::CScrollDialog::OnInitial();
	//////////////////////////////////////////////////////////////////////////
	GUI::CScrollWnd& wnd = GetScrollWnd();

	assert( m_cbItemVec!=NULL );		//必须初始化m_cbItemVec
	assert( m_cbItemVec->size()>0 );	//

	unsigned long width = 0;
	unsigned long pageCount = GetPageCount();
	if( m_cbItemVec->size() > pageCount )
	{
		width = m_rect.Width() - m_length[SCROLL_WIDTH] - 2;
	}
	else
	{
		width = m_rect.Width();
	}

	//这里的数据调整不合理可能引起滚动窗口将控件裁剪掉
	SetScrollWndPos(1,2,1,1);
	wnd.SetSizeOffset(width-2,m_length[COMBO_HEIGHT]);
	wnd.SetViewMaxSize(width-2,m_length[COMBO_HEIGHT] * m_cbItemVec->size());

	for ( int i = 0; i<m_cbItemVec->size(); i++ )
	{
		//滚动窗口要求控件必须能被容纳在视图内
		GUI::CRect rect(0,0, width - 6,m_length[STATIC_HEIGHT]);
		unsigned long x, y,cx,cy;
		wnd.GetViewPos( 0,i,x,y);
		wnd.GetViewSize( 0,i,cx,cy);

		rect.Offset(x,y);
		GUI::CComboItem* pItem = dynamic_cast<GUI::CComboItem*>(wnd.AddWnd( WND_TYPE_COMBO_ITEM,GetRandWndID(),rect,0,i,(*m_cbItemVec)[i].item));
		assert(pItem);

		pItem->SetCaption((*m_cbItemVec)[i].str.c_str(),false);
	}

	////
	m_bInit = true;
	SetCurSel(m_curSel, false);
}

void CComboMenu::OnPostInitial()
{
	CScrollDialog::OnPostInitial();

	int sel = GetCurSel();
	sel = sel > 0 ? sel : 0;
	SetCurScrollPos(0,sel,false);

	//

}

void CComboMenu::OnDestroy()
{
	CScrollDialog::OnDestroy();

	////
	m_bInit = false;
}

void CComboMenu::OnOutOfDlg(unsigned long keyID)
{
	GUI::CScrollDialog::OnOutOfDlg(keyID);

	if (KEY_VIRTUAL_ENTER == keyID)
	{
		EndDialog(DLG_ESC);
	}
}

void CComboMenu::OnChangeFocus(unsigned long oldID, unsigned long newID)
{
	//GUI::CScrollWnd& wnd = GetScrollWnd();
	//int sel = wnd.GetItemWndIndex( newID );
}

unsigned long CComboMenu::OnFocusProcEx(unsigned long keyID, unsigned long xPoint, unsigned long yPoint)
{
	if (KEY_VIRTUAL_MOVE == keyID)
	{
		return OnMove(CTRLID_NULL, keyID, xPoint, yPoint);
		//return KEY_VIRTUAL_MSG_CONTINUE;
	}
	else if (KEY_VIRTUAL_UP == keyID)
	{
		int sel = GetCurSel();
		//int count = GetCountSel();

		int xPos = 0, yPos = 0;
		GetCurScrollPos(xPos, yPos);

		bool bRepaint = false;

		if(sel - 1 >= 0)
		{
			sel -= 1;

			//超过缩放窗口时,重新定位
			if( sel < yPos || sel > yPos + m_pageCount)
			{
				SetCurScrollPos(0,sel,false);
				bRepaint = true;
			}

			//设置当前选择
			SetCurSel(sel, !bRepaint);
		}
		else
		{
			if( sel < yPos || sel > yPos + m_pageCount)
			{
				SetCurScrollPos(0,sel,false);
				bRepaint = true;
			}
		}

		if(bRepaint)
		{
			Repaint();
		}

		return KEY_VIRTUAL_MSG_STOP;
	}
	else if (KEY_VIRTUAL_DOWN == keyID)
	{
		int sel = GetCurSel();
		int count = GetCountSel();

		int xPos = 0, yPos = 0;
		GetCurScrollPos(xPos, yPos);

		bool bRepaint = false;

		if(sel + 1 < count)
		{
			sel += 1;

			//超过缩放窗口时,重新定位
			if( sel < yPos || sel >= yPos + m_pageCount)
			{
				if( sel >= yPos + 1 && sel < yPos + m_pageCount + 1)
				{
					SetCurScrollPos(0, yPos + 1, false);
				}
				else
				{
					SetCurScrollPos(0, sel, false);
				}
				bRepaint = true;
			}

			//设置当前选择
			SetCurSel(sel, !bRepaint);
		}
		else
		{
			if( sel < yPos || sel > yPos + m_pageCount)
			{
				SetCurScrollPos(0,sel,false);
				bRepaint = true;
			}
		}

		if(bRepaint)
		{
			Repaint();
		}

		return KEY_VIRTUAL_MSG_STOP;
	}

	return KEY_VIRTUAL_MSG_STOP;
}

void CComboMenu::SetCurSel(int item,bool bRefresh)
{
	if( m_bInit )
	{
		assert(m_cbItemVec);
		assert(item < m_cbItemVec->size());

		GUI::CComboItem* pOldSelCb = dynamic_cast<GUI::CComboItem*>(GetScrollWnd().GetWnd(m_curSel));
		assert(pOldSelCb);
		if( pOldSelCb->GetSel() )
		{
			pOldSelCb->SetSel(false);
			if( bRefresh && m_curSel != item)
			{
				pOldSelCb->Repaint();
			}
		}
		else
		{
			for ( int i = 0; i < m_cbItemVec->size(); i++ )
			{
				GUI::CComboItem* pCbItem = dynamic_cast<GUI::CComboItem*>(GetScrollWnd().GetWnd(i));
				assert(pCbItem);
				if( pCbItem->GetSel() )
				{
					pCbItem->SetSel(false);

					if( bRefresh && m_curSel != item)
					{
						pCbItem->Repaint();
					}
				}
			}
		}

		GUI::CComboItem* pNewSelCb = dynamic_cast<GUI::CComboItem*>(GetScrollWnd().GetWnd(item));
		assert(pNewSelCb);
		pNewSelCb->SetSel(true);
		if( bRefresh && m_curSel != item)
		{
			pNewSelCb->Repaint();
		}
	}

	m_curSel = item;
}

int CComboMenu::GetCurSel()
{
	return m_curSel;
}

int CComboMenu::GetCountSel()
{
	if(m_cbItemVec)
	{
		return m_cbItemVec->size();
	}
	else
	{
		return 0;
	}
}

void CComboMenu::SetPageCount(int pageCount )
{
	m_pageCount = pageCount;
}

int CComboMenu::GetPageCount()
{
	if( m_pageCount <= 0)
	{
		m_pageCount = m_rect.Height()/m_length[COMBO_HEIGHT];
	}

	assert(m_pageCount>0);
	return m_pageCount;
}

void CComboMenu::SetItemVector(COMBO_ITEM_VECTOR& itemVec )
{
	m_cbItemVec = &itemVec;
}

BEGIN_MSG_MAP(CComboMenu, CScrollDialog)
	ON_MSG_EX(GetScrollWnd().GetID(),KEY_VIRTUAL_ENTER,OnSelItem)
	ON_MSG_EX(GetID(),KEY_VIRTUAL_ENTER,OnSelItem)
END_MSG_MAP()

unsigned long CComboMenu::OnSelItem(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	//int index = -1;
	//unsigned long tmpIndex = (keyID >> 20)&0x7FF;
	//if( tmpIndex < 0x7FF )
	//{
	//	index = tmpIndex;
	//}
	//unsigned long id = keyID&0xFFFFF;

	//if( index >= 0 )
	//{
	//	SetCurSel(index, false);
	//	EndDialog(DLG_OK);
	//}

	/*
		Modify[2010-01-25 L]
		原算法依赖于焦点变迁,满足不了:
		当没有移动事件,不产生焦点变迁时,的点击操作;
		即原算法要求先移动再点击,才有正确结果
	*/
	if( (m_width <= xPos) || (m_height <= yPos))
	{
		EndDialog(DLG_OK);
	}
	else
	{
		//鼠标点击事件(x,y)不可能为无效值
		int x_view = xPos;
		int y_view = yPos;
		ScreenToClient( x_view, y_view );

		int xIndex = 0,yIndex = 0;
		if( GetScrollWnd().GetCurViewIndex(x_view,y_view,xIndex,yIndex) && xIndex == 0)
		{
			assert(yIndex >= 0);
			assert(yIndex < m_cbItemVec->size());
			SetCurSel(yIndex, false);
			EndDialog(DLG_OK);
		}
	}

	return KEY_VIRTUAL_MSG_CONTINUE;
}

unsigned long CComboMenu::OnMove(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	int  x_view = xPos;
	int y_view = yPos;
	ScreenToClient( x_view, y_view );

	int xIndex = 0,yIndex = 0;
	if( GetScrollWnd().GetCurViewIndex(x_view,y_view,xIndex,yIndex) && xIndex == 0)
	{
		assert(yIndex >= 0);
		assert(yIndex < m_cbItemVec->size());
		SetCurSel(yIndex, true);
	}

	return KEY_VIRTUAL_MSG_CONTINUE;
}

//////////////////////////////////////////////////////////////////////////
//组合框
//////////////////////////////////////////////////////////////////////////
CComboCtrl::CComboCtrl() : m_curItem(INVALID_ITEM)
,m_countDropDown(6)
,m_maxEditStringLen(32)
{
	m_ctrlType = GUI::WND_TYPE_COMBO;

	m_invalidItem.item = -1;
	m_invalidItem.data = -1;
	m_invalidItem.str = "";
}

CComboCtrl::~CComboCtrl()
{

}

int  CComboCtrl::AddItem(const char* pStr, LONGLONG data /*= 0*/)
{
	CTRL_COMBO_ITEM comboItem;
	comboItem.item = m_itemVec.size();
	comboItem.str = pStr;
	comboItem.data = data;

	m_itemVec.push_back(comboItem);
	return comboItem.item;
}

void CComboCtrl::RemoveAllItem(bool bRefresh)
{
	m_itemVec.clear();
	m_editText.SetCaption("", false);

	if (bRefresh)
	{
		Repaint();
	}
}

unsigned long CComboCtrl::GetItemCount() const
{
	return m_itemVec.size();
}

void CComboCtrl::RemoveItem(int item, bool bRefresh /*= true*/)
{
	bool bRemove = false;

	for (COMBO_ITEM_VECTOR::iterator it = m_itemVec.begin(); it != m_itemVec.end(); )
	{
		if (bRemove)
		{
			it->item -= 1;
		}

		if ((std::distance(m_itemVec.begin(), it) == item) && !bRemove)
		{
			it = m_itemVec.erase(it);
			bRemove = true;
		}
		else
		{
			++it;
		}
	}

	if (bRefresh)
	{
		Repaint();
	}
}

void CComboCtrl::SetCurItem(int item, bool bRefresh /*= false*/)
{
	if (IsValidItem(item))
	{
		m_editText.SetCaption(m_itemVec[item].str, bRefresh);
		m_curItem = item;
	}
}

bool CComboCtrl::SetItemData(int item, LONGLONG data)
{
	if (item >= m_itemVec.size())
	{
		assert(false);
		return false;
	} 
	else
	{
		m_itemVec[item].data = data;
		return true;
	}
}

LONGLONG CComboCtrl::GetItemData(int item) const
{
	if ((item >= m_itemVec.size()) || (item < 0))
	{
		//assert(false);
		return -1;
	} 
	else
	{
		return m_itemVec[item].data;
	}
}

void CComboCtrl::GetItemString(int item, std::string& strItem) const
{
	if ((item >= m_itemVec.size()) || (item < 0))
	{
		//assert(false);
		strItem = m_invalidItem.str;
	} 
	else
	{
		strItem = m_itemVec[item].str;
	}
}

bool CComboCtrl::SetCurItemData(LONGLONG data, bool bRefresh)
{
	int item = INVALID_ITEM;

	for (COMBO_ITEM_VECTOR::const_iterator iter = m_itemVec.begin(); iter != m_itemVec.end(); ++iter)
	{
		if ((*iter).data == data)
		{
			item = (*iter).item;
			break;
		}
	}

	if (INVALID_ITEM != item)
	{
		SetCurItem(item, bRefresh);
		return true;
	} 
	else
	{
		printf("Set current item data=0x%X.\n", data);
		//gui_assert(false, Dump());
		//return false;

		/*
			[2011-02-16 L] 允许设置其他非初始化设定的值,记录在m_invalidItem中
		*/

		m_invalidItem.data = data;
		char buff[128] = {0};
		snprintf(buff, 128, "%ld", m_invalidItem.data);		
		m_invalidItem.str = buff;

		m_editText.SetCaption(m_invalidItem.str, bRefresh);
		return true;
	}
}

LONGLONG CComboCtrl::GetCurItemData() const
{
	int item = GetCurItem();
	if (INVALID_ITEM != item)
	{
		return GetItemData(item);
	} 
	else
	{
		return m_invalidItem.data;
		//assert(false);
		//return -1;
	}
}

void CComboCtrl::SetMaxStringLen(int maxLen)
{
	m_maxEditStringLen = maxLen;
	m_editText.SetMaxStringLen(maxLen);
}

int CComboCtrl::GetMaxStringLen() const
{
	return m_editText.GetMaxStringLen();
}

void CComboCtrl::SetEnable(bool bEnable, bool bRefresh /*= true*/)
{
	GUI::CWnd::SetEnable( bEnable, false);
	//m_editText.SetEnable(false,false);

	if( bRefresh )
	{
		Repaint();
	}
}

void CComboCtrl::SetDropDownCount(int count)
{
	m_countDropDown = count;
}

void CComboCtrl::SetEditText(const char* pStr, bool bRefresh)
{
	/*
		直接设置编辑框的值,不影响选择项
	*/
	m_editText.SetCaption(pStr, false);

	if(bRefresh)
	{
		Repaint();
	}
}

const char *CComboCtrl::GetItemText(int item)
{
	if((item < 0) || (item >= m_itemVec.size()))
	{
		return NULL;
	}

	return m_itemVec[item].str.c_str();
}

bool CComboCtrl::Create(unsigned long ctrlID, unsigned long x, unsigned long y, unsigned long cx, unsigned long cy,
						  GUI::CWnd* pParent, int xFocus, int yFocus)
{
	unsigned long style = WND_CHILD | WND_VISIBLE;

	unsigned long w = m_length[COMBO_WIDTH];
	cx = (cx < w) ? w : cx;
	cy = (cy < m_length[COMBO_HEIGHT]) ? m_length[COMBO_HEIGHT] : cy;

	if (GUI::CWnd::Create(ctrlID, x, y, cx, cy, pParent, style, xFocus, yFocus))
	{
		return true;
	}
	else
	{
		return false;
	}
}

void CComboCtrl::OnInitial()
{
	CWnd::OnInitial();
	//////////////////////////////////////////////////////////////////////////
	unsigned long x = 0, y = 0;
	unsigned long width = m_rect.Width(),height = m_rect.Height();

	m_editText.Create(GetRandWndID(),x+1,y,width-m_length[COMBO_WIDTH]-3,height,this,1,0,true,NULL,false);
	//m_editText.SetEnable(false,false);
	m_editText.SetMaxStringLen(m_maxEditStringLen);
	m_editText.SetCurosrMode(CEditCtrl::CURSOR_MODE_NO);
	m_editText.SetBkColor(PAINT_STATUS_FOCUS,COLOR_EDIT,COLOR_LINE_WHITE);
	m_editText.EnableInput(false);
	m_editText.SetEnable(false, false);

	x = width - m_length[COMBO_WIDTH]-2;
	//[20x20]
	m_btnDropDown.Create(GetRandWndID(),x,y+1,m_length[COMBO_WIDTH],height,this,0,0);
	m_btnDropDown.SetImage(PAINT_STATUS_NORMAL,BMP_COMBO_BTN_DOWN,false);
	m_btnDropDown.SetImage(PAINT_STATUS_FOCUS,BMP_COMBO_BTN_DOWN_FOCUS,false);
	m_btnDropDown.SetImage(PAINT_STATUS_DISABLE,BMP_COMBO_BTN_DOWN_DISABLE,false);
}

void CComboCtrl::OnPaint(PAINT_STATUS status)
{
	GUI::CWnd::DrawRectBorder(m_rect.m_left, m_rect.m_top, m_rect.Width(), m_rect.Height(), COLOR_LINE_WHITE);
}

void CComboCtrl::OnDestroy()
{
	GUI::CWnd::OnDestroy();

	m_itemVec.clear();
}

void CComboCtrl::OnChangeFocus(unsigned long oldID, unsigned long newID)
{

}

void CComboCtrl::MoveWnd(int x, int y)
{
	m_rect.MoveToXY(x, y);
	m_showRect.MoveToXY(x, y);

	m_editText.MoveWnd(x+1,y);
	m_btnDropDown.MoveWnd(x+m_rect.Width()- m_length[COMBO_WIDTH]-2,y+1);
}

bool CComboCtrl::IsValidItem(int item) const
{
	if ((item >= 0) && (item < m_itemVec.size()))
	{
		return true;
	}
	else
	{
		return false;
	}
}

BEGIN_MSG_MAP(CComboCtrl, GUI::CWnd)
	ON_MSG(CTRLID_NULL,KEY_VIRTUAL_ENTER,OnBtnDropDown)
	//ON_MSG(m_editText.GetID(), KEY_VIRTUAL_ADD, OnAdd)
	//ON_MSG(m_editText.GetID(), KEY_VIRTUAL_DEC, OnDec)
	//ON_MSG(m_btnDropDown.GetID(),KEY_VIRTUAL_ADD,OnAdd)
	//ON_MSG(m_btnDropDown.GetID(),KEY_VIRTUAL_DEC,OnDec)
	//ON_MSG(GetID(),KEY_VIRTUAL_ADD,OnAdd)
	//ON_MSG(GetID(),KEY_VIRTUAL_DEC,OnDec)
END_MSG_MAP()

unsigned long CComboCtrl::OnBtnDropDown()
{
	int pageCount = ( m_countDropDown > m_itemVec.size() ) ? m_itemVec.size() : m_countDropDown;

	//需要弹出Item必须一个以上
	if( pageCount <= 0 ) return KEY_VIRTUAL_MSG_CONTINUE;

	unsigned long x = m_rect.m_left,y=m_rect.m_bottom+2;
	unsigned long cx = m_rect.Width();
	unsigned long cy = m_length[COMBO_HEIGHT]*pageCount+5;

	//调整窗口位置
	//如果超出顶层窗口高度和宽度
	GUI::CWnd* pWndTop = GetTopestWnd();
	if( pWndTop != NULL )
	{
		GUI::CRect rcTop = pWndTop->GetWindowRect();

		//2010-11-24 16:42 原计算方式在处于窗口顶部父窗口比较小的情况下,会产生溢出
		//上下边距,控件处于可见区域必定处于窗口之内
		unsigned long yTopOff = m_rect.m_top - rcTop.m_top - 2;
		unsigned long yBottomOff = rcTop.m_bottom - m_rect.m_bottom - 2;

		if(cy > yBottomOff)
		{
			if(cy > yTopOff)
			{
				//上下都不满足,由屏幕来决定
				if(m_rect.m_bottom + cy + 2 < m_height)
				{
					//向下弹出
					y = m_rect.m_bottom + 2;
				}
				else
				{
					//向上弹出
					y = m_rect.m_top - cy - 2;
				}
			}
			else
			{
				//向上弹出
				y = m_rect.m_top - cy - 2;
			}
		}
		//x方向不需要检测,能创建控件,说明x方向上必然满足
	}

	m_popupMenu.SetCurSel(GetCurItem(),false);
	m_popupMenu.SetPageCount(pageCount);
	m_popupMenu.SetItemVector(m_itemVec);
	if( DLG_OK == m_popupMenu.DoModal(this, x, y, cx, cy) )
	{
		int sel = m_popupMenu.GetCurSel();
		SetCurItem(sel, true);
	}

	return KEY_VIRTUAL_MSG_CONTINUE;
}

unsigned long CComboCtrl::OnAdd()
{
	int item = m_curItem - 1;
	if( item < 0 )
	{
		item = m_itemVec.size() - 1;
	}

	SetCurItem(item,true);

	return KEY_VIRTUAL_MSG_CONTINUE;
}


unsigned long CComboCtrl::OnDec()
{
	int item = m_curItem + 1;
	if( item >= m_itemVec.size() )
	{
		item = 0;
	}
	SetCurItem(item,true);

	return KEY_VIRTUAL_MSG_CONTINUE;
}

//////////////////////////////////////////////////////////////////////////
// 调试输出
//////////////////////////////////////////////////////////////////////////

void CComboCtrl::Dump()
{
	printf("Combo id = %d.\n", GetID());

	for ( int i = 0; i < m_itemVec.size(); i++ )
	{
		printf("Item=%d,str=%s,data=0x%X.  ", m_itemVec[i].item, m_itemVec[i].str.c_str(), m_itemVec[i].data);

		if( i !=0 && (i%5 == 0) )
		{
			printf("\n");
		}
	}
	printf("\n");
}
