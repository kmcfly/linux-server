#ifndef _SCROLL_WND_H_
#define _SCROLL_WND_H_


#include "Wnd.h"
#include "Rect.h"
#include <vector>
#include <map>

/*
  采用固定网格子的方式实现

  缺陷:每次移动视图,如果子控件部分区域处于视图之外
  则有可能无法显示此控件。编程时尽量避免此情况发生

  方法:尽量将控件置于网格(单格或倍数格)之内,视图移
  动一般按单格或倍数格为单位

  说明:本类并不管理窗口区域的绘制,窗口的表现在
  CScrollDialog中实现.本类只管理置于本视图区域的子
  控件,以及相关的参数信息.

  思路:在一个相对坐标中下创建大视图区域(能比实际
  显示窗口大),实际窗口(相当于一块镜子)一次只能看到此
  视图区域中的一部分.

  实现:记录每个子控件的相对位置,初始位置都位于相对
  原点.每次移动视图时,利用初始相对位置和窗口大小计算
  每个子控件信息:显隐状态、位置信息,将处于显示状态的
  窗口移动到相应的位置.

  扩展:采用非固定网格,手动指定每个移动点的位置.
  即不均匀网格

  注意:加入本类的窗口控件ID用20位表示,编号<0为不索引
  控件、编号用11位表示
*/

namespace GUI
{

class CScrollWnd : public GUI::CWnd
{
	friend class CScrollDialog;
public:
	CScrollWnd();
	virtual ~CScrollWnd();

public:
	enum SW_WND_ALIGN
	{
		SW_WND_NULL		= 0x0000,
		SW_WND_OFFSET	= 0x0001,

		SW_WND_LEFT		= 0x0002,
		SW_WND_RIGHT	= 0x0004,
		SW_WND_CENTER_X = 0x0008,

		SW_WND_TOP		= 0x0010,
		SW_WND_BOTTOM	= 0x0020,
		SW_WND_CENTER_Y = 0x0040,
	};

protected:
	struct WND_ITEM 
	{
		int				Index;
		bool			Show;
		GUI::CRect		Rect;
		GUI::CWnd*		pWnd;
		WND_ITEM& operator=(const WND_ITEM& item);
	};

public:
	//为子控件创建焦点
	bool CreateWndFocusMap();

	//添加窗口元素,相对左角点(0,0)坐标
	GUI::CWnd* AddWnd(WND_TYPE wndType, unsigned long ctrlID, const GUI::CRect& rect, int xFocus,int yFocus, int index = -1);
	GUI::CWnd* AddWnd(WND_TYPE wndType, unsigned long ctrlID, const GUI::CRect& rect, int xIndex, int yIndex, int xFocus,int yFocus, unsigned long align = SW_WND_NULL, int index = -1, int xOffset = 0, int yOffset = 0);

	//重新设置窗口元素的原始位置
	void ResetWndRect(int index, const GUI::CRect& rect);

	//是否显示某个子窗口
	void ShowSubWnd(int index, bool show = true);
	void ShowSubWndID(unsigned long ctrlID, bool show = true );

	//移动视图,坐标值
	void ScrollView(int x, int y);

	//移动视图,相对标号
	void ScrollViewTo(int xIndex, int yIndex);

	//设置窗口位置
	//void SetWndPos(int x, int y, int cx, int cy);

	//设置网格块大小
	void SetSizeOffset(int sizeX,int sizeY );
	//设置视获取视图区域最大值
	void SetViewMaxSize(int maxCx,int maxCy);
	void GetViewMaxSize(int& cx, int& cy);

	//根据ID值获取某子窗口
	GUI::CWnd* GetWnd(int index);

	//获取某编号网格块左角点坐标,获取网格点大小
	void GetViewPos(int xIndex,int yIndex,unsigned long& x, unsigned long& y );
	void GetViewSize(int xIndex,int yIndex,unsigned long& cx, unsigned long&cy );

	//从当前窗口[x,y]坐标获取实际对应的网格编号
	bool GetCurViewIndex(int x,int y,int& xIndex, int& yIndex);

	//从实际网格序号获取所处于当前窗口的块编号
	bool GetCurViewMap(int xIndex,int yIndex,int& xWnd,int& yWnd);

	//获取一屏最大行列数
	void GetPageCount(unsigned long& x,unsigned long& y);
	//获取[cx,cy]的最大行列
	void GetMaxPageCount(unsigned long cx, unsigned long cy, unsigned long& x,unsigned long& y);

	//绘制视图区域边框
	void DrawViewBorder(int xIndex, int yIndex, unsigned long color = COLOR_LINE_GRAY, int offset = 0);

	//从窗口id获取其编号,-1为无效值
	int GetItemWndIndex(unsigned long id);

protected:
	void OnPaint(PAINT_STATUS status);
	void OnDestroy();

protected:
	virtual unsigned long OnCommand (unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);

protected:
	GUI::CWnd* DoAddWnd(WND_TYPE wndType, unsigned long ctrlID, const GUI::CRect& rect, int xFocus,int yFocus, int index);
	bool DoAddPosOriginally( unsigned long id, const WND_ITEM& item );
	void DoScrollWnd(int x, int y);

private:
	std::map<unsigned long,WND_ITEM> m_mapPosOriginally;		//记录子窗口控件ID,对应的原始区域,相对坐标

	int		m_xSizeOff;		//视图区域一次x偏移大小
	int		m_ySizeOff;		//视图区域一次y偏移大小
	int		m_xIndexOff;	//视图区域x偏移 = m_xSizeOff * m_xIndexOff;
	int		m_yIndexOff;	//视图区域y偏移 = m_ySizeOff * m_yIndexOff;

	int		m_maxCx;		//视图区域最大边界
	int		m_maxCy;		//视图区域最大边界

	//std::vector<int>	m_xOffVec;
	//std::vector<int>	m_yOffVec;
};

}

#endif

