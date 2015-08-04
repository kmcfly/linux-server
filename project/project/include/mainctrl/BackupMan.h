//���������ǣ�
// 1 ��ߵ���StartBackUp.��һЩ׼��������
// 2 ����Backup.���б��ݣ�ͬʱ���ûص������ѱ��ݽ��Ⱥͱ��ݽ���״̬������ߡ�
// 3 ���ݽ����󣬱���ģ����Լ����Լ�����stop�����������ڵõ����ݽ�����״̬�����Է���StopBackup��Ҳ���Բ�����

// ����ΪDVR��ʽ�����ݣ�û����ϸ���Ա��ݳ�����reclog�Ƿ�����á�
// û�в��Ա�������д���ļ���


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







