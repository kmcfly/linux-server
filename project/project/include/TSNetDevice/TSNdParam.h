/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : L
** Date         : 2011-04-23
** Name         : TSNdParam.h
** Version      : 1.0
** Description  : IPC状态参数, 可能需要更新的部分
** Modify Record:
1:创建
***********************************************************************/

#ifndef _TS_ND_PARAM_H_
#define _TS_ND_PARAM_H_

#include "NetDevicePlatform.h"
#include <vector>
#include "ipc_v3/platform_ipc_v3.h"
#include <list>

const unsigned long TS_MAX_STREAM_NUM = 10;

const unsigned long DVR_DEFAULT_COLOR_BRIGHT = 128;
const unsigned long DVR_DEFAULT_COLOR_CONTRAST = 128;
const unsigned long DVR_DEFAULT_COLOR_SATURATION = 128;
const unsigned long DVR_DEFAULT_COLOR_HUE = 128;

unsigned long inline DVRColorToIPCColor(unsigned long dvrColor, unsigned long dvrDefaColor, unsigned long ipcDefaColor, unsigned long ipcMaxColor)
{
	if(dvrColor < dvrDefaColor)
	{
		return ipcDefaColor * dvrColor/dvrDefaColor;
	}
	else if(dvrColor == dvrDefaColor)
	{
		return ipcDefaColor;
	}
	else
	{
		return (ipcMaxColor-ipcDefaColor)*(dvrColor-dvrDefaColor)/(255-dvrDefaColor) + ipcDefaColor;
	}
}

class CTSNdParam
{
public:
	CTSNdParam();
	virtual ~CTSNdParam();

	///////////////////////////////////////////////////
	//初始化, 反初始化
	bool Initial();
	void Quit();

	////////////////////////////////////////////////////
	//获取

	//网络设备参数信息
	void GetDevInfo(long videoIndex, ND_DEV_INFO& devInfo);

	long GetEncodeInfo(char * pVideoEncodeInfo, int iVideoEncodeInfoLen);
	//得到码流个数
	int GetStreamNum(){return m_streamNum;}

	//得到第三码流的序号
	int GetNvrShowStreamNum(){return m_NvrShowStreamNum;}

	//获取支持的分辨率 DD_VIDEO_SIZE
	unsigned long GetVideoSize();

	//获取Motion区域块[x,y,cx,cy]
	void GetMotionArea(unsigned long& x, unsigned long& y, unsigned long& cx, unsigned long& cy);

	////////////////////////////////////////////////////
	//设置

	//获取需要初始化参数设置的数据
	bool GetInitialBuff(unsigned char*& pBuff, unsigned long& dataLen);

	//设置哪些 有变化的item项
	void SetModify(unsigned long itemID);

	//检查是否需要重新从IPC读取数据
	bool IsUpdate();

	//需要重新从IPC读取数据的命令Buff
	bool GetBuff(unsigned char*& pBuff, unsigned long& dataLen);

	//通过IPC的配置, 设置参数
	void SetParamByCfg(unsigned char* pData, unsigned long dataLen);

	//通过IPC的登录返回，设置IPCAM的系统属性
	void SetParamByLoginReply(const IPC_V3::NET_LOGIN_SUCCESS_REPLY &loginReply);

	void GetMaxColorValue(IPC_V3::NET_CHNN_COLOR &maxColorVal);
	void GetDefaColorValue(IPC_V3::NET_CHNN_COLOR &defaultColor);
	void GetTimeStamp(IPC_V3::NCFG_TIME_STAMP_CONFIG &timeStamp);
	void GetCamName(IPC_V3::NCFG_INFO_DEV_NAME_EX &camName);
	int GetSensorInNum() const;

	unsigned long GetEncodeLevel(unsigned long stream);
	unsigned long GetIFrameInterval(unsigned long stream);

protected:
	//缓存
	unsigned char*	m_p_cfg_cmd_buff;
	unsigned long	m_cfg_cmd_len;
	unsigned long	m_cfg_cmd_len_max;

	//网络设备信息
	ND_DEV_INFO		m_dev_info[TS_MAX_STREAM_NUM];

	int				m_streamNum;
	int             m_NvrShowStreamNum;

	//motion区域的起始 [x, y, cx, cy]
	unsigned long m_motion_x;
	unsigned long m_motion_y;
	unsigned long m_motion_cx;
	unsigned long m_motion_cy;

	//记录变化了的IPC item 项
	std::vector<unsigned long>	m_modify_item_vec;

	IPC_V3::NET_CHNN_COLOR	m_maxColorValue;
	IPC_V3::NET_CHNN_COLOR	m_defaColorValue;
	IPC_V3::NCFG_TIME_STAMP_CONFIG	m_timeStamp;
	IPC_V3::NCFG_INFO_DEV_NAME_EX	m_CamName;
	IPC_V3::NET_LOGIN_SUCCESS_REPLY	m_ipcDeviceParam;

	unsigned long m_encodeLevel[TS_MAX_STREAM_NUM];
	unsigned long m_IFrameInterval[TS_MAX_STREAM_NUM];

	std::list<CHN_VIDEO_ENCODE_INFO_DETAIL> m_firstVideoEncodeInfoList;
	std::list<CHN_VIDEO_ENCODE_INFO_DETAIL> m_secondVideoEncodeInfoList;
};

#endif

//end
