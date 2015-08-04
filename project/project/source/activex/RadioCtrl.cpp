#include "RadioCtrl.h"
using namespace GUI;


//CRadioCtrl
GUI::CRadioCtrl::CRadioCtrl() : m_pRadioGroup(NULL)
{
	m_ctrlType = GUI::WND_TYPE_RADIO;
}

GUI::CRadioCtrl::~CRadioCtrl()
{

}

bool GUI::CRadioCtrl::Create(unsigned long ctrlID, 
							 unsigned long x, 
							 unsigned long y,
							 unsigned long cx, 
							 unsigned long cy,
							 CWnd* pParent, 
							 unsigned long xFocus, 
							 unsigned long yFocus, 
							 CRadioGroup* pRadioGroup /* = NULL */)
{
	if (GUI::CCheckCtrl::Create(ctrlID, x, y, (cx < m_length[RADIO_WIDTH]) ? m_length[RADIO_WIDTH] : cx, (cy < m_length[RADIO_HEIGHT]) ? m_length[RADIO_HEIGHT] : cy, pParent, xFocus, yFocus))
	{
		if (NULL != pRadioGroup)
		{
			AddGroup(pRadioGroup);
		}
		return true;
	}
	else
	{
		return false;
	}
}

unsigned long GUI::CRadioCtrl::OnCommand (unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	if ((KEY_VIRTUAL_ENTER == keyID) && (ctrlID == m_windowID))
	{
		SetCheck(!m_bCheck, true);
	}

	return KEY_VIRTUAL_MSG_CONTINUE;
}


void GUI::CRadioCtrl::OnPaint(PAINT_STATUS status)
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

void GUI::CRadioCtrl::SetCheck(bool bCheck, bool bRefresh /* = true */)
{
	if (IsCheck())
	{
		//不在任何操作
	}
	else
	{
		if (NULL != m_pRadioGroup)
		{
			CRadioCtrl* pCheckRadio = m_pRadioGroup->GetCheckRadio();
			if (NULL != pCheckRadio)
			{
				assert(pCheckRadio != this);
				pCheckRadio->GUI::CCheckCtrl::SetCheck(false, bRefresh);
			}
		}
	
		GUI::CCheckCtrl::SetCheck(true, bRefresh);
	}
}

void GUI::CRadioCtrl::AddGroup(CRadioGroup* pRadioGroup)
{
	assert(NULL != pRadioGroup);
	pRadioGroup->AddRadio(this);
	m_pRadioGroup = pRadioGroup;
}

//CRadioGroup
GUI::CRadioGroup::CRadioGroup()
{

}

GUI::CRadioGroup::~CRadioGroup()
{

}

void GUI::CRadioGroup::AddRadio(GUI::CRadioCtrl* pRadioCtrl)
{
	assert(NULL != pRadioCtrl);
	m_radioList.push_back(pRadioCtrl);
}

GUI::CRadioCtrl* GUI::CRadioGroup::GetCheckRadio()
{
	RADIO_LIST::iterator iter;
	for (iter = m_radioList.begin(); iter != m_radioList.end(); ++iter)
	{
		if ((*iter)->IsCheck())
		{
			return (*iter);
		}
	}

	return NULL;
}

void GUI::CRadioGroup::Clear()
{
	m_radioList.clear();
}
