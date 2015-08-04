///////////////////////////////////////////////////////////////////////////////////////
#ifndef _UPNP_SOCK_H_
#define _UPNP_SOCK_H_

#include "SWL_Public.h"
#include "PUB_common.h"

class CUpnpSock
{
public:
	CUpnpSock();
	virtual ~CUpnpSock();
	virtual bool CreateSock(unsigned short localPort, unsigned short remotePort, unsigned long remoteInAddr);
	bool CreateSock(unsigned short localPort, unsigned short remotePort, const char *pSZRemoteAddr);

	bool ReCreate();
	virtual void CloseSock();

	int SendData(const char *pData, unsigned long dataLen, unsigned long sleepUS=0);
	int RecvData(char *pBuf, unsigned long bufLen, unsigned long sleepUS=0);

	SWL_socket_t GetSocket(){return m_sockFd;}
	bool SetTTL(int ttl);

protected:
	unsigned short		m_localPort;
	unsigned short		m_remotePort;
	unsigned long		m_remoteInAddr;

	SWL_socket_t		m_sockFd;
	struct sockaddr_in	m_remoteSockAddr;

private:
	virtual int Send(const char *pData, unsigned long dataLen)=0;
	virtual int Recv(char *pBuf, unsigned long bufLen)=0;
	
};


class CUpnpUDPSock : public CUpnpSock
{
public:
	CUpnpUDPSock(){};
	~CUpnpUDPSock(){};

	bool CreateSock(unsigned short localPort, unsigned short remotePort, unsigned long remoteInAddr);
protected:
private:
	int Send(const char *pData, unsigned long dataLen);
	int Recv(char *pBuf, unsigned long bufLen);
};


class CUpnpUPDMSock : public CUpnpUDPSock
{
public:
	CUpnpUPDMSock(){};
	~CUpnpUPDMSock(){};
	bool CreateSock(unsigned short localPort, unsigned short remotePort, unsigned long remoteInAddr);
	void CloseSock();
protected:
private:
};

class CUpnpTCPSock : public CUpnpSock
{
public:
	CUpnpTCPSock(){};
	~CUpnpTCPSock(){};
	bool CreateSock(unsigned short localPort, unsigned short remotePort, unsigned long remoteInAddr);
protected:
private:
	int Send(const char *pData, unsigned long dataLen);
	int Recv(char *pBuf, unsigned long bufLen);
};
#endif /*_UPNP_SOCK_H_*/
///////////////////////////////////////////////////////////////////////////////////////

