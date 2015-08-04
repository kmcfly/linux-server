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
#ifndef _ZOOM_PANEL_DLG_H_
#define _ZOOM_PANEL_DLG_H_

#include "Dialog.h"
#include "Button.h"

using namespace GUI;

class CZoomPanelDlg : public GUI::CDialog
{
public:
	CZoomPanelDlg();
	~CZoomPanelDlg();

	void SetCHInfo(unsigned char chnn, ULONGLONG authCH){m_channel = chnn; m_authCH = authCH;}
	void OnMove(unsigned long x1, unsigned y1, unsigned long x2, unsigned y2);
	void GetZoomXYinfo(ZOOM_INFO &zoomInfo){zoomInfo.x = m_zoomInfo.x;zoomInfo.y =m_zoomInfo.y;};
	void SetZoomWHinfo(ZOOM_INFO zoomInfo){m_zoomInfo.width = zoomInfo.width;m_zoomInfo.height = zoomInfo.height;m_offsetX = zoomInfo.width / 16;
	m_offsetY = zoomInfo.height / 32;m_zoomInfo.x=zoomInfo.x ;m_zoomInfo.y=zoomInfo.y;};
protected:
	DECLARE_MSG_MAP()
	void OnInitial();
	void OnDestroy();
	
	unsigned long OnCH(int ctrlID);
	unsigned long OnUp();
	unsigned long OnRight();
	unsigned long OnDown();
	unsigned long OnLeft();
private:
	unsigned char m_channel;
	unsigned short m_offsetX, m_offsetY;
	ULONGLONG	m_authCH;
	GUI::CButton *m_pbtnCH;
	ZOOM_INFO m_zoomInfo;
	GUI::CButton m_btnUp, m_btnRight, m_btnDown, m_btnLeft;
};
#endif //_ZOOM_PANEL_DLG_H_
