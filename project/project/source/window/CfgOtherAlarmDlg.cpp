/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : hz
** Date         : 2009-08-01
** Name         :CfgOtherAlarmDlg.cpp 
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/
#include "CfgOtherAlarmDlg.h"

CCfgOtherAlarmDlg::CCfgOtherAlarmDlg() : m_pOtherAlarmInfo(NULL)
{

}

CCfgOtherAlarmDlg::~CCfgOtherAlarmDlg()
{

}

//////////////////////////////////////////////////////////////////////////
void CCfgOtherAlarmDlg::OnInitial()
{
	CCfgDlg::OnInitial();
	SetImage(BMP_MEM_DLG_LARGE, false);
	SetCaption(m_pStrTable->String(E_STR_ID_TITLE_OTHER_ALARM), false);
	SetDefaultBtn( DEFAULT_BTN_APPLY | DEFAULT_BTN_OK | DEFAULT_BTN_CANCEL );
	SetDefaultBtnCaption( DEFAULT_BTN_CANCEL, m_pStrTable->String(E_STR_ID_EXIT));
	SetDefaultBtnCaption( DEFAULT_BTN_OK,m_pStrTable->String(E_STR_ID_APPLY) );
	SetDefaultBtnCaption( DEFAULT_BTN_APPLY,m_pStrTable->String(E_STR_ID_DEFAULT) );
	//////////////////////////////////////////////////////////////////////////

	unsigned short x = m_length[DLG_OFFSET_LEFT];
	unsigned short y = m_length[TITLE_HEIGHT] + m_length[DLG_OFFSET_TOP];
	unsigned short cx = m_rect.Width() - 2 * m_length[DLG_OFFSET_LEFT];
	unsigned short cy = m_rect.Height() - y - m_length[DLG_OFFSET_BOTTOM] - m_length[DEFAULT_BTN_BOTTOM_YPOS];
	unsigned short height = cy -  (cy - 4) % m_length[LIST_ITEM_HEIGHT];

	if(NULL == m_pOtherAlarmInfo)
	{
		m_pOtherAlarmInfo = new OTHER_ALARM_INFO[8];
	}
	memset(m_pOtherAlarmInfo, 0, sizeof(OTHER_ALARM_INFO) * 8);

	m_otherAlarm.SetMaxAlarmOut(CCfgDlg::s_loginSuccessReply.productInfo.relayOutputNum);
	m_otherAlarm.SetOtherAlarmInfo(m_pOtherAlarmInfo);

	m_otherAlarm.Create(GetRandWndID(), x, y + (cy - height)/2, cx, height, this, 0, 1);


	////
	CreateTipWnd();
}

void CCfgOtherAlarmDlg::OnDestroy()
{
	CCfgDlg::OnDestroy();

	if(m_pOtherAlarmInfo)
	{
		delete [] m_pOtherAlarmInfo;
		m_pOtherAlarmInfo = NULL;
	}
}

void CCfgOtherAlarmDlg::UpdateView(bool bRefresh)
{
	CCfgDlg::UpdateView(bRefresh);

	m_otherAlarm.InitCtrlData(bRefresh);
}

void CCfgOtherAlarmDlg::GetItemList(CFGITEM_LIST& itemList) const
{
	itemList.push_back(NCFG_ITEM_DISK_ALARM_FREE);

	itemList.push_back(NCFG_ITEM_DISK_FULL_BUZZER);
	itemList.push_back(NCFG_ITEM_DISK_FULL_TO_ALARM_OUT);
#ifdef __DVR_BASIC__
	itemList.push_back(NCFG_ITEM_DISK_FULL_TO_EMAIL);
#endif
	itemList.push_back(NCFG_ITEM_IP_CONFLICT_BUZZER);
	itemList.push_back(NCFG_ITEM_IP_CONFLICT_TO_ALARM_OUT);
#ifdef __DVR_BASIC__
	itemList.push_back(NCFG_ITEM_IP_CONFLICT_TO_EMAIL);
#endif
	itemList.push_back(NCFG_ITEM_DISCONNECT_BUZZER);
	itemList.push_back(NCFG_ITEM_DISCONNECT_TO_ALARM_OUT);
#ifdef __DVR_BASIC__
	itemList.push_back(NCFG_ITEM_DISCONNECT_TO_EMAIL);
#endif
	itemList.push_back(NCFG_ITEM_DISK_WARNING_BUZZER);
	itemList.push_back(NCFG_ITEM_DISK_WARNING_TO_ALARM_OUT);
	itemList.push_back(NCFG_ITEM_DISK_WARNING_TO_EMAIL);
	//zxx新加
	itemList.push_back(NCFG_ITEM_DISK_DISCONNECT_BUZZER);
	itemList.push_back(NCFG_ITEM_DISK_DISCONNECT_TO_ALARM_OUT);
	itemList.push_back(NCFG_ITEM_DISK_DISCONNECT_TO_EMAIL);
}

bool CCfgOtherAlarmDlg::ShowData(NCFG_ITEM_ID cfgID, const BYTE* pData, int len, bool bRefresh)
{
	assert(NULL != pData);
	assert(len > 0);

	switch (cfgID)
	{
	case NCFG_ITEM_DISK_ALARM_FREE:
		{
			assert(len == sizeof(unsigned long));
			m_pOtherAlarmInfo[0].DiskAlarmFree = *reinterpret_cast<const unsigned long*>(pData);
		}
		return true;
		break;
	case NCFG_ITEM_DISK_FULL_BUZZER:
		{
			assert(len == sizeof(unsigned long));
			m_pOtherAlarmInfo[0].Buzzer = *reinterpret_cast<const unsigned long*>(pData);
		}
		return true;
		break;
	case NCFG_ITEM_DISK_FULL_TO_ALARM_OUT:
		{
			assert(len == sizeof(ULONGLONG));
			m_pOtherAlarmInfo[0].AlarmOut = *reinterpret_cast<const ULONGLONG*>(pData);
		}
		return true;
		break;
#ifdef __DVR_BASIC__
	case NCFG_ITEM_DISK_FULL_TO_EMAIL:
		{
			assert(len == sizeof(unsigned long));
			m_pOtherAlarmInfo[0].Email = *reinterpret_cast<const unsigned long*>(pData);
		}
		return true;
		break;
#endif
	case NCFG_ITEM_IP_CONFLICT_BUZZER:
		{
			assert(len == sizeof(unsigned long));
			m_pOtherAlarmInfo[1].Buzzer = *reinterpret_cast<const unsigned long*>(pData);
		}
		return true;
		break;
	case NCFG_ITEM_IP_CONFLICT_TO_ALARM_OUT:
		{
			assert(len == sizeof(ULONGLONG));
			m_pOtherAlarmInfo[1].AlarmOut = *reinterpret_cast<const ULONGLONG*>(pData);
		}
		return true;
		break;
#ifdef __DVR_BASIC__
	case NCFG_ITEM_IP_CONFLICT_TO_EMAIL:
		{
			assert(len == sizeof(unsigned long));
			m_pOtherAlarmInfo[1].Email = *reinterpret_cast<const unsigned long*>(pData);
		}
		return true;
		break;
#endif
	case NCFG_ITEM_DISCONNECT_BUZZER:
		{
			assert(len == sizeof(unsigned long));
			m_pOtherAlarmInfo[2].Buzzer = *reinterpret_cast<const unsigned long*>(pData);
		}
		return true;
		break;
	case NCFG_ITEM_DISCONNECT_TO_ALARM_OUT:
		{
			assert(len == sizeof(ULONGLONG));
			m_pOtherAlarmInfo[2].AlarmOut = *reinterpret_cast<const ULONGLONG*>(pData);
		}
		return true;
		break;
#ifdef __DVR_BASIC__
	case NCFG_ITEM_DISCONNECT_TO_EMAIL:
		{
			assert(len == sizeof(unsigned long));
			m_pOtherAlarmInfo[2].Email = *reinterpret_cast<const unsigned long*>(pData);
		}
		return true;
		break;
#endif
	case NCFG_ITEM_DISK_WARNING_BUZZER:
		{
			assert(len == sizeof(unsigned long));
			m_pOtherAlarmInfo[3].Buzzer = *reinterpret_cast<const unsigned long*>(pData);
		}
		return true;
		break;
	case NCFG_ITEM_DISK_WARNING_TO_ALARM_OUT:
		{
			assert(len == sizeof(ULONGLONG));
			m_pOtherAlarmInfo[3].AlarmOut = *reinterpret_cast<const ULONGLONG*>(pData);
		}
		return true;
		break;
	case NCFG_ITEM_DISK_WARNING_TO_EMAIL:
		{
			assert(len == sizeof(unsigned long));
			m_pOtherAlarmInfo[3].Email = *reinterpret_cast<const unsigned long*>(pData);
		}
		return true;
		break;
	case NCFG_ITEM_DISK_DISCONNECT_BUZZER:
		{
			assert(len == sizeof(unsigned long));
			m_pOtherAlarmInfo[4].Buzzer = *reinterpret_cast<const unsigned long*>(pData);
		}
		return true;
		break;
	case NCFG_ITEM_DISK_DISCONNECT_TO_ALARM_OUT:
		{
			assert(len == sizeof(ULONGLONG));
			m_pOtherAlarmInfo[4].AlarmOut = *reinterpret_cast<const ULONGLONG*>(pData);
		}
		return true;
		break;
	case NCFG_ITEM_DISK_DISCONNECT_TO_EMAIL:
		{
			assert(len == sizeof(unsigned long));
			m_pOtherAlarmInfo[4].Email = *reinterpret_cast<const unsigned long*>(pData);
		}
		return true;
		break;
	default:
		return false;
		break;
	}
	return false;
}

bool CCfgOtherAlarmDlg::SetData(NCFG_ITEM_ID cfgID, BYTE*& pData, int& len) const
{
	assert(NULL != pData);
	assert(len > 0);

	switch (cfgID)
	{
	case NCFG_ITEM_DISK_ALARM_FREE:
		{
			assert(len == sizeof(unsigned long));
			*reinterpret_cast<unsigned long*>(pData) = m_pOtherAlarmInfo[0].DiskAlarmFree;
		}
		return true;
		break;
	case NCFG_ITEM_DISK_FULL_BUZZER:
		{
			assert(len == sizeof(unsigned long));
			*reinterpret_cast<unsigned long*>(pData) = m_pOtherAlarmInfo[0].Buzzer;
		}
		return true;
		break;
	case NCFG_ITEM_DISK_FULL_TO_ALARM_OUT:
		{
			assert(len == sizeof(ULONGLONG));
			*reinterpret_cast<ULONGLONG*>(pData) = m_pOtherAlarmInfo[0].AlarmOut;
		}
		return true;
		break;
#ifdef __DVR_BASIC__
	case NCFG_ITEM_DISK_FULL_TO_EMAIL:
		{
			assert(len == sizeof(unsigned long));
			*reinterpret_cast<unsigned long*>(pData) = m_pOtherAlarmInfo[0].Email;
		}
		return true;
		break;
#endif
	case NCFG_ITEM_IP_CONFLICT_BUZZER:
		{
			assert(len == sizeof(unsigned long));
			*reinterpret_cast<unsigned long*>(pData) = m_pOtherAlarmInfo[1].Buzzer;
		}
		return true;
		break;
	case NCFG_ITEM_IP_CONFLICT_TO_ALARM_OUT:
		{
			assert(len == sizeof(ULONGLONG));
			*reinterpret_cast<ULONGLONG*>(pData) = m_pOtherAlarmInfo[1].AlarmOut;
		}
		return true;
		break;
#ifdef __DVR_BASIC__
	case NCFG_ITEM_IP_CONFLICT_TO_EMAIL:
		{
			assert(len == sizeof(unsigned long));
			*reinterpret_cast<unsigned long*>(pData) = m_pOtherAlarmInfo[1].Email;
		}
		return true;
		break;
#endif
	case NCFG_ITEM_DISCONNECT_BUZZER:
		{
			assert(len == sizeof(unsigned long));
			*reinterpret_cast<unsigned long*>(pData) = m_pOtherAlarmInfo[2].Buzzer;
		}
		return true;
		break;
	case NCFG_ITEM_DISCONNECT_TO_ALARM_OUT:
		{
			assert(len == sizeof(ULONGLONG));
			*reinterpret_cast<ULONGLONG*>(pData) = m_pOtherAlarmInfo[2].AlarmOut;
		}
		return true;
		break;
#ifdef __DVR_BASIC__
	case NCFG_ITEM_DISCONNECT_TO_EMAIL:
		{
			assert(len == sizeof(unsigned long));
			*reinterpret_cast<unsigned long*>(pData) = m_pOtherAlarmInfo[2].Email;
		}
		return true;
		break;
#endif
	case NCFG_ITEM_DISK_WARNING_BUZZER:
		{
			assert(len == sizeof(unsigned long));
			*reinterpret_cast<unsigned long*>(pData) = m_pOtherAlarmInfo[3].Buzzer;			
		}
		return true;
		break;
	case NCFG_ITEM_DISK_WARNING_TO_ALARM_OUT:
		{
			assert(len == sizeof(ULONGLONG));
			*reinterpret_cast<ULONGLONG*>(pData) = m_pOtherAlarmInfo[3].AlarmOut;
		}
		return true;
		break;
	case NCFG_ITEM_DISK_WARNING_TO_EMAIL:
		{
			assert(len == sizeof(unsigned long));
			*reinterpret_cast<unsigned long*>(pData) = m_pOtherAlarmInfo[3].Email;
		}
		return true;
		break;
	case NCFG_ITEM_DISK_DISCONNECT_BUZZER:
		{
			assert(len == sizeof(unsigned long));
			*reinterpret_cast<unsigned long*>(pData) = m_pOtherAlarmInfo[4].Buzzer;			
		}
		return true;
		break;
	case NCFG_ITEM_DISK_DISCONNECT_TO_ALARM_OUT:
		{
			assert(len == sizeof(ULONGLONG));
			*reinterpret_cast<ULONGLONG*>(pData) = m_pOtherAlarmInfo[4].AlarmOut;
		}
		return true;
		break;
	case NCFG_ITEM_DISK_DISCONNECT_TO_EMAIL:
		{
			assert(len == sizeof(unsigned long));
			*reinterpret_cast<unsigned long*>(pData) = m_pOtherAlarmInfo[4].Email;
		}
		return true;
		break;
	default:
		return false;
		break;
	}

	return false;
}

unsigned long CCfgOtherAlarmDlg::OnApply()
{
	//Load default
	unsigned long ret = OnLoadDefault(this);

	m_otherAlarm.InitCtrlData(false);

	if( IsModify() )
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

BEGIN_MSG_MAP(CCfgOtherAlarmDlg, CCfgDlg)
	ON_MSG_EX(m_otherAlarm.GetID(), KEY_VIRTUAL_MSG_DEFAULT_TAG, OnClickScrollDlg)
END_MSG_MAP()

unsigned long CCfgOtherAlarmDlg::OnClickScrollDlg(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	int index = -1;
	unsigned long tmpIndex = (keyID >> 20) & 0x7FF;
	if( tmpIndex < 0x7FF )
	{
		index = tmpIndex;
	}
	unsigned long id = keyID & 0xFFFFF;

	if((!IsModify()) && (index >= 0))
	{
		if (IsChange(NCFG_ITEM_DISK_ALARM_FREE) ||
			IsChange(NCFG_ITEM_DISK_FULL_BUZZER) ||
			IsChange(NCFG_ITEM_DISK_FULL_TO_ALARM_OUT) ||
#ifdef __DVR_BASIC__
			IsChange(NCFG_ITEM_DISK_FULL_TO_EMAIL) ||
#endif
			IsChange(NCFG_ITEM_IP_CONFLICT_BUZZER) ||
			IsChange(NCFG_ITEM_IP_CONFLICT_TO_ALARM_OUT) ||
#ifdef __DVR_BASIC__
			IsChange(NCFG_ITEM_IP_CONFLICT_TO_EMAIL) ||
#endif
			IsChange(NCFG_ITEM_DISCONNECT_BUZZER) ||
			IsChange(NCFG_ITEM_DISCONNECT_TO_ALARM_OUT) ||
#ifdef __DVR_BASIC__
			IsChange(NCFG_ITEM_DISCONNECT_TO_EMAIL) ||
#endif
			IsChange(NCFG_ITEM_DISK_WARNING_BUZZER) ||
			IsChange(NCFG_ITEM_DISK_WARNING_TO_ALARM_OUT) ||
			IsChange(NCFG_ITEM_DISK_WARNING_TO_EMAIL) ||
			IsChange(NCFG_ITEM_DISK_DISCONNECT_BUZZER) ||
			IsChange(NCFG_ITEM_DISK_DISCONNECT_TO_ALARM_OUT) ||
			IsChange(NCFG_ITEM_DISK_DISCONNECT_TO_EMAIL))

		{
			SetModify(true);
			EnableOkBtn(true, true);
			return keyID;
		}
	}

	return KEY_VIRTUAL_MSG_STOP;
}

//end
