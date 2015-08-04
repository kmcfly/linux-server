/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : l
** Date         : 2009-08-12
** Name         : CfgDateTimeDlg.h
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/

#ifndef _CFG_NEW_DATE_TIME_DLG_H_
#define _CFG_NEW_DATE_TIME_DLG_H_

#include "CfgDlg.h"
#include "ListCtrl.h"
#include "DateCtrl.h"
#include "TimeCtrl.h"

using namespace GUI;

extern std::string g_zoneName[];

class CCfgNewDateTimeDlg : public CCfgDlg
{
	friend class CCfgBasicManDlg;
public:
	CCfgNewDateTimeDlg();
	~CCfgNewDateTimeDlg();

protected:
	//void CheckUI(bool bRefresh);
	unsigned long RecoverChange();

	void SetModifyTime(bool modify, bool bRefresh);

	unsigned long OnPreCommand(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);

protected:
	DECLARE_MSG_MAP()
	void OnInitial();
	void SetTip();
	void OnChangeFocus(unsigned long oldID, unsigned long newID);
	unsigned long OnClickList(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);
	unsigned long OnMouseWheel();

	bool ShowData(NCFG_ITEM_ID cfgID, const BYTE* pData, int len, bool bRefresh);
	bool SetData(NCFG_ITEM_ID cfgID, BYTE*& pData, int& len)  const;
	void OnIdle();//空闲，DoModal模式下无按键

protected:
	GUI::CListCtrl	m_cfgDateTimeList;

	enum LIST_ROW
	{
		ROW_DATE_FORMAT,
		ROW_TIME_FORMAT,
		ROW_TIME_ZONE,
#if defined(__DVR_BASIC__) && !defined(__TW01_RILI__)
		ROW_TIME_SYNC,
		ROW_TIME_SERVER,
		ROW_TIME_UPDATENOW,
#endif
		ROW_SYS_DATE,
		ROW_SYS_TIME,
		ROW_MODIFY_TIME,
	};

	void SetEnableUI(  LIST_ROW rowNum, bool brefalsh );

private:
	unsigned long	m_currTime;
	bool			m_bStopRefreshTime;
	bool			m_bUpdateTimeNow;						//为true时要即时NTP同步，这时候屏蔽其他消息
};


#endif