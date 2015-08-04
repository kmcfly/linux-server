/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : hz
** Date         : 2009-08-01
** Name         : CfgSensorDlg.h 
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/
#ifndef _CFG_SENSOR_DLG_H_
#define _CFG_SENSOR_DLG_H_

#include "CfgDlg.h"
#include "ListCtrl.h"

#include <map>

using namespace GUI;

class CCfgSensorDlg : public CCfgDlg
{
public:
	CCfgSensorDlg();
	~CCfgSensorDlg();

	void UpdateView(bool bRefresh);

protected:
	DECLARE_MSG_MAP()
	void OnInitial();
	void OnDestroy();
	
	unsigned long OnClickList(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);
	unsigned long OnClickListAll(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);

	enum COL_INDEX
	{
		COL_CHANNEL,
		COL_ENABLE,
		COL_TYPE,
		COL_NAME,
	};
	//ÖØÐ´
	bool ShowData(NCFG_ITEM_ID cfgID, const BYTE* pData, int len, bool bRefresh);
	bool SetData(NCFG_ITEM_ID cfgID, BYTE*& pData, int& len)  const;


private:
	GUI::CListCtrl m_listCtrl;
	GUI::CListCtrl m_listAll;
	GUI::CStatic   m_scAll;

	std::map<int, int>  m_rowChnnMap;
};

#endif//_CFG_SENSOR_DLG_H_

