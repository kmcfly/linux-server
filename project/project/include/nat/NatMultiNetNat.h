// SWL_MultiNetComm.h: interface for the CSWL_MultiNetNat class.
//
//////////////////////////////////////////////////////////////////////

#ifndef  __NAT_MULTI_NET_MAT__
#define __NAT_MULTI_NET_MAT__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "SWL_Public.h"
#include "mylist.cpp"
#include "PUB_common.h"
#include "BufferPool.h"
#include "NatSocket.h"

#include "SWL_MultiNet.h"

#define		NAT_MAX_KEEP_TIME		60000
// 
// //#define		SWL_DATA_TYPE_LIVE		0
// //#define		SWL_DATA_TYPE_PLAY		1
// 
// const long	SOCK_TIME_OUT = 10000;		//发送接收超时时间
// 
// //"PACK"分包数据的头部标识
// const long PACKED_DATA_FLAG = mmioFOURCC('P', 'A', 'C', 'K');
// 
// const unsigned long SWL_HEADFLAG = mmioFOURCC('h', 'e', 'a', 'd');
// //连接预发送数据，用于判断是否是我们的设备
// typedef struct _pre_send_data
// {
// 	unsigned long		dwFlag;			//预发送数据标识，总为HEADFLAG
// 	unsigned long		dwDevType;		//设备类型
// 	unsigned long		dwProductType;	//产品类型
// 	unsigned long		dwProtocolVer;	//协议版本
// 	unsigned long		dwConfigVer;	//配置版本
// 	unsigned long       dwID;           //
// 	unsigned long		dwReserved[10];	//保留字节
// }PRE_SEND_DATA;
// 
// //当为SWL_COMBINATION_DATA数据时会跟结构体
// typedef struct swl_combined_data
// {
// 	unsigned	long	dwDataID;		//本组合数据的ID，组合数据里的每个包的数据ID相同
// 	unsigned	long	dwTotalPack;	//一共有几个包
// 	unsigned	long	dwTotalLen;		//所有的有效数据的总长度
// 	unsigned	long	dwIndex;		//第几个包，基数从1开始
// 	unsigned	long	dwLen;			//本包长度
// 	//SWL_DATA_TYPE_LIVE  	 SWL_DATA_TYPE_PLAY
// 	unsigned	long	dwPadding;		//为兼容上一版本的网络传输模块附加
// }NET_COMBINED_DATA;
// 
// typedef struct swl_combined_info
// {
// 	unsigned	long	dwUsed;			//是否正被使用	0 没有用 1 使用了
// 	unsigned	long	dwDataID;		//本组合数据的ID，组合数据里的每个包的数据ID相同
// 	unsigned	long	dwTotalPack;	//一共有几个包
// 	unsigned	long	dwTotalLen;		//所有的有效数据的总长度
// 	unsigned	long	dwLen;			//已经接收/发送了多少长度
// 	unsigned	char	*pValidData;	//只在接收的时候有用
// }NET_COMBINED_INFO;
// 
// //发送缓冲区大小
// //const long SEND_BUFFER_SIZE = 1536*1024;
// 
// //数据处理成功返回0，否则返回-1，此时会进入阻塞状态
// typedef int (*RECVASYN_CALLBACK)(long clientID, void* pParam, const char* pBuf, int iLen);
// 
// typedef struct _comm_pack_head_
// {
// 	char		headFlag[4];
// 	long		dataLen;
// }COMM_PACK_HEAD;
// 
// enum RECV_DATA_TYPE
// {
// 	DATA_TYPE_TRANS			= 0x0,	//透明数据
// 	DATA_TYPE_PACK_INFO		= 0x1
// };
// typedef struct _recv_data_buffer_
// {
// 	long			dataSize;	//要接收的总的数据大小
// 	char			*pData;		//接收数据的缓冲区
// 	long			recvSize;	//已经接收的数据
// 	long			dataType;	//用于解析一些特殊数据，为兼容以前的传输模块设计
// }RECV_DATA_BUFFER;

//网络连接资源结构体
// typedef struct _nat_link_resource_
// {
// 	SOCK_INFO 		    sock;
// 	long				deviceID;
// 	long				bufferMaxLen;		//根据请求数据的通道数动态指定的最大可用缓冲大小
// 	CBufferPool			sendBuffer;			//发送数据缓冲区
// 	RECV_DATA_BUFFER	recvBuffer;			//要接收的数据
// 	RECVASYN_CALLBACK	pfRecvCallBack;		//接收数据的回调函数
// 	void*				pParam;				//回调函数参数
// 	bool				bBroken;			//是否已断开
// 	bool				bAutoRecv;			//是否为自动接收
// 	LONGLONG			prevRecvTime;		//上次接收到数据的时间(ms)
// 	LONGLONG			prevSendTime;		//上次发送数据的时间(ms)
// 	bool				bSendIdle;			//发送空闲，可以发送心跳包
// }NAT_LINK_RESOURCE;

class CSWL_MultiNetNat : public CSWL_MultiNet
{
public:
	CSWL_MultiNetNat();
	virtual ~CSWL_MultiNetNat();

public:
	bool Init(long sendBufSize, bool bSendSync = false);

	void Destroy();
	//增加新的网络连接
	int AddNewComm(long deviceID, const NatSocket sock);

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

	bool GetLinkResourceByLinkID(long deviceID, NAT_LINK_RESOURCE * &pLinkResource);

	void UpdateRemoveComm();

private:
	CMyList<NAT_LINK_RESOURCE *>			m_lstNetLink;
	CMyList<NAT_LINK_RESOURCE *>			m_tempDelNetLink;
	CPUB_Lock								m_lstLinkLock;
	CPUB_Lock								m_listLinkSendLock;
	
//	int										m_maxSockInt;
	bool									m_bSockThreadRun;
 	NatSocketSet							m_fdReadSet;
 	NatSocketSet							m_fdWriteSet;
	PUB_thread_t							m_threadId;

	long									m_sendBufSize;

	bool									m_bSendSync;	//是否为同步发送网络数据,false：异步，true：同步
															//如果为异步发送数据，需要调用SendData，否则调用SendDataSync
};

#endif // !defined(AFX_SWL_MULTINETCOMM_H__9E723A1A_97DE_445E_80BD_820A85DC30B6__INCLUDED_)
