/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : L
** Date         : 2009-08-15
** Name         : CfgSensorManDlg.cpp
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/

#include "CfgSensorManDlg.h"


CCfgSensorManDlg::CCfgSensorManDlg() : m_curPage(PAGE_SENSOR),
m_initAnchorPage(PAGE_SENSOR),
m_initPageMask(0)
{

}

CCfgSensorManDlg::~CCfgSensorManDlg()
{

}

void CCfgSensorManDlg::OnInitial()
{
	CCfgDlg::OnInitial();
	SetImage(BMP_MEM_DLG_LARGE, false);
	SetDefaultBtn( DEFAULT_BTN_APPLY | DEFAULT_BTN_OK | DEFAULT_BTN_CANCEL);
	SetDefaultBtnCaption( DEFAULT_BTN_CANCEL, m_pStrTable->String(E_STR_ID_EXIT) );
	SetDefaultBtnCaption( DEFAULT_BTN_OK, m_pStrTable->String(E_STR_ID_APPLY) );
	SetDefaultBtnCaption( DEFAULT_BTN_APPLY, m_pStrTable->String(E_STR_ID_DEFAULT) );
	//////////////////////////////////////////////////////////////////////////

	m_initPageMask = 0x1;

#ifndef __DVR_ANCHOR__
	SetCaption(m_pStrTable->String(E_STR_ID_TITLE_SENSOR), false);
	unsigned short x = m_length[DLG_OFFSET_LEFT];
	unsigned short y = m_length[TITLE_HEIGHT] + m_length[TABLE_HEIGHT] + m_length[DLG_OFFSET_TOP];
	//unsigned short offset = 4;
	unsigned short cx = m_rect.Width() - 2 * m_length[DLG_OFFSET_LEFT];
	unsigned short cy = m_rect.Height() - y- m_length[DLG_OFFSET_BOTTOM] - m_length[DEFAULT_BTN_BOTTOM_YPOS];

	m_cfgSensorDlg.Create(GUI::CWnd::GetRandWndID(), x, y, cx, cy, this, 0, 2);
	//m_cfgSensorHandingDlg.Create(GUI::CWnd::GetRandWndID(), x, y, cx, cy, this, 0, 3);
	//m_cfgSensorScheduleDlg.SetMaxChnn(s_loginSuccessReply.productInfo.sensorInputNum);
	//m_cfgSensorScheduleDlg.Create(GUI::CWnd::GetRandWndID(),x,y,cx,cy,this,0,4);

	//m_cfgSensorScheduleDlg.SetScheduleCfgID(NCFG_ITEM_SENSOR_IN_SCH_WEEK, NCFG_ITEM_SENSOR_IN_SCH_HOLIDAY);
	m_cfgSensorScheduleDlg.SetScheduleCfgID(NCFG_ITEM_SENSOR_IN_SCH_WEEK, NCFG_ITEM_SENSOR_IN_SCH_HOLIDAY);

	m_tabCtrl.Create(GUI::CWnd::GetRandWndID(), x, m_length[TITLE_HEIGHT]+m_length[DLG_OFFSET_TOP], cx, m_length[TABLE_HEIGHT], this, 0, 1);
	m_tabCtrl.AddItem(&m_cfgSensorDlg, m_pStrTable->String(E_STR_ID_BASIC));
	m_tabCtrl.AddItem(&m_cfgSensorHandingDlg, m_pStrTable->String(E_STR_ID_ALARM_HANDING));
	m_tabCtrl.AddItem(&m_cfgSensorScheduleDlg,m_pStrTable->String(E_STR_ID_SCHEDULE));

	m_curPage = PAGE_SENSOR;
	m_tabCtrl.SetCurItem( m_curPage , false );
	CCfgDlg::SetRefreshView( false );

#else

	unsigned short x = m_length[DLG_OFFSET_LEFT];
	unsigned short y = m_length[TITLE_HEIGHT] + m_length[DLG_OFFSET_TOP];
	unsigned short offset = m_length[DLG_OFFSET_BOTTOM];
	unsigned short cx = m_rect.Width() - 2 * m_length[DLG_OFFSET_LEFT];
	unsigned short cy = m_rect.Height() - y - offset - m_length[DEFAULT_BTN_BOTTOM_YPOS];

	switch (m_initAnchorPage)
	{
	case PAGE_SENSOR:
		SetCaption(m_pStrTable->String(E_STR_ID_TITLE_SENSOR), false);
		m_cfgSensorDlg.Create(GetRandWndID(), x, y, cx, cy, this, 0, 1);
		break;
	case PAGE_HANDING:
		SetCaption(m_pStrTable->String(E_STR_ID_TITLE_ALARM_HANDING), false);
		m_cfgSensorHandingDlg.Create(GetRandWndID(), x, y, cx, cy, this, 0, 1);
		break;
	case PAGE_SCHEDULE:
		SetCaption(m_pStrTable->String(E_STR_ID_TITLE_SCHEDULE), false);
		if (0 < s_loginSuccessReply.productInfo.netVideoInputNum)
		{
			//NVR版本
			m_cfgSensorScheduleDlg.SetMaxChnn(s_loginSuccessReply.productInfo.sensorInputNum+s_loginSuccessReply.productInfo.netVideoInputNum, 0);
		}
		else
		{
			//DVR版本
			m_cfgSensorScheduleDlg.SetMaxChnn(s_loginSuccessReply.productInfo.sensorInputNum,s_loginSuccessReply.productInfo.sensorInputNum);
		}

		m_cfgSensorScheduleDlg.SetScheduleCfgID(NCFG_ITEM_SENSOR_IN_SCH_WEEK, NCFG_ITEM_SENSOR_IN_SCH_HOLIDAY);
		m_cfgSensorScheduleDlg.Create(GetRandWndID(), x, y, cx, cy, this, 0, 1);
		break;
	default:
		assert(false);
		break;
	}

	m_curPage = m_initAnchorPage;
	CCfgDlg::SetRefreshView( false );

#endif

	//create tip
	CreateTipWnd();
}

void CCfgSensorManDlg::GetItemList(CFGITEM_LIST& itemList) const
{
	itemList.push_back(NCFG_ITEM_SENSOR_IN_SCH_WEEK);

#ifdef __DVR_BASIC__
	itemList.push_back(NCFG_ITEM_EMAIL_RECV_INFO);
#endif
	itemList.push_back(NCFG_ITEM_SENSOR_IN_ENABLE);
	itemList.push_back(NCFG_ITEM_SENSOR_IN_TYPE);
	itemList.push_back(NCFG_ITEM_SENSOR_IN_NAME);

	itemList.push_back(NCFG_ITEM_SENSOR_IN_HOLD_TIME);
	itemList.push_back(NCFG_ITEM_SENSOR_IN_BUZZ);
	itemList.push_back(NCFG_ITEM_SENSOR_IN_TO_ALARM_OUT);
	itemList.push_back(NCFG_ITEM_SENSOR_IN_TO_RECORD);
	itemList.push_back(NCFG_ITEM_SENSOR_TO_BIG_VIEW);
#ifdef __DVR_BASIC__	
	itemList.push_back(NCFG_ITEM_SENSOR_IN_TO_EMAIL_ENABLE);
	itemList.push_back(NCFG_ITEM_SENSOR_TO_EMAIL_ADDR);
#endif

#ifdef __ALARM_TO_UPDATE_FTP_VIDEO__
	itemList.push_back(NCFG_ITEM_SENSOR_IN_TO_FTP_ENABLE);
#endif

	if(s_loginSuccessReply.productInfo.videoInputNum > 0)   // localVideoInputNum  改成  videoInputNum    xian 
	{
		itemList.push_back(NCFG_ITEM_SENSOR_IN_TO_PTZ);
	}
	itemList.push_back(NCFG_ITEM_SENSOR_IN_TO_SNAP);

	//read 
	itemList.push_back(NCFG_ITEM_SENSOR_IN_NAME_MAX_LEN);
}

unsigned long CCfgSensorManDlg::OnApply()
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

unsigned long CCfgSensorManDlg::CheckValue()
{
	CCfgDlg* pCfgDlg = GetPage(m_curPage);
	assert(pCfgDlg);

	return pCfgDlg->CheckValue();
}

CCfgDlg* CCfgSensorManDlg::GetPage(PAGE_ITEM item)
{
	CCfgDlg* pCfgDlg = NULL;
	switch (item)
	{
	case PAGE_SENSOR:
		pCfgDlg = &m_cfgSensorDlg;
		break;
	case PAGE_HANDING:
		pCfgDlg = &m_cfgSensorHandingDlg;
		break;
	case PAGE_SCHEDULE:
		pCfgDlg = &m_cfgSensorScheduleDlg;
		break;
	default:
		assert(false);
		break;
	}
	return pCfgDlg;
}

void CCfgSensorManDlg::SetAnchorPage(PAGE_ITEM item)
{
	m_initAnchorPage = item;
}

BEGIN_MSG_MAP(CCfgSensorManDlg,CCfgDlg)
	ON_MSG(m_cfgSensorDlg.GetID(), KEY_VIRTUAL_ENTER, OnClickSensor)
	ON_MSG(m_cfgSensorScheduleDlg.GetID(),KEY_VIRTUAL_ENTER,OnClickSchedule)
	ON_MSG(m_cfgSensorScheduleDlg.GetID(),KEY_VIRTUAL_DBCLICK,OnClickSchedule)
	ON_MSG(m_cfgSensorHandingDlg.GetID(), KEY_VIRTUAL_ENTER, OnClickHanding)
END_MSG_MAP()

unsigned long CCfgSensorManDlg::OnPreCommand(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	if ((ctrlID == m_tabCtrl.GetID()) && (KEY_VIRTUAL_ENTER == keyID))
	{
		{
			unsigned short x = m_length[DLG_OFFSET_LEFT];
			unsigned short y = m_length[TITLE_HEIGHT] + m_length[TABLE_HEIGHT] + m_length[DLG_OFFSET_TOP];
			//unsigned short offset = 4;
			unsigned short cx = m_rect.Width() - 2 * m_length[DLG_OFFSET_LEFT];
			unsigned short cy = m_rect.Height() - y- m_length[DLG_OFFSET_BOTTOM] - m_length[DEFAULT_BTN_BOTTOM_YPOS];

			if(!((m_initPageMask >> PAGE_HANDING) & 0x1) && (PAGE_HANDING == m_tabCtrl.GetFocusItem()))
			{
				m_cfgSensorHandingDlg.Create(GUI::CWnd::GetRandWndID(),x,y,cx,cy, this,0, PAGE_HANDING + 2);

				SetSubCfgDlg(&m_cfgSensorHandingDlg);
				m_initPageMask |= (0x1 << PAGE_HANDING);
			}

			if(!((m_initPageMask >> PAGE_SCHEDULE) & 0x1) && (PAGE_SCHEDULE == m_tabCtrl.GetFocusItem()))
			{
				if (0 < s_loginSuccessReply.productInfo.netVideoInputNum)
				{
					m_cfgSensorScheduleDlg.SetMaxChnn(s_loginSuccessReply.productInfo.sensorInputNum + s_loginSuccessReply.productInfo.netVideoInputNum, 0);
				}
				else
				{
					m_cfgSensorScheduleDlg.SetMaxChnn(s_loginSuccessReply.productInfo.sensorInputNum+s_loginSuccessReply.productInfo.netVideoInputNum, s_loginSuccessReply.productInfo.sensorInputNum);
				}
				m_cfgSensorScheduleDlg.Create(GUI::CWnd::GetRandWndID(),x,y,cx,cy, this,0, PAGE_SCHEDULE + 2);

				SetSubCfgDlg(&m_cfgSensorScheduleDlg);
				m_initPageMask |= (0x1 << PAGE_SCHEDULE);
			}
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

unsigned long CCfgSensorManDlg::OnClickSensor()
{
	if (m_cfgSensorDlg.IsModify())
	{
		EnableOkBtn(true, true);
	}

	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CCfgSensorManDlg::OnClickHanding()
{
	if (m_cfgSensorHandingDlg.IsModify())
	{
		EnableOkBtn(true, true);
	}

	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CCfgSensorManDlg::OnClickSchedule()
{
	if( m_cfgSensorScheduleDlg.IsModify() )
	{
		EnableOkBtn(true,true);
	}

	return KEY_VIRTUAL_MSG_STOP;
}


//////////////////////////////////////////////////////////////////////////
// __DVR_ANCHOR__
//////////////////////////////////////////////////////////////////////////

CCfgSensorAnchorDlg::CCfgSensorAnchorDlg()
{

}

CCfgSensorAnchorDlg::~CCfgSensorAnchorDlg()
{

}

void CCfgSensorAnchorDlg::OnInitial()
{
	GUI::CDialog::OnInitial();
	SetImage(BMP_MEM_DLG_NORMAL, false);
	SetCaption(m_pStrTable->String(E_STR_ID_TITLE_SENSOR), false);
	//////////////////////////////////////////////////////////////////////////
	unsigned short x = 200;
	unsigned short y = m_length[TITLE_HEIGHT] + m_length[DLG_OFFSET_TOP] + 25;
	unsigned short x_off = 60, y_off = 10;
	unsigned short cx = m_length[ANCHOR_WIDTH], cy = m_length[ANCHOR_HEIGHT];

	int yFocus = 1;

	m_anSensor.Create(GUI::CWnd::GetRandWndID(), x, y, cx, cy, this, 0, yFocus++, m_pStrTable->String(E_STR_ID_SENSOR));
	m_anSensor.SetImage(BMP_ANCHOR_SENSOR, BMP_ANCHOR_SENSOR_F, BMP_ANCHOR_SENSOR);

	y += 60;
	m_anHanding.Create(GUI::CWnd::GetRandWndID(), x, y, cx, cy, this, 0, yFocus++, m_pStrTable->String(E_STR_ID_ALARM_HANDING));
	m_anHanding.SetImage(BMP_ANCHOR_ALARM_HANDLING, BMP_ANCHOR_ALARM_HANDLING_F, BMP_ANCHOR_ALARM_HANDLING);

	y += 60;
	m_anSchedule.Create(GUI::CWnd::GetRandWndID(), x, y, cx, cy, this, 0, yFocus++, m_pStrTable->String(E_STR_ID_SCHEDULE));
	m_anSchedule.SetImage(BMP_ANCHOR_SCHEDULE, BMP_ANCHOR_SCHEDULE_F, BMP_ANCHOR_SCHEDULE);

	y += 60;
	m_anExit.Create(GUI::CWnd::GetRandWndID(), x, y, cx, cy, this, 0, yFocus++, m_pStrTable->String(E_STR_ID_EXIT));
	m_anExit.SetImage(BMP_ANCHOR_EXIT, BMP_ANCHOR_EXIT_F, BMP_ANCHOR_EXIT);
}

BEGIN_MSG_MAP(CCfgSensorAnchorDlg, GUI::CDialog)
	ON_MSG(m_anSensor.GetID(), KEY_VIRTUAL_ENTER, OnAnchorSensor)
	ON_MSG(m_anHanding.GetID(), KEY_VIRTUAL_ENTER, OnAnchorHanding)
	ON_MSG(m_anSchedule.GetID(), KEY_VIRTUAL_ENTER, OnAnchorSchedule)
	ON_MSG(m_anExit.GetID(), KEY_VIRTUAL_ENTER, OnExit)
END_MSG_MAP()

unsigned long CCfgSensorAnchorDlg::OnAnchorSensor()
{
	m_cfgSensorManDlg.SetAnchorPage(CCfgSensorManDlg::PAGE_SENSOR);
	m_cfgSensorManDlg.DoModal(this, CENTER_POS, CENTER_POS, MAX_SCREEN_X, MAX_SCREEN_Y, false);

	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CCfgSensorAnchorDlg::OnAnchorHanding()
{
	m_cfgSensorManDlg.SetAnchorPage(CCfgSensorManDlg::PAGE_HANDING);
	m_cfgSensorManDlg.DoModal(this, CENTER_POS, CENTER_POS, MAX_SCREEN_X, MAX_SCREEN_Y, false);

	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CCfgSensorAnchorDlg::OnAnchorSchedule()
{
	m_cfgSensorManDlg.SetAnchorPage(CCfgSensorManDlg::PAGE_SCHEDULE);
	m_cfgSensorManDlg.DoModal(this, CENTER_POS, CENTER_POS, MAX_SCREEN_X, MAX_SCREEN_Y, false);

	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CCfgSensorAnchorDlg::OnExit()
{
	EndDialog(DLG_ESC);
	return KEY_VIRTUAL_MSG_STOP;
}

//end
