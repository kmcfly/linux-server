/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : L
** Date         : 2009-01-20
** Name         : CfgTriggerToPtzDlg.h
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/
#ifndef _CFG_TRIGGER_TO_PTZ_DLG_H_
#define _CFG_TRIGGER_TO_PTZ_DLG_H_

#include "Dialog.h"
#include "ListCtrl.h"
#include "CfgDlg.h"
#include "WndStructDef.h"
#include "CfgCruiseDlg.h"

#include <map>

using namespace GUI;

class CCfgTriggerToPtzDlg : public CCfgDlg
{
public:
	CCfgTriggerToPtzDlg();
	~CCfgTriggerToPtzDlg();

	void SetAlarmTriggerInfo(ALARM_TRIGGER_INFO* pTrigger) { m_pTriggerInfo = pTrigger;}

	void InitCtrlData(bool bRefresh);

	unsigned long OnApply();

protected:
	void OnInitial();
	void OnDestroy();

	bool ShowData(NCFG_ITEM_ID cfgID, const BYTE* pData, int len, bool bRefresh);

protected:
	DECLARE_MSG_MAP()
	unsigned long OnClickList(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);

protected:
	GUI::CListCtrl	m_listCtrl;

protected:
	enum LIST_COL
	{
		COL_CHNN,
		COL_TYPE,
		COL_NO,
	};

	enum TOPTZ_TYPE
	{
		TOPTZ_TYPE_NO,
		TOPTZ_TYPE_PRESET,
		TOPTZ_TYPE_CRUISE,
		TOPTZ_TYPE_TRACK,
	};

	std::vector<PTZ_PRESET>*	m_pPresetList;
	std::vector<CRUISE_INFO>*	m_pCruiseList;
	TOPTZ_TYPE*							m_pLastToptzType;

	std::map<int, int>  m_rowChnnMap;
protected:
	ALARM_TRIGGER_INFO*		m_pTriggerInfo;

};


#endif
