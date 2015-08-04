/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : L
** Date         : 2010-06-30
** Name         : OtherAlarmDlg.cpp
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/

#include "OtherAlarmDlg.h"
#include "FuncCustom.h"

using namespace GUI;

COtherAlarmDlg::COtherAlarmDlg() : m_pOtherAlarmInfo(NULL),
m_maxAlarmOut(0)
{

}

COtherAlarmDlg::~COtherAlarmDlg()
{

}

void COtherAlarmDlg::OnInitial()
{
	GUI::CScrollDialog::OnInitial();
	GUI::CScrollWnd& wnd = GetScrollWnd();
	//////////////////////////////////////////////////////////////////////////

	unsigned short border = 2;
	unsigned long width = m_rect.Width();
	unsigned long height= m_length[LIST_ITEM_HEIGHT];

	//设置滚动窗口的边框和可视区域
	SetDlgBorder(border);

	//设置滚动窗口单元格大小和最大视图区域
	wnd.SetSizeOffset(width / 2, height);
	wnd.SetViewMaxSize(width, height * 5);

	unsigned char offset_y = m_length[LIST_ITEM_YPOS];
	unsigned char offset_x = m_length[LIST_ITEM_XPOS];
	unsigned long xx = 0, yy = 0, cx = width / 2 - 10, cy = height;
	int xFocus = 0, yFocus = 0;
	int row = 0;
	GUI::CRect rect;
	char strtmp[64] = {0};

	//报警类型
	rect.SetRect(0, 0, cx, m_length[STATIC_HEIGHT]);
	GUI::CWnd* pWnd = wnd.AddWnd(WND_TYPE_STATIC, GetRandWndID(), rect, 0, row, 0, 0, CScrollWnd::SW_WND_LEFT, -1);
	pWnd->SetCaption(m_pStrTable->String(E_STR_ID_ALARM_TYPE), false);
	
	rect.SetRect(0, 0, 180, m_length[COMBO_HEIGHT]);
	GUI::CComboCtrl* pCombo = dynamic_cast<GUI::CComboCtrl*>(wnd.AddWnd(WND_TYPE_COMBO, GetRandWndID(), rect, 1, row, 0, yFocus++, CScrollWnd::SW_WND_LEFT, SW_ITEM_ID_TYPE));
	pCombo->AddItem(m_pStrTable->String(E_STR_ID_DISK_FULL), ITEM_TYPE_DISK_FULL);
#if !defined(__TW01_RILI__)
	pCombo->AddItem(m_pStrTable->String(E_STR_ID_LOG_IP_CONFLICT), ITEM_TYPE_IP_CONFLICT);
	pCombo->AddItem(m_pStrTable->String(E_STR_ID_DISCONNECT), ITEM_TYPE_NET_DISCONNECT);
	pCombo->AddItem(m_pStrTable->String(E_STR_ID_LOG_DISK_WARNING), ITEM_TYPE_DISK_WARNING);
	pCombo->AddItem(m_pStrTable->String(E_STR_ID_LOG_DISK_DISCONNECT), ITEM_TYPE_DISK_DISCONNECT);
#else
	pCombo->AddItem(m_pStrTable->String(E_STR_ID_LOG_DISK_WARNING), ITEM_TYPE_DISK_WARNING);
#endif
	pCombo->SetCurItemData(0, false);

	//声音报警
	row += 2;
	rect.SetRect(0, 0, cx, m_length[STATIC_HEIGHT]);
	pWnd = wnd.AddWnd(WND_TYPE_STATIC, GetRandWndID(), rect, 0, row, 0, 0, CScrollWnd::SW_WND_LEFT, -1);
	pWnd->SetCaption(m_pStrTable->String(E_STR_ID_BUZZER), false);

	rect.SetRect(0, 0, m_length[CHECK_WIDTH], m_length[CHECK_HEIGHT]);
	wnd.AddWnd(WND_TYPE_CHECK, GetRandWndID(), rect, 1, row, 0, yFocus++, CScrollWnd::SW_WND_LEFT, SW_ITEM_ID_BUZZER);

#ifdef __DVR_BASIC__
#if !defined(__TW01_RILI__)
	//Email alarm
	row += 1;
	rect.SetRect(0, 0, cx, m_length[STATIC_HEIGHT]);
	pWnd = wnd.AddWnd(WND_TYPE_STATIC, GetRandWndID(), rect, 0, row, 0, 0, CScrollWnd::SW_WND_LEFT, SW_ITEM_ID_EMAIL_EN_STATIC);
	pWnd->SetCaption(m_pStrTable->String(E_STR_ID_EMAIL), false);

	rect.SetRect(0, 0, m_length[CHECK_WIDTH], m_length[CHECK_HEIGHT]);
	wnd.AddWnd(WND_TYPE_CHECK, GetRandWndID(), rect, 1, row, 0, yFocus++, CScrollWnd::SW_WND_LEFT, SW_ITEM_ID_EMAIL_EN);
#endif
#endif

	//Alarm out
	if (IsSupportAlarmOut())
	{
		row += 1;
		rect.SetRect(0, 0, cx, m_length[STATIC_HEIGHT]);
		pWnd = wnd.AddWnd(WND_TYPE_STATIC, GetRandWndID(), rect, 0, row, 0, 0, CScrollWnd::SW_WND_LEFT, -1);
		pWnd->SetCaption(m_pStrTable->String(E_STR_ID_TO_ALARM_OUT), false);

		int numAlarmOut = m_maxAlarmOut/*CCfgDlg::s_loginSuccessReply.productInfo.relayOutputNum*/;
		xFocus = 0;
		rect.SetRect(0, 0, m_length[CHECK_WIDTH] + 24, m_length[CHECK_HEIGHT]);
		for (int i = 0; i < numAlarmOut; i++)
		{
			memset(strtmp, 0 , sizeof(strtmp));	
			GUI::CCheckCtrl* pCheck = dynamic_cast<GUI::CCheckCtrl*>(wnd.AddWnd(WND_TYPE_CHECK, GetRandWndID(), rect, 1, row, xFocus++, yFocus, CScrollWnd::SW_WND_LEFT, i + SW_ITEM_ID_ALARM_OUT_0));
			sprintf(strtmp, "%d", i + 1);
			pCheck->SetCaption(strtmp, false, false);

			rect.Offset(m_length[CHECK_WIDTH] + 24, 0);
		}
		yFocus += 1;
	}

	//Disk Free
	row += 1;
	rect.SetRect(0, 0, cx, m_length[STATIC_HEIGHT]);
	pWnd = wnd.AddWnd(WND_TYPE_STATIC, GetRandWndID(), rect, 0, row, 0, 0, CScrollWnd::SW_WND_LEFT, SW_ITEM_ID_DISK_FREE_SATIC);
	pWnd->SetCaption(m_pStrTable->String(E_STR_ID_DISK_ALARM), false);

	rect.SetRect(0, 0, 160, m_length[COMBO_HEIGHT]);
	pCombo = dynamic_cast<GUI::CComboCtrl*>(wnd.AddWnd(WND_TYPE_COMBO, GetRandWndID(), rect, 1, row, 0, yFocus++, CScrollWnd::SW_WND_LEFT, SW_ITEM_ID_DISK_FREE));
	for (int i = 0; i < 5; i++ )
	{
		sprintf(strtmp, "%d MB", i * 128);
		pCombo->AddItem(strtmp,  i * 128);
	}
	sprintf(strtmp, "%d MB", 8 * 128);		
	pCombo->AddItem(strtmp,  8 * 128);
	sprintf(strtmp, "%d MB", 16 * 128);		
	pCombo->AddItem(strtmp,  16 * 128);
	pCombo->AddItem(m_pStrTable->String(E_STR_ID_NONE), INVALID_INDEX(unsigned long));
	pCombo->SetCurItemData(128, false);
}

void COtherAlarmDlg::OnPaint(PAINT_STATUS status)
{
	//仿ListCtrl风格
	GUI::CScrollDialog::OnPaint(status);

	GUI::CScrollWnd& wnd = GetScrollWnd();
	unsigned long maxCol = 0, maxRow = 0;
	wnd.GetPageCount(maxCol, maxRow);

	unsigned long x_left = m_rect.m_left, y_top = m_rect.m_top;
	unsigned long center_x = 0;
	unsigned long width = 0;
	unsigned long height = m_rect.Height() - 2;

	unsigned long xx = 0, yy = 0, cx = 0, cy = 0;
	wnd.GetViewPos(1, 0, xx, yy);
	center_x = x_left + xx;

	wnd.GetViewSize(1, 0, cx, cy);
	width = xx + cx;				//如果设置的行数大于maxRow,还需预留滚动条的位置

	unsigned long x = x_left, y = y_top;
	for (int i = 0; i < maxRow - 1; i++)
	{
		y += cy;
		CWnd::DrawLine(x, y, x + width, y, COLOR_LINE_GRAY);
	}

	CWnd::DrawLine(center_x, y_top, center_x, y_top + cy, COLOR_LINE_GRAY);
	CWnd::DrawLine(center_x, y_top + 2 * cy, center_x, y_top + height, COLOR_LINE_GRAY);
}

void COtherAlarmDlg::SetOtherAlarmInfo(OTHER_ALARM_INFO* pInfo)
{
	m_pOtherAlarmInfo = pInfo;
}

void COtherAlarmDlg::SetMaxAlarmOut(int max)
{
	m_maxAlarmOut = max;
}

void COtherAlarmDlg::InitCtrlData(bool bRefresh)
{
	GUI::CScrollWnd& wnd = GetScrollWnd();

	//报警类型
	GUI::CComboCtrl* pComboType = dynamic_cast<GUI::CComboCtrl*>(wnd.GetWnd(SW_ITEM_ID_TYPE));
	unsigned long index = pComboType->GetCurItemData();

	//声音报警
	GUI::CCheckCtrl* pCheck = dynamic_cast<GUI::CCheckCtrl*>(wnd.GetWnd(SW_ITEM_ID_BUZZER));
	pCheck->SetCheck(m_pOtherAlarmInfo[index].Buzzer, false);

#ifdef __DVR_BASIC__
#if !defined(__TW01_RILI__)
	//Email 报警
	{
		GUI::CCheckCtrl*pCheck = dynamic_cast<GUI::CCheckCtrl*>(wnd.GetWnd(SW_ITEM_ID_EMAIL_EN));
		pCheck->SetCheck(m_pOtherAlarmInfo[index].Email, false);
		GUI::CStatic* pStatic = dynamic_cast<GUI::CStatic*>(wnd.GetWnd(SW_ITEM_ID_EMAIL_EN_STATIC));
		if ((index == static_cast<unsigned long>(ITEM_TYPE_NET_DISCONNECT))
		  ||(index == static_cast<unsigned long>(ITEM_TYPE_IP_CONFLICT)))
		{
			if(pStatic->IsShow())
			{
				pStatic->Show(false, false);
			}

			if(pCheck->IsShow())
			{
				pCheck->Show(false, false);
			}
		}
		else
		{
			if(!pStatic->IsShow())
			{
				pStatic->Show(true, false);
			}

			if(!pCheck->IsShow())
			{
				pCheck->Show(true, false);
			}
		}
	}
#endif
#endif

	//Alarm Out
	if (IsSupportAlarmOut())
	{
		for(int i = 0; i < m_maxAlarmOut; i++)
		{
			pCheck = dynamic_cast<GUI::CCheckCtrl*>(wnd.GetWnd(i + SW_ITEM_ID_ALARM_OUT_0));
			pCheck->SetCheck((m_pOtherAlarmInfo[index].AlarmOut & (0x1 << i)), false);
		}
	}

	//Disk Free
	GUI::CComboCtrl* pCombo = dynamic_cast<GUI::CComboCtrl*>(wnd.GetWnd(SW_ITEM_ID_DISK_FREE));
	GUI::CStatic* pStatic = dynamic_cast<GUI::CStatic*>(wnd.GetWnd(SW_ITEM_ID_DISK_FREE_SATIC));
	if(index == static_cast<unsigned long>(ITEM_TYPE_DISK_FULL))
	{
		if(!pStatic->IsShow())
		{
			pStatic->Show(true, false);
		}

		if(!pCombo->IsShow())
		{
			pCombo->Show(true, false);
		}

		pCombo->SetCurItemData(m_pOtherAlarmInfo[index].DiskAlarmFree, false);
	}
	else
	{
		if(pStatic->IsShow())
		{
			pStatic->Show(false, false);
		}

		if(pCombo->IsShow())
		{
			pCombo->Show(false, false);
		}
	}

	if(bRefresh)
	{
		Repaint();
	}
}

BEGIN_MSG_MAP(COtherAlarmDlg, GUI::CScrollDialog)
	ON_MSG_EX(GetScrollWnd().GetID(), KEY_VIRTUAL_MSG_DEFAULT_TAG, OnClickScrollDlg)
END_MSG_MAP()

unsigned long COtherAlarmDlg::OnClickScrollDlg(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	GUI::CScrollWnd& wnd = GetScrollWnd();

	int index = -1;
	unsigned long tmpIndex = (keyID >> 20) & 0x7FF;
	if( tmpIndex < 0x7FF )
	{
		index = tmpIndex;
	}
	unsigned long id = keyID & 0xFFFFF;

	if(index >= 0)
	{
		GUI::CComboCtrl* pCombo = dynamic_cast<GUI::CComboCtrl*>(wnd.GetWnd(SW_ITEM_ID_TYPE));
		unsigned long item = pCombo->GetCurItemData();

		if(SW_ITEM_ID_TYPE == index)
		{
			InitCtrlData(true);
			return KEY_VIRTUAL_MSG_STOP;
		}

		if((SW_ITEM_ID_DISK_FREE == index) && (ITEM_TYPE_DISK_FULL == item))
		{
			pCombo = dynamic_cast<GUI::CComboCtrl*>(wnd.GetWnd(SW_ITEM_ID_DISK_FREE));
			m_pOtherAlarmInfo[item].DiskAlarmFree = pCombo->GetCurItemData();

#if defined(__TW01_RILI__)
			if (ITEM_TYPE_DISK_WARNING == item)
			{
				m_pOtherAlarmInfo[ITEM_TYPE_DISK_DISCONNECT].DiskAlarmFree = m_pOtherAlarmInfo[item].DiskAlarmFree;
			}
#endif

			return keyID;
		}

		if(SW_ITEM_ID_BUZZER == index)
		{
			GUI::CCheckCtrl* pCheck = dynamic_cast<GUI::CCheckCtrl*>(wnd.GetWnd(SW_ITEM_ID_BUZZER));
			m_pOtherAlarmInfo[item].Buzzer = (pCheck->IsCheck() ? 1 : 0);

#if defined(__TW01_RILI__)
			if (ITEM_TYPE_DISK_WARNING == item)
			{
				m_pOtherAlarmInfo[ITEM_TYPE_DISK_DISCONNECT].Buzzer = m_pOtherAlarmInfo[item].Buzzer;
			}
#endif

			return keyID;
		}

#ifdef __DVR_BASIC__
#if !defined(__TW01_RILI__)
		if(SW_ITEM_ID_EMAIL_EN == index)
		{
			GUI::CCheckCtrl* pCheck = dynamic_cast<GUI::CCheckCtrl*>(wnd.GetWnd(SW_ITEM_ID_EMAIL_EN));
			m_pOtherAlarmInfo[item].Email = (pCheck->IsCheck() ? 1 : 0);
			return keyID;
		}
#endif
#endif

		if((SW_ITEM_ID_ALARM_OUT_0 <= index) &&
			(SW_ITEM_ID_ALARM_OUT_0 + m_maxAlarmOut > index))
		{
			ULONGLONG alarmout = 0;
			for(int i = 0; i < m_maxAlarmOut; i++)
			{
				GUI::CCheckCtrl* pCheck = dynamic_cast<GUI::CCheckCtrl*>(wnd.GetWnd(SW_ITEM_ID_ALARM_OUT_0 + i));
				if (pCheck->IsCheck())
				{
					alarmout |= (0x1 << i);
				}
				else
				{
					alarmout &= ~(0x1 << i);
				}
				
			}
			m_pOtherAlarmInfo[item].AlarmOut = alarmout;

#if defined(__TW01_RILI__)
			if (ITEM_TYPE_DISK_WARNING == item)
			{
				m_pOtherAlarmInfo[ITEM_TYPE_DISK_DISCONNECT].AlarmOut = m_pOtherAlarmInfo[item].AlarmOut;
			}
#endif

			return keyID;
		}
	}

	return KEY_VIRTUAL_MSG_STOP;
}


//end
