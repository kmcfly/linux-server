/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : Z
** Date         : 20130129
** Name         : CfgNetworkFtpInfoDlg.h 
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/
#ifndef __CFG_NETWORK_FTP_INFO_DLG_H__
#define __CFG_NETWORK_FTP_INFO_DLG_H__

#include "Dialog.h"
#include "Static.h"
#include "CheckCtrl.h"
#include "ListCtrl.h"
#include "CfgDlg.h"
#include "WndStructDef.h"
#include "ScrollDialog.h"

using namespace GUI;

class CCfgNetworkFtpInfoDlg : public GUI::CScrollDialog
{
public:
	CCfgNetworkFtpInfoDlg();
	~CCfgNetworkFtpInfoDlg();

	void SetFtpCfgInfo(TVT_FTP_CONFIG * pFtpCfg, unsigned long maxFtpUpdateChannel)
	{	
		if (NULL != pFtpCfg)
		{
			memcpy(&m_ftpCfg, pFtpCfg, sizeof(TVT_FTP_CONFIG));
		}

		m_maxFtpUpdateChannel = maxFtpUpdateChannel;
	}

	void GetFtpCfgInfo(TVT_FTP_CONFIG * pFtpCfg) const
	{
		* pFtpCfg = m_ftpCfg;
	}

	void InitCtrlData(bool bRefresh); //初始化控件内容

protected:
	void OnInitial();
	void OnDestroy();
	void OnPaint(PAINT_STATUS status);

	void OnClickChnn(int index, bool setValue, bool bRefresh);
	void OnClickChnnAll(int index, bool setValue, bool bRefresh);

protected:
	DECLARE_MSG_MAP()
	unsigned long OnClickList(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);

protected:
	GUI::CListCtrl	m_listCtrl;

private:
	bool IsMoreUpdateChnn();
protected:
	enum LIST_ROW
	{
		ROW_FTP_ENABLE = 0,
		ROW_FTP_SERVER_ADDR = 1,
		ROW_FTP_SERVER_PORT = 2,
		//ROW_FTP_SSL_CHECK,
		ROW_FTP_ANONYMITY_LOGIN,
		ROW_FTP_USER_NAME,
		ROW_FTP_USER_PASS,
		ROW_FTP_FILE_LEN,
		ROW_FTP_UPDATE_URL,
		ROW_COUNT,
	};

	enum SW_ITEM_ID
	{
		SW_ITEM_ID_FTP_ENABLE		= 1,
		SW_ITEM_ID_FTP_SERVER_ADDR,
		SW_ITEM_ID_FTP_SERVER_PORT,
		//SW_ITEM_ID_FTP_SSL_CHEKC,
		SW_ITEM_ID_FTP_ANONYMITY,
		SW_ITEM_ID_FTP_USER_NAME,
		SW_ITEM_ID_FTP_PASSWORD,
		SW_ITEM_ID_FTP_FILE_LEN,
		SW_ITEM_ID_FTP_UPDATE_URL,

		SW_ITEM_ID_FTP_CH_GROUP= 200,
		SW_ITEM_ID_FTP_CH_0	= 220,
	};

protected:

	std::vector<GUI::CCheckCtrl*>	m_vecChnn;	//存储通道指针
	std::vector<GUI::CCheckCtrl*>	m_vecChnnAll;//存储All单选指针

	unsigned int		m_maxChnn;
	unsigned char		m_channel;

	TVT_FTP_CONFIG		m_ftpCfg;
	unsigned long		m_maxFtpUpdateChannel;
};


#endif /*__CFG_NETWORK_FTP_INFO_DLG_H__*/
