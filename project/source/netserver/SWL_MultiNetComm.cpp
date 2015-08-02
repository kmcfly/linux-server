// SWL_MultiNetComm.cpp: implementation of the CSWL_MultiNetComm class.
//
//////////////////////////////////////////////////////////////////////
#include "Typedef.h"
#ifdef __ENVIRONMENT_LINUX__
#include "lvfw.h"
#else
#include <vfw.h>
#endif
#include "SWL_MultiNetComm.h"

#include "PUB_common.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSWL_MultiNetComm::CSWL_MultiNetComm()
{
	m_maxSockInt		= 0;
	m_bSockThreadRun	= false;
	m_threadId			= PUB_CREATE_THREAD_FAIL;
	FD_ZERO(&m_fdReadSet);
	FD_ZERO(&m_fdWriteSet);

	m_bSendSync			= false;
	m_bNeedDelNetLink	= false;
}

CSWL_MultiNetComm::~CSWL_MultiNetComm()
{

}

bool CSWL_MultiNetComm::Init(long sendBufSize, bool bSendSync /* = false */)
{
	assert(sendBufSize > 0);
	m_sendBufSize = sendBufSize;
	m_bSendSync = bSendSync;

	m_threadId = PUB_CreateThread(RecvThread, this, &m_bSockThreadRun);
	if(m_threadId == PUB_CREATE_THREAD_FAIL)
	{
		assert(false);
		return false;
	}
	return true;
}

void CSWL_MultiNetComm::Destroy()
{
	if(m_threadId != PUB_CREATE_THREAD_FAIL)
	{
		PUB_ExitThread(&m_threadId, &m_bSockThreadRun);
	}
	m_lstLinkLock.Lock();
	m_listLinkSendLock.Lock();

	POS pos = m_lstNetLink.GetHeadPosition();
	while(pos)
	{
		NET_LINK_RESOURCE *pNetLinkResource = m_lstNetLink.GetNext(pos);
		pNetLinkResource->sendBuffer.Destroy();
		FD_CLR(pNetLinkResource->sock, &m_fdReadSet);
		FD_CLR(pNetLinkResource->sock, &m_fdWriteSet);
		SWL_CloseSocket(pNetLinkResource->sock);
		delete pNetLinkResource;
	}
	m_lstNetLink.RemoveAll();
	m_tempDelNetLink.RemoveAll();

	m_listLinkSendLock.UnLock();
	m_lstLinkLock.UnLock();
}
/******************************************************************************
*
*
*
******************************************************************************/
int CSWL_MultiNetComm::AddNewComm(long deviceID, SWL_socket_t sock)
{
	m_lstLinkLock.Lock();
	m_listLinkSendLock.Lock();

	NET_LINK_RESOURCE *pNetLinkResource = new NET_LINK_RESOURCE;
	pNetLinkResource->deviceID = deviceID;
	pNetLinkResource->sock = sock;
	pNetLinkResource->bBroken = false;
	pNetLinkResource->prevRecvTime = GetCurrTime()/1000;
	pNetLinkResource->prevSendTime = GetCurrTime()/1000;
	pNetLinkResource->pfRecvCallBack = NULL;
	pNetLinkResource->bAutoRecv = false;
	pNetLinkResource->sendBuffer.Init(m_sendBufSize);
	pNetLinkResource->bufferMaxLen = pNetLinkResource->sendBuffer.GetTotalSize();
	pNetLinkResource->bSendIdle = true;
	memset(&pNetLinkResource->recvBuffer, 0, sizeof(RECV_DATA_BUFFER));

	//����SOCKET���ͼ����ջ�������С
	int bufLen;
	SWL_socklen_t bufLenSize=sizeof(int); 

	if(getsockopt(sock,SOL_SOCKET,SO_RCVBUF,(char*)&bufLen,&bufLenSize) != SWL_SOCKET_ERROR)
	{
		if(bufLen < 32000)
		{
			bufLen = 32000;
			setsockopt(sock,SOL_SOCKET,SO_RCVBUF,(char*)&bufLen,bufLenSize); 
		}
	}

	if(getsockopt(sock,SOL_SOCKET,SO_SNDBUF,(char*) &bufLen,&bufLenSize)!=SWL_SOCKET_ERROR) 
	{ 
		if(bufLen < 32000)
		{
			bufLen = 32000;
			setsockopt(sock,SOL_SOCKET,SO_SNDBUF,(char*)&bufLen,bufLenSize);
		}
	}

	m_lstNetLink.AddTail(pNetLinkResource);

	if(static_cast<int>(sock) > m_maxSockInt)
	{
		m_maxSockInt = static_cast<int>(sock);
	}

	FD_SET(sock, &m_fdReadSet);
	FD_SET(sock, &m_fdWriteSet);
	
	m_listLinkSendLock.UnLock();
	m_lstLinkLock.UnLock();

	return 0;
}

/******************************************************************************
*
*
*
******************************************************************************/
int CSWL_MultiNetComm::RemoveComm(long deviceID)
{
	m_lstLinkLock.Lock();
	m_listLinkSendLock.Lock();

	POS pos = m_lstNetLink.GetHeadPosition();
	while(pos)
	{
		NET_LINK_RESOURCE *pNetLinkResource = m_lstNetLink.GetAt(pos);
		if(pNetLinkResource->deviceID == deviceID)
		{
			m_tempDelNetLink.AddTail(pNetLinkResource);
			m_bNeedDelNetLink = true;
			break;
		}
		m_lstNetLink.GetNext(pos);
	}
	
	m_listLinkSendLock.UnLock();
	m_lstLinkLock.UnLock();

	return 0;
}

int CSWL_MultiNetComm::RemoveAllConnect()
{
	m_lstLinkLock.Lock();

	POS pos = m_lstNetLink.GetHeadPosition();
	while(pos)
	{

		NET_LINK_RESOURCE *pNetLinkResource = m_lstNetLink.GetNext(pos);
		m_tempDelNetLink.AddTail(pNetLinkResource);

	}

	m_lstLinkLock.UnLock();


	UpdateRemoveComm();

	return 0;

}
/******************************************************************************
*
*	�����첽�������ݵķ�ʽ��ͨ���˽ӿڰ�Ҫ���͵������ݴ��뷢�����ݻ�����У��ɷ�
	���̴߳Ӵ˻�����в��ϵ�ȡ���ݷ���
*
*	����ֵ��
*	��������еĿ��пռ䲻���Է���Ҫ���͵�����ʱ������-1�����򷵻�0
*
*	������bRawData: Ϊtrue��ʾ���Ͳ����κ�ͷ����Ϣ��ԭʼ���ݣ��������һ��COMM_PACK_HEADͷ��Ϣ
*		  bBlock: �������Ϊtrue�����˽ӿ�Ϊ����ģʽ�����ȴ�������еĿ��пռ����Է���
		  ����������Ϊֹ
*
******************************************************************************/
int CSWL_MultiNetComm::SendData(long deviceID, const void *pBuf, int iLen, bool bBlock, bool bRawData)
{
	assert(!m_bSendSync);

	time_t		startTime = time(NULL);	//������¼��һ��û���ͳɹ���ʱ�䣬���㳬ʱ��
	time_t		nowTime = 0;	//������¼��ǰʱ��
	time_t		disTime = 0;

	while(true)
	{
		m_listLinkSendLock.Lock();
		NET_LINK_RESOURCE *pNetLinkResource;
		if(GetLinkResourceByLinkID(deviceID, pNetLinkResource))
		{
			if((pNetLinkResource->sendBuffer.GetDataSize() + (long)(iLen + sizeof(COMM_PACK_HEAD))) <= pNetLinkResource->bufferMaxLen)
			{
				if(!bRawData)
				{
					COMM_PACK_HEAD commPackHead;
					memset(commPackHead.headFlag, '1', sizeof(commPackHead.headFlag));
					commPackHead.dataLen = iLen;
					pNetLinkResource->sendBuffer.PushData(&commPackHead, sizeof(COMM_PACK_HEAD));
				}
				pNetLinkResource->sendBuffer.PushData(pBuf, iLen);
				
				m_listLinkSendLock.UnLock();
				return 0;
			}
			else
			{
				if(pNetLinkResource->bufferMaxLen < (long)(iLen + sizeof(COMM_PACK_HEAD)))
				{
					pNetLinkResource->bufferMaxLen = (long)(iLen + sizeof(COMM_PACK_HEAD));
					assert(pNetLinkResource->bufferMaxLen <= pNetLinkResource->sendBuffer.GetTotalSize());
					m_listLinkSendLock.UnLock();
					continue;
				}

				if(bBlock)
				{
					nowTime = time(NULL);
					disTime = nowTime - startTime;

					//BUGS ����������ʱ�⣬��ϵͳʱ��Ŀ�Ҳ���ܳ����������
					if(static_cast<time_t>(SOCK_TIME_OUT) < disTime)
					{
						m_listLinkSendLock.UnLock();
						return -1;
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
				else
				{
					m_listLinkSendLock.UnLock();
					break;
				}
			}
		}
		else //!GetLinkResource
		{
			assert(false);
			m_listLinkSendLock.UnLock();
			return -1;
		}
		m_listLinkSendLock.UnLock();
	}
	
	return -1;
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
int CSWL_MultiNetComm::SendDataSync(long deviceID, const void *pBuf, int iLen, bool bBlock)
{
	assert(m_bSendSync);
	m_listLinkSendLock.Lock();

	int sendLen = -1;

	if(bBlock)
	{
		sendLen = SendBuff(deviceID, pBuf, iLen, true, 100);	//�ٶ�100����ܷ��ͳɹ�
		if((sendLen > 0) && (iLen != sendLen))
		{
			printf("sendLen=%d, iLen=%d\n", sendLen, iLen);
			assert(sendLen == iLen);
		}
	}
	else
	{
		sendLen = SendBuff(deviceID, pBuf, iLen, false, 0);
	}

	NET_LINK_RESOURCE *pNetLinkRes;
	if(!GetLinkResourceByLinkID(deviceID, pNetLinkRes))
	{
		m_listLinkSendLock.UnLock();
		assert(false);
		return -1;
	}

	if(sendLen > 0)
	{
		//�����ϴλỰʱ��
		pNetLinkRes->prevSendTime = GetCurrTime()/1000;
		
		//ָ�����ݴ�С��ȫ��������ϣ����Ϳ��У����Է���������
		if(sendLen == iLen)
		{
			pNetLinkRes->bSendIdle = true;
		}
		else	////����Ѿ����ͳɹ��˲������ݣ���ʱ����Ϊ��æ���������ʱ����������
		{
			pNetLinkRes->bSendIdle = false;
		}
	}
	else
	{
		//�˴����ݷ���ʧ��,������������Ҳ�����ǶϿ�
		pNetLinkRes->bSendIdle = false;
//		assert(false);
	}

	m_listLinkSendLock.UnLock();

	return sendLen;
}

/******************************************************************************
*
*��������ָ�����ȵ�����
*����ֵ��>=0��ʾʵ�ʶ�ȡ�����ֽ�����-1��ʾ�����ѶϿ���-2��ʾ��������
*
******************************************************************************/
int CSWL_MultiNetComm::RecvData(long deviceID, char *pData, int iLen, bool bBlock /* = false */, long lBlockTime /* = 0 */)
{
	m_lstLinkLock.Lock();
	NET_LINK_RESOURCE *pNetLinkResource;
	if(GetLinkResourceByLinkID(deviceID, pNetLinkResource))
	{
		if(pNetLinkResource->bAutoRecv)	//�Զ�����ģʽ�£����������
		{
			m_lstLinkLock.UnLock();
			return 0;
		}
		else
		{
			m_lstLinkLock.UnLock();
			return RecvBuff(deviceID, pData, iLen, bBlock, lBlockTime);
		}
	}
	else
	{
		m_lstLinkLock.UnLock();
		return -2;
	}
}

/******************************************************************************
*
*���ý���ģʽ���Զ����ջ���������
*�ɹ�����0��ʧ�ܷ���-1
*
******************************************************************************/
int CSWL_MultiNetComm::SetRecvMode(long deviceID, bool bAutoRecv)
{
	m_lstLinkLock.Lock();

	NET_LINK_RESOURCE *pNetLinkResource;
	if(GetLinkResourceByLinkID(deviceID, pNetLinkResource))
	{
		pNetLinkResource->bAutoRecv = bAutoRecv;
		m_lstLinkLock.UnLock();
		return 0;
	}

	m_lstLinkLock.UnLock();
	return -1;
}

/******************************************************************************
*
*�����Զ��������ݵĻص�����
*�ɹ�����0��ʧ�ܷ���-1
*
******************************************************************************/
int CSWL_MultiNetComm::SetAutoRecvCallback(long deviceID, RECVASYN_CALLBACK pfRecvCallBack, void *pParam)
{
	m_lstLinkLock.Lock();

	NET_LINK_RESOURCE *pNetLinkResource;
	if(GetLinkResourceByLinkID(deviceID, pNetLinkResource))
	{
		pNetLinkResource->pfRecvCallBack = pfRecvCallBack;
		pNetLinkResource->pParam = pParam;
		m_lstLinkLock.UnLock();
		return 0;
	}
	m_lstLinkLock.UnLock();

	return -1;
}

void CSWL_MultiNetComm::SetBufferMaxLen(long deviceID, long bufferMaxLen)
{
	assert(bufferMaxLen > 0);
	assert(bufferMaxLen <= m_sendBufSize);

	m_lstLinkLock.Lock();

	NET_LINK_RESOURCE *pNetLinkResource;
	if(GetLinkResourceByLinkID(deviceID, pNetLinkResource))
	{
		pNetLinkResource->bufferMaxLen = (bufferMaxLen <= pNetLinkResource->sendBuffer.GetTotalSize()) ? bufferMaxLen : pNetLinkResource->sendBuffer.GetTotalSize();
	}

	m_lstLinkLock.UnLock();
}

void CSWL_MultiNetComm::SendHeartBeat()
{
	assert(m_bSendSync);
	m_listLinkSendLock.Lock();

	COMM_PACK_HEAD commPackHead;
	memset(commPackHead.headFlag, '1', sizeof(commPackHead.headFlag));
	commPackHead.dataLen = 0;

	POS pos = m_lstNetLink.GetHeadPosition();
	while(pos)
	{
		NET_LINK_RESOURCE *pNetLinkResource = m_lstNetLink.GetNext(pos);

		if(pNetLinkResource->bSendIdle && \
			(((GetCurrTime()/1000 - pNetLinkResource->prevSendTime) > ((LONGLONG)MAX_KEEP_TIME/4)) \
			|| ((GetCurrTime()/1000) < pNetLinkResource->prevSendTime)) )
		{
			int sendLen = SendBuff(pNetLinkResource->deviceID, &commPackHead, sizeof(COMM_PACK_HEAD), true, 100);	//�ٶ�100����ܷ��ͳɹ�
			if((sendLen > 0) && (sizeof(COMM_PACK_HEAD) != sendLen))
			{
				printf("sendLen=%d\n", sendLen);
				assert(sizeof(COMM_PACK_HEAD) != sendLen);
			}

			pNetLinkResource->prevSendTime = GetCurrTime()/1000;
		}
	}

	m_listLinkSendLock.UnLock();
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

int CSWL_MultiNetComm::SendBuff(long deviceID, const void *pBuf, int iLen, bool bBlock, long lBlockTime)
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
int CSWL_MultiNetComm::RecvBuff(long deviceID, void *pBuf, int iLen, bool bBlock, long lBlockTime)
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
		}
		else if( SWL_SOCKET_ERROR == iRet)
		{
			//�������ʱû������,�ж��Ƿ��Ѿ���ʱ
			if(SWL_EWOULDBLOCK() && !pNetLinkResource->bBroken)
			{
				nowTime = time(NULL);
				disTime = nowTime - startTime;
				if (disTime >= lBlockTime)	//BUGS ���������ĳ�ʱ�⣬ϵͳ��ʱ��Ŀ�Ҳ���ܵ���
				{
//					pNetLinkResource->bBroken = true;
					return iLen - iLeft;
				}
				else if(disTime >= 0)	//��ֹϵͳʱ��������²�ֵΪ����������������Ҫһ��̫��ʱ����ܳ�ʱ���缸ʮ��
				{
					if(bBlock || bContinue) //������������������˲������ݣ�
					{
						PUB_Sleep(0);
						continue;
					}
					else
					{
						return iLen - iLeft;
					}
				}
				else	//ϵͳ��ʱ����������´�������򵥵Ĵ�����Ϊ���¼�ʱ
				{
					startTime = time(NULL);
				}
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

PUB_THREAD_RESULT PUB_THREAD_CALL CSWL_MultiNetComm::RecvThread(void *pParam)
{
#ifdef __ENVIRONMENT_LINUX__
	pid_t pid = getpid();
	pid_t tid = syscall(__NR_gettid);
	printf("%s, %d, pid = %d, tid=%d\n", __FILE__, __LINE__, pid, tid);
#if defined(__CHIP3535__)
	char szCommond[128] = {0};
	snprintf(szCommond, sizeof(szCommond), "taskset -p 0x2 %d", tid);
	printf("%s:%s:%d, The commond is %s\n", __FUNCTION__, __FILE__, __LINE__, szCommond);
	DVRSystem(szCommond);
	PUB_Sleep(100);
	printf("%s:%s:%d, The commond is %s\n", __FUNCTION__, __FILE__, __LINE__, szCommond);
	DVRSystem(szCommond);
#elif defined(__CHIP3531__)
	BindThreadToCpu(tid, 1);
#endif
#endif

	CSWL_MultiNetComm * pThis = reinterpret_cast<CSWL_MultiNetComm *>(pParam);
	pThis->RecvThreadRun();

	return 0;
}

void CSWL_MultiNetComm::RecvThreadRun()
{
	int ret = 0;
	timeval timeOut;


	fd_set tempReadSet, tempWriteSet;
	LONGLONG llCurTime = 0;
	
	while(m_bSockThreadRun)
	{
		if (m_bNeedDelNetLink)
		{
			UpdateRemoveComm(true);
		}

		tempReadSet = m_fdReadSet;
		tempWriteSet = m_fdWriteSet;

		//����select��ʱʱ��Ϊ50ms
		timeOut.tv_sec = 0;
		timeOut.tv_usec = 50000;

		ret = select(m_maxSockInt+1, &tempReadSet, &tempWriteSet, NULL, &timeOut);
		
		llCurTime = GetCurrTime();
		llCurTime = (llCurTime / 1000);

		if(ret > 0)
		{
//			printf("[");
			//����
			m_lstLinkLock.Lock();

			//�ڴ˴ζ�д�������У������ӽ����˶�д������bProcessedData����Ϊtrue���Ա����Ͻ�����һ�ζ�д���
			bool bProcessedData = false;
			//��ÿ��socket����д���
			POS pos = m_lstNetLink.GetHeadPosition();
			while(pos)
			{
				NET_LINK_RESOURCE *pNetLinkResource = m_lstNetLink.GetAt(pos);

				//�����ӽ��пɶ��Լ��
				if(FD_ISSET(pNetLinkResource->sock, &tempReadSet))
				{
					//��������
					bProcessedData = true;
					
					if(pNetLinkResource->bAutoRecv)
					{
						//��������ͷ���õ�Ҫ���յ����ݳ���
						if(pNetLinkResource->recvBuffer.dataSize == 0)
						{
							COMM_PACK_HEAD commPackHead;
							int ret = RecvBuff(pNetLinkResource->deviceID, &commPackHead, sizeof(COMM_PACK_HEAD), false, 0);
							if(ret > 0)
							{
								pNetLinkResource->prevRecvTime = llCurTime;
								while (ret < sizeof(COMM_PACK_HEAD))
								{
									int temp = RecvBuff(pNetLinkResource->deviceID, \
										((char *)&commPackHead) + ret, sizeof(COMM_PACK_HEAD) - ret, false, 0);
									if(temp >= 0)
									{
										ret += temp;
									}
									else if(temp == -1)
									{
										//�����ѶϿ�
										m_lstNetLink.GetNext(pos);
										delete [] pNetLinkResource->recvBuffer.pData;
										pNetLinkResource->recvBuffer.dataSize = 0;
										pNetLinkResource->recvBuffer.recvSize = 0;
										pNetLinkResource->recvBuffer.pData = NULL;
										pNetLinkResource->bBroken = true;
										RECVASYN_CALLBACK pfRecvCallBack = pNetLinkResource->pfRecvCallBack;
										m_lstLinkLock.UnLock();
										if(pfRecvCallBack != NULL)
										{
											pfRecvCallBack(pNetLinkResource->deviceID, \
												pNetLinkResource->pParam, NULL, -1);
										}
										m_lstLinkLock.Lock();
										break;
									}
								}
								
								if(ret == sizeof(COMM_PACK_HEAD))
								{
									if(memcmp(commPackHead.headFlag, "1111", 4) != 0)	//���������ǹ淶������
									{
										assert(false);
										m_lstNetLink.GetNext(pos);
										continue;
									}
									if(commPackHead.dataLen > 0)
									{
										pNetLinkResource->recvBuffer.pData = new char [commPackHead.dataLen];
										assert(pNetLinkResource->recvBuffer.pData != NULL);
										pNetLinkResource->recvBuffer.dataSize = commPackHead.dataLen;
									}
									else if(commPackHead.dataLen == 0)
									{
										pNetLinkResource->recvBuffer.dataSize = 0;
										pNetLinkResource->recvBuffer.pData = NULL;
//										bProcessedData = false;
									}
									else
									{
										pNetLinkResource->recvBuffer.dataSize = sizeof(NET_COMBINED_DATA);
										pNetLinkResource->recvBuffer.pData = new char [sizeof(NET_COMBINED_DATA)];
										assert(pNetLinkResource->recvBuffer.pData != NULL);
										pNetLinkResource->recvBuffer.dataType = DATA_TYPE_PACK_INFO;
									}
								}
								else	//������һ�����ӵ����ݽ��գ���ǰ�����ѶϿ�����������δ���
								{
									continue;
								}
								
							}
							else if(ret == -1)
							{
								//�����ѶϿ�
								m_lstNetLink.GetNext(pos);
								delete [] pNetLinkResource->recvBuffer.pData;
								pNetLinkResource->recvBuffer.dataSize = 0;
								pNetLinkResource->recvBuffer.recvSize = 0;
								pNetLinkResource->recvBuffer.pData = NULL;
								pNetLinkResource->bBroken = true;
								RECVASYN_CALLBACK pfRecvCallBack = pNetLinkResource->pfRecvCallBack;
								m_lstLinkLock.UnLock();
								if(pfRecvCallBack != NULL)
								{
									pfRecvCallBack(pNetLinkResource->deviceID, pNetLinkResource->pParam, NULL, -1);
								}
								m_lstLinkLock.Lock();
								continue;
							}
						}
						
						//����һ�����ݰ�
						if(pNetLinkResource->recvBuffer.dataSize > 0)
						{
							if(pNetLinkResource->recvBuffer.recvSize < pNetLinkResource->recvBuffer.dataSize)
							{
								int ret = RecvBuff(pNetLinkResource->deviceID, \
									pNetLinkResource->recvBuffer.pData + pNetLinkResource->recvBuffer.recvSize, \
									pNetLinkResource->recvBuffer.dataSize - pNetLinkResource->recvBuffer.recvSize, false, 0);
								if(ret > 0)
								{
									pNetLinkResource->recvBuffer.recvSize += ret;
								}
								else if(ret == -1)
								{
									//�����ѶϿ�
									m_lstNetLink.GetNext(pos);
									delete [] pNetLinkResource->recvBuffer.pData;
									pNetLinkResource->recvBuffer.dataSize = 0;
									pNetLinkResource->recvBuffer.recvSize = 0;
									pNetLinkResource->recvBuffer.pData = NULL;
									pNetLinkResource->bBroken = true;
									RECVASYN_CALLBACK pfRecvCallBack = pNetLinkResource->pfRecvCallBack;
									m_lstLinkLock.UnLock();
									if (pfRecvCallBack)
									{
										pfRecvCallBack(pNetLinkResource->deviceID, pNetLinkResource->pParam, NULL, -1);
									}
									
									m_lstLinkLock.Lock();
									continue;
								}
							}
							
							if(pNetLinkResource->recvBuffer.recvSize == pNetLinkResource->recvBuffer.dataSize)
							{
								if(memcmp(pNetLinkResource->recvBuffer.pData, "1111", 4) == 0)
								{
									char *pTempData = pNetLinkResource->recvBuffer.pData+sizeof(COMM_PACK_HEAD);
									COMM_PACK_HEAD *pPackHead = (COMM_PACK_HEAD *)pNetLinkResource->recvBuffer.pData;
									unsigned long payloadLen = pNetLinkResource->recvBuffer.recvSize - sizeof(COMM_PACK_HEAD);

									if(pPackHead->dataLen > payloadLen)
									{
										pNetLinkResource->recvBuffer.pData = new char [pPackHead->dataLen];
										memmove(pNetLinkResource->recvBuffer.pData, pTempData, payloadLen);
										pNetLinkResource->recvBuffer.recvSize = payloadLen;
										pNetLinkResource->recvBuffer.dataSize = pPackHead->dataLen;
										delete [] (pTempData - sizeof(COMM_PACK_HEAD));
									}
									else
									{
										//���ݻص���ȥ
										RECVASYN_CALLBACK pfRecvCallBack = pNetLinkResource->pfRecvCallBack;
										m_lstLinkLock.UnLock();
										int callBackRet = 0;
										if(pfRecvCallBack != NULL)
										{
											pfRecvCallBack(pNetLinkResource->deviceID, \
												pNetLinkResource->pParam, \
												pTempData, \
												pPackHead->dataLen);
										}
										m_lstLinkLock.Lock();
										if(callBackRet == 0)	//���ݴ���ɹ�
										{
											POS posTemp = m_lstNetLink.GetHeadPosition();
											while(posTemp)
											{
												if(m_lstNetLink.GetNext(posTemp) == pNetLinkResource)
												{
													if(payloadLen == pPackHead->dataLen)
													{
														delete [] pNetLinkResource->recvBuffer.pData;
														pNetLinkResource->recvBuffer.dataSize = 0;
														pNetLinkResource->recvBuffer.recvSize = 0;
														pNetLinkResource->recvBuffer.pData = NULL;
													}
													else
													{
														pNetLinkResource->recvBuffer.dataSize -= sizeof(COMM_PACK_HEAD);
														pNetLinkResource->recvBuffer.dataSize -= pPackHead->dataLen;
														pNetLinkResource->recvBuffer.recvSize = pNetLinkResource->recvBuffer.dataSize;
														memmove(pNetLinkResource->recvBuffer.pData, pTempData+pPackHead->dataLen, \
															pNetLinkResource->recvBuffer.dataSize);
														assert(memcmp(pNetLinkResource->recvBuffer.pData, "1111", 4) == 0);
													}
													break;
												}
											}//end while
										}//end if callback succ
									}
									
									assert(false);
									continue;
								}

								if(pNetLinkResource->recvBuffer.dataType == DATA_TYPE_PACK_INFO)
								{
									NET_COMBINED_DATA *pCombinedData = (NET_COMBINED_DATA *)pNetLinkResource->recvBuffer.pData;
									pNetLinkResource->recvBuffer.dataType = DATA_TYPE_TRANS;
									pNetLinkResource->recvBuffer.dataSize = pCombinedData->dwLen \
										+ sizeof(NET_COMBINED_DATA) + sizeof(long);
									pNetLinkResource->recvBuffer.pData = new char [pNetLinkResource->recvBuffer.dataSize];
									assert(pNetLinkResource->recvBuffer.pData != NULL);
									memcpy(pNetLinkResource->recvBuffer.pData, &PACKED_DATA_FLAG, sizeof(long));
									memcpy(pNetLinkResource->recvBuffer.pData+sizeof(long), pCombinedData, sizeof(NET_COMBINED_DATA));
									pNetLinkResource->recvBuffer.recvSize = sizeof(long) + sizeof(NET_COMBINED_DATA);
									delete [] (char *)pCombinedData;
								}
								else
								{
									//���ݻص���ȥ
									RECVASYN_CALLBACK pfRecvCallBack = pNetLinkResource->pfRecvCallBack;
									m_lstLinkLock.UnLock();
									int callBackRet = 0;
									if(pfRecvCallBack != NULL)
									{
										pfRecvCallBack(pNetLinkResource->deviceID, \
											pNetLinkResource->pParam, \
											pNetLinkResource->recvBuffer.pData, \
											pNetLinkResource->recvBuffer.dataSize);
									}
									m_lstLinkLock.Lock();
									if(callBackRet == 0)	//���ݴ���ɹ�
									{
										POS posTemp = m_lstNetLink.GetHeadPosition();
										while(posTemp)
										{
											if(m_lstNetLink.GetNext(posTemp) == pNetLinkResource)
											{
												delete [] pNetLinkResource->recvBuffer.pData;
												pNetLinkResource->recvBuffer.dataSize = 0;
												pNetLinkResource->recvBuffer.recvSize = 0;
												pNetLinkResource->recvBuffer.pData = NULL;
												break;
											}
										}
									}
								}
							}
						}
					}
					else
					{
						//������������
					}
				}

				if(FD_ISSET(pNetLinkResource->sock, &tempWriteSet) && !m_bSendSync)
				{
					//��������
					char *pData = pNetLinkResource->sendBuffer.GetDataBuffer(0);
					char *pFree = pNetLinkResource->sendBuffer.GetFreeBuffer(0);
					assert(NULL != pData);
					assert(NULL != pFree);
					if(pData > pFree)
					{
						int sendLen = 0;
						//��ʱ�����ڻ���ص����ˣ�����Ҫ�����η���
						int dataLen = pNetLinkResource->sendBuffer.GetBufferTail() - pData;
						if(dataLen > 0)
						{
							sendLen = SendBuff(pNetLinkResource->deviceID, pData, dataLen, false, 0);
							if(sendLen > 0)
							{
								pNetLinkResource->sendBuffer.FreeBuffer(sendLen);
							}
							else if(sendLen == -1)
							{
								m_lstNetLink.GetNext(pos);
								delete [] pNetLinkResource->recvBuffer.pData;
								pNetLinkResource->recvBuffer.dataSize = 0;
								pNetLinkResource->recvBuffer.recvSize = 0;
								pNetLinkResource->recvBuffer.pData = NULL;
								pNetLinkResource->bBroken = true;
								RECVASYN_CALLBACK pfRecvCallBack = pNetLinkResource->pfRecvCallBack;
								m_lstLinkLock.UnLock();
								if(pfRecvCallBack != NULL)
								{
									pfRecvCallBack(pNetLinkResource->deviceID, pNetLinkResource->pParam, NULL, -1);
								}
								m_lstLinkLock.Lock();
								continue;
							}
						}

						if(sendLen == dataLen)
						{
							////���β��������һ�η����з�����ɣ�����ŷ���ͷ������
							pData = pNetLinkResource->sendBuffer.GetDataBuffer(0);

							//��ʱ����λ��Ӧ���ǻ���ؿ�ʼλ��
							assert(pData == pNetLinkResource->sendBuffer.GetBufferHead());
							dataLen = pNetLinkResource->sendBuffer.GetDataSize();

							if(dataLen > 0)
							{
								sendLen = SendBuff(pNetLinkResource->deviceID, pData, dataLen, false, 0);
								if(sendLen > 0)
								{
									pNetLinkResource->sendBuffer.FreeBuffer(sendLen);
								}
								else if(sendLen == -1)
								{
									m_lstNetLink.GetNext(pos);
									delete [] pNetLinkResource->recvBuffer.pData;
									pNetLinkResource->recvBuffer.dataSize = 0;
									pNetLinkResource->recvBuffer.recvSize = 0;
									pNetLinkResource->recvBuffer.pData = NULL;
									pNetLinkResource->bBroken = true;
									RECVASYN_CALLBACK pfRecvCallBack = pNetLinkResource->pfRecvCallBack;
									m_lstLinkLock.UnLock();
									if(pfRecvCallBack != NULL)
									{
										pfRecvCallBack(pNetLinkResource->deviceID, pNetLinkResource->pParam, NULL, -1);
									}
									m_lstLinkLock.Lock();
									continue;
								}
							}
						}

						bProcessedData = true;
						pNetLinkResource->prevSendTime = llCurTime;
					}
					else if(pFree > pData)
					{
						//����λ���м�λ�ã�һ�η��ͼ������
						int sendLen = SendBuff(pNetLinkResource->deviceID, pData, pFree-pData, false, 0);
						if(sendLen > 0)
						{
							pNetLinkResource->sendBuffer.FreeBuffer(sendLen);
						}
						else if(sendLen == -1)
						{
							m_lstNetLink.GetNext(pos);
							delete [] pNetLinkResource->recvBuffer.pData;
							pNetLinkResource->recvBuffer.dataSize = 0;
							pNetLinkResource->recvBuffer.recvSize = 0;
							pNetLinkResource->recvBuffer.pData = NULL;
							pNetLinkResource->bBroken = true;
							RECVASYN_CALLBACK pfRecvCallBack = pNetLinkResource->pfRecvCallBack;
							m_lstLinkLock.UnLock();
							if(pfRecvCallBack != NULL)
							{
								pfRecvCallBack(pNetLinkResource->deviceID, pNetLinkResource->pParam, NULL, -1);
							}
							m_lstLinkLock.Lock();
							continue;
						}

						bProcessedData = true;
						pNetLinkResource->prevSendTime = GetCurrTime()/1000;
					}
					else
					{
						if( pNetLinkResource->bSendIdle && \
							(((llCurTime - pNetLinkResource->prevSendTime) > ((LONGLONG)MAX_KEEP_TIME/4)) \
							|| (llCurTime < pNetLinkResource->prevSendTime)) )	//ʱ���С
						{
							//û�����ݷ���,�ͷ�һ���������ӵ�������Ϣ
							COMM_PACK_HEAD commPackHead;
							memset(commPackHead.headFlag, '1', sizeof(commPackHead.headFlag));
							commPackHead.dataLen = 0;
							int sendLen = SendBuff(pNetLinkResource->deviceID, &commPackHead, sizeof(COMM_PACK_HEAD), false, 0);
							if(0 < sendLen)
							{
								while(sendLen < sizeof(COMM_PACK_HEAD))
								{
									int newSendLen =  SendBuff(pNetLinkResource->deviceID, \
										(unsigned char *)&commPackHead + sendLen, sizeof(COMM_PACK_HEAD) - sendLen, false, 0);
									if(newSendLen < 0)
									{
										m_lstNetLink.GetNext(pos);
										delete [] pNetLinkResource->recvBuffer.pData;
										pNetLinkResource->recvBuffer.dataSize = 0;
										pNetLinkResource->recvBuffer.recvSize = 0;
										pNetLinkResource->recvBuffer.pData = NULL;
										pNetLinkResource->bBroken = true;
										RECVASYN_CALLBACK pfRecvCallBack = pNetLinkResource->pfRecvCallBack;
										m_lstLinkLock.UnLock();
										if(pfRecvCallBack != NULL)
										{
											pfRecvCallBack(pNetLinkResource->deviceID, pNetLinkResource->pParam, NULL, -1);
										}
										m_lstLinkLock.Lock();
										break;
									}
									else if(newSendLen == 0)
									{
										assert(false);
									}
									else
									{
										sendLen += newSendLen;
									}
								}
								if(sendLen < sizeof(COMM_PACK_HEAD))
								{
									assert(false);
									continue;
								}

								pNetLinkResource->prevSendTime = llCurTime;
							}//������������Ϣ
						}
						else
						{
							bProcessedData = false;
						}
					}
				}
				
				if((llCurTime - pNetLinkResource->prevRecvTime) > (LONGLONG)MAX_KEEP_TIME*2)	//��Ϊʱ�䱻�޸�
				{
					pNetLinkResource->prevRecvTime = llCurTime;
				}
				else if((llCurTime - pNetLinkResource->prevRecvTime) > (LONGLONG)MAX_KEEP_TIME)
				{
					//�豸�Ͽ�
					m_lstNetLink.GetNext(pos);
					delete [] pNetLinkResource->recvBuffer.pData;
					pNetLinkResource->recvBuffer.dataSize = 0;
					pNetLinkResource->recvBuffer.recvSize = 0;
					pNetLinkResource->recvBuffer.pData = NULL;
					pNetLinkResource->bBroken = true;
					RECVASYN_CALLBACK pfRecvCallBack = pNetLinkResource->pfRecvCallBack;
					m_lstLinkLock.UnLock();
					if(pfRecvCallBack != NULL)
					{
						pfRecvCallBack(pNetLinkResource->deviceID, pNetLinkResource->pParam, NULL, -1);
					}
					m_lstLinkLock.Lock();
					continue;
				}
				
				m_lstNetLink.GetNext(pos);
			}
			m_lstLinkLock.UnLock();

			if(!bProcessedData)
			{
			//��WIN�£���Ҫ���ܴ������ݣ������ߣ���LINUX�ϣ����������ٶ�����
#if defined(__ENVIRONMENT_WIN32__)
				//PUB_Sleep(3);
#else
				PUB_Sleep(20);
#endif
			}
		}
		else	//û�����ݿɶ�д(selectʧ��)
		{
			m_lstLinkLock.Lock();

			//��ÿ��socket�����ж��Ƿ�ʱ
			POS pos = m_lstNetLink.GetHeadPosition();
			while(pos)
			{
				NET_LINK_RESOURCE *pNetLinkResource = m_lstNetLink.GetNext(pos);
				ULONGLONG llCurrentTime = llCurTime;
				if((llCurrentTime - pNetLinkResource->prevRecvTime) > (LONGLONG)MAX_KEEP_TIME*2)	//��Ϊʱ�䱻�޸�
				{
					pNetLinkResource->prevRecvTime = llCurrentTime;
				}
				else if((llCurrentTime - pNetLinkResource->prevRecvTime) > (LONGLONG)MAX_KEEP_TIME)
				{
					//�豸�Ͽ�
					delete [] pNetLinkResource->recvBuffer.pData;
					pNetLinkResource->recvBuffer.dataSize = 0;
					pNetLinkResource->recvBuffer.recvSize = 0;
					pNetLinkResource->recvBuffer.pData = NULL;
					pNetLinkResource->bBroken = true;
					RECVASYN_CALLBACK pfRecvCallBack = pNetLinkResource->pfRecvCallBack;
					m_lstLinkLock.UnLock();
					if(pfRecvCallBack != NULL)
					{
						pfRecvCallBack(pNetLinkResource->deviceID, pNetLinkResource->pParam, NULL, -1);
					}
					m_lstLinkLock.Lock();
				}
			}
			m_lstLinkLock.UnLock();
			
			if (0 != ret)
			{
				//printf("%s:%s:%d, Running at here\n", __FUNCTION__, __FILE__, __LINE__);
			}

			PUB_Sleep(20);
		}
	}
}

bool CSWL_MultiNetComm::GetLinkResourceByLinkID(long deviceID, NET_LINK_RESOURCE * &pLinkResource)
{
	POS pos = m_lstNetLink.GetHeadPosition();
	while(pos)
	{
		NET_LINK_RESOURCE *pNetLinkResource = m_lstNetLink.GetNext(pos);
		if(pNetLinkResource->deviceID == deviceID)
		{
			pLinkResource = pNetLinkResource;
			return true;
		}
	}

	return false;
}

void CSWL_MultiNetComm::UpdateRemoveComm(bool bClearBar/*=false*/)
{
	m_lstLinkLock.Lock();
	m_listLinkSendLock.Lock();

	while(m_tempDelNetLink.GetCount() > 0)
	{
		NET_LINK_RESOURCE *pNetLinkResource = m_tempDelNetLink.RemoveHead();

		POS pos = m_lstNetLink.GetHeadPosition();
		while(pos)
		{
			if(m_lstNetLink.GetAt(pos) == pNetLinkResource)
			{
				m_lstNetLink.RemoveAt(pos);
				pNetLinkResource->sendBuffer.Destroy();
				FD_CLR(pNetLinkResource->sock, &m_fdReadSet);
				FD_CLR(pNetLinkResource->sock, &m_fdWriteSet);
				SWL_CloseSocket(pNetLinkResource->sock);
				delete pNetLinkResource;
				break;
			}
			m_lstNetLink.GetNext(pos);
		}
	}
	
	if (bClearBar)
	{
		m_bNeedDelNetLink = false;
	}
	m_listLinkSendLock.UnLock();
	m_lstLinkLock.UnLock();
}

