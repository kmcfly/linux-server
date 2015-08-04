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

#ifndef __CFG_NETWORK_FTP_DLG_H__
#define __CFG_NETWORK_FTP_DLG_H__

#include "ListCtrl.h"
#include "CfgDlg.h"

#include "CfgNetworkFtpInfoDlg.h"

using namespace GUI;

class CCfgNetworkFtpDlg : public CCfgDlg
{
public:
	CCfgNetworkFtpDlg();
	~CCfgNetworkFtpDlg();

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

	unsigned long OnFtpCfgInfoChanged();
protected:

	CCfgNetworkFtpInfoDlg m_triggerDlg;
};
#endif


