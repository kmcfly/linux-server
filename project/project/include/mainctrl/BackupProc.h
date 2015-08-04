#ifndef _BACKUPPROC_H_
#define  _BACKUPPROC_H_
#include "ReclogManEx.h"
#include "dvrdvsdef.h"
#include "OneChnnReader.h"
#include "Product.h"
#include "MessageMan.h"
#include "NetProtocol.h"


#define BACKUP_STORE_DATA_PATH        "00"
#define BACKUP_STORE_VIDEOPLAY_PATH   "VideoPlay"


typedef struct _backup_initial_info
{
	FILE_INFO_EX_BACKUP *pFileInfoBackup;   //Ҫ���ݵ��ļ��б�
    unsigned long       fileNum;          //Ҫ���ݵ��ļ�����
	unsigned long       startTime;        //���ݵĿ�ʼʱ��
	unsigned long       endTime;          //���ݵĽ���ʱ��
	char                *pDiskName;       //����ʱʹ�õ��豸����
	char                *pMountDir;       //���ݳ�ʹ�õ��豸����·��
	DISK_TYPE           deviceType;       //����ʱʹ�õ��豸����
	BACKUP_DATA_TYPE    dataType;         //Ҫ���ݵ���������
	MESSAGE_CALLBACK_EX callBack;         //����ʱ��Ҫ�ص��ĺ���
	void                *pCallBackObj;    //
	CReclogManEX        *pReclogMan;      //����ϵͳ������
	unsigned char       channelNum;       //ϵͳ�ܹ�ͨ������
	char                *pSrcFilePath;     //ϵͳ���ݴ�ŵ�·��.�硢/mnt
	bool                bHaveVideoPlay;   //�Ƿ���Ҫ���ݲ�����
	char                *pSrcPlayerPath;  //���������ڵ�Ŀ¼
	bool                bCombined;
	unsigned long       diskOwnerby;      //���������ڱ��ػ���������    
}BACKUP_INITIAL_INFO;


class CBackupProc
{
public:
	//
	virtual int StartBackup(BACKUP_INITIAL_INFO *pBackupInfo) = 0;

	//
	//virtual int Backup(MESSAGE_CALLBACK pCallBack, void *pCallBackObj) = 0;
    virtual int Backup() = 0;
	
	//
	virtual void StopBackup() = 0;


	virtual ~CBackupProc(){;}

};


#endif








