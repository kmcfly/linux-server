#include "HxMultiReader.h"

CHxMultiReader::CHxMultiReader(void)
{
	m_pFilePath = NULL;
}

CHxMultiReader::~CHxMultiReader(void)
{
}

bool CHxMultiReader::Initial(CReclogManEX *pRecLogManEx, const char *pFilePath)
{
	m_pRecLogMan = pRecLogManEx;
	m_readerIndex = 0;
	
	if(pFilePath != NULL)
	{
		long pathLen = strlen(pFilePath)+1;
		m_pFilePath = new char[pathLen];
		strcpy(m_pFilePath, pFilePath);
	}

	return true;
}

void CHxMultiReader::Quit()
{
	if(m_pFilePath != NULL)
	{
		delete [] m_pFilePath;
	}
}

bool CHxMultiReader::Start(unsigned long streamID, unsigned long startTime, unsigned long stopTime, long channel, long diskOwner)
{
	m_lock.Lock();

	HX_READER reader;
	memset(&reader, 0, sizeof(HX_READER));
	reader.pBuffer = new CHxCycleBuffer;
	reader.pReader = new COneChnnReader;
	reader.streamID = streamID;
	reader.chnnNum = channel;

	if(!reader.pBuffer->Initial(1024 * 1024))
	{
		delete reader.pBuffer;
		delete reader.pReader;

		m_lock.UnLock();
		return false;
	}

	if(!reader.pReader->Initial(m_pRecLogMan, diskOwner))
	{
		reader.pBuffer->Quit();
		delete reader.pBuffer;
		delete reader.pReader;

		m_lock.UnLock();
		return false;
	}

	if(!reader.pReader->StartRead(startTime, stopTime, channel, m_pFilePath))
	{
		reader.pReader->Quit();
		reader.pBuffer->Quit();
		delete reader.pBuffer;
		delete reader.pReader;

		m_lock.UnLock();
		return false;
	}
	
	reader.startTime = startTime;
	reader.stopTime = stopTime;
	
	m_readers.push_back(reader);

	m_lock.UnLock();
	return true;
}

void CHxMultiReader::Stop(unsigned long streamID)
{
	m_lock.Lock();

	for(std::vector<HX_READER>::iterator it = m_readers.begin(); it != m_readers.end(); it++)
	{
		if(it->streamID == streamID)
		{
			//缓冲区需全部释放后才能执行删除动作
			if(it->pBuffer->Quit())
			{
				it->pReader->StopRead();
				it->pReader->Quit();
				delete it->pBuffer;
				delete it->pReader;
				m_readers.erase(it);
			}
			break;
		}
	}

	if(m_readerIndex >= m_readers.size())
	{
		m_readerIndex = 0;
	}

	m_lock.UnLock();
}


long CHxMultiReader::GetNextFrame(FRAME_INFO_EX &frameInfo)
{
	long readRet = HX_READ_ERR_NO_DATA;
	m_lock.Lock();

	if(m_readers.empty())
	{
		m_lock.UnLock();
		return HX_READ_ERR_NO_DATA;
	}

	long readerCount = m_readers.size();
	for(int i = 0; i < readerCount; i++)
	{
		//该流的数据已经播放完毕
		if(m_readers[m_readerIndex].playEnd)
		{
			m_readerIndex += 1;
			m_readerIndex %= readerCount;
			continue;
		}

		if(m_readers[m_readerIndex].pReader->GetFrameTime()/1000000 > m_readers[m_readerIndex].stopTime)
		{
			
			//发送到最后一帧，要手动添加一个结束帧
			frameInfo.pData = NULL;
			frameInfo.frameType = FRAME_TYPE_END;
			frameInfo.channel = m_readers[m_readerIndex].chnnNum;
			frameInfo.streamID = m_readers[m_readerIndex].streamID;
			frameInfo.length = 0;
			frameInfo.width = 0;
			frameInfo.height = 0;
			
			m_readers[m_readerIndex].playEnd = true;
			m_readerIndex += 1;
			m_readerIndex %= readerCount;

			m_lock.UnLock();

			return HX_READ_SUCCESS;
		}

		

		switch(m_readers[m_readerIndex].pReader->GetFrameType())
		{
		case FRAME_TYPE_VIDEO_FORMAT:
			{
				frameInfo.length = sizeof(BITMAPINFOHEADER);
				break;
			}
		case FRAME_TYPE_AUDIO_FORMAT:
			{	
				frameInfo.length = sizeof(WAVEFORMATEX);
				break;
			}
		case FRAME_TYPE_END:
			{
				printf("%s:%s:%d, the end of the frame\n", __FUNCTION__, __FILE__, __LINE__);
			}
		default:
			frameInfo.length = m_readers[m_readerIndex].pReader->GetFrameLen();
			break;
		}

		frameInfo.pData = m_readers[m_readerIndex].pBuffer->GetBuffer(frameInfo.length);

		if(NULL == frameInfo.pData)
		{
			m_readerIndex += 1;
			m_readerIndex %= readerCount;
			readRet = HX_READ_ERR_NO_BUFFER;

			break;
		}
		else
		{
			if(ONE_CHNN_READ_SUC != m_readers[m_readerIndex].pReader->GetFrame(frameInfo))
			{
				m_readerIndex += 1;
				m_readerIndex %= readerCount;
				frameInfo.frameType = FRAME_TYPE_NONE;
				m_readers[m_readerIndex].playEnd = true;

				m_lock.UnLock();
				return HX_READ_SUCCESS;
			}
			else
			{
				if(SEEK_FRAME_OK != m_readers[m_readerIndex].pReader->SeekFrame(SEEK_NEXT_VFRM))
				{
					m_readers[m_readerIndex].playEnd = true;
				}

				frameInfo.streamID = m_readers[m_readerIndex].streamID;
				m_readerIndex++;
				m_readerIndex %= readerCount;

				m_lock.UnLock();
				return HX_READ_SUCCESS;
			}
		}
	}

	m_lock.UnLock();
	return readRet;
}

void CHxMultiReader::ReleaseFrame(const FRAME_INFO_EX &frameInfo)
{
	m_lock.Lock();

	for(std::vector<HX_READER>::iterator it = m_readers.begin(); it != m_readers.end(); it++)
	{
		if(it->streamID == frameInfo.streamID)
		{
			it->pBuffer->ReleaseBuffer(frameInfo.pData, frameInfo.length);
			
			//缓冲区需全部释放后才能执行删除动作
			if(it->pBuffer->WaitForQuit() && it->pBuffer->Quit())
			{
				it->pReader->StopRead();
				it->pReader->Quit();
				delete it->pBuffer;
				delete it->pReader;
				m_readers.erase(it);
				if(m_readerIndex >= m_readers.size())
				{
					m_readerIndex = 0;
				}
			}
			break;
		}
	}

	m_lock.UnLock();
}
