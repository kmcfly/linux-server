/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : ZXX
** Date         : 2012-09-24
** Name         : WzdUS02GetPortDlg.h
** Version      : 1.0
** Description  : �򵼹���Ի���
** Modify Record:
***********************************************************************/

#ifndef _WZD_US02_GETPORT_DLG_H_
#define _WZD_US02_GETPORT_DLG_H_
#include "Dialog.h"
#include "WzdUS02PageDlg.h"
#include "Static.h"
#include "CheckCtrl.h"
using namespace GUI;
class CWzdUS02GetPortDlg : public CWzdUS02PageDlg
{
public:
	CWzdUS02GetPortDlg();
	~CWzdUS02GetPortDlg();
	void SetWzdInfo(US02_WzdInfo * pWzdInfo);
	//�ڻ�ȡ���ݺ����ÿؼ���Ϣ
	void InitCtrlInfo();
	void ShowWzdData(bool bRefresh);
protected:
	void OnInitial();
	void OnDestroy();
protected:
	DECLARE_MSG_MAP()
	unsigned long OnCheck();
protected:
	US02_WzdInfo * m_pWzdInfo;
	GUI::CStatic m_showStatic;
	GUI::CCheckCtrl m_checkCtrl;
};


#endif
