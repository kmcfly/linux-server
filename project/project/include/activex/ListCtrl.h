#ifndef _LIST_CTRL_H_
#define _LIST_CTRL_H_

//#include <vector>
#include "Wnd.h"
#include "ListHeaderCtrl.h"
#include "ListBox.h"
#include "ScrollCtrl.h"

namespace GUI
{
	class CListCtrl : public GUI::CWnd
	{
	public:
		CListCtrl();
		virtual ~CListCtrl();

		virtual bool Create( unsigned long ctrlID, 
			unsigned long x, 
			unsigned long y,
			unsigned long cx,
			unsigned long cy,
			CWnd* pParent,
			int xFocus,
			int yFocus,
			bool hasHeader = true);

		int AddColumn(const char* pStr, int width);//插入列，返回列序号

		GUI::CWnd* AddItem(unsigned short row, unsigned short col, WND_TYPE wndType, unsigned short cx = -1, unsigned short cy = -1 );//创建一项,返回插入的窗口
		void AddItem(unsigned short row, unsigned short col, const char* pStr, bool bRefresh = false);

		void RemoveItem(unsigned short row, bool bRefresh)
		{
			m_listBox.RemoveItem(row, bRefresh);
		}

		void RemoveAllItem(bool bRefresh)
		{
			m_listBox.RemoveAllItem(bRefresh);
		}

		GUI::CWnd* GetItemWnd(unsigned short row, unsigned short col);
		const GUI::CWnd* GetItemWnd(unsigned short row, unsigned short col) const;
		std::string GetItemText(unsigned short row, unsigned short col);

		int GetItemCount() const
		{
			return m_listBox.GetItemCount();
		}

		void DrawLine(bool bDrawLine)
		{
			m_listBox.DrawLine(bDrawLine);
		}

		void DrawLine(unsigned short row, unsigned long width)
		{
			m_listBox.DrawLine(row, width);
		}

		void SetItemColor(unsigned short row, unsigned short col, unsigned long color)
		{
			m_listBox.SetItemColor(row, col, color);
		}

		void SetRowColor(unsigned short row, unsigned long color)
		{
			m_listBox.SetRowColor(row, color);
		}

		void EnableSingleSelect(bool bSelect) 
		{ 
			m_listBox.EnableSingleSelect(bSelect);
		}		
		
		void SetCurSingleSelect(int item, bool bRefresh);

		int  GetCurSingleSelect() const
		{
			return m_listBox.GetCurSingleSelect();
		}

		bool CreateListFocusMap();

	protected:
		DECLARE_MSG_MAP()
		virtual void OnInitial();
		virtual void OnPaint(PAINT_STATUS status);
		//unsigned long OnClickScroll();
		unsigned long OnAdd(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);
		unsigned long OnDec(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);
		unsigned long OnScrollchange(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);

		void SetScroll();

	private:
		bool			m_hasHeader;
		CListHeaderCtrl m_header;
		GUI::CListBox m_listBox;
		GUI::CScrollCtrl m_scroll_v;

		enum
		{
			LIST_SCROLL_XFOCUS = 10,
			LIST_SCROLL_YFOCUS = 35
		};
	};
}

#endif

