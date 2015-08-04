

#include "BackupMan.h"


//
CBackupMan::CBackupMan()
{
	m_pBackupInitialInfo = NULL;
	m_pBackupProc = NULL;
}

//
CBackupMan::~CBackupMan()
{
	Quit();

	if (NULL != m_pBackupProc)
	{
		delete m_pBackupProc;
		m_pBackupProc = NULL;
	}
}

//
bool CBackupMan::Initail()
{
	m_pBackupInitialInfo = NULL;
	m_pBackupInitialInfo = new BACKUP_INITIAL_INFO;

	return true;
}

//
void CBackupMan::Quit()
{
  if (NULL != m_pBackupInitialInfo)
  {
	  delete m_pBackupInitialInfo;
	  m_pBackupInitialInfo = NULL;
  }
}

//
int CBackupMan::StartBackup(BACKUP_INITIAL_INFO BackupInitialInfo)
{
	assert(NULL == m_pBackupProc);
	assert(BackupInitialInfo.pFileInfoBackup != NULL);
	assert(BackupInitialInfo.startTime < BackupInitialInfo.endTime);
	assert(BackupInitialInfo.fileNum > 0);
	
	int ret = 0;

	if (NULL == m_pBackupInitialInfo)
	{
		return -1;
	}

	if (BackupInitialInfo.bCombined)
	{
		if (NET_DISK_TYPE_CDDVD == BackupInitialInfo.deviceType)
		{
			return -1;
		}
	}
	
	memset(m_pBackupInitialInfo, 0, sizeof(BACKUP_INITIAL_INFO));
	memcpy(m_pBackupInitialInfo, &BackupInitialInfo, sizeof(BACKUP_INITIAL_INFO));

	if (NULL != m_pBackupProc)
	{
		delete m_pBackupProc;
		m_pBackupProc = NULL;
	}
#ifdef __ENVIRONMENT_LINUX__
	if (NET_DISK_TYPE_REMOVEABLE == m_pBackupInitialInfo->deviceType)
	{
		m_pBackupProc = new CUsbBackupProc;
	}
	else if (NET_DISK_TYPE_CDDVD == m_pBackupInitialInfo->deviceType)
	{
		m_pBackupProc = new CDBackupProc;
	}
#endif

	

	ret = m_pBackupProc->StartBackup(m_pBackupInitialInfo);
	if (0 != ret)
	{
		delete m_pBackupProc;
		m_pBackupProc = NULL;
	}

	return ret;

}

//
int CBackupMan::Backup()
{
	if (NULL == m_pBackupProc)
	{
		return -1;
	}

	//return m_pBackupProc->Backup(ProBackupCallBackMsg, this);
	return m_pBackupProc->Backup();
}


void CBackupMan::StopBackup()
{
	if (NULL == m_pBackupProc)
	{
		return ;
	}
	
	m_pBackupProc->StopBackup();

	delete m_pBackupProc;
	m_pBackupProc = NULL;

}









