/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : ZXX
** Date         : 2012-10-16
** Name         : WzdUS02LanguageSetDlg.h
** Version      : 1.0
** Description  : �򵼹���Ի���
** Modify Record:
***********************************************************************/

#ifndef _WZD_US02_LANGUAGE_SET_DLG_H_
#define _WZD_US02_LANGUAGE_SET_DLG_H_
#include "Dialog.h"
#include "WzdUS02PageDlg.h"
#include "Static.h"
#include "ComboCtrl.h"

using namespace GUI;


class CWzdUS02LanguageSetDlg:public CWzdUS02PageDlg
{
public:
	CWzdUS02LanguageSetDlg();
	~CWzdUS02LanguageSetDlg();

	//�ڻ�ȡ���ݺ����ÿؼ���Ϣ
	void InitCtrlInfo();
	//��������Ϣ
	void SetWzdInfo(US02_WzdInfo* pWzdInfo);
	
	//ҳ����ɶ�ȡ����Ϣ
	void GetWzdData(bool bNext = true);

protected:
	void OnInitial();
	void OnDestroy();

	//############�������Ըı�
	DECLARE_MSG_MAP()
	unsigned long OnChangeLanguage();
private:
	GUI::CComboCtrl m_selectLanguageCombo;
	GUI::CStatic	m_staticTip;

	US02_WzdInfo * m_pUS02LanguageWzdInfo;

};



#endif
