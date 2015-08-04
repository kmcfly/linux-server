/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : xzw
** Date         : 2010-10-11
** Name         : CfgNetFtpDlg.h
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/

#ifndef _CFG_NET_FTP_DLG_H_
#define _CFG_NET_FTP_DLG_H_

#include "ListCtrl.h"
#include "CfgDlg.h"

using namespace GUI;

class CCfgNetFtpDlg : public CCfgDlg
{
public:
	CCfgNetFtpDlg();
	~CCfgNetFtpDlg();

	void DoOther();
	void EndOther();

protected:
	void OnInitial();

protected: 
	bool ShowData(NCFG_ITEM_ID cfgID, const BYTE* pData, int len, bool bRefresh);
	bool SetData(NCFG_ITEM_ID cfgID, BYTE*& pData, int& len)  const;
	void TestEmailInfo(void);
	void SetErrorText(unsigned char checkResult);

	DECLARE_MSG_MAP()
	unsigned long OnClickList(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);

protected:
	GUI::CListCtrl	m_cfgFtpList;

	enum LIST_ROW
	{
		ROW_FTP_ENABLE,
		ROW_FTP_SERVER_ADDR,
		ROW_FTP_SERVER_PORT,
		ROW_FTP_SSL_CHECK,
		ROW_FTP_ANONYMITY_LOGIN,
		ROW_FTP_USER_NAME,
		ROW_FTP_USER_PASS,
		ROW_FTP_UPDATE_URL,
		ROW_COUNT,
	};

	enum EMAIL_RETURN_STATE
	{
		FTP_TEST_SUCC = 1,
		FTP_TEST_FAIL,
		FTP_TEST_OTHER,
	};
};




#endif

