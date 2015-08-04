#ifndef _STATIC_H_
#define _STATIC_H_
#include "Wnd.h"

namespace GUI
{
	class CStatic : public GUI::CWnd
	{
	public:
		CStatic();
		virtual ~CStatic();

		virtual bool Create( unsigned long ctrlID, unsigned long x, unsigned long y, unsigned long cx, \
			unsigned long cy, CWnd* pParent, const char* pCaption = NULL);

		enum LINE_STYlE
		{
			NO_LINE,
			HORZ_LINE,
			VERT_LINE,
		};

		void SetLineStyle(LINE_STYlE style) { m_lineStyle = style; }
		void SetAlignText(ALIGN_TEXT align) { m_alignText = align; }

		//bAllRect表示十分刷新整个指定区域，否表示只是刷新字符串对应的宽度。
		void SetBKColor(unsigned long color, bool bAllRect = true) { m_bkColor = color; m_bAllRect = bAllRect;}
		void SetTextColor(unsigned long color) { m_textColor = color; }

	protected:
		virtual void OnPaint(PAINT_STATUS status);

		LINE_STYlE m_lineStyle;
		ALIGN_TEXT m_alignText;

		unsigned long m_bAllRect;
		unsigned long m_bkColor;
		unsigned long m_textColor;

		enum { INVALID_COLOR = -1};

	private:
	};
}

#endif

