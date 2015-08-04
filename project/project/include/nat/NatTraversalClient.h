// NatTraversalClientPeer.h: interface for the CNatTraversalClientPeer class.
//  ��ע��һ�������账����ɺ󣬲��ܴ�����һ������
//////////////////////////////////////////////////////////////////////

#ifndef _NAT_TRAVERSAL_CLIENT_H_
#define _NAT_TRAVERSAL_CLIENT_H_

#include "NatSocketBase.h"
#include "NatRunner.h"
#include "NatConnectProc.h"
#include "NatUdtTransport.h"
#include "NatUdt.h"
#include "NatTraversalHandler.h"
#include "NatTraversalProtocol.h"
#include "NatSimpleDataBlock.h"
#include "tinyxml.h"
#include "RelayObj.h"

/**
 * ��͸�ͻ���
 * ��Ҫ����͸Э��Ŀͻ�����߼����봩͸Э��ķ��������Ӧ
 */
class CNatTraversalClient: public CUdtNotifier, CNatTraversalHandler::CHandlerNotifier
{
public:
    static const int UDT_SEND_WINDOW_SIZE_MAX = 30; // * CNatUdt::SEND_PACKET_BUF_SIZE = 2 KB
    static const int UDT_RECV_WINDOW_SIZE_MAX = 40; // * CNatUdt::RECV_PACKET_BUF_SIZE = 6 K
public:
    enum TraversalServerState
    {
        STATE_STOPED,
        STATE_CONNECTING,
        STATE_CONNECTED,
        STATE_DISCONNECTED
    };

    enum TraversalRequestState
    {
        STATE_REQ_NONE,
        STATE_REQ_STARTING,
        STATE_REQ_SENT,
        STATE_REQ_COMPLETED
    };

    class CTraversalNotifier
    {
    public:
        /**
         * ����������ӳɹ��Ļص�֪ͨ
         */
        virtual void OnServerConnect() = 0;
        /**
         * ������������ص�֪ͨ
         * �������������ͨ������������Ӧ��������ͨ��OnReply�ص��ġ�
         */
        virtual void OnActiveCmd(NatTraversalCmdId cmdId, void* cmd) = 0;

        /**
         * ���������Ӧ��ص�֪ͨ
         * @param[in] reqId     ����ID
         * @param[in] req       ��������
         * @param[in] succeeded �����Ƿ�ɹ������ʧ�ܣ���ʾΪ��ʱ������Ҫ���Ժ�����ֶΣ�replyId��reply��
         * @param[in] replyId   Ӧ��ID
         * @param[in] reply     Ӧ������
         */
        virtual void OnReply(NatTraversalCmdId reqId, const void* req, bool succeeded,
            NatTraversalCmdId replyId, void* reply) = 0;

    };

public:
    CNatTraversalClient();
    ~CNatTraversalClient();

	void SetHeartbeatInterval(unsigned long interval);
	void SetConnectionTimeout(unsigned long timeout);
	void SetKeepHoleTTL(int ttl);

    bool Start(unsigned long serverIp, unsigned short serverPort, 
        CNatUdtTransport *sendTransport);

    void Stop();

    bool IsStarted();

    bool IsConnected();

    TraversalServerState GetConnectState() { return m_state; };

    NatRunResult Run(unsigned long curTickCount);

    void NotifyRecvDatagram(const NAT_TRANSPORT_RECV_DATAGRAM* datagram, bool *isHandled = NULL);

    bool IsSendingCmd();

    void SendCmd(NatTraversalCmdId cmdId, void *cmd, int cmdLen);

    bool IsRequesting();

    void StartRequest(NatTraversalCmdId reqId, const void* req, unsigned long reqLen);

    unsigned long GenNextReqSeq();

    void SetNotifier(CTraversalNotifier *notifier);
public:
    ////////////////////////////////////////////////////////////////////////////////
    // interface for CUdtNotifier
    virtual void OnConnect(CNatUdt *pUDT, int iErrorCode);

    virtual int OnRecv(CNatUdt *pUDT, const void* pBuf, int iLen);

    virtual int OnSendPacket(CNatUdt *pUDT, const void* pBuf, int iLen, CNatUdtTransport::SEND_OPTIONS *options);
public:
    ////////////////////////////////////////////////////////////////////////////////
    // CNatTraversalHandler::CHandlerNotifier public interface
    virtual int OnSendTraversalData(const char* pData, int iLen);

    virtual void OnBeforeRecvCmd(const NAT_TRAVERSAL_VERSION* version, const NatTraversalCmdId &cmdId, bool &isHandled);

    virtual void OnRecvCmd(const NAT_TRAVERSAL_VERSION* version, const NatTraversalCmdId &cmdId, void* cmd);

    virtual void OnRecvCmdError(CNatTraversalXmlParser::ParseError error);
private:
    void Clear();
    void ChangeDisconnected();
    void TrySendReq();
    void NotifyReply(bool succeeded, NatTraversalCmdId replyId, void* reply);
private:

    CNatUdtTransport *m_sendTransport;
    TraversalServerState m_state;
    CTraversalNotifier *m_notifier;
    CNatUdt m_udt;
    CNatTraversalHandler m_traversalHandler;

    TraversalRequestState m_reqState;
    unsigned long m_reqStartTime;
    NatTraversalCmdId m_reqId;
    char m_reqBuf[NAT_DATA_BLOCK_MAX_SIZE];
    int m_reqLen;
    unsigned long m_reqSeq;
};


NAT_CLIENT_ERROR Nat_TraslateClientError( TraversalReplyStatus replyStatus );

NAT_CLIENT_ERROR Nat_TraslateClientError(int replyStatus);

#endif//_NAT_TRAVERSAL_CLIENT_H_