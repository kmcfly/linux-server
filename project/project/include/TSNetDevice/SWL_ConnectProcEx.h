// SWL_ConnectProcEx.h: interface for the CSWL_ConnectProcEx class.
//
//////////////////////////////////////////////////////////////////////
/*
	һ��CSWL_ConnectProc����ֻ������һ��ָ��IP��ַ��һ���˿ڣ����ǿ��Դ����
	�����д����������

һ�����ӣ�ConnectSyn��������һ�Σ�������Ӳ�����������

�첽���ӣ�ConnectAsyn�ṩ�첽���ӣ��������������ʱ������������첽�����̣߳���
		  ���ӳɹ���ᴥ���ص����������˳��̣߳��´ε���ConnectAsyn��ʱ��Ҫjoin,
		  ����Destroy�����ʱ��join����Ϊuclibc��֧���̵߳�detach����,���Բ����
		  ����
		  ����һ���첽����û�з����꣬�򱾴ε������ʧ��

ע�⣺  1������Ķ���ֻ����NewConnectProc�Ӷ��ﴴ��������Destroy���٣�ÿ�������������һ��
		   �ԳƵĲ������������
		2��������Destroy�ͱ�ĺ�����ͬ�����⣬Destroy�൱�ڶԸö����delete���ͺ���û�б�Ҫ
		   ����delete��һ����������ͬ������
		3���������������ȥ�����麯�������Ҫ�̳У�����Ҫ��������麯����
		   ����ֻ����Destroy���ٶ��������������Ϊ˽��
*/

#if !defined(AFX_SWL_CONNECTPROCEX_H__5EC97C8D_8F8B_4BDD_BC3B_F3C66A2BABE9__INCLUDED_)
#define AFX_SWL_CONNECTPROCEX_H__5EC97C8D_8F8B_4BDD_BC3B_F3C66A2BABE9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DDPublic.h"
#include "SWL_TypeDefine.h"
#include "SWL_Public.h"

//���пͻ������Ӻ�accept���Ժ���øú���
//pParam1 ��ע��ص�����ʱ���ⲿ����ȥ�Ĳ���
//pParam2 �����¼�ʱ��һЩ������Ϣ
typedef int (*GET_CONNECT_LINK_CALLBACKEX)(SWL_socket_t sock, void *pObject, void *pParam);

#define		SWL_MAX_IP_LENGTH	16	//"XXX.XXX.XXX.XXX" IP��ַ��󳤶�Ϊ16


class CSWL_ConnectProcEx  
{
public:
	//CreateMethod
	static CSWL_ConnectProcEx* NewConnectProc(const char *pIPAddr, unsigned short nPort, const char *pEther="eth0");
	int Destroy();
	
	//��������һ��,�Ժ���Ϊ��ʱ��λ
	SWL_socket_t ConnectSynEx(int timeOut);
				
	//���ӣ��������ϴ����ص�����,��ʱ�Ժ���Ϊ��λ
//	int ConnectAsyn(GET_CONNECT_LINK_CALLBACK pCallback, void* pObject, int iTimeOut);
	int ConnectAsynEx(GET_CONNECT_LINK_CALLBACKEX pCallback, void* pObject, int iTimeOut);	

private:
	CSWL_ConnectProcEx(const char *pIPAddr, unsigned short nPort, const char *pEther);	
	~CSWL_ConnectProcEx();

	//connect�̣߳�����ConnectThreadRun
// 	static PUB_THREAD_RESULT PUB_THREAD_CALL StartConnect(void *pParam);
// 	int ConnectThreadRun();

	static PUB_THREAD_RESULT PUB_THREAD_CALL StartConnectEx(void *pParam);
	int ConnectThreadRunEx();

private:
	char					m_szIP[SWL_MAX_IP_LENGTH];	   //������Ҫ���ӵ�IP��ַ
	char					m_ether[16];
	unsigned long			m_dwIP;		   //ת�����޷������ε�IP��ַ
	const unsigned short	m_nPort;       //������Ҫ���ӵĶ˿�
	int						m_iTimeOut;	   //�첽���ӵ�ʱ��ĳ�ʱʱ�䣬��Ϊ����Ϊ��λ
	SWL_socket_t			m_Sock;        //�������ӵ�socket

//	GET_CONNECT_LINK_CALLBACK m_pConnectCallback;
//	void					  *m_pConnectCallbackParam;
	
	GET_CONNECT_LINK_CALLBACKEX m_pConnectCallbackEx;
	void					  *m_pConnectCallbackParamEx;

	PUB_thread_t            m_ConnectThreadID;   //�첽�����̵߳�ID
	bool                    m_bConnectThreadRun; //�첽�����߳��Ƿ���ִ��
	bool					m_bDestroy;
	
	PUB_lock_t				m_ConnectSynLock; 
	PUB_lock_t				m_ConnectAsynLock; 

};

#endif // !defined(AFX_SWL_CONNECTPROCEX_H__5EC97C8D_8F8B_4BDD_BC3B_F3C66A2BABE9__INCLUDED_)
