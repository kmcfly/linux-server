// NatUpnpClient.h: interface for the CNatUpnpClient class.
//
//////////////////////////////////////////////////////////////////////
#ifndef __NAT_UPNP_CLIENT_H__
#define __NAT_UPNP_CLIENT_H__

#include "miniupnpc.h"
#include "upnpcommands.h"
#include "upnperrors.h"

/**
 * UPNP端口映射的协议类型
 */
#define NAT_PORT_PROTOCOL_TCP  "TCP"
#define NAT_PORT_PROTOCOL_UDP  "UDP"

/**
 * UPNP客户端
 */
class CNatUpnpClient
{
public:
	typedef struct _specific_port_mapping_entry_
	{
		unsigned long		internalClient;
		unsigned short		internalPort;
		char				desc[128];
		int					enabled;
		unsigned long		leaseDuration;
	}SPECIFIC_PORT_MAPPING_ENTRY;

	typedef struct _generic_port_mapping_entry_
	{
		unsigned long		remoteHost;
		unsigned long		internalClient;
		unsigned short		externalPort;
		unsigned short		internalPort;
		char				protocol[8];
		char				desc[128];
		int					enabled;
		unsigned long		leaseDuration;
	}GENERIC_PORT_MAPPING_ENTRY;

public:
	CNatUpnpClient();
	~CNatUpnpClient();

	/**
	 * 初始化
	 * @param[in] multicastif 网络接口名称或IP
	 */
	bool Initial(const char* multicastif, int *isCanceled);
	void Quit();
	bool IsInited();

	int GetSpecificPortMappingEntry(unsigned long remoteHost, unsigned short externalPort, 
		const char *protocol, SPECIFIC_PORT_MAPPING_ENTRY &info);
	int GetGenericPortMappingEntry(const int index, GENERIC_PORT_MAPPING_ENTRY &info);	

	int AddPortMapping(unsigned long remoteHost, unsigned long externalPort, const char *protocol, 
		unsigned long internalClient, unsigned long internalPort, const char *desc, unsigned long leaseDuration);

	int DeletePortMapping(unsigned long remoteHost, unsigned short externalPort, const char *protocol);

	bool Validate(int *isCanceled);

	unsigned long GetLanIp() { return m_lanIp; };
private:
	void Clear();
private:
	const char* GetIGDRetText(int retCode);
private:
	bool m_isInited;
	struct UPNPDev		*m_devlist;
	struct UPNPUrls		m_urls;
	struct IGDdatas		m_igdData;
	unsigned long		m_lanIp;	/* my ip address on the LAN */

};

#endif//__NAT_UPNP_CLIENT_H__