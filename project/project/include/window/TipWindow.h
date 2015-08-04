//FileName:	TipWindow.h

#ifndef  __TIP_WINDOW_H__
#define  __TIP_WINDOW_H__

#include "Static.h"
#include <map>
#include "Wnd.h"

namespace GUI
{
class CTipWindow: public GUI::CStatic
{
	public:
		CTipWindow();
		virtual ~CTipWindow();

		void SetShowTime(unsigned long time);
		unsigned long GetShowTime();

		void SetCtrl(CTRLID oldCtrl){m_oldCtrl = oldCtrl;};
		CTRLID GetCtrl(){return m_oldCtrl;};

		void SetRect(int left, int top, int width, int height);
		bool isRectEq(int left, int top, int width, int height);

	private:
		unsigned long m_showTime;

		CTRLID	m_oldCtrl;
		int m_oldLeft;
		int m_oldTop;
		int m_oldWidth;
		int m_oldHeight;
};
}

#endif//__TIP_WINDOW_H__
