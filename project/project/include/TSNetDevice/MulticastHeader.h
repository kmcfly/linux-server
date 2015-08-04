
#ifndef _MULTICAST_HEADER_
#define _MULTICAST_HEADER_

const unsigned long MULTICAST_PORT =23456;
const char * const MULTICAST_IPTOOL_SEND_IP   = "234.55.55.55";
const char * const MULTICAST_IPTOOL_RECV_IP   = "234.55.55.56";

const char * const MULTICAST_IP   = "234.55.55.55";

const unsigned long DEFAULT_LOCAL_PORT	= 9407;


#define mmioFOURCC1( ch0, ch1, ch2, ch3 ) \
	( (unsigned long)(unsigned char)(ch0) | ( (unsigned long)(unsigned char)(ch1) << 8 ) |	\
	( (unsigned long)(unsigned char)(ch2) << 16 ) | ( (unsigned long)(unsigned char)(ch3) << 24 ) )

const unsigned long FLAG_MULTIHEAD		=		mmioFOURCC1('M', 'H', 'E', 'D');

//0x10000-0x10001 设备名称编码由ANSI变为UTF8
//0x10001-0x10002 支持设置穿透版本序列号功能
//0x10002-0x10003 支持DNS信息功能及设置DNS、服务端口、HTTP端口、RTSP端口功能
//0x10003-0x10004 IPCamera从一个地址收组播，回复发到另外一个组播
//0x10004-0x10005 IPCamera增加了DHCP和自动搜索的配置
//0x10005-0x10006 增加了修改IP有回复
//0x10006-0x10007	增加了搜索设备会回复产品型号、默认制式、客户版本三个信息、组播回复还有表示设备的随机数,
//					增加了结构体 MULTICAST_INFO_V14
//					设置mac地址增加了回复
const unsigned long MULTICAST_VER			= 0x10000;
const unsigned long	MULTICAST_10002_VERSION = 0x10002;
const unsigned long	MULTICAST_10003_VERSION = 0x10003;
const unsigned long	MULTICAST_10004_VERSION = 0x10004;
const unsigned long	MULTICAST_10005_VERSION = 0x10005;
const unsigned long	MULTICAST_10006_VERSION = 0x10006;
const unsigned long	MULTICAST_10007_VERSION = 0x10007;
const unsigned long MULTICAST_CURRENT_VERSION	= MULTICAST_10007_VERSION;

const unsigned long DEVICE_NAME_MAX_LEN_MULTI	= 20;		//设备名最大长度		
const unsigned long MULTICAST_CMD_SEARCH = 1; //由搜索器发出的搜索指令
const unsigned long MULTICAST_CMD_INFO	  =2; //由DVS发出的响应指令，发送自己的信息
const unsigned long MULTICAST_CMD_SET	  =3; //由搜索器发出的设置指令，设置IP地址及设备名称
const unsigned long MULTICAST_CMD_SET_MAC =4; //由搜索器发出的设置指令，设置MAC地址
const unsigned long MULTICAST_CMD_SET_DEFAULT_VIDEOFORMAT = 5; //由搜索器发出的设置指令，设置默认制式（IPCamera制式出场后固定）
const unsigned long MULTICAST_CMD_SET_DEV_VER=6;//由搜索器发出的设置指令，设置设备硬件版本号
const unsigned long MULTICAST_CMD_SET_CUSTOMERID=7;//由搜索器发出的设置指令，设置订制客户号，使用inputform值，具体值参考网络协议中的定义
const unsigned long MULTICAST_CMD_SET_NATSERVER=8;
const unsigned long MULTICAST_CMD_SET_SEQUENCE_NO=9;//设置序列号
const unsigned long MULTICAST_CMD_SET_AUTOSEARCH_SERVER=10;//自动搜索路由器时使用的服务器的地址
//huxinhua add begin
const unsigned long MULTICAST_CMD_SET_IPC_TYPE = 11; //设置IPC的型号
//huxinhua add end
const unsigned long MULTICAST_CMD_SET_RESET_9411 = 12; //9411恢复出厂设置

const unsigned long MULTICAST_CMD_SEARCH_ONE_DEVICE = 13; // 通过别名或者序号搜索指定的设备
const unsigned long MULTICAST_CMD_SEARCH_ONE_DEVICE_REPLY = 14;

const unsigned long MULTICAST_CMD_SET_LANGUAGE = 15;

const unsigned long MULTICAST_CMD_SET_REPLY = 16;	//设置IP地址的回复

const unsigned long MULTICAST_CMS_SET_IPC_ATTRIBUTES = 17;

const unsigned long MULTICAST_CMS_SET_IPC_PRODUCT_MANUFACTURERY = 18;

typedef struct MultiCastHead
{
	ULONG			head;		//是FLAG_MULTIHEAD
	ULONG			ver;		//结构信息版本
	ULONG			cmd;
}MULTICASTHEAD;

typedef struct MultiCastInfo
{	
	MULTICASTHEAD			mulHead;
	char			DevName[DEVICE_NAME_MAX_LEN_MULTI];//如果此编码格式为UTF8请将版本号设置为0x10001
	BYTE			MacAddr[6];
	unsigned short	netport;
	ULONG			ipaddr;
	ULONG			netmask;
	ULONG			route;	
	unsigned long	softver;			
	unsigned long	softbuilddate;	
	unsigned short	nHttpPort;
	char			NoUsed[2];
	unsigned long	deviceType;
	unsigned long	productType;	
	unsigned long	kernelVersion;
	unsigned long	VideoFormat;
	unsigned long	devVer;//设备硬件版本号 
	char			szPasswd[28];//设置IP必须输入正确的admin密码，采用base64编码
	BYTE			Resved[28];//以后增加功能就利用这些字节
}MULTICAST_INFO;

struct  MULTI_SET_MAC
{
	MULTICASTHEAD	mulHead;
	//这个DevName没用，只是为了兼容以前的
	char			DevName[DEVICE_NAME_MAX_LEN_MULTI];//如果此编码格式为UTF8请将版本号设置为0x10001
	BYTE			dstMac[6];
	unsigned char	noused[2];
	BYTE			srcMac[6];
	unsigned char	noused1[126];
};

typedef struct MultiCastInfo_v13
{
	MULTICASTHEAD			mulHead;
	char			DevName[DEVICE_NAME_MAX_LEN_MULTI];//如果此编码格式为UTF8请将版本号设置为0x10001
	BYTE			MacAddr[6];
	unsigned short	netport;
	ULONG			ipaddr;
	ULONG			netmask;
	ULONG			route;	
	unsigned long	softver;			
	unsigned long	softbuilddate;	
	unsigned short	nHttpPort;
	unsigned short	nRTSPPort;
	unsigned long	deviceType;
	unsigned long	productType;	
	unsigned long	kernelVersion;
	unsigned long	VideoFormat;		//视频制式
	unsigned long	devVer;				//设备硬件版本号 
	char			szPasswd[28];		//设置IP必须输入正确的admin密码，采用base64编码
	unsigned long	dwDNSServer1;		//静态IP设定时的DNS地址1
	unsigned long	dwDNSServer2;		//静态IP设定时的DNS地址2
	unsigned long	dwIPCameraNo;		//IPCamera序列号
	BYTE			bTraversOnline;		//IPCamera想穿透服务器注册上了	
	BYTE			ucIsWIFI;			//0 有线 1 wifi
	BYTE			ucStartAutoSearch;	//0 没有开自动搜索 1 开了自动搜索
	BYTE			ucSupportAutoSearch; //0 不支持自动搜索 1 支持自动搜索	
	unsigned long	dwSecondIP;
	unsigned long	dwSecondMask;
	unsigned char   ucDhcpcdStatus;		// 回复状态时：0 未启动 1 正在启动 2 成功 3 失败
										// IPTOOL 设置IPCamera时： 0 关闭 1开启
	unsigned char	ucLanguage;
	unsigned char   ucIPMode;			//0 静态IP 1 dhcp
	unsigned char	ucStartPPPOE;		//0 未开启 1 已开启 pppoe拨号	
}MULTICAST_INFO_V13;


typedef struct MultiCastInfo_v14
{
	MULTICASTHEAD			mulHead;
	char			DevName[DEVICE_NAME_MAX_LEN_MULTI];//如果此编码格式为UTF8请将版本号设置为0x10001
	BYTE			MacAddr[6];
	unsigned short	netport;
	ULONG			ipaddr;
	ULONG			netmask;
	ULONG			route;	
	unsigned long	softver;			
	unsigned long	softbuilddate;	
	unsigned short	nHttpPort;
	unsigned short	nRTSPPort;
	unsigned long	deviceType;
	unsigned long	productType;	
	unsigned long	kernelVersion;
	unsigned long	VideoFormat;		//视频制式
	unsigned long	devVer;				//设备硬件版本号 
	char			szPasswd[28];		//设置IP必须输入正确的admin密码，采用base64编码
	unsigned long	dwDNSServer1;		//静态IP设定时的DNS地址1
	unsigned long	dwDNSServer2;		//静态IP设定时的DNS地址2
	unsigned long	dwIPCameraNo;		//IPCamera序列号
	BYTE			bTraversOnline;		//IPCamera想穿透服务器注册上了	
	BYTE			ucIsWIFI;			//0 有线 1 wifi
	BYTE			ucStartAutoSearch;	//0 没有开自动搜索 1 开了自动搜索
	BYTE			ucSupportAutoSearch; //0 不支持自动搜索 1 支持自动搜索	
	unsigned long	dwSecondIP;
	unsigned long	dwSecondMask;
	unsigned char   ucDhcpcdStatus;		// 回复状态时：0 未启动 1 正在启动 2 成功 3 失败
										// IPTOOL 设置IPCamera时： 0 关闭 1开启
	unsigned char	ucLanguage;
	unsigned char   ucIPMode;			//0 静态IP 1 dhcp
	unsigned char	ucStartPPPOE;		//0 未开启 1 已开启 pppoe拨号

	char			szProduct[24];
	char			szCustomer[24];
	char			defaultInputNorm;
	char			softReset;			//1 需要软件复位
	char			noused1[2];
	int				iRand;
    
    char            szProductType[16];//   以下两个是给NVR显示的
    char            szManufacturery[16]; //    
	char			noused2[12];
}MULTICAST_INFO_V14;

/********************	设置NATSERVER IP  ********************/
typedef struct Multi_NatServer
{
	bool			bUsed;		//是否有填服务器地址
	bool			noused;		//没用
	unsigned short	nPort;
	char			szServer[64];//Nat服务器的地址	
}MULTI_NATSERVER;

// MULTICAST_CMD_SET_REPLY
typedef struct MultiSetIpReply
{
	MULTICASTHEAD		mulHead;
	DWORD				ipAddress;
	unsigned short		nPort;
	unsigned short		nResult; //0 成功 1 失败
	BYTE				Resved[120];
}MULTI_SETIP_REPLY;

typedef struct Multi_NatServer_Info
{
	unsigned long		dwReLoginTime;
	MULTI_NATSERVER		natServer[10];	
}MULTI_NATSERVER_INFO;

typedef struct MultiSetNatServer
{
	MULTICASTHEAD			mulHead;
	BYTE					MacAddr[8];	//指定修改谁的NatServer
	MULTI_NATSERVER_INFO	mulNatServerInfo;
}MULTI_SETNATSERVER;
/********************	设置NATSERVER IP  ********************/

/********************	设置 IPC 序列号  ********************/
typedef struct MultiSetSeqNo
{
	MULTICASTHEAD			mulHead;
	BYTE					MacAddr[8];	//指定修改谁的序列号
	unsigned long			dwIPCameraNo;
}MULTI_SET_SEQ_NO;
/********************	设置 IPC 序列号  ********************/

/********************	设置 自动搜索路由器的服务器  ********************/
struct MULTI_AUTO_SEARCH_SERVER
{
	bool	bUsed;
	char	noused;
	unsigned short	nPort;
	unsigned long	dwIP;
};

struct MULTI_SET_AUTO_SEARCH_SERVER
{
	MULTICASTHEAD			mulHead;
	BYTE					MacAddr[8];	//指定修改谁的NatServer
	MULTI_AUTO_SEARCH_SERVER	servers[10];
};
/********************	设置 自动搜索路由器的服务器   ********************/

/********************	9411 恢复出厂设置  ********************/
struct MULTI_SET_RESET_9411	
{
	MULTICASTHEAD			mulHead;
	char					password[16];
};

struct MULTI_SET_RESET
{
	MULTICASTHEAD			mulHead;
	char					password[16];
	unsigned char			szMac[8];
};
/********************	9411 恢复出厂设置  ********************/

//MULTICAST_CMD_SEARCH_ONE_DEVICE
struct MULTICAST_INFO_ONE_DEVICE
{
	MULTICASTHEAD	mulHead;
	char			szSeqOrAlias[32];
};

//MULTICAST_CMD_SEARCH_ONE_DEVICE_REPLY
struct MULTICAST_INFO_ONE_DEVICE_REPLY
{
	MULTICASTHEAD			mulHead;
	char			szSeqOrAlias[32];
	char			DevName[DEVICE_NAME_MAX_LEN_MULTI];//如果此编码格式为UTF8请将版本号设置为0x10001
	BYTE			MacAddr[6];
	unsigned short	netport;
	ULONG			ipaddr;
	ULONG			netmask;
	ULONG			route;	
	unsigned long	softver;			
	unsigned long	softbuilddate;	
	unsigned short	nHttpPort;
	unsigned short	nRTSPPort;
	unsigned long	deviceType;
	unsigned long	productType;	
	unsigned long	kernelVersion;
	unsigned long	VideoFormat;		//视频制式
	unsigned long	devVer;				//设备硬件版本号 
	char			szPasswd[28];		//设置IP必须输入正确的admin密码，采用base64编码
	unsigned long	dwDNSServer1;		//静态IP设定时的DNS地址1
	unsigned long	dwDNSServer2;		//静态IP设定时的DNS地址2
	unsigned long	dwIPCameraNo;		//IPCamera序列号
	BYTE			bTraversOnline;		//IPCamera想穿透服务器注册上了	
	BYTE			ucIsWIFI;			//0 有线 1 wifi 10 没有从IP
	BYTE			ucStartAutoSearch;	//0 没有开自动搜索 1 开了自动搜索
	BYTE			Resved1[1];			//以后增加功能就利用这些字节
	unsigned long	dwSecondIP;
	unsigned long	dwSecondMask;
	unsigned char   ucDhcpcdStatus;		// 回复状态时：0 未启动 1 正在启动 2 成功 3 失败
										// IPTOOL 设置IPCamera时： 0 关闭 1开启
	unsigned char	ucLanguage;
	BYTE			Resved2[2];			//以后增加功能就利用这些字节
};

//MULTICAST_CMD_SET_LANGUAGE
struct MULTI_SET_LANGUAE
{
	MULTICASTHEAD		mulHead;
	unsigned long		dwLanguage;	 //支持哪些语言 0 简体中文 + 英文  1 繁体中文 + 英文
};

//MULTICAST_CMS_SET_IPC_ATTRIBUTES
struct IPC_ATTRIBUTES
{
	MULTICASTHEAD		mulHead;
	unsigned long		dwDefaultInput;
	unsigned long		dwProductType;
	unsigned long		dwCustomerType;
	unsigned long		dwLanguage;
	unsigned long		noused[50];
};

#endif //_MULTICAST_HEADER_

