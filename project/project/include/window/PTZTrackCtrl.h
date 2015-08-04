/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : L
** Date         : 2009-08-17
** Name         : PTZTrackCtrl.h 
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/

#ifndef _PTZ_TRACK_CTRL_H_
#define _PTZ_TRACK_CTRL_H_

#include "Dialog.h"
#include "ComboCtrl.h"
#include "Static.h"
#include "GroupCtrl.h"
#include "MessageMan.h"

using namespace GUI;

class CPTZTrackCtrl : public GUI::CDialog
{
public:
	CPTZTrackCtrl();
	~CPTZTrackCtrl();

	enum
	{
		WIDTH = 280,
		HEIGHT = 100,
	};
	
	void SetPTZConfig(std::vector<PTZ_SERIAL_INFO> ptzConfig){m_ptzConfig = ptzConfig;}
	void ChangeBtnCaption(bool bChnnChanged);
protected:
	DECLARE_MSG_MAP()
	void OnInitial();
	void OnPaint(PAINT_STATUS status);
	void OnDestroy();

	unsigned long OnClickTrack();
	unsigned long OnClickRecord();
	unsigned long OnClickBrush();
	unsigned long OnClickLamp();

	void RefalseBtnCaption(bool refresh = true);
	bool ReleaseMsgBuf(MESSAGE_DATA & msgData);
	//////////////////////////////////////////////////////////////////////////
	GUI::CButton			m_btnTrack;
	GUI::CButton			m_btnRecord;	

	GUI::CGroupCtrl			m_groupCtrl;

	BYTE					m_channel;
	std::vector<PTZ_SERIAL_INFO> m_ptzConfig;
	GUI::CButton	m_btnBrush;
	GUI::CButton	m_btnLamp;
};


#endif
