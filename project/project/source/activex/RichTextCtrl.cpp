#include "RichTextCtrl.h"

using namespace GUI;

extern bool g_langRightOrder;

CRichTextCtrl::CRichTextCtrl() : m_OffLeft(4),
m_OffTop(4),
m_OffBottom(4),
m_bInitCtrl(false),
m_bInitDrawList(false)
{
	m_ctrlType = WND_TYPE_RICH_TEXT;
	m_bScrollSCreen =false;//默认是false
	m_curRow = 0;
}

CRichTextCtrl::~CRichTextCtrl()
{

}

bool CRichTextCtrl::Create(unsigned long ctrlID, unsigned long x, unsigned long y, unsigned long cx, unsigned long cy, CWnd* pParent)
{
	m_TextRow =0;
	if (GUI::CWnd::Create(ctrlID, x, y, cx, cy, pParent, WND_CHILD | WND_VISIBLE | WND_NO_IMAGE_BK | WND_DISABLE_FOCUS, 0, 0))
	{
		m_bInitCtrl = true;
		return true;
	}
	else
	{
		return false;
	}
}

void CRichTextCtrl::OnPostInitial()
{
	GUI::CWnd::OnPostInitial();

	if(!m_bInitDrawList)
	{
		AutoLayoutText();
	}
}

void CRichTextCtrl::OnDestroy()
{
	GUI::CWnd::OnDestroy();

	m_strText = "";

	m_drawList.clear();
}

void CRichTextCtrl::SetText(const std::string& str, bool bRefresh)
{
	m_strText = str;
	m_curRow = 0;
	m_bInitDrawList = false;
	if(m_bInitCtrl)
	{
		AutoLayoutText();
	}

	if( bRefresh )
	{
		Repaint();
	}
}

void CRichTextCtrl::AppendText(const std::string& str, bool bRefresh)
{
	m_strText += str;

	m_bInitDrawList = false;
	if(m_bInitCtrl)
	{
		AutoLayoutText();
	}

	if( bRefresh )
	{
		Repaint();
	}
}

unsigned short CRichTextCtrl::GetTextRow()
{
	return m_TextRow;
}

void CRichTextCtrl::SetScrollShow(bool bScroll)
{
	m_bScrollSCreen = bScroll;
}

void CRichTextCtrl::SetCurRow(unsigned short row)
{
	m_curRow = row;
}

void CRichTextCtrl::OnPaint(PAINT_STATUS status)
{
	CWnd::OnPaint(status);

	//GUI::CWnd::DrawRectBorder(m_rect.m_left, m_rect.m_top, m_rect.Width(), m_rect.Height(), COLOR_SELECT, 2);

	unsigned short x_start = m_rect.m_left + m_OffLeft, y_start = m_rect.m_top + m_OffTop;
	unsigned short cx = m_rect.Width() - 2 * m_OffLeft, cy = m_rect.Height() - m_OffTop - m_OffBottom;
	unsigned short strHeight = m_length[STATIC_HEIGHT], height = strHeight + 8;

	unsigned short len = 0;
	unsigned short x = x_start, y = y_start;
	unsigned short rowCount = 0;

	std::list<std::string>::iterator iter;
	for (iter = m_drawList.begin(); iter != m_drawList.end(); iter++)
	{
		if(m_bScrollSCreen)
		{
			//从m_curRow往下显示
			if(rowCount >= m_curRow)//找到需要显示的第一行了
			{
				if(g_langRightOrder)
				{
					GUI::CWnd::DrawText(iter->c_str(), iter->length(), x, y, cx, strHeight, COLOR_TEXT_NORMAL, false, 1, false);
				}
				else
				{
					GUI::CWnd::DrawText(iter->c_str(), iter->length(), x, y, cx, strHeight);
				}
				y += height;
				x = x_start;
			}
			
		}
		else
	{
		if(g_langRightOrder)
		{
			GUI::CWnd::DrawText(iter->c_str(), iter->length(), x, y, cx, strHeight, COLOR_TEXT_NORMAL, false, 1, false);
		}
		else
		{
			GUI::CWnd::DrawText(iter->c_str(), iter->length(), x, y, cx, strHeight);
		}

		y += height;
		x = x_start;
		}
		
		
		if(m_bScrollSCreen)
		{
			++rowCount;
		}
		if(y >= m_rect.m_bottom)
		{
			break;
		}
	}
}

void CRichTextCtrl::Reset()
{
	m_curPos = 0;
}

bool CRichTextCtrl::ReadNext(std::string& out)
{
	int len = m_strText.length();
	if( m_curPos >= len ) 
	{
		return false;
	}

	std::string str;
	std::basic_string <char>::size_type pos = m_strText.find('\\', m_curPos);
	if( pos != std::string::npos && pos > m_curPos )
	{
		out = m_strText.substr(m_curPos, pos - m_curPos);

		if( m_strText[pos+1] == 'n' )
		{
			m_curPos = pos + 2;
		}
		else
		{
			m_curPos = pos + 1;
			gui_trace_np(true, printf("Invalid format : \\, text=%s.\n", m_strText.c_str()));
		}

		return true;
	}
	else
	{
		out = m_strText.substr(m_curPos, m_strText.length() - m_curPos);
		return false;
	}
}

unsigned short CRichTextCtrl::GetMaxDrawLen(bool& outbEnd, const std::string& str, unsigned short startLen, unsigned short maxWidth)
{
	assert( str.length() > startLen );

	unsigned short cx = 0, cy = 0;
	unsigned short char_width = 8;
	unsigned short len = maxWidth / char_width;

	//[2010-08-12 L] TextSize的参数len要求不会过界
	len = (len > (str.length() - startLen)) ? (str.length() - startLen) : len;

	m_bAddLast = 0;
	bool bOut = false;
	while(true)
	{
		GUI::CWnd::TextSize(str.c_str() + startLen, len, cx, cy);

		if( maxWidth >= cx && len > str.length() - startLen)
		{
			len = str.length() - startLen;
			break;
		}

		if( bOut || (maxWidth > cx && maxWidth - cx < char_width) )
		{
			if( str.length() > startLen + len + 1 )
			{
				char char_last = str[startLen + len - 1];
				char char_last_next = str[startLen + len];

				if( IsAsciiLetter(char_last) && IsAsciiLetter(char_last_next) )
				{
					m_lenLast = len;
					char char_last_tmp = str[startLen + len - 1];
					do 
					{
						if( len <= 1 )
						{
							//找到最前边了,说明本行全部为字母,则按原始方式输出
							len = m_lenLast;
							break;
						}
						len -= 1;
						char_last_tmp = str[startLen + len - 1];
					} while (IsAsciiLetter(char_last_tmp));
				}
			}

			break;
		}

		if( maxWidth > cx )
		{
			if( m_bAddLast < 0 )
			{
				bOut = true;
				//break;
			}
			else
			{
				len += 1;
				m_bAddLast = 1;
			}
		}
		else
		{
			if( m_bAddLast > 0 )
			{
				bOut = true;
				//break;
			}
			else
			{
				len -= 1;
				m_bAddLast = -1;
			}
		}
	}

	//超出范围修正
	len = (len > str.length() - startLen) ? (str.length() - startLen) : len;

	//utf-8修正,避免截断
	char* utf8Ptr = const_cast<char*>(str.c_str() + startLen);
	if(len > 0)
	{
		//utf-8完整性修正
		unsigned short ii = 0;
		unsigned short lastsize = 0;
		unsigned short last_order_len = 0;	//右序,长度
		unsigned short last_len = 0;
		int b_pre_right_order = 0;
		unsigned long last_char_id = 0;
		do
		{
			unsigned long charID = 0;
			int size = 0;
			GUI_UTF8_TO_UCS2(utf8Ptr, charID, size);

			if(g_langRightOrder)
			{
				//此段只能简单的从右序语言中识别非右序语言字符
				//如果在右序显示情况下,此段全部为非右序语言字符,则
				//无排版格式.且只处理了掺杂少量非右序语言字符的情况
				int b_rightorder = 0;
				if(IsRightOrderLetter(charID))
				{
					b_rightorder = 1;
				}
				else
				{
					if(IsSymbolEn(charID))
					{
						b_rightorder = 0;
					}
					else
					{
						b_rightorder = -1;
					}
				}

				if(0 == b_pre_right_order)
				{
					last_order_len = ii;
				}
				else
				{
					if((b_rightorder > 0) && (b_pre_right_order < 0))
					{
						last_order_len = ii;
					}

					if((b_rightorder < 0) && (b_pre_right_order > 0))
					{
						last_order_len = ii;
					}

					if(b_rightorder == 0)
					{
						last_order_len = ii + size;
					}
				}

				b_pre_right_order = b_rightorder;
			}

			utf8Ptr += size;
			ii += size;
			lastsize = size;
			last_char_id = charID;
		}while(ii < len);

		if(g_langRightOrder && (last_order_len > 0))
		{
			len = last_order_len;
		}
		else
		{
			len = ii;
		}

		//最后一次位置修正
		GUI::CWnd::TextSize(str.c_str() + startLen, len, cx, cy);
		if(cx > maxWidth)
		{
			if((last_char_id < 0x80) && IsAsciiLetter(static_cast<char>(last_char_id)))
			{
				//最后一个是英文时,
				//len = len - lastsize;
			}
			else
			{
				len = len - lastsize;
			}
		}
	}

	return len;
}

bool CRichTextCtrl::IsAsciiLetter(char ch)
{
	if( ch >= '0' && ch <= '9' )
	{
		return true;
	}

	if( ch >= 'a' && ch <= 'z' )
	{
		return true;
	}

	if( ch >= 'A' && ch <= 'Z' )
	{
		return true;
	}

	return false;
}

bool CRichTextCtrl::IsSymbolEn(unsigned long charID)
{
	if( (0x30 > charID)||
		((0x39 < charID) && (0x41 > charID)) ||
		((0x5A < charID) && (0x61 > charID)) ||
		((0x7A < charID) && (0x80 > charID)) )
	{
		//英文键盘非字符区域
		return true;
	}

	return false;
}

bool CRichTextCtrl::IsRightOrderLetter(unsigned long charID)
{
	//右序语言区
	if((0x0590 <= charID) && (0x077F >= charID))
	{
		return true;
	}
	else
	{
		return false;
	}
}

void CRichTextCtrl::AutoLayoutText()
{
	unsigned short cx = m_rect.Width() - 3 * m_OffLeft;
	unsigned short len = 0;

	m_drawList.clear();

	Reset();
	while( 1 )
	{
		std::string draw_text;
		bool ret = ReadNext(draw_text);

		if( !draw_text.empty() )
		{
			if(g_langRightOrder)
			{
				char* pNewStr = new char[draw_text.length() + 1];
				memset(pNewStr, 0, draw_text.length() + 1);
				
				GUI::CWnd::ReSortText(pNewStr, draw_text.c_str(), draw_text.length());
				
				draw_text= pNewStr;
				delete [] pNewStr;
			}

			len = 0;
			bool bEnd = false;
			do 
			{
				bEnd = false;
				unsigned short max_draw_len = GetMaxDrawLen(bEnd, draw_text, len, cx);//获取本行最大可以显示字符数目

				m_drawList.push_back(draw_text.substr(len, max_draw_len));
				len += max_draw_len;
				++m_TextRow;
			} while(len < draw_text.length() && !bEnd);
		}

		if( !ret )
		{
			break;
		}
	}

	m_bInitDrawList = true;
}

//end
