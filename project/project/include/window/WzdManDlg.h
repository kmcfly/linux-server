/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : L
** Date         : 2010-02-26
** Name         : WzdManDlg.h
** Version      : 1.0
** Description  : �򵼹���Ի���
** Modify Record:
***********************************************************************/

#ifndef _WZD_MAN_DLG_H_
#define _WZD_MAN_DLG_H_

#include "Static.h"
#include "CheckCtrl.h"
#include "Dialog.h"
#include "WndStructDef.h"
#include "WzdPageDlg.h"
#include "CfgDlg.h"
#include "WzdTipNetDlg.h"
#include "WzdRecordStreamDlg.h"
#include "WzdDiskManDlg.h"
#include <vector>

using namespace GUI;

class CWzdManDlg : public CCfgDlg
{
public:
	CWzdManDlg();
	~CWzdManDlg();

	void SetDiskStatus(unsigned long status);

	//��������,ĳЩ���������ڽ�����֮����ܽ���.
	bool TryEnterConfig();

protected:
	void OnInitial();
	void OnPostInitial();
	void OnDestroy();
	void OnPaint(PAINT_STATUS status);
	void EndDialog(DLG_END_VALUE result);
	
	unsigned long OnOK(); //��һҳ
	unsigned long OnCancel(); //��һҳ
	unsigned long OnApply(); //��һҳ
	unsigned long SaveWizard();//���沢�˳�

protected:
	//���ò������
	virtual void GetItemList(CFGITEM_LIST& itemList) const;
	virtual bool SetData(NCFG_ITEM_ID cfgID, BYTE*& pData, int& len) const;

	bool EnterConfig();
	void ExitConfig() const;

protected:
	void CreateWizardInfo();	//�����򵼱���
	void InitWizardInfo();		//��ʼ���򵼱���

	void CreateWizardPage();	//��ʼ����ҳ��

	void ShowPage(int index);	//��ʾĳһ��ҳ��

	void ShowBtn(int index);	//���°�ť״̬

	void OnWzdEnd();			//�򵼽�����Ч����

	void OnIdle();				//�򵼶�ʱ�رմ���

protected:
	//����������ʾҳ��
	CWzdPageDlg* CreateTipDiskDlg(unsigned short x, unsigned short y, unsigned short cx, unsigned short cy, bool show );
	//����������ʾҳ��
	CWzdTipNetDlg * CreateTipNetDlg(unsigned short x, unsigned short y, unsigned short cx, unsigned short cy, bool show );
	//����¼����ʾҳ��
	CWzdRecordStreamDlg *CreateTipRecordDlg(unsigned short x, unsigned short y, unsigned short cx, unsigned short cy, bool show );
	//�������̹�����ʾҳ��
	CWzdDiskManDlg *CreateDiskManDlg(unsigned short x, unsigned short y, unsigned short cx, unsigned short cy, bool show );

protected:
	int m_curIndex;								//��ǰҳ�����

	WIZARD_INFO*		m_pWizardInfo;			//������

	std::vector<CWzdPageDlg*>	m_vecWzdPage;	//��ҳ��

	unsigned long m_sysDiskStatus;				//ϵͳ����״̬

	bool m_bEnterConfig;						// �Ƿ��������
	unsigned long   m_displayResolutionMask;
	LONGLONG	m_curTime;
	LONGLONG	m_lastTime;
	LONGLONG    m_maxTime;

};

#endif

