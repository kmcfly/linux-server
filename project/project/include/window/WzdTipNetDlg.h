/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : zxx
** Date         : 2010-07-20
** Name         : WzdTipNetDlg.h
** Version      : 1.0
** Description  : �򵼶Ի���
** Modify Record:
***********************************************************************/
#ifndef _WZD_PAGE_TIP_NET_H
#define _WZD_PAGE_TIP_NET_H

#include "Dialog.h"
#include "WndStructDef.h"
#include "WzdPageDlg.h"
#include "ListCtrl.h"
#include "NetProtocol.h"
using namespace GUI;

class CWzdTipNetDlg :public CWzdPageDlg
{
public:
	CWzdTipNetDlg();
	~CWzdTipNetDlg();
	
	//��ҳ���Ƿ�ʹ��
	void SetUsed(bool bUsed = true);
	bool IsUsed();

	//�ڻ�ȡ���ݺ����ÿؼ���Ϣ
	virtual void InitCtrlInfo();

	//��������Ϣ
	virtual void SetWzdInfo(WIZARD_INFO* pWzdInfo);

	//��ʾ������
	virtual void ShowWzdData(bool bRefresh);

	//ҳ����ɶ�ȡ����Ϣ
	virtual void GetWzdData(bool bNext = true);

	//��ȡ����IP�Ƿ��ͻ
	bool GetIpIsValid(){return m_bIpIsValid;};
	bool CheckIPIfConflict();
	unsigned long GetErrorTextNum();
	
protected:
	DECLARE_MSG_MAP()
	unsigned long OnClickList(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);
protected:
	void OnInitial();
	WIZARD_INFO* m_pWzdInfo;
	bool m_bUsed;
	void SetTip();
private:
	GUI::CListCtrl	NetSetList;
	enum LIST_ROW
	{
		ROW_HTTP_PORT = 0,
		ROW_SERVER_PORT = 1,

		ROW_GETIP_DHCP = 3,
		ROW_STATIC_IP = 4,
		ROW_SUB_NET_MASK = 5,
		ROW_GATEWAY = 6,
		ROW_DNS_1 = 7,
		ROW_DNS_2 = 8,

		ROW_PPPOE_ENABLE = 9,
		ROW_PPPOE_USER_NAME = 10,
		ROW_PPPOE_PASSWORD = 11,
		ROW_NET_TEST = 12,

		ROW_NUM = 13,
	};
private:
	
		
	typedef enum _refrash_ui_num
	{
		REFRASH_UI_AUTOIP  =0,
		REFRASH_UI_ALL     =2,
	}REFRASH_UI_NUM;
	unsigned long m_port;              //http�˿ں�
	unsigned long m_serverPort;        //����˿ں�
	NETWORK_ADDR_INFO m_addr_Info;     //��ַ����Ϣ
	unsigned long m_DNS_MainIP;          //��DNS����
	unsigned long m_DNS_SubIP;          //��DNS����
	bool m_getIpModeAuto;
private:
	void CheckEnableUI( bool bRefresh);
	void DoUpdateUI( bool bRefresh);

	GUI::CButton  *m_netTestButton;
	bool m_bIpIsValid;
	NET_CTRL_FAIL			m_netCheckResult;

};

#endif

