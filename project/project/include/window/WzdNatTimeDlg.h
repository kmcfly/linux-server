/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : ZXX
** Date         : 2013-11-26
** Name         : WzdNatTimeDlg.h
** Version      : 1.0
** Description  : ��ҳ��Ի���
** Modify Record:
***********************************************************************/


#ifndef _WZD_NAT_Time_DLG_H_
#define _WZD_NAT_Time_DLG_H_
#include "Dialog.h"
#include "WzdNatPageDlg.h"
#include "ListCtrl.h"

using namespace GUI;

class CWzdNatTimeDlg:public CWzdNatPageDlg
{
public:
	CWzdNatTimeDlg();
	~CWzdNatTimeDlg();

	//�ڻ�ȡ���ݺ����ÿؼ���Ϣ
	void InitCtrlInfo();
	//��������Ϣ
	void SetWzdInfo(NAT_WIZARD_INFO* pWzdInfo);
	//��ʾ������
	void ShowWzdData(bool bRefresh);

	//ҳ����ɶ�ȡ����Ϣ
	void GetWzdData(bool bNext = true);

	void OnLeavePage();

	bool GetCurrZoneTimeInfo(ZONE_TIME_INFO& zoneTimeInfo, int zone);
protected:
	void OnInitial();
	void OnIdle();

	bool IsModify();
	void SetModify(bool modify, bool bRefresh);
	bool CanModify();

	void RefreshDateTime(LONGLONG gmt, int zone, bool bRefresh);
	void GetDateTime(LONGLONG& gmt, int zone);

	unsigned long OnPreCommand(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);
protected:
	DECLARE_MSG_MAP()
	unsigned long OnClickList(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);
protected:
	GUI::CListCtrl		m_listTimeCtrl;
	enum LIST_ROW
	{
		ROW_SYSTEM_TIME,
		ROW_DATE_FORMAT,
		ROW_TIME_FORMAT,
		ROW_TIME_ZONE,
		ROW_SYSTEM_DATE,
		ROW_TIME_SYNC,
		ROW_TIME_SERVER,
		ROW_TIME_UPDATENOW,
	};

	NAT_WIZARD_INFO * m_pNatTimeWzdInfo;

	bool				m_bModifyTime;		//�Ƿ��޸�ʱ��
	bool				m_bStopRefreshTime;	//�޸�ʱ��ʱ������ֹͣˢ��

	LONGLONG			m_pre_sys_tm;		//��һ�ε�ϵͳʱ��[��ʱ��]
	LONGLONG			m_gmt_user_tm;		//�û��޸ĵ�GMT0ʱ��ʱ��

	LONGLONG			m_refresh_count_tm;	//�ۼ�ˢ��ʱ��

protected:
	void SetEnableUI(  LIST_ROW rowNum, bool brefalsh );
private:
	bool				m_bUpdateTimeNow;						//ΪtrueʱҪ��ʱNTPͬ������ʱ������������Ϣ
};


#endif

