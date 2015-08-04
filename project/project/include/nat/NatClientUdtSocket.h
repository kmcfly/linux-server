// NatTraversalClientPeer.h: interface for the CNatTraversalClientPeer class.
//  ��ע��һ�������账����ɺ󣬲��ܴ�����һ������
//////////////////////////////////////////////////////////////////////

#ifndef _NAT_TRAVERSAL_CLIENT_CONNECTOR_H_
#define _NAT_TRAVERSAL_CLIENT_CONNECTOR_H_

#include "NatSocketBase.h"
#include "NatRunner.h"
#include "NatUdtTransport.h"
#include "NatUdt.h"

class CNatClientUdtSocket: public CNatSocketBase, CNatUdtTransportNotifier,CUdtNotifier
{
public:    

	CNatClientUdtSocket(CNatUdt *pUdt,CNatUdtTransport *pUdtTransport, NAT_SOCKET_CATEGORY category);
	virtual ~CNatClientUdtSocket();

public:
    ////////////////////////////////////////////////////////////////////////////////
    // Implements CNatSocketBase public interface
    virtual int Close();

    virtual NatRunResult Run();

	virtual NAT_SOCKET_CATEGORY GetSocketCategory() { return m_category; };

    virtual int Send(const void *pBuf, int iLen);

    virtual int Recv(void *pBuf, int iLen);
    /**
     * ��ȡ��ǰ���Է������ݵ��ֽڴ�С
     * @return ���û�д����򷵻ص�ǰ���Է������ݵ��ֽڴ�С��0��ʾ���ͻ��������ˣ����򣬳�����-1
     */
    virtual int GetSendAvailable();

    /**
     * ��ȡ���Խ������ݵ��ֽڴ�С����Ҫ����������������
     * @return ���û�д����򷵻ص�ǰ���Խ������ݵ��ֽڴ�С��0��ʾ�����ݣ����򣬳�����-1
     */
    virtual int GetRecvAvailable();

    virtual int SetAutoRecv(bool bAutoRecv);

    virtual unsigned long GetRemoteIp();

    virtual unsigned short GetRemotePort();
public:
    ////////////////////////////////////////////////////////////////////////////////
    // Implements CNatUdtTransportNotifier public interface

    virtual void OnRecvDatagram(CNatUdtTransport *transport, const NAT_TRANSPORT_RECV_DATAGRAM* packet);
public:
	////////////////////////////////////////////////////////////////////////////////
	// Implements CUdtNotifier public interface

	virtual void OnConnect(CNatUdt *pUDT, int iErrorCode);

	virtual int OnRecv(CNatUdt *pUDT, const void* pBuf, int iLen);

	virtual int OnSendPacket(CNatUdt *pUDT, const void* pBuf, int iLen, CNatUdtTransport::SEND_OPTIONS *options);
private:
	void InternalClose();
private:
	NAT_SOCKET_CATEGORY m_category;
    CNatUdtTransport *m_pUdtTransport;
	CNatUdt		     *m_udt;
};

#endif //_NAT_TRAVERSAL_CLIENT_CONNECTOR_H_