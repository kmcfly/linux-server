#include "TabCtrl.h"
#include "CtrlID.h"
#include "DrawManager.h"

using namespace GUI;

//Class CTabButton
GUI::CTabButton::CTabButton()
{
	m_ctrlType = GUI::WND_TYPE_TAB;
}

GUI::CTabButton::~CTabButton()
{

}

void GUI::CTabButton::OnPaint(PAINT_STATUS status)
{
	if (WND_NO_IMAGE_BK & m_style)
	{
		if (m_bCheck)
		{
			GUI::CWnd::DrawRect(m_rect.m_left, m_rect.m_top, m_rect.Width(), m_rect.Height(), GUI_RGB(100, 100, 100));
			GUI::CWnd::DrawLine(m_rect.m_left, m_rect.m_top, m_rect.m_left + m_rect.Width() - 1, m_rect.m_top, GUI_RGB(255, 255, 255));
			GUI::CWnd::DrawLine(m_rect.m_left, m_rect.m_top, m_rect.m_left, m_rect.m_top + m_rect.Height() - 1, GUI_RGB(192, 192, 192), 2);
			GUI::CWnd::DrawLine(m_rect.m_left + m_rect.Width() - 2, m_rect.m_top, m_rect.m_left + m_rect.Width() - 2, m_rect.m_top + m_rect.Height() - 1, GUI_RGB(192, 192, 192), 2);
		}
		else
		{
			GUI::CWnd::DrawRect(m_rect.m_left, m_rect.m_top, m_rect.Width(), m_rect.Height(), GUI_RGB(60, 60, 60));
			GUI::CWnd::DrawLine(m_rect.m_left, m_rect.m_top, m_rect.m_left + m_rect.Width() - 1, m_rect.m_top, GUI_RGB(128, 128, 128));
			GUI::CWnd::DrawLine(m_rect.m_left, m_rect.m_top, m_rect.m_left, m_rect.m_top + m_rect.Height() - 1, GUI_RGB(192, 192, 192), 2);
			GUI::CWnd::DrawLine(m_rect.m_left + m_rect.Width() - 2, m_rect.m_top, m_rect.m_left + m_rect.Width() - 2, m_rect.m_top + m_rect.Height() - 1, GUI_RGB(192, 192, 192), 2);
		}
	}
	else
	{
		BITMAP_ID bmpID = BMP_INVALID;
		if (m_bCheck)
		{
			switch (status)
			{
			case PAINT_STATUS_FOCUS:
				bmpID = BMP_TABBUTTON_CHECK_FOCUS;
				break;
			case PAINT_STATUS_DISABLE:
				bmpID = BMP_TABBUTTON_CHECK_DISABLE;
				break;
			default:
				bmpID = BMP_TABBUTTON_CHECK;
				break;
			}
		} 
		else
		{
			switch (status)
			{
			case PAINT_STATUS_FOCUS:
				bmpID = BMP_TABBUTTON_FOCUS;
				break;
			case PAINT_STATUS_DISABLE:
				bmpID = BMP_TABBUTTON_DISABLE;
				break;
			default:
				bmpID = BMP_TABBUTTON;
				break;
			}
		}

		CDrawManager drawMan;
		int len = 0;
		const unsigned char* pData = drawMan.Stretch(bmpID, m_rect.Width(), m_rect.Height(), 6, CDrawManager::INVLAID, &len, true);
		if (NULL != pData)
		{
			GUI::CWnd::DrawImage(pData, len, m_rect.m_left, m_rect.m_top, m_rect.Width(), m_rect.Width(), m_rect.Height());
		}		
	}

	if (m_strCaption.length() > 0)
	{
		assert (PAINT_STATUS_NULL != status);

		unsigned short cx = 0, cy = 0;
		GUI::CWnd::TextSize(m_strCaption.c_str(), m_strCaption.length(), cx, cy);

		char strDot[32] = "...";
		unsigned short dot_cx = 0, dot_cy = 0;
		GUI::CWnd::TextSize(strDot, strlen(strDot), dot_cx, dot_cy);

		bool bDrawFull = true;
		unsigned short drawWidth = cx;

		if(((cx + 10) > m_rect.Width()) && ((dot_cx + 10) < m_rect.Width()))
		{
			bDrawFull = false;
			drawWidth = m_rect.Width() - 10 - dot_cx;
		}

		if (PAINT_STATUS_NORMAL == status)
		{
			GUI::CWnd::DrawText(m_strCaption.c_str(), m_strCaption.length(), m_rect.m_left+5, m_rect.m_top+5, drawWidth, m_rect.Height()-2);
		
			if(!bDrawFull)
			{
				GUI::CWnd::DrawText(strDot, strlen(strDot), m_rect.m_left + 5 + drawWidth, m_rect.m_top + 5, dot_cx, m_rect.Height() - 2);
			}
		}
		else if (PAINT_STATUS_FOCUS == status)
		{
			GUI::CWnd::DrawText(m_strCaption.c_str(), m_strCaption.length(), m_rect.m_left+5, m_rect.m_top+5, drawWidth, m_rect.Height()-2, COLOR_TEXT_FOCUS);
		
			if(!bDrawFull)
			{
				GUI::CWnd::DrawText(strDot, strlen(strDot), m_rect.m_left + 5 + drawWidth, m_rect.m_top + 5, dot_cx, m_rect.Height() - 2, COLOR_TEXT_FOCUS);
			}
		}
		else
		{
			GUI::CWnd::DrawText(m_strCaption.c_str(), m_strCaption.length(), m_rect.m_left+5, m_rect.m_top+5, drawWidth, m_rect.Height()-2, COLOR_LINE_GRAY);
			
			if(!bDrawFull)
			{
				GUI::CWnd::DrawText(strDot, strlen(strDot), m_rect.m_left + 5 + drawWidth, m_rect.m_top + 5, dot_cx, m_rect.Height() - 2, COLOR_TEXT_FOCUS);
			}
		}
	}
}

void GUI::CTabButton::SetWindowPos(int x, int y, int cx, int cy)
{
	GUI::CWnd::SetWndPos(x, y, cx, cy);
}
//Class CTabCtrl
GUI::CTabCtrl::CTabCtrl() : m_itemCount(0)
, m_curItem(0)
, m_tabLen(FIRST_BTN_OFFSET)
, m_btnHeight(0)
{
	m_ctrlType = GUI::WND_TYPE_TAB;
}

GUI::CTabCtrl::~CTabCtrl()
{

}

bool GUI::CTabCtrl::Create(unsigned long ctrlID, 
						  unsigned long x, 
						  unsigned long y, 
						  unsigned long cx, 
						  unsigned long cy, 
						  CWnd* pParent, 
						  int xFocus, 
						  int yFocus)
{
	if (GUI::CWnd::Create(ctrlID, x, y, cx, cy, pParent, WND_CHILD | WND_VISIBLE | WND_NO_IMAGE_BK, xFocus, yFocus))
	{
		return true;
	}
	else
	{
		return false;
	}
}

void GUI::CTabCtrl::OnInitial()
{
	m_tabLen = m_length[DLG_OFFSET_SUB_LEFT];

	unsigned long ctrlID = CTRLID_TAB_BTN_FIRST;
	int xFocus = 0;
	int yFocus = 0;
	int iamge_cx = 0;
	GetImageSize(BMP_TABBUTTON, iamge_cx, m_btnHeight);
	CRect rect(0, 0, iamge_cx, m_btnHeight);
	for (int i = 0; i < MAX_TAB_ITEM; ++i)
	{
		m_btnArrary[i].Create(ctrlID, rect.m_left, rect.m_top, rect.Width(), rect.Height(), this, WND_CHILD /*| WND_NO_IMAGE_BK*/, xFocus, yFocus);
		m_btnArrary[i].AddGroup(&m_btnGroup);

		++ctrlID;
		++xFocus;
		rect.MoveToX(rect.m_right);
		-- rect.m_left;

		m_itemLen[i] = 0;
	}
}

void GUI::CTabCtrl::OnDestroy()
{
	GUI::CWnd::OnDestroy();

	m_itemCount = 0;
	//m_tabLen = FIRST_BTN_OFFSET;
	m_tabLen = m_length[DLG_OFFSET_SUB_LEFT];
	m_curItem = 0;
}

bool GUI::CTabCtrl::AddItem(GUI::CWnd* pWnd, const std::string& str)
{
	assert(NULL != pWnd);
	if (m_itemCount < MAX_TAB_ITEM)
	{
		if (m_itemCount > 0)
		{
			m_btnArrary[m_itemCount - 1].GetWnd()->Show(false,false);
			m_btnArrary[m_itemCount - 1].SetCheck(false, false);
		}

		//根据字符串长度，调整其长度
		unsigned short cx = 0, cy = 0;
		if (!GUI::CWnd::TextSize(str.c_str(), str.length(), cx, cy))
		{
			assert (false);
		}

		cx += 12;
		m_itemLen[m_itemCount] = cx - 1;
		if(m_itemLen[m_itemCount] + m_tabLen > m_rect.Width())
		{
			m_tabLen = 0;
			for(int i = 0; i < m_itemCount; i++)
			{
				m_itemLen[i] = (m_rect.Width() - 12) / (m_itemCount + 1);
				m_btnArrary[i].SetWindowPos(m_tabLen, 0, m_itemLen[i] + 1, m_btnHeight);
				m_tabLen += m_itemLen[i];
			}

			m_itemLen[m_itemCount] = (m_rect.Width() - 12) / (m_itemCount + 1);
		}

		m_btnArrary[m_itemCount].SetWindowPos(m_tabLen, 0, m_itemLen[m_itemCount] + 1, m_btnHeight);
		m_tabLen += m_itemLen[m_itemCount];

		m_btnArrary[m_itemCount].MapWnd(pWnd);
		m_btnArrary[m_itemCount].SetCaption(str, true, false);
		m_btnArrary[m_itemCount].Show(true, false);
		m_btnArrary[m_itemCount].SetCheck(true, false);

		pWnd->Show(false, false);

		++m_itemCount;

		return true;
	} 
	else
	{
		return false;
	}

	return true;
}

void GUI::CTabCtrl::SetCurItem(int item, bool bRefresh/* = true*/)
{
	assert(item < MAX_TAB_ITEM);
	assert(item >= 0);
	assert(m_curItem < MAX_TAB_ITEM);

	m_btnArrary[m_curItem].GetWnd()->Show(false, false);
	m_btnArrary[item].GetWnd()->Show(true, bRefresh);
	m_btnArrary[item].SetCheck(true, bRefresh);
	m_curItem = item;
}

int GUI::CTabCtrl::GetFocusItem()
{
	assert(m_curItem < MAX_TAB_ITEM);

	for (int i = 0; i<MAX_TAB_ITEM; ++i)
	{
		if (m_btnArrary[i].IsFocus())
		{
			return i;
		}
	}
	
	return m_curItem;
}

GUI::CWnd* GUI::CTabCtrl::GetWnd( int item /*= -1*/ ) const
{
	assert(item < MAX_TAB_ITEM);

	int sel = item >= 0 ? item : m_curItem;

	return m_btnArrary[sel].GetWnd();
}

unsigned long GUI::CTabCtrl::OnCommand (unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	if (KEY_VIRTUAL_ENTER == keyID)
	{
		if ((ctrlID >= CTRLID_TAB_BTN_FIRST) && (ctrlID < CTRLID_TAB_BTN_END))
		{
			int index = ctrlID - CTRLID_TAB_BTN_FIRST;
			SetCurItem(index, true);
		}
	}

	return KEY_VIRTUAL_MSG_CONTINUE;
}

