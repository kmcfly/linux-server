/***********************************************************************
** Copyright   Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : 
** Date         : 2007-09-14
** Name         : SearchDisk.h
** Version      : 1.0
** Description  :
** Modify Record:
1:
***********************************************************************/
#ifndef _SEARCHDISK_H_
#define _SEARCHDISK_H_

#include "Disk_Config.h"

class CSearchDisk
{
public:

	//����¼�����
	int SearchFixedDisk(DISK_INFO_LIST &disklist, ESATA_INDEX_LIST &esataInterfaceList);

	//�����Ȱβ��豸
	int SearchRemoveableDisk(DISK_INFO_LIST *pDiskInfoList, ESATA_INDEX_LIST &esataInterfaceList, char *pUsbToEsataPath);
	
private:
	//�����ƶ�Ӳ�̺�esataӲ��	
	int SearchREMDisk(DISK_INFO_LIST *pDiskInfoList, ESATA_INDEX_LIST &esataInterfaceList, char *pUsbToEsataPath);

	//����U��
	int SearchUSBDisk(DISK_INFO_LIST *pDiskInfoList);
	
};
#endif



