// SWL_MultiNetComm.h: interface for the CSWL_MultiNetComm class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __SWL_MULTINET_H__
#define __SWL_MULTINET_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SWL_Public.h"
#include "mylist.cpp"
#include "PUB_common.h"
#include "BufferPool.h"
#include "NatSocket.h"

#define		MAX_KEEP_TIME		30000

//#define		SWL_DATA_TYPE_LIVE		0
//#define		SWL_DATA_TYPE_PLAY		1

const long	SOCK_TIME_OUT = 10000;		//���ͽ��ճ�ʱʱ��

//"PACK"�ְ����ݵ�ͷ����ʶ
const long PACKED_DATA_FLAG = mmioFOURCC('P', 'A', 'C', 'K');

const unsigned long SWL_HEADFLAG = mmioFOURCC('h', 'e', 'a', 'd');
//����Ԥ�������ݣ������ж��Ƿ������ǵ��豸
typedef struct _pre_send_data
{
	unsigned long		dwFlag;			//Ԥ�������ݱ�ʶ����ΪHEADFLAG
	unsigned long		dwDevType;		//�豸����
	unsigned long		dwProductType;	//��Ʒ����
	unsigned long		dwProtocolVer;	//Э��汾
	unsigned long		dwConfigVer;	//���ð汾
	unsigned long       dwID;           //
	unsigned long		dwReserved[10];	//�����ֽ�
}PRE_SEND_DATA;

//��ΪSWL_COMBINATION_DATA����ʱ����ṹ��
typedef struct swl_combined_data
{
	unsigned	long	dwDataID;		//��������ݵ�ID������������ÿ����������ID��ͬ
	unsigned	long	dwTotalPack;	//һ���м�����
	unsigned	long	dwTotalLen;		//���е���Ч���ݵ��ܳ���
	unsigned	long	dwIndex;		//�ڼ�������������1��ʼ
	unsigned	long	dwLen;			//��������
	//SWL_DATA_TYPE_LIVE  	 SWL_DATA_TYPE_PLAY
	unsigned	long	dwPadding;		//Ϊ������һ�汾�����紫��ģ�鸽��
}NET_COMBINED_DATA;

typedef struct swl_combined_info
{
	unsigned	long	dwUsed;			//�Ƿ�����ʹ��	0 û���� 1 ʹ����
	unsigned	long	dwDataID;		//��������ݵ�ID������������ÿ����������ID��ͬ
	unsigned	long	dwTotalPack;	//һ���м�����
	unsigned	long	dwTotalLen;		//���е���Ч���ݵ��ܳ���
	unsigned	long	dwLen;			//�Ѿ�����/�����˶��ٳ���
	unsigned	char	*pValidData;	//ֻ�ڽ��յ�ʱ������
}NET_COMBINED_INFO;

//���ͻ�������С
//const long SEND_BUFFER_SIZE = 1536*1024;

//���ݴ���ɹ�����0�����򷵻�-1����ʱ���������״̬
typedef int (*RECVASYN_CALLBACK)(long clientID, void* pParam, const char* pBuf, int iLen);

typedef struct _comm_pack_head_
{
	char		headFlag[4];
	long		dataLen;
}COMM_PACK_HEAD;

enum RECV_DATA_TYPE
{
	DATA_TYPE_TRANS			= 0x0,	//͸������
	DATA_TYPE_PACK_INFO		= 0x1
};
typedef struct _recv_data_buffer_
{
	long			dataSize;	//Ҫ���յ��ܵ����ݴ�С
	char			*pData;		//�������ݵĻ�����
	long			recvSize;	//�Ѿ����յ�����
	long			dataType;	//���ڽ���һЩ�������ݣ�Ϊ������ǰ�Ĵ���ģ�����
}RECV_DATA_BUFFER;

class CSWL_MultiNet;
//����������Դ�ṹ��
typedef struct _net_link_resource_
{
	SWL_socket_t		sock;
	long				deviceID;
	long				bufferMaxLen;		//�����������ݵ�ͨ������ָ̬���������û����С
	CBufferPool			sendBuffer;			//�������ݻ�����
	RECV_DATA_BUFFER	recvBuffer;			//Ҫ���յ�����
	RECVASYN_CALLBACK	pfRecvCallBack;		//�������ݵĻص�����
	void*				pParam;				//�ص���������
	bool				bBroken;			//�Ƿ��ѶϿ�
	bool				bAutoRecv;			//�Ƿ�Ϊ�Զ�����
	LONGLONG			prevRecvTime;		//�ϴν��յ����ݵ�ʱ��(ms)
	LONGLONG			prevSendTime;		//�ϴη������ݵ�ʱ��(ms)
	bool				bSendIdle;			//���Ϳ��У����Է���������
}NET_LINK_RESOURCE;

//����������Դ�ṹ��
 typedef struct _nat_link_resource_
 {
 	NatSocket 		    sock;
 	long				deviceID;
 	long				bufferMaxLen;		//�����������ݵ�ͨ������ָ̬���������û����С
 	CBufferPool			sendBuffer;			//�������ݻ�����
 	RECV_DATA_BUFFER	recvBuffer;			//Ҫ���յ�����
 	RECVASYN_CALLBACK	pfRecvCallBack;		//�������ݵĻص�����
 	void*				pParam;				//�ص���������
 	bool				bBroken;			//�Ƿ��ѶϿ�
 	bool				bAutoRecv;			//�Ƿ�Ϊ�Զ�����
 	LONGLONG			prevRecvTime;		//�ϴν��յ����ݵ�ʱ��(ms)
 	LONGLONG			prevSendTime;		//�ϴη������ݵ�ʱ��(ms)
 	bool				bSendIdle;			//���Ϳ��У����Է���������
 }NAT_LINK_RESOURCE;

class CSWL_MultiNet 
{
public:
	CSWL_MultiNet();
	virtual ~CSWL_MultiNet();

public:
	virtual bool Init(long sendBufSize, bool bSendSync = false);

	virtual void Destroy();
	//�����µ���������
	virtual int AddNewComm(long deviceID, SWL_socket_t sock);
	virtual int AddNewComm(long deviceID, const NatSocket sock){return 0;}

	//ɾ��ָ��ID����������
	virtual int RemoveComm(long deviceID);

	//ɾ�����е���������
	virtual int RemoveAllConnect();

	//�������ݣ������������ʹ��
	virtual int SendData(long deviceID, const void *pBuf, int iLen, bool bBlock, bool bRawData = false);

	virtual int SendDataSync(long deviceID, const void *pBuf, int iLen, bool bBlock);

	//�ⲿ�������ô˽ӿڽ�������
	virtual int RecvData(long deviceID, char *pData, int iLen, bool bBlock = false, long lBlockTime = 0);

	//�������ݽ���ģʽ�����Զ����ջ����ⲿ��������
	virtual int SetRecvMode(long deviceID, bool bAutoRecv);

	//�����Զ��������ݵĻص�����
	virtual int SetAutoRecvCallback(long deviceID, RECVASYN_CALLBACK pfRecvCallBack, void *pParam){return 0;}

	virtual void SetBufferMaxLen(long deviceID, long bufferMaxLen);

	//�������������ÿ�ζ�ʵ�ڵط����������ݣ����������ж��ǲ��Ǻܾ�û�����ݷ����ˣ�����ǣ�����һ����������
	virtual void SendHeartBeat();

private:
	virtual int SendBuff(long deviceID, const void *pBuf, int iLen, bool bBlock, long lBlockTime);
	virtual int RecvBuff(long deviceID, void *pBuf, int iLen, bool bBlock, long lBlockTime);
	//static PUB_THREAD_RESULT PUB_THREAD_CALL RecvThread(void *pParam);
	virtual void RecvThreadRun();

	virtual void UpdateRemoveComm();
protected:
	virtual bool GetLinkResourceByLinkID(long deviceID, NET_LINK_RESOURCE * &pLinkResource){return false;}
	virtual bool GetLinkResourceByLinkID(long deviceID, NAT_LINK_RESOURCE * &pLinkResource){return false;}
// private:
//  protected:
//  	CMyList<NET_LINK_RESOURCE *>			m_lstNetLink;
//  	CMyList<NET_LINK_RESOURCE *>			m_tempDelNetLink;
//  	CPUB_Lock								m_lstLinkLock;
//  	CPUB_Lock								m_listLinkSendLock;
//  	
//  	int										m_maxSockInt;
//  	bool									m_bSockThreadRun;
//  	fd_set									m_fdReadSet;
//  	fd_set									m_fdWriteSet;
//  	PUB_thread_t							m_threadId;
//  
//  	long									m_sendBufSize;
//  
//  	bool									m_bSendSync;	//�Ƿ�Ϊͬ��������������,false���첽��true��ͬ��
//  															//���Ϊ�첽�������ݣ���Ҫ����SendData���������SendDataSync
};

#endif
