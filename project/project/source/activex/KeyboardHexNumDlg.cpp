
#include "KeyboardHexNumDlg.h"

using namespace GUI;

CKeyboardHexNumDlg::CKeyboardHexNumDlg()
{
	m_ctrlType = GUI::WND_TYPE_KEYBOARD_HEX_NUM;
}

CKeyboardHexNumDlg::~CKeyboardHexNumDlg()
{

}

void CKeyboardHexNumDlg::OnInitial()
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

	//A
	int nIndex = 10;
	x = x_offset;
	y += NUM_BTN_HEIGHT + y_offset;
	strBtn[0] = 'a';
	m_btnNum[nIndex].Create(CTRLID_KEYBORAD_BTN_BEGIN + 24, x, y, NUM_BTN_WIDTH, NUM_BTN_HEIGHT, this, (nIndex - 1) % 3, (nIndex - 1)/3+1, strBtn, WND_CHILD | WND_VISIBLE | WND_NO_IMAGE_BK);
	m_btnNum[nIndex].SetData(strBtn[0]);//设置按键数据

	//B
	nIndex += 1;
	x += NUM_BTN_WIDTH + x_offset;
	strBtn[0] = 'b';
	m_btnNum[nIndex].Create(CTRLID_KEYBORAD_BTN_BEGIN + 39, x, y, NUM_BTN_WIDTH, NUM_BTN_HEIGHT, this, (nIndex - 1) % 3, (nIndex - 1)/3+1, strBtn, WND_CHILD | WND_VISIBLE | WND_NO_IMAGE_BK);
	m_btnNum[nIndex].SetData(strBtn[0]);//设置按键数据

	//C
	nIndex += 1;
	x += NUM_BTN_WIDTH + x_offset;
	strBtn[0] = 'c';
	m_btnNum[nIndex].Create(CTRLID_KEYBORAD_BTN_BEGIN + 37, x, y, NUM_BTN_WIDTH, NUM_BTN_HEIGHT, this, (nIndex - 1) % 3, (nIndex - 1)/3+1, strBtn, WND_CHILD | WND_VISIBLE | WND_NO_IMAGE_BK);
	m_btnNum[nIndex].SetData(strBtn[0]);//设置按键数据

	//D
	nIndex += 1;
	x = x_offset;
	y += NUM_BTN_HEIGHT + y_offset;
	strBtn[0] = 'd';
	m_btnNum[nIndex].Create(CTRLID_KEYBORAD_BTN_BEGIN + 26, x, y, NUM_BTN_WIDTH, NUM_BTN_HEIGHT, this, (nIndex - 1) % 3, (nIndex - 1)/3+1, strBtn, WND_CHILD | WND_VISIBLE | WND_NO_IMAGE_BK);
	m_btnNum[nIndex].SetData(strBtn[0]);//设置按键数据

	//E
	nIndex += 1;
	x += NUM_BTN_WIDTH + x_offset;
	strBtn[0] = 'e';
	m_btnNum[nIndex].Create(CTRLID_KEYBORAD_BTN_BEGIN + 13, x, y, NUM_BTN_WIDTH, NUM_BTN_HEIGHT, this, (nIndex - 1) % 3, (nIndex - 1)/3+1, strBtn, WND_CHILD | WND_VISIBLE | WND_NO_IMAGE_BK);
	m_btnNum[nIndex].SetData(strBtn[0]);//设置按键数据

	//F
	nIndex += 1;
	x += NUM_BTN_WIDTH + x_offset;
	strBtn[0] = 'f';
	m_btnNum[nIndex].Create(CTRLID_KEYBORAD_BTN_BEGIN + 27, x, y, NUM_BTN_WIDTH, NUM_BTN_HEIGHT, this, (nIndex - 1) % 3, (nIndex - 1)/3+1, strBtn, WND_CHILD | WND_VISIBLE | WND_NO_IMAGE_BK);
	m_btnNum[nIndex].SetData(strBtn[0]);//设置按键数据

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

	if( m_strValue.length() > 0 )
	{
		m_inputEdit.SetCaption(m_strValue, false);
	}
	else
	{
		m_inputEdit.SetCaption("",false);
	}
}

void CKeyboardHexNumDlg::SetMaxStringLen(int maxLen)
{
	m_inputEdit.SetMaxStringLen(maxLen);
}

int CKeyboardHexNumDlg::GetMaxStringLen() const
{
	return m_inputEdit.GetMaxStringLen();
}

void CKeyboardHexNumDlg::SetValue(std::string strValue, bool bRefresh/* = true*/)
{
	m_strValue = strValue;

	if(bRefresh)
	{
		m_inputEdit.SetCaption(m_strValue, bRefresh);
	}
}

BEGIN_MSG_MAP(CKeyboardHexNumDlg, GUI::CDialog)
	ON_MSG(CTRLID_NULL, KEY_VIRTUAL_DEC, OnDec)
	ON_MSG(m_btnBack.GetID(), KEY_VIRTUAL_ENTER, OnDec)
	ON_MSG(m_btnOk.GetID(),KEY_VIRTUAL_ENTER,OnOK)
	ON_MSG_CTRL_RANGE(CTRLID_KEYBORAD_BTN_BEGIN, CTRLID_KEYBORAD_BTN_BEGIN + 10, KEY_VIRTUAL_ENTER, OnBtnNum)
	ON_MSG_CTRL_RANGE(CTRLID_KEYBORAD_BTN_BEGIN, CTRLID_KEYBORAD_BTN_BEGIN + 10, KEY_VIRTUAL_DBCLICK, OnBtnNum)
	ON_MSG_CTRL_RANGE(CTRLID_KEYBORAD_BTN_BEGIN + 11, CTRLID_KEYBORAD_BTN_BEGIN + 40, KEY_VIRTUAL_ENTER, OnBtnChar)
	ON_MSG_CTRL_RANGE(CTRLID_KEYBORAD_BTN_BEGIN + 11, CTRLID_KEYBORAD_BTN_BEGIN + 40, KEY_VIRTUAL_DBCLICK, OnBtnChar)
	ON_MSG_KEY_RANGE(CTRLID_NULL, KEY_VIRTUAL_0, KEY_VIRTUAL_9, OnNumKey)
END_MSG_MAP()

unsigned long CKeyboardHexNumDlg::OnDec()
{
	m_inputEdit.DeleteChar();
	return KEY_VIRTUAL_MSG_CONTINUE;
}

unsigned long CKeyboardHexNumDlg::OnBtnNum(unsigned long ctrlID)
{
	int index = ctrlID - CTRLID_KEYBORAD_BTN_BEGIN;
	assert(index < 10);

	char ch = static_cast<char>(m_btnNum[index].GetData());
	m_inputEdit.AddChar(ch);
	return KEY_VIRTUAL_MSG_CONTINUE;
}

unsigned long CKeyboardHexNumDlg::OnBtnChar(unsigned long ctrlID)
{
	int nTmp = ctrlID - CTRLID_KEYBORAD_BTN_BEGIN;
	assert((nTmp > 10) && (nTmp < 40));
	
	int nIndex = 0;
	switch (nTmp)
	{
		case 24:	//a
			nIndex = 10;
		break;
		case 39:	//b
			nIndex = 11;
			break;
		case 37:	//c
			nIndex = 12;
			break;
		case 26:	//d
			nIndex = 13;
			break;
		case 13:	//e
			nIndex = 14;
			break;
		case 27:	//f
			nIndex = 15;
			break;
		default:
			break;
	}

	char ch = static_cast<char>(m_btnNum[nIndex].GetData());
	m_inputEdit.AddChar(ch);
	return KEY_VIRTUAL_MSG_CONTINUE;
}

unsigned long CKeyboardHexNumDlg::OnNumKey(unsigned long keyID)
{
	char ch = static_cast<char>(keyID - KEY_VIRTUAL_0) + '0';
	m_inputEdit.AddChar(ch);
	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CKeyboardHexNumDlg::OnOK()
{
	m_strValue = m_inputEdit.GetCaption();
	return CDialog::OnOK();
}

void CKeyboardHexNumDlg::OnOutOfDlg(unsigned long keyID)
{
	GUI::CDialog::OnOutOfDlg(keyID);

	if (KEY_VIRTUAL_ENTER == keyID)
	{
		m_strValue = m_inputEdit.GetCaption();
		EndDialog(DLG_OK);
	}
}

//////////////////////////////////////////////////////////////////////////
