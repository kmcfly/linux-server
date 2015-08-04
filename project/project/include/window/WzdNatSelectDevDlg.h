/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : ZXX
** Date         : 2013-11-26
** Name         : WzdNatSelectDevDlg.h
** Version      : 1.0
** Description  : 穿透向导页面开始对话框
** Modify Record:
***********************************************************************/

#ifndef _WZD_NAT_SELECTDEV_DLG_H_
#define _WZD_NAT_SELECTDEV_DLG_H_
#include "Dialog.h"
#include "WzdNatPageDlg.h"
#include "RichTextCtrl.h"
#include "RadioCtrl.h"
#include "CheckCtrl.h"
#include "ScrollCtrl.h"
using namespace GUI;

//////////////////////////////////////////////////////////////////////////
// * 向导 设备选择页面
//////////////////////////////////////////////////////////////////////////

class CWzdNatSelectDevDlg : public CWzdNatPageDlg
{
public:
	CWzdNatSelectDevDlg();
	~CWzdNatSelectDevDlg();

	//设置向导信息
	void SetWzdInfo(NAT_WIZARD_INFO* pWzdInfo);
	void ShowWzdData(bool bRefresh);
	//页面完成读取向导信息
	void GetWzdData(bool bNext);
	void OnLeavePage();
	void SetTipInfo(const std::string& str);

	void SetDevSelectIndex(int devIndex);
	int	 GetDevSelectIndex();
protected:
	void OnInitial();
	void OnDestroy();
	virtual void InitCtrlInfo();
protected:
	DECLARE_MSG_MAP()
	unsigned long OnCheck();
	unsigned long OnClickRadioCtrl();//选择不同的选项后，下一个页面也变得有所不同
protected:
	GUI::CRichTextCtrl	m_richTextCtrl;
	GUI::CCheckCtrl		m_checkCtrl;
	NAT_WIZARD_INFO     * m_pWzdInfo;

	GUI::CRadioGroup	m_rdGroup;
	GUI::CRadioCtrl		m_rdDevAndroid;
	GUI::CRadioCtrl		m_rdDevApple;
	GUI::CRadioCtrl		m_rdDevPc;

	int					m_devSelectIndex;//设备选择项目
	GUI::CScrollCtrl	m_scroll_v;
	unsigned short		m_toprow;//滚屏显示的第一行
};

#endif


