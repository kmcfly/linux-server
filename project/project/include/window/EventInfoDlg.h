/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : ysw
** Date         : 2009-08-06
** Name         : EventInfoDlg.h
** Version      : 1.0
** Description  : 
** Modify Record: 2009-09-01
1:
***********************************************************************/
#ifndef _EVENT_INFO_DLG_H_
#define _EVENT_INFO_DLG_H_
#include "Button.h"
#include "CheckCtrl.h"
#include "Dialog.h"
#include "ListCtrlEx.h"
#include "SearchCdn2TimeDlg.h"
#include "SearchCdnTimeDlg.h"

using namespace GUI;

class CEventInfoDlg : public GUI::CDialog
{
public:
	CEventInfoDlg();
	~CEventInfoDlg();
protected:
	DECLARE_MSG_MAP()
	void OnInitial();

	void OnPostInitial();

	void CreateFoldCtrl();	//����չ�����۵� �ķ�ʽ�����ӿؼ�
	void CreateUnfoldCtrl();//����չ�����۵� �ķ�ʽ�����ӿؼ�

private:
	//////////////////////////////////////////////////////////////////////////
	unsigned long OnOK();
	unsigned long OnApply();
	//////////////////////////////////////////////////////////////////////////
	unsigned long OnBtnFlex();
	//////////////////////////////////////////////////////////////////////////
	unsigned long OnCheckSensor();
	bool				m_bFlexSearchConditionDlg;
	unsigned short		m_searchConditionDlgWidth;

	GUI::CButton		m_btnFlex;

	GUI::CCheckCtrl		m_ckMotion;
	GUI::CCheckCtrl		m_ckSensor;
	GUI::CCheckCtrl		m_ckVideoLoss;

	GUI::CListCtrlEx <EVENT_LOG> m_listCtrlEx;

	CSearchCdn2TimeDlg	m_searchCdn2TimeDlg;
	CSearchCdnTimeDlg	m_searchCdnTimeDlg;

	EVENT_LOG			*m_pEventLog;			//���������¼�����
	int					m_EventLogNum;			//���������¼�����

	long				m_channelNum;
};
#endif //_EVENT_INFO_DLG_H_
