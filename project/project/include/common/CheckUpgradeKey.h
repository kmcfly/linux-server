#ifndef __CHECKUPGRADEKEY_H__
#define __CHECKUPGRADEKEY_H__

#include "CheckUpgradeKeyList.h"

class UpgradeDevice
{
public:
	UpgradeDevice();
	~UpgradeDevice();

	static UpgradeDevice *Instance();
	char *Upgrade(unsigned long venderId, unsigned long productId, char *hardwareVersion);

private:
	FILE *m_pKeyFd;
	char *m_pKey;
	int m_listSize;
	DISK_INFO_LIST *pRemoveableDiskList;
	CDiskSearchEx *pDisk;
	UPGRADE_INFORMATION *m_pUpgradeInfo;
};

#endif
