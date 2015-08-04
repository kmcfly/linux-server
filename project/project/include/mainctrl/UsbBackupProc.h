#ifndef _USB_BACKUP_PROC_H_
#define _USB_BACKUP_PROC_H_
#include "BackupProc.h"

#include "BackupReader.h"
#include "BackupWriter.h"
#include "BackupCutWrite.h"
#include <string>
#include <vector>
using namespace std;

#define USB_BACKUP_MOUNT  "/mnt/usbbk"

class CUsbBackupProc : public CBackupProc
{
public:
	//
	CUsbBackupProc();

	//
	~CUsbBackupProc();

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
	int ProcWriteFrame(FRAME_INFO_EX & frameInfo);

	//
	int ProcWriteFrame_Dvr(FRAME_INFO_EX & frameInfo);

	//
	int ProcWriteFrame_Avi(FRAME_INFO_EX & frameInfo);
	
    //
	int ProcWriteReclog(unsigned short srcDiskIndex, unsigned short srcFileIndex, char *pDesReclog);
	
	//
	BACKUP_INITIAL_INFO *m_pBackupInitialInfo;

	
	//
	bool m_bBackuping;
	PUB_thread_t m_hBackupThread;

	
	//
	CBackupReader  m_backupReader;

	//
	/*
	unsigned long m_srcFileIndex;
	unsigned long m_srcFrameCount;
	COneChnnReader m_reader;
	*/

	
	//
	CBackupWriter  * m_pBackupWriter;

	CBackupCutWriter * m_pBackupCutWriter;
	
	//
	/*
	CReclogEx m_desWriteReclog;
	unsigned long m_desFileIndex;	
	CAVIWriter m_stdAviWriter;
	CRecWriter m_dvrWriter;
	char *m_pBackupPathName;
	unsigned long m_backupFirstFrameTime;
	unsigned long m_backupLastFrameTime;
    */

	//
	MESSAGE_CALLBACK_EX m_pMsgCallBack;
	void *m_pCallBackObj;

	//
	char m_desDataPath[128];

	char m_desVideoPlayPath[128];

	REC_FILE_EVENT_INFO *m_pRecFileEventInfo;
	unsigned long        m_eventInfoNum;

};


#endif



