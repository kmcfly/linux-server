/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : lufen
** Date         : 2010-03-25
** Name         : CfgDriverInfo.h
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/

#ifndef _CFG_DRIVER_INFO_DLG_H_
#define _CFG_DRIVER_INFO_DLG_H_

#include "ListCtrl.h"
#include "CfgDlg.h"

using namespace GUI;

class CCfgDriverInfoDlg : public CCfgDlg
{
public:
	CCfgDriverInfoDlg();
	~CCfgDriverInfoDlg();

protected:
	void OnInitial();

protected: 
	bool ShowData(NCFG_ITEM_ID cfgID, const BYTE* pData, int len, bool bRefresh);
	bool SetData(NCFG_ITEM_ID cfgID, BYTE*& pData, int& len)  const;


	DECLARE_MSG_MAP()
	unsigned long OnClickList(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);

protected:
	GUI::CListCtrl	m_cfgDriverInfoList;
	unsigned int	m_DelayTime;

	enum LIST_ROW
	{
		ROW_DRIVER_NAME,
		ROW_DEVICE_NUM,
		ROW_COMPANY_NAME,
		ROW_SHUTDOWNDELAY,
	};
};




#endif

