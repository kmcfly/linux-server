/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : xzw
** Date         : 2011-6-28
** Name         : AsyncReader.cpp
** Version      : 3.1.9
** Description  : 
** Modify Record:
1:
***********************************************************************/
#include "AsyncReader.h"


CAsyncReader::CAsyncReader()
{
	m_readType = READ_TYPE_NULL;
	m_pFilePath = NULL;
	m_pReader = NULL;
	m_maxCHNum = 0;
	m_chnnBits	= 0;
	m_currReaderID = -1;
	m_showTime = MAX_TIME;
	m_bRealPause = false;
}

CAsyncReader::~CAsyncReader()
{

}

bool CAsyncReader::Initial(long maxCHNum, CReclogManEX *pReclogManEx, unsigned long diskOwnerIndex)
{
	assert(maxCHNum > 0);
	assert(m_pReader == NULL);

	m_pFilePath = new char [256];

	m_maxCHNum = maxCHNum;
	m_pReader = new LOCAL_READER[maxCHNum];
	for(int i = 0; i < maxCHNum; i++)
	{
		m_pReader[i].reader.Initial(pReclogManEx, diskOwnerIndex);
		m_pReader[i].bSwitch = false;
		m_pReader[i].time = 0;
		m_pReader[i].chnn = INVALID_INDEX(unsigned long);
		m_pReader[i].bforward =  true;
		m_pReader[i].baseFrameTime = 0;
		m_pReader[i].baseShowTime = 0;
		m_pReader[i].lastReadFrameTime=0;
		m_pReader[i].speed = 1;
		m_pReader[i].bPause = false;
		m_pReader[i].bPlayNext = false;
	}

	m_chnnBits = 0;
	return true;
}
void CAsyncReader::Quit()
{
	m_chnnBits = 0;

	if(m_pReader != NULL)
	{
		for(int i = 0; i < m_maxCHNum; i++)
		{
			m_pReader[i].reader.Quit();
		}

		delete [] m_pReader;
		m_pReader = NULL;
	}

	if (m_pFilePath != NULL)
	{
		delete [] m_pFilePath;
		m_pFilePath = NULL;
	}	
}

long CAsyncReader::StartRead(FILE_INFO_EX_LIST *pFileList, ULONGLONG & outCH, const char *pFilePath /* = NULL */)
{
	if(pFileList == NULL)
	{
		assert(false);
		return MR_RET_NOTHING_TO_READ;
	}

	if(pFileList->IsEmpty())
	{
		return MR_RET_NOTHING_TO_READ;
	}

	FILE_INFO_EX_LIST	*pFileListCH = new FILE_INFO_EX_LIST [m_maxCHNum];
	unsigned long	*pIndexToCH = new unsigned long [m_maxCHNum];

	if ((NULL == pFileListCH) || (NULL == pIndexToCH))
	{
		assert (false);
		return MR_RET_UNKNOWN_ERROR;
	}

	//先统计通道数目
	ULONGLONG chnnBits = 0;
	POS pos = pFileList->GetHeadPosition();
	while (NULL != pos)
	{
		FILE_INFO_EX & node = pFileList->GetNext(pos);
		chnnBits |= (static_cast<ULONGLONG>(0x01) << node.chnn);
	}

	//然后按照升序关系把通道和INDEX对应上
	unsigned char index = 0, chnn = 0;
	do 
	{
		if (0x01 & chnnBits)
		{
			pIndexToCH [index] = chnn;
			++ index;
		}

		chnnBits = (chnnBits >> 1);
		++ chnn;
	} while(0 != chnnBits);

	//取两者小为匹配数目
	unsigned char num = (index < m_maxCHNum) ? index : m_maxCHNum;
	
	//
	m_startTime = MAX_TIME;
	m_endTime = 0;
	outCH = 0;

	unsigned char i = 0;
	pos = pFileList->GetHeadPosition();
	while (NULL != pos)
	{
		FILE_INFO_EX & node = pFileList->GetNext(pos);

		m_startTime = (((LONGLONG)(node.startTime)*1000000) < m_startTime) ? ((LONGLONG)(node.startTime)*1000000) : m_startTime;
		m_endTime = (((LONGLONG)(node.endTime)*1000000) < m_endTime) ? m_endTime : ((LONGLONG)(node.endTime)*1000000);

		//在已经建立了的对应关系中找到相关项，则把文件加入列表后处理下一个文件。
		for (i=0; i<num; ++i)
		{
			if (pIndexToCH[i] == node.chnn)
			{
				pFileListCH [i].AddTail(node);
				break;
			}
		}
	}

	for (i = 0; i<num; ++i)
	{
		//调用单通道读取模块，注意为按文件列表读取的接口
		if (ONE_CHNN_READ_FAIL == m_pReader[i].reader.StartRead(pFileListCH[i], pFilePath))
		{
			m_pReader[i].bSwitch = false;
		}
		else
		{
			outCH |= ((ULONGLONG)0x01 << CHANNEL_ID(pIndexToCH[i]));//登记真正开启的通道
			m_pReader[i].bSwitch = true;
			m_pReader[i].chnn = pIndexToCH[i];
		}

		pFileListCH[i].RemoveAll();
	}

	//
	delete [] pFileListCH;
	delete [] pIndexToCH;
	
	//检查所有通道是否都是SLEEP状态，如果是则返回失败。
	if (CheckSleep())
	{
		outCH = 0;

		m_readType = READ_TYPE_NULL;
		m_chnnBits = outCH;

		return MR_RET_NOTHING_TO_READ;
	}
	else
	{
		m_readType = READ_BY_FILE_LIST;
		m_chnnBits = outCH;

		return MR_RET_OK;
	}
}

//要求通道映射列表按映射序号升序排列
long CAsyncReader::StartRead(unsigned long startTime, unsigned long endTime, \
							 const CHANNEL_LIST &chnnList, ULONGLONG & outCH, const char *pFilePath /* = NULL */)
{
	if(startTime >= endTime)
	{
		assert(false);
		return MR_RET_PARAM_ERROR;
	}

	if(chnnList.GetCount() > m_maxCHNum)
	{
		return MR_RET_TOO_MANY_CHANNELS;
	}

	//调用单通道读取模块，注意为按时间开始的接口。
	outCH = 0;
	unsigned char i = 0;
	unsigned long chnn = 0;
	POS pos = chnnList.GetHeadPosition();
	while(pos)
	{
		chnn = chnnList.GetNext(pos);

		if (m_pReader[i].reader.StartRead(startTime, endTime, chnn, pFilePath))
		{
			outCH |= ((ULONGLONG)0x01 << chnn);
			m_pReader[i].bSwitch = true;
			m_pReader[i].chnn = chnn;			
		}
		else
		{
			m_pReader[i].bSwitch = false;
		}

		++ i;
	}
	
	if(outCH == 0)
	{
		return MR_RET_NOTHING_TO_READ;
	}

	//检查所有通道是否都是SLEEP状态，如果是则返回失败。
	if (CheckSleep())
	{
		outCH = 0;

		m_readType = READ_TYPE_NULL;
		m_startTime = 0;
		m_endTime = 0;
		m_chnnBits = outCH;

		return MR_RET_NOTHING_TO_READ;
	}
	else
	{
		m_readType = READ_BY_START_END;
		m_startTime = (LONGLONG)(startTime)*1000000;
		m_endTime = (LONGLONG)(endTime)*1000000;
		m_chnnBits = outCH;

		if (NULL != pFilePath)
		{
			strcpy(m_pFilePath, pFilePath);
		}

		return MR_RET_OK;
	}
}

long CAsyncReader::StartRead(unsigned long startTime, const CHANNEL_LIST &chnnList, ULONGLONG & outCH, const char *pFilePath)
{
	if(chnnList.GetCount() > m_maxCHNum)
	{
		return MR_RET_TOO_MANY_CHANNELS;
	}

	//调用单通道读开始按时间读接口
	outCH = 0;
	unsigned char i = 0;
	unsigned long chnn = 0;
	POS pos = chnnList.GetHeadPosition();
	while(pos)
	{
		chnn = chnnList.GetNext(pos);

		if (m_pReader[i].reader.StartRead(startTime, chnn, pFilePath))
		{
			outCH |= ((ULONGLONG)0x01 << CHANNEL_ID(chnn));
			m_pReader[i].bSwitch = true;
			m_pReader[i].chnn = chnn;
		}
		else
		{
			m_pReader[i].bSwitch = false;
		}

		++ i;
	}

	//检查所有通道是否都是SLEEP状态，如果是则返回失败。
	if (CheckSleep())
	{
		outCH = 0;

		m_readType = READ_TYPE_NULL;
		m_startTime = 0;
		m_endTime = 0;
		m_chnnBits = outCH;

		return MR_RET_NOTHING_TO_READ;
	}
	else
	{
		m_readType = READ_BY_START;
		m_startTime = (LONGLONG)(startTime)*1000000;
		m_endTime = MAX_TIME;
		m_chnnBits = outCH;

		if (NULL != pFilePath)
		{
			strcpy(m_pFilePath, pFilePath);
		}

		return MR_RET_OK;
	}
}

void CAsyncReader::StopRead()
{
	if (m_pReader != NULL)
	{
		for(int i = 0; i < m_maxCHNum; i++)
		{
			m_pReader[i].bSwitch = false;
			m_pReader[i].time = 0;
			m_pReader[i].reader.StopRead();
		}
	}
	m_readType = READ_TYPE_NULL;
}

long CAsyncReader::SetCurTime(bool bForward, unsigned long time, ULONGLONG & outCH, unsigned long chnn)
{
	for (long index=0; index<m_maxCHNum; ++index)
	{
		if (chnn == m_pReader[index].chnn )
		{
			if (m_pReader[index].bSwitch)
			{
				if (ONE_CHNN_READ_FAIL != m_pReader[index].reader.SeekFrameToTime(bForward, time))
				{
					outCH = m_chnnBits;
				}
				else
				{
					m_pReader[index].bSwitch = false;
					m_pReader[index].chnn	 = INVALID_INDEX(unsigned long);
					outCH = m_chnnBits & (~(ULONGLONG)0x01 << CHANNEL_ID(m_pReader[index].chnn));
				}
			}
		}
	}

	//检查所有通道是否都是SLEEP状态，如果是则返回失败。
	if (CheckSleep())
	{
		outCH = 0;

		m_readType = READ_TYPE_NULL;
		m_startTime = 0;
		m_endTime = 0;
		m_chnnBits = outCH;

		return MR_RET_NOTHING_TO_READ;
	}
	else
	{
		m_chnnBits = outCH;

		return MR_RET_OK;
	}
}

/******************************************************************************

函数功能：设置当前读取数据的通道
参数说明：chnnBits - 要读取数据的通道掩码
返回值：

******************************************************************************/
long CAsyncReader::SetCurChannel(ULONGLONG chnnBits, ULONGLONG & outCH)
{
	//文件列表的形式，不支持切换通道。 
	if (READ_BY_FILE_LIST == m_readType)
	{
		outCH = m_chnnBits;
		return MR_RET_OK;
	}

	outCH = 0;

	assert (0 != chnnBits);
	if (0 == chnnBits)
	{
		return MR_RET_PARAM_ERROR;
	}

	if (chnnBits == m_chnnBits)
	{
		outCH = m_chnnBits;
		return MR_RET_OK;
	}

	//先检查已经打开的通道，并修改相关参数。
	for (unsigned char i=0; i<m_maxCHNum; ++i)
	{
		if (m_pReader[i].bSwitch && (((ULONGLONG)0x01 << CHANNEL_ID(m_pReader[i].chnn)) & chnnBits))
		{
			chnnBits &= ~((ULONGLONG)0x01 << CHANNEL_ID(m_pReader[i].chnn));
			outCH |= ((ULONGLONG)0x01 << CHANNEL_ID(m_pReader[i].chnn));

			//定位到当前时间的前关键帧，以便及时播放
			if (ONE_CHNN_READ_FAIL == m_pReader[i].reader.SeekFrameToTime(true, static_cast<unsigned long>(m_pReader[i].lastReadFrameTime/1000000)))
			{
				m_pReader[i].bSwitch = false;
				m_pReader[i].chnn	 = INVALID_INDEX(unsigned long);
				outCH &= ~((ULONGLONG)0x01 << CHANNEL_ID(m_pReader[i].chnn));
			}
		}
		else if (m_pReader[i].bSwitch)
		{
			m_pReader[i].bSwitch = false;
			m_pReader[i].chnn = INVALID_INDEX(unsigned long);
			m_pReader[i].time = 0;
			m_pReader[i].reader.StopRead();
		}
	}

	//如果等于零，则表示这次选择的是正播放通道的子集。
	if (0 == chnnBits)
	{
		m_chnnBits = outCH;
		return MR_RET_OK;
	}

	//assert (m_startTime <= m_lastTime);

	//如果0 != chnnBits，则意味有新的之前未开启的通道。
	while (0 != chnnBits)	//说明有新的通道加入，需要启动
	{
		//寻找到对应的通道号
		unsigned char chnn = 0;
		ULONGLONG CH = chnnBits;
		while ((0 != CH) && !(CH & 0x01))
		{
			CH = (CH >> 1);
			++ chnn;
		}

		chnnBits &= ~((ULONGLONG)(0x01) << chnn);

		unsigned char index = INVALID_INDEX(unsigned char);
		for (unsigned char i=0; i<m_maxCHNum; ++i)	//注意要检查所有资源通道。
		{
			if (!m_pReader[i].bSwitch)
			{
				index = i;//该资源可用。
				break;
			}
		}

		if (index != INVALID_INDEX(unsigned char))
		{
			if (READ_BY_START_END == m_readType)
			{
				if (ONE_CHNN_READ_FAIL != m_pReader[index].reader.StartRead(static_cast<unsigned long>(m_pReader[index].lastReadFrameTime/1000000), 
					static_cast<unsigned long>(m_endTime/1000000), chnn, m_pFilePath))
				{
					m_pReader[index].bSwitch = true;
					m_pReader[index].chnn	= chnn;
					m_pReader[index].time	= m_startTime;

					outCH |= ((ULONGLONG)0x01 << CHANNEL_ID(m_pReader[index].chnn));				
				}
			}
			else if (READ_BY_START == m_readType)
			{
				if (ONE_CHNN_READ_FAIL != m_pReader[index].reader.StartRead(static_cast<unsigned long>(m_pReader[index].lastReadFrameTime/1000000), chnn, m_pFilePath))
				{
					m_pReader[index].bSwitch = true;
					m_pReader[index].chnn	= chnn;
					m_pReader[index].time	= m_pReader[index].lastReadFrameTime;

					outCH |= ((ULONGLONG)0x01 << CHANNEL_ID(m_pReader[index].chnn));
				}
			}
			else
			{
				//读取文件列表的形式，则不打开新的通道
			}
		}
		else
		{
			return MR_RET_TOO_MANY_CHANNELS;
		}
	}

	//检查所有通道是否都是SLEEP状态，如果是则返回失败。
	if (CheckSleep())
	{
		outCH = 0;

		m_readType = READ_TYPE_NULL;
		m_startTime = 0;
		m_endTime = 0;
		m_chnnBits = outCH;

		return MR_RET_NOTHING_TO_READ;
	}
	else
	{
		m_chnnBits = outCH;

		return MR_RET_OK;
	}
}

long CAsyncReader::GetNextFrameType(FRAME_INFO_EX &frameInfo, unsigned long &readerID,bool &bneedchange)
{
	int ret=MR_RET_OK;//返回值
	bool bpause=false;
	m_currReaderID = GetNextReadChannel(bneedchange,ret);
	
	if (m_currReaderID == -1)//所有通道没有可播数据
	{
		if(ret == MR_RET_ALLCHNN_PAUSE)
		{
			return ret;
		}
		else
		{
			return MR_RET_NOTHING_TO_READ;
		}
		
	}

	frameInfo.frameType = m_pReader[m_currReaderID].reader.GetFrameType();
	frameInfo.channel	= CHANNEL_ID(m_pReader[m_currReaderID].chnn);
	readerID = m_currReaderID;
	return ret;
}
/******************************************************************************

函数功能：得到下一帧数据
参数说明：bVideoOnly - 是否只是读取视频数据
		  pFrameInfo - 保存要读取的一帧的信息及数据，传入时，length和pData必须填充
		  bForward - 是否是向正方向播放
返回值：成功返回MR_RET_OK
		否则返回MR_RET_NOTHING_TO_READ，表示已无数据可读

*******************************************************************************/
long CAsyncReader::GetNextFrame(FRAME_INFO_EX &frameInfo, unsigned long &readerID)
{
	if (m_currReaderID == -1)//所有通道没有可播数据
	{
		return MR_RET_NOTHING_TO_READ;
	}

	
	
	if(ONE_CHNN_READ_SUC != m_pReader[m_currReaderID].reader.GetFrame(frameInfo))
	{
		return MR_RET_NOTHING_TO_READ;
	}
	else
	{ 
        if ((FRAME_TYPE_VIDEO == frameInfo.frameType) || (FRAME_TYPE_AUDIO == frameInfo.frameType))
		{
			frameInfo.frameAttrib = PLAY_FRAME_SHOW;

			if (FRAME_TYPE_VIDEO == frameInfo.frameType)
			{

			}
			if(SEEK_FRAME_OK != m_pReader[m_currReaderID].reader.SeekFrame(SEEK_NEXT_VFRM))
			{
				frameInfo.frameAttrib = PLAY_FRAME_SEC_END;
			}
		}
		readerID = m_currReaderID;		
		
		return MR_RET_OK;
 	}
}

/******************************************************************************

函数功能：得到一关键帧
参数说明：nStepFrame - 取值1，2，... 分别表示得到下一关键帧，下下一帧关键帧...
		  bForward：读取关键帧的方向，true表示向正方向得到下一关键帧，false表示反方向
		  pFrameInfo：保存要读取的关键帧的信息及数据，传入时，length和pData必须填充
返回值：成功返回MR_RET_OK
		否则返回MR_RET_NOTHING_TO_READ，表示已无数据可读

*******************************************************************************/
long CAsyncReader::GetKeyFrame(FRAME_INFO_EX &frameInfo, long nStepFrame, bool bForward, unsigned long &readerID)
{
	if (m_currReaderID == -1)//所有通道没有可播数据
	{
		return MR_RET_NOTHING_TO_READ;
	}

	//可能第一次调用这个函数时，得到的当前帧非视频关键帧，所以要调用一次查找关键帧后再读
	while(true)
	{
		if (ONE_CHNN_READ_SUC !=m_pReader[m_currReaderID].reader.GetFrame(frameInfo) )
		{
			return MR_RET_NOTHING_TO_READ;
		}
		
		if (FRAME_TYPE_VIDEO == frameInfo.frameType)
		{
			frameInfo.frameAttrib = PLAY_FRAME_SHOW;			
		}
		
		if((FRAME_TYPE_VIDEO_FORMAT != frameInfo.frameType) && (FRAME_TYPE_AUDIO_FORMAT != frameInfo.frameType))
		{
			SEEK_MODE seekMode = bForward ? SEEK_NEXT_KEY : SEEK_PREV_KEY;
			if(SEEK_FRAME_OK != m_pReader[m_currReaderID].reader.SeekFrame(seekMode, nStepFrame))
			{
				frameInfo.frameAttrib = PLAY_FRAME_SEC_END;
			}

			if((frameInfo.frameType == FRAME_TYPE_VIDEO) && frameInfo.keyFrame)
			{
				break;
			}
		}
		else
		{
			break;
		}			
	}	
	
	readerID = m_currReaderID;	
	return MR_RET_OK;
}
bool CAsyncReader::HasAudioData(long channel)
{
//	return m_pReader[channel].reader.HasAudioData();
	return true;
}

//用于检测所有通道是否都处于READ_STATE_BEHIND_SLEEP状态。
bool CAsyncReader::CheckSleep()
{
	//如果所有开启成功的通道都是：这个时间点之前有数据，之后没有数据，那么就认为没有数据
	unsigned char readSucNum = 0;
	unsigned char behindSleepNum = 0;
	for (unsigned char i = 0; i < m_maxCHNum; ++ i)
	{
		if (m_pReader[i].bSwitch)
		{
			++ readSucNum;

			if (READ_STATE_BEHIND_SLEEP == m_pReader[i].reader.GetReadState())
			{
				++ behindSleepNum;
			}
		}
	}

	if (0 == readSucNum)
	{
		return true;//没有启动成功，也可以认为是都在SLEEP。
	}
	else
	{
		if (readSucNum == behindSleepNum)
		{
			for (unsigned char i = 0; i < m_maxCHNum; ++ i)
			{
				if (m_pReader[i].bSwitch)
				{
					m_pReader[i].reader.StopRead();
					m_pReader[i].bSwitch = false;
				}
			}

			return true;	//所有通道都在SLEEP，则返回正确。
		}

		return false;	//只要有一个通道不是SLEEP，则返回失败。
	}
}

/******************************************************************************

函数功能：获取下一帧
参数说明：frameInfo 帧信息数据
返回值： 成功返回MR_RET_OK，失败返回失败原因。

******************************************************************************/
long CAsyncReader::GetFrame(FRAME_INFO_EX &frameinfo, unsigned long &readerID)
{

	if (false == m_pReader[m_currReaderID].bforward)
	{
		
		return GetKeyFrame(frameinfo, 1, false, readerID);
	}
	else if (true == m_pReader[m_currReaderID].bJumping)
	{
		
		return GetKeyFrame(frameinfo, 1, true, readerID);
	}
	else
	{
		return GetNextFrame(frameinfo, readerID);
	}
}


/******************************************************************************

函数功能：设置某个通道播放方向
参数说明：chnn为通道号，bforward为是否向前播放
返回值：无

******************************************************************************/
void CAsyncReader::SetForward(unsigned long chnn, bool bforward)
{
	for (unsigned char i = 0; i < m_maxCHNum; ++ i)
	{
		if (m_pReader[i].chnn == chnn)
		{
			m_pReader[i].bforward = bforward;
		}
	}
}

/******************************************************************************

函数功能：设置某个通道播放速度
参数说明：chnn为通道号，speed为播放速度
返回值：  无

******************************************************************************/
void CAsyncReader::SetPlaySpeed(unsigned long chnn, unsigned short speed)
{
	for (unsigned char i = 0; i < m_maxCHNum; ++ i)
	{
		if (m_pReader[i].chnn == chnn)
		{
			m_pReader[i].speed = speed;
		}
	}
}

/******************************************************************************

函数功能：设置某个通道是否关键帧播放
参数说明：chnn为通道号，bJumping为是否关键帧播放
返回值：无

******************************************************************************/
void CAsyncReader::SetJumping(unsigned long chnn, bool bJumping)
{
	for (unsigned char i = 0; i < m_maxCHNum; ++ i)
	{
		if (m_pReader[i].chnn == chnn)
		{
			m_pReader[i].bJumping = bJumping;
		}
	}
}

/******************************************************************************

函数功能：设置暂停播放
参数说明：chnn为通道号， bPause为是否暂停
返回值：  无

******************************************************************************/
void CAsyncReader::SetPause(unsigned long chnn, bool bPause)
{
	for (unsigned char i = 0; i < m_maxCHNum; ++ i)
	{
		if (m_pReader[i].chnn == chnn)
		{
			m_pReader[i].bPause = bPause;
		}
	}
}


/******************************************************************************

函数功能：设置立即播放下一帧
参数说明：chnn为通道号，bPlayNext为是否播放下一帧
返回值：  无

******************************************************************************/
void CAsyncReader::SetPlayNext(unsigned long chnn, bool bPlayNext)
{
	for (unsigned char i = 0; i < m_maxCHNum; ++ i)
	{
		if (m_pReader[i].chnn == chnn)
		{
			m_pReader[i].bPlayNext = bPlayNext;
		}
	}
}

void CAsyncReader::SetBaseShowTime(unsigned long readerID, LONGLONG baseShowTime)
{
	m_pReader[readerID].baseShowTime = baseShowTime;
}

void CAsyncReader::SetBaseFrameTime(unsigned long readerID, LONGLONG baseFrameTime)
{
	m_pReader[readerID].baseFrameTime = baseFrameTime;
}

void CAsyncReader::SetLastReadFrameTime(unsigned long readerID, LONGLONG lastReadFrameTime)
{
	m_pReader[readerID].lastReadFrameTime = lastReadFrameTime;
}

LONGLONG CAsyncReader::GetBaseFrameTime(unsigned long readerID)
{
	return m_pReader[readerID].baseFrameTime;
}
/******************************************************************************

						Protected Functions

******************************************************************************/

/******************************************************************************
******************************************************************************/

int CAsyncReader::GetNextReadChannel(bool &bneedchange,int &chnnstatus)
{
	int nextReadChannel = -1;
	LONGLONG	showTime= 0;
	m_showTime  = MAX_TIME;
	int Pausecount=0,bswitchcount=0;
	LONGLONG timeInterval=0;
	
	for(int i = 0; i < m_maxCHNum; i++)
	{
		if(m_pReader[i].bSwitch)
		{
			bswitchcount++;
			if (m_pReader[i].bPause == true && m_pReader[i].bPlayNext == false && m_bRealPause)
			{
				Pausecount++;
				continue;
			}

			//只有处于活动状态的通道和反向播放中进入的睡眠才参与时间排序
			int teststatus = m_pReader[i].reader.GetReadState();	
			
			if((teststatus == READ_STATE_ACTION) \
				|| (teststatus == READ_STATE_FORWARD_SLEEP))
			{
				m_pReader[i].time = m_pReader[i].reader.GetFrameTime();
				
				if (m_pReader[i].time > m_pReader[i].baseFrameTime)
				{
					timeInterval = m_pReader[i].time - m_pReader[i].lastReadFrameTime;
					if (m_pReader[i].baseFrameTime != 0)
					{
						if(timeInterval>120000000)
						{
							//两端时间间隔超过60秒，时间间隔给为0,第一帧的的时间改为当前帧的显示时间
							if(m_pReader[i].time!=0)
							{
								m_pReader[i].baseFrameTime = m_pReader[i].time;
								bneedchange=true;
							}
							
						}
						else if(0 == timeInterval)//说明有数据前进到头了
						{
							continue;
						}
						
					}
					if (m_pReader[i].speed >> 8)
					{
						showTime = m_pReader[i].baseShowTime + (m_pReader[i].time - m_pReader[i].baseFrameTime) * (m_pReader[i].speed &(~(0x1 << 8)));
					}
					else
					{
						showTime = m_pReader[i].baseShowTime + (m_pReader[i].time - m_pReader[i].baseFrameTime) / m_pReader[i].speed;
					}					
				}
				else
				{
					timeInterval = m_pReader[i].lastReadFrameTime -m_pReader[i].time;
					if(timeInterval > 0)
					{
						if (m_pReader[i].baseFrameTime != 0)
						{
							if(timeInterval>120000000)
							{
								//两端时间间隔超过60秒，时间间隔给为0,第一帧的的时间改为当前帧的显示时间
								if(m_pReader[i].time!=0)
								{
									m_pReader[i].baseFrameTime = m_pReader[i].time;
									bneedchange=true;
								}

							}
						}
					}
					else if(0 == timeInterval)//说明有数据后退到头了
					{
						continue;
					}
					if (m_pReader[i].speed >> 8)
					{
						showTime = m_pReader[i].baseShowTime + (m_pReader[i].baseFrameTime -m_pReader[i].time) * (m_pReader[i].speed &(~(0x1 << 8)));
					}
					else
					{
						showTime = m_pReader[i].baseShowTime + (m_pReader[i].baseFrameTime -m_pReader[i].time) / m_pReader[i].speed;
					}
				}
			}
			
			if(showTime!=0)
			{			
				if (showTime < m_showTime)
				{
					m_showTime = showTime;
					nextReadChannel = i;
					
				}
			}
			
			if(m_pReader[i].bPlayNext == true)
			{
				m_pReader[i].bPlayNext = false;
				nextReadChannel = i;
				break;
			}
			if(m_pReader[i].baseFrameTime == 0)
			{
				return i;
			}
			
		}//end if(m_pReader[i].bSwitch)
	}

	//唤醒在播放中进入的睡眠
	if((nextReadChannel != -1) && \
		((m_pReader[nextReadChannel].reader.GetReadState() == READ_STATE_FORWARD_SLEEP) \
		|| (m_pReader[nextReadChannel].reader.GetReadState() == READ_STATE_BEHIND_SLEEP)) )
	{
		m_pReader[nextReadChannel].reader.WakeUp();  
	}
	if(bswitchcount==Pausecount)
	{
		chnnstatus = MR_RET_ALLCHNN_PAUSE;
	}
	
	return nextReadChannel;
}


