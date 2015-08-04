#ifndef __DDNS_TECVOZ_H__
#define __DDNS_TECVOZ_H__

#include "DdnsClient.h"
#include "Typedef.h"
#include "Product.h"


class CDdnsTecvoz : public CDdnsClient
{
public:
	CDdnsTecvoz();
	~CDdnsTecvoz();

	unsigned long DdnsIPupdate(const char *pUserName, const char *pPassword , const char *pDdnsServer);
	void SetDeviceName(const char *pDeviceName){/*strcpy(m_deviceName, pDeviceName)*/;}
	void SetHttpPort(unsigned short httpPort){m_httpPort = httpPort;}
	void SetServerPort(unsigned short serverPort){m_serverPort = serverPort;}

protected:
private:
	void GetDeviceName(PRODUCT_TYPE productType, char * pDeviceName);
private:
	char m_cmdBuf[2048];
	char m_encodePwd[512];
	char m_deviceName[120];
	char m_deviceMac[240];
	PRODUCT_INFO   m_productInfo;
	unsigned short m_httpPort;
	unsigned short m_serverPort;
};

#endif
