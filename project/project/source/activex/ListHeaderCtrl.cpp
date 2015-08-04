#include "ListHeaderCtrl.h"
#include "DrawManager.h"

using namespace GUI;

CListHeaderCtrl::CListHeaderCtrl()
{

}

CListHeaderCtrl::~CListHeaderCtrl()
{

}

void CListHeaderCtrl::OnDestroy()
{
	GUI::CWnd::OnDestroy();

	m_itemList.clear();
}

int CListHeaderCtrl::AddItem(const char* pStr, int width)
{
	assert(NULL != pStr);
	HEADER_ITEM headerItem;
	headerItem.item = m_itemList.size();
	headerItem.width = width;
	headerItem.str = pStr;

	m_itemList.push_back(headerItem);
	return headerItem.item;
}

int CListHeaderCtrl::GetItemWidth(int item) const
{
	if (IsValidItem(item))
	{
		return m_itemList[item].width;
	} 
	else
	{
		return 0;
	}
}

bool CListHeaderCtrl::IsValidItem(int item) const
{
	return ((item >=0) && (item < m_itemList.size()));
}

void CListHeaderCtrl::OnPaint(PAINT_STATUS status)
{
//	GUI::CWnd::DrawRect(m_rect.m_left, m_rect.m_top, m_rect.Width(), m_rect.Height(), GUI_RGB(100, 100, 100));
//	GUI::CWnd::DrawRectBorder(m_rect.m_left, m_rect.m_top, m_rect.Width(), m_rect.Height(), GUI_RGB(192, 192, 192), 2);
	CDrawManager drawMan;
	int len = 0;
	const unsigned char* pData = drawMan.Stretch(BMP_LIST_HEADER, m_rect.Width(), m_rect.Height(), 0, CDrawManager::INVLAID, &len, true);
	if (NULL != pData)
	{
		GUI::CWnd::DrawImage(pData, len, m_rect.m_left, m_rect.m_top, m_rect.Width(), m_rect.Width(), m_rect.Height());
	}	

	int x = m_rect.m_left;
	HEADER_ITEM_VECTOR::const_iterator iter;
	for (iter = m_itemList.begin(); iter != m_itemList.end(); ++iter)
	{
		const HEADER_ITEM& headerItem = *iter;
		//标题
		if (headerItem.str.length() > 0)
		{
			GUI::CWnd::DrawText(headerItem.str.c_str(), headerItem.str.length(), x+m_length[EDIT_XPOS], m_rect.m_top+m_length[EDIT_YPOS], headerItem.width- 2 * m_length[EDIT_XPOS], m_length[LIST_HEAD_HEIGHT]-m_length[EDIT_YPOS]);
		}
		
		x += headerItem.width;

		//边框
		if (iter != m_itemList.end() - 1)
		{	
			GUI::CWnd::DrawLine(x, m_rect.m_top, x, m_rect.m_bottom + 1, COLOR_LINE_GRAY);
		}
	}

	//两边的竖条
	GUI::CWnd::DrawLine(m_rect.m_left, m_rect.m_top, m_rect.m_left, m_rect.m_bottom, COLOR_LINE_GRAY, 2);
	GUI::CWnd::DrawLine(m_rect.m_right-1, m_rect.m_top, m_rect.m_right-1, m_rect.m_bottom, COLOR_LINE_GRAY, 2);
}

