#include "KeyboardNumDlg.h"

using namespace GUI;

GUI::CKeyboardNumDlg::CKeyboardNumDlg()
: m_value(-1)
{
	m_ctrlType = GUI::WND_TYPE_KEYBOARD_NUM;
}

GUI::CKeyboardNumDlg::~CKeyboardNumDlg()
{

}

void GUI::CKeyboardNumDlg::OnInitial()
{
	m_style |= WND_NO_TITLE;
	GUI::CDialog::OnInitial();
	/*SetImage(BMP_DLG_KEYBOARD_NUM, false);*/

	unsigned long x_offset = NUM_X_OFFSET;
	unsigned long y_offset = NUM_Y_OFFSET;
	unsigned long x = x_offset;
	unsigned long y = y_offset;

	m_inputEdit.Create(CTRLID_KEYBORAD_BTN_BEGIN+100,x,y,NUM_BTN_WIDTH*3+x_offset*2-1,m_length[EDIT_HEIGHT],this,0,0);
	m_inputEdit.SetEnable(false, false);
	m_inputEdit.SetCurosrMode(GUI::CEditCtrl::CURSOR_MODE_ALWAYS);
	
	y += m_length[EDIT_HEIGHT]+y_offset;

	char strBtn[2] = {0};
	//1 ～ 9
	y = y-NUM_BTN_HEIGHT-y_offset;
	for (int i = 0; i < 9; ++i)
	{
		if( i%3==0 )
		{
			x = x_offset;
			y += NUM_BTN_HEIGHT + y_offset;
		}

		strBtn[0] = '1' + i;
		m_btnNum[i + 1].Create(CTRLID_KEYBORAD_BTN_BEGIN + 1 + i, x, y, NUM_BTN_WIDTH, NUM_BTN_HEIGHT, this, i%3, i/3+1, strBtn, WND_CHILD | WND_VISIBLE | WND_NO_IMAGE_BK);
		m_btnNum[i + 1].SetData(strBtn[0]);//设置按键数据

		x += NUM_BTN_WIDTH + x_offset;
	}

	x = x_offset;
	y += NUM_BTN_HEIGHT + y_offset;

	//退格
	m_btnBack.Create(CTRLID_KEYBORAD_BTN_BACK, x, y, NUM_BTN_WIDTH, NUM_BTN_HEIGHT, this, 0, 4, "C", WND_CHILD | WND_VISIBLE | WND_NO_IMAGE_BK);

	x += NUM_BTN_WIDTH + x_offset;

	//0
	m_btnNum[0].Create(CTRLID_KEYBORAD_BTN_BEGIN, x, y, NUM_BTN_WIDTH, NUM_BTN_HEIGHT, this, 1, 4, "0", WND_CHILD | WND_VISIBLE | WND_NO_IMAGE_BK);
	m_btnNum[0].SetData('0');//设置按键数据

	x += NUM_BTN_WIDTH + x_offset;

	//OK
	m_btnOk.Create(CTRLID_DEFAULT_BTN_OK, x, y, NUM_BTN_WIDTH, NUM_BTN_HEIGHT, this, 2, 4, "OK", WND_CHILD | WND_VISIBLE | WND_NO_IMAGE_BK);

	if( m_value > 0 )
	{
		char str[32];
		snprintf(str, 32, "%d", m_value);
		m_inputEdit.SetCaption(str,false);
	}
	else
	{
		m_inputEdit.SetCaption("",false);
	}
}

void GUI::CKeyboardNumDlg::SetMaxStringLen(int maxLen)
{
	m_inputEdit.SetMaxStringLen(maxLen);
}

int GUI::CKeyboardNumDlg::GetMaxStringLen() const
{
	return m_inputEdit.GetMaxStringLen();
}

void GUI::CKeyboardNumDlg::SetValue(int value,bool bRefresh)
{
	m_value = value;

	if(bRefresh)
	{
		char str[32];
		snprintf(str, 32, "%d", m_value);
		m_inputEdit.SetCaption(str,bRefresh);
	}
}

BEGIN_MSG_MAP(GUI::CKeyboardNumDlg, GUI::CDialog)
	ON_MSG(CTRLID_NULL, KEY_VIRTUAL_DEC, OnDec)
	ON_MSG(m_btnBack.GetID(), KEY_VIRTUAL_ENTER, OnDec)
	ON_MSG(m_btnOk.GetID(),KEY_VIRTUAL_ENTER,OnOK)
	ON_MSG_CTRL_RANGE(CTRLID_KEYBORAD_BTN_BEGIN, CTRLID_KEYBORAD_BTN_BEGIN + 10, KEY_VIRTUAL_ENTER, OnBtn)
	ON_MSG_CTRL_RANGE(CTRLID_KEYBORAD_BTN_BEGIN, CTRLID_KEYBORAD_BTN_BEGIN + 10, KEY_VIRTUAL_DBCLICK, OnBtn)
	ON_MSG_KEY_RANGE(CTRLID_NULL, KEY_VIRTUAL_0, KEY_VIRTUAL_9, OnNumKey)
END_MSG_MAP()

unsigned long GUI::CKeyboardNumDlg::OnDec()
{
	m_inputEdit.DeleteChar();
	return KEY_VIRTUAL_MSG_CONTINUE;
}

unsigned long GUI::CKeyboardNumDlg::OnBtn(unsigned long ctrlID)
{
	int index = ctrlID - CTRLID_KEYBORAD_BTN_BEGIN;
	assert(index < 10);

	char ch = static_cast<char>(m_btnNum[index].GetData());
	m_inputEdit.AddChar(ch);
	return KEY_VIRTUAL_MSG_CONTINUE;
}

unsigned long CKeyboardNumDlg::OnNumKey(unsigned long keyID)
{
	char ch = static_cast<char>(keyID - KEY_VIRTUAL_0) + '0';
	m_inputEdit.AddChar(ch);
	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long GUI::CKeyboardNumDlg::OnOK()
{
	m_value = atoi(m_inputEdit.GetCaption().c_str());
	return CDialog::OnOK();
}

void GUI::CKeyboardNumDlg::OnOutOfDlg(unsigned long keyID)
{
	GUI::CDialog::OnOutOfDlg(keyID);

	if (KEY_VIRTUAL_ENTER == keyID)
	{
		m_value = atoi(m_inputEdit.GetCaption().c_str());
		EndDialog(DLG_OK);
	}
}
//////////////////////////////////////////////////////////////////////////
