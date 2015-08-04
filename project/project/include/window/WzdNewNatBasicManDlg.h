/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : zxx
** Date         : 2013-11-26
** Name         : WzdNatBasicManDlg.h
** Version      : 1.0
** Description  : ��ҳ��Ի���
** Modify Record:
***********************************************************************/

//����Ϊ����ʱ�������ʱ����

#ifndef _WZD_NEW_NAT_BASICMAN_DLG_H_
#define _WZD_NEW_NAT_BASICMAN_DLG_H_

#include "Dialog.h"
#include "WzdNatPageDlg.h"
#include "WzdNewNatTimeDlg.h"
#include "WzdNatDSTDlg.h"
#include "WzdNatLanguageSetDlg.h"
#include "TabCtrl.h"
#include "CheckCtrl.h"
using namespace GUI;

class CWzdNewNatBasicManDlg : public CWzdNatPageDlg
{
public:
	CWzdNewNatBasicManDlg();
	~CWzdNewNatBasicManDlg();
	unsigned long CheckValue();
	//��������Ϣ
	void SetWzdInfo(NAT_WIZARD_INFO* pWzdInfo);
	void InitCtrlInfo();
	//��ʾ������
	void ShowWzdData(bool bRefresh);

	//ҳ����ɶ�ȡ����Ϣ
	virtual void GetWzdData(bool bNext = true);

	void OnLeavePage();

	bool GetCurrZoneTimeInfo(ZONE_TIME_INFO& zoneTimeInfo, int zone);
public:
	enum WZDPAGE_ITEM
	{
		PAGE_LANGUAGE,
		PAGE_TIME,
		PAGE_DST
	};

protected:
	void OnInitial();
	void OnDestroy();
protected:
	DECLARE_MSG_MAP()
	unsigned long  OnCheck();
protected:
	GUI::CTabCtrl			m_tabCtrl;
	GUI::CCheckCtrl			m_checkCtrl;				//�´��Ƿ���ʾ��
	CWzdNewNatTimeDlg			m_wzdNatTimeDlg;
	CWzdNatDSTDlg		    m_wzdNatDSTDlg;
	CWzdNatLanguageSetDlg	m_wzdNatLanguageSetDlg;

	WZDPAGE_ITEM		m_curPage;
	NAT_WIZARD_INFO  *  m_pWzdInfo;
};

#endif

