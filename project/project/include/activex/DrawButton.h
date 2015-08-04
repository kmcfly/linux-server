#ifndef _DRAW_BUTTON_H_
#define _DRAW_BUTTON_H_

#include "Button.h"

namespace GUI
{
	class CDrawButton : public GUI::CButton
	{
	public:
		CDrawButton();
		virtual ~CDrawButton();

		enum DRAWBTN_TYPE
		{
			SQUARE,
			TRIANGLE_LEFT,
			TRIANGLE_RIGHT,
			TRIANGLE_UP,
			TRIANGLE_DOWN,
		};

		virtual bool Create( unsigned long ctrlID, 
			unsigned long x, 
			unsigned long y,
			unsigned long cx, //如果自定义图片，无效
			unsigned long cy, //无效
			CWnd* pParent,
			int xFocus,
			int yFocus,
			DRAWBTN_TYPE type = SQUARE);

	protected:
		virtual void OnPaint(PAINT_STATUS status);

		DRAWBTN_TYPE m_drawType;
	};
}

#endif//_DRAW_BUTTON_H_



