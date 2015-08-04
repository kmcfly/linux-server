#ifndef __DDNS_HI_VIEW_H__
#define __DDNS_HI_VIEW_H__

#include "DdnsClient.h"
#include "Typedef.h"


class CDdnsHiView:public CDdnsClient
{
public:
	CDdnsHiView();
	~CDdnsHiView();

	unsigned long DdnsIPupdate(const char *pUserName, const char *pPassword, const char *pmyDomainName);

private:
public:
	int Connect(const char * pSeverName);
	int Process(const char * pSendBuf, unsigned long sendLen);
	//private:
public:
	char * m_pSendBuf;
	char * m_pRecvBuf;

	unsigned long m_bufLen;

	int	m_sockfd;
};

#endif
