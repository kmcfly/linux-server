#ifndef _COMBO_CTRL_EX_H_
#define _COMBO_CTRL_EX_H_
#include <vector>
#include "Wnd.h"
#include "EditCtrl.h"
#include "Button.h"

namespace GUI
{
	class CComboCtrlEx : public GUI::CWnd
	{
	public:
		CComboCtrlEx();
		virtual ~CComboCtrlEx();

		virtual bool Create( unsigned long ctrlID, 
			unsigned long x, 
			unsigned long y,
			unsigned long cx, 
			unsigned long cy,
			CWnd* pParent,
			int xFocus,
			int yFocus,
			bool bNumEdit = false);

		void EnableInput(bool bEnable, bool bRefresh = false);

		int  AddItem(const char* pStr, LONGLONG data = 0);//返回item
		void RemoveAllItem(bool bRefresh);

		bool SetItemData(int item, LONGLONG data);
		LONGLONG GetItemData(int item) const;

		void SetCurItem(int item, bool bRefresh = false);
		int  GetCurItem() const { return m_curItem; }

		bool SetCurItemData(LONGLONG data, bool bRefresh);//根据Data设置当前项
		LONGLONG GetCurItemData() const;

		void SetMaxStringLen(int maxLen);
		int GetMaxStringLen() const;

		virtual void SetEnable(bool bEnable, bool bRefresh = true);
	protected:
		DECLARE_MSG_MAP()
		virtual void OnInitial();
		virtual void OnDestroy();

		virtual void OnChangeFocus(unsigned long oldID, unsigned long newID);
		unsigned long OnClickLeftBtn();
		unsigned long OnClickRightBtn();
		unsigned long OnKeyAdd();
		unsigned long OnKeyDec();
		unsigned long OnInput(unsigned long keyID);

		bool IsValidItem(int item) const;
		void SetCurStringToList();

		void NextItem();
		void PrevItem();

		int GetItemFromData(LONGLONG data) const;

		GUI::CEditCtrl* m_pEdit;
		GUI::CButton m_btnLeft;
		GUI::CButton m_btnRight;
		int m_curItem;
		bool	m_editEnable;

		enum 
		{
			INVALID_ITEM = -1,
		};

		struct COMBO_ITEM 
		{
			int item;
			std::string str;
			LONGLONG data;
		};
		typedef std::vector<COMBO_ITEM> ITEM_VECTOR;
		ITEM_VECTOR m_itemList;

	private:
	};
}

#endif

