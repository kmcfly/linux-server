#ifndef __DDNS_CNB_H__
#define __DDNS_CNB_H__

#include <arpa/inet.h>
#include "DdnsClient.h"
#include "ddnscli.h"
#include "Product.h"

#define DDNSLONGINCMDLEN 1024


class CDdnsCNB:public CDdnsClient
{
public:
	CDdnsCNB();
	~CDdnsCNB();

	unsigned long DdnsIPupdate(const char *pUserName, const char *pPassword, const char *pmyDomainName );
	unsigned long DdnsAutoReg(char * pUser, char * pPsw, char *pmyDomainName);
	virtual bool RemoveDDNS();
	void SetHttpPort(unsigned short httpPort){m_nWebPort = httpPort;}
	void SetServerPort(unsigned short serverPort){m_nSessionPort = serverPort;}
	
private:
	int GetLocalError(DDNS_ERROR_NO cnbErrorNo);
	void GetDeviceName(PRODUCT_TYPE productType, char * pDeviceName);
private:
	int m_nWebPort;
	int m_nSessionPort;
	int m_nProductType;				//当前默认为 2;
	char m_sModel[128];				//CNB的产品型号;			//IPCAMERA/DVR/NVR Model Name
	char m_sSoftVer[128];			//软件版本		//Firmware or software Version
};


#endif
