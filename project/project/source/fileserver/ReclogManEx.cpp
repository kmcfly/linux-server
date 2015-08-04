/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       :	袁石维
** Date         : 2008-12-11
** Name         : ReclogManEx.cpp
** Version      : 1.0
** Description  :
** Modify Record:
***********************************************************************/
#include "ReclogManEx.h"

//////////////////////////////////////////////////////////////////////////
CReclogManEX::CReclogManEX ():m_pFilePath(NULL), m_pFileHoldTimeD(NULL), m_FileHoldChnnNum(0)
{
	m_diskFreeAlarmSizeM = INVALID_INDEX(unsigned long);
	m_bSupportRedunRec   = false;
	m_reclogList.RemoveAll();
	m_bRecycleRecord    = false;
}

CReclogManEX::~CReclogManEX ()
{

}

bool CReclogManEX::Initial(DISK_LOG_INFO *pDiskStatus, unsigned char diskNum, bool bSupportRedun, const char * pFilePath)
{

	m_lock.Lock();

	assert(0 == m_reclogList.GetCount());

	if (m_reclogList.GetCount() > 0)
	{
		//重复初始化。
		m_lock.UnLock();
		return true;
	}

	
	m_reclogList.RemoveAll();

	//基本参数初始化.保存文件路径
	assert (NULL == m_pFilePath);
	if (NULL != m_pFilePath)
	{
		delete [] m_pFilePath;
		m_pFilePath = NULL;
	}

	m_pFilePath = new char [512];
	memset(m_pFilePath, 0, 512);

	if (NULL != pFilePath)
	{
		strcpy(m_pFilePath, pFilePath);
	}

	
	//没有可用分区时
	{
		if (0 == diskNum)
		{
			m_lock.UnLock();
			return true;
		}
	}
	

	//初始化CReclogEx对象。
	{
		assert (0 == m_reclogList.GetCount());
		
		bool bSucc = true;
		for (unsigned char i = 0; i < diskNum; ++i)
		{
			RECLOGEX_STATUS reclog;			
			reclog.pReclog     = new CReclogEx; 
			reclog.diskOwnedBy = pDiskStatus[i].diskOwnedBy;
			reclog.diskIndex   = pDiskStatus[i].diskIndex;
			reclog.diskProperty  = pDiskStatus[i].diskProperty;
			if (reclog.pReclog->Initial(reclog.diskIndex, pFilePath))
			{
				reclog.bAction = true;
				m_reclogList.AddTail(reclog);				
			}
			else
			{
				bSucc = false;
				break;
			}			
		}

		if (!bSucc)
		{
			POS pos = m_reclogList.GetHeadPosition();
			while (NULL != pos)
			{
				RECLOGEX_STATUS &reclog = m_reclogList.GetAt(pos);
				reclog.pReclog->Quit();
				delete reclog.pReclog;
				m_reclogList.GetNext(pos);
			}
			m_reclogList.RemoveAll();

			Release(NULL, 1, &m_pFilePath);
			
			m_lock.UnLock();
			return false;
		}
	}

	{
		assert(NULL == m_pFileHoldTimeD);
		if (NULL != m_pFileHoldTimeD)
		{
			delete [] m_pFileHoldTimeD;
			m_pFileHoldTimeD = NULL;
		}
		m_pFileHoldTimeD = NULL;
		m_FileHoldChnnNum = 0;
	}

	m_diskFreeAlarmSizeM = INVALID_INDEX(unsigned long);
	m_bSupportRedunRec   = bSupportRedun;

	m_lock.UnLock();
	return true;
}

void CReclogManEX::Quit ()
{
	m_lock.Lock();

	POS pos = m_reclogList.GetHeadPosition();
	while (NULL != pos)
	{
		RECLOGEX_STATUS & reclog = m_reclogList.GetAt(pos);
		reclog.pReclog->Quit();
		delete reclog.pReclog;
		m_reclogList.GetNext(pos);
	}

	m_reclogList.RemoveAll();

	
	if (NULL != m_pFilePath)
	{
		delete [] m_pFilePath;
		m_pFilePath = NULL;
	}

	if (NULL != m_pFileHoldTimeD)
	{
		delete [] m_pFileHoldTimeD;
		m_pFileHoldTimeD = NULL;
	}
	
	m_lock.UnLock();
}

void CReclogManEX::CreateDateList()
{
	m_lock.Lock();

	POS pos = m_reclogList.GetHeadPosition();
	while (NULL != pos)
	{
		RECLOGEX_STATUS & reclog = m_reclogList.GetAt(pos);
		if (reclog.bAction)
		{
			reclog.pReclog->CreateDateList();
		}
		
		m_reclogList.GetNext(pos);
	}

	m_lock.UnLock();
}

///////////////////////////////////////////////////////////////////////////////////
bool CReclogManEX::AddDisk(DISK_LOG_INFO *pDiskStatus, unsigned char num)
{
	m_lock.Lock();
	
	int oldNum = m_reclogList.GetCount();
	bool bSucc = true;
	for (unsigned char i = 0; i < num; ++i)
	{
		RECLOGEX_STATUS reclog;
		reclog.bAction      = true;
		reclog.diskOwnedBy  = pDiskStatus[i].diskOwnedBy;
		reclog.diskIndex    = pDiskStatus[i].diskIndex;	
		reclog.diskProperty = pDiskStatus[i].diskProperty;
		reclog.pReclog      = new CReclogEx;		
		if (reclog.pReclog->Initial(reclog.diskIndex, m_pFilePath))
		{
			reclog.pReclog->CreateDateList();
			m_reclogList.AddTail(reclog);
		}
		else
		{
			bSucc = false;
			break;
		}
	}

	if (!bSucc)
	{
		int count = 0;
		int newNum = m_reclogList.GetCount();
		POS pos = m_reclogList.GetTailPosition();
		while ((NULL != pos) && (count < newNum - oldNum))
		{
			RECLOGEX_STATUS &reclog = m_reclogList.GetAt(pos);
			reclog.pReclog->Quit();
			delete reclog.pReclog;
			m_reclogList.RemoveAt(pos);
			pos = m_reclogList.GetTailPosition();
			count++;
		}
	}

	m_lock.UnLock();
	return true;	
}


bool CReclogManEX::ActionDisk(DISK_LOG_INFO *pDiskStatus, unsigned char num, bool bAction)
{
	m_lock.Lock();

	assert(num > 0);
	
	POS pos = m_reclogList.GetHeadPosition();
	while (NULL != pos)
	{
		RECLOGEX_STATUS &reclog = m_reclogList.GetAt(pos);
		for (int i = 0; i < num; ++i)
		{
			if (reclog.diskIndex == pDiskStatus[i].diskIndex)
			{
				assert(reclog.bAction != bAction);
				if (!bAction)
				{
					reclog.pReclog->Quit();
					reclog.bAction = bAction;
				}
				else
				{
					if (!reclog.pReclog->Initial(reclog.diskIndex, m_pFilePath))
					{
						assert(false);
						reclog.bAction = false;
					}
					else
					{
						reclog.diskOwnedBy = pDiskStatus[i].diskOwnedBy;
						reclog.diskIndex = pDiskStatus[i].diskIndex;
						reclog.diskProperty = pDiskStatus[i].diskProperty;
						reclog.pReclog->CreateDateList();
						reclog.bAction = bAction;
					}
				}

				break;
			}
		}

		m_reclogList.GetNext(pos);
	}


	m_lock.UnLock();
	return true;

}


bool CReclogManEX::ActionAllDisk(bool bAction)
{
	m_lock.Lock();

	POS pos = m_reclogList.GetHeadPosition();
	while (NULL != pos)
	{
		RECLOGEX_STATUS &reclog = m_reclogList.GetAt(pos);
		assert(reclog.bAction != bAction);
		if (!bAction)
		{
			reclog.pReclog->Quit();
			reclog.bAction = bAction;
		}
		else
		{
			if (!reclog.pReclog->Initial(reclog.diskIndex, m_pFilePath))
			{
				assert(false);
				reclog.bAction = false;
			}
			else
			{
				reclog.pReclog->CreateDateList();
				reclog.bAction = bAction;
			}
		}
		m_reclogList.GetNext(pos);
	}

	m_lock.UnLock();
	return true;
}


void CReclogManEX::SetRecycleRecord(bool bEnable)
{
	m_bRecycleRecord = bEnable;
}

///////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////
  
bool CReclogManEX::GetInvalidFile(unsigned long chnn, unsigned short &diskIndex, unsigned short &fileIndex, unsigned char fromeType)
{
	assert((NET_DISK_READ_WRITE == fromeType) || (NET_DISK_REDUNDANCY == fromeType));

	m_lock.Lock();

	POS pos = m_reclogList.GetHeadPosition();
	while (NULL != pos)
	{
		RECLOGEX_STATUS &reclog = m_reclogList.GetAt(pos);
		if (reclog.bAction && (NET_DISK_OWNED_BY_THIS == reclog.diskOwnedBy) && (fromeType == reclog.diskProperty))
		{
			if (reclog.pReclog->GetInvalidFile(chnn, fileIndex))
			{
				diskIndex = reclog.diskIndex;
				m_lock.UnLock();
				return true;
			}
		}
		m_reclogList.GetNext(pos);
	}

	m_lock.UnLock();
	return false;	
}

bool CReclogManEX::FindRecycleFile(FILE_INFO_EX &fileInfo, unsigned char fromeType)
{
	assert((NET_DISK_READ_WRITE == fromeType) || (NET_DISK_REDUNDANCY == fromeType));
	m_lock.Lock();

	unsigned long maxTime = 0x7fffffff;
	bool bSucc = false;
	POS pos = m_reclogList.GetHeadPosition();
	while (NULL != pos)
	{
		RECLOGEX_STATUS &reclog = m_reclogList.GetAt(pos);
		if (reclog.bAction && (NET_DISK_OWNED_BY_THIS == reclog.diskOwnedBy) && (fromeType == reclog.diskProperty))
		{	
			FILE_INFO_EX fileInfoTmp;
			if (reclog.pReclog->FindRecycleFile(fileInfoTmp))
			{
				if (fileInfoTmp.startTime < maxTime)
				{
					fileInfo = fileInfoTmp;
					maxTime = fileInfoTmp.startTime;
					bSucc = true;
				}				
			}
		}
		m_reclogList.GetNext(pos);
	}

	m_lock.UnLock();
	return bSucc;	
	
}
unsigned long CReclogManEX::GetSectionInfo(ULONGLONG chnnbBits, unsigned long startTime, unsigned long endTime, \
							 SECTION_INFO_EX_LIST *pList, unsigned char chnnNum, unsigned char diskOwnedBy, bool bOfficial)
{
	m_lock.Lock();

	//在不支持冗余录像的情况下，搜索的文件可以出现重复。
	//这里的重复是指：同一个通道，出现两个开始时间一样的文件。
	//比如在修改时间时，就有可能出现两个录像文件的开始时间一样
	bool bRepeat = false;
	if (!m_bSupportRedunRec)
	{
		bRepeat = true;
	}
	
	unsigned long num = 0;
	POS pos = m_reclogList.GetHeadPosition();
	while (NULL != pos)
	{
		RECLOGEX_STATUS &reclog = m_reclogList.GetAt(pos);
		if (reclog.bAction && (diskOwnedBy == reclog.diskOwnedBy))
		{
			num += reclog.pReclog->GetSectionInfo(chnnbBits, startTime, endTime, pList, chnnNum, bRepeat, bOfficial);
		}

		m_reclogList.GetNext(pos);
	}

	m_lock.UnLock();
	return num;
	
}

unsigned long CReclogManEX::GetFileInfo(unsigned long chnn, unsigned long startTime, unsigned long endTime, \
										FILE_INFO_EX_LIST & list, unsigned char diskOwnedBy, bool bOfficial)
{
	m_lock.Lock();

	bool bRepeat = false;
	if (!m_bSupportRedunRec)
	{
		bRepeat = true;
	}

	unsigned long num = 0;
	POS pos = m_reclogList.GetHeadPosition();
	while (NULL != pos)
	{
		RECLOGEX_STATUS &reclog = m_reclogList.GetAt(pos);
		if (reclog.bAction && (diskOwnedBy == reclog.diskOwnedBy))
		{
			bool bOnlyRead = false;
			if (NET_DISK_ONLY_READ == reclog.diskProperty)
			{
				bOnlyRead = true;
			}
			else
			{
				bOnlyRead = false;
			}
			num += reclog.pReclog->GetFileInfo(chnn, startTime, endTime, list, bOnlyRead, bRepeat, bOfficial);
		}

		m_reclogList.GetNext(pos);
	}

	m_lock.UnLock();
	return num;


}

unsigned long CReclogManEX::GetFileInfo(ULONGLONG chnnbBits, unsigned long startTime, unsigned long endTime, \
						  FILE_INFO_EX_LIST *pList, unsigned char chnnNum, unsigned char diskOwnedBy, bool bOfficial)
{
	m_lock.Lock();

	bool bRepeat = false;
	if (!m_bSupportRedunRec)
	{
		bRepeat = true;
	}

	unsigned long num = 0;
	POS pos = m_reclogList.GetHeadPosition();
	while (NULL != pos)
	{
		RECLOGEX_STATUS &reclog = m_reclogList.GetAt(pos);
		if (reclog.bAction && (diskOwnedBy == reclog.diskOwnedBy))
		{
			bool bOnlyRead = false;
			if (NET_DISK_ONLY_READ == reclog.diskProperty)
			{
				bOnlyRead = true;
			}
			else
			{
				bOnlyRead = false;
			}
			num += reclog.pReclog->GetFileInfo(chnnbBits, startTime, endTime, pList, chnnNum, bOnlyRead, bRepeat, bOfficial);
		}

		m_reclogList.GetNext(pos);
	}

	m_lock.UnLock();
	return num;
}

unsigned long CReclogManEX::GetReclog(unsigned long chnn, unsigned long type, unsigned long startTime, unsigned long endTime, \
									  RECLOG_INFO_EX_LIST &list, unsigned char diskOwnedBy, bool bOfficial)
{
	m_lock.Lock();

	bool bRepeat = false;
	if (!m_bSupportRedunRec)
	{
		bRepeat = true;
	}

	unsigned long num = 0;
	POS pos = m_reclogList.GetHeadPosition();
	while (NULL != pos)
	{
		RECLOGEX_STATUS &reclog = m_reclogList.GetAt(pos);
		bool bOnlyRead = false;
		if (NET_DISK_ONLY_READ == reclog.diskProperty)
		{
			bOnlyRead = true;
		}
		else
		{
			bOnlyRead = false;
		}
		if (reclog.bAction && (diskOwnedBy == reclog.diskOwnedBy))
		{
			num += reclog.pReclog->GetReclog(chnn, type, startTime, endTime, list, bOnlyRead, bRepeat, bOfficial);
		}

		m_reclogList.GetNext(pos);
	}

	m_lock.UnLock();
	return num;	
}

unsigned long CReclogManEX::GetReclog(ULONGLONG chnnbBits, unsigned long type, unsigned long startTime, unsigned long endTime, 
						RECLOG_INFO_EX_LIST *pList, unsigned char chnnNum, unsigned char diskOwnedBy, bool bOfficial)
{
	m_lock.Lock();
	
	bool bRepeat = false;
	if (!m_bSupportRedunRec)
	{
		bRepeat = true;
	}

	unsigned long num = 0;
	POS pos = m_reclogList.GetHeadPosition();
	while (NULL != pos)
	{
		RECLOGEX_STATUS &reclog = m_reclogList.GetAt(pos);
		bool bOnlyRead = false;
		if (NET_DISK_ONLY_READ == reclog.diskProperty)
		{
			bOnlyRead = true;
		}
		else
		{
			bOnlyRead = false;
		}
		if (reclog.bAction && (diskOwnedBy == reclog.diskOwnedBy))
		{
			num += reclog.pReclog->GetReclog(chnnbBits, type, startTime, endTime, pList, chnnNum, bOnlyRead, bRepeat, bOfficial);
		}

		m_reclogList.GetNext(pos);
	}

	m_lock.UnLock();
	return num;

}

unsigned long CReclogManEX::GetDateInfo(CMyList <DVR_DATE> &list, unsigned char diskOwnedBy)
{

	m_lock.Lock();

	unsigned long num = 0;
	POS pos = m_reclogList.GetHeadPosition();
	while (NULL != pos)
	{
		RECLOGEX_STATUS &reclog = m_reclogList.GetAt(pos);

		if (reclog.bAction && (diskOwnedBy == reclog.diskOwnedBy))
		{
			num += reclog.pReclog->GetDateInfo(list);
		}
		
		m_reclogList.GetNext(pos);
	}

	m_lock.UnLock();
	return num;

}

unsigned long CReclogManEX::GetFileInfo(unsigned long chnn, unsigned long time, \
										FILE_INFO_EX_LIST &list, unsigned char diskOwnedBy, bool bOfficial)
{
	m_lock.Lock();
	
	bool bRepeat = false;
	if (!m_bSupportRedunRec)
	{
		bRepeat = true;
	}

	unsigned long num = 0;
	POS pos = m_reclogList.GetHeadPosition();
	while (NULL != pos)
	{
		RECLOGEX_STATUS &reclog = m_reclogList.GetAt(pos);
		if (reclog.bAction && (diskOwnedBy == reclog.diskOwnedBy))
		{
			bool bOnlyRead = false;
			if (NET_DISK_ONLY_READ == reclog.diskProperty)
			{
				bOnlyRead = true;
			}
			else
			{
				bOnlyRead = false;
			}
			num += reclog.pReclog->GetFileInfo(chnn, time, list, bOnlyRead, bRepeat, bOfficial);
		}

		m_reclogList.GetNext(pos);
	}

	m_lock.UnLock();
	return num;
}


//////////////////////////////////////////////////////////////////////////
bool CReclogManEX::WriteLogStart(const FILE_INFO_EX &fileInfo)
{
	m_lock.Lock();

	POS pos = m_reclogList.GetHeadPosition();
	while (NULL != pos)
	{
		RECLOGEX_STATUS &reclog = m_reclogList.GetAt(pos);
		if (reclog.diskIndex == fileInfo.diskIndex)
		{
			assert(NET_DISK_OWNED_BY_THIS == reclog.diskOwnedBy);
			assert(NET_DISK_ONLY_READ != reclog.diskProperty);
			if (reclog.bAction)
			{
				bool ret = reclog.pReclog->WriteReclogStart(fileInfo.fileIndex, fileInfo.chnn, (RECORD_TYPE)fileInfo.type, fileInfo.startTime);
				m_lock.UnLock();
				return ret;
			}
			break;
		}
		m_reclogList.GetNext(pos);
	}

	m_lock.UnLock();
	return false;
	
}

bool CReclogManEX::WriteLogEnd(const FILE_INFO_EX &fileInfo)
{
	m_lock.Lock();

	POS pos = m_reclogList.GetHeadPosition();
	while (NULL != pos)
	{
		RECLOGEX_STATUS &reclog = m_reclogList.GetAt(pos);
		if (reclog.diskIndex == fileInfo.diskIndex)
		{
			assert(NET_DISK_OWNED_BY_THIS == reclog.diskOwnedBy);
			assert(NET_DISK_ONLY_READ != reclog.diskProperty);
			if (reclog.bAction)
			{
				bool ret = reclog.pReclog->WriteReclogEnd(fileInfo.fileIndex, fileInfo.chnn, (RECORD_TYPE)fileInfo.type, fileInfo.endTime);
				m_lock.UnLock();
				return ret;
			}
			break;
		}
		m_reclogList.GetNext(pos);
	}

	m_lock.UnLock();
	return false;	
}

void CReclogManEX::RegisterRecordFile(unsigned channel, unsigned short diskIndex, unsigned short fileIndex)
{
	m_lock.Lock();

	POS pos = m_reclogList.GetHeadPosition();
	while (NULL != pos)
	{
		RECLOGEX_STATUS &reclog = m_reclogList.GetAt(pos);
		if (reclog.diskIndex == diskIndex)
		{
			assert((reclog.bAction) && (NET_DISK_OWNED_BY_THIS == reclog.diskOwnedBy) && (NET_DISK_ONLY_READ != reclog.diskProperty));
			reclog.pReclog->RegisterRecordFile(channel, fileIndex);
			break;
		}
		m_reclogList.GetNext(pos);
	}
	assert(NULL != pos);

	m_lock.UnLock();
	return;
}

void CReclogManEX::CancelRecordFile(unsigned channel, unsigned short diskIndex, unsigned short fileIndex)
{
	m_lock.Lock();

	POS pos = m_reclogList.GetHeadPosition();
	while (NULL != pos)
	{
		RECLOGEX_STATUS &reclog = m_reclogList.GetAt(pos);
		if (reclog.diskIndex == diskIndex)
		{
			assert((reclog.bAction) && (NET_DISK_OWNED_BY_THIS == reclog.diskOwnedBy) && (NET_DISK_ONLY_READ != reclog.diskProperty));
			reclog.pReclog->CancelRecordFile(channel, fileIndex);
			break;
		}
		m_reclogList.GetNext(pos);
	}
	assert(NULL != pos);

	m_lock.UnLock();
	return;
}

void CReclogManEX::RegisterReadFile(unsigned channel, unsigned short diskIndex, unsigned short fileIndex)
{
	m_lock.Lock();

	POS pos = m_reclogList.GetHeadPosition();
	while (NULL != pos)
	{
		RECLOGEX_STATUS &reclog = m_reclogList.GetAt(pos);
		if (reclog.diskIndex == diskIndex)
		{
			assert(reclog.bAction);
			reclog.pReclog->RegisterReadFile(channel, fileIndex);
			break;
		}
		m_reclogList.GetNext(pos);
	}
	assert(NULL != pos);

	m_lock.UnLock();
	return;
}



void CReclogManEX::CancelReadFile(unsigned channel, unsigned short diskIndex, unsigned short fileIndex)
{
	m_lock.Lock();

	POS pos = m_reclogList.GetHeadPosition();
	while (NULL != pos)
	{
		RECLOGEX_STATUS &reclog = m_reclogList.GetAt(pos);
		if (reclog.diskIndex == diskIndex)
		{
			assert(reclog.bAction);
			reclog.pReclog->CancelReadFile(channel, fileIndex);
			break;
		}
		m_reclogList.GetNext(pos);
	}
	assert(NULL != pos);

	m_lock.UnLock();
	return;
}

void CReclogManEX::GetUsingPartitionMask(ULONGLONG &heightMask, ULONGLONG &lowMask)
{
	m_lock.Lock();

	ULONGLONG lRet = 0;
	ULONGLONG hRet = 0;
	POS pos = m_reclogList.GetHeadPosition();
	while (NULL != pos)
	{
		RECLOGEX_STATUS &reclog = m_reclogList.GetAt(pos);
		if ((reclog.bAction) && (reclog.pReclog->IsBeWoring()))
		{
			assert(reclog.diskIndex >= 0);
			assert(reclog.diskIndex < 128);
			if (reclog.diskIndex < 64)
			{
				lRet |= (((ULONGLONG)1) << (reclog.diskIndex));
				hRet |= 0;
			}
			else
			{
				lRet |= 0;
				hRet |= (((ULONGLONG)1) << (reclog.diskIndex));
			}
		}
		m_reclogList.GetNext(pos);
	}

	lowMask    |= lRet;
	heightMask |= hRet;

	m_lock.UnLock();
	return;
}

void CReclogManEX::GetUnfinishedFile(FILE_INFO_EX_LIST &list)
{
	m_lock.Lock();
	
	POS pos = m_reclogList.GetHeadPosition();
	while (NULL != pos)
	{
		RECLOGEX_STATUS &reclog = m_reclogList.GetAt(pos);
		//if (reclog.bAction && (NET_DISK_OWNED_BY_THIS == reclog.diskOwnedBy) && (NET_DISK_ONLY_READ != reclog.diskProperty))
		if (reclog.bAction)
		{
			reclog.pReclog->GetUnfinishedFile(list);
		}
		m_reclogList.GetNext(pos);
	}

	m_lock.UnLock();
	return;

}

bool CReclogManEX::Repair(const FILE_INFO_EX &fileInfo, unsigned long endTime)
{
	m_lock.Lock();

	bool ret = true;
	POS pos = m_reclogList.GetHeadPosition();
	while (NULL != pos)
	{
		RECLOGEX_STATUS &reclog = m_reclogList.GetAt(pos);
		if (reclog.diskIndex == fileInfo.diskIndex)
		{
			//assert(NET_DISK_OWNED_BY_THIS == reclog.diskOwnedBy);
			//assert(NET_DISK_ONLY_READ != reclog.diskProperty);
			if (reclog.bAction)
			{
				ret = reclog.pReclog->Repair(fileInfo, endTime);
			}
			break;
		}
		m_reclogList.GetNext(pos);
	}
   
	m_lock.UnLock();
	return ret;	
}


bool CReclogManEX::GetOneFileAllInfo(unsigned short diskIndex, unsigned short fileIndex, 
									 unsigned char *pFileHeadInfo, unsigned char *pFileSectionInfo, unsigned char *pFileReclogInfo)
{
	//没有对正在写的文件进行修复。
	assert(pFileHeadInfo != NULL);
	assert(pFileSectionInfo != NULL);
	assert(pFileReclogInfo != NULL);	

	m_lock.Lock();
	
	bool bRet = false;
	POS pos = m_reclogList.GetHeadPosition();
	while (NULL != pos)
	{
		RECLOGEX_STATUS &reclog = m_reclogList.GetAt(pos);
		if (reclog.diskIndex == diskIndex)
		{
			if (reclog.bAction)
			{
				bRet =reclog.pReclog->GetOneFileAllInfo(fileIndex, pFileHeadInfo, pFileSectionInfo, pFileReclogInfo);
			}
			break;
		}

		m_reclogList.GetNext(pos);
	}

    m_lock.UnLock();
	return bRet;

}



unsigned long CReclogManEX::GetDiskFreeSpace(unsigned char startDiskIndex, unsigned char num)
{
	m_lock.Lock();

	unsigned long ret = 0;	
	POS pos = m_reclogList.GetHeadPosition();
	while (NULL != pos)
	{
		RECLOGEX_STATUS &reclog = m_reclogList.GetAt(pos);
		if ((reclog.diskIndex >= startDiskIndex) && (reclog.diskIndex < startDiskIndex + num)) 
		{
			ret += reclog.pReclog->GetFreeFileNum() * MAX_RECORD_FILE_LEN_M;
		}
		m_reclogList.GetNext(pos);
	}

	m_lock.UnLock();
	return ret;

}

bool CReclogManEX::GetWorkDiskFreeFileNum(unsigned char startDiskIndex, unsigned char num, unsigned long &freeFileNum)
{
	m_lock.Lock();

	bool bIsWorkDisk = false;	
	POS pos = m_reclogList.GetHeadPosition();
	while (NULL != pos)
	{
		RECLOGEX_STATUS &reclog = m_reclogList.GetAt(pos);
		if ((reclog.bAction) && (NET_DISK_OWNED_BY_THIS == reclog.diskOwnedBy) && (NET_DISK_ONLY_READ != reclog.diskProperty))
		{
			bIsWorkDisk = true;
			if ((reclog.diskIndex >= startDiskIndex) && (reclog.diskIndex < startDiskIndex + num)) 
			{
				freeFileNum += reclog.pReclog->GetFreeFileNum();
			}
		}
		m_reclogList.GetNext(pos);
	}

	m_lock.UnLock();
	return bIsWorkDisk;

}

bool CReclogManEX::CheckDiskAlarm(unsigned long freeFileNum)
{
	//如果是循环录像，就不检查
	if (m_bRecycleRecord)
	{
		return false;
	}

	//如果m_diskFreeAlarmSizeM为最大值，则不检查
	if (INVALID_INDEX(unsigned long) == m_diskFreeAlarmSizeM)
	{
		return false;
	}
	
	unsigned long alarmFileNum = m_diskFreeAlarmSizeM / MAX_RECORD_FILE_LEN_M;
	if (0 != m_diskFreeAlarmSizeM % MAX_RECORD_FILE_LEN_M)
	{
		alarmFileNum++;
	}


	if (freeFileNum <= alarmFileNum)
	{
		return true;
	}
	
	return false;
}

bool CReclogManEX::CheckReadDisk()
{
	bool bRet = false;

	m_lock.Lock();
	POS pos = m_reclogList.GetHeadPosition();
	while (NULL != pos)
	{
		RECLOGEX_STATUS &reclog = m_reclogList.GetAt(pos);
		if (reclog.pReclog->IsBeReading())
		{
			bRet = true;
			break;
		}		
		m_reclogList.GetNext(pos);
	}
	m_lock.UnLock();

	return bRet;
}

unsigned long CReclogManEX::GetDiskChannelNum(unsigned char diskIndex , bool bIsBackUpCreated/* = false*/)
{
	m_lock.Lock();

	unsigned long channelNum = 0;	
	POS pos = m_reclogList.GetHeadPosition();
	while (NULL != pos)
	{
		RECLOGEX_STATUS &reclog = m_reclogList.GetAt(pos);
		if (reclog.diskIndex == diskIndex)
		{
			channelNum = reclog.pReclog->GetChannelNum(bIsBackUpCreated);
			break;
		}
		m_reclogList.GetNext(pos);
	}

	assert(NULL != pos);

	m_lock.UnLock();
	return channelNum;
}

bool CReclogManEX::DeleteFile(const FILE_INFO_EX &fileInfo)
{
	m_lock.Lock();

	bool ret = true;
	POS pos = m_reclogList.GetHeadPosition();
	while (NULL != pos)
	{
		RECLOGEX_STATUS &reclog = m_reclogList.GetAt(pos);
		if (reclog.diskIndex == fileInfo.diskIndex)
		{
			//deleteFile也可以用在删除其他硬盘里的数据。比如：在初始化其他硬盘里的reclog时,
			//有可能会修复其中一个有问题的文件，如果修复失败，就删除这个文件。
			//assert(NET_DISK_OWNED_BY_THIS == reclog.diskOwnedBy);
			//assert(NET_DISK_ONLY_READ != reclog.diskProperty);
		    //assert(!RECORD_FILE_IS_LOCK(fileInfo.type));
			assert(!RECORD_FILE_IS_WRITING(fileInfo.type));
			//assert(!RECORD_FILE_IS_READING(fileInfo.type));			
			if (reclog.bAction)
			{
				ret = reclog.pReclog->DeleteFile(fileInfo);
			}
			break;
		}
		m_reclogList.GetNext(pos);			 
	}

	m_lock.UnLock();
	return ret;
}

bool CReclogManEX::DeleteFileEx(const FILE_INFO_EX &fileInfo)
{
	m_lock.Lock();

	bool ret = true;
	POS pos = m_reclogList.GetHeadPosition();
	while (NULL != pos)
	{	
		unsigned long num = 0;
		FILE_INFO_EX_LIST list;
		assert(list.IsEmpty());

		RECLOGEX_STATUS &reclog = m_reclogList.GetAt(pos);
	
		if ((reclog.bAction) && (NET_DISK_OWNED_BY_THIS == reclog.diskOwnedBy) && (NET_DISK_ONLY_READ != reclog.diskProperty))
		{
			num += reclog.pReclog->GetFileInfo(fileInfo.chnn, fileInfo.startTime, fileInfo.endTime, list, false, true, false);
		}

		if (num > 0)
		{
			do 
			{
				FILE_INFO_EX &tmp = list.GetHead();
				if ((fileInfo.diskIndex == tmp.diskIndex) && (fileInfo.fileIndex == tmp.fileIndex) && (!RECORD_FILE_IS_LOCK(tmp.type)) &&\
					(!RECORD_FILE_IS_WRITING(tmp.type)) && (!RECORD_FILE_IS_READING(tmp.type)))
				{
					ret = reclog.pReclog->DeleteFile(tmp);
					list.RemoveAll();

					m_lock.UnLock();
					return ret;
				}
				else
				{
					list.RemoveHead();
				}
			} while(list.GetCount() > 0);
		}

		m_reclogList.GetNext(pos);
	}
	
	m_lock.UnLock();
	return ret;
}

bool CReclogManEX::LockFile(const FILE_INFO_EX &fileInfo, bool bLock)
{
	m_lock.Lock();

	bool ret = true;
	POS pos = m_reclogList.GetHeadPosition();
	while (NULL != pos)
	{
		RECLOGEX_STATUS &reclog = m_reclogList.GetAt(pos);
		if (reclog.diskIndex == fileInfo.diskIndex)
		{
			assert(NET_DISK_OWNED_BY_THIS == reclog.diskOwnedBy);
			assert(NET_DISK_ONLY_READ != reclog.diskProperty);
            assert(!RECORD_FILE_IS_WRITING(fileInfo.type));
			if (reclog.bAction)
			{
				ret = reclog.pReclog->LockFile(fileInfo, bLock);
			}
			break;
		}
		m_reclogList.GetNext(pos);			 
	}

	m_lock.UnLock();

	return ret;
	
}

bool CReclogManEX::LockFileEx(const FILE_INFO_EX &fileInfo, bool bLock)
{
	m_lock.Lock();

	bool ret = true;
	POS pos = m_reclogList.GetHeadPosition();
	while (NULL != pos)
	{	
		unsigned long num = 0;
		FILE_INFO_EX_LIST list;
		assert(list.IsEmpty());

		RECLOGEX_STATUS &reclog = m_reclogList.GetAt(pos);
		if ((reclog.bAction) && (NET_DISK_OWNED_BY_THIS == reclog.diskOwnedBy) && (NET_DISK_ONLY_READ != reclog.diskProperty))
		{
			num += reclog.pReclog->GetFileInfo(fileInfo.chnn, fileInfo.startTime, fileInfo.endTime, list, false, true, false);
		}

		if (num > 0)
		{
			do 
			{
				FILE_INFO_EX &tmp = list.GetHead();
				if ((fileInfo.diskIndex == tmp.diskIndex) && (fileInfo.fileIndex == tmp.fileIndex) && !RECORD_FILE_IS_WRITING(tmp.type))
				{
					ret = reclog.pReclog->LockFile(tmp, bLock);
					list.RemoveAll();

					m_lock.UnLock();
					return ret;
				}
				else
				{
					list.RemoveHead();
				}
			} while(list.GetCount() > 0);				
		}

		m_reclogList.GetNext(pos);
	}

	m_lock.UnLock();
	return ret;

}

void CReclogManEX::SetFileHoldTimeD(const unsigned long *pHoldTimeD, unsigned long num)
{
	m_lock.Lock();

	if (NULL != m_pFileHoldTimeD)
	{
		delete [] m_pFileHoldTimeD;
		m_pFileHoldTimeD = NULL;
		m_FileHoldChnnNum = 0;
	}

	m_pFileHoldTimeD = new unsigned long [num];
	for (int i = 0; i < num; ++i)
	{
		m_pFileHoldTimeD[i] = pHoldTimeD[i];
	}

	m_FileHoldChnnNum = num;
   
	m_lock.UnLock();

	return;
}

bool CReclogManEX::DeleteOverdueFiles( )
{
	m_lock.Lock();
    
	if ((NULL == m_pFileHoldTimeD) || (0 == m_FileHoldChnnNum))
	{
		m_lock.UnLock();
		return true;
	}

	POS pos = m_reclogList.GetHeadPosition();
	while (NULL != pos)
	{
		RECLOGEX_STATUS &reclog = m_reclogList.GetAt(pos);
		if ((reclog.bAction) && (NET_DISK_OWNED_BY_THIS == reclog.diskOwnedBy) && (NET_DISK_ONLY_READ != reclog.diskProperty))
		{
			reclog.pReclog->DeleteOverdueFiles(m_pFileHoldTimeD, m_FileHoldChnnNum);
		}
		m_reclogList.GetNext(pos);
	}

	m_lock.UnLock();
	return true;
}

void CReclogManEX::SetDiskAlarmSize(unsigned long diskSize)
{
	m_lock.Lock();
	printf("%s,%d,diskSize:%d\n",__FILE__,__LINE__,diskSize);
	
	m_diskFreeAlarmSizeM = diskSize;

	m_lock.UnLock();
	return;
}



//////////////////////////////////////////////////////////////////////////

