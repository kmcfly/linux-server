/////////////////////////////////////////////////////////////////////////////////////
/***********************************************************************
** 深圳同为数码科技有限公司保留所有权利。
** 作者			: zhl
** 日期         : 2011-12-22
** 名称         : UpnpMan.h
** 版本号		: 1.0
** 描述			: Upnp管理
				1: UPNP初始化时需要网卡接口个数和网卡接口名称信息
				2: 检测某个端口是否已经在网卡连接的路由器上映射
				3: 把网卡连接的路由器上的某个端口映射删除
				4: 向网卡连接的路由器上添加一个端口映射
				5: 获取网卡连接的路由器上关于本网卡的端口映射状态
				6: 获取网卡所在广域网(WAN)上的IP信息
				7: 对已经映射的端口需要定时向路由器刷新映射，防止路由器重启后映射被撤销
** 修改记录		:
***********************************************************************/
#ifndef _UPNP_MAN_H_
#define _UPNP_MAN_H_

#include "ssdp.h"
#include "upnpDevice.h"

typedef enum _port_map_status_
{
	PORT_MAP_NONE		= 0,		//操作成功。检测(当前端口没有被映射)、删除、添加端口映射成功
	PORT_MAP_EXIST		= 1,		//当前端口已经映射在路由器上
	PORT_MAP_ADD_FAIL	= 2,		//添加映射失败
	PORT_MAP_DEL_FAIL	= 3,		//删除映射失败
}PORT_MAP_STATUS;

typedef enum _port_type_
{
	PORT_TYPE_NONE	= 0,			//普通端口
	PORT_TYPE_DATA	= 1,			//数据端口
	PORT_TYPE_WEB	= 2,			//HTTP端口
}PORT_TYPE;


/*
*存放已经映射到路由的端口信息，在端口映射线程要定时查看这些端口是否已经被撤销映射(可能是路由器重启)、
*被别的IP映射了这个端口(在路由重启后其他IP先进行了映射操作)，这需要从新映射端口，并标记当前端口在路由器上的映射状态
*/
typedef struct _port_map_info_
{
	char szNicName[20];				//接口名称
	unsigned short	port;			//端口
	unsigned short	notUsed;		//未用
	unsigned long	mapStatus;		//端口当前状态，映射线程每次向路由器从新映射会刷新这个状态。
									//如果映射失败会标记为PORT_MAP_ADD_FAIL, 如果端口已经被映射标记为PORT_MAP_EXIST，否则为PORT_MAP_NONE
	PORT_TYPE		portType;
	unsigned long	localIP;
}PORT_MAP_INFO;

class CUpnpMan 
{
public:
	static CUpnpMan * Instance();
	~CUpnpMan();
	
//	bool Initial(std::list<char *> nicNameList);
	bool Initial();
	void Quit();

	void EnableUpnp(bool bUseUpnp);
	void SetDeviceName(const char * pDeviceName);
	void IPChanged(const char * pNicName);

	bool Start();
	void Stop();
	
	//查看网卡pNicName的端口port是否已经映射在路由器上
	unsigned long CheckPortMap(const char * pNicName, unsigned short port);
	//US02向导使用，根据dvrip查看对应upnp是否映射成功
	
	int CheckPortMap(unsigned long localIP);
	
	//撤销网卡pNicName的端口port在路由器上的映射
	unsigned long DeletePortMap(const char * pNicName, unsigned short port);
	unsigned long DeletePortMap();

	//把网卡pNicName的端口port添加到路由器上的映射上
	unsigned long AddPortMap(const char * pNicName, unsigned short port, PORT_TYPE portType);
	void AddPort(const char * pNicName, unsigned short port, PORT_TYPE portType);

	//获取网卡pNicName在路由器上的端口映射信息
	unsigned long GetNicMapInfo(const char * pNicName, std::list<PORT_MAP_INFO> & portMapInfoList);
	//获取网卡pNicName的外网IP地址.使用路由器的UPNP功能获取，如果设备通过pppoe方式上网则要通过其他方式获取IP地址
	int GetWanIP(char * pNicName);

private:
	CUpnpMan();

	unsigned long AddPortMap(unsigned long localIp, unsigned short port);
	int AddPortMap(unsigned long localIp, unsigned short port, unsigned long &mapStatus);
	unsigned long DelPortMap(unsigned short port);
	void PouseUpnp(bool bPouse);

	static PUB_THREAD_RESULT PUB_THREAD_CALL PortMapThread(void * pParam);
	void PortMapProc();

	int GetExtIp(char *pIp);
private:
	bool						m_bHasInitial;
	static CUpnpMan *			m_pUpnpMan;

	bool						m_bUseUpnp;
	bool						m_bPouse;
	CPUB_Lock					m_useUpnpLock;

	//std::list<CSsdp *>		m_ssdpList;
	CSsdp						m_ssdp;
	//std::list<char *>			m_nicNameList;

	PUB_thread_t				m_portMapProcID;
	bool						m_bPortMapProc;

	std::list<PORT_MAP_INFO>	m_portMapInfoList;
	CPUB_Lock					m_listLock;

	char						m_deviceName[256];
};
#endif	/*_UPNP_MAN_H_*/

/////////////////////////////////////////////////////////////////////////////////////

