/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : L
** Date         : 2010-02-26
** Name         : WzdStepDlg.h
** Version      : 1.0
** Description  : 向导页面对话框
** Modify Record:
***********************************************************************/

#ifndef _WZD_STEP_DLG_H_
#define _WZD_STEP_DLG_H_

#include "Dialog.h"
#include "WzdPageDlg.h"
#include "ListCtrl.h"
using namespace GUI;

//////////////////////////////////////////////////////////////////////////
// * 向导 第一页面
//////////////////////////////////////////////////////////////////////////
class CWzdBasicSetDlg : public CWzdPageDlg
{
public:
	CWzdBasicSetDlg();
	~CWzdBasicSetDlg();

	//显示向导数据
	virtual void ShowWzdData(bool bRefresh);

	//页面完成读取向导信息
	virtual void GetWzdData(bool bNext = true);

	virtual void OnLeavePage();

	bool GetCurrZoneTimeInfo(ZONE_TIME_INFO& zoneTimeInfo, int zone);

protected:
	void OnInitial();
	void OnIdle();
	void InitCtrlInfo();
	
protected:
	bool IsModify();
	void SetModify(bool modify, bool bRefresh);
	bool CanModify();

	void RefreshDateTime(LONGLONG gmt, int zone, bool bRefresh);
	void GetDateTime(LONGLONG& gmt, int zone);

protected:
	DECLARE_MSG_MAP()
	unsigned long OnClickList(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);
	
protected:
	GUI::CListCtrl		m_listCtrl;

	bool				m_bModifyTime;		//是否修改时间
	bool				m_bStopRefreshTime;	//修改时间时候用来停止刷新
	
	LONGLONG			m_pre_sys_tm;		//上一次的系统时间[带时区]
	LONGLONG			m_gmt_user_tm;		//用户修改的GMT0时区时间

	LONGLONG			m_refresh_count_tm;	//累计刷新时间

	enum LIST_ROW
	{
		ROW_DEVICE_NAME,
		ROW_LANGUAGE,
		ROW_DATE_FORMAT,
		ROW_TIME_FORMAT,
		ROW_TIME_ZONE,
		ROW_SYSTEM_DATE,
		ROW_SYSTEM_TIME,
		ROW_SHOW_WZD,
	};
};

#endif
