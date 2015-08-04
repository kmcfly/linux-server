/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : ZXX
** Date         : 2013-11-26
** Name         : WzdNatSelectDevDlg.h
** Version      : 1.0
** Description  : ��͸��ҳ�濪ʼ�Ի���
** Modify Record:
***********************************************************************/

#ifndef _WZD_NAT_SELECTDEV_DLG_H_
#define _WZD_NAT_SELECTDEV_DLG_H_
#include "Dialog.h"
#include "WzdNatPageDlg.h"
#include "RichTextCtrl.h"
#include "RadioCtrl.h"
#include "CheckCtrl.h"
#include "ScrollCtrl.h"
using namespace GUI;

//////////////////////////////////////////////////////////////////////////
// * �� �豸ѡ��ҳ��
//////////////////////////////////////////////////////////////////////////

class CWzdNatSelectDevDlg : public CWzdNatPageDlg
{
public:
	CWzdNatSelectDevDlg();
	~CWzdNatSelectDevDlg();

	//��������Ϣ
	void SetWzdInfo(NAT_WIZARD_INFO* pWzdInfo);
	void ShowWzdData(bool bRefresh);
	//ҳ����ɶ�ȡ����Ϣ
	void GetWzdData(bool bNext);
	void OnLeavePage();
	void SetTipInfo(const std::string& str);

	void SetDevSelectIndex(int devIndex);
	int	 GetDevSelectIndex();
protected:
	void OnInitial();
	void OnDestroy();
	virtual void InitCtrlInfo();
protected:
	DECLARE_MSG_MAP()
	unsigned long OnCheck();
	unsigned long OnClickRadioCtrl();//ѡ��ͬ��ѡ�����һ��ҳ��Ҳ���������ͬ
protected:
	GUI::CRichTextCtrl	m_richTextCtrl;
	GUI::CCheckCtrl		m_checkCtrl;
	NAT_WIZARD_INFO     * m_pWzdInfo;

	GUI::CRadioGroup	m_rdGroup;
	GUI::CRadioCtrl		m_rdDevAndroid;
	GUI::CRadioCtrl		m_rdDevApple;
	GUI::CRadioCtrl		m_rdDevPc;

	int					m_devSelectIndex;//�豸ѡ����Ŀ
	GUI::CScrollCtrl	m_scroll_v;
	unsigned short		m_toprow;//������ʾ�ĵ�һ��
};

#endif


