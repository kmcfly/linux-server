
#include "BackupWriter.h"

#include "CreateHtmlLog.h"

#define AVI_BACKUP_NO_AUDIO

//
CBackupWriter::CBackupWriter()
{
	m_pBackupPathName = NULL;
	m_nWidth = 0;
	m_nHeight = 0;
	
}

//
CBackupWriter::~CBackupWriter()
{
	if (NULL != m_pBackupPathName)
	{
		delete [] m_pBackupPathName;
		m_pBackupPathName = NULL;
	}
	m_paviNameVec =NULL;
}

//
int CBackupWriter::Start(char *pBackupPath, BACKUP_DATA_TYPE dataType)
{
	assert(m_pBackupPathName == NULL);
	m_pBackupPathName = new char [255];

	if (NULL == m_pBackupPathName)
	{
		return -1;
	}

	m_pBackupPath = pBackupPath;
	
	m_dataType = dataType;

	//m_channelNum = channelNum;

	m_dvrWriter.fileIndex = 0;
	m_dvrWriter.hasOpen = false;
	m_dvrWriter.backupFirstFrameTime = 0;
	m_dvrWriter.backupLastFrameTime = 0;
	m_dvrWriter.reclogIndex = 0;

	m_stdWriter.stdFrmIndex = 0;
	m_stdWriter.lasttime = 0;
	m_stdWriter.hasAudio = false;
	m_stdWriter.hasOpen = false;
	m_stdWriter.pWavex = NULL;

	

	
	return 0;

}

//
int CBackupWriter::WriteFrame(FRAME_INFO_EX &frameInfo)
{
	if (NET_BACKUP_DATA_TYPE_DVR == m_dataType) 
	{	
		return DVRBackup(frameInfo);
	}
	else if (NET_BACKUP_DATA_TYPE_AVI == m_dataType) 
	{
		return AVIBackup(frameInfo);
	}
	return -1;
}

int CBackupWriter::DVRBackup(FRAME_INFO_EX &frameInfo)
{

	unsigned long frameType =  frameInfo.frameType;
	switch(frameType) 
	{
	case FRAME_TYPE_FILE_HEAD:
		CalculatePath(m_pBackupPathName, 0, false);
		CReclogEx::CreateLogFile(m_pBackupPathName, frameInfo.length, frameInfo.channel,true);
		if (!m_dvrWriter.writeReclog.OpenLogFileForBackup(m_pBackupPathName))
		{
			return BACKUP_WRITER_ERROR_OPEN_FAIL;
		}
		m_dvrWriter.fileIndex = 0;
		break;

	case FRAME_TYPE_FILE_INFO:
		m_dvrWriter.backupFirstFrameTime = 0;
		m_dvrWriter.backupLastFrameTime  = 0;
		m_dvrWriter.reclogIndex = 0;
		m_dvrWriter.hasOpen = false;
		memset(&m_dvrWriter.fileInfo, 0, sizeof(FILE_FILE_INFO));
		memset(&m_dvrWriter.fileSectionInfo, 0, sizeof(SECTION_INFO_EX)*MAX_SECTION_NUM);
		memset(&m_dvrWriter.fileReclogInfo, 0, sizeof(FILE_RECLOG_INFO)*MAX_RECLOG_NUM);

		memcpy(&m_dvrWriter.fileInfo, frameInfo.pData, sizeof(FILE_FILE_INFO));
		memcpy(&m_dvrWriter.fileSectionInfo, frameInfo.pData + sizeof(FILE_FILE_INFO), sizeof(SECTION_INFO_EX)*MAX_SECTION_NUM);
		memcpy(&m_dvrWriter.fileReclogInfo, frameInfo.pData+sizeof(FILE_FILE_INFO)+sizeof(SECTION_INFO_EX)*MAX_SECTION_NUM, sizeof(FILE_RECLOG_INFO)*MAX_RECLOG_NUM);
		m_dvrWriter.writeReclog.WriteFileStartForBackup(m_dvrWriter.fileIndex, m_dvrWriter.fileInfo.chnn);
		break;
	case FRAME_TYPE_VIDEO_FORMAT:
		if (!m_dvrWriter.hasOpen) 
		{
			CalculatePath(m_pBackupPathName, m_dvrWriter.fileIndex,true);
			if (m_dvrWriter.dvrWriter.Open(m_pBackupPathName, MAX_RECORD_FILE_LEN_M,false, true))
			{
				m_dvrWriter.hasOpen =  true;
			}
			else
			{
				return BACKUP_WRITER_ERROR_OPEN_FAIL;
			}
		}
		if (WRITE_FRAME_FAIL == m_dvrWriter.dvrWriter.WriteData(frameInfo))
		{	
			return BACKUP_WRITER_ERROR_WRITER_FAIL;
		}
		break;
	case FRAME_TYPE_AUDIO_FORMAT:
		assert(true == m_dvrWriter.hasOpen);
		if (WRITE_FRAME_FAIL == m_dvrWriter.dvrWriter.WriteData(frameInfo))
		{	
			return BACKUP_WRITER_ERROR_WRITER_FAIL;
		}
		break;
	case FRAME_TYPE_VIDEO:
		assert(true == m_dvrWriter.hasOpen);
		if (0 == m_dvrWriter.backupFirstFrameTime)
		{
			assert(0 == m_dvrWriter.reclogIndex);
			assert(0 == (m_dvrWriter.fileReclogInfo[m_dvrWriter.reclogIndex].time & RECLOG_END));
			m_dvrWriter.backupFirstFrameTime = frameInfo.time;
		}

		if (WRITE_FRAME_FAIL == m_dvrWriter.dvrWriter.WriteData(frameInfo))
		{	
			return BACKUP_WRITER_ERROR_WRITER_FAIL;
		}
		m_dvrWriter.backupLastFrameTime = frameInfo.time;
		break; 
	case FRAME_TYPE_AUDIO:
		assert(true == m_dvrWriter.hasOpen);		
		if (WRITE_FRAME_FAIL == m_dvrWriter.dvrWriter.WriteData(frameInfo))
		{	
			return BACKUP_WRITER_ERROR_WRITER_FAIL;
		}

		break; 
	case FRAME_TYPE_END:
		{
			m_dvrWriter.dvrWriter.Close();
			m_dvrWriter.dvrWriter.ClearAudioBuffer();
			m_dvrWriter.hasOpen =  false;
			bool bRet = WriteReclogInfo(m_dvrWriter);		
			m_dvrWriter.writeReclog.WriteFileEndForBackup();
            if (!bRet)
            {
				return BACKUP_WRITER_ERROR_WRITER_FAIL;
            }
			m_dvrWriter.fileIndex++;
		}		
		break;	
	default:
		break;
	}
	return BACKUP_WRITER_ERROR_SUC;
}



bool CBackupWriter::WriteReclogInfo(DVRWRITER &dvrWriter)
{
	unsigned long startTime32 = dvrWriter.backupFirstFrameTime / 1000000;
	unsigned long endTime32 = dvrWriter.backupLastFrameTime / 1000000;

	printf("first frame and end frame time:\n");
	PrintfBit32Time(startTime32);
	PrintfBit32Time(endTime32);

	unsigned char recTypeNum = RecTypeNum();
	assert(recTypeNum > 0);
    RECLOG_INFO_EX_LIST reclogInfoList;
	FILE_RECLOG_INFO *pData = dvrWriter.fileReclogInfo;
	unsigned long channel = dvrWriter.fileInfo.chnn;
	unsigned short diskIndex = 0;
	unsigned short fileIndex = dvrWriter.fileIndex;
	RECLOG_INFO_EX *pReclog = new RECLOG_INFO_EX [recTypeNum];
	memset(pReclog, 0, sizeof(RECLOG_INFO_EX)*recTypeNum);

	for (unsigned long i=0; i< MAX_RECLOG_NUM; ++i)
	{		
		//表示开始
		if ((RECORD_TYPE_NULL != pData[i].type) && (pData[i].type < RECORD_TYPE_END) && \
			(0 == (pData[i].type & RECLOG_END)))
		{
			pReclog [RecTypeIndex(static_cast<RECORD_TYPE>(pData[i].type))].chnn		= channel;
			pReclog [RecTypeIndex(static_cast<RECORD_TYPE>(pData[i].type))].type		= pData[i].type;
			pReclog [RecTypeIndex(static_cast<RECORD_TYPE>(pData[i].type))].startTime	= pData[i].time;

			pReclog [RecTypeIndex(static_cast<RECORD_TYPE>(pData[i].type))].diskIndex	= diskIndex;
			pReclog [RecTypeIndex(static_cast<RECORD_TYPE>(pData[i].type))].fileIndex	= fileIndex;
		}

		//表示结束，且时间不为零。
		if ((RECORD_TYPE_NULL != pData[i].type) && ((pData[i].type - RECLOG_END) < RECORD_TYPE_END) &&\
			(0 < pData[i].time) && (pData[i].type & RECLOG_END))
		{
			assert (pReclog [RecTypeIndex(static_cast<RECORD_TYPE>(pData[i].type))].type == (pData[i].type - RECLOG_END));
			pReclog [RecTypeIndex(static_cast<RECORD_TYPE>(pData[i].type))].endTime		= pData[i].time;
            AddToReclogList(pReclog [RecTypeIndex(static_cast<RECORD_TYPE>(pData[i].type))], reclogInfoList);

			memset(&pReclog [RecTypeIndex(static_cast<RECORD_TYPE>(pData[i].type))], 0, sizeof(RECLOG_INFO_EX));
		}
		
		//表示该预留空间未使用
		if ((RECORD_TYPE_NULL == pData[i].type) && (0 == pData[i].time))
		{
			break;
		}
	}
	
	//处理正在写的事件，因为其没有结束，所以需要在此根据其开始，并加上当前时间为结束时间作为一个事件加入列表。
	for (unsigned char i = 0; i < recTypeNum; ++i)
	{
		if ((RECORD_TYPE_NULL != pReclog [i].type) && (pReclog [i].type < RECORD_TYPE_END))
		{
			pReclog [i].endTime		= endTime32;
			if (pReclog[i].endTime <= pReclog[i].startTime)
			{
				PrintfBit32Time(pReclog[i].startTime);
				PrintfBit32Time(pReclog[i].endTime);
				assert(false);
				continue;
			}
			AddToReclogList(pReclog[i], reclogInfoList);
		}
	}
	delete [] pReclog;

	//过滤
	POS nextPos = NULL;
	POS pos = reclogInfoList.GetHeadPosition();
	while (NULL != pos)
	{
		RECLOG_INFO_EX &reclogInfo = reclogInfoList.GetAt(pos);
		nextPos = pos;
		reclogInfoList.GetNext(nextPos);
		if (!((reclogInfo.endTime <= startTime32) || (reclogInfo.startTime >= endTime32)))
		{
			if (startTime32 > reclogInfo.startTime)
			{
				reclogInfo.startTime = startTime32;
			}

			if (endTime32 < reclogInfo.endTime)
			{
				reclogInfo.endTime = endTime32;
			}
		}
		else
		{
			reclogInfoList.RemoveAt(pos);
		}
		
		pos = nextPos;
	}

	assert(reclogInfoList.GetCount() > 0);
	if (0 == reclogInfoList.GetCount())
	{
		RECLOG_INFO_EX temp;
		temp.startTime = startTime32;
		temp.endTime = endTime32;
		temp.type = RECORD_TYPE_MANUAL;
		reclogInfoList.AddTail(temp);
	}

	//写事件
	bool bRet = true;
	pos = reclogInfoList.GetHeadPosition();
	while (NULL != pos)
	{
		RECLOG_INFO_EX &reclogInfo = reclogInfoList.GetNext(pos);

		bRet = dvrWriter.writeReclog.WriteReclogStartForBackup(fileIndex, channel, (RECORD_TYPE)reclogInfo.type, reclogInfo.startTime);
        bRet = dvrWriter.writeReclog.WriteReclogEndForBackup(fileIndex, channel, (RECORD_TYPE)reclogInfo.type, reclogInfo.endTime);		
	}
	
	reclogInfoList.RemoveAll();

	return bRet;


}



void CBackupWriter::AddToReclogList(RECLOG_INFO_EX &reclog, RECLOG_INFO_EX_LIST &list)
{
	POS pos = NULL;

	//按照开始时间排序
	pos = GetReclogPrevNodePos(list, reclog.startTime);
	if (NULL != pos)
	{
		list.InsertAfter(pos, reclog);		
	}
	else
	{
		list.AddHead(reclog);
	}		

}


POS CBackupWriter::GetReclogPrevNodePos(RECLOG_INFO_EX_LIST &list, unsigned long time)
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
			//先检查最近插入的节点
			POS lastPos = list.GetLastInsertPosition();
			if ((NULL != lastPos) && (list.GetAt(lastPos).startTime < time) && (time <= list.NextNode(lastPos).startTime))
			{
				return lastPos;
			}

			/*
			把列表的节点对应于数组的下标，其在列表中相对于头节点的数目就是下标值。
			headPos--代表每次查找的一段列表的头节点
			pos--代表匹配的当前节点
			h--代表当前查找段的头节点下标
			t--代表当前查找段的尾节点下标
			m--代表当前查找段的中部节点下标
			*/
			POS headPos = list.GetHeadPosition();
			POS pos = NULL;
			long h=0, t=0, i=0, m=0, num = 0;
			t = list.GetCount()-1;

			while (h <= t)
			{
				pos = headPos;
				m = ((h+t)>>1);//计算中间节点的下标

				//意味着找到合适值了，pos = headPos。
				if (h == m)
				{
					break;
				}

				//找到m对应的节点，从理论上说该节点应该存在，不能找到NULL。
				i = 0;
				num = m - h;
				while (i < num)
				{
					list.GetNext(pos);
					++ i;
				}
				assert (NULL != pos);

				//把当前段的中间节点的开始时间和time比较，
				//如果node.startTime <= time，则需要对后半段再进行查找，所以headPos = pos，h = m。
				//否则，要对前半段进行查找，t = m。
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

int CBackupWriter::AVIBackup(FRAME_INFO_EX &frameInfo)
{

	int ret = 0;
	unsigned long frameType =  frameInfo.frameType;
	switch(frameType) 
	{
	case FRAME_TYPE_FILE_HEAD:
		//printf("%s,%d\n",__FILE__,__LINE__);		
		break;

	case FRAME_TYPE_FILE_INFO:
		//printf("%s,%d\n",__FILE__,__LINE__);
		m_stdWriter.hasOpen = false;
		m_stdWriter.hasAudio = false;
		m_stdWriter.lasttime = 0;
		m_stdWriter.stdFrmIndex = 0;
		m_stdWriter.offStdVideoIndex = 0;
		m_stdWriter.preStdVideoIndex = m_stdWriter.stdFrmIndex;
		m_stdWriter.firstTime = 0;
		m_stdWriter.preStdAudioIndex = 0;
		m_stdWriter.offStdAudioIndex = 0;
		memset(&m_stdWriter.fileInfo, 0 , sizeof(FILE_FILE_INFO));
		memset(&m_stdWriter.bmphdr, 0, sizeof(BITMAPINFOHEADER));
		if(m_stdWriter.pWavex != NULL)
		{
			delete m_stdWriter.pWavex;
			m_stdWriter.pWavex = NULL;
		}
        memcpy(&m_stdWriter.fileInfo, frameInfo.pData, sizeof(FILE_FILE_INFO));
		break;
	case FRAME_TYPE_VIDEO_FORMAT:
		//printf("%s,%d\n",__FILE__,__LINE__);
		memcpy(&(m_stdWriter.bmphdr), frameInfo.pData, frameInfo.length);
		CalculateFormat(m_stdWriter);
		break;
	case FRAME_TYPE_AUDIO_FORMAT:
		//printf("%s,%d\n",__FILE__,__LINE__);
		m_stdWriter.hasAudio = true;
		TranslateAudioFormat(frameInfo.pData, frameInfo.length);
#ifdef AVI_BACKUP_NO_AUDIO
		m_stdWriter.hasAudio = false;
#endif
		break;
	case FRAME_TYPE_VIDEO:
		//printf("%s,%d\n",__FILE__,__LINE__);
		if (!m_stdWriter.hasOpen) 
		{
			tm starttime = DVRTime32ToTm(m_stdWriter.fileInfo.startTime);
			sprintf(m_pBackupPathName, "%s/CH%02d-%04d-%02d-%02d-%02d-%02d-%02d.avi", m_pBackupPath,m_stdWriter.fileInfo.chnn+1, 
				                        starttime.tm_year+1900,starttime.tm_mon+1,starttime.tm_mday,starttime.tm_hour,
										starttime.tm_min,starttime.tm_sec);
			WAVEFORMATEX *pWformat = NULL;
			if (m_stdWriter.hasAudio)
			{
				pWformat = m_stdWriter.pWavex;
			}
			else
			{
				pWformat = NULL;
			}
			m_nWidth = frameInfo.width;
			m_nHeight = frameInfo.height;
			if (m_stdWriter.stdAviWriter.Open(m_pBackupPathName ,&m_stdWriter.bmphdr,pWformat, 
				                              (VIDEO_FORMAT_PAL == m_stdWriter.format)?true:false,AVI_TYPE_STD)) 
			{
				m_stdWriter.hasOpen =  true;
			}
			else
			{
				return BACKUP_WRITER_ERROR_OPEN_FAIL;
			}			
		}
		else
		{
			if (frameInfo.keyFrame)
			{
				//int nWidth = 0;
				//int nHeitht = 0;
				//getvideosize(frameInfo.pData, frameInfo.length, nWidth, nHeitht);
				if (frameInfo.width != m_nWidth || frameInfo.height != m_nHeight)
				{
					//printf("Change Format oldWidth = %d, oldHeight = %d, newWidth = %d, newHeight = %d\n",
							//m_nWidth, m_nHeight, nWidth, nHeitht);
					m_nHeight = frameInfo.height;
					m_nWidth  = frameInfo.width;
					
					m_stdWriter.stdAviWriter.Close();
					m_stdWriter.hasOpen = false;
					m_stdWriter.lasttime = 0;
					m_stdWriter.stdFrmIndex = 0;
					m_stdWriter.offStdVideoIndex = 0;
					m_stdWriter.preStdVideoIndex = m_stdWriter.stdFrmIndex;
					m_stdWriter.firstTime = frameInfo.time;
					m_stdWriter.preStdAudioIndex = 0;
					m_stdWriter.offStdAudioIndex = 0;
					
					m_stdWriter.bmphdr.biHeight = m_nHeight;
					m_stdWriter.bmphdr.biWidth = m_nWidth;
					m_stdWriter.fileInfo.startTime = frameInfo.time;

					CalculateFormat(m_stdWriter);
					tm starttime = DVRTime32ToTm(frameInfo.time);
					sprintf(m_pBackupPathName, "%s/CH%02d-%04d-%02d-%02d-%02d-%02d-%02d.avi", m_pBackupPath,m_stdWriter.fileInfo.chnn+1, 
						starttime.tm_year+1900,starttime.tm_mon+1,starttime.tm_mday,starttime.tm_hour,
						starttime.tm_min,starttime.tm_sec);
					WAVEFORMATEX *pWformat = NULL;
					if (m_stdWriter.hasAudio)
					{
						pWformat = m_stdWriter.pWavex;
					}
					else
					{
						pWformat = NULL;
					}

					if (m_stdWriter.stdAviWriter.Open(m_pBackupPathName ,&m_stdWriter.bmphdr,pWformat, 
						(VIDEO_FORMAT_PAL == m_stdWriter.format)?true:false,AVI_TYPE_STD)) 
					{
						m_stdWriter.hasOpen =  true;
						//printf("Open a new file filename = %s\n", m_pBackupPathName);
					}
					else
					{
						return BACKUP_WRITER_ERROR_OPEN_FAIL;
					}

					ret = m_stdWriter.stdAviWriter.WriteData(frameInfo.pData, frameInfo.length, m_stdWriter.stdFrmIndex, true, (0 == frameInfo.keyFrame)?false:true);
					if (AVI_WRITER_SECC == ret)
					{
						//printf("Write new frame success");
						break;
					}
					else
					{
						return BACKUP_WRITER_ERROR_WRITER_FAIL;
					}
				}	
			}
			if (VIDEO_FORMAT_PAL == m_stdWriter.format)//PAL
			{
				m_stdWriter.stdFrmIndex = ((frameInfo.time - m_stdWriter.firstTime) / 40000);
			}
			else//NTSC
			{
				m_stdWriter.stdFrmIndex = ((frameInfo.time - m_stdWriter.firstTime) / 33333);
			}

			//如果两帧之间相差过大（>45）时，过滤
			if( m_stdWriter.stdFrmIndex - m_stdWriter.preStdVideoIndex > 45 )
			{
				m_stdWriter.offStdVideoIndex += m_stdWriter.stdFrmIndex - m_stdWriter.preStdVideoIndex;
			}			
		}

		if (0 == m_stdWriter.firstTime)
		{
			ret = m_stdWriter.stdAviWriter.WriteData(frameInfo.pData, frameInfo.length, m_stdWriter.stdFrmIndex, true, (0 == frameInfo.keyFrame)?false:true);		
			m_stdWriter.firstTime = frameInfo.time;
			m_stdWriter.preStdVideoIndex = m_stdWriter.stdFrmIndex;
		}
		else
		{
			ret = m_stdWriter.stdAviWriter.WriteData(frameInfo.pData, frameInfo.length, m_stdWriter.stdFrmIndex - m_stdWriter.offStdVideoIndex, true, (0 == frameInfo.keyFrame)?false:true);		
			m_stdWriter.preStdVideoIndex = m_stdWriter.stdFrmIndex;			
		}

		if (AVI_WRITER_SECC != ret)
		{
			return BACKUP_WRITER_ERROR_WRITER_FAIL;
		}
		break;

	case FRAME_TYPE_AUDIO:
		//printf("%s,%d\n",__FILE__,__LINE__);
#ifdef AVI_BACKUP_NO_AUDIO
		break;
#else
		if (!m_stdWriter.hasOpen) 
		{
			tm starttime = DVRTimeToTm(m_stdWriter.fileInfo.startTime);	
			sprintf(m_pBackupPathName, "%s/CH%02d-%04d-%02d-%02d-%02d-%02d-%02d.avi", m_pBackupPath,m_stdWriter.fileInfo.chnn+1, 
				                       starttime.tm_year+1900,starttime.tm_mon+1,starttime.tm_mday,starttime.tm_hour,
				                       starttime.tm_min,starttime.tm_sec);
			WAVEFORMATEX *pWformat = NULL;
			if (m_stdWriter.hasAudio)
			{
				pWformat = m_stdWriter.pWavex;
			}
			else
			{
				pWformat = NULL;
			}

			if (m_stdWriter.stdAviWriter.Open(m_pBackupPathName ,&m_stdWriter.bmphdr,pWformat, 
				(VIDEO_FORMAT_PAL == m_stdWriter.format)?true:false,AVI_TYPE_STD)) 
			{
				m_stdWriter.hasOpen =  true;
			}
			else
			{
				return BACKUP_WRITER_ERROR_OPEN_FAIL;
			}
		}
		else
		{
			int ret = WriteAudioFrame(frameInfo);
			if (AVI_WRITER_SECC != ret)
			{
				return BACKUP_WRITER_ERROR_WRITER_FAIL;
			}
		}
		break;
#endif
	case FRAME_TYPE_END:
		{
			if(m_pBackupPathName != NULL)
			{
				m_paviNameVec->push_back(m_pBackupPathName);
			}
			m_stdWriter.stdAviWriter.Close();
			m_stdWriter.stdFrmIndex = 0;
			m_stdWriter.hasAudio = false;
			m_stdWriter.hasOpen = false;
			m_nWidth = 0;
			m_nHeight = 0;
		}
		break;
	default:
		return BACKUP_WRITER_ERROR_OPEN_FAIL;
		break;
	}

	return BACKUP_WRITER_ERROR_SUC;

}


void CBackupWriter::CalculatePath(char* pPath, unsigned short fileIndex, bool bFile)
{

	if (!bFile)
	{
		sprintf(pPath, "%s/reclog.bin", m_pBackupPath);
	}
	else
	{
		sprintf(pPath, "%s/%08d.dat", m_pBackupPath, fileIndex);
	}
}
	


void CBackupWriter::CalculateFormat(STDWRITER &stdWriter)
{
	if((double)(stdWriter.bmphdr.biHeight) / (stdWriter.bmphdr.biWidth) > 240.0/352)
	{
		stdWriter.format = VIDEO_FORMAT_PAL;
	}
	else
	{
		stdWriter.format = VIDEO_FORMAT_NTSC;
	}
	
}

//

//
void CBackupWriter::Stop()
{
	if (NULL != m_pBackupPathName)
	{
		delete [] m_pBackupPathName;
		m_pBackupPathName = NULL;
	}

	if (NET_BACKUP_DATA_TYPE_DVR == m_dataType)
	{
		m_dvrWriter.writeReclog.CloseLogFileForBackup();
		if (m_dvrWriter.hasOpen)
		{
			m_dvrWriter.dvrWriter.Close();
			m_dvrWriter.dvrWriter.ClearAudioBuffer();
			m_dvrWriter.hasOpen = false;
		}
	}

	if (NET_BACKUP_DATA_TYPE_AVI == m_dataType)
	{
		if (m_stdWriter.hasOpen)
		{
			m_stdWriter.stdAviWriter.Close();
			m_stdWriter.hasOpen = false;
		}
	}
	


}


//转换并保存音频格式
void CBackupWriter::TranslateAudioFormat(unsigned char *pData, unsigned long len)
{
	WAVEFORMATEX *pWaveFormat = reinterpret_cast<WAVEFORMATEX *>(pData);
	
	if(NULL != m_stdWriter.pWavex)
	{
		delete m_stdWriter.pWavex;
	}

	//现在只有一种音频格式，HISI的ADPCM_DVI4，以后如有其他格式，要在此扩充，并且要把格式的wFormatTag用标准定义
	if(WAVE_FORMAT_DVI_ADPCM == pWaveFormat->wFormatTag)
	{
		IMAADPCMWAVEFORMAT *pDviWaveFormat = new IMAADPCMWAVEFORMAT;
		pDviWaveFormat->wfx.wFormatTag = 0x11;	//IMA_ADPCM
		pDviWaveFormat->wfx.wBitsPerSample = 4;
		pDviWaveFormat->wfx.nChannels = 1;
		pDviWaveFormat->wfx.nSamplesPerSec = 8000;
		pDviWaveFormat->wfx.nBlockAlign = 164;
		pDviWaveFormat->wfx.nAvgBytesPerSec = 4100;
		pDviWaveFormat->wfx.cbSize = 2;
		pDviWaveFormat->wSamplesPerBlock = (pDviWaveFormat->wfx.nBlockAlign/4-1)*8+1;
		m_stdWriter.pWavex = (WAVEFORMATEX*)pDviWaveFormat;
	}
	else
	{
		m_stdWriter.pWavex = new WAVEFORMATEX;
		memcpy(m_stdWriter.pWavex, pWaveFormat, sizeof(WAVEFORMATEX));
	}
}

//写音频帧，根据不同的编码格式区分来写，有数据丢失时，要插入空帧
int CBackupWriter::WriteAudioFrame(const FRAME_INFO_EX &frameInfo)
{
	if(NULL == m_stdWriter.pWavex)
	{
		return AVI_WRITER_SECC;
	}

	if(WAVE_FORMAT_DVI_ADPCM == m_stdWriter.pWavex->wFormatTag)
	{
		long blockSize = m_stdWriter.pWavex->nBlockAlign;

		if (0 == m_stdWriter.firstTime)
		{
			m_stdWriter.firstTime = frameInfo.time;
			m_stdWriter.preStdAudioIndex = 0;
			m_stdWriter.offStdAudioIndex = 0;
		}
		else
		{
			long stdFrmIndex = ((frameInfo.time - m_stdWriter.firstTime) / 200000);	// 200ms/frame

			//如果两帧之间相差过大（>45）时，过滤
			if( stdFrmIndex - m_stdWriter.preStdAudioIndex > 45 )
			{
				unsigned long newOffIndex = m_stdWriter.offStdVideoIndex*40/200 - m_stdWriter.offStdAudioIndex;
				m_stdWriter.preStdVideoIndex += newOffIndex;
				m_stdWriter.offStdAudioIndex += newOffIndex;
			}

			long dataLen = (frameInfo.length/blockSize)*blockSize;
			unsigned char *pTempData = new unsigned char[dataLen];
			//插入音频空帧
			for(int i = m_stdWriter.preStdVideoIndex+1; i < stdFrmIndex; i++)
			{
				long ret = m_stdWriter.stdAviWriter.WriteData(pTempData, dataLen, 0, false);
				if (AVI_WRITER_SECC != ret)
				{
					delete [] pTempData;
					return ret;
				}
			}

			delete [] pTempData;

			m_stdWriter.preStdAudioIndex = stdFrmIndex;
		}

		int pack = 0;
		for(pack = 0; pack < frameInfo.length/blockSize; pack++)
		{
			long ret = m_stdWriter.stdAviWriter.WriteData(frameInfo.pData+pack*(blockSize+4)+4, blockSize, 0,false);
			if (AVI_WRITER_SECC != ret)
			{
				return ret;
			}
		}
	}

	return AVI_WRITER_SECC;
}

void CBackupWriter::SetAviFileNameVec(vector<std::string> *pAviNameVec)
{
	m_paviNameVec=pAviNameVec;
}

