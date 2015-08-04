/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : L
** Date         : 2009-08-19
** Name         : ManualAlarmDlg.cpp
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/

#include "ManualAlarmDlg.h"
#include "NetProtocol.h"

extern NET_LOGIN_SUCCESS_REPLY *g_p_login_succ;

CManualAlarmDlg::CManualAlarmDlg()
{

}

CManualAlarmDlg::~CManualAlarmDlg()
{

}

void CManualAlarmDlg::OnInitial()
{
	CCfgDlg::OnInitial();
	SetImage(BMP_MEM_DLG_LARGE, false);
	SetCaption(m_pStrTable->String(E_STR_ID_TITLE_MANUAL_ALARM), false);

	SetDefaultBtn( DEFAULT_BTN_APPLY | DEFAULT_BTN_OK | DEFAULT_BTN_CANCEL);
	SetDefaultBtnCaption( DEFAULT_BTN_APPLY,m_pStrTable->String(E_STR_ID_ALARM));
	SetDefaultBtnCaption( DEFAULT_BTN_OK,m_pStrTable->String(E_STR_ID_CLEAR_ALARM));
	SetDefaultBtnCaption( DEFAULT_BTN_CANCEL, m_pStrTable->String(E_STR_ID_EXIT) );
	//////////////////////////////////////////////////////////////////////////

	GUI::CListCtrl& list = m_manualAlarmList;
	GUI::CWnd * pWnd = NULL;
	GUI::CCheckCtrl* pCheck = NULL;
	GUI::CComboCtrl* pCombo = NULL;

	unsigned short xLeft = m_length[DLG_OFFSET_LEFT], yTop = m_length[TITLE_HEIGHT] + m_length[DLG_OFFSET_TOP];
	unsigned short x = xLeft, y = yTop;
	//unsigned short xOff = m_length[DLG_OFFSET_X], yOff = m_length[DLG_OFFSET_Y];

	unsigned short width = m_rect.Width() - 2 * m_length[DLG_OFFSET_LEFT];
	unsigned short height = m_rect.Height() - y - m_length[DEFAULT_BTN_BOTTOM_YPOS];

	/////List

	list.Create(GUI::CWnd::GetRandWndID(), x, y, width, height, this, 0, 1, true);
	list.AddColumn(m_pStrTable->String(E_STR_ID_INDEX), 64);
	list.AddColumn(m_pStrTable->String(E_STR_ID_ALARM_NAME),(width - 64)*16/32);
#if !defined(__TW01_RILI__)
	list.AddColumn(m_pStrTable->String(E_STR_ID_IP),(width - 64)*8/32);
#endif
	list.AddColumn(m_pStrTable->String(E_STR_ID_TRIGGER),(width - 64)*8/32);

	//GUI::CRect rect = list.GetClientRect();
	//unsigned short yOff = (m_rect.Height() - rect.Height() - m_length[DEFAULT_BTN_BOTTOM_YPOS] - y - m_length[DLG_OFFSET_BOTTOM]) * 2 / 5;
	//y += yOff;
	//list.MoveWnd( m_rect.m_left + x, m_rect.m_top + y );

	char str[128] = {0};
	int num = g_p_login_succ->productInfo.relayOutputNum;

	for ( int i = 0; i < num; i++ )
	{
		sprintf( str, "%d", i + 1 );
		list.AddItem( i, COL_INDEX, str );

		list.AddItem( i, COL_ALARM_NAME, "" );

		//获取本地IP
#if !defined(__TW01_RILI__)
		strcpy( str, "127.0.0.1" );
		list.AddItem( i, COL_IP_ADDRESS, str );
#endif
		pCheck = dynamic_cast<GUI::CCheckCtrl*>( list.AddItem( i, COL_TRIGGER, WND_TYPE_CHECK ) );
	}

	list.CreateListFocusMap();

	x = xLeft;
	y = m_rect.Height() - m_length[DEFAULT_BTN_BOTTOM_YPOS];

	unsigned short cx = 0, cy = 0;
	const char* pStr = m_pStrTable->String(E_STR_ID_ALL);
	TextSize(pStr, strlen(pStr), cx, cy);

	m_ckAll.Create( GUI::CWnd::GetRandWndID(), x, y, cx + m_length[CHECK_WIDTH] + 6, m_length[CHECK_HEIGHT], this, 1, 1 );
	m_ckAll.SetCaption( m_pStrTable->String(E_STR_ID_ALL), false, false );

	m_ckAll.SetCheck(false, false);

	m_pMsgMan = CMessageMan::Instance();
}


void CManualAlarmDlg::UpdateView(bool bRefresh)
{
	CCfgDlg::UpdateView(bRefresh);

	SetModify(true);
	EnableOkBtn(true, false);
}

void CManualAlarmDlg::GetItemList(CFGITEM_LIST& itemList) const
{
	itemList.push_back(NCFG_ITEM_ALARM_OUT_NAME);
}

bool CManualAlarmDlg::ShowData(NCFG_ITEM_ID cfgID, const BYTE* pData, int len, bool bRefresh)
{
	assert(NULL != pData);
	assert(len > 0);

	int num = g_p_login_succ->productInfo.relayOutputNum;

	switch (cfgID)
	{
	case NCFG_ITEM_ALARM_OUT_NAME:
		{
			assert(len == sizeof(NCFG_INFO_SHORT_NAME) * num);
			const NCFG_INFO_SHORT_NAME* pName = reinterpret_cast<const NCFG_INFO_SHORT_NAME*>(pData);

			for (int i = 0; i < num; i++)
			{
				m_manualAlarmList.AddItem(i, COL_ALARM_NAME, pName[i].name);
			}
		}
		return true;
	default:
		break;
	}
	return false;
}

unsigned long CManualAlarmDlg::OnApply()
{
	//clear
	GUI::CListCtrl* pList = &m_manualAlarmList;
	GUI::CCheckCtrl* pCheck = NULL;

	ULONGLONG alarm = 0;
	for (int i = 0; i < pList->GetItemCount(); i++)
	{
		pCheck = dynamic_cast<GUI::CCheckCtrl*>(pList->GetItemWnd(i, COL_TRIGGER));
		assert(pCheck);
		if(pCheck->IsCheck())
		{
			alarm |= ((ULONGLONG)(0x01) << i);
		}
	}

	m_pMsgMan->PostMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_OPEN_ALARM, (unsigned char *)&alarm, sizeof(ULONGLONG));

	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CManualAlarmDlg::OnOK()
{
	//alarm
	GUI::CListCtrl* pList = &m_manualAlarmList;
	GUI::CCheckCtrl* pCheck = NULL;

	ULONGLONG alarm = 0;
	for (int i = 0; i < pList->GetItemCount(); i++)
	{
		pCheck = dynamic_cast<GUI::CCheckCtrl*>(pList->GetItemWnd(i, COL_TRIGGER));
		assert(pCheck);
		if(pCheck->IsCheck())
		{
			alarm |= ((ULONGLONG)(0x01) << i);
		}
	}

	m_pMsgMan->PostMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_CLEAN_ALARM, (unsigned char *)&alarm, sizeof(ULONGLONG));

	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CManualAlarmDlg::OnCancel()
{
	return GUI::CDialog::OnCancel();
}

unsigned long CManualAlarmDlg::CheckUI(bool bCk, bool bRefresh)
{
	GUI::CListCtrl* pList = &m_manualAlarmList;
	GUI::CCheckCtrl* pCheck = NULL;

	if(bCk)
	{
		bool checkAll = m_ckAll.IsCheck();
		for (int i = 0; i < pList->GetItemCount(); i++)
		{
			pCheck = dynamic_cast<GUI::CCheckCtrl*>(pList->GetItemWnd(i, COL_TRIGGER));
			assert(pCheck);
			if(pCheck->IsCheck() != checkAll)
			{
				pCheck->SetCheck(checkAll, bRefresh);
			}
		}
	}
	else
	{
		bool checkAll = true;
		for (int i = 0; i < pList->GetItemCount(); i++)
		{
			pCheck = dynamic_cast<GUI::CCheckCtrl*>(pList->GetItemWnd(i, COL_TRIGGER));
			assert(pCheck);
			if(!pCheck->IsCheck())
			{
				checkAll = false;
				break;
			}
		}

		if(checkAll != m_ckAll.IsCheck())
		{
			m_ckAll.SetCheck(checkAll, bRefresh);
		}
	}

	return KEY_VIRTUAL_MSG_STOP;
}

BEGIN_MSG_MAP(CManualAlarmDlg, CCfgDlg)
	ON_MSG(m_ckAll.GetID(), KEY_VIRTUAL_ENTER, OnClickAll)
	ON_MSG_EX(m_manualAlarmList.GetID(), KEY_VIRTUAL_MSG_DEFAULT_TAG, OnClickList)
END_MSG_MAP()

unsigned long CManualAlarmDlg::OnClickAll()
{
	bool bCheckAll = m_ckAll.IsCheck();

	CheckUI(true, true);

	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CManualAlarmDlg::OnClickList(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	unsigned short row = (keyID >> 16)&0x7fff;	//行
	unsigned short col = keyID&0xffff;			//列

	if(COL_TRIGGER == col)
	{
		CheckUI(false, true);
	}

	return KEY_VIRTUAL_MSG_STOP;
}
