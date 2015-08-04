#ifndef __CLIENT_CONN_MAN_H__
#define __CLIENT_CONN_MAN_H__
#include "NatSocket.h"
#include "SWL_Public.h"
#include "PUB_common.h"

typedef struct _client_conn_cfg_
{
	char           dwDeviceNo[64];  /*< Ҫ���ӵ��豸�����к� */
	char           pServerIp[128];  /*< NAT������IP��ַ */
	unsigned short nServerPort;     /*< NAT�������˿ں� */
}CLIENT_CONN_CFG;

typedef enum _conn_sock_type_
{
	CONN_SOCK_TYPE_INVALID = 0,
	CONN_SOCK_TYPE_NAT,
	CONN_SOCK_TYPE_TCP
}CONN_SOCK_TYPE;

typedef enum _client_conn_state_
{
	CLI_CONN_STATE_NONE,
	CLI_CONN_STATE_CONNECTING,
	CLI_CONN_STATE_FINLISH
}CLIENT_CONN_STATE;

typedef struct _client_sock_
{
	CONN_SOCK_TYPE sock_type;
	NatSocket      nat_sock;
	SWL_socket_t   tcp_sock;
}CLIENT_SOCK;

/**
 * �����ӽ����Ժ󣬵��øú���
 * @param[in] sock �����ӵ��׽���
 * @param[in] pObject �ص���ض���ָ��
 * @param[in] nParam ����������
 */
typedef void (*CLIENT_CONNECT_CALLBACK)(CLIENT_SOCK sock, void *pObject, void *pParam);

class CClientConnMan
{
private:
	CClientConnMan(CLIENT_CONN_CFG& cfg);
	virtual ~CClientConnMan();
public:
	static CClientConnMan* NewClientConnMan(CLIENT_CONN_CFG& cfg);

	/**
	* ���ٺ�����������ɺ���ô˺�����
	*/  
	void Destroy();

	/**
	* ��ͬ���ķ�ʽ��������һ��
	* @return ��������socket�����CLIENT_SOCK.sock_type == CONN_SOCK_TYPE_INVALID��������ʧ�� ��
	*/		
	CLIENT_SOCK ConnectSyn();

	/**
	* ��ʼ���첽��ʽִ�����ӡ�
	* @param[in] pCallback ���ӻص�������
	* @param[in] pObject ���������ָ�롣
	* @return ����ɹ�������0�����򣬷�������ֵ��
	*/  
	int ConnectAsyn(CLIENT_CONNECT_CALLBACK pCallback, void* pObject);	

	//��ȡ����״̬,���첽����ʱ��ѯ�����Ƿ����
	CLIENT_CONN_STATE GetConnState();
private:
	void Clear();
	bool StartTryToConnect();
	static int LanConnCallback(SWL_socket_t sock, void *pObject, void *pParam);
	static int UpnpConnCallback(SWL_socket_t sock, void *pObject, void *pParam);
	static int NatConnCallback(NatSocket sock, void *pObject, void *pParam);
	static PUB_THREAD_RESULT PUB_THREAD_CALL AsynConnThread(void *pParam);
private:
	CLIENT_CONN_CFG             m_connCfg;
	SWL_socket_t                m_lanSock;
	SWL_socket_t                m_upnpSock;
	NatSocket                   m_natSock;
	bool                        m_bLanConnFinish;
	bool                        m_bUpnpConnFinish;
	bool                        m_bNatConnFinish;
	bool                        m_bRunning;
	CLIENT_CONN_STATE           m_connState;

	//�첽����
	PUB_thread_t                m_threadConn;
	CLIENT_CONNECT_CALLBACK     m_pAsynConnCallback;
	void*                       m_pCallbackObj;
};
#endif
