
#include "PinYinInputCtrl.h"

using namespace GUI;

CPinYinInput* CPinYinInputCtrl::m_pPinYinInput = CPinYinInput::Instance();

CPinYinInputCtrl::CPinYinInputCtrl() : m_bInPinYin(false)
{
	m_ctrlType = WND_TYPE_INPUT_PINYIN;
}

CPinYinInputCtrl::~CPinYinInputCtrl()
{

}

bool CPinYinInputCtrl::Create(unsigned long ctrlID, unsigned long x, unsigned long y, unsigned long cx, unsigned long cy, CWnd* pParent, int xFocus, int yFocus, bool bVisible)
{
	unsigned long style = WND_CHILD;
	if (bVisible)
	{
		style |= WND_VISIBLE;
	}

	if (GUI::CWnd::Create(ctrlID, x, y, cx, cy, pParent, style, xFocus, yFocus))
	{
		return true;
	}
	else
	{
		return false;
	}
}

void CPinYinInputCtrl::OnInitial()
{
	GUI::CWnd::OnInitial();

	unsigned short x = 0, y = 0;
	unsigned short width = m_rect.Width();
	unsigned short height = m_rect.Height();
	unsigned short btnWidth = 24;

	m_editPinYin.Create(GetRandWndID(), x, y, 85 + 1, height, this, 0, 0);
	m_editPinYin.SetEnable(false, false);
	m_editPinYin.SetMaxStringLen(7);

	x += 85;
	m_editCnChar.Create(GetRandWndID(), x, y, width - x - 2 * btnWidth - 1, height, this, 0, 0);
	m_editCnChar.SetEnable(false, false);
	m_editCnChar.SetMaxStringLen(64);

	x = width - 2 * btnWidth - 1;
	m_btnPrev.Create(GetRandWndID(), x, y + 1, btnWidth, height, this, 0, 0);
	m_btnPrev.SetImage(PAINT_STATUS_NORMAL, BMP_COMBO_BTN_LEFT, false);
	m_btnPrev.SetImage(PAINT_STATUS_FOCUS, BMP_COMBO_BTN_LEFT_FOCUS, false);
	m_btnPrev.SetImage(PAINT_STATUS_DISABLE, BMP_COMBO_BTN_LEFT_DISABLE, false);

	x += btnWidth;
	m_btnNext.Create(GetRandWndID(), x, y + 1, btnWidth, height, this, 0, 0);
	m_btnNext.SetImage(PAINT_STATUS_NORMAL, BMP_COMBO_BTN_RIGHT, false);
	m_btnNext.SetImage(PAINT_STATUS_FOCUS, BMP_COMBO_BTN_RIGHT_FOCUS, false);
	m_btnNext.SetImage(PAINT_STATUS_DISABLE, BMP_COMBO_BTN_RIGHT_DISABLE, false);
}

void CPinYinInputCtrl::OnPaint(PAINT_STATUS status)
{
	GUI::CWnd::OnPaint(status);

	GUI::CWnd::DrawRect(m_rect.m_left, m_rect.m_top, m_rect.Width(), m_rect.Height(), COLOR_EDIT);
	GUI::CWnd::DrawRectBorder(m_rect.m_left, m_rect.m_top, m_rect.Width(), m_rect.Height(), COLOR_LINE_WHITE);
}

bool CPinYinInputCtrl::SetPinYin(char ch, bool bAdd, bool bRefresh)
{
	bool bRead = false;

	if(bAdd)
	{
		if(m_editPinYin.GetCaption().length() < 7)
		{
			m_editPinYin.AddChar(ch);
			bRead = true;
		}
	}
	else
	{
		m_editPinYin.DeleteChar();
		bRead = true;
	}

	if(m_editPinYin.GetCaption().length() > 0)
	{
		if( bRead )
		{
			m_pPinYinInput->SetChar(m_editPinYin.GetCaption().c_str(), m_editPinYin.GetCaption().length());

			char str[64] = {0};
			m_pPinYinInput->GetText(str);
			m_editCnChar.SetCaption(str, bRefresh);

			m_bInPinYin = true;
		}
	}
	else
	{
		m_pPinYinInput->ClearTextBuff();

		m_editCnChar.SetCaption("", bRefresh);

		m_bInPinYin = false;
	}

	return m_bInPinYin;
}

void CPinYinInputCtrl::ClearPinYin(bool bRefresh)
{
	if( m_bInPinYin )
	{
		m_editPinYin.SetCaption("", bRefresh);
		m_editCnChar.SetCaption("", bRefresh);

		m_bInPinYin = false;
	}

	m_pPinYinInput->ClearTextBuff();
}

std::string CPinYinInputCtrl::GetPinYin(const char ch)
{
	if(m_bInPinYin)
	{
		m_pPinYinInput->SetNumber(ch);

		char str[64] = {0};
		m_pPinYinInput->GetSingleText(str);

		return std::string(str);
	}
	else
	{
		return "";
	}
}

bool CPinYinInputCtrl::IsInPinYin()
{
	return m_bInPinYin;
}

int CPinYinInputCtrl::GetHitNum(unsigned long xPos, unsigned long yPos)
{
	int ret = -1;

	std::string strCnChar = m_editCnChar.GetCaption();
	GUI::CRect rect = m_editCnChar.GetWindowRect();
	if((strCnChar.length() > 0) && (m_width > xPos) && (m_height > yPos) && rect.PtInRect(xPos, yPos))
	{
		std::string strTest = strCnChar.substr(0, strCnChar.find(0x20) + 2);

		unsigned short cx = 0, cy = 0;
		GUI::CWnd::TextSize(strTest.c_str(), strTest.length(), cx, cy);

		if(cx > 0)
		{
			ret = (xPos - rect.m_left - m_length[EDIT_XPOS]) / cx + 1;
		}
	}

	return ret;
}

BEGIN_MSG_MAP(CPinYinInputCtrl, GUI::CWnd)
	ON_MSG(m_btnPrev.GetID(), KEY_VIRTUAL_ENTER, OnBtnPrev)
	ON_MSG(m_btnNext.GetID(), KEY_VIRTUAL_ENTER, OnBtnNext)
END_MSG_MAP()

unsigned long CPinYinInputCtrl::OnBtnPrev()
{
	if(m_bInPinYin && m_pPinYinInput->GetPageUp())
	{
		char str[64] = {0};
		m_pPinYinInput->GetText(str);
		m_editCnChar.SetCaption(str, true);
	}

	return KEY_VIRTUAL_MSG_CONTINUE;
}

unsigned long CPinYinInputCtrl::OnBtnNext()
{
	if(m_bInPinYin && m_pPinYinInput->GetPageDown())
	{
		char str[64] = {0};
		m_pPinYinInput->GetText(str);
		m_editCnChar.SetCaption(str, true);
	}

	return KEY_VIRTUAL_MSG_CONTINUE;
}

