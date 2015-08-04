
#include "BackupCutWrite.h"


//
CBackupCutWriter::CBackupCutWriter()
{
	m_pBackupPathName = NULL;
	m_nWidth = 0;
	m_nHeight = 0;
	m_pRecFileEventInfo = NULL;
	m_eventInfoNum = 0;
}

//
CBackupCutWriter::~CBackupCutWriter()
{
	if (NULL != m_pBackupPathName)
	{
		delete [] m_pBackupPathName;
		m_pBackupPathName = NULL;
	}

}

//
int CBackupCutWriter::Start(char *pBackupPath, REC_FILE_EVENT_INFO *pRecFileEventInfo, unsigned long num)
{
	assert(m_pBackupPathName == NULL);
	if (NULL == pRecFileEventInfo)
	{
		return -1;
	}
	m_pRecFileEventInfo = pRecFileEventInfo;
	m_eventInfoNum = num;

	printf("%s,%d,m_eventInfoNum:%d\n",__FILE__,__LINE__,m_eventInfoNum);
	m_pBackupPathName = new char [255];

	if (NULL == m_pBackupPathName)
	{
		return -1;
	}



	m_pBackupPath = pBackupPath;

	m_dvrWriter.fileIndex = 0;
	m_dvrWriter.hasOpen = false;

	m_startWriteFrameTime = (LONGLONG)0x4D1E6E80*1000000; //2011-1-1 0:0:0
	PrintfBit64(m_startWriteFrameTime,false);
	
	return 0;
}

//
int CBackupCutWriter::WriteFrame(FRAME_INFO_EX &frameInfo)
{
	return DVRBackup(frameInfo);	
}

int CBackupCutWriter::DVRBackup(FRAME_INFO_EX &frameInfo)
{	
	unsigned long frameType =  frameInfo.frameType;
	unsigned long writeRet = 0;
	switch(frameType) 
	{
	case FRAME_TYPE_FILE_HEAD:	
		m_dvrWriter.fileIndex = 0;		
		m_dvrWriter.hasOpen = false;
		m_dvrWriter.bHasWriteVideoFormat = false;
		m_dvrWriter.bHasWriteAudioFormat = false;
		m_dvrWriter.firstWriteFrameTime  = 0;
		m_dvrWriter.lastWriteFrameTime  = m_startWriteFrameTime - 33000;
		m_dvrWriter.firstBackupFrameTime = 0;
		m_dvrWriter.backupFileIndex = -1;
		break;
	case FRAME_TYPE_FILE_INFO:
		m_dvrWriter.firstBackupFrameTime = 0;
		m_dvrWriter.firstWriteFrameTime = m_dvrWriter.lastWriteFrameTime + 33000;			
		m_dvrWriter.backupFileIndex++;
		if ((m_dvrWriter.backupFileIndex < m_eventInfoNum) && (m_dvrWriter.backupFileIndex >= 0))
		{
			m_pRecFileEventInfo[m_dvrWriter.backupFileIndex].startTime = m_dvrWriter.firstWriteFrameTime;
		}		
		break;
	case FRAME_TYPE_VIDEO_FORMAT:
		if (!m_dvrWriter.hasOpen) 
		{
			CalculatePath(m_pBackupPathName, m_dvrWriter.fileIndex,true);
			if (m_dvrWriter.dvrWriter.Open(m_pBackupPathName, 512, true, true))
			{
				m_dvrWriter.hasOpen =  true;
			}
			else
			{
				return BACKUP_CUT_WRITER_ERROR_OPEN_FAIL;
			}
		}
		if (!m_dvrWriter.bHasWriteVideoFormat)
		{
			if (/*CRecWriter::*/WRITE_FRAME_FAIL == m_dvrWriter.dvrWriter.WriteData(frameInfo))
			{	
				return BACKUP_CUT_WRITER_ERROR_WRITER_FAIL;
			}
			else
			{
				m_dvrWriter.bHasWriteVideoFormat = true;
			}
		}

		if ((m_dvrWriter.backupFileIndex < m_eventInfoNum) && (m_dvrWriter.backupFileIndex >= 0))
		{
			BITMAPINFOHEADER bmphdr;
			memset(&bmphdr, 0, sizeof(bmphdr));
			memcpy(&bmphdr, frameInfo.pData, frameInfo.length);
			m_pRecFileEventInfo[m_dvrWriter.backupFileIndex].width = bmphdr.biWidth;
			m_pRecFileEventInfo[m_dvrWriter.backupFileIndex].height = bmphdr.biHeight;
		}	

		break;
	case FRAME_TYPE_AUDIO_FORMAT:
		if (!m_dvrWriter.bHasWriteAudioFormat)
		{
			if (/*CRecWriter::*/WRITE_FRAME_FAIL == m_dvrWriter.dvrWriter.WriteData(frameInfo))
			{	
				return BACKUP_CUT_WRITER_ERROR_WRITER_FAIL;
			}
			else
			{
				m_dvrWriter.bHasWriteAudioFormat = true;
			}
		}				
		break;
	case FRAME_TYPE_VIDEO:	
		{
			if (0 == m_dvrWriter.firstBackupFrameTime)
			{				
				m_dvrWriter.firstBackupFrameTime = frameInfo.time;
				frameInfo.time = m_dvrWriter.firstWriteFrameTime;
			}
			else
			{
				LONGLONG d = frameInfo.time - m_dvrWriter.firstBackupFrameTime;
				if (d < 0)
				{
					printf("%s,%d,video time error\n",__FILE__,__FILE__);
					PrintfBit64(frameInfo.time,false);
					PrintfBit64(m_dvrWriter.firstBackupFrameTime,false);
					return BACKUP_CUT_WRITER_ERROR_WRITER_FAIL;
				}			
				frameInfo.time = m_dvrWriter.firstWriteFrameTime + d;
			}

			m_dvrWriter.lastWriteFrameTime = frameInfo.time;
			if ((m_dvrWriter.backupFileIndex < m_eventInfoNum) && (m_dvrWriter.backupFileIndex >= 0))
			{
				m_pRecFileEventInfo[m_dvrWriter.backupFileIndex].endTime = m_dvrWriter.lastWriteFrameTime;
			}	
			writeRet = m_dvrWriter.dvrWriter.WriteData(frameInfo);
			if (/*CRecWriter::*/WRITE_FRAME_FULL == writeRet)
			{
				return BACKUP_CUT_WRITER_ERROR_WRITER_FAIL;
			}
			else if (/*CRecWriter::*/WRITE_FRAME_FAIL == writeRet)
			{	
				return BACKUP_CUT_WRITER_ERROR_WRITER_FAIL;
			}

		}		
		break; 
	case FRAME_TYPE_AUDIO:
		{
			LONGLONG d = frameInfo.time - m_dvrWriter.firstBackupFrameTime;
			if (d < 0)
			{
				printf("%s,%d,audio time error\n",__FILE__,__FILE__);
				PrintfBit64(frameInfo.time,false);
				PrintfBit64(m_dvrWriter.firstBackupFrameTime,false);
				frameInfo.time = m_dvrWriter.firstWriteFrameTime + d;
				if (frameInfo.time < m_dvrWriter.firstWriteFrameTime)
				{
					printf("audio time less first writetime\n");
					break;
				}
			}
			else
			{
				frameInfo.time = m_dvrWriter.firstWriteFrameTime + d;
			}
			

			m_dvrWriter.lastWriteFrameTime = frameInfo.time;			
			writeRet = m_dvrWriter.dvrWriter.WriteData(frameInfo);
			if (/*CRecWriter::*/WRITE_FRAME_FULL == writeRet)
			{
				return BACKUP_CUT_WRITER_ERROR_WRITER_FAIL;
			}
			else if (/*CRecWriter::*/WRITE_FRAME_FAIL == writeRet)
			{	
				return BACKUP_CUT_WRITER_ERROR_WRITER_FAIL;
			}
		}
		break; 
	case FRAME_TYPE_END:
		{	
					
		}		
		break;	
	default:
		break;
	}
	return BACKUP_CUT_WRITER_ERROR_SUC;
}




void CBackupCutWriter::CalculatePath(char* pPath, unsigned short fileIndex, bool bFile)
{	
	char szTmp[512] = {0};
	tm time = DVRTime32ToTm(GetCurrTime32());
	sprintf(pPath, "%s/B%d%02d%02d%02d%02d%02d.dat", m_pBackupPath, time.tm_year+1900, time.tm_mon+1, time.tm_mday, time.tm_hour, time.tm_min, time.tm_sec);	
}
	

void CBackupCutWriter::WriteFileEventInfo()
{
	if ((m_dvrWriter.backupFileIndex < m_eventInfoNum) && (m_dvrWriter.backupFileIndex >= 0))
	{
		if ((m_pRecFileEventInfo[m_dvrWriter.backupFileIndex].startTime > 0) && (0 == m_pRecFileEventInfo[m_dvrWriter.backupFileIndex].endTime))
		{
			m_pRecFileEventInfo[m_dvrWriter.backupFileIndex].endTime = m_dvrWriter.lastWriteFrameTime;
		}
		for (int i = 0; i < m_eventInfoNum; ++i)
		{
			printf("WriteFileEventInfo,%s,%d,index:%d,chnn:%d,audio:%d\n",__FILE__,__LINE__,i,m_pRecFileEventInfo[i].chnn,m_pRecFileEventInfo[i].audio);
			printf("                                                     startTime:");
			PrintfBit64(m_pRecFileEventInfo[i].startTime,false);
			printf("                                                     endTime  :");
			PrintfBit64(m_pRecFileEventInfo[i].endTime,false);
		}
		m_dvrWriter.dvrWriter.WriteFileEventInfo(m_pRecFileEventInfo, m_eventInfoNum);
	}	
}

//
void CBackupCutWriter::Stop()
{
	if (NULL != m_pBackupPathName)
	{
		delete [] m_pBackupPathName;
		m_pBackupPathName = NULL;
	}


	if (m_dvrWriter.hasOpen)
	{
		m_dvrWriter.dvrWriter.Close();
		m_dvrWriter.hasOpen = false;
	}

}


