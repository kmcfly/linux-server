/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : L
** Date         : 2009-01-20
** Name         : CfgTriggerToRecordDlg.cpp
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/
#ifndef _CFG_TRIGGER_TO_FTP_DLG_
#define _CFG_TRIGGER_TO_FTP_DLG_

#include "Dialog.h"
#include "ListCtrl.h"
#include "CfgDlg.h"
#include "WndStructDef.h"
#include <vector>

using namespace GUI;

class CCfgTriggerToFtpDlg : public CCfgDlg
{
public:
	CCfgTriggerToFtpDlg();
	~CCfgTriggerToFtpDlg();

	void SetAlarmTriggerInfo(ALARM_TRIGGER_INFO* pTrigger) { m_pTriggerInfo = pTrigger;}

	void InitCtrlData(bool bRefresh);

protected:
	void OnInitial();
	void OnDestroy();

protected:
	void OnClickChnn(int index, bool setValue, bool bRefresh);
	void OnClickChnnAll(int index, bool setValue, bool bRefresh);

protected:
	DECLARE_MSG_MAP()
	unsigned long OnClickList(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);

protected:
	GUI::CListCtrl		m_listCtrl;
	std::vector<GUI::CCheckCtrl*>	m_vecChnn;	//存储通道指针
	std::vector<GUI::CCheckCtrl*>	m_vecChnnAll;//存储All单选指针

protected:
	ALARM_TRIGGER_INFO* m_pTriggerInfo;

};

#endif
