
#ifndef _DDNS_NOIP_H_
#define _DDNS_NOIP_H_


#include "DdnsClient.h"

const char NOIP_DDNS_SERVER[] = "dynupdate.no-ip.com";
const unsigned short NOIP_DDNS_PORT = 80;


class CDdnsNoip:public CDdnsClient
{
public:
	CDdnsNoip();
	virtual  ~CDdnsNoip();
	unsigned long DdnsIPupdate(const char *pUserName, const char *pPassword, const char *pmyDomainName );
	
private:	

};

#endif /*_DDNS_NOIP_H_*/


