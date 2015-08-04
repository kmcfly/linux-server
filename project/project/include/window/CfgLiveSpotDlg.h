/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : L
** Date         : 2009-10-21
** Name         : CfgLiveSpotDlg.h 
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/

#ifndef _CFG_LIVE_SPOT_DLG_H_
#define _CFG_LIVE_SPOT_DLG_H_

#include "Dialog.h"
#include "CfgDlg.h"
#include "ComboCtrl.h"
#include "Static.h"
#include "DwellModeDlg.h"

using namespace GUI;

class CCfgLiveSpotDlg : public CCfgDlg
{
public:
	CCfgLiveSpotDlg();
	~CCfgLiveSpotDlg();

	void OnModifyCfgData(NCFG_ITEM_ID cfgID);
	void OnModifyGroupInfo(const std::string& str, bool bRefresh = true);
protected:
	void OnInitial();

protected:
	bool ShowData(NCFG_ITEM_ID cfgID, const BYTE* pData, int len, bool bRefresh);
	bool SetData(NCFG_ITEM_ID cfgID, BYTE*& pData, int& len)  const;

protected:
	DECLARE_MSG_MAP()
	unsigned long OnClickDwellModeDlg(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);

protected:
	GUI::CComboCtrl			m_comboMode;
	GUI::CStatic			m_scMode;
	GUI::CStatic			m_scCountInfo;

	CDwellModeDlg			m_dwellModeDlg;

protected:
	unsigned long*			m_data[6];				//只用到了一个
	unsigned long			m_dwellTime;
};

#endif
