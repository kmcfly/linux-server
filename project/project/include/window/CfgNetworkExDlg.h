/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : zxx
** Date         : 2011-07-15
** Name         : CfgNetworkExDlg.h
** Version      : 1.0
** Description  : 
** Modify Record:

***********************************************************************/

#ifndef _CFGNETWORKDLGEX_H_
#define _CFGNETWORKDLGEX_H_

#include "Dialog.h"
#include "ListCtrl.h"
#include "CfgDlg.h" 
#include "RadioCtrl.h"
#include "Static.h"
#include "CfgLocalNetModeDlg.h"
#include "CfgWireLessModeDlg.h"
using namespace GUI;



class CCfgNetworkExDlg : public CCfgDlg
{
public:
	enum PAGE_ITEM
	{
		PAGE_NETWORK  =0,
		PAGE_WIRELESS = 1,
		PAGE_NULL  = 2,
	};

	CCfgNetworkExDlg();
	~CCfgNetworkExDlg();
	void SetDataInfo(SETUPDATAINFO DataInfo) { m_datainfo = DataInfo;}
	int GetCurPage(){return m_curPage;}
public:
	//检查数据是否被修改
	void OnModifyNetModeData(unsigned long cfgId);
protected:
	virtual void OnInitial();
	void OnPaint(PAINT_STATUS status);
	void CheckUI(bool bRefresh);
	void SetPage(PAGE_ITEM item = PAGE_NETWORK,bool bRefresh = true );
protected:
	bool ShowData(NCFG_ITEM_ID cfgID, const BYTE* pData, int len, bool bRefresh);
	bool SetData(NCFG_ITEM_ID cfgID, BYTE*& pData, int& len)  const;

	//刷新数据到界面
	void RefreshView(bool bRefresh=true);

	DECLARE_MSG_MAP()
	
	unsigned long CheckValue();
	unsigned long OnClickList(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);
	unsigned long OnClickRadio();
protected:
	GUI::CListCtrl			m_cfgNetworkList;

	enum LIST_ROW
	{
		ROW_HTTP_PORT = 0,
		ROW_SERVER_PORT = 1,
	};

	GUI::CRadioGroup	m_rdGroup;
	GUI::CRadioCtrl		m_rdNetWork;
	GUI::CRadioCtrl		m_rdWireless;
	GUI::CStatic		m_scInfo;//和2个单选框在同一行的静态框

	CCfgLocalNetModeDlg m_localNetModeDlg;
	CCfgWireLessModeDlg m_wirelessModeDlg;

	int m_poeNum;

private:
	PAGE_ITEM	m_curPage;
	SETUPDATAINFO m_datainfo;
};


#endif

