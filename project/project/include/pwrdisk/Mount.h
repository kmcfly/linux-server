#ifndef __MOUNT_INFO_H__
#define __MOUNT_INFO_H__

class CMount
{
public:
	static CMount * Instance();
	~CMount();

	bool HasLoadMountInfo();
	bool LoadMountInfo();
	bool GetMountInfo(char * pDev, char * pFileName, const char *pFilePath);
protected:
private:
	CMount();
	static CMount * m_pMount;
	char * m_pMountData;
};
#endif