/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : ZXX
** Date         : 2013-11-26
** Name         : WzdNatTimeDlg.h
** Version      : 1.0
** Description  : 脧貌碌录脪鲁脙忙露脭禄掳驴貌
** Modify Record:
***********************************************************************/

#include "WzdNewNatTimeDlg.h"
#include "WzdNewNatManDlg.h"
//////////////////////////////////////////////////////////////////////////
extern std::string g_zoneName[]; 
extern LANGUAGE 	g_language; 

//////////////////////////////////////////////////////////////////////////
// 碌脷脪禄脪鲁脙忙
//////////////////////////////////////////////////////////////////////////
static int g_zoneValue[]={720, 660, 600, 540, 480, 420, 360, 300, 270, 240, 
210, 180, 120, 60, 0, 60, 120, 180, 210, 240, 
270, 300, 330, 345, 360, 390, 420, 480, 540, 570, 
600, 660, 720, 780};

CWzdNewNatTimeDlg::CWzdNewNatTimeDlg() : m_pNatTimeWzdInfo(NULL)
{
	m_bModifyTime = false;
	m_bStopRefreshTime = false;

	m_pre_sys_tm = 0;
	m_gmt_user_tm = 0;
	m_refresh_count_tm = 0;
	m_bUpdateTimeNow = false;
}

CWzdNewNatTimeDlg::~CWzdNewNatTimeDlg()
{

}

void  CWzdNewNatTimeDlg::InitCtrlInfo()
{
	GUI::CListCtrl& list = m_listTimeCtrl;

	GUI::CComboCtrl*pCombo = dynamic_cast<GUI::CComboCtrl*>(list.GetItemWnd(ROW_DATE_FORMAT, 1));
	pCombo->AddItem("YY-MM-DD", DATE_MODE_YMD);
	pCombo->AddItem("MM-DD-YY", DATE_MODE_MDY);
	pCombo->AddItem("DD-MM-YY", DATE_MODE_DMY);
	pCombo->SetCurItemData(m_pNatTimeWzdInfo->DateMode, false);

	pCombo = dynamic_cast<GUI::CComboCtrl*>(list.GetItemWnd( ROW_TIME_FORMAT, 1 ));
	pCombo->AddItem("12 Hour",TIME_MODE_12);
	pCombo->AddItem("24 Hour",TIME_MODE_24);
	pCombo->SetCurItemData(m_pNatTimeWzdInfo->TimeMode, false);

	pCombo = dynamic_cast<GUI::CComboCtrl*>(list.GetItemWnd(ROW_TIME_ZONE, 1));
	int count = TIME_ZONE_GMT_A13 + 1;
	for(int i = 0; i < count; i++)
	{
		int zone = E_STR_ID_TIP_01;
		zone += i;
		ENUM_STRING_ID id;
		id = ENUM_STRING_ID(zone);
		if(id == E_STR_ID_TIP_01_04 && g_language == LANGUAGE_ENGLISH)
		{		
			pCombo->AddItem(m_pStrTable->String(E_STR_ID_TIP_01_04_01), i);
		}
		else
		{
			pCombo->AddItem(/*g_zoneName[i].c_str()*/m_pStrTable->String(id), i);
		}
	}
	pCombo->SetCurItemData(m_pNatTimeWzdInfo->TimeZone, false);

	//脢卤录盲
	LONGLONG currTime = GetCurrTime();
	m_pre_sys_tm = currTime;

	if(m_pNatTimeWzdInfo->TimeZone >= TIME_ZONE_GMT)
	{
		m_gmt_user_tm = currTime - static_cast<LONGLONG>(g_zoneValue[m_pNatTimeWzdInfo->TimeZone]) * 60 * 1000000;
	}
	else
	{
		m_gmt_user_tm = currTime + static_cast<LONGLONG>(g_zoneValue[m_pNatTimeWzdInfo->TimeZone]) * 60 * 1000000;
	}

	RefreshDateTime(m_gmt_user_tm, m_pNatTimeWzdInfo->TimeZone, false);

	//路镁脦帽脝梅脥卢虏陆潞脥路镁脦帽脝梅碌脴脰路
	GUI::CCheckCtrl* pCheck = dynamic_cast<GUI::CCheckCtrl*>(list.GetItemWnd( ROW_TIME_SYNC, 1 ));
	pCheck->SetCheck(m_pNatTimeWzdInfo->sync,false);

	GUI::CEditCtrl *pEdit = dynamic_cast<GUI::CEditCtrl*>(list.GetItemWnd( ROW_TIME_SERVER, 1 ));

	NCFG_INFO_LONG_NAME pName = m_pNatTimeWzdInfo->timeServer;
	pEdit->SetCaption(pName.name,false);
}

void CWzdNewNatTimeDlg::SetWzdInfo(NAT_WIZARD_INFO* pWzdInfo)
{
	m_pNatTimeWzdInfo = pWzdInfo;
}

void CWzdNewNatTimeDlg::ShowWzdData(bool bRefresh)
{
	SetEnableUI(ROW_TIME_SYNC,false);
	m_bStopRefreshTime = false;
}

void CWzdNewNatTimeDlg::GetWzdData(bool bNext)
{
	GUI::CListCtrl& list = m_listTimeCtrl;

	GUI::CComboCtrl*pCombo = dynamic_cast<GUI::CComboCtrl*>(list.GetItemWnd(ROW_DATE_FORMAT, 1));
	m_pNatTimeWzdInfo->DateMode = pCombo->GetCurItemData();

	pCombo = dynamic_cast<GUI::CComboCtrl*>(list.GetItemWnd(ROW_TIME_FORMAT, 1));
	m_pNatTimeWzdInfo->TimeMode = pCombo->GetCurItemData();

	pCombo = dynamic_cast<GUI::CComboCtrl*>(list.GetItemWnd(ROW_TIME_ZONE, 1));
	m_pNatTimeWzdInfo->TimeZone = pCombo->GetCurItemData();

	GUI::CCheckCtrl* pCheck  = dynamic_cast<GUI::CCheckCtrl*>(list.GetItemWnd(ROW_TIME_SYNC, 1));
	if(pCheck->IsCheck())
	{
		m_pNatTimeWzdInfo ->sync = 1;
	}
	else
	{
		m_pNatTimeWzdInfo ->sync = 0;
	}

	GUI::CEditCtrl*  pEdit = dynamic_cast<GUI::CEditCtrl*>(list.GetItemWnd(ROW_TIME_SERVER, 1));
	strcpy(m_pNatTimeWzdInfo->timeServer.name,pEdit->GetCaption().c_str());
}

void CWzdNewNatTimeDlg::OnLeavePage()
{
	m_bStopRefreshTime = false;
}

bool CWzdNewNatTimeDlg::GetCurrZoneTimeInfo(ZONE_TIME_INFO& zoneTimeInfo, int zone)
{
	if(m_bModifyTime)
	{
		LONGLONG zone_tm = 0;

		if(zone >= TIME_ZONE_GMT)
		{
			zone_tm = m_gmt_user_tm + static_cast<LONGLONG>(g_zoneValue[zone]) * 60 * 1000000;
		}
		else
		{
			zone_tm = m_gmt_user_tm - static_cast<LONGLONG>(g_zoneValue[zone]) * 60 * 1000000;
		}

		tm tmpTm = DVRTimeToTm(zone_tm);
		zoneTimeInfo.year = tmpTm.tm_year + 1900;
		zoneTimeInfo.month = tmpTm.tm_mon;
		zoneTimeInfo.mday = tmpTm.tm_mday;
		zoneTimeInfo.hour = tmpTm.tm_hour;
		zoneTimeInfo.min = tmpTm.tm_min;
		zoneTimeInfo.sec = tmpTm.tm_sec;

		return true;
	}

	return false;
}

void CWzdNewNatTimeDlg::OnInitial()
{
	CWzdNatPageDlg::OnInitial();
	//////////////////////////////////////////////////////////////////////////
	GUI::CListCtrl& list = m_listTimeCtrl;
	GUI::CWnd* pWnd = NULL;
	GUI::CEditCtrl*  pEdit = NULL;
	GUI::CComboCtrl* pCombo = NULL;
	GUI::CDateCtrl* pDate = NULL;
	GUI::CTimeCtrl* pTime = NULL;
	GUI::CCheckCtrl* pCheck = NULL;

	unsigned short x = m_length[DLG_OFFSET_SUB_LEFT];
	unsigned short y = m_length[DLG_OFFSET_SUB_TOP];
	unsigned short width = m_rect.Width() - 2 * m_length[DLG_OFFSET_SUB_LEFT];
	unsigned short height = m_rect.Height() - y - m_length[DLG_OFFSET_SUB_BOTTOM];
	unsigned short offsetEdge = 4;
	int maxListRow = (height - m_length[LIST_HEAD_HEIGHT]) / m_length[LIST_ITEM_HEIGHT]; //List驴貌麓酶脥路
	int numChnn = 7;
	if( numChnn > maxListRow )
	{
		offsetEdge = m_length[SCROLL_WIDTH] + 8;
	}

	list.Create(GUI::CWnd::GetRandWndID(), x, y, width, height, this, 0, 1, false);
	list.AddColumn(NULL, width/2);
	list.AddColumn(NULL, width/2 - offsetEdge );
	list.AddItem(ROW_SYSTEM_TIME, 0, m_pStrTable->String(E_STR_ID_SYSTEM_TIME));
	pTime = dynamic_cast<GUI::CTimeCtrl*>(list.AddItem(ROW_SYSTEM_TIME, 1, WND_TYPE_TIME,m_length[DATE_CTRL_WIDTH],m_length[DATE_CTRL_HEIGHT]));
	pTime->AddToNotifyList();
	assert(pTime);

	list.AddItem(ROW_DATE_FORMAT, 0, m_pStrTable->String(E_STR_ID_DATE_FORMAT));
	pCombo = dynamic_cast<GUI::CComboCtrl*>(list.AddItem(ROW_DATE_FORMAT, 1, WND_TYPE_COMBO));
	assert(pCombo);	

	list.AddItem(ROW_TIME_FORMAT, 0, m_pStrTable->String(E_STR_ID_TIME_FORMAT));
	pCombo = dynamic_cast<GUI::CComboCtrl*>(list.AddItem(ROW_TIME_FORMAT, 1, WND_TYPE_COMBO));
	assert(pCombo);

	list.AddItem(ROW_TIME_ZONE, 0,  m_pStrTable->String(E_STR_ID_TIME_ZONE));
	pCombo = dynamic_cast<GUI::CComboCtrl*>(list.AddItem(ROW_TIME_ZONE, 1, WND_TYPE_COMBO));
	pCombo->SetMaxStringLen( 55 );
	assert(pCombo);

	list.AddItem(ROW_SYSTEM_DATE, 0,m_pStrTable->String(E_STR_ID_SYSTEM_DATE));
	pDate = dynamic_cast<GUI::CDateCtrl*>(list.AddItem(ROW_SYSTEM_DATE, 1, WND_TYPE_DATE,m_length[DATE_CTRL_WIDTH],m_length[DATE_CTRL_HEIGHT]));
	pDate->AddToNotifyList();
	assert(pDate);
#ifdef __DVR_BASIC__
	list.AddItem(ROW_TIME_SYNC, 0,m_pStrTable->String(E_STR_ID_TIP_01_47_01));
	pCheck = dynamic_cast<GUI::CCheckCtrl*>(list.AddItem(ROW_TIME_SYNC, 1, WND_TYPE_CHECK));
	assert(pCheck);

	list.AddItem(ROW_TIME_SERVER, 0, m_pStrTable->String(E_STR_ID_NTP_SERVER_01));
	pEdit = dynamic_cast<GUI::CEditCtrl*>(list.AddItem(ROW_TIME_SERVER, 1, WND_TYPE_EDIT));
	assert(pEdit);
	pEdit->SetEnable(false,false);

	GUI::CButton *pBtn = dynamic_cast<GUI::CButton*>( list.AddItem( ROW_TIME_UPDATENOW, 1, WND_TYPE_BUTTON, m_length[BTN_WIDTH] + 50, m_length[BTN_HEIGHT]) );
	assert( pBtn );
	pBtn->SetCaption( m_pStrTable->String(E_STR_ID_UPDATE_NOW), false);
	pBtn->SetEnable(false,false);

#endif
	list.CreateListFocusMap();
}

void CWzdNewNatTimeDlg::OnIdle()
{
	CWzdNatPageDlg::OnIdle();

	LONGLONG curr_tm = GetCurrTime();

	if(!m_bStopRefreshTime)
	{
		LONGLONG off_tm = 0;
		if(curr_tm > m_pre_sys_tm)
		{
			off_tm = curr_tm - m_pre_sys_tm;
			m_gmt_user_tm += off_tm;

		}
		else
		{
			off_tm = m_pre_sys_tm - curr_tm;
			m_gmt_user_tm -= off_tm;

		}

		m_refresh_count_tm += off_tm;
		if(m_refresh_count_tm > 1000000)
		{
			GUI::CComboCtrl* pCombo = dynamic_cast<GUI::CComboCtrl*>(m_listTimeCtrl.GetItemWnd(ROW_TIME_ZONE, 1));
			int zone = static_cast<unsigned int>(pCombo->GetCurItemData());

			RefreshDateTime(m_gmt_user_tm, zone, true);

			m_refresh_count_tm = 0;
		}
	}

	m_pre_sys_tm = curr_tm;
}

bool CWzdNewNatTimeDlg::IsModify()
{
	return m_bModifyTime;
}

void CWzdNewNatTimeDlg::SetModify(bool modify, bool bRefresh)
{
	m_bModifyTime = modify;
}

bool CWzdNewNatTimeDlg::CanModify()
{
	bool ret = false;

	CWzdNewNatManDlg* pWzdManDlg = dynamic_cast<CWzdNewNatManDlg*>(GetParent());
	assert(pWzdManDlg);
	if( pWzdManDlg )
	{
		ret = pWzdManDlg->TryEnterConfig();
	}

	if( !ret )
	{
		//脠莽鹿没脙禄脫脨陆酶脜盲脰脙,脤谩脢戮脨猫脪陋驴脥禄搂露脣脥脣鲁枚脜盲脰脙
		MessageOut(GetTopestWnd(), static_cast<ENUM_STRING_ID>(E_STR_ID_MESSAGE_104), TIP_MSG);
	}

	return ret;
}

void CWzdNewNatTimeDlg::RefreshDateTime(LONGLONG gmt, int zone, bool bRefresh)
{
	GUI::CListCtrl& list = m_listTimeCtrl;

	LONGLONG zone_tm = gmt;

	assert(zone >= 0);
	assert(zone <= TIME_ZONE_GMT_A13);
	if(zone >= TIME_ZONE_GMT)
	{
		zone_tm += static_cast<LONGLONG>(g_zoneValue[zone]) * 60 * 1000000;
	}
	else
	{
		zone_tm -= static_cast<LONGLONG>(g_zoneValue[zone]) * 60 * 1000000;
	}

	tm tmpTm = DVRTimeToTm(zone_tm);

	GUI::CDateCtrl* pDateCtrl = dynamic_cast<GUI::CDateCtrl*>(list.GetItemWnd(ROW_SYSTEM_DATE, 1));
	pDateCtrl->SetDate(tmpTm.tm_year + 1900, tmpTm.tm_mon, tmpTm.tm_mday, bRefresh);

	GUI::CTimeCtrl* pTimeCtrl = dynamic_cast<GUI::CTimeCtrl*>(list.GetItemWnd(ROW_SYSTEM_TIME, 1));
	pTimeCtrl->SetTime(tmpTm.tm_hour, tmpTm.tm_min, tmpTm.tm_sec, bRefresh);
}

void CWzdNewNatTimeDlg::GetDateTime(LONGLONG& gmt, int zone)
{
	GUI::CListCtrl& list = m_listTimeCtrl;

	GUI::CDateCtrl* pDateCtrl = dynamic_cast<GUI::CDateCtrl*>(list.GetItemWnd(ROW_SYSTEM_DATE, 1));
	GUI::CTimeCtrl* pTimeCtrl = dynamic_cast<GUI::CTimeCtrl*>(list.GetItemWnd(ROW_SYSTEM_TIME, 1));

	tm tmpTm;
	memset(&tmpTm,0,sizeof(tm));
	pDateCtrl->GetDate(tmpTm.tm_year, tmpTm.tm_mon, tmpTm.tm_mday);
	pTimeCtrl->GetTime(tmpTm.tm_hour, tmpTm.tm_min, tmpTm.tm_sec);
	tmpTm.tm_year -= 1900;

	LONGLONG zone_tm = TmToDVRTime(tmpTm);

	if(zone >= TIME_ZONE_GMT)
	{
		gmt = zone_tm - static_cast<LONGLONG>(g_zoneValue[zone]) * 60 * 1000000;
	}
	else
	{
		gmt = zone_tm + static_cast<LONGLONG>(g_zoneValue[zone]) * 60 * 1000000;
	}
}

unsigned long CWzdNewNatTimeDlg::OnPreCommand(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	if( m_bUpdateTimeNow )
	{
		//脭脷NTP录麓脢卤脥卢虏陆碌脛脢卤潞貌脝脕卤脦脝盲脣没脧没脧垄脧矛脫娄
		return KEY_VIRTUAL_MSG_STOP;
	}
	else
	{
		return CDialog::OnPreCommand( ctrlID, keyID ,xPos, yPos );
	}
}

BEGIN_MSG_MAP(CWzdNewNatTimeDlg, CWzdNatPageDlg)
	ON_MSG_EX(m_listTimeCtrl.GetID(), KEY_VIRTUAL_MSG_DEFAULT_TAG, OnClickList)
END_MSG_MAP()

unsigned long CWzdNewNatTimeDlg::OnClickList(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	unsigned short row = (keyID >> 16)&0x7fff;	//脨脨
	unsigned short col = keyID&0xffff;			//脕脨

	GUI::CListCtrl& list = m_listTimeCtrl;

	if(1 == col)
	{
		switch (row)
		{
		case ROW_SYSTEM_TIME:
			{
				const GUI::CComboCtrl* pCombo = dynamic_cast<const GUI::CComboCtrl*>(list.GetItemWnd(ROW_TIME_ZONE, 1));
				assert(NULL != pCombo);

				int zone = static_cast<int>(pCombo->GetCurItemData());

				GetDateTime(m_gmt_user_tm, zone);
				m_bStopRefreshTime = true;

				SetModify(true, false);
			}
			break;
		case ROW_DATE_FORMAT:
			{
				const GUI::CComboCtrl* pComboCtrl = dynamic_cast<const GUI::CComboCtrl*>(list.GetItemWnd(ROW_DATE_FORMAT, 1));
				assert(NULL != pComboCtrl);

				unsigned long value = static_cast<unsigned long>(pComboCtrl->GetCurItemData());
				unsigned long mode = CDateCtrl::DateMode(value);

				if (mode != CDateCtrl::GetDateMode())
				{
					CDateCtrl::SetDateMode(mode, true);
				}
			}
			break;
		case ROW_TIME_FORMAT:
			{
				const GUI::CComboCtrl* pComboCtrl = dynamic_cast<const GUI::CComboCtrl*>(list.GetItemWnd(ROW_TIME_FORMAT, 1));
				assert( NULL != pComboCtrl );

				unsigned long value = static_cast<unsigned long>(pComboCtrl->GetCurItemData());
				unsigned long mode = CTimeCtrl::TimeMode(value);

				if (mode != CTimeCtrl::GetTimeMode())
				{
					CTimeCtrl::SetTimeMode(mode, true);
				}
			}
			break;
		case ROW_TIME_ZONE:
			{
				const GUI::CComboCtrl* pCombo = dynamic_cast<const GUI::CComboCtrl*>(list.GetItemWnd(ROW_TIME_ZONE, 1));
				assert(NULL != pCombo);

				int zone = static_cast<int>(pCombo->GetCurItemData());
				RefreshDateTime(m_gmt_user_tm, zone, true);
			}
			break;
		case ROW_SYSTEM_DATE:
			{
				const GUI::CComboCtrl* pCombo = dynamic_cast<const GUI::CComboCtrl*>(list.GetItemWnd(ROW_TIME_ZONE, 1));
				assert(NULL != pCombo);

				int zone = static_cast<int>(pCombo->GetCurItemData());

				GetDateTime(m_gmt_user_tm, zone);
				m_bStopRefreshTime = true;

				SetModify(true, false);
			}
			break;
		case ROW_TIME_SYNC:
			{
				SetEnableUI(ROW_TIME_SYNC,false);
				SetModify(true, false);
				Repaint();
				break;
			}
		case ROW_TIME_UPDATENOW:
			{
				bool bChangeTimeOK =false;
				NCFG_INFO_LONG_NAME NTPServerName;
				const GUI::CEditCtrl* pEdit = dynamic_cast<const GUI::CEditCtrl*>( m_listTimeCtrl.GetItemWnd( ROW_TIME_SERVER, 1 ) );
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
					if( CMD_REPLY_SYN_TIME_NOW_SUCC == pCmd->cmdType )
					{
						MessageOut( GetTopestWnd(), E_STR_ID_MESSAGE_73, TIP_ERR, false );
						bChangeTimeOK = true;
					}
					else
					{
						MessageOut( GetTopestWnd(), E_STR_ID_MESSAGE_74, TIP_ERR, false );
						bChangeTimeOK = false;
					}
				}
				else
				{
					assert( false );
				}

				CMessageMan::Instance()->ReleaseMsgBuff( msgDate );
				m_bUpdateTimeNow = false;
				m_pNatTimeWzdInfo->UpDateWzdTime = 1;
				SetModify(true, false);
				GetTopestWnd()->Repaint();
				break;
			}
		default:
			break;	
		} 
	}
	return KEY_VIRTUAL_MSG_STOP;
}

void CWzdNewNatTimeDlg::SetEnableUI(  LIST_ROW rowNum, bool brefalsh )
{
	switch( rowNum )
	{
#ifdef __DVR_BASIC__
	case ROW_TIME_SYNC:
		{
			GUI::CCheckCtrl* pCheck = dynamic_cast<GUI::CCheckCtrl*>( m_listTimeCtrl.GetItemWnd( ROW_TIME_SYNC, 1 ) );
			bool enable = pCheck->IsCheck();
			GUI::CEditCtrl *pEdit = dynamic_cast<GUI::CEditCtrl*>( m_listTimeCtrl.GetItemWnd( ROW_TIME_SERVER, 1 ) );
			pEdit->SetEnable( enable, brefalsh );
			GUI::CButton *pBtn = dynamic_cast<GUI::CButton*>( m_listTimeCtrl.GetItemWnd( ROW_TIME_UPDATENOW, 1 ) );
			pBtn->SetEnable( enable, brefalsh );
		}
		break;
#endif
	default:
		break;
	}
}
//end

