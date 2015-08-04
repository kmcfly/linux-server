/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : L
** Date         : 2009-01-20
** Name         : CfgTriggerBasicDlg.h 
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/
#ifndef _CFG_TRIGGER_BASIC_DLG_H_
#define _CFG_TRIGGER_BASIC_DLG_H_

#include "Dialog.h"
#include "Static.h"
#include "CheckCtrl.h"
#include "ListCtrl.h"
#include "CfgDlg.h"
#include "WndStructDef.h"
#include "ScrollDialog.h"

using namespace GUI;

class CCfgTriggerBasicDlg : public GUI::CScrollDialog
{
public:
	CCfgTriggerBasicDlg();
	~CCfgTriggerBasicDlg();

	void SetAlarmTriggerInfo(ALARM_TRIGGER_INFO* pTrigger, unsigned char chnn) { m_pTriggerInfo = pTrigger; m_channel = chnn;}

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

protected:
	enum LIST_ROW
	{
		ROW_BUZZER = 0,
		ROW_FULL_SCREEN = 1,
		ROW_TO_ALARM_OUT = 2,
		ROW_EMAIL_EN = 3,
	};

	enum SW_ITEM_ID
	{
		SW_ITEM_ID_BUZZER		= 1,
		SW_ITEM_ID_FULL_SCREEN	= 2,
		SW_ITEM_ID_EMAIL_EN		= 3,

		SW_ITEM_ID_ALARM_OUT_0	= 100,

		SW_ITEM_ID_SNAP_CH_GROUP= 200,
		SW_ITEM_ID_SNAP_CH_0	= 220,
	};

protected:
	ALARM_TRIGGER_INFO* m_pTriggerInfo;
	std::vector<GUI::CCheckCtrl*>	m_vecChnn;	//存储通道指针
	std::vector<GUI::CCheckCtrl*>	m_vecChnnAll;//存储All单选指针

	unsigned int	m_maxChnn;
	unsigned char	m_channel;
};


#endif
