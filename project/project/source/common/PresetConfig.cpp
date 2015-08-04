/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : Ԭʯά
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
��ʼ������
����ļ����ڣ�����ֻ����ʽ���ļ���Ȼ���ȡ���ݡ�
����ļ������ڣ��򴴽��ļ�����д��Ĭ���������ݣ�Ȼ������ֻ����ʽ���ļ���
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
		//û���ļ����ڣ������´����ļ�
		m_pFile = fopen(m_pFileName, "wb");	
		if (NULL == m_pFile)
		{
			delete [] m_pFileName;
			m_pFileName = NULL;
			//�����ļ�ʧ�ܡ�
			assert (false);
			return false;
		}

		//���´������ļ����г�ʼ������
		if(!DefaultConfig(chnnNum))
		{
			fclose(m_pFile);
			m_pFile = NULL;
			delete [] m_pFileName;
			m_pFileName = NULL;
			return false;
		}

		//�ر��ļ�����������ֻ����ʽ�򿪡�
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
��ʼ���ļ�:�����ļ�ͷ��Ϣ,����ͨ����INDEX����,û���κ�Ԥ�õ���Ϣ.
2010-02-08 ��ÿ��ͨ��Ĭ�ϵ�Ԥ�õ����ΪMAX_PRESET_NUM, ��Ϊ����״̬
Ԥ�õ��ļ������ݴ�Ÿ�ʽΪ��
----------------------------------------
|			PRESET_CFG_HEAD(16�ֽ�)	   |				���ͨ����Ŀ���ļ����ȵ�
----------------------------------------
|			��������(32�ֽ�)		   |
----------------------------------------
|			PRESET_INDEX * ͨ������    |				ÿ��ͨ��һ��PRESET_INDEX���洢��ͨ����Ԥ�õ����������Ԥ�õ�ĳ��ȡ���ͨ��Ԥ�õ����ļ��п�ʼλ��
----------------------------------------
|			PTZ_PRESET * ÿ��ͨ��      |				����ͨ��Ԥ�õ����Ϣ
|			���Ԥ�õ���� * ͨ������  |
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
	unsigned long startPos = PRESET_DATA_POS + sizeof(PRESET_INDEX)*m_cfgHead.chnnNum;				//��һ��Ԥ�õ����ļ��е�λ��
	for(unsigned char i = 0; i<m_cfgHead.chnnNum; ++i)
	{
		pIndex[i].item_head.itemID = i;
		pIndex[i].item_head.num = MAX_PRESET_NUM;
		pIndex[i].item_head.subLen = sizeof(PTZ_PRESET);
		pIndex[i].item_head.len = ONE_CHNN_PRESET_LEN;												//ÿ��ͨ�����ݳ���Ϊ40*128= 5120 B 
		pIndex[i].start_pos = startPos;	
		startPos += pIndex[i].item_head.len;
	}

	//Ĭ��������Ϣ��
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
			pPresetOneCh[j].enable	= 1;									//Ĭ�ϴ�����Ԥ�õ���Ϣ
#endif
			pPresetOneCh[j].index	= j;
			sprintf( pPresetOneCh[j].name, "%s%03d", "preset", ( j + 1 ) );
		}

		if( 0 == fseek( m_pFile, pIndex[i].start_pos, SEEK_SET ) )
		{
			if( MAX_PRESET_NUM != fwrite( pPresetOneCh, sizeof( PTZ_PRESET ), pIndex[i].item_head.num, m_pFile ) )		//һ��дһ��ͨ��������Ԥ�õ���Ϣ
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
��ȡ������Ϣ:
���ȼ���INDEX����,��������Ϣ�ټ���Ԥ�õ�����,����ͨ��Ϊ��λ����һ���б�.
*************************************************************************/
bool CPresetConfig::LoadConfig()
{
	//�ȶ�ȡͷ��Ϣ
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

	//����ϵͳ����
	PRESET_INDEX *pIndex = new PRESET_INDEX[m_cfgHead.chnnNum];
	memset(pIndex, 0, m_cfgHead.chnnNum*sizeof(PRESET_INDEX));

	if( 0 == fseek( m_pFile, PRESET_DATA_POS, SEEK_SET ) )
	{
		//��ȡ����ʧ�ܡ�
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
��ȡĳ��ͨ����Ԥ�õ���Ϣ:
�������: chnn--ͨ���š�
���������list--�����б�����Ϊ�ա�
*************************************************************************/
bool CPresetConfig::GetPresetList(CMyList <PTZ_PRESET> &list, unsigned char chnn)
{
	assert (chnn < m_cfgHead.chnnNum);
	if (chnn >= m_cfgHead.chnnNum)	//���ͨ���ţ���ֹԽ�硣
	{
		return false;
	}

	//ZHL 2010-02-23�޸ģ�ÿ��ͨ������MAX_PRESET_NUM��Ԥ�õ�
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



	//Ԥ�õ��б���Ϊ�գ�������б����Ϊ�ա�
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

����ĳ��ͨ����Ԥ�õ���Ϣ:
�������: chnn--ͨ���š�
���������list--Ԥ�õ���Ϣ�б����Ϊ�ձ�ʾ��ո�ͨ�����е�Ԥ�õ㡣

���ģ�
1�� ZHL 2010-02-08 
list�е�Ԥ�õ��б���Ϣ���ܿ�,����ΪMAX_PRESET_NUM��
�ڵ���UpdateDate()����PresetConfig.dat�е����ݳɹ�ʱ�Ÿı�m_pPresetList[chnn]�л����Ԥ�õ���Ϣ��


*************************************************************************/
bool CPresetConfig::ModifyPreset(CMyList <PTZ_PRESET> &list, unsigned char chnn)
{
	assert (chnn < m_cfgHead.chnnNum);
	if (chnn >= m_cfgHead.chnnNum)	//���ͨ���ţ���ֹԽ�硣
	{
		assert( false );
		return false;
	}

	//�б���Ϊ�գ� ÿ��ͨ�����й̶���Ԥ�õ� 2010-02-08
	if( MAX_PRESET_NUM != list.GetCount() )
	{
		assert( false );
		return false;
	}

	//��������б�Ϊ�գ���ʾҪ������е�Ԥ�õ㡣
	//if(list.IsEmpty())
	//{
	//	assert( 0 );
	//	return false;
	//}

	if( UpdateData( list, chnn ) )
	{
		//��ԭ���������������
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

���ݵ�ǰ����ͨ����Ԥ�õ�������Ϣ�������ļ���


���ģ�
1��	ZHL 2010-02-08.
��Ԥ�õ�ĸ����̶���ֻдԤ�õ㱻�ı��ͨ����Ԥ�õ�
�������ļ�������Ҫ����д����ͨ����Ԥ�õ���Ϣ����ΪPresetConfig.dat
��ÿ��ͨ������Ҫ�����ݳ��Ⱥ�λ�ù̶���

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
		fclose(m_pFile);	//�ر��ļ���Ȼ���Զ�д��ʽ���ļ���
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

	
	//һ�ΰѸ�ͨ�������õ���Ϣд���ļ���
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



	////�����ļ�ͷ��Ϣ��
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
		assert (false);	//�����쳣
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
		//���ļ�ʧ�ܡ�
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
