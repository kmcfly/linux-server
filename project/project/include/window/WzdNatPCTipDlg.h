/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : ZXX
** Date         : 2013-12-20
** Name         : WzdNatPCTipDlg.h
** Version      : 1.0
** Description  : ��͸��ҳ���ά��Ի���
** Modify Record:
***********************************************************************/

#ifndef _WZD_NAT_PCTIP_DLG_H_
#define _WZD_NAT_PCTIP_DLG_H_
#include "Dialog.h"
#include "WzdNatPageDlg.h"
#include "RichTextCtrl.h"
#include "CheckCtrl.h"
#include "ScrollCtrl.h"
using namespace GUI;

//////////////////////////////////////////////////////////////////////////
// * �� ��ά��ҳ��
//////////////////////////////////////////////////////////////////////////
class CWzdNatPCTipDlg : public CWzdNatPageDlg
{
public:
	CWzdNatPCTipDlg();
	~CWzdNatPCTipDlg();

	//��������Ϣ
	virtual void SetWzdInfo(NAT_WIZARD_INFO* pWzdInfo);
	virtual void ShowWzdData(bool bRefresh);
	virtual void GetWzdData(bool bNext);
	virtual void OnLeavePage();	
protected:
	void OnInitial();
	virtual void InitCtrlInfo();
	void OnDestroy();
protected:
	DECLARE_MSG_MAP()
	unsigned long OnCheck();
	unsigned long OnScrollchange(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);
protected:
	GUI::CRichTextCtrl	m_richTextCtrl;
	GUI::CCheckCtrl		m_checkCtrl;
	NAT_WIZARD_INFO     * m_pWzdInfo;
	unsigned short		m_toprow;//������ʾ�ĵ�һ��
	GUI::CScrollCtrl	m_scroll_v;
};

#endif

