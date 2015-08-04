/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : l
** Date         : 2009-08-12
** Name         : CfgScheduleChildDlg.h
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/

#ifndef _CFG_SCHEDULE_CHILD_DLG_H_
#define _CFG_SCHEDULE_CHILD_DLG_H_

#include "CfgDlg.h"
#include "ScheduleListCtrl.h"
#include "Static.h"
#include "ComboCtrl.h"
#include "StateButton.h"
#include "CfgScheduleEditDlg.h"

#include "PictureCtrl.h"

using namespace GUI;

typedef enum __schedule_child_type__
{
	SCHEDULE_CHILD_NONE,
	SCHEDULE_CHILD_RECORD,
	SCHEDULE_CHILD_MOTION,
	SCHEDULE_CHILD_ALARM,
	SCHEDULE_CHILD_COUNT,
}SCHEDULE_CHILD_TYPE;

class CCfgScheduleChildDlg : public CCfgDlg
{
public:
	CCfgScheduleChildDlg();
	~CCfgScheduleChildDlg();

	void SetMaxChnn(int max, int type = 0){ m_maxChnn = max; m_typeCH = type;}
	void SetScheduleChildType(SCHEDULE_CHILD_TYPE sct){m_scheduleChildType = sct;}
	void SetScheduleCfgID(NCFG_ITEM_ID week, NCFG_ITEM_ID holiday)
	{
		m_weekCfgID = week;
		m_holidayCfgID = holiday;
	}

	void GetScheduleCfgID(NCFG_ITEM_ID &week){week = m_weekCfgID;}

	enum STATE_SEL_OP
	{
		STATE_NULL = 0,
		STATE_ADD,
		STATE_DEC,
	};

	enum
	{
		STATE_SELECT_NO,
		STATE_SELECT_YES,
	};

	enum
	{
		COMBO_CHNN_ALL  = 244,
		COMBO_CHNN_NULL = 255,
	};

	enum
	{
		TYPE_CH = 0,
		TYPE_ALL,
	};

	void SetSetTip(){SetTip();}

protected:
	DECLARE_MSG_MAP()
	void OnInitial();
	void OnDestroy();

	void SetTip();
	//void OnChangeFocus(unsigned long oldID, unsigned long newID);

	void OnTipCopy( const char* preText );
	
	unsigned long OnDBClickWeekSchedule(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);
	unsigned long OnClickSchedule(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);
	unsigned long OnClickBtnHoliday();
	unsigned long OnClickBtnCopy();
	unsigned long OnChangeChnn();
	unsigned long OnClickBtnAdd();
	unsigned long OnClickBtnDec();

	unsigned long OnClickBtnEdit();

	void ShowWeekSchedule(bool bRefresh);
	void CreateHolidayScheduleList(const BYTE* pData, int len);
	BYTE* CreateHolidayData(int * pLen) const;

	void SetCurState(int state, bool bRefresh = false);

	//重写
	bool ShowData(NCFG_ITEM_ID cfgID, const BYTE* pData, int len, bool bRefresh);
	bool SetData(NCFG_ITEM_ID cfgID, BYTE*& pData, int& len)  const;

protected:
	GUI::CStatic m_scChannel;
	GUI::CComboCtrl m_comboChnn;
	GUI::CButton m_btnEdit;
	//GUI::CButton m_btnHoliday;
	GUI::CScheduleListCtrl m_scheduleWeekCtrl;
//	GUI::CScheduleListCtrl m_scheduleHolidayCtrl;

	GUI::CComboCtrl m_comboCopy;
	GUI::CButton m_btnCopy;
	GUI::CStatic m_stCopy;
	GUI::CStatic m_scCopyChnn;

	#ifdef __CUSTOM_US02_NEW__
		GUI::CStatic m_scTip;
		GUI::CPictureCtrl m_picTip;
	#endif

	GUI::CStateButton	m_btnAdd;
	GUI::CStateButton	m_btnDec;
	STATE_SEL_OP		m_curSelOp;

	NCFG_ITEM_ID m_weekCfgID;
	NCFG_ITEM_ID m_holidayCfgID;

	WEEK_SCHEDULE	 * m_pWeekSchedule;
	int				   m_weekScheduleLen;

	std::vector<HOLIDAY_SCHEDULE_LIST> m_holidayList;

	int					m_maxChnn;	//最大通道数目
	int					m_typeCH;	//是否使用不定通道

	SCHEDULE_CHILD_TYPE	m_scheduleChildType;
};

#endif
