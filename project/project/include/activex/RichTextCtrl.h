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

		
		void SetText(const std::string& str, bool bRefresh);//�����ı�
		void AppendText(const std::string& str, bool bRefresh);//��������ַ���
		unsigned short GetTextRow();//��ȡ�ı�����
		void SetScrollShow(bool bScroll);//���ù�����ʾ
		void SetCurRow(unsigned short row);//������Ҫ��ʾ�ĵ�һ�У�������ʾʹ��
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

		std::string	m_strText; //�ַ���
		std::basic_string <char>::size_type m_curPos;

		unsigned short m_lenLast;
		int m_bAddLast;

		std::list<std::string> m_drawList;

		bool m_bInitCtrl;
		bool m_bInitDrawList;
		unsigned short m_TextRow;//������ı���Ҫ������
		bool m_bScrollSCreen;//�Ƿ������ʾ
		unsigned short	m_curRow;//��ǰ��������У�ֻ���ڷ�����ʾ�²�ʹ��
	};
}

#endif
