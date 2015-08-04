#ifndef _CHECK_CTRL_H_
#define _CHECK_CTRL_H_

#include "Wnd.h"

namespace GUI
{
	class CCheckCtrl : public GUI::CWnd
	{
	public:
		CCheckCtrl();
		virtual ~CCheckCtrl();

		virtual bool Create( unsigned long ctrlID, unsigned long x, unsigned long y, unsigned long cx, unsigned long cy, CWnd* pParent, unsigned long xFocus, unsigned long yFocus);

		virtual void SetCheck(bool bCheck, bool bRefresh = true);
		bool IsCheck() const
		{ return m_bCheck; }
		void SetCaption(const std::string& strCaption, bool bLeft = true, bool bRefresh = true);
	protected:
		virtual void OnPaint(PAINT_STATUS status);
		virtual unsigned long OnCommand (unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);

		bool m_bCheck;
		bool m_bLeftText;
	};
}

#endif
