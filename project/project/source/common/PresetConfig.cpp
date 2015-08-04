/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : 袁石维
** Date         : 2008-12-01
** Name         : PresetConfig.cpp
** Version      : 1.0
** Description  :
** Modify Record:

***********************************************************************/
#include "PresetConfig.h"


CPresetConfig::CPresetConfig():m_pFile(NULL), m_pFileName(NULL), m_pPresetList(NULL)
{}

CPresetConfig::~CPresetConfig()
{
}

/*************************************************************************
初始化对象：
如果文件存在，在以只读方式打开文件，然后读取数据。
如果文件不存在，则创建文件，并写入默认配置数据，然后再以只读方式打开文件。
*************************************************************************/
bool CPresetConfig::Initial(unsigned char chnnNum, const char *pFilePath)
{
	m_pFileName = new char [512];
	memset(m_pFileName, 0, 512);
	if (NULL != pFilePath)
	{
#ifdef __ENVIRONMENT_WIN32__
		sprintf(m_pFileName, "%s\\config\\PresetConfig.dat", pFilePath);
#else
		sprintf(m_pFileName, "%s/config/PresetConfig.dat", pFilePath);
#endif
	}
	else
	{
#ifdef __ENVIRONMENT_WIN32__
		strcpy(m_pFileName, "\\config\\PresetConfig.dat");
#else
		strcpy(m_pFileName, "config/PresetConfig.dat");
#endif
	}

	m_pFile = fopen(m_pFileName,"rb+");
	
	if(NULL == m_pFile)
	{
		//没有文件存在，则重新创建文件
		m_pFile = fopen(m_pFileName, "wb");	
		if (NULL == m_pFile)
		{
			delete [] m_pFileName;
			m_pFileName = NULL;
			//创建文件失败。
			assert (false);
			return false;
		}

		//将新创建的文件进行初始化处理
		if(!DefaultConfig(chnnNum))
		{
			fclose(m_pFile);
			m_pFile = NULL;
			delete [] m_pFileName;
			m_pFileName = NULL;
			return false;
		}

		//关闭文件，并重新以只读方式打开。
		fclose(m_pFile);
		m_pFile = fopen(m_pFileName, "rb+");
		if (NULL == m_pFile)
		{
			delete [] m_pFileName;
			m_pFileName = NULL;
			assert (false);
			return false;
		}
	}	

	if (LoadConfig())
	{
		return true;
	}
	else
	{
		fclose(m_pFile);
		m_pFile = NULL;

		delete [] m_pPresetList;
		m_pPresetList = NULL;

		delete [] m_pFileName;
		m_pFileName = NULL;

		return false;
	}
}

void CPresetConfig::Quit()
{
	if(NULL != m_pFile)
	{
		fclose(m_pFile);
		m_pFile = NULL;
	}

	delete [] m_pPresetList;
	m_pPresetList = NULL;

	delete [] m_pFileName;
	m_pFileName = NULL;
}

/*************************************************************************
初始化文件:更新文件头信息,及各通道的INDEX数据,没有任何预置点信息.
2010-02-08 把每个通道默认的预置点个数为MAX_PRESET_NUM, 都为可用状态
预置点文件中数据存放格式为：
----------------------------------------
|			PRESET_CFG_HEAD(16字节)	   |				存放通道数目、文件长度等
----------------------------------------
|			空数据区(32字节)		   |
----------------------------------------
|			PRESET_INDEX * 通道个数    |				每个通道一个PRESET_INDEX，存储该通道的预置点个数、所有预置点的长度、该通道预置点在文件中开始位置
----------------------------------------
|			PTZ_PRESET * 每个通道      |				所有通道预置点的信息
|			最大预置点个数 * 通道个数  |
----------------------------------------
*************************************************************************/
bool CPresetConfig::DefaultConfig(unsigned char chnnNum)
{
	m_cfgHead.chnnNum = chnnNum;
	m_cfgHead.fileLength = PRESET_DATA_POS + sizeof(PRESET_INDEX)*m_cfgHead.chnnNum + sizeof( PTZ_PRESET ) * MAX_PRESET_NUM * m_cfgHead.chnnNum;
	m_cfgHead.version = PRESET_FILE_VER;
	m_cfgHead.headLen = sizeof(PRESET_CFG_HEAD);

	rewind(m_pFile);

	if(1 != fwrite(&m_cfgHead, sizeof(PRESET_CFG_HEAD), 1, m_pFile))
	{
		return false;
	}

	PRESET_INDEX *pIndex = new PRESET_INDEX [m_cfgHead.chnnNum];
	unsigned long startPos = PRESET_DATA_POS + sizeof(PRESET_INDEX)*m_cfgHead.chnnNum;				//第一个预置点在文件中的位置
	for(unsigned char i = 0; i<m_cfgHead.chnnNum; ++i)
	{
		pIndex[i].item_head.itemID = i;
		pIndex[i].item_head.num = MAX_PRESET_NUM;
		pIndex[i].item_head.subLen = sizeof(PTZ_PRESET);
		pIndex[i].item_head.len = ONE_CHNN_PRESET_LEN;												//每个通道数据长度为40*128= 5120 B 
		pIndex[i].start_pos = startPos;	
		startPos += pIndex[i].item_head.len;
	}

	//默认配置信息。
	if(0 == fseek(m_pFile, PRESET_DATA_POS, SEEK_SET))
	{
		if(m_cfgHead.chnnNum != fwrite(pIndex, sizeof(PRESET_INDEX), m_cfgHead.chnnNum, m_pFile))
		{
			delete [] pIndex;
			pIndex = NULL;

			return false;
		}
	}


	PTZ_PRESET	*pPresetOneCh = NULL;

	for( unsigned long i=0; i < m_cfgHead.chnnNum; i++ )
	{
		pPresetOneCh = new PTZ_PRESET [pIndex[i].item_head.num];
		assert( NULL != pPresetOneCh );
		memset( pPresetOneCh, 0, sizeof( PTZ_PRESET ) * pIndex[i].item_head.num );
		assert( pIndex[i].item_head.len == sizeof(PTZ_PRESET) * pIndex[i].item_head.num );
		assert( pIndex[i].start_pos + pIndex[i].item_head.len <= m_cfgHead.fileLength );

		for( int j=0; j < pIndex[i].item_head.num; j++ )
		{
#ifdef __NETSERVER_HXHT__
			pPresetOneCh[j].enable	= 0;
#else
			pPresetOneCh[j].enable	= 1;									//默认打开所有预置点信息
#endif
			pPresetOneCh[j].index	= j;
			sprintf( pPresetOneCh[j].name, "%s%03d", "preset", ( j + 1 ) );
		}

		if( 0 == fseek( m_pFile, pIndex[i].start_pos, SEEK_SET ) )
		{
			if( MAX_PRESET_NUM != fwrite( pPresetOneCh, sizeof( PTZ_PRESET ), pIndex[i].item_head.num, m_pFile ) )		//一次写一个通道的所有预置点信息
			{
				assert( 0 );
				if( NULL != pPresetOneCh )
				{
					delete [] pPresetOneCh;
					pPresetOneCh = NULL;
				}

				if( NULL != pIndex )
				{
					delete [] pIndex;	
					pIndex = NULL;
				}
				return false;
			}
		}

		if( NULL != pPresetOneCh )
		{
			delete [] pPresetOneCh;
			pPresetOneCh = NULL;
		}

	}

	if( NULL != pPresetOneCh )
	{
		delete [] pPresetOneCh;
		pPresetOneCh = NULL;
	}

	if( NULL != pIndex )
	{
		delete [] pIndex;	
		pIndex = NULL;
	}
	
	return true;
}

/*************************************************************************
读取配置信息:
首先加载INDEX数据,根据其信息再加载预置点数据,并以通道为单位建立一个列表.
*************************************************************************/
bool CPresetConfig::LoadConfig()
{
	//先读取头信息
	if (0 != fseek(m_pFile, 0, SEEK_SET))
	{
		return false;
	}
	else
	{
		if (1 != fread(&m_cfgHead, sizeof(PRESET_CFG_HEAD), 1, m_pFile))
		{
			return false;
		}
	}

	assert ((MAX_CHNN_NUM >= m_cfgHead.chnnNum) && (m_cfgHead.chnnNum > 0));
	m_pPresetList	= new CMyList <PTZ_PRESET> [m_cfgHead.chnnNum];
	assert (NULL != m_pPresetList);

	//加载系统参数
	PRESET_INDEX *pIndex = new PRESET_INDEX[m_cfgHead.chnnNum];
	memset(pIndex, 0, m_cfgHead.chnnNum*sizeof(PRESET_INDEX));

	if( 0 == fseek( m_pFile, PRESET_DATA_POS, SEEK_SET ) )
	{
		//读取数据失败。
		if(m_cfgHead.chnnNum != fread(pIndex, sizeof(PRESET_INDEX), m_cfgHead.chnnNum, m_pFile))
		{		
			delete [] pIndex;	
			return false;
		}

		for(unsigned char i=0; i<m_cfgHead.chnnNum; ++i)
		{
			if(0 != pIndex[i].item_head.num)
			{
				PTZ_PRESET *pPreset = new PTZ_PRESET[pIndex[i].item_head.num];
				if(NULL == pPreset)
				{
					delete [] pIndex;
					return false;
				}

				if(0 != fseek(m_pFile, pIndex[i].start_pos, SEEK_SET))
				{
					delete [] pIndex;
					delete [] pPreset;
					pPreset = NULL;
					return false;
				}

				if(pIndex[i].item_head.num != fread(pPreset,sizeof(PTZ_PRESET), pIndex[i].item_head.num, m_pFile))
				{
					delete [] pIndex;
					delete []pPreset;
					pPreset = NULL;
					return false;
				}

				for(unsigned int j=0; j<pIndex[i].item_head.num; ++j)
				{
					m_pPresetList[i].AddTail(pPreset[j]);
				}

				delete [] pPreset;
				pPreset = NULL;
			}
		}

		delete [] pIndex;
		return true;
	}

	delete [] pIndex;
	return false;
}

/*************************************************************************
获取某个通道的预置点信息:
输入参数: chnn--通道号。
输出参数：list--接收列表。必须为空。
*************************************************************************/
bool CPresetConfig::GetPresetList(CMyList <PTZ_PRESET> &list, unsigned char chnn)
{
	assert (chnn < m_cfgHead.chnnNum);
	if (chnn >= m_cfgHead.chnnNum)	//检查通道号，防止越界。
	{
		return false;
	}

	//ZHL 2010-02-23修改：每个通道都有MAX_PRESET_NUM个预置点
	if( MAX_PRESET_NUM != m_pPresetList[chnn].GetCount() )
	{
		assert( false );
		return false;
	}
	//if (m_pPresetList[chnn].IsEmpty())//hz
	//{
	//	list.RemoveAll();
	//	return true;
	//}



	//预置点列表不能为空，且输出列表必须为空。
	if(/*m_pPresetList[chnn].IsEmpty() ||*/ !list.IsEmpty())
	{
		assert(false);
		return false;
	}
	
	POS pos = m_pPresetList[chnn].GetHeadPosition();
	while(NULL != pos)
	{		
		list.AddTail(m_pPresetList[chnn].GetNext(pos));
	}

	return true;
}

/*************************************************************************

调整某个通道的预置点信息:
输入参数: chnn--通道号。
输入参数：list--预置点信息列表。如果为空表示清空该通道所有的预置点。

更改：
1： ZHL 2010-02-08 
list中的预置点列表信息不能空,必须为MAX_PRESET_NUM；
在调用UpdateDate()更新PresetConfig.dat中的数据成功时才改变m_pPresetList[chnn]中缓存的预置点信息；


*************************************************************************/
bool CPresetConfig::ModifyPreset(CMyList <PTZ_PRESET> &list, unsigned char chnn)
{
	assert (chnn < m_cfgHead.chnnNum);
	if (chnn >= m_cfgHead.chnnNum)	//检查通道号，防止越界。
	{
		assert( false );
		return false;
	}

	//列表不能为空， 每个通道都有固定的预置点 2010-02-08
	if( MAX_PRESET_NUM != list.GetCount() )
	{
		assert( false );
		return false;
	}

	//如果输入列表为空，表示要清空所有的预置点。
	//if(list.IsEmpty())
	//{
	//	assert( 0 );
	//	return false;
	//}

	if( UpdateData( list, chnn ) )
	{
		//把原来的内容清除掉。
		if(!m_pPresetList[chnn].IsEmpty())
		{
			m_pPresetList[chnn].RemoveAll();
		}

		POS pos = list.GetHeadPosition();
		while(NULL != pos)
		{
			m_pPresetList[chnn].AddTail(list.GetNext(pos));
		}
		
		return true;
	}
	
	return false;
}

/*************************************************************************

根据当前所有通道的预置点配置信息，更新文件。


更改：
1：	ZHL 2010-02-08.
把预置点的个数固定后只写预置点被改变的通道的预置点
到配置文件，不需要从新写所有通道的预置点信息。因为PresetConfig.dat
中每个通道所需要的数据长度和位置固定了

*************************************************************************/
bool CPresetConfig::UpdateData( CMyList <PTZ_PRESET> &list, unsigned char chnn )
{
	assert(NULL != m_pFile);
	if (NULL == m_pFile)
	{
		return false;
	}
	else
	{
		fclose(m_pFile);	//关闭文件，然后以读写方式打开文件。
		m_pFile = NULL;
	}
	
	//change access-file mode
	m_pFile = fopen(m_pFileName,"r+b");
	if(NULL == m_pFile)
	{
		return false;
	}

	if(0==fseek(m_pFile, PRESET_DATA_POS, SEEK_SET))
	{

		PRESET_INDEX *pIndex = new PRESET_INDEX[m_cfgHead.chnnNum];
		assert( NULL != pIndex );
		memset( pIndex, 0, sizeof( PRESET_INDEX ) * m_cfgHead.chnnNum );

		if( m_cfgHead.chnnNum == fread( pIndex, sizeof( PRESET_INDEX ), m_cfgHead.chnnNum, m_pFile ) )
		{
			assert( pIndex[chnn].item_head.itemID == chnn );
			assert( pIndex[chnn].item_head.num == MAX_PRESET_NUM );
			assert( pIndex[chnn].item_head.len == sizeof( PTZ_PRESET ) * MAX_PRESET_NUM );
			assert( pIndex[chnn].item_head.subLen == sizeof( PTZ_PRESET ) );
			assert( pIndex[chnn].start_pos == PRESET_DATA_POS + sizeof( PRESET_INDEX ) * m_cfgHead.chnnNum + sizeof( PTZ_PRESET ) * MAX_PRESET_NUM * chnn );
			assert( list.GetCount() * sizeof( PTZ_PRESET ) == pIndex[chnn].item_head.len );

			if( 0 == fseek( m_pFile, pIndex[chnn].start_pos, SEEK_SET ) )
			{
				PTZ_PRESET *pPreset = new PTZ_PRESET[pIndex[chnn].item_head.num];
				assert( NULL != pPreset );
				memset( pPreset, 0, sizeof( PTZ_PRESET ) * pIndex[chnn].item_head.num );
				PTZ_PRESET *pPresetTemp = pPreset;

				POS pos = list.GetHeadPosition();
				while(NULL != pos)
				{
					*pPresetTemp = list.GetNext(pos);
					pPresetTemp++;

				}

				assert( pPresetTemp == pPreset + pIndex[chnn].item_head.num );
				if( pIndex[chnn].item_head.num == fwrite( pPreset, sizeof( PTZ_PRESET ), pIndex[chnn].item_head.num, m_pFile ) )
				{
#ifndef __RELEASE__
					unsigned long filelen = 0;
					if(0 == fseek(m_pFile,0,SEEK_END))
					{
						filelen = ftell(m_pFile);
					}
					assert( m_cfgHead.fileLength = filelen );
#endif

					delete [] pIndex;
					pIndex = NULL;

					delete [] pPreset;
					pPreset = NULL;
					FileFlush(m_pFile);
					return true;
				}
				else
				{
					delete [] pPreset;
					pPreset = NULL;
				}

			}
		}

		if( NULL != pIndex )
		{
			delete [] pIndex;
			pIndex = NULL;
		}
	}

	
	//一次把各通道的配置点信息写入文件。
	//for(unsigned char j=0; j<m_cfgHead.chnnNum; ++j)
	//{
	//	if(0 == fseek(m_pFile, pIndex[j].start_pos, SEEK_SET))
	//	{
	//		POS pos = m_pPresetList[j].GetHeadPosition();
	//		while(NULL != pos)
	//		{
	//			PTZ_PRESET & preset = m_pPresetList[j].GetNext(pos);
	//			if(1 != fwrite(&preset, sizeof(PTZ_PRESET), 1, m_pFile))
	//			{
	//				return false;
	//			}
	//		}
	//	}
	//}



	////更新文件头信息。
	//m_cfgHead.fileLength = filelen;

	//rewind(m_pFile);
	//if(1 != fwrite(&m_cfgHead, sizeof(PRESET_CFG_HEAD), 1, m_pFile))
	//{
	//	return false;
	//}
	
	return false;
}


bool CPresetConfig::LoadDefault(unsigned char chnnNum)
{
	if (m_pFile != NULL)
	{
		fclose(m_pFile);
		m_pFile = NULL;
	}

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

	FileFlush(m_pFile);
	return false;
}

//end
