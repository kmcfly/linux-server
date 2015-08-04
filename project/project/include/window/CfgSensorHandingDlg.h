/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : hz
** Date         : 2009-08-01
** Name         : CfgSensorHandingDlg.h 
** Version      : 1.0
** Description  : 
** Modify Record: 2009-10-14 L
1:
***********************************************************************/
#ifndef _CFG_SENSOR_HANDING_DLG_H_
#define _CFG_SENSOR_HANDING_DLG_H_

#include "CfgDlg.h"
#include "ListCtrl.h"

#include "CfgTriggerDlg.h"
#include <map>

using namespace GUI;

class CCfgSensorHandingDlg : public CCfgDlg
{
public:
	CCfgSensorHandingDlg();
	~CCfgSensorHandingDlg();

protected:
	DECLARE_MSG_MAP()
	void OnInitial();
	void OnDestroy();

	void UpdateView(bool bRefresh);

	unsigned long OnClickList(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);
	unsigned long OnClickListAll(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);

	enum COL_INDEX
	{
		COL_CHANNEL,
#if !(defined(__CUSTOM_US02__) && defined(__CUSTOM_US02_NEW__))
		COL_HOLD_TIME,
#endif
		COL_TRIGGER,
	};

	bool ShowData(NCFG_ITEM_ID cfgID, const BYTE* pData, int len, bool bRefresh);
	bool SetData(NCFG_ITEM_ID cfgID, BYTE*& pData, int& len)  const;


private:
	GUI::CListCtrl m_listCtrl;
	GUI::CListCtrl m_listAll;
	GUI::CStatic   m_scAll;

	ALARM_TRIGGER_INFO* m_pTriggerInfo;
	ALARM_TRIGGER_INFO* m_pAllTriggerInfo;

	std::map<int, int>  m_rowChnnMap;
};

#endif//_CFG_SENSOR_ADVANCE_DLG_H_

