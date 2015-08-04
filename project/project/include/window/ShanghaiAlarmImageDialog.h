/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : zxx
** Date         : 2012-11-19
** Name         : ShanghaiAlarmImageDialog.h
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/

#ifndef __SHANGHAI_ALARMIMAGEDIALOG_H__
#define __SHANGHAI_ALARMIMAGEDIALOG_H__

#include "Dialog.h"
#include "ListCtrl.h"
#include "Static.h"
#include "CfgDlg.h"
#include "ComboCtrl.h"
#include "MessageMan.h"
#include "CharCodeTable.h"
using namespace GUI;

class CShanghaiAlarmImageDialog : public CCfgDlg
{
public:
	CShanghaiAlarmImageDialog();
	~CShanghaiAlarmImageDialog();

protected:
	void OnInitial();
	
	void SetTip();
public:
	virtual void UpdateView(bool bRefresh);
protected:
	virtual bool ShowData(NCFG_ITEM_ID cfgID, const BYTE* pData, int len, bool bRefresh);
	virtual bool SetData(NCFG_ITEM_ID cfgID, BYTE*& pData, int& len)  const;

protected:
	DECLARE_MSG_MAP()
	unsigned long OnClickList(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);
	unsigned long OnClickListAll(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);
	unsigned long OnClickCheck();
private:
	string  ChangeChinese(const char *pstr);
protected:
	GUI::CListCtrl			m_cfgList;
	GUI::CStatic			m_cfgScAll;
	GUI::CListCtrl			m_cfgListAll;

	GUI::CCheckCtrl			m_checkCtrl;
	GUI::CStatic			m_staticNote;
	enum LIST_COL
	{
		COL_CHNN,
		COL_PRE_SNAP_TIME,
		COL_POST_SNAP_TIME,
	};
	CCharCodeTable* m_codeTable; 
};


#endif
//end