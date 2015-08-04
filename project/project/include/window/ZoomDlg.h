/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : YSW
** Date         : 2010-07-1
** Name         : ZoomDlg.h
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/
#ifndef _ZOOM_DLG_H_
#define _ZOOM_DLG_H_

#include "Dialog.h"
#include "Button.h"
#include "ZoomPanelDlg.h"

using namespace GUI;

class CZoomDlg : public GUI::CDialog
{
public:
	CZoomDlg();
	~CZoomDlg();

	void SetCHInfo(unsigned char chnn, ULONGLONG authCH, bool bHidePanel = false);
	bool IsInZoom(){return m_bIsInZoom;}
	
protected:
	DECLARE_MSG_MAP()
	void OnInitial();
	void OnDestroy();

	void OnPaint(GUI::PAINT_STATUS status);

	//////////////////////////////////////////////////////////////////////////
	unsigned long OnEnter(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);
	unsigned long OnMove(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);
	unsigned long OnMouseWheelAdd();
	unsigned long OnMouseWheelDec();	
	
private:
	bool IsAlmostAbove(unsigned long x1, unsigned long y1, unsigned long x2, unsigned long y2, unsigned long distance);

	void DealWheelZoomMsg(bool bZoomOut);
	//////////////////////////////////////////////////////////////////////////
	unsigned long m_x;
	unsigned long m_y;
	CZoomPanelDlg m_panel;

	bool m_bHideZoomPanel;
	float m_zoomParm;
	bool m_bIsInZoom;
};
#endif //_ZOOM_DLG_H_
