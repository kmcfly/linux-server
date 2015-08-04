/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : hz
** Date         : 2009-07-10
** Name         : Rect.h 
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/
#ifndef _RECT_H_
#define _RECT_H_

#ifndef NULL
#define NULL 0
#endif

namespace GUI 
{
	class CRect
	{
	public:
		CRect(void);
		CRect(int left, int top, int right, int bottom);
		~CRect(void);

		int Width() const;
		int Height() const;
		void SetRect(int left, int top, int right, int bottom);
		bool PtInRect(int x, int y) const;//判断一点是否在矩形上
		void Offset(int x, int y);//矩形偏移
		void Infrate(int x, int y);//矩形放大
		void Defrate(int x, int y);//矩形缩小
		void SetSize(int cx, int cy);//设置矩形长宽
		void MoveToY(int y);//垂直移动
		void MoveToX(int x);//水平移动
		void MoveToXY(int x, int y);//任意移动
		void Modify(int x, int y, int cx, int cy);
		CRect GetCenterRect(int cx, int cy) const;//获得居中的一定尺寸矩形
		
		bool IntersectRect(const CRect& cmp, CRect* outRect = NULL); //判断矩形相交,并求出交集矩形
		CRect UnionRect(const CRect& cmp); //求矩形并集

		int m_left;
		int m_top;
		int m_right;
		int m_bottom;
	};
}


#endif


