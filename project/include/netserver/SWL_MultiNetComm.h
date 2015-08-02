// SWL_MultiNetComm.h: interface for the CSWL_MultiNetComm class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SWL_MULTINETCOMM_H__9E723A1A_97DE_445E_80BD_820A85DC30B6__INCLUDED_)
#define AFX_SWL_MULTINETCOMM_H__9E723A1A_97DE_445E_80BD_820A85DC30B6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SWL_MultiNet.h"


class CSWL_MultiNetComm : public CSWL_MultiNet
{
public:
	CSWL_MultiNetComm();
	~CSWL_MultiNetComm();

public:
	bool Init(long sendBufSize, bool bSendSync = false);

	void Destroy();
	//增加新的网络连接
	int AddNewComm(long deviceID, SWL_socket_t sock);

	//删除指定ID的网络连接
	int RemoveComm(long deviceID);

	//删除所有的网络连接
	int RemoveAllConnect();

	//发送数据，两个函数组合使用
	int SendData(long deviceID, const void *pBuf, int iLen, bool bBlock, bool bRawData = false);

	int SendDataSync(long deviceID, const void *pBuf, int iLen, bool bBlock);

	//外部主动调用此接口接收数据
	int RecvData(long deviceID, char *pData, int iLen, bool bBlock = false, long lBlockTime = 0);

	//设置数据接收模式，是自动接收还是外部主动接收
	int SetRecvMode(long deviceID, bool bAutoRecv);

	//设置自动接收数据的回调函数
	int SetAutoRecvCallback(long deviceID, RECVASYN_CALLBACK pfRecvCallBack, void *pParam);

	void SetBufferMaxLen(long deviceID, long bufferMaxLen);

	//这个函数并不是每次都实在地发送心跳数据，而是首先判断是不是很久没有数据发出了，如果是，则发送一个心跳数据
	void SendHeartBeat();

private:
	int SendBuff(long deviceID, const void *pBuf, int iLen, bool bBlock, long lBlockTime);
	int RecvBuff(long deviceID, void *pBuf, int iLen, bool bBlock, long lBlockTime);
	static PUB_THREAD_RESULT PUB_THREAD_CALL RecvThread(void *pParam);
	void RecvThreadRun();

	bool GetLinkResourceByLinkID(long deviceID, NET_LINK_RESOURCE * &pLinkResource);

 	void UpdateRemoveComm(bool bClearBar=false);
 
private:
 	CMyList<NET_LINK_RESOURCE *>			m_lstNetLink;
 	CMyList<NET_LINK_RESOURCE *>			m_tempDelNetLink;
 	bool									m_bNeedDelNetLink;
 	CPUB_Lock								m_lstLinkLock;
 	CPUB_Lock								m_listLinkSendLock;
 	
 	int										m_maxSockInt;
 	bool									m_bSockThreadRun;
 	fd_set									m_fdReadSet;
 	fd_set									m_fdWriteSet;
 	PUB_thread_t							m_threadId;
 
 	long									m_sendBufSize;
 
 	bool									m_bSendSync;	//是否为同步发送网络数据,false：异步，true：同步
 															//如果为异步发送数据，需要调用SendData，否则调用SendDataSync
};

#endif // !defined(AFX_SWL_MULTINETCOMM_H__9E723A1A_97DE_445E_80BD_820A85DC30B6__INCLUDED_)
