
#include "CfgSpeedAlarmDlg.h"


CCfgSpeedAlarmDlg::CCfgSpeedAlarmDlg():m_curPage(PAGE_BASIC), m_initAnchorPage(PAGE_BASIC)
{

}

CCfgSpeedAlarmDlg::~CCfgSpeedAlarmDlg()
{

}

void CCfgSpeedAlarmDlg::OnInitial()
{
	CCfgDlg::OnInitial();
	SetImage(BMP_MEM_DLG_LARGE, false);
	SetDefaultBtn( DEFAULT_BTN_APPLY | DEFAULT_BTN_OK | DEFAULT_BTN_CANCEL );
	SetDefaultBtnCaption( DEFAULT_BTN_CANCEL, m_pStrTable->String(E_STR_ID_EXIT));
	SetDefaultBtnCaption( DEFAULT_BTN_OK,m_pStrTable->String(E_STR_ID_APPLY) );
	SetDefaultBtnCaption( DEFAULT_BTN_APPLY,m_pStrTable->String(E_STR_ID_DEFAULT) );
	//////////////////////////////////////////////////////////////////////////

#ifndef __DVR_ANCHOR__
	SetCaption(m_pStrTable->String(E_STR_ID_TITLE_ALARMSPEED), false);

	unsigned short x = m_length[DLG_OFFSET_LEFT];
	unsigned short y = m_length[TITLE_HEIGHT] + m_length[TABLE_HEIGHT] + m_length[DLG_OFFSET_TOP];
	unsigned short offset = m_length[DLG_OFFSET_BOTTOM];
	unsigned short cx = m_rect.Width() - 2 * m_length[DLG_OFFSET_LEFT];
	unsigned short cy = m_rect.Height() - y- offset - m_length[DEFAULT_BTN_BOTTOM_YPOS];


	m_cfgSpeedAlmBasicDlg.Create(GetRandWndID(), x, y, cx, cy, this, 0, 1);
	m_cfgSpeedAlmOutputDlg.Create(GetRandWndID(), x, y, cx, cy, this, 0, 2);


	m_tabCtrl.Create(GetRandWndID(), x, m_length[TITLE_HEIGHT] + m_length[DLG_OFFSET_TOP], cx, m_length[TABLE_HEIGHT], this, 0, 1);
	m_tabCtrl.AddItem(&m_cfgSpeedAlmBasicDlg, m_pStrTable->String( E_STR_ID_BASIC ) );
	m_tabCtrl.AddItem(&m_cfgSpeedAlmOutputDlg, m_pStrTable->String( E_STR_ID_ALARM_HANDING ) );

	m_curPage = PAGE_BASIC;
	m_tabCtrl.SetCurItem(m_curPage , false );
	CCfgDlg::SetRefreshView( false );
	/////////////////////////////////
#else
	unsigned short x = m_length[DLG_OFFSET_LEFT];
	unsigned short y = m_length[TITLE_HEIGHT] + m_length[DLG_OFFSET_TOP];
	unsigned short offset = m_length[DLG_OFFSET_BOTTOM];
	unsigned short cx = m_rect.Width() - 2 * m_length[DLG_OFFSET_LEFT];
	unsigned short cy = m_rect.Height() - y - offset - m_length[DEFAULT_BTN_BOTTOM_YPOS];

	switch (m_initAnchorPage)
	{
	case PAGE_BASIC:
		SetCaption(m_pStrTable->String(E_STR_ID_TITLE_ALARMSPEED), false);
		m_cfgSpeedAlmBasicDlg.Create(GetRandWndID(), x, y, cx, cy, this, 0, 1);
		break;
	case PAGE_OUTPUT:
		SetCaption(m_pStrTable->String(E_STR_ID_TITLE_ALARM_HANDING), false);
		m_cfgSpeedAlmOutputDlg.Create(GetRandWndID(), x, y, cx, cy, this, 0, 1);
		break;
	default:
		assert(false);
		break;
	}

	m_curPage = m_initAnchorPage;
	CCfgDlg::SetRefreshView( false );
#endif

	CreateTipWnd();

}

void CCfgSpeedAlarmDlg::OnDestroy()
{
	CCfgDlg::OnDestroy();
}

unsigned long CCfgSpeedAlarmDlg::CheckValue()
{
	CCfgDlg* pCfgDlg = GetPage(m_curPage);
	assert(pCfgDlg);

	return pCfgDlg->CheckValue();
}

unsigned long CCfgSpeedAlarmDlg::OnApply()
{
	//Load default
	CCfgDlg* pCfgDlg = GetPage(m_curPage);
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
void CCfgSpeedAlarmDlg::GetItemList(CFGITEM_LIST& itemList) const
{
#ifdef __DVR_BASIC__
	itemList.push_back(NCFG_ITEM_EMAIL_RECV_INFO);
#endif
	itemList.push_back(NCFG_ITEM_SPEED_SOURCE);
	itemList.push_back(NCFG_ITEM_SPEED_UNIT);
	itemList.push_back(NCFG_ITEM_HIGH_SPEED_ENABLE);
	itemList.push_back(NCFG_ITEM_HIGH_SPEED_VALUE);
	itemList.push_back(NCFG_ITEM_HIGH_SPEED_HOLD_TIME);
	itemList.push_back(NCFG_ITEM_HIGH_SPEED_BUZZ);
	itemList.push_back(NCFG_ITEM_HIGH_SPEED_TO_ALARM_OUT);
	itemList.push_back(NCFG_ITEM_HIGH_SPEED_TO_RECORD);
	itemList.push_back(NCFG_ITEM_HIGH_SPEED_BIG_VIEW);
#ifdef __DVR_BASIC__	
	itemList.push_back(NCFG_ITEM_HIGH_SPEED_TO_EMAIL_ENABLE);
	itemList.push_back(NCFG_ITEM_HIGH_SPEED_TO_EMAIL_ADDR);
#endif
	itemList.push_back(NCFG_ITEM_HIGH_SPEED_TO_PTZ);
	itemList.push_back(NCFG_ITEM_LOW_SPEED_ENABLE);
	itemList.push_back(NCFG_ITEM_LOW_SPEED_VALUE);
	itemList.push_back(NCFG_ITEM_LOW_SPEED_HOLD_TIME);
	itemList.push_back(NCFG_ITEM_LOW_SPEED_BUZZ);
	itemList.push_back(NCFG_ITEM_LOW_SPEED_TO_ALARM_OUT);
	itemList.push_back(NCFG_ITEM_LOW_SPEED_TO_RECORD);
	itemList.push_back(NCFG_ITEM_LOW_SPEED_BIG_VIEW);
#ifdef __DVR_BASIC__	
	itemList.push_back(NCFG_ITEM_LOW_SPEED_TO_EMAIL_ENABLE);
	itemList.push_back(NCFG_ITEM_LOW_SPEED_TO_EMAIL_ADDR);
#endif
	itemList.push_back(NCFG_ITEM_LOW_SPEED_TO_PTZ);

	//.. read only
	itemList.push_back(NCFG_ITEM_SPEED_VALUE_MUX_LEN);
	//itemList.push_back(NCFG_ITEM_SUPPORT_LANGUAGE);
}


CCfgDlg* CCfgSpeedAlarmDlg::GetPage(PAGE_ITEM item)
{
	CCfgDlg* pCfgDlg = NULL;
	switch (item)
	{
	case PAGE_BASIC:
		pCfgDlg = &m_cfgSpeedAlmBasicDlg;
		break;
	case PAGE_OUTPUT:
		pCfgDlg = &m_cfgSpeedAlmOutputDlg;
		break;
	default:
		assert(false);
		break;
	}
	return pCfgDlg;
}

void CCfgSpeedAlarmDlg::SetAnchorPage(PAGE_ITEM item)
{
	m_initAnchorPage = item;
}

BEGIN_MSG_MAP(CCfgSpeedAlarmDlg,CCfgDlg)
	ON_MSG(m_cfgSpeedAlmBasicDlg.GetID(), KEY_VIRTUAL_ENTER, OnClickBasic)
	ON_MSG(m_cfgSpeedAlmOutputDlg.GetID(), KEY_VIRTUAL_ENTER, OnOutputDlg)
END_MSG_MAP()


unsigned long CCfgSpeedAlarmDlg::OnClickBasic()
{
	if (m_cfgSpeedAlmBasicDlg.IsModify())
	{
		EnableOkBtn(true, true);
	}

	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CCfgSpeedAlarmDlg::OnOutputDlg()
{
	if (m_cfgSpeedAlmOutputDlg.IsModify())
	{
		EnableOkBtn(true, true);
	}

	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CCfgSpeedAlarmDlg::OnPreCommand(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	if ((ctrlID == m_tabCtrl.GetID()) && (KEY_VIRTUAL_ENTER == keyID))
	{
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




//////////////////////////////////////////////////////////////////////////
// __DVR_ANCHOR__
//////////////////////////////////////////////////////////////////////////

CCfgSpeedAlarmAnchorDlg::CCfgSpeedAlarmAnchorDlg()
{

}

CCfgSpeedAlarmAnchorDlg::~CCfgSpeedAlarmAnchorDlg()
{

}

void CCfgSpeedAlarmAnchorDlg::OnInitial()
{
	GUI::CDialog::OnInitial();
	SetImage(BMP_MEM_DLG_NORMAL, false);
	SetCaption(m_pStrTable->String(E_STR_ID_TITLE_ALARMSPEED), false);
	//////////////////////////////////////////////////////////////////////////
	unsigned short x = 200;
	unsigned short y = m_length[TITLE_HEIGHT] + m_length[DLG_OFFSET_TOP] + 25;
	unsigned short x_off = 60, y_off = 10;
	unsigned short cx = m_length[ANCHOR_WIDTH], cy = m_length[ANCHOR_HEIGHT];

	int yFocus = 1;

	m_anSpeedAlmBasic.Create(GUI::CWnd::GetRandWndID(), x, y, cx, cy, this, 0, yFocus++, m_pStrTable->String(E_STR_ID_MENU_SPEED_ALARM));
	m_anSpeedAlmBasic.SetImage(BMP_ANCHOR_CONFIG, BMP_ANCHOR_CONFIG_F, BMP_ANCHOR_CONFIG);

	y += 60;
	m_anSpeedAlmOutput.Create(GUI::CWnd::GetRandWndID(), x, y, cx, cy, this, 0, yFocus++, m_pStrTable->String(E_STR_ID_ALARM_HANDING));
	m_anSpeedAlmOutput.SetImage(BMP_ANCHOR_CONFIG, BMP_ANCHOR_CONFIG_F, BMP_ANCHOR_CONFIG);

	y += 60;
	m_anExit.Create(GUI::CWnd::GetRandWndID(), x, y, cx, cy , this, 0, yFocus++, m_pStrTable->String(E_STR_ID_EXIT));
	m_anExit.SetImage(BMP_ANCHOR_CONFIG, BMP_ANCHOR_CONFIG_F, BMP_ANCHOR_CONFIG);
}

BEGIN_MSG_MAP(CCfgSpeedAlarmAnchorDlg, GUI::CDialog)
	ON_MSG(m_anSpeedAlmBasic.GetID(), KEY_VIRTUAL_ENTER, OnAnchorSpeedAlmBasic)
	ON_MSG(m_anSpeedAlmOutput.GetID(), KEY_VIRTUAL_ENTER, OnAnchorSpeedAlmOutput)
	ON_MSG(m_anExit.GetID(), KEY_VIRTUAL_ENTER, OnExit)
END_MSG_MAP()

unsigned long CCfgSpeedAlarmAnchorDlg::OnAnchorSpeedAlmBasic()
{
	m_cfgSpeedAlarmDlg.SetAnchorPage(CCfgSpeedAlarmDlg::PAGE_BASIC);
	m_cfgSpeedAlarmDlg.DoModal(this, CENTER_POS, CENTER_POS, MAX_SCREEN_X, MAX_SCREEN_Y, false);

	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CCfgSpeedAlarmAnchorDlg::OnAnchorSpeedAlmOutput()
{
	m_cfgSpeedAlarmDlg.SetAnchorPage(CCfgSpeedAlarmDlg::PAGE_OUTPUT);
	m_cfgSpeedAlarmDlg.DoModal(this, CENTER_POS, CENTER_POS, MAX_SCREEN_X, MAX_SCREEN_Y, false);

	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CCfgSpeedAlarmAnchorDlg::OnExit()
{
	EndDialog(DLG_ESC);
	return KEY_VIRTUAL_MSG_STOP;
}

//end
