/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : L
** Date         : 2009-10-16
** Name         : CfgLiveHostDlg.h 
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/

#ifndef _CFG_LIVE_HOST_DLG_H_
#define _CFG_LIVE_HOST_DLG_H_

#include "CfgDlg.h"
#include "ListCtrl.h"
#include "ComboCtrl.h"
#include "DwellModeDlg.h"
#include "DwellModeDlgNXN.h"

using namespace GUI;

class CCfgLiveHostDlg : public CCfgDlg
{
public:
	CCfgLiveHostDlg();
	~CCfgLiveHostDlg();

	void OnModifyCfgData(NCFG_ITEM_ID cfgID);
	void OnModifyGroupInfo(const std::string& str, bool bRefresh = true);

	unsigned long CheckDwell();

	unsigned long CheckValue()
	{
		if (0 != CheckDwell())
		{
			return E_STR_ID_TIP_02_03;
		}
		else
		{
			return 0;
		}
	}
protected:
	void OnInitial();
	void OnChangeFocus(unsigned long oldID, unsigned long newID);

protected:
	bool ShowData(NCFG_ITEM_ID cfgID, const BYTE* pData, int len, bool bRefresh);
	bool SetData(NCFG_ITEM_ID cfgID, BYTE*& pData, int& len)  const;

protected:
	int GetPageIndex(unsigned long mode);

protected:
	DECLARE_MSG_MAP()
	unsigned long OnClickComboMode();
	unsigned long OnClickDwellModeDlg(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);

protected:
	GUI::CComboCtrl			m_comboMode;
	GUI::CStatic			m_scMode;
	GUI::CStatic			m_scGroupInfo;

	CDwellModeDlg			m_dwellModeDlg;
protected:
	//子页面直接修改此处值,并通知本窗口,有值变动
	unsigned long*			m_data[CDwellModeDlg::PAGE_MAX];	//
	unsigned long			m_dwellTime[32];


};


#endif
