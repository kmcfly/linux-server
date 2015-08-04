/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : lufen
** Date         : 2010-04-30
** Name         : CfgNetMailDlg.h
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/

#ifndef _CFG_NET_EMAIL_DLG_H_
#define _CFG_NET_EMAIL_DLG_H_

#include "ListCtrl.h"
#include "CfgDlg.h"

using namespace GUI;

class CCfgNetEmailDlg : public CCfgDlg
{
public:
	CCfgNetEmailDlg();
	~CCfgNetEmailDlg();

	void DoOther();
	void EndOther();

protected:
	void OnInitial();

#ifdef __CUSTOM_US02_NEW__
	void SetTip();
	void TipAll(WND_LIST child, ENUM_STRING_ID strId);
#endif

protected: 
	bool ShowData(NCFG_ITEM_ID cfgID, const BYTE* pData, int len, bool bRefresh);
	bool SetData(NCFG_ITEM_ID cfgID, BYTE*& pData, int& len)  const;
	void TestEmailInfo(void);
	void SetErrorText(unsigned char checkResult);

	DECLARE_MSG_MAP()
	unsigned long OnClickList(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);

protected:
	GUI::CListCtrl	m_cfgEmailList;

	enum LIST_ROW
	{
		ROW_SMTP_NAME ,
		ROW_SMTP_PORT ,
		ROW_SLL_CHECK ,
		ROW_SEND_ADDR ,
		ROW_SEND_PASS ,
		ROW_RECV_ADDR = 5,
		ROW_TEST      = 8,
		ROW_ADV       = 9,
		ROW_IMG_CHECK   = 10,
		//ROW_SNAP_TIME = 11,

		ROW_COUNT,
	};

	enum EMAIL_RETURN_STATE
	{
		EMAIL_TEST_SUCC = 1,
		EMAIL_TEST_FAIL,
		EMAIL_TEST_OTHER,
	};
};




#endif

