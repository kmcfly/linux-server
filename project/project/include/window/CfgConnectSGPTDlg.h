/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : zxx
** Date         : 2012-12-10
** Name         : CfgConnectSGPTDlg.h
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/

#ifndef _CFG_CONNECT_SGPT_H_
#define _CFG_CONNECT_SGPT_H_
#include "Dialog.h"
#include "CfgDlg.h"
#include "Button.h"
#include "NumEditCtrl.h"
#include "CheckCtrl.h"
#include "ListCtrl.h"
#include "ComboCtrl.h"
#include "CharCodeTable.h"
#include <string>
using namespace GUI;

class CCfgConnectSGPTDlg : public CCfgDlg
{
public:
	CCfgConnectSGPTDlg();
	~CCfgConnectSGPTDlg();
protected:
	void OnInitial();
	bool ShowData(NCFG_ITEM_ID cfgID, const BYTE* pData, int len, bool bRefresh);
	bool SetData(NCFG_ITEM_ID cfgID, BYTE*& pData, int& len)  const;
protected:
	DECLARE_MSG_MAP()
	unsigned long OnClickList(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);
protected:
	GUI::CListCtrl	m_cfgSGPTList;

private:
	string ChangeChinese(const char *pstr);
	void PtOperServer();//平台操作服务
private:
	enum LIST_ROW
	{
		ROW_SGPT =0,
		ROW_AGENTID,
		ROW_IPEDIT,  //IP或者域名编辑框
		ROW_PORT,
		ROW_TYPE,	//人员选择
		ROW_BTN,	//打开新页面的按钮
		ROW_NUM,
	};

	enum COMBO_ROW
	{
		ROW_OPER =0,
		ROW_KEEP,
		ROW_CHECK,
	};
	CCharCodeTable* m_codeTable; 
};

#endif

