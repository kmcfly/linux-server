/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : xzw
** Date         : 2010-10-11
** Name         : CfgNetPhoneDlg.h
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/

#ifndef _CFG_NET_PHONE_DLG_H_
#define _CFG_NET_PHONE_DLG_H_

#include "ListCtrl.h"
#include "CfgDlg.h"

using namespace GUI;

class CCfgNetPhoneDlg : public CCfgDlg
{
public:
	CCfgNetPhoneDlg();
	~CCfgNetPhoneDlg();

	//void DoOther();
	//void EndOther();

protected:
	void OnInitial();

protected: 
	bool ShowData(NCFG_ITEM_ID cfgID, const BYTE* pData, int len, bool bRefresh);
	bool SetData(NCFG_ITEM_ID cfgID, BYTE*& pData, int& len)  const;
	//void TestEmailInfo(void);
	//void SetErrorText(unsigned char checkResult);

	DECLARE_MSG_MAP()
	unsigned long OnClickList(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);

protected:
	GUI::CListCtrl	m_cfgPhoneList;

	enum LIST_ROW
	{		
		ROW_PHONE_IP,
		ROW_PHONE_PORT,
		ROW_COUNT,
		
	};

	
};




#endif

