// NatTcpClientPeer.h: interface for the NatTcpClientPeer class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _NAT_TCP_CLIENT_PEER_H_
#define _NAT_TCP_CLIENT_PEER_H_

#include "NatDeviceLanSearcher.h"
#include "NatSocketBase.h"

class CNatTcpClientPeer
{
public:
	CNatTcpClientPeer();
	~CNatTcpClientPeer();

	int Init(int iTimeout, char *pSn);
	void SetConnectNotifier(CNatSocketConnectNotifier* pNotifier);
public:
	//搜索到设备后调用的回调函数
	static int OnSearch(const NAT_DEVICE_LAN_SEARCHER_INFO &info, void *pObject, void *pParam);

private:
	int ConnectToDevice(const NAT_DEVICE_LAN_SEARCHER_INFO &info);
	void NotifyOnConnect(NatSocket sock,int iErrorCode);

private:
	CNatDeviceLanSearcher       *m_pSearcher;
	CNatSocketConnectNotifier	*m_pNotifier;
};

#endif
