#ifndef _DDNS_VISIONICA_H_
#define _DDNS_VISIONICA_H_

#include "DdnsClient.h"

const char VISIONICA_DDNS_SERVER[] = "dynamic.visionica.com";
const unsigned short VISIONICA_DDNS_PORT = 8080;

typedef enum _ddns_visionica_mode_
{
	DDNS_VISIONICA_MODE_REGISTER		= 0,
	DDNS_VISIONICA_MODE_SIMPLE_UPDATE	= 1,
	DDNS_VISIONICA_MODE_KEY_UPDATE		= 2,
}DDNS_VISIONICA_MODE;

class CDdnsVisionica:public CDdnsClient
{
public:
	CDdnsVisionica();
	~CDdnsVisionica();

	unsigned long RegisterAccount(const char *pUserName, const char *pmyDomainName);

	unsigned long DdnsIPupdate( const char *pUserName, const char *pPassword, const char *pmyDomainName );
protected:
	int ConnectToServer(const char *pServer, unsigned short serverPort);
	unsigned long RegisterORUpdate(const char *pUserName, const char *pmyDomainName, DDNS_VISIONICA_MODE mode);
private:
};
#endif /*_DDNS_VISIONICA_H_*/
