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
		bool PtInRect(int x, int y) const;//�ж�һ���Ƿ��ھ�����
		void Offset(int x, int y);//����ƫ��
		void Infrate(int x, int y);//���ηŴ�
		void Defrate(int x, int y);//������С
		void SetSize(int cx, int cy);//���þ��γ���
		void MoveToY(int y);//��ֱ�ƶ�
		void MoveToX(int x);//ˮƽ�ƶ�
		void MoveToXY(int x, int y);//�����ƶ�
		void Modify(int x, int y, int cx, int cy);
		CRect GetCenterRect(int cx, int cy) const;//��þ��е�һ���ߴ����
		
		bool IntersectRect(const CRect& cmp, CRect* outRect = NULL); //�жϾ����ཻ,�������������
		CRect UnionRect(const CRect& cmp); //����β���

		int m_left;
		int m_top;
		int m_right;
		int m_bottom;
	};
}


#endif


