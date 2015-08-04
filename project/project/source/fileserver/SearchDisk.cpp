/***********************************************************************
** Copyright   Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : 
** Date         : 2007-09-14
** Name         : searchdisk.cpp
** Version      : 1.0
** Description  : 
** Modify Record:
1:2009-4-27 修改的原因：
  
  识别固定硬盘的方法：如果是固定硬盘，那么内核中的文件/sys/block/sd（a）device/vendor的内容为:ATA ；反之，则不是
                      ATA。

  识别U盘方法： 如果是U盘，那么内核中的文件/sys/block/sd（a）/removable中记录必为1.

  

***********************************************************************/
#include "SearchDisk.h"


//#define USB_DEVICE_PATH  "/sys/devices/fotg2xx_dev/usb1/1-1/1-1:1.0/host%d/target%d:0:0/%d:0:0:0"
//#define DISK_A_DEVICE_PATH "/sys/devices/pci0000:00/0000:00:0a.0/host%d/target%d:0:0/%d:0:0:0"
//#define DISK_B_DEVICE_PATH "/sys/devices/pci0000:00/0000:00:0b.0/host%d/target%d:0:0/%d:0:0:0"

/**********************************************************************
函数名：  BYTE CSearchDisk::SearchFixedDisk(DISKINFO* pDiskInfo, 
                BYTE nDiskInfoMaxNum)
功能描述：搜索所有存在硬盘,只获得一些基本信息.并放到pDiskInfo指向的空间中，
          最多保存nDiskInfoMaxNum个
输入参数：
输出参数：
返回值：      
说明：   
创建者/修改者
创建/修改日期
备注                           
***********************************************************************/
int CSearchDisk::SearchFixedDisk(DISK_INFO_LIST &disklist, ESATA_INDEX_LIST &esataInterfaceList)
{
	FILE* procpt = NULL;
	char line[MAX_PATH] = {0};
	char ptname[NAME_LENGTH] = {0};
	char devname[MAX_PATH] = {0};
	char *s = NULL;
	int ma = 0, mi = 0;
	ULONGLONG size = 0;
	BYTE nId = disklist.GetCount();
	
	//打开存储设备信息的文件
	procpt = fopen(PROC_PARTITIONS, "r");
	if (NULL == procpt) 
	{
		printf("cannot open %s\n", PROC_PARTITIONS);
		return -1;
	}
	
	//统计硬盘的个数，并且把信息保存到disklist中
	while (fgets(line, sizeof(line), procpt)) 
	{
		if (sscanf (line, " %d %d %d %[^\n ]",	&ma, &mi, &size, ptname) != 4)
			continue;
		
		for (s = ptname; *s; s++);
		if (isdigit(s[-1]))
			continue;
		
		if ((NULL == strstr(ptname,"sd")) &&(NULL == strstr(ptname,"hd")))
		{
			continue;
		}
		
		//到此处，表示line中的信息是一个整硬盘的信息
		//硬盘的名称
		DISK_INFO p;
		memset(&p, 0, sizeof(p));
		p.diskid = nId++;
		if (strstr(ptname,"hd"))
		{
			//说明设备是IDE硬盘
			p.diskInterfacetype = NET_DISK_IDE_INTERFACE;
			p.interfaceindex = 1;
		}
		else
		{
			//说明设备是sata硬盘或usb
			//下面的是区分两者的代码
			char szDevicePath[MAX_PATH] = {0};
			char szDeviceType[MAX_PATH] = {0};
			int  nDeviceType = 0;
			
			//通过读取 removable 已经不能判断出是否是硬盘还是移动硬盘。因为移动硬盘的removable
			//也记录为0
			//注：固定硬盘的removable中记录为0
			//    移动硬盘的removable中记录为0
			//         U盘的removable中记录为1
            snprintf(szDevicePath, MAX_PATH, "/sys/block/%s/removable", ptname);
			FILE *f = fopen(szDevicePath, "rb");
			if (NULL == f)
			{
				continue;
			}			
            fgets(szDeviceType, MAX_PATH, f);
			sscanf(szDeviceType, "%d", &nDeviceType);
			fclose(f);
			f = NULL;			
			if (0 != nDeviceType)
			{
				continue;
			}
			
#if !defined(__TDFH__) && !defined(__TDNXP__) && !defined(__CHIP3531__) && !defined(__CHIP3535__) && !defined(__CHIP3521__) && !defined(__CHIP3520A__) && !defined(__CHIPTI__) && !defined(__CHIP3520D__) && !defined(__CHIPGM__) 

			//下面的代码用于判断是否固定硬盘
			//方法：
			//如果是固定硬盘，那么文件/sys/block/sd*/device/vendor的内容为:ATA 
			memset(szDevicePath, 0, MAX_PATH);
			snprintf(szDevicePath, MAX_PATH, "/sys/block/%s/device/vendor", ptname);
			f = fopen(szDevicePath, "r");
			if (NULL == f)
			{
				continue;
			}
			fgets(szDeviceType, MAX_PATH, f);
			fclose(f);
			f = NULL;
		//	printf("sata:%s\n",szDeviceType);
			if (strncasecmp("ATA", szDeviceType, 3) != 0)
			{
				continue;
			}


			//下面的代码的作用是：得到设备sd*，连接在那个sata口。
			//然后用得到的结果判断出：设备sd*是否接在esata接口上。
			memset(szDevicePath, 0, MAX_PATH);
			char szInterfaceIndex[MAX_PATH] = {0};
			snprintf(szDevicePath, MAX_PATH, "/sys/block/%s/device", ptname);
			DIR* dir = opendir(szDevicePath);			
			//没有这个目录，说明不存在这个设备
			if(NULL == dir)
			{
				continue;
			}			
			struct dirent* ptr = NULL;		
			while((ptr = readdir(dir)) != NULL)
			{
				if(strstr(ptr->d_name,"scsi_device:"))
				{
					strcpy(szInterfaceIndex,ptr->d_name);
					break;
				}
			}
			closedir(dir);
			if (0 == strlen(szInterfaceIndex))
			{
				//说明不存在这个设备
				continue;
			}

			int nInterfaceIndex = atoi(&szInterfaceIndex[12]);	
			if (esataInterfaceList.GetCount() > 0)
			{
				bool bEsata = false;
				POS pos = esataInterfaceList.GetHeadPosition();
				while (pos != NULL)
				{
					int nEsataInterfaceTemp = esataInterfaceList.GetNext(pos);
					if (nEsataInterfaceTemp == nInterfaceIndex)
					{						
						bEsata = true;
						break;						
					}
				}

				if (bEsata)
				{
					//说明是Esata硬盘
					continue;
				}
			}

#endif			
			
			//说明设备是sata硬盘
			p.diskType = NET_DISK_TYPE_FIXEDDISK;
			p.diskInterfacetype = NET_DISK_SATA_INTERFACE;
#if defined(__TDFH__) || defined(__TDNXP__) || defined(__CHIP3531__) || defined(__CHIP3535__) || defined(__CHIP3521__) || defined(__CHIP3520A__) || defined(__CHIPTI__) || defined(__CHIP3520D__) || defined(__CHIPGM__) 
			p.interfaceindex = 0;
#else
			p.interfaceindex = nInterfaceIndex;
#endif
		}
		snprintf(p.diskname, sizeof(p.diskname), "/dev/%s", ptname);		
		p.disksize = ( ULONG )(size/1024); //记录大小单位M
		
		disklist.AddTail(p);
	}
	fclose(procpt);
	return 0;
}


int CSearchDisk::SearchRemoveableDisk(DISK_INFO_LIST *pDiskInfoList, ESATA_INDEX_LIST &esataInterfaceList, char *pUsbToEsataPath)
{
	int ret = 0;
	ret = SearchREMDisk(pDiskInfoList, esataInterfaceList, pUsbToEsataPath);
	ret += SearchUSBDisk(pDiskInfoList);	
	return ret;
}

int CSearchDisk::SearchREMDisk(DISK_INFO_LIST *pDiskInfoList, ESATA_INDEX_LIST &esataInterfaceList, char *pUsbToEsataPath)
{
	FILE* procpt = NULL;
	char line[MAX_PATH] = {0};
	char ptname[NAME_LENGTH] = {0};
	char devname[MAX_PATH] = {0};
	char *s = NULL;
	int ma = 0, mi = 0;
	int size = 0;
	int nIndex = 0;
	DISK_INFO diskInfo;
    

	//打开存储设备信息的文件
	procpt = fopen(PROC_PARTITIONS, "r");
	if (NULL == procpt) 
	{
		printf("cannot open %s\n", PROC_PARTITIONS);
		return 0;
	}

	/*******************
	* /proc/partitions文件内容样例
	major minor  #blocks  name
	8     0  156290904 sda
	8     1   39070048 sda1
	8     2   39070080 sda2
	8     3   39070080 sda3
	8     4   39078112 sda4
	8    16  156290904 sdb
	8    17   39070048 sdb1
	8    18   39070080 sdb2
	8    19   39070080 sdb3
	8    20   39078112 sdb4
	8    32     257280 sdc
	8    33     257248 sdc1
	********************/
	//统计硬盘的个数，并且把信息保存到pDiskInfo中
	while (fgets(line, sizeof(line), procpt)) 
	{
		if (sscanf (line, " %d %d %d %[^\n ]",	&ma, &mi, &size, ptname) != 4)
			continue;

		for (s = ptname; *s; s++);
		if (isdigit(s[-1]))
			continue;

		if (NULL == strstr(ptname,"sd"))
		{
			continue;
		}


		char szDevicePath[MAX_PATH] = {0};
		char szDeviceType[MAX_PATH] = {0};
		int  nDeviceType = 0;

		snprintf(szDevicePath, MAX_PATH, "/sys/block/%s/removable", ptname);
		FILE *f = fopen(szDevicePath, "rb");
		if (NULL == f)
		{
			continue;
		}			
		fgets(szDeviceType, MAX_PATH, f);
		sscanf(szDeviceType, "%d", &nDeviceType);
		fclose(f);
		f = NULL;			
		if (0 != nDeviceType)
		{
			continue;
		}

		//下面的代码的作用是：得到设备sd*，连接在那个sata口。		
		memset(szDevicePath, 0, MAX_PATH);
		char szInterfaceIndex[MAX_PATH] = {0};
		snprintf(szDevicePath, MAX_PATH, "/sys/block/%s/device", ptname);
		DIR* dir = opendir(szDevicePath);			
		//没有这个目录，说明不存在这个设备
		if(NULL == dir)
		{
			continue;
		}			
		struct dirent* ptr = NULL;		
		while((ptr = readdir(dir)) != NULL)
		{
			if(strstr(ptr->d_name,"scsi_device:"))
			{
				strcpy(szInterfaceIndex,ptr->d_name);
				break;
			}
		}
		closedir(dir);
		if (0 == strlen(szInterfaceIndex))
		{
			//说明不存在这个设备
			continue;
		}
		int nInterfaceIndex = atoi(&szInterfaceIndex[12]);	


		memset(szDevicePath, 0, MAX_PATH);
		snprintf(szDevicePath, MAX_PATH, "/sys/block/%s/device/vendor", ptname);
		f = fopen(szDevicePath, "r");
		if (NULL == f)
		{
			continue;
		}
		fgets(szDeviceType, MAX_PATH, f);
		fclose(f);
		f = NULL;
		if (0 == strncasecmp("ATA", szDeviceType, 3))
		{
			//到此处，说明是pci转的esata或固定硬盘
			if (esataInterfaceList.GetCount() <= 0)
			{
				continue;
			}			

			{
				bool bEsata = false;
				POS pos = esataInterfaceList.GetHeadPosition();
				while (pos != NULL)
				{
					int nEsataInterfaceTemp = esataInterfaceList.GetNext(pos);
					if (nEsataInterfaceTemp == nInterfaceIndex)
					{
						bEsata = true;
						break;
					}
				}

				if (!bEsata)
				{
					//说明不是Esata硬盘
					continue;
				}
			}

			if (NULL != pDiskInfoList)
			{
				memset(&diskInfo, 0 ,sizeof(diskInfo));
				sprintf(diskInfo.diskname, "/dev/%s", ptname);
				diskInfo.diskType = NET_DISK_TYPE_REMOVEABLE;
				diskInfo.diskInterfacetype = NET_DISK_ESATA_INETERFACE;
				diskInfo.disksize = size/1024; //记录大小单位M
				pDiskInfoList->AddTail(diskInfo);
			}
			nIndex++;
		}
		else
		{
			//到此处，说明是usb转的esata或USB移动硬盘
			//下面是区分两者
			bool bUsbToEsata = false;
			if (strlen(pUsbToEsataPath) > 0)
			{
				DIR* dir = opendir(pUsbToEsataPath);				
				if(NULL != dir)
				{
					bUsbToEsata = true;
					closedir(dir);
				}				
			}

			
			if (bUsbToEsata)
			{
				if (NULL != pDiskInfoList)
				{
					memset(&diskInfo, 0 ,sizeof(diskInfo));
					sprintf(diskInfo.diskname, "/dev/%s", ptname);
					diskInfo.diskType = NET_DISK_TYPE_REMOVEABLE;
					diskInfo.diskInterfacetype = NET_DISK_ESATA_INETERFACE;
					diskInfo.disksize = size/1024; //记录大小单位M
					pDiskInfoList->AddTail(diskInfo);
				}
				nIndex++;
			}
			else
			{
				if (NULL != pDiskInfoList)
				{
					memset(&diskInfo, 0 ,sizeof(diskInfo));
					sprintf(diskInfo.diskname, "/dev/%s", ptname);
					diskInfo.diskType = NET_DISK_TYPE_REMOVEABLE;
					diskInfo.diskInterfacetype = NET_DISK_REMDISK_INTERFACE;
					diskInfo.disksize = size/1024; //记录大小单位M
					pDiskInfoList->AddTail(diskInfo);
				}
				nIndex++;
			}
		}	
		
	}

	fclose(procpt);
	return nIndex;

}

int CSearchDisk::SearchUSBDisk(DISK_INFO_LIST *pDiskInfoList)
{

	FILE* procpt = NULL;
	char line[MAX_PATH] = {0};
	char ptname[NAME_LENGTH] = {0};
	char devname[MAX_PATH] = {0};
	char *s = NULL;
	int ma = 0, mi = 0;
	int size = 0;
	int nIndex = 0;
	DISK_INFO diskInfo;


	//打开存储设备信息的文件
	procpt = fopen(PROC_PARTITIONS, "r");
	if (NULL == procpt) 
	{
		printf("cannot open %s\n", PROC_PARTITIONS);
		return 0;
	}

	/*******************
	* /proc/partitions文件内容样例
	major minor  #blocks  name
	8     0  156290904 sda
	8     1   39070048 sda1
	8     2   39070080 sda2
	8     3   39070080 sda3
	8     4   39078112 sda4
	8    16  156290904 sdb
	8    17   39070048 sdb1
	8    18   39070080 sdb2
	8    19   39070080 sdb3
	8    20   39078112 sdb4
	8    32     257280 sdc
	8    33     257248 sdc1
	********************/
	//统计硬盘的个数，并且把信息保存到pDiskInfo中
	while (fgets(line, sizeof(line), procpt)) 
	{
		if (sscanf (line, " %d %d %d %[^\n ]",	&ma, &mi, &size, ptname) != 4)
			continue;

		for (s = ptname; *s; s++);
		if (isdigit(s[-1]))
			continue;

		if (NULL == strstr(ptname,"sd"))
		{
			continue;
		}


		//说明设备是sata硬盘或usb
		//下面的是区分两者的代码
		char szDevicePath[MAX_PATH] = {0};
		char szDeviceType[MAX_PATH] = {0};
		int  nDeviceType = 0;


		memset(szDevicePath, 0, MAX_PATH);
		snprintf(szDevicePath, MAX_PATH, "/sys/block/%s/removable", ptname);
		FILE *f = fopen(szDevicePath, "rb");
		if (NULL == f)
		{
			continue;
		}			
		fgets(szDeviceType, MAX_PATH, f);
		sscanf(szDeviceType, "%d", &nDeviceType);
		fclose(f);
		f = NULL;			
		if (1 != nDeviceType)
		{
			continue;
		}


		if (NULL != pDiskInfoList)
		{
			memset(&diskInfo, 0 ,sizeof(diskInfo));
			sprintf(diskInfo.diskname, "/dev/%s", ptname);
			diskInfo.diskType = NET_DISK_TYPE_REMOVEABLE;
			diskInfo.diskInterfacetype = NET_DISK_USB_INTERFACE;
			diskInfo.disksize = size/1024; //记录大小单位M
			pDiskInfoList->AddTail(diskInfo);
		}
		nIndex++;
	}

	fclose(procpt);
	return nIndex;

}


