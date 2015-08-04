/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : 
** Date         : 2013-03-18
** Name         : DdnsMintAuto.h
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/
#ifndef __DDNS_MINT_AUTO_H__
#define __DDNS_MINT_AUTO_H__

#include "DdnsClient.h"
#include "Typedef.h"


class CDdnsMintAuto:public CDdnsClient
{
public:
	CDdnsMintAuto();
	~CDdnsMintAuto();

	unsigned long DdnsIPupdate(const char *pUserName, const char *pPassword, const char *pmyDomainName);
	unsigned long DdnsAutoReg(const char *pmyDomainName);

	void SetMac(const char * pMac);
	virtual bool RemoveDDNS();
private:
public:
	int Connect(const char * pSeverName);
	int Process(const char * pSendBuf, unsigned long sendLen);
	void Close();
	int AddDm(const char * pDomainName, const char * pExIp, const char *pUser="admin", const char * pPass="password");
	int RemoveDm();
	int Update(const char * pExIp);

//private:
public:
	char m_hostName[256];
	char m_userName[256];
	char m_password[256];

	char m_mac[256];

	char * m_pSendBuf;
	char * m_pRecvBuf;
	char * m_pTempBuf;

	unsigned long m_bufLen;
	int	m_sockfd;
};

#endif //__DDNS_MINT_AUTO_H__
