// MultiReader.cpp: implementation of the CMultiReader class.
//
/*////////////////////////////////////////////////////////////////////
更改记录:
2010-01-18 09:43 (YSW)    
修改前只有一个StartRead和SetCurrChannel接口检查了所有通道是否都处于后
SLEEP状态，比如SetTime就没有检查，这样当定位到某个点（后面所有通道都没有
数据），则也会返回成功（但是获取数据会失败）。所以增加检查Sleep的方法，用于
检查所有通道是否都处于Sleep状态。

/////////////////////////////////////////////////////////////////////*/
#include "MultiReader.h"
#include <omp.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMultiReader::CMultiReader()
{
	m_readType = READ_TYPE_NULL;
	m_pFilePath = NULL;
	m_pReader = NULL;
	m_maxCHNum = 0;
	m_chnnBits	= 0;
}

CMultiReader::~CMultiReader()
{

}

bool CMultiReader::Initial(long maxCHNum, CReclogManEX *pReclogManEx, unsigned long diskOwnerIndex)
{
	assert(maxCHNum > 0);
	assert(m_pReader == NULL);

	m_pFilePath = new char [256];
	if (NULL == m_pFilePath)
	{
		printf("%s:%s:%d, new m_pFilePath\n", __FUNCTION__, __FILE__, __LINE__);
		fflush(stderr);
		return false;
	}
	else
	{
		memset(m_pFilePath, 0, 256);
	}

	m_maxCHNum = maxCHNum;
	m_pReader = new LOCAL_READER[maxCHNum];
	for(int i = 0; i < maxCHNum; i++)
	{
		m_pReader[i].reader.Initial(pReclogManEx, diskOwnerIndex);
		m_pReader[i].bSwitch = false;
		m_pReader[i].time = 0;
		m_pReader[i].chnn = INVALID_INDEX(unsigned long);
	}

	m_chnnBits = 0;
	return true;
}
void CMultiReader::Quit()
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

	delete [] m_pFilePath;
	m_pFilePath = NULL;
}

long CMultiReader::StartRead(FILE_INFO_EX_LIST *pFileList, ULONGLONG & outCH, const char *pFilePath /* = NULL */)
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
	int num = (index < m_maxCHNum) ? index : m_maxCHNum;
	
	//
	m_startTime = MAX_TIME;
	m_endTime = 0;
	outCH = 0;

	int i = 0;
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

	ULONGLONG outCHT[128] = {0};

	#pragma omp parallel for num_threads(num)
	for (i = 0; i<num; ++i)
	{
		//调用单通道读取模块，注意为按文件列表读取的接口
		if (ONE_CHNN_READ_FAIL == m_pReader[i].reader.StartRead(pFileListCH[i], pFilePath))
		{
			m_pReader[i].bSwitch = false;
		}
		else
		{
			outCHT[i] = ((ULONGLONG)0x01 << CHANNEL_ID(pIndexToCH[i]));//登记真正开启的通道
			m_pReader[i].bSwitch = true;
			m_pReader[i].chnn = pIndexToCH[i];
		}

		pFileListCH[i].RemoveAll();
	}

	for (i = 0; i<num; ++i)
	{
		outCH |= outCHT[i];
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
long CMultiReader::StartRead(unsigned long startTime, unsigned long endTime, \
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
#if 0
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
#endif

	ULONGLONG outCHT[128] = {0};
	
#if 1
	int iCount = chnnList.GetCount();

	#pragma omp parallel for num_threads(iCount)
	for(int i=0; i<iCount; i++)
	{
		POS pos = chnnList.GetHeadPosition();
		int j=0;
		int channel = 0;

		while(pos)
		{
			if(i==j)
			{
				break;
			}
			channel = chnnList.GetNext(pos);
			j++;
			
		}

		channel = chnnList.GetAt(pos);
		if (m_pReader[i].reader.StartRead(startTime, endTime, channel, pFilePath))
		{
			outCHT[i] = ((ULONGLONG)0x01 << channel);
			m_pReader[i].bSwitch = true;
			m_pReader[i].chnn = channel;
		}
		else
		{
			m_pReader[i].bSwitch = false;
		}
	}
#endif
	
	for(int i=0; i<iCount; i++)
	{
		outCH |= outCHT[i];
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

long CMultiReader::StartRead(unsigned long startTime, const CHANNEL_LIST &chnnList, ULONGLONG & outCH, const char *pFilePath)
{
	if(chnnList.GetCount() > m_maxCHNum)
	{
		return MR_RET_TOO_MANY_CHANNELS;
	}

	//调用单通道读开始按时间读接口
	outCH = 0;
	unsigned char i = 0;
	unsigned long chnn = 0;

#if 0
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
#endif

	ULONGLONG outCHT[128] = {0};

#if 1
	int iCount = chnnList.GetCount();
	
	#pragma omp parallel for num_threads(iCount)
	for(int i=0;i<iCount;i++)
	{
		POS pos = chnnList.GetHeadPosition();
		int j=0;
		int channel = 0;
		while(pos)
		{
			if(i==j)
			{
				break;
			}
			channel = chnnList.GetNext(pos);
			j++;
			
		}
		channel = chnnList.GetAt(pos);

		if (m_pReader[i].reader.StartRead(startTime, channel, pFilePath))
		{
			outCHT[i] = ((ULONGLONG)0x01 << CHANNEL_ID(channel));
			m_pReader[i].bSwitch = true;
			m_pReader[i].chnn = channel;
		}
		else
		{
			m_pReader[i].bSwitch = false;
		}
	}
#endif

	for(int i=0; i<iCount; i++)
	{
		outCH |= outCHT[i];
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

void CMultiReader::StopRead()
{
	for(int i = 0; i < m_maxCHNum; i++)
	{
		m_pReader[i].bSwitch = false;
		m_pReader[i].time = 0;
		m_pReader[i].reader.StopRead();
	}

	m_readType = READ_TYPE_NULL;
}

long CMultiReader::SetCurTime(bool bForward, unsigned long time, ULONGLONG & outCH)
{
	ULONGLONG outCHArray[64];
	memset(outCHArray,0,sizeof(outCHArray));

	outCH = 0;
	#pragma omp parallel for num_threads(m_maxCHNum)
	for (long index=0; index<m_maxCHNum; ++index)
	{
		if (m_pReader[index].bSwitch)
		{
			if (ONE_CHNN_READ_FAIL != m_pReader[index].reader.SeekFrameToTime(bForward, time))
			{
				outCHArray[index] |= (ULONGLONG)0x01 << CHANNEL_ID(m_pReader[index].chnn);
 			}
			else
			{
				m_pReader[index].bSwitch = false;
				m_pReader[index].chnn	 = INVALID_INDEX(unsigned long);
			}
		}
	}

	for (long index=0; index<m_maxCHNum; ++index)
	{
		outCH |= outCHArray[index];
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
long CMultiReader::SetCurChannel(ULONGLONG chnnBits, ULONGLONG & outCH)
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
			if (ONE_CHNN_READ_FAIL == m_pReader[i].reader.SeekFrameToTime(true, static_cast<unsigned long>(m_lastTime/1000000)))
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


	//改为openMP并行读取文件，这样加快回放时切换回放通道的速度。真正放入多核处理的变量必须具有独立性，所以这里做了特殊初始化。
	ULONGLONG outCHArray[64];
	unsigned char chnnNumNew = 0;
	unsigned char chnnArray[64];
	unsigned char readerArray[64];
	memset(outCHArray,0,sizeof(outCHArray));
	memset(chnnArray,0,sizeof(chnnArray));
	memset(readerArray,0,sizeof(readerArray));
	while (0 != chnnBits)
	{
		ULONGLONG CH = chnnBits;
		while ((0 != CH) && !(CH & 0x01))
		{
			CH = (CH >> 1);
			++ (chnnArray[chnnNumNew]);
		}
		chnnBits &= ~((ULONGLONG)(0x01) << (chnnArray[chnnNumNew]));

		readerArray[chnnNumNew] = INVALID_INDEX(unsigned char);
		for (unsigned char i=0; i<m_maxCHNum; ++i)	//注意要检查所有资源通道。
		{
			if (!m_pReader[i].bSwitch)
			{
				readerArray[chnnNumNew] = i;//该资源可用。
				m_pReader[i].bSwitch = true;
				m_pReader[i].chnn	= chnnArray[chnnNumNew];
				m_pReader[i].time	= m_startTime;
				break;
			}
		}
		if (readerArray[chnnNumNew] == INVALID_INDEX(unsigned char))
		{
			return MR_RET_TOO_MANY_CHANNELS;
		}

		chnnNumNew++;
	}

	//如果0 != chnnBits，则意味有新的之前未开启的通道。
#pragma omp parallel for num_threads(chnnNumNew)
	for(int chnnArrayIndex = 0;chnnArrayIndex<chnnNumNew;++chnnArrayIndex) //说明有新的通道加入，需要启动
	{
		unsigned char chnn = chnnArray[chnnArrayIndex];
		unsigned char index = readerArray[chnnArrayIndex];

		if (index != INVALID_INDEX(unsigned char))
		{
			if (READ_BY_START_END == m_readType)
			{
				if (ONE_CHNN_READ_FAIL != m_pReader[index].reader.StartRead(static_cast<unsigned long>(m_lastTime/1000000), 
					static_cast<unsigned long>(m_endTime/1000000), chnn, m_pFilePath))
				{
					//m_pReader[index].bSwitch = true;
					//m_pReader[index].chnn = chnn;
					//m_pReader[index].time = m_startTime;

					outCHArray[chnnArrayIndex] |= ((ULONGLONG)0x01 << CHANNEL_ID(m_pReader[index].chnn));				
				}
			}
			else if (READ_BY_START == m_readType)
			{
				if (ONE_CHNN_READ_FAIL != m_pReader[index].reader.StartRead(static_cast<unsigned long>(m_lastTime/1000000), chnn, m_pFilePath))
				{
					//m_pReader[index].bSwitch = true;
					//m_pReader[index].chnn = chnn;
					//m_pReader[index].time = m_lastTime;

					outCHArray[chnnArrayIndex] |= ((ULONGLONG)0x01 << CHANNEL_ID(m_pReader[index].chnn));
				}
			}
			else
			{
				//读取文件列表的形式，则不打开新的通道
			}
		}
	}

	for (unsigned char chnnArrayIndex = 0;chnnArrayIndex<chnnNumNew;++chnnArrayIndex)
	{
		outCH |= outCHArray[chnnArrayIndex];
	}
	//至此，OpenMP处理结束。


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

long CMultiReader::GetNextFrameType(FRAME_INFO_EX &frameInfo, bool bForward)
{
	int index = GetNextReadChannel(bForward);
	if (index == -1)//所有通道没有可播数据
	{
		return MR_RET_NOTHING_TO_READ;
	}

	frameInfo.frameType = m_pReader[index].reader.GetFrameType();
	frameInfo.channel	= CHANNEL_ID(m_pReader[index].chnn);
	return MR_RET_OK;
}
/******************************************************************************

函数功能：得到下一帧数据
参数说明：bVideoOnly - 是否只是读取视频数据
		  pFrameInfo - 保存要读取的一帧的信息及数据，传入时，length和pData必须填充
		  bForward - 是否是向正方向播放
返回值：成功返回MR_RET_OK
		否则返回MR_RET_NOTHING_TO_READ，表示已无数据可读

*******************************************************************************/
long CMultiReader::GetNextFrame(FRAME_INFO_EX &frameInfo, bool bForward /* = true */, bool bVideoOnly /* = false */)
{
	int index = GetNextReadChannel(bForward);
	if (index == -1)//所有通道没有可播数据
	{
		return MR_RET_NOTHING_TO_READ;
	}

	if(ONE_CHNN_READ_SUC != m_pReader[index].reader.GetFrame(frameInfo))
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
				m_lastTime = frameInfo.time;//记录最近读出的一帧的时间
			}
			
			if(SEEK_FRAME_OK != m_pReader[index].reader.SeekFrame(SEEK_NEXT_VFRM))
			{
				frameInfo.frameAttrib = PLAY_FRAME_SEC_END;
			}
		}
		
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
long CMultiReader::GetKeyFrame(FRAME_INFO_EX &frameInfo, long nStepFrame /* = 1 */, bool bForward /* = true */)
{
	int index = GetNextReadChannel(bForward);

	if (index == -1)//所有通道没有可播数据
	{
		return MR_RET_NOTHING_TO_READ;
	}

	//可能第一次调用这个函数时，得到的当前帧非视频关键帧，所以要调用一次查找关键帧后再读
	while(true)
	{
		while ((index != -1) && (ONE_CHNN_READ_SUC != m_pReader[index].reader.GetFrame(frameInfo)))
		{
			index = GetNextReadChannel(bForward);
		}
		
		if (index == -1)//所有通道没有可播数据
		{
			return MR_RET_NOTHING_TO_READ;
		}
		else
		{
			if (FRAME_TYPE_VIDEO == frameInfo.frameType)
			{
				frameInfo.frameAttrib = PLAY_FRAME_SHOW;
				if (FRAME_TYPE_VIDEO == frameInfo.frameType)
				{
					m_lastTime = frameInfo.time;//记录最近读出的一帧的时间
				}
			}
			
			if((FRAME_TYPE_VIDEO_FORMAT != frameInfo.frameType) && (FRAME_TYPE_AUDIO_FORMAT != frameInfo.frameType))
			{
				SEEK_MODE seekMode = bForward ? SEEK_NEXT_KEY : SEEK_PREV_KEY;
				if(SEEK_FRAME_OK != m_pReader[index].reader.SeekFrame(seekMode, nStepFrame))
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
	}
	
	return MR_RET_OK;
}
bool CMultiReader::HasAudioData(long channel)
{
//	return m_pReader[channel].reader.HasAudioData();
	return true;
}

//用于检测所有通道是否都处于READ_STATE_BEHIND_SLEEP状态。
bool CMultiReader::CheckSleep()
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

						Protected Functions

******************************************************************************/

/******************************************************************************

函数功能：得到下一个要读取数据的通道(依照时间顺序)
参数说明：bForwardRead，TRUE表示正向播放，FALSE表示反向播放
返回值：找到下一个要读取的通道，返回通道号，否则返回-1

******************************************************************************/
int CMultiReader::GetNextReadChannel(bool bForwardRead)
{
	int nextReadChannel = -1;
	if(bForwardRead)
	{
		LONGLONG EarlistTime = MAX_TIME;
		for(int i = 0; i < m_maxCHNum; i++)
		{
			if(m_pReader[i].bSwitch)
			{
				//只有处于活动状态的通道和反向播放中进入的睡眠才参与时间排序
				if((m_pReader[i].reader.GetReadState() == READ_STATE_ACTION) \
					|| (m_pReader[i].reader.GetReadState() == READ_STATE_FORWARD_SLEEP))
				{
					m_pReader[i].time = m_pReader[i].reader.GetFrameTime();
					if(m_pReader[i].time < EarlistTime)
					{
						EarlistTime = m_pReader[i].time;
						nextReadChannel = i;
					}
				}
			}//end if(m_pReader[i].bSwitch)
		}
	}//end if(bForward)
	else
	{
		LONGLONG LatestTime = 0;
		for(int i = 0; i < m_maxCHNum; i++)
		{
			if(m_pReader[i].bSwitch)
			{
				//只有处于活动状态的通道和在正向播放中进入的睡眠才参与时间排序
				if((m_pReader[i].reader.GetReadState() == READ_STATE_ACTION) \
					|| (m_pReader[i].reader.GetReadState() == READ_STATE_BEHIND_SLEEP))
				{
					m_pReader[i].time = m_pReader[i].reader.GetFrameTime();
					if(m_pReader[i].time > LatestTime)
					{
						LatestTime = m_pReader[i].time;
						nextReadChannel = i;
					}
				}
			}//end if(m_pReader[i].bSwitch)
		}
	}//end if(!bForward)

	//唤醒在播放中进入的睡眠
	if((nextReadChannel != -1) && \
		((m_pReader[nextReadChannel].reader.GetReadState() == READ_STATE_FORWARD_SLEEP) \
		|| (m_pReader[nextReadChannel].reader.GetReadState() == READ_STATE_BEHIND_SLEEP)) )
	{
		m_pReader[nextReadChannel].reader.WakeUp();  
	}

	return nextReadChannel;
}
