/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : zhl
** Date         : 2012-02-10
** Name         : DiskSearchEx.h
** Version      : 1.0
** Description  : 磁盘搜索
**				1 U盘识别 如果文件/sys/block/sdX/removable中的值是1则表示这个设备是U盘
**				2 USB移动硬盘 如果不是U盘的存储设备，获取目录/sys/block/sdb/device的绝对路径，然后在/sys/bus/usb/drivers/usb-storage中找已经插入到DVR的USB设备。
**				  如果路径相同就表明是USB移动硬盘。比如/sys/block/sdb/device的绝对路径是/sys/devices/platform/faradayEHCI.0/usb1/1-1/1-1:1.0/host2/target2:0:0/2:0:0:0
**				  /sys/bus/usb/drivers/usb-storage中有USB设备1-1:1.0， 其绝对路径为/sys/devices/platform/faradayEHCI.0/usb1/1-1/1-1:1.0，可以看到，sdb设备是USB存储设备
**				  这可以认为sdb是USB移动硬盘。
**				3 剩余的则为固定应盘。固定应盘的连接的接口在FH平台和其他平台不同
**				
** Modify Record:
1:
***********************************************************************/
#ifndef __DISK_SEARCH_EX_H_
#define __DISK_SEARCH_EX_H_

#include "Disk_Config.h"

class CDiskSearchEx
{
public:
	CDiskSearchEx();
	~CDiskSearchEx();

	//搜索录像磁盘
	int SearchFixedDisk(DISK_INFO_LIST &disklist, ESATA_INDEX_LIST &esataInterfaceList);
	//搜索热拔插设备
	int SearchRemoveableDisk(DISK_INFO_LIST *pDiskInfoList, ESATA_INDEX_LIST &esataInterfaceList, char *pUsbToEsataPath);

protected:
private:
	int GetStorageDiskInfo(DISK_INFO_LIST &diskList, ESATA_INDEX_LIST &esataInterfaceList);
	//获取设备在那个接口上连接	
	int GetIfIndex(const char * pDevicePath);
	int IsUsbDisk(const char * pSName);
	int IsUsbStorage(const char * pSName);
};
#endif	/*__DISK_SEARCH_EX_H_*/

