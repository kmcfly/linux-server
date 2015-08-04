// SWL_MultiNetComm.h: interface for the CTS_MultiNetComm class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _SWL_MULTI_NET_COMM_H_
#define _SWL_MULTI_NET_COMM_H_

#include "SWL_Public.h"
#include "DDPublic.h"
#include <string.h>

typedef struct _recv_data_buffer_
{
	long			bufferSize;	//缓冲区容量
	long			dataSize;	//缓冲区中数据大小
	char			*pData;		//接收数据的缓冲区
}RECV_DATA_BUFFER;

//数据处理成功返回0，否则返回-1，此时会进入阻塞状态
typedef int (*RECVASYN_CALLBACK)(long clientID, void* pParam, RECV_DATA_BUFFER *pDataBuffer);
//网络连接资源结构体
typedef struct _net_link_resource_
{
	SWL_socket_t		sock;
	long				deviceID;
	long				bufferMaxLen;		//根据请求数据的通道数动态指定的最大可用缓冲大小
	RECV_DATA_BUFFER	recvBuffer;			//要接收的数据
	RECVASYN_CALLBACK	pfRecvCallBack;		//接收数据的回调函数
	void*				pParam;				//回调函数参数
	bool				bBroken;			//是否已断开
}NET_LINK_RESOURCE;

class CTS_MultiNetComm  
{
public:
	CTS_MultiNetComm();
	virtual ~CTS_MultiNetComm();

public:
	bool Init();

	void Destroy();
	//增加新的网络连接
	int AddComm(long deviceID, SWL_socket_t sock);

	//删除指定ID的网络连接
	int RemoveComm(long deviceID);

	//删除所有的网络连接
	int RemoveAllConnect();

	int SendData(long deviceID, const void *pBuf, int iLen, bool bBlock);

	//设置自动接收数据的回调函数
	int SetAutoRecvCallback(long deviceID, RECVASYN_CALLBACK pfRecvCallBack, void *pParam);

	//设置指定连接的接收缓冲区大小
	void SetRecvBufferLen(long deviceID, long bufferLen);

private:
	int SendBuff(long deviceID, const void *pBuf, int iLen, bool bBlock, long lBlockTime);
	int RecvBuff(NET_LINK_RESOURCE *pNetLinkResource);
	static bool BindThreadToCpu(int pid, int whichCpu);
	static PUB_THREAD_RESULT PUB_THREAD_CALL RecvThread(void *pParam);
	void RecvThreadRun();

	bool GetLinkResourceByLinkID(long deviceID, NET_LINK_RESOURCE * &pLinkResource);

	void UpdateRemoveComm(bool bClearBar=false);

private:
	list<NET_LINK_RESOURCE *>		m_listNetLink;
	list<NET_LINK_RESOURCE *>		m_tempDelNetLink;
	bool							m_bNeedDelNetLink;
	CPUB_Lock						m_lstLinkLock;

	int								m_maxSockInt;
	bool							m_bSockThreadRun;
	fd_set							m_fdReadSet;
	PUB_thread_t					m_hRecvThread;
	int								m_recvThreadId;
};

#endif
