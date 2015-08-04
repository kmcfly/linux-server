
#include "MenuEx.h"

using namespace GUI;

//////////////////////////////////////////////////////////////////////////
CMenuExItem::ITEM_STATE& CMenuExItem::ITEM_STATE::operator =(const CMenuExItem::ITEM_STATE& item)
{
	item_state = item.item_state;
	paint_status = item.paint_status;
	bmp_id = item.bmp_id;

	return *this;
}

CMenuExItem::CMenuExItem() : m_itemId(INVALID_ID),
m_menuType(0),
m_itemType(0),
m_curState(-1),
m_tailBmpID(BMP_MENU_ITEM_ARROW),
m_itemState(MITEM_NORMAL)
{

}

CMenuExItem::~CMenuExItem()
{
	m_bmpStateVec.clear();
	m_paintStatusMap.clear();
}

bool CMenuExItem::Create(unsigned long ctrlID, CWnd* pParent, unsigned long style, int xFocus, int yFocus)
{
	if (CWnd::Create(ctrlID, 0, 0, ITEM_HEIGHT, ITEM_HEIGHT, pParent, style, xFocus, yFocus))
	{
		return true;
	}
	else
	{
		return false;
	}
}

void CMenuExItem::OnInitial()
{
	GUI::CWnd::OnInitial();

	m_itemState &= (~MITEM_IS_POPUP);

	int image_cy = 0;
	GetImageSize(BMP_MENU_ITEM_BG_F, m_bmpBgWidth, image_cy);
}

void CMenuExItem::OnPaint(PAINT_STATUS status)
{
	if((PAINT_STATUS_FOCUS == status) || (m_itemState & MITEM_IS_POPUP))
	{
		//GUI::CWnd::DrawRect(m_rect.m_left, m_rect.m_top, m_rect.Width(), m_rect.Height(), COLOR_SELECT);
		//GUI::CWnd::DrawLine(m_rect.m_left, m_rect.m_top, m_rect.m_right, m_rect.m_top, COLOR_DLG_CHILD_BK);
		//GUI::CWnd::DrawLine(m_rect.m_left, m_rect.m_bottom, m_rect.m_right, m_rect.m_bottom, COLOR_DLG_CHILD_BK);
		CDrawManager drawMan;
		const unsigned char* pData = drawMan.Stretch(BMP_MENU_ITEM_BG_F, m_rect.Width(), m_rect.Height(), m_bmpBgWidth / 2, CDrawManager::INVLAID, NULL, true);
		int len = drawMan.GetDataLen();

		if (NULL != pData)
		{
			GUI::CWnd::DrawImage(pData, len, m_rect.m_left, m_rect.m_top, m_rect.Width(), m_rect.Width(), m_rect.Height());
		}
	}
	else
	{
		GUI::CWnd::DrawRect(m_rect.m_left, m_rect.m_top, m_rect.Width(), m_rect.Height(), COLOR_MENU_BK);
	}

	unsigned long x = m_rect.m_left;
	unsigned long y = m_rect.m_top;

	unsigned long x1 = x, x2 = 0, x3 = 0;
	unsigned long cx = 0;

	//����λ��
	if(m_menuType & MENUEX_IMAGE)
	{
		x1 = x + ITEM_OFFSET;
		x2 = x + ITEM_IMAGE_WIDTH + 2 * ITEM_OFFSET;
		x3 = x + m_rect.Width() - ITEM_OFFSET - ITEM_TAIL_WIDTH;
		cx = m_rect.Width() - 3 * ITEM_OFFSET - ITEM_IMAGE_WIDTH;
		if(m_menuType & MENUEX_POPUP)
		{
			cx = cx -  ITEM_TAIL_WIDTH - ITEM_OFFSET;
		}
	}
	else
	{
		x1 = x + 2 * ITEM_OFFSET;
		x2 = x + 2 * ITEM_OFFSET;
		x3 = x + m_rect.Width() - ITEM_OFFSET - ITEM_TAIL_WIDTH;
		cx = m_rect.Width() - 4 * ITEM_OFFSET;
		if(m_menuType & MENUEX_POPUP)
		{
			cx = cx -  ITEM_TAIL_WIDTH;
		}
	}

	//����
	if(m_itemType & MENUEX_SEPARATOR)
	{
		//���Ʒָ���
		CDrawManager drawMan;
		const unsigned char* pData = drawMan.Stretch(BMP_MENU_SEPARATOR, m_rect.Width(), m_rect.Height(), m_bmpBgWidth / 2, CDrawManager::INVLAID, NULL, true);
		int len = drawMan.GetDataLen();

		if (NULL != pData)
		{
			GUI::CWnd::DrawImage(pData, len, m_rect.m_left, m_rect.m_top, m_rect.Width(), m_rect.Width(), m_rect.Height());
		}

		//GUI::CWnd::DrawLine(m_rect.m_left, m_rect.m_top + 2, m_rect.m_right, m_rect.m_top + 2, GUI_RGB(83, 87,95));
	}
	else
	{
		//������ͼƬ
		if((m_itemType & MENUEX_IMAGE) && !(m_itemState & MITEM_UN_ENABLE))
		{
			PAINT_STATUS itemStatus = status;
			if((PAINT_STATUS_FOCUS == status) || (m_itemState & MITEM_IS_POPUP))
			{
				itemStatus = PAINT_STATUS_FOCUS;
			}

			std::map<PAINT_STATUS, BITMAP_ID>::iterator iter;
			iter = m_paintStatusMap.find(itemStatus);
			if(iter != m_paintStatusMap.end())
			{
				BITMAP_ID bmpID = iter->second;
				GUI::CWnd::DrawImage(bmpID, x1, m_rect.m_top + (ITEM_HEIGHT - ITEM_IMAGE_HEIGHT) / 2);
			}
		}

		//��������
		if(m_itemType & MENUEX_STRING)
		{
			unsigned short start_x = x2;
			unsigned short start_y = y + (ITEM_HEIGHT - m_length[FONT_WIDTH]) / 2 - 1;

			if(m_itemState & MITEM_UN_ENABLE)
			{
				//��ʹ��,����
				GUI::CWnd::DrawText(GetCaption().c_str(), GetCaption().length(), start_x, start_y, cx, m_length[FONT_WIDTH], COLOR_DISABLE);
			}
			else
			{
				//ʹ��,����
				if(PAINT_STATUS_FOCUS == status)
				{
					start_x += 1;
					start_y += 1;
				}

				if(PAINT_STATUS_FOCUS == status)
				{
					GUI::CWnd::DrawText(GetCaption().c_str(), GetCaption().length(), start_x, start_y, cx, m_length[FONT_WIDTH], COLOR_TEXT_FOCUS);
				}
				else
				{
					GUI::CWnd::DrawText(GetCaption().c_str(), GetCaption().length(), start_x, start_y, cx, m_length[FONT_WIDTH]);
				}
			}
		}
	
		//����βͼƬ
		if((m_itemType & MENUEX_POPUP) && !(m_itemState & MITEM_UN_ENABLE))
		{
			BITMAP_ID arrowID = BMP_MENU_ITEM_ARROW;
			if((PAINT_STATUS_FOCUS == status) || (m_itemState & MITEM_IS_POPUP))
			{
				arrowID = BMP_MENU_ITEM_ARROW_F;
			}
			GUI::CWnd::DrawImage(arrowID, x3, m_rect.m_top + (ITEM_HEIGHT - ITEM_TAIL_HEIGHT) / 2);
		}
	}
}

void CMenuExItem::OnDestroy()
{
	GUI::CWnd::OnDestroy();
}

void CMenuExItem::SetItemId(unsigned long id)
{
	m_itemId = id;
}

void CMenuExItem::SetMenuType(unsigned long type)
{
	m_menuType = type;
}

void CMenuExItem::SetItemType(unsigned long type)
{
	m_itemType = type;
}

unsigned long CMenuExItem::GetMenuType()
{
	return m_menuType;
}

unsigned long CMenuExItem::GetItemType()
{
	return m_itemType;
}

unsigned long CMenuExItem::GetItemId()
{
	return m_itemId;
}

void CMenuExItem::SetStateImage(int uiState, PAINT_STATUS status, BITMAP_ID id)
{
	//�޸�����
	m_itemType |= MENUEX_IMAGE;

	ITEM_STATE itemState;
	itemState.item_state = uiState;
	itemState.paint_status = status;
	itemState.bmp_id = id;

	std::vector<ITEM_STATE>::iterator iter;
	//�Ȳ����Ƿ��д�״̬��ӹ�
	for ( iter = m_bmpStateVec.begin(); iter != m_bmpStateVec.end(); iter++)
	{
		if((iter->item_state == itemState.item_state) && (iter->paint_status == itemState.paint_status))
		{
			iter->bmp_id = itemState.bmp_id;
			return;
		}
	}

	//�����һ����״̬,ֱ�����
	m_bmpStateVec.push_back(itemState);
}

void CMenuExItem::SetTailImage(BITMAP_ID id)
{
	m_tailBmpID = id;
}

void CMenuExItem::SetCurState(int uiState, bool bRefresh)
{
	//�ų��ظ��������
	if(uiState == m_curState)
	{
		return;
	}

	m_curState = uiState;
	m_paintStatusMap.clear();

	std::vector<ITEM_STATE>::iterator iter;
	for (iter = m_bmpStateVec.begin(); iter != m_bmpStateVec.end(); iter++)
	{
		if( iter->item_state == m_curState )
		{
			m_paintStatusMap.insert(std::make_pair(iter->paint_status, iter->bmp_id));	
		}
	}

	if( bRefresh )
	{
		Repaint();
	}
}

int CMenuExItem::GetCurState()
{
	return m_curState;
}

void CMenuExItem::SetEnable(bool enable, bool bRefresh)
{
	/*
		�ı����CWnd��SetEnable�ĺ���
		�˵������ʹ��ָ�������ң�����Ӧ�û����룬�������Խ��ܽ���
	*/
	unsigned long state = GetItemState();
	if(enable)
	{
		state &= (~MITEM_UN_ENABLE);
	}
	else
	{
		state |= MITEM_UN_ENABLE;
	}

	SetItemState(state, bRefresh);
}

bool CMenuExItem::IsEnable()
{
	if(m_itemState & MITEM_UN_ENABLE)
	{
		return false;
	}
	else
	{
		return true;
	}
}

void CMenuExItem::SetVisible(bool visible)
{
	/*
		�ɼ������ɼ���Ӱ��˵�����
		�����ڵ����˵�ǰ����
	*/
	unsigned long state = GetItemState();
	if(visible)
	{
		state &= (~MITEM_UN_VISIBLE);
	}
	else
	{
		state |= MITEM_UN_VISIBLE;
	}

	SetItemState(state, false);
}

bool CMenuExItem::IsVisible()
{
	if(m_itemState & MITEM_UN_VISIBLE)
	{
		return false;
	}
	else
	{
		return true;
	}
}

void CMenuExItem::SetEnPopupEnter(bool enbale)
{
	/*
		���ÿ��Ե����Ӳ˵�������,������ӦEnter��Ϣ
	Ĭ�������, ���Ե����Ӳ˵������ǲ�����ӦEnter��Ϣ
	[Set Enable Popup Endter]
	*/
	unsigned long state = GetItemState();
	if(enbale)
	{
		state |= MITEM_EN_POPUP;
	}
	else
	{
		state &= (~MITEM_EN_POPUP);
	}

	SetItemState(state, false);
}

bool CMenuExItem::IsEnPopupEnter()
{
	if(m_itemState & MITEM_EN_POPUP)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void CMenuExItem::SetItemState(unsigned long state, bool bRefresh)
{
	m_itemState = state;

	if(bRefresh)
	{
		Repaint();
	}
}

unsigned long CMenuExItem::GetItemState()
{
	return m_itemState;
}

unsigned long CMenuExItem::GetItemWidth()
{
	//��ȡ��Item������Ҫ�Ŀ��,����������SetItemType
	gui_assert(m_menuType != 0, Dump());

	unsigned long cx =  ITEM_OFFSET;

	if(m_menuType & static_cast<unsigned long>(MENUEX_IMAGE))
	{
		cx += ITEM_IMAGE_WIDTH;
	}
	else
	{
		cx += ITEM_OFFSET;
	}

	if(m_menuType & static_cast<unsigned long>(MENUEX_STRING))
	{
		unsigned short width = 0, height = 0;
		GUI::CWnd::TextSize(GetCaption().c_str(), GetCaption().length(), width, height);

		cx += width;
		cx += 2 * ITEM_OFFSET;
	}

	if(m_menuType & static_cast<unsigned long>(MENUEX_POPUP))
	{
		cx += ITEM_TAIL_WIDTH;
		cx += ITEM_OFFSET;
	}
	else
	{
		cx += ITEM_OFFSET;
	}

	return cx;
}

unsigned long CMenuExItem::GetItemHeight()
{
	//��ȡ��Item������Ҫ�ĸ߶�,����������m_itemType
	gui_assert(m_itemType != 0, Dump());

	if(m_itemType & static_cast<unsigned long>(MENUEX_SEPARATOR))
	{
		return ITEM_SEPARATOR;
	}
	else if(m_itemType & static_cast<unsigned long>(MENUEX_NORMAL))
	{
		return ITEM_HEIGHT;
	}
	else
	{
		gui_assert(false, Dump());
		return 0;
	}
}

bool CMenuExItem::IsPopupSubMenu(bool popup)
{
	/*
		�����Ƿ���Ҫ�����������Ӳ˵�
	*/

	if(m_itemState & MITEM_IS_POPUP)
	{
		if(popup)
		{
			return false;
		}
		else
		{
			return true;
		}
	}
	else
	{
		if(popup)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
}

//////////////////////////////////////////////////////////////////////////

CMenuEx::CMenuEx() : m_minWidth(100),
m_retValue(INVALID_ID),
m_posLeft(0),
m_posTop(0)
{
}

CMenuEx::~CMenuEx()
{
	Clear();
}

void CMenuEx::OnInitial()
{
	CDialog::OnInitial();
	m_style |= WND_NO_TITLE;

	////
	GetImageSize(BMP_MENU_BG, m_bmpBgWidth, m_bmpBgHeight);

	////
	int yFocus = 0;
	MENUEX_ITEM_LIST::iterator iter = m_itemList.begin();
	for (iter = m_itemList.begin(); iter != m_itemList.end(); iter++)
	{
		unsigned long style = WND_CHILD | WND_NO_IMAGE_BK;
		if((*iter)->GetItemType() & MENUEX_SEPARATOR)
		{
			style |= WND_DISABLE_FOCUS;
		}
		(*iter)->Create(GetRandWndID(), this, style, 0, yFocus++);
	}

	yFocus = 0;
	MENUEX_POPUP_WND::iterator iterWnd;
	for (iterWnd = m_popupWndMap.begin(); iterWnd != m_popupWndMap.end(); iterWnd++)
	{
		CMenuEx* pSubMenu = iterWnd->second;
		if(pSubMenu)
		{
			int xFocus = 1;
			CMenuExItem* pItem = GetItem(iterWnd->first, this);
			if(pItem)
			{
				pItem->GetFocusPos(xFocus, yFocus);
			}
			else
			{
				yFocus++;
			}
			pSubMenu->Create(GetRandWndID(), 0, 0, m_rect.Width(), m_rect.Height(), this, 1, yFocus, false);
		}
		else
		{
			gui_assert(false, printf("item id = %d.\n", iterWnd->first));
			gui_assert(false, Dump());
		}
	}

	AutoLayout();
}

void CMenuEx::OnDestroy()
{
	CDialog::OnDestroy();
}

void CMenuEx::OnPaint(PAINT_STATUS status)
{
	//GUI::CWnd::DrawRect(m_rectMenu.m_left, m_rectMenu.m_top, m_rectMenu.Width(), m_rectMenu.Height(), COLOR_DLG_CHILD_BK);
	//GUI::CWnd::DrawRectBorder(m_rectMenu.m_left, m_rectMenu.m_top, m_rectMenu.Width(), m_rectMenu.Height(), COLOR_SELECT);

	CDrawManager drawMan;
	const unsigned char* pData = drawMan.Stretch(BMP_MENU_BG, m_rectMenu.Width(), m_rectMenu.Height(), m_bmpBgWidth / 2, m_bmpBgHeight / 2, NULL, true);
	int len = drawMan.GetDataLen();

	if (NULL != pData)
	{
		GUI::CWnd::DrawImage(pData, len, m_rectMenu.m_left, m_rectMenu.m_top, m_rectMenu.Width(), m_rectMenu.Width(), m_rectMenu.Height());
	}
}

void CMenuEx::OnChangeFocus(unsigned long oldID, unsigned long newID)
{
	/*
		����˵����Popup״̬
	*/

	CMenuExItem* pItem = GetItemByWndId(newID, this);
	if(pItem)
	{
		CMenuEx* pOldSubMenu = NULL;
		CMenuEx* pNewSubMenu = NULL;

		MENUEX_ITEM_LIST::iterator iter;
		for(iter = m_itemList.begin(); iter != m_itemList.end(); iter++)
		{
			MENUEX_POPUP_WND::iterator iterWnd = m_popupWndMap.find((*iter)->GetItemId());
			if(((*iter)->IsEnable()) && (iterWnd != m_popupWndMap.end()))
			{
				if((*iter)->GetID() == newID )
				{
					if((*iter)->IsPopupSubMenu(true))
					{
						unsigned long itemState = (*iter)->GetItemState() | MITEM_IS_POPUP;

						if((*iter)->IsPopupSubMenu(true) && (!iterWnd->second->IsShow()))
						{
							pNewSubMenu = iterWnd->second;
						}

						(*iter)->SetItemState(itemState, true);
					}
				}
				else
				{
					if((*iter)->IsPopupSubMenu(false) )
					{
						unsigned long itemState = (*iter)->GetItemState() & (~MITEM_IS_POPUP);

						if((*iter)->IsPopupSubMenu(false) && iterWnd->second->IsShow())
						{
							pOldSubMenu = iterWnd->second;
						}

						(*iter)->SetItemState(itemState, true);
					}
				}
			}
		}
#if 0
		GUI::CRect rect1, rect2;
		GUI::CRect* pRect1 = NULL;
		GUI::CRect* pRect2 = NULL;

		if(pOldSubMenu)
		{
			pOldSubMenu->Show(false, false);
			rect1 = pOldSubMenu->GetMenuRect();
			pRect1 = &rect1;
		}

		if(pNewSubMenu)
		{
			//ResetPopupState(pNewSubMenu);
			pNewSubMenu->Show(true, false);
			rect2 = pNewSubMenu->GetMenuRect();
			pRect2 = &rect2;
		}

		if(pOldSubMenu || pNewSubMenu)
		{
			RefreshMenu(pRect1, pRect2);
		}
#else
		if(pOldSubMenu)
		{
			pOldSubMenu->Show(false, false);
		}

		if(pNewSubMenu)
		{
			pNewSubMenu->Show(true, false);
		}

		if(pOldSubMenu || pNewSubMenu)
		{
			GetRootWnd()->Repaint();
		}
#endif
	}

	GUI::CDialog::OnChangeFocus(oldID, newID);
}

unsigned long CMenuEx::Popup(unsigned long x, unsigned long y)
{
	/*
		�����˵�
	*/
	m_posLeft = x;
	m_posTop = y;

	unsigned long ret = INVALID_ID;
	if(DLG_OK == GUI::CDialog::DoModal(NULL, CENTER_POS, CENTER_POS, MAX_SCREEN_X, MAX_SCREEN_Y))
	{
		ret = GetRetValue();
	}

	//gui_trace_np(INVALID_ID != ret, printf("Enter menu item. id = 0x%.8X, str = \"%s\".\n", ret, GetItem(ret)->GetCaption().c_str()));
	return ret;
}

CMenuExItem* CMenuEx::AddItem(const char* pStr, unsigned long itemID, CMenuEx* pPopupWnd)
{
	/*
		��Ӳ˵���, ��������ָ�������������ľ�����Ϣ
	����:	pStr		ΪNULL,��ʾ��ӷָ��; ��NULL,��ʾ�˵���ı��⴮
			itemID		�����ʾID. ΪINVALID_IDʱ�޷�����
			pPopupWnd	������Ӳ˵�
	*/

#ifndef NDEBUG
	//����������ظ�ID�Ĳ˵���Ŀ
	if(INVALID_ID != itemID && GetItem(itemID))
	{
		gui_assert(false, Dump());
	}
#endif

	CMenuExItem* pItem = new CMenuExItem();

	if(NULL == pStr)
	{
		//��ӷָ��
		gui_assert(NULL == pPopupWnd, Dump());

		pItem->SetItemId(INVALID_ID);
		pItem->SetItemType(MENUEX_SEPARATOR);
	}
	else
	{
		//�����ͨ�˵���
		unsigned long itemType = MENUEX_NORMAL | MENUEX_STRING;
		if(pPopupWnd)
		{
			itemType |= MENUEX_POPUP;
			m_popupWndMap.insert(std::make_pair(itemID, pPopupWnd));
		}

		pItem->SetItemId(itemID);
		pItem->SetItemType(itemType);
		pItem->SetCaption(pStr, false);
	}

	m_itemList.push_back(pItem);

	return pItem;
}

CMenuExItem* CMenuEx::GetItem(unsigned long itemID, CMenuEx* pMenuEx)
{
	/*
		��ȡ�˵�����
	����: pMenuEx    NULL: �������˵��Լ����е����Ӳ˵�
					 ��NULL: ������pMenuEx�˵�
	*/

	if(INVALID_ID == itemID)
	{
		return NULL;
	}

	if(pMenuEx)
	{
		MENUEX_ITEM_LIST::iterator iter;
		for (iter = pMenuEx->m_itemList.begin(); iter != pMenuEx->m_itemList.end(); iter++)
		{
			if((*iter)->GetItemId() == itemID)
			{
				return *iter;
			}
		}
	}
	else
	{
		//���������˵�
		MENUEX_ITEM_LIST::iterator iter;
		for (iter = m_itemList.begin(); iter != m_itemList.end(); iter++)
		{
			if((*iter)->GetItemId() == itemID)
			{
				return *iter;
			}
		}

		//�����������˵�
		MENUEX_POPUP_WND::iterator iterWnd;
		for (iterWnd = m_popupWndMap.begin(); iterWnd != m_popupWndMap.end(); iterWnd++)
		{
			CMenuEx* pSubMenuEx = iterWnd->second;
			if(pSubMenuEx)
			{
				for (iter = pSubMenuEx->m_itemList.begin(); iter != pSubMenuEx->m_itemList.end(); iter++)
				{
					if((*iter)->GetItemId() == itemID)
					{
						return *iter;
					}
				}
			}
			else
			{
				gui_trace_np(true, printf("(%s.%d) item id = 0x.8X.\n", __FILE__, __LINE__, itemID));
				gui_assert(false, Dump());
			}
		}
	}

	return NULL;
}

CMenuExItem* CMenuEx::GetItemByWndId(unsigned long wndId, CMenuEx* pMenuEx)
{
	/*
		��ȡ�˵�����
	����: pMenuEx    NULL: �������˵��Լ����е����Ӳ˵�
	��NULL: ������pMenuEx�˵�
	*/

	if(INVALID_ID == wndId)
	{
		return NULL;
	}

	if(pMenuEx)
	{
		MENUEX_ITEM_LIST::iterator iter;
		for (iter = pMenuEx->m_itemList.begin(); iter != pMenuEx->m_itemList.end(); iter++)
		{
			if((*iter)->GetID() == wndId)
			{
				return *iter;
			}
		}
	}
	else
	{
		//���������˵�
		MENUEX_ITEM_LIST::iterator iter;
		for (iter = m_itemList.begin(); iter != m_itemList.end(); iter++)
		{
			if((*iter)->GetID() == wndId)
			{
				return *iter;
			}
		}

		//�����������˵�
		MENUEX_POPUP_WND::iterator iterWnd;
		for (iterWnd = m_popupWndMap.begin(); iterWnd != m_popupWndMap.end(); iterWnd++)
		{
			CMenuEx* pSubMenuEx = iterWnd->second;
			if(pSubMenuEx)
			{
				for (iter = pSubMenuEx->m_itemList.begin(); iter != pSubMenuEx->m_itemList.end(); iter++)
				{
					if((*iter)->GetID() == wndId)
					{
						return *iter;
					}
				}
			}
			else
			{
				gui_trace_np(true, printf("(%s.%d) wnd id = 0x.8X.\n", __FILE__, __LINE__, wndId));
				gui_assert(false, Dump());
			}
		}
	}

	return NULL;
}

void CMenuEx::SetItemTailImage(unsigned long itemID, BITMAP_ID id)
{
	CMenuExItem* pItem = GetItem(itemID);
	if(pItem)
	{
		pItem->SetTailImage(id);
	}
}

void CMenuEx::SetItemStateImage(unsigned long itemID, int uiState, PAINT_STATUS status, BITMAP_ID id)
{
	CMenuExItem* pItem = GetItem(itemID);
	if(pItem)
	{
		pItem->SetStateImage(uiState, status, id);
	}
}

void CMenuEx::SetItemCurState(unsigned long itemID, int uiState, bool bRefresh)
{
	CMenuExItem* pItem = GetItem(itemID);
	if(pItem)
	{
		pItem->SetCurState(uiState, bRefresh);
	}
}

void CMenuEx::SetItemEnable(unsigned long itemID, bool enable , bool bRefresh)
{
	CMenuExItem* pItem = GetItem(itemID);
	if(pItem)
	{
		pItem->SetEnable(enable, bRefresh);
	}
}

void CMenuEx::SetItemVisible(unsigned long itemID, bool visible )
{
	CMenuExItem* pItem = GetItem(itemID);
	if(pItem)
	{
		pItem->SetVisible(visible);
	}
}

void CMenuEx::SetItemEnPopupEnter(unsigned long itemID, bool enable)
{
	CMenuExItem* pItem = GetItem(itemID);
	if(pItem)
	{
		pItem->SetEnPopupEnter(enable);
	}
}

int CMenuEx::GetItemCurState(unsigned long itemID)
{
	CMenuExItem* pItem = GetItem(itemID);
	if(pItem)
	{
		return pItem->GetCurState();
	}

	gui_assert(INVALID_ID != itemID, Dump());
	return -1;
}

bool CMenuEx::GetItemEnable(unsigned long itemID)
{
	CMenuExItem* pItem = GetItem(itemID);
	if(pItem)
	{
		return pItem->IsEnable();
	}
	
	gui_assert(INVALID_ID != itemID, Dump());
	return false;
}

bool CMenuEx::GetItemVisible(unsigned long itemID)
{
	CMenuExItem* pItem = GetItem(itemID);
	if(pItem)
	{
		return pItem->IsVisible();
	}
	
	gui_assert(INVALID_ID != itemID, Dump());
	return false;
}

bool CMenuEx::GetItemEnPopupEnter(unsigned long itemID)
{
	CMenuExItem* pItem = GetItem(itemID);
	if(pItem)
	{
		return pItem->IsEnPopupEnter();
	}

	gui_assert(INVALID_ID != itemID, Dump());
	return false;
}

void CMenuEx::Clear()
{
	//������в˵���Ŀ,�����Ӳ˵�

	MENUEX_ITEM_LIST::iterator iter;
	for (iter = m_itemList.begin(); iter != m_itemList.end(); iter++)
	{
		delete (*iter);
	}
	m_itemList.clear();

	MENUEX_POPUP_WND::iterator iterWnd;
	for (iterWnd = m_popupWndMap.begin(); iterWnd != m_popupWndMap.end(); iterWnd++)
	{
		delete iterWnd->second;
	}
	m_popupWndMap.clear();
}

void CMenuEx::AutoLayout()
{
	/*
		�Զ����ִ���Ԫ��
		�޷�����Ԫ�����������³�����Ļ��ȸ߶�����.
	*/
	unsigned long width = m_minWidth;
	unsigned long height = 0;
	unsigned long menuType = 0;

	//����˵�����
	MENUEX_ITEM_LIST::iterator iter;
	for (iter = m_itemList.begin(); iter != m_itemList.end(); iter++)
	{
		menuType |= (*iter)->GetItemType();
	}

	//����˵�����Ŀ�Ⱥ͸߶�,�������߿�
	for (iter = m_itemList.begin(); iter != m_itemList.end(); iter++)
	{
		//���ò˵�����
		(*iter)->SetMenuType(menuType);

		//������߶�,�����ڲ˵�����
		unsigned long w = (*iter)->GetItemWidth();
		if(w > width)
		{
			width = w;
		}

		unsigned long h = (*iter)->GetItemHeight();
		height += h;
	}

	//����λ��,���ⳬ����Ļ
	//������
	if(m_posLeft > m_width || m_posTop > m_height)
	{
		m_posLeft = (m_width - width - MENUEX_BORDER_LEFT - MENUEX_BORDER_RIGHT) / 2;
		m_posTop = (m_height - height - MENUEX_BORDER_TOP - MENUEX_BORDER_BOTTOM) / 2;
	}

	//��ȳ���
	if((m_posLeft + width + MENUEX_BORDER_LEFT + MENUEX_BORDER_RIGHT) > m_width)
	{
		if(m_width > (width + MENUEX_BORDER_LEFT + MENUEX_BORDER_RIGHT))
		{
			m_posLeft = m_width - width - MENUEX_BORDER_LEFT - MENUEX_BORDER_RIGHT;
		}
		else
		{
			//�˵���ȹ���,Ӧ������
			gui_assert(false, Dump());
			m_posLeft = 0;
			width = m_width - 2;
		}
	}

	//�߶ȳ���
	if((m_posTop + height + MENUEX_BORDER_TOP + MENUEX_BORDER_BOTTOM) > m_height)
	{
		if(m_height > (height + MENUEX_BORDER_TOP + MENUEX_BORDER_BOTTOM))
		{
			m_posTop = m_height - height - MENUEX_BORDER_TOP - MENUEX_BORDER_BOTTOM;
		}
		else
		{
			//�˵��߶ȹ���,Ӧ������
			gui_assert(false, Dump());
			m_posTop = 0;
			height = m_height - 2;
		}
	}
	unsigned long x = m_posLeft + MENUEX_BORDER_LEFT, y = m_posTop + MENUEX_BORDER_TOP;


	//���þ�������λ��
	for (iter = m_itemList.begin(); iter != m_itemList.end(); iter++)
	{
		if(!(*iter)->IsVisible())
		{
			continue;
		}

		//������߶�
		unsigned long h = (*iter)->GetItemHeight();
		(*iter)->SetWndPos(x, y, width, h);

		if(!(*iter)->IsShow())
		{
			(*iter)->Show(true, false);
		}

		//�����Ӵ���
		MENUEX_POPUP_WND::iterator iterWnd = m_popupWndMap.find((*iter)->GetItemId());
		if(iterWnd != m_popupWndMap.end())
		{
			GUI::CRect rect = iterWnd->second->GetMenuRect();
			int sub_left = m_posLeft + width + MENUEX_BORDER_LEFT + MENUEX_BORDER_RIGHT, sub_top = y;
			//�����Ӳ˵�λ��
			if(sub_left + rect.Width() > m_width)
			{
				if(m_posLeft > rect.Width())
				{
					sub_left = m_posLeft - rect.Width();
				}
				else
				{
					gui_assert(false, Dump()); //�˵���ȹ���
					sub_left = 0;
				}
			}
			if(sub_top + rect.Height() + 2 > m_height)
			{
				if((m_rect.Height() - 2) > rect.Height())
				{
					sub_top = m_rect.Height() - rect.Height() - 2;
				}
				else
				{
					gui_assert(false, Dump()); //�˵��߶ȹ���
					sub_top = 0;
				}
			}
			
			rect.SetRect(sub_left, sub_top, sub_left + rect.Width(), sub_top + rect.Height());
			iterWnd->second->MoveWnd(sub_left, sub_top);
			iterWnd->second->SetWndPos(rect);
			iterWnd->second->SetMenuRect(rect);
		}

		y += h;
	}

	//���ò˵�ʵ��ʹ�õ�λ��
	SetMenuRect(GUI::CRect(m_posLeft, m_posTop, m_posLeft + width + MENUEX_BORDER_LEFT + MENUEX_BORDER_RIGHT, y + MENUEX_BORDER_BOTTOM));
}

void CMenuEx::SetMenuRect(const GUI::CRect& rect)
{
	m_rectMenu = rect;
}

GUI::CRect CMenuEx::GetMenuRect()
{
	return m_rectMenu;
}

void CMenuEx::SetRetValue(unsigned long value)
{
	m_retValue = value;
}

unsigned long CMenuEx::GetRetValue()
{
	return m_retValue;
}

void CMenuEx::RefreshMenu(const GUI::CRect* pRect1, const GUI::CRect* pRect2)
{
	/*
		�ֶ�ˢ�²˵�����[�ӻ�������Ļ]
	*/
	GUI::CRect rect = m_rectMenu;
	
	if(pRect1)
	{
		rect = rect.UnionRect(*pRect1);
	}

	if(pRect2)
	{
		rect = rect.UnionRect(*pRect2);
	}

	GUI::CWnd::Refresh(rect.m_left, rect.m_top, rect.Width(), rect.Height());
}

BEGIN_MSG_MAP(GUI::CMenuEx, GUI::CDialog)
	ON_MSG_EX(CTRLID_NULL, KEY_VIRTUAL_ENTER, OnLClick)
END_MSG_MAP()

unsigned long CMenuEx::OnLClick(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	/*
		���������������Enter
	*/
	CMenuEx* pTopWnd = dynamic_cast<CMenuEx*>(GetTopestWnd());


	if ((INVALID_INDEX(unsigned long) == xPos) && (INVALID_INDEX(unsigned long) == yPos)|| (KEY_MY1003_POS == xPos) && (KEY_MY1003_POS == yPos))
	{	
		//�����������1003K��������
		CMenuExItem* pItem = dynamic_cast<CMenuExItem*>(GetActiveCtrl());
		if(pItem && pItem->IsEnable() && (!(pItem->GetItemType() & MENUEX_POPUP) || pItem->IsEnPopupEnter()))
		{
			pTopWnd->SetRetValue(pItem->GetItemId());
			pTopWnd->EndDialog(DLG_OK);
			return KEY_VIRTUAL_MSG_STOP;
		}
	}
	else
	{
		//������
		if(m_rectMenu.PtInRect(xPos, yPos))
		{
			CMenuExItem* pItem = GetItemByWndId(ctrlID, this);
			if(pItem && pItem->IsEnable() && (!(pItem->GetItemType() & MENUEX_POPUP) || pItem->IsEnPopupEnter()))
			{
				pTopWnd->SetRetValue(pItem->GetItemId());
				pTopWnd->EndDialog(DLG_OK);
				return KEY_VIRTUAL_MSG_STOP;
			}
		}
		else
		{
			//�˳�
			pTopWnd->EndDialog(DLG_CANCEL);
			return KEY_VIRTUAL_MSG_STOP;
		}
	}

	return KEY_VIRTUAL_MSG_CONTINUE;
}

//////////////////////////////////////////////////////////////////////////

void CMenuEx::Dump()
{
	printf("========>>>>>>>>>(file=%s.line=%d)\n", __FILE__, __LINE__);
	printf("class name = \"CMenuEx\". id = %d.\n", GetID());
	printf("menu rect = (%d, %d, %d, %d)\n", m_rectMenu.m_left, m_rectMenu.m_top, m_rectMenu.m_right, m_rectMenu.m_bottom);
	printf("ret value = 0x%.8X\n", m_retValue);
	printf("items = menu type, itme type, item id, ui state, item state, string.\n");
	
	int index = 0;
	MENUEX_ITEM_LIST::iterator iter;
	for (iter = m_itemList.begin(); iter != m_itemList.end(); iter++)
	{
		CMenuExItem* pItem = (*iter);
		printf("%d = 0x%.8X, 0x%.8X, 0x%.8X, %.4d, 0x%.8X, str = %s.\n", 
				index++, pItem->GetMenuType(), pItem->GetItemType(), pItem->GetItemId(), pItem->GetCurState(),
				pItem->GetItemState(), pItem->GetCaption().c_str());
	}
	printf("========>>>>>>>>>(end)\n");
}

//end

