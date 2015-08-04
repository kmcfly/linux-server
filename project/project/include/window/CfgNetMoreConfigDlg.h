/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : zhl
** Date         : 2010-06-9
** Name         : CfgNetMoreConfigDlg.h
** Version      : 1.0
** Description  : 
					���������е��������ã���ʱֻ��DDNS������
			
** Modify Record:
1:
***********************************************************************/

#ifndef _CFG_NET_DDNS_DLG_H_
#define _CFG_NET_DDNS_DLG_H_

#include "Dialog.h"
#include "ListCtrl.h"
#include "CfgDlg.h"

const char szVISIONICA_DNS_SERVER_NAME[] = ".visionica.info";

class CCfgNetMoreConfigDlg : public CCfgDlg
{
public:
	CCfgNetMoreConfigDlg();
	~CCfgNetMoreConfigDlg();

protected:

	typedef enum _parameter_check_status
	{
		CHECK_STATUS_NOCHANGE	=0,					//�������û�иı�
		CHECK_STATUS_OK			=1,					//���Գɹ�
		CHECK_STATUS_NOCHECK	=2,					//û�н��в���			
		CHECK_STATUS_FAIL		=3,					//����ʧ��
	}CHECK_STATUS;
	
	typedef enum _use_which_list_
	{
		USE_FIRST_LIST	= 0,
		USE_SECOND_LIST	= 1,
	}USE_WHICH_LIST;

protected:
	virtual void OnInitial();

	//�ڼ��DDNS������ȷ�Ե�ʱ��ú��������ⲿ����
	unsigned long OnPreCommand(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);
	
	void SetTip();

protected:
	bool ShowData(NCFG_ITEM_ID cfgID, const BYTE* pData, int len, bool bRefresh);
	bool SetData(NCFG_ITEM_ID cfgID, BYTE*& pData, int& len)  const;

	void CheckEnableUI( bool bRefresh );
	void CheckEnableUIS( bool bRefresh );
	unsigned long GetErrorTextNum();

	unsigned long CheckDdnsParam();
	unsigned long CheckValue();

protected:
	DECLARE_MSG_MAP()
	unsigned long OnClickList(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);

	void ClickFirstList(unsigned short row, unsigned short col);
	bool ChangeToSList(bool bRefresh);
	void ClickSecondList(unsigned short row, unsigned short col);
	bool ChangeToFList(bool bRefresh);
	void RefreshListF(const DDNS_INFO* pDDNS, bool bRefresh);
	void RefreshListS(const DDNS_INFO* pDDNS, bool bRefresh);
	void SetMacToDomianInfo(const DDNS_INFO *pDNS);

	void InitialListF(bool bRefresh);
	void InitialListS(bool bRefresh);

	void FUIToIPServerUI(GUI::CListCtrl& list);
	void IPServerUIToFUI(GUI::CListCtrl& list);

protected:
	GUI::CListCtrl			m_cfgNetMoreListF;
	GUI::CListCtrl			m_cfgNetMoreListS;
	USE_WHICH_LIST			m_useWhichList;


	DDNS_SERVER_INFO*		m_pDdnsSeverInfo;						//�����е�DDNS_SERVER_INFO��Ϣ
	int						m_ddnsServerCount;						//�ܹ�ӵ�е�DDNS_SERVER_INFO����

	bool					m_bCheckParam;
	CHECK_STATUS			m_StatusddnsCheckOK;
	NET_CTRL_FAIL			m_netCheckResult;
	

	DDNS_INFO				m_ddnsInfo;
	unsigned char			m_ddnsServerName[132];
	unsigned long			m_ddnsUpdateCycle;
	unsigned long			m_bUpnp;

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

		ROW_UPNP_ENABLE = 9,

		ROW_NUM = 8,
	};


	enum LIST_ROW_S
	{
		ROW_S_DDNS_ENABLE = 0,
		ROW_S_DDNS_TYPE = 1,
		ROW_S_DDNS_MAIL_ADDR = 2,
		
		ROW_S_DDNS_DOMAIN_INFO = 3,
		ROW_S_DDNS_UPDATE_CYCLE = 4,

		ROW_S_DDNS_REGISTER = 5,

		ROW_S_UPNP_ENABLE = 7,

		ROW_S_NUM = 7,
	};

	bool m_bAutoReg;
private:
	bool CheckDomainParam();//���IN23���������Ƿ�Ϸ�
};

#endif


