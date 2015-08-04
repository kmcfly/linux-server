
#ifndef _CONFIG_PARSER_H_
#define _CONFIG_PARSER_H_

#include "dvrdvsconfig.h"
//#include "DVR_NET_SDK.h"

typedef struct _net_sdk_deviceinfo
{
	long videoInputNum;
	long netVideoNum;
	long sensorOutputNum;
	long audioInputNum;
	long sensorInputNum;
}NET_SDK_DEVICEINFO, *LPNET_SDK_DEVICEINFO;

class CConfigParser
{
public:
	CConfigParser(void){};
	virtual ~CConfigParser(void){};

	virtual void SetVideoInputNum(long localVideoNum, long netVideoNum) = 0;
	virtual void SetSensorInNum(long sensorInNum) = 0;

	//由SDK接口类型的配置项转换为设备协议支持的请求数据格式(请求配置时用)
	virtual long GetCfgRequestParam(DD_CONFIG_ITEM_ID configItem, void *pCfgParam, long cfgParamLen) = 0;

	//由设备返回的配置信息转换为SDK可以识别的配置信息(请求配置时用)
	//传入参数为设备配置信息及长度、保存转换成SDK的配置的指针及空间长度
	//返回转换后的SDK配置内容字节长度
	virtual long CfgDevice2CfgSDK(void *pDeviceCfg, long deviceCfgLen, unsigned char*& pSDKCfg, long& sdkCfgLen, DD_CONFIG_ITEM_HEAD* pDDcfgHead) = 0;

	//由SDK组装的配置信息转换为设备可识别的配置信息(保存配置时用)
	//传入参数为SDK配置信息及长度、保存转换成设备配置的指针及空间长度
	//返回转换后的设备配置内容字节长度
	virtual long CfgSDK2CfgDevice(void *pSDKCfg, long sdkCfgLen, void *pDeviceCfg, long deviceCfgLen) = 0;

	//登陆后从netdeivce类中获取到设备的信息
	virtual void SetDeivceInfo(LPNET_SDK_DEVICEINFO deviceInfo) = 0;
};

#endif
