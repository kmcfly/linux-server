#ifndef _LIST_CTRL_EX3_H_
#define _LIST_CTRL_EX3_H_

#include "Wnd.h"
#include "ListHeaderCtrl.h"
#include "Button.h"
#include "CheckCtrl.h"
#include "mylist.cpp"

namespace GUI
{
	template <class T>
	class CListItemExMulti : public GUI::CWnd
	{
	public:
		CListItemExMulti();
		virtual ~CListItemExMulti();

		virtual bool Create(unsigned long ctrlID, 
			unsigned long x, 
			unsigned long y,
			unsigned long cx,
			unsigned long cy,
			CWnd* pParent,
			int xFocus,
			int yFocus);


	protected:
		virtual void OnPaint(PAINT_STATUS status);

	private:
	};

	//////////////////////////////////////////////////////////////////////////

	template <class T>
	class CListCtrlExMulti : public GUI::CWnd
	{
		friend class CListItemExMulti<T>;
	public:
		CListCtrlExMulti();
		virtual ~CListCtrlExMulti();

		virtual bool Create( unsigned long ctrlID, 
			unsigned long x, 
			unsigned long y,
			unsigned long cx,
			unsigned long cy,
			CWnd* pParent,
			int xFocus,
			int yFocus,
			bool bSelect = false);

		int AddColumn(const char* pStr, int width);//�����У����������

		void PutData(T * pData, unsigned long num, bool bRefresh = true);
		T*   GetListData() { return m_pData; }
		const T* GetListData() const { return m_pData; }

		void AddItem(const T& item, bool bRefresh);
		const T* GetItem(int item) const;
		T* GetItem(int item);
		void DelItem(long item, bool bRefresh);

		int GetCount() const { return m_itemNum; }
	
		T* GetSelectData(unsigned long & count) const;
		
		void EnableSingleSelect() { m_bEnableSingleSel = true; }
		void SetSingleSelect(long item, bool bRefresh);
		long GetSingleSelect() const { return m_singleSelItem; }

		std::vector<unsigned long> GetMultiSelect()  { return m_multiSelectList; }
		bool ClearMultiSelectList() { m_multiSelectList.clear(); return true; }

        void CleanSelected();
		void SetSelAll();

		//bool IsSelect(unsigned long item) const;
		//int  GetCount() const;
		long ForEach(long param, bool bRefresh = false);//����ÿ����Ԫ���в����� ForEach(T &node, long item, long param)��Ҫ��д
        
		void EnableItemFocusNotify() {m_bItemFocusNotify = true;} //�����Ƿ������л�ȡ��ʱ��֪ͨ�ϲ㴰�ڴ���

	protected:
		DECLARE_MSG_MAP()
		virtual void OnInitial();
		virtual void OnDestroy();
		virtual void OnPaint(PAINT_STATUS status);
	
	protected:
		void OnDrawItem(int index, PAINT_STATUS status); //�ӿؼ�֪ͨˢ��
		void OnFocusItem(int index); //�ӿؼ��۽�ʱ

	private:
		unsigned long OnFirstPage();
		unsigned long OnPrevPage();
		unsigned long OnNextPage();
		unsigned long OnEndPage();
		unsigned long OnSelAll();
		unsigned long OnClickCheck(unsigned long ctrlID);
		unsigned long OnClickList(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);
		unsigned long OnDBClickList(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);
		long LineIndex(unsigned long xPos, unsigned long yPos);

		void ShowData(PAINT_STATUS status);
		void RefreshCheckStatus(bool bRefresh);
		bool IsItemSelect(int item) const;
		std::string & Text(T &node, unsigned short col);
		long ForEach(T &node, long item, long param);

		void DrawItem(long item, unsigned long style, bool bRefresh = true);
		//////////////////////////////////////////////////////////////////////////
		long m_singleSelItem;
		bool m_bEnableSingleSel;
		bool m_bEnableMultiSel;
		bool m_bSelect;
		long m_currLine;
		unsigned long m_pageNum;
		unsigned long m_curPage;
		long m_lineNum;
		std::string m_str;
		//////////////////////////////////////////////////////////////////////////
		CListHeaderCtrl m_header;
		typedef std::vector<int> LISTBOX_COL_INFO;
		LISTBOX_COL_INFO m_colWidthVector;

		typedef std::vector<unsigned long> SELECT_LIST;
		SELECT_LIST m_selectList;

	public:
		SELECT_LIST m_multiSelectList;
	private:

		T * m_pData;
		unsigned long m_currItem;
		unsigned long m_itemNum;

		bool m_bItemFocusNotify;		//���������ȡ����ʱ,�Ƿ�֪ͨ�ϲ㴰�ڴ���
		//////////////////////////////////////////////////////////////////////////
		enum
		{
			LIST_CTRL_BOTTOM_HEIGHT = 30,
			LIST_CTRL_BUTTON_WIDTH = 40,
			LIST_CTRL_BUTTON_HEIGHT = 24,
			LIST_CTRL_CHECK_WIDTH = 80,
		};

		//////////////////////////////////////////////////////////////////////////

		enum ITEM_DRAW_STYLE
		{
			ITEM_BG =		0x0001,	//����ͨ����
			ITEM_RC =		0x0002,	//����Row Col

			ITEM_SEL_BG =	0x0100,	//ѡ���� ����
			ITEM_FOCUS =	0x0200,	//������ ǰ��

			ITEM_ALL =		~(0x0),
		};
		//////////////////////////////////////////////////////////////////////////

		GUI::CButton	m_btnFirst;
		GUI::CButton	m_btnPrev;
		GUI::CButton	m_btnNext;
		GUI::CButton	m_btnEnd;

		GUI::CCheckCtrl m_ckAll;
		GUI::CCheckCtrl *m_pCKSelect;

		public:
			GUI::CListItemExMulti<T> *m_pListItemVir;	//������
	};
}

#endif//_LIST_CTRL_EX_H_

