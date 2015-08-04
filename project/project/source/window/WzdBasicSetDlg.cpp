/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : L
** Date         : 2010-02-26
** Name         : WzdStepDlg.h
** Version      : 1.0
** Description  : 向导页面对话框
** Modify Record:
***********************************************************************/

#include "WzdBasicSetDlg.h"
#include "WzdManDlg.h"
#include <math.h>
//////////////////////////////////////////////////////////////////////////
extern std::string g_zoneName[];  

//////////////////////////////////////////////////////////////////////////
// 第一页面
//////////////////////////////////////////////////////////////////////////
static int g_zoneValue[]={720, 660, 600, 540, 480, 420, 360, 300, 270, 240, 
						210, 180, 120, 60, 0, 60, 120, 180, 210, 240, 
						270, 300, 330, 345, 360, 390, 420, 480, 540, 570, 
						600, 660, 720, 780};

CWzdBasicSetDlg::CWzdBasicSetDlg()
{
	m_bModifyTime = false;
	m_bStopRefreshTime = false;

	m_pre_sys_tm = 0;
	m_gmt_user_tm = 0;
	m_refresh_count_tm = 0;
}

CWzdBasicSetDlg::~CWzdBasicSetDlg()
{

}

void CWzdBasicSetDlg::OnInitial()
{
	CWzdPageDlg::OnInitial();
	//////////////////////////////////////////////////////////////////////////

	GUI::CListCtrl& list = m_listCtrl;
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
	list.AddItem(ROW_DEVICE_NAME, 0, m_pStrTable->String(E_STR_ID_DEV_NAME));
	
	pEdit = dynamic_cast<GUI::CEditCtrl*>(list.AddItem(ROW_DEVICE_NAME, 1, WND_TYPE_EDIT));
	assert(NULL != pEdit);
	list.AddItem(ROW_LANGUAGE, 0, m_pStrTable->String(E_STR_ID_LANGUAGE));
	pCombo = dynamic_cast<GUI::CComboCtrl*>(list.AddItem(ROW_LANGUAGE, 1, WND_TYPE_COMBO));
	assert(pCombo);
	
	list.AddItem(ROW_DATE_FORMAT, 0, m_pStrTable->String(E_STR_ID_DATE_FORMAT));
	pCombo = dynamic_cast<GUI::CComboCtrl*>(list.AddItem(ROW_DATE_FORMAT, 1, WND_TYPE_COMBO));
	assert(pCombo);	
	
	list.AddItem(ROW_TIME_FORMAT, 0, m_pStrTable->String(E_STR_ID_TIME_FORMAT));
	pCombo = dynamic_cast<GUI::CComboCtrl*>(list.AddItem(ROW_TIME_FORMAT, 1, WND_TYPE_COMBO));
	assert(pCombo);
	
	list.AddItem(ROW_TIME_ZONE, 0, m_pStrTable->String(E_STR_ID_TIME_ZONE));
	pCombo = dynamic_cast<GUI::CComboCtrl*>(list.AddItem(ROW_TIME_ZONE, 1, WND_TYPE_COMBO));
	pCombo->SetMaxStringLen( 32 );
	assert(pCombo);
	
	list.AddItem(ROW_SYSTEM_DATE, 0, m_pStrTable->String(E_STR_ID_SYSTEM_DATE));
	pDate = dynamic_cast<GUI::CDateCtrl*>(list.AddItem(ROW_SYSTEM_DATE, 1, WND_TYPE_DATE,m_length[DATE_CTRL_WIDTH],m_length[DATE_CTRL_HEIGHT]));
	pDate->AddToNotifyList();
	assert(pDate);
	
	list.AddItem(ROW_SYSTEM_TIME, 0, m_pStrTable->String(E_STR_ID_SYSTEM_TIME));
	pTime = dynamic_cast<GUI::CTimeCtrl*>(list.AddItem(ROW_SYSTEM_TIME, 1, WND_TYPE_TIME,m_length[DATE_CTRL_WIDTH],m_length[DATE_CTRL_HEIGHT]));
	pTime->AddToNotifyList();
	assert(pTime);

	list.AddItem(ROW_SHOW_WZD, 0, m_pStrTable->String(E_STR_ID_SHOW_WZD));
	pCheck = dynamic_cast<GUI::CCheckCtrl*>(list.AddItem(ROW_SHOW_WZD, 1, WND_TYPE_CHECK));
	assert(pCheck);

	list.CreateListFocusMap();
}

void CWzdBasicSetDlg::InitCtrlInfo()
{
	GUI::CListCtrl& list = m_listCtrl;
	//////////////////////////////////////////////////////////////////////////
	
	GUI::CEditCtrl* pEdit = dynamic_cast<GUI::CEditCtrl*>(list.GetItemWnd(ROW_DEVICE_NAME, 1));
	pEdit->SetMaxStringLen(m_pWzdInfo->DevMaxNameLen);
	pEdit->SetCaption(m_pWzdInfo->DeviceName, false);
	
	GUI::CComboCtrl* pCombo = dynamic_cast<GUI::CComboCtrl*>(list.GetItemWnd(ROW_LANGUAGE, 1));
	assert(pCombo);
	pCombo->RemoveAllItem(false);
	for(int i = 0; i < m_pWzdInfo->LanguageNum; i++)
	{
		unsigned long langID = m_pWzdInfo->Language[i].languageId;
		pCombo->AddItem(m_pWzdInfo->Language[i].name, (LONGLONG)langID);
	}
	pCombo->SetCurItemData(m_pWzdInfo->CurrentLanguage, false);
#if defined(__TW01_RILI__)	
	pCombo->SetEnable(false, false);
#endif

	pCombo = dynamic_cast<GUI::CComboCtrl*>(list.GetItemWnd(ROW_DATE_FORMAT, 1));
	pCombo->AddItem(m_pStrTable->String(E_STR_ID_YY_MM_DD), DATE_MODE_YMD);
	pCombo->AddItem(m_pStrTable->String(E_STR_ID_MM_DD_YY), DATE_MODE_MDY);
	pCombo->AddItem(m_pStrTable->String(E_STR_ID_DD_MM_YY), DATE_MODE_DMY);
	pCombo->SetCurItemData(m_pWzdInfo->DateMode, false);
#if defined(__CUSTOM_RU04__)
	pCombo->SetEnable(false, false);
#endif
	
	pCombo = dynamic_cast<GUI::CComboCtrl*>(list.GetItemWnd( ROW_TIME_FORMAT, 1 ));
	pCombo->AddItem(m_pStrTable->String(E_STR_ID_TIME_HOUR_12),TIME_MODE_12);
	pCombo->AddItem(m_pStrTable->String(E_STR_ID_TIME_HOUR_24),TIME_MODE_24);
	pCombo->SetCurItemData(m_pWzdInfo->TimeMode, false);
#if defined(__CUSTOM_RU04__)
	pCombo->SetEnable(false, false);
#endif

	pCombo = dynamic_cast<GUI::CComboCtrl*>(list.GetItemWnd(ROW_TIME_ZONE, 1));
	int count = TIME_ZONE_GMT_A13 + 1;
	for(int i = 0; i < count; i++)
	{
		pCombo->AddItem(g_zoneName[i].c_str(), i);
	}
	pCombo->SetCurItemData(m_pWzdInfo->TimeZone, false);

#if defined(__TW01_RILI__)
	pCombo->SetEnable(false, false);
#endif

	GUI::CCheckCtrl* pCheck = dynamic_cast<GUI::CCheckCtrl*>(list.GetItemWnd(ROW_SHOW_WZD, 1));
	if(0 != m_pWzdInfo->ShowWzd)
	{
		pCheck->SetCheck(true, false);
	}
	else
	{
		pCheck->SetCheck(false, false);
	}

	//时间
	LONGLONG currTime = GetCurrTime();
	m_pre_sys_tm = currTime;

	if(m_pWzdInfo->TimeZone >= TIME_ZONE_GMT)
	{
		m_gmt_user_tm = currTime - static_cast<LONGLONG>(g_zoneValue[m_pWzdInfo->TimeZone]) * 60 * 1000000;
	}
	else
	{
		m_gmt_user_tm = currTime + static_cast<LONGLONG>(g_zoneValue[m_pWzdInfo->TimeZone]) * 60 * 1000000;
	}

	RefreshDateTime(m_gmt_user_tm, m_pWzdInfo->TimeZone, false);
}

void CWzdBasicSetDlg::OnIdle()
{
	CWzdPageDlg::OnIdle();

	LONGLONG curr_tm = GetCurrTime();

	if(!m_bStopRefreshTime)
	{
		LONGLONG off_tm = curr_tm - m_pre_sys_tm;
		m_gmt_user_tm += off_tm;

		m_refresh_count_tm += off_tm;
		if(m_refresh_count_tm > 1000000)
		{
			GUI::CComboCtrl* pCombo = dynamic_cast<GUI::CComboCtrl*>(m_listCtrl.GetItemWnd(ROW_TIME_ZONE, 1));
			int zone = static_cast<unsigned int>(pCombo->GetCurItemData());

			RefreshDateTime(m_gmt_user_tm, zone, true);

			m_refresh_count_tm = 0;
		}
	}

	m_pre_sys_tm = curr_tm;
}

void CWzdBasicSetDlg::ShowWzdData(bool bRefresh)
{
	m_bStopRefreshTime = false;
}

void CWzdBasicSetDlg::GetWzdData(bool bNext )
{
	GUI::CListCtrl& list = m_listCtrl;

	GUI::CEditCtrl* pEdit = dynamic_cast<GUI::CEditCtrl*>(list.GetItemWnd(ROW_DEVICE_NAME, 1));
	strcpy(m_pWzdInfo->DeviceName, pEdit->GetCaption().c_str());

	GUI::CComboCtrl* pCombo = dynamic_cast<GUI::CComboCtrl*>(list.GetItemWnd(ROW_LANGUAGE, 1));
	m_pWzdInfo->CurrentLanguage = pCombo->GetCurItemData();

	pCombo = dynamic_cast<GUI::CComboCtrl*>(list.GetItemWnd(ROW_DATE_FORMAT, 1));
	m_pWzdInfo->DateMode = pCombo->GetCurItemData();

	pCombo = dynamic_cast<GUI::CComboCtrl*>(list.GetItemWnd(ROW_TIME_FORMAT, 1));
	m_pWzdInfo->TimeMode = pCombo->GetCurItemData();

	pCombo = dynamic_cast<GUI::CComboCtrl*>(list.GetItemWnd(ROW_TIME_ZONE, 1));
	m_pWzdInfo->TimeZone = pCombo->GetCurItemData();

	GUI::CCheckCtrl* pCheck = dynamic_cast<GUI::CCheckCtrl*>(list.GetItemWnd(ROW_SHOW_WZD, 1));
	m_pWzdInfo->ShowWzd = (pCheck->IsCheck() ? 1 : 0);
}

void CWzdBasicSetDlg::OnLeavePage()
{
	m_bStopRefreshTime = false;
}

bool CWzdBasicSetDlg::GetCurrZoneTimeInfo(ZONE_TIME_INFO& zoneTimeInfo, int zone)
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

bool CWzdBasicSetDlg::IsModify()
{
	return m_bModifyTime;
}

void CWzdBasicSetDlg::SetModify(bool modify, bool bRefresh)
{
	m_bModifyTime = modify;
}

bool CWzdBasicSetDlg::CanModify()
{
	bool ret = false;

	CWzdManDlg* pWzdManDlg = dynamic_cast<CWzdManDlg*>(GetParent());
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

void CWzdBasicSetDlg::RefreshDateTime(LONGLONG gmt, int zone, bool bRefresh)
{
	GUI::CListCtrl& list = m_listCtrl;

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

void CWzdBasicSetDlg::GetDateTime(LONGLONG& gmt, int zone)
{
	GUI::CListCtrl& list = m_listCtrl;

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

BEGIN_MSG_MAP(CWzdBasicSetDlg, CWzdPageDlg)
	ON_MSG_EX(m_listCtrl.GetID(), KEY_VIRTUAL_MSG_DEFAULT_TAG, OnClickList)
END_MSG_MAP()

unsigned long CWzdBasicSetDlg::OnClickList(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	unsigned short row = (keyID >> 16)&0x7fff;	//行
	unsigned short col = keyID&0xffff;			//列

	GUI::CListCtrl& list = m_listCtrl;

	switch (row)
	{
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
	default:
		break;	
	} 

	return KEY_VIRTUAL_MSG_STOP;
}

//end
