/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : hz
** Date         : 2009-08-01
** Name         : BackupDlg.cpp
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/
#include "BackupDlg.h"
#include "CalendarDlg.h"
#include "ListCtrlEx.cpp"
#include "PlaybackDlg.h"
#include "UserMan.h"

extern NET_LOGIN_SUCCESS_REPLY *g_p_login_succ;
//////////////////////////////////////////////////////////////////////////

//在FileManDlg.cpp文件中已经有一份了。

template <class T>
std::string &  GUI::CListCtrlEx<T>::Text(T &node, unsigned short col)
{
	char str [512] = {0};
#ifdef __ENVIRONMENT_LINUX__
	FILE_INFO_EX_BACKUP &it = node;

	if (0 == col)
	{
		if (RECORD_FILE_IS_UNOFFICAL(it.fileInfo.type))
		{
			sprintf(str, "%d+", it.fileInfo.chnn + 1);
		}
		else
		{
			sprintf(str, "%d", it.fileInfo.chnn + 1);
		}
	}

	if (1 == col)
	{
		//tm time = DVRTime32ToTm(it.startTime);
		//sprintf(str, "%d-%02d-%02d %02d:%02d:%02d", time.tm_year+1900, time.tm_mon+1, time.tm_mday, time.tm_hour, time.tm_min, time.tm_sec);
		std::string strtime;
		CTimeCtrl::GetTime32Text(strtime,it.fileInfo.startTime);
		strcpy(str,strtime.c_str());
	}

	if (2 == col)
	{
		//tm time = DVRTime32ToTm(it.endTime);
		//sprintf(str, "%d-%02d-%02d %02d:%02d:%02d", time.tm_year+1900, time.tm_mon+1, time.tm_mday, time.tm_hour, time.tm_min, time.tm_sec);
		std::string strtime;
		CTimeCtrl::GetTime32Text(strtime,it.fileInfo.endTime);
		strcpy(str,strtime.c_str());
	}

	if (3 == col)
	{
		if (RECORD_FILE_IS_LOCK(it.fileInfo.type))
		{
			strcpy(str, m_pStrTable->String(E_STR_ID_LOCKED));
		}
		else if (RECORD_FILE_IS_ONLYREAD(it.fileInfo.type))
		{
			strcpy(str, m_pStrTable->String(E_STR_ID_READ_ONLY));			
		}
		else if (RECORD_FILE_IS_WRITING(it.fileInfo.type))
		{
			//更准确的说是：正在录像或等待写
			strcpy(str, m_pStrTable->String(E_STR_ID_WRITING));
		}
		else if (RECORD_FILE_IS_READING(it.fileInfo.type))
		{
			strcpy(str, m_pStrTable->String(E_STR_ID_READING));
		}		
	}
#else
	memset(str, 0, sizeof(str));
#endif

	m_str = str;
	return m_str;
}


CBackupDlg::CBackupDlg()
{
	m_chnnNum = 0;
	m_diskOwnerIndex = 0;
}

CBackupDlg::~CBackupDlg()
{

}

void CBackupDlg::SetInitialInfo(unsigned char chnnNum, unsigned long diskOwnerIndex)
{
	m_chnnNum = chnnNum;
	m_diskOwnerIndex = diskOwnerIndex;
}

//////////////////////////////////////////////////////////////////////////
void CBackupDlg::OnInitial()
{
	GUI::CDialog::OnInitial();
	SetImage(BMP_MEM_DLG_LARGE, false);
	SetCaption(m_pStrTable->String(E_STR_ID_TITLE_BACKUP), false);

	SetDefaultBtn(DEFAULT_BTN_APPLY | DEFAULT_BTN_OK | DEFAULT_BTN_CANCEL );
	SetDefaultBtnCaption(DEFAULT_BTN_APPLY, m_pStrTable->String(E_STR_ID_BACKUP));
	SetDefaultBtnCaption(DEFAULT_BTN_OK, m_pStrTable->String(E_STR_ID_SEARCH));
	SetDefaultBtnCaption( DEFAULT_BTN_CANCEL, m_pStrTable->String(E_STR_ID_EXIT) );
	//////////////////////////////////////////////////////////////////////////

	if( GUI::CWnd::m_width >= 1024 && GUI::CWnd::m_height >= 768 )
	{
		CreateUnfoldCtrl();
	}
	else
	{
		CreateFoldCtrl();
	}

	//////////////////////////////////////////////////////////////////////////

	tm tmTime = DVRTime32ToTm(GetCurrTime32());
	//tmTime.tm_mon = tmTime.tm_mon;
	//tmTime.tm_year = tmTime.tm_year + 1900;
	//tmTime.tm_year = (tmTime.tm_year < 2000) ? 2000 : tmTime.tm_year;
	tmTime.tm_hour = 0;
	tmTime.tm_min  = 0;
	tmTime.tm_sec  = 0;
	
#if defined(__GROUY_SEARCH__)
	{
		if (GUI::CWnd::m_width >= 1024 && GUI::CWnd::m_height >= 768)
		{
			m_searchGroupTimeDlg.SetStartTime(tmTime);
		}
		else
		{
			m_searchGroupScrollTimeDlg.SetStartTime(tmTime);
		}
	}
#else
	{
		if (GUI::CWnd::m_width >= 1024 && GUI::CWnd::m_height >= 768 ||m_chnnNum <= 16)
		{
			m_searchCdn2TimeDlg.SetStartTime(tmTime);
		}
		else
		{
			m_searchCdnTimeDlg.SetStartTime(tmTime);
		}
	}
#endif
	


	tmTime.tm_hour = 23;
	tmTime.tm_min  = 59;
	tmTime.tm_sec  = 59;
#if defined(__GROUY_SEARCH__)
	{
		if (GUI::CWnd::m_width >= 1024 && GUI::CWnd::m_height >= 768)
		{
			m_searchGroupTimeDlg.SetEndTime(tmTime);
		}
		else
		{
			m_searchGroupScrollTimeDlg.SetEndTime(tmTime);
		}
	}
	
#else
	{
		if (GUI::CWnd::m_width >= 1024 && GUI::CWnd::m_height >= 768 || m_chnnNum <= 16)
		{
			m_searchCdn2TimeDlg.SetEndTime(tmTime);
		}
		else
		{
			m_searchCdnTimeDlg.SetEndTime(tmTime);
		}
	}
#endif
	
	
}

void CBackupDlg::OnPostInitial()
{
	GUI::CDialog::OnPostInitial();
	GUI::CButton *pBtn = dynamic_cast<GUI::CButton*>(GetCtrl(CTRLID_DEFAULT_BTN_APPLY));
	assert(pBtn != NULL);
	pBtn->SetEnable(false, false);
}

void CBackupDlg::CreateFoldCtrl()
{
#if defined(__GROUY_SEARCH__)
	m_searchCdn2TimeDlgWidth = m_length[CALENDAR_WIDTH] + 30;
#else
	if (m_chnnNum > 16)
	{
		m_searchCdn2TimeDlgWidth = m_length[CALENDAR_WIDTH] + 30;
	}
	else
	{
		m_searchCdn2TimeDlgWidth = m_length[CALENDAR_WIDTH] + 10;
	}
#endif
	unsigned short xLeft = m_length[DLG_OFFSET_LEFT], yTop = m_length[TITLE_HEIGHT] + m_length[DLG_OFFSET_TOP];
	unsigned short width = m_rect.Width() - 2 * m_length[DLG_OFFSET_LEFT];

	unsigned short x = xLeft, y = yTop;

	unsigned short listH = m_rect.Height() - yTop - m_length[DEFAULT_BTN_BOTTOM_YPOS] - m_length[DLG_OFFSET_BOTTOM] ;
	m_listCtrlExBackup.Create(GUI::CWnd::GetRandWndID(), x, y, width, listH, this, 0, 1, true );

	m_listCtrlExBackup.AddColumn(m_pStrTable->String(E_STR_ID_CH), width * 3 / 32);
	m_listCtrlExBackup.AddColumn(m_pStrTable->String(E_STR_ID_START_TIME), width * 11 / 32);
	m_listCtrlExBackup.AddColumn(m_pStrTable->String(E_STR_ID_END_TIME), width * 11 / 32);
	m_listCtrlExBackup.AddColumn(m_pStrTable->String(E_STR_ID_STATUS), width * 5 / 32);
	m_listCtrlExBackup.EnableSingleSelect();

	//调整List位置
	GUI::CRect rect = m_listCtrlExBackup.GetClientRect();
	unsigned short yOff = ( m_rect.Height()- yTop - m_length[DEFAULT_BTN_BOTTOM_YPOS] - m_length[DLG_OFFSET_BOTTOM] - rect.Height() )  / 2;
	x = xLeft;
	y = yTop + yOff;
	m_listCtrlExBackup.MoveWnd( m_rect.m_left + x, m_rect.m_top + y );

	ULONGLONG authCH = (g_p_login_succ->productInfo.bPasswordCheck) ? g_p_login_succ->authBackupCH : ALL_CHNN_MASK(g_p_login_succ->productInfo.videoInputNum);
	x = xLeft+ width - m_searchCdn2TimeDlgWidth;
#if defined(__GROUY_SEARCH__)
	m_searchGroupScrollTimeDlg.SetInitialInfo(g_p_login_succ->productInfo.localVideoInputNum, true,authCH);
	m_searchGroupScrollTimeDlg.Create(GUI::CWnd::GetRandWndID(), x, y, m_searchCdn2TimeDlgWidth, rect.Height(), this, 2, 1);
#else
	if (m_chnnNum > 16)
	{
		m_searchCdnTimeDlg.SetInitialInfo(g_p_login_succ->productInfo.videoInputNum, authCH);
		m_searchCdnTimeDlg.Create(GUI::CWnd::GetRandWndID(), x, y, m_searchCdn2TimeDlgWidth, rect.Height(), this, 2, 1);
	}
	else
	{
		m_searchCdn2TimeDlg.SetInitialInfo(g_p_login_succ->productInfo.videoInputNum, authCH);
		m_searchCdn2TimeDlg.Create(GUI::CWnd::GetRandWndID(), x, y, m_searchCdn2TimeDlgWidth, rect.Height(), this, 2, 1);
	}
#endif
	x -= m_length[FLEX_BTN_WIDTH];
	m_btnFlex.Create(GUI::CWnd::GetRandWndID(), x, (m_rect.Height()/2)-(m_length[FLEX_BTN_HEIGHT]/2), m_length[FLEX_BTN_WIDTH], m_length[FLEX_BTN_HEIGHT], this, 1, 1);

	m_listCtrlExBackup.SetWndSize(false, false, width - m_searchCdn2TimeDlgWidth - m_length[FLEX_BTN_WIDTH], rect.Height());

	m_btnFlex.SetImage(PAINT_STATUS_NORMAL, BMP_BUTTON_DLG_FLEX_R, false);
	m_btnFlex.SetImage(PAINT_STATUS_FOCUS, BMP_BUTTON_DLG_FLEX_R_F, false);

	m_bFlexSearchConditionDlg = false;

	//////////////////////////////////////////////////////////////////////////
}


void CBackupDlg::CreateUnfoldCtrl()
{
	m_searchCdn2TimeDlgWidth = m_length[CALENDAR_WIDTH] + 10;
	unsigned short xLeft = m_length[DLG_OFFSET_LEFT], yTop = m_length[TITLE_HEIGHT] + m_length[DLG_OFFSET_TOP];
	unsigned short width = m_rect.Width() - 2 * m_length[DLG_OFFSET_LEFT];
	unsigned short listWidth = width - m_searchCdn2TimeDlgWidth - 4;
	unsigned short x = xLeft, y = yTop;

	unsigned short listH = m_rect.Height() - yTop - m_length[DEFAULT_BTN_BOTTOM_YPOS] - m_length[DLG_OFFSET_BOTTOM] ;
	m_listCtrlExBackup.Create(GUI::CWnd::GetRandWndID(), x, y, listWidth, listH, this, 0, 1, true );

	m_listCtrlExBackup.AddColumn(m_pStrTable->String(E_STR_ID_CH), width * 3 / 32);
	m_listCtrlExBackup.AddColumn(m_pStrTable->String(E_STR_ID_START_TIME), width * 11 / 32);
	m_listCtrlExBackup.AddColumn(m_pStrTable->String(E_STR_ID_END_TIME), width * 11 / 32);
	m_listCtrlExBackup.AddColumn(m_pStrTable->String(E_STR_ID_STATUS), width * 5 / 32);
	m_listCtrlExBackup.EnableSingleSelect();

	//调整List位置
	GUI::CRect rect = m_listCtrlExBackup.GetClientRect();
	unsigned short yOff = ( m_rect.Height()- yTop - m_length[DEFAULT_BTN_BOTTOM_YPOS] - m_length[DLG_OFFSET_BOTTOM] - rect.Height() )  / 2;
	x = xLeft;
	y = yTop + yOff;
	m_listCtrlExBackup.MoveWnd( m_rect.m_left + x, m_rect.m_top + y );

	ULONGLONG authCH = (g_p_login_succ->productInfo.bPasswordCheck) ? g_p_login_succ->authBackupCH : ALL_CHNN_MASK(g_p_login_succ->productInfo.videoInputNum);
	x = xLeft+ width - m_searchCdn2TimeDlgWidth;
#if defined(__GROUY_SEARCH__)
	m_searchGroupTimeDlg.SetInitialInfo(g_p_login_succ->productInfo.videoInputNum, true,authCH);
	m_searchGroupTimeDlg.Create(GUI::CWnd::GetRandWndID(), x, y, m_searchCdn2TimeDlgWidth, rect.Height(), this, 1, 1);
	m_searchGroupTimeDlg.Show(true,false);
#else
	m_searchCdn2TimeDlg.SetInitialInfo(g_p_login_succ->productInfo.videoInputNum, authCH);
	m_searchCdn2TimeDlg.Create(GUI::CWnd::GetRandWndID(), x, y, m_searchCdn2TimeDlgWidth, rect.Height(), this, 1, 1);
#endif
	//////////////////////////////////////////////////////////////////////////
}


unsigned long CBackupDlg::OnApply()
{
	FILE_INFO_EX_BACKUP *pDataBackup = NULL;
	unsigned long count = 0;

#ifndef __ENVIRONMENT_WIN32__
	MESSAGE_DATA msgData;
	memset(&msgData, 0, sizeof(MESSAGE_DATA));
	PACKCMD *pCmd = NULL;
	unsigned long cmdType;
#endif

	DVR_DISK_INFO *pPartionInfo = NULL;
	DVR_DISK_INFO_LIST partionInfoList;
	int partionNum = 0;

	unsigned long startTime = 0;

	//获取要进行备份的数据
	pDataBackup = m_listCtrlExBackup.GetSelectData(count);
	if (0 == count)
	{
		MessageBox(m_pStrTable->String(E_STR_ID_MESSAGE_50), m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK | DEFAULT_ICON_WARNING);
		return KEY_VIRTUAL_MSG_STOP;
	}


	//查找硬件设备:uSB， 硬盘硬盘，光驱	
	partionInfoList.RemoveAll();
#ifndef __ENVIRONMENT_WIN32__    
	//查找所有的热拔插设备的分区信息
	CMessageMan::Instance()->SendMsgDataToServer(0, LOCAL_DEVICE_ID, CMD_REQUEST_DISK_GET_REMOVABLE_PARTION, NULL, 0, msgData);
	pPartionInfo = (DVR_DISK_INFO *)(msgData.pData + sizeof(PACKCMD));
	partionNum = (msgData.dataLen - sizeof(PACKCMD))/sizeof(DVR_DISK_INFO);
	if (partionNum > 0)
	{
		for (int i = 0; i < partionNum; ++i)
		{	
			DVR_DISK_INFO &tmp =	pPartionInfo[i];
			partionInfoList.AddTail(tmp);
		}
	}
	CMessageMan::Instance()->ReleaseMsgBuff(msgData);
	
	//获取光驱的信息
	memset(&msgData, 0, sizeof(MESSAGE_DATA));
	CMessageMan::Instance()->SendMsgDataToServer(0, LOCAL_DEVICE_ID, CMD_REQUEST_DISK_GET_CD_INFO, NULL, 0, msgData);
	pPartionInfo = (DVR_DISK_INFO *)(msgData.pData + sizeof(PACKCMD));
	partionNum = (msgData.dataLen - sizeof(PACKCMD))/sizeof(DVR_DISK_INFO);
	if (partionNum > 0)
	{
		for (int i = 0; i < partionNum; ++i)
		{	
			DVR_DISK_INFO tmp =	pPartionInfo[i];
			partionInfoList.AddTail(tmp);
		}
	}
	CMessageMan::Instance()->ReleaseMsgBuff(msgData);

    
	if (0 == partionInfoList.GetCount())
	{
		MessageBox(m_pStrTable->String(E_STR_ID_MESSAGE_07),m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK | DEFAULT_ICON_WARNING);
		if (NULL != pDataBackup)
		{
			delete [] pDataBackup;
			pDataBackup = NULL;
		}
		return KEY_VIRTUAL_MSG_STOP;
	}

	if ((1 == partionInfoList.GetCount()) 
	  &&(NET_DISK_TYPE_CDDVD == partionInfoList.GetHead().diskType)
	  &&(NET_DISK_NO_CD == partionInfoList.GetHead().diskStatus))
	{
		MessageBox(m_pStrTable->String(E_STR_ID_MESSAGE_60), m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK | DEFAULT_ICON_WARNING);
		if (NULL != pDataBackup)
		{
			delete [] pDataBackup;
			pDataBackup = NULL;
		}
	    partionInfoList.RemoveAll();
		return KEY_VIRTUAL_MSG_STOP;
	}

#else
	DVR_DISK_INFO partionInfo;
	partionInfo.diskType = NET_DISK_TYPE_REMOVEABLE;
	partionInfo.diskInterface = NET_DISK_USB_INTERFACE;
	partionInfo.diskStatus = NET_DISK_IS_MOUNT;
	strcpy(partionInfo.diskname, "/dev/sdb1");
	strcpy(partionInfo.mountdir, "F:/backup");
	partionInfo.totalSpace = 0;
	partionInfo.freeSpace  = 0;
	partionInfoList.AddTail(partionInfo);

	DVR_DISK_INFO partionInfo2;
	partionInfo2.diskType = NET_DISK_TYPE_CDDVD;
	partionInfo2.diskInterface = NET_DISK_CDDVD_INTERFACE;
	strcpy(partionInfo2.diskname, "/dev/cdrom");
	strcpy(partionInfo2.mountdir, "F:/backup");
	partionInfo2.totalSpace = 0;
	partionInfo2.freeSpace  = 0;
	partionInfoList.AddTail(partionInfo2);

#endif	

	m_backUpInfoDlg.SetInitialInfo(&partionInfoList, pDataBackup, count, m_startTime, m_endTime,m_diskOwnerIndex);
	m_backUpInfoDlg.DoModal(this);

	
	if (NULL != pDataBackup)
	{
		delete [] pDataBackup;
		pDataBackup = NULL;
	}	
	partionInfoList.RemoveAll();

	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CBackupDlg::OnOK()
{

	MESSAGE_DATA msgData;
	memset(&msgData, 0, sizeof(MESSAGE_DATA));
	NET_DATA_SEARCH searchCond;
	
#if defined(__GROUY_SEARCH__)
	{
		if (GUI::CWnd::m_width >=  1024 && GUI::CWnd::m_height >= 768)
		{
			if (0 == m_searchGroupTimeDlg.CH())
			{
				MessageBox(m_pStrTable->String(E_STR_ID_MESSAGE_34), m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK | DEFAULT_ICON_WARNING);
				return KEY_VIRTUAL_MSG_STOP;
			}
			if (m_searchGroupTimeDlg.TimeStart() >= m_searchGroupTimeDlg.TimeEnd())
			{
				MessageBox(m_pStrTable->String(E_STR_ID_MESSAGE_51), m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK | DEFAULT_ICON_WARNING);
				return KEY_VIRTUAL_MSG_STOP;
			}
			searchCond.searchType = DATA_SEARCH_TYPE_FILE;
			searchCond.videoCH = m_searchGroupTimeDlg.CH();
			searchCond.startTime = m_searchGroupTimeDlg.TimeStart();
			searchCond.endTime = m_searchGroupTimeDlg.TimeEnd();
			searchCond.dataOwnerby = m_diskOwnerIndex;
		}
		else
		{
			if (0 == m_searchGroupScrollTimeDlg.CH())
			{
				MessageBox(m_pStrTable->String(E_STR_ID_MESSAGE_34), m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK | DEFAULT_ICON_WARNING);
				return KEY_VIRTUAL_MSG_STOP;
			}
			if (m_searchGroupScrollTimeDlg.TimeStart() >= m_searchGroupScrollTimeDlg.TimeEnd())
			{
				MessageBox(m_pStrTable->String(E_STR_ID_MESSAGE_51), m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK | DEFAULT_ICON_WARNING);
				return KEY_VIRTUAL_MSG_STOP;
			}
			searchCond.searchType = DATA_SEARCH_TYPE_FILE;
			searchCond.videoCH = m_searchGroupScrollTimeDlg.CH();
			searchCond.startTime = m_searchGroupScrollTimeDlg.TimeStart();
			searchCond.endTime = m_searchGroupScrollTimeDlg.TimeEnd();
			searchCond.dataOwnerby = m_diskOwnerIndex;

		}
	}
#else
	{
		if (GUI::CWnd::m_width >=  1024 && GUI::CWnd::m_height >= 768 ||  m_chnnNum <= 16)
		{
			if (0 == m_searchCdn2TimeDlg.CH())
			{
				MessageBox(m_pStrTable->String(E_STR_ID_MESSAGE_34), m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK | DEFAULT_ICON_WARNING);
				return KEY_VIRTUAL_MSG_STOP;
			}

			if (m_searchCdn2TimeDlg.TimeStart() >= m_searchCdn2TimeDlg.TimeEnd())
			{
				MessageBox(m_pStrTable->String(E_STR_ID_MESSAGE_51), m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK | DEFAULT_ICON_WARNING);
				return KEY_VIRTUAL_MSG_STOP;
			}

			searchCond.searchType = DATA_SEARCH_TYPE_FILE;
			searchCond.videoCH = m_searchCdn2TimeDlg.CH();
			searchCond.startTime = m_searchCdn2TimeDlg.TimeStart();
			searchCond.endTime = m_searchCdn2TimeDlg.TimeEnd();
			searchCond.dataOwnerby = m_diskOwnerIndex;
		}
		else
		{
			if (0 == m_searchCdnTimeDlg.CH())
			{
				MessageBox(m_pStrTable->String(E_STR_ID_MESSAGE_34), m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK | DEFAULT_ICON_WARNING);
				return KEY_VIRTUAL_MSG_STOP;
			}

			if (m_searchCdnTimeDlg.TimeStart() >= m_searchCdnTimeDlg.TimeEnd())
			{
				MessageBox(m_pStrTable->String(E_STR_ID_MESSAGE_51), m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK | DEFAULT_ICON_WARNING);
				return KEY_VIRTUAL_MSG_STOP;
			}

			searchCond.searchType = DATA_SEARCH_TYPE_FILE;
			searchCond.videoCH = m_searchCdnTimeDlg.CH();
			searchCond.startTime = m_searchCdnTimeDlg.TimeStart();
			searchCond.endTime = m_searchCdnTimeDlg.TimeEnd();
			searchCond.dataOwnerby = m_diskOwnerIndex;
		}
	}
#endif
		
	CMessageMan::Instance()->SendMsgDataToServer(0, LOCAL_DEVICE_ID, CMD_REQUEST_DATA_SEARCH, (unsigned char *)&searchCond, sizeof(NET_DATA_SEARCH), msgData);

	FILE_INFO_EX *pDateTemp = (FILE_INFO_EX *)(msgData.pData + sizeof(PACKCMD) + sizeof(NET_DATA_INFO));
	int num = (msgData.dataLen - sizeof(PACKCMD) - sizeof(NET_DATA_INFO))/sizeof(FILE_INFO_EX);
	if (0 == num)
	{
		CMessageMan::Instance()->ReleaseMsgBuff(msgData);
		MessageBox(m_pStrTable->String(E_STR_ID_MESSAGE_36), m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK | DEFAULT_ICON_WARNING);
		return KEY_VIRTUAL_MSG_STOP;
	} 

	FILE_INFO_EX_BACKUP *pFileInfoBackup = new FILE_INFO_EX_BACKUP[num];
	for (int count = 0; count < num; ++count)
	{
		pFileInfoBackup[count].fileInfo = pDateTemp[count];
		pFileInfoBackup[count].hasBackupAudio = 1;
	}
	m_listCtrlExBackup.PutData(pFileInfoBackup, num);
	pFileInfoBackup = NULL;

	CMessageMan::Instance()->ReleaseMsgBuff(msgData);	

	m_startTime = searchCond.startTime;
	m_endTime   = searchCond.endTime;

	GUI::CButton *pBtn = dynamic_cast<GUI::CButton*>(GetCtrl(CTRLID_DEFAULT_BTN_APPLY));
	assert(pBtn != NULL);
	pBtn->SetEnable(true, true);

	m_listCtrlExBackup.SetSelAll();

	return KEY_VIRTUAL_MSG_STOP;
}

BEGIN_MSG_MAP(CBackupDlg, GUI::CDialog)
	ON_MSG(m_btnFlex.GetID(), KEY_VIRTUAL_ENTER, OnBtnFlex)
	ON_MSG(CTRLID_NULL, KEY_VIRTUAL_BACKUP, OnApply)
	ON_MSG(CTRLID_NULL, KEY_VIRTUAL_SEARCH, OnOK)
	ON_MSG(m_listCtrlExBackup.GetID(), KEY_VIRTUAL_DBCLICK, OnPlayback)
	ON_MSG(CTRLID_NULL, KEY_VIRTUAL_PLAY, OnPlayback)
END_MSG_MAP()

unsigned long CBackupDlg::OnBtnFlex()
{
#if defined(__GROUY_SEARCH__)
	if (GUI::CWnd::m_width >=  1024 && GUI::CWnd::m_height >= 768)
	{
		m_searchCdn2TimeDlgWidth = m_length[CALENDAR_WIDTH] + 10;
	}
	else
	{
		m_searchCdn2TimeDlgWidth = m_length[CALENDAR_WIDTH] + 30;
	}
#else
	if (GUI::CWnd::m_width >=  1024 && GUI::CWnd::m_height >= 768 ||  m_chnnNum <= 16)
	{
		m_searchCdn2TimeDlgWidth = m_length[CALENDAR_WIDTH] + 10;
	}
	else
	{
		m_searchCdn2TimeDlgWidth = m_length[CALENDAR_WIDTH] + 30;
	}
#endif
	unsigned short xLeft = m_length[DLG_OFFSET_LEFT], yTop = m_length[TITLE_HEIGHT] + m_length[DLG_OFFSET_TOP];
	unsigned short width = m_rect.Width() - 2 * m_length[DLG_OFFSET_LEFT];

	unsigned short x = xLeft, y = yTop;
	GUI::CRect rect = m_listCtrlExBackup.GetClientRect();

	if (m_bFlexSearchConditionDlg)
	{
		m_listCtrlExBackup.SetWndSize(false, false, width - m_searchCdn2TimeDlgWidth - m_length[FLEX_BTN_WIDTH], rect.Height() );
#if defined(__GROUY_SEARCH__)
		if (GUI::CWnd::m_width >=  1024 && GUI::CWnd::m_height >= 768)
		{
			m_searchGroupTimeDlg.Show(true, false);
		}
		else
		{
			m_searchGroupScrollTimeDlg.Show(true, false);
		}
#else
		if (GUI::CWnd::m_width >=  1024 && GUI::CWnd::m_height >= 768 ||  m_chnnNum <= 16)
		{
			m_searchCdn2TimeDlg.Show(true, false);
		}
		else
		{
			m_searchCdnTimeDlg.Show(true, false);
		}
#endif
		x = m_rect.m_left + xLeft + width - m_searchCdn2TimeDlgWidth - m_length[FLEX_BTN_WIDTH];
		y = m_rect.m_top + m_rect.Height()/2 - m_length[FLEX_BTN_HEIGHT]/2;
		m_btnFlex.MoveWnd( x, y );

		m_btnFlex.SetImage(PAINT_STATUS_NORMAL, BMP_BUTTON_DLG_FLEX_R, false);
		m_btnFlex.SetImage(PAINT_STATUS_FOCUS, BMP_BUTTON_DLG_FLEX_R_F, false);
	}
	else
	{
		m_listCtrlExBackup.SetWndSize(true, false);
#if defined(__GROUY_SEARCH__)
		if (GUI::CWnd::m_width >=  1024 && GUI::CWnd::m_height >= 768)
		{
			m_searchGroupTimeDlg.Show(false, false);
		}
		else
		{
			m_searchGroupScrollTimeDlg.Show(false, false);
		}

#else
		if (GUI::CWnd::m_width >=  1024 && GUI::CWnd::m_height >= 768 ||  m_chnnNum <= 16)
		{
			m_searchCdn2TimeDlg.Show(false, false);
		}
		else
		{
			m_searchCdnTimeDlg.Show(false, false);
		}
#endif
		x = m_rect.m_left + xLeft + width;
		y = m_rect.m_top + m_rect.Height()/2 - m_length[FLEX_BTN_HEIGHT]/2;
		m_btnFlex.MoveWnd( x, y );

		m_btnFlex.SetImage(PAINT_STATUS_NORMAL, BMP_BUTTON_DLG_FLEX_L, false);
		m_btnFlex.SetImage(PAINT_STATUS_FOCUS, BMP_BUTTON_DLG_FLEX_L_F, false);
	}

	m_bFlexSearchConditionDlg = !m_bFlexSearchConditionDlg;

	Repaint();
	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CBackupDlg::OnPlayback()
{
	unsigned short row = 0;	
	row = m_listCtrlExBackup.GetSingleSelect();
	if ((row >= m_listCtrlExBackup.GetCount()) || (row < 0))
	{
		return KEY_VIRTUAL_MSG_STOP;
	}

	FILE_INFO_EX_BACKUP * pFileInfoBackup = NULL;
	pFileInfoBackup = m_listCtrlExBackup.GetListData();
	if (NULL == pFileInfoBackup)
	{
		return KEY_VIRTUAL_MSG_STOP;
	}

	if (pFileInfoBackup[row].fileInfo.chnn >= m_chnnNum)
	{
		return KEY_VIRTUAL_MSG_STOP;
	}

	if (pFileInfoBackup[row].fileInfo.startTime >= pFileInfoBackup[row].fileInfo.endTime)
	{
		return KEY_VIRTUAL_MSG_STOP;
	}

	unsigned char maxVideoOutNum = 0;

	//判断是不是有权限
	ULONGLONG auth = CUserMan::Instance()->CheckAuthority(AUTH_PLAYBACK);
	if(auth ==0)
	{
		MessageBox(m_pStrTable->String(E_STR_ID_MESSAGE_77), m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK | DEFAULT_ICON_WARNING);
		return KEY_VIRTUAL_MSG_STOP;
	}
	else
	{
		unsigned long perauth =((auth>>pFileInfoBackup[row].fileInfo.chnn)&(0x01));//判断每位的权限
		printf("au=%d,pFileInfo[row].chnn=%d\n",perauth,pFileInfoBackup[row].fileInfo.chnn);
		if(perauth!=1)
		{
			MessageBox(m_pStrTable->String(E_STR_ID_MESSAGE_77), m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK | DEFAULT_ICON_WARNING);
			return KEY_VIRTUAL_MSG_STOP;
		}
	}
	
	
	CPlaybackDlg playbackDlg;

	if (RECORD_FILE_IS_UNOFFICAL(pFileInfoBackup[row].fileInfo.type))
	{
		playbackDlg.SetPlayInfo(pFileInfoBackup[row].fileInfo, m_diskOwnerIndex);
	}
	else
	{
		playbackDlg.SetPlayInfo(pFileInfoBackup[row].fileInfo.startTime, pFileInfoBackup[row].fileInfo.endTime, pFileInfoBackup[row].fileInfo.startTime, \
			((ULONGLONG)(0x01) << pFileInfoBackup[row].fileInfo.chnn), m_chnnNum, g_p_login_succ->productInfo.videoOuputNum, true, m_diskOwnerIndex);
	}

	playbackDlg.DoModal(this, CENTER_POS, CENTER_POS, MAX_SCREEN_X, MAX_SCREEN_Y, false);

	return KEY_VIRTUAL_MSG_STOP;
}
#if defined(__GROUY_SEARCH__)
void CBackupDlg::EndDialog(DLG_END_VALUE result)
{
	if (GUI::CWnd::m_width >=  1024 && GUI::CWnd::m_height >= 768)
	{
		if(m_searchGroupTimeDlg.IsModify())
		{
			m_searchGroupTimeDlg.SaveGroupItemCfg();
		}
	}
	else 
	{
		if(m_searchGroupScrollTimeDlg.IsModify())
		{
			m_searchGroupScrollTimeDlg.SaveGroupItemCfg();
		}
	}
	
	
	return GUI::CDialog::EndDialog(result);
}
#endif
//end









