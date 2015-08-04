#include "Menu.h"

using namespace GUI;

GUI::CMenu::CMenu() : m_selItem(-1)
{
	m_ctrlType = GUI::WND_TYPE_MENU;
}

GUI::CMenu::~CMenu()
{
	ClearItemList();
}

void GUI::CMenu::ClearItemList()
{
	for (MENU_ITEM_LIST::iterator iter = m_menuItemList.begin(); iter != m_menuItemList.end(); ++iter)
	{
		if (NULL != (*iter))
		{
			//先设置其数据为零，防止未弹出过该Menu，但是被设置过数值，导致析构时断言的错误。
			(*iter)->SetData(0);
			delete (*iter);
			*iter = NULL;
		}
	}

	m_menuItemList.clear();
	m_itemDataList.clear();
}


void GUI::CMenu::OnInitial()
{
	GUI::CDialog::OnInitial();

	m_style |= WND_NO_TITLE;
	/////////////////////////////////////////////////
	int count = m_menuItemList.size();
	int x = BORDER_WIDHT;
	int y = BORDER_WIDHT;
	int cx = m_rect.Width() - BORDER_WIDHT*2;
	int cy = m_length[BTN_HEIGHT];

	for (int i = 0; i < count; ++i)
	{
		m_menuItemList[i]->Create(GetRandWndID(), x, y, cx, cy, this, 0, i, m_menuItemList[i]->GetCaption().c_str(), WND_CHILD|WND_VISIBLE|WND_NO_IMAGE_BK);
		m_menuItemList[i]->SetAlignText(GUI::ALIGN_TEXT_LEFT);
		m_menuItemList[i]->DrawBorder(false);
		m_menuItemList[i]->SetBkFocusColor(COLOR_MENU_BK_FOCUS);
		y += cy;
	}

	m_selItem = -1;

}

void GUI::CMenu::OnDestroy()
{
	GUI::CDialog::OnDestroy();
}

void GUI::CMenu::OnPaint(PAINT_STATUS status)
{
	GUI::CWnd::DrawRectBorder(m_rect.m_left, m_rect.m_top, m_rect.Width(), m_rect.Height(), COLOR_LINE_BLACK, 2);	
	GUI::CWnd::DrawRectBorder(m_rect.m_left, m_rect.m_top, m_rect.Width(), m_rect.Height(), COLOR_LINE_GRAY, 1);
}

int GUI::CMenu::PopUp(int x, int y, GUI::CWnd* pWnd)
{
	unsigned short cx = 0;
	unsigned short cy = m_menuItemList.size() * m_length[BTN_HEIGHT];

	unsigned short text_cx = 0;
	unsigned short text_cy = 0;
	std::string str;
	for (MENU_ITEM_LIST::const_iterator iter = m_menuItemList.begin(); iter != m_menuItemList.end(); ++iter)
	{
		str = (*iter)->GetCaption();
		TextSize(str.c_str(), str.length(), text_cx, text_cy);
		if (text_cx > cx)
		{
			cx = text_cx;
		}
	}

	cx += 8;

	if (cx < 48)
	{
		cx = 48;
	}

	GUI::CDialog::DoModal(NULL, x, y, cx + BORDER_WIDHT*2, cy + BORDER_WIDHT*2);

	return m_selItem;
}

int GUI::CMenu::AddItem(const char* pStr)
{
	GUI::CButton * pBtn = new GUI::CButton;
	pBtn->SetCaption(pStr, false);

	m_menuItemList.push_back(pBtn);

	m_itemDataList.push_back(0);

	return (m_menuItemList.size() - 1);
}

void GUI::CMenu::SetItemData(int item, LONGLONG data)
{
	if (item < m_itemDataList.size())
	{
		m_itemDataList[item] = static_cast<unsigned long>(data);
	} 
	else
	{
		assert(false);
	}
}

LONGLONG GUI::CMenu::GetItemData(int item) const
{
	if (item < m_itemDataList.size())
	{
		return m_itemDataList[item];
	} 
	else
	{
		assert(false);
		return -1;
	}
}

void GUI::CMenu::DelteItem(int item)
{
	if (item < m_menuItemList.size())
	{
		GUI::CButton * pBtn = m_menuItemList.at(item);
		if (NULL != pBtn)
		{
			//先设置其数据为零，防止未弹出过该Menu，但是被设置过数值，导致析构时断言的错误。
			pBtn->SetData(0);
			delete pBtn;
		}

		m_menuItemList.erase(m_menuItemList.begin() + item);

		m_itemDataList.erase(m_itemDataList.begin() + item);
	}
}

void GUI::CMenu::DeleteAllItem()
{
	for (MENU_ITEM_LIST::iterator iter = m_menuItemList.begin(); iter != m_menuItemList.end(); ++iter)
	{
		if (NULL != (*iter))
		{
			//先设置其数据为零，防止未弹出过该Menu，但是被设置过数值，导致析构时断言的错误。
			(*iter)->SetData(0);
			delete (*iter);
			*iter = NULL;
		}
	}

	m_menuItemList.clear();
	m_itemDataList.clear();
}

unsigned short GUI::CMenu::GetWidth()
{
	unsigned short cx = 0;

	unsigned short text_cx = 0;
	unsigned short text_cy = 0;
	std::string str;
	for (MENU_ITEM_LIST::const_iterator iter = m_menuItemList.begin(); iter != m_menuItemList.end(); ++iter)
	{
		str = (*iter)->GetCaption();
		TextSize(str.c_str(), str.length(), text_cx, text_cy);
		if (text_cx > cx)
		{
			cx = text_cx;
		}
	}

	cx += 8;

	if (cx < 48)
	{
		cx = 48;
	}

	return cx + BORDER_WIDHT*2;
}

unsigned short GUI::CMenu::GetHeight()
{
	return m_menuItemList.size() * m_length[BTN_HEIGHT] + BORDER_WIDHT*2;
}

BEGIN_MSG_MAP(GUI::CMenu, GUI::CDialog)
	ON_MSG_CTRL_RANGE(m_menuItemList[0]->GetID(), m_menuItemList[m_menuItemList.size() - 1]->GetID(), KEY_VIRTUAL_ENTER, OnClickItem)
END_MSG_MAP()

unsigned long GUI::CMenu::OnClickItem(unsigned long ctrlID)
{
	m_selItem = ctrlID - m_menuItemList[0]->GetID();
	EndDialog(DLG_OK);
	return KEY_VIRTUAL_MSG_STOP;
}

void GUI::CMenu::OnOutOfDlg(unsigned long keyID)
{
	GUI::CDialog::OnOutOfDlg(keyID);

	if (KEY_VIRTUAL_ENTER == keyID)
	{
		EndDialog(DLG_ESC);
	}
}

