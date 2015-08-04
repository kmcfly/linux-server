
/***********************************************************************
** Copyright   Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : 
** Date         : 2007-09-14
** Name         : diskmanager.cpp
** Version      : 1.0
** Description  : 磁盘管理模块的实现文件
** Modify Record:
1:
***********************************************************************/
#include "DiskManager.h"
#include "PUB_common.h"
#include "Product.h"


#ifdef	LJ_DEBUG
#include "user_config.h"
#endif

extern bool g_bRu042304SEMETo2504HE_M;

#if!defined(__TDNXP__)
 #if defined(__CHIP3531__) || defined(__CHIP3535__) || defined(__CHIP3520A__) || defined(__CHIP3521__) || defined(__TDFH__) || defined(__CHIP3520D__) || defined(__CHIPTI__) || defined(__CHIPGM__) 
 #define llseek lseek64
 #else
_syscall5(int,  _llseek, uint, fd, ulong, hi, ulong, lo, loff_t *, res,
		  uint, wh);
loff_t llseek(int fd,loff_t offset,int whence)
{
	loff_t actual;	
	if (_llseek(fd, offset>>32, offset&0xffffffff, &actual, whence) != 0)
	{
		return (loff_t)-1;
	}
	return actual;
}
#endif
#endif

#define Set_CHS(h,s,c,sector,geometry) { \
				s = sector % geometry.sectors + 1;	\
				sector /= geometry.sectors;	\
				h = sector % geometry.heads;	\
				sector /= geometry.heads;	\
				c = sector & 0xff;	\
				s |= (sector >> 2) & 0xc0;	\
			}


#if defined(__TDNXP__)
#define  seekto(pos,errstr)						\
	do {									\
    off_t __pos = (pos);						\
    if (lseek (fd, __pos,SEEK_SET) != __pos)				\
	error("seek to " errstr " failed whilst writing tables");	\
	} while(0)
#else
#define seekto(pos,errstr)						\
	do {									\
    loff_t __pos = (pos);						\
    if (llseek (fd, __pos,SEEK_SET) != __pos)				\
	error("seek to " errstr " failed whilst writing tables");	\
	} while(0)
#endif

#define FREE()    free(pFat);	\
 			      free(pZeroFat);  \
			      free(pRootDir);  \
			      free(pInfoSector)
#define error(str)  \
	do {   \
          FREE(); \
		  printf(str); \
		  return false; \
	}while(0)
   



CDiskManager* CDiskManager::m_pDiskMgrIns = NULL;

CDiskManager::CDiskManager()
{

}
CDiskManager::~CDiskManager()
{

}

CDiskManager* CDiskManager::Instance()
{
	if (NULL == m_pDiskMgrIns) 
	{
		m_pDiskMgrIns = new CDiskManager;
		assert(NULL != m_pDiskMgrIns);
	}
	return m_pDiskMgrIns;
}

//对外提供接口


bool CDiskManager::Initial(const DISK_INIT_INFO *pDiskInitInfo)
{
	m_firstdisknum=0;
	printf("dvrId:%02x%02x%02x%02x%02x%02x%02x%02x\n", 
		   pDiskInitInfo->dvrId[0],pDiskInitInfo->dvrId[1],
		   pDiskInitInfo->dvrId[2],pDiskInitInfo->dvrId[3],
		   pDiskInitInfo->dvrId[4],pDiskInitInfo->dvrId[5],
		   pDiskInitInfo->dvrId[6],pDiskInitInfo->dvrId[7]);

	memset(&m_DiskPara, 0, sizeof(DISK_INIT_INFO));
	memcpy(&m_DiskPara, pDiskInitInfo, sizeof(DISK_INIT_INFO));

	memset(m_diskVersion, 0, sizeof(m_diskVersion));
	//截止到201205041130，支持的产品有下面这些。如果以后要增加新产品就需要修改这里，同时要给新增加的产品设置一个新
	//的diskversion
	PRODUCT_TYPE productType = CProduct::Instance()->ProductType();
	if (TD_2316SE_S == productType)
	{
		snprintf(m_diskVersion, 256, "diskver=2.3");
	}
	else if (TD_2324SE_S == productType)
	{
		snprintf(m_diskVersion, 256, "diskver=2.3");
	}
	else if (TD_2304SE == productType)
	{
		snprintf(m_diskVersion, 256, "diskver=2.3");
	}
	else if (TD_2308SE == productType)
	{
		snprintf(m_diskVersion, 256, "diskver=2.3");
	}
	else if (TD_2316SE == productType)
	{
		snprintf(m_diskVersion, 256, "diskver=2.3");
	}
	else if (TD_2304CDS == productType)
	{
		snprintf(m_diskVersion, 256, "diskver=2.3");
	}
	else if (TD_2308CDS == productType)
	{
		snprintf(m_diskVersion, 256, "diskver=2.3");
	}
	else if (TD_2316CDS == productType)
	{
		snprintf(m_diskVersion, 256, "diskver=2.3");
	}
	else if (TD_2304HS == productType)
	{
		snprintf(m_diskVersion, 256, "diskver=2.3");
	}
	else if (TD_2304ME == productType)
	{
		snprintf(m_diskVersion, 256, "diskver=2.3");
	}
	else if (TD_2308ME == productType)
	{
		snprintf(m_diskVersion, 256, "diskver=2.3");
	}
	else if (TD_2316ME == productType)
	{
		snprintf(m_diskVersion, 256, "diskver=2.3");
	}
	else if (TD_2304ME_A == productType)
	{
		snprintf(m_diskVersion, 256, "diskver=2.3");
	}
	else if (TD_2308ME_A == productType)
	{
		snprintf(m_diskVersion, 256, "diskver=2.3");
	}
	else if (TD_2316ME_A == productType)
	{
		snprintf(m_diskVersion, 256, "diskver=2.3");
	}
	else if (TD_2404MD == productType)
	{
		snprintf(m_diskVersion, 256, "diskver=2.3");
	}
	else if ((TD_2408MD == productType) || (TD_2408MD_4A == productType))
	{
		snprintf(m_diskVersion, 256, "diskver=2.3");
	}
	else if ((TD_2416MD == productType) || (TD_2416MD_4A == productType))
	{
		snprintf(m_diskVersion, 256, "diskver=2.3");
	}
	else if (TD_2424MD == productType)
	{
		snprintf(m_diskVersion, 256, "diskver=2.3");
	}
	else if ((TD_2432MD == productType) || (TD_2432MD_4A == productType))
	{
		snprintf(m_diskVersion, 256, "diskver=2.3");
	}
	else if (TD_2504HE == productType)
	{
		snprintf(m_diskVersion, 256, "diskver=2.3");
	}
	else if (TD_2508HE == productType)
	{
		snprintf(m_diskVersion, 256, "diskver=2.3");
	}
	else if (TD_2516HE == productType)
	{
		snprintf(m_diskVersion, 256, "diskver=2.3");
	}
	else if (TD_2504HE_M == productType)
	{
		snprintf(m_diskVersion, 256, "diskver=2.3");
	}
	else if (TD_2508HE_B == productType || TD_2516HE_B == productType || TD_2516HE_B_N100 == productType || TD_2524HE_B == productType || TD_2532HE_B == productType)
	{
		snprintf(m_diskVersion, 256, "diskver=2.3");
	}
	else if (TD_2504HC == productType)
	{
		snprintf(m_diskVersion, 256, "diskver=2.3");
	}
	else if (TD_2508HC == productType)
	{
		snprintf(m_diskVersion, 256, "diskver=2.3");
	}
	else if (TD_2516HC == productType)
	{
		snprintf(m_diskVersion, 256, "diskver=2.3");
	}
	else if (TD_2504HD == productType)
	{
		snprintf(m_diskVersion, 256, "diskver=2.3");
	}
	else if ((TD_2508HD == productType) || (TD_2508HD_4A == productType))
	{
		snprintf(m_diskVersion, 256, "diskver=2.3");
	}
	else if ((TD_2516HD_4A == productType)||(TD_2516HD == productType))
	{
		snprintf(m_diskVersion, 256, "diskver=2.3");
	}
	else if (TD_2504FD == productType)
	{
		snprintf(m_diskVersion, 256, "diskver=2.3");
	}
	else if ((TD_2508FD_4A == productType) || (TD_2508FD == productType))
	{
		snprintf(m_diskVersion, 256, "diskver=2.3");
	}
	else if ((TD_2516FD_4A == productType) || (TD_2516FD == productType))
	{
		snprintf(m_diskVersion, 256, "diskver=2.3");
	}
	else if (TD_2516FD_S == productType)
	{
		snprintf(m_diskVersion, 256, "diskver=2.3");
	}
	else if (TD_2604LD_S == productType)
	{
		snprintf(m_diskVersion, 256, "diskver=2.3");
	}
	else if (TD_2608LD_S == productType)
	{
		snprintf(m_diskVersion, 256, "diskver=2.3");
	}
	else if (TD_2616LD_S == productType)
	{
		snprintf(m_diskVersion, 256, "diskver=2.3");
	}
	else if (TD_2616LD == productType)
	{
		snprintf(m_diskVersion, 256, "diskver=2.3");
	}
	else if (TD_2704HD == productType)
	{
		snprintf(m_diskVersion, 256, "diskver=5.2");
	}
	else if (TD_2704XE_L == productType)
	{
		snprintf(m_diskVersion, 256, "diskver=5.2");
	}
	else if (TD_2708XD_L == productType)
	{
		snprintf(m_diskVersion, 256, "diskver=5.2");
	}
	else if (TD_2716XD_L == productType)
	{
		snprintf(m_diskVersion, 256, "diskver=5.2");
	}
	else if ((TD_2704XD_P == productType) || (TD_2708XD_P == productType) || (TD_2716XD_P == productType))
	{
		snprintf(m_diskVersion, 256, "diskver=6.0");
	}
	else if (TD_7004SB == productType)
	{
		snprintf(m_diskVersion, 256, "diskver=2.3");
	}
	else if (TD_7008SB == productType)
	{
		snprintf(m_diskVersion, 256, "diskver=2.3");
	}
	else if (TD_2504HE_B == productType)
	{
		snprintf(m_diskVersion, 256, "diskver=7.0");
	}
	else if (TD_2508HE_B == productType)
	{
		snprintf(m_diskVersion, 256, "diskver=7.1");
	}
	else if (TD_2504HE_L == productType)
	{
		snprintf(m_diskVersion, 256, "diskver=7.2");
	}
	else if (TD_2508HE_L == productType)
	{
		snprintf(m_diskVersion, 256, "diskver=7.3");
	}
	else if (TD_2516HE_L == productType)
	{
		snprintf(m_diskVersion, 256, "diskver=7.4");
	}
	else if (TD_2604LE == productType || TD_2608LE == productType || TD_2616LE == productType || TD_2524HE_C == productType || TD_2532HE_C == productType)
	{
		snprintf(m_diskVersion, 256, "diskver=7.5");
	}
	else if (TD_2304SS_B == productType || TD_2308SS_B == productType || TD_2316SS_B == productType || TD_2304SS_C == productType || TD_2308SS_C == productType || TD_2316SS_C == productType  || TD_2304SL_C == productType || TD_2308SL_C == productType)
	{
		snprintf(m_diskVersion, 256, "diskver=7.6");
	}
	else if (TD_2504HD_C == productType || TD_2508HD_C == productType || TD_2516HD_C == productType || TD_2524HD_C == productType || TD_2532HD_C == productType)
	{
		snprintf(m_diskVersion, 256, "diskver=7.7");
	}
	else if (TD_2504HS_BL == productType || TD_2508HS_BL == productType || TD_2516HS_BL)
	{
		snprintf(m_diskVersion, 256, "diskver=7.8");
	}
	else if (TD_2304ME_C == productType || TD_2308ME_C == productType || TD_2316ME_C == productType || TD_2316ME_BH == productType || TD_2508HE_B_3521 == productType)
	{
		snprintf(m_diskVersion, 256, "diskver=7.9");
	}
	else if (TD_2704XD_M == productType || TD_2708XD_M == productType || TD_2716XD_M == productType)
	{
		snprintf(m_diskVersion, 256, "diskver=8.0");
	}
	else if (TD_2304SE_B == productType || TD_2308SE_B == productType || TD_2316SE_B == productType)
	{
		snprintf(m_diskVersion, 256, "diskver=8.1");
	}
	else if (TD_2304SS_SL == productType || TD_2308SS_SL == productType)
	{
		snprintf(m_diskVersion, 256, "diskver=8.2");
	}
	else if (TD_2508HE_C_3521 == productType)
	{
		snprintf(m_diskVersion, 256, "diskver=8.3");
	}
	else if ((TD_2708XE_M == productType) || TD_2716XE_M == productType || (TD_2708XE_MH == productType))
	{
		snprintf(m_diskVersion, 256, "diskver=8.4");
	}
	else if ((TD_2404MD_B == productType) || (TD_2408MD_B == productType) || (TD_2416MD_B == productType))
	{
		snprintf(m_diskVersion, 256, "diskver=8.5");
	}
	else if (TD_2708XE_P == productType)
	{
		snprintf(m_diskVersion, 256, "diskver=8.6");
	}
	else if ((TD_2704XE_S == productType) || (TD_2708XE_S == productType) || (TD_2716XE_S == productType) || (TD_2704XS_SL == productType) || (TD_2708XS_SL == productType) || (TD_2716XS_SL == productType)
		 || (TD_2704AS_SL == productType) || (TD_2708AS_SL == productType) || (TD_2716AS_SL == productType)
		 || (TD_2704AS_SL_A == productType) || (TD_2708AS_SL_A == productType) || (TD_2716AS_SL_A == productType)
		 || (TD_2704AS_S == productType) || (TD_2708AS_S == productType) || (TD_2716AS_S == productType)
		 || (TD_2704AS_S_A == productType) || (TD_2708AS_S_A == productType) || (TD_2716AS_S_A == productType)
		 || (TD_2704TS_S == productType) || (TD_2708TS_S == productType) || (TD_2716TS_S == productType)
		 || (TD_2704TS_PL == productType) || (TD_2708TS_PL == productType) || (TD_2716TS_PL == productType)
		 || (TD_2704TD_PL == productType) || (TD_2708TD_PL == productType) || (TD_2716TD_PL == productType) || (TD_2732TD_PL == productType)
		 || (TD_2704TD_P == productType) || (TD_2708TD_P == productType) || (TD_2716TD_P == productType)
		 || (TD_2704TE_M == productType) || (TD_2708TE_M == productType) || (TD_2716TE_M == productType) 
		 || (TD_2704AE_SL == productType) || (TD_2708AE_SL == productType) || (TD_2716AS_SL == productType)
		 || (TD_2004AL == productType) || (TD_2008AS == productType)
		 || (TD_2704AS_PL == productType)
		 || (TD_2004AL == productType) || (TD_2008AS == productType)
		 || (TD_2704AS_PL == productType) || (TD_2708AS_PL == productType) || (TD_2716AS_PL == productType)
		 || (TD_2716AC_PL == productType)
		 || (TD_2704AE_PL == productType) || (TD_2708AE_PL == productType) || (TD_2716AE_PL == productType) )
	{
		snprintf(m_diskVersion, 256, "diskver=8.7");
	}
	else if (TD_2304SE_C == productType || TD_2308SE_C == productType || TD_2316SE_C == productType)
	{
		snprintf(m_diskVersion, 256, "diskver=8.8");
	}
	else if(TD_2704XD_S == productType || TD_2708XD_S == productType || TD_2716XD_S == productType)
	{
		snprintf(m_diskVersion, 256, "diskver=8.9");
	}
	else if (TD_2304SS_SDI == productType || TD_2308SS_SDI == productType  || TD_2508HE_SDI == productType  || TD_2512HE_SDI == productType  || TD_2524HE_SDI == productType)
	{
		snprintf(m_diskVersion, 256, "diskver=9.0");
	}
	else if (TD_2304SS_SL_3515A == productType)
	{
		snprintf(m_diskVersion, 256, "diskver=9.1");
	}
	/*如果以后要增加新产品就需要修改这里，同时要给新增加的产品设置一个新
	的diskversion.即每一个新增加一个产品，就使用一个新的diskversion.
	起始diskversion是7.0
	*/
	/*
	else if ()
	{

	}
    */
	else
	{
		printf("################%s,%d,diskman no this product:%d\n",__FILE__,__LINE__,productType);
		return false;		
	}

	m_FixedDiskList.RemoveAll();
	m_healthFixedDiskList.RemoveAll();
	m_nFixedStartPartId = FIXEDDISK_BEGIN_MOUNT_DIR;
	m_nFixedEndPartId = m_nFixedStartPartId;
	m_bHaveThisDvrFixedDisk = false;

	m_otherDvrInfoList.RemoveAll();

	m_RemoveableDiskList.RemoveAll();
	m_RemoveableDiskStartMountDir = REMOVEABLEDISK_BEGIN_MOUNT_DIR;
    memset(m_bMountDirUsed, 0, sizeof(m_bMountDirUsed));
	

	//统计机器支持的esata的编号
	{
		m_eSataIndexList.RemoveAll();
		if (m_DiskPara.bHaveEsata)
		{
			int nInterface = m_DiskPara.nEsataInterface;
			nInterface--;
			m_eSataIndexList.AddTail(nInterface);
		}
		if (m_DiskPara.nPciToEsataNum > 0)
		{
			for (int count = 0; count < m_DiskPara.nPciToEsataNum; ++count)
			{
				int nInterface = m_DiskPara.pciToEsataIndex[count];
				nInterface--;
				m_eSataIndexList.AddTail(nInterface);			
			}
		}		
	}
	
	
	//搜索固定硬盘
	m_SearchDiskTool.SearchFixedDisk(m_FixedDiskList, m_eSataIndexList);

	//获取每个硬盘的信息
	POS pos = m_FixedDiskList.GetHeadPosition();
	while (pos != NULL)
	{
		DISK_INFO &diskInfo = m_FixedDiskList.GetAt(pos);
		diskInfo.partitionNum = 0;
		bool bBigDiskFlag=false;
		if(diskInfo.disksize > 2*1024*1024)
		{
			bBigDiskFlag = true;
		}
		else
		{
			bBigDiskFlag = false;
		}
		GetFixedDiskInfo(diskInfo.diskname, diskInfo.partitionInfo, &diskInfo.partitionNum,bBigDiskFlag);
		DISK_HEALTH_INFO diskHealth;
		snprintf(diskHealth.szDiskName, sizeof(diskHealth.szDiskName), "%s", diskInfo.diskname);
		diskHealth.bSleep = 0;
		memset(&diskHealth.diskSmartInfo, 0, sizeof(diskHealth.diskSmartInfo));
		m_healthFixedDiskList.AddTail(diskHealth);
		m_FixedDiskList.GetNext(pos);
	}
	
	//挂载每个硬盘	
	pos = m_FixedDiskList.GetHeadPosition();
	while (pos != NULL)
	{
		DISK_INFO &diskInfo = m_FixedDiskList.GetNext(pos);
		MountFixedDisk(diskInfo, m_nFixedEndPartId);		
	}

	//在只接一个硬盘且是其他机器的，则可以把这个硬盘修改本机硬盘
	if ((m_FixedDiskList.GetCount() == 1) && (m_otherDvrInfoList.GetCount() == 1))
	{
		printf("%s,%d,change other disk to this\n",__FILE__,__LINE__);
		POS pos = m_FixedDiskList.GetHeadPosition();
		while (pos != NULL)
		{
			DISK_INFO &diskInfo = m_FixedDiskList.GetAt(pos);
			if (NET_DISK_NO_FORMAT == diskInfo.diskStatus)
			{
				m_FixedDiskList.GetNext(pos);
				continue;						
			}
			diskInfo.diskProperty = NET_DISK_READ_WRITE;
			diskInfo.diskOwnedBy = NET_DISK_OWNED_BY_THIS;
			for (int i = 0; i < diskInfo.partitionNum; ++i)
			{
				char szTmp[MAX_PATH] = {0};
				PART_INFO *p = &diskInfo.partitionInfo[i];
				FILE *f = NULL;
				snprintf(szTmp, MAX_PATH, "%s/diskver", p->partpath);
				f = fopen(szTmp, "rb+");
				if (NULL == f)
				{
					break;
				}
				if (0 != fseek(f, 256*2, SEEK_SET))
				{
					fclose(f);
					break;
				}
				memset(szTmp, 0, sizeof(szTmp));
				snprintf(szTmp, sizeof(DISK_ID), "%s", DISK_ID);
				memcpy(&szTmp[sizeof(DISK_ID)], m_DiskPara.dvrId, 8);
				if (256 != fwrite(szTmp, sizeof(char), 256, f))
				{
					fclose(f);
					break;
				}
				fclose(f);
			}

			m_FixedDiskList.GetNext(pos);
		}

		sync();
		
		memset(&m_thisDvrDiskOwner, 0, sizeof(DVR_DISK_OWNER));
		m_bHaveThisDvrFixedDisk = true;
		m_thisDvrDiskOwner.dvrIndex = NET_DISK_OWNED_BY_THIS;
		memcpy(m_thisDvrDiskOwner.dvrId, m_DiskPara.dvrId, 8);
		m_thisDvrDiskOwner.dvrPara = m_DiskPara.videoInputNum;

		m_otherDvrInfoList.RemoveAll();
	}



	//获取分区与硬盘的对应关系
	assert(m_nFixedEndPartId <= FIXEDDISK_MAX_MOUNT_DIR);
	pos = m_FixedDiskList.GetHeadPosition();
	while (pos != NULL)
	{
		DISK_INFO &diskInfo = m_FixedDiskList.GetAt(pos);
		if (NET_DISK_NO_FORMAT == diskInfo.diskStatus)
		{
			m_FixedDiskList.GetNext(pos);
			continue;						
		}
		
		for (int i = 0; i < diskInfo.partitionNum; ++i)
		{
			m_mountDirToDisk[diskInfo.partitionInfo[i].partid] = pos;			
		}

		m_FixedDiskList.GetNext(pos);
	}

	for (int i = 0;i < m_nFixedEndPartId; ++i)
	{
		DISK_INFO &diskInfo = m_FixedDiskList.GetAt(m_mountDirToDisk[i]);
		printf("%s/%02d === %s\n", m_DiskPara.szDiskMountPath, i, diskInfo.diskname);
	}

	return true;
}

//
//int CDiskManager::SearchRemovableDisk(DISK_TYPE nDiskType, DVR_DISK_INFO_LIST *pDiskInfoList)
//{
//	int ret = 0;
//	if (NET_DISK_TYPE_REMDISK == nDiskType)
//	{
//		ret = m_SearchDiskTool.SearchREMDisk(pDiskInfoList);
//	}
//	else if (NET_DISK_TYPE_USB == nDiskType)
//	{
//		ret = m_SearchDiskTool.SearchUSBDisk(pDiskInfoList);
//	}	
//	return ret;
//}


int CDiskManager::Mount(DISK_TYPE nDiskType, DISK_INTERFACE_TYPE nDiskInterface, char *diskname, unsigned long sizeM)
{
	if (NET_DISK_TYPE_REMOVEABLE == nDiskType)
	{
		//查找这个设备是否已经挂载了
		POS pos = m_RemoveableDiskList.GetHeadPosition();
		while (NULL != pos)
		{
			DISK_INFO &diskInfo = m_RemoveableDiskList.GetAt(pos);
			if (0 == strcmp(diskInfo.diskname, diskname))
			{
				assert(false);
				return 0;
			}
			m_RemoveableDiskList.GetNext(pos);
		}
		
		DISK_INFO diskInfoTmp;
		memset(&diskInfoTmp, 0, sizeof(diskInfoTmp));
		diskInfoTmp.diskOwnedBy = NET_DISK_OWNED_BY_THIS;
		diskInfoTmp.diskStatus = NET_DISK_NO_MOUNT;
		sprintf(diskInfoTmp.diskname, "%s", diskname);
		diskInfoTmp.diskType = nDiskType;
		diskInfoTmp.diskInterfacetype = nDiskInterface;
		diskInfoTmp.disksize = sizeM;		
		diskInfoTmp.partitionNum = 0;
		if (GetRemovableDiskInfo(diskInfoTmp.diskname, diskInfoTmp.partitionInfo, &diskInfoTmp.partitionNum, diskInfoTmp.disksize) < 0)
		{
			return -1;
		}

		assert(diskInfoTmp.partitionNum > 0);
		int mountDir = -1;
		if (FindRemovealbeDiskMountDir(&mountDir, diskInfoTmp.partitionNum) < 0)
		{
			return -1;
		}

		if (MountRemovableDisk(diskInfoTmp, mountDir) < 0)
		{			
			return -1;
		}

		
		m_RemoveableDiskList.AddTail(diskInfoTmp);

		return 0;
	}
	else
	{
		assert(false);
	}

	return -1;

}


int CDiskManager::FormatDisk(DISK_TYPE nDiskType, char *diskname, MESSAGE_CALLBACK_EX PercentCallBack, void *pPara)
{
	assert(NULL != PercentCallBack);
	assert(NULL != pPara);

	if (NET_DISK_TYPE_FIXEDDISK == nDiskType)
	{
		return FormatFixedDisk(diskname, PercentCallBack, pPara);
	}
	return 0;
}

int CDiskManager::ChangeDiskProperty(DISK_TYPE nDiskType, char *pDiskName, unsigned char newDiskProperty)
{
	assert(pDiskName != NULL);
	char szTmp[256] = {0};
	FILE *pFile = NULL;
	PART_INFO *p = NULL;
	int ret = 0;
	if (NET_DISK_TYPE_FIXEDDISK == nDiskType)
	{
		POS pos = m_FixedDiskList.GetHeadPosition();
		while (pos != NULL)
		{
			DISK_INFO &diskInfo = m_FixedDiskList.GetAt(pos);
			if (0 == strcmp(diskInfo.diskname, pDiskName))
			{
				assert(NET_DISK_OWNED_BY_THIS == diskInfo.diskOwnedBy);
				assert(NET_DISK_NORMAL == diskInfo.diskStatus);
				assert(diskInfo.diskProperty != newDiskProperty);
				diskInfo.diskProperty = newDiskProperty;
				for (int i = 0; i < diskInfo.partitionNum; ++i)
				{
					p = &diskInfo.partitionInfo[i];
					memset(szTmp, 0, sizeof(szTmp));
					snprintf(szTmp, 256, "%s/diskver", p->partpath);
					pFile = fopen(szTmp, "rb+");
					if (NULL == pFile)
					{
						ret = -1;
						break;
					}
					
					if (0 != fseek(pFile, 3 * 256 * sizeof(char), SEEK_SET))
					{
						ret = -1;
						break;
					}

					memset(szTmp, 0, sizeof(szTmp));
					snprintf(szTmp, 256, "%s%d", DISK_ACCESS_PROPERTY, diskInfo.diskProperty);
					if (256 != fwrite(szTmp, sizeof(char), 256, pFile))
					{
						ret = -1;
						break;
					}

					fflush(pFile);
					if (NULL != pFile)
					{
						fclose(pFile);
						pFile = NULL;
					}
				}

				if (NULL != pFile)
				{
					fclose(pFile);
					pFile = NULL;
				}

				break;
			}			
			m_FixedDiskList.GetNext(pos);
		}
		assert(NULL != pos);

	}
	else
	{
		assert(false);
	}


	return ret;

}

int CDiskManager::GetDeviceDiskInfo(DISK_TYPE nDiskType, char *pDiskName, DVR_DISK_INFO_LIST &diskInfoList)
{
	int nRet = 0;
	if (NET_DISK_TYPE_FIXEDDISK == nDiskType)
	{
		POS pos = m_FixedDiskList.GetHeadPosition();
		while (pos != NULL)
		{
			DVR_DISK_INFO diskInfoTmp;
			DISK_INFO &diskInfo = m_FixedDiskList.GetAt(pos);
			if ((NULL != pDiskName) && (0 != strcmp(diskInfo.diskname, pDiskName)))
			{
				m_FixedDiskList.GetNext(pos);
				continue;
			}
			diskInfoTmp.diskOwnedBy   = diskInfo.diskOwnedBy;
			diskInfoTmp.diskType      = diskInfo.diskType;
			diskInfoTmp.diskInterface = diskInfo.diskInterfacetype;		
			diskInfoTmp.diskStatus    = diskInfo.diskStatus;
			diskInfoTmp.diskProperty  = diskInfo.diskProperty;
			strcpy(diskInfoTmp.diskname, diskInfo.diskname);
			diskInfoTmp.totalSpace    = diskInfo.disksize;			    
			diskInfoTmp.partitionNum  = diskInfo.partitionNum;	
			diskInfoTmp.reserved	  = diskInfo.interfaceindex;		//索引
			if (NET_DISK_NORMAL == diskInfo.diskStatus)
			{
				diskInfoTmp.diskpara  = diskInfo.partitionInfo[0].partid;
			}
			diskInfoList.AddTail(diskInfoTmp);
			if (NULL != pDiskName)
			{
				break;
			}
			m_FixedDiskList.GetNext(pos);
		}
		nRet = diskInfoList.GetCount();
	}
	else if (NET_DISK_TYPE_REMOVEABLE == nDiskType)
	{
		//m_RemoveableDiskList中存放的是：所有挂载的，即正在使用的设备
		DISK_INFO_LIST diskUseList;
		POS pos = m_RemoveableDiskList.GetHeadPosition();
		while (NULL != pos)
		{
			DISK_INFO &tmp = m_RemoveableDiskList.GetAt(pos);
			diskUseList.AddTail(tmp);
			m_RemoveableDiskList.GetNext(pos);
		}
		
		//allDiskInfoList中存放的是：当前时刻，所有该类型的设备
		DISK_INFO_LIST allDiskInfoList;
		m_SearchDiskTool.SearchRemoveableDisk(&allDiskInfoList,m_eSataIndexList, m_DiskPara.szUsbToEsataPath);
        
		//从diskUseList中删除：没有卸载就拔掉的设备
		POS prePos = NULL;
		pos = diskUseList.GetHeadPosition();			
		while (NULL != pos)
		{
			prePos = pos;
			diskUseList.GetNext(pos);
			DISK_INFO &diskInfo = diskUseList.GetAt(prePos);
			bool bFind = false;
			POS allPos = allDiskInfoList.GetHeadPosition();
			while (NULL != allPos)
			{
				DISK_INFO &tmp = allDiskInfoList.GetAt(allPos);
				if (0 == strcmp(tmp.diskname, diskInfo.diskname))
				{
					bFind = true;
					break;
				}
				allDiskInfoList.GetNext(allPos);
			}

			if (!bFind)
			{
				diskUseList.RemoveAt(prePos);
			}
			else
			{
				allDiskInfoList.RemoveAt(allPos);
			}
		}
		
		//这时，diskUseList中的设备都是正在使用的。,
		//allDiskInfoList中只有是新增加的设备。

		//刷新diskUseList中的设备使用空间
		pos = diskUseList.GetHeadPosition();
		while (NULL != pos)
		{
			DISK_INFO &diskInfo = diskUseList.GetAt(pos);
			GetDiskUseSpace(diskInfo);
			diskUseList.GetNext(pos);
		}
		
		//把allDiskInfoList剩余的设备,进行临时挂载，目的是：得到该设备的详细信息
		prePos = NULL;
		pos = allDiskInfoList.GetHeadPosition();
		while (NULL != pos)
		{
			prePos = pos;
			allDiskInfoList.GetNext(pos);
			DISK_INFO &diskInfo = allDiskInfoList.GetAt(prePos);			

			if (GetRemovableDiskInfo(diskInfo.diskname, diskInfo.partitionInfo, &diskInfo.partitionNum, diskInfo.disksize) < 0)
			{
				continue;
			}

			//寻找挂载目录
			int mountDir = -1;
			if (FindRemovealbeDiskMountDir(&mountDir, diskInfo.partitionNum) < 0)
			{
				continue;
			}

			if (MountRemovableDisk(diskInfo, mountDir) < 0)
			{	
				continue;
			}

			
		}
		

		//把diskUseList和allDiskInfoList中的设备信息，转换成外面想要的格式
		pos = diskUseList.GetHeadPosition();
		while (NULL != pos)
		{
			DVR_DISK_INFO dvrDiskInfo;
			DISK_INFO &diskInfo = diskUseList.GetAt(pos);
			if ((NULL != pDiskName) && (0 != strcmp(diskInfo.diskname, pDiskName)))
			{
				diskUseList.GetNext(pos);
				continue;
			}
			memset(&dvrDiskInfo, 0, sizeof(dvrDiskInfo));
			dvrDiskInfo.diskType = diskInfo.diskType;
			dvrDiskInfo.diskInterface = diskInfo.diskInterfacetype;
			strcpy(dvrDiskInfo.diskname, diskInfo.diskname);
			dvrDiskInfo.diskStatus = diskInfo.diskStatus;
			assert(NET_DISK_IS_MOUNT == diskInfo.diskStatus);
			dvrDiskInfo.diskProperty = diskInfo.diskProperty;
			dvrDiskInfo.totalSpace = diskInfo.disksize;
			dvrDiskInfo.freeSpace  = diskInfo.freesize;
			dvrDiskInfo.partitionNum = diskInfo.partitionNum;
			diskInfoList.AddTail(dvrDiskInfo);
			diskUseList.GetNext(pos);
		}


		pos = allDiskInfoList.GetHeadPosition();
		while (NULL != pos)
		{
			DVR_DISK_INFO dvrDiskInfo;
			DISK_INFO &diskInfo = diskUseList.GetAt(pos);
			if ((NULL != pDiskName) && (0 != strcmp(diskInfo.diskname, pDiskName)))
			{
				diskUseList.GetNext(pos);
				continue;
			}
			memset(&dvrDiskInfo, 0, sizeof(dvrDiskInfo));
			dvrDiskInfo.diskType = diskInfo.diskType;
			dvrDiskInfo.diskInterface = diskInfo.diskInterfacetype;
			strcpy(dvrDiskInfo.diskname, diskInfo.diskname);
			dvrDiskInfo.diskProperty = diskInfo.diskProperty;
			dvrDiskInfo.totalSpace = diskInfo.disksize;
			if (NET_DISK_IS_MOUNT == diskInfo.diskStatus)
			{
				dvrDiskInfo.freeSpace = diskInfo.freesize;
				dvrDiskInfo.partitionNum = diskInfo.partitionNum;
			}
			else
			{
				dvrDiskInfo.freeSpace = diskInfo.disksize;
				dvrDiskInfo.partitionNum = 0;
			}
			dvrDiskInfo.diskStatus = NET_DISK_NO_MOUNT;
			diskInfoList.AddTail(dvrDiskInfo);
			allDiskInfoList.GetNext(pos);
		}

		nRet = diskInfoList.GetCount();

		pos = allDiskInfoList.GetHeadPosition();
		while (NULL != pos)
		{
			DISK_INFO &diskInfo = allDiskInfoList.GetAt(pos);
			UnMountRemovableDisk(diskInfo);
			allDiskInfoList.GetNext(pos);
		}

		diskUseList.RemoveAll();
		allDiskInfoList.RemoveAll();

	}
	else 
	{
		assert(false);
	}
	
	return nRet;
}

void CDiskManager::GetOneRWDiskPartionNum(unsigned char &startMountIndex, unsigned char &partNum, char *pDevName /* = NULL */)
{
	startMountIndex = 0;
	partNum = 0;
	POS pos = m_FixedDiskList.GetHeadPosition();
	while (pos != NULL)
	{		
		DISK_INFO &diskInfo = m_FixedDiskList.GetNext(pos);
		if ((NET_DISK_OWNED_BY_THIS == diskInfo.diskOwnedBy)
		  &&(NET_DISK_NORMAL == diskInfo.diskStatus)
		  &&(NET_DISK_READ_WRITE == diskInfo.diskProperty))
		{
			startMountIndex = diskInfo.partitionInfo[0].partid;
			partNum = diskInfo.partitionNum;
			if (pDevName != NULL)
			{
				strcpy(pDevName, diskInfo.partitionInfo[0].devnod);
			}			
			break;
		}
	}
	
}


int CDiskManager::GetDevicePartionInfo(DISK_TYPE nDiskType, char *pDiskName, DVR_DISK_INFO_LIST &partionInfoList)
{
	POS DiskPos = NULL;
	POS PartionPos = NULL;
	int nRet = 0;
	if (NET_DISK_TYPE_FIXEDDISK == nDiskType)
	{
		POS pos = m_FixedDiskList.GetHeadPosition();
		while (pos != NULL)
		{			
			DISK_INFO &diskInfo = m_FixedDiskList.GetAt(pos);
			if ((NULL != pDiskName) && (0 != strcmp(diskInfo.diskname, pDiskName)))
			{
				m_FixedDiskList.GetNext(pos);
				continue;
			}
			
			for (int i = 0; i < diskInfo.partitionNum; ++i)
			{
				DVR_DISK_INFO partionInfoTmp;
				PART_INFO *partionInfo       = &diskInfo.partitionInfo[i];
				partionInfoTmp.diskOwnedBy   = diskInfo.diskOwnedBy;
				partionInfoTmp.diskType      = diskInfo.diskType;
				partionInfoTmp.diskInterface = diskInfo.diskInterfacetype;
				partionInfoTmp.diskStatus    = diskInfo.diskStatus;
				partionInfoTmp.diskProperty  = diskInfo.diskProperty;
				strcpy(partionInfoTmp.diskname, partionInfo->devnod);
				partionInfoTmp.totalSpace    = partionInfo->partsize;
				partionInfoTmp.diskpara      = partionInfo->partid;
				partionInfoList.AddTail(partionInfoTmp);				
			}			
			m_FixedDiskList.GetNext(pos);
		}
		nRet = partionInfoList.GetCount();

	}
	else if (NET_DISK_TYPE_REMOVEABLE == nDiskType)
	{
		//m_RemoveableDiskList中存放的是：所有挂载的，即正在使用的设备
		DISK_INFO_LIST diskUseList;
		POS pos = m_RemoveableDiskList.GetHeadPosition();
		while (NULL != pos)
		{
			DISK_INFO &tmp = m_RemoveableDiskList.GetAt(pos);
			diskUseList.AddTail(tmp);
			m_RemoveableDiskList.GetNext(pos);
		}


		//allDiskInfoList中存放的是：当前时刻，所有该类型的设备
		DISK_INFO_LIST allDiskInfoList;
		m_SearchDiskTool.SearchRemoveableDisk(&allDiskInfoList,m_eSataIndexList,m_DiskPara.szUsbToEsataPath);

		//从diskUseList中删除：没有卸载就拔掉的设备
		POS prePos = NULL;
		pos = diskUseList.GetHeadPosition();			
		while (NULL != pos)
		{
			prePos = pos;
			diskUseList.GetNext(pos);
			DISK_INFO &diskInfo = diskUseList.GetAt(prePos);
			bool bFind = false;
			POS allPos = allDiskInfoList.GetHeadPosition();
			while (NULL != allPos)
			{
				DISK_INFO &tmp = allDiskInfoList.GetAt(allPos);
				if (0 == strcmp(tmp.diskname, diskInfo.diskname))
				{
					bFind = true;
					break;
				}
				allDiskInfoList.GetNext(allPos);
			}

			if (!bFind)
			{
				diskUseList.RemoveAt(prePos);
			}
			else
			{
				allDiskInfoList.RemoveAt(allPos);
			}
		}

		//这时，diskUseList中的设备都是正在使用的。,
		//allDiskInfoList中只有是新增加的设备。

		//刷新diskUseList中的设备使用空间
		pos = diskUseList.GetHeadPosition();
		while (NULL != pos)
		{
			DISK_INFO &diskInfo = diskUseList.GetAt(pos);
			GetDiskUseSpace(diskInfo);
			diskUseList.GetNext(pos);
		}

		//把allDiskInfoList剩余的设备,进行挂载，目的是：得到该设备的详细信息
		prePos = NULL;
		pos = allDiskInfoList.GetHeadPosition();
		while (NULL != pos)
		{
			prePos = pos;
			allDiskInfoList.GetNext(pos);
			DISK_INFO &diskInfo = allDiskInfoList.GetAt(prePos);
			if (GetRemovableDiskInfo(diskInfo.diskname, diskInfo.partitionInfo, &diskInfo.partitionNum, diskInfo.disksize) < 0)
			{
				continue;
			}

			//寻找挂载目录

			int mountDir = 0;
			if (FindRemovealbeDiskMountDir(&mountDir, diskInfo.partitionNum) < 0)
			{
				continue;
			}


			if (MountRemovableDisk(diskInfo, mountDir) < 0)
			{			
				continue;
			}




			
		}


		//把diskUseList和allDiskInfoList中的设备信息，转换成外面想要的格式
		pos = diskUseList.GetHeadPosition();
		while (NULL != pos)
		{
			DISK_INFO &diskInfo = diskUseList.GetAt(pos);
			if ((NULL != pDiskName) && (0 != strcmp(diskInfo.diskname, pDiskName)))
			{
				diskUseList.GetNext(pos);
				continue;
			}
			assert(NET_DISK_IS_MOUNT == diskInfo.diskStatus);
			for (int i = 0; i < diskInfo.partitionNum; ++i)
			{
				DVR_DISK_INFO partionInfoTmp;
				PART_INFO *partionInfo = &diskInfo.partitionInfo[i];
				partionInfoTmp.diskType = diskInfo.diskType;
				partionInfoTmp.diskInterface = diskInfo.diskInterfacetype;
				partionInfoTmp.diskStatus = diskInfo.diskStatus;
				partionInfoTmp.diskProperty = diskInfo.diskProperty;
				strcpy(partionInfoTmp.diskname, partionInfo->devnod);
				strcpy(partionInfoTmp.mountdir, partionInfo->partpath);
				partionInfoTmp.totalSpace = partionInfo->partsize;
				partionInfoTmp.freeSpace = partionInfo->freespace;				
				partionInfoList.AddTail(partionInfoTmp);
			}			
			diskUseList.GetNext(pos);
		}


		pos = allDiskInfoList.GetHeadPosition();
		while (NULL != pos)
		{

			DISK_INFO &diskInfo = allDiskInfoList.GetAt(pos);

			if ((NULL != pDiskName) && (0 != strcmp(diskInfo.diskname, pDiskName)))
			{
				diskUseList.GetNext(pos);
				continue;
			}
			for (int i = 0; i < diskInfo.partitionNum; ++i)
			{
				DVR_DISK_INFO partionInfoTmp;
				PART_INFO *partionInfo = &diskInfo.partitionInfo[i];
				partionInfoTmp.diskType = diskInfo.diskType;
				partionInfoTmp.diskInterface = diskInfo.diskInterfacetype;				
				strcpy(partionInfoTmp.diskname, partionInfo->devnod);
				partionInfoTmp.totalSpace = partionInfo->partsize;
				if (NET_DISK_IS_MOUNT == diskInfo.diskStatus)
				{
					partionInfoTmp.freeSpace = partionInfo->freespace;
				}
				else
				{
					partionInfoTmp.freeSpace = diskInfo.disksize;
				}				
				partionInfoTmp.diskStatus = NET_DISK_NO_MOUNT;
				partionInfoList.AddTail(partionInfoTmp);				
			}			
			allDiskInfoList.GetNext(pos);
		}

		nRet = partionInfoList.GetCount();

		pos = allDiskInfoList.GetHeadPosition();
		while (NULL != pos)
		{
			DISK_INFO &diskInfo = allDiskInfoList.GetAt(pos);

			UnMountRemovableDisk(diskInfo);
			allDiskInfoList.GetNext(pos);
		}

		diskUseList.RemoveAll();
		allDiskInfoList.RemoveAll();
	}
	else
	{
		assert(false);
	}

	return nRet;

}

//
//int CDiskManager::GetMountedPartionNum(DISK_TYPE nDiskType)
//{
//	int nRet = 0;
//	if (NET_DISK_TYPE_FIXEDDISK == nDiskType)
//	{
//		nRet = m_nFixedEndPartId - m_nFixedStartPartId;
//		if (nRet < 0)
//		{
//			nRet = 0;
//		}		
//	}
//	return nRet;
//}


int CDiskManager::UnMount(DISK_TYPE nDiskType, char *diskname)
{
	if (NET_DISK_TYPE_REMOVEABLE == nDiskType)
	{		
		POS pos = m_RemoveableDiskList.GetHeadPosition();
		while (NULL != pos)
		{
			DISK_INFO &diskInfo = m_RemoveableDiskList.GetAt(pos);
			if (0 == strcmp(diskname, diskInfo.diskname))
			{
				UnMountRemovableDisk(diskInfo);				
				m_RemoveableDiskList.RemoveAt(pos);
				break;
			}
			m_RemoveableDiskList.GetNext(pos);
		}

		return 0;
	}
	else
	{
		assert(false);
	}

	return 0;


}

int CDiskManager::GetDiskOwner(DISK_TYPE nDiskType, DVR_DISK_OWNER_LIST &diskOwnerList)
{
	int num = 0;
	if (NET_DISK_TYPE_FIXEDDISK == nDiskType)
	{
		if (m_bHaveThisDvrFixedDisk)
		{
			diskOwnerList.AddTail(m_thisDvrDiskOwner);
			num++;
		}

		POS pos = m_otherDvrInfoList.GetHeadPosition();
		while (NULL != pos)
		{
			DVR_DISK_OWNER &owner = m_otherDvrInfoList.GetAt(pos);
			diskOwnerList.AddTail(owner);
			num++;
			m_otherDvrInfoList.GetNext(pos);
		}

		return num;
	}
	else
	{
		assert(false);
	}
}

int CDiskManager::GetOwnerInfo(BYTE diskDvrIndex, DVR_DISK_OWNER &diskOwnerInfo)
{
	int ret = -1;
	if (NET_DISK_OWNED_BY_THIS == diskDvrIndex)
	{
		diskOwnerInfo.dvrIndex = NET_DISK_OWNED_BY_THIS;
		memcpy(diskOwnerInfo.dvrId, m_DiskPara.dvrId, 8);
		diskOwnerInfo.dvrPara = m_DiskPara.videoInputNum;
		ret = 0;
	}
	else
	{
		POS pos = m_otherDvrInfoList.GetHeadPosition();
		while (NULL != pos)
		{
			DVR_DISK_OWNER &owner = m_otherDvrInfoList.GetAt(pos);
			if (owner.dvrIndex == diskDvrIndex)
			{
				diskOwnerInfo = owner;
				ret = 0;
				break;
			}
			m_otherDvrInfoList.GetNext(pos);
		}

		if (0 != ret)
		{
			assert(false);
		}
	}
	
	return ret;

}

void CDiskManager::ProcessDiskSleep(ULONGLONG heightMask, ULONGLONG lowMask)
{
	//printf("check partition for disk sleep --");
	//PrintfBit32Time(GetCurrTime32());	

	{
		POS healthPos = m_healthFixedDiskList.GetHeadPosition();
		while (healthPos != NULL)
		{
			DISK_HEALTH_INFO &diskHealthInfo = m_healthFixedDiskList.GetNext(healthPos);
			diskHealthInfo.bSleep = 0;
		}
	}

	POS pos = m_FixedDiskList.GetHeadPosition();
	while (NULL != pos)
	{
		DISK_INFO &diskInfo = m_FixedDiskList.GetAt(pos);
		if (NET_DISK_NO_FORMAT == diskInfo.diskStatus)
		{
			//printf("sleep no format disk %s--", diskInfo.diskname);
			//PrintfBit32Time(GetCurrTime32());			
			Set_Sata_Sleep(diskInfo.diskname);

		}
		m_FixedDiskList.GetNext(pos);
	}

	for (int count = 0; count < m_nFixedEndPartId; count+=64)
	{
		ULONGLONG partMask = lowMask;
		if (count >= 64)
		{
			partMask = heightMask;
		}
		for (int nPartIndex = count; (nPartIndex < 64 + count)&&(nPartIndex < m_nFixedEndPartId); nPartIndex+=m_DiskPara.partitionNum)
		{
			int i = 0;
			for (i = nPartIndex; i < m_DiskPara.partitionNum + nPartIndex; ++i)
			{
				if ((partMask >> i) & ((ULONGLONG)1))
				{
					break;
				}
			}

			if (i == nPartIndex + m_DiskPara.partitionNum)
			{
				POS pos = m_mountDirToDisk[nPartIndex];
				DISK_INFO &diskInfo = m_FixedDiskList.GetAt(pos);
				//printf("from no use partition:/mnt/%02d --- %02d ", nPartIndex, nPartIndex+m_DiskPara.partitionNum - 1);
				//printf("to sleep disk %s --",diskInfo.diskname);
				//PrintfBit32Time(GetCurrTime32());
				Set_Sata_Sleep(diskInfo.diskname);
			}		
		}

	}	

}


void CDiskManager::Quit()
{
	POS pos = NULL;
	pos = m_FixedDiskList.GetHeadPosition();
	while (pos != NULL)
	{
		DISK_INFO &diskInfo = m_FixedDiskList.GetAt(pos);
		UnMountFixedDisk(diskInfo);	
		Set_Sata_Sleep(diskInfo.diskname);
		m_FixedDiskList.GetNext(pos);
	}
	m_FixedDiskList.RemoveAll();
	m_nFixedStartPartId = FIXEDDISK_BEGIN_MOUNT_DIR;
	m_nFixedEndPartId   = m_nFixedStartPartId;


	pos = m_RemoveableDiskList.GetHeadPosition();
	while (pos != NULL)
	{
		DISK_INFO &diskInfo = m_RemoveableDiskList.GetAt(pos);
		UnMountRemovableDisk(diskInfo);		
		m_RemoveableDiskList.GetNext(pos);
	}
	m_RemoveableDiskList.RemoveAll();
    m_RemoveableDiskStartMountDir = REMOVEABLEDISK_BEGIN_MOUNT_DIR;
	memset(m_bMountDirUsed, 0, sizeof(m_bMountDirUsed));

}


bool CDiskManager::ProcessDiskHealth(DISK_HEALTH_INFO_LIST &warningList)
{
	if (m_healthFixedDiskList.IsEmpty())
	{
		return true;
	}

	POS prePos = NULL;
	POS pos = m_healthFixedDiskList.GetHeadPosition();
	while (NULL != pos)
	{
		prePos = pos;
		DISK_HEALTH_INFO &diskHealth = m_healthFixedDiskList.GetNext(pos);
		if (diskHealth.bSleep)
		{
			continue;
		}
		if (!GetDiskModuleImp(diskHealth.szDiskName, diskHealth.diskSmartInfo))
		{
			continue;
		}

		if (!GetDiskSmartInfoImp(diskHealth.szDiskName, diskHealth.diskSmartInfo))
		{
			continue;
		}

		if (diskHealth.diskSmartInfo.diskHealth > 0)
		{
			warningList.AddTail(diskHealth);
			m_healthFixedDiskList.RemoveAt(prePos);
		}	
	}

	return true;
}


bool CDiskManager::GetDiskSmartInfo(char *pDiskName, NET_DISK_SMART_INFO &stInfo)
{
	bool bRet = false;
	
	memset(&stInfo, 0, sizeof(stInfo));
	if (!GetDiskModuleImp(pDiskName, stInfo))
	{
		return false;
	}

	if (!GetDiskSmartInfoImp(pDiskName, stInfo))
	{
		return false;
	}

	return true;
}

//////下面是对对固定硬盘的操作
#if  defined(__VERY_BIG_DISK__)
 int CDiskManager::GetFixedDiskInfo(char *diskname, PART_INFO *pPartitionInfo, BYTE *pPartitionNum,bool bBigDiskFlag)
{
	int fd;
    char szMbrBuffer[BOOT_LENGTH] = {0};
	struct partition* pParttemp = NULL;
	bool bigDiskFlag = false;
	
	//fd = open( diskname, O_RDWR );
	//if ( fd < 0 )
	//{
	//	return -1;
	//}
	//
	//llseek( fd, 0,SEEK_SET );

	//if ( BOOT_LENGTH != read(fd, szMbrBuffer, BOOT_LENGTH) )
	//{
	//   close(fd);
	//	 return -1;
	//}
	//close(fd);

	if ( true == bBigDiskFlag )
	{                                                                                                   
		return GetGptParttionInfo(diskname,  pPartitionInfo, pPartitionNum);
	}
	else
	{
		return GetParttionInfo(diskname,  pPartitionInfo, pPartitionNum);

	}
	
}
#else


int CDiskManager::GetFixedDiskInfo(char *diskname, PART_INFO *pPartitionInfo, BYTE *pPartitionNum,bool bBigDiskFlag)
{
	if (GetParttionInfo(diskname,  pPartitionInfo, pPartitionNum) < 0)
	{
		return -1;
	}

	return 0;
}
#endif


int CDiskManager::MountFixedDisk(DISK_INFO &diskInfo, int &startPartId, bool bCreateLogFile, unsigned long lAviNum, unsigned long lEventLogSizeM, unsigned long lOperLogSizeM)
{
	diskInfo.diskOwnedBy  = NET_DISK_OWNED_BY_THIS;
	diskInfo.diskStatus   = NET_DISK_NO_FORMAT;	
	diskInfo.diskProperty = NET_DISK_READ_WRITE;

	if (diskInfo.partitionNum != 4)
	{
		return -1;
	}

	for (int i = 0; i < diskInfo.partitionNum; ++i)
	{
		if (diskInfo.partitionInfo[i].partformat != FAT32)
		{
			return -1;
		}
	}

	MakeNode(diskInfo.diskname, m_DiskPara.szDiskMountPath, startPartId, diskInfo.partitionNum);


	//下面进行挂载操作
	BYTE nStart = startPartId;
	char szCmd[MAX_PATH] = {0};
	for (int i = 0; i < diskInfo.partitionNum; ++i)
	{
		PART_INFO *p = &diskInfo.partitionInfo[i];
		p->partid = nStart;
		snprintf(p->partpath, sizeof(p->partpath), "%s/%02d", m_DiskPara.szDiskMountPath, nStart);		
		//先卸载，然后再挂载
		memset(szCmd, 0x00, sizeof(szCmd));
		snprintf(szCmd, sizeof(szCmd), "umount %s", p->partpath);
		DVRSystem(szCmd);
		memset(szCmd, 0x00, MAX_PATH);
		snprintf(szCmd, MAX_PATH, "mount -t vfat -o usefree %s %s ", p->devnod, p->partpath);
	
		if (DVRSystem(szCmd) < 0)
		{
			printf("%s failed\n",szCmd);
			return -1;
		}
		else
		{
			printf("%s successed\n",szCmd);		
		}
		nStart++;
	}

	if (bCreateLogFile)
	{
		for (int i = 0; i < diskInfo.partitionNum; ++i)
		{
			char szTmp[MAX_PATH] = {0};
			PART_INFO *p = &diskInfo.partitionInfo[i];
			FILE *f = NULL;
			unsigned long num = 0;

			snprintf(szTmp, MAX_PATH, "rm -rf %s/temprec.dat", p->partpath);
			DVRSystem(szTmp);

			snprintf(szTmp, MAX_PATH, "rm -rf %s/templog.bin", p->partpath);
			DVRSystem(szTmp);

			snprintf(szTmp, MAX_PATH, "%s/reclog.bin", p->partpath);
#if defined(__SUPPORT_HONGDI__)
			unsigned long JPEG_FILE_NUM_COUNT = (lAviNum / 5) * 2;
			if (!CReclogEx::CreateLogFile(szTmp, (lAviNum - JPEG_FILE_NUM_COUNT), m_DiskPara.videoInputNum))
			{
				return -1;
			}
			snprintf(szTmp, MAX_PATH, "%s/PictMan.dat", p->partpath);
			if (!CSnapMan::CreateLogFile(szTmp, JPEG_FILE_NUM_COUNT, (lAviNum - JPEG_FILE_NUM_COUNT), 1000))
			{
				printf("CSnapMan CreateLogFileFaild\n");
				return -1;
			}

#else
			if (!CReclogEx::CreateLogFile(szTmp, (lAviNum - JPEG_FILE_NUM), m_DiskPara.videoInputNum))
			{
				return -1;
			}
			snprintf(szTmp, MAX_PATH, "%s/PictMan.dat", p->partpath);
			if (!CSnapMan::CreateLogFile(szTmp, JPEG_FILE_NUM, (lAviNum - JPEG_FILE_NUM), 500))
			{
				printf("CSnapMan CreateLogFileFaild\n");
				return -1;
			}

#endif

			num = (lEventLogSizeM*1024*1024)/(sizeof(EVENT_LOG));
			snprintf(szTmp, MAX_PATH, "%s/eventlog.bin", p->partpath);
			if (!CLog::CreateLogFile(szTmp, sizeof(EVENT_LOG), num))
			{
				return -1;
			}

			num = (lOperLogSizeM*1024*1024)/(sizeof(OPERATOR_LOG));
			snprintf(szTmp, MAX_PATH, "%s/operlog.bin", p->partpath);
			if (!CLog::CreateLogFile(szTmp, sizeof(OPERATOR_LOG), num))
			{
				return -1;
			}
		
			
			snprintf(szTmp, MAX_PATH, "%s/diskver", p->partpath);
			f = fopen(szTmp, "wb");
			if (NULL == f)
			{
				return -1;
			}
           
			memset(szTmp, 0, sizeof(szTmp));
			snprintf(szTmp, 256, "%s", m_diskVersion);
			fwrite(szTmp, sizeof(char), 256, f);

			memset(szTmp, 0, sizeof(szTmp));
			snprintf(szTmp, 256, "%s%d", DISK_PRODUCT_NAME, m_DiskPara.productId);
			fwrite(szTmp, sizeof(char), 256, f);
			
			memset(szTmp, 0, sizeof(szTmp));
			snprintf(szTmp, sizeof(DISK_ID), "%s", DISK_ID);
            memcpy(&szTmp[sizeof(DISK_ID)], m_DiskPara.dvrId, 8);
			fwrite(szTmp, sizeof(char), 256, f);

			memset(szTmp, 0, sizeof(szTmp));
			snprintf(szTmp, 256, "%s%d", DISK_ACCESS_PROPERTY, NET_DISK_READ_WRITE);
			fwrite(szTmp, sizeof(char), 256, f);
			fflush(f);
			fclose(f);	
			sync();  //因为这个文件会用于判断，硬盘是否格式了。
			         //所以需要把缓存的东西都写入硬盘
		}
	}
	else
	{
		BYTE dvrIdTmp[8] = {0};
		BYTE dvrPropertyTmp = 0;
		

		for (int i = 0; i < diskInfo.partitionNum; ++i)
		{
			char szTmp[MAX_PATH] = {0};
			char szRead[MAX_PATH] = {0};
			PART_INFO *p = &diskInfo.partitionInfo[i];
			FILE *f = NULL;

			snprintf(szTmp, MAX_PATH, "rm -rf %s/HtmlData/*", p->partpath);
			printf("########szTmp:%s\n",szTmp);
			DVRSystem(szTmp);

			snprintf(szTmp, MAX_PATH, "%s/PictMan.dat", p->partpath);
			printf("open %s ",szTmp);
			f = fopen(szTmp, "rb");
			if (NULL == f)
			{
				printf("no %s\n",szTmp);
				snprintf(szTmp, MAX_PATH, "%s/00999999.dat", p->partpath);
				FILE *fdate = fopen(szTmp, "wb");
				fclose(fdate);

				snprintf(szTmp, MAX_PATH, "%s/PictMan.dat", p->partpath);
				if (!CSnapMan::CreateLogFile(szTmp, 1, 999999, 500))
				{
					printf("CSnapMan CreateLogFileFaild\n");					
				}
			}
			else
			{
				printf("ok \n");
				fclose(f);
				f = NULL;
			}
		


			snprintf(szTmp, MAX_PATH, "%s/reclog.bin", p->partpath);
			printf("open %s ",szTmp);
			f = fopen(szTmp, "rb");
			if (NULL == f)
			{
				printf("error\n",szTmp);
				return -1;
			}
			printf("ok \n");
			fclose(f);
			f = NULL;

			snprintf(szTmp, MAX_PATH, "%s/eventlog.bin", p->partpath);
			printf("open %s ",szTmp);
			f = fopen(szTmp, "rb");
			if (NULL == f)
			{
				printf("error\n",szTmp);
				return -1;
			}
			printf("ok \n");
			fclose(f);
			f = NULL;

			snprintf(szTmp, MAX_PATH, "%s/operlog.bin", p->partpath);
			printf("open %s ",szTmp);
			f = fopen(szTmp, "rb");
			if (NULL == f)
			{
				printf("error\n",szTmp);
				return -1;
			}
			printf("ok \n");
			fclose(f); 
			f = NULL;

			snprintf(szTmp, MAX_PATH, "%s/diskver", p->partpath);
			printf("open %s ",szTmp);
			f = fopen(szTmp, "rb");
			if (NULL == f)
			{
				printf("error\n",szTmp);
				return -1;
			}
			printf("ok\n");
			
			memset(szRead, 0, sizeof(szRead));
			if (256 != fread(szRead, sizeof(char), 256, f))
			{
				fclose(f);
				f = NULL;
				return -1;
			}
			memset(szTmp, 0, sizeof(szTmp));
			snprintf(szTmp, 256, "%s", m_diskVersion);
			if (0 != strncmp(szTmp, szRead, 256))
			{
				fclose(f);
				f = NULL;
				return -1;
			}

			memset(szRead, 0, sizeof(szRead));
			if (256 != fread(szRead, sizeof(char), 256, f))
			{
				fclose(f);
				f = NULL;
				return -1;
			}
			
			if (g_bRu042304SEMETo2504HE_M)
			{
				char sz2304SE[MAX_PATH] = {0};
				BYTE b2304SEID          = TD_2304SE;
				bool b2304SE            = false;

				char sz2304ME[MAX_PATH] = {0};				
				BYTE b2304MEID          = TD_2304ME;
				bool b2304ME            = false;

				char sz2504HE[MAX_PATH]  = {0};
				BYTE b2504HEID           = TD_2504HE_M;				
				bool b2504HE             = false;

				snprintf(sz2304SE, 256, "%s%d", DISK_PRODUCT_NAME, b2304SEID);
				snprintf(sz2304ME, 256, "%s%d", DISK_PRODUCT_NAME, b2304MEID);
				snprintf(sz2504HE, 256, "%s%d", DISK_PRODUCT_NAME, b2504HEID);

				printf("%s,%d, productId:%d, szRead:%s\n",__FILE__,__LINE__,m_DiskPara.productId,szRead);
				if (0 != strncmp(sz2304SE, szRead, 256))
				{
					printf("cmp 2304SE faild\n");
					b2304SE = false;
				}
				else
				{
					printf("cmp 2304SE ok\n");
					b2304SE = true;
				}

				if (0 != strncmp(sz2304ME, szRead, 256))
				{
					printf("cmp 2304ME faild\n");
					b2304ME = false;
				}
				else
				{
					printf("cmp 2304ME ok\n");
					b2304ME = true;
				}

				if (0 != strncmp(sz2504HE, szRead, 256))
				{
					printf("cmp 2504HE faild\n");
					b2504HE = false;
				}
				else
				{
					printf("cmp 2504HE ok\n");
					b2504HE = true;
				}

				if (!b2304SE && !b2304ME && !b2504HE)
				{
					fclose(f);
					f = NULL;
					return -1;
				}
			}
			else
			{
				PRODUCT_TYPE productType = CProduct::Instance()->ProductType();
				if ((TD_2316ME == productType) || (TD_2316SE == productType))
				{
					printf("%s,%d, productId:%d, szRead:%s\n",__FILE__,__LINE__,m_DiskPara.productId,szRead);
					char sz2316ME[MAX_PATH] = {0};
					char sz2316SE[MAX_PATH] = {0};
					bool b2316ME = false;
					bool b2316SE = false;
					BYTE b2316MEID = TD_2316ME;
					BYTE b2316SEID = TD_2316SE;
					snprintf(sz2316ME, 256, "%s%d", DISK_PRODUCT_NAME, b2316MEID);
					snprintf(sz2316SE, 256, "%s%d", DISK_PRODUCT_NAME, b2316SEID);
					if (0 != strncmp(sz2316ME, szRead, 256))
					{
						printf("cmp 2316ME faild\n");
						b2316ME = false;
					}
					else
					{
						printf("cmp 2316ME ok\n");
						b2316ME = true;
					}

					if (0 != strncmp(sz2316SE, szRead, 256))
					{
						printf("cmp 2316SE faild\n");
						b2316SE = false;
					}
					else
					{
						printf("cmp 2316SE ok\n");
						b2316ME = true;
					}

					if (!b2316ME && !b2316SE)
					{
						fclose(f);
						f = NULL;
						return -1;
					}
				}
				else
				{
					memset(szTmp, 0, sizeof(szTmp));
					snprintf(szTmp, 256, "%s%d", DISK_PRODUCT_NAME, m_DiskPara.productId);
					if (0 != strncmp(szTmp, szRead, 256))
					{
						fclose(f);
						f = NULL;
						return -1;
					}
				}
			}
						
			memset(szRead, 0, sizeof(szRead));
			if (256 != fread(szRead, sizeof(char), 256, f))
			{
				fclose(f);
				f = NULL;
				return -1;
			}
			memset(szTmp, 0, sizeof(szTmp));
			if (256 != fread(szTmp, sizeof(char), 256, f))
			{
				fclose(f);
				f = NULL;
				return -1;
			}
			if (0 == i)
            {
				memcpy(dvrIdTmp, &szRead[sizeof(DISK_ID)], 8);
				dvrPropertyTmp = szTmp[sizeof(DISK_ACCESS_PROPERTY) - 1] - '0';
			}
			else
			{
				if (0 != DvrIdnCmp(dvrIdTmp, (BYTE*)(&szRead[sizeof(DISK_ID)]), 8))
				{
					fclose(f);
					f = NULL;
					return -1;
				}

				if (dvrPropertyTmp != (szTmp[sizeof(DISK_ACCESS_PROPERTY) - 1] - '0'))
				{
					fclose(f);
					f = NULL;
					return -1;
				}
			}
			
			fclose(f);
			f = NULL;						
		}

		if (0 == DvrIdnCmp(m_DiskPara.dvrId, dvrIdTmp, 8))
		{
			diskInfo.diskOwnedBy = NET_DISK_OWNED_BY_THIS;
			diskInfo.diskProperty = dvrPropertyTmp;

			if (diskInfo.diskProperty > 2)
			{
				assert(false);
				diskInfo.diskProperty = 0;
			}

			if (!m_bHaveThisDvrFixedDisk)
			{
				memset(&m_thisDvrDiskOwner, 0, sizeof(DVR_DISK_OWNER));
				m_bHaveThisDvrFixedDisk = true;
				m_thisDvrDiskOwner.dvrIndex = NET_DISK_OWNED_BY_THIS;
				memcpy(m_thisDvrDiskOwner.dvrId, m_DiskPara.dvrId, 8);
				m_thisDvrDiskOwner.dvrPara = m_DiskPara.videoInputNum;
			}
			m_thisDvrDiskOwner.diskNum++;
		}
		else
		{
			//如果产品类型相同，但是产品标示（mac地址）不同，说明这个硬盘是别的设备上的，那么这个硬盘只能读，不能写
			diskInfo.diskProperty = NET_DISK_ONLY_READ;

			BYTE nextOtherDvrIndex = NET_DISK_OWNED_BY_OTHER_1 + m_otherDvrInfoList.GetCount();
			POS pos = m_otherDvrInfoList.GetHeadPosition();
			while (NULL != pos)
			{
				DVR_DISK_OWNER &otherDvrInfo = m_otherDvrInfoList.GetAt(pos);
				if (0 == DvrIdnCmp(otherDvrInfo.dvrId, dvrIdTmp, 8))
				{
					diskInfo.diskOwnedBy = otherDvrInfo.dvrIndex;
					otherDvrInfo.diskNum++;
					break;
				}
				m_otherDvrInfoList.GetNext(pos);
			}
			if (NULL == pos)
			{
				DVR_DISK_OWNER otherDvrInfo;
				memset(&otherDvrInfo, 0, sizeof(DVR_DISK_OWNER));
				otherDvrInfo.dvrIndex = nextOtherDvrIndex;
				memcpy(otherDvrInfo.dvrId, dvrIdTmp, 8);
				otherDvrInfo.dvrPara = diskInfo.partitionInfo[0].partid;
				otherDvrInfo.diskNum++;
				m_otherDvrInfoList.AddTail(otherDvrInfo);
				diskInfo.diskOwnedBy = nextOtherDvrIndex;					
			}
		}

	}

	
	//到此处，硬盘可以用来录像了
	diskInfo.diskStatus = NET_DISK_NORMAL;

	startPartId = nStart;   
	return 0;

}

int CDiskManager::FormatFixedDisk(char *diskname, MESSAGE_CALLBACK_EX PercentCallBack, void *pPara)
{
	unsigned long lAviNum = 0;
	unsigned long lEventLogSizeM = 0;
	unsigned long lOperLogSizeM = 0;
    int progress = 0;

	PercentCallBack(pPara, CALL_BACK_MODULE_DISK_MAN, CALL_BACK_MSG_PROGRESS, &progress, NULL);

	POS pos = m_FixedDiskList.GetHeadPosition();
	while (pos != NULL)
	{
		DISK_INFO &diskInfo = m_FixedDiskList.GetAt(pos);
		if (0 == strcmp(diskInfo.diskname, diskname))
		{
			break;
		}
		m_FixedDiskList.GetNext(pos);
	}
	
	if (NULL == pos)
	{
		PercentCallBack(pPara, CALL_BACK_MODULE_DISK_MAN, CALL_BACK_MSG_FAIL, NULL, NULL);
		return -1;
	}

	DISK_INFO &diskInfo = m_FixedDiskList.GetAt(pos);
	int nMountDir = 0;
	bool bFormated = false;
	unsigned long diskOwnerIndex = 0;
	if (NET_DISK_NORMAL == diskInfo.diskStatus)
	{
		bFormated = true;
		nMountDir = diskInfo.partitionInfo[0].partid;
		diskOwnerIndex = diskInfo.diskOwnedBy;
	}
	else
	{
		assert (NET_DISK_OWNED_BY_THIS == diskInfo.diskOwnedBy);
		diskOwnerIndex = diskInfo.diskOwnedBy;
		bFormated = false;
		nMountDir = m_nFixedEndPartId;
	}

	if (bFormated)
	{
		char szCmd[256] = {0};
		for (int i = 0; i < diskInfo.partitionNum; ++i)
		{
			PART_INFO *p = &diskInfo.partitionInfo[i];
			snprintf(szCmd, 256, "rm -rf %s/disver", p->partpath);
			DVRSystem(szCmd);
			snprintf(szCmd, 256, "rm -rf %s/*.bin", p->partpath);
			DVRSystem(szCmd);
		}
	}	
	
	UnMountFixedDisk(diskInfo);

	progress = 5;
	PercentCallBack(pPara, CALL_BACK_MODULE_DISK_MAN, CALL_BACK_MSG_PROGRESS, &progress, NULL);

	diskInfo.partitionNum = 0;
	memset(diskInfo.partitionInfo, 0, sizeof(diskInfo.partitionInfo));
	bool bBIgDiskCheck=false;
	if(diskInfo.disksize > 2*1024*1024)
	{
		bBIgDiskCheck =true;//大硬盘
	}
	else
	{
		bBIgDiskCheck =false;
	}
	if (FormatDiskImp(diskInfo.diskname, 4, m_DiskPara.fileSizeM, lAviNum, lEventLogSizeM, lOperLogSizeM, PercentCallBack, pPara,bBIgDiskCheck) < 0)
	{
		PercentCallBack(pPara, CALL_BACK_MODULE_DISK_MAN, CALL_BACK_MSG_FAIL, NULL, NULL);
		return -1;
	}

	progress = 90;
	PercentCallBack(pPara, CALL_BACK_MODULE_DISK_MAN, CALL_BACK_MSG_PROGRESS, &progress, NULL);

	if (GetFixedDiskInfo(diskInfo.diskname, diskInfo.partitionInfo, &diskInfo.partitionNum,bBIgDiskCheck) < 0)
	{
		PercentCallBack(pPara, CALL_BACK_MODULE_DISK_MAN, CALL_BACK_MSG_FAIL, NULL, NULL);
		return -1;
	}

	progress = 95;
	PercentCallBack(pPara, CALL_BACK_MODULE_DISK_MAN, CALL_BACK_MSG_PROGRESS, &progress, NULL);

	if (MountFixedDisk(diskInfo, nMountDir, true, lAviNum, lEventLogSizeM, lOperLogSizeM) < 0)
	{
		PercentCallBack(pPara, CALL_BACK_MODULE_DISK_MAN, CALL_BACK_MSG_FAIL, NULL, NULL);
		return -1;
	}

	if (NET_DISK_OWNED_BY_THIS != diskOwnerIndex)
	{
		POS pos = m_otherDvrInfoList.GetHeadPosition();
		while (NULL != pos)
		{
			DVR_DISK_OWNER &owner = m_otherDvrInfoList.GetAt(pos);
			if (owner.dvrIndex == diskOwnerIndex)
			{
				owner.diskNum--;
				if (0 == owner.diskNum)
				{
					m_otherDvrInfoList.RemoveAt(pos);
				}
				if (!m_bHaveThisDvrFixedDisk)
				{
					memset(&m_thisDvrDiskOwner, 0, sizeof(DVR_DISK_OWNER));
					m_bHaveThisDvrFixedDisk = true;
					m_thisDvrDiskOwner.dvrIndex = NET_DISK_OWNED_BY_THIS;
					memcpy(m_thisDvrDiskOwner.dvrId, m_DiskPara.dvrId, 8);
					m_thisDvrDiskOwner.dvrPara = m_DiskPara.videoInputNum;
				}
				m_thisDvrDiskOwner.diskNum++;
				break;
			}
			m_otherDvrInfoList.GetNext(pos);
		}
		assert(NULL != pos);

	}

	if (!bFormated)
	{
		m_nFixedEndPartId = nMountDir;

		if (!m_bHaveThisDvrFixedDisk)
		{
			memset(&m_thisDvrDiskOwner, 0, sizeof(DVR_DISK_OWNER));
			m_bHaveThisDvrFixedDisk = true;
			m_thisDvrDiskOwner.dvrIndex = NET_DISK_OWNED_BY_THIS;
			memcpy(m_thisDvrDiskOwner.dvrId, m_DiskPara.dvrId, 8);
			m_thisDvrDiskOwner.dvrPara = m_DiskPara.videoInputNum;
		}
		m_thisDvrDiskOwner.diskNum++;
	}

	//获取分区与硬盘的对应关系
	assert(m_nFixedEndPartId <= FIXEDDISK_MAX_MOUNT_DIR);
	pos = m_FixedDiskList.GetHeadPosition();
	while (pos != NULL)
	{
		DISK_INFO &diskInfo = m_FixedDiskList.GetAt(pos);
		if (NET_DISK_NO_FORMAT == diskInfo.diskStatus)
		{
			m_FixedDiskList.GetNext(pos);	
			continue;					
		}

		for (int i = 0; i < diskInfo.partitionNum; ++i)
		{
			m_mountDirToDisk[diskInfo.partitionInfo[i].partid] = pos;			
		}

		m_FixedDiskList.GetNext(pos);
	}
	for (int i = 0;i < m_nFixedEndPartId; ++i)
	{
		DISK_INFO &diskInfo = m_FixedDiskList.GetAt(m_mountDirToDisk[i]);
		printf("%s/%02d === %s\n", m_DiskPara.szDiskMountPath, i, diskInfo.diskname);
	}

	progress = 100;
	PercentCallBack(pPara, CALL_BACK_MODULE_DISK_MAN, CALL_BACK_MSG_PROGRESS, &progress, NULL);

	return 0;

}

int CDiskManager::UnMountFixedDisk(DISK_INFO &DiskInfo)
{
	char cmd[MAX_PATH] = {0};

	//如果磁盘没有挂载，就没有必要卸载
	if (NET_DISK_NORMAL != DiskInfo.diskStatus)
	{
		return 0;
	}

	//卸载每一个分区
	for (int i = 0; i < DiskInfo.partitionNum; ++i)
	{
		PART_INFO *p = &DiskInfo.partitionInfo[i];
		snprintf(cmd, MAX_PATH, "umount  %s ", p->partpath);
	    if (0 != DVRSystem(cmd))
		{
			printf("Umount %s  failed\n",p->partpath);
		}
		else
		{
			printf("Umount %s  successed\n",p->partpath);
		}
	}

	
	//设置该设备已挂载
	DiskInfo.diskStatus = NET_DISK_NO_FORMAT;
	return 0;
}

////下面这部分是对热拔插设备操作
int CDiskManager::GetRemovableDiskInfo(char *diskname,  PART_INFO *pPartitionInfo, BYTE *pPartitionNum, unsigned long sizeM)
{

	char szCmd[MAX_PATH] = {0};
	
	memset(szCmd, 0x00, MAX_PATH);
	snprintf(szCmd, MAX_PATH, "rm -rf /rem_tmp");
	DVRSystem(szCmd);
	memset(szCmd, 0x00, MAX_PATH);
	snprintf(szCmd, MAX_PATH, "mkdir -p /rem_tmp ");
	DVRSystem(szCmd);	
	
	memset(szCmd, 0x00, MAX_PATH);
	snprintf(szCmd, MAX_PATH, "umount /rem_tmp");
	DVRSystem(szCmd);

	memset(szCmd, 0x00, MAX_PATH);
	snprintf(szCmd, MAX_PATH, "mount -t vfat  -o usefree %s /rem_tmp ", diskname);
	if (0 == DVRSystem(szCmd))  
	{
		pPartitionInfo[0].partsize = sizeM;	
		snprintf(pPartitionInfo[0].devnod, sizeof(pPartitionInfo[0].devnod), "%s", diskname);
		*pPartitionNum = 1;

		memset(szCmd, 0x00, MAX_PATH);
		snprintf(szCmd, MAX_PATH, "umount /rem_tmp ");
		DVRSystem(szCmd);

		memset(szCmd, 0x00, MAX_PATH);
		snprintf(szCmd, MAX_PATH, "rm -rf /rem_tmp ");
		DVRSystem(szCmd);

		return 0;

	}

	memset(szCmd, 0x00, MAX_PATH);
	snprintf(szCmd, MAX_PATH, "rm -rf /rem_tmp");
	DVRSystem(szCmd);

	if (GetParttionInfo(diskname,  pPartitionInfo, pPartitionNum) >= 0)
	{
		return 0;
	}
	return -1;	
	
}

int CDiskManager::FindRemovealbeDiskMountDir(int *pMountDir, int partitionNum)
{
	assert(partitionNum > 0);

	printf("partitionNum:%d\n", partitionNum);
	int i = 0;
	int j = 0;
	for (i = 0; i < REMOVEABLEDISK_MAX_MOUNT_DIR; )
	{
		printf("m_bMountDirUsed[%d]:%d ", i, m_bMountDirUsed[i]);
		if (!m_bMountDirUsed[i])
		{
			for (j = i + 1; (j < REMOVEABLEDISK_MAX_MOUNT_DIR) && (j < partitionNum + i); ++j)
			{
				printf("m_bMountDirUsed[%d]:%d ", j , m_bMountDirUsed[j]);
				if (m_bMountDirUsed[j])
				{
					break;
				}
			}

			if (j == (partitionNum + i))
			{
				printf("find \n");
				*pMountDir = m_RemoveableDiskStartMountDir + i;
				return 0;
			}
			else if (j == REMOVEABLEDISK_MAX_MOUNT_DIR)
			{				
				printf("no find \n");
				return -1;
			}
			else
			{
				printf("\n");
				i = i + partitionNum;				
		    }			
		}
		else
		{
			++i;
		}
	}

	return -1;
}



int CDiskManager::MountRemovableDisk(DISK_INFO &diskInfo, int &startPartId)
{

	MakeNode(diskInfo.diskname, m_DiskPara.szDiskMountPath, startPartId, diskInfo.partitionNum);

	//下面进行挂载操作
	diskInfo.diskStatus = NET_DISK_NO_MOUNT;
    diskInfo.diskProperty = NET_DISK_READ_WRITE;
	BYTE nStart = startPartId;
	char szCmd[MAX_PATH] = {0};

	{
		PART_INFO *pPartionInfoTemp = new PART_INFO [diskInfo.partitionNum];
		int index = 0;
		int indexTemp = 0;
		for (index = 0; index < diskInfo.partitionNum; ++index)
		{
			PART_INFO *p = &diskInfo.partitionInfo[index];
			p->partid = nStart;
			snprintf(p->partpath, sizeof(p->partpath), "%s/%02d", m_DiskPara.szDiskMountPath, nStart);
			//先卸载，然后再挂载
			snprintf(szCmd, sizeof(szCmd), "umount %s", p->partpath);
			DVRSystem(szCmd);
			snprintf(szCmd, sizeof(szCmd), "mount -t vfat  -o usefree %s %s ", p->devnod, p->partpath);
			if (DVRSystem(szCmd) < 0)
			{
				printf("%s failed\n",szCmd);
			}
			else
			{
				printf("%s successed\n",szCmd);	
				nStart++;
				pPartionInfoTemp[indexTemp] = (*p);
				indexTemp++;
			}
		}

		if (0 == indexTemp)
		{
			delete [] pPartionInfoTemp;
			pPartionInfoTemp = NULL;
			return -1;
		}

		diskInfo.partitionNum = indexTemp;
		for (int count = 0; count < indexTemp; ++count)
		{
			diskInfo.partitionInfo[count] = pPartionInfoTemp[count];
		}

		delete [] pPartionInfoTemp;
		pPartionInfoTemp = NULL;
	}

   
	//到此处，硬盘可以用来录像了
	diskInfo.diskStatus = NET_DISK_IS_MOUNT;

	GetDiskUseSpace(diskInfo);
	
	for (int i = nStart; i < nStart + diskInfo.partitionNum; ++i)
	{
		m_bMountDirUsed[i] = true;
	}

	startPartId = nStart;
	
	return 0;
}   



int CDiskManager::UnMountRemovableDisk(DISK_INFO &DiskInfo)
{

	//printf("(%s,%d)%s\n",__FILE__,__LINE__,DiskInfo.diskname);
	char cmd[MAX_PATH] = {0};

	//如果磁盘没有挂载，就没有必要卸载
	if (NET_DISK_NO_MOUNT == DiskInfo.diskStatus)
	{
		return 0;
	}

	//卸载每一个分区
	for (int i = 0; i < DiskInfo.partitionNum; ++i)
	{
		PART_INFO *p = &DiskInfo.partitionInfo[i];
		//printf("%s,%s\n",p->devnod,p->partpath);
		snprintf(cmd, MAX_PATH, "umount  %s ", p->partpath);
		if (0 != DVRSystem(cmd))
		{
			printf("Umount %s  failed\n",p->partpath);
		}
		else
		{
			printf("Umount %s  successed\n",p->partpath);
		}
	}

	int start = DiskInfo.partitionInfo[0].partid;
	for (int index = start; index < start + DiskInfo.partitionNum; ++index)
	{
		m_bMountDirUsed[index] = false;
	}

	//设置该设备已挂载
	DiskInfo.diskStatus = NET_DISK_NO_MOUNT;
	return 0;

}



////下面这部分是关于硬盘，U盘一些通用操作
int CDiskManager::FormatDiskImp(char *diskName, unsigned char partitionNum, 
							 unsigned long fileSizeM, unsigned long &lAviNum,
							 unsigned long &lEventLogSizeM,unsigned long &lOperLogSizeM,MESSAGE_CALLBACK_EX PercentCallBack,
							 void *pPara,bool bBigDiskCheck)
{
	int fPart = -1;
	int nIndex = 0;
	char szPartName[MAX_PATH] = {0};
	bool bRet = false;
	unsigned long lAviNumtemp = 0;
	unsigned long lEventLogSizeMtemp = 0;
	unsigned long lOperLogSizeMtemp  = 0;
    int progress  = 0;
	

	lAviNum = 0xffffffff;
	lEventLogSizeM = 0xffffffff;
	lOperLogSizeM = 0xffffffff;


	partitionNum = 4;


	progress = 5;
	PercentCallBack(pPara, CALL_BACK_MODULE_DISK_MAN, CALL_BACK_MSG_PROGRESS, &progress, NULL);
#if defined(__VERY_BIG_DISK__)
	if (SetPartitionInfo(diskName, partitionNum, FAT32, PercentCallBack, pPara, bBigDiskCheck) < 0)
	{
		return -1;
	}
#else	
	if (SetPartitionInfo(diskName, partitionNum, FAT32, PercentCallBack, pPara) < 0)
	{
		return -1;
	}
#endif

	MakeNode(diskName, NULL, 0, partitionNum);

	progress = 10;
	PercentCallBack(pPara, CALL_BACK_MODULE_DISK_MAN, CALL_BACK_MSG_PROGRESS, &progress, NULL);

	for (nIndex = 0; nIndex < partitionNum; nIndex++)
	{
		char szPartName[MAX_PATH] = {0};
		snprintf(szPartName,MAX_PATH,"%s%d", diskName, nIndex+1);

		printf("\nstart format %s ...\n",szPartName);
		
		fPart = open(szPartName, O_RDWR);
		if (fPart < 0)
		{
			printf("open %s fail\n", szPartName);
			return -1;
		}		
#if defined(__VERY_BIG_DISK__)
		bRet = FormatParttion(fPart, fileSizeM*1024*1024, lAviNumtemp, lEventLogSizeMtemp, lOperLogSizeMtemp, bBigDiskCheck);
#else
		bRet = FormatParttion(fPart, fileSizeM*1024*1024, lAviNumtemp, lEventLogSizeMtemp, lOperLogSizeMtemp);		
#endif
		close(fPart);

		if (!bRet)
		{
			printf("format failed\n");
			return -1;
		}

		if (lAviNumtemp < lAviNum)
		{
			lAviNum = lAviNumtemp;
		}
		if (lEventLogSizeMtemp < lEventLogSizeM)
		{
			lEventLogSizeM = lEventLogSizeMtemp;
		}
		if (lOperLogSizeMtemp < lOperLogSizeM)
		{
			lOperLogSizeM = lOperLogSizeMtemp;
		}

		progress += (80/partitionNum);
		PercentCallBack(pPara, CALL_BACK_MODULE_DISK_MAN, CALL_BACK_MSG_PROGRESS, &progress, NULL);
	}

	return 0;
}

#if defined(__VERY_BIG_DISK__)

//大小端存储的转换函数。 
void CDiskManager::ReverseBytes(void* theValue, int numBytes) 
{
	char* tempValue = NULL;
	int i;
	tempValue = new char [numBytes];
	if (tempValue != NULL)
	{
		memcpy(tempValue, theValue, numBytes);
		for ( i = 0; i < numBytes; i++ )
		{
			( (char*) theValue )[i] = tempValue[numBytes - i - 1];
		}

		delete[] tempValue;
	} 
	else
	{
		assert(false);
	}
}

int CDiskManager::GptDiskPartitionProc(char *diskName, unsigned char partitionNum )
{
	int fd;
	unsigned char nIndex = 0;
	unsigned char nLength = 0;
	char szMbrBuffer[BOOT_LENGTH] = {0};
	unsigned long lTotalSectors;             //总扇区数
	struct hd_geometry geometry;             //硬盘的几何参数
	unsigned long lUnits;                    //分区单位：以柱面为单位
	unsigned long lTotalCylinders;           //总共柱面数
	unsigned long lUnitsPerParttion;           
	unsigned long start;                     //在分区过程中记录某个分区的起始扇区
	unsigned long stop;                      //在分区过程中记录某个分区的结束扇区
	unsigned char nMainPartNum;
	unsigned char nExtendPartNum;
	unsigned char szEbrBoot[BOOT_LENGTH] = {0};
	loff_t uExtendBase = 0;   				//扩展分区的基本绝对起始扇区
	loff_t uEbrStart = 0;
	unsigned long uEbrEnd = 0;
	struct __mbr_partition* pParttemp = NULL;


	if ( ( partitionNum <= 0 ) || ( partitionNum > 16 ) )
	{
		partitionNum = 4;
	}

	fd = open( diskName, O_RDWR );
	if ( fd < 0 )
	{
		return -1;
	}

	//开始分区
	printf("fdisk %s start...... %d\n", diskName,fd);

	llseek( fd, 0,SEEK_SET );

	//get Block size
	{
		int err = -1, blockSize = 0;
		err = ioctl(fd, BLKSSZGET, &blockSize);
		if ( -1 == err )
		{
			assert(false);
			return -1;
		}

	}

	//get device size
	LONG sz = 0;
	LONGLONG b = 0;
	LONGLONG sectors = 0;
	int err = -1;

	err = ioctl( fd, BLKGETSIZE64, &b );

	if (0 != err)
	{
		return -1;
	}
	else
	{
		sectors = ( b >> 9 );
	}


	/*
	先把分区表搞好，在搞好分区表头部，之所以是这个顺序，是因为CRC校验值的获得是按照这个顺序的。
	partitionNum 的值，暂时采用4的倍数的分区方式，暂时分为4个分区了。
	我们最多可以分128个分区，呵呵。
	*/
	//暂时强制分为4分区。
	if ( partitionNum != 4 )
	{
		partitionNum = 4;
	}

	char *pBuf = new  char[32*DEFAULT_SECTOR_SIZE];
	if ( pBuf == NULL )
	{
		assert (false);
	}
	memset(pBuf, 0, 32*DEFAULT_SECTOR_SIZE);

	struct __partition_table * pTable = ( struct __partition_table * ) pBuf;

	//减去 10M 的空间不分区
	sectors -= 10*1024*1024;

	//得出每个分区的大小(LBA)
	LONGLONG partitionSize = sectors/partitionNum;

	LONGLONG leftSize = sectors%partitionNum;

	ULONGLONG 	tmpSize;

	//开始设置各个分区表项的值了
	for ( int i = 0; i < partitionNum; i++)
	{
		//分区类型为: linux数据分区类型。
		pTable[i].partitionType[0] = 0xa2;
		pTable[i].partitionType[1] = 0xa0;
		pTable[i].partitionType[2] = 0xd0;
		pTable[i].partitionType[3] = 0xeb;

		pTable[i].partitionType[4] = 0xe5;
		pTable[i].partitionType[5] = 0xb9;

		pTable[i].partitionType[6] = 0x33;
		pTable[i].partitionType[7] = 0x44;

		pTable[i].partitionType[8] = 0x87;
		pTable[i].partitionType[9] = 0xc0;
		pTable[i].partitionType[10] = 0x68;
		pTable[i].partitionType[11] = 0xb6;
		pTable[i].partitionType[12] = 0xb7;
		pTable[i].partitionType[13] = 0x26;
		pTable[i].partitionType[14] = 0x99;
		pTable[i].partitionType[15] = 0xc7;



		//设置唯一的GUID或者说UUID
		//GetUUID( &(pTable[i].uniqueGUID) );

		//uuid_clear(pTable[i].uniqueGUID);
		uuid_generate(pTable[i].uniqueGUID);

		if ( i == 0 )
		{
			pTable[i].firstLBA = 0x22;
		}
		else
		{
			pTable[i].firstLBA = tmpSize + 1;
		}
		pTable[i].lastLBA = pTable[i].firstLBA + partitionSize -1;
		tmpSize = pTable[i].lastLBA ;

		pTable[i].attributes = 0;
		sprintf( (char *)pTable[i].partitonName, "sda%d", i );


	}

	/*
	再弄好GPT头部
	*/
	struct __gpt_header 	gptHeader;
	memset(&gptHeader, 0, sizeof(struct __gpt_header) );


	ULONGLONG	signature;	 
	ULONG data1 = 0x54524150;
	ULONG data2 = 0x20494645;

	//0x5452415020494645
	signature = data1;
	signature = signature<<32;
	signature += data2;

	gptHeader.signature = signature;
	gptHeader.revision 		= 0x10000;
	gptHeader.headerSize 	= 0x5c;
	gptHeader.headerCRC 	= 0; 
	gptHeader.reserved 		= 0;
	gptHeader.currentLBA 	= 1; //肯定是 1 了
	gptHeader.backupLBA 	= sectors - 1; //备份在最后一个LBA
	gptHeader.firstUsableLBA = 0x22;
	gptHeader.lastUsableLBA = sectors - 34;//值都是自己算出来的


	//uuid_clear(gptHeader.diskGUID);
	//GetUUID( &(gptHeader.diskGUID) );
	uuid_generate(gptHeader.diskGUID);

	//ReverseBytes( gptHeader.diskGUID.data4, 8 );


	gptHeader.partitionEntriesLBA = 2;
	gptHeader.numParts = 0x80;
	gptHeader.sizeOfPartitionEntries =  0x80;
	gptHeader.partitionEntriesCRC = GetCRC_Value ( ( char *) pTable, 32*DEFAULT_SECTOR_SIZE );
	//赋好了值之后，重新计算 headerCRC
	gptHeader.headerCRC = GetCRC_Value ( ( char *) &gptHeader, 92 );


	/*
	把GPT头部和GPT分区表项都写入磁盘，这样就算分好区了
	*/

	//构建备份分区表头
	struct __gpt_header SecondGptHead;
	memset( &SecondGptHead, 0, sizeof(struct __gpt_header) );
	SecondGptHead.signature = gptHeader.signature;
	SecondGptHead.revision = gptHeader.revision;
	SecondGptHead.headerSize = gptHeader.headerSize;
	SecondGptHead.headerCRC = 0;//需要重新赋值，这个就要重新计算了
	SecondGptHead.reserved = gptHeader.reserved;
	SecondGptHead.currentLBA = gptHeader.backupLBA;
	SecondGptHead.backupLBA = gptHeader.currentLBA;
	SecondGptHead.firstUsableLBA = gptHeader.firstUsableLBA;
	SecondGptHead.lastUsableLBA = gptHeader.lastUsableLBA;

	//SecondGptHead.diskGUID = gptHeader.diskGUID;
	memcpy( (char *) &(SecondGptHead.diskGUID), (char *) &(gptHeader.diskGUID), sizeof(uuid_t) );

	SecondGptHead.partitionEntriesLBA = SecondGptHead.lastUsableLBA + 1;

	SecondGptHead.numParts = gptHeader.numParts;
	SecondGptHead.sizeOfPartitionEntries = gptHeader.sizeOfPartitionEntries;
	SecondGptHead.partitionEntriesCRC = gptHeader.partitionEntriesCRC; //可以直接赋值。因为到时候还原回去也要一样。

	SecondGptHead.headerCRC = GetCRC_Value ( ( char *) &SecondGptHead, 92);//重新计算headerCRC的值。


	//首先把GPT表头和分区表项清空
	{
		char * pDeleteBuf = new char[ 33*DEFAULT_SECTOR_SIZE ] ;
		if ( NULL == pDeleteBuf )
		{
			assert(false);
		}

		memset(pDeleteBuf, 0, 33*DEFAULT_SECTOR_SIZE );

		llseek(fd, DEFAULT_SECTOR_SIZE,SEEK_SET);
		if ( write(fd, pDeleteBuf, 33*DEFAULT_SECTOR_SIZE ) != 33*DEFAULT_SECTOR_SIZE )
		{
			assert(false);
		}

		if (NULL != pDeleteBuf)
		{
			delete [] pDeleteBuf;
			pDeleteBuf = NULL;
		}

	}

	/*
	save secondary GPT and GPT Header
	*/
	llseek( fd, sectors - 33, SEEK_SET );
	if ( write(fd, pBuf, 32*DEFAULT_SECTOR_SIZE ) != 32*DEFAULT_SECTOR_SIZE )
	{
		if (NULL != pBuf)
		{
			delete [] pBuf;
			pBuf = NULL;
		}
		close(fd);
		assert(false);
		return -1;
	}	

	if ( write(fd, &SecondGptHead, sizeof(struct __gpt_header) ) != sizeof(struct __gpt_header) )
	{
		if (NULL != pBuf)
		{
			delete [] pBuf;
			pBuf = NULL;
		}

		close(fd);
		assert(false);
		return -1;
	}

	//写分区表项
	llseek(fd, DEFAULT_SECTOR_SIZE*2,SEEK_SET);
	if ( write(fd, pBuf, 32*DEFAULT_SECTOR_SIZE ) != 32*DEFAULT_SECTOR_SIZE )
	{
		if (NULL != pBuf)
		{
			delete [] pBuf;
			pBuf = NULL;
		}

		close(fd);
		assert(false);
		return -1;
	}	

	//写GPT头
	llseek(fd, DEFAULT_SECTOR_SIZE,SEEK_SET);
	if ( write(fd, &gptHeader, sizeof(struct __gpt_header) ) != sizeof(struct __gpt_header) )
	{
		if (NULL != pBuf)
		{
			delete [] pBuf;
			pBuf = NULL;
		}

		close(fd);
		assert(false);
		return -1;
	}	

	//写MBR
	//先读取到缓冲里面
	llseek( fd, 0 , SEEK_SET );
	//读取mbr中的内容
	read( fd, szMbrBuffer, BOOT_LENGTH );
	nIndex = 0;
	pParttemp = mbr_pt_offset( szMbrBuffer , nIndex );	
	pParttemp[0].diskStatus = 0;
	pParttemp[0].firstSector[0] = 0;
	pParttemp[0].firstSector[1] = 2;
	pParttemp[0].firstSector[2] = 0;
	pParttemp[0].partitionType = 0xee;
	pParttemp[0].lastSector[0] = 0xff;
	pParttemp[0].lastSector[1] = 0xff;
	pParttemp[0].lastSector[2] = 0xff;
	pParttemp[0].firstLBA = 1;
	pParttemp[0].lengthLBA = 0xffffffff;

	memset( &(pParttemp[1]), 0, 3*sizeof(struct __mbr_partition) );

	//设置结束标志
	szMbrBuffer[BOOT_LENGTH - 2] = 0x55;
	szMbrBuffer[BOOT_LENGTH - 1] = 0xaa;

	llseek( fd, 0 , SEEK_SET );
	if ( write(fd, szMbrBuffer, BOOT_LENGTH ) != BOOT_LENGTH )
	{
		if (NULL != pBuf)
		{
			delete [] pBuf;
			pBuf = NULL;
		}

		close(fd);
		assert(false);
		return -1;
	}	



	if (NULL != pBuf)
	{
		delete [] pBuf;
		pBuf = NULL;
	}

	//为了保证将数据存到磁盘上,刷新内核缓冲区
	sleep(1);
	fsync(fd);
	//sleep(1);

	if ( ( ioctl(fd, BLKRRPART) ) != 0 )
		printf( "\nWARNING: Re-reading the partition table failed.\n" );

	close(fd);
	return 0;


}
int CDiskManager::NormalDiskPartitionProc(char *diskName, unsigned char partitionNum , unsigned char nSys)
{	
	int fd;
	unsigned char nIndex = 0;
	unsigned char nLength = 0;
	char szMbrBuffer[BOOT_LENGTH] = { 0 };
	unsigned long lTotalSectors;		 //总扇区数
	struct hd_geometry geometry;		 //硬盘的几何参数
	unsigned long lUnits;				 //分区单位：以柱面为单位
	unsigned long lTotalCylinders;		 //总共柱面数
	unsigned long lUnitsPerParttion;		   
	unsigned long start;				 //在分区过程中记录某个分区的起始扇区
	unsigned long stop; 				 //在分区过程中记录某个分区的结束扇区
	unsigned char nMainPartNum;
	unsigned char nExtendPartNum;
	unsigned char szEbrBoot[BOOT_LENGTH] = { 0 };
	loff_t uExtendBase = 0;   //扩展分区的基本绝对起始扇区
	loff_t uEbrStart = 0;
	unsigned long uEbrEnd = 0;
	struct partition* pParttemp = NULL;

	if ( ( partitionNum <= 0 ) || ( partitionNum > 16 ) )
	{
		partitionNum = 4;
	}

	fd = open( diskName, O_RDWR );
	if (fd < 0)
	{
		return -1;
	}
	
	//开始分区
	printf( "fdisk %s start...... %d\n", diskName,fd );
	
	//读取mbr中的内容
	nLength = read(fd, szMbrBuffer, BOOT_LENGTH);
	
	//清除分区表信息
	nIndex	= 0;
	for ( nIndex  = 0; nIndex < DPT_PARTTION; nIndex++ )
	{
		pParttemp = pt_offset( szMbrBuffer, nIndex );		
		Clear_Partition(pParttemp);
	}

		
	//获得硬盘的扇区数
	if ( ioctl( fd, BLKGETSIZE, &lTotalSectors ) )
	{	
		close( fd );
		return -1;
	}

	//获得硬盘的柱面数
	if ( ioctl( fd, HDIO_GETGEO, &geometry ) )
	{
		close( fd );
		return -1;
	}

	lTotalCylinders = lTotalSectors/( geometry.heads * geometry.sectors );
   
	//获取分区单位,也就是说分区的最小颗粒。即是每一个柱面的扇区数。
	lUnits = geometry.heads * geometry.sectors;
	
	//获取每个分区包含多少个分区单位，即多少个柱面
	lUnitsPerParttion = lTotalCylinders/partitionNum;


	if ( partitionNum > DPT_PARTTION )
	{
		nMainPartNum = DPT_PARTTION - 1;
		nExtendPartNum = partitionNum - nMainPartNum;
	}
	else
	{
		nMainPartNum = partitionNum;
		nExtendPartNum = 0;
	}

	//写主分区表
	//第一分区的起始扇区；要预留出隐藏扇区
	start = geometry.sectors; // geometry.sectors == 63 XXX
	for ( nIndex = 0; nIndex < nMainPartNum; nIndex++ )
	{
		//得到了这个分区的总扇区数 XXX
		stop = ( nIndex + 1 ) * lUnitsPerParttion * lUnits - 1;
		SetParttion( szMbrBuffer, nIndex, start, stop, nSys, geometry );
		start = stop + 1;
	}

	//设置结束标志
	szMbrBuffer[BOOT_LENGTH - 2] = 0x55;
	szMbrBuffer[BOOT_LENGTH - 1] = 0xaa;

	//把分区后的信息写入主分区表中
	lseek(fd, 0, SEEK_SET);
	if ( write(fd, szMbrBuffer, BOOT_LENGTH) != BOOT_LENGTH )
	{
		close(fd);
		return -1;
	}

	//为了保证将数据存到磁盘上,使用sync()来刷新内核缓冲区
	sync();
	sleep(2);
	if ( ( ioctl(fd, BLKRRPART) ) != 0 )
		printf( "\nWARNING: Re-reading the partition table failed.\n" );

	printf( " partition NormalDisk %s ok!!!\n", diskName );
	close(fd);
	return 0;	
}



// 测试设备的大小端，结果证明我们的设备是小段设备。
int CDiskManager::IsLittleEndian(void) 
{   
	int littleE = 1; 
	union	  
	{ 	 
		unsigned int num;		
		unsigned char uc[sizeof(unsigned int)];   
	} endian;
	
	endian.num = 1;
	if ( endian.uc[0] != (unsigned char) 1 )	
	{	   
		littleE = 0;   
	} 
	return (littleE);
} 

//bigDiskFlag 返回 true 那么说明当前操作的磁盘是GPT磁盘。
 int CDiskManager::SetPartitionInfo(char *diskName, unsigned char partitionNum, unsigned char nSys, 
								   MESSAGE_CALLBACK_EX PercentCallBack, void *pPara, bool pBigDiskFlag)
{
	int fd;
	unsigned char nIndex = 0;
    char szMbrBuffer[BOOT_LENGTH] = {0};
	
	struct partition* pParttemp = NULL;
	
	if ((partitionNum <= 0) || (partitionNum > 16))
	{
		partitionNum = 4;
	}

	fd = open(diskName, O_RDWR);
	if (fd < 0)
	{
		return -1;
	}
	
	 if ( BOOT_LENGTH != read(fd, szMbrBuffer, BOOT_LENGTH) )
 	 {
		 return -1;
	 }

	//需要验证下其实剩余的三个分区表项应该都为0，否则磁盘会被保护。

	close(fd);
	
	if (pBigDiskFlag == true )
	{
		return GptDiskPartitionProc( diskName, partitionNum );
	}
	else
	{
		return NormalDiskPartitionProc( diskName,  partitionNum ,  nSys);
	}
	
}

bool CDiskManager::FormatParttion(int &fd, unsigned long filesize, unsigned long &lAviNum, unsigned long &eventlogSizeM, unsigned long &operlogSizeM, bool bigDiskFlag)
{
	//获取分区的物理信息，
	struct hd_geometry geometry;
	if (ioctl(fd, HDIO_GETGEO, &geometry))
		return false;

	//得到分区的大小
	LONGLONG cblocks = 0;	// 单位是 512 字节
	
	if ( true == bigDiskFlag )
	{
		//说明当前是超大容量磁盘
		LONG sz = 0;
		LONGLONG b = 0;
		int err = -1;
	
		err = ioctl( fd, BLKGETSIZE64, &b );
	
	
		if (0 != err)
		{
			return false;
		}
		else
		{
			cblocks = ( b >> 9 );
		}
		
		
		//减去 10M 的空间不分区
		cblocks -= 10*1024*2;
		
		//得出每个分区的大小(LBA)
		cblocks = cblocks;
	
	}
	else
	{
#if defined(__TDNXP__)
		{
			unsigned long lTotalSectorsTemp;
			if (ioctl(fd, BLKGETSIZE, &lTotalSectorsTemp))
			{
				return false;
			}
			cblocks = ((unsigned long long )lTotalSectorsTemp) * DEFAULT_SECTOR_SIZE / BLOCK_SIZE;
		}
		printf("%s,%d,clo:%d\n",__FILE__,__LINE__,cblocks);
#else
		//得到分区的大小
		loff_t high, low;  // loff_t is long long int
		for (high = 1, low = 0; ValidOffset (fd, high); high *= 2)
			low = high;	
		while (low < high - 1)
		{
			loff_t mid = (low + high) / 2;		
			if (ValidOffset (fd, mid))
				low = mid;
			else
				high = mid;
		}
		ValidOffset (fd, 0); //把文件指针回复到开始位置
		cblocks = (low + 1) / BLOCK_SIZE;
#endif	
	
	}
	
	///获得文件的状态信息
	struct stat statbuf;
	if (fstat(fd, &statbuf) < 0)
		return false;
	if ( !S_ISBLK(statbuf.st_mode) )//如果文件是一个块设备，则S_ISBLK()返回真
	{
		statbuf.st_rdev = 0;///st_rdev;  /* device type (if inode device) */
	}
	
	//初始化DBR中的参数，即引导扇区
	msdos_boot_sector mbs;	/* Boot sector data */
	unsigned char szDummyBootJump[3] = {0};
	szDummyBootJump[0] = 0xeb;
	szDummyBootJump[1] = 0x3c;
	szDummyBootJump[2] = 0x90;
	memcpy(mbs.boot_jump,szDummyBootJump, 3);
	mbs.boot_jump[1] =((char *) &mbs.fat32.boot_code - (char *)&mbs) - 2;
	strcpy ((char *)mbs.system_id, "MSDOS5.0"); 
	mbs.sector_size[0] = (char) ( DEFAULT_SECTOR_SIZE & 0x00ff );
	mbs.sector_size[1] = (char) ( (DEFAULT_SECTOR_SIZE & 0xff00) >> 8 );
	mbs.reserved = CT_LE_W(DBR_RESERVED_SECTORS);
	mbs.fats = (char)DEFAULT_FAT_NUM;	/* Default number of FATs to produce */
	mbs.dir_entries[0] = (char)0;  
	mbs.dir_entries[1] = (char)0; //fat32中根目录也当成文件来处理。fat16才使用该字段。fat32要为0
	mbs.media = (char)0xf8; /* Set up the media descriptor for a hard drive */
	mbs.fat_length = CT_LE_W(0);//fat32要求该字段为0
	mbs.secs_track = CT_LE_W(geometry.sectors);	/* Set up the geometry information */
	mbs.heads = CT_LE_W(geometry.heads);
	mbs.hidden = CT_LE_L(0);

	/* set up additional FAT32 fields */
	mbs.fat32.flags = CT_LE_W(0);
	mbs.fat32.version[0] = 0;
	mbs.fat32.version[1] = 0;
	mbs.fat32.root_cluster = CT_LE_L(2);
	mbs.fat32.info_sector = CT_LE_W(1);
	int backupboot = (DBR_RESERVED_SECTORS >= 7) ? 6 :
	(DBR_RESERVED_SECTORS >= 2) ? DBR_RESERVED_SECTORS-1 : 0;
	mbs.fat32.backup_boot = CT_LE_W(backupboot);
	memset( &mbs.fat32.reserved2, 0, sizeof( mbs.fat32.reserved2 ) );
    
	//FAT32分区的扩展BPB字段  
	time_t CreateTime;
	time(&CreateTime);//可用于卷标，
	long lVolumeId = (long)CreateTime;	/* Default volume ID = creation time */
	struct msdos_volume_info *vi =  &mbs.fat32.vi;
	vi->volume_id[0] = (unsigned char) (lVolumeId & 0x000000ff);
	vi->volume_id[1] = (unsigned char) ((lVolumeId & 0x0000ff00) >> 8);
	vi->volume_id[2] = (unsigned char) ((lVolumeId & 0x00ff0000) >> 16);
	vi->volume_id[3] = (unsigned char) (lVolumeId >> 24);
	memcpy( vi->volume_label, "vol", 11 );
	memcpy( vi->fs_type, MSDOS_FAT32_SIGN, 8 );

	char szDummyBootCode[BOOTCODE_FAT32_SIZE] = {0};
	szDummyBootCode[BOOTCODE_FAT32_SIZE-1] = 0;
	memcpy(mbs.fat32.boot_code, szDummyBootCode, BOOTCODE_FAT32_SIZE);
	mbs.boot_sign = CT_LE_W(BOOT_SIGN);///#define BOOT_SIGN 0xAA55/* Boot sector magic number */

	//计算出参数cluster_size fat32_length的取值
	unsigned long long VolSize = cblocks*SECTORS_PER_BLOCK;//
	printf("cblocks=%lld\n",cblocks);
	//先给cluster_size赋一个粗值
	if (VolSize <= 66600)
	{
		printf("Volume %d too small for FAT32,cblocks=%lld, SECTORS_PER_BLOCK=%d\n", VolSize, cblocks, SECTORS_PER_BLOCK);
		return false;
	}else if (VolSize <= 532480)
		mbs.cluster_size = 1;
	else if (VolSize <= 16777216)
		mbs.cluster_size = 8;
	else if (VolSize <= 33554432)
		mbs.cluster_size = 16;
	else if (VolSize <= 67108864)
		mbs.cluster_size = 32;
	else
		mbs.cluster_size = 64;

	//为cluster_size选择一个合适的值
	unsigned long  lTotalSectors=0;
	if(bigDiskFlag)
	{
		lTotalSectors = (unsigned long) cblocks;	
	}
	else
	{
		lTotalSectors= cblocks*BLOCK_SIZE/DEFAULT_SECTOR_SIZE;
	}
	 
	
	unsigned long long lFatData = lTotalSectors - DBR_RESERVED_SECTORS;	
	int maxclustsize = 128;	
	unsigned long lClust32; //记录数据区的簇数
	unsigned long lMaxClust32;
	unsigned long lFatLength32; //记录fat32表占的扇区数
	do 
	{
		lClust32 = (lFatData * DEFAULT_SECTOR_SIZE - DEFAULT_FAT_NUM*8) /
			((int) mbs.cluster_size * DEFAULT_SECTOR_SIZE + DEFAULT_FAT_NUM*4);
		lFatLength32 = ((lClust32+2) * 4 + DEFAULT_SECTOR_SIZE - 1) / DEFAULT_SECTOR_SIZE;

		/* Need to recalculate number of clusters, since the unused parts of the
		* FATS and data area together could make up space for an additional,
		* not really present cluster. */
		lClust32 = (lFatData - DEFAULT_FAT_NUM*lFatLength32)/mbs.cluster_size;
		lMaxClust32 = (lFatLength32 * DEFAULT_SECTOR_SIZE) / 4;
		if (lMaxClust32 > MAX_CLUST_32)///#define MAX_CLUST_32	((1 << 28) - 16)
			lMaxClust32 = MAX_CLUST_32;

		if (lClust32 > lMaxClust32) 
		{
			lClust32 = 0;
		}
		
		if (lClust32 )///mine
			break;
		
		mbs.cluster_size <<= 1;
	} while (mbs.cluster_size && mbs.cluster_size <= maxclustsize);

	if ( !lClust32 )
	{
		printf("\nAttempting to create a too large file system\n");
		return false;
	}

	//到此处就为cluster_size选择了一个合适的值
	mbs.fat32.fat32_length = CT_LE_L( lFatLength32 );
	//填写参数sectors  total_sect
	if ( lTotalSectors >= 65536 )
	{
		mbs.sectors[0] = (char)0;
		mbs.sectors[1] = (char)0;
		mbs.total_sect = CT_LE_L( lTotalSectors );
	}
	else
	{
		mbs.sectors[0] = (char)( lTotalSectors & 0x00ff );
		mbs.sectors[1] = (char)((lTotalSectors & 0xff00) >> 8);
		mbs.total_sect = CT_LE_L(0);
	}

	/* The two following vars are in hard sectors, i.e. 512 byte sectors! */
	unsigned long lStartDataofSector = (DBR_RESERVED_SECTORS + DEFAULT_FAT_NUM*lFatLength32);
	unsigned long lStartDataofBlock = (lStartDataofSector + SECTORS_PER_BLOCK - 1) / SECTORS_PER_BLOCK;
	if (cblocks < lStartDataofBlock + 32)	/* Arbitrary undersize file system! */
	{
		printf("\nToo few blocks for viable file system\n");
		return false;
	}

	//下面是填写fat32表和根目录文件。
	unsigned char* pFat = NULL; 
	unsigned char* pZeroFat = NULL;
	if ( ( pFat = (unsigned char *)malloc(DEFAULT_SECTOR_SIZE) ) == NULL )
	{
		printf("\nunable to allocate space for FAT image in memory\n");
		return false;
	}
	memset(pFat, 0, DEFAULT_SECTOR_SIZE);
	
	if ((pZeroFat = (unsigned char *)malloc(DEFAULT_SECTOR_SIZE)) == NULL)
	{
		printf("\nunable to allocate space for FAT image in memory\n");
		free(pFat);
		return false;
	}
	memset(pZeroFat, 0, DEFAULT_SECTOR_SIZE);

	//填写fat表的第0，1簇
	Mark_FAT_Cluster(0, 0xffffffff,pFat);	/* Initial fat entries */
	pFat[0] = (unsigned char)mbs.media;	/* Put media type in first byte! */
	Mark_FAT_Cluster(1, 0xffffffff,pFat);

	//文件所占的簇数
	unsigned long lClustersofFile = filesize/(mbs.cluster_size*DEFAULT_SECTOR_SIZE);
	//整个FAT可写的文件数
	unsigned long lFileNum = (lFatLength32*DEFAULT_SECTOR_SIZE/4-2)/lClustersofFile;
	//根目录所占的簇数
	unsigned long lClustersofRoot = (lFileNum+1)*32/(mbs.cluster_size*DEFAULT_SECTOR_SIZE)+1;

	//下面填写根目录所在的簇，FAT32将根目录等同于普通的文件
	for(unsigned long nRootCluCount = 0; nRootCluCount < lClustersofRoot; nRootCluCount++)//lClustersofRoot一般为1
	{
		if(nRootCluCount == (lClustersofRoot - 1))
		{
			Mark_FAT_Cluster(nRootCluCount+2,0xffffffff,pFat);
		}
		else
		{
			Mark_FAT_Cluster(nRootCluCount+2,nRootCluCount+3,pFat);
		}
	}


	//根目录文件的大小(字节数)
	unsigned long lRootFileSize = lClustersofRoot*mbs.cluster_size*DEFAULT_SECTOR_SIZE;
	struct msdos_dir_entry* pRootDir = NULL; //指向根目录文件
	if ((pRootDir = (struct msdos_dir_entry *)malloc (lRootFileSize)) == NULL)
	{
		free(pFat);		/* Tidy up before we die! */
		free(pZeroFat);
		printf("\nunable to allocate space for root directory in memory\n");
		return false;
	}
	memset(pRootDir, 0, lRootFileSize);

	//下面是写根目录文件
	
	//根目录的第0文件为：卷标
	////初始化第0个目录项(32个字节)//////////
	struct msdos_dir_entry *de = &pRootDir[0];  
	memcpy(de->name, "DVR", 11);
	de->attr = ATTR_VOLUME;//mine
	struct tm stCtimeTmp = {0};
	localtime_r( &CreateTime, &stCtimeTmp );
	struct tm* ctime = &stCtimeTmp;
	de->time = CT_LE_W( (unsigned short)((ctime->tm_sec >> 1) +
		                	(ctime->tm_min << 5) + (ctime->tm_hour << 11)));
	de->date = CT_LE_W( (unsigned short)(ctime->tm_mday +
				               ((ctime->tm_mon+1) << 5) +
				               ((ctime->tm_year-80) << 9)) );
	de->ctime_ms = 0;
	de->ctime = de->time;
	de->cdate = de->date;
	de->adate = de->date;
	de->starthi = CT_LE_W(0);
	de->start = CT_LE_W(0);
	de->size = CT_LE_L(0);
    
	//分区上要有几个特定的文件
	//unsigned long disksizeG=lTotalSectors*DEFAULT_SECTOR_SIZE/1024/1024/1024;
	LONGLONG diskSizeBytes = ((LONGLONG)lTotalSectors)*DEFAULT_SECTOR_SIZE;
    unsigned long disksizeG = diskSizeBytes/(1024*1024*1024);
	printf("disksizeG=%d\n",disksizeG);	
	unsigned long lStartClu = lClustersofRoot + 2;//数据区从二开始编簇号,数据区先放根目录，然后才放其它文件（在mbr中已经规定好了）
	eventlogSizeM = (disksizeG/64)+1;
	operlogSizeM  = (((disksizeG/64)+1)*4);

	CreateFileItem(&pRootDir[1],"NORMALS BIN",lStartClu,1*1024*1024,mbs.cluster_size);
#if defined(__PACKID_9__) || defined(__PACKID_14__) || defined(__PACKID_27__)
	CreateFileItem(&pRootDir[2],"RECLOG  BIN",lStartClu, 5*1024*1024,mbs.cluster_size);
#else
	CreateFileItem(&pRootDir[2],"RECLOG  BIN",lStartClu,((disksizeG/5)+1)*1*1024*1024,mbs.cluster_size);
#endif
	CreateFileItem(&pRootDir[3],"EVENTLOGBIN",lStartClu,((disksizeG/64)+1)*1*1024*1024,mbs.cluster_size);
	CreateFileItem(&pRootDir[4],"OPERLOG BIN",lStartClu,((disksizeG/64)+1)*1*1024*1024,mbs.cluster_size);
	CreateFileItem(&pRootDir[5],"TEMPREC DAT",lStartClu, filesize,mbs.cluster_size);
	CreateFileItem(&pRootDir[6],"TEMPLOG BIN",lStartClu, ((disksizeG/5)+1)*1*1024*1024,mbs.cluster_size);
	
    
   	//有实际簇的个数得到的视频文件数
    unsigned long lActualAviNum = (lClust32 - (lStartClu -2))/lClustersofFile;
		
	//循环写视频文件
	//预留一个文件索引用于交换文件或升级文件
	unsigned long lAviCount = 0;
	while (lAviCount < lActualAviNum - 1)
	{
		char sz[13];
		int num = lAviCount;
		for(int j=0;j<8;j++)
		{
			sz[7-j]='0'+num%10;
			num=num/10;
		}
		strcpy(&sz[8],"DAT");
		CreateFileItem( &pRootDir[lAviCount+7],sz,lStartClu,filesize,mbs.cluster_size );
		lAviCount++;
	}
	lAviNum = lAviCount;

    //到此处根目录中的目录项都分配完成，只保留在内存pRootDir中，还没有写入硬盘中

	//下面写文件信息
	unsigned char* pInfoSector = NULL;
	if ( ( pInfoSector = (unsigned char *)malloc(DEFAULT_SECTOR_SIZE)) == NULL )
	{	
		free(pFat);		/* Tidy up before we die! */
		free(pZeroFat);
		free(pRootDir);
		printf("\nOut of memory\n");
		return false;		
	}
	memset(pInfoSector, 0, DEFAULT_SECTOR_SIZE);
	/* fsinfo structure is at offset 0x1e0 in info sector by observation */
	struct fat32_fsinfo* info = (struct fat32_fsinfo *)(pInfoSector + 0x1e0);	
	/* Info sector magic */
	pInfoSector[0] = 'R';
	pInfoSector[1] = 'R';
	pInfoSector[2] = 'a';
	pInfoSector[3] = 'A';	
	/* Magic for fsinfo structure */
	info->signature = CT_LE_L(0x61417272);
	/* We've allocated cluster 2 for the root dir. */
	info->free_clusters = CT_LE_L(lClust32-(lStartClu-2));//
	info->next_cluster = CT_LE_L(lStartClu);
	/* Info sector also must have boot sign */
	*(__u16 *)(pInfoSector + 0x1fe) = CT_LE_W(BOOT_SIGN);

	//到此处文件信息写完。

	//下面是把上面的内容，写到硬盘中
    //把分区的保留空间清零
	seekto( 0, "start of device" );
	BYTE nIndex = 0;
	for( nIndex = 0; nIndex < DBR_RESERVED_SECTORS; nIndex++)//写了32*512字节为0
	{
		if (write(fd,pZeroFat,DEFAULT_SECTOR_SIZE) != DEFAULT_SECTOR_SIZE)
		{
			FREE();
			return false;
		}
	}
    	
	//写dbr
	seekto( 0, "boot sector" );//写了512字节
	if(write(fd,(char *)&mbs, sizeof(struct msdos_boot_sector)) != DEFAULT_SECTOR_SIZE)
	{
		FREE();
		return false;
	}
	
	//写文件信息
	seekto(CF_LE_W(mbs.fat32.info_sector)*DEFAULT_SECTOR_SIZE, "info sector" );//定位到第512个字节
	if(write(fd,(char *)pInfoSector,DEFAULT_SECTOR_SIZE) != DEFAULT_SECTOR_SIZE)
	{
		FREE();
		return false;
	}

	//写backup boot sector
	seekto(CF_LE_W(mbs.fat32.backup_boot)*DEFAULT_SECTOR_SIZE, "backup boot sector" );
	if(write(fd,(char *)&mbs, sizeof (struct msdos_boot_sector)) != DEFAULT_SECTOR_SIZE)
	{
		FREE();
		return false;
	}

	//写FAT表中，在前面已经写了一部份了
	seekto(DBR_RESERVED_SECTORS*DEFAULT_SECTOR_SIZE, "first FAT" );
	for (int nFatCount = 1; nFatCount <= DEFAULT_FAT_NUM; nFatCount++)
	{
		//对fat表是一个个扇区的大小写入硬盘中
		int nPageNum = lFatLength32/6;//用于显示格式的进度
		int nClusterCount  =0; 
		unsigned long cu = 0;//用于记录当前簇
		struct msdos_dir_entry *pFile=&pRootDir[1];
		for (unsigned long fatno = 0; fatno < lFatLength32; fatno++)
		{
			unsigned char *pFatSector=pFat;
			if(fatno == 0)
			{
				pFatSector=pFat;
				cu=lClustersofRoot+2;
				nClusterCount = cu;
			}
			else
			{
				pFatSector=pZeroFat;
				memset(pZeroFat, 0, DEFAULT_SECTOR_SIZE);
				nClusterCount = 0;
			}
			bool bNewFile = true;
			unsigned long lEnd = 0;
			while((nClusterCount <  DEFAULT_SECTOR_SIZE/4) && (pFile->size > 0))
			{
				if (bNewFile)
				{
					lEnd = pFile->starthi;
					lEnd<<=16;
					lEnd|=pFile->start;
					lEnd += pFile->size/(mbs.cluster_size*DEFAULT_SECTOR_SIZE);
					if((pFile->size%(mbs.cluster_size*DEFAULT_SECTOR_SIZE)) == 0)
					{
						lEnd--;
					}					
				}	
				if(cu < lEnd)//当前簇小于文件的簇数
				{
					Mark_FAT_Cluster(cu,cu+1,pFatSector);
					bNewFile = false;
				}
				else
				{
					Mark_FAT_Cluster(cu,0xffffffff,pFatSector);
					pFile++;
					bNewFile = true;
				}
				cu++;
				nClusterCount++;
			}
			
			if (write(fd,(char *)pFatSector, DEFAULT_SECTOR_SIZE) != DEFAULT_SECTOR_SIZE)
			{
				FREE();
				return false;
			}

			if (!(fatno % nPageNum))
			{
//				m_nPercent++;	
//				if(m_PercentCallBack)
//				{
//					m_PercentCallBack(m_nPercent, m_pPara);
//				}
			}
			//printf("\rfat%d:finsh %ld/%ld", nFatCount,fatno,lFatLength32);
		}
	//	printf("\rfat%d:finsh %ld/%ld\n", nFatCount,lFatLength32,lFatLength32);
		
	}
	

	//写根目录文件
	if (write(fd,(char *)pRootDir, lRootFileSize) != (int)lRootFileSize)
	{
		FREE();
		return false;
	}
	
	FREE();
	sync();
	sleep(2);
	return true;
}

//执行这个函数之前已经判断出了，当前磁盘时GPT磁盘。所以就不进行磁盘检查了。
int CDiskManager::GetGptParttionInfo( char *diskname, PART_INFO *pPartitionInfo, BYTE *pPartitionNum )
{


    int fd = -1;
	int partitionNum = 0;
	
	fd = open(diskname,O_RDWR);
	if (fd < -1)
	{
		printf("(%s,%d):%s\n", __FILE__, __LINE__, diskname);
		return -1;
	}
	
	char *pBuf = new  char[32*DEFAULT_SECTOR_SIZE];
	if ( pBuf == NULL )
	{
		assert (false);
	}
	memset(pBuf, 0, 32*DEFAULT_SECTOR_SIZE);

	//定位到分区表项处。
	llseek( fd, DEFAULT_SECTOR_SIZE*2,SEEK_SET );
	if ( 32*DEFAULT_SECTOR_SIZE != read( fd, pBuf, 32*DEFAULT_SECTOR_SIZE )  )
	{
		delete [] pBuf;
		close(fd);
		return -1;
	}
	
	
	struct __partition_table * pTable = ( struct __partition_table * ) pBuf;

	//通过值来判断分区有多少个。简单点，就可以了的。GPT磁盘最多可以有128个分区
	for ( int i = 0; i < 16; i++)
	{
	
		if ( (0 == pTable[i].firstLBA) || ( 0 == pTable[i].lastLBA ) )
		{
			//分区就这么多个了，可以结束查询了。
			break;				
		}
		else
		{
			partitionNum++;
		}
	}
	
	if((0 == partitionNum)||( partitionNum > 16))
	{
		delete [] pBuf;
		close(fd);
		return -1;
	}

	*pPartitionNum = partitionNum;
	
	ULONGLONG	tmpLBA;
	ULONG		tmpSize = 0;

	//把分区信息赋值到内存中去。
	for ( int i = 0; i < partitionNum; i++ )
	{
		//单位是 M ,用移位的方式实现64位的除法。
		tmpLBA = (pTable[i].lastLBA - pTable[i].firstLBA + 1);
		tmpLBA = tmpLBA>>11;
		tmpSize = (ULONG)tmpLBA;
		
		pPartitionInfo[i].partsize = tmpSize;
		snprintf( pPartitionInfo[i].devnod, 20, "%s%d", diskname, i+1 );
	}

	if ( NULL != pBuf )
	{
		delete [] pBuf;
		pBuf = NULL;
	}
	close(fd);
	
	for(int i=0;i<partitionNum;i++)
	{
		GetDBRFilesystemInfo(pPartitionInfo[i].devnod,pPartitionInfo[i].partformat);
	}

	return 0;

}

bool CDiskManager::GetDBRFilesystemInfo(char *pDiskname,unsigned char &partformat)
{
	int fd=open(pDiskname,O_RDWR);
	if(fd<0)
	{
		partformat = 0;
		return false;
	}
	else
	{
		msdos_boot_sector mbs;
		lseek(fd,0,SEEK_SET);
		if(read(fd,(char *)&mbs, sizeof(struct msdos_boot_sector)) != DEFAULT_SECTOR_SIZE)
		{
			partformat = 0;
			close(fd);
			return false;
		}
		else
		{
			struct msdos_volume_info *vi =  &mbs.fat32.vi;
			if(strcmp((char *)(vi->fs_type),MSDOS_FAT32_SIGN)!=0)
			{
				partformat = 0;
				close(fd);
				return false;
			}
			else
			{
				partformat = FAT32;
			}
		}
	}
	close(fd);
	return true;
}


#else



int CDiskManager::SetPartitionInfo(char *diskName, unsigned char partitionNum, unsigned char nSys, 
								   MESSAGE_CALLBACK_EX PercentCallBack, void *pPara)
{
	int fd;
	unsigned char nIndex = 0;
    unsigned char nLength = 0;
    char szMbrBuffer[BOOT_LENGTH] = {0};
	unsigned long lTotalSectors;             //总扇区数
	struct hd_geometry geometry;             //硬盘的几何参数
	unsigned long lUnits;                    //分区单位：以柱面为单位
	unsigned long lTotalCylinders;           //总共柱面数
	unsigned long lUnitsPerParttion;           
	unsigned long start;                     //在分区过程中记录某个分区的起始扇区
	unsigned long stop;                      //在分区过程中记录某个分区的结束扇区
	unsigned char nMainPartNum;
    unsigned char nExtendPartNum;
	unsigned char szEbrBoot[BOOT_LENGTH] = {0};
	loff_t uExtendBase = 0;   //扩展分区的基本绝对起始扇区
	loff_t uEbrStart = 0;
	unsigned long uEbrEnd = 0;
	struct partition* pParttemp = NULL;

	if ((partitionNum <= 0) || (partitionNum > 16))
	{
		partitionNum = 4;
	}

	fd = open(diskName, O_RDWR);
	if (fd < 0)
	{
		return -1;
	}
	
	//开始分区
	printf("fdisk %s start...... %d\n", diskName,fd);
	
	//读取mbr中的内容
	nLength = read(fd, szMbrBuffer, BOOT_LENGTH);
	
	//清除分区表信息
	nIndex  = 0;
	for (nIndex  = 0; nIndex < DPT_PARTTION; nIndex++)
	{
		pParttemp = pt_offset(szMbrBuffer, nIndex);		
		Clear_Partition(pParttemp);
	}

		
	//获得硬盘的扇区数
	if (ioctl(fd, BLKGETSIZE, &lTotalSectors))
	{
		close(fd);
		return -1;
	}

	//获得硬盘的柱面数
	if (ioctl(fd, HDIO_GETGEO, &geometry))
	{
		close(fd);
		return -1;
	}
	lTotalCylinders = lTotalSectors/(geometry.heads * geometry.sectors);
   
	//获取分区单位,也就是说分区的最小颗粒。
	lUnits = geometry.heads * geometry.sectors;
	
	//获取每个分区包含多少个分区单位
	lUnitsPerParttion = lTotalCylinders/partitionNum;


	if (partitionNum > DPT_PARTTION)
	{
		nMainPartNum = DPT_PARTTION - 1;
		nExtendPartNum = partitionNum - nMainPartNum;
	}
	else
	{
		nMainPartNum = partitionNum;
		nExtendPartNum = 0;
	}

	//写主分区表
	//第一分区的起始扇区；要预留出隐藏扇区
	start = geometry.sectors; 
	for (nIndex = 0; nIndex < nMainPartNum; nIndex++)
	{
		stop = (nIndex + 1) * lUnitsPerParttion * lUnits - 1;
		SetParttion(szMbrBuffer, nIndex, start, stop, nSys, geometry);
		start = stop + 1;
	}

//	//写扩展分区表
//	start = stop + 1;
//	if (0 != nExtendPartNum)
//	{
//		stop = lTotalCylinders * lUnits - 1;
//		SetParttion(szMbrBuffer, nMainPartNum, start, stop, EXTENDED, geometry);
//	}
//    
//
//	pParttemp = pt_offset(szMbrBuffer, nMainPartNum);
//	uExtendBase = Get_Start_Sect(pParttemp); 
//	uEbrStart = 0;
//	start = 0;
//	stop = lUnitsPerParttion * lUnits - 1;
//	for (nIndex = 0; nIndex < nExtendPartNum - 1; nIndex++)
//	{
//		memset(szEbrBoot, 0, sizeof(szEbrBoot));
//		
//		uEbrStart = uExtendBase + start;
//		start += geometry.sectors;
//		SetParttion(szEbrBoot, 0, start, stop, nSys, geometry)
//
//		start = stop + 1;
//		stop = (nIndex + 2) * lUnitsPerParttion * lUnits - 1;
//
//		SetParttion(szEbrBoot, 1, start, stop, nSys, geometry);
//
//		szEbrBoot[BOOT_LENGTH - 2] = 0x55;
//		szEbrBoot[BOOT_LENGTH - 1] = 0xaa;
//		lseek(fd, uEbrStart, SEEK_SET);
//		if (BOOT_LENGTH != write(fd, szEbrBoot, BOOT_LENGTH))
//		{
//			close(fd);
//			return -1;
//		}
//	}
//
//	memset(szEbrBoot, 0, sizeof(szEbrBoot));
//    SetParttion(szEbrBoot, 0, geometry.sectors, stop, nSys, geometry);
//	pParttemp = pt_offset(szEbrBoot, 1);
//    Clear_Partition(pParttemp);
//
//    

	//设置结束标志
	szMbrBuffer[BOOT_LENGTH - 2] = 0x55;
	szMbrBuffer[BOOT_LENGTH - 1] = 0xaa;

	//把分区后的信息写入主分区表中
	lseek(fd, 0, SEEK_SET);
	if (write(fd, szMbrBuffer, BOOT_LENGTH) != BOOT_LENGTH)
	{
		close(fd);
		return -1;
	}

	///为了保证将数据存到磁盘上,使用sync()来刷新内核缓冲区
	sync();
	sleep(2);
	if ((ioctl(fd, BLKRRPART)) != 0)
		printf("\nWARNING: Re-reading the partition table failed.\n");

	printf("fdisk %s ok!!!\n",diskName);
	close(fd);
	return 0;   
}


bool CDiskManager::FormatParttion(int &fd, unsigned long filesize, unsigned long &lAviNum,unsigned long &eventlogSizeM, unsigned long &operlogSizeM)
{
	//获取分区的物理信息，
	struct hd_geometry geometry;
	if (ioctl(fd, HDIO_GETGEO, &geometry))
		return false;
	unsigned long long cblocks;
#if defined(__TDNXP__)
	{
		unsigned long lTotalSectorsTemp;
		if (ioctl(fd, BLKGETSIZE, &lTotalSectorsTemp))
		{
			return false;
		}
		cblocks = ((unsigned long long )lTotalSectorsTemp) * DEFAULT_SECTOR_SIZE / BLOCK_SIZE;
	}
	printf("%s,%d,clo:%d\n",__FILE__,__LINE__,cblocks);
#else
	//得到分区的大小
	loff_t high, low;  // loff_t is long long int
	for (high = 1, low = 0; ValidOffset (fd, high); high *= 2)
		low = high;	
	while (low < high - 1)
    {
		loff_t mid = (low + high) / 2;		
		if (ValidOffset (fd, mid))
			low = mid;
		else
			high = mid;
    }
	ValidOffset (fd, 0); //把文件指针回复到开始位置
	cblocks = (low + 1) / BLOCK_SIZE;
#endif	
	
	///获得文件的状态信息
	struct stat statbuf;
	if (fstat(fd, &statbuf) < 0)
		return false;
	if (!S_ISBLK(statbuf.st_mode))//如果文件是一个块设备，则S_ISBLK()返回真
	{
		statbuf.st_rdev = 0;///st_rdev;  /* device type (if inode device) */
	}
	
	//初始化DBR中的参数，即引导扇区
	msdos_boot_sector mbs;	/* Boot sector data */
	unsigned char szDummyBootJump[3] = {0};
	szDummyBootJump[0] = 0xeb;
	szDummyBootJump[1] = 0x3c;
	szDummyBootJump[2] = 0x90;
	memcpy(mbs.boot_jump,szDummyBootJump, 3);
	mbs.boot_jump[1] =((char *) &mbs.fat32.boot_code - (char *)&mbs) - 2;
	strcpy ((char *)mbs.system_id, "MSDOS5.0"); 
	mbs.sector_size[0] = (char) ( DEFAULT_SECTOR_SIZE & 0x00ff);
	mbs.sector_size[1] = (char) ((DEFAULT_SECTOR_SIZE & 0xff00) >> 8);
	mbs.reserved = CT_LE_W(DBR_RESERVED_SECTORS);
	mbs.fats = (char)DEFAULT_FAT_NUM;	/* Default number of FATs to produce */
	mbs.dir_entries[0] = (char)0;  
	mbs.dir_entries[1] = (char)0; //fat32中根目录也当成文件来处理。fat16才使用该字段。fat32要为0
	mbs.media = (char)0xf8; /* Set up the media descriptor for a hard drive */
	mbs.fat_length = CT_LE_W(0);//fat32要求该字段为0
	mbs.secs_track = CT_LE_W(geometry.sectors);	/* Set up the geometry information */
	mbs.heads = CT_LE_W(geometry.heads);
	mbs.hidden = CT_LE_L(0);

	/* set up additional FAT32 fields */
	mbs.fat32.flags = CT_LE_W(0);
	mbs.fat32.version[0] = 0;
	mbs.fat32.version[1] = 0;
	mbs.fat32.root_cluster = CT_LE_L(2);
	mbs.fat32.info_sector = CT_LE_W(1);
	int backupboot = (DBR_RESERVED_SECTORS >= 7) ? 6 :
	(DBR_RESERVED_SECTORS >= 2) ? DBR_RESERVED_SECTORS-1 : 0;
	mbs.fat32.backup_boot = CT_LE_W(backupboot);
	memset(&mbs.fat32.reserved2, 0, sizeof(mbs.fat32.reserved2));
    
	//FAT32分区的扩展BPB字段  
	time_t CreateTime;
	time(&CreateTime);//可用于卷标，
	long lVolumeId = (long)CreateTime;	/* Default volume ID = creation time */
	struct msdos_volume_info *vi =  &mbs.fat32.vi;
	vi->volume_id[0] = (unsigned char) (lVolumeId & 0x000000ff);
	vi->volume_id[1] = (unsigned char) ((lVolumeId & 0x0000ff00) >> 8);
	vi->volume_id[2] = (unsigned char) ((lVolumeId & 0x00ff0000) >> 16);
	vi->volume_id[3] = (unsigned char) (lVolumeId >> 24);
	memcpy(vi->volume_label, "vol", 11);
	memcpy(vi->fs_type, MSDOS_FAT32_SIGN, 8);

	char szDummyBootCode[BOOTCODE_FAT32_SIZE] = {0};
	szDummyBootCode[BOOTCODE_FAT32_SIZE-1] = 0;
	memcpy(mbs.fat32.boot_code, szDummyBootCode, BOOTCODE_FAT32_SIZE);
	mbs.boot_sign = CT_LE_W(BOOT_SIGN);///#define BOOT_SIGN 0xAA55/* Boot sector magic number */

	//计算出参数cluster_size fat32_length的取值
	unsigned long long VolSize = cblocks*SECTORS_PER_BLOCK;//
	printf("or cblocks=%lld\n",cblocks);
	//先给cluster_size赋一个粗值
	if (VolSize <= 66600)
	{
		printf("Volume too small for FAT32\n");
		return false;
	}else if (VolSize <= 532480)
		mbs.cluster_size = 1;
	else if (VolSize <= 16777216)
		mbs.cluster_size = 8;
	else if (VolSize <= 33554432)
		mbs.cluster_size = 16;
	else if (VolSize <= 67108864)
		mbs.cluster_size = 32;
	else
		mbs.cluster_size = 64;

	//为cluster_size选择一个合适的值
	unsigned long  lTotalSectors = cblocks*BLOCK_SIZE/DEFAULT_SECTOR_SIZE;	
	unsigned long long lFatData = lTotalSectors - DBR_RESERVED_SECTORS;	
	int maxclustsize = 128;	
	unsigned long lClust32; //记录数据区的簇数
	unsigned long lMaxClust32;
	unsigned long lFatLength32; //记录fat32表占的扇区数
	do 
	{
		lClust32 = (lFatData * DEFAULT_SECTOR_SIZE - DEFAULT_FAT_NUM*8) /
			((int) mbs.cluster_size * DEFAULT_SECTOR_SIZE + DEFAULT_FAT_NUM*4);
		lFatLength32 = ((lClust32+2) * 4 + DEFAULT_SECTOR_SIZE - 1) / DEFAULT_SECTOR_SIZE;

		/* Need to recalculate number of clusters, since the unused parts of the
		* FATS and data area together could make up space for an additional,
		* not really present cluster. */
		lClust32 = (lFatData - DEFAULT_FAT_NUM*lFatLength32)/mbs.cluster_size;
		lMaxClust32 = (lFatLength32 * DEFAULT_SECTOR_SIZE) / 4;
		if (lMaxClust32 > MAX_CLUST_32)///#define MAX_CLUST_32	((1 << 28) - 16)
			lMaxClust32 = MAX_CLUST_32;

		if (lClust32 > lMaxClust32) 
		{
			lClust32 = 0;
		}
		
		if (lClust32 )///mine
			break;
		
		mbs.cluster_size <<= 1;
	} while (mbs.cluster_size && mbs.cluster_size <= maxclustsize);

	if (!lClust32)
	{
		printf("\nAttempting to create a too large file system\n");
		return false;
	}

	//到此处就为cluster_size选择了一个合适的值
	mbs.fat32.fat32_length = CT_LE_L(lFatLength32);
	//填写参数sectors  total_sect
	if (lTotalSectors >= 65536)
	{
		mbs.sectors[0] = (char)0;
		mbs.sectors[1] = (char)0;
		mbs.total_sect = CT_LE_L(lTotalSectors);
	}
	else
	{
		mbs.sectors[0] = (char)(lTotalSectors & 0x00ff);
		mbs.sectors[1] = (char)((lTotalSectors & 0xff00) >> 8);
		mbs.total_sect = CT_LE_L(0);
	}

	/* The two following vars are in hard sectors, i.e. 512 byte sectors! */
	unsigned long lStartDataofSector = (DBR_RESERVED_SECTORS + DEFAULT_FAT_NUM*lFatLength32);
	unsigned long lStartDataofBlock = (lStartDataofSector + SECTORS_PER_BLOCK - 1) / SECTORS_PER_BLOCK;
	if (cblocks < lStartDataofBlock + 32)	/* Arbitrary undersize file system! */
	{
		printf("\nToo few blocks for viable file system\n");
		return false;
	}


	//下面是填写fat32表和根目录文件。
	unsigned char* pFat = NULL; 
	unsigned char* pZeroFat = NULL;
	if ((pFat = (unsigned char *)malloc(DEFAULT_SECTOR_SIZE)) == NULL)
	{
		printf("\nunable to allocate space for FAT image in memory\n");
		return false;
	}
	memset(pFat, 0, DEFAULT_SECTOR_SIZE);
	
	if ((pZeroFat = (unsigned char *)malloc(DEFAULT_SECTOR_SIZE)) == NULL)
	{
		printf("\nunable to allocate space for FAT image in memory\n");
		free(pFat);
		return false;
	}
	memset(pZeroFat, 0, DEFAULT_SECTOR_SIZE);

	//填写fat表的第0，1簇
	Mark_FAT_Cluster(0, 0xffffffff,pFat);	/* Initial fat entries */
	pFat[0] = (unsigned char)mbs.media;	/* Put media type in first byte! */
	Mark_FAT_Cluster(1, 0xffffffff,pFat);

	//文件所占的簇数
	unsigned long lClustersofFile = filesize/(mbs.cluster_size*DEFAULT_SECTOR_SIZE);
	//整个FAT可写的文件数
	unsigned long lFileNum = (lFatLength32*DEFAULT_SECTOR_SIZE/4-2)/lClustersofFile;
	//根目录所占的簇数
	unsigned long lClustersofRoot = (lFileNum+1)*32/(mbs.cluster_size*DEFAULT_SECTOR_SIZE)+1;


	//下面填写根目录所在的簇，FAT32将根目录等同于普通的文件
	for(unsigned long nRootCluCount = 0; nRootCluCount < lClustersofRoot; nRootCluCount++)//lClustersofRoot一般为1
	{
		if(nRootCluCount == (lClustersofRoot - 1))
		{
			Mark_FAT_Cluster(nRootCluCount+2,0xffffffff,pFat);
		}
		else
		{
			Mark_FAT_Cluster(nRootCluCount+2,nRootCluCount+3,pFat);
		}
	}


	//根目录文件的大小(字节数)
	unsigned long lRootFileSize = lClustersofRoot*mbs.cluster_size*DEFAULT_SECTOR_SIZE;
	struct msdos_dir_entry* pRootDir = NULL; //指向根目录文件
	if ((pRootDir = (struct msdos_dir_entry *)malloc (lRootFileSize)) == NULL)
	{
		free(pFat);		/* Tidy up before we die! */
		free(pZeroFat);
		printf("\nunable to allocate space for root directory in memory\n");
		return false;
	}
	memset(pRootDir, 0, lRootFileSize);


	//下面是写根目录文件
	
	//根目录的第0文件为：卷标
	////初始化第0个目录项(32个字节)//////////
	struct msdos_dir_entry *de = &pRootDir[0];  
	memcpy(de->name, "DVR", 11);
	de->attr = ATTR_VOLUME;//mine
	struct tm stCtimeTmp = {0};
	localtime_r( &CreateTime, &stCtimeTmp );
	struct tm* ctime = &stCtimeTmp;
	de->time = CT_LE_W((unsigned short)((ctime->tm_sec >> 1) +
		               (ctime->tm_min << 5) + (ctime->tm_hour << 11)));
	de->date = CT_LE_W((unsigned short)(ctime->tm_mday +
		               ((ctime->tm_mon+1) << 5) +
		               ((ctime->tm_year-80) << 9)));
	de->ctime_ms = 0;
	de->ctime = de->time;
	de->cdate = de->date;
	de->adate = de->date;
	de->starthi = CT_LE_W(0);
	de->start = CT_LE_W(0);
	de->size = CT_LE_L(0);
    
	//分区上要有几个特定的文件
	//unsigned long disksizeG=lTotalSectors*DEFAULT_SECTOR_SIZE/1024/1024/1024;
	LONGLONG diskSizeBytes = ((LONGLONG)lTotalSectors)*DEFAULT_SECTOR_SIZE;
    unsigned long disksizeG = diskSizeBytes/(1024*1024*1024);
	printf("disksizeG = %d\n",disksizeG);

	unsigned long lStartClu = lClustersofRoot + 2;//数据区从二开始编簇号,数据区先放根目录，然后才放其它文件（在mbr中已经规定好了）
	eventlogSizeM = (disksizeG/64)+1;
	operlogSizeM  = (((disksizeG/64)+1)*4);

	CreateFileItem(&pRootDir[1],"NORMALS BIN",lStartClu,1*1024*1024,mbs.cluster_size);
#if defined(__PACKID_9__) || defined(__PACKID_14__) || defined(__PACKID_27__)
	CreateFileItem(&pRootDir[2],"RECLOG  BIN",lStartClu, 5*1024*1024,mbs.cluster_size);
#else
	CreateFileItem(&pRootDir[2],"RECLOG  BIN",lStartClu,((disksizeG/5)+1)*1*1024*1024,mbs.cluster_size);
#endif
	CreateFileItem(&pRootDir[3],"EVENTLOGBIN",lStartClu,((disksizeG/64)+1)*1*1024*1024,mbs.cluster_size);
	CreateFileItem(&pRootDir[4],"OPERLOG BIN",lStartClu,(((disksizeG/64)+1)*4)*1*1024*1024,mbs.cluster_size);
	CreateFileItem(&pRootDir[5],"TEMPREC DAT",lStartClu, filesize,mbs.cluster_size);
	CreateFileItem(&pRootDir[6],"TEMPLOG BIN",lStartClu, ((disksizeG/5)+1)*1*1024*1024,mbs.cluster_size);

    
   	//有实际簇的个数得到的视频文件数
    unsigned long lActualAviNum = (lClust32 - (lStartClu -2))/lClustersofFile;
		
	//循环写视频文件
	//预留3个文件索引用于交换文件或升级文件
	unsigned long lAviCount = 0;
	while (lAviCount < lActualAviNum - 3)
	{
		char sz[13];
		int num = lAviCount;
		for(int j=0;j<8;j++)
		{
			sz[7-j]='0'+num%10;
			num=num/10;
		}
		strcpy(&sz[8],"DAT");
		CreateFileItem(&pRootDir[lAviCount+7],sz,lStartClu,filesize,mbs.cluster_size);
		lAviCount++;
	}
	lAviNum = lAviCount;

    //到此处根目录中的目录项都分配完成，只保留在内存pRootDir中，还没有写入硬盘中

	//下面写文件信息
	unsigned char* pInfoSector = NULL;
	if ((pInfoSector = (unsigned char *)malloc(DEFAULT_SECTOR_SIZE)) == NULL)
	{	
		free(pFat);		/* Tidy up before we die! */
		free(pZeroFat);
		free(pRootDir);
		printf("\nOut of memory\n");
		return false;		
	}
	memset(pInfoSector, 0, DEFAULT_SECTOR_SIZE);
	/* fsinfo structure is at offset 0x1e0 in info sector by observation */
	struct fat32_fsinfo* info = (struct fat32_fsinfo *)(pInfoSector + 0x1e0);	
	/* Info sector magic */
	pInfoSector[0] = 'R';
	pInfoSector[1] = 'R';
	pInfoSector[2] = 'a';
	pInfoSector[3] = 'A';	
	/* Magic for fsinfo structure */
	info->signature = CT_LE_L(0x61417272);
	/* We've allocated cluster 2 for the root dir. */
	info->free_clusters = CT_LE_L(lClust32-(lStartClu-2));//
	info->next_cluster = CT_LE_L(lStartClu);
	/* Info sector also must have boot sign */
	*(__u16 *)(pInfoSector + 0x1fe) = CT_LE_W(BOOT_SIGN);

	//到此处文件信息写完。

	//下面是把上面的内容，写到硬盘中
    //把分区的保留空间清零
	seekto( 0, "start of device" );
	BYTE nIndex = 0;
	for( nIndex = 0; nIndex < DBR_RESERVED_SECTORS; nIndex++)//写了32*512字节为0
	{
		if (write(fd,pZeroFat,DEFAULT_SECTOR_SIZE) != DEFAULT_SECTOR_SIZE)
		{
			FREE();
			return false;
		}
	}
    	
	//写dbr
	seekto( 0, "boot sector" );//写了512字节
	if(write(fd,(char *)&mbs, sizeof(struct msdos_boot_sector)) != DEFAULT_SECTOR_SIZE)
	{
		FREE();
		return false;
	}
	
	//写文件信息
	seekto(CF_LE_W(mbs.fat32.info_sector)*DEFAULT_SECTOR_SIZE, "info sector" );//定位到第512个字节
	if(write(fd,(char *)pInfoSector,DEFAULT_SECTOR_SIZE) != DEFAULT_SECTOR_SIZE)
	{
		FREE();
		return false;
	}

	//写backup boot sector
	seekto(CF_LE_W(mbs.fat32.backup_boot)*DEFAULT_SECTOR_SIZE, "backup boot sector" );
	if(write(fd,(char *)&mbs, sizeof (struct msdos_boot_sector)) != DEFAULT_SECTOR_SIZE)
	{
		FREE();
		return false;
	}

	//写FAT表中，在前面已经写了一部份了
	seekto(DBR_RESERVED_SECTORS*DEFAULT_SECTOR_SIZE, "first FAT" );
	for (int nFatCount = 1; nFatCount <= DEFAULT_FAT_NUM; nFatCount++)
	{
		//对fat表是一个个扇区的大小写入硬盘中
		int nPageNum = lFatLength32/6;//用于显示格式的进度
		int nClusterCount  =0; 
		unsigned long cu = 0;//用于记录当前簇
		struct msdos_dir_entry *pFile=&pRootDir[1];
		for (unsigned long fatno = 0; fatno < lFatLength32; fatno++)
		{
			unsigned char *pFatSector=pFat;
			if(fatno == 0)
			{
				pFatSector=pFat;
				cu=lClustersofRoot+2;
				nClusterCount = cu;
			}
			else
			{
				pFatSector=pZeroFat;
				memset(pZeroFat, 0, DEFAULT_SECTOR_SIZE);
				nClusterCount = 0;
			}
			bool bNewFile = true;
			unsigned long lEnd = 0;
			while((nClusterCount <  DEFAULT_SECTOR_SIZE/4) && (pFile->size > 0))
			{
				if (bNewFile)
				{
					lEnd = pFile->starthi;
					lEnd<<=16;
					lEnd|=pFile->start;
					lEnd += pFile->size/(mbs.cluster_size*DEFAULT_SECTOR_SIZE);
					if((pFile->size%(mbs.cluster_size*DEFAULT_SECTOR_SIZE)) == 0)
					{
						lEnd--;
					}					
				}	
				if(cu < lEnd)//当前簇小于文件的簇数
				{
					Mark_FAT_Cluster(cu,cu+1,pFatSector);
					bNewFile = false;
				}
				else
				{
					Mark_FAT_Cluster(cu,0xffffffff,pFatSector);
					pFile++;
					bNewFile = true;
				}
				cu++;
				nClusterCount++;
			}
			
			if (write(fd,(char *)pFatSector, DEFAULT_SECTOR_SIZE) != DEFAULT_SECTOR_SIZE)
			{
				FREE();
				return false;
			}

			if (!(fatno % nPageNum))
			{
//				m_nPercent++;	
//				if(m_PercentCallBack)
//				{
//					m_PercentCallBack(m_nPercent, m_pPara);
//				}
			}
			//printf("\rfat%d:finsh %ld/%ld", nFatCount,fatno,lFatLength32);
		}
	//	printf("\rfat%d:finsh %ld/%ld\n", nFatCount,lFatLength32,lFatLength32);
		
	}
	

	//写根目录文件
	if (write(fd,(char *)pRootDir, lRootFileSize) != (int)lRootFileSize)
	{
		FREE();
		return false;
	}
	FREE();
	sync();
	sleep(2);
	return true;
	
}
#endif

int CDiskManager::GetParttionInfo(char *diskname, PART_INFO *pPartitionInfo, BYTE *pPartitionNum)
{
	struct partition *pParttemp = NULL;
	int nIndex = 0; //记录分区的索引号
	BYTE nPartNum = 0; //记录分区的个数
	int nExtendId = -1; //记录在主分区表中扩展分区的ID
	int nClearedPartNum = 0; //全零分区项的个数
	int nExtendPartNum = 0; //扩展分区项的个数
	int nLength = 0;
	char szDiskBoot[BOOT_LENGTH] = {0};
    int fd = -1;
    
	fd = open(diskname,O_RDWR);
	if (fd < -1)
	{
		printf("(%s,%d):%s\n", __FILE__, __LINE__, diskname);
		return -1;
	}

	//读磁盘第一扇区
	nLength = read(fd, szDiskBoot, sizeof(szDiskBoot));
	if (nLength < sizeof(szDiskBoot))
	{
		assert(false);
		close(fd);
		return -1;
	}
	
	//判断最后的两个字节是否为“55AA”
	if (!Valid_Part_Table_Flag((unsigned char *)szDiskBoot)) 
	{
		close(fd);
		return -1;
	}

	//获取主分区表中的信息，那些分区项是基本分区，那个是扩展分区
	/*********************************************************
	*             MBR的组成
 	*         446个字节的引导程序
	*         16个字节的分区一        
	*         16个字节的分区二     
	*         16个字节的分区三     
	*         16个字节的分区四
	*         两个字节的结束标志“55AA“
	*********************************************************/
	for (nIndex = 0; nIndex < DPT_PARTTION; nIndex++)
	{	
		pParttemp = pt_offset(szDiskBoot,nIndex);
		if (Is_Cleared_Partition(pParttemp))
	   	{
			nClearedPartNum++;
			continue;
		}
			
		//如果是扩展分区，就记录其在分区表中的索引
		if (EXTENDED == pParttemp->sys_ind || 0xf == pParttemp->sys_ind)
		{
			nExtendPartNum++;
			nExtendId = nIndex;
			continue;
		}
		
		//到此处，说明该分区项的类型是除扩展分区之外的类型
		PART_INFO *p = &pPartitionInfo[nPartNum];	
		ULONG uSects = Get_Nr_Sects(pParttemp);
		uSects /= 1024/DEFAULT_SECTOR_SIZE;
		p->partsize = uSects/1024; //分区的大小，单位M
		p->partformat = pParttemp->sys_ind;//分区的类型
		snprintf(p->devnod, sizeof(p->devnod), "%s%d", diskname, nIndex + 1);//分区的名称如: /dev/hda1
		nPartNum++;
	}

	//如果分区项全为零，说明该磁盘还没有分区
	//如果扩展分区的个数多余一个，说明该磁盘分区有误
	if (nClearedPartNum == DPT_PARTTION)
	{
		close(fd);
		*pPartitionNum = 0;
		return -1;
	}
	
	if (nExtendPartNum >= 2)
	{
		close(fd);
		*pPartitionNum = 0;
		return -1;
	}

    //没有扩展分区的情况
	if (-1 == nExtendId)
	{
		close(fd);
		*pPartitionNum = nPartNum;
		return 0;
	}
	
	//下面是有扩展分区的情况
	unsigned char szEbrBoot[BOOT_LENGTH] = {0};
	pParttemp = pt_offset(szDiskBoot,nExtendId);
	loff_t uExtendBase = Get_Start_Sect(pParttemp);   //扩展分区的基本绝对起始扇区
	loff_t uEbrStart = 0;
	nIndex = DPT_PARTTION + 1; //扩展分区的起始索引为5
	while (!Is_Cleared_Partition(pParttemp))
	{
		
		//扩展mbr的起始字节
		if (uExtendBase == (loff_t)Get_Start_Sect(pParttemp))
		{
			uEbrStart = (loff_t)(Get_Start_Sect(pParttemp)) * DEFAULT_SECTOR_SIZE;
		}
		else
		{
			uEbrStart = ((loff_t)(Get_Start_Sect(pParttemp)) + uExtendBase) * DEFAULT_SECTOR_SIZE;
		}
	
		//读取扩展mbr的内容
#if defined(__TDNXP__)
		lseek(fd, uEbrStart,SEEK_SET);
#else
		llseek(fd, uEbrStart,SEEK_SET);
#endif
		read(fd,szEbrBoot,BOOT_LENGTH);
		pParttemp = pt_offset(szEbrBoot,0);
		PART_INFO *p = &pPartitionInfo[nPartNum];
		ULONG uSects = Get_Nr_Sects(pParttemp);
		uSects /= 1024/DEFAULT_SECTOR_SIZE;
		p->partsize = uSects/1024; //单位M
		p->partformat = pParttemp->sys_ind; //分区格式
		snprintf(p->devnod, sizeof(p->devnod), "%s%d", diskname, nIndex++);		
	    nPartNum++;

		//获取下一扩展分区
		pParttemp = pt_offset(szEbrBoot,1);	
			
	}
	
	close(fd);
	*pPartitionNum = nPartNum;
	return 0;
}

void CDiskManager::SetParttion(char* pszMbr, BYTE nIndex, 
							 unsigned long start,unsigned long stop, 
							 BYTE nSys,struct hd_geometry &ge)
{
	struct partition *p = pt_offset(pszMbr, nIndex);
	p->boot_ind = 0;
	p->lock_status = 0;
	p->sys_ind = nSys;
	Set_Start_Sect(p, start);
	Set_Nr_Sects(p, stop - start + 1);
	Set_CHS(p->head, p->sector, p->cyl, start,ge);
	Set_CHS(p->end_head, p->end_sector, p->end_cyl, stop,ge);
}

#if !defined(__TDNXP__)
int CDiskManager::ValidOffset(int fd, loff_t offset)
{
	char ch;	
	if (llseek (fd, offset, SEEK_SET) < 0) ///定位文件指针,把文件指针设定到相对于whence值偏移offset的位置
					///成功时返回新指针位置
		return 0;
	if (read (fd, &ch, 1) < 1)
		return 0;
	return 1;
}
#endif

void CDiskManager::CreateFileItem(msdos_dir_entry *de,char *pFileName,
		                unsigned long& start, unsigned long filesize,
						unsigned char nClusterSize)
{
	struct tm *ctime;
	time_t createtime = 0;
	struct tm stCtimeTmp = {0};

	time(&createtime);
	strcpy(de->name, pFileName);
	de->attr = 0x20;	
	localtime_r(&createtime, &stCtimeTmp);//
	ctime = &stCtimeTmp;
	de->time = CT_LE_W((unsigned short)((ctime->tm_sec >> 1) +
		               (ctime->tm_min << 5) + 
					   (ctime->tm_hour << 11)));
	de->date = CT_LE_W((unsigned short)(ctime->tm_mday +
		               ((ctime->tm_mon+1) << 5) +
					   ((ctime->tm_year-80) << 9)));
	de->ctime_ms = 0;
	de->ctime = de->time;
	de->cdate = de->date;
	de->adate = de->date;
	de->starthi = CT_LE_W(start>>16);
	de->start = CT_LE_W(start&0xffff);
	de->size = filesize;
	de->lcase = 0x18;//mine 

	start+=filesize/(nClusterSize*DEFAULT_SECTOR_SIZE);
	if((filesize % (nClusterSize*DEFAULT_SECTOR_SIZE)) != 0)
		start+=1;

}




int CDiskManager::GetDiskUseSpace(DISK_INFO &diskInfo)
{

	if (NET_DISK_TYPE_REMOVEABLE == diskInfo.diskType)
	{
		ULONG freeSizeM = 0;
		for (int i = 0; i < diskInfo.partitionNum; ++i)
		{
			PART_INFO *pPartion = &(diskInfo.partitionInfo[i]);
			struct statfs stattmp;
			memset(&stattmp, 0, sizeof(stattmp));
			if (statfs(pPartion->partpath, &stattmp) != -1)
			{
				unsigned long lTotal = (stattmp.f_blocks >> 10)*(stattmp.f_bsize >> 10);
				unsigned long lAvail = (stattmp.f_bavail >> 10)*(stattmp.f_bsize >> 10);
				pPartion->freespace = lAvail;
				pPartion->usedspace = lTotal - lAvail;
				freeSizeM += lAvail;
			}
		}
		
		diskInfo.freesize = freeSizeM;
		return 0;

	}
	else
	{
		assert(false);
	}

	return 0;


}


int CDiskManager::MakeNode(char *pDiskname, char *pMountDir, unsigned char startMountDirId, unsigned char partitionNum)
{
	int distance = 0;
	if (('h' == pDiskname[5]) && ('d' == pDiskname[6]))
	{
		distance = 64;
	}
	else
	{
		distance = 16;
	}
	
	printf("######%s\n",pDiskname);
	int nStart = (pDiskname[7] - 'a') * distance;
	char cmd[100] = {0};
	if (nStart >= 0)
	{	
		sprintf(cmd, "mknod %s b 8 %d", pDiskname, nStart);
		DVRSystem(cmd);
		printf("%s\n", cmd);
		
		for (int i = 1; i <= partitionNum; ++i)
		{
			sprintf(cmd, "mknod %s%d b 8 %d", pDiskname, i, nStart + i);
			DVRSystem(cmd);			
			printf("%s\n", cmd);
		}
	}

	if (pMountDir != NULL)
	{
		for (int i = 0; i < partitionNum; ++i)
		{
			sprintf(cmd, "mkdir -p %s/%02d", pMountDir, startMountDirId + i);
			DVRSystem(cmd);
			printf("%s\n", cmd);
		}

	}	
	return 0;

}

int CDiskManager::DvrIdnCmp(BYTE *DvrId1, BYTE *DvrId2, int num)
{
	int ret = 0;
	for (int i = 0; i < num; ++i)
	{
		if (DvrId1[i] != DvrId2[i])
		{
			ret = -1;
			break;
		}
	}
	return ret;
}



int CDiskManager::Set_Sata_Sleep(char *szDevice)
{
	int fd = 0;

	static __u8 args[512] = { 0 };

	void *data = (void *)(args);
	unsigned int data_bytes = 512;

	unsigned char cdb[SG_ATA_16_LEN] = { 0 };
	unsigned char sb[32], *desc;
	unsigned char ata_status, ata_error;
	struct sg_io_hdr io_hdr;

	if(!strstr(szDevice,"/dev/sd"))
	{
		printf("Device name error!\n");
		return -1;
	}

	if(isdigit(szDevice[strlen(szDevice) -1]))
	{
		printf("Warning:Not a partision ,should be Device!stop!\n");
		return -1;
	}


	//打开设备
	fd = open(szDevice, O_RDONLY);
	if (fd < 0) 
	{
		printf("open %s error\n",szDevice);
		return -1;
	}


	//设置cmdp
	memset(cdb,0,sizeof(cdb));
	cdb[0] = SG_ATA_16;
	cdb[1] = SG_ATA_PROTO_NON_DATA;//SG_ATA_PROTO_PIO_IN;
	cdb[2] = SG_CDB2_CHECK_COND;
	if(0)
	{
		cdb[2] |= SG_CDB2_TLEN_NSECT | SG_CDB2_TLEN_SECTORS;
		cdb[2] |= SG_CDB2_TDIR_FROM_DEV;
	}
	cdb[13] = ATA_USING_LBA;
	cdb[14] = ATA_OP_STANDBYNOW1;//ATA_OP_IDENTIFY;

	//设置sdp
	memset((void*)&(sb[0]), 0, sizeof(sb));

	//设置sg_io_hdr结构
	memset((void*)&io_hdr, 0, sizeof(struct sg_io_hdr));
	io_hdr.interface_id    	= 'S';
	io_hdr.cmd_len        	= SG_ATA_16_LEN;
	io_hdr.mx_sb_len    	= sizeof(sb);
	io_hdr.dxfer_direction	= SG_DXFER_FROM_DEV;
	io_hdr.dxfer_len    		= data_bytes;
	io_hdr.dxferp        		= data;
	io_hdr.cmdp        		= cdb;
	io_hdr.sbp        		= sb;
	io_hdr.timeout        	= 5000; /* msecs */


	//调用ioctl
	if (ioctl(fd, SG_IO, &io_hdr) == -1) 
	{
		cdb[14] = ATA_OP_STANDBYNOW2;		
		memset((void*)&(sb[0]), 0, sizeof(sb));
		if(ioctl(fd, SG_IO, &io_hdr) == -1)
		{
			fprintf(stderr, "SG_IO ioctl not supported\n");
			close(fd);
			return -1;    /* SG_IO not supported */
		}
	}


	//检查sg_io_hdr中的状态值
	if (io_hdr.host_status || io_hdr.driver_status != SG_DRIVER_SENSE
		|| (io_hdr.status && io_hdr.status != SG_CHECK_CONDITION))
	{
		errno = EIO;
		close(fd);
		return -2;
	}

	//检查sdp中的状态值
	if (sb[0] != 0x72 || sb[7] < 14) 
	{
		errno = EIO;
		close(fd);
		return -3;
	}
	desc = sb + 8;
	if (desc[0] != 9 || desc[1] < 12)
	{
		errno = EIO;
		close(fd);
		return -4;
	}

	ata_error = desc[3];
	ata_status = desc[13];
	if(ata_status & (0x01 |(1 << 3)))
	{    /* ERR_STAT */
		errno = EIO;
		close(fd);
		return -5;
	}

	close(fd);

	{
		POS healthPos = m_healthFixedDiskList.GetHeadPosition();
		while (healthPos != NULL)
		{
			DISK_HEALTH_INFO &diskHealthInfo = m_healthFixedDiskList.GetNext(healthPos);
			if (0 == strcmp(diskHealthInfo.szDiskName, szDevice))
			{
				diskHealthInfo.bSleep = 1;
				break;
			}
		}

	}

	return 0;
}

//
//
//int CDiskManager::CalEsataIndex()
//{
//	int nInterface = m_DiskPara.nEsataInterface;
//	if (m_DiskPara.bHaveEsata)
//	{
//		if (nInterface < 1)
//		{
//			assert(false);
//			nInterface = 8;
//		}
//		nInterface--;
//	}
//	else
//	{
//		nInterface = -1;
//	}
//	
//	return nInterface;	
//}


/////***************************************S.M.A.R.T. begine ********************************/
/*   bAttrID                英文含义                               中文含义         
0x01			"Raw Read Error Rate     "          Raw 读取错误率
0x03			"Spin-Up Time            ",			累计加电时间
0x04			"Start/Stop Count        ",			开始/停止计时
0x05			"Reallocated Sector Count",			再分配扇区计数
0x07			"Seek Error Rate         ",			寻道错误率
0x09			"Power On Hours Count    ",			累计通电时间
0x0A			"Spin Retry Count        ", 		马达重试计数
0x0C			"Power Cycle Count       ", 		通电周期计数
0xBB			"Reported Uncorrectable  ", 		反馈无法校正的错误
0xBD			"High Fly Writes         ", 		高飞写入
0xBE			"Airflow Temperature     ", 		气流温度(西部数据)
0xC2			"Temperature             "			当前内部温度
0xC3			"Hardware ECC Recovered  "  		已恢复的硬件ECC
0xC5			"Current Pending Sector Count"		当前待映射扇区计数 
0xC6			"Uncorrectable Sector Count"		无法校正扇区计数 
0xC7			"UltraDMA CRC Error Count"			直接内存访问校验错误计数 
0xC8			"Write Error Rate        "			写入错误率 
0xCA			"UnKnown                 "          未知表征
*/
bool CDiskManager::GetDiskSmartInfoImp(char *pDiskName, NET_DISK_SMART_INFO &stInfo)
{	
	if (NULL == pDiskName)
	{
		return false;
	}

	int diskHandle = open(pDiskName, O_RDONLY | O_NONBLOCK);
	if (0 >= diskHandle)
	{		
		return false;
	}

	//读取smart信息
	unsigned char req[4+512] = {0}; 	
	req[0]= SMART_CMD;
	req[1]= 1;
	req[2]= READ_ATTRIBUTES;
	req[3]= 1;
	if(ioctl(diskHandle, 0x031f , &req))
	{		
		close(diskHandle);
		return false;
	}

	//读取smart阀值信息。
	unsigned char reqth[4+512] = {0};
	reqth[0]= SMART_CMD;
	reqth[1]= 1;
	reqth[2]= READ_THRESHOLDS;
	reqth[3]= 1;	
	if (ioctl(diskHandle, 0x031f , &reqth))
	{	
		close(diskHandle);
		return false;
	}

	PDRIVEATTRIBUTE pDt  =  (PDRIVEATTRIBUTE)&req[6];
	PATTRTHRESHOLD pThres = (PATTRTHRESHOLD)&reqth[6];
	int health = 0;
	int nCount = 0;
	for (int i=0; i<30; i++)
	{
		if (pDt->bAttrID)
		{
			stInfo.smartInfo[nCount].nIndex     = pDt->bAttrID;
			stInfo.smartInfo[nCount].nCurrent   = pDt->bAttrValue;
			stInfo.smartInfo[nCount].nWorest    = pDt->bWorstValue;
			stInfo.smartInfo[nCount].nThreshold = pThres->bWarrantyThreshold;
			stInfo.smartInfo[nCount].nRawValue  = *((unsigned int *)pDt->bRawValue);
			if (stInfo.smartInfo[nCount].nCurrent < stInfo.smartInfo[nCount].nThreshold)
			{
				health++;
				stInfo.smartInfo[nCount].nStatus = 1;
			}
			nCount++;
			if (nCount>=20)
			{
				break;
			}
		}
		pDt++;
		pThres++;
	}

	if (health <= 0)
	{
		stInfo.diskHealth = 0;
	}
	else if (health <= 2)
	{
		stInfo.diskHealth = 1;
	}
	else
	{
		stInfo.diskHealth = 2;
	}


	close(diskHandle);

	return true;
}


bool CDiskManager::GetDiskModuleImp(char *pDiskName, NET_DISK_SMART_INFO &stInfo)
{
	if (NULL == pDiskName)
	{
		return false;
	}

	int diskHandle = open(pDiskName, O_RDONLY | O_NONBLOCK);
	if (0 >= diskHandle)
	{		
		return false;
	}

	//
	unsigned char req[4+512] = {0}; 
	req[0]= 0xEC;
	req[1]= 1;
	req[2]= 0;
	req[3]= 1;
	if(ioctl(diskHandle, 0x031f , &req))
	{
		close(diskHandle);
		return false;
	}

	//
	ST_IDSECTOR stIdSector;
	memset(&stIdSector, 0, sizeof(stIdSector));
	memcpy(&stIdSector, &req[4], sizeof(stIdSector));

	CopyString((char *)stInfo.szModelNumber, (char *)stIdSector.sModelNumber, 39);
	CopyString((char *)stInfo.szSerialNumber, (char *)stIdSector.sSerialNumber, 20);
	CopyString((char *)stInfo.szFirmwareRev, (char *)stIdSector.sFirmwareRev, 8);


	close(diskHandle);
	return true;
}


void CDiskManager::CopyString(char *pDesStr, char *pSouStr, unsigned long nLen)
{	
	char szTemp[48] = {0};
	for(int nCount = 0; nCount < nLen; nCount += 2)
	{
		szTemp[nCount] = pSouStr[nCount+1];
		szTemp[nCount+1] = pSouStr[nCount];
	}

	char *p = &szTemp[0];
	char *p1 = p + strlen(szTemp) - 1;
	int nLenTemp = strlen(szTemp);
	if (nLenTemp > 0)
	{		
		while(*p && isspace(*p)) p++;
		while(p1 > p && isspace(*p1)) *p1-- = '\0';
		nLenTemp = strlen(p);
	}

	if (nLenTemp > 0)
	{
		memcpy(pDesStr, p, strlen(p));
	}	
}
int CDiskManager::GetDiskCount()
{
	//搜索固定硬盘
	DISK_INFO_LIST tmpDiskList;
	//ESATA_INDEX_LIST tmpeSataIndexList;
	//因为初始化比GetDiskCount快，所以此时m_eSataIndexList已经有Esata信息
	m_SearchDiskTool.SearchFixedDisk(tmpDiskList, m_eSataIndexList);
	int disknum=tmpDiskList.GetCount();
	tmpDiskList.RemoveAll();
	return disknum;
}

/*
int main(int argc, char **argv)
{
	DISK_INIT_INFO DiskInitInfo;
	CDiskManager* pDiskMan = CDiskManager::Instance();

	DiskInitInfo.productId = 0;

	for (int i = 0; i < 0; i++)
	{
		DiskInitInfo.dvrId[i] = 0;
	}
	
	DiskInitInfo.bHaveEsata = false;
    DiskInitInfo.partitionNum = 4;
    DiskInitInfo.fileSizeM = 64;

	pDiskMan->Initial(&DiskInitInfo);

	//pDiskMan->FormatFixedDisk("/dev/sda", NULL, NULL);
	pDiskMan->FormatDisk(DRIVER_TYPE_FIXEDDISK, "/dev/sda", NULL, NULL);
	


}


*/




