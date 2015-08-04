#ifndef __CLIENT_CONN_MAN_H__
#define __CLIENT_CONN_MAN_H__
#include "NatSocket.h"
#include "SWL_Public.h"
#include "PUB_common.h"

typedef struct _client_conn_cfg_
{
	char           dwDeviceNo[64];  /*< 要连接的设备的序列号 */
	char           pServerIp[128];  /*< NAT服务器IP地址 */
	unsigned short nServerPort;     /*< NAT服务器端口号 */
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
 * 当连接建立以后，调用该函数
 * @param[in] sock 新连接的套接字
 * @param[in] pObject 回调相关对象指针
 * @param[in] nParam 保留参数。
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
	* 销毁函数，连接完成后调用此函数。
	*/  
	void Destroy();

	/**
	* 以同步的方式尝试连接一次
	* @return 返回连接socket，如果CLIENT_SOCK.sock_type == CONN_SOCK_TYPE_INVALID，则连接失败 。
	*/		
	CLIENT_SOCK ConnectSyn();

	/**
	* 开始以异步方式执行连接。
	* @param[in] pCallback 连接回调函数。
	* @param[in] pObject 相关联对象指针。
	* @return 如果成功，返回0；否则，返回其它值。
	*/  
	int ConnectAsyn(CLIENT_CONNECT_CALLBACK pCallback, void* pObject);	

	//获取连接状态,在异步连接时查询连接是否结束
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

	//异步连接
	PUB_thread_t                m_threadConn;
	CLIENT_CONNECT_CALLBACK     m_pAsynConnCallback;
	void*                       m_pCallbackObj;
};
#endif
