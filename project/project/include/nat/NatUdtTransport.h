// NatUdtTransport.h: interface for the CNatUdtTransport class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _NAT_UDP_TRANSPORT_H_
#define _NAT_UDP_TRANSPORT_H_

#include "NatRunner.h"
#include <vector>
#include "NatHeapDataManager.h"

#define MAX_RECVBUFF					128*1024
#define MAX_SENDBUFF					128*1024

class CNatUdtTransport;

typedef struct _nat_udt_transport_config
{
    unsigned long localIp;
    unsigned short localPort;
}NAT_UDT_TRANSPORT_CONFIG;

const static int NAT_TRANSPORT_RECV_SIZE_MAX = 1500;

typedef struct _nat_transport_recv_diagram
{
    unsigned long fromIp;
    unsigned short fromPort;
    unsigned long dataSize;
    char data[NAT_TRANSPORT_RECV_SIZE_MAX];
}NAT_TRANSPORT_RECV_DATAGRAM;

class CNatUdtTransportNotifier
{
public:
    ~CNatUdtTransportNotifier() {};
    virtual void OnRecvDatagram(CNatUdtTransport *transport, const NAT_TRANSPORT_RECV_DATAGRAM* datagram) = 0;
};


#ifdef USE_UDT_TRANSPORT_STAT
typedef struct __nat_udt_transport_stat
{
    unsigned long long totalSendPacket;
    unsigned long long totalSendSize;
    unsigned long long totalRecvPacket;
    unsigned long long totalRecvSize;

}NAT_UDT_TRANSPORT_STAT;

typedef struct __nat_udt_transport_updater
{
    unsigned long long startTimeSec;
    unsigned long long lastUpdateTimeSec;
    NAT_UDT_TRANSPORT_STAT lastStat;
}nat_udt_transport_updater;

#endif

/**
 * ����UDPЭ���UDT����
 * ʵ�ʴ���UDTЭ�����ݵĵײ��շ���
 */
class CNatUdtTransport
{
#ifdef USE_UDT_TRANSPORT_STAT
public:
    static const unsigned long STAT_UPDATE_INTERVAL = 3; // 3 MSec
    NAT_UDT_TRANSPORT_STAT stat;
    nat_udt_transport_updater updater;

    void UpdateStat();
#endif
public:
    static const int MAX_UDP_DIAGRAM_SIZE = 1700;

	typedef struct _send_options
	{
		int				*ttl; // ��� != NULL����ʹ�ø�TTL�����ݰ�
	}SEND_OPTIONS;

public:
    CNatUdtTransport();
    virtual ~CNatUdtTransport();

    int Start(const NAT_UDT_TRANSPORT_CONFIG* config);

    void Stop();

    bool IsStarted();

    NatRunResult Run();

	int Send(unsigned long destIP, unsigned short destPort, 
		const void* packet, int iLen, SEND_OPTIONS *options);

    void SetNotifier(CNatUdtTransportNotifier* notifier);

    CNatUdtTransportNotifier* GetNotifier();
     /**
      * ��ȡ���ذ󶨵�IP��ַ 
      */
    unsigned long GetLocalIp();
   /**
    * ��ȡ���ذ󶨵Ķ˿� 
    */
    unsigned short GetLocalPort();
private:
    void InternalStop();
    NatRunResult RecvDatagram();

	void NotifyOnRecvDatagram();
	int SendWithTTL(unsigned long destIP, unsigned short destPort, 
		const void* packet, int iLen, int ttl);
private:
    bool m_started;
    NAT_UDT_TRANSPORT_CONFIG m_config;
    CNatUdtTransportNotifier* m_notifier;
    SWL_socket_t m_sock;
    NAT_TRANSPORT_RECV_DATAGRAM m_recvDatagram;
    bool m_randInited;
};


SWL_socket_t CreateAndBindUdpSock( unsigned long ip, unsigned short int port);

SWL_socket_t NatCreateUdpSocket();

unsigned short GenRandomPort();

int NatUdpSendTo(SWL_socket_t sock, const void *pData, int iLen, unsigned long dwIP,
                 unsigned short nPort);

int NatUdpRecvfrom(SWL_socket_t sock, void* pBuff, int iLen, unsigned long *pIP, 
                 unsigned short *pPort, unsigned long dwTimeOut);

#endif//_NAT_UDP_TRANSPORT_H_