#ifndef _PICTURE_CTRL_H_
#define _PICTURE_CTRL_H_

#include "Wnd.h"

namespace GUI
{
	class CPictureCtrl : public GUI::CWnd
	{
	public:
		CPictureCtrl();
		virtual ~CPictureCtrl();

		virtual bool Create( unsigned long ctrlID, unsigned long x, unsigned long y, CWnd* pParent, BITMAP_ID bmpID, bool bVisible = true);

		void SetImage(BITMAP_ID id, bool bRefresh);

	protected:
		virtual void OnPaint(PAINT_STATUS status);
	};
}

#endif

