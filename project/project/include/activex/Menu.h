#ifndef _MENU_H_
#define _MENU_H_

#include "Dialog.h"

namespace GUI
{
	class CMenu : public GUI::CDialog
	{
	public:
		CMenu();
		virtual ~CMenu();

		int AddItem(const char* pStr);//·µ»ØÐòºÅ
		void SetItemData(int item, LONGLONG data);
		LONGLONG GetItemData(int item) const;

		void DelteItem(int item);
		void DeleteAllItem();

		unsigned short GetWidth();
		unsigned short GetHeight();

		//////////////////////////////////////////////////////////////////////////
		int PopUp(int x, int y, GUI::CWnd* pWnd);

	protected:
		DECLARE_MSG_MAP()
		void OnInitial();
		void OnDestroy();
		void OnPaint(PAINT_STATUS status);
		void OnOutOfDlg(unsigned long keyID);

		unsigned long OnClickItem(unsigned long ctrlID);

		void ClearItemList();

	private:
		typedef std::vector<GUI::CButton*> MENU_ITEM_LIST;
		typedef std::vector<unsigned long> MENU_ITEM_DATA_LIST;

		MENU_ITEM_LIST m_menuItemList;
		MENU_ITEM_DATA_LIST m_itemDataList;

		int m_selItem;

		enum { BORDER_WIDHT = 1 };
	};
}

#endif//_MENU_H_
