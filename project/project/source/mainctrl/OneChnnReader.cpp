//COneChnnReader.cpp
#include "MessageMan.h"
#include "OneChnnReader.h"

enum
{	
	GET_NEW_FILE_FAIL,
	GET_NEW_FILE_SUC,
	RESERVER_OLD_FILE
};
//pbulic
COneChnnReader::COneChnnReader()
{
	m_currPos		= NULL;
	m_bOpenFile		= false;
	m_bReAudioFmat	= false;
	m_bReVideoFmat	= false;
	m_channel		= 0;
	m_readerType	= FILE_TYPE_FTVT;
	m_bDynSearch	= false;

	memset(&m_bitmapInfo, 0, sizeof(BITMAPINFOHEADER));
	memset(&m_waveFormat, 0, sizeof(WAVEFORMATEX));
	memset(m_filePath, 0, sizeof(m_filePath));

	m_diskOwnerIndex = 0;
	m_bRegister = false;

	m_videoTime = 0;
}

COneChnnReader::~COneChnnReader()
{

}

bool COneChnnReader::Initial(CReclogManEX *pReclogManEx, unsigned long diskOwnerIndex)
{
	m_pReclogMan = pReclogManEx;
	m_diskOwnerIndex = diskOwnerIndex;
	m_bRegister = false;
	return true;
}

void COneChnnReader::Quit()
{
	//回放结束后要释放所有关闭掉
	assert(!m_bRegister);

}

/************************************************************************
函数原型	long StartRead(LONGLONG startTime, LONGLONG endTime, const CHANNEL_ID_MAPPER &channelMapper)
功能描述	指定mac地址,通道，指定开始时间和结束时间.如果在两时间之间搜索到文件，则打开第一个文件
			并定位到开始时间的紧前关键帧,为后续读取数据做准备.
输入参数	startTime ：查找录像的开始时间。
			endTime   : 结束时间
			channelMapper: 包含指定通道和MAC地址
输出参数	无
返回值		ONE_CHNN_READ_SUC：成功； ONE_CHNN_READ_FAIL：失败
************************************************************************/
long COneChnnReader::StartRead(unsigned long startTime, unsigned long endTime, unsigned long chnn, const char *pRecPath /* = NULL */)
{
	//保证没有开启过，或者已经关闭
	assert(m_fileList.IsEmpty());
	assert(NULL == m_currPos);
	assert(!m_bRegister);

	//目前回放只用了该接口，但是需要提供自动搜索功能，所以改为自动搜索。
	m_bDynSearch = true;

	if(pRecPath != NULL)
	{
		strcpy(m_recPath, pRecPath);
	}
	else
	{
		m_recPath[0] = '\0';
	}

	m_channel = chnn;
	//取得的文件列表已存放在m_fileList中
	if (m_pReclogMan->GetFileInfo(m_channel, startTime, m_fileList, m_diskOwnerIndex, true) > 0) 
	{
		//2010-04-14 17:53:00 YSW
		//如果文件列表的时间不包含条件时间所在天，则认为启动失败
		if ( (m_fileList.GetTail().endTime <= startTime) || \
			(endTime <= m_fileList.GetHead().startTime))
		{
			RemoveFileList();
			return ONE_CHNN_READ_FAIL;
		}

		//2010-01-25 11:29:00 YSW
		//修正开始时间再定位，因为可能选择的开始时间点没有录像数据，而我们希望能选择到最接近
		//数据开始回放。由于SeekFrameToTime中会依据startTime判断是否在文件列表的时间区间内（不再
		//则重新搜索），这样就会在启动时做了两次搜索动作。
		//因而再次先修改时间，就是如果startTime < head.StartTime，则进行修正。

		if (startTime < m_fileList.GetHead().startTime)
		{
			startTime = m_fileList.GetHead().startTime;
		}

		return SeekFrameToTime(true, startTime);
	}
	else
	{
		RemoveFileList();
		return ONE_CHNN_READ_FAIL;
	}
}

/************************************************************************
函数原型	long StartRead(const CMyList <FILE_INFO_EX> & fileList)
功能描述	根据上层指定的文件列表，打开第一个文件并定位到第一个关键帧。
			列表的文件必须同属一个通道。
输入参数	fileList ：调用者指定的文件列表。
输出参数	无
返回值	ONE_CHNN_READ_SUC：成功； ONE_CHNN_READ_FAIL：失败
************************************************************************/
long COneChnnReader::StartRead(const FILE_INFO_EX_LIST & fileList, const char *pRecPath /* = NULL */)
{
	assert(!fileList.IsEmpty());	//链表中必须有文件。
	assert(m_fileList.IsEmpty());	//保证工作目录是空的.
    assert(!m_bRegister);

	m_bDynSearch = false;
	if(pRecPath != NULL)
	{
		strcpy(m_recPath, pRecPath);
	}
	else
	{
		m_recPath[0] = '\0';
	}
	
	//把fileList拷贝一份到m_fileList中;
	POS pFilePos = fileList.GetHeadPosition();
	while (NULL != pFilePos) 
	{	
		m_fileList.AddTail(fileList.GetNext(pFilePos));
	}
	
	m_currPos = m_fileList.GetHeadPosition();
	if (GetFilePathEx(m_currPos))		//取得第一个文件的路径
	{
		FILE_INFO_EX & fileInfo = m_fileList.GetAt(m_currPos);

		if (ONE_CHNN_READ_SUC == OpenFile(fileInfo.startTime,fileInfo.chnn))		//打开该文件
		{
			if (SEEK_FRAME_OK == SeekFrame(SEEK_FIRST_KEY))		//定位到该文件的第一个关键帧
			{
				FILE_INFO_EX &fileInfo = m_fileList.GetHead();
				
				//取了第一个文件里的通道号，需要保证所有文件都是同一个通道里的文件.	
				m_channel	= fileInfo.chnn;
				m_readState = READ_STATE_ACTION;
                RegisterReadFileIndex();
				return ONE_CHNN_READ_SUC;
			}
			else
			{
				RUN_LOG(RUN_LOG_ITEM_PB_SEEK_TIME_ERR, CRunLog::RUN_Num("The state is:", m_readState), CRunLog::RUN_Num("chnn:", m_channel), m_filePath);
				RemoveFileList();
				CloseFile();
				return ONE_CHNN_READ_FAIL;
			}
		}
		else
		{
			RUN_LOG(RUN_LOG_ITEM_PB_OPEN_FILE_FAIL, CRunLog::RUN_Num("The state is:", m_readState), CRunLog::RUN_Num("chnn:", m_channel), m_filePath);
			RemoveFileList();
			return ONE_CHNN_READ_FAIL;
		}
	}
	else
	{
		RemoveFileList();
		assert(false);
		return ONE_CHNN_READ_FAIL;
	}
	
}

long COneChnnReader::StartRead(unsigned long playTime, unsigned long chnn, const char *pRecPath /* = NULL */)
{
	//保证没有开启过，或者已经关闭
	assert(m_fileList.IsEmpty());
	assert(NULL == m_currPos);

	m_bDynSearch = true;

	if(pRecPath != NULL)
	{
		strcpy(m_recPath, pRecPath);
	}
	else
	{
		m_recPath[0] = '\0';
	}

	m_channel = chnn;
	//取得的文件列表已存放在m_fileList中
	if (m_pReclogMan->GetFileInfo(m_channel, playTime, m_fileList, m_diskOwnerIndex, true) > 0) 
	{
		//2010-04-14 17:53:00 YSW
		//如果文件列表的时间不包含条件时间所在天，则认为启动失败
		if ( (m_fileList.GetTail().endTime <= playTime) || \
			(GetZeroTimeOfDate(playTime) <= m_fileList.GetHead().startTime))
		{
			RemoveFileList();
			return ONE_CHNN_READ_FAIL;
		}

		//2010-01-25 11:29:00 YSW
		//修正开始时间再定位，因为可能选择的开始时间点没有录像数据，而我们希望能选择到最接近
		//数据开始回放。由于SeekFrameToTime中会依据playTime判断是否在文件列表的时间区间内（不再
		//则重新搜索），这样就会在启动时做了两次搜索动作。
		//因而再次先修改时间，就是如果playTime < head.StartTime，则进行修正。

		if (playTime < m_fileList.GetHead().startTime)
		{
			playTime = m_fileList.GetHead().startTime;
		}

		return SeekFrameToTime(true, playTime);
	}
	else
	{
		RUN_LOG(RUN_LOG_ITEM_PB_GET_FILE_FAIL, CRunLog::RUN_Time("The time is:", playTime), CRunLog::RUN_Num("chnn:", chnn), "Open file fail");
		RemoveFileList();
		return ONE_CHNN_READ_FAIL;
	}
}

 
/************************************************************************
函数原型	void StopRead();
功能描述	清空文件列表，停止读取，关闭已打开的录像文件。
输入参数	无
输出参数	无
返回值		无
************************************************************************/
void COneChnnReader::StopRead()
{
	//删除文件列表。
	RemoveFileList();

	//停止读取，关闭录像文件，并做相关变量复位操作。
	CloseFile();
    assert(!m_bRegister);
	m_readState = READ_STATE_STOP;
}


/************************************************************************
函数原型	long GetFrame(FRAME_INFO_EX &frameInfo)
功能描述	取得当前帧.
输入参数	无
输出参数	frameInfo  :输出一帧及帧信息.
返回值		ONE_CHNN_READ_SUC：成功； ONE_CHNN_READ_FAIL：失败 ONE_CHNN_READ_SLEEP:睡眠状态
************************************************************************/
long COneChnnReader::GetFrame(FRAME_INFO_EX &frameInfo)
{
	assert(m_bOpenFile);
	assert(NULL != frameInfo.pData);
	
	if (!m_bOpenFile || (NULL == frameInfo.pData))
	{
		return ONE_CHNN_READ_FAIL;
	}

	//读取状态不为活动状态时,直接返回失败.
	if (READ_STATE_ACTION != m_readState )
	{
		return ONE_CHNN_READ_SLEEP;
	}
	
	frameInfo.channel	= m_channel;
	
	//新打开文件，需要将视频和音频格式送出去
	if (m_bReVideoFmat)  //视频格式
	{
		frameInfo.frameType = FRAME_TYPE_VIDEO_FORMAT;
		frameInfo.length = sizeof(BITMAPINFOHEADER);
		memcpy(frameInfo.pData, &m_bitmapInfo, frameInfo.length);		
		m_bReVideoFmat = false;		
		return ONE_CHNN_READ_SUC;
	}
	
	if (m_bReAudioFmat)//音频格式
	{
		frameInfo.frameType = FRAME_TYPE_AUDIO_FORMAT;
		frameInfo.length = sizeof(WAVEFORMATEX);
		memcpy(frameInfo.pData, &m_waveFormat, frameInfo.length);
		m_bReAudioFmat = false;
		return ONE_CHNN_READ_SUC;
	}
	
	//读取一帧.
	if (ERR_READ_FILE_OK == GetCurrFrame(frameInfo))
	{
		frameInfo.width		= m_bitmapInfo.biWidth;
		frameInfo.height	= m_bitmapInfo.biHeight;
		return ONE_CHNN_READ_SUC;
	}
	else
	{
		return ONE_CHNN_READ_FAIL;
	}
}

/************************************************************************
函数原型	LONGLONG GetFrameTime ()
功能描述	取得当前帧的时间。
输入参数	无
输出参数	无
返回值		返回当前帧的时间
************************************************************************/
LONGLONG COneChnnReader::GetFrameTime()
{
	if (m_readerType == FILE_TYPE_FTVT)
	{
		m_videoTime = m_cRecReader.GetCurrFrameTime();
	}
	else
	{
		m_videoTime = m_cAviReader.GetCurrFrameTime();
	}

	return 	m_videoTime;
}

FRAME_TYPE COneChnnReader::GetFrameType()
{
	if(m_bReVideoFmat)
	{
		return FRAME_TYPE_VIDEO_FORMAT;
	}
	else if(m_bReAudioFmat)
	{
		return FRAME_TYPE_AUDIO_FORMAT;
	}
	else if(m_readerType == FILE_TYPE_FTVT)
	{
		return m_cRecReader.GetCurrFrameType();
	}
	else
	{
		return m_cAviReader.GetCurrFrameType();
	}
}

/************************************************************************
函数原型	long SeekFrame(SEEK_MODE mode, unsigned long num = 1);
功能描述	定位上一帧，下一帧，第一个关键帧，最后一个关键帧，向前和向后跳多个关键帧。
输入参数	mode : 取值范围	SEEK_PREV_VFRM,	SEEK_NEXT_VFRM,	SEEK_FIRST_KEY,SEEK_LAST_KEY,\
				   SEEK_PREV_KEY, SEEK_NEXT_KEY,
			num : 向前，向后跳关键帧的数目。
输出参数	无
返回值	SEEK_FRAME_OK：成功；SEEK_FRAME_FAIL：失败 SEEK_FRAME_SLEEP: 睡眠状态。

修改记录：
		2009-12-30-20:54:00（YSW）
		增加了动态搜索文件，满足可持续播放新录制的文件。
************************************************************************/
long COneChnnReader::SeekFrame(SEEK_MODE mode, unsigned long num /* = 1 */)
{
	//在已打开文件中定位
	unsigned long ret = SeekVideoFrame(mode, num);
	if (ERR_READ_FILE_OK == ret)
	{
		return SEEK_FRAME_OK;
	}
	else if ((mode == SEEK_NEXT_VFRM) || (mode == SEEK_NEXT_KEY) || (mode ==SEEK_FIRST_KEY))
	{
		//取得下一个文件
		if (GET_NEW_FILE_SUC == GetNextFile())			//取得新文件
		{
			//定位到第一个关键帧
			return SeekFrame(SEEK_FIRST_KEY);
		}
		else			//sleep
		{
			if (m_bDynSearch)
			{
				//2010-02-25 11:20:00 YSW
				//如果获取下一个文件不成功（包括进入SLEEP状态），因为系统一般总是处于录像状态中（即在回放过程中录像数据发生了变化）
				//比如某个时间点回放搜索最近一段时间的录像文件，播放完这些文件后（又增添一批新的录像文件）
				//此时会进入这个分支，那么这种情况下，应该重新进行搜索，以更新文件信息。
				//把当前文件列表删除后，在SeekFrameToTime中就会重新搜索文件，并再次定位。
				RemoveFileList();

				if (ONE_CHNN_READ_FAIL != SeekFrameToTime(true, (m_videoTime/1000000)+1))
				{
					return SEEK_FRAME_OK;
				}
			}
			
			//2010-01-25 17:32:00 YSW
			//GetNextFile在打开文件列表中的文件时，可能全部不能打开（典型情况是全部被覆盖，或者磁盘出错了）
			//GetNextFile内部在此种情况下，则是把文件列表给清空，并返回失败，同时把m_readState = READ_STATE_STOP。
			//如果是动态搜索，则会进去上面的段落中，同样也会有一个结果，如果不是动态搜索，则
			//需要在此判定好返回正确地状态。

			return (READ_STATE_STOP != m_readState) ? SEEK_FRAME_SLEEP : SEEK_FRAME_FAIL;
		}
		
	}
	else if ((mode == SEEK_PREV_VFRM) || (mode == SEEK_PREV_KEY) || (mode == SEEK_LAST_KEY))
	{
		//取得上一个文件
		if (GET_NEW_FILE_SUC == GetPrevFile())			//取得新文件
		{
			//定位到最后一个关键帧
			return SeekFrame(SEEK_LAST_KEY);
		}
		else		//sleep
		{
			if (m_bDynSearch)
			{
				//2010-02-25 11:20:00 YSW
				//描述见GetNextFile分支.
				RemoveFileList();

				if (ONE_CHNN_READ_FAIL != SeekFrameToTime(false, (m_videoTime/1000000)-1))
				{
					return SEEK_FRAME_OK;
				}
			}

			//2010-01-25 17:32:00 YSW
			//GetPrevFile在打开文件列表中的文件时，可能全部不能打开（典型情况是全部被覆盖，或者磁盘出错了）
			//GetPrevFile内部在此种情况下，则是把文件列表给清空，并返回失败，同时把m_readState = READ_STATE_STOP。
			//如果是动态搜索，则会进去上面的段落中，同样也会有一个结果，如果不是动态搜索，则
			//需要在此判定好返回正确地状态。

			return (READ_STATE_STOP != m_readState) ? SEEK_FRAME_SLEEP : SEEK_FRAME_FAIL;
		}
	}
	else
	{
		RUN_LOG(RUN_LOG_ITEM_PB_SEEK_TIME_ERR, CRunLog::RUN_Time("The mode is:", mode), CRunLog::RUN_Num("chnn:", CHANNEL_ID(m_channel)), "seek fail");
		return SEEK_FRAME_FAIL;
	}
	
}

long COneChnnReader::GetFrameLen()
{
	if (m_readerType == FILE_TYPE_FTVT)
	{
		return m_cRecReader.GetCurrFrameLen();
	}
	else
	{
		return m_cAviReader.GetCurrFrameLen();
	}

}
/************************************************************************
函数原型	long SeekFrameToTime(bool bForward, LONGLONG time);
功能描述	按时间定位视频帧
输入参数	time  需要定位视频帧的时间。
			bForward 当前读取方向是向前还是向后
输出参数	无
返回值		ONE_CHNN_READ_SUC：成功；ONE_CHNN_READ_FAIL：失败; 
			ONE_CHNN_READ_SLEEP: 前后没有文件,sleep 状态.
************************************************************************/
long COneChnnReader::SeekFrameToTime(bool bForward, unsigned long time)
{
	if (m_fileList.IsEmpty())
	{
		assert (NULL == m_currPos);
		assert (READ_STATE_STOP == m_readState);

		if (m_bDynSearch)
		{
			if (0 == m_pReclogMan->GetFileInfo(m_channel, time, m_fileList, m_diskOwnerIndex, true))
			{
				return ONE_CHNN_READ_FAIL;
			}
			//else 搜索到文件，进入下一个比对环节。
		}
		else
		{
			return ONE_CHNN_READ_FAIL;
		}
	}
	else
	{
		//time不再列表的时间区间内，且是自动搜索模式，则删除原来的文件，再次重新搜索。
		//如果不是自动模式，则还是使用原来的文件做比对。
		if (((time < m_fileList.GetHead().startTime) || (m_fileList.GetTail().endTime < time)) && m_bDynSearch)
		{
			RemoveFileList();

			RUN_LOG(RUN_LOG_ITEM_PB_FILE_NULL, CRunLog::RUN_Time("The time is:", time), CRunLog::RUN_Num("chnn:", m_channel), "playback all files in list,and search file again");
			
			//由于这样搜索方式必然成功，如果其返回零，则表示没有任何文件存在。此时可以将状态改为停止状态。
			if (0 == m_pReclogMan->GetFileInfo(m_channel, time, m_fileList, m_diskOwnerIndex, true))
			{
				assert (false);
				CloseFile();
				m_readState = READ_STATE_STOP;
				return ONE_CHNN_READ_FAIL;
			}
			//搜索到文件，则进入比对环节。
		}
	}

	unsigned long startTime = m_fileList.GetHead().startTime;
	unsigned long endTime = m_fileList.GetTail().endTime;
   
	if (time < startTime)//在时间区间前，当然只有向后SLEEP的状态
	{
		POS pos = m_fileList.GetHeadPosition();
		while (NULL != pos)
		{
			if (m_currPos == pos)//该文件已经打开
			{
				unsigned long seekRet = SeekKeyFrameToTime(time);
				assert(ERR_READ_FILE_SEEK_TO_END != seekRet);
				if ((ERR_READ_FILE_OK == seekRet) || (ERR_READ_FILE_SEEK_TO_BEGIN == seekRet))	//
				{
					m_readState = READ_STATE_FORWARD_SLEEP;
					return ONE_CHNN_READ_SLEEP;
				}
				//不成功，则跳向下一个文件。
			}
			else
			{
				if (GetFilePathEx(pos))		//取得第一个文件的路径
				{
					CloseFile();

					FILE_INFO_EX & fileInfo = m_fileList.GetAt(pos);
					if (ONE_CHNN_READ_SUC == OpenFile(fileInfo.startTime,fileInfo.chnn))		//打开该文件
					{
						unsigned long seekRet = SeekKeyFrameToTime(time);
						assert(ERR_READ_FILE_SEEK_TO_END != seekRet);
						if ((ERR_READ_FILE_OK == seekRet) || (ERR_READ_FILE_SEEK_TO_BEGIN == seekRet))	//
						{
							m_currPos	= pos;
							m_readState = READ_STATE_FORWARD_SLEEP;
							RegisterReadFileIndex();
							return ONE_CHNN_READ_SLEEP;
						}
					}
				}
			}

			m_fileList.GetNext(pos);
		}
	}
	else if ((startTime <= time) && (time < endTime))//在时间区间内
	{
		//2010-01-25 15:31:00 YSW
		//(time < endTime)不能相等，因为当某个文件结束时，紧接着会开始下一个文件，由于
		//我们采用秒为单位，所以非常有可能前个文件的结束时间等于后一个文件的开始时间。

		/*
		从头开始找合适的文件节点（紧前式），遍历文件列表。
		不管是向前还是向后，都需要找到紧靠time的一个关键帧，所以可以采用
		从最小时间的文件开始向后遍历，这样可以满足向前向后的需要。
		*/
		POS tempPos = NULL;
		POS pos = m_fileList.GetHeadPosition();
		while (NULL != pos)
		{
			FILE_INFO_EX & node = m_fileList.GetAt(pos);
			
			//2010-01-25 15:31:00 YSW
			//if (time <= node.endTime)
			//当我们采用某个文件的开始时间作为回放的起点时间时，在这个地方定位的话，如果(time <= node.endTime)
			//，则会进入前面的文件并进行定位（能定位到最后一个关键帧）。所以出现了点击一个事件回放，却回放了
			//前面一个文件的情况。
			if (time < node.endTime)
			{
				tempPos	= pos;
				break;
			}
			m_fileList.GetNext(pos);
		}

		//如果找到的是当前已经打开的文件，则先进行seek动作。
		if ((NULL != tempPos) && (tempPos == m_currPos))
		{
			if (bForward)
			{
				unsigned long seekRet = SeekKeyFrameToTime(time);
				//assert(ERR_READ_FILE_SEEK_TO_END != seekRet); 此处是存在定位到最后一帧的情况
				if ((ERR_READ_FILE_SEEK_TO_END == seekRet) || (ERR_READ_FILE_OK == seekRet) || (ERR_READ_FILE_SEEK_TO_BEGIN == seekRet))
				{
					m_readState = READ_STATE_ACTION;
					return ONE_CHNN_READ_SUC;
				}
			}
			else
			{
				unsigned long seekRet = SeekKeyFrameToTime(time);
				assert(ERR_READ_FILE_SEEK_TO_BEGIN != seekRet);
				if ((ERR_READ_FILE_OK == seekRet) || (ERR_READ_FILE_SEEK_TO_END == seekRet))	//
				{
					m_readState = READ_STATE_ACTION;
					return ONE_CHNN_READ_SUC;
				}
			}
		}
	
		m_currPos = tempPos;
		if (bForward)
		{
			POS pos = m_currPos;
			while (NULL != pos)
			{
				if (GetFilePathEx(pos))		//取得第一个文件的路径
				{
					CloseFile();

					FILE_INFO_EX & fileInfo = m_fileList.GetAt(pos);
					if (ONE_CHNN_READ_SUC == OpenFile(fileInfo.startTime,fileInfo.chnn))		//打开该文件
					{
						unsigned long seekRet = SeekKeyFrameToTime(time);
						//assert(ERR_READ_FILE_SEEK_TO_END != seekRet); 此处是存在定位到最后一帧的情况
					    if ((ERR_READ_FILE_SEEK_TO_END == seekRet) || (ERR_READ_FILE_OK == seekRet) || (ERR_READ_FILE_SEEK_TO_BEGIN == seekRet))	//
						{
							m_currPos	= pos;
							m_readState = READ_STATE_ACTION;
							RegisterReadFileIndex();
							return ONE_CHNN_READ_SUC;
						}
					}
				}

				m_fileList.GetNext(pos);
			}

			pos = m_currPos;
			while (NULL != pos)
			{
				if (GetFilePathEx(pos))		//取得第一个文件的路径
				{
					CloseFile();

					FILE_INFO_EX & fileInfo = m_fileList.GetAt(pos);
					if (ONE_CHNN_READ_SUC == OpenFile(fileInfo.startTime,fileInfo.chnn))		//打开该文件
					{
						unsigned long seekRet = SeekKeyFrameToTime(time);
						assert(seekRet != ERR_READ_FILE_SEEK_TO_BEGIN);
						if ((ERR_READ_FILE_OK == seekRet) || (ERR_READ_FILE_SEEK_TO_END == seekRet))	//
						{
							m_currPos	= pos;
							m_readState = READ_STATE_BEHIND_SLEEP;
							RegisterReadFileIndex();
							return ONE_CHNN_READ_SLEEP;
						}
					}
				}

				m_fileList.GetPrev(pos);
			}
		}
		else	//!bForward
		{
			pos = m_currPos;
			while (NULL != pos)
			{
				if (GetFilePathEx(pos))		//取得第一个文件的路径
				{
					CloseFile();

					FILE_INFO_EX & fileInfo = m_fileList.GetAt(pos);
					if (ONE_CHNN_READ_SUC == OpenFile(fileInfo.startTime,fileInfo.chnn))		//打开该文件
					{
						unsigned long seekRet = SeekKeyFrameToTime(time);
						//assert(ERR_READ_FILE_SEEK_TO_BEGIN != seekRet); 此处是存在定位到第一帧的情况，比如，只有一个文件time等于第一帧的时间。
						if ((ERR_READ_FILE_SEEK_TO_BEGIN == seekRet) || (ERR_READ_FILE_OK == seekRet) || (ERR_READ_FILE_SEEK_TO_END == seekRet))
						{
							m_currPos	= pos;
							m_readState = READ_STATE_ACTION;
							RegisterReadFileIndex();
							return ONE_CHNN_READ_SUC;
						}
					}
				}

				m_fileList.GetPrev(pos);
			}

			POS pos = m_currPos;
			while (NULL != pos)
			{
				if (GetFilePathEx(pos))		//取得第一个文件的路径
				{
					CloseFile();

					FILE_INFO_EX & fileInfo = m_fileList.GetAt(pos);
					if (ONE_CHNN_READ_SUC == OpenFile(fileInfo.startTime,fileInfo.chnn))		//打开该文件
					{
						unsigned long seekRet = SeekKeyFrameToTime(time);
						assert(ERR_READ_FILE_SEEK_TO_END != seekRet);
						if ((ERR_READ_FILE_OK == seekRet) || (ERR_READ_FILE_SEEK_TO_BEGIN == seekRet))	//
						{
							m_currPos	= pos;
							m_readState = READ_STATE_FORWARD_SLEEP;
							RegisterReadFileIndex();
							return ONE_CHNN_READ_SLEEP;
						}
					}
				}

				m_fileList.GetNext(pos);
			}
		}
	}
	else//在时间区间后，当然只有向前SLEEP的状态
	{
		POS pos = m_fileList.GetTailPosition();
		while (NULL != pos)
		{
			if (m_currPos == pos)//该文件已经打开
			{
				unsigned long seekRet = SeekKeyFrameToTime(time);
				assert(ERR_READ_FILE_SEEK_TO_BEGIN != seekRet);
				if ((ERR_READ_FILE_OK == seekRet) || (ERR_READ_FILE_SEEK_TO_END == seekRet))	//
				{
					m_readState = READ_STATE_BEHIND_SLEEP;
					return ONE_CHNN_READ_SLEEP;
				}
				//不成功，则跳向前一个文件。
			}
			else
			{
				if (GetFilePathEx(pos))		//取得第一个文件的路径
				{
					CloseFile();

					FILE_INFO_EX & fileInfo = m_fileList.GetAt(pos);
					if (ONE_CHNN_READ_SUC == OpenFile(fileInfo.startTime,fileInfo.chnn))		//打开该文件
					{
						unsigned long seekRet = SeekKeyFrameToTime(time);
						assert(ERR_READ_FILE_SEEK_TO_BEGIN != seekRet);
						if ((ERR_READ_FILE_OK == seekRet) || (ERR_READ_FILE_SEEK_TO_END == seekRet))	//
						{
							m_currPos	= pos;
							m_readState = READ_STATE_BEHIND_SLEEP;
							RegisterReadFileIndex();
							return ONE_CHNN_READ_SLEEP;
						}
					}
				}
			}

			m_fileList.GetPrev(pos);
		}

		
	}

	//修改前：
	/*
	//如果找不到合适的文件（不存在这种情况），或者是不能打开任何一个文件，则转入停止状态。
	assert (false);
	*/
	//修改后：原因：这个断言和说明有问题.//存在没有找到文件的情况。比如文件列表中只有一个正在录像的文件，就存在打不开这个文件的情况。或者硬盘错误，那么就打不开文件
    //assert (false);
	RUN_LOG(RUN_LOG_ITEM_PB_OPEN_ALL_FILE_FAIL, CRunLog::RUN_Time("The time is:", time), CRunLog::RUN_Num("chnn:", m_channel), "open all file List faild");

	RemoveFileList();
	CloseFile();
	m_readState = READ_STATE_STOP;
	return ONE_CHNN_READ_FAIL;
}

/************************************************************************
函数原型	long  GetReadState();
功能描述	取得当前单通道读的状态.
输入参数	无
输出参数	无
返回值		返回当前单通道读的状态.
************************************************************************/
long COneChnnReader::GetReadState()
{
	return m_readState;
}

/************************************************************************
函数原型	long WakeUp()
功能描述	读位置从没有数据时间区间回到有数据的区间,重新定位到临近的关键帧.
输入参数	无
输出参数	无
返回值		ONE_CHNN_READ_SUC：成功； ONE_CHNN_READ_FAIL：失败
************************************************************************/
long COneChnnReader::WakeUp()
{
	assert (m_readState != READ_STATE_ACTION );	//可读取状态不能调用该涵数
	assert (m_readState != READ_STATE_STOP);	//初始状态不能调用该涵数
	
	switch(m_readState)
	{
	case READ_STATE_FORWARD_SLEEP:
		m_readState = READ_STATE_ACTION;
		return ONE_CHNN_READ_SUC;
		break;
	case READ_STATE_BEHIND_SLEEP:
		m_readState = READ_STATE_ACTION;
		return ONE_CHNN_READ_SUC;
		break;
	default:
		break;
	}
	return ONE_CHNN_READ_FAIL;
	
}

//protected
/************************************************************************
函数原型	long OpenFile(unsigned long startTime)
功能描述	打开m_filePath指定的录像文件.获取音、视频格式信息。
输入参数	无
输出参数	无
返回值		ONE_CHNN_READ_SUC：成功； ONE_CHNN_READ_FAIL：失败
************************************************************************/
long COneChnnReader::OpenFile(unsigned long startTime,unsigned long channel)
{
	if(m_bOpenFile)
	{
		CloseFile();
		m_bOpenFile = false;
	}

	FILE_BUF_TYPE fileType = FILE_BUF_TYPE_LOCAL;
	if(channel<CProduct::Instance()->LocalVideoInputNum())
	{
		fileType = FILE_BUF_TYPE_LOCAL;
	}
	else if(channel<CProduct::Instance()->VideoInputNum())
	{
		fileType = FILE_BUF_TYPE_IP;
	}
	else
	{
		fileType = FILE_BUF_TYPE_MIN;
	}
		
	if (m_cRecReader.Open(m_filePath,fileType))
	{
		m_readerType = FILE_TYPE_FTVT;
	}
	else
	{
		if (m_cAviReader.Open(m_filePath))
		{
			m_readerType = FILE_TYPE_AVI;
		}
		else
		{
			return ONE_CHNN_READ_FAIL;
		}
	}
	
	//2010-01-25 14:08:00 YSW
	//startTime的值有两种情况：一是日志记录中的文件的开始时间。二是日志记录中这个文件中某个事件的开始时间
	//检索文件开头的时间是不是和startTime接近，如果不接近，则表示在搜索出来该文件的信息后
	//因为录像覆盖，把其实质内容改写了，和m_fileList中保持的信息不对应了，所以不能打开该文件
	//进行读取操作，当成打开文件失败的情况处理。
	{
		unsigned long fileStartTime = 0;
		unsigned long fileEndTime = 0;
		if (FILE_TYPE_FTVT == m_readerType)
		{
			m_cRecReader.GetTimeRange(fileStartTime, fileEndTime);
			if (0 == fileEndTime)
			{
				fileEndTime = GetCurrTime32() - 60;
			}

			//2010-02-08 21:33:00 YSW
			//存在着一些帧率很低的情况，时间间隔比较长，不能用10秒来判定。120秒可以容纳极限情况。
			if ((startTime < (fileStartTime - 120)) || (startTime >= fileEndTime))
			{
				m_cRecReader.Close();			

				RUN_LOG(RUN_LOG_ITEM_PB_FILE_REWRITE, CRunLog::RUN_Time("The time is:", startTime), CRunLog::RUN_Num("chnn:", m_channel), "file has been rewrite");
				printf("%s:%s:%d, time error\n", __FUNCTION__, __FILE__, __LINE__);
				PrintfBit64(startTime, true);
				PrintfBit64(fileStartTime, true);
				PrintfBit64(fileEndTime, true);

				return ONE_CHNN_READ_FAIL;
			}
		}
		else
		{
			m_cAviReader.SeekVideoFrame(SEEK_FIRST_KEY);
			fileStartTime = static_cast<unsigned long>(m_cAviReader.GetCurrFrameTime()/1000000);
			m_cAviReader.SeekVideoFrame(SEEK_LAST_KEY);
			fileEndTime = static_cast<unsigned long>(m_cAviReader.GetCurrFrameTime()/1000000);
			        
			if ((startTime < fileStartTime - 120) || (startTime > fileEndTime))
			{
				m_cAviReader.Close();			

				RUN_LOG(RUN_LOG_ITEM_PB_FILE_REWRITE, CRunLog::RUN_Time("The time is:", startTime), CRunLog::RUN_Num("chnn:", m_channel), "file has been rewrite");
				return ONE_CHNN_READ_FAIL;
			}
			m_cAviReader.Close();
			m_cAviReader.Open(m_filePath);						
		}		
	}
	

	//检查完成了，通过了才进行下面的步骤。
	if(GetVideoInfo(m_bitmapInfo))
	{
		m_bReVideoFmat = true;
	}
	else
	{
		//既然是被搜索到,又没有数据帧,应已出异常,断言死掉.
		RUN_LOG(RUN_LOG_ITEM_PB_OPEN_FILE_FAIL, CRunLog::RUN_Time("The time is:", startTime), CRunLog::RUN_Num("chnn:", m_channel), "open file fail");
		assert(false);

		return ONE_CHNN_READ_FAIL;
	}
	
	if(GetAudioInfo(m_waveFormat))
	{
		m_bReAudioFmat = true;
	}
	else
	{
		m_bReAudioFmat = false;
	}
	
	m_bOpenFile = true;

	printf("channel %d open file %s succ\n", CHANNEL_ID(m_channel), m_filePath);

	return ONE_CHNN_READ_SUC;
	
}

/************************************************************************
函数原型	void CloseFile()
功能描述	关闭录像文件。
输入参数	无
输出参数	无
返回值		无
************************************************************************/
void COneChnnReader::CloseFile()
{
//	assert(true == m_bOpenFile);
	if (m_bRegister)
	{
		assert(m_bOpenFile);
		CancelReadFileIndex();
	}

	if(m_bOpenFile)
	{
		if (m_readerType == FILE_TYPE_FTVT)
		{
			m_cRecReader.Close();
		}
		else
		{
			m_cAviReader.Close();
		}
		m_bOpenFile = false;
	}
}

/************************************************************************
函数原型	long GetPrevFile()
功能描述	取得上一个文件
输入参数	无
输出参数	无
返回值		GET_NEW_FILE_SUC: 取得新文件成功；GET_NEW_FILE_FAIL：取得新文件失败
			RESERVER_OLD_FILE:  前面已经没有文件,保留原有文件.
************************************************************************/
long COneChnnReader::GetPrevFile()
{
	assert (NULL != m_currPos);
	POS tmpPos = m_currPos;
	
	m_fileList.GetPrev(tmpPos);
	while (tmpPos != NULL)
	{
		if (m_bOpenFile)
		{
			CloseFile();
		}
		
		if (GetFilePathEx(tmpPos))
		{
			FILE_INFO_EX & fileInfo = m_fileList.GetAt(tmpPos);

			//打开新文件失败
			if (ONE_CHNN_READ_SUC == OpenFile(fileInfo.startTime,fileInfo.chnn))
			{
				m_currPos = tmpPos;
				RegisterReadFileIndex();
				return GET_NEW_FILE_SUC;
			}
		}
		
		//在以上取得文件路径失败或者打开文件失败的时候取下一个文件
		m_fileList.GetPrev(tmpPos);
	}
	
	if (!m_bOpenFile)
	{
		if (GetFilePathEx(m_currPos))
		{
			FILE_INFO_EX & fileInfo = m_fileList.GetAt(m_currPos);
			if (ONE_CHNN_READ_SUC == OpenFile(fileInfo.startTime,fileInfo.chnn))
			{
				if (ERR_READ_FILE_OK == SeekVideoFrame(SEEK_FIRST_KEY))
				{
					m_readState = READ_STATE_FORWARD_SLEEP;
					RegisterReadFileIndex();
					return RESERVER_OLD_FILE;
				}
			}
		}
		//恢复现场失败
		//assert(false);

		//2010-01-25 17:22:00（YSW）
		//可能在此过程中，当前文件也被覆盖的情况，所以应该清空列表并返回失败
		RemoveFileList();
		m_readState = READ_STATE_STOP;
		return GET_NEW_FILE_FAIL;
	}

	//2009-12-30 11:49:00修改（YSW），GetNextFile只是在SeekFrame函数中调用，且
	//在调用返回后，再次调用SeekFrame(SEEK_LAST_KEY)，然后返回，再加之此处并没有判断SeekFrame(SEEK_LAST_KEY)的返回值
	//事实上是一个动作做了两次，因此把此处给去掉。
	//SeekFrame(SEEK_LAST_KEY);
	m_readState = READ_STATE_FORWARD_SLEEP;
	return RESERVER_OLD_FILE;
}

/************************************************************************
函数原型	long GetNextFile()
功能描述	取得下一个文件
输入参数	无
输出参数	无
返回值		GET_NEW_FILE_SUC: 取得新文件成功；GET_NEW_FILE_FAIL：取得新文件失败
			RESERVER_OLD_FILE:  后面已经没有文件,保留原有文件.
************************************************************************/
long COneChnnReader::GetNextFile()
{
	assert (NULL != m_currPos);
	POS tmpPos = m_currPos;
	
	m_fileList.GetNext(tmpPos);	
	while (tmpPos != NULL)
	{
		if (m_bOpenFile)
		{						
			CloseFile();
		}
		
		if (GetFilePathEx(tmpPos))
		{
			FILE_INFO_EX & fileInfo = m_fileList.GetAt(tmpPos);
			if (ONE_CHNN_READ_SUC == OpenFile(fileInfo.startTime,fileInfo.chnn))
			{
				m_currPos = tmpPos;
				RegisterReadFileIndex();
				return GET_NEW_FILE_SUC;
			}
		}
		//在以上取得文件路径失败或者打开文件失败的时候取下一个文件
		m_fileList.GetNext(tmpPos);
	}
	
	//上一段落尝试打开一些文件，但是因为某些原因失败了，所以为未打开状态
	//此时需要恢复现场
	if (!m_bOpenFile)
	{
		if (GetFilePathEx(m_currPos))
		{
			FILE_INFO_EX & fileInfo = m_fileList.GetAt(m_currPos);
			if (ONE_CHNN_READ_SUC == OpenFile(fileInfo.startTime,fileInfo.chnn))
			{
				if (ERR_READ_FILE_OK == SeekVideoFrame(SEEK_LAST_KEY))
				{
					m_readState = READ_STATE_BEHIND_SLEEP;
					RegisterReadFileIndex();
					return RESERVER_OLD_FILE;
				}
			}
		}
		//恢复现场失败
		//assert(false);

		//2010-01-25 17:22:00（YSW）
		//可能在此过程中，当前文件也被覆盖的情况，所以应该清空列表并返回失败
		RemoveFileList();
		m_readState = READ_STATE_STOP;
		return GET_NEW_FILE_FAIL;
	}

	//可能当前文件为列表里的最后一个文件，那么既未尝试打开其他文件
	//更没有回复现场的操作，而是直接进入SLEEP状态。

	//2009-12-30 11:49:00修改（YSW），GetNextFile只是在SeekFrame函数中调用，且
	//在调用返回后，再次调用SeekFrame(SEEK_LAST_KEY)，然后返回，再加之此处并没有判断SeekFrame(SEEK_LAST_KEY)的返回值
	//事实上是一个动作做了两次，因此把此处给去掉。
	//SeekFrame(SEEK_LAST_KEY);
	m_readState = READ_STATE_BEHIND_SLEEP;
	return RESERVER_OLD_FILE;
}


/************************************************************************
函数原型	void COneChnnReader::RemoveFileList()
功能描述	删除文件链表
输入参数	无
输出参数	无
返回值		无
************************************************************************/	
void COneChnnReader::RemoveFileList()
{
	m_fileList.RemoveAll();
	m_currPos = NULL;
	m_readState = READ_STATE_STOP;
}


/************************************************************************
函数原型	bool GetFilePathEx(POS filePos)
功能描述	取得文件的路径
输入参数	filePos  ：filePos指向m_ fileList中某个结点的位置
输出参数	无
返回值		true：成功； false：失败

************************************************************************/
bool COneChnnReader::GetFilePathEx(POS filePos)
{
	assert(filePos != NULL);
	assert(!(m_fileList.IsEmpty()));
	memset(m_filePath, 0, sizeof(m_filePath));
	FILE_INFO_EX &fileInfo = m_fileList.GetAt(filePos);

#ifdef WIN32
#ifdef __ENVIRONMENT_VIDEO_PLAY__
	if (1 < strlen(m_recPath))
	{
		sprintf(m_filePath, "%s\\%08d.dat", m_recPath, fileInfo.fileIndex);
	}
	else
	{
		assert (1 == strlen(m_recPath));
		sprintf(m_filePath, "%c:\\%08d.dat", m_recPath[0]+fileInfo.diskIndex, fileInfo.fileIndex);
	}
#else
	if (1 < strlen(m_recPath))
	{
		sprintf(m_filePath, "%s\\%02d\\%08d.dat", m_recPath, fileInfo.diskIndex, fileInfo.fileIndex);
	}
	else
	{
		assert (1 == strlen(m_recPath));
		sprintf(m_filePath, "%c:\\%08d.dat", m_recPath[0]+fileInfo.diskIndex, fileInfo.fileIndex);
	}
#endif
#else
	sprintf(m_filePath,"%s/%02d/%08d.dat", m_recPath, fileInfo.diskIndex, fileInfo.fileIndex);
#endif

	return true;
}

unsigned long COneChnnReader::SeekVideoFrame(SEEK_MODE mode, unsigned long seekNum)
{
	if ((SEEK_NEXT_VFRM == mode) && (seekNum == 1))
	{
		if (m_readerType == FILE_TYPE_FTVT)
		{
			return m_cRecReader.SeekNextFrame();
		}
		else
		{
			return m_cAviReader.SeekNextFrame();
		}

	}

	if (m_readerType == FILE_TYPE_FTVT)
	{
		return m_cRecReader.SeekVideoFrame(mode, seekNum);
	}
	else
	{
		return m_cAviReader.SeekVideoFrame(mode, seekNum);
	}
}

unsigned long COneChnnReader::SeekKeyFrameToTime(unsigned long time)
{
	if (m_readerType == FILE_TYPE_FTVT)
	{
		return m_cRecReader.SeekKeyFrameToTime((LONGLONG)time*1000000);
	}
	else
	{
		return m_cAviReader.SeekKeyFrameToTime((LONGLONG)time*1000000);
	}
}

bool COneChnnReader::GetVideoInfo(BITMAPINFOHEADER &bitmapInfo)
{
	if (m_readerType == FILE_TYPE_FTVT)
	{
		if(m_cRecReader.GetFrameNum(FRAME_TYPE_VIDEO) > 0)
		{
			if (m_cRecReader.GetVideoInfo(&bitmapInfo) == ERR_READ_FILE_OK)
			{
				return true;
			}
		}
	}
	else
	{
		if(m_cAviReader.GetFrameNum(FRAME_TYPE_VIDEO) > 0)
		{
			if (m_cAviReader.GetVideoInfo(&bitmapInfo) == ERR_READ_FILE_OK)
			{
				return true;
			}
		}
	}
	return false;
}

bool COneChnnReader::GetAudioInfo(WAVEFORMATEX &waveFormat)
{
	if (m_readerType == FILE_TYPE_FTVT)
	{
		if(m_cRecReader.GetFrameNum(FRAME_TYPE_AUDIO) > 0)
		{
			if (m_cRecReader.GetAudioInfo(&waveFormat) == ERR_READ_FILE_OK)
			{
				return true;
			}
		}
	}
	else
	{
		if(m_cAviReader.GetFrameNum(FRAME_TYPE_AUDIO) > 0)
		{
			if (m_cAviReader.GetAudioInfo(&waveFormat) == ERR_READ_FILE_OK)
			{
				return true;
			}
		}
	}
	return false;	
}

unsigned long COneChnnReader::GetCurrFrame(FRAME_INFO_EX &frameInfo)
{
	if (m_readerType == FILE_TYPE_FTVT)
	{
		return m_cRecReader.GetCurrFrame(frameInfo);
	}
	else
	{
		return m_cAviReader.GetCurrFrame(frameInfo);
	}
}


void COneChnnReader::RegisterReadFileIndex( )
{
	assert(m_fileList.GetCount() > 0);
	assert(NULL != m_currPos);
	assert(!m_bRegister);

	FILE_INFO_EX &reading = m_fileList.GetAt(m_currPos);
	m_readFileInfo = reading;
	m_pReclogMan->RegisterReadFile(m_readFileInfo.chnn, m_readFileInfo.diskIndex, m_readFileInfo.fileIndex);
	m_bRegister = true;
}


void COneChnnReader::CancelReadFileIndex( )
{
	assert(m_bRegister);
	m_pReclogMan->CancelReadFile(m_readFileInfo.chnn, m_readFileInfo.diskIndex, m_readFileInfo.fileIndex);
	m_bRegister = false;
}
















