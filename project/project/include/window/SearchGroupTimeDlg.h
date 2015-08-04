/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : zxx
** Date         : 2012-02-20
** Name         : SearchGroupTimeDlg.h
** Version      : 3.0
** Description  : 
** Modify Record:
1:
***********************************************************************/
#if defined(__GROUY_SEARCH__)
#ifndef _SEARCH_GROUP_TIME_DLG_H_
#define _SEARCH_GROUP_TIME_DLG_H_

#include "Dialog.h"
#include "Static.h"
#include "DateCtrl.h"
#include "TimeCtrl.h"
#include "CheckCtrl.h"
#include "Button.h"
#include "CalendarDlg.h"
#include "SearchAddGroupDlg.h"
#include "ConfigEx.h"
#include "ConfigBlock.h"
#include "MessageMan.h"
#include "MessageBox.h"
#include "UserMan.h"
#include "StringDefine.h"
#include <list>
using namespace GUI;

class CSearchGroupTimeDlg : public GUI::CDialog
{
public:
	CSearchGroupTimeDlg();
	~CSearchGroupTimeDlg();

public:
	void SetInitialInfo(unsigned long channelNum, bool bBackUp, ULONGLONG authCH = INVALID_INDEX(ULONGLONG));

	unsigned long TimeStart();
	unsigned long TimeEnd();
	ULONGLONG CH();	

	void SetStartTime(tm time);
	void SetEndTime(tm time);

	int SaveGroupItemCfg();
	int LoadGroupItemCfg();
	bool IsModify() const {return m_bModify;}
protected:
	void OnInitial();
	void OnPostInitial();
	void OnDestroy();
	void OnPaint(PAINT_STATUS status);

	unsigned long DoClickChnn( unsigned long index, bool bRefresh );
	void GetItemList(std::vector<NCFG_ITEM_ID>& itemList) const;
	bool ShowData(NCFG_ITEM_ID cfgID, const BYTE* pData, int len, bool bRefresh);
	bool SetData(NCFG_ITEM_ID cfgID, BYTE*& pData, unsigned long& len)  const;
	

	unsigned long OnClickCheckGroup();
	unsigned long OnClickComboGroup();
	unsigned long OnClickBtnAdd();
	unsigned long OnClickBtnDel();
	unsigned long OnClickBtnEdit();
protected:
	DECLARE_MSG_MAP()
	unsigned long OnTimeStart(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);
	unsigned long OnTimeEnd(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);

	unsigned long OnClickChnn( unsigned long ctrlID );
	unsigned long OnClickGroup( unsigned long ctrlID );
protected:
	GUI::CStatic		m_scStart;
	GUI::CStatic		m_scEnd;

	GUI::CDateCtrl		m_dateStart;
	GUI::CTimeCtrl		m_timeStart;
	GUI::CDateCtrl		m_dateEnd;
	GUI::CTimeCtrl		m_timeEnd;

	GUI::CCheckCtrl		*m_pCKChnn;
	GUI::CCheckCtrl		*m_pCKGroup;

	CCalendarDlg		m_calendarDlg;

	GUI::CCheckCtrl m_checkCtrlGroup;
	GUI::CStatic m_staticGroup;
	GUI::CComboCtrl m_comboCtrlGroup;

	GUI::CButton m_buttonAdd;
	GUI::CButton m_buttonDel;
	GUI::CButton m_buttonEdit;
protected:
	unsigned long       m_channelNum;
	ULONGLONG			m_authCH;
private:
	ULONGLONG GetCurChannel() const;
	void SetChannel(const ULONGLONG chanbits);
	void SetAllCheckEnable(bool benable);
	void SetAllCheck(bool bcheck);//����ҳ��������check��״̬��ȫ��Ϊ��ѡ�����޹�ѡ
	void SetComboCurChannel(int curItem);
private:
	bool m_bModify;
	int m_enable;//����ʹ��,�����ö�ȡ������ֻ��������һ��ҳ�汣���ű���������
	GROUP_ITEM_LIST m_cfgGroupList;//��ȡ���������ô���list
	unsigned long m_curItem;//�����ö�ȡ�����ĵ�ǰҳ����ʾ������������,��Ϊ��list,����Ҳ��combo���
	CSearchAddGroupDlg m_DlgAddGroup;
	bool m_bBackUp;//�Ƿ��Ǳ���ҳ��
};


#endif
#endif




