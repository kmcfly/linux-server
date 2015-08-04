/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : 袁石维
** Date         : 2008-07-30
** Name         : CruiseConfig.cpp
** Version      : 1.0
** Description  :
** Modify Record:
20081201		调整
***********************************************************************/
#include "ScheduleConfig.h"
#include "Product.h"

CScheduleConfig::CScheduleConfig():m_pFile(NULL), m_pFileName(NULL), m_pScheduleList(NULL)
{
}

CScheduleConfig::~CScheduleConfig()
{

}

bool CScheduleConfig::Initial(unsigned char chnnNum, unsigned char sensorInNum, unsigned char sensorOutNum, const char *pFilePath /* = NULL */)
{
	m_pFileName = new char [512];
	memset(m_pFileName, 0, 512);
	if (NULL != pFilePath)
	{
#ifdef __ENVIRONMENT_WIN32__
		sprintf(m_pFileName, "%s\\config\\ScheduleConfig.dat", pFilePath);
#else
		sprintf(m_pFileName, "%s/config/ScheduleConfig.dat", pFilePath);
#endif
	}
	else
	{
#ifdef __ENVIRONMENT_WIN32__
		strcpy(m_pFileName, "\\config\\ScheduleConfig.dat");
#else
		strcpy(m_pFileName, "config/ScheduleConfig.dat");
#endif
	}

	m_pFile = fopen(m_pFileName,"rb+");
	if(NULL == m_pFile)
	{
		m_pFile = fopen(m_pFileName,"wb");
		if (NULL == m_pFile)
		{
			delete [] m_pFileName;
			m_pFileName = NULL;
			assert (false);	//出现异常
			return false;
		}

		if(!DefaultConfig(chnnNum, sensorInNum, sensorOutNum))
		{
			fclose(m_pFile);
			m_pFile = NULL;
			delete [] m_pFileName;
			m_pFileName = NULL;
			return false;
		}

		fclose(m_pFile);
		m_pFile = fopen(m_pFileName,"rb+");

		if (NULL == m_pFile)
		{
			delete [] m_pFileName;
			m_pFileName = NULL;
			//重新打开文件失败。
			assert(false);
			return false;
		}
	}

	if (LoadConfig())
	{
		return true;
	}
	else
	{
		RemoveScheduleList();

		fclose(m_pFile);
		m_pFile = NULL;

		delete [] m_pScheduleList;
		m_pScheduleList = NULL;

		delete [] m_pFileName;
		m_pFileName = NULL;
		return false;
	}
}

void CScheduleConfig::Quit()
{
	if(NULL != m_pFile)
	{
		fclose(m_pFile);
		m_pFile = NULL;
	}

	RemoveScheduleList();

	delete [] m_pScheduleList;
	m_pScheduleList = NULL;
	delete [] m_pFileName;
	m_pFileName = NULL;
}

void CScheduleConfig::RemoveScheduleList()
{
	if (NULL != m_pScheduleList)
	{
		for(unsigned char i=0; i<m_cfgHead.idNum; ++i)
		{
			if(!m_pScheduleList[i].IsEmpty())
			{
				POS pos = m_pScheduleList[i].GetHeadPosition();

				while(NULL != pos)
				{
					SCHEDULE_NODE &sheduleNode = m_pScheduleList[i].GetNext(pos);
					if(NULL != sheduleNode.pHolidayList)
					{
						sheduleNode.pHolidayList->RemoveAll();
						delete sheduleNode.pHolidayList;
						sheduleNode.pHolidayList = NULL;
					}				
				}

				m_pScheduleList[i].RemoveAll();
			}
		}
	}
}

bool CScheduleConfig::DefaultConfig(unsigned char chnnNum, unsigned char sensorInNum, unsigned char sensorOutNum)
{
	m_cfgHead.idNum = HOLIDAY_SCH_ITEM_NUM;
	m_cfgHead.channelNum = chnnNum;
	m_cfgHead.sensorInNum = sensorInNum;
	m_cfgHead.sensorOutNum = sensorOutNum;
	m_cfgHead.version = SCHEDULE_FILE_VER;
	m_cfgHead.headLen = sizeof(SCHEDULE_CFG_HEAD);
	m_cfgHead.fileLength = SCHEDULE_DATA_POS + sizeof(SCHEDULE_ITEM_ID_HEAD)*m_cfgHead.idNum;

	unsigned long currPos = SCHEDULE_DATA_POS + sizeof(SCHEDULE_ITEM_ID_HEAD)*m_cfgHead.idNum;
	SCHEDULE_ITEM_ID_HEAD *pScheduleIndex = new SCHEDULE_ITEM_ID_HEAD [m_cfgHead.idNum];
	pScheduleIndex[0].item_head.itemID = NCFG_ITEM_REC_SCH_SCHEDULE_HOLIDAY;
	pScheduleIndex[0].item_head.num = chnnNum;
	pScheduleIndex[0].item_head.subLen = sizeof(SCHEDULE_CHNN_HEAD);
	pScheduleIndex[0].item_head.len = pScheduleIndex[0].item_head.num*pScheduleIndex[0].item_head.subLen;//sublen*num = 0
	pScheduleIndex[0].start_pos = currPos;
	currPos += pScheduleIndex[0].item_head.len;
	m_cfgHead.fileLength += sizeof(SCHEDULE_ITEM_ID_HEAD)*chnnNum;

	pScheduleIndex[1].item_head.itemID = NCFG_ITEM_REC_SCH_MOTION_HOLIDAY;
	pScheduleIndex[1].item_head.num = chnnNum;
	pScheduleIndex[1].item_head.subLen = sizeof(SCHEDULE_CHNN_HEAD);
	pScheduleIndex[1].item_head.len = pScheduleIndex[1].item_head.num*pScheduleIndex[1].item_head.subLen;//sublen*num = 0
	pScheduleIndex[1].start_pos = currPos;
	currPos += pScheduleIndex[1].item_head.len;
	m_cfgHead.fileLength += sizeof(SCHEDULE_ITEM_ID_HEAD)*chnnNum;

	pScheduleIndex[2].item_head.itemID = NCFG_ITEM_REC_SCH_SENSOR_HOLIDAY;
	pScheduleIndex[2].item_head.num = chnnNum;
	pScheduleIndex[2].item_head.subLen = sizeof(SCHEDULE_CHNN_HEAD);
	pScheduleIndex[2].item_head.len = pScheduleIndex[2].item_head.num*pScheduleIndex[2].item_head.subLen;//sublen*num = 0
	pScheduleIndex[2].start_pos = currPos;
	currPos += pScheduleIndex[2].item_head.len;
	m_cfgHead.fileLength += sizeof(SCHEDULE_ITEM_ID_HEAD)*chnnNum;

	pScheduleIndex[3].item_head.itemID = NCFG_ITEM_MOTION_SCH_HOLIDAY;
	pScheduleIndex[3].item_head.num = chnnNum;
	pScheduleIndex[3].item_head.subLen = sizeof(SCHEDULE_CHNN_HEAD);
	pScheduleIndex[3].item_head.len = pScheduleIndex[3].item_head.num*pScheduleIndex[3].item_head.subLen;//sublen*num = 0
	pScheduleIndex[3].start_pos = currPos;
	currPos += pScheduleIndex[3].item_head.len;
	m_cfgHead.fileLength += sizeof(SCHEDULE_ITEM_ID_HEAD)*chnnNum;

	pScheduleIndex[4].item_head.itemID = NCFG_ITEM_SENSOR_IN_SCH_HOLIDAY;
	pScheduleIndex[4].item_head.num = sensorInNum;
	pScheduleIndex[4].item_head.subLen = sizeof(SCHEDULE_CHNN_HEAD);
	pScheduleIndex[4].item_head.len = pScheduleIndex[4].item_head.num*pScheduleIndex[4].item_head.subLen;//sublen*num = 0
	pScheduleIndex[4].start_pos = currPos;
	currPos += pScheduleIndex[4].item_head.len;
	m_cfgHead.fileLength += sizeof(SCHEDULE_ITEM_ID_HEAD)*sensorInNum;

	pScheduleIndex[5].item_head.itemID = NCFG_ITEM_ALARMOUT_SCH_HOLIDAY;
	pScheduleIndex[5].item_head.num = sensorOutNum;
	pScheduleIndex[5].item_head.subLen = sizeof(SCHEDULE_CHNN_HEAD);
	pScheduleIndex[5].item_head.len = pScheduleIndex[5].item_head.num*pScheduleIndex[5].item_head.subLen;//sublen*num = 0
	pScheduleIndex[5].start_pos = currPos;
	currPos += pScheduleIndex[5].item_head.len;
	m_cfgHead.fileLength += sizeof(SCHEDULE_ITEM_ID_HEAD)*sensorOutNum;

	pScheduleIndex[6].item_head.itemID = NCFG_ITEM_NET_ALARM_SCH_HOLIDAY;
	pScheduleIndex[6].item_head.num = chnnNum;
	pScheduleIndex[6].item_head.subLen = sizeof(SCHEDULE_CHNN_HEAD);
	pScheduleIndex[6].item_head.len = pScheduleIndex[6].item_head.num*pScheduleIndex[6].item_head.subLen;//sublen*num = 0
	pScheduleIndex[6].start_pos = currPos;
	currPos += pScheduleIndex[6].item_head.len;
	m_cfgHead.fileLength += sizeof(SCHEDULE_ITEM_ID_HEAD)*chnnNum;

	rewind(m_pFile);
	if(1 != fwrite(&m_cfgHead, sizeof(SCHEDULE_CFG_HEAD), 1, m_pFile))
	{
		delete [] pScheduleIndex;
		pScheduleIndex = NULL;

		return false;
	}

	if(0 == fseek(m_pFile, SCHEDULE_DATA_POS, SEEK_SET))
	{
		if(m_cfgHead.idNum != fwrite(pScheduleIndex, sizeof(SCHEDULE_ITEM_ID_HEAD), m_cfgHead.idNum, m_pFile))
		{
			delete [] pScheduleIndex;
			pScheduleIndex = NULL;

			return false;
		}
	}
	
	long totalSchNum = chnnNum*(m_cfgHead.idNum-2)+sensorInNum+sensorOutNum;
	SCHEDULE_CHNN_HEAD *pChnnHead = new SCHEDULE_CHNN_HEAD[totalSchNum];
	SCHEDULE_CHNN_HEAD *pTempHead = pChnnHead;
	for (unsigned long j=0; j<m_cfgHead.idNum; j++)
	{
		for (unsigned long i=0; i<pScheduleIndex[j].item_head.num; i++)
		{
			pTempHead->item_head.itemID = (unsigned short)i;
			pTempHead->item_head.num = 0;
			pTempHead->item_head.subLen = sizeof(HOLIDAY_SCHEDULE);
			pTempHead->item_head.len = 0;//sublen*num = 0
			pTempHead->start_pos = m_cfgHead.fileLength;

			pTempHead++;
		}
	}

	if(0 == fseek(m_pFile, SCHEDULE_DATA_POS + sizeof(SCHEDULE_ITEM_ID_HEAD)*m_cfgHead.idNum, SEEK_SET))
	{
		if(totalSchNum != fwrite(pChnnHead, sizeof(SCHEDULE_CHNN_HEAD), totalSchNum, m_pFile))
		{
			delete [] pScheduleIndex;
			pScheduleIndex = NULL;

			delete [] pChnnHead;
			pChnnHead = NULL;
			return false;
		}
	}
	delete [] pChnnHead;
	pChnnHead = NULL;

	delete [] pScheduleIndex;
	pScheduleIndex = NULL;

	return true;
}

bool CScheduleConfig::LoadConfig()
{
	//先读取头信息
	if (0 != fseek(m_pFile, 0, SEEK_SET))
	{
		return false;
	}
	else
	{
		if (1 != fread(&m_cfgHead, sizeof(SCHEDULE_CFG_HEAD), 1, m_pFile))
		{
			return false;
		}
	}

	assert ((HOLIDAY_SCH_ITEM_NUM >= m_cfgHead.idNum) && (m_cfgHead.idNum > 0));
	m_pScheduleList	= new CMyList <SCHEDULE_NODE> [m_cfgHead.idNum];
	assert (NULL != m_pScheduleList);

	SCHEDULE_ITEM_ID_HEAD *pScheduleIndex = new SCHEDULE_ITEM_ID_HEAD[m_cfgHead.idNum];
	memset(pScheduleIndex, 0, m_cfgHead.idNum*sizeof(SCHEDULE_ITEM_ID_HEAD));

	if(0 == fseek(m_pFile, SCHEDULE_DATA_POS, SEEK_SET))
	{
		if(m_cfgHead.idNum != fread(pScheduleIndex, sizeof(SCHEDULE_ITEM_ID_HEAD), m_cfgHead.idNum, m_pFile))
		{
			delete [] pScheduleIndex;
			pScheduleIndex = NULL;
			return false;
		}

		for(unsigned long i=0; i<m_cfgHead.idNum; ++i)
		{
			if(0 == fseek(m_pFile, pScheduleIndex[i].start_pos, SEEK_SET))
			{
				SCHEDULE_CHNN_HEAD *pChnnHead = new SCHEDULE_CHNN_HEAD[pScheduleIndex[i].item_head.num];
				memset(pChnnHead, 0, pScheduleIndex[i].item_head.num*sizeof(SCHEDULE_CHNN_HEAD));
				if(pScheduleIndex[i].item_head.num != fread(pChnnHead, sizeof(SCHEDULE_CHNN_HEAD),\
					pScheduleIndex[i].item_head.num, m_pFile))
				{
					delete [] pChnnHead;
					pChnnHead = NULL;
					return false;
				}

				for (unsigned long j=0; j<pScheduleIndex[i].item_head.num; j++)
				{
					LoadInsertToList(pChnnHead[j], pScheduleIndex[i].item_head.itemID, i);
				}

				delete [] pChnnHead;
				pChnnHead = NULL;
			}
		}

		delete [] pScheduleIndex;
		pScheduleIndex = NULL;
		return true;
	}

	delete [] pScheduleIndex;
	pScheduleIndex = NULL;
	return false;
}

bool CScheduleConfig::LoadInsertToList(SCHEDULE_CHNN_HEAD &ChnnHead, int itemID, int itemIdHeadIndex)
{
	SCHEDULE_NODE scheduleNode;
	scheduleNode.chnn = ChnnHead.item_head.itemID;
	scheduleNode.itemID = itemID;
	scheduleNode.pHolidayList = new CMyList <HOLIDAY_SCHEDULE>;
	if (ChnnHead.item_head.num > 0)
	{
		if (0 != fseek(m_pFile, ChnnHead.start_pos, SEEK_SET))
		{
			return false;
		}

		HOLIDAY_SCHEDULE *pHoliday = new HOLIDAY_SCHEDULE[ChnnHead.item_head.num];
		if (pHoliday == NULL)
		{
			return false;
		}

		if (1 != fread(pHoliday, sizeof(HOLIDAY_SCHEDULE)*ChnnHead.item_head.num, 1, m_pFile))
		{
			delete [] pHoliday;
			pHoliday = NULL;
			return false;
		}

//		scheduleNode.pHolidayList = new CMyList<HOLIDAY_SCHEDULE>;
		for (int i=0; i<ChnnHead.item_head.num; i++)
		{
			scheduleNode.pHolidayList->AddTail(pHoliday[i]);
		}
		delete [] pHoliday;
		pHoliday = NULL;
	}
	m_pScheduleList[itemIdHeadIndex].AddTail(scheduleNode);
	return true;
}


bool CScheduleConfig::GetHolidayScheduleList(CMyList<SCHEDULE_NODE> &scheduleList, unsigned long itemID)
{
	bool bFlg = false;
	unsigned long index = 0;
	for (unsigned long i=0; i<m_cfgHead.idNum; i++)
	{
		SCHEDULE_NODE & tmpScheduleNode = m_pScheduleList[i].GetHead();
		if (itemID == tmpScheduleNode.itemID)
		{
			bFlg = true;
			break;
		}
		index++;
	}

	//在配置中未找到itemID
	if ((!bFlg) || (index >= m_cfgHead.idNum))
	{
		return false;
	}

	//不为空，则进行清除
	if(!scheduleList.IsEmpty())
	{
		POS pos = scheduleList.GetHeadPosition();
		while(NULL != pos)
		{
			SCHEDULE_NODE & scheduleNode = scheduleList.GetNext(pos);
			if(NULL != scheduleNode.pHolidayList)
			{
				scheduleNode.pHolidayList->RemoveAll();
				delete scheduleNode.pHolidayList;
				scheduleNode.pHolidayList = NULL;
			}
		}

		scheduleList.RemoveAll();
	}

	if(m_pScheduleList[index].IsEmpty())
	{
		return true;
	}

	POS pos = m_pScheduleList[index].GetHeadPosition();
	SCHEDULE_NODE schNode;
	while(NULL != pos)
	{
		SCHEDULE_NODE & node = m_pScheduleList[index].GetNext(pos);
		schNode.chnn = node.chnn;
		schNode.itemID = node.itemID;
		schNode.pHolidayList = new CMyList<HOLIDAY_SCHEDULE>;

		if(NULL != node.pHolidayList)
		{
			POS subpos = node.pHolidayList->GetHeadPosition();
			while(NULL != subpos)
			{
				schNode.pHolidayList->AddTail(node.pHolidayList->GetNext(subpos));
			}
		}
		scheduleList.AddTail(schNode);
	}	
	return true;
}

bool CScheduleConfig::ModifyHolidayScheduleList(CMyList<SCHEDULE_NODE> &scheduleList)
{
	SCHEDULE_NODE scheduleNode;
	scheduleNode = scheduleList.GetHead();
	bool bFlg = false;
	unsigned long index=0;
	for (unsigned long i=0; i<m_cfgHead.idNum; i++)
	{
		SCHEDULE_NODE & tmpScheduleNode = m_pScheduleList[i].GetHead();
		if (scheduleNode.itemID == tmpScheduleNode.itemID)
		{
			bFlg = true;
			break;
		}
		index++;
	}

	//在配置中未找到itemID
	if ((!bFlg) || (index >= m_cfgHead.idNum))
	{
		return false;
	}

	if(!m_pScheduleList[index].IsEmpty())
	{
		POS pos = m_pScheduleList[index].GetHeadPosition();
		while(NULL != pos)
		{
			SCHEDULE_NODE & scheduleNode = m_pScheduleList[index].GetNext(pos);
			if(NULL != scheduleNode.pHolidayList)
			{
				scheduleNode.pHolidayList->RemoveAll();	
				delete scheduleNode.pHolidayList;
				scheduleNode.pHolidayList =NULL;
			}
		}
		m_pScheduleList[index].RemoveAll();
	}

	if(scheduleList.IsEmpty())
	{
		return UpdateData();
	}

	POS pos = scheduleList.GetHeadPosition();
	SCHEDULE_NODE schNode;
	while(NULL != pos)
	{
		SCHEDULE_NODE & node = scheduleList.GetNext(pos);
		schNode.chnn = node.chnn;
		schNode.itemID = node.itemID;
		schNode.pHolidayList= new CMyList<HOLIDAY_SCHEDULE>;
		if(NULL != node.pHolidayList)
		{
			POS subpos = node.pHolidayList->GetHeadPosition();
			while(NULL != subpos)
			{
				schNode.pHolidayList->AddTail(node.pHolidayList->GetNext(subpos));
			}
		}
		m_pScheduleList[index].AddTail(schNode);		
	}

	return UpdateData();
}

bool CScheduleConfig::UpdateData()
{
	//change file mode
	fclose(m_pFile);
	m_pFile = fopen(m_pFileName,"wb");
	if(NULL == m_pFile)
	{
		return false;
	}

	unsigned long startPos = SCHEDULE_DATA_POS + sizeof(SCHEDULE_ITEM_ID_HEAD)*m_cfgHead.idNum;
	SCHEDULE_ITEM_ID_HEAD *pItemHead = new SCHEDULE_ITEM_ID_HEAD[m_cfgHead.idNum];
	
	long totalSchNum = 0;
	for(unsigned long i = 0; i < m_cfgHead.idNum; i++)
	{
		totalSchNum += pItemHead[i].item_head.num;
	}
	SCHEDULE_CHNN_HEAD *pChnnHead = new SCHEDULE_CHNN_HEAD[totalSchNum];

	for(unsigned long i = 0; i < m_cfgHead.idNum;i++)
	{
		pItemHead[i].item_head.itemID = (unsigned short)m_pScheduleList[i].GetHead().itemID;
		pItemHead[i].item_head.num = m_pScheduleList[i].GetCount();
		pItemHead[i].item_head.subLen = sizeof(SCHEDULE_CHNN_HEAD);
		pItemHead[i].item_head.len = sizeof(SCHEDULE_CHNN_HEAD)*pItemHead[i].item_head.num;
		pItemHead[i].start_pos = startPos;
		startPos += pItemHead[i].item_head.len;
	}

	rewind(m_pFile);
	if (1 != fwrite(&m_cfgHead, sizeof(SCHEDULE_CFG_HEAD), 1, m_pFile))
	{
		delete[] pItemHead;
		pItemHead = NULL;
		delete[] pChnnHead;
		pChnnHead = NULL;
		return false;
	}

	//写入SCHEDULE_ITEM_ID_HEAD
	if (0 != fseek(m_pFile, SCHEDULE_DATA_POS, SEEK_SET))
	{
		delete[] pItemHead;
		pItemHead = NULL;
		delete[] pChnnHead;
		pChnnHead = NULL;
		return false;
	}

	if (1 != fwrite(pItemHead, m_cfgHead.idNum*sizeof(SCHEDULE_ITEM_ID_HEAD), 1, m_pFile))
	{
		delete[] pItemHead;
		pItemHead = NULL;
		delete[] pChnnHead;
		pChnnHead = NULL;
		return false;
	}

	unsigned long channHeadPos = SCHEDULE_DATA_POS + sizeof(SCHEDULE_ITEM_ID_HEAD)*m_cfgHead.idNum;
	unsigned long dayPos = SCHEDULE_DATA_POS + sizeof(SCHEDULE_ITEM_ID_HEAD)*m_cfgHead.idNum + \
		sizeof(SCHEDULE_CHNN_HEAD)*totalSchNum;
	unsigned long holidayLen = 0;

	SCHEDULE_CHNN_HEAD *pTempHead = pChnnHead;
	for (unsigned long i = 0;i < m_cfgHead.idNum;i++)
	{
		POS pos = m_pScheduleList[i].GetHeadPosition();
		int j = 0;
		while (pos != NULL)
		{
			SCHEDULE_NODE &scheduleNode = m_pScheduleList[i].GetNext(pos);
			int holidayCount =  scheduleNode.pHolidayList->GetCount();
			pTempHead->item_head.itemID = (unsigned short)scheduleNode.chnn;
			pTempHead->item_head.subLen = sizeof(HOLIDAY_SCHEDULE);
			if (holidayCount > 0)
			{
				pTempHead->start_pos = dayPos + holidayLen;
				pTempHead->item_head.len = holidayCount*sizeof(HOLIDAY_SCHEDULE);
				pTempHead->item_head.num = holidayCount;

				//写假日
				POS holidayPos = scheduleNode.pHolidayList->GetHeadPosition();
				while (holidayPos != NULL)
				{
					if (0 != fseek(m_pFile, dayPos+holidayLen, SEEK_SET))
					{
						delete[] pItemHead;
						pItemHead = NULL;
						delete[] pChnnHead;
						pChnnHead = NULL;
						return false;
					}
					HOLIDAY_SCHEDULE & holidaySche = scheduleNode.pHolidayList->GetNext(holidayPos);
					if (1 != fwrite(&holidaySche, sizeof(HOLIDAY_SCHEDULE), 1, m_pFile))
					{
						delete[] pItemHead;
						pItemHead = NULL;
						delete[] pChnnHead;
						pChnnHead = NULL;
						return false;
					}
					holidayLen += sizeof(HOLIDAY_SCHEDULE);
				}
			}
			else
			{
				pTempHead->start_pos = dayPos;
				pTempHead->item_head.len = 0;
				pTempHead->item_head.num = 0;
			}
			pTempHead++;
		}

		//写SCHEDULE_CHNN_HEAD
		if (0 != fseek(m_pFile, channHeadPos, SEEK_SET))
		{
			delete[] pItemHead;
			pItemHead = NULL;
			delete[] pChnnHead;
			pChnnHead = NULL;
			return false;
		}
		if (1 != fwrite(pChnnHead, totalSchNum*sizeof(SCHEDULE_CHNN_HEAD), 1, m_pFile))
		{
			delete[] pItemHead;
			pItemHead = NULL;
			delete[] pChnnHead;
			pChnnHead = NULL;
			return false;
		}		
	}
	fflush(m_pFile);
	delete[] pItemHead;
	pItemHead = NULL;
	delete[] pChnnHead;
	pChnnHead = NULL;
	return true;
}

bool CScheduleConfig::LoadDefault(unsigned char chnnNum, unsigned char sensorInNum, unsigned char sensorOutNum)
{
	if (m_pFile != NULL)
	{
		fclose(m_pFile);
		m_pFile = NULL;
	}
	
	RemoveScheduleList();

	m_pFile = fopen(m_pFileName,"wb");
	if (NULL == m_pFile)
	{
		assert (false);	//出现异常
		return false;
	}

	if(!DefaultConfig(chnnNum, sensorInNum, sensorOutNum))
	{
		fclose(m_pFile);
		m_pFile = NULL;
		return false;
	}
	fclose(m_pFile);

	m_pFile = fopen(m_pFileName,"rb+");
	if (NULL == m_pFile)
	{
		//打开文件失败。
		assert(false);
		return false;
	}

	if (LoadConfig())
	{
		return true;
	}

	return false;
}


