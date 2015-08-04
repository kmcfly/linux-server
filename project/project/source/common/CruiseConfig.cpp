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
#include "CruiseConfig.h"
#include "Product.h"

CCruiseConfig::CCruiseConfig():m_pFile(NULL), m_pFileName(NULL), m_pCruiseList(NULL)
{
}

CCruiseConfig::~CCruiseConfig()
{

}

bool CCruiseConfig::Initial(unsigned char chnnNum, const char *pFilePath)
{
	assert (chnnNum > 0);

	m_pFileName = new char [512];
	memset(m_pFileName, 0, 512);
	if (NULL != pFilePath)
	{
#ifdef __ENVIRONMENT_WIN32__
		sprintf(m_pFileName, "%s\\config\\CruiseConfig.dat", pFilePath);
#else
		sprintf(m_pFileName, "%s/config/CruiseConfig.dat", pFilePath);
#endif
	}
	else
	{
#ifdef __ENVIRONMENT_WIN32__
		strcpy(m_pFileName, "\\config\\CruiseConfig.dat");
#else
		strcpy(m_pFileName, "config/CruiseConfig.dat");
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

		if(!DefaultConfig(chnnNum))
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
		RemoveCruiseList();

		fclose(m_pFile);
		m_pFile = NULL;

		delete [] m_pCruiseList;
		m_pCruiseList = NULL;

		delete [] m_pFileName;
		m_pFileName = NULL;
		return false;
	}
}

void CCruiseConfig::Quit()
{
	if(NULL != m_pFile)
	{
		fclose(m_pFile);
		m_pFile = NULL;
	}

	RemoveCruiseList();

	delete [] m_pCruiseList;
	m_pCruiseList = NULL;
	delete [] m_pFileName;
	m_pFileName = NULL;
}

void CCruiseConfig::RemoveCruiseList()
{
	if (NULL != m_pCruiseList)
	{
		for(unsigned char i=0; i<m_cfgHead.channelNum; ++i)
		{
			if(!m_pCruiseList[i].IsEmpty())
			{
				POS pos = m_pCruiseList[i].GetHeadPosition();

				while(NULL != pos)
				{
					CRUISE &cruise = m_pCruiseList[i].GetNext(pos);
					if(NULL != cruise.pointList)
					{
						cruise.pointList->RemoveAll();
						delete cruise.pointList;
						cruise.pointList = NULL;
					}				
				}

				m_pCruiseList[i].RemoveAll();
			}
		}
	}
}

bool CCruiseConfig::DefaultConfig(unsigned char chnnNum)
{
	m_cfgHead.channelNum = chnnNum;
	m_cfgHead.fileLength = CRUISE_DATA_POS + sizeof(CRUISE_INDEX)*m_cfgHead.channelNum;
	m_cfgHead.version = CRUISE_FILE_VER;
	m_cfgHead.headLen = sizeof(CRUISE_CFG_HEAD);

	rewind(m_pFile);

	if(1 != fwrite(&m_cfgHead, sizeof(CRUISE_CFG_HEAD), 1, m_pFile))
	{
		return false;
	}

	CRUISE_INDEX *pCruiseIndex = new CRUISE_INDEX [m_cfgHead.channelNum];
	for(unsigned int i = 0;i < m_cfgHead.channelNum;i++)
	{
		pCruiseIndex[i].item_head.itemID = i;
		pCruiseIndex[i].item_head.num = 0;
		pCruiseIndex[i].item_head.subLen = sizeof(CRUISE_POINT_INDEX);
		pCruiseIndex[i].item_head.len = 0;//sublen*num = 0
		pCruiseIndex[i].start_pos = m_cfgHead.fileLength;
	}

	if(0 == fseek(m_pFile, CRUISE_DATA_POS, SEEK_SET))
	{
		if(m_cfgHead.channelNum != fwrite(pCruiseIndex, sizeof(CRUISE_INDEX), m_cfgHead.channelNum, m_pFile))
		{
			delete [] pCruiseIndex;
			return false;
		}
	}	
	
	delete [] pCruiseIndex;
	return true;
}

bool CCruiseConfig::LoadConfig()
{
	//先读取头信息
	if (0 != fseek(m_pFile, 0, SEEK_SET))
	{
		return false;
	}
	else
	{
		if (1 != fread(&m_cfgHead, sizeof(CRUISE_CFG_HEAD), 1, m_pFile))
		{
			return false;
		}
	}

	assert ((MAX_CHNN_NUM >= m_cfgHead.channelNum) && (m_cfgHead.channelNum > 0));
	m_pCruiseList	= new CMyList <CRUISE> [m_cfgHead.channelNum];
	assert (NULL != m_pCruiseList);

	CRUISE_INDEX *pCruiseIndex = new CRUISE_INDEX [m_cfgHead.channelNum];
	memset(pCruiseIndex, 0, m_cfgHead.channelNum*sizeof(CRUISE_INDEX));

	if(0 == fseek(m_pFile, CRUISE_DATA_POS, SEEK_SET))
	{
		if(m_cfgHead.channelNum != fread(pCruiseIndex, sizeof(CRUISE_INDEX), m_cfgHead.channelNum, m_pFile))
		{
			delete [] pCruiseIndex;
			return false;
		}

		for(unsigned char i=0; i<m_cfgHead.channelNum; ++i)
		{
			assert (i == pCruiseIndex[i].item_head.itemID);

			if(pCruiseIndex[i].item_head.num > 0)
			{	
				if(!LoadCruiseIndex(pCruiseIndex[i]))
				{
					delete [] pCruiseIndex;
					return false;
				}
			}
		}
		
		delete [] pCruiseIndex;
		return true;
	}

	delete [] pCruiseIndex;
	return false;
}

bool CCruiseConfig::LoadCruiseIndex(const CRUISE_INDEX & cruiseIndex)
{
	if(0 == fseek(m_pFile, cruiseIndex.start_pos, SEEK_SET))
	{
		CRUISE_POINT_INDEX *pPointIndex = new CRUISE_POINT_INDEX[cruiseIndex.item_head.num];
		if(cruiseIndex.item_head.num != fread(pPointIndex, sizeof(CRUISE_POINT_INDEX),\
			cruiseIndex.item_head.num, m_pFile))
		{
			delete [] pPointIndex;
			pPointIndex = NULL;

			return false;
		}

		for(unsigned short i=0; i<cruiseIndex.item_head.num; ++i)
		{
			if(!LoadCruisePoint(pPointIndex[i], cruiseIndex.item_head.itemID))
			{
				delete []pPointIndex;
				pPointIndex = NULL;
				return false;
			}
		}	

		delete []pPointIndex;
		pPointIndex = NULL;
		return true;
	}

	return false;
}

bool CCruiseConfig::LoadCruisePoint(const CRUISE_POINT_INDEX & pointIndex,unsigned long chnn)
{
	if (pointIndex.item_head.num > 0)
	{
		if(0 == fseek(m_pFile, pointIndex.start_pos, SEEK_SET))
		{
			CRUISE cruise;
			memset(&cruise, 0, sizeof(CRUISE));
			cruise.pointList = NULL;

			cruise.index = pointIndex.item_head.itemID;
			strcpy(cruise.name, pointIndex.name);

			PTZ_CURISE_POINT *pCruisePoint = new PTZ_CURISE_POINT[pointIndex.item_head.num];
			if(NULL == pCruisePoint)
			{
				return false;
			}

			if(pointIndex.item_head.num != fread(pCruisePoint, sizeof(PTZ_CURISE_POINT),\
				pointIndex.item_head.num, m_pFile))
			{
				delete []pCruisePoint;
				pCruisePoint = NULL;
				return false;
			}

			cruise.pointList = new CMyList<PTZ_CURISE_POINT>;
			for(unsigned int i = 0;i < pointIndex.item_head.num;i++)
			{
				cruise.pointList->AddTail(pCruisePoint[i]);
			}

			delete []pCruisePoint;
			pCruisePoint = NULL;

			m_pCruiseList[chnn].AddTail(cruise);			
			return true;
		}

		return false;
	}
	else
	{
		return true;
	}
}

bool CCruiseConfig::GetCruiseList(CMyList<CRUISE> &curise, unsigned char chnn)
{
	assert (chnn < m_cfgHead.channelNum);
	if (chnn >= m_cfgHead.channelNum)
	{
		return false;
	}

	//不为空，则进行清楚
	if(!curise.IsEmpty())
	{
		POS pos = curise.GetHeadPosition();
		while(NULL != pos)
		{
			CRUISE &lcruise = curise.GetNext(pos);
			if(NULL != lcruise.pointList)
			{
				lcruise.pointList->RemoveAll();
				delete lcruise.pointList;
				lcruise.pointList = NULL;
			}
		}

		curise.RemoveAll();
	}

	if(m_pCruiseList[chnn].IsEmpty())
	{
		return true;
	}

	POS pos = m_pCruiseList[chnn].GetHeadPosition();
	CRUISE lcruise;
	while(NULL != pos)
	{
		CRUISE & node = m_pCruiseList[chnn].GetNext(pos);
		lcruise = node;
		lcruise.pointList = new CMyList<PTZ_CURISE_POINT>;

		if(NULL != node.pointList)
		{
			POS subpos = node.pointList->GetHeadPosition();
			while(NULL != subpos)
			{
				lcruise.pointList->AddTail(node.pointList->GetNext(subpos));
			}
		}

		curise.AddTail(lcruise);
	}	

	return true;
}

bool CCruiseConfig::ModifyCruiseList(CMyList<CRUISE> &curise,unsigned char chnn)
{
	assert (chnn < m_cfgHead.channelNum);
	if (chnn >= m_cfgHead.channelNum)
	{
		return false;
	}

	if(!m_pCruiseList[chnn].IsEmpty())
	{
		POS pos = m_pCruiseList[chnn].GetHeadPosition();
		while(NULL != pos)
		{
			CRUISE &lcruise = m_pCruiseList[chnn].GetNext(pos);
			if(NULL != lcruise.pointList)
			{
				lcruise.pointList->RemoveAll();	
				delete lcruise.pointList;
				lcruise.pointList =NULL;
			}
		}
		m_pCruiseList[chnn].RemoveAll();
	}

	if(curise.IsEmpty())
	{
		return UpdateData();
	}

	POS pos = curise.GetHeadPosition();
	CRUISE lcruise;
	while(NULL != pos)
	{
		CRUISE & node = curise.GetNext(pos);
		lcruise = node;
		lcruise.pointList = new CMyList<PTZ_CURISE_POINT>;
		if(NULL != node.pointList)
		{
			POS subpos = node.pointList->GetHeadPosition();
			while(NULL != subpos)
			{
				lcruise.pointList->AddTail(node.pointList->GetNext(subpos));
			}
		}
		m_pCruiseList[chnn].AddTail(lcruise);		
	}

	return UpdateData();
}

bool CCruiseConfig::UpdateData()
{
	//change file mode
	fclose(m_pFile);
	m_pFile = fopen(m_pFileName,"wb");
	if(NULL == m_pFile)
	{
		return false;
	}

	unsigned long pos = CRUISE_DATA_POS + sizeof(CRUISE_INDEX)*m_cfgHead.channelNum;
	CRUISE_INDEX *pIndex = new CRUISE_INDEX [m_cfgHead.channelNum];

	//cruise list index
	{ ////////限制i的作用域
		for(unsigned int i = 0;i < m_cfgHead.channelNum;i++)
		{
			pIndex[i].item_head.itemID = i;
			pIndex[i].item_head.num = m_pCruiseList[i].GetCount();
			pIndex[i].item_head.subLen = sizeof(CRUISE_POINT_INDEX);
			pIndex[i].item_head.len = pIndex[i].item_head.num*pIndex[i].item_head.subLen;
			pIndex[i].start_pos = pos;//tmpIndex.start_pos + tmpIndex.item_head.len;
			pos += pIndex[i].item_head.len;
		}
	}

	
	unsigned int singleCruiseNum = 0;

	{ ///////限制i的作用域
		for(unsigned int i = 0;i < m_cfgHead.channelNum;i++)
		{
			singleCruiseNum += pIndex[i].item_head.num;	
		}
	}


	CRUISE_POINT_INDEX *pPointIndextb = new CRUISE_POINT_INDEX[singleCruiseNum];
	if(NULL == pPointIndextb)
	{
		delete [] pIndex;
		pIndex = NULL;

		return false;
	}
	unsigned int Pointcount = 0;
	unsigned long DataWrite_StartPos = pos;//tmpIndex.start_pos + tmpIndex.item_head.len;

	//single cruise point index;	
	for(unsigned int i = 0;i < m_cfgHead.channelNum;i++)
	{
		if(pIndex[i].item_head.num == 0)
			continue;
		
		POS pos = m_pCruiseList[i].GetHeadPosition();
		CRUISE tmpcruise;
		while(NULL != pos)
		{
			tmpcruise = m_pCruiseList[i].GetNext(pos);
			pPointIndextb[Pointcount].item_head.itemID = static_cast<unsigned short>(tmpcruise.index);//指明那一条巡航线路
			strcpy(pPointIndextb[Pointcount].name,tmpcruise.name);	
			if(NULL == tmpcruise.pointList)
			{
				pPointIndextb[Pointcount].item_head.num = 0;
			}
			else
			{
				pPointIndextb[Pointcount].item_head.num = (tmpcruise.pointList->GetCount());
			}			
			pPointIndextb[Pointcount].item_head.subLen = sizeof(PTZ_CURISE_POINT);
			pPointIndextb[Pointcount].item_head.len = pPointIndextb[Pointcount].item_head.num*\
				pPointIndextb[Pointcount].item_head.subLen;
			pPointIndextb[Pointcount].start_pos = Pointcount == 0 ? DataWrite_StartPos:\
				pPointIndextb[Pointcount - 1].start_pos + pPointIndextb[Pointcount - 1].item_head.len;
			if(pPointIndextb[Pointcount].item_head.num)
			{
				WriteOneCruise(tmpcruise,pPointIndextb[Pointcount].start_pos);
			}
			Pointcount++;
			assert(Pointcount <= singleCruiseNum);			
		}		
	}

	if(0 != fseek(m_pFile,CRUISE_DATA_POS,SEEK_SET))
	{
		delete [] pIndex;
		pIndex = NULL;

		delete []pPointIndextb;
		pPointIndextb = NULL;
		return false;
	}
	//通道索引
	if(m_cfgHead.channelNum != fwrite(pIndex,sizeof(CRUISE_INDEX),m_cfgHead.channelNum,m_pFile))
	{
		delete [] pIndex;
		pIndex = NULL;

		delete []pPointIndextb;
		pPointIndextb = NULL;
		return false;
	}
	//每个通道的各个巡航线索引
	if(singleCruiseNum != fwrite(pPointIndextb,sizeof(CRUISE_POINT_INDEX),singleCruiseNum,m_pFile))
	{
		delete [] pIndex;
		pIndex = NULL;

		delete []pPointIndextb;
		pPointIndextb = NULL;
		return false;
	}

	delete [] pIndex;
	pIndex = NULL;

	delete []pPointIndextb;
	pPointIndextb = NULL;

	unsigned filelen = 0;
	if(0 == fseek(m_pFile,0,SEEK_END))
	{
		filelen = ftell(m_pFile);
	}

	//CRUISE_CFG_HEAD cruiseHead;
	//cruiseHead.fileLength = filelen;
	//cruiseHead.headLen = sizeof(CRUISE_CFG_HEAD);
	//cruiseHead.version = CRUISE_FILE_VER;
	rewind(m_pFile);

	if(1 != fwrite(&m_cfgHead,sizeof(CRUISE_CFG_HEAD),1,m_pFile))
	{
		return false;
	}
	FileFlush(m_pFile);

	return true;
}

bool CCruiseConfig::WriteOneCruise(const CRUISE &cruise,unsigned long startPos)
{
	PTZ_CURISE_POINT *pPoint = new PTZ_CURISE_POINT[cruise.pointList->GetCount()];
	assert(NULL != pPoint && NULL != cruise.pointList);
	if(NULL == pPoint)
	{
		return false;
	}
	POS pos = cruise.pointList->GetHeadPosition();
	unsigned int index = 0;
	while(NULL != pos)
	{
		pPoint[index++] = cruise.pointList->GetNext(pos); 
	}

	if(0 != fseek(m_pFile,startPos,SEEK_SET))
	{
		delete []pPoint;
		pPoint = NULL;
		return false;	
	}
	if(index != fwrite(pPoint,sizeof(PTZ_CURISE_POINT),cruise.pointList->GetCount(),m_pFile))
	{
		delete []pPoint;
		pPoint = NULL;
		return false;
	}

	delete []pPoint;
	pPoint = NULL;
	return true;
}


bool CCruiseConfig::LoadDefault(unsigned char chnnNum)
{
	if (m_pFile != NULL)
	{
		fclose(m_pFile);
		m_pFile = NULL;
	}

	RemoveCruiseList();

	m_pFile = fopen(m_pFileName,"wb");
	if (NULL == m_pFile)
	{
		assert (false);	//出现异常
		return false;
	}

	if(!DefaultConfig(chnnNum))
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

