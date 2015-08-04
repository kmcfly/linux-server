/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : hz
** Date         : 2009-08-12
** Name         : CfgScheduleDlg.cpp
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/

#include "CfgScheduleDlg.h"

#include "FuncCustom.h"

extern NET_LOGIN_SUCCESS_REPLY *g_p_login_succ;

CCfgScheduleDlg::CCfgScheduleDlg() : m_curPage(SCHEDULE_RECORD),
m_initAnchorPage(SCHEDULE_RECORD),
m_initPageMask(0)
{

}

CCfgScheduleDlg::~CCfgScheduleDlg()
{

}

void CCfgScheduleDlg::OnInitial()
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
	SetCaption(m_pStrTable->String(E_STR_ID_TITLE_SCHEDULE), false);
	unsigned short x = m_length[DLG_OFFSET_LEFT];
	unsigned short y = m_length[TITLE_HEIGHT] + m_length[TABLE_HEIGHT] + m_length[DLG_OFFSET_TOP];
	unsigned short cx = m_rect.Width() - 2 * m_length[DLG_OFFSET_LEFT];
	unsigned short cy = m_rect.Height() - y - m_length[DLG_OFFSET_BOTTOM] - m_length[DEFAULT_BTN_BOTTOM_YPOS];

	int maxChnn = s_loginSuccessReply.productInfo.videoInputNum;
	
	m_childDlg[0].SetScheduleChildType(SCHEDULE_CHILD_RECORD);
	m_childDlg[1].SetScheduleChildType(SCHEDULE_CHILD_MOTION);
	m_childDlg[2].SetScheduleChildType(SCHEDULE_CHILD_ALARM);

	for (int i = 0; i <= SCHEDULE_RECORD; ++i)
	{
		m_childDlg[i].SetMaxChnn(maxChnn, s_loginSuccessReply.productInfo.localVideoInputNum);
		m_childDlg[i].Create(GetRandWndID(), x, y, cx, cy, this, 0, i+2);	
	}

	m_childDlg[SCHEDULE_RECORD].SetScheduleCfgID(NCFG_ITEM_REC_SCH_SCHEDULE_WEEK, NCFG_ITEM_REC_SCH_SCHEDULE_HOLIDAY);
	m_childDlg[SCHEDULE_MOTION].SetScheduleCfgID(NCFG_ITEM_REC_SCH_MOTION_WEEK, NCFG_ITEM_REC_SCH_MOTION_HOLIDAY);
	m_childDlg[SCHEDULE_ALARM].SetScheduleCfgID(NCFG_ITEM_REC_SCH_SENSOR_WEEK, NCFG_ITEM_REC_SCH_SENSOR_HOLIDAY);
	
	m_tabCtrl.Create(GetRandWndID(), x, m_length[TITLE_HEIGHT] + m_length[DLG_OFFSET_TOP], cx, m_length[TABLE_HEIGHT], this, 0, 1);

	m_tabCtrl.AddItem(&m_childDlg[SCHEDULE_RECORD], m_pStrTable->String(E_STR_ID_REC_SCHEDULE));
	m_tabCtrl.AddItem(&m_childDlg[SCHEDULE_MOTION], m_pStrTable->String(E_STR_ID_REC_MOTION));
	if (IsSupportAlarmOut() || (g_p_login_succ->productInfo.netVideoInputNum > 0))
	{
		m_tabCtrl.AddItem(&m_childDlg[SCHEDULE_ALARM], m_pStrTable->String(E_STR_ID_REC_SENSOR));
	}

	m_tabCtrl.SetCurItem(SCHEDULE_RECORD, false);
	m_curPage = SCHEDULE_RECORD;
	CCfgDlg::SetRefreshView( false );

#else

	unsigned short x = m_length[DLG_OFFSET_LEFT];
	unsigned short y = m_length[TITLE_HEIGHT] + m_length[DLG_OFFSET_TOP];
	unsigned short offset = m_length[DLG_OFFSET_BOTTOM];
	unsigned short cx = m_rect.Width() - 2 * m_length[DLG_OFFSET_LEFT];
	unsigned short cy = m_rect.Height() - y - offset - m_length[DEFAULT_BTN_BOTTOM_YPOS];

	int maxChnn = s_loginSuccessReply.productInfo.videoInputNum;
	switch (m_initAnchorPage)
	{
	case SCHEDULE_RECORD:
		SetCaption(m_pStrTable->String(E_STR_ID_TITLE_SCHEDULE), false);
		m_childDlg[SCHEDULE_RECORD].SetMaxChnn(maxChnn, s_loginSuccessReply.productInfo.localVideoInputNum);
		m_childDlg[SCHEDULE_RECORD].Create(GetRandWndID(), x, y, cx, cy, this, 0, 1);
		m_childDlg[SCHEDULE_RECORD].SetScheduleCfgID(NCFG_ITEM_REC_SCH_SCHEDULE_WEEK, NCFG_ITEM_REC_SCH_SCHEDULE_HOLIDAY);
		break;
	case SCHEDULE_MOTION:
		SetCaption(m_pStrTable->String(E_STR_ID_TITLE_MOTION), false);
		m_childDlg[SCHEDULE_MOTION].SetMaxChnn(maxChnn,s_loginSuccessReply.productInfo.localVideoInputNum);
		m_childDlg[SCHEDULE_MOTION].Create(GetRandWndID(), x, y, cx, cy, this, 0, 1);
		m_childDlg[SCHEDULE_MOTION].SetScheduleCfgID(NCFG_ITEM_REC_SCH_MOTION_WEEK, NCFG_ITEM_REC_SCH_MOTION_HOLIDAY);
		break;
	case SCHEDULE_ALARM:
		SetCaption(m_pStrTable->String(E_STR_ID_TITLE_SENSOR), false);
		m_childDlg[SCHEDULE_ALARM].SetMaxChnn(maxChnn,s_loginSuccessReply.productInfo.sensorInputNum+s_loginSuccessReply.productInfo.netVideoInputNum);
		m_childDlg[SCHEDULE_ALARM].Create(GetRandWndID(), x, y, cx, cy, this, 0, 1);
		m_childDlg[SCHEDULE_ALARM].SetScheduleCfgID(NCFG_ITEM_REC_SCH_SENSOR_WEEK, NCFG_ITEM_REC_SCH_SENSOR_HOLIDAY);
		break;
	default:
		assert(false);
		break;
	}

	m_curPage = m_initAnchorPage;
	CCfgDlg::SetRefreshView( false );

#endif

	////
	CreateTipWnd();
}


void CCfgScheduleDlg::GetItemList(CFGITEM_LIST& itemList) const
{
	itemList.push_back(NCFG_ITEM_REC_SCH_SCHEDULE_WEEK);
	itemList.push_back(NCFG_ITEM_REC_SCH_MOTION_WEEK);
	itemList.push_back(NCFG_ITEM_REC_SCH_SENSOR_WEEK);
	//itemList.push_back(NCFG_ITEM_REC_SCH_SCHEDULE_HOLIDAY);
	//itemList.push_back(NCFG_ITEM_REC_SCH_MOTION_HOLIDAY);
	//itemList.push_back(NCFG_ITEM_REC_SCH_SENSOR_HOLIDAY);
}

unsigned long CCfgScheduleDlg::OnApply()
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

unsigned long CCfgScheduleDlg::CheckValue()
{
	CCfgDlg* pCfgDlg = GetPage(m_curPage);
	assert(pCfgDlg);

	return pCfgDlg->CheckValue();
}

CCfgDlg* CCfgScheduleDlg::GetPage(SCHEDULE_TYPE item)
{
	return &m_childDlg[item];
}

void CCfgScheduleDlg::SetAnchorPage(SCHEDULE_TYPE item)
{
	m_initAnchorPage = item;
}

BEGIN_MSG_MAP(CCfgScheduleDlg,CCfgDlg)
	ON_MSG(m_childDlg[SCHEDULE_RECORD].GetID(), KEY_VIRTUAL_ENTER, OnClickRecord)
	ON_MSG(m_childDlg[SCHEDULE_RECORD].GetID(), KEY_VIRTUAL_DBCLICK, OnClickRecord)
	ON_MSG(m_childDlg[SCHEDULE_MOTION].GetID(), KEY_VIRTUAL_ENTER, OnClickMotion)
	ON_MSG(m_childDlg[SCHEDULE_MOTION].GetID(), KEY_VIRTUAL_DBCLICK, OnClickMotion)
	ON_MSG(m_childDlg[SCHEDULE_ALARM].GetID(), KEY_VIRTUAL_ENTER, OnClickAlarm)
	ON_MSG(m_childDlg[SCHEDULE_ALARM].GetID(), KEY_VIRTUAL_DBCLICK, OnClickAlarm)
END_MSG_MAP()


unsigned long CCfgScheduleDlg::OnPreCommand(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	if ((ctrlID == m_tabCtrl.GetID()) && (KEY_VIRTUAL_ENTER == keyID))
	{
		{
			unsigned short x = m_length[DLG_OFFSET_LEFT];
			unsigned short y = m_length[TITLE_HEIGHT] + m_length[TABLE_HEIGHT] + m_length[DLG_OFFSET_TOP];
			unsigned short cx = m_rect.Width() - 2 * m_length[DLG_OFFSET_LEFT];
			unsigned short cy = m_rect.Height() - y - m_length[DLG_OFFSET_BOTTOM] - m_length[DEFAULT_BTN_BOTTOM_YPOS];

			int maxChnn = s_loginSuccessReply.productInfo.videoInputNum;
			int maxLocalChnn = s_loginSuccessReply.productInfo.localVideoInputNum;

			if(!((m_initPageMask >> SCHEDULE_MOTION) & 0x1) && (SCHEDULE_MOTION == m_tabCtrl.GetFocusItem()))
			{
				m_childDlg[SCHEDULE_MOTION].SetMaxChnn(maxChnn, maxLocalChnn);
				m_childDlg[SCHEDULE_MOTION].Create(GUI::CWnd::GetRandWndID(),x,y,cx,cy, this,0, SCHEDULE_MOTION + 2);

				SetSubCfgDlg(&m_childDlg[SCHEDULE_MOTION]);
				m_initPageMask |= (0x1 << SCHEDULE_MOTION);
			}

			if(!((m_initPageMask >> SCHEDULE_ALARM) & 0x1) && (SCHEDULE_ALARM == m_tabCtrl.GetFocusItem()))
			{
				m_childDlg[SCHEDULE_ALARM].SetMaxChnn(maxChnn, maxLocalChnn);
				m_childDlg[SCHEDULE_ALARM].Create(GUI::CWnd::GetRandWndID(),x,y,cx,cy, this,0, SCHEDULE_ALARM + 2);

				SetSubCfgDlg(&m_childDlg[SCHEDULE_ALARM]);
				m_initPageMask |= (0x1 << SCHEDULE_ALARM);
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
				m_curPage = static_cast<SCHEDULE_TYPE>(m_tabCtrl.GetFocusItem());
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

unsigned long CCfgScheduleDlg::OnClickRecord()
{
	if (m_childDlg[SCHEDULE_RECORD].IsModify())
	{
		EnableOkBtn(true, true);
	}

	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CCfgScheduleDlg::OnClickMotion()
{
	if (m_childDlg[SCHEDULE_MOTION].IsModify())
	{
		EnableOkBtn(true, true);
	}

	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CCfgScheduleDlg::OnClickAlarm()
{
	if (m_childDlg[SCHEDULE_ALARM].IsModify())
	{
		EnableOkBtn(true, true);
	}

	return KEY_VIRTUAL_MSG_STOP;
}



//////////////////////////////////////////////////////////////////////////
// __DVR_ANCHOR__
//////////////////////////////////////////////////////////////////////////

CCfgScheduleAnchorDlg::CCfgScheduleAnchorDlg()
{

}

CCfgScheduleAnchorDlg::~CCfgScheduleAnchorDlg()
{

}

void CCfgScheduleAnchorDlg::OnInitial()
{
	GUI::CDialog::OnInitial();
	SetImage(BMP_MEM_DLG_NORMAL, false);
	SetCaption(m_pStrTable->String(E_STR_ID_TITLE_SCHEDULE), false);
	//////////////////////////////////////////////////////////////////////////
	unsigned short x = 200;
	unsigned short y = m_length[TITLE_HEIGHT] + m_length[DLG_OFFSET_TOP] + 25;
	unsigned short x_off = 60, y_off = 10;
	unsigned short cx = m_length[ANCHOR_WIDTH], cy = m_length[ANCHOR_HEIGHT];

	int yFocus = 1;

	m_anRecord.Create(GUI::CWnd::GetRandWndID(), x, y, cx, cy, this, 0, yFocus++, m_pStrTable->String(E_STR_ID_REC_SCHEDULE));
	m_anRecord.SetImage(BMP_ANCHOR_SCHEDULE, BMP_ANCHOR_SCHEDULE_F, BMP_ANCHOR_SCHEDULE);

	y += 60;
	m_anMotion.Create(GUI::CWnd::GetRandWndID(), x, y, cx, cy, this, 0, yFocus++, m_pStrTable->String(E_STR_ID_REC_MOTION));
	m_anMotion.SetImage(BMP_ANCHOR_MOTION, BMP_ANCHOR_MOTION_F, BMP_ANCHOR_MOTION);

	if (IsSupportAlarmOut() || (g_p_login_succ->productInfo.netVideoInputNum > 0))
	{
		y += 60;
		m_anAlarm.Create(GUI::CWnd::GetRandWndID(), x, y, cx, cy, this, 0, yFocus++, m_pStrTable->String(E_STR_ID_REC_SENSOR));
		m_anAlarm.SetImage(BMP_ANCHOR_SENSOR, BMP_ANCHOR_SENSOR_F, BMP_ANCHOR_SENSOR);
	}
	
	y += 60;
	m_anExit.Create(GUI::CWnd::GetRandWndID(), x, y, cx, cy, this, 0, yFocus++, m_pStrTable->String(E_STR_ID_EXIT));
	m_anExit.SetImage(BMP_ANCHOR_EXIT, BMP_ANCHOR_EXIT_F, BMP_ANCHOR_EXIT);
}

BEGIN_MSG_MAP(CCfgScheduleAnchorDlg, GUI::CDialog)
	ON_MSG(m_anRecord.GetID(), KEY_VIRTUAL_ENTER, OnAnchorRecord)
	ON_MSG(m_anMotion.GetID(), KEY_VIRTUAL_ENTER, OnAnchorMotion)
	ON_MSG(m_anAlarm.GetID(), KEY_VIRTUAL_ENTER, OnAnchorAlarm)
	ON_MSG(m_anExit.GetID(), KEY_VIRTUAL_ENTER, OnExit)
END_MSG_MAP()

unsigned long CCfgScheduleAnchorDlg::OnAnchorRecord()
{
	m_cfgScheduleDlg.SetAnchorPage(CCfgScheduleDlg::SCHEDULE_RECORD);
	m_cfgScheduleDlg.DoModal(this, CENTER_POS, CENTER_POS, MAX_SCREEN_X, MAX_SCREEN_Y, false);

	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CCfgScheduleAnchorDlg::OnAnchorMotion()
{
	m_cfgScheduleDlg.SetAnchorPage(CCfgScheduleDlg::SCHEDULE_MOTION);
	m_cfgScheduleDlg.DoModal(this, CENTER_POS, CENTER_POS, MAX_SCREEN_X, MAX_SCREEN_Y, false);

	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CCfgScheduleAnchorDlg::OnAnchorAlarm()
{
	m_cfgScheduleDlg.SetAnchorPage(CCfgScheduleDlg::SCHEDULE_ALARM);
	m_cfgScheduleDlg.DoModal(this, CENTER_POS, CENTER_POS, MAX_SCREEN_X, MAX_SCREEN_Y, false);

	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CCfgScheduleAnchorDlg::OnExit()
{
	EndDialog(DLG_ESC);
	return KEY_VIRTUAL_MSG_STOP;
}

//end