/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : wn
** Date         : 2010-12-01
** Name         : SearchGroupConditionDlg.h
** Version      : 1.0
** Description  : 
** Modify Record:
** Modify BY    :wn,zxx
1:  注意： 通道总数有外面传入
***********************************************************************/
#if defined(__GROUY_SEARCH__)
#ifndef _SEARCH_GROUP_CONDITION_DLG_H_
#define _SEARCH_GROUP_CONDITION_DLG_H_
#include "Button.h"
#include "CheckCtrl.h"
#include "Dialog.h"
#include "CalendarCtrl.h"
#include "Static.h"
#include "ComboCtrl.h"
#include "SearchAddGroupDlg.h"
#include "ConfigEx.h"
#include "ConfigBlock.h"
#include <list>
using namespace GUI;


class CSearchGroupConditionDlg : public GUI::CDialog
{
public:
	CSearchGroupConditionDlg();
	~CSearchGroupConditionDlg();

	//////////////////////////////////////////////////////////////////////////
	void SetInitialInfo(unsigned long channelNum, ULONGLONG authCH);
	unsigned long Time();
	ULONGLONG CH();	
	void MarkDay(unsigned short year, unsigned char month, unsigned char day, unsigned long color = COLOR_BTN_DEFAULT_MARK, bool bRefresh = true);
	bool IsModify() const {return m_bModify;}
	int SaveGroupItemCfg();
	int LoadGroupItemCfg();
protected:
	void OnInitial();
	void OnPostInitial();
	void OnDestroy();
	void OnPaint(PAINT_STATUS status);
protected:
	DECLARE_MSG_MAP()
	unsigned long OnClickChnn( unsigned long ctrlID );
	unsigned long OnClickGroup( unsigned long ctrlID );
	unsigned long OnClickCheckGroup();
	unsigned long OnClickComboGroup();
	unsigned long OnClickBtnAdd();
	unsigned long OnClickBtnDel();
	unsigned long OnClickBtnEdit();
protected:
	void GetItemList(std::vector<NCFG_ITEM_ID>& itemList) const;

	bool ShowData(NCFG_ITEM_ID cfgID, const BYTE* pData, int len, bool bRefresh);
	bool SetData(NCFG_ITEM_ID cfgID, BYTE*& pData, unsigned long& len)  const;
private:
	ULONGLONG GetCurChannel() const;
	void SetChannel(const ULONGLONG chanbits);
	void SetAllCheckEnable(bool benable);
	void SetAllCheck(bool bcheck);//设置页面上所有check框状态，全部为勾选或者无勾选
	void SetComboCurChannel(int curItem);
	unsigned long DoClickChnn( unsigned long index, bool bRefresh );
private:
	//////////////////////////////////////////////////////////////////////////
	GUI::CCheckCtrl				*m_pCKCH, *m_pCKGroup;
	GUI::CCalendarCtrl			m_calendar;
	unsigned long				m_channelNum;	
	ULONGLONG					m_authCH;
	GUI::CCheckCtrl m_checkCtrlGroup;
	GUI::CStatic m_staticGroup;
	GUI::CComboCtrl m_comboCtrlGroup;

	GUI::CButton m_buttonAdd;
	GUI::CButton m_buttonDel;
	GUI::CButton m_buttonEdit;

	CSearchAddGroupDlg m_DlgAddGroup;

	bool m_bModify;
	int m_enable;//分组使能,从配置读取上来，只有在另外一个页面保存后才保存入配置
	GROUP_ITEM_LIST m_cfgGroupList;//读取上来的配置存入list
	unsigned long m_curItem;//从配置读取上来的当前页面显示的数组分组序号,因为是list,所以也是combo序号
	
};
#endif //_SEARCH_CONDITION_DLG_H_
#endif
