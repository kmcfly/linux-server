/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : hz
** Date         : 2009-08-01
** Name         : CfgMotionDlg.h 
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/
#ifndef _CFG_NEW_MOTION_DLG_H_
#define _CFG_NEW_MOTION_DLG_H_

#include "CfgDlg.h"
#include "CfgMotionDlg.h"
#include "CfgScheduleChildDlg.h"
#include "TabCtrl.h"

#include "CfgDlg.h"
#include "ListCtrl.h"
#include "Anchor.h"

#include "CfgTriggerDlg.h"

#include "CfgScheduleChildDlg.h"
#include "MotionAreDlg.h"

#include <map>

class CSubCfgMotionManDlg : public CCfgDlg
{
public:
	CSubCfgMotionManDlg();
	~CSubCfgMotionManDlg();

	CCfgMotionDlg 	m_cfgMotionDlg;
	CCfgScheduleChildDlg 	m_cfgScheduleChildDlg;

	/*bool ShowData(NCFG_ITEM_ID cfgID, const BYTE* pData, int len, bool bRefresh);
	bool SetData(NCFG_ITEM_ID cfgID, BYTE*& pData, int& len) const;*/

	enum PAGE_ITEM
	{
		PAGE_MOTION,
		PAGE_SCHEDULE,
	};
	bool IsModify() const;
	void SetModify(bool modify);

	void SetTip();
protected:
	void OnInitial();
protected:
	GUI::CTabCtrl			m_tabCtrl;

private:
	


	PAGE_ITEM				m_curPage;

};

#endif