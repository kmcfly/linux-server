
#ifndef _HEX_NUM_EDIT_CTRL_H_
#define _HEX_NUM_EDIT_CTRL_H_

#include "EditCtrl.h"

namespace GUI
{
	class CHexNumEditCtrl : public CEditCtrl
	{
	public:
		CHexNumEditCtrl();
		virtual ~CHexNumEditCtrl();

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

		bool SetValue(std::string &strValue, bool bRefresh = true);
		void GetValue(std::string &strValue)  const{ strValue = m_strValue; }

	protected:
		virtual void OnInitial();
		virtual unsigned long OnEnter();

		DECLARE_MSG_MAP()
		unsigned long OnKeyDec();

		std::string m_strValue;
		int m_minValue;
		int m_maxValue;
		int m_maxLen;
		bool m_bFillEmpty;
		bool m_bEnAddDec;
	};
}
#endif
