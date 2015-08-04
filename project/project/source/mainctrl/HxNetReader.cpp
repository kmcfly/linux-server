#include "HxNetReader.h"

 CHxNetReader *	CHxNetReader::m_pHxNetReader = NULL;
 CHxNetReader * CHxNetReader::Instance()
 {
 	if (NULL == m_pHxNetReader)
 	{
 		m_pHxNetReader = new CHxNetReader;
 	}
 
 	return m_pHxNetReader;
 }

CHxNetReader::CHxNetReader(void)
{
	m_bReading = false;
	m_pFilePath = NULL;
}

CHxNetReader::~CHxNetReader(void)
{
}

/************************************************************************
*
*							public method
*
************************************************************************/
bool CHxNetReader::Initial(CReclogManEX *pReclogManEx, CMessageMan *pMsgMan, const char *pFilePath)
{
	assert(pReclogManEx != NULL);
	assert(pMsgMan != NULL);

	m_pReclogMan = pReclogManEx;
	m_pMsgMan = pMsgMan;
	m_hReadThread = PUB_THREAD_ID_NOINIT;

	return m_reader.Initial(pReclogManEx, pFilePath);
}

void CHxNetReader::Quit()
{
	m_reader.Quit();

	m_pReclogMan       = NULL;
	m_pFilePath        = NULL;
	m_pMsgMan          = NULL;
}

bool CHxNetReader::Start()
{
	return CreateReadThd();
}

void CHxNetReader::Stop()
{
	CloseReadThd();
}

bool CHxNetReader::StartRead(unsigned long clientID, long channel, unsigned long startTime, unsigned long endTime, unsigned long diskOwnerIndex)
{
	//互信互通的一个链接有可能请求多次。

	//清除当前使用的空间
	m_frameTempListLock.Lock();

	for(std::list<CLIENT_FRAME_TEMP>::iterator clientFrameTempIter = m_clientFrameTempList.begin(); clientFrameTempIter != m_clientFrameTempList.end(); clientFrameTempIter++)
	{
		if (clientID == clientFrameTempIter->clientID)
		{
			std::list<CFrameData *> *pFrameList = clientFrameTempIter->pFrameList;

			for(std::list<CFrameData *>::iterator iter = pFrameList->begin(); iter != pFrameList->end();)
			{
				assert(0 == (*iter)->GetStatus());
				m_reader.ReleaseFrame((*iter)->GetFrameInfo());
				delete *iter;
				iter = pFrameList->erase(iter);
			}

			delete clientFrameTempIter->pFrameList;
			
			m_clientFrameTempList.erase(clientFrameTempIter);

			break;
		}
	}

	m_frameTempListLock.UnLock();
	
	//清除当前读数据功能块
	m_reader.Stop(clientID);

	//添加一个新的读数据功能块
	bool bStartOK = m_reader.Start(clientID, startTime, endTime, channel, diskOwnerIndex);
	

	CLIENT_FRAME_TEMP newClientFrameTemp;
	newClientFrameTemp.clientID = clientID;
	newClientFrameTemp.pFrameList = new std::list<CFrameData *>;

	m_frameTempListLock.Lock();
	m_clientFrameTempList.push_back(newClientFrameTemp);
	m_frameTempListLock.UnLock();
	
	return bStartOK;
}

void CHxNetReader::StopRead(unsigned long clientID)
{
	m_frameTempListLock.Lock();

	for(std::list<CLIENT_FRAME_TEMP>::iterator clientFrameTempIter = m_clientFrameTempList.begin(); clientFrameTempIter != m_clientFrameTempList.end(); clientFrameTempIter++)
	{
		if (clientID == clientFrameTempIter->clientID)
		{
			std::list<CFrameData *> *pFrameList = clientFrameTempIter->pFrameList;

			for(std::list<CFrameData *>::iterator iter = pFrameList->begin(); iter != pFrameList->end();)
			{
				while (0 != (*iter)->GetStatus())
				{
					PUB_Sleep(10);
					printf("%s:%s:%d, The data is in use\n", __FUNCTION__, __FILE__, __LINE__);
				}

				m_reader.ReleaseFrame((*iter)->GetFrameInfo());

				delete *iter;
				iter = pFrameList->erase(iter);
			}

			delete clientFrameTempIter->pFrameList;

			m_clientFrameTempList.erase(clientFrameTempIter);

			break;
		}
	}

	m_frameTempListLock.UnLock();

	
	m_reader.Stop(clientID);
}

/************************************************************************
*
*							private method
*
************************************************************************/
RESULT WINAPI CHxNetReader::ReadThread(LPVOID lpParam)
{
#ifdef __ENVIRONMENT_LINUX__
	pid_t pid = getpid();
	pid_t tid = syscall(__NR_gettid);
	printf("%s, %d, pid = %d, tid=%d\n", __FILE__, __LINE__, pid, tid);
#endif

	CHxNetReader *pThis = reinterpret_cast<CHxNetReader *>(lpParam);
	if(pThis != NULL)
	{
		pThis->ReadProc();
	}

	return 0;
}

bool CHxNetReader::CreateReadThd()
{
	assert(PUB_THREAD_ID_NOINIT == m_hReadThread); //只能建立一次线程

	m_hReadThread = PUB_CreateThread(ReadThread, (void *)this, &m_bReading);	
	if( PUB_CREATE_THREAD_FAIL == m_hReadThread)
	{
		PUB_PrintError(__FILE__,__LINE__);
		return false;
	}

	return  true;
}

void CHxNetReader::CloseReadThd()
{
	PUB_ExitThread(&m_hReadThread, &m_bReading);
}

void CHxNetReader::ReadProc()
{
	FRAME_INFO_EX frameInfo;
	long readRet = HX_READ_SUCCESS;

	while(m_bReading)
	{
		DelUsedFrame();

		readRet = m_reader.GetNextFrame(frameInfo);
		if(HX_READ_SUCCESS == readRet)
		{
			CFrameData *pFrameData = new CFrameData(frameInfo, 1);
			assert(NULL != pFrameData);
			pFrameData->SetProtocol(CMD_REPLY_DATA_STREAM);
			pFrameData->SetDataId(frameInfo.streamID);

			if (FRAME_TYPE_NONE != frameInfo.frameType)
			{
				m_pMsgMan->PutPBFrame(pFrameData);
			}

			AddNewFrame(frameInfo.streamID, pFrameData);
		}
		else if(HX_READ_ERR_NO_BUFFER == readRet)
		{
			PUB_Sleep(10);
		}
		else
		{
			PUB_Sleep(10);
		}
	}
}


bool CHxNetReader::AddNewFrame(unsigned long clientID, CFrameData * pFrameData)
{
	if (NULL == pFrameData )
	{
		return false;
	}

	if (FRAME_TYPE_END == pFrameData->GetFrameInfo().frameType)
	{
		return true;
	}

	m_frameTempListLock.Lock();
	for(std::list<CLIENT_FRAME_TEMP>::iterator iter = m_clientFrameTempList.begin(); iter != m_clientFrameTempList.end(); iter++)
	{
		if (clientID == iter->clientID)
		{
			iter->pFrameList->push_back(pFrameData);

			m_frameTempListLock.UnLock();
			return true;
		}
	}

	m_frameTempListLock.UnLock();
	assert("false");
	return false;
}

bool CHxNetReader::DelUsedFrame()
{
	m_frameTempListLock.Lock();
	for(std::list<CLIENT_FRAME_TEMP>::iterator iter = m_clientFrameTempList.begin(); iter != m_clientFrameTempList.end(); iter++)
	{
		for (std::list<CFrameData *>::iterator it = iter->pFrameList->begin(); it != iter->pFrameList->end();)
		{
			if (0 == (*it)->GetStatus())
			{
				m_reader.ReleaseFrame((*it)->GetFrameInfo());
				delete (*it);
				it = iter->pFrameList->erase(it);
			}
			else
			{
				break;
			}
		}
	}

	m_frameTempListLock.UnLock();
	return true;
}
