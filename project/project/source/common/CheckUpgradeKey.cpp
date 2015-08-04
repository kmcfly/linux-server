#include <stdio.h>
#include <iostream>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#include "PUB_common.h"
#include "DiskSearchEx.h"
#include "Disk_Config.h"
#include "CheckUpgradeKey.h"

UpgradeDevice::UpgradeDevice()
{
	m_pKey = new char[32+1];
	memset(m_pKey, 0, 32+1);
	m_listSize = sizeof(KeyList) / sizeof(UPGRADE_INFORMATION);
	pRemoveableDiskList = new DISK_INFO_LIST;
	pDisk = new CDiskSearchEx;
}

UpgradeDevice::~UpgradeDevice()
{
	delete [] m_pKey;
	m_pKey = NULL;
	delete pDisk;
	pDisk = NULL;
	delete pRemoveableDiskList;
	pRemoveableDiskList = NULL;
}

UpgradeDevice *UpgradeDevice::Instance()
{
	static UpgradeDevice s_upgradeDevice;
	return &s_upgradeDevice;
}

char *UpgradeDevice::Upgrade(unsigned long venderId, unsigned long productId, char *pHardwareVersion)
{
	if (NULL == pHardwareVersion)
	{
		fprintf(stderr, "%s %s %d\n", strerror(errno), __FILE__, __LINE__);
		return NULL;
	}

	/* create disk list(pRemoveableDiskList) */
	ESATA_INDEX_LIST esataInterfaceList;
	int remCount = pDisk->SearchRemoveableDisk(pRemoveableDiskList, esataInterfaceList, NULL);
	if (0 == remCount)
	{
    		printf("########### No removable disk found. %s %d\n", __FILE__, __LINE__);
        	return NULL;
	}
	
	/* mkdir /mnt/upgradedisk */
	char cmd[512] = {0};
	sprintf(cmd, "mkdir -p %s", "/mnt/UpgradeDisk");
	if (0 != DVRSystem(cmd))
	{
	    	printf("########### Unable to make directory: \"/mnt/UpgradeDisk\". %s %d\n", __FILE__, __LINE__);
        	return NULL;
	}

	/* mount removable disk on /mnt/upgradedisk and read key file */
	char pDiskName[NAME_LENGTH];
	int nodeCount = pRemoveableDiskList->GetCount();
	POS pos = pRemoveableDiskList->GetHeadPosition();
	while (NULL != pos)
	{
		int partition = 1;
		DISK_INFO &node = pRemoveableDiskList->GetAt(pos);
		while (5 >= partition)    //less than 5 partitions
		{
			sprintf(pDiskName, "%s%d", node.diskname, partition);
			sprintf(cmd, "mount -t vfat %s %s", pDiskName, "/mnt/UpgradeDisk");
			DVRSystem(cmd);
			if ((m_pKeyFd = fopen("/mnt/UpgradeDisk/key.txt", "r")) != NULL) 
			{
				printf("%s\n", cmd);
				goto KeyFileFound;
			}
			partition++;
			sprintf(cmd, "umount %s", "/mnt/UpgradeDisk");
                        DVRSystem(cmd);
		}
		pRemoveableDiskList->GetNext(pos);
	}
	printf("########### key is not found. %s %d\n", __FILE__, __LINE__);
	return NULL;

KeyFileFound:
	for (int index = 0; index < 32; index++)
	{
		fseek(m_pKeyFd, 32, SEEK_CUR);
		*(m_pKey + index) = getc(m_pKeyFd);
	}
	*(m_pKey+32) = '\0';
	fclose(m_pKeyFd);

	/* umount and remove /mnt/upgradedisk */
	sprintf(cmd, "umount %s;rm -rf %s", "/mnt/UpgradeDisk", "/mnt/UpgradeDisk");
	DVRSystem(cmd);
	printf("%s\n", cmd);

	/* check key */
	bool checkStatus = false;
	for (int index = 0; index < m_listSize; index++)
	{
		m_pUpgradeInfo = (UPGRADE_INFORMATION *)&KeyList[index];
		if (strcmp(m_pUpgradeInfo->key, m_pKey) == 0)
		{
			checkStatus = true;
			break;
		}
	}
	if (!checkStatus)
	{
		printf("########### Key is not matched. %s %d\n", __FILE__, __LINE__);
		return NULL;
	}

	/* check vender id and product id */
	if (m_pUpgradeInfo->venderId != venderId)
	{
		printf("########### Vender id is not matched. %s %d\n", __FILE__, __LINE__);
		return NULL;
	}
	if (m_pUpgradeInfo->productId != productId)
	{
		printf("########### Product id is not matched. %s %d\n", __FILE__, __LINE__);
		return NULL;
	}

	/* modify hardware version */
	switch (m_pUpgradeInfo->channels)
	{
	case 8:
		pHardwareVersion[5] = 'B'; 
		break;
	case 16:
		pHardwareVersion[5] = 'C'; 
		break;
	case 24:
		pHardwareVersion[5] = 'D';
		break;
	case 32:
		pHardwareVersion[5] = 'E'; 
		break;
	case 64:
		pHardwareVersion[5] = 'F'; 
		break;
	default:
		return NULL;
		break;
	}

	/* return new hardware version */
	return pHardwareVersion;          
}
