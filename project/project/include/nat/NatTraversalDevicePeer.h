// NatTraversalClientPeer.h: interface for the CNatTraversalClientPeer class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _NAT_TRAVERSAL_CLIENT_PEER_H_
#define _NAT_TRAVERSAL_CLIENT_PEER_H_

#include "NatRunner.h"
#include "NatConnectProc.h"
#include "NatUdtTransport.h"
#include "NatListenProc.h"
#include "NatTraversalClient.h"
#include <string>

class CNatDnsParser
{
public:
    enum ParseState
    {
        STATE_NONE,
        STATE_INIT,
        STATE_BEGIN_PARSE,
        STATE_PARSING,
        STATE_COMPLETED
    };
public:
    CNatDnsParser();
    ~CNatDnsParser();
    bool Initialize();
    void Finalize();
    bool IsInitialized();

    void BeginParse(const char* destName);

    ParseState GetState() { return m_state; };

    bool IsCompleted() { return m_state == STATE_COMPLETED; };

    bool IsSucceeded();

    unsigned long GetDestIp() { return m_destIp; };

    CPUB_Lock& GetLock() { return m_lock; };
private:

    static PUB_THREAD_RESULT PUB_THREAD_CALL WorkThreadFunc( void *pParam);
    int RunWork();
private:
    ParseState m_state;
    bool m_workThreadRunning;
    PUB_thread_t m_workThreadId;
    CPUB_Lock      m_lock;
    std::string m_destName;
    unsigned long m_destIp;
    bool m_isSucceeded;
};

class CNatDevicePeer;

/**
* NAT��͸�ͻ���
* ��Ҫ�Ǹ�����NAT��������ͨ�Ŵ���������Traversal XML Э��Ľ������ͻ��˲��֣�
*/
class CNatTraversalDevicePeer: public  CNatTraversalClient::CTraversalNotifier
{
public:
    typedef struct _register_config_
    {
        char           deviceNo[64];        /**< �豸�����к� */
        char           serverIp[256];       /**< NAT������IP��ַ */
        unsigned short serverPort;          /**< NAT�������˿ں� */        
		unsigned char  refuseRelay;         /**< �豸�Ƿ�ܾ��м̷���Ϊ0��ʾ���ܾ���Ϊ��0��ʾ�ܾ���Ĭ��Ϊ0 */
		byte           ucNatType;           /**< �豸���������NAT���ͣ�Ϊ0��ʾδ֪���ͣ�����Ϊ�գ���ʾ0��Ĭ��Ϊ0 */
		uint16         usUpnpPort;          /**< �豸����UDT�˿ڵ�UPNPӳ��˿ڣ�Ϊ0��ʾ��֧��UPNP������Ϊ�գ���ʾ0��Ĭ��Ϊ0 */
		char           caExtraInfo[512];    /**< ��չ��Ϣ��������XML��ʽ���ڲ�������ֻ�����豸�Ϳͻ����Ͻ������Դ�͸��������͸���� */
    }REGISTER_CONFIG;


    enum ConnectState
    {
        STATE_NONE=0,
		STATE_NO_REGISTER,
        STATE_PARSE_SERV_IP,
        STATE_CONNECTING,
        STATE_REGISTERING,
        STATE_REGISTERED,
        STATE_REGISTER_FAILED,
        STATE_DISCONNECTED
    };

	
	typedef struct _server_reply_info
	{
		bool hasReply;
		NatTraversalCmdId replyCmdId;
		int replyBufSize;
		char replyBuf[NAT_DATA_BLOCK_MAX_SIZE];
	}SERVER_REPLY_INFO;

public:
    static const int UDT_SEND_WINDOW_SIZE_MAX = 3; // * CNatUdt::SEND_PACKET_BUF_SIZE = 2 KB
    static const int UDT_RECV_WINDOW_SIZE_MAX = 4; // * CNatUdt::RECV_PACKET_BUF_SIZE = 6 K

	/****
	*��ʱʱ��
	****/
	static const unsigned long ERROR_WAIT_TIME	 = 10*1000;     // ����ȴ�ʱ��

	CNatTraversalDevicePeer();
	~CNatTraversalDevicePeer();

	int GetIndex() { return m_index; };

	/**
	* ��ʼ��
	* @param config �豸���á�
	* @return ����ɹ�������0�����򷵻�����ֵ��
	*/    
	bool Start(CNatDevicePeer *devicePeer, CNatUdtTransport *udtTransport, int index, 
		const REGISTER_CONFIG *config);

	/**
	* ֹͣ��
	* @return ����ɹ�������0�����򷵻�����ֵ��
	*/
	void Stop();

	/**
	* �Ƿ��ѿ�ʼ��
	* @return �����Ƿ��ѿ�ʼ��
	*/
	bool IsStarted();

    void OnRecvDatagram(const NAT_TRANSPORT_RECV_DATAGRAM* datagram, bool *isHandled = NULL);

	/**
	* ������NatServer������
	* @return ����0
	*/
	int ResetNatServer(const NAT_SERVER_LIST_INFO *serverInfo);
	
	/**
	 * ����UPNP�˿�
	 * @param[in] natUpnpPort ��͸�˿ڵ�UPNPӳ��˿ڣ�UDP��
	 */
	void SetUpnpPort(unsigned short natUpnpPort);

	/**
	 * �����豸����չ��Ϣ
	 * @param[in] extraInfo �豸��չ��Ϣ
	 */
	void SetDeviceExtraInfo(const char* extraInfo);

	/**
	*�첽�����ӿ�
	*@return ����NatRunResult
	*/
	NatRunResult Run(unsigned long curTickCount);

	NAT_DEVICE_SERVER_ERROR GetServerError();

	NAT_DEVICE_SERVER_STATE GetServerState();

	/**
	 * ��ȡ�豸�Ĺ���IP
	 */
	unsigned long GetPublicIp();
	/**
	 * ��ȡ�豸�Ĺ����˿�
	 */
	unsigned short GetPublicPort();
public:
    ////////////////////////////////////////////////////////////////////////////////
    // interface for CNatTraversalClientUtil::CTraversalNotifier
    virtual void OnServerConnect();
    virtual void OnActiveCmd(NatTraversalCmdId cmdId, void* cmd);
    virtual void OnReply(NatTraversalCmdId reqId, const void* req, bool succeeded, 
        NatTraversalCmdId replyId, void* reply);

private:

	void InternalClose();

    void ChangeParseServIp();

    bool ChangeConnecting(long serverIp, unsigned short serverPort);

    void ChangeRegistering();

    bool ChangeRegistered(const NAT_DEVICE_REGISTER_REPLY *reply);

    void ChangeRegisterFailed();

	void ChangeDisconnected();

	void ReplyRelayConnectReq(unsigned long dwRequestSeq,unsigned long dwStatus);

    void HandleConnectP2PReq(const NAT_NOTIFY_CONNECT_P2P_REQ *req);

    void HandleConnectRelayReq(const NAT_NOTIFY_CONNECT_RELAY_REQ *req);

	int CheckSendHeartBeat();

	void TrySendReply();

	bool CheckConnectTimeout();

	void  StopTraversalClient();

    void StartSendReply(NatTraversalCmdId replyId, const void* reply, int replySize);

    void RunDnsPaseCompleted();

    bool CheckErrorTimeout();

	void ModifyRegInfo();
private:
	int m_index;
    REGISTER_CONFIG m_registerConfig;
    REGISTER_CONFIG m_resetConfig;
	CNatUdtTransport *m_pUdtTransport;
	CNatDevicePeer  *m_pDevicePeer;

	SERVER_REPLY_INFO m_replyInfo;
	unsigned long  m_lastSendCmdTime;   //���ʱʱ��
	unsigned long  m_lastRecvCmdTime; //�����Ŀ�ʼʱ��
	unsigned long  m_lastErrorTime;   // �ϴδ���ʱ��

	bool           m_isResetServer;
	CPUB_Lock      m_resetServerLock;

	bool					m_isModifyRegInfo;
	CPUB_Lock				m_modifyLock;
	NAT_DEVICE_MODIFY_REQ	m_modifyReq;

	ConnectState m_connectState;  //��ǰ���ӵ�״̬

    CNatDnsParser m_dnsParser;
    CNatTraversalClient m_traversalClient;
    NAT_DEVICE_SERVER_ERROR m_lastError;
	unsigned long m_curTickCount;
	unsigned long m_sessionId;
	unsigned long m_publicIp;
	unsigned short m_publicPort;

};

#endif