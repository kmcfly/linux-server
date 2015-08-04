// NatClientTcpSocket.h: interface for the CNatClientTcpSocket class.
// ���ھ���������
//////////////////////////////////////////////////////////////////////

#ifndef _NAT_CLIENT_TCP_SOCKET_H_
#define _NAT_CLIENT_TCP_SOCKET_H_

#include "NatSocketBase.h"

class CNatClientTcpSocket: public CNatSocketBase
{
public:
	CNatClientTcpSocket(SWL_socket_t socket, unsigned long ip, unsigned short port, NAT_SOCKET_CATEGORY category);
	~CNatClientTcpSocket();

public:
	////////////////////////////////////////////////////////////////////////////////
	// Implements CNatSocketBase public interface

	virtual int Close();

	virtual NatRunResult Run(){return RUN_NONE;};

	virtual NAT_SOCKET_CATEGORY GetSocketCategory() { return m_category; };

    virtual int Send(const void *pBuf, int iLen);

    virtual int Recv(void *pBuf, int iLen);
    /**
     * ��ȡ���ͻ�����״̬
     * @return ���û�д�����socket�������ݿɷ����򷵻�1��0��ʾ�����ݣ����򣬳�����-1
     */
	virtual int GetSendAvailable();

    /**
     * ��ȡ���ջ�����״̬����Ҫ����������������
     * @return ���û�д�����socket�������ݿɽ����򷵻�1��0��ʾ�����ݣ����򣬳�����-1
     */
	virtual int GetRecvAvailable();

	virtual int SetAutoRecv(bool bAutoRecv){return 0;};

	virtual unsigned long GetRemoteIp(){return m_ip;}

	virtual unsigned short GetRemotePort(){return m_port;}

private:
	NAT_SOCKET_CATEGORY   m_category;
	SWL_socket_t          m_sockFd;
	unsigned long         m_ip;
	unsigned short        m_port;
};

#endif //_NAT_CLIENT_TCP_SOCKET_H_