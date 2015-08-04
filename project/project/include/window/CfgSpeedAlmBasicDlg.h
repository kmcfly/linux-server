/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : lufen
** Date         : 2010-03-30
** Name         : CfgSpeedAlmBasicDlg.h
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/

#ifndef _CFG_SPEED_ALM_BASIC_DLG_H_
#define _CFG_SPEED_ALM_BASIC_DLG_H_

#include "ListCtrl.h"
#include "CfgDlg.h"

using namespace GUI;

class CCfgSpeedAlmBasicDlg : public CCfgDlg
{
public:
	CCfgSpeedAlmBasicDlg();
	~CCfgSpeedAlmBasicDlg();

	//unsigned long PreCheckValue();

protected:
	void OnInitial();

	//void SetTip();
	//void OnChangeFocus(unsigned long oldID, unsigned long newID);

protected: 
	bool ShowData(NCFG_ITEM_ID cfgID, const BYTE* pData, int len, bool bRefresh);
	bool SetData(NCFG_ITEM_ID cfgID, BYTE*& pData, int& len)  const;


	DECLARE_MSG_MAP()
	unsigned long OnClickList(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);

protected:
	GUI::CListCtrl	m_cfgSpeedAlmBasicList;

	enum LIST_ROW
	{
		ROW_SPEED_SOURCE = 0,
		ROW_SPEED_UNIT    =1,
		ROW_HIGH_SPEED_CHECK =3,
		ROW_HIGH_SPEED_VALUE =4,
		ROW_LOW_SPEED_CHECK  =6,
		ROW_LOW_SPEED_VALUE  =7,
	};
};




#endif

