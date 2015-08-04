/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : Ԭʯά
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
//ԭ������������ͻ�����������Ϊȫ�־�̬������������Linux�����£����ǳ��ֲ��ܽ��͵�����
//��Ϊ��һ����������ƴ���ַ���Ȼ��д���ļ�������ʵ��������ֻд���ʼƴ�ӽ�ȥ��һ����������
//�ƺ���һ����ʱ��Ա�в������û�ˡ�
//��ΪĿǰ������Ϊ��ĳ�Ա��ʽ��һ�ж������ˡ�
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

	//ͳ�ƿ���д��־�ķ���
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

	//�������д��־�ķ���
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
	//�����ʼ��ʧ�ܣ�Ӧ�ð�һЩ��Դ�ͷţ�������δ��ʼ��״̬
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

	//����·���ʹ�����Ŀ
	{
		assert (NULL == m_pFilePath);	//�����ظ���ʼ��

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
			//�����ļ��ڲ�ͬ�����µ�·��
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

			//���Դ��ļ������û���򴴽�һ�����ļ���
			//����ܴ򿪣������Ѿ�д��ļ�¼��Ŀ���������ߵ���RUN_LOG_MAX_LOG_NUM
			//��رո��ļ����ٳ��Դ���һ���ļ����������Ƶ�RUN_LOG_MAX_FILE_NUM��
			//�����ɹ�����������һ�����̷������������ԡ�
			//������б����󶼲��ܳɹ���һ�������ļ�������Ҫ��ͷ��ʼ���ǡ�
			{
				fstream file(filePath, ios_base::in | ios_base::binary);
				if (file.is_open())
				{
					if (GetFileInfo(file, m_logNum, m_startTime, m_endTime))
					{
						//��¼������û�дﵽ�����Ŀ��˵�����ļ����á�
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

								//�ͷ���Դ
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
					//���û�д򿪣�����ζ�Ż�û�д���������Ӧ������ѭ���ˣ���������Ŵ������ļ���
					break;
				}
			}

			++ m_fileIndex;
		} while(m_fileIndex < RUN_LOG_MAX_FILE_NUM);
		
		if (m_fileIndex < RUN_LOG_MAX_FILE_NUM)
		{
			//��ζ����ǰ�����ڲ�ѭ����Ҳ��Ҫ�����ⲿѭ����
			break;
		}

		++ m_diskIndex;
	}while (m_diskIndex < m_diskNum);

	//û���ҵ������ļ������ͷ��ʼ����
	if ((RUN_LOG_MAX_FILE_NUM == m_fileIndex) || (m_diskNum == m_diskIndex))
	{
		m_diskIndex = 0;
		m_fileIndex = 0;
	}
	

	//�����ļ��ڲ�ͬ�����µ�·��
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
			//дXML�ļ�ͷ
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
			//�����ʼ��ʧ�ܣ�Ӧ�ð�һЩ��Դ�ͷţ�������δ��ʼ��״̬
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
	//��Ҫ�ж��Ƿ��Ǵ�״̬�������������
	//��û�д�ȴ������close���ᵼ�¸ö������´�һ���ļ�ʱ�����ܳɹ�д�����ݡ�
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
		//û���ҵ������ļ������ͷ��ʼ����
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
		m_logNum = 0;//��Ŀ����

		char filePath [512] = {0};
		//�����ļ��ڲ�ͬ�����µ�·��
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

		//���ȹر�ԭ�����ļ�
		m_file.close();

		{
			m_file.open(filePath, ios_base::in | ios_base::out | ios_base::binary | ios_base::trunc);
			if (!m_file.is_open())
			{
				return false;
			}

			//дXML�ļ�ͷ
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
