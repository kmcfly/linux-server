/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : L
** Date         : 2009-08-17
** Name         : PtzControlDlg.h
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/

#ifndef _PTZ_CONTROL_DLG_H_
#define _PTZ_CONTROL_DLG_H_

#include "Dialog.h"
#include "SliderCtrl.h"
#include "Static.h"
#include "MessageMan.h"

using namespace GUI;

typedef enum _ptz_func_mode
{
	PTZ_FUNC_NULL,
	PTZ_FUNC_ZOOM,
	PTZ_FUNC_FOCUS,
	PTZ_FUNC_IRIS,
	PTZ_FUNC_SPEED,
	PTZ_FUNC_PRESET,
	PTZ_FUNC_CRUISE,
	PTZ_FUNC_TRACK
}PTZ_FUNC_MODE;

class CPtzControlDlg : public GUI::CDialog
{
public:
	CPtzControlDlg();
	~CPtzControlDlg();
	BYTE GetPTZSpeed(){ return m_sliderSpeed.GetPos() + 1;}
	unsigned long ResponseHideCommond(  unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);

	enum
	{
		WIDTH = 255,
		HEIGHT = 140,
	};

	void SetPTZFuncMode(PTZ_FUNC_MODE mode);
protected:
	void OnInitial();
	void OnPaint(PAINT_STATUS status);

protected:
	DECLARE_MSG_MAP()
	unsigned long OnSpeedChange(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);
	unsigned long OnUp(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);
	unsigned long OnDown(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);
	unsigned long OnRight(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);
	unsigned long OnLeft(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);
	unsigned long OnStop();
	unsigned long OnUpRight(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);
	unsigned long OnUpLeft(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);
	unsigned long OnDownRight(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);
	unsigned long OnDownLeft(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);
	unsigned long OnNear(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);
	unsigned long OnFar(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);
	unsigned long OnZoomOut(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);
	unsigned long OnZoomIn(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);
	unsigned long OnIrisOpen(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);
	unsigned long OnIrisClose(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);
	
	unsigned long OnPTZSpeedAdd();
	unsigned long OnPTZSpeedDec();
	void SpeedChange();
	unsigned long OnMouseLftBttnUp( unsigned long ctrlID );


	bool ReleaseMsgBuff(MESSAGE_DATA &msgData);

protected:
	
	GUI::CStatic			m_scSpeed;
	GUI::CSliderCtrl		m_sliderSpeed;

	GUI::CButton			m_btnZoomSub;
	GUI::CButton			m_btnFocusSub;
	GUI::CButton			m_btnApertureSub;

	GUI::CStatic			m_scZoom;
	GUI::CStatic			m_scFocus;
	GUI::CStatic			m_scAperture;

	GUI::CButton			m_btnZoomAdd;
	GUI::CButton			m_btnFocusAdd;
	GUI::CButton			m_btnApertureAdd;


	GUI::CButton			m_btnUp;
	GUI::CButton			m_btnUpRight;
	GUI::CButton			m_btnRight;
	GUI::CButton			m_btnDownRight;
	GUI::CButton			m_btnDown;
	GUI::CButton			m_btnDownLeft;
	GUI::CButton			m_btnLeft;
	GUI::CButton			m_btnUpLeft;
	GUI::CButton			m_btnStop;

	//GUI::CButton			m_btnExit;

	GUI::CRect	m_rectPtzCtrl;//坐标相对于对话框
};


#endif

