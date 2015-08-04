// SWL_ListenProcEx.h: interface for the CSWL_ListenProcEx class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SWL_LISTENPROCEX_H__8A2C9646_247E_4B30_94A7_FD3F66A78973__INCLUDED_)
#define AFX_SWL_LISTENPROCEX_H__8A2C9646_247E_4B30_94A7_FD3F66A78973__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "SWL_TypeDefine.h"
#include "SWL_Public.h"
#include "PUB_common.h"

// typedef struct _client_sock_info_
// {
// 	SWL_socket_t	pTcpSock;
// 	unsigned long	dwIP;
// 	unsigned short	nPort;
// }CLIENT_SOCK_INFO;
//���пͻ������Ӻ�accept���Ժ���øú���
//pParam1 ��ע��ص�����ʱ���ⲿ����ȥ�Ĳ���
//pParam2 �����¼�ʱ��һЩ������Ϣ
typedef int (*GET_ACCEPT_LINK_CALLBACK)(CLIENT_SOCK_INFO * pClientSockInfo, void *pParam1, void *pParam2);

class CSWL_ListenProcEx  
{
public:
	//pCallback����Ҫ�����췵�أ���Ϊ����Ҫ����listen
	//���pCallback��û�а����ɵ�socket�ӹ�����ᵼ����Դй¶
	CSWL_ListenProcEx(GET_ACCEPT_LINK_CALLBACK pCallback, void* pParam);
	~CSWL_ListenProcEx();

	int StartListen(unsigned short int nPort);     //��ʼ�����˿ڣ�����Ƿ����������
	int StopListen();      //ֹͣ�����˿�

private:
	//��ֹʹ��Ĭ�ϵĿ������캯����=�����
	CSWL_ListenProcEx(const CSWL_ListenProcEx&);
	CSWL_ListenProcEx& operator=(const CSWL_ListenProcEx&);
	
	//accept�̣߳�����AcceptThreadRun,Ϊ���ڴ����Ϸֿ�������Ĳ���
	static PUB_THREAD_RESULT PUB_THREAD_CALL StartAccept(void *pParam);
	int AcceptThreadRun();	   //accept�߳�,accept��run����
	int AcceptThread6Run();		//IPV6

	SWL_socket_t CreateIPV4Socket(unsigned short port);
	SWL_socket_t CreateIPV6Socket(unsigned short port);

private:
	bool				m_bAcceptThreadRun;    //accept�߳��Ƿ�����
	bool				m_bListenStart;     	//�Ƿ�����listen
	GET_ACCEPT_LINK_CALLBACK m_pAcceptCallback;    //�пͻ���connect������accept����ô˻ص�����
	void				*m_pParam;             //�ص�������
	SWL_socket_t		m_SocketListen;       //���ڼ�����socket
	PUB_thread_t		m_AcceptThreadID;	    //accept�̵߳�ID
	PUB_lock_t			m_ListenLock;      	//��ʼ������ֹͣ��������

	CLIENT_SOCK_INFO	m_clientSockInfo;
};

#endif // !defined(AFX_SWL_LISTENPROCEX_H__8A2C9646_247E_4B30_94A7_FD3F66A78973__INCLUDED_)
