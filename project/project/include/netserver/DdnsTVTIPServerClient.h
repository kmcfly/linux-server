#ifndef DDNS_TVT_IP_SERVER_H_
#define DDNS_TVT_IP_SERVER_H_

#include <arpa/inet.h>
#include "DdnsClient.h"
#include "entitydef.h"

class CDdnsTVTIPServerClient:public CDdnsClient
{
public:
	CDdnsTVTIPServerClient();
	~CDdnsTVTIPServerClient();

	void SetDeviceName(const char *pDeviceName);
	void SetHttpPort(unsigned short httpPort);
	void SetServerPort(unsigned short serverPort);

	unsigned long DdnsIPupdate(const char *pUserName, const char *pPassword, const char *pmyDomainName);
protected:
	DDNS_IPUPDATE_STATUS GetMessageFromServer(int sockfd, char *pbuf, int &len);

private:
	SERVER_MSG	m_serverMsg;
	ANSWER_PACKET m_answerPacket;
};
#endif

