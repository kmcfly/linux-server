/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : L
** Date         : 2009-08-17
** Name         : PTZLiveCtrl.h 
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/

#ifndef _PTZ_LIVE_CTRL_H_
#define _PTZ_LIVE_CTRL_H_

#include "Dialog.h"
#include "ComboCtrl.h"
#include "Static.h"
#include "NetConfigDefine.h"
#include "ConfigBlock.h"
#include "MessageMan.h"

using namespace GUI;

class CPTZLiveCtrl : public GUI::CDialog
{
public:
	CPTZLiveCtrl();
	~CPTZLiveCtrl();
	unsigned long ResponseHideCommond( unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);
	enum
	{
		WIDTH = 380,
		HEIGHT = 100,
	};

	void SetChnn(ULONGLONG CH);
	void ChangeBtnCaption(bool bChnnChanged);
	
protected:
	void OnInitial();
	void OnDestroy();
	void OnPaint(PAINT_STATUS status);
	bool LoadPTZConfigInfo();
	void SetBtnFunction();
	unsigned short GetDataCount( NCFG_ITEM_ID itemID, CConfigBlock & CfgBlock );

protected:
	DECLARE_MSG_MAP()
	unsigned long OnClickTrack();
	unsigned long OnClickScan();
	unsigned long OnClickChnn();
	unsigned long OnChangeChnn( unsigned long keyID );
	unsigned long OnClickPresetGo();
	unsigned long OnPresetGo();
	unsigned long OnClickCruise();
	unsigned long OnClickCruiseRun();
	unsigned long OnClickBrush();
	unsigned long OnClickLamp();

protected:
	bool SendPtzCtrl(PTZ_CTRL& ptzCtrl);
	void RefreshBtnCaption(unsigned long ptzState, bool bRefresh);
	unsigned long GetPtzState(unsigned long chnn);

	//////////////////////////////////////////////////////////////////////////

	ULONGLONG m_viewChnn;

	//////////////////////////////////////////////////////////////////////////
	GUI::CStatic	m_scPreset;
	GUI::CStatic	m_scCruise;

	GUI::CComboCtrl	m_comboPreset;
	GUI::CComboCtrl	m_comboCruise;

	GUI::CButton	m_btnPreset;
	GUI::CButton	m_btnCruise;
	GUI::CButton	m_btnTrack;
	GUI::CButton	m_btnAutoScan;
	GUI::CButton	m_btnStartCruise;

	GUI::CButton	m_btnBrush;
	GUI::CButton	m_btnLamp;
	
	//记录预置点、巡航线、和云台配置信息
	std::vector< std::vector<PTZ_PRESET> > m_presetList;
	std::vector< std::vector<NCFG_INFO_CRUISE_HEAD> > m_curiseList;
	std::vector<PTZ_SERIAL_INFO> m_ptzConfig;
};


#endif

