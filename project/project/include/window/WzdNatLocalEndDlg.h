/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : ZXX
** Date         : 2013-12-18
** Name         : WzdNatLocalEndDlg.h
** Version      : 1.0
** Description  : 向导管理对话框
** Modify Record:
***********************************************************************/


#ifndef _WZD_NAT_LOCALEND_DLG_H_
#define _WZD_NAT_LOCALEND_DLG_H_
#include "Dialog.h"
#include "WzdNatPageDlg.h"
#include "RichTextCtrl.h"
#include "CheckCtrl.h"

using namespace GUI;

class CWzdNatLocalEndDlg: public CWzdNatPageDlg
{
public:
	CWzdNatLocalEndDlg();
	~CWzdNatLocalEndDlg();

	//设置向导信息
	void SetWzdInfo(NAT_WIZARD_INFO* pWzdInfo);
	//显示向导数据
	void ShowWzdData(bool bRefresh);
	void GetWzdData(bool bNext);
protected:
	void OnInitial();
	void OnDestroy();
private:
	void SetTipInfo();
	void GetStaticIp(unsigned long ip,char *pIP);//把unsigned long ip组装为字符串型
protected:
	DECLARE_MSG_MAP()
	unsigned long OnCheck();
protected:
	GUI::CRichTextCtrl   m_richTextCtrl;
	GUI::CCheckCtrl		 m_checkCtrl;				//下次是否显示向导

	NAT_WIZARD_INFO		*m_pWzdInfo;
};


#endif