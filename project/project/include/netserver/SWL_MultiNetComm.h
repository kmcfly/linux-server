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
	//�����µ���������
	int AddNewComm(long deviceID, SWL_socket_t sock);

	//ɾ��ָ��ID����������
	int RemoveComm(long deviceID);

	//ɾ�����е���������
	int RemoveAllConnect();

	//�������ݣ������������ʹ��
	int SendData(long deviceID, const void *pBuf, int iLen, bool bBlock, bool bRawData = false);

	int SendDataSync(long deviceID, const void *pBuf, int iLen, bool bBlock);

	//�ⲿ�������ô˽ӿڽ�������
	int RecvData(long deviceID, char *pData, int iLen, bool bBlock = false, long lBlockTime = 0);

	//�������ݽ���ģʽ�����Զ����ջ����ⲿ��������
	int SetRecvMode(long deviceID, bool bAutoRecv);

	//�����Զ��������ݵĻص�����
	int SetAutoRecvCallback(long deviceID, RECVASYN_CALLBACK pfRecvCallBack, void *pParam);

	void SetBufferMaxLen(long deviceID, long bufferMaxLen);

	//�������������ÿ�ζ�ʵ�ڵط����������ݣ����������ж��ǲ��Ǻܾ�û�����ݷ����ˣ�����ǣ�����һ����������
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
 
 	bool									m_bSendSync;	//�Ƿ�Ϊͬ��������������,false���첽��true��ͬ��
 															//���Ϊ�첽�������ݣ���Ҫ����SendData���������SendDataSync
};

#endif // !defined(AFX_SWL_MULTINETCOMM_H__9E723A1A_97DE_445E_80BD_820A85DC30B6__INCLUDED_)
