
//RecReader.cpp

#include "RecReader.h"
#include "fcntl.h"
#ifdef __TDFH__
#include "MemPool.h"
#endif
unsigned char MAX_READ_TRY_NUM = 1;
unsigned long REC_DATA_READ_BUF_SIZE	= 256 * 1024;

CRecReader::CRecReader ()
{
	m_pFile = NULL;
	m_pIndexInfo = NULL;

	memset(&m_bufInfo, 0, sizeof(REC_BUF_INFO));

// #if REC_READ_DO_BUFFER
// #ifdef __TDFH__
// 	m_bufInfo.pData = (unsigned char *)CMemPool::Instance()->GetMem(MEM_REC_READ, REC_DATA_READ_BUF_SIZE);//
// #else
// 	m_bufInfo.pData = new unsigned char [REC_DATA_READ_BUF_SIZE];//
// #endif
// 
// 	
// 	memset(m_bufInfo.pData, 0, REC_DATA_READ_BUF_SIZE);
// 	m_bufInfo.bufLen = REC_DATA_READ_BUF_SIZE;
// #endif
}

CRecReader::~CRecReader ()
{
#if REC_READ_DO_BUFFER
#ifdef __TDFH__
	CMemPool::Instance()->ReleaseMem(m_bufInfo.pData);
#else
	if (NULL != m_bufInfo.pData)
	{
		delete [] m_bufInfo.pData;
	}
	memset(&m_bufInfo, 0, sizeof(REC_BUF_INFO));
#endif
#endif
}

bool CRecReader::Open(const char * pFilePath,FILE_BUF_TYPE fileType, bool bForBackup)
{
	assert(m_pFile == NULL);
	 
#if REC_READ_DO_BUFFER
	if (NULL == m_bufInfo.pData)
	{
#ifdef __TDFH__
		m_bufInfo.pData = (unsigned char *)CMemPool::Instance()->GetMem(MEM_REC_READ, REC_DATA_READ_BUF_SIZE);//
#else
		m_bufInfo.pData = new unsigned char [REC_DATA_READ_BUF_SIZE];//
#endif
		memset(m_bufInfo.pData, 0, REC_DATA_READ_BUF_SIZE);
		m_bufInfo.bufLen = REC_DATA_READ_BUF_SIZE;
	}
	else
	{
		memset(m_bufInfo.pData, 0, m_bufInfo.bufLen);
	}
#endif
	unsigned int cnt=0;
	while(1)
	{
		m_pFile = fopen(pFilePath, "rb");
		if(m_pFile == NULL)
		{
			return false;
		}
		if(0 == fileno(m_pFile))
		{
			cnt++;
			fclose(m_pFile);
			m_pFile = NULL; 
			int fd0=open("/dev/null",O_RDWR);
			if(cnt >=2)
			 	return false;				
		}
		else
		{
			break;
		}
	}

	//读取文件头信息
	if(0 != fseek(m_pFile, 0, SEEK_SET))
	{
		fclose(m_pFile);
		m_pFile = NULL;
		return false;
	}

	if(1 != fread(&m_fileHeader, sizeof(REC_FILE_HEADER), 1, m_pFile))
	{
		fclose(m_pFile);
		m_pFile = NULL;
		return false;
	}

	//判断文件是否为录像文件
	/*if((m_fileHeader.fileFlag != FILE_FLAG) || (0 == m_fileHeader.indexNum) || (0 == m_fileHeader.videoNum))*/
	if(m_fileHeader.fileFlag != FILE_FLAG)
	{
		fclose(m_pFile);
		m_pFile = NULL;
		return false;
	}

	//判断文件是否已写完
	if(m_fileHeader.writeFlg == WRITE_ING_FLAG)	//文件未写结束
	{
		m_bFileNormal = false;
	}
	else
	{
		m_bFileNormal = true;
	}

	m_bufInfo.filePos = m_fileHeader.dataAddr;
	m_bufInfo.currPos = 0;
	m_bufInfo.dataLen = 0;  //保证每次新打开录像文件时，清空以前的缓存数据。

	//抽取所需要的文件信息
	if(!ExtractInfoFromFile())
	{
		fclose(m_pFile);
		m_pFile = NULL;
		return false;
	}

	if(0 != RECFseek(m_pFile, m_fileHeader.dataAddr, SEEK_SET))
	{
		fclose(m_pFile);
		m_pFile = NULL;
		return false;
	}

	PrintfBit64(m_fileHeader.startTime,false);
	PrintfBit64(m_fileHeader.endTime,false);

	//读取第一帧数据信息
	if(SeekVideoFrame(SEEK_FIRST_KEY) != ERR_READ_FILE_OK)
	{
		delete [] m_pIndexInfo;
		m_pIndexInfo = NULL;
		fclose(m_pFile);
		m_pFile = NULL;

		return false;
	}

	return true;
}

bool CRecReader::Close()
{
	if(m_pFile == NULL)
	{
		return true;
	}

	fclose(m_pFile);
	m_pFile = NULL;

	if (NULL != m_pIndexInfo)
	{
		delete [] m_pIndexInfo;
	}
	m_pIndexInfo = NULL;


	if (NULL != m_bufInfo.pData)
	{
		delete [] m_bufInfo.pData;
	}
	m_bufInfo.pData = NULL;

	return true;
}

unsigned long CRecReader::GetFrameNum(FRAME_TYPE type)
{
	if(type == FRAME_TYPE_VIDEO)
	{
		return m_fileHeader.videoNum;
	}
	else if(type == FRAME_TYPE_AUDIO)
	{
		return m_fileHeader.audioNum;
	}
	else if(type == FRAME_TYPE_TEXT)
	{
		return m_fileHeader.textNum;
	}
	else
	{
		assert(false);
		return 0;
	}
}

unsigned long CRecReader::GetVideoInfo(BITMAPINFOHEADER *pBitmapInfo)
{
	assert(pBitmapInfo != NULL);

	if(m_pFile == NULL)
	{
		return ERR_READ_FILE_NO_OPEN;
	}
	memcpy(pBitmapInfo, &m_bitmapInfo, sizeof(BITMAPINFOHEADER));
	return ERR_READ_FILE_OK;
}

unsigned long CRecReader::GetAudioInfo(WAVEFORMATEX *pWaveInfo)
{
	assert(pWaveInfo != NULL);

	if(m_pFile == NULL)
	{
		return ERR_READ_FILE_NO_OPEN;
	}

	if(m_waveInfo.nAvgBytesPerSec == 0)
	{
		return ERR_READ_FILE_NO_AUDIO;
	}
	memcpy(pWaveInfo, &m_waveInfo, sizeof(WAVEFORMATEX));
	return ERR_READ_FILE_OK;
}

void CRecReader::GetTimeRange(unsigned long &startTime, unsigned long &endTime)
{
	startTime = static_cast<unsigned long>(m_fileHeader.startTime/1000000);
	endTime = static_cast<unsigned long>(m_fileHeader.endTime/1000000);
}

FRAME_TYPE CRecReader::GetCurrFrameType()
{
	FRAME_TYPE type;
	switch(m_curFrame.flag)
	{
	case I_VIDEO_FLAG:
	case B_VIDEO_FLAG:
		type = FRAME_TYPE_VIDEO;
		break;
	case I_AUDIO_FLAG:
		type = FRAME_TYPE_AUDIO;
		break;
	case I_TEXT_FLAG:
		type = FRAME_TYPE_TEXT;
		break;
	default:
		type = FRAME_TYPE_NONE;	
		assert(false);
	}

	return type;
}

/******************************************************************************
函数功能：读取当前帧数据
参数说明：frameInfo在作为传入参数时，其pData字段要指向存放数据的缓冲区，
		  length字段为pData字段所指向缓冲区长度
返回值:	成功返回：ERR_READ_FILE_OK
		失败返回：ERR_READ_FILE_READ_FAILED（读取失败）
				  ERR_READ_FILE_BUF_SMALL(frameInfo字段中的pData所指向缓冲区空间太小)
******************************************************************************/
unsigned long CRecReader::GetCurrFrame(FRAME_INFO_EX & frameInfo)
{
	assert(m_curFrame.flag != 0);
	assert(frameInfo.pData != NULL);

	frameInfo.frameType = GetCurrFrameType();
	/*if(frameInfo.length < m_curFrame.length)
	{
		assert(false);
		return ERR_READ_FILE_BUF_SMALL;
	}*/
	frameInfo.length = m_curFrame.length;
	frameInfo.time = m_curFrame.time;
	if(m_curFrame.flag == B_VIDEO_FLAG)
	{
		frameInfo.keyFrame = 0;
	}
	else
	{
		frameInfo.keyFrame = 1;
	}

	if(1 != RECFread(frameInfo.pData, frameInfo.length, 1, m_pFile))
	{
		return ERR_READ_FILE_READ_FAILED;
	}

	return ERR_READ_FILE_OK;
}

/******************************************************************************
函数功能:在文件中定位到指定的视频帧
参数说明:mode--定位模式(SEEK_NEXT,SEEK_PREV,SEEK_NEXT_KEY,SEEK_PREV_KEY,SEEK_FIRST_KEY,SEEK_LAST_KEY)
		 seekNum--定位帧数(只在跳关键帧时才用)
返回值	:定位成功返回 ERR_READ_FILE_OK
		 定位失败返回 ERR_READ_FILE_NO_OPEN(文件没打开)
					  ERR_READ_FILE_SEEK_TO_END(文件已到达最后)
					  ERR_READ_FILE_SEEK_TO_BEGIN(文件已到达开始)
					  ERR_READ_FILE_SEEK_FAILED(文件定位失败)
					  ERR_READ_FILE_READ_FAILED(读取帧信息失败)
******************************************************************************/

unsigned long CRecReader::SeekVideoFrame(SEEK_MODE mode, unsigned long seekNum /* = 1 */)
{
	if(m_pFile == NULL)
	{
		return ERR_READ_FILE_NO_OPEN;
	}

	m_curFrame.flag = 0;
	if(mode == SEEK_NEXT_VFRM)
	{
		unsigned long ret = ERR_READ_FILE_READ_FAILED;
		while((m_curFrame.flag != I_VIDEO_FLAG) && (m_curFrame.flag != B_VIDEO_FLAG))
		{
			unsigned char tryNum = 0;
			do 
			{
#if REC_READ_DO_BUFFER
				//表示正在写的文件，且第一次读取不成功。且是有读取缓存才有效。
				if (0 < tryNum)
				{
					if (0 == fseek(m_pFile, m_bufInfo.filePos, SEEK_SET))
					{
						unsigned long len = fread(m_bufInfo.pData, 1, m_bufInfo.bufLen, m_pFile);
						if ((0 < len) && (len <= m_bufInfo.bufLen))
						{
							//成功，则应该更新一下数据长度。
							m_bufInfo.dataLen = len;
						}
						else
						{
							return ERR_READ_FILE_READ_FAILED;
						}
					}
					else
					{
						return ERR_READ_FILE_SEEK_FAILED;
					}
				}
#endif

				//找到下一帧的位置
				if(0 != RECFseek(m_pFile, m_curFrame.nextPos, SEEK_SET))
				{
					return ERR_READ_FILE_SEEK_FAILED;
				}

				//读取数据标识
				if(1 != RECFread(&m_curFrame.flag, sizeof(m_curFrame.flag), 1, m_pFile))
				{
					return ERR_READ_FILE_READ_FAILED;
				}

				//判断数据标识的合法性
				if(IsValidDataFlag(m_curFrame.flag))
				{
					//定位并读取下一帧的帧信息
					if(0 != RECFseek(m_pFile, m_curFrame.nextPos, SEEK_SET))
					{
						return ERR_READ_FILE_SEEK_FAILED;
					}

					if(1 != RECFread(&m_curFrame, sizeof(m_curFrame), 1, m_pFile))
					{
						return ERR_READ_FILE_READ_FAILED;
					}

					//再次检查数据标识的合法性
					if(IsValidDataFlag(m_curFrame.flag))
					{
						//读取下一帧信息正确，则退出循环。
						ret = ERR_READ_FILE_OK;
						break;
					}
					else//标记不正确
					{
						ret = ERR_READ_FILE_SEEK_TO_END;
					}
				}
				else
				{
					//表示标记不正确，如果是正在写的文件，则再尝试一次。
					ret = ERR_READ_FILE_SEEK_TO_END;
				}

				++ tryNum;
			} while(!m_bFileNormal && (tryNum <= MAX_READ_TRY_NUM));
		}

		return ret;
	}
	else if(mode == SEEK_PREV_VFRM)
	{
		unsigned long ret = ERR_READ_FILE_READ_FAILED;
		while((m_curFrame.flag != I_VIDEO_FLAG) && (m_curFrame.flag != B_VIDEO_FLAG))
		{
			unsigned char tryNum = 0;
			do 
			{
#if REC_READ_DO_BUFFER
				//表示正在写的文件，且第一次读取不成功。且是有读取缓存才有效。
				if (0 < tryNum)
				{
					if (0 == fseek(m_pFile, m_bufInfo.filePos, SEEK_SET))
					{
						unsigned long len = fread(m_bufInfo.pData, 1, m_bufInfo.bufLen, m_pFile);
						if ((0 < len) && (len <= m_bufInfo.bufLen))
						{
							//成功，则应该更新一下数据长度。
							m_bufInfo.dataLen = len;
						}
						else
						{
							return ERR_READ_FILE_READ_FAILED;
						}
					}
					else
					{
						return ERR_READ_FILE_SEEK_FAILED;
					}
				}
#endif

				//找到下一帧的位置
				if(0 != RECFseek(m_pFile, m_curFrame.prevPos, SEEK_SET))
				{
					return ERR_READ_FILE_SEEK_FAILED;
				}

				//读取数据标识
				if(1 != RECFread(&m_curFrame.flag, sizeof(m_curFrame.flag), 1, m_pFile))
				{
					return ERR_READ_FILE_READ_FAILED;
				}

				//判断数据标识的合法性
				if(IsValidDataFlag(m_curFrame.flag))
				{
					//定位并读取下一帧的帧信息
					if(0 != RECFseek(m_pFile, m_curFrame.prevPos, SEEK_SET))
					{
						return ERR_READ_FILE_SEEK_FAILED;
					}

					if(1 != RECFread(&m_curFrame, sizeof(m_curFrame), 1, m_pFile))
					{
						return ERR_READ_FILE_READ_FAILED;
					}

					//再次检查数据标识的合法性
					if(IsValidDataFlag(m_curFrame.flag))
					{
						//读取下一帧信息正确，则退出循环。
						ret = ERR_READ_FILE_OK;
						break;
					}
					else//标记不正确
					{
						ret = ERR_READ_FILE_SEEK_TO_BEGIN;
					}
				}
				else
				{
					//表示标记不正确，如果是正在写的文件，则再尝试一次。
					ret = ERR_READ_FILE_SEEK_TO_BEGIN;
				}

				++ tryNum;
			} while(!m_bFileNormal && (tryNum <= MAX_READ_TRY_NUM));
		}

		return ret;
	}
	else	//关键帧定位
	{
		bool bContinue = false;
		unsigned long ret = ERR_READ_FILE_READ_FAILED;
		unsigned char tryNum = 0;
		do 
		{
#if REC_READ_DO_BUFFER
			if (0 < tryNum)
			{
				if (!ExtractInfoFromFile())
				{
					return ERR_READ_FILE_READ_FAILED;
				}
			}
#endif

			bContinue = false;
			if(mode == SEEK_NEXT_KEY)
			{
				if(m_pIndexInfo[m_curIndex].position >= m_curFrame.nextPos)
				{
					while((--m_curIndex >= 0) && (m_pIndexInfo[m_curIndex].position >= m_curFrame.nextPos))
					{
						;
					}

					assert(m_curIndex >= 0);
					m_curIndex += seekNum;
				}
				else
				{
					while((++m_curIndex < m_fileHeader.indexNum) && (m_pIndexInfo[m_curIndex].position < m_curFrame.nextPos))
					{
						;
					}

					if(m_curIndex == m_fileHeader.indexNum)
					{
						bContinue = true;
						ret = ERR_READ_FILE_SEEK_TO_END;
					}
					else if((m_curIndex - 1 + seekNum) >= m_fileHeader.indexNum)
					{
						m_curIndex = m_fileHeader.indexNum - 1;
					}
					else
					{
						m_curIndex -= 1;
						m_curIndex += seekNum;
					}
				}
			}
			else if(mode == SEEK_PREV_KEY)
			{
				//后紧邻视频关键帧索引 - seekNum
				if(m_pIndexInfo[m_curIndex].position > m_curFrame.prevPos)
				{
					while((--m_curIndex >= 0) && (m_pIndexInfo[m_curIndex].position > m_curFrame.prevPos))
					{
						;
					}

					if(m_curIndex == -1)
					{
						bContinue = true;
						ret = ERR_READ_FILE_SEEK_TO_BEGIN;
					}
					else if((m_curIndex + 1) < seekNum)
					{
						m_curIndex = 0;
					}
					else
					{
						m_curIndex += 1;
						m_curIndex -= seekNum;
					}
				}
				else
				{
					while((++m_curIndex < m_fileHeader.indexNum) && (m_pIndexInfo[m_curIndex].position <= m_curFrame.prevPos))
					{
						;
					}

					assert(m_curIndex <= m_fileHeader.indexNum);
					if (m_curIndex < seekNum)
					{
						m_curIndex = 0;
					}
					else
					{
						m_curIndex -= seekNum;
					}
				}
			}
			else if(mode == SEEK_FIRST_KEY)
			{
				m_curIndex = 0;
			}
			else if(mode == SEEK_LAST_KEY)
			{
				m_curIndex = m_fileHeader.indexNum-1;
			}
			else
			{
				assert(false);
				return ERR_READ_FILE_PARAM_INVALID;
			}

			++ tryNum;

		} while(bContinue && !m_bFileNormal && (tryNum < MAX_READ_TRY_NUM));
		
		//如果bContinue为真，则意味着上面定位不成功（如果要尝试重新打开也已经尝试了最大次数），
		//那么再次就应该返回其值，即定位到了文件头或者文件尾了。其他情况则意味着其是成功的。
		if (bContinue)
		{
			return ret;
		}

		tryNum = 0;
		do 
		{
#if REC_READ_DO_BUFFER
			//表示正在写的文件，且第一次读取不成功。且是有读取缓存才有效。
			if (0 < tryNum)
			{
				if (0 == fseek(m_pFile, m_bufInfo.filePos, SEEK_SET))
				{
					unsigned long len = fread(m_bufInfo.pData, 1, m_bufInfo.bufLen, m_pFile);
					if ((0 < len) && (len <= m_bufInfo.bufLen))
					{
						//成功，则应该更新一下数据长度。
						m_bufInfo.dataLen = len;
					}
					else
					{
						return ERR_READ_FILE_READ_FAILED;
					}
				}
				else
				{
					return ERR_READ_FILE_SEEK_FAILED;
				}
			}
#endif
			if(0 != RECFseek(m_pFile, m_pIndexInfo[m_curIndex].position, SEEK_SET))
			{
				return ERR_READ_FILE_SEEK_FAILED;
			}

			if(1 != RECFread(&m_curFrame, m_fileHeader.frameInfoLen, 1, m_pFile))
			{
				return ERR_READ_FILE_READ_FAILED;
			}

			if(IsValidDataFlag(m_curFrame.flag))
			{
				return ERR_READ_FILE_OK;
			}

			++ tryNum;
		} while(!m_bFileNormal && (tryNum <= MAX_READ_TRY_NUM));

		return ERR_READ_FILE_READ_FAILED;
	}
}

/******************************************************************************
函数功能:在文件中定位到下一帧
参数说明:
返回值	:定位成功返回 ERR_READ_FILE_OK
		 定位失败返回 ERR_READ_FILE_NO_OPEN(文件没打开)
					  ERR_READ_FILE_SEEK_TO_END(文件已到达最后)
					  ERR_READ_FILE_SEEK_TO_BEGIN(文件已到达开始)
					  ERR_READ_FILE_SEEK_FAILED(文件定位失败)
					  ERR_READ_FILE_READ_FAILED(读取帧信息失败)
******************************************************************************/
unsigned long CRecReader::SeekNextFrame()
{
	if(m_pFile == NULL)
	{
		return ERR_READ_FILE_NO_OPEN;
	}

	unsigned char tryNum = 0;
	do 
	{
#if REC_READ_DO_BUFFER
		//表示正在写的文件，且第一次读取不成功。且是有读取缓存才有效。
		if (0 < tryNum)
		{
			if (0 == fseek(m_pFile, m_bufInfo.filePos, SEEK_SET))
			{
				unsigned long len = fread(m_bufInfo.pData, 1, m_bufInfo.bufLen, m_pFile);
				if ((0 < len) && (len <= m_bufInfo.bufLen))
				{
					//成功，则应该更新一下数据长度。
					m_bufInfo.dataLen = len;
				}
				else
				{
					return ERR_READ_FILE_READ_FAILED;
				}
			}
			else
			{
				return ERR_READ_FILE_SEEK_FAILED;
			}
		}
#endif

		//找到下一帧的位置
		if(0 != RECFseek(m_pFile, m_curFrame.nextPos, SEEK_SET))
		{
			return ERR_READ_FILE_SEEK_FAILED;
		}

		//读取数据标识
		if(1 != RECFread(&m_curFrame.flag, sizeof(m_curFrame.flag), 1, m_pFile))
		{
			return ERR_READ_FILE_READ_FAILED;
		}

		//判断数据标识的合法性
		if(IsValidDataFlag(m_curFrame.flag))
		{
			//定位并读取下一帧的帧信息
			if(0 != RECFseek(m_pFile, m_curFrame.nextPos, SEEK_SET))
			{
				return ERR_READ_FILE_SEEK_FAILED;
			}

			if(1 != RECFread(&m_curFrame, sizeof(m_curFrame), 1, m_pFile))
			{
				return ERR_READ_FILE_READ_FAILED;
			}

			//再次检查数据标识的合法性
			if(IsValidDataFlag(m_curFrame.flag))
			{
				//正确，则返回成功。
				return ERR_READ_FILE_OK;
			}
			//else 表示标记不正确，如果是正在写的文件，则再尝试一次。
		}
		//else 表示标记不正确，如果是正在写的文件，则再尝试一次。

		++ tryNum;
	} while(!m_bFileNormal && (tryNum <= MAX_READ_TRY_NUM));

	//到这里意味数据不正常，或者是到了数据尾部了。
	return ERR_READ_FILE_SEEK_TO_END;
}

/******************************************************************************
函数功能:在文件中定位到指定时间的紧前关键帧
参数说明:
返回值	:定位成功返回 ERR_READ_FILE_OK
		 定位失败返回 ERR_READ_FILE_NO_OPEN(文件没打开)
					  ERR_READ_FILE_SEEK_TO_END(文件已到达最后)
					  ERR_READ_FILE_SEEK_TO_BEGIN(文件已到达开始)
					  ERR_READ_FILE_SEEK_FAILED(文件定位失败)
					  ERR_READ_FILE_READ_FAILED(读取帧信息失败)
					  ERR_READ_FILE_EXCEPTION(文件异常)
******************************************************************************/
unsigned long CRecReader::SeekKeyFrameToTime(LONGLONG time)
{
	unsigned long index = 0;
	unsigned long ret = 0;
	if(m_pFile == NULL)
	{
		return ERR_READ_FILE_NO_OPEN;
	}
	
	unsigned char tryNum = 0;
	do 
	{
#if REC_READ_DO_BUFFER
		if (0 < tryNum)
		{
			if (!ExtractInfoFromFile())
			{
				return ERR_READ_FILE_READ_FAILED;
			}
		}
#endif

		if(time < m_fileHeader.startTime)
		{
			index = 0;
			ret = ERR_READ_FILE_SEEK_TO_BEGIN;
		}
		else if(time > m_fileHeader.endTime)
		{
			index = m_fileHeader.indexNum-1;
			ret = ERR_READ_FILE_SEEK_TO_END;		
		}
		else
		{
			LONGLONG seekTime = time;
			LONGLONG startTime = m_pIndexInfo[0].time;
			LONGLONG endTime = m_pIndexInfo[m_fileHeader.indexNum-1].time;
			unsigned long startIndex = 0;
			unsigned long endIndex = (m_fileHeader.indexNum-1);

			/* 下面使用这个公式和二分查找的目的是：在文件中定位到指定时间的紧前关键帧。
			下面这个公式在时间没有跨段的录像文件里是合适的。
			但是在时间出现跳跃的录像文件里，就有可能定位到指定时间的第n个关键帧.n>=1
			所有修改下面的做法。直接采用二分查找，不用公式。
			//一般情况下，关键帧数量与所跨时间近似成线性关系，此处利用线性公式由时间求得索引序号
			//
			//							  (要定位到的时间 - 开始索引时间) * (结束索引序号 - 开始索引序号)
			// 索引序号 = 开始索引序号 + —————————————————————————————————
			//									     (结束索引时间 - 开始索引时间)
			//
			//由于只是近似成线性关系，所以可能一次并不能定位到指定关键帧，所以要迭代，在迭代时，又结合了
			//二分法，可以使查找速度更快些
			
			while(((index+1) < (m_fileHeader.indexNum-1)) && (m_pIndexInfo[index+1].time < time))
			{
				unsigned long inc = (unsigned long)((seekTime - startTime) * (endIndex - startIndex) / (endTime - startTime));
				if (0 == inc)
				{
					index += 1;
					break;
				}

				index = startIndex + inc;

				if(m_pIndexInfo[index].time < time)
				{
					startIndex = index;
					startTime = m_pIndexInfo[startIndex].time;
					if(m_pIndexInfo[(startIndex+endIndex)/2].time > time)
					{
						endIndex = (startIndex+endIndex)/2;
						endTime = m_pIndexInfo[endIndex].time;
					}
				}
				else
				{
					endIndex = index;
					endTime = m_pIndexInfo[endIndex].time;
					if(m_pIndexInfo[(startIndex+endIndex)/2].time < time)
					{
						startIndex = (startIndex+endIndex)/2;
						startTime = m_pIndexInfo[startIndex].time;
					}
					index = startIndex;
				}
			}
			*/

			if (time <= startTime)
			{
				index = 0;
			}
			else if (time >= endTime)
			{
				index = m_fileHeader.indexNum-1;
			}
			else
			{
				//下面这个算法，要定位的time，只能在最大值和最小值之间，否则，会定位不准。
				while (startIndex <= endIndex)
				{
					index = ((startIndex + endIndex)>>1);
					if (startIndex == index)
					{
						break;
					}

					if (m_pIndexInfo[index].time <= time)
					{
						startIndex = index;
					}
					else
					{
						endIndex = index;
					}
				}

				//到此处，我们找到指定时间的紧前关键帧。
				assert(index < m_fileHeader.indexNum);
				
				//如果紧接指定时间之后的关键帧更合理的化，就使用紧接指定时间之后的关键帧。
				if ((m_pIndexInfo[index].time < time) && ((index + 1) < m_fileHeader.indexNum))
				{
					LONGLONG d1 = time - m_pIndexInfo[index].time;
					LONGLONG d2 = m_pIndexInfo[index + 1].time - time;
					if (d1 > 300000000)
					{
						//如果指定时间与紧前关键帧的时间相差大于5分钟，就说明录像文件出现
						//了时间跳跃现象，使用紧接指定时间之后的关键帧
						//printf("(%s,%d)SeekKeyFrameToTime: file is not serial.use next frame\n",__FILE__,__LINE__);
						index += 1;
					}
					else if ((m_pIndexInfo[index + 1].time)/1000000 == (time / 1000000))
					{
						//如果指定时间与紧接指定时间之后的关键帧的时间，在秒单位上相等，
						//就使用紧接指定时间之后的关键帧
						//printf("(%s,%d)SeekKeyFrameToTime: next frame time (s) == seek time (s)\n",__FILE__,__LINE__);
						index += 1;
					}
					else if (d2 < d1)
					{
						//如果指定时间与紧接指定时间之后的关键帧的时间比较接近的化，
						//就使用紧接指定时间之后的关键帧
						//printf("(%s,%d)SeekKeyFrameToTime: next frame time is near seek time (s)\n",__FILE__,__LINE__);
						index += 1;
					}
				}
			}
			ret = ERR_READ_FILE_OK;
		}

		++ tryNum;
	} while(!m_bFileNormal && (tryNum <= MAX_READ_TRY_NUM));


	tryNum = 0;
	m_cutSize=0;
	do 
	{
#if REC_READ_DO_BUFFER
		if (0 < tryNum)
		{
			if (0 == fseek(m_pFile, m_pIndexInfo[index].position, SEEK_SET))
			{
				unsigned long len = fread(m_bufInfo.pData, 1, m_bufInfo.bufLen, m_pFile);
				if ((0 < len) && (len <= m_bufInfo.bufLen))
				{
					//成功，则应该更新一下数据长度。
					m_bufInfo.filePos	= m_pIndexInfo[index].position;
					m_bufInfo.currPos	= 0;
					m_bufInfo.dataLen	= len;
				}
				else
				{
					return ERR_READ_FILE_READ_FAILED;
				}
			}
			else
			{
				return ERR_READ_FILE_SEEK_FAILED;
			}
		}
#endif

		if(0 != RECFseek(m_pFile, m_pIndexInfo[index].position, SEEK_SET))
		{
			m_cutSize=0;
			return ERR_READ_FILE_SEEK_FAILED;
		}

		memset(&m_curFrame, 0, m_fileHeader.frameInfoLen);
		if(1 != RECFread(&m_curFrame, m_fileHeader.frameInfoLen, 1, m_pFile))
		{
			m_cutSize=0;
			return ERR_READ_FILE_READ_FAILED;
		}

		if(IsValidDataFlag(m_curFrame.flag))
		{
			m_cutSize= m_pIndexInfo[index].position;
			return ret;
		}

		++ tryNum;
	} while(!m_bFileNormal && (0 < tryNum));

	return ERR_READ_FILE_EXCEPTION;	//成功
}

#if 0
//该方法目前没有使用，且有问题。比如没有检查该文件是否已经被写结束，然后再加载除索引
//之外的信息。所以暂时屏蔽，可以使用ExtractInfoFromFile方法代替。
/******************************************************************************
函数功能：对正在写的文件重读文件索引区，刷新读模块实例中读入的该文件的帧索引数据

******************************************************************************/
bool CRecReader::RefreshIndex()
{
	//如果文件打开的时候就已经是写完整的文件，则不应该刷新。
	if(m_bFileNormal)
	{
		return false;
	}

	if(0 != fseek(m_pFile, WRITE_FLAG_POS, SEEK_SET))
	{
		return false;
	}

	unsigned long writeFlag;
	if(1 != fread(&writeFlag, sizeof(long), 1, m_pFile))
	{
		return false;
	}

	//文件在读的过程中已经写结束,则重新读入文件信息头和索引信息
	if(writeFlag == WRITE_END_FLAG)
	{
		if(0 != fseek(m_pFile, m_fileHeader.indexAddr, SEEK_SET))
		{
			return false;
		}

		//只读取上次未读取到的帧索引
		if(1 != fread(m_pIndexInfo + m_fileHeader.indexNum, sizeof(INDEX_INFO)*(MAX_KEY_FRAME_NUM - m_fileHeader.indexNum), 1, m_pFile))
		{
			return false;
		}

		if(0 != fseek(m_pFile, 0, SEEK_SET))
		{
			return false;
		}

		if(1 != fread(&m_fileHeader, sizeof(REC_FILE_HEADER), 1, m_pFile))
		{
			return false;
		}
	}
	else
	{
		if(0 != fseek(m_pFile, m_fileHeader.indexAddr, SEEK_SET))
		{
			return false;
		}

		//只读取上次未读取到的帧索引
		if(1 != fread(m_pIndexInfo + m_fileHeader.indexNum, sizeof(INDEX_INFO)*(MAX_KEY_FRAME_NUM - m_fileHeader.indexNum), 1, m_pFile))
		{
			return false;
		}

		int i = 0;
		for(i = 0; i < MAX_KEY_FRAME_NUM; i++)
		{
			if(m_pIndexInfo[i].flag != I_VIDEO_FLAG)
			{
				break;
			}
		}

		m_fileHeader.indexNum = i;
		m_fileHeader.videoNum = m_pIndexInfo[m_fileHeader.indexNum-1].index+1;
	}

	if(0 != fseek(m_pFile, m_curFrame.nextPos - m_curFrame.length, SEEK_SET))
	{
		return false;
	}

	return true;
}
#endif

/******************************************************************************
函数功能：修复未写完整的录像文件
实现说明：先统计索引帧数。读取出索引区的所有数据，根据标识段判别可统计得所有的索引数据，
		  对音频不作统计，只是在根据音频头判断得有音频时，就让音频总数等于一个大于零的数。
		  对视频作统计，从最后一帧索引对应的数据帧位置开始，顺序读取帧信息，遇到
		  视频帧就对视频帧数加1并且将该帧时间作为文件的结束时间，直到读到的一帧标识
		  为非法标识时，宣布文件读到结尾，此时，令文件头中的文件长度=最后一帧的下一帧位置
		  开始时间等于第一关键帧时间，把文件头信息重写入文件头位置。
******************************************************************************/
bool CRecReader::Repair(const char *pFilePath, LONGLONG *pEndTime)
{
	FILE *pFile = NULL;
	REC_FILE_HEADER fileHeader;
	WAVEFORMATEX waveInfo;
	INDEX_INFO *pIndexInfo = NULL;
	REC_FRAME_INFO frameInfo;
	unsigned long max_key_frame_num = 0;

	*pEndTime = 0;

	pFile = fopen(pFilePath, "rb+");
	if(pFile == NULL)
	{
		return false;
	}

	if(0 != fseek(pFile, 0, SEEK_SET))
	{
		goto ERROR_PROC;
	}

	if(1 != fread(&fileHeader, sizeof(REC_FILE_HEADER), 1, pFile))
	{
		goto ERROR_PROC;
	}

	if(fileHeader.fileFlag != FILE_FLAG)
	{
		goto ERROR_PROC;
	}

	if((fileHeader.writeFlg == WRITE_END_FLAG) && (0 < fileHeader.startTime) && (fileHeader.startTime < GetCurrTime()) && (fileHeader.startTime < fileHeader.endTime))
	{
		*pEndTime = fileHeader.endTime;
		fclose(pFile);
		return true;
	}

	if(0 != fseek(pFile, fileHeader.audioFormatAddr, SEEK_SET))
	{
		goto ERROR_PROC;
	}

	if(1 != fread(&waveInfo, sizeof(waveInfo), 1, pFile))
	{
		goto ERROR_PROC;
	}

	if(0 != fseek(pFile, fileHeader.indexAddr, SEEK_SET))
	{
		goto ERROR_PROC;
	}

	max_key_frame_num = (fileHeader.videoFormatAddr - sizeof (REC_FILE_HEADER) - 32) / sizeof(INDEX_INFO);

	pIndexInfo = new INDEX_INFO[max_key_frame_num];
	if(pIndexInfo == NULL)
	{
		goto ERROR_PROC;
	}
	if(1 != fread(pIndexInfo, sizeof(INDEX_INFO) * max_key_frame_num, 1, pFile))
	{
		goto ERROR_PROC;
	}

	fileHeader.audioNum = (waveInfo.nAvgBytesPerSec == 0) ? 0 : 0x0fffffff;

	//统计索引帧数
	for(fileHeader.indexNum = 0; fileHeader.indexNum < max_key_frame_num; fileHeader.indexNum++)
	{
		if(pIndexInfo[fileHeader.indexNum].flag != I_VIDEO_FLAG)
		{
			break;
		}
	}

	if(fileHeader.indexNum == 0)
	{
		goto ERROR_PROC;
	}

	if (0 == fileHeader.startTime)
	{
		fileHeader.startTime = pIndexInfo[0].time;	//置文件的开始时间为第一帧时间
	}
	else
	{

	}

	//从最后一个帧索引对应的帧数据位置开始检查后面的数据
	{
#if defined(__TDFH__) || defined(__CHIPTI__)
		REC_FRAME_INFO lastFrameInfo;
		memset(&lastFrameInfo, 0, sizeof(REC_FRAME_INFO));
#endif
		unsigned long i = fileHeader.indexNum;
		do 
		{
			fileHeader.indexNum = i;
			if (0 == i)
			{
				goto ERROR_PROC;
			}
			-- i;


			fileHeader.videoNum = pIndexInfo[i].index;
			if(0 != fseek(pFile, pIndexInfo[i].position, SEEK_SET))
			{
				goto ERROR_PROC;
			}

			int pCount = 0;
			while(1 == fread(&frameInfo, fileHeader.frameInfoLen, 1, pFile))
			{
				if(!IsValidDataFlag(frameInfo.flag))
				{
					printf("%s:%s:%d, The frame is invalid data, flag=0x%08x, i = %d, pCount=%d\n", __FUNCTION__, __FILE__, __LINE__, frameInfo.flag, i, pCount);
					//此时的时间戳是上个帧的时间戳，应当是正常时间.如果flag是0，并且这个时间不正常说明帧时间错误
					PrintfBit64(fileHeader.startTime, true);
					PrintfBit64(fileHeader.endTime, true);
					PrintfBit64(frameInfo.time, true);

					break;
				}

				if (frameInfo.time < fileHeader.startTime)
				{
					//时间错误，属于异常情况
					printf("%s:%s:%d, The frame time is < startTime, flag=0x%08x, i = %d, pCount=%d\n", __FUNCTION__, __FILE__, __LINE__, frameInfo.flag, i, pCount);
					PrintfBit64(fileHeader.startTime, true);
					PrintfBit64(fileHeader.endTime, true);
					PrintfBit64(frameInfo.time, true);

					PrintfBit64(fileHeader.startTime, false);
					PrintfBit64(fileHeader.endTime, false);
					PrintfBit64(frameInfo.time, false);

					break;
				}

#if defined(__TDFH__) || defined(__CHIPTI__)
				//新平台的timeStampFlag采用校验函数。
				//而旧平台就不采用校验了。原因是：要兼容之前的录像文件
				unsigned long frameInfoChsum = frameInfo.timeStampFlag;

				frameInfo.timeStampFlag = 0;
				frameInfo.timeStampFlag = RecFrameInfoChsum(&frameInfo);

				//printf("%s:%s:%d, check  rec frame info error!frameInfoChsum=(0x%08x 0x%08x) \n", __FUNCTION__, __FILE__, __LINE__, frameInfoChsum, frameInfo.timeStampFlag);

				if (frameInfoChsum != frameInfo.timeStampFlag)
				{
					printf("%s:%s:%d, check  rec frame info error!frameInfoChsum=(0x%08x 0x%08x) \n", __FUNCTION__, __FILE__, __LINE__, frameInfoChsum, frameInfo.timeStampFlag);
					PrintfBit64(fileHeader.startTime, true);
					PrintfBit64(fileHeader.endTime, true);
					PrintfBit64(frameInfo.time, true);
					break;
				}

				//检测这一帧的时间是否比关键帧的时间小,如果比关键帧的时间小则不正常
				//如果这一帧的时间比关键帧的时间大于10分钟以上则以为不正常
				//10分钟太小，在FH平台，有视频的录像文件，录像时间超过10分钟时异常断电，在文件修复时会发现有的文件
				//只写一两个关键帧索引。如果在写关键帧时用sync()函数来刷新数据到硬盘，也会出现只写一部分关键帧索引的通道。
				//比如录像了15分钟，有的通道只有40多个关键帧索引，而其他通道则有150个以上。FH平台120帧一个关键帧
				//所以要加大时间检测，特别时在处理视频丢失的文件时更应该加大.增加到10个小时
				if (pIndexInfo[i].time > frameInfo.time || 
					((frameInfo.time - pIndexInfo[i].time) / 1000000) > 36000)
				{
					printf("%s:%s:%d, The frame time is error! flag=0x%08x, i = %d , pCount=%d\n", __FUNCTION__, __FILE__, __LINE__, frameInfo.flag, i, pCount);
					PrintfBit64(frameInfo.time, true);
					PrintfBit64(pIndexInfo[i].time, true);

					PrintfBit64(fileHeader.startTime, true);
					PrintfBit64(fileHeader.endTime, true);
					PrintfBit64(frameInfo.time, true);
					break;
				}
#else
				if (frameInfo.timeStampFlag != 1)
				{
					printf("%s,%d, check  rec frame info error!timeStampFlag\n", __FILE__,__LINE__);
					PrintfBit64(fileHeader.startTime, false);
					PrintfBit64(fileHeader.endTime, false);
					PrintfBit64(frameInfo.time, false);
					break;
				}

				//目前单帧的长度最大是1M，
				if (frameInfo.length  > 10*1024*1024)
				{
					printf("%s,%d,frame length error:%d\n",__FILE__,__LINE__,frameInfo.length);
					break;
				}

				if((frameInfo.flag == I_VIDEO_FLAG) || (frameInfo.flag == B_VIDEO_FLAG))
				{
					if (frameInfo.time < pIndexInfo[i].time)
					{
						printf("%s,%d, frameInfo.time < pIndexInfo[%d].time,type0x%08x\n",__FILE__,__LINE__,i,frameInfo.flag);
						PrintfBit64(fileHeader.startTime, false);
						PrintfBit64(fileHeader.endTime, false);
						PrintfBit64(frameInfo.time, false);
						PrintfBit64(pIndexInfo[i].time, false);
						break;
					}

					//如果这一帧的时间比关键帧的时间大于1440分钟(24小时)以上则以为不正常
					if ((frameInfo.time - pIndexInfo[i].time)/1000000 > 86400)
					{
						printf("%s,%d, frame time >>>pIndexInfo[%d].time\n",__FILE__,__LINE__,i);
						PrintfBit64(fileHeader.startTime, false);
						PrintfBit64(fileHeader.endTime, false);
						PrintfBit64(frameInfo.time, false);
						PrintfBit64(pIndexInfo[i].time, false);
						break;
					}
				}
#endif

				fileHeader.fileLen = frameInfo.nextPos;
				if((frameInfo.flag == I_VIDEO_FLAG) || (frameInfo.flag == B_VIDEO_FLAG))
				{
					fileHeader.videoNum++;
					fileHeader.endTime = frameInfo.time;
#if defined(__TDFH__) || defined(__CHIPTI__)
					memcpy(&lastFrameInfo, &frameInfo, sizeof(REC_FRAME_INFO));
#endif
				}


				pCount++;
				if(0 != fseek(pFile, frameInfo.nextPos, SEEK_SET))
				{
					break;
				}

			}
		} while(0 == fileHeader.endTime);

#if defined(__TDFH__) || defined(__CHIPTI__)
		//FH平台，修复的数据去掉最后10帧，防止最后的数据有问题
		//定位到当前检测的正常帧的上10帧
		int dropCount = 0;
		
		while (0 != lastFrameInfo.prevPos && 10 > dropCount && lastFrameInfo.flag != I_VIDEO_FLAG)
		{
			dropCount++;
			if( 0 != fseek(pFile, lastFrameInfo.prevPos, SEEK_SET) ||
				1 != fread(&lastFrameInfo, fileHeader.frameInfoLen, 1, pFile))
			{
				goto ERROR_PROC;
			}
		}

		fileHeader.fileLen = lastFrameInfo.nextPos;
		fileHeader.endTime = lastFrameInfo.time;
		if (fileHeader.videoNum > dropCount)
		{
			fileHeader.videoNum -= dropCount;
		}
		else
		{
			goto ERROR_PROC;
		}
#endif
	}

	//把文件头信息重写入文件头位置
	fileHeader.writeFlg = WRITE_END_FLAG;
	if(0 != fseek(pFile, 0, SEEK_SET))
	{
		goto ERROR_PROC;
	}

	if(1 != fwrite(&fileHeader, sizeof(REC_FILE_HEADER), 1, pFile))
	{
		goto ERROR_PROC;
	}

	fclose(pFile);
	delete [] pIndexInfo;

	//传出参数赋值
	*pEndTime = fileHeader.endTime;

	return true;

ERROR_PROC:
	{
		fclose(pFile);

		if(pIndexInfo != NULL)
		{
			delete [] pIndexInfo;
		}
		return false;
	}
	
}

/******************************************************************************

						protected functions

******************************************************************************/

/******************************************************************************
函数功能：从文件中读取文件头信息、视频格式、音频格式及索引数据
实现说明：对已写完整的文件可直接读取到正确的索引数据、索引帧数、视频帧数及音频帧数
		  但未写完整的文件则需要统计。
		  统计的方法为：先读取出索引区的所有数据，根据标识段判别可统计得所有的索引数据，
		  然后把索引中最后一帧的帧序号加1当作当前视频数据帧数，
		  而对音频不作统计，只是在根据音频头判断得有音频时，就让音频总数等于一个大于零的数。
		  
		  不统计出音频、视频具体数目的原因是：在文件的各种读取及定位过程中，并用不到
		  这两个数据，而只是在判断文件有无音频视频数据时才用上。

******************************************************************************/
bool CRecReader::ExtractInfoFromFile()
{
	//这段代码，如果打开文件则是重复。如果是读取工程中，重新加载文件信息，则不是多余
	//因为读取过程中，可能写线程已经把该文件写结束了，应该适当地修改标志。
	{
		//读取文件头信息
		if(0 != fseek(m_pFile, 0, SEEK_SET))
		{
			return false;
		}

		if(1 != fread(&m_fileHeader, sizeof(REC_FILE_HEADER), 1, m_pFile))
		{
			return false;
		}

		//判断文件是否已写完
		if(m_fileHeader.writeFlg == WRITE_END_FLAG)	//文件未写结束
		{
			m_bFileNormal = true;
		}
		else
		{
			m_bFileNormal = false;
		}
	}

	//读取视频信息
	if(0 != fseek(m_pFile, m_fileHeader.videoFormatAddr, SEEK_SET))
	{
		return false;
	}

	if(1 != fread(&m_bitmapInfo, sizeof(BITMAPINFOHEADER), 1, m_pFile))
	{
		return false;
	}

	//读取音频信息
	if(0 != fseek(m_pFile, m_fileHeader.audioFormatAddr, SEEK_SET))
	{
		return false;
	}

	if(1 != fread(&m_waveInfo, sizeof(WAVEFORMATEX), 1, m_pFile))
	{
		return false;
	}

	unsigned long max_key_frame_num = (m_fileHeader.videoFormatAddr - sizeof (REC_FILE_HEADER) - 32) / sizeof(INDEX_INFO);

	//读取索引信息
	if(m_bFileNormal)
	{
		if(0 != fseek(m_pFile, m_fileHeader.indexAddr, SEEK_SET))
		{
			return false;
		}

		if (NULL != m_pIndexInfo)
		{
			delete [] m_pIndexInfo;
			m_pIndexInfo = NULL;
		}

		//2010-02-08 21:32:00 YSW
		//检查关键帧数目是否合法
		if ((0 < m_fileHeader.indexNum) && (m_fileHeader.indexNum <= max_key_frame_num))
		{
			m_pIndexInfo = new INDEX_INFO[m_fileHeader.indexNum];
			if(m_pIndexInfo == NULL)
			{
				assert(false);
				return false;
			}

			if(1 != fread(m_pIndexInfo, sizeof(INDEX_INFO)*m_fileHeader.indexNum, 1, m_pFile))
			{
				delete [] m_pIndexInfo;
				return false;
			}
		}
		else
		{
			return false;
		}
	}
	else
	{
		assert(0 != m_fileHeader.startTime);
		assert(0 == m_fileHeader.endTime);
		if (0 == m_fileHeader.startTime)
		{
			return false;
		}

		if(0 != fseek(m_pFile, m_fileHeader.indexAddr, SEEK_SET))
		{
			return false;
		}

		if (NULL != m_pIndexInfo)
		{
			delete [] m_pIndexInfo;
			m_pIndexInfo = NULL;
		}

		m_pIndexInfo = new INDEX_INFO[max_key_frame_num];
		if(m_pIndexInfo == NULL)
		{
			assert(false);
			return false;
		}

		if(1 != fread(m_pIndexInfo, sizeof(INDEX_INFO)*max_key_frame_num, 1, m_pFile))
		{
			delete [] m_pIndexInfo;
			return false;
		}

		int i = 0;
		for(i = 0; i < max_key_frame_num; i++)
		{
			if(m_pIndexInfo[i].flag != I_VIDEO_FLAG)
			{
				break;
			}
		}
		
		//m_fileHeader.indexNum = i;
		//m_fileHeader.videoNum = m_pIndexInfo[m_fileHeader.indexNum-1].index+1;
		if(m_waveInfo.nAvgBytesPerSec != 0)
		{
			m_fileHeader.audioNum = 0x0fffffff;
		}

		//由于recWriter有可能使用缓存写的方式：索引每次都写，数据要等缓存满后才写。
		//所以要从实际的写到硬盘里的数据，得到结束时间和关键帧数
		while (i > 0)
		{
			if(0 == fseek(m_pFile, m_pIndexInfo[i - 1].position, SEEK_SET))
			{
				m_fileHeader.videoNum = m_pIndexInfo[i - 1].index;
				
				REC_FRAME_INFO frameInfo;
				while(1 == fread(&frameInfo, m_fileHeader.frameInfoLen, 1, m_pFile))
				{
					if(!IsValidDataFlag(frameInfo.flag))
					{
						//正在写的文件，正常情况下这个地方的flag应该是0。如果这里不是0，则表示录像时没写结束信息
						printf("%s:%s:%d, The frame is invalid data, flag=0x%08x, i = %d\n", __FUNCTION__, __FILE__, __LINE__, frameInfo.flag, i);
						//此时的时间戳是上个帧的时间戳，应当是正常时间.如果flag是0，并且这个时间不正常说明帧时间错误
						PrintfBit64(m_fileHeader.startTime, true);
						PrintfBit64(m_fileHeader.endTime, true);
						PrintfBit64(frameInfo.time, true);

						break;
					}

					if (frameInfo.time < m_fileHeader.startTime)
					{
						//时间错误，属于异常情况
						printf("%s:%s:%d, The frame time is < startTime, flag=0x%08x, i = %d\n", __FUNCTION__, __FILE__, __LINE__, frameInfo.flag, i);
						PrintfBit64(m_fileHeader.startTime, true);
						PrintfBit64(m_fileHeader.endTime, true);
						PrintfBit64(frameInfo.time, true);

						break;
					}

#if defined(__TDFH__) || defined(__CHIPTI__)
					unsigned long frameInfoChsum = frameInfo.timeStampFlag;

					frameInfo.timeStampFlag = 0;
					frameInfo.timeStampFlag = RecFrameInfoChsum(&frameInfo);

					//printf("%s:%s:%d, check  rec frame info error!frameInfoChsum=(0x%08x 0x%08x) \n", __FUNCTION__, __FILE__, __LINE__, frameInfoChsum, frameInfo.timeStampFlag);

					if (frameInfoChsum != frameInfo.timeStampFlag)
					{
						printf("%s:%s:%d, check  rec frame info error!frameInfoChsum=(0x%08x 0x%08x) \n", __FUNCTION__, __FILE__, __LINE__, frameInfoChsum, frameInfo.timeStampFlag);
						PrintfBit64(m_fileHeader.startTime, true);
						PrintfBit64(m_fileHeader.endTime, true);
						PrintfBit64(frameInfo.time, true);
						break;
					}

					//检测这一帧的时间是否比关键帧的时间小,如果比关键帧的时间小则不正常
					//如果这一帧的时间比关键帧的时间大10小时以上则以为不正常
					if (m_pIndexInfo[i - 1].time > frameInfo.time || 
						((frameInfo.time - m_pIndexInfo[i-1].time) / 1000000) > 36000)
					{
						printf("%s:%s:%d, The frame time is error! flag=0x%08x, i = %d\n", __FUNCTION__, __FILE__, __LINE__, frameInfo.flag, i);
						PrintfBit64(frameInfo.time, true);
						PrintfBit64(m_pIndexInfo[i-1].time, true);

						PrintfBit64(m_fileHeader.startTime, true);
						PrintfBit64(m_fileHeader.endTime, true);
						PrintfBit64(frameInfo.time, true);
						break;
					}

#else
					if (frameInfo.timeStampFlag != 1)
					{
						printf("%s,%d, check  rec frame info error!timeStampFlag\n", __FILE__,__LINE__);
						PrintfBit64(m_fileHeader.startTime, false);
						PrintfBit64(m_fileHeader.endTime, false);
						PrintfBit64(frameInfo.time, false);
						break;
					}

					//目前单帧的长度最大是1M，
					if (frameInfo.length  > 10*1024*1024)
					{
						printf("%s,%d,frame length error:%d\n",__FILE__,__LINE__,frameInfo.length);
						break;
					}

					if((frameInfo.flag == I_VIDEO_FLAG) || (frameInfo.flag == B_VIDEO_FLAG))
					{
						if (frameInfo.time < m_pIndexInfo[i-1].time)
						{
							printf("%s,%d, frameInfo.time < m_pIndexInf[%d-1].time,type0x%08x\n",__FILE__,__LINE__,i,frameInfo.flag);
							PrintfBit64(m_fileHeader.startTime, false);
							PrintfBit64(m_fileHeader.endTime, false);
							PrintfBit64(frameInfo.time, false);
							PrintfBit64(m_pIndexInfo[i-1].time, false);
							break;
						}

						//如果这一帧的时间比关键帧的时间大于1440分钟以上则以为不正常
						if ((frameInfo.time - m_pIndexInfo[i-1].time)/1000000 > 86400)
						{
							printf("%s,%d, frame time >>>m_pIndexInfo[%d-1].time\n",__FILE__,__LINE__,i);
							PrintfBit64(m_fileHeader.startTime, false);
							PrintfBit64(m_fileHeader.endTime, false);
							PrintfBit64(frameInfo.time, false);
							PrintfBit64(m_pIndexInfo[i-1].time, false);
							break;
						}
					}

#endif

					m_fileHeader.fileLen = frameInfo.nextPos;
					if((frameInfo.flag == I_VIDEO_FLAG) || (frameInfo.flag == B_VIDEO_FLAG))
					{
						m_fileHeader.videoNum ++;
						m_fileHeader.endTime = frameInfo.time;
					}
					
					if(0 != fseek(m_pFile, frameInfo.nextPos, SEEK_SET))
					{
						break;
					}
				}
				
				if (0 != m_fileHeader.endTime)
				{
					m_fileHeader.indexNum = i;
					break;
				}
			}

			i--;
		}

		if (0 == i)
		{
			delete [] m_pIndexInfo;
			m_pIndexInfo = NULL;

			return false;
		}

		
		assert(m_fileHeader.endTime >= m_fileHeader.startTime);
	}

	return true;
}

//判断是不是正确的帧标识
bool CRecReader::IsValidDataFlag(unsigned long flag)
{
	if((flag == I_VIDEO_FLAG) || (flag == B_VIDEO_FLAG) || (flag == I_AUDIO_FLAG) || (flag == I_TEXT_FLAG))
	{
		return true;
	}
	return false;
}


/******************************************************************************

					缓冲机制处理读数据过程(利用 生产—消费 同步机制)

******************************************************************************/

int CRecReader::RECFseek(FILE *pFile, long offset, int seek)
{
#if REC_READ_DO_BUFFER
	if ((m_bufInfo.filePos <= offset) && (offset < (m_bufInfo.filePos + m_bufInfo.dataLen)))
	{
		m_bufInfo.currPos = (offset - m_bufInfo.filePos);
		return 0;
	}
	else
	{
		if (0 == fseek(pFile, offset, SEEK_SET))
		{
			unsigned long len = fread(m_bufInfo.pData, 1, m_bufInfo.bufLen, pFile);
			if ((0 < len) && (len <= m_bufInfo.bufLen))
			{
				m_bufInfo.dataLen = len;
				m_bufInfo.filePos = offset;
				m_bufInfo.currPos = 0;

				return 0;
			}
			else
			{
				return -1;
			}
		}
		else
		{
			return -1;
		}
	}
#else
	return fseek(pFile, offset, seek);
#endif
}

size_t CRecReader::RECFread(void *pBuf, size_t len, size_t count, FILE *pFile)
{
#if REC_READ_DO_BUFFER
	assert (m_bufInfo.currPos <= m_bufInfo.dataLen);
	
	size_t length = (len * count);

	if ((m_bufInfo.currPos + length) <= m_bufInfo.dataLen)
	{
		memcpy(pBuf, m_bufInfo.pData + m_bufInfo.currPos, length);
		m_bufInfo.currPos  += length;

		return count;
	}
	else
	{
		unsigned long readLen = (m_bufInfo.dataLen - m_bufInfo.currPos);
		memcpy(pBuf, m_bufInfo.pData + m_bufInfo.currPos, readLen);
		m_bufInfo.currPos  += readLen;

		while (readLen < length)
		{
			if (0 == fseek(pFile, m_bufInfo.filePos + m_bufInfo.currPos, SEEK_SET))
			{
				size_t dataLen = fread(m_bufInfo.pData, 1, m_bufInfo.bufLen, pFile);
				if (0 < dataLen)
				{
					m_bufInfo.dataLen = dataLen;
					m_bufInfo.filePos += m_bufInfo.currPos;
					m_bufInfo.currPos = 0;

					unsigned long subLen = (length - readLen);
					subLen = (subLen <= dataLen) ? subLen : dataLen;
					memcpy((unsigned char *)pBuf+readLen, m_bufInfo.pData, subLen);

					readLen += subLen;
					m_bufInfo.currPos = subLen;
				}
				else
				{
					break;
				}
			}
			else
			{
				break;
			}
		}

		return (readLen / len);
	}
#else
	return fread(pBuf, len, count, pFile);
#endif
}

unsigned long CRecReader::RECFtell()
{
	return m_cutSize;
}
