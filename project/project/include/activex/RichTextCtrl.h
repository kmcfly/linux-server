#ifndef _RICH_TEXT_CTRL_H_
#define _RICH_TEXT_CTRL_H_

#include "Wnd.h"
#include <list>

namespace GUI
{
	class CRichTextCtrl : public CWnd
	{
	public:
		CRichTextCtrl();
		~CRichTextCtrl();

		virtual bool Create( unsigned long ctrlID, unsigned long x, unsigned long y, unsigned long cx, unsigned long cy, CWnd* pParent);

		
		void SetText(const std::string& str, bool bRefresh);//设置文本
		void AppendText(const std::string& str, bool bRefresh);//设置添加字符串
		unsigned short GetTextRow();//获取文本行数
		void SetScrollShow(bool bScroll);//设置滚屏显示
		void SetCurRow(unsigned short row);//设置需要显示的第一行，滚屏显示使用
	protected:
		void OnPostInitial();
		void OnDestroy();
		void OnPaint(PAINT_STATUS status);

	protected:
		void Reset();
		bool ReadNext(std::string& out);
		unsigned short GetMaxDrawLen(bool& outbEnd, const std::string& str, unsigned short startLen, unsigned short maxWidth);
		bool IsAsciiLetter(char ch);
		bool IsSymbolEn(unsigned long charID);
		bool IsRightOrderLetter(unsigned long charID);
		void AutoLayoutText();

	protected:
		unsigned short m_OffLeft;
		unsigned short m_OffTop;
		unsigned short m_OffBottom;

		std::string	m_strText; //字符串
		std::basic_string <char>::size_type m_curPos;

		unsigned short m_lenLast;
		int m_bAddLast;

		std::list<std::string> m_drawList;

		bool m_bInitCtrl;
		bool m_bInitDrawList;
		unsigned short m_TextRow;//输入的文本需要多少行
		bool m_bScrollSCreen;//是否滚屏显示
		unsigned short	m_curRow;//当前输入多少行，只有在分屏显示下才使用
	};
}

#endif
