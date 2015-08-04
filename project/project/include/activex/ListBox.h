#ifndef _LIST_BOX_H_
#define _LIST_BOX_H_

#include <vector>
#include <map>
#include "Wnd.h"
#include "Static.h"
#include "EditCtrl.h"
#include "NumEditCtrl.h"
#include "CheckCtrl.h"
#include "Button.h"
#include "RadioCtrl.h"
#include "ComboCtrl.h"
#include "DateCtrl.h"
#include "TimeCtrl.h"
#include "IPAddressCtrl.h"
#include "ProgressCtrl.h"
#if defined (__CUSTOM_IPV6__)
#include "IPv6Ctrl.h"
#include "HexNumEditCtrl.h"
#endif
#include "EditStaticCtrl.h"

namespace GUI
{
	class CListBox : public GUI::CWnd
	{
	public:
		CListBox();
		virtual ~CListBox();

		int  AddColumn(int width);
		GUI::CWnd*  AddItem(unsigned short row, unsigned short col, WND_TYPE wndType, unsigned short cx = -1, unsigned short cy = -1 );//创建一项
		void AddItem(unsigned short row, unsigned short col, const char* pStr, bool bRefresh = false);

		void RemoveItem(unsigned short row, bool bRefresh);
		void RemoveAllItem(bool bRefresh);

		bool CreateListFocusMap();

		int GetItemCount() const { return m_itemCount; }
		int GetPageSize() const;//返回一页有多少行
		int GetPageCount() const;

		void SetTopRow(unsigned short topRow);
		unsigned short GetTopRow() const { return m_topRow; }

		GUI::CWnd* GetItemWnd(unsigned short row, unsigned short col);
		const GUI::CWnd* GetItemWnd(unsigned short row, unsigned short col) const;
		std::string GetItemText(unsigned short row, unsigned short col);

		void DrawLine(bool bDrawLine) { m_bDrawLine = bDrawLine; }
		void DrawLine(unsigned short row, unsigned long width);

		void SetItemColor(unsigned short row, unsigned short col, unsigned long color);
		void SetRowColor(unsigned short row, unsigned long color);
		
		bool NextItem();
		bool PrevItem();

		void EnableSingleSelect(bool bSelect) { m_bSingleSelect = bSelect; }
		void SetCurSingleSelect(int item, bool bRefresh);
		int  GetCurSingleSelect() const { return m_curSelectItem; }

	protected:
		DECLARE_MSG_MAP()
		virtual void OnDestroy();
		virtual void OnPaint(PAINT_STATUS status);

		GUI::CWnd* CreateSubWnd(unsigned short row, unsigned short col, WND_TYPE wndType, unsigned short cx = -1, unsigned short cy = -1 );
		
		unsigned long OnEnter(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);
	private:
		unsigned long  GetKey(unsigned short row, unsigned short col) const;
		unsigned short GetRow(unsigned long key) const;
		unsigned short GetCol(unsigned long key) const;
		bool CalculateItemPos(unsigned short row, unsigned short col, int& x, int& y) const;
		bool IsValid(unsigned short row, unsigned short col) const;

		void ShowRow(unsigned short beginRow, unsigned short endRow, bool bShow);
		void OffsetRow(unsigned short beginRow, unsigned short endRow, int rowOffset);
		

		bool GetPageRow(int page, unsigned short & beginRow, unsigned short & endRow) const;
		unsigned short GetBottomRow(unsigned short topRow) const;
		
		
		typedef std::vector<int> LISTBOX_COL_INFO;
		LISTBOX_COL_INFO m_colWidthVector;

		typedef std::map<unsigned long, GUI::CWnd*> LISTBOX_ITEM_MAP;
		LISTBOX_ITEM_MAP m_itemMap;

		typedef std::map<unsigned long, unsigned long> LISTBOX_COLOR_MAP;
		LISTBOX_COLOR_MAP m_itemColorMap;

		typedef std::map<unsigned long, unsigned long> LISTBOX_LINE_MAP;
		LISTBOX_COLOR_MAP m_itemLineMap;

		typedef std::map<unsigned long, std::string> LISTBOX_ITEM_STRING_MAP;
		LISTBOX_ITEM_STRING_MAP m_itemMapStr;

		unsigned short m_itemCount;//最大行数
		//int m_curPage;
		unsigned short m_topRow;//列表顶行
		bool m_bDrawLine;

		bool m_bSingleSelect;
		int m_curSelectItem;

		enum { LISTBOX_INVALID_ITEM = -1 };
	};
}

#endif

