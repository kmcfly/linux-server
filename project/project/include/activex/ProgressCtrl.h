#ifndef _PROGRESS_CTRL_H_
#define _PROGRESS_CTRL_H_

#include "Wnd.h"

namespace GUI
{
	class CProgressCtrl : public GUI::CWnd
	{
	public:
		CProgressCtrl();
		virtual ~CProgressCtrl();

		virtual bool Create( unsigned long ctrlID, 
			unsigned long x, 
			unsigned long y,
			unsigned long cx, 
			unsigned long cy,
			CWnd* pParent,
			bool bVisible = true,
			bool bShowPercent = true);

		void SetPercent(int percet, bool bRefresh = true);
		int  GetPercent() const { return m_percent; }

	protected:
		virtual void OnPaint(PAINT_STATUS status);

		int m_percent;
		bool m_bShowPercent;

	private:
	};
}

#endif

