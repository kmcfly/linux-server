#ifndef _NAT_MULTI_DEVICE_FETCHER_SUIT_
#define _NAT_MULTI_DEVICE_FETCHER_SUIT_

#include "NatUdtTransport.h"
#include "NatTraversalClient.h"
#include "NatConnectProc.h"
#include "NatDeviceFetcher.h"

class CNatMultiDeviceFetcherSuit : public CNatUdtTransportNotifier, CNatTraversalClient::CTraversalNotifier
{
public:
	CNatMultiDeviceFetcherSuit();
	~CNatMultiDeviceFetcherSuit();

	bool Start(NAT_CLIENT_TRAVERSAL_CONFIG *pConf, unsigned long servSeq, unsigned long timeOut = 10000);
	void Stop();
	int Run(unsigned long currTick);
	int IsConnect();
	unsigned long GetServSeq() {return m_ServSeq;}
	bool FillInDeviceInfo(NAT_DEVICE_INFO &deviceInfo);
	NAT_CLIENT_ERROR GetErr() {return m_Err;}

public:
	////////////////////////////////////////////////////////////////////////////////
	// interface for CNatUdtTransportNotifier
	virtual void OnRecvDatagram(CNatUdtTransport *transport, const NAT_TRANSPORT_RECV_DATAGRAM* datagram);

public:
	////////////////////////////////////////////////////////////////////////////////
	// interface for CNatTraversalClientUtil::CTraversalNotifier
	virtual void OnServerConnect();
	virtual void OnActiveCmd(NatTraversalCmdId cmdId, void* cmd) { /* ignore */};
	virtual void OnReply(NatTraversalCmdId reqId, const void* req, bool succeeded, 
		NatTraversalCmdId replyId, void* reply);

private:
	void Finish(NAT_CLIENT_ERROR error, NAT_FETCH_DEVICE_REPLY *reply);

private:
	bool							m_bConnect;
	CPUB_Lock						m_Lock;
	bool							m_bStart;

	NAT_CLIENT_TRAVERSAL_CONFIG		m_Conf;
	CNatUdtTransport				m_udtTransport;
	CNatTraversalClient				m_traversalClient;
	unsigned long					m_startTime;
	unsigned long					m_TimeOut;
	unsigned long					m_ServSeq;
	NAT_FETCH_DEVICE_REPLY			m_DeviceInfo;
	NAT_CLIENT_ERROR				m_Err;
};


#endif