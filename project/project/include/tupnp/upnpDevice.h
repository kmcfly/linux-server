/////////////////////////////////////////////////////////////////////
#ifndef _UPNP_DEVICE_H_
#define _UPNP_DEVICE_H_

#include "upnpSock.h"
#include "uhttp.h"
#include "IGDevice.h"

class CUpnpDevice
{
public:
	CUpnpDevice();
	~CUpnpDevice();

	bool Initial(const char *pLocation);
	void Quit();

	bool ISSupportService(const char *pService);
	const char *GetServiceScpdUrl(const char *pService);
	bool GetScpdInfo(const char *pService);

	bool GetSpecificPortMappingEntry(unsigned long remoteHost,unsigned short externalPort, const char *pProtocol);
	bool AddPortMapping(unsigned long remoteHost,unsigned short externalPort, const char *pProtocol, unsigned short internalPort, unsigned long internalClient, bool bEnable, const char *pDescription);
	bool DeletePortMapping(unsigned long remoteHost, unsigned short externalPort, const char *pProtocol);

	bool GetExternalIPAddress(char *pIp);
private:
	int GetDevDesInfo(const char *pDesFilePath, const char *pHost);
	int GetServiceScpdInfo(const char *pDesFilePath, const char *pHost);
private:

	CUpnpSock	*m_pSock;
	CUHttp		*m_pUHttp;
	CIGDevice	m_igDevice;

	char		*m_pHost;
};
#endif /*_UPNP_DEVICE_H_*/
/////////////////////////////////////////////////////////////////////
