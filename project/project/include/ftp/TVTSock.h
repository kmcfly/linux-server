///////////////////////////////////////////////////////////////////////////////////////
#ifndef _TVT_SOCK_H_
#define _TVT_SOCK_H_

#ifndef WIN32
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <netdb.h>
#endif

#include "SWL_Public.h"
#include "FtpType.h"

#include "SSLClient.h"

class CTVTSock
{
public:
	CTVTSock();
	virtual ~CTVTSock();

	virtual bool CreateSock(unsigned short localPort, unsigned short remotePort, unsigned long remoteInAddr);
	bool CreateSock(unsigned short localPort, unsigned short remotePort, const char *pSZRemoteAddr);

	bool ReCreate();
	virtual void CloseSock();

	int SendData(const char *pData, unsigned long dataLen, unsigned long sleepUS=0);
	int RecvData(char *pBuf, unsigned long bufLen, long sleepUS=0);

	SWL_socket_t GetSocket(){return m_sockFd;}
	bool SetTTL(int ttl);

	bool SetSSLClient(CSSLClient *pSSLClient);
	CSSLClient * GetSSLClient(){return m_pSSLClient;}

	int	SetSockOpt(int level, int optname, void * optVal, socklen_t * pOptLen);
	int GetSockOpt(int level, int optname, void * optVal, socklen_t * pOptLen);
	void SetSockBlock(bool bBlock=true);
	bool IsCreateSocket()
	{
		if (SWL_INVALID_SOCKET != m_sockFd)
		{
			return true;
		}

		return false;
	}
protected:
	unsigned short		m_localPort;
	unsigned short		m_remotePort;
	unsigned long		m_remoteInAddr;

	SWL_socket_t		m_sockFd;
	struct sockaddr_in	m_remoteSockAddr;

	CSSLClient		*	m_pSSLClient;

private:
	virtual int Send(const char *pData, unsigned long dataLen)=0;
	virtual int Recv(char *pBuf, unsigned long bufLen)=0;
	
};


class CTVTUDPSock : public CTVTSock
{
public:
	CTVTUDPSock(){};
	~CTVTUDPSock(){};

	bool CreateSock(unsigned short localPort, unsigned short remotePort, unsigned long remoteInAddr);
protected:
private:
	int Send(const char *pData, unsigned long dataLen);
	int Recv(char *pBuf, unsigned long bufLen);
};


class CTVTUPDMSock : public CTVTUDPSock
{
public:
	CTVTUPDMSock(){};
	~CTVTUPDMSock(){};
	bool CreateSock(unsigned short localPort, unsigned short remotePort, unsigned long remoteInAddr);
	void CloseSock();
protected:
private:
};

class CTVTTCPSock : public CTVTSock
{
public:
	CTVTTCPSock(){};
	~CTVTTCPSock(){};
	bool CreateSock(unsigned short localPort, unsigned short remotePort, unsigned long remoteInAddr);
protected:
private:
	int Send(const char *pData, unsigned long dataLen);
	int Recv(char *pBuf, unsigned long bufLen);
private:
};
#endif /*_TVT_SOCK_H_*/
///////////////////////////////////////////////////////////////////////////////////////

