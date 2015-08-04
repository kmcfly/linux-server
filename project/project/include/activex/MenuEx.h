#ifndef _MENU_EX_H_
#define _MENU_EX_H_

#include "Dialog.h"
#include "DrawManager.h"
#include <list>
#include <map>

namespace GUI
{
	class CMenuEx;

	enum MENUEX_TYPE
	{
		//�˵����˵���������
		MENUEX_NORMAL		= 0x0001,	//��ͨ��
		MENUEX_SEPARATOR	= 0x0002,	//�ָ����

		MENUEX_IMAGE		= 0x0100,	//[���]ͼƬ
		MENUEX_STRING		= 0x0200,	//[�м�]�ַ���
		MENUEX_POPUP		= 0x0400,	//[�ұ�]������
	};

	enum MENUEX_ITEM_STATE
	{
		//����״̬
		MITEM_NORMAL		= 0x0001,	//һ��
		MITEM_IS_POPUP		= 0x0002,	//������Ӳ˵���������ʾ

		MITEM_UN_ENABLE		= 0x0004,	//���ʹ��
		MITEM_UN_VISIBLE	= 0x0008,	//����ɼ�
		
		MITEM_EN_POPUP		= 0x0010,	//�е����Ӳ˵�ʱ,ʹ��ӦEnter�¼�[Ĭ�ϲ���Ӧ]
	};

	////�˵���
	class CMenuExItem : public GUI::CWnd
	{
		friend class CMenuEx;
	public:
		CMenuExItem();
		virtual ~CMenuExItem();

		virtual bool Create(unsigned long ctrlID, CWnd* pParent, unsigned long style, int xFocus, int yFocus);

		//��ȡ����ID
		unsigned long GetItemId();

		//���á���ȡ ״̬: ���û������״̬,��Ҫ�ı����ͼƬ��
		void SetTailImage(BITMAP_ID id);
		void SetStateImage(int uiState, PAINT_STATUS status, BITMAP_ID id);
		void SetCurState(int uiState, bool bRefresh = false);
		int GetCurState();

		//ʹ��
		void SetEnable(bool enable = true, bool bRefresh = false);
		bool IsEnable();

		//ʹ�ɼ�
		void SetVisible(bool visible = true);
		bool IsVisible();

		//���ÿ��Ե����Ӳ˵�������,������ӦEnter��Ϣ
		void SetEnPopupEnter(bool enbale = false);
		bool IsEnPopupEnter();

		//��ȡ����������״̬: ���ı�����������ʾ�Լ���Ϊ��
		void SetItemState(unsigned long state, bool bRefresh);
		unsigned long GetItemState();

	public:
		enum
		{
			ITEM_HEIGHT				= 28,	//��ͨ����߶�
			ITEM_SEPARATOR			= 4,	//�ָ����߶�

			ITEM_IMAGE_HEIGHT		= 22,	//���ͼƬ�߶�
			ITEM_IMAGE_WIDTH		= 24,	//���ͼƬ���
			ITEM_TAIL_HEIGHT		= 12,	//�ұ�ͼƬ�߶�
			ITEM_TAIL_WIDTH			= 10,	//�ұ�ͼƬ���
			ITEM_OFFSET				= 8,	//ͼƬ����֮��ļ�϶
		};

	protected:
		// ���û�ȡ����
		void SetItemId(unsigned long id);
		void SetMenuType(unsigned long type);
		void SetItemType(unsigned long type);
		unsigned long GetMenuType();
		unsigned long GetItemType();

		//���ò˵����͡��������ͺ�,���Լ��㱾��������Ŀ�Ⱥ͸߶�
		unsigned long GetItemWidth();
		unsigned long GetItemHeight();

		//���������� �Ӳ˵�
		bool IsPopupSubMenu(bool popup = true);
	protected:
		void OnInitial();
		void OnPaint(PAINT_STATUS status);
		void OnDestroy();

	private:
		unsigned long	m_menuType;			//�����˵�������
		unsigned long	m_itemId;			//��Item��ID
		unsigned long	m_itemType;			//��Item������

		int				m_curState;			//��ǰ�û��趨��״̬

		unsigned long	m_itemState;		//��ǰ�����״̬

		////��ͼ����
		struct ITEM_STATE
		{
			int				item_state;		//ĳ��״̬
			PAINT_STATUS	paint_status;	//����״̬
			BITMAP_ID		bmp_id;			//��ԴͼƬID

			ITEM_STATE& operator=(const ITEM_STATE& item);
		};

		std::vector<ITEM_STATE>				m_bmpStateVec;		//�����û�����״̬��ԴͼƬ
		std::map<PAINT_STATUS, BITMAP_ID>	m_paintStatusMap;	//��ǰ״̬��Դ
		BITMAP_ID							m_tailBmpID;		//�ұ���ԴͼƬID

		int									m_bmpBgWidth;		//���㱳��ͼƬ���
	};

	//////////////////////////////////////////////////////////////////////////
	//�˵� : ֧�����2��
	//////////////////////////////////////////////////////////////////////////

	////�˵�
	class CMenuEx : public GUI::CDialog
	{
	public:
		CMenuEx();
		virtual ~CMenuEx();

		//�����˵�
		unsigned long Popup(unsigned long x, unsigned long y);

		//�������
		CMenuExItem* AddItem(const char* pStr = NULL, unsigned long itemID = INVALID_ID, CMenuEx* pPopupWnd = NULL);

		//��ȡ����
		CMenuExItem* GetItem(unsigned long itemID, CMenuEx* pMenuEx = NULL);
		CMenuExItem* GetItemByWndId(unsigned long wndId, CMenuEx* pMenuEx = NULL);

		//���á���ȡ������Ϣ
		void SetItemTailImage(unsigned long itemID, BITMAP_ID id);
		void SetItemStateImage(unsigned long itemID, int uiState, PAINT_STATUS status, BITMAP_ID id);
		void SetItemCurState(unsigned long itemID, int uiState, bool bRefresh = false);
		void SetItemEnable(unsigned long itemID, bool enable = true, bool bRefresh = false);
		void SetItemVisible(unsigned long itemID, bool visible = true);
		void SetItemEnPopupEnter(unsigned long itemID, bool enable = false);

		int GetItemCurState(unsigned long itemID);
		bool GetItemEnable(unsigned long itemID);
		bool GetItemVisible(unsigned long itemID);
		bool GetItemEnPopupEnter(unsigned long itemID);

		//��ղ˵�����
		void Clear();

	public:
		enum
		{
			MENUEX_BORDER_LEFT		= 6,
			MENUEX_BORDER_TOP		= 6,
			MENUEX_BORDER_RIGHT		= 6,
			MENUEX_BORDER_BOTTOM	= 6,
		};

	protected:
		//���ִ���
		void AutoLayout();

		//���á���ȡ���˵���Ч��ʾ����
		void SetMenuRect(const GUI::CRect& rect);
		GUI::CRect GetMenuRect();

		//���á���ȡ�˵�����ֵ
		void SetRetValue(unsigned long value);
		unsigned long GetRetValue();

		void RefreshMenu(const GUI::CRect* pRect1 = NULL, const GUI::CRect* pRect2 = NULL);

	protected:
		virtual void OnInitial();
		virtual void OnDestroy();
		virtual void OnPaint(PAINT_STATUS status);
		virtual void OnChangeFocus(unsigned long oldID, unsigned long newID);
		DECLARE_MSG_MAP()
		unsigned long OnLClick(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);

	protected:
		typedef std::list<CMenuExItem*>				MENUEX_ITEM_LIST;
		typedef std::map<unsigned long, CMenuEx*>	MENUEX_POPUP_WND;

		MENUEX_ITEM_LIST m_itemList;	//�洢����
		MENUEX_POPUP_WND m_popupWndMap;	//�洢������

		unsigned long	m_minWidth;		//�����Сֵ
		unsigned long	m_posLeft;		//����
		unsigned long	m_posTop;		//�

		GUI::CRect		m_rectMenu;		//�˵�����

		unsigned long	m_retValue;		//����ֵ

		//
		int				m_bmpBgWidth;	//����ͼ����
		int				m_bmpBgHeight;

	////dump
	public:
		void Dump();
	};
}

#endif

//end
