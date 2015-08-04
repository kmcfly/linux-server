/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : ZXX
** Date         : 2013-12-26
** Name         : WzdNatPassWordDlg.h
** Version      : 1.0
** Description  : ���޸�����ҳ��
** Modify Record:
***********************************************************************/

#ifndef _WZD_NEW_NAT_PASSWORD_DLG_H_
#define _WZD_NEW_NAT_PASSWORD_DLG_H_
#include "WzdNatPageDlg.h"
#include "RichTextCtrl.h"
#include "ScrollCtrl.h"
#include "CheckCtrl.h"
#include "ListCtrl.h"
#include <string>
#include "CfgDlg.h"
using namespace GUI;

//////////////////////////////////////////////////////////////////////////
// * �����ô�͸���޸�����ҳ��
//////////////////////////////////////////////////////////////////////////

class CWzdNewNatPasswordDlg : public CWzdNatPageDlg
{
public:
	CWzdNewNatPasswordDlg();
	~CWzdNewNatPasswordDlg();

	//��������Ϣ
	void SetWzdInfo(NAT_WIZARD_INFO* pWzdInfo);
	void OnLeavePage();

	//��ʾ������
	void ShowWzdData(bool bRefresh);
	//ҳ����ɶ�ȡ����Ϣ
	void GetWzdData(bool bNext);
	
protected:
	void OnInitial();
	void InitCtrlInfo();
	void OnDestroy();
	void SetTipInfo();
	void OnPostInitial();//�����û����������������󳤶�
protected:
	DECLARE_MSG_MAP()
	unsigned long OnCheck();
	unsigned long OnScrollchange(unsigned long ctrlID, unsigned long keyID, unsigned long WzdNewNat, unsigned long yPos);
	unsigned long OnClickList(unsigned long ctrlID, unsigned long keyID, unsigned long WzdNewNat, unsigned long yPos);

protected:
	GUI::CRichTextCtrl	m_richTextCtrl;
	GUI::CListCtrl		m_listCtrl;
	GUI::CCheckCtrl		m_checkCtrl;
	GUI::CScrollCtrl	m_scroll_v;

	NAT_WIZARD_INFO     * m_pWzdInfo;
	unsigned short		m_toprow;//������ʾ�ĵ�һ��

	enum LIST_ROW
	{
		ROW_USER_NAME,
	//	ROW_ORIGIAN_PASSWORD,
		ROW_NEW_PASSWORD,
		ROW_CONFIRM_PASSWORD,
		ROW_COUNT,
	};
private:
	string			m_modifyName;//��Ҫ�޸ĵ��û���
	//string			m_origianPass;//ԭ�ȵ�����
	string			m_newPass;//�µ�����
	string			m_confirmPass;//ȷ�ϵ�����
	bool			m_bChangePassword;
	
};

#endif
