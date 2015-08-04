#ifndef _LIST_HEADER_CTRL_H_
#define _LIST_HEADER_CTRL_H_
#include <vector>
#include "Wnd.h"

namespace GUI
{
	class CListHeaderCtrl : public GUI::CWnd
	{
	public:
		CListHeaderCtrl();
		virtual ~CListHeaderCtrl();

		void OnDestroy();

		int AddItem(const char* pStr, int width);//·µ»ØÐòºÅ
		int GetItemWidth(int item) const;
		int GetCount() const { return m_itemList.size(); }

	protected:
		virtual void OnPaint(PAINT_STATUS status);
	
	private:
		bool IsValidItem(int item) const;
		struct HEADER_ITEM
		{
			int item;
			int width;
			std::string str;
		};

		typedef std::vector<HEADER_ITEM> HEADER_ITEM_VECTOR;
		HEADER_ITEM_VECTOR m_itemList;

	};
}

#endif

