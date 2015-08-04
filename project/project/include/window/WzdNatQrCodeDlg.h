/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : ZXX
** Date         : 2013-11-25
** Name         : WzdNatQrCodeDlg.h
** Version      : 1.0
** Description  : ��͸��ҳ���ά��Ի���
** Modify Record:
***********************************************************************/

#ifndef _WZD_NAT_QRCODE_DLG_H_
#define _WZD_NAT_QRCODE_DLG_H_
#include "Dialog.h"
#include "WzdNatPageDlg.h"
#include "RichTextCtrl.h"
#include "CheckCtrl.h"
#include "ScrollCtrl.h"
using namespace GUI;

//////////////////////////////////////////////////////////////////////////
// * �� ��ά��ҳ��
//////////////////////////////////////////////////////////////////////////
class CWzdNatQrCodeDlg : public CWzdNatPageDlg
{
public:
	CWzdNatQrCodeDlg();
	~CWzdNatQrCodeDlg();

	//��������Ϣ
	virtual void SetWzdInfo(NAT_WIZARD_INFO* pWzdInfo);
	virtual void ShowWzdData(bool bRefresh);
	virtual void GetWzdData(bool bNext);
	virtual void OnLeavePage();	
	//���ݲ�ͬ���豸������ʾ��ͬ��ҳ��
	virtual void SetDevIndex(int index);
protected:
	void OnInitial();
	virtual void InitCtrlInfo();
	void OnPaint(PAINT_STATUS status);
	void OnDestroy();
protected:
	DECLARE_MSG_MAP()
	unsigned long OnCheck();
	unsigned long OnScrollchange(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);
private:
	void SetMaix(unsigned char * pMaix,unsigned long w,unsigned long h);
protected:
	GUI::CRichTextCtrl	m_phoneRichTextCtrl;
	GUI::CCheckCtrl		m_checkCtrl;
	NAT_WIZARD_INFO     * m_pWzdInfo;

	unsigned char * m_pMaix;
	unsigned long m_maxW;//��ά���ͼ���
	unsigned long m_maxH;//��ά���ͼ�߶�
	int			  m_drawStartX;//��ά���ͼ��ʼX����
	int			  m_drawStartY;//��ά���ͼ��ʼY����
	int			  m_devIndex;

	unsigned short		m_toprow;//������ʾ�ĵ�һ��
	GUI::CScrollCtrl	m_scroll_vPhone;
};

#endif


