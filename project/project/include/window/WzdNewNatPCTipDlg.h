/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : ZXX
** Date         : 2013-12-20
** Name         : WzdNatPCTipDlg.h
** Version      : 1.0
** Description  : 穿透向导页面二维码对话框
** Modify Record:
***********************************************************************/

#ifndef _WZD_NEW_NAT_PCTIP_DLG_H_
#define _WZD_NEW_NAT_PCTIP_DLG_H_
#include "Dialog.h"
#include "WzdNatPageDlg.h"
#include "RichTextCtrl.h"
#include "CheckCtrl.h"
#include "BinaryRadioCtrl.h"
#include "ScrollCtrl.h"
using namespace GUI;

//////////////////////////////////////////////////////////////////////////
// * 向导 二维码页面
//////////////////////////////////////////////////////////////////////////
class CWzdNewNatPCTipDlg : public CWzdNatPageDlg
{
public:
	CWzdNewNatPCTipDlg();
	~CWzdNewNatPCTipDlg();

	//设置向导信息
	virtual void SetWzdInfo(NAT_WIZARD_INFO* pWzdInfo);
	virtual void ShowWzdData(bool bRefresh);
	virtual void GetWzdData(bool bNext);
	virtual void OnLeavePage();	

	bool					m_bSelect;//设备选择项目
protected:
	void OnInitial();
	virtual void InitCtrlInfo();
	void OnDestroy();
protected:
	DECLARE_MSG_MAP()
	unsigned long OnCheck();
	unsigned long OnScrollchange(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);
	unsigned long OnClickRadioCtrl();//选择不同的选项后，下一个页面也变得有所不同
protected:
	GUI::CRichTextCtrl	m_richTextCtrl;
	GUI::CCheckCtrl		m_checkCtrl;
	NAT_WIZARD_INFO     * m_pWzdInfo;
	unsigned short		m_toprow;//滚屏显示的第一行
	GUI::CScrollCtrl	m_scroll_v;

	GUI::CBinaryRadioCtrl		m_rdDevMobile;


	
};

#endif

