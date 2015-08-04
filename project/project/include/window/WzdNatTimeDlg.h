/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : ZXX
** Date         : 2013-11-26
** Name         : WzdNatTimeDlg.h
** Version      : 1.0
** Description  : 向导页面对话框
** Modify Record:
***********************************************************************/


#ifndef _WZD_NAT_Time_DLG_H_
#define _WZD_NAT_Time_DLG_H_
#include "Dialog.h"
#include "WzdNatPageDlg.h"
#include "ListCtrl.h"

using namespace GUI;

class CWzdNatTimeDlg:public CWzdNatPageDlg
{
public:
	CWzdNatTimeDlg();
	~CWzdNatTimeDlg();

	//在获取数据后设置控件信息
	void InitCtrlInfo();
	//设置向导信息
	void SetWzdInfo(NAT_WIZARD_INFO* pWzdInfo);
	//显示向导数据
	void ShowWzdData(bool bRefresh);

	//页面完成读取向导信息
	void GetWzdData(bool bNext = true);

	void OnLeavePage();

	bool GetCurrZoneTimeInfo(ZONE_TIME_INFO& zoneTimeInfo, int zone);
protected:
	void OnInitial();
	void OnIdle();

	bool IsModify();
	void SetModify(bool modify, bool bRefresh);
	bool CanModify();

	void RefreshDateTime(LONGLONG gmt, int zone, bool bRefresh);
	void GetDateTime(LONGLONG& gmt, int zone);

	unsigned long OnPreCommand(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);
protected:
	DECLARE_MSG_MAP()
	unsigned long OnClickList(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);
protected:
	GUI::CListCtrl		m_listTimeCtrl;
	enum LIST_ROW
	{
		ROW_SYSTEM_TIME,
		ROW_DATE_FORMAT,
		ROW_TIME_FORMAT,
		ROW_TIME_ZONE,
		ROW_SYSTEM_DATE,
		ROW_TIME_SYNC,
		ROW_TIME_SERVER,
		ROW_TIME_UPDATENOW,
	};

	NAT_WIZARD_INFO * m_pNatTimeWzdInfo;

	bool				m_bModifyTime;		//是否修改时间
	bool				m_bStopRefreshTime;	//修改时间时候用来停止刷新

	LONGLONG			m_pre_sys_tm;		//上一次的系统时间[带时区]
	LONGLONG			m_gmt_user_tm;		//用户修改的GMT0时区时间

	LONGLONG			m_refresh_count_tm;	//累计刷新时间

protected:
	void SetEnableUI(  LIST_ROW rowNum, bool brefalsh );
private:
	bool				m_bUpdateTimeNow;						//为true时要即时NTP同步，这时候屏蔽其他消息
};


#endif

