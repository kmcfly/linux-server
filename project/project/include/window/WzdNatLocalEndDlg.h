/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : ZXX
** Date         : 2013-12-18
** Name         : WzdNatLocalEndDlg.h
** Version      : 1.0
** Description  : �򵼹���Ի���
** Modify Record:
***********************************************************************/


#ifndef _WZD_NAT_LOCALEND_DLG_H_
#define _WZD_NAT_LOCALEND_DLG_H_
#include "Dialog.h"
#include "WzdNatPageDlg.h"
#include "RichTextCtrl.h"
#include "CheckCtrl.h"

using namespace GUI;

class CWzdNatLocalEndDlg: public CWzdNatPageDlg
{
public:
	CWzdNatLocalEndDlg();
	~CWzdNatLocalEndDlg();

	//��������Ϣ
	void SetWzdInfo(NAT_WIZARD_INFO* pWzdInfo);
	//��ʾ������
	void ShowWzdData(bool bRefresh);
	void GetWzdData(bool bNext);
protected:
	void OnInitial();
	void OnDestroy();
private:
	void SetTipInfo();
	void GetStaticIp(unsigned long ip,char *pIP);//��unsigned long ip��װΪ�ַ�����
protected:
	DECLARE_MSG_MAP()
	unsigned long OnCheck();
protected:
	GUI::CRichTextCtrl   m_richTextCtrl;
	GUI::CCheckCtrl		 m_checkCtrl;				//�´��Ƿ���ʾ��

	NAT_WIZARD_INFO		*m_pWzdInfo;
};


#endif