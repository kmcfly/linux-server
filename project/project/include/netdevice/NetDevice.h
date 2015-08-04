/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : 袁石维
** Date         : 2009-09-10
** Name         : NetDevice.h
** Version      : 1.0
** Description  :
** Modify Record:
1:
***********************************************************************/
#ifndef _NET_DEVICE_H_
#define _NET_DEVICE_H_
/************************************************************************
1、	本文件用于描述DVR设备连接IP数字设备时的接口规范；
2、	目标是通过简单规范地接口，实现对多种IP设备的兼容，如果能基于统一的标准
则更好。
3、	IP设备传输视音频如果支持RTSP，则是否就可以统一接入方式实现对视音频的传输
工作？如果不能则需要对每种设备实现接口类。
4、	除视音频外的控制和通信流，可以采用XML规范来做，这样就不需要暴露具体的设计
细节，而达到兼容的目的。
5、	考虑到以后要兼容其他厂家的设备，及实现互信互通，所以接口上就采用以上描述
的方式来做。
6、	每个设备只限于开启一个线程。
************************************************************************/

#include <string>
#include <list>

#include "NetDeviceTypedef.h"
#include "NetDeviceDefine.h"
#include "IPDataBufferMan.h"

class CNetDevice
{
public:
	CNetDevice()
	{
		m_reConnectCount = -1;
		m_bUseBaseLine = false;
	}
	virtual ~CNetDevice()
	{
		m_reConnectCount = -1;
	}

	//////////////////////////////////////////////////////////////////////////
	/************************************************************************
	接口功能描述：初始化设备接口。
	输入参数描述：
	1、chnn：	分配给该设备的通道号；
	2、IP：		该设备的网络地址；
	3、user：	用于登录的用户名称；
	4、netPort：登录端口
	5、httpPort：IE连接端口
	6、ctrlPort： 控制端口
	4、password：用于登录的密码；
	5、mac:		本地设备的网络物理地址，用于登录验证使用。
	返回值描述：返回成功或者失败。
	注: 在该接口内部，需要尝试登录到IP设备，根据登录的结果返回结果。
	注: 在接口内部, 处理重复初始化
	注: 各个设备接口, 必须依赖连接状态做相应处理
	************************************************************************/
	virtual bool Initial(nd_in long deviceId,						//
						 nd_in long chnn,							//
						 nd_in char *pServer,						//
						 nd_in unsigned short netPort,				//
						 nd_in unsigned short httpPort,				//
						 nd_in unsigned short ctrlPort,				//
						 nd_in const std::string & user,			//
						 nd_in const std::string & password,		//
						 nd_in const ND_MAC & mac,					//
						 nd_in const char* pEther="eth0",
						 nd_in long sensorNum=0) = 0;				//
	

	/************************************************************************
	接口功能描述 : 反初始化
	************************************************************************/
	virtual void Quit() = 0;

	/************************************************************************
	是否需要在连接时设置网络设备的配置参数
	************************************************************************/
	virtual bool IfNeedInitNetConfig(){return true;}

	/************************************************************************
	接口功能描述 : 设置数据缓存管理器
	1、本函数由NVR调用.
	2、收到网络图像数据(H264)后对其填充.
	************************************************************************/
	virtual void SetDataBufferMan(nd_in CIPDataBufferMan *pDataBufferMan) = 0;

	/************************************************************************
	接口功能描述 : 设置是否使用baseline编码
	************************************************************************/
	virtual void SetUseBaseLine(bool bUseBaseLine){m_bUseBaseLine = bUseBaseLine;}

	/***********************************************************************
	修改设备绑定的通道号
	************************************************************************/
	virtual void SetBindChannel(nd_in long chnn) = 0;
	virtual long GetBindChannel() = 0;

	virtual void SetLocalVideoNum(long localNum) =0;
	virtual unsigned int GetLocalVideoNum() =0;

	/***********************************************************************
	修改设备绑定的传感器通道号
	************************************************************************/
	virtual void SetBindSensorChannel(long sensorChnn)=0;
	virtual long GetBindSensorChannel()=0;

	/***********************************************************************
	得到该网络设备的状态,断开或正常
	************************************************************************/
	virtual long GetDevStatus() = 0;
	
	/***********************************************************************
	查看网络设备是否处于重连中
	************************************************************************/
	virtual bool IsReConnectting()
	{
		if (0 <= m_reConnectCount)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	/************************************************************************
	得到当前连接状态, ND_SC_BUSY以下, 认为不能处理后续命令                                                                    
	************************************************************************/
	virtual long IsReady() = 0;

	/************************************************************************
	接口功能描述：请求视频及音频数据流。
	输入参数描述：
	1、video1：主码流MASK。
	2、video2：次码流MASK。
	3、video3：手机码流MASK。
	4、video4：音频流MASK。
	返回值描述：返回成功或者失败。
	注：对应的位为零，表示停止。
	************************************************************************/
	virtual bool RequestStream( nd_in ULONGLONG video1,
								nd_in ULONGLONG video2,
								nd_in ULONGLONG video3,
								nd_in ULONGLONG audio) = 0;

	/************************************************************************
	接口功能描述：请求图片数据流。
	输入参数描述：
	1、channlMask: 图片流通道MASK。
	返回值描述：返回成功或者失败。
	注：对应的位为零，表示停止。
	************************************************************************/
	virtual bool RequestJpegStream(nd_in ULONGLONG channlMask){return true;}

	/************************************************************************
	接口功能描述：请求视频关键帧
	输入参数描述：
	1、video1：主码流MASK。
	2、video2：次码流MASK。
	3、video3：手机码流MASK。
	返回值描述：返回成功或者失败。
	注：对应的位为零，表示停止。
	************************************************************************/
	virtual bool RequestKeyFrame(nd_in ULONGLONG video1, 
								 nd_in ULONGLONG video2,
								 nd_in ULONGLONG video3)
	{
		return true;
	}


	/************************************************************************
	接口功能描述：云台控制接口。
	输入参数描述：
	************************************************************************/
	virtual bool PTZControl(nd_in long chnn,
							nd_in unsigned long cmdtype,
							nd_in unsigned long ptzParam,
							nd_in unsigned long speed,
							nd_in unsigned long waiteTime) = 0;


	/************************************************************************
	接口功能描述：获取图像色彩信息。
	************************************************************************/
	virtual bool ColorControl(nd_in long chnn,
							  nd_in unsigned long cmdtype,
							  nd_in_out DD_VIDEO_COLOR& videoColor) = 0;
    /*
    function:
	镜像，翻转，走廊模式，畸变校正，自动对焦，ROI(感兴趣点)
	*/
	virtual bool OtherControl(nd_in long chnn,
							  nd_in unsigned long cmdtype,
							  nd_in unsigned char *pChar) = 0;


	/************************************************************************
	接口功能描述：设备的报警信息。
	输出参数描述：
	1、alarm：报警信息
	返回值描述：无。
	************************************************************************/
	virtual ULONGLONG GetMotionAlarm() = 0;
	virtual ULONGLONG GetSensorAlarm() = 0;
	virtual ULONGLONG GetVlossAlarm() = 0;
	virtual ULONGLONG GetOtherAlarm() = 0;


	/************************************************************************
	接口功能描述 : 设置配置
	1、
	************************************************************************/
	virtual long SetConfig( nd_in long chnn,
							nd_in unsigned long itemID,
							nd_in const unsigned char* pData,
							nd_in unsigned long dataLen,
							nd_in bool bImmediately) = 0;

	virtual long FlushConfig() = 0;

	/************************************************************************
	设置时间
	************************************************************************/
	virtual long SetDateTime(nd_in const DD_TIME& ddTm) = 0;

	/************************************************************************
	获取设备支持参数信息, videoIndex取值0，1，2表示哪个码流
	************************************************************************/
	virtual long GetDevInfo(nd_in long videoIndex, nd_out ND_DEV_INFO& ndDevInfo) = 0;
	virtual long GetEncodeInfo(char * pVideoEncodeInfo, int iVideoEncodeInfoLen) = 0;

	/************************************************************************
	接口功能描述 : 错误处理 
	1、正整型为常规错误, 负整数为严重错误
	************************************************************************/
	virtual bool GetLastError(nd_out std::string& strErr) = 0;
	virtual long GetLastError() = 0;
	virtual void SetLastError(nd_in long err) = 0;

	/************************************************************************
	每秒钟调用一次，通知当前时间，模块可用此作为定时器
	************************************************************************/
	virtual void OnTimer(unsigned long curTime) = 0;

	/***********************************************************************
	设置网络超时时间,单位ms
	***********************************************************************/
	virtual void SetNetTimeout(unsigned long msTimeout) = 0;

	/***********************************************************************
	获取设备码流是否大于300万
	***********************************************************************/
	virtual unsigned long GetDevResolutionVal( int* width, int* height ) = 0;

	//定时同步IPC的时间
	virtual int ResyncIPCSystemTime(int sync_flag) = 0;

protected:
	int m_reConnectCount;
	bool m_bUseBaseLine;
};

#endif //_NET_DEVICE_H_

//end
