/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : zxx
** Date         : 2012-02-20
** Name         : SearchGroupScrollTimeDlg.h
** Version      : 3.0
** Description  : 
** Modify Record:
1:
***********************************************************************/

#if defined(__GROUY_SEARCH__)
#ifndef _SEARCH_GROUP_SCROLL_TIME__DLG_H_
#define _SEARCH_GROUP_SCROLL_TIME__DLG_H_

#include "Dialog.h"
#include "Static.h"
#include "DateCtrl.h"
#include "TimeCtrl.h"
#include "CheckCtrl.h"
#include "Button.h"
#include "SearchAddGroupDlg.h"
#include "ConfigEx.h"
#include "ConfigBlock.h"
#include "MessageMan.h"
#include "MessageBox.h"
#include "UserMan.h"
#include "StringDefine.h"
#include <list>

using namespace GUI;

class CSearchGroupScrollTimeDlg : public GUI::CScrollDialog
{
public:
	CSearchGroupScrollTimeDlg();
	~CSearchGroupScrollTimeDlg();

public:
	void SetInitialInfo(unsigned long channelNum, bool bBackUp,ULONGLONG authCH = INVALID_INDEX(ULONGLONG));
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

	void GetItemList(std::vector<NCFG_ITEM_ID>& itemList) const;
	bool ShowData(NCFG_ITEM_ID cfgID, const BYTE* pData, int len, bool bRefresh);
	bool SetData(NCFG_ITEM_ID cfgID, BYTE*& pData, unsigned long& len)  const;
protected:
	DECLARE_MSG_MAP()
	unsigned long OnClickScrollWnd(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);
	unsigned long OnTimeStart(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);
	unsigned long OnTimeEnd(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);

	unsigned long OnClickChnn( unsigned long ctrlID );
	unsigned long OnClickGroup( unsigned long ctrlID );
	unsigned long DoClickChnn( unsigned long index, bool bRefresh );

	unsigned long OnClickCheckGroup();
	unsigned long OnClickComboGroup();
	unsigned long OnClickBtnAdd();
	unsigned long OnClickBtnDel();
	unsigned long OnClickBtnEdit();
	
private:
	ULONGLONG GetCurChannel() const;
	void SetChannel(const ULONGLONG chanbits);
	void SetAllCheckEnable(bool benable);
	void SetAllCheck(bool bcheck);//设置页面上所有check框状态，全部为勾选或者无勾选
	void SetComboCurChannel(int curItem);

protected:
	unsigned long       m_channelNum;
	ULONGLONG			m_authCH;

	enum WND_ID
	{
		ID_START_STATIC = 0,
		ID_END_STATIC,
		ID_DATE_START,
		ID_TIME_START,
		ID_DATE_END,
		ID_TIME_END,

		ID_GROUP = 10,
		ID_CH = 20,

		ID_GROUP_CHECK  =60,//check框
		ID_GROUP_STATIC =70,
		ID_GROUP_COMBO  =80,
		ID_GROUP_BTNADD =90,
		ID_GROUP_BTNDEL =100,
		ID_GROUP_BTNEDIT =110,
	};
private:
	
	GUI::CCheckCtrl	 **m_pCKCH;
	GUI::CCheckCtrl	 **m_pCKGroup;
	GUI::CCheckCtrl* m_pCheckGroup;
	GUI::CComboCtrl* m_pComboCtrl;
	GUI::CButton* m_pBtnAdd;
	GUI::CButton* m_pBtnDel;
	GUI::CButton* m_pBtnEdit;

	CSearchAddGroupDlg m_DlgAddGroup;

	bool m_bModify;
	int m_enable;//分组使能,从配置读取上来，只有在另外一个页面保存后才保存入配置
	GROUP_ITEM_LIST m_cfgGroupList;//读取上来的配置存入list
	unsigned long m_curItem;//从配置读取上来的当前页面显示的数组分组序号,因为是list,所以也是combo序号
	bool m_bBackUp;
};

#endif
#endif
