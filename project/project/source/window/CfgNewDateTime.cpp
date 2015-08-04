/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : l
** Date         : 2009-08-12
** Name         : CfgDateTimeDlg.cpp
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/

#include "CfgNewDateTimeDlg.h"
#include "GuiAssert.h"

#ifdef __ENVIRONMENT_LINUX__
#include "TZoneDZoneCfg.h"
#endif

//#if defined(__TW01_RILI__)
//std::string g_zoneName[] = {"UTC-12:00", "UTC-11:00", "UTC-10:00", "UTC-09:00", "UTC-08:00", 
//							"UTC-07:00", "UTC-06:00", "UTC-05:00", "UTC-04:30", "UTC-04:00", 
//							"UTC-03:30",  "UTC-03:00", "UTC-02:00", "UTC-01:00", "UTC",
//							"UTC+01:00", "UTC+02:00", "UTC+03:00", "UTC+03:30", "UTC+04:00", 
//							"UTC+04:30", "UTC+05:00", "UTC+05:30", "UTC+05:45", "UTC+06:00",
//							"UTC+06:30", "UTC+07:00", "UTC+08:00", "UTC+09:00", "UTC+09:30", 
//							"UTC+10:00", "UTC+11:00", "UTC+12:00", "UTC+13:00"};
//
//#else
//std::string g_zoneName[] = {"GMT-12:00", "GMT-11:00", "GMT-10:00", "GMT-09:00", "GMT-08:00", 
//						   "GMT-07:00", "GMT-06:00", "GMT-05:00", "GMT-04:30", "GMT-04:00", 
//						   "GMT-03:30",  "GMT-03:00", "GMT-02:00", "GMT-01:00", "GMT",
//						   "GMT+01:00", "GMT+02:00", "GMT+03:00", "GMT+03:30", "GMT+04:00", 
//						   "GMT+04:30", "GMT+05:00", "GMT+05:30", "GMT+05:45", "GMT+06:00",
//						   "GMT+06:30", "GMT+07:00", "GMT+08:00", "GMT+09:00", "GMT+09:30", 
//						   "GMT+10:00", "GMT+11:00", "GMT+12:00", "GMT+13:00"};
//#endif
////static int g_zoneValue[]={720, 660, 600, 540, 480, 420, 360, 300, 270, 240, 
////										210, 180, 120, 60, 0, -60, -120, -180, -210, -240, 
////										-270, -300, -330, -345, -360, -390, -420, -480, -540, -570, 
////										-600, -660, -720, -780};

CCfgNewDateTimeDlg::CCfgNewDateTimeDlg() : m_currTime(0), m_bStopRefreshTime(false)
{

}

CCfgNewDateTimeDlg::~CCfgNewDateTimeDlg()
{

}

void CCfgNewDateTimeDlg::OnInitial()
{
	CCfgDlg::OnInitial();
	//////////////////////////////////////////////////////////////////////////
	GUI::CListCtrl& list = m_cfgDateTimeList;
	GUI::CWnd * pWnd = NULL;
	GUI::CCheckCtrl* pCheck = NULL;
	GUI::CComboCtrl* pCombo = NULL;
	GUI::CEditCtrl*  pEdit = NULL;
	GUI::CDateCtrl* pDate = NULL;
	GUI::CTimeCtrl*  pTime = NULL;
	GUI::CButton* pBtn = NULL;
	GUI::CNumEditCtrl* pNumEdit = NULL;

	unsigned short x = m_length[DLG_OFFSET_SUB_LEFT], y = m_length[DLG_OFFSET_SUB_TOP];
	unsigned short width = m_rect.Width() - 2 * m_length[DLG_OFFSET_SUB_LEFT];
	unsigned short height = m_rect.Height() - y - m_length[DLG_OFFSET_SUB_BOTTOM];

	//unsigned short offsetEdge = m_length[SCROLL_WIDTH] + 2;;
	//if( GUI::CWnd::m_width >= 1024 && GUI::CWnd::m_height >= 768 )
	//{
	//	offsetEdge = 4;
	//}

	list.Create(GUI::CWnd::GetRandWndID(), x, y, width, height, this, 0, 0, false);
	list.AddColumn(NULL, width / 2);
	list.AddColumn(NULL, width / 2 - 4/*- offsetEdge*/);

	////
	list.AddItem(ROW_DATE_FORMAT, 0, m_pStrTable->String(E_STR_ID_DATE_FORMAT));
	pCombo = dynamic_cast<GUI::CComboCtrl*>(list.AddItem(ROW_DATE_FORMAT, 1, WND_TYPE_COMBO));
	assert( pCombo );
	pCombo->AddItem(m_pStrTable->String(E_STR_ID_YY_MM_DD), DATE_MODE_YMD);
	pCombo->AddItem(m_pStrTable->String(E_STR_ID_MM_DD_YY), DATE_MODE_MDY);
	pCombo->AddItem(m_pStrTable->String(E_STR_ID_DD_MM_YY), DATE_MODE_DMY);
	pCombo->SetCurItem(0);
#if defined (__CUSTOM_RU04__)
	pCombo->SetEnable(false, false);
#endif

	char str[256] = {0};
	list.AddItem(ROW_TIME_FORMAT, 0, m_pStrTable->String(E_STR_ID_TIME_FORMAT) );
	pCombo = dynamic_cast<GUI::CComboCtrl*>(list.AddItem(ROW_TIME_FORMAT,1,WND_TYPE_COMBO));
	assert( pCombo );
	pCombo->AddItem(m_pStrTable->String(E_STR_ID_TIME_HOUR_12),TIME_MODE_12);
	pCombo->AddItem(m_pStrTable->String(E_STR_ID_TIME_HOUR_24),TIME_MODE_24);
	pCombo->SetMaxStringLen(32);
	pCombo->SetCurItemData(TIME_MODE_24,false);
#if defined (__CUSTOM_RU04__)
	pCombo->SetEnable(false, false);
#endif

	list.AddItem(ROW_TIME_ZONE, 0, m_pStrTable->String(E_STR_ID_TIME_ZONE));
	pCombo = dynamic_cast<GUI::CComboCtrl*>(list.AddItem(ROW_TIME_ZONE, 1, WND_TYPE_COMBO));
	assert(pCombo);
	pCombo->SetMaxStringLen( 32 );

	int count = TIME_ZONE_GMT_A13 + 1;
	//for (int i = 0; i < count; ++i)
	//{
	//	pCombo->AddItem(g_zoneName[i].c_str(),i);
	//}
	for(int i = 0; i < count; i++)
	{
		int zone = E_STR_ID_TIP_01;
		zone += i;
		ENUM_STRING_ID id;
		id = ENUM_STRING_ID(zone);
		if(id == E_STR_ID_TIP_01_04)
		{		
			pCombo->AddItem(m_pStrTable->String(E_STR_ID_TIP_01_04_01), i);
		}
		else
		{
			pCombo->AddItem(/*g_zoneName[i].c_str()*/m_pStrTable->String(id), i);
		}
	}
	pCombo->SetCurItemData(TIME_ZONE_GMT, false);
#if defined(__TW01_RILI__)
	pCombo->SetEnable(false, false);
#endif

#if defined(__DVR_BASIC__) && !defined(__TW01_RILI__)
	list.AddItem(ROW_TIME_SYNC, 0, "Auto Sync Time(Internet connection required)"/*m_pStrTable->String(E_STR_ID_SYNC_TIME_WITH_NTP)*/);
	pCheck = dynamic_cast<GUI::CCheckCtrl*>(list.AddItem(ROW_TIME_SYNC, 1, WND_TYPE_CHECK));
	assert(pCheck);

	list.AddItem(ROW_TIME_SERVER, 0, "Internet Time Server"/*m_pStrTable->String(E_STR_ID_NTP_SERVER)*/);
	pEdit = dynamic_cast<GUI::CEditCtrl*>(list.AddItem(ROW_TIME_SERVER, 1, WND_TYPE_EDIT));
	assert(pEdit);

	pBtn = dynamic_cast<GUI::CButton*>( list.AddItem( ROW_TIME_UPDATENOW, 1, WND_TYPE_BUTTON, m_length[BTN_WIDTH] + 50, m_length[BTN_HEIGHT]) );
	assert( pBtn );
	pBtn->SetCaption( m_pStrTable->String(E_STR_ID_UPDATE_NOW), false);
#endif

	list.AddItem(ROW_SYS_DATE, 0, m_pStrTable->String(E_STR_ID_SYSTEM_DATE));
	pDate = dynamic_cast<GUI::CDateCtrl*>(list.AddItem(ROW_SYS_DATE, 1, WND_TYPE_DATE,m_length[DATE_CTRL_WIDTH],m_length[DATE_CTRL_HEIGHT]));
	assert(pDate);
	m_currTime = GetCurrTime32();
	tm tmpTm = DVRTime32ToTm(m_currTime);
	pDate->SetDate(tmpTm.tm_year+1900,tmpTm.tm_mon,tmpTm.tm_mday, false);
	pDate->AddToNotifyList();

	list.AddItem(ROW_SYS_TIME, 0, m_pStrTable->String(E_STR_ID_SYSTEM_TIME));
	pTime = dynamic_cast<GUI::CTimeCtrl*>(list.AddItem(ROW_SYS_TIME, 1, WND_TYPE_TIME,m_length[DATE_CTRL_WIDTH]+4,m_length[DATE_CTRL_HEIGHT]));
	assert(pTime);
	pTime->SetTime(tmpTm.tm_hour,tmpTm.tm_min,tmpTm.tm_sec,false);
	pTime->AddToNotifyList();

	pBtn = dynamic_cast<GUI::CButton*>( list.AddItem( ROW_MODIFY_TIME, 1, WND_TYPE_BUTTON, m_length[BTN_WIDTH] + 50, m_length[BTN_HEIGHT]) );
	assert( pBtn );
	pBtn->SetCaption( m_pStrTable->String(E_STR_ID_SAVE_NOW), false);

	list.CreateListFocusMap();

	//
	//CheckUI( false );

	SetModifyTime(false, false);

	m_bUpdateTimeNow = false;
}

unsigned long CCfgNewDateTimeDlg::RecoverChange()
{
	GUI::CComboCtrl* pComboDate = dynamic_cast<GUI::CComboCtrl*>(m_cfgDateTimeList.GetItemWnd(ROW_DATE_FORMAT,1));
	GUI::CComboCtrl* pComboTime = dynamic_cast<GUI::CComboCtrl*>(m_cfgDateTimeList.GetItemWnd(ROW_TIME_FORMAT,1));
	assert(pComboDate);
	assert(pComboTime);

	//用于当放弃或者不保存退出时，回复日期格式。
	unsigned long modeDate = CDateCtrl::DateMode(pComboDate->GetCurItemData());
	if (modeDate != CDateCtrl::GetDateMode())
	{
		CDateCtrl::SetDateMode(modeDate, false);
	}

	unsigned long modeTime = CTimeCtrl::TimeMode(pComboTime->GetCurItemData());
	if (modeTime != CTimeCtrl::GetTimeMode())
	{
		CTimeCtrl::SetTimeMode(modeTime, false);
	}

	return 0;
}

void CCfgNewDateTimeDlg::SetModifyTime(bool modify, bool bRefresh)
{
	GUI::CButton* pBtn = dynamic_cast<GUI::CButton*>(m_cfgDateTimeList.GetItemWnd(ROW_MODIFY_TIME,1));
	assert(pBtn);
	m_bStopRefreshTime = modify;
	if(pBtn->IsEnable() != m_bStopRefreshTime)
	{
		pBtn->SetEnable(m_bStopRefreshTime, bRefresh);
	}
}

unsigned long CCfgNewDateTimeDlg::OnPreCommand(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	if( m_bUpdateTimeNow )
	{
		//在NTP即时同步的时候屏蔽其他消息响应
		return KEY_VIRTUAL_MSG_STOP;
	}
	else
	{
		return CCfgDlg::OnPreCommand( ctrlID, keyID ,xPos, yPos );
	}

}

void CCfgNewDateTimeDlg::SetTip()
{
	GUI::CListCtrl& list = m_cfgDateTimeList;

#if defined(__DVR_BASIC__) && !defined(__TW01_RILI__)
	AddTip(list.GetItemWnd(ROW_TIME_SYNC,1)->GetID(),E_STR_ID_TIP_01_47);
	AddTip(list.GetItemWnd(ROW_TIME_UPDATENOW,1)->GetID(),E_STR_ID_TIP_01_35);
#endif
	AddTip(list.GetItemWnd(ROW_MODIFY_TIME,1)->GetID(),E_STR_ID_TIP_01_34);
}

void CCfgNewDateTimeDlg::OnChangeFocus(unsigned long oldID, unsigned long newID)
{
	if( GetShowTipType(TIP_DFOCUS) )
	{
		GUI::CComboCtrl* pComboDate = dynamic_cast<GUI::CComboCtrl*>(m_cfgDateTimeList.GetItemWnd(ROW_DATE_FORMAT,1));
		GUI::CComboCtrl* pComboTime = dynamic_cast<GUI::CComboCtrl*>(m_cfgDateTimeList.GetItemWnd(ROW_TIME_FORMAT,1));
		assert(pComboDate);
		assert(pComboTime);

		//if( newID == pComboDate->GetID() )
		//{
		//	unsigned long time = GetCurrTime32();
		//	std::string strTime;
		//	CTimeCtrlEx::GetTimeText(strTime,time,CTimeCtrlEx::TIMEDISP_DATE,static_cast<DATE_MODE>(pComboDate->GetCurItemData()),TIME_MODE_24);
		//	std::string strTip = m_pStrTable->String(E_STR_ID_TIP_01_42);
		//	strTip += strTime;
		//	MessageOut(GetTopestWnd(),strTip.c_str(),TIP_DFOCUS);
		//	return;
		//}

		//if( newID == pComboTime->GetID() )
		//{
		//	unsigned long time = GetCurrTime32();
		//	std::string strTime;
		//	CTimeCtrlEx::GetTimeText(strTime,time,CTimeCtrlEx::TIMEDISP_TIME,DATE_MODE_YMD,static_cast<TIME_MODE>(pComboTime->GetCurItemData()));
		//	std::string strTip = m_pStrTable->String(E_STR_ID_TIP_01_43);
		//	strTip += strTime;
		//	MessageOut(GetTopestWnd(),strTip.c_str(),TIP_DFOCUS);
		//	return;
		//}
			
		GUI::CComboCtrl* pCombo = dynamic_cast<GUI::CComboCtrl*>(m_cfgDateTimeList.GetItemWnd(ROW_TIME_ZONE,1));
		assert(pCombo);
		if( newID == pCombo->GetID() )
		{
			int sel = pCombo->GetCurItemData();
			MessageOut(GetTopestWnd(),static_cast<ENUM_STRING_ID>(E_STR_ID_TIP_01+sel),TIP_DFOCUS);
			return;
		}
	}

	//继续向上级窗口传递消息
	CCfgDlg::OnChangeFocus(oldID,newID);
}

//void CCfgNewDateTimeDlg::CheckUI(bool bRefresh)
//{
//	GUI::CListCtrl& list = m_cfgDateTimeList;
//	
//	//dst
//	GUI::CCheckCtrl* pCheck = dynamic_cast<GUI::CCheckCtrl*>(list.GetItemWnd(ROW_TIME_DST,1));
//	assert( pCheck );
//
//	list.GetItemWnd(ROW_TIME_DST_SET,1)->SetEnable( pCheck->IsCheck(), bRefresh );
//}

bool CCfgNewDateTimeDlg::ShowData(NCFG_ITEM_ID cfgID, const BYTE* pData, int len, bool bRefresh)
{
	assert(NULL != pData);
	assert(len > 0);

	BYTE* pTempData = NULL;
	unsigned long tempDataLen = 0;

	switch (cfgID)
	{
	case NCFG_ITEM_DATE_FORMAT:
		{
			assert(len == sizeof(unsigned long));
			const unsigned long* pValue = reinterpret_cast<const unsigned long*>(pData);

			GUI::CComboCtrl* pComboCtrl = dynamic_cast<GUI::CComboCtrl*>( m_cfgDateTimeList.GetItemWnd( ROW_DATE_FORMAT, 1) );
			assert( NULL != pComboCtrl );
			pComboCtrl->SetCurItemData( *pValue, bRefresh );
		}
		return true;
		break;
	case NCFG_ITEM_HOUR_FORMAT:
		{
			assert(len == sizeof(unsigned long));
			const unsigned long* pValue = reinterpret_cast<const unsigned long*>(pData);

			GUI::CComboCtrl* pComboCtrl = dynamic_cast<GUI::CComboCtrl*>( m_cfgDateTimeList.GetItemWnd( ROW_TIME_FORMAT, 1) );
			assert( NULL != pComboCtrl );
			pComboCtrl->SetCurItemData( *pValue, bRefresh );
		}
		return true;
		break;
#if defined(__DVR_BASIC__) && !defined(__TW01_RILI__)
	case NCFG_ITEM_TIME_SYNC:
		{
			assert(len == sizeof(unsigned long));
			const unsigned long* pValue = reinterpret_cast<const unsigned long*>(pData);

			GUI::CCheckCtrl* pCheck = dynamic_cast<GUI::CCheckCtrl*>( m_cfgDateTimeList.GetItemWnd( ROW_TIME_SYNC, 1 ) );
			assert(NULL != pCheck );
			pCheck->SetCheck((0 != (*pValue)), bRefresh);
			SetEnableUI( ROW_TIME_SYNC, bRefresh );
		}
		return true;
		break;
#endif
	case NCFG_ITEM_TIME_ZONE:
		{
			assert(len == sizeof(long));
			const unsigned long* pValue = reinterpret_cast<const unsigned long*>(pData);

			GUI::CComboCtrl* pCombo = dynamic_cast<GUI::CComboCtrl*>( m_cfgDateTimeList.GetItemWnd( ROW_TIME_ZONE, 1) );
			assert(NULL != pCombo );
			pCombo->SetCurItemData(*pValue, bRefresh);
			//int count = sizeof(g_zoneValue) / sizeof(g_zoneValue[0]);
			//for (int i = 0; i < count; ++i)
			//{
			//	if (*pValue == g_zoneValue[i])
			//	{
			//		pCombo->SetCurItem(i, bRefresh);
			//		break;
			//	}
			//}
		}
		return true;
		break;
#if defined(__DVR_BASIC__) && !defined(__TW01_RILI__)
	case NCFG_ITEM_TIME_SERVER:
		{
			assert(len == sizeof(NCFG_INFO_LONG_NAME));

			CCfgDlg::GetConfig( NCFG_ITEM_TIME_SERVER_MAX_LEN, &pTempData, tempDataLen );
			assert( tempDataLen == sizeof(unsigned long) );
			unsigned long max_len = *reinterpret_cast<unsigned long*>(pTempData);

			const NCFG_INFO_LONG_NAME* pName = reinterpret_cast<const NCFG_INFO_LONG_NAME*>(pData);

			GUI::CEditCtrl* pEdit = dynamic_cast<GUI::CEditCtrl*>( m_cfgDateTimeList.GetItemWnd( ROW_TIME_SERVER, 1 ) );
			assert(NULL != pEdit );
			pEdit->SetMaxStringLen( max_len);
			pEdit->SetCaption(pName->name, bRefresh);
		}
		return true;
		break;
#endif
	default:
		return false;
		break;
	}
}

bool CCfgNewDateTimeDlg::SetData(NCFG_ITEM_ID cfgID, BYTE*& pData, int& len)  const
{
	assert(NULL != pData);
	assert(len > 0);

	switch (cfgID)
	{
	case NCFG_ITEM_DATE_FORMAT:
		{
			assert(len == sizeof(unsigned long));
			unsigned long* pValue = reinterpret_cast<unsigned long*>(pData);

			const GUI::CComboCtrl* pComboCtrl = dynamic_cast<const GUI::CComboCtrl*>( m_cfgDateTimeList.GetItemWnd( ROW_DATE_FORMAT, 1) );
			assert( NULL != pComboCtrl );
			*pValue = static_cast<unsigned char>(pComboCtrl->GetCurItemData());
		}
		return true;
		break;
	case NCFG_ITEM_HOUR_FORMAT:
		{
			assert(len == sizeof(unsigned long));
			unsigned long* pValue = reinterpret_cast<unsigned long*>(pData);

			const GUI::CComboCtrl* pComboCtrl = dynamic_cast<const GUI::CComboCtrl*>( m_cfgDateTimeList.GetItemWnd( ROW_TIME_FORMAT, 1) );
			assert( NULL != pComboCtrl );
			*pValue = static_cast<unsigned char>(pComboCtrl->GetCurItemData());
		}
		return true;
		break;
#if defined(__DVR_BASIC__) && !defined(__TW01_RILI__)
	case NCFG_ITEM_TIME_SYNC:
		{
			assert(len == sizeof(unsigned long));
			unsigned long* pValue = reinterpret_cast<unsigned long*>(pData);

			const GUI::CCheckCtrl* pCheck = dynamic_cast<const GUI::CCheckCtrl*>( m_cfgDateTimeList.GetItemWnd( ROW_TIME_SYNC, 1 ) );
			assert(NULL != pCheck );

			if (pCheck->IsCheck())
			{
				*pValue = 1;
			} 
			else
			{
				*pValue = 0;
			}
		}
		return true;
		break;
#endif
	case NCFG_ITEM_TIME_ZONE:
		{
			assert(len == sizeof(long));
			unsigned long* pValue = reinterpret_cast<unsigned long*>(pData);

			const GUI::CComboCtrl* pCombo = dynamic_cast<const GUI::CComboCtrl*>( m_cfgDateTimeList.GetItemWnd( ROW_TIME_ZONE, 1) );
			assert(NULL != pCombo );
			//int sel = pCombo->GetCurItem();
			//assert(sel < (sizeof(g_zoneValue) / sizeof(g_zoneValue[0])));

			//*pValue = g_zoneValue[sel];

			*pValue = pCombo->GetCurItemData();
		}
		return true;
		break;
#if defined(__DVR_BASIC__) && !defined(__TW01_RILI__)
	case NCFG_ITEM_TIME_SERVER:
		{
			assert(len == sizeof(NCFG_INFO_LONG_NAME));
			NCFG_INFO_LONG_NAME* pName = reinterpret_cast<NCFG_INFO_LONG_NAME*>(pData);

			const GUI::CEditCtrl* pEdit = dynamic_cast<const GUI::CEditCtrl*>( m_cfgDateTimeList.GetItemWnd( ROW_TIME_SERVER, 1 ) );
			assert(NULL != pEdit );
			strcpy(pName->name, pEdit->GetCaption().c_str());
		}
		return true;
		break;
#endif
	default:
		return false;
		break;
	}
}

BEGIN_MSG_MAP(CCfgNewDateTimeDlg, CCfgDlg)
	ON_MSG_EX(m_cfgDateTimeList.GetID(), KEY_VIRTUAL_MSG_DEFAULT_TAG, OnClickList)
	ON_MSG(m_cfgDateTimeList.GetID(), KEY_VIRTUAL_ADD, OnMouseWheel)
	ON_MSG(m_cfgDateTimeList.GetID(), KEY_VIRTUAL_DEC, OnMouseWheel)
END_MSG_MAP()

unsigned long CCfgNewDateTimeDlg::OnClickList(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	unsigned short row = (keyID >> 16)&0x7fff;	//行
	unsigned short col = keyID&0xffff;			//列

	//show tip
	if( GetShowTipType(TIP_DFOCUS) )
	{
		GUI::CComboCtrl* pCombo = NULL;

		if( (ROW_TIME_ZONE == row) && (1 == col) )
		{
			pCombo = dynamic_cast<GUI::CComboCtrl*>(m_cfgDateTimeList.GetItemWnd(ROW_TIME_ZONE,1));
			int sel = pCombo->GetCurItemData();
			MessageOut(GetTopestWnd(),static_cast<ENUM_STRING_ID>(E_STR_ID_TIP_01+sel),TIP_DFOCUS);
		}
	}

	if ((ROW_DATE_FORMAT == row) && (1 == col))
	{
		const GUI::CComboCtrl* pComboCtrl = dynamic_cast<const GUI::CComboCtrl*>( m_cfgDateTimeList.GetItemWnd( ROW_DATE_FORMAT, 1) );
		assert( NULL != pComboCtrl );

		unsigned long value = static_cast<unsigned long>(pComboCtrl->GetCurItemData());
		unsigned long mode = CDateCtrl::DateMode(value);

		if (mode != CDateCtrl::GetDateMode())
		{
			CDateCtrl::SetDateMode(mode, true);
		}
	}
	
	if ((ROW_TIME_FORMAT == row) && (1 == col))
	{
		const GUI::CComboCtrl* pComboCtrl = dynamic_cast<const GUI::CComboCtrl*>( m_cfgDateTimeList.GetItemWnd( ROW_TIME_FORMAT, 1) );
		assert( NULL != pComboCtrl );

		unsigned long value = static_cast<unsigned long>(pComboCtrl->GetCurItemData());
		unsigned long mode = CTimeCtrl::TimeMode(value);

		if (mode != CTimeCtrl::GetTimeMode())
		{
			CTimeCtrl::SetTimeMode(mode, true);
		}
	}
	
#if defined(__DVR_BASIC__) && !defined(__TW01_RILI__)
	if( ( ROW_TIME_SYNC == row ) && ( 1 == col ) )
	{
		SetEnableUI( ROW_TIME_SYNC, true );
	}

	if( ( ROW_TIME_UPDATENOW == row ) && ( 1 == col ) )
	{
		NCFG_INFO_LONG_NAME NTPServerName;
		const GUI::CEditCtrl* pEdit = dynamic_cast<const GUI::CEditCtrl*>( m_cfgDateTimeList.GetItemWnd( ROW_TIME_SERVER, 1 ) );
		assert(NULL != pEdit );
		strcpy(NTPServerName.name, pEdit->GetCaption().c_str());
		
		m_bUpdateTimeNow = true;
	
		
		MessageOut( GetTopestWnd(), E_STR_ID_PLEASE_WAIT, TIP_ERR );

		MESSAGE_DATA msgDate;
		memset( &msgDate, 0, sizeof(MESSAGE_DATA));	
		CMessageMan::Instance()->SendMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_SYN_TIME_NOW, reinterpret_cast<unsigned char *>(NTPServerName.name), sizeof(NTPServerName.name), msgDate);
		PACKCMD *pCmd = reinterpret_cast<PACKCMD *>(msgDate.pData);
		assert( NULL != pCmd );
		if( NULL != pCmd )
		{
			ENUM_STRING_ID strID = E_STR_ID_NULL;
			if( CMD_REPLY_SYN_TIME_NOW_SUCC == pCmd->cmdType )
			{
				MessageOut( GetTopestWnd(), E_STR_ID_MESSAGE_73, TIP_ERR, false );
			}
			else
			{
				MessageOut( GetTopestWnd(), E_STR_ID_MESSAGE_74, TIP_ERR, false );
			}
		}
		else
		{
			assert( false );
		}
		
		CMessageMan::Instance()->ReleaseMsgBuff( msgDate );
		
		GetTopestWnd()->Repaint();
		
		m_bUpdateTimeNow = false;
	}
#endif

	if ((ROW_MODIFY_TIME == row) && (1 == col))
	{
		if (DLG_OK == MessageBox(m_pStrTable->String(E_STR_ID_MESSAGE_03), m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK | DEFAULT_BTN_CANCEL | DEFAULT_ICON_WARNING))
		{
			//调整时间
			unsigned long time = m_currTime;
			GUI::CDateCtrl* pDate = dynamic_cast<GUI::CDateCtrl*>( m_cfgDateTimeList.GetItemWnd(ROW_SYS_DATE, 1) );
			GUI::CTimeCtrl *pTime = dynamic_cast<GUI::CTimeCtrl*>( m_cfgDateTimeList.GetItemWnd(ROW_SYS_TIME, 1) );
			if (NULL != pDate && NULL != pTime)
			{
				tm tmpTm;
				memset(&tmpTm,0,sizeof(tm));
				pDate->GetDate(tmpTm.tm_year,tmpTm.tm_mon,tmpTm.tm_mday);
				pTime->GetTime(tmpTm.tm_hour,tmpTm.tm_min,tmpTm.tm_sec);
				gui_trace(true,printf("get from ctrl, time=%d-%d-%d %d:%d:%d\n",tmpTm.tm_year,tmpTm.tm_mon+1,tmpTm.tm_mday,tmpTm.tm_hour,tmpTm.tm_min,tmpTm.tm_sec));
				tmpTm.tm_year -= 1900;
				time =TmToDVRTime32(tmpTm);
				gui_trace(true,printf("transform to 32 dvr time=%d,text=%s\n",time,CTimeCtrl::GetTime32Text(time).c_str()));
			}
			else
			{
				assert(false);
			}

			CMessageMan::Instance()->PostMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_L_CHANGE_TIME, (unsigned char *)&time, sizeof(unsigned long));


			//m_bStopRefreshTime = false;
			SetModifyTime(false, true);
			m_currTime = GetCurrTime32();
		}

		return KEY_VIRTUAL_MSG_STOP;
	}
	
	if ((ROW_SYS_TIME == row) && (1 == col) )
	{
		//m_bStopRefreshTime = true;
		SetModifyTime(true, true);
		return KEY_VIRTUAL_MSG_STOP;
	}
	
	if ( (ROW_SYS_DATE == row) && (1 == col) )
	{
		//m_bStopRefreshTime = true;
		SetModifyTime(true, true);
		return KEY_VIRTUAL_MSG_STOP;
	}
	
	{
		if (!IsModify())
		{
			GUI::CButton* pBtn = NULL;

			switch (row)
			{
			case ROW_DATE_FORMAT:
				if (IsChange(NCFG_ITEM_DATE_FORMAT))
				{
					SetModify(true);
				}
				break;
			case ROW_TIME_FORMAT:
				if(IsChange(NCFG_ITEM_HOUR_FORMAT))
				{
					SetModify(true);
				}
			case ROW_TIME_ZONE:
				if (IsChange(NCFG_ITEM_TIME_ZONE))
				{
					SetModify(true);
				}
				break;
#if defined(__DVR_BASIC__) && !defined(__TW01_RILI__)
			case ROW_TIME_SYNC:
				if (IsChange(NCFG_ITEM_TIME_SYNC))
				{
					SetModify(true);
				}
				break;
			case ROW_TIME_SERVER:
				if (IsChange(NCFG_ITEM_TIME_SERVER))
				{
					SetModify(true);
				}
				break;
#endif
			default:
				return KEY_VIRTUAL_MSG_STOP;
				break;
			}

			return KEY_VIRTUAL_MSG_CONTINUE;
		} 
		else
		{
			return KEY_VIRTUAL_MSG_STOP;
		}
	}
}

void CCfgNewDateTimeDlg::OnIdle()
{
	//设置时间
	unsigned long time = GetCurrTime32();

	if (!m_bStopRefreshTime && (1 <= (time - m_currTime)))
	{
		m_currTime = time;

		//刷新界面的时间
		GUI::CDateCtrl* pDate = dynamic_cast<GUI::CDateCtrl*>( m_cfgDateTimeList.GetItemWnd(ROW_SYS_DATE,1) );
		GUI::CTimeCtrl *pTime = dynamic_cast<GUI::CTimeCtrl*>( m_cfgDateTimeList.GetItemWnd(ROW_SYS_TIME, 1) );
		if ( NULL != pDate && NULL != pTime)
		{
			tm tmpTm = DVRTime32ToTm(time);
			pDate->SetDate(tmpTm.tm_year+1900,tmpTm.tm_mon,tmpTm.tm_mday,true);
			pTime->SetTime(tmpTm.tm_hour,tmpTm.tm_min,tmpTm.tm_sec,true);
		}
		else
		{
			assert(false);
		}
	}
}

void CCfgNewDateTimeDlg::SetEnableUI( LIST_ROW rowNum, bool brefalsh )
{
	switch( rowNum )
	{
#if defined(__DVR_BASIC__) && !defined(__TW01_RILI__)
	case ROW_TIME_SYNC:
		{
			GUI::CCheckCtrl* pCheck = dynamic_cast<GUI::CCheckCtrl*>( m_cfgDateTimeList.GetItemWnd( ROW_TIME_SYNC, 1 ) );
			bool enable = pCheck->IsCheck();
			GUI::CEditCtrl *pEdit = dynamic_cast<GUI::CEditCtrl*>( m_cfgDateTimeList.GetItemWnd( ROW_TIME_SERVER, 1 ) );
			pEdit->SetEnable( enable, brefalsh );
			GUI::CButton* pBtn = dynamic_cast<GUI::CButton *>( m_cfgDateTimeList.GetItemWnd( ROW_TIME_UPDATENOW, 1 ) );
			pBtn->SetEnable( enable, brefalsh );
			
		}
		break;
#endif
	default:
		break;
	}
}

unsigned long CCfgNewDateTimeDlg::OnMouseWheel()
{
	if( !m_bStopRefreshTime )
	{
		GUI::CDateCtrl* pDate = dynamic_cast<GUI::CDateCtrl*>( m_cfgDateTimeList.GetItemWnd(ROW_SYS_DATE,1) );
		GUI::CTimeCtrl *pTime = dynamic_cast<GUI::CTimeCtrl*>( m_cfgDateTimeList.GetItemWnd(ROW_SYS_TIME, 1) );

		tm tmpTm;
		memset(&tmpTm,0,sizeof(tm));
		pDate->GetDate(tmpTm.tm_year,tmpTm.tm_mon,tmpTm.tm_mday);
		pTime->GetTime(tmpTm.tm_hour,tmpTm.tm_min,tmpTm.tm_sec);
		tmpTm.tm_year -= 1900;
		unsigned long time = TmToDVRTime32(tmpTm);

		if(m_currTime != time)
		{
			//m_bStopRefreshTime = true;
			SetModifyTime(true, true);
		}
	}

	return KEY_VIRTUAL_MSG_CONTINUE;
}

//end
