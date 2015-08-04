
//RecWriter.cpp

#include "RecWriter.h"

#ifdef __REC_WRITE_BUF_256__
unsigned long REC_DATA_WRITE_BUF_SIZE	= 256 * 1024;
#else
#ifdef __TDFH__
unsigned long REC_DATA_WRITE_BUF_SIZE	= 1024 * 1024 + 2 * 1024;
#else
unsigned long REC_DATA_WRITE_BUF_SIZE	= 128 * 1024;
#endif
#endif

unsigned long CRecWriter::m_lastFFlushTime = 0;

CRecWriter::CRecWriter ()
{
	m_pFile = NULL;
	m_pDataEndFlag = NULL;

	memset(&m_bufInfo, 0, sizeof(REC_BUF_INFO));

#if REC_WRITE_DO_BUFFER
	m_bufInfo.pData = new unsigned char [REC_DATA_WRITE_BUF_SIZE];//
	memset(m_bufInfo.pData, 0, REC_DATA_WRITE_BUF_SIZE);
	m_bufInfo.bufLen = REC_DATA_WRITE_BUF_SIZE;
#endif

	m_bCombined = false;
	m_bBuckUp = false;
}

CRecWriter::~CRecWriter ()
{
#if REC_WRITE_DO_BUFFER
	if (NULL != m_bufInfo.pData)
	{
		delete [] m_bufInfo.pData;
		memset(&m_bufInfo, 0, sizeof(REC_BUF_INFO));
	}
#endif
}

bool CRecWriter::Open(const char *pPath,unsigned long fileSizeM, bool bCombined, bool bBackup)
{
	FD_ZERO(&m_fdset);

	m_bBuckUp = bBackup;

	m_vpFrameNum = 0;	
	m_bNeedWriteKey = true;
	assert(m_pFile == NULL);
	m_bCombined = bCombined;
	if (128 == fileSizeM)
	{
		m_MAX_REC_SUB_OFF_LEN	  = 4 * 1024 * 1024;			//当剩余空间小于此值时，一旦进入关键帧，则不能写入且返回失败
		m_MAX_REC_FILE_LEN	      = 128 * 1024 * 1024;
		m_MAX_KEY_FRAME_NUM	      = 16*m_MAX_REC_FILE_LEN/(1024*1024);	//一个文件最多可以写入的关键帧索引（根据码流256KBPS，和100帧一个关键帧算出）。
	}
	else if(256 == fileSizeM)
	{
#if defined(__CHIP3531__) || defined(__CHIP3535__)
		m_MAX_REC_SUB_OFF_LEN	  = 36 * 1024 * 1024;
#else
		m_MAX_REC_SUB_OFF_LEN	  = 10 * 1024 * 1024;
#endif
		m_MAX_REC_FILE_LEN	      = 256 * 1024 * 1024;
		m_MAX_KEY_FRAME_NUM	      = 4096;
	}
	else if (512 == fileSizeM)
	{
		m_MAX_REC_SUB_OFF_LEN	  = 10 * 1024 * 1024;			
		m_MAX_REC_FILE_LEN	      = 512 * 1024 * 1024;
		m_MAX_KEY_FRAME_NUM	      = 8192;	
	}
	else if (16 == fileSizeM)
	{
		m_MAX_REC_SUB_OFF_LEN	  = 4 * 1024 * 1024;			
		m_MAX_REC_FILE_LEN	      = 16 * 1024 * 1024;
		m_MAX_KEY_FRAME_NUM	      = 16*m_MAX_REC_FILE_LEN/(1024*1024);	
	}
	else if (32 == fileSizeM)
	{
		m_MAX_REC_SUB_OFF_LEN	  = 4 * 1024 * 1024;			
		m_MAX_REC_FILE_LEN	      = 32 * 1024 * 1024;
		m_MAX_KEY_FRAME_NUM	      = 16*m_MAX_REC_FILE_LEN/(1024*1024);	
	}
	else
	{
		m_MAX_REC_SUB_OFF_LEN	  = 4 * 1024 * 1024;			
		m_MAX_REC_FILE_LEN	      = 128 * 1024 * 1024;
		m_MAX_KEY_FRAME_NUM	      = 16*m_MAX_REC_FILE_LEN/(1024*1024);
	}

	
	m_VIDEO_FORMAT_POS	          = INDEX_INFO_POS + (m_MAX_KEY_FRAME_NUM * sizeof(INDEX_INFO));
	m_AUDIO_FORMAT_POS	          = m_VIDEO_FORMAT_POS + sizeof(BITMAPINFOHEADER) + 32;
	if (m_bCombined)
	{
		m_REC_FILE_EVENT_INFO_POS = m_AUDIO_FORMAT_POS + sizeof(WAVEFORMATEX) + 32;
		m_FRAME_DATA_POS          = m_REC_FILE_EVENT_INFO_POS + sizeof(REC_FILE_EVENT_INFO)*MAX_REC_FILE_EVENT_INFO;
	}
	else
	{
		m_REC_FILE_EVENT_INFO_POS = 0;
		m_FRAME_DATA_POS          = m_AUDIO_FORMAT_POS + sizeof(WAVEFORMATEX) + 32;
	}
	m_FILE_BASIC_LEN		= m_FRAME_DATA_POS;
	
	if(m_pFile != NULL)
	{
		return false;
	}

	m_pFile = fopen(pPath, "rb+");

	//Linux环境里，不能以重新创建的方式去打开文件。
//#ifdef __ENVIRONMENT_WIN32__
	if(m_pFile == NULL)
	{
		m_pFile = fopen(pPath, "wb");
	}
//#endif

	if(m_pFile == NULL)
	{
		return false;
	}

	m_fd = fileno(m_pFile);

	INDEX_INFO *pIndexBuf = new INDEX_INFO[m_MAX_KEY_FRAME_NUM];
	if(pIndexBuf == NULL)
	{
		assert(false);
		Release(&m_pFile, 0);
		return false;
	}
	memset(pIndexBuf, 0, sizeof(INDEX_INFO) * m_MAX_KEY_FRAME_NUM);

	memset(&m_fileHeader, 0, sizeof(REC_FILE_HEADER));
	if (m_bCombined)
	{
		m_fileHeader.hFlag = HEADER_FLAG_COMBINE;		//该信息头标记
	}
	else
	{
		m_fileHeader.hFlag = HEADER_FLAG;					//该信息头标记
	}	
	m_fileHeader.hLen = sizeof(REC_FILE_HEADER);		//该信息头长度
	m_fileHeader.frameInfoLen = sizeof(REC_FRAME_INFO);				//
	m_fileHeader.fileFlag = FILE_FLAG;					//文件标记——"FTVT"
	m_fileHeader.fileLen = m_FILE_BASIC_LEN;				//文件总长度
	m_fileHeader.writeFlg = WRITE_ING_FLAG;				//写标记
	m_fileHeader.videoFormatAddr = m_VIDEO_FORMAT_POS;	//视频格式位置
	m_fileHeader.audioFormatAddr = m_AUDIO_FORMAT_POS;	//音频格式位置
	m_fileHeader.indexAddr = INDEX_INFO_POS;			//索引区位置
	m_fileHeader.dataAddr = m_FRAME_DATA_POS;				//数据区位置
   
	m_startTime = 0;
	m_endTime = 0;

	if(0 != fseek(m_pFile, 0, SEEK_SET))
	{
		assert(false);
		Release(&m_pFile, 1, &pIndexBuf);
		return false;
	}

	if(1 != RecFWrite(&m_fileHeader, sizeof(REC_FILE_HEADER), 1, m_pFile))
	{
		Release(&m_pFile, 1, &pIndexBuf);
		return false;
	}

	//清空索引区
	if(0 != fseek(m_pFile, INDEX_INFO_POS, SEEK_SET))
	{
		Release(&m_pFile, 1, &pIndexBuf);
		return false;
	}
	
	if(m_MAX_KEY_FRAME_NUM != RecFWrite(pIndexBuf, sizeof(INDEX_INFO), m_MAX_KEY_FRAME_NUM, m_pFile))
	{
		Release(&m_pFile, 1, &pIndexBuf);
		return false;
	}

	//清空视频格式
	BITMAPINFOHEADER bmpInfoHeader;
	memset(&bmpInfoHeader, 0, sizeof(BITMAPINFOHEADER));
	if(0 != fseek(m_pFile, m_VIDEO_FORMAT_POS, SEEK_SET))
	{
		Release(&m_pFile, 0);
		return false;
	}
	if(1 != RecFWrite(&bmpInfoHeader, sizeof(BITMAPINFOHEADER), 1, m_pFile))
	{
		Release(&m_pFile, 0);
		return false;
	}


	//清空音频格式
	WAVEFORMATEX wfx;
	memset(&wfx, 0, sizeof(WAVEFORMATEX));
	if(0 != fseek(m_pFile, m_AUDIO_FORMAT_POS, SEEK_SET))
	{
		Release(&m_pFile, 0);
		return false;
	}
	if(1 != RecFWrite(&wfx, sizeof(WAVEFORMATEX), 1, m_pFile))
	{
		Release(&m_pFile, 0);
		return false;
	}

	if (m_bCombined)
	{
		REC_FILE_EVENT_INFO eventInfo;
		memset(&eventInfo, 0, sizeof(eventInfo));
		if (0 != fseek(m_pFile, m_REC_FILE_EVENT_INFO_POS, SEEK_SET))
		{
			Release(&m_pFile, 0);
			return false;
		}
		for (int i = 0; i < MAX_REC_FILE_EVENT_INFO; ++i)
		{
			if (1 != RecFWrite(&eventInfo, sizeof(eventInfo), 1, m_pFile))
			{
				Release(&m_pFile, 0);
				return false;
			}
		}
	}	
	
	//还需要清空第一帧的信息
	if(0 != fseek(m_pFile, m_fileHeader.dataAddr, SEEK_SET))
	{
		Release(&m_pFile, 1, &pIndexBuf);
		return false;
	}	
    REC_FRAME_INFO frameInfo;
	memset(&frameInfo, 0, sizeof(REC_FRAME_INFO));
	if (1 != RecFWrite(&frameInfo, sizeof(REC_FRAME_INFO), 1, m_pFile))
	{
		Release(&m_pFile, 1, &pIndexBuf);
		return false;
	}

	//把文件指针定位到数据区的开始位置
	if(0 != fseek(m_pFile, m_fileHeader.dataAddr, SEEK_SET))
	{
		Release(&m_pFile, 1, &pIndexBuf);
		return false;
	}	

	m_IndexNum	= 0;
	m_videoNum	= 0;
	m_audioNum	= 0;
	m_textNum	= 0;

	m_indexPos	= INDEX_INFO_POS;
	m_bufInfo.filePos = m_fileHeader.dataAddr;

	m_bHasVideo	= false;
	m_bHasAudio	= false;

	memset(&m_frameInfo, 0, sizeof(REC_FRAME_INFO));
	memset(&m_indexInfo, 0, sizeof(INDEX_INFO));
	m_indexInfo.flag = I_VIDEO_FLAG;

	delete [] pIndexBuf;
	
	m_pDataEndFlag = new unsigned char[sizeof(REC_FRAME_INFO)];
	memset(m_pDataEndFlag, 0, sizeof(REC_FRAME_INFO));
	
#if REC_WRITE_DO_BUFFER
	memset(m_bufInfo.pData, 0, REC_DATA_WRITE_BUF_SIZE);
	m_bufInfo.dataLen = 0;
#endif

	return true;
}

int CRecWriter::WriteFileEventInfo(REC_FILE_EVENT_INFO *pEventInfo, int num)
{
	int writeNum = num;
	if (writeNum > MAX_REC_FILE_EVENT_INFO)
	{
		writeNum = MAX_REC_FILE_EVENT_INFO;
	}
	if (NULL == pEventInfo)
	{
		return 0;
	}

	if (!m_bCombined)
	{
		return 0;
	}

	if (0 != fseek(m_pFile, m_REC_FILE_EVENT_INFO_POS, SEEK_SET))
	{
		return -1;
	}
	
	if (writeNum != RecFWrite(pEventInfo, sizeof(REC_FILE_EVENT_INFO), writeNum, m_pFile))
	{
		return -1;
	}
	return 0;
}

void CRecWriter::Close ()
{
	if(NULL != m_pFile)
	{
		////清除音频列表
		//while(m_audioList.GetCount() > 0)
		//{
		//	WriteFrame(m_audioList.GetHead());

		//	delete [] m_audioList.RemoveHead().pData;
		//}
#if REC_WRITE_DO_BUFFER
		//把缓冲区中的数据写到文件中
		if(m_bufInfo.dataLen > 0)
		{
			if (0 == fseek(m_pFile, m_bufInfo.filePos, SEEK_SET))
			{
				RecFWrite(m_bufInfo.pData, m_bufInfo.dataLen+sizeof(REC_FRAME_INFO), 1, m_pFile);
				m_bufInfo.filePos += m_bufInfo.dataLen;
			}
			else
			{
				assert(false);
			}
		}
#endif
		//写文件头
		{
			m_fileHeader.fileLen = m_bufInfo.filePos;
			m_fileHeader.indexNum = m_IndexNum;
			m_fileHeader.videoNum = m_videoNum;
			m_fileHeader.audioNum = m_audioNum;
			m_fileHeader.textNum = m_textNum;
			m_fileHeader.startTime = m_startTime;
			m_fileHeader.endTime = m_endTime;
			m_fileHeader.writeFlg = WRITE_END_FLAG;

			if(0 != fseek(m_pFile, 0, SEEK_SET))
			{
				assert(false);
			}

			if(1 != RecFWrite(&m_fileHeader, sizeof(REC_FILE_HEADER), 1, m_pFile))
			{
				assert(false);
			}
		}

#if defined(__TDFH__) || defined(__CHIPTI__)
		//FH平台在操作系统层缓冲数据太多，导致在异常断电时关键帧的索引没有写入
		if (fflush(m_pFile))
		{
			printf("%s:%d, errno=%d\n", __FILE__, __LINE__, errno);
		}


		if (fsync(fileno(m_pFile)))
		{
			printf("%s:%d, errno=%d\n", __FILE__, __LINE__, errno);
		}
#endif
		fclose(m_pFile);
		m_pFile = NULL;
		delete [] m_pDataEndFlag;
	}
}

void CRecWriter::ClearAudioBuffer()
{
	while(m_audioList.GetCount() > 0)
	{
		delete [] m_audioList.RemoveHead().pData;
	}
}

bool CRecWriter::FFlush()
{
#if REC_WRITE_DO_BUFFER
	if(0 == m_bufInfo.dataLen)
	{
		return true;
	}

	if (0 != fseek(m_pFile, m_bufInfo.filePos, SEEK_SET))
	{
		return false;
	}
		
	RecFWrite(m_bufInfo.pData, m_bufInfo.dataLen+sizeof(REC_FRAME_INFO), 1, m_pFile);
	fflush(m_pFile);
	//PrintfBit64(m_endTime, false);
	m_bufInfo.filePos += m_bufInfo.dataLen;
	m_bufInfo.dataLen = 0;			

	return true;
#else
	return true;
#endif

}

/******************************************************************************
函数功能：写入一帧数据(音频，视频或文本),要求每一类型各自有序输入
参数说明：frame：要写入的数据帧，数据段keyFrame、frameType、length、pData、time
		  必须填充
返回值：成功返回：WRITE_FRAME_SUCC
		失败返回：WRITE_FRAME_NEED_VIDEO_FORMAT(需要视频格式)
				  WRITE_FRAME_NEED_AUDIO_FORMAT(需要音频格式)
				  WRITE_FRAME_FULL(文件已经写满)
				  WRITE_FRAME_FAIL(磁盘写出错或其他异常错误)
******************************************************************************/
unsigned long CRecWriter::WriteData(const FRAME_INFO_EX & frame)
{
	
	assert(m_pFile != NULL);
	
	if (!m_bHasVideo && (FRAME_TYPE_VIDEO_FORMAT == frame.frameType))
	{
		assert (sizeof(BITMAPINFOHEADER) == frame.length);
		if (0 == fseek(m_pFile, m_fileHeader.videoFormatAddr, SEEK_SET))
		{
			if (1 == RecFWrite(frame.pData, sizeof(BITMAPINFOHEADER), 1, m_pFile))
			{
				m_bHasVideo = true;
				
				return WRITE_FRAME_SUCC;
			}
		}

		return WRITE_FRAME_FAIL;
	}
	else if (FRAME_TYPE_AUDIO_FORMAT == frame.frameType)
	{
		assert (sizeof(WAVEFORMATEX) == frame.length);
		if (0 == fseek(m_pFile, m_fileHeader.audioFormatAddr, SEEK_SET))
		{
			if (1 == RecFWrite(frame.pData, sizeof(WAVEFORMATEX), 1, m_pFile))
			{
				m_bHasAudio = true;
				
				return WRITE_FRAME_SUCC;
			}
		}

		return WRITE_FRAME_FAIL;
	}
	else if (m_bHasVideo && (FRAME_TYPE_VIDEO == frame.frameType))
	{
		return OnWriteOneVideo(frame);
	}
	else if (FRAME_TYPE_AUDIO == frame.frameType)//要求输入的音频按时间有序,把新的音频帧加入到音频帧列表尾
	{
		if(!m_bHasAudio)	//音频格式尚未到达
		{
			return WRITE_FRAME_NEED_AUDIO_FORMAT;
		}

#if defined(__TDFH__) || defined(__CHIP3531__) || defined(__CHIP3535__) || defined(__CHIP3520A__) || defined(__CHIP3521__) || defined(__CHIPTI__) || defined(__CHIP3520D__) || defined(__CHIPGM__)
		//音频流的时间小于于当前文件的开始时间则认为音频时间错
		//音频流的时间和视频流的时间不能相差很大，如果相差很大，会导致丢掉一些音频数据
		if (0 == m_fileHeader.startTime || frame.time < m_fileHeader.startTime)
		{
			//printf("%s:%s:%d, The audio time is < the file start time\n", __FUNCTION__, __FILE__, __LINE__);
			//PrintfBit64(frame.time, true);
			//PrintfBit64(m_fileHeader.startTime, true);
			//PrintfBit64(frame.time, false);
			//PrintfBit64(m_fileHeader.startTime, false);
			return WRITE_FRAME_SUCC;
		}
#endif
		//音频数量过多时，从列表中删除最老的一帧音频，以在加入最新的一帧音频时数量没有超标
		if (m_audioList.GetCount() >= AUDIO_BUF_NUM)
		{
			delete [] m_audioList.RemoveHead().pData;
		}

		//音频要求递增有序，否则断言且不加入列表
		if(!m_audioList.IsEmpty() && (frame.time < m_audioList.GetTail().time))
		{
			//printf("chnn = %d, frame.time = 0x%08x%08x, prevTime = 0x%08x%08x\n", frame.channel, frame.time, m_audioList.GetTail().time);
		}
		
		assert (m_audioList.IsEmpty() || (frame.time >= m_audioList.GetTail().time));

		if (m_audioList.IsEmpty() || (frame.time >= m_audioList.GetTail().time))
		{
			FRAME_INFO_EX audioFrame = frame;
			audioFrame.pData = new BYTE[audioFrame.length];
			if(NULL == audioFrame.pData)
			{
				assert(false);
			}
			else
			{
				memcpy(audioFrame.pData, frame.pData, audioFrame.length);
				m_audioList.AddTail(audioFrame);
			}
		}

		return  WRITE_FRAME_SUCC;
	}
	else if (FRAME_TYPE_TEXT == frame.frameType)	//文本数据暂不处理
	{
		//将文本有序地加入到文本列表中;
		assert (false);
		return WRITE_FRAME_FAIL;
	}
	else
	{
		if (!m_bHasVideo)
		{
			return WRITE_FRAME_NEED_VIDEO_FORMAT;
		}

		return WRITE_FRAME_FAIL;
	}
}

/******************************************************************************

				protected functions

******************************************************************************/

/******************************************************************************
函数功能：向文件中写入帧索引
调用顺序：要求在关键帧来时
******************************************************************************/
unsigned long CRecWriter::WriteIndex(unsigned long index, unsigned long pos, LONGLONG time)
{
	//意味着预留的Index区域已经用完，不能再写入新的帧，所以返回失败。
	if (m_MAX_KEY_FRAME_NUM <= m_IndexNum)
	{
		return WRITE_FRAME_FULL;
	}

	m_indexInfo.index		= index;
	m_indexInfo.position	= pos;
	m_indexInfo.time		= time;

	if(0 != fseek(m_pFile, m_indexPos, SEEK_SET))
	{
		return WRITE_FRAME_FAIL;
	}

	if(1 != RecFWrite(&m_indexInfo, sizeof(INDEX_INFO), 1, m_pFile))
	{
		return WRITE_FRAME_FAIL;
	}

#if defined(__TDFH__) || defined(__CHIPTI__)
	//FH平台在操作系统层缓冲数据太多，导致在异常断电时关键帧的索引没有写入
 	FFlushEx(m_pFile);
	//if (fflush(m_pFile))
 //	{
 //		printf("%s:%d, errno=%d\n", __FILE__, __LINE__, errno);
 //	}
 //	
 //
 //	if (fsync(fileno(m_pFile)))
 //	{
 //		printf("%s:%d, errno=%d\n", __FILE__, __LINE__, errno);
 //	}
#endif

	++ m_IndexNum;
	m_indexPos += sizeof(INDEX_INFO);

	return WRITE_FRAME_SUCC;
}

/******************************************************************************
函数功能：写入一帧数据的具体实现
实现方法：先将数据写入缓冲区，然后再多帧统一写入文件
******************************************************************************/
unsigned long CRecWriter::WriteFrame(const FRAME_INFO_EX & frame)
{
	//构造帧信息结构
	{
		if (FRAME_TYPE_VIDEO == frame.frameType)
		{
			m_frameInfo.flag	= frame.keyFrame ? I_VIDEO_FLAG : B_VIDEO_FLAG;
			m_vpFrameNum++;

			if(m_vpFrameNum > 80)
			{
				m_bNeedWriteKey = true;
				m_vpFrameNum = 0;
			}
		}
		else if (FRAME_TYPE_AUDIO == frame.frameType)
		{
			m_frameInfo.flag	= I_AUDIO_FLAG;
		}
		else if (FRAME_TYPE_TEXT == frame.frameType)
		{
			m_frameInfo.flag	= I_TEXT_FLAG;
		}
		else
		{
			assert (false);
			return WRITE_FRAME_FAIL;
		}

		if(0 == m_frameInfo.nextPos)		//写第一帧数据
		{
			m_frameInfo.prevPos	= 0;
			m_frameInfo.nextPos = m_fileHeader.dataAddr + sizeof(REC_FRAME_INFO) + frame.length;
		}
		else
		{
			m_frameInfo.prevPos = m_frameInfo.nextPos - m_frameInfo.length - sizeof(REC_FRAME_INFO);
			m_frameInfo.nextPos = m_frameInfo.nextPos + sizeof(REC_FRAME_INFO) + frame.length;
		}

		
		m_frameInfo.length	= frame.length;
		m_frameInfo.time	= frame.time;
#if defined(__TDFH__) || defined(__CHIPTI__)
		m_frameInfo.timeStampFlag = 0;
		m_frameInfo.timeStampFlag = RecFrameInfoChsum(&m_frameInfo);
#else
		m_frameInfo.timeStampFlag = 1;
#endif
	}

	//问题：备份一个录像长度大约为1小时20分钟的文件，必然失败。
	//原因：问题出现在这个录像文件本身，是在当时在写这个录像文件时，就出现问题了。
	//由于之前是先判断录像文件是否写满，如果没有写满，就帧写入到文件里，然后判断
	//关键帧是否写满，如果写满就返回失败。这就出现下面的问题：关键帧已经写入文件里，但是返回失败。
	//所以在此处先判断关键帧是否写满。
	if ((FRAME_TYPE_VIDEO == frame.frameType) && frame.keyFrame)
	{
		//m_IndexNum编号从0开始计数，表示已经写入关键帧的个数。
		//如果当前m_IndexNum为MAX_KEY_FRAME_NUM，即文件已经写入了MAX_KEY_FRAME_NUM个关键帧，
		//而在这时再来一个关键帧的话，那么就没有地方写这个关键帧的索引了，所以要返回满
		if (m_MAX_KEY_FRAME_NUM <= m_IndexNum)
		{
			return WRITE_FRAME_FULL;
		}
	}
	//end 2010-1-23 15:52 xwm

#if REC_WRITE_DO_BUFFER
	//
	if (m_MAX_REC_FILE_LEN >= (m_bufInfo.filePos + sizeof(REC_FRAME_INFO) + frame.length + sizeof(REC_FRAME_INFO) + m_bufInfo.dataLen))
	{
		do 
		{
			if ((m_bufInfo.dataLen + sizeof(REC_FRAME_INFO) + m_frameInfo.length + sizeof(REC_FRAME_INFO)) <= m_bufInfo.bufLen)
			{
				//
				memcpy(m_bufInfo.pData + m_bufInfo.dataLen, &m_frameInfo, sizeof(REC_FRAME_INFO));
				m_bufInfo.dataLen += sizeof(REC_FRAME_INFO);
				//
				memcpy(m_bufInfo.pData + m_bufInfo.dataLen, frame.pData, m_frameInfo.length);
				m_bufInfo.dataLen += m_frameInfo.length;
				//
				memcpy(m_bufInfo.pData + m_bufInfo.dataLen, m_pDataEndFlag, sizeof(REC_FRAME_INFO));

				//每次写视频关键帧时，也写一次磁盘
				if ((FRAME_TYPE_VIDEO == frame.frameType) && frame.keyFrame && m_bNeedWriteKey)
				{
					if (0 != fseek(m_pFile, m_bufInfo.filePos, SEEK_SET))
					{					
						assert (false);
						return WRITE_FRAME_FAIL;
					}

					if (1 != RecFWrite(m_bufInfo.pData, m_bufInfo.dataLen+sizeof(REC_FRAME_INFO), 1, m_pFile))
					{
						assert (false);
						return WRITE_FRAME_FAIL;
					}

					m_bufInfo.filePos += m_bufInfo.dataLen;
					m_bufInfo.dataLen = 0;
				}

				//
				break;
			}
			else if ((sizeof(REC_FRAME_INFO) + m_frameInfo.length + sizeof(REC_FRAME_INFO)) <= m_bufInfo.bufLen)
			{
				//意味着该帧本身长度小于等于缓冲区长度
				if (0 != fseek(m_pFile, m_bufInfo.filePos, SEEK_SET))
				{
					assert (false);
					return WRITE_FRAME_FAIL;
				}

				if (1 != RecFWrite(m_bufInfo.pData, m_bufInfo.dataLen+sizeof(REC_FRAME_INFO), 1, m_pFile))
				{
					assert (false);
					return WRITE_FRAME_FAIL;
				}

				m_bufInfo.filePos += m_bufInfo.dataLen;
				m_bufInfo.dataLen = 0;
			}
			else
			{
				if (0 != fseek(m_pFile, m_bufInfo.filePos, SEEK_SET))
				{
					assert (false);
					return WRITE_FRAME_FAIL;
				}

				if (m_bufInfo.dataLen > 0)
				{
					if (1 != RecFWrite(m_bufInfo.pData, m_bufInfo.dataLen, 1, m_pFile))
					{
						assert (false);
						return WRITE_FRAME_FAIL;
					}
				}
			
				//
				if (1 != RecFWrite(&m_frameInfo, sizeof(REC_FRAME_INFO), 1, m_pFile))
				{
					assert (false);
					return WRITE_FRAME_FAIL;
				}

				if (1 != RecFWrite(frame.pData, m_frameInfo.length, 1, m_pFile))
				{
					assert (false);
					return WRITE_FRAME_FAIL;
				}

				if (1 != RecFWrite(m_pDataEndFlag, sizeof(REC_FRAME_INFO), 1, m_pFile))
				{
					assert (false);
					return WRITE_FRAME_FAIL;
				}

				m_bufInfo.filePos += (m_bufInfo.dataLen + sizeof(REC_FRAME_INFO) + m_frameInfo.length);
				m_bufInfo.dataLen = 0;

				break;
			}
		} while(true);
	}
	else
	{
		return WRITE_FRAME_FULL;
	}
#else
	{
		/*写入一帧数据，如果总长度超出文件总长度，则返回文件已经满*/
		if (m_MAX_REC_FILE_LEN >= (m_bufInfo.filePos + sizeof(REC_FRAME_INFO) + frame.length + sizeof(REC_FRAME_INFO)))
		{
			//正常数据的位置
			if(0 != fseek(m_pFile, m_bufInfo.filePos, SEEK_SET))
			{
				assert (false);				
				return WRITE_FRAME_FAIL;
			}

			//写入帧信息头
			if (1 != RecFWrite(&m_frameInfo, sizeof(REC_FRAME_INFO), 1, m_pFile))
			{
				assert (false);
				return WRITE_FRAME_FAIL;
			}

			//写入帧数据
			if (1 != RecFWrite(frame.pData, frame.length, 1, m_pFile))
			{
				assert (false);				
				return WRITE_FRAME_FAIL;
			}
			
			//写入数据结束标记
			if (1 != RecFWrite(m_pDataEndFlag, sizeof(REC_FRAME_INFO), 1, m_pFile))
			{
				assert (false);
				return WRITE_FRAME_FAIL;
			}

			m_bufInfo.filePos += (sizeof(REC_FRAME_INFO) + frame.length);
		}
		else
		{			
			return WRITE_FRAME_FULL;
		}
	}
#endif

	//视频关键帧，则需要写入索引
	if ((FRAME_TYPE_VIDEO == frame.frameType) && frame.keyFrame && m_bNeedWriteKey)
	{
		//写失败，则把当前写入数据也看着失败
		if (WRITE_FRAME_FAIL == WriteIndex(m_videoNum, m_frameInfo.nextPos-frame.length-sizeof(REC_FRAME_INFO), frame.time))
		{			
			return WRITE_FRAME_FAIL;
		}
	}

	if (FRAME_TYPE_VIDEO == frame.frameType)
	{
		++ m_videoNum;

		if (0 == m_startTime)	//第一个视频帧，需要记录开始时间
		{
			m_startTime = frame.time;
			m_fileHeader.startTime = m_startTime;
			if(0 != fseek(m_pFile, 0, SEEK_SET))
			{
				assert(false);
			}

			if(1 != RecFWrite(&m_fileHeader, sizeof(REC_FILE_HEADER), 1, m_pFile))
			{
				assert(false);
			}
		}
		m_endTime = frame.time;	//只更新视频的时间
	}
	else if (FRAME_TYPE_AUDIO == frame.frameType)
	{
		++ m_audioNum;
	}
	else if (FRAME_TYPE_TEXT == frame.frameType)
	{
		++ m_textNum;
	}
	return WRITE_FRAME_SUCC;
}

unsigned long CRecWriter::OnWriteOneVideo(const FRAME_INFO_EX & frame)
{
	//2010-01-25 13:48:00 YSW
	//由于采用了缓存和预录像机制，所以及时关键帧的产生有一定的时延，所以在文件写满后
	//再请求关键帧，在时延作用下，必然产生一定的丢失情况（新文件从关键帧开始读取，关键帧前面
	//的帧不能读出，相当于丢失）。但是如果能在一个文件将写满的时候，一旦发现视频关键帧，就不写入
	//该文件，且返回写入失败。外部模块则会将该关键帧先送入预录像模块进行缓存，然后打开新的文件并
	//尝试写入，这样实质上就能达到我们不丢一帧，而文件开头也是关键帧的目的。
	//另外一个限制（关键帧数目的限制），由于写入前首先检查是否为关键帧及是否已经达到最大数目，所以
	//只要是这种情况（关键帧数目满），其在关闭前送入的一帧一定是关键帧。那么也会返回文件满的值，外部处理
	//也是将该帧送入预录像缓存，并打开新文件再写入。

	//根据以上描述，在此检查当前文件中实质写入的数据数目
	if ((FRAME_TYPE_VIDEO == frame.frameType) && frame.keyFrame)
	{		

		//当把此帧写入后，将会进入到文件最后的保留区，则返回满。
		if ((m_MAX_REC_FILE_LEN - m_MAX_REC_SUB_OFF_LEN) < (m_bufInfo.filePos + sizeof(REC_FRAME_INFO) + frame.length + sizeof(REC_FRAME_INFO) + m_bufInfo.dataLen))
		{			
			return WRITE_FRAME_FULL;
		}
	}

	//将音频列表中比当前帧时间早的帧写入文件,并将这些音频从列表中清除
	{
		unsigned long ret = WRITE_FRAME_FAIL;
		while(m_audioList.GetCount() > 0)
		{
			if(m_audioList.GetHead().time < frame.time)
			{
				ret = WriteFrame(m_audioList.GetHead());
				if (WRITE_FRAME_SUCC != ret)
				{					
					return ret;
				}

				delete [] m_audioList.RemoveHead().pData;
			}
			else
			{
				break;
			}
		}
	}

	//将文本列表中比当前帧时间早的帧写入文件

	//写入视频帧
	return WriteFrame(frame);
}

void CRecWriter::FFlushEx(FILE * pFile)
{
	if (NULL == pFile)
	{
		return;
	}

	if (0 == m_lastFFlushTime)
	{
		m_lastFFlushTime = GetCurrTime32();
		return;
	}
	
	if (m_lastFFlushTime > GetCurrTime32() || 15 < (GetCurrTime32() - m_lastFFlushTime))
	{
		if (fflush(pFile))
		{
			printf("%s:%d, errno=%d\n", __FILE__, __LINE__, errno);
		}
#ifdef __ENVIRONMENT_LINUX__
		if (fsync(fileno(pFile)))
		{
			printf("%s:%d, errno=%d\n", __FILE__, __LINE__, errno);
		}
#endif
		m_lastFFlushTime = GetCurrTime32();
	}
}

size_t CRecWriter::RecFWrite(const void *ptr, size_t size, size_t nmemb, FILE *stream)
{
	if (m_bBuckUp)
	{
		timeval timeOut = {2, 0};

		FD_ZERO(&m_fdset);
		FD_SET(m_fd, &m_fdset);
		select(m_fd + 1, NULL, &m_fdset, NULL, &timeOut);
	}

#if defined(__PACKID_14__) || defined(__PACKID_16__) || defined(__PACKID_22__) || defined(__PACKID_27__)
	int wCount = 60;
	size_t ret = 0;

	do 
	{
		ret = fwrite(ptr, size, nmemb, stream);
		if (0 < ret)
		{
			break;
		}
		PUB_Sleep(1000);
	} while (--wCount);

	return ret;
#else
	return fwrite(ptr, size, nmemb, stream);
#endif
}
//end
