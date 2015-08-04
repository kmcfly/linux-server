/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : ZXX
** Date         : 2012-09-19
** Name         : WzdUS02GetNetWorkSetUpDlg.h
** Version      : 1.0
** Description  : �򵼹���Ի���
** Modify Record:
***********************************************************************/

#ifndef _WZD_US02_GETNETWORKSETUP_DLG_H_
#define _WZD_US02_GETNETWORKSETUP_DLG_H_
#include "Dialog.h"
#include "WzdUS02PageDlg.h"
#include "ListCtrl.h"
#include "Static.h"
#include "RichTextCtrl.h"
#include "CheckCtrl.h"
class CWzdUS02GetNetWorkSetUpDlg : public CWzdUS02PageDlg
{
public:
	CWzdUS02GetNetWorkSetUpDlg();
	~CWzdUS02GetNetWorkSetUpDlg();

	//��������Ϣ
	void SetWzdInfo(US02_WzdInfo* pWzdInfo);
	//�ڻ�ȡ���ݺ����ÿؼ���Ϣ
	void InitCtrlInfo();
	//��ʾ������
	void ShowWzdData(bool bRefresh);

	//ҳ����ɶ�ȡ����Ϣ
	void GetWzdData(bool bNext = true);
protected:
	void OnInitial();
	void OnDestroy();
private:
	bool GetIp();
	void GetStaticIp(unsigned long ip,char *pIP);//��unsigned long ip��װΪ�ַ�����
protected:
	DECLARE_MSG_MAP()
	unsigned long OnCheck();
protected:
	GUI::CListCtrl		 m_netList;
	GUI::CRichTextCtrl   m_netNoteRichText;
	GUI::CCheckCtrl		 m_checkCtrl;				//�´��Ƿ���ʾ��
	
private:
	US02_WzdInfo		*m_pNetWzdInfo;
	enum LIST_ROW
	{
		ROW_NET_STATUS,
		ROW_GETIP_STATUS,
		ROW_DVRIP,
	};

};




#endif

