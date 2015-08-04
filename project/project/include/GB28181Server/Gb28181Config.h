#pragma once
#include <stdio.h>
#include <map>
#include <utility>
#include <string>

#include "GBT28181_Define.h"

#define  GB_FAIL  0xFFFF

enum RELOAD_TYPE
{
	SIPSERVER,		//sip server 更新
	CHANNELID,		//通道id更新
	ALL,			//全部更新
};
typedef struct _gb28181_init_info
{
	unsigned long	dwMaxVideo;
	unsigned long   dwMaxALarm;
	unsigned long	dwMaxAudio;
	unsigned long	dwDeviceType;

	PRODUCT_TYPE	dwProductType;
	unsigned long	dwProductID;
	unsigned long	dwProductSubID;

	unsigned long	dwDevVer;
	unsigned long	dwSoftVer;
	unsigned long	dwBuildTime;
	char			deviceName [20];
}Gb28181_INIT_INFO;
#ifndef  __ENVIRONMENT_LINUX__
#include<WinSock2.h>
#pragma comment(lib,"WS2_32")
class CInitSock
{
public:
	CInitSock(BYTE minorVer=2,BYTE majorVer=2)
	{
		WSADATA wsaData;
		WORD sockVersion=MAKEWORD(minorVer,majorVer);
		if(WSAStartup(sockVersion,&wsaData)!=0)
		{
			exit(0);
		}
	}
	~CInitSock()
	{
		WSACleanup();
	}
};
#endif
class GB28181_CONFIG
{
public:

	static GB28181_CONFIG* Instance();

	GB28181_CONFIG();

	~GB28181_CONFIG();

	//删除所有设备
	void Delete();

	 void SetGB28181Config(const GB28181_SIP_SERVER & Gb28181SipServer);
	
	 void SetGB28181InfoConfig(const Gb28181_INIT_INFO & Gb28181Info);

	//通道ID 获取DeviceID
	std::string GetDvrID(unsigned short uiChannel); 
	
	//通过DeviceID 获取通道ID
	unsigned short GetChannel(const string &strDvrID); 

    void InsertData(const string & strDvrID,unsigned short uiChannel);

	void InsertAlarmData(const string & strAlarmID,unsigned short uiAlarmChannel);

	//获取告警设备的通道ID
	unsigned short GetAlarmChannel(const string &strAlarmID);

	//获取告警设备ID
	std::string GetAlarmID(unsigned short uiAlarmChannel);


	std::string GetProductName(PRODUCT_TYPE type);

	//当配置有变化时，清除对列中通道数据，重新插入
	void ClearData();

	//当配置有变化时，需清除对列中Alarm数据，重新插入
	void ClearAlarmData();

	void ClearDvrIDsData();
	
	void ClearDvrAlarmIDsData();

	//重新加载配置
	bool ReloadConf(const enum RELOAD_TYPE){return true;};

	bool SetSipServerIP(unsigned long ulServerIP);

	bool SetDvrIP(unsigned long uldvrIP);
	
	#ifndef __ENVIRONMENT_LINUX__
	bool SetDvrWindowsIP();
	#endif
public:
	char m_wSipserverIP[32];				//sip服务器IP地址
	char m_SipServerId[32];					//sip服务器ID
	char m_Dvrip[32];						//DVR ip
	char m_DvrId[32];						//设备Id
	char m_UserID[32];						//用户名
	char m_RegisterPasswd[32];				//注册密码

	char m_szOwner[64];					//<!-- 当为设备时，设备归属（必选） -->
	char m_szCivilCode[64];				//<!-- 行政区域（必选） -->
	char m_szBlock[64];					//<!-- 警区（可选） -->

	unsigned short	m_wSipserverPort;		//sip服务器会话端口
	unsigned short	m_wSipDvrPort;			//dvr sip会话端口
	unsigned int	m_expires;				//过期时间

	unsigned char	m_dwMaxCamera;          //最大支持的摄像机数目
	unsigned char	m_dwMaxALarm;            //最大支持的告警数
	unsigned long	m_dwDevVer;             //硬件版本
	unsigned long	m_dwSoftVer;            //软件版本
	unsigned long	m_dwBuildTime;          //版本编译时间
	char			m_deviceName [20];       //设备名称
	PRODUCT_TYPE    m_productType;           //产品类型

	deque<CCatalogItem*> m_deqItem;		//<!-- 设备目录项列表,Num表示目录项个数 -->
	deque<CDeviceStatusItem*> m_deqAlarmItem;  //!--报警设备目录

	//std::map<string,unsigned short> dvrIDs;
	//std::map<string,unsigned short> dvrAlarmIDs;

	std::map<unsigned short,string> dvrIDs;
	std::map<unsigned short,string> dvrAlarmIDs;
};
