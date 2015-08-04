/***********************************************************************
** Copyright   Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : 
** Date         : 2007-09-14
** Name         : searchdisk.cpp
** Version      : 1.0
** Description  : 
** Modify Record:
1:2009-4-27 �޸ĵ�ԭ��
  
  ʶ��̶�Ӳ�̵ķ���������ǹ̶�Ӳ�̣���ô�ں��е��ļ�/sys/block/sd��a��device/vendor������Ϊ:ATA ����֮������
                      ATA��

  ʶ��U�̷����� �����U�̣���ô�ں��е��ļ�/sys/block/sd��a��/removable�м�¼��Ϊ1.

  

***********************************************************************/
#include "SearchDisk.h"


//#define USB_DEVICE_PATH  "/sys/devices/fotg2xx_dev/usb1/1-1/1-1:1.0/host%d/target%d:0:0/%d:0:0:0"
//#define DISK_A_DEVICE_PATH "/sys/devices/pci0000:00/0000:00:0a.0/host%d/target%d:0:0/%d:0:0:0"
//#define DISK_B_DEVICE_PATH "/sys/devices/pci0000:00/0000:00:0b.0/host%d/target%d:0:0/%d:0:0:0"

/**********************************************************************
��������  BYTE CSearchDisk::SearchFixedDisk(DISKINFO* pDiskInfo, 
                BYTE nDiskInfoMaxNum)
�����������������д���Ӳ��,ֻ���һЩ������Ϣ.���ŵ�pDiskInfoָ��Ŀռ��У�
          ��ౣ��nDiskInfoMaxNum��
���������
���������
����ֵ��      
˵����   
������/�޸���
����/�޸�����
��ע                           
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
	
	//�򿪴洢�豸��Ϣ���ļ�
	procpt = fopen(PROC_PARTITIONS, "r");
	if (NULL == procpt) 
	{
		printf("cannot open %s\n", PROC_PARTITIONS);
		return -1;
	}
	
	//ͳ��Ӳ�̵ĸ��������Ұ���Ϣ���浽disklist��
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
		
		//���˴�����ʾline�е���Ϣ��һ����Ӳ�̵���Ϣ
		//Ӳ�̵�����
		DISK_INFO p;
		memset(&p, 0, sizeof(p));
		p.diskid = nId++;
		if (strstr(ptname,"hd"))
		{
			//˵���豸��IDEӲ��
			p.diskInterfacetype = NET_DISK_IDE_INTERFACE;
			p.interfaceindex = 1;
		}
		else
		{
			//˵���豸��sataӲ�̻�usb
			//��������������ߵĴ���
			char szDevicePath[MAX_PATH] = {0};
			char szDeviceType[MAX_PATH] = {0};
			int  nDeviceType = 0;
			
			//ͨ����ȡ removable �Ѿ������жϳ��Ƿ���Ӳ�̻����ƶ�Ӳ�̡���Ϊ�ƶ�Ӳ�̵�removable
			//Ҳ��¼Ϊ0
			//ע���̶�Ӳ�̵�removable�м�¼Ϊ0
			//    �ƶ�Ӳ�̵�removable�м�¼Ϊ0
			//         U�̵�removable�м�¼Ϊ1
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

			//����Ĵ��������ж��Ƿ�̶�Ӳ��
			//������
			//����ǹ̶�Ӳ�̣���ô�ļ�/sys/block/sd*/device/vendor������Ϊ:ATA 
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


			//����Ĵ���������ǣ��õ��豸sd*���������Ǹ�sata�ڡ�
			//Ȼ���õõ��Ľ���жϳ����豸sd*�Ƿ����esata�ӿ��ϡ�
			memset(szDevicePath, 0, MAX_PATH);
			char szInterfaceIndex[MAX_PATH] = {0};
			snprintf(szDevicePath, MAX_PATH, "/sys/block/%s/device", ptname);
			DIR* dir = opendir(szDevicePath);			
			//û�����Ŀ¼��˵������������豸
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
				//˵������������豸
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
					//˵����EsataӲ��
					continue;
				}
			}

#endif			
			
			//˵���豸��sataӲ��
			p.diskType = NET_DISK_TYPE_FIXEDDISK;
			p.diskInterfacetype = NET_DISK_SATA_INTERFACE;
#if defined(__TDFH__) || defined(__TDNXP__) || defined(__CHIP3531__) || defined(__CHIP3535__) || defined(__CHIP3521__) || defined(__CHIP3520A__) || defined(__CHIPTI__) || defined(__CHIP3520D__) || defined(__CHIPGM__) 
			p.interfaceindex = 0;
#else
			p.interfaceindex = nInterfaceIndex;
#endif
		}
		snprintf(p.diskname, sizeof(p.diskname), "/dev/%s", ptname);		
		p.disksize = ( ULONG )(size/1024); //��¼��С��λM
		
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
    

	//�򿪴洢�豸��Ϣ���ļ�
	procpt = fopen(PROC_PARTITIONS, "r");
	if (NULL == procpt) 
	{
		printf("cannot open %s\n", PROC_PARTITIONS);
		return 0;
	}

	/*******************
	* /proc/partitions�ļ���������
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
	//ͳ��Ӳ�̵ĸ��������Ұ���Ϣ���浽pDiskInfo��
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

		//����Ĵ���������ǣ��õ��豸sd*���������Ǹ�sata�ڡ�		
		memset(szDevicePath, 0, MAX_PATH);
		char szInterfaceIndex[MAX_PATH] = {0};
		snprintf(szDevicePath, MAX_PATH, "/sys/block/%s/device", ptname);
		DIR* dir = opendir(szDevicePath);			
		//û�����Ŀ¼��˵������������豸
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
			//˵������������豸
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
			//���˴���˵����pciת��esata��̶�Ӳ��
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
					//˵������EsataӲ��
					continue;
				}
			}

			if (NULL != pDiskInfoList)
			{
				memset(&diskInfo, 0 ,sizeof(diskInfo));
				sprintf(diskInfo.diskname, "/dev/%s", ptname);
				diskInfo.diskType = NET_DISK_TYPE_REMOVEABLE;
				diskInfo.diskInterfacetype = NET_DISK_ESATA_INETERFACE;
				diskInfo.disksize = size/1024; //��¼��С��λM
				pDiskInfoList->AddTail(diskInfo);
			}
			nIndex++;
		}
		else
		{
			//���˴���˵����usbת��esata��USB�ƶ�Ӳ��
			//��������������
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
					diskInfo.disksize = size/1024; //��¼��С��λM
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
					diskInfo.disksize = size/1024; //��¼��С��λM
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


	//�򿪴洢�豸��Ϣ���ļ�
	procpt = fopen(PROC_PARTITIONS, "r");
	if (NULL == procpt) 
	{
		printf("cannot open %s\n", PROC_PARTITIONS);
		return 0;
	}

	/*******************
	* /proc/partitions�ļ���������
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
	//ͳ��Ӳ�̵ĸ��������Ұ���Ϣ���浽pDiskInfo��
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


		//˵���豸��sataӲ�̻�usb
		//��������������ߵĴ���
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
			diskInfo.disksize = size/1024; //��¼��С��λM
			pDiskInfoList->AddTail(diskInfo);
		}
		nIndex++;
	}

	fclose(procpt);
	return nIndex;

}


