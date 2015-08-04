/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : hz
** Date         : 2009-10-14
** Name         : CfgNetAlarmDlg.h 
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/
#ifndef _CFG_NET_ALARM_DLG_H_
#define _CFG_NET_ALARM_DLG_H_

#include "CfgDlg.h"
#include "ListCtrl.h"
#include "CfgManDlg.h"
#include "CfgManDlg.cpp"
#include "CfgScheduleChildDlg.h"
#include "CfgNetAlarmHandingDlg.h"

using namespace GUI;

class CCfgNetAlarmDlg : public CCfgDlg
{
public:
	CCfgNetAlarmDlg();
	~CCfgNetAlarmDlg();

	void UpdateView(bool bRefresh);

protected:
	DECLARE_MSG_MAP()
	void OnInitial();
	
	unsigned long OnClickList(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);
	unsigned long OnClickListAll(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);

	enum COL_INDEX
	{
		COL_CHANNEL,
		COL_ENABLE,
		COL_IP,
		COL_NAME,
	};
	//ÖØÐ´
	bool ShowData(NCFG_ITEM_ID cfgID, const BYTE* pData, int len, bool bRefresh);
	bool SetData(NCFG_ITEM_ID cfgID, BYTE*& pData, int& len)  const;


private:
	GUI::CListCtrl m_listCtrl;
	GUI::CListCtrl m_listAll;
	GUI::CStatic   m_scAll;
};

class CCfgNetAlarmManDlg : public CCfgManDlg3<CCfgNetAlarmDlg,CCfgNetAlarmHandingDlg,CCfgScheduleChildDlg>
{
public:
	CCfgNetAlarmManDlg();
	virtual ~CCfgNetAlarmManDlg();

protected:
	void OnPreInitial();

protected:
	void GetItemList(CFGITEM_LIST& itemList) const;
};

#endif//_CFG_SENSOR_DLG_H_

