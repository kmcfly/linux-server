/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : zxx
** Date         : 2012-11-19
** Name         : ShanghaiCfgDailyImageDialog.h
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/

#ifndef __SHANGHAI_CFGDAILYIMAGE_DIALOG_H__
#define __SHANGHAI_CFGDAILYIMAGE_DIALOG_H__

#include "CheckCtrl.h"
#include "ListCtrlEx.h"
#include "CfgDlg.h"
#include "ShanghaiCfgDailySelectDialog.h"
#include "ShanghaiCmd.h"
#include "CharCodeTable.h"

class CShanghaiCfgDailyimageDialog:public CCfgDlg
{
public:
	CShanghaiCfgDailyimageDialog();
	~CShanghaiCfgDailyimageDialog();
	unsigned long SetListData(SNAP_INFOEX *pListData);//获取新加的m_listEx里面的内容,返回值是m_listEx列表的个数
protected:
	DECLARE_MSG_MAP()
	void OnInitial();

	bool ShowData(NCFG_ITEM_ID cfgID, const BYTE* pData, int len, bool bRefresh);
	bool SetData(NCFG_ITEM_ID cfgID, BYTE*& pData, int& len) const;

	unsigned long OnBtnAdd();
	unsigned long OnBtnDel();
protected:
	GUI::CListCtrlEx <SNAP_INFOEX> m_listCtrlEx;
	GUI::CButton				   m_BtnAdd;
	GUI::CButton				   m_BtnDel;
private:
	string ChangeChinese(const char *pstr);
private:
	CShanghaiCfgDailySelectDialog m_dailySelectDialog;
	CCharCodeTable* m_codeTable; 
};

#endif
//end