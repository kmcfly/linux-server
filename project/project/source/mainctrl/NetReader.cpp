/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : 袁石维
** Date         : 2007-12-26
** Name         : NetReader.cpp
** Version      : 1.0
** Description  :
** Modify Record:
1:
***********************************************************************/
#include "NetReader.h"

#ifdef __TDFH__
#include "MemPool.h"
#endif

#ifdef	LJ_DEBUG
#include "user_config.h"
#endif

#include "mylist.cpp"

const unsigned long MAX_READ_DATA_FRAME_NUM = NET_PLAYBACK_DATA_FRAME_NUM;
const unsigned long MIN_READ_BUF_NUM = 4;

extern NET_LOGIN_SUCCESS_REPLY * g_p_login_succ;

//public
CNetReader::CNetReader() : m_hRead(0), m_bRead(false)
{
#ifdef WIN32
	m_pBKPath = "E:\\20071226110152";
#endif

	
}

CNetReader::~CNetReader()
{

}

bool CNetReader::Initail(char *pFilePath, CReclogManEX *pReclogManEx, CMessageMan *pMsgMan)
{
	assert(pReclogManEx != NULL);
	assert(pFilePath != NULL);

	m_pReclogMan       = pReclogManEx;
	m_pFilePath        = pFilePath;
	m_pMsgMan          = pMsgMan;
	m_bAction          = true;
	return true;
}


void CNetReader::Quit()
{
	m_pReclogMan       = NULL;
	m_pFilePath        = NULL;
	m_pMsgMan          = NULL;
	m_bAction          = false;
}

bool CNetReader::Start()
{
	return CreateReadThd();
}

void CNetReader::Stop()
{
	CloseReadThd();

	NET_DATA_READRE *pNode = NULL;

	while (m_readerList.GetCount() > 0)
	{
		pNode = m_readerList.RemoveHead();
		
		DestroyStream(pNode);

		delete pNode;
		pNode = NULL;
	}
}

void CNetReader::Stop(unsigned long clientID)
{
	m_readLock.Lock();
	if (!m_bAction)
	{
		m_readLock.UnLock();
		return;
	}

	NET_DATA_READRE *pNode = NULL;

	POS prevPos = NULL;
	POS pos = m_readerList.GetHeadPosition();
	while (NULL != pos)
	{
		prevPos = pos;

		pNode = m_readerList.GetNext(pos);
		if (pNode->clientID == clientID)
		{
			printf("Stop stream client ID = %d, line:%d\n", clientID, __LINE__);
			DestroyStream(pNode);
			m_readerList.RemoveAt(prevPos);
			delete pNode;
			pNode = NULL;
		}
	}

	m_readLock.UnLock();
}

void CNetReader::Action(bool bAction)
{
	assert(m_bAction != bAction);

	m_readLock.Lock();
	if (!bAction)
	{
		printf("Action false  %s  %d\n", __FILE__, __LINE__);
		NET_DATA_READRE *pNode = NULL;

		//发送结束标记
		POS pos = m_readerList.GetHeadPosition();
		while (NULL != pos)
		{
			pNode = m_readerList.GetNext(pos);

			if (pNode->bAction)
			{
				FRAME_INFO_EX info;
				memset(&info, 0, sizeof(FRAME_INFO_EX));
				info.frameType = FRAME_TYPE_END;
				info.channel = pNode->channelForSendEndFrame;
				info.streamID = pNode->streamID;
				info.frameIndex = pNode->sendIndex;
				CFrameData *pData = new CFrameData (info);
				assert (NULL != pData);
				pData->SetDataId(pNode->clientID);
				pData->Add();
				pData->SetProtocol(CMD_REPLY_DATA_STREAM);
				m_pMsgMan->PutPBFrame(pData);
				pData->Dec();
			}		
		}

		while (m_readerList.GetCount() > 0)
		{
			pNode = m_readerList.RemoveHead();
			DestroyStream(pNode);
			delete pNode;
			pNode = NULL;
		}
	}

	m_bAction = bAction;	
	m_readLock.UnLock();
}

/************************************************************************************
*
*dataType：NET_DATA_SEARCH_PLAYBACK或NET_DATA_SEARCH_BACKUP，表示用于回放还是备份
*streamID：流ID，同一种数据可能开多个流
*
*************************************************************************************/
bool CNetReader::StartRead(unsigned long clientID, unsigned long streamID, unsigned long dataType,
						   ULONGLONG videoCH, ULONGLONG audioCH, 
						   unsigned long startTime, unsigned long endTime, 
						   ULONGLONG &outCH, unsigned long channelNum, unsigned long diskOwnerIndex)
{
	assert(channelNum > 0);

	bool bRet = false;
	unsigned char chnn = 0;
    
	m_readLock.Lock();
	if (!m_bAction)
	{
		m_readLock.UnLock();
		return false;
	}

	outCH = 0;

	char *pPath = NULL;
#ifdef WIN32
	pPath = m_pBKPath;
#endif

	printf("Start stream client ID = %d, stream ID = %d, videoCH = %x\n", clientID, streamID, (long)videoCH);
	if (!HasStream(clientID, streamID))
	{
		NET_DATA_READRE *pNode = CreateStream(clientID, streamID, dataType, videoCH, audioCH, \
			startTime, endTime, channelNum, diskOwnerIndex);

		//不管是否成功，都需要回复信息，所以在此都要把节点加入列表。
		//等发完信息再销毁节点。
		pNode->pReader->StartRead(pNode->startTime, pNode->endTime, pNode->videoChannelList, outCH, m_pFilePath);

		pNode->bAction = true;
		pNode->curPlayChannelNum = 0;
		pNode->channelForSendEndFrame = INVALID_INDEX(unsigned long);

		for (chnn = 0; chnn < pNode->channelNum; ++chnn)
		{
			if (outCH & (((ULONGLONG)1) << chnn))
			{
				pNode->curPlayChannelNum++;
				if(INVALID_INDEX(unsigned long) == pNode->channelForSendEndFrame)
				{
					pNode->channelForSendEndFrame = chnn;
				}
			}
		}

		if(INVALID_INDEX(unsigned long) == pNode->channelForSendEndFrame)
		{
			pNode->channelForSendEndFrame = 0;
		}

		m_readerList.AddTail(pNode);
		m_readLock.UnLock();
		bRet = true;
	}
	else
	{
		assert (false);
		m_readLock.UnLock();
		bRet = false;
	}

	return bRet;
}			

int CNetReader::Stop(unsigned long clientID, unsigned long streamID)
{
	int stoptype = -1;
	m_readLock.Lock();
	
	if (!m_bAction)
	{
		m_readLock.UnLock();
		return stoptype;
	}

	NET_DATA_READRE *pNode = NULL;

	POS prevPos = NULL;
	POS pos = m_readerList.GetHeadPosition();
	while (NULL != pos)
	{
		prevPos = pos;

		pNode = m_readerList.GetNext(pos);
		if ((pNode->clientID == clientID) && (pNode->streamID == streamID))
		{
			printf("Stop stream client ID = %d, stream ID:%d, line:%d\n", clientID, streamID, __LINE__);
			DestroyStream(pNode);
			m_readerList.RemoveAt(prevPos);
			stoptype = pNode->dataType;
			delete pNode;
			pNode = NULL;
			break;
		}
	}

	m_readLock.UnLock();
	return stoptype;
}

bool CNetReader::Play(unsigned long clientID, unsigned long streamID)
{
	m_readLock.Lock();
	if (!m_bAction)
	{
		m_readLock.UnLock();
		return false;
	}

	NET_DATA_READRE *pNode = NULL;

	POS pos = m_readerList.GetHeadPosition();
	while (NULL != pos)
	{
		pNode = m_readerList.GetNext(pos);
		if ((pNode->clientID == clientID) && (pNode->streamID == streamID))
		{
			assert (DATA_READING != pNode->status);

			pNode->status = DATA_READING;
		
			m_readLock.UnLock();
			return true;
		}
	}

	m_readLock.UnLock();
	return false;
}

void CNetReader::Pause(unsigned long clientID, unsigned long streamID, ULONGLONG videoCH)
{
	m_readLock.Lock();
	if (!m_bAction)
	{
		m_readLock.UnLock();
		return;
	}

	NET_DATA_READRE *pNode = NULL;

	POS pos = m_readerList.GetHeadPosition();
	while (NULL != pos)
	{
		pNode = m_readerList.GetNext(pos);
		if ((pNode->clientID == clientID) && (pNode->streamID == streamID))
		{
			pNode->status = DATA_PAUSE;
			break;
		}
	}
	
	m_readLock.UnLock();
}

bool CNetReader::FF(unsigned long clientID, unsigned long streamID, ULONGLONG videoCH, unsigned long speed)
{
	m_readLock.Lock();
	if (!m_bAction)
	{
		m_readLock.UnLock();
		return false;
	}

	NET_DATA_READRE *pNode = NULL;

	POS pos = m_readerList.GetHeadPosition();
	while (NULL != pos)
	{
		pNode = m_readerList.GetNext(pos);
		if ((pNode->clientID == clientID) && (pNode->streamID == streamID))
		{
			pNode->status = DATA_FF;
			pNode->speed = speed;
            assert(pNode->speed <= NET_PLAY_SPEED_16X);
			assert(pNode->speed > NET_PLAY_SPEED_4X);
			
			pNode->kfOffSetNum = 1;
			unsigned long num = pNode->speed - NET_PLAY_SPEED_8X;
			for (unsigned long i = 0; i < num; ++i)
			{
				pNode->kfOffSetNum *= 2;
			}
			

			break;
		}  
	}
	
	m_readLock.UnLock();

	return true;
}

bool CNetReader::REW(unsigned long clientID, unsigned long streamID, ULONGLONG videoCH, unsigned long speed)
{  
	m_readLock.Lock();
	if (!m_bAction)
	{
		m_readLock.UnLock();
		return false;
	}

	NET_DATA_READRE *pNode = NULL;

	POS pos = m_readerList.GetHeadPosition();
	while (NULL != pos)
	{
		pNode = m_readerList.GetNext(pos);
		if ((pNode->clientID == clientID) && (pNode->streamID == streamID))
		{
			pNode->status = DATA_REW;
			pNode->speed = speed;
			assert(pNode->speed >= NET_PLAY_SPEED_8X);
			pNode->kfOffSetNum = 1;
			unsigned long num = pNode->speed - NET_PLAY_SPEED_8X;
			for (unsigned long i = 0; i < num; ++i)
			{
				pNode->kfOffSetNum *= 2;
			}
			break;
		}
	}
	
	m_readLock.UnLock();

	return true;
}

void CNetReader::ChangeAudio(unsigned long clientID, unsigned long streamID, ULONGLONG audioCH)
{
	m_readLock.Lock();
	if (!m_bAction)
	{
		m_readLock.UnLock();
		return;
	}

	NET_DATA_READRE *pNode = NULL;

	POS pos = m_readerList.GetHeadPosition();
	while (NULL != pos)
	{
		pNode = m_readerList.GetNext(pos);
		if ((pNode->clientID == clientID) && (pNode->streamID == streamID))
		{
			pNode->audioCH = audioCH;
			break;
		}
	}

	m_readLock.UnLock();
}
/*
bool CNetReader::ChangeCH(unsigned long clientID, unsigned long streamID, ULONGLONG videoCH, ULONGLONG audioCH, ULONGLONG & outCH)
{
	m_readLock.Lock();

	outCH = 0;

	char *pPath = NULL;
#ifdef WIN32
	pPath = m_pBKPath;
#endif

	NET_DATA_READRE *pNode = NULL;
	POS prevPos = NULL;
	POS pos = m_readerList.GetHeadPosition();
	while (NULL != pos)
	{
		prevPos = pos;

		pNode = m_readerList.GetNext(pos);
		if ((pNode->clientID == clientID) && (pNode->streamID == streamID))
		{
			pNode->pReader->StopRead();

			if (!pNode->pReader->StartRead()videoCH, audioCH, pNode->startTime, pNode->endTime, outCH, pPath))
			{
				m_pNetServer->StopStream(pNode->streamID, pNode->clientID);

				DestroyStream(pNode);
				m_readerList.RemoveAt(prevPos);
				delete pNode;
				pNode = NULL;

				m_readLock.UnLock();
				return false;
			}
			else
			{
				pNode->videoCH = videoCH;
				pNode->audioCH = audioCH;
				
				if (0 != m_pNetServer->ChangeStream(outCH, 0, pNode->audioCH, pNode->streamID, pNode->clientID))
				{
					DestroyStream(pNode);
					m_readerList.RemoveAt(prevPos);
					delete pNode;
					pNode = NULL;

					m_readLock.UnLock();
					return false;
				}

				m_readLock.UnLock();
				return true;
			}
		}
	}
	
	m_readLock.UnLock();
	return false;
}

bool CNetReader::ChangeTime(unsigned long clientID, unsigned long streamID, LONGLONG startTime, LONGLONG endTime, ULONGLONG & outCH)
{
	m_readLock.Lock();

	outCH = 0;

	char *pPath = NULL;
#ifdef WIN32
	pPath = m_pBKPath;
#endif

	NET_DATA_READRE *pNode = NULL;
	POS prevPos = NULL;
	POS pos = m_readerList.GetHeadPosition();
	while (NULL != pos)
	{
		prevPos = pos;

		pNode = m_readerList.GetNext(pos);
		if ((pNode->clientID == clientID) && (pNode->streamID == streamID))
		{
			pNode->pReader->StopRead();

			if (!pNode->pReader->StartRead(pNode->videoCH, pNode->audioCH, startTime, endTime, outCH, pPath))
			{
				DestroyStream(pNode);
				m_readerList.RemoveAt(prevPos);
				delete pNode;
				pNode = NULL;

				m_readLock.UnLock();
				return false;
			}
			else
			{
				pNode->startTime = startTime;
				pNode->endTime = endTime;

				m_readLock.UnLock();
				return true;
			}
		}
	}
	
	m_readLock.UnLock();
	return false;
}
*/
void CNetReader::RelayIndex(unsigned long clientID, unsigned long streamID, unsigned long index)
{
	m_readLock.Lock();
	if (!m_bAction)
	{
		m_readLock.UnLock();
		return;
	}

	NET_DATA_READRE *pNode = NULL;

	POS pos = m_readerList.GetHeadPosition();
	while (NULL != pos)
	{
		pNode = m_readerList.GetNext(pos);
		if ((pNode->clientID == clientID) && (pNode->streamID == streamID))
		{
			pNode->receiveIndex = index;
			break;
		}
	}
	
	m_readLock.UnLock();
}
//protected

//private
RESULT WINAPI CNetReader::ReadThread(LPVOID lpParam)
{
#ifdef __ENVIRONMENT_LINUX__
	pid_t pid = getpid();
	pid_t tid = syscall(__NR_gettid);
	printf("%s, %d, pid = %d, tid=%d\n", __FILE__, __LINE__, pid, tid);
#endif

	assert(NULL != lpParam);
	CNetReader *pThis = static_cast <CNetReader *> (lpParam);

	pThis->ReadProc();

	return 0;
}

bool CNetReader::CreateReadThd()
{
	assert(0 == m_hRead); //只能建立一次线程

	m_hRead = PUB_CreateThread(ReadThread, (void *)this, &m_bRead);	
	if( PUB_CREATE_THREAD_FAIL == m_hRead )
	{
		PUB_PrintError(__FILE__,__LINE__);
		return false;
	}

	return  true;
}

void CNetReader::CloseReadThd()
{
	PUB_ExitThread(&m_hRead, &m_bRead);
}

void CNetReader::ReadProc()
{
	POS prevPos = NULL;
	POS pos = NULL;
	NET_DATA_READRE *pNode = NULL;
	unsigned long index = INVALID_INDEX(unsigned long);
	FRAME_INFO_EX frameInfo;
	memset (&frameInfo, 0, sizeof (FRAME_INFO_EX));
	bool bHasClient = false;
	bool bAction = false;
	bool bSucc = true;
	bool bFormat = false;
	long ret = 0;
	bool bPlayEnd = false;

	int  nReadSize = MAX_READ_DATA_FRAME_NUM;

	while (m_bRead)
	{
		m_readLock.Lock();
		if (!m_bAction)
		{
			m_readLock.UnLock();
			PUB_Sleep(20);
			continue;
		}

		bHasClient  = false;
		bAction     = false;

		pos = m_readerList.GetHeadPosition();
		while (NULL != pos)
		{
			bHasClient = true;
			prevPos = pos;
			bSucc = true;

			pNode = m_readerList.GetNext(pos);
			if (pNode->bAction && (DATA_PAUSE != pNode->status))
			{
				//正常回放采取的方法：
				//如果发送出去的且未被处理的帧累积到一定程度，则暂停发送。
				//
				//快进或快退采取方法是：
				//服务器端：
				//服务器发送一帧数据给客户端，然后，就等待该帧的回复，
				//如果收到回复信息，就继续发下一帧；
				//如果没有收到回复，就继续等待。
				//客户端：
				//客户端收到帧之后，就马上回复。
				if ((DATA_REW == pNode->status) || (DATA_FF == pNode->status))
				{
					nReadSize = 1;
				}
				else
				{
					nReadSize = MAX_READ_DATA_FRAME_NUM * pNode->curPlayChannelNum;
				}
				// printf("sendIndex:%d,receiveIndex:%d,nReadSize:%d\n", pNode->sendIndex,pNode->receiveIndex,nReadSize);

				if (pNode->sendIndex <= (pNode->receiveIndex + nReadSize) )
				{
					index = GetBufIndex(pNode);
					if (INVALID_INDEX(unsigned long) != index)
					{
						bAction = true;						
						frameInfo.bufIndex = index;
						frameInfo.pData = pNode->pDataBuf[index].pBuf;
						pNode->pDataBuf [index].bUsed = true;
						frameInfo.streamID = pNode->streamID;
						bPlayEnd = false;

						if ((DATA_READING == pNode->status) || (DATA_FF == pNode->status))
						{
							ret = pNode->pReader->GetNextFrameType(frameInfo, true);
						}
						else if (DATA_REW == pNode->status)
						{
							ret = pNode->pReader->GetNextFrameType(frameInfo, false);
						}
						else
						{
							assert(false);
						}

						if(MR_RET_OK != ret)
						{
							printf("GetNextFrame failed, playback end\n");
							bPlayEnd = true;
							bSucc = false;
						}

						if (bSucc)
						{
							if(frameInfo.frameType == FRAME_TYPE_VIDEO_FORMAT)
							{
								ret = pNode->pReader->GetNextFrame(frameInfo, !(DATA_REW == pNode->status));
							}
							else if(frameInfo.frameType == FRAME_TYPE_AUDIO_FORMAT)
							{
								ret = pNode->pReader->GetNextFrame(frameInfo, !(DATA_REW == pNode->status));
							}
							else
							{
								if (DATA_READING == pNode->status)
								{
									ret = pNode->pReader->GetNextFrame(frameInfo);
								}
								else if (DATA_FF == pNode->status)
								{
									assert(pNode->speed >= NET_PLAY_SPEED_8X);
									ret = pNode->pReader->GetKeyFrame(frameInfo, pNode->kfOffSetNum, true);
								}
								else if (DATA_REW == pNode->status)
								{
									assert(pNode->speed >= NET_PLAY_SPEED_8X);
									ret = pNode->pReader->GetKeyFrame(frameInfo, pNode->kfOffSetNum, false);
								}
								else
								{
									assert(false);
								}
							}

							if ((FRAME_TYPE_VIDEO_FORMAT == frameInfo.frameType) || (FRAME_TYPE_AUDIO_FORMAT == frameInfo.frameType))
							{
								bFormat = true;
							}
							else
							{
								bFormat = false;
							}

							if ((MR_RET_OK == ret) && (bFormat || (!bFormat && ((frameInfo.time/1000000) <= pNode->endTime))))
							{
								if((FRAME_TYPE_AUDIO == frameInfo.frameType) && \
									!(((ULONGLONG)0x01 << frameInfo.channel) & pNode->audioCH))
								{
									ReleaseBuf(pNode, index);
									continue;
								}

								CFrameData *pData = new CFrameData (frameInfo, 1);
								assert(NULL != pData);
								pData->SetProtocol(CMD_REPLY_DATA_STREAM);
								pData->SetDataId(pNode->clientID);
								pData->Add();
								if ((FRAME_TYPE_VIDEO_FORMAT == frameInfo.frameType)
									||(FRAME_TYPE_VIDEO == frameInfo.frameType) \
									|| (FRAME_TYPE_AUDIO_FORMAT == frameInfo.frameType) \
									|| (FRAME_TYPE_AUDIO == frameInfo.frameType))
								{
									pData->GetFrameInfo().frameIndex = pNode->sendIndex;
									++pNode->sendIndex;
								}

								m_pMsgMan->PutPBFrame(pData);

								if (0 < pData->Dec())
								{
									pNode->pDataList->AddTail(pData);
								}
								else
								{
									ReleaseBuf(pNode, index);
									delete pData;
									pData = NULL;
								}
							}
							else
							{
								printf("playback end: ret = %d, bFormat = %d, frameTime = %d, endTime = %d\n", \
									ret, bFormat, (long)(frameInfo.time/1000000), pNode->endTime);
								bPlayEnd = true;
							}
						}

						if (bPlayEnd)
						{
							//发送结束标记
							FRAME_INFO_EX info;
							memset(&info, 0, sizeof(FRAME_INFO_EX));
							info.frameType = FRAME_TYPE_END;
							info.channel = pNode->channelForSendEndFrame;
							info.streamID = pNode->streamID;
							info.frameIndex = pNode->sendIndex;
							CFrameData *pData = new CFrameData (info);
							assert (NULL != pData);
							pData->SetDataId(pNode->clientID);
							pData->Add();
							pData->SetProtocol(CMD_REPLY_DATA_STREAM);
							m_pMsgMan->PutPBFrame(pData);
							pData->Dec();

							//释放buf，并停止该客户端的读取
							ReleaseBuf(pNode, index);

							pNode->pReader->StopRead();
							pNode->bAction = false;
						}
					}

				}
			}

			if (pNode->pDataList->GetCount() > 0)
			{
				CFrameData *pHead = pNode->pDataList->GetHead();
				while ((pHead != NULL) && (pHead->GetStatus() == 0))
				{
					ReleaseBuf(pNode, pHead->GetFrameInfo().bufIndex);
					pNode->pDataList->RemoveHead();
					delete pHead;
					pHead = NULL;

					if (pNode->pDataList->GetCount() > 0)
					{
						pHead = pNode->pDataList->GetHead();
					}
				}
			}

			if (pNode->pDataList->GetCount() == 0)	//链表数据都已经发送完成
			{
				if (!pNode->bAction)	//非活动状态
				{
					DestroyStream(pNode);
					m_readerList.RemoveAt(prevPos);
					delete pNode;
					pNode = NULL;
				}
			}

		}
		
		POS pos = m_tempFrameList.GetHeadPosition();
		while(pos)
		{
			CFrameData *pData = m_tempFrameList.GetAt(pos);
			if(pData->GetStatus() == 0)
			{
				if(pData->GetFrameInfo().pData != NULL)
				{
#ifdef __TDFH__
					CMemPool::Instance()->ReleaseMem(pData->GetFrameInfo().pData);
#else
					delete [] pData->GetFrameInfo().pData;
#endif
					
					pData->GetFrameInfo().pData = NULL;
				}
				delete pData;
				POS tempPos = pos;
				m_tempFrameList.GetNext(pos);
				m_tempFrameList.RemoveAt(tempPos);
				continue;
			}
			m_tempFrameList.GetNext(pos);
		}

		m_readLock.UnLock();

		//no client or none can read, want to sleep
		if(!bHasClient)
		{
#ifdef __TD2316SES_SPEC__
			PUB_Sleep(1000);
#else
			PUB_Sleep(20);
#endif
		}

		if (!bAction)
		{
			PUB_Sleep(0);
		}
	}
}


bool CNetReader::HasStream(unsigned long clientID, unsigned long streamID)
{
	NET_DATA_READRE *pNode = NULL;
	POS pos = m_readerList.GetHeadPosition();
	while (NULL != pos)
	{
		pNode = m_readerList.GetNext(pos);
		if ((pNode->clientID == clientID) && (pNode->streamID == streamID))
		{
			return true;
		}
	}

	return false;
}

NET_DATA_READRE * CNetReader::CreateStream(unsigned long clientID, unsigned long streamID, unsigned long dataType, \
										   ULONGLONG videoCH, ULONGLONG audioCH, unsigned long startTime, \
										   unsigned long endTime, unsigned long channelNum, unsigned long diskOwnerIndex)
{
	printf("Create stream client ID:%d stream ID:%d line:%d\n", clientID, streamID, __LINE__);
	NET_DATA_READRE * pNode = new NET_DATA_READRE;
	assert(NULL != pNode);
	memset (pNode, 0, sizeof (NET_DATA_READRE));

	pNode->channelNum = channelNum;
	pNode->diskOwnerIndex = diskOwnerIndex;

	pNode->pReader	= new CMultiReader;
	assert (NULL != pNode->pReader);
	pNode->pReader->Initial(pNode->channelNum, m_pReclogMan, pNode->diskOwnerIndex);
	pNode->pDataList = new CMyList <CFrameData *> ;
	assert (NULL != pNode->pDataList);
	
	pNode->clientID		= clientID;
	pNode->streamID		= streamID;
	pNode->dataType		= dataType;

	pNode->status		= DATA_READING;
	pNode->bAction		= false;
	pNode->speed        = 0;

    pNode->sendIndex    = 0;
	pNode->receiveIndex = 0;
	pNode->startTime    = startTime;
	pNode->endTime      = endTime;	
	pNode->videoCH      = videoCH;
	pNode->audioCH      = audioCH;

	if (pNode->videoChannelList.GetCount() > 0)
	{
		pNode->videoChannelList.RemoveAll();
	}

	unsigned long succCHNum = 0;
	for (unsigned char chnn = 0; chnn < pNode->channelNum; ++chnn)
	{
		if (videoCH & (ULONGLONG)0x01 << chnn)
		{
			pNode->videoChannelList.AddTail(chnn);
			++ succCHNum;
		}
	}

	assert ((0 < succCHNum) && (succCHNum <= pNode->channelNum));
	//每次实际播放的通道申请一块缓冲区。
#if defined(__CHIP3515__) || defined(__CHIPTI__) || defined (__CHIP3521__)|| defined (__CHIP3520A__) || defined (__CHIP3520D__)
	PRODUCT_TYPE productID = CProduct::Instance()->ProductType();
	if ((TD_2704XS_SL == productID) || (TD_2704AS_SL == productID) || (TD_2704TS_S == productID) || (TD_2704AS_S == productID) || (TD_2708AS_S == productID) || (TD_2008AS == productID)
	|| (TD_2704AS_S_A == productID) || (TD_2708AS_S_A == productID)
	|| (TD_2704AS_SL_A == productID)
	|| (TD_2704AS_PL == productID)
	)
	{
		pNode->bufNum		= succCHNum + 4;
	}
	else
	{
		pNode->bufNum		= succCHNum + 2;
	}
#else

	PRODUCT_TYPE productID = CProduct::Instance()->ProductType();
	if (TD_2808NE_A == productID)
	{
		pNode->bufNum		= (MIN_READ_BUF_NUM < succCHNum) ? succCHNum : 4;
	}
	else if (TD_2708TS_PL == productID || TD_2716TS_S == productID || TD_2716TS_PL == productID || TD_2716TD_PL == productID)
	{
		pNode->bufNum		= (MIN_READ_BUF_NUM < succCHNum) ? succCHNum : succCHNum + 2;
	}
	else if ((0 < g_p_login_succ->productInfo.netVideoInputNum) && (TD_2508HE_C_3521 == productID))
	{
		pNode->bufNum		= (4 < succCHNum) ? succCHNum : succCHNum + 2;
	}
	else
	{
		pNode->bufNum		= (MIN_READ_BUF_NUM < succCHNum) ? succCHNum : MIN_READ_BUF_NUM;
	}
#endif
	pNode->pDataBuf     = new NET_DATA_BUF [pNode->bufNum];
	for (unsigned char i = 0; i < pNode->bufNum; ++i)
	{
#ifdef __TDFH__
		unsigned long getMemCount = 0;
		do 
		{
			pNode->pDataBuf[i].pBuf =  (BYTE *) CMemPool::Instance()->GetMem(MEM_NET_PB, MAX_VIDEO_FRAME_LEN);
		
			if (NULL == pNode->pDataBuf[i].pBuf)
			{
				PUB_Sleep(10);
			}
			getMemCount++;
			if (20000 < getMemCount)
			{
				break;
			}
		} while (NULL == pNode->pDataBuf[i].pBuf);
#else
		pNode->pDataBuf[i].pBuf = new BYTE [MAX_VIDEO_FRAME_LEN];
#endif
		if (NULL == pNode->pDataBuf[i].pBuf)
		{
			printf("%s:%s:%d, new error\n", __FUNCTION__, __FILE__, __LINE__);
		}
		else
		{
			memset(pNode->pDataBuf[i].pBuf, 0, MAX_VIDEO_FRAME_LEN);
		}
		assert(NULL != pNode->pDataBuf[i].pBuf);
		pNode->pDataBuf[i].bufLen = MAX_VIDEO_FRAME_LEN;
		pNode->pDataBuf[i].bUsed = false;
	}

	pNode->sendVideoTime = 0;	
	return pNode;
}

void CNetReader::DestroyStream(NET_DATA_READRE *pNode)
{
	assert (NULL != pNode);
	printf("Destroy stream  client ID:%d, stream ID:%d line:%d\n", pNode->clientID, pNode->streamID, __LINE__);

	assert (NULL != pNode->pReader);
	if (pNode->bAction)
	{
		pNode->pReader->StopRead();
		pNode->bAction = false;
	}

	pNode->pReader->Quit();
	delete pNode->pReader;
	pNode->pReader = NULL;	

	//需要把已经送入NetServer的数据清空
	CFrameData *pData = NULL;
	while (pNode->pDataList->GetCount() > 0)
	{
		pData = pNode->pDataList->RemoveHead();
		ReleaseBuf(pNode, pData->GetFrameInfo().bufIndex);
#if defined(__TDFH__) || defined(__CHIP3515__) || defined(__CHIP3520__) || defined(__CHIP3520A__) || defined(__CHIP3520D__) || defined(__CHIP3521__) || defined(__CHIP3531__) || defined(__CHIP3535__) || defined(__CHIPGM__) 
		unsigned long waitCount = 0;
		while (pData->GetStatus() != 0)
		{
			waitCount++;
			if (waitCount > 20000)
			{
				break;
			}
			PUB_Sleep(50);
		}
#endif
		if(pData->GetStatus() != 0)	//把送入Netserver但还没有发送出去的数据放入m_tempFrameList列表中
		{
			assert(pData->GetStatus() > 0);
			m_tempFrameList.AddTail(pData);
		}
		else
		{
			delete pData;
			pData = NULL;
		}
	}
	delete pNode->pDataList;
	pNode->pDataList = NULL;

	for (unsigned char i = 0; i< pNode->bufNum; ++i)
	{
		assert (NULL != pNode->pDataBuf[i].pBuf);
		
		//把暂存在m_tempFrameList列表中的帧所对应的buffer保留
		POS pos = m_tempFrameList.GetHeadPosition();
		while(pos)
		{
			CFrameData *&pData = m_tempFrameList.GetAt(pos);
			if(pData->GetFrameInfo().pData == pNode->pDataBuf[i].pBuf)
			{
				break;
			}

			m_tempFrameList.GetNext(pos);
		}

		if(pos != NULL)
		{
			continue;
		}
#ifdef __TDFH__
		CMemPool::Instance()->ReleaseMem(pNode->pDataBuf[i].pBuf);
#else
		delete [] pNode->pDataBuf[i].pBuf;
#endif
		

		pNode->pDataBuf[i].pBuf = NULL;

		pNode->pDataBuf[i].bufLen = 0;
		pNode->pDataBuf[i].bUsed = false;
	}
	
	if (pNode->pDataBuf != NULL)
	{
		delete [] pNode->pDataBuf;
		pNode->pDataBuf = NULL;
		pNode->bufNum = 0;
	}
	
	if (pNode->videoChannelList.GetCount() > 0)
	{
		pNode->videoChannelList.RemoveAll();
	}

}

unsigned long CNetReader::GetBufIndex(NET_DATA_READRE *pNode)
{
	unsigned long ret = INVALID_INDEX(unsigned long);
	
	for (unsigned char i=0; i< pNode->bufNum; ++i)
	{
		if (!pNode->pDataBuf[i].bUsed)
		{
			ret = i;
			break;
		}
	}
	
	return ret ;
}

void CNetReader::ReleaseBuf (NET_DATA_READRE *pNode, unsigned long index)
{
	assert (index < pNode->bufNum);
	assert (pNode->pDataBuf [index].bUsed);
	
	pNode->pDataBuf [index].bUsed = false;
}

unsigned long CNetReader::GetStreamType(unsigned long clientID, unsigned long streamID)
{
	NET_DATA_READRE *pNode = NULL;
	POS pos = m_readerList.GetHeadPosition();
	while (NULL != pos)
	{
		pNode = m_readerList.GetNext(pos);
		if ((pNode->clientID == clientID) && (pNode->streamID == streamID))
		{
			return pNode->dataType;
		}
	}

	return INVALID_INDEX(unsigned long);
}
//end
