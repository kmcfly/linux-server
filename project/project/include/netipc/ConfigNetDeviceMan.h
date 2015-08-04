#ifndef _CONFIG_NET_DEVICE_MAN_H_
#define _CONFIG_NET_DEVICE_MAN_H_

#include "ConfigEx.h"
#include "ConfigParser_V3.h"
#include "PUB_common.h"
#include <map>
#include <vector>


/*
 * 
 *
*/

class CNetDeviceManager;
class CNetDevice;


class CConfigNetDeviceMan
{
public:
	virtual ~CConfigNetDeviceMan();
	static CConfigNetDeviceMan *Instance();

	bool Initial(CConfigEx* pConfigEx,
				 long videoInputNum,
				 long netVideoNum,
				 long sensorOutputNum,
				 long audioInputNum,
				 long localSensorInputNum);
	void Quit();

	//设置网络设备配置
	void SetNetConfig(CNetDeviceManager* pNetDeviceMan, CNetDevice* pNetDeviceOne, const unsigned char* pCfgData, unsigned long dataLen);

	//检出修改的配置数据是否需要, 重新读取初始化配置
	bool TestUpdataInitData(const unsigned char* pCfgData, unsigned long dataLen);

	//更新需要初始化的配置信息
	bool UpdateInitData(long channl,CNetDevice* pNetDeviceOne=NULL);

	//设置初始化的配置信息
	bool SetInitNetConfig(CNetDeviceManager* pNetDeviceMan, CNetDevice* pNetDeviceOne,long channl);

	void SetIPDeviceInfo(IP_DEVICE_INFO *pIPDeviceInfo, long num);

	void GetNetDeviceInfo(IP_DEVICE_INFO *pIPDeviceInfo, int  &num);
protected:
	typedef struct
	{
		unsigned long  itemID;
		unsigned long  useLen;
		unsigned long  buffLen;
		unsigned char* pData;
	}SDK_ITEM_DATA;

	typedef std::vector<NCFG_ITEM_ID> CFG_ITEM_VEC;

protected:

	//获取需要初始化的配置项目ID
	void GetInitialItemId(CFG_ITEM_VEC& itemVec);

	void SubstituteItemData(NCFG_ITEM_ID itemID, const unsigned char *pSrcCfgData, unsigned char *pDstCfgData);

protected:
	CConfigNetDeviceMan();

	CConfigEx*								m_pConfigEx;
	CConfigParser_V3*						m_pConfigParserV3;

	std::map<unsigned long, unsigned long>	m_devSdkMap;		//设备, SDK
	std::vector<IP_DEVICE_INFO>				m_ipDeviceInfo;

	//设备信息
	long	m_videoInputNum;
	long	m_netVideoNum;
	long	m_sensorOutputNum;
	long	m_audioInputNum;
//	long	m_sensorInputNum;

	//nvr配置
	unsigned char*  m_pNvrCfg[128];
	unsigned long	m_cfgLen[128];

	bool			m_bRefreshData;			//是否需要重新读取配置数据

	PUB_lock_t		m_lock_t;
};

#endif

//end
