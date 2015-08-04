#ifndef _READ_DDNS_H
#define _READ_DDNS_H

#include "NetConfigDefine.h"

class CReadDdns
{
public:
	
	//
	~CReadDdns();

	//
	static CReadDdns * Instance();

	//
	bool Initial( unsigned long subProductID);
	unsigned long GetCompany(){return (unsigned long)m_ddnsCmpny;}
	//
	void Quit();

	//
	int GetDddnsCount();

	//
	bool GetDddnsServerInfo(int index, DDNS_SERVER_INFO *pServerInfo);

protected:

private:

	typedef enum _ddns_company_
	{
		DDNS_COMPANY_US02	= 2,
		DDNS_COMPANY_US09_1 = 9,
		DDNS_COMPANY_US09_2 = 10,
		DDNS_COMPANY_US61	= 61,
		DDNS_COMPANY_BR27	= 27,
		DDNS_COMPANY_TW01	= 21,
		DDNS_COMPANY_TH13	= 13,
		DDNS_COMPANY_MX15	= 100,
		DDNS_COMPANY_VN11	= 101,
	}DDNS_COMPANY;

	//
	CReadDdns();

	//
	bool GetCompanyDdnsServer(unsigned long subProductID, unsigned long company, DDNS_SERVER_INFO **pServerInfo, int &count);
	
	//
	int             m_DdnsServerCount;

	//
	DDNS_SERVER_INFO   *m_pDdnsServer;	
	unsigned long	m_ddnsCmpny;
};
#endif
