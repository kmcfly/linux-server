
#include "HexNumEditCtrl.h"
#include "KeyboardHexNumDlg.h"

using namespace GUI;

CHexNumEditCtrl::CHexNumEditCtrl() : m_bFillEmpty(false)
, m_minValue(0)
, m_maxValue(65555)
, m_maxLen(4)
, m_bEnAddDec(false)
{
	m_ctrlType = WND_TYPE_HEX_NUM_EDIT;
}

CHexNumEditCtrl::~CHexNumEditCtrl()
{

}

bool CHexNumEditCtrl::Create(unsigned long ctrlID, 
						  unsigned long x, 
						  unsigned long y,
						  unsigned long cx, 
						  unsigned long cy,
						  CWnd* pParent, 
						  int xFocus, 
						  int yFocus, 
						  bool bVisible/* = ture*/,
						  bool bBorder /*= true*/,
						  bool bFillEmpty/* = true*/)
{
	m_bFillEmpty = bFillEmpty;
	return GUI::CEditCtrl::Create(ctrlID, x, y, cx, cy, pParent, xFocus, yFocus, bVisible, NULL, bBorder);
}

void CHexNumEditCtrl::OnInitial()
{
	GUI::CEditCtrl::OnInitial();

	GUI::CEditCtrl::SetMaxStringLen(m_maxLen);
	
	std::string strTemp("");
	SetValue(strTemp, false);
}


bool CHexNumEditCtrl::SetValue(std::string &strValue, bool bRefresh /*= true*/)
{
	if( m_bFillEmpty )
	{
		char str_fill[32]={0};
		int len = strValue.length();
		for (int i = 0; i < m_maxLen - len; i++)
		{
			str_fill[i]='0';
		}
		strcat(str_fill, strValue.c_str());

		m_strValue = str_fill;
		GUI::CEditCtrl::SetCaption(str_fill, bRefresh);
	}
	else
	{
		GUI::CEditCtrl::SetCaption(strValue, bRefresh);
	}

	return true;
}

unsigned long CHexNumEditCtrl::OnEnter()
{
	GUI::CRect rect = GetWindowRect();
	unsigned short top = rect.m_bottom+2;
	unsigned short left = rect.m_left;

	CKeyboardHexNumDlg keyboard;
	GUI::CRect rcKeyboard = keyboard.GetKeyboardRect();
	//如果超出屏幕高度和宽度
	if (top + rcKeyboard.Height() >= (m_height - 40))
	{
		top = rect.m_top - rcKeyboard.Height()-2;
	}

	keyboard.SetMaxStringLen(m_maxLen);
	if( DLG_OK == keyboard.DoModal(this, left, top, rcKeyboard.Width(), rcKeyboard.Height()) )
	{
		std::string strValue;
		keyboard.GetValue(strValue);
		SetValue(strValue, true);
	}

	return KEY_VIRTUAL_MSG_CONTINUE;
}


BEGIN_MSG_MAP(GUI::CHexNumEditCtrl, GUI::CEditCtrl)
	ON_MSG(GetID(), KEY_VIRTUAL_ADD, OnKeyAdd)
	ON_MSG(GetID(), KEY_VIRTUAL_DEC, OnKeyDec)
END_MSG_MAP()


unsigned long CHexNumEditCtrl::OnKeyDec()
{
	if(m_bEnAddDec)
	{
		if (m_strValue.size() > 0)
		{
			m_strValue.resize(m_strValue.size() - 1);
		}
		SetValue(m_strValue, true);
	}
	return KEY_VIRTUAL_MSG_CONTINUE;
}
