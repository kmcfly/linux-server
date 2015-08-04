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
*	����ͬ���������ݵķ�ʽ��ͨ���˽ӿ�ֱ�ӵ���socket���ͽӿڷ�������
*
*	����ֵ��
*	���ͳɹ�������ʵ�ʷ��ͳɹ����ֽ���
	����ʧ�ܣ�����-1
*
*	������bBlock: �������Ϊtrue�����˽ӿ�Ϊ����ģʽ�����ȴ����ݷ�����ϻ�ʱΪֹ
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
		sendLen = SendBuff(deviceID, pBuf, iLen, true, 100);	//�ٶ�100����ܷ��ͳɹ�
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
*�����Զ��������ݵĻص�����
*�ɹ�����0��ʧ�ܷ���-1
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

		//ָ���ķ��ͻ�������С������ڴ��������ݴ�С
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
*���ܣ�ִ��һ���������ݷ���
*����˵����bBlock true���� false����������ʱdwBlockTime����Ч��
		   dwBlockTime ����Ϊ��λ����
*����ֵ��>=0���͵��ֽ���
		 -1��ʾ�Է��Ͽ�socket
		 -2��������
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
	int iLeft = iLen;    //��Ҫ���͵����ݳ���
	const char *pTmp = static_cast<const char*>(pBuf);

	time_t		startTime = time(NULL);	//������¼��һ��û���ͳɹ���ʱ�䣬���㳬ʱ��
	time_t		nowTime = 0;	//������¼��ǰʱ��
	time_t		disTime = 0;

	while ( 0 != iLeft )
	{
		//���recv����0����֪���Է��Ͽ������������﷢�ͻ����Է��͵�������
		//�������ж�һ���Ƿ��Ѿ��Ͽ�
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
			//�������ʱû������
			if(SWL_EWOULDBLOCK() && !pNetLinkResource->bBroken)
			{
				if (!bBlock) 
				{
					nowTime = time(NULL);
					disTime = nowTime - startTime;
					
					//BUGS ����������ʱ�⣬��ϵͳʱ��Ŀ�Ҳ���ܳ����������
					if(static_cast<time_t>(lBlockTime) <= disTime)
					{
						return iLen - iLeft;
					}
					else if (0 <= disTime) //��ֹϵͳʱ�䱻�����ܶ�����	
					{
						;//ʲô������Ҫ��
					}
					else	//ϵͳ��ʱ����������´�������򵥵Ĵ�����Ϊ���¼�ʱ
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
			//���ᷢ�ͳ���Ϊ0�����ݣ����Բ�������Ϊ0�ķ���ֵ
			SWL_PrintError(__FILE__, __LINE__);
			assert(false);
			return iLen - iLeft;
		}
	}
	
	return iLen - iLeft;
}

/******************************************************************************
*
*���ܣ�ִ��һ���������ݽ���
*����˵����bBlock true���� false����������ʱdwBlockTime����Ч��
		   dwBlockTime ����Ϊ��λ����
*����ֵ��>=0���յ��ֽ���,
		 -1��ʾ�Է��Ͽ�socket����
		 -2��ʾ��������
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
	int iLeft = iLen;    //��Ҫ���յ����ݳ���
	char *pTmp = pNetLinkResource->recvBuffer.pData+pNetLinkResource->recvBuffer.dataSize;
	char * pBuf = pTmp;

	bool bContinue = false;	//�ڷ���������£��Ƿ���Ҫ�ٵȴ�����
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
			//�������ʱû������,�ж��Ƿ��Ѿ���ʱ
			if(SWL_EWOULDBLOCK() && !pNetLinkResource->bBroken)
			{
				return iLen - iLeft;
// 				nowTime = time(NULL);
// 				disTime = nowTime - startTime;
// 				if (disTime >= lBlockTime)	//BUGS ���������ĳ�ʱ�⣬ϵͳ��ʱ��Ŀ�Ҳ���ܵ���
// 				{
// //					pNetLinkResource->bBroken = true;
// 					return iLen - iLeft;
// 				}
// 				else if(disTime >= 0)	//��ֹϵͳʱ��������²�ֵΪ����������������Ҫһ��̫��ʱ����ܳ�ʱ���缸ʮ��
// 				{
// 					if(bBlock || bContinue) //������������������˲������ݣ�
// 					{
// 						PUB_Sleep(0);
// 						continue;
// 					}
// 					else
// 					{
// 						return iLen - iLeft;
// 					}
// 				}
// 				else	//ϵͳ��ʱ����������´�������򵥵Ĵ�����Ϊ���¼�ʱ
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

		//����select��ʱʱ��Ϊ10ms
		//timeOut.tv_sec = 0;
		timeOut.tv_usec = 10000;

		ret = select(m_maxSockInt+1, &tempReadSet, NULL, NULL, &timeOut);
		if(ret > 0)
		{
			m_lstLinkLock.Lock();

			//�ڴ˴ζ��������У������ӽ����˶�������bProcessedData����Ϊtrue���Ա����Ͻ�����һ�ζ����
			bProcessedData = false;

			//��ÿ��socket����д���
			for(it = m_listNetLink.begin(); it != m_listNetLink.end(); it++)
			{
				pNetLinkResource = *it;

				//�����ӽ��пɶ��Լ��
				if(FD_ISSET(pNetLinkResource->sock, &tempReadSet))
				{
					//���ջ��������пռ�
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
					//�����л��пռ�����µ�����,����������Ӹս��յ�������,�Ͳ���ҪSleep
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

	//�߳�ִ�н���
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

