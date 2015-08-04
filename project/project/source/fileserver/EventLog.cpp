/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       :Ԭʯά
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
		//ԭ��ReadLog((unsigned char *)pEvent, sizeof(EVENT_LOG)*bufNum, bufNum, index)����bufNum��readNum���
		//��ͷ��ʼ���ÿ���¼��������һ��ʱ���ܴ��ڲ���bufNum�������ReadLog�ڲ���ѭ����ͷ����ȡһ�����ݲ���
		//num����ԭ��ΪbufNum������ΪReadLog��������Ϊ�˲��ظ���ȡ��ʹ��num�޶��䲻�޹��ظ���
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
					//����������
					//list.AddTail(pEvent[i]);
					AddToList(pEvent[i], list);
					++ num;
				}
 				else if (((0 == pEvent[i].startTime) || (0 == pEvent[i].endTime)) && (index + i >= m_headInfo.index))
 				{
					//��ʼʱ��ͽ���ʱ��Ϊ0��ʾһ����Ч����־������־û�м�ʱд�������£�
					//�����m_headInfo.index��ֵ֮ǰ��λ��Ҳ����Ч��־�������������ˢ����ͷ��Ϣ����ʵ����־��Ϣû��д�������
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

		//��ǰ��
		if (log.startTime < eventLog.startTime)
		{
			do 
			{
				const EVENT_LOG & node = list.GetAt(pos);

				//����ڵ�Ŀ�ʼʱ��С�ڻ��ߵ��ڴ�����ڵ�Ŀ�ʼʱ��
				if (node.startTime <= log.startTime)
				{
					list.InsertAfter(pos, log);
					break;
				}

				list.GetPrev(pos);
			} while(NULL != pos);

			//�ҵ�ͷ��û���ҵ����ʵ�λ�ã��Ͳ��뵽ͷ��
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

				//������ڴ�����ڵ�Ŀ�ʼʱ��С�ڽڵ�Ŀ�ʼʱ��
				if (log.startTime < node.startTime)
				{
					list.InsertBefore(pos, log);
					break;
				}

				list.GetNext(pos);
			} while(NULL != pos);

			//�ҵ�β��û���ҵ����ʵ�λ�ã��Ͳ��뵽β��
			if (NULL == pos)
			{
				list.AddTail(log);
			}	
		}
	}
}
////////////////////////////////////////////////////////////////////////////////////
