#include "NumEditCtrl.h"
#include "KeyboardNumDlg.h"

using namespace GUI;

CNumEditCtrl::CNumEditCtrl() : m_value(0)
, m_bFillEmpty(false)
, m_minValue(0)
, m_maxValue(255)
, m_maxLen(3)
, m_bEnAddDec(false)
{
	m_ctrlType = WND_TYPE_NUM_EDIT;
}

CNumEditCtrl::~CNumEditCtrl()
{

}

bool CNumEditCtrl::Create(unsigned long ctrlID, 
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

void CNumEditCtrl::OnInitial()
{
	GUI::CEditCtrl::OnInitial();

	//int len = GetDigit(m_maxValue);
	GUI::CEditCtrl::SetMaxStringLen(m_maxLen);

	SetValue(0, false);
}

void CNumEditCtrl::SetRange(int min, int max)
{
	m_minValue = min; 
	m_maxValue = max;

	int digit = GetDigit(max);
	m_maxLen = digit;
	GUI::CEditCtrl::SetMaxStringLen(m_maxLen);
}

bool CNumEditCtrl::SetValue(int value, bool bRefresh /* = true */)
{
	if( value < m_minValue ) value = m_minValue;
	if( value > m_maxValue ) value = m_maxValue;


	m_value = value;

	char str[32];
	snprintf(str, 32, "%d", value);

	if( m_bFillEmpty )
	{
		char str_fill[32]={0};
		int len = strlen(str);
		for (int i=0;i<m_maxLen-len;i++)
		{
			str_fill[i]='0';
		}
		strcat(str_fill,str);

		GUI::CEditCtrl::SetCaption(str_fill, bRefresh);
	}
	else
	{
		GUI::CEditCtrl::SetCaption(str, bRefresh);
	}

	return true;
}

unsigned long CNumEditCtrl::OnEnter()
{
	GUI::CRect rect = GetWindowRect();
	unsigned short top = rect.m_bottom+2;
	unsigned short left = rect.m_left;

	CKeyboardNumDlg keyboard;
	GUI::CRect rcKeyboard = keyboard.GetKeyboardRect();
	//如果超出屏幕高度和宽度
	if (top + rcKeyboard.Height() >= (m_height - 40))
	{
		top = rect.m_top - rcKeyboard.Height()-2;
	}

	keyboard.SetMaxStringLen(m_maxLen);
	if( DLG_OK == keyboard.DoModal(this, left, top, rcKeyboard.Width(), rcKeyboard.Height()) )
	{
		int value = keyboard.GetValue();

		SetValue(value,true);
	}

	return KEY_VIRTUAL_MSG_CONTINUE;
}

int CNumEditCtrl::GetDigit(int x) const
{
	assert(x >= 0);
	int digit = 0;

	do
	{
		x = (x / 10);
		++ digit;
	}while (0 != x);

	return digit;
}

BEGIN_MSG_MAP(GUI::CNumEditCtrl, GUI::CEditCtrl)
	ON_MSG(GetID(), KEY_VIRTUAL_ADD, OnKeyAdd)
	ON_MSG(GetID(), KEY_VIRTUAL_DEC, OnKeyDec)
	//ON_MSG_KEY_RANGE(CTRLID_NULL, KEY_VIRTUAL_0, KEY_VIRTUAL_9, OnNumKey)
END_MSG_MAP()

unsigned long CNumEditCtrl::OnKeyAdd()
{
	if(m_bEnAddDec)
	{
		int value = m_value;
		if(value < m_maxValue)
		{
			++value;
		}
		else
		{
			value = m_minValue;
		}
		SetValue(value, true);
	}
	return KEY_VIRTUAL_MSG_CONTINUE;
}

unsigned long CNumEditCtrl::OnKeyDec()
{
	if(m_bEnAddDec)
	{
		int value = m_value;
		if(value > m_minValue)
		{
			--value;
		}
		else
		{
			value = m_maxValue;
		}

		SetValue(value, true);
	}
	//else
	//{
	//	if (0 < GetDigit(m_value))
	//	{
	//		unsigned long value = (m_value / 10);
	//		if (m_minValue < value)
	//		{
	//			SetValue(value);	
	//		}
	//		else
	//		{
	//			SetValue(value);
	//		}

	//		return KEY_VIRTUAL_MSG_STOP;
	//	}
	//}

	return KEY_VIRTUAL_MSG_CONTINUE;
}

unsigned long CNumEditCtrl::OnNumKey(unsigned long keyID)
{
	if (GetDigit(m_value) < m_maxLen)
	{
		unsigned long value = (m_value * 10) + (keyID - KEY_VIRTUAL_0);

		if (value < m_maxValue)
		{
			SetValue(value);
			return KEY_VIRTUAL_MSG_STOP;
		}
	}

	return keyID;
}

void CNumEditCtrl::EnableAddDec(bool enable)
{
	m_bEnAddDec = enable;
}

void CNumEditCtrl::Dump()
{
	printf("class name=\"CNumEditCtrl\",id=%d.\n",GetID());
	printf("m_value=%d,m_minValue=%d,m_maxValue=%d,m_maxLen=%d,m_bFillEmpty=%d.\n",
			m_value,m_minValue,m_maxValue,m_maxLen,m_bFillEmpty);
}
