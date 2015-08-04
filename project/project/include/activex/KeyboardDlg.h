#ifndef _KEYBOARD_DLG_H_
#define _KEYBOARD_DLG_H_

#include "Dialog.h"
#include "EditCtrl.h"
#include "PinYinInputCtrl.h"

namespace GUI
{
	class CKeyboardDlg : public GUI::CDialog
	{
	public:
		CKeyboardDlg();
		virtual ~CKeyboardDlg();

		static void SetCurLanguage(unsigned long lang);

		std::string GetText() const { return m_inputEdit.GetCaption(); }
		void SetText(const char* pText) { m_inputEdit.SetCaption(pText, false); }

		void SetMaxLen(int len) { m_inputEdit.SetMaxStringLen(len); }//显示最大输入字符串长度
		void SetPassword(bool bPassword) { m_inputEdit.SetPassword(bPassword); }

		GUI::CRect GetKeyboardRect() const 
		{
			return CRect(0, 0, 434, 190);
		}

	protected:
		DECLARE_MSG_MAP()

		enum KEY_TYPE 
		{
			KEY_TYPE_CHAR,
			KEY_TYPE_CTRL,
		};

		enum SHIFT_MODE
		{
			SHIFT_LOWER,
			SHIFT_CAPS,
			SHIFT_CHINESE_S,
			SHIFT_MODE_COUNT,//shift模式数目
		};

		enum BTN_SIZE
		{
			CHAR_BTN_WIDTH = 28,
			CHAR_BTN_HEIGHT = 26,
		};

		virtual void OnInitial();
		virtual void OnDestroy();

		unsigned long OnInput(unsigned long keyID);
		unsigned long OnBtn(unsigned long ctrlID);
		unsigned long OnDec();
		unsigned long OnShift();
		unsigned long OnPinYinCtrl(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);

		void ShiftBtn(SHIFT_MODE mode, bool bRefresh = true);

		void RefreshPinyinInput(bool bRefresh = true);

		bool IsPurAsciiLetter(char ch);
		bool IsPurAsciiNum(char ch);

		GUI::CEditCtrl m_inputEdit;//输入显示框
		GUI::CButton m_btnNum[10];//数字按键
		GUI::CButton m_btnBack;//退格键
		GUI::CButton m_btnShift;//换档键
		GUI::CButton m_btnEnter;		

		SHIFT_MODE m_curShiftMode;

		bool m_bShowPinYin;				//拼音输入法是否隐藏
		CPinYinInputCtrl m_pinYinCtrl;	//拼音输入法
		
		struct KEY_INFO 
		{
			GUI::CButton* pButton;
			int id;
			int row; //行号
			int col; //列号
			int size;//占有的单元格数
			KEY_TYPE type;//按键类型
			char value[SHIFT_MODE_COUNT];//按键对应的字符，对应于shift模式
			char str[16];//字符串,控制按钮有效
		};

		static KEY_INFO s_keyInfoArray[];
		static unsigned long s_curLanguage;
	
	private:
		
	};
}

#endif

