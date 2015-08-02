// SWL_MultiNetComm.cpp: implementation of the CSWL_MultiNet class.
//
//////////////////////////////////////////////////////////////////////
#include "Typedef.h"
#ifdef __ENVIRONMENT_LINUX__
#include "lvfw.h"
#else
#include <vfw.h>
#endif
#include "SWL_MultiNet.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSWL_MultiNet::CSWL_MultiNet()
{
// 	m_maxSockInt		= 0;
// 	m_bSockThreadRun	= false;
// 	m_threadId			= PUB_CREATE_THREAD_FAIL;
// 	FD_ZERO(&m_fdReadSet);
// 	FD_ZERO(&m_fdWriteSet);
// 
// 	m_bSendSync			= false;
}

CSWL_MultiNet::~CSWL_MultiNet()
{
}

bool CSWL_MultiNet::Init(long sendBufSize, bool bSendSync /* = false */)
{
// 	assert(sendBufSize > 0);
// 	m_sendBufSize = sendBufSize;
// 	m_bSendSync = bSendSync;
// 
// 	m_threadId = PUB_CreateThread(RecvThread, this, &m_bSockThreadRun);
// 	if(m_threadId == PUB_CREATE_THREAD_FAIL)
// 	{
// 		assert(false);
// 		return false;
// 	}
// 
 	return true;
}

void CSWL_MultiNet::Destroy()
{
// 	if(m_threadId != PUB_CREATE_THREAD_FAIL)
// 	{
// 		PUB_ExitThread(&m_threadId, &m_bSockThreadRun);
// 	}
}
/******************************************************************************
*
*
*
******************************************************************************/
int CSWL_MultiNet::AddNewComm(long deviceID, SWL_socket_t sock)
{
	return 0;
}

/******************************************************************************
*
*
*
******************************************************************************/
int CSWL_MultiNet::RemoveComm(long deviceID)
{
	return 0;
}

int CSWL_MultiNet::RemoveAllConnect()
{
	return 0;
}
/******************************************************************************
*
*	采用异步发送数据的方式，通过此接口把要发送的数据暂存入发送数据缓冲池中，由发
	送线程从此缓冲池中不断地取数据发送
*
*	返回值：
*	当缓冲池中的空闲空间不足以放下要发送的数据时，返回-1，否则返回0
*
*	参数：bRawData: 为true表示发送不加任何头部信息的原始数据，否则会有一个COMM_PACK_HEAD头信息
*		  bBlock: 如果设置为true，即此接口为阻塞模式，则会等待缓冲池中的空闲空间足以放下
		  待发送数据为止
*
******************************************************************************/
int CSWL_MultiNet::SendData(long deviceID, const void *pBuf, int iLen, bool bBlock, bool bRawData)
{
	return -1;
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
int CSWL_MultiNet::SendDataSync(long deviceID, const void *pBuf, int iLen, bool bBlock)
{
	return -1;
}

/******************************************************************************
*
*主动接收指定长度的数据
*返回值：>=0表示实际读取到的字节数，-1表示连接已断开，-2表示其他错误
*
******************************************************************************/
int CSWL_MultiNet::RecvData(long deviceID, char *pData, int iLen, bool bBlock /* = false */, long lBlockTime /* = 0 */)
{
	return 0;
}

/******************************************************************************
*
*设置接收模式，自动接收或主动接收
*成功返回0，失败返回-1
*
******************************************************************************/
int CSWL_MultiNet::SetRecvMode(long deviceID, bool bAutoRecv)
{
	return -1;
}

/******************************************************************************
*
*设置自动接收数据的回调函数
*成功返回0，失败返回-1
*
******************************************************************************/
// int CSWL_MultiNet::SetAutoRecvCallback(long deviceID, RECVASYN_CALLBACK pfRecvCallBack, void *pParam)
// {
// // 	m_lstLinkLock.Lock();
// // 
// // 	NET_LINK_RESOURCE *pNetLinkResource;
// // 	if(GetLinkResourceByLinkID(deviceID, pNetLinkResource))
// // 	{
// // 		pNetLinkResource->pfRecvCallBack = pfRecvCallBack;
// // 		pNetLinkResource->pParam = pParam;
// // 		m_lstLinkLock.UnLock();
// // 		return 0;
// // 	}
// // 	m_lstLinkLock.UnLock();
// 
// 	return -1;
// }

void CSWL_MultiNet::SetBufferMaxLen(long deviceID, long bufferMaxLen)
{
	//assert(bufferMaxLen > 0);
	//assert(bufferMaxLen <= m_sendBufSize);

	//m_lstLinkLock.Lock();

	//NET_LINK_RESOURCE *pNetLinkResource;
	//if(GetLinkResourceByLinkID(deviceID, pNetLinkResource))
	//{
	//	pNetLinkResource->bufferMaxLen = (bufferMaxLen <= pNetLinkResource->sendBuffer.GetTotalSize()) ? bufferMaxLen : pNetLinkResource->sendBuffer.GetTotalSize();
	//}

	//m_lstLinkLock.UnLock();
}

void CSWL_MultiNet::SendHeartBeat()
{
	//assert(m_bSendSync);
	//m_listLinkSendLock.Lock();

	//COMM_PACK_HEAD commPackHead;
	//memset(commPackHead.headFlag, '1', sizeof(commPackHead.headFlag));
	//commPackHead.dataLen = 0;

	//POS pos = m_lstNetLink.GetHeadPosition();
	//while(pos)
	//{
	//	NET_LINK_RESOURCE *pNetLinkResource = m_lstNetLink.GetNext(pos);

	//	if(pNetLinkResource->bSendIdle && \
	//		(((GetCurrTime()/1000 - pNetLinkResource->prevSendTime) > ((LONGLONG)MAX_KEEP_TIME/4)) \
	//		|| ((GetCurrTime()/1000) < pNetLinkResource->prevSendTime)) )
	//	{
	//		int sendLen = SendBuff(pNetLinkResource->deviceID, &commPackHead, sizeof(COMM_PACK_HEAD), true, 100);	//假定100秒会能发送成功
	//		if((sendLen > 0) && (sizeof(COMM_PACK_HEAD) != sendLen))
	//		{
	//			printf("sendLen=%d\n", sendLen);
	//			assert(sizeof(COMM_PACK_HEAD) != sendLen);
	//		}

	//		pNetLinkResource->prevSendTime = GetCurrTime()/1000;
	//	}
	//}

	//m_listLinkSendLock.UnLock();
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

int CSWL_MultiNet::SendBuff(long deviceID, const void *pBuf, int iLen, bool bBlock, long lBlockTime)
{
	return 0;
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
int CSWL_MultiNet::RecvBuff(long deviceID, void *pBuf, int iLen, bool bBlock, long lBlockTime)
{
	return 0;
}

//PUB_THREAD_RESULT PUB_THREAD_CALL CSWL_MultiNet::RecvThread(void *pParam)
//{
//#ifdef __ENVIRONMENT_LINUX__
//	pid_t pid = getpid();
//	pid_t tid = syscall(__NR_gettid);
//	printf("%s, %d, pid = %d, tid=%d\n", __FILE__, __LINE__, pid, tid);
//#endif
//
//	CSWL_MultiNet * pThis = reinterpret_cast<CSWL_MultiNet *>(pParam);
//	pThis->RecvThreadRun();
//
//	return 0;
//}

void CSWL_MultiNet::RecvThreadRun()
{
}

// bool CSWL_MultiNet::GetLinkResourceByLinkID(long deviceID, NET_LINK_RESOURCE * &pLinkResource)
// {
// 	POS pos = m_lstNetLink.GetHeadPosition();
// 	while(pos)
// 	{
// 		NET_LINK_RESOURCE *pNetLinkResource = m_lstNetLink.GetNext(pos);
// 		if(pNetLinkResource->deviceID == deviceID)
// 		{
// 			pLinkResource = pNetLinkResource;
// 			return true;
// 		}
// 	}
// 
// 	return false;
// }

void CSWL_MultiNet::UpdateRemoveComm()
{
}

