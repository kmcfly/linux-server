/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : zxx
** Date         : 2010-07-21
** Name         : CWzdDiskManDlg.h
** Version      : 1.0
** Description  : 
** Modify Record:
作用:磁盘管理的向导
***********************************************************************/
#ifndef _WzdDiskMan_H_
#define _WzdDiskMan_H_

#include "Dialog.h"
#include "WndStructDef.h"
#include "WzdPageDlg.h"
#include "ListCtrl.h"
#include "Static.h"
#include "Button.h"
#include "ProgressDlg.h"
#include "CheckCtrl.h"

using namespace GUI;

class CWzdDiskManDlg:public CWzdPageDlg
{
public:
	CWzdDiskManDlg();
	~CWzdDiskManDlg();

	//此页面是否使用
	void SetUsed(bool bUsed = true);
	bool IsUsed();

	//在获取数据后设置控件信息
	virtual void InitCtrlInfo();

	//设置向导信息
	virtual void SetWzdInfo(WIZARD_INFO* pWzdInfo);

	//显示向导数据
	virtual void ShowWzdData(bool bRefresh);

	//页面完成读取向导信息
	virtual void GetWzdData(bool bNext = true);
	void SetPercent( int percet );
	void EndProgress();
	void RefreshDiskInfo(bool bRefresh = true);

protected:
	virtual void OnInitial();
	void OnIdle();

	DECLARE_MSG_MAP()
	unsigned long OnFormat();
	unsigned long OnRefresh();
	unsigned long OnClickList(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);

protected:
	void EnableFormatBtn(bool bEnable, bool bRefresh);
	void EnableRefreshBtn(bool bEnable, bool bRefresh);
	void EnableDefaultBtn(bool bEnable, bool bRefresh);//禁止开放上一步，下一步和退出按键

protected:
	WIZARD_INFO*		m_pWzdInfo;
	bool				m_bUsed;

	int					m_finishDiskNum;  //已完成格式化的个数

private:
	GUI::CListCtrl		m_diskManList;
	CProgressDlg		m_progressDlg;
	GUI::CStatic		m_stcMsg;
	GUI::CButton		m_bntRefresh;
	GUI::CButton		m_bntFormat;
	
	
	bool				m_prgShow;
	DVR_DISK_INFO_LIST	m_diskInfoList;
	POS*				m_listRowToDiskInfo;
	int					m_curSelectRow;

	bool				m_bFormating;
	int					m_diplayPrecent;
};

#endif

//end
