#pragma once

#include <list>
#include "NetDevicePlatform.h"
#include "RegNetDeviceLog.h"
#include "IPDataBufferMan.h"
#include "NetProtocol.h"
#include "ConfigNetDeviceMan.h"
#include "NdLibMan.h"
#include "LogMan.h"
#include "UserMan.h"

#include "PoeOnvifDeviceMan.h"
#include "PoeDeviceMan.h"

#include <omp.h>

using namespace std;
const unsigned int	g_MaxNetChannCnt = 128;
typedef struct _net_link_info_ex
{
	NET_DEVICE_LINK_INFO linkInfo;
	
	unsigned long		status;
	unsigned long		bPOEOnLine;	//1:online, 0:offline
	unsigned long		POEOnLineTime; //POE上线时的时间，以开机累积时间计（单位ms）
	unsigned long		disconnectTime;	//断开的时间，采用GetTickCount的时间
	unsigned char		bWriteConnectLog;//是否需要在连接上时写日志
	unsigned char		resv[3];
	
	CNetDevice			*pNetDevice;
}NET_LINK_INFO_EX;

typedef struct _resolution_info
{
	unsigned long		resolution;		//分辨率,参考VIDEO_SIZE
	unsigned long		multiNumCIF;	//该分辨率约为几个CIF
}RESOLUTION_INFO;

enum RESOLUTION_INDEX
{
	RESOLUTION_QCIF,
	RESOLUTION_QVGA,
	RESOLUTION_CIF,
	RESOLUTION_HD1,
	RESOLUTION_VGA,
	RESOLUTION_D1,
	RESOLUTION_960H,
	RESOLUTION_HD720,
	RESOLUTION_HD1080,

	RESOLUTION_960P,
	RESOLUTION_UXGA,
	RESOLUTION_3M,
	RESOLUTION_4K,

	RESOLUTION_SXGA,

	RESOLUTION_16_9_3M,
	RESOLUTION_4M,
	RESOLUTION_5M,


	RESOLUTION_COUNT,
};

unsigned long DDVideoSizeToVideoSize(unsigned long ddvideoSize);
unsigned long VideoSizeToDDVideoSize(unsigned long videoSize);
unsigned long GetMaxVideoSize(unsigned long videoSize);
bool IsUnderVideoSize(unsigned long videoSize, unsigned long checkVideoSize);

class CNetDeviceManager
{
public:
	static CNetDeviceManager *Instance();
	
	bool Initial(long localVideoInputNum, long netVideoInputNum, long localSensorInNum);
	void Quit();

	bool Start();
	void Stop();
	
	//通知注册的设备改变了
	void NotifyRegDeviceChanged();
	void NotifyRegDeviceChanged1();

	//stream：码流类型，具体参考NET_DEVICE_STREAM_TYPE
	bool GetDataBuffer(long chipId, unsigned char *&pBuffer, unsigned long &dataLen, unsigned long stream = NET_DEVICE_MAIN_STREAM);
	void ReleaseDataBuffer(long chipId, unsigned char *pBuffer, unsigned long dataLen, unsigned long stream = NET_DEVICE_MAIN_STREAM);
	long GetBufferedNum(long chipId, unsigned long stream = NET_DEVICE_MAIN_STREAM);
	bool IsBufferFull(long chipId, unsigned long stream = NET_DEVICE_MAIN_STREAM);
	bool PrintBufferInfo(long chipId, unsigned long stream = NET_DEVICE_MAIN_STREAM);
	unsigned long GetAverageRate(unsigned long stream  = NET_DEVICE_MAIN_STREAM );

	void DeviceAction(bool bAction);
	void ClearDataBuffer(unsigned long stream = NET_DEVICE_MAX_STREAM);
	
	bool KeyFrame(ULONGLONG ullCH, ULONGLONG ullSecondCH, ULONGLONG ullThirdCH);

	//查询报警状态
	void GetMotionStatus(ULONGLONG &status);
	void GetSensorInStatus(ULONGLONG &status);
	void GetVideoLossStatus(ULONGLONG &status);
	void GetOtherAlarm(ULONGLONG &status);
	void GetExceptionAlarm(ULONGLONG &status);

	//云台控制, 和PtzMan控制方式一致
	bool PTZAction(unsigned long channel, unsigned char cmdtype, unsigned long ptzParam, unsigned char speed, unsigned char waiteTime);

	//图像色彩相关, 只能获取网络通道部分数据
	bool SetVideoColor(unsigned char channel, unsigned char brightness, unsigned char hue, unsigned char saturation, unsigned char contrast);
	bool SaveVideoColor(unsigned char channel, unsigned char brightness, unsigned char hue, unsigned char saturation, unsigned char contrast);
	bool GetDefaultVideoColor(unsigned char channel, unsigned char& brightness, unsigned char& hue, unsigned char& saturation, unsigned char& contrast);

	//图像翻转，镜像，走廊模式
	bool SetVideoOther(unsigned int channel,unsigned char *pOther,unsigned long lType);
	
	//即时 设置 在线网络设备 的配置
	bool SetNetConfig(const unsigned char* pCfgData, unsigned long dataLen);

	//同步所有[或某个]连接的IPC时间
	void SyncTime(unsigned char channel);
	void SyncTimeAll();

	//设置网络设备, 获取网络设备 [配置了的设备]
	bool SetNetDevice(IP_DEVICE_INFO *pNetDevice, unsigned long num);
	bool SetNetDevice1(IP_DEVICE_INFO *pNetDevice, unsigned long num);
	void GetNetDeviceEffect(std::list<NET_DEVICE_LINK_INFO> &linkInfoList);
	unsigned long GetNetDevice(IP_DEVICE_INFO *pNetDevice, unsigned long num);
	unsigned long GetNetDeviceCount();
	unsigned long GetNetDeviceMax();

	void EnablePoeDiscoverThreadRun( bool bRunPoeDiscover );

	//由通道编号获取设备对象
	CNetDevice* GetNetDeviceByChnn(unsigned char channel, bool bConnectOnly = false);
	bool IsNetDeviceHaveConnect(ND_DEVICE_INFO * pNDDeviceInfo);

	//获取当前活跃的网络设备连接总数[已经创建连接对象的设备]
	long GetActiveCount(bool bConnectOnly = false);

	bool IfNeedRefresh();
	void RefreshDisplay();

	void SecondStreamMaxRateForFirstStreamNum(unsigned long limitRateForFirstNum);

	void SetSecondStreamLimitRate(unsigned long maxCopies);
	void ChangeSecondStreamCH(ULONGLONG secondStreamCH, ULONGLONG secondStreamCH_mobile, unsigned long secondAllSendNum);

	void SetFirstStreamLimitBitRate(unsigned long maxTotalBitRate);

	void OnNetworkRestart();

	void RequestMinorStream(ULONGLONG minorStreamCH);

	//修改switch网口的IP地址
	void ChangeSwitchIP(unsigned long switchIP, unsigned long switchMask);

	//获取 IPC 当前的 Resolution
	void GetIPCCurrentResolution( int channel, int* width, int* height );
	void SetCurDisplaySpiltMode( unsigned int live_spilt_mode );
	void SetDispModeBigImageUsedD1( unsigned int channel, bool bUsedD1 );

	int GetNetEncodeInfo(unsigned long streamType, NET_CHANNEL_ENC_INFO * pNetChannelEncInfo);
	int SetNetEncodeInfo(int chnn, int iStream, char * pSZResolution, int iRate, int iEncodeType, int iQuality, int iBitRateStream);

	int GetEncodeInfo(int chnn,char * pVideoEncodeInfo, int iVideoEncodeInfoLen);
	int SetEncodeInfo(int chnn, int iStream, char * pSZResolution, int iRate, int iEncodeType, int iQuality, int iBitRateStream);
	int GetCurrentEncodeInfo(int chnn,char * pVideoEncodeInfo, int iVideoEncodeInfoLen);
private:
	CNetDeviceManager(void);
	~CNetDeviceManager(void);
	
	//新建一个连接的网络设备，如果接入成功，返回true，否则返回false
	bool NewNetDevice(NET_LINK_INFO_EX &netLinkInfoEx);

	//删除一个连接的网络设备
	void DelNetDevice(NET_LINK_INFO_EX &netLinkInfoEx);
	
	bool SetChannelMapper(unsigned char channel, unsigned short deviceID, unsigned char netChannel);

	static RESULT WINAPI ConnectThread(void * pParam);
	void ConnectProc();
	
	void DeviceManager();
	void ClearNetLinkInfo(NET_LINK_INFO_EX * pNetLinkInfoEx);

	static RESULT WINAPI TimerThread(void *pParam);

	void WriteLog(unsigned long clientID, LOG_TYPE type, const char *pDevIP = NULL, unsigned long logTime = 0);

	void SetSecondStreamEncode();
	void SetNetDeviceSecondStreamCfg(CNetDevice *pNetDevice, bool bAdj=true);
	void SetNetDeviceThirdStreamCfg(CNetDevice *pNetDevice);

	void UpdateConfigOnConnect(NET_LINK_INFO_EX &netlinkInfo);
	bool CheckVideoResolution(NET_LINK_INFO_EX &netlinkInfo);

	void SyncTime(CNetDevice* pNetDevice);

	static void OnvifOnlineCallBack(POE_ONVIF_DEVICE_INFO &deviceInfo, POE_IPC_EXT_CTRL_PARAM& stCtrlParam, void *pUser);
	static void OnvifOfflineCallBack(POE_ONVIF_DEVICE_INFO &deviceInfo, POE_IPC_EXT_CTRL_PARAM& stCtrlParam, void *pUser);
	void OnOnvifOnline(POE_ONVIF_DEVICE_INFO &deviceInfo, POE_IPC_EXT_CTRL_PARAM& stCtrlParam );
	void OnOnvifOffline(POE_ONVIF_DEVICE_INFO &deviceInfo, POE_IPC_EXT_CTRL_PARAM& stCtrlParam );

	static void GetPhyChannelByMacAddrCallBack( char* macAddr, int len, bool bOnvifDevice, int* chnn, bool bClearOnvifDeviceMac, void* pUser );
	void GetPhyChannelByMacAddr( char* macAddr, int len, bool bOnvifDevice, int* chnn, bool bClearOnvifDeviceMac );

	unsigned long AdjustBitStream(unsigned long bitStream);
	unsigned long AdjustFrameRate(unsigned long chnn, unsigned long chnnAvailRate, unsigned long maxRate, unsigned long resolutionIndex);
	unsigned long AdjustResolution(unsigned long chnn, unsigned long resolutionIndex);

	unsigned long AdjustThirdBitStream(unsigned long bitStream);
	unsigned long AdjustThirdFrameRate(unsigned long chnn, unsigned long chnnAvailRate, unsigned long maxRate, unsigned long resolutionIndex);
	unsigned long AdjustThirdResolution(unsigned long chnn, unsigned long resolutionIndex);

	unsigned char GetBitIndex(unsigned long mask);
	unsigned long GetResolutionIndex(unsigned long resolution);

	bool IsPoeDevice(const NET_DEVICE_LINK_INFO &linkInfo);
	bool CheckIPCIsSame( const NET_DEVICE_LINK_INFO &linkInfo1, const NET_DEVICE_LINK_INFO &linkInfo2 );
	void InitPoeChnnParam( NET_DEVICE_LINK_INFO &stDeviceInfo, int chnn );
	unsigned long GetMaxRate(unsigned long chnn, unsigned long videoSize);
	unsigned long GetMaxBitrate(unsigned long chnn, unsigned long videoSize);
	
	void ClearNetLinkInfoEx(NET_LINK_INFO_EX &netLinkInfoEx);
	void ClearAllNetLinkInfoEx(NET_LINK_INFO_EX *pNetLinkInfo);
	
	void RefreshNetLinkInfo(NET_LINK_INFO_EX &netLinkInfoEx,int &channl);
	void RefreshTempNetLinkInfo(int &channl);
	bool IfCheckStatusOver();

	bool IfChannNetLinkInfoValid(NET_LINK_INFO_EX &netLinkInfoEx);
private:
	NET_LINK_INFO_EX			m_netLinkInfo[g_MaxNetChannCnt];
	CPUB_Lock					m_netLinkInfoLock[g_MaxNetChannCnt];
	NET_LINK_INFO_EX			m_tempNetLinkInfo[g_MaxNetChannCnt];
	bool						m_bSetSecondCfg[g_MaxNetChannCnt];
	bool						m_bSetThirdCfg[g_MaxNetChannCnt];
	
	CRegNetDeviceLog			*m_pRegDeviceLog;
	CConfigNetDeviceMan			*m_pCfgNetDeviceMan;
	CNdLibMan					*m_pNdLibMan;

	bool						m_bLinkInfoChanged;
	bool                        m_bPoeLinkChg;
	
	list<NET_LINK_INFO_EX>		m_tempLinkInfoList;
//	list<NET_LINK_INFO_EX>		m_listNetLinkInfo;
	bool						m_undealChannFlag[g_MaxNetChannCnt];
	CPUB_Lock					m_deviceListLock;
	CPUB_Lock					m_deviceListLock2;
	CPUB_Lock					m_snapPicLock;
	bool						m_bRunning;
	PUB_thread_t				m_threadConnect;
	PUB_thread_t				m_threadTimer;
	
	static long					m_nextDeviceId;
	CIPDataBufferMan			m_dataBufferMan[NET_DEVICE_MAX_STREAM];

	unsigned char				*m_pSecondStreamBuffer;

	unsigned char               *m_pJepgStreamBuffer;

	//通道参数信息
	long						m_localVideoInputNum;
	long						m_netVideoInputNum;
	long						m_videoInputNum;
	long						m_localSensorInNum;
	long						m_clientID;

	//记录是否需要刷新图像显示（如设备绑定改变、设备删除、设备断开等）
	CPUB_Lock					m_refreshDisplayLock;
	bool						m_bRefreshDisplay;
	unsigned long				m_checkRefreshTime;

	ENCODE_INFO_EX				*m_pSecondStreamEncode;
	ND_DEV_INFO					*m_pDevInfo;

	ENCODE_INFO_EX				*m_pThirdStreamEncode;
	ND_DEV_INFO					*m_pThirdDevInfo;

	ULONGLONG					m_ullSecondCH;
	ULONGLONG					m_ullMinorCH;

	unsigned long				m_maxSecondStreamCopies;
	CPUB_Lock					m_maxCopiesLock;

	ULONGLONG					m_secondStreamCH;
	ULONGLONG					m_secondStreamCH_mobile;
	unsigned long				m_secondAllSendNum;

	unsigned long				m_maxMainStreamTotalBitRate;
	unsigned long				m_secondLimitRateForFirstStreamNum;

	RESOLUTION_INFO				m_resolutionInfo[RESOLUTION_COUNT];

	//POE相关
	unsigned char				m_poeInputNum;
	unsigned long				m_switchIP;
	unsigned long				m_switchMask;

#ifndef __ENVIRONMENT_WIN32__
	CPoeDeviceMan				m_poeDeviceMan;
#endif
	POE_CHANNEL_SN				*m_pPoeChannelSn;
	CPoeOnvifDeviceMan			m_poeOnvifDeviceMan;

	unsigned long               m_ulLiveDisplayMode;

	unsigned long               m_ulUsedD1Channel;
	bool                        m_bUsedD1ShowImage;

	CPUB_Lock					m_configInfoLock;

	unsigned long               m_resyncTimeCount[g_MaxNetChannCnt];

};
