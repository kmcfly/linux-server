/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : l
** Date         : 2009-08-12
** Name         : CfgScheduleChildDlg.cpp
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/
#include "CfgScheduleChildDlg.h"
#include "CfgScheduleHolidayDlg.h"

#include "Schedule.h"

#include "FuncCustom.h"

extern NET_LOGIN_SUCCESS_REPLY * g_p_login_succ;

#include "CfgScheduleDlg.h"

#include "CfgNewMotionDlg.h"
CCfgScheduleChildDlg::CCfgScheduleChildDlg() : m_weekCfgID(NCFG_ITEM_REC_SCH_SCHEDULE_WEEK)
, m_holidayCfgID(NCFG_ITEM_REC_SCH_SCHEDULE_HOLIDAY)
, m_pWeekSchedule(NULL)
, m_weekScheduleLen(0)
, m_maxChnn(-1)
, m_typeCH(TYPE_CH)
, m_scheduleChildType(SCHEDULE_CHILD_NONE)
{

}

CCfgScheduleChildDlg::~CCfgScheduleChildDlg()
{

}

void CCfgScheduleChildDlg::OnInitial()
{
	assert(m_maxChnn > 0);

	CCfgDlg::OnInitial();
	//////////////////////////////////////////////////////////////////////////
	
	unsigned short x = m_length[DLG_OFFSET_SUB_LEFT], y = m_length[DLG_OFFSET_SUB_TOP];
	int schedule_width = m_rect.Width() - 2 * m_length[DLG_OFFSET_SUB_LEFT];
	int schedule_height = m_length[SCHEDULE_HEADER_HEIGHT] + (m_length[SCHEDULE_ITEM_HEIGHT] - 1)* 7 + 1;
	int text_width = 124;
	
	m_scChannel.Create(GetRandWndID(), x, y, text_width, m_length[STATIC_HEIGHT], this, m_pStrTable->String(E_STR_ID_CHANNEL));
	m_comboChnn.Create(GetRandWndID(), x + text_width + 26, y, 120, m_length[COMBO_HEIGHT], this, 0, 0);
	
	char sz[32] = {0};

	ULONGLONG CH = 0;
	int localVideoInputNum = 0, netVideoInputNum = 0;
	int effectChnn = 0;

	if(TYPE_ALL == m_typeCH)
	{
		for(int i = 0; i < m_maxChnn; i++)
		{
			CH |= (0x1 << i);
		}
	}
	else
	{
		IsSupportCH(CH, &effectChnn, &localVideoInputNum, &netVideoInputNum);

		//考虑到本地SensorIn的通道数与视频通道数不等
		CH >>= localVideoInputNum;
		int localSensorInNum = m_maxChnn - netVideoInputNum;
		CH <<= localSensorInNum;

// 		if (SCHEDULE_CHILD_ALARM == m_scheduleChildType)
// 		{
// 			localSensorInNum = g_p_login_succ->productInfo.sensorInputNum;
// 		}

		CH |= ALL_CHNN_MASK(localSensorInNum);
	}
	
	int count = 0;

	for (int ch = 0; ch < m_maxChnn; ++ch)
	{
		if((CH >> ch) & 0x1)
		{
			snprintf(sz, sizeof(sz), "%d", ch + 1);
			m_comboChnn.AddItem(sz,ch);
			count++;
		}
	}

	if(count <= 0)
	{
		m_comboChnn.AddItem(m_pStrTable->String(E_STR_ID_NONE), COMBO_CHNN_NULL);
	}

	m_comboChnn.SetCurItem(0);
	m_comboChnn.SetData(0);

	//m_btnHoliday.Create(GetRandWndID(), m_rect.Width() - m_length[BTN_WIDTH] - m_length[DLG_OFFSET_SUB_LEFT], y,  m_length[BTN_WIDTH], m_length[BTN_HEIGHT], this, 1, 0, m_pStrTable->String(E_STR_ID_HOLIDAY_SCHEDULE));
	////////////////////////////////
	y += m_length[COMBO_HEIGHT] + 15;
	//int cy = 186;
	m_scheduleWeekCtrl.Create(GetRandWndID(), x, y, schedule_width, schedule_height, this, 0, 1, text_width);
	m_scheduleWeekCtrl.AddItem(0, m_pStrTable->String(E_STR_ID_SUNDAY), false);
	m_scheduleWeekCtrl.AddItem(1, m_pStrTable->String(E_STR_ID_MONDAY), false);
	m_scheduleWeekCtrl.AddItem(2, m_pStrTable->String(E_STR_ID_TUESDAY), false);
	m_scheduleWeekCtrl.AddItem(3, m_pStrTable->String(E_STR_ID_WEDNESDAY), false);
	m_scheduleWeekCtrl.AddItem(4, m_pStrTable->String(E_STR_ID_THURSDAY), false);
	m_scheduleWeekCtrl.AddItem(5, m_pStrTable->String(E_STR_ID_FRIDAY), false);
	m_scheduleWeekCtrl.AddItem(6, m_pStrTable->String(E_STR_ID_SATURDAY), false);
	//m_scheduleWeekCtrl.CreateFocusMap();

// 	m_scheduleWeekCtrl.AddData(0, 0, 0, 0, 2, 0, 0);
// 	m_scheduleWeekCtrl.AddData(0, 6, 30, 0, 10, 30, 0);
// 	m_scheduleWeekCtrl.AddData(0, 16, 0, 0, 23, 59, 59);
// 
// 	m_scheduleWeekCtrl.AddData(1, 1, 0, 0, 2, 0, 0);
// 	m_scheduleWeekCtrl.AddData(1, 6, 59, 0, 10, 30, 0);
// 	m_scheduleWeekCtrl.AddData(1, 16, 0, 0, 23, 59, 59);

	//Copy static
	unsigned short stCopy_width = text_width + 26;//m_rect.Width() / 4;
	y += schedule_height + 20;
	m_stCopy.Create(CTRLID_STATIC, x, y, stCopy_width, m_length[STATIC_HEIGHT], this, m_pStrTable->String(E_STR_ID_APPLY_SETTINGS_TO));
	//copy combo
	x += stCopy_width + 3;
	unsigned short comboCopy_width = 120;
	m_comboCopy.Create(GetRandWndID(), x, y, comboCopy_width, m_length[COMBO_HEIGHT], this, 2, 1);

	count = 0;
	for (int ch = 0; ch < m_maxChnn; ++ch)
	{
		if((CH >> ch) & 0x1)
		{
			snprintf(sz, sizeof(sz), "%d", ch + 1);
			m_comboCopy.AddItem(sz,ch);
			count++;
		}
	}

	if(count <= 0)
	{
		m_comboCopy.AddItem(m_pStrTable->String(E_STR_ID_NONE), COMBO_CHNN_NULL);
		m_comboCopy.SetCurItem(0);
	}
	else
	{
		m_comboCopy.AddItem(m_pStrTable->String(E_STR_ID_ALL), COMBO_CHNN_ALL);
		m_comboCopy.SetCurItemData(COMBO_CHNN_ALL, false);
	}

	x += comboCopy_width + 10;
	if(!g_langRightOrder)
	{
		m_scCopyChnn.Create(GetRandWndID(), x, y, text_width, m_length[STATIC_HEIGHT], this, m_pStrTable->String(E_STR_ID_CHANNEL));
	}
	else
	{
		m_scCopyChnn.Create(GetRandWndID(), x, y, 45, m_length[STATIC_HEIGHT], this, m_pStrTable->String(E_STR_ID_CHANNEL));
	}
	
	
	//Copy Button
	x += text_width + 10;
	m_btnCopy.Create(GetRandWndID(), x, y, m_length[BTN_WIDTH], m_length[BTN_HEIGHT], this, 1, 2, m_pStrTable->String(E_STR_ID_COPY));
#if defined(__TW01_RILI__)   
	x += m_length[BTN_WIDTH]+10;
	m_btnEdit.Create(GetRandWndID(),x,y,m_length[BTN_WIDTH],m_length[BTN_HEIGHT], this, 3, 0, m_pStrTable->String(E_STR_ID_EDIT));
#endif

	#ifdef __CUSTOM_US02_NEW__
		NCFG_ITEM_ID week;
		GetScheduleCfgID(week);
		if(NCFG_ITEM_MOTION_SCH_WEEK == week)
		{
			unsigned short tipPosX = m_length[DLG_OFFSET_SUB_LEFT], tipPosY = GetClientRect().m_bottom - 5 * m_length[DLG_OFFSET_BOTTOM];
			m_picTip.Create(GetRandWndID(), tipPosX, tipPosY,this,BMP_ICON_INFO,false);
			m_scTip.Create(GetRandWndID(), tipPosX + 25,tipPosY, 300 + 24,m_length[STATIC_HEIGHT],this,NULL);
			m_picTip.Show(true, false);


			m_scTip.SetBKColor(COLOR_DLG_CHILD_BK);

			m_scTip.SetCaption(m_pStrTable->String(E_STR_ID_MOTION_SCHEDULE), false);
		}
	#endif

	//Edit state button
	unsigned short bmpWidth = 30;
	unsigned short bmpHeight = 25;
	GUI::CRect rect = m_scheduleWeekCtrl.GetWindowRect();
	x = m_length[DLG_OFFSET_SUB_LEFT] + schedule_width - (schedule_width - rect.Width()) / 2 - bmpWidth * 2 - 4;
	y = m_length[DLG_OFFSET_SUB_TOP] + (m_length[COMBO_HEIGHT] - bmpHeight) / 2;

	m_btnAdd.Create(GetRandWndID(), x, y, bmpWidth, bmpHeight, this, 1, 0);
	m_btnAdd.SetStateImage(STATE_SELECT_NO, PAINT_STATUS_NORMAL, BMP_EDIT);
	m_btnAdd.SetStateImage(STATE_SELECT_NO, PAINT_STATUS_FOCUS, BMP_EDIT);
	m_btnAdd.SetStateImage(STATE_SELECT_NO, PAINT_STATUS_DISABLE, BMP_EDIT);
	m_btnAdd.SetStateImage(STATE_SELECT_YES, PAINT_STATUS_NORMAL, BMP_EDIT_FOCUS);
	m_btnAdd.SetStateImage(STATE_SELECT_YES, PAINT_STATUS_FOCUS, BMP_EDIT_FOCUS);
	m_btnAdd.SetStateImage(STATE_SELECT_YES, PAINT_STATUS_DISABLE, BMP_EDIT_FOCUS);

	x += bmpWidth + 4;
	m_btnDec.Create(GetRandWndID(), x, y, bmpWidth, bmpHeight, this, 2, 0);
	m_btnDec.SetStateImage(STATE_SELECT_NO, PAINT_STATUS_NORMAL, BMP_ERASE);
	m_btnDec.SetStateImage(STATE_SELECT_NO, PAINT_STATUS_FOCUS, BMP_ERASE);
	m_btnDec.SetStateImage(STATE_SELECT_NO, PAINT_STATUS_DISABLE, BMP_ERASE);
	m_btnDec.SetStateImage(STATE_SELECT_YES, PAINT_STATUS_NORMAL, BMP_ERASE_FOCUS);
	m_btnDec.SetStateImage(STATE_SELECT_YES, PAINT_STATUS_FOCUS, BMP_ERASE_FOCUS);
	m_btnDec.SetStateImage(STATE_SELECT_YES, PAINT_STATUS_DISABLE, BMP_ERASE_FOCUS);

	//
	SetCurState(STATE_NULL, false);
#if defined(__TW01_RILI__) 
	m_btnAdd.Show(false,false);
	m_btnDec.Show(false,false);

#endif
}

void CCfgScheduleChildDlg::OnDestroy()
{
	CCfgDlg::OnDestroy();
	if (NULL != m_pWeekSchedule)
	{
		delete [] (unsigned char*)m_pWeekSchedule;
		m_pWeekSchedule = NULL;
		m_weekScheduleLen = 0;
	}
}

void CCfgScheduleChildDlg::SetTip()
{	
	#ifdef __CUSTOM_US02_NEW__
		NCFG_ITEM_ID week;
		GetScheduleCfgID(week);
		if(week == NCFG_ITEM_REC_SCH_SCHEDULE_WEEK)
		{
			
			AddTip(m_scheduleWeekCtrl.GetID(),E_STR_ID_TIP_04_01_01);
		}
		if(week == NCFG_ITEM_REC_SCH_MOTION_WEEK)
		{
			
			this->AddTip(m_scheduleWeekCtrl.GetID(), E_STR_ID_TIP_04_01_02);

		}
		/*if(week == NCFG_ITEM_MOTION_SCH_WEEK)
		{
			
			AddTip(GetParent()->GetID(), E_STR_ID_TIP_04_01_01);
			//this->GetParent()->AddTip();

		}*/
	#else
		AddTip(m_scheduleWeekCtrl.GetID(),E_STR_ID_TIP_04_01);
	#endif
}

//void CCfgScheduleChildDlg::OnChangeFocus(unsigned long oldID, unsigned long newID)
//{
//	if( newID == m_comboCopy.GetID() ||
//		newID == m_btnCopy.GetID() )
//	{
//		OnTipCopy("");
//		return;
//	}
//
//	CCfgDlg::OnChangeFocus(oldID,newID);
//}

void CCfgScheduleChildDlg::OnTipCopy( const char* preText )
{
	if( GetShowTipType(TIP_DFOCUS) )
	{
		int chnn_count = m_maxChnn;
		int selChnn = m_comboChnn.GetCurItemData();
		int selCpy = m_comboCopy.GetCurItemData();
		if( selCpy < chnn_count && selChnn < chnn_count )
		{
			char str[16] = {0};
			std::string strOut = preText;
			strOut += m_pStrTable->String(E_STR_ID_TIP_04_02);
			sprintf(str," %d ",selChnn+1);
			strOut += str;
			strOut += m_pStrTable->String(E_STR_ID_TIP_04_03);
			sprintf(str," %d",selCpy+1);
			strOut += str;
			MessageOut(GetTopestWnd(),strOut.c_str(),TIP_DFOCUS);
		}
		else
		{
			char str[16] = {0};
			std::string strOut = preText;
			strOut += m_pStrTable->String(E_STR_ID_TIP_04_02);
			sprintf(str," %d ",selChnn+1);
			strOut += str;
			strOut += m_pStrTable->String(E_STR_ID_TIP_04_04);
			MessageOut(GetTopestWnd(),strOut.c_str(),TIP_DFOCUS);
		}
	}
}

bool CCfgScheduleChildDlg::ShowData(NCFG_ITEM_ID cfgID, const BYTE* pData, int len, bool bRefresh)
{
	assert(NULL != pData);
	assert(len > 0);

	if (cfgID == m_weekCfgID)
	{
		assert(len >= sizeof(WEEK_SCHEDULE) * m_maxChnn);
		const WEEK_SCHEDULE* pValue = reinterpret_cast<const WEEK_SCHEDULE*>(pData);

		//拷贝数据
		if (NULL != m_pWeekSchedule)
		{
			delete [] (unsigned char*)m_pWeekSchedule;
		}
		m_weekScheduleLen = len;
		assert(m_weekScheduleLen > 0);
		m_pWeekSchedule = reinterpret_cast<WEEK_SCHEDULE *>(new unsigned char [m_weekScheduleLen]);
		memcpy(m_pWeekSchedule, pValue, len);

		ShowWeekSchedule(bRefresh);
		return true;
	} 
	//else if (cfgID == m_holidayCfgID)
	//{
	//	CreateHolidayScheduleList(pData, len);

	//	return true;
	//}
	
	return false;
}

void CCfgScheduleChildDlg::CreateHolidayScheduleList(const BYTE* pData, int len)
{
	int video_count = m_maxChnn;
	assert(len >= sizeof(NCFG_INFO_HOLIDAY_SCHE_HEAD) * video_count);
	assert((len - sizeof(NCFG_INFO_HOLIDAY_SCHE_HEAD) * video_count) % sizeof(HOLIDAY_SCHEDULE) == 0);
	
	m_holidayList.clear();
	
	const NCFG_INFO_HOLIDAY_SCHE_HEAD* pHead = reinterpret_cast<const NCFG_INFO_HOLIDAY_SCHE_HEAD*>(pData);
	for (int ch = 0; ch < video_count; ++ch)
	{
		HOLIDAY_SCHEDULE_LIST list;
		const HOLIDAY_SCHEDULE* pSchedule = reinterpret_cast<const HOLIDAY_SCHEDULE*>(pData + pHead[ch].offset);
		for (int i = 0; i < pHead[ch].holidaynum; ++i, ++pSchedule)
		{
			list.push_back(*pSchedule);
		}
		
		m_holidayList.push_back(list);
	}
}

void CCfgScheduleChildDlg::ShowWeekSchedule(bool bRefresh)
{
	assert(NULL != m_pWeekSchedule);
	assert(m_weekScheduleLen >= sizeof(WEEK_SCHEDULE) * m_maxChnn);

	unsigned long cur_chnn = m_comboChnn.GetCurItemData();
	
	if(cur_chnn != COMBO_CHNN_NULL)
	{
		std::list<TIME_SEC> timeSecList;

		for (int day = 0; day < 7; ++day)
		{
			timeSecList.clear();
			CSchedule::ScheduleToTimeSec(timeSecList, m_pWeekSchedule[cur_chnn].week[day]);

			m_scheduleWeekCtrl.ClearData(day, false);
			for (std::list<TIME_SEC>::const_iterator iter = timeSecList.begin(); iter != timeSecList.end(); ++iter)
			{
				m_scheduleWeekCtrl.AddData(day, (*iter).startTimeHour, (*iter).startTimeMinute, 0, (*iter).endTimeHour, (*iter).endTimeMinute, 0);
			}
		}
	}

	if (bRefresh)
	{
		m_scheduleWeekCtrl.Repaint();
	}		
}

bool CCfgScheduleChildDlg::SetData(NCFG_ITEM_ID cfgID, BYTE*& pData, int& len)  const
{
	assert(NULL != pData);
	assert(len > 0);

	if (cfgID == m_weekCfgID)
	{
		assert(len == sizeof(WEEK_SCHEDULE) * m_maxChnn);
		WEEK_SCHEDULE* pValue = reinterpret_cast<WEEK_SCHEDULE*>(pData);

		assert(NULL != m_pWeekSchedule);
		if (NULL != m_pWeekSchedule)
		{
			memcpy(pValue, m_pWeekSchedule, len);
		}
		
		return true;
	} 
	//else if (cfgID == m_holidayCfgID)
	//{
	//	int holidayDataLen = 0;
	//	BYTE * pHolidayData = CreateHolidayData(&holidayDataLen);
	//	assert((NULL != pHolidayData) && (holidayDataLen > 0));
	//	if ((NULL != pHolidayData) && (holidayDataLen > 0))
	//	{
	//		delete [] pData;
	//		pData = pHolidayData;
	//		len = holidayDataLen;
	//	}
	//	return true;
	//}

	return false;
}

BYTE* CCfgScheduleChildDlg::CreateHolidayData(int * pLen) const
{
	//计算长度
	int scheduleLen = m_holidayList.size() * sizeof(NCFG_INFO_HOLIDAY_SCHE_HEAD);
	
	for (std::vector<HOLIDAY_SCHEDULE_LIST>::const_iterator iter = m_holidayList.begin(); iter != m_holidayList.end(); ++iter)
	{
		scheduleLen += iter->size() * sizeof(HOLIDAY_SCHEDULE);
	}
	assert(0 < scheduleLen);
	BYTE* pScheduleData = new BYTE[scheduleLen];
	memset(pScheduleData, 0, scheduleLen);

	//填充数据
	unsigned long offset = m_holidayList.size() * sizeof(NCFG_INFO_HOLIDAY_SCHE_HEAD);
	NCFG_INFO_HOLIDAY_SCHE_HEAD* pHead = reinterpret_cast<NCFG_INFO_HOLIDAY_SCHE_HEAD*>(pScheduleData);
	for (std::vector<HOLIDAY_SCHEDULE_LIST>::const_iterator iter = m_holidayList.begin(); iter != m_holidayList.end(); ++iter)
	{
		pHead->holidaynum = iter->size();
		pHead->offset = offset;
		
		HOLIDAY_SCHEDULE * pHoliday = reinterpret_cast<HOLIDAY_SCHEDULE*>(pScheduleData + offset);
		for (HOLIDAY_SCHEDULE_LIST::const_iterator schedule_iter = iter->begin(); schedule_iter != iter->end(); ++schedule_iter)
		{
			*pHoliday = *schedule_iter;
			++pHoliday;
		}
		offset += iter->size() * sizeof(HOLIDAY_SCHEDULE);
		++pHead;
	}

	if (NULL != pLen)
	{
		*pLen = scheduleLen;
	}

	return pScheduleData;
}

void CCfgScheduleChildDlg::SetCurState(int state, bool bRefresh)
{
	assert(state >= STATE_NULL);
	assert(state <= STATE_DEC);

	m_curSelOp = static_cast<STATE_SEL_OP>(state);

	if(STATE_ADD == m_curSelOp)
	{
		if(STATE_SELECT_YES != m_btnAdd.GetCurState())
		{
			m_btnAdd.SetCurState(STATE_SELECT_YES, bRefresh);
		}

		if(STATE_SELECT_NO != m_btnDec.GetCurState())
		{
			m_btnDec.SetCurState(STATE_SELECT_NO, bRefresh);
		}
	}
	else if(STATE_DEC == m_curSelOp)
	{
		if(STATE_SELECT_NO != m_btnAdd.GetCurState())
		{
			m_btnAdd.SetCurState(STATE_SELECT_NO, bRefresh);
		}

		if(STATE_SELECT_YES != m_btnDec.GetCurState())
		{
			m_btnDec.SetCurState(STATE_SELECT_YES, bRefresh);
		}
	}
	else
	{
		if(STATE_SELECT_NO != m_btnAdd.GetCurState())
		{
			m_btnAdd.SetCurState(STATE_SELECT_NO, bRefresh);
		}

		if(STATE_SELECT_NO != m_btnDec.GetCurState())
		{
			m_btnDec.SetCurState(STATE_SELECT_NO, bRefresh);
		}
	}
}

BEGIN_MSG_MAP(CCfgScheduleChildDlg, CCfgDlg)
	ON_MSG_EX(m_scheduleWeekCtrl.GetID(), KEY_VIRTUAL_DBCLICK, OnDBClickWeekSchedule)
	ON_MSG_EX(m_scheduleWeekCtrl.GetID(), KEY_VIRTUAL_ENTER, OnClickSchedule)
	ON_MSG(m_btnCopy.GetID(), KEY_VIRTUAL_ENTER, OnClickBtnCopy)
	ON_MSG(m_comboChnn.GetID(), KEY_VIRTUAL_ENTER, OnChangeChnn)
	ON_MSG(m_comboChnn.GetID(), KEY_VIRTUAL_ADD, OnChangeChnn)
	ON_MSG(m_comboChnn.GetID(), KEY_VIRTUAL_DEC, OnChangeChnn)
	ON_MSG(m_btnAdd.GetID(), KEY_VIRTUAL_ENTER, OnClickBtnAdd)
	ON_MSG(m_btnDec.GetID(), KEY_VIRTUAL_ENTER, OnClickBtnDec)
	ON_MSG(m_btnEdit.GetID(),KEY_VIRTUAL_ENTER, OnClickBtnEdit)
END_MSG_MAP()


unsigned long CCfgScheduleChildDlg::OnDBClickWeekSchedule(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	if(STATE_NULL != m_curSelOp)
	{
		return KEY_VIRTUAL_MSG_STOP;
	}

	int item = 0;
	BYTE hour, min, sec;
	if (m_scheduleWeekCtrl.HitTest(xPos, yPos, item, hour, min, sec))
	{
		CCfgScheduleEditDlg dlg;
		unsigned long cur_chnn = m_comboChnn.GetCurItemData();
		assert(cur_chnn < m_maxChnn);

		if(cur_chnn != COMBO_CHNN_NULL)
		{
			dlg.SetInitScheduleDay(item);
			dlg.SetWeekSchedule(m_pWeekSchedule[cur_chnn]);

			if (DLG_OK == dlg.DoModal(this))
			{
				dlg.GetWeekSchedule(m_pWeekSchedule[cur_chnn]);
				ShowWeekSchedule(true);

				if (!IsModify())
				{
					if (IsChange(m_weekCfgID))
					{
						SetModify(true);
					}
				}
			}
		}
	}

	return KEY_VIRTUAL_MSG_CONTINUE;
}

unsigned long CCfgScheduleChildDlg::OnClickSchedule(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	if(STATE_NULL == m_curSelOp)
	{
		return KEY_VIRTUAL_MSG_STOP;
	}

	int item = -1;
	unsigned long cur_chnn = m_comboChnn.GetCurItemData();

	if(cur_chnn == COMBO_CHNN_NULL)
	{
		return KEY_VIRTUAL_MSG_STOP;
	}

	std::list<TIME_SEC> timeSecList;
	std::list<TIME_SEC> timeSecListDec;
	BYTE hour, min, sec;
	if(m_scheduleWeekCtrl.HitTest(xPos, yPos, item, hour, min, sec))
	{
		if(STATE_ADD == m_curSelOp)
		{
			timeSecList.clear();
			CSchedule::ScheduleToTimeSec(timeSecList, m_pWeekSchedule[cur_chnn].week[item]);

			TIME_SEC tmpTmSec;
			tmpTmSec.startTimeHour = hour;
			tmpTmSec.startTimeMinute = 0;
			tmpTmSec.endTimeHour = hour;
			tmpTmSec.endTimeMinute = 59;
			timeSecList.push_back(tmpTmSec);
			CSchedule::TimeSecToSchedule(timeSecList, m_pWeekSchedule[cur_chnn].week[item]);

			timeSecList.clear();
			CSchedule::ScheduleToTimeSec(timeSecList, m_pWeekSchedule[cur_chnn].week[item]);

			m_scheduleWeekCtrl.ClearData(item, false);
			for (std::list<TIME_SEC>::const_iterator iter = timeSecList.begin(); iter != timeSecList.end(); ++iter)
			{
				m_scheduleWeekCtrl.AddData(item, (*iter).startTimeHour, (*iter).startTimeMinute, 0, (*iter).endTimeHour, (*iter).endTimeMinute, 0);
			}

			m_scheduleWeekCtrl.RepaintData(item);

			if (!IsModify())
			{
				if (IsChange(m_weekCfgID))
				{
					SetModify(true);
				}
			}

			return KEY_VIRTUAL_MSG_CONTINUE;
			
		}
		else if(STATE_DEC == m_curSelOp)
		{
			timeSecList.clear();
			CSchedule::ScheduleToTimeSec(timeSecList, m_pWeekSchedule[cur_chnn].week[item]);

			TIME_SEC tmpTmSec;
			tmpTmSec.startTimeHour = hour;
			tmpTmSec.startTimeMinute = 0;
			tmpTmSec.endTimeHour = hour;
			tmpTmSec.endTimeMinute = 59;
			timeSecListDec.push_back(tmpTmSec);
			CSchedule::TimeSecToSchedule(timeSecList, m_pWeekSchedule[cur_chnn].week[item], &timeSecListDec);

			timeSecList.clear();
			CSchedule::ScheduleToTimeSec(timeSecList, m_pWeekSchedule[cur_chnn].week[item]);

			m_scheduleWeekCtrl.ClearData(item, false);
			for (std::list<TIME_SEC>::const_iterator iter = timeSecList.begin(); iter != timeSecList.end(); ++iter)
			{
				m_scheduleWeekCtrl.AddData(item, (*iter).startTimeHour, (*iter).startTimeMinute, 0, (*iter).endTimeHour, (*iter).endTimeMinute, 0);
			}

			m_scheduleWeekCtrl.RepaintData(item);

			if (!IsModify())
			{
				if (IsChange(m_weekCfgID))
				{
					SetModify(true);
				}
			}

			return KEY_VIRTUAL_MSG_CONTINUE;
		}
		else
		{
			assert(false);
		}
	}

	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CCfgScheduleChildDlg::OnClickBtnHoliday()
{
	CCfgScheduleHolidayDlg dlg;
	int cur_chnn = m_comboChnn.GetCurItem();
	assert(cur_chnn < m_maxChnn);

	dlg.SetHolidaySchedule(m_holidayList[cur_chnn]);
	if (DLG_OK == dlg.DoModal(this))
	{
		dlg.GetHolidaySchedule(m_holidayList[cur_chnn]);

		if (!IsModify())
		{
			if (IsChange(m_holidayCfgID))
			{
				SetModify(true);
			}
		}
	}
	return KEY_VIRTUAL_MSG_CONTINUE;
}


unsigned long CCfgScheduleChildDlg::OnClickBtnCopy()
{
	unsigned long cur_chnn = m_comboChnn.GetCurItemData();
	if (cur_chnn == COMBO_CHNN_NULL)
	{
		return KEY_VIRTUAL_MSG_STOP;
	}

	assert(cur_chnn < m_maxChnn);
	unsigned long copy_chnn = m_comboCopy.GetCurItemData();

	if((cur_chnn == COMBO_CHNN_NULL) || (cur_chnn == COMBO_CHNN_NULL))
	{
		return KEY_VIRTUAL_MSG_CONTINUE;
	}

	if (cur_chnn != copy_chnn)
	{
		if (copy_chnn != COMBO_CHNN_ALL)
		{
			m_pWeekSchedule[copy_chnn] = m_pWeekSchedule[cur_chnn];
			//m_holidayList[copy_chnn] = m_holidayList[cur_chnn];
		}
		else//Copy to all
		{
			ULONGLONG CH = 0;
			int localVideoInputNum = 0, netVideoInputNum = 0;
			int maxChnn = 0, effectChnn = 0;
			
			if(TYPE_ALL == m_typeCH)
			{
				for(int i = 0; i < m_maxChnn; i++)
				{
					CH |= (0x1 << i);
				}
			}
			else
			{
				IsSupportCH(CH, &effectChnn, &localVideoInputNum, &netVideoInputNum);
			}

			//maxChnn = localVideoInputNum + netVideoInputNum;

			for (int i = 0; i < m_maxChnn; ++i)
			{
				if((CH >> i) & 0x1)
				{
					m_pWeekSchedule[i] = m_pWeekSchedule[cur_chnn];
					//m_holidayList[i] = m_holidayList[cur_chnn];
				}
			}
		}

		if (!IsModify())
		{
			if (IsChange(m_weekCfgID))
			{
				SetModify(true);
			}
		}

		OnTipCopy(m_pStrTable->String(E_STR_ID_TIP_04_05));
	}

	return KEY_VIRTUAL_MSG_CONTINUE;
}


unsigned long CCfgScheduleChildDlg::OnChangeChnn()
{
	int cur_chnn = m_comboChnn.GetCurItem();
	int last_chnn = m_comboChnn.GetData();

	if (cur_chnn != last_chnn)
	{
		m_comboChnn.SetData(cur_chnn);

		ShowWeekSchedule(true);
	}	

	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CCfgScheduleChildDlg::OnClickBtnAdd()
{
	if(STATE_ADD != m_curSelOp)
	{
		SetCurState(STATE_ADD, true);
	}
	else
	{
		SetCurState(STATE_NULL, true);
	}

	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CCfgScheduleChildDlg::OnClickBtnDec()
{
	if(STATE_DEC != m_curSelOp)
	{
		SetCurState(STATE_DEC, true);
	}
	else
	{
		SetCurState(STATE_NULL, true);
	}

	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CCfgScheduleChildDlg::OnClickBtnEdit()
{
#if defined(__TW01_RILI__)
	CCfgScheduleEditDlg dlg;
	unsigned long cur_chnn = m_comboChnn.GetCurItemData();
	assert(cur_chnn < m_maxChnn);

	if(cur_chnn != COMBO_CHNN_NULL)
	{
		dlg.SetInitScheduleDay(0);
		dlg.SetWeekSchedule(m_pWeekSchedule[cur_chnn]);

		if (DLG_OK == dlg.DoModal(this))
		{
			dlg.GetWeekSchedule(m_pWeekSchedule[cur_chnn]);
			ShowWeekSchedule(true);

			if (!IsModify())
			{
				if (IsChange(m_weekCfgID))
				{
					SetModify(true);
				}
			}
		}
	}
#endif

	return KEY_VIRTUAL_MSG_STOP;
}
//end
