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
int CSWL_MultiNet::SendData(long deviceID, const void *pBuf, int iLen, bool bBlock, bool bRawData)
{
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
int CSWL_MultiNet::SendDataSync(long deviceID, const void *pBuf, int iLen, bool bBlock)
{
	return -1;
}

/******************************************************************************
*
*��������ָ�����ȵ�����
*����ֵ��>=0��ʾʵ�ʶ�ȡ�����ֽ�����-1��ʾ�����ѶϿ���-2��ʾ��������
*
******************************************************************************/
int CSWL_MultiNet::RecvData(long deviceID, char *pData, int iLen, bool bBlock /* = false */, long lBlockTime /* = 0 */)
{
	return 0;
}

/******************************************************************************
*
*���ý���ģʽ���Զ����ջ���������
*�ɹ�����0��ʧ�ܷ���-1
*
******************************************************************************/
int CSWL_MultiNet::SetRecvMode(long deviceID, bool bAutoRecv)
{
	return -1;
}

/******************************************************************************
*
*�����Զ��������ݵĻص�����
*�ɹ�����0��ʧ�ܷ���-1
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
	//		int sendLen = SendBuff(pNetLinkResource->deviceID, &commPackHead, sizeof(COMM_PACK_HEAD), true, 100);	//�ٶ�100����ܷ��ͳɹ�
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
*���ܣ�ִ��һ���������ݷ���
*����˵����bBlock true���� false����������ʱdwBlockTime����Ч��
		   dwBlockTime ����Ϊ��λ����
*����ֵ��>=0���͵��ֽ���
		 -1��ʾ�Է��Ͽ�socket
		 -2��������
*
******************************************************************************/

int CSWL_MultiNet::SendBuff(long deviceID, const void *pBuf, int iLen, bool bBlock, long lBlockTime)
{
	return 0;
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

