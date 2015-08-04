/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : l
** Date         : 2009-08-14
** Name         : CfgPtzDlg.cpp
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/

#include "CfgPtzDlg.h"


CCfgPtzDlg::CCfgPtzDlg() : m_curPage(PAGE_SERIAL),
m_initAnchorPage(PAGE_SERIAL),
m_initPageMask(0)
{

}

CCfgPtzDlg::~CCfgPtzDlg()
{

}

void CCfgPtzDlg::OnInitial()
{
	CCfgDlg::OnInitial();
	SetImage(BMP_MEM_DLG_LARGE, false);
	SetDefaultBtn( DEFAULT_BTN_APPLY | DEFAULT_BTN_OK | DEFAULT_BTN_CANCEL);
	SetDefaultBtnCaption( DEFAULT_BTN_CANCEL, m_pStrTable->String(E_STR_ID_EXIT) );
	SetDefaultBtnCaption( DEFAULT_BTN_OK, m_pStrTable->String(E_STR_ID_APPLY) );
	SetDefaultBtnCaption( DEFAULT_BTN_APPLY, m_pStrTable->String(E_STR_ID_DEFAULT) );
	//////////////////////////////////////////////////////////////////////////

	m_initPageMask = 0;

#ifndef __DVR_ANCHOR__
	////
	SetCaption(m_pStrTable->String(E_STR_ID_TITLE_PTZ), false);
	unsigned short x = m_length[DLG_OFFSET_LEFT];
	unsigned short y = m_length[TITLE_HEIGHT] + m_length[TABLE_HEIGHT] + m_length[DLG_OFFSET_TOP];
	unsigned short cx = m_rect.Width() - 2 * m_length[DLG_OFFSET_LEFT];
	unsigned short cy = m_rect.Height() - y- m_length[DLG_OFFSET_BOTTOM] - m_length[DEFAULT_BTN_BOTTOM_YPOS];
	
	m_serialPortDlg.Create(GetRandWndID(), x, y, cx, cy, this, 0, 2);
	//m_AdvanceDlg.Create(GetRandWndID(), x, y, cx, cy, this, 0, 3);

	m_tabCtrl.Create(GetRandWndID(), x, m_length[TITLE_HEIGHT] + m_length[DLG_OFFSET_TOP], cx, m_length[TABLE_HEIGHT], this, 0, 1);

	m_tabCtrl.AddItem(&m_serialPortDlg, m_pStrTable->String(E_STR_ID_SERIAL_PORT));
	m_tabCtrl.AddItem(&m_AdvanceDlg, m_pStrTable->String(E_STR_ID_ADVANCE));

	m_curPage = PAGE_SERIAL;
	m_tabCtrl.SetCurItem(m_curPage, false);
	CCfgDlg::SetRefreshView( false );
#else
	////
	unsigned short x = m_length[DLG_OFFSET_LEFT];
	unsigned short y = m_length[TITLE_HEIGHT] + m_length[DLG_OFFSET_TOP];
	unsigned short offset = m_length[DLG_OFFSET_BOTTOM];
	unsigned short cx = m_rect.Width() - 2 * m_length[DLG_OFFSET_LEFT];
	unsigned short cy = m_rect.Height() - y - offset - m_length[DEFAULT_BTN_BOTTOM_YPOS];

	switch (m_initAnchorPage)
	{
	case PAGE_SERIAL:
		SetCaption(m_pStrTable->String(E_STR_ID_TITLE_SERIAL_PORT), false);
		m_serialPortDlg.Create(GetRandWndID(), x, y, cx, cy, this, 0, 1);
		break;
	case PAGE_ADVANCE:
		SetCaption(m_pStrTable->String(E_STR_ID_TITLE_ADVANCED), false);
		m_AdvanceDlg.Create(GetRandWndID(), x, y, cx, cy, this, 0, 1);
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


void CCfgPtzDlg::GetItemList(CFGITEM_LIST& itemList) const
{
	itemList.push_back(NCFG_ITEM_PTZ_SERIAL);
	itemList.push_back(NCFG_ITEM_PTZ_CURISE_TYPE);
	
	for (int i = 0; i < s_loginSuccessReply.productInfo.videoInputNum; ++i)
	{
		assert(NCFG_ITEM_PTZ_INFO_PRESET_INFO_CHBASE + i < NCFG_ITEM_PTZ_INFO_PRESET_INFO_CHEND);
		itemList.push_back(static_cast<NCFG_ITEM_ID>(NCFG_ITEM_PTZ_INFO_PRESET_INFO_CHBASE + i));
	}


	for (int i = 0; i < s_loginSuccessReply.productInfo.videoInputNum; ++i)
	{
		assert(NCFG_ITEM_PTZ_INFO_CRUISE_INFO_CHBASE + i < NCFG_ITEM_PTZ_INFO_CRUISE_INFO_CHEND);
		itemList.push_back(static_cast<NCFG_ITEM_ID>(NCFG_ITEM_PTZ_INFO_CRUISE_INFO_CHBASE + i));
	}
}

unsigned long CCfgPtzDlg::OnApply()
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

unsigned long CCfgPtzDlg::CheckValue()
{
	CCfgDlg* pCfgDlg = GetPage(m_curPage);
	assert(pCfgDlg);

	return pCfgDlg->CheckValue();
}

CCfgDlg* CCfgPtzDlg::GetPage(PAGE_ITEM item)
{
	CCfgDlg* pCfgDlg = NULL;
	switch (item)
	{
	case PAGE_SERIAL:
		pCfgDlg = &m_serialPortDlg;
		break;
	case PAGE_ADVANCE:
		pCfgDlg = &m_AdvanceDlg;
		break;
	default:
		assert(false);
		break;
	}

	return pCfgDlg;
}

void CCfgPtzDlg::SetAnchorPage(PAGE_ITEM item)
{
	m_initAnchorPage = item;
}

BEGIN_MSG_MAP(CCfgPtzDlg,CCfgDlg)
	ON_MSG(m_serialPortDlg.GetID(), KEY_VIRTUAL_ENTER, OnClickSerial)
	ON_MSG(m_AdvanceDlg.GetID(), KEY_VIRTUAL_ENTER, OnClickAdvance)
END_MSG_MAP()


unsigned long CCfgPtzDlg::OnPreCommand(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	std::vector<PTZ_SERIAL_INFO> ptzConfigList;
	if ((ctrlID == m_tabCtrl.GetID()) && (KEY_VIRTUAL_ENTER == keyID))
	{
		{
			unsigned short x = m_length[DLG_OFFSET_LEFT];
			unsigned short y = m_length[TITLE_HEIGHT] + m_length[TABLE_HEIGHT] + m_length[DLG_OFFSET_TOP];
			unsigned short cx = m_rect.Width() - 2 * m_length[DLG_OFFSET_LEFT];
			unsigned short cy = m_rect.Height() - y- m_length[DLG_OFFSET_BOTTOM] - m_length[DEFAULT_BTN_BOTTOM_YPOS];

			if(!((m_initPageMask >> PAGE_ADVANCE) & 0x1) && (PAGE_ADVANCE == m_tabCtrl.GetFocusItem()))
			{
				m_AdvanceDlg.Create(GUI::CWnd::GetRandWndID(),x,y,cx,cy, this,0, PAGE_ADVANCE + 2);

				SetSubCfgDlg(&m_AdvanceDlg);
				m_initPageMask |= (0x1 << PAGE_ADVANCE);
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
				if(PAGE_ADVANCE == m_curPage)
				{
					m_AdvanceDlg.GetConfigList(ptzConfigList);
					m_AdvanceDlg.SetListBtn(ptzConfigList, false);
				}
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

unsigned long CCfgPtzDlg::OnClickSerial()
{
	if (m_serialPortDlg.IsModify())
	{
		EnableOkBtn(true, true);
	}

	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CCfgPtzDlg::OnClickAdvance()
{
	if (m_AdvanceDlg.IsModify())
	{
		EnableOkBtn(true, true);
	}

	return KEY_VIRTUAL_MSG_STOP;
}



//////////////////////////////////////////////////////////////////////////
// __DVR_ANCHOR__
//////////////////////////////////////////////////////////////////////////

CCfgPtzAnchorDlg::CCfgPtzAnchorDlg()
{

}

CCfgPtzAnchorDlg::~CCfgPtzAnchorDlg()
{

}

void CCfgPtzAnchorDlg::OnInitial()
{
	GUI::CDialog::OnInitial();
	SetImage(BMP_MEM_DLG_NORMAL, false);
	SetCaption(m_pStrTable->String(E_STR_ID_TITLE_PTZ), false);
	//////////////////////////////////////////////////////////////////////////
	unsigned short x = 200;
	unsigned short y = m_length[TITLE_HEIGHT] + m_length[DLG_OFFSET_TOP] + 25;
	unsigned short x_off = 60, y_off = 10;
	unsigned short cx = m_length[ANCHOR_WIDTH], cy = m_length[ANCHOR_HEIGHT];

	int yFocus = 1;

	m_anSerialPort.Create(GUI::CWnd::GetRandWndID(), x, y, cx, cy , this, 0, yFocus++, m_pStrTable->String(E_STR_ID_SERIAL_PORT));
	m_anSerialPort.SetImage(BMP_ANCHOR_SERIAL_PORT, BMP_ANCHOR_SERIAL_PORT_F, BMP_ANCHOR_SERIAL_PORT);

	y += 60;
	m_anAdvance.Create(GUI::CWnd::GetRandWndID(), x, y, cx, cy, this, 0, yFocus++, m_pStrTable->String(E_STR_ID_ADVANCE));
	m_anAdvance.SetImage(BMP_ANCHOR_PTZ_ADVANCED, BMP_ANCHOR_PTZ_ADVANCED_F, BMP_ANCHOR_PTZ_ADVANCED);

	y += 60;
	m_anExit.Create(GUI::CWnd::GetRandWndID(), x, y, cx, cy, this, 0, yFocus++, m_pStrTable->String(E_STR_ID_EXIT));
	m_anExit.SetImage(BMP_ANCHOR_EXIT, BMP_ANCHOR_EXIT_F, BMP_ANCHOR_EXIT);
}

BEGIN_MSG_MAP(CCfgPtzAnchorDlg, GUI::CDialog)
	ON_MSG(m_anSerialPort.GetID(), KEY_VIRTUAL_ENTER, OnAnchorSerialPort)
	ON_MSG(m_anAdvance.GetID(), KEY_VIRTUAL_ENTER, OnAnchorAdvance)
	ON_MSG(m_anExit.GetID(), KEY_VIRTUAL_ENTER, OnExit)
END_MSG_MAP()

unsigned long CCfgPtzAnchorDlg::OnAnchorSerialPort()
{
	m_cfgPtzDlg.SetAnchorPage(CCfgPtzDlg::PAGE_SERIAL);
	m_cfgPtzDlg.DoModal(this, CENTER_POS, CENTER_POS, MAX_SCREEN_X, MAX_SCREEN_Y, false);

	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CCfgPtzAnchorDlg::OnAnchorAdvance()
{
	m_cfgPtzDlg.SetAnchorPage(CCfgPtzDlg::PAGE_ADVANCE);
	m_cfgPtzDlg.DoModal(this, CENTER_POS, CENTER_POS, MAX_SCREEN_X, MAX_SCREEN_Y, false);

	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CCfgPtzAnchorDlg::OnExit()
{
	EndDialog(DLG_ESC);
	return KEY_VIRTUAL_MSG_STOP;
}

//end
