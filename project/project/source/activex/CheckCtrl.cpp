#include "CheckCtrl.h"
using namespace GUI;

//CCheckCtrl
GUI::CCheckCtrl::CCheckCtrl() : m_bCheck(false), m_bLeftText(false)
{
	m_ctrlType = GUI::WND_TYPE_CHECK;
}

GUI::CCheckCtrl::~CCheckCtrl()
{

}

bool GUI::CCheckCtrl::Create(unsigned long ctrlID, unsigned long x, unsigned long y, unsigned long cx, unsigned long cy, CWnd* pParent, unsigned long xFocus, unsigned long yFocus)
{
	if (GUI::CWnd::Create(ctrlID, x, y, (cx < m_length[CHECK_WIDTH]) ? m_length[CHECK_WIDTH] : cx, (cy < m_length[CHECK_HEIGHT]) ? m_length[CHECK_HEIGHT] : cy, pParent, WND_CHILD | WND_VISIBLE, xFocus, yFocus))
	{
		return true;
	}
	else
	{
		return false;
	}
}

void GUI::CCheckCtrl::OnPaint(PAINT_STATUS status)
{
	BITMAP_ID bmpID = BMP_INVALID;
	if (m_bCheck)
	{
		switch (status)
		{
		case PAINT_STATUS_FOCUS:
			bmpID = BMP_CHECKCTRL_CHECK_FOCUS;
			break;
		case PAINT_STATUS_DISABLE:
			bmpID = BMP_CHECKCTRL_CHECK_DISABLE;
			break;
		default:
			bmpID = BMP_CHECKCTRL_CHECK;
			break;
		}
	} 
	else
	{
		switch (status)
		{
		case PAINT_STATUS_FOCUS:
			bmpID = BMP_CHECKCTRL_FOCUS;
			break;
		case PAINT_STATUS_DISABLE:
			bmpID = BMP_CHECKCTRL_DISABLE;
			break;
		default:
			bmpID = BMP_CHECKCTRL;
			break;
		}
	}
	
	if ((m_strCaption.length() > 0) && (m_rect.Width() > m_length[CHECK_WIDTH]))
	{
		if (m_bLeftText)
		{
			unsigned long len = GUI::CWnd::DrawText(m_strCaption.c_str(), m_strCaption.length(), m_rect.m_left, m_rect.m_top, m_rect.Width() - m_length[CHECK_WIDTH] - 2, m_rect.Height());

			if(!g_langRightOrder)
			{
			if (len < m_strCaption.length())	//表示没有显示完整
			{
				DrawImage(bmpID, m_rect.m_left + m_rect.Width() - m_length[CHECK_WIDTH], m_rect.m_top);
			}
			else
			{
				unsigned short cx = 0, cy = 0;
				if (!GUI::CWnd::TextSize(m_strCaption.c_str(), m_strCaption.length(), cx, cy))
				{
					assert (false);
				}

					DrawImage(bmpID, m_rect.m_left + cx + 2, m_rect.m_top);
			}
		}
		else
			{
				DrawImage(bmpID, m_rect.m_left + m_rect.Width() - m_length[CHECK_WIDTH], m_rect.m_top);
			}
		}
		else
		{
			if(!g_langRightOrder)
			{
			DrawImage(bmpID, m_rect.m_left, m_rect.m_top);
			GUI::CWnd::DrawText(m_strCaption.c_str(), m_strCaption.length(), m_rect.m_left+m_length[CHECK_WIDTH]+2, m_rect.m_top, m_rect.Width() - m_length[CHECK_WIDTH], m_rect.Height());
			}
			else
			{
				DrawImage(bmpID, m_rect.m_left + m_rect.Width() - m_length[CHECK_WIDTH], m_rect.m_top);
				GUI::CWnd::DrawText(m_strCaption.c_str(), m_strCaption.length(), m_rect.m_left, m_rect.m_top, m_rect.Width() - m_length[CHECK_WIDTH] - 2, m_rect.Height());
			}
		}	
	}
	else
	{
		DrawImage(bmpID, m_rect.m_left, m_rect.m_top);
	}
}

void GUI::CCheckCtrl::SetCheck(bool bCheck, bool bRefresh /* = true */)
{
	//不使能时，不再响应。
	if (m_bEnable && (m_bCheck != bCheck))
	{
		m_bCheck = bCheck;

		if (bRefresh)
		{
			Repaint();
		}
	}
}

void GUI::CCheckCtrl::SetCaption(const std::string& strCaption, bool bLeft/* = true*/, bool bRefresh/* = true*/)
{
	m_strCaption = strCaption;
	m_bLeftText = bLeft;

	if (bRefresh)
	{
		Repaint();
	}
}

unsigned long GUI::CCheckCtrl::OnCommand (unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	if ((KEY_VIRTUAL_ENTER == keyID) && (ctrlID == m_windowID))
	{
		SetCheck(!m_bCheck, true);
	}

	return KEY_VIRTUAL_MSG_CONTINUE;
}




