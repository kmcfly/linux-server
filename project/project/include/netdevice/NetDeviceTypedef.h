#ifndef _NET_DEVICE_TYPEDEF_
#define _NET_DEVICE_TYPEDEF_

#ifdef __ENVIRONMENT_WIN32__
#pragma pack(4)
#endif

enum NET_DEVICE_STATUS
{
	ND_STATUS_OK,				//连接正常
	ND_STATUS_DISCONNECT,		//连接已经断开
};

enum NET_DEVICE_STATUS_CONNECT
{
	ND_SC_READY,				//就绪
	ND_SC_ERROR,				//连接断开或超时

	ND_SC_BUSY			= 100,	//繁忙
	ND_SC_GET_CFG,				//获取配置
	ND_SC_SET_CFG,				//设置配置
	ND_SC_CTRL_COLOR,			//控制色彩
	ND_SC_CTRL_PTZ				//控制云台
};

enum NET_DEVICE_CTRL_COLOR
{
	ND_CTRL_COLOR_NULL,			//控制色彩
	ND_CTRL_COLOR_GET,			//获取
	ND_CTRL_COLOR_SET,			//设置[只针对图像]
	ND_CTRL_COLOR_SAVE,			//设置并保存到配置
	ND_CTRL_COLOR_GET_DEFAULT,	//获取默认
	ND_CTRL_COLOR_SET_DEFAULT	//设置默认
};
enum NET_DEVICE_CTRL_OTHER
{
	ND_CTRL_OTHER_MIRROR,
	ND_CTRL_OTHER_FLIP,
	ND_CTRL_OTHER_ROTATO,
	ND_CTRL_OTHER_AUTOFOCUS,
	ND_CTRL_OTHER_DISTORTION,
    ND_CTRL_OTHER_ROI,
};


//////////////////////////////////////////////////////////////////////////
// 错误值定义, 以下是通用错误
// 错误值段由NVR分配各厂商

enum NET_DEVICE_ERR_COMMON
{
	ND_ERR_OK				= 0,	//正常 "Ok"
	ND_ERR_UNKNOWN			= 1,	//未知错误 "Unknown"

	ND_ERR_CUSTOM			= 1000	//以下为厂商自定区 "Custom"
};

#ifdef	__ENVIRONMENT_WIN32__
#pragma pack()
#endif

#endif
//end
