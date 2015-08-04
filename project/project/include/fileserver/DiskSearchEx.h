/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : zhl
** Date         : 2012-02-10
** Name         : DiskSearchEx.h
** Version      : 1.0
** Description  : ��������
**				1 U��ʶ�� ����ļ�/sys/block/sdX/removable�е�ֵ��1���ʾ����豸��U��
**				2 USB�ƶ�Ӳ�� �������U�̵Ĵ洢�豸����ȡĿ¼/sys/block/sdb/device�ľ���·����Ȼ����/sys/bus/usb/drivers/usb-storage�����Ѿ����뵽DVR��USB�豸��
**				  ���·����ͬ�ͱ�����USB�ƶ�Ӳ�̡�����/sys/block/sdb/device�ľ���·����/sys/devices/platform/faradayEHCI.0/usb1/1-1/1-1:1.0/host2/target2:0:0/2:0:0:0
**				  /sys/bus/usb/drivers/usb-storage����USB�豸1-1:1.0�� �����·��Ϊ/sys/devices/platform/faradayEHCI.0/usb1/1-1/1-1:1.0�����Կ�����sdb�豸��USB�洢�豸
**				  �������Ϊsdb��USB�ƶ�Ӳ�̡�
**				3 ʣ�����Ϊ�̶�Ӧ�̡��̶�Ӧ�̵����ӵĽӿ���FHƽ̨������ƽ̨��ͬ
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

	//����¼�����
	int SearchFixedDisk(DISK_INFO_LIST &disklist, ESATA_INDEX_LIST &esataInterfaceList);
	//�����Ȱβ��豸
	int SearchRemoveableDisk(DISK_INFO_LIST *pDiskInfoList, ESATA_INDEX_LIST &esataInterfaceList, char *pUsbToEsataPath);

protected:
private:
	int GetStorageDiskInfo(DISK_INFO_LIST &diskList, ESATA_INDEX_LIST &esataInterfaceList);
	//��ȡ�豸���Ǹ��ӿ�������	
	int GetIfIndex(const char * pDevicePath);
	int IsUsbDisk(const char * pSName);
	int IsUsbStorage(const char * pSName);
};
#endif	/*__DISK_SEARCH_EX_H_*/

