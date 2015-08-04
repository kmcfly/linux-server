/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : ZXX
** Date         : 2012-10-17
** Name         : WzdUS02DDNSTipDlg.h
** Version      : 1.0
** Description  : �򵼹���Ի���
** Modify Record:
***********************************************************************/

#ifndef _WZD_US02_DDNSTIP_DLG_H_
#define _WZD_US02_DDNSTIP_DLG_H_
#include "Dialog.h"
#include "WzdUS02PageDlg.h"
#include "RichTextCtrl.h"
#include "CheckCtrl.h"
#include "ListCtrl.h"
#include "ComboCtrl.h"
#include "NetProtocol.h"
#include <string>
using namespace GUI;

class CWzdUS02DDNStipDlg:public CWzdUS02PageDlg
{
public:
	CWzdUS02DDNStipDlg();
	~CWzdUS02DDNStipDlg();
	void SetWzdInfo(US02_WzdInfo* pWzdInfo);
	void ShowWzdData(bool bRefresh);
	unsigned long CheckValue();
protected:
	void OnInitial();
	void OnDestroy();
	//�ڼ��DDNS������ȷ�Ե�ʱ��ú��������ⲿ����
	unsigned long OnPreCommand(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);
	
protected:
	DECLARE_MSG_MAP()
	unsigned long OnCheck();
	unsigned long OnClickList(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);
private:
	void FUIToIPServerUI();
	void IPServerUIToFUI();
	void CheckEnableUI(bool bRefresh);
	unsigned long CheckDdnsParam();
	unsigned long GetErrorTextNum();
protected:

	typedef enum _parameter_check_status
	{
		CHECK_STATUS_NOCHANGE	=0,					//�������û�иı�
		CHECK_STATUS_OK			=1,					//���Գɹ�
		CHECK_STATUS_NOCHECK	=2,					//û�н��в���			
		CHECK_STATUS_FAIL		=3,					//����ʧ��
	}CHECK_STATUS;
private:
	GUI::CRichTextCtrl  m_richText;
	GUI::CCheckCtrl		m_checkCtrl;				//�´��Ƿ���ʾ��
	GUI::CListCtrl		m_listDDNS;

	US02_WzdInfo		*m_pWzdInfo;

	bool				m_bCheckParam;
	CHECK_STATUS		m_StatusddnsCheckOK;
	NET_CTRL_FAIL		m_netCheckResult;
	

	enum LIST_ROW
	{
		ROW_DDNS_ENABLE = 0,
		ROW_DDNS_TYPE = 1,
		ROW_DDNS_SERVER_NAME = 2,
		ROW_DDNS_USER_NAME = 3,
		ROW_DDNS_PASSWORD = 4,
		ROW_DDNS_DOMAIN_INFO = 5,
		ROW_DDNS_UPDATE_CYCLE = 6,

		ROW_DDNS_TEST = 7,
	};
};



#endif

