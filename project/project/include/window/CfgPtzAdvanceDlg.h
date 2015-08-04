/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : hz
** Date         : 2009-08-14
** Name         : CfgPtzAdvanceDlg.h 
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/

#ifndef _CFG_PTZ_ADVANCE_DLG_H_
#define _CFG_PTZ_ADVANCE_DLG_H_

#include "CfgDlg.h"
#include "ListCtrl.h"
#include "PTZViewDlg.h"
#include "PTZTrackCtrl.h"
#include "CfgPtzCruiseDlg.h"

#include <map>

using namespace GUI;

class CCfgPtzAdvanceDlg : public CCfgDlg
{
public:
	CCfgPtzAdvanceDlg();
	~CCfgPtzAdvanceDlg();
	void SetListBtn(std::vector<PTZ_SERIAL_INFO>& ptzConfigList, bool Refresh);
	void GetConfigList(std::vector<PTZ_SERIAL_INFO>& ptzConfigList);


protected:
	void OnInitial();
	void OnDestroy();

protected:
	DECLARE_MSG_MAP()
	unsigned long OnClickList(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);
	unsigned long OnClickListAll( unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos );
	
	enum COL_INDEX
	{
		COL_CHANNEL,
		COL_PRESET,
		COL_CRUISE,
		COL_TRACK,
	};
	//ÖØÐ´
	void UpdateView(bool bRefresh);
	bool ShowData(NCFG_ITEM_ID cfgID, const BYTE* pData, int len, bool bRefresh);
	bool SetData(NCFG_ITEM_ID cfgID, BYTE*& pData, int& len)  const;

	void GetSubItemInfoFromDlg( const NCFG_ITEM_ID itemID, unsigned short& subItemCount, unsigned short& subItemLen);
	unsigned long RecoverChange();

protected:
	GUI::CListCtrl			m_listCtrl;
	GUI::CListCtrl			m_listAll;

	GUI::CStatic			m_scAll;

	CPTZViewDlg m_ptzViewDlg;
	CPTZTrackCtrl m_ptzTrackDlg;
	// m_cruiseDlg;

	std::vector<PTZ_PRESET> * m_pPresetList;
	std::vector<CRUISE_INFO> * m_pCruiseList;
	std::vector<PTZ_SERIAL_INFO> m_ptzConfigList;

	std::map<int, int>		m_rowChnnMap;
};



#endif



