/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : L
** Date         : 2010-02-26
** Name         : WzdStepDlg.h
** Version      : 1.0
** Description  : ��ҳ��Ի���
** Modify Record:
***********************************************************************/

#ifndef _WZD_STEP_DLG_H_
#define _WZD_STEP_DLG_H_

#include "Dialog.h"
#include "WzdPageDlg.h"
#include "ListCtrl.h"
using namespace GUI;

//////////////////////////////////////////////////////////////////////////
// * �� ��һҳ��
//////////////////////////////////////////////////////////////////////////
class CWzdBasicSetDlg : public CWzdPageDlg
{
public:
	CWzdBasicSetDlg();
	~CWzdBasicSetDlg();

	//��ʾ������
	virtual void ShowWzdData(bool bRefresh);

	//ҳ����ɶ�ȡ����Ϣ
	virtual void GetWzdData(bool bNext = true);

	virtual void OnLeavePage();

	bool GetCurrZoneTimeInfo(ZONE_TIME_INFO& zoneTimeInfo, int zone);

protected:
	void OnInitial();
	void OnIdle();
	void InitCtrlInfo();
	
protected:
	bool IsModify();
	void SetModify(bool modify, bool bRefresh);
	bool CanModify();

	void RefreshDateTime(LONGLONG gmt, int zone, bool bRefresh);
	void GetDateTime(LONGLONG& gmt, int zone);

protected:
	DECLARE_MSG_MAP()
	unsigned long OnClickList(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);
	
protected:
	GUI::CListCtrl		m_listCtrl;

	bool				m_bModifyTime;		//�Ƿ��޸�ʱ��
	bool				m_bStopRefreshTime;	//�޸�ʱ��ʱ������ֹͣˢ��
	
	LONGLONG			m_pre_sys_tm;		//��һ�ε�ϵͳʱ��[��ʱ��]
	LONGLONG			m_gmt_user_tm;		//�û��޸ĵ�GMT0ʱ��ʱ��

	LONGLONG			m_refresh_count_tm;	//�ۼ�ˢ��ʱ��

	enum LIST_ROW
	{
		ROW_DEVICE_NAME,
		ROW_LANGUAGE,
		ROW_DATE_FORMAT,
		ROW_TIME_FORMAT,
		ROW_TIME_ZONE,
		ROW_SYSTEM_DATE,
		ROW_SYSTEM_TIME,
		ROW_SHOW_WZD,
	};
};

#endif
