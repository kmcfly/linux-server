#ifndef _DDNS_DVRDYDNS_H
#define _DDNS_DVRDYDNS_H

#include "DdnsClient.h"

class CDdnsDvrdydns:public CDdnsClient
{
public:
	CDdnsDvrdydns( );
	~CDdnsDvrdydns();

	unsigned long DdnsIPupdate(const char *pUserName, const char *pPassword , const char *pDdnsServer );

protected:
	
	bool ConnectToServer();
	DDNS_IPUPDATE_STATUS SendMessageToServer( const char *pMessage );
	
private:
	int m_connectfd;
	
	char m_userName[128];
	char m_passWord[128];
	char m_userPass[256];
	char m_auther[512];		//���ܺ���û���������
	char m_myDomainName[132];
	
};

#endif


