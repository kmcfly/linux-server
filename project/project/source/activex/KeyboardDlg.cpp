#include "KeyboardDlg.h"

using namespace GUI;

GUI::CKeyboardDlg::KEY_INFO GUI::CKeyboardDlg::s_keyInfoArray[] = 
{
	//第一行
	//pButton, id,                          row,col,size, type,      value,      str
	{NULL, CTRLID_KEYBORAD_BTN_BEGIN + 1, 0, 0, 1, KEY_TYPE_CHAR, {'1', '!', '1'}, ""},
	{NULL, CTRLID_KEYBORAD_BTN_BEGIN + 2, 0, 1, 1, KEY_TYPE_CHAR, {'2', '@', '2'}, ""},
	{NULL, CTRLID_KEYBORAD_BTN_BEGIN + 3, 0, 2, 1, KEY_TYPE_CHAR, {'3', '#', '3'}, ""},
	{NULL, CTRLID_KEYBORAD_BTN_BEGIN + 4, 0, 3, 1, KEY_TYPE_CHAR, {'4', '$', '4'}, ""},
	{NULL, CTRLID_KEYBORAD_BTN_BEGIN + 5, 0, 4, 1, KEY_TYPE_CHAR, {'5', '%', '5'}, ""},
	{NULL, CTRLID_KEYBORAD_BTN_BEGIN + 6, 0, 5, 1, KEY_TYPE_CHAR, {'6', '^', '6'}, ""},
	{NULL, CTRLID_KEYBORAD_BTN_BEGIN + 7, 0, 6, 1, KEY_TYPE_CHAR, {'7', '&', '7'}, ""},
	{NULL, CTRLID_KEYBORAD_BTN_BEGIN + 8, 0, 7, 1, KEY_TYPE_CHAR, {'8', '*', '8'}, ""},
	{NULL, CTRLID_KEYBORAD_BTN_BEGIN + 9, 0, 8, 1, KEY_TYPE_CHAR, {'9', '(', '9'}, ""},
	{NULL, CTRLID_KEYBORAD_BTN_BEGIN + 10, 0, 9, 1, KEY_TYPE_CHAR, {'0', ')', '0'}, ""},
	//第二行
	//pButton, id,                          row,col,size, type,      value,      str
	{NULL, CTRLID_KEYBORAD_BTN_BEGIN + 11, 1, 0, 1, KEY_TYPE_CHAR, {'q', 'Q', 'q'}, ""},
	{NULL, CTRLID_KEYBORAD_BTN_BEGIN + 12, 1, 1, 1, KEY_TYPE_CHAR, {'w', 'W', 'w'}, ""},
	{NULL, CTRLID_KEYBORAD_BTN_BEGIN + 13, 1, 2, 1, KEY_TYPE_CHAR, {'e', 'E', 'e'}, ""},
	{NULL, CTRLID_KEYBORAD_BTN_BEGIN + 14, 1, 3, 1, KEY_TYPE_CHAR, {'r', 'R', 'r'}, ""},
	{NULL, CTRLID_KEYBORAD_BTN_BEGIN + 15, 1, 4, 1, KEY_TYPE_CHAR, {'t', 'T', 't'}, ""},
	{NULL, CTRLID_KEYBORAD_BTN_BEGIN + 16, 1, 5, 1, KEY_TYPE_CHAR, {'y', 'Y', 'y'}, ""},
	{NULL, CTRLID_KEYBORAD_BTN_BEGIN + 17, 1, 6, 1, KEY_TYPE_CHAR, {'u', 'U', 'u'}, ""},
	{NULL, CTRLID_KEYBORAD_BTN_BEGIN + 18, 1, 7, 1, KEY_TYPE_CHAR, {'i', 'I', 'i'}, ""},
	{NULL, CTRLID_KEYBORAD_BTN_BEGIN + 19, 1, 8, 1, KEY_TYPE_CHAR, {'o', 'O', 'o'}, ""},
	{NULL, CTRLID_KEYBORAD_BTN_BEGIN + 20, 1, 9, 1, KEY_TYPE_CHAR, {'p', 'P', 'p'}, ""},
	{NULL, CTRLID_KEYBORAD_BTN_BEGIN + 21, 1, 10, 1, KEY_TYPE_CHAR, {'[', '{', '['}, ""},
	{NULL, CTRLID_KEYBORAD_BTN_BEGIN + 22, 1, 11, 1, KEY_TYPE_CHAR, {']', '}', ']'}, ""},
	{NULL, CTRLID_KEYBORAD_BTN_BEGIN + 23, 1, 12, 1, KEY_TYPE_CHAR, {'\\', '|', '\\'}, ""},
	
	//第三行
	//pButton, id,                          row,col,size, type,      value,      str
	{NULL, CTRLID_KEYBORAD_BTN_BEGIN + 24, 2, 0, 1, KEY_TYPE_CHAR, {'a', 'A', 'a'}, ""},
	{NULL, CTRLID_KEYBORAD_BTN_BEGIN + 25, 2, 1, 1, KEY_TYPE_CHAR, {'s', 'S', 's'}, ""},
	{NULL, CTRLID_KEYBORAD_BTN_BEGIN + 26, 2, 2, 1, KEY_TYPE_CHAR, {'d', 'D', 'd'}, ""},
	{NULL, CTRLID_KEYBORAD_BTN_BEGIN + 27, 2, 3, 1, KEY_TYPE_CHAR, {'f', 'F', 'f'}, ""},
	{NULL, CTRLID_KEYBORAD_BTN_BEGIN + 28, 2, 4, 1, KEY_TYPE_CHAR, {'g', 'G', 'g'}, ""},
	{NULL, CTRLID_KEYBORAD_BTN_BEGIN + 29, 2, 5, 1, KEY_TYPE_CHAR, {'h', 'H', 'h'}, ""},
	{NULL, CTRLID_KEYBORAD_BTN_BEGIN + 30, 2, 6, 1, KEY_TYPE_CHAR, {'j', 'J', 'j'}, ""},
	{NULL, CTRLID_KEYBORAD_BTN_BEGIN + 31, 2, 7, 1, KEY_TYPE_CHAR, {'k', 'K', 'k'}, ""},
	{NULL, CTRLID_KEYBORAD_BTN_BEGIN + 32, 2, 8, 1, KEY_TYPE_CHAR, {'l', 'L', 'l'}, ""},
	{NULL, CTRLID_KEYBORAD_BTN_BEGIN + 33, 2, 9, 1, KEY_TYPE_CHAR, {';', ':', ';'}, ""},
	{NULL, CTRLID_KEYBORAD_BTN_BEGIN + 34, 2, 10, 1, KEY_TYPE_CHAR, {'\'', '"', '\''}, ""},
	
	//第四行
	//pButton, id,                          row,col,size, type,      value,      str
	{NULL, CTRLID_KEYBORAD_BTN_BEGIN + 35, 3, 2, 1, KEY_TYPE_CHAR, {'z', 'Z', 'z'}, ""},
	{NULL, CTRLID_KEYBORAD_BTN_BEGIN + 36, 3, 3, 1, KEY_TYPE_CHAR, {'x', 'X', 'x'}, ""},
	{NULL, CTRLID_KEYBORAD_BTN_BEGIN + 37, 3, 4, 1, KEY_TYPE_CHAR, {'c', 'C', 'c'}, ""},
	{NULL, CTRLID_KEYBORAD_BTN_BEGIN + 38, 3, 5, 1, KEY_TYPE_CHAR, {'v', 'V', 'v'}, ""},
	{NULL, CTRLID_KEYBORAD_BTN_BEGIN + 39, 3, 6, 1, KEY_TYPE_CHAR, {'b', 'B', 'b'}, ""},
	{NULL, CTRLID_KEYBORAD_BTN_BEGIN + 40, 3, 7, 1, KEY_TYPE_CHAR, {'n', 'N', 'n'}, ""},
	{NULL, CTRLID_KEYBORAD_BTN_BEGIN + 41, 3, 8, 1, KEY_TYPE_CHAR, {'m', 'M', 'm'}, ""},
	{NULL, CTRLID_KEYBORAD_BTN_BEGIN + 42, 3, 9, 1, KEY_TYPE_CHAR, {',', '<', ','}, ""},
	{NULL, CTRLID_KEYBORAD_BTN_BEGIN + 43, 3, 10, 1, KEY_TYPE_CHAR, {'.', '>', '.'}, ""},
	{NULL, CTRLID_KEYBORAD_BTN_BEGIN + 44, 3, 11, 1, KEY_TYPE_CHAR, {'/', '?', '/'}, ""},
	{NULL, CTRLID_KEYBORAD_BTN_BEGIN + 45, 3, 12, 1, KEY_TYPE_CHAR, {'`', '~', '`'}, ""},
	//第五行
	{NULL, CTRLID_KEYBORAD_BTN_BEGIN + 46, 4, 2, 9, KEY_TYPE_CHAR, {' ', ' ', ' '}, ""},
	{NULL, CTRLID_KEYBORAD_BTN_BEGIN + 47, 4, 11, 1, KEY_TYPE_CHAR, {'-', '_', '-'}, ""},
	{NULL, CTRLID_KEYBORAD_BTN_BEGIN + 48, 4, 12, 1, KEY_TYPE_CHAR, {'=', '+', '='}, ""},
	//功能键
	//pButton, id,                          row,col,size, type,      value,      str

	{NULL, CTRLID_DEFAULT_BTN_OK, 2, 11, 2, KEY_TYPE_CTRL, {0, 0, 0}, "Enter"},
	{NULL, CTRLID_KEYBORAD_BTN_SHIFT, 3, 0, 2, KEY_TYPE_CTRL, {0, 0, 0}, "Shift"},
	{NULL, CTRLID_KEYBORAD_BTN_BACK,  0, 10, 3, KEY_TYPE_CTRL, {0, 0, 0}, "Backspace"},
	{NULL, CTRLID_DEFAULT_BTN_CANCEL, 4, 0, 2, KEY_TYPE_CTRL, {0, 0, 0}, "Esc"},
};

unsigned long GUI::CKeyboardDlg::s_curLanguage = LANGUAGE_ENGLISH;

void GUI::CKeyboardDlg::SetCurLanguage(unsigned long lang)
{
	s_curLanguage = lang;
}

GUI::CKeyboardDlg::CKeyboardDlg() :m_curShiftMode(SHIFT_LOWER)
{
	m_ctrlType = GUI::WND_TYPE_KEYBOARD;
}

GUI::CKeyboardDlg::~CKeyboardDlg()
{

}

void GUI::CKeyboardDlg::OnInitial()
{
	GUI::CDialog::OnInitial();
	unsigned long style = GetStyle();
	SetStyle(style | WND_NO_TITLE);

	m_inputEdit.Create(CTRLID_KEYBORAD_EDIT_INPUT, 5, 5, 424, m_length[EDIT_HEIGHT], this, 0, 0);
	m_inputEdit.SetEnable(false, false);
	m_inputEdit.SetCurosrMode(GUI::CEditCtrl::CURSOR_MODE_ALWAYS);

	int offset_x = 5;
	int offset_y = 5;
	int count = sizeof(s_keyInfoArray) / sizeof(s_keyInfoArray[0]);
	for (int i = 0; i < count; ++i)
	{
		KEY_INFO& key = s_keyInfoArray[i];
		key.pButton = new GUI::CButton;
		int x = key.col * (CHAR_BTN_WIDTH + offset_x) + offset_x;
		int y = key.row * (CHAR_BTN_HEIGHT + offset_y) + 35;

		key.pButton->Create(key.id, 
			x, 
			y, 
			(CHAR_BTN_WIDTH + offset_x)* key.size - offset_x, 
			CHAR_BTN_HEIGHT, 
			this, 
			key.col, 
			key.row + 1, 
			NULL, 
			WND_CHILD|WND_VISIBLE|WND_NO_IMAGE_BK);
	}

	int x = offset_x;
	int y = 4 * (CHAR_BTN_HEIGHT + offset_y) + 35;

	m_pinYinCtrl.Create(GetRandWndID(), x, y, m_rect.Width() - 2 * offset_x - 2, CHAR_BTN_HEIGHT, this, 5, 0, false);
	m_pinYinCtrl.ClearPinYin(false);
	m_bShowPinYin = false;

	ShiftBtn(SHIFT_LOWER, false);
}

void GUI::CKeyboardDlg::OnDestroy()
{
	GUI::CDialog::OnDestroy();

	int count = sizeof(s_keyInfoArray) / sizeof(s_keyInfoArray[0]);
	for (int i = 0; i < count; ++i)
	{
		if (NULL != s_keyInfoArray[i].pButton)
		{
			delete s_keyInfoArray[i].pButton;
			s_keyInfoArray[i].pButton = NULL;
		}
	}
}


BEGIN_MSG_MAP(GUI::CKeyboardDlg, GUI::CDialog)
	ON_MSG_KEY_RANGE(CKeyboardDlg::GetID(), KEY_VIRTUAL_0, KEY_VIRTUAL_9, OnInput)
	ON_MSG(CKeyboardDlg::GetID(), KEY_VIRTUAL_REW, OnDec)
	ON_MSG(CKeyboardDlg::GetID(), KEY_VIRTUAL_DEC, OnDec)
	ON_MSG_CTRL_RANGE(CTRLID_KEYBORAD_BTN_BEGIN, CTRLID_KEYBORAD_BTN_END, KEY_VIRTUAL_ENTER, OnBtn)
	ON_MSG(CTRLID_KEYBORAD_BTN_BACK, KEY_VIRTUAL_ENTER, OnDec)
	ON_MSG_CTRL_RANGE(CTRLID_KEYBORAD_BTN_BEGIN, CTRLID_KEYBORAD_BTN_END, KEY_VIRTUAL_DBCLICK, OnBtn)
	ON_MSG(CTRLID_KEYBORAD_BTN_BACK, KEY_VIRTUAL_DBCLICK, OnDec)
	ON_MSG(CTRLID_KEYBORAD_BTN_SHIFT, KEY_VIRTUAL_ENTER, OnShift)
	ON_MSG_EX(m_pinYinCtrl.GetID(), KEY_VIRTUAL_ENTER, OnPinYinCtrl);
END_MSG_MAP()


unsigned long GUI::CKeyboardDlg::OnInput(unsigned long keyID)
{
	if(SHIFT_CHINESE_S != m_curShiftMode || !m_pinYinCtrl.IsInPinYin())
	{
		m_inputEdit.AddChar('0' + keyID - KEY_VIRTUAL_0);
	}
	else
	{
		std::string strRet = m_pinYinCtrl.GetPinYin('0' + keyID - KEY_VIRTUAL_0);
		if(!strRet.empty())
		{
			m_pinYinCtrl.ClearPinYin(true);
			std::string strTmp = m_inputEdit.GetCaption();
			strTmp += strRet;
			if(strTmp.length() <= m_inputEdit.GetMaxStringLen())
			{
				m_inputEdit.SetCaption(strTmp, true);
			}
		}

		RefreshPinyinInput(true);
	}
	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long GUI::CKeyboardDlg::OnDec()
{
	if(SHIFT_CHINESE_S != m_curShiftMode || !m_pinYinCtrl.IsInPinYin())
	{
		m_inputEdit.DeleteChar();
	}
	else
	{
		m_pinYinCtrl.SetPinYin(0, false, m_bShowPinYin);

		RefreshPinyinInput(true);
	}
	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long GUI::CKeyboardDlg::OnBtn(unsigned long ctrlID)
{
	int count = sizeof(s_keyInfoArray) / sizeof(s_keyInfoArray[0]);
	for (int i = 0; i < count; ++i)
	{
		const KEY_INFO& key = s_keyInfoArray[i];
		if (key.id == ctrlID)
		{
			if(SHIFT_CHINESE_S != m_curShiftMode)
			{
				m_inputEdit.AddChar(key.value[m_curShiftMode]);
				return KEY_VIRTUAL_MSG_STOP;
			}
			else
			{
				if(IsPurAsciiLetter(key.value[m_curShiftMode]))
				{
					m_pinYinCtrl.SetPinYin(key.value[m_curShiftMode], true, m_bShowPinYin);

					RefreshPinyinInput(true);

					return KEY_VIRTUAL_MSG_STOP;
				}
				else if( m_pinYinCtrl.IsInPinYin() && IsPurAsciiNum(key.value[m_curShiftMode]))
				{
					std::string strRet = m_pinYinCtrl.GetPinYin(key.value[m_curShiftMode]);
					if(!strRet.empty())
					{
						m_pinYinCtrl.ClearPinYin(false);
						std::string strTmp = m_inputEdit.GetCaption();
						strTmp += strRet;
						if(strTmp.length() <= m_inputEdit.GetMaxStringLen())
						{
							m_inputEdit.SetCaption(strTmp, false);
						}
					}

					RefreshPinyinInput(true);

					return KEY_VIRTUAL_MSG_STOP;
				}
				else
				{
					m_inputEdit.AddChar(key.value[m_curShiftMode]);
				}

				return KEY_VIRTUAL_MSG_STOP;
			}
		}
	}

	assert(false);//找不到对应的ID
	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long GUI::CKeyboardDlg::OnShift()
{
	if(LANGUAGE_CHINESE_S == static_cast<LANGUAGE>(s_curLanguage))
	{
#if defined(__CUSTOM_BR27__)
		if (SHIFT_LOWER == m_curShiftMode)
		{
			m_curShiftMode = SHIFT_CAPS;
		}
		else
		{
			m_curShiftMode = SHIFT_LOWER;
		}
#else
#if defined(__KR24_IN__) || defined(__KOREAN__)
		if (SHIFT_LOWER == m_curShiftMode)
		{
			m_curShiftMode = SHIFT_CAPS;
		}
		else
		{
			m_curShiftMode = SHIFT_LOWER;
		}
#else
		if (SHIFT_LOWER == m_curShiftMode)
		{
			m_curShiftMode = SHIFT_CAPS;
		}
		else if (SHIFT_CAPS == m_curShiftMode)
		{
			m_curShiftMode = SHIFT_CHINESE_S;
		}

		else if (SHIFT_CHINESE_S == m_curShiftMode)
		{
			m_curShiftMode = SHIFT_LOWER;
		}
#endif

#endif
	}
	else
	{
		if (SHIFT_LOWER == m_curShiftMode)
		{
			m_curShiftMode = SHIFT_CAPS;
		}
		else
		{
			m_curShiftMode = SHIFT_LOWER;
		}
	}

	ShiftBtn(m_curShiftMode, false);

	Repaint();

	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long GUI::CKeyboardDlg::OnPinYinCtrl(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	int hitNum = m_pinYinCtrl.GetHitNum(xPos, yPos);
	if(hitNum > 0 && hitNum < 8)
	{
		// 1 ~ 7
		std::string strRet = m_pinYinCtrl.GetPinYin(0x30 + hitNum);
		if(!strRet.empty())
		{
			m_pinYinCtrl.ClearPinYin(false);
			std::string strTmp = m_inputEdit.GetCaption();
			strTmp += strRet;
			if(strTmp.length() <= m_inputEdit.GetMaxStringLen())
			{
				m_inputEdit.SetCaption(strTmp, false);
			}
		}

		RefreshPinyinInput(true);
	}

	return KEY_VIRTUAL_MSG_STOP;
}

void GUI::CKeyboardDlg::ShiftBtn(SHIFT_MODE mode, bool bRefresh/* = true*/)
{
	static const char UTF8_CHINESE_S[] = {0xE4,0xB8,0xAD,0xE6,0x96,0x87,0x0};

	assert(mode < SHIFT_MODE_COUNT);

	char str[32] = {0};
	int count = sizeof(s_keyInfoArray) / sizeof(s_keyInfoArray[0]);
	for (int i = 0; i < count; ++i)
	{
		KEY_INFO& key = s_keyInfoArray[i];
		assert(NULL != key.pButton);

		if (KEY_TYPE_CHAR == key.type)
		{
			snprintf(str, 32, "%c", key.value[mode]);
			assert(NULL != key.pButton);
			key.pButton->SetCaption(str, bRefresh);
		}
		else
		{
			if( key.id == CTRLID_KEYBORAD_BTN_SHIFT && SHIFT_CHINESE_S == mode)
			{
				std::string caption = UTF8_CHINESE_S/*"中文"*/;
				key.pButton->SetCaption(caption, bRefresh);
			}
			else
			{
				key.pButton->SetCaption(key.str, bRefresh);
			}
		}
	}

	m_curShiftMode = mode;
}

void GUI::CKeyboardDlg::RefreshPinyinInput(bool bRefresh)
{
	bool bInPinYin = m_pinYinCtrl.IsInPinYin();

	if(m_bShowPinYin != bInPinYin)
	{
		m_bShowPinYin = bInPinYin;

		bool bChanged = false;

		////
		if(m_bShowPinYin != m_pinYinCtrl.IsShow())
		{
			bChanged = true;
			m_pinYinCtrl.Show(m_bShowPinYin, false);
		}

		////ESE键
		bool bShowInput = !m_bShowPinYin;
		GUI::CWnd* pWnd = GetCtrl(CTRLID_DEFAULT_BTN_CANCEL);
		if(pWnd && bShowInput != pWnd->IsShow())
		{
			pWnd->Show(bShowInput, false);
		}

		////空格键
		pWnd = GetCtrl(static_cast<GUI::CTRLID>(CTRLID_KEYBORAD_BTN_BEGIN + 46));
		if(pWnd && bShowInput != pWnd->IsShow())
		{
			pWnd->Show(bShowInput, false);
		}


		////-号键
		pWnd = GetCtrl(static_cast<GUI::CTRLID>(CTRLID_KEYBORAD_BTN_BEGIN + 47));
		if(pWnd && bShowInput != pWnd->IsShow())
		{
			pWnd->Show(bShowInput, false);
		}

		////=号键
		pWnd = GetCtrl(static_cast<GUI::CTRLID>(CTRLID_KEYBORAD_BTN_BEGIN + 48));
		if(pWnd && bShowInput != pWnd->IsShow())
		{
			pWnd->Show(bShowInput, false);
		}

		if(bRefresh && bChanged)
		{
			Repaint();
		}
	}
}

bool GUI::CKeyboardDlg::IsPurAsciiLetter(char ch)
{
	if(ch >= 'a' && ch <= 'z')
	{
		return true;
	}

	if(ch >= 'A' && ch <= 'Z')
	{
		return true;
	}

	return false;
}

bool GUI::CKeyboardDlg::IsPurAsciiNum(char ch)
{
	if(ch >= '0' && ch <= '9' )
	{
		return true;
	}

	return false;
}
