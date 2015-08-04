/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : hz
** Date         : 2009-08-14
** Name         : CfgPtzSerialPortDlg.h 
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/

#ifndef _CFG_PTZ_SERIAL_PORT_DLG_H_
#define _CFG_PTZ_SERIAL_PORT_DLG_H_

#include "CfgDlg.h"
#include "ListCtrl.h"

#include <map>

using namespace GUI;

class CCfgPtzSerialPortDlg : public CCfgDlg
{
public:
	CCfgPtzSerialPortDlg();
	~CCfgPtzSerialPortDlg();

protected:
	void OnInitial();
	void OnDestroy();
	void UpdateView(bool bRefresh);

	unsigned long OnClickList(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);
	unsigned long OnClickListAll(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);

	enum COL_INDEX
	{
		COL_CHANNEL,
		COL_ENABLE,
		COL_ADDRESS,
		COL_BAUD_RATE,
		COL_PROTOCOL,
		COL_CURISE_TYPE
	};
	//ÖØÐ´
	bool ShowData(NCFG_ITEM_ID cfgID, const BYTE* pData, int len, bool bRefresh);
	bool SetData(NCFG_ITEM_ID cfgID, BYTE*& pData, int& len)  const;

	DECLARE_MSG_MAP()

protected:
	GUI::CListCtrl			m_listCtrl;
	GUI::CListCtrl			m_listAll;
	GUI::CStatic			m_scAll;

	std::map<int, int>		m_rowChnnMap;
};



#endif
