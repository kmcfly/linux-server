#include "DdnsCNB.h"
#include "ddnscli.h"

CDdnsCNB::CDdnsCNB()
{
	m_nWebPort = 0;
	m_nSessionPort = 0;
	m_nProductType = 2;				//当前默认为 2;
	GetDeviceName(CProduct::Instance()->ProductType(), m_sModel);
	CProduct::Instance()->GetFirmwareVersion(m_sSoftVer, sizeof(m_sSoftVer));
	
	DDNS_InitDDNSInfo(m_nProductType,m_sModel,m_sSoftVer);
	DDNS_SetDDNSServer("autoipset.com");
}

CDdnsCNB::~CDdnsCNB()
{
	DDNS_ClearDDNSInfo();
}

unsigned long CDdnsCNB::DdnsIPupdate(const char *pUserName, const char *pPassword, const char *pmyDomainName )
{
	int nError = DDNS_UpdateDDNSInfo();

	return GetLocalError((DDNS_ERROR_NO)nError);
}

unsigned long CDdnsCNB::DdnsAutoReg(char * pUser, char * pPsw, char *pmyDomainName)
{
	RemoveDDNS();

	int iError = DDNS_RegisterDDNSInfo(pmyDomainName,pUser,pPsw, m_nWebPort, m_nSessionPort);
	
	return GetLocalError((DDNS_ERROR_NO)iError);
}

bool CDdnsCNB::RemoveDDNS()
{
	DDNS_RemoveDDNSInfo();
}

int CDdnsCNB::GetLocalError(DDNS_ERROR_NO cnbErrorNo)
{
	switch (cnbErrorNo)
	{
	case DE_SUCCESS://			=0,		//己傍
	return DDNS_IPUPDATE_OK;
	case DE_QUERY_FAIL://		=1,		//DB Query Failed
	return DDNS_IPUPDATE_EUNKNOWN;
	case DE_NO_USER://			=2,		//Not found user
	return DDNS_IPUPDATE_ENAMEPWD;
	case DE_INSERT_DEVICE://	=3,		//Failed to insert device
	return DDNS_IPUPDATE_EUNKNOWN;
	case DE_UPDATE_DEVICE://	=4,		//Failed to update ff
	return DDNS_IPUPDATE_EUNKNOWN;
	case DE_INSERT_RR://		=5,		//Failed to insert rr
	return DDNS_IPUPDATE_EUNKNOWN;
	case DE_UPDATE_RR://		=6,		//Failed to update rr
	return DDNS_IPUPDATE_EUNKNOWN;
	case DE_INSERT_SOA://		=7,		//Failed to insert soa
	return DDNS_IPUPDATE_EUNKNOWN;
	case DE_UPDATE_SOA://		=8,		//Failed to update soa
	return DDNS_IPUPDATE_EUNKNOWN;
	case DE_INSERT_USER://		=9,		//Failed to insert user
	return DDNS_IPUPDATE_EUNKNOWN;
	case DE_UPDATE_USER://		=10,	//Failed to update user
	return DDNS_IPUPDATE_EUNKNOWN;
	case DE_ALREADY_HOST://		=11,	//Already registered hostname
	return DDNS_AUTOREG_DOMAIN_ALREADY_USE;
	case DE_REMOVE_RR://		=12,	//Failed to remove ff
	return DDNS_IPUPDATE_EUNKNOWN;
	case DE_REMOVE_DEVICE://	=13,	//Failed to remove device
	return DDNS_IPUPDATE_EUNKNOWN;
	case DE_NO_MACADDRESS://	=14,	//Not found Macadddress
	return DDNS_IPUPDATE_EUNKNOWN;
	case DE_NULL_HOSTNAME://	=15,	//Hostname is empty.
	return DDNS_IPUPDATE_EHOSTDOMAIN_EMPTY;
	case DE_DISCONNECTED://		=16,	//Internet was disconnected.
	return DDNS_IPUPDATE_ENETUNREACHD;
	case DE_NOT_FOUND_ADAPTER://=17,	//couldn't find the ethernet device
	return DDNS_IPUPDATE_ENETUNREACHD;
	case DE_NOT_SERVER://		=18,	//need to set server ip address or domain.
	return DDNS_IPUPDATE_ENETUNREACHD;
	case DE_FAILED_HOSTBYNAME://=19,	//Failed to get host by name.
	return DDNS_IPUPDATE_ENETUNREACHD;
	case DE_FAILED_SEND://		=20,	//Failed to send data.
	return DDNS_IPUPDATE_ENETUNREACHD;
	case DE_NOT_ALPHABET_NUM://	=21,	//Hostname could be used the Alphabet & numeral.
	return DDNS_IPUPDATE_EHOSTDOMAIN;
	case DE_PW_DENY_CHAR://		=22,	// !,#,<,>,|,&,\n,\t,' ' can not use ascii.
	return DDNS_IPUPDATE_EUNKNOWN;
	case DE_UPDATE_AFTER_5MIN://	=23	// have to update after 5 min.
	return DDNS_IPUPDATE_EUNKNOWN;
	default:
	return DDNS_IPUPDATE_EUNKNOWN;
	}
}

void CDdnsCNB::GetDeviceName(PRODUCT_TYPE productType, char * pDeviceName)
{
	switch (productType)
	{
	case TD_2704TS_PL:
		strcpy(pDeviceName, "RTC-041");
		break;
	case TD_2708TS_PL:
		strcpy(pDeviceName, "RTC-081");
		break;
	case TD_2716TS_PL:
		strcpy(pDeviceName, "RTC-162");
		break;
	default:
		strcpy(pDeviceName, "SDVR-");
		break;
	}
}

