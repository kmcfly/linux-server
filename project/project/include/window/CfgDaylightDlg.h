/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author			: L
** Date				: 2009-11-12
** Name			: CfgDaylightDlg.h
** Version			: 1.0
** Description	: 
** Modify Record:
1:
***********************************************************************/

#ifndef _CFG_DAYLIGHT_DLG_H_
#define _CFG_DAYLIGHT_DLG_H_

#include "Dialog.h"
#include "RadioCtrl.h"
#include "ComboCtrl.h"
#include "Static.h"
#include "ListCtrl.h"
#include "TimeCtrl.h"
#include "DateCtrl.h"
#include "CfgDlg.h"

using namespace GUI;

//////////////////////////////////////////////////////////////////////////
/* 关于本窗口数据处理方式
 *     将CCfgDaylightDlg::m_dstData数据区域直接交给
 * [周/日期]子窗口管理,子窗口可能修改值的动作都通知
 * CCfgDaylightDlg去检测处理.
*/
//////////////////////////////////////////////////////////////////////////

class CDstWeekDlg : public GUI::CDialog
{
public:
	CDstWeekDlg();
	~CDstWeekDlg();

	void SetDstData(DAYLIGHT_INFO* pDst) { m_dst = pDst;}

	//刷新数据到界面
	void RefreshView(bool bRefresh=true);

protected:
	void OnModifyWeekData();

protected:
	void OnInitial();
	void OnPaint(PAINT_STATUS status);

protected:
	DECLARE_MSG_MAP()
	unsigned long OnClickList(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);

protected:
	GUI::CListCtrl			m_listCtrl;

	enum LIST_ROW
	{
		ROW_START_MONTH,
		ROW_START_WEEKOFMON,
		ROW_START_WEEK,
		ROW_START_TIME,

		ROW_END_MONTH,
		ROW_END_WEEKOFMON,
		ROW_END_WEEK,
		ROW_END_TIME,
	};

protected:
	DAYLIGHT_INFO*		m_dst;
};

//////////////////////////////////////////////////////////////////////////

class CDstDateDlg : public GUI::CDialog
{
public:
	CDstDateDlg();
	~CDstDateDlg();

	void SetDstData(DAYLIGHT_INFO* pDst) { m_dst = pDst;}

	//刷新数据到界面
	void RefreshView(bool bRefresh=true);

protected:
	void OnModifyDateData();

protected:
	void OnInitial();
	void OnPaint(PAINT_STATUS status);

protected:
	DECLARE_MSG_MAP()
	unsigned long OnClickList(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);

protected:
	GUI::CListCtrl			m_listCtrl;

	enum LIST_ROW
	{
		ROW_START_DATE,
		ROW_START_TIME,

		ROW_END_DATE,
		ROW_END_TIME,
	};

protected:
	DAYLIGHT_INFO*		m_dst;
};

//////////////////////////////////////////////////////////////////////////

class CCfgDaylightDlg : public CCfgDlg
{
public:
	CCfgDaylightDlg();
	~CCfgDaylightDlg();

	enum PAGE_ITEM
	{
		PAGE_NULL	= -1,
		PAGE_WEEK	= DAYLIGHT_TYPE_WEEK,
		PAGE_DATE	= DAYLIGHT_TYPE_DAY,
	};

public:
	//检查数据是否被修改
	void OnModifyDstData();

protected:
	void SetPage(PAGE_ITEM item = PAGE_WEEK,bool bRefresh = true );
	unsigned long CheckValue();
	void CheckUI(bool bRefresh);

protected:
	bool ShowData(NCFG_ITEM_ID cfgID, const BYTE* pData, int len, bool bRefresh);
	bool SetData(NCFG_ITEM_ID cfgID, BYTE*& pData, int& len)  const;

protected:
	//刷新数据到界面
	void RefreshView(bool bRefresh=true);

protected:
	void OnInitial();
	void OnDestroy();
	void OnPaint(PAINT_STATUS status);
	unsigned long OnOK();

protected:
	DECLARE_MSG_MAP()
	unsigned long OnClickList(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);
	unsigned long OnClickRadio();

protected:
	GUI::CListCtrl			m_listCtrl;
	GUI::CRadioGroup	m_rdGroup;
	GUI::CRadioCtrl		m_rdWeek;
	GUI::CRadioCtrl		m_rdDate;
	GUI::CStatic				m_scInfo;

	CDstWeekDlg			m_dstWeekDlg;
	CDstDateDlg			m_dstDateDlg;

	enum LIST_ROW
	{
		ROW_ENABLE_DST,
		ROW_TIME_OFFSET,
	};

private:
	PAGE_ITEM	m_curPage;

	DAYLIGHT_INFO		m_dstData;//配置ID项目绑定于本内存区域
};

#endif