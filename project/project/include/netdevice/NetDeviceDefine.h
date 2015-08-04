#ifndef _NET_DEVICE_DEFINE_
#define _NET_DEVICE_DEFINE_

/*
 * 关于定义 Net Device 库
 * 加前缀 ND_
 * NVR版本定义 : 后缀 _NVR_V3 , _NVR_V4
 * IPC版本定义 : 后缀 _IPC_V3 , _IPC_V4
 * NVR启动后会搜索..\plugin目录, 加载动态库, 并检索库支持的IPC
 * 搜索局域网中的IPC并连接会话, 辨别是否为支持的IPC种类
*/

#ifdef __ENVIRONMENT_WIN32__
#pragma pack(4)
#endif

const char MAIN_ETHER[] = "eth0";
const char POE_ETHER[] = "eth1";

enum NET_DEVICE_STREAM_TYPE
{
	NET_DEVICE_MAIN_STREAM,		//主码流
	NET_DEVICE_SUB_STREAM,		//子码流
	NET_DEVICE_MINOR_STREAM,	//现场小码流
	NET_DEVICE_JPEG_STREAM,		//图片流
	NET_DEVICE_MAX_STREAM,
};

enum MANUFACTURE_ID
{
	MID_UNKNOWN,
	MID_TVT,
	MID_ONVIF,
	MID_COUNT,
};

enum ONVIF_MANU_ID
{
	MANU_ONVIF_NORMAL,
	MANU_ONVIF_HIKVISION,
	MANU_ONVIF_DAHUA,
	MANU_ONVIF_XIONGMAI,
	MANU_ONVIF_ZNV,
	MANU_ONVIF_ZHONGWEI,
	MANU_ONVIF_TIANSHITONG,
	MANU_ONVIF_LONGPLUS,
	MANU_ONVIF_TVT,
	MANU_ONVIF_QSEE,
	MANU_ONVIF_XTS,
	MANU_ONVIF_KEC,
	MANU_ONVIF_NVSIP,
	MANU_ONVIF_STENLEY,
	MANU_ONVIF_SUNELL,
};

//lib动态库的基本信息
typedef struct _nd_lib_info 
{
	unsigned int	manufacturerId;		//厂商ID，参照MANUFACTURE_ID
	char			version[28];		//库版本信息 例如"v2.0.1"
	unsigned char	bNeedHttpPort;		//是否需要HTTP端口进行通信
	unsigned char	bNeedCtrlPort;		//是否需要专门的控制端口通信
	unsigned char	resv[2];
}ND_LIB_INFO;

//MAC
typedef struct _nd_mac
{
	unsigned char mac[8];
}ND_MAC;

//搜索局域网 得到的网络设备信息
typedef struct _nd_device_info
{
	char			szAddr[64];
	unsigned long	netmask;
	unsigned long	gateway;
	unsigned short	netport;
	unsigned short	nHttpPort;
	unsigned short	nCtrlPort;
	char			szID[64];	//网络设备标识
	char			firmwareVersion[16];
	unsigned long	manufacturerId;
	char			manufacturerName[36];
	char			productModel[36];
	char			szEther[8];
}ND_DEVICE_INFO;

typedef struct _nd_device_network_info_
{
	unsigned long	curIpAddr;
	unsigned long	newIpaddr;
	unsigned long	newNetmask;
	unsigned long	newGateway;
	char			userName[36];
	char			password[28];
//	unsigned char	MacAddr[6];
	char			szID[64];	//网络设备标识(或是MAC地址)
	unsigned short	notused;
}ND_DEVICE_NETWORK_INFO;

//网络设备支持的差异信息
typedef struct _nd_dev_info
{
	unsigned long	chnn;			//通道
	unsigned long	videoSize;		//支持的屏幕分辨率 DD_VIDEO_SIZE, 比如 是否支持D1: (videoSize & (0x1 << 3))
	unsigned char	ucRate[32];		//每种分辨率对应的最大帧率, 比如 D1对应最大帧率: ucRate[3]
	unsigned long	maxBitRate[32];	//每种分辨率对应的最大码率，比如 D1对应最大码率: maxBitRate[3]
}ND_DEV_INFO;

//网络设备报警信息
typedef struct _nd_alarm_info
{
#ifdef	__ENVIRONMENT_WIN32__

	ULONGLONG	sensor;				//传感器报警
	ULONGLONG	motion;				//移动侦测
	ULONGLONG	videoLoss;			//视频丢失
	ULONGLONG   other;				//其他报警
#else
	unsigned long long 	sensor;				//传感器报警
	unsigned long long 	motion;				//移动侦测
	unsigned long long 	videoLoss;			//视频丢失
	unsigned long long  other;				//其他报警
#endif
}ND_ALARM_INFO;

#ifdef	__ENVIRONMENT_WIN32__
#pragma pack()
#endif

#endif
//end
