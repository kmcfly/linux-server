#include "IPv6Ctrl.h"

using namespace GUI;

GUI::CIPV6Ctrl::CIPV6Ctrl()
{
	m_ctrlType = GUI::WND_TYPE_IPV6;
}

GUI::CIPV6Ctrl::~CIPV6Ctrl()
{

}

bool GUI::CIPV6Ctrl::Create( unsigned long ctrlID, 
								 unsigned long x, 
								 unsigned long y,
								 unsigned long cx, 
								 unsigned long cy,
								 CWnd* pParent,
								 int xFocus,
								 int yFocus,
								 bool bVisible/* = true*/)
{
	//设置对话框风格
	unsigned long style = WND_CHILD;
	if (bVisible)
	{
		style |= WND_VISIBLE;
	} 

	//创建窗口
	CalculatePos();
	int min_len = TestDrawLen();
	printf("min_len=%d\n",min_len);
	cx = (cx < min_len) ? min_len : cx;
	cy = (cy < m_length[IP_CTRL_HEIGHT]) ? m_length[IP_CTRL_HEIGHT] : cy;
	if (GUI::CWnd::Create(ctrlID, x, y, cx, cy, pParent, style, xFocus, yFocus))
	{
		return true;
	}
	else
	{
		return false;
	}
}

void GUI::CIPV6Ctrl::SetIP(const unsigned char *pIP, unsigned length, bool bRefresh)
{
	assert (pIP);

	assert(40 == length);		//IPv6存储的长度固定是40
	char szTemp[40] = {0};
	std::string strTemp;
	int ipv6 = 0;

	for (int i = 0; i < FIELD_COUNT; i++)
	{
		memset(szTemp, 0, sizeof(szTemp));
		ipv6 = 0;
		for (int j = 0; j < length; j++)
		{
			if ((pIP[j] == ':') || (pIP[j] == '\0'))
			{
				memcpy(szTemp, pIP, j);
				pIP += j + 1;
				break;
			}
		}
		strTemp = szTemp;
		m_field[i].SetValue(strTemp, false);
	}

	if (bRefresh)
	{
		Repaint();
	}
}

void GUI::CIPV6Ctrl::GetIP(unsigned char *pIPv6, int length) const
{
	assert(pIPv6);
	assert(40 == length);

	std::string strTemp;
	std::string strValue;
	for (int i = 0; i < FIELD_COUNT; i++)
	{
		strTemp.clear();
		m_field[i].GetValue(strTemp);
		strValue += strTemp;
		if (i != FIELD_COUNT - 1)
		{
			strValue += ":";
		}
	}
	memcpy(pIPv6, strValue.c_str(), strValue.length());
}

void GUI::CIPV6Ctrl::OnInitial()
{
	GUI::CWnd::OnInitial();

	int x = 1;
	int y = 0;
	int cx = m_field_width;
	int cy = m_rect.Height();

	for (int i = 0; i < FIELD_COUNT; ++i)
	{
		m_field[i].Create(CTRLID_IPCTRL_FIELD1 + i, x, y, cx, cy, this, i, 0, true, false, true);
		m_field[i].AtuoClear(true);

		x += m_field_width + m_parition_width;
	}
}

void GUI::CIPV6Ctrl::OnPaint(PAINT_STATUS status)
{
	GUI::CWnd::OnPaint(status);

	if (PAINT_STATUS_DISABLE == status)
	{
		GUI::CWnd::DrawRect(m_rect.m_left, m_rect.m_top, m_rect.Width(), m_rect.Height(), COLOR_DISABLE);
	} 
	else
	{
		GUI::CWnd::DrawRect(m_rect.m_left, m_rect.m_top, m_rect.Width(), m_rect.Height(), COLOR_EDIT);
	}

	GUI::CWnd::DrawRectBorder(m_rect.m_left, m_rect.m_top, m_rect.Width(), m_rect.Height(), COLOR_LINE_WHITE);

	//绘制分割符
	{
		int x = m_rect.m_left + m_field_width;
		int y = m_rect.m_top + 1;
		int cx = m_parition_width;
		int cy = m_rect.Height() - 2;

		for (int i = 0; i < FIELD_COUNT - 1; ++i)
		{
			GUI::CWnd::DrawText(":", 1, x, y, cx, cy);
			x += m_field_width + m_parition_width;
		}
	}
}

void GUI::CIPV6Ctrl::CalculatePos()
{
	unsigned short cx = 0;
	unsigned short cy = 0;
	GUI::CWnd::TextSize(":", 1, cx, cy) ;
	m_parition_width = static_cast<int>(cx) ;

	GUI::CWnd::TextSize("8888", 4, cx, cy);
	m_field_width = static_cast<int>(cx) + m_length[EDIT_XPOS] * 2;
}

int GUI::CIPV6Ctrl::TestDrawLen()
{
	if(m_width<=800)
	{
		return 0;
	}
	else
	{
		return (m_field_width * FIELD_COUNT + m_parition_width * (FIELD_COUNT - 1) + 2);
	}
	
}

