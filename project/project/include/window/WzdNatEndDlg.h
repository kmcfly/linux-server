/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : ZXX
** Date         : 2013-11-27
** Name         : WzdNatEndDlg.h
** Version      : 1.0
** Description  : ��͸��ҳ������Ի���
** Modify Record:
***********************************************************************/

#ifndef _WZD_NAT_END_DLG_H_
#define _WZD_NAT_END_DLG_H_
#include "Dialog.h"
#include "WzdNatPageDlg.h"
#include "ScrollCtrl.h"
#include "RichTextCtrl.h"
#include "CheckCtrl.h"
using namespace GUI;

//////////////////////////////////////////////////////////////////////////
// * �� ����ҳ��
//////////////////////////////////////////////////////////////////////////

class CWzdNatEndDlg : public CWzdNatPageDlg
{
public:
	CWzdNatEndDlg();
	~CWzdNatEndDlg();

	//��������Ϣ
	void SetWzdInfo(NAT_WIZARD_INFO* pWzdInfo);
	void ShowWzdData(bool bRefresh);
	//ҳ����ɶ�ȡ����Ϣ
	void GetWzdData(bool bNext);
	void OnLeavePage();
	void SetTipInfo(const std::string& str);
protected:
	void OnInitial();
	void OnDestroy();
	virtual void InitCtrlInfo();
	void OnPaint(PAINT_STATUS status);
protected:
	DECLARE_MSG_MAP()
	unsigned long OnCheck();
	unsigned long OnScrollchange(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);
private:
	void DrawTriangle(int x,int y);//��������
protected:
	GUI::CRichTextCtrl	m_richTextCtrl;
	GUI::CCheckCtrl		m_checkCtrl;
	GUI::CScrollCtrl	m_scroll_v;
	NAT_WIZARD_INFO     *m_pWzdInfo;

	int					m_drawX;//�Ի�ͼ��ʼX����
	int					m_drawY;//�Ի�ͼ��ʼY����
	unsigned short		m_toprow;//������ʾ�ĵ�һ��

};

#endif
