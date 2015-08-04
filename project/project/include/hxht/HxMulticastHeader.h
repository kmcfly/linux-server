
#ifndef _MULTICAST_HEADER_
#define _MULTICAST_HEADER_

const unsigned long MULTICAST_PORT =23456;
const char * const MULTICAST_IP   ="234.55.55.55";

const unsigned long DEFAULT_LOCAL_PORT	=7838;

#define mmioFOURCC1( ch0, ch1, ch2, ch3 ) \
	( (unsigned long)(unsigned char)(ch0) | ( (unsigned long)(unsigned char)(ch1) << 8 ) |	\
	( (unsigned long)(unsigned char)(ch2) << 16 ) | ( (unsigned long)(unsigned char)(ch3) << 24 ) )

const unsigned long FLAG_MULTIHEAD		=		mmioFOURCC1('M', 'H', 'E', 'D');
const unsigned long MULTICAST_VER	=0x10001; 

const unsigned long DEVICE_NAME_MAX_LEN_MULTI	= 20;		//设备名最大长度		
const unsigned long MULTICAST_CMD_SEARCH		= 1; //由搜索器发出的搜索指令
const unsigned long MULTICAST_CMD_INFO			=2; //由DVS发出的响应指令，发送自己的信息
const unsigned long MULTICAST_CMD_SET			=3; //由搜索器发出的设置指令，设置IP地址及设备名称
const unsigned long MULTICAST_CMD_SET_MAC		=4; //由搜索器发出的设置指令，设置MAC地址
const unsigned long MULTICAST_CMD_SET_DEFAULT_INPUTFORM = 5; //由搜索器发出的设置指令，设置默认制式（IPCamera制式出场后固定）
const unsigned long MULTICAST_CMD_SET_DEV_VER	=6;//由搜索器发出的设置指令，设置设备硬件版本号

typedef struct MultiCastInfo
{
	ULONG			head;		//是FLAG_MULTIHEAD
	ULONG			ver;		//结构信息版本
	ULONG			cmd;
	char			DevName[DEVICE_NAME_MAX_LEN_MULTI];
	BYTE			MacAddr[6];
	unsigned short	netport;
	ULONG			ipaddr;
	ULONG			netmask;
	ULONG			route;	
	unsigned long	softver;			
	unsigned long	softbuilddate;	//没有用了
	unsigned short	nHttpPort;
	char			NoUsed[2];
	unsigned long	deviceType;
	unsigned long	productType;	
	unsigned long	kernelVersion;
	unsigned long	inputform;
	unsigned long	devVer;//设备硬件版本号 
	char			szPasswd[28];//设置IP必须输入正确的admin密码，采用base64编码
	BYTE			Resved[28];//以后增加功能就利用这些字节
}MULTICAST_INFO;

//////////////////////////////////////////////////////////////////////////
//extral data(代替Resved[28])

//MULTICAST_CMD_SET
typedef struct _hxht_access_info
{
	char				deviceSerialNo[20];		//设备序列号
	unsigned long		dwAccessServerIP;			//接入服务器IP
	unsigned short		wMsgPort;					//接入服务器消息端口
	unsigned short		wStoreRegPort;				//存储注册端口
}HXHT_ACCESS_INFO;

#endif //_MULTICAST_HEADER_
