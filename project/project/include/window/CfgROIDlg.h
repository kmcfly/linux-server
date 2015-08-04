/*
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : xian shi wei
** Date         : 2014-10-15
** Name         : CfgROIDlg.h
** Version      : 1.0
** Description  : 
** Modify Record:
*/

#ifndef __CFG_ROI_DLG_H__
#define __CFG_ROI_DLG_H__

#include "Wnd.h"
#include "Dialog.h"
#include "ListCtrl.h"
#include "Button.h"

#include "CfgDlg.h"

#include "SetROIDlg.h"

#include <map>

using namespace GUI;

class CCfgROIDlg : public CCfgDlg
{
public:
	CCfgROIDlg();
	~CCfgROIDlg();
    
protected:
	void OnInitial();
	void OnDestroy();

	void SetTip();
	void DoOther();

protected:
	bool ShowData(NCFG_ITEM_ID cfgID, const BYTE* pData, int len, bool bRefresh);
	bool SetData(NCFG_ITEM_ID cfgID, BYTE*& pData, int& len)  const;

protected:
	DLG_END_VALUE PopupSetROIDlg(int chnn, VIDEO_CONFIG_ROI_INFO& ROIInfo);

protected:
	DECLARE_MSG_MAP()
	unsigned long OnClickList(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);

protected:
	GUI::CListCtrl		m_listCtrl;

	VIDEO_CONFIG_ROI_INFO*	m_pROIInfo;
	unsigned int			m_buffLen;

	CSetROIDlg		        m_setROIDlg;

	std::map<int, int>  m_rowChnnMap;
	int                 m_localInputNum;
    int                 m_crruChnn;

	enum LIST_COL
	{
		COL_CHNN,
		COL_VIDEO_ROI,
	};
};

#endif
