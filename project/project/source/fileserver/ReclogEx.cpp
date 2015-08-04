/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : 袁石维
** Date         : 2008-12-11
** Name         : ReclogEx.cpp
** Version      : 1.0
** Description  :
** Modify Record:
***********************************************************************/
#include "ReclogEx.h"
#include "Product.h"
#include "MessageMan.h"
#include "StreamRecordManEx.h"

/**************************重大修改记录*********************************
//2010-02-27 09:17:00 YSW
问题描述：出现了文件的开始时间、结束时间；第一段的开始时间、最后一段的结束时间；第一个事件
		的开始时间、最后一个事件的结束时间都对应的情况（此时录像文件中的数据帧也和此区间对应）。
		但是中间的段和事件的记录都是上次录像（本次覆盖前）的数据。根据分析得出结论如下：
1、向前修改时间（时间要在对应文件的开始时间之前），这样重新加载时该文件会被过滤掉，放入了
无效文件列表。（但是reclog.bin中的数据未被改写）
2、开始录像，那么总会用到上述文件，此时写了一个开始（文件信息、段、事件都被修改），且其时间
和上次录像写入的肯定有差异。
3、异常重启，该文件需要修复。然后逐项读取reclog.bin中的数据进行分析，因为修改后（写了开始时间）
的数据并不是严格意义上的错误，所以会找到上次录像写的段和事件的最后一条进行修复（即修改结束时间）
。这样一来就出现描述中的情况。
解决办法：在写段和写事件时，如果是第一条记录（且是开始），则把对应的信息区域全部清零。
***********************************************************************/
//////////////////////////////////////////////////////////////////////////
unsigned long MinTime()
{
	tm time;
	memset(&time, 0, sizeof(tm));

	time.tm_year	= 2008 - 1900;
	time.tm_mon		= 1 - 1;
	time.tm_mday	= 1;

	return TmToDVRTime32(time);
}

unsigned long MaxTime()
{
	tm time;
	memset(&time, 0, sizeof(tm));

	time.tm_year	= 2025 - 1900;
	time.tm_mon		= 12 - 1;
	time.tm_mday	= 31;
	time.tm_hour	= 23;
	time.tm_min		= 59;
	time.tm_sec		= 59;

	return TmToDVRTime32(time);
}

unsigned long CReclogEx::m_minTime = MinTime();
unsigned long CReclogEx::m_maxTime = MaxTime();

unsigned char * CReclogEx::m_pCleanDataBuf = NULL;
/////////////////////////////////////////////////////////////////////////////////////////////
bool CReclogEx::CreateLogFile(const char *pFilePath, unsigned long fileNum, unsigned char videoInputNum,bool bIsBackUpCreated/* = false*/)
{
	assert (NULL != pFilePath);
	assert (fileNum > 0);
	assert (videoInputNum > 0);

	unsigned char netVideoInputNum = CProduct::Instance()->NetVideoInputNum();
	unsigned char localVideoInputNum = CProduct::Instance()->LocalVideoInputNum();
	if(!bIsBackUpCreated)
	{
		if(CStreamRecordManEx::Instance()->IsSupportMinorStreamRecord() && netVideoInputNum>0)
		{
			videoInputNum += netVideoInputNum;
			printf("CReclogEx::CreateLogFile-1:%u\n",videoInputNum);
		}
	}
	//备份录像数据时会在目的磁盘重新生成一个日志文件，
	//因为不需要备份小码流文件
	//所以日志文件的通道数目保持为大码流的通道数.
	else
	{
		videoInputNum = netVideoInputNum+localVideoInputNum;
	}

	if ((NULL == pFilePath) || (0 == fileNum) || (0 == videoInputNum))
	{
		return false;
	}

	FILE *pFile = fopen (pFilePath, "wb+");
	if (NULL == pFile)
	{
		return false;
	}

	//初始化头信息
	{
		RECLOG_FILE_HEAD_INFO headInfo;

		headInfo.headFlag		= 0;
		headInfo.version		= RECLOG_EX_FILE_VER;
		headInfo.isNormalClose	= 1;
		headInfo.chnnNum		= videoInputNum;
		headInfo.fileNum		= fileNum;
		headInfo.fileRecSecNum	= MAX_SECTION_NUM;
		headInfo.fileLogNum		= MAX_RECLOG_NUM;

		headInfo.fileInfoOffset	= FILE_INFO_OFFSET;
		headInfo.sectionOffset	= FILE_INFO_OFFSET + sizeof(FILE_FILE_INFO)*fileNum + 32;
		headInfo.reclogoffset	= headInfo.sectionOffset + sizeof(SECTION_INFO_EX)*MAX_SECTION_NUM*fileNum+ 32;

		if (1 != fwrite(&headInfo, sizeof(RECLOG_FILE_HEAD_INFO), 1, pFile))
		{
			Release (&pFile, 0);
			return false;
		}
	}

	//初始化文件信息

	{
		if (0 != fseek(pFile, FILE_INFO_OFFSET, SEEK_SET))
		{
			Release(&pFile, 0);
			return false;
		}

		FILE_FILE_INFO * pData = new FILE_FILE_INFO [fileNum];
		assert (NULL != pData);
		if (NULL == pData)
		{
			Release(&pFile, 0);
			return false;
		}

		memset (pData, 0, sizeof(FILE_FILE_INFO)*fileNum);
		if (fileNum != fwrite(pData, sizeof(FILE_FILE_INFO), fileNum, pFile))
		{
			Release(&pFile, 1, &pData);
			return false;
		}

		delete [] pData;
		pData = NULL;
	}

	//初始化文件段信息
	{
		if (0 != fseek(pFile, FILE_INFO_OFFSET + sizeof(FILE_FILE_INFO)*fileNum + 32, SEEK_SET))
		{
			Release(&pFile, 0);
			return false;
		}

		unsigned long num = MAX_BUFFER_LEN/sizeof(SECTION_INFO_EX);
		SECTION_INFO_EX *pData = new SECTION_INFO_EX [num];
		assert (NULL != pData);
		if (NULL == pData)
		{
			Release(&pFile, 0);
			return false;
		}
		memset (pData, 0, sizeof (SECTION_INFO_EX)*num);

		unsigned long loopNum = (MAX_SECTION_NUM * fileNum) / num;
		for (unsigned long i=0; i<loopNum; ++i)
		{
			if (num != fwrite(pData, sizeof(SECTION_INFO_EX), num, pFile))
			{
				Release(&pFile, 1, &pData);
				return false;
			}
		}

		num = (MAX_SECTION_NUM * fileNum) % num;
		if (num > 0)
		{
			if (num != fwrite(pData, sizeof(SECTION_INFO_EX), num, pFile))
			{
				Release(&pFile, 1, &pData);
				return false;
			}
		}

		delete [] pData;
		pData = NULL;
	}

	//初始化文件事件段

	{
		if (0 != fseek(pFile, FILE_INFO_OFFSET + sizeof(FILE_FILE_INFO)*fileNum + 32 + \
			sizeof(SECTION_INFO_EX)*MAX_SECTION_NUM*fileNum+ 32, SEEK_SET))
		{
			Release(&pFile, 0);
			return false;
		}

		unsigned long num = MAX_BUFFER_LEN/sizeof(FILE_RECLOG_INFO);
		FILE_RECLOG_INFO *pData = new FILE_RECLOG_INFO [num];
		assert (NULL != pData);
		if (NULL == pData)
		{
			Release(&pFile, 0);
			return false;
		}
		memset (pData, 0, sizeof (FILE_RECLOG_INFO)*num);

		unsigned long loopNum = (MAX_RECLOG_NUM * fileNum) / num;
		for (unsigned long i=0; i<loopNum; ++i)
		{
			if (num != fwrite(pData, sizeof(FILE_RECLOG_INFO), num, pFile))
			{
				Release(&pFile, 1, &pData);
				return false;
			}
		}

		num = (MAX_RECLOG_NUM * fileNum) % num;
		if (num != fwrite(pData, sizeof(FILE_RECLOG_INFO), num, pFile))
		{
			Release(&pFile, 1, &pData);
			return false;
		}

		delete [] pData;
		pData = NULL;
	}

	Release(&pFile, 0);
	return true;
}
/////////////////////////////////////////////////////////////////////////////////////////////
CReclogEx::CReclogEx ():m_diskIndex(0), m_pFile(NULL), m_pFilePath(NULL), \
		m_pFileInfoList(NULL), m_pRecStatus(NULL), m_pDateInfoList(NULL), m_pChannelRecordFile(NULL), \
		m_pChannelReadFile(NULL)
{
	memset (&m_headInfo, 0, sizeof(RECLOG_FILE_HEAD_INFO));
	m_pFilePathForBackup = NULL;
	m_pFileForBackup = NULL;	

}

CReclogEx::~CReclogEx ()
{

}

/************************************************************************
功能描述：初始化一个磁盘分区的数据库管理对象。
输入参数：	diskIndex--磁盘分区的index
			pFilePath--磁盘相对路径名
返回值：	成功或失败
备注：
		1、基本流程：检查路径名和磁盘序号的合法性--尝试打开数据库文件--读取
		文件头信息（检查数据正确性）--加载文件的信息（根据文件信息建立未使用
		文件列表、已使用文件列表（以通道为单位成列）、文件和通道的对应关系）
		--初始化录像的状态。
		2、考虑到兼容性的问题，初始化时应该带入设备标识，用于处理兼容性。
************************************************************************/
bool CReclogEx::Initial (unsigned char diskIndex, const char *pFilePath)
{
	//处理路径和磁盘数目
	{
		assert (NULL == m_pFilePath);	//避免重复初始化

		m_pFilePath = new char [512];
		assert (NULL != m_pFilePath);
		if (NULL == m_pFilePath)
		{
			return false;
		}

		memset(m_pFilePath, 0, 512);
		if (NULL != pFilePath)
		{
			strcpy(m_pFilePath, pFilePath);
		}

		assert (diskIndex < 32);
		if (diskIndex >= 32)	//目前看最多是8个硬盘，所以定位32个分区。

		{
			return false;
		}

		m_diskIndex = diskIndex;		
	}

	//处理文件完整路径，并尝试打开文件。
	{
		char filePath [512] = {0};
#ifdef __ENVIRONMENT_WIN32__
#ifdef __ENVIRONMENT_VIDEO_PLAY__
		if (1 < strlen(m_pFilePath))
		{
			sprintf(filePath, "%s\\reclog.bin", m_pFilePath);
		}
		else
		{
			assert (1 == strlen(m_pFilePath));
			sprintf(filePath, "%c:\\reclog.bin", m_pFilePath[0]+m_diskIndex);
		}
#else

		if (1 < strlen(m_pFilePath))
		{
			sprintf(filePath, "%s\\%02d\\reclog.bin", m_pFilePath, m_diskIndex);
		}
		else
		{
			assert (1 == strlen(m_pFilePath));
			sprintf(filePath, "%c:\\reclog.bin", m_pFilePath[0]+m_diskIndex);
		}
#endif
#else
		sprintf(filePath, "%s/%02d/reclog.bin", m_pFilePath, m_diskIndex);
#endif
		assert (NULL == m_pFile);
		if (NULL != m_pFile)
		{
			fclose(m_pFile);
		}

		m_pFile = fopen(filePath, "rb+");
		if (NULL == m_pFile)
		{
#ifdef __ENVIRONMENT_VIDEO_PLAY__
			//[2010-04-01 L]光驱备份的文件不能以"rb+"打开
			m_pFile = fopen(filePath, "rb");

			if(NULL == m_pFile)
			{
				Release(NULL, 1, &m_pFilePath);
				return false;
			}
#else
			Release(NULL, 1, &m_pFilePath);
			return false;
#endif
		}
	}

	//读取头信息
	{
		if (1 != fread(&m_headInfo, sizeof(RECLOG_FILE_HEAD_INFO), 1, m_pFile))
		{
			Release(&m_pFile, 1, &m_pFilePath);
			return false;
		}

		//说明文件出错了，不允许文件和通道数目为零的情况

		if ((0 >= m_headInfo.fileNum) || (0 >= m_headInfo.chnnNum))
		{
			Release(&m_pFile, 1, &m_pFilePath);
			return false;
		}	
	}

	//加载文件信息
	{
		FILE_FILE_INFO *pData = new FILE_FILE_INFO [m_headInfo.fileNum];
		assert (NULL != pData);
		if (NULL == pData)
		{
			Release(&m_pFile, 1, &m_pFilePath);
			return false;
		}

		if (m_headInfo.fileNum != ReadFileInfo(pData, sizeof(FILE_FILE_INFO)*m_headInfo.fileNum, m_headInfo.fileNum, 0))
		{
			Release(&m_pFile, 2, &m_pFilePath, &pData);
			return false;
		}

		//根据通道建立列表数组，每个通道单独成列。

		m_pFileInfoList = new FILE_INFO_EX_LIST [m_headInfo.chnnNum];
		assert (NULL != m_pFileInfoList);
		if (NULL == m_pFileInfoList)
		{
			Release(&m_pFile, 2, &m_pFilePath, &pData);
			return false;
		}
		
		//录像状态
		m_pRecStatus = new REC_STATUS [m_headInfo.chnnNum];
		assert (NULL != m_pRecStatus);
		if (NULL == m_pRecStatus)
		{
			Release(&m_pFile, 2, &m_pFilePath, &pData);
			return false;
		}
		memset(m_pRecStatus, 0, sizeof(REC_STATUS)*m_headInfo.chnnNum);
		for (int i = 0; i < m_headInfo.chnnNum; ++i)
		{
			m_pRecStatus[i].remainReclogNum = m_headInfo.fileLogNum;
		}

		unsigned long currTime = GetCurrTime32();
		FILE_INFO_EX fileInfo;
		//便历文件信息，根据其使用与否的情况，分别加入不同的列表，并建立和通道的对应关系。

		for (unsigned short i=0; i<m_headInfo.fileNum; ++i)
		{
			//2010-07-08 16:11:00 YSW
			//把修改时间产生的重叠文件不再加入无效列表，而是做好标记加入有效列表
			//所以下面的第四项去掉。
			//2010-05-14 14:37:00 YSW
			//要对数据进行一个全面的判定，发现异常的数据则应该放入无效列表。
			//一是确实没有用于录像的文件(0 == pData[i].type)
			//二是通道编号不正常，超出设计要求(MAX_CHNN_NUM < CHANNEL_ID(pData[i].chnn))
			//三是文件的开始时间居然小于系统指定的最小时间(pData[i].startTime < m_minTime)
			//四是文件的开始时间居然大于系统当前时间(currTime <= pData[i].startTime)
			//修改时间后可能存在这种情况，所以把这样的文件归入无效队列，只要不被覆盖都不会被删除。
			//五是文件的结束时间居然大于当前系统允许的最大时间(m_maxTime < pData[i].endTime)，
			//六是文件的开始时间大于或者等于结束时间(pData[i].endTime <= pData[i].startTime)
			//快速修改时间可能存在着这样的情况，所以归入无效队列。
			if ((0 == RECORD_TYPE_IS_NORMAL(pData[i].type)) ||\
				(MAX_CHNN_NUM < CHANNEL_ID(pData[i].chnn)) ||\
				(pData[i].startTime < m_minTime) ||\
				(pData[i].status && (m_maxTime < pData[i].endTime)) ||\
				((pData[i].status > 0) && (pData[i].status != FILE_STATUS_WRITING) && (pData[i].endTime <= pData[i].startTime)) ||\
				((pData[i].status == FILE_STATUS_WRITING) && (pData[i].endTime != 0) && (pData[i].endTime <= pData[i].startTime)))
			{
				m_invalidFileList.AddTail(i);
			}
			else
			{
				fileInfo.type		= pData[i].type;
				fileInfo.chnn		= pData[i].chnn;
				fileInfo.startTime	= pData[i].startTime;
				fileInfo.endTime	= pData[i].endTime;
				fileInfo.diskIndex	= m_diskIndex;
				fileInfo.fileIndex	= i;

				//2010-07-08 16:11:00 YSW
				//某个文件的时间区间包含了当前文件，说明需要加入非正式标记；
				//或者是已经有非正式标记的文件，也需要增加。
				{
					if ((FILE_STATUS_UNOFFICIAL & pData[i].status) ||\
						((currTime <= pData[i].startTime) || (currTime < pData[i].endTime)))
					{
						RECORD_FILE_EN_UNOFFICAL(fileInfo.type);
					}

					//修改文件标记
					if ((currTime <= pData[i].startTime) || (currTime < pData[i].endTime))
					{
						pData[i].status |= FILE_STATUS_UNOFFICIAL;
						WriteFileInfo(i, &pData[i]);
					}
				}

				AddToFileListForInitial(fileInfo, m_pFileInfoList[CHANNEL_ID(pData[i].chnn)], true);

				if (FILE_STATUS_WRITING == pData[i].status)
				{
					m_unFinishedFileList.AddTail(fileInfo);
				}
			}

			//
		}

		delete [] pData;
		pData = NULL;
	}	

	assert(NULL == m_pChannelRecordFile);
	m_pChannelRecordFile = new CHANNEL_USE_FILE_INFO [m_headInfo.chnnNum];	
	for (int i = 0; i < m_headInfo.chnnNum; ++i)
	{
		m_pChannelRecordFile[i].channel = i;
		m_pChannelRecordFile[i].pFileList = new CMyList<unsigned short>;
	}

	assert(NULL == m_pChannelReadFile);
	m_pChannelReadFile = new CHANNEL_USE_FILE_INFO [m_headInfo.chnnNum];
	for (int i = 0; i < m_headInfo.chnnNum; ++i)
	{
		m_pChannelReadFile[i].channel = i;
		m_pChannelReadFile[i].pFileList = new CMyList <unsigned short>;
	}
	
	if (NULL != m_pCleanDataBuf)
	{
		delete [] m_pCleanDataBuf;
		m_pCleanDataBuf = NULL;
	}

	return true;
}

void CReclogEx::Quit()
{
	if (NULL != m_pRecStatus)
	{
		delete [] m_pRecStatus;
		m_pRecStatus = NULL;
	}

	if (NULL != m_pFileInfoList)
	{
		for (unsigned char i=0; i<m_headInfo.chnnNum; ++i)
		{
			m_pFileInfoList[i].RemoveAll();
		}

		delete [] m_pFileInfoList;
		m_pFileInfoList = NULL;
	}

	m_invalidFileList.RemoveAll();

	{
		if (NULL != m_pDateInfoList)
		{
			for (unsigned char chnn=0; chnn<m_headInfo.chnnNum; ++chnn)
			{
				POS pos = m_pDateInfoList[chnn].GetHeadPosition();
				while (NULL != pos)
				{
					DATE_INFO & node = m_pDateInfoList[chnn].GetNext(pos);
					if (NULL != node.pFileList)
					{
						node.pFileList->RemoveAll();
						delete node.pFileList;
						node.pFileList = NULL;
					}
				}

				m_pDateInfoList[chnn].RemoveAll();
			}

			delete [] m_pDateInfoList;
			m_pDateInfoList = NULL;
		}
	}

	if (NULL != m_pFilePath)
	{
		delete [] m_pFilePath;
		m_pFilePath = NULL;
	}

	Release(&m_pFile, 1, NULL);

	if (NULL != m_pChannelRecordFile)
	{
		for (int i = 0; i < m_headInfo.chnnNum; ++i)
		{
			//退出之前要把打开的录像文件关闭掉
			assert(0 == m_pChannelRecordFile[i].pFileList->GetCount());
			delete m_pChannelRecordFile[i].pFileList;
			m_pChannelRecordFile[i].pFileList = NULL;
		}
		delete [] m_pChannelRecordFile;
		m_pChannelRecordFile = NULL;
	}

	if (NULL != m_pChannelReadFile)
	{
		for (int i = 0; i < m_headInfo.chnnNum; ++i)
		{
			//退出之前要把读取的文件关闭掉
			assert(0 == m_pChannelReadFile[i].pFileList->GetCount());
			delete m_pChannelReadFile[i].pFileList;
			m_pChannelReadFile[i].pFileList = NULL;
		}
		delete [] m_pChannelReadFile;
		m_pChannelReadFile = NULL;
	}


	if (NULL != m_pCleanDataBuf)
	{
		delete [] m_pCleanDataBuf;
		m_pCleanDataBuf = NULL;
	}


	if (NULL != m_pFilePathForBackup)
	{
		delete [] m_pFilePathForBackup;
		m_pFilePathForBackup = NULL;
	}
}
/************************************************************************
功能描述：获取一个未使用文件的索引号。
输入参数：chnn--外部将要使用该文件的通道号。
输出参数：fileIndex--用于记录文件索引号。
返回成功或失败。
备注：	1、只要有未用文件存在，则将其从未用文件列表中弹出，并加入和通道相对的
		已用文件列表中，同时更新文件和通道对应关系图。
		2、这样处理的目的为了在写入记录时切换文件后可以方便地处理。
		3、如果该文件被获取出去，却因为某系原因没有真正使用，在系统重启后其将
		重新被加入未用文件列表（因为磁盘对应的信息并没有被修改，只是修改了内存里
		建立的信息）。
************************************************************************/
bool CReclogEx::GetInvalidFile (unsigned long chnn, unsigned short &fileIndex)
{
	if (CHANNEL_ID(chnn) < m_headInfo.chnnNum)
	{
		if (m_invalidFileList.GetCount() > 0)
		{
			//获得文件的index
			fileIndex = m_invalidFileList.RemoveHead();

			//获得后即时加入和通道对应的文件使用列表，并建立对应关系
#if 0
			//只对应关系，不再加入文件列表，因为列表中存在一个没有对应录像的文件，总是
			//造成误操作。改为在写录像开始时，判断列表尾部的文件序号，如果相等则不是新
			//文件，可以采用更新的办法。如果不相等，则需要加入新的节点。
			FILE_INFO_EX fileInfo;
			memset(&fileInfo, 0, sizeof(FILE_INFO_EX));
			fileInfo.fileIndex = fileIndex;
			fileInfo.chnn = chnn;

			m_pFileInfoList[CHANNEL_ID(chnn)].AddTail(fileInfo);
#endif

			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}

/************************************************************************
功能描述：
输入参数：	
输出参数：
备注：	
************************************************************************/
bool CReclogEx::FindRecycleFile(FILE_INFO_EX &fileInfo)
{
	assert(m_headInfo.chnnNum > 0);

	bool bSucc = false;
	unsigned long maxTime = 0x7fffffff;	
	for (unsigned short chnnTmp = 0; chnnTmp < m_headInfo.chnnNum; ++chnnTmp)
	{
		if (m_pFileInfoList[chnnTmp].IsEmpty())
		{
			continue;
		}

		//m_pFileInfoList[chnnTmp]里的文件按时间递增的。
		//找到第一个没有锁定,没有正在读取并且没有正在录像的最早的文件
		POS pos = m_pFileInfoList[chnnTmp].GetHeadPosition();
		while (NULL != pos)
		{
			FILE_INFO_EX &node = m_pFileInfoList[chnnTmp].GetAt(pos);
			
			assert(node.chnn == chnnTmp);
			if (RECORD_FILE_IS_LOCK(node.type))
			{
				m_pFileInfoList[chnnTmp].GetNext(pos);
				continue;
			}
			if (IsFileBeUsingForRead(node.chnn, node.fileIndex))
			{
				m_pFileInfoList[chnnTmp].GetNext(pos);
				continue;
			}
			if (IsFileBeUsingForRecord(node.chnn, node.fileIndex))
			{
				m_pFileInfoList[chnnTmp].GetNext(pos);
				continue;
			}

			assert(node.startTime > 0);	
			if (node.startTime < maxTime)
			{
				fileInfo = node;
				maxTime = node.startTime;
				bSucc = true;
			}
			break;
		}		
		
	}

	return bSucc;
}

/************************************************************************
功能描述：获取录像信息，返回录像段信息。以通道为单位返回段信息列表。
输入参数：	chnnBits--搜索的通道模板（位代表一个通道）
			startTime--搜索的开始时间（精度到秒）
			endTime--搜索的结束时间（精度到秒）
			chnnNum--输出列表的元素数目，理论上为系统给定的视频通道数目。
输出参数：	pList--指向一个列表数组，每维用于返回一个通道的录像段信息。
返回段的数目。

备注：	因为本类设计用于可以兼容各种不同通道数目的产品使用的数据库文件，所以
		就有出现外部搜索条件和数据库记录的通道数目不一致的情况，因而在给定搜索
		条件时，增加一个chnnNum参数，用于指明外部要查询的通道数目。而外部在使用
		结果时，需要根据pList指向的数组下标来确定和通道的对应的关系。

		不能对pList做任何删除动作（因为其可能已经加入其他磁盘的信息），在往列表里
		加入新的节点时，考虑列表为非空的情况，并做好排序处理。
		************************************************************************/
unsigned long CReclogEx::GetSectionInfo(ULONGLONG chnnbBits, unsigned long startTime, unsigned long endTime, SECTION_INFO_EX_LIST *pList, unsigned char chnnNum, bool bRepeat, bool bOfficial)
{
	assert (0 != chnnbBits);
	assert (endTime > startTime);
	assert (NULL != m_pFile);
	assert (NULL != m_pFileInfoList);
	if ((NULL == m_pFile) || (NULL == m_pFileInfoList))
	{
		return 0;
	}
	/*
	POS pos = m_pFileInfoList->GetHeadPosition();
	while(pos)
	{
		FILE_INFO_EX &node = m_pFileInfoList->GetAt(pos);
		printf("%s,%d,%d,%ld,%ld\n",__FILE__,__LINE__,node.fileIndex,node.startTime,node.endTime);
		//start time
		struct tm *ptmStartTime=NULL;
		char buf[128] = {0};
		LONGLONG startTime =node.startTime;
		if ( NULL != (ptmStartTime = gmtime((time_t *)&startTime)) )
		{
			strftime(buf,sizeof(buf),"%Y-%m-%d %H:%M:%S",ptmStartTime);
			printf("startTime :    %s\n",buf);
		}
		//end time
		struct tm *ptmEndTime=NULL;
		char bufEnd[128] = {0};
		LONGLONG endTime =node.endTime;
		if ( NULL != (ptmEndTime = gmtime((time_t *)&endTime)) )
		{
			strftime(bufEnd,sizeof(bufEnd),"%Y-%m-%d %H:%M:%S",ptmEndTime);
			printf("endTime :    %s\n",bufEnd);
		}
		m_pFileInfoList->GetNext(pos);
	}
	*/
	assert (NULL != pList);
	assert (chnnNum > 0);
	if ((NULL == pList) || (0 == chnnNum))
	{
		return 0;
	}

	//说明最大通道的编号超出了chnnNum
	if (chnnbBits & ~ALL_CHNN_MASK(chnnNum))
	{
		chnnbBits = chnnbBits & ALL_CHNN_MASK(chnnNum);	//进行修正
	}

	//首先根据条件搜索到对应的文件，结果放在fileList列表中，列表必须以文件序号为序。
	FILE_INFO_EX_LIST fileList;
	if (0 == GetFileList(chnnbBits, startTime, endTime, fileList))		//没有满足条件的文件存在，直接返回
	{
		return 0;
	}
	else	//有满足条件的文件存在
	{
		//开启缓冲区，考虑到对内存的使用，定义了一个最大缓冲区（降低内存使用高峰值），
		//这样一来就需要做多段处理，因为一个缓冲区不能完全满足使用要求，所以需要分批次处理。
		unsigned long num = MAX_BUFFER_LEN/(sizeof(SECTION_INFO_EX) * m_headInfo.fileRecSecNum);
		SECTION_INFO_EX *pData = new SECTION_INFO_EX [num*m_headInfo.fileRecSecNum];
		assert (NULL != pData);
		if (NULL == pData)
		{
			return 0;
		}

		/*
		1、一次读取一批文件的段记录信息，然后依据已经获得的文件列表来寻找对应的段记录。
		2、最外层的循环是遍历满足条件的文件列表，每次针对一个文件寻找对应的段信息。
		3、由于文件列表和段记录信息都是按照文件序号的升序排列，所以可以每次在针对某个文件
		进行加载段信息时，从前次结束的段记录开始往后扫描，不需要每次都从缓冲区的开始扫描。
		4、鉴于3描述的原因，把第二层循环的下标i定义在循环外部，用于记录上次下标。只有在重新
		读取一批段记录时和retNum同时更新（i = 0）。
		*/
		unsigned long currTime = GetCurrTime32() - MAX_TIME_OFFSET;

		unsigned short headIndex = 0, tailIndex = 0;
		unsigned long retNum = 0, getNum = 0;
		headIndex = tailIndex = fileList.GetHead().fileIndex;	//此时headIndex == tailIndex是为了第一次进入读取段记录路径。
		POS pos = fileList.GetHeadPosition();
		SECTION_INFO_EX *pSection = NULL;
		unsigned long chnn = 0, i = 0;
		SECTION_INFO_EX	section;

		while (NULL != pos)
		{
			FILE_INFO_EX &node = fileList.GetAt(pos);

			//2010-07-08 09:20:00 YSW
			//过滤掉非正式文件
			if (bOfficial && RECORD_FILE_IS_UNOFFICAL(node.type))
			{
				fileList.GetNext(pos);
				continue;
			}

			//当前文件的序号已经超出了缓冲区里数据对应的文件序号最大值，则说明需要重新读取下一段数据了。
			if (tailIndex <= node.fileIndex)
			{
				//headIndex始终指向缓冲区对应的第一个文件序号
				headIndex = node.fileIndex;
				retNum = ReadSection(pData, sizeof(SECTION_INFO_EX) * m_headInfo.fileRecSecNum * num, num, headIndex);
				if (0 == retNum)
				{
					//获取失败，说明异常
					assert (false);
					delete [] pData;
					return getNum;
				}

				i = 0;	//重新读取了一批段记录，需要更新i。
				tailIndex += (unsigned short)retNum;//tailIndex指向缓冲区对应的最后一个文件序号
			}

			for (; i<retNum; ++i)
			{
				//如果文件序号想对应，则进行加载段记录。
				if ((headIndex + i) == node.fileIndex)
				{
					pSection = pData + (i * m_headInfo.fileRecSecNum);

					chnn = node.chnn;	//获得通道号

					//此时应该对应起来，否则说明前面搜索文件出错，或者是建立通道和文件关系出错。
					assert(chnnbBits & ((ULONGLONG)0x01 << chnn));	

					//实际上可以不要判断了，但是为了防止意外还是加上。					
					if (chnnbBits & ((ULONGLONG)0x01 << chnn))
					{
						unsigned long oldNum = pList[chnn].GetCount();
						for (unsigned long j=0; j<m_headInfo.fileRecSecNum; ++j)
						{
							//2010-01-24 12:00:00 YSW
							//修改前当发现未写完整的事件时，把当前时间作为其结束进行补足。
							//当时存在可能某个事件段在写结束时，其结束时间和写入时刻有差异（例如事件结束时间为12:00:54，但是这个写入
							//行为却发生在12:01:00，加入在（12:00:54，12:01:00）时间段内进入本接口进行搜索，那么其结束时间是超出
							//12:00:54的，即是不精确的，所以采用限定时间的办法来避免这种误差。

							//如果0 == pData[i]->endTime，且0 < pSection[j].startTime则说明该段信息没有写完整。
							//那么就需要检查其开始时间是否在限定时间之前，如果不是则该段需要给过滤掉。
							if ((0 == pSection[j].endTime) && (0 < pSection[j].startTime))
							{
								//2010-05-21 13:58:00 YSW
								//如果该文件的开始时间和结束时间正常，则使用文件的结束时间来补足。
								if (node.startTime <= node.endTime)
								{
									pSection[j].endTime = node.endTime;	
								}
								else
								{
									//条件为：(pSection[j].startTime + MAX_TIME_OFFSET) < GetCurrTime32()
									//满足条件表示该段不能被过滤，且需要填补结束时间
									if (pSection[j].startTime < currTime)
									{
										//因为在启动时，对异常文件做了修复，所以该种情况就是该段尚未写完，所以补足结束时间。
										pSection[j].endTime = currTime;//结束时间应该在限定时间之前。
									}
									else
									{
										//要过滤掉
										continue;
									}
								}
							}

							if ((currTime + MAX_TIME_OFFSET) < pSection[j].endTime)
							{
								pSection[j].endTime = (currTime + MAX_TIME_OFFSET);
							}

							//2010-01-24 12:00:00 YSW，用上面一段使用限定时间来代替了。
							////如果0 == pData[i]->endTime，且0 < pSection[j].startTime则说明该段信息没有写完整。
							//if ((0 == pSection[j].endTime) && (0 < pSection[j].startTime))
							//{
							//	//因为在启动时，对异常文件做了修复，所以该种情况就是该段尚未写完，所以补足结束时间。
							//	pSection[j].endTime = GetCurrTime32();
							//}

							//2010-02-27 09:29:00 YSW
							//检查时间逻辑合理性，比如开始时间要小于或者等于结束时间(注意：此时结束时间已经得到修正）
							if (pSection[j].endTime < pSection[j].startTime)
							{
								continue;
							}

							//满足条件，采用排除法鉴定。
							if (!((endTime <= pSection[j].startTime) || (pSection[j].endTime <= startTime)))
							{
								section.startTime	= pSection[j].startTime;
								section.endTime		= pSection[j].endTime;
                                
								AddToSectionList(section, pList[chnn], bRepeat);
								//++ getNum;
							}

							//说明当前文件的所有的段记录都已经遍历完成，后面的都是些无效段信息了，可以跳出结束了。
							//一种是全等于零，表示该记录空间从来未被使用过，一种是开始时间小于文件的开始时间，表示
							//该空间以前被使用过，但是文件被覆盖了，段没有以前多，又未用到该空间。
							if (((0 == pSection[j].startTime) && (0 == pSection[j].endTime)) || (node.startTime > pSection[j].startTime))
							{
								break;
							}
						}

						assert(pList[chnn].GetCount() >= oldNum);
						getNum += pList[chnn].GetCount() - oldNum;
					}

					//跳出循环，匹配下一个文件
					break;
				}
			}

			fileList.GetNext(pos);
		}

		delete [] pData;
		return getNum;
	}
}

/************************************************************************
功能描述：获取文件列表，并以通道+时间排序的方式加入返回列表。
输入参数：	chnnBits--搜索通道模板
			startTime--搜索的开始时间
			endTime--搜索的结束时间
输出参数：	list--记录文件信息的列表
返回满足条件的文件数目。
备注：
		不能对list做任何删除动作（因为其可能已经加入其他磁盘的信息），在往列表里
		加入新的节点时，考虑列表为非空的情况，并做好排序处理。
************************************************************************/
unsigned long CReclogEx::GetFileInfo(unsigned long chnn, unsigned long startTime, unsigned long endTime, \
									 FILE_INFO_EX_LIST & list, bool bOnlyRead, bool bRepeat, bool bOfficial)
{
	chnn = CHANNEL_ID(chnn);
	assert (chnn < m_headInfo.chnnNum);
	assert (endTime > startTime);
	assert (NULL != m_pFile);
	assert (NULL != m_pFileInfoList);
	assert (NULL != m_pChannelRecordFile);
	assert (chnn == m_pChannelRecordFile[chnn].channel);
	assert (NULL != m_pChannelRecordFile[chnn].pFileList);
	if ((NULL == m_pFile) || (NULL == m_pFileInfoList))
	{
		return 0;
	} 

	bool bOpenRecordFile = false;
	unsigned short curRecordFileIndex = -1;
	if (m_pChannelRecordFile[chnn].pFileList->GetCount() > 0)
	{
		bOpenRecordFile = true;
		assert(1 == m_pChannelRecordFile[chnn].pFileList->GetCount());
		assert(!bOnlyRead); //既然这个分区可以录像，就不可能是只读属性
		curRecordFileIndex = m_pChannelRecordFile[chnn].pFileList->GetHead();
	}
   
	unsigned long oldNum = list.GetCount();
	unsigned long num = 0;

	FILE_INFO_EX node;
	POS pos = GetNodePos(m_pFileInfoList[chnn], startTime);//根据开始时间获得最前面的文件对应的节点。
	while (NULL != pos)
	{
		node = m_pFileInfoList[chnn].GetNext(pos);

#if 0
		//不再采用一拿到文件就加入文件列表的做法，所以以下条件将不满足。20090916-YSW
		//链表中的最后一个节点，有可能是：录像状态为NULL。
		//如：一启动会预先打开一些文件，但是一直没有录像，则这些文件的录像状态就是NULL
		if ((NULL == pos) && (RECORD_TYPE_NULL == node.type))
		{
			assert(node.startTime == 0);
			assert(node.endTime   == 0);
			continue;
		}
#endif

		//过滤掉“非正式文件”
		if (bOfficial && RECORD_FILE_IS_UNOFFICAL(node.type))
		{
			continue;
		}

		//列表最后一个，且其录像状态不为空，则说明该文件为正在录像的文件，需要修正结束时间。
		//if ((NULL == pos) && (RECORD_TYPE_NULL != m_pRecStatus[chnn].type))
		//由于可以兼容随意修改时间而不删除数据，所以排列表队尾的不一定是正在录像的文件，所以判断条件需要修改。
		if (node.fileIndex == curRecordFileIndex)
		{
			//2010-01-24 15:05:00 YSW
			//过滤掉限定时间内部的文件
			unsigned long currTime = GetCurrTime32() - MAX_TIME_OFFSET;

			if (currTime <= node.startTime)
			{
				continue;
			}

			if (RECORD_TYPE_NULL != m_pRecStatus[chnn].type)
			{
				node.endTime = currTime;//在限定时间点
			}
			else
			{
				if(node.endTime < node.startTime)
				{
					assert(false);
					node.endTime = currTime;//在限定时间点
				}
			}
			
			assert(bOpenRecordFile);
			assert(node.fileIndex == curRecordFileIndex);			
			assert(node.endTime > node.startTime);

		}
       
		if (!((node.endTime <= startTime) || (endTime <= node.startTime)))
		{
			if (bOpenRecordFile && (curRecordFileIndex == node.fileIndex))
			{
				RECORD_FILE_EN_WRITING(node.type);
			}
			if (IsFileBeUsingForRead(node.chnn, node.fileIndex))
			{
				RECORD_FILE_EN_READING(node.type);
			}

			AddToFileListForSearch(node, list, bOnlyRead, bRepeat);
			//++ num;
		}
		else if (endTime <= node.startTime)
		{
			//由于已用文件是以通道成列的，且在列表中已开始时间排序，所以当当前节点的开始时间大于或者等于搜索
			//条件的结束时间，说明后续节点的开始时间都要大于该结束时间（即都不满足要求）。所以可以退出遍历。

			break;
		}
	}
    
	assert(list.GetCount() >= oldNum);
	num = list.GetCount() - oldNum;
	return num;
}

/************************************************************************
功能描述：获取录像文件列表，以通道为单位返回列表。
输入参数：	chnnBits--搜索的通道模板（位代表一个通道）
			startTime--搜索的开始时间（精度到秒）
			endTime--搜索的结束时间（精度到秒）
			chnnNum--输出列表的元素数目，理论上为系统给定的视频通道数目。
输出参数：	pList--指向一个列表数组，每维用于返回一个通道的录像文件。
返回文件的数目。

备注：	因为本类设计用于可以兼容各种不同通道数目的产品使用的数据库文件，所以
		就有出现外部搜索条件和数据库记录的通道数目不一致的情况，因而在给定搜索
		条件时，增加一个chnnNum参数，用于指明外部要查询的通道数目。而外部在使用
		结果时，需要根据pList指向的数组下标来确定和通道的对应的关系。

		不能对pList做任何删除动作（因为其可能已经加入其他磁盘的信息），在往列表里
		加入新的节点时，考虑列表为非空的情况，并做好排序处理。
************************************************************************/
unsigned long CReclogEx::GetFileInfo(ULONGLONG chnnbBits, unsigned long startTime, unsigned long endTime, \
						  FILE_INFO_EX_LIST *pList, unsigned char chnnNum, bool bOnlyRead, bool bRepeat, bool bOfficial)
{
	assert (0 != chnnbBits);
	assert (endTime > startTime);
	assert (NULL != m_pFile);
	assert (NULL != m_pFileInfoList);
	assert (NULL != m_pChannelRecordFile);	

	if ((NULL == m_pFile) || (NULL == m_pFileInfoList))
	{
		return 0;
	}

	assert (NULL != pList);
	assert (chnnNum > 0);
	if ((NULL == pList) || (0 == chnnNum))
	{
		return 0;
	}

	//说明最大通道的编号超出了chnnNum
	if (chnnbBits & ~ALL_CHNN_MASK(chnnNum))
	{
		chnnbBits = chnnbBits & ALL_CHNN_MASK(chnnNum);	//进行修正
	}

	//取chnnNum和m_headInfo.chnnNum的最小值用于遍历通道
	chnnNum = (m_headInfo.chnnNum > chnnNum) ? chnnNum : m_headInfo.chnnNum;


	unsigned long num = 0;
	for (unsigned char chnn=0; chnn<chnnNum; ++chnn)
	{
		if (chnnbBits & ((ULONGLONG)0x01 << chnn))
		{
			assert (chnn == m_pChannelRecordFile[chnn].channel);
			assert (NULL != m_pChannelRecordFile[chnn].pFileList);
			bool bOpenRecordFile = false;
			unsigned short curRecordFileIndex = -1;
			if (m_pChannelRecordFile[chnn].pFileList->GetCount() > 0)
			{
				bOpenRecordFile = true;
				assert(1 == m_pChannelRecordFile[chnn].pFileList->GetCount());
				assert(!bOnlyRead); //既然这个分区可以录像，就不可能是只读属性
				curRecordFileIndex = m_pChannelRecordFile[chnn].pFileList->GetHead();
			}

			unsigned long oldNum = pList[chnn].GetCount();
			FILE_INFO_EX node;
			POS pos = GetNodePos(m_pFileInfoList[chnn], startTime);//根据开始时间获得最前面的文件对应的节点。
			while (NULL != pos)
			{
				node = m_pFileInfoList[chnn].GetNext(pos);

#if 0
				//不再采用一拿到文件就加入文件列表的做法，所以以下条件将不满足。20090916-YSW
				//链表中的最后一个节点，有可能是：录像状态为NULL。
				//如：一启动会预先打开一些文件，但是一直没有录像，则这些文件的录像状态就是NULL
				if ((NULL == pos) && (RECORD_TYPE_NULL == node.type))
				{
					assert(node.startTime == 0);
					assert(node.endTime   == 0);
					continue;
				}
#endif
				//过滤掉“非正式文件”
				if (bOfficial && RECORD_FILE_IS_UNOFFICAL(node.type))
				{
					continue;
				}

				//列表最后一个，且其录像状态不为空，则说明该文件为正在录像的文件，需要修正结束时间。
				//if ((NULL == pos) && (RECORD_TYPE_NULL != m_pRecStatus[chnn].type))
				//由于可以兼容随意修改时间而不删除数据，所以排列表队尾的不一定是正在录像的文件，所以判断条件需要修改。
				if (node.fileIndex == curRecordFileIndex)
				{
					//2010-01-24 15:05:00 YSW
					//过滤掉限定时间内部的文件
					unsigned long currTime = GetCurrTime32() - MAX_TIME_OFFSET;

					if (currTime <= node.startTime)
					{
						continue;
					}

					if (RECORD_TYPE_NULL != m_pRecStatus[chnn].type)
					{
						node.endTime = currTime;//在限定时间点
					}
					else
					{
						if(node.endTime < node.startTime)
						{
							assert(false);
							node.endTime = currTime;//在限定时间点
						}
					}
					assert(bOpenRecordFile);
					assert(node.fileIndex == curRecordFileIndex);	
					assert(node.endTime > node.startTime);
				}

				

				if (!((node.endTime <= startTime) || (endTime <= node.startTime)))
				{
					if (bOpenRecordFile && (curRecordFileIndex == node.fileIndex))
					{
						RECORD_FILE_EN_WRITING(node.type);
					}
					if (IsFileBeUsingForRead(node.chnn, node.fileIndex))
					{
						RECORD_FILE_EN_READING(node.type);
					}

					AddToFileListForSearch(node, pList[chnn], bOnlyRead, bRepeat);
					//++ num;
				}
				else if (endTime <= node.startTime)
				{
					//由于已用文件是以通道成列的，且在列表中已开始时间排序，所以当当前节点的开始时间大于或者等于搜索
					//条件的结束时间，说明后续节点的开始时间都要大于该结束时间（即都不满足要求）。所以可以退出遍历。

					break;
				}
			}
			assert(pList[chnn].GetCount() >= oldNum);
			num += pList[chnn].GetCount() - oldNum;
		}
	}

	return num;
}

/************************************************************************
功能描述：获取录像文件列表，把time对应节点的前后各MAX_READ_FILE_NUM个文件
			加入列表。
输入参数：	chnn--通道序号
			time--时间条件
输出参数：	list--返回列表
返回文件数目。
备注：	
		不能对list做任何删除动作（因为其可能已经加入其他磁盘的信息），在往列表里
		加入新的节点时，考虑列表为非空的情况，并做好排序处理。
************************************************************************/
unsigned long CReclogEx::GetFileInfo(unsigned long chnn, unsigned long time, \
									 FILE_INFO_EX_LIST &list, bool bOnlyRead, bool bRepeat, bool bOfficial)
{
	chnn = CHANNEL_ID(chnn);
	assert (chnn < m_headInfo.chnnNum);
	assert (time > 0);
	assert (NULL != m_pChannelRecordFile);
	assert (chnn == m_pChannelRecordFile[chnn].channel);
	assert (NULL != m_pChannelRecordFile[chnn].pFileList);

	if ((m_headInfo.chnnNum <= chnn) || (0 == time))
	{
		return 0;
	}

	unsigned long num = 0;
	FILE_INFO_EX node;
	POS pos = GetNodePos(m_pFileInfoList[chnn], time);

	if ((NULL == pos) && (!m_pFileInfoList[chnn].IsEmpty()))
	{
		pos = m_pFileInfoList[chnn].GetTailPosition();
	}

	while ((NULL != pos) && (num < MAX_READ_FILE_NUM))
	{
		m_pFileInfoList[chnn].GetPrev(pos);
		++ num;
	}

	if (NULL == pos)
	{
		pos = m_pFileInfoList[chnn].GetHeadPosition();
	}

	bool bOpenRecordFile = false;
	unsigned short curRecordFileIndex = -1;
	if (m_pChannelRecordFile[chnn].pFileList->GetCount() > 0)
	{
		bOpenRecordFile = true;
		assert(1 == m_pChannelRecordFile[chnn].pFileList->GetCount());
		assert(!bOnlyRead); //既然这个分区可以录像，就不可能是只读属性
		curRecordFileIndex = m_pChannelRecordFile[chnn].pFileList->GetHead();
	}

	num = 0;
    unsigned long oldNum = list.GetCount();
	while ((NULL != pos) && (num < (MAX_READ_FILE_NUM << 1)))
	{
		
		node = m_pFileInfoList[chnn].GetNext(pos);
#if 0
		//链表中的最后一个节点，有可能是：录像状态为NULL。
		//如：一启动会预先打开一些文件，但是一直没有录像，则这些文件的录像状态就是NULL
		if ((NULL == pos) && (RECORD_TYPE_NULL == node.type))
		{
			assert(node.startTime == 0);
			assert(node.endTime   == 0);
			continue;
		}

#endif
		//过滤掉“非正式文件”
		if (bOfficial && RECORD_FILE_IS_UNOFFICAL(node.type))
		{
			continue;
		}

		//正在录像的节点，需要更新结束时间。	
		//if ((NULL == pos) && (RECORD_TYPE_NULL != m_pRecStatus[chnn].type))
		//由于可以兼容随意修改时间而不删除数据，所以排列表队尾的不一定是正在录像的文件，所以判断条件需要修改。
		if (node.fileIndex == curRecordFileIndex)
		{
			//2010-01-24 15:05:00 YSW
			//过滤掉限定时间内部的文件
			unsigned long currTime = GetCurrTime32() - MAX_TIME_OFFSET;

			if (currTime <= node.startTime)
			{
				continue;
			}

			if (RECORD_TYPE_NULL != m_pRecStatus[chnn].type)
			{
				node.endTime = currTime;//在限定时间点
			}
			else
			{
				if(node.endTime < node.startTime)
				{
					assert(false);
					node.endTime = currTime;//在限定时间点
				}
			}
			assert(bOpenRecordFile);
			assert(node.fileIndex == curRecordFileIndex);	
			assert(node.endTime > node.startTime);
		}

		if (bOpenRecordFile && (curRecordFileIndex == node.fileIndex))
		{
			RECORD_FILE_EN_WRITING(node.type);
		}

		if (IsFileBeUsingForRead(node.chnn, node.fileIndex))
		{
			RECORD_FILE_EN_READING(node.type);
		}

		AddToFileListForSearch(node, list, bOnlyRead, bRepeat);
		//++ num;
        
	}

	assert(list.GetCount() >= oldNum);
	num = list.GetCount() - oldNum;

	return num;
}

/************************************************************************
功能描述：获取录像记录列表，并以通道+时间排序的方式加入返回列表。
输入参数：	chnn--搜索通道
			type--录像类型模板
			startTime--搜索的开始时间
			endTime--搜索的结束时间
输出参数：	list--记录文件信息的列表
返回满足条件的文件数目。
备注：
		不能对list做任何删除动作（因为其可能已经加入其他磁盘的信息），在往列表里
		加入新的节点时，考虑列表为非空的情况，并做好排序处理。
************************************************************************/
unsigned long  CReclogEx::GetReclog(unsigned long chnn, unsigned long type, unsigned long startTime, unsigned long endTime, \
									RECLOG_INFO_EX_LIST &list, bool bOnlyRead, bool bRepeat, bool bOfficial)
{
	chnn = CHANNEL_ID(chnn);
	assert (chnn < m_headInfo.chnnNum);
	assert (0 != type);
	assert (endTime > startTime);
	assert (NULL != m_pFile);
	assert (NULL != m_pFileInfoList);
	if ((NULL == m_pFile) || (NULL == m_pFileInfoList))
	{
		return 0;
	}

	//先获得满足时间条件的文件列表
	FILE_INFO_EX_LIST fileList;
	if (0 == GetFileList((ULONGLONG)0x01<<chnn, startTime, endTime, fileList))
	{
		return 0;
	}
	else
	{
		unsigned long num = MAX_BUFFER_LEN/(sizeof(FILE_RECLOG_INFO) * m_headInfo.fileLogNum);
		FILE_RECLOG_INFO *pData = new FILE_RECLOG_INFO [num*m_headInfo.fileLogNum];
		assert (NULL != pData);
		if (NULL == pData)
		{
			return 0;
		}

		unsigned short headIndex = 0, tailIndex = 0;
		unsigned long retNum = 0, getNum = 0, i = 0, chnn = 0;
		headIndex = tailIndex = fileList.GetHead().fileIndex;
		FILE_RECLOG_INFO *pSection = pData;

		POS pos = fileList.GetHeadPosition();
		while (NULL != pos)
		{
			FILE_INFO_EX &node = fileList.GetNext(pos);

			//过滤掉非正式文件
			if (bOfficial && RECORD_FILE_IS_UNOFFICAL(node.type))
			{
				continue;
			}

			//如果遍历完了缓冲区的所有数据，则需要重新读取一批数据
			if (tailIndex <= node.fileIndex)
			{
				//headIndex始终指向缓冲区对应的第一个文件序号
				headIndex = tailIndex;

				retNum = ReadReclog(pData, sizeof(FILE_RECLOG_INFO) * m_headInfo.fileLogNum * num, num, headIndex);
				if (0 == retNum)
				{
					//获取失败，说明异常
					assert (false);
					delete [] pData;
					return getNum;
				}

				i = 0;	//重新读取了一批段记录，需要更新i。
				tailIndex += (unsigned short)retNum;//tailIndex指向缓冲区对应的最后一个文件序号
			}

			for (; i<retNum; ++i)	
			{
				if ((headIndex + i) == node.fileIndex)
				{
					pSection = pData + (i * m_headInfo.fileLogNum);

					//满足通道条件
					if (chnn == node.chnn)
					{
						getNum += AddToReclogList(pSection, type, startTime, endTime, list, node, bOnlyRead, bRepeat);
					}

					break;
				}
			}
		}

		delete [] pData;
		return getNum;
	}

	return 0;
}

/************************************************************************
功能描述：获取录像记录列表，并以通道为列表单位及按照时间排序的方式加入返回。
输入参数：	chnnbBits--搜索通道模板
			type--录像类型模板
			startTime--搜索的开始时间
			endTime--搜索的结束时间
			chnnNum--搜索的通道数目（和视频通道相同）
输出参数：	pList--记录文件信息的列表
返回满足条件的文件数目。
备注：
			不能对pList做任何删除动作（因为其可能已经加入其他磁盘的信息），在往列表里
			加入新的节点时，考虑列表为非空的情况，并做好排序处理。
************************************************************************/
unsigned long CReclogEx::GetReclog(ULONGLONG chnnbBits, unsigned long type, unsigned long startTime, unsigned long endTime, 
						RECLOG_INFO_EX_LIST *pList, unsigned char chnnNum, bool bOnlyRead, bool bRepeat, bool bOfficial)
{
	assert (0 != chnnbBits);
	assert (0 != type);
	assert (endTime > startTime);
	assert (NULL != m_pFile);
	assert (NULL != m_pFileInfoList);
	if ((NULL == m_pFile) || (NULL == m_pFileInfoList))
	{
		return 0;
	}

	assert (NULL != pList);
	assert (chnnNum > 0);
	if ((NULL == pList) || (0 == chnnNum))
	{
		return 0;
	}

	//说明最大通道的编号超出了chnnNum
	if (chnnbBits & ~ALL_CHNN_MASK(chnnNum))
	{
		chnnbBits = chnnbBits & ALL_CHNN_MASK(chnnNum);	//进行修正
	}

	//取chnnNum和m_headInfo.chnnNum的最小值用于遍历通道
	chnnNum = (m_headInfo.chnnNum > chnnNum) ? chnnNum : m_headInfo.chnnNum;

	//先获得满足时间条件的文件列表，必须以文件索引为序
	FILE_INFO_EX_LIST fileList;
	if (0 == GetFileList(chnnbBits, startTime, endTime, fileList))
	{
		return 0;
	}
	else
	{
		unsigned long num = MAX_BUFFER_LEN/(sizeof(FILE_RECLOG_INFO) * m_headInfo.fileLogNum);
		FILE_RECLOG_INFO *pData = new FILE_RECLOG_INFO [num*m_headInfo.fileLogNum];
		assert (NULL != pData);
		if (NULL == pData)
		{
			return 0;
		}

		unsigned short headIndex = 0, tailIndex = 0;
		unsigned long retNum = 0, getNum = 0, i = 0, chnn = 0;
		headIndex = tailIndex = fileList.GetHead().fileIndex;
		FILE_RECLOG_INFO *pSection = pData;

		POS pos = fileList.GetHeadPosition();
		int fileCount = fileList.GetCount();
		bool bFindOneFile = true;

		FILE_INFO_EX node;
		for (int findCount=0; findCount < fileCount; )
		{
			if (bFindOneFile)
			{
				if (NULL == pos)
				{
					//获取失败，说明异常
					assert (false);
					delete [] pData;
					return getNum;
				}

				node = fileList.GetNext(pos);
				//过滤掉非正式文件
				if (bOfficial && RECORD_FILE_IS_UNOFFICAL(node.type))
				{
					findCount++;
					continue;
				}

				bFindOneFile = false;
			}

			//如果遍历完了缓冲区的所有数据，则需要重新读取一批数据。
			if (tailIndex <= node.fileIndex)
			{
				//headIndex始终指向缓冲区对应的第一个文件序号。
				headIndex = tailIndex;
				
				retNum = ReadReclog(pData, sizeof(FILE_RECLOG_INFO) * m_headInfo.fileLogNum * num, num, headIndex);
				if (0 == retNum)
				{
					//获取失败，说明异常
					assert (false);
					delete [] pData;
					return getNum;
				}

				i = 0;	//重新读取了一批段记录，需要更新i。
				tailIndex += (unsigned short)retNum;//tailIndex指向缓冲区对应的最后一个文件序号
			}

			for (; i<retNum; ++i)	
			{
				if ((headIndex + i) == node.fileIndex)
				{
					pSection = pData + (i * m_headInfo.fileLogNum);

					chnn = node.chnn;
					assert (chnnbBits & ((ULONGLONG)0x01 << chnn));

					//满足通道条件
					if (chnnbBits & ((ULONGLONG)0x01 << chnn))
					{
						getNum += AddToReclogList(pSection, type, startTime, endTime, pList[chnn], node, bOnlyRead, bRepeat);
					}

					bFindOneFile = true;
					findCount++;
					break;
				}
			}
		}

		delete [] pData;
		return getNum;
	}
}

/************************************************************************
功能描述：构建日期列表。

备注：
		必须在初始化并修复文件后调用，如果不调用，则没有日期列表。
************************************************************************/
bool CReclogEx::CreateDateList()
{
	assert (m_headInfo.chnnNum > 0);
	m_pDateInfoList	= new DATE_INFO_LIST [m_headInfo.chnnNum];
	assert (NULL != m_pDateInfoList);

	for (unsigned char chnn=0; chnn<m_headInfo.chnnNum; ++chnn)
	{
		POS pos = m_pFileInfoList[chnn].GetHeadPosition();
		while (NULL != pos)
		{
			FILE_INFO_EX & node = m_pFileInfoList[chnn].GetNext(pos);

			//2010-07-19 16:16:00
			//隔天文件，即7月17开始录像的文件，其结束时间为7月19日之后。
			if ((GetEndTime32OfDate(node.startTime) + ONE_DAY_SECOND_NUM) < node.endTime)
			{
				
				//读出实际的段信息，根据段信息加入列表
				unsigned long bufLen = sizeof(SECTION_INFO_EX)*m_headInfo.fileRecSecNum;
				SECTION_INFO_EX *pSection = new SECTION_INFO_EX [m_headInfo.fileRecSecNum];

				if (1 == ReadSection (pSection, bufLen, 1, node.fileIndex))
				{
					for (unsigned long i=0; i<m_headInfo.fileRecSecNum; ++i)
					{
						if ((0 < pSection[i].startTime) && (pSection[i].startTime <= pSection[i].endTime))
						{
							AddToDateList(chnn, node.fileIndex, pSection[i].startTime, pSection[i].endTime);
						}
						else
						{
							//段的记录是连续的，如果发现不满足条件，则可以退出循环。
							break;
						}
					}
				}
				else
				{
					//读取失败，不加入列表
				}

				delete [] pSection;
				pSection = NULL;
			}
			else
			{
				AddToDateList(chnn, node.fileIndex, node.startTime, node.endTime);
			}
		}
	}

	/*
	printf("####on disk:%d\n", m_diskIndex);
	for (unsigned char chnn=0; chnn<m_headInfo.chnnNum; ++chnn)
	{
		printf("###channel:%d\n",chnn);
		POS pos = m_pDateInfoList[chnn].GetHeadPosition();
		while (NULL != pos)
		{
		   DATE_INFO &date = m_pDateInfoList[chnn].GetNext(pos);
		   printf("date:");
		   PrintfBit32Time(date.time);
		   printf("file:\n");
		   POS posfile = date.pFileList->GetHeadPosition();
		   while(NULL != posfile)
		   {
			   unsigned short fileIndex = date.pFileList->GetNext(posfile);
			   printf("     %d\n",fileIndex);
		   }
		}
	}
	*/
	return true;
}

/************************************************************************
功能描述：获得日期列表
输出参数：list--返回列表（以日期的升序排序）
返回数目。
备注：
			不能对list做任何删除动作（因为其可能已经加入其他磁盘的信息），在往列表里
			加入新的节点时，考虑列表为非空的情况，并做好排序处理。
************************************************************************/
unsigned long CReclogEx::GetDateInfo(CMyList <DVR_DATE> &list)
{
	assert (NULL != m_pDateInfoList);
	if (NULL == m_pDateInfoList)
	{
		return 0;
	}

	DVR_DATE date;
	tm		time;

	DATE_INFO *pNode = new DATE_INFO [m_headInfo.chnnNum];
	POS	*pPos = new POS [m_headInfo.chnnNum];

	for (unsigned char chnn=0; chnn<m_headInfo.chnnNum; ++chnn)
	{
		pPos[chnn] = m_pDateInfoList[chnn].GetHeadPosition();
	}

	unsigned char chnnNum = 0;
	for (unsigned char chnn=0; chnn<m_headInfo.chnnNum; ++chnn)
	{
		if (NULL != pPos[chnn])
		{
			pNode[chnn] = m_pDateInfoList[chnn].GetHead();
			++ chnnNum;
		}
	}

	if (0 == chnnNum)	//没有满足条件的通道，返回零
	{
		delete [] pNode;
		delete [] pPos;
		return 0;
	}

	unsigned char i = 0;	//表示当前日期时间最小的通道序号
	unsigned long dateTime = -1;	//用于日期时间最小的通道
	unsigned long num = 0;
	while (chnnNum > 0)
	{
		//查找文件序号最小的通道序号，用i记录。
		dateTime = -1;
		for (unsigned char chnn=0; chnn<m_headInfo.chnnNum; ++chnn)
		{
			if ((NULL != pPos [chnn]) && (dateTime > pNode[chnn].time))
			{
				dateTime = pNode[chnn].time;
				i = chnn;
			}
		}

		//针对该通道遍历列表，把能满足条件的日期加入返回列表
		while (NULL != pPos[i])
		{
			//如果不再时最小时间，则跳出循环，继续寻找合适的通道
			for (unsigned char chnn=0; chnn<m_headInfo.chnnNum; ++chnn)
			{
				if ((NULL != pPos [chnn]) && (pNode[i].time > pNode[chnn].time))
				{
					break;
				}
			}
            
			time = DVRTime32ToTm(pNode[i].time);
			date.year	= time.tm_year + 1900;
			date.month	= time.tm_mon + 1;
			date.mday	= time.tm_mday;
			date.time	= pNode[i].time;

			num += AddToDateList(date, list);

			//过滤掉日期时间相同的节点，包括了刚才已经加入的日期节点。
			for (unsigned char chnn=0; chnn<m_headInfo.chnnNum; ++chnn)
			{
				if ((NULL != pPos [chnn]) && (pNode[i].time == pNode[chnn].time))
				{
					m_pDateInfoList[chnn].GetNext(pPos [chnn]);
					if (NULL != pPos[chnn])
					{
						pNode[chnn] = m_pDateInfoList[chnn].GetAt(pPos[chnn]);
					}
					else
					{
						-- chnnNum;
					}
				}
			}
		}
	}

	delete [] pNode;
	delete [] pPos;
	return num;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CReclogEx::WriteReclogStart(unsigned short fileIndex, unsigned long chnn, RECORD_TYPE type, unsigned long startTime)
{
	assert (NULL != m_pFile);
	if (NULL == m_pFile)
	{
		return false;
	}

	//如果列表为空，或者尾部文件序号和需要写的文件序号不同，则意味着为新的文件，需要加入新的节点。
	//if (m_pFileInfoList[CHANNEL_ID(chnn)].IsEmpty() || (fileIndex != m_pFileInfoList[CHANNEL_ID(chnn)].GetTail().fileIndex))
	if (NULL == GetNodePosEx(m_pFileInfoList[CHANNEL_ID(chnn)], fileIndex))
	{
		FILE_INFO_EX fileInfo;
		memset(&fileInfo, 0, sizeof(FILE_INFO_EX));
		fileInfo.fileIndex = fileIndex;
		fileInfo.diskIndex = m_diskIndex;
		fileInfo.chnn = chnn;

		POS pos = GetPrevNodePos(m_pFileInfoList[CHANNEL_ID(chnn)], startTime);
		if (NULL != pos)
		{
			m_pFileInfoList[CHANNEL_ID(chnn)].InsertAfter(pos, fileInfo);
		}
		else
		{
			m_pFileInfoList[CHANNEL_ID(chnn)].AddHead(fileInfo);
		}
	}

	//FILE_INFO_EX &fileInfo = m_pFileInfoList[CHANNEL_ID(chnn)].GetTail();
	FILE_INFO_EX &fileInfo = m_pFileInfoList[CHANNEL_ID(chnn)].GetAt(GetNodePosEx(m_pFileInfoList[CHANNEL_ID(chnn)], fileIndex));

	assert (fileIndex == fileInfo.fileIndex);
	assert (chnn == fileInfo.chnn);
	assert (!(type & m_pRecStatus[CHANNEL_ID(chnn)].type));
	assert (0 < startTime);
	assert (startTime >= fileInfo.startTime);
	assert (startTime >= fileInfo.endTime);

	{
		//开始时间等于零标识第一次对该文件写事件，需要加入到日期列表里去。
		if (0 == fileInfo.startTime)
		{
			memset(&m_pRecStatus[CHANNEL_ID(chnn)], 0, sizeof(REC_STATUS));
			m_pRecStatus[CHANNEL_ID(chnn)].remainReclogNum = m_headInfo.fileLogNum;
			AddToDateList(fileInfo.chnn, fileInfo.fileIndex, startTime, startTime);
		}
		else//否则更新日期列表
		{
			UpdateDateList(fileInfo.chnn, fileInfo.fileIndex, startTime, startTime);
		}

		//开启新的一段，需要添加段记录
		//在写事件的开始时：要先段，然后事件开始时间。
		//在写事件的结束时：要事件结束时间，然后写段。
		if (RECORD_TYPE_NULL == m_pRecStatus[CHANNEL_ID(chnn)].type)
		{
			if (!WriteSection(fileIndex, chnn, startTime, true))
			{
				return false;
			}
		}

		if (!WriteType(fileIndex, chnn, type, startTime, true))
		{
			return false;
		}

		m_pRecStatus[CHANNEL_ID(chnn)].type |= type;
	}

	{
		if ((0 == fileInfo.startTime) || !(fileInfo.type & type))
		{
			fileInfo.startTime = (0 == fileInfo.startTime) ? startTime : fileInfo.startTime;//如果已经有了开始时间，则不应该改写它。
			fileInfo.type |= type;

			FILE_FILE_INFO info;
			info.chnn		= fileInfo.chnn;
			info.type		= fileInfo.type;
			info.startTime	= fileInfo.startTime;
			info.endTime	= fileInfo.endTime;
			info.status		= FILE_STATUS_WRITING;

			//第一次对该文件写入事件，或者是写入一个新的类型，则需要更新文件信息。
			if (!WriteFileInfo(fileIndex, &info))
			{
				return false;
			}
		}
	}

	return true;
}

bool CReclogEx::WriteReclogEnd(unsigned short fileIndex, unsigned long chnn, RECORD_TYPE type, unsigned long endTime)
{
	assert (NULL != m_pFile);
	if (NULL == m_pFile)
	{
		return false;
	}

	//FILE_INFO_EX &fileInfo = m_pFileInfoList[CHANNEL_ID(chnn)].GetTail();
	FILE_INFO_EX &fileInfo = m_pFileInfoList[CHANNEL_ID(chnn)].GetAt(GetNodePosEx(m_pFileInfoList[CHANNEL_ID(chnn)], fileIndex));

	assert (fileIndex == fileInfo.fileIndex);
	assert (chnn == fileInfo.chnn);
	assert (type & m_pRecStatus[CHANNEL_ID(chnn)].type);
	assert (endTime >= fileInfo.startTime);
	assert (endTime >= fileInfo.endTime);

	{
		if (!WriteType(fileIndex, chnn, type, endTime, false))
		{
			return false;
		}

		m_pRecStatus[CHANNEL_ID(chnn)].type &= ~type;

		//2010-07-19 16:16:00
		//隔天文件，即7月17开始录像的文件，其结束时间为7月19日之后。
		if ((GetEndTime32OfDate(fileInfo.startTime) + ONE_DAY_SECOND_NUM) < endTime)
		{
			//读出实际的段信息，根据段信息加入列表
			unsigned long bufLen = sizeof(SECTION_INFO_EX)*m_headInfo.fileRecSecNum;
			SECTION_INFO_EX *pSection = new SECTION_INFO_EX [m_headInfo.fileRecSecNum];

			if (1 == ReadSection (pSection, bufLen, 1, fileInfo.fileIndex))
			{
				for (unsigned long i=0; i<m_headInfo.fileRecSecNum; ++i)
				{
					if ((0 < pSection[i].startTime) && (0 == pSection[i].endTime))
					{
						//写了开始，但是未写结束的段，即最近记录的段。
						UpdateDateList(fileInfo.chnn, fileInfo.fileIndex, pSection[i].startTime, pSection[i].endTime);
					}
					else if ((0 == pSection[i].startTime) && (0 == pSection[i].endTime))
					{
						//段为被使用，可退出。
						break;
					}
				}
			}
			else
			{
				//读取失败，不加入列表
			}

			delete [] pSection;
			pSection = NULL;
		}
		else
		{
			UpdateDateList(fileInfo.chnn, fileInfo.fileIndex, fileInfo.startTime, endTime);
		}
	}

	{
		fileInfo.endTime = endTime;

		if (RECORD_TYPE_NULL == m_pRecStatus[CHANNEL_ID(chnn)].type)
		{
			if (!WriteSection(fileIndex, chnn, endTime, false))
			{
				return false;
			}

			FILE_FILE_INFO info;
			info.chnn		= fileInfo.chnn;
			info.type		= fileInfo.type;
			info.startTime	= fileInfo.startTime;
			info.endTime	= fileInfo.endTime;
			info.status		= FILE_STATUS_WRITING;

			//如果写事件结束后，暂时停止了所有类型的录像，则更新文件信息（结束时间）。
			if (!WriteFileInfo(fileIndex, &info))
			{
				return false;
			}
		}
	}

	return true;
}

void CReclogEx::RegisterRecordFile(unsigned long channel, unsigned short fileIndex)
{
	assert(NULL != m_pChannelRecordFile);
	assert(channel >= 0);
	assert(channel < m_headInfo.chnnNum);
	//同一时刻，一个通道在这个分区只能使用一个文件。
	assert(0 == m_pChannelRecordFile[channel].pFileList->GetCount());
	
	m_pChannelRecordFile[channel].pFileList->AddTail(fileIndex);

	RUN_LOG(RUN_LOG_ITEM_REC_ADD_REC_LIST, CRunLog::RUN_Num("file:", fileIndex), CRunLog::RUN_Num("chnn:", channel), "Add file to Record list");
}

void CReclogEx::CancelRecordFile(unsigned long channel, unsigned short fileIndex)
{
	assert(NULL != m_pChannelRecordFile);
	assert(channel >= 0);
	assert(channel < m_headInfo.chnnNum);
	assert(1 == m_pChannelRecordFile[channel].pFileList->GetCount());
	assert(fileIndex == m_pChannelRecordFile[channel].pFileList->GetHead());

	//把上一个文件的信息写完整，实质性的为把标记文件完整结束的位置设为非零。
	WriteFileInfoEnd(fileIndex);

	m_pChannelRecordFile[channel].pFileList->RemoveHead();

	RUN_LOG(RUN_LOG_ITEM_REC_PUT_OUT_REC_LIST, CRunLog::RUN_Num("file:", fileIndex), CRunLog::RUN_Num("chnn:", channel), "remove file from Record list");
}

void CReclogEx::RegisterReadFile(unsigned long channel, unsigned short fileIndex)
{
	assert(NULL != m_pChannelReadFile);
	assert(channel >= 0);
	assert(channel < m_headInfo.chnnNum);

	//同一时刻，一个通道在这个分区可以使用多个文件，并且这个多个文件可以重复
	m_pChannelReadFile[channel].pFileList->AddTail(fileIndex);

	RUN_LOG(RUN_LOG_ITEM_PB_ADD_TO_READ_LIST, CRunLog::RUN_Num("file:", fileIndex), CRunLog::RUN_Num("chnn:", channel), "Add file to Read list");
}

void CReclogEx::CancelReadFile(unsigned long channel, unsigned short fileIndex)
{
	assert(NULL != m_pChannelReadFile);
	assert(channel >= 0);
	assert(channel < m_headInfo.chnnNum);
	assert(m_pChannelReadFile[channel].pFileList->GetCount() > 0);

	POS pos = m_pChannelReadFile[channel].pFileList->GetHeadPosition();
	while (NULL != pos)
	{   
		if (fileIndex == m_pChannelReadFile[channel].pFileList->GetAt(pos))
		{
			m_pChannelReadFile[channel].pFileList->RemoveAt(pos);

			RUN_LOG(RUN_LOG_ITEM_PB_PUT_OUT_READ_LIST, CRunLog::RUN_Num("file:", fileIndex), CRunLog::RUN_Num("chnn:", channel), "Remove file to Read list");
			break;
		}
		m_pChannelReadFile[channel].pFileList->GetNext(pos);
	}
	assert(NULL != pos);
}

/*
由于每次出现的异常文件，理论上在每个通道对应的文件列表的尾部，所以可以直接尾部的文件
加入返回列表。不再费心去检查是不是真地出现了异常，不管是有异常无异常，都进行修复动作。
*/
void CReclogEx::GetUnfinishedFile(FILE_INFO_EX_LIST &list)
{
#if 0
	for(unsigned char chnn = 0; chnn < m_headInfo.chnnNum; ++chnn)
	{
		if(!m_pFileInfoList[chnn].IsEmpty())
		{
			list.AddTail(m_pFileInfoList[chnn].GetTail());
		}
	}
#else
	POS pos = m_unFinishedFileList.GetHeadPosition();
	while (NULL != pos)
	{
		list.AddTail(m_unFinishedFileList.GetNext(pos));
	}
#endif
}

bool CReclogEx::Repair(const FILE_INFO_EX & fileInfo, unsigned long endTime)
{
	/*if(m_pFileInfoList[CHANNEL_ID(fileInfo.chnn)].GetTail().fileIndex != fileInfo.fileIndex)
	{
		assert(false);
		return false;
	}*/

	bool bNeedRepair = false;
	//修复段信息
	{
		SECTION_INFO_EX *pSection = new SECTION_INFO_EX[m_headInfo.fileRecSecNum];
		assert(pSection != NULL);

		if(0 != fseek(m_pFile, m_headInfo.sectionOffset + sizeof(SECTION_INFO_EX) * m_headInfo.fileRecSecNum * fileInfo.fileIndex, SEEK_SET))
		{
			delete [] pSection;
			assert(false);
			return false;
		}

		if(1 != fread(pSection, sizeof(SECTION_INFO_EX)*m_headInfo.fileRecSecNum, 1, m_pFile))
		{
			delete [] pSection;
			assert(false);
			return false;
		}

		for(int secIndex = 0; secIndex < m_headInfo.fileRecSecNum; secIndex++)
		{
			if((0 == pSection[secIndex].startTime) || (pSection[secIndex].startTime < fileInfo.startTime))
			{
				break;
			}
			else if(0 == pSection[secIndex].endTime)
			{
				pSection[secIndex].endTime = endTime;
				bNeedRepair = true;
				break;
			}
		}

		if(bNeedRepair)
		{
			if(0 != fseek(m_pFile, -1 * (long)(sizeof(SECTION_INFO_EX) * m_headInfo.fileRecSecNum), SEEK_CUR))
			{
				assert(false);
				delete [] pSection;
				return false;
			}

			if(1 != fwrite(pSection, sizeof(SECTION_INFO_EX)*m_headInfo.fileRecSecNum, 1, m_pFile))
			{
				delete [] pSection;
				assert(false);
				return false;
			}
		}

		delete [] pSection;
	}

	//修复事件信息
	{
		FILE_RECLOG_INFO *pReclog = new FILE_RECLOG_INFO[m_headInfo.fileLogNum];
		assert(pReclog != NULL);
		if(0 != fseek(m_pFile, m_headInfo.reclogoffset + sizeof(FILE_RECLOG_INFO) * m_headInfo.fileLogNum * fileInfo.fileIndex, SEEK_SET))
		{
			delete [] pReclog;
			assert(false);
			return false;
		}

		if(1 != fread(pReclog, sizeof(FILE_RECLOG_INFO)*m_headInfo.fileLogNum, 1, m_pFile))
		{
			delete [] pReclog;
			assert(false);
			return false;
		}

		unsigned long reclogIndex = 0;

		//检查录像事件部分
		unsigned char recTypeNum = RecTypeNum();
		assert (recTypeNum > 0);
		FILE_RECLOG_INFO  *pTempReclog = new FILE_RECLOG_INFO[recTypeNum];
		memset(pTempReclog, 0, sizeof(FILE_RECLOG_INFO)*recTypeNum);
		
		for (unsigned long i=0; i<m_headInfo.fileLogNum; ++i)
		{
			//表示开始
			if ((RECORD_TYPE_NULL != pReclog[i].type) && (pReclog[i].type < RECORD_TYPE_END) && (0 == (pReclog[i].type & RECLOG_END)))
			{
				pTempReclog [RecTypeIndex(static_cast<RECORD_TYPE>(pReclog[i].type))] = pReclog[i];
			}
			
			//表示结束，且时间不为零。
			if ((RECORD_TYPE_NULL != pReclog[i].type) && ((pReclog[i].type - RECLOG_END) < RECORD_TYPE_END) &&\
				(0 < pReclog[i].time) && (pReclog[i].type & RECLOG_END))
			{
				memset(&pTempReclog [RecTypeIndex(static_cast<RECORD_TYPE>(pReclog[i].type))], 0, sizeof(FILE_RECLOG_INFO));
			}

			//表示该预留空间未使用
			if ((RECORD_TYPE_NULL == pReclog[i].type) && (0 == pReclog[i].time))
			{
				break;
			}

			++ reclogIndex;
		}

		for (unsigned char i=0; i<recTypeNum; ++i)
		{
			if ((RECORD_TYPE_NULL != pTempReclog[i].type) && (0 < pTempReclog[i].time))
			{
				//不能越界
				if (reclogIndex < m_headInfo.fileLogNum)
				{
					pReclog[reclogIndex].type = (pTempReclog[i].type | RECLOG_END);
					pReclog[reclogIndex].time = endTime;

					++ reclogIndex;
				}
				else
				{
					break;
				}
			}
		}

		if(bNeedRepair)
		{
			if(0 != fseek(m_pFile, -1 * (long)(sizeof(FILE_RECLOG_INFO) * m_headInfo.fileLogNum), SEEK_CUR))
			{
				delete [] pTempReclog;
				delete [] pReclog;
				assert(false);
				return false;
			}

			if(1 != fwrite(pReclog, sizeof(FILE_RECLOG_INFO)*m_headInfo.fileLogNum, 1, m_pFile))
			{
				delete [] pTempReclog;
				delete [] pReclog;
				assert(false);
				return false;
			}
		}

		delete [] pTempReclog;
		delete [] pReclog;
	}

	//修复文件信息
	if(bNeedRepair)
	{
		if(0 != fseek(m_pFile, m_headInfo.fileInfoOffset + sizeof(FILE_FILE_INFO) * fileInfo.fileIndex, SEEK_SET))
		{
			assert(false);
			return false;
		}

		FILE_FILE_INFO info;
		info.chnn		= fileInfo.chnn;
		info.type		= fileInfo.type;
		info.startTime	= fileInfo.startTime;
		info.endTime	= endTime;
		info.status		= FILE_STATUS_END;

		if(1 != fwrite(&info, sizeof(FILE_FILE_INFO), 1, m_pFile))
		{
			assert(false);
			return false;
		}
	}

	
	//修改列表中的时间。
	//m_pFileInfoList[CHANNEL_ID(fileInfo.chnn)].GetTail().endTime = endTime;
	POS pos = GetNodePosEx(m_pFileInfoList[CHANNEL_ID(fileInfo.chnn)], fileInfo.fileIndex);
	FILE_INFO_EX &node = m_pFileInfoList[CHANNEL_ID(fileInfo.chnn)].GetAt(pos);
	node.endTime = endTime;

	//修改reclog文件
	{
		if(0 != fseek(m_pFile, m_headInfo.fileInfoOffset + sizeof(FILE_FILE_INFO) * fileInfo.fileIndex, SEEK_SET))
		{
			assert(false);
			return false;
		}

		FILE_FILE_INFO info;
		info.chnn		= fileInfo.chnn;
		info.type		= fileInfo.type;
		info.startTime	= fileInfo.startTime;
		info.endTime	= endTime;
		info.status		= FILE_STATUS_END;

		if(1 != fwrite(&info, sizeof(FILE_FILE_INFO), 1, m_pFile))
		{
			assert(false);
			return false;
		}
	}

	return true;
}

bool CReclogEx::DeleteFile(const FILE_INFO_EX &  fileInfo)
{
	POS pos = GetNodePosEx(m_pFileInfoList[CHANNEL_ID(fileInfo.chnn)], fileInfo.fileIndex);
	if (NULL != pos)
	{
		FILE_INFO_EX & node = m_pFileInfoList[CHANNEL_ID(fileInfo.chnn)].GetAt(pos);
		if (node.fileIndex != fileInfo.fileIndex)
		{
			assert (false);
			return false;
		}
				
		PutOutDateList(fileInfo);
		DeleteFileInfo(fileInfo.fileIndex);	
		AddToInvalidFileList(m_invalidFileList, fileInfo.fileIndex);
		//注意：node是引用。如果把下面的删除放到前面，那么node指向的东西都为空了。
		m_pFileInfoList[CHANNEL_ID(fileInfo.chnn)].RemoveAt(pos);
	}	
		
	return true;
}

bool CReclogEx::LockFile(const FILE_INFO_EX &fileInfo, bool bLock)
{
	POS pos = GetNodePos(m_pFileInfoList[CHANNEL_ID(fileInfo.chnn)], fileInfo.startTime);
	if (NULL != pos)
	{
		FILE_INFO_EX &node = m_pFileInfoList[CHANNEL_ID(fileInfo.chnn)].GetAt(pos);
		if (node.fileIndex != fileInfo.fileIndex)
		{
			assert (false);
			return false;
		}

		if (bLock)
		{
			if (RECORD_FILE_IS_LOCK(node.type))
			{
				return true;
			}				
			else
			{
				RECORD_FILE_EN_LOCK(node.type);
			}
		}
		else
		{
			if (RECORD_FILE_IS_LOCK(node.type))
			{
				RECORD_FILE_EN_UNLOCK(node.type);
			}
			else
			{
				return true;
			}
		}

		unsigned long offset = m_headInfo.fileInfoOffset + node.fileIndex*sizeof(FILE_FILE_INFO);	
		if (0 != fseek(m_pFile, offset, SEEK_SET))
		{
			return false;
		}

		FILE_FILE_INFO fileFileInfo;

		if (1 != fread(&fileFileInfo, sizeof(FILE_FILE_INFO), 1, m_pFile))
		{
			return false;
		}

		if (0 != fseek(m_pFile, offset, SEEK_SET))
		{
			return false;
		}

		fileFileInfo.chnn = node.chnn;
		fileFileInfo.startTime = node.startTime;
		fileFileInfo.endTime = node.endTime;
		fileFileInfo.type = node.type;
		if (1 != fwrite(&fileFileInfo, sizeof(FILE_FILE_INFO), 1, m_pFile))
		{		
			return false;
		}
	}

	return true;
}

bool CReclogEx::DeleteOverdueFiles(unsigned long *pHoldTimeD, unsigned long num)
{
	unsigned long *pHoldTimeDNew = NULL;
	unsigned long numNew = 0;
	unsigned char netVideoInputNum = CProduct::Instance()->NetVideoInputNum();
	if(CStreamRecordManEx::Instance()->IsSupportMinorStreamRecord() && netVideoInputNum>0)
	{
		numNew = num + netVideoInputNum;
		pHoldTimeDNew = new unsigned long[numNew];
		assert(NULL!=pHoldTimeDNew);
		memset(pHoldTimeDNew,0,sizeof(unsigned long)*numNew);
		memcpy(pHoldTimeDNew,pHoldTimeD,sizeof(unsigned long)*num);
		memcpy(pHoldTimeDNew+num,pHoldTimeD+(num-netVideoInputNum),sizeof(unsigned long)*netVideoInputNum);
		
	}
	else
	{
		pHoldTimeDNew = pHoldTimeD;
		numNew = num;
	}

	assert(numNew == m_headInfo.chnnNum);

	unsigned long ltoday = GetZeroTime32OfDate(GetCurrTime32());
	unsigned long *pEndDayl = new unsigned long [numNew];
	for (int i = 0; i < m_headInfo.chnnNum; ++i)
	{ 
		if (0 == pHoldTimeDNew[i])
		{
			//表示，此通道的文件不进行过期删除动作。
			pEndDayl[i] = 0;
		}
		else
		{
			pEndDayl[i] = ltoday - (pHoldTimeDNew[i]+ ONE_DAY_SECOND_NUM);
		}		
	}


	for (unsigned short chnn = 0; chnn < m_headInfo.chnnNum; ++chnn)
	{
		if (0 == pEndDayl[chnn])
		{
			continue;
		}

		if (m_pDateInfoList[chnn].IsEmpty())
		{
			continue;
		}
		
		POS nextDatePos = NULL;
		POS datePos = m_pDateInfoList[chnn].GetHeadPosition();
		while (NULL != datePos)
		{
			DATE_INFO &dateInfo = m_pDateInfoList[chnn].GetAt(datePos);
			nextDatePos = datePos;
			m_pDateInfoList[chnn].GetNext(nextDatePos);

			if (dateInfo.time <= pEndDayl[chnn])
			{
				//删除这一天的文件
				POS nextFileIndexPos = NULL;
				POS fileIndexPos = dateInfo.pFileList->GetHeadPosition();
				while (NULL != fileIndexPos)
				{
					unsigned short fileInfoIndex = dateInfo.pFileList->GetAt(fileIndexPos);
					nextFileIndexPos = fileIndexPos;
					dateInfo.pFileList->GetNext(nextFileIndexPos);

					//从文件连中找到这个文件
					POS fileListPos = m_pFileInfoList[chnn].GetHeadPosition();
					while (NULL != fileListPos)
					{
						FILE_INFO_EX &fileInfoTmp = m_pFileInfoList[chnn].GetAt(fileListPos);
						if (fileInfoTmp.fileIndex == fileInfoIndex)
						{
							break;
						}
						m_pFileInfoList[chnn].GetNext(fileListPos);
					}

					//如果没有找到，说明这个文件有可能是跨天文件，在上个日期中被删除了
					if (NULL == fileListPos)
					{
						dateInfo.pFileList->RemoveAt(fileIndexPos);
						fileIndexPos = nextFileIndexPos;
						continue;
					}

					//如果找到这个文件
					FILE_INFO_EX fileInfo = m_pFileInfoList[chnn].GetAt(fileListPos);
					
					//判断这个文件是否正在被写，读或锁定。
					if (IsFileBeUsingForRead(fileInfo.chnn, fileInfo.fileIndex)
					  ||(IsFileBeUsingForRecord(fileInfo.chnn, fileInfo.fileIndex))
					  ||RECORD_FILE_IS_LOCK(fileInfo.type))
					{
						fileIndexPos = nextFileIndexPos;
						continue;
					}
					//判断这个文件是否跨过删除日期
					//assert(fileInfo.endTime >= fileInfo.startTime);
					if (GetZeroTime32OfDate(fileInfo.endTime) > pEndDayl[chnn])
					{
						fileIndexPos = nextFileIndexPos;
						continue;
					}

                    //现在，就可以删除这个文件了
					dateInfo.pFileList->RemoveAt(fileIndexPos);
					m_pFileInfoList[chnn].RemoveAt(fileListPos);

					bool suc = DeleteFileInfo(fileInfo.fileIndex);
					assert(suc);
					AddToInvalidFileList(m_invalidFileList, fileInfo.fileIndex);
					fileIndexPos = nextFileIndexPos;
				}

				if (dateInfo.pFileList->IsEmpty())
				{
					delete dateInfo.pFileList;					
					m_pDateInfoList[chnn].RemoveAt(datePos);
				}
			}
			datePos = nextDatePos;			
		}		
	}

	delete [] pEndDayl;
	pEndDayl = NULL;

	if(CStreamRecordManEx::Instance()->IsSupportMinorStreamRecord() && netVideoInputNum>0)
	{
		delete []pHoldTimeDNew;
		pHoldTimeDNew = NULL;
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////////////////////

void CReclogEx::AddToFileListForInitial(FILE_INFO_EX & fileInfo, FILE_INFO_EX_LIST & list, bool bTime)
{
	POS pos = NULL;

	if (bTime)	//按照开始时间排序
	{
		pos = GetPrevNodePos(list, fileInfo.startTime);
		if (NULL != pos)
		{
			list.InsertAfter(pos, fileInfo);			
		}
		else
		{
			//需要插在头部		
			list.AddHead(fileInfo);
		}
	}
	else	//按照文件序号排序
	{
		pos = GetPrevNodePosEx(list, fileInfo.fileIndex, 0);		
		if (NULL != pos)
		{
			list.InsertAfter(pos, fileInfo);
		}		
		else
		{
			//需要插在头部
			list.AddHead(fileInfo);
		}
	}
}

void CReclogEx::AddToFileListForSearch(FILE_INFO_EX fileInfo, FILE_INFO_EX_LIST & list, bool bOnlyRead, bool bRepeat)
{
	/*unsigned long currTime = GetCurrTime32();
	if (currTime < fileInfo.endTime)
	{
		fileInfo.endTime = currTime;
	}*/

	POS pos = NULL;
	
	pos = GetPrevNodePos(list, fileInfo.startTime);
	if (NULL != pos)
	{
		if (bRepeat)
		{
			if (bOnlyRead)
			{
				RECORD_FILE_EN_ONLYREAD(fileInfo.type);
			}
			list.InsertAfter(pos, fileInfo);
		}
		else
		{
			FILE_INFO_EX &fileInfoTmp = list.GetAt(pos);
			if (fileInfoTmp.startTime == fileInfo.startTime)
			{
				//fileInfo.endTime与fileInfoTmp.endTime应该是完全相等的。
				//但是有可能在修复这个文件时，产生偏差。理论上讲这个偏差也就是几秒
				//assert(fileInfo.endTime == fileInfoTmp.endTime);					
				assert(fileInfo.chnn == fileInfoTmp.chnn);
				//如果出现重复的文件，只要有一个不是只读的，就要以不只读属性加入。
				//因为我们对只读的文件不进行只读操作，只读的文件肯定来自只读硬盘，那么循环就不可能覆盖这个文件。
				//所以要以来自非只读硬盘的文件为准，只有这样我们才可以锁定这个文件。
				if ((RECORD_FILE_IS_ONLYREAD(fileInfoTmp.type)) && (!bOnlyRead))
				{
					fileInfoTmp = fileInfo;
				}			
			}
			else
			{
				if (bOnlyRead)
				{
					RECORD_FILE_EN_ONLYREAD(fileInfo.type);
				}
				list.InsertAfter(pos, fileInfo);
			}
		}
	}
	else
	{
		//需要插在头部
		if (bOnlyRead)
		{
			RECORD_FILE_EN_ONLYREAD(fileInfo.type);
		}
		list.AddHead(fileInfo);
	}	
}

/************************************************************************
功能描述：在列表list中，找到开始时间小于time的节点，其后一个节点的开始时间
			应该大于或者等于time。
备注：	1、如果列表为空，当然返回NULL（外部根据返回为NULL，会把新节点直接加入头部）
		2、如果time < list.GetHead().startTime，也是需要插入头部，所以返回NULL
		3、list.GetTail().startTime <= time，则需要插入到尾部
		4、如果需要插入到列表中间，则利用二分法查找合适的节点
************************************************************************/
POS	CReclogEx::GetPrevNodePos(FILE_INFO_EX_LIST &list, unsigned long time)
{
	if (list.IsEmpty())
	{
		return NULL;
	}
	else
	{
		if (time < list.GetHead().startTime)
		{
			return NULL;
		}
		else if (list.GetTail().startTime <= time)
		{
			return list.GetTailPosition();
		}
		else
		{
			//先检查最近插入的节点
			/* 当lastPos为最后一个节点是，下面的代码有问题
			POS lastPos = list.GetLastInsertPosition();
			if ((NULL != lastPos) && (list.GetAt(lastPos).startTime < time) && (time <= list.NextNode(lastPos).startTime))
			{
				return lastPos;
			}
			*/

			{
				POS lastPos =  NULL;
				POS nextPos = NULL;
				lastPos = list.GetLastInsertPosition();
				if (NULL != lastPos)
				{
					nextPos = lastPos;
					list.GetNext(nextPos);
				}
				if ((NULL != nextPos) && (list.GetAt(lastPos).startTime < time) && (time <= list.NextNode(lastPos).startTime))
				{
					return lastPos;
				}
			}
			
			/*
				把列表的节点对应于数组的下标，其在列表中相对于头节点的数目就是下标值。
				headPos--代表每次查找的一段列表的头节点
				pos--代表匹配的当前节点
				h--代表当前查找段的头节点下标
				t--代表当前查找段的尾节点下标
				m--代表当前查找段的中部节点下标
			*/
			POS headPos = list.GetHeadPosition();
			POS pos = NULL;
			long h=0, t=0, i=0, m=0, num = 0;
			t = list.GetCount()-1;

			while (h <= t)
			{
				pos = headPos;
				m = ((h+t)>>1);//计算中间节点的下标

				//意味着找到合适值了，pos = headPos。
				if (h == m)
				{
					break;
				}

				//找到m对应的节点，从理论上说该节点应该存在，不能找到NULL。
				i = 0;
				num = m - h;
				while (i < num)
				{
					list.GetNext(pos);
					++ i;
				}
				assert (NULL != pos);

				//把当前段的中间节点的开始时间和time比较，
				//如果node.startTime <= time，则需要对后半段再进行查找，所以headPos = pos，h = m。
				//否则，要对前半段进行查找，t = m。
				FILE_INFO_EX &node = list.GetAt(pos);
				if (node.startTime <= time)//
				{
					headPos = pos;
					h = m;
				}
				else
				{
					t = m;
				}
			}

			return pos;
		}
	}
}

/************************************************************************
找文件序号比index小的节点，其后一个节点的文件序号应该比index大。
备注：	参考GetPrevNodePos的注释，两个方法的结构一样。
************************************************************************/
POS	CReclogEx::GetPrevNodePosEx(FILE_INFO_EX_LIST &list, unsigned long index, unsigned long no)
{
	if (list.IsEmpty())
	{
		return NULL;
	}
	else
	{
		if (index < list.GetHead().fileIndex)
		{
			return NULL;
		}
		else if (list.GetTail().fileIndex < index)
		{
			return list.GetTailPosition();
		}
		else
		{
			//先检查最近插入的节点
			POS lastPos = list.GetLastInsertPosition();
			if ((NULL != lastPos) && (list.GetAt(lastPos).fileIndex < index) && (index < list.NextNode(lastPos).fileIndex))
			{
				return lastPos;
			}

			POS headPos = list.GetHeadPosition();
			POS pos = NULL;
			long h=0, t=0, i=0, m=0, num = 0;
			t = list.GetCount()-1;
			while (h <= t)
			{
				pos = headPos;
				m = ((h+t)>>1);

				if (h == m)
				{
					break;
				}

				i = 0;
				num = m - h;
				while (i < num)
				{
					list.GetNext(pos);
					++ i;
				}
				assert (NULL != pos);

				FILE_INFO_EX &node = list.GetAt(pos);
				if (node.fileIndex < index)
				{
					headPos = pos;
					h = m;
				}
				else
				{
					t = m;
				}
			}

			return pos;
		}
	}
}

/************************************************************************
查找开始时间大于或者等于time的节点，其前一个节点的开始时间一定小于time，
其后一个节点的开始时间一定大于其开始时间。
************************************************************************/
POS	CReclogEx::GetNodePos(FILE_INFO_EX_LIST &list, unsigned long time,unsigned long endTime)
{
	if (list.IsEmpty())
	{
		return NULL;
	}
	else
	{
		if (time <= list.GetHead().startTime)
		{
			return list.GetHeadPosition();
		}
		/*
		else if ((0 == list.GetTail().endTime) && (GetCurrTime32() <= time))
		{
			//如果尾节点的结束时间等于零，则表示该文件只是写了开始，还没有写结束。
			//所以需要用当前系统时间和time比对。如果要求的开始时间（time）大于或者等于当前系统时间，
			//则说明条件不满足，应该返回NULL。
			return NULL;
		}
		else if (list.GetTail().startTime < list.GetTail().endTime)
		{
			//如果尾节点的开始时间小于结束时间，则表示该文件是一个已经录像完成的，或者是一个已经写过结束的文件。
			//如果该文件是正在录像的文件，则有两种状态，一是已经停止录像（当前时刻处于停止状态），一是曾经停止过，
			//但是又重新开始录像的。

			//当前对应的通道正在录像（真正地写数据），则意味着尾节点的type不等于RECORD_TYPE_NULL。
			if (RECORD_TYPE_NULL == m_pRecStatus[CHANNEL_ID(list.GetTail().chnn)].type)
			{
				//非录像状态（完全停止写数据），则用endTime和time比对。
				if (list.GetTail().endTime <= time)
				{
					return NULL;
				}
			}
			else
			{
				//录像状态（写数据），则用当前系统时间和time比对。
				if (GetCurrTime32() <= time)
				{
					return NULL;
				}
			}

			//其他的情况，则不需要返回，需要走入下面的步骤。 
		}
		*/
		
		{
#if 0
			//先检查最近访问的节点
			POS lastPos = list.GetLastReadPosition();
			if ((NULL != lastPos) && (list.prevNode(lastPos).endTime <= time) && (time <= list.GetAt(lastPos).startTime))
			{
				return lastPos;
			}

			//向后移动一个节点，再检查
			list.GetNext(lastPos);
			if ((NULL != lastPos) && (list.prevNode(lastPos).endTime <= time) && (time <= list.GetAt(lastPos).startTime))
			{
				return lastPos;
			}
#endif

			POS headPos = list.GetHeadPosition();
			POS endPos = list.GetTailPosition();
			POS pos = NULL;
			long h=0, t=0, i=0, m=0, num = 0;
			t = list.GetCount()-1;
			
			while (h <= t)
			{			
				pos = headPos;
				m = ((h+t)>>1);

				i = 0;
				num = m - h;
				while (i < num)
				{
					list.GetNext(pos);
					++ i;
				}
				assert (NULL != pos);

				FILE_INFO_EX &node = list.GetAt(pos);
				if (node.startTime < time)
				{  
					h = m + 1;	
					if (h > t)
					{
						break;
					}
					else
					{
						list.GetNext(pos);
						if (NULL == pos)
						{
							pos = endPos;
							break;
						}
						headPos = pos;
					}					
				}
				else if (node.startTime == time)
				{
					break;
				}
				else
				{
					t = m - 1;
					
					list.GetPrev(pos);
					if (NULL == pos)
					{
						pos = headPos;
						break;
					}

					
					/*if ((t - h) <= 1)
					{
						break;
					}*/
					
				}
			}
			if (list.GetCount()>0)
			{
				POS posFind = list.GetHeadPosition();
				while(posFind != pos)
				{
					FILE_INFO_EX &node = list.GetAt(posFind);
					if ( (time < node.endTime)&&(node.endTime<endTime) )
					{
						pos = posFind;
						break;
					}
					list.GetNext(posFind);
				}
			}
			return pos;
		}
	}
}

/************************************************************************
查找查找文件序号等于index的节点。
************************************************************************/
POS	CReclogEx::GetNodePosEx(FILE_INFO_EX_LIST &list, unsigned long index)
{
	if (list.IsEmpty())
	{
		return NULL;
	}
	else
	{
		//首先匹配最近插入的节点
		if (NULL != list.GetLastInsertPosition())
		{
			FILE_INFO_EX &lastInsertNode = list.GetLastInsert();
			if (lastInsertNode.fileIndex == index)
			{
				return list.GetLastInsertPosition();
			}
		}
			
		//不存在最近插入节点或者最近插入节点不符合条件
		{
			POS pos = list.GetHeadPosition();
			while (NULL != pos)
			{
				FILE_INFO_EX & node = list.GetAt(pos);
				if (node.fileIndex == index)
				{
					return pos;
				}

				list.GetNext(pos);
			}

			return NULL;
		}
	}
}

#if 0
POS CReclogEx::GetNextNodePos(FILE_INFO_EX_LIST &list, unsigned long time)
{
	if (list.IsEmpty())
	{
		return NULL;
	}
	else
	{
		if (time <= list.GetTail().startTime)
		{
			return NULL;
		}
		else if (list.GetHead().startTime < time)
		{
			return list.GetHeadPosition();
		}
		else
		{
			//先检查最近插入的节点
			POS lastPos = list.GetLastInsertPosition();
			if ((NULL != lastPos) && (list.GetAt(lastPos).startTime < time) && (time <= list.prevNode(lastPos).startTime))
			{
				return lastPos;
			}

			/*
			把列表的节点对应于数组的下标，其在列表中相对于头节点的数目就是下标值。
			headPos--代表每次查找的一段列表的头节点
			pos--代表匹配的当前节点
			h--代表当前查找段的头节点下标
			t--代表当前查找段的尾节点下标
			m--代表当前查找段的中部节点下标
			*/
			POS headPos = list.GetHeadPosition();
			POS pos = NULL;
			long h=0, t=0, i=0, m=0, num = 0;
			t = list.GetCount()-1;

			while (h <= t)
			{
				pos = headPos;
				m = ((h+t)>>1);//计算中间节点的下标

				//意味着找到合适值了，pos = headPos。
				if (h == m)
				{
					break;
				}

				//找到m对应的节点，从理论上说该节点应该存在，不能找到NULL。
				i = 0;
				num = m - h;
				while (i < num)
				{
					list.GetNext(pos);
					++ i;
				}
				assert (NULL != pos);

				//把当前段的中间节点的开始时间和time比较，
				//如果node.startTime >= time，则需要对后半段再进行查找，所以headPos = pos，h = m。
				//否则，要对前半段进行查找，t = m。
				FILE_INFO_EX &node = list.GetAt(pos);
				if (node.startTime >= time)//
				{
					headPos = pos;
					h = m;
				}
				else
				{
					t = m;
				}
			}

			return pos;
		}
	}
}

POS	CReclogEx::GetNextNodePosEx(FILE_INFO_EX_LIST &list, unsigned long index, unsigned long no)
{
	if (list.IsEmpty())
	{
		return NULL;
	}
	else
	{
		if (index < list.GetTail().fileIndex)
		{
			return NULL;
		}
		else if (list.GetHead().fileIndex < index)
		{
			return list.GetHeadPosition();
		}
		else
		{
			//先检查最近插入的节点
			POS lastPos = list.GetLastInsertPosition();
			if ((NULL != lastPos) && (list.GetAt(lastPos).fileIndex < index) && (index < list.prevNode(lastPos).fileIndex))
			{
				return lastPos;
			}

			POS headPos = list.GetHeadPosition();
			POS pos = NULL;
			long h=0, t=0, i=0, m=0, num = 0;
			t = list.GetCount()-1;
			while (h <= t)
			{
				pos = headPos;
				m = ((h+t)>>1);

				if (h == m)
				{
					break;
				}

				i = 0;
				num = m - h;
				while (i < num)
				{
					list.GetNext(pos);
					++ i;
				}
				assert (NULL != pos);

				FILE_INFO_EX &node = list.GetAt(pos);
				if (node.fileIndex > index)
				{
					headPos = pos;
					h = m;
				}
				else
				{
					t = m;
				}
			}

			return pos;
		}
	}
}

POS	CReclogEx::GetNodePosEx(FILE_INFO_EX_LIST &list, unsigned long time)
{
	if (list.IsEmpty())
	{
		return NULL;
	}
	else
	{
		FILE_INFO_EX node;
		if (time < list.GetTail().startTime)
		{
			return list.GetTailPosition();
		}
		else if ((0 == list.GetHead().startTime) && (0 == list.GetHead().endTime))
		{
			//表示加入列表，但是还没有真正被使用，需要跳到其前一个节点，再去判断。
			if (1 == list.GetCount())
			{
				return NULL;
			}
			else
			{
				POS pos = list.GetHeadPosition();
				node = list.GetNext(pos);
			}
		}
		else
		{
			node = list.GetHead();
		}

		if ((node.endTime <= time) || ((0 == node.endTime) && (GetCurrTime32() <= time)))
		{
			return NULL;
		}
		else
		{
			POS headPos = list.GetHeadPosition();
			POS pos = NULL;
			long h=0, t=0, i=0, m=0, num = 0;
			t = list.GetCount()-1;
			while (h <= t)
			{
				pos = headPos;
				m = ((h+t)>>1);

				if (h == m)
				{
					break;
				}

				i = 0;
				num = m - h;
				while (i < num)
				{
					list.GetNext(pos);
					++ i;
				}
				assert (NULL != pos);

				FILE_INFO_EX &node = list.GetAt(pos);
				if (node.startTime > time)
				{
					headPos = pos;
					h = m;
				}
				else
				{
					t = m;
				}
			}

			return pos;
		}
	}
}
#endif
/************************************************************************
功能描述：获取文件列表，以文件序号为序加入返回列表。
输入参数：	chnnBits--通道模版
			startTime--开始时间
			endTime--结束时间
输出参数：	list
返回文件数目。
备注：	1、理论上如果找到了各通道列表中满足条件的文件对应的节点，那么紧跟其后
			的一些节点都是能满足要求（这样的情况应该发生95%概率上）。
		2、而每个通道列表中相邻的两个文件序号会相隔一定数目（实际录像通道数目），
			如果只是简单地按照通道每次遍历整个列表，在加入返回列表时进行排序需要花费
			比较多的时间度。
		3、但是我们能利用2中所说的特征，进行各列表综合查找则能节省很多的时间度。
		4、算法为：
			a、首先找到每个列表中满足条件第一个节点；
			b、然后从文件序号最小的一个通道开始遍历，查找某个通道时，一直到
				某个节点的文件序号大于当前所有通道第一个节点中的最小值为止；
			c、某个通道超出条件范围，则退出检查组。
************************************************************************/
unsigned long CReclogEx::GetFileList(ULONGLONG chnnbBits, unsigned long startTime, unsigned long endTime, FILE_INFO_EX_LIST & list)
{
	FILE_INFO_EX *pNode = new FILE_INFO_EX [m_headInfo.chnnNum];
	POS *pPos = new POS [m_headInfo.chnnNum];

	//首先得到每个通道能满足条件的节点
	unsigned char chnnNum = 0;
	for (unsigned char chnn=0; chnn<m_headInfo.chnnNum; ++chnn)
	{
		if (chnnbBits & ((ULONGLONG)0x01 << chnn))
		{
			pPos [chnn] = GetNodePos(m_pFileInfoList[chnn], startTime,endTime);
		}
		else
		{
			pPos [chnn] = NULL;
		}
	}

	//获得每个最靠前节点的值，用于比较；并记录下满足要求的通道数目，用于下一轮的循环。
	for (unsigned char chnn=0; chnn<m_headInfo.chnnNum; ++chnn)
	{
		if ((chnnbBits & ((ULONGLONG)0x01 << chnn)) && (NULL != pPos [chnn]))
		{
			pNode[chnn] = m_pFileInfoList[chnn].GetAt(pPos [chnn]);
			++ chnnNum;
		}
	}

	if (0 == chnnNum)	//没有满足条件的通道，返回零
	{
		delete [] pNode;
		delete [] pPos;
		return 0;
	}

	unsigned char i = 0;	//表示当前文件序号最小的通道序号
	unsigned short index = -1;	//用于寻找文件序号最小的通道
	unsigned long num = 0;
	while (chnnNum > 0)
	{
		//查找文件序号最小的通道序号，用i记录。
		index = -1;
		for (unsigned char chnn=0; chnn<m_headInfo.chnnNum; ++chnn)
		{
			if ((chnnbBits & ((ULONGLONG)0x01 << chnn)) && (NULL != pPos [chnn]))
			{
				if (index > pNode[chnn].fileIndex)
				{
					index = pNode[chnn].fileIndex;
					i = chnn;
				}
			}
		}

		bool bOpenRecordFile = false;
		unsigned short curRecordFileIndex = -1;
		if (m_pChannelRecordFile[i].pFileList->GetCount() > 0)
		{
			bOpenRecordFile = true;
			assert(1 == m_pChannelRecordFile[i].pFileList->GetCount());
			curRecordFileIndex = m_pChannelRecordFile[i].pFileList->GetHead();
		}

		//针对该通道遍历列表，把能满足条件的文件加入返回列表
		while (NULL != pPos[i])
		{
			pNode[i] = m_pFileInfoList[i].GetAt(pPos [i]);

			//如果不再时最小序号，则跳出循环，继续寻找合适的通道
			for (unsigned char chnn=0; chnn<m_headInfo.chnnNum; ++chnn)
			{
				if ((NULL != pPos [chnn]) && (pNode[i].fileIndex > pNode[chnn].fileIndex))
				{
					break;
				}
			}

			/*
			if (RECORD_TYPE_NULL != m_pRecStatus[i].type)
			{
				pNode[i].endTime = GetCurrTime32();
			}
			*/

			//该文件是正在录像的文件
			if (pNode[i].fileIndex == curRecordFileIndex)
			{
				//录像文件按是否写满情况分为三种：1.写满。
				//2 未写满，但是没有正在写入。3 未写满。正在写。
				//如果是第一种情况，就用文件的结束时间
				//如果是第二种情况，就用文件的当前结束时间
				//如果是第三种情况，就用当前时间。


				//2010-01-24 14:47:00 YSW
				//加入限定时间搜索处理
				unsigned long currTime = GetCurrTime32();

				//当文件的开始时间处于限定时间内（currTime - MAX_TIME_OFFSET， currTime），则应该被过滤掉。
				if ((currTime - MAX_TIME_OFFSET) <= pNode[i].startTime)
				{
					//不应该大于当前时间
					assert (pNode[i].startTime <= currTime);

					//指向下一个，过滤当前文件。
					m_pFileInfoList[i].GetNext(pPos [i]);
					chnnNum -= (NULL == pPos [i]) ? 1 : 0;	//如果列表遍历完成，也要退出检查组
					continue;
				}

				if (RECORD_TYPE_NULL != m_pRecStatus[i].type)
				{
					//如果段的开始时间处于限定时间内（currTime - MAX_TIME_OFFSET， currTime），则应该在前面被过滤掉了。
					//不在限定时间内部，则其结束时间要在限定点上：currTime - MAX_TIME_OFFSET。
					pNode[i].endTime = currTime - MAX_TIME_OFFSET;
				}
				else
				{
					//这里是防止出错
					if(pNode[i].endTime < pNode[i].startTime)
					{
						assert(false);
						//如果段的开始时间处于限定时间内（currTime - MAX_TIME_OFFSET， currTime），则应该在前面被过滤掉了。
						//不在限定时间内部，则其结束时间要在限定点上：currTime - MAX_TIME_OFFSET。
						pNode[i].endTime = currTime - MAX_TIME_OFFSET;
					}
				}

				assert(bOpenRecordFile);
				assert(pNode[i].fileIndex == curRecordFileIndex);			
				assert(pNode[i].endTime > pNode[i].startTime);
			}

			if (!((pNode[i].endTime <= startTime) || (endTime <= pNode[i].startTime)))
			{
				AddToFileListForInitial(pNode[i], list, false);//以文件序号排序

				++ num;
			}
			else if (endTime <= pNode[i].startTime)	//退出检查组
			{
				pPos [i] = NULL;
				-- chnnNum;
				break;
			}

			m_pFileInfoList[i].GetNext(pPos [i]);
			chnnNum -= (NULL == pPos [i]) ? 1 : 0;	//如果列表遍历完成，也要退出检查组
		}
	}

	delete [] pNode;
	delete [] pPos;

	return num;
}

/************************************************************************
功能描述：把某些满足要求的事件加入列表尾部
************************************************************************/


POS CReclogEx::GetSectionPrevNodePos(SECTION_INFO_EX_LIST &list, unsigned long time)
{
	if (list.IsEmpty())
	{
		return NULL;
	}
	else
	{
		if (time < list.GetHead().startTime)
		{
			return NULL;
		}
		else if (list.GetTail().startTime <= time)
		{
			return list.GetTailPosition();
		}
		else
		{
			//先检查最近插入的节点
			{
				POS lastPos = NULL;
				POS nextPos = NULL;
				lastPos = list.GetLastInsertPosition();
				if (lastPos != NULL)
				{
					nextPos = lastPos;
					list.GetNext(nextPos);
				}
				if ((NULL != nextPos) && (list.GetAt(lastPos).startTime < time) && (time <= list.NextNode(lastPos).startTime))
				{
					return lastPos;
				}
			}
			
			/*
			把列表的节点对应于数组的下标，其在列表中相对于头节点的数目就是下标值。
			headPos--代表每次查找的一段列表的头节点
			pos--代表匹配的当前节点
			h--代表当前查找段的头节点下标
			t--代表当前查找段的尾节点下标
			m--代表当前查找段的中部节点下标
			*/
			POS headPos = list.GetHeadPosition();
			POS pos = NULL;
			long h=0, t=0, i=0, m=0, num = 0;
			t = list.GetCount()-1;

			while (h <= t)
			{
				pos = headPos;
				m = ((h+t)>>1);//计算中间节点的下标

				//意味着找到合适值了，pos = headPos。
				if (h == m)
				{
					break;
				}

				//找到m对应的节点，从理论上说该节点应该存在，不能找到NULL。
				i = 0;
				num = m - h;
				while (i < num)
				{
					list.GetNext(pos);
					++ i;
				}
				assert (NULL != pos);

				//把当前段的中间节点的开始时间和time比较，
				//如果node.startTime <= time，则需要对后半段再进行查找，所以headPos = pos，h = m。
				//否则，要对前半段进行查找，t = m。
				SECTION_INFO_EX &node = list.GetAt(pos);
				if (node.startTime <= time)//
				{
					headPos = pos;
					h = m;
				}
				else
				{
					t = m;
				}

			}

			return pos;

		}
	}

}

POS CReclogEx::GetReclogPrevNodePos(RECLOG_INFO_EX_LIST &list, unsigned long time)
{
	if (list.IsEmpty())
	{
		return NULL;
	}
	else
	{
		if (time < list.GetHead().startTime)
		{
			return NULL;
		}
		else if (list.GetTail().startTime <= time)
		{
			return list.GetTailPosition();
		}
		else
		{
			//先检查最近插入的节点
			{
				POS lastPos = NULL;
				POS nextPos = NULL;
				lastPos = list.GetLastInsertPosition();
				if (lastPos != NULL)
				{
					nextPos = lastPos;
					list.GetNext(nextPos);
				}
				if ((NULL != nextPos) && (list.GetAt(lastPos).startTime < time) && (time <= list.NextNode(lastPos).startTime))
				{
					return lastPos;
				}
			}
			
			/*
			把列表的节点对应于数组的下标，其在列表中相对于头节点的数目就是下标值。
			headPos--代表每次查找的一段列表的头节点
			pos--代表匹配的当前节点
			h--代表当前查找段的头节点下标
			t--代表当前查找段的尾节点下标
			m--代表当前查找段的中部节点下标
			*/
			POS headPos = list.GetHeadPosition();
			POS pos = NULL;
			long h=0, t=0, i=0, m=0, num = 0;
			t = list.GetCount()-1;

			while (h <= t)
			{
				pos = headPos;
				m = ((h+t)>>1);//计算中间节点的下标

				//意味着找到合适值了，pos = headPos。
				if (h == m)
				{
					break;
				}

				//找到m对应的节点，从理论上说该节点应该存在，不能找到NULL。
				i = 0;
				num = m - h;
				while (i < num)
				{
					list.GetNext(pos);
					++ i;
				}
				assert (NULL != pos);

				//把当前段的中间节点的开始时间和time比较，
				//如果node.startTime <= time，则需要对后半段再进行查找，所以headPos = pos，h = m。
				//否则，要对前半段进行查找，t = m。
				RECLOG_INFO_EX &node = list.GetAt(pos);
				if (node.startTime <= time)//
				{
					headPos = pos;
					h = m;
				}
				else
				{
					t = m;
				}

			}

			return pos;

		}
	}
}

unsigned long CReclogEx::AddToReclogList(FILE_RECLOG_INFO *pData, unsigned long type, \
										 unsigned long startTime, unsigned long endTime,
										 RECLOG_INFO_EX_LIST &list, 
										 const FILE_INFO_EX &  fileInfo, bool bOnlyRead, bool bRepeat)
{
	unsigned long currTime = GetCurrTime32() - MAX_TIME_OFFSET;

	unsigned char recTypeNum = RecTypeNum();
	assert(recTypeNum > 0);

	RECLOG_INFO_EX *pReclog = new RECLOG_INFO_EX [recTypeNum];
	memset(pReclog, 0, sizeof(RECLOG_INFO_EX)*recTypeNum);

	unsigned long getNum = 0;
    unsigned long oldNum = list.GetCount();
	for (unsigned long i=0; i<m_headInfo.fileLogNum; ++i)
	{
		//表示开始
		if ((RECORD_TYPE_NULL != pData[i].type) && (pData[i].type < RECORD_TYPE_END) && \
			(0 == (pData[i].type & RECLOG_END)))
		{
			pReclog [RecTypeIndex(static_cast<RECORD_TYPE>(pData[i].type))].chnn		= fileInfo.chnn;
			pReclog [RecTypeIndex(static_cast<RECORD_TYPE>(pData[i].type))].type		= pData[i].type;
			pReclog [RecTypeIndex(static_cast<RECORD_TYPE>(pData[i].type))].startTime	= pData[i].time;

			pReclog [RecTypeIndex(static_cast<RECORD_TYPE>(pData[i].type))].diskIndex	= fileInfo.diskIndex;
			pReclog [RecTypeIndex(static_cast<RECORD_TYPE>(pData[i].type))].fileIndex	= fileInfo.fileIndex;

		}

		//表示结束，且时间不为零。
		if ((RECORD_TYPE_NULL != pData[i].type) && ((pData[i].type - RECLOG_END) < RECORD_TYPE_END) &&\
			(0 < pData[i].time) && (pData[i].type & RECLOG_END))
		{
			assert (pReclog [RecTypeIndex(static_cast<RECORD_TYPE>(pData[i].type))].type == (pData[i].type - RECLOG_END));

			pReclog [RecTypeIndex(static_cast<RECORD_TYPE>(pData[i].type))].endTime		= pData[i].time;//(currTime < pData[i].time) ? currTime : pData[i].time;

			//进行类型过滤
			if (pReclog [RecTypeIndex(static_cast<RECORD_TYPE>(pData[i].type))].type & type)
			{
				//2010-02-27 09:29:00 YSW
				//检查时间逻辑合理性，比如开始时间要小于或者等于结束时间(注意：此时结束时间已经得到修正）
				if (pReclog [RecTypeIndex(static_cast<RECORD_TYPE>(pData[i].type))].startTime <= pReclog [RecTypeIndex(static_cast<RECORD_TYPE>(pData[i].type))].endTime)
				{
					//进行时间过滤(2010-01-24 15:20:00 YSW)
					if ( !( (pReclog [RecTypeIndex(static_cast<RECORD_TYPE>(pData[i].type))].endTime <= startTime) ||\
						(endTime <= pReclog [RecTypeIndex(static_cast<RECORD_TYPE>(pData[i].type))].startTime) ) )
					{
						pReclog [RecTypeIndex(static_cast<RECORD_TYPE>(pData[i].type))].type |= (fileInfo.type & 0xffff0000);
						AddToReclogList (pReclog [RecTypeIndex(static_cast<RECORD_TYPE>(pData[i].type))], list, bOnlyRead, bRepeat);
					}
				}
			}

			memset(&pReclog [RecTypeIndex(static_cast<RECORD_TYPE>(pData[i].type))], 0, sizeof(RECLOG_INFO_EX));
		}

		//表示该预留空间未使用
		if ((RECORD_TYPE_NULL == pData[i].type) && (0 == pData[i].time))
		{
			break;
		}
	}

	assert(list.GetCount() >= oldNum);
	getNum += list.GetCount() - oldNum;

	//处理正在写的事件，因为其没有结束，所以需要在此根据其开始，并加上当前时间为结束时间作为一个事件加入列表。
	oldNum = list.GetCount();
	for (unsigned char i=0; i<recTypeNum; ++i)
	{
		if ((RECORD_TYPE_NULL != pReclog [i].type) && (pReclog [i].type < RECORD_TYPE_END))
		{
			//加入限定条件(2010-01-24 15:20:00 YSW)
			if (currTime <= pReclog [i].startTime)
			{
				continue;
			}

			pReclog [i].endTime		= (currTime + MAX_TIME_OFFSET);//限定时间点
            assert(pReclog[i].endTime > pReclog[i].startTime);

			if (pReclog [i].type & type)
			{
				//进行时间过滤(2010-02-27 09:36:00 YSW)
				if ( !( (pReclog [i].endTime <= startTime) ||\
					(endTime <= pReclog [i].startTime) ) )
				{
					AddToReclogList (pReclog [i], list, bOnlyRead, bRepeat);
				}
				
			}
		}
	}
	assert(list.GetCount() >= oldNum);
	getNum += list.GetCount() - oldNum;

	delete [] pReclog;
	return getNum;
}

void CReclogEx::AddToReclogList(RECLOG_INFO_EX reclog, RECLOG_INFO_EX_LIST &list, bool bOnlyRead, bool bRepeat)
{
	/*unsigned long currTime = GetCurrTime32();
	if (currTime < reclog.endTime)
	{
		reclog.endTime = currTime;
	}*/

	POS pos = NULL;

	//按照开始时间排序
	pos = GetReclogPrevNodePos(list, reclog.startTime);
	if (NULL != pos)
	{
		if (bRepeat)
		{
			if (bOnlyRead)
			{
				RECORD_FILE_EN_ONLYREAD(reclog.type);
			}
			list.InsertAfter(pos, reclog);
		}
		else
		{
			RECLOG_INFO_EX &reclogTmp = list.GetAt(pos);
			if ((reclogTmp.startTime == reclog.startTime)
              &&(reclogTmp.type == reclog.type)
			  &&(reclogTmp.chnn == reclog.chnn))
			{
				//此处是为了把 主录像中的事件 与 冗余录像中的事件 中，相同的事件只
				//返回一条记录。
				//两条事件相同的条件是：
				//1. reclogTmp.startTime 与 reclog.startTime应该是完全相等的
				//2. reclogTmp.type 与 reclog.type应该是完全相等的
				//3. reclogTmp.endTime与reclog.endTime应该是完全相等的。
				//   但是有可能在修复这个文件时，产生偏差。理论上讲这个偏差也就是几秒
				//4  reclogTmp.chnn 与 reclog.chnn应该是完全相等的.
				//如果出现重复的文件，只要有一个不是只读的，就要以不只读属性加入。
				//因为我们对只读的文件不进行只读操作，只读的文件肯定来自只读硬盘，那么循环就不可能覆盖这个文件。
				//所以要以来自非只读硬盘的文件为准，只有这样我们才可以锁定这个文件。
				if ((RECORD_FILE_IS_ONLYREAD(reclogTmp.type)) && (!bOnlyRead))
				{
					reclogTmp = reclog;
				}
			}
			else
			{
				if (bOnlyRead)
				{
					RECORD_FILE_EN_ONLYREAD(reclog.type);
				}
				list.InsertAfter(pos, reclog);
			}
		}
	}
	else
	{
		//需要插在头部
		if (bOnlyRead)
		{
			RECORD_FILE_EN_ONLYREAD(reclog.type);
		}
		list.AddHead(reclog);
	}		
	
}


void CReclogEx::AddToSectionList(const SECTION_INFO_EX & section, SECTION_INFO_EX_LIST &list, bool bRepeat)
{
	POS pos = NULL;

	//按照开始时间排序
	pos = GetSectionPrevNodePos(list, section.startTime);
	if (NULL != pos)
	{
		if (bRepeat)
		{
			list.InsertAfter(pos, section);
		}
		else
		{
			SECTION_INFO_EX &sectionTmp = list.GetAt(pos);
			if (sectionTmp.startTime == section.startTime)
			{
				//sectionTmp.endTime与section.endTime应该是完全相等的。
				//但是有可能在修复这个文件时，产生偏差。理论上讲这个偏差也就是几秒
				//assert(sectionTmp.endTime == section.endTime);
			}
			else
			{
				list.InsertAfter(pos, section);
			}
		}
	}
	else
	{
		//需要插在头部
		list.AddHead(section);
	}		
}
/************************************************************************
功能描述：从fileIndex文件开始读取num个文件的信息。
备注：	根据缓冲区和num的最大容量来定。
************************************************************************/
unsigned long CReclogEx::ReadFileInfo (FILE_FILE_INFO *pData, unsigned long bufLen, unsigned long num, unsigned short fileIndex)
{
	assert (NULL != pData);
	assert (num > 0);
	assert (fileIndex < m_headInfo.fileNum);
	assert (bufLen >= (sizeof(FILE_FILE_INFO)*num));
	if ((NULL == pData) || (0 == num) || (fileIndex >= m_headInfo.fileNum) || (bufLen < (sizeof(FILE_FILE_INFO)*num)))
	{
		return 0;
	}

	//检查要读的数目是否已经超出了总数目
	if (m_headInfo.fileNum < (fileIndex + num))
	{
		num = m_headInfo.fileNum - fileIndex;
	}

	if (0 == fseek(m_pFile, m_headInfo.fileInfoOffset+fileIndex*sizeof(FILE_FILE_INFO), SEEK_SET))
	{
		size_t ret = fread(pData, sizeof(FILE_FILE_INFO), num, m_pFile);
		if (ret > 0)
		{
			return (unsigned long)ret;
		}
		else
		{
			return 0;
		}
	}
	else
	{
		return 0;
	}
}

/************************************************************************
功能描述：从序号为fileIndex文件开始读取num个文件的段信息。
备注：	根据缓冲区和num的最大容量来定。
************************************************************************/
unsigned long CReclogEx::ReadSection (SECTION_INFO_EX *pData, unsigned long bufLen, unsigned long num, unsigned short fileIndex)
{
	assert (NULL != pData);
	assert (num > 0);
	assert (fileIndex < m_headInfo.fileNum);
	assert (bufLen >= (sizeof(SECTION_INFO_EX)*m_headInfo.fileRecSecNum*num));
	if ((NULL == pData) || (0 == num) || (fileIndex >= m_headInfo.fileNum) || (bufLen < (sizeof(SECTION_INFO_EX)*m_headInfo.fileRecSecNum*num)))
	{
		return 0;
	}

	//检查要读的数目是否已经超出了总数目
	if (m_headInfo.fileNum < (fileIndex + num))
	{
		num = m_headInfo.fileNum - fileIndex;
	}

	if (0 == fseek(m_pFile, m_headInfo.sectionOffset+fileIndex*sizeof(SECTION_INFO_EX)*m_headInfo.fileRecSecNum, SEEK_SET))
	{
		size_t ret = fread(pData, sizeof(SECTION_INFO_EX)*m_headInfo.fileRecSecNum, num, m_pFile);
		if (ret > 0)
		{
			return (unsigned long)ret;
		}
		else
		{
			return 0;
		}
	}
	else
	{
		return 0;
	}
}

/************************************************************************
功能描述：从序号为fileIndex文件开始读取num个文件的事件记录信息。
备注：	根据缓冲区和num的最大容量来定。
************************************************************************/
unsigned long CReclogEx::ReadReclog (FILE_RECLOG_INFO *pData, unsigned long bufLen, unsigned long num, unsigned short fileIndex)
{
	assert (NULL != pData);
	assert (num > 0);
	assert (fileIndex < m_headInfo.fileNum);
	assert (bufLen >= (sizeof(FILE_RECLOG_INFO)*m_headInfo.fileLogNum*num));
	if ((NULL == pData) || (0 == num) || (fileIndex >= m_headInfo.fileNum) || (bufLen < (sizeof(FILE_RECLOG_INFO)*m_headInfo.fileLogNum*num)))
	{
		return 0;
	}

	//检查要读的数目是否已经超出了总数目
	if (m_headInfo.fileNum < (fileIndex + num))
	{
		num = m_headInfo.fileNum - fileIndex;
	}

	if (0 == fseek(m_pFile, m_headInfo.reclogoffset+fileIndex*sizeof(FILE_RECLOG_INFO)*m_headInfo.fileLogNum, SEEK_SET))
	{
		size_t ret = fread(pData, sizeof(FILE_RECLOG_INFO)*m_headInfo.fileLogNum, num, m_pFile);
		if (ret > 0)
		{
			return (unsigned long)ret;
		}
		else
		{
			return 0;
		}
	}
	else
	{
		return 0;
	}
}

bool CReclogEx::WriteFileInfo(unsigned long fileIndex, FILE_FILE_INFO *pFileInfo)
{
	if (0 == fseek(m_pFile, m_headInfo.fileInfoOffset+fileIndex*sizeof(FILE_FILE_INFO), SEEK_SET))
	{
		if (1 == fwrite(pFileInfo, sizeof(FILE_FILE_INFO), 1, m_pFile))
		{
			fflush(m_pFile);
			return true;
		}
	}

	return false;
}

bool CReclogEx::WriteSection(unsigned long fileIndex, unsigned long chnn, unsigned long time, bool bStart)
{
	//2010-05-21 13:56:00 YSW
	//事件记录预留的空间已经写满，则返回失败。
	if (bStart && (m_pRecStatus[CHANNEL_ID(chnn)].remainReclogNum < 2))
	{
		return false;
	}

	//预留空间已经写满，则返回失败
	if (m_headInfo.fileRecSecNum <= m_pRecStatus[CHANNEL_ID(chnn)].section_index)
	{
		return false;
	}

	unsigned long offset = m_headInfo.sectionOffset + \
							fileIndex*m_headInfo.fileRecSecNum*sizeof(SECTION_INFO_EX) + \
							m_pRecStatus[CHANNEL_ID(chnn)].section_index*sizeof(SECTION_INFO_EX);

	//开始时间写前面四个字节，结束时间写后面四个字节。
	offset += (bStart ? 0 : sizeof(unsigned long));

	//2010-02=27 09:07:00 YSW
	//如果是写第一条的开始，则先把该文件对应的段信息区域全部清零.
	if (bStart && (0 == m_pRecStatus[CHANNEL_ID(chnn)].section_index))
	{
		if (0 != fseek(m_pFile, offset, SEEK_SET))
		{
			return false;
		}

		if (NULL == m_pCleanDataBuf)
		{
			unsigned long len = (sizeof(SECTION_INFO_EX) * m_headInfo.fileRecSecNum);
			len = (len < (sizeof(FILE_RECLOG_INFO) * m_headInfo.fileLogNum)) ? (sizeof(FILE_RECLOG_INFO) * m_headInfo.fileLogNum) : len;
		
			m_pCleanDataBuf = new unsigned char [len];
			memset(m_pCleanDataBuf, 0, len);
		}

		if (1 != fwrite(m_pCleanDataBuf, sizeof(SECTION_INFO_EX)*m_headInfo.fileRecSecNum, 1, m_pFile))
		{
			return false;
		}
	}

	if (0 == fseek(m_pFile, offset, SEEK_SET))
	{
		if (1 == fwrite(&time, sizeof(unsigned long), 1, m_pFile))
		{
			if (!bStart)
			{
				//结束时，指向下一条记录索引处。
				++ m_pRecStatus[CHANNEL_ID(chnn)].section_index;
			}
			
			fflush(m_pFile);
			return true;
		}
	}

	return false;
}
 
bool CReclogEx::WriteType(unsigned long fileIndex, unsigned long chnn, RECORD_TYPE type, unsigned long time, bool bStart)
{
	//对于事件日志，在写一条事件的开始的时候，要同时为该事件的结束预留好空间。
	assert(m_pRecStatus[CHANNEL_ID(chnn)].remainReclogNum <= m_headInfo.fileLogNum);
	if (bStart)
	{
		//预留好空间已经写满，则返回失败
		if (m_pRecStatus[CHANNEL_ID(chnn)].remainReclogNum < 2)
		{
			return false;
		}
		m_pRecStatus[CHANNEL_ID(chnn)].remainReclogNum -= 2;
	}
	assert(m_pRecStatus[CHANNEL_ID(chnn)].reclog_index < m_headInfo.fileLogNum);
	
	FILE_RECLOG_INFO reclog_info;

	unsigned long offset =	m_headInfo.reclogoffset + \
							fileIndex*m_headInfo.fileLogNum*sizeof(FILE_RECLOG_INFO) + \
							m_pRecStatus[CHANNEL_ID(chnn)].reclog_index*sizeof(FILE_RECLOG_INFO);
	
	//2010-02=27 09:16:00 YSW
	//如果是写第一条的开始，则先把该文件对应的事件信息区域全部清零.
	if (bStart && (0 == m_pRecStatus[CHANNEL_ID(chnn)].reclog_index))
	{
		if (0 != fseek(m_pFile, offset, SEEK_SET))
		{
			return false;
		}

		if (NULL == m_pCleanDataBuf)
		{
			unsigned long len = (sizeof(SECTION_INFO_EX) * m_headInfo.fileRecSecNum);
			len = (len < (sizeof(FILE_RECLOG_INFO) * m_headInfo.fileLogNum)) ? (sizeof(FILE_RECLOG_INFO) * m_headInfo.fileLogNum) : len;

			m_pCleanDataBuf = new unsigned char [len];
			memset(m_pCleanDataBuf, 0, len);
		}

		if (1 != fwrite(m_pCleanDataBuf, sizeof(FILE_RECLOG_INFO)*m_headInfo.fileLogNum, 1, m_pFile))
		{
			return false;
		}
	}

	//生成事件记录
	{
		reclog_info.type = type;
		if (!bStart)	//加入结束标记
		{
			reclog_info.type |= RECLOG_END;
		}

		reclog_info.time = time;
	}

	if (0 == fseek(m_pFile, offset, SEEK_SET))
	{
		if (1 == fwrite(&reclog_info, sizeof(FILE_RECLOG_INFO), 1, m_pFile))
		{
			++ m_pRecStatus[CHANNEL_ID(chnn)].reclog_index;
			
			fflush(m_pFile);
			return true;
		}
	}

	return false;
}

bool CReclogEx::WriteFileInfoEnd(unsigned long fileIndex)
{
	if (0 == fseek(m_pFile, m_headInfo.fileInfoOffset+fileIndex*sizeof(FILE_FILE_INFO), SEEK_SET))
	{
		FILE_FILE_INFO fileInfo;

		if (1 == fread(&fileInfo, sizeof(FILE_FILE_INFO), 1, m_pFile))
		{
			if (RECORD_TYPE_NULL != fileInfo.type)
			{
				if (0 == fseek(m_pFile, m_headInfo.fileInfoOffset+fileIndex*sizeof(FILE_FILE_INFO), SEEK_SET))
				{
					assert (FILE_STATUS_WRITING == fileInfo.status);

					fileInfo.status = FILE_STATUS_END;

					if (1 == fwrite(&fileInfo, sizeof(FILE_FILE_INFO), 1, m_pFile))
					{
						fflush(m_pFile);
						return true;
					}
				}
				
			}
		}
	}

	return false;
}

bool CReclogEx::DeleteFileInfo(unsigned long fileIndex)
{
	{
		unsigned long offset = m_headInfo.fileInfoOffset + fileIndex*sizeof(FILE_FILE_INFO);	
		if (0 != fseek(m_pFile, offset, SEEK_SET))
		{
			return false;
		}
		FILE_FILE_INFO fileFileInfo;
		memset(&fileFileInfo, 0, sizeof(FILE_FILE_INFO));
		if (1 != fwrite(&fileFileInfo, sizeof(FILE_FILE_INFO), 1, m_pFile))
		{		
			return false;
		}
	}
	{
		unsigned long offset = m_headInfo.sectionOffset + fileIndex*m_headInfo.fileRecSecNum*sizeof(SECTION_INFO_EX);
		if (0 != fseek(m_pFile, offset, SEEK_SET))
		{
			return false;
		}
		SECTION_INFO_EX *pSectionInfo = new SECTION_INFO_EX [m_headInfo.fileRecSecNum];
		memset(pSectionInfo, 0, sizeof(SECTION_INFO_EX)*m_headInfo.fileRecSecNum);
		if (m_headInfo.fileRecSecNum != fwrite(pSectionInfo, sizeof(SECTION_INFO_EX), m_headInfo.fileRecSecNum, m_pFile))
		{
			delete []pSectionInfo;
			return false;
		}
		delete []pSectionInfo;
		pSectionInfo = NULL;
	}
	{
		unsigned long offset = m_headInfo.reclogoffset + fileIndex*m_headInfo.fileLogNum*sizeof(FILE_RECLOG_INFO);
		if (0 != fseek(m_pFile, offset, SEEK_SET))
		{
			return false;
		}
		FILE_RECLOG_INFO *pFileReclogInfo = new FILE_RECLOG_INFO [m_headInfo.fileLogNum];
		memset(pFileReclogInfo, 0 ,sizeof(FILE_RECLOG_INFO)*m_headInfo.fileLogNum);
		if (m_headInfo.fileLogNum != fwrite(pFileReclogInfo, sizeof(FILE_RECLOG_INFO), m_headInfo.fileLogNum, m_pFile))
		{
			delete [] pFileReclogInfo;
			return false;
		}
		delete [] pFileReclogInfo;
		pFileReclogInfo = NULL;
	}

	fflush(m_pFile);
	return true;

}
unsigned long CReclogEx::AddToDateList(const DVR_DATE &date, CMyList <DVR_DATE> &list)
{
	if (list.IsEmpty())
	{
		list.AddTail(date);
		return 1;
	}
	else
	{
		if (date.time < list.GetHead().time)
		{
			list.AddHead(date);
			return 1;
		}
		else if (list.GetTail().time < date.time)
		{
			list.AddTail(date);
			return 1;
		}
		else
		{
			POS pos = list.GetHeadPosition();
			while (NULL != pos)
			{
				DVR_DATE & node = list.GetAt(pos);
				if (date.time < node.time)
				{
					list.InsertBefore(pos, date);
					return 1;
				}
				else if (date.time == node.time)
				{
					return 0;
				}
				else
				{
					list.GetNext(pos);
				}
			}

			list.AddTail(date);
			return 1;
		}
	}
}

void CReclogEx::AddToDateList(unsigned long chnn, unsigned short fileIndex, unsigned long startTime, unsigned long endTime)
{
	//此函数的做用是：把文件放加入到的 startTime 到 endTime中所有日期节点中。
	//此函数即支持安整个文件的方式（startTime和endTime分为文件的开始和结束时间），调用。
	//也可以根据文件中的段信息，把这个文件加入到相应的日期节点中。
	//
	//如果文件没有跨天，就要用整个文件的方式        
	//如果是跨天的文件，就要按段信息，把这个文件加入到相应的日期节点中。
	//
	chnn = CHANNEL_ID(chnn);
	assert (chnn < m_headInfo.chnnNum);
	assert (NULL != m_pDateInfoList);

	if (NULL != m_pDateInfoList)
	{
		DATE_INFO date;
		memset(&date, 0, sizeof(DATE_INFO));

		if (m_pDateInfoList[chnn].IsEmpty())
		{

			date.time = GetZeroTime32OfDate(startTime);
			do 
			{
				date.pFileList = new CMyList <unsigned short>;
				assert (NULL != date.pFileList);
				//date.pFileList->AddTail(fileIndex);
				AddToList(fileIndex, *date.pFileList);
				m_pDateInfoList[chnn].AddTail(date);
				date.time += ONE_DAY_SECOND_NUM;
			} while(date.time < endTime);
		}
		else
		{
			POS lastPos = m_pDateInfoList[chnn].GetLastInsertPosition();
			if (NULL == lastPos)
			{
				lastPos = m_pDateInfoList[chnn].GetTailPosition();
			}

			{
				DATE_INFO node = m_pDateInfoList[chnn].GetAt(lastPos);
				POS pos = lastPos;
			
				if (startTime <= node.time)
				{
					date.time = (endTime <= node.time) ? GetZeroTime32OfDate(endTime) : GetZeroTime32OfDate(node.time);

					while (GetZeroTime32OfDate(startTime) <= date.time)
					{
						
						if (NULL == pos)
						{
							date.pFileList	= new CMyList <unsigned short>;
							assert (NULL != date.pFileList);
							//date.pFileList->AddTail(fileIndex);
							AddToList(fileIndex, *date.pFileList);
							m_pDateInfoList[chnn].AddHead(date);
						}
						else
						{
							node = m_pDateInfoList[chnn].GetAt(pos);
							if (date.time < node.time)
							{
								m_pDateInfoList[chnn].GetPrev(pos);
								continue;
							}
							else if (date.time == node.time)
							{
								//node.pFileList->AddTail(fileIndex);
								AddToList(fileIndex, *node.pFileList);
								m_pDateInfoList[chnn].GetPrev(pos);
							}
							else
							{
								date.pFileList	= new CMyList <unsigned short>;
								assert (NULL != date.pFileList);
								//date.pFileList->AddTail(fileIndex);
								AddToList(fileIndex, *date.pFileList);
								m_pDateInfoList[chnn].InsertAfter(pos, date);
							}
						}

						date.time -= ONE_DAY_SECOND_NUM;
					}
				}
			}

			{
				DATE_INFO node = m_pDateInfoList[chnn].GetAt(lastPos);
				POS pos = lastPos;
				if (node.time < endTime)
				{
					date.time = (node.time <= startTime) ? GetZeroTime32OfDate(startTime) : (ONE_DAY_SECOND_NUM + GetZeroTime32OfDate(node.time));

					while (date.time < endTime)
					{
					
						if (NULL == pos)
						{
							date.pFileList	= new CMyList <unsigned short>;
							assert (NULL != date.pFileList);
							//date.pFileList->AddTail(fileIndex);
							AddToList(fileIndex, *date.pFileList);
							m_pDateInfoList[chnn].AddTail(date);
						}
						else
						{
							node = m_pDateInfoList[chnn].GetAt(pos);
							if (date.time < node.time)
							{
								date.pFileList	= new CMyList <unsigned short>;
								assert (NULL != date.pFileList);
								//date.pFileList->AddTail(fileIndex);
								AddToList(fileIndex, *date.pFileList);
								m_pDateInfoList[chnn].InsertBefore(pos, date);
							}
							else if (date.time == node.time)
							{
								//node.pFileList->AddTail(fileIndex);
								AddToList(fileIndex, *node.pFileList);
								m_pDateInfoList[chnn].GetNext(pos);
							}
							else
							{
								m_pDateInfoList[chnn].GetNext(pos);
								continue;
							}
						}

						date.time += ONE_DAY_SECOND_NUM;
					}
				}
			}
		}
	}
}

/*
检查新的文件在时间跨度上是否已经超出了日期列表中最后一个日期，即需要增加新的日期节点。
首先要求该文件已经加入在列表中，然后检查是否需要增加新的日期节点。
*/
void CReclogEx::UpdateDateList(unsigned long chnn, unsigned short fileIndex, unsigned long startTime, unsigned long endTime)
{
#if 0
	chnn = CHANNEL_ID(chnn);
	assert (chnn < m_headInfo.chnnNum);
	assert (NULL != m_pDateInfoList);

	if (NULL != m_pDateInfoList)
	{
		assert (m_pDateInfoList[chnn].GetCount() > 0);
		assert (NULL != m_pDateInfoList[chnn].GetTail().pFileList);
		assert (m_pDateInfoList[chnn].GetTail().pFileList->GetCount() > 0);
		assert (m_pDateInfoList[chnn].GetTail().pFileList->GetTail() == fileIndex);

		DATE_INFO &node = m_pDateInfoList[chnn].GetTail();
		assert ((node.time + ONE_DAY_SECOND_NUM) > startTime);//尾节点和文件对应，则startTime必然在该天内。

		if ((node.time + ONE_DAY_SECOND_NUM) < endTime)//超出当前日期了
		{
			DATE_INFO date;
			date.time = node.time + ONE_DAY_SECOND_NUM;//使用紧接着最后日期的一天开始时间

			do 
			{
				date.pFileList = new CMyList <unsigned short>;
				assert (NULL != date.pFileList);
				//date.pFileList->AddTail(fileIndex);
				AddToList(fileIndex, *date.pFileList);
				m_pDateInfoList[chnn].AddTail(date);
				date.time += ONE_DAY_SECOND_NUM;
			} while(date.time < endTime);
		}
	}
#else
	chnn = CHANNEL_ID(chnn);
	assert (chnn < m_headInfo.chnnNum);
	assert (NULL != m_pDateInfoList);

	if (NULL != m_pDateInfoList)
	{
		DATE_INFO date;
		memset(&date, 0, sizeof(DATE_INFO));

		if (m_pDateInfoList[chnn].IsEmpty())
		{
			date.time = GetZeroTime32OfDate(startTime);
			do 
			{
				date.pFileList = new CMyList <unsigned short>;
				assert (NULL != date.pFileList);
				//date.pFileList->AddTail(fileIndex);
				AddToList(fileIndex, *date.pFileList);
				m_pDateInfoList[chnn].AddTail(date);
				date.time += ONE_DAY_SECOND_NUM;
			} while(date.time < endTime);
		}
		else
		{
			POS lastPos = m_pDateInfoList[chnn].GetLastInsertPosition();
			if (NULL == lastPos)
			{
				lastPos = m_pDateInfoList[chnn].GetTailPosition();
			}

			{
				DATE_INFO node = m_pDateInfoList[chnn].GetAt(lastPos);
				POS pos = lastPos;
				if (startTime <= node.time)
				{
					date.time = (endTime <= node.time) ? GetZeroTime32OfDate(endTime) : GetZeroTime32OfDate(node.time);

					while (GetZeroTime32OfDate(startTime) <= date.time)
					{
						if (NULL == pos)
						{
							date.pFileList	= new CMyList <unsigned short>;
							assert (NULL != date.pFileList);
							//date.pFileList->AddTail(fileIndex);
							AddToList(fileIndex, *date.pFileList);
							m_pDateInfoList[chnn].AddHead(date);
						}
						else
						{
							node = m_pDateInfoList[chnn].GetAt(pos);

							if (date.time < node.time)
							{
								m_pDateInfoList[chnn].GetPrev(pos);
								continue;
							}
							else if (date.time == node.time)
							{
								//node.pFileList->AddTail(fileIndex);
								m_pDateInfoList[chnn].GetPrev(pos);
							}
							else
							{
								date.pFileList	= new CMyList <unsigned short>;
								assert (NULL != date.pFileList);
								//date.pFileList->AddTail(fileIndex);
								AddToList(fileIndex, *date.pFileList);
								m_pDateInfoList[chnn].InsertAfter(pos, date);
							}
						}

						date.time -= ONE_DAY_SECOND_NUM;
					}
				}
			}

			{
				DATE_INFO node = m_pDateInfoList[chnn].GetAt(lastPos);
				POS pos = lastPos;
				if (node.time < endTime)
				{
					date.time = (node.time < startTime) ? GetZeroTime32OfDate(startTime) : (ONE_DAY_SECOND_NUM+GetZeroTime32OfDate(node.time));

					while (date.time < endTime)
					{
						if (NULL == pos)
						{
							date.pFileList	= new CMyList <unsigned short>;
							assert (NULL != date.pFileList);
							//date.pFileList->AddTail(fileIndex);
							AddToList(fileIndex, *date.pFileList);
							m_pDateInfoList[chnn].AddTail(date);
						}
						else
						{
							node = m_pDateInfoList[chnn].GetAt(pos);

							if (date.time < node.time)
							{
								date.pFileList	= new CMyList <unsigned short>;
								assert (NULL != date.pFileList);
								//date.pFileList->AddTail(fileIndex);
								AddToList(fileIndex, *date.pFileList);
								m_pDateInfoList[chnn].InsertBefore(pos, date);
							}
							else if (date.time == node.time)
							{
								//node.pFileList->AddTail(fileIndex);
								m_pDateInfoList[chnn].GetNext(pos);
							}
							else
							{
								m_pDateInfoList[chnn].GetNext(pos);
								continue;
							}
						}

						date.time += ONE_DAY_SECOND_NUM;
					}
				}
			}
		}
	}
#endif
}

//注意：要考虑到有重复文件到链表中。如果链表中，有这个文件，就不用在加入了。
void CReclogEx::AddToList(unsigned short index, CMyList <unsigned short> & list)
{
	/*
	printf("%s,%d,begain: on disk :%d, index:%d\n",__FILE__,__LINE__,m_diskIndex, index);
	{
		POS pos = list.GetHeadPosition();
		printf("              list fileindex:\n");
		while (NULL != pos)
		{
			unsigned short fileIndex = list.GetNext(pos);
			printf("                         :%d\n", fileIndex);
		}
	}
*/
	if (list.IsEmpty())
	{
		list.AddTail(index);
	}
	else if (index < list.GetHead())
	{
		list.AddHead(index);
	}
	else if (index > list.GetTail())
	{
		list.AddTail(index);
	}
	else
	{
		bool bInsert = true;
		POS headPos = list.GetHeadPosition();
		POS endPos = list.GetTailPosition();
		POS pos = NULL;
		long h=0, t=0, i=0, m=0, num = 0;
		t = list.GetCount()-1;
		while (h <= t)
		{
			pos = headPos;
			m = ((h+t)>>1);

			i = 0;
			num = m - h;
			while (i < num)
			{
				list.GetNext(pos);
				++ i;
			}
			assert (NULL != pos);

			unsigned short nodeIndex = list.GetAt(pos);
			if (nodeIndex < index)
			{
				h = m + 1;	
				if (h > t)
				{
					break;
				}
				else
				{
					list.GetNext(pos);
					if (NULL == pos)
					{
						pos = endPos;
						break;
					}
					headPos = pos;
				}
			}
			else if (nodeIndex == index)
			{
				bInsert = false;
				break;
				//assert (index != nodeIndex);
			}
			else
			{
				t = m - 1;
				list.GetPrev(pos);
				if (NULL == pos)
				{
					pos = headPos;
					break;
				}
			}
		}

		if (bInsert)
		{
			list.InsertAfter(pos, index);
		}		
	}

	/*
	printf("%s,%d,   end: on disk :%d, index:%d\n",__FILE__,__LINE__,m_diskIndex, index);
	{
		POS pos = list.GetHeadPosition();
		printf("              list fileindex:\n");
		while (NULL != pos)
		{
			unsigned short fileIndex = list.GetNext(pos);
			printf("                         :%d\n", fileIndex);
		}
	}
	*/
}
/*
把fileInfo中记录的文件从某个日期节点的列表中给删除。
*/
void CReclogEx::PutOutDateList(const FILE_INFO_EX & fileInfo)
{
	unsigned long chnn = CHANNEL_ID(fileInfo.chnn);
	assert (chnn < m_headInfo.chnnNum);
	assert (NULL != m_pDateInfoList);

	if (NULL != m_pDateInfoList)
	{
		if (m_pDateInfoList[chnn].GetCount() > 0)
		{
			POS pos = m_pDateInfoList[chnn].GetHeadPosition();
			while (NULL != pos)
			{
				DATE_INFO & node = m_pDateInfoList[chnn].GetAt(pos);

				if (!((fileInfo.endTime <= node.time) || ((node.time+ONE_DAY_SECOND_NUM) <= fileInfo.startTime)))
				{
					assert (NULL != node.pFileList);
					POS p = node.pFileList->GetHeadPosition();
					while (NULL != p)
					{
						if (fileInfo.fileIndex == node.pFileList->GetAt(p))
						{
							node.pFileList->RemoveAt(p);
							break;
						}
						else
						{
							node.pFileList->GetNext(p);
						}
					}

					//检查对应的日期节点所有的文件列表中是否还有文件存在，如果不存在，则需要把该日期节点给删除。
					if (node.pFileList->IsEmpty())
					{
						POS prevPos = pos;
						m_pDateInfoList[chnn].GetNext(pos);

						//这样删除有问题。因为 node.pFileList的申请采用的是new CMyList <unsigned short>
						//所以释放时，要采用delete node.pFileList;
						//delete [] node.pFileList;
						delete node.pFileList;

						node.pFileList = NULL;
						m_pDateInfoList[chnn].RemoveAt(prevPos);
					}
					else
					{
						m_pDateInfoList[chnn].GetNext(pos);
					}					
				}
				else if (fileInfo.endTime <= node.time)
				{
					break;
				}
				else
				{
					m_pDateInfoList[chnn].GetNext(pos);
				}
			}
		}
	}
}

void CReclogEx::AddToInvalidFileList(CMyList <unsigned short> & list, unsigned short fileIndex)
{
	if (list.IsEmpty())
	{
		list.AddTail(fileIndex);
	}
	else
	{
		if (fileIndex < list.GetHead())
		{
			list.AddHead(fileIndex);
		}
		else if (list.GetTail() < fileIndex)
		{
			list.AddTail(fileIndex);
		}
		else
		{
			POS headPos = list.GetHeadPosition();
			POS pos = NULL;
			long h=0, t=0, i=0, m=0, num = 0;
			t = list.GetCount()-1;
			while (h <= t)
			{
				pos = headPos;
				m = ((h+t)>>1);

				if (h == m)
				{
					break;
				}

				i = 0;
				num = m - h;
				while (i < num)
				{
					list.GetNext(pos);
					++ i;
				}
				assert (NULL != pos);

				unsigned short &node = list.GetAt(pos);
				if (node < fileIndex)
				{
					headPos = pos;
					h = m;
				}
				else
				{
					t = m;
				}
			}

			list.InsertAfter(pos, fileIndex);
		}
	}
}

bool CReclogEx::IsFileBeUsingForRecord(unsigned long channel, unsigned short fileIndex)
{
	assert(NULL != m_pChannelRecordFile);
	assert(channel < m_headInfo.chnnNum);

	bool bRet = false;
	if (0 == m_pChannelRecordFile[channel].pFileList->GetCount())
	{
		return bRet;
	}

	//如果在这个分区上，通道channel正在录像，那么这个通道同一时刻在这个分区上，只能把数据录到一个文件里
	assert(1 == m_pChannelRecordFile[channel].pFileList->GetCount());
	if (fileIndex == m_pChannelRecordFile[channel].pFileList->GetHead())
	{
		bRet = true;
	}

	return bRet;
}

bool CReclogEx::IsFileBeUsingForRead(unsigned long channel, unsigned short fileIndex)
{
	assert(NULL != m_pChannelReadFile);
	assert(channel < m_headInfo.chnnNum);

	bool bRet = false;
	if (0 == m_pChannelReadFile[channel].pFileList->GetCount())
	{
		return bRet;
	}

	POS pos = m_pChannelReadFile[channel].pFileList->GetHeadPosition();
	while (NULL != pos)
	{
		if (fileIndex == m_pChannelReadFile[channel].pFileList->GetAt(pos))
		{
			bRet = true;
			break;
		}
		m_pChannelReadFile[channel].pFileList->GetNext(pos);
	}

	return bRet;
}


//////////////////////////////////////////////////////////////////////////
bool CReclogEx::GetOneFileAllInfo(unsigned short fileIndex, unsigned char *pFileHeadInfo, 
								  unsigned char *pFileSectionInfo, unsigned char *pFileReclogInfo)
{    
	assert (fileIndex < m_headInfo.fileNum);

	unsigned long offset = m_headInfo.fileInfoOffset + fileIndex*sizeof(FILE_FILE_INFO);	
	if (0 != fseek(m_pFile, offset, SEEK_SET))
	{
		return false;
	}	
	if (1 != fread(pFileHeadInfo, sizeof(FILE_FILE_INFO), 1, m_pFile))
	{		
		return false;
	}


	offset = m_headInfo.sectionOffset + fileIndex*m_headInfo.fileRecSecNum*sizeof(SECTION_INFO_EX);
	if (0 != fseek(m_pFile, offset, SEEK_SET))
	{
		return false;
	}	
	if (m_headInfo.fileRecSecNum != fread(pFileSectionInfo, sizeof(SECTION_INFO_EX), m_headInfo.fileRecSecNum, m_pFile))
	{
		return false;
	}


	offset = m_headInfo.reclogoffset + fileIndex*m_headInfo.fileLogNum*sizeof(FILE_RECLOG_INFO);
	if (0 != fseek(m_pFile, offset, SEEK_SET))
	{
		return false;
	}
	if (m_headInfo.fileLogNum != fread(pFileReclogInfo, sizeof(FILE_RECLOG_INFO), m_headInfo.fileLogNum, m_pFile))
	{
		return false;
	}

	return true;

}  

//////////////////////////////////////////////////////////////////////////////////////
bool CReclogEx::OpenLogFileForBackup(char *pFilePath)
{

	assert (NULL == m_pFilePathForBackup);	//避免重复初始化
	m_pFilePathForBackup = new char [512];
	assert (NULL != m_pFilePathForBackup);
	if (NULL == m_pFilePathForBackup)
	{
		return false;
	}
	memset(m_pFilePathForBackup, 0, 512);
	if (NULL != pFilePath)
	{
		strcpy(m_pFilePathForBackup, pFilePath);
	}


	//处理文件完整路径，并尝试打开文件。	
	//char filePath [512] = {0};
	//sprintf(filePath, "%s/reclog.bin", m_pFilePathForBackup);
	assert (NULL == m_pFileForBackup);
	if (NULL != m_pFileForBackup)
	{
		fclose(m_pFileForBackup);
	}	
	m_pFileForBackup = fopen(m_pFilePathForBackup, "rb+");
	if (NULL == m_pFileForBackup)
	{
		Release(NULL, 1, &m_pFilePathForBackup);
		return false;
	}
	
	//确认reclog文件是否出错了，不允许文件和通道数目为零的情况
	if (1 != fread(&m_headInfoForBackup, sizeof(RECLOG_FILE_HEAD_INFO), 1, m_pFileForBackup))
	{
		Release(&m_pFileForBackup, 1, &m_pFilePathForBackup);
		return false;
	}		
	if ((0 >= m_headInfoForBackup.fileNum) || (0 >= m_headInfoForBackup.chnnNum))
	{
		Release(&m_pFileForBackup, 1, &m_pFilePathForBackup);
		return false;
	}

	//录像状态
	//m_pRecStatusForBackup = new REC_STATUS [m_headInfoForBackup.chnnNum];
	//assert (NULL != m_pRecStatusForBackup);
	//if (NULL == m_pRecStatusForBackup)
	//{
	//	Release(&m_pFileForBackup, 1, &m_pFilePathForBackup);
	//	return false;
	//}
	memset(&m_recStatusForBackup, 0, sizeof(REC_STATUS));

	return true;

}


void CReclogEx::CloseLogFileForBackup()
{
	Release(&m_pFileForBackup, 1, &m_pFilePathForBackup);

}

bool CReclogEx::WriteFileStartForBackup(unsigned short fileIndex, unsigned long chnn)
{
	assert (NULL != m_pFileForBackup);
	if (NULL == m_pFileForBackup)
	{
		return false;
	}

	memset(&m_recStatusForBackup, 0, sizeof(REC_STATUS));

	memset(&m_fileInfoForBackup, 0, sizeof(FILE_INFO_EX));
	m_fileInfoForBackup.chnn = chnn;
	m_fileInfoForBackup.fileIndex = fileIndex;

	return true;

}

bool CReclogEx::WriteFileEndForBackup()
{
	assert (NULL != m_pFileForBackup);
	if (NULL == m_pFileForBackup)
	{
		return false;
	}

	memset(&m_recStatusForBackup, 0, sizeof(REC_STATUS));

	memset(&m_fileInfoForBackup, 0, sizeof(FILE_INFO_EX));
	return true;
}

bool CReclogEx::WriteReclogStartForBackup(unsigned short fileIndex, unsigned long chnn, RECORD_TYPE type, unsigned long startTime)
{

	assert (NULL != m_pFileForBackup);
	if (NULL == m_pFileForBackup)
	{
		return false;
	}

	
	//FILE_INFO_EX &fileInfo = m_pFileInfoList[CHANNEL_ID(chnn)].GetTail();

	assert (fileIndex == m_fileInfoForBackup.fileIndex);
	assert (chnn == m_fileInfoForBackup.chnn);
	assert (!(type & m_recStatusForBackup.type));
	assert (startTime >= m_fileInfoForBackup.startTime);

	
	//开始时间等于零标识第一次对该文件写事件，需要加入到日期列表里去。
	if (0 == m_fileInfoForBackup.startTime)
	{
		memset(&m_recStatusForBackup, 0, sizeof(REC_STATUS));
	}

	//开启新的一段，需要添加段记录
	if (RECORD_TYPE_NULL == m_recStatusForBackup.type)
	{
		if (!WriteSectionForBackup(fileIndex, chnn, startTime, true))
		{
			return false;
		}
	}
	
	if (!WriteTypeForBackup(fileIndex, chnn, type, startTime, true))
	{
		return false;
	}

	m_recStatusForBackup.type |= type;
	

	if ((0 == m_fileInfoForBackup.startTime) || !(m_fileInfoForBackup.type & type))
	{
		//如果已经有了开始时间，则不应该改写它。
		m_fileInfoForBackup.startTime = (0 == m_fileInfoForBackup.startTime) ? startTime : m_fileInfoForBackup.startTime;
		m_fileInfoForBackup.type |= type;
		
		FILE_FILE_INFO info;
		info.chnn		= m_fileInfoForBackup.chnn;
		info.type		= m_fileInfoForBackup.type;
		info.startTime	= m_fileInfoForBackup.startTime;
		info.endTime	= m_fileInfoForBackup.endTime;
		info.status		= FILE_STATUS_WRITING;

		//第一次对该文件写入事件，或者是写入一个新的类型，则需要更新文件信息。
		if (!WriteFileInfoForBackup(fileIndex, &info))
		{
			return false;
		}
		
	}
	

	return true;
}

bool CReclogEx::WriteReclogEndForBackup(unsigned short fileIndex, unsigned long chnn, RECORD_TYPE type, unsigned long endTime)
{
	assert (NULL != m_pFileForBackup);
	if (NULL == m_pFileForBackup)
	{
		return false;
	}

	//FILE_INFO_EX &fileInfo = m_pFileInfoList[CHANNEL_ID(chnn)].GetTail();

	assert (fileIndex == m_fileInfoForBackup.fileIndex);
	assert (chnn == m_fileInfoForBackup.chnn);
	assert (type & m_recStatusForBackup.type);
	assert (endTime >= m_fileInfoForBackup.startTime);

	
	if (!WriteTypeForBackup(fileIndex, chnn, type, endTime, false))
	{
		return false;
	}
	
	m_recStatusForBackup.type &= ~type;
	
	if (endTime > m_fileInfoForBackup.endTime)
	{
		m_fileInfoForBackup.endTime = endTime;
	}
		
	if (RECORD_TYPE_NULL == m_recStatusForBackup.type)
	{
		if (!WriteSectionForBackup(fileIndex, chnn, endTime, false))
		{
			return false;
		}
		
		FILE_FILE_INFO info;
		info.chnn		= m_fileInfoForBackup.chnn;
		info.type		= m_fileInfoForBackup.type;
		info.startTime	= m_fileInfoForBackup.startTime;
		info.endTime	= m_fileInfoForBackup.endTime;
		info.status		= FILE_STATUS_END;

		//如果写事件结束后，暂时停止了所有类型的录像，则更新文件信息（结束时间）。
		if (!WriteFileInfoForBackup(fileIndex, &info))
		{
			return false;
		}
	}

	return true;

}


bool CReclogEx::WriteTypeForBackup(unsigned long fileIndex, unsigned long chnn, RECORD_TYPE type, unsigned long time, bool bStart)
{
	//预留空间已经写满，则返回失败
	if (m_headInfoForBackup.fileLogNum <= m_recStatusForBackup.reclog_index)
	{
		return false;
	}

	FILE_RECLOG_INFO reclog_info;
	unsigned long offset = m_headInfoForBackup.reclogoffset
		                 + fileIndex*m_headInfoForBackup.fileLogNum*sizeof(FILE_RECLOG_INFO)
						 + m_recStatusForBackup.reclog_index*sizeof(FILE_RECLOG_INFO);
	//生成事件记录
	reclog_info.type = type;
	if (!bStart)	//加入结束标记
	{
		reclog_info.type |= RECLOG_END;
	}
	reclog_info.time = time;
	


	if (0 == fseek(m_pFileForBackup, offset, SEEK_SET))
	{
		if (1 == fwrite(&reclog_info, sizeof(FILE_RECLOG_INFO), 1, m_pFileForBackup))
		{
			++m_recStatusForBackup.reclog_index;
			return true;
		}
	}

	return false;

}


bool CReclogEx::WriteSectionForBackup(unsigned long fileIndex, unsigned long chnn, unsigned long time, bool bStart)
{
	//预留空间已经写满，则返回失败
	if (m_headInfoForBackup.fileRecSecNum <= m_recStatusForBackup.section_index)
	{
		return false;
	}
	
	unsigned long offset = m_headInfoForBackup.sectionOffset
		                 + fileIndex*m_headInfoForBackup.fileRecSecNum*sizeof(SECTION_INFO_EX)
						 + m_recStatusForBackup.section_index*sizeof(SECTION_INFO_EX);

	//开始时间写前面四个字节，结束时间写后面四个字节。
	offset += (bStart ? 0 : sizeof(unsigned long));

	if (0 == fseek(m_pFileForBackup, offset, SEEK_SET))
	{
		if (1 == fwrite(&time, sizeof(unsigned long), 1, m_pFileForBackup))
		{
			if (!bStart)
			{
				//结束时，指向下一条记录索引处。
				++ m_recStatusForBackup.section_index;
			}

			return true;
		}
	}

	return false;


}







bool CReclogEx::WriteFileInfoForBackup(unsigned long fileIndex, FILE_FILE_INFO *pFileInfo)
{
	if (0 == fseek(m_pFileForBackup, m_headInfoForBackup.fileInfoOffset+fileIndex*sizeof(FILE_FILE_INFO), SEEK_SET))
	{
		if (1 == fwrite(pFileInfo, sizeof(FILE_FILE_INFO), 1, m_pFileForBackup))
		{
			return true;
		}
	}

	return false;


}


bool CReclogEx::WriteOneFileAllInfo(unsigned short fileIndex, unsigned char *pFileHeadInfo, 
									unsigned char *pFileSectionInfo, unsigned char *pFileReclogInfo)
{
	assert (fileIndex < m_headInfoForBackup.fileNum);

	unsigned long offset = m_headInfoForBackup.fileInfoOffset + fileIndex*sizeof(FILE_FILE_INFO);	
	if (0 != fseek(m_pFileForBackup, offset, SEEK_SET))
	{
		return false;
	}	
	if (1 != fwrite(pFileHeadInfo, sizeof(FILE_FILE_INFO), 1, m_pFileForBackup))
	{		
		return false;
	}


	offset = m_headInfoForBackup.sectionOffset + fileIndex*m_headInfoForBackup.fileRecSecNum*sizeof(SECTION_INFO_EX);
	if (0 != fseek(m_pFileForBackup, offset, SEEK_SET))
	{
		return false;
	}	
	if (m_headInfoForBackup.fileRecSecNum != fwrite(pFileSectionInfo, sizeof(SECTION_INFO_EX), m_headInfoForBackup.fileRecSecNum, m_pFileForBackup))
	{
		return false;
	}


	offset = m_headInfoForBackup.reclogoffset + fileIndex*m_headInfoForBackup.fileLogNum*sizeof(FILE_RECLOG_INFO);
	if (0 != fseek(m_pFileForBackup, offset, SEEK_SET))
	{
		return false;
	}
	if (m_headInfoForBackup.fileLogNum != fwrite(pFileReclogInfo, sizeof(FILE_RECLOG_INFO), m_headInfoForBackup.fileLogNum, m_pFileForBackup))
	{
		return false;
	}

	return true;

}

bool CReclogEx::RepairForBackup(unsigned char *pFileHeadInfo, unsigned char *pFileSectionInfo, 
								unsigned char *pFileReclogInfo, FILE_INFO_EX fileInfo)
{

	bool bNeedRepair = false;
	//修复段信息
	{
		SECTION_INFO_EX *pSection = (SECTION_INFO_EX *)pFileSectionInfo;
		for(int secIndex = 0; secIndex < m_headInfoForBackup.fileRecSecNum; secIndex++)
		{
			if((0 == pSection[secIndex].startTime) || (pSection[secIndex].startTime < fileInfo.startTime))
			{
				break;
			}
			else if(0 == pSection[secIndex].endTime)
			{
				pSection[secIndex].endTime = fileInfo.endTime;
				bNeedRepair = true;
				break;
			}
		}		
	}

	//修复事件信息
	if (bNeedRepair)
	{
		FILE_RECLOG_INFO *pReclog = (FILE_RECLOG_INFO *)pFileReclogInfo;
		unsigned long reclogIndex = 0;

		//检查录像事件部分
		unsigned char recTypeNum = RecTypeNum();
		assert (recTypeNum > 0);
		FILE_RECLOG_INFO  *pTempReclog = new FILE_RECLOG_INFO[recTypeNum];
		memset(pTempReclog, 0, sizeof(FILE_RECLOG_INFO)*recTypeNum);

		for (unsigned long i=0; i<m_headInfoForBackup.fileLogNum; ++i)
		{
			//表示开始
			if ((RECORD_TYPE_NULL != pReclog[i].type) && (pReclog[i].type < RECORD_TYPE_END) && (0 == (pReclog[i].type & RECLOG_END)))
			{
				pTempReclog [RecTypeIndex(static_cast<RECORD_TYPE>(pReclog[i].type))] = pReclog[i];
			}

			//表示结束，且时间不为零。
			if ((RECORD_TYPE_NULL != pReclog[i].type) && ((pReclog[i].type - RECLOG_END) < RECORD_TYPE_END) &&\
				(0 < pReclog[i].time) && (pReclog[i].type & RECLOG_END))
			{
				memset(&pTempReclog [RecTypeIndex(static_cast<RECORD_TYPE>(pReclog[i].type))], 0, sizeof(FILE_RECLOG_INFO));
			}

			//表示该预留空间未使用
			if ((RECORD_TYPE_NULL == pReclog[i].type) && (0 == pReclog[i].time))
			{
				break;
			}

			++ reclogIndex;
		}

		for (unsigned char i=0; i<recTypeNum; ++i)
		{
			if ((RECORD_TYPE_NULL != pTempReclog[i].type) && (0 < pTempReclog[i].time))
			{
				//不能越界
				if (reclogIndex < m_headInfoForBackup.fileLogNum)
				{
					pReclog[reclogIndex].type = (pTempReclog[i].type | RECLOG_END);
					pReclog[reclogIndex].time = fileInfo.endTime;

					++ reclogIndex;
				}
				else
				{
					break;
				}
			}
		}	

		delete [] pTempReclog;
		pTempReclog = NULL;
	}

	//修复文件信息
	FILE_FILE_INFO *pInfo = (FILE_FILE_INFO *)pFileHeadInfo;
	if(bNeedRepair)
	{		
		pInfo->endTime = fileInfo.endTime;		
	}
	pInfo->status = FILE_STATUS_END;
	return true;	
}


////////////////////////////////////////////////////////////////////////////////////////////////
unsigned long CReclogEx::GetFreeFileNum()
{
	return m_invalidFileList.GetCount();
}

unsigned long CReclogEx::GetChannelNum(bool bIsBackUpCreated/* = false*/)
{
	if(!bIsBackUpCreated)
	{
		return m_headInfo.chnnNum;
	}
	else
	{
		unsigned char netVideoInputNum = CProduct::Instance()->NetVideoInputNum();
		unsigned char localVideoInputNum = CProduct::Instance()->LocalVideoInputNum();
		return (localVideoInputNum+netVideoInputNum);
	}
	
}

bool CReclogEx::IsBeWoring()
{
	assert(m_pChannelRecordFile != NULL);
	assert(m_pChannelReadFile != NULL);
    
	bool bRet = false;
	for (int chnn = 0; chnn < m_headInfo.chnnNum; ++chnn)
	{
		if (m_pChannelRecordFile[chnn].pFileList->GetCount() > 0)
		{
			bRet = true;
			break;
		}
		if (m_pChannelReadFile[chnn].pFileList->GetCount() > 0)
		{
			bRet = true;
			break;
		}
	}
	
	return bRet;
}

bool CReclogEx::IsBeReading()
{
	assert(m_pChannelReadFile != NULL);

	bool bRet = false;
	for (int chnn = 0; chnn < m_headInfo.chnnNum; ++chnn)
	{		
		if (m_pChannelReadFile[chnn].pFileList->GetCount() > 0)
		{
			bRet = true;
			break;
		}
	}

	return bRet;
}










/////////////////////////////////////////////////////////////////////////////////////////////
