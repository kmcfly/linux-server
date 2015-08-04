
#ifndef _CONFIG_PARSER_V3_H_
#define _CONFIG_PARSER_V3_H_

#include "ConfigParser.h"
#include "NetConfigDefine.h"
//#include "NetProtocol.h"
#include "ConfigBlock_V4.h"
#include "ConfigBlock_V3.h"

#ifdef __ENVIRONMENT_WIN32__
#include <atltypes.h>
#endif

#include <stdlib.h>

//SDK���ö�Ӧ�豸��������
const unsigned short DD_DEVICE_INFO_NUM = 3;
const unsigned short DD_BASIC_CONFIG_NUM = 10;
const unsigned short DD_DATE_TIME_CONFIG_NUM = 6;
const unsigned short DD_DAYLIGHT_INFO_NUM = 1;
const unsigned short DD_LIVE_VIDEO_GROUP_NUM = 9;
const unsigned short DD_LIVE_SPOT_GROUO_NUM = 3;
const unsigned short DD_LIVE_AUDIO_GROUP_NUM = 2;
const unsigned short DD_LIVE_DISPLAY_NUM = 3;
const unsigned short DD_CHANNEL_CONFIG_NUM = 2;
const unsigned short DD_VIDEO_COLOR_NUM = 1;
const unsigned short DD_VIDEO_OSD_CONFIG_NUM = 3;
const unsigned short DD_VIDEO_ROI_CONFIG_NUM = 1;
const unsigned short DD_CAM_PARAMTER_CONFIG_NUM = 1;
const unsigned short DD_ENCODE_CONFIG_MASK_NUM = 4;
const unsigned short DD_ENCODE_CONFIG_NUM = 1;
const unsigned short DD_ENCODE_CONFIG_JPEG_NUM = 1;
const unsigned short DD_ENCODE_OTHER_CONFIG_NUM = 1;
const unsigned short DD_RECORD_CONFIG_NUM = 6;
const unsigned short DD_WEEK_SCHEDULE_NUM = 1;
const unsigned short DD_NETWORK_IP_CONFIG_NUM = 4;
const unsigned short DD_NETWORK_ADVANCE_CONFIG_NUM = 5;
const unsigned short DD_DDNS_CONFIG_NUM = 2;
const unsigned short DD_SMTP_CONFIG_NUM = 2;
const unsigned short DD_ACCOUNT_CONFIG_NUM = 1;
const unsigned short DD_SENSOR_CONFIG_NUM = 4;
const unsigned short DD_SENSOR_SCHEDULE_NUM = 1;
const unsigned short DD_TRIGGER_ALARM_OUT_NUM = 4;
const unsigned short DD_TRIGGER_RECORD_NUM = 2;
const unsigned short DD_TRIGGER_PTZ_NUM = 1;
const unsigned short DD_MOTION_CONFIG_NUM = 3;
const unsigned short DD_MOTION_SCHEDULE_NUM = 1;
const unsigned short DD_RELAY_CONFIG_NUM = 2;
const unsigned short DD_BUZZER_CONFIG_NUM = 2;
const unsigned short DD_PTZ_CONFIG_NUM = 1;
const unsigned short DD_PTZ_PRESET_CONFIG_NUM = 1;
const unsigned short DD_PTZ_CURISE_CONFIG_NUM = 1;
const unsigned short DD_CONFIG_ITEM_RECORD_MASK_NUM = 1;

const unsigned short CRUISE_NUMBER_MAX = 8;

typedef struct cfg_item_info
{
	DD_CONFIG_ITEM_ID	itemID;
	unsigned long		parserLen;
	unsigned char		*pConfigItem;
}ITEM_INFO;

class CConfigParser_V3 : public CConfigParser
{
public:
	CConfigParser_V3(void);
	~CConfigParser_V3(void);

	virtual void SetVideoInputNum(long localVideoNum, long netVideoNum);
	virtual void SetSensorInNum(long sensorInNum);

	//��SDK�ӿ����͵�������ת��Ϊ�豸Э��֧�ֵ��������ݸ�ʽ(��������ʱ��)
	virtual long GetCfgRequestParam(DD_CONFIG_ITEM_ID configItem, void *pCfgParam, long cfgParamLen);

	//���豸���ص�������Ϣת��ΪSDK����ʶ���������Ϣ(��������ʱ��)
	//�������Ϊ�豸������Ϣ�����ȡ�����ת����SDK�����õ�ָ�뼰�ռ䳤��
	//����ת�����SDK���������ֽڳ���
	virtual long CfgDevice2CfgSDK(void *pDeviceCfg, long deviceCfgLen, unsigned char*& pSDKCfg, long& sdkCfgLen, DD_CONFIG_ITEM_HEAD* pDDcfgHead);

	//��SDK��װ��������Ϣת��Ϊ�豸��ʶ���������Ϣ(��������ʱ��)
	//�������ΪSDK������Ϣ�����ȡ�����ת�����豸���õ�ָ�뼰�ռ䳤��
	//����ת������豸���������ֽڳ���
	virtual long CfgSDK2CfgDevice(void *pSDKCfg, long sdkCfgLen, void *pDeviceCfg, long deviceCfgLen);

	virtual void SetDeivceInfo(LPNET_SDK_DEVICEINFO deviceInfo);
private:
	LONG GetCfgItemLength(unsigned short itemNum, long cfgParamLen);
	void SetCfgItemInfo(unsigned short itemNum, unsigned char *pBuf, unsigned short *pArray);	

	unsigned long ToSDKResolution(unsigned long videoSize);
private:
	long			m_localVideoNum;
	long			m_netVideoNum;
	long			m_sensorInNum;
	NET_SDK_DEVICEINFO m_deiviceInfo;
};

#endif

