///////////////////////////////////////////////////////////
#ifndef _UPNP_TYPE_H_
#define _UPNP_TYPE_H_

//////////////////////////////////////////////////////////
/*
*�ο��ĵ�:
*		1 UPnP Device Architecture 1.1 (Document Revision Date 15 October 2008)
*		2 WANIPConnection:1 Service Template Version 1.01
*
*1 ��������һ��UPNP�豸(���Ƶ�)Ҫ��ѭ���߼��ṹ
*2 �����������豸(IGD)��WANIPConnection:1����Ӧ����ѭ�Ĺ����ṩ���������Ͷ�Ӧ�������������
*/
//////////////////////////////////////////////////////////
const char HTTP_VERSION_1_1[]="HTTP/1.1";

const unsigned long  MILLION_MICRO_SECOND	= 1000000;		//����΢�룬 1S
const unsigned long  THOUSAND_MICRO_SECOND	= 1000;			//һǧ΢�룬 1ms

const unsigned short SSDP_MULTI_PORT = 1900;
const char SSDP_MULTI_IP[] = "239.255.255.250";


typedef enum _ssdp_msg_type_
{
	SSDP_MSG_DEVICE_ALIVE	= 0,			//�豸���� NOTIFY with ssdp:alive
	SSDP_MSG_DEVICE_UALIVE	= 1,			//�豸�˳� NOTIFY with ssdp:byebye
	SSDP_MSG_DEVICE_UPDATE	= 2,			//NOTIFY with ssdp:update
	SSDP_MSG_DEVICE_SEARCH	= 3,			//���Ƶ㷢���豸
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
