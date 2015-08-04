#include "SnapMan.h"
#include "JpegDecode.h"

const char szPicMan[] = "PictMan.dat";
const char szPicType[] = ".jpeg";
const char g_szVersion[] = "SnapMan1.0";
CSnapMan *CSnapMan::m_pSnapMan = NULL;

CSnapMan::CSnapMan(void)
{
	memset(m_szBasePath, 0 , sizeof(m_szBasePath));
	memset(&m_stLastInsert, 0, sizeof(m_stLastInsert));
	m_stLastInsert.diskIndex = 254;  //
}

CSnapMan::~CSnapMan(void)
{
	Quit();
}



CSnapMan *CSnapMan::GetInstance()
{
	if (NULL == m_pSnapMan)
	{
		m_pSnapMan = new CSnapMan;
	}

	return m_pSnapMan;
}

//log文件结构
/*****************************************
*1.version 信息。
*2.log管理文件个数。
*3.每个文件存放文件个数。
*4.第一个文件名称。
*5.第一个文件图片信息。
*6.重复 4和5 次数为文件个数-1次。
******************************************/
bool CSnapMan::CreateLogFile(const char *pFilePath, unsigned long fileNum, unsigned long fileBegineNum, int nMaxPic)
{
	assert(NULL != pFilePath);
	assert(fileNum > 0);
	//assert(videoInputNum > 0);
	assert(nMaxPic >0);

	if ((NULL == pFilePath) || (0 == fileNum))
	{
		printf("create log file fail, parameter error! file=%s, line=%d\n", __FILE__, __LINE__);
		return false;
	}

	FILE *fp = fopen (pFilePath, "wb+");
	if (NULL == fp)
	{
		printf("Open file path = %s fail! file=%s, line=%d\n", pFilePath, __FILE__, __LINE__);
		return false;
	}

	PICTURE_MAN_INFO stManInfo;
	memset(&stManInfo, 0, sizeof(stManInfo));
	stManInfo.nFileNum = fileNum;
	stManInfo.nMaxPct = nMaxPic;
	strcpy(stManInfo.szVersion, g_szVersion);
	
	int nWrite = fwrite(&stManInfo, sizeof(PICTURE_MAN_INFO), 1,fp);//写log文件总信息。
	assert(nWrite == 1);

	SNAP_FILE_BASE stBase;
	PPICTURE_INFO pstInfo = new PICTURE_INFO[nMaxPic];
	if (NULL == pstInfo)
	{
		printf("New fail! file=%s, line=%d\n", __FILE__, __LINE__);
		fclose(fp);
		return false;
	}
	memset(pstInfo, 0, sizeof(PICTURE_INFO)*nMaxPic);
	for (int i=0; i<fileNum; i++)
	{
		stBase.nFileName = fileBegineNum+i;
		stBase.nFill = 0;
		nWrite = fwrite(&stBase, sizeof(SNAP_FILE_BASE), 1, fp);//单个文件基本信息。
		assert(nWrite == 1);
		nWrite = fwrite(pstInfo, sizeof(PICTURE_INFO)*nMaxPic, 1, fp);//单个文件图片信息
		assert(nWrite == 1);
	}
	fclose(fp);
	delete []pstInfo;
	return true;
}

bool CSnapMan::DeleteDiskLogData(SNAP_DISK_INFO &diskNode)
{
	m_lock.Lock();
	bool ret = DeleteDiskLogDataProc(diskNode);
	m_lock.UnLock();
	return ret;
}

bool CSnapMan::Initial(DISK_LOG_INFO *pDiskStatus, int diskNum, const char * pFilePath)
{	
	if (NULL != pFilePath)        //记录基本路径。
	{
		strcpy(m_szBasePath, pFilePath);
	}

	if (diskNum <= 0)
	{
		return true;
	}

	return AddDisk(pDiskStatus, diskNum);
}

void CSnapMan::Quit()
{
	while(m_stDiskList.GetCount()> 0)
	{
		SNAP_DISK_INFO stNode = m_stDiskList.RemoveHead();
		DeleteDiskLogDataProc(stNode);
	}
}

bool CSnapMan::AddDisk(DISK_LOG_INFO *pDiskStatus, unsigned char diskNum)
{
	m_lock.Lock();
	bool bSucc = true;
	int nOldCount = m_stDiskList.GetCount();
	
	if (diskNum <= 0)
	{
		m_lock.UnLock();
		return true;
	}

	for (int i=0; i<diskNum; i++)
	{
		POS pos = m_stDiskList.GetHeadPosition();
		while(NULL != pos)
		{
			SNAP_DISK_INFO &node = m_stDiskList.GetAt(pos);

			if (node.stDisk.diskIndex == pDiskStatus[i].diskIndex)
			{
				DeleteDiskLogDataProc(node);
				m_stDiskList.RemoveAt(pos);
				break;
			}
			m_stDiskList.GetNext(pos);
		}
	}
	//从磁盘中读取出抓图信息。
	for (int i=0; i<diskNum; i++)
	{
		char szPath[256];
		memset(szPath, 0, sizeof(szPath));
		sprintf(szPath, "%s/%02d/%s", m_szBasePath, pDiskStatus[i].diskIndex, szPicMan);
		FILE *fp = fopen(szPath, "r+");
		if (NULL == fp)
		{
			printf("Open file path = %s fail! file=%s, line=%d\n", szPath, __FILE__, __LINE__);
			bSucc = false;
			break;
		}

		PICTURE_MAN_INFO stManInfo;
		memset(&stManInfo, 0, sizeof(stManInfo));
		//读取log文件总信息。
		fseek(fp, 0, SEEK_SET);
		int nRead = fread(&stManInfo, sizeof(PICTURE_MAN_INFO), 1, fp);
		assert(nRead == 1);

		SNAP_DISK_INFO diskTmp;
		memset(&diskTmp, 0, sizeof(SNAP_DISK_INFO));
		diskTmp.nMaxPct = stManInfo.nMaxPct;
		diskTmp.nLogNum = stManInfo.nFileNum;

		//得到管理的文件信息。
		if (stManInfo.nFileNum >0)	
		{
			SNAP_LOG_INFO *pstInfo = new SNAP_LOG_INFO[stManInfo.nFileNum];
			if (NULL == pstInfo)
			{
				printf("New fail! file=%s, line=%d\n", __FILE__, __LINE__);
				fclose(fp);
				bSucc = false;
				break;
			}
			memset(pstInfo, 0, sizeof(SNAP_LOG_INFO)*stManInfo.nFileNum);
			SNAP_FILE_BASE stBase;
			for (int icount=0; icount<stManInfo.nFileNum; icount++)
			{
				memset(&stBase, 0, sizeof(SNAP_FILE_BASE));           //读文件头。
				nRead = fread(&stBase, sizeof(SNAP_FILE_BASE), 1, fp);
				assert(nRead == 1);
				
				PPICTURE_INFO pInfo = new PICTURE_INFO[diskTmp.nMaxPct];   //读文件图片信息。
				if (pInfo == NULL)
				{
					fclose(fp);
					bSucc = false;
					for (int j=0; j<icount; j++)
					{
						delete []pstInfo[icount].pstPctInfo;
					}
					goto END;
				}
				memset(pInfo, 0, sizeof(PICTURE_INFO)*diskTmp.nMaxPct);
				nRead = fread(pInfo, sizeof(PICTURE_INFO)*diskTmp.nMaxPct, 1, fp);
				assert(nRead == 1);
				pstInfo[icount].fileBase = stBase;
				pstInfo[icount].pstPctInfo = pInfo;
			}

			diskTmp.bAction = true;
			diskTmp.stDisk = pDiskStatus[i];
			diskTmp.pLogInfo = pstInfo;
		}
		else
		{
			diskTmp.bAction = true;
			diskTmp.stDisk = pDiskStatus[i];
			diskTmp.nLogNum = 0;
			diskTmp.pLogInfo = NULL;
		}
		fclose(fp);
		m_stDiskList.AddTail(diskTmp);
		printf("add disk path = %s\n", szPath);
	}
END:
	if (!bSucc)//如果有一个添加不成功，则把前面添加的也删除掉。
	{
		int nNewCount = m_stDiskList.GetCount();
		int count = 0;
		POS pos = m_stDiskList.GetTailPosition();

		while ((NULL != pos) && (count < nNewCount - nOldCount))
		{
			SNAP_DISK_INFO &stNode = m_stDiskList.GetAt(pos);
			DeleteDiskLogDataProc(stNode);
			m_stDiskList.RemoveAt(pos);
			pos = m_stDiskList.GetTailPosition();
			count++;
		}
		printf("Delete disk\n");
		m_lock.UnLock();
		return false;
	}
	m_lock.UnLock();
	return true;
}

bool CSnapMan::ActionDisk(DISK_LOG_INFO *pDiskStatus, unsigned char num, bool bAction)
{
	m_lock.Lock();
	assert(num > 0);

	POS pos = m_stDiskList.GetHeadPosition();
	while (NULL != pos)
	{
		SNAP_DISK_INFO &stNode = m_stDiskList.GetAt(pos);
		for (int i = 0; i < num; ++i)
		{
			if (stNode.stDisk.diskIndex == pDiskStatus[i].diskIndex)
			{
				printf("Action disk status=%d\n", bAction);
				stNode.bAction = bAction;
				stNode.stDisk.diskOwnedBy = pDiskStatus[i].diskOwnedBy;
				stNode.stDisk.diskProperty = pDiskStatus[i].diskProperty;
				break;
			}
		}
		m_stDiskList.GetNext(pos);
	}
	m_lock.UnLock();
	return true;
}

bool CSnapMan::ActionAllDisk(bool bAction)
{
	m_lock.Lock();
	POS pos = m_stDiskList.GetHeadPosition();
	while (NULL != pos)
	{
		SNAP_DISK_INFO &stNode = m_stDiskList.GetAt(pos);
		stNode.bAction = bAction;
		m_stDiskList.GetNext(pos);
	}
	m_lock.UnLock();
	return true;
}



bool CSnapMan::GetUsingPartitionMask(ULONGLONG heightMask, ULONGLONG lowMask)
{
	m_lock.Lock();

	ULONGLONG lRet = 0;
	ULONGLONG hRet = 0;
	
	if (m_stLastInsert.diskIndex == 254)
	{
		m_lock.UnLock();
		return false;
	}

	if (m_stDiskList.IsEmpty())
	{
		m_lock.UnLock();
		return false;
	}

	bool bFind = false;
	POS pos = m_stDiskList.GetHeadPosition();
	while (pos != NULL)
	{
		SNAP_DISK_INFO &diskInfo = m_stDiskList.GetAt(pos);
		if (diskInfo.stDisk.diskIndex == m_stLastInsert.diskIndex)
		{
			if (diskInfo.bAction && (diskInfo.stDisk.diskOwnedBy == NET_DISK_OWNED_BY_THIS) && (diskInfo.stDisk.diskProperty != NET_DISK_ONLY_READ))
			{
				bFind = true;
			}
			break;
		}
		m_stDiskList.GetNext(pos);
	}

	if (!bFind)
	{
		m_lock.UnLock();
		return false;
	}

	if (m_stLastInsert.diskIndex < 64)
	{
		lRet |= (((ULONGLONG)1) << (m_stLastInsert.diskIndex));
		hRet |= 0;
	}
	else
	{
		lRet |= 0;
		hRet |= (((ULONGLONG)1) << (m_stLastInsert.diskIndex));
	}
	

	lowMask    = lRet;
	heightMask = hRet;

	m_lock.UnLock();
	return true;
}


//从所有管理的文件中寻找能放置图片的位置。
bool CSnapMan::GetOneEmptyPos(int *pDiskIndex, int *pLogPos, int *pPos)
{
	POS pos = m_stDiskList.GetHeadPosition();
	
	if (m_stLastInsert.nPos>0)
	{
		while(NULL != pos)
		{
			SNAP_DISK_INFO &stNode = m_stDiskList.GetAt(pos);
			if ((stNode.stDisk.diskProperty == NET_DISK_READ_WRITE)&&    //磁盘可写&&Action&&拥有可写文件。
				(stNode.bAction)&&(stNode.nLogNum>0))
			{
				if (stNode.stDisk.diskIndex == m_stLastInsert.diskIndex)
				{
					if (m_stLastInsert.nPos >= stNode.nMaxPct-1)
					{
						break;
					}
					if (stNode.pLogInfo[m_stLastInsert.nLogPos].pstPctInfo[m_stLastInsert.nPos+1].bFill == false)
					{
						//printf("lastInsert get pos\n");
						*pDiskIndex = stNode.stDisk.diskIndex;
						*pLogPos = m_stLastInsert.nLogPos;
						*pPos = m_stLastInsert.nPos+1;
						return true;
					}
					else
					{
						break;
					}
				}
			}
			m_stDiskList.GetNext(pos);
		}
		
	}
	
	pos = m_stDiskList.GetHeadPosition();
	while (NULL != pos)
	{
		//printf("Get one empty pos 1\n");
		SNAP_DISK_INFO &stNode = m_stDiskList.GetAt(pos);
		if ((stNode.stDisk.diskProperty == NET_DISK_READ_WRITE)&&    //磁盘可写&&Action&&拥有可写文件。
			(stNode.bAction)&&(stNode.nLogNum>0))
		{
			for (int i=0; i<stNode.nLogNum; i++)
			{
				if (stNode.pLogInfo[i].fileBase.nFill < stNode.nMaxPct)
				{
					for (int j=0; i<stNode.nMaxPct; j++)
					{
						if (stNode.pLogInfo[i].pstPctInfo[j].bFill == false)
						{
							*pDiskIndex = stNode.stDisk.diskIndex;
							*pLogPos = i;
							*pPos = j;
							return true;
						}
					}
				}
			}
		}
		m_stDiskList.GetNext(pos);
	}
	return false;
}

bool CSnapMan::UpdateSnapLogFile(int nDiskIndex, SNAP_LOG_INFO* pLogInfo, unsigned int nNum, unsigned int nOffSet, unsigned int nMaxPic)
{
	char szPath[256];

	memset(szPath, 0, sizeof(szPath));
	sprintf(szPath, "%s/%02d/%s", m_szBasePath, nDiskIndex, szPicMan);

	FILE *fp= fopen(szPath, "r+");
	if (NULL == fp)
	{
		printf("Open file path = %s fail! file=%s, line=%d\n", szPath, __FILE__, __LINE__);
		return false;
	}

	if (-1 == fseek(fp, nOffSet, SEEK_SET))
	{
		printf("Open file path = %s fail! file=%s, line=%d\n", szPath, __FILE__, __LINE__);
		fclose(fp);
		return false;
	}

	int nWrite =0;
	for (int i=0; i<nNum; i++)
	{
		nOffSet += sizeof(unsigned int);
		if (-1 == fseek(fp, nOffSet, SEEK_SET))
		{
			printf("Open file path = %s fail! file=%s, line=%d\n", szPath, __FILE__, __LINE__);
			fclose(fp);
			return false;
		}

		nWrite = fwrite(&pLogInfo[i].fileBase.nFill, sizeof(unsigned int), 1, fp);
		if (1 != nWrite)
		{
			printf("%s:%s:%d, Write snap file base error\n", __FUNCTION__, __FILE__, __LINE__);
			fclose(fp);
			return false;
		}
		assert(nWrite == 1);
		nOffSet += sizeof(unsigned int);

		nWrite = fwrite(pLogInfo[i].pstPctInfo, sizeof(PICTURE_INFO)*nMaxPic, 1, fp);
		assert(nWrite == 1);
		if (1 != nWrite)
		{
			printf("%s:%s:%d, Write snap file base error\n", __FUNCTION__, __FILE__, __LINE__);
			fclose(fp);
			return false;
		}
		nOffSet += sizeof(PICTURE_INFO) * nMaxPic;
	}
	
	fclose(fp);
	fp = NULL;
	return true;
}

//删除每个文件中最老的一张图片，并返回最先删除的磁盘文件位置。
bool CSnapMan::DeleteTheOldestFromFile(int *pDiskIndex, int *pFileNum, int *pPicNum)
{
	POS pos = m_stDiskList.GetHeadPosition();
	bool bSucc = false;
	bool bFirst = true;

	while(pos!=NULL)
	{
		SNAP_DISK_INFO &stNode = m_stDiskList.GetAt(pos);

		if ((stNode.stDisk.diskProperty == NET_DISK_READ_WRITE)&&    //磁盘可写&&Action&&拥有可写文件。
			(stNode.bAction)&&
			(stNode.nLogNum>0))
		{
			bool bChange = false;
			for (int i=0; i<stNode.nLogNum; i++)
			{
				ULONGLONG time = MAX_TIME;
				int nPos = 0;

				////assert(stNode.pLogInfo[i].nFill == MAX_PICTURE_IN_ONE_FILE);

				for (int j=0; j<stNode.nMaxPct; j++)//找到最老的未锁定的位置。
				{
					if ((stNode.pLogInfo[i].pstPctInfo[j].time < time)&&
						!stNode.pLogInfo[i].pstPctInfo[j].bLock)
					{
						time = stNode.pLogInfo[i].pstPctInfo[j].time;
						nPos = j;
					}
				}

				if (time != MAX_TIME)
				{
					bChange = true;
					stNode.pLogInfo[i].fileBase.nFill--;
					memset(&stNode.pLogInfo[i].pstPctInfo[nPos], 0, sizeof(PICTURE_INFO));
					//printf("delete file from /mnt/%02d/%s pos = %d\n", stNode.stDisk.diskIndex, stNode.pLogInfo[i].szFileName, nPos);
					if (bFirst)
					{
						*pDiskIndex = stNode.stDisk.diskIndex;
						*pFileNum = i;
						*pPicNum = nPos;
						bFirst = false;
					}
				}
			}
			
			if (bChange)
			{
				UpdateSnapLogFile(stNode.stDisk.diskIndex, stNode.pLogInfo, stNode.nLogNum, sizeof(PICTURE_MAN_INFO), stNode.nMaxPct);
				bSucc = true;
			}
		}
		m_stDiskList.GetNext(pos);
	}

	return bSucc;
}

bool CSnapMan::InsertToDeleteList(int index, int nFilePos, int nPos, LONGLONG time, OUT_PICTURE_INFO_LIST &list)
{
	if (list.GetCount() < MAX_DELETE_OLD)
	{
		OUT_PICTURE_INFO node;
		memset(&node, 0, sizeof(OUT_PICTURE_INFO));
		node.diskIndex = index;
		node.nLogPos = nFilePos;
		node.nPos = nPos;
		node.attribute.bLock = false;
		node.attribute.time = time;

		if (list.GetCount()<=0)
		{
			list.AddTail(node);
			return true;
		}
		POS pos = list.GetHeadPosition();
		while(pos!= NULL)
		{
			OUT_PICTURE_INFO &nodeTmp = list.GetAt(pos);
			if ( node.attribute.time >= nodeTmp.attribute.time)
			{
				list.InsertBefore(pos, node);
				return true;
			}
			list.GetNext(pos);
		}
		list.AddTail(node);
		
	}
	else
	{
		
		OUT_PICTURE_INFO &nodeHead = list.GetHead();
		if (time < nodeHead.attribute.time)
		{
			OUT_PICTURE_INFO node;
			memset(&node, 0, sizeof(OUT_PICTURE_INFO));
			node.diskIndex = index;
			node.nLogPos = nFilePos;
			node.nPos = nPos;
			node.attribute.bLock = false;
			node.attribute.time = time;
			list.RemoveHead();
			POS pos = list.GetHeadPosition();
			while(pos != NULL)
			{
				OUT_PICTURE_INFO &nodeTmp = list.GetAt(pos);
				if (node.attribute.time >= nodeHead.attribute.time)
				{
					list.InsertBefore(pos, node);
					return true;
				}
				list.GetNext(pos);
			}
			list.AddTail(node);
		}
	}
	return true;
}

//删除所有文件中最老的100张图片，并返回最先删除的磁盘文件位置。
bool CSnapMan::DeleteNOldest(int *pDiskIndex, int *pFileNum, int *pPicNum)
{
	OUT_PICTURE_INFO_LIST list;
	ULONGLONG time = MAX_TIME;
	POS pos = m_stDiskList.GetHeadPosition();

	while(pos!=NULL)
	{
		SNAP_DISK_INFO &stNode = m_stDiskList.GetAt(pos);

		if ((stNode.stDisk.diskProperty == NET_DISK_READ_WRITE)&&    //磁盘可写&&Action&&拥有可写文件。
			(stNode.bAction)&&
			(stNode.nLogNum>0))
		{
			for (int i=0; i<stNode.nLogNum; i++)
			{
				for (int j=0; j<stNode.nMaxPct; j++)//找到最老的未锁定的位置。
				{
					if (!stNode.pLogInfo[i].pstPctInfo[j].bLock)
					{
						InsertToDeleteList(stNode.stDisk.diskIndex, i, j, stNode.pLogInfo[i].pstPctInfo[j].time, list);
					}
				}
			}
		}
		m_stDiskList.GetNext(pos);
	}
	
	if (list.GetCount()>0)
	{
		OUT_PICTURE_INFO&node = list.GetTail();
		*pDiskIndex = node.diskIndex;
		*pFileNum = node.nLogPos;
		*pPicNum = node.nPos;
		//printf("begine delete1\n");
		OnBatchDelete(list);
		//printf("end delete\n");
		list.RemoveAll();
	}
	else
	{
		return false;
	}
	return true;
}

bool CSnapMan::DeleteDiskLogDataProc(SNAP_DISK_INFO &diskNode)
{
	if((diskNode.nLogNum > 0) && (diskNode.pLogInfo != NULL))
	{
		for(int i=0; i<diskNode.nLogNum; i++)
		{
			if (diskNode.pLogInfo[i].pstPctInfo != NULL)
			{
				delete []diskNode.pLogInfo[i].pstPctInfo;
				diskNode.pLogInfo[i].pstPctInfo = NULL;
			}
		}
		delete []diskNode.pLogInfo;
		diskNode.pLogInfo = NULL;
	}	
	return true;
}


bool CSnapMan::SaveBufTofile(void*buf, int nLen, int type, int nChannel, 
							 bool bLock, int nDiskIndex, int nLogPos, int nPos, 
							 LONGLONG time, unsigned short width, unsigned short height)
{
	char filePath[256];
	FILE *fp =NULL;
	POS pos =NULL;
	int nRzt = 0;

	memset(filePath, 0, sizeof(filePath));
	pos = m_stDiskList.GetHeadPosition();	
	while(NULL != pos)
	{
		SNAP_DISK_INFO &stNode = m_stDiskList.GetAt(pos);
		if (stNode.stDisk.diskIndex == nDiskIndex)
		{
			unsigned char *pTmp = new unsigned char[MAX_PICTURE_SIZE];
			if (NULL == pTmp)
			{
				printf("New error! file = %s, line = %d\n", __FILE__, __LINE__);
				return false;
			}
			memset(pTmp, 0, sizeof(char)*MAX_PICTURE_SIZE);
			memcpy(pTmp, buf, nLen);

			sprintf(filePath, "%s/%02d/%08d.dat", m_szBasePath, nDiskIndex, stNode.pLogInfo[nLogPos].fileBase.nFileName);
			fp = fopen(filePath, "r+");
			if (NULL == fp)
			{
				printf("Open file path = %s fail! file=%s, line=%d\n", filePath,  __FILE__, __LINE__);
				delete[]pTmp;
				return false;
			}
			fseek(fp, nPos*MAX_PICTURE_SIZE, SEEK_SET);
			nRzt = fwrite(pTmp, MAX_PICTURE_SIZE, 1, fp);
			
			
			assert(nRzt == 1);
			fclose(fp);
			fp = NULL;
			//printf("save picture path = %s, pos = %d\n", filePath, nPos);

			delete[]pTmp;
			stNode.pLogInfo[nLogPos].pstPctInfo[nPos].bFill = true;
			stNode.pLogInfo[nLogPos].pstPctInfo[nPos].nLen = nLen;
			stNode.pLogInfo[nLogPos].pstPctInfo[nPos].nChannel = nChannel;
			stNode.pLogInfo[nLogPos].pstPctInfo[nPos].nType  = type;
			stNode.pLogInfo[nLogPos].pstPctInfo[nPos].bLock  = bLock;
			stNode.pLogInfo[nLogPos].pstPctInfo[nPos].time   = time;
			stNode.pLogInfo[nLogPos].fileBase.nFill++;

			nRzt = UpdateSnapLogFile(stNode.stDisk.diskIndex, stNode.pLogInfo, stNode.nLogNum, sizeof(PICTURE_MAN_INFO), stNode.nMaxPct);

			assert(nRzt == true);
			break;
		}
		m_stDiskList.GetNext(pos);
	}
	//printf("Save picture end\n");
	return true;
}

//保存抓图到磁盘,增加。
bool CSnapMan::SaveSnapPicture(void*buf, int nLen, int ntype, int nChannel,  LONGLONG time, unsigned short width, unsigned short height, bool bLock)
{
	m_lock.Lock();
	int nLogPos = 0;
	int nPos = 0; 
	int nRzt = 0;
	int nDiskIndex = 0;

	if (0 >= nLen || MAX_PICTURE_SIZE < nLen)
	{
		printf("Buf error size = %d fail! file=%s, line=%d\n", nLen,  __FILE__, __LINE__);
		m_lock.UnLock();
		return true;
	}
	
	if (GetOneEmptyPos(&nDiskIndex, &nLogPos, &nPos)) //有空间
	{
		m_stLastInsert.diskIndex = nDiskIndex;
		m_stLastInsert.nLogPos = nLogPos;
		m_stLastInsert.nPos = nPos;
		bool ret = SaveBufTofile(buf, nLen, ntype, nChannel, bLock, nDiskIndex, nLogPos, nPos, time, width, height);
		if (!ret)
		{
			//如果写失败，表示写的文件有问题，删除当前写的分区
			DeleteOneDisk(nDiskIndex);
		}
		m_lock.UnLock();
		return ret;
	}
	else  //没空间。
	{
		//删除掉每一个管理文件中保存的最老的图片。
		if (DeleteNOldest(&nDiskIndex, &nLogPos, &nPos))
		{
			m_stLastInsert.diskIndex = nDiskIndex;
			m_stLastInsert.nLogPos = nLogPos;
			m_stLastInsert.nPos = nPos;
			//将需添加的图片添加到删除得到的空位置。
			bool ret = SaveBufTofile(buf, nLen, ntype, nChannel, bLock, nDiskIndex, nLogPos, nPos, time, width, height);
			if (!ret)
			{
				//如果写失败，表示写的文件有问题，删除当前写的分区
				DeleteOneDisk(nDiskIndex);
			}
			m_lock.UnLock();
			return ret;
		}
		else
		{
			m_lock.UnLock();
			return false;
		}
	}

	m_lock.UnLock();	
	return true;
}

bool CSnapMan::AddToListBySort(OUT_PICTURE_INFO_LIST &list, OUT_PICTURE_INFO& node, SORT_TYPE sort)
{
	if (list.GetCount()<=0)
	{
		list.AddTail(node);
		return true;
	}

	switch (sort)
	{
	case SORT_BY_TIME:
		{
			OUT_PICTURE_INFO stNode;
			stNode = list.GetHead();
			if (stNode.attribute.time <= node.attribute.time)//比最新的还新，加入到最前面。
			{
				//printf("add head\n");
				list.AddHead(node);
				break;
			}
			stNode = list.GetTail();
			if (stNode.attribute.time >= node.attribute.time)//比最老的还老，加入到最后面。
			{
				printf("add tail\n");
				list.AddTail(node);
				break;
			}

			//特殊处理方法。
			{
				POS posInsert = list.GetLastInsertPosition();
				OUT_PICTURE_INFO &Insert = list.GetPrev(posInsert);
				OUT_PICTURE_INFO &InsertPrv = list.GetAt(posInsert);
				if ((Insert.attribute.time<= node.attribute.time)&&
					(InsertPrv.attribute.time >= node.attribute.time) )
				{
					list.InsertAfter(posInsert, node);
					//printf("Insert afte\n");
					break;
				}
			}
			
			POS pos = list.GetHeadPosition();
			while(pos!= NULL)
			{
				OUT_PICTURE_INFO &NodeTmp = list.GetAt(pos);
				if (NodeTmp.attribute.time <= node.attribute.time)
				{
					//printf("add center\n");
					list.InsertBefore(pos, node);
					break;
				}
				
				list.GetNext(pos);
			}
		}
		break;
	case SORT_BY_CHANNEL:
		{
			POS pos = list.GetHeadPosition();
			while(pos!= NULL)
			{
				OUT_PICTURE_INFO &stTmp = list.GetAt(pos);
				if (stTmp.attribute.nChannel = node.attribute.nChannel)
				{
					if (stTmp.attribute.time < node.attribute.time)
					{
						list.InsertBefore(pos, node);
						return true;
					}
				}
				else if (stTmp.attribute.nChannel > node.attribute.nChannel)
				{
					list.InsertBefore(pos, node);
					return true;
				}

				list.GetNext(pos);
			}
			list.AddTail(node);
		}
		break;
	case SORT_BY_TRIG_TYPE:
		{
			POS pos = list.GetHeadPosition();
			while(pos!= NULL)
			{
				OUT_PICTURE_INFO &stTmp = list.GetAt(pos);
				if (stTmp.attribute.nType = node.attribute.nType)
				{
					if (stTmp.attribute.time < node.attribute.time)
					{
						list.InsertBefore(pos, node);
						return true;
					}
				}
				else if (stTmp.attribute.nType > node.attribute.nType)
				{
					list.InsertBefore(pos, node);
					return true;
				}

				list.GetNext(pos);
			}
			list.AddTail(node);
		}
		break;
	}
	return true;
}

bool CSnapMan::GetPicture(unsigned long chnn, unsigned long type, LONGLONG startTime, \
								   LONGLONG endTime, OUT_PICTURE_INFO_LIST &list)
{
	m_lock.Lock();
	POS pos = m_stDiskList.GetHeadPosition();

	while(NULL != pos)
	{
		SNAP_DISK_INFO &stNode = m_stDiskList.GetAt(pos);
		if ((stNode.bAction)&&(stNode.nLogNum>0) && (stNode.pLogInfo!=NULL))
		{
			for (int i=0; i<stNode.nLogNum; i++)
			{
				for (int j=0; j<stNode.nMaxPct; j++)
				{
					if ((stNode.pLogInfo[i].pstPctInfo[j].bFill == true)&&
						(stNode.pLogInfo[i].pstPctInfo[j].nChannel == chnn)&&
						(stNode.pLogInfo[i].pstPctInfo[j].time>=startTime)&&
						(stNode.pLogInfo[i].pstPctInfo[j].time<=endTime)&&
						(stNode.pLogInfo[i].pstPctInfo[j].nType == type))
					{
						OUT_PICTURE_INFO out;
						out.diskIndex = stNode.stDisk.diskIndex;
						out.attribute = stNode.pLogInfo[i].pstPctInfo[j];
						out.nPos = j;
						out.nLogPos = i;
						AddToListBySort(list, out, SORT_BY_TIME);
					}
				}
			}
		}
		m_stDiskList.GetNext(pos);
	}
	m_lock.UnLock();
	return true;
}

bool CSnapMan::IsInChannels(unsigned long *chnns,int chnnNum, unsigned long chann)
{
	if (NULL == chnns)
	{
		printf("No channs return false! file=%s, line=%d\n", __FILE__, __LINE__);
		return false;
	}
	for (int i= 0; i<chnnNum; i++)
	{
		if (chnns[i] == chann)
		{
			return true;
		}
	}
	return false;
}

bool CSnapMan::IsInType(unsigned long *types,int typeNum, unsigned long type)
{
	if (NULL == types)
	{
		printf("No types return false! file=%s, line=%d\n", __FILE__, __LINE__);
		return false;
	}
	for (int i= 0; i<typeNum; i++)
	{
		if (types[i] == type)
		{
			return true;
		}
	}
	return false;
}

bool  CSnapMan::GetPicture(unsigned long *chnns, int chnnNum, unsigned long *types, int typeNum, LONGLONG startTime,\
				LONGLONG endTime, OUT_PICTURE_INFO_LIST &list, unsigned long diskOwnBy)
{
	m_lock.Lock();
	POS pos = m_stDiskList.GetHeadPosition();


	while(NULL != pos)
	{
		SNAP_DISK_INFO &stNode = m_stDiskList.GetAt(pos);
		if ((stNode.bAction)&&(stNode.nLogNum>0) && (stNode.pLogInfo!=NULL) && (stNode.stDisk.diskOwnedBy == diskOwnBy))
		{
			for (int i=0; i<stNode.nLogNum; i++)
			{
				for (int j=0; j<stNode.nMaxPct; j++)
				{						
					if ((stNode.pLogInfo[i].pstPctInfo[j].bFill == true)&&
						(IsInChannels(chnns, chnnNum, stNode.pLogInfo[i].pstPctInfo[j].nChannel))&&
						(stNode.pLogInfo[i].pstPctInfo[j].time>=startTime)&&
						(stNode.pLogInfo[i].pstPctInfo[j].time<=endTime)&&
						(IsInType(types, typeNum, stNode.pLogInfo[i].pstPctInfo[j].nType)))
					{						
						OUT_PICTURE_INFO out;
						out.diskIndex = stNode.stDisk.diskIndex;
						out.attribute = stNode.pLogInfo[i].pstPctInfo[j];
						out.nPos = j;
						out.nLogPos = i;
						if (stNode.stDisk.diskOwnedBy == NET_DISK_ONLY_READ)
						{
							SNAP_PIC_EN_ONLYREAD(out.attribute.nType);
						}
					
						AddToListBySort(list, out, SORT_BY_TIME);
					}
				}
			}
		}
		m_stDiskList.GetNext(pos);
	}
	m_lock.UnLock();
	return true;
}

//得到图片总个数
int CSnapMan::GetPictureNum()
{
	m_lock.Lock();
	int nCount = 0;

	//遍历每个文件保存的文件个数。
	POS pos = m_stDiskList.GetHeadPosition();
	while(NULL!=pos)
	{
		SNAP_DISK_INFO &stNode = m_stDiskList.GetAt(pos);
		if (stNode.bAction && (stNode.nLogNum>0) && (stNode.pLogInfo!=NULL))
		{
			for (int i=0; i<stNode.nLogNum; i++)
			{
				nCount += stNode.pLogInfo[i].fileBase.nFill;
			}
		}
		m_stDiskList.GetNext(pos);
	}
	m_lock.UnLock();
	return nCount;
}


bool CSnapMan::GetDiskPictureNum(unsigned char diskIndex, unsigned long &allNum, unsigned long &usedNum)
{
	m_lock.Lock();
	int nCount = 0;

	allNum = 0;
	usedNum = 0;

	//遍历每个文件保存的文件个数。
	POS pos = m_stDiskList.GetHeadPosition();
	while(NULL!=pos)
	{
		SNAP_DISK_INFO &stNode = m_stDiskList.GetAt(pos);
		if (stNode.stDisk.diskIndex == diskIndex)
		{			
			if (stNode.pLogInfo != NULL)
			{
				for (int i=0; i<stNode.nLogNum; i++)
				{
					usedNum += stNode.pLogInfo[i].fileBase.nFill;
					allNum += stNode.nMaxPct;
				}
			}
			break;
		}
		m_stDiskList.GetNext(pos);
	}
	m_lock.UnLock();
	return true;
}


//得到所有图片。
bool CSnapMan::GetAllPicture(OUT_PICTURE_INFO_LIST &list)
{
	m_lock.Lock();
	POS pos = m_stDiskList.GetHeadPosition();

	while(NULL != pos)
	{
		SNAP_DISK_INFO &stNode = m_stDiskList.GetAt(pos);
		if ((stNode.bAction) && (stNode.nLogNum>0) && (stNode.pLogInfo!=NULL))
		{
			for (int i=0; i<stNode.nLogNum; i++)
			{
				for (int j=0; j<stNode.nMaxPct; j++)
				{
					if (stNode.pLogInfo[i].pstPctInfo[j].bFill == true)
					{
						OUT_PICTURE_INFO out;
						out.diskIndex = stNode.stDisk.diskIndex;
						out.attribute = stNode.pLogInfo[i].pstPctInfo[j];
						out.nPos = j;
						out.nLogPos = i;
						AddToListBySort(list, out, SORT_BY_TIME);
					}
				}
			}
		}
		m_stDiskList.GetNext(pos);
	}
	m_lock.UnLock();
	return true;
}

//删除图片。
bool CSnapMan::DeleteOne(OUT_PICTURE_INFO& node)
{
	m_lock.Lock();

	POS pos = m_stDiskList.GetHeadPosition();

	while(NULL != pos)
	{
		SNAP_DISK_INFO &stNode = m_stDiskList.GetAt(pos);
		if (stNode.stDisk.diskIndex == node.diskIndex)
		{
			if ((stNode.stDisk.diskProperty != NET_DISK_READ_WRITE)||			//磁盘只读
				(!stNode.bAction)||												//磁盘睡眠
				(stNode.pLogInfo[node.nLogPos].pstPctInfo[node.nPos].bLock))     //被锁定，不能删除
			{
				printf("File can not delete! file=%s, line=%d\n", __FILE__, __LINE__);
				m_lock.UnLock();
				return false;
			}
			else
			{
				stNode.pLogInfo[node.nLogPos].fileBase.nFill--;
				memset(&stNode.pLogInfo[node.nLogPos].pstPctInfo[node.nPos], 0, sizeof(PICTURE_INFO));

				UpdateSnapLogFile(stNode.stDisk.diskIndex, stNode.pLogInfo, stNode.nLogNum, sizeof(PICTURE_MAN_INFO), stNode.nMaxPct);

				break;
			}
		}
		
		m_stDiskList.GetNext(pos);
	}
	m_lock.UnLock();
	return true;
}

bool CSnapMan::DeleteAll()
{
	m_lock.Lock();
	POS pos = m_stDiskList.GetHeadPosition();

	while(NULL != pos)
	{
		SNAP_DISK_INFO &stNode = m_stDiskList.GetAt(pos);
		if (stNode.bAction&&
		   (stNode.stDisk.diskProperty == NET_DISK_READ_WRITE) &&
		   (stNode.stDisk.diskOwnedBy == NET_DISK_OWNED_BY_THIS))
		{
			if ((stNode.nLogNum>0) && (stNode.pLogInfo!=NULL))
			{
				for (int i=0; i<stNode.nLogNum; i++)
				{
					//只读图片不能删除。
					for (int j=0; j<stNode.nMaxPct; j++)
					{
						if (stNode.pLogInfo[i].pstPctInfo[j].bFill && 
							!stNode.pLogInfo[i].pstPctInfo[j].bLock)
						{
							stNode.pLogInfo[i].fileBase.nFill--;
							memset(&stNode.pLogInfo[i].pstPctInfo[j], 0, sizeof(PICTURE_INFO));
						}
					}
				}
				
				UpdateSnapLogFile(stNode.stDisk.diskIndex, stNode.pLogInfo, stNode.nLogNum, sizeof(PICTURE_MAN_INFO), stNode.nMaxPct);
			}
			
		}
		m_stDiskList.GetNext(pos);
	}
	m_lock.UnLock();
	return true;
}

//批量删除，删除指定个数的图片。
bool CSnapMan::OnBatchDelete(OUT_PICTURE_INFO_LIST& list)
{
	//printf("begine OnBatchDelete listcount = %d\n", list.GetCount());
	POS pos = m_stDiskList.GetHeadPosition();
	while( pos != NULL)
	{
		SNAP_DISK_INFO &stNode = m_stDiskList.GetAt(pos);
		if (stNode.bAction&&
			(stNode.stDisk.diskProperty == NET_DISK_READ_WRITE) &&
			(stNode.stDisk.diskOwnedBy == NET_DISK_OWNED_BY_THIS))
		{
			//printf("begine OnBatchDelete1\n");
			if ((stNode.nLogNum>0) && (stNode.pLogInfo!=NULL))
			{
				POS posTmp = list.GetHeadPosition();
				bool bChange = false;
				//printf("begine OnBatchDelete2\n");
				while(NULL != posTmp)
				{
					OUT_PICTURE_INFO &outNode = list.GetAt(posTmp);
					
					if ((outNode.diskIndex == stNode.stDisk.diskIndex)&&
						!stNode.pLogInfo[outNode.nLogPos].pstPctInfo[outNode.nPos].bLock)
					{
						//printf("delete file /mnt/%02d/%s, nfile = %d, nPos=%d\n", 
								//outNode.diskIndex, stNode.pLogInfo[outNode.nLogPos].szFileName, outNode.nLogPos, outNode.nPos);
						stNode.pLogInfo[outNode.nLogPos].fileBase.nFill--;
						memset(&stNode.pLogInfo[outNode.nLogPos].pstPctInfo[outNode.nPos], 0, sizeof(PICTURE_INFO));
						bChange = true;
					}
					list.GetNext(posTmp);
				}

				if (bChange)
				{
					//printf("begine save\n");
					UpdateSnapLogFile(stNode.stDisk.diskIndex, stNode.pLogInfo, stNode.nLogNum, sizeof(PICTURE_MAN_INFO), stNode.nMaxPct);
				}
			}
		}

		m_stDiskList.GetNext(pos);
	}
	return true;
}
bool CSnapMan::GetPictureData(SNAP_PIC_PRORITY picNode, unsigned char *buf, int &picWidth, int &picHeight, int &picLen)
{
	m_lock.Lock();
	POS pos = m_stDiskList.GetHeadPosition();
	while(NULL != pos)
	{
		SNAP_DISK_INFO &stNode = m_stDiskList.GetAt(pos);
		if (stNode.stDisk.diskIndex == picNode.diskIndex)
		{
			if (stNode.bAction == false)
			{
				m_lock.UnLock();
				return false;
			}

			bool bFindPic = false;
			SNAP_LOG_INFO * pLogInfo = stNode.pLogInfo + picNode.nLogPos;
			PPICTURE_INFO pstPctInfo = NULL;
			for (int i=0; i<stNode.nMaxPct; i++)
			{
				pstPctInfo = pLogInfo->pstPctInfo + i;
				if ((pstPctInfo->nChannel == picNode.nChannel) && (pstPctInfo->nType == picNode.nType) && (pstPctInfo->time == picNode.snapTime))
				{
					bFindPic = true;
				}
			}

			if (!bFindPic)
			{
				printf("%s:%s:%d, can not find pic\n", __FUNCTION__, __FILE__, __LINE__);
				m_lock.UnLock();
				return false;
			}

			int nRead = 0;

			char filePath[256] = {0};
			sprintf(filePath, "%s/%02d/%08d.dat", m_szBasePath, picNode.diskIndex, stNode.pLogInfo[picNode.nLogPos].fileBase.nFileName);
			FILE *fp = fopen(filePath, "r");
			if (NULL == fp)
			{
				printf("Open file path = %s fail! file=%s, line=%d\n", filePath,  __FILE__, __LINE__);
				m_lock.UnLock();
				return false;
			}
			fseek(fp, MAX_PICTURE_SIZE*picNode.nPos, SEEK_SET);
			unsigned char *pJpegBuf = new unsigned char [picNode.nLen];
			memset(pJpegBuf, 0, sizeof(unsigned char )* picNode.nLen);
			if (1 != fread(pJpegBuf, picNode.nLen, 1, fp))
			{
				fclose(fp);
				delete [] pJpegBuf;
				fp = NULL;
				m_lock.UnLock();
				return false;
			}
			fclose(fp);
			fp = NULL;
			
			bool bRet = false;
			CJpegDecode decode;
			if (0 == decode.LoadJpegFile(buf, pJpegBuf, picWidth, picHeight, picLen))
			{
				bRet = true;
			}

			delete [] pJpegBuf;
			pJpegBuf = NULL;

			m_lock.UnLock();
			return bRet;
		}
		m_stDiskList.GetNext(pos);
	}
	m_lock.UnLock();
	return false;

}

bool CSnapMan::WritePictureToFile(SNAP_PIC_PRORITY picNode, FILE *pFile)
{
	m_lock.Lock();
	POS pos = m_stDiskList.GetHeadPosition();
	while(NULL != pos)
	{
		SNAP_DISK_INFO &stNode = m_stDiskList.GetAt(pos);
		if (stNode.stDisk.diskIndex == picNode.diskIndex)
		{
			if (stNode.bAction == false)
			{
				m_lock.UnLock();
				return false;
			}
			char filePath[256] = {0};
			FILE *fp = NULL;
			int nRead = 0;

			sprintf(filePath, "%s/%02d/%08d.dat", m_szBasePath, picNode.diskIndex, stNode.pLogInfo[picNode.nLogPos].fileBase.nFileName);
			fp = fopen(filePath, "r");
			if (NULL == fp)
			{
				printf("Open file path = %s fail! file=%s, line=%d\n", filePath,  __FILE__, __LINE__);
				m_lock.UnLock();
				return false;
			}
			fseek(fp, MAX_PICTURE_SIZE*picNode.nPos, SEEK_SET);
			unsigned char *pBuf = new unsigned char [picNode.nLen];
			memset(pBuf, 0, sizeof(unsigned char )* picNode.nLen);
			nRead = fread(pBuf, picNode.nLen, 1, fp);
			if (nRead != 1)
			{
				fclose(fp);
				fp = NULL;
				delete [] pBuf;
				pBuf = NULL;
				m_lock.UnLock();
				return false;
			}			
			fclose(fp);
			fp = NULL;

			if (picNode.nLen != fwrite(pBuf, sizeof(unsigned char), picNode.nLen, pFile))
			{
				delete [] pBuf;
				pBuf = NULL;
				m_lock.UnLock();
				return false;
			}
			
			delete [] pBuf;
			pBuf = NULL;
			m_lock.UnLock();
			return true;
		}
		m_stDiskList.GetNext(pos);
	}
	m_lock.UnLock();
	return false;

}

bool CSnapMan::GetPictureData(OUT_PICTURE_INFO &node, unsigned char *buf)
{
	m_lock.Lock();
	POS pos = m_stDiskList.GetHeadPosition();
	while(NULL != pos)
	{
		SNAP_DISK_INFO &stNode = m_stDiskList.GetAt(pos);
		if (stNode.stDisk.diskIndex == node.diskIndex)
		{
			if (stNode.bAction == false)
			{
				m_lock.UnLock();
				return false;
			}
			char filePath[256] = {0};
			FILE *fp = NULL;
			int nRead = 0;

			sprintf(filePath, "%s/%02d/%08d.dat", m_szBasePath, node.diskIndex, stNode.pLogInfo[node.nLogPos].fileBase.nFileName);
			fp = fopen(filePath, "r");
			if (NULL == fp)
			{
				printf("Open file path = %s fail! file=%s, line=%d\n", filePath,  __FILE__, __LINE__);
				m_lock.UnLock();
				return false;
			}
			fseek(fp, MAX_PICTURE_SIZE*node.nPos, SEEK_SET);
			nRead = fread(buf, node.attribute.nLen, 1, fp);
			assert(nRead == 1);
			fclose(fp);
			fp = NULL;
			m_lock.UnLock();

			return true;
		}
		m_stDiskList.GetNext(pos);
	}
	m_lock.UnLock();
	return false;
}

//排序。
int CSnapMan::GetOneFileSize(char *fileName)
{
	return 0;
}

bool CSnapMan::SendPicture(OUT_PICTURE_INFO& node, SEND_TYPE type)
{
	return true;
	unsigned char *pBuf = new unsigned char[MAX_PICTURE_SIZE];
	if (NULL == pBuf)
	{
		printf("New faild! file=%s, line=%d\n",  __FILE__, __LINE__);
		return false;
	}
	if (GetPictureData(node, pBuf))
	{
		switch (type)
		{
		case SEND_TYPE_NET://需继续添加具体发送方法。
			break;
		case SEND_TYPE_MAIL:
			break;
		}

		delete []pBuf;
		pBuf = NULL;
	}
	else
	{
		printf("Send faild, the reason is get picture data fail! file=%s, line=%d\n",  __FILE__, __LINE__);
		return false;
	}
	return true;
}

bool CSnapMan::WriteJpegFile(FILE *fp, int nPos, int nLen, char * ForldPath, int i, int nchannel, LONGLONG time)
{
	char path[256] = {0};
	unsigned char *buf = new unsigned char[nLen];
	if (buf==NULL)
	{
		return false;
	}
	fseek(fp, 0, SEEK_SET);
	fseek(fp, nPos*MAX_PICTURE_SIZE, SEEK_SET);
	fread(buf, nLen, 1, fp);
	tm strTime = DVRTimeToTm(time);
	sprintf(path, "%s/%02d_%04d_%02d_%02d_%02d_%02d_%02d_%d.jpg", 
		ForldPath, nchannel+1, strTime.tm_year+1900, strTime.tm_mon+1, strTime.tm_mday, strTime.tm_hour, strTime.tm_min, strTime.tm_sec,i);
	
	FILE *jpgfp = fopen(path,"w+");
	if (NULL == jpgfp)
	{
		delete[]buf;
		return false;
	}
	fwrite(buf, nLen, 1, jpgfp);
	fclose(jpgfp);
	delete[]buf;
	return true;
}

//从.dat文件中导出jpeg文件。
bool CSnapMan::SaveAllAsJpeg(char * ForldPath, int nGet)
{
	m_lock.Lock();
	POS pos = m_stDiskList.GetHeadPosition();
	int nCount = 0;
	
	while(NULL != pos)
	{
		SNAP_DISK_INFO &stNode = m_stDiskList.GetAt(pos);
		if (stNode.bAction&&stNode.nLogNum>0)
		{	
			for (int i=0; i<stNode.nLogNum; i++)
			{
				if (stNode.pLogInfo[i].fileBase.nFill<=0)
				{
					continue;
				}
				char szDataPath[256];
				memset(szDataPath, 0, sizeof(szDataPath));
				sprintf(szDataPath, "%s/%02d/%08d.dat", m_szBasePath, stNode.stDisk.diskIndex, stNode.pLogInfo[i].fileBase.nFileName);
				FILE *fp = fopen(szDataPath, "r+");
				if (NULL == fp)
				{
					m_lock.UnLock();
					return false;
				}
				for (int j=0; j<stNode.nMaxPct; j++)
				{
					if (stNode.pLogInfo[i].pstPctInfo[j].bFill)
					{
						WriteJpegFile(fp, j, stNode.pLogInfo[i].pstPctInfo[j].nLen, ForldPath, nCount, 
							stNode.pLogInfo[i].pstPctInfo[j].nChannel, stNode.pLogInfo[i].pstPctInfo[j].time);
						nCount++;
						if ((nGet > 0) && (nCount >= nGet))
						{
							fclose(fp);
							fp = NULL;
							m_lock.UnLock();
							return true;
						}
					}	
				}
				fclose(fp);
				fp = NULL;
			}
			break;
		}

		m_stDiskList.GetNext(pos);
	}
	m_lock.UnLock();
	return true;
}

bool CSnapMan::SaveBatchAsJpeg(char * ForldPath, OUT_PICTURE_INFO_LIST &list)
{
	m_lock.Lock();
	POS pos = m_stDiskList.GetHeadPosition();
	int nCount = 0;
	while( pos != NULL)
	{
		SNAP_DISK_INFO &stNode = m_stDiskList.GetAt(pos);
		if (stNode.bAction && (stNode.nLogNum>0) && (stNode.pLogInfo!=NULL))
		{
			for (int i=0; i<stNode.nLogNum; i++)
			{
				if (stNode.pLogInfo[i].fileBase.nFill<=0)
				{
					continue;
				}
				char szDataPath[256];
				memset(szDataPath, 0, sizeof(szDataPath));
				sprintf(szDataPath, "%s/%02d/%08d.dat", m_szBasePath, stNode.stDisk.diskIndex, stNode.pLogInfo[i].fileBase.nFileName);
				FILE *fp = fopen(szDataPath, "r+");
				if (NULL == fp)
				{
					m_lock.UnLock();
					return false;
				}
				POS posTmp = list.GetHeadPosition();
				while(NULL != posTmp)
				{
					OUT_PICTURE_INFO &outNode = list.GetAt(posTmp);

					if (outNode.diskIndex == stNode.stDisk.diskIndex)
					{
						WriteJpegFile(fp, outNode.nLogPos, outNode.attribute.nLen, ForldPath, nCount, 
							outNode.attribute.nChannel, outNode.attribute.time);
						nCount++;
					}
					list.GetNext(posTmp);
				}
				fclose(fp);
				fp = NULL;
			}
		}

		m_stDiskList.GetNext(pos);
	}
	m_lock.UnLock();
	return true;
}

bool CSnapMan::SetLockStatus(OUT_PICTURE_INFO& node, bool bLock)
{
	m_lock.Lock();
	POS pos = m_stDiskList.GetHeadPosition();
	while(NULL != pos)
	{
		SNAP_DISK_INFO &stNode = m_stDiskList.GetAt(pos);
		if (stNode.stDisk.diskIndex == node.diskIndex)
		{
			if ((stNode.stDisk.diskProperty != NET_DISK_READ_WRITE)||			//磁盘只读
				(!stNode.bAction)||
				(stNode.stDisk.diskOwnedBy != NET_DISK_OWNED_BY_THIS))     
			{
				m_lock.UnLock();
				return false;
			}
			else
			{
				if(stNode.pLogInfo[node.nLogPos].pstPctInfo[node.nPos].bLock != bLock)
				{
					stNode.pLogInfo[node.nLogPos].pstPctInfo[node.nPos].bLock = bLock;
					UpdateSnapLogFile(stNode.stDisk.diskIndex, stNode.pLogInfo, stNode.nLogNum, sizeof(PICTURE_MAN_INFO), stNode.nMaxPct);
				}
				break;
			}
		}

		m_stDiskList.GetNext(pos);
	}
	m_lock.UnLock();
	return true;
}

bool CSnapMan::SetLockStatusBatch(OUT_PICTURE_INFO_LIST& list, bool bLock)
{
	m_lock.Lock();

	if (list.GetCount() <= 0)
	{
		m_lock.UnLock();
		return true;
	}

	POS pos = m_stDiskList.GetHeadPosition();
	while( pos != NULL)
	{
		SNAP_DISK_INFO &stNode = m_stDiskList.GetAt(pos);
		if ((stNode.bAction)&&
			(stNode.stDisk.diskProperty == NET_DISK_READ_WRITE) &&
			(stNode.stDisk.diskOwnedBy == NET_DISK_OWNED_BY_THIS))
		{
			if ((stNode.nLogNum>0) && (stNode.pLogInfo!=NULL))
			{
				POS posTmp = list.GetHeadPosition();
				bool bChange = false;
				while(NULL != posTmp)
				{
					OUT_PICTURE_INFO &outNode = list.GetAt(posTmp);
					
					if (outNode.diskIndex == stNode.stDisk.diskIndex)
					{
						if(stNode.pLogInfo[outNode.nLogPos].pstPctInfo[outNode.nPos].bLock != bLock)
						{
							stNode.pLogInfo[outNode.nLogPos].pstPctInfo[outNode.nPos].bLock = bLock;
							bChange = true;
						}	
					}
					list.GetNext(posTmp);
				}
				if (bChange)
				{
					UpdateSnapLogFile(stNode.stDisk.diskIndex, stNode.pLogInfo, stNode.nLogNum, sizeof(PICTURE_MAN_INFO), stNode.nMaxPct);
				}
			}
		}

		m_stDiskList.GetNext(pos);
	}
	m_lock.UnLock();
}

bool CSnapMan::DeleteOneDiskPicture(unsigned char diskIndex)
{
	m_lock.Lock();

	POS pos = m_stDiskList.GetHeadPosition();

	while(NULL != pos)
	{
		SNAP_DISK_INFO &stNode = m_stDiskList.GetAt(pos);
		if (stNode.stDisk.diskIndex == diskIndex)
		{
			if((stNode.bAction)&&
			    (stNode.stDisk.diskProperty == NET_DISK_READ_WRITE) &&
			    (stNode.stDisk.diskOwnedBy == NET_DISK_OWNED_BY_THIS))
			{
				if ((stNode.nLogNum>0) && (stNode.pLogInfo!=NULL))
				{
					for (int i=0; i<stNode.nLogNum; i++)
					{
						//只读图片不能删除。
						for (int j=0; j<stNode.nMaxPct; j++)
						{
							if (stNode.pLogInfo[i].pstPctInfo[j].bFill && 
								!stNode.pLogInfo[i].pstPctInfo[j].bLock)
							{
								stNode.pLogInfo[i].fileBase.nFill--;
								memset(&stNode.pLogInfo[i].pstPctInfo[j], 0, sizeof(PICTURE_INFO));
							}
						}
					}

					UpdateSnapLogFile(stNode.stDisk.diskIndex, stNode.pLogInfo, stNode.nLogNum, sizeof(PICTURE_MAN_INFO), stNode.nMaxPct);
				}
				else
				{
					break;
				}
			}
			else
			{
				break;
			}
		}
		m_stDiskList.GetNext(pos);
	}
	m_lock.UnLock();
	return true;
}

bool CSnapMan::DeleteOneDisk(unsigned char diskIndex)
{
	m_lock.Lock();

	POS pos = m_stDiskList.GetHeadPosition();

	while(NULL != pos)
	{
		SNAP_DISK_INFO &stNode = m_stDiskList.GetAt(pos);
		if (stNode.stDisk.diskIndex == diskIndex)
		{
			m_stDiskList.RemoveAt(pos);
		}
		m_stDiskList.GetNext(pos);
	}

	m_lock.UnLock();
}

bool CSnapMan::IsCanSavePicture()
{
	m_lock.Lock();
	bool bReturn = false;

	POS pos = m_stDiskList.GetHeadPosition();

	while(NULL != pos)
	{
		SNAP_DISK_INFO &stNode = m_stDiskList.GetAt(pos);
		if((stNode.bAction)&&
			(stNode.stDisk.diskProperty == NET_DISK_READ_WRITE) &&
			(stNode.stDisk.diskOwnedBy == NET_DISK_OWNED_BY_THIS))
		{
			bReturn =  true;
			break;
		}
		m_stDiskList.GetNext(pos);
	}
	m_lock.UnLock();
	return bReturn;
}
