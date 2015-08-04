// 下列 ifdef 块是创建使从 DLL 导出更简单的
// 宏的标准方法。此 DLL 中的所有文件都是用命令行上定义的 TSNETDEVICE_EXPORTS
// 符号编译的。在使用此 DLL 的
// 任何其他项目上不应定义此符号。这样，源文件中包含此文件的任何其他项目都会将
// TSNETDEVICE_API 函数视为是从 DLL 导入的，而此 DLL 则将用此宏定义的
// 符号视为是被导出的。
#ifndef _TS_NETDEVICE_H_
#define _TS_NETDEVICE_H_

#include "NetDevicePlatform.h"
#include "SWL_MultiNetComm.h"

#ifdef __ENVIRONMENT_LINUX__
#include "lvfw.h"
#else
#include <vfw.h>
#endif

#ifdef TEST_DISCONNECT
const int MAX_KEEP_TIME = 4000;			//保持连接时间(ms)
#else
const int MAX_KEEP_TIME = 20000;			//保持连接时间(ms)
#endif

const long	SOCK_TIME_OUT = 10000;		//发送接收超时时间(ms)
const long REPLY_DATA_TIME_OUT = 10000;		//网络回复数据超时时间(ms)

const unsigned long SWL_HEADFLAG = mmioFOURCC('h', 'e', 'a', 'd');

const long			RECV_BUFFER_SIZE = 256*1024;

const long			INVALID_CMD_ID = -1;

typedef struct _reply_cmd_data
{
	long			requestCmdID;
	long			replyCmdID;
	unsigned char	*pData;
	long			dataLen;
}REPLY_CMD_DATA;

typedef struct _net_combined_info_ex
{
	unsigned	long		dwUsed;			//是否正被使用	0 没有用 1 使用了
	unsigned	long		dwDataID;		//本组合数据的ID，组合数据里的每个包的数据ID相同
	unsigned	long		dwTotalPack;	//一共有几个包
	unsigned	long		dwTotalLen;		//所有的有效数据的总长度
	unsigned	long		dwLen;			//已经接收/发送了多少长度
	unsigned	char		*pBuffer;		//只在接收的时候有用
	long					bufferLen;

	unsigned long		streamId;
}NET_COMBINED_INFO_EX;

//当为SWL_COMBINATION_DATA数据时会跟结构体
typedef struct swl_combined_data
{
	unsigned	long	dwDataID;		//本组合数据的ID，组合数据里的每个包的数据ID相同
	unsigned	long	dwTotalPack;	//一共有几个包
	unsigned	long	dwTotalLen;		//所有的有效数据的总长度
	unsigned	long	dwIndex;		//第几个包，基数从1开始
	unsigned	long	dwLen;			//本包长度
	unsigned	long	dwPadding;		//为兼容上一版本的网络传输模块附加
}NET_COMBINED_DATA;

typedef struct _comm_pack_head_
{
	char		headFlag[4];
	long		dataLen;
}COMM_PACK_HEAD;

enum RECV_DATA_TYPE
{
	DATA_TYPE_TRANS			= 0x0,	//透明数据
	DATA_TYPE_PACK_INFO		= 0x1
};

// 此类是从 TSNetDevice.dll 导出的

struct _ts_netdevice_param;
class CTSCfgToIpc;
class CTSNdParam;

class CTSNetDevice : public CNetDevice
{
	friend class CTSCfgToIpc;
	friend class CTSNdParam;
public:
	CTSNetDevice();
	virtual ~CTSNetDevice();

	virtual bool Initial(long deviceId, long chnn, char *pServer, unsigned short netPort, unsigned short httpPort, \
		unsigned short ctrlPort, const std::string & user, const std::string & password, const ND_MAC & mac, const char *pEther="eth0", long sensorNum=0);
	//反初始化设备，释放资源及登出。
	virtual void Quit();

	virtual bool IfNeedInitNetConfig();

	virtual void SetDataBufferMan(CIPDataBufferMan *pDataBufferMan);

	/***********************************************************************
	修改设备绑定的通道号
	************************************************************************/
	virtual void SetBindChannel(long chnn);
	virtual long GetBindChannel();
	virtual void SetLocalVideoNum(long localNum);
	virtual unsigned int GetLocalVideoNum(){ return m_localVideoNum;};

	/***********************************************************************
	得到该网络设备的状态,断开或正常
	************************************************************************/
	virtual long GetDevStatus();


	/***********************************************************************
	查看网络设备是否处于重连中
	************************************************************************/
	virtual bool IsReConnectting();
	
	
	/***********************************************************************
	修改设备绑定的传感器通道号
	************************************************************************/
	virtual void SetBindSensorChannel(long sensorChnn);
	virtual long GetBindSensorChannel();

	/*
	得到当前连接控制状态
	*/
	virtual long IsReady();

	/************************************************************************
	接口功能描述：请求视频及音频数据流。
	输入参数描述：
	1、video1：主码流MASK。
	2、video2：次码流MASK。
	3、video3：手机码流MASK。
	4、audio：音频流MASK。
	返回值描述：返回成功或者失败。
	注：对应的位为零，表示停止。
	************************************************************************/
	virtual bool RequestStream(ULONGLONG video1, ULONGLONG video2, ULONGLONG video3, ULONGLONG audio);


	virtual bool RequestJpegStream(ULONGLONG channlMask);
	/************************************************************************
	接口功能描述：请求视频关键帧
	输入参数描述：
	1、video1：主码流MASK。
	2、video2：次码流MASK。
	3、video3：手机码流MASK。
	返回值描述：返回成功或者失败。
	注：对应的位为零，表示停止。
	************************************************************************/
	virtual bool RequestKeyFrame(ULONGLONG video1, ULONGLONG video2, ULONGLONG video3);

	/************************************************************************
	接口功能描述：云台控制接口。
	************************************************************************/
	virtual bool PTZControl(long chnn, unsigned long cmdtype, unsigned long ptzParam, unsigned long speed, unsigned long waiteTime);

	/************************************************************************
	接口功能描述：获取图像色彩信息。
	输入参数描述：
	1、ctrlInfo：控制信息，具体见PTZ_CTRL结构体定义。
	返回值描述：返回成功或者失败。
	************************************************************************/
	virtual bool ColorControl(long chnn, unsigned long cmdtype, DD_VIDEO_COLOR& videoColor);

	/*
    function:
		镜像，翻转，走廊模式，畸变校正，自动对焦，ROI(感兴趣点)
    input:
		pchar 发送的内容
	return:
	    成功true, 失败 false 
	*/

	virtual bool OtherControl(long chnn, unsigned long cmdtype, unsigned char *pChar);

	/************************************************************************
	接口功能描述：设备的报警信息。
	输出参数描述：
	返回值描述：无。
	************************************************************************/
	virtual ULONGLONG GetMotionAlarm();
	virtual ULONGLONG GetSensorAlarm();
	virtual ULONGLONG GetVlossAlarm();
	virtual ULONGLONG GetOtherAlarm();

	//
	virtual long SetConfig(long chnn, unsigned long itemID, const unsigned char* pData, unsigned long dataLen, bool bImmediately = true);
	virtual long FlushConfig();

	/************************************************************************
	设置时间
	************************************************************************/
	virtual long SetDateTime(const DD_TIME& ddTm);

	/************************************************************************
	获取设备支持参数信息, videoIndex取值0，1，2表示哪个码流
	************************************************************************/
	virtual long GetDevInfo(long videoIndex, ND_DEV_INFO& ndDevInfo);
	virtual long GetEncodeInfo(char * pVideoEncodeInfo, int iVideoEncodeInfoLen);

	virtual bool GetLastError(std::string& strErr){return true;};
	virtual long GetLastError() {return 0;};
	virtual void SetLastError(long err){};

	virtual void OnTimer(unsigned long curTime);

	/***********************************************************************
	设置网络超时时间,单位ms
	***********************************************************************/
	virtual void SetNetTimeout(unsigned long msTimeout);
	virtual unsigned long GetDevResolutionVal( int* width, int* height );

	//定时同步IPC的时间
	virtual int ResyncIPCSystemTime(int sync_flag);

protected:

protected:
	bool InitParam();
	void QuitParam();
	//数据处理成功返回0，否则返回-1，此时会进入阻塞状态
	static int ReceiveCallback(long clientID, void* pParam, RECV_DATA_BUFFER *pDataBuffer);
	int ProcessData(RECV_DATA_BUFFER *pDataBuffer);
	void ProcRecvCmd(unsigned long cmdType, unsigned char *pData, unsigned long datalen);
	
	bool ProcessFrame(const unsigned char *pBuf, int iLen);
	bool CombineFrame(const unsigned char *pBuf, int iLen);
	bool ProcessWaitFrame();

	bool WaitReplyData();

	bool PostCmd(unsigned long iCmdType, unsigned char* pData, unsigned long dataLen);
	bool SendCmd(unsigned long iCmdType, unsigned char* pData, unsigned long dataLen);

	void EncryptLoginInfo(void *pLoginInfo);

	bool SetInitConfig();

	unsigned long DDPtzToIpcPTZ(unsigned long ddPtzCmd);

	bool WaitForReady();

protected:
	_ts_netdevice_param*		m_pTSNetDeviceParam;

	CTSCfgToIpc*				m_pTsCfgToIpc;
	CTSNdParam*					m_pTsNdParam;

	int                         m_iIpcUsedChipFlag;

	int                         m_iAudioFrameMaxCnt;

	int                         m_localVideoNum;
};

#endif