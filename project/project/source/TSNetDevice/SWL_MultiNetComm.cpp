// SWL_MultiNetComm.cpp: implementation of the CTS_MultiNetComm class.
//
//////////////////////////////////////////////////////////////////////
#include "Typedef.h"
#include "SWL_MultiNetComm.h"
#ifdef __ENVIRONMENT_LINUX__
#include <unistd.h>
#endif
#include "TSNetDevice.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTS_MultiNetComm::CTS_MultiNetComm()
{
	m_maxSockInt		= 0;
	m_bSockThreadRun	= false;
	m_hRecvThread		= PUB_CREATE_THREAD_FAIL;
	FD_ZERO(&m_fdReadSet);

	m_recvThreadId		= 0;
	m_bNeedDelNetLink	= false;
}

CTS_MultiNetComm::~CTS_MultiNetComm()
{

}

bool CTS_MultiNetComm::Init()
{
	m_bNeedDelNetLink	= false;
	m_hRecvThread = PUB_CreateThread(RecvThread, this, &m_bSockThreadRun);
	if(m_hRecvThread == PUB_CREATE_THREAD_FAIL)
	{
		assert(false);
		return false;
	}
	return true;
}

void CTS_MultiNetComm::Destroy()
{
	if(m_hRecvThread != PUB_CREATE_THREAD_FAIL)
	{
		PUB_ExitThread(&m_hRecvThread, &m_bSockThreadRun);
	}
	m_lstLinkLock.Lock();

	list<NET_LINK_RESOURCE*>::iterator it;
	for(it = m_listNetLink.begin(); it != m_listNetLink.end(); it++)
	{
		NET_LINK_RESOURCE *pNetLinkResource = *it;
		FD_CLR(pNetLinkResource->sock, &m_fdReadSet);
		SWL_CloseSocket(pNetLinkResource->sock);
		if(pNetLinkResource->recvBuffer.pData != NULL)
		{
			delete [] pNetLinkResource->recvBuffer.pData;
		}
		delete pNetLinkResource;
	}
	m_listNetLink.clear();
	m_tempDelNetLink.clear();

	m_lstLinkLock.UnLock();
}
/******************************************************************************
*
*
*
******************************************************************************/
int CTS_MultiNetComm::AddComm(long deviceID, SWL_socket_t sock)
{
#ifdef __ENVIRONMENT_LINUX__
	int threadId = syscall(224);
#else
	int threadId = GetCurrentThreadId();
#endif

	if(threadId != m_recvThreadId)
	{
		m_lstLinkLock.Lock();
	}

	NET_LINK_RESOURCE *pNetLinkResource = new NET_LINK_RESOURCE;

	pNetLinkResource->deviceID = deviceID;
	pNetLinkResource->sock = sock;
	pNetLinkResource->bBroken = false;
	pNetLinkResource->pfRecvCallBack = NULL;
	memset(&pNetLinkResource->recvBuffer, 0, sizeof(RECV_DATA_BUFFER));

	m_listNetLink.push_back(pNetLinkResource);

	if(static_cast<int>(sock) > m_maxSockInt)
	{
		m_maxSockInt = static_cast<int>(sock);
	}

	FD_SET(sock, &m_fdReadSet);
	
	if(threadId != m_recvThreadId)
	{
		m_lstLinkLock.UnLock();
	}

	return 0;
}

/******************************************************************************
*
*
*
******************************************************************************/
int CTS_MultiNetComm::RemoveComm(long deviceID)
{
#ifdef __ENVIRONMENT_LINUX__
	int threadId = syscall(224);
#else
	int threadId = GetCurrentThreadId();
#endif
	
	if(threadId != m_recvThreadId)
	{
		m_lstLinkLock.Lock();
	}

	list<NET_LINK_RESOURCE*>::iterator it;
	for(it = m_listNetLink.begin(); it != m_listNetLink.end(); it++)
	{
		NET_LINK_RESOURCE *pNetLinkResource = (*it);
		if(pNetLinkResource->deviceID == deviceID)
		{
			m_tempDelNetLink.push_back(pNetLinkResource);
			m_bNeedDelNetLink	= true;
		}
	}

	if(threadId != m_recvThreadId)
	{
		m_lstLinkLock.UnLock();
	}

	return 0;
}

int CTS_MultiNetComm::RemoveAllConnect()
{
#ifdef __ENVIRONMENT_LINUX__
	int threadId = syscall(224);
#else
	int threadId = GetCurrentThreadId();
#endif

	if(threadId != m_recvThreadId)
	{
		m_lstLinkLock.Lock();
	}

	list<NET_LINK_RESOURCE*>::iterator it;
	for(it = m_listNetLink.begin(); it != m_listNetLink.end(); it++)
	{
		m_tempDelNetLink.push_back(*it);
	}

	if(threadId != m_recvThreadId)
	{
		m_lstLinkLock.UnLock();
	}

	UpdateRemoveComm();

	return 0;
}

/******************************************************************************
*
*	采用同步发送数据的方式，通过此接口直接调用socket发送接口发送数据
*
*	返回值：
*	发送成功，返回实际发送成功的字节数
	发送失败，返回-1
*
*	参数：bBlock: 如果设置为true，即此接口为阻塞模式，则会等待数据发送完毕或超时为止
*
******************************************************************************/
int CTS_MultiNetComm::SendData(long deviceID, const void *pBuf, int iLen, bool bBlock)
{
#ifdef __ENVIRONMENT_LINUX__
	int threadId = syscall(224);
#else
	int threadId = GetCurrentThreadId();
#endif

	if(threadId != m_recvThreadId)
	{
		m_lstLinkLock.Lock();
	}

	int sendLen = -1;
	if(bBlock)
	{
		sendLen = SendBuff(deviceID, pBuf, iLen, true, 100);	//假定100秒会能发送成功
		assert(sendLen == iLen);
	}
	else
	{
		sendLen = SendBuff(deviceID, pBuf, iLen, false, 0);
	}

	if(threadId != m_recvThreadId)
	{
		m_lstLinkLock.UnLock();
	}

	return sendLen;
}

/******************************************************************************
*
*设置自动接收数据的回调函数
*成功返回0，失败返回-1
*
******************************************************************************/
int CTS_MultiNetComm::SetAutoRecvCallback(long deviceID, RECVASYN_CALLBACK pfRecvCallBack, void *pParam)
{
#ifdef __ENVIRONMENT_LINUX__
	int threadId = syscall(224);
#else
	int threadId = GetCurrentThreadId();
#endif

	if(threadId != m_recvThreadId)
	{
		m_lstLinkLock.Lock();
	}

	NET_LINK_RESOURCE *pNetLinkResource;
	if(GetLinkResourceByLinkID(deviceID, pNetLinkResource))
	{
		pNetLinkResource->pfRecvCallBack = pfRecvCallBack;
		pNetLinkResource->pParam = pParam;

		if(threadId != m_recvThreadId)
		{
			m_lstLinkLock.UnLock();
		}

		return 0;
	}

	if(threadId != m_recvThreadId)
	{
		m_lstLinkLock.UnLock();
	}

	return -1;
}

void CTS_MultiNetComm::SetRecvBufferLen(long deviceID, long bufferLen)
{
	assert(bufferLen > 0);

#ifdef __ENVIRONMENT_LINUX__
	int threadId = syscall(224);
#else
	int threadId = GetCurrentThreadId();
#endif

	if(threadId != m_recvThreadId)
	{
		m_lstLinkLock.Lock();
	}

	NET_LINK_RESOURCE *pNetLinkResource;
	if(GetLinkResourceByLinkID(deviceID, pNetLinkResource))
	{
		assert(pNetLinkResource->recvBuffer.dataSize <= bufferLen);

		//指定的发送缓冲区大小必须大于待发送数据大小
		if(pNetLinkResource->recvBuffer.dataSize > bufferLen)
		{
			if(threadId != m_recvThreadId)
			{
				m_lstLinkLock.UnLock();
			}
			return;
		}

		char *pOldBuffer = pNetLinkResource->recvBuffer.pData;
		pNetLinkResource->recvBuffer.bufferSize = bufferLen;
		pNetLinkResource->recvBuffer.pData = new char [bufferLen];
		memset(pNetLinkResource->recvBuffer.pData, 0, bufferLen);

		if(pOldBuffer != NULL)
		{
			if(pNetLinkResource->recvBuffer.dataSize > 0)
			{
				memcpy(pNetLinkResource->recvBuffer.pData, pOldBuffer, pNetLinkResource->recvBuffer.dataSize);
			}

			delete [] pOldBuffer;
		}
	}

	if(threadId != m_recvThreadId)
	{
		m_lstLinkLock.UnLock();
	}
}

/******************************************************************************
*
*功能：执行一次网络数据发送
*参数说明：bBlock true阻塞 false非阻塞，此时dwBlockTime才有效，
		   dwBlockTime 以秒为单位阻塞
*返回值：>=0发送的字节数
		 -1表示对方断开socket
		 -2其他错误
*
******************************************************************************/

int CTS_MultiNetComm::SendBuff(long deviceID, const void *pBuf, int iLen, bool bBlock, long lBlockTime)
{
	NET_LINK_RESOURCE *pNetLinkResource;
	if(!GetLinkResourceByLinkID(deviceID, pNetLinkResource))
	{
		return -2;
	}

	assert(NULL != pBuf);
	assert(0 < iLen);
	int iRet = 0;
	int iLeft = iLen;    //还要发送的数据长度
	const char *pTmp = static_cast<const char*>(pBuf);

	time_t		startTime = time(NULL);	//用来记录第一次没发送成功的时间，计算超时用
	time_t		nowTime = 0;	//用来记录当前时间
	time_t		disTime = 0;

	while ( 0 != iLeft )
	{
		//如果recv返回0（即知道对方断开），但是这里发送还可以发送到缓冲区
		//所以先判断一下是否已经断开
		if (pNetLinkResource->bBroken) 
		{
			return -1;
		}
		
		assert(0 < iLeft);
		assert(iLeft <= iLen);
		assert(pTmp >= pBuf);
		assert(pTmp <= (static_cast<const char*>(pBuf) + iLen));

		iRet = SWL_Send(pNetLinkResource->sock, pTmp , iLeft, 0);

		if ( 0 < iRet)
		{
			pTmp += iRet;
			iLeft -= iRet;
		}
		else if( SWL_SOCKET_ERROR == iRet)
		{
			//如果是暂时没有数据
			if(SWL_EWOULDBLOCK() && !pNetLinkResource->bBroken)
			{
				if (!bBlock) 
				{
					nowTime = time(NULL);
					disTime = nowTime - startTime;
					
					//BUGS 除了正常超时外，包系统时间改快也可能出现这种情况
					if(static_cast<time_t>(lBlockTime) <= disTime)
					{
						return iLen - iLeft;
					}
					else if (0 <= disTime) //防止系统时间被改慢很多的情况	
					{
						;//什么都不需要做
					}
					else	//系统将时间调慢将导致此情况，简单的处理方法为重新记时
					{
						startTime = time(NULL);
					}
				}				

#ifdef  __ENVIRONMENT_WIN32__
				PUB_Sleep(3);
#elif defined __ENVIRONMENT_LINUX__
				PUB_Sleep(0);
#endif
				continue;
			}
			else
			{
				pNetLinkResource->bBroken = true;
				SWL_PrintError(__FILE__, __LINE__);
				return -1;
			}
		}
		else	// 0 == iRet
		{
			//不会发送长度为0的数据，所以不可能有为0的返回值
			SWL_PrintError(__FILE__, __LINE__);
			assert(false);
			return iLen - iLeft;
		}
	}
	
	return iLen - iLeft;
}

/******************************************************************************
*
*功能：执行一次网络数据接收
*参数说明：bBlock true阻塞 false非阻塞，此时dwBlockTime才有效，
		   dwBlockTime 以秒为单位阻塞
*返回值：>=0接收的字节数,
		 -1表示对方断开socket连接
		 -2表示其他错误
*
******************************************************************************/
int CTS_MultiNetComm::RecvBuff(NET_LINK_RESOURCE *pNetLinkResource)
{
// 	NET_LINK_RESOURCE *pNetLinkResource;
// 	if(!GetLinkResourceByLinkID(deviceID, pNetLinkResource))
// 	{
// 		assert(false);
// 		return -2;
// 	}

	int iRet = 0;
	int iLen = pNetLinkResource->recvBuffer.bufferSize - pNetLinkResource->recvBuffer.dataSize;
	int iLeft = iLen;    //还要接收的数据长度
	char *pTmp = pNetLinkResource->recvBuffer.pData+pNetLinkResource->recvBuffer.dataSize;
	char * pBuf = pTmp;

	bool bContinue = false;	//在非阻塞情况下，是否需要再等待数据
	time_t startTime = time(NULL);
	time_t nowTime;
	time_t disTime;
	
	while ( 0 != iLeft ) 
	{	
		if (pNetLinkResource->bBroken)
		{
			return -1;
		}	

		assert(0 < iLeft);
		assert(iLeft <= iLen);
		assert(pTmp >= pBuf);
		assert(pTmp < (static_cast<const char*>(pBuf) + iLen));

		iRet = SWL_Recv(pNetLinkResource->sock, pTmp, iLeft, 0);
		if (0 < iRet)
		{
			pTmp += iRet;
			iLeft -= iRet;
			bContinue = true;
//#if defined(__CHIP3535__)
			return iLen - iLeft;
//#endif
		}
		else if( SWL_SOCKET_ERROR == iRet)
		{
			//如果是暂时没有数据,判断是否已经超时
			if(SWL_EWOULDBLOCK() && !pNetLinkResource->bBroken)
			{
				return iLen - iLeft;
// 				nowTime = time(NULL);
// 				disTime = nowTime - startTime;
// 				if (disTime >= lBlockTime)	//BUGS 除了正常的超时外，系统把时间改快也可能导致
// 				{
// //					pNetLinkResource->bBroken = true;
// 					return iLen - iLeft;
// 				}
// 				else if(disTime >= 0)	//防止系统时间调慢后导致差值为负数，甚至导致需要一个太长时间才能超时，如几十年
// 				{
// 					if(bBlock || bContinue) //阻塞，或非阻塞接收了部分数据，
// 					{
// 						PUB_Sleep(0);
// 						continue;
// 					}
// 					else
// 					{
// 						return iLen - iLeft;
// 					}
// 				}
// 				else	//系统将时间调慢将导致此情况，简单的处理方法为重新记时
// 				{
// 					startTime = time(NULL);
// 				}
			}
			else
			{
				SWL_PrintError(__FILE__, __LINE__);
				pNetLinkResource->bBroken = true;
				return -1;
			}			
		}
		else //0 == iRet
		{
			SWL_PrintError(__FILE__, __LINE__);
			pNetLinkResource->bBroken = true;
			return -1;
		}
	}
	return iLen - iLeft;
}

bool CTS_MultiNetComm::BindThreadToCpu(int pid, int whichCpu)
{
#ifdef __ENVIRONMENT_LINUX__
	cpu_set_t mask;

	CPU_ZERO(&mask);
	CPU_SET(whichCpu, &mask);
	if (0 != sched_setaffinity((pid_t)pid, sizeof(mask), &mask))
	{
		fprintf(stderr, "%s %s %d\n", strerror(errno), __FILE__, __LINE__);
		return false;
	}
	else
	{
		return true;
	}
#else 
	return false;
#endif
}

PUB_THREAD_RESULT PUB_THREAD_CALL CTS_MultiNetComm::RecvThread(void *pParam)
{
	CTS_MultiNetComm * pThis = reinterpret_cast<CTS_MultiNetComm *>(pParam);

#ifdef __ENVIRONMENT_LINUX__
	pThis->m_recvThreadId = syscall(224);
	printf("%s, %d, pid = %d\n", __FILE__, __LINE__, pThis->m_recvThreadId);
#if defined(__CHIP3535__)
	char szCommond[128] = {0};
	snprintf(szCommond, sizeof(szCommond), "taskset -p 0x2 %d", pThis->m_recvThreadId);
	printf("%s:%s:%d, The commond is %s\n", __FUNCTION__, __FILE__, __LINE__, szCommond);
	DVRSystem(szCommond);
	PUB_Sleep(100);
	printf("%s:%s:%d, The commond is %s\n", __FUNCTION__, __FILE__, __LINE__, szCommond);
	DVRSystem(szCommond);
#elif defined(__CHIP3531__)
	CTS_MultiNetComm::BindThreadToCpu(pThis->m_recvThreadId, 1);
#endif
#else
	pThis->m_recvThreadId = GetCurrentThreadId();
#endif

	pThis->RecvThreadRun();

	return 0;
}

void CTS_MultiNetComm::RecvThreadRun()
{
	int ret = 0;
	int callbackRet;

	timeval timeOut;
	fd_set tempReadSet;
	bool bFrameOver = false;
	bool bProcessedData = false;
	list<NET_LINK_RESOURCE *>::iterator it;
	NET_LINK_RESOURCE *pNetLinkResource = NULL;

	timeOut.tv_sec = 0;

	while(m_bSockThreadRun)
	{
		if (m_bNeedDelNetLink)
		{
			UpdateRemoveComm();
		}

		tempReadSet = m_fdReadSet;

		//设置select超时时间为10ms
		//timeOut.tv_sec = 0;
		timeOut.tv_usec = 10000;

		ret = select(m_maxSockInt+1, &tempReadSet, NULL, NULL, &timeOut);
		if(ret > 0)
		{
			m_lstLinkLock.Lock();

			//在此次读检查过程中，有连接进行了读操作，bProcessedData便置为true，以便马上进入下一次读检查
			bProcessedData = false;

			//对每个socket作读写检查
			for(it = m_listNetLink.begin(); it != m_listNetLink.end(); it++)
			{
				pNetLinkResource = *it;

				//对连接进行可读性检查
				if(FD_ISSET(pNetLinkResource->sock, &tempReadSet))
				{
					//接收缓冲区还有空间
					if(pNetLinkResource->recvBuffer.dataSize < pNetLinkResource->recvBuffer.bufferSize)
					{
						ret = RecvBuff(pNetLinkResource);

						if(ret > 0)
						{
							pNetLinkResource->recvBuffer.dataSize += ret;
						}
						else if(ret < 0)
						{
							pNetLinkResource->bBroken = true;
							FD_CLR(pNetLinkResource->sock, &m_fdReadSet);

							if(pNetLinkResource->pfRecvCallBack != NULL)
							{
								pNetLinkResource->pfRecvCallBack(pNetLinkResource->deviceID, pNetLinkResource->pParam, NULL);
							}

							break;
						}
						else
						{
//							assert(false);
							continue;
						}
					}
				}

				if(pNetLinkResource->recvBuffer.dataSize > 0)
				{
					if(pNetLinkResource->pfRecvCallBack != NULL)
					{
						if(pNetLinkResource->recvBuffer.dataSize > sizeof(COMM_PACK_HEAD))
						{
							callbackRet = pNetLinkResource->pfRecvCallBack(pNetLinkResource->deviceID, \
								pNetLinkResource->pParam, \
								&pNetLinkResource->recvBuffer);
						}
					}
					else
					{
						callbackRet = 0;
						pNetLinkResource->recvBuffer.dataSize = 0;
					}
				}

				if(pNetLinkResource->recvBuffer.dataSize <= (pNetLinkResource->recvBuffer.bufferSize >> 1))
				{
					//缓存中还有空间接收新的数据,并且这个连接刚接收到了数据,就不需要Sleep
					if(FD_ISSET(pNetLinkResource->sock, &tempReadSet) && (ret >= 5120))
					{
						bProcessedData = true;
					}
				}
			}

			m_lstLinkLock.UnLock();


#if defined(__ENVIRONMENT_WIN32__)
			PUB_Sleep(10);
#else

#if !defined(__PACKID_21__)
			if (0 < timeOut.tv_usec && 10000 >= timeOut.tv_usec)
			{
				usleep(10000 - timeOut.tv_usec);
			}
#endif
#endif
		}
	}

	//线程执行结束
	printf("CTS_MultiNetComm::%s exit. %s, %d\n", __FUNCTION__, __FILE__, __LINE__);
}

bool CTS_MultiNetComm::GetLinkResourceByLinkID(long deviceID, NET_LINK_RESOURCE * &pLinkResource)
{
	list<NET_LINK_RESOURCE*>::iterator it;
	for(it = m_listNetLink.begin(); it != m_listNetLink.end(); it++)
	{
		NET_LINK_RESOURCE *pNetLinkResource = *it;
		
		if(pNetLinkResource->deviceID == deviceID)
		{
			pLinkResource = pNetLinkResource;
			return true;
		}
	}

	return false;
}

void CTS_MultiNetComm::UpdateRemoveComm(bool bClearBar/*=false*/)
{
	m_lstLinkLock.Lock();

	while(!m_tempDelNetLink.empty())
	{
		NET_LINK_RESOURCE *pNetLinkResource = m_tempDelNetLink.front();
		m_tempDelNetLink.pop_front();

		list<NET_LINK_RESOURCE*>::iterator it;
		for(it = m_listNetLink.begin(); it != m_listNetLink.end(); it++)
		{
			if(*it == pNetLinkResource)
			{
				m_listNetLink.erase(it);
				FD_CLR(pNetLinkResource->sock, &m_fdReadSet);

				SWL_CloseSocket(pNetLinkResource->sock);
				if(NULL != pNetLinkResource->recvBuffer.pData)
				{
					delete [] pNetLinkResource->recvBuffer.pData;
				}
				delete pNetLinkResource;
				break;
			}
		}
	}
	
	if (bClearBar)
	{
		m_bNeedDelNetLink	= false;
	}
	
	m_lstLinkLock.UnLock();
}

