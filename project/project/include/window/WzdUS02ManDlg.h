/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : ZXX
** Date         : 2012-09-17
** Name         : WzdUS02ManDlg.h
** Version      : 1.0
** Description  : �򵼹���Ի���
** Modify Record:
***********************************************************************/

#ifndef _WZD_US02_MAN_DLG_H_
#define _WZD_US02_MAN_DLG_H_
#include "Static.h"
#include "CheckCtrl.h"
#include "Dialog.h"
#include "CfgDlg.h"
#include "WzdUS02PageDlg.h"
#include <vector>
#include <string>
using namespace GUI;

class CWzdUS02ManDlg : public CCfgDlg
{
public:
	CWzdUS02ManDlg();
	~CWzdUS02ManDlg();

	//��������,ĳЩ���������ڽ�����֮����ܽ���.
	bool TryEnterConfig();
protected:
	void OnInitial();
	void OnPostInitial();
	void OnDestroy();
	void OnPaint(PAINT_STATUS status);
	void EndDialog(DLG_END_VALUE result);

	unsigned long OnOK(); //��һҳ
	unsigned long OnCancel(); //�˳�
	unsigned long OnApply(); //��һҳ
	unsigned long SaveUS02Wizard();//���沢�˳�
	void OnIdle();				//�򵼶�ʱ�رմ���
protected:
	unsigned long CheckValue();
	//���ò������
	virtual void GetItemList(CFGITEM_LIST& itemList) const;
	virtual bool SetData(NCFG_ITEM_ID cfgID, BYTE*& pData, int& len) const;

	bool EnterConfig();
	void ExitConfig() const;

protected:
	void CreateUS02WizardPage();	//��ʼ����ҳ��
	void CreateWizardInfo();		//�����򵼱���
	void InitWizardInfo();			//��ʼ���򵼱���


	void ShowPage(int index);		//��ʾĳһ��ҳ��
	void ShowBtn(int index);		//���°�ť״̬
	void OnWzdEnd();			//�򵼽�����Ч����

protected:
	//������һҳ���򵼻�ӭҳ��
	CWzdUS02PageDlg* CreateUS02TipStartDlg(unsigned short x, unsigned short y, unsigned short cx, unsigned short cy, bool show,int pageIndex);
	//������tab�ؼ����򵼵�2ҳ�棬���������ԡ�ʱ���dst����
	CWzdUS02PageDlg* CreateUS02TabBasicManDlg(unsigned short x, unsigned short y, unsigned short cx, unsigned short cy, bool show,int pageIndex);
	//������������ǰ����ʾҳ��
	CWzdUS02PageDlg* CreateUS02ConntctTipDlg(unsigned short x, unsigned short y, unsigned short cx, unsigned short cy, bool show,int pageIndex);
	//��ȡIP���������õ�ҳ��
	CWzdUS02PageDlg* CreateUS02GetNetWorkSetUpDlg(unsigned short x, unsigned short y, unsigned short cx, unsigned short cy, bool show,int pageIndex);
	//�������upnp���ܵ�ҳ��
	CWzdUS02PageDlg *CreateUS02CheckUpnpDlg(unsigned short x, unsigned short y, unsigned short cx, unsigned short cy, bool show,int pageIndex);
	//�����򿪶˿�ҳ��
	CWzdUS02PageDlg *CreateUS02GetPortDlg(unsigned short x, unsigned short y, unsigned short cx, unsigned short cy, bool show,int pageIndex);
	//������ȡ·��ipҳ��
	CWzdUS02PageDlg *CreateUS02GetRouterIpDlg(unsigned short x, unsigned short y, unsigned short cx, unsigned short cy, bool show,int pageIndex);
	//�����ƶ��豸ѡ��ҳ��
	CWzdUS02PageDlg *CreateUS02SelectMobileDlg(unsigned short x, unsigned short y, unsigned short cx, unsigned short cy, bool show,int pageIndex);
	//�����ƶ��豸��ʾҳ��
	CWzdUS02PageDlg *CreateUS02MobileTipDlg(unsigned short x, unsigned short y, unsigned short cx, unsigned short cy, bool show,int pageIndex);
	//����DDNSʹ����ʾҳ��
	CWzdUS02PageDlg *CreateUS02DDNSTipDlg(unsigned short x, unsigned short y, unsigned short cx, unsigned short cy, bool show,int pageIndex);
	//����DDNS������ʾҳ��
	CWzdUS02PageDlg *CreateUS02DDNSConnectTipDlg(unsigned short x, unsigned short y, unsigned short cx, unsigned short cy, bool show,int pageIndex);
	//������ά����ʾҳ��
	CWzdUS02PageDlg *CreateUS02QRShowDlg(unsigned short x, unsigned short y, unsigned short cx, unsigned short cy, bool show,int pageIndex);
	//�������ҳ��
	CWzdUS02PageDlg *CreateUS02EndUpDlg(unsigned short x, unsigned short y, unsigned short cx, unsigned short cy, bool show,int pageIndex);
private:
	//����������ַ���ID,���ض�Ӧ���ַ���
	void GetTipText(unsigned long strID,char *errText);
protected:
	int m_curIndex;								//��ǰҳ�����

	US02_WzdInfo*		m_pUS02WzdInfo;			//������

	std::vector<CWzdUS02PageDlg*>	m_vecWzdPage;	//��ҳ��

	unsigned long m_sysDiskStatus;				//ϵͳ����״̬

	bool m_bEnterConfig;						// �Ƿ��������
	unsigned long   m_displayResolutionMask;
	LONGLONG	m_curTime;
	LONGLONG	m_lastTime;
	LONGLONG    m_maxTime;

	enum WZD_PAGE
	{
		PAGE_NETWORK		= 3,
		PAGE_CHECKUPNP		= 4,
		PAGE_PORT			= 5,
		PAGE_ROUTERIP		= 6,
		PAGE_MOBIELESELECT	= 7,
	};


		//############�������Ըı�
	DECLARE_MSG_MAP()
	unsigned long  OnChangeLanguage();
};







#endif