/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author			: L
** Date				: 2009-11-24
** Name			: CfgLiveOsdDlg.h
** Version			: 1.0
** Description   : 
** Modify Record:
1:
***********************************************************************/

#ifndef _CFG_LIVE_OSD_DLG_H_
#define _CFG_LIVE_OSD_DLG_H_

#include "Dialog.h"
#include "CfgDlg.h"
#include "ListCtrl.h"
#include "SetPositionDlg.h"

#include <map>

using namespace GUI;

class CCfgRecOsdDlg : public CCfgDlg
{
public:
	CCfgRecOsdDlg();
	~CCfgRecOsdDlg();

	void UpdateView(bool bRefresh);

protected:
	void OnInitial();
	void OnDestroy();

	void SetTip();

protected:
	bool ShowData(NCFG_ITEM_ID cfgID, const BYTE* pData, int len, bool bRefresh);
	bool SetData(NCFG_ITEM_ID cfgID, BYTE*& pData, int& len)  const;

protected:
	void PopUpSetPositionDlg(unsigned short chnn, 
											bool bCam, unsigned short inCamX, unsigned short inCamY, 
											unsigned short& outCamX, unsigned short& outCamY,
											bool bTm, unsigned short inTmX, unsigned short inTmY,
											unsigned short& outTmX, unsigned short& outTmY);

protected:
	DECLARE_MSG_MAP()
	unsigned long OnClickList(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);
	unsigned long OnClickListAll(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);

protected:
	GUI::CListCtrl		m_listCtrl;
	GUI::CListCtrl		m_listAll;

	GUI::CStatic		m_scAll;

	CSetPositionDlg m_setPositionDlg;

protected:
	OSD_INFO*	m_pOsdCamera;
	OSD_INFO* m_pOsdTime;

	OSD_INFO m_allOsdCamera;
	OSD_INFO m_allOsdTime;

	std::map<int, int>  m_rowChnnMap;

protected:

	enum LIST_COL
	{
		COL_CHNN,
		COL_SHOW_NAME,
		COL_TIME_STAMP,
		COL_POS,
	};
};

#endif
