/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : ZXX
** Date         : 2012-09-18
** Name         : WzdUS02TimeDlg.h
** Version      : 1.0
** Description  : 向导页面对话框
** Modify Record:
***********************************************************************/

#include "WzdUS02TimeDlg.h"
#include "WzdUS02ManDlg.h"
//////////////////////////////////////////////////////////////////////////
extern std::string g_zoneName[];  

//////////////////////////////////////////////////////////////////////////
// 第一页面
//////////////////////////////////////////////////////////////////////////
static int g_zoneValue[]={720, 660, 600, 540, 480, 420, 360, 300, 270, 240, 
210, 180, 120, 60, 0, 60, 120, 180, 210, 240, 
270, 300, 330, 345, 360, 390, 420, 480, 540, 570, 
600, 660, 720, 780};

CWzdUS02TimeDlg::CWzdUS02TimeDlg() : m_pUS02timeWzdInfo(NULL)
{
	m_bModifyTime = false;
	m_bStopRefreshTime = false;

	m_pre_sys_tm = 0;
	m_gmt_user_tm = 0;
	m_refresh_count_tm = 0;
	m_bUpdateTimeNow = false;
}

CWzdUS02TimeDlg::~CWzdUS02TimeDlg()
{

}

void  CWzdUS02TimeDlg::InitCtrlInfo()
{
	GUI::CListCtrl& list = m_listTimeCtrl;

	GUI::CComboCtrl*pCombo = dynamic_cast<GUI::CComboCtrl*>(list.GetItemWnd(ROW_DATE_FORMAT, 1));
	pCombo->AddItem("YY-MM-DD", DATE_MODE_YMD);
	pCombo->AddItem("MM-DD-YY", DATE_MODE_MDY);
	pCombo->AddItem("DD-MM-YY", DATE_MODE_DMY);
	pCombo->SetCurItemData(m_pUS02timeWzdInfo->DateMode, false);

	pCombo = dynamic_cast<GUI::CComboCtrl*>(list.GetItemWnd( ROW_TIME_FORMAT, 1 ));
	pCombo->AddItem("12 Hour",TIME_MODE_12);
	pCombo->AddItem("24 Hour",TIME_MODE_24);
	pCombo->SetCurItemData(m_pUS02timeWzdInfo->TimeMode, false);

	pCombo = dynamic_cast<GUI::CComboCtrl*>(list.GetItemWnd(ROW_TIME_ZONE, 1));
	int count = TIME_ZONE_GMT_A13 + 1;
	for(int i = 0; i < count; i++)
	{
		pCombo->AddItem(g_zoneName[i].c_str(), i);
	}
	pCombo->SetCurItemData(m_pUS02timeWzdInfo->TimeZone, false);

	//时间
	LONGLONG currTime = GetCurrTime();
	m_pre_sys_tm = currTime;

	if(m_pUS02timeWzdInfo->TimeZone >= TIME_ZONE_GMT)
	{
		m_gmt_user_tm = currTime - static_cast<LONGLONG>(g_zoneValue[m_pUS02timeWzdInfo->TimeZone]) * 60 * 1000000;
	}
	else
	{
		m_gmt_user_tm = currTime + static_cast<LONGLONG>(g_zoneValue[m_pUS02timeWzdInfo->TimeZone]) * 60 * 1000000;
	}

	RefreshDateTime(m_gmt_user_tm, m_pUS02timeWzdInfo->TimeZone, false);

	//服务器同步和服务器地址
	GUI::CCheckCtrl* pCheck = dynamic_cast<GUI::CCheckCtrl*>(list.GetItemWnd( ROW_TIME_SYNC, 1 ));
	pCheck->SetCheck(m_pUS02timeWzdInfo->sync,false);

	GUI::CEditCtrl *pEdit = dynamic_cast<GUI::CEditCtrl*>(list.GetItemWnd( ROW_TIME_SERVER, 1 ));
	
	NCFG_INFO_LONG_NAME pName = m_pUS02timeWzdInfo->timeServer;
	pEdit->SetCaption(pName.name,false);
}

void CWzdUS02TimeDlg::SetWzdInfo(US02_WzdInfo* pWzdInfo)
{
	m_pUS02timeWzdInfo = pWzdInfo;
}

void CWzdUS02TimeDlg::ShowWzdData(bool bRefresh)
{
	SetEnableUI(ROW_TIME_SYNC,false);
	m_bStopRefreshTime = false;
}

void CWzdUS02TimeDlg::GetWzdData(bool bNext)
{
	GUI::CListCtrl& list = m_listTimeCtrl;

	GUI::CComboCtrl*pCombo = dynamic_cast<GUI::CComboCtrl*>(list.GetItemWnd(ROW_DATE_FORMAT, 1));
	m_pUS02timeWzdInfo->DateMode = pCombo->GetCurItemData();

	pCombo = dynamic_cast<GUI::CComboCtrl*>(list.GetItemWnd(ROW_TIME_FORMAT, 1));
	m_pUS02timeWzdInfo->TimeMode = pCombo->GetCurItemData();

	pCombo = dynamic_cast<GUI::CComboCtrl*>(list.GetItemWnd(ROW_TIME_ZONE, 1));
	m_pUS02timeWzdInfo->TimeZone = pCombo->GetCurItemData();

	GUI::CCheckCtrl* pCheck  = dynamic_cast<GUI::CCheckCtrl*>(list.GetItemWnd(ROW_TIME_SYNC, 1));
	if(pCheck->IsCheck())
	{
		m_pUS02timeWzdInfo ->sync = 1;
	}
	else
	{
		m_pUS02timeWzdInfo ->sync = 0;
	}
	
	GUI::CEditCtrl*  pEdit = dynamic_cast<GUI::CEditCtrl*>(list.GetItemWnd(ROW_TIME_SERVER, 1));
	strcpy(m_pUS02timeWzdInfo->timeServer.name,pEdit->GetCaption().c_str());
}

void CWzdUS02TimeDlg::OnLeavePage()
{
	m_bStopRefreshTime = false;
}

bool CWzdUS02TimeDlg::GetCurrZoneTimeInfo(ZONE_TIME_INFO& zoneTimeInfo, int zone)
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

void CWzdUS02TimeDlg::OnInitial()
{
	CWzdUS02PageDlg::OnInitial();
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
	int maxListRow = (height - m_length[LIST_HEAD_HEIGHT]) / m_length[LIST_ITEM_HEIGHT]; //List框带头
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
	pCombo->SetMaxStringLen( 32 );
	assert(pCombo);

	list.AddItem(ROW_SYSTEM_DATE, 0,m_pStrTable->String(E_STR_ID_SYSTEM_DATE));
	pDate = dynamic_cast<GUI::CDateCtrl*>(list.AddItem(ROW_SYSTEM_DATE, 1, WND_TYPE_DATE,m_length[DATE_CTRL_WIDTH],m_length[DATE_CTRL_HEIGHT]));
	pDate->AddToNotifyList();
	assert(pDate);
#ifdef __DVR_BASIC__
	list.AddItem(ROW_TIME_SYNC, 0, m_pStrTable->String(E_STR_ID_SYNC_TIME_WITH_NTP));
	pCheck = dynamic_cast<GUI::CCheckCtrl*>(list.AddItem(ROW_TIME_SYNC, 1, WND_TYPE_CHECK));
	assert(pCheck);

	list.AddItem(ROW_TIME_SERVER, 0, m_pStrTable->String(E_STR_ID_NTP_SERVER));
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

void CWzdUS02TimeDlg::OnIdle()
{
	CWzdUS02PageDlg::OnIdle();

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

bool CWzdUS02TimeDlg::IsModify()
{
	return m_bModifyTime;
}

void CWzdUS02TimeDlg::SetModify(bool modify, bool bRefresh)
{
	m_bModifyTime = modify;
}

bool CWzdUS02TimeDlg::CanModify()
{
	bool ret = false;

	CWzdUS02ManDlg* pWzdManDlg = dynamic_cast<CWzdUS02ManDlg*>(GetParent());
	assert(pWzdManDlg);
	if( pWzdManDlg )
	{
		ret = pWzdManDlg->TryEnterConfig();
	}

	if( !ret )
	{
		//如果没有进配置,提示需要客户端退出配置
		MessageOut(GetTopestWnd(), static_cast<ENUM_STRING_ID>(E_STR_ID_MESSAGE_104), TIP_MSG);
	}

	return ret;
}

void CWzdUS02TimeDlg::RefreshDateTime(LONGLONG gmt, int zone, bool bRefresh)
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

void CWzdUS02TimeDlg::GetDateTime(LONGLONG& gmt, int zone)
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

unsigned long CWzdUS02TimeDlg::OnPreCommand(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	if( m_bUpdateTimeNow )
	{
		//在NTP即时同步的时候屏蔽其他消息响应
		return KEY_VIRTUAL_MSG_STOP;
	}
	else
	{
		return CDialog::OnPreCommand( ctrlID, keyID ,xPos, yPos );
	}
}

BEGIN_MSG_MAP(CWzdUS02TimeDlg, CWzdUS02PageDlg)
	ON_MSG_EX(m_listTimeCtrl.GetID(), KEY_VIRTUAL_MSG_DEFAULT_TAG, OnClickList)
END_MSG_MAP()

unsigned long CWzdUS02TimeDlg::OnClickList(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	unsigned short row = (keyID >> 16)&0x7fff;	//行
	unsigned short col = keyID&0xffff;			//列

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
				m_pUS02timeWzdInfo->UpDateWzdTime = 1;
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

void CWzdUS02TimeDlg::SetEnableUI(  LIST_ROW rowNum, bool brefalsh )
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

