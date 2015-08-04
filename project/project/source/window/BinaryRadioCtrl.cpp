#include "BinaryRadioCtrl.h"
using namespace GUI;


//CRadioCtrl
GUI::CBinaryRadioCtrl::CBinaryRadioCtrl()
{
	m_ctrlType = GUI::WND_TYPE_RADIO;
}

GUI::CBinaryRadioCtrl::~CBinaryRadioCtrl()
{

}

bool GUI::CBinaryRadioCtrl::Create(unsigned long ctrlID, 
							 unsigned long x, 
							 unsigned long y,
							 unsigned long cx, 
							 unsigned long cy,
							 CWnd* pParent, 
							 unsigned long xFocus, 
							 unsigned long yFocus 
							 )
{
	if (GUI::CCheckCtrl::Create(ctrlID, x, y, (cx < m_length[RADIO_WIDTH]) ? m_length[RADIO_WIDTH] : cx, (cy < m_length[RADIO_HEIGHT]) ? m_length[RADIO_HEIGHT] : cy, pParent, xFocus, yFocus))
	{
		return true;
	}
	else
	{
		return false;
	}
}

unsigned long GUI::CBinaryRadioCtrl::OnCommand (unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	if ((KEY_VIRTUAL_ENTER == keyID) && (ctrlID == m_windowID))
	{
		SetCheck(!m_bCheck, true);
	}

	return KEY_VIRTUAL_MSG_CONTINUE;
}


void GUI::CBinaryRadioCtrl::OnPaint(PAINT_STATUS status)
{
	BITMAP_ID bmpID = BMP_INVALID;
	if (m_bCheck)
	{
		switch (status)
		{
		case PAINT_STATUS_FOCUS:
			bmpID = BMP_RADIOCTRL_CHECK_FOCUS;
			break;
		case PAINT_STATUS_DISABLE:
			bmpID = BMP_RADIOCTRL_CHECK_DISABLE;
			break;
		default:
			bmpID = BMP_RADIOCTRL_CHECK;
			break;
		}
	} 
	else
	{
		switch (status)
		{
		case PAINT_STATUS_FOCUS:
			bmpID = BMP_RADIOCTRL_FOCUS;
			break;
		case PAINT_STATUS_DISABLE:
			bmpID = BMP_RADIOCTRL_DISABLE;
			break;
		default:
			bmpID = BMP_RADIOCTRL;
			break;
		}
	}

	if ((m_strCaption.length() > 0) && (m_rect.Width() > m_length[RADIO_WIDTH]))
	{
		if (m_bLeftText)
		{
			unsigned long len = GUI::CWnd::DrawText(m_strCaption.c_str(), m_strCaption.length(), m_rect.m_left, m_rect.m_top, m_rect.Width() - m_length[RADIO_WIDTH] - 2, m_rect.Height());

			if (len < m_strCaption.length())	//表示没有显示完整
			{
				DrawImage(bmpID, m_rect.m_left + m_rect.Width() - m_length[RADIO_WIDTH], m_rect.m_top);
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
			if(!g_langRightOrder)
			{
				DrawImage(bmpID, m_rect.m_left, m_rect.m_top);
				GUI::CWnd::DrawText(m_strCaption.c_str(), m_strCaption.length(), m_rect.m_left+m_length[RADIO_WIDTH]+2, m_rect.m_top, m_rect.Width() - m_length[RADIO_WIDTH] - 2, m_rect.Height());
			}
			else
			{
				DrawImage(bmpID, m_rect.m_left + m_rect.Width() - m_length[RADIO_WIDTH], m_rect.m_top);
				GUI::CWnd::DrawText(m_strCaption.c_str(), m_strCaption.length(), m_rect.m_left, m_rect.m_top, m_rect.Width() - m_length[RADIO_WIDTH] - 2, m_rect.Height());
			}
		}
	}
	else
	{
		DrawImage(bmpID, m_rect.m_left, m_rect.m_top);
	}
}

void GUI::CBinaryRadioCtrl::SetCheck(bool bCheck, bool bRefresh /* = true */)
{
	if (IsCheck())
	{
		GUI::CCheckCtrl::SetCheck(false, bRefresh);
	}
	else
	{
	
		GUI::CCheckCtrl::SetCheck(true, bRefresh);
	}
}