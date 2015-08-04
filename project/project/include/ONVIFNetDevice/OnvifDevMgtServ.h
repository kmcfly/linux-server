/*******************************************************************************
** Copyright (c) Tongwei Video Technology Co.,Ltd. All rights reserved.				
** AUTHOR		:	骆名猛	
** DATE			:	2013-02-21
** FILENAME		:	OnvifDevMgtServ.h
** Version		:	ONVIF 2.1
** DESCRIPTION	:
** Modify Record:
*******************************************************************************/
#ifndef _OnvifDevMgtServ_H_
#define _OnvifDevMgtServ_H_

#include "soapServ.h"

const int  DEVICE_MGT_STR_LEN = 128;
#define ONVIF_TIMEZONE_SET_NORMAL_MODE 0
#define ONVIF_TIMEZONE_SET_DAHUA_MODE 1

//enum declare
enum EOnvifSetDateTimeType
{
	TSetManual = 0, 
	TSetNTP
};

enum EOnvifUserLevel 
{
	TAdministrator = 0, 
	TOperator = 1, 
	TUser = 2, 
	TAnonymous = 3, 
	TExtended = 4
};

//struct
struct OnvifDevInfo
{
	char Manufacturer[64];	/* SOAP 1.2 RPC return element (when namespace qualified) */	/* required element of type string */
	char Model[64];	/* required element of type string */
	char FirmwareVersion[64];	/* required element of type string */
	char SerialNumber[64];	/* required element of type string */
	char HardwareId[64];	/* required element of type string */
};

struct OnvifDevTime
{
	int Hour;	/* required element of type xsd:int */
	int Minute;	/* required element of type xsd:int */
	int Second;	/* required element of type xsd:int */
};
struct OnvifDevDate
{
	int Year;	/* required element of type xsd:int */
	int Month;	/* required element of type xsd:int */
	int Day;	/* required element of type xsd:int */
};

struct OnvifDateTime
{
   enum EOnvifSetDateTimeType dateTimeType;
	bool                     bDaylightSave;
	char                     timeZone[128]; //options
	struct OnvifDevDate       date;
	struct OnvifDevTime       time;
    
	//EOnvifSetDateTimeType     setStatus;

	//TimeZone table and DST tab
	char   TZTab[64];
	char   DSTTab[64];

	//the delta time from local time to UTC time
	int deltaHour;
	int deltaMinute;
	bool bBehindUtc;

	//day light saving period
	bool             bWeekType;//day light saving type:false is the date type,else is week type
	int              tOffset; //delta time
	OnvifDevDate      beginDate;
	OnvifDevTime      beginTime;
	OnvifDevDate      endDate;
	OnvifDevTime      endTime;

	unsigned int  beginWeekIndex;	//第几周进入DST（周方式有效）
	unsigned char beginWday;		//星期几进入DST（周方式有效）
	unsigned char endWeekIndex;	//第几周出DST（周方式有效）
	unsigned char endWday;		//星期几出DST（周方式有效）

	//day light saving buf
	char   dstInfo[96];
};

//user
struct OnvifUserInfo
{
	char Username[DEVICE_MGT_STR_LEN];	/* required element of type xsd:string */
	char Password[DEVICE_MGT_STR_LEN];	/* optional element of type xsd:string */
	enum EOnvifUserLevel  UserLevel;	/* required element of type tt:UserLevel */	
};

struct OnvifUsersList
{
	int sizeUser;	/* SOAP 1.2 RPC return element (when namespace qualified) */	/* sequence of elements <User> */
	struct OnvifUserInfo *User;	/* optional element of type tt:User */
};

struct OnvifDeleteUsers
{
	int sizeUsername;	/* sequence of elements <Username> */
	char **Username;	/* required element of type xsd:string */
};

//network
enum EOnvifIPSetType
{
	TIPManual= 0,
	TIPLinkLocal=1,
	TIPFromDHCP=2,
	TIPFromRA = 3,
	TIPEnd
};

enum EOnvifIPv6DHCPConf 
{
	TDHCPAuto = 0, 
	TDHCPStateful = 1, 
	TIPv6DHCPStateless = 2, 
	TIPv6DHCPOff = 3,
	TIPv6DHCP_End
};

enum EOnvifIPTYPE
{
	TManualType =0,
	TDhcpType,
	TPppoeType,
	TOtherType
};

enum EOnvifNtpType
{
	TIpv4=0,
	TIpv6=1,
	TDNSName = 2,
	TEnd
};

enum EOnvifDynDNSType
{
	TDDNSNoUpdate = 0, 
	TDDNSClientUpdates, 
	TDDNSServerUpdates
};

enum EOnvifIPAddrFilter 
{
	FilterType__Allow = 0, 
	FilterType__Deny = 1
};

struct OnvifIpv4Configuration
{
  char ipAddr[20];
  int  prefixLength;
  enum EOnvifIPSetType ipSetType;
  bool dhcp;
};

struct OnvifIpv6Configuration
{
	enum EOnvifIPv6DHCPConf ipv6DhcpConf;
	char ipv6Addr[48];
	int  prefixLength;
    enum EOnvifIPSetType ipSetType;  
};

struct OnvifDevIpAddr
{
  char hardwareAddr[DEVICE_MGT_STR_LEN];//used in get the ip address case.
  bool ipv4Enable;
  struct OnvifIpv4Configuration ipv4;
  bool ipv6Enable;
  struct OnvifIpv6Configuration ipv6;
};

struct OnvifIpv4Set
{
	char Addr[16];
	int prefixLength;
	bool bDhcp;
};

struct OnvifIpv6Set
{
	bool acceptRouterAdvert;
	enum EOnvifIPv6DHCPConf dhcpType;
	bool bManual;
	char Addr[48];
	int prefixlength;
};

struct OnvifIpAddrSet
{
	bool ipv4Enable;
	struct OnvifIpv4Set ipv4Set;
	bool ipv6Enable;
	struct OnvifIpv6Set ipv6Set;
};

struct OnvifProtocolProt
{
	bool bHttpEnable;
	int  httpPort;
	bool bHttpsEnable;
	int  httpsPort;
    bool bRtspEnable;
	int  rtspPort;
};

struct OnvifIpAddr
{
	bool ipv4Type; //ipv4 or ipv6
	char ipAddress[48];
};
struct OnvifDnsData
{
	enum EOnvifIPTYPE ipsource;//DHCP or manual or others
	int dnsIpSize;
	struct OnvifIpAddr *dnsIp;
};

struct OnvifGateWayIP
{
	bool ipv4Enable;
	char ipv4[16];
	bool ipv6Enable;
	char ipv6[48];
};


struct OnvifNTPAddr
{
	EOnvifNtpType type;//ipv4 ,ipv6 or DNS Name.0 is ipv4,1 is ipv6 ,2 is DNSName
	union{
		struct OnvifIpAddr ipAddr;
		char DNSName[256];
	};
};

struct OnvifNTPData
{
	bool FromDHCP;
	int addrSize;
	struct OnvifNTPAddr *addr;
};


struct OnvifDDnsStruc
{
	enum EOnvifDynDNSType Type;	/* required element of type tt:DynamicDNSType */
	char Name[60];	/* optional element of type tt:DNSName */
	char TTL[40];	/* optional element of type xsd:string */
};

struct OnvifNwParameter
{
	bool   ipSuccess;
	struct OnvifDevIpAddr      ipAddr;
	//bool   portSuccess;
	//struct OnvifProtocolProt   portData;
	bool   dnsSuccess;
	struct OnvifDnsData        dnsData;
	bool   gatewaySuccess;
	struct OnvifGateWayIP      gatewayData;
};


struct OnvifIpFilter
{
	int prefixlength;
	char ip[64];
};

struct OnvifIpAddrFilter
{
	//boolean ipv4Type;
	enum EOnvifIPAddrFilter filterType;
	int ipv4Size;
	struct OnvifIpFilter *ipv4Addr;
	int ipv6Size;
	struct OnvifIpFilter *ipv6Addr;
};

enum EOnvifCapabilityType
{
	TAll = 0,
	TDeviceMgt,
	TEvents,
	TImaging,
	TMedia,
	TPTZ,	
	TAnalytics,
	TCapaEnd
};

struct OnvifDevCapaServ 
{
	bool bDevMgtEnable;
	char devMgtServ[DEVICE_MGT_STR_LEN];
	bool bEventEnable;
	char eventsServ[DEVICE_MGT_STR_LEN];
    bool bImagingEnable;
	char imagingServ[DEVICE_MGT_STR_LEN];
	bool bMediaEnable;
	char mediaServ[DEVICE_MGT_STR_LEN];
	int  maxMediaProfileNum;
	bool bPTZEnable;
	char PTZServ[DEVICE_MGT_STR_LEN];
	bool bAnalyticsEnable;
	char analyticsServ[DEVICE_MGT_STR_LEN];
};

class COnvifDevMgt
{
public:
  COnvifDevMgt(void);
  ~COnvifDevMgt(void);

  //after device discover,get device capability through this function
  //this function not need username and password
  //in:device service address
  //in:device capability type
  //out:device capability service address,such as:device management,imaging,media,PTZ,event.
  int GetDevCapability(char *aDevServAddr,enum EOnvifCapabilityType capabilityType,struct OnvifDevCapaServ *capaResp,unsigned int aDigest);
  int GetDevUtcTimeCapability(char *aDevServAddr,enum EOnvifCapabilityType capabilityType,struct OnvifDevCapaServ *capaResp,unsigned int aDigest);

  //register device
  bool RegisterUser(char *deviceMgtServ,char *userName,char *password,unsigned int aDigest);

  //system manage
  void InitDevMgtParameter(char *aServAddr,char *aUserName,char *aPassKey); 
  void InitDevUtcTimes( bool bClearUtcTime, char* aServAddr, unsigned int ipcCtrlType );
  void GetExistUtcTm(ONVIF_PUBLIC_UTC_TIME* pstUtc)
		{memcpy( pstUtc, &stOnvifInputParam.stDevUtcTime, sizeof(ONVIF_PUBLIC_UTC_TIME) );}


  int GetDevInfo(struct OnvifDevInfo *aDevInfoResp,unsigned int aDigest);

  int GetDevDateTime(struct OnvifDateTime *dateTimeResp,unsigned int aDigest);
  int SetDevDateTime(struct OnvifDateTime *dateTimeSet,unsigned int aDigest, unsigned int ipcManuID );

  int FormatTimeZoneString(char* timeBuf, struct OnvifDateTime *dateTimeSet, unsigned int ipcManuID);
  
  int SetDstActive(struct OnvifDateTime *dateTimeSet,unsigned int aDigest, unsigned int ipcManuID);
  int SetNtpActive(struct OnvifDateTime *dateTimeSet,unsigned int aDigest, unsigned int ipcManuID);

  int RebootDev(char *devResp,unsigned int aDigest);
 
  int SetDevFactoryDefault(bool bSoftDefault,unsigned int aDigest);
  
  //user
  int GetDevUsers(struct OnvifUsersList *usersResp,unsigned int aDigest);
  int AddUser(struct OnvifUsersList *addUsers,unsigned int aDigest);
  int DeleteUser(struct OnvifDeleteUsers *delUsers,unsigned int aDigest);
  int SetUserCode(struct OnvifUsersList *setUsers,unsigned int aDigest);
  // void SetUserName(void);

  //network
  void GetNetworkParameter(struct OnvifNwParameter *nwParameterResp,unsigned int aDigest);
  void SetNetworkParameter(struct OnvifNwParameter *nwParameterSet,unsigned int aDigest);
   
  int GetDevPort(struct OnvifProtocolProt *getPortParam,unsigned int aDigest);
  int SetDevPort(struct OnvifProtocolProt *portSetParam,unsigned int aDigest);

  int GetNtpSetting(struct OnvifNTPData *getNtpData,unsigned int aDigest);
  int SetNtpSetting(struct OnvifNTPData *setNtpData,unsigned int aDigest);
 
  int GetDynamicDnsSetting(struct OnvifDDnsStruc *getDDnsData,unsigned int aDigest);
  int SetDynamicDnsSetting(struct OnvifDDnsStruc *setDDnsData,unsigned int aDigest);

  int GetHostName(char *hostNameParam,int size,unsigned int aDigest);
  int SetHostName(char *hostNameParam,unsigned int aDigest);
  int SetHostNameDHCP(bool fromDhcpParam,unsigned int aDigest);

  int GetIpAddressFilter(struct OnvifIpAddrFilter *ipListParam,unsigned int aDigest);
  int SetIpAddressFilter(struct OnvifIpAddrFilter *setFilter,unsigned int aDigest);
  int AddIpAddressFilter(struct OnvifIpAddrFilter *addIpFilterParam,unsigned int aDigest);
  int RemoveIpAddressFilter(struct OnvifIpAddrFilter *removeFilter,unsigned int aDigest);

protected:
  int GetDevIpAddr(struct OnvifDevIpAddr *devIpResp,unsigned int aDigest);
  int GetDevDns(struct OnvifDnsData *dnsResp,unsigned int aDigest);
  int GetDefaultGateway(struct OnvifGateWayIP *gatewayResp,unsigned int aDigest);

  int SetDevIpAddr(struct OnvifDevIpAddr *setParam,unsigned int aDigest);
  int SetDevDns(struct OnvifDnsData *setDnsParam,unsigned int aDigest);
  int SetDefaultGateway(char *gateWayIp,bool ipv4Add,unsigned int aDigest);

  void GetAppUserFromDevice(struct OnvifUserInfo *appUser,struct tt__User *devUser);
  void GetDevUserFromApp(struct tt__User *devUser,struct OnvifUserInfo *appUser);
  void InitDevFilterAddr(struct tt__IPAddressFilter *devFilter,struct OnvifIpAddrFilter *appFilter);
  
  //transform time
  void LocalToUTC(OnvifDateTime *localTime,struct OnvifDevDate &UTCDate,struct OnvifDevTime &UTCTime);
  void UTCToLocal(OnvifDateTime *localTime,struct OnvifDevDate &UTCDate,struct OnvifDevTime &UTCTime);

  //time transform
  bool   leap(int year);
  void   SubtractTime(OnvifDateTime *Time,struct OnvifDevDate &UTCDate,struct OnvifDevTime &UTCTime);
  void   AddTime(OnvifDateTime *Time,struct OnvifDevDate &UTCDate,struct OnvifDevTime &UTCTime);
private:


	char m_servAddr[DEVICE_MGT_STR_LEN];
	char m_userName[32];
	char m_passKey[32];
   
	//char m_timeZone[DEVICE_MGT_STR_LEN*2];
   
	//users

	//network
   char m_ipSetToken[DEVICE_MGT_STR_LEN];

   ONVIF_INIT_INPUT_PARAM stOnvifInputParam;

};


#endif