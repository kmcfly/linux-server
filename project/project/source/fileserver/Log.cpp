/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       :	袁石维
** Date         : 2008-12-06
** Name         : Log.cpp
** Version      : 1.0
** Description  :
** Modify Record:
***********************************************************************/
#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "Log.h"

////////////////////////////////////////////////////////////////////////

/**************************************************************************************
功能描述：
	静态的创建log文件的接口，需要指定完整的路径名，及元素长度和总数。
	除了头信息外，其他的任何数据区都清零。
***************************************************************************************/
bool CLog::CreateLogFile(const char *pFileName, unsigned long subLen, unsigned long num)
{
	assert ((NULL != pFileName) && (subLen > 0) && (num > 0));
	if ((NULL == pFileName) || (subLen == 0) || (num == 0))
	{
		return false;
	}

	//如果文件存在，则删除原来的数据，重新分配空间并初始化。
	FILE *pFile = fopen(pFileName, "wb+");
	if (NULL == pFile)
	{
		return false;
	}
	
	//写头信息
	{
		LOG_HEAD_INFO headInfo;
		headInfo.fileVer	= LOG_FILE_VER;
		headInfo.headLen	= sizeof(LOG_HEAD_INFO);
		headInfo.indexNum	= num;
		headInfo.index		= 0;
		
		if (1 != fwrite(&headInfo, sizeof(LOG_HEAD_INFO), 1, pFile))
		{
			Release(&pFile, 0);
			return false;
		}
	}
	
	//写月表
	{
		if (0 != fseek(pFile, LOG_MONTH_DATA_POS, SEEK_SET))
		{
			Release(&pFile, 0);
			return false;
		}
		
		unsigned long *pMonthBit = new unsigned long [MAX_YEAR_LEN * 12];
		assert (NULL != pMonthBit);
		if (NULL == pMonthBit)
		{
			Release(&pFile, 0);
			return false;
		}
		
		memset (pMonthBit, 0, sizeof(unsigned long) * MAX_YEAR_LEN * 12);
		if (12 != fwrite(pMonthBit, sizeof(unsigned long) * MAX_YEAR_LEN, 12, pFile))
		{
			Release(&pFile, 1, &pMonthBit);
			return false;
		}
		delete [] pMonthBit;
		pMonthBit = NULL;
	}
	
	//写数据区
	{
		/********************************************************************************
		因为数据区可以大批量写入，所以采用一次写入MAX_READ_LOG_NUM条记录的方式来提高效率。
		考虑到总数可能不是MAX_READ_LOG_NUM的整数倍，所以分两块处理，显示循环处理整数倍的
		记录，最后在写入剩余的不足MAX_READ_LOG_NUM的记录。
		********************************************************************************/
		if (0 != fseek(pFile, LOG_DATA_POS, SEEK_SET))
		{
			Release(&pFile, 0);
			return false;
		}

		unsigned char *pData = new unsigned char [MAX_READ_LOG_NUM * subLen];	
		assert (NULL != pData);
		if (NULL == pData)
		{
			Release(&pFile, 0);
			return false;
		}

		memset (pData, 0, MAX_READ_LOG_NUM * subLen);

		unsigned long loopNum = num / MAX_READ_LOG_NUM;
		for (unsigned long i=0; i<loopNum; ++i)
		{
			if (1 != fwrite(pData, MAX_READ_LOG_NUM*subLen, 1, pFile))
			{
				delete [] pData;
				Release(&pFile, 0, NULL);
				return false;
			}
		}
		
		//可能是MAX_READ_LOG_NUM的整数倍，所以需要做判断。
		if ((num % MAX_READ_LOG_NUM) > 0)
		{
			if (1 != fwrite(pData, (num % MAX_READ_LOG_NUM)*subLen, 1, pFile))
			{
				delete [] pData;
				Release(&pFile, 0, NULL);
				return false;
			}
		}
		
		delete [] pData;
		Release(&pFile, 0, NULL);
		return true;
	}
}

////////////////////////////////////////////////////////////////////////
CLog::CLog(unsigned long subLen) : m_subLen(subLen), m_pMonthBit(NULL), m_pFile(NULL), m_writeNum(0)
{
	memset(&m_headInfo, 0, sizeof(LOG_DATA_POS));
}

CLog::~CLog()
{
	
}

/********************************************************************************
功能描述：
	打开一个log文件，读取头信息及月表信息。考虑到可能需要修复异常数据，所以用读写
	方式打开。
********************************************************************************/
bool CLog::Open (const char *pFileName, bool bReadOnly)
{
	assert ((NULL != pFileName) && (NULL == m_pFile));
	if (NULL == pFileName)
	{
		return false;
	}

	if (NULL != m_pFile)
	{
		fclose(m_pFile);
		m_pFile = NULL;
	}

	if (bReadOnly)
	{
		m_pFile = fopen(pFileName, "rb");
	}
	else
	{
		m_pFile = fopen(pFileName, "rb+");
	}
	
	if (NULL == m_pFile)
	{
		return false;
	}

	//读取头信息
	{
		if (1 != fread(&m_headInfo, sizeof(LOG_HEAD_INFO), 1, m_pFile))
		{
			Release(&m_pFile, 0);
			return false;
		}
	}
	
	{
		if (0 != fseek(m_pFile, LOG_MONTH_DATA_POS, SEEK_SET))
		{
			Release(&m_pFile, 0);
			return false;
		}
		
		m_pMonthBit = new unsigned long [MAX_YEAR_LEN * 12];
		assert (NULL != m_pMonthBit);
		if (NULL == m_pMonthBit)
		{
			Release(&m_pFile, 0);
			return false;
		}

		if (12 != fread(m_pMonthBit, sizeof(unsigned long)*MAX_YEAR_LEN, 12, m_pFile))
		{
			Release(&m_pFile, 1, &m_pMonthBit);
			return false;
		}	
	}
	
	{
		//非只读方式，才检查数据异常并进行修复。
		if (!bReadOnly)
		{
			/****************************************************************************
			原理描述：由于写入日志信息时，累积到一定程度才进行刷新一次头信息（避免多次写入
				磁盘而有损磁盘寿命）。所以一旦出现异常，相对于m_headInfo.index指定位置，最
				多有MAX_REFRESH_HEAD_NUM条新写入的记录，因此可以通过检查m_headInfo.index后
				MAX_REFRESH_HEAD_NUM记录来断定是否有异常并进行修复。
			****************************************************************************/
			
			//
			unsigned long startTime=0, endTime=0;
			if (0 == m_headInfo.index)	//表示没有写入过记录或者才写入几条，头信息没有刷新。
			{
				if (!GetLogTime(0, startTime, endTime))
				{
					Release(&m_pFile, 1, &m_pMonthBit);
					return false;
				}

				if (0 == startTime)//说明没有写入过数据，不需要修复。
				{
					m_writeNum	= 0;
					m_firstST	= 0;
					m_firstET	= 0;
					m_secondST	= 0;
					m_secondET	= 0;
					return true;
				}
			}
			else
			{
				if (m_headInfo.index <= m_headInfo.indexNum)
				{
					if (!GetLogTime(m_headInfo.index-1, startTime, endTime))
					{
						Release(&m_pFile, 1, &m_pMonthBit);
						return false;
					}
				}
				else
				{
					Release(&m_pFile, 1, &m_pMonthBit);
					return false;
				}
			}

			unsigned char *pData = new unsigned char [m_subLen * MAX_REFRESH_HEAD_NUM];
			assert (NULL != pData);
			if (NULL == pData)
			{
				Release(&m_pFile, 1, &m_pMonthBit);
				return false;
			}

			//循环调整
			unsigned long index = 0;
			if (m_headInfo.index < m_headInfo.indexNum)
			{
				index = m_headInfo.index;
			}

			//注意该函数返回的数据可能是循环的。即尾部接头部的数据。
			//有可能文件里元素个数小于需求数目，需要做处理。
			unsigned long num = ReadLog(pData, m_subLen*MAX_REFRESH_HEAD_NUM, MAX_REFRESH_HEAD_NUM, index);
			if ((num != m_headInfo.indexNum) && (MAX_REFRESH_HEAD_NUM != num))
			{
				Release(&m_pFile, 2, &m_pMonthBit, &pData);
				return false;
			}

			/********************************************************************
			i=0开始，因为m_headInfo.index指向的是最近一段后的元素，那么只要在MAX_REFRESH_HEAD_NUM
			个元素中找到一个，m_headInfo.index都需要加一，因而i从一开始。
			********************************************************************/
			unsigned long st = 0, et = 0;
			for (unsigned short i=0; i<num; ++i)
			{
				if (!GetLogTime(pData,	num, i, st, et))
				{
					Release(&m_pFile, 2, &m_pMonthBit, &pData);
					return false;
				}

				//刷新月表，可能存在重复刷新的情况，但是为了方便处理不理会那么多了。
				if ((0 < st) && (st < et))
				{
					RefreshMonthBit(st, et);
				}

				//考虑到一些记录的开始时间可能相等，所有只能是小于的才满足条件。
				if (st < startTime)
				{
					//考虑循环返回
					if ((m_headInfo.index + i) <= m_headInfo.indexNum)
					{
						m_headInfo.index += i;
					}
					else
					{
						m_headInfo.index = i - (m_headInfo.indexNum - m_headInfo.index);
					}

					break;
				}
			}

			delete [] pData;
			RefreshHead ();
		}
	}

	{
		unsigned long time = 0;

		if (m_headInfo.index == m_headInfo.indexNum)
		{
			GetLogTime(0, m_firstST, time);
			GetLogTime(m_headInfo.indexNum-1, time, m_firstET);
			m_secondST = m_firstST;
			m_secondET = m_firstET;
		}
		else
		{
			if ((0 < m_headInfo.index) && (m_headInfo.index < m_headInfo.indexNum))
			{
				GetLogTime(m_headInfo.index, m_firstST, time);
				GetLogTime(m_headInfo.indexNum-1, time, m_firstET);

				GetLogTime(0, m_secondST, time);
				GetLogTime(m_headInfo.index-1, time, m_secondET);
			}
			else
			{
				m_firstST = 0;
				m_firstET = 0;
				m_secondST = 0;
				m_secondET = 0;

				m_headInfo.index = 0;
			}
		}
	}

	m_writeNum = 0;
	return true;
}

void CLog::Close ()
{
	if (NULL != m_pFile)
	{
		RefreshHead();

		fflush(m_pFile);

		m_firstST	= 0;
		m_firstET	= 0;
		m_secondST	= 0;
		m_secondET	= 0;
	}
	
	delete [] m_pMonthBit;
	m_pMonthBit = NULL;
	
	Release(&m_pFile, 1, NULL);
}

void CLog::GetTime(unsigned long & firstST, unsigned long & firstET, unsigned long & secondST, unsigned long & secondET)
{
	firstST = m_firstST;
	firstET = m_firstET;
	secondST = m_secondST;
	secondET = m_secondET;
}
////////////////////////////////////////////////////////////////////////
/********************************************************************************
功能描述：
	写入一条记录，根据m_headInfo.index确定位置，而后m_headInfo.index指向下一个元素
	。最大值为indexNum，表示文件写满一次轮回，下次需要重头覆盖写。
	m_headInfo.index是早前段和最近段的分界线，据此可以获取两段的开头和结束点。
返回值：0表示失败，1表示成功，2表示成功，且文件写满。其中返回2表示以后再写入新的记
	录，将会从头开始写入。需要由外部根据总体决定来覆盖写还是换文件写。
********************************************************************************/
long CLog::WriteLog(unsigned char *pData, unsigned long subLen)
{
	assert (NULL != pData);
	assert (m_subLen == subLen);

	if ((NULL == pData) || (m_subLen != subLen))
	{
		return 0;
	}

	if (m_headInfo.index >= m_headInfo.indexNum)
	{
		m_headInfo.index = 0;
	}
	
	if (0 == fseek(m_pFile, LOG_DATA_POS+(m_headInfo.index*m_subLen), SEEK_SET))
	{
		if (1 == fwrite(pData, m_subLen, 1, m_pFile))
		{
			++ m_headInfo.index;
			
			//刷新月表
			unsigned long startTime=0, endTime=0;
			if (GetLogTime(pData, 1, 0, startTime , endTime))
			{
				RefreshMonthBit(startTime, endTime);
			}
			
			//判断是否达到MAX_REFRESH_HEAD_NUM次，是则刷新头信息。
			++ m_writeNum;
			if (MAX_REFRESH_HEAD_NUM == m_writeNum)
			{
				RefreshHead();

				//此时要刷新文件
				fflush(m_pFile);

				m_writeNum = 0;
			}
			
			{
				//获得下次将被覆盖的日志的开始时间
				unsigned long time = 0;
				if (m_headInfo.index == m_headInfo.indexNum)
				{
					GetLogTime(0, m_firstST, time);
					GetLogTime(pData, 1, 0, time, m_secondET);
					m_secondST = m_firstST;
					m_firstET = m_secondET;
				}
				else if (1 == m_headInfo.index)
				{
					GetLogTime(pData, 1, 0, m_secondST, m_secondET);
					GetLogTime(m_headInfo.index, m_firstST, time);
				}
				else
				{
					GetLogTime(m_headInfo.index, m_firstST, time);
					GetLogTime(pData, 1, 0, time, m_secondET);
				}
			}
			
			return (m_headInfo.index == m_headInfo.indexNum)? 2 : 1;
		}
	}
	
	return 0;
}

/********************************************************************************
功能描述：
	读取所有的数据。
********************************************************************************/
bool CLog::ReadLog(unsigned char *pData, unsigned long bufLen)
{
	assert (NULL != pData);
	assert (bufLen >= (m_subLen * m_headInfo.indexNum));
	if ((NULL == pData) || (bufLen < (m_subLen * m_headInfo.indexNum)))
	{
		return false;
	}

	//修复的时候会以读写方式打开，此时不能改变m_headInfo.index的值，否则关闭文件时回被更新。
	//所以使用index
	unsigned long index = 0;
	if (m_headInfo.index < m_headInfo.indexNum)
	{
		index = m_headInfo.index;
	}

	if (0 != fseek(m_pFile, LOG_DATA_POS+(m_subLen*index), SEEK_SET))
	{
		return false;
	}

	if (1 != fread(pData, m_subLen*(m_headInfo.indexNum-index), 1, m_pFile))
	{
		return false;
	}
	
	if (index > 0)
	{
		if (0 != fseek(m_pFile, LOG_DATA_POS, SEEK_SET))
		{
			return false;
		}
		
		if (1 != fread(pData, m_subLen*m_headInfo.index, 1, m_pFile))
		{
			return false;
		}
	}
	
	return true;
}

/********************************************************************************
功能描述：
	读取一条日志记录，index指定其位置（相对于文件）。
********************************************************************************/
bool CLog::ReadLog(unsigned char *pData, unsigned long bufLen, unsigned long index)
{
	assert (NULL != pData);
	assert (bufLen >= m_subLen);
	assert (index < m_headInfo.indexNum);
	if ((NULL == pData) || (bufLen < m_subLen) || (index >= m_headInfo.indexNum))
	{
		return false;
	}

	if (0 == fseek(m_pFile, LOG_DATA_POS+(index*m_subLen), SEEK_SET))
	{
		if (1 == fread(pData, m_subLen, 1, m_pFile))
		{
			return true;
		}
	}

	return false;
}


/********************************************************************************
功能描述：
	读取连续地一段日志记录，index指定其开始元素位置（相对于文件）。
********************************************************************************/
unsigned long CLog::ReadLog(unsigned char *pData, unsigned long bufLen, unsigned long num, unsigned long index)
{
	assert (NULL != pData);
	assert (num > 0);
	assert (bufLen >= (m_subLen * num));
	assert (index < m_headInfo.indexNum);
	if ((NULL == pData) || (0 == num) || (bufLen < (m_subLen*num)) || (index >= m_headInfo.indexNum))
	{
		return 0;
	}
	
	//考虑都有元素个数不足需求的情况
	if (num > m_headInfo.indexNum)
	{
		num = m_headInfo.indexNum;
	}

	//考虑到index后不足nun个元素的情况
	if ((index+num) > m_headInfo.indexNum)
	{
		//先读尾部的一部分
		if (0 != fseek(m_pFile, LOG_DATA_POS+(index*m_subLen), SEEK_SET))
		{
			return 0;
		}

		size_t ret = fread(pData, m_subLen, num, m_pFile);

		if ((0 >= ret) || (0 != fseek(m_pFile, LOG_DATA_POS, SEEK_SET)))
		{
			return 0;
		}

		unsigned long retNum = (unsigned long)ret;
		//再对头部的一部分
		ret = fread(pData, m_subLen, (num - ret), m_pFile);
		if (ret > 0)
		{
			return retNum + (unsigned long)ret;
		}
		else
		{
			return retNum;
		}
	}
	else
	{
		if (0 != fseek(m_pFile, LOG_DATA_POS+(index*m_subLen), SEEK_SET))
		{
			return 0;
		}
		
		size_t ret = fread(pData, m_subLen, num, m_pFile);
		if (ret > 0)
		{
			return (unsigned long)ret;
		}
		else
		{
			return 0;
		}
	}
}
////////////////////////////////////////////////////////////////////////
void CLog::RefreshMonthBit(unsigned long startTime, unsigned long endTime)
{
	tm st = DVRTime32ToTm(startTime);

	unsigned long dayNum = (endTime/ONE_DAY_SECOND_NUM - startTime/ONE_DAY_SECOND_NUM);
	unsigned long i = (st.tm_year - 100)*12+st.tm_mon;
	unsigned long j = st.tm_mday-1;
	unsigned long *p = m_pMonthBit + i;
	unsigned long month = 0, d=0;
	
	do 
	{
		do 
		{
			month |= (0x01 << j);
			++ j;
			++ d;
		} while((j < 32) && (d < dayNum));
		
		*p |= month;
		
		j = 0;
		month = 0;
		++p;
	} while(d < dayNum);
}

void CLog::RefreshHead()
{
	if (0 == fseek(m_pFile, 0, SEEK_SET))
	{
		if (1 == fwrite(&m_headInfo, sizeof(LOG_HEAD_INFO), 1, m_pFile))
		{
			if (0 == fseek(m_pFile, LOG_MONTH_DATA_POS, SEEK_SET))
			{
				fwrite(m_pMonthBit, sizeof(unsigned long)*MAX_YEAR_LEN*12, 1, m_pFile);
			}
		}
	}
}
////////////////////////////////////////////////////////////////////////
