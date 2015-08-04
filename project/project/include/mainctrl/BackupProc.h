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
	FILE_INFO_EX_BACKUP *pFileInfoBackup;   //要备份的文件列表
    unsigned long       fileNum;          //要备份的文件个数
	unsigned long       startTime;        //备份的开始时间
	unsigned long       endTime;          //备份的结束时间
	char                *pDiskName;       //备份时使用的设备名称
	char                *pMountDir;       //备份出使用的设备挂载路径
	DISK_TYPE           deviceType;       //备份时使用的设备类型
	BACKUP_DATA_TYPE    dataType;         //要备份的数据类型
	MESSAGE_CALLBACK_EX callBack;         //备份时需要回调的函数
	void                *pCallBackObj;    //
	CReclogManEX        *pReclogMan;      //管理系统的数据
	unsigned char       channelNum;       //系统总共通道数。
	char                *pSrcFilePath;     //系统数据存放的路径.如、/mnt
	bool                bHaveVideoPlay;   //是否需要备份播放器
	char                *pSrcPlayerPath;  //播放器所在的目录
	bool                bCombined;
	unsigned long       diskOwnerby;      //磁盘是属于本地还是其他的    
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








