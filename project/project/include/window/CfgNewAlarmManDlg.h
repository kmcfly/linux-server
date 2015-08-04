/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : hz
** Date         : 2009-08-01
** Name         : CfgAlarmManDlg.h 
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/
#ifndef _CFG_NEW_ALARM_MAN_DLG_H_
#define _CFG_NEW_ALARM_MAN_DLG_H_

#include "Dialog.h"
#include "Static.h"
#include "Anchor.h"

#include "CfgMotionDlg.h"
#include "CfgOtherAlarmDlg.h"
#include "CfgSensorManDlg.h"
#include "CfgAlarmOutDlg.h"
#include "CfgVideoLossDlg.h"
#include "CfgAlarmOutBuzzerDlg.h"
#include "CfgAlarmBuzzerDlg.h"

#ifdef __MOBILE_DVR__
#include "CfgSpeedAlarmDlg.h"
#endif

using namespace GUI;

class CCfgNewAlarmManDlg : public GUI::CDialog
{
public:
	CCfgNewAlarmManDlg();
	~CCfgNewAlarmManDlg();

protected:
	DECLARE_MSG_MAP()
	void OnInitial();

private:
	//////////////////////////////////////////////////////////////////////////
	unsigned long OnSensor();
	// unsigned long OnMotion();
	unsigned long OnVideoLoss();
	unsigned long OnOtherAlarm();
	unsigned long OnAlarmOut();
	unsigned long OnExit();

#ifdef __MOBILE_DVR__
	unsigned long OnSpeedAlarm();
#ifndef __DVR_ANCHOR__
	GUI::CStatic			m_stcSpeedAlarm;
	GUI::CButton			m_btnSpeedAlarm;
	CCfgSpeedAlarmDlg		m_SpeedAlarmDlg;
#else
	GUI::CAnchor			m_anSpeedAlarm;
	CCfgSpeedAlarmAnchorDlg m_SpeedAlarmDlg;
#endif
#endif
	//////////////////////////////////////////////////////////////////////////

#ifndef __DVR_ANCHOR__
	GUI::CStatic m_stcSensor;
	// GUI::CStatic m_stcMotion;
	GUI::CStatic m_stcVideoLoss;
	GUI::CStatic m_stcOtherAlarm;
	GUI::CStatic m_stcAlarmOut;
	GUI::CStatic m_stcExit;

	GUI::CButton m_btnSensor;
	// GUI::CButton m_btnMotion;
	GUI::CButton m_btnVideoLoss;
	GUI::CButton m_btnOtherAlarm;
	GUI::CButton m_btnAlarmOut;
	GUI::CButton m_btnExit;
#else
	GUI::CAnchor		m_anSensor;
	GUI::CAnchor		m_anMotion;
	GUI::CAnchor		m_anVideoLoss;
	GUI::CAnchor		m_anOtherAlarm;
	GUI::CAnchor		m_anAlarmOut;
	GUI::CAnchor		m_anExit;
#endif

#ifndef __DVR_ANCHOR__
	CCfgSensorManDlg		m_sensorDlg;
	// CCfgMotionManDlg		m_motionManDlg;
	CCfgAlarmOutManDlg		m_alarmOutManDlg;
#else
	CCfgSensorAnchorDlg		m_sensorDlg;
	CCfgMotionManAnchorDlg	m_motionManDlg;
	CCfgAlarmOutManAnchorDlg m_alarmOutManDlg;
#endif

	CCfgAlarmOutBuzzerDlg		m_alarmOutBuzDlg;
	CCfgAlarmBuzzerDlg		m_alarmBuzzerDlg;
	CCfgOtherAlarmDlg		m_otherAlarmDlg;
	CCfgVideoLossDlg		m_videoLossDlg;
};

#endif//_CFG_ALARM_MAN_DLG_H_

