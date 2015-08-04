#ifndef _COMBO_CTRL_H_
#define _COMBO_CTRL_H_

#include "Wnd.h"
#include "Static.h"
#include "Button.h"
#include "EditCtrl.h"
#include "ScrollDialog.h"

namespace GUI
{
	//////////////////////////////////////////////////////////////////////////

	struct CTRL_COMBO_ITEM 
	{
		int item;
		std::string str;
		LONGLONG data;
	};
	typedef std::vector<CTRL_COMBO_ITEM> COMBO_ITEM_VECTOR;

	//////////////////////////////////////////////////////////////////////////
	class CComboItem : public GUI::CWnd
	{
	public:
		CComboItem();
		virtual ~CComboItem();

		void SetSel(bool sel) { m_bSel = sel; }
		bool GetSel() { return m_bSel; }

	public:
		virtual bool Create( unsigned long ctrlID, unsigned long x, unsigned long y, unsigned long cx, unsigned long cy,
										CWnd* pParent, int xFocus, int yFocus);

	protected:
		virtual void OnPaint(PAINT_STATUS status);
		virtual bool SetFocus(bool bFocus, bool bRefresh = true);

		bool m_bSel;
	};

	//////////////////////////////////////////////////////////////////////////

	class CComboMenu : public CScrollDialog
	{
	public:
		CComboMenu();
		virtual ~CComboMenu();

		void SetCurSel(int sel,bool bRefresh);
		int GetCurSel();
		int GetCountSel();

		void SetPageCount(int pageCount );									//ֵ������ҳ��߶�Э��
		int GetPageCount();
		void SetItemVector(COMBO_ITEM_VECTOR& itemVec );

	protected:
		virtual void OnInitial();
		virtual void OnPostInitial();
		virtual void OnDestroy();
		virtual void OnOutOfDlg(unsigned long keyID);
		virtual void OnChangeFocus(unsigned long oldID, unsigned long newID);
		virtual unsigned long OnFocusProcEx(unsigned long keyID, unsigned long xPoint, unsigned long yPoint);
	protected:
		DECLARE_MSG_MAP()
		unsigned long OnSelItem(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);
		unsigned long OnMove(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);

	protected:
		int							m_pageCount;		//ҳ�����������
		COMBO_ITEM_VECTOR*			m_cbItemVec;		//��Ͽ�����

	protected:
		int			m_curSel;
		bool		m_bInit;
	};

	//////////////////////////////////////////////////////////////////////////

	class CComboCtrl : public GUI::CWnd
	{
	public:
		CComboCtrl();
		virtual ~CComboCtrl();

	public:
		int  AddItem(const char* pStr, LONGLONG data = 0);//����item
		void RemoveAllItem(bool bRefresh);

		void SetCurItem(int item, bool bRefresh = false);
		int  GetCurItem() const { return m_curItem; }

		bool SetItemData(int item, LONGLONG data);
		LONGLONG GetItemData(int item) const;

		bool SetCurItemData(LONGLONG data, bool bRefresh);//����Data���õ�ǰ��
		LONGLONG GetCurItemData() const;

		void SetMaxStringLen(int maxLen);
		int GetMaxStringLen() const;

		unsigned long GetItemCount() const;
		void RemoveItem(int item, bool bRefresh = true);
		void GetItemString(int item, std::string& strItem) const;
		virtual void SetEnable(bool bEnable, bool bRefresh = true);

		//���������б�һ����ʾ��Ŀ��
		void SetDropDownCount(int count);

		//����Edit����ʱֵ
		void SetEditText(const char* pStr, bool bRefresh = true);

		void Dump();

		const char *GetItemText(int item);

	public:
		virtual bool Create(unsigned long ctrlID, unsigned long x, unsigned long y, unsigned long cx, unsigned long cy,
									CWnd* pParent, int xFocus, int yFocus);

		virtual void MoveWnd(int x, int y);

	protected:
		virtual void OnInitial();
		virtual void OnPaint(PAINT_STATUS status);
		virtual void OnDestroy();
		virtual void OnChangeFocus(unsigned long oldID, unsigned long newID);

	protected:
		DECLARE_MSG_MAP()
		unsigned long OnBtnDropDown();
		unsigned long OnAdd();
		unsigned long OnDec();

	protected:
		bool IsValidItem(int item) const;

	protected:
		GUI::CEditCtrl		m_editText;
		GUI::CButton		m_btnDropDown;
		CComboMenu			m_popupMenu;

	protected:
		enum 
		{
			INVALID_ITEM = -1,
		};

		COMBO_ITEM_VECTOR m_itemVec;	//�洢 Item��

		int m_curItem;					//��ǰѡ�����Ŀ
		int m_countDropDown;			//��������ʾ��Ŀ
		int m_maxEditStringLen;			//�༭����󳤶�

		CTRL_COMBO_ITEM m_invalidItem;	//�����õ�ֵ��Ч[����m_itemVec��],�洢�ڴ�.������Ͽ����÷�ѡ���е�ֵ
	};

}

#endif
