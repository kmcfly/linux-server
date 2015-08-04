// SWL_MultiNetComm.cpp: implementation of the CSWL_MultiNetCommEx class.
//
//////////////////////////////////////////////////////////////////////
//#include "TVT_PubDefine.h"

#include "SWL_MultiNetCommEx.h"

#ifndef WIN32
#include <sys/syscall.h>
#endif
#ifdef __DEBUG_NEW__
#include "debug_new.h"
#define new DEBUG_NEW
#endif

#if defined __ENVIRONMENT_LINUX__
#include <sys/types.h>
#include <unistd.h>
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSWL_MultiNetCommEx::CSWL_MultiNetCommEx()
{
	m_maxSockInt		= 0;
	m_bSockThreadRun	= false;
	m_threadId			= PUB_CREATE_THREAD_FAIL;
	FD_ZERO(&m_fdReadSet);
}

CSWL_MultiNetCommEx::~CSWL_MultiNetCommEx()
{

}

bool CSWL_MultiNetCommEx::Init()
{
	m_threadId = PUB_CreateThread(RecvThread, this, &m_bSockThreadRun);
	if(m_threadId == PUB_CREATE_THREAD_FAIL)
	{
		assert(false);
		return false;
	}
	return true;
}

void CSWL_MultiNetCommEx::Destroy()
{
	if(m_threadId != PUB_CREATE_THREAD_FAIL)
	{
		PUB_ExitThread(&m_threadId, &m_bSockThreadRun);
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
int CSWL_MultiNetCommEx::AddComm(long deviceID, SWL_socket_t sock)
{
	m_lstLinkLockForCallback.Lock();
	m_lstLinkLock.Lock();

	NET_LINK_RESOURCE *pNetLinkResource = new NET_LINK_RESOURCE;
	assert(NULL != pNetLinkResource);

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
	
	m_lstLinkLock.UnLock();
	m_lstLinkLockForCallback.UnLock();

	return 0;
}

/******************************************************************************
*
*
*
******************************************************************************/
int CSWL_MultiNetCommEx::RemoveComm(long deviceID)
{
	m_lstLinkLock.Lock();

	list<NET_LINK_RESOURCE*>::iterator it;
	for(it = m_listNetLink.begin(); it != m_listNetLink.end(); it++)
	{
		NET_LINK_RESOURCE *pNetLinkResource = (*it);
		if(pNetLinkResource->deviceID == deviceID)
		{
			m_tempDelNetLink.push_back(pNetLinkResource);
		}
	}

	m_lstLinkLock.UnLock();

	return 0;
}

int CSWL_MultiNetCommEx::RemoveAllConnect()
{
	m_lstLinkLock.Lock();

	list<NET_LINK_RESOURCE*>::iterator it;
	for(it = m_listNetLink.begin(); it != m_listNetLink.end(); it++)
	{
		m_tempDelNetLink.push_back(*it);
	}

	m_lstLinkLock.UnLock();

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
int CSWL_MultiNetCommEx::SendData(long deviceID, const void *pBuf, int iLen, bool bBlock)
{
	m_lstLinkLock.Lock();

	int sendLen = -1;
	if(bBlock)
	{
		sendLen = SendBuff(deviceID, pBuf, iLen, true, 100);	//�ٶ�100����ܷ��ͳɹ�
//		assert(sendLen == iLen);
	}
	else
	{
		sendLen = SendBuff(deviceID, pBuf, iLen, false, 0);
	}

	m_lstLinkLock.UnLock();

	return sendLen;
}

/******************************************************************************
*
*�����Զ��������ݵĻص�����
*�ɹ�����0��ʧ�ܷ���-1
*
******************************************************************************/
int CSWL_MultiNetCommEx::SetAutoRecvCallback(long deviceID, RECVASYN_CALLBACK pfRecvCallBack, void *pParam)
{
	m_lstLinkLockForCallback.Lock();

	NET_LINK_RESOURCE *pNetLinkResource;
	if(GetLinkResourceByLinkID(deviceID, pNetLinkResource))
	{
		pNetLinkResource->pfRecvCallBack = pfRecvCallBack;
		pNetLinkResource->pParam = pParam;
		m_lstLinkLockForCallback.UnLock();
		return 0;
	}
	m_lstLinkLockForCallback.UnLock();

	return -1;
}

void CSWL_MultiNetCommEx::SetRecvBufferLen(long deviceID, long bufferLen)
{
	assert(bufferLen > 0);

	m_lstLinkLockForCallback.Lock();

	NET_LINK_RESOURCE *pNetLinkResource;
	if(GetLinkResourceByLinkID(deviceID, pNetLinkResource))
	{
		assert(pNetLinkResource->recvBuffer.dataSize <= bufferLen);

		//ָ���ķ��ͻ�������С������ڴ��������ݴ�С
		if(pNetLinkResource->recvBuffer.dataSize > bufferLen)
		{
			m_lstLinkLockForCallback.UnLock();
			return;
		}

		char *pOldBuffer = pNetLinkResource->recvBuffer.pData;
		pNetLinkResource->recvBuffer.bufferSize = bufferLen;
		pNetLinkResource->recvBuffer.pData = new char [bufferLen];
		assert(NULL != pNetLinkResource->recvBuffer.pData);
		memset(pNetLinkResource->recvBuffer.pData, 0, bufferLen );

		if(pOldBuffer != NULL)
		{
			if(pNetLinkResource->recvBuffer.dataSize > 0)
			{
				memcpy(pNetLinkResource->recvBuffer.pData, pOldBuffer, pNetLinkResource->recvBuffer.dataSize);
			}

			delete [] pOldBuffer;
		}
	}

	m_lstLinkLockForCallback.UnLock();
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

int CSWL_MultiNetCommEx::SendBuff(long deviceID, const void *pBuf, int iLen, bool bBlock, long lBlockTime)
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

#ifdef  WIN32
				PUB_Sleep(3);
#else
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
int CSWL_MultiNetCommEx::RecvBuff(long deviceID, void *pBuf, int iLen, bool bBlock, long lBlockTime)
{
	NET_LINK_RESOURCE *pNetLinkResource;
	if(!GetLinkResourceByLinkID(deviceID, pNetLinkResource))
	{
		assert(false);
		return -2;
	}
	assert( (NULL != pBuf) && (0 != iLen) );

	int iRet = 0;
	int iLeft = iLen;    //��Ҫ���յ����ݳ���
	char *pTmp = reinterpret_cast<char*>(pBuf);

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
			if(!bBlock)
			{
				break;
			}
		}
		else if( SWL_SOCKET_ERROR == iRet)
		{
			//�������ʱû������,�ж��Ƿ��Ѿ���ʱ
			if(SWL_EWOULDBLOCK() && !pNetLinkResource->bBroken)
			{
				if(bBlock)
				{
					nowTime = time(NULL);
					disTime = nowTime - startTime;
					if (disTime >= lBlockTime)	//BUGS ���������ĳ�ʱ�⣬ϵͳ��ʱ��Ŀ�Ҳ���ܵ���
					{
						break;
					}
					else if(disTime >= 0)	//��ֹϵͳʱ��������²�ֵΪ����������������Ҫһ��̫��ʱ����ܳ�ʱ���缸ʮ��
					{
						PUB_Sleep(0);
						continue;
					}
					else	//ϵͳ��ʱ����������´�������򵥵Ĵ�����Ϊ���¼�ʱ
					{
						startTime = time(NULL);
					}
				}
				else
				{
					break;
				}
			}
			else
			{
				SWL_PrintError(__FILE__, __LINE__);
				//SWL_CloseSocket(pNetLinkResource->sock);

				pNetLinkResource->bBroken = true;
				return -1;
			}			
		}
		else //0 == iRet
		{
			//SWL_PrintError(__FILE__, __LINE__);
			//SWL_CloseSocket(pNetLinkResource->sock);

			pNetLinkResource->bBroken = true;
			return -1;
		}
	}
	return iLen - iLeft;
}

PUB_THREAD_RESULT PUB_THREAD_CALL CSWL_MultiNetCommEx::RecvThread(void *pParam)
{
#ifndef	WIN32
	pid_t pid = getpid();
	pid_t tid = syscall(__NR_gettid);
	printf("%s, %d, pid = %d, tid=%d\n", __FILE__, __LINE__, pid, tid);
#endif

	CSWL_MultiNetCommEx * pThis = reinterpret_cast<CSWL_MultiNetCommEx *>(pParam);
	pThis->RecvThreadRun();

	return 0;
}

void CSWL_MultiNetCommEx::RecvThreadRun()
{
	int ret = 0;
	int callbackRet;

	timeval timeOut;
	fd_set tempReadSet;

	while(m_bSockThreadRun)
	{
		UpdateRemoveComm();

		//���û�����ӣ���Sleepһ�ᣬֱ������
		m_lstLinkLockForCallback.Lock();
		if(m_listNetLink.empty())
		{
			m_lstLinkLockForCallback.UnLock();
			PUB_Sleep(10);
			continue;
		}
		m_lstLinkLockForCallback.UnLock();

		tempReadSet = m_fdReadSet;

		//����select��ʱʱ��Ϊ10ms
		timeOut.tv_sec = 0;
		timeOut.tv_usec = 10000;

		ret = select(m_maxSockInt+1, &tempReadSet, NULL, NULL, &timeOut);
		if(ret > 0)
		{
			m_lstLinkLockForCallback.Lock();

			//�ڴ˴ζ��������У������ӽ����˶�������bProcessedData����Ϊtrue���Ա����Ͻ�����һ�ζ����
			bool bProcessedData = false;

			//��ÿ��socket����д���
			list<NET_LINK_RESOURCE *>::iterator it;
			for(it = m_listNetLink.begin(); it != m_listNetLink.end(); it++)
			{
				NET_LINK_RESOURCE *pNetLinkResource = *it;

				//�����ӽ��пɶ��Լ��
				if(FD_ISSET(pNetLinkResource->sock, &tempReadSet))
				{
					//���ջ��������пռ�
					if(pNetLinkResource->recvBuffer.dataSize < pNetLinkResource->recvBuffer.bufferSize)
					{
						ret = RecvBuff(pNetLinkResource->deviceID, \
							pNetLinkResource->recvBuffer.pData+pNetLinkResource->recvBuffer.dataSize, \
							pNetLinkResource->recvBuffer.bufferSize - pNetLinkResource->recvBuffer.dataSize, \
							false, 0);

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
								pNetLinkResource->pfRecvCallBack(pNetLinkResource->deviceID, \
									pNetLinkResource->pParam, NULL);
							}

							break;
						}
						else
						{
							continue;
						}
					}
				}

				if(pNetLinkResource->recvBuffer.dataSize > 0)
				{
					if(pNetLinkResource->pfRecvCallBack != NULL)
					{
						callbackRet = pNetLinkResource->pfRecvCallBack(pNetLinkResource->deviceID, \
							pNetLinkResource->pParam, \
							&pNetLinkResource->recvBuffer);
					}
					else
					{
						callbackRet = 0;
						pNetLinkResource->recvBuffer.dataSize = 0;
					}
				}

				if(pNetLinkResource->recvBuffer.dataSize < pNetLinkResource->recvBuffer.bufferSize)
				{
					//�����л��пռ�����µ�����,����������Ӹս��յ�������,�Ͳ���ҪSleep
					if(FD_ISSET(pNetLinkResource->sock, &tempReadSet))
					{
						bProcessedData = true;
					}
				}
			}

			m_lstLinkLockForCallback.UnLock();

			//�ڴ˹����в�û�������շ�������ʱ������߳�
			if(!bProcessedData)
			{
#ifdef  WIN32
				PUB_Sleep(3);
#else
				PUB_Sleep(0);
#endif
			}
		}
		else
		{
			//selectʧ�ܣ�û�����ݿɶ��������û��֮ǰδ��������ݣ��ص�����
			//��ÿ��socket����д���
			m_lstLinkLockForCallback.Lock();
			list<NET_LINK_RESOURCE *>::iterator it;
			for(it = m_listNetLink.begin(); it != m_listNetLink.end(); it++)
			{
				NET_LINK_RESOURCE *pNetLinkResource = *it;
				if(pNetLinkResource->recvBuffer.dataSize > 0)
				{
					if(pNetLinkResource->pfRecvCallBack != NULL)
					{
						pNetLinkResource->pfRecvCallBack(pNetLinkResource->deviceID, \
							pNetLinkResource->pParam, \
							&pNetLinkResource->recvBuffer);
					}
					else
					{
						pNetLinkResource->recvBuffer.dataSize = 0;
					}
				}
			}
			m_lstLinkLockForCallback.UnLock();
		}
	}

	//�߳�ִ�н���
	printf("CSWL_MultiNetCommEx::%s exit. %s, %d\n", __FUNCTION__, __FILE__, __LINE__);
}

bool CSWL_MultiNetCommEx::GetLinkResourceByLinkID(long deviceID, NET_LINK_RESOURCE * &pLinkResource)
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

void CSWL_MultiNetCommEx::UpdateRemoveComm()
{
	m_lstLinkLockForCallback.Lock();
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
				//printf("$$$$$$$$event close sock id is %d!\n",pNetLinkResource->sock);
				if(NULL != pNetLinkResource->recvBuffer.pData)
				{
					delete [] pNetLinkResource->recvBuffer.pData;
				}
				delete pNetLinkResource;
				break;
			}
		}
	}
	
	m_lstLinkLock.UnLock();
	m_lstLinkLockForCallback.UnLock();
}

