#ifndef DDNS_MEIBU_H_
#define DDNS_MEIBU_H_
#include <arpa/inet.h>
#include "DdnsClient.h"

#define DDNSLONGINCMDLEN 1024


class CDdnsMeibu:public CDdnsClient
{
public:
	CDdnsMeibu();
	~CDdnsMeibu();

	unsigned long DdnsIPupdate(const char *pUserName, const char *pPassword, const char *pmyDomainName );

protected:
	
	//获得登陆服务器的命令
	void GetLoginCommand( const char *pUserName, const char *pPassword );
	unsigned long ConnectToServer( const char *pDdnsServer , unsigned short sPort );
	unsigned long ReciveMsgFromServer( char *pMessage,  int messageLen );
	
private:
	char m_LoginCommand[DDNSLONGINCMDLEN];	

	int m_ServerSockfd;
	struct sockaddr_in m_Server;
	
		
};


#endif

