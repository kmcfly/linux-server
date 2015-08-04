/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : ZXX
** Date         : 2013-11-26
** Name         : WzdNewNatGetNetWorkSetUpDlg.h
** Version      : 1.0
** Description  : �򵼹���Ի���
** Modify Record:
***********************************************************************/

#ifndef _WZD_NAT_GETNETWORKSETUP_DLG_H_
#define _WZD_NAT_GETNETWORKSETUP_DLG_H_
#include "Dialog.h"
#include "WzdNatPageDlg.h"
#include "ScrollCtrl.h"
#include "ListCtrl.h"
#include "Static.h"
#include "RichTextCtrl.h"
#include "CheckCtrl.h"
#include "RadioCtrl.h"
class CWzdNewNatGetNetWorkSetUpDlg : public CWzdNatPageDlg
{
public:
	CWzdNewNatGetNetWorkSetUpDlg();
	~CWzdNewNatGetNetWorkSetUpDlg();

	//��������Ϣ
	void SetWzdInfo(NAT_WIZARD_INFO* pWzdInfo);
	//�ڻ�ȡ���ݺ����ÿؼ���Ϣ
	void InitCtrlInfo();
	//��ʾ������
	void ShowWzdData(bool bRefresh);

	//ҳ����ɶ�ȡ����Ϣ
	void GetWzdData(bool bNext = true);

	bool GetNatEnaleFlag();
protected:
	void OnInitial();
	void OnDestroy();
private:
	bool GetIp();
	void GetStaticIp(unsigned long ip,char *pIP);//��unsigned long ip��װΪ�ַ�����
protected:
	DECLARE_MSG_MAP()
	unsigned long OnCheck();
	unsigned long OnScrollchange(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);
	unsigned long OnClickRadioCtrl();//ѡ��ͬ��ѡ�����һ��ҳ��Ҳ���������ͬ
protected:
	GUI::CListCtrl		 m_netList;
	GUI::CRichTextCtrl   m_netNoteRichText;
	GUI::CCheckCtrl		 m_checkCtrl;				//�´��Ƿ���ʾ��
	GUI::CScrollCtrl	 m_scroll_v;
	GUI::CRadioGroup	 m_rdGroup;
	GUI::CRadioCtrl		 m_rdNat;
	GUI::CRadioCtrl		 m_rdLocal;


	unsigned short		m_toprow;//������ʾ�ĵ�һ��
private:
	NAT_WIZARD_INFO		*m_pNetWzdInfo;
	enum LIST_ROW
	{
		ROW_NET_STATUS,
		ROW_GETIP_STATUS,
		ROW_DVRIP,
	};

	typedef enum _NET_SELECT_
	{
		NET_NAT,
		NET_LOCAL,
	}NET_SELECT;

	bool m_bEnableNatFlag;//�Ƿ�򿪴�͸�ı�־��Ĭ��Ϊtrue;
};

#endif

