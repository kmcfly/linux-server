///////////////////////////////////////////////////////////
#ifndef _UPNP_TYPE_H_
#define _UPNP_TYPE_H_

//////////////////////////////////////////////////////////
/*
*参考文档:
*		1 UPnP Device Architecture 1.1 (Document Revision Date 15 October 2008)
*		2 WANIPConnection:1 Service Template Version 1.01
*
*1 中描述了一个UPNP设备(控制点)要遵循的逻辑结构
*2 中描述网关设备(IGD)中WANIPConnection:1服务应当遵循的规则、提供操作方法和对应的输入输出参数
*/
//////////////////////////////////////////////////////////
const char HTTP_VERSION_1_1[]="HTTP/1.1";

const unsigned long  MILLION_MICRO_SECOND	= 1000000;		//百万微秒， 1S
const unsigned long  THOUSAND_MICRO_SECOND	= 1000;			//一千微秒， 1ms

const unsigned short SSDP_MULTI_PORT = 1900;
const char SSDP_MULTI_IP[] = "239.255.255.250";


typedef enum _ssdp_msg_type_
{
	SSDP_MSG_DEVICE_ALIVE	= 0,			//设备可用 NOTIFY with ssdp:alive
	SSDP_MSG_DEVICE_UALIVE	= 1,			//设备退出 NOTIFY with ssdp:byebye
	SSDP_MSG_DEVICE_UPDATE	= 2,			//NOTIFY with ssdp:update
	SSDP_MSG_DEVICE_SEARCH	= 3,			//控制点发现设备
}SSDP_MSG_TYPE;


//Notification Sub Type
const char SSDP_NTS[][20]=
						{
							"ssdp:alive", 
							"ssdp:byebye", 
							"ssdp:update",
							"\"ssdp:discover\""
						};


#endif /*_UPNP_TYPE_H_*/
///////////////////////////////////////////////////////////
