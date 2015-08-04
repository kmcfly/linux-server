
/***********************************************************************
** Copyright   Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : 
** Date         : 2007-09-14
** Name         : diskmanager.h
** Version      : 1.0
** Description  :  对于磁盘模块接口使用的规范说明：
                  1 固定硬盘使用说明：
                    1） 调用Initial函数，就可以获取并且挂载所有的固定录像硬盘。
				    2） 可以通过下面的接口 GetDeviceInfo， GetMountedPartionNum，FormatDisk，对固定硬盘访问
                  2 可以热拔插设备使用说明：
				    1）可以通过下面的接口 GetDeviceInfo，GetDevicePartionInfo，获取正在使用或未使用设备的详细信息
					2）通过函数Mount来使用这个设备					
					3）使用完后，调用Unmount释放这个设备
				  3 注意：目前支持对多个热拔插设备进行操作。
                  4 对于冗余硬盘的说明：
** Modify Record:
1:
***********************************************************************/
#ifndef _DISKMANANGER_H_
#define _DISKMANANGER_H_
#include "Disk_Config.h"
#if defined (__TDFH__) || defined (__TDNXP__) || defined (__CHIP3531__) || defined(__CHIP3535__) || defined (__CHIP3521__) || defined(__CHIP3520A__) || defined(__CHIPTI__) || defined(__CHIP3520D__) || defined(__CHIPGM__) 
#include "DiskSearchEx.h"
#else
#include "SearchDisk.h"
#endif

#include "ReclogEx.h"
#include "Log.h"
#include "SnapMan.h"



class CDiskManager  
{
public: 
	//构造函数
	CDiskManager();

	//析构函数
	~CDiskManager();

	//创建磁盘管理的实例，采用单件模式
	static CDiskManager*Instance( );

	//初始化函数
	bool Initial(const DISK_INIT_INFO *pDiskInitInfo);

	////搜索移动硬盘，u盘，返回结果代表的个数。 设备的信息只使用了diskType， diskname,totalSpace这几个域。
	//int SearchRemovableDisk(DISK_TYPE nDiskType, DVR_DISK_INFO_LIST *pDiskInfoList);

	//挂载设备。
	int Mount(DISK_TYPE nDiskType, DISK_INTERFACE_TYPE nDiskInterface, char *diskname, unsigned long sizeM);

	//获取某种类型的设备整体信息，如果diskName为空，则获取所有的已经挂载和为挂载的设备
	int GetDeviceDiskInfo(DISK_TYPE nDiskType, char *pDiskName, DVR_DISK_INFO_LIST &diskInfoList);

	//
	void GetOneRWDiskPartionNum(unsigned char &startMountIndex, unsigned char &partNum, char *pDevName = NULL);

	//获取某种类型的设备分区信息，如果diskName为空，则获取所有的已经挂载和为挂载的设备的分区
	int GetDevicePartionInfo(DISK_TYPE nDiskType, char *pDiskName, DVR_DISK_INFO_LIST &partionInfoList);

	//获取挂载成功的分区数
	//int GetMountedPartionNum(DISK_TYPE nDiskType);	

	//格式化硬盘
	int FormatDisk(DISK_TYPE nDiskType, char *diskname, MESSAGE_CALLBACK_EX PercentCallBack, void *pPara);

	//改变硬盘的属性
	int ChangeDiskProperty(DISK_TYPE nDiskType, char *pDiskName, unsigned char newDiskProperty);

	//得到硬盘隶属于哪些DVR。
    int GetDiskOwner(DISK_TYPE nDiskType, DVR_DISK_OWNER_LIST &diskOwnerList);
	
	//
	int GetOwnerInfo(BYTE diskDvrIndex, DVR_DISK_OWNER &diskOwnerInfo);

	//卸载
	int UnMount(DISK_TYPE nDiskType, char *diskname);

	//
	void ProcessDiskSleep(ULONGLONG heightMask, ULONGLONG lowMask);

	void Quit();

	//
	bool ProcessDiskHealth(DISK_HEALTH_INFO_LIST &warningList);

	//
	bool GetDiskSmartInfo(char *pDiskName, NET_DISK_SMART_INFO &stInfo);
	
	
private:
//////////////////////////////////////////////////////////////////////////
//////下面是对固定硬盘的操作
	//
	int GetFixedDiskInfo(char *diskname,  PART_INFO *pPartitionInfo, BYTE *pPartitionNum,bool bBigDiskFlag);

	//
	int MountFixedDisk(DISK_INFO &diskInfo, int &startPartId, bool bCreateLogFile = false, unsigned long lAviNum = 0, unsigned long lEventLogSizeM = 0, unsigned long lOperLogSizeM = 0);

	//
	int FormatFixedDisk(char *diskname, MESSAGE_CALLBACK_EX PercentCallBack, void *pPara);

	//
	int UnMountFixedDisk(DISK_INFO &DiskInfo);

//////////////////////////////////////////////////////////////////////////
//////下面是对热拔插硬盘的操作
	//
	int GetRemovableDiskInfo(char *diskname,  PART_INFO *pPartitionInfo, BYTE *pPartitionNum, unsigned long sizeM);
	
	//
	int FindRemovealbeDiskMountDir(int *pMountDir, int partitionNum);

	//
	int MountRemovableDisk(DISK_INFO &diskInfo, int &startPartId);

	//
	int FormatRemovableDisk(char *diskname, MESSAGE_CALLBACK_EX PercentCallBack, void *pPara);

    //
    int UnMountRemovableDisk(DISK_INFO &DiskInfo);
//////////////////////////////////////////////////////////////////////////////	

//////////////////////////////////////////////////////////////////////////////
////下面这部分是关于硬盘，U盘一些通用操作
	//
	int FormatDiskImp(char *diskName, unsigned char partitionNum, unsigned long fileSizeM, unsigned long &lAviNum,
		              unsigned long &lEventLogSizeM,unsigned long &lOperLogSizeM, MESSAGE_CALLBACK_EX PercentCallBack, void *pPara,bool bBigDiskCheck);

#if defined(__VERY_BIG_DISK__)
	void ReverseBytes( void* theValue, int numBytes );
	int GptDiskPartitionProc(char *diskName, unsigned char partitionNum );
	
	int NormalDiskPartitionProc(char *diskName, unsigned char partitionNum , unsigned char nSys);

	int IsLittleEndian(void);	
	
	 //对硬盘分区
	 int SetPartitionInfo(char *diskName, unsigned char partitionNum, unsigned char nSys, 
		                 MESSAGE_CALLBACK_EX PercentCallBack, void *pPara, bool pBigDiskFlag);

	bool FormatParttion(int &fd, unsigned long filesize, unsigned long &lAviNum,
										unsigned long &eventlogSizeM, unsigned long &operlogSizeM, bool bigDiskFlag);

	int GetGptParttionInfo( char *diskname, PART_INFO *pPartitionInfo, BYTE *pPartitionNum );

	bool GetDBRFilesystemInfo(char *pDiskname,unsigned char &partformat);
#else
	//对硬盘分区
	int SetPartitionInfo(char *diskName, unsigned char partitionNum, unsigned char nSys, 
		                 MESSAGE_CALLBACK_EX PercentCallBack, void *pPara);

	//
	bool FormatParttion(int &fd, unsigned long filesize, unsigned long &lAviNum,unsigned long &eventlogSizeM, unsigned long &operlogSizeM);
#endif
	//读取硬盘分区信息
    int GetParttionInfo(char *diskname,  PART_INFO *pPartitionInfo, BYTE *pPartitionNum);

	//得到分区剩余空间
	int GetDiskUseSpace(DISK_INFO &diskInfo);

	//创建节点和挂载目录
	int MakeNode(char *pDiskname, char *pMountDir, unsigned char startMountDirId, unsigned char partitionNum);


	
	//判断引导扇区是否包含结束标志
	inline int Valid_Part_Table_Flag(unsigned char *b) 
	{
		return (b[BOOT_LENGTH - 2] == 0x55 && b[BOOT_LENGTH - 1] == 0xaa);
	}

	//得到反字节序数（占四个字节）的十进制值
	inline ULONG Read4_Little_Endian(unsigned char *cp)
	{
		return  (ULONG)(cp[0])
			   +((ULONG)(cp[1]) << 8)
			   +((ULONG)(cp[2]) << 16) 
			   + ((ULONG)(cp[3]) << 24);
	}

#if defined(__VERY_BIG_DISK__)
	//得到反字节序数（占八个字节）的值
	inline ULONGLONG Read8_Little_Endian(unsigned char *cp)
	{
		return  	( ULONGLONG) (cp[0] )
			   		+( (ULONGLONG) (cp[1]) << 8 )
			   		+( (ULONGLONG) (cp[2]) << 16 ) 
			   		+( (ULONGLONG) (cp[3]) << 24 )
			   		+( (ULONGLONG) (cp[4]) << 32 ) 
			   		+( (ULONGLONG) (cp[5]) << 40 ) 
			   		+( (ULONGLONG) (cp[6]) << 48 ) 
			   		+( (ULONGLONG) (cp[7]) << 56 );
	}
	
	inline ULONGLONG Read3_Little_Endian(unsigned char *cp)
	{
		return  	( ULONGLONG) (cp[0] )
			   		+( (ULONGLONG) (cp[1]) << 8 )
			   		+( (ULONGLONG) (cp[2]) << 16 );
	}	
#endif
	
	//从分区表的一个分区项中得到分区的起始扇区
	inline ULONG Get_Start_Sect(struct partition *p) 
	{
		return Read4_Little_Endian(p->start4);
	}
	
	//从分区表的一个分区项中得到分区的总扇区数
	inline ULONG Get_Nr_Sects(struct partition *p) 
	{
		return Read4_Little_Endian(p->size4);
	}

	//判断分区表的一个分区项是为全零
	//如果全为零，则返回1，否则，返回0；
	inline int Is_Cleared_Partition(struct partition *p) 
	{
		return !(!p || p->boot_ind || p->head || p->sector || p->cyl ||
			p->sys_ind || p->end_head || p->end_sector || p->end_cyl ||
			Get_Start_Sect(p) || Get_Start_Sect(p));
	}
    
	void Clear_Partition(struct partition *p)
	{
		p->boot_ind = 0;
		p->head = 0;
		p->sector = 0;
		p->cyl = 0;
		p->sys_ind = 0;
		p->end_head = 0;
		p->end_sector = 0;
		p->end_cyl = 0;
		Set_Start_Sect(p,0);
		Set_Nr_Sects(p,0);
	}
	//
	void Store4_Little_Endian(unsigned char *cp, unsigned int val) 
	{
		cp[0] = (val & 0xff);
		cp[1] = ((val >> 8) & 0xff);
		cp[2] = ((val >> 16) & 0xff);
		cp[3] = ((val >> 24) & 0xff);
	}
#if defined(__VERY_BIG_DISK__)
	void Store8_Little_Endian(unsigned char *cp, ULONGLONG val) 
	{
		cp[0] = (val & 0xff);
		cp[1] = ((val >> 8) & 0xff);
		cp[2] = ((val >> 16) & 0xff);
		cp[3] = ((val >> 24) & 0xff);
		cp[4] = ((val >> 32) & 0xff);
		cp[5] = ((val >> 40) & 0xff);
		cp[6] = ((val >> 48) & 0xff);
		cp[7] = ((val >> 56) & 0xff);
	}
#endif
	void Set_Start_Sect(struct partition *p, unsigned int start_sect) 
	{
		Store4_Little_Endian(p->start4, start_sect);
	}

	void Set_Nr_Sects(struct partition *p, unsigned int nr_sects) 
	{
		Store4_Little_Endian(p->size4, nr_sects);
	}

	void SetParttion(char* pszMbr, BYTE nIndex, 
		    		 unsigned long start,unsigned long stop, 
					 BYTE nSys,struct hd_geometry &ge);

#if !defined(__TDNXP__)
	int ValidOffset(int fd, loff_t offset);
#endif

	//写分区表中某个簇
	inline void Mark_FAT_Cluster (unsigned long cluster, unsigned long value, unsigned char* pFat)
	{
		cluster %= (DEFAULT_SECTOR_SIZE/4);
		value &= 0xffffffff;//mine
		pFat[4 * cluster] = (unsigned char)(value & 0x000000ff);
		pFat[(4 * cluster) + 1] = (unsigned char) ((value & 0x0000ff00) >> 8);
		pFat[(4 * cluster) + 2] = (unsigned char) ((value & 0x00ff0000) >> 16);
		pFat[(4 * cluster) + 3] = (unsigned char) ((value & 0xff000000) >> 24);
	}

	void CreateFileItem(msdos_dir_entry *de,char *pFileName,
		                unsigned long& start, unsigned long filesize,
						unsigned char nClusterSize);


	int DvrIdnCmp(BYTE *DvrId1, BYTE *DvrId2, int num);

	int Set_Sata_Sleep(char *szDevice);


	//计数esata的接口序号
	//int CalEsataIndex();

	//
	bool GetDiskSmartInfoImp(char *pDiskName, NET_DISK_SMART_INFO &stInfo);
	bool GetDiskModuleImp(char *pDiskName, NET_DISK_SMART_INFO &stInfo);
	void CopyString(char *pDesStr, char *pSouStr, unsigned long nLen);

//////////////////////////////////////////////////////////////////////////////////////////////
	
	//磁盘管理模块的实例
	static CDiskManager* m_pDiskMgrIns;  

	//与本模块相关的参数
	DISK_INIT_INFO m_DiskPara;

	//与固定硬盘相关的信息
	DISK_INFO_LIST m_FixedDiskList;   //全部固定硬盘的信息
	int m_nFixedStartPartId;               //固定硬盘起始挂载目录
	int m_nFixedEndPartId;                 //固定硬盘结束挂载目录
	POS m_mountDirToDisk[FIXEDDISK_MAX_MOUNT_DIR]; //由/mnt/XX找到对应的硬盘
	DISK_HEALTH_INFO_LIST m_healthFixedDiskList;


	//与热拔插相关的设备
	DISK_INFO_LIST m_RemoveableDiskList;  //m_RemovableDiskList中存放的是：所有挂载的，即正在使用的设备
	int            m_RemoveableDiskStartMountDir;
	bool           m_bMountDirUsed[REMOVEABLEDISK_MAX_MOUNT_DIR];



	//搜索设备的类，属于工具类
#if defined (__TDFH__) || defined (__TDNXP__) || defined (__CHIP3531__) || defined(__CHIP3535__) || defined (__CHIP3521__) || defined(__CHIP3520A__) || defined(__CHIPTI__) || defined(__CHIP3520D__) || defined(__CHIPGM__) 
	CDiskSearchEx m_SearchDiskTool;
#else
	CSearchDisk m_SearchDiskTool;
#endif
	ESATA_INDEX_LIST m_eSataIndexList;


	//处理进度
	MESSAGE_CALLBACK_EX  m_percentCallBack;
	void                 *m_pCallBackObject;
	
	//除了接入本机器的硬盘外，还接入了另外一些机器的硬盘，用此链保存其他机器。
	DVR_DISK_OWNER_LIST m_otherDvrInfoList;
	
	//是否接有本机硬盘
	bool                m_bHaveThisDvrFixedDisk;
	DVR_DISK_OWNER      m_thisDvrDiskOwner;
//zxx新加
private:
	int m_firstdisknum;//开机时候检测的磁盘个数，如果以后检查发现的磁盘比它少，报警，比它多，则修改为多的数
public:
	int GetFirstDiskNum() 
	{
		return m_firstdisknum;
	};
	int SetFirstDiskNum(int disknum) 
	{
		if(disknum >=0)
		{
			m_firstdisknum=disknum;
		}
		else
		{
			assert(false);
		}
	};

	int GetDiskCount();

#if defined(__VERY_BIG_DISK__)
	//如果为TRUE，说明当前设备所持有的硬盘是GPT硬盘。
	bool                m_bigDiskFlag;
#endif

	char         m_diskVersion[256];


};
#endif

