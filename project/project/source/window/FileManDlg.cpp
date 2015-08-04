/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : ysw
** Date         : 2009-08-01
** Name         : FileManDlg.cpp
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/
#include "FileManDlg.h"
#include "ListCtrlEx.cpp"
#include "TimeCtrl.h"
#include "UserMan.h"
#include "StreamRecordManEx.h"
//////////////////////////////////////////////////////////////////////////
template <class T> std::string &  GUI::CListCtrlEx<T>::Text(T &node, unsigned short col)
{
	char str [512] = {0};

	FILE_INFO_EX &it = node;

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
		std::string strtime;
		CTimeCtrl::GetTime32Text(strtime,it.startTime);
		strcpy(str,strtime.c_str());
	}

	if (2 == col)
	{
		std::string strtime;
		CTimeCtrl::GetTime32Text(strtime,it.endTime);
		strcpy(str,strtime.c_str());
	}

	if (3 == col)
	{
		if (RECORD_FILE_IS_LOCK(it.type))
		{
			strcpy(str, m_pStrTable->String(E_STR_ID_LOCKED));
		}
		else if (RECORD_FILE_IS_ONLYREAD(it.type))
		{
			strcpy(str, m_pStrTable->String(E_STR_ID_READ_ONLY));
		}
		else if (RECORD_FILE_IS_WRITING(it.type))
		{
			//更准确的说是：正在录像或等待写
			strcpy(str, m_pStrTable->String(E_STR_ID_WRITING));
		}
		else if (RECORD_FILE_IS_READING(it.type))
		{
			strcpy(str, m_pStrTable->String(E_STR_ID_READING));
		}
	}

	m_str = str;
	
	return m_str;
}
//////////////////////////////////////////////////////////////////////////
CFileManDlg::CFileManDlg()
{
	m_channelNum = 0;
}

CFileManDlg::~CFileManDlg()
{

}


void CFileManDlg::SetInitialInfo(unsigned long channelNum, unsigned long diskOwnerIndex)
{
	m_channelNum = channelNum;
	m_diskOwnerIndex = diskOwnerIndex;
}


//////////////////////////////////////////////////////////////////////////
void CFileManDlg::OnInitial()
{
	assert(m_channelNum > 0);
	GUI::CDialog::OnInitial();

	bool bBigScreen = ((GUI::CWnd::m_width >= 1024) && (GUI::CWnd::m_height >= 768)) ? true : false;

	//////////////////////////////////////////////////////////////////////////
	unsigned short x = m_length[DLG_OFFSET_SUB_LEFT], y = m_length[DLG_OFFSET_SUB_TOP];
	unsigned short width = m_rect.Width() - 2 * m_length[DLG_OFFSET_SUB_LEFT];
	unsigned short height = m_rect.Height() - y - m_length[DLG_OFFSET_SUB_BOTTOM] - m_length[BTN_HEIGHT];
	m_listCtrlEx.Create(GUI::CWnd::GetRandWndID(), x, y, width, height, this, 0, 0, true);
	m_listCtrlEx.AddColumn(m_pStrTable->String(E_STR_ID_CH), width * 3 / 32);
	m_listCtrlEx.AddColumn(m_pStrTable->String(E_STR_ID_START_TIME), width * 11 / 32);
	m_listCtrlEx.AddColumn(m_pStrTable->String(E_STR_ID_END_TIME), width * 11 / 32);
	m_listCtrlEx.AddColumn(m_pStrTable->String(E_STR_ID_STATUS), width * 7 / 32 - m_length[CHECK_WIDTH] - 4);
	m_listCtrlEx.EnableSingleSelect();

	GUI::CRect rect = m_listCtrlEx.GetClientRect();
	y += rect.Height() + 4;
#if defined(__CUSTOM_BR27__) || (defined(__CUSTOM_TW01__) && !defined(__TW01_RILI__)) || defined(__CUSTOM_TW01TS__)
	m_btnLock.Create(GUI::CWnd::GetRandWndID(), x, y, m_length[BTN_WIDTH], m_length[BTN_HEIGHT], this, 0, 1, m_pStrTable->String(E_STR_ID_LOCK));
	ULONGLONG bAuth = CUserMan::Instance()->CheckAuthority(AUTH_DATA_MAN);
	if ((NET_DISK_OWNED_BY_THIS != m_diskOwnerIndex) || (0 == bAuth))
	{
		m_btnLock.SetEnable(false,false);
	}
#else

	if (!CStreamRecordManEx::Instance()->IsSupportMinorStreamRecord())
	{
		m_btnDelete.Create(GUI::CWnd::GetRandWndID(), x, y, m_length[BTN_WIDTH], m_length[BTN_HEIGHT], this, 0, 1, m_pStrTable->String(E_STR_ID_DELETE));
		x += m_length[BTN_WIDTH] + width * 4 / 32;
		m_btnLock.Create(GUI::CWnd::GetRandWndID(), x, y, m_length[BTN_WIDTH], m_length[BTN_HEIGHT], this, 1, 1, m_pStrTable->String(E_STR_ID_LOCK));
		ULONGLONG bAuth = CUserMan::Instance()->CheckAuthority(AUTH_DATA_MAN);
		if ((NET_DISK_OWNED_BY_THIS != m_diskOwnerIndex) || (0 == bAuth))
		{
			m_btnLock.SetEnable(false,false);
			m_btnDelete.SetEnable(false,false);
		}
	}

#endif
}

void CFileManDlg::OnSearch(unsigned long startTime, unsigned long endTime, ULONGLONG chnnBits, unsigned long dataOwnerby)
{
	//添加搜索处理

	MESSAGE_DATA msgData;
	NET_DATA_SEARCH searchCond;

	searchCond.searchType = DATA_SEARCH_TYPE_FILE;
	searchCond.videoCH = chnnBits;
	searchCond.startTime = startTime;
	searchCond.endTime = endTime;
	searchCond.dataOwnerby = dataOwnerby;
	m_curSearchCond = searchCond;
	CMessageMan::Instance()->SendMsgDataToServer(0, LOCAL_DEVICE_ID, CMD_REQUEST_DATA_SEARCH, (unsigned char *)&searchCond, sizeof(NET_DATA_SEARCH), msgData);
 
	FILE_INFO_EX *pDate = (FILE_INFO_EX *)(msgData.pData + sizeof(PACKCMD) + sizeof(NET_DATA_INFO));
	int num = (msgData.dataLen - sizeof(PACKCMD) - sizeof(NET_DATA_INFO))/sizeof(FILE_INFO_EX);
	if (0 == num)
	{
		MessageBox(m_pStrTable->String(E_STR_ID_MESSAGE_36), m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK | DEFAULT_ICON_WARNING);
		CMessageMan::Instance()->ReleaseMsgBuff(msgData);	
		return;
	} 

	FILE_INFO_EX *pFileInfo = new FILE_INFO_EX[num];
	for (int count = 0; count < num; ++count)
	{
		pFileInfo[count] = pDate[count];		
	}

	
	m_listCtrlEx.PutData(pFileInfo, num);
	pFileInfo = NULL;

	CMessageMan::Instance()->ReleaseMsgBuff(msgData);	

}

unsigned long CFileManDlg::GetPlaybackInfo(FILE_INFO_EX & fileInfo)
{
	unsigned short row = 0;	
	row = m_listCtrlEx.GetSingleSelect();
	if ((row >= m_listCtrlEx.GetCount()) || (row < 0))
	{
		return KEY_VIRTUAL_MSG_STOP;
	}

	FILE_INFO_EX * pFileInfo = NULL;
	pFileInfo = m_listCtrlEx.GetListData();
	if (NULL == pFileInfo)
	{
		return KEY_VIRTUAL_MSG_STOP;
	}

	if (pFileInfo[row].chnn >= m_channelNum)
	{
		return KEY_VIRTUAL_MSG_STOP;
	}

	if (pFileInfo[row].startTime >= pFileInfo[row].endTime)
	{
		return KEY_VIRTUAL_MSG_STOP;
	}

	m_selFileInfo = pFileInfo[row];
	fileInfo = m_selFileInfo;

	return KEY_VIRTUAL_MSG_CONTINUE;
}

BEGIN_MSG_MAP(CFileManDlg, GUI::CDialog)
	ON_MSG(m_btnLock.GetID(), KEY_VIRTUAL_ENTER, OnClickLock)
	ON_MSG(m_btnDelete.GetID(), KEY_VIRTUAL_ENTER, OnClickDel)
	ON_MSG_EX(m_listCtrlEx.GetID(), KEY_VIRTUAL_DBCLICK, OnClickList)
	ON_MSG_EX(CTRLID_NULL, KEY_VIRTUAL_PLAY, OnClickList)
END_MSG_MAP()

unsigned long CFileManDlg::OnClickLock()
{
	//如果列表中没有文件，则不做任何处理直接返回
	if (0 == m_listCtrlEx.GetCount())
	{
		return KEY_VIRTUAL_MSG_STOP;
	}

	//锁定操作
	{
		//获取要进行锁定的数据
		FILE_INFO_EX *pLockData = NULL;
		unsigned long lockFileNum = 0;
		pLockData = m_listCtrlEx.GetSelectData(lockFileNum); //函数GetSelectData里面分配的内存，有外面释放。
		if (0 == lockFileNum)
		{
			MessageBox(m_pStrTable->String(E_STR_ID_MESSAGE_50), m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK | DEFAULT_ICON_WARNING);
			return KEY_VIRTUAL_MSG_STOP;
		}
		if (NULL == pLockData)
		{
			return KEY_VIRTUAL_MSG_STOP;
		}

		int index = 0;
		for (index = 0; index < lockFileNum; ++index)
		{
			if (RECORD_FILE_IS_ONLYREAD(pLockData[index].type))
			{
				continue;
			}
			if (RECORD_FILE_IS_WRITING(pLockData[index].type))
			{
				continue;
			}
			break;
		}
		if (index == lockFileNum)
		{
			//此处表示，所选择的文件都不能被锁定
			MessageBox(m_pStrTable->String(E_STR_ID_MESSAGE_105), m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK | DEFAULT_ICON_WARNING);
			if (NULL != pLockData)
			{
				delete [] pLockData;
				pLockData = NULL;
			}
			return KEY_VIRTUAL_MSG_STOP;
		}

		
		MESSAGE_DATA msgData;
		long cmd;
		char str[512] = {0};
		if (RECORD_FILE_IS_LOCK(pLockData[index].type))
		{
			cmd = CMD_REQUEST_DATA_UNLOCK;
			snprintf(str, 512, "%s", m_pStrTable->String(E_STR_ID_MESSAGE_106));
		}
		else
		{
			cmd = CMD_REQUEST_DATA_LOCK;
#if defined(__TW01_RILI__)
			snprintf(str, 512, "%s", m_pStrTable->String(E_STR_ID_MESSAGE_12));
#else
			snprintf(str, 512, "%s %s", m_pStrTable->String(E_STR_ID_MESSAGE_105), m_pStrTable->String(E_STR_ID_MESSAGE_12));
#endif
		}

		//锁定前提示
		if (DLG_OK != MessageBox(str, m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK | DEFAULT_BTN_CANCEL | DEFAULT_ICON_WARNING))
		{
			//放弃则直接返回
			if (NULL != pLockData)
			{
				delete [] pLockData;
				pLockData = NULL;
			}
			return KEY_VIRTUAL_MSG_STOP;
		}

		//发送消息
		CMessageMan::Instance()->SendMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, cmd, (unsigned char*)(&pLockData[index]), sizeof(FILE_INFO_EX) * (lockFileNum - index), msgData);
		if (NULL != pLockData)
		{
			delete [] pLockData;
			pLockData = NULL;
		}

		//接受处理后的结果
		CMessageMan::Instance()->ReleaseMsgBuff(msgData);
	}
	
	//刷新
	{ 
		m_listCtrlEx.CleanSelected();
		m_listCtrlEx.PutData(NULL, 0);
		MESSAGE_DATA msgData;
		CMessageMan::Instance()->SendMsgDataToServer(0, LOCAL_DEVICE_ID, CMD_REQUEST_DATA_SEARCH, (unsigned char *)(&m_curSearchCond), sizeof(NET_DATA_SEARCH), msgData);
		FILE_INFO_EX *pDate = (FILE_INFO_EX *)(msgData.pData + sizeof(PACKCMD) + sizeof(NET_DATA_INFO));
		int num = (msgData.dataLen - sizeof(PACKCMD) - sizeof(NET_DATA_INFO))/sizeof(FILE_INFO_EX);
		FILE_INFO_EX *pFileInfo = new FILE_INFO_EX[num];
		for (int count = 0; count < num; ++count)
		{
			pFileInfo[count] = pDate[count];		
		}
		m_listCtrlEx.PutData(pFileInfo, num);
		pFileInfo = NULL;
		CMessageMan::Instance()->ReleaseMsgBuff(msgData);
	}
	
	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CFileManDlg::OnClickDel()
{
	//如果列表中没有文件，则不做任何处理直接返回
	if (0 == m_listCtrlEx.GetCount())
	{
		return KEY_VIRTUAL_MSG_STOP;
	}

	//删除操作
	{
		//获取要进行删除的数据
		FILE_INFO_EX *pDelData = NULL;
		unsigned long delFileNum = 0;
		pDelData = m_listCtrlEx.GetSelectData(delFileNum);
		if (0 == delFileNum)
		{
			MessageBox(m_pStrTable->String(E_STR_ID_MESSAGE_50), m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK | DEFAULT_ICON_WARNING);
			return KEY_VIRTUAL_MSG_STOP;
		}

		//删除前提示
		if (DLG_OK != MessageBox(m_pStrTable->String(E_STR_ID_MESSAGE_18), m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK | DEFAULT_BTN_CANCEL | DEFAULT_ICON_WARNING))
		{
			//放弃则直接返回
			return KEY_VIRTUAL_MSG_STOP;
		}

		int index = 0;
		for (index = 0; index < delFileNum; ++index)
		{
			if (RECORD_FILE_IS_ONLYREAD(pDelData[index].type))
			{
				continue;
			}
			if (RECORD_FILE_IS_WRITING(pDelData[index].type))
			{
				continue;
			}
			if (RECORD_FILE_IS_LOCK(pDelData[index].type))
			{
				continue;
			}
			if (RECORD_FILE_IS_READING(pDelData[index].type))
			{
				continue;
			}
			break;
		}
		if (index == delFileNum)
		{
			if (NULL != pDelData)
			{
				delete [] pDelData;
				pDelData = NULL;
			}
			return KEY_VIRTUAL_MSG_STOP;
		}
	
		//发送消息
		MESSAGE_DATA msgData;
		CMessageMan::Instance()->SendMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_DATA_DELETE, (unsigned char*)(&pDelData[index]), sizeof(FILE_INFO_EX) * (delFileNum - index), msgData);
		if (NULL != pDelData)
		{
			delete [] pDelData;
			pDelData = NULL;
		}

		//接受处理后的结果
		CMessageMan::Instance()->ReleaseMsgBuff(msgData);
	}

	//刷新
	{
		m_listCtrlEx.CleanSelected();
		m_listCtrlEx.PutData(NULL, 0);
		MESSAGE_DATA msgData;
		CMessageMan::Instance()->SendMsgDataToServer(0, LOCAL_DEVICE_ID, CMD_REQUEST_DATA_SEARCH, (unsigned char *)(&m_curSearchCond), sizeof(NET_DATA_SEARCH), msgData);
		FILE_INFO_EX *pDate = (FILE_INFO_EX *)(msgData.pData + sizeof(PACKCMD) + sizeof(NET_DATA_INFO));
		int num = (msgData.dataLen - sizeof(PACKCMD) - sizeof(NET_DATA_INFO))/sizeof(FILE_INFO_EX);
		FILE_INFO_EX *pFileInfo = new FILE_INFO_EX[num];
		for (int count = 0; count < num; ++count)
		{
			pFileInfo[count] = pDate[count];		
		}
		m_listCtrlEx.PutData(pFileInfo, num);
		pFileInfo = NULL;
		CMessageMan::Instance()->ReleaseMsgBuff(msgData);		
	}

	return KEY_VIRTUAL_MSG_STOP;
}


void CFileManDlg::EndDialog(DLG_END_VALUE result)
{
	m_listCtrlEx.PutData(NULL, 0);
	CDialog::EndDialog(result);
}

unsigned long CFileManDlg::OnClickList(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{     
	return KEY_VIRTUAL_PLAY;
}






