/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       :袁石维
** Date         : 2008-12-08
** Name         : OperatorLog.h
** Version      : 1.0
** Description  :
** Modify Record:
***********************************************************************/
#include "OperatorLog.h"
#include "Product.h"

const unsigned long  MAX_OPERATOR_LOG_NUM = MAX_BUFFER_LEN/sizeof(OPERATOR_LOG);
///////////////////////////////////////////////////////////////////////////////////
COperatorLog::COperatorLog() : CLog (sizeof(OPERATOR_LOG))
{

}

COperatorLog::~COperatorLog()
{

}


#if 0
unsigned long COperatorLog::GetOperatorList (OPERATOR_LOG_LIST & list)
{
	assert (list.IsEmpty());
	
	if (0 == fseek(m_pFile, LOG_DATA_POS, SEEK_SET))
	{
		OPERATOR_LOG *pOperator = new OPERATOR_LOG [m_headInfo.indexNum];
		assert (NULL != pOperator);
		
		if (1 == fread(pOperator, sizeof(OPERATOR_LOG)*m_headInfo.indexNum, 1, m_pFile))
		{
			for (unsigned long i=0; i<m_headInfo.indexNum; ++i)
			{
				list.AddTail(pOperator[i]);
			}
			
			delete [] pOperator;
			return m_headInfo.indexNum;
		}

		delete [] pOperator;
	}
	
	return 0;
}
#endif

long COperatorLog::WriteOperatorLog(OPERATOR_LOG & operatorLog)
{
	return WriteLog((unsigned char *)&operatorLog, sizeof(OPERATOR_LOG));
}

unsigned long COperatorLog::GetOperatorLog(unsigned long startTime , unsigned long endTime, unsigned short contentID, OPERATOR_LOG_LIST & list)
{
	assert (startTime < endTime);
	if (startTime >= endTime)
	{
		return 0;
	}

	unsigned long num = 0, index = 0, readNum = 0, bufNum = MAX_BUFFER_LEN/sizeof(OPERATOR_LOG), getNum = 0, i = 0;

	OPERATOR_LOG *pOperator = new OPERATOR_LOG [bufNum];

	do 
	{
		readNum = (bufNum <= (m_headInfo.indexNum - index)) ? bufNum : (m_headInfo.indexNum - index);
		readNum = ReadLog((unsigned char *)pOperator, sizeof(OPERATOR_LOG)*bufNum, readNum, index);

		if (readNum > 0)
		{
			i = 0;

			do
			{
				if ((pOperator[i].contentID & contentID) && !((pOperator[i].time < startTime) || (endTime < pOperator[i].time)))
				{
					AddToList(pOperator[i], list);
					++ num;
				}
				else if (0 == pOperator[i].time)
				{
					delete [] pOperator;
					return num;
				}

				++ i;
			}while((i < readNum) && (num < MAX_OPERATOR_LOG_NUM));
		}
		else
		{
			delete [] pOperator;
			return num;
		}

		index += readNum;
	} while(index < m_headInfo.indexNum);

	delete [] pOperator;
	return num;
}
///////////////////////////////////////////////////////////////////////////////////
bool COperatorLog::GetLogTime(unsigned long index, unsigned long & startTime, unsigned long & endTime)
{
	OPERATOR_LOG  operatorLog;

	if (ReadLog((unsigned char *)&operatorLog, sizeof(OPERATOR_LOG), index))
	{
		startTime	= operatorLog.time;
		endTime		= operatorLog.time;

		return true;
	}

	return false;
}

bool COperatorLog::GetLogTime(unsigned char *pData, unsigned long num , unsigned long index, unsigned long & startTime, unsigned long & endTime)
{
	assert (NULL != pData);
	assert (num > 0);
	assert (index < num);

	if ((NULL == pData) || (0 == num) || (index >= num))
	{
		return false;
	}

	OPERATOR_LOG *pOperator = (OPERATOR_LOG *)pData;

	startTime	= pOperator[index].time;
	endTime		= pOperator[index].time;

	return true;
}

void COperatorLog::AddToList(const OPERATOR_LOG & log, OPERATOR_LOG_LIST & list)
{
	if (list.IsEmpty())
	{
		list.AddHead(log);
	}
	else
	{
		POS lastInsertPos = list.GetLastInsertPosition();
		POS pos = lastInsertPos;

		const OPERATOR_LOG & operatorLog = list.GetAt(pos);

		//往前找
		if (log.time < operatorLog.time)
		{
			do 
			{
				const OPERATOR_LOG & node = list.GetAt(pos);

				//如果节点的时间小于或者等于待插入节点的时间
				if (node.time <= log.time)
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
				const OPERATOR_LOG & node = list.GetAt(pos);

				//如果等于待插入节点的时间小于节点的时间
				if (log.time < node.time)
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
///////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////
