#include "EditStaticCtrl.h"
#include "KeyboardDlg.h"

using namespace GUI;

GUI::CEditStaticCtrl::CEditStaticCtrl() : m_maxLen(32)
, m_bPassword(false)
, m_cursorMode(CURSOR_MODE_AUTO)
, m_bAutoClear(false)
, m_bEnableInput(true)
, m_bFollowCtrl(false)
{
	m_ctrlType = GUI::WND_TYPE_EDIT;
}

GUI::CEditStaticCtrl::~CEditStaticCtrl()
{

}

void GUI::CEditStaticCtrl::SetStr(const char *pStr)
{
	if(0!= strlen(pStr))
	{
		strcpy(m_str,pStr);
	}
	else
	{
		memset(m_str,0,sizeof(m_str));
	}
	
}

bool GUI::CEditStaticCtrl::Create(unsigned long ctrlID, 
							unsigned long x, 
							unsigned long y,
							unsigned long cx, 
							unsigned long cy,
							CWnd* pParent, 
							int xFocus, 
							int yFocus, 
							bool bVisible/* = ture*/,
							const char* pCaption /* = NULL */,
							bool bBorder /*= true*/)
{
	//���öԻ�����
	unsigned long style = WND_CHILD;
	if (bVisible)
	{
		style |= WND_VISIBLE;
	} 

	if (!bBorder)
	{
		style |= WND_NO_BORDER;
	}

	//��������
	cx = (cx < m_length[FONT_WIDTH]) ? m_length[FONT_WIDTH] : cx;
	cy = (cy < m_length[EDIT_HEIGHT]) ? m_length[EDIT_HEIGHT] : cy;
	if (GUI::CWnd::Create(ctrlID, x, y, cx, cy, pParent, style, xFocus, yFocus))
	{
		if (NULL != pCaption)
		{
			SetCaption(pCaption, false);
		}
		memset(m_str,0,sizeof(m_str));
		return true;
	}
	else
	{
		return false;
	}
}

void GUI::CEditStaticCtrl::SetCaption(const std::string& strCaption, bool bRefresh /* = true */)
{
	if (strCaption.length() > m_maxLen)
	{
		std::string str = strCaption;
		str.erase(m_maxLen, strCaption.length() - m_maxLen);
		GUI::CWnd::SetCaption(str, bRefresh);
	}
	else
	{
		GUI::CWnd::SetCaption(strCaption, bRefresh);
	}	
}

void GUI::CEditStaticCtrl::SetBkColor(PAINT_STATUS status, unsigned long background_color, unsigned long border_color)
{
	COLOR_MAP::iterator iter = m_colorMap.find(status);
	if (iter != m_colorMap.end())
	{
		iter->second.background_color = background_color;
		iter->second.border_color = border_color;
	}
	else
	{
		assert(false);
	}
}

void GUI::CEditStaticCtrl::OnInitial()
{
	GUI::CWnd::OnInitial();

	EDIT_COLOR color;
	color.background_color = COLOR_EDIT;
	color.border_color = COLOR_LINE_WHITE;
	color.string_color = COLOR_TEXT_NORMAL;
	m_colorMap[PAINT_STATUS_NORMAL] = color;

	color.background_color = COLOR_SELECT;
	color.border_color = COLOR_LINE_WHITE;
	m_colorMap[PAINT_STATUS_FOCUS] = color;

	color.background_color = COLOR_DISABLE;
	color.border_color = COLOR_LINE_WHITE;
	m_colorMap[PAINT_STATUS_DISABLE] = color;
}

void GUI::CEditStaticCtrl::OnPaint(PAINT_STATUS status)
{
	COLOR_MAP::const_iterator iter = m_colorMap.find(status);
	if (iter == m_colorMap.end())
	{
		assert(false);
		return;
	}
	const EDIT_COLOR& editColor = iter->second;
	int strLen = 0;
	if(0 != strlen(m_str))
	{
		strLen = 2 * m_length[BTN_WIDTH];
	}
	GUI::CWnd::DrawRect(m_rect.m_left, m_rect.m_top , m_rect.Width()- strLen, m_rect.Height(), editColor.background_color);	

	if (0 == (WND_NO_BORDER & m_style))
	{
		GUI::CWnd::DrawRectBorder(m_rect.m_left, m_rect.m_top, m_rect.Width()- strLen, m_rect.Height(), editColor.border_color);
	}
	else
	{
		GUI::CWnd::DrawLine(m_rect.m_left, m_rect.m_top, m_rect.m_right - strLen, m_rect.m_top, editColor.border_color);
		GUI::CWnd::DrawLine(m_rect.m_left, m_rect.m_bottom, m_rect.m_right - strLen, m_rect.m_bottom, editColor.border_color);
	}


	if (m_strCaption.length() > 0)
	{

		if (!m_bPassword)
		{
			GUI::CWnd::DrawText(m_strCaption.c_str(), m_strCaption.length() , m_rect.m_left+m_length[EDIT_XPOS], m_rect.m_top+m_length[EDIT_YPOS], m_rect.Width() - 2 * m_length[EDIT_XPOS]-strLen, m_rect.Height()-m_length[EDIT_YPOS], editColor.string_color);
		} 
		else
		{
			int len = m_strCaption.length() + 1;
			char* pwd = new char[len];
			memset(pwd, '*', len);
			pwd[len-1] = 0;
			GUI::CWnd::DrawText(pwd, strlen(pwd) , m_rect.m_left+m_length[EDIT_XPOS], m_rect.m_top+m_length[EDIT_YPOS], m_rect.Width() - 2 * m_length[EDIT_XPOS]-strLen, m_rect.Height()-m_length[EDIT_YPOS], editColor.string_color);
			delete [] pwd;
		}
	}
	//β�ͺ��油���ַ���
	
	if(0 !=strLen)
	{	
		GUI::CWnd::DrawText(m_str, strlen(m_str) , m_rect.m_left+m_rect.Width()-strLen, m_rect.m_top+m_length[EDIT_YPOS], strLen, m_rect.Height()-m_length[EDIT_YPOS], editColor.string_color);
	}
	
	//������
	switch (m_cursorMode)
	{
	case CURSOR_MODE_AUTO:
		{
			if ((PAINT_STATUS_FOCUS == status) && (m_strCaption.length() < m_maxLen))
			{
				DrawCursor();
			}	
		}
		break;
	case CURSOR_MODE_ALWAYS:
		if (m_strCaption.length() < m_maxLen)
		{
			DrawCursor();
		}		
		break;
	default:
		break;
	}

}

void GUI::CEditStaticCtrl::DrawCursor()
{
	unsigned short textWidth = 0, textHeight = 0;
	int len = m_strCaption.length();
	if (len > 0)
	{
		if(m_bPassword)
		{
			GUI::CWnd::TextSize("*", 1, textWidth, textHeight);
			textWidth *= len;
		}
		else
		{
			GUI::CWnd::TextSize(m_strCaption.c_str(), m_strCaption.length(), textWidth, textHeight);
		}
	}

	GUI::CWnd::DrawText("_", 1, 
		m_rect.m_left + m_length[EDIT_XPOS] + textWidth, 
		m_rect.m_top + m_length[EDIT_YPOS], 
		m_rect.Width()-m_length[EDIT_XPOS] - textWidth, 
		m_rect.Height()-m_length[EDIT_YPOS],
		COLOR_TEXT_FOCUS);
}

bool GUI::CEditStaticCtrl::AddChar(char ch)
{
	if (m_strCaption.length() < m_maxLen)
	{
		m_strCaption.push_back(ch);

		Repaint();
		return true;
	}
	else
	{
		if (m_bAutoClear)
		{
			m_strCaption.clear();
			m_strCaption.push_back(ch);
			Repaint();
			return true;
		} 
		else
		{
			return false;
		}		
	}
}

bool GUI::CEditStaticCtrl::DeleteChar()
{
	int len = m_strCaption.length();
	if (len > 0)
	{
#if 0
		//gb2312
		//[2010-04-19 15-38-00] L ����ַ��������β�����һ�����Ƿ�Ϊ˫�ֽ��ַ�
		//ֻ������˫�ֽ� ���λΪ1,���ж�.
		bool bDoubleByte = false;
		int i = 0;
		for(i = 0; i < len; i++ )
		{
			if(m_strCaption[i] & 0x80)
			{
				bDoubleByte = true;
				i++;
			}
			else
			{
				bDoubleByte = false;
			}
		}

		//����ԭ�����ܴ��ڴ���:���һ��˫��,ֻ�����λ
		if(i > len)
		{
			bDoubleByte = false;
		}

		if( bDoubleByte )
		{
			m_strCaption.erase(m_strCaption.end() - 1);
			if(m_strCaption.length() > 0)
			{
				m_strCaption.erase(m_strCaption.end() - 1);
			}
		}
		else
		{
			m_strCaption.erase(m_strCaption.end() - 1);
		}
#else
		//utf-8
		char* utf8Ptr = const_cast<char*>(m_strCaption.c_str());
		int index = 0;
		int last_size = 0;
		while(len > index)
		{
			//�������һ���ַ���ռλ��
			int size = 0;
			unsigned long charID = 0;
			GUI_UTF8_TO_UCS2(utf8Ptr, charID, size);

			utf8Ptr += size;

			if(index + size == len)
			{
				//ԭ�ַ���������ĩβ�ַ���
				last_size = size;
				break;
			}
			else if(index + size > len)
			{
				//ԭ�ַ�����������ĩβ�ַ���
				//��֤ǰ�沿���ַ�������
				if(len > index)
				{
					last_size = len - index;
				}
				else
				{
					last_size = size;
					gui_assert(false, printf("str=%s. len=%d. index=%d.\n", m_strCaption.c_str(),len, index));
				}
				break;
			}

			index += size;
		}

		if(last_size > 0)
		{
			m_strCaption.erase(m_strCaption.end() - last_size, m_strCaption.end());
		}

#endif
		Repaint();
		return true;
	}
	else
	{
		return false;
	}
}

char GUI::CEditStaticCtrl::GetChar(unsigned long keyID) const
{
	return ('0' + (keyID - KEY_VIRTUAL_0));	
}

BEGIN_MSG_MAP(CEditStaticCtrl, GUI::CWnd)
	//ON_MSG_KEY_RANGE(CEditCtrl::GetID(), KEY_VIRTUAL_0, KEY_VIRTUAL_9, OnInput)
	//ON_MSG(CEditCtrl::GetID(), KEY_VIRTUAL_REW, OnDelete)
	ON_MSG(CEditStaticCtrl::GetID(), KEY_VIRTUAL_ENTER, OnEnter)
	//ON_MSG(CEditCtrl::GetID(), KEY_VIRTUAL_ADD, OnKeyAdd)
	//ON_MSG(CEditCtrl::GetID(), KEY_VIRTUAL_DEC, OnKeyDec)
END_MSG_MAP()

unsigned long GUI::CEditStaticCtrl::OnInput(unsigned long keyID)
{
	if( !m_bEnableInput ) return KEY_VIRTUAL_MSG_CONTINUE;

	AddChar(GetChar(keyID));
	return KEY_VIRTUAL_MSG_CONTINUE;
}

unsigned long GUI::CEditStaticCtrl::OnDelete()
{
	if( !m_bEnableInput ) return KEY_VIRTUAL_MSG_CONTINUE;

	DeleteChar();
	return KEY_VIRTUAL_MSG_CONTINUE;
}

unsigned long GUI::CEditStaticCtrl::OnEnter()
{
	if(!m_bEnableInput) return KEY_VIRTUAL_MSG_CONTINUE;

	CKeyboardDlg keyboardDlg;
	keyboardDlg.SetMaxLen(m_maxLen);
	keyboardDlg.SetText(m_strCaption.c_str());
	keyboardDlg.SetPassword(m_bPassword);
	GUI::CRect rcKeyboard = keyboardDlg.GetKeyboardRect();

	int x = CENTER_POS, y = CENTER_POS;
	int cx = rcKeyboard.Width();
	int cy = rcKeyboard.Height();
	if(m_bFollowCtrl)
	{
		x = m_rect.m_left;
		y = m_rect.m_bottom + 2;
		if( y + cy > static_cast<int>(m_height))
		{
			y = m_rect.m_top - cy - 2;
		}
		if( x + cx > static_cast<int>(m_width))
		{
			x = (m_width - cx - 2)/2;
		}
	}

	if (DLG_OK == keyboardDlg.DoModal(this, x, y, cx, cy))
	{
		m_strCaption = keyboardDlg.GetText();
		while (m_strCaption.length() > m_maxLen)
		{
			m_strCaption.erase(m_strCaption.end() - 1);
		}

		GUI::CWnd* pWndTop = GetTopestWnd();
		if (NULL != pWndTop)
		{
			pWndTop->Repaint();
		}
	}
	return KEY_VIRTUAL_MSG_CONTINUE;
}

unsigned long GUI::CEditStaticCtrl::OnKeyAdd()
{
	if( !m_bEnableInput ) return KEY_VIRTUAL_MSG_CONTINUE;

	return KEY_VIRTUAL_MSG_CONTINUE;
}

unsigned long GUI::CEditStaticCtrl::OnKeyDec()
{
	if( !m_bEnableInput ) return KEY_VIRTUAL_MSG_CONTINUE;

	//��ǰ���һ���ַ�
	DeleteChar();
	return KEY_VIRTUAL_MSG_CONTINUE;
}
