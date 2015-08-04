/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : L
** Date         : 2009-10-16
** Name         : DwellModeDlg.h 
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/

#ifndef _LIVE_DWELL_DLG_H_
#define _LIVE_DWELL_DLG_H_

#include "Dialog.h"
#include "Button.h"
#include "ComboCtrl.h"
#include "Static.h"
#include "DwellModeDlgNXN.h"
#include "NetConfigDefine.h"

using namespace GUI;

class CDwellModeDlg : public GUI::CDialog
{
public:
	CDwellModeDlg();
	~CDwellModeDlg();

	enum DLG_PAGE
	{
		PAGE_1X1 = 0,
		PAGE_2X2,
		PAGE_2X3,
		PAGE_3X3,
		PAGE_4X4,
		PAGE_5X5,
		PAGE_6X6,

		PAGE_MAX,
	};

	CDwellModeDlgNXN* GetPage(int index);
	CDwellModeDlgNXN* GetCurPage();
	GUI::CRect GetPageRect();
	int GetCurPageIndex() {return m_curPage;}

	void ShowPage( int index, bool bRefresh );

	void SetMaxChnn(int maxChnn, int localChnnNum,int netChannNum);
	void SetSpot(bool bSopt=false){m_bSpot = bSopt;}

	int GetMaxGroup();

	//设定修改绑定的数据,页面数据修改直接修改对应数据值
	void SetCfgData(unsigned long* pData,unsigned long* pDwell, int dwellLen);

	//向上级窗体发送修改数据动作
	void SendModifyCfgData(NCFG_ITEM_ID cfgID);

	//向上级窗口发送切换页面动作
	void SendModifyGroupInfo(bool bRefresh);

	//判断同一页面上的通道号不重复和不全无。
	unsigned long 	CheckDwell();

public:
	void SetCfgDwellTime(NCFG_ITEM_ID dwelltime ) { m_cfgDwellTime = dwelltime; }
	void SetCfgSplitBase(NCFG_ITEM_ID split) { m_cfgSplitBase = split; }

protected:
	void ShowData(int index,int group,bool bRefresh);

	int GetPageMax(int index);

	std::string GetGroupTip(ENUM_STRING_ID preID);

protected:
	void OnInitial();
	void OnDestroy();
	void SetTip();
	void OnPaint(PAINT_STATUS status);
	void OnChangeFocus(unsigned long oldID, unsigned long newID);

	unsigned long OnEnter(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);
	unsigned long OnMouseDrag(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);
	unsigned long OnMouseLeftUp(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);

protected:
	DECLARE_MSG_MAP()
	unsigned long OnBtnPre();
	unsigned long OnBtnNext();
	unsigned long OnDwellTime();
	unsigned long OnClickScrollWnd(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);

protected:
	CDwellModeDlgNXN		m_dwellModeDlg;

	GUI::CButton			m_btnPre;
	GUI::CButton			m_btnNext;

	GUI::CComboCtrl			m_cbDwellTime;
	GUI::CStatic			m_scDwellTime;
private:
	GUI::CRect				m_rectNXN;
	int						m_curPage;		//当前显示的页面
	int						m_group;		//当前组

protected:
	int						m_maxChnn;		//最大通道数
	int						m_localChnnNum;	//本地通道数
	int 					m_netChannNum;	//网络通道数
	int						m_dwellTimeLen; //
	unsigned long*			m_pData;		//引用的数据区域
	unsigned long*			m_pDwellTime;	//引用的数据区域

protected:
	NCFG_ITEM_ID			m_cfgDwellTime;	//跳台时间ID
	NCFG_ITEM_ID			m_cfgSplitBase;	//分割模式


	//鼠标操作
	bool			m_bDrag;
	bool			m_bFirstDrag;

	unsigned long	m_enterPosX;
	unsigned long	m_enterPosY;
	bool			m_bSpot;

	int 			m_indexCurItem[128];
};


#endif
