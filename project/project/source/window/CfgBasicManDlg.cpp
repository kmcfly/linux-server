/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : L
** Date         : 2009-08-11
** Name         : CfgBasicManDlg.cpp
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/

#include "CfgBasicManDlg.h"
#include "FuncCustom.h"

//////////////////////////////////////////////////////////////////////////
CCfgBasicManDlg::CCfgBasicManDlg() : m_curPage(PAGE_SYSTEM),
m_initAnchorPage(PAGE_SYSTEM),
m_initPageMask(0)
{

}

CCfgBasicManDlg::~CCfgBasicManDlg()
{

}

//////////////////////////////////////////////////////////////////////////


void CCfgBasicManDlg::OnInitial()
{
	CCfgDlg::OnInitial();
	SetImage(BMP_MEM_DLG_LARGE, false);
	SetDefaultBtn( DEFAULT_BTN_APPLY | DEFAULT_BTN_OK | DEFAULT_BTN_CANCEL );
	SetDefaultBtnCaption( DEFAULT_BTN_CANCEL, m_pStrTable->String(E_STR_ID_EXIT));
	SetDefaultBtnCaption( DEFAULT_BTN_OK,m_pStrTable->String(E_STR_ID_APPLY) );
	SetDefaultBtnCaption( DEFAULT_BTN_APPLY,m_pStrTable->String(E_STR_ID_DEFAULT) );
	//////////////////////////////////////////////////////////////////////////

	m_initPageMask = 0x1;

#ifndef __DVR_ANCHOR__
	SetCaption(m_pStrTable->String(E_STR_ID_TITLE_BASIC), false);
	////

	unsigned short x = m_length[DLG_OFFSET_LEFT];
	unsigned short y = m_length[TITLE_HEIGHT] + m_length[TABLE_HEIGHT] + m_length[DLG_OFFSET_TOP];
	unsigned short offset = m_length[DLG_OFFSET_BOTTOM];
	unsigned short cx = m_rect.Width() - 2 * m_length[DLG_OFFSET_LEFT];
	unsigned short cy = m_rect.Height() - y- offset - m_length[DEFAULT_BTN_BOTTOM_YPOS];


	m_cfgSystemDlg.Create(GetRandWndID(), x, y, cx, cy, this, 0, 2);
//	m_cfgDateTimeDlg.Create(GetRandWndID(), x, y, cx, cy, this, 0, 3);
//
//	m_cfgDaylightDlg.Create(GetRandWndID(),x,y,cx,cy,this,0,4);
//
//#ifdef __MOBILE_DVR__
//	m_cfgDriverInfoDlg.Create(GetRandWndID(),x,y,cx,cy,this,0,5);
//#endif

	m_tabCtrl.Create(GetRandWndID(), x, m_length[TITLE_HEIGHT] + m_length[DLG_OFFSET_TOP], cx, m_length[TABLE_HEIGHT], this, 0, 1);
	m_tabCtrl.AddItem(&m_cfgSystemDlg, m_pStrTable->String( E_STR_ID_SYSTEM ) );
	m_tabCtrl.AddItem(&m_cfgDateTimeDlg, m_pStrTable->String( E_STR_ID_DATE_AND_TIME ) );
#if !defined(__TW01_RILI__)
	m_tabCtrl.AddItem(&m_cfgDaylightDlg,m_pStrTable->String(E_STR_ID_DST));
#endif

#ifdef __MOBILE_DVR__ 
	m_tabCtrl.AddItem(&m_cfgDriverInfoDlg,m_pStrTable->String(E_STR_ID_DRIVERINFO));
#endif

	m_curPage = PAGE_SYSTEM;
	m_tabCtrl.SetCurItem(m_curPage , false );
	CCfgDlg::SetRefreshView( false );
	/////////////////////////////////
#else
	////

	unsigned short x = m_length[DLG_OFFSET_LEFT];
	unsigned short y = m_length[TITLE_HEIGHT] + m_length[DLG_OFFSET_TOP];
	unsigned short offset = m_length[DLG_OFFSET_BOTTOM];
	unsigned short cx = m_rect.Width() - 2 * m_length[DLG_OFFSET_LEFT];
	unsigned short cy = m_rect.Height() - y - offset - m_length[DEFAULT_BTN_BOTTOM_YPOS];

	switch (m_initAnchorPage)
	{
	case PAGE_SYSTEM:
		SetCaption(m_pStrTable->String(E_STR_ID_TITLE_SYSTEM), false);
		m_cfgSystemDlg.Create(GetRandWndID(), x, y, cx, cy, this, 0, 1);
		break;
	case PAGE_TIME:
		SetCaption(m_pStrTable->String(E_STR_ID_TITLE_DATE_AND_TIME), false);
		m_cfgDateTimeDlg.Create(GetRandWndID(), x, y, cx, cy, this, 0, 1);
		break;
#if !defined(__TW01_RILI__)
	case PAGE_DST:
		SetCaption(m_pStrTable->String(E_STR_ID_DST), false);
		m_cfgDaylightDlg.Create(GetRandWndID(), x, y, cx, cy, this, 0, 1);
		break;
#endif
#ifdef __MOBILE_DVR__
	case PAGE_DRIVER:
		SetCaption(m_pStrTable->String(E_STR_ID_TITLE_DRIVERINFO), false);
		m_cfgDriverInfoDlg.Create(GetRandWndID(), x, y, cx, cy, this, 0, 1);
		break;
#endif
	default:
		assert(false);
		break;
	}

	m_curPage = m_initAnchorPage;
	CCfgDlg::SetRefreshView( false );
#endif

	CreateTipWnd();
}

void CCfgBasicManDlg::OnDestroy()
{
	CCfgDlg::OnDestroy();
}

unsigned long CCfgBasicManDlg::OnApply()
{
	//Load default
	CCfgDlg* pCfgDlg = GetPage(m_curPage);
	assert(pCfgDlg);
	unsigned long ret = OnLoadDefault(pCfgDlg);

	if( pCfgDlg->IsModify() )
	{
		EnableOkBtn(true,false);
	}
	else
	{
		EnableOkBtn(false,false);
	}

	ENUM_STRING_ID strID = ret ? static_cast<ENUM_STRING_ID>(ret) : E_STR_ID_TIP_00_03;
	MessageOut(strID,TIP_MSG,false);

	//一次重绘全部
	Repaint();

	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CCfgBasicManDlg::CheckValue()
{
	CCfgDlg* pCfgDlg = GetPage(m_curPage);
	assert(pCfgDlg);

	return pCfgDlg->CheckValue();
}

void CCfgBasicManDlg::GetItemList(CFGITEM_LIST& itemList) const
{
	itemList.push_back(NCFG_ITEM_DEVICE_ID);
	itemList.push_back(NCFG_ITEM_DEVICE_NAME);
	itemList.push_back(NCFG_ITEM_VIDEO_FORMAT);
	itemList.push_back(NCFG_ITEM_AUTHORIZATION);
	itemList.push_back(NCFG_ITEM_DATE_FORMAT);
	itemList.push_back(NCFG_ITEM_HOUR_FORMAT);
	itemList.push_back(NCFG_ITEM_LANGUAGE);
	itemList.push_back(NCFG_ITEM_TIME_ZONE);
#ifdef __DVR_BASIC__
	itemList.push_back(NCFG_ITEM_TIME_SYNC);
	itemList.push_back(NCFG_ITEM_TIME_SERVER);
#endif
	itemList.push_back(NCFG_ITEM_LIVE_TIME_INFO);
	itemList.push_back(NCFG_ITEM_SCRENN_SAVE_TIME);
	itemList.push_back(NCFG_ITEM_VGA_RESOLUTION);
	itemList.push_back(NCFG_ITEM_DAYLIGHT_INFO);
	itemList.push_back(NCFG_ITEM_MAX_CLIENT_NUM);
	itemList.push_back(NCFG_ITEM_TRANSPARENCY);

#ifndef __CUSTOM_US02__ 
	if(IsSupportWzd())
	{
		itemList.push_back(NCFG_ITEM_SHOW_WIZZARD);
	}
#else
		itemList.push_back(NCFG_ITEM_SHOW_WIZZARD);
#endif

#ifdef __MOBILE_DVR__
	itemList.push_back(NCFG_ITEM_COMPANY_NAME);
	itemList.push_back(NCFG_ITEM_DRIVER_NAME);
	itemList.push_back(NCFG_ITEM_CAR_NUMBER);
	itemList.push_back(NCFG_ITEM_SHUT_DOWN_DELAY);
#endif
	itemList.push_back(NCFG_ITEM_CHANN_VISIBLE);

#if (defined(__CHIP3531__) || defined(__CHIPGM__) || defined(__CHIP3535__) || defined(__CHIP3521__) || defined(__CHIP3520A__) || defined(__CHIP3520D__)) && !defined(__DVR_ULTIMATE__)
	itemList.push_back(NCFG_ITEM_CVBS_VO_ADJUST);
#endif
	//.. read only
#if defined(__CHIP3520A__) || defined(__CHIP3520D__) || defined(__CHIP3521__)
	itemList.push_back(NCFG_ITEM_CVBS_TO_SPOT);
	itemList.push_back(NCFG_ITEM_ENCODE_960H);
#endif
	itemList.push_back(NCFG_ITEM_DEVICE_NAME_MAX_LEN);
	itemList.push_back(NCFG_ITEM_TIME_SERVER_MAX_LEN);
	itemList.push_back(NCFG_ITEM_SUPPORT_LANGUAGE);

}

CCfgDlg* CCfgBasicManDlg::GetPage(PAGE_ITEM item)
{
	CCfgDlg* pCfgDlg = NULL;
	switch (item)
	{
	case PAGE_SYSTEM:
		pCfgDlg = &m_cfgSystemDlg;
		break;
	case PAGE_TIME:
		pCfgDlg = &m_cfgDateTimeDlg;
		break;
#if !defined(__TW01_RILI__)
	case PAGE_DST:
		pCfgDlg = &m_cfgDaylightDlg;
		break;
#endif
#ifdef __MOBILE_DVR__
	case PAGE_DRIVER:
		pCfgDlg = &m_cfgDriverInfoDlg;
		break;
#endif
	default:
		assert(false);
		break;
	}
	return pCfgDlg;
}

void CCfgBasicManDlg::SetAnchorPage(PAGE_ITEM item)
{
	m_initAnchorPage = item;
}

BEGIN_MSG_MAP(CCfgBasicManDlg,CCfgDlg)
	ON_MSG(m_cfgSystemDlg.GetID(), KEY_VIRTUAL_ENTER, OnClickSystem)
	ON_MSG(m_cfgDateTimeDlg.GetID(), KEY_VIRTUAL_ENTER, OnClickDateTime)
#if !defined(__TW01_RILI__)
	ON_MSG(m_cfgDaylightDlg.GetID(),KEY_VIRTUAL_ENTER,OnClickDst)
#endif
#ifdef __MOBILE_DVR__
	ON_MSG(m_cfgDriverInfoDlg.GetID(),KEY_VIRTUAL_ENTER,OnDriverInfoDst)
#endif
END_MSG_MAP()

unsigned long CCfgBasicManDlg::OnPreCommand(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	if ((ctrlID == m_tabCtrl.GetID()) && (KEY_VIRTUAL_ENTER == keyID))
	{
		{
			unsigned short x = m_length[DLG_OFFSET_LEFT];
			unsigned short y = m_length[TITLE_HEIGHT] + m_length[TABLE_HEIGHT] + m_length[DLG_OFFSET_TOP];
			unsigned short offset = m_length[DLG_OFFSET_BOTTOM];
			unsigned short cx = m_rect.Width() - 2 * m_length[DLG_OFFSET_LEFT];
			unsigned short cy = m_rect.Height() - y- offset - m_length[DEFAULT_BTN_BOTTOM_YPOS];

			if(!((m_initPageMask >> PAGE_TIME) & 0x1) && (PAGE_TIME == m_tabCtrl.GetFocusItem()))
			{
				m_cfgDateTimeDlg.Create(GUI::CWnd::GetRandWndID(),x,y,cx,cy, this,0, PAGE_TIME + 2);

				SetSubCfgDlg(&m_cfgDateTimeDlg);
				m_initPageMask |= (0x1 << PAGE_TIME);
			}

#if !defined(__TW01_RILI__)
			if(!((m_initPageMask >> PAGE_DST) & 0x1) && (PAGE_DST == m_tabCtrl.GetFocusItem()))
			{
				m_cfgDaylightDlg.Create(GUI::CWnd::GetRandWndID(),x,y,cx,cy, this,0, PAGE_DST + 2);

				SetSubCfgDlg(&m_cfgDaylightDlg);
				m_initPageMask |= (0x1 << PAGE_DST);
			}
#endif
		}

		if (m_curPage != m_tabCtrl.GetFocusItem())
		{
			unsigned long ret = 0;
			if (GetPage(m_curPage)->IsModify())
			{
				ret = TrySaveChange(GetPage(m_curPage),true,false,false);
			}

			if ( ret == 0 )
			{
				m_curPage = static_cast<PAGE_ITEM>(m_tabCtrl.GetFocusItem());
			}
			else
			{
				Repaint();

				return KEY_VIRTUAL_MSG_STOP;
			}
		}	
	}

	return keyID;
}

unsigned long CCfgBasicManDlg::OnClickSystem()
{
	if (m_cfgSystemDlg.IsModify())
	{
		EnableOkBtn(true, true);
	}

	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CCfgBasicManDlg::OnClickDateTime()
{
	if (m_cfgDateTimeDlg.IsModify())
	{
		EnableOkBtn(true, true);
	}

	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CCfgBasicManDlg::OnClickDst()
{	
#if !defined(__TW01_RILI__)
	if( m_cfgDaylightDlg.IsModify() )
	{
		EnableOkBtn(true,true);
	}
#endif
	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CCfgBasicManDlg::OnOK()
{
	unsigned long uTemp = CCfgDlg::OnOK();
	if (PAGE_SYSTEM == m_curPage)
	{
		m_cfgSystemDlg.OnOK();
	}
	return uTemp;
}

#ifdef __MOBILE_DVR__
unsigned long CCfgBasicManDlg::OnDriverInfoDst()
{
	if( m_cfgDriverInfoDlg.IsModify() )
	{
		EnableOkBtn(true,true);
	}

	return KEY_VIRTUAL_MSG_STOP;
}
#endif



//////////////////////////////////////////////////////////////////////////
// __DVR_ANCHOR__
//////////////////////////////////////////////////////////////////////////

CCfgBasicManAnchorDlg::CCfgBasicManAnchorDlg()
{

}

CCfgBasicManAnchorDlg::~CCfgBasicManAnchorDlg()
{

}

void CCfgBasicManAnchorDlg::OnInitial()
{
	GUI::CDialog::OnInitial();
	SetImage(BMP_MEM_DLG_NORMAL, false);
	SetCaption(m_pStrTable->String(E_STR_ID_TITLE_BASIC), false);
	//////////////////////////////////////////////////////////////////////////
	unsigned short x = 200;
	unsigned short y = m_length[TITLE_HEIGHT] + m_length[DLG_OFFSET_TOP] + 25;
	unsigned short x_off = 60, y_off = 10;
	unsigned short cx = m_length[ANCHOR_WIDTH], cy = m_length[ANCHOR_HEIGHT];

	int yFocus = 1;

	m_anSystem.Create(GUI::CWnd::GetRandWndID(), x, y, cx, cy, this, 0, yFocus++, m_pStrTable->String(E_STR_ID_SYSTEM));
	m_anSystem.SetImage(BMP_ANCHOR_SYSTEM, BMP_ANCHOR_SYSTEM_F, BMP_ANCHOR_SYSTEM);

	y += 60;
	m_anDateTime.Create(GUI::CWnd::GetRandWndID(), x, y, cx, cy, this, 0, yFocus++, m_pStrTable->String(E_STR_ID_DATE_AND_TIME));
	m_anDateTime.SetImage(BMP_ANCHOR_DATE_TIME, BMP_ANCHOR_DATE_TIME_F, BMP_ANCHOR_DATE_TIME);

#if !defined(__TW01_RILI__)
	y += 60;
	m_anDaylight.Create(GUI::CWnd::GetRandWndID(), x, y, cx, cy, this, 0, yFocus++, m_pStrTable->String(E_STR_ID_DST));
	m_anDaylight.SetImage(BMP_ANCHOR_DST, BMP_ANCHOR_DST_F, BMP_ANCHOR_DST);
#endif

#ifdef __MOBILE_DVR__
	y += 60;
	m_anDriverInfo.Create(GUI::CWnd::GetRandWndID(), x, y, cx, cy, this, 0, yFocus++, m_pStrTable->String(E_STR_ID_DRIVERINFO));
	m_anDriverInfo.SetImage(BMP_ANCHOR_CONFIG, BMP_ANCHOR_CONFIG_F, BMP_ANCHOR_CONFIG);
#endif

	y += 60;
	m_anExit.Create(GUI::CWnd::GetRandWndID(), x, y, cx, cy, this, 0, yFocus++, m_pStrTable->String(E_STR_ID_EXIT));
	m_anExit.SetImage(BMP_ANCHOR_EXIT, BMP_ANCHOR_EXIT_F, BMP_ANCHOR_EXIT);
}

BEGIN_MSG_MAP(CCfgBasicManAnchorDlg, GUI::CDialog)
	ON_MSG(m_anSystem.GetID(), KEY_VIRTUAL_ENTER, OnAnchorSystem)
	ON_MSG(m_anDateTime.GetID(), KEY_VIRTUAL_ENTER, OnAnchorDateTime)
#if !defined(__TW01_RILI__)
	ON_MSG(m_anDaylight.GetID(), KEY_VIRTUAL_ENTER, OnAnchorDaylight)
#endif
	ON_MSG(m_anDriverInfo.GetID(), KEY_VIRTUAL_ENTER, OnAnchorDriverInfo)
	ON_MSG(m_anExit.GetID(), KEY_VIRTUAL_ENTER, OnExit)
END_MSG_MAP()

unsigned long CCfgBasicManAnchorDlg::OnAnchorSystem()
{
	m_cfgBasicManDlg.SetAnchorPage(CCfgBasicManDlg::PAGE_SYSTEM);
	m_cfgBasicManDlg.DoModal(this, CENTER_POS, CENTER_POS, MAX_SCREEN_X, MAX_SCREEN_Y, false);

	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CCfgBasicManAnchorDlg::OnAnchorDateTime()
{
	m_cfgBasicManDlg.SetAnchorPage(CCfgBasicManDlg::PAGE_TIME);
	m_cfgBasicManDlg.DoModal(this, CENTER_POS, CENTER_POS, MAX_SCREEN_X, MAX_SCREEN_Y, false);

	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CCfgBasicManAnchorDlg::OnAnchorDaylight()
{
#if !defined(__TW01_RILI__)
	m_cfgBasicManDlg.SetAnchorPage(CCfgBasicManDlg::PAGE_DST);
	m_cfgBasicManDlg.DoModal(this, CENTER_POS, CENTER_POS, MAX_SCREEN_X, MAX_SCREEN_Y, false);
#endif
	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CCfgBasicManAnchorDlg::OnAnchorDriverInfo()
{
#ifdef __MOBILE_DVR__
	m_cfgBasicManDlg.SetAnchorPage(CCfgBasicManDlg::PAGE_DRIVER);
	m_cfgBasicManDlg.DoModal(this, CENTER_POS, CENTER_POS, MAX_SCREEN_X, MAX_SCREEN_Y, false);
#endif

	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CCfgBasicManAnchorDlg::OnExit()
{
	EndDialog(DLG_ESC);
	return KEY_VIRTUAL_MSG_STOP;
}

//end

