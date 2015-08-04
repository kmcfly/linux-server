#ifndef _CFG_GB28181_DLG_H_
#define _CFG_GB28181_DLG_H_

///---------------------------------------------------------------------
// Include
///---------------------------------------------------------------------
#include "CfgDlg.h"
#include "TabCtrl.h"
#include "Button.h"
#include "Anchor.h"

#include "CfgSystemDlg.h"
#include "CfgDateTimeDlg.h"
#include "CfgDaylightDlg.h"
#include "CfgSystemDlg.h"
#include "CfgSipServerDlg.h"

#include "CfgDvrIDDlg.h"
#include "CfgAlarmIDDlg.h"

using namespace GUI;

///---------------------------------------------------------------------
// Predefine
///---------------------------------------------------------------------

///---------------------------------------------------------------------
// Class Define
///---------------------------------------------------------------------
class CCfGb28281Dlg : public CCfgDlg
{
public:
	CCfGb28281Dlg();
	~CCfGb28281Dlg();

public:
	enum PAGE_ITEM
	{
		PAGE_SIPSERVER,
		PAGE_CHANNEL,
		PAGE_ALARM,
		PAGE_COUNT,
	};

	void SetAnchorPage(PAGE_ITEM item);

protected:
	virtual void OnInitial();			//初始化

	void OnDestroy();
	unsigned long OnApply();
	unsigned long CheckValue();

	void GetItemList(CFGITEM_LIST& itemList) const;
	CCfgDlg* GetPage(PAGE_ITEM item);

protected:
	DECLARE_MSG_MAP()
	unsigned long OnClickSipServer();
	unsigned long OnClickChannel();
	unsigned long OnClickAlarm();

protected:
	unsigned long OnPreCommand(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);
	unsigned long TrySaveChange(CCfgDlg* pCfgDlg, bool bChooseDlg, bool bHide,bool bRefresh);
protected:
	GUI::CTabCtrl				m_tabCtrl;
	CCfgSipServerDlg			m_cfgSipServerDlg;
	CCfgDvrIdDlg				m_cfgDvrIdDlg;
	CCfgAlarmIdDlg				m_cfgAlarmIdDlg;

	PAGE_ITEM					m_curPage;
	unsigned long				m_initPageMask;			//记录已经初始化的页面
	PAGE_ITEM					m_szPageItem[PAGE_COUNT];
	//
	PAGE_ITEM m_initAnchorPage;

	GUI::CAnchor		m_anNetwork;
};
#endif