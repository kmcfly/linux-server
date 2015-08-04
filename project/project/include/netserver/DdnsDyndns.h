/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : 
** Date         : 2009-08-19
** Name         : DdnsDydns.h
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/
#ifndef _DDNS_DYNDNS_H_
#define _DDNS_DYNDNS_H_

#include "DdnsClient.h"
#include "Typedef.h"


class CDdnsDyndns:public CDdnsClient
{
public:
	CDdnsDyndns();
	~CDdnsDyndns();
	
	unsigned long DdnsIPupdate(const char *pUserName, const char *pPassword, const char *pmyDomainName );

protected:
	int KeepLinkProc();

	char m_CmdBuf[1024];
	char m_HostName[256];
	char m_Username[256];
	char m_Password[256];
	char m_EncodePwd[512];
	int  m_DNSPort;
};

#endif //_DDNS_DYNDNS_H_


