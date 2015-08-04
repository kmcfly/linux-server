// NatDeviceFetcher.h: interface for the CNatDeviceFetcher class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _NAT_DEV_FETCHER_CACHE_H_
#define _NAT_DEV_FETCHER_CACHE_H_

#include "NatUserDefine.h"

#ifdef NAT_TVT_DVR_4_0
#include "TVT_PubCom.h"
#else
#include "PUB_common.h"
#endif//NAT_TVT_DVR_4_0

#include "SWL_Public.h"
#include "NatPublicTypes.h"
#include "NatMultiDeviceFetcher.h"
#include <vector>
#include "tinyxml.h"

class CNatDevFetcherCache
{
public:
	CNatDevFetcherCache(const char *serverKey);
	~CNatDevFetcherCache();
	bool Load(const char* buf);
	bool LoadFromFile(const char* fileName);
	bool Save(char* buf, int size);
	bool SaveToFile(const char* fileName);

	NAT_DEV_REG_SERV_INFO* GetCache(const char* deviceNo);

	void SetCache(const char* deviceNo, const NAT_DEV_REG_SERV_INFO* cache);

	const char* GetServerKey();
private:


	typedef struct _nat_dev_reg_cache_
	{
		char					deviceNo[64];
		NAT_SERVER_LIST_INFO	serverInfo;
	}NAT_DEV_REG_CACHE;

	typedef std::vector<NAT_SERVER_LIST_INFO> NatServerVector;
	typedef std::vector<NAT_DEV_REG_CACHE> NatDevRegVector;
	typedef TiXmlElement  NatXmlElement;
	typedef TiXmlDocument NatXmlDocument;

	static const int MAX_SERVER_COUNT = 32;
	static const int MAX_DEV_REG_COUNT = 100;
private:
	bool InternalLoad(NatXmlDocument &doc);
	bool InternalSave(NatXmlDocument &doc);
	bool LoadServerInfo(NatXmlElement *element, NAT_SERVER_LIST_INFO &serverInfo);
	bool SaveServerInfo(NatXmlElement *element, const NAT_SERVER_LIST_INFO &serverInfo);
private:
	NatServerVector			m_serverVector;
	NatDevRegVector			m_devRegVector;
	std::string				m_serverKey;
	NAT_DEV_REG_SERV_INFO	m_cache;
private:

};


#endif//_NAT_DEV_FETCHER_CACHE_H_