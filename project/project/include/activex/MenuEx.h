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
		//菜单、菜单子项类型
		MENUEX_NORMAL		= 0x0001,	//普通项
		MENUEX_SEPARATOR	= 0x0002,	//分割符号

		MENUEX_IMAGE		= 0x0100,	//[左边]图片
		MENUEX_STRING		= 0x0200,	//[中间]字符串
		MENUEX_POPUP		= 0x0400,	//[右边]弹出项
	};

	enum MENUEX_ITEM_STATE
	{
		//子项状态
		MITEM_NORMAL		= 0x0001,	//一般
		MITEM_IS_POPUP		= 0x0002,	//子项的子菜单被弹出显示

		MITEM_UN_ENABLE		= 0x0004,	//子项不使能
		MITEM_UN_VISIBLE	= 0x0008,	//子项不可见
		
		MITEM_EN_POPUP		= 0x0010,	//有弹出子菜单时,使响应Enter事件[默认不响应]
	};

	////菜单项
	class CMenuExItem : public GUI::CWnd
	{
		friend class CMenuEx;
	public:
		CMenuExItem();
		virtual ~CMenuExItem();

		virtual bool Create(unsigned long ctrlID, CWnd* pParent, unsigned long style, int xFocus, int yFocus);

		//获取子项ID
		unsigned long GetItemId();

		//设置、获取 状态: 由用户定义的状态,主要改变左边图片区
		void SetTailImage(BITMAP_ID id);
		void SetStateImage(int uiState, PAINT_STATUS status, BITMAP_ID id);
		void SetCurState(int uiState, bool bRefresh = false);
		int GetCurState();

		//使能
		void SetEnable(bool enable = true, bool bRefresh = false);
		bool IsEnable();

		//使可见
		void SetVisible(bool visible = true);
		bool IsVisible();

		//设置可以弹出子菜单的子项,可以响应Enter消息
		void SetEnPopupEnter(bool enbale = false);
		bool IsEnPopupEnter();

		//获取、设置子项状态: 将改变子项整体显示以及行为等
		void SetItemState(unsigned long state, bool bRefresh);
		unsigned long GetItemState();

	public:
		enum
		{
			ITEM_HEIGHT				= 28,	//普通子项高度
			ITEM_SEPARATOR			= 4,	//分割符项高度

			ITEM_IMAGE_HEIGHT		= 22,	//左边图片高度
			ITEM_IMAGE_WIDTH		= 24,	//左边图片宽度
			ITEM_TAIL_HEIGHT		= 12,	//右边图片高度
			ITEM_TAIL_WIDTH			= 10,	//右边图片宽度
			ITEM_OFFSET				= 8,	//图片文字之间的间隙
		};

	protected:
		// 设置获取类型
		void SetItemId(unsigned long id);
		void SetMenuType(unsigned long type);
		void SetItemType(unsigned long type);
		unsigned long GetMenuType();
		unsigned long GetItemType();

		//设置菜单类型、子项类型后,可以计算本子项需求的宽度和高度
		unsigned long GetItemWidth();
		unsigned long GetItemHeight();

		//弹出、收起 子菜单
		bool IsPopupSubMenu(bool popup = true);
	protected:
		void OnInitial();
		void OnPaint(PAINT_STATUS status);
		void OnDestroy();

	private:
		unsigned long	m_menuType;			//弹出菜单的类型
		unsigned long	m_itemId;			//本Item的ID
		unsigned long	m_itemType;			//本Item的类型

		int				m_curState;			//当前用户设定的状态

		unsigned long	m_itemState;		//当前子项的状态

		////绘图数据
		struct ITEM_STATE
		{
			int				item_state;		//某种状态
			PAINT_STATUS	paint_status;	//绘制状态
			BITMAP_ID		bmp_id;			//资源图片ID

			ITEM_STATE& operator=(const ITEM_STATE& item);
		};

		std::vector<ITEM_STATE>				m_bmpStateVec;		//所有用户定义状态资源图片
		std::map<PAINT_STATUS, BITMAP_ID>	m_paintStatusMap;	//当前状态资源
		BITMAP_ID							m_tailBmpID;		//右边资源图片ID

		int									m_bmpBgWidth;		//焦点背景图片宽度
	};

	//////////////////////////////////////////////////////////////////////////
	//菜单 : 支持最多2级
	//////////////////////////////////////////////////////////////////////////

	////菜单
	class CMenuEx : public GUI::CDialog
	{
	public:
		CMenuEx();
		virtual ~CMenuEx();

		//弹出菜单
		unsigned long Popup(unsigned long x, unsigned long y);

		//添加子项
		CMenuExItem* AddItem(const char* pStr = NULL, unsigned long itemID = INVALID_ID, CMenuEx* pPopupWnd = NULL);

		//获取子项
		CMenuExItem* GetItem(unsigned long itemID, CMenuEx* pMenuEx = NULL);
		CMenuExItem* GetItemByWndId(unsigned long wndId, CMenuEx* pMenuEx = NULL);

		//设置、获取子项信息
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

		//清空菜单子项
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
		//布局窗口
		void AutoLayout();

		//设置、获取本菜单有效显示区域
		void SetMenuRect(const GUI::CRect& rect);
		GUI::CRect GetMenuRect();

		//设置、获取菜单返回值
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

		MENUEX_ITEM_LIST m_itemList;	//存储子项
		MENUEX_POPUP_WND m_popupWndMap;	//存储弹出项

		unsigned long	m_minWidth;		//宽度最小值
		unsigned long	m_posLeft;		//最左
		unsigned long	m_posTop;		//最顶

		GUI::CRect		m_rectMenu;		//菜单区域

		unsigned long	m_retValue;		//返回值

		//
		int				m_bmpBgWidth;	//背景图宽、高
		int				m_bmpBgHeight;

	////dump
	public:
		void Dump();
	};
}

#endif

//end
