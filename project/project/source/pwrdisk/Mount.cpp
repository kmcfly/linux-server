#include "Mount.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include "PUB_common.h"

const char MOUNT_INFO_PATH[] = "/proc/mounts";

CMount * CMount::m_pMount = NULL;

CMount::CMount()
{
	m_pMountData = NULL;
}

CMount::~CMount()
{
	if (NULL != m_pMountData)
	{
		delete [] m_pMountData;
		m_pMountData = NULL;
	}
}

CMount * CMount::Instance()
{
	if (NULL == m_pMount)
	{
		m_pMount = new CMount();
		m_pMount->LoadMountInfo();
	}

	return m_pMount;
}

bool CMount::HasLoadMountInfo()
{
	if (NULL == m_pMountData)
	{
		return false;
	}
	else
	{
		return true;
	}
};

bool CMount::LoadMountInfo()
{
	DVRSystem("cat /proc/mounts > /tmp/mounts");
	FILE * pMountFile = fopen("/tmp/mounts", "rb+");
	if (NULL == pMountFile)
	{
		return false;
	}
	fseek(pMountFile, 0, SEEK_END);
	int fileLen = ftell(pMountFile);
	if (-1 == fileLen)
	{
		fclose(pMountFile);
		return false;
	}

	fseek(pMountFile, 0, SEEK_SET);

	if (NULL != m_pMountData)
	{
		delete [] m_pMountData;
		m_pMountData = NULL;
	}

	m_pMountData = new char [fileLen + 120];
	if (NULL == m_pMountData)
	{
		fclose(pMountFile);
		return false;
	}

	memset(m_pMountData, 0, fileLen + 120);
	fread(m_pMountData, fileLen, 1, pMountFile);

	fclose(pMountFile);

	printf("%d %s\n", fileLen, m_pMountData);
	return true;
}

bool CMount::GetMountInfo(char * pDev, char * pFileName, const char *pFilePath)
{
	if (NULL == m_pMountData)
	{
		return false;
	}

	char path[10] = {0};

	memcpy(path, pFilePath, 7);

	char * pPos = strstr(m_pMountData, path);
	if (NULL != pPos)
	{
		pPos -= 10;
		memcpy(pDev, pPos, 9);
		pDev[9]='\0';
		strcpy(pFileName, pFilePath + 8);

		return true;
	}
	else
	{
		return false;
	}
}

/*
int main()
{
	CMount * pMount = CMount::Instance();
	pMount->LoadMountInfo();
	char dev[40] = {0};
	char fileName[40] = {0};
	char filePath[60] = "/mnt/00/1009.dat";
	pMount->GetMountInfo(dev, fileName, filePath);
	printf("dev=%s, filename=%s, filepath=%s\n", dev, fileName, filePath);


	pMount->GetMountInfo(dev, fileName, "/mnt/01/1009.dat");
	printf("dev=%s, filename=%s, filepath=/mnt/01/1009.dat\n", dev, fileName);


	pMount->GetMountInfo(dev, fileName, "/mnt/02/1009.dat");
	printf("dev=%s, filename=%s, filepath=/mnt/02/1009.dat\n", dev, fileName);

	pMount->GetMountInfo(dev, fileName, "/mnt/03/1009.dat");
	printf("dev=%s, filename=%s, filepath=/mnt/03/1009.dat\n", dev, fileName);
}
*/