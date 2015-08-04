//ddnscli.h
#ifndef __DDNS_CLI_H__
#define __DDNS_CLI_H__

#define UPDATE_INTERVAL		 300000 // 5 minutes 
#define PING_INTERVAL		 60000  // 60 seconds

enum  PRODUCT_TYPE_CNB{
	PT_NONE			=0,
	PT_PC_DVR		=1,		//PC-DVR
	PT_STD_DVR		=2,		//STAND-ALONE DVR
	PT_IP_CAMERA		=3,		//IP Camera
	PT_IP_SERVER		=4,		//IP Server
}; 

enum DDNS_ERROR_NO{
	DE_SUCCESS			=0,		//¼º°ø
	DE_QUERY_FAIL		=1,		//DB Query Failed
	DE_NO_USER			=2,		//Not found user
	DE_INSERT_DEVICE	=3,		//Failed to insert device
	DE_UPDATE_DEVICE	=4,		//Failed to update ff
	DE_INSERT_RR		=5,		//Failed to insert rr
	DE_UPDATE_RR		=6,		//Failed to update rr
	DE_INSERT_SOA		=7,		//Failed to insert soa
	DE_UPDATE_SOA		=8,		//Failed to update soa
	DE_INSERT_USER		=9,		//Failed to insert user
	DE_UPDATE_USER		=10,	//Failed to update user
	DE_ALREADY_HOST		=11,	//Already registered hostname
	DE_REMOVE_RR		=12,	//Failed to remove ff
	DE_REMOVE_DEVICE	=13,	//Failed to remove device
	DE_NO_MACADDRESS	=14,	//Not found Macadddress
	DE_NULL_HOSTNAME	=15,	//Hostname is empty.
	DE_DISCONNECTED		=16,	//Internet was disconnected.
	DE_NOT_FOUND_ADAPTER=17,	//couldn't find the ethernet device
	DE_NOT_SERVER		=18,	//need to set server ip address or domain.
	DE_FAILED_HOSTBYNAME=19,	//Failed to get host by name.
	DE_FAILED_SEND		=20,	//Failed to send data.
	DE_NOT_ALPHABET_NUM	=21,	//Hostname could be used the Alphabet & numeral.
	DE_PW_DENY_CHAR		=22,	// !,#,<,>,|,&,\n,\t,' ' can not use ascii.
	DE_UPDATE_AFTER_5MIN	=23	// have to update after 5 min.
};



#define SERVER_MAX_NUMS		1
static char *g_szServerList[]={
	"autoipset.com",
};

// Setting the default information for registration device.
// 1. nProductType have to refer PRODUCT_TYPE enum.
// 2. szModel is a model name.
// 3. szSoftVer is Firmware version or Software version.
int DDNS_InitDDNSInfo(int nProductType, char *szModel,char *szSoftVer);

// Setting server ip address or domain.
//1. the szServer is ddns server's ip address or domain.
int DDNS_SetDDNSServer(char *szServer);

// Registration Hostname
// 1. szHostname is hostname.
// 2. szUserID is user id for management hostname.
// 3. szPassword is user's password.
// 4. nWebPort is port number for web page.
// 5. nSessionPort is a etc port.
// If szUserID is NULL, user id shall be changed 'anonymous'.
int DDNS_RegisterDDNSInfo(char *szHostName, char *szUserID, char *szPassword, int nWebPort,int nSessionPort);

// update Ip by periods.
// the minmum is 5 minutes.
// you have to call this function to offer the latest information.
int DDNS_UpdateDDNSInfo();		

// Remove hostname on ddns server.
int DDNS_RemoveDDNSInfo();

// clear setting information.
void DDNS_ClearDDNSInfo();

#endif

