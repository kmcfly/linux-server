#include "UsbBackupProc.h"

#ifdef __ENVIRONMENT_WIN32__
#include "direct.h"
#endif

#include "CreateHtmlLog.h"



#ifdef __ENVIRONMENT_WIN32__

// �ж�Ŀ¼(����������ѯ��Ŀ¼,��������ʧ��.)�Ƿ���ڵĺ���
static BOOL IsPathExist(LPCTSTR lpszPath)
{
	char szTemp[MAX_PATH];
	lstrcpy(szTemp, lpszPath);

	BOOL bExist; // ���������ʾĿ¼�Ƿ����
	if(szTemp[strlen(lpszPath)-1]=='\\')
	{
		szTemp[strlen(lpszPath)-1]='\0';    // ��ɾ�����ġ�\��
	}
	WIN32_FIND_DATA wfd;                                  // ����
	HANDLE hFind = FindFirstFile(szTemp, &wfd);                 
	if(hFind == INVALID_HANDLE_VALUE)
	{
		bExist = FALSE;            // û���ҵ��䱸��Ŀ¼�϶�������
	}
	else
	{
		if(wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) // ����ҵ��Ľ���Ƿ�Ŀ¼
			bExist = TRUE;     // ��Ŀ¼,Ŀ¼����
		else
			bExist = FALSE;       // ���ļ�,Ŀ¼������

		FindClose(hFind);
	}

	// debug
	printf("ִ��IsPathExist()���!\n");

	return bExist;
}

static BOOL CreateDirectoryEx(LPCTSTR lpszCreatePath)
{
	char szCreatePathTemp[MAX_PATH];
	lstrcpy(szCreatePathTemp, lpszCreatePath);
	if(szCreatePathTemp[strlen(szCreatePathTemp)-1]=='/')
	{
		szCreatePathTemp[strlen(szCreatePathTemp)-1]='\0';    // ��ɾ�����ġ�\��
	}

	char szTemp[MAX_PATH] = ""; // ��ʱ����
	// ����·���ָ�����'\'
	char *p = strchr(szCreatePathTemp, '/'); // ��һ�εĳ���λ��(��ַ)
	if (p != NULL)
	{
		p = strchr(p+1, '/');   // �ڶ���λ��, �����NULL ��˵����һ��Ŀ¼, ��: c:\path ���������Ŀ¼�����ڻ�ֱ�Ӵ�����Ŀ¼
	}   

	while (p != NULL)            // ֮���λ��(Ҫ�ܽ����ѭ��, �����Ƕ���Ŀ¼, ��: c:\һ��Ŀ¼\����Ŀ¼\...)
	{
		strncpy(szTemp, szCreatePathTemp, p-szCreatePathTemp);     // ��: e:\1

		if (!IsPathExist(szTemp)) // ���������, ��һ·�����������ڶ���Ŀ¼
		{
			// ����
			if (!CreateDirectory(szTemp, NULL))
			{
				printf("����Ŀ¼%sʧ��!�������: %d\n", szTemp, GetLastError());
				return FALSE;
			}
			printf("�ѳɹ�����Ŀ¼:%s\n", szTemp); // �����ò�Ŀ¼, ֮��һ·���´��������ж�

			p = strchr(p+1, '/');   // ��һ����Ŀ¼
			while( p != NULL )
			{
				strncpy(szTemp, szCreatePathTemp, p-szCreatePathTemp); // ��: e:\1\2

				// ����
				if (!CreateDirectory(szTemp, NULL))
				{
					printf("����Ŀ¼%sʧ��!�������: %d\n", szTemp, GetLastError());
					return FALSE;
				}
				printf("�ѳɹ�����Ŀ¼:%s\n", szTemp);

				p = strchr(p+1, '/');
			}

		}

		if ( p != NULL ) // ��ֹ�����ڲ��while
		{
			p = strchr(p+1, '/');   // ��һ����Ŀ¼
		}
	}

	if (!IsPathExist(lpszCreatePath)) // ���Ŀ¼������, �򴴽�Ŀ¼.(Ҳ�Ǵ������һ��Ŀ¼)
	{
		// ����
		if (!CreateDirectory(lpszCreatePath, NULL))
		{
			printf("����Ŀ¼%sʧ��!�������: %d\n", lpszCreatePath, GetLastError());
			return FALSE;
		}
		printf("�ѳɹ�����Ŀ¼:%s\n", lpszCreatePath);
	}

	return TRUE;
}

#endif


CUsbBackupProc::CUsbBackupProc()
{
	m_pBackupInitialInfo = NULL;

	m_hBackupThread = PUB_THREAD_ID_NOINIT;
	m_bBackuping    = false;

	/*m_srcFileIndex  = 0;
	m_srcFrameCount = 0;

	m_desFileIndex = 0;
	m_pBackupPathName = NULL;*/

	//m_pMsgCallBack = NULL;
	//m_pCallBackObj = NULL;

	m_pBackupWriter = NULL;
	m_pBackupCutWriter = NULL;

	m_pRecFileEventInfo = NULL;
	m_eventInfoNum = 0;
}

//
CUsbBackupProc::~CUsbBackupProc()
{

	/*if (NULL != m_pBackupPathName)
	{
		delete m_pBackupPathName;
		m_pBackupPathName = NULL;
	}

	m_reader.Quit();*/
}

//
int CUsbBackupProc::StartBackup(BACKUP_INITIAL_INFO *pBackupInfo)
{
	assert(pBackupInfo != NULL);
	assert((NET_BACKUP_DATA_TYPE_AVI == pBackupInfo->dataType)
		 ||(NET_BACKUP_DATA_TYPE_DVR == pBackupInfo->dataType));
	assert(NULL != pBackupInfo->pSrcFilePath);

	m_pBackupInitialInfo = pBackupInfo;
	m_pRecFileEventInfo = NULL;
	m_eventInfoNum = 0;

	char szTmp[512] = {0};
#ifndef __ENVIRONMENT_WIN32__
	if (NULL == m_pBackupInitialInfo->pMountDir)
	{
		sprintf(szTmp, "rm -rf %s", USB_BACKUP_MOUNT);
		DVRSystem(szTmp);

		sprintf(szTmp, "mkdir -p %s", USB_BACKUP_MOUNT);
		if (0 != DVRSystem(szTmp))
		{
			return -1;
		}

		sprintf(szTmp, "umount %s", USB_BACKUP_MOUNT);
		DVRSystem(szTmp);

		sprintf(szTmp, "mount -t vfat %s %s", m_pBackupInitialInfo->pDiskName, USB_BACKUP_MOUNT);
		if (0 != DVRSystem(szTmp))
		{
			return -1;
		}
		printf("%s\n", szTmp);

		if (m_pBackupInitialInfo->bCombined)
		{			
			sprintf(m_desDataPath, "%s", USB_BACKUP_MOUNT);			
		}
		else
		{
			tm time = DVRTime32ToTm(GetCurrTime32());
			sprintf(m_desDataPath, "%s/%d%02d%02d%02d%02d%02d", USB_BACKUP_MOUNT, time.tm_year+1900, time.tm_mon+1, time.tm_mday, time.tm_hour, time.tm_min, time.tm_sec);
			sprintf(szTmp, "mkdir -p %s", m_desDataPath);
			if (0 != DVRSystem(szTmp))
			{
				sprintf(szTmp, "umount %s", USB_BACKUP_MOUNT);
				DVRSystem(szTmp);
				return -1;
			}
		}
		
		
		if (m_pBackupInitialInfo->bHaveVideoPlay)
		{
			sprintf(m_desVideoPlayPath, "%s/%s", USB_BACKUP_MOUNT, BACKUP_STORE_VIDEOPLAY_PATH);
			sprintf(szTmp, "mkdir -p %s", m_desVideoPlayPath);
			if (0 != DVRSystem(szTmp))
			{
				sprintf(szTmp, "umount %s", USB_BACKUP_MOUNT);
				DVRSystem(szTmp);
				return -1;
			}
		}

	}
	else
	{
		if (m_pBackupInitialInfo->bCombined)
		{
			sprintf(m_desDataPath, "%s", m_pBackupInitialInfo->pMountDir);	
			sprintf(szTmp, "mkdir -p %s", m_desDataPath);
			if (0 != DVRSystem(szTmp))
			{
				return -1;
			}
		}
		else
		{
			tm time = DVRTime32ToTm(GetCurrTime32());
			sprintf(m_desDataPath, "%s/%d%02d%02d%02d%02d%02d", m_pBackupInitialInfo->pMountDir, time.tm_year+1900, time.tm_mon+1, time.tm_mday, time.tm_hour, time.tm_min, time.tm_sec);
			sprintf(szTmp, "mkdir -p %s", m_desDataPath);
			if (0 != DVRSystem(szTmp))
			{
				return -1;
			}
		}
		
		if (m_pBackupInitialInfo->bHaveVideoPlay)
		{
			sprintf(m_desVideoPlayPath, "%s/%s", m_pBackupInitialInfo->pMountDir, BACKUP_STORE_VIDEOPLAY_PATH);
			sprintf(szTmp, "mkdir -p %s", m_desVideoPlayPath);
			if (0 != DVRSystem(szTmp))
			{
				return -1;
			}
		}		
	}
#else
	assert(NULL != m_pBackupInitialInfo->pMountDir);
	tm time = DVRTime32ToTm(GetCurrTime32());
	sprintf(m_desDataPath, "%s/%d%02d%02d%02d%02d%02d", m_pBackupInitialInfo->pMountDir, time.tm_year+1900, time.tm_mon+1, time.tm_mday, time.tm_hour, time.tm_min, time.tm_sec);
	if (!CreateDirectoryEx(m_desDataPath))
	{
		return -1;
	}
#endif

	
	bool bMustAudioFormat = false;
	if (!m_pBackupInitialInfo->bCombined)
	{	
		m_pRecFileEventInfo = NULL;
		m_eventInfoNum = 0;
		for (unsigned long i = 0; i < m_pBackupInitialInfo->fileNum; ++i)
		{
			if (m_pBackupInitialInfo->pFileInfoBackup[i].fileInfo.startTime < m_pBackupInitialInfo->startTime)
			{
				m_pBackupInitialInfo->pFileInfoBackup[i].fileInfo.startTime = m_pBackupInitialInfo->startTime;
			}			

			if (m_pBackupInitialInfo->pFileInfoBackup[i].fileInfo.endTime > m_pBackupInitialInfo->endTime)
			{
				m_pBackupInitialInfo->pFileInfoBackup[i].fileInfo.endTime = m_pBackupInitialInfo->endTime;
			}
		}
		bMustAudioFormat = false;
	}
	else
	{
		bMustAudioFormat = true;
		if (m_pBackupInitialInfo->fileNum > MAX_REC_FILE_EVENT_INFO)
		{
			m_eventInfoNum = MAX_REC_FILE_EVENT_INFO;
		}
		else
		{
			m_eventInfoNum = m_pBackupInitialInfo->fileNum;
		}
		m_pRecFileEventInfo = new REC_FILE_EVENT_INFO [m_eventInfoNum];
		memset(m_pRecFileEventInfo, 0, sizeof(REC_FILE_EVENT_INFO) * m_eventInfoNum);

		for (unsigned long i = 0; i < m_eventInfoNum; ++i)
		{
			m_pRecFileEventInfo[i].chnn = m_pBackupInitialInfo->pFileInfoBackup[i].fileInfo.chnn;
			m_pRecFileEventInfo[i].type = m_pBackupInitialInfo->pFileInfoBackup[i].fileInfo.type;
			if (0 == m_pBackupInitialInfo->pFileInfoBackup[i].hasBackupAudio)
			{
				m_pRecFileEventInfo[i].audio = 0;
			}
			else
			{
				m_pRecFileEventInfo[i].audio = 1;
			}
		}
	}
	
    bool bSuc = false;
	do
	{
		if (0 != m_backupReader.Start(m_pBackupInitialInfo->pFileInfoBackup, m_pBackupInitialInfo->fileNum, m_pBackupInitialInfo->pSrcFilePath,			                          
			                          m_pBackupInitialInfo->dataType, m_pBackupInitialInfo->pReclogMan, bMustAudioFormat))
		{
			break;
		}


		if (m_pBackupInitialInfo->bCombined)
		{
			if (NULL != m_pBackupCutWriter)
			{
				m_pBackupCutWriter = new CBackupCutWriter;
			}

			if (0 != m_pBackupCutWriter->Start(m_desDataPath, m_pRecFileEventInfo, m_eventInfoNum))
			{
				m_backupReader.Stop();
				delete m_pBackupCutWriter;
				m_pBackupCutWriter = NULL;

				break;
			}
		}
		else
		{
			if (NULL == m_pBackupWriter)
			{
				m_pBackupWriter = new CBackupWriter;
			}

			if (0 != m_pBackupWriter->Start(m_desDataPath, m_pBackupInitialInfo->dataType))
			{
				m_backupReader.Stop();
				delete m_pBackupWriter;
				m_pBackupWriter = NULL;

				break;
			}
		}		
		bSuc = true;		
	}while(0);


	if (!bSuc)
	{
		if (NULL == m_pBackupInitialInfo->pMountDir)
		{
			sprintf(szTmp, "umount %s", USB_BACKUP_MOUNT);
			printf("%s\n", szTmp);
			DVRSystem(szTmp);
			return -1;
		}
	}
	

//
//	if (NET_BACKUP_DATA_TYPE_DVR == m_pBackupInitialInfo->dataType)
//	{
//#ifndef __ENVIRONMENT_WIN32__
//		sprintf(m_pBackupPathName, "mkdir -p %s/%s", m_pBackupInitialInfo->pBackupPath, BACKUP_STORE_DATA_PATH);
//		if (0 != DVRSystem(m_pBackupPathName))
//		{
//			return -1;
//		}
//#else
//		sprintf(m_pBackupPathName, "%s/%s", m_pBackupInitialInfo->pBackupPath, BACKUP_STORE_DATA_PATH);
//		if (!CreateDirectoryEx(m_pBackupPathName))
//		{
//			return -1;
//		}
//#endif
//		sprintf(m_pBackupPathName, "%s/%s/reclog.bin", m_pBackupInitialInfo->pBackupPath, BACKUP_STORE_DATA_PATH);
//		if (!CReclogEx::CreateLogFile(m_pBackupPathName, m_pBackupInitialInfo->fileNum, m_pBackupInitialInfo->channelNum))
//		{
//			return -1;
//		}
//	}


	m_pMsgCallBack = m_pBackupInitialInfo->callBack;
	m_pCallBackObj = m_pBackupInitialInfo->pCallBackObj;

	return 0;
}


/*
int CUsbBackupProc::Backup(MESSAGE_CALLBACK pCallBack, void *pCallBackObj)
{
	if (PUB_THREAD_ID_NOINIT != m_hBackupThread)
	{
		PUB_ExitThread(&m_hBackupThread, &m_bBackuping);
	}

	m_hBackupThread = PUB_CreateThread(BackupProc, this, &m_bBackuping);
	if(PUB_THREAD_ID_NOINIT == m_hBackupThread)
	{
		return -1;
	}

	m_pMsgCallBack = pCallBack;
	m_pCallBackObj = pCallBackObj;

	return 0;
}
*/

int CUsbBackupProc::Backup()
{
	bool bSuc = true;
	if (PUB_THREAD_ID_NOINIT != m_hBackupThread)
	{
		PUB_ExitThread(&m_hBackupThread, &m_bBackuping);
	}

	m_hBackupThread = PUB_CreateThread(BackupProc, this, &m_bBackuping);
	if(PUB_THREAD_ID_NOINIT == m_hBackupThread)
	{
		bSuc = false;
	}

	if (!bSuc)
	{
		if (NULL == m_pBackupInitialInfo->pMountDir)
		{
			char szTmp[128] = {0};
			sprintf(szTmp, "umount %s", USB_BACKUP_MOUNT);
			printf("%s\n", szTmp);
			DVRSystem(szTmp);
			return -1;
		}
	}
    
	

	return 0;
}

RESULT WINAPI CUsbBackupProc::BackupProc(void *pParam)
{
#ifdef __ENVIRONMENT_LINUX__
	pid_t pid = getpid();
	pid_t tid = syscall(__NR_gettid);
	printf("%s, %d, pid = %d, tid=%d\n", __FILE__, __LINE__, pid, tid);

#if defined(__CHIP3535__)
	char szCommond[128] = {0};
	snprintf(szCommond, sizeof(szCommond), "taskset -p 0x2 %d", tid);
	printf("%s:%s:%d, The commond is %s\n", __FUNCTION__, __FILE__, __LINE__, szCommond);
	DVRSystem(szCommond);
	PUB_Sleep(100);
	printf("%s:%s:%d, The commond is %s\n", __FUNCTION__, __FILE__, __LINE__, szCommond);
	DVRSystem(szCommond);
#endif

#endif

	CUsbBackupProc *pThis = (CUsbBackupProc *)pParam;
	pThis->BackupProc();

	return 0;
}


void CUsbBackupProc::StopBackup()
{
	if (PUB_THREAD_ID_NOINIT != m_hBackupThread)
	{
		PUB_ExitThread(&m_hBackupThread, &m_bBackuping);
	}
	m_backupReader.Stop();

	if (m_pBackupInitialInfo->bCombined)
	{
		if (m_pRecFileEventInfo != NULL)
		{
			m_pBackupCutWriter->WriteFileEventInfo();
			delete [] m_pRecFileEventInfo;
			m_pRecFileEventInfo = NULL;
			m_eventInfoNum = 0;
		}
		
		if (NULL != m_pBackupCutWriter)
		{
			m_pBackupCutWriter->Stop();
			
			delete m_pBackupCutWriter;
			m_pBackupCutWriter = NULL;
		}
	}
	else
	{
		if (NULL != m_pBackupWriter)
		{
			m_pBackupWriter->Stop();
			delete m_pBackupWriter;
			m_pBackupWriter = NULL;
		}
	}

	


	/*if (NULL != m_pBackupPathName)
	{
	delete m_pBackupPathName;
	m_pBackupPathName = NULL;
	}

	m_reader.Quit();*/
	

}

void CUsbBackupProc::BackupProc()
{
	int prePrecent = 0;
	int precent = 0;
	int count = 0;
	FRAME_INFO_EX frameInfo;
	int readerRet = 0;
	int writerRet = 0;
	char szCmd[128] = {0};
	memset(&frameInfo, 0, sizeof(FRAME_INFO_EX));
	frameInfo.length = MAX_VIDEO_FRAME_LEN;
	frameInfo.pData = new unsigned char [MAX_VIDEO_FRAME_LEN];
	memset(frameInfo.pData, 0 ,frameInfo.length);

	if (!m_pBackupInitialInfo->bCombined)
	{		
		CCreateHtmlLog::Instance()->Initial(false,m_pBackupInitialInfo,m_desDataPath);
		if (NET_BACKUP_DATA_TYPE_AVI == m_pBackupInitialInfo->dataType)
		{
			vector<std::string> *pAviNameVec = NULL;
			pAviNameVec=CCreateHtmlLog::Instance()->GetAllAviFileName();
			m_pBackupWriter->SetAviFileNameVec(pAviNameVec);
		}		

		CCreateHtmlLog::Instance()->WirteHtmlLogAction();
	}
	
	PRODUCT_TYPE productID = CProduct::Instance()->ProductType();

	while (m_bBackuping)
	{
#ifndef __ENVIRONMENT_WIN32__
		if (m_pBackupInitialInfo->bHaveVideoPlay)
		{
			sprintf(szCmd, "cp %s/* %s -rf", m_pBackupInitialInfo->pSrcPlayerPath, m_desVideoPlayPath);
			DVRSystem(szCmd);
		}
#endif
		if (NET_BACKUP_DATA_TYPE_AVI == m_pBackupInitialInfo->dataType)
		{
			sprintf(szCmd, "cp /mnt/mtd/WebServer/h264codec.exe %s", m_desDataPath);			
			DVRSystem(szCmd);
		}

		bool bNeedSleep = false;
		unsigned long writeFrameCount = 0;
		while (m_bBackuping)
		{
			readerRet = m_backupReader.GetFrame(frameInfo);
			if (CBackupReader::BACKUP_READER_ERROR_SUC != readerRet)
			{
				break;
			}

			if (m_pBackupInitialInfo->bCombined)
			{
				writerRet = m_pBackupCutWriter->WriteFrame(frameInfo);
				if (CBackupCutWriter::BACKUP_CUT_WRITER_ERROR_SUC != writerRet)
				{
					break;
				}
			}
			else
			{
				writerRet = m_pBackupWriter->WriteFrame(frameInfo);
				if (CBackupWriter::BACKUP_WRITER_ERROR_SUC != writerRet)
				{
					break;
				}
			}

			//�������

			precent = m_backupReader.GetOffSetTime();
			if (precent > 100)
			{
				precent = prePrecent;
			}
			
			if (prePrecent != precent)
			{
				prePrecent = precent;

				m_pMsgCallBack(m_pCallBackObj, CALL_BACK_MODULE_BACKUP_MAN, CALL_BACK_MSG_PROGRESS, &precent, NULL);
			}
		}

		//���ݽ���
		m_bBackuping = false;
		m_backupReader.Stop();
		if (m_pBackupInitialInfo->bCombined)
		{
			if (m_pRecFileEventInfo != NULL)
			{
				m_pBackupCutWriter->WriteFileEventInfo();
				delete [] m_pRecFileEventInfo;
				m_pRecFileEventInfo = NULL;
				m_eventInfoNum = 0;
			}
			
			m_pBackupCutWriter->Stop();

			delete m_pBackupCutWriter;
			m_pBackupCutWriter = NULL;
		}
		else
		{
			m_pBackupWriter->Stop();
			delete m_pBackupWriter;
			m_pBackupWriter = NULL;
		}
		
	}


	if (!m_pBackupInitialInfo->bCombined)
	{
		CCreateHtmlLog::Instance()->CreateHtmlAction();
		CCreateHtmlLog::Instance()->Quit();		
	}	



	if (NULL == m_pBackupInitialInfo->pMountDir)
	{
		char szTmp[128] = {0};
		sprintf(szTmp, "umount %s", USB_BACKUP_MOUNT);
		printf("%s\n", szTmp);
		DVRSystem(szTmp);

		sprintf(szTmp, "rm -rf %s", USB_BACKUP_MOUNT);
		printf("%s\n", szTmp);
		DVRSystem(szTmp);
	}

	if (NULL != frameInfo.pData)
	{
		delete [] frameInfo.pData;
		frameInfo.pData = NULL;
	}

	if (CBackupWriter::BACKUP_WRITER_ERROR_SUC != writerRet)
	{
		m_pMsgCallBack(m_pCallBackObj, CALL_BACK_MODULE_BACKUP_MAN, CALL_BACK_MSG_FAIL, NULL, NULL); //����ʧ��
	}
	else 
	{
		if (CBackupReader::BACKUP_READER_ERROR_READ_FINISH == readerRet)
		{
			m_pMsgCallBack(m_pCallBackObj,CALL_BACK_MODULE_BACKUP_MAN,  CALL_BACK_MSG_FINISH, NULL, NULL); //�������
		}
		else
		{
			m_pMsgCallBack(m_pCallBackObj, CALL_BACK_MODULE_BACKUP_MAN, CALL_BACK_MSG_FAIL, NULL, NULL); //����ʧ��
		}
	}

}













