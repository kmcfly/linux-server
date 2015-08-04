/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : ysw
** Date         : 2009-08-01
** Name         : EventSearchDlg.cpp
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/
#include "EventSearchDlg.h"
#include "ListCtrlEx.cpp"
#include "TimeCtrl.h"
#include "FuncCustom.h"

extern NET_LOGIN_SUCCESS_REPLY *g_p_login_succ;
//////////////////////////////////////////////////////////////////////////
template <class T>
std::string &  GUI::CListCtrlEx<T>::Text(T &node, unsigned short col)
{
	RECLOG_INFO_EX &it = node;
	char str [512] = {0};

	int localChnnNum = CProduct::Instance()->LocalVideoInputNum();
	if (0 == col)
	{
		if (RECORD_FILE_IS_UNOFFICAL(it.type))
		{
			sprintf(str, "%d+", it.chnn + 1);
		}
		else
		{
			sprintf(str, "%d", it.chnn + 1);
		}
	}

	if (1 == col)
	{
		if (RECORD_TYPE_MANUAL & it.type)
		{
			strcpy(str, m_pStrTable->String(E_STR_ID_MANUAL));
		}

		if (RECORD_TYPE_SCHEDULE & it.type)
		{
			strcpy(str, m_pStrTable->String(E_STR_ID_REC_SCHEDULE));
		}

		if (RECORD_TYPE_MOTION & it.type)
		{
			strcpy(str, m_pStrTable->String(E_STR_ID_MOTION));
		}

		if (RECORD_TYPE_SENSOR & it.type)
		{
			strcpy(str, m_pStrTable->String(E_STR_ID_SENSOR));
		}
	}

	if (2 == col)
	{
		//tm time = DVRTime32ToTm(it.startTime);
		//sprintf(str, "%d-%02d-%02d %02d:%02d:%02d", time.tm_year+1900, time.tm_mon+1, time.tm_mday, time.tm_hour, time.tm_min, time.tm_sec);
		std::string strtime;
		CTimeCtrl::GetTime32Text(strtime,it.startTime);
		strcpy(str,strtime.c_str());
	}

	if (3 == col)
	{
		//tm time = DVRTime32ToTm(it.endTime);
		//sprintf(str, "%d-%02d-%02d %02d:%02d:%02d", time.tm_year+1900, time.tm_mon+1, time.tm_mday, time.tm_hour, time.tm_min, time.tm_sec);
		std::string strtime;
		CTimeCtrl::GetTime32Text(strtime,it.endTime);
		strcpy(str,strtime.c_str());
	}

	m_str = str;
	return m_str;
}
//////////////////////////////////////////////////////////////////////////
CEventSearchDlg::CEventSearchDlg()
{
	m_channelNum = 0;
}

CEventSearchDlg::~CEventSearchDlg()
{

}

void CEventSearchDlg::SetInitialInfo(unsigned long channelNum)
{
	m_channelNum = channelNum;
}

void CEventSearchDlg::OnSearch(unsigned long startTime, unsigned long endTime, ULONGLONG chnnBits, unsigned long dataOwnerby)
{
	//Ìí¼ÓËÑË÷´¦Àí
	if ((!m_ckMotion.IsCheck()) && (!m_ckSensor.IsCheck()) && (!m_ckAll.IsCheck()))
	{
		MessageBox(m_pStrTable->String(E_STR_ID_MESSAGE_35), m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK | DEFAULT_ICON_WARNING);
		return ;
	}

	MESSAGE_DATA msgData;
	NET_DATA_SEARCH searchCond;
	searchCond.searchType = DATA_SEARCH_TYPE_EVENT;
	searchCond.videoCH = chnnBits;
	searchCond.startTime = startTime;
	searchCond.endTime = endTime;
	searchCond.RecordTypeMASK = RECORD_TYPE_NULL;
	searchCond.dataOwnerby = dataOwnerby;

	if (m_ckMotion.IsCheck())
	{
		searchCond.RecordTypeMASK |= RECORD_TYPE_MOTION;
	}

	if (m_ckSensor.IsCheck())
	{
		searchCond.RecordTypeMASK |= RECORD_TYPE_SENSOR;
	} 

	if (m_ckAll.IsCheck())
	{
		searchCond.RecordTypeMASK |= RECORD_TYPE_MANUAL;
		searchCond.RecordTypeMASK |= RECORD_TYPE_SCHEDULE;
		searchCond.RecordTypeMASK |= RECORD_TYPE_MOTION;
		searchCond.RecordTypeMASK |= RECORD_TYPE_SENSOR;
	}

	memset(&msgData, 0, sizeof(MESSAGE_DATA));
	CMessageMan::Instance()->SendMsgDataToServer(0, LOCAL_DEVICE_ID, CMD_REQUEST_DATA_SEARCH, (unsigned char *)&searchCond, sizeof(NET_DATA_SEARCH), msgData);

	RECLOG_INFO_EX *pDate = (RECLOG_INFO_EX *)(msgData.pData + sizeof(PACKCMD) + sizeof(NET_DATA_INFO));
	int num = (msgData.dataLen - sizeof(PACKCMD) - sizeof(NET_DATA_INFO))/sizeof(RECLOG_INFO_EX);
	if (0 == num)
	{
		MessageBox(m_pStrTable->String(E_STR_ID_MESSAGE_36), m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK | DEFAULT_ICON_WARNING);
		CMessageMan::Instance()->ReleaseMsgBuff(msgData);	
		return;
	}

	RECLOG_INFO_EX *pReclong = new RECLOG_INFO_EX[num];
	for (int count = 0; count < num; ++count)
	{	
		pReclong[count] = pDate[count];
	}

	m_listCtrlEx.PutData(pReclong, num);
	pReclong = NULL;
	CMessageMan::Instance()->ReleaseMsgBuff(msgData);	
}

unsigned long CEventSearchDlg::GetPlaybackInfo(FILE_INFO_EX & fileInfo)
{
	unsigned short row = 0;	
	row = m_listCtrlEx.GetSingleSelect();
	if ((row >= m_listCtrlEx.GetCount()) || (row < 0))
	{
		return KEY_VIRTUAL_MSG_STOP;
	}

	RECLOG_INFO_EX * pReclogInfo = NULL;
	pReclogInfo = m_listCtrlEx.GetListData();
	if (NULL == pReclogInfo)
	{
		return KEY_VIRTUAL_MSG_STOP;
	}

	if (pReclogInfo[row].chnn >= m_channelNum)
	{
		return KEY_VIRTUAL_MSG_STOP;
	}

	if (pReclogInfo[row].startTime >= pReclogInfo[row].endTime)
	{
		return KEY_VIRTUAL_MSG_STOP;
	}

	m_selRelogInfo = pReclogInfo[row];

	fileInfo.startTime	= m_selRelogInfo.startTime;
	fileInfo.endTime	= m_selRelogInfo.endTime;
	fileInfo.chnn		= m_selRelogInfo.chnn;
	fileInfo.diskIndex	= m_selRelogInfo.diskIndex;
	fileInfo.fileIndex	= m_selRelogInfo.fileIndex;
	fileInfo.type		= m_selRelogInfo.type;
	fileInfo.deviceID	= m_selRelogInfo.deviceID;
	fileInfo.cameraID	= m_selRelogInfo.cameraID;

	return KEY_VIRTUAL_MSG_CONTINUE;
}

//////////////////////////////////////////////////////////////////////////
void CEventSearchDlg::OnInitial()
{
	assert(m_channelNum > 0);
	GUI::CDialog::OnInitial();

	//////////////////////////////////////////////////////////////////////////
	unsigned short x = m_length[DLG_OFFSET_SUB_LEFT], y = m_length[DLG_OFFSET_SUB_TOP];
	unsigned short width = m_rect.Width() - 2 * m_length[DLG_OFFSET_SUB_LEFT];
	unsigned short height = m_rect.Height() - y - m_length[DLG_OFFSET_BOTTOM] - m_length[CHECK_HEIGHT];

	m_listCtrlEx.Create(GUI::CWnd::GetRandWndID(), x, y, width, height, this, 0, 0);
	m_listCtrlEx.AddColumn(m_pStrTable->String(E_STR_ID_CH), width *  3 / 32);
	m_listCtrlEx.AddColumn(m_pStrTable->String(E_STR_ID_TYPE), width * 7 / 32 );
	m_listCtrlEx.AddColumn(m_pStrTable->String(E_STR_ID_START_TIME), width * 11 / 32);
	m_listCtrlEx.AddColumn(m_pStrTable->String(E_STR_ID_END_TIME), width * 11 / 32);
	m_listCtrlEx.EnableSingleSelect();


	GUI::CRect rect = m_listCtrlEx.GetClientRect();
	y += rect.Height() + 4;

	m_ckMotion.Create(GUI::CWnd::GetRandWndID(), x, y, m_length[BTN_WIDTH] + width * 4 / 32, m_length[CHECK_HEIGHT], this, 0, 1);
	x += m_length[BTN_WIDTH] + width * 4 / 32;

	if (IsSupportAlarmOut() || (g_p_login_succ->productInfo.netVideoInputNum > 0))
	{
		m_ckSensor.Create(GUI::CWnd::GetRandWndID(), x, y, m_length[BTN_WIDTH] + width * 4 / 32, m_length[CHECK_HEIGHT], this, 1, 1);
		x += m_length[BTN_WIDTH] + width * 4 / 32;
		m_ckAll.Create(GUI::CWnd::GetRandWndID(), x, y, m_length[BTN_WIDTH] + width * 4 / 32, m_length[CHECK_HEIGHT], this, 2, 1);
	}
	else
	{
		m_ckAll.Create(GUI::CWnd::GetRandWndID(), x, y, m_length[BTN_WIDTH] + width * 4 / 32, m_length[CHECK_HEIGHT], this, 1, 1);
	}

	m_ckMotion.SetCaption(m_pStrTable->String(E_STR_ID_MOTION), false, false);
	m_ckSensor.SetCaption(m_pStrTable->String(E_STR_ID_SENSOR), false, false);
	m_ckAll.SetCaption(m_pStrTable->String(E_STR_ID_ALL), false, false);

	m_ckMotion.SetCheck(true, false);
	m_ckSensor.SetCheck(true, false);
	m_ckAll.SetCheck(false, false);
}

void CEventSearchDlg::EndDialog(DLG_END_VALUE result)
{
	m_listCtrlEx.PutData(NULL, 0);
	GUI::CDialog::EndDialog(result);	
}

BEGIN_MSG_MAP(CEventSearchDlg, GUI::CDialog)
	ON_MSG_EX(m_listCtrlEx.GetID(), KEY_VIRTUAL_DBCLICK, OnClickList)
	ON_MSG(m_ckMotion.GetID(), KEY_VIRTUAL_ENTER, OnCkMotion)
	ON_MSG(m_ckSensor.GetID(), KEY_VIRTUAL_ENTER, OnCkSensor)
	ON_MSG(m_ckAll.GetID(), KEY_VIRTUAL_ENTER, OnCkAll)
	ON_MSG_EX(CTRLID_NULL, KEY_VIRTUAL_PLAY, OnClickList)
END_MSG_MAP()



unsigned long CEventSearchDlg::OnClickList(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{     
	return KEY_VIRTUAL_PLAY;
}

unsigned long CEventSearchDlg::OnCkMotion()
{
	if(m_ckMotion.IsCheck() || m_ckSensor.IsCheck())
	{
		if(m_ckAll.IsCheck())
		{
			m_ckAll.SetCheck(false, true);
		}
	}

	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CEventSearchDlg::OnCkSensor()
{
	if(m_ckMotion.IsCheck() || m_ckSensor.IsCheck())
	{
		if(m_ckAll.IsCheck())
		{
			m_ckAll.SetCheck(false, true);
		}
	}

	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CEventSearchDlg::OnCkAll()
{
	if(m_ckAll.IsCheck())
	{
		if(m_ckMotion.IsCheck())
		{
			m_ckMotion.SetCheck(false, true);
		}

		if(m_ckSensor.IsCheck())
		{
			m_ckSensor.SetCheck(false, true);
		}
	}

	return KEY_VIRTUAL_MSG_STOP;
}














