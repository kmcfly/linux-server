/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : Ԭʯά
** Date         : 2008-12-11
** Name         : ReclogEx.cpp
** Version      : 1.0
** Description  :
** Modify Record:
***********************************************************************/
#include "ReclogEx.h"
#include "Product.h"
#include "MessageMan.h"
#include "StreamRecordManEx.h"

/**************************�ش��޸ļ�¼*********************************
//2010-02-27 09:17:00 YSW
�����������������ļ��Ŀ�ʼʱ�䡢����ʱ�䣻��һ�εĿ�ʼʱ�䡢���һ�εĽ���ʱ�䣻��һ���¼�
		�Ŀ�ʼʱ�䡢���һ���¼��Ľ���ʱ�䶼��Ӧ���������ʱ¼���ļ��е�����֡Ҳ�ʹ������Ӧ����
		�����м�Ķκ��¼��ļ�¼�����ϴ�¼�񣨱��θ���ǰ�������ݡ����ݷ����ó��������£�
1����ǰ�޸�ʱ�䣨ʱ��Ҫ�ڶ�Ӧ�ļ��Ŀ�ʼʱ��֮ǰ�����������¼���ʱ���ļ��ᱻ���˵���������
��Ч�ļ��б�������reclog.bin�е�����δ����д��
2����ʼ¼����ô�ܻ��õ������ļ�����ʱд��һ����ʼ���ļ���Ϣ���Ρ��¼������޸ģ�������ʱ��
���ϴ�¼��д��Ŀ϶��в��졣
3���쳣���������ļ���Ҫ�޸���Ȼ�������ȡreclog.bin�е����ݽ��з�������Ϊ�޸ĺ�д�˿�ʼʱ�䣩
�����ݲ������ϸ������ϵĴ������Ի��ҵ��ϴ�¼��д�Ķκ��¼������һ�������޸������޸Ľ���ʱ�䣩
������һ���ͳ��������е������
����취����д�κ�д�¼�ʱ������ǵ�һ����¼�����ǿ�ʼ������Ѷ�Ӧ����Ϣ����ȫ�����㡣
***********************************************************************/
//////////////////////////////////////////////////////////////////////////
unsigned long MinTime()
{
	tm time;
	memset(&time, 0, sizeof(tm));

	time.tm_year	= 2008 - 1900;
	time.tm_mon		= 1 - 1;
	time.tm_mday	= 1;

	return TmToDVRTime32(time);
}

unsigned long MaxTime()
{
	tm time;
	memset(&time, 0, sizeof(tm));

	time.tm_year	= 2025 - 1900;
	time.tm_mon		= 12 - 1;
	time.tm_mday	= 31;
	time.tm_hour	= 23;
	time.tm_min		= 59;
	time.tm_sec		= 59;

	return TmToDVRTime32(time);
}

unsigned long CReclogEx::m_minTime = MinTime();
unsigned long CReclogEx::m_maxTime = MaxTime();

unsigned char * CReclogEx::m_pCleanDataBuf = NULL;
/////////////////////////////////////////////////////////////////////////////////////////////
bool CReclogEx::CreateLogFile(const char *pFilePath, unsigned long fileNum, unsigned char videoInputNum,bool bIsBackUpCreated/* = false*/)
{
	assert (NULL != pFilePath);
	assert (fileNum > 0);
	assert (videoInputNum > 0);

	unsigned char netVideoInputNum = CProduct::Instance()->NetVideoInputNum();
	unsigned char localVideoInputNum = CProduct::Instance()->LocalVideoInputNum();
	if(!bIsBackUpCreated)
	{
		if(CStreamRecordManEx::Instance()->IsSupportMinorStreamRecord() && netVideoInputNum>0)
		{
			videoInputNum += netVideoInputNum;
			printf("CReclogEx::CreateLogFile-1:%u\n",videoInputNum);
		}
	}
	//����¼������ʱ����Ŀ�Ĵ�����������һ����־�ļ���
	//��Ϊ����Ҫ����С�����ļ�
	//������־�ļ���ͨ����Ŀ����Ϊ��������ͨ����.
	else
	{
		videoInputNum = netVideoInputNum+localVideoInputNum;
	}

	if ((NULL == pFilePath) || (0 == fileNum) || (0 == videoInputNum))
	{
		return false;
	}

	FILE *pFile = fopen (pFilePath, "wb+");
	if (NULL == pFile)
	{
		return false;
	}

	//��ʼ��ͷ��Ϣ
	{
		RECLOG_FILE_HEAD_INFO headInfo;

		headInfo.headFlag		= 0;
		headInfo.version		= RECLOG_EX_FILE_VER;
		headInfo.isNormalClose	= 1;
		headInfo.chnnNum		= videoInputNum;
		headInfo.fileNum		= fileNum;
		headInfo.fileRecSecNum	= MAX_SECTION_NUM;
		headInfo.fileLogNum		= MAX_RECLOG_NUM;

		headInfo.fileInfoOffset	= FILE_INFO_OFFSET;
		headInfo.sectionOffset	= FILE_INFO_OFFSET + sizeof(FILE_FILE_INFO)*fileNum + 32;
		headInfo.reclogoffset	= headInfo.sectionOffset + sizeof(SECTION_INFO_EX)*MAX_SECTION_NUM*fileNum+ 32;

		if (1 != fwrite(&headInfo, sizeof(RECLOG_FILE_HEAD_INFO), 1, pFile))
		{
			Release (&pFile, 0);
			return false;
		}
	}

	//��ʼ���ļ���Ϣ

	{
		if (0 != fseek(pFile, FILE_INFO_OFFSET, SEEK_SET))
		{
			Release(&pFile, 0);
			return false;
		}

		FILE_FILE_INFO * pData = new FILE_FILE_INFO [fileNum];
		assert (NULL != pData);
		if (NULL == pData)
		{
			Release(&pFile, 0);
			return false;
		}

		memset (pData, 0, sizeof(FILE_FILE_INFO)*fileNum);
		if (fileNum != fwrite(pData, sizeof(FILE_FILE_INFO), fileNum, pFile))
		{
			Release(&pFile, 1, &pData);
			return false;
		}

		delete [] pData;
		pData = NULL;
	}

	//��ʼ���ļ�����Ϣ
	{
		if (0 != fseek(pFile, FILE_INFO_OFFSET + sizeof(FILE_FILE_INFO)*fileNum + 32, SEEK_SET))
		{
			Release(&pFile, 0);
			return false;
		}

		unsigned long num = MAX_BUFFER_LEN/sizeof(SECTION_INFO_EX);
		SECTION_INFO_EX *pData = new SECTION_INFO_EX [num];
		assert (NULL != pData);
		if (NULL == pData)
		{
			Release(&pFile, 0);
			return false;
		}
		memset (pData, 0, sizeof (SECTION_INFO_EX)*num);

		unsigned long loopNum = (MAX_SECTION_NUM * fileNum) / num;
		for (unsigned long i=0; i<loopNum; ++i)
		{
			if (num != fwrite(pData, sizeof(SECTION_INFO_EX), num, pFile))
			{
				Release(&pFile, 1, &pData);
				return false;
			}
		}

		num = (MAX_SECTION_NUM * fileNum) % num;
		if (num > 0)
		{
			if (num != fwrite(pData, sizeof(SECTION_INFO_EX), num, pFile))
			{
				Release(&pFile, 1, &pData);
				return false;
			}
		}

		delete [] pData;
		pData = NULL;
	}

	//��ʼ���ļ��¼���

	{
		if (0 != fseek(pFile, FILE_INFO_OFFSET + sizeof(FILE_FILE_INFO)*fileNum + 32 + \
			sizeof(SECTION_INFO_EX)*MAX_SECTION_NUM*fileNum+ 32, SEEK_SET))
		{
			Release(&pFile, 0);
			return false;
		}

		unsigned long num = MAX_BUFFER_LEN/sizeof(FILE_RECLOG_INFO);
		FILE_RECLOG_INFO *pData = new FILE_RECLOG_INFO [num];
		assert (NULL != pData);
		if (NULL == pData)
		{
			Release(&pFile, 0);
			return false;
		}
		memset (pData, 0, sizeof (FILE_RECLOG_INFO)*num);

		unsigned long loopNum = (MAX_RECLOG_NUM * fileNum) / num;
		for (unsigned long i=0; i<loopNum; ++i)
		{
			if (num != fwrite(pData, sizeof(FILE_RECLOG_INFO), num, pFile))
			{
				Release(&pFile, 1, &pData);
				return false;
			}
		}

		num = (MAX_RECLOG_NUM * fileNum) % num;
		if (num != fwrite(pData, sizeof(FILE_RECLOG_INFO), num, pFile))
		{
			Release(&pFile, 1, &pData);
			return false;
		}

		delete [] pData;
		pData = NULL;
	}

	Release(&pFile, 0);
	return true;
}
/////////////////////////////////////////////////////////////////////////////////////////////
CReclogEx::CReclogEx ():m_diskIndex(0), m_pFile(NULL), m_pFilePath(NULL), \
		m_pFileInfoList(NULL), m_pRecStatus(NULL), m_pDateInfoList(NULL), m_pChannelRecordFile(NULL), \
		m_pChannelReadFile(NULL)
{
	memset (&m_headInfo, 0, sizeof(RECLOG_FILE_HEAD_INFO));
	m_pFilePathForBackup = NULL;
	m_pFileForBackup = NULL;	

}

CReclogEx::~CReclogEx ()
{

}

/************************************************************************
������������ʼ��һ�����̷��������ݿ�������
���������	diskIndex--���̷�����index
			pFilePath--�������·����
����ֵ��	�ɹ���ʧ��
��ע��
		1���������̣����·�����ʹ�����ŵĺϷ���--���Դ����ݿ��ļ�--��ȡ
		�ļ�ͷ��Ϣ�����������ȷ�ԣ�--�����ļ�����Ϣ�������ļ���Ϣ����δʹ��
		�ļ��б���ʹ���ļ��б���ͨ��Ϊ��λ���У����ļ���ͨ���Ķ�Ӧ��ϵ��
		--��ʼ��¼���״̬��
		2�����ǵ������Ե����⣬��ʼ��ʱӦ�ô����豸��ʶ�����ڴ�������ԡ�
************************************************************************/
bool CReclogEx::Initial (unsigned char diskIndex, const char *pFilePath)
{
	//����·���ʹ�����Ŀ
	{
		assert (NULL == m_pFilePath);	//�����ظ���ʼ��

		m_pFilePath = new char [512];
		assert (NULL != m_pFilePath);
		if (NULL == m_pFilePath)
		{
			return false;
		}

		memset(m_pFilePath, 0, 512);
		if (NULL != pFilePath)
		{
			strcpy(m_pFilePath, pFilePath);
		}

		assert (diskIndex < 32);
		if (diskIndex >= 32)	//Ŀǰ�������8��Ӳ�̣����Զ�λ32��������

		{
			return false;
		}

		m_diskIndex = diskIndex;		
	}

	//�����ļ�����·���������Դ��ļ���
	{
		char filePath [512] = {0};
#ifdef __ENVIRONMENT_WIN32__
#ifdef __ENVIRONMENT_VIDEO_PLAY__
		if (1 < strlen(m_pFilePath))
		{
			sprintf(filePath, "%s\\reclog.bin", m_pFilePath);
		}
		else
		{
			assert (1 == strlen(m_pFilePath));
			sprintf(filePath, "%c:\\reclog.bin", m_pFilePath[0]+m_diskIndex);
		}
#else

		if (1 < strlen(m_pFilePath))
		{
			sprintf(filePath, "%s\\%02d\\reclog.bin", m_pFilePath, m_diskIndex);
		}
		else
		{
			assert (1 == strlen(m_pFilePath));
			sprintf(filePath, "%c:\\reclog.bin", m_pFilePath[0]+m_diskIndex);
		}
#endif
#else
		sprintf(filePath, "%s/%02d/reclog.bin", m_pFilePath, m_diskIndex);
#endif
		assert (NULL == m_pFile);
		if (NULL != m_pFile)
		{
			fclose(m_pFile);
		}

		m_pFile = fopen(filePath, "rb+");
		if (NULL == m_pFile)
		{
#ifdef __ENVIRONMENT_VIDEO_PLAY__
			//[2010-04-01 L]�������ݵ��ļ�������"rb+"��
			m_pFile = fopen(filePath, "rb");

			if(NULL == m_pFile)
			{
				Release(NULL, 1, &m_pFilePath);
				return false;
			}
#else
			Release(NULL, 1, &m_pFilePath);
			return false;
#endif
		}
	}

	//��ȡͷ��Ϣ
	{
		if (1 != fread(&m_headInfo, sizeof(RECLOG_FILE_HEAD_INFO), 1, m_pFile))
		{
			Release(&m_pFile, 1, &m_pFilePath);
			return false;
		}

		//˵���ļ������ˣ��������ļ���ͨ����ĿΪ������

		if ((0 >= m_headInfo.fileNum) || (0 >= m_headInfo.chnnNum))
		{
			Release(&m_pFile, 1, &m_pFilePath);
			return false;
		}	
	}

	//�����ļ���Ϣ
	{
		FILE_FILE_INFO *pData = new FILE_FILE_INFO [m_headInfo.fileNum];
		assert (NULL != pData);
		if (NULL == pData)
		{
			Release(&m_pFile, 1, &m_pFilePath);
			return false;
		}

		if (m_headInfo.fileNum != ReadFileInfo(pData, sizeof(FILE_FILE_INFO)*m_headInfo.fileNum, m_headInfo.fileNum, 0))
		{
			Release(&m_pFile, 2, &m_pFilePath, &pData);
			return false;
		}

		//����ͨ�������б����飬ÿ��ͨ���������С�

		m_pFileInfoList = new FILE_INFO_EX_LIST [m_headInfo.chnnNum];
		assert (NULL != m_pFileInfoList);
		if (NULL == m_pFileInfoList)
		{
			Release(&m_pFile, 2, &m_pFilePath, &pData);
			return false;
		}
		
		//¼��״̬
		m_pRecStatus = new REC_STATUS [m_headInfo.chnnNum];
		assert (NULL != m_pRecStatus);
		if (NULL == m_pRecStatus)
		{
			Release(&m_pFile, 2, &m_pFilePath, &pData);
			return false;
		}
		memset(m_pRecStatus, 0, sizeof(REC_STATUS)*m_headInfo.chnnNum);
		for (int i = 0; i < m_headInfo.chnnNum; ++i)
		{
			m_pRecStatus[i].remainReclogNum = m_headInfo.fileLogNum;
		}

		unsigned long currTime = GetCurrTime32();
		FILE_INFO_EX fileInfo;
		//�����ļ���Ϣ��������ʹ������������ֱ���벻ͬ���б���������ͨ���Ķ�Ӧ��ϵ��

		for (unsigned short i=0; i<m_headInfo.fileNum; ++i)
		{
			//2010-07-08 16:11:00 YSW
			//���޸�ʱ��������ص��ļ����ټ�����Ч�б��������ñ�Ǽ�����Ч�б�
			//��������ĵ�����ȥ����
			//2010-05-14 14:37:00 YSW
			//Ҫ�����ݽ���һ��ȫ����ж��������쳣��������Ӧ�÷�����Ч�б�
			//һ��ȷʵû������¼����ļ�(0 == pData[i].type)
			//����ͨ����Ų��������������Ҫ��(MAX_CHNN_NUM < CHANNEL_ID(pData[i].chnn))
			//�����ļ��Ŀ�ʼʱ���ȻС��ϵͳָ������Сʱ��(pData[i].startTime < m_minTime)
			//�����ļ��Ŀ�ʼʱ���Ȼ����ϵͳ��ǰʱ��(currTime <= pData[i].startTime)
			//�޸�ʱ�����ܴ���������������԰��������ļ�������Ч���У�ֻҪ�������Ƕ����ᱻɾ����
			//�����ļ��Ľ���ʱ���Ȼ���ڵ�ǰϵͳ��������ʱ��(m_maxTime < pData[i].endTime)��
			//�����ļ��Ŀ�ʼʱ����ڻ��ߵ��ڽ���ʱ��(pData[i].endTime <= pData[i].startTime)
			//�����޸�ʱ����ܴ�������������������Թ�����Ч���С�
			if ((0 == RECORD_TYPE_IS_NORMAL(pData[i].type)) ||\
				(MAX_CHNN_NUM < CHANNEL_ID(pData[i].chnn)) ||\
				(pData[i].startTime < m_minTime) ||\
				(pData[i].status && (m_maxTime < pData[i].endTime)) ||\
				((pData[i].status > 0) && (pData[i].status != FILE_STATUS_WRITING) && (pData[i].endTime <= pData[i].startTime)) ||\
				((pData[i].status == FILE_STATUS_WRITING) && (pData[i].endTime != 0) && (pData[i].endTime <= pData[i].startTime)))
			{
				m_invalidFileList.AddTail(i);
			}
			else
			{
				fileInfo.type		= pData[i].type;
				fileInfo.chnn		= pData[i].chnn;
				fileInfo.startTime	= pData[i].startTime;
				fileInfo.endTime	= pData[i].endTime;
				fileInfo.diskIndex	= m_diskIndex;
				fileInfo.fileIndex	= i;

				//2010-07-08 16:11:00 YSW
				//ĳ���ļ���ʱ����������˵�ǰ�ļ���˵����Ҫ�������ʽ��ǣ�
				//�������Ѿ��з���ʽ��ǵ��ļ���Ҳ��Ҫ���ӡ�
				{
					if ((FILE_STATUS_UNOFFICIAL & pData[i].status) ||\
						((currTime <= pData[i].startTime) || (currTime < pData[i].endTime)))
					{
						RECORD_FILE_EN_UNOFFICAL(fileInfo.type);
					}

					//�޸��ļ����
					if ((currTime <= pData[i].startTime) || (currTime < pData[i].endTime))
					{
						pData[i].status |= FILE_STATUS_UNOFFICIAL;
						WriteFileInfo(i, &pData[i]);
					}
				}

				AddToFileListForInitial(fileInfo, m_pFileInfoList[CHANNEL_ID(pData[i].chnn)], true);

				if (FILE_STATUS_WRITING == pData[i].status)
				{
					m_unFinishedFileList.AddTail(fileInfo);
				}
			}

			//
		}

		delete [] pData;
		pData = NULL;
	}	

	assert(NULL == m_pChannelRecordFile);
	m_pChannelRecordFile = new CHANNEL_USE_FILE_INFO [m_headInfo.chnnNum];	
	for (int i = 0; i < m_headInfo.chnnNum; ++i)
	{
		m_pChannelRecordFile[i].channel = i;
		m_pChannelRecordFile[i].pFileList = new CMyList<unsigned short>;
	}

	assert(NULL == m_pChannelReadFile);
	m_pChannelReadFile = new CHANNEL_USE_FILE_INFO [m_headInfo.chnnNum];
	for (int i = 0; i < m_headInfo.chnnNum; ++i)
	{
		m_pChannelReadFile[i].channel = i;
		m_pChannelReadFile[i].pFileList = new CMyList <unsigned short>;
	}
	
	if (NULL != m_pCleanDataBuf)
	{
		delete [] m_pCleanDataBuf;
		m_pCleanDataBuf = NULL;
	}

	return true;
}

void CReclogEx::Quit()
{
	if (NULL != m_pRecStatus)
	{
		delete [] m_pRecStatus;
		m_pRecStatus = NULL;
	}

	if (NULL != m_pFileInfoList)
	{
		for (unsigned char i=0; i<m_headInfo.chnnNum; ++i)
		{
			m_pFileInfoList[i].RemoveAll();
		}

		delete [] m_pFileInfoList;
		m_pFileInfoList = NULL;
	}

	m_invalidFileList.RemoveAll();

	{
		if (NULL != m_pDateInfoList)
		{
			for (unsigned char chnn=0; chnn<m_headInfo.chnnNum; ++chnn)
			{
				POS pos = m_pDateInfoList[chnn].GetHeadPosition();
				while (NULL != pos)
				{
					DATE_INFO & node = m_pDateInfoList[chnn].GetNext(pos);
					if (NULL != node.pFileList)
					{
						node.pFileList->RemoveAll();
						delete node.pFileList;
						node.pFileList = NULL;
					}
				}

				m_pDateInfoList[chnn].RemoveAll();
			}

			delete [] m_pDateInfoList;
			m_pDateInfoList = NULL;
		}
	}

	if (NULL != m_pFilePath)
	{
		delete [] m_pFilePath;
		m_pFilePath = NULL;
	}

	Release(&m_pFile, 1, NULL);

	if (NULL != m_pChannelRecordFile)
	{
		for (int i = 0; i < m_headInfo.chnnNum; ++i)
		{
			//�˳�֮ǰҪ�Ѵ򿪵�¼���ļ��رյ�
			assert(0 == m_pChannelRecordFile[i].pFileList->GetCount());
			delete m_pChannelRecordFile[i].pFileList;
			m_pChannelRecordFile[i].pFileList = NULL;
		}
		delete [] m_pChannelRecordFile;
		m_pChannelRecordFile = NULL;
	}

	if (NULL != m_pChannelReadFile)
	{
		for (int i = 0; i < m_headInfo.chnnNum; ++i)
		{
			//�˳�֮ǰҪ�Ѷ�ȡ���ļ��رյ�
			assert(0 == m_pChannelReadFile[i].pFileList->GetCount());
			delete m_pChannelReadFile[i].pFileList;
			m_pChannelReadFile[i].pFileList = NULL;
		}
		delete [] m_pChannelReadFile;
		m_pChannelReadFile = NULL;
	}


	if (NULL != m_pCleanDataBuf)
	{
		delete [] m_pCleanDataBuf;
		m_pCleanDataBuf = NULL;
	}


	if (NULL != m_pFilePathForBackup)
	{
		delete [] m_pFilePathForBackup;
		m_pFilePathForBackup = NULL;
	}
}
/************************************************************************
������������ȡһ��δʹ���ļ��������š�
���������chnn--�ⲿ��Ҫʹ�ø��ļ���ͨ���š�
���������fileIndex--���ڼ�¼�ļ������š�
���سɹ���ʧ�ܡ�
��ע��	1��ֻҪ��δ���ļ����ڣ������δ���ļ��б��е������������ͨ����Ե�
		�����ļ��б��У�ͬʱ�����ļ���ͨ����Ӧ��ϵͼ��
		2�����������Ŀ��Ϊ����д���¼ʱ�л��ļ�����Է���ش���
		3��������ļ�����ȡ��ȥ��ȴ��Ϊĳϵԭ��û������ʹ�ã���ϵͳ�������佫
		���±�����δ���ļ��б���Ϊ���̶�Ӧ����Ϣ��û�б��޸ģ�ֻ���޸����ڴ���
		��������Ϣ����
************************************************************************/
bool CReclogEx::GetInvalidFile (unsigned long chnn, unsigned short &fileIndex)
{
	if (CHANNEL_ID(chnn) < m_headInfo.chnnNum)
	{
		if (m_invalidFileList.GetCount() > 0)
		{
			//����ļ���index
			fileIndex = m_invalidFileList.RemoveHead();

			//��ú�ʱ�����ͨ����Ӧ���ļ�ʹ���б���������Ӧ��ϵ
#if 0
			//ֻ��Ӧ��ϵ�����ټ����ļ��б���Ϊ�б��д���һ��û�ж�Ӧ¼����ļ�������
			//������������Ϊ��д¼��ʼʱ���ж��б�β�����ļ���ţ�������������
			//�ļ������Բ��ø��µİ취���������ȣ�����Ҫ�����µĽڵ㡣
			FILE_INFO_EX fileInfo;
			memset(&fileInfo, 0, sizeof(FILE_INFO_EX));
			fileInfo.fileIndex = fileIndex;
			fileInfo.chnn = chnn;

			m_pFileInfoList[CHANNEL_ID(chnn)].AddTail(fileInfo);
#endif

			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}

/************************************************************************
����������
���������	
���������
��ע��	
************************************************************************/
bool CReclogEx::FindRecycleFile(FILE_INFO_EX &fileInfo)
{
	assert(m_headInfo.chnnNum > 0);

	bool bSucc = false;
	unsigned long maxTime = 0x7fffffff;	
	for (unsigned short chnnTmp = 0; chnnTmp < m_headInfo.chnnNum; ++chnnTmp)
	{
		if (m_pFileInfoList[chnnTmp].IsEmpty())
		{
			continue;
		}

		//m_pFileInfoList[chnnTmp]����ļ���ʱ������ġ�
		//�ҵ���һ��û������,û�����ڶ�ȡ����û������¼���������ļ�
		POS pos = m_pFileInfoList[chnnTmp].GetHeadPosition();
		while (NULL != pos)
		{
			FILE_INFO_EX &node = m_pFileInfoList[chnnTmp].GetAt(pos);
			
			assert(node.chnn == chnnTmp);
			if (RECORD_FILE_IS_LOCK(node.type))
			{
				m_pFileInfoList[chnnTmp].GetNext(pos);
				continue;
			}
			if (IsFileBeUsingForRead(node.chnn, node.fileIndex))
			{
				m_pFileInfoList[chnnTmp].GetNext(pos);
				continue;
			}
			if (IsFileBeUsingForRecord(node.chnn, node.fileIndex))
			{
				m_pFileInfoList[chnnTmp].GetNext(pos);
				continue;
			}

			assert(node.startTime > 0);	
			if (node.startTime < maxTime)
			{
				fileInfo = node;
				maxTime = node.startTime;
				bSucc = true;
			}
			break;
		}		
		
	}

	return bSucc;
}

/************************************************************************
������������ȡ¼����Ϣ������¼�����Ϣ����ͨ��Ϊ��λ���ض���Ϣ�б�
���������	chnnBits--������ͨ��ģ�壨λ����һ��ͨ����
			startTime--�����Ŀ�ʼʱ�䣨���ȵ��룩
			endTime--�����Ľ���ʱ�䣨���ȵ��룩
			chnnNum--����б��Ԫ����Ŀ��������Ϊϵͳ��������Ƶͨ����Ŀ��
���������	pList--ָ��һ���б����飬ÿά���ڷ���һ��ͨ����¼�����Ϣ��
���ضε���Ŀ��

��ע��	��Ϊ����������ڿ��Լ��ݸ��ֲ�ͬͨ����Ŀ�Ĳ�Ʒʹ�õ����ݿ��ļ�������
		���г����ⲿ�������������ݿ��¼��ͨ����Ŀ��һ�µ����������ڸ�������
		����ʱ������һ��chnnNum����������ָ���ⲿҪ��ѯ��ͨ����Ŀ�����ⲿ��ʹ��
		���ʱ����Ҫ����pListָ��������±���ȷ����ͨ���Ķ�Ӧ�Ĺ�ϵ��

		���ܶ�pList���κ�ɾ����������Ϊ������Ѿ������������̵���Ϣ���������б���
		�����µĽڵ�ʱ�������б�Ϊ�ǿյ������������������
		************************************************************************/
unsigned long CReclogEx::GetSectionInfo(ULONGLONG chnnbBits, unsigned long startTime, unsigned long endTime, SECTION_INFO_EX_LIST *pList, unsigned char chnnNum, bool bRepeat, bool bOfficial)
{
	assert (0 != chnnbBits);
	assert (endTime > startTime);
	assert (NULL != m_pFile);
	assert (NULL != m_pFileInfoList);
	if ((NULL == m_pFile) || (NULL == m_pFileInfoList))
	{
		return 0;
	}
	/*
	POS pos = m_pFileInfoList->GetHeadPosition();
	while(pos)
	{
		FILE_INFO_EX &node = m_pFileInfoList->GetAt(pos);
		printf("%s,%d,%d,%ld,%ld\n",__FILE__,__LINE__,node.fileIndex,node.startTime,node.endTime);
		//start time
		struct tm *ptmStartTime=NULL;
		char buf[128] = {0};
		LONGLONG startTime =node.startTime;
		if ( NULL != (ptmStartTime = gmtime((time_t *)&startTime)) )
		{
			strftime(buf,sizeof(buf),"%Y-%m-%d %H:%M:%S",ptmStartTime);
			printf("startTime :    %s\n",buf);
		}
		//end time
		struct tm *ptmEndTime=NULL;
		char bufEnd[128] = {0};
		LONGLONG endTime =node.endTime;
		if ( NULL != (ptmEndTime = gmtime((time_t *)&endTime)) )
		{
			strftime(bufEnd,sizeof(bufEnd),"%Y-%m-%d %H:%M:%S",ptmEndTime);
			printf("endTime :    %s\n",bufEnd);
		}
		m_pFileInfoList->GetNext(pos);
	}
	*/
	assert (NULL != pList);
	assert (chnnNum > 0);
	if ((NULL == pList) || (0 == chnnNum))
	{
		return 0;
	}

	//˵�����ͨ���ı�ų�����chnnNum
	if (chnnbBits & ~ALL_CHNN_MASK(chnnNum))
	{
		chnnbBits = chnnbBits & ALL_CHNN_MASK(chnnNum);	//��������
	}

	//���ȸ���������������Ӧ���ļ����������fileList�б��У��б�������ļ����Ϊ��
	FILE_INFO_EX_LIST fileList;
	if (0 == GetFileList(chnnbBits, startTime, endTime, fileList))		//û�������������ļ����ڣ�ֱ�ӷ���
	{
		return 0;
	}
	else	//�������������ļ�����
	{
		//���������������ǵ����ڴ��ʹ�ã�������һ����󻺳����������ڴ�ʹ�ø߷�ֵ����
		//����һ������Ҫ����δ�����Ϊһ��������������ȫ����ʹ��Ҫ��������Ҫ�����δ���
		unsigned long num = MAX_BUFFER_LEN/(sizeof(SECTION_INFO_EX) * m_headInfo.fileRecSecNum);
		SECTION_INFO_EX *pData = new SECTION_INFO_EX [num*m_headInfo.fileRecSecNum];
		assert (NULL != pData);
		if (NULL == pData)
		{
			return 0;
		}

		/*
		1��һ�ζ�ȡһ���ļ��Ķμ�¼��Ϣ��Ȼ�������Ѿ���õ��ļ��б���Ѱ�Ҷ�Ӧ�Ķμ�¼��
		2��������ѭ���Ǳ��������������ļ��б�ÿ�����һ���ļ�Ѱ�Ҷ�Ӧ�Ķ���Ϣ��
		3�������ļ��б�Ͷμ�¼��Ϣ���ǰ����ļ���ŵ��������У����Կ���ÿ�������ĳ���ļ�
		���м��ض���Ϣʱ����ǰ�ν����Ķμ�¼��ʼ����ɨ�裬����Ҫÿ�ζ��ӻ������Ŀ�ʼɨ�衣
		4������3������ԭ�򣬰ѵڶ���ѭ�����±�i������ѭ���ⲿ�����ڼ�¼�ϴ��±ꡣֻ��������
		��ȡһ���μ�¼ʱ��retNumͬʱ���£�i = 0����
		*/
		unsigned long currTime = GetCurrTime32() - MAX_TIME_OFFSET;

		unsigned short headIndex = 0, tailIndex = 0;
		unsigned long retNum = 0, getNum = 0;
		headIndex = tailIndex = fileList.GetHead().fileIndex;	//��ʱheadIndex == tailIndex��Ϊ�˵�һ�ν����ȡ�μ�¼·����
		POS pos = fileList.GetHeadPosition();
		SECTION_INFO_EX *pSection = NULL;
		unsigned long chnn = 0, i = 0;
		SECTION_INFO_EX	section;

		while (NULL != pos)
		{
			FILE_INFO_EX &node = fileList.GetAt(pos);

			//2010-07-08 09:20:00 YSW
			//���˵�����ʽ�ļ�
			if (bOfficial && RECORD_FILE_IS_UNOFFICAL(node.type))
			{
				fileList.GetNext(pos);
				continue;
			}

			//��ǰ�ļ�������Ѿ������˻����������ݶ�Ӧ���ļ�������ֵ����˵����Ҫ���¶�ȡ��һ�������ˡ�
			if (tailIndex <= node.fileIndex)
			{
				//headIndexʼ��ָ�򻺳�����Ӧ�ĵ�һ���ļ����
				headIndex = node.fileIndex;
				retNum = ReadSection(pData, sizeof(SECTION_INFO_EX) * m_headInfo.fileRecSecNum * num, num, headIndex);
				if (0 == retNum)
				{
					//��ȡʧ�ܣ�˵���쳣
					assert (false);
					delete [] pData;
					return getNum;
				}

				i = 0;	//���¶�ȡ��һ���μ�¼����Ҫ����i��
				tailIndex += (unsigned short)retNum;//tailIndexָ�򻺳�����Ӧ�����һ���ļ����
			}

			for (; i<retNum; ++i)
			{
				//����ļ�������Ӧ������м��ضμ�¼��
				if ((headIndex + i) == node.fileIndex)
				{
					pSection = pData + (i * m_headInfo.fileRecSecNum);

					chnn = node.chnn;	//���ͨ����

					//��ʱӦ�ö�Ӧ����������˵��ǰ�������ļ����������ǽ���ͨ�����ļ���ϵ����
					assert(chnnbBits & ((ULONGLONG)0x01 << chnn));	

					//ʵ���Ͽ��Բ�Ҫ�ж��ˣ�����Ϊ�˷�ֹ���⻹�Ǽ��ϡ�					
					if (chnnbBits & ((ULONGLONG)0x01 << chnn))
					{
						unsigned long oldNum = pList[chnn].GetCount();
						for (unsigned long j=0; j<m_headInfo.fileRecSecNum; ++j)
						{
							//2010-01-24 12:00:00 YSW
							//�޸�ǰ������δд�������¼�ʱ���ѵ�ǰʱ����Ϊ��������в��㡣
							//��ʱ���ڿ���ĳ���¼�����д����ʱ�������ʱ���д��ʱ���в��죨�����¼�����ʱ��Ϊ12:00:54���������д��
							//��Ϊȴ������12:01:00�������ڣ�12:00:54��12:01:00��ʱ����ڽ��뱾�ӿڽ�����������ô�����ʱ���ǳ���
							//12:00:54�ģ����ǲ���ȷ�ģ����Բ����޶�ʱ��İ취������������

							//���0 == pData[i]->endTime����0 < pSection[j].startTime��˵���ö���Ϣû��д������
							//��ô����Ҫ����俪ʼʱ���Ƿ����޶�ʱ��֮ǰ�����������ö���Ҫ�����˵���
							if ((0 == pSection[j].endTime) && (0 < pSection[j].startTime))
							{
								//2010-05-21 13:58:00 YSW
								//������ļ��Ŀ�ʼʱ��ͽ���ʱ����������ʹ���ļ��Ľ���ʱ�������㡣
								if (node.startTime <= node.endTime)
								{
									pSection[j].endTime = node.endTime;	
								}
								else
								{
									//����Ϊ��(pSection[j].startTime + MAX_TIME_OFFSET) < GetCurrTime32()
									//����������ʾ�öβ��ܱ����ˣ�����Ҫ�����ʱ��
									if (pSection[j].startTime < currTime)
									{
										//��Ϊ������ʱ�����쳣�ļ������޸������Ը���������Ǹö���δд�꣬���Բ������ʱ�䡣
										pSection[j].endTime = currTime;//����ʱ��Ӧ�����޶�ʱ��֮ǰ��
									}
									else
									{
										//Ҫ���˵�
										continue;
									}
								}
							}

							if ((currTime + MAX_TIME_OFFSET) < pSection[j].endTime)
							{
								pSection[j].endTime = (currTime + MAX_TIME_OFFSET);
							}

							//2010-01-24 12:00:00 YSW��������һ��ʹ���޶�ʱ���������ˡ�
							////���0 == pData[i]->endTime����0 < pSection[j].startTime��˵���ö���Ϣû��д������
							//if ((0 == pSection[j].endTime) && (0 < pSection[j].startTime))
							//{
							//	//��Ϊ������ʱ�����쳣�ļ������޸������Ը���������Ǹö���δд�꣬���Բ������ʱ�䡣
							//	pSection[j].endTime = GetCurrTime32();
							//}

							//2010-02-27 09:29:00 YSW
							//���ʱ���߼������ԣ����翪ʼʱ��ҪС�ڻ��ߵ��ڽ���ʱ��(ע�⣺��ʱ����ʱ���Ѿ��õ�������
							if (pSection[j].endTime < pSection[j].startTime)
							{
								continue;
							}

							//���������������ų���������
							if (!((endTime <= pSection[j].startTime) || (pSection[j].endTime <= startTime)))
							{
								section.startTime	= pSection[j].startTime;
								section.endTime		= pSection[j].endTime;
                                
								AddToSectionList(section, pList[chnn], bRepeat);
								//++ getNum;
							}

							//˵����ǰ�ļ������еĶμ�¼���Ѿ�������ɣ�����Ķ���Щ��Ч����Ϣ�ˣ��������������ˡ�
							//һ����ȫ�����㣬��ʾ�ü�¼�ռ����δ��ʹ�ù���һ���ǿ�ʼʱ��С���ļ��Ŀ�ʼʱ�䣬��ʾ
							//�ÿռ���ǰ��ʹ�ù��������ļ��������ˣ���û����ǰ�࣬��δ�õ��ÿռ䡣
							if (((0 == pSection[j].startTime) && (0 == pSection[j].endTime)) || (node.startTime > pSection[j].startTime))
							{
								break;
							}
						}

						assert(pList[chnn].GetCount() >= oldNum);
						getNum += pList[chnn].GetCount() - oldNum;
					}

					//����ѭ����ƥ����һ���ļ�
					break;
				}
			}

			fileList.GetNext(pos);
		}

		delete [] pData;
		return getNum;
	}
}

/************************************************************************
������������ȡ�ļ��б�����ͨ��+ʱ������ķ�ʽ���뷵���б�
���������	chnnBits--����ͨ��ģ��
			startTime--�����Ŀ�ʼʱ��
			endTime--�����Ľ���ʱ��
���������	list--��¼�ļ���Ϣ���б�
���������������ļ���Ŀ��
��ע��
		���ܶ�list���κ�ɾ����������Ϊ������Ѿ������������̵���Ϣ���������б���
		�����µĽڵ�ʱ�������б�Ϊ�ǿյ������������������
************************************************************************/
unsigned long CReclogEx::GetFileInfo(unsigned long chnn, unsigned long startTime, unsigned long endTime, \
									 FILE_INFO_EX_LIST & list, bool bOnlyRead, bool bRepeat, bool bOfficial)
{
	chnn = CHANNEL_ID(chnn);
	assert (chnn < m_headInfo.chnnNum);
	assert (endTime > startTime);
	assert (NULL != m_pFile);
	assert (NULL != m_pFileInfoList);
	assert (NULL != m_pChannelRecordFile);
	assert (chnn == m_pChannelRecordFile[chnn].channel);
	assert (NULL != m_pChannelRecordFile[chnn].pFileList);
	if ((NULL == m_pFile) || (NULL == m_pFileInfoList))
	{
		return 0;
	} 

	bool bOpenRecordFile = false;
	unsigned short curRecordFileIndex = -1;
	if (m_pChannelRecordFile[chnn].pFileList->GetCount() > 0)
	{
		bOpenRecordFile = true;
		assert(1 == m_pChannelRecordFile[chnn].pFileList->GetCount());
		assert(!bOnlyRead); //��Ȼ�����������¼�񣬾Ͳ�������ֻ������
		curRecordFileIndex = m_pChannelRecordFile[chnn].pFileList->GetHead();
	}
   
	unsigned long oldNum = list.GetCount();
	unsigned long num = 0;

	FILE_INFO_EX node;
	POS pos = GetNodePos(m_pFileInfoList[chnn], startTime);//���ݿ�ʼʱ������ǰ����ļ���Ӧ�Ľڵ㡣
	while (NULL != pos)
	{
		node = m_pFileInfoList[chnn].GetNext(pos);

#if 0
		//���ٲ���һ�õ��ļ��ͼ����ļ��б���������������������������㡣20090916-YSW
		//�����е����һ���ڵ㣬�п����ǣ�¼��״̬ΪNULL��
		//�磺һ������Ԥ�ȴ�һЩ�ļ�������һֱû��¼������Щ�ļ���¼��״̬����NULL
		if ((NULL == pos) && (RECORD_TYPE_NULL == node.type))
		{
			assert(node.startTime == 0);
			assert(node.endTime   == 0);
			continue;
		}
#endif

		//���˵�������ʽ�ļ���
		if (bOfficial && RECORD_FILE_IS_UNOFFICAL(node.type))
		{
			continue;
		}

		//�б����һ��������¼��״̬��Ϊ�գ���˵�����ļ�Ϊ����¼����ļ�����Ҫ��������ʱ�䡣
		//if ((NULL == pos) && (RECORD_TYPE_NULL != m_pRecStatus[chnn].type))
		//���ڿ��Լ��������޸�ʱ�����ɾ�����ݣ��������б��β�Ĳ�һ��������¼����ļ��������ж�������Ҫ�޸ġ�
		if (node.fileIndex == curRecordFileIndex)
		{
			//2010-01-24 15:05:00 YSW
			//���˵��޶�ʱ���ڲ����ļ�
			unsigned long currTime = GetCurrTime32() - MAX_TIME_OFFSET;

			if (currTime <= node.startTime)
			{
				continue;
			}

			if (RECORD_TYPE_NULL != m_pRecStatus[chnn].type)
			{
				node.endTime = currTime;//���޶�ʱ���
			}
			else
			{
				if(node.endTime < node.startTime)
				{
					assert(false);
					node.endTime = currTime;//���޶�ʱ���
				}
			}
			
			assert(bOpenRecordFile);
			assert(node.fileIndex == curRecordFileIndex);			
			assert(node.endTime > node.startTime);

		}
       
		if (!((node.endTime <= startTime) || (endTime <= node.startTime)))
		{
			if (bOpenRecordFile && (curRecordFileIndex == node.fileIndex))
			{
				RECORD_FILE_EN_WRITING(node.type);
			}
			if (IsFileBeUsingForRead(node.chnn, node.fileIndex))
			{
				RECORD_FILE_EN_READING(node.type);
			}

			AddToFileListForSearch(node, list, bOnlyRead, bRepeat);
			//++ num;
		}
		else if (endTime <= node.startTime)
		{
			//���������ļ�����ͨ�����еģ������б����ѿ�ʼʱ���������Ե���ǰ�ڵ�Ŀ�ʼʱ����ڻ��ߵ�������
			//�����Ľ���ʱ�䣬˵�������ڵ�Ŀ�ʼʱ�䶼Ҫ���ڸý���ʱ�䣨����������Ҫ�󣩡����Կ����˳�������

			break;
		}
	}
    
	assert(list.GetCount() >= oldNum);
	num = list.GetCount() - oldNum;
	return num;
}

/************************************************************************
������������ȡ¼���ļ��б���ͨ��Ϊ��λ�����б�
���������	chnnBits--������ͨ��ģ�壨λ����һ��ͨ����
			startTime--�����Ŀ�ʼʱ�䣨���ȵ��룩
			endTime--�����Ľ���ʱ�䣨���ȵ��룩
			chnnNum--����б��Ԫ����Ŀ��������Ϊϵͳ��������Ƶͨ����Ŀ��
���������	pList--ָ��һ���б����飬ÿά���ڷ���һ��ͨ����¼���ļ���
�����ļ�����Ŀ��

��ע��	��Ϊ����������ڿ��Լ��ݸ��ֲ�ͬͨ����Ŀ�Ĳ�Ʒʹ�õ����ݿ��ļ�������
		���г����ⲿ�������������ݿ��¼��ͨ����Ŀ��һ�µ����������ڸ�������
		����ʱ������һ��chnnNum����������ָ���ⲿҪ��ѯ��ͨ����Ŀ�����ⲿ��ʹ��
		���ʱ����Ҫ����pListָ��������±���ȷ����ͨ���Ķ�Ӧ�Ĺ�ϵ��

		���ܶ�pList���κ�ɾ����������Ϊ������Ѿ������������̵���Ϣ���������б���
		�����µĽڵ�ʱ�������б�Ϊ�ǿյ������������������
************************************************************************/
unsigned long CReclogEx::GetFileInfo(ULONGLONG chnnbBits, unsigned long startTime, unsigned long endTime, \
						  FILE_INFO_EX_LIST *pList, unsigned char chnnNum, bool bOnlyRead, bool bRepeat, bool bOfficial)
{
	assert (0 != chnnbBits);
	assert (endTime > startTime);
	assert (NULL != m_pFile);
	assert (NULL != m_pFileInfoList);
	assert (NULL != m_pChannelRecordFile);	

	if ((NULL == m_pFile) || (NULL == m_pFileInfoList))
	{
		return 0;
	}

	assert (NULL != pList);
	assert (chnnNum > 0);
	if ((NULL == pList) || (0 == chnnNum))
	{
		return 0;
	}

	//˵�����ͨ���ı�ų�����chnnNum
	if (chnnbBits & ~ALL_CHNN_MASK(chnnNum))
	{
		chnnbBits = chnnbBits & ALL_CHNN_MASK(chnnNum);	//��������
	}

	//ȡchnnNum��m_headInfo.chnnNum����Сֵ���ڱ���ͨ��
	chnnNum = (m_headInfo.chnnNum > chnnNum) ? chnnNum : m_headInfo.chnnNum;


	unsigned long num = 0;
	for (unsigned char chnn=0; chnn<chnnNum; ++chnn)
	{
		if (chnnbBits & ((ULONGLONG)0x01 << chnn))
		{
			assert (chnn == m_pChannelRecordFile[chnn].channel);
			assert (NULL != m_pChannelRecordFile[chnn].pFileList);
			bool bOpenRecordFile = false;
			unsigned short curRecordFileIndex = -1;
			if (m_pChannelRecordFile[chnn].pFileList->GetCount() > 0)
			{
				bOpenRecordFile = true;
				assert(1 == m_pChannelRecordFile[chnn].pFileList->GetCount());
				assert(!bOnlyRead); //��Ȼ�����������¼�񣬾Ͳ�������ֻ������
				curRecordFileIndex = m_pChannelRecordFile[chnn].pFileList->GetHead();
			}

			unsigned long oldNum = pList[chnn].GetCount();
			FILE_INFO_EX node;
			POS pos = GetNodePos(m_pFileInfoList[chnn], startTime);//���ݿ�ʼʱ������ǰ����ļ���Ӧ�Ľڵ㡣
			while (NULL != pos)
			{
				node = m_pFileInfoList[chnn].GetNext(pos);

#if 0
				//���ٲ���һ�õ��ļ��ͼ����ļ��б���������������������������㡣20090916-YSW
				//�����е����һ���ڵ㣬�п����ǣ�¼��״̬ΪNULL��
				//�磺һ������Ԥ�ȴ�һЩ�ļ�������һֱû��¼������Щ�ļ���¼��״̬����NULL
				if ((NULL == pos) && (RECORD_TYPE_NULL == node.type))
				{
					assert(node.startTime == 0);
					assert(node.endTime   == 0);
					continue;
				}
#endif
				//���˵�������ʽ�ļ���
				if (bOfficial && RECORD_FILE_IS_UNOFFICAL(node.type))
				{
					continue;
				}

				//�б����һ��������¼��״̬��Ϊ�գ���˵�����ļ�Ϊ����¼����ļ�����Ҫ��������ʱ�䡣
				//if ((NULL == pos) && (RECORD_TYPE_NULL != m_pRecStatus[chnn].type))
				//���ڿ��Լ��������޸�ʱ�����ɾ�����ݣ��������б��β�Ĳ�һ��������¼����ļ��������ж�������Ҫ�޸ġ�
				if (node.fileIndex == curRecordFileIndex)
				{
					//2010-01-24 15:05:00 YSW
					//���˵��޶�ʱ���ڲ����ļ�
					unsigned long currTime = GetCurrTime32() - MAX_TIME_OFFSET;

					if (currTime <= node.startTime)
					{
						continue;
					}

					if (RECORD_TYPE_NULL != m_pRecStatus[chnn].type)
					{
						node.endTime = currTime;//���޶�ʱ���
					}
					else
					{
						if(node.endTime < node.startTime)
						{
							assert(false);
							node.endTime = currTime;//���޶�ʱ���
						}
					}
					assert(bOpenRecordFile);
					assert(node.fileIndex == curRecordFileIndex);	
					assert(node.endTime > node.startTime);
				}

				

				if (!((node.endTime <= startTime) || (endTime <= node.startTime)))
				{
					if (bOpenRecordFile && (curRecordFileIndex == node.fileIndex))
					{
						RECORD_FILE_EN_WRITING(node.type);
					}
					if (IsFileBeUsingForRead(node.chnn, node.fileIndex))
					{
						RECORD_FILE_EN_READING(node.type);
					}

					AddToFileListForSearch(node, pList[chnn], bOnlyRead, bRepeat);
					//++ num;
				}
				else if (endTime <= node.startTime)
				{
					//���������ļ�����ͨ�����еģ������б����ѿ�ʼʱ���������Ե���ǰ�ڵ�Ŀ�ʼʱ����ڻ��ߵ�������
					//�����Ľ���ʱ�䣬˵�������ڵ�Ŀ�ʼʱ�䶼Ҫ���ڸý���ʱ�䣨����������Ҫ�󣩡����Կ����˳�������

					break;
				}
			}
			assert(pList[chnn].GetCount() >= oldNum);
			num += pList[chnn].GetCount() - oldNum;
		}
	}

	return num;
}

/************************************************************************
������������ȡ¼���ļ��б���time��Ӧ�ڵ��ǰ���MAX_READ_FILE_NUM���ļ�
			�����б�
���������	chnn--ͨ�����
			time--ʱ������
���������	list--�����б�
�����ļ���Ŀ��
��ע��	
		���ܶ�list���κ�ɾ����������Ϊ������Ѿ������������̵���Ϣ���������б���
		�����µĽڵ�ʱ�������б�Ϊ�ǿյ������������������
************************************************************************/
unsigned long CReclogEx::GetFileInfo(unsigned long chnn, unsigned long time, \
									 FILE_INFO_EX_LIST &list, bool bOnlyRead, bool bRepeat, bool bOfficial)
{
	chnn = CHANNEL_ID(chnn);
	assert (chnn < m_headInfo.chnnNum);
	assert (time > 0);
	assert (NULL != m_pChannelRecordFile);
	assert (chnn == m_pChannelRecordFile[chnn].channel);
	assert (NULL != m_pChannelRecordFile[chnn].pFileList);

	if ((m_headInfo.chnnNum <= chnn) || (0 == time))
	{
		return 0;
	}

	unsigned long num = 0;
	FILE_INFO_EX node;
	POS pos = GetNodePos(m_pFileInfoList[chnn], time);

	if ((NULL == pos) && (!m_pFileInfoList[chnn].IsEmpty()))
	{
		pos = m_pFileInfoList[chnn].GetTailPosition();
	}

	while ((NULL != pos) && (num < MAX_READ_FILE_NUM))
	{
		m_pFileInfoList[chnn].GetPrev(pos);
		++ num;
	}

	if (NULL == pos)
	{
		pos = m_pFileInfoList[chnn].GetHeadPosition();
	}

	bool bOpenRecordFile = false;
	unsigned short curRecordFileIndex = -1;
	if (m_pChannelRecordFile[chnn].pFileList->GetCount() > 0)
	{
		bOpenRecordFile = true;
		assert(1 == m_pChannelRecordFile[chnn].pFileList->GetCount());
		assert(!bOnlyRead); //��Ȼ�����������¼�񣬾Ͳ�������ֻ������
		curRecordFileIndex = m_pChannelRecordFile[chnn].pFileList->GetHead();
	}

	num = 0;
    unsigned long oldNum = list.GetCount();
	while ((NULL != pos) && (num < (MAX_READ_FILE_NUM << 1)))
	{
		
		node = m_pFileInfoList[chnn].GetNext(pos);
#if 0
		//�����е����һ���ڵ㣬�п����ǣ�¼��״̬ΪNULL��
		//�磺һ������Ԥ�ȴ�һЩ�ļ�������һֱû��¼������Щ�ļ���¼��״̬����NULL
		if ((NULL == pos) && (RECORD_TYPE_NULL == node.type))
		{
			assert(node.startTime == 0);
			assert(node.endTime   == 0);
			continue;
		}

#endif
		//���˵�������ʽ�ļ���
		if (bOfficial && RECORD_FILE_IS_UNOFFICAL(node.type))
		{
			continue;
		}

		//����¼��Ľڵ㣬��Ҫ���½���ʱ�䡣	
		//if ((NULL == pos) && (RECORD_TYPE_NULL != m_pRecStatus[chnn].type))
		//���ڿ��Լ��������޸�ʱ�����ɾ�����ݣ��������б��β�Ĳ�һ��������¼����ļ��������ж�������Ҫ�޸ġ�
		if (node.fileIndex == curRecordFileIndex)
		{
			//2010-01-24 15:05:00 YSW
			//���˵��޶�ʱ���ڲ����ļ�
			unsigned long currTime = GetCurrTime32() - MAX_TIME_OFFSET;

			if (currTime <= node.startTime)
			{
				continue;
			}

			if (RECORD_TYPE_NULL != m_pRecStatus[chnn].type)
			{
				node.endTime = currTime;//���޶�ʱ���
			}
			else
			{
				if(node.endTime < node.startTime)
				{
					assert(false);
					node.endTime = currTime;//���޶�ʱ���
				}
			}
			assert(bOpenRecordFile);
			assert(node.fileIndex == curRecordFileIndex);	
			assert(node.endTime > node.startTime);
		}

		if (bOpenRecordFile && (curRecordFileIndex == node.fileIndex))
		{
			RECORD_FILE_EN_WRITING(node.type);
		}

		if (IsFileBeUsingForRead(node.chnn, node.fileIndex))
		{
			RECORD_FILE_EN_READING(node.type);
		}

		AddToFileListForSearch(node, list, bOnlyRead, bRepeat);
		//++ num;
        
	}

	assert(list.GetCount() >= oldNum);
	num = list.GetCount() - oldNum;

	return num;
}

/************************************************************************
������������ȡ¼���¼�б�����ͨ��+ʱ������ķ�ʽ���뷵���б�
���������	chnn--����ͨ��
			type--¼������ģ��
			startTime--�����Ŀ�ʼʱ��
			endTime--�����Ľ���ʱ��
���������	list--��¼�ļ���Ϣ���б�
���������������ļ���Ŀ��
��ע��
		���ܶ�list���κ�ɾ����������Ϊ������Ѿ������������̵���Ϣ���������б���
		�����µĽڵ�ʱ�������б�Ϊ�ǿյ������������������
************************************************************************/
unsigned long  CReclogEx::GetReclog(unsigned long chnn, unsigned long type, unsigned long startTime, unsigned long endTime, \
									RECLOG_INFO_EX_LIST &list, bool bOnlyRead, bool bRepeat, bool bOfficial)
{
	chnn = CHANNEL_ID(chnn);
	assert (chnn < m_headInfo.chnnNum);
	assert (0 != type);
	assert (endTime > startTime);
	assert (NULL != m_pFile);
	assert (NULL != m_pFileInfoList);
	if ((NULL == m_pFile) || (NULL == m_pFileInfoList))
	{
		return 0;
	}

	//�Ȼ������ʱ���������ļ��б�
	FILE_INFO_EX_LIST fileList;
	if (0 == GetFileList((ULONGLONG)0x01<<chnn, startTime, endTime, fileList))
	{
		return 0;
	}
	else
	{
		unsigned long num = MAX_BUFFER_LEN/(sizeof(FILE_RECLOG_INFO) * m_headInfo.fileLogNum);
		FILE_RECLOG_INFO *pData = new FILE_RECLOG_INFO [num*m_headInfo.fileLogNum];
		assert (NULL != pData);
		if (NULL == pData)
		{
			return 0;
		}

		unsigned short headIndex = 0, tailIndex = 0;
		unsigned long retNum = 0, getNum = 0, i = 0, chnn = 0;
		headIndex = tailIndex = fileList.GetHead().fileIndex;
		FILE_RECLOG_INFO *pSection = pData;

		POS pos = fileList.GetHeadPosition();
		while (NULL != pos)
		{
			FILE_INFO_EX &node = fileList.GetNext(pos);

			//���˵�����ʽ�ļ�
			if (bOfficial && RECORD_FILE_IS_UNOFFICAL(node.type))
			{
				continue;
			}

			//����������˻��������������ݣ�����Ҫ���¶�ȡһ������
			if (tailIndex <= node.fileIndex)
			{
				//headIndexʼ��ָ�򻺳�����Ӧ�ĵ�һ���ļ����
				headIndex = tailIndex;

				retNum = ReadReclog(pData, sizeof(FILE_RECLOG_INFO) * m_headInfo.fileLogNum * num, num, headIndex);
				if (0 == retNum)
				{
					//��ȡʧ�ܣ�˵���쳣
					assert (false);
					delete [] pData;
					return getNum;
				}

				i = 0;	//���¶�ȡ��һ���μ�¼����Ҫ����i��
				tailIndex += (unsigned short)retNum;//tailIndexָ�򻺳�����Ӧ�����һ���ļ����
			}

			for (; i<retNum; ++i)	
			{
				if ((headIndex + i) == node.fileIndex)
				{
					pSection = pData + (i * m_headInfo.fileLogNum);

					//����ͨ������
					if (chnn == node.chnn)
					{
						getNum += AddToReclogList(pSection, type, startTime, endTime, list, node, bOnlyRead, bRepeat);
					}

					break;
				}
			}
		}

		delete [] pData;
		return getNum;
	}

	return 0;
}

/************************************************************************
������������ȡ¼���¼�б�����ͨ��Ϊ�б�λ������ʱ������ķ�ʽ���뷵�ء�
���������	chnnbBits--����ͨ��ģ��
			type--¼������ģ��
			startTime--�����Ŀ�ʼʱ��
			endTime--�����Ľ���ʱ��
			chnnNum--������ͨ����Ŀ������Ƶͨ����ͬ��
���������	pList--��¼�ļ���Ϣ���б�
���������������ļ���Ŀ��
��ע��
			���ܶ�pList���κ�ɾ����������Ϊ������Ѿ������������̵���Ϣ���������б���
			�����µĽڵ�ʱ�������б�Ϊ�ǿյ������������������
************************************************************************/
unsigned long CReclogEx::GetReclog(ULONGLONG chnnbBits, unsigned long type, unsigned long startTime, unsigned long endTime, 
						RECLOG_INFO_EX_LIST *pList, unsigned char chnnNum, bool bOnlyRead, bool bRepeat, bool bOfficial)
{
	assert (0 != chnnbBits);
	assert (0 != type);
	assert (endTime > startTime);
	assert (NULL != m_pFile);
	assert (NULL != m_pFileInfoList);
	if ((NULL == m_pFile) || (NULL == m_pFileInfoList))
	{
		return 0;
	}

	assert (NULL != pList);
	assert (chnnNum > 0);
	if ((NULL == pList) || (0 == chnnNum))
	{
		return 0;
	}

	//˵�����ͨ���ı�ų�����chnnNum
	if (chnnbBits & ~ALL_CHNN_MASK(chnnNum))
	{
		chnnbBits = chnnbBits & ALL_CHNN_MASK(chnnNum);	//��������
	}

	//ȡchnnNum��m_headInfo.chnnNum����Сֵ���ڱ���ͨ��
	chnnNum = (m_headInfo.chnnNum > chnnNum) ? chnnNum : m_headInfo.chnnNum;

	//�Ȼ������ʱ���������ļ��б��������ļ�����Ϊ��
	FILE_INFO_EX_LIST fileList;
	if (0 == GetFileList(chnnbBits, startTime, endTime, fileList))
	{
		return 0;
	}
	else
	{
		unsigned long num = MAX_BUFFER_LEN/(sizeof(FILE_RECLOG_INFO) * m_headInfo.fileLogNum);
		FILE_RECLOG_INFO *pData = new FILE_RECLOG_INFO [num*m_headInfo.fileLogNum];
		assert (NULL != pData);
		if (NULL == pData)
		{
			return 0;
		}

		unsigned short headIndex = 0, tailIndex = 0;
		unsigned long retNum = 0, getNum = 0, i = 0, chnn = 0;
		headIndex = tailIndex = fileList.GetHead().fileIndex;
		FILE_RECLOG_INFO *pSection = pData;

		POS pos = fileList.GetHeadPosition();
		int fileCount = fileList.GetCount();
		bool bFindOneFile = true;

		FILE_INFO_EX node;
		for (int findCount=0; findCount < fileCount; )
		{
			if (bFindOneFile)
			{
				if (NULL == pos)
				{
					//��ȡʧ�ܣ�˵���쳣
					assert (false);
					delete [] pData;
					return getNum;
				}

				node = fileList.GetNext(pos);
				//���˵�����ʽ�ļ�
				if (bOfficial && RECORD_FILE_IS_UNOFFICAL(node.type))
				{
					findCount++;
					continue;
				}

				bFindOneFile = false;
			}

			//����������˻��������������ݣ�����Ҫ���¶�ȡһ�����ݡ�
			if (tailIndex <= node.fileIndex)
			{
				//headIndexʼ��ָ�򻺳�����Ӧ�ĵ�һ���ļ���š�
				headIndex = tailIndex;
				
				retNum = ReadReclog(pData, sizeof(FILE_RECLOG_INFO) * m_headInfo.fileLogNum * num, num, headIndex);
				if (0 == retNum)
				{
					//��ȡʧ�ܣ�˵���쳣
					assert (false);
					delete [] pData;
					return getNum;
				}

				i = 0;	//���¶�ȡ��һ���μ�¼����Ҫ����i��
				tailIndex += (unsigned short)retNum;//tailIndexָ�򻺳�����Ӧ�����һ���ļ����
			}

			for (; i<retNum; ++i)	
			{
				if ((headIndex + i) == node.fileIndex)
				{
					pSection = pData + (i * m_headInfo.fileLogNum);

					chnn = node.chnn;
					assert (chnnbBits & ((ULONGLONG)0x01 << chnn));

					//����ͨ������
					if (chnnbBits & ((ULONGLONG)0x01 << chnn))
					{
						getNum += AddToReclogList(pSection, type, startTime, endTime, pList[chnn], node, bOnlyRead, bRepeat);
					}

					bFindOneFile = true;
					findCount++;
					break;
				}
			}
		}

		delete [] pData;
		return getNum;
	}
}

/************************************************************************
�������������������б�

��ע��
		�����ڳ�ʼ�����޸��ļ�����ã���������ã���û�������б�
************************************************************************/
bool CReclogEx::CreateDateList()
{
	assert (m_headInfo.chnnNum > 0);
	m_pDateInfoList	= new DATE_INFO_LIST [m_headInfo.chnnNum];
	assert (NULL != m_pDateInfoList);

	for (unsigned char chnn=0; chnn<m_headInfo.chnnNum; ++chnn)
	{
		POS pos = m_pFileInfoList[chnn].GetHeadPosition();
		while (NULL != pos)
		{
			FILE_INFO_EX & node = m_pFileInfoList[chnn].GetNext(pos);

			//2010-07-19 16:16:00
			//�����ļ�����7��17��ʼ¼����ļ��������ʱ��Ϊ7��19��֮��
			if ((GetEndTime32OfDate(node.startTime) + ONE_DAY_SECOND_NUM) < node.endTime)
			{
				
				//����ʵ�ʵĶ���Ϣ�����ݶ���Ϣ�����б�
				unsigned long bufLen = sizeof(SECTION_INFO_EX)*m_headInfo.fileRecSecNum;
				SECTION_INFO_EX *pSection = new SECTION_INFO_EX [m_headInfo.fileRecSecNum];

				if (1 == ReadSection (pSection, bufLen, 1, node.fileIndex))
				{
					for (unsigned long i=0; i<m_headInfo.fileRecSecNum; ++i)
					{
						if ((0 < pSection[i].startTime) && (pSection[i].startTime <= pSection[i].endTime))
						{
							AddToDateList(chnn, node.fileIndex, pSection[i].startTime, pSection[i].endTime);
						}
						else
						{
							//�εļ�¼�������ģ�������ֲ�����������������˳�ѭ����
							break;
						}
					}
				}
				else
				{
					//��ȡʧ�ܣ��������б�
				}

				delete [] pSection;
				pSection = NULL;
			}
			else
			{
				AddToDateList(chnn, node.fileIndex, node.startTime, node.endTime);
			}
		}
	}

	/*
	printf("####on disk:%d\n", m_diskIndex);
	for (unsigned char chnn=0; chnn<m_headInfo.chnnNum; ++chnn)
	{
		printf("###channel:%d\n",chnn);
		POS pos = m_pDateInfoList[chnn].GetHeadPosition();
		while (NULL != pos)
		{
		   DATE_INFO &date = m_pDateInfoList[chnn].GetNext(pos);
		   printf("date:");
		   PrintfBit32Time(date.time);
		   printf("file:\n");
		   POS posfile = date.pFileList->GetHeadPosition();
		   while(NULL != posfile)
		   {
			   unsigned short fileIndex = date.pFileList->GetNext(posfile);
			   printf("     %d\n",fileIndex);
		   }
		}
	}
	*/
	return true;
}

/************************************************************************
������������������б�
���������list--�����б������ڵ���������
������Ŀ��
��ע��
			���ܶ�list���κ�ɾ����������Ϊ������Ѿ������������̵���Ϣ���������б���
			�����µĽڵ�ʱ�������б�Ϊ�ǿյ������������������
************************************************************************/
unsigned long CReclogEx::GetDateInfo(CMyList <DVR_DATE> &list)
{
	assert (NULL != m_pDateInfoList);
	if (NULL == m_pDateInfoList)
	{
		return 0;
	}

	DVR_DATE date;
	tm		time;

	DATE_INFO *pNode = new DATE_INFO [m_headInfo.chnnNum];
	POS	*pPos = new POS [m_headInfo.chnnNum];

	for (unsigned char chnn=0; chnn<m_headInfo.chnnNum; ++chnn)
	{
		pPos[chnn] = m_pDateInfoList[chnn].GetHeadPosition();
	}

	unsigned char chnnNum = 0;
	for (unsigned char chnn=0; chnn<m_headInfo.chnnNum; ++chnn)
	{
		if (NULL != pPos[chnn])
		{
			pNode[chnn] = m_pDateInfoList[chnn].GetHead();
			++ chnnNum;
		}
	}

	if (0 == chnnNum)	//û������������ͨ����������
	{
		delete [] pNode;
		delete [] pPos;
		return 0;
	}

	unsigned char i = 0;	//��ʾ��ǰ����ʱ����С��ͨ�����
	unsigned long dateTime = -1;	//��������ʱ����С��ͨ��
	unsigned long num = 0;
	while (chnnNum > 0)
	{
		//�����ļ������С��ͨ����ţ���i��¼��
		dateTime = -1;
		for (unsigned char chnn=0; chnn<m_headInfo.chnnNum; ++chnn)
		{
			if ((NULL != pPos [chnn]) && (dateTime > pNode[chnn].time))
			{
				dateTime = pNode[chnn].time;
				i = chnn;
			}
		}

		//��Ը�ͨ�������б������������������ڼ��뷵���б�
		while (NULL != pPos[i])
		{
			//�������ʱ��Сʱ�䣬������ѭ��������Ѱ�Һ��ʵ�ͨ��
			for (unsigned char chnn=0; chnn<m_headInfo.chnnNum; ++chnn)
			{
				if ((NULL != pPos [chnn]) && (pNode[i].time > pNode[chnn].time))
				{
					break;
				}
			}
            
			time = DVRTime32ToTm(pNode[i].time);
			date.year	= time.tm_year + 1900;
			date.month	= time.tm_mon + 1;
			date.mday	= time.tm_mday;
			date.time	= pNode[i].time;

			num += AddToDateList(date, list);

			//���˵�����ʱ����ͬ�Ľڵ㣬�����˸ղ��Ѿ���������ڽڵ㡣
			for (unsigned char chnn=0; chnn<m_headInfo.chnnNum; ++chnn)
			{
				if ((NULL != pPos [chnn]) && (pNode[i].time == pNode[chnn].time))
				{
					m_pDateInfoList[chnn].GetNext(pPos [chnn]);
					if (NULL != pPos[chnn])
					{
						pNode[chnn] = m_pDateInfoList[chnn].GetAt(pPos[chnn]);
					}
					else
					{
						-- chnnNum;
					}
				}
			}
		}
	}

	delete [] pNode;
	delete [] pPos;
	return num;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CReclogEx::WriteReclogStart(unsigned short fileIndex, unsigned long chnn, RECORD_TYPE type, unsigned long startTime)
{
	assert (NULL != m_pFile);
	if (NULL == m_pFile)
	{
		return false;
	}

	//����б�Ϊ�գ�����β���ļ���ź���Ҫд���ļ���Ų�ͬ������ζ��Ϊ�µ��ļ�����Ҫ�����µĽڵ㡣
	//if (m_pFileInfoList[CHANNEL_ID(chnn)].IsEmpty() || (fileIndex != m_pFileInfoList[CHANNEL_ID(chnn)].GetTail().fileIndex))
	if (NULL == GetNodePosEx(m_pFileInfoList[CHANNEL_ID(chnn)], fileIndex))
	{
		FILE_INFO_EX fileInfo;
		memset(&fileInfo, 0, sizeof(FILE_INFO_EX));
		fileInfo.fileIndex = fileIndex;
		fileInfo.diskIndex = m_diskIndex;
		fileInfo.chnn = chnn;

		POS pos = GetPrevNodePos(m_pFileInfoList[CHANNEL_ID(chnn)], startTime);
		if (NULL != pos)
		{
			m_pFileInfoList[CHANNEL_ID(chnn)].InsertAfter(pos, fileInfo);
		}
		else
		{
			m_pFileInfoList[CHANNEL_ID(chnn)].AddHead(fileInfo);
		}
	}

	//FILE_INFO_EX &fileInfo = m_pFileInfoList[CHANNEL_ID(chnn)].GetTail();
	FILE_INFO_EX &fileInfo = m_pFileInfoList[CHANNEL_ID(chnn)].GetAt(GetNodePosEx(m_pFileInfoList[CHANNEL_ID(chnn)], fileIndex));

	assert (fileIndex == fileInfo.fileIndex);
	assert (chnn == fileInfo.chnn);
	assert (!(type & m_pRecStatus[CHANNEL_ID(chnn)].type));
	assert (0 < startTime);
	assert (startTime >= fileInfo.startTime);
	assert (startTime >= fileInfo.endTime);

	{
		//��ʼʱ��������ʶ��һ�ζԸ��ļ�д�¼�����Ҫ���뵽�����б���ȥ��
		if (0 == fileInfo.startTime)
		{
			memset(&m_pRecStatus[CHANNEL_ID(chnn)], 0, sizeof(REC_STATUS));
			m_pRecStatus[CHANNEL_ID(chnn)].remainReclogNum = m_headInfo.fileLogNum;
			AddToDateList(fileInfo.chnn, fileInfo.fileIndex, startTime, startTime);
		}
		else//������������б�
		{
			UpdateDateList(fileInfo.chnn, fileInfo.fileIndex, startTime, startTime);
		}

		//�����µ�һ�Σ���Ҫ��Ӷμ�¼
		//��д�¼��Ŀ�ʼʱ��Ҫ�ȶΣ�Ȼ���¼���ʼʱ�䡣
		//��д�¼��Ľ���ʱ��Ҫ�¼�����ʱ�䣬Ȼ��д�Ρ�
		if (RECORD_TYPE_NULL == m_pRecStatus[CHANNEL_ID(chnn)].type)
		{
			if (!WriteSection(fileIndex, chnn, startTime, true))
			{
				return false;
			}
		}

		if (!WriteType(fileIndex, chnn, type, startTime, true))
		{
			return false;
		}

		m_pRecStatus[CHANNEL_ID(chnn)].type |= type;
	}

	{
		if ((0 == fileInfo.startTime) || !(fileInfo.type & type))
		{
			fileInfo.startTime = (0 == fileInfo.startTime) ? startTime : fileInfo.startTime;//����Ѿ����˿�ʼʱ�䣬��Ӧ�ø�д����
			fileInfo.type |= type;

			FILE_FILE_INFO info;
			info.chnn		= fileInfo.chnn;
			info.type		= fileInfo.type;
			info.startTime	= fileInfo.startTime;
			info.endTime	= fileInfo.endTime;
			info.status		= FILE_STATUS_WRITING;

			//��һ�ζԸ��ļ�д���¼���������д��һ���µ����ͣ�����Ҫ�����ļ���Ϣ��
			if (!WriteFileInfo(fileIndex, &info))
			{
				return false;
			}
		}
	}

	return true;
}

bool CReclogEx::WriteReclogEnd(unsigned short fileIndex, unsigned long chnn, RECORD_TYPE type, unsigned long endTime)
{
	assert (NULL != m_pFile);
	if (NULL == m_pFile)
	{
		return false;
	}

	//FILE_INFO_EX &fileInfo = m_pFileInfoList[CHANNEL_ID(chnn)].GetTail();
	FILE_INFO_EX &fileInfo = m_pFileInfoList[CHANNEL_ID(chnn)].GetAt(GetNodePosEx(m_pFileInfoList[CHANNEL_ID(chnn)], fileIndex));

	assert (fileIndex == fileInfo.fileIndex);
	assert (chnn == fileInfo.chnn);
	assert (type & m_pRecStatus[CHANNEL_ID(chnn)].type);
	assert (endTime >= fileInfo.startTime);
	assert (endTime >= fileInfo.endTime);

	{
		if (!WriteType(fileIndex, chnn, type, endTime, false))
		{
			return false;
		}

		m_pRecStatus[CHANNEL_ID(chnn)].type &= ~type;

		//2010-07-19 16:16:00
		//�����ļ�����7��17��ʼ¼����ļ��������ʱ��Ϊ7��19��֮��
		if ((GetEndTime32OfDate(fileInfo.startTime) + ONE_DAY_SECOND_NUM) < endTime)
		{
			//����ʵ�ʵĶ���Ϣ�����ݶ���Ϣ�����б�
			unsigned long bufLen = sizeof(SECTION_INFO_EX)*m_headInfo.fileRecSecNum;
			SECTION_INFO_EX *pSection = new SECTION_INFO_EX [m_headInfo.fileRecSecNum];

			if (1 == ReadSection (pSection, bufLen, 1, fileInfo.fileIndex))
			{
				for (unsigned long i=0; i<m_headInfo.fileRecSecNum; ++i)
				{
					if ((0 < pSection[i].startTime) && (0 == pSection[i].endTime))
					{
						//д�˿�ʼ������δд�����ĶΣ��������¼�ĶΡ�
						UpdateDateList(fileInfo.chnn, fileInfo.fileIndex, pSection[i].startTime, pSection[i].endTime);
					}
					else if ((0 == pSection[i].startTime) && (0 == pSection[i].endTime))
					{
						//��Ϊ��ʹ�ã����˳���
						break;
					}
				}
			}
			else
			{
				//��ȡʧ�ܣ��������б�
			}

			delete [] pSection;
			pSection = NULL;
		}
		else
		{
			UpdateDateList(fileInfo.chnn, fileInfo.fileIndex, fileInfo.startTime, endTime);
		}
	}

	{
		fileInfo.endTime = endTime;

		if (RECORD_TYPE_NULL == m_pRecStatus[CHANNEL_ID(chnn)].type)
		{
			if (!WriteSection(fileIndex, chnn, endTime, false))
			{
				return false;
			}

			FILE_FILE_INFO info;
			info.chnn		= fileInfo.chnn;
			info.type		= fileInfo.type;
			info.startTime	= fileInfo.startTime;
			info.endTime	= fileInfo.endTime;
			info.status		= FILE_STATUS_WRITING;

			//���д�¼���������ʱֹͣ���������͵�¼��������ļ���Ϣ������ʱ�䣩��
			if (!WriteFileInfo(fileIndex, &info))
			{
				return false;
			}
		}
	}

	return true;
}

void CReclogEx::RegisterRecordFile(unsigned long channel, unsigned short fileIndex)
{
	assert(NULL != m_pChannelRecordFile);
	assert(channel >= 0);
	assert(channel < m_headInfo.chnnNum);
	//ͬһʱ�̣�һ��ͨ�����������ֻ��ʹ��һ���ļ���
	assert(0 == m_pChannelRecordFile[channel].pFileList->GetCount());
	
	m_pChannelRecordFile[channel].pFileList->AddTail(fileIndex);

	RUN_LOG(RUN_LOG_ITEM_REC_ADD_REC_LIST, CRunLog::RUN_Num("file:", fileIndex), CRunLog::RUN_Num("chnn:", channel), "Add file to Record list");
}

void CReclogEx::CancelRecordFile(unsigned long channel, unsigned short fileIndex)
{
	assert(NULL != m_pChannelRecordFile);
	assert(channel >= 0);
	assert(channel < m_headInfo.chnnNum);
	assert(1 == m_pChannelRecordFile[channel].pFileList->GetCount());
	assert(fileIndex == m_pChannelRecordFile[channel].pFileList->GetHead());

	//����һ���ļ�����Ϣд������ʵ���Ե�Ϊ�ѱ���ļ�����������λ����Ϊ���㡣
	WriteFileInfoEnd(fileIndex);

	m_pChannelRecordFile[channel].pFileList->RemoveHead();

	RUN_LOG(RUN_LOG_ITEM_REC_PUT_OUT_REC_LIST, CRunLog::RUN_Num("file:", fileIndex), CRunLog::RUN_Num("chnn:", channel), "remove file from Record list");
}

void CReclogEx::RegisterReadFile(unsigned long channel, unsigned short fileIndex)
{
	assert(NULL != m_pChannelReadFile);
	assert(channel >= 0);
	assert(channel < m_headInfo.chnnNum);

	//ͬһʱ�̣�һ��ͨ���������������ʹ�ö���ļ��������������ļ������ظ�
	m_pChannelReadFile[channel].pFileList->AddTail(fileIndex);

	RUN_LOG(RUN_LOG_ITEM_PB_ADD_TO_READ_LIST, CRunLog::RUN_Num("file:", fileIndex), CRunLog::RUN_Num("chnn:", channel), "Add file to Read list");
}

void CReclogEx::CancelReadFile(unsigned long channel, unsigned short fileIndex)
{
	assert(NULL != m_pChannelReadFile);
	assert(channel >= 0);
	assert(channel < m_headInfo.chnnNum);
	assert(m_pChannelReadFile[channel].pFileList->GetCount() > 0);

	POS pos = m_pChannelReadFile[channel].pFileList->GetHeadPosition();
	while (NULL != pos)
	{   
		if (fileIndex == m_pChannelReadFile[channel].pFileList->GetAt(pos))
		{
			m_pChannelReadFile[channel].pFileList->RemoveAt(pos);

			RUN_LOG(RUN_LOG_ITEM_PB_PUT_OUT_READ_LIST, CRunLog::RUN_Num("file:", fileIndex), CRunLog::RUN_Num("chnn:", channel), "Remove file to Read list");
			break;
		}
		m_pChannelReadFile[channel].pFileList->GetNext(pos);
	}
	assert(NULL != pos);
}

/*
����ÿ�γ��ֵ��쳣�ļ�����������ÿ��ͨ����Ӧ���ļ��б��β�������Կ���ֱ��β�����ļ�
���뷵���б����ٷ���ȥ����ǲ�����س������쳣�����������쳣���쳣���������޸�������
*/
void CReclogEx::GetUnfinishedFile(FILE_INFO_EX_LIST &list)
{
#if 0
	for(unsigned char chnn = 0; chnn < m_headInfo.chnnNum; ++chnn)
	{
		if(!m_pFileInfoList[chnn].IsEmpty())
		{
			list.AddTail(m_pFileInfoList[chnn].GetTail());
		}
	}
#else
	POS pos = m_unFinishedFileList.GetHeadPosition();
	while (NULL != pos)
	{
		list.AddTail(m_unFinishedFileList.GetNext(pos));
	}
#endif
}

bool CReclogEx::Repair(const FILE_INFO_EX & fileInfo, unsigned long endTime)
{
	/*if(m_pFileInfoList[CHANNEL_ID(fileInfo.chnn)].GetTail().fileIndex != fileInfo.fileIndex)
	{
		assert(false);
		return false;
	}*/

	bool bNeedRepair = false;
	//�޸�����Ϣ
	{
		SECTION_INFO_EX *pSection = new SECTION_INFO_EX[m_headInfo.fileRecSecNum];
		assert(pSection != NULL);

		if(0 != fseek(m_pFile, m_headInfo.sectionOffset + sizeof(SECTION_INFO_EX) * m_headInfo.fileRecSecNum * fileInfo.fileIndex, SEEK_SET))
		{
			delete [] pSection;
			assert(false);
			return false;
		}

		if(1 != fread(pSection, sizeof(SECTION_INFO_EX)*m_headInfo.fileRecSecNum, 1, m_pFile))
		{
			delete [] pSection;
			assert(false);
			return false;
		}

		for(int secIndex = 0; secIndex < m_headInfo.fileRecSecNum; secIndex++)
		{
			if((0 == pSection[secIndex].startTime) || (pSection[secIndex].startTime < fileInfo.startTime))
			{
				break;
			}
			else if(0 == pSection[secIndex].endTime)
			{
				pSection[secIndex].endTime = endTime;
				bNeedRepair = true;
				break;
			}
		}

		if(bNeedRepair)
		{
			if(0 != fseek(m_pFile, -1 * (long)(sizeof(SECTION_INFO_EX) * m_headInfo.fileRecSecNum), SEEK_CUR))
			{
				assert(false);
				delete [] pSection;
				return false;
			}

			if(1 != fwrite(pSection, sizeof(SECTION_INFO_EX)*m_headInfo.fileRecSecNum, 1, m_pFile))
			{
				delete [] pSection;
				assert(false);
				return false;
			}
		}

		delete [] pSection;
	}

	//�޸��¼���Ϣ
	{
		FILE_RECLOG_INFO *pReclog = new FILE_RECLOG_INFO[m_headInfo.fileLogNum];
		assert(pReclog != NULL);
		if(0 != fseek(m_pFile, m_headInfo.reclogoffset + sizeof(FILE_RECLOG_INFO) * m_headInfo.fileLogNum * fileInfo.fileIndex, SEEK_SET))
		{
			delete [] pReclog;
			assert(false);
			return false;
		}

		if(1 != fread(pReclog, sizeof(FILE_RECLOG_INFO)*m_headInfo.fileLogNum, 1, m_pFile))
		{
			delete [] pReclog;
			assert(false);
			return false;
		}

		unsigned long reclogIndex = 0;

		//���¼���¼�����
		unsigned char recTypeNum = RecTypeNum();
		assert (recTypeNum > 0);
		FILE_RECLOG_INFO  *pTempReclog = new FILE_RECLOG_INFO[recTypeNum];
		memset(pTempReclog, 0, sizeof(FILE_RECLOG_INFO)*recTypeNum);
		
		for (unsigned long i=0; i<m_headInfo.fileLogNum; ++i)
		{
			//��ʾ��ʼ
			if ((RECORD_TYPE_NULL != pReclog[i].type) && (pReclog[i].type < RECORD_TYPE_END) && (0 == (pReclog[i].type & RECLOG_END)))
			{
				pTempReclog [RecTypeIndex(static_cast<RECORD_TYPE>(pReclog[i].type))] = pReclog[i];
			}
			
			//��ʾ��������ʱ�䲻Ϊ�㡣
			if ((RECORD_TYPE_NULL != pReclog[i].type) && ((pReclog[i].type - RECLOG_END) < RECORD_TYPE_END) &&\
				(0 < pReclog[i].time) && (pReclog[i].type & RECLOG_END))
			{
				memset(&pTempReclog [RecTypeIndex(static_cast<RECORD_TYPE>(pReclog[i].type))], 0, sizeof(FILE_RECLOG_INFO));
			}

			//��ʾ��Ԥ���ռ�δʹ��
			if ((RECORD_TYPE_NULL == pReclog[i].type) && (0 == pReclog[i].time))
			{
				break;
			}

			++ reclogIndex;
		}

		for (unsigned char i=0; i<recTypeNum; ++i)
		{
			if ((RECORD_TYPE_NULL != pTempReclog[i].type) && (0 < pTempReclog[i].time))
			{
				//����Խ��
				if (reclogIndex < m_headInfo.fileLogNum)
				{
					pReclog[reclogIndex].type = (pTempReclog[i].type | RECLOG_END);
					pReclog[reclogIndex].time = endTime;

					++ reclogIndex;
				}
				else
				{
					break;
				}
			}
		}

		if(bNeedRepair)
		{
			if(0 != fseek(m_pFile, -1 * (long)(sizeof(FILE_RECLOG_INFO) * m_headInfo.fileLogNum), SEEK_CUR))
			{
				delete [] pTempReclog;
				delete [] pReclog;
				assert(false);
				return false;
			}

			if(1 != fwrite(pReclog, sizeof(FILE_RECLOG_INFO)*m_headInfo.fileLogNum, 1, m_pFile))
			{
				delete [] pTempReclog;
				delete [] pReclog;
				assert(false);
				return false;
			}
		}

		delete [] pTempReclog;
		delete [] pReclog;
	}

	//�޸��ļ���Ϣ
	if(bNeedRepair)
	{
		if(0 != fseek(m_pFile, m_headInfo.fileInfoOffset + sizeof(FILE_FILE_INFO) * fileInfo.fileIndex, SEEK_SET))
		{
			assert(false);
			return false;
		}

		FILE_FILE_INFO info;
		info.chnn		= fileInfo.chnn;
		info.type		= fileInfo.type;
		info.startTime	= fileInfo.startTime;
		info.endTime	= endTime;
		info.status		= FILE_STATUS_END;

		if(1 != fwrite(&info, sizeof(FILE_FILE_INFO), 1, m_pFile))
		{
			assert(false);
			return false;
		}
	}

	
	//�޸��б��е�ʱ�䡣
	//m_pFileInfoList[CHANNEL_ID(fileInfo.chnn)].GetTail().endTime = endTime;
	POS pos = GetNodePosEx(m_pFileInfoList[CHANNEL_ID(fileInfo.chnn)], fileInfo.fileIndex);
	FILE_INFO_EX &node = m_pFileInfoList[CHANNEL_ID(fileInfo.chnn)].GetAt(pos);
	node.endTime = endTime;

	//�޸�reclog�ļ�
	{
		if(0 != fseek(m_pFile, m_headInfo.fileInfoOffset + sizeof(FILE_FILE_INFO) * fileInfo.fileIndex, SEEK_SET))
		{
			assert(false);
			return false;
		}

		FILE_FILE_INFO info;
		info.chnn		= fileInfo.chnn;
		info.type		= fileInfo.type;
		info.startTime	= fileInfo.startTime;
		info.endTime	= endTime;
		info.status		= FILE_STATUS_END;

		if(1 != fwrite(&info, sizeof(FILE_FILE_INFO), 1, m_pFile))
		{
			assert(false);
			return false;
		}
	}

	return true;
}

bool CReclogEx::DeleteFile(const FILE_INFO_EX &  fileInfo)
{
	POS pos = GetNodePosEx(m_pFileInfoList[CHANNEL_ID(fileInfo.chnn)], fileInfo.fileIndex);
	if (NULL != pos)
	{
		FILE_INFO_EX & node = m_pFileInfoList[CHANNEL_ID(fileInfo.chnn)].GetAt(pos);
		if (node.fileIndex != fileInfo.fileIndex)
		{
			assert (false);
			return false;
		}
				
		PutOutDateList(fileInfo);
		DeleteFileInfo(fileInfo.fileIndex);	
		AddToInvalidFileList(m_invalidFileList, fileInfo.fileIndex);
		//ע�⣺node�����á�����������ɾ���ŵ�ǰ�棬��ônodeָ��Ķ�����Ϊ���ˡ�
		m_pFileInfoList[CHANNEL_ID(fileInfo.chnn)].RemoveAt(pos);
	}	
		
	return true;
}

bool CReclogEx::LockFile(const FILE_INFO_EX &fileInfo, bool bLock)
{
	POS pos = GetNodePos(m_pFileInfoList[CHANNEL_ID(fileInfo.chnn)], fileInfo.startTime);
	if (NULL != pos)
	{
		FILE_INFO_EX &node = m_pFileInfoList[CHANNEL_ID(fileInfo.chnn)].GetAt(pos);
		if (node.fileIndex != fileInfo.fileIndex)
		{
			assert (false);
			return false;
		}

		if (bLock)
		{
			if (RECORD_FILE_IS_LOCK(node.type))
			{
				return true;
			}				
			else
			{
				RECORD_FILE_EN_LOCK(node.type);
			}
		}
		else
		{
			if (RECORD_FILE_IS_LOCK(node.type))
			{
				RECORD_FILE_EN_UNLOCK(node.type);
			}
			else
			{
				return true;
			}
		}

		unsigned long offset = m_headInfo.fileInfoOffset + node.fileIndex*sizeof(FILE_FILE_INFO);	
		if (0 != fseek(m_pFile, offset, SEEK_SET))
		{
			return false;
		}

		FILE_FILE_INFO fileFileInfo;

		if (1 != fread(&fileFileInfo, sizeof(FILE_FILE_INFO), 1, m_pFile))
		{
			return false;
		}

		if (0 != fseek(m_pFile, offset, SEEK_SET))
		{
			return false;
		}

		fileFileInfo.chnn = node.chnn;
		fileFileInfo.startTime = node.startTime;
		fileFileInfo.endTime = node.endTime;
		fileFileInfo.type = node.type;
		if (1 != fwrite(&fileFileInfo, sizeof(FILE_FILE_INFO), 1, m_pFile))
		{		
			return false;
		}
	}

	return true;
}

bool CReclogEx::DeleteOverdueFiles(unsigned long *pHoldTimeD, unsigned long num)
{
	unsigned long *pHoldTimeDNew = NULL;
	unsigned long numNew = 0;
	unsigned char netVideoInputNum = CProduct::Instance()->NetVideoInputNum();
	if(CStreamRecordManEx::Instance()->IsSupportMinorStreamRecord() && netVideoInputNum>0)
	{
		numNew = num + netVideoInputNum;
		pHoldTimeDNew = new unsigned long[numNew];
		assert(NULL!=pHoldTimeDNew);
		memset(pHoldTimeDNew,0,sizeof(unsigned long)*numNew);
		memcpy(pHoldTimeDNew,pHoldTimeD,sizeof(unsigned long)*num);
		memcpy(pHoldTimeDNew+num,pHoldTimeD+(num-netVideoInputNum),sizeof(unsigned long)*netVideoInputNum);
		
	}
	else
	{
		pHoldTimeDNew = pHoldTimeD;
		numNew = num;
	}

	assert(numNew == m_headInfo.chnnNum);

	unsigned long ltoday = GetZeroTime32OfDate(GetCurrTime32());
	unsigned long *pEndDayl = new unsigned long [numNew];
	for (int i = 0; i < m_headInfo.chnnNum; ++i)
	{ 
		if (0 == pHoldTimeDNew[i])
		{
			//��ʾ����ͨ�����ļ������й���ɾ��������
			pEndDayl[i] = 0;
		}
		else
		{
			pEndDayl[i] = ltoday - (pHoldTimeDNew[i]+ ONE_DAY_SECOND_NUM);
		}		
	}


	for (unsigned short chnn = 0; chnn < m_headInfo.chnnNum; ++chnn)
	{
		if (0 == pEndDayl[chnn])
		{
			continue;
		}

		if (m_pDateInfoList[chnn].IsEmpty())
		{
			continue;
		}
		
		POS nextDatePos = NULL;
		POS datePos = m_pDateInfoList[chnn].GetHeadPosition();
		while (NULL != datePos)
		{
			DATE_INFO &dateInfo = m_pDateInfoList[chnn].GetAt(datePos);
			nextDatePos = datePos;
			m_pDateInfoList[chnn].GetNext(nextDatePos);

			if (dateInfo.time <= pEndDayl[chnn])
			{
				//ɾ����һ����ļ�
				POS nextFileIndexPos = NULL;
				POS fileIndexPos = dateInfo.pFileList->GetHeadPosition();
				while (NULL != fileIndexPos)
				{
					unsigned short fileInfoIndex = dateInfo.pFileList->GetAt(fileIndexPos);
					nextFileIndexPos = fileIndexPos;
					dateInfo.pFileList->GetNext(nextFileIndexPos);

					//���ļ������ҵ�����ļ�
					POS fileListPos = m_pFileInfoList[chnn].GetHeadPosition();
					while (NULL != fileListPos)
					{
						FILE_INFO_EX &fileInfoTmp = m_pFileInfoList[chnn].GetAt(fileListPos);
						if (fileInfoTmp.fileIndex == fileInfoIndex)
						{
							break;
						}
						m_pFileInfoList[chnn].GetNext(fileListPos);
					}

					//���û���ҵ���˵������ļ��п����ǿ����ļ������ϸ������б�ɾ����
					if (NULL == fileListPos)
					{
						dateInfo.pFileList->RemoveAt(fileIndexPos);
						fileIndexPos = nextFileIndexPos;
						continue;
					}

					//����ҵ�����ļ�
					FILE_INFO_EX fileInfo = m_pFileInfoList[chnn].GetAt(fileListPos);
					
					//�ж�����ļ��Ƿ����ڱ�д������������
					if (IsFileBeUsingForRead(fileInfo.chnn, fileInfo.fileIndex)
					  ||(IsFileBeUsingForRecord(fileInfo.chnn, fileInfo.fileIndex))
					  ||RECORD_FILE_IS_LOCK(fileInfo.type))
					{
						fileIndexPos = nextFileIndexPos;
						continue;
					}
					//�ж�����ļ��Ƿ���ɾ������
					//assert(fileInfo.endTime >= fileInfo.startTime);
					if (GetZeroTime32OfDate(fileInfo.endTime) > pEndDayl[chnn])
					{
						fileIndexPos = nextFileIndexPos;
						continue;
					}

                    //���ڣ��Ϳ���ɾ������ļ���
					dateInfo.pFileList->RemoveAt(fileIndexPos);
					m_pFileInfoList[chnn].RemoveAt(fileListPos);

					bool suc = DeleteFileInfo(fileInfo.fileIndex);
					assert(suc);
					AddToInvalidFileList(m_invalidFileList, fileInfo.fileIndex);
					fileIndexPos = nextFileIndexPos;
				}

				if (dateInfo.pFileList->IsEmpty())
				{
					delete dateInfo.pFileList;					
					m_pDateInfoList[chnn].RemoveAt(datePos);
				}
			}
			datePos = nextDatePos;			
		}		
	}

	delete [] pEndDayl;
	pEndDayl = NULL;

	if(CStreamRecordManEx::Instance()->IsSupportMinorStreamRecord() && netVideoInputNum>0)
	{
		delete []pHoldTimeDNew;
		pHoldTimeDNew = NULL;
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////////////////////

void CReclogEx::AddToFileListForInitial(FILE_INFO_EX & fileInfo, FILE_INFO_EX_LIST & list, bool bTime)
{
	POS pos = NULL;

	if (bTime)	//���տ�ʼʱ������
	{
		pos = GetPrevNodePos(list, fileInfo.startTime);
		if (NULL != pos)
		{
			list.InsertAfter(pos, fileInfo);			
		}
		else
		{
			//��Ҫ����ͷ��		
			list.AddHead(fileInfo);
		}
	}
	else	//�����ļ��������
	{
		pos = GetPrevNodePosEx(list, fileInfo.fileIndex, 0);		
		if (NULL != pos)
		{
			list.InsertAfter(pos, fileInfo);
		}		
		else
		{
			//��Ҫ����ͷ��
			list.AddHead(fileInfo);
		}
	}
}

void CReclogEx::AddToFileListForSearch(FILE_INFO_EX fileInfo, FILE_INFO_EX_LIST & list, bool bOnlyRead, bool bRepeat)
{
	/*unsigned long currTime = GetCurrTime32();
	if (currTime < fileInfo.endTime)
	{
		fileInfo.endTime = currTime;
	}*/

	POS pos = NULL;
	
	pos = GetPrevNodePos(list, fileInfo.startTime);
	if (NULL != pos)
	{
		if (bRepeat)
		{
			if (bOnlyRead)
			{
				RECORD_FILE_EN_ONLYREAD(fileInfo.type);
			}
			list.InsertAfter(pos, fileInfo);
		}
		else
		{
			FILE_INFO_EX &fileInfoTmp = list.GetAt(pos);
			if (fileInfoTmp.startTime == fileInfo.startTime)
			{
				//fileInfo.endTime��fileInfoTmp.endTimeӦ������ȫ��ȵġ�
				//�����п������޸�����ļ�ʱ������ƫ������Ͻ����ƫ��Ҳ���Ǽ���
				//assert(fileInfo.endTime == fileInfoTmp.endTime);					
				assert(fileInfo.chnn == fileInfoTmp.chnn);
				//��������ظ����ļ���ֻҪ��һ������ֻ���ģ���Ҫ�Բ�ֻ�����Լ��롣
				//��Ϊ���Ƕ�ֻ�����ļ�������ֻ��������ֻ�����ļ��϶�����ֻ��Ӳ�̣���ôѭ���Ͳ����ܸ�������ļ���
				//����Ҫ�����Է�ֻ��Ӳ�̵��ļ�Ϊ׼��ֻ���������ǲſ�����������ļ���
				if ((RECORD_FILE_IS_ONLYREAD(fileInfoTmp.type)) && (!bOnlyRead))
				{
					fileInfoTmp = fileInfo;
				}			
			}
			else
			{
				if (bOnlyRead)
				{
					RECORD_FILE_EN_ONLYREAD(fileInfo.type);
				}
				list.InsertAfter(pos, fileInfo);
			}
		}
	}
	else
	{
		//��Ҫ����ͷ��
		if (bOnlyRead)
		{
			RECORD_FILE_EN_ONLYREAD(fileInfo.type);
		}
		list.AddHead(fileInfo);
	}	
}

/************************************************************************
�������������б�list�У��ҵ���ʼʱ��С��time�Ľڵ㣬���һ���ڵ�Ŀ�ʼʱ��
			Ӧ�ô��ڻ��ߵ���time��
��ע��	1������б�Ϊ�գ���Ȼ����NULL���ⲿ���ݷ���ΪNULL������½ڵ�ֱ�Ӽ���ͷ����
		2�����time < list.GetHead().startTime��Ҳ����Ҫ����ͷ�������Է���NULL
		3��list.GetTail().startTime <= time������Ҫ���뵽β��
		4�������Ҫ���뵽�б��м䣬�����ö��ַ����Һ��ʵĽڵ�
************************************************************************/
POS	CReclogEx::GetPrevNodePos(FILE_INFO_EX_LIST &list, unsigned long time)
{
	if (list.IsEmpty())
	{
		return NULL;
	}
	else
	{
		if (time < list.GetHead().startTime)
		{
			return NULL;
		}
		else if (list.GetTail().startTime <= time)
		{
			return list.GetTailPosition();
		}
		else
		{
			//�ȼ���������Ľڵ�
			/* ��lastPosΪ���һ���ڵ��ǣ�����Ĵ���������
			POS lastPos = list.GetLastInsertPosition();
			if ((NULL != lastPos) && (list.GetAt(lastPos).startTime < time) && (time <= list.NextNode(lastPos).startTime))
			{
				return lastPos;
			}
			*/

			{
				POS lastPos =  NULL;
				POS nextPos = NULL;
				lastPos = list.GetLastInsertPosition();
				if (NULL != lastPos)
				{
					nextPos = lastPos;
					list.GetNext(nextPos);
				}
				if ((NULL != nextPos) && (list.GetAt(lastPos).startTime < time) && (time <= list.NextNode(lastPos).startTime))
				{
					return lastPos;
				}
			}
			
			/*
				���б�Ľڵ��Ӧ��������±꣬�����б��������ͷ�ڵ����Ŀ�����±�ֵ��
				headPos--����ÿ�β��ҵ�һ���б��ͷ�ڵ�
				pos--����ƥ��ĵ�ǰ�ڵ�
				h--����ǰ���Ҷε�ͷ�ڵ��±�
				t--����ǰ���Ҷε�β�ڵ��±�
				m--����ǰ���Ҷε��в��ڵ��±�
			*/
			POS headPos = list.GetHeadPosition();
			POS pos = NULL;
			long h=0, t=0, i=0, m=0, num = 0;
			t = list.GetCount()-1;

			while (h <= t)
			{
				pos = headPos;
				m = ((h+t)>>1);//�����м�ڵ���±�

				//��ζ���ҵ�����ֵ�ˣ�pos = headPos��
				if (h == m)
				{
					break;
				}

				//�ҵ�m��Ӧ�Ľڵ㣬��������˵�ýڵ�Ӧ�ô��ڣ������ҵ�NULL��
				i = 0;
				num = m - h;
				while (i < num)
				{
					list.GetNext(pos);
					++ i;
				}
				assert (NULL != pos);

				//�ѵ�ǰ�ε��м�ڵ�Ŀ�ʼʱ���time�Ƚϣ�
				//���node.startTime <= time������Ҫ�Ժ����ٽ��в��ң�����headPos = pos��h = m��
				//����Ҫ��ǰ��ν��в��ң�t = m��
				FILE_INFO_EX &node = list.GetAt(pos);
				if (node.startTime <= time)//
				{
					headPos = pos;
					h = m;
				}
				else
				{
					t = m;
				}
			}

			return pos;
		}
	}
}

/************************************************************************
���ļ���ű�indexС�Ľڵ㣬���һ���ڵ���ļ����Ӧ�ñ�index��
��ע��	�ο�GetPrevNodePos��ע�ͣ����������Ľṹһ����
************************************************************************/
POS	CReclogEx::GetPrevNodePosEx(FILE_INFO_EX_LIST &list, unsigned long index, unsigned long no)
{
	if (list.IsEmpty())
	{
		return NULL;
	}
	else
	{
		if (index < list.GetHead().fileIndex)
		{
			return NULL;
		}
		else if (list.GetTail().fileIndex < index)
		{
			return list.GetTailPosition();
		}
		else
		{
			//�ȼ���������Ľڵ�
			POS lastPos = list.GetLastInsertPosition();
			if ((NULL != lastPos) && (list.GetAt(lastPos).fileIndex < index) && (index < list.NextNode(lastPos).fileIndex))
			{
				return lastPos;
			}

			POS headPos = list.GetHeadPosition();
			POS pos = NULL;
			long h=0, t=0, i=0, m=0, num = 0;
			t = list.GetCount()-1;
			while (h <= t)
			{
				pos = headPos;
				m = ((h+t)>>1);

				if (h == m)
				{
					break;
				}

				i = 0;
				num = m - h;
				while (i < num)
				{
					list.GetNext(pos);
					++ i;
				}
				assert (NULL != pos);

				FILE_INFO_EX &node = list.GetAt(pos);
				if (node.fileIndex < index)
				{
					headPos = pos;
					h = m;
				}
				else
				{
					t = m;
				}
			}

			return pos;
		}
	}
}

/************************************************************************
���ҿ�ʼʱ����ڻ��ߵ���time�Ľڵ㣬��ǰһ���ڵ�Ŀ�ʼʱ��һ��С��time��
���һ���ڵ�Ŀ�ʼʱ��һ�������俪ʼʱ�䡣
************************************************************************/
POS	CReclogEx::GetNodePos(FILE_INFO_EX_LIST &list, unsigned long time,unsigned long endTime)
{
	if (list.IsEmpty())
	{
		return NULL;
	}
	else
	{
		if (time <= list.GetHead().startTime)
		{
			return list.GetHeadPosition();
		}
		/*
		else if ((0 == list.GetTail().endTime) && (GetCurrTime32() <= time))
		{
			//���β�ڵ�Ľ���ʱ������㣬���ʾ���ļ�ֻ��д�˿�ʼ����û��д������
			//������Ҫ�õ�ǰϵͳʱ���time�ȶԡ����Ҫ��Ŀ�ʼʱ�䣨time�����ڻ��ߵ��ڵ�ǰϵͳʱ�䣬
			//��˵�����������㣬Ӧ�÷���NULL��
			return NULL;
		}
		else if (list.GetTail().startTime < list.GetTail().endTime)
		{
			//���β�ڵ�Ŀ�ʼʱ��С�ڽ���ʱ�䣬���ʾ���ļ���һ���Ѿ�¼����ɵģ�������һ���Ѿ�д���������ļ���
			//������ļ�������¼����ļ�����������״̬��һ���Ѿ�ֹͣ¼�񣨵�ǰʱ�̴���ֹͣ״̬����һ������ֹͣ����
			//���������¿�ʼ¼��ġ�

			//��ǰ��Ӧ��ͨ������¼��������д���ݣ�������ζ��β�ڵ��type������RECORD_TYPE_NULL��
			if (RECORD_TYPE_NULL == m_pRecStatus[CHANNEL_ID(list.GetTail().chnn)].type)
			{
				//��¼��״̬����ȫֹͣд���ݣ�������endTime��time�ȶԡ�
				if (list.GetTail().endTime <= time)
				{
					return NULL;
				}
			}
			else
			{
				//¼��״̬��д���ݣ������õ�ǰϵͳʱ���time�ȶԡ�
				if (GetCurrTime32() <= time)
				{
					return NULL;
				}
			}

			//���������������Ҫ���أ���Ҫ��������Ĳ��衣 
		}
		*/
		
		{
#if 0
			//�ȼ��������ʵĽڵ�
			POS lastPos = list.GetLastReadPosition();
			if ((NULL != lastPos) && (list.prevNode(lastPos).endTime <= time) && (time <= list.GetAt(lastPos).startTime))
			{
				return lastPos;
			}

			//����ƶ�һ���ڵ㣬�ټ��
			list.GetNext(lastPos);
			if ((NULL != lastPos) && (list.prevNode(lastPos).endTime <= time) && (time <= list.GetAt(lastPos).startTime))
			{
				return lastPos;
			}
#endif

			POS headPos = list.GetHeadPosition();
			POS endPos = list.GetTailPosition();
			POS pos = NULL;
			long h=0, t=0, i=0, m=0, num = 0;
			t = list.GetCount()-1;
			
			while (h <= t)
			{			
				pos = headPos;
				m = ((h+t)>>1);

				i = 0;
				num = m - h;
				while (i < num)
				{
					list.GetNext(pos);
					++ i;
				}
				assert (NULL != pos);

				FILE_INFO_EX &node = list.GetAt(pos);
				if (node.startTime < time)
				{  
					h = m + 1;	
					if (h > t)
					{
						break;
					}
					else
					{
						list.GetNext(pos);
						if (NULL == pos)
						{
							pos = endPos;
							break;
						}
						headPos = pos;
					}					
				}
				else if (node.startTime == time)
				{
					break;
				}
				else
				{
					t = m - 1;
					
					list.GetPrev(pos);
					if (NULL == pos)
					{
						pos = headPos;
						break;
					}

					
					/*if ((t - h) <= 1)
					{
						break;
					}*/
					
				}
			}
			if (list.GetCount()>0)
			{
				POS posFind = list.GetHeadPosition();
				while(posFind != pos)
				{
					FILE_INFO_EX &node = list.GetAt(posFind);
					if ( (time < node.endTime)&&(node.endTime<endTime) )
					{
						pos = posFind;
						break;
					}
					list.GetNext(posFind);
				}
			}
			return pos;
		}
	}
}

/************************************************************************
���Ҳ����ļ���ŵ���index�Ľڵ㡣
************************************************************************/
POS	CReclogEx::GetNodePosEx(FILE_INFO_EX_LIST &list, unsigned long index)
{
	if (list.IsEmpty())
	{
		return NULL;
	}
	else
	{
		//����ƥ���������Ľڵ�
		if (NULL != list.GetLastInsertPosition())
		{
			FILE_INFO_EX &lastInsertNode = list.GetLastInsert();
			if (lastInsertNode.fileIndex == index)
			{
				return list.GetLastInsertPosition();
			}
		}
			
		//�������������ڵ�����������ڵ㲻��������
		{
			POS pos = list.GetHeadPosition();
			while (NULL != pos)
			{
				FILE_INFO_EX & node = list.GetAt(pos);
				if (node.fileIndex == index)
				{
					return pos;
				}

				list.GetNext(pos);
			}

			return NULL;
		}
	}
}

#if 0
POS CReclogEx::GetNextNodePos(FILE_INFO_EX_LIST &list, unsigned long time)
{
	if (list.IsEmpty())
	{
		return NULL;
	}
	else
	{
		if (time <= list.GetTail().startTime)
		{
			return NULL;
		}
		else if (list.GetHead().startTime < time)
		{
			return list.GetHeadPosition();
		}
		else
		{
			//�ȼ���������Ľڵ�
			POS lastPos = list.GetLastInsertPosition();
			if ((NULL != lastPos) && (list.GetAt(lastPos).startTime < time) && (time <= list.prevNode(lastPos).startTime))
			{
				return lastPos;
			}

			/*
			���б�Ľڵ��Ӧ��������±꣬�����б��������ͷ�ڵ����Ŀ�����±�ֵ��
			headPos--����ÿ�β��ҵ�һ���б��ͷ�ڵ�
			pos--����ƥ��ĵ�ǰ�ڵ�
			h--����ǰ���Ҷε�ͷ�ڵ��±�
			t--����ǰ���Ҷε�β�ڵ��±�
			m--����ǰ���Ҷε��в��ڵ��±�
			*/
			POS headPos = list.GetHeadPosition();
			POS pos = NULL;
			long h=0, t=0, i=0, m=0, num = 0;
			t = list.GetCount()-1;

			while (h <= t)
			{
				pos = headPos;
				m = ((h+t)>>1);//�����м�ڵ���±�

				//��ζ���ҵ�����ֵ�ˣ�pos = headPos��
				if (h == m)
				{
					break;
				}

				//�ҵ�m��Ӧ�Ľڵ㣬��������˵�ýڵ�Ӧ�ô��ڣ������ҵ�NULL��
				i = 0;
				num = m - h;
				while (i < num)
				{
					list.GetNext(pos);
					++ i;
				}
				assert (NULL != pos);

				//�ѵ�ǰ�ε��м�ڵ�Ŀ�ʼʱ���time�Ƚϣ�
				//���node.startTime >= time������Ҫ�Ժ����ٽ��в��ң�����headPos = pos��h = m��
				//����Ҫ��ǰ��ν��в��ң�t = m��
				FILE_INFO_EX &node = list.GetAt(pos);
				if (node.startTime >= time)//
				{
					headPos = pos;
					h = m;
				}
				else
				{
					t = m;
				}
			}

			return pos;
		}
	}
}

POS	CReclogEx::GetNextNodePosEx(FILE_INFO_EX_LIST &list, unsigned long index, unsigned long no)
{
	if (list.IsEmpty())
	{
		return NULL;
	}
	else
	{
		if (index < list.GetTail().fileIndex)
		{
			return NULL;
		}
		else if (list.GetHead().fileIndex < index)
		{
			return list.GetHeadPosition();
		}
		else
		{
			//�ȼ���������Ľڵ�
			POS lastPos = list.GetLastInsertPosition();
			if ((NULL != lastPos) && (list.GetAt(lastPos).fileIndex < index) && (index < list.prevNode(lastPos).fileIndex))
			{
				return lastPos;
			}

			POS headPos = list.GetHeadPosition();
			POS pos = NULL;
			long h=0, t=0, i=0, m=0, num = 0;
			t = list.GetCount()-1;
			while (h <= t)
			{
				pos = headPos;
				m = ((h+t)>>1);

				if (h == m)
				{
					break;
				}

				i = 0;
				num = m - h;
				while (i < num)
				{
					list.GetNext(pos);
					++ i;
				}
				assert (NULL != pos);

				FILE_INFO_EX &node = list.GetAt(pos);
				if (node.fileIndex > index)
				{
					headPos = pos;
					h = m;
				}
				else
				{
					t = m;
				}
			}

			return pos;
		}
	}
}

POS	CReclogEx::GetNodePosEx(FILE_INFO_EX_LIST &list, unsigned long time)
{
	if (list.IsEmpty())
	{
		return NULL;
	}
	else
	{
		FILE_INFO_EX node;
		if (time < list.GetTail().startTime)
		{
			return list.GetTailPosition();
		}
		else if ((0 == list.GetHead().startTime) && (0 == list.GetHead().endTime))
		{
			//��ʾ�����б����ǻ�û��������ʹ�ã���Ҫ������ǰһ���ڵ㣬��ȥ�жϡ�
			if (1 == list.GetCount())
			{
				return NULL;
			}
			else
			{
				POS pos = list.GetHeadPosition();
				node = list.GetNext(pos);
			}
		}
		else
		{
			node = list.GetHead();
		}

		if ((node.endTime <= time) || ((0 == node.endTime) && (GetCurrTime32() <= time)))
		{
			return NULL;
		}
		else
		{
			POS headPos = list.GetHeadPosition();
			POS pos = NULL;
			long h=0, t=0, i=0, m=0, num = 0;
			t = list.GetCount()-1;
			while (h <= t)
			{
				pos = headPos;
				m = ((h+t)>>1);

				if (h == m)
				{
					break;
				}

				i = 0;
				num = m - h;
				while (i < num)
				{
					list.GetNext(pos);
					++ i;
				}
				assert (NULL != pos);

				FILE_INFO_EX &node = list.GetAt(pos);
				if (node.startTime > time)
				{
					headPos = pos;
					h = m;
				}
				else
				{
					t = m;
				}
			}

			return pos;
		}
	}
}
#endif
/************************************************************************
������������ȡ�ļ��б����ļ����Ϊ����뷵���б�
���������	chnnBits--ͨ��ģ��
			startTime--��ʼʱ��
			endTime--����ʱ��
���������	list
�����ļ���Ŀ��
��ע��	1������������ҵ��˸�ͨ���б��������������ļ���Ӧ�Ľڵ㣬��ô�������
			��һЩ�ڵ㶼��������Ҫ�����������Ӧ�÷���95%�����ϣ���
		2����ÿ��ͨ���б������ڵ������ļ���Ż����һ����Ŀ��ʵ��¼��ͨ����Ŀ����
			���ֻ�Ǽ򵥵ذ���ͨ��ÿ�α��������б��ڼ��뷵���б�ʱ����������Ҫ����
			�Ƚ϶��ʱ��ȡ�
		3����������������2����˵�����������и��б��ۺϲ������ܽ�ʡ�ܶ��ʱ��ȡ�
		4���㷨Ϊ��
			a�������ҵ�ÿ���б�������������һ���ڵ㣻
			b��Ȼ����ļ������С��һ��ͨ����ʼ����������ĳ��ͨ��ʱ��һֱ��
				ĳ���ڵ���ļ���Ŵ��ڵ�ǰ����ͨ����һ���ڵ��е���СֵΪֹ��
			c��ĳ��ͨ������������Χ�����˳�����顣
************************************************************************/
unsigned long CReclogEx::GetFileList(ULONGLONG chnnbBits, unsigned long startTime, unsigned long endTime, FILE_INFO_EX_LIST & list)
{
	FILE_INFO_EX *pNode = new FILE_INFO_EX [m_headInfo.chnnNum];
	POS *pPos = new POS [m_headInfo.chnnNum];

	//���ȵõ�ÿ��ͨ�������������Ľڵ�
	unsigned char chnnNum = 0;
	for (unsigned char chnn=0; chnn<m_headInfo.chnnNum; ++chnn)
	{
		if (chnnbBits & ((ULONGLONG)0x01 << chnn))
		{
			pPos [chnn] = GetNodePos(m_pFileInfoList[chnn], startTime,endTime);
		}
		else
		{
			pPos [chnn] = NULL;
		}
	}

	//���ÿ���ǰ�ڵ��ֵ�����ڱȽϣ�����¼������Ҫ���ͨ����Ŀ��������һ�ֵ�ѭ����
	for (unsigned char chnn=0; chnn<m_headInfo.chnnNum; ++chnn)
	{
		if ((chnnbBits & ((ULONGLONG)0x01 << chnn)) && (NULL != pPos [chnn]))
		{
			pNode[chnn] = m_pFileInfoList[chnn].GetAt(pPos [chnn]);
			++ chnnNum;
		}
	}

	if (0 == chnnNum)	//û������������ͨ����������
	{
		delete [] pNode;
		delete [] pPos;
		return 0;
	}

	unsigned char i = 0;	//��ʾ��ǰ�ļ������С��ͨ�����
	unsigned short index = -1;	//����Ѱ���ļ������С��ͨ��
	unsigned long num = 0;
	while (chnnNum > 0)
	{
		//�����ļ������С��ͨ����ţ���i��¼��
		index = -1;
		for (unsigned char chnn=0; chnn<m_headInfo.chnnNum; ++chnn)
		{
			if ((chnnbBits & ((ULONGLONG)0x01 << chnn)) && (NULL != pPos [chnn]))
			{
				if (index > pNode[chnn].fileIndex)
				{
					index = pNode[chnn].fileIndex;
					i = chnn;
				}
			}
		}

		bool bOpenRecordFile = false;
		unsigned short curRecordFileIndex = -1;
		if (m_pChannelRecordFile[i].pFileList->GetCount() > 0)
		{
			bOpenRecordFile = true;
			assert(1 == m_pChannelRecordFile[i].pFileList->GetCount());
			curRecordFileIndex = m_pChannelRecordFile[i].pFileList->GetHead();
		}

		//��Ը�ͨ�������б����������������ļ����뷵���б�
		while (NULL != pPos[i])
		{
			pNode[i] = m_pFileInfoList[i].GetAt(pPos [i]);

			//�������ʱ��С��ţ�������ѭ��������Ѱ�Һ��ʵ�ͨ��
			for (unsigned char chnn=0; chnn<m_headInfo.chnnNum; ++chnn)
			{
				if ((NULL != pPos [chnn]) && (pNode[i].fileIndex > pNode[chnn].fileIndex))
				{
					break;
				}
			}

			/*
			if (RECORD_TYPE_NULL != m_pRecStatus[i].type)
			{
				pNode[i].endTime = GetCurrTime32();
			}
			*/

			//���ļ�������¼����ļ�
			if (pNode[i].fileIndex == curRecordFileIndex)
			{
				//¼���ļ����Ƿ�д�������Ϊ���֣�1.д����
				//2 δд��������û������д�롣3 δд��������д��
				//����ǵ�һ������������ļ��Ľ���ʱ��
				//����ǵڶ�������������ļ��ĵ�ǰ����ʱ��
				//����ǵ�������������õ�ǰʱ�䡣


				//2010-01-24 14:47:00 YSW
				//�����޶�ʱ����������
				unsigned long currTime = GetCurrTime32();

				//���ļ��Ŀ�ʼʱ�䴦���޶�ʱ���ڣ�currTime - MAX_TIME_OFFSET�� currTime������Ӧ�ñ����˵���
				if ((currTime - MAX_TIME_OFFSET) <= pNode[i].startTime)
				{
					//��Ӧ�ô��ڵ�ǰʱ��
					assert (pNode[i].startTime <= currTime);

					//ָ����һ�������˵�ǰ�ļ���
					m_pFileInfoList[i].GetNext(pPos [i]);
					chnnNum -= (NULL == pPos [i]) ? 1 : 0;	//����б������ɣ�ҲҪ�˳������
					continue;
				}

				if (RECORD_TYPE_NULL != m_pRecStatus[i].type)
				{
					//����εĿ�ʼʱ�䴦���޶�ʱ���ڣ�currTime - MAX_TIME_OFFSET�� currTime������Ӧ����ǰ�汻���˵��ˡ�
					//�����޶�ʱ���ڲ����������ʱ��Ҫ���޶����ϣ�currTime - MAX_TIME_OFFSET��
					pNode[i].endTime = currTime - MAX_TIME_OFFSET;
				}
				else
				{
					//�����Ƿ�ֹ����
					if(pNode[i].endTime < pNode[i].startTime)
					{
						assert(false);
						//����εĿ�ʼʱ�䴦���޶�ʱ���ڣ�currTime - MAX_TIME_OFFSET�� currTime������Ӧ����ǰ�汻���˵��ˡ�
						//�����޶�ʱ���ڲ����������ʱ��Ҫ���޶����ϣ�currTime - MAX_TIME_OFFSET��
						pNode[i].endTime = currTime - MAX_TIME_OFFSET;
					}
				}

				assert(bOpenRecordFile);
				assert(pNode[i].fileIndex == curRecordFileIndex);			
				assert(pNode[i].endTime > pNode[i].startTime);
			}

			if (!((pNode[i].endTime <= startTime) || (endTime <= pNode[i].startTime)))
			{
				AddToFileListForInitial(pNode[i], list, false);//���ļ��������

				++ num;
			}
			else if (endTime <= pNode[i].startTime)	//�˳������
			{
				pPos [i] = NULL;
				-- chnnNum;
				break;
			}

			m_pFileInfoList[i].GetNext(pPos [i]);
			chnnNum -= (NULL == pPos [i]) ? 1 : 0;	//����б������ɣ�ҲҪ�˳������
		}
	}

	delete [] pNode;
	delete [] pPos;

	return num;
}

/************************************************************************
������������ĳЩ����Ҫ����¼������б�β��
************************************************************************/


POS CReclogEx::GetSectionPrevNodePos(SECTION_INFO_EX_LIST &list, unsigned long time)
{
	if (list.IsEmpty())
	{
		return NULL;
	}
	else
	{
		if (time < list.GetHead().startTime)
		{
			return NULL;
		}
		else if (list.GetTail().startTime <= time)
		{
			return list.GetTailPosition();
		}
		else
		{
			//�ȼ���������Ľڵ�
			{
				POS lastPos = NULL;
				POS nextPos = NULL;
				lastPos = list.GetLastInsertPosition();
				if (lastPos != NULL)
				{
					nextPos = lastPos;
					list.GetNext(nextPos);
				}
				if ((NULL != nextPos) && (list.GetAt(lastPos).startTime < time) && (time <= list.NextNode(lastPos).startTime))
				{
					return lastPos;
				}
			}
			
			/*
			���б�Ľڵ��Ӧ��������±꣬�����б��������ͷ�ڵ����Ŀ�����±�ֵ��
			headPos--����ÿ�β��ҵ�һ���б��ͷ�ڵ�
			pos--����ƥ��ĵ�ǰ�ڵ�
			h--����ǰ���Ҷε�ͷ�ڵ��±�
			t--����ǰ���Ҷε�β�ڵ��±�
			m--����ǰ���Ҷε��в��ڵ��±�
			*/
			POS headPos = list.GetHeadPosition();
			POS pos = NULL;
			long h=0, t=0, i=0, m=0, num = 0;
			t = list.GetCount()-1;

			while (h <= t)
			{
				pos = headPos;
				m = ((h+t)>>1);//�����м�ڵ���±�

				//��ζ���ҵ�����ֵ�ˣ�pos = headPos��
				if (h == m)
				{
					break;
				}

				//�ҵ�m��Ӧ�Ľڵ㣬��������˵�ýڵ�Ӧ�ô��ڣ������ҵ�NULL��
				i = 0;
				num = m - h;
				while (i < num)
				{
					list.GetNext(pos);
					++ i;
				}
				assert (NULL != pos);

				//�ѵ�ǰ�ε��м�ڵ�Ŀ�ʼʱ���time�Ƚϣ�
				//���node.startTime <= time������Ҫ�Ժ����ٽ��в��ң�����headPos = pos��h = m��
				//����Ҫ��ǰ��ν��в��ң�t = m��
				SECTION_INFO_EX &node = list.GetAt(pos);
				if (node.startTime <= time)//
				{
					headPos = pos;
					h = m;
				}
				else
				{
					t = m;
				}

			}

			return pos;

		}
	}

}

POS CReclogEx::GetReclogPrevNodePos(RECLOG_INFO_EX_LIST &list, unsigned long time)
{
	if (list.IsEmpty())
	{
		return NULL;
	}
	else
	{
		if (time < list.GetHead().startTime)
		{
			return NULL;
		}
		else if (list.GetTail().startTime <= time)
		{
			return list.GetTailPosition();
		}
		else
		{
			//�ȼ���������Ľڵ�
			{
				POS lastPos = NULL;
				POS nextPos = NULL;
				lastPos = list.GetLastInsertPosition();
				if (lastPos != NULL)
				{
					nextPos = lastPos;
					list.GetNext(nextPos);
				}
				if ((NULL != nextPos) && (list.GetAt(lastPos).startTime < time) && (time <= list.NextNode(lastPos).startTime))
				{
					return lastPos;
				}
			}
			
			/*
			���б�Ľڵ��Ӧ��������±꣬�����б��������ͷ�ڵ����Ŀ�����±�ֵ��
			headPos--����ÿ�β��ҵ�һ���б��ͷ�ڵ�
			pos--����ƥ��ĵ�ǰ�ڵ�
			h--����ǰ���Ҷε�ͷ�ڵ��±�
			t--����ǰ���Ҷε�β�ڵ��±�
			m--����ǰ���Ҷε��в��ڵ��±�
			*/
			POS headPos = list.GetHeadPosition();
			POS pos = NULL;
			long h=0, t=0, i=0, m=0, num = 0;
			t = list.GetCount()-1;

			while (h <= t)
			{
				pos = headPos;
				m = ((h+t)>>1);//�����м�ڵ���±�

				//��ζ���ҵ�����ֵ�ˣ�pos = headPos��
				if (h == m)
				{
					break;
				}

				//�ҵ�m��Ӧ�Ľڵ㣬��������˵�ýڵ�Ӧ�ô��ڣ������ҵ�NULL��
				i = 0;
				num = m - h;
				while (i < num)
				{
					list.GetNext(pos);
					++ i;
				}
				assert (NULL != pos);

				//�ѵ�ǰ�ε��м�ڵ�Ŀ�ʼʱ���time�Ƚϣ�
				//���node.startTime <= time������Ҫ�Ժ����ٽ��в��ң�����headPos = pos��h = m��
				//����Ҫ��ǰ��ν��в��ң�t = m��
				RECLOG_INFO_EX &node = list.GetAt(pos);
				if (node.startTime <= time)//
				{
					headPos = pos;
					h = m;
				}
				else
				{
					t = m;
				}

			}

			return pos;

		}
	}
}

unsigned long CReclogEx::AddToReclogList(FILE_RECLOG_INFO *pData, unsigned long type, \
										 unsigned long startTime, unsigned long endTime,
										 RECLOG_INFO_EX_LIST &list, 
										 const FILE_INFO_EX &  fileInfo, bool bOnlyRead, bool bRepeat)
{
	unsigned long currTime = GetCurrTime32() - MAX_TIME_OFFSET;

	unsigned char recTypeNum = RecTypeNum();
	assert(recTypeNum > 0);

	RECLOG_INFO_EX *pReclog = new RECLOG_INFO_EX [recTypeNum];
	memset(pReclog, 0, sizeof(RECLOG_INFO_EX)*recTypeNum);

	unsigned long getNum = 0;
    unsigned long oldNum = list.GetCount();
	for (unsigned long i=0; i<m_headInfo.fileLogNum; ++i)
	{
		//��ʾ��ʼ
		if ((RECORD_TYPE_NULL != pData[i].type) && (pData[i].type < RECORD_TYPE_END) && \
			(0 == (pData[i].type & RECLOG_END)))
		{
			pReclog [RecTypeIndex(static_cast<RECORD_TYPE>(pData[i].type))].chnn		= fileInfo.chnn;
			pReclog [RecTypeIndex(static_cast<RECORD_TYPE>(pData[i].type))].type		= pData[i].type;
			pReclog [RecTypeIndex(static_cast<RECORD_TYPE>(pData[i].type))].startTime	= pData[i].time;

			pReclog [RecTypeIndex(static_cast<RECORD_TYPE>(pData[i].type))].diskIndex	= fileInfo.diskIndex;
			pReclog [RecTypeIndex(static_cast<RECORD_TYPE>(pData[i].type))].fileIndex	= fileInfo.fileIndex;

		}

		//��ʾ��������ʱ�䲻Ϊ�㡣
		if ((RECORD_TYPE_NULL != pData[i].type) && ((pData[i].type - RECLOG_END) < RECORD_TYPE_END) &&\
			(0 < pData[i].time) && (pData[i].type & RECLOG_END))
		{
			assert (pReclog [RecTypeIndex(static_cast<RECORD_TYPE>(pData[i].type))].type == (pData[i].type - RECLOG_END));

			pReclog [RecTypeIndex(static_cast<RECORD_TYPE>(pData[i].type))].endTime		= pData[i].time;//(currTime < pData[i].time) ? currTime : pData[i].time;

			//�������͹���
			if (pReclog [RecTypeIndex(static_cast<RECORD_TYPE>(pData[i].type))].type & type)
			{
				//2010-02-27 09:29:00 YSW
				//���ʱ���߼������ԣ����翪ʼʱ��ҪС�ڻ��ߵ��ڽ���ʱ��(ע�⣺��ʱ����ʱ���Ѿ��õ�������
				if (pReclog [RecTypeIndex(static_cast<RECORD_TYPE>(pData[i].type))].startTime <= pReclog [RecTypeIndex(static_cast<RECORD_TYPE>(pData[i].type))].endTime)
				{
					//����ʱ�����(2010-01-24 15:20:00 YSW)
					if ( !( (pReclog [RecTypeIndex(static_cast<RECORD_TYPE>(pData[i].type))].endTime <= startTime) ||\
						(endTime <= pReclog [RecTypeIndex(static_cast<RECORD_TYPE>(pData[i].type))].startTime) ) )
					{
						pReclog [RecTypeIndex(static_cast<RECORD_TYPE>(pData[i].type))].type |= (fileInfo.type & 0xffff0000);
						AddToReclogList (pReclog [RecTypeIndex(static_cast<RECORD_TYPE>(pData[i].type))], list, bOnlyRead, bRepeat);
					}
				}
			}

			memset(&pReclog [RecTypeIndex(static_cast<RECORD_TYPE>(pData[i].type))], 0, sizeof(RECLOG_INFO_EX));
		}

		//��ʾ��Ԥ���ռ�δʹ��
		if ((RECORD_TYPE_NULL == pData[i].type) && (0 == pData[i].time))
		{
			break;
		}
	}

	assert(list.GetCount() >= oldNum);
	getNum += list.GetCount() - oldNum;

	//��������д���¼�����Ϊ��û�н�����������Ҫ�ڴ˸����俪ʼ�������ϵ�ǰʱ��Ϊ����ʱ����Ϊһ���¼������б�
	oldNum = list.GetCount();
	for (unsigned char i=0; i<recTypeNum; ++i)
	{
		if ((RECORD_TYPE_NULL != pReclog [i].type) && (pReclog [i].type < RECORD_TYPE_END))
		{
			//�����޶�����(2010-01-24 15:20:00 YSW)
			if (currTime <= pReclog [i].startTime)
			{
				continue;
			}

			pReclog [i].endTime		= (currTime + MAX_TIME_OFFSET);//�޶�ʱ���
            assert(pReclog[i].endTime > pReclog[i].startTime);

			if (pReclog [i].type & type)
			{
				//����ʱ�����(2010-02-27 09:36:00 YSW)
				if ( !( (pReclog [i].endTime <= startTime) ||\
					(endTime <= pReclog [i].startTime) ) )
				{
					AddToReclogList (pReclog [i], list, bOnlyRead, bRepeat);
				}
				
			}
		}
	}
	assert(list.GetCount() >= oldNum);
	getNum += list.GetCount() - oldNum;

	delete [] pReclog;
	return getNum;
}

void CReclogEx::AddToReclogList(RECLOG_INFO_EX reclog, RECLOG_INFO_EX_LIST &list, bool bOnlyRead, bool bRepeat)
{
	/*unsigned long currTime = GetCurrTime32();
	if (currTime < reclog.endTime)
	{
		reclog.endTime = currTime;
	}*/

	POS pos = NULL;

	//���տ�ʼʱ������
	pos = GetReclogPrevNodePos(list, reclog.startTime);
	if (NULL != pos)
	{
		if (bRepeat)
		{
			if (bOnlyRead)
			{
				RECORD_FILE_EN_ONLYREAD(reclog.type);
			}
			list.InsertAfter(pos, reclog);
		}
		else
		{
			RECLOG_INFO_EX &reclogTmp = list.GetAt(pos);
			if ((reclogTmp.startTime == reclog.startTime)
              &&(reclogTmp.type == reclog.type)
			  &&(reclogTmp.chnn == reclog.chnn))
			{
				//�˴���Ϊ�˰� ��¼���е��¼� �� ����¼���е��¼� �У���ͬ���¼�ֻ
				//����һ����¼��
				//�����¼���ͬ�������ǣ�
				//1. reclogTmp.startTime �� reclog.startTimeӦ������ȫ��ȵ�
				//2. reclogTmp.type �� reclog.typeӦ������ȫ��ȵ�
				//3. reclogTmp.endTime��reclog.endTimeӦ������ȫ��ȵġ�
				//   �����п������޸�����ļ�ʱ������ƫ������Ͻ����ƫ��Ҳ���Ǽ���
				//4  reclogTmp.chnn �� reclog.chnnӦ������ȫ��ȵ�.
				//��������ظ����ļ���ֻҪ��һ������ֻ���ģ���Ҫ�Բ�ֻ�����Լ��롣
				//��Ϊ���Ƕ�ֻ�����ļ�������ֻ��������ֻ�����ļ��϶�����ֻ��Ӳ�̣���ôѭ���Ͳ����ܸ�������ļ���
				//����Ҫ�����Է�ֻ��Ӳ�̵��ļ�Ϊ׼��ֻ���������ǲſ�����������ļ���
				if ((RECORD_FILE_IS_ONLYREAD(reclogTmp.type)) && (!bOnlyRead))
				{
					reclogTmp = reclog;
				}
			}
			else
			{
				if (bOnlyRead)
				{
					RECORD_FILE_EN_ONLYREAD(reclog.type);
				}
				list.InsertAfter(pos, reclog);
			}
		}
	}
	else
	{
		//��Ҫ����ͷ��
		if (bOnlyRead)
		{
			RECORD_FILE_EN_ONLYREAD(reclog.type);
		}
		list.AddHead(reclog);
	}		
	
}


void CReclogEx::AddToSectionList(const SECTION_INFO_EX & section, SECTION_INFO_EX_LIST &list, bool bRepeat)
{
	POS pos = NULL;

	//���տ�ʼʱ������
	pos = GetSectionPrevNodePos(list, section.startTime);
	if (NULL != pos)
	{
		if (bRepeat)
		{
			list.InsertAfter(pos, section);
		}
		else
		{
			SECTION_INFO_EX &sectionTmp = list.GetAt(pos);
			if (sectionTmp.startTime == section.startTime)
			{
				//sectionTmp.endTime��section.endTimeӦ������ȫ��ȵġ�
				//�����п������޸�����ļ�ʱ������ƫ������Ͻ����ƫ��Ҳ���Ǽ���
				//assert(sectionTmp.endTime == section.endTime);
			}
			else
			{
				list.InsertAfter(pos, section);
			}
		}
	}
	else
	{
		//��Ҫ����ͷ��
		list.AddHead(section);
	}		
}
/************************************************************************
������������fileIndex�ļ���ʼ��ȡnum���ļ�����Ϣ��
��ע��	���ݻ�������num���������������
************************************************************************/
unsigned long CReclogEx::ReadFileInfo (FILE_FILE_INFO *pData, unsigned long bufLen, unsigned long num, unsigned short fileIndex)
{
	assert (NULL != pData);
	assert (num > 0);
	assert (fileIndex < m_headInfo.fileNum);
	assert (bufLen >= (sizeof(FILE_FILE_INFO)*num));
	if ((NULL == pData) || (0 == num) || (fileIndex >= m_headInfo.fileNum) || (bufLen < (sizeof(FILE_FILE_INFO)*num)))
	{
		return 0;
	}

	//���Ҫ������Ŀ�Ƿ��Ѿ�����������Ŀ
	if (m_headInfo.fileNum < (fileIndex + num))
	{
		num = m_headInfo.fileNum - fileIndex;
	}

	if (0 == fseek(m_pFile, m_headInfo.fileInfoOffset+fileIndex*sizeof(FILE_FILE_INFO), SEEK_SET))
	{
		size_t ret = fread(pData, sizeof(FILE_FILE_INFO), num, m_pFile);
		if (ret > 0)
		{
			return (unsigned long)ret;
		}
		else
		{
			return 0;
		}
	}
	else
	{
		return 0;
	}
}

/************************************************************************
���������������ΪfileIndex�ļ���ʼ��ȡnum���ļ��Ķ���Ϣ��
��ע��	���ݻ�������num���������������
************************************************************************/
unsigned long CReclogEx::ReadSection (SECTION_INFO_EX *pData, unsigned long bufLen, unsigned long num, unsigned short fileIndex)
{
	assert (NULL != pData);
	assert (num > 0);
	assert (fileIndex < m_headInfo.fileNum);
	assert (bufLen >= (sizeof(SECTION_INFO_EX)*m_headInfo.fileRecSecNum*num));
	if ((NULL == pData) || (0 == num) || (fileIndex >= m_headInfo.fileNum) || (bufLen < (sizeof(SECTION_INFO_EX)*m_headInfo.fileRecSecNum*num)))
	{
		return 0;
	}

	//���Ҫ������Ŀ�Ƿ��Ѿ�����������Ŀ
	if (m_headInfo.fileNum < (fileIndex + num))
	{
		num = m_headInfo.fileNum - fileIndex;
	}

	if (0 == fseek(m_pFile, m_headInfo.sectionOffset+fileIndex*sizeof(SECTION_INFO_EX)*m_headInfo.fileRecSecNum, SEEK_SET))
	{
		size_t ret = fread(pData, sizeof(SECTION_INFO_EX)*m_headInfo.fileRecSecNum, num, m_pFile);
		if (ret > 0)
		{
			return (unsigned long)ret;
		}
		else
		{
			return 0;
		}
	}
	else
	{
		return 0;
	}
}

/************************************************************************
���������������ΪfileIndex�ļ���ʼ��ȡnum���ļ����¼���¼��Ϣ��
��ע��	���ݻ�������num���������������
************************************************************************/
unsigned long CReclogEx::ReadReclog (FILE_RECLOG_INFO *pData, unsigned long bufLen, unsigned long num, unsigned short fileIndex)
{
	assert (NULL != pData);
	assert (num > 0);
	assert (fileIndex < m_headInfo.fileNum);
	assert (bufLen >= (sizeof(FILE_RECLOG_INFO)*m_headInfo.fileLogNum*num));
	if ((NULL == pData) || (0 == num) || (fileIndex >= m_headInfo.fileNum) || (bufLen < (sizeof(FILE_RECLOG_INFO)*m_headInfo.fileLogNum*num)))
	{
		return 0;
	}

	//���Ҫ������Ŀ�Ƿ��Ѿ�����������Ŀ
	if (m_headInfo.fileNum < (fileIndex + num))
	{
		num = m_headInfo.fileNum - fileIndex;
	}

	if (0 == fseek(m_pFile, m_headInfo.reclogoffset+fileIndex*sizeof(FILE_RECLOG_INFO)*m_headInfo.fileLogNum, SEEK_SET))
	{
		size_t ret = fread(pData, sizeof(FILE_RECLOG_INFO)*m_headInfo.fileLogNum, num, m_pFile);
		if (ret > 0)
		{
			return (unsigned long)ret;
		}
		else
		{
			return 0;
		}
	}
	else
	{
		return 0;
	}
}

bool CReclogEx::WriteFileInfo(unsigned long fileIndex, FILE_FILE_INFO *pFileInfo)
{
	if (0 == fseek(m_pFile, m_headInfo.fileInfoOffset+fileIndex*sizeof(FILE_FILE_INFO), SEEK_SET))
	{
		if (1 == fwrite(pFileInfo, sizeof(FILE_FILE_INFO), 1, m_pFile))
		{
			fflush(m_pFile);
			return true;
		}
	}

	return false;
}

bool CReclogEx::WriteSection(unsigned long fileIndex, unsigned long chnn, unsigned long time, bool bStart)
{
	//2010-05-21 13:56:00 YSW
	//�¼���¼Ԥ���Ŀռ��Ѿ�д�����򷵻�ʧ�ܡ�
	if (bStart && (m_pRecStatus[CHANNEL_ID(chnn)].remainReclogNum < 2))
	{
		return false;
	}

	//Ԥ���ռ��Ѿ�д�����򷵻�ʧ��
	if (m_headInfo.fileRecSecNum <= m_pRecStatus[CHANNEL_ID(chnn)].section_index)
	{
		return false;
	}

	unsigned long offset = m_headInfo.sectionOffset + \
							fileIndex*m_headInfo.fileRecSecNum*sizeof(SECTION_INFO_EX) + \
							m_pRecStatus[CHANNEL_ID(chnn)].section_index*sizeof(SECTION_INFO_EX);

	//��ʼʱ��дǰ���ĸ��ֽڣ�����ʱ��д�����ĸ��ֽڡ�
	offset += (bStart ? 0 : sizeof(unsigned long));

	//2010-02=27 09:07:00 YSW
	//�����д��һ���Ŀ�ʼ�����ȰѸ��ļ���Ӧ�Ķ���Ϣ����ȫ������.
	if (bStart && (0 == m_pRecStatus[CHANNEL_ID(chnn)].section_index))
	{
		if (0 != fseek(m_pFile, offset, SEEK_SET))
		{
			return false;
		}

		if (NULL == m_pCleanDataBuf)
		{
			unsigned long len = (sizeof(SECTION_INFO_EX) * m_headInfo.fileRecSecNum);
			len = (len < (sizeof(FILE_RECLOG_INFO) * m_headInfo.fileLogNum)) ? (sizeof(FILE_RECLOG_INFO) * m_headInfo.fileLogNum) : len;
		
			m_pCleanDataBuf = new unsigned char [len];
			memset(m_pCleanDataBuf, 0, len);
		}

		if (1 != fwrite(m_pCleanDataBuf, sizeof(SECTION_INFO_EX)*m_headInfo.fileRecSecNum, 1, m_pFile))
		{
			return false;
		}
	}

	if (0 == fseek(m_pFile, offset, SEEK_SET))
	{
		if (1 == fwrite(&time, sizeof(unsigned long), 1, m_pFile))
		{
			if (!bStart)
			{
				//����ʱ��ָ����һ����¼��������
				++ m_pRecStatus[CHANNEL_ID(chnn)].section_index;
			}
			
			fflush(m_pFile);
			return true;
		}
	}

	return false;
}
 
bool CReclogEx::WriteType(unsigned long fileIndex, unsigned long chnn, RECORD_TYPE type, unsigned long time, bool bStart)
{
	//�����¼���־����дһ���¼��Ŀ�ʼ��ʱ��ҪͬʱΪ���¼��Ľ���Ԥ���ÿռ䡣
	assert(m_pRecStatus[CHANNEL_ID(chnn)].remainReclogNum <= m_headInfo.fileLogNum);
	if (bStart)
	{
		//Ԥ���ÿռ��Ѿ�д�����򷵻�ʧ��
		if (m_pRecStatus[CHANNEL_ID(chnn)].remainReclogNum < 2)
		{
			return false;
		}
		m_pRecStatus[CHANNEL_ID(chnn)].remainReclogNum -= 2;
	}
	assert(m_pRecStatus[CHANNEL_ID(chnn)].reclog_index < m_headInfo.fileLogNum);
	
	FILE_RECLOG_INFO reclog_info;

	unsigned long offset =	m_headInfo.reclogoffset + \
							fileIndex*m_headInfo.fileLogNum*sizeof(FILE_RECLOG_INFO) + \
							m_pRecStatus[CHANNEL_ID(chnn)].reclog_index*sizeof(FILE_RECLOG_INFO);
	
	//2010-02=27 09:16:00 YSW
	//�����д��һ���Ŀ�ʼ�����ȰѸ��ļ���Ӧ���¼���Ϣ����ȫ������.
	if (bStart && (0 == m_pRecStatus[CHANNEL_ID(chnn)].reclog_index))
	{
		if (0 != fseek(m_pFile, offset, SEEK_SET))
		{
			return false;
		}

		if (NULL == m_pCleanDataBuf)
		{
			unsigned long len = (sizeof(SECTION_INFO_EX) * m_headInfo.fileRecSecNum);
			len = (len < (sizeof(FILE_RECLOG_INFO) * m_headInfo.fileLogNum)) ? (sizeof(FILE_RECLOG_INFO) * m_headInfo.fileLogNum) : len;

			m_pCleanDataBuf = new unsigned char [len];
			memset(m_pCleanDataBuf, 0, len);
		}

		if (1 != fwrite(m_pCleanDataBuf, sizeof(FILE_RECLOG_INFO)*m_headInfo.fileLogNum, 1, m_pFile))
		{
			return false;
		}
	}

	//�����¼���¼
	{
		reclog_info.type = type;
		if (!bStart)	//����������
		{
			reclog_info.type |= RECLOG_END;
		}

		reclog_info.time = time;
	}

	if (0 == fseek(m_pFile, offset, SEEK_SET))
	{
		if (1 == fwrite(&reclog_info, sizeof(FILE_RECLOG_INFO), 1, m_pFile))
		{
			++ m_pRecStatus[CHANNEL_ID(chnn)].reclog_index;
			
			fflush(m_pFile);
			return true;
		}
	}

	return false;
}

bool CReclogEx::WriteFileInfoEnd(unsigned long fileIndex)
{
	if (0 == fseek(m_pFile, m_headInfo.fileInfoOffset+fileIndex*sizeof(FILE_FILE_INFO), SEEK_SET))
	{
		FILE_FILE_INFO fileInfo;

		if (1 == fread(&fileInfo, sizeof(FILE_FILE_INFO), 1, m_pFile))
		{
			if (RECORD_TYPE_NULL != fileInfo.type)
			{
				if (0 == fseek(m_pFile, m_headInfo.fileInfoOffset+fileIndex*sizeof(FILE_FILE_INFO), SEEK_SET))
				{
					assert (FILE_STATUS_WRITING == fileInfo.status);

					fileInfo.status = FILE_STATUS_END;

					if (1 == fwrite(&fileInfo, sizeof(FILE_FILE_INFO), 1, m_pFile))
					{
						fflush(m_pFile);
						return true;
					}
				}
				
			}
		}
	}

	return false;
}

bool CReclogEx::DeleteFileInfo(unsigned long fileIndex)
{
	{
		unsigned long offset = m_headInfo.fileInfoOffset + fileIndex*sizeof(FILE_FILE_INFO);	
		if (0 != fseek(m_pFile, offset, SEEK_SET))
		{
			return false;
		}
		FILE_FILE_INFO fileFileInfo;
		memset(&fileFileInfo, 0, sizeof(FILE_FILE_INFO));
		if (1 != fwrite(&fileFileInfo, sizeof(FILE_FILE_INFO), 1, m_pFile))
		{		
			return false;
		}
	}
	{
		unsigned long offset = m_headInfo.sectionOffset + fileIndex*m_headInfo.fileRecSecNum*sizeof(SECTION_INFO_EX);
		if (0 != fseek(m_pFile, offset, SEEK_SET))
		{
			return false;
		}
		SECTION_INFO_EX *pSectionInfo = new SECTION_INFO_EX [m_headInfo.fileRecSecNum];
		memset(pSectionInfo, 0, sizeof(SECTION_INFO_EX)*m_headInfo.fileRecSecNum);
		if (m_headInfo.fileRecSecNum != fwrite(pSectionInfo, sizeof(SECTION_INFO_EX), m_headInfo.fileRecSecNum, m_pFile))
		{
			delete []pSectionInfo;
			return false;
		}
		delete []pSectionInfo;
		pSectionInfo = NULL;
	}
	{
		unsigned long offset = m_headInfo.reclogoffset + fileIndex*m_headInfo.fileLogNum*sizeof(FILE_RECLOG_INFO);
		if (0 != fseek(m_pFile, offset, SEEK_SET))
		{
			return false;
		}
		FILE_RECLOG_INFO *pFileReclogInfo = new FILE_RECLOG_INFO [m_headInfo.fileLogNum];
		memset(pFileReclogInfo, 0 ,sizeof(FILE_RECLOG_INFO)*m_headInfo.fileLogNum);
		if (m_headInfo.fileLogNum != fwrite(pFileReclogInfo, sizeof(FILE_RECLOG_INFO), m_headInfo.fileLogNum, m_pFile))
		{
			delete [] pFileReclogInfo;
			return false;
		}
		delete [] pFileReclogInfo;
		pFileReclogInfo = NULL;
	}

	fflush(m_pFile);
	return true;

}
unsigned long CReclogEx::AddToDateList(const DVR_DATE &date, CMyList <DVR_DATE> &list)
{
	if (list.IsEmpty())
	{
		list.AddTail(date);
		return 1;
	}
	else
	{
		if (date.time < list.GetHead().time)
		{
			list.AddHead(date);
			return 1;
		}
		else if (list.GetTail().time < date.time)
		{
			list.AddTail(date);
			return 1;
		}
		else
		{
			POS pos = list.GetHeadPosition();
			while (NULL != pos)
			{
				DVR_DATE & node = list.GetAt(pos);
				if (date.time < node.time)
				{
					list.InsertBefore(pos, date);
					return 1;
				}
				else if (date.time == node.time)
				{
					return 0;
				}
				else
				{
					list.GetNext(pos);
				}
			}

			list.AddTail(date);
			return 1;
		}
	}
}

void CReclogEx::AddToDateList(unsigned long chnn, unsigned short fileIndex, unsigned long startTime, unsigned long endTime)
{
	//�˺����������ǣ����ļ��ż��뵽�� startTime �� endTime���������ڽڵ��С�
	//�˺�����֧�ְ������ļ��ķ�ʽ��startTime��endTime��Ϊ�ļ��Ŀ�ʼ�ͽ���ʱ�䣩�����á�
	//Ҳ���Ը����ļ��еĶ���Ϣ��������ļ����뵽��Ӧ�����ڽڵ��С�
	//
	//����ļ�û�п��죬��Ҫ�������ļ��ķ�ʽ        
	//����ǿ�����ļ�����Ҫ������Ϣ��������ļ����뵽��Ӧ�����ڽڵ��С�
	//
	chnn = CHANNEL_ID(chnn);
	assert (chnn < m_headInfo.chnnNum);
	assert (NULL != m_pDateInfoList);

	if (NULL != m_pDateInfoList)
	{
		DATE_INFO date;
		memset(&date, 0, sizeof(DATE_INFO));

		if (m_pDateInfoList[chnn].IsEmpty())
		{

			date.time = GetZeroTime32OfDate(startTime);
			do 
			{
				date.pFileList = new CMyList <unsigned short>;
				assert (NULL != date.pFileList);
				//date.pFileList->AddTail(fileIndex);
				AddToList(fileIndex, *date.pFileList);
				m_pDateInfoList[chnn].AddTail(date);
				date.time += ONE_DAY_SECOND_NUM;
			} while(date.time < endTime);
		}
		else
		{
			POS lastPos = m_pDateInfoList[chnn].GetLastInsertPosition();
			if (NULL == lastPos)
			{
				lastPos = m_pDateInfoList[chnn].GetTailPosition();
			}

			{
				DATE_INFO node = m_pDateInfoList[chnn].GetAt(lastPos);
				POS pos = lastPos;
			
				if (startTime <= node.time)
				{
					date.time = (endTime <= node.time) ? GetZeroTime32OfDate(endTime) : GetZeroTime32OfDate(node.time);

					while (GetZeroTime32OfDate(startTime) <= date.time)
					{
						
						if (NULL == pos)
						{
							date.pFileList	= new CMyList <unsigned short>;
							assert (NULL != date.pFileList);
							//date.pFileList->AddTail(fileIndex);
							AddToList(fileIndex, *date.pFileList);
							m_pDateInfoList[chnn].AddHead(date);
						}
						else
						{
							node = m_pDateInfoList[chnn].GetAt(pos);
							if (date.time < node.time)
							{
								m_pDateInfoList[chnn].GetPrev(pos);
								continue;
							}
							else if (date.time == node.time)
							{
								//node.pFileList->AddTail(fileIndex);
								AddToList(fileIndex, *node.pFileList);
								m_pDateInfoList[chnn].GetPrev(pos);
							}
							else
							{
								date.pFileList	= new CMyList <unsigned short>;
								assert (NULL != date.pFileList);
								//date.pFileList->AddTail(fileIndex);
								AddToList(fileIndex, *date.pFileList);
								m_pDateInfoList[chnn].InsertAfter(pos, date);
							}
						}

						date.time -= ONE_DAY_SECOND_NUM;
					}
				}
			}

			{
				DATE_INFO node = m_pDateInfoList[chnn].GetAt(lastPos);
				POS pos = lastPos;
				if (node.time < endTime)
				{
					date.time = (node.time <= startTime) ? GetZeroTime32OfDate(startTime) : (ONE_DAY_SECOND_NUM + GetZeroTime32OfDate(node.time));

					while (date.time < endTime)
					{
					
						if (NULL == pos)
						{
							date.pFileList	= new CMyList <unsigned short>;
							assert (NULL != date.pFileList);
							//date.pFileList->AddTail(fileIndex);
							AddToList(fileIndex, *date.pFileList);
							m_pDateInfoList[chnn].AddTail(date);
						}
						else
						{
							node = m_pDateInfoList[chnn].GetAt(pos);
							if (date.time < node.time)
							{
								date.pFileList	= new CMyList <unsigned short>;
								assert (NULL != date.pFileList);
								//date.pFileList->AddTail(fileIndex);
								AddToList(fileIndex, *date.pFileList);
								m_pDateInfoList[chnn].InsertBefore(pos, date);
							}
							else if (date.time == node.time)
							{
								//node.pFileList->AddTail(fileIndex);
								AddToList(fileIndex, *node.pFileList);
								m_pDateInfoList[chnn].GetNext(pos);
							}
							else
							{
								m_pDateInfoList[chnn].GetNext(pos);
								continue;
							}
						}

						date.time += ONE_DAY_SECOND_NUM;
					}
				}
			}
		}
	}
}

/*
����µ��ļ���ʱ�������Ƿ��Ѿ������������б������һ�����ڣ�����Ҫ�����µ����ڽڵ㡣
����Ҫ����ļ��Ѿ��������б��У�Ȼ�����Ƿ���Ҫ�����µ����ڽڵ㡣
*/
void CReclogEx::UpdateDateList(unsigned long chnn, unsigned short fileIndex, unsigned long startTime, unsigned long endTime)
{
#if 0
	chnn = CHANNEL_ID(chnn);
	assert (chnn < m_headInfo.chnnNum);
	assert (NULL != m_pDateInfoList);

	if (NULL != m_pDateInfoList)
	{
		assert (m_pDateInfoList[chnn].GetCount() > 0);
		assert (NULL != m_pDateInfoList[chnn].GetTail().pFileList);
		assert (m_pDateInfoList[chnn].GetTail().pFileList->GetCount() > 0);
		assert (m_pDateInfoList[chnn].GetTail().pFileList->GetTail() == fileIndex);

		DATE_INFO &node = m_pDateInfoList[chnn].GetTail();
		assert ((node.time + ONE_DAY_SECOND_NUM) > startTime);//β�ڵ���ļ���Ӧ����startTime��Ȼ�ڸ����ڡ�

		if ((node.time + ONE_DAY_SECOND_NUM) < endTime)//������ǰ������
		{
			DATE_INFO date;
			date.time = node.time + ONE_DAY_SECOND_NUM;//ʹ�ý�����������ڵ�һ�쿪ʼʱ��

			do 
			{
				date.pFileList = new CMyList <unsigned short>;
				assert (NULL != date.pFileList);
				//date.pFileList->AddTail(fileIndex);
				AddToList(fileIndex, *date.pFileList);
				m_pDateInfoList[chnn].AddTail(date);
				date.time += ONE_DAY_SECOND_NUM;
			} while(date.time < endTime);
		}
	}
#else
	chnn = CHANNEL_ID(chnn);
	assert (chnn < m_headInfo.chnnNum);
	assert (NULL != m_pDateInfoList);

	if (NULL != m_pDateInfoList)
	{
		DATE_INFO date;
		memset(&date, 0, sizeof(DATE_INFO));

		if (m_pDateInfoList[chnn].IsEmpty())
		{
			date.time = GetZeroTime32OfDate(startTime);
			do 
			{
				date.pFileList = new CMyList <unsigned short>;
				assert (NULL != date.pFileList);
				//date.pFileList->AddTail(fileIndex);
				AddToList(fileIndex, *date.pFileList);
				m_pDateInfoList[chnn].AddTail(date);
				date.time += ONE_DAY_SECOND_NUM;
			} while(date.time < endTime);
		}
		else
		{
			POS lastPos = m_pDateInfoList[chnn].GetLastInsertPosition();
			if (NULL == lastPos)
			{
				lastPos = m_pDateInfoList[chnn].GetTailPosition();
			}

			{
				DATE_INFO node = m_pDateInfoList[chnn].GetAt(lastPos);
				POS pos = lastPos;
				if (startTime <= node.time)
				{
					date.time = (endTime <= node.time) ? GetZeroTime32OfDate(endTime) : GetZeroTime32OfDate(node.time);

					while (GetZeroTime32OfDate(startTime) <= date.time)
					{
						if (NULL == pos)
						{
							date.pFileList	= new CMyList <unsigned short>;
							assert (NULL != date.pFileList);
							//date.pFileList->AddTail(fileIndex);
							AddToList(fileIndex, *date.pFileList);
							m_pDateInfoList[chnn].AddHead(date);
						}
						else
						{
							node = m_pDateInfoList[chnn].GetAt(pos);

							if (date.time < node.time)
							{
								m_pDateInfoList[chnn].GetPrev(pos);
								continue;
							}
							else if (date.time == node.time)
							{
								//node.pFileList->AddTail(fileIndex);
								m_pDateInfoList[chnn].GetPrev(pos);
							}
							else
							{
								date.pFileList	= new CMyList <unsigned short>;
								assert (NULL != date.pFileList);
								//date.pFileList->AddTail(fileIndex);
								AddToList(fileIndex, *date.pFileList);
								m_pDateInfoList[chnn].InsertAfter(pos, date);
							}
						}

						date.time -= ONE_DAY_SECOND_NUM;
					}
				}
			}

			{
				DATE_INFO node = m_pDateInfoList[chnn].GetAt(lastPos);
				POS pos = lastPos;
				if (node.time < endTime)
				{
					date.time = (node.time < startTime) ? GetZeroTime32OfDate(startTime) : (ONE_DAY_SECOND_NUM+GetZeroTime32OfDate(node.time));

					while (date.time < endTime)
					{
						if (NULL == pos)
						{
							date.pFileList	= new CMyList <unsigned short>;
							assert (NULL != date.pFileList);
							//date.pFileList->AddTail(fileIndex);
							AddToList(fileIndex, *date.pFileList);
							m_pDateInfoList[chnn].AddTail(date);
						}
						else
						{
							node = m_pDateInfoList[chnn].GetAt(pos);

							if (date.time < node.time)
							{
								date.pFileList	= new CMyList <unsigned short>;
								assert (NULL != date.pFileList);
								//date.pFileList->AddTail(fileIndex);
								AddToList(fileIndex, *date.pFileList);
								m_pDateInfoList[chnn].InsertBefore(pos, date);
							}
							else if (date.time == node.time)
							{
								//node.pFileList->AddTail(fileIndex);
								m_pDateInfoList[chnn].GetNext(pos);
							}
							else
							{
								m_pDateInfoList[chnn].GetNext(pos);
								continue;
							}
						}

						date.time += ONE_DAY_SECOND_NUM;
					}
				}
			}
		}
	}
#endif
}

//ע�⣺Ҫ���ǵ����ظ��ļ��������С���������У�������ļ����Ͳ����ڼ����ˡ�
void CReclogEx::AddToList(unsigned short index, CMyList <unsigned short> & list)
{
	/*
	printf("%s,%d,begain: on disk :%d, index:%d\n",__FILE__,__LINE__,m_diskIndex, index);
	{
		POS pos = list.GetHeadPosition();
		printf("              list fileindex:\n");
		while (NULL != pos)
		{
			unsigned short fileIndex = list.GetNext(pos);
			printf("                         :%d\n", fileIndex);
		}
	}
*/
	if (list.IsEmpty())
	{
		list.AddTail(index);
	}
	else if (index < list.GetHead())
	{
		list.AddHead(index);
	}
	else if (index > list.GetTail())
	{
		list.AddTail(index);
	}
	else
	{
		bool bInsert = true;
		POS headPos = list.GetHeadPosition();
		POS endPos = list.GetTailPosition();
		POS pos = NULL;
		long h=0, t=0, i=0, m=0, num = 0;
		t = list.GetCount()-1;
		while (h <= t)
		{
			pos = headPos;
			m = ((h+t)>>1);

			i = 0;
			num = m - h;
			while (i < num)
			{
				list.GetNext(pos);
				++ i;
			}
			assert (NULL != pos);

			unsigned short nodeIndex = list.GetAt(pos);
			if (nodeIndex < index)
			{
				h = m + 1;	
				if (h > t)
				{
					break;
				}
				else
				{
					list.GetNext(pos);
					if (NULL == pos)
					{
						pos = endPos;
						break;
					}
					headPos = pos;
				}
			}
			else if (nodeIndex == index)
			{
				bInsert = false;
				break;
				//assert (index != nodeIndex);
			}
			else
			{
				t = m - 1;
				list.GetPrev(pos);
				if (NULL == pos)
				{
					pos = headPos;
					break;
				}
			}
		}

		if (bInsert)
		{
			list.InsertAfter(pos, index);
		}		
	}

	/*
	printf("%s,%d,   end: on disk :%d, index:%d\n",__FILE__,__LINE__,m_diskIndex, index);
	{
		POS pos = list.GetHeadPosition();
		printf("              list fileindex:\n");
		while (NULL != pos)
		{
			unsigned short fileIndex = list.GetNext(pos);
			printf("                         :%d\n", fileIndex);
		}
	}
	*/
}
/*
��fileInfo�м�¼���ļ���ĳ�����ڽڵ���б��и�ɾ����
*/
void CReclogEx::PutOutDateList(const FILE_INFO_EX & fileInfo)
{
	unsigned long chnn = CHANNEL_ID(fileInfo.chnn);
	assert (chnn < m_headInfo.chnnNum);
	assert (NULL != m_pDateInfoList);

	if (NULL != m_pDateInfoList)
	{
		if (m_pDateInfoList[chnn].GetCount() > 0)
		{
			POS pos = m_pDateInfoList[chnn].GetHeadPosition();
			while (NULL != pos)
			{
				DATE_INFO & node = m_pDateInfoList[chnn].GetAt(pos);

				if (!((fileInfo.endTime <= node.time) || ((node.time+ONE_DAY_SECOND_NUM) <= fileInfo.startTime)))
				{
					assert (NULL != node.pFileList);
					POS p = node.pFileList->GetHeadPosition();
					while (NULL != p)
					{
						if (fileInfo.fileIndex == node.pFileList->GetAt(p))
						{
							node.pFileList->RemoveAt(p);
							break;
						}
						else
						{
							node.pFileList->GetNext(p);
						}
					}

					//����Ӧ�����ڽڵ����е��ļ��б����Ƿ����ļ����ڣ���������ڣ�����Ҫ�Ѹ����ڽڵ��ɾ����
					if (node.pFileList->IsEmpty())
					{
						POS prevPos = pos;
						m_pDateInfoList[chnn].GetNext(pos);

						//����ɾ�������⡣��Ϊ node.pFileList��������õ���new CMyList <unsigned short>
						//�����ͷ�ʱ��Ҫ����delete node.pFileList;
						//delete [] node.pFileList;
						delete node.pFileList;

						node.pFileList = NULL;
						m_pDateInfoList[chnn].RemoveAt(prevPos);
					}
					else
					{
						m_pDateInfoList[chnn].GetNext(pos);
					}					
				}
				else if (fileInfo.endTime <= node.time)
				{
					break;
				}
				else
				{
					m_pDateInfoList[chnn].GetNext(pos);
				}
			}
		}
	}
}

void CReclogEx::AddToInvalidFileList(CMyList <unsigned short> & list, unsigned short fileIndex)
{
	if (list.IsEmpty())
	{
		list.AddTail(fileIndex);
	}
	else
	{
		if (fileIndex < list.GetHead())
		{
			list.AddHead(fileIndex);
		}
		else if (list.GetTail() < fileIndex)
		{
			list.AddTail(fileIndex);
		}
		else
		{
			POS headPos = list.GetHeadPosition();
			POS pos = NULL;
			long h=0, t=0, i=0, m=0, num = 0;
			t = list.GetCount()-1;
			while (h <= t)
			{
				pos = headPos;
				m = ((h+t)>>1);

				if (h == m)
				{
					break;
				}

				i = 0;
				num = m - h;
				while (i < num)
				{
					list.GetNext(pos);
					++ i;
				}
				assert (NULL != pos);

				unsigned short &node = list.GetAt(pos);
				if (node < fileIndex)
				{
					headPos = pos;
					h = m;
				}
				else
				{
					t = m;
				}
			}

			list.InsertAfter(pos, fileIndex);
		}
	}
}

bool CReclogEx::IsFileBeUsingForRecord(unsigned long channel, unsigned short fileIndex)
{
	assert(NULL != m_pChannelRecordFile);
	assert(channel < m_headInfo.chnnNum);

	bool bRet = false;
	if (0 == m_pChannelRecordFile[channel].pFileList->GetCount())
	{
		return bRet;
	}

	//�������������ϣ�ͨ��channel����¼����ô���ͨ��ͬһʱ������������ϣ�ֻ�ܰ�����¼��һ���ļ���
	assert(1 == m_pChannelRecordFile[channel].pFileList->GetCount());
	if (fileIndex == m_pChannelRecordFile[channel].pFileList->GetHead())
	{
		bRet = true;
	}

	return bRet;
}

bool CReclogEx::IsFileBeUsingForRead(unsigned long channel, unsigned short fileIndex)
{
	assert(NULL != m_pChannelReadFile);
	assert(channel < m_headInfo.chnnNum);

	bool bRet = false;
	if (0 == m_pChannelReadFile[channel].pFileList->GetCount())
	{
		return bRet;
	}

	POS pos = m_pChannelReadFile[channel].pFileList->GetHeadPosition();
	while (NULL != pos)
	{
		if (fileIndex == m_pChannelReadFile[channel].pFileList->GetAt(pos))
		{
			bRet = true;
			break;
		}
		m_pChannelReadFile[channel].pFileList->GetNext(pos);
	}

	return bRet;
}


//////////////////////////////////////////////////////////////////////////
bool CReclogEx::GetOneFileAllInfo(unsigned short fileIndex, unsigned char *pFileHeadInfo, 
								  unsigned char *pFileSectionInfo, unsigned char *pFileReclogInfo)
{    
	assert (fileIndex < m_headInfo.fileNum);

	unsigned long offset = m_headInfo.fileInfoOffset + fileIndex*sizeof(FILE_FILE_INFO);	
	if (0 != fseek(m_pFile, offset, SEEK_SET))
	{
		return false;
	}	
	if (1 != fread(pFileHeadInfo, sizeof(FILE_FILE_INFO), 1, m_pFile))
	{		
		return false;
	}


	offset = m_headInfo.sectionOffset + fileIndex*m_headInfo.fileRecSecNum*sizeof(SECTION_INFO_EX);
	if (0 != fseek(m_pFile, offset, SEEK_SET))
	{
		return false;
	}	
	if (m_headInfo.fileRecSecNum != fread(pFileSectionInfo, sizeof(SECTION_INFO_EX), m_headInfo.fileRecSecNum, m_pFile))
	{
		return false;
	}


	offset = m_headInfo.reclogoffset + fileIndex*m_headInfo.fileLogNum*sizeof(FILE_RECLOG_INFO);
	if (0 != fseek(m_pFile, offset, SEEK_SET))
	{
		return false;
	}
	if (m_headInfo.fileLogNum != fread(pFileReclogInfo, sizeof(FILE_RECLOG_INFO), m_headInfo.fileLogNum, m_pFile))
	{
		return false;
	}

	return true;

}  

//////////////////////////////////////////////////////////////////////////////////////
bool CReclogEx::OpenLogFileForBackup(char *pFilePath)
{

	assert (NULL == m_pFilePathForBackup);	//�����ظ���ʼ��
	m_pFilePathForBackup = new char [512];
	assert (NULL != m_pFilePathForBackup);
	if (NULL == m_pFilePathForBackup)
	{
		return false;
	}
	memset(m_pFilePathForBackup, 0, 512);
	if (NULL != pFilePath)
	{
		strcpy(m_pFilePathForBackup, pFilePath);
	}


	//�����ļ�����·���������Դ��ļ���	
	//char filePath [512] = {0};
	//sprintf(filePath, "%s/reclog.bin", m_pFilePathForBackup);
	assert (NULL == m_pFileForBackup);
	if (NULL != m_pFileForBackup)
	{
		fclose(m_pFileForBackup);
	}	
	m_pFileForBackup = fopen(m_pFilePathForBackup, "rb+");
	if (NULL == m_pFileForBackup)
	{
		Release(NULL, 1, &m_pFilePathForBackup);
		return false;
	}
	
	//ȷ��reclog�ļ��Ƿ�����ˣ��������ļ���ͨ����ĿΪ������
	if (1 != fread(&m_headInfoForBackup, sizeof(RECLOG_FILE_HEAD_INFO), 1, m_pFileForBackup))
	{
		Release(&m_pFileForBackup, 1, &m_pFilePathForBackup);
		return false;
	}		
	if ((0 >= m_headInfoForBackup.fileNum) || (0 >= m_headInfoForBackup.chnnNum))
	{
		Release(&m_pFileForBackup, 1, &m_pFilePathForBackup);
		return false;
	}

	//¼��״̬
	//m_pRecStatusForBackup = new REC_STATUS [m_headInfoForBackup.chnnNum];
	//assert (NULL != m_pRecStatusForBackup);
	//if (NULL == m_pRecStatusForBackup)
	//{
	//	Release(&m_pFileForBackup, 1, &m_pFilePathForBackup);
	//	return false;
	//}
	memset(&m_recStatusForBackup, 0, sizeof(REC_STATUS));

	return true;

}


void CReclogEx::CloseLogFileForBackup()
{
	Release(&m_pFileForBackup, 1, &m_pFilePathForBackup);

}

bool CReclogEx::WriteFileStartForBackup(unsigned short fileIndex, unsigned long chnn)
{
	assert (NULL != m_pFileForBackup);
	if (NULL == m_pFileForBackup)
	{
		return false;
	}

	memset(&m_recStatusForBackup, 0, sizeof(REC_STATUS));

	memset(&m_fileInfoForBackup, 0, sizeof(FILE_INFO_EX));
	m_fileInfoForBackup.chnn = chnn;
	m_fileInfoForBackup.fileIndex = fileIndex;

	return true;

}

bool CReclogEx::WriteFileEndForBackup()
{
	assert (NULL != m_pFileForBackup);
	if (NULL == m_pFileForBackup)
	{
		return false;
	}

	memset(&m_recStatusForBackup, 0, sizeof(REC_STATUS));

	memset(&m_fileInfoForBackup, 0, sizeof(FILE_INFO_EX));
	return true;
}

bool CReclogEx::WriteReclogStartForBackup(unsigned short fileIndex, unsigned long chnn, RECORD_TYPE type, unsigned long startTime)
{

	assert (NULL != m_pFileForBackup);
	if (NULL == m_pFileForBackup)
	{
		return false;
	}

	
	//FILE_INFO_EX &fileInfo = m_pFileInfoList[CHANNEL_ID(chnn)].GetTail();

	assert (fileIndex == m_fileInfoForBackup.fileIndex);
	assert (chnn == m_fileInfoForBackup.chnn);
	assert (!(type & m_recStatusForBackup.type));
	assert (startTime >= m_fileInfoForBackup.startTime);

	
	//��ʼʱ��������ʶ��һ�ζԸ��ļ�д�¼�����Ҫ���뵽�����б���ȥ��
	if (0 == m_fileInfoForBackup.startTime)
	{
		memset(&m_recStatusForBackup, 0, sizeof(REC_STATUS));
	}

	//�����µ�һ�Σ���Ҫ��Ӷμ�¼
	if (RECORD_TYPE_NULL == m_recStatusForBackup.type)
	{
		if (!WriteSectionForBackup(fileIndex, chnn, startTime, true))
		{
			return false;
		}
	}
	
	if (!WriteTypeForBackup(fileIndex, chnn, type, startTime, true))
	{
		return false;
	}

	m_recStatusForBackup.type |= type;
	

	if ((0 == m_fileInfoForBackup.startTime) || !(m_fileInfoForBackup.type & type))
	{
		//����Ѿ����˿�ʼʱ�䣬��Ӧ�ø�д����
		m_fileInfoForBackup.startTime = (0 == m_fileInfoForBackup.startTime) ? startTime : m_fileInfoForBackup.startTime;
		m_fileInfoForBackup.type |= type;
		
		FILE_FILE_INFO info;
		info.chnn		= m_fileInfoForBackup.chnn;
		info.type		= m_fileInfoForBackup.type;
		info.startTime	= m_fileInfoForBackup.startTime;
		info.endTime	= m_fileInfoForBackup.endTime;
		info.status		= FILE_STATUS_WRITING;

		//��һ�ζԸ��ļ�д���¼���������д��һ���µ����ͣ�����Ҫ�����ļ���Ϣ��
		if (!WriteFileInfoForBackup(fileIndex, &info))
		{
			return false;
		}
		
	}
	

	return true;
}

bool CReclogEx::WriteReclogEndForBackup(unsigned short fileIndex, unsigned long chnn, RECORD_TYPE type, unsigned long endTime)
{
	assert (NULL != m_pFileForBackup);
	if (NULL == m_pFileForBackup)
	{
		return false;
	}

	//FILE_INFO_EX &fileInfo = m_pFileInfoList[CHANNEL_ID(chnn)].GetTail();

	assert (fileIndex == m_fileInfoForBackup.fileIndex);
	assert (chnn == m_fileInfoForBackup.chnn);
	assert (type & m_recStatusForBackup.type);
	assert (endTime >= m_fileInfoForBackup.startTime);

	
	if (!WriteTypeForBackup(fileIndex, chnn, type, endTime, false))
	{
		return false;
	}
	
	m_recStatusForBackup.type &= ~type;
	
	if (endTime > m_fileInfoForBackup.endTime)
	{
		m_fileInfoForBackup.endTime = endTime;
	}
		
	if (RECORD_TYPE_NULL == m_recStatusForBackup.type)
	{
		if (!WriteSectionForBackup(fileIndex, chnn, endTime, false))
		{
			return false;
		}
		
		FILE_FILE_INFO info;
		info.chnn		= m_fileInfoForBackup.chnn;
		info.type		= m_fileInfoForBackup.type;
		info.startTime	= m_fileInfoForBackup.startTime;
		info.endTime	= m_fileInfoForBackup.endTime;
		info.status		= FILE_STATUS_END;

		//���д�¼���������ʱֹͣ���������͵�¼��������ļ���Ϣ������ʱ�䣩��
		if (!WriteFileInfoForBackup(fileIndex, &info))
		{
			return false;
		}
	}

	return true;

}


bool CReclogEx::WriteTypeForBackup(unsigned long fileIndex, unsigned long chnn, RECORD_TYPE type, unsigned long time, bool bStart)
{
	//Ԥ���ռ��Ѿ�д�����򷵻�ʧ��
	if (m_headInfoForBackup.fileLogNum <= m_recStatusForBackup.reclog_index)
	{
		return false;
	}

	FILE_RECLOG_INFO reclog_info;
	unsigned long offset = m_headInfoForBackup.reclogoffset
		                 + fileIndex*m_headInfoForBackup.fileLogNum*sizeof(FILE_RECLOG_INFO)
						 + m_recStatusForBackup.reclog_index*sizeof(FILE_RECLOG_INFO);
	//�����¼���¼
	reclog_info.type = type;
	if (!bStart)	//����������
	{
		reclog_info.type |= RECLOG_END;
	}
	reclog_info.time = time;
	


	if (0 == fseek(m_pFileForBackup, offset, SEEK_SET))
	{
		if (1 == fwrite(&reclog_info, sizeof(FILE_RECLOG_INFO), 1, m_pFileForBackup))
		{
			++m_recStatusForBackup.reclog_index;
			return true;
		}
	}

	return false;

}


bool CReclogEx::WriteSectionForBackup(unsigned long fileIndex, unsigned long chnn, unsigned long time, bool bStart)
{
	//Ԥ���ռ��Ѿ�д�����򷵻�ʧ��
	if (m_headInfoForBackup.fileRecSecNum <= m_recStatusForBackup.section_index)
	{
		return false;
	}
	
	unsigned long offset = m_headInfoForBackup.sectionOffset
		                 + fileIndex*m_headInfoForBackup.fileRecSecNum*sizeof(SECTION_INFO_EX)
						 + m_recStatusForBackup.section_index*sizeof(SECTION_INFO_EX);

	//��ʼʱ��дǰ���ĸ��ֽڣ�����ʱ��д�����ĸ��ֽڡ�
	offset += (bStart ? 0 : sizeof(unsigned long));

	if (0 == fseek(m_pFileForBackup, offset, SEEK_SET))
	{
		if (1 == fwrite(&time, sizeof(unsigned long), 1, m_pFileForBackup))
		{
			if (!bStart)
			{
				//����ʱ��ָ����һ����¼��������
				++ m_recStatusForBackup.section_index;
			}

			return true;
		}
	}

	return false;


}







bool CReclogEx::WriteFileInfoForBackup(unsigned long fileIndex, FILE_FILE_INFO *pFileInfo)
{
	if (0 == fseek(m_pFileForBackup, m_headInfoForBackup.fileInfoOffset+fileIndex*sizeof(FILE_FILE_INFO), SEEK_SET))
	{
		if (1 == fwrite(pFileInfo, sizeof(FILE_FILE_INFO), 1, m_pFileForBackup))
		{
			return true;
		}
	}

	return false;


}


bool CReclogEx::WriteOneFileAllInfo(unsigned short fileIndex, unsigned char *pFileHeadInfo, 
									unsigned char *pFileSectionInfo, unsigned char *pFileReclogInfo)
{
	assert (fileIndex < m_headInfoForBackup.fileNum);

	unsigned long offset = m_headInfoForBackup.fileInfoOffset + fileIndex*sizeof(FILE_FILE_INFO);	
	if (0 != fseek(m_pFileForBackup, offset, SEEK_SET))
	{
		return false;
	}	
	if (1 != fwrite(pFileHeadInfo, sizeof(FILE_FILE_INFO), 1, m_pFileForBackup))
	{		
		return false;
	}


	offset = m_headInfoForBackup.sectionOffset + fileIndex*m_headInfoForBackup.fileRecSecNum*sizeof(SECTION_INFO_EX);
	if (0 != fseek(m_pFileForBackup, offset, SEEK_SET))
	{
		return false;
	}	
	if (m_headInfoForBackup.fileRecSecNum != fwrite(pFileSectionInfo, sizeof(SECTION_INFO_EX), m_headInfoForBackup.fileRecSecNum, m_pFileForBackup))
	{
		return false;
	}


	offset = m_headInfoForBackup.reclogoffset + fileIndex*m_headInfoForBackup.fileLogNum*sizeof(FILE_RECLOG_INFO);
	if (0 != fseek(m_pFileForBackup, offset, SEEK_SET))
	{
		return false;
	}
	if (m_headInfoForBackup.fileLogNum != fwrite(pFileReclogInfo, sizeof(FILE_RECLOG_INFO), m_headInfoForBackup.fileLogNum, m_pFileForBackup))
	{
		return false;
	}

	return true;

}

bool CReclogEx::RepairForBackup(unsigned char *pFileHeadInfo, unsigned char *pFileSectionInfo, 
								unsigned char *pFileReclogInfo, FILE_INFO_EX fileInfo)
{

	bool bNeedRepair = false;
	//�޸�����Ϣ
	{
		SECTION_INFO_EX *pSection = (SECTION_INFO_EX *)pFileSectionInfo;
		for(int secIndex = 0; secIndex < m_headInfoForBackup.fileRecSecNum; secIndex++)
		{
			if((0 == pSection[secIndex].startTime) || (pSection[secIndex].startTime < fileInfo.startTime))
			{
				break;
			}
			else if(0 == pSection[secIndex].endTime)
			{
				pSection[secIndex].endTime = fileInfo.endTime;
				bNeedRepair = true;
				break;
			}
		}		
	}

	//�޸��¼���Ϣ
	if (bNeedRepair)
	{
		FILE_RECLOG_INFO *pReclog = (FILE_RECLOG_INFO *)pFileReclogInfo;
		unsigned long reclogIndex = 0;

		//���¼���¼�����
		unsigned char recTypeNum = RecTypeNum();
		assert (recTypeNum > 0);
		FILE_RECLOG_INFO  *pTempReclog = new FILE_RECLOG_INFO[recTypeNum];
		memset(pTempReclog, 0, sizeof(FILE_RECLOG_INFO)*recTypeNum);

		for (unsigned long i=0; i<m_headInfoForBackup.fileLogNum; ++i)
		{
			//��ʾ��ʼ
			if ((RECORD_TYPE_NULL != pReclog[i].type) && (pReclog[i].type < RECORD_TYPE_END) && (0 == (pReclog[i].type & RECLOG_END)))
			{
				pTempReclog [RecTypeIndex(static_cast<RECORD_TYPE>(pReclog[i].type))] = pReclog[i];
			}

			//��ʾ��������ʱ�䲻Ϊ�㡣
			if ((RECORD_TYPE_NULL != pReclog[i].type) && ((pReclog[i].type - RECLOG_END) < RECORD_TYPE_END) &&\
				(0 < pReclog[i].time) && (pReclog[i].type & RECLOG_END))
			{
				memset(&pTempReclog [RecTypeIndex(static_cast<RECORD_TYPE>(pReclog[i].type))], 0, sizeof(FILE_RECLOG_INFO));
			}

			//��ʾ��Ԥ���ռ�δʹ��
			if ((RECORD_TYPE_NULL == pReclog[i].type) && (0 == pReclog[i].time))
			{
				break;
			}

			++ reclogIndex;
		}

		for (unsigned char i=0; i<recTypeNum; ++i)
		{
			if ((RECORD_TYPE_NULL != pTempReclog[i].type) && (0 < pTempReclog[i].time))
			{
				//����Խ��
				if (reclogIndex < m_headInfoForBackup.fileLogNum)
				{
					pReclog[reclogIndex].type = (pTempReclog[i].type | RECLOG_END);
					pReclog[reclogIndex].time = fileInfo.endTime;

					++ reclogIndex;
				}
				else
				{
					break;
				}
			}
		}	

		delete [] pTempReclog;
		pTempReclog = NULL;
	}

	//�޸��ļ���Ϣ
	FILE_FILE_INFO *pInfo = (FILE_FILE_INFO *)pFileHeadInfo;
	if(bNeedRepair)
	{		
		pInfo->endTime = fileInfo.endTime;		
	}
	pInfo->status = FILE_STATUS_END;
	return true;	
}


////////////////////////////////////////////////////////////////////////////////////////////////
unsigned long CReclogEx::GetFreeFileNum()
{
	return m_invalidFileList.GetCount();
}

unsigned long CReclogEx::GetChannelNum(bool bIsBackUpCreated/* = false*/)
{
	if(!bIsBackUpCreated)
	{
		return m_headInfo.chnnNum;
	}
	else
	{
		unsigned char netVideoInputNum = CProduct::Instance()->NetVideoInputNum();
		unsigned char localVideoInputNum = CProduct::Instance()->LocalVideoInputNum();
		return (localVideoInputNum+netVideoInputNum);
	}
	
}

bool CReclogEx::IsBeWoring()
{
	assert(m_pChannelRecordFile != NULL);
	assert(m_pChannelReadFile != NULL);
    
	bool bRet = false;
	for (int chnn = 0; chnn < m_headInfo.chnnNum; ++chnn)
	{
		if (m_pChannelRecordFile[chnn].pFileList->GetCount() > 0)
		{
			bRet = true;
			break;
		}
		if (m_pChannelReadFile[chnn].pFileList->GetCount() > 0)
		{
			bRet = true;
			break;
		}
	}
	
	return bRet;
}

bool CReclogEx::IsBeReading()
{
	assert(m_pChannelReadFile != NULL);

	bool bRet = false;
	for (int chnn = 0; chnn < m_headInfo.chnnNum; ++chnn)
	{		
		if (m_pChannelReadFile[chnn].pFileList->GetCount() > 0)
		{
			bRet = true;
			break;
		}
	}

	return bRet;
}










/////////////////////////////////////////////////////////////////////////////////////////////
