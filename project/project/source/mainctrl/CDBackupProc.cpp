#include "CDBackupProc.h"

#include "CreateHtmlLog.h"




#ifdef __ENVIRONMENT_WIN32__
#include "direct.h"
#endif

#ifdef __ENVIRONMENT_WIN32__

// 判断目录(不能用来查询根目录,否则总是失败.)是否存在的函数
static BOOL IsPathExist(LPCTSTR lpszPath)
{
	char szTemp[MAX_PATH];
	lstrcpy(szTemp, lpszPath);

	BOOL bExist; // 最后结果，表示目录是否存在
	if(szTemp[strlen(lpszPath)-1]=='\\')
	{
		szTemp[strlen(lpszPath)-1]='\0';    // 先删除最后的“\”
	}
	WIN32_FIND_DATA wfd;                                  // 查找
	HANDLE hFind = FindFirstFile(szTemp, &wfd);                 
	if(hFind == INVALID_HANDLE_VALUE)
	{
		bExist = FALSE;            // 没有找到配备，目录肯定不存在
	}
	else
	{
		if(wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) // 检查找到的结果是否目录
			bExist = TRUE;     // 是目录,目录存在
		else
			bExist = FALSE;       // 是文件,目录不存在

		FindClose(hFind);
	}

	// debug
	printf("执行IsPathExist()完毕!\n");

	return bExist;
}

static BOOL CreateDirectoryEx(LPCTSTR lpszCreatePath)
{
	char szCreatePathTemp[MAX_PATH];
	lstrcpy(szCreatePathTemp, lpszCreatePath);
	if(szCreatePathTemp[strlen(szCreatePathTemp)-1]=='/')
	{
		szCreatePathTemp[strlen(szCreatePathTemp)-1]='\0';    // 先删除最后的“\”
	}

	char szTemp[MAX_PATH] = ""; // 临时缓存
	// 查找路径分隔符号'\'
	char *p = strchr(szCreatePathTemp, '/'); // 第一次的出现位置(地址)
	if (p != NULL)
	{
		p = strchr(p+1, '/');   // 第二次位置, 如果是NULL 则说明是一级目录, 如: c:\path 这样如果该目录不存在会直接创建该目录
	}   

	while (p != NULL)            // 之后的位置(要能进入此循环, 至少是二级目录, 如: c:\一级目录\二级目录\...)
	{
		strncpy(szTemp, szCreatePathTemp, p-szCreatePathTemp);     // 如: e:\1

		if (!IsPathExist(szTemp)) // 如果不存在, 则一路创建到倒数第二个目录
		{
			// 测试
			if (!CreateDirectory(szTemp, NULL))
			{
				printf("创建目录%s失败!错误代码: %d\n", szTemp, GetLastError());
				return FALSE;
			}
			printf("已成功创建目录:%s\n", szTemp); // 创建该层目录, 之后一路往下创建不用判断

			p = strchr(p+1, '/');   // 下一级子目录
			while( p != NULL )
			{
				strncpy(szTemp, szCreatePathTemp, p-szCreatePathTemp); // 如: e:\1\2

				// 测试
				if (!CreateDirectory(szTemp, NULL))
				{
					printf("创建目录%s失败!错误代码: %d\n", szTemp, GetLastError());
					return FALSE;
				}
				printf("已成功创建目录:%s\n", szTemp);

				p = strchr(p+1, '/');
			}

		}

		if ( p != NULL ) // 防止上面内层的while
		{
			p = strchr(p+1, '/');   // 下一级子目录
		}
	}

	if (!IsPathExist(lpszCreatePath)) // 如果目录不存在, 则创建目录.(也是创建最后一个目录)
	{
		// 测试
		if (!CreateDirectory(lpszCreatePath, NULL))
		{
			printf("创建目录%s失败!错误代码: %d\n", lpszCreatePath, GetLastError());
			return FALSE;
		}
		printf("已成功创建目录:%s\n", lpszCreatePath);
	}

	return TRUE;
}

#endif


CDBackupProc::CDBackupProc()
{
	m_pBackupInitialInfo = NULL;

	m_hBackupThread = PUB_THREAD_ID_NOINIT;
	m_bBackuping    = false;

#ifndef __ENVIRONMENT_WIN32__
	m_pCdOperationMan = CDOperationMan::Instance();
#endif

	m_hProgressThread = PUB_THREAD_ID_NOINIT;
	m_bProgressing = false;

}

//
CDBackupProc::~CDBackupProc()
{


}

//
int CDBackupProc::StartBackup(BACKUP_INITIAL_INFO *pBackupInfo)
{
	assert(pBackupInfo != NULL);
	assert(NET_BACKUP_DATA_TYPE_DVR == pBackupInfo->dataType);
	assert(NULL != pBackupInfo->pSrcFilePath);
    assert(NULL != pBackupInfo->pMountDir);

	m_pBackupInitialInfo = pBackupInfo;

#ifndef __ENVIRONMENT_WIN32__
	char szTmp[128] = {0};
	sprintf(szTmp, "rm -rf %s/*", m_pBackupInitialInfo->pMountDir);
	printf("szTmp:%s\n", szTmp);
	DVRSystem(szTmp);

	tm time = DVRTime32ToTm(GetCurrTime32());
	sprintf(m_desDataPath, "%s/%d%02d%02d%02d%02d%02d",m_pBackupInitialInfo->pMountDir, time.tm_year+1900, time.tm_mon+1, time.tm_mday, time.tm_hour, time.tm_min, time.tm_sec); 	
	sprintf(szTmp, "mkdir -p %s", m_desDataPath);
	printf("szTmp:%s\n", szTmp);
	if (0 != DVRSystem(szTmp))
	{
		return -1;
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

	m_pCdOperationMan->SetPercentZero();
	m_pMsgCallBack = m_pBackupInitialInfo->callBack;
	m_pCallBackObj = m_pBackupInitialInfo->pCallBackObj;
#else
	assert(NULL != m_pBackupInitialInfo->pMountDir);
	tm time = DVRTime32ToTm(GetCurrTime32());
	sprintf(m_desDataPath, "%s/%d%02d%02d%02d%02d%02d", m_pBackupInitialInfo->pMountDir, time.tm_year+1900, time.tm_mon+1, time.tm_mday, time.tm_hour, time.tm_min, time.tm_sec);
	if (!CreateDirectoryEx(m_desDataPath))
	{
		return -1;
	}
	m_pMsgCallBack = m_pBackupInitialInfo->callBack;
	m_pCallBackObj = m_pBackupInitialInfo->pCallBackObj;
#endif

	return 0;
}


int CDBackupProc::Backup()
{

	if (PUB_THREAD_ID_NOINIT != m_hBackupThread)
	{
		PUB_ExitThread(&m_hBackupThread, &m_bBackuping);
	}


	if (PUB_THREAD_ID_NOINIT != m_hProgressThread)
	{
		PUB_ExitThread(&m_hProgressThread, &m_bProgressing);
	}


	m_hBackupThread = PUB_CreateThread(BackupProc, this, &m_bBackuping);
	if (PUB_THREAD_ID_NOINIT == m_hBackupThread)
	{
		return -1;
	}

	printf("start m_hBackupThread ok \n");

	m_hProgressThread = PUB_CreateThread(ProgressProc, this, &m_bProgressing);
	if (PUB_THREAD_ID_NOINIT == m_hProgressThread)
	{
		PUB_ExitThread(&m_hBackupThread, &m_bBackuping);
		return -1;
	}

	printf("start m_hProgressThread ok \n");

	return 0;
}

RESULT WINAPI CDBackupProc::BackupProc(void *pParam)
{
#ifdef __ENVIRONMENT_LINUX__
	pid_t pid = getpid();
	pid_t tid = syscall(__NR_gettid);
	printf("%s, %d, pid = %d, tid=%d\n", __FILE__, __LINE__, pid, tid);
#endif

	CDBackupProc *pThis = (CDBackupProc *)pParam;
	pThis->BackupProc();

	return 0;
}


RESULT WINAPI CDBackupProc::ProgressProc(void *pParam)
{
#ifdef __ENVIRONMENT_LINUX__
	pid_t pid = getpid();
	pid_t tid = syscall(__NR_gettid);
	printf("%s, %d, pid = %d, tid=%d\n", __FILE__, __LINE__, pid, tid);
#endif

	CDBackupProc *pThis = (CDBackupProc *)pParam;
	pThis->ProgressProc();
	return 0;
}



void CDBackupProc::StopBackup()
{
	if (PUB_THREAD_ID_NOINIT != m_hBackupThread)
	{
		PUB_ExitThread(&m_hBackupThread, &m_bBackuping);
	}

	printf("stop m_hBackupThread ok \n");

	if (PUB_THREAD_ID_NOINIT != m_hProgressThread)
	{
		PUB_ExitThread(&m_hProgressThread, &m_bProgressing);
	}

	printf("stop m_hProgressThread ok \n");


}


void CDBackupProc::BackupProc()
{
#ifndef __ENVIRONMENT_WIN32__
	char szCmd[128] = {0};

	if (0 != LinkFile(m_pBackupInitialInfo->pSrcFilePath, m_pBackupInitialInfo->pFileInfoBackup, m_pBackupInitialInfo->fileNum, m_pBackupInitialInfo->pReclogMan, 
		              m_desDataPath, &m_reclogEx, m_pBackupInitialInfo->channelNum))
	{
		sprintf(szCmd, "rm -rf %s/* ", m_pBackupInitialInfo->pMountDir);
		DVRSystem(szCmd);
		m_pMsgCallBack(m_pCallBackObj, CALL_BACK_MODULE_BACKUP_MAN, CALL_BACK_MSG_FAIL, NULL, NULL);//备份失败
		return;
	}

	if (m_pBackupInitialInfo->bHaveVideoPlay)
	{
		sprintf(szCmd, "ln -s %s/* %s/", m_pBackupInitialInfo->pSrcPlayerPath, m_desVideoPlayPath); 
		if (0 != DVRSystem(szCmd))
		{
			sprintf(szCmd, "rm -rf %s/* ", m_pBackupInitialInfo->pMountDir);
			DVRSystem(szCmd);
			m_pMsgCallBack(m_pCallBackObj, CALL_BACK_MODULE_BACKUP_MAN, CALL_BACK_MSG_FAIL, NULL, NULL);//备份失败
			return;
		}
	}

	char *pszHtmlSrcPath = NULL;

	if (!m_pBackupInitialInfo->bCombined)
	{
		pszHtmlSrcPath = new char [128];
		if (NULL == pszHtmlSrcPath)
		{
			sprintf(szCmd, "rm -rf %s/* ", m_pBackupInitialInfo->pMountDir);
			DVRSystem(szCmd);
			m_pMsgCallBack(m_pCallBackObj, CALL_BACK_MODULE_BACKUP_MAN, CALL_BACK_MSG_FAIL, NULL, NULL);//备份失败
			return;
		}
		memset(pszHtmlSrcPath,0,128);
		snprintf(pszHtmlSrcPath,128,"%s/%02d/HtmlData",m_pBackupInitialInfo->pSrcFilePath,m_pBackupInitialInfo->pFileInfoBackup[0].fileInfo.diskIndex);
		snprintf(szCmd, 128, "rm -rf %s/*", pszHtmlSrcPath);
		DVRSystem(szCmd);

		snprintf(szCmd,128,"mkdir -p %s",pszHtmlSrcPath);
		if (0 != DVRSystem(szCmd))
		{
			if (NULL != pszHtmlSrcPath)
			{
				delete [] pszHtmlSrcPath;
				pszHtmlSrcPath = NULL;
			}
			sprintf(szCmd, "rm -rf %s/* ", m_pBackupInitialInfo->pMountDir);
			DVRSystem(szCmd);
			m_pMsgCallBack(m_pCallBackObj, CALL_BACK_MODULE_BACKUP_MAN, CALL_BACK_MSG_FAIL, NULL, NULL);//备份失败
			return;
		}
		CCreateHtmlLog::Instance()->Initial(true,m_pBackupInitialInfo,pszHtmlSrcPath);
		CCreateHtmlLog::Instance()->SetLinkPath(m_desDataPath);
		CCreateHtmlLog::Instance()->WirteHtmlLogAction();
		CCreateHtmlLog::Instance()->CreateHtmlAction();
		CCreateHtmlLog::Instance()->Quit();		
	}


	
    bool bSucc = false;
	int reCount = 0;
	while (m_bBackuping)
	{
		if (0 != m_pCdOperationMan->Format())
		{
			bSucc = false;
			break;
		}

		PUB_Sleep(10000);
		for (reCount = 0; reCount < 3; reCount++)
		{
			if (0 == m_pCdOperationMan->Write(m_pBackupInitialInfo->pMountDir))
			{
				bSucc = true;
				break;
			}
			PUB_Sleep(10000);
		}

		if (3 == reCount)
		{
			bSucc = false;
			break;
		}

		m_bBackuping = false;
	}

	sprintf(szCmd, "rm -rf %s/* ", m_pBackupInitialInfo->pMountDir);
	DVRSystem(szCmd);

	if (pszHtmlSrcPath != NULL)
	{
		snprintf(szCmd, 128, "rm -rf %s/*", pszHtmlSrcPath);
		DVRSystem(szCmd);
	}
	
	if (bSucc)
	{
		m_pMsgCallBack(m_pCallBackObj, CALL_BACK_MODULE_BACKUP_MAN, CALL_BACK_MSG_FINISH, NULL, NULL); //备份完成			
	}
	else
	{
		m_pMsgCallBack(m_pCallBackObj, CALL_BACK_MODULE_BACKUP_MAN, CALL_BACK_MSG_FAIL, NULL, NULL); //备份完成			
	}

#else
	
	LinkFile(m_pBackupInitialInfo->pSrcFilePath, m_pBackupInitialInfo->pFileInfoBackup, m_pBackupInitialInfo->fileNum, m_pBackupInitialInfo->pReclogMan, 
		     m_desDataPath, &m_reclogEx, m_pBackupInitialInfo->channelNum);


	m_pMsgCallBack(m_pCallBackObj, CALL_BACK_MODULE_BACKUP_MAN, CALL_BACK_MSG_FINISH, NULL, NULL); //备份完成			
	
	
#endif

	return;
}


void CDBackupProc::ProgressProc()
{
#ifndef __ENVIRONMENT_WIN32__
	while (m_bProgressing)
	{
		if (m_bBackuping)
		{
			long precent = m_pCdOperationMan->GetPercent();
			CMessageMan::Instance()->RefreshBackupProgress(precent);
		}		

		PUB_Sleep(10000);
	}
#else
	return;   
#endif
}

int CDBackupProc::LinkFile(char *pSrcFilePath, FILE_INFO_EX_BACKUP *pSrcFileInfoBackup, int srcFileNum, CReclogManEX *pSrcReclogMan, 
						   char *pDesPath, CReclogEx *pDesReclogEx, unsigned char videoInputNum)
{
	char szSrcFilePathName[255] = {0};
	char szDesFilePathName[255] = {0};
	char szCmd[255] = {0};
	FILE_FILE_INFO fileHeadInfo;
	SECTION_INFO_EX *pFileSectionInfo = new SECTION_INFO_EX [MAX_SECTION_NUM * sizeof(SECTION_INFO_EX)];
	FILE_RECLOG_INFO *pFileReclogInfo = new FILE_RECLOG_INFO [MAX_RECLOG_NUM * sizeof(FILE_RECLOG_INFO)];
	int desFileCount = 0;

    
	sprintf(szDesFilePathName, "%s/reclog.bin", pDesPath);
	if (!pDesReclogEx->CreateLogFile(szDesFilePathName, srcFileNum, videoInputNum,true))
	{
		delete [] pFileSectionInfo;
		delete [] pFileReclogInfo;
		pFileSectionInfo = NULL;
		pFileReclogInfo  = NULL;		
		return -1;
	}

	if (!pDesReclogEx->OpenLogFileForBackup(szDesFilePathName))
	{
		delete [] pFileSectionInfo;
		delete [] pFileReclogInfo;
		pFileSectionInfo = NULL;
		pFileReclogInfo  = NULL;
		return -1;
	}

	int bRet = 0;
	do 
	{
		for (int i = 0, desFileCount = 0; i < srcFileNum; ++i, ++desFileCount)
		{		
			sprintf(szSrcFilePathName, "%s/%02d/%08d.dat", pSrcFilePath, pSrcFileInfoBackup[i].fileInfo.diskIndex, pSrcFileInfoBackup[i].fileInfo.fileIndex);
			sprintf(szDesFilePathName, "%s/%08d.dat", pDesPath, desFileCount);
#ifndef __ENVIRONMENT_WIN32__
			sprintf(szCmd, "ln -s %s %s", szSrcFilePathName, szDesFilePathName);
			if (0 != DVRSystem(szCmd))
			{
				bRet = -1;
				break;
			}
#else
			CopyFileA(szSrcFilePathName, szDesFilePathName, false);
#endif
			memset(&fileHeadInfo, 0, sizeof(FILE_FILE_INFO));
			memset(pFileSectionInfo, 0, MAX_SECTION_NUM * sizeof(SECTION_INFO_EX));
			memset(pFileReclogInfo, 0, MAX_RECLOG_NUM * sizeof(FILE_RECLOG_INFO));
			if (!pSrcReclogMan->GetOneFileAllInfo(pSrcFileInfoBackup[i].fileInfo.diskIndex, pSrcFileInfoBackup[i].fileInfo.fileIndex, (unsigned char *)(&fileHeadInfo), (unsigned char *)pFileSectionInfo, (unsigned char *)pFileReclogInfo))
			{
				bRet = -1;
				break;
			}

			if (!pDesReclogEx->RepairForBackup((unsigned char *)(&fileHeadInfo),(unsigned char *)pFileSectionInfo, (unsigned char *)pFileReclogInfo, pSrcFileInfoBackup[i].fileInfo))
			{
				bRet = -1;
				break;
			}

			if (!pDesReclogEx->WriteOneFileAllInfo(desFileCount, (unsigned char *)(&fileHeadInfo), (unsigned char *)pFileSectionInfo, (unsigned char *)pFileReclogInfo))
			{
				bRet = -1;
				break;
			}
		}
	}while (0);
	
	pDesReclogEx->CloseLogFileForBackup();

	if (pFileSectionInfo != NULL)
	{
		delete [] pFileSectionInfo;
		pFileSectionInfo = NULL;
	}

	if (pFileReclogInfo != NULL)
	{
		delete [] pFileReclogInfo;
		pFileReclogInfo = NULL;
	}

	return bRet;			
}




