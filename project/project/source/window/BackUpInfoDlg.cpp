/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : L
** Date         : 2009-08-25
** Name         : BackUpInfoDlg.h 
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/

#include "BackUpInfoDlg.h"
#include "TimeCtrl.h"
#ifndef __ENVIRONMENT_WIN32__
#include <sys/vfs.h> 
#endif

/*
PUB_THREAD_RESULT PUB_THREAD_CALL CBackUpInfoDlg::BackUpThread( void* pParam )
{
	CBackUpInfoDlg *pThis = (CBackUpInfoDlg *)pParam;
	pThis->BackUpProc();

	return 0;
}
*/

CBackUpInfoDlg::CBackUpInfoDlg()
{
	m_prgShow       = false;
	m_pDiskInfoList = NULL;
	m_startTime     = 0;
	m_endTime       = 0;
	m_pFileInfoBackup   = NULL;
	m_fileNum       = 0;
	m_pDiskName     = NULL;
	m_bBackupIng    = false;
	m_precent       = 0;
	m_displayPrecent = 0;
	m_pMountDir     = NULL;
	m_diskOwnerby =0;
}

CBackUpInfoDlg::~CBackUpInfoDlg()
{

}

void CBackUpInfoDlg::SetInitialInfo(DVR_DISK_INFO_LIST *pDiskInfoList, FILE_INFO_EX_BACKUP *pDataBackup, unsigned long fileNum, unsigned long startTime, unsigned long endTime,unsigned long  diskOwnerby)
{
	//assert(pData != NULL);
	assert(pDiskInfoList->GetCount() > 0);
	m_pDiskInfoList = pDiskInfoList;
	m_startTime     = startTime;
	m_endTime       = endTime;
	m_pFileInfoBackup = pDataBackup;
	m_fileNum       = fileNum;
	m_diskOwnerby   = diskOwnerby;
}

void CBackUpInfoDlg::OnInitial()
{
	m_style |= WND_NO_TITLE;  //不需要TITLE，因为光驱备份不能够停止
	//
	GUI::CDialog::OnInitial();
	SetImage(BMP_MEM_DLG_LARGE, false);
	SetCaption( m_pStrTable->String(E_STR_ID_TITLE_BACKUP_INFO), false);
	SetDefaultBtn(DEFAULT_BTN_OK | DEFAULT_BTN_CANCEL);
	SetDefaultBtnCaption(DEFAULT_BTN_OK, m_pStrTable->String(E_STR_ID_START));
	SetDefaultBtnCaption(DEFAULT_BTN_CANCEL, m_pStrTable->String(E_STR_ID_CANCEL) );
	

	//
	GUI::CListCtrl& list = m_backUpInfoList;
	GUI::CWnd * pWnd = NULL;
	GUI::CCheckCtrl* pCheck = NULL;
	GUI::CComboCtrl* pCombo = NULL;
	GUI::CStatic* pStatic = NULL;

	unsigned short x = m_length[DLG_OFFSET_LEFT], y = m_length[TITLE_HEIGHT] + m_length[DLG_OFFSET_TOP];
	unsigned short ctrlHeight = m_length[BTN_HEIGHT];
	unsigned short ctrlWidth = m_length[BTN_WIDTH];;
	unsigned short width = m_rect.Width() - 2 * m_length[DLG_OFFSET_LEFT];
	unsigned short height = m_rect.Height() - y - m_length[LIST_ITEM_HEIGHT] - m_length[DEFAULT_BTN_BOTTOM_YPOS] - m_length[DLG_OFFSET_BOTTOM];

	//List
	list.Create(GUI::CWnd::GetRandWndID(), x, y, width, height, this, 0, 1, false);
	list.AddColumn(NULL, width/2);
	list.AddColumn(NULL, width/2 - 4);
	GUI::CRect rect = list.GetClientRect();
	unsigned short yOff = ( m_rect.Height() - y - m_length[LIST_EX_ITEM_HEIGHT] - m_length[DEFAULT_BTN_BOTTOM_YPOS] - m_length[DLG_OFFSET_BOTTOM] - rect.Height() ) / 2;
	y += yOff;
	list.MoveWnd( m_rect.m_left + x, m_rect.m_top + y );
	{
		list.AddItem(ROW_START_TIME, 0, m_pStrTable->String(E_STR_ID_START_TIME));
		pStatic = dynamic_cast<GUI::CStatic*>(list.AddItem(ROW_START_TIME, 1, WND_TYPE_STATIC));

		list.AddItem(ROW_END_TIME, 0, m_pStrTable->String(E_STR_ID_END_TIME));
		pStatic = dynamic_cast<GUI::CStatic*>(list.AddItem(ROW_END_TIME, 1, WND_TYPE_STATIC));
		
		list.AddItem(ROW_FILE_NUM, 0, m_pStrTable->String(E_STR_ID_FILES_NUM));
		pStatic = dynamic_cast<GUI::CStatic*>(list.AddItem(ROW_FILE_NUM, 1, WND_TYPE_STATIC));
		
		list.AddItem(ROW_FILE_SIZE, 0, m_pStrTable->String(E_STR_ID_SIZE_GB));
		pStatic = dynamic_cast<GUI::CStatic*>(list.AddItem(ROW_FILE_SIZE, 1, WND_TYPE_STATIC));

		list.AddItem(ROW_STORAGE_TYPE, 0, m_pStrTable->String(E_STR_ID_STORAGE_TYPE));
		pCombo = dynamic_cast<GUI::CComboCtrl*>(list.AddItem(ROW_STORAGE_TYPE, 1,  WND_TYPE_COMBO) );
		
		list.AddItem(ROW_FREE_SIZE, 0, m_pStrTable->String(E_STR_ID_FREE_GB) );		
		pStatic = dynamic_cast<GUI::CStatic*>( list.AddItem(ROW_FREE_SIZE, 1, WND_TYPE_STATIC ) );
		assert( pStatic );
		
		list.AddItem(ROW_PLAYER, 0, m_pStrTable->String(E_STR_ID_BACKUP_PLAYER));
		pCheck = dynamic_cast<GUI::CCheckCtrl*>(list.AddItem(ROW_PLAYER, 1, WND_TYPE_CHECK));
		
		list.AddItem(ROW_BACKUP_TYPE, 0, m_pStrTable->String(E_STR_ID_SAVE_FILE_TYPE));
		pCombo = dynamic_cast<GUI::CComboCtrl*>( list.AddItem(ROW_BACKUP_TYPE, 1, WND_TYPE_COMBO));
		assert( pCombo );		
	}
	list.CreateListFocusMap();

	//
	GUI::CRect listRect =  list.GetClientRect();
	{
		y += listRect.Height() + 4;
		m_progressDlg.Create( GUI::CWnd::GetRandWndID(), x, y, width, m_length[LIST_ITEM_HEIGHT], this, 0, 2 );
	}

	TextSize(m_pStrTable->String(E_STR_ID_DISK_CLEAN_UP), strlen(m_pStrTable->String(E_STR_ID_DISK_CLEAN_UP)), width, height);
	x = m_rect.Width() - m_length[DEFAULT_BTN_RIGHT_XPOS] - 2 * m_length[BTN_WIDTH] - 2 * m_length[DEFAULT_BTN_OFFSET] - width - 20;
	y = m_rect.Height() - m_length[DEFAULT_BTN_BOTTOM_YPOS];
	m_diskManBtn.Create(GUI::CWnd::GetRandWndID(), x, y, width + 20, m_length[BTN_HEIGHT], this, 1, 0, m_pStrTable->String(E_STR_ID_DISK_CLEAN_UP)) ;

}
void CBackUpInfoDlg::OnPostInitial()
{
	CDialog::OnPostInitial();

	GUI::CListCtrl& list = m_backUpInfoList;
	GUI::CCheckCtrl* pCheck = NULL;
	GUI::CComboCtrl* pCombo = NULL;
	GUI::CStatic* pStatic = NULL;
	GUI::CTimeCtrl *pTimeCtrl = NULL;
	float spaceG = 0.0;
	POS pos = NULL;
	char str[128] = {0};
	std::string strTime;
	
	//开始和结束时间
	CTimeCtrl::GetTime32Text(strTime, m_startTime);
	pStatic = dynamic_cast<GUI::CStatic*>(list.GetItemWnd(ROW_START_TIME, 1));
	pStatic->SetCaption(strTime);
	CTimeCtrl::GetTime32Text(strTime, m_endTime);
	pStatic = dynamic_cast<GUI::CStatic*>(list.GetItemWnd(ROW_END_TIME, 1));
	pStatic->SetCaption(strTime);

	//文件数
	pStatic = dynamic_cast<GUI::CStatic*>(list.GetItemWnd(ROW_FILE_NUM, 1));
	sprintf(str, "%d", m_fileNum);
	pStatic->SetCaption(str);

	//添加设备
	int usbCount = 0;
	int esataCount = 0;
	int usbDiskCount = 0;
	int otherCount = 0;
	pCombo = dynamic_cast<GUI::CComboCtrl*>(list.GetItemWnd(ROW_STORAGE_TYPE, 1));
	pCombo->SetMaxStringLen(30);
	assert(pCombo);
	pos = m_pDiskInfoList->GetHeadPosition();
	while (NULL != pos)
	{
		DVR_DISK_INFO &diskInfo = m_pDiskInfoList->GetAt(pos);
		if (NET_DISK_TYPE_REMOVEABLE == diskInfo.diskType)
		{
			if (NET_DISK_USB_INTERFACE == diskInfo.diskInterface)
			{
				usbCount++;
				sprintf(str, "USB-%d", usbCount);
			}
			else if (NET_DISK_REMDISK_INTERFACE == diskInfo.diskInterface)
			{
				usbDiskCount++;
				sprintf(str, "USB-DISK-%d", usbDiskCount);
			}
			else if (NET_DISK_ESATA_INETERFACE == diskInfo.diskInterface)
			{
				esataCount++;
				sprintf(str, "e-SATA-%d", esataCount);
			}
			else
			{
				otherCount++;
				sprintf(str, "USB-%d", otherCount);
			}			
		}			
		else if (NET_DISK_TYPE_CDDVD == diskInfo.diskType)
		{
			strcpy(str, "CD/DVR-ROM");
		}
		else
		{
			assert(false);
		}
		pCombo->AddItem(str, (LONGLONG)pos);
		m_pDiskInfoList->GetNext(pos);
	}
	pCombo->SetCurItem(0, false);
	m_curDevicePos = (POS)(pCombo->GetCurItemData());
	assert(m_curDevicePos != NULL);
	DVR_DISK_INFO diskInfo = m_pDiskInfoList->GetAt(m_curDevicePos);
	if(NET_DISK_TYPE_CDDVD == diskInfo.diskType)
	{
		m_diskManBtn.SetEnable(false,false);
	}

	//计算剩余空间
	pStatic = dynamic_cast<GUI::CStatic*>(list.GetItemWnd(ROW_FREE_SIZE, 1));
	assert( pStatic );	
	spaceG = diskInfo.freeSpace / (float)1024;
	sprintf(str, "%0.3f", spaceG);			
	pStatic->SetCaption(str, false);

	//是否备份播放器
	pCheck = dynamic_cast<GUI::CCheckCtrl*>(list.GetItemWnd(ROW_PLAYER, 1));
	assert( pCheck );
	pCheck->SetCheck(true, false);
	m_bHavePlayer = true;
	
	//添加备份出来的数据类型
	pCombo = dynamic_cast<GUI::CComboCtrl*>(list.GetItemWnd(ROW_BACKUP_TYPE, 1));
	assert(pCombo);
	pCombo->AddItem(m_pStrTable->String(E_STR_ID_DVR), NET_BACKUP_DATA_TYPE_DVR);
	pCombo->AddItem(m_pStrTable->String(E_STR_ID_AVI), NET_BACKUP_DATA_TYPE_AVI);
	if (NET_DISK_TYPE_CDDVD == diskInfo.diskType)
	{
		pCombo->SetCurItem(NET_BACKUP_DATA_TYPE_DVR);
		pCombo->SetEnable(false);
	}
	else
	{
		pCombo->SetCurItem(0, false);	
		pCombo->SetEnable(true, true);		
	}
	m_dataSaveType = (BACKUP_DATA_TYPE)(pCombo->GetCurItemData());

	//计算所需的空间
	m_needSpace = CalNeedSpaceSize(m_fileNum, (DISK_TYPE)(diskInfo.diskType), m_dataSaveType, m_bHavePlayer);
	pStatic = dynamic_cast<GUI::CStatic*>(list.GetItemWnd(ROW_FILE_SIZE, 1));
	spaceG = m_needSpace / (float)1024;
	sprintf(str, "%0.3f", spaceG);			
	pStatic->SetCaption(str, false);

	//初始化一些变量
	if (NULL != m_pDiskName)
	{
		delete [] m_pDiskName;
		m_pDiskName = NULL;
	}
	m_pDiskName = new char [255];
	if (NULL != m_pMountDir)
	{
		delete [] m_pMountDir;
		m_pMountDir = NULL;
	}
	m_pMountDir = new char [255];	
	m_bBackupIng = false;
	m_precent    = 0;
	m_displayPrecent = 0;
	SetPercent(0);
}

void CBackUpInfoDlg::OnDestroy()
{
	GUI::CDialog::OnDestroy();
	if (NULL != m_pDiskName)
	{
		delete [] m_pDiskName;
		m_pDiskName = NULL;
	}

	if (NULL != m_pMountDir)
	{
		delete [] m_pMountDir;
		m_pMountDir = NULL;
	}
}

unsigned long CBackUpInfoDlg::OnClickDiskMan()
{
//	GUI::CDialog::OnApply();
#ifdef __ENVIRONMENT_LINUX__
	if ((NULL == m_curDevicePos) || (NULL == m_pDiskInfoList))
	{
		return KEY_VIRTUAL_MSG_STOP;
	}

	CUSBExploreDlg usbexploredlg;
	DVR_DISK_INFO &diskInfo = m_pDiskInfoList->GetAt(m_curDevicePos);
	usbexploredlg.SetCurDevice(CUSBExploreDlg::PARTITION, diskInfo.diskname);
	usbexploredlg.SetStyle(BACKUP_INFO_DLG);
	usbexploredlg.SetCaptionName(m_pStrTable->String(E_STR_ID_TITLE_INFORMATION));
	usbexploredlg.DoModal(this);

	//
	{
		bool bChange = false;
		char szCmd[48] = {0};
		snprintf(szCmd, 48, "mkdir -p /mnt/testsize");
		DVRSystem(szCmd);

		snprintf(szCmd, 48, "umount /mnt/testsize");
		DVRSystem(szCmd);

		snprintf(szCmd, 48, "mount -t vfat %s /mnt/testsize/", diskInfo.diskname);
		if (0 == DVRSystem(szCmd))
		{
			struct statfs stattmp;
			memset(&stattmp, 0, sizeof(stattmp));
			if (statfs("/mnt/testsize", &stattmp) != -1)
			{
				unsigned long lTotal = (stattmp.f_blocks/1024)*(stattmp.f_bsize/1024);					
				unsigned long lAvail = (stattmp.f_bavail/1024)*(stattmp.f_bsize/1024);	
				diskInfo.freeSpace = lAvail;
				bChange = true;
			}

			snprintf(szCmd, 48, "umount /mnt/testsize");
			if (0 == DVRSystem(szCmd))
			{
				snprintf(szCmd, 48, "rm -rf /mnt/testsize");
				DVRSystem(szCmd);
			}
		}

		if (bChange)
		{
			GUI::CStatic *pStatic = dynamic_cast<GUI::CStatic*>(m_backUpInfoList.GetItemWnd(ROW_FREE_SIZE, 1));
			assert(pStatic);				
			float spaceG = 0.0;
			spaceG = diskInfo.freeSpace / (float)1024;
			snprintf(szCmd, 48, "%0.3f", spaceG);			
			pStatic->SetCaption(szCmd);
		}
	}
#endif
	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CBackUpInfoDlg::OnOK()
{
	LOCAL_CLIENT_BACKUP_INFO backupInfo;
	MESSAGE_DATA msgData;
	PACKCMD *pCmd = NULL;
	unsigned long cmdType;
	bool bHaveVideoPlay = false;
	unsigned long freeSpace = 0;
	POS pos = NULL;

	if (m_bBackupIng)
	{
		return KEY_VIRTUAL_MSG_STOP;
	}


	//判断剩余空间是否够
	DVR_DISK_INFO &diskInfo = m_pDiskInfoList->GetAt(m_curDevicePos);
	freeSpace = diskInfo.freeSpace;
	if (freeSpace < m_needSpace)
	{
#ifndef __ENVIRONMENT_WIN32__
		MessageBox(m_pStrTable->String(E_STR_ID_MESSAGE_61), m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK|DEFAULT_ICON_WARNING);
		
		//退回到上级界面
		KEY_VALUE_INFO keyValue;
		keyValue.type = KEY_TYPE_VIRTUAL;
		keyValue.value = KEY_VIRTUAL_ESC;
		AddKeyValueEx(keyValue);
		return KEY_VIRTUAL_MSG_STOP;
#endif
	}

	//判断备份的条件是否合理
	assert((NET_BACKUP_DATA_TYPE_AVI == m_dataSaveType) || (NET_BACKUP_DATA_TYPE_DVR == m_dataSaveType));

	//开始备份
	backupInfo.pFileInfoBackup    = (unsigned long)m_pFileInfoBackup;
	backupInfo.fileNum      = m_fileNum;
	backupInfo.startTime    = m_startTime;
	backupInfo.endTime      = m_endTime;
	backupInfo.diskOwnerby  = m_diskOwnerby;
	sprintf(m_pDiskName, "%s", diskInfo.diskname);
	backupInfo.pDiskName    = (unsigned long)m_pDiskName;
	if (NET_DISK_TYPE_REMOVEABLE == diskInfo.diskType)
	{
		if (NET_DISK_IS_MOUNT == diskInfo.diskStatus)
		{
#ifndef __ENVIRONMENT_WIN32__		
			if (NET_DISK_TYPE_REMOVEABLE == diskInfo.diskType)
			{
				assert(false);
			}		
#endif
			sprintf(m_pMountDir, "%s", diskInfo.mountdir);
			backupInfo.pMountDir = (unsigned long)m_pMountDir;
		}
		else
		{
			backupInfo.pMountDir = NULL;
		}
	}
	else if (NET_DISK_TYPE_CDDVD == diskInfo.diskType)
	{
		sprintf(m_pMountDir, "%s", diskInfo.mountdir);
		backupInfo.pMountDir = (unsigned long)m_pMountDir;
	}
	else
	{
		assert(false);
	}
	backupInfo.diskType     = (unsigned long)diskInfo.diskType;
	backupInfo.dataType     = m_dataSaveType;		
	if (m_bHavePlayer)
	{
		backupInfo.bHaveVideoPlay = 1;
	}
	else
	{
		backupInfo.bHaveVideoPlay = 0;
	}
	backupInfo.bCombined = 0;
	memset(&msgData, 0, sizeof(MESSAGE_DATA));
	CMessageMan::Instance()->SendMsgDataToServer(0, LOCAL_DEVICE_ID, CMD_REQUEST_LOCAL_CLIENT_BACKUP_STARTREAD, (unsigned char *)(&backupInfo), sizeof(backupInfo), msgData);	
	pCmd = reinterpret_cast<PACKCMD *>(msgData.pData);
	cmdType = (NET_PROTOCOL)pCmd->cmdType;
	CMessageMan::Instance()->ReleaseMsgBuff(msgData);
	if (CMD_REPLY_LOCAL_CLIENT_BACKUP_STARTREAD_FAIL == cmdType)
	{
		MessageBox(m_pStrTable->String(E_STR_ID_MESSAGE_08), m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK | DEFAULT_ICON_WARNING);

		//退回到上级界面
		KEY_VALUE_INFO keyValue;
		keyValue.type = KEY_TYPE_VIRTUAL;
		keyValue.value = KEY_VIRTUAL_ESC;
		AddKeyValueEx(keyValue);

		return KEY_VIRTUAL_MSG_STOP;
	}

	//开始备份成功
	m_bBackupIng = true;
	m_prgShow = true;
	m_precent = 0;
	m_displayPrecent = 0;
	SetPercent(0);
	CMessageMan::Instance()->RefreshBackupProgress(0);

	//进行备份
	memset(&msgData, 0, sizeof(MESSAGE_DATA));
	CMessageMan::Instance()->SendMsgDataToServer(0, LOCAL_DEVICE_ID, CMD_REQUEST_LOCAL_CLIENT_BACKUP_BACKUPING, NULL, 0, msgData);
	pCmd = reinterpret_cast<PACKCMD *>(msgData.pData);
	cmdType = (NET_PROTOCOL)pCmd->cmdType;
	CMessageMan::Instance()->ReleaseMsgBuff(msgData);	
	if (CMD_REPLY_LOCAL_CLIENT_BACKUP_BACKUPING_FAIL == cmdType)
	{
		m_bBackupIng = false;
		MessageBox(m_pStrTable->String(E_STR_ID_MESSAGE_08),m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK | DEFAULT_ICON_WARNING);
		MESSAGE_DATA msgData;
		CMessageMan::Instance()->SendMsgDataToServer(0, LOCAL_DEVICE_ID, CMD_REQUEST_LOCAL_CLIENT_BACKUP_STOP, NULL, 0, msgData);
		CMessageMan::Instance()->ReleaseMsgBuff(msgData);
		
		//退回到上级界面
		KEY_VALUE_INFO keyValue;
		keyValue.type = KEY_TYPE_VIRTUAL;
		keyValue.value = KEY_VIRTUAL_ESC;
		AddKeyValueEx(keyValue);
		return KEY_VIRTUAL_MSG_STOP;
	}
	SetButtonStatus(true, (DISK_TYPE)(diskInfo.diskType));

	return KEY_VIRTUAL_MSG_STOP;
}

//点击退出键
unsigned long CBackUpInfoDlg::OnCancel()
{
	if (m_bBackupIng)
	{
		//发送取消备份
		printf("exit backup\n");
		MESSAGE_DATA msgData;
		CMessageMan::Instance()->SendMsgDataToServer(0, LOCAL_DEVICE_ID, CMD_REQUEST_LOCAL_CLIENT_BACKUP_STOP, NULL, 0, msgData);
		CMessageMan::Instance()->ReleaseMsgBuff(msgData);
		EndProgress();	
		m_bBackupIng = false;
		SetButtonStatus(false);
	}

	CDialog::OnCancel();
	return KEY_VIRTUAL_MSG_STOP;
}

//右键退出
void CBackUpInfoDlg::EndDialog(DLG_END_VALUE result)
{
	if (m_bBackupIng)
	{
		return;
	}
	
	CDialog::EndDialog(result);	
}

BEGIN_MSG_MAP(CBackUpInfoDlg, GUI::CDialog)
	ON_MSG_EX(m_backUpInfoList.GetID(), KEY_VIRTUAL_MSG_DEFAULT_TAG, OnClickList)
	ON_MSG(m_diskManBtn.GetID(), KEY_VIRTUAL_ENTER, OnClickDiskMan)
END_MSG_MAP()

unsigned long CBackUpInfoDlg::OnClickList(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	unsigned short row = (keyID >> 16)&0x7fff;	//行
	unsigned short col = keyID&0xffff;			//列
	char str[128] = {0};
	GUI::CComboCtrl* pCombo = NULL;
	GUI::CListCtrl& list = m_backUpInfoList;
	GUI::CStatic *pStatic = NULL;
	GUI::CCheckCtrl *pCheck = NULL;
	POS pos = NULL;
	float spaceG = 0.0;
	bool bTemp = false;
	unsigned long  temp =  0;

	if (m_bBackupIng)
	{
		return KEY_VIRTUAL_MSG_STOP;
	}

	if ((ROW_STORAGE_TYPE == row) && (1 == col))
	{
		pCombo = dynamic_cast<GUI::CComboCtrl*>(list.GetItemWnd(row, col));
		assert(pCombo);
		pos = (POS)(pCombo->GetCurItemData());
		m_curDevicePos = pos;
		DVR_DISK_INFO &diskInfo = m_pDiskInfoList->GetAt(m_curDevicePos);
		if ((NET_DISK_TYPE_CDDVD == diskInfo.diskType) && (NET_DISK_NO_CD == diskInfo.diskStatus))
		{
			MESSAGE_DATA msgData;
			CMessageMan::Instance()->SendMsgDataToServer(0, LOCAL_DEVICE_ID, CMD_REQUEST_DISK_GET_CD_INFO, NULL, 0, msgData);
			DVR_DISK_INFO* cdInfo = (DVR_DISK_INFO *)(msgData.pData + sizeof(PACKCMD));
			int num = (msgData.dataLen - sizeof(PACKCMD))/sizeof(DVR_DISK_INFO);
			if (num > 0)
			{
				for (int i = 0; i < num; ++i)
				{	
					if (0 == strcmp(diskInfo.diskname, cdInfo[i].diskname))
					{
						memcpy(&diskInfo, &cdInfo[i], sizeof(diskInfo));
						break;							
					}
				}
			}			
			CMessageMan::Instance()->ReleaseMsgBuff(msgData);			
		}

		
		pStatic = dynamic_cast<GUI::CStatic*>(list.GetItemWnd(ROW_FREE_SIZE, 1));
		assert(pStatic);				
		spaceG = diskInfo.freeSpace / (float)1024;
		sprintf(str, "%0.3f", spaceG);			
		pStatic->SetCaption(str);

		if (NET_DISK_TYPE_CDDVD == diskInfo.diskType)
		{
			pCombo = dynamic_cast<GUI::CComboCtrl*>(list.GetItemWnd(ROW_BACKUP_TYPE, 1));
			assert(pCombo);
			pCombo->SetCurItem(NET_BACKUP_DATA_TYPE_DVR);
			pCombo->SetEnable(false, true);
			m_dataSaveType = NET_BACKUP_DATA_TYPE_DVR; 
			m_diskManBtn.SetEnable(false, true);
		}
		else
		{
			pCombo = dynamic_cast<GUI::CComboCtrl*>(list.GetItemWnd(ROW_BACKUP_TYPE, 1));
			assert( pCombo );
			pCombo->SetEnable(true, true);
			m_diskManBtn.SetEnable(true,true);
		}

		m_needSpace = CalNeedSpaceSize(m_fileNum, (DISK_TYPE)(diskInfo.diskType), m_dataSaveType, m_bHavePlayer);
		pStatic = dynamic_cast<GUI::CStatic*>(list.GetItemWnd(ROW_FILE_SIZE, 1));
		assert(pStatic);				
		spaceG = m_needSpace / (float)1024;
		sprintf(str, "%0.3f", spaceG);			
		pStatic->SetCaption(str);
	}
	else if ((ROW_PLAYER == row) && (1 == col))
	{
		pCheck = dynamic_cast<GUI::CCheckCtrl*>(list.GetItemWnd(row, col));
		assert( pCheck );
		if (pCheck->IsCheck())
		{
			bTemp = true;
		}
		else
		{
			bTemp = false;
		}
		if (bTemp != m_bHavePlayer)
		{
			assert(m_curDevicePos != NULL);
			DVR_DISK_INFO &diskInfo = m_pDiskInfoList->GetAt(m_curDevicePos);
			m_bHavePlayer = bTemp;
			m_needSpace = CalNeedSpaceSize(m_fileNum, (DISK_TYPE)(diskInfo.diskType), m_dataSaveType, m_bHavePlayer);
			pStatic = dynamic_cast<GUI::CStatic*>(list.GetItemWnd(ROW_FILE_SIZE, 1));
			assert(pStatic);				
			spaceG = m_needSpace / (float)1024;
			sprintf(str, "%0.3f", spaceG);			
			pStatic->SetCaption(str);
		}
	}
	else if ((ROW_BACKUP_TYPE == row) && (1 == col))
	{
		pCombo = dynamic_cast<GUI::CComboCtrl*>(list.GetItemWnd(row, col));
		assert(pCombo);
		temp = pCombo->GetCurItemData();
		if ((BACKUP_DATA_TYPE)temp != m_dataSaveType)
		{
			assert(m_curDevicePos != NULL);
			DVR_DISK_INFO &diskInfo = m_pDiskInfoList->GetAt(m_curDevicePos);
			m_dataSaveType = (BACKUP_DATA_TYPE)temp;
			m_needSpace = CalNeedSpaceSize(m_fileNum, (DISK_TYPE)(diskInfo.diskType), m_dataSaveType, m_bHavePlayer);
			pStatic = dynamic_cast<GUI::CStatic*>(list.GetItemWnd(ROW_FILE_SIZE, 1));
			assert(pStatic);				
			spaceG = m_needSpace / (float)1024;
			sprintf(str, "%0.3f", spaceG);			
			pStatic->SetCaption(str);
		}
	}

	return KEY_VIRTUAL_MSG_STOP;
}



void CBackUpInfoDlg::OnIdle()
{
	if (m_bBackupIng)
	{
		CMessageMan::Instance()->GetBackupProgress(&m_precent);
		if (m_precent <= 5)
		{
			if (m_displayPrecent < 5)
			{
				SetPercent(m_displayPrecent);
				m_displayPrecent++;
				PUB_Sleep(200);				
			}			
		}
		else if (m_precent < 100)
		{
			if (m_displayPrecent < m_precent)
			{
				m_displayPrecent = m_precent;
				SetPercent(m_displayPrecent);
//				m_displayPrecent++;
				PUB_Sleep(200);
			}
		}
		else if (100 == m_precent)//100
		{
			PUB_Sleep(5000);

			SetPercent(100);			
			EndProgress();

			MESSAGE_DATA msgData;
			CMessageMan::Instance()->SendMsgDataToServer(0, LOCAL_DEVICE_ID, CMD_REQUEST_LOCAL_CLIENT_BACKUP_STOP, NULL, 0, msgData);
			CMessageMan::Instance()->ReleaseMsgBuff(msgData);
			MessageBox(m_pStrTable->String(E_STR_ID_MESSAGE_09), m_pStrTable->String(E_STR_ID_TITLE_BACKUP), DEFAULT_BTN_OK);
			m_bBackupIng = false;
			SetButtonStatus(false);

			KEY_VALUE_INFO keyValue;
			keyValue.type = KEY_TYPE_VIRTUAL;
			keyValue.value = KEY_VIRTUAL_ESC;
			AddKeyValueEx(keyValue);
		}
		else //101
		{
			PUB_Sleep(5000);
			EndProgress();	
			MESSAGE_DATA msgData;
			CMessageMan::Instance()->SendMsgDataToServer(0, LOCAL_DEVICE_ID, CMD_REQUEST_LOCAL_CLIENT_BACKUP_STOP, NULL, 0, msgData);
            CMessageMan::Instance()->ReleaseMsgBuff(msgData);
			MessageBox(m_pStrTable->String(E_STR_ID_MESSAGE_08), m_pStrTable->String(E_STR_ID_TITLE_BACKUP), DEFAULT_BTN_OK);
			m_bBackupIng = false;
			SetButtonStatus(false);

			KEY_VALUE_INFO keyValue;
			keyValue.type = KEY_TYPE_VIRTUAL;
			keyValue.value = KEY_VIRTUAL_ESC;
			AddKeyValueEx(keyValue);
		}

	}
}



void CBackUpInfoDlg::SetPercent( int percet )
{
	m_progressDlg.SetPercent( percet, m_prgShow );
}

void CBackUpInfoDlg::EndProgress()
{
	m_progressDlg.Show( m_prgShow, true );
}

void CBackUpInfoDlg::SetButtonStatus(bool bBackuping, DISK_TYPE diskType)
{
	GUI::CButton *pButton = NULL;

	//设置备份键的状态
	pButton = dynamic_cast<GUI::CButton*>(GetCtrl(CTRLID_DEFAULT_BTN_OK));
	assert(pButton);
	if (bBackuping)
	{
		pButton->SetEnable(false);
	}
	else
	{
		pButton->SetEnable(true);
	}

	

	//设置取消键的状态
	pButton = dynamic_cast<GUI::CButton*>(GetCtrl(CTRLID_DEFAULT_BTN_CANCEL));
	assert(pButton);
	if (bBackuping && (NET_DISK_TYPE_CDDVD == diskType))
	{
		pButton->SetEnable(false);
	}
	else
	{
		pButton->SetEnable(true);
	}

	/*
	//设置退出键的状态
	pButton = dynamic_cast<GUI::CButton*>(GetCtrl(CTRLID_DEFAULT_BTN_CANCEL));
	assert(pButton);
	if (bBackuping && (NET_DISK_TYPE_CDDVD == diskType))
	{
		pButton->SetEnable(false);
	}
	else
	{
		pButton->SetEnable(true);
	}
	*/

	if (bBackuping)
	{
		m_diskManBtn.SetEnable(false);
	}
	else
	{
		m_diskManBtn.SetEnable(true);
	}
}

unsigned long CBackUpInfoDlg::CalNeedSpaceSize(unsigned long fileNum, DISK_TYPE deviceType, BACKUP_DATA_TYPE dataSaveType, bool bHavePlayer)
{
	unsigned long ret = 0;
	ret = fileNum * MAX_RECORD_FILE_LEN_M;


	if (m_bHavePlayer)
	{
		ret += 6; //播放器的大小6M。用一个最大值。
	}

	ret += 2;  //存放文件目录格式。用一个最大值2M。

	ret += 10; //用于存放日志

	return ret;
}























