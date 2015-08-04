
/***********************************************************************
** Copyright   Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : 
** Date         : 2007-09-14
** Name         : diskmanager.cpp
** Version      : 1.0
** Description  : ���̹���ģ���ʵ���ļ�
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

//�����ṩ�ӿ�


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
	//��ֹ��201205041130��֧�ֵĲ�Ʒ��������Щ������Ժ�Ҫ�����²�Ʒ����Ҫ�޸����ͬʱҪ�������ӵĲ�Ʒ����һ����
	//��diskversion
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
	/*����Ժ�Ҫ�����²�Ʒ����Ҫ�޸����ͬʱҪ�������ӵĲ�Ʒ����һ����
	��diskversion.��ÿһ��������һ����Ʒ����ʹ��һ���µ�diskversion.
	��ʼdiskversion��7.0
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
	

	//ͳ�ƻ���֧�ֵ�esata�ı��
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
	
	
	//�����̶�Ӳ��
	m_SearchDiskTool.SearchFixedDisk(m_FixedDiskList, m_eSataIndexList);

	//��ȡÿ��Ӳ�̵���Ϣ
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
	
	//����ÿ��Ӳ��	
	pos = m_FixedDiskList.GetHeadPosition();
	while (pos != NULL)
	{
		DISK_INFO &diskInfo = m_FixedDiskList.GetNext(pos);
		MountFixedDisk(diskInfo, m_nFixedEndPartId);		
	}

	//��ֻ��һ��Ӳ���������������ģ�����԰����Ӳ���޸ı���Ӳ��
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



	//��ȡ������Ӳ�̵Ķ�Ӧ��ϵ
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
		//��������豸�Ƿ��Ѿ�������
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
			diskInfoTmp.reserved	  = diskInfo.interfaceindex;		//����
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
		//m_RemoveableDiskList�д�ŵ��ǣ����й��صģ�������ʹ�õ��豸
		DISK_INFO_LIST diskUseList;
		POS pos = m_RemoveableDiskList.GetHeadPosition();
		while (NULL != pos)
		{
			DISK_INFO &tmp = m_RemoveableDiskList.GetAt(pos);
			diskUseList.AddTail(tmp);
			m_RemoveableDiskList.GetNext(pos);
		}
		
		//allDiskInfoList�д�ŵ��ǣ���ǰʱ�̣����и����͵��豸
		DISK_INFO_LIST allDiskInfoList;
		m_SearchDiskTool.SearchRemoveableDisk(&allDiskInfoList,m_eSataIndexList, m_DiskPara.szUsbToEsataPath);
        
		//��diskUseList��ɾ����û��ж�ؾͰε����豸
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
		
		//��ʱ��diskUseList�е��豸��������ʹ�õġ�,
		//allDiskInfoList��ֻ���������ӵ��豸��

		//ˢ��diskUseList�е��豸ʹ�ÿռ�
		pos = diskUseList.GetHeadPosition();
		while (NULL != pos)
		{
			DISK_INFO &diskInfo = diskUseList.GetAt(pos);
			GetDiskUseSpace(diskInfo);
			diskUseList.GetNext(pos);
		}
		
		//��allDiskInfoListʣ����豸,������ʱ���أ�Ŀ���ǣ��õ����豸����ϸ��Ϣ
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

			//Ѱ�ҹ���Ŀ¼
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
		

		//��diskUseList��allDiskInfoList�е��豸��Ϣ��ת����������Ҫ�ĸ�ʽ
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
		//m_RemoveableDiskList�д�ŵ��ǣ����й��صģ�������ʹ�õ��豸
		DISK_INFO_LIST diskUseList;
		POS pos = m_RemoveableDiskList.GetHeadPosition();
		while (NULL != pos)
		{
			DISK_INFO &tmp = m_RemoveableDiskList.GetAt(pos);
			diskUseList.AddTail(tmp);
			m_RemoveableDiskList.GetNext(pos);
		}


		//allDiskInfoList�д�ŵ��ǣ���ǰʱ�̣����и����͵��豸
		DISK_INFO_LIST allDiskInfoList;
		m_SearchDiskTool.SearchRemoveableDisk(&allDiskInfoList,m_eSataIndexList,m_DiskPara.szUsbToEsataPath);

		//��diskUseList��ɾ����û��ж�ؾͰε����豸
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

		//��ʱ��diskUseList�е��豸��������ʹ�õġ�,
		//allDiskInfoList��ֻ���������ӵ��豸��

		//ˢ��diskUseList�е��豸ʹ�ÿռ�
		pos = diskUseList.GetHeadPosition();
		while (NULL != pos)
		{
			DISK_INFO &diskInfo = diskUseList.GetAt(pos);
			GetDiskUseSpace(diskInfo);
			diskUseList.GetNext(pos);
		}

		//��allDiskInfoListʣ����豸,���й��أ�Ŀ���ǣ��õ����豸����ϸ��Ϣ
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

			//Ѱ�ҹ���Ŀ¼

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


		//��diskUseList��allDiskInfoList�е��豸��Ϣ��ת����������Ҫ�ĸ�ʽ
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

//////�����ǶԶԹ̶�Ӳ�̵Ĳ���
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


	//������й��ز���
	BYTE nStart = startPartId;
	char szCmd[MAX_PATH] = {0};
	for (int i = 0; i < diskInfo.partitionNum; ++i)
	{
		PART_INFO *p = &diskInfo.partitionInfo[i];
		p->partid = nStart;
		snprintf(p->partpath, sizeof(p->partpath), "%s/%02d", m_DiskPara.szDiskMountPath, nStart);		
		//��ж�أ�Ȼ���ٹ���
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
			sync();  //��Ϊ����ļ��������жϣ�Ӳ���Ƿ��ʽ�ˡ�
			         //������Ҫ�ѻ���Ķ�����д��Ӳ��
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
			//�����Ʒ������ͬ�����ǲ�Ʒ��ʾ��mac��ַ����ͬ��˵�����Ӳ���Ǳ���豸�ϵģ���ô���Ӳ��ֻ�ܶ�������д
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

	
	//���˴���Ӳ�̿�������¼����
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
		bBIgDiskCheck =true;//��Ӳ��
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

	//��ȡ������Ӳ�̵Ķ�Ӧ��ϵ
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

	//�������û�й��أ���û�б�Ҫж��
	if (NET_DISK_NORMAL != DiskInfo.diskStatus)
	{
		return 0;
	}

	//ж��ÿһ������
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

	
	//���ø��豸�ѹ���
	DiskInfo.diskStatus = NET_DISK_NO_FORMAT;
	return 0;
}

////�����ⲿ���Ƕ��Ȱβ��豸����
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

	//������й��ز���
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
			//��ж�أ�Ȼ���ٹ���
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

   
	//���˴���Ӳ�̿�������¼����
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

	//�������û�й��أ���û�б�Ҫж��
	if (NET_DISK_NO_MOUNT == DiskInfo.diskStatus)
	{
		return 0;
	}

	//ж��ÿһ������
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

	//���ø��豸�ѹ���
	DiskInfo.diskStatus = NET_DISK_NO_MOUNT;
	return 0;

}



////�����ⲿ���ǹ���Ӳ�̣�U��һЩͨ�ò���
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

//��С�˴洢��ת�������� 
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
	unsigned long lTotalSectors;             //��������
	struct hd_geometry geometry;             //Ӳ�̵ļ��β���
	unsigned long lUnits;                    //������λ��������Ϊ��λ
	unsigned long lTotalCylinders;           //�ܹ�������
	unsigned long lUnitsPerParttion;           
	unsigned long start;                     //�ڷ��������м�¼ĳ����������ʼ����
	unsigned long stop;                      //�ڷ��������м�¼ĳ�������Ľ�������
	unsigned char nMainPartNum;
	unsigned char nExtendPartNum;
	unsigned char szEbrBoot[BOOT_LENGTH] = {0};
	loff_t uExtendBase = 0;   				//��չ�����Ļ���������ʼ����
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

	//��ʼ����
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
	�Ȱѷ������ã��ڸ�÷�����ͷ����֮���������˳������ΪCRCУ��ֵ�Ļ���ǰ������˳��ġ�
	partitionNum ��ֵ����ʱ����4�ı����ķ�����ʽ����ʱ��Ϊ4�������ˡ�
	���������Է�128���������Ǻǡ�
	*/
	//��ʱǿ�Ʒ�Ϊ4������
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

	//��ȥ 10M �Ŀռ䲻����
	sectors -= 10*1024*1024;

	//�ó�ÿ�������Ĵ�С(LBA)
	LONGLONG partitionSize = sectors/partitionNum;

	LONGLONG leftSize = sectors%partitionNum;

	ULONGLONG 	tmpSize;

	//��ʼ���ø������������ֵ��
	for ( int i = 0; i < partitionNum; i++)
	{
		//��������Ϊ: linux���ݷ������͡�
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



		//����Ψһ��GUID����˵UUID
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
	��Ū��GPTͷ��
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
	gptHeader.currentLBA 	= 1; //�϶��� 1 ��
	gptHeader.backupLBA 	= sectors - 1; //���������һ��LBA
	gptHeader.firstUsableLBA = 0x22;
	gptHeader.lastUsableLBA = sectors - 34;//ֵ�����Լ��������


	//uuid_clear(gptHeader.diskGUID);
	//GetUUID( &(gptHeader.diskGUID) );
	uuid_generate(gptHeader.diskGUID);

	//ReverseBytes( gptHeader.diskGUID.data4, 8 );


	gptHeader.partitionEntriesLBA = 2;
	gptHeader.numParts = 0x80;
	gptHeader.sizeOfPartitionEntries =  0x80;
	gptHeader.partitionEntriesCRC = GetCRC_Value ( ( char *) pTable, 32*DEFAULT_SECTOR_SIZE );
	//������ֵ֮�����¼��� headerCRC
	gptHeader.headerCRC = GetCRC_Value ( ( char *) &gptHeader, 92 );


	/*
	��GPTͷ����GPT�������д����̣���������ֺ�����
	*/

	//�������ݷ�����ͷ
	struct __gpt_header SecondGptHead;
	memset( &SecondGptHead, 0, sizeof(struct __gpt_header) );
	SecondGptHead.signature = gptHeader.signature;
	SecondGptHead.revision = gptHeader.revision;
	SecondGptHead.headerSize = gptHeader.headerSize;
	SecondGptHead.headerCRC = 0;//��Ҫ���¸�ֵ�������Ҫ���¼�����
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
	SecondGptHead.partitionEntriesCRC = gptHeader.partitionEntriesCRC; //����ֱ�Ӹ�ֵ����Ϊ��ʱ��ԭ��ȥҲҪһ����

	SecondGptHead.headerCRC = GetCRC_Value ( ( char *) &SecondGptHead, 92);//���¼���headerCRC��ֵ��


	//���Ȱ�GPT��ͷ�ͷ����������
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

	//д��������
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

	//дGPTͷ
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

	//дMBR
	//�ȶ�ȡ����������
	llseek( fd, 0 , SEEK_SET );
	//��ȡmbr�е�����
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

	//���ý�����־
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

	//Ϊ�˱�֤�����ݴ浽������,ˢ���ں˻�����
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
	unsigned long lTotalSectors;		 //��������
	struct hd_geometry geometry;		 //Ӳ�̵ļ��β���
	unsigned long lUnits;				 //������λ��������Ϊ��λ
	unsigned long lTotalCylinders;		 //�ܹ�������
	unsigned long lUnitsPerParttion;		   
	unsigned long start;				 //�ڷ��������м�¼ĳ����������ʼ����
	unsigned long stop; 				 //�ڷ��������м�¼ĳ�������Ľ�������
	unsigned char nMainPartNum;
	unsigned char nExtendPartNum;
	unsigned char szEbrBoot[BOOT_LENGTH] = { 0 };
	loff_t uExtendBase = 0;   //��չ�����Ļ���������ʼ����
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
	
	//��ʼ����
	printf( "fdisk %s start...... %d\n", diskName,fd );
	
	//��ȡmbr�е�����
	nLength = read(fd, szMbrBuffer, BOOT_LENGTH);
	
	//�����������Ϣ
	nIndex	= 0;
	for ( nIndex  = 0; nIndex < DPT_PARTTION; nIndex++ )
	{
		pParttemp = pt_offset( szMbrBuffer, nIndex );		
		Clear_Partition(pParttemp);
	}

		
	//���Ӳ�̵�������
	if ( ioctl( fd, BLKGETSIZE, &lTotalSectors ) )
	{	
		close( fd );
		return -1;
	}

	//���Ӳ�̵�������
	if ( ioctl( fd, HDIO_GETGEO, &geometry ) )
	{
		close( fd );
		return -1;
	}

	lTotalCylinders = lTotalSectors/( geometry.heads * geometry.sectors );
   
	//��ȡ������λ,Ҳ����˵��������С����������ÿһ���������������
	lUnits = geometry.heads * geometry.sectors;
	
	//��ȡÿ�������������ٸ�������λ�������ٸ�����
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

	//д��������
	//��һ��������ʼ������ҪԤ������������
	start = geometry.sectors; // geometry.sectors == 63 XXX
	for ( nIndex = 0; nIndex < nMainPartNum; nIndex++ )
	{
		//�õ�������������������� XXX
		stop = ( nIndex + 1 ) * lUnitsPerParttion * lUnits - 1;
		SetParttion( szMbrBuffer, nIndex, start, stop, nSys, geometry );
		start = stop + 1;
	}

	//���ý�����־
	szMbrBuffer[BOOT_LENGTH - 2] = 0x55;
	szMbrBuffer[BOOT_LENGTH - 1] = 0xaa;

	//�ѷ��������Ϣд������������
	lseek(fd, 0, SEEK_SET);
	if ( write(fd, szMbrBuffer, BOOT_LENGTH) != BOOT_LENGTH )
	{
		close(fd);
		return -1;
	}

	//Ϊ�˱�֤�����ݴ浽������,ʹ��sync()��ˢ���ں˻�����
	sync();
	sleep(2);
	if ( ( ioctl(fd, BLKRRPART) ) != 0 )
		printf( "\nWARNING: Re-reading the partition table failed.\n" );

	printf( " partition NormalDisk %s ok!!!\n", diskName );
	close(fd);
	return 0;	
}



// �����豸�Ĵ�С�ˣ����֤�����ǵ��豸��С���豸��
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

//bigDiskFlag ���� true ��ô˵����ǰ�����Ĵ�����GPT���̡�
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

	//��Ҫ��֤����ʵʣ���������������Ӧ�ö�Ϊ0��������̻ᱻ������

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
	//��ȡ������������Ϣ��
	struct hd_geometry geometry;
	if (ioctl(fd, HDIO_GETGEO, &geometry))
		return false;

	//�õ������Ĵ�С
	LONGLONG cblocks = 0;	// ��λ�� 512 �ֽ�
	
	if ( true == bigDiskFlag )
	{
		//˵����ǰ�ǳ�����������
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
		
		
		//��ȥ 10M �Ŀռ䲻����
		cblocks -= 10*1024*2;
		
		//�ó�ÿ�������Ĵ�С(LBA)
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
		//�õ������Ĵ�С
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
		ValidOffset (fd, 0); //���ļ�ָ��ظ�����ʼλ��
		cblocks = (low + 1) / BLOCK_SIZE;
#endif	
	
	}
	
	///����ļ���״̬��Ϣ
	struct stat statbuf;
	if (fstat(fd, &statbuf) < 0)
		return false;
	if ( !S_ISBLK(statbuf.st_mode) )//����ļ���һ�����豸����S_ISBLK()������
	{
		statbuf.st_rdev = 0;///st_rdev;  /* device type (if inode device) */
	}
	
	//��ʼ��DBR�еĲ���������������
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
	mbs.dir_entries[1] = (char)0; //fat32�и�Ŀ¼Ҳ�����ļ�������fat16��ʹ�ø��ֶΡ�fat32ҪΪ0
	mbs.media = (char)0xf8; /* Set up the media descriptor for a hard drive */
	mbs.fat_length = CT_LE_W(0);//fat32Ҫ����ֶ�Ϊ0
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
    
	//FAT32��������չBPB�ֶ�  
	time_t CreateTime;
	time(&CreateTime);//�����ھ�꣬
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

	//���������cluster_size fat32_length��ȡֵ
	unsigned long long VolSize = cblocks*SECTORS_PER_BLOCK;//
	printf("cblocks=%lld\n",cblocks);
	//�ȸ�cluster_size��һ����ֵ
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

	//Ϊcluster_sizeѡ��һ�����ʵ�ֵ
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
	unsigned long lClust32; //��¼�������Ĵ���
	unsigned long lMaxClust32;
	unsigned long lFatLength32; //��¼fat32��ռ��������
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

	//���˴���Ϊcluster_sizeѡ����һ�����ʵ�ֵ
	mbs.fat32.fat32_length = CT_LE_L( lFatLength32 );
	//��д����sectors  total_sect
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

	//��������дfat32��͸�Ŀ¼�ļ���
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

	//��дfat��ĵ�0��1��
	Mark_FAT_Cluster(0, 0xffffffff,pFat);	/* Initial fat entries */
	pFat[0] = (unsigned char)mbs.media;	/* Put media type in first byte! */
	Mark_FAT_Cluster(1, 0xffffffff,pFat);

	//�ļ���ռ�Ĵ���
	unsigned long lClustersofFile = filesize/(mbs.cluster_size*DEFAULT_SECTOR_SIZE);
	//����FAT��д���ļ���
	unsigned long lFileNum = (lFatLength32*DEFAULT_SECTOR_SIZE/4-2)/lClustersofFile;
	//��Ŀ¼��ռ�Ĵ���
	unsigned long lClustersofRoot = (lFileNum+1)*32/(mbs.cluster_size*DEFAULT_SECTOR_SIZE)+1;

	//������д��Ŀ¼���ڵĴأ�FAT32����Ŀ¼��ͬ����ͨ���ļ�
	for(unsigned long nRootCluCount = 0; nRootCluCount < lClustersofRoot; nRootCluCount++)//lClustersofRootһ��Ϊ1
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


	//��Ŀ¼�ļ��Ĵ�С(�ֽ���)
	unsigned long lRootFileSize = lClustersofRoot*mbs.cluster_size*DEFAULT_SECTOR_SIZE;
	struct msdos_dir_entry* pRootDir = NULL; //ָ���Ŀ¼�ļ�
	if ((pRootDir = (struct msdos_dir_entry *)malloc (lRootFileSize)) == NULL)
	{
		free(pFat);		/* Tidy up before we die! */
		free(pZeroFat);
		printf("\nunable to allocate space for root directory in memory\n");
		return false;
	}
	memset(pRootDir, 0, lRootFileSize);

	//������д��Ŀ¼�ļ�
	
	//��Ŀ¼�ĵ�0�ļ�Ϊ�����
	////��ʼ����0��Ŀ¼��(32���ֽ�)//////////
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
    
	//������Ҫ�м����ض����ļ�
	//unsigned long disksizeG=lTotalSectors*DEFAULT_SECTOR_SIZE/1024/1024/1024;
	LONGLONG diskSizeBytes = ((LONGLONG)lTotalSectors)*DEFAULT_SECTOR_SIZE;
    unsigned long disksizeG = diskSizeBytes/(1024*1024*1024);
	printf("disksizeG=%d\n",disksizeG);	
	unsigned long lStartClu = lClustersofRoot + 2;//�������Ӷ���ʼ��غ�,�������ȷŸ�Ŀ¼��Ȼ��ŷ������ļ�����mbr���Ѿ��涨���ˣ�
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
	
    
   	//��ʵ�ʴصĸ����õ�����Ƶ�ļ���
    unsigned long lActualAviNum = (lClust32 - (lStartClu -2))/lClustersofFile;
		
	//ѭ��д��Ƶ�ļ�
	//Ԥ��һ���ļ��������ڽ����ļ��������ļ�
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

    //���˴���Ŀ¼�е�Ŀ¼�������ɣ�ֻ�������ڴ�pRootDir�У���û��д��Ӳ����

	//����д�ļ���Ϣ
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

	//���˴��ļ���Ϣд�ꡣ

	//�����ǰ���������ݣ�д��Ӳ����
    //�ѷ����ı����ռ�����
	seekto( 0, "start of device" );
	BYTE nIndex = 0;
	for( nIndex = 0; nIndex < DBR_RESERVED_SECTORS; nIndex++)//д��32*512�ֽ�Ϊ0
	{
		if (write(fd,pZeroFat,DEFAULT_SECTOR_SIZE) != DEFAULT_SECTOR_SIZE)
		{
			FREE();
			return false;
		}
	}
    	
	//дdbr
	seekto( 0, "boot sector" );//д��512�ֽ�
	if(write(fd,(char *)&mbs, sizeof(struct msdos_boot_sector)) != DEFAULT_SECTOR_SIZE)
	{
		FREE();
		return false;
	}
	
	//д�ļ���Ϣ
	seekto(CF_LE_W(mbs.fat32.info_sector)*DEFAULT_SECTOR_SIZE, "info sector" );//��λ����512���ֽ�
	if(write(fd,(char *)pInfoSector,DEFAULT_SECTOR_SIZE) != DEFAULT_SECTOR_SIZE)
	{
		FREE();
		return false;
	}

	//дbackup boot sector
	seekto(CF_LE_W(mbs.fat32.backup_boot)*DEFAULT_SECTOR_SIZE, "backup boot sector" );
	if(write(fd,(char *)&mbs, sizeof (struct msdos_boot_sector)) != DEFAULT_SECTOR_SIZE)
	{
		FREE();
		return false;
	}

	//дFAT���У���ǰ���Ѿ�д��һ������
	seekto(DBR_RESERVED_SECTORS*DEFAULT_SECTOR_SIZE, "first FAT" );
	for (int nFatCount = 1; nFatCount <= DEFAULT_FAT_NUM; nFatCount++)
	{
		//��fat����һ���������Ĵ�Сд��Ӳ����
		int nPageNum = lFatLength32/6;//������ʾ��ʽ�Ľ���
		int nClusterCount  =0; 
		unsigned long cu = 0;//���ڼ�¼��ǰ��
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
				if(cu < lEnd)//��ǰ��С���ļ��Ĵ���
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
	

	//д��Ŀ¼�ļ�
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

//ִ���������֮ǰ�Ѿ��жϳ��ˣ���ǰ����ʱGPT���̡����ԾͲ����д��̼���ˡ�
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

	//��λ�����������
	llseek( fd, DEFAULT_SECTOR_SIZE*2,SEEK_SET );
	if ( 32*DEFAULT_SECTOR_SIZE != read( fd, pBuf, 32*DEFAULT_SECTOR_SIZE )  )
	{
		delete [] pBuf;
		close(fd);
		return -1;
	}
	
	
	struct __partition_table * pTable = ( struct __partition_table * ) pBuf;

	//ͨ��ֵ���жϷ����ж��ٸ����򵥵㣬�Ϳ����˵ġ�GPT������������128������
	for ( int i = 0; i < 16; i++)
	{
	
		if ( (0 == pTable[i].firstLBA) || ( 0 == pTable[i].lastLBA ) )
		{
			//��������ô����ˣ����Խ�����ѯ�ˡ�
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

	//�ѷ�����Ϣ��ֵ���ڴ���ȥ��
	for ( int i = 0; i < partitionNum; i++ )
	{
		//��λ�� M ,����λ�ķ�ʽʵ��64λ�ĳ�����
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
	unsigned long lTotalSectors;             //��������
	struct hd_geometry geometry;             //Ӳ�̵ļ��β���
	unsigned long lUnits;                    //������λ��������Ϊ��λ
	unsigned long lTotalCylinders;           //�ܹ�������
	unsigned long lUnitsPerParttion;           
	unsigned long start;                     //�ڷ��������м�¼ĳ����������ʼ����
	unsigned long stop;                      //�ڷ��������м�¼ĳ�������Ľ�������
	unsigned char nMainPartNum;
    unsigned char nExtendPartNum;
	unsigned char szEbrBoot[BOOT_LENGTH] = {0};
	loff_t uExtendBase = 0;   //��չ�����Ļ���������ʼ����
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
	
	//��ʼ����
	printf("fdisk %s start...... %d\n", diskName,fd);
	
	//��ȡmbr�е�����
	nLength = read(fd, szMbrBuffer, BOOT_LENGTH);
	
	//�����������Ϣ
	nIndex  = 0;
	for (nIndex  = 0; nIndex < DPT_PARTTION; nIndex++)
	{
		pParttemp = pt_offset(szMbrBuffer, nIndex);		
		Clear_Partition(pParttemp);
	}

		
	//���Ӳ�̵�������
	if (ioctl(fd, BLKGETSIZE, &lTotalSectors))
	{
		close(fd);
		return -1;
	}

	//���Ӳ�̵�������
	if (ioctl(fd, HDIO_GETGEO, &geometry))
	{
		close(fd);
		return -1;
	}
	lTotalCylinders = lTotalSectors/(geometry.heads * geometry.sectors);
   
	//��ȡ������λ,Ҳ����˵��������С������
	lUnits = geometry.heads * geometry.sectors;
	
	//��ȡÿ�������������ٸ�������λ
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

	//д��������
	//��һ��������ʼ������ҪԤ������������
	start = geometry.sectors; 
	for (nIndex = 0; nIndex < nMainPartNum; nIndex++)
	{
		stop = (nIndex + 1) * lUnitsPerParttion * lUnits - 1;
		SetParttion(szMbrBuffer, nIndex, start, stop, nSys, geometry);
		start = stop + 1;
	}

//	//д��չ������
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

	//���ý�����־
	szMbrBuffer[BOOT_LENGTH - 2] = 0x55;
	szMbrBuffer[BOOT_LENGTH - 1] = 0xaa;

	//�ѷ��������Ϣд������������
	lseek(fd, 0, SEEK_SET);
	if (write(fd, szMbrBuffer, BOOT_LENGTH) != BOOT_LENGTH)
	{
		close(fd);
		return -1;
	}

	///Ϊ�˱�֤�����ݴ浽������,ʹ��sync()��ˢ���ں˻�����
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
	//��ȡ������������Ϣ��
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
	//�õ������Ĵ�С
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
	ValidOffset (fd, 0); //���ļ�ָ��ظ�����ʼλ��
	cblocks = (low + 1) / BLOCK_SIZE;
#endif	
	
	///����ļ���״̬��Ϣ
	struct stat statbuf;
	if (fstat(fd, &statbuf) < 0)
		return false;
	if (!S_ISBLK(statbuf.st_mode))//����ļ���һ�����豸����S_ISBLK()������
	{
		statbuf.st_rdev = 0;///st_rdev;  /* device type (if inode device) */
	}
	
	//��ʼ��DBR�еĲ���������������
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
	mbs.dir_entries[1] = (char)0; //fat32�и�Ŀ¼Ҳ�����ļ�������fat16��ʹ�ø��ֶΡ�fat32ҪΪ0
	mbs.media = (char)0xf8; /* Set up the media descriptor for a hard drive */
	mbs.fat_length = CT_LE_W(0);//fat32Ҫ����ֶ�Ϊ0
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
    
	//FAT32��������չBPB�ֶ�  
	time_t CreateTime;
	time(&CreateTime);//�����ھ�꣬
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

	//���������cluster_size fat32_length��ȡֵ
	unsigned long long VolSize = cblocks*SECTORS_PER_BLOCK;//
	printf("or cblocks=%lld\n",cblocks);
	//�ȸ�cluster_size��һ����ֵ
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

	//Ϊcluster_sizeѡ��һ�����ʵ�ֵ
	unsigned long  lTotalSectors = cblocks*BLOCK_SIZE/DEFAULT_SECTOR_SIZE;	
	unsigned long long lFatData = lTotalSectors - DBR_RESERVED_SECTORS;	
	int maxclustsize = 128;	
	unsigned long lClust32; //��¼�������Ĵ���
	unsigned long lMaxClust32;
	unsigned long lFatLength32; //��¼fat32��ռ��������
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

	//���˴���Ϊcluster_sizeѡ����һ�����ʵ�ֵ
	mbs.fat32.fat32_length = CT_LE_L(lFatLength32);
	//��д����sectors  total_sect
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


	//��������дfat32��͸�Ŀ¼�ļ���
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

	//��дfat��ĵ�0��1��
	Mark_FAT_Cluster(0, 0xffffffff,pFat);	/* Initial fat entries */
	pFat[0] = (unsigned char)mbs.media;	/* Put media type in first byte! */
	Mark_FAT_Cluster(1, 0xffffffff,pFat);

	//�ļ���ռ�Ĵ���
	unsigned long lClustersofFile = filesize/(mbs.cluster_size*DEFAULT_SECTOR_SIZE);
	//����FAT��д���ļ���
	unsigned long lFileNum = (lFatLength32*DEFAULT_SECTOR_SIZE/4-2)/lClustersofFile;
	//��Ŀ¼��ռ�Ĵ���
	unsigned long lClustersofRoot = (lFileNum+1)*32/(mbs.cluster_size*DEFAULT_SECTOR_SIZE)+1;


	//������д��Ŀ¼���ڵĴأ�FAT32����Ŀ¼��ͬ����ͨ���ļ�
	for(unsigned long nRootCluCount = 0; nRootCluCount < lClustersofRoot; nRootCluCount++)//lClustersofRootһ��Ϊ1
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


	//��Ŀ¼�ļ��Ĵ�С(�ֽ���)
	unsigned long lRootFileSize = lClustersofRoot*mbs.cluster_size*DEFAULT_SECTOR_SIZE;
	struct msdos_dir_entry* pRootDir = NULL; //ָ���Ŀ¼�ļ�
	if ((pRootDir = (struct msdos_dir_entry *)malloc (lRootFileSize)) == NULL)
	{
		free(pFat);		/* Tidy up before we die! */
		free(pZeroFat);
		printf("\nunable to allocate space for root directory in memory\n");
		return false;
	}
	memset(pRootDir, 0, lRootFileSize);


	//������д��Ŀ¼�ļ�
	
	//��Ŀ¼�ĵ�0�ļ�Ϊ�����
	////��ʼ����0��Ŀ¼��(32���ֽ�)//////////
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
    
	//������Ҫ�м����ض����ļ�
	//unsigned long disksizeG=lTotalSectors*DEFAULT_SECTOR_SIZE/1024/1024/1024;
	LONGLONG diskSizeBytes = ((LONGLONG)lTotalSectors)*DEFAULT_SECTOR_SIZE;
    unsigned long disksizeG = diskSizeBytes/(1024*1024*1024);
	printf("disksizeG = %d\n",disksizeG);

	unsigned long lStartClu = lClustersofRoot + 2;//�������Ӷ���ʼ��غ�,�������ȷŸ�Ŀ¼��Ȼ��ŷ������ļ�����mbr���Ѿ��涨���ˣ�
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

    
   	//��ʵ�ʴصĸ����õ�����Ƶ�ļ���
    unsigned long lActualAviNum = (lClust32 - (lStartClu -2))/lClustersofFile;
		
	//ѭ��д��Ƶ�ļ�
	//Ԥ��3���ļ��������ڽ����ļ��������ļ�
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

    //���˴���Ŀ¼�е�Ŀ¼�������ɣ�ֻ�������ڴ�pRootDir�У���û��д��Ӳ����

	//����д�ļ���Ϣ
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

	//���˴��ļ���Ϣд�ꡣ

	//�����ǰ���������ݣ�д��Ӳ����
    //�ѷ����ı����ռ�����
	seekto( 0, "start of device" );
	BYTE nIndex = 0;
	for( nIndex = 0; nIndex < DBR_RESERVED_SECTORS; nIndex++)//д��32*512�ֽ�Ϊ0
	{
		if (write(fd,pZeroFat,DEFAULT_SECTOR_SIZE) != DEFAULT_SECTOR_SIZE)
		{
			FREE();
			return false;
		}
	}
    	
	//дdbr
	seekto( 0, "boot sector" );//д��512�ֽ�
	if(write(fd,(char *)&mbs, sizeof(struct msdos_boot_sector)) != DEFAULT_SECTOR_SIZE)
	{
		FREE();
		return false;
	}
	
	//д�ļ���Ϣ
	seekto(CF_LE_W(mbs.fat32.info_sector)*DEFAULT_SECTOR_SIZE, "info sector" );//��λ����512���ֽ�
	if(write(fd,(char *)pInfoSector,DEFAULT_SECTOR_SIZE) != DEFAULT_SECTOR_SIZE)
	{
		FREE();
		return false;
	}

	//дbackup boot sector
	seekto(CF_LE_W(mbs.fat32.backup_boot)*DEFAULT_SECTOR_SIZE, "backup boot sector" );
	if(write(fd,(char *)&mbs, sizeof (struct msdos_boot_sector)) != DEFAULT_SECTOR_SIZE)
	{
		FREE();
		return false;
	}

	//дFAT���У���ǰ���Ѿ�д��һ������
	seekto(DBR_RESERVED_SECTORS*DEFAULT_SECTOR_SIZE, "first FAT" );
	for (int nFatCount = 1; nFatCount <= DEFAULT_FAT_NUM; nFatCount++)
	{
		//��fat����һ���������Ĵ�Сд��Ӳ����
		int nPageNum = lFatLength32/6;//������ʾ��ʽ�Ľ���
		int nClusterCount  =0; 
		unsigned long cu = 0;//���ڼ�¼��ǰ��
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
				if(cu < lEnd)//��ǰ��С���ļ��Ĵ���
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
	

	//д��Ŀ¼�ļ�
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
	int nIndex = 0; //��¼������������
	BYTE nPartNum = 0; //��¼�����ĸ���
	int nExtendId = -1; //��¼��������������չ������ID
	int nClearedPartNum = 0; //ȫ�������ĸ���
	int nExtendPartNum = 0; //��չ������ĸ���
	int nLength = 0;
	char szDiskBoot[BOOT_LENGTH] = {0};
    int fd = -1;
    
	fd = open(diskname,O_RDWR);
	if (fd < -1)
	{
		printf("(%s,%d):%s\n", __FILE__, __LINE__, diskname);
		return -1;
	}

	//�����̵�һ����
	nLength = read(fd, szDiskBoot, sizeof(szDiskBoot));
	if (nLength < sizeof(szDiskBoot))
	{
		assert(false);
		close(fd);
		return -1;
	}
	
	//�ж����������ֽ��Ƿ�Ϊ��55AA��
	if (!Valid_Part_Table_Flag((unsigned char *)szDiskBoot)) 
	{
		close(fd);
		return -1;
	}

	//��ȡ���������е���Ϣ����Щ�������ǻ����������Ǹ�����չ����
	/*********************************************************
	*             MBR�����
 	*         446���ֽڵ���������
	*         16���ֽڵķ���һ        
	*         16���ֽڵķ�����     
	*         16���ֽڵķ�����     
	*         16���ֽڵķ�����
	*         �����ֽڵĽ�����־��55AA��
	*********************************************************/
	for (nIndex = 0; nIndex < DPT_PARTTION; nIndex++)
	{	
		pParttemp = pt_offset(szDiskBoot,nIndex);
		if (Is_Cleared_Partition(pParttemp))
	   	{
			nClearedPartNum++;
			continue;
		}
			
		//�������չ�������ͼ�¼���ڷ������е�����
		if (EXTENDED == pParttemp->sys_ind || 0xf == pParttemp->sys_ind)
		{
			nExtendPartNum++;
			nExtendId = nIndex;
			continue;
		}
		
		//���˴���˵���÷�����������ǳ���չ����֮�������
		PART_INFO *p = &pPartitionInfo[nPartNum];	
		ULONG uSects = Get_Nr_Sects(pParttemp);
		uSects /= 1024/DEFAULT_SECTOR_SIZE;
		p->partsize = uSects/1024; //�����Ĵ�С����λM
		p->partformat = pParttemp->sys_ind;//����������
		snprintf(p->devnod, sizeof(p->devnod), "%s%d", diskname, nIndex + 1);//������������: /dev/hda1
		nPartNum++;
	}

	//���������ȫΪ�㣬˵���ô��̻�û�з���
	//�����չ�����ĸ�������һ����˵���ô��̷�������
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

    //û����չ���������
	if (-1 == nExtendId)
	{
		close(fd);
		*pPartitionNum = nPartNum;
		return 0;
	}
	
	//����������չ���������
	unsigned char szEbrBoot[BOOT_LENGTH] = {0};
	pParttemp = pt_offset(szDiskBoot,nExtendId);
	loff_t uExtendBase = Get_Start_Sect(pParttemp);   //��չ�����Ļ���������ʼ����
	loff_t uEbrStart = 0;
	nIndex = DPT_PARTTION + 1; //��չ��������ʼ����Ϊ5
	while (!Is_Cleared_Partition(pParttemp))
	{
		
		//��չmbr����ʼ�ֽ�
		if (uExtendBase == (loff_t)Get_Start_Sect(pParttemp))
		{
			uEbrStart = (loff_t)(Get_Start_Sect(pParttemp)) * DEFAULT_SECTOR_SIZE;
		}
		else
		{
			uEbrStart = ((loff_t)(Get_Start_Sect(pParttemp)) + uExtendBase) * DEFAULT_SECTOR_SIZE;
		}
	
		//��ȡ��չmbr������
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
		p->partsize = uSects/1024; //��λM
		p->partformat = pParttemp->sys_ind; //������ʽ
		snprintf(p->devnod, sizeof(p->devnod), "%s%d", diskname, nIndex++);		
	    nPartNum++;

		//��ȡ��һ��չ����
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
	if (llseek (fd, offset, SEEK_SET) < 0) ///��λ�ļ�ָ��,���ļ�ָ���趨�������whenceֵƫ��offset��λ��
					///�ɹ�ʱ������ָ��λ��
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


	//���豸
	fd = open(szDevice, O_RDONLY);
	if (fd < 0) 
	{
		printf("open %s error\n",szDevice);
		return -1;
	}


	//����cmdp
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

	//����sdp
	memset((void*)&(sb[0]), 0, sizeof(sb));

	//����sg_io_hdr�ṹ
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


	//����ioctl
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


	//���sg_io_hdr�е�״ֵ̬
	if (io_hdr.host_status || io_hdr.driver_status != SG_DRIVER_SENSE
		|| (io_hdr.status && io_hdr.status != SG_CHECK_CONDITION))
	{
		errno = EIO;
		close(fd);
		return -2;
	}

	//���sdp�е�״ֵ̬
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
/*   bAttrID                Ӣ�ĺ���                               ���ĺ���         
0x01			"Raw Read Error Rate     "          Raw ��ȡ������
0x03			"Spin-Up Time            ",			�ۼƼӵ�ʱ��
0x04			"Start/Stop Count        ",			��ʼ/ֹͣ��ʱ
0x05			"Reallocated Sector Count",			�ٷ�����������
0x07			"Seek Error Rate         ",			Ѱ��������
0x09			"Power On Hours Count    ",			�ۼ�ͨ��ʱ��
0x0A			"Spin Retry Count        ", 		������Լ���
0x0C			"Power Cycle Count       ", 		ͨ�����ڼ���
0xBB			"Reported Uncorrectable  ", 		�����޷�У���Ĵ���
0xBD			"High Fly Writes         ", 		�߷�д��
0xBE			"Airflow Temperature     ", 		�����¶�(��������)
0xC2			"Temperature             "			��ǰ�ڲ��¶�
0xC3			"Hardware ECC Recovered  "  		�ѻָ���Ӳ��ECC
0xC5			"Current Pending Sector Count"		��ǰ��ӳ���������� 
0xC6			"Uncorrectable Sector Count"		�޷�У���������� 
0xC7			"UltraDMA CRC Error Count"			ֱ���ڴ����У�������� 
0xC8			"Write Error Rate        "			д������� 
0xCA			"UnKnown                 "          δ֪����
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

	//��ȡsmart��Ϣ
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

	//��ȡsmart��ֵ��Ϣ��
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
	//�����̶�Ӳ��
	DISK_INFO_LIST tmpDiskList;
	//ESATA_INDEX_LIST tmpeSataIndexList;
	//��Ϊ��ʼ����GetDiskCount�죬���Դ�ʱm_eSataIndexList�Ѿ���Esata��Ϣ
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




