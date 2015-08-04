#ifndef _KEYBOARD_NUM_DLG_H_
#define _KEYBOARD_NUM_DLG_H_

#include "Dialog.h"
#include "EditCtrl.h"

namespace GUI
{
	class CKeyboardNumDlg : public GUI::CDialog
	{
	public:
		CKeyboardNumDlg();
		~CKeyboardNumDlg();

		GUI::CRect GetKeyboardRect() const
		{
			return CRect(0, 0, NUM_BTN_WIDTH*3 + NUM_X_OFFSET*4-1, NUM_BTN_HEIGHT*4+NUM_Y_OFFSET*6-1+m_length[EDIT_HEIGHT]);
		}

		void SetMaxStringLen(int maxLen);
		int GetMaxStringLen() const;

		void SetValue(int value,bool bRefresh=true);
		int GetValue(){ return m_value;}

	protected:
		DECLARE_MSG_MAP()

		virtual void OnInitial();
		virtual void OnOutOfDlg(unsigned long keyID);

		unsigned long OnBtn(unsigned long ctrlID);
		unsigned long OnDec();
		unsigned long OnOK();
		unsigned long OnNumKey(unsigned long keyID);

		GUI::CEditCtrl m_inputEdit;//输入显示框
		GUI::CButton m_btnNum[10];//数字按键
		GUI::CButton m_btnBack;//退格键
		GUI::CButton m_btnOk;

		GUI::CEditCtrl* m_pWndOutPut;

		int		m_value;

		enum BTN_SIZE
		{
			NUM_X_OFFSET   = 5,
			NUM_Y_OFFSET   = 5,
			NUM_BTN_WIDTH = 26,
			NUM_BTN_HEIGHT = 26,
		};
	};
}


#endif //_KEYBOARD_NUM_DLG_H_

