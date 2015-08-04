#ifndef _CDBACKUPPROC_H_
#define _CDBACKUPPROC_H_

#include "BackupProc.h"
#include "ReclogEx.h"
#ifndef __ENVIRONMENT_WIN32__
#include "CDOperationMan.h"
#endif


class CDBackupProc : public CBackupProc
{
public:
	//
	CDBackupProc();

	//
	~CDBackupProc();

	//
	int StartBackup(BACKUP_INITIAL_INFO *pBackupInfo);

	//
	//int Backup(MESSAGE_CALLBACK pCallBack, void *pCallBackObj);
	int Backup();

	//
	void StopBackup();


private:
	//
	static RESULT WINAPI BackupProc(void *pParam);
	void BackupProc();

	//
	static RESULT WINAPI ProgressProc(void *pParam);
	void ProgressProc();
	
	//
	int LinkFile(char *pSrcFilePath, FILE_INFO_EX_BACKUP *pSrcFileInfoBackup, int srcFileNum, CReclogManEX *pSrcReclogMan, 
		         char *pDesPath, CReclogEx *pDesReclogEx, unsigned char videoInputNum);

	//
	
	BACKUP_INITIAL_INFO *m_pBackupInitialInfo;
	MESSAGE_CALLBACK_EX m_pMsgCallBack;
	void *m_pCallBackObj;

	//
	CReclogEx     m_reclogEx;
#ifndef __ENVIRONMENT_WIN32__
	CDOperationMan *m_pCdOperationMan;
#endif
	char m_desDataPath[128];
	char m_desVideoPlayPath[128];

	//
	bool m_bBackuping;
	PUB_thread_t m_hBackupThread;

	//
	bool m_bProgressing;
	PUB_thread_t m_hProgressThread;
};


#endif




