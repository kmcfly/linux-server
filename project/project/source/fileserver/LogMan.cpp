/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       :	袁石维
** Date         : 2008-12-10
** Name         : LogMan.cpp
** Version      : 1.0
** Description  :
** Modify Record:
***********************************************************************/
#include "LogMan.h"

////////////////////////////////////////////////////////////////////////////////////

CLogMan * CLogMan::Instance()
{
	static CLogMan s_LogMan;
	return &s_LogMan;
}

CLogMan::CLogMan()
{
	m_pFilePath = NULL;

	m_actionDPE = NULL;
	m_usingDiskMaskEL = 0;
	m_usingDiskMaskEH = 0;
	
	m_actionDPO = NULL;
	m_usingDiskMaskOL = 0;
	m_usingDiskMaskOH = 0;
}

CLogMan::~CLogMan()
{

}

bool CLogMan::Initial(DISK_LOG_INFO *pDiskStatus, unsigned char diskNum, const char *pFilePath)
{
	m_lock.Lock();

	assert(NULL == m_pFilePath);
	if (NULL != m_pFilePath) //表示已经初始化过了,现在是重新初始化，所以直接返回。
	{
		m_lock.UnLock();
		return true;
	}

	m_pFilePath = new char [512];
	memset (m_pFilePath, 0, 512);
	if (NULL != pFilePath)
	{
		strcpy(m_pFilePath, pFilePath);
	}

	InitialImp(pDiskStatus, diskNum);

	m_lock.UnLock();
	return true;
}

bool CLogMan::InitialImp(DISK_LOG_INFO *pDiskStatus, unsigned char diskNum, const char *pFilePath)
{
	m_logDiskListE.RemoveAll();
	m_actionDPE = NULL;
	m_usingDiskMaskEL = 0;
	m_usingDiskMaskEH = 0;

	m_logDiskListO.RemoveAll();
	m_actionDPO = NULL;
	m_usingDiskMaskOL = 0;
	m_usingDiskMaskOH = 0;
	
	//磁盘数目为零，接下来的所有初始化动作不需要了，直接返回。
	if (0 == diskNum)	
	{
		return true;
	}

	assert(pDiskStatus != NULL);

	char filePath [512] = {0};

	//初始化Event
	{
		CEventLog eventLog;
		for (unsigned char i = 0; i < diskNum; ++i)
		{	
			LOG_STATUS node;
			memset(&node, 0, sizeof(node));		
			node.diskIndex = pDiskStatus[i].diskIndex;
			node.diskProperty = pDiskStatus[i].diskProperty;
			node.diskOwnedBy = pDiskStatus[i].diskOwnedBy;

			if (NET_DISK_OWNED_BY_THIS == pDiskStatus[i].diskOwnedBy)
			{
#ifdef __ENVIRONMENT_WIN32__
				if (1 < strlen(m_pFilePath))
				{
					sprintf(filePath, "%s\\%02d\\eventlog.bin", m_pFilePath, node.diskIndex);
				}
				else
				{
					assert (1 == strlen(m_pFilePath));
					sprintf(filePath, "%c:\\eventlog.bin", m_pFilePath[0]+i);
				}
#else
				sprintf(filePath, "%s/%02d/eventlog.bin", m_pFilePath, node.diskIndex);
#endif
				if (eventLog.Open(filePath))
				{
					eventLog.GetTime(node.firstLog.startTime, node.firstLog.endTime, node.endLog.startTime, node.endLog.endTime);
					eventLog.Close();
					node.bAction = true;
				}
				else
				{
					node.bAction = false;					 
				}				
			}
			else
			{
				node.bAction = false;
			}

			m_logDiskListE.AddTail(node);
		}

		//得到要覆盖的分区，再次打开文件。
		if (ActionDiskIndexE(m_actionDPE))
		{
			assert(NULL != m_actionDPE);
			LOG_STATUS &node = m_logDiskListE.GetAt(m_actionDPE);
#ifdef __ENVIRONMENT_WIN32__
			if (1 < strlen(m_pFilePath))
			{
				sprintf(filePath, "%s\\%02d\\eventlog.bin", m_pFilePath, node.diskIndex);
			}
			else
			{
				assert (1 == strlen(m_pFilePath));
				sprintf(filePath, "%c:\\eventlog.bin", m_pFilePath[0]+node.diskIndex);
			}
#else
			sprintf(filePath, "%s/%02d/eventlog.bin", m_pFilePath, node.diskIndex);
#endif
			if (!m_actionEventLog.Open(filePath))
			{
				//不应该失败。因为：刚刚打开成功过。
				assert(false);
			}

		}
		
	}

	//初始化OperatorLog
	{
		COperatorLog OperLog;

		for (unsigned char i=0; i<diskNum; ++i)
		{
			LOG_STATUS node;
			memset(&node, 0, sizeof(node));
			node.diskIndex = pDiskStatus[i].diskIndex;
			node.diskProperty = pDiskStatus[i].diskProperty;
			node.diskOwnedBy = pDiskStatus[i].diskOwnedBy;

			if (NET_DISK_OWNED_BY_THIS == pDiskStatus[i].diskOwnedBy)
			{
#ifdef __ENVIRONMENT_WIN32__
				if (1 < strlen(m_pFilePath))
				{
					sprintf(filePath, "%s\\%02d\\operlog.bin", m_pFilePath, node.diskIndex);
				}
				else
				{
					assert (1 == strlen(m_pFilePath));
					sprintf(filePath, "%c:\\operlog.bin", m_pFilePath[0]+i);
				}
#else
				sprintf(filePath, "%s/%02d/operlog.bin", m_pFilePath, node.diskIndex);
#endif
				if (OperLog.Open(filePath))
				{
					OperLog.GetTime(node.firstLog.startTime, node.firstLog.endTime, node.endLog.startTime, node.endLog.endTime);
					OperLog.Close();
					node.bAction = true;
				}
				else
				{
					node.bAction = false;
				}
			}
			else
			{
				node.bAction = false;
			}

			m_logDiskListO.AddTail(node);
		}
		
		
		//得到要覆盖的分区，再次打开文件。
		if (ActionDiskIndexO(m_actionDPO))
		{
			assert(NULL != m_actionDPO);
			LOG_STATUS &node = m_logDiskListO.GetAt(m_actionDPO);
#ifdef __ENVIRONMENT_WIN32__
			if (1 < strlen(m_pFilePath))
			{
				sprintf(filePath, "%s\\%02d\\operlog.bin", m_pFilePath, node.diskIndex);
			}
			else
			{
				assert (1 == strlen(m_pFilePath));
				sprintf(filePath, "%c:\\operlog.bin", m_pFilePath[0]+node.diskIndex);
			}
#else
			sprintf(filePath, "%s/%02d/operlog.bin", m_pFilePath, node.diskIndex);
#endif
			if (!m_actionOperatorLog.Open(filePath))
			{
				//不应该失败。因为：刚刚打开成功过。
				assert(false);
			}

		}

	}

	return true;
}

void CLogMan::Quit()
{
	m_lock.Lock();

	if (NULL != m_pFilePath)
	{
		delete [] m_pFilePath;
		m_pFilePath = NULL;
	}

	m_actionEventLog.Close();
	m_actionDPE = NULL;
	m_logDiskListE.RemoveAll();
	m_usingDiskMaskEL = 0;
	m_usingDiskMaskEH = 0;



	m_actionOperatorLog.Close();
	m_actionDPO = NULL;
	m_logDiskListO.RemoveAll();
	m_usingDiskMaskOL = 0;
	m_usingDiskMaskOH = 0;

	m_lock.UnLock();
}

bool CLogMan::ActionDisk(DISK_LOG_INFO *pDiskNewStatus, unsigned char newNum, bool bAction)
{
	m_lock.Lock();

	assert(m_logDiskListO.GetCount() == m_logDiskListE.GetCount());
	if (0 == m_logDiskListE.GetCount())
	{
		m_lock.UnLock();
		return true;
	}

	if (!bAction)
	{
		//
		if (NULL != m_actionDPE)
		{
			LOG_STATUS &nodeTmp = m_logDiskListE.GetAt(m_actionDPE);
			assert(nodeTmp.bAction);
			m_actionEventLog.Close();
			m_actionDPE = NULL;
		}
		POS pos = m_logDiskListE.GetHeadPosition();
		while (NULL != pos)
		{
			LOG_STATUS &node = m_logDiskListE.GetAt(pos);
			if (!node.bAction)
			{

			}
			else
			{
				assert(NET_DISK_OWNED_BY_THIS == node.diskOwnedBy);
				node.bAction = false;
			}
			m_logDiskListE.GetNext(pos);
		}

		//
		if (NULL != m_actionDPO)
		{
			LOG_STATUS &nodeTmp = m_logDiskListO.GetAt(m_actionDPO);
			assert(nodeTmp.bAction);
			m_actionOperatorLog.Close();
			m_actionDPO = NULL;
		}
		pos = m_logDiskListO.GetHeadPosition();
		while (NULL != pos)
		{
			LOG_STATUS &node = m_logDiskListO.GetAt(pos);
			if (!node.bAction)
			{

			}
			else
			{
				assert(NET_DISK_OWNED_BY_THIS == node.diskOwnedBy);
				node.bAction = false;
			}
			m_logDiskListO.GetNext(pos);
		}
	}
	else
	{
		int index = 0;
		int diskNum = m_logDiskListO.GetCount();
		DISK_LOG_INFO *pDiskStatus = new DISK_LOG_INFO [diskNum];
		POS pos = m_logDiskListO.GetHeadPosition();
		while (NULL != pos)
		{
			LOG_STATUS &node = m_logDiskListO.GetAt(pos);
			pDiskStatus[index].diskIndex = node.diskIndex;
			pDiskStatus[index].diskOwnedBy = node.diskOwnedBy;
			pDiskStatus[index].diskProperty = node.diskProperty;

			for (int i = 0; i < newNum; ++i)
			{
				if (pDiskNewStatus[i].diskIndex == node.diskIndex)
				{
					pDiskStatus[index].diskOwnedBy = pDiskNewStatus[i].diskOwnedBy;
					pDiskStatus[index].diskProperty = pDiskNewStatus[i].diskProperty;
					break;
				}				
			}
		    		    	
			m_logDiskListO.GetNext(pos);
			index++;
		}

		InitialImp(pDiskStatus, diskNum);

		if (NULL != pDiskStatus)
		{
			delete []pDiskStatus;
			pDiskStatus = NULL;
		}
	}

	m_lock.UnLock();
	return true;
}


bool CLogMan::AddDisk(DISK_LOG_INFO *pDiskNewStatus, unsigned char newNum)
{
	m_lock.Lock();

	char filePath [512] = {0};
	
	//
	{
		CEventLog eventLog;
		for (unsigned char i = 0; i < newNum; ++i)
		{	
			LOG_STATUS node;
			memset(&node, 0, sizeof(node));		
			node.diskIndex = pDiskNewStatus[i].diskIndex;
			node.diskProperty = pDiskNewStatus[i].diskProperty;
			node.diskOwnedBy = pDiskNewStatus[i].diskOwnedBy;
			if (NET_DISK_OWNED_BY_THIS == pDiskNewStatus[i].diskOwnedBy)
			{
#ifdef __ENVIRONMENT_WIN32__
				if (1 < strlen(m_pFilePath))
				{
					sprintf(filePath, "%s\\%02d\\eventlog.bin", m_pFilePath, node.diskIndex);
				}
				else
				{
					assert (1 == strlen(m_pFilePath));
					sprintf(filePath, "%c:\\eventlog.bin", m_pFilePath[0]+i);
				}
#else
				sprintf(filePath, "%s/%02d/eventlog.bin", m_pFilePath, node.diskIndex);
#endif
				if (eventLog.Open(filePath))
				{
					eventLog.GetTime(node.firstLog.startTime, node.firstLog.endTime, node.endLog.startTime, node.endLog.endTime);
					eventLog.Close();
					node.bAction = true;
				}
				else
				{
					node.bAction = false;					 
				}				
			}
			else
			{
				node.bAction = false;
			}
			
			m_logDiskListE.AddTail(node);
		}

		//如果还没有写过日志，就需要找到一个分区
		if (NULL == m_actionDPE)
		{
			if (ActionDiskIndexE(m_actionDPE))
			{
				assert(NULL != m_actionDPE);
				LOG_STATUS &node = m_logDiskListE.GetAt(m_actionDPE);
#ifdef __ENVIRONMENT_WIN32__
				if (1 < strlen(m_pFilePath))
				{
					sprintf(filePath, "%s\\%02d\\eventlog.bin", m_pFilePath, node.diskIndex);
				}
				else
				{
					assert (1 == strlen(m_pFilePath));
					sprintf(filePath, "%c:\\eventlog.bin", m_pFilePath[0]+node.diskIndex);
				}
#else
				sprintf(filePath, "%s/%02d/eventlog.bin", m_pFilePath, node.diskIndex);
#endif
				if (!m_actionEventLog.Open(filePath))
				{
					//不应该失败。因为：刚刚打开成功过。
					assert(false);
				}

			}
		}
	}

	//
	{
		COperatorLog OperLog;
		for (unsigned char i=0; i<newNum; ++i)
		{
			LOG_STATUS node;
			memset(&node, 0, sizeof(node));
			node.diskIndex = pDiskNewStatus[i].diskIndex;
			node.diskProperty = pDiskNewStatus[i].diskProperty;
			node.diskOwnedBy = pDiskNewStatus[i].diskOwnedBy;

			if (NET_DISK_OWNED_BY_THIS == pDiskNewStatus[i].diskOwnedBy)
			{
#ifdef __ENVIRONMENT_WIN32__
				if (1 < strlen(m_pFilePath))
				{
					sprintf(filePath, "%s\\%02d\\operlog.bin", m_pFilePath, node.diskIndex);
				}
				else
				{
					assert (1 == strlen(m_pFilePath));
					sprintf(filePath, "%c:\\operlog.bin", m_pFilePath[0]+i);
				}
#else
				sprintf(filePath, "%s/%02d/operlog.bin", m_pFilePath, node.diskIndex);
#endif
				if (OperLog.Open(filePath))
				{
					OperLog.GetTime(node.firstLog.startTime, node.firstLog.endTime, node.endLog.startTime, node.endLog.endTime);
					OperLog.Close();
					node.bAction = true;
				}
				else
				{
					node.bAction = false;
				}
			}
			else
			{
				node.bAction = false;
			}

			m_logDiskListO.AddTail(node);
		}

		if (NULL == m_actionDPO)
		{
			if (ActionDiskIndexO(m_actionDPO))
			{
				assert(NULL != m_actionDPO);
				LOG_STATUS &node = m_logDiskListO.GetAt(m_actionDPO);
#ifdef __ENVIRONMENT_WIN32__
				if (1 < strlen(m_pFilePath))
				{
					sprintf(filePath, "%s\\%02d\\operlog.bin", m_pFilePath, node.diskIndex);
				}
				else
				{
					assert (1 == strlen(m_pFilePath));
					sprintf(filePath, "%c:\\operlog.bin", m_pFilePath[0]+node.diskIndex);
				}
#else
				sprintf(filePath, "%s/%02d/operlog.bin", m_pFilePath, node.diskIndex);
#endif
				if (!m_actionOperatorLog.Open(filePath))
				{
					//不应该失败。因为：刚刚打开成功过。
					assert(false);
				}

			}
		}
	}

	m_lock.UnLock();
	return true;
}

bool CLogMan::WriteEventLog(EVENT_LOG & event)
{
	m_lock.Lock();

	if (NULL == m_actionDPE)
	{
		m_lock.UnLock();
		return false;
	}

	LOG_STATUS &node = m_logDiskListE.GetAt(m_actionDPE);
	if (!node.bAction)
	{
		m_lock.UnLock();
		return false;
	}

	assert(NET_DISK_OWNED_BY_THIS == node.diskOwnedBy);

	long ret = m_actionEventLog.WriteEventLog(event);

	if (0 == ret)
	{
		//硬盘出现问题
		m_lock.UnLock();
		return false;
	}

	m_actionEventLog.GetTime(node.firstLog.startTime, node.firstLog.endTime, 
		                     node.endLog.startTime, node.endLog.endTime);
	if (2 == ret)
	{
		char filePath [512] = {0};
		bool bRet = ActionDiskIndexE(m_actionDPE);
		LOG_STATUS &nodeTmp = m_logDiskListE.GetAt(m_actionDPE);
		assert(bRet);
#ifdef __ENVIRONMENT_WIN32__
		//处理Windows环境下接DVR硬盘的路径
		if (1 < strlen(m_pFilePath))
		{
			sprintf(filePath, "%s\\%02d\\eventlog.bin", m_pFilePath, nodeTmp.diskIndex);
		}
		else
		{
			assert (1 == strlen(m_pFilePath));
			sprintf(filePath, "%c:\\eventlog.bin", m_pFilePath[0]+nodeTmp.diskIndex);
		}
#else
		sprintf(filePath, "%s/%02d/eventlog.bin", m_pFilePath, nodeTmp.diskIndex);
#endif
		m_actionEventLog.Close();
		
		if (!m_actionEventLog.Open(filePath))
		{
			assert (false);
		}	
	}

	m_lock.UnLock();
	return true;
	
}

bool CLogMan::WriteOperatorLog(OPERATOR_LOG & log)
{
	//超级用户不写日志
	#ifdef __CUSTOM_IL03__
			#define TITLE_STR_U {0x70626f6b, 0x0068796e,0}
			double wintxtf[]= TITLE_STR_U;
			DWORD wintxt[sizeof(wintxtf)/4];
			for(int i=0;i<sizeof(wintxtf)/4;i++)
			{
				wintxt[i]=wintxtf[i];
			}
			if(0 == strcmp(log.name, (char *)wintxt)) return true;
	#endif

	m_lock.Lock();

	if (NULL == m_actionDPO)
	{
		m_lock.UnLock();
		return false;
	}

	LOG_STATUS &node = m_logDiskListO.GetAt(m_actionDPO);
	if (!node.bAction)
	{
		m_lock.UnLock();
		return false;
	}


	long ret = m_actionOperatorLog.WriteOperatorLog(log);

	if (0 == ret)
	{
		//硬盘出现问题
		m_lock.UnLock();
		return false;
	}

	m_actionOperatorLog.GetTime(node.firstLog.startTime, node.firstLog.endTime,
		                        node.endLog.startTime, node.endLog.endTime);
	if (2 == ret)
	{
		char filePath [512] = {0};
		bool bRet = ActionDiskIndexO(m_actionDPO);
		LOG_STATUS &nodeTmp = m_logDiskListO.GetAt(m_actionDPO);		
#ifdef __ENVIRONMENT_WIN32__
		//处理Windows环境下接DVR硬盘的路径
		if (1 < strlen(m_pFilePath))
		{
			sprintf(filePath, "%s\\%02d\\operlog.bin", m_pFilePath, nodeTmp.diskIndex);
		}
		else
		{
			assert (1 == strlen(m_pFilePath));
			sprintf(filePath, "%c:\\operlog.bin", m_pFilePath[0]+nodeTmp.diskIndex);
		}
#else
		sprintf(filePath, "%s/%02d/operlog.bin", m_pFilePath, nodeTmp.diskIndex);
#endif
		m_actionOperatorLog.Close();
		
		if (!m_actionOperatorLog.Open(filePath))
		{
			assert (false);
		}
	}
	
	m_lock.UnLock();
	return true;	
}

unsigned long CLogMan::GetEventLog(unsigned long startTime , unsigned long endTime, unsigned char type, ULONGLONG chnnBits, EVENT_LOT_LIST & list)
{
	unsigned long currTime = GetCurrTime32();
	if (currTime < endTime)
	{
		endTime = currTime;
	}

	if (endTime <= startTime)
	{
		return 0;
	}

	m_lock.Lock();

	unsigned long num = 0;
	char filePath [512] = {0};
	CEventLog log;

	POS pos = m_logDiskListE.GetHeadPosition();
	while (NULL != pos)
	{
		LOG_STATUS &node = m_logDiskListE.GetAt(pos);
		if (node.bAction && (NET_DISK_OWNED_BY_THIS == node.diskOwnedBy))
		{
			if ( !((0 == node.firstLog.startTime) || (node.firstLog.endTime == 0)) || \
				!((0 == node.endLog.startTime) || (node.endLog.endTime == 0)) )
			{
				if (pos == m_actionDPE)
				{
					num += m_actionEventLog.GetEventLog(startTime, endTime, type, chnnBits, list);
				}
				else
				{
#ifdef __ENVIRONMENT_WIN32__
					//处理Windows环境下接DVR硬盘的路径
					if (1 < strlen(m_pFilePath))
					{
						sprintf(filePath, "%s\\%02d\\eventlog.bin", m_pFilePath, node.diskIndex);
					}
					else
					{
						assert (1 == strlen(m_pFilePath));
						sprintf(filePath, "%c:\\eventlog.bin", m_pFilePath[0]+node.diskIndex);
					}
#else
					sprintf(filePath, "%s/%02d/eventlog.bin", m_pFilePath, node.diskIndex);
#endif	
					if (log.Open(filePath, true))
					{
						num += log.GetEventLog(startTime, endTime, type, chnnBits, list);

						log.Close();
					}
				}
			}
		}
		

		m_logDiskListE.GetNext(pos);

	}

	m_lock.UnLock();
	return num;
}

unsigned long CLogMan::GetOperatorLog(unsigned long startTime , unsigned long endTime, unsigned short contentID, OPERATOR_LOG_LIST & list)
{
	unsigned long currTime = GetCurrTime32();
	if (currTime < endTime)
	{
		endTime = currTime;
	}

	if (endTime <= startTime)
	{
		return 0;
	}

	m_lock.Lock();

	unsigned long num = 0;
	char filePath [512] = {0};
	COperatorLog log;

	POS pos = m_logDiskListO.GetHeadPosition();
	while (NULL != pos)
	{
		LOG_STATUS &node = m_logDiskListO.GetAt(pos);
		if (node.bAction && (NET_DISK_OWNED_BY_THIS == node.diskOwnedBy))
		{
			if ( !((0 == node.firstLog.startTime) || (node.firstLog.endTime == 0)) || \
				!((0 == node.endLog.startTime) || (node.endLog.endTime == 0)) )
			{
				if (pos == m_actionDPO)
				{
					num += m_actionOperatorLog.GetOperatorLog(startTime, endTime, contentID, list);
				}
				else
				{
#ifdef __ENVIRONMENT_WIN32__
					//处理Windows环境下接DVR硬盘的路径
					if (1 < strlen(m_pFilePath))
					{
						sprintf(filePath, "%s\\%02d\\operlog.bin", m_pFilePath, node.diskIndex);
					}
					else
					{
						assert (1 == strlen(m_pFilePath));
						sprintf(filePath, "%c:\\operlog.bin", m_pFilePath[0]+node.diskIndex);
					}
#else
					sprintf(filePath, "%s/%02d/operlog.bin", m_pFilePath, node.diskIndex);
#endif	
					if (log.Open(filePath, true))
					{
						num += log.GetOperatorLog(startTime, endTime, contentID, list);

						log.Close();
					}
				}
			}

		}

		m_logDiskListO.GetNext(pos);
	}

	m_lock.UnLock();
	return num;
}

void CLogMan::GetUsingPartitionMask(ULONGLONG &heightMask, ULONGLONG &lowMask)
{
	lowMask    |= m_usingDiskMaskEL;
	heightMask |= m_usingDiskMaskEH;

	lowMask    |= m_usingDiskMaskOL;
	heightMask |= m_usingDiskMaskOH;
}
////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////
bool CLogMan::ActionDiskIndexE(POS &actionPos)
{
	unsigned char diskIndex = 0;
	unsigned long time = 0x7fffffff;

	if (m_logDiskListE.IsEmpty())
	{
		return false;
	}

	bool bRet = false;
	POS pos = m_logDiskListE.GetHeadPosition();
	while (NULL != pos)
	{
		LOG_STATUS &node = m_logDiskListE.GetAt(pos);
		if (node.bAction && (NET_DISK_OWNED_BY_THIS == node.diskOwnedBy) && (NET_DISK_ONLY_READ != node.diskProperty))
		{
			unsigned long tmp = node.firstLog.startTime;
			if (node.endLog.startTime < tmp)
			{
				tmp = node.endLog.startTime;
			}

			if (tmp < time)
			{
				time = tmp;
				actionPos = pos;
				diskIndex = node.diskIndex;
				bRet = true;
			}
		}

		m_logDiskListE.GetNext(pos);
	}

	m_usingDiskMaskEL = 0;
	m_usingDiskMaskEH = 0;
	if (bRet)
	{
		assert(diskIndex >= 0);
		assert(diskIndex < 128);
		if (diskIndex < 64)
		{
			m_usingDiskMaskEL |= (((ULONGLONG)1) << diskIndex);
			m_usingDiskMaskEH |= 0;
		}
		else
		{
			m_usingDiskMaskEL |= 0;
			m_usingDiskMaskEH |= (((ULONGLONG)1) << diskIndex);		
		}		
	}	
	

	return bRet;

}


bool CLogMan::ActionDiskIndexO(POS &actionPos)
{
	unsigned char diskIndex = 0;
	unsigned long time = 0x7fffffff;
	
	actionPos = NULL;
	if (m_logDiskListO.IsEmpty())
	{
		return false;
	}
	
	bool bRet = false;
	POS pos = m_logDiskListO.GetHeadPosition();
	while (NULL != pos)
	{
		LOG_STATUS &node = m_logDiskListO.GetAt(pos);
		if (node.bAction && (NET_DISK_OWNED_BY_THIS == node.diskOwnedBy) && (NET_DISK_ONLY_READ != node.diskProperty))
		{
			unsigned long tmp = node.firstLog.startTime;
			if (node.endLog.startTime < tmp)
			{
				tmp = node.endLog.startTime;
			}

			if (tmp < time)
			{
				time = tmp;
				actionPos = pos;
				diskIndex = node.diskIndex;
				bRet = true;
			}
		}

		m_logDiskListO.GetNext(pos);
	}

	m_usingDiskMaskOL = 0;
	m_usingDiskMaskOH = 0;
	if (bRet)
	{
		assert(diskIndex >= 0);
		assert(diskIndex < 128);
		if (diskIndex < 64)
		{
			m_usingDiskMaskOL |= (((ULONGLONG)1) << diskIndex);
			m_usingDiskMaskOH |= 0;
		}
		else
		{
			m_usingDiskMaskOL |= 0;
			m_usingDiskMaskOH |= (((ULONGLONG)1) << diskIndex);		
		}
	}
		
	return bRet;
}



////////////////////////////////////////////////////////////////////////////////////
