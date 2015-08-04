#ifndef _BUTTON_H_
#define _BUTTON_H_
#include <map>
#include "Wnd.h"
#include "Image.h"

namespace GUI
{
	class CButton : public GUI::CWnd
	{
	public:
		CButton();
		virtual ~CButton();

		virtual bool Create( unsigned long ctrlID, 
			unsigned long x, 
			unsigned long y,
			unsigned long cx, //如果自定义图片，无效
			unsigned long cy, //无效
			CWnd* pParent,
			int xFocus,
			int yFocus,
			const char* pCaption = NULL,
			unsigned long style = WND_CHILD | WND_VISIBLE | WND_DEFAULT_BK);

		void SetImage(PAINT_STATUS status, BITMAP_ID id, bool bRefresh);
		BITMAP_ID GetImage(PAINT_STATUS status) const;

		void SetBkColor(unsigned color, bool bRefresh = false);
		void SetBkFocusColor(unsigned color, bool bRefresh = false);
		void SetMark(bool bMark, unsigned color = COLOR_BTN_DEFAULT_MARK, bool bRefresh = false);

		void SetAlignText(ALIGN_TEXT align) { m_alignText = align; }
		void DrawBorder(bool bDraw) { m_bDrawBorder = bDraw; }

	protected:
		virtual void OnPaint(PAINT_STATUS status);

	protected:
		typedef std::map<PAINT_STATUS, BITMAP_ID> BTN_IMAGE;
		BTN_IMAGE m_statusImage;
		unsigned long m_color;
		unsigned long m_color_focus;
		
		bool m_bMark;
		unsigned long m_markColor;

		ALIGN_TEXT m_alignText;
		bool m_bDrawBorder;
	};
}

#endif
