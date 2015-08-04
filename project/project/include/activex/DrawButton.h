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
			unsigned long cx, //����Զ���ͼƬ����Ч
			unsigned long cy, //��Ч
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



