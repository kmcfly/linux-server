/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : 袁石维
** Date         : 2010-01-19
** Name         : RunLog.cpp
** Version      : 1.0
** Description  :
** Modify Record:
1:
***********************************************************************/
#include "RunLog.h"
#include <assert.h>
#include "RunLogItemDef.h"
//////////////////////////////////////////////////////////////////////////
//2010-01-31 18:48:00 YSW
//原来把这里的锁和缓冲区都定义为全局静态变量，但是在Linux环境下，总是出现不能解释的问题
//以为在一个缓冲区中拼接字符串然后写入文件，但是实际上总是只写了最开始拼接进去的一串，其他的
//似乎是一个临时成员中操作后就没了。
//改为目前这种作为类的成员方式后，一切都正常了。
CPUB_Lock CRunLog::m_lock;
char CRunLog::m_buf [2048] = {0};

void CRunLog::RUN_Time(const char *pStr, unsigned long t)
{
	assert (NULL != pStr);
	strcat(CRunLog::m_buf, "\t\t\t");
	strcat(CRunLog::m_buf, pStr);
	tm time = DVRTime32ToTm(t);
	sprintf(CRunLog::m_buf+strlen(CRunLog::m_buf), "%d-%02d-%02d %02d:%02d:%02d", time.tm_year+1900, time.tm_mon+1, time.tm_mday, time.tm_hour, time.tm_min, time.tm_sec);
	strcat(CRunLog::m_buf, "\r\n");
}

void CRunLog::RUN_Num(const char *pStr, unsigned long num)
{
	assert (NULL != pStr);
	strcat(CRunLog::m_buf, "\t\t\t");
	strcat(CRunLog::m_buf, pStr);
	sprintf(CRunLog::m_buf+strlen(CRunLog::m_buf), "%d", num);
	strcat(CRunLog::m_buf, "\r\n");
}
//////////////////////////////////////////////////////////////////////////
CRunLog * CRunLog::Instance()
{
	static CRunLog s_RunLog;
	return &s_RunLog;
}

CRunLog::~CRunLog()
{

}

bool CRunLog::Initial(DISK_LOG_INFO *pDiskStatus, unsigned char diskNum, const char *pFilePath)
{
	CRunLog::m_lock.Lock();

	//统计可以写日志的分区
	unsigned char diskValidNum = 0;
	for (unsigned char i = 0; i < diskNum; ++i)
	{
		if ((NET_DISK_OWNED_BY_THIS == pDiskStatus[i].diskOwnedBy) \
		  && (NET_DISK_READ_WRITE == pDiskStatus[i].diskProperty))
		{
			++ diskValidNum;
		}
	}

	if (0 == diskValidNum)
	{
		CRunLog::m_lock.UnLock();
		return false;
	}

	//保存可以写日志的分区
	assert(NULL == m_pVaildDisk);
	m_pVaildDisk = new unsigned long [diskValidNum];
	unsigned char count = 0;
	for (unsigned char i = 0; i < diskNum; ++i)
	{
		if ((NET_DISK_OWNED_BY_THIS == pDiskStatus[i].diskOwnedBy) \
			&& (NET_DISK_READ_WRITE == pDiskStatus[i].diskProperty))
		{
			m_pVaildDisk[count] = pDiskStatus[i].diskIndex;
			count++;
		}
	}
	assert(count == diskValidNum);

	//2010-01-31 14:51:00 YSW
	//如果初始化失败，应该把一些资源释放，并返回未初始化状态
	if (!Initial(pFilePath, diskValidNum))
	{
		delete [] m_pVaildDisk;
		m_pVaildDisk = NULL;

		CRunLog::m_lock.UnLock();
		return false;
	}
	else
	{
		CRunLog::m_lock.UnLock();
		return true;
	}
}

bool CRunLog::Initial(const char *pFilePath, unsigned char diskNum)
{
	{
		m_fileInfoPos	= strlen(LOG_FILE_XML_HEADER) + strlen("\r\n") + strlen(RUN_LOG_TAG_HEAD) + strlen("\r\n");
		m_logTagEndPos	= strlen(RUN_LOG_TAG_END) + strlen("\r\n");
	}

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

		m_diskNum = diskNum;		
	}

	char filePath [512] = {0};
	m_diskIndex = 0;
	do
	{
		m_fileIndex = 0;

		do 
		{
			//处理文件在不同环境下的路径
			{
#ifdef __ENVIRONMENT_WIN32__
				if (1 < strlen(m_pFilePath))
				{
					sprintf(filePath, "%s\\%02d\\%04d.xml", m_pFilePath, m_pVaildDisk[m_diskIndex], m_fileIndex);
				}
				else
				{
					assert (1 == strlen(m_pFilePath));
					sprintf(filePath, "%c:\\%04d.xml", m_pFilePath[0]+m_pVaildDisk[m_diskIndex], m_fileIndex);
				}
#else
				sprintf(filePath, "%s/%02d/%04d.xml", m_pFilePath, m_pVaildDisk[m_diskIndex], m_fileIndex);
#endif
			}

			//尝试打开文件，如果没有则创建一个新文件。
			//如果能打开，则检查已经写入的记录数目，超过或者等于RUN_LOG_MAX_LOG_NUM
			//则关闭该文件，再尝试打开下一个文件。依次类推到RUN_LOG_MAX_FILE_NUM。
			//都不成功，则跳下下一个磁盘分区，继续尝试。
			//如果所有遍历后都不能成功打开一个可用文件，则需要从头开始覆盖。
			{
				fstream file(filePath, ios_base::in | ios_base::binary);
				if (file.is_open())
				{
					if (GetFileInfo(file, m_logNum, m_startTime, m_endTime))
					{
						//记录的条数没有达到最大数目，说明该文件可用。
						if (m_logNum < RUN_LOG_MAX_LOG_NUM)
						{
							m_file.open(filePath, ios_base::in | ios_base::out | ios_base::ate | ios_base::binary);
							if (m_file.is_open())
							{
								file.close();
								return true;
							}
							else
							{
								file.close();

								//释放资源
								delete [] m_pFilePath;
								m_pFilePath = NULL;
								m_diskNum	= 0;
								m_logNum	= 0;

								assert (false);
								return false;
							}
						}
					}
				}
				else
				{
					//如果没有打开，则意味着还没有创建出来，应该跳出循环了，紧接着序号创建新文件。
					break;
				}
			}

			++ m_fileIndex;
		} while(m_fileIndex < RUN_LOG_MAX_FILE_NUM);
		
		if (m_fileIndex < RUN_LOG_MAX_FILE_NUM)
		{
			//意味着提前跳出内部循环，也需要跳出外部循环。
			break;
		}

		++ m_diskIndex;
	}while (m_diskIndex < m_diskNum);

	//没有找到可用文件，则从头开始覆盖
	if ((RUN_LOG_MAX_FILE_NUM == m_fileIndex) || (m_diskNum == m_diskIndex))
	{
		m_diskIndex = 0;
		m_fileIndex = 0;
	}
	

	//处理文件在不同环境下的路径
	{
#ifdef __ENVIRONMENT_WIN32__
		if (1 < strlen(m_pFilePath))
		{
			sprintf(filePath, "%s\\%02d\\%04d.xml", m_pFilePath, m_pVaildDisk[m_diskIndex], m_fileIndex);
		}
		else
		{
			assert (1 == strlen(m_pFilePath));
			sprintf(filePath, "%c:\\%04d.xml", m_pFilePath[0]+m_pVaildDisk[m_diskIndex], m_fileIndex);
		}
#else
		sprintf(filePath, "%s/%02d/%04d.xml", m_pFilePath, m_pVaildDisk[m_diskIndex], m_fileIndex);
#endif
	}

	{
		m_file.open(filePath, ios_base::in | ios_base::out | ios_base::binary | ios_base::trunc);
		if (m_file.is_open())
		{
			//写XML文件头
			m_file << LOG_FILE_XML_HEADER << "\r\n";
			m_file << RUN_LOG_TAG_HEAD << "\r\n";

			WriteFileInfo(m_file, 0, 0, 0);

			m_file << RUN_LOG_TAG_END << "\r\n";

			m_file.flush();

			return true;
		}
		else
		{
			//2010-01-31 14:51:00 YSW
			//如果初始化失败，应该把一些资源释放，并返回未初始化状态
			delete [] m_pFilePath;
			m_pFilePath = NULL;
			m_diskNum	= 0;
			m_logNum	= 0;

			return false;
		}
	}
}

void CRunLog::Quit()
{
	CRunLog::m_lock.Lock();

	//2010-01-31 19:00:00 YSW
	//需要判断是否是打开状态，否则带来问题
	//当没有打开却调用了close，会导致该对象重新打开一个文件时，不能成功写入内容。
	if (m_file.is_open())
	{
		m_file.close();
	}

	if (NULL != m_pFilePath)
	{
		delete [] m_pFilePath;
		m_pFilePath = NULL;
	}

	if (NULL != m_pVaildDisk)
	{
		delete [] m_pVaildDisk;
		m_pVaildDisk = NULL;
	}

	m_fileInfoPos	= 0;
	m_logTagEndPos	= 0;

	m_diskIndex = 0;
	m_fileIndex = 0;
	m_diskNum	= 0;
	m_logNum	= 0;
	m_startTime	= 0;
	m_endTime	= 0;

	CRunLog::m_lock.UnLock();
}

bool CRunLog::WriteLog(unsigned long itemID, const char *pInfo, const char *pFile, unsigned long line)
{
	if (RUN_LOG_MAX_LOG_NUM <= m_logNum)
	{
		//没有找到可用文件，则从头开始覆盖
		++ m_fileIndex;
		if (RUN_LOG_MAX_FILE_NUM <= m_fileIndex)
		{
			++ m_diskIndex;
			m_fileIndex = 0;

			if (m_diskNum <= m_diskIndex)
			{
				m_diskIndex = 0;
			}
		}

		//
		m_logNum = 0;//数目归零

		char filePath [512] = {0};
		//处理文件在不同环境下的路径
		{
#ifdef __ENVIRONMENT_WIN32__
			if (1 < strlen(m_pFilePath))
			{
				sprintf(filePath, "%s\\%02d\\%04d.xml", m_pFilePath, m_pVaildDisk[m_diskIndex], m_fileIndex);
			}
			else
			{
				assert (1 == strlen(m_pFilePath));
				sprintf(filePath, "%c:\\%04d.xml", m_pFilePath[0]+m_pVaildDisk[m_diskIndex], m_fileIndex);
			}
#else
			sprintf(filePath, "%s/%02d/%04d.xml", m_pFilePath, m_pVaildDisk[m_diskIndex], m_fileIndex);
#endif
		}

		//首先关闭原来的文件
		m_file.close();

		{
			m_file.open(filePath, ios_base::in | ios_base::out | ios_base::binary | ios_base::trunc);
			if (!m_file.is_open())
			{
				return false;
			}

			//写XML文件头
			m_file << LOG_FILE_XML_HEADER << "\r\n";
			m_file << RUN_LOG_TAG_HEAD << "\r\n";

			WriteFileInfo(m_file, 0, 0, 0);

			m_file << RUN_LOG_TAG_END << "\r\n";

			m_file.flush();
		}
	}

	if (m_file.is_open())
	{
		tm time = DVRTime32ToTm(GetCurrTime32());
		char tt [32] = {0};
		sprintf(tt, "%d-%02d-%02d %02d:%02d:%02d", time.tm_year+1900, time.tm_mon+1, time.tm_mday, time.tm_hour, time.tm_min, time.tm_sec);
		char type [128] = {0};
		if (RUN_LOG_ITEM_PB_BASE & itemID)
		{
			strcpy(type, "Read Data");
		}
		else if (RUN_LOG_ITEM_REC_BASE & itemID)
		{
			strcpy(type, "Record");
		}
		else if (RUN_LOG_ITEM_BK_BASE & itemID)
		{
			strcpy(type, "Backup");
		}
		else
		{
			strcpy(type, "Otherf");
		}

		m_file.seekp(-m_logTagEndPos, ios::end);

		m_file << "\t" << RUN_LOG_LOG_TAG_HEAD << "\r\n";

		m_file << "\t\t" << RUN_LOG_TYPE_TAG_HEAD << type << RUN_LOG_TYPE_TAG_END << "\r\n";
		m_file << "\t\t" << RUN_LOG_ID_TAG_HEAD << hex << itemID << RUN_LOG_ID_TAG_END << "\r\n";
		m_file << "\t\t" << RUN_LOG_INFO_TAG_HEAD << pInfo << RUN_LOG_INFO_TAG_END << "\r\n";
		m_file << "\t\t" << RUN_LOG_TIME_TAG_HEAD << tt << RUN_LOG_TIME_TAG_END << "\r\n";
		m_file << "\t\t" << RUN_LOG_FILE_TAG_HEAD << pFile << RUN_LOG_FILE_TAG_END << "\r\n";
		m_file << "\t\t" << RUN_LOG_LINE_TAG_HEAD << dec << line << RUN_LOG_LINE_TAG_END << "\r\n";

		m_file << "\t" << RUN_LOG_LOG_TAG_END << "\r\n";

		m_file << RUN_LOG_TAG_END << "\r\n";

		++ m_logNum;
		m_endTime = GetCurrTime32();

		WriteFileInfo(m_file, m_logNum, m_startTime, m_endTime);

		m_file.flush();
		return true;
	}
	else
	{
		return false;
	}	
}

void CRunLog::GetUsingPartitionMask(ULONGLONG &heightMask, ULONGLONG &lowMask)
{
	CRunLog::m_lock.Lock();

	if ((NULL == m_pVaildDisk) || (0 == m_diskNum))
	{ 
		CRunLog::m_lock.UnLock();
		return;
	}

	if (!m_file.is_open())
	{
		CRunLog::m_lock.UnLock();
		return;
	}

	assert(m_diskIndex < m_diskNum);
	ULONGLONG lRet = 0;
	ULONGLONG hRet = 0;
    unsigned long disk = m_pVaildDisk[m_diskIndex];
	assert(disk >= 0);
	assert(disk < 128);
	if (disk < 64)
	{
		lRet |= (((ULONGLONG)1) << (disk));
		hRet |= 0;
	}
	else
	{
		lRet |= 0;
		hRet |= (((ULONGLONG)1) << (disk));
	}
	lowMask    |= lRet;
	heightMask |= hRet;

	CRunLog::m_lock.UnLock();
	return;
}

//////////////////////////////////////////////////////////////////////////
CRunLog::CRunLog() : m_pFilePath(NULL), m_fileInfoPos(0), m_logTagEndPos(0), m_diskIndex(0), m_fileIndex(0), m_diskNum(0), m_logNum(0), m_startTime(0), m_endTime(0)
{
	m_pVaildDisk = NULL;
}

bool CRunLog::GetFileInfo(fstream & file, unsigned long & logNum, unsigned long & startTime, unsigned long & endTime)
{
	string str;
	if (getline(file, str))
	{
		if (LOG_FILE_XML_HEADER != str.substr(0, strlen(LOG_FILE_XML_HEADER)))
		{
			logNum = 0, startTime = 0, endTime = 0;
			return false;
		}
		else
		{
			unsigned char succNum = 0;
			do 
			{
				if(getline(file, str))
				{
					//num
					if (RUN_LOG_NUM_TAG_HEAD == str.substr(strlen("\t\t"), strlen(RUN_LOG_NUM_TAG_HEAD)))
					{
						if (EraseTag(str))
						{
							m_logNum = static_cast<unsigned long>(atoi(str.data()));
						}

						++ succNum;
						continue;
					}

					//start time
					if (RUN_LOG_START_TIME_TAG_HEAD == str.substr(strlen("\t\t"), strlen(RUN_LOG_START_TIME_TAG_HEAD)))
					{
						if (EraseTag(str))
						{
							m_startTime = Time(str);
							//PrintfBit32Time(m_startTime);
						}

						++ succNum;
						continue;
					}

					//end time
					if (RUN_LOG_END_TIME_TAG_HEAD == str.substr(strlen("\t\t"), strlen(RUN_LOG_END_TIME_TAG_HEAD)))
					{
						if (EraseTag(str))
						{
							m_endTime = Time(str);
							//PrintfBit32Time(m_startTime);
						}

						++ succNum;
						continue;
					}

					if (RUN_LOG_TAG_END == str.substr(0, strlen(RUN_LOG_TAG_END)))
					{
						logNum = 0, startTime = 0, endTime = 0;
						return false;
					}
				}
				else
				{
					logNum = 0, startTime = 0, endTime = 0;
					return false;
				}
			} while(RUN_LOG_FILE_INFO_TAG_END != str.substr(strlen("\t"), strlen(RUN_LOG_FILE_INFO_TAG_END)));

			return (3 == succNum) ? true : false;
		}
	}
	else
	{
		logNum = 0, startTime = 0, endTime = 0;
		return false;
	}	
}

void CRunLog::WriteFileInfo(fstream & file, unsigned long logNum, unsigned long startTime, unsigned long endTime)
{
	char num [8] = {0};
	char sTime [32] = {0};
	char eTime [32] = {0};

	{
		if (0 == startTime)
		{
			startTime = GetCurrTime32();
		}

		if (0 == endTime)
		{
			endTime = GetCurrTime32();
		}

		sprintf(num, "%04d", logNum);

		tm time;
		time = DVRTime32ToTm(startTime);
		sprintf(sTime, "%d-%02d-%02d %02d:%02d:%02d", time.tm_year+1900, time.tm_mon+1, time.tm_mday, time.tm_hour, time.tm_min, time.tm_sec);
		time = DVRTime32ToTm(endTime);
		sprintf(eTime, "%d-%02d-%02d %02d:%02d:%02d", time.tm_year+1900, time.tm_mon+1, time.tm_mday, time.tm_hour, time.tm_min, time.tm_sec);

	}

	{
		file.seekp(m_fileInfoPos, ios::beg);

		file << "\t" << RUN_LOG_FILE_INFO_TAG_HEAD << "\r\n";
		file << "\t\t" << RUN_LOG_NUM_TAG_HEAD << num << RUN_LOG_NUM_TAG_END << "\r\n";
		file << "\t\t" << RUN_LOG_START_TIME_TAG_HEAD << sTime << RUN_LOG_START_TIME_TAG_END << "\r\n";
		file << "\t\t" << RUN_LOG_END_TIME_TAG_HEAD << eTime << RUN_LOG_END_TIME_TAG_END << "\r\n";
		file << "\t" << RUN_LOG_FILE_INFO_TAG_END << "\r\n";
	}
}

bool CRunLog::EraseTag(string & str)
{
	while ('>' != str[0])
	{
		str.erase(0, 1);
	}

	str.erase(0, 1);

	while ('<' != str[str.length() - 1])
	{
		str.erase(str.length() - 1, 1);
	}

	str.erase(str.length() - 1, 1);

	return (str.length() > 0) ? true : false;
}

unsigned long CRunLog::Time(string & str)
{
	tm time;
	memset(&time, 0, sizeof(tm));

	time.tm_year = static_cast<int>(atoi(str.data())) - 1900;

	time.tm_mon = static_cast<int>(atoi(str.substr(strlen("2010-"), 2).data())) - 1;

	time.tm_mday = static_cast<int>(atoi(str.substr(strlen("2010-01-"), 2).data()));

	time.tm_hour = static_cast<int>(atoi(str.substr(strlen("2010-01-19 "), 2).data()));

	time.tm_min = static_cast<int>(atoi(str.substr(strlen("2010-01-19 16:"), 2).data()));

	time.tm_sec = static_cast<int>(atoi(str.substr(strlen("2010-01-19 16:04:"), 2).data()));

	return TmToDVRTime32(time);
}
//////////////////////////////////////////////////////////////////////////
