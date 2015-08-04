#include "BackupReader.h"
#include "LocalDevice.h"
CBackupReader::CBackupReader()
{
	m_pRecReader = NULL;
	m_pSrcFilePathName = NULL;
    
	m_bOpenFile = false;
	m_channelNum = 0;
	m_bMustBackupAudioFormat = false;
}

CBackupReader::~CBackupReader()
{
	CloseFile();
	if (NULL != m_pRecReader)
	{
		delete m_pRecReader;
		m_pRecReader = NULL;
	}
	if (NULL != m_pSrcFilePathName)
	{
		delete [] m_pSrcFilePathName;
		m_pSrcFilePathName = NULL;
	}

	m_channelNum = 0;
	m_bMustBackupAudioFormat = false;
}



int CBackupReader::Start(FILE_INFO_EX_BACKUP *pFileInfoBackup, unsigned long fileNum, char *pSrcFilePath, 
						 BACKUP_DATA_TYPE dataType,CReclogManEX *pReclogMan,bool bMustAudioFormat)
{
	//start成功后，一定要stop
	assert(NULL == m_pRecReader);
	assert(NULL == m_pSrcFilePathName);
    assert(fileNum > 0);
	assert(pFileInfoBackup != NULL);

	m_bMustBackupAudioFormat = bMustAudioFormat;

#if defined(__PRIMARY_WRDISK__)
	m_pRecReader = new CPrimaryRecReader;
#else
	m_pRecReader = new CRecReader;
#endif
    m_bOpenFile = false;
	if (NULL == m_pRecReader)
	{
		return -1;
	}

	m_pSrcFilePathName = new char [255];
	memset(m_pSrcFilePathName, 0, 255);
	if (NULL == m_pSrcFilePathName)
	{
		delete m_pRecReader;
		m_pRecReader = NULL;
		return -1;
	}

	m_pFileInfoBackup = pFileInfoBackup;
	m_fileNum   = fileNum;
	m_pSrcFilePath = pSrcFilePath;
	//m_startTime    = (LONGLONG)startTime * 1000000;
	//m_endTime      = (LONGLONG)endTime * 1000000;
	m_dataType     = dataType;
	m_pReclogMan   = pReclogMan;
	
	m_channelNum = m_pReclogMan->GetDiskChannelNum(pFileInfoBackup[0].fileInfo.diskIndex,true);


	//计算要备份的总时间。单位：秒
	unsigned long startTmp = 0;
	unsigned long endTmp = 0;
	m_allTime = 0;
	for (unsigned long i = 0; i < fileNum; ++i)
	{
		startTmp = m_pFileInfoBackup[i].fileInfo.startTime;
		endTmp = m_pFileInfoBackup[i].fileInfo.endTime;
		
		assert(endTmp >= startTmp);
		m_allTime += endTmp - startTmp;
	}

	m_offSetTime             = 0;
	m_oneFileBaseTime        = 0;
	m_curOffSetTimeofOneFile = 0;


	m_status       = 0;

	return 0;

}

//
int CBackupReader::GetFrame(FRAME_INFO_EX &frameInfo)
{
	if (NULL == m_pRecReader)
	{
		return BACKUP_READER_ERROR_NO_INIT;
	}


	if (0 == m_status)
	{
		//初始化
		frameInfo.frameType = FRAME_TYPE_FILE_HEAD;
		frameInfo.length = m_fileNum;
		frameInfo.channel = m_channelNum;
		m_fileIndex = 0;
		m_status = 1;

		m_offSetTime = 0;
		m_oneFileBaseTime = 0;
		m_curOffSetTimeofOneFile = 0;

		return BACKUP_READER_ERROR_SUC;
	}
	else if (1 == m_status)
	{
		//打开文件
ll:		if (m_fileIndex >= m_fileNum)
		{
			return BACKUP_READER_ERROR_READ_FINISH;
		}

		if (0 != OpenFile(m_fileIndex))
		{
			m_fileIndex++;
			goto ll;
		}

		frameInfo.frameType = FRAME_TYPE_FILE_INFO;
		frameInfo.length = sizeof(FILE_FILE_INFO)+MAX_SECTION_NUM*sizeof(SECTION_INFO_EX)+MAX_RECLOG_NUM*sizeof(FILE_RECLOG_INFO);
		if (!m_pReclogMan->GetOneFileAllInfo(m_pFileInfoBackup[m_fileIndex].fileInfo.diskIndex,m_pFileInfoBackup[m_fileIndex].fileInfo.fileIndex,
			                            frameInfo.pData, frameInfo.pData+sizeof(FILE_FILE_INFO),
										frameInfo.pData+sizeof(FILE_FILE_INFO)+MAX_SECTION_NUM*sizeof(SECTION_INFO_EX)))
		{
			CloseFile();
			m_fileIndex++;
			goto ll;
		}

		m_status = 2;

		m_oneFileBaseTime = 0;
		m_curOffSetTimeofOneFile = 0;


		return BACKUP_READER_ERROR_SUC;

	}
	else if (2 == m_status)
	{
		//printf("%s,%d\n",__FILE__,__LINE__);

lnext:	if (0 == GetOneFrame(frameInfo))
		{
			//printf("%s,%d\n",__FILE__,__LINE__);
			//PrintfBit64(frameInfo.time, false);
			if (0 == m_oneFileBaseTime)
			{
				if (FRAME_TYPE_VIDEO == frameInfo.frameType)
				{
					m_oneFileBaseTime = (unsigned long)(frameInfo.time/1000000);
				}
			}
			else
			{
				if (FRAME_TYPE_VIDEO == frameInfo.frameType)
				{
					m_curOffSetTimeofOneFile = (unsigned long)(frameInfo.time/1000000) - m_oneFileBaseTime;					
				}
			}

			if (FRAME_TYPE_END == frameInfo.frameType)
			{
				CloseFile();
				m_fileIndex++;
				m_status = 1;	

				m_offSetTime += m_curOffSetTimeofOneFile;
			}

			
			if (0 == m_pFileInfoBackup[m_fileIndex].hasBackupAudio)
			{
				if (FRAME_TYPE_AUDIO == frameInfo.frameType)
				{
					goto lnext;
				}
			}
			return BACKUP_READER_ERROR_SUC;			
		}
		else
		{
			//printf("%s,%d\n",__FILE__,__LINE__);
			CloseFile();
			return BACKUP_READER_ERROR_READ_FAILD;
		}
	}

	return BACKUP_READER_ERROR_NO_INIT;

}

//
void CBackupReader::Stop()
{
	CloseFile();
	if (NULL != m_pRecReader)
	{
		delete m_pRecReader;
		m_pRecReader = NULL;
	}
	if (NULL != m_pSrcFilePathName)
	{
		delete [] m_pSrcFilePathName;
		m_pSrcFilePathName = NULL;
	}

	m_channelNum = 0;

}




//
int CBackupReader::OpenFile(unsigned long fileIndex)
{

	assert(!m_bOpenFile);
	m_bReVideoFmat = true;
	m_bReAudioFmat = false;
	m_startTime = 0;
	m_endTime = 0;
#ifdef __ENVIRONMENT_WIN32__
	if (strlen(m_pSrcFilePath) == 1)
	{
		sprintf(m_pSrcFilePathName, "%c:\\%08d.dat", m_pSrcFilePath[0] + m_pFileInfoBackup[fileIndex].fileInfo.diskIndex, m_pFileInfoBackup[fileIndex].fileInfo.fileIndex);
	}
	else
	{
		sprintf(m_pSrcFilePathName, "%s\\%02d\\%08d.dat", m_pSrcFilePath, m_pFileInfoBackup[fileIndex].fileInfo.diskIndex, m_pFileInfoBackup[fileIndex].fileInfo.fileIndex);
	}
#else
	sprintf(m_pSrcFilePathName, "%s/%02d/%08d.dat", m_pSrcFilePath, m_pFileInfoBackup[fileIndex].fileInfo.diskIndex, m_pFileInfoBackup[fileIndex].fileInfo.fileIndex);
#endif
	//printf("ddd %s\n", m_pSrcFilePathName);
	if (!m_pRecReader->Open(m_pSrcFilePathName,FILE_BUF_TYPE_LOCAL, true))
	{
		return -1;
	}

	if(m_pRecReader->GetFrameNum(FRAME_TYPE_VIDEO) <= 0)
	{
		m_pRecReader->Close();
		return -1;
	}	
	if (ERR_READ_FILE_OK != m_pRecReader->GetVideoInfo(&m_bitmapInfo))
	{
		m_pRecReader->Close();
		return -1;
	}
	m_bReVideoFmat = true;

	
	if(m_pRecReader->GetFrameNum(FRAME_TYPE_AUDIO) > 0)
	{
		if (ERR_READ_FILE_OK == m_pRecReader->GetAudioInfo(&m_waveFormat))
		{
			m_bReAudioFmat = true;
		}
	}

	if (m_bMustBackupAudioFormat)
	{
		if (!m_bReAudioFmat)
		{
			CLocalDevice::Instance()->GetAudioFormat(m_waveFormat);
			m_bReAudioFmat = true;
		}
	}

	//应该用m_startTime	
	//if (ERR_READ_FILE_OK != m_pRecReader->SeekVideoFrame(SEEK_FIRST_KEY))
	m_startTime    = (LONGLONG)(m_pFileInfoBackup[m_fileIndex].fileInfo.startTime) * 1000000;
	m_endTime      = (LONGLONG)(m_pFileInfoBackup[m_fileIndex].fileInfo.endTime) * 1000000;
    unsigned long ret = m_pRecReader->SeekKeyFrameToTime(m_startTime);
	if ((ERR_READ_FILE_OK != ret) 
	  &&(ERR_READ_FILE_SEEK_TO_BEGIN != ret)
	  &&(ERR_READ_FILE_SEEK_TO_END != ret))
	{
		m_pRecReader->Close();
		return -1;
	}
    
	m_bOpenFile = true;
	m_bReadFileEnd = false;	
	return 0;

}

void CBackupReader::CloseFile()
{
	if (NULL != m_pRecReader)
	{
		if (m_bOpenFile)
		{
			m_pRecReader->Close();
			m_bOpenFile = false;
		}		
	}
}


int CBackupReader::GetOneFrame(FRAME_INFO_EX &frameInfo)
{
	if (m_bReadFileEnd)
	{
		frameInfo.frameType = FRAME_TYPE_END;
		return 0;
	}

	//新打开文件，需要将视频和音频格式送出去
	if (m_bReVideoFmat)  //视频格式
	{
		frameInfo.frameType = FRAME_TYPE_VIDEO_FORMAT;
		frameInfo.length = sizeof(BITMAPINFOHEADER);
		memcpy(frameInfo.pData, &m_bitmapInfo, frameInfo.length);		
		m_bReVideoFmat = false;		
		return 0;
	}

	if (m_bReAudioFmat)//音频格式
	{
		frameInfo.frameType = FRAME_TYPE_AUDIO_FORMAT;
		frameInfo.length = sizeof(WAVEFORMATEX);
		memcpy(frameInfo.pData, &m_waveFormat, frameInfo.length);
		m_bReAudioFmat = false;
		return 0;
	}

	if (ERR_READ_FILE_OK != m_pRecReader->GetCurrFrame(frameInfo))
	{
		return -1;
	}

   	if (frameInfo.time > m_endTime)
	{
		m_bReadFileEnd = true;
		frameInfo.frameType = FRAME_TYPE_END;
		return 0;
	}

	if (ERR_READ_FILE_OK != m_pRecReader->SeekNextFrame())
	{
		m_bReadFileEnd = true;
	}
	
	frameInfo.width		= m_bitmapInfo.biWidth;
	frameInfo.height	= m_bitmapInfo.biHeight;

	return 0;
}



