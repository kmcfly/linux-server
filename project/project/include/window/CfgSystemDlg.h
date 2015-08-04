/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : L
** Date         : 2009-08-12
** Name         : CfgSystemDlg.h
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/

#ifndef _CFG_SYSTEM_DLG_H_
#define _CFG_SYSTEM_DLG_H_

#include "ListCtrl.h"
#include "CheckCtrl.h"
#include "CfgDlg.h"
#include "MessageMan.h"

using namespace GUI;

struct status_CVBS{
	unsigned long last;
	unsigned long current;
};

class CCfgSystemDlg : public CCfgDlg
{
public:
	CCfgSystemDlg();
	~CCfgSystemDlg();

	unsigned long PreCheckValue();
	unsigned long OnOK();

protected:
	void OnInitial();

	void SetTip();
	//void OnChangeFocus(unsigned long oldID, unsigned long newID);
	
protected: // ui
	bool ShowData(NCFG_ITEM_ID cfgID, const BYTE* pData, int len, bool bRefresh);
	bool SetData(NCFG_ITEM_ID cfgID, BYTE*& pData, int& len) const;
	void DoOther();


	DECLARE_MSG_MAP()
	unsigned long OnClickList(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);

protected:
	GUI::CListCtrl	m_cfgSystemList;

	unsigned long   m_netClientNum;
	unsigned long   m_displayResolutionMask;

	typedef struct _cfg_id_pos_
	{
		NCFG_ITEM_ID	cfgItemID;
	}CFG_ID_POS;

private:
	bool ReuseCVBSOutput()const;
	bool HaveSpot()const;
	bool RealCVBSOutput() const;
	void SetCfgIDPOs(unsigned long listPos, NCFG_ITEM_ID cfgItemID);
	unsigned long GetCfgIDPos(const NCFG_ITEM_ID cfgItemID)const;
	bool isSupportCVBSVideoOut() const;//是否支持CVBS输出
private:
	CFG_ID_POS	m_cfgIdPos[32];
	unsigned long m_maxPosNum;
};


#endif
