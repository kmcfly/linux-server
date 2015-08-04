/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       :	Ԭʯά
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
����������
	��̬�Ĵ���log�ļ��Ľӿڣ���Ҫָ��������·��������Ԫ�س��Ⱥ�������
	����ͷ��Ϣ�⣬�������κ������������㡣
***************************************************************************************/
bool CLog::CreateLogFile(const char *pFileName, unsigned long subLen, unsigned long num)
{
	assert ((NULL != pFileName) && (subLen > 0) && (num > 0));
	if ((NULL == pFileName) || (subLen == 0) || (num == 0))
	{
		return false;
	}

	//����ļ����ڣ���ɾ��ԭ�������ݣ����·���ռ䲢��ʼ����
	FILE *pFile = fopen(pFileName, "wb+");
	if (NULL == pFile)
	{
		return false;
	}
	
	//дͷ��Ϣ
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
	
	//д�±�
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
	
	//д������
	{
		/********************************************************************************
		��Ϊ���������Դ�����д�룬���Բ���һ��д��MAX_READ_LOG_NUM����¼�ķ�ʽ�����Ч�ʡ�
		���ǵ��������ܲ���MAX_READ_LOG_NUM�������������Է����鴦����ʾѭ��������������
		��¼�������д��ʣ��Ĳ���MAX_READ_LOG_NUM�ļ�¼��
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
		
		//������MAX_READ_LOG_NUM����������������Ҫ���жϡ�
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
����������
	��һ��log�ļ�����ȡͷ��Ϣ���±���Ϣ�����ǵ�������Ҫ�޸��쳣���ݣ������ö�д
	��ʽ�򿪡�
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

	//��ȡͷ��Ϣ
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
		//��ֻ����ʽ���ż�������쳣�������޸���
		if (!bReadOnly)
		{
			/****************************************************************************
			ԭ������������д����־��Ϣʱ���ۻ���һ���̶ȲŽ���ˢ��һ��ͷ��Ϣ��������д��
				���̶��������������������һ�������쳣�������m_headInfo.indexָ��λ�ã���
				����MAX_REFRESH_HEAD_NUM����д��ļ�¼����˿���ͨ�����m_headInfo.index��
				MAX_REFRESH_HEAD_NUM��¼���϶��Ƿ����쳣�������޸���
			****************************************************************************/
			
			//
			unsigned long startTime=0, endTime=0;
			if (0 == m_headInfo.index)	//��ʾû��д�����¼���߲�д�뼸����ͷ��Ϣû��ˢ�¡�
			{
				if (!GetLogTime(0, startTime, endTime))
				{
					Release(&m_pFile, 1, &m_pMonthBit);
					return false;
				}

				if (0 == startTime)//˵��û��д������ݣ�����Ҫ�޸���
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

			//ѭ������
			unsigned long index = 0;
			if (m_headInfo.index < m_headInfo.indexNum)
			{
				index = m_headInfo.index;
			}

			//ע��ú������ص����ݿ�����ѭ���ġ���β����ͷ�������ݡ�
			//�п����ļ���Ԫ�ظ���С��������Ŀ����Ҫ������
			unsigned long num = ReadLog(pData, m_subLen*MAX_REFRESH_HEAD_NUM, MAX_REFRESH_HEAD_NUM, index);
			if ((num != m_headInfo.indexNum) && (MAX_REFRESH_HEAD_NUM != num))
			{
				Release(&m_pFile, 2, &m_pMonthBit, &pData);
				return false;
			}

			/********************************************************************
			i=0��ʼ����Ϊm_headInfo.indexָ��������һ�κ��Ԫ�أ���ôֻҪ��MAX_REFRESH_HEAD_NUM
			��Ԫ�����ҵ�һ����m_headInfo.index����Ҫ��һ�����i��һ��ʼ��
			********************************************************************/
			unsigned long st = 0, et = 0;
			for (unsigned short i=0; i<num; ++i)
			{
				if (!GetLogTime(pData,	num, i, st, et))
				{
					Release(&m_pFile, 2, &m_pMonthBit, &pData);
					return false;
				}

				//ˢ���±����ܴ����ظ�ˢ�µ����������Ϊ�˷��㴦�������ô���ˡ�
				if ((0 < st) && (st < et))
				{
					RefreshMonthBit(st, et);
				}

				//���ǵ�һЩ��¼�Ŀ�ʼʱ�������ȣ�����ֻ����С�ڵĲ�����������
				if (st < startTime)
				{
					//����ѭ������
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
����������
	д��һ����¼������m_headInfo.indexȷ��λ�ã�����m_headInfo.indexָ����һ��Ԫ��
	�����ֵΪindexNum����ʾ�ļ�д��һ���ֻأ��´���Ҫ��ͷ����д��
	m_headInfo.index����ǰ�κ�����εķֽ��ߣ��ݴ˿��Ի�ȡ���εĿ�ͷ�ͽ����㡣
����ֵ��0��ʾʧ�ܣ�1��ʾ�ɹ���2��ʾ�ɹ������ļ�д�������з���2��ʾ�Ժ���д���µļ�
	¼�������ͷ��ʼд�롣��Ҫ���ⲿ�����������������д���ǻ��ļ�д��
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
			
			//ˢ���±�
			unsigned long startTime=0, endTime=0;
			if (GetLogTime(pData, 1, 0, startTime , endTime))
			{
				RefreshMonthBit(startTime, endTime);
			}
			
			//�ж��Ƿ�ﵽMAX_REFRESH_HEAD_NUM�Σ�����ˢ��ͷ��Ϣ��
			++ m_writeNum;
			if (MAX_REFRESH_HEAD_NUM == m_writeNum)
			{
				RefreshHead();

				//��ʱҪˢ���ļ�
				fflush(m_pFile);

				m_writeNum = 0;
			}
			
			{
				//����´ν������ǵ���־�Ŀ�ʼʱ��
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
����������
	��ȡ���е����ݡ�
********************************************************************************/
bool CLog::ReadLog(unsigned char *pData, unsigned long bufLen)
{
	assert (NULL != pData);
	assert (bufLen >= (m_subLen * m_headInfo.indexNum));
	if ((NULL == pData) || (bufLen < (m_subLen * m_headInfo.indexNum)))
	{
		return false;
	}

	//�޸���ʱ����Զ�д��ʽ�򿪣���ʱ���ܸı�m_headInfo.index��ֵ������ر��ļ�ʱ�ر����¡�
	//����ʹ��index
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
����������
	��ȡһ����־��¼��indexָ����λ�ã�������ļ�����
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
����������
	��ȡ������һ����־��¼��indexָ���俪ʼԪ��λ�ã�������ļ�����
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
	
	//���Ƕ���Ԫ�ظ���������������
	if (num > m_headInfo.indexNum)
	{
		num = m_headInfo.indexNum;
	}

	//���ǵ�index����nun��Ԫ�ص����
	if ((index+num) > m_headInfo.indexNum)
	{
		//�ȶ�β����һ����
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
		//�ٶ�ͷ����һ����
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
