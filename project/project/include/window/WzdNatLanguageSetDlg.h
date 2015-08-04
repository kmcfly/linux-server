/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : ZXX
** Date         : 2013-11-26
** Name         : WzdNatLanguageSetDlg.h
** Version      : 1.0
** Description  : �򵼹���Ի���
** Modify Record:
***********************************************************************/

#ifndef _WZD_NAT_LANGUAGE_SET_DLG_H_
#define _WZD_NAT_LANGUAGE_SET_DLG_H_
#include "Dialog.h"
#include "WzdNatPageDlg.h"
#include "Static.h"
#include "ComboCtrl.h"
#include "RichTextCtrl.h"
#include <string>
using namespace GUI;


class CWzdNatLanguageSetDlg:public CWzdNatPageDlg
{
public:
	CWzdNatLanguageSetDlg();
	~CWzdNatLanguageSetDlg();

	//�ڻ�ȡ���ݺ����ÿؼ���Ϣ
	void InitCtrlInfo();
	//��������Ϣ
	void SetWzdInfo(NAT_WIZARD_INFO* pWzdInfo);

	//ҳ����ɶ�ȡ����Ϣ
	void GetWzdData(bool bNext = true);

protected:
	void OnInitial();
	void OnDestroy();

	//############�������Ըı�
	DECLARE_MSG_MAP()
	unsigned long  OnChangeLanguage();

private:
	GUI::CComboCtrl m_selectLanguageCombo;
	GUI::CStatic	m_staticTip;
	GUI::CRichTextCtrl	m_richTextCtrl;

	NAT_WIZARD_INFO * m_pNatLanguageWzdInfo;

};


#endif
