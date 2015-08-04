#ifndef _SCROLL_DIALOG_H_
#define _SCROLL_DIALOG_H_

#include "Wnd.h"
#include "ScrollCtrl.h"
#include "Dialog.h"
#include "ScrollWnd.h"
#include <vector>

namespace GUI
{

class CScrollDialog : public GUI::CDialog
{
public:
	CScrollDialog();
	virtual ~CScrollDialog();

public:
	bool CreateWndFocusMap();

	//添加窗口元素,相对左角点(0,0)坐标
	GUI::CWnd* AddWnd(WND_TYPE wndType, unsigned long ctrlID, const GUI::CRect& rect, int xFocus,int yFocus, int index = -1);

	GUI::CScrollWnd& GetScrollWnd();

	void AddDrawBorderPos( int x, int y );

	void ClearAllDrawBorderPos();

	//设置当前滚动位置
	void SetCurScrollPos(int x,int y,bool bRefresh=true);
	//获取当前滚动位置
	void GetCurScrollPos(int& x, int& y);

public:
	//设置ScrollWnd窗口距离Dlg边界相对距离
	void SetScrollWndPos(int left=0,int top=0,int right=0,int bottom=0);

	//设置窗口边框大小
	void SetDlgBorder(int border = 1);

	//颜色
	void SetBackgroundColor(unsigned long color) {m_color.backgroud=color;}
	void SetBorderColor(unsigned long color) {m_color.border=color;}
	void SetWndBorderColor(unsigned long color) {m_color.wnd_border=color;}
	void SetMouseOverColor(unsigned long color) {m_color.mouse_over=color;}

	void SetMouseOverOffset(int offset) {m_size.mouse_over_offset=offset;}

protected:
	virtual void OnInitial();
	virtual void OnPostInitial();
	virtual void OnDestroy();
	virtual void OnPaint(PAINT_STATUS status);
	virtual void OnErase(PAINT_STATUS status);

protected:
	DECLARE_MSG_MAP()
	unsigned long OnScrollchangeV(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);
	unsigned long OnScrollchangeH(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);

	unsigned long OnMove(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);

	unsigned long OnAdd(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);
	unsigned long OnDec(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);

protected:
	void ScrollViewTo(int xIndex,int yIndex,bool bRefresh=true);

protected:
	GUI::CScrollWnd	m_wnd;
	GUI::CScrollCtrl	m_scroll_h;
	GUI::CScrollCtrl	m_scroll_v;

	int							m_scroll_x;			//水平滚动条,位置
	int							m_scroll_y;			//垂直滚动条,位置
	bool						m_init_scroll_x;		//是否初始化水平滚动条
	bool						m_init_scroll_y;		//是否初始化垂直滚动条

protected:

	struct POS_INDEX
	{
		int		x;
		int		y;
	};
	std::vector<POS_INDEX>		m_posDrawBorder;//需要绘制区域背景边框编号区域

	POS_INDEX			m_cur_pos;		//当前鼠标所处于的视图编号区域

protected:

	//颜色属性
	struct ITEM_COLOR
	{
		unsigned long backgroud;		//背景颜色
		unsigned long border;				//边框颜色
		unsigned long wnd_border;		//子区域边框颜色
		unsigned long mouse_over;		//鼠标滑过选框颜色
	};
	ITEM_COLOR		m_color;

	//位置、大小属性
	struct ITEM_SIZE
	{
		int mouse_over_offset;			//鼠标滑过选框绘制偏移值

		int wnd_left;
		int wnd_top;
		int wnd_right;
		int wnd_bottom;

		int dlg_border;
	};
	ITEM_SIZE			m_size;
};

}


#endif

