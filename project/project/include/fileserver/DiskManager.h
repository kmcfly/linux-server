
/***********************************************************************
** Copyright   Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : 
** Date         : 2007-09-14
** Name         : diskmanager.h
** Version      : 1.0
** Description  :  ���ڴ���ģ��ӿ�ʹ�õĹ淶˵����
                  1 �̶�Ӳ��ʹ��˵����
                    1�� ����Initial�������Ϳ��Ի�ȡ���ҹ������еĹ̶�¼��Ӳ�̡�
				    2�� ����ͨ������Ľӿ� GetDeviceInfo�� GetMountedPartionNum��FormatDisk���Թ̶�Ӳ�̷���
                  2 �����Ȱβ��豸ʹ��˵����
				    1������ͨ������Ľӿ� GetDeviceInfo��GetDevicePartionInfo����ȡ����ʹ�û�δʹ���豸����ϸ��Ϣ
					2��ͨ������Mount��ʹ������豸					
					3��ʹ����󣬵���Unmount�ͷ�����豸
				  3 ע�⣺Ŀǰ֧�ֶԶ���Ȱβ��豸���в�����
                  4 ��������Ӳ�̵�˵����
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
	//���캯��
	CDiskManager();

	//��������
	~CDiskManager();

	//�������̹����ʵ�������õ���ģʽ
	static CDiskManager*Instance( );

	//��ʼ������
	bool Initial(const DISK_INIT_INFO *pDiskInitInfo);

	////�����ƶ�Ӳ�̣�u�̣����ؽ������ĸ����� �豸����Ϣֻʹ����diskType�� diskname,totalSpace�⼸����
	//int SearchRemovableDisk(DISK_TYPE nDiskType, DVR_DISK_INFO_LIST *pDiskInfoList);

	//�����豸��
	int Mount(DISK_TYPE nDiskType, DISK_INTERFACE_TYPE nDiskInterface, char *diskname, unsigned long sizeM);

	//��ȡĳ�����͵��豸������Ϣ�����diskNameΪ�գ����ȡ���е��Ѿ����غ�Ϊ���ص��豸
	int GetDeviceDiskInfo(DISK_TYPE nDiskType, char *pDiskName, DVR_DISK_INFO_LIST &diskInfoList);

	//
	void GetOneRWDiskPartionNum(unsigned char &startMountIndex, unsigned char &partNum, char *pDevName = NULL);

	//��ȡĳ�����͵��豸������Ϣ�����diskNameΪ�գ����ȡ���е��Ѿ����غ�Ϊ���ص��豸�ķ���
	int GetDevicePartionInfo(DISK_TYPE nDiskType, char *pDiskName, DVR_DISK_INFO_LIST &partionInfoList);

	//��ȡ���سɹ��ķ�����
	//int GetMountedPartionNum(DISK_TYPE nDiskType);	

	//��ʽ��Ӳ��
	int FormatDisk(DISK_TYPE nDiskType, char *diskname, MESSAGE_CALLBACK_EX PercentCallBack, void *pPara);

	//�ı�Ӳ�̵�����
	int ChangeDiskProperty(DISK_TYPE nDiskType, char *pDiskName, unsigned char newDiskProperty);

	//�õ�Ӳ����������ЩDVR��
    int GetDiskOwner(DISK_TYPE nDiskType, DVR_DISK_OWNER_LIST &diskOwnerList);
	
	//
	int GetOwnerInfo(BYTE diskDvrIndex, DVR_DISK_OWNER &diskOwnerInfo);

	//ж��
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
//////�����ǶԹ̶�Ӳ�̵Ĳ���
	//
	int GetFixedDiskInfo(char *diskname,  PART_INFO *pPartitionInfo, BYTE *pPartitionNum,bool bBigDiskFlag);

	//
	int MountFixedDisk(DISK_INFO &diskInfo, int &startPartId, bool bCreateLogFile = false, unsigned long lAviNum = 0, unsigned long lEventLogSizeM = 0, unsigned long lOperLogSizeM = 0);

	//
	int FormatFixedDisk(char *diskname, MESSAGE_CALLBACK_EX PercentCallBack, void *pPara);

	//
	int UnMountFixedDisk(DISK_INFO &DiskInfo);

//////////////////////////////////////////////////////////////////////////
//////�����Ƕ��Ȱβ�Ӳ�̵Ĳ���
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
////�����ⲿ���ǹ���Ӳ�̣�U��һЩͨ�ò���
	//
	int FormatDiskImp(char *diskName, unsigned char partitionNum, unsigned long fileSizeM, unsigned long &lAviNum,
		              unsigned long &lEventLogSizeM,unsigned long &lOperLogSizeM, MESSAGE_CALLBACK_EX PercentCallBack, void *pPara,bool bBigDiskCheck);

#if defined(__VERY_BIG_DISK__)
	void ReverseBytes( void* theValue, int numBytes );
	int GptDiskPartitionProc(char *diskName, unsigned char partitionNum );
	
	int NormalDiskPartitionProc(char *diskName, unsigned char partitionNum , unsigned char nSys);

	int IsLittleEndian(void);	
	
	 //��Ӳ�̷���
	 int SetPartitionInfo(char *diskName, unsigned char partitionNum, unsigned char nSys, 
		                 MESSAGE_CALLBACK_EX PercentCallBack, void *pPara, bool pBigDiskFlag);

	bool FormatParttion(int &fd, unsigned long filesize, unsigned long &lAviNum,
										unsigned long &eventlogSizeM, unsigned long &operlogSizeM, bool bigDiskFlag);

	int GetGptParttionInfo( char *diskname, PART_INFO *pPartitionInfo, BYTE *pPartitionNum );

	bool GetDBRFilesystemInfo(char *pDiskname,unsigned char &partformat);
#else
	//��Ӳ�̷���
	int SetPartitionInfo(char *diskName, unsigned char partitionNum, unsigned char nSys, 
		                 MESSAGE_CALLBACK_EX PercentCallBack, void *pPara);

	//
	bool FormatParttion(int &fd, unsigned long filesize, unsigned long &lAviNum,unsigned long &eventlogSizeM, unsigned long &operlogSizeM);
#endif
	//��ȡӲ�̷�����Ϣ
    int GetParttionInfo(char *diskname,  PART_INFO *pPartitionInfo, BYTE *pPartitionNum);

	//�õ�����ʣ��ռ�
	int GetDiskUseSpace(DISK_INFO &diskInfo);

	//�����ڵ�͹���Ŀ¼
	int MakeNode(char *pDiskname, char *pMountDir, unsigned char startMountDirId, unsigned char partitionNum);


	
	//�ж����������Ƿ����������־
	inline int Valid_Part_Table_Flag(unsigned char *b) 
	{
		return (b[BOOT_LENGTH - 2] == 0x55 && b[BOOT_LENGTH - 1] == 0xaa);
	}

	//�õ����ֽ�������ռ�ĸ��ֽڣ���ʮ����ֵ
	inline ULONG Read4_Little_Endian(unsigned char *cp)
	{
		return  (ULONG)(cp[0])
			   +((ULONG)(cp[1]) << 8)
			   +((ULONG)(cp[2]) << 16) 
			   + ((ULONG)(cp[3]) << 24);
	}

#if defined(__VERY_BIG_DISK__)
	//�õ����ֽ�������ռ�˸��ֽڣ���ֵ
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
	
	//�ӷ������һ���������еõ���������ʼ����
	inline ULONG Get_Start_Sect(struct partition *p) 
	{
		return Read4_Little_Endian(p->start4);
	}
	
	//�ӷ������һ���������еõ���������������
	inline ULONG Get_Nr_Sects(struct partition *p) 
	{
		return Read4_Little_Endian(p->size4);
	}

	//�жϷ������һ����������Ϊȫ��
	//���ȫΪ�㣬�򷵻�1�����򣬷���0��
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

	//д��������ĳ����
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


	//����esata�Ľӿ����
	//int CalEsataIndex();

	//
	bool GetDiskSmartInfoImp(char *pDiskName, NET_DISK_SMART_INFO &stInfo);
	bool GetDiskModuleImp(char *pDiskName, NET_DISK_SMART_INFO &stInfo);
	void CopyString(char *pDesStr, char *pSouStr, unsigned long nLen);

//////////////////////////////////////////////////////////////////////////////////////////////
	
	//���̹���ģ���ʵ��
	static CDiskManager* m_pDiskMgrIns;  

	//�뱾ģ����صĲ���
	DISK_INIT_INFO m_DiskPara;

	//��̶�Ӳ����ص���Ϣ
	DISK_INFO_LIST m_FixedDiskList;   //ȫ���̶�Ӳ�̵���Ϣ
	int m_nFixedStartPartId;               //�̶�Ӳ����ʼ����Ŀ¼
	int m_nFixedEndPartId;                 //�̶�Ӳ�̽�������Ŀ¼
	POS m_mountDirToDisk[FIXEDDISK_MAX_MOUNT_DIR]; //��/mnt/XX�ҵ���Ӧ��Ӳ��
	DISK_HEALTH_INFO_LIST m_healthFixedDiskList;


	//���Ȱβ���ص��豸
	DISK_INFO_LIST m_RemoveableDiskList;  //m_RemovableDiskList�д�ŵ��ǣ����й��صģ�������ʹ�õ��豸
	int            m_RemoveableDiskStartMountDir;
	bool           m_bMountDirUsed[REMOVEABLEDISK_MAX_MOUNT_DIR];



	//�����豸���࣬���ڹ�����
#if defined (__TDFH__) || defined (__TDNXP__) || defined (__CHIP3531__) || defined(__CHIP3535__) || defined (__CHIP3521__) || defined(__CHIP3520A__) || defined(__CHIPTI__) || defined(__CHIP3520D__) || defined(__CHIPGM__) 
	CDiskSearchEx m_SearchDiskTool;
#else
	CSearchDisk m_SearchDiskTool;
#endif
	ESATA_INDEX_LIST m_eSataIndexList;


	//�������
	MESSAGE_CALLBACK_EX  m_percentCallBack;
	void                 *m_pCallBackObject;
	
	//���˽��뱾������Ӳ���⣬������������һЩ������Ӳ�̣��ô�����������������
	DVR_DISK_OWNER_LIST m_otherDvrInfoList;
	
	//�Ƿ���б���Ӳ��
	bool                m_bHaveThisDvrFixedDisk;
	DVR_DISK_OWNER      m_thisDvrDiskOwner;
//zxx�¼�
private:
	int m_firstdisknum;//����ʱ����Ĵ��̸���������Ժ��鷢�ֵĴ��̱����٣������������࣬���޸�Ϊ�����
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
	//���ΪTRUE��˵����ǰ�豸�����е�Ӳ����GPTӲ�̡�
	bool                m_bigDiskFlag;
#endif

	char         m_diskVersion[256];


};
#endif

