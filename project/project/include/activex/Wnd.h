#ifndef _BASE_CTRL_H_
#define _BASE_CTRL_H_
#include <list>
#include <string.h>
#include "Rect.h"
#include "gui.h"
#include "ImageList.h"
#include "KeyValue.h"
#include "FBInterface.h"
#include "StringTable.h"
#include "GuiAssert.h"

//////////////////////////////////////////////////////////////////////////
//extern unsigned long COLOR_TEXT_NORMAL = GUI_RGB(200, 200, 200);
//////////////////////////////////////////////////////////////////////////
//缺陷
//1.焦点移到：当只有一行焦点时，按“上键”没有实现焦点移动循环 090720
//2.静态控件刷新问题

namespace GUI
{
	class CWnd/* : public GUI::CWnd*/
	{
		friend class GUI::CImageList;
	public:
		CWnd();
		virtual ~CWnd();

		typedef std::list<CWnd*> WND_LIST;
		//设置分辨率，静态接口：使用静态变量记录宽高，用于动态计算窗体坐标。
		static void SetSize(unsigned short width, unsigned short height);

		//创建、获取根窗口,根窗口有很多限制
		//唯一的用处是记录所有[顶层]弹出窗口,并可以随时刷新整个窗口区域
		static GUI::CWnd* GetRootWnd();
		//通过该接口，可以直接刷新现场（只包括FB中控制的分割线和状态信息等）
		static void RefreshLive();
		//销毁公共数据
		static void Quit();

		////建立与销毁
		virtual bool Create ( unsigned long ctrlID, 
			unsigned long x, 
			unsigned long y, 
			unsigned long cx, 
			unsigned long cy, 
			CWnd* pParent,
			unsigned long style,
			int xFocus,
			int yFocus
			);

		void Destroy();

		/////窗口属性操作
		CTRLID GetID() const { return static_cast<CTRLID>(m_windowID); }//获取ID
		unsigned long GetStyle() const{ return m_style;}//获取风格
		void SetStyle(unsigned long style) { m_style = style; }//设置风格
		GUI::WND_TYPE GetCtrlType() const {return m_ctrlType;}//获取窗口类型
		//设焦点
		virtual bool SetFocus(bool bFocus, bool bRefresh = true);
		bool IsFocus() const { return m_bFocus; }
		//使能
		virtual void SetEnable(bool bEnable, bool bRefresh = true);
		bool IsEnable() const { return m_bEnable; }
		//可见
		void Show(bool bShow, bool bRefresh = true);
		bool IsShow() const;
		//区域
		virtual CRect GetWindowRect() const;//获得屏幕区域
		virtual CRect GetClientRect() const { return GUI::CRect(0, 0, m_rect.Width() - 1, m_rect.Height() - 1); }//获得客户区区域
		void ClientToScreen(unsigned long & x, unsigned long & y) const;//客户区一点转换到屏幕坐标系
		void ScreenToClient(int & x, int & y) const;
		void ScreenToClient(CRect& rect) const;
	
		//获得活动子窗口
		CWnd* GetActiveCtrl() { return m_pActiveCtrl; }//获得当前子窗口，具有焦点的窗口
		CWnd* GetCtrl(CTRLID ctrlID);//根据ID获取子窗口
		//设置窗口标题
		virtual void SetCaption(const std::string& strCaption, bool bRefresh = true);
		virtual std::string GetCaption() const { return m_strCaption; }
		//获得焦点坐标
		void GetFocusPos(int & xFocus, int & yFocus) const 
		{
			xFocus = m_xFocus;
			yFocus = m_yFocus;
		}
		//设置\获得图片
		virtual void SetImage(BITMAP_ID id, bool bRefresh);
		virtual BITMAP_ID GetImage() const { return m_imageID; }

		////窗口层次
		//获取子窗口链表中最顶层窗口
		CWnd* GetTopestWnd();
		//获取父chuangk
		CWnd* GetParent() { return m_pParent; }

		////绑定数据,可以绑定某个整数数或者指针
		void SetData(LONGLONG pData) { m_data = pData;}
		LONGLONG GetData() const { return m_data; }

		//移动窗口 注意：不会刷新窗口
		virtual void MoveWnd(int x, int y);
		void SetWndSize(bool bOriginal = true, bool bRefresh = true, int cx = 0, int cy = 0);

		//设置窗口位置
		void SetWndPos(const CRect& rect);
		void SetWndPos(int x, int y, int cx, int cy);

		//重绘
		void Repaint();
		//////////////////////////////////////////////////////////////////////////
		////获得系统分配的一个窗口ID
		static CTRLID GetRandWndID() { return static_cast<CTRLID>(++s_randWndID); }
	protected:
		////用于继承重写
		//初始化与反初始化
		virtual void OnInitial() {};
		virtual void OnDestroy() {};
		//初始化后的操作
		virtual void OnPostInitial() {};
		//绘制窗口
		virtual void OnPaint(PAINT_STATUS status){};
		virtual void OnErase(PAINT_STATUS status){};
		//焦点改变响应
		virtual void OnChangeFocus(unsigned long oldID, unsigned long newID);
		virtual unsigned long OnFocusProcEx(unsigned long keyID, unsigned long xPoint, unsigned long yPoint)
		{return keyID;}
		////响应按键
		//响应对于子窗口的按键消息
		virtual unsigned long OnCommand (unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
		{ return keyID; }
		virtual unsigned long OnPreCommand(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
		{ return keyID; }
		
		
		////不继承重写
		//处理按键消息：焦点处理、按键响应
		unsigned long OnKeyProc(unsigned long keyID, unsigned long xPoint, unsigned long yPoint);
		//处理焦点
		unsigned long OnFocusProc(unsigned long keyID, unsigned long xPoint, unsigned long yPoint);
		//通过ID获得子窗口，未实现
		CWnd* GetChild(unsigned long id) { return NULL;}
		//光标在哪个有效的控件上
		CWnd* InWhichActiceCtrl(unsigned long xPoint, unsigned long yPoint); 
		//光标是否在窗口上
		bool IsInTheRect(unsigned long xPoint, unsigned long yPoint) const;
		//建立焦点地图
		bool CreateFocusMap();
		//获取焦点数目
		void GetFocusCountInMap(int& x, int & y) const;
		void CalculateFocusCount(int& x, int & y) const;

		//显示图片
		//bool DrawImage(const std::string& strFile, int x, int y);
		bool DrawImage(BITMAP_ID id, int x, int y);
		bool GetImageSize(BITMAP_ID id, int& cx, int& cy) const;
		
		//////////////////////////////////////////////////////////////////////////
		void DrawClear(unsigned short x, unsigned short y, unsigned short cx, unsigned short cy, bool bImmediate = false);
		void DrawRect(unsigned short x, unsigned short y, unsigned short cx, unsigned short cy, unsigned long color, bool bImmediate = false);
		void DrawRectBorder(unsigned short x, unsigned short y, unsigned short cx, unsigned short cy, unsigned long color, unsigned short height = 1, unsigned short width = 0, bool bImmediate = false);//绘制矩形边框
		void DrawLine(unsigned short x1, unsigned short y1, unsigned short x2, unsigned short y2, unsigned long color, unsigned short width = 1, bool bImmediate = false);
		unsigned long DrawText(const char *pStr, unsigned long len, unsigned short x, unsigned short y, unsigned short cx, unsigned short cy, unsigned long color = COLOR_TEXT_NORMAL, bool bImmediate = false, unsigned char zoom = 1, bool bResort = true);
		void DrawImage(const unsigned char *pImage, unsigned long len, unsigned short x, unsigned short y, unsigned short width, unsigned short cx, unsigned short cy, bool bImmediate = false);
		bool TextSize(const char *pStr, unsigned long len, unsigned short &cx, unsigned short &cy, unsigned char zoom = 1);
		void Refresh(unsigned short x, unsigned short y, unsigned short cx, unsigned short cy);
		bool ReSortText(char* pText, const char* pSor, int len);
		//////////////////////////////////////////////////////////////////////////
	
		//方便调试提示信息,与gui_assert配合使用.出现异常时,Dump类的相关信息
		virtual void Dump(){};

	public://以下函数注意使用
		//删除子对话框
		void DestroyChild();
		//加入与移除子窗口		
		bool AddChildList(CWnd* pNode);
		bool RemoveChild(CWnd* pNode);

	private:
		//绘制与清除
		void Draw();
		void Clear();
		
		////焦点操作
		//是否可设置焦点
		bool CanSetFocus() const;
		//根据按键管理子窗口焦点		
		//寻找上面一个可以设置焦点的子窗口
		CWnd* MoveFocusUp(unsigned long xFocus, unsigned long yFocus) const;   
		CWnd* MoveFocusDown(unsigned long xFocus, unsigned long yFocus) const; 
		CWnd* MoveFocusLeft(unsigned long xFocus, unsigned long yFocus) const; 
		CWnd* MoveFocusRight(unsigned long xFocus, unsigned long yFocus) const;

		CWnd* MoveFocusRightEx(unsigned long xFocus, unsigned long yFocus) const;//在一行内，焦点右移，知道有控件具有可设置焦点为止，失败返回NULL
		CWnd* MoveFocusLeftEx(unsigned long xFocus, unsigned long yFocus) const;
		//设置默认焦点
		unsigned long SetDefaultFocus(bool bRefresh = true);
		//设置默认焦点模式
		void SetDefaultFocusMode(DEFUALT_FOCUS mode)
		{
			m_defaultFocus = mode;
		}
		//获取默认焦点子窗口
		CWnd* GetDefaultFocusCtrl(DEFUALT_FOCUS mode);
		//设置控件焦点
		bool SetCtrlFocus(CWnd* pCtrl, bool bFocus, bool bRefresh = true); 
		//是否可支持设置焦点
		bool IsSupportFocus() const;
		PAINT_STATUS PaintStatus();
	public:
		////属性
		static unsigned short m_width, m_height;
		static unsigned short m_length [LENGTH_NUM];
		static CStringTable	*m_pStrTable;
		
		//窗口属性
		CRect m_showRect;//指定的显示区域，可能比实际要求的区域小
		CRect m_rect;//窗口区域
		bool m_bShow;//显示状态
		bool m_bFocus;//焦点状态
		bool m_bEnable;//使能状态
		unsigned long m_style;//窗口风格
		unsigned long m_windowID;//窗口ID
		WND_TYPE m_ctrlType;//窗口类型
		std::string m_strCaption;//标题
		BITMAP_ID m_imageID;//背景图片ID

		////窗口结果
		WND_LIST m_childList;//子窗口链表
		CWnd* m_pParent;//父窗口

		////焦点
		CWnd *m_pActiveCtrl;//活动子窗口，具有焦点的
		int m_hCtrlNum;	//水平方向上(支持焦点的)控件的最大数目
		int m_vCtrlNum;	//垂直方向上(支持焦点的)控件的最大数目
		int m_xFocus;//焦点坐标 x
		int m_yFocus;//焦点坐标 y
		CWnd**	m_pFocusMap;	//焦点地图
		DEFUALT_FOCUS m_defaultFocus;//默认焦点位置

		//绑定数据
		//void* m_pData;
		LONGLONG m_data;
	private:
		static GUI::CFBInterface *m_pFBInterface;
		static int s_randWndID;//系统分配ID

	private:
		//根窗口不做任何事,只记录所有的Popup窗口
		//根窗口目前唯一的用处是刷新当前整个屏幕
		static GUI::CWnd* m_pRootWnd;
	};
}
#endif

