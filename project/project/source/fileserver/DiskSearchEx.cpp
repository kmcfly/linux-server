#include "DiskSearchEx.h"

CDiskSearchEx::CDiskSearchEx()
{

}

CDiskSearchEx::~CDiskSearchEx()
{

}

int CDiskSearchEx::SearchFixedDisk(DISK_INFO_LIST &disklist, ESATA_INDEX_LIST &esataInterfaceList)
{
	int count = 0;
	BYTE diskID = disklist.GetCount();
	DISK_INFO_LIST diskInfoList;

	GetStorageDiskInfo(diskInfoList, esataInterfaceList);

	POS pos = diskInfoList.GetHeadPosition();
	while (NULL != pos)
	{
		DISK_INFO diskInfo = diskInfoList.GetAt(pos);
		if (NET_DISK_TYPE_FIXEDDISK == diskInfo.diskType)
		{
			diskInfo.diskid = diskID;
			diskID++;
			count++;
			disklist.AddTail(diskInfo);
		}

		diskInfoList.GetNext(pos);
	}
	
	diskInfoList.RemoveAll();

	return count;
}


int CDiskSearchEx::SearchRemoveableDisk(DISK_INFO_LIST *pDiskInfoList, ESATA_INDEX_LIST &esataInterfaceList, char *pUsbToEsataPath)
{
	int count = 0;
	DISK_INFO_LIST diskInfoList;

	GetStorageDiskInfo(diskInfoList, esataInterfaceList);

	POS pos = diskInfoList.GetHeadPosition();
	while (NULL != pos)
	{
		DISK_INFO diskInfo = diskInfoList.GetAt(pos);
		if (NET_DISK_TYPE_FIXEDDISK != diskInfo.diskType)
		{
			count++;
			pDiskInfoList->AddTail(diskInfo);
		}

		diskInfoList.GetNext(pos);
	}
	diskInfoList.RemoveAll();

	return count;
}


/********************************************************************************
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
**********************************************************************************/

int CDiskSearchEx::GetStorageDiskInfo(DISK_INFO_LIST &diskList, ESATA_INDEX_LIST &esataInterfaceList)
{
	//打开存储设备信息的文件
	FILE * pFPartition = fopen(PROC_PARTITIONS, "r");
	if (NULL == pFPartition) 
	{
		printf("%s:%s:%d, cannot open %s\n", __FUNCTION__, __FILE__, __LINE__, PROC_PARTITIONS);
		return 0;
	}

	//统计存储设备的个数，并且把信息保存到pDiskInfo中
	char line[120] = {0};
	char sName[120] = {0};		//存储设备名称
	unsigned long major = 0;	//主设备号
	unsigned long minor = 0;	//次设备号
	unsigned long long size  = 0;	//存储容量

	while (fgets(line, sizeof(line), pFPartition)) 
	{
		if (4 != sscanf (line, " %d %d %llu %[^\n ]",	&major, &minor, &size, sName))
		{
			continue;
		}

		//存储器的是分区名称
		if (isdigit(sName[strlen(sName) - 1]))
		{
			continue;
		}

		if (NULL == strstr(sName,"sd") && NULL == strstr(sName, "hd"))
		{
			//不是存储设备
			continue;
		}
		
		DISK_INFO diskInfo;
		memset(&diskInfo, 0, sizeof(DISK_INFO));
		
		//printf("%s:%s:%d, disk size=%lld, %s\n", __FUNCTION__, __FILE__, __LINE__, size, sName);
		snprintf(diskInfo.diskname, sizeof(diskInfo.diskname), "/dev/%s", sName);
		diskInfo.disksize = (size / 1024);		//M
	
		//查看设备是不是U盘
		int retVal = IsUsbDisk(sName);

		if (-1 == retVal)
		{
			continue;
		}
		else if (0 == retVal)
		{
			
			diskInfo.diskType = NET_DISK_TYPE_REMOVEABLE;
			diskInfo.diskInterfacetype = NET_DISK_USB_INTERFACE;
			diskList.AddTail(diskInfo);
			continue;
		}
		
		retVal = IsUsbStorage(sName);
		if (-1 == retVal)
		{
			continue;
		}
		else if (0 == retVal)
		{
			//设备是移动硬盘
			diskInfo.diskType = NET_DISK_TYPE_REMOVEABLE;
			diskInfo.diskInterfacetype = NET_DISK_REMDISK_INTERFACE;
			diskList.AddTail(diskInfo);
			continue;
		}

		//其他都认为是固定硬盘
		//获取硬盘连接的硬件接口
		int index = GetIfIndex(sName);
		if (0 > index)
		{
			continue;
		}
		else
		{
			bool bEsata = false;
			POS pos = esataInterfaceList.GetHeadPosition();
			while (pos != NULL)
			{
				int nEsataInterfaceTemp = esataInterfaceList.GetNext(pos);
				if (nEsataInterfaceTemp == index)
				{
					bEsata = true;
					break;
				}
			}

			if (bEsata)
			{
				diskInfo.diskType = NET_DISK_TYPE_REMOVEABLE;
				diskInfo.diskInterfacetype = NET_DISK_ESATA_INETERFACE;
				diskInfo.interfaceindex = index;
			}
			else
			{
				diskInfo.diskType = NET_DISK_TYPE_FIXEDDISK;
				diskInfo.diskInterfacetype = NET_DISK_SATA_INTERFACE;
				diskInfo.interfaceindex = index;
			}
		}

		diskList.AddTail(diskInfo);
	}

	fclose(pFPartition);
	pFPartition = NULL;

	return diskList.GetCount();
}


int CDiskSearchEx::GetIfIndex(const char * pSName)
{
	char szDevicelnPath[MAX_PATH] = {0};
	char szDevicePath[MAX_PATH] = {0};

	snprintf(szDevicelnPath, sizeof(szDevicelnPath), "/sys/block/%s/device", pSName);

	//获取设备的绝对路径
	char pwd[120] = {0};
	getcwd(pwd, sizeof(pwd));

	chdir(szDevicelnPath);
	getcwd(szDevicePath, sizeof(szDevicePath));

	chdir(pwd);

	//printf("the device ln path=%s\n", szDevicelnPath);
	//printf("the device path=%s\n", szDevicePath);

	char * pHost = strstr(szDevicePath, "/host");
	if (NULL == pHost)
	{
		printf("%s:%s:%d, szDevicePath=%s\n", __FUNCTION__, __FILE__, __LINE__, szDevicePath);
		return -1;
	}
	else
	{
		int ifIndex = -1;

#if defined(__TDFH__) || defined(__CHIPTI__)
		for (char * p = pHost; p >= szDevicePath; p--)
		{
			if ('.' == * p)
			{
				char szIfIndex[10] = {0};
				memcpy(szIfIndex, p + 1, pHost - p - 1);
				ifIndex = atoi(szIfIndex);
				//printf("%s:%s:%d, The disk %s connect at %d\n", __FUNCTION__, __FILE__, __LINE__, pSName, ifIndex);
				break;
			}
		}
#else
		pHost += 1;
		pHost = strstr(pHost, "target");
		if (NULL == pHost)
		{
			return -1;
		}

		if (6 > strlen(pHost))
		{
			return -1;
		}

		char szIndex[10] = {0};
		memcpy(szIndex, pHost + 6, 1);
		if (!isdigit(szIndex[0]))
		{
			return -1;
		}
		ifIndex = atoi(szIndex);

		if (8 > strlen(pHost))
		{
			return -1;
		}
		
		
		memcpy(szIndex, pHost + 8, 1);
		if (!isdigit(szIndex[0]))
		{
			return -1;
		}

//		ifIndex *= 5;
		ifIndex += atoi(szIndex);

// 		char * pEnd = strstr(pHost, "/");
// 		for (char * p=pHost; p!=pEnd; p++)
// 		{
// 			if (isdigit(*p))
// 			{
// 				char szIndex[10] = {0};
// 				memcpy(szIndex, p, pEnd - p);
// 				ifIndex = atoi(szIndex);
// 				//printf("%s:%s:%d, The disk %s connect at %d\n", __FUNCTION__, __FILE__, __LINE__, pSName, ifIndex);
// 				break;
// 			}
// 		}
#endif
		
		return ifIndex;
	}
}


int CDiskSearchEx::IsUsbDisk(const char * pSName)
{
	char szRemovable[MAX_PATH] = {0};
	snprintf(szRemovable, MAX_PATH, "/sys/block/%s/removable", pSName);
	FILE * f = fopen(szRemovable, "rb");
	if (NULL == f)
	{
		assert(false);
		printf("%s:%s:%d, Open file %s error\n", __FUNCTION__, __FILE__, __LINE__, szRemovable);
		return -1;
	}

	char line[120] = {0};
	fgets(line, sizeof(line), f);
	fclose(f);
	f = NULL;
	int removeable = 0;
	sscanf(line, "%d", &removeable);
				
	if (1 == removeable)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

int CDiskSearchEx::IsUsbStorage(const char * pSName)
{
	char szDevicelnPath[MAX_PATH] = {0};
	char szDevicePath[MAX_PATH] = {0};
	char szUsbInterfacePath[MAX_PATH] = {0};

	snprintf(szDevicelnPath, MAX_PATH, "/sys/block/%s/device", pSName);

	//获取设备的绝对路径
	char pwd[120] = {0};
	getcwd(pwd, sizeof(pwd));
	//printf("%s:%s:%d, pwd=%s\n", __FUNCTION__, __FILE__, __LINE__, pwd);

	chdir(szDevicelnPath);
	getcwd(szDevicePath, MAX_PATH);

	//printf("the device ln path=%s\n", szDevicelnPath);
	//printf("the device path=%s\n", szDevicePath);

	//
	DIR * pUsbStorageDir = opendir("/sys/bus/usb/drivers/usb-storage/");			
	//没有这个目录，说明不存在这个设备
	if(NULL == pUsbStorageDir)
	{
		printf("%s:%s:%d, Open dir /sys/bus/usb/drivers/usb-storage/ error\n", __FUNCTION__, __FILE__, __LINE__);
		chdir(pwd);
		return -1;
	}
	struct dirent * pDirentUsbSDir = NULL;	
	while((pDirentUsbSDir = readdir(pUsbStorageDir)) != NULL)
	{
		char checkName[120] = {0};
		snprintf(checkName, sizeof(checkName), "/%s", pDirentUsbSDir->d_name);

		if(NULL != strstr(szDevicePath, checkName))
		{
			//表明在usb-storage中的某个目录文件和/sys/block/%s/device路径中的一个目录名字相同
			//printf("%s:%s:%d, Get the dir %s\n", __FUNCTION__, __FILE__, __LINE__, pDirentUsbSDir->d_name);
			char path[120] = {0};
			snprintf(path, sizeof(path), "/sys/bus/usb/drivers/usb-storage/%s/", pDirentUsbSDir->d_name);
			chdir(path);
			getcwd(szUsbInterfacePath, sizeof(szUsbInterfacePath));
			//printf("%s:%s:%d, The usb interface path=%s\n", __FUNCTION__, __FILE__, __LINE__, szUsbInterfacePath);
			break;
		}
	}
	closedir(pUsbStorageDir);
	chdir(pwd);

	if (0 < strlen(szUsbInterfacePath) && 0 == strncmp(szDevicePath, szUsbInterfacePath, strlen(szUsbInterfacePath)))
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

