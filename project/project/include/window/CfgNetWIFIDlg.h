/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : lufen
** Date         : 2010-04-9
** Name         : CfgNetWIFIDlg.h
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/

#ifndef _CFG_NET_WIFI_DLG_H_
#define _CFG_NET_WIFI_DLG_H_

#include "ListCtrl.h"
#include "CfgDlg.h"
#include "ListCtrlEx.h"

using namespace GUI;

class CCfgNetWIFIDlg : public CCfgDlg
{
public:
	CCfgNetWIFIDlg();
	~CCfgNetWIFIDlg();

protected:
	void OnInitial();
	void OnDestroy();

protected: 
	bool ShowData(NCFG_ITEM_ID cfgID, const BYTE* pData, int len, bool bRefresh);
	bool SetData(NCFG_ITEM_ID cfgID, BYTE*& pData, int& len)  const;
	void SearchSignal(void);
	void CompareListItem(const BYTE* pData, bool bRefresh);
	void SetErrorText(unsigned char checkResult);

	DECLARE_MSG_MAP()
	unsigned long OnClickEnableList(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);
	//unsigned long OnClickSearchList(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);
	unsigned long OnClickInputPasswod(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);
	unsigned long OnBtnSetup();
	unsigned long OnBtnTest();
protected:
	GUI::CListCtrl	m_cfgListEnable;
	GUI::CStatic	m_cfgListStrSearch;
	GUI::CListCtrl  m_cfgListInputPass;
	GUI::CButton	m_cfgTestBtn;
	GUI::CButton	m_cfgSetupBtn;

	GUI::CListCtrlEx<NET_WIFI_MODEM> m_cfgListSignal;	//搜索到的信号列表		
	
	unsigned int   m_SelectItemBak;		
	WIFI_CONFIG    *m_pwificfg;

	enum LIST_ROW
	{
		ROW_WIFI_ENABLE = 0,
	};

	enum WIFI_RETURN_STATE
	{
		WIFI_CONNECT_SUCC = 1,
		WIFI_CONNECT_FAIL,
		WIFI_ERR_OTHER,
			
	};
};




#endif

