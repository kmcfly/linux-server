#include "ReadDdns.h"
#include "LocalDevice.h"
#include "dvrdvsdef.h"

CReadDdns::CReadDdns()
{
	m_pDdnsServer = NULL;
	m_DdnsServerCount = 0;
}

CReadDdns::~CReadDdns()
{

}

CReadDdns * CReadDdns::Instance()
{
	static CReadDdns s_PReadDddns;
	return &s_PReadDddns;
}

bool CReadDdns::Initial(unsigned long subProductID)
{
	if (m_pDdnsServer!= NULL)
	{
		assert(false);
		delete [] m_pDdnsServer;
		m_pDdnsServer = NULL;
	}
	m_DdnsServerCount = 0;
	m_ddnsCmpny = 0;

#ifndef __ENVIRONMENT_WIN32__
	unsigned long company = CLocalDevice::GetDDNSTypeFromFlash();
	m_ddnsCmpny = company;
	//
	//2代表--US02公司要求的DDNS种类，
	//9代表--US09公司的DDNS1
	//10代表--US09公司的DDNS2
	//61代表--US61公司的
	//27代表--BR27公司的
	//1-代表其他公司的。
	GetCompanyDdnsServer(subProductID, company, &m_pDdnsServer, m_DdnsServerCount);
#else
	unsigned long company = 0;
	m_ddnsCmpny = company;
	GetCompanyDdnsServer(subProductID, company, &m_pDdnsServer, m_DdnsServerCount);
#endif

	return true;	
}

void CReadDdns::Quit()
{
	if (m_pDdnsServer!= NULL)
	{
		assert(false);
		delete [] m_pDdnsServer;
		m_pDdnsServer = NULL;
	}
	m_DdnsServerCount = 0;	
}

int CReadDdns::GetDddnsCount()
{
	if (NULL == m_pDdnsServer)
	{
		return 1;
	}
	else
	{
		return m_DdnsServerCount;
	}
}

/*
以下是需定制ddns的客户清单和具体要求
1:US02
要求内置两个ddns,一个客人专用MY Q-SEE 和一个DYNDNS
客人服务器地址：myq-see.com 
2: US61
要求内置两个ddns,一个客人专用DVRLIST 和一个DYNDNS
客人服务器地址：ns1.dvrlists.com 
3: US09
要求内置两个ddns,一个客人专用EASTERNDNS 和一个DYNDNS
客人服务器地址：www.easterndns.com
上述三个客人服务器用的协议都是MINTDNS
3:BR27
要求内置他们自己的ddns dynamic.visionica.com
该客户的DDNS协议和其他不同
*/

bool CReadDdns::GetDddnsServerInfo(int index, DDNS_SERVER_INFO *pServerInfo)
{
	if (NULL == m_pDdnsServer)
	{
#if defined(__CUSTOM_RU04__)
		snprintf(pServerInfo->szDDNSServerName, 64, "%s", "www.no-ip.com");
		pServerInfo->ucDDNSID = DDNS_NOIPDNS;		
		pServerInfo->supportproperty = NCFG_ENUM_DDNS_SUPPORT_DOMAIN1;
#else
		snprintf(pServerInfo->szDDNSServerName, 64, "%s", "www.dvrdydns.com");
		pServerInfo->ucDDNSID = DDNS_DVRDYDNS;		
		pServerInfo->supportproperty = NCFG_ENUM_DDNS_SUPPORT_DOMAIN1;
#endif
		return true;
	}
	else
	{
		assert(index < m_DdnsServerCount);
		memcpy(pServerInfo, &(m_pDdnsServer[index]), sizeof(DDNS_SERVER_INFO));
		return true;
	}
}


bool CReadDdns::GetCompanyDdnsServer(unsigned long subProductID, unsigned long company, DDNS_SERVER_INFO **pServerInfo, int &count)
{

	if ((1 == company)||(0 == company))
	{
		//国内产品
		if(1 == subProductID)
		{
#if defined(__AUTOREG__)
			count = 6;
#else
			count = 5;
#endif
			DDNS_SERVER_INFO *pServerInfoTemp = new DDNS_SERVER_INFO [count];

			if (NULL == pServerInfoTemp)
			{
				count = 0;
			}
			else
			{
#if defined(__AUTOREG__)
				snprintf(pServerInfoTemp[0].szDDNSServerName, 64, "%s",  "www.autoddns.com");
				pServerInfoTemp[0].ucDDNSID = DDNS_MINT_AUTO;
				pServerInfoTemp[0].supportproperty = NCFG_ENUM_DDNS_SUPPORT_DOMAIN1;

				snprintf(pServerInfoTemp[1].szDDNSServerName, 64, "%s",  "www.dvrdydns.com");
				pServerInfoTemp[1].ucDDNSID = DDNS_DVRDYDNS;
				pServerInfoTemp[1].supportproperty = NCFG_ENUM_DDNS_SUPPORT_DOMAIN1;

				snprintf(pServerInfoTemp[2].szDDNSServerName, 64, "%s",  "www.dyndns.com");
				pServerInfoTemp[2].ucDDNSID = DDNS_DYNDNS;
				pServerInfoTemp[2].supportproperty = NCFG_ENUM_DDNS_SUPPORT_DOMAIN1;

				snprintf(pServerInfoTemp[3].szDDNSServerName, 64, "%s",  "www.meibu.com");
				pServerInfoTemp[3].ucDDNSID = DDNS_MEIBU;
				pServerInfoTemp[3].supportproperty = 0;

				snprintf(pServerInfoTemp[4].szDDNSServerName, 64, "%s",  "IP Server");
				pServerInfoTemp[4].ucDDNSID = DDNS_TVT_IP_SERVER;
				pServerInfoTemp[4].supportproperty = 0;

				snprintf(pServerInfoTemp[5].szDDNSServerName, 64, "%s",  "mintdns");
				pServerInfoTemp[5].ucDDNSID = DDNS_MINTDNS;
				pServerInfoTemp[5].supportproperty = NCFG_ENUM_DDNS_SUPPORT_DOMAIN1;
#else
				snprintf(pServerInfoTemp[0].szDDNSServerName, 64, "%s",  "www.dvrdydns.com");
				pServerInfoTemp[0].ucDDNSID = DDNS_DVRDYDNS;
				pServerInfoTemp[0].supportproperty = NCFG_ENUM_DDNS_SUPPORT_DOMAIN1;

				snprintf(pServerInfoTemp[1].szDDNSServerName, 64, "%s",  "www.dyndns.com");
				pServerInfoTemp[1].ucDDNSID = DDNS_DYNDNS;
				pServerInfoTemp[1].supportproperty = NCFG_ENUM_DDNS_SUPPORT_DOMAIN1;

				snprintf(pServerInfoTemp[2].szDDNSServerName, 64, "%s",  "www.meibu.com");
				pServerInfoTemp[2].ucDDNSID = DDNS_MEIBU;
				pServerInfoTemp[2].supportproperty = 0;

				snprintf(pServerInfoTemp[3].szDDNSServerName, 64, "%s",  "IP Server");
				pServerInfoTemp[3].ucDDNSID = DDNS_TVT_IP_SERVER;
				pServerInfoTemp[3].supportproperty = 0;

				snprintf(pServerInfoTemp[4].szDDNSServerName, 64, "%s",  "mintdns");
				pServerInfoTemp[4].ucDDNSID = DDNS_MINTDNS;
				pServerInfoTemp[4].supportproperty = NCFG_ENUM_DDNS_SUPPORT_DOMAIN1;
#endif
				*pServerInfo = pServerInfoTemp;
			}
		}
		else
		{
			//国外产品

#if defined(__KR24_IN__)
			count = 1;
			DDNS_SERVER_INFO *pServerInfoTemp = new DDNS_SERVER_INFO [count];

			if (NULL == pServerInfoTemp)
			{
				count = 0;
			}
			else
			{
				snprintf(pServerInfoTemp[0].szDDNSServerName, 64, "%s", "autoipset.com");
				pServerInfoTemp[0].ucDDNSID = DDNS_CNB;
				pServerInfoTemp[0].supportproperty = NCFG_ENUM_DDNS_SUPPORT_DOMAIN1;
			}
#endif

#if defined(__AUTOREG__) && !defined(__KR24_IN__)
			count = 6;
			DDNS_SERVER_INFO *pServerInfoTemp = new DDNS_SERVER_INFO [count];
#else
			count = 5;
			DDNS_SERVER_INFO *pServerInfoTemp = new DDNS_SERVER_INFO [count];
#endif

			
			if (NULL == pServerInfoTemp)
			{
				count = 0;
			}
			else
			{
#if defined(__CUSTOM_RU04__)
				snprintf(pServerInfoTemp[0].szDDNSServerName, 64, "%s",  "www.no-ip.com");
				pServerInfoTemp[0].ucDDNSID = DDNS_NOIPDNS;
				pServerInfoTemp[0].supportproperty = NCFG_ENUM_DDNS_SUPPORT_DOMAIN1;

				snprintf(pServerInfoTemp[1].szDDNSServerName, 64, "%s",  "www.dyndns.com");
				pServerInfoTemp[1].ucDDNSID = DDNS_DYNDNS;
				pServerInfoTemp[1].supportproperty = NCFG_ENUM_DDNS_SUPPORT_DOMAIN1;

				snprintf(pServerInfoTemp[2].szDDNSServerName, 64, "%s",  "www.meibu.com");
				pServerInfoTemp[2].ucDDNSID = DDNS_MEIBU;
				pServerInfoTemp[2].supportproperty = 0;			

				snprintf(pServerInfoTemp[3].szDDNSServerName, 64, "%s",  "www.dvrdydns.com");
				pServerInfoTemp[3].ucDDNSID = DDNS_DVRDYDNS;
				pServerInfoTemp[3].supportproperty = NCFG_ENUM_DDNS_SUPPORT_DOMAIN1;

				snprintf(pServerInfoTemp[4].szDDNSServerName, 64, "%s",  "mintdns");
				pServerInfoTemp[4].ucDDNSID = DDNS_MINTDNS;
				pServerInfoTemp[4].supportproperty = NCFG_ENUM_DDNS_SUPPORT_DOMAIN1;
#else
#if defined(__AUTOREG__) && !defined(__KR24_IN__)
#if defined(__CUSTOM_IN23__)
				snprintf(pServerInfoTemp[0].szDDNSServerName, 64, "%s",  "www.dyndns.com");
				pServerInfoTemp[0].ucDDNSID = DDNS_DYNDNS;
				pServerInfoTemp[0].supportproperty = NCFG_ENUM_DDNS_SUPPORT_DOMAIN1;

				snprintf(pServerInfoTemp[1].szDDNSServerName, 64, "%s",  "www.meibu.com");
				pServerInfoTemp[1].ucDDNSID = DDNS_MEIBU;
				pServerInfoTemp[1].supportproperty = 0;

				snprintf(pServerInfoTemp[2].szDDNSServerName, 64, "%s",  "www.no-ip.com");
				pServerInfoTemp[2].ucDDNSID = DDNS_NOIPDNS;
				pServerInfoTemp[2].supportproperty = NCFG_ENUM_DDNS_SUPPORT_DOMAIN1;

				snprintf(pServerInfoTemp[3].szDDNSServerName, 64, "%s",  "www.dvrdydns.com");
				pServerInfoTemp[3].ucDDNSID = DDNS_DVRDYDNS;
				pServerInfoTemp[3].supportproperty = NCFG_ENUM_DDNS_SUPPORT_DOMAIN1;

				snprintf(pServerInfoTemp[4].szDDNSServerName, 64, "%s",  "mintdns");
				pServerInfoTemp[4].ucDDNSID = DDNS_MINTDNS;
				pServerInfoTemp[4].supportproperty = NCFG_ENUM_DDNS_SUPPORT_DOMAIN1;
				
				snprintf(pServerInfoTemp[5].szDDNSServerName, 64, "%s",  "www.cpplusddns.com");
				pServerInfoTemp[5].ucDDNSID = DDNS_MINT_AUTO;
				pServerInfoTemp[5].supportproperty = NCFG_ENUM_DDNS_SUPPORT_DOMAIN1;
#else
				snprintf(pServerInfoTemp[0].szDDNSServerName, 64, "%s",  "www.autoddns.com");
				pServerInfoTemp[0].ucDDNSID = DDNS_MINT_AUTO;
				pServerInfoTemp[0].supportproperty = NCFG_ENUM_DDNS_SUPPORT_DOMAIN1;
				
				snprintf(pServerInfoTemp[1].szDDNSServerName, 64, "%s",  "www.dyndns.com");
				pServerInfoTemp[1].ucDDNSID = DDNS_DYNDNS;
				pServerInfoTemp[1].supportproperty = NCFG_ENUM_DDNS_SUPPORT_DOMAIN1;

				snprintf(pServerInfoTemp[2].szDDNSServerName, 64, "%s",  "www.meibu.com");
				pServerInfoTemp[2].ucDDNSID = DDNS_MEIBU;
				pServerInfoTemp[2].supportproperty = 0;

				snprintf(pServerInfoTemp[3].szDDNSServerName, 64, "%s",  "www.no-ip.com");
				pServerInfoTemp[3].ucDDNSID = DDNS_NOIPDNS;
				pServerInfoTemp[3].supportproperty = NCFG_ENUM_DDNS_SUPPORT_DOMAIN1;

				snprintf(pServerInfoTemp[4].szDDNSServerName, 64, "%s",  "www.dvrdydns.com");
				pServerInfoTemp[4].ucDDNSID = DDNS_DVRDYDNS;
				pServerInfoTemp[4].supportproperty = NCFG_ENUM_DDNS_SUPPORT_DOMAIN1;

				snprintf(pServerInfoTemp[5].szDDNSServerName, 64, "%s",  "mintdns");
				pServerInfoTemp[5].ucDDNSID = DDNS_MINTDNS;
				pServerInfoTemp[5].supportproperty = NCFG_ENUM_DDNS_SUPPORT_DOMAIN1;
#endif
#endif
#endif

				*pServerInfo = pServerInfoTemp;
			}
		}

	}
	else if (2 == company)
	{
		count = 3;
		DDNS_SERVER_INFO *pServerInfoTemp = new DDNS_SERVER_INFO [count];
		if (NULL == pServerInfoTemp)
		{
			count = 0;
		}
		else
		{
			snprintf(pServerInfoTemp[0].szDDNSServerName, 64, "%s",  "myq-see.com");
			pServerInfoTemp[0].ucDDNSID = DDNS_MYQSEE;
			pServerInfoTemp[0].supportproperty = NCFG_ENUM_DDNS_SUPPORT_DOMAIN1;	
			
			snprintf(pServerInfoTemp[1].szDDNSServerName, 64, "%s",  "www.dyndns.com");
			pServerInfoTemp[1].ucDDNSID = DDNS_DYNDNS;
			pServerInfoTemp[1].supportproperty = NCFG_ENUM_DDNS_SUPPORT_DOMAIN1;
			
			snprintf(pServerInfoTemp[2].szDDNSServerName, 64, "%s",  "mintdns");
			pServerInfoTemp[2].ucDDNSID = DDNS_MINTDNS;
			pServerInfoTemp[2].supportproperty = NCFG_ENUM_DDNS_SUPPORT_DOMAIN1;

			*pServerInfo = pServerInfoTemp;
		}
	}
	else if (9 == company)
	{
		count = 4;
		DDNS_SERVER_INFO *pServerInfoTemp = new DDNS_SERVER_INFO [count];
		if (NULL == pServerInfoTemp)
		{
			count = 0;
		}
		else
		{
			snprintf(pServerInfoTemp[0].szDDNSServerName, 64, "%s",  "www.easterndns.com");
			pServerInfoTemp[0].ucDDNSID = DDNS_EASTERNDNS;
			pServerInfoTemp[0].supportproperty = NCFG_ENUM_DDNS_SUPPORT_DOMAIN1;	

			snprintf(pServerInfoTemp[1].szDDNSServerName, 64, "%s",  "www.dyndns.com");
			pServerInfoTemp[1].ucDDNSID = DDNS_DYNDNS;
			pServerInfoTemp[1].supportproperty = NCFG_ENUM_DDNS_SUPPORT_DOMAIN1;

			snprintf(pServerInfoTemp[2].szDDNSServerName, 64, "%s",  "mintdns");
			pServerInfoTemp[2].ucDDNSID = DDNS_MINTDNS;
			pServerInfoTemp[2].supportproperty = NCFG_ENUM_DDNS_SUPPORT_DOMAIN1;
#if defined(__AUTOREG__)
#if defined(__CUSTOM_IN23__)
			snprintf(pServerInfoTemp[3].szDDNSServerName, 64, "%s",  "www.cpplusddns.com");
#else
			snprintf(pServerInfoTemp[3].szDDNSServerName, 64, "%s",  "www.autoddns.com");
#endif
			pServerInfoTemp[3].ucDDNSID = DDNS_MINT_AUTO;
			pServerInfoTemp[3].supportproperty = NCFG_ENUM_DDNS_SUPPORT_DOMAIN1;
#endif
			*pServerInfo = pServerInfoTemp;
		}
	}
	else if (10 == company)
	{
		count = 4;
		DDNS_SERVER_INFO *pServerInfoTemp = new DDNS_SERVER_INFO [count];
		if (NULL == pServerInfoTemp)
		{
			count = 0;
		}
		else
		{
			snprintf(pServerInfoTemp[0].szDDNSServerName, 64, "%s",  "www.newddns.com");
			pServerInfoTemp[0].ucDDNSID = DDNS_NEWDDNS;
			pServerInfoTemp[0].supportproperty = NCFG_ENUM_DDNS_SUPPORT_DOMAIN1;	

			snprintf(pServerInfoTemp[1].szDDNSServerName, 64, "%s",  "www.dyndns.com");
			pServerInfoTemp[1].ucDDNSID = DDNS_DYNDNS;
			pServerInfoTemp[1].supportproperty = NCFG_ENUM_DDNS_SUPPORT_DOMAIN1;

			snprintf(pServerInfoTemp[2].szDDNSServerName, 64, "%s",  "mintdns");
			pServerInfoTemp[2].ucDDNSID = DDNS_MINTDNS;
			pServerInfoTemp[2].supportproperty = NCFG_ENUM_DDNS_SUPPORT_DOMAIN1;
#if defined(__AUTOREG__)
#if defined(__CUSTOM_IN23__)
			snprintf(pServerInfoTemp[3].szDDNSServerName, 64, "%s",  "www.cpplusddns.com");
#else
			snprintf(pServerInfoTemp[3].szDDNSServerName, 64, "%s",  "www.autoddns.com");
#endif
			pServerInfoTemp[3].ucDDNSID = DDNS_MINT_AUTO;
			pServerInfoTemp[3].supportproperty = NCFG_ENUM_DDNS_SUPPORT_DOMAIN1;
#endif
			*pServerInfo = pServerInfoTemp;
		}
	}
	else if (61 == company)
	{
		count = 5;
		DDNS_SERVER_INFO *pServerInfoTemp = new DDNS_SERVER_INFO [count];
		if (NULL == pServerInfoTemp)
		{
			count = 0;
		}
		else
		{
			snprintf(pServerInfoTemp[0].szDDNSServerName, 64, "%s",  "ns1.dvrlists.com");
			pServerInfoTemp[0].ucDDNSID = DDNS_DVRLIS;
			pServerInfoTemp[0].supportproperty = NCFG_ENUM_DDNS_SUPPORT_DOMAIN1;	

			snprintf(pServerInfoTemp[1].szDDNSServerName, 64, "%s",  "www.dyndns.com");
			pServerInfoTemp[1].ucDDNSID = DDNS_DYNDNS;
			pServerInfoTemp[1].supportproperty = NCFG_ENUM_DDNS_SUPPORT_DOMAIN1;

			snprintf(pServerInfoTemp[2].szDDNSServerName, 64, "%s",  "www.dvrdydns.com");
			pServerInfoTemp[2].ucDDNSID = DDNS_DVRDYDNS;
			pServerInfoTemp[2].supportproperty = NCFG_ENUM_DDNS_SUPPORT_DOMAIN1;

			snprintf(pServerInfoTemp[3].szDDNSServerName, 64, "%s",  "www.meibu.com");
			pServerInfoTemp[3].ucDDNSID = DDNS_MEIBU;
			pServerInfoTemp[3].supportproperty = 0;

			snprintf(pServerInfoTemp[4].szDDNSServerName, 64, "%s",  "mintdns");
			pServerInfoTemp[4].ucDDNSID = DDNS_MINTDNS;
			pServerInfoTemp[4].supportproperty = NCFG_ENUM_DDNS_SUPPORT_DOMAIN1;

			*pServerInfo = pServerInfoTemp;
		}
	}
	else if (27 == company)
	{		
		count = 8;
		
		DDNS_SERVER_INFO *pServerInfoTemp = new DDNS_SERVER_INFO [count];
		if (NULL == pServerInfoTemp)
		{
			count = 0;
		}
		else
		{
			snprintf(pServerInfoTemp[0].szDDNSServerName, 64, "%s",  "TecvozDDnS");
			pServerInfoTemp[0].ucDDNSID = DDNS_TECVOZ;
			pServerInfoTemp[0].supportproperty = NCFG_ENUM_DDNS_SUPPORT_DOMAIN1;

			snprintf(pServerInfoTemp[1].szDDNSServerName, 64, "%s",  "www.ddns.com.br");
			pServerInfoTemp[1].ucDDNSID = DDNS_WINCO_BR27;
			pServerInfoTemp[1].supportproperty = NCFG_ENUM_DDNS_SUPPORT_DOMAIN1;

			snprintf(pServerInfoTemp[2].szDDNSServerName, 64, "%s",  "www.dyndns.com");
			pServerInfoTemp[2].ucDDNSID = DDNS_DYNDNS;
			pServerInfoTemp[2].supportproperty = NCFG_ENUM_DDNS_SUPPORT_DOMAIN1;

			snprintf(pServerInfoTemp[3].szDDNSServerName, 64, "%s", "www.no-ip.com");
			pServerInfoTemp[3].ucDDNSID = DDNS_NOIPDNS;		
			pServerInfoTemp[3].supportproperty = NCFG_ENUM_DDNS_SUPPORT_DOMAIN1;

			snprintf(pServerInfoTemp[4].szDDNSServerName, 64, "%s",  "www.dvrdydns.com");
			pServerInfoTemp[4].ucDDNSID = DDNS_DVRDYDNS;
			pServerInfoTemp[4].supportproperty = NCFG_ENUM_DDNS_SUPPORT_DOMAIN1;

			snprintf(pServerInfoTemp[5].szDDNSServerName, 64, "%s",  "www.autoddns.com");
			pServerInfoTemp[5].ucDDNSID = DDNS_MINT_AUTO;
			pServerInfoTemp[5].supportproperty = NCFG_ENUM_DDNS_SUPPORT_DOMAIN1;

			snprintf(pServerInfoTemp[6].szDDNSServerName, 64, "%s",  "mintdns");
			pServerInfoTemp[6].ucDDNSID = DDNS_MINTDNS;
			pServerInfoTemp[6].supportproperty = NCFG_ENUM_DDNS_SUPPORT_DOMAIN1;

			snprintf(pServerInfoTemp[7].szDDNSServerName, 64, "%s",  "www.meibu.com");
			pServerInfoTemp[7].ucDDNSID = DDNS_MEIBU;
			pServerInfoTemp[7].supportproperty = 0;

			*pServerInfo = pServerInfoTemp;
		}
	}
	else if (DDNS_COMPANY_TW01 == company)
	{

#if defined(__AUTOREG__)
		count = 7;
#else
		count = 6;
#endif

		DDNS_SERVER_INFO *pServerInfoTemp = new DDNS_SERVER_INFO [count];
		if (NULL == pServerInfoTemp)
		{
			count = 0;
		}
		else
		{
			snprintf(pServerInfoTemp[0].szDDNSServerName, 64, "%s",  "www.dyndns.com");
			pServerInfoTemp[0].ucDDNSID = DDNS_DYNDNS;
			pServerInfoTemp[0].supportproperty = NCFG_ENUM_DDNS_SUPPORT_DOMAIN1;

			snprintf(pServerInfoTemp[1].szDDNSServerName, 64, "%s",  "www.meibu.com");
			pServerInfoTemp[1].ucDDNSID = DDNS_MEIBU;
			pServerInfoTemp[1].supportproperty = 0;

			snprintf(pServerInfoTemp[2].szDDNSServerName, 64, "%s",  "www.no-ip.com");
			pServerInfoTemp[2].ucDDNSID = DDNS_NOIPDNS;
			pServerInfoTemp[2].supportproperty = NCFG_ENUM_DDNS_SUPPORT_DOMAIN1;

			snprintf(pServerInfoTemp[3].szDDNSServerName, 64, "%s",  "www.dvrdydns.com");
			pServerInfoTemp[3].ucDDNSID = DDNS_DVRDYDNS;
			pServerInfoTemp[3].supportproperty = NCFG_ENUM_DDNS_SUPPORT_DOMAIN1;

			snprintf(pServerInfoTemp[4].szDDNSServerName, 64, "%s",  "www.jflddns.com.br");
			pServerInfoTemp[4].ucDDNSID = DDNS_MINCO;
			pServerInfoTemp[4].supportproperty = NCFG_ENUM_DDNS_SUPPORT_DOMAIN1;

			snprintf(pServerInfoTemp[5].szDDNSServerName, 64, "%s",  "mintdns");
			pServerInfoTemp[5].ucDDNSID = DDNS_MINTDNS;
			pServerInfoTemp[5].supportproperty = NCFG_ENUM_DDNS_SUPPORT_DOMAIN1;

#if defined(__AUTOREG__)
			snprintf(pServerInfoTemp[6].szDDNSServerName, 64, "%s",  "www.autoddns.com");
			pServerInfoTemp[6].ucDDNSID = DDNS_MINT_AUTO;
			pServerInfoTemp[6].supportproperty = NCFG_ENUM_DDNS_SUPPORT_DOMAIN1;
#endif
			*pServerInfo = pServerInfoTemp;
		}
	}
	else if (DDNS_COMPANY_TH13 == company)
	{
		count = 6;
		DDNS_SERVER_INFO *pServerInfoTemp = new DDNS_SERVER_INFO [count];

		if (NULL == pServerInfoTemp)
		{
			count = 0;
		}
		else
		{
			snprintf(pServerInfoTemp[0].szDDNSServerName, 64, "%s",  "www.dyndns.com");
			pServerInfoTemp[0].ucDDNSID = DDNS_DYNDNS;
			pServerInfoTemp[0].supportproperty = NCFG_ENUM_DDNS_SUPPORT_DOMAIN1;

			snprintf(pServerInfoTemp[1].szDDNSServerName, 64, "%s",  "www.meibu.com");
			pServerInfoTemp[1].ucDDNSID = DDNS_MEIBU;
			pServerInfoTemp[1].supportproperty = 0;

			snprintf(pServerInfoTemp[2].szDDNSServerName, 64, "%s",  "www.no-ip.com");
			pServerInfoTemp[2].ucDDNSID = DDNS_NOIPDNS;
			pServerInfoTemp[2].supportproperty = NCFG_ENUM_DDNS_SUPPORT_DOMAIN1;
			
			snprintf(pServerInfoTemp[3].szDDNSServerName, 64, "%s",  "www.dvrdydns.com");
			pServerInfoTemp[3].ucDDNSID = DDNS_DVRDYDNS;
			pServerInfoTemp[3].supportproperty = NCFG_ENUM_DDNS_SUPPORT_DOMAIN1;

			snprintf(pServerInfoTemp[4].szDDNSServerName, 64, "%s",  "mintdns");
			pServerInfoTemp[4].ucDDNSID = DDNS_MINTDNS;
			pServerInfoTemp[4].supportproperty = NCFG_ENUM_DDNS_SUPPORT_DOMAIN1;

			snprintf(pServerInfoTemp[5].szDDNSServerName, 64, "%s",  "www.hiview.org");
			pServerInfoTemp[5].ucDDNSID = DDNS_HIVIEW;
			pServerInfoTemp[5].supportproperty = NCFG_ENUM_DDNS_SUPPORT_DOMAIN1;

			*pServerInfo = pServerInfoTemp;
		}
	}
	else if (DDNS_COMPANY_MX15 == company)
	{
		//国外产品
#if defined(__AUTOREG__)
		count = 7;
#else
		count = 6;
#endif

		DDNS_SERVER_INFO *pServerInfoTemp = new DDNS_SERVER_INFO [count];
		if (NULL == pServerInfoTemp)
		{
			count = 0;
		}
		else
		{
			snprintf(pServerInfoTemp[0].szDDNSServerName, 64, "%s",  "mymeriva.com");
			pServerInfoTemp[0].ucDDNSID = DDNS_MX15;
			pServerInfoTemp[0].supportproperty = NCFG_ENUM_DDNS_SUPPORT_DOMAIN1;

			snprintf(pServerInfoTemp[1].szDDNSServerName, 64, "%s",  "www.dyndns.com");
			pServerInfoTemp[1].ucDDNSID = DDNS_DYNDNS;
			pServerInfoTemp[1].supportproperty = NCFG_ENUM_DDNS_SUPPORT_DOMAIN1;

			snprintf(pServerInfoTemp[2].szDDNSServerName, 64, "%s",  "www.meibu.com");
			pServerInfoTemp[2].ucDDNSID = DDNS_MEIBU;
			pServerInfoTemp[2].supportproperty = 0;

			snprintf(pServerInfoTemp[3].szDDNSServerName, 64, "%s",  "www.no-ip.com");
			pServerInfoTemp[3].ucDDNSID = DDNS_NOIPDNS;
			pServerInfoTemp[3].supportproperty = NCFG_ENUM_DDNS_SUPPORT_DOMAIN1;

			snprintf(pServerInfoTemp[4].szDDNSServerName, 64, "%s",  "www.dvrdydns.com");
			pServerInfoTemp[4].ucDDNSID = DDNS_DVRDYDNS;
			pServerInfoTemp[4].supportproperty = NCFG_ENUM_DDNS_SUPPORT_DOMAIN1;

			snprintf(pServerInfoTemp[5].szDDNSServerName, 64, "%s",  "mintdns");
			pServerInfoTemp[5].ucDDNSID = DDNS_MINTDNS;
			pServerInfoTemp[5].supportproperty = NCFG_ENUM_DDNS_SUPPORT_DOMAIN1;


#if defined(__AUTOREG__)
			snprintf(pServerInfoTemp[6].szDDNSServerName, 64, "%s",  "www.autoddns.com");

			pServerInfoTemp[6].ucDDNSID = DDNS_MINT_AUTO;
			pServerInfoTemp[6].supportproperty = NCFG_ENUM_DDNS_SUPPORT_DOMAIN1;
#endif
			*pServerInfo = pServerInfoTemp;
		}
	}
	else if (DDNS_COMPANY_VN11 == company)
	{
		//国外产品
#if defined(__AUTOREG__)
		count = 7;
#else
		count = 6;
#endif

		DDNS_SERVER_INFO *pServerInfoTemp = new DDNS_SERVER_INFO [count];
		if (NULL == pServerInfoTemp)
		{
			count = 0;
		}
		else
		{
			snprintf(pServerInfoTemp[0].szDDNSServerName, 64, "%s",  "questek.tv");
			pServerInfoTemp[0].ucDDNSID = DDNS_VN11;
			pServerInfoTemp[0].supportproperty = NCFG_ENUM_DDNS_SUPPORT_DOMAIN1;

			snprintf(pServerInfoTemp[1].szDDNSServerName, 64, "%s",  "www.dyndns.com");
			pServerInfoTemp[1].ucDDNSID = DDNS_DYNDNS;
			pServerInfoTemp[1].supportproperty = NCFG_ENUM_DDNS_SUPPORT_DOMAIN1;

			snprintf(pServerInfoTemp[2].szDDNSServerName, 64, "%s",  "www.meibu.com");
			pServerInfoTemp[2].ucDDNSID = DDNS_MEIBU;
			pServerInfoTemp[3].supportproperty = 0;

			snprintf(pServerInfoTemp[3].szDDNSServerName, 64, "%s",  "www.no-ip.com");
			pServerInfoTemp[3].ucDDNSID = DDNS_NOIPDNS;
			pServerInfoTemp[3].supportproperty = NCFG_ENUM_DDNS_SUPPORT_DOMAIN1;

			snprintf(pServerInfoTemp[4].szDDNSServerName, 64, "%s",  "www.dvrdydns.com");
			pServerInfoTemp[4].ucDDNSID = DDNS_DVRDYDNS;
			pServerInfoTemp[4].supportproperty = NCFG_ENUM_DDNS_SUPPORT_DOMAIN1;

			snprintf(pServerInfoTemp[5].szDDNSServerName, 64, "%s",  "mintdns");
			pServerInfoTemp[5].ucDDNSID = DDNS_MINTDNS;
			pServerInfoTemp[5].supportproperty = NCFG_ENUM_DDNS_SUPPORT_DOMAIN1;

#if defined(__AUTOREG__)
			snprintf(pServerInfoTemp[6].szDDNSServerName, 64, "%s",  "www.autoddns.com");
			pServerInfoTemp[6].ucDDNSID = DDNS_MINT_AUTO;
			pServerInfoTemp[6].supportproperty = NCFG_ENUM_DDNS_SUPPORT_DOMAIN1;
#endif
			*pServerInfo = pServerInfoTemp;
		}
	}
	else
	{
		count = 0;
	}
	return true;
}






