#include "IPAddressCtrl.h"

#ifdef __ENVIRONMENT_LINUX__
#include<netinet/in.h>
#endif

using namespace GUI;

GUI::CIPAddressCtrl::CIPAddressCtrl()
{
	m_ctrlType = GUI::WND_TYPE_IP;
}

GUI::CIPAddressCtrl::~CIPAddressCtrl()
{

}

bool GUI::CIPAddressCtrl::Create( unsigned long ctrlID, 
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

void GUI::CIPAddressCtrl::OnInitial()
{
	GUI::CWnd::OnInitial();

	int x = 1;
	int y = 0;
	int cx = m_field_width;
	int cy = m_rect.Height();

	for (int i = 0; i < FIELD_COUNT; ++i)
	{
		m_field[i].Create(CTRLID_IPCTRL_FIELD1 + i, x, y, cx, cy, this, i, 0, true, false, true);
		m_field[i].SetRange(0, 255);
		m_field[i].AtuoClear(true);

		x += m_field_width + m_parition_width;
	}
}

void GUI::CIPAddressCtrl::CalculatePos()
{
	unsigned short cx = 0;
	unsigned short cy = 0;
	GUI::CWnd::TextSize(".", 1, cx, cy) ;
	m_parition_width = static_cast<int>(cx) ;

	GUI::CWnd::TextSize("888", 3, cx, cy);
	m_field_width = static_cast<int>(cx) + m_length[EDIT_XPOS] * 2;
}

void GUI::CIPAddressCtrl::OnPaint(PAINT_STATUS status)
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
			GUI::CWnd::DrawText(".", 1, x, y, cx, cy);
			x += m_field_width + m_parition_width;
		}
	}
}

int GUI::CIPAddressCtrl::TestDrawLen()
{
	return (m_field_width * FIELD_COUNT + m_parition_width * (FIELD_COUNT - 1) + 2);
}

void GUI::CIPAddressCtrl::SetIP(BYTE field1, BYTE field2, BYTE field3, BYTE field4, bool bRefresh)
{
	m_field[0].SetValue(static_cast<int>(field1), false);
	m_field[1].SetValue(static_cast<int>(field2), false);
	m_field[2].SetValue(static_cast<int>(field3), false);
	m_field[3].SetValue(static_cast<int>(field4), false);

	if (bRefresh)
	{
		Repaint();
	}
}

void GUI::CIPAddressCtrl::GetIP(BYTE& field1, BYTE& field2, BYTE& field3, BYTE& field4) const
{
	field1 = static_cast<BYTE>(m_field[0].GetValue());
	field2 = static_cast<BYTE>(m_field[1].GetValue());
	field3 = static_cast<BYTE>(m_field[2].GetValue());
	field4 = static_cast<BYTE>(m_field[3].GetValue());
}

void GUI::CIPAddressCtrl::SetIP(unsigned long ip, bool bRefresh)
{
	unsigned char ch;
	for(int i = 0; i < 4; i++)
	{
		ch = IPADDRESS_FIELD(ip, i);
		m_field[i].SetValue(static_cast<int>(ch), false);
	}

	if (bRefresh)
	{
		Repaint();
	}
}

unsigned long GUI::CIPAddressCtrl::GetIP() const
{
	return IPADDRESS_VALUE(m_field[0].GetValue(), \
		m_field[1].GetValue(), \
		m_field[2].GetValue(), \
		m_field[3].GetValue());
}


int GUI::CIPAddressCtrl::IsValidIP(DWORD ip,DWORD dwMask,DWORD route)
{
	//IP和子网掩码的或不能全为1
	if(0xffffffff == (ip|dwMask))
	{
		// "The combination of IP address and subnet mask is invalid.All of the bits ";
		// "in the host address portion of the IP address are set to 1.\nPlease enter a valid combination of ";
		// "IP address and subnet mask.";
		return 1;
	}
	//检查主机号，不能为0
	if(0 == (ip&~dwMask))
	{
		//"The combination of IP address and subnet mask is invalid.All of the bits ";
		//"in the host address portion of the IP address are set to 0.\nPlease enter a valid combination of ";
		//"IP address and subnet mask.";
		return 2;
	}
	////检验网络号
	BYTE nIPField0 = static_cast<BYTE>(ip & 0xff);
	BYTE nRouteField0 = static_cast<BYTE>(route & 0xff);
	if((nIPField0 == 127) || (nRouteField0 == 127))
	{
		//"IP addresses starting with 127 are not valid because they are reserved for loopback addresses.";
		//"Please specify some other valid value between 1 and 223.";
		return 3;
	}
	bool ipnetnumerr = (nIPField0 == 0) || (nIPField0 > 223);
	bool routenetnumerr=(nRouteField0 == 0) || (nRouteField0 > 223);
	if(ipnetnumerr || routenetnumerr)
	{
		//" %d is not valid entry.Please specify a value between 1 and 223."
		return 4;
	}

	//掩码中间出现了断位
	DWORD dwTemp = ntohl(dwMask);
	bool maskError = true;
	if ((nIPField0 >= 1) && (nIPField0 <= 126))
	{
		if (dwTemp < 0xff000000)
		{
			maskError = false;
			return 5;
		}
	}
	else if ((nIPField0 >= 128) && (nIPField0 <= 191))
	{
		if (dwTemp < 0xffff0000)
		{
			maskError = false;
			return 5;
		}
	}
	else if((nIPField0 >= 192) && (nIPField0 <= 223))
	{
		if (dwTemp < 0xffffff00)
		{
			maskError = false;
			return 5;
		}
	}

	for(int i = 0; (i < 32 ) && dwTemp; i++)
	{
		if(0 == (dwTemp & (0x01<<31)))		//中间出现了为断位
		{
			maskError = false;
			return 5;
		}
		dwTemp<<=1;
	}
	//////////////////////////////////////////////////////////////////////////
	return 0;
}

