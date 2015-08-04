/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : L
** Date         : 2009-08-14
** Name         : CfgNetworkDlg.h
** Version      : 1.0
** Description  : 
** Modify Record:
1:2010-06-10,�Ѹ�ҳ���е�DDNS���÷ָ����������CfgNetMoreConfigDlg.cppҳ���� ZHL
***********************************************************************/

#ifndef _CFG_SIPSERVER_DLG_H_
#define _CFG_SIPSERVER_DLG_H_

#include "Dialog.h"
#include "ListCtrl.h"
#include "CfgDlg.h"

#if defined (__CUSTOM_IPV6__)
#include "IPv6Ctrl.h"
#endif
using namespace GUI;

class CCfgSipServerDlg : public CCfgDlg
{
public:
	CCfgSipServerDlg();
	~CCfgSipServerDlg();

protected:
	virtual void OnInitial();
	//�ڼ��IP������ȷ�Ե�ʱ��ú��������ⲿ����
	//unsigned long OnPreCommand(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);

	void SetTip();

	DECLARE_MSG_MAP()
	unsigned long OnClickList(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);

protected:
	//void CheckEnableUI( bool bRefresh, REFRASH_UI_NUM uiNum=REFRASH_UI_ALL );
	//void DoUpdateUI( GET_IP_MODE& getIpMode, bool bRefresh,REFRASH_UI_NUM uiNum);

	unsigned long CheckValue();
	unsigned long PreCheckValue();

	bool IsValidID(const string &strID);
protected:
	bool ShowData(NCFG_ITEM_ID cfgID, const BYTE* pData, int len, bool bRefresh);
	bool SetData(NCFG_ITEM_ID cfgID, BYTE*& pData, int& len)  const;

protected:
	enum LIST_ROW
	{
		ROW_SIP_SERVER_IP = 0,
		ROW_SIP_SERVER_PORT = 1,
		ROW_SIP_EXPIRES  = 2,
		ROW_SIP_SERVER_ID = 3,
		ROW_SIP_USER_NAME = 4,
		ROW_SIP_PASSWORD = 5,
		ROW_DVR_ID		= 6,
		ROW_DVR_PORT	= 7,
		ROW_DVR_DEVOWNER = 8,
		ROW_DVR_CIVILCODE = 9,
		ROW_DVR_INSTALLADDR = 10,
		ROW_NUM = 11,
	};
	enum LIST_COLUMN
	{
		COLUMN_DESCRIBE,
		COLUMN_DATA,
	};

	GUI::CListCtrl	m_cfgSipServerList;
};

#endif

