/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : ZXX
** Date         : 2012-09-19
** Name         : WzdUS02GetNetWorkSetUpDlg.h
** Version      : 1.0
** Description  : 向导管理对话框
** Modify Record:
***********************************************************************/

#ifndef _WZD_US02_GETNETWORKSETUP_DLG_H_
#define _WZD_US02_GETNETWORKSETUP_DLG_H_
#include "Dialog.h"
#include "WzdUS02PageDlg.h"
#include "ListCtrl.h"
#include "Static.h"
#include "RichTextCtrl.h"
#include "CheckCtrl.h"
class CWzdUS02GetNetWorkSetUpDlg : public CWzdUS02PageDlg
{
public:
	CWzdUS02GetNetWorkSetUpDlg();
	~CWzdUS02GetNetWorkSetUpDlg();

	//设置向导信息
	void SetWzdInfo(US02_WzdInfo* pWzdInfo);
	//在获取数据后设置控件信息
	void InitCtrlInfo();
	//显示向导数据
	void ShowWzdData(bool bRefresh);

	//页面完成读取向导信息
	void GetWzdData(bool bNext = true);
protected:
	void OnInitial();
	void OnDestroy();
private:
	bool GetIp();
	void GetStaticIp(unsigned long ip,char *pIP);//把unsigned long ip组装为字符串型
protected:
	DECLARE_MSG_MAP()
	unsigned long OnCheck();
protected:
	GUI::CListCtrl		 m_netList;
	GUI::CRichTextCtrl   m_netNoteRichText;
	GUI::CCheckCtrl		 m_checkCtrl;				//下次是否显示向导
	
private:
	US02_WzdInfo		*m_pNetWzdInfo;
	enum LIST_ROW
	{
		ROW_NET_STATUS,
		ROW_GETIP_STATUS,
		ROW_DVRIP,
	};

};




#endif

