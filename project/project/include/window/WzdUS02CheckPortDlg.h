/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : ZXX
** Date         : 2012-09-19
** Name         : WzdUS02CheckPortDlg.h
** Version      : 1.0
** Description  : �򵼹���Ի���
** Modify Record:
***********************************************************************/

#ifndef _WZD_US02_CHECKPORT_DLG_H_
#define _WZD_US02_CHECKPORT_DLG_H_
#include "Dialog.h"
#include "WzdUS02PageDlg.h"
#include "Static.h"
using namespace GUI;
class CWzdUS02CheckPortDlg : public CWzdUS02PageDlg
{
public:
	CWzdUS02CheckPortDlg();
	~CWzdUS02CheckPortDlg();
	//��������Ϣ
	void SetWzdInfo(US02_WzdInfo* pWzdInfo);
	//��ʾ������
	void ShowWzdData(bool bRefresh);
protected:
	void OnInitial();
	void OnDestroy();
protected:
	GUI::CStatic m_static;
	US02_WzdInfo *m_pWzdInfo;
};

#endif
