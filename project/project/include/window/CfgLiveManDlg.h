/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : L
** Date         : 2009-08-15
** Name         : CfgLiveManDlg.h
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/

#ifndef _CFG_LIVE_MAN_DLG_H_
#define _CFG_LIVE_MAN_DLG_H_

#include "Dialog.h"
#include "TabCtrl.h"
#include "Anchor.h"

#include "CfgDlg.h"
#include "CfgLiveDlg.h"
#ifdef __DVR_BASIC__
#include "CfgLiveHostDlg.h"
#endif
#include "CfgLiveSpotDlg.h"
#include "CfgCoverDlg.h"
#include "CfgROIDlg.h"
#include "CfgCameraParamDlg.h"

using namespace GUI;

class CCfgLiveManDlg : public CCfgDlg
{
public:
	CCfgLiveManDlg();
	~CCfgLiveManDlg();

public:
	enum PAGE_ITEM
	{
		PAGE_LIVE,
#ifdef __DVR_BASIC__
		PAGE_HOST,
#endif
		PAGE_SPOT,
		PAGE_COVER,
		PAGE_ROI,
		PAGE_CAM_PARA,
	};

	void SetAnchorPage(PAGE_ITEM item);

protected:
	void GetItemList(CFGITEM_LIST& itemList) const;

protected:
	void OnInitial();

	unsigned long OnApply();

#ifdef __DVR_BASIC__
	unsigned long OnOK();

	unsigned long OnCancel();
#endif

	unsigned long CheckValue();

protected:
	CCfgDlg* GetPage(PAGE_ITEM item);

protected:
	unsigned long OnPreCommand(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);

	unsigned long OnVirtualEsc(unsigned long keyID, unsigned long xPos, unsigned long yPos);

protected:
	DECLARE_MSG_MAP()
	unsigned long OnClickLive();
	unsigned long OnClickHost();
	unsigned long OnClickSpot();
	unsigned long OnClickCover();
	unsigned long OnClickROI();
	unsigned long OnClickCamPara();

protected:
	GUI::CTabCtrl			m_tabCtrl;

	CCfgLiveDlg				m_cfgLiveDlg;
#ifdef __DVR_BASIC__
	CCfgLiveHostDlg			m_cfgLiveHostDlg;
#endif
	CCfgLiveSpotDlg			m_cfgLiveSpotDlg;
	CCfgCoverDlg			m_cfgCoverDlg;
	CCfgROIDlg              m_cfgROIDlg;
	CCfgCameraParamDlg      m_cfgCamParaDlg;

	PAGE_ITEM				m_curPage;
	unsigned long			m_initPageMask;			//��¼�Ѿ���ʼ����ҳ��

	//
	PAGE_ITEM				m_initAnchorPage;

	PAGE_ITEM				m_arrPages[6];
};


//////////////////////////////////////////////////////////////////////////
// __DVR_ANCHOR__

class CCfgLiveManAnchorDlg : public CCfgDlg
{
public:
	CCfgLiveManAnchorDlg();
	~CCfgLiveManAnchorDlg();

protected:
	void OnInitial();

protected:
	DECLARE_MSG_MAP()

	unsigned long OnAnchorLive();
	unsigned long OnAnchorLiveHost();
	unsigned long OnAnchorLiveSpot();
	unsigned long OnAnchorCover();
	unsigned long OnAnchorROI();
	unsigned long OnAnchorCamPara();
	unsigned long OnExit();

protected:
	GUI::CAnchor		m_anLive;
	GUI::CAnchor		m_anLiveHost;
	GUI::CAnchor		m_anLiveSpot;
	GUI::CAnchor		m_anCover;
	GUI::CAnchor        m_anROI;
	GUI::CAnchor        m_anCamPara;
	GUI::CAnchor		m_anExit;

	CCfgLiveManDlg		m_cfgLiveManDlg;

};


#endif
