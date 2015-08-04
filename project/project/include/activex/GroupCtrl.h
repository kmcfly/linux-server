#ifndef _GROUP_H_
#define _GROUP_H_
#include "Wnd.h"

namespace GUI
{
	class CGroupCtrl : public GUI::CWnd
	{
	public:
		CGroupCtrl();
		virtual ~CGroupCtrl();


		virtual bool Create( unsigned long ctrlID, unsigned long x, unsigned long y, unsigned long cx, \
			unsigned long cy, CWnd* pParent, const char* pCaption = NULL, bool bCheck = false);

	protected:
		virtual void OnPaint(PAINT_STATUS status);

	private:
	};
}

#endif//_GROUP_H_

