/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : L
** Date         : 2010-06-30
** Name         : OtherAlarmDlg.h 
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/

#ifndef _OTHER_ALARM_DLG_H_
#define _OTHER_ALARM_DLG_H_

#include "Wnd.h"
#include "ComboCtrl.h"
#include "CheckCtrl.h"
#include "ScrollDialog.h"
#include "WndStructDef.h"

using namespace GUI;

class COtherAlarmDlg : public GUI::CScrollDialog
{
public:
	COtherAlarmDlg();
	virtual ~COtherAlarmDlg();

	void SetOtherAlarmInfo(OTHER_ALARM_INFO* pInfo);

	void SetMaxAlarmOut(int max);

	void InitCtrlData(bool bRefresh); //初始化控件内容

protected:
	void OnInitial();
	void OnPaint(PAINT_STATUS status);

protected:
	DECLARE_MSG_MAP()
	unsigned long OnClickScrollDlg(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);

protected:
	OTHER_ALARM_INFO* m_pOtherAlarmInfo;
	int m_maxAlarmOut;

protected:
	enum SW_ITEM_ID
	{
		SW_ITEM_ID_TYPE				= 1,
		SW_ITEM_ID_DISK_FREE		= 2,
		SW_ITEM_ID_DISK_FREE_SATIC  = 3,

		SW_ITEM_ID_BUZZER			= 4,
		SW_ITEM_ID_EMAIL_EN			= 5,
		SW_ITEM_ID_EMAIL_EN_STATIC  = 6,

		SW_ITEM_ID_ALARM_OUT_0		= 100,
	};

	enum
	{
		ITEM_TYPE_DISK_FULL		 = 0,
		ITEM_TYPE_IP_CONFLICT	 = 1,
		ITEM_TYPE_NET_DISCONNECT = 2,
		ITEM_TYPE_DISK_WARNING   = 3,
		ITEM_TYPE_DISK_DISCONNECT=4,
	};
};

#endif

//end
