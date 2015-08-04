/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : YSW
** Date         : 2010-06-18
** Name         : Anchor.h
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/
#ifndef _ANCHOR_H_
#define _ANCHOR_H_
#include "Wnd.h"

namespace GUI
{
	class CAnchor : public GUI::CWnd
	{
	public:
		CAnchor();
		virtual ~CAnchor();

		virtual bool Create(unsigned long ctrlID, unsigned long x, unsigned long y, unsigned long cx, \
							unsigned long cy, CWnd* pParent, int xFocus, int yFocus, const char* pCaption = NULL);

		void SetTextColor(unsigned long color) { m_textColor = color; }

		void SetImage(BITMAP_ID idNormal, BITMAP_ID idFocus, BITMAP_ID idDisable);

	protected:
		virtual void OnInitial();
		virtual void OnPaint(PAINT_STATUS status);

	private:
		unsigned long m_textColor;

		BITMAP_ID m_idNormal;
		BITMAP_ID m_idFocus;
		BITMAP_ID m_idDisable;

		int m_bmpBgWidth;
		int m_bmpBgHeight;

		bool m_bmpNot;
	};
}

#endif //_ANCHOR_H_
