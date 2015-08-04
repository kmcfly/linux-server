#ifndef _NUM_EDIT_CTRL_H_
#define _NUM_EDIT_CTRL_H_

#include "EditCtrl.h"

namespace GUI
{
	class CNumEditCtrl : public GUI::CEditCtrl
	{
	public:
		CNumEditCtrl();
		virtual ~CNumEditCtrl();

		virtual bool Create( unsigned long ctrlID, 
			unsigned long x, 
			unsigned long y,
			unsigned long cx, 
			unsigned long cy,
			CWnd* pParent,
			int xFocus,
			int yFocus,
			bool bVisible = true,
			bool bBorder = true,
			bool bFillEmpty = false);

		bool SetValue(int value, bool bRefresh = true);
		int  GetValue() const { return m_value; }
		void SetRange(int min, int max);

		void EnableAddDec(bool enable = false);

		void Dump();

	protected:
		virtual void OnInitial();
		virtual unsigned long OnEnter();

	protected:
		DECLARE_MSG_MAP()
		unsigned long OnKeyAdd();
		unsigned long OnKeyDec();
		unsigned long OnNumKey(unsigned long keyID);

		int GetDigit(int x) const;//获得整型的位数
		//void AdjustValue();

		int m_value;
		int m_minValue;
		int m_maxValue;
		int m_maxLen;
		bool m_bFillEmpty;
		bool m_bEnAddDec;
	private:
	};
}

#endif

