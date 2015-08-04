//备份流程是：
// 1 外边调用StartBackUp.做一些准备工作。
// 2 调用Backup.进行备份，同时利用回调函数把备份进度和备份结束状态发给外边。
// 3 备份结束后，备份模块会自己向自己发送stop命令。所以外边在得到备份结束的状态，可以发送StopBackup，也可以不发。

// 备份为DVR方式的数据，没有详细测试备份出来的reclog是否可以用。
// 没有测试备份正在写的文件。


#ifndef _BACKUPMAN_H_
#define _BACKUPMAN_H_

#include "mylist.cpp"
#ifdef __ENVIRONMENT_LINUX__
#include "UsbBackupProc.h"
#include "CDBackupProc.h"
#else
#include "BackupProc.h"
#endif




class CBackupMan
{
public:
	//
	CBackupMan();

	//
	~CBackupMan();

	//
	bool Initail();

	//
	void Quit();

	//
	int StartBackup(BACKUP_INITIAL_INFO BackupInitialInfo);
	
	//
	int Backup();

	//
	void StopBackup();

	//
	//static int ProBackupCallBackMsg(void* pObject, int iMsg, void *pParam1, void *pParam2);

protected:
private:	
	//
	CBackupProc  *m_pBackupProc;
	//
	BACKUP_INITIAL_INFO *m_pBackupInitialInfo;


};


#endif







