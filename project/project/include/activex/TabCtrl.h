#ifndef _TAB_CTRL_H_
#define _TAB_CTRL_H_
#include <vector>
#include "Wnd.h"
#include "RadioCtrl.h"

namespace GUI
{
	class CTabButton : public GUI::CRadioCtrl
	{
	public:
		CTabButton();
		virtual ~CTabButton();

		virtual bool Create ( unsigned long ctrlID, 
			unsigned long x, 
			unsigned long y, 
			unsigned long cx, 
			unsigned long cy, 
			CWnd* pParent,
			unsigned long style,
			int xFocus,
			int yFocus)
		{
			return GUI::CWnd::Create(ctrlID, x, y, cx, cy, pParent, style, xFocus, yFocus);
		}

		void MapWnd(GUI::CWnd* pWnd)
		{ 
			SetData(reinterpret_cast<LONGLONG>(pWnd)); 
		}

		GUI::CWnd* GetWnd() const 
		{ 
			return reinterpret_cast<GUI::CWnd*>(GetData());
		}

		void SetWindowPos(int x, int y, int cx, int cy);
	protected:
		virtual void OnPaint(PAINT_STATUS status);
	};


	class CTabCtrl : public GUI::CWnd
	{
		

	public:
		CTabCtrl();
		virtual ~CTabCtrl();

		virtual bool Create( unsigned long ctrlID, 
			unsigned long x, 
			unsigned long y,
			unsigned long cx, 
			unsigned long cy, 
			CWnd* pParent,
			int xFocus,
			int yFocus);

		bool AddItem(GUI::CWnd* pWnd, const std::string& str);
		void SetCurItem(int item, bool bRefresh = true);
		int  GetCurItem() const
		{ return m_curItem; }

		//返回当前具有焦点的项。
		int GetFocusItem();

		//默认值-1表示，获取当前窗口指针
		GUI::CWnd* GetWnd( int item = -1 ) const;

	protected:
		virtual void OnInitial();
		virtual void OnDestroy();
		virtual unsigned long OnCommand (unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);
	private:
		//TAB_BTN_LIST m_btnList;
		enum { MAX_TAB_ITEM = 8 };
		enum { FIRST_BTN_OFFSET = 15 };
		GUI::CTabButton m_btnArrary[MAX_TAB_ITEM];
		unsigned short	m_itemLen[MAX_TAB_ITEM];
		GUI::CRadioGroup m_btnGroup;
		int m_itemCount;
		int m_curItem;
		unsigned short m_tabLen;
		int m_btnHeight;
	};
}

#endif
