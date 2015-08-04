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

	//ËÑË÷Â¼Ïñ´ÅÅÌ
	int SearchFixedDisk(DISK_INFO_LIST &disklist, ESATA_INDEX_LIST &esataInterfaceList);

	//ËÑË÷ÈÈ°Î²åÉè±¸
	int SearchRemoveableDisk(DISK_INFO_LIST *pDiskInfoList, ESATA_INDEX_LIST &esataInterfaceList, char *pUsbToEsataPath);
	
private:
	//ËÑË÷ÒÆ¶¯Ó²ÅÌºÍesataÓ²ÅÌ	
	int SearchREMDisk(DISK_INFO_LIST *pDiskInfoList, ESATA_INDEX_LIST &esataInterfaceList, char *pUsbToEsataPath);

	//ËÑË÷UÅÌ
	int SearchUSBDisk(DISK_INFO_LIST *pDiskInfoList);
	
};
#endif



