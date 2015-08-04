/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : hz
** Date         : 2009-08-01
** Name         : CfgMotionDlg.h 
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/
#ifndef _CFG_MOTION_DLG_H_
#define _CFG_MOTION_DLG_H_

#include "CfgDlg.h"
#include "ListCtrl.h"
#include "Anchor.h"

#include "CfgTriggerDlg.h"

#include "CfgManDlg.h"
#include "CfgManDlg.cpp"
#include "CfgScheduleChildDlg.h"
#include "MotionAreDlg.h"

#include <map>

using namespace GUI;
/*********************zxx新加***************/
#ifdef __NETSERVER_HXHT__
typedef struct _client_point
{
	bool recover;//是否被覆盖
	bool enable;//是否有效
}CLIENT_POINT;

typedef struct _client_area_
{
	bool enable;
	unsigned short x;
	unsigned short y;
	unsigned short width;
	unsigned short height;
}CLIENT_AREA;
#endif
/************************************/
class CCfgMotionDlg : public CCfgDlg
{
public:
	CCfgMotionDlg();
	~CCfgMotionDlg();

	void UpdateView(bool bRefresh);
	unsigned long RecoverChange();

protected:
	DECLARE_MSG_MAP()
	void OnInitial();
	void OnDestroy();

	unsigned long OnClickList(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);
	unsigned long OnClickListAll(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);

	enum COL_INDEX
	{
		COL_CHANNEL,
		COL_ENABLE,
		#ifndef __CUSTOM_US02_NEW__
		COL_HOLD_TIME,
		#endif
		COL_TRIGGER,
		COL_AREA,
	};

	//重写
	bool ShowData(NCFG_ITEM_ID cfgID, const BYTE* pData, int len, bool bRefresh);
	bool SetData(NCFG_ITEM_ID cfgID, BYTE*& pData, int& len) const;
	DLG_END_VALUE PopupMontionAreaDlg( CMotionAreDlg &motionAreadlg, int chnn );

private:
	GUI::CListCtrl  m_listCtrl;

	GUI::CStatic	m_staticAll;
	GUI::CListCtrl  m_listCtrlAll;

	ALARM_TRIGGER_INFO* m_pTriggerInfo;
	MOTION_DETECT_INFO * m_pMotionInfo;

	std::map<int, int>  m_rowChnnMap;

#ifdef __NETSERVER_HXHT__
	HXHT_MOTION_AREA_INFO* m_hxhtpData;//hxht侦测区域
#endif
};

//////////////////////////////////////////////////////////////////////////

class CCfgMotionManDlg : public CCfgManDlg2<CCfgMotionDlg,CCfgScheduleChildDlg>
{
public:
	CCfgMotionManDlg();
	~CCfgMotionManDlg();

protected:
	void OnPreInitial();
	void GetItemList(CFGITEM_LIST& itemList) const;
};


//////////////////////////////////////////////////////////////////////////
// __DVR_ANCHOR__
//////////////////////////////////////////////////////////////////////////

class CCfgMotionManAnchorDlg : public GUI::CDialog
{
public:
	CCfgMotionManAnchorDlg();
	~CCfgMotionManAnchorDlg();

protected:
	void OnInitial();

protected:
	DECLARE_MSG_MAP()

	unsigned long OnAnchorMotion();
	unsigned long OnAnchorSchedule();
	unsigned long OnExit();

protected:
	GUI::CAnchor				m_anMotion;
	GUI::CAnchor				m_anSchedule;
	GUI::CAnchor				m_anExit;

	CCfgMotionManDlg			m_cfgMotionManDlg;
};

/*************zxx新加hxht配置类***********************/
#ifdef __NETSERVER_HXHT__
class CCfgHxhtMotion
{
public:
	CCfgHxhtMotion(MOTION_DETECT_INFO *pInfo);
	~CCfgHxhtMotion();
	void ChangeHxhtSqure(CLIENT_AREA *savesqure);//转为互信互通的东西
private:
	MOTION_DETECT_INFO * m_pHxhtInfo;
	CLIENT_POINT m_big[15][22];//本地转换需要
	//确定矩形高度，正常返回矩形高度，否则返回-1
	//参数：
	//xstart 开始行数 
	//ystart 开始列数
	//yend 结束列数
int CheckSqure(int xstart,int ystart,int yend);


/************************************/
};
#endif
#endif
//_CFG_MOTION_DLG_H_

