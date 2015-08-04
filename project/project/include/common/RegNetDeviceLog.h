#pragma once

#include <list>
#include "PUB_common.h"

const unsigned long	MAX_NET_DEVICE_NUM = 128;
const unsigned long NETDEVICE_LOG_HEAD_FLAG = mmioFOURCC('N', 'D', 'L', 'H');

typedef struct _net_device_link_info
{
	unsigned long	deviceID;	//网络设备ID，从1开始

	unsigned char	enabled;	//当前网络设备是否可用（可用则会接入该设备）
	unsigned char	channel;	//该网络设备在本地分配的通道
	
	char			szEtherName[16];		//如果为空，默认为eth0
	char			szServer[64];	//网络设备IP地址
	unsigned short	nPort;		//网络设备端口号
	unsigned short	nHttpPort;	//HTTP端口
	unsigned short	nCtrlPort;	//控制端口，一般与nPort相同
	char			szID[64];	//网络设备标识（或是MAC地址）
	char			username[36];//注册到网络设备上时所用的用户名
	char			password[36];//注册到网络设备上时所用的密码

	unsigned long	manufacturerId;	//设备厂商
	char 			manufacturerName[36];
	char			productModel[36];//产品型号

	unsigned char	bUseDefaultCfg;
	unsigned char	resv[3];
}NET_DEVICE_LINK_INFO;

typedef struct _netdevice_log_header
{
	unsigned long		headFlag;	//
	unsigned long		deviceTotalNum;	//文件记录中所容纳的设备总个数
	unsigned long		deviceUsedNum;	//文件记录中使用的记录条数
	unsigned long		itemLen;	//每一个设备项的长度
}NETDEVICE_LOG_HEADER;

typedef struct _nedevice_ext_func_flag
{
	unsigned long      functins[4];
	unsigned long      flag_poeIsPtoP;
	unsigned long      resv[11];
}NEDEVICE_EXT_FUNC_FLAG;

class CRegNetDeviceLog
{
public:
	static CRegNetDeviceLog *Instance();
	
	bool Initial(const char *pLogFilePath);
	void Quit();
	inline long GetClientID(){return m_clientID;}
	void EnterIPCamConfig(unsigned long clientID = LOCAL_DEVICE_ID);
	void ExitIPCamConfig();

	//获取设置POE使用的模式
	bool GetPoeUsedPtoPMode(){return m_bPoeUsedPtoPFunc;}
	void SetPoeUsedPtoPMode( bool bPoeUsedModePtoP );
	
	/*********************************************************************************
	得到最大可接受的接入设备数
	*********************************************************************************/
	unsigned long GetMaxDeviceCount() const;
	
	/*********************************************************************************
	设置接入的网络设备参数
	**********************************************************************************/
	bool SetNetDevice(NET_DEVICE_LINK_INFO *pNetDevice, unsigned long num);
	
	/*********************************************************************************
	用以下两个函数返回网络设备的一个数组
	**********************************************************************************/
	unsigned long GetNetDeviceCount() const;
	unsigned long GetNetDevice(NET_DEVICE_LINK_INFO *pNetDevice, unsigned long num);
	
	/*********************************************************************************
	返回的网络设备的列表
	**********************************************************************************/
	void GetChBindNetDevice(std::list<NET_DEVICE_LINK_INFO> &list);
	
	///*********************************************************************************
	//为指定的网络设备分配通道，只有分配到本地通道的设备才会被接入
	//**********************************************************************************/
	//bool AllocChannelForDevice(unsigned short deviceID, unsigned char channel);	
	
private:
	CRegNetDeviceLog(void);
	~CRegNetDeviceLog(void);
	
	bool AddOneDevice(NET_DEVICE_LINK_INFO &netDevice);

private:
	char						*m_pFileLogName;
	NET_DEVICE_LINK_INFO				*m_pNetDeviceInfo;
	NETDEVICE_LOG_HEADER		m_logHeader;
	CPUB_Lock					m_fileLock;
	long						m_clientID;
	std::list<long>				m_IDList;

	//是否使用一对一模式
	bool                        m_bPoeUsedPtoPFunc;
};
