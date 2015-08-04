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

		void SetMaxLen(int len) { m_inputEdit.SetMaxStringLen(len); }//��ʾ��������ַ�������
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
			SHIFT_MODE_COUNT,//shiftģʽ��Ŀ
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

		GUI::CEditCtrl m_inputEdit;//������ʾ��
		GUI::CButton m_btnNum[10];//���ְ���
		GUI::CButton m_btnBack;//�˸��
		GUI::CButton m_btnShift;//������
		GUI::CButton m_btnEnter;		

		SHIFT_MODE m_curShiftMode;

		bool m_bShowPinYin;				//ƴ�����뷨�Ƿ�����
		CPinYinInputCtrl m_pinYinCtrl;	//ƴ�����뷨
		
		struct KEY_INFO 
		{
			GUI::CButton* pButton;
			int id;
			int row; //�к�
			int col; //�к�
			int size;//ռ�еĵ�Ԫ����
			KEY_TYPE type;//��������
			char value[SHIFT_MODE_COUNT];//������Ӧ���ַ�����Ӧ��shiftģʽ
			char str[16];//�ַ���,���ư�ť��Ч
		};

		static KEY_INFO s_keyInfoArray[];
		static unsigned long s_curLanguage;
	
	private:
		
	};
}

#endif

