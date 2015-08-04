#ifndef _CDOPERATION_MAN_H_
#define _CDOPERATION_MAN_H_
#include "CScsi.h"
#include "PUB_common.h"
#include "dvrdvsdef.h"

typedef enum __cd_type__
{
	CD_TYPE_CDR         =0,     //CD-R
	CD_TYPE_CDRW        =1,		//CD-RW
	CD_TYPE_DVDR        =2,		//DVD-R
	CD_TYPE_DVDR_PLUSE  =3,		//DVD+R
	CD_TYPE_DVDRW       =4,		//DVD-RW
	CD_TYPE_DVDRW_PLUSE =5,		//DVD+RW
	CD_TYPE_DVDRAM      =6,		//DVD-RAM
	CD_TYPE_NODISK      =7,		//光驱中未放置碟片
	CD_TYPE_UNKNOWN     =8		//未定义类型
} CD_TYPE;

typedef enum __writedisk_type__
{
	WRITE_TYPE_FIRST,
	WRITE_TYPE_NORMAL,
	WRITE_TYPE_END
} WRITE_TYPE;

#define WRITECD					"mkisofs"			//create cd disk file mirror
#define CDFORMAT				"cdrecord"			//format and write cd
#define WRITEDVD				"growisofs"		    //write dvd
#define DVDFORMAT				"dvd+rw-format"	    //format dvd	

#define CDROM_INFO_PATH_NAME    "/proc/sys/dev/cdrom/info"
#define CDROM_MOUNT_DIR         "/backup"   
#define PROGRESSPATH			"/nfsdir/bin"
#define PROGRESSFILE            "/nfsdir/bin/progress"   
#define PROC_SCSI_SCSI			"/proc/scsi/scsi"

//CD光盘的容量
#define CD_DISK_SIZE        650  //MB

//DVD光盘的容量
#define DVD_DISK_SIZE       4403 //MB DVD盘上标记得4.7G（用的是1000）,实际只有4.3G






class CDOperationMan
{
public:

	//
	CDOperationMan();

	//
	~CDOperationMan();
	
	//采用单件模式
	static CDOperationMan*Instance( );
	

	//cmdPath是刻录用到的执行程序如，mkisofs growisofs等，存放的目录。
	bool Initial(const char *cmdPath);

	//
	void Quit();


	//
	int GetDeviceDiskInfo(DVR_DISK_INFO_LIST &cdInfoList);
	
	//
	int Format();				//对可写碟片格式化

	//
	int Write(char *pFileName, WRITE_TYPE mark = WRITE_TYPE_NORMAL);

	//
	void SetPercentZero();
	
	//
	long GetPercent();			//获得备份的进度

private:

	//获得设备的相关参数
	int GetDevicePara(unsigned int *pScsiNum, unsigned int *pCH, unsigned int *pID, unsigned int *pLun);

	//
	CD_TYPE GetCDType(char *cdPathName);


	//
private:	

	static CDOperationMan* m_pCdOperIns;  

	char m_cdPath[36];
	int  m_progressFile;
	long *m_pPrecent;

	char m_cmdPath[128];

};


#endif

















