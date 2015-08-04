#include "NetDevicePlatform.h"
#include "IPDataBufferMan.h"
#include <assert.h>
#include <string.h>
#include <stdio.h>


#ifdef __ENVIRONMENT_LINUX__
#include <errno.h>
#include <sys/time.h>
#endif

CIPDataBufferMan::CIPDataBufferMan(void)
{
	m_decChipNum		= 0;
	m_pChannelDecChip	= NULL;
	m_pDataBufferInfo	= NULL;
	m_bAction			= false;

	m_blockSize			= MAX_DATA_BLOCK_LEN;
}

CIPDataBufferMan::~CIPDataBufferMan(void)
{
}

bool CIPDataBufferMan::Initial(NET_DEVICE_STREAM_TYPE streamType)
{
	m_pChannelDecChip	= NULL;
	m_decChipNum		= 0;
	m_bAction			= true;
	m_pSupportVideoSize	= NULL;
	m_pStreamInfo		= NULL;

	
	m_streamType = streamType;

	return true;
}

void CIPDataBufferMan::Quit()
{
	if(m_pStreamInfo != NULL)
	{
		delete [] m_pStreamInfo;
		m_pStreamInfo = NULL;
	}

	if(m_pSupportVideoSize != NULL)
	{
		delete [] m_pSupportVideoSize;
		m_pSupportVideoSize = NULL;
	}

	if(m_pChannelDecChip != NULL)
	{
		delete [] m_pChannelDecChip;
		m_pChannelDecChip = NULL;
	}

	if(m_pbNeedKeyFrame != NULL)
	{
		delete [] m_pbNeedKeyFrame;
		m_pbNeedKeyFrame = NULL;
	}

	if(m_pDataBufferInfo != NULL)
	{
		for(int i = 0; i < m_decChipNum; i++)
		{
			if(m_pDataBufferInfo[i].pLock != NULL)
			{
				delete m_pDataBufferInfo[i].pLock;
			}
		}

		delete [] m_pDataBufferInfo;
		m_pDataBufferInfo = NULL;
	}
}

void CIPDataBufferMan::SetDecodeChipNum(long decodeChipNum)
{
	m_decChipNum = decodeChipNum;
	m_pDataBufferInfo = new IP_DATA_BUFFER_INFO[decodeChipNum];

	assert(m_pDataBufferInfo != NULL);

	memset(m_pDataBufferInfo, 0, sizeof(IP_DATA_BUFFER_INFO) * decodeChipNum);
	for(int i = 0; i < decodeChipNum; i++)
	{
		m_pDataBufferInfo[i].pLock = new CPUB_Lock_IPD();
	}
}

void CIPDataBufferMan::SetChannelDecChip(long channelNum, long *pDecChip)
{
	assert(m_pChannelDecChip == NULL);
	if(m_pChannelDecChip != NULL)
	{
		delete [] m_pChannelDecChip;
		m_pChannelDecChip = NULL;
	}

	m_channelNum = channelNum;

	m_pChannelDecChip = new long[channelNum];
	m_pbNeedKeyFrame = new bool[channelNum];
	m_pStreamInfo = new CHANNEL_STREAM_INFO[channelNum];

	for(long channel = 0; channel < channelNum; channel++)
	{
		m_pChannelDecChip[channel] = pDecChip[channel];
		m_pbNeedKeyFrame[channel] = true;
		m_pStreamInfo[channel].bSupport = true;
		m_pStreamInfo[channel].width = 0;
		m_pStreamInfo[channel].height = 0;
	}
}

void CIPDataBufferMan::SetChipBuffer(long chipId, unsigned char *pDataBuffer, unsigned long bufSize)
{
	m_pDataBufferInfo[chipId].pLock->Lock();
	m_pDataBufferInfo[chipId].bufferSize = bufSize;
	m_pDataBufferInfo[chipId].pBuffer = pDataBuffer;
	m_pDataBufferInfo[chipId].dataLen = 0;
	m_pDataBufferInfo[chipId].freeLen = bufSize;
	m_pDataBufferInfo[chipId].freeOffset = 0;
	m_pDataBufferInfo[chipId].writeOffset = 0;
	m_pDataBufferInfo[chipId].readOffset = 0;
	m_pDataBufferInfo[chipId].tailOffset = bufSize;

	memset(m_pDataBufferInfo[chipId].dataBlocks, 0, sizeof(ONE_DATA_BLOCK)*BLOCK_NUM);

	m_pDataBufferInfo[chipId].dataBlocks[0].pData = pDataBuffer;
	m_pDataBufferInfo[chipId].pLock->UnLock();
}

void CIPDataBufferMan::SetSupportVideoSize(unsigned long *pSupportVideoSize, long channelNum)
{
	if(NULL == pSupportVideoSize)
	{
		return;
	}

	m_pSupportVideoSize = new unsigned long[channelNum];
	memcpy(m_pSupportVideoSize, pSupportVideoSize, sizeof(unsigned long)*channelNum);
}

void CIPDataBufferMan::SetOneBlockSize(unsigned long blockSize)
{
	blockSize = (blockSize < 128*1024) ? 128*1024 : blockSize;
	blockSize = (blockSize > 1024 * 1024) ? 1024 * 1024 : blockSize;

	m_blockSizeLock.Lock();
	m_blockSize = blockSize;
	m_blockSizeLock.UnLock();
}

long CIPDataBufferMan::GetBufferedNum(long chipId)
{
	assert(chipId < m_decChipNum);

	long bufferedNum = 0;

	m_pDataBufferInfo[chipId].pLock->Lock();
	for(int i = 0; i < BLOCK_NUM; i++)
	{
		if((NULL == m_pDataBufferInfo[chipId].dataBlocks[i].pData) \
			|| (m_pDataBufferInfo[chipId].dataBlocks[i].dataLen <= 0))
		{
			bufferedNum = i;
			break;
		}
	}
	m_pDataBufferInfo[chipId].pLock->UnLock();

	return bufferedNum;
}

bool CIPDataBufferMan::GetDataBuffer(long chipId, unsigned char *&pBuffer, unsigned long &dataLen)
{
	assert(m_pDataBufferInfo != NULL);
	assert(chipId < m_decChipNum);

	m_pDataBufferInfo[chipId].pLock->Lock();

	if(!m_bAction)
	{
		m_pDataBufferInfo[chipId].pLock->UnLock();
		return false;
	}

	ONE_DATA_BLOCK &oneDataBlock = m_pDataBufferInfo[chipId].dataBlocks[0];
	if(oneDataBlock.dataLen <= 0)
	{
		m_pDataBufferInfo[chipId].pLock->UnLock();
		return false;
	}

	pBuffer = oneDataBlock.pData;
	dataLen = oneDataBlock.dataLen;
	assert(pBuffer != NULL);
	assert(dataLen > 0);

//	printf("-------------GetBuffer : %p, length = %d\n", pBuffer, dataLen);

	m_pDataBufferInfo[chipId].readOffset += dataLen;
	if(m_pDataBufferInfo[chipId].readOffset >= m_pDataBufferInfo[chipId].tailOffset)
	{
		m_pDataBufferInfo[chipId].readOffset = 0;
	}
	m_pDataBufferInfo[chipId].dataLen -= dataLen;

	//下一个数据块已经定位
	if(m_pDataBufferInfo[chipId].dataBlocks[1].pData != NULL)
	{
		memmove(m_pDataBufferInfo[chipId].dataBlocks, m_pDataBufferInfo[chipId].dataBlocks+1, sizeof(ONE_DATA_BLOCK)*(BLOCK_NUM-1));
		memset(&m_pDataBufferInfo[chipId].dataBlocks[BLOCK_NUM-1], 0, sizeof(ONE_DATA_BLOCK));
		
		m_pDataBufferInfo[chipId].pLock->UnLock();
		return true;
	}

	m_pDataBufferInfo[chipId].dataLen = 0;

	//由于缓冲区中数据长度为零，故读写位置相同

	if(m_pDataBufferInfo[chipId].writeOffset != m_pDataBufferInfo[chipId].readOffset)
	{
		printf("pBuffer:%p, writeOffset:%d, readOffset:%d, freeOffset:%d, tailOffset:%d\n", \
			m_pDataBufferInfo[chipId].pBuffer, \
			m_pDataBufferInfo[chipId].writeOffset, \
			m_pDataBufferInfo[chipId].readOffset, \
			m_pDataBufferInfo[chipId].freeOffset, \
			m_pDataBufferInfo[chipId].tailOffset);
	}
	assert(m_pDataBufferInfo[chipId].writeOffset == m_pDataBufferInfo[chipId].readOffset);

	//把新的写入位置加入到列表
	m_pDataBufferInfo[chipId].dataBlocks[0].pData = m_pDataBufferInfo[chipId].pBuffer + m_pDataBufferInfo[chipId].writeOffset;
	m_pDataBufferInfo[chipId].dataBlocks[0].dataLen = 0;

	m_pDataBufferInfo[chipId].pLock->UnLock();
	return true;
}

void CIPDataBufferMan::ReleaseDataBuffer(long chipId, unsigned char *pBuffer, unsigned long dataLen)
{
	assert(m_pDataBufferInfo != NULL);
	assert(chipId < m_decChipNum);

	m_pDataBufferInfo[chipId].pLock->Lock();

//	printf("%p, %p, %p\n",m_pDataBufferInfo[chipId].pBuffer, pBuffer, pBuffer+dataLen);

	//应该从上次释放的位置释放，即要求顺序释放
	if((pBuffer != (m_pDataBufferInfo[chipId].pBuffer+m_pDataBufferInfo[chipId].freeOffset)) \
		&& (pBuffer != m_pDataBufferInfo[chipId].pBuffer))
	{
		printf("pBuffer:%p, pHead:%p, freeOffset:%d, tailOffset:%d\n", pBuffer, m_pDataBufferInfo[chipId].pBuffer, \
			m_pDataBufferInfo[chipId].freeOffset, m_pDataBufferInfo[chipId].tailOffset);

		printf("pBuffer:%p, writeOffset:%d, readOffset:%d, freeOffset:%d, tailOffset:%d\n", \
			m_pDataBufferInfo[chipId].pBuffer, \
			m_pDataBufferInfo[chipId].writeOffset, \
			m_pDataBufferInfo[chipId].readOffset, \
			m_pDataBufferInfo[chipId].freeOffset, \
			m_pDataBufferInfo[chipId].tailOffset);

		assert(pBuffer == (m_pDataBufferInfo[chipId].pBuffer+m_pDataBufferInfo[chipId].freeOffset));
	}

	//释放的结束位置不应超出缓冲区右边界
	if((pBuffer+dataLen) > (m_pDataBufferInfo[chipId].pBuffer+m_pDataBufferInfo[chipId].bufferSize))
	{
		printf("pBuffer:%p, pHead:%p, dataLen:%d\n", pBuffer, m_pDataBufferInfo[chipId].pBuffer, dataLen);
		assert((pBuffer+dataLen) <= (m_pDataBufferInfo[chipId].pBuffer+m_pDataBufferInfo[chipId].bufferSize));
	}

	//当释放的缓冲区起始位置在读取位置左侧时，释放的右边界也应在读取位置的左侧
	if( (pBuffer < (m_pDataBufferInfo[chipId].pBuffer+m_pDataBufferInfo[chipId].readOffset))
		&& ((pBuffer+dataLen) > (m_pDataBufferInfo[chipId].pBuffer+m_pDataBufferInfo[chipId].readOffset)) )
	{
		printf("pBuffer:%p, pHead:%p, readoffset:%d, dataLen:%x\n", pBuffer, m_pDataBufferInfo[chipId].pBuffer, \
			m_pDataBufferInfo[chipId].readOffset, dataLen);
		assert(false);
		m_pDataBufferInfo[chipId].pLock->UnLock();
		return;
	}

	m_pDataBufferInfo[chipId].freeOffset = pBuffer + dataLen - m_pDataBufferInfo[chipId].pBuffer;
	m_pDataBufferInfo[chipId].freeLen += dataLen;

	//已经释放到缓冲区结尾
	if(m_pDataBufferInfo[chipId].freeOffset >= m_pDataBufferInfo[chipId].tailOffset)
	{
		if(m_pDataBufferInfo[chipId].writeOffset > m_pDataBufferInfo[chipId].tailOffset)
		{
			printf("pBuffer:%p, writeOffset:%d, readOffset:%d, freeOffset:%d, tailOffset:%d\n", \
									m_pDataBufferInfo[chipId].pBuffer, \
									m_pDataBufferInfo[chipId].writeOffset, \
									m_pDataBufferInfo[chipId].readOffset, \
									m_pDataBufferInfo[chipId].freeOffset, \
									m_pDataBufferInfo[chipId].tailOffset);
		}
		assert(m_pDataBufferInfo[chipId].writeOffset <= m_pDataBufferInfo[chipId].tailOffset);

		//此时tailOffset==bufferSize，且缓冲区中没有数据，且全部为释放状态
		if(m_pDataBufferInfo[chipId].writeOffset >= m_pDataBufferInfo[chipId].tailOffset)
		{
			m_pDataBufferInfo[chipId].freeLen = m_pDataBufferInfo[chipId].bufferSize;
			m_pDataBufferInfo[chipId].writeOffset = 0;
			m_pDataBufferInfo[chipId].readOffset = 0;
			m_pDataBufferInfo[chipId].dataLen = 0;
		}
		else
		{
			//从当前写入位置一直到缓冲区结尾都为释放状态，只有从缓冲区开始位置到写位置为未释放状态
			m_pDataBufferInfo[chipId].freeLen = m_pDataBufferInfo[chipId].bufferSize-m_pDataBufferInfo[chipId].writeOffset;
		}

		m_pDataBufferInfo[chipId].freeOffset = m_pDataBufferInfo[chipId].bufferSize;
		m_pDataBufferInfo[chipId].tailOffset = m_pDataBufferInfo[chipId].bufferSize;
	}

	m_pDataBufferInfo[chipId].bBufferFull = 0;

	m_pDataBufferInfo[chipId].pLock->UnLock();
}

#define VIEW_PUSH_DATA_FAIL 1

bool CIPDataBufferMan::PushData(long channel, const DVR_IP_IN_FRM_INFO &ipFrameInfo, const unsigned char *pData)
{
	//此内存缓存区为空表示不支持此码流，码流不放入缓冲区，直接返回即可。
	if(m_pDataBufferInfo == NULL)
	{
		return true;
	}

	assert(m_pDataBufferInfo != NULL);

	long dataLen = sizeof(DVR_IP_IN_FRM_INFO) + ipFrameInfo.vir_len;
	assert( 0 == (dataLen % 0x1000) );

	if(COM_FRAME_TYPE_VIDEO_FORMAT == ipFrameInfo.data_type)
	{
		CheckVideoSize(channel, ipFrameInfo.width, ipFrameInfo.height);
	}

	if(!m_pStreamInfo[channel].bSupport)
	{
		printf("channel = %08x,%s,%d\n", channel, __FILE__, __LINE__ );
		return true;
	}

	long chipId = GetDecodeChipId(channel & 0xff);
	if(chipId < 0)
	{
		printf("channel = %08x,%s,%d\n", channel, __FILE__, __LINE__ );
		assert(false);
		return false;
	}

	m_blockSizeLock.Lock();
	unsigned long tempBlockSize = m_blockSize;
	if(dataLen > m_blockSize)
	{
		printf("!**!pushdata!**! CH %d PushData exception----datalen:%d\n", channel, ipFrameInfo.len);
	}
	m_blockSizeLock.UnLock();

	if(ipFrameInfo.data_type == COM_FRAME_TYPE_VIDEO)
	{
		if(ipFrameInfo.keyframe)
		{
			m_pbNeedKeyFrame[channel] = false;
		}

		if(m_pbNeedKeyFrame[channel])
		{
			return true;
		}
	}

	m_pDataBufferInfo[chipId].pLock->Lock();

	if(!m_bAction && (ipFrameInfo.data_type != COM_FRAME_TYPE_VIDEO_FORMAT) && (ipFrameInfo.data_type != COM_FRAME_TYPE_AUDIO_FORMAT))
	{
		m_pbNeedKeyFrame[channel] = true;
		m_pDataBufferInfo[chipId].pLock->UnLock();
		return true;
	}

	//剩余空间不足
	if(m_pDataBufferInfo[chipId].freeLen < dataLen)
	{
#if VIEW_PUSH_DATA_FAIL
		static unsigned long preTime = GetCurrTime32();
		if(GetCurrTime32() > preTime)
		{
			preTime = GetCurrTime32();
			printf("!!71!!!!!!!!!!!!!!!!!!PushData %d stream failed[%d] %d %d, %d, %d, %d, %d, %d, %d...! %s, %d\n", \
				m_streamType, chipId,
				m_pDataBufferInfo[chipId].bufferSize,
				m_pDataBufferInfo[chipId].dataLen,
				m_pDataBufferInfo[chipId].freeLen,
				m_pDataBufferInfo[chipId].readOffset,
				m_pDataBufferInfo[chipId].writeOffset,
				m_pDataBufferInfo[chipId].freeOffset,
				m_pDataBufferInfo[chipId].tailOffset, 
				dataLen,
				__FILE__, __LINE__);
		}
#endif
		m_pDataBufferInfo[chipId].bBufferFull = 1;
		m_pDataBufferInfo[chipId].pLock->UnLock();
		//printf("channel = %08x,%s,%d\n", channel, __FILE__, __LINE__ );
		return false;
	}

	long i = 0;
	for(i = 0; i < (BLOCK_NUM-1); i++)
	{
		if(m_pDataBufferInfo[chipId].dataBlocks[i+1].pData == NULL)
		{
			break;
		}
	}

	if(i == (BLOCK_NUM-1))
	{
		m_pDataBufferInfo[chipId].bBufferFull = 1;
		m_pDataBufferInfo[chipId].pLock->UnLock();
#if VIEW_PUSH_DATA_FAIL
		static unsigned long preTime2 = GetCurrTime32();
		if(1)//GetCurrTime32() > preTime2)
		{
			preTime2 = GetCurrTime32();
			printf("!!2!!!!!!!!!!!!PushData failed[%d] blockNum:%d...! %s, %d\n", chipId, i, __FILE__, __LINE__);
		}
#endif
		printf("channel = %08x,%s,%d\n", channel, __FILE__, __LINE__ );
		return false;
	}
	ONE_DATA_BLOCK &dataBlock = m_pDataBufferInfo[chipId].dataBlocks[i];

	//加入最后一个小于m_blockSize的数据包已超过m_blockSize，或已重新从缓冲区头开始，则加入新的数据包

	if( ((dataLen < tempBlockSize) && ((dataBlock.dataLen + dataLen) > tempBlockSize)) \
		|| ((dataBlock.dataLen > 0) && (m_pDataBufferInfo[chipId].writeOffset == 0)) )
//	if(dataBlock.dataLen > 0)
	{
		ONE_DATA_BLOCK newDataBlock;
		newDataBlock.pData = m_pDataBufferInfo[chipId].pBuffer + m_pDataBufferInfo[chipId].writeOffset;
		newDataBlock.dataLen = 0;
		if(m_pDataBufferInfo[chipId].writeOffset >= m_pDataBufferInfo[chipId].freeOffset)
		{
			//缓冲区尾部空间不够，则跳转到缓冲区头部
			if((m_pDataBufferInfo[chipId].writeOffset + dataLen) > m_pDataBufferInfo[chipId].bufferSize)
			{
				m_pDataBufferInfo[chipId].tailOffset = m_pDataBufferInfo[chipId].writeOffset;
				m_pDataBufferInfo[chipId].writeOffset = 0;
				m_pDataBufferInfo[chipId].freeLen = m_pDataBufferInfo[chipId].freeOffset;

				if(m_pDataBufferInfo[chipId].readOffset == m_pDataBufferInfo[chipId].tailOffset)
				{
					m_pDataBufferInfo[chipId].readOffset = 0;
				}
				if(m_pDataBufferInfo[chipId].freeOffset == m_pDataBufferInfo[chipId].tailOffset)
				{
					m_pDataBufferInfo[chipId].freeOffset = m_pDataBufferInfo[chipId].bufferSize;
					m_pDataBufferInfo[chipId].tailOffset = m_pDataBufferInfo[chipId].bufferSize;
					m_pDataBufferInfo[chipId].freeLen = m_pDataBufferInfo[chipId].bufferSize;
				}

				newDataBlock.pData = m_pDataBufferInfo[chipId].pBuffer;
				if(m_pDataBufferInfo[chipId].freeOffset >= dataLen)
				{
					newDataBlock.dataLen = dataLen;
					memcpy(m_pDataBufferInfo[chipId].pBuffer+m_pDataBufferInfo[chipId].writeOffset, &ipFrameInfo, sizeof(DVR_IP_IN_FRM_INFO));
					memcpy(m_pDataBufferInfo[chipId].pBuffer+m_pDataBufferInfo[chipId].writeOffset+sizeof(DVR_IP_IN_FRM_INFO), pData, ipFrameInfo.len);
					m_pDataBufferInfo[chipId].writeOffset += dataLen;
					m_pDataBufferInfo[chipId].dataLen += dataLen;
					m_pDataBufferInfo[chipId].freeLen -= dataLen;

					m_pDataBufferInfo[chipId].dataBlocks[i+1] = newDataBlock;
				}
				else
				{
					m_pDataBufferInfo[chipId].dataBlocks[i+1] = newDataBlock;
#if VIEW_PUSH_DATA_FAIL
					static unsigned long preTime2 = GetCurrTime32();
					if(GetCurrTime32() > preTime2)
					{
						preTime2 = GetCurrTime32();
						printf("!!3!!!!!!!!!!!!!!!!!!PushData failed[%d] %d, %d, %d, %d, %d, %d, %d...! %s, %d\n", \
							chipId,
							m_pDataBufferInfo[chipId].dataLen,
							m_pDataBufferInfo[chipId].freeLen,
							m_pDataBufferInfo[chipId].readOffset,
							m_pDataBufferInfo[chipId].writeOffset,
							m_pDataBufferInfo[chipId].freeOffset,
							m_pDataBufferInfo[chipId].tailOffset, 
							dataLen,
							__FILE__, __LINE__);
					}
#endif
					m_pDataBufferInfo[chipId].bBufferFull = 1;
					m_pDataBufferInfo[chipId].pLock->UnLock();
					printf("channel = %08x,%s,%d\n", channel, __FILE__, __LINE__ );
					return false;
				}
			}
			else	//缓冲区尾部空间足够
			{
				newDataBlock.dataLen = dataLen;
				memcpy(m_pDataBufferInfo[chipId].pBuffer+m_pDataBufferInfo[chipId].writeOffset, &ipFrameInfo, sizeof(DVR_IP_IN_FRM_INFO));
				memcpy(m_pDataBufferInfo[chipId].pBuffer+m_pDataBufferInfo[chipId].writeOffset+sizeof(DVR_IP_IN_FRM_INFO), pData, ipFrameInfo.len);
				m_pDataBufferInfo[chipId].writeOffset += dataLen;
				m_pDataBufferInfo[chipId].dataLen += dataLen;
				m_pDataBufferInfo[chipId].freeLen -= dataLen;

				if(m_pDataBufferInfo[chipId].writeOffset == m_pDataBufferInfo[chipId].bufferSize)
				{
					m_pDataBufferInfo[chipId].writeOffset = 0;
				}

				m_pDataBufferInfo[chipId].dataBlocks[i+1] = newDataBlock;
			}
		}
		else//writeOffset < freeOffset
		{
			if((m_pDataBufferInfo[chipId].writeOffset+dataLen) > m_pDataBufferInfo[chipId].freeOffset)
			{
				m_pDataBufferInfo[chipId].dataBlocks[i+1] = newDataBlock;
#if VIEW_PUSH_DATA_FAIL
				static unsigned long preTime2 = GetCurrTime32();
				if(GetCurrTime32() > preTime2)
				{
					preTime2 = GetCurrTime32();
					printf("!!4!!!!!!!!!!!!!!!!!!PushData %d stream failed[%d] %d, %d, %d, %d, %d, %d, %d, %d...! %s, %d\n", \
						m_streamType, chipId,
						m_pDataBufferInfo[chipId].bufferSize,
						m_pDataBufferInfo[chipId].dataLen,
						m_pDataBufferInfo[chipId].freeLen,
						m_pDataBufferInfo[chipId].readOffset,
						m_pDataBufferInfo[chipId].writeOffset,
						m_pDataBufferInfo[chipId].freeOffset,
						m_pDataBufferInfo[chipId].tailOffset, 
						dataLen,
						__FILE__, __LINE__);
				}
#endif
				m_pDataBufferInfo[chipId].bBufferFull = 1;
				m_pDataBufferInfo[chipId].pLock->UnLock();
				printf("channel = %08x,%s,%d\n", channel, __FILE__, __LINE__ );
				return false;
			}
			else
			{
				newDataBlock.dataLen = dataLen;
				memcpy(m_pDataBufferInfo[chipId].pBuffer+m_pDataBufferInfo[chipId].writeOffset, &ipFrameInfo, sizeof(DVR_IP_IN_FRM_INFO));
				memcpy(m_pDataBufferInfo[chipId].pBuffer+m_pDataBufferInfo[chipId].writeOffset+sizeof(DVR_IP_IN_FRM_INFO), pData, ipFrameInfo.len);
				m_pDataBufferInfo[chipId].writeOffset += dataLen;
				m_pDataBufferInfo[chipId].dataLen += dataLen;
				m_pDataBufferInfo[chipId].freeLen -= dataLen;

				if(m_pDataBufferInfo[chipId].writeOffset == m_pDataBufferInfo[chipId].bufferSize)
				{
					m_pDataBufferInfo[chipId].writeOffset = 0;
					m_pDataBufferInfo[chipId].freeOffset = 0;
				}

				m_pDataBufferInfo[chipId].dataBlocks[i+1] = newDataBlock;
			}
		}
	}
	else	//最后一个包还没有满
	{
		if(m_pDataBufferInfo[chipId].writeOffset >= m_pDataBufferInfo[chipId].freeOffset)
		{
			//缓冲区尾部空间不够，则跳转到缓冲区头部
			if((m_pDataBufferInfo[chipId].writeOffset + dataLen) > m_pDataBufferInfo[chipId].bufferSize)
			{
				m_pDataBufferInfo[chipId].tailOffset = m_pDataBufferInfo[chipId].writeOffset;
				m_pDataBufferInfo[chipId].writeOffset = 0;
				m_pDataBufferInfo[chipId].freeLen = m_pDataBufferInfo[chipId].freeOffset;

				if(m_pDataBufferInfo[chipId].readOffset == m_pDataBufferInfo[chipId].tailOffset)
				{
					m_pDataBufferInfo[chipId].readOffset = 0;
				}

				if(m_pDataBufferInfo[chipId].freeOffset == m_pDataBufferInfo[chipId].tailOffset)
				{
					m_pDataBufferInfo[chipId].freeOffset = m_pDataBufferInfo[chipId].bufferSize;
					m_pDataBufferInfo[chipId].tailOffset = m_pDataBufferInfo[chipId].bufferSize;
					m_pDataBufferInfo[chipId].freeLen = m_pDataBufferInfo[chipId].bufferSize;
				}

				if(0 == dataBlock.dataLen)
				{
					dataBlock.pData = NULL;
					dataBlock.dataLen = 0;
				}
				else
				{
					i += 1;
				}

				ONE_DATA_BLOCK newDataBlock;
				newDataBlock.dataLen = 0;
				newDataBlock.pData = m_pDataBufferInfo[chipId].pBuffer;
				if(m_pDataBufferInfo[chipId].freeOffset >= dataLen)
				{
					newDataBlock.dataLen = dataLen;
					memcpy(m_pDataBufferInfo[chipId].pBuffer+m_pDataBufferInfo[chipId].writeOffset, &ipFrameInfo, sizeof(DVR_IP_IN_FRM_INFO));
					memcpy(m_pDataBufferInfo[chipId].pBuffer+m_pDataBufferInfo[chipId].writeOffset+sizeof(DVR_IP_IN_FRM_INFO), pData, ipFrameInfo.len);
					m_pDataBufferInfo[chipId].writeOffset += dataLen;
					m_pDataBufferInfo[chipId].dataLen += dataLen;
					m_pDataBufferInfo[chipId].freeLen -= dataLen;

					m_pDataBufferInfo[chipId].dataBlocks[i] = newDataBlock;
				}
				else
				{
					m_pDataBufferInfo[chipId].dataBlocks[i] = newDataBlock;
#if VIEW_PUSH_DATA_FAIL
					static unsigned long preTime2 = GetCurrTime32();
					if(GetCurrTime32() > preTime2)
					{
						preTime2 = GetCurrTime32();
						printf("!!5!!!!!!!!!!!!!!!!!!PushData %d stream failed[%d],%d, %d, %d, %d, %d, %d, %d, %d...! %s, %d\n", \
							m_streamType, chipId,
							m_pDataBufferInfo[chipId].bufferSize,
							m_pDataBufferInfo[chipId].dataLen,
							m_pDataBufferInfo[chipId].freeLen,
							m_pDataBufferInfo[chipId].readOffset,
							m_pDataBufferInfo[chipId].writeOffset,
							m_pDataBufferInfo[chipId].freeOffset,
							m_pDataBufferInfo[chipId].tailOffset, 
							dataLen,
							__FILE__, __LINE__);
					}
#endif
					m_pDataBufferInfo[chipId].bBufferFull = 1;
					m_pDataBufferInfo[chipId].pLock->UnLock();
					printf("channel = %08x,%s,%d\n", channel, __FILE__, __LINE__ );
					return false;
				}
			}
			else	//缓冲区尾部空间足够
			{
				memcpy(m_pDataBufferInfo[chipId].pBuffer+m_pDataBufferInfo[chipId].writeOffset, &ipFrameInfo, sizeof(DVR_IP_IN_FRM_INFO));
				memcpy(m_pDataBufferInfo[chipId].pBuffer+m_pDataBufferInfo[chipId].writeOffset+sizeof(DVR_IP_IN_FRM_INFO), pData, ipFrameInfo.len);
				m_pDataBufferInfo[chipId].writeOffset += dataLen;
				m_pDataBufferInfo[chipId].dataLen += dataLen;
				m_pDataBufferInfo[chipId].freeLen -= dataLen;
				dataBlock.dataLen += dataLen;

				if(m_pDataBufferInfo[chipId].writeOffset == m_pDataBufferInfo[chipId].bufferSize)
				{
					m_pDataBufferInfo[chipId].writeOffset = 0;
				}
			}
		}
		else//writerOffset < freeOffset
		{
			if((m_pDataBufferInfo[chipId].writeOffset+dataLen) > m_pDataBufferInfo[chipId].freeOffset)
			{
#if VIEW_PUSH_DATA_FAIL
				static unsigned long preTime2 = GetCurrTime32();
				if(GetCurrTime32() > preTime2)
				{
					preTime2 = GetCurrTime32();
					printf("!!6!!!!!!!!!!!!!!!!!!PushData %d stream failed[%d], %d, %d, %d, %d, %d, %d, %d, %d...! %s, %d\n", \
						m_streamType, chipId,
						m_pDataBufferInfo[chipId].bufferSize,
						m_pDataBufferInfo[chipId].dataLen,
						m_pDataBufferInfo[chipId].freeLen,
						m_pDataBufferInfo[chipId].readOffset,
						m_pDataBufferInfo[chipId].writeOffset,
						m_pDataBufferInfo[chipId].freeOffset,
						m_pDataBufferInfo[chipId].tailOffset, 
						dataLen,
						__FILE__, __LINE__);
				}
#endif
				m_pDataBufferInfo[chipId].bBufferFull = 1;
				m_pDataBufferInfo[chipId].pLock->UnLock();
				printf("channel = %08x,%s,%d\n", channel, __FILE__, __LINE__ );
				return false;
			}
			else
			{
				memcpy(m_pDataBufferInfo[chipId].pBuffer+m_pDataBufferInfo[chipId].writeOffset, &ipFrameInfo, sizeof(DVR_IP_IN_FRM_INFO));
				memcpy(m_pDataBufferInfo[chipId].pBuffer+m_pDataBufferInfo[chipId].writeOffset+sizeof(DVR_IP_IN_FRM_INFO), pData, ipFrameInfo.len);
				m_pDataBufferInfo[chipId].writeOffset += dataLen;
				m_pDataBufferInfo[chipId].dataLen += dataLen;
				m_pDataBufferInfo[chipId].freeLen -= dataLen;
				dataBlock.dataLen += dataLen;

				if(m_pDataBufferInfo[chipId].writeOffset == m_pDataBufferInfo[chipId].bufferSize)
				{
					m_pDataBufferInfo[chipId].writeOffset = 0;
					m_pDataBufferInfo[chipId].freeOffset = 0;
				}
			}
		}
	}

	m_pDataBufferInfo[chipId].pLock->UnLock();

	//统计接收的码流速度
	//m_averageRateLock.Lock();
	unsigned long curTime = GetCurrTime32();
	if(m_pDataBufferInfo[chipId].curTime == 0)
	{
		m_pDataBufferInfo[chipId].curTime = curTime;
	}
	else if(curTime != m_pDataBufferInfo[chipId].curTime)
	{
//		unsigned long totalRecvLen = 0;
//		for(int i = 0; i < m_decChipNum; i++)
//		{
//			unsigned long chipRecvLen = 0;
//			for(int j = 0; j < 5; j++)
//			{
//				chipRecvLen += m_pDataBufferInfo[i].recvLenOneSecond[j];
//			}
//			totalRecvLen += chipRecvLen;
//		}
//		printf("total recv rate = %d\n", totalRecvLen/5);
		m_pDataBufferInfo[chipId].curTime = curTime;
		memmove(m_pDataBufferInfo[chipId].recvLenOneSecond+1, m_pDataBufferInfo[chipId].recvLenOneSecond, sizeof(unsigned long)*4);
		m_pDataBufferInfo[chipId].recvLenOneSecond[0] = 0;
	}

	m_pDataBufferInfo[chipId].recvLenOneSecond[0] += ipFrameInfo.len;

	//m_averageRateLock.UnLock();

	return true;
}

void CIPDataBufferMan::Action(bool bAction)
{
	for(long chipId = 0; chipId < m_decChipNum; chipId++)
	{
		m_pDataBufferInfo[chipId].pLock->Lock();
	}

	m_bAction = bAction;

	for(long chipId = (m_decChipNum-1); chipId >= 0; chipId--)
	{
		m_pDataBufferInfo[chipId].pLock->UnLock();
	}
}

void CIPDataBufferMan::ClearDataBuffer()
{
	for(long chipId = 0; chipId < m_decChipNum; chipId++)
	{
		m_pDataBufferInfo[chipId].pLock->Lock();
		memset(m_pDataBufferInfo[chipId].dataBlocks, 0, sizeof(ONE_DATA_BLOCK)*BLOCK_NUM);

		m_pDataBufferInfo[chipId].dataLen = 0;
		m_pDataBufferInfo[chipId].writeOffset = m_pDataBufferInfo[chipId].readOffset;
		m_pDataBufferInfo[chipId].tailOffset = m_pDataBufferInfo[chipId].bufferSize;
		m_pDataBufferInfo[chipId].freeLen = m_pDataBufferInfo[chipId].bufferSize;
		m_pDataBufferInfo[chipId].bBufferFull = 0;

		m_pDataBufferInfo[chipId].dataBlocks[0].pData = m_pDataBufferInfo[chipId].pBuffer + m_pDataBufferInfo[chipId].writeOffset;
		m_pDataBufferInfo[chipId].dataBlocks[0].dataLen = 0;

		printf("CIPDataBufferMan::ClearDataBuffer.........\n");

		m_pDataBufferInfo[chipId].pLock->UnLock();
	}
}

unsigned long CIPDataBufferMan::GetAverageRate()
{
	m_averageRateLock.Lock();

	unsigned long totalRecvLen = 0;
	for(int i = 0; i < m_decChipNum; i++)
	{
		unsigned long chipRecvLen = 0;
		for(int j = 1; j < 5; j++)
		{
			chipRecvLen += m_pDataBufferInfo[i].recvLenOneSecond[j];
		}
		totalRecvLen += chipRecvLen;
	}

	m_averageRateLock.UnLock();

	return totalRecvLen/4;
}

bool CIPDataBufferMan::IsBufferFull(long chipId)
{
	m_pDataBufferInfo[chipId].pLock->Lock();
	bool bRet = m_pDataBufferInfo[chipId].bBufferFull;
	m_pDataBufferInfo[chipId].pLock->UnLock();

	return bRet;
}

bool CIPDataBufferMan::PrintBufferInfo(long chipId)
{
	m_pDataBufferInfo[chipId].pLock->Lock();
	printf("Buffer info: %d stream failed[%d] %d %d, %d, %d, %d, %d, %d...! %s, %d\n", \
				m_streamType, chipId,
				m_pDataBufferInfo[chipId].bufferSize,
				m_pDataBufferInfo[chipId].dataLen,
				m_pDataBufferInfo[chipId].freeLen,
				m_pDataBufferInfo[chipId].readOffset,
				m_pDataBufferInfo[chipId].writeOffset,
				m_pDataBufferInfo[chipId].freeOffset,
				m_pDataBufferInfo[chipId].tailOffset, 
				__FILE__, __LINE__);
	m_pDataBufferInfo[chipId].pLock->UnLock();

	return true;
}


long CIPDataBufferMan::GetDecodeChipId(long channel)
{
	assert(NULL != m_pChannelDecChip);
	assert(channel < m_channelNum);
	
	if(channel < m_channelNum)
	{
		return m_pChannelDecChip[channel];
	}
	else
	{
		return -1;
	}
}

unsigned long CIPDataBufferMan::GetCurrTime32()
{
	
#ifdef __ENVIRONMENT_WIN32__
	LONGLONG time = 0;
	SYSTEMTIME systemtime;
	FILETIME  filetime;
	GetSystemTime(&systemtime);
	if (SystemTimeToFileTime(&systemtime, &filetime))
	{
		time = TimeToDVRTime32(filetime);
	}
	
	return time;
#else
	timeval tp;
	if (0 == gettimeofday(&tp, NULL))
	{
		return tp.tv_sec;
	}
	else
	{
		return 0;
	}
	
#endif
}

#ifdef __ENVIRONMENT_WIN32__
unsigned long CIPDataBufferMan::TimeToDVRTime32(FILETIME  DVRtm)   
{
	LONGLONG  AdjustTime = (LONGLONG)(116444736000000000);
	LONGLONG  Temptm = *((LONGLONG *)&DVRtm) ;
	Temptm -= AdjustTime;
	Temptm /= 10;

	return (unsigned long)(Temptm / 1000000);
}
#else
unsigned long CIPDataBufferMan::TimeToDVRTime32(timeval  DVRtm)
{
	LONGLONG  Temptm = (LONGLONG)DVRtm.tv_sec*1000000 + (LONGLONG)DVRtm.tv_usec;

	return (unsigned long)(Temptm / 1000000);
}
#endif

void CIPDataBufferMan::CheckVideoSize(long channel, long width, long height)
{
	if((width == m_pStreamInfo[channel].width) && (height == m_pStreamInfo[channel].height))
	{
		return;
	}

	if(NULL != m_pSupportVideoSize)
	{
		long videoSize = 0;
		if((width == 320) && (height == 240))
		{
			videoSize = DD_VIDEO_SIZE_QVGA;
		}
		else if((width <= 352) && (height <= 288))
		{
			videoSize = DD_VIDEO_SIZE_CIF;
		}
		else if((width <= 720) && (height <= 288))
		{
			videoSize = DD_VIDEO_SIZE_HD1;
		}
		else if((width == 640) && (height == 480))
		{
			videoSize = DD_VIDEO_SIZE_VGA;
		}
		else if((width <= 720) && (height <= 576))
		{
			videoSize = DD_VIDEO_SIZE_D1;
		}
		//else if((width <= 800) && (height <= 600))
		//{
		//	videoSize = DD_VIDEO_SIZE_SVGA;
		//}
		else if((width <= 1280) && (height <= 768))
		{
			videoSize = DD_VIDEO_SIZE_720P;
		}
		//else if((width <= 1024) && (height <= 768))
		//{
		//	videoSize = DD_VIDEO_SIZE_XVGA;
		//}
		else if((width <= 1280) && (height <= 960))
		{
			videoSize = DD_VIDEO_SIZE_960P;
		}
		else if((width <= 1280) && (height <= 1024))
		{
			videoSize = DD_VIDEO_SIZE_SXGA;
		}
		else if((width <= 1920) && (height <= 1088))
		{
			videoSize = DD_VIDEO_SIZE_1080P;
		}
		else if((width <= 2048) && (height <= 1536))
		{
			videoSize = DD_VIDEO_SIZE_3M;
		}
		else if((width <= 2304) && (height <= 1296))
		{
			videoSize = DD_VIDEO_SIZE_16_9_3M;
		}
		else if((width <= 2592) && (height <= 1944))
		{
			videoSize = DD_VIDEO_SIZE_5M;
		}
		else if((width <= 2688) && (height <= 1520))
		{
			videoSize = DD_VIDEO_SIZE_4M;
		}

		if(videoSize & m_pSupportVideoSize[channel])
		{
			m_pStreamInfo[channel].bSupport = true;
		}
		else
		{
			m_pStreamInfo[channel].bSupport = false;
			printf("don't support video size(w:%d, h:%d), videoSize = %x, supportMask = %x\n", width, height, videoSize, m_pSupportVideoSize[channel]);
		}
	}
	else
	{
		m_pStreamInfo[channel].bSupport = false;
		printf("No video size support remark ! %s, %d\n", __FILE__, __LINE__);
	}

	m_pStreamInfo[channel].width = width;
	m_pStreamInfo[channel].height = height;
}

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////

CIPDataBufferMan::CPUB_Lock_IPD::CPUB_Lock_IPD()
{
#ifdef         __ENVIRONMENT_WIN32__
	InitializeCriticalSection(&m_Lock);
#elif defined  __ENVIRONMENT_LINUX__
	pthread_mutexattr_t mutexAttr;
	pthread_mutexattr_init(&mutexAttr);
	pthread_mutexattr_settype(&mutexAttr, PTHREAD_MUTEX_RECURSIVE);
	pthread_mutex_init(&m_Lock, &mutexAttr);
	pthread_mutexattr_destroy(&mutexAttr);
	pthread_cond_init(&m_cond, NULL);
#endif
}

CIPDataBufferMan::CPUB_Lock_IPD::~CPUB_Lock_IPD()
{
#ifdef         __ENVIRONMENT_WIN32__
	DeleteCriticalSection(&m_Lock);
#elif defined  __ENVIRONMENT_LINUX__
	pthread_cond_destroy(&m_cond);
	pthread_mutex_destroy(&m_Lock);
#endif
}

void CIPDataBufferMan::CPUB_Lock_IPD::Lock()
{
#ifdef         __ENVIRONMENT_WIN32__
	EnterCriticalSection(&m_Lock);
#elif defined  __ENVIRONMENT_LINUX__
	pthread_mutex_lock(&m_Lock);
#endif
}

void CIPDataBufferMan::CPUB_Lock_IPD::UnLock()
{
#ifdef         __ENVIRONMENT_WIN32__
	LeaveCriticalSection(&m_Lock);
#elif defined  __ENVIRONMENT_LINUX__
	pthread_mutex_unlock(&m_Lock);
#endif
}

#ifdef	__ENVIRONMENT_LINUX__
void CIPDataBufferMan::CPUB_Lock_IPD::CondWait()
{
	pthread_cond_wait(&m_cond, &m_Lock);
}

void CIPDataBufferMan::CPUB_Lock_IPD::CondSignal()
{
	pthread_cond_signal(&m_cond);
}

//return value: 0 成功 -1 加锁失败
int CIPDataBufferMan::CPUB_Lock_IPD::TryLock()
{
	if(0 == pthread_mutex_trylock(&m_Lock))
	{
		return 0;
	}
	return -1;
}

//return value:	0 成功得到锁 1 超时 2 其他错误
//BUGS:这个在uclibc上好像没有用，或者是自己就写了一个bug
int CIPDataBufferMan::CPUB_Lock_IPD::TimeLock(int iMicrosecond)
{
	struct timeval     timeNow;
	struct timespec    Timeout;
	gettimeofday(&timeNow, NULL);
	Timeout.tv_sec = timeNow.tv_sec;
	Timeout.tv_nsec = (timeNow.tv_usec + iMicrosecond) * 1000;              
	int iRet = pthread_mutex_timedlock(&m_Lock, &Timeout);
	if (0 == iRet) 
	{
		return 0;
	}
	else if (-1 == iRet) 
	{
		if (ETIMEDOUT == errno) 
		{
			return 1;
		}
		else
		{
			assert(false);
			return 2;
		}
	}
	return 0;
}
#endif

//end
