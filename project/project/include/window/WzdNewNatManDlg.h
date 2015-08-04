/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : ZXX
** Date         : 2013-11-25
** Name         : WzdNatManDlg.h
** Version      : 1.0
** Description  : ��͸�򵼹���Ի���
** Modify Record:
***********************************************************************/

#ifndef _WZD__NEW_NAT_MAN_DLG_H_
#define _WZD_NEW_NAT_MAN_DLG_H_
#include "Static.h"
#include "CheckCtrl.h"
#include "DateCtrl.h"
#include "TimeCtrl.h"
#include "Dialog.h"
#include "CfgDlg.h"
#include <vector>
#include <string>
#include "WzdNatPageDlg.h"

using namespace GUI;

class CWzdNewNatManDlg : public CCfgDlg
{
public:
	CWzdNewNatManDlg();
	~CWzdNewNatManDlg();

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
	unsigned long SaveNatWizard();//���沢�˳�
	void OnIdle();				//�򵼶�ʱ�رմ���
protected:
	unsigned long CheckValue();
	//���ò������
	virtual void GetItemList(CFGITEM_LIST& itemList) const;
	virtual bool SetData(NCFG_ITEM_ID cfgID, BYTE*& pData, int& len) const;

	bool EnterConfig();
	void ExitConfig() const;

protected:
	void CreateNatWizardPage();	//��ʼ����ҳ��
	void CreateWizardInfo();		//�����򵼱���
	void InitWizardInfo();			//��ʼ���򵼱���


	void ShowPage(int index);		//��ʾĳһ��ҳ��
	void ShowBtn(int index);		//���°�ť״̬
	void OnWzdEnd();			//�򵼽�����Ч����

protected:
	//������͸�򵼻�ӭҳ�棨��һҳ��
	CWzdNatPageDlg* CreateNewNatStartDlg(unsigned short x, unsigned short y, unsigned short cx, unsigned short cy, bool bshow,int pageIndex);
	
	//������tab�ؼ����򵼵�2ҳ�棬���������ԡ�ʱ���dst����
	CWzdNatPageDlg* CreateNewNatTabBasicManDlg(unsigned short x, unsigned short y, unsigned short cx, unsigned short cy, bool bshow,int pageIndex);

	//������͸���޸�����ҳ��
	CWzdNatPageDlg* CreateNewNatPasswordDlg(unsigned short x, unsigned short y, unsigned short cx, unsigned short cy, bool bshow,int pageIndex);

	CWzdNatPageDlg* CreateRemoteAccessTipDlg(unsigned short x, unsigned short y, unsigned short cx, unsigned short cy, bool bshow,int pageIndex);



	//���IPҳ�棬�����ⲻ�������ܼ�������
	CWzdNatPageDlg* CreateNewNatGetNetWorkSetUpDlg(unsigned short x, unsigned short y, unsigned short cx, unsigned short cy, bool bshow,int pageIndex);
	

	//���ѡ���˱������ӣ���ҳ�������ҳ��
	CWzdNatPageDlg* CreateLocalConnectEndDlg(unsigned short x, unsigned short y, unsigned short cx, unsigned short cy, bool bshow,int pageIndex);

	//�����豸ѡ��ҳ�棨android,apple,pc��
	CWzdNatPageDlg* CreateNewNatSelectDevDlg(unsigned short x, unsigned short y, unsigned short cx, unsigned short cy, bool bshow,int pageIndex);
	
	//������ά��ҳ��
	CWzdNatPageDlg* CreateNewNatQrCodeDlg(unsigned short x, unsigned short y, unsigned short cx, unsigned short cy, bool bshow,int pageIndex);

	//����PC��ʾҳ��
	CWzdNatPageDlg* CreateNewNatPCTipDlg(unsigned short x, unsigned short y, unsigned short cx, unsigned short cy, bool bshow,int pageIndex);

	//������͸���ҳ��
	CWzdNatPageDlg *CreateNatEndUpDlg(unsigned short x, unsigned short y, unsigned short cx, unsigned short cy, bool bshow,int pageIndex);
private:
	//����������ַ���ID,���ض�Ӧ���ַ���
	void GetTipText(unsigned long strID,char *errText);
protected:
	int m_curIndex;								//��ǰҳ�����

	NAT_WIZARD_INFO*		m_pNatWzdInfo;			//������

	std::vector<CWzdNatPageDlg*>	m_vecWzdPage;	//��ҳ��

	unsigned long m_sysDiskStatus;				//ϵͳ����״̬

	bool m_bEnterConfig;						// �Ƿ��������
	unsigned long   m_displayResolutionMask;
	LONGLONG	m_curTime;
	LONGLONG	m_lastTime;
	LONGLONG    m_maxTime;
	
	//############�������Ըı�
	DECLARE_MSG_MAP()
	unsigned long  OnChangeLanguage();

	enum WZD_PAGE
	{
		PAGE_NATPASS		= 2,//�����޸�����ҳ��
		PAGE_NETWORK		= 4,
		PAGE_LOCALEND		= 5,//���ؽ���ҳ��
		PAGE_DEVSELECT		= 6,
		PAGE_QRCODE			= 7,
		PAGE_PCTIP			= 8,
	};

	int	m_devSelect;//ѡ����豸��Ĭ��ΪDEV_ANDROID

};







#endif