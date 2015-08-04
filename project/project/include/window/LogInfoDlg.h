/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : ysw
** Date         : 2009-08-06
** Name         : LogInfoDlg.h
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/
#ifndef _LOG_INFO_DLG_H_
#define _LOG_INFO_DLG_H_
#include "Dialog.h"
#include "ListCtrlEx.h"
#include "LogSearchConditionDlg.h"



using namespace GUI;

class CLogInfoDlg : public GUI::CDialog
{
public:
	CLogInfoDlg();
	~CLogInfoDlg();

	//////////////////////////////////////////////////////////////////////////
protected:
	DECLARE_MSG_MAP()
	void OnInitial();

	void OnPostInitial();

	unsigned long OnOK();

	unsigned long OnApply();
	void CreateFoldCtrl();	//����չ�����۵� �ķ�ʽ�����ӿؼ�
	void CreateUnfoldCtrl();//����չ�����۵� �ķ�ʽ�����ӿؼ�

	unsigned long OnListExDBClick(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);

private:
	//////////////////////////////////////////////////////////////////////////
	unsigned long OnBtnFlex();
	//////////////////////////////////////////////////////////////////////////
	bool				m_bFlexSearchConditionDlg;
	unsigned short		m_searchConditionDlgWidth;

	GUI::CButton		m_btnFlex;
	GUI::CListCtrlEx <OPERATOR_LOG> m_listCtrlEx;
	CLogSearchConditionDlg m_logSearchConditionDlg;
	int m_logNum;					//����������־����
	OPERATOR_LOG * m_pLogData;		//����������־����
};
#endif //_LOG_INFO_DLG_H_
