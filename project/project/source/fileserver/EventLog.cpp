/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       :袁石维
** Date         : 2008-12-08
** Name         : EventLog.cpp
** Version      : 1.0
** Description  :
** Modify Record:
***********************************************************************/
#include "EventLog.h"
#include "Product.h"

const unsigned long MAX_EVENT_LOG_NUM = MAX_BUFFER_LEN/sizeof(EVENT_LOG);
////////////////////////////////////////////////////////////////////////////////////
CEventLog::CEventLog() : CLog (sizeof(EVENT_LOG))
{

}

CEventLog::~CEventLog()
{

}

#if 0
unsigned long CEventLog::GetEventLog (EVENT_LOT_LIST & list)
{
	assert (list.IsEmpty());
	
	if (0 == fseek(m_pFile, LOG_DATA_POS, SEEK_SET))
	{
		EVENT_LOG *pEvent = new EVENT_LOG [m_headInfo.indexNum];
		assert (NULL != pEvent);
		
		if (1 == fread(pEvent, sizeof(EVENT_LOG)*m_headInfo.indexNum, 1, m_pFile))
		{
			for (unsigned long i=0; i<m_headInfo.indexNum; ++i)
			{
				list.AddTail(pEvent[i]);
			}
			
			delete [] pEvent;
			return m_headInfo.indexNum;
		}

		delete [] pEvent;
	}
	
	return 0;
}
#endif

long CEventLog::WriteEventLog(EVENT_LOG & event)
{
	return WriteLog((unsigned char *)&event, sizeof(EVENT_LOG));
}

unsigned long CEventLog::GetEventLog(unsigned long startTime , unsigned long endTime, unsigned char type, ULONGLONG chnnBits, EVENT_LOT_LIST & list)
{
	assert (startTime < endTime);
	assert (0 != type);
	assert (0 != chnnBits);
	if ((startTime >= endTime) || (0 == type) || (0== chnnBits))
	{
		return 0;
	}

	unsigned long num = 0, index = 0, readNum =0, bufNum = MAX_BUFFER_LEN/sizeof(EVENT_LOG), getNum = 0, i = 0;
	EVENT_LOG *pEvent = new EVENT_LOG [bufNum];
	
	do 
	{
		//2010-02-27 10:39:00 YSW
		//原来ReadLog((unsigned char *)pEvent, sizeof(EVENT_LOG)*bufNum, bufNum, index)，把bufNum用readNum替代
		//从头开始检查每个事件，当最后一批时可能存在不足bufNum的情况，ReadLog内部会循环从头部读取一定数据补足
		//num个（原来为bufNum，现在为ReadLog），所以为了不重复读取，使用num限定其不无故重复。
		readNum = (bufNum <= (m_headInfo.indexNum - index)) ? bufNum : (m_headInfo.indexNum - index);
		readNum = ReadLog((unsigned char *)pEvent, sizeof(EVENT_LOG)*bufNum, /*bufNum*/readNum, index);

		if (readNum > 0)
		{
			i = 0;

			do
			{
				if ((chnnBits & ((ULONGLONG)0x01<<pEvent[i].chnn)) && (pEvent[i].type & type) && !((pEvent[i].endTime <= startTime) || (endTime <= pEvent[i].startTime)))
				{
					//2010-02-27 10:48:00 YSW
					//增加排序功能
					//list.AddTail(pEvent[i]);
					AddToList(pEvent[i], list);
					++ num;
				}
 				else if (((0 == pEvent[i].startTime) || (0 == pEvent[i].endTime)) && (index + i >= m_headInfo.index))
 				{
					//开始时间和结束时间为0表示一个无效的日志，在日志没有及时写入的情况下，
					//会出现m_headInfo.index的值之前的位置也有无效日志，这可能是由于刷新了头信息后真实的日志信息没有写入引起的
 					delete [] pEvent;
 					return num;
 				}

				++ i;
			}while((i < readNum) && (num < MAX_EVENT_LOG_NUM));
		}
		else
		{
			delete [] pEvent;
			return num;
		}

		index += readNum;
	} while(index < m_headInfo.indexNum);

	delete [] pEvent;
	return num;
}
////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////
bool CEventLog::GetLogTime(unsigned long index, unsigned long & startTime, unsigned long & endTime)
{
	EVENT_LOG  event;

	if (ReadLog((unsigned char *)&event, sizeof(EVENT_LOG), index))
	{
		startTime	= event.startTime;
		endTime		= event.endTime;

		return true;
	}

	return false;
}

bool CEventLog::GetLogTime(unsigned char *pData, unsigned long num , unsigned long index, unsigned long & startTime, unsigned long & endTime)
{
	assert (NULL != pData);
	assert (num > 0);
	assert (index < num);

	if ((NULL == pData) || (0 == num) || (index >= num))
	{
		return false;
	}

	EVENT_LOG *pEvent = (EVENT_LOG *)pData;

	startTime	= pEvent[index].startTime;
	endTime		= pEvent[index].endTime;

	return true;
}

void CEventLog::AddToList(const EVENT_LOG & log, EVENT_LOT_LIST & list)
{
	if (list.IsEmpty())
	{
		list.AddHead(log);
	}
	else
	{
		POS lastInsertPos = list.GetLastInsertPosition();
		POS pos = lastInsertPos;
		
		const EVENT_LOG & eventLog = list.GetAt(pos);

		//往前找
		if (log.startTime < eventLog.startTime)
		{
			do 
			{
				const EVENT_LOG & node = list.GetAt(pos);

				//如果节点的开始时间小于或者等于待插入节点的开始时间
				if (node.startTime <= log.startTime)
				{
					list.InsertAfter(pos, log);
					break;
				}

				list.GetPrev(pos);
			} while(NULL != pos);

			//找到头都没有找到合适的位置，就插入到头部
			if (NULL == pos)
			{
				list.AddHead(log);
			}	
		}
		else
		{
			do 
			{
				const EVENT_LOG & node = list.GetAt(pos);

				//如果等于待插入节点的开始时间小于节点的开始时间
				if (log.startTime < node.startTime)
				{
					list.InsertBefore(pos, log);
					break;
				}

				list.GetNext(pos);
			} while(NULL != pos);

			//找到尾都没有找到合适的位置，就插入到尾部
			if (NULL == pos)
			{
				list.AddTail(log);
			}	
		}
	}
}
////////////////////////////////////////////////////////////////////////////////////
