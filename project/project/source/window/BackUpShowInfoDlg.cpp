#include "BackUpShowInfoDlg.h"
#ifndef __ENVIRONMENT_WIN32__
#include <sys/vfs.h> 
#endif

CBackUpShowInfoDlg::CBackUpShowInfoDlg()
{
	m_pDiskInfoList = NULL;
	m_bBackupIng=false;
	m_prgShow=false;
	m_precent       = 0;
	m_displayPrecent = 0;
	m_pDiskName=NULL;
	m_pMountDir=NULL;
	m_pFileInfoBackup=NULL;
	m_fileNum=0;
}

CBackUpShowInfoDlg::~CBackUpShowInfoDlg()
{

}

void CBackUpShowInfoDlg::SetInitialInfo(DVR_DISK_INFO_LIST *pDiskInfoList, FILE_INFO_EX_BACKUP *pDataBackup, unsigned long fileNum, unsigned long allSize)
{
	//assert(pData != NULL);
	assert(pDiskInfoList->GetCount() > 0);
	m_pDiskInfoList = pDiskInfoList;
	m_pFileInfoBackup = pDataBackup;
	m_fileNum       = fileNum;
	m_needSpace     = allSize;
}
void CBackUpShowInfoDlg::OnInitial()
{
	m_style |= WND_NO_TITLE;
	GUI::CDialog::OnInitial();
	SetImage(BMP_MEM_DLG_LARGE, false);
	SetCaption(m_pStrTable->String(E_STR_ID_TITLE_BACKUP_INFO), false);
	

	GUI::CComboCtrl* pCombo = NULL;
	GUI::CStatic* pStatic = NULL;
	GUI::CButton* pButton = NULL;

	SetDefaultBtn(DEFAULT_BTN_APPLY | DEFAULT_BTN_CANCEL );
	SetDefaultBtnCaption(DEFAULT_BTN_APPLY, m_pStrTable->String(E_STR_ID_START));
	SetDefaultBtnCaption(DEFAULT_BTN_CANCEL, m_pStrTable->String(E_STR_ID_EXIT));

	unsigned short xLeft = m_length[DLG_OFFSET_LEFT], yTop = m_length[TITLE_HEIGHT] + m_length[DLG_OFFSET_TOP];
	unsigned short width = m_rect.Width() - 2 * m_length[DLG_OFFSET_LEFT];
	unsigned short listWidth = width - 4;
	unsigned short x = xLeft, y = yTop;
	unsigned short listH=0;
	if (m_width>800)
	{
		 listH = (m_rect.Height() - yTop - m_length[DEFAULT_BTN_BOTTOM_YPOS] - m_length[DLG_OFFSET_BOTTOM])/4*3-m_length[BTN_HEIGHT];
	}
	else
	{
		listH = (m_rect.Height() - yTop - m_length[DEFAULT_BTN_BOTTOM_YPOS] - m_length[DLG_OFFSET_BOTTOM])/4*3-3*m_length[BTN_HEIGHT];
	}
	
	m_backUpInfoList.Create(GUI::CWnd::GetRandWndID(), x, y, listWidth, listH, this, 0, 1, true );
	m_backUpInfoList.AddColumn(" ", width * 2 / 32);
	m_backUpInfoList.AddColumn(m_pStrTable->String(E_STR_ID_CH), width * 2 / 32);
	m_backUpInfoList.AddColumn(m_pStrTable->String(E_STR_ID_START_TIME), width * 12 / 32);
	m_backUpInfoList.AddColumn(m_pStrTable->String(E_STR_ID_END_TIME), width * 20 / 32-m_length[SCROLL_WIDTH] - 2);
	
	y=yTop+listH;
	listH=5*m_length[BTN_HEIGHT];
	m_backUpOperationList.Create(GUI::CWnd::GetRandWndID(), x, y, listWidth, listH, this, 0, 2, false );

	m_backUpOperationList.AddColumn(NULL, width/2);
	m_backUpOperationList.AddColumn(NULL, width/2 - 4);
	{
		m_backUpOperationList.AddItem(ROW_FILE_SIZE, 0, m_pStrTable->String(E_STR_ID_SIZE_GB));
		pStatic = dynamic_cast<GUI::CStatic*>(m_backUpOperationList.AddItem(ROW_FILE_SIZE, 1, WND_TYPE_STATIC,m_length[BTN_WIDTH]+15, m_length[BTN_HEIGHT]));
		assert( pStatic );

		m_backUpOperationList.AddItem(ROW_STORAGE_TYPE, 0, m_pStrTable->String(E_STR_ID_STORAGE_TYPE));
		pCombo = dynamic_cast<GUI::CComboCtrl*>(m_backUpOperationList.AddItem(ROW_STORAGE_TYPE, 1,  WND_TYPE_COMBO) );

		m_backUpOperationList.AddItem(ROW_FREE_SIZE, 0, m_pStrTable->String(E_STR_ID_FREE_GB) );		
		pStatic = dynamic_cast<GUI::CStatic*>( m_backUpOperationList.AddItem(ROW_FREE_SIZE, 1, WND_TYPE_STATIC,m_length[BTN_WIDTH]+15, m_length[BTN_HEIGHT]) );
		assert( pStatic );

		m_backUpOperationList.AddItem(ROW_DISK_CLEAN, 0, m_pStrTable->String(E_STR_ID_DISK_CLEAN_UP) );
		pButton=dynamic_cast<GUI::CButton*>( m_backUpOperationList.AddItem(ROW_DISK_CLEAN, 1, WND_TYPE_BUTTON,m_length[BTN_WIDTH]+25, m_length[BTN_HEIGHT]) );
		pButton->SetCaption(m_pStrTable->String(E_STR_ID_DISK_CLEAN_UP),false);
	}
	m_backUpOperationList.CreateListFocusMap();

	GUI::CRect listRect =  m_backUpOperationList.GetClientRect();
	y += listRect.Height() + 4;
	m_progressDlg.Create( GUI::CWnd::GetRandWndID(), x, y, width, m_length[LIST_ITEM_HEIGHT], this, 0, 2 );
	
}

void CBackUpShowInfoDlg::OnPostInitial()
{
	GUI::CDialog::OnPostInitial();
	//刷新控件
	//添加m_backUpInfoList
	char str[128]={0};
	tm time;
	for(int i=0;i<m_fileNum;i++)
	{
		memset(str,0,sizeof(str));
		snprintf(str,128,"%d",i+1);
		m_backUpInfoList.AddItem(i,COL_SEQ,str);

		memset(str,0,sizeof(str));
		snprintf(str,128,"%d",m_pFileInfoBackup[i].fileInfo.chnn+1);
		m_backUpInfoList.AddItem(i,COL_CHNN,str);

		memset(str,0,sizeof(str));
		memset(&time,0,sizeof(tm));
		time= DVRTime32ToTm(m_pFileInfoBackup[i].fileInfo.startTime);
		snprintf(str,128,"%d-%02d-%02d %02d:%02d:%02d", time.tm_year+1900,time.tm_mon+1, time.tm_mday, time.tm_hour, time.tm_min, time.tm_sec);
		m_backUpInfoList.AddItem(i,COL_STARTTIME,str);

		memset(str,0,sizeof(str));
		memset(&time,0,sizeof(tm));
		time= DVRTime32ToTm(m_pFileInfoBackup[i].fileInfo.endTime);
		snprintf(str,128,"%d-%02d-%02d %02d:%02d:%02d", time.tm_year+1900,time.tm_mon+1, time.tm_mday, time.tm_hour, time.tm_min, time.tm_sec);
		m_backUpInfoList.AddItem(i,COL_ENDTIME,str);
	}

	//添加m_backUpOpeaationList
	GUI::CComboCtrl* pCombo = NULL;
	GUI::CStatic* pStatic = NULL;
	int usbCount = 0;
	int esataCount = 0;
	int usbDiskCount = 0;
	int otherCount = 0;
	POS pos = NULL;
	pCombo = dynamic_cast<GUI::CComboCtrl*>(m_backUpOperationList.GetItemWnd(ROW_STORAGE_TYPE, 1));
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
				memset(str,0,sizeof(str));
				sprintf(str, "USB-%d", usbCount);
			}
			else if (NET_DISK_REMDISK_INTERFACE == diskInfo.diskInterface)
			{
				usbDiskCount++;
				memset(str,0,sizeof(str));
				sprintf(str, "USB-DISK-%d", usbDiskCount);
			}
			else if (NET_DISK_ESATA_INETERFACE == diskInfo.diskInterface)
			{
				esataCount++;
				memset(str,0,sizeof(str));
				sprintf(str, "e-SATA-%d", esataCount);
			}
			else
			{
				otherCount++;
				memset(str,0,sizeof(str));
				sprintf(str, "USB-%d", otherCount);
			}			
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
		
	//计算剩余空间
	pStatic = dynamic_cast<GUI::CStatic*>(m_backUpOperationList.GetItemWnd(ROW_FREE_SIZE, 1));
	assert( pStatic );	
	float spaceG = 0.0;
	spaceG = diskInfo.freeSpace / (float)1024;
	memset(str,0,sizeof(str));
	sprintf(str, "%0.3f", spaceG);			
	pStatic->SetCaption(str, false);
	
	pStatic = dynamic_cast<GUI::CStatic*>(m_backUpOperationList.GetItemWnd(ROW_FILE_SIZE, 1));
	assert( pStatic );	
	memset(str,0,sizeof(str));
	
	printf("%s,%d,m_needSpace:%d\n",__FILE__,__LINE__,m_needSpace);
	float tmp=(m_needSpace / ((float)1024)) /((float)1024)/((float)1024);
	if(tmp<0.001)
	{
		tmp=0.001;
	}
	printf("tmp=%0.3f\n",tmp);
	sprintf(str, "%0.3f", tmp);
	pStatic->SetCaption(str,false);
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

void CBackUpShowInfoDlg::OnDestroy()
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


void CBackUpShowInfoDlg::OnClickDiskCleanUp()
{
#ifdef __ENVIRONMENT_LINUX__
	if ((NULL == m_curDevicePos) || (NULL == m_pDiskInfoList))
	{
		return;
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
			GUI::CStatic *pStatic = dynamic_cast<GUI::CStatic*>(m_backUpOperationList.GetItemWnd(ROW_FREE_SIZE, 1));
			assert(pStatic);				
			float spaceG = 0.0;
			spaceG = diskInfo.freeSpace / (float)1024;
			snprintf(szCmd, 48, "%0.3f", spaceG);			
			pStatic->SetCaption(szCmd);
		}
	}
#endif

}


unsigned long CBackUpShowInfoDlg::OnApply()
{
	LOCAL_CLIENT_BACKUP_INFO backupInfo;
	MESSAGE_DATA msgData;
	PACKCMD *pCmd = NULL;
	unsigned long cmdType;
	unsigned long freeSpace = 0;
	POS pos = NULL;

	if (m_bBackupIng)
	{
		return KEY_VIRTUAL_MSG_STOP;
	}

	//判断剩余空间是否够
	DVR_DISK_INFO &diskInfo = m_pDiskInfoList->GetAt(m_curDevicePos);
	freeSpace = diskInfo.freeSpace;
	
	unsigned long needSpace=((m_needSpace)/(float)1024)/((float)1024) + 6;
	printf("freeSpace=%d,needSpace=%d\n",freeSpace,needSpace);
	if (freeSpace < needSpace)
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


	backupInfo.pFileInfoBackup    = (unsigned long)m_pFileInfoBackup;
	backupInfo.fileNum            = m_fileNum;
	backupInfo.startTime          = 0;
	backupInfo.endTime            = 0;
	sprintf(m_pDiskName, "%s", diskInfo.diskname);
	backupInfo.pDiskName          = (unsigned long)m_pDiskName;
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
	else
	{
		assert(false);
	}
	backupInfo.diskType       = (unsigned long)diskInfo.diskType;
	backupInfo.dataType       = NET_BACKUP_DATA_TYPE_DVR;		
	backupInfo.bHaveVideoPlay = 1;
	backupInfo.bCombined      = 1;

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
unsigned long CBackUpShowInfoDlg::OnCancel()
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
void CBackUpShowInfoDlg::EndDialog(DLG_END_VALUE result)
{
	if (m_bBackupIng)
	{
		return;
	}

	CDialog::EndDialog(result);	
}


BEGIN_MSG_MAP(CBackUpShowInfoDlg, GUI::CDialog)
	ON_MSG_EX(m_backUpOperationList.GetID(), KEY_VIRTUAL_MSG_DEFAULT_TAG, OnClickList)
END_MSG_MAP()

unsigned long CBackUpShowInfoDlg::OnClickList(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	unsigned short row = (keyID >> 16)&0x7fff;	//行
	unsigned short col = keyID&0xffff;			//列
	char str[128] = {0};
	GUI::CComboCtrl* pCombo = NULL;
	GUI::CListCtrl& list = m_backUpOperationList;
	GUI::CStatic *pStatic = NULL;
	POS pos = NULL;
	float spaceG = 0.0;

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
	}
	else if ((ROW_DISK_CLEAN == row) && (1 == col))
	{
		OnClickDiskCleanUp();
	}
	return KEY_VIRTUAL_MSG_STOP;

}


void CBackUpShowInfoDlg::OnIdle()
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
				SetPercent(m_displayPrecent);
				m_displayPrecent++;
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

void CBackUpShowInfoDlg::SetPercent( int percet )
{
	m_progressDlg.SetPercent( percet, m_prgShow );
}


void CBackUpShowInfoDlg::EndProgress()
{
	m_progressDlg.Show( m_prgShow, true );
}



void CBackUpShowInfoDlg::SetButtonStatus(bool bBackuping, DISK_TYPE diskType)
{
	GUI::CButton *pButton = NULL;

	//设置备份键的状态
	pButton = dynamic_cast<GUI::CButton*>(GetCtrl(CTRLID_DEFAULT_BTN_APPLY));
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

	pButton = dynamic_cast<GUI::CButton*>(m_backUpOperationList.GetItemWnd(ROW_DISK_CLEAN,1));
	assert(pButton!=NULL);
	//pCombo=dynamic_cast<GUI::CComboCtrl*>(m_preBackUpList.GetItemWnd(row,COL_SEQ));
	if (bBackuping)
	{
		pButton->SetEnable(false);
	}
	else
	{
		pButton->SetEnable(true);
	}
}
