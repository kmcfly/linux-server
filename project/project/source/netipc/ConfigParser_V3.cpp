#include "ConfigParser_V3.h"
#include "NetDevicePlatform.h"

#include <stdio.h>
#include <cmath>
#include <assert.h>
#include "dvrdvsdef.h"
#ifdef __ENVIRONMENT_WIN32__
#include "stdafx.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#endif

#pragma warning(disable:4244)

extern unsigned long DDVideoSizeToVideoSize(unsigned long ddvideoSize);
extern unsigned long VideoSizeToDDVideoSize(unsigned long videoSize);

unsigned long MyVRateToRate(unsigned long vRate, DD_VIDEO_FORMAT videoFormat)
{
	assert ((5 <= vRate) && (vRate <= 150));

	if (DD_VIDEO_FORMAT_NTSC == videoFormat)
	{
		unsigned long rate = (vRate + 2) / 5;
		return (0 < rate) ? rate : 1;
	}
	else
	{
		unsigned long rate = (vRate + 3) / 6;
		return (0 < rate) ? rate : 1;
	}
}

unsigned long MyRateToVRate(unsigned long rate, DD_VIDEO_FORMAT videoFormat)
{
	if (DD_VIDEO_FORMAT_NTSC == videoFormat)
	{
		assert ((1 <= rate) && (rate <= 30));

		return (rate * 5);
	}
	else
	{
		assert ((1 <= rate) && (rate <= 25));
		return (rate * 6);
	}
}

CConfigParser_V3::CConfigParser_V3()
{
	m_localVideoNum = 0;
	m_netVideoNum = 0;
	m_sensorInNum = 0;

	memset(&m_deiviceInfo, 0, sizeof(NET_SDK_DEVICEINFO));
}

CConfigParser_V3::~CConfigParser_V3()
{

}

void CConfigParser_V3::SetVideoInputNum(long localVideoNum, long netVideoNum)
{
	m_localVideoNum = localVideoNum;
	m_netVideoNum = netVideoNum;
}

void CConfigParser_V3::SetSensorInNum(long sensorInNum)
{
	m_sensorInNum = sensorInNum;
}

long CConfigParser_V3::GetCfgRequestParam(DD_CONFIG_ITEM_ID configItem, void *pCfgParam, long cfgParamLen)
{
	long ItemLength = 0;
	switch(configItem)
	{
		case DD_CONFIG_ITEM_DEVICE_INFO:
			{
				ItemLength = GetCfgItemLength(DD_DEVICE_INFO_NUM, cfgParamLen);
				if (ItemLength > 0)
				{
					unsigned char *pBuf = new unsigned char[ItemLength];

					unsigned short ItemArray[DD_DEVICE_INFO_NUM] = {NCFG_ITEM_DEVICE_ID,NCFG_ITEM_DEVICE_NAME, NCFG_ITEM_LIVE_AUDIO_ENABLE};

					SetCfgItemInfo(DD_DEVICE_INFO_NUM, pBuf, ItemArray);
					memcpy(pCfgParam, pBuf, ItemLength);
					delete []pBuf;
					pBuf =	NULL;
				}
				break;
			}
		case DD_CONFIG_ITEM_SYSTEM_BASIC:
			{
				ItemLength = GetCfgItemLength(DD_BASIC_CONFIG_NUM, cfgParamLen);
				if (ItemLength > 0)
				{
					unsigned char *pBuf = new unsigned char[ItemLength];

					unsigned short ItemArray[DD_BASIC_CONFIG_NUM] = {NCFG_ITEM_VIDEO_FORMAT, NCFG_ITEM_AUTHORIZATION,NCFG_ITEM_LANGUAGE, NCFG_ITEM_SCRENN_SAVE_TIME, NCFG_ITEM_REC_RECYCLE, NCFG_ITEM_VGA_RESOLUTION, NCFG_ITEM_VGA_REFRESH, NCFG_ITEM_SUPPORT_VIDEOFORMAT, NCFG_ITEM_SUPPORT_LANGUAGE, NCFG_ITEM_VGA_SUPPORT_RESOLUTION};

					SetCfgItemInfo(DD_BASIC_CONFIG_NUM, pBuf, ItemArray);
					memcpy(pCfgParam, pBuf, ItemLength);
					delete []pBuf;
					pBuf =	NULL;
				}
				break;
			}
		case DD_CONFIG_ITEM_DATE_TIME:
			{
				ItemLength = GetCfgItemLength(DD_DATE_TIME_CONFIG_NUM, cfgParamLen);
				if (ItemLength > 0)
				{
					unsigned char *pBuf = new unsigned char[ItemLength];

					unsigned short ItemArray[DD_DATE_TIME_CONFIG_NUM] = {NCFG_ITEM_DATE_FORMAT, NCFG_ITEM_HOUR_FORMAT, NCFG_ITEM_TIME_ZONE, NCFG_ITEM_TIME_SYNC, NCFG_ITEM_TIME_SERVER, NCFG_ITEM_NTP_PORT};

					SetCfgItemInfo(DD_DATE_TIME_CONFIG_NUM, pBuf, ItemArray);
					memcpy(pCfgParam, pBuf, ItemLength);
					delete []pBuf;
					pBuf =	NULL;
				}
				break;
			}
		case DD_CONFIG_ITEM_DAYLIGHT_INFO:
			{
				ItemLength = GetCfgItemLength(DD_DAYLIGHT_INFO_NUM, cfgParamLen);
				if (ItemLength > 0)
				{
					unsigned char *pBuf = new unsigned char[ItemLength];

					unsigned short ItemArray[DD_DAYLIGHT_INFO_NUM] = {NCFG_ITEM_DAYLIGHT_INFO};

					SetCfgItemInfo(DD_DAYLIGHT_INFO_NUM, pBuf, ItemArray);
					memcpy(pCfgParam, pBuf, ItemLength);
					delete []pBuf;
					pBuf =	NULL;
				}
				break;
			}
		case DD_CONFIG_ITEM_LIVE_MAIN_CAMERA:
			{
				ItemLength = GetCfgItemLength(DD_LIVE_VIDEO_GROUP_NUM, cfgParamLen);
				if (ItemLength > 0)
				{
					unsigned char *pBuf = new unsigned char[ItemLength];

					unsigned short ItemArray[DD_LIVE_VIDEO_GROUP_NUM] = {NCFG_ITEM_VIEW_SPLIT_MODE,NCFG_ITEM_VIEW_DWELL_TIME, NCFG_ITEM_VIEW_SPLIT_1X1, NCFG_ITEM_VIEW_SPLIT_2X2, NCFG_ITEM_VIEW_SPLIT_2X3, NCFG_ITEM_VIEW_SPLIT_3X3, NCFG_ITEM_VIEW_SPLIT_4X4, NCFG_ITEM_VIEW_SPLIT_5X5, NCFG_ITEM_VIEW_SPLIT_6X6};

					SetCfgItemInfo(DD_LIVE_VIDEO_GROUP_NUM, pBuf, ItemArray);
					memcpy(pCfgParam, pBuf, ItemLength);
					delete []pBuf;
					pBuf =	NULL;
				}
				break;
			}
		case DD_CONFIG_ITEM_LIVE_SPOT_CAMERA:
			{
				ItemLength = GetCfgItemLength(DD_LIVE_SPOT_GROUO_NUM, cfgParamLen);
				if (ItemLength > 0)
				{
					unsigned char *pBuf = new unsigned char[ItemLength];

					unsigned short ItemArray[DD_LIVE_SPOT_GROUO_NUM] = {NCFG_ITEM_SPOT_SPLIT_MODE, NCFG_ITEM_SPOT_DWELL_TIME, NCFG_ITEM_SPOT_SPLIT_1X1};

					SetCfgItemInfo(DD_LIVE_SPOT_GROUO_NUM, pBuf, ItemArray);
					memcpy(pCfgParam, pBuf, ItemLength);
					delete []pBuf;
					pBuf =	NULL;
				}
				break;
			}
		case DD_CONFIG_ITEM_LIVE_AUDIO:
			{
				ItemLength = GetCfgItemLength(DD_LIVE_AUDIO_GROUP_NUM, cfgParamLen);
				if (ItemLength > 0)
				{
					unsigned char *pBuf = new unsigned char[ItemLength];

					unsigned short ItemArray[DD_LIVE_AUDIO_GROUP_NUM] = {NCFG_ITEM_LIVE_AUDIO_CHNN,NCFG_ITEM_LIVE_AUDIO_VOLUME};

					SetCfgItemInfo(DD_LIVE_AUDIO_GROUP_NUM, pBuf, ItemArray);
					memcpy(pCfgParam, pBuf, ItemLength);
					delete []pBuf;
					pBuf =	NULL;
				}
				break;
			}
		case DD_CONFIG_ITEM_LIVE_DISPLAY:
			{
				ItemLength = GetCfgItemLength(DD_LIVE_DISPLAY_NUM, cfgParamLen);
				if (ItemLength > 0)
				{
					unsigned char *pBuf = new unsigned char[ItemLength];

					unsigned short ItemArray[DD_LIVE_DISPLAY_NUM] = {NCFG_ITEM_LIVE_TIME_INFO, NCFG_ITEM_LIVE_REC_STATUS, NCFG_ITEM_SHOW_CAM_NAME};

					SetCfgItemInfo(DD_LIVE_DISPLAY_NUM, pBuf, ItemArray);
					memcpy(pCfgParam, pBuf, ItemLength);
					delete []pBuf;
					pBuf =	NULL;
				}
				break;
			}
		case DD_CONFIG_ITEM_CHNN_CONFIG:
			{
				ItemLength = GetCfgItemLength(DD_CHANNEL_CONFIG_NUM, cfgParamLen);
				if (ItemLength > 0)
				{
					unsigned char *pBuf = new unsigned char[ItemLength];

					unsigned short ItemArray[DD_CHANNEL_CONFIG_NUM] = {NCFG_ITEM_CAMERA_NAME, NCFG_ITEM_SHOW_CAM_NAME};
					SetCfgItemInfo(DD_CHANNEL_CONFIG_NUM, pBuf, ItemArray);
					memcpy(pCfgParam, pBuf, ItemLength);
					delete []pBuf;
					pBuf =	NULL;
				}
				break;
			}
		case DD_CONFIG_ITEM_VIDEO_COLOR:
			{
				ItemLength = GetCfgItemLength(DD_VIDEO_COLOR_NUM, cfgParamLen);
				if (ItemLength > 0)
				{
					unsigned char *pBuf = new unsigned char[ItemLength];

					unsigned short ItemArray[DD_VIDEO_COLOR_NUM] = {NCFG_ITEM_VIDEO_COLOR};

					SetCfgItemInfo(DD_VIDEO_COLOR_NUM, pBuf, ItemArray);
					memcpy(pCfgParam, pBuf, ItemLength);
					delete []pBuf;
					pBuf =	NULL;
				}
				break;
			}
		case DD_CONFIG_ITEM_VIDEO_OSD:
			{
				ItemLength = GetCfgItemLength(DD_VIDEO_OSD_CONFIG_NUM, cfgParamLen);
				if (ItemLength > 0)
				{
					unsigned char *pBuf = new unsigned char[ItemLength];

					unsigned short ItemArray[DD_VIDEO_OSD_CONFIG_NUM] = {NCFG_ITEM_TIME_STAMP_INFO, NCFG_ITEM_CAM_NAME_STAMP_INFO, NCFG_ITEM_CAM_COVER_INFO};

					SetCfgItemInfo(DD_VIDEO_OSD_CONFIG_NUM, pBuf, ItemArray);
					memcpy(pCfgParam, pBuf, ItemLength);
					delete []pBuf;
					pBuf =	NULL;
				}
				break;
			}
		case DD_CONFIG_ITEM_VIDEO_ROI:    //xian
			{
				ItemLength = GetCfgItemLength(DD_VIDEO_ROI_CONFIG_NUM, cfgParamLen);
				if (ItemLength > 0)
				{
					unsigned char *pBuf = new unsigned char[ItemLength];

					unsigned short ItemArray[DD_VIDEO_ROI_CONFIG_NUM] = {NCFG_ITEM_CAM_ROI_INFO};

					SetCfgItemInfo(DD_VIDEO_ROI_CONFIG_NUM, pBuf, ItemArray);
					memcpy(pCfgParam, pBuf, ItemLength);
					delete []pBuf;
					pBuf =	NULL;
				}
				break;
			}
		case DD_CONFIG_ITEM_CAM_MIRROR:
			{
				ItemLength = GetCfgItemLength(DD_CAM_PARAMTER_CONFIG_NUM, cfgParamLen);
				if (ItemLength > 0)
				{
					unsigned char *pBuf = new unsigned char[ItemLength];

					unsigned short ItemArray[DD_CAM_PARAMTER_CONFIG_NUM] = {NCFG_ITEM_CAM_MIRROR};

					SetCfgItemInfo(DD_CAM_PARAMTER_CONFIG_NUM, pBuf, ItemArray);
					memcpy(pCfgParam, pBuf, ItemLength);
					delete []pBuf;
					pBuf =	NULL;
				}
				break;
			}
		case DD_CONFIG_ITEM_CAM_FILP:
			{
				ItemLength = GetCfgItemLength(DD_CAM_PARAMTER_CONFIG_NUM, cfgParamLen);
				if (ItemLength > 0)
				{
					unsigned char *pBuf = new unsigned char[ItemLength];

					unsigned short ItemArray[DD_CAM_PARAMTER_CONFIG_NUM] = {NCFG_ITEM_CAM_FILP};

					SetCfgItemInfo(DD_CAM_PARAMTER_CONFIG_NUM, pBuf, ItemArray);
					memcpy(pCfgParam, pBuf, ItemLength);
					delete []pBuf;
					pBuf =	NULL;
				}
				break;
			}
		case DD_CONFIG_ITEM_CAM_ROTATO:
			{
				ItemLength = GetCfgItemLength(DD_CAM_PARAMTER_CONFIG_NUM, cfgParamLen);
				if (ItemLength > 0)
				{
					unsigned char *pBuf = new unsigned char[ItemLength];

					unsigned short ItemArray[DD_CAM_PARAMTER_CONFIG_NUM] = {NCFG_ITEM_CAM_ROTATO};

					SetCfgItemInfo(DD_CAM_PARAMTER_CONFIG_NUM, pBuf, ItemArray);
					memcpy(pCfgParam, pBuf, ItemLength);
					delete []pBuf;
					pBuf =	NULL;
				}
				break;
			}
		case DD_CONFIG_ITEM_CAM_LENS_DISTORTION:
			{
				ItemLength = GetCfgItemLength(DD_CAM_PARAMTER_CONFIG_NUM, cfgParamLen);
				if (ItemLength > 0)
				{
					unsigned char *pBuf = new unsigned char[ItemLength];

					unsigned short ItemArray[DD_CAM_PARAMTER_CONFIG_NUM] = {NCFG_ITEM_CAM_LENS_DISTORTION};

					SetCfgItemInfo(DD_CAM_PARAMTER_CONFIG_NUM, pBuf, ItemArray);
					memcpy(pCfgParam, pBuf, ItemLength);
					delete []pBuf;
					pBuf =	NULL;
				}
				break;
			}
		case DD_CONFIG_ITEM_CAM_TIME_FOCUS_MODE:
			{
				ItemLength = GetCfgItemLength(DD_CAM_PARAMTER_CONFIG_NUM, cfgParamLen);
				if (ItemLength > 0)
				{
					unsigned char *pBuf = new unsigned char[ItemLength];

					unsigned short ItemArray[DD_CAM_PARAMTER_CONFIG_NUM] = {NCFG_ITEM_CAM_TIME_FOCUS_MODE};

					SetCfgItemInfo(DD_CAM_PARAMTER_CONFIG_NUM, pBuf, ItemArray);
					memcpy(pCfgParam, pBuf, ItemLength);
					delete []pBuf;
					pBuf =	NULL;
				}
				break;
			}
		case DD_CONFIG_ITEM_CAM_IRCHANGE_FOCUS:
			{
				ItemLength = GetCfgItemLength(DD_CAM_PARAMTER_CONFIG_NUM, cfgParamLen);
				if (ItemLength > 0)
				{
					unsigned char *pBuf = new unsigned char[ItemLength];

					unsigned short ItemArray[DD_CAM_PARAMTER_CONFIG_NUM] = {NCFG_ITEM_CAM_IRCHANGE_FOCUS};

					SetCfgItemInfo(DD_CAM_PARAMTER_CONFIG_NUM, pBuf, ItemArray);
					memcpy(pCfgParam, pBuf, ItemLength);
					delete []pBuf;
					pBuf =	NULL;
				}
				break;
			}
		case DD_CONFIG_ITEM_CAM_INTERVAL_TIME:
			{
				ItemLength = GetCfgItemLength(DD_CAM_PARAMTER_CONFIG_NUM, cfgParamLen);
				if (ItemLength > 0)
				{
					unsigned char *pBuf = new unsigned char[ItemLength];

					unsigned short ItemArray[DD_CAM_PARAMTER_CONFIG_NUM] = {NCFG_ITEM_CAM_INTERVAL_TIME};

					SetCfgItemInfo(DD_CAM_PARAMTER_CONFIG_NUM, pBuf, ItemArray);
					memcpy(pCfgParam, pBuf, ItemLength);
					delete []pBuf;
					pBuf =	NULL;
				}
				break;
			}
		case DD_CONFIG_ITEM_ENCODE_MASK_MAJOR:
			{
				ItemLength = GetCfgItemLength(DD_ENCODE_CONFIG_MASK_NUM, cfgParamLen);
				if (ItemLength > 0)
				{
					unsigned char *pBuf = new unsigned char[ItemLength];

					unsigned short ItemArray[DD_ENCODE_CONFIG_MASK_NUM] = {NCFG_ITEM_ENCODE_MAJOR_SUPPORT_PROPERTY, NCFG_ITEM_MAJOR_SUPPORT_QULITY_LEVELNUM\
					, NCFG_ITEM_MAJOR_SUPPORT_ENCODEMODE, NCFG_ITEM_MAJOR_SUPPORT_BITRATE_RANGE};

					SetCfgItemInfo(DD_ENCODE_CONFIG_MASK_NUM, pBuf, ItemArray);
					memcpy(pCfgParam, pBuf, ItemLength);
					delete []pBuf;
					pBuf =	NULL;
				}
				break;
			}
		case DD_CONFIG_ITEM_ENCODE_MASK_MINOR:
			{
				ItemLength = GetCfgItemLength(DD_ENCODE_CONFIG_MASK_NUM, cfgParamLen);
				if (ItemLength > 0)
				{
					unsigned char *pBuf = new unsigned char[ItemLength];

					unsigned short ItemArray[DD_ENCODE_CONFIG_MASK_NUM] = {NCFG_ITEM_ENCODE_MINOR_SUPPORT_PROPERTY, NCFG_ITEM_MINOR_SUPPORT_QULITY_LEVELNUM,
					NCFG_ITEM_MINOR_SUPPORT_ENCODEMODE, NCFG_ITEM_MINOR_SUPPORT_BITRATE_RANGE};

					SetCfgItemInfo(DD_ENCODE_CONFIG_MASK_NUM, pBuf, ItemArray);
					memcpy(pCfgParam, pBuf, ItemLength);
					delete []pBuf;
					pBuf =	NULL;
				}
				break;
			}
		case DD_CONFIG_ITEM_ENCODE_SCHEDULE:
		case DD_CONFIG_ITEM_ENCODE_ALARM:
			{
				ItemLength = GetCfgItemLength(DD_ENCODE_OTHER_CONFIG_NUM, cfgParamLen);
				if (ItemLength > 0)
				{
					unsigned char *pBuf = new unsigned char[ItemLength];

					unsigned short ItemArray[DD_ENCODE_OTHER_CONFIG_NUM] = {NCFG_ITEM_ENCODE_MAJOR_EX};

					SetCfgItemInfo(DD_ENCODE_OTHER_CONFIG_NUM, pBuf, ItemArray);
					memcpy(pCfgParam, pBuf, ItemLength);
					delete []pBuf;
					pBuf =	NULL;
				}
				break;
			}
		case DD_CONFIG_ITEM_ENCODE_NETWORK:
			{
				ItemLength = GetCfgItemLength(DD_ENCODE_OTHER_CONFIG_NUM, cfgParamLen);
				if (ItemLength > 0)
				{
					unsigned char *pBuf = new unsigned char[ItemLength];

					unsigned short ItemArray[DD_ENCODE_OTHER_CONFIG_NUM] = {NCFG_ITEM_ENCODE_MINOR_EX};

					SetCfgItemInfo(DD_ENCODE_OTHER_CONFIG_NUM, pBuf, ItemArray);
					memcpy(pCfgParam, pBuf, ItemLength);
					delete []pBuf;
					pBuf =	NULL;
				}
				break;
			}
		case DD_CONFIG_ITEM_ENCODE_JPEG:
			{
				ItemLength = GetCfgItemLength(DD_ENCODE_CONFIG_JPEG_NUM, cfgParamLen);
				if (ItemLength > 0)
				{
					unsigned char *pBuf = new unsigned char[ItemLength];

					unsigned short ItemArray[DD_ENCODE_CONFIG_JPEG_NUM] = {NCFG_ITEM_JPEG_SIZE};

					SetCfgItemInfo(DD_ENCODE_CONFIG_JPEG_NUM, pBuf, ItemArray);
					memcpy(pCfgParam, pBuf, ItemLength);
					delete []pBuf;
					pBuf =	NULL;
				}
				break;
			}
		case DD_CONFIG_ITEM_RECORD_MASK:
			{
				ItemLength = GetCfgItemLength(DD_CONFIG_ITEM_RECORD_MASK_NUM, cfgParamLen);
				if (ItemLength > 0)
				{
					unsigned char *pBuf = new unsigned char[ItemLength];

					unsigned short ItemArray[DD_CONFIG_ITEM_RECORD_MASK_NUM] = {NCFG_ITEM_MAX_PRE_ALARM_TIME};

					SetCfgItemInfo(DD_CONFIG_ITEM_RECORD_MASK_NUM, pBuf, ItemArray);
					memcpy(pCfgParam, pBuf, ItemLength);
					delete []pBuf;
					pBuf =	NULL;
				}
				break;
			}
		case DD_CONFIG_ITEM_RECORD_SETUP:
			{
				ItemLength = GetCfgItemLength(DD_RECORD_CONFIG_NUM, cfgParamLen);
				if (ItemLength > 0)
				{
					unsigned char *pBuf = new unsigned char[ItemLength];

					unsigned short ItemArray[DD_RECORD_CONFIG_NUM] = {NCFG_ITEM_REC_ENABLE, NCFG_ITEM_PRE_ALARM_REC_TIME, NCFG_ITEM_POST_ALARM_REC_TIME, NCFG_ITEM_REC_HOLD_TIME, NCFG_ITEM_REC_AUDIO, NCFG_ITEM_REC_REDUNDANT};

					SetCfgItemInfo(DD_RECORD_CONFIG_NUM, pBuf, ItemArray);
					memcpy(pCfgParam, pBuf, ItemLength);
					delete []pBuf;
					pBuf =	NULL;
				}
				break;
			}
		case DD_CONFIG_ITEM_RECORD_SCHEDULE_SCHEDULE:
			{
				ItemLength = GetCfgItemLength(DD_WEEK_SCHEDULE_NUM, cfgParamLen);
				if (ItemLength > 0)
				{
					unsigned char *pBuf = new unsigned char[ItemLength];

					unsigned short ItemArray[DD_WEEK_SCHEDULE_NUM] = {NCFG_ITEM_REC_SCH_SCHEDULE_WEEK};

					SetCfgItemInfo(DD_WEEK_SCHEDULE_NUM, pBuf, ItemArray);
					memcpy(pCfgParam, pBuf, ItemLength);
					delete []pBuf;
					pBuf =	NULL;
				}
				break;
			}
		case DD_CONFIG_ITEM_RECORD_SCHEDULE_MOTION:
			{
				ItemLength = GetCfgItemLength(DD_WEEK_SCHEDULE_NUM, cfgParamLen);
				if (ItemLength > 0)
				{
					unsigned char *pBuf = new unsigned char[ItemLength];

					unsigned short ItemArray[DD_WEEK_SCHEDULE_NUM] = {NCFG_ITEM_REC_SCH_MOTION_WEEK};

					SetCfgItemInfo(DD_WEEK_SCHEDULE_NUM, pBuf, ItemArray);
					memcpy(pCfgParam, pBuf, ItemLength);
					delete []pBuf;
					pBuf =	NULL;
				}
				break;
			}
		case DD_CONFIG_ITEM_RECORD_SCHEDULE_SENSOR:
			{
				ItemLength = GetCfgItemLength(DD_WEEK_SCHEDULE_NUM, cfgParamLen);
				if (ItemLength > 0)
				{
					unsigned char *pBuf = new unsigned char[ItemLength];

					unsigned short ItemArray[DD_WEEK_SCHEDULE_NUM] = {NCFG_ITEM_REC_SCH_SENSOR_WEEK};

					SetCfgItemInfo(DD_WEEK_SCHEDULE_NUM, pBuf, ItemArray);
					memcpy(pCfgParam, pBuf, ItemLength);
					delete []pBuf;
					pBuf =	NULL;
				}
				break;
			}
		case DD_CONFIG_ITEM_NETWORK_IP:
			{
				ItemLength = GetCfgItemLength(DD_NETWORK_IP_CONFIG_NUM, cfgParamLen);
				if (ItemLength > 0)
				{
					unsigned char *pBuf = new unsigned char[ItemLength];

					unsigned short ItemArray[DD_NETWORK_IP_CONFIG_NUM] = {NCFG_ITEM_NETWORK_ADDR_INFO, NCFG_ITEM_DNS_1, NCFG_ITEM_DNS_2, NCFG_ITEM_PPPOE_INFO};

					SetCfgItemInfo(DD_NETWORK_IP_CONFIG_NUM, pBuf, ItemArray);
					memcpy(pCfgParam, pBuf, ItemLength);
					delete []pBuf;
					pBuf =	NULL;
				}
				break;
			}
		case DD_CONFIG_ITEM_NETWORK_ADVANCE:
			{
				ItemLength = GetCfgItemLength(DD_NETWORK_ADVANCE_CONFIG_NUM, cfgParamLen);
				if (ItemLength > 0)
				{
					unsigned char *pBuf = new unsigned char[ItemLength];

					unsigned short ItemArray[DD_NETWORK_ADVANCE_CONFIG_NUM] = {NCFG_ITEM_MAX_CLIENT_NUM, NCFG_ITEM_HTTP_PORT, NCFG_ITEM_SERVER_PORT, NCFG_ITEM_MULTI_NET_ADDR, NCFG_ITEM_SUPPORT_MAXCLIENTNUM};

					SetCfgItemInfo(DD_NETWORK_ADVANCE_CONFIG_NUM, pBuf, ItemArray);
					memcpy(pCfgParam, pBuf, ItemLength);
					delete []pBuf;
					pBuf =	NULL;
				}
				break;
			}
		case DD_CONFIG_ITEM_NETWORK_DDNS:
			{
				ItemLength = GetCfgItemLength(DD_DDNS_CONFIG_NUM, cfgParamLen);
				if (ItemLength > 0)
				{
					unsigned char *pBuf = new unsigned char[ItemLength];

					unsigned short ItemArray[DD_DDNS_CONFIG_NUM] = {NCFG_ITEM_DDNS_INFO, NCFG_ITEM_DDNS_UPDATE_CYCLE};

					SetCfgItemInfo(DD_DDNS_CONFIG_NUM, pBuf, ItemArray);
					memcpy(pCfgParam, pBuf, ItemLength);
					delete []pBuf;
					pBuf =	NULL;
				}
				break;
			}
		case DD_CONFIG_ITEM_NETWORK_SMTP:
			{
				ItemLength = GetCfgItemLength(DD_SMTP_CONFIG_NUM, cfgParamLen);
				if (ItemLength > 0)
				{
					unsigned char *pBuf = new unsigned char[ItemLength];

					unsigned short ItemArray[DD_SMTP_CONFIG_NUM] = {NCFG_ITEM_EMAIL_SEND_INFO, NCFG_ITEM_EMAIL_RECV_INFO};

					SetCfgItemInfo(DD_SMTP_CONFIG_NUM, pBuf, ItemArray);
					memcpy(pCfgParam, pBuf, ItemLength);
					delete []pBuf;
					pBuf =	NULL;
				}
				break;
			}
		case DD_CONFIG_ITEM_DDNS_SERVER_INFO:
			{
				ItemLength = GetCfgItemLength(DD_CONFIG_ITEM_RECORD_MASK_NUM, cfgParamLen);
				if (ItemLength > 0)
				{
					unsigned char *pBuf = new unsigned char[ItemLength];

					unsigned short ItemArray[DD_CONFIG_ITEM_RECORD_MASK_NUM] = {NCFG_ITEM_DDNS_SERVER_INFO};

					SetCfgItemInfo(DD_CONFIG_ITEM_RECORD_MASK_NUM, pBuf, ItemArray);
					memcpy(pCfgParam, pBuf, ItemLength);
					delete []pBuf;
					pBuf =	NULL;
				}
				break;
			}
		case DD_CONFIG_ITEM_ACCOUNT:
			{
				ItemLength = GetCfgItemLength(DD_ACCOUNT_CONFIG_NUM, cfgParamLen);
				if (ItemLength > 0)
				{
					unsigned char *pBuf = new unsigned char[ItemLength];

					unsigned short ItemArray[DD_ACCOUNT_CONFIG_NUM] = {NCFG_ITEM_USER};

					SetCfgItemInfo(DD_ACCOUNT_CONFIG_NUM, pBuf, ItemArray);
					memcpy(pCfgParam, pBuf, ItemLength);
					delete []pBuf;
					pBuf =	NULL;
				}
				break;
			}
		case DD_CONFIG_ITEM_SENSOR_SETUP:
			{
				ItemLength = GetCfgItemLength(DD_SENSOR_CONFIG_NUM, cfgParamLen);
				if (ItemLength > 0)
				{
					unsigned char *pBuf = new unsigned char[ItemLength];

					unsigned short ItemArray[DD_SENSOR_CONFIG_NUM] = {NCFG_ITEM_SENSOR_IN_ENABLE, NCFG_ITEM_SENSOR_IN_TYPE, NCFG_ITEM_SENSOR_IN_NAME, NCFG_ITEM_SENSOR_IN_HOLD_TIME};

					SetCfgItemInfo(DD_SENSOR_CONFIG_NUM, pBuf, ItemArray);
					memcpy(pCfgParam, pBuf, ItemLength);
					delete []pBuf;
					pBuf =	NULL;
				}
				break;
			}
		case DD_CONFIG_ITEM_SENSOR_SCHEDULE:
			{
				ItemLength = GetCfgItemLength(DD_SENSOR_SCHEDULE_NUM, cfgParamLen);
				if (ItemLength > 0)
				{
					unsigned char *pBuf = new unsigned char[ItemLength];

					unsigned short ItemArray[DD_SENSOR_SCHEDULE_NUM] = {NCFG_ITEM_SENSOR_IN_SCH_WEEK};

					SetCfgItemInfo(DD_SENSOR_SCHEDULE_NUM, pBuf, ItemArray);
					memcpy(pCfgParam, pBuf, ItemLength);
					delete []pBuf;
					pBuf =	NULL;
				}
				break;
			}
		case DD_CONFIG_ITEM_SENSOR_ALARM_OUT:
			{
				ItemLength = GetCfgItemLength(DD_TRIGGER_ALARM_OUT_NUM, cfgParamLen);
				if (ItemLength > 0)
				{
					unsigned char *pBuf = new unsigned char[ItemLength];

					unsigned short ItemArray[DD_TRIGGER_ALARM_OUT_NUM] = {NCFG_ITEM_SENSOR_IN_BUZZ, NCFG_ITEM_SENSOR_IN_TO_ALARM_OUT, NCFG_ITEM_SENSOR_TO_BIG_VIEW, NCFG_ITEM_SENSOR_IN_TO_EMAIL_ENABLE};

					SetCfgItemInfo(DD_TRIGGER_ALARM_OUT_NUM, pBuf, ItemArray);
					memcpy(pCfgParam, pBuf, ItemLength);
					delete []pBuf;
					pBuf =	NULL;
				}
				break;
			}
		case DD_CONFIG_ITEM_SENSOR_TO_RECORD:
			{
				ItemLength = GetCfgItemLength(DD_TRIGGER_RECORD_NUM, cfgParamLen);
				if (ItemLength > 0)
				{
					unsigned char *pBuf = new unsigned char[ItemLength];

					unsigned short ItemArray[DD_TRIGGER_RECORD_NUM] = {NCFG_ITEM_SENSOR_IN_TO_RECORD, NCFG_ITEM_SENSOR_IN_TO_SNAP};

					SetCfgItemInfo(DD_TRIGGER_RECORD_NUM, pBuf, ItemArray);
					memcpy(pCfgParam, pBuf, ItemLength);
					delete []pBuf;
					pBuf =	NULL;
				}
				break;
			}
		case DD_CONFIG_ITEM_SENSOR_TO_PTZ:
			{
				ItemLength = GetCfgItemLength(DD_TRIGGER_PTZ_NUM, cfgParamLen);
				if (ItemLength > 0)
				{
					unsigned char *pBuf = new unsigned char[ItemLength];

					unsigned short ItemArray[DD_TRIGGER_PTZ_NUM] = {NCFG_ITEM_SENSOR_IN_TO_PTZ};

					SetCfgItemInfo(DD_TRIGGER_PTZ_NUM, pBuf, ItemArray);
					memcpy(pCfgParam, pBuf, ItemLength);
					delete []pBuf;
					pBuf =	NULL;
				}
				break;
			}
		case DD_CONFIG_ITEM_MOTION_SETUP:
			{
				ItemLength = GetCfgItemLength(DD_MOTION_CONFIG_NUM, cfgParamLen);
				if (ItemLength > 0)
				{
					unsigned char *pBuf = new unsigned char[ItemLength];

					unsigned short ItemArray[DD_MOTION_CONFIG_NUM] = {NCFG_ITEM_MOTION_ENABLE, NCFG_ITEM_MOTION_AREA_INFO, NCFG_ITEM_MOTION_HOLD_TIME};

					SetCfgItemInfo(DD_MOTION_CONFIG_NUM, pBuf, ItemArray);
					memcpy(pCfgParam, pBuf, ItemLength);
					delete []pBuf;
					pBuf =	NULL;
				}
				break;
			}
		case DD_CONFIG_ITEM_MOTION_SCHEDULE:
			{
				ItemLength = GetCfgItemLength(DD_MOTION_SCHEDULE_NUM, cfgParamLen);
				if (ItemLength > 0)
				{
					unsigned char *pBuf = new unsigned char[ItemLength];

					unsigned short ItemArray[DD_MOTION_SCHEDULE_NUM] = {NCFG_ITEM_MOTION_SCH_WEEK};

					SetCfgItemInfo(DD_MOTION_SCHEDULE_NUM, pBuf, ItemArray);
					memcpy(pCfgParam, pBuf, ItemLength);
					delete []pBuf;
					pBuf =	NULL;
				}
				break;
			}
		case DD_CONFIG_ITEM_MOTION_ALARM_OUT:
			{
				ItemLength = GetCfgItemLength(DD_TRIGGER_ALARM_OUT_NUM, cfgParamLen);
				if (ItemLength > 0)
				{
					unsigned char *pBuf = new unsigned char[ItemLength];

					unsigned short ItemArray[DD_TRIGGER_ALARM_OUT_NUM] = {NCFG_ITEM_MOTION_TO_ALARM_OUT, NCFG_ITEM_MOTION_TO_BIG_VIEW, NCFG_ITEM_MOTION_TO_EMAIL_ENABLE, NCFG_ITEM_MOTION_TO_BUZZ};

					SetCfgItemInfo(DD_TRIGGER_ALARM_OUT_NUM, pBuf, ItemArray);
					memcpy(pCfgParam, pBuf, ItemLength);
					delete []pBuf;
					pBuf =	NULL;
				}
				break;
			}
		case DD_CONFIG_ITEM_MOTION_TO_RECORD:
			{
				ItemLength = GetCfgItemLength(DD_TRIGGER_RECORD_NUM, cfgParamLen);
				if (ItemLength > 0)
				{
					unsigned char *pBuf = new unsigned char[ItemLength];

					unsigned short ItemArray[DD_TRIGGER_RECORD_NUM] = {NCFG_ITEM_MOTION_TO_RECORD, NCFG_ITEM_MOTION_TO_SNAP};

					SetCfgItemInfo(DD_TRIGGER_RECORD_NUM, pBuf, ItemArray);
					memcpy(pCfgParam, pBuf, ItemLength);
					delete []pBuf;
					pBuf =	NULL;
				}
				break;
			}
		case DD_CONFIG_ITEM_SHELTER_SCHEDULE:
			{
				break;
			}
		case DD_CONFIG_ITEM_SHELTER_ALARM_OUT:
			{
				ItemLength = GetCfgItemLength(DD_TRIGGER_ALARM_OUT_NUM, cfgParamLen);
				if (ItemLength > 0)
				{
					unsigned char *pBuf = new unsigned char[ItemLength];

					unsigned short ItemArray[DD_TRIGGER_ALARM_OUT_NUM] = {NCFG_ITEM_SHELTER_TO_ALARM_OUT, NCFG_ITEM_SHELTER_TO_BIG_VIEW, NCFG_ITEM_SHELTER_TO_EMAIL_ADDR, NCFG_ITEM_SHELTER_TO_BUZZ};

					SetCfgItemInfo(DD_TRIGGER_ALARM_OUT_NUM, pBuf, ItemArray);
					memcpy(pCfgParam, pBuf, ItemLength);
					delete []pBuf;
					pBuf =	NULL;
				}
				break;
			}
		case DD_CONFIG_ITEM_SHELTER_TO_RECORD:
			{
				ItemLength = GetCfgItemLength(DD_TRIGGER_RECORD_NUM, cfgParamLen);
				if (ItemLength > 0)
				{
					unsigned char *pBuf = new unsigned char[ItemLength];

					unsigned short ItemArray[DD_TRIGGER_RECORD_NUM] = {NCFG_ITEM_SHELTER_TO_RECORD, NCFG_ITEM_SHELTER_TO_SNAP};

					SetCfgItemInfo(DD_TRIGGER_RECORD_NUM, pBuf, ItemArray);
					memcpy(pCfgParam, pBuf, ItemLength);
					delete []pBuf;
					pBuf =	NULL;
				}
				break;
			}
		case DD_CONFIG_ITEM_VLOSS_SCHEDULE:
			{
				break;
			}
		case DD_CONFIG_ITEM_VLOSS_ALARM_OUT:
			{
				ItemLength = GetCfgItemLength(DD_TRIGGER_ALARM_OUT_NUM, cfgParamLen);
				if (ItemLength > 0)
				{
					unsigned char *pBuf = new unsigned char[ItemLength];

					unsigned short ItemArray[DD_TRIGGER_ALARM_OUT_NUM] = {NCFG_ITEM_VL_TO_ALARM_OUT, NCFG_ITEM_VL_TO_BIG_VIEW, NCFG_ITEM_VL_TO_EMAIL_ENABLE, NCFG_ITEM_VL_TO_BUZZ};

					SetCfgItemInfo(DD_TRIGGER_ALARM_OUT_NUM, pBuf, ItemArray);
					memcpy(pCfgParam, pBuf, ItemLength);
					delete []pBuf;
					pBuf =	NULL;
				}
				break;
			}
		case DD_CONFIG_ITEM_VLOSS_TO_RECORD:
			{
				ItemLength = GetCfgItemLength(DD_TRIGGER_RECORD_NUM, cfgParamLen);
				if (ItemLength > 0)
				{
					unsigned char *pBuf = new unsigned char[ItemLength];

					unsigned short ItemArray[DD_TRIGGER_RECORD_NUM] = {NCFG_ITEM_VL_TO_RECORD, NCFG_ITEM_VL_TO_SNAP};

					SetCfgItemInfo(DD_TRIGGER_RECORD_NUM, pBuf, ItemArray);
					memcpy(pCfgParam, pBuf, ItemLength);
					delete []pBuf;
					pBuf =	NULL;
				}
				break;
			}
		case DD_CONFIG_ITEM_RELAY_SETUP:
			{
				ItemLength = GetCfgItemLength(DD_RELAY_CONFIG_NUM, cfgParamLen);
				if (ItemLength > 0)
				{
					unsigned char *pBuf = new unsigned char[ItemLength];

					unsigned short ItemArray[DD_RELAY_CONFIG_NUM] = {NCFG_ITEM_ALARM_OUT_NAME, NCFG_ITEM_ALARM_OUT_HOLD_TIME};

					SetCfgItemInfo(DD_RELAY_CONFIG_NUM, pBuf, ItemArray);
					memcpy(pCfgParam, pBuf, ItemLength);
					delete []pBuf;
					pBuf =	NULL;
				}
				break;
			}
		case DD_CONFIG_ITEM_RELAY_SCHEDULE:
			{
				break;
			}
		case DD_CONFIG_ITEM_BUZZER_SETUP:
			{
				ItemLength = GetCfgItemLength(DD_BUZZER_CONFIG_NUM, cfgParamLen);
				if (ItemLength > 0)
				{
					unsigned char *pBuf = new unsigned char[ItemLength];

					unsigned short ItemArray[DD_BUZZER_CONFIG_NUM] = {NCFG_ITEM_BUZZER_ENABLE,NCFG_ITEM_BUZZER_HOLD_TIME};

					SetCfgItemInfo(DD_BUZZER_CONFIG_NUM, pBuf, ItemArray);
					memcpy(pCfgParam, pBuf, ItemLength);
					delete []pBuf;
					pBuf =	NULL;
				}
				break;
			}
		case DD_CONFIG_ITEM_BUZZER_SCHEDULE:
			{
				break;
			}
		case DD_CONFIG_ITEM_PTZ_PROTOCOL_INFO:
			{
				ItemLength = GetCfgItemLength(DD_PTZ_PRESET_CONFIG_NUM, cfgParamLen);
				if (ItemLength > 0)
				{
					unsigned char *pBuf = new unsigned char[ItemLength];

					unsigned short ItemArray[DD_PTZ_PRESET_CONFIG_NUM] = {NCFG_ITEM_PTZ_SUPPORT_INFO};

					SetCfgItemInfo(DD_PTZ_PRESET_CONFIG_NUM, pBuf, ItemArray);
					memcpy(pCfgParam, pBuf, ItemLength);
					delete []pBuf;
					pBuf =	NULL;
				}
				break;
			}
		case DD_CONFIG_ITEM_PTZ_SETUP:
			{
				ItemLength = GetCfgItemLength(DD_PTZ_CONFIG_NUM, cfgParamLen);
				if (ItemLength > 0)
				{
					unsigned char *pBuf = new unsigned char[ItemLength];

					unsigned short ItemArray[DD_PTZ_CONFIG_NUM] = {NCFG_ITEM_PTZ_SERIAL};

					SetCfgItemInfo(DD_PTZ_CONFIG_NUM, pBuf, ItemArray);
					memcpy(pCfgParam, pBuf, ItemLength);
					delete []pBuf;
					pBuf =	NULL;
				}
				break;
			}
		case DD_CONFIG_ITEM_PTZ_PRESET:
			{
				ItemLength = GetCfgItemLength(m_deiviceInfo.videoInputNum, cfgParamLen);
				if (ItemLength > 0)
				{
					unsigned char *pBuf = new unsigned char[ItemLength];

					unsigned short *pItemArray = new unsigned short[m_deiviceInfo.videoInputNum];
					for(long i = 0; i < m_deiviceInfo.videoInputNum; i++)
					{
						pItemArray[i] = NCFG_ITEM_PTZ_INFO_PRESET_INFO_CHBASE + i;
					}

					SetCfgItemInfo(m_deiviceInfo.videoInputNum, pBuf, pItemArray);
					memcpy(pCfgParam, pBuf, ItemLength);
					delete [] pBuf;
					delete [] pItemArray;
					pItemArray = NULL;
					pBuf =	NULL;
				}
				break;
			}
	}

	if ((DD_CONFIG_ITEM_PTZ_CRUISE <= configItem)&&(DD_CONFIG_ITEM_PTZ_CRUISE_END > configItem))
	{
		ItemLength = GetCfgItemLength(DD_PTZ_CURISE_CONFIG_NUM, cfgParamLen);
		if (ItemLength > 0)
		{
			unsigned char *pBuf = new unsigned char[ItemLength];
			unsigned int channel = configItem - DD_CONFIG_ITEM_PTZ_CRUISE;
			unsigned short *pItemArray = new unsigned short[DD_PTZ_CURISE_CONFIG_NUM];

			/*for(long i = 0; i < DD_PTZ_CURISE_CONFIG_NUM; i++)
			{*/
			pItemArray[0] = NCFG_ITEM_PTZ_INFO_CRUISE_INFO_CHBASE + channel;
			/*}*/

			SetCfgItemInfo(DD_PTZ_CURISE_CONFIG_NUM, pBuf, pItemArray);
			memcpy(pCfgParam, pBuf, ItemLength);
			delete [] pBuf;
			delete [] pItemArray;
			pItemArray = NULL;
			pBuf =	NULL;
		}
	}
	return ItemLength;
}

long CConfigParser_V3::CfgDevice2CfgSDK(void *pDeviceCfg, long deviceCfgLen, unsigned char*& pSDKCfg, long& sdkCfgLen, DD_CONFIG_ITEM_HEAD* pDDcfgHead)
{
	/*
		pDDcfgHead 为空时, 只读取头
		pDDcfgHead 非空时，才读取整个数据
	*/

	CConfigBlock_v3 pConfigBlock;
	if (deviceCfgLen == 0)
	{
		return 0;
	}
	pConfigBlock.CreateBlock((unsigned char *)pDeviceCfg, deviceCfgLen);

	std::list<NCFG_ITEM_HEAD_EX> &configList = pConfigBlock.GetConfigItemList();
	unsigned char *pItemData = NULL;
	unsigned long itemDataLen;
	NCFG_ITEM_HEAD itemHead;
	std::list<ITEM_INFO> iteminfolist;
	std::list<ITEM_INFO>::iterator its;

	for(std::list<NCFG_ITEM_HEAD_EX>::iterator it = configList.begin(); it != configList.end(); it++)
	{
		itemHead = (*it).item_head;
		if(!pConfigBlock.GetConfig((NCFG_ITEM_ID)itemHead.itemID, &pItemData, itemDataLen))
		{
			assert(false);
			continue;
		}
		switch (itemHead.itemID)
		{
		case NCFG_ITEM_DEVICE_ID:
			{
				if (iteminfolist.empty())
				{
					ITEM_INFO item_info;
					item_info.itemID = DD_CONFIG_ITEM_DEVICE_INFO;
					item_info.parserLen = sizeof(DD_DEVICE_INFO);
					item_info.pConfigItem = (unsigned char *)new DD_DEVICE_INFO;
					memset(item_info.pConfigItem, 0, sizeof(DD_DEVICE_INFO));
					((DD_DEVICE_INFO*)(item_info.pConfigItem))->deviceID = *((unsigned long*)pItemData);

					iteminfolist.push_back(item_info);
				}
				else
				{
					for (its = iteminfolist.begin(); its != iteminfolist.end(); its++)
					{
						if ((*its).itemID == DD_CONFIG_ITEM_DEVICE_INFO)
						{
							((DD_DEVICE_INFO*)(*its).pConfigItem)->deviceID = *((unsigned long*)pItemData);
							break;
						}
					}
				}
				break;
			}
		case NCFG_ITEM_DEVICE_NAME:
			{
				if (iteminfolist.empty())
				{
					ITEM_INFO item_info;
					item_info.itemID = DD_CONFIG_ITEM_DEVICE_INFO;
					item_info.parserLen = sizeof(DD_DEVICE_INFO);
					item_info.pConfigItem = (unsigned char *)new DD_DEVICE_INFO;
					memset(item_info.pConfigItem, 0, sizeof(DD_DEVICE_INFO));
					NCFG_INFO_SHORT_NAME *deviceName = (NCFG_INFO_SHORT_NAME*)pItemData;
					strcpy(((DD_DEVICE_INFO*)item_info.pConfigItem)->deviceName, deviceName->name);

					iteminfolist.push_back(item_info);
				}
				else
				{
					for (its = iteminfolist.begin(); its != iteminfolist.end(); its++)
					{
						if ((*its).itemID == DD_CONFIG_ITEM_DEVICE_INFO)
						{
							NCFG_INFO_SHORT_NAME *deviceName = (NCFG_INFO_SHORT_NAME*)pItemData;
							strcpy(((DD_DEVICE_INFO*)(*its).pConfigItem)->deviceName, deviceName->name);
							break;
						}
					}
				}
				break;
			}
		case NCFG_ITEM_LIVE_AUDIO_ENABLE:
			{
				if (iteminfolist.empty())
				{
					ITEM_INFO item_info;
					item_info.itemID = DD_CONFIG_ITEM_DEVICE_INFO;
					item_info.parserLen = sizeof(DD_DEVICE_INFO);
					item_info.pConfigItem = (unsigned char *)new DD_DEVICE_INFO;
					memset(item_info.pConfigItem, 0, sizeof(DD_DEVICE_INFO));
					((DD_DEVICE_INFO*)item_info.pConfigItem)->audioNum = *((unsigned long*)pItemData);
					
					iteminfolist.push_back(item_info);
				}
				else
				{
					for (its = iteminfolist.begin(); its != iteminfolist.end(); its++)
					{
						if ((*its).itemID == DD_CONFIG_ITEM_DEVICE_INFO)
						{
							((DD_DEVICE_INFO*)(*its).pConfigItem)->audioNum = *((unsigned long*)pItemData);
							break;
						}
					}
				}
				break;
			}
		case NCFG_ITEM_VIDEO_FORMAT:
			{
				if (iteminfolist.empty())
				{
					ITEM_INFO item_info;
					item_info.itemID = DD_CONFIG_ITEM_SYSTEM_BASIC;
					item_info.parserLen = sizeof(DD_BASIC_CONFIG);
					item_info.pConfigItem = (unsigned char *)new DD_BASIC_CONFIG;
					memset(item_info.pConfigItem, 0, sizeof(DD_BASIC_CONFIG));
					((DD_BASIC_CONFIG*)item_info.pConfigItem)->videoFormat = *((unsigned long*)pItemData);
	
					iteminfolist.push_back(item_info);
				}
				else
				{
					for (its = iteminfolist.begin(); its != iteminfolist.end(); its++)
					{
						if ((*its).itemID == DD_CONFIG_ITEM_SYSTEM_BASIC)
						{
							((DD_BASIC_CONFIG*)(*its).pConfigItem)->videoFormat = *((unsigned long*)pItemData);
							break;
						}
					}
				}
				break;
			}
		case NCFG_ITEM_SCRENN_SAVE_TIME:
			{
				if (iteminfolist.empty())
				{
					ITEM_INFO item_info;
					item_info.itemID = DD_CONFIG_ITEM_SYSTEM_BASIC;
					item_info.parserLen = sizeof(DD_BASIC_CONFIG);
					item_info.pConfigItem = (unsigned char *)new DD_BASIC_CONFIG;
					memset(item_info.pConfigItem, 0, sizeof(DD_BASIC_CONFIG));
					((DD_BASIC_CONFIG*)item_info.pConfigItem)->screensaver = *((unsigned long*)pItemData);
					
					iteminfolist.push_back(item_info);
				}
				else
				{
					for (its = iteminfolist.begin(); its != iteminfolist.end(); its++)
					{
						if ((*its).itemID == DD_CONFIG_ITEM_SYSTEM_BASIC)
						{
							((DD_BASIC_CONFIG*)(*its).pConfigItem)->screensaver = *((unsigned long*)pItemData);
							break;
						}
					}
				}
				break;
			}
		case NCFG_ITEM_VGA_RESOLUTION:
			{
				if (iteminfolist.empty())
				{
					ITEM_INFO item_info;
					item_info.itemID = DD_CONFIG_ITEM_SYSTEM_BASIC;
					item_info.parserLen = sizeof(DD_BASIC_CONFIG);
					item_info.pConfigItem = (unsigned char *)new DD_BASIC_CONFIG;
					memset(item_info.pConfigItem, 0, sizeof(DD_BASIC_CONFIG));
					((DD_BASIC_CONFIG*)item_info.pConfigItem )->videoOutResolution = *((unsigned long*)pItemData);
					
					iteminfolist.push_back(item_info);
				}
				else
				{
					for (its = iteminfolist.begin(); its != iteminfolist.end(); its++)
					{
						if ((*its).itemID == DD_CONFIG_ITEM_SYSTEM_BASIC)
						{
							((DD_BASIC_CONFIG*)(*its).pConfigItem)->videoOutResolution = *((unsigned long*)pItemData);
							break;
						}
					}
				}
				break;
			}
		case NCFG_ITEM_VGA_REFRESH:
			{
				if (iteminfolist.empty())
				{
					ITEM_INFO item_info;
					item_info.itemID = DD_CONFIG_ITEM_SYSTEM_BASIC;
					item_info.parserLen = sizeof(DD_BASIC_CONFIG);
					item_info.pConfigItem = (unsigned char *)new DD_BASIC_CONFIG;
					memset(item_info.pConfigItem, 0, sizeof(DD_BASIC_CONFIG));
					((DD_BASIC_CONFIG*)item_info.pConfigItem )->VGARefresh = *((unsigned long*)pItemData);
					
					iteminfolist.push_back(item_info);
				}
				else
				{
					for (its = iteminfolist.begin(); its != iteminfolist.end(); its++)
					{
						if ((*its).itemID == DD_CONFIG_ITEM_SYSTEM_BASIC)
						{
							((DD_BASIC_CONFIG*)(*its).pConfigItem)->VGARefresh = *((unsigned long*)pItemData);
							break;
						}
					}
				}
				break;
			}
		case NCFG_ITEM_LANGUAGE:
			{
				if (iteminfolist.empty())
				{
					ITEM_INFO item_info;
					item_info.itemID = DD_CONFIG_ITEM_SYSTEM_BASIC;
					item_info.parserLen = sizeof(DD_BASIC_CONFIG);
					item_info.pConfigItem = (unsigned char *)new DD_BASIC_CONFIG;
					memset(item_info.pConfigItem, 0, sizeof(DD_BASIC_CONFIG));
					((DD_BASIC_CONFIG*)item_info.pConfigItem)->deviceLanguage = *((unsigned long*)pItemData);
					
					iteminfolist.push_back(item_info);
				}
				else
				{
					for (its = iteminfolist.begin(); its != iteminfolist.end(); its++)
					{
						if ((*its).itemID == DD_CONFIG_ITEM_SYSTEM_BASIC)
						{
							((DD_BASIC_CONFIG*)(*its).pConfigItem)->deviceLanguage = *((unsigned long*)pItemData);
							break;
						}
					}
				}
				break;
			}
		case NCFG_ITEM_REC_RECYCLE:
			{
				if (iteminfolist.empty())
				{
					ITEM_INFO item_info;
					item_info.itemID = DD_CONFIG_ITEM_SYSTEM_BASIC;
					item_info.parserLen = sizeof(DD_BASIC_CONFIG);
					item_info.pConfigItem = (unsigned char *)new DD_BASIC_CONFIG;
					memset(item_info.pConfigItem, 0, sizeof(DD_BASIC_CONFIG));
					((DD_BASIC_CONFIG*)item_info.pConfigItem )->RecycleRecord = *((unsigned long*)pItemData);
					
					iteminfolist.push_back(item_info);
				}
				else
				{
					for (its = iteminfolist.begin(); its != iteminfolist.end(); its++)
					{
						if ((*its).itemID == DD_CONFIG_ITEM_SYSTEM_BASIC)
						{
							((DD_BASIC_CONFIG*)(*its).pConfigItem)->RecycleRecord = *((unsigned long*)pItemData);
							break;
						}
					}
				}
				break;
			}
		case NCFG_ITEM_AUTHORIZATION:
			{
				if (iteminfolist.empty())
				{
					ITEM_INFO item_info;
					item_info.itemID = DD_CONFIG_ITEM_SYSTEM_BASIC;
					item_info.parserLen = sizeof(DD_BASIC_CONFIG);
					item_info.pConfigItem = (unsigned char *)new DD_BASIC_CONFIG;
					memset(item_info.pConfigItem, 0, sizeof(DD_BASIC_CONFIG));
					((DD_BASIC_CONFIG*)item_info.pConfigItem)->passwordCheck = *((unsigned long*)pItemData);

					iteminfolist.push_back(item_info);
				}
				else
				{
					for (its = iteminfolist.begin(); its != iteminfolist.end(); its++)
					{
						if ((*its).itemID == DD_CONFIG_ITEM_SYSTEM_BASIC)
						{
							((DD_BASIC_CONFIG*)(*its).pConfigItem)->passwordCheck = *((unsigned long*)pItemData);
							break;
						}
					}
				}
				break;
			}
		case NCFG_ITEM_SUPPORT_VIDEOFORMAT:
			{
				if (iteminfolist.empty())
				{
					ITEM_INFO item_info;
					item_info.itemID = DD_CONFIG_ITEM_SYSTEM_BASIC;
					item_info.parserLen = sizeof(DD_BASIC_CONFIG);
					item_info.pConfigItem = (unsigned char *)new DD_BASIC_CONFIG;
					memset(item_info.pConfigItem, 0, sizeof(DD_BASIC_CONFIG));
					((DD_BASIC_CONFIG*)item_info.pConfigItem )->videoFormatMask = *((unsigned long*)pItemData);
					
					iteminfolist.push_back(item_info);
				}
				else
				{
					for (its = iteminfolist.begin(); its != iteminfolist.end(); its++)
					{
						if ((*its).itemID == DD_CONFIG_ITEM_SYSTEM_BASIC)
						{
							((DD_BASIC_CONFIG*)(*its).pConfigItem)->videoFormatMask = *((unsigned long*)pItemData);
							break;
						}
					}
				}
				break;
			}
		case NCFG_ITEM_SUPPORT_LANGUAGE:
			{
				if (iteminfolist.empty())
				{
					ITEM_INFO item_info;
					item_info.itemID = DD_CONFIG_ITEM_SYSTEM_BASIC;
					item_info.parserLen = sizeof(DD_BASIC_CONFIG);
					item_info.pConfigItem = (unsigned char *)new DD_BASIC_CONFIG;
					memset(item_info.pConfigItem, 0, sizeof(DD_BASIC_CONFIG));
					NCFG_INFO_LANGUAGE *language = (NCFG_INFO_LANGUAGE*)pItemData;
					((DD_BASIC_CONFIG*)item_info.pConfigItem)->languageMask = language->languageId;
					
					iteminfolist.push_back(item_info);
				}
				else
				{
					for (its = iteminfolist.begin(); its != iteminfolist.end(); its++)
					{
						if ((*its).itemID == DD_CONFIG_ITEM_SYSTEM_BASIC)
						{
							NCFG_INFO_LANGUAGE *language = (NCFG_INFO_LANGUAGE*)pItemData;
							((DD_BASIC_CONFIG*)(*its).pConfigItem)->languageMask = language->languageId;
							break;
						}
					}
				}
				break;
			}
		case NCFG_ITEM_VGA_SUPPORT_RESOLUTION:
			{
				if (iteminfolist.empty())
				{
					ITEM_INFO item_info;
					item_info.itemID = DD_CONFIG_ITEM_SYSTEM_BASIC;
					item_info.parserLen = sizeof(DD_BASIC_CONFIG);
					item_info.pConfigItem = (unsigned char *)new DD_BASIC_CONFIG;
					memset(item_info.pConfigItem, 0, sizeof(DD_BASIC_CONFIG));
					((DD_BASIC_CONFIG*)item_info.pConfigItem)->videoOutResolutionMask = *((unsigned long*)pItemData);
					iteminfolist.push_back(item_info);
				}
				else
				{
					for (its = iteminfolist.begin(); its != iteminfolist.end(); its++)
					{
						if ((*its).itemID == DD_CONFIG_ITEM_SYSTEM_BASIC)
						{
							((DD_BASIC_CONFIG*)(*its).pConfigItem)->videoOutResolutionMask = *((unsigned long*)pItemData);
							break;
						}
					}
				}
				break;
			}
		case NCFG_ITEM_HOUR_FORMAT:
			{
				if (iteminfolist.empty())
				{
					ITEM_INFO item_info;
					item_info.itemID = DD_CONFIG_ITEM_DATE_TIME;
					item_info.parserLen = sizeof(DD_DATE_TIME_CONFIG);
					item_info.pConfigItem = (unsigned char *)new DD_DATE_TIME_CONFIG;
					memset(item_info.pConfigItem, 0, sizeof(DD_DATE_TIME_CONFIG));
					((DD_DATE_TIME_CONFIG*)item_info.pConfigItem)->timeFormat = *pItemData;
					
					iteminfolist.push_back(item_info);
				}
				else
				{
					for (its = iteminfolist.begin(); its != iteminfolist.end(); its++)
					{
						if ((*its).itemID == DD_CONFIG_ITEM_DATE_TIME)
						{
							((DD_DATE_TIME_CONFIG*)(*its).pConfigItem)->timeFormat = *pItemData;
							break;
						}
					}
				}
				break;
			}
		case NCFG_ITEM_TIME_ZONE: 
			{
				if (iteminfolist.empty())
				{
					ITEM_INFO item_info;
					item_info.itemID = DD_CONFIG_ITEM_DATE_TIME;
					item_info.parserLen = sizeof(DD_DATE_TIME_CONFIG);
					item_info.pConfigItem = (unsigned char *)new DD_DATE_TIME_CONFIG;
					memset(item_info.pConfigItem, 0, sizeof(DD_DATE_TIME_CONFIG));
					((DD_DATE_TIME_CONFIG*)item_info.pConfigItem)->timeZone = *pItemData;
					
					iteminfolist.push_back(item_info);
				}
				else
				{
					for (its = iteminfolist.begin(); its != iteminfolist.end(); its++)
					{
						if ((*its).itemID == DD_CONFIG_ITEM_DATE_TIME)
						{
							((DD_DATE_TIME_CONFIG*)(*its).pConfigItem)->timeZone = *pItemData;
							break;
						}
					}
				}
				break;
			}
		case NCFG_ITEM_TIME_SYNC:
			{
				if (iteminfolist.empty())
				{
					ITEM_INFO item_info;
					item_info.itemID = DD_CONFIG_ITEM_DATE_TIME;
					item_info.parserLen = sizeof(DD_DATE_TIME_CONFIG);
					item_info.pConfigItem = (unsigned char *)new DD_DATE_TIME_CONFIG;
					memset(item_info.pConfigItem, 0, sizeof(DD_DATE_TIME_CONFIG));
					((DD_DATE_TIME_CONFIG*)item_info.pConfigItem)->enableNTP = *pItemData;
					
					iteminfolist.push_back(item_info);
				}
				else
				{
					for (its = iteminfolist.begin(); its != iteminfolist.end(); its++)
					{
						if ((*its).itemID == DD_CONFIG_ITEM_DATE_TIME)
						{
							((DD_DATE_TIME_CONFIG*)(*its).pConfigItem)->enableNTP = *pItemData;
							break;
						}
					}
				}
				break;
			}
		case NCFG_ITEM_NTP_PORT:
			{
				if (iteminfolist.empty())
				{
					ITEM_INFO item_info;
					item_info.itemID = DD_CONFIG_ITEM_DATE_TIME;
					item_info.parserLen = sizeof(DD_DATE_TIME_CONFIG);
					item_info.pConfigItem = (unsigned char *)new DD_DATE_TIME_CONFIG;
					memset(item_info.pConfigItem, 0, sizeof(DD_DATE_TIME_CONFIG));
					((DD_DATE_TIME_CONFIG*)item_info.pConfigItem)->ntpPort = *((unsigned short*)pItemData);

					iteminfolist.push_back(item_info);
				}
				else
				{
					for (its = iteminfolist.begin(); its != iteminfolist.end(); its++)
					{
						if ((*its).itemID == DD_CONFIG_ITEM_DATE_TIME)
						{
							((DD_DATE_TIME_CONFIG*)(*its).pConfigItem)->ntpPort = *((unsigned short*)pItemData);
							break;
						}
					}
				}
				break;
			}
		case NCFG_ITEM_TIME_SERVER:
			{
				if (iteminfolist.empty())
				{
					ITEM_INFO item_info;
					item_info.itemID = DD_CONFIG_ITEM_DATE_TIME;
					item_info.parserLen = sizeof(DD_DATE_TIME_CONFIG);
					item_info.pConfigItem = (unsigned char *)new DD_DATE_TIME_CONFIG;
					memset(item_info.pConfigItem, 0, sizeof(DD_DATE_TIME_CONFIG));
					strcpy(((DD_DATE_TIME_CONFIG*)item_info.pConfigItem)->ntpServerAddr, (char*)pItemData);
					
					iteminfolist.push_back(item_info);
				}
				else
				{
					for (its = iteminfolist.begin(); its != iteminfolist.end(); its++)
					{
						if ((*its).itemID == DD_CONFIG_ITEM_DATE_TIME)
						{
							strcpy(((DD_DATE_TIME_CONFIG*)(*its).pConfigItem)->ntpServerAddr, (char*)pItemData);
							break;
						}
					}
				}
				break;
			}
		case NCFG_ITEM_DATE_FORMAT:
			{
				if (iteminfolist.empty())
				{
					ITEM_INFO item_info;
					item_info.itemID = DD_CONFIG_ITEM_DATE_TIME;
					item_info.parserLen = sizeof(DD_DATE_TIME_CONFIG);
					item_info.pConfigItem = (unsigned char *)new DD_DATE_TIME_CONFIG;
					memset(item_info.pConfigItem, 0, sizeof(DD_DATE_TIME_CONFIG));
					((DD_DATE_TIME_CONFIG*)item_info.pConfigItem)->dateFormat = *pItemData;

					iteminfolist.push_back(item_info);
				}
				else
				{
					for (its = iteminfolist.begin(); its != iteminfolist.end(); its++)
					{
						if ((*its).itemID == DD_CONFIG_ITEM_DATE_TIME)
						{
							((DD_DATE_TIME_CONFIG*)(*its).pConfigItem)->dateFormat = *pItemData;
							break;
						}
					}
				}
				break;
			}
		case NCFG_ITEM_DAYLIGHT_INFO:
			{
				/*ITEM_INFO item_info;
				item_info.itemID = DD_CONFIG_ITEM_DAYLIGHT_INFO;
				item_info.parserLen = sizeof(DD_DAYLIGHT_INFO);
				item_info.pConfigItem = (unsigned char *)new DD_DAYLIGHT_INFO;
				memset(item_info.pConfigItem, 0, sizeof(DD_DAYLIGHT_INFO));
				((DD_DAYLIGHT_INFO*)item_info.pConfigItem)->enable = ((DAYLIGHT_INFO*)pItemData)->enable;
				((DD_DAYLIGHT_INFO*)item_info.pConfigItem)->InMday = ((DAYLIGHT_INFO*)pItemData)->InMday;
				((DD_DAYLIGHT_INFO*)item_info.pConfigItem)->InSecond = ((DAYLIGHT_INFO*)pItemData)->InSecond;
				((DD_DAYLIGHT_INFO*)item_info.pConfigItem)->InMonth = ((DAYLIGHT_INFO*)pItemData)->InMonth;
				((DD_DAYLIGHT_INFO*)item_info.pConfigItem)->InWday = ((DAYLIGHT_INFO*)pItemData)->InWday;
				((DD_DAYLIGHT_INFO*)item_info.pConfigItem)->InYear = ((DAYLIGHT_INFO*)pItemData)->InYear;
				((DD_DAYLIGHT_INFO*)item_info.pConfigItem)->InWeekIndex = ((DAYLIGHT_INFO*)pItemData)->InWeekIndex;
				((DD_DAYLIGHT_INFO*)item_info.pConfigItem)->offSet = ((DAYLIGHT_INFO*)pItemData)->offSet;
				((DD_DAYLIGHT_INFO*)item_info.pConfigItem)->OutMday = ((DAYLIGHT_INFO*)pItemData)->OutMday;
				((DD_DAYLIGHT_INFO*)item_info.pConfigItem)->OutMonth = ((DAYLIGHT_INFO*)pItemData)->OutMonth;
				((DD_DAYLIGHT_INFO*)item_info.pConfigItem)->OutSecond = ((DAYLIGHT_INFO*)pItemData)->OutSecond;
				((DD_DAYLIGHT_INFO*)item_info.pConfigItem)->OutWday = ((DAYLIGHT_INFO*)pItemData)->OutWday;
				((DD_DAYLIGHT_INFO*)item_info.pConfigItem)->OutWeekIndex = ((DAYLIGHT_INFO*)pItemData)->OutWeekIndex;
				((DD_DAYLIGHT_INFO*)item_info.pConfigItem)->OutYear = ((DAYLIGHT_INFO*)pItemData)->OutYear;
				((DD_DAYLIGHT_INFO*)item_info.pConfigItem)->type = ((DAYLIGHT_INFO*)pItemData)->type;
				
				iteminfolist.push_back(item_info);*/
				if (iteminfolist.empty())
				{
					ITEM_INFO item_info;
					item_info.itemID = DD_CONFIG_ITEM_DAYLIGHT_INFO;
					item_info.parserLen = sizeof(DD_DAYLIGHT_INFO);
					item_info.pConfigItem = (unsigned char *)new DD_DAYLIGHT_INFO;
					memset(item_info.pConfigItem, 0, sizeof(DD_DAYLIGHT_INFO));
					((DD_DAYLIGHT_INFO*)item_info.pConfigItem)->enable = ((DAYLIGHT_INFO*)pItemData)->enable;
					((DD_DAYLIGHT_INFO*)item_info.pConfigItem)->InMday = ((DAYLIGHT_INFO*)pItemData)->InMday;
					((DD_DAYLIGHT_INFO*)item_info.pConfigItem)->InSecond = ((DAYLIGHT_INFO*)pItemData)->InSecond;
					((DD_DAYLIGHT_INFO*)item_info.pConfigItem)->InMonth = ((DAYLIGHT_INFO*)pItemData)->InMonth;
					((DD_DAYLIGHT_INFO*)item_info.pConfigItem)->InWday = ((DAYLIGHT_INFO*)pItemData)->InWday;
					((DD_DAYLIGHT_INFO*)item_info.pConfigItem)->InYear = ((DAYLIGHT_INFO*)pItemData)->InYear;
					((DD_DAYLIGHT_INFO*)item_info.pConfigItem)->InWeekIndex = ((DAYLIGHT_INFO*)pItemData)->InWeekIndex;
					((DD_DAYLIGHT_INFO*)item_info.pConfigItem)->offSet = ((DAYLIGHT_INFO*)pItemData)->offSet;
					((DD_DAYLIGHT_INFO*)item_info.pConfigItem)->OutMday = ((DAYLIGHT_INFO*)pItemData)->OutMday;
					((DD_DAYLIGHT_INFO*)item_info.pConfigItem)->OutMonth = ((DAYLIGHT_INFO*)pItemData)->OutMonth;
					((DD_DAYLIGHT_INFO*)item_info.pConfigItem)->OutSecond = ((DAYLIGHT_INFO*)pItemData)->OutSecond;
					((DD_DAYLIGHT_INFO*)item_info.pConfigItem)->OutWday = ((DAYLIGHT_INFO*)pItemData)->OutWday;
					((DD_DAYLIGHT_INFO*)item_info.pConfigItem)->OutWeekIndex = ((DAYLIGHT_INFO*)pItemData)->OutWeekIndex;
					((DD_DAYLIGHT_INFO*)item_info.pConfigItem)->OutYear = ((DAYLIGHT_INFO*)pItemData)->OutYear;
					((DD_DAYLIGHT_INFO*)item_info.pConfigItem)->type = ((DAYLIGHT_INFO*)pItemData)->type;

					iteminfolist.push_back(item_info);
				}
				else
				{
					for (its = iteminfolist.begin(); its != iteminfolist.end(); its++)
					{
						if ((*its).itemID == DD_CONFIG_ITEM_DAYLIGHT_INFO)
						{
							((DD_DAYLIGHT_INFO*)(*its).pConfigItem)->enable = ((DAYLIGHT_INFO*)pItemData)->enable;
							((DD_DAYLIGHT_INFO*)(*its).pConfigItem)->InMday = ((DAYLIGHT_INFO*)pItemData)->InMday;
							((DD_DAYLIGHT_INFO*)(*its).pConfigItem)->InSecond = ((DAYLIGHT_INFO*)pItemData)->InSecond;
							((DD_DAYLIGHT_INFO*)(*its).pConfigItem)->InMonth = ((DAYLIGHT_INFO*)pItemData)->InMonth;
							((DD_DAYLIGHT_INFO*)(*its).pConfigItem)->InWday = ((DAYLIGHT_INFO*)pItemData)->InWday;
							((DD_DAYLIGHT_INFO*)(*its).pConfigItem)->InYear = ((DAYLIGHT_INFO*)pItemData)->InYear;
							((DD_DAYLIGHT_INFO*)(*its).pConfigItem)->InWeekIndex = ((DAYLIGHT_INFO*)pItemData)->InWeekIndex;
							((DD_DAYLIGHT_INFO*)(*its).pConfigItem)->offSet = ((DAYLIGHT_INFO*)pItemData)->offSet;
							((DD_DAYLIGHT_INFO*)(*its).pConfigItem)->OutMday = ((DAYLIGHT_INFO*)pItemData)->OutMday;
							((DD_DAYLIGHT_INFO*)(*its).pConfigItem)->OutMonth = ((DAYLIGHT_INFO*)pItemData)->OutMonth;
							((DD_DAYLIGHT_INFO*)(*its).pConfigItem)->OutSecond = ((DAYLIGHT_INFO*)pItemData)->OutSecond;
							((DD_DAYLIGHT_INFO*)(*its).pConfigItem)->OutWday = ((DAYLIGHT_INFO*)pItemData)->OutWday;
							((DD_DAYLIGHT_INFO*)(*its).pConfigItem)->OutWeekIndex = ((DAYLIGHT_INFO*)pItemData)->OutWeekIndex;
							((DD_DAYLIGHT_INFO*)(*its).pConfigItem)->OutYear = ((DAYLIGHT_INFO*)pItemData)->OutYear;
							((DD_DAYLIGHT_INFO*)(*its).pConfigItem)->type = ((DAYLIGHT_INFO*)pItemData)->type;
						}
					}
				}
				break;
			}
		case NCFG_ITEM_VIEW_DWELL_TIME:
			{
				if (iteminfolist.empty())
				{
					ITEM_INFO item_info;
					item_info.itemID = DD_CONFIG_ITEM_LIVE_MAIN_CAMERA;
					item_info.parserLen = sizeof(DD_LIVE_VIDEO_GROUP);
					item_info.pConfigItem = (unsigned char *)new DD_LIVE_VIDEO_GROUP;
					memset(item_info.pConfigItem, 0, sizeof(DD_LIVE_VIDEO_GROUP));
					((DD_LIVE_VIDEO_GROUP*)item_info.pConfigItem)->holdTime = *((unsigned short*)pItemData);

					iteminfolist.push_back(item_info);
				}
				else
				{
					for (its = iteminfolist.begin(); its != iteminfolist.end(); its++)
					{
						if ((*its).itemID == DD_CONFIG_ITEM_LIVE_MAIN_CAMERA)
						{
							((DD_LIVE_VIDEO_GROUP*)(*its).pConfigItem)->holdTime = *((unsigned short*)pItemData);
						}
					}
				}
				
				break;
			}
		case NCFG_ITEM_VIEW_SPLIT_MODE:
			{
				
				VIEW_SPLIT_MODE splitMode = *((VIEW_SPLIT_MODE *)pItemData);
				long cameraNum = m_deiviceInfo.videoInputNum;
				DD_VIEW_SPLIT_MODE dd_splitMode = DD_VIEW_SPLIT_1X1;
				
				NCFG_ITEM_ID itemid = NCFG_ITEM_VIEW_SPLIT_1X1;
				if (VIEW_SPLIT_1X1 == splitMode)
				{
					itemid = NCFG_ITEM_VIEW_SPLIT_1X1;
					dd_splitMode = DD_VIEW_SPLIT_1X1;
				}
				else if (VIEW_SPLIT_2X2 == splitMode)
				{
					itemid = NCFG_ITEM_VIEW_SPLIT_2X2;
					dd_splitMode = DD_VIEW_SPLIT_2X2;
				}
				else if (VIEW_SPLIT_2X3 == splitMode)
				{
					itemid = NCFG_ITEM_VIEW_SPLIT_2X3;
					dd_splitMode = DD_VIEW_SPLIT_2X3;
				}
				else if (VIEW_SPLIT_3X3 == splitMode)
				{
					itemid = NCFG_ITEM_VIEW_SPLIT_3X3;
					dd_splitMode = DD_VIEW_SPLIT_3X3;
				}
				else if (VIEW_SPLIT_4X4 == splitMode)
				{
					itemid = NCFG_ITEM_VIEW_SPLIT_4X4;
					dd_splitMode = DD_VIEW_SPLIT_4X4;
				}
				else if (VIEW_SPLIT_5X5 == splitMode)
				{
					itemid = NCFG_ITEM_VIEW_SPLIT_5X5;
					dd_splitMode = DD_VIEW_SPLIT_5X5;
				}
				else if (VIEW_SPLIT_6X6 == splitMode)
				{
					itemid = NCFG_ITEM_VIEW_SPLIT_6X6;
					dd_splitMode = DD_VIEW_SPLIT_6X6;
				}
				else
				{
					itemid = NCFG_ITEM_VIEW_SPLIT_1X1;
					dd_splitMode = DD_VIEW_SPLIT_1X1;
				}

				if(!pConfigBlock.GetConfig(itemid, &pItemData, itemDataLen))
				{
					ITEM_INFO item_info;
					item_info.itemID = DD_CONFIG_ITEM_LIVE_MAIN_CAMERA;
					item_info.parserLen = sizeof(DD_LIVE_VIDEO_GROUP);
					item_info.pConfigItem = (unsigned char *)new DD_LIVE_VIDEO_GROUP;
					iteminfolist.push_back(item_info);

					break;
				}

				unsigned long *data = (unsigned long *)pItemData;
				int groupNum = data[1];
				data = data + 2;
				int group = 0;
				for (; group < groupNum; group++)
				{
					ITEM_INFO item_info;
					item_info.itemID = DD_CONFIG_ITEM_LIVE_MAIN_CAMERA;
					item_info.parserLen = sizeof(DD_LIVE_VIDEO_GROUP);
					item_info.pConfigItem = (unsigned char *)new DD_LIVE_VIDEO_GROUP;
					memset(item_info.pConfigItem, 0, sizeof(DD_LIVE_VIDEO_GROUP));
					DD_LIVE_VIDEO_GROUP *lvg = reinterpret_cast<DD_LIVE_VIDEO_GROUP *>(item_info.pConfigItem);
					lvg->channelNum = cameraNum;
					lvg->splitMode = (unsigned long)dd_splitMode;
					memset(lvg->channel, 0xff, DD_MAX_CAMERA_NUM * sizeof(unsigned char));
					for (int i = 0; i < DDSplitModeToNum(dd_splitMode); i++)
					{
						if (data[i] != 0xffffffff)
						{
							lvg->channel[data[i]] = i;
						}
					}

					data = data + DDSplitModeToNum(dd_splitMode);
					iteminfolist.push_back(item_info);
				}
				for (; group < m_deiviceInfo.videoInputNum; group++)
				{
					ITEM_INFO item_info;
					item_info.itemID = DD_CONFIG_ITEM_LIVE_MAIN_CAMERA;
					item_info.parserLen = sizeof(DD_LIVE_VIDEO_GROUP);
					item_info.pConfigItem = (unsigned char *)new DD_LIVE_VIDEO_GROUP;
					memset(item_info.pConfigItem, 0, sizeof(DD_LIVE_VIDEO_GROUP));
					DD_LIVE_VIDEO_GROUP *lvg = reinterpret_cast<DD_LIVE_VIDEO_GROUP *>(item_info.pConfigItem);
					lvg->channelNum = cameraNum;
					lvg->splitMode = (unsigned long)splitMode;
					memset(lvg->channel, 0xff, DD_MAX_CAMERA_NUM * sizeof(unsigned char));
					iteminfolist.push_back(item_info);
				}

				break;
			}
		case NCFG_ITEM_SPOT_SPLIT_MODE:
			{
				DD_VIEW_SPLIT_MODE splitMode = *((DD_VIEW_SPLIT_MODE *)pItemData);
				long cameraNum = m_deiviceInfo.videoInputNum;

				NCFG_ITEM_ID itemid = NCFG_ITEM_SPOT_SPLIT_1X1;
				
				if(!pConfigBlock.GetConfig(itemid, &pItemData, itemDataLen))
				{
					assert(false);
					continue;
				}

				unsigned long *data = (unsigned long *)pItemData;
				int groupNum = data[1];
				data = data + 2;
				for (int group = 0; group < groupNum; group++)
				{
					ITEM_INFO item_info;
					item_info.itemID = DD_CONFIG_ITEM_LIVE_SPOT_CAMERA;
					item_info.parserLen = sizeof(DD_LIVE_VIDEO_GROUP);
					item_info.pConfigItem = (unsigned char *)new DD_LIVE_VIDEO_GROUP;
					memset(item_info.pConfigItem, 0, sizeof(DD_LIVE_VIDEO_GROUP));
					DD_LIVE_VIDEO_GROUP *lvg = reinterpret_cast<DD_LIVE_VIDEO_GROUP *>(item_info.pConfigItem);
					lvg->channelNum = cameraNum;
					lvg->splitMode = splitMode;
					memset(lvg->channel, 0xff, DD_MAX_CAMERA_NUM * sizeof(unsigned char));
					for (int i = 0; i < DDSplitModeToNum(splitMode); i++)
					{
						lvg->channel[data[i]] = i;
					}

					data = data + DDSplitModeToNum(splitMode);
					iteminfolist.push_back(item_info);
				}
				break;
			}

		case NCFG_ITEM_SPOT_DWELL_TIME:
			{
				if (iteminfolist.empty())
				{
					ITEM_INFO item_info;
					item_info.itemID = DD_CONFIG_ITEM_LIVE_SPOT_CAMERA;
					item_info.parserLen = sizeof(DD_LIVE_VIDEO_GROUP);
					item_info.pConfigItem = (unsigned char *)new DD_LIVE_VIDEO_GROUP;
					memset(item_info.pConfigItem, 0, sizeof(DD_LIVE_VIDEO_GROUP));
					((DD_LIVE_VIDEO_GROUP*)item_info.pConfigItem)->holdTime = *((unsigned short*)pItemData);

					iteminfolist.push_back(item_info);
				}
				else
				{
					for (its = iteminfolist.begin(); its != iteminfolist.end(); its++)
					{
						if ((*its).itemID == DD_CONFIG_ITEM_LIVE_SPOT_CAMERA)
						{
							((DD_LIVE_VIDEO_GROUP*)(*its).pConfigItem)->holdTime = *((unsigned short*)pItemData);
						}
					}
				}
				break;
			}

		case NCFG_ITEM_LIVE_AUDIO_VOLUME:
			{
				if (iteminfolist.empty())
				{
					ITEM_INFO item_info;
					
					item_info.itemID = DD_CONFIG_ITEM_LIVE_AUDIO;
					item_info.parserLen = sizeof(DD_LIVE_AUDIO_GROUP);
					item_info.pConfigItem = (unsigned char *)new DD_LIVE_AUDIO_GROUP;
					memset(item_info.pConfigItem, 0, sizeof(DD_LIVE_AUDIO_GROUP));
					((DD_LIVE_AUDIO_GROUP*)item_info.pConfigItem)->volume = *pItemData;
					iteminfolist.push_back(item_info);
					
				}
				else
				{
					for (its = iteminfolist.begin(); its != iteminfolist.end(); its++)
					{
						if ((*its).itemID == DD_CONFIG_ITEM_LIVE_AUDIO)
						{
							((DD_LIVE_AUDIO_GROUP*)(*its).pConfigItem)->volume = *pItemData;
							break;
						}
					}
				}
				break;
			}
		case NCFG_ITEM_LIVE_AUDIO_CHNN:
			{
				if (iteminfolist.empty())
				{
					ITEM_INFO item_info;

					item_info.itemID = DD_CONFIG_ITEM_LIVE_AUDIO;
					item_info.parserLen = sizeof(DD_LIVE_AUDIO_GROUP);
					item_info.pConfigItem = (unsigned char *)new DD_LIVE_AUDIO_GROUP;
					memset(item_info.pConfigItem, 0, sizeof(DD_LIVE_AUDIO_GROUP));
					((DD_LIVE_AUDIO_GROUP*)item_info.pConfigItem)->channel = *pItemData;
					((DD_LIVE_AUDIO_GROUP*)item_info.pConfigItem)->holdTime = 1;
					iteminfolist.push_back(item_info);
				}
				else
				{
					for (its = iteminfolist.begin(); its != iteminfolist.end(); its++)
					{
						if ((*its).itemID == DD_CONFIG_ITEM_LIVE_AUDIO)
						{
							((DD_LIVE_AUDIO_GROUP*)(*its).pConfigItem)->channel = *pItemData;
							((DD_LIVE_AUDIO_GROUP*)(*its).pConfigItem)->holdTime = 1;
							break;
						}
					}
				}
				break;
			}
		case NCFG_ITEM_LIVE_TIME_INFO:
			{
				if (iteminfolist.empty())
				{
					ITEM_INFO item_info;
					item_info.itemID = DD_CONFIG_ITEM_LIVE_DISPLAY;
					item_info.parserLen = sizeof(DD_LIVE_DISPLAY);
					item_info.pConfigItem = (unsigned char *)new DD_LIVE_DISPLAY;
					memset(item_info.pConfigItem, 0, sizeof(DD_LIVE_DISPLAY));
					((DD_LIVE_DISPLAY*)item_info.pConfigItem)->showTime = ((OSD_INFO*)pItemData)->bEnable;
					
					iteminfolist.push_back(item_info);
				}
				else
				{
					for (its = iteminfolist.begin(); its != iteminfolist.end(); its++)
					{
						if ((*its).itemID == DD_CONFIG_ITEM_LIVE_DISPLAY)
						{
							((DD_LIVE_DISPLAY*)(*its).pConfigItem)->showTime = ((OSD_INFO*)pItemData)->bEnable;
							break;
						}
					}
				}
				break;
			}
		case NCFG_ITEM_LIVE_REC_STATUS:
			{
				if (iteminfolist.empty())
				{
					ITEM_INFO item_info;
					item_info.itemID = DD_CONFIG_ITEM_LIVE_DISPLAY;
					item_info.parserLen = sizeof(DD_LIVE_DISPLAY);
					item_info.pConfigItem = (unsigned char *)new DD_LIVE_DISPLAY;
					memset(item_info.pConfigItem, 0, sizeof(DD_LIVE_DISPLAY));
					//strcpy((char*)((DD_LIVE_DISPLAY*)item_info.pConfigItem)->showRecordStatus, (char*)pItemData);
					memset(((DD_LIVE_DISPLAY*)item_info.pConfigItem)->showRecordStatus, *pItemData, DD_MAX_CAMERA_NUM);
					iteminfolist.push_back(item_info);
				}
				else
				{
					for (its = iteminfolist.begin(); its != iteminfolist.end(); its++)
					{
						if ((*its).itemID == DD_CONFIG_ITEM_LIVE_DISPLAY)
						{
							//strcpy((char*)((DD_LIVE_DISPLAY*)(*its).pConfigItem)->showRecordStatus, (char*)pItemData);
							memset(((DD_LIVE_DISPLAY*)(*its).pConfigItem)->showRecordStatus, *pItemData, DD_MAX_CAMERA_NUM);
							break;
						}
					}
				}
				break;
			}
		case NCFG_ITEM_CAMERA_NAME:
			{
				if (iteminfolist.empty())
				{
					ITEM_INFO item_info[DD_MAX_CAMERA_NUM];
					for (int i = 0; i < itemHead.num; i++)
					{
						item_info[i].itemID = DD_CONFIG_ITEM_CHNN_CONFIG;
						item_info[i].parserLen = sizeof(DD_CHANNEL_CONFIG);
						item_info[i].pConfigItem = (unsigned char *)new DD_CHANNEL_CONFIG;
						memset(item_info[i].pConfigItem, 0, sizeof(DD_CHANNEL_CONFIG));
						strcpy(((DD_CHANNEL_CONFIG*)item_info[i].pConfigItem)->name, ((NCFG_INFO_SHORT_NAME*)pItemData)->name);
						pItemData += sizeof(((NCFG_INFO_SHORT_NAME*)pItemData)->name);

						iteminfolist.push_back(item_info[i]);
					}
				}
				else
				{
					for (its = iteminfolist.begin(); its != iteminfolist.end(); its++)
					{
						if ((*its).itemID == DD_CONFIG_ITEM_CHNN_CONFIG)
						{
							strcpy(((DD_CHANNEL_CONFIG*)(*its).pConfigItem)->name, ((NCFG_INFO_SHORT_NAME*)pItemData)->name);
							pItemData += sizeof(((NCFG_INFO_SHORT_NAME*)pItemData)->name);
						}
					}
				}
				break;
			}
		case NCFG_ITEM_SHOW_CAM_NAME:
			{
				if (iteminfolist.empty())
				{
					ITEM_INFO item_info[DD_MAX_CAMERA_NUM];
					for (int i = 0; i < itemHead.num; i++)
					{
						item_info[i].itemID = DD_CONFIG_ITEM_CHNN_CONFIG;
						item_info[i].parserLen = sizeof(DD_CHANNEL_CONFIG);
						item_info[i].pConfigItem = (unsigned char *)new DD_CHANNEL_CONFIG;
						memset(item_info[i].pConfigItem, 0, sizeof(DD_CHANNEL_CONFIG));
						((DD_CHANNEL_CONFIG*)item_info[i].pConfigItem)->hide = *((unsigned long*)pItemData);
						pItemData += sizeof(unsigned long);

						iteminfolist.push_back(item_info[i]);
					}
				}
				else
				{
					for (its = iteminfolist.begin(); its != iteminfolist.end(); its++)
					{
						if ((*its).itemID == DD_CONFIG_ITEM_CHNN_CONFIG)
						{
							((DD_CHANNEL_CONFIG*)(*its).pConfigItem)->hide = *((unsigned long*)pItemData);
							pItemData += sizeof(unsigned long);
						}
						else if ((*its).itemID == DD_CONFIG_ITEM_LIVE_DISPLAY)
						{
							char *pTemp = (char *)((DD_LIVE_DISPLAY*)(*its).pConfigItem)->showCameraName;
							for (int i = 0; i < m_deiviceInfo.videoInputNum; i++)
							{
								strcpy(pTemp, (char*)pItemData);
								pItemData += sizeof(unsigned long);
								pTemp ++;
							}
						}
					}
				}
				break;
			}
		case NCFG_ITEM_VIDEO_COLOR:
			{
				if (iteminfolist.empty())
				{
					ITEM_INFO item_info;
					for (int i = 0; i < itemHead.num; i++)
					{
						item_info.itemID = DD_CONFIG_ITEM_VIDEO_COLOR;
						item_info.parserLen = sizeof(DD_VIDEO_COLOR_CONFIG);
						item_info.pConfigItem = (unsigned char *)new DD_VIDEO_COLOR_CONFIG;
						memset(item_info.pConfigItem, 0, sizeof(DD_VIDEO_COLOR_CONFIG));
						long color = *((unsigned long*)pItemData);
						((DD_VIDEO_COLOR_CONFIG*)item_info.pConfigItem)->videoColor[0].brightness = (color >> 24) & 0xff;
						((DD_VIDEO_COLOR_CONFIG*)item_info.pConfigItem)->videoColor[0].contrast = (color >> 16) & 0xff;
						((DD_VIDEO_COLOR_CONFIG*)item_info.pConfigItem)->videoColor[0].hue = color & 0xff;
						((DD_VIDEO_COLOR_CONFIG*)item_info.pConfigItem)->videoColor[0].saturation = (color >> 8) & 0xff;
						pItemData += sizeof(unsigned long);

						iteminfolist.push_back(item_info);
					}
				}
				else
				{
					for (its = iteminfolist.begin(); its != iteminfolist.end(); its++)
					{
						if ((*its).itemID == DD_CONFIG_ITEM_VIDEO_COLOR)
						{
							long color = *((unsigned long*)pItemData);
							((DD_VIDEO_COLOR_CONFIG*)(*its).pConfigItem)->videoColor[0].brightness = (color >> 24) & 0xff;
							((DD_VIDEO_COLOR_CONFIG*)(*its).pConfigItem)->videoColor[0].contrast = (color >> 16) & 0xff;
							((DD_VIDEO_COLOR_CONFIG*)(*its).pConfigItem)->videoColor[0].hue = color & 0xff;
							((DD_VIDEO_COLOR_CONFIG*)(*its).pConfigItem)->videoColor[0].saturation = (color >> 8) & 0xff;
							pItemData += sizeof(unsigned long);
						}
					}
				}
				break;
			}
		case NCFG_ITEM_TIME_STAMP_INFO:
			{
				if (iteminfolist.empty())
				{
					ITEM_INFO item_info[DD_MAX_CAMERA_NUM];
					for (int i = 0; i < itemHead.num; i++)
					{
						item_info[i].itemID = DD_CONFIG_ITEM_VIDEO_OSD;
						item_info[i].parserLen = sizeof(DD_VIDEO_OSD_CONFIG);
						item_info[i].pConfigItem = (unsigned char *)new DD_VIDEO_OSD_CONFIG;
						memset(item_info[i].pConfigItem, 0, sizeof(DD_VIDEO_OSD_CONFIG));
						((DD_VIDEO_OSD_CONFIG*)item_info[i].pConfigItem)->enableTimeStamp = ((OSD_INFO*)pItemData)->bEnable;
						((DD_VIDEO_OSD_CONFIG*)item_info[i].pConfigItem)->timeStamp.x = ((OSD_INFO*)pItemData)->x;
						((DD_VIDEO_OSD_CONFIG*)item_info[i].pConfigItem)->timeStamp.y = ((OSD_INFO*)pItemData)->y;

						pItemData += sizeof(OSD_INFO);
						iteminfolist.push_back(item_info[i]);
					}
				}
				else
				{
					for (its = iteminfolist.begin(); its != iteminfolist.end(); its++)
					{
						if ((*its).itemID == DD_CONFIG_ITEM_VIDEO_OSD)
						{
							((DD_VIDEO_OSD_CONFIG*)(*its).pConfigItem)->enableTimeStamp = ((OSD_INFO*)pItemData)->bEnable;
							((DD_VIDEO_OSD_CONFIG*)(*its).pConfigItem)->timeStamp.x = ((OSD_INFO*)pItemData)->x;
							((DD_VIDEO_OSD_CONFIG*)(*its).pConfigItem)->timeStamp.y = ((OSD_INFO*)pItemData)->y;

							pItemData += sizeof(OSD_INFO);
						}
					}
				}
				break;
			}
		case NCFG_ITEM_CAM_NAME_STAMP_INFO:
			{
				if (iteminfolist.empty())
				{
					ITEM_INFO item_info[DD_MAX_CAMERA_NUM];
					for (int i = 0; i < itemHead.num; i++)
					{
						item_info[i].itemID = DD_CONFIG_ITEM_VIDEO_OSD;
						item_info[i].parserLen = sizeof(DD_VIDEO_OSD_CONFIG);
						item_info[i].pConfigItem = (unsigned char *)new DD_VIDEO_OSD_CONFIG;
						memset(item_info[i].pConfigItem, 0, sizeof(DD_VIDEO_OSD_CONFIG));
						((DD_VIDEO_OSD_CONFIG*)item_info[i].pConfigItem)->enableCameraName = ((OSD_INFO*)pItemData)->bEnable;
						((DD_VIDEO_OSD_CONFIG*)item_info[i].pConfigItem)->cameraName.x = ((OSD_INFO*)pItemData)->x;
						((DD_VIDEO_OSD_CONFIG*)item_info[i].pConfigItem)->cameraName.y = ((OSD_INFO*)pItemData)->y;

						pItemData += sizeof(OSD_INFO);
						iteminfolist.push_back(item_info[i]);
					}
				}
				else
				{
					for (its = iteminfolist.begin(); its != iteminfolist.end(); its++)
					{
						if ((*its).itemID == DD_CONFIG_ITEM_VIDEO_OSD)
						{
							((DD_VIDEO_OSD_CONFIG*)(*its).pConfigItem)->enableCameraName = ((OSD_INFO*)pItemData)->bEnable;
							((DD_VIDEO_OSD_CONFIG*)(*its).pConfigItem)->cameraName.x = ((OSD_INFO*)pItemData)->x;
							((DD_VIDEO_OSD_CONFIG*)(*its).pConfigItem)->cameraName.y = ((OSD_INFO*)pItemData)->y;

							pItemData += sizeof(OSD_INFO);
						}
					}
				}
				break;
			}
		case NCFG_ITEM_CAM_COVER_INFO:
			{
				if (iteminfolist.empty())
				{
					ITEM_INFO item_info[DD_MAX_CAMERA_NUM];
					for (int i = 0; i < itemHead.num; i++)
					{
						item_info[i].itemID = DD_CONFIG_ITEM_VIDEO_OSD;
						item_info[i].parserLen = sizeof(DD_VIDEO_OSD_CONFIG);
						item_info[i].pConfigItem = (unsigned char *)new DD_VIDEO_OSD_CONFIG;
						memset(item_info[i].pConfigItem, 0, sizeof(DD_VIDEO_OSD_CONFIG));

						DD_VIDEO_OSD_CONFIG* pTmpOSDCfg = (DD_VIDEO_OSD_CONFIG*)item_info[i].pConfigItem;
						CAMERA_COVER_INFO* pTmpCoverInfo = (CAMERA_COVER_INFO*)pItemData;
						for( int idx_tmp = 0; idx_tmp < DD_MAX_VIDEO_COVER_NUM; idx_tmp++)
						{

							if( 0 != pTmpCoverInfo->area[idx_tmp].cx  && 0 != pTmpCoverInfo->area[idx_tmp].cy )
							{
								pTmpOSDCfg->cover[idx_tmp].enable = 1;
								pTmpOSDCfg->cover[idx_tmp].area.x = pTmpCoverInfo->area[idx_tmp].x;
								pTmpOSDCfg->cover[idx_tmp].area.y = pTmpCoverInfo->area[idx_tmp].y;
								pTmpOSDCfg->cover[idx_tmp].area.cx = pTmpCoverInfo->area[idx_tmp].cx;
								pTmpOSDCfg->cover[idx_tmp].area.cy = pTmpCoverInfo->area[idx_tmp].cy;
							}
							else
							{
								pTmpOSDCfg->cover[idx_tmp].enable =  0;
							}
							
						}

						pItemData += sizeof(CAMERA_COVER_INFO);
						iteminfolist.push_back(item_info[i]);
					}
				}
				else
				{
					for (its = iteminfolist.begin(); its != iteminfolist.end(); its++)
					{
						if ((*its).itemID == DD_CONFIG_ITEM_VIDEO_OSD)
						{
							DD_VIDEO_OSD_CONFIG* pTmpOSDCfg = (DD_VIDEO_OSD_CONFIG*)(*its).pConfigItem;
							CAMERA_COVER_INFO* pTmpCoverInfo = (CAMERA_COVER_INFO*)pItemData;
							for( int idx_tmp = 0; idx_tmp < DD_MAX_VIDEO_COVER_NUM; idx_tmp++)
							{

								if( 0 != pTmpCoverInfo->area[idx_tmp].cx  && 0 != pTmpCoverInfo->area[idx_tmp].cy )
								{
									pTmpOSDCfg->cover[idx_tmp].enable = 1;
									pTmpOSDCfg->cover[idx_tmp].area.x = pTmpCoverInfo->area[idx_tmp].x;
									pTmpOSDCfg->cover[idx_tmp].area.y = pTmpCoverInfo->area[idx_tmp].y;
									pTmpOSDCfg->cover[idx_tmp].area.cx = pTmpCoverInfo->area[idx_tmp].cx;
									pTmpOSDCfg->cover[idx_tmp].area.cy = pTmpCoverInfo->area[idx_tmp].cy;
								}
								else
								{
									pTmpOSDCfg->cover[idx_tmp].enable =  0;
								}

							}

							pItemData += sizeof(CAMERA_COVER_INFO);
						}
					}
				}
				break;
			}
		case NCFG_ITEM_CAM_ROI_INFO:    
			{
				if (iteminfolist.empty())
				{
					ITEM_INFO item_info[DD_MAX_CAMERA_NUM];
					for (int i = 0; i < itemHead.num; i++)
					{
						item_info[i].itemID = DD_CONFIG_ITEM_VIDEO_ROI;
						item_info[i].parserLen = sizeof(DD_VIDEO_ROI_CONFIG);
						item_info[i].pConfigItem = (unsigned char *)new DD_VIDEO_ROI_CONFIG;
						memset(item_info[i].pConfigItem, 0, sizeof(DD_VIDEO_ROI_CONFIG));

						DD_VIDEO_ROI_CONFIG* pTmpROICfg = (DD_VIDEO_ROI_CONFIG*)item_info[i].pConfigItem;
					    VIDEO_CONFIG_ROI_INFO* pTmpROIInfo = (VIDEO_CONFIG_ROI_INFO*)pItemData;
						unsigned int tempSize = sizeof(pTmpROIInfo->roi[0].szAreaName);
						for( int idx_tmp = 0; idx_tmp < DD_MAX_VIDEO_ROI_NUM; idx_tmp++)
						{
							/*if (0 != pTmpROIInfo->roi[idx_tmp].w && 0!=pTmpROIInfo->roi[idx_tmp].h)*/
							{
								pTmpROICfg->roi[idx_tmp].iChn = pTmpROIInfo->roi[idx_tmp].iChn;
								pTmpROICfg->roi[idx_tmp].iEnable = pTmpROIInfo->roi[idx_tmp].iEnable;
								pTmpROICfg->roi[idx_tmp].iLevel = pTmpROIInfo->roi[idx_tmp].iLevel;
								memcpy(pTmpROICfg->roi[idx_tmp].szAreaName,pTmpROIInfo->roi[idx_tmp].szAreaName,tempSize);
								pTmpROICfg->roi[idx_tmp].x = pTmpROIInfo->roi[idx_tmp].x;
								pTmpROICfg->roi[idx_tmp].y = pTmpROIInfo->roi[idx_tmp].y;
								pTmpROICfg->roi[idx_tmp].h = pTmpROIInfo->roi[idx_tmp].h;
								pTmpROICfg->roi[idx_tmp].w = pTmpROIInfo->roi[idx_tmp].w;
							}/*else{
								pTmpROICfg->roi[idx_tmp].iEnable = 0;
								pTmpROICfg->roi[idx_tmp].iLevel  = 1;
							}*/
						}

						pItemData += sizeof(VIDEO_CONFIG_ROI_INFO);
						iteminfolist.push_back(item_info[i]);
					}
				}else{
					for (its = iteminfolist.begin(); its != iteminfolist.end(); its++)
					{
						if ((*its).itemID == DD_CONFIG_ITEM_VIDEO_ROI)
						{
							DD_VIDEO_ROI_CONFIG* pTmpROICfg = (DD_VIDEO_ROI_CONFIG*)(*its).pConfigItem;
							VIDEO_CONFIG_ROI_INFO* pTmpROIInfo = (VIDEO_CONFIG_ROI_INFO*)pItemData;
							unsigned int tempSize = sizeof(pTmpROIInfo->roi[0].szAreaName);
							for( int idx_tmp = 0; idx_tmp < DD_MAX_VIDEO_ROI_NUM; idx_tmp++)
							{
								if (0 != pTmpROIInfo->roi[idx_tmp].w && 0!=pTmpROIInfo->roi[idx_tmp].h)
								{
									pTmpROICfg->roi[idx_tmp].iChn = pTmpROIInfo->roi[idx_tmp].iChn;
									pTmpROICfg->roi[idx_tmp].iEnable = pTmpROIInfo->roi[idx_tmp].iEnable;
									pTmpROICfg->roi[idx_tmp].iLevel = pTmpROIInfo->roi[idx_tmp].iLevel;
									memcpy(pTmpROICfg->roi[idx_tmp].szAreaName,pTmpROIInfo->roi[idx_tmp].szAreaName,tempSize);
									pTmpROICfg->roi[idx_tmp].x = pTmpROIInfo->roi[idx_tmp].x;
									pTmpROICfg->roi[idx_tmp].y = pTmpROIInfo->roi[idx_tmp].y;
									pTmpROICfg->roi[idx_tmp].h = pTmpROIInfo->roi[idx_tmp].h;
									pTmpROICfg->roi[idx_tmp].w = pTmpROIInfo->roi[idx_tmp].w;
								}else{
									pTmpROICfg->roi[idx_tmp].iEnable = 0;
								}
							}
							pItemData += sizeof(VIDEO_CONFIG_ROI_INFO);
						}
					}
				}
				break;
			}
		case NCFG_ITEM_CAM_MIRROR:	
			if (iteminfolist.empty())
			{
				ITEM_INFO item_info;
				for(int i=0; i< itemHead.num; i++)
				{
					item_info.itemID = DD_CONFIG_ITEM_CAM_MIRROR;
					item_info.parserLen = sizeof(unsigned char);
					item_info.pConfigItem = new unsigned char;
					*(item_info.pConfigItem) = *((unsigned char *)pItemData);
					iteminfolist.push_back(item_info);
					++pItemData;
				}
			}else{
				for (its = iteminfolist.begin(); its != iteminfolist.end(); its++)
				{
					if ((*its).itemID == DD_CONFIG_ITEM_CAM_MIRROR)
					{
						*((*its).pConfigItem) = *((unsigned char *)pItemData);
						++pItemData;
					}
				}
			}
			break;
		case NCFG_ITEM_CAM_FILP:
			if (iteminfolist.empty())
			{
				ITEM_INFO item_info;
				for(int i=0; i< itemHead.num; i++)
				{
					item_info.itemID = DD_CONFIG_ITEM_CAM_FILP;
					item_info.parserLen = sizeof(unsigned char);
					item_info.pConfigItem = new unsigned char;
					*(item_info.pConfigItem) = *((unsigned char *)pItemData);
					iteminfolist.push_back(item_info);
					++pItemData;
				}
			}else{
				for (its = iteminfolist.begin(); its != iteminfolist.end(); its++)
				{
					if ((*its).itemID == DD_CONFIG_ITEM_CAM_FILP)
					{
						*((*its).pConfigItem) = *((unsigned char *)pItemData);
						++pItemData;
					}
				}
			}
			break;
		case NCFG_ITEM_CAM_ROTATO:
			if (iteminfolist.empty())
			{
				ITEM_INFO item_info;
				for(int i=0; i< itemHead.num; i++)
				{
					item_info.itemID = DD_CONFIG_ITEM_CAM_ROTATO;
					item_info.parserLen = sizeof(unsigned long);
					item_info.pConfigItem = (unsigned char *)new unsigned long;
					*(unsigned long *)(item_info.pConfigItem) = *((unsigned long *)pItemData);
					iteminfolist.push_back(item_info);
					++pItemData;
				}
			}else{
				for (its = iteminfolist.begin(); its != iteminfolist.end(); its++)
				{
					if ((*its).itemID == DD_CONFIG_ITEM_CAM_ROTATO)
					{
						*(unsigned long *)((*its).pConfigItem) = *((unsigned long *)pItemData);
						pItemData += sizeof(unsigned long);
					}
				}
			}
			break;
		case NCFG_ITEM_CAM_LENS_DISTORTION:
			if (iteminfolist.empty())
			{
				ITEM_INFO item_info;
				for(int i=0; i< itemHead.num; i++)
				{
					item_info.itemID = DD_CONFIG_ITEM_CAM_LENS_DISTORTION;
					item_info.parserLen = sizeof(LENS_DISTORTION_CONFIG);
					item_info.pConfigItem = (unsigned char *)new LENS_DISTORTION_CONFIG;
					LENS_DISTORTION_CONFIG * pSDK = (LENS_DISTORTION_CONFIG *)item_info.pConfigItem;
                    LENS_DISTORTION_CONFIG * pDevice = (LENS_DISTORTION_CONFIG *)pItemData;
					pSDK->ucLensDistortion = pDevice->ucLensDistortion;
					pSDK->ucLensDistortionValue = pDevice->ucLensDistortionValue;
					strcpy(pSDK->noUse,pDevice->noUse);
					iteminfolist.push_back(item_info);
				    pItemData+=sizeof(LENS_DISTORTION_CONFIG);
				}
			}else{
				for (its = iteminfolist.begin(); its != iteminfolist.end(); its++)
				{
					if ((*its).itemID == DD_CONFIG_ITEM_CAM_LENS_DISTORTION)
					{
						LENS_DISTORTION_CONFIG * pSDK = (LENS_DISTORTION_CONFIG *)((*its).pConfigItem);
						LENS_DISTORTION_CONFIG * pDevice = (LENS_DISTORTION_CONFIG *)pItemData;
						pSDK->ucLensDistortion = pDevice->ucLensDistortion;
						pSDK->ucLensDistortionValue = pDevice->ucLensDistortionValue;
						strcpy(pSDK->noUse,pDevice->noUse);
						pItemData+=sizeof(LENS_DISTORTION_CONFIG);
					}
				}
			}
			break;	
		case NCFG_ITEM_CAM_TIME_FOCUS_MODE:
			if (iteminfolist.empty())
			{
				ITEM_INFO item_info;
				for(int i=0; i< itemHead.num; i++)
				{
					item_info.itemID = DD_CONFIG_ITEM_CAM_TIME_FOCUS_MODE;
					item_info.parserLen = sizeof(unsigned char);
					item_info.pConfigItem = new unsigned char;
					*(item_info.pConfigItem) = *((unsigned char *)pItemData);
					iteminfolist.push_back(item_info);
					++pItemData;
				}
			}else{
				for (its = iteminfolist.begin(); its != iteminfolist.end(); its++)
				{
					if ((*its).itemID == DD_CONFIG_ITEM_CAM_TIME_FOCUS_MODE)
					{
						*((*its).pConfigItem) = *((unsigned char *)pItemData);
						++pItemData;
					}
				}
			}
			break;
		case NCFG_ITEM_CAM_IRCHANGE_FOCUS:	
			if (iteminfolist.empty())
			{
				ITEM_INFO item_info;
				for(int i=0; i< itemHead.num; i++)
				{
					item_info.itemID = DD_CONFIG_ITEM_CAM_IRCHANGE_FOCUS;
					item_info.parserLen = sizeof(unsigned char);
					item_info.pConfigItem = new unsigned char;
					*(item_info.pConfigItem) = *((unsigned char *)pItemData);
					iteminfolist.push_back(item_info);
					++pItemData;
				}
			}else{
				for (its = iteminfolist.begin(); its != iteminfolist.end(); its++)
				{
					if ((*its).itemID == DD_CONFIG_ITEM_CAM_IRCHANGE_FOCUS)
					{
						*((*its).pConfigItem) = *((unsigned char *)pItemData);
						++pItemData;
					}
				}
			}
			break;
		case NCFG_ITEM_CAM_INTERVAL_TIME:
			if (iteminfolist.empty())
			{
				ITEM_INFO item_info;
				for(int i=0; i< itemHead.num; i++)
				{
					item_info.itemID = DD_CONFIG_ITEM_CAM_INTERVAL_TIME;
					item_info.parserLen = sizeof(unsigned long);
					item_info.pConfigItem = (unsigned char *)new unsigned long;
					*(unsigned long *)(item_info.pConfigItem) = *((unsigned long *)pItemData);
					iteminfolist.push_back(item_info);
					pItemData+=sizeof(unsigned long);
				}
			}else{
				for (its = iteminfolist.begin(); its != iteminfolist.end(); its++)
				{
					if ((*its).itemID == DD_CONFIG_ITEM_CAM_INTERVAL_TIME)
					{
						*(unsigned long *)((*its).pConfigItem) = *((unsigned long *)pItemData);
						pItemData+=sizeof(unsigned long);
					}
				}
			}
			break;
		case NCFG_ITEM_ENCODE_MAJOR_SUPPORT_PROPERTY:
			{

				if (iteminfolist.empty())
				{
					ITEM_INFO item_info;
					item_info.itemID = DD_CONFIG_ITEM_ENCODE_MASK_MAJOR;
					item_info.parserLen = sizeof(DD_ENCODE_CONFIG);
					item_info.pConfigItem = (unsigned char *)new DD_ENCODE_CONFIG;
					memset(item_info.pConfigItem, 0x0, sizeof(DD_ENCODE_CONFIG));
					DD_ENCODE_CONFIG *item = reinterpret_cast<DD_ENCODE_CONFIG *>(item_info.pConfigItem);
					NCFG_INFO_SUPPORT_FRAME_RATE * mask = reinterpret_cast<NCFG_INFO_SUPPORT_FRAME_RATE *>(pItemData);
					item->resolution = mask->dwResolution;
					
					unsigned char ntfs= (unsigned char)MyVRateToRate(mask->chnnRateNtscOnCif, DD_VIDEO_FORMAT_NTSC);
					unsigned char pal = (unsigned char)MyVRateToRate(mask->chnnRatePalOnCif, DD_VIDEO_FORMAT_PAL);

					item->rate = ((unsigned short)ntfs) | ((unsigned short)pal) << 8;

					iteminfolist.push_back(item_info);
				}
				else
				{
					for (its = iteminfolist.begin(); its != iteminfolist.end(); its++)
					{
						if ((*its).itemID == DD_CONFIG_ITEM_ENCODE_MASK_MAJOR)
						{
							DD_ENCODE_CONFIG *item = reinterpret_cast<DD_ENCODE_CONFIG *>(ITEM_INFO(*its).pConfigItem);
							NCFG_INFO_SUPPORT_FRAME_RATE * mask = reinterpret_cast<NCFG_INFO_SUPPORT_FRAME_RATE *>(pItemData);
							item->resolution = mask->dwResolution;
						}
					}
				}

				break;
			}
		case NCFG_ITEM_MAJOR_SUPPORT_QULITY_LEVELNUM:
			{
				if (iteminfolist.empty())
				{
					ITEM_INFO item_info;
					item_info.itemID = DD_CONFIG_ITEM_ENCODE_MASK_MAJOR;
					item_info.parserLen = sizeof(DD_ENCODE_CONFIG);
					item_info.pConfigItem = (unsigned char *)new DD_ENCODE_CONFIG;
					memset(item_info.pConfigItem, 0x0, sizeof(DD_ENCODE_CONFIG));
					((DD_ENCODE_CONFIG*)item_info.pConfigItem)->iSize = sizeof(DD_ENCODE_CONFIG);
					((DD_ENCODE_CONFIG*)item_info.pConfigItem)->encodeType = ((ENCODE_INFO*)pItemData)->encodeType;
					((DD_ENCODE_CONFIG*)item_info.pConfigItem)->maxBitrate = ((ENCODE_INFO*)pItemData)->hBitStream;
					((DD_ENCODE_CONFIG*)item_info.pConfigItem)->minBitrate = ((ENCODE_INFO*)pItemData)->lBitStream;
					((DD_ENCODE_CONFIG*)item_info.pConfigItem)->quality = ((ENCODE_INFO*)pItemData)->quality;
					((DD_ENCODE_CONFIG*)item_info.pConfigItem)->rate = ((ENCODE_INFO*)pItemData)->rate;
					((DD_ENCODE_CONFIG*)item_info.pConfigItem)->resolution = ((ENCODE_INFO*)pItemData)->resolution;
				}
				else
				{
					for (its = iteminfolist.begin(); its != iteminfolist.end(); its++)
					{
						if ((*its).itemID == DD_CONFIG_ITEM_ENCODE_MASK_MAJOR)
						{
							DD_ENCODE_CONFIG *item = reinterpret_cast<DD_ENCODE_CONFIG *>(ITEM_INFO(*its).pConfigItem);
							((DD_ENCODE_CONFIG*)(*its).pConfigItem)->iSize = sizeof(DD_ENCODE_CONFIG);
							((DD_ENCODE_CONFIG*)(*its).pConfigItem)->encodeType = ((ENCODE_INFO*)pItemData)->encodeType;
							((DD_ENCODE_CONFIG*)(*its).pConfigItem)->maxBitrate = ((ENCODE_INFO*)pItemData)->hBitStream;
							((DD_ENCODE_CONFIG*)(*its).pConfigItem)->minBitrate = ((ENCODE_INFO*)pItemData)->lBitStream;
							((DD_ENCODE_CONFIG*)(*its).pConfigItem)->quality = ((ENCODE_INFO*)pItemData)->quality;
							((DD_ENCODE_CONFIG*)(*its).pConfigItem)->rate = ((ENCODE_INFO*)pItemData)->rate;
							((DD_ENCODE_CONFIG*)(*its).pConfigItem)->resolution = ((ENCODE_INFO*)pItemData)->resolution;
						}
					}
				}
				break;
			}
		case NCFG_ITEM_MAJOR_SUPPORT_ENCODEMODE:
			{
				if (iteminfolist.empty())
				{
					ITEM_INFO item_info;
					item_info.itemID = DD_CONFIG_ITEM_ENCODE_MASK_MAJOR;
					item_info.parserLen = sizeof(DD_ENCODE_CONFIG);
					item_info.pConfigItem = (unsigned char *)new DD_ENCODE_CONFIG;
					memset(item_info.pConfigItem, 0x0, sizeof(DD_ENCODE_CONFIG));
					DD_ENCODE_CONFIG *item = reinterpret_cast<DD_ENCODE_CONFIG *>(item_info.pConfigItem);
					item->encodeType = *(unsigned long *)(pItemData);
					iteminfolist.push_back(item_info);
				}
				else
				{
					for (its = iteminfolist.begin(); its != iteminfolist.end(); its++)
					{
						if ((*its).itemID == DD_CONFIG_ITEM_ENCODE_MASK_MAJOR)
						{
							DD_ENCODE_CONFIG *item = reinterpret_cast<DD_ENCODE_CONFIG *>(ITEM_INFO(*its).pConfigItem);
							item->encodeType = *(unsigned long *)(pItemData);
						}
					}
				}
				break;
			}
		case NCFG_ITEM_MAJOR_SUPPORT_BITRATE_RANGE:
			{
				if (iteminfolist.empty())
				{
					ITEM_INFO item_info;
					item_info.itemID = DD_CONFIG_ITEM_ENCODE_MASK_MAJOR;
					item_info.parserLen = sizeof(DD_ENCODE_CONFIG);
					item_info.pConfigItem = (unsigned char *)new DD_ENCODE_CONFIG;
					memset(item_info.pConfigItem, 0x0, sizeof(DD_ENCODE_CONFIG));
					DD_ENCODE_CONFIG *item = reinterpret_cast<DD_ENCODE_CONFIG *>(item_info.pConfigItem);
					NCFG_INFO_BITRATE_RANG * mask = reinterpret_cast<NCFG_INFO_BITRATE_RANG *>(pItemData);
					item->maxBitrate = mask->high;
					item->minBitrate = mask->low;
					iteminfolist.push_back(item_info);
				}
				else
				{
					for (its = iteminfolist.begin(); its != iteminfolist.end(); its++)
					{
						if ((*its).itemID == DD_CONFIG_ITEM_ENCODE_MASK_MAJOR)
						{
							DD_ENCODE_CONFIG *item = reinterpret_cast<DD_ENCODE_CONFIG *>(ITEM_INFO(*its).pConfigItem);
							NCFG_INFO_BITRATE_RANG * mask = reinterpret_cast<NCFG_INFO_BITRATE_RANG *>(pItemData);
							item->maxBitrate = mask->high;
							item->minBitrate = mask->low;
						}
					}
				}
				break;
			}
		case NCFG_ITEM_ENCODE_MAJOR_EX:
			{
				if (iteminfolist.empty())
				{
					ITEM_INFO item_info[DD_MAX_CAMERA_NUM];
					for (int i = 0; i < m_deiviceInfo.videoInputNum; i++)
					{
						item_info[i].itemID = DD_CONFIG_ITEM_ENCODE_SCHEDULE;
						item_info[i].parserLen = sizeof(DD_ENCODE_CONFIG);
						item_info[i].pConfigItem = (unsigned char *)new DD_ENCODE_CONFIG;
						memset(item_info[i].pConfigItem, 0, sizeof(DD_RECORD_CONFIG));

						DD_ENCODE_CONFIG *pddEncode = (DD_ENCODE_CONFIG *)item_info[i].pConfigItem;
						ENCODE_INFO_EX *pEncodeEx = (ENCODE_INFO_EX *)pItemData;

						pddEncode->resolution = VideoSizeToDDVideoSize(pEncodeEx->resolution);
						pddEncode->rate = pEncodeEx->rate;
						pddEncode->encodeType = pEncodeEx->encodeType;
						pddEncode->quality = pEncodeEx->quality;
						pddEncode->minBitrate = pEncodeEx->lBitStream;
						pddEncode->maxBitrate = pEncodeEx->hBitStream;

						iteminfolist.push_back(item_info[i]);
						pItemData += sizeof(ENCODE_INFO_EX);
					}
				}
				else
				{
					int channelIndex = 0;
					for (its = iteminfolist.begin(); its != iteminfolist.end(); its++)
					{
						if ((*its).itemID == DD_CONFIG_ITEM_ENCODE_SCHEDULE)
						{
							channelIndex++;
							if (channelIndex<m_deiviceInfo.videoInputNum)
							{
								DD_ENCODE_CONFIG *pddEncode = (DD_ENCODE_CONFIG *)(*its).pConfigItem;
								ENCODE_INFO_EX *pEncodeEx = (ENCODE_INFO_EX *)pItemData;

								pddEncode->resolution = VideoSizeToDDVideoSize(pEncodeEx->resolution);
								pddEncode->rate = pEncodeEx->rate;
								pddEncode->encodeType = pEncodeEx->encodeType;
								pddEncode->quality = pEncodeEx->quality;
								pddEncode->minBitrate = pEncodeEx->lBitStream;
								pddEncode->maxBitrate = pEncodeEx->hBitStream;

								pItemData += sizeof(ENCODE_INFO_EX);
							}
						}
					}
				}
				break;
			}
		case NCFG_ITEM_ENCODE_MINOR_SUPPORT_PROPERTY:
			{

				if (iteminfolist.empty())
				{
					ITEM_INFO item_info;
					item_info.itemID = DD_CONFIG_ITEM_ENCODE_MASK_MINOR;
					item_info.parserLen = sizeof(DD_ENCODE_CONFIG);
					item_info.pConfigItem = (unsigned char *)new DD_ENCODE_CONFIG;
					memset(item_info.pConfigItem, 0x0, sizeof(DD_ENCODE_CONFIG));
					DD_ENCODE_CONFIG *item = reinterpret_cast<DD_ENCODE_CONFIG *>(item_info.pConfigItem);
					NCFG_INFO_SUPPORT_FRAME_RATE * mask = reinterpret_cast<NCFG_INFO_SUPPORT_FRAME_RATE *>(pItemData);
					item->resolution = mask->dwResolution;
					
					unsigned char ntfs= (unsigned char)MyVRateToRate(mask->chnnRateNtscOnCif, DD_VIDEO_FORMAT_NTSC);
					unsigned char pal = (unsigned char)MyVRateToRate(mask->chnnRatePalOnCif, DD_VIDEO_FORMAT_PAL);

					item->rate = ((unsigned short)ntfs) | ((unsigned short)pal << 8);

					iteminfolist.push_back(item_info);
				}
				else
				{
					for (its = iteminfolist.begin(); its != iteminfolist.end(); its++)
					{
						if ((*its).itemID == DD_CONFIG_ITEM_ENCODE_MASK_MAJOR)
						{
							DD_ENCODE_CONFIG *item = reinterpret_cast<DD_ENCODE_CONFIG *>(ITEM_INFO(*its).pConfigItem);
							NCFG_INFO_SUPPORT_FRAME_RATE * mask = reinterpret_cast<NCFG_INFO_SUPPORT_FRAME_RATE *>(pItemData);
							item->resolution = mask->dwResolution;
						}
					}
				}

				break;
			}
		case NCFG_ITEM_MINOR_SUPPORT_QULITY_LEVELNUM:
			{
				if (iteminfolist.empty())
				{
					ITEM_INFO item_info;
					item_info.itemID = DD_CONFIG_ITEM_ENCODE_MASK_MINOR;
					item_info.parserLen = sizeof(DD_ENCODE_CONFIG);
					item_info.pConfigItem = (unsigned char *)new DD_ENCODE_CONFIG;
					memset(item_info.pConfigItem, 0x0, sizeof(DD_ENCODE_CONFIG));
				}
				else
				{

				}
				break;
			}
		case NCFG_ITEM_MINOR_SUPPORT_ENCODEMODE:
			{
				if (iteminfolist.empty())
				{
					ITEM_INFO item_info;
					item_info.itemID = DD_CONFIG_ITEM_ENCODE_MASK_MINOR;
					item_info.parserLen = sizeof(DD_ENCODE_CONFIG);
					item_info.pConfigItem = (unsigned char *)new DD_ENCODE_CONFIG;
					memset(item_info.pConfigItem, 0x0, sizeof(DD_ENCODE_CONFIG));
					DD_ENCODE_CONFIG *item = reinterpret_cast<DD_ENCODE_CONFIG *>(item_info.pConfigItem);
					item->encodeType = *(unsigned long *)(pItemData);
					iteminfolist.push_back(item_info);
				}
				else
				{
					for (its = iteminfolist.begin(); its != iteminfolist.end(); its++)
					{
						if ((*its).itemID == DD_CONFIG_ITEM_ENCODE_MASK_MINOR)
						{
							DD_ENCODE_CONFIG *item = reinterpret_cast<DD_ENCODE_CONFIG *>(ITEM_INFO(*its).pConfigItem);
							item->encodeType = *(unsigned long *)(pItemData);
						}
					}
				}
				break;
			}
		case NCFG_ITEM_MINOR_SUPPORT_BITRATE_RANGE:
			{
				if (iteminfolist.empty())
				{
					ITEM_INFO item_info;
					item_info.itemID = DD_CONFIG_ITEM_ENCODE_MASK_MINOR;
					item_info.parserLen = sizeof(DD_ENCODE_CONFIG);
					item_info.pConfigItem = (unsigned char *)new DD_ENCODE_CONFIG;
					memset(item_info.pConfigItem, 0x0, sizeof(DD_ENCODE_CONFIG));
					DD_ENCODE_CONFIG *item = reinterpret_cast<DD_ENCODE_CONFIG *>(item_info.pConfigItem);
					NCFG_INFO_BITRATE_RANG * mask = reinterpret_cast<NCFG_INFO_BITRATE_RANG *>(pItemData);
					item->maxBitrate = mask->high;
					item->minBitrate = mask->low;
					iteminfolist.push_back(item_info);
				}
				else
				{
					for (its = iteminfolist.begin(); its != iteminfolist.end(); its++)
					{
						if ((*its).itemID == DD_CONFIG_ITEM_ENCODE_MASK_MINOR)
						{
							DD_ENCODE_CONFIG *item = reinterpret_cast<DD_ENCODE_CONFIG *>(ITEM_INFO(*its).pConfigItem);
							NCFG_INFO_BITRATE_RANG * mask = reinterpret_cast<NCFG_INFO_BITRATE_RANG *>(pItemData);
							item->maxBitrate = mask->high;
							item->minBitrate = mask->low;
						}
					}
				}
				break;
			}
		case NCFG_ITEM_ENCODE_MINOR_EX:
			{
				if (iteminfolist.empty())
				{
					ITEM_INFO item_info[DD_MAX_CAMERA_NUM];
					for (int i = 0; i < itemHead.num; i++)
					{
						item_info[i].itemID = DD_CONFIG_ITEM_ENCODE_NETWORK;
						item_info[i].parserLen = sizeof(DD_ENCODE_CONFIG);
						item_info[i].pConfigItem = (unsigned char *)new DD_ENCODE_CONFIG;
						memset(item_info[i].pConfigItem, 0, sizeof(DD_ENCODE_CONFIG));
						((DD_ENCODE_CONFIG*)item_info[i].pConfigItem)->resolution = VideoSizeToDDVideoSize(((ENCODE_INFO_EX*)pItemData)->resolution);
						((DD_ENCODE_CONFIG*)item_info[i].pConfigItem)->rate = (unsigned short)((ENCODE_INFO_EX*)pItemData)->rate;
						((DD_ENCODE_CONFIG*)item_info[i].pConfigItem)->encodeType = (unsigned short)((ENCODE_INFO_EX*)pItemData)->encodeType;
						((DD_ENCODE_CONFIG*)item_info[i].pConfigItem)->quality = (unsigned short)((ENCODE_INFO_EX*)pItemData)->quality;
						((DD_ENCODE_CONFIG*)item_info[i].pConfigItem)->minBitrate = (unsigned short)((ENCODE_INFO_EX*)pItemData)->lBitStream;
						((DD_ENCODE_CONFIG*)item_info[i].pConfigItem)->maxBitrate = (unsigned short)((ENCODE_INFO_EX*)pItemData)->hBitStream;
						
						iteminfolist.push_back(item_info[i]);
						pItemData += sizeof(ENCODE_INFO_EX);
					}
				}
				else
				{
					for (its = iteminfolist.begin(); its != iteminfolist.end(); its++)
					{
						if ((*its).itemID == DD_CONFIG_ITEM_ENCODE_NETWORK)
						{
							((DD_ENCODE_CONFIG*)(*its).pConfigItem)->resolution = VideoSizeToDDVideoSize(((ENCODE_INFO_EX*)pItemData)->resolution);
							((DD_ENCODE_CONFIG*)(*its).pConfigItem)->rate = (unsigned short)((ENCODE_INFO_EX*)pItemData)->rate;
							((DD_ENCODE_CONFIG*)(*its).pConfigItem)->encodeType = (unsigned short)((ENCODE_INFO_EX*)pItemData)->encodeType;
							((DD_ENCODE_CONFIG*)(*its).pConfigItem)->quality = (unsigned short)((ENCODE_INFO_EX*)pItemData)->quality;
							((DD_ENCODE_CONFIG*)(*its).pConfigItem)->minBitrate = (unsigned short)((ENCODE_INFO_EX*)pItemData)->lBitStream;
							((DD_ENCODE_CONFIG*)(*its).pConfigItem)->maxBitrate = (unsigned short)((ENCODE_INFO_EX*)pItemData)->hBitStream;
							pItemData += sizeof(ENCODE_INFO_EX);
						}
					}
				}
				break;
			}
		case NCFG_ITEM_JPEG_SIZE:
			{
				if (iteminfolist.empty())
				{
					ITEM_INFO item_info[DD_MAX_CAMERA_NUM];
					for (int i = 0; i < itemHead.num; i++)
					{
						item_info[i].itemID = DD_CONFIG_ITEM_ENCODE_JPEG;
						item_info[i].parserLen = sizeof(DD_ENCODE_CONFIG);
						item_info[i].pConfigItem = (unsigned char *)new DD_ENCODE_CONFIG;
						memset(item_info[i].pConfigItem, 0, sizeof(DD_ENCODE_CONFIG));
						((DD_ENCODE_CONFIG*)item_info[i].pConfigItem)->resolution = ToSDKResolution(*((unsigned long *)pItemData));
						iteminfolist.push_back(item_info[i]);
						pItemData += sizeof(unsigned long);
					}
				}
				else
				{
					for (its = iteminfolist.begin(); its != iteminfolist.end(); its++)
					{
						if ((*its).itemID == DD_CONFIG_ITEM_ENCODE_JPEG)
						{
							((DD_ENCODE_CONFIG*)(*its).pConfigItem)->resolution = ToSDKResolution(*((unsigned long *)pItemData));
							pItemData += sizeof(unsigned long);
						}
					}
				}
				break;
			}
		case NCFG_ITEM_REC_ENABLE:
			{
				if (iteminfolist.empty())
				{
					ITEM_INFO item_info[DD_MAX_CAMERA_NUM];
					for (int i = 0; i < itemHead.num; i++)
					{
						item_info[i].itemID = DD_CONFIG_ITEM_RECORD_SETUP;
						item_info[i].parserLen = sizeof(DD_RECORD_CONFIG);
						item_info[i].pConfigItem = (unsigned char *)new DD_RECORD_CONFIG;
						memset(item_info[i].pConfigItem, 0, sizeof(DD_RECORD_CONFIG));
						((DD_RECORD_CONFIG*)item_info[i].pConfigItem)->bOnlyVideo = *pItemData;

						pItemData += sizeof(unsigned long);
						iteminfolist.push_back(item_info[i]);
					}
				}
				else
				{
					for (its = iteminfolist.begin(); its != iteminfolist.end(); its++)
					{
						if ((*its).itemID == DD_CONFIG_ITEM_RECORD_SETUP)
						{
							((DD_RECORD_CONFIG*)(*its).pConfigItem)->bOnlyVideo = *pItemData;
							pItemData += sizeof(unsigned long);
						}
					}

				}
				break;
			}
		case NCFG_ITEM_PRE_ALARM_REC_TIME:
			{
				if (iteminfolist.empty())
				{
					ITEM_INFO item_info[DD_MAX_CAMERA_NUM];
					for (int i = 0; i < itemHead.num; i++)
					{
						item_info[i].itemID = DD_CONFIG_ITEM_RECORD_SETUP;
						item_info[i].parserLen = sizeof(DD_RECORD_CONFIG);
						item_info[i].pConfigItem = (unsigned char *)new DD_RECORD_CONFIG;
						memset(item_info[i].pConfigItem, 0, sizeof(DD_RECORD_CONFIG));
						((DD_RECORD_CONFIG*)item_info[i].pConfigItem)->preAlarmTime = *((unsigned short*)pItemData);
						pItemData += sizeof(unsigned long);
						iteminfolist.push_back(item_info[i]);
					}
				}
				else
				{
					for (its = iteminfolist.begin(); its != iteminfolist.end(); its++)
					{
						if ((*its).itemID == DD_CONFIG_ITEM_RECORD_SETUP)
						{
							((DD_RECORD_CONFIG*)(*its).pConfigItem)->preAlarmTime = *((unsigned short*)pItemData);
							pItemData += sizeof(unsigned long);
						}
					}
				}
				break;
			}
		case NCFG_ITEM_POST_ALARM_REC_TIME:
			{
				if (iteminfolist.empty())
				{
					ITEM_INFO item_info[DD_MAX_CAMERA_NUM];
					for (int i = 0; i < itemHead.num; i++)
					{
						item_info[i].itemID = DD_CONFIG_ITEM_RECORD_SETUP;
						item_info[i].parserLen = sizeof(DD_RECORD_CONFIG);
						item_info[i].pConfigItem = (unsigned char *)new DD_RECORD_CONFIG;
						memset(item_info[i].pConfigItem, 0, sizeof(DD_RECORD_CONFIG));
						((DD_RECORD_CONFIG*)item_info[i].pConfigItem)->postAlarmTime = *((unsigned short*)pItemData);
						pItemData += sizeof(unsigned long);
						iteminfolist.push_back(item_info[i]);
					}
				}
				else
				{
					for (its = iteminfolist.begin(); its != iteminfolist.end(); its++)
					{
						if ((*its).itemID == DD_CONFIG_ITEM_RECORD_SETUP)
						{
							((DD_RECORD_CONFIG*)(*its).pConfigItem)->postAlarmTime = *((unsigned short*)pItemData);
							pItemData += sizeof(unsigned long);
						}
					}
				}
				break;
			}
		case NCFG_ITEM_REC_HOLD_TIME:
			{
				if (iteminfolist.empty())
				{
					ITEM_INFO item_info[DD_MAX_CAMERA_NUM];
					for (int i = 0; i < itemHead.num; i++)
					{
						item_info[i].itemID = DD_CONFIG_ITEM_RECORD_SETUP;
						item_info[i].parserLen = sizeof(DD_RECORD_CONFIG);
						item_info[i].pConfigItem = (unsigned char *)new DD_RECORD_CONFIG;
						memset(item_info[i].pConfigItem, 0, sizeof(DD_RECORD_CONFIG));
						unsigned long test = *((unsigned long*)pItemData);
						((DD_RECORD_CONFIG*)item_info[i].pConfigItem)->expired = test / 86400;
						
						pItemData += sizeof(unsigned long);
						iteminfolist.push_back(item_info[i]);
					}
				}
				else
				{
					for (its = iteminfolist.begin(); its != iteminfolist.end(); its++)
					{
						if ((*its).itemID == DD_CONFIG_ITEM_RECORD_SETUP)
						{
							unsigned long test = *((unsigned long*)pItemData);
							((DD_RECORD_CONFIG*)(*its).pConfigItem)->expired = test / 86400;
							pItemData += sizeof(unsigned long);
						}
					}
				}
				break;
			}
		case NCFG_ITEM_REC_AUDIO:
			{
				if (iteminfolist.empty())
				{
					ITEM_INFO item_info[DD_MAX_CAMERA_NUM];
					for (int i = 0; i < itemHead.num; i++)
					{
						item_info[i].itemID = DD_CONFIG_ITEM_RECORD_SETUP;
						item_info[i].parserLen = sizeof(DD_RECORD_CONFIG);
						item_info[i].pConfigItem = (unsigned char *)new DD_RECORD_CONFIG;
						memset(item_info[i].pConfigItem, 0, sizeof(DD_RECORD_CONFIG));
						((DD_RECORD_CONFIG*)item_info[i].pConfigItem)->bWithAudio = *pItemData;
						
						pItemData += sizeof(unsigned long);
						iteminfolist.push_back(item_info[i]);
					}
				}
				else
				{
					for (its = iteminfolist.begin(); its != iteminfolist.end(); its++)
					{
						if ((*its).itemID == DD_CONFIG_ITEM_RECORD_SETUP)
						{
							((DD_RECORD_CONFIG*)(*its).pConfigItem)->bWithAudio = *pItemData;
							pItemData += sizeof(unsigned long);
						}
					}
				}
				break;
			}
		case NCFG_ITEM_REC_REDUNDANT:
			{
				if (iteminfolist.empty())
				{
					ITEM_INFO item_info[DD_MAX_CAMERA_NUM];
					for (int i = 0; i < itemHead.num; i++)
					{
						item_info[i].itemID = DD_CONFIG_ITEM_RECORD_SETUP;
						item_info[i].parserLen = sizeof(DD_RECORD_CONFIG);
						item_info[i].pConfigItem = (unsigned char *)new DD_RECORD_CONFIG;
						memset(item_info[i].pConfigItem, 0, sizeof(DD_RECORD_CONFIG));
						((DD_RECORD_CONFIG*)item_info[i].pConfigItem)->bRedundancy = *pItemData;
						
						pItemData += sizeof(unsigned long);
						iteminfolist.push_back(item_info[i]);
					}
				}
				else
				{
					for (its = iteminfolist.begin(); its != iteminfolist.end(); its++)
					{
						if ((*its).itemID == DD_CONFIG_ITEM_RECORD_SETUP)
						{
							((DD_RECORD_CONFIG*)(*its).pConfigItem)->bRedundancy = *pItemData;
							pItemData += sizeof(unsigned long);
						}
					}
				}
				break;
			}
		case NCFG_ITEM_REC_SCH_SCHEDULE_WEEK:
			{
				if (iteminfolist.empty())
				{
					ITEM_INFO item_info[DD_MAX_CAMERA_NUM];
					for (int i = 0; i < itemHead.num; i++)
					{
						item_info[i].itemID = DD_CONFIG_ITEM_RECORD_SCHEDULE_SCHEDULE;
						item_info[i].parserLen = sizeof(DD_WEEK_SCHEDULE);
						item_info[i].pConfigItem = (unsigned char *)new DD_WEEK_SCHEDULE;
						memset(item_info[i].pConfigItem, 0, sizeof(DD_WEEK_SCHEDULE));
						DD_DATE_SCHEDULE *pDD_Date = ((DD_WEEK_SCHEDULE*)item_info[i].pConfigItem)->week;
						DATE_SCHEDULE *pDate = ((WEEK_SCHEDULE*)pItemData)->week;
						for (int j = 0; j < 7; j++)
						{
							memcpy(pDD_Date->hour, pDate->hour, sizeof(unsigned long long) * 24);
							pDD_Date++;
							pDate++;
						}
						
						pItemData += sizeof(WEEK_SCHEDULE);
						iteminfolist.push_back(item_info[i]);
					}
				}
				else
				{
					for (its = iteminfolist.begin(); its != iteminfolist.end(); its++)
					{
						if ((*its).itemID == DD_CONFIG_ITEM_RECORD_SCHEDULE_SCHEDULE)
						{
							DD_DATE_SCHEDULE *pDD_Date = ((DD_WEEK_SCHEDULE*)(*its).pConfigItem)->week;
							DATE_SCHEDULE *pDate = ((WEEK_SCHEDULE*)pItemData)->week;
							for (int j = 0; j < 7; j++)
							{
								memcpy(pDD_Date->hour, pDate->hour, sizeof(unsigned long long) * 24);
								pDD_Date++;
								pDate++;
							}

							pItemData += sizeof(WEEK_SCHEDULE);
						}
					}
				}
				break;
			}
		case NCFG_ITEM_REC_SCH_MOTION_WEEK:
			{
				if (iteminfolist.empty())
				{
					ITEM_INFO item_info[DD_MAX_CAMERA_NUM];
					for (int i = 0; i < itemHead.num; i++)
					{
						item_info[i].itemID = DD_CONFIG_ITEM_RECORD_SCHEDULE_MOTION;
						item_info[i].parserLen = sizeof(DD_WEEK_SCHEDULE);
						item_info[i].pConfigItem = (unsigned char *)new DD_WEEK_SCHEDULE;
						memset(item_info[i].pConfigItem, 0, sizeof(DD_WEEK_SCHEDULE));
						DD_DATE_SCHEDULE *pDD_Date = ((DD_WEEK_SCHEDULE*)item_info[i].pConfigItem)->week;
						DATE_SCHEDULE *pDate = ((WEEK_SCHEDULE*)pItemData)->week;
						for (int j = 0; j < 7; j++)
						{
							memcpy(pDD_Date->hour, pDate->hour, sizeof(unsigned long long) * 24);
							pDD_Date++;
							pDate++;
						}

						pItemData += sizeof(WEEK_SCHEDULE);
						iteminfolist.push_back(item_info[i]);
					}
				}
				else
				{
					for (its = iteminfolist.begin(); its != iteminfolist.end(); its++)
					{
						if ((*its).itemID == DD_CONFIG_ITEM_RECORD_SCHEDULE_MOTION)
						{
							DD_DATE_SCHEDULE *pDD_Date = ((DD_WEEK_SCHEDULE*)(*its).pConfigItem)->week;
							DATE_SCHEDULE *pDate = ((WEEK_SCHEDULE*)pItemData)->week;
							for (int j = 0; j < 7; j++)
							{
								memcpy(pDD_Date->hour, pDate->hour, sizeof(unsigned long long) * 24);
								pDD_Date++;
								pDate++;
							}

							pItemData += sizeof(WEEK_SCHEDULE);
						}
					}
				}
				break;
			}
		case NCFG_ITEM_REC_SCH_SENSOR_WEEK:
			{
				if (iteminfolist.empty())
				{
					ITEM_INFO item_info[DD_MAX_CAMERA_NUM];
					for (int i = 0; i < itemHead.num; i++)
					{
						item_info[i].itemID = DD_CONFIG_ITEM_RECORD_SCHEDULE_SENSOR;
						item_info[i].parserLen = sizeof(DD_WEEK_SCHEDULE);
						item_info[i].pConfigItem = (unsigned char *)new DD_WEEK_SCHEDULE;
						memset(item_info[i].pConfigItem, 0, sizeof(DD_WEEK_SCHEDULE));
						DD_DATE_SCHEDULE *pDD_Date = ((DD_WEEK_SCHEDULE*)item_info[i].pConfigItem)->week;
						DATE_SCHEDULE *pDate = ((WEEK_SCHEDULE*)pItemData)->week;
						for (int j = 0; j < 7; j++)
						{
							memcpy(pDD_Date->hour, pDate->hour, sizeof(unsigned long long) * 24);
							pDD_Date++;
							pDate++;
						}

						pItemData += sizeof(WEEK_SCHEDULE);
						iteminfolist.push_back(item_info[i]);
					}
				}
				else
				{
					for (its = iteminfolist.begin(); its != iteminfolist.end(); its++)
					{
						if ((*its).itemID == DD_CONFIG_ITEM_RECORD_SCHEDULE_MOTION)
						{
							DD_DATE_SCHEDULE *pDD_Date = ((DD_WEEK_SCHEDULE*)(*its).pConfigItem)->week;
							DATE_SCHEDULE *pDate = ((WEEK_SCHEDULE*)pItemData)->week;
							for (int j = 0; j < 7; j++)
							{
								memcpy(pDD_Date->hour, pDate->hour, sizeof(unsigned long long) * 24);
								pDD_Date++;
								pDate++;
							}

							pItemData += sizeof(WEEK_SCHEDULE);
						}
					}
				}
				break;
			}
		case NCFG_ITEM_NETWORK_ADDR_INFO:
			{
				if (iteminfolist.empty())
				{
					ITEM_INFO item_info;
					item_info.itemID = DD_CONFIG_ITEM_NETWORK_IP;
					item_info.parserLen = sizeof(DD_NETWORK_IP_CONFIG);
					item_info.pConfigItem = (unsigned char *)new DD_NETWORK_IP_CONFIG;
					memset(item_info.pConfigItem, 0, sizeof(DD_NETWORK_IP_CONFIG));
					((DD_NETWORK_IP_CONFIG*)item_info.pConfigItem)->useDHCP = ((NETWORK_ADDR_INFO*)pItemData)->bDHCP;
					((DD_NETWORK_IP_CONFIG*)item_info.pConfigItem)->IP = ((NETWORK_ADDR_INFO*)pItemData)->staticIP;
					((DD_NETWORK_IP_CONFIG*)item_info.pConfigItem)->subnetMask = ((NETWORK_ADDR_INFO*)pItemData)->netMask;
					((DD_NETWORK_IP_CONFIG*)item_info.pConfigItem)->gateway = ((NETWORK_ADDR_INFO*)pItemData)->gateway;
					
					iteminfolist.push_back(item_info);
				}
				else
				{
					for (its = iteminfolist.begin(); its != iteminfolist.end(); its++)
					{
						if ((*its).itemID == DD_CONFIG_ITEM_NETWORK_IP)
						{
							((DD_NETWORK_IP_CONFIG*)(*its).pConfigItem)->useDHCP = ((NETWORK_ADDR_INFO*)pItemData)->bDHCP;
							((DD_NETWORK_IP_CONFIG*)(*its).pConfigItem)->IP = ((NETWORK_ADDR_INFO*)pItemData)->staticIP;
							((DD_NETWORK_IP_CONFIG*)(*its).pConfigItem)->subnetMask = ((NETWORK_ADDR_INFO*)pItemData)->netMask;
							((DD_NETWORK_IP_CONFIG*)(*its).pConfigItem)->gateway = ((NETWORK_ADDR_INFO*)pItemData)->gateway;
							break;
						}
					}
				}
				break;
			}
		case NCFG_ITEM_DNS_1:
			{
				if (iteminfolist.empty())
				{
					ITEM_INFO item_info;
					item_info.itemID = DD_CONFIG_ITEM_NETWORK_IP;
					item_info.parserLen = sizeof(DD_NETWORK_IP_CONFIG);
					item_info.pConfigItem = (unsigned char *)new DD_NETWORK_IP_CONFIG;
					memset(item_info.pConfigItem, 0, sizeof(DD_NETWORK_IP_CONFIG));
					((DD_NETWORK_IP_CONFIG*)item_info.pConfigItem)->preferredDNS = *((unsigned long*)pItemData);

					iteminfolist.push_back(item_info);
				}
				else
				{
					for (its = iteminfolist.begin(); its != iteminfolist.end(); its++)
					{
						if ((*its).itemID == DD_CONFIG_ITEM_NETWORK_IP)
						{
							((DD_NETWORK_IP_CONFIG*)(*its).pConfigItem)->preferredDNS = *((unsigned long*)pItemData);
							break;
						}
					}
				}
				break;
			}
		case NCFG_ITEM_DNS_2:
			{
				if (iteminfolist.empty())
				{
					ITEM_INFO item_info;
					item_info.itemID = DD_CONFIG_ITEM_NETWORK_IP;
					item_info.parserLen = sizeof(DD_NETWORK_IP_CONFIG);
					item_info.pConfigItem = (unsigned char *)new DD_NETWORK_IP_CONFIG;
					memset(item_info.pConfigItem, 0, sizeof(DD_NETWORK_IP_CONFIG));
					((DD_NETWORK_IP_CONFIG*)item_info.pConfigItem)->alternateDNS = *((unsigned long*)pItemData);

					iteminfolist.push_back(item_info);
				}
				else
				{
					for (its = iteminfolist.begin(); its != iteminfolist.end(); its++)
					{
						if ((*its).itemID == DD_CONFIG_ITEM_NETWORK_IP)
						{
							((DD_NETWORK_IP_CONFIG*)(*its).pConfigItem)->alternateDNS = *((unsigned long*)pItemData);
							break;
						}
					}
				}
				break;
			}
		case NCFG_ITEM_PPPOE_INFO:
			{
				if (iteminfolist.empty())
				{
					ITEM_INFO item_info;
					item_info.itemID = DD_CONFIG_ITEM_NETWORK_IP;
					item_info.parserLen = sizeof(DD_NETWORK_IP_CONFIG);
					item_info.pConfigItem = (unsigned char *)new DD_NETWORK_IP_CONFIG;
					memset(item_info.pConfigItem, 0, sizeof(DD_NETWORK_IP_CONFIG));
					((DD_NETWORK_IP_CONFIG*)item_info.pConfigItem)->usePPPoE = ((PPPOE_INFO*)pItemData)->bEnable;
					memcpy(((DD_NETWORK_IP_CONFIG*)item_info.pConfigItem)->account, ((PPPOE_INFO*)pItemData)->user, sizeof(((PPPOE_INFO*)pItemData)->user));
					memcpy(((DD_NETWORK_IP_CONFIG*)item_info.pConfigItem)->password, ((PPPOE_INFO*)pItemData)->password, sizeof(((PPPOE_INFO*)pItemData)->password));

					iteminfolist.push_back(item_info);
				}
				else
				{
					for (its = iteminfolist.begin(); its != iteminfolist.end(); its++)
					{
						if ((*its).itemID == DD_CONFIG_ITEM_NETWORK_IP)
						{
							((DD_NETWORK_IP_CONFIG*)(*its).pConfigItem)->usePPPoE = ((PPPOE_INFO*)pItemData)->bEnable;
							memcpy(((DD_NETWORK_IP_CONFIG*)(*its).pConfigItem)->account, ((PPPOE_INFO*)pItemData)->user, sizeof(((PPPOE_INFO*)pItemData)->user));
							memcpy(((DD_NETWORK_IP_CONFIG*)(*its).pConfigItem)->password, ((PPPOE_INFO*)pItemData)->password, sizeof(((PPPOE_INFO*)pItemData)->password));
							break;
						}
					}
				}
				break;
			}
		case NCFG_ITEM_MAX_CLIENT_NUM:
			{
				if (iteminfolist.empty())
				{
					ITEM_INFO item_info;
					item_info.itemID = DD_CONFIG_ITEM_NETWORK_ADVANCE;
					item_info.parserLen = sizeof(DD_NETWORK_ADVANCE_CONFIG);
					item_info.pConfigItem = (unsigned char *)new DD_NETWORK_ADVANCE_CONFIG;
					memset(item_info.pConfigItem, 0, sizeof(DD_NETWORK_ADVANCE_CONFIG));
					((DD_NETWORK_ADVANCE_CONFIG*)item_info.pConfigItem)->OnlineUserNum = *((unsigned short*)pItemData);

					iteminfolist.push_back(item_info);
				}
				else
				{
					for (its = iteminfolist.begin(); its != iteminfolist.end(); its++)
					{
						if ((*its).itemID == DD_CONFIG_ITEM_NETWORK_ADVANCE)
						{
							((DD_NETWORK_ADVANCE_CONFIG*)(*its).pConfigItem)->OnlineUserNum = *((unsigned short*)pItemData);
							break;
						}
					}
				}
				break;
			}
		case NCFG_ITEM_HTTP_PORT:
			{
				if (iteminfolist.empty())
				{
					ITEM_INFO item_info;
					item_info.itemID = DD_CONFIG_ITEM_NETWORK_ADVANCE;
					item_info.parserLen = sizeof(DD_NETWORK_ADVANCE_CONFIG);
					item_info.pConfigItem = (unsigned char *)new DD_NETWORK_ADVANCE_CONFIG;
					memset(item_info.pConfigItem, 0, sizeof(DD_NETWORK_ADVANCE_CONFIG));
					((DD_NETWORK_ADVANCE_CONFIG*)item_info.pConfigItem)->httpPort = *((unsigned short*)pItemData);

					iteminfolist.push_back(item_info);
				}
				else
				{
					for (its = iteminfolist.begin(); its != iteminfolist.end(); its++)
					{
						if ((*its).itemID == DD_CONFIG_ITEM_NETWORK_ADVANCE)
						{
							((DD_NETWORK_ADVANCE_CONFIG*)(*its).pConfigItem)->httpPort = *((unsigned short*)pItemData);
							break;
						}
					}
				}
				break;
			}

		case NCFG_ITEM_SERVER_PORT:
			{
				if (iteminfolist.empty())
				{
					ITEM_INFO item_info;
					item_info.itemID = DD_CONFIG_ITEM_NETWORK_ADVANCE;
					item_info.parserLen = sizeof(DD_NETWORK_ADVANCE_CONFIG);
					item_info.pConfigItem = (unsigned char *)new DD_NETWORK_ADVANCE_CONFIG;
					memset(item_info.pConfigItem, 0, sizeof(DD_NETWORK_ADVANCE_CONFIG));
					((DD_NETWORK_ADVANCE_CONFIG*)item_info.pConfigItem)->datePort = *((unsigned short*)pItemData);
					((DD_NETWORK_ADVANCE_CONFIG*)item_info.pConfigItem)->messagePort = *((unsigned short*)pItemData);
					((DD_NETWORK_ADVANCE_CONFIG*)item_info.pConfigItem)->alarmPort = *((unsigned short*)pItemData);

					iteminfolist.push_back(item_info);
				}
				else
				{
					for (its = iteminfolist.begin(); its != iteminfolist.end(); its++)
					{
						if ((*its).itemID == DD_CONFIG_ITEM_NETWORK_ADVANCE)
						{
							((DD_NETWORK_ADVANCE_CONFIG*)(*its).pConfigItem)->datePort = *((unsigned short*)pItemData);
							((DD_NETWORK_ADVANCE_CONFIG*)(*its).pConfigItem)->messagePort = *((unsigned short*)pItemData);
							((DD_NETWORK_ADVANCE_CONFIG*)(*its).pConfigItem)->alarmPort = *((unsigned short*)pItemData);
							break;
						}
					}
				}
				break;
			}

		case NCFG_ITEM_MULTI_NET_ADDR:
			{
				if (iteminfolist.empty())
				{
					ITEM_INFO item_info;
					item_info.itemID = DD_CONFIG_ITEM_NETWORK_ADVANCE;
					item_info.parserLen = sizeof(DD_NETWORK_ADVANCE_CONFIG);
					item_info.pConfigItem = (unsigned char *)new DD_NETWORK_ADVANCE_CONFIG;
					memset(item_info.pConfigItem, 0, sizeof(DD_NETWORK_ADVANCE_CONFIG));
					((DD_NETWORK_ADVANCE_CONFIG*)item_info.pConfigItem)->multiCastIP = *((unsigned long*)pItemData);

					iteminfolist.push_back(item_info);
				}
				else
				{
					for (its = iteminfolist.begin(); its != iteminfolist.end(); its++)
					{
						if ((*its).itemID == DD_CONFIG_ITEM_NETWORK_ADVANCE)
						{
							((DD_NETWORK_ADVANCE_CONFIG*)(*its).pConfigItem)->multiCastIP = *((unsigned long*)pItemData);
							break;
						}
					}
				}
				break;
			}
		case NCFG_ITEM_DDNS_SERVER_INFO:
			{
				if (iteminfolist.empty())
				{
					ITEM_INFO item_info[DD_MAX_CAMERA_NUM];
					for (int i = 0; i < itemHead.len / sizeof(DD_DDNS_SERVER_INFO); i++)
					{	
						item_info[i].itemID = DD_CONFIG_ITEM_DDNS_SERVER_INFO;
						item_info[i].parserLen = sizeof(DD_DDNS_SERVER_INFO);
						item_info[i].pConfigItem = (unsigned char *)new DD_DDNS_SERVER_INFO;
						memset(item_info[i].pConfigItem, 0, sizeof(DD_DDNS_SERVER_INFO));
						memcpy((DD_NETWORK_ADVANCE_CONFIG*)item_info[i].pConfigItem, (DD_NETWORK_ADVANCE_CONFIG*)pItemData, sizeof(DD_NETWORK_ADVANCE_CONFIG));

						iteminfolist.push_back(item_info[i]);
						pItemData += sizeof(DD_DDNS_SERVER_INFO);
					}
				}
				else
				{
					for (its = iteminfolist.begin(); its != iteminfolist.end(); its++)
					{
						if ((*its).itemID == DD_CONFIG_ITEM_DDNS_SERVER_INFO)
						{
							memcpy((DD_NETWORK_ADVANCE_CONFIG*)(*its).pConfigItem, (DD_NETWORK_ADVANCE_CONFIG*)pItemData, sizeof(DD_NETWORK_ADVANCE_CONFIG));

							pItemData += sizeof(DD_DDNS_SERVER_INFO);
						}
					}
				}
				break;
			}
		case NCFG_ITEM_PTZ_SUPPORT_INFO:
			{
				if (iteminfolist.empty())
				{
					ITEM_INFO item_info[DD_MAX_CAMERA_NUM];
					for (int i = 0; i < itemHead.len / sizeof(DD_PTZ_PROTOCOL_INFO); i++)
					{	
						item_info[i].itemID = DD_CONFIG_ITEM_PTZ_PROTOCOL_INFO;
						item_info[i].parserLen = sizeof(DD_PTZ_PROTOCOL_INFO);
						item_info[i].pConfigItem = (unsigned char *)new DD_PTZ_PROTOCOL_INFO;
						memset(item_info[i].pConfigItem, 0, sizeof(DD_PTZ_PROTOCOL_INFO));
						memcpy((DD_PTZ_PROTOCOL_INFO*)item_info[i].pConfigItem, (DD_PTZ_PROTOCOL_INFO*)pItemData, sizeof(DD_PTZ_PROTOCOL_INFO));

						iteminfolist.push_back(item_info[i]);
						pItemData += sizeof(DD_PTZ_PROTOCOL_INFO);
					}
				}
				else
				{
					for (its = iteminfolist.begin(); its != iteminfolist.end(); its++)
					{
						if ((*its).itemID == DD_CONFIG_ITEM_PTZ_PROTOCOL_INFO)
						{
							memcpy((DD_PTZ_PROTOCOL_INFO*)(*its).pConfigItem, (DD_PTZ_PROTOCOL_INFO*)pItemData, sizeof(DD_PTZ_PROTOCOL_INFO));

							pItemData += sizeof(DD_PTZ_PROTOCOL_INFO);
						}
					}
				}
				break;
			}
		case NCFG_ITEM_SUPPORT_MAXCLIENTNUM:
			{
				if (iteminfolist.empty())
				{
					ITEM_INFO item_info;
					item_info.itemID = DD_CONFIG_ITEM_NETWORK_ADVANCE;
					item_info.parserLen = sizeof(DD_NETWORK_ADVANCE_CONFIG);
					item_info.pConfigItem = (unsigned char *)new DD_NETWORK_ADVANCE_CONFIG;
					memset(item_info.pConfigItem, 0, sizeof(DD_NETWORK_ADVANCE_CONFIG));
					((DD_NETWORK_ADVANCE_CONFIG*)item_info.pConfigItem)->maxOnlineUserNum = *((unsigned short*)pItemData);

					iteminfolist.push_back(item_info);
				}
				else
				{
					for (its = iteminfolist.begin(); its != iteminfolist.end(); its++)
					{
						if ((*its).itemID == DD_CONFIG_ITEM_NETWORK_ADVANCE)
						{
							((DD_NETWORK_ADVANCE_CONFIG*)(*its).pConfigItem)->maxOnlineUserNum = *((unsigned short*)pItemData);
							break;
						}
					}
				}
				break;
			}
		case NCFG_ITEM_DDNS_INFO:
			{
				if (iteminfolist.empty())
				{
					ITEM_INFO item_info;
					item_info.itemID = DD_CONFIG_ITEM_NETWORK_DDNS;
					item_info.parserLen = sizeof(DD_DDNS_CONFIG);
					item_info.pConfigItem = (unsigned char *)new DD_DDNS_CONFIG;
					memset(item_info.pConfigItem, 0, sizeof(DD_DDNS_CONFIG));
					((DD_DDNS_CONFIG*)item_info.pConfigItem)->enable = (unsigned short)((DDNS_INFO*)pItemData)->bEnable;
					memcpy(((DD_DDNS_CONFIG*)item_info.pConfigItem)->userName, ((DDNS_INFO*)pItemData)->user, sizeof(((DDNS_INFO*)pItemData)->user));
					memcpy(((DD_DDNS_CONFIG*)item_info.pConfigItem)->password, ((DDNS_INFO*)pItemData)->password, sizeof(((DDNS_INFO*)pItemData)->password));
					memcpy(((DD_DDNS_CONFIG*)item_info.pConfigItem)->hostDomain, ((DDNS_INFO*)pItemData)->domain, sizeof(((DDNS_INFO*)pItemData)->domain));
					((DD_DDNS_CONFIG*)item_info.pConfigItem)->userHostDomain = (unsigned long)((DDNS_INFO*)pItemData)->bUseDomain;
					((DD_DDNS_CONFIG*)item_info.pConfigItem)->useDDNSServer = (unsigned long)((DDNS_INFO*)pItemData)->type;
					iteminfolist.push_back(item_info);
				}
				else
				{
					for (its = iteminfolist.begin(); its != iteminfolist.end(); its++)
					{
						if ((*its).itemID == DD_CONFIG_ITEM_NETWORK_DDNS)
						{
							((DD_DDNS_CONFIG*)(*its).pConfigItem)->enable = (unsigned short)((DDNS_INFO*)pItemData)->bEnable;
							memcpy(((DD_DDNS_CONFIG*)(*its).pConfigItem)->userName, ((DDNS_INFO*)pItemData)->user, sizeof(((DDNS_INFO*)pItemData)->user));
							memcpy(((DD_DDNS_CONFIG*)(*its).pConfigItem)->password, ((DDNS_INFO*)pItemData)->password, sizeof(((DDNS_INFO*)pItemData)->password));
							memcpy(((DD_DDNS_CONFIG*)(*its).pConfigItem)->hostDomain, ((DDNS_INFO*)pItemData)->domain, sizeof(((DDNS_INFO*)pItemData)->domain));
							((DD_DDNS_CONFIG*)(*its).pConfigItem)->userHostDomain = (unsigned long)((DDNS_INFO*)pItemData)->bUseDomain;
							break;
						}
					}
				}
				break;
			}
		case NCFG_ITEM_DDNS_UPDATE_CYCLE:
			{
				if (iteminfolist.empty())
				{
					ITEM_INFO item_info;
					item_info.itemID = DD_CONFIG_ITEM_NETWORK_DDNS;
					item_info.parserLen = sizeof(DD_DDNS_CONFIG);
					item_info.pConfigItem = (unsigned char *)new DD_DDNS_CONFIG;
					memset(item_info.pConfigItem, 0, sizeof(DD_DDNS_CONFIG));
					((DD_DDNS_CONFIG*)item_info.pConfigItem)->interval = *((unsigned short*)pItemData);

					iteminfolist.push_back(item_info);
				}
				else
				{
					for (its = iteminfolist.begin(); its != iteminfolist.end(); its++)
					{
						if ((*its).itemID == DD_CONFIG_ITEM_NETWORK_DDNS)
						{
							((DD_DDNS_CONFIG*)(*its).pConfigItem)->interval = *((unsigned short*)pItemData);
							break;
						}
					}
				}
				break;
			}
		case NCFG_ITEM_EMAIL_SEND_INFO:
			{
				if (iteminfolist.empty())
				{
					ITEM_INFO item_info;
					item_info.itemID = DD_CONFIG_ITEM_NETWORK_SMTP;
					item_info.parserLen = sizeof(DD_SMTP_CONFIG);
					item_info.pConfigItem = (unsigned char *)new DD_SMTP_CONFIG;
					memset(item_info.pConfigItem, 0, sizeof(DD_SMTP_CONFIG));
					((DD_SMTP_CONFIG*)item_info.pConfigItem)->port = (unsigned short)((MAIL_SEND_INFO*)pItemData)->port;
					strcpy(((DD_SMTP_CONFIG*)item_info.pConfigItem)->server, ((MAIL_SEND_INFO*)pItemData)->server);
					strcpy(((DD_SMTP_CONFIG*)item_info.pConfigItem)->password, ((MAIL_SEND_INFO*)pItemData)->passwd);
					strcpy(((DD_SMTP_CONFIG*)item_info.pConfigItem)->sendAddress, ((MAIL_SEND_INFO*)pItemData)->name);
					((DD_SMTP_CONFIG*)item_info.pConfigItem)->enableSSL = (unsigned short)((MAIL_SEND_INFO*)pItemData)->bSSL;
					iteminfolist.push_back(item_info);
				}
				else
				{
					for (its = iteminfolist.begin(); its != iteminfolist.end(); its++)
					{
						if ((*its).itemID == DD_CONFIG_ITEM_NETWORK_SMTP)
						{
							((DD_SMTP_CONFIG*)(*its).pConfigItem)->port = (unsigned short)((MAIL_SEND_INFO*)pItemData)->port;
							strcpy(((DD_SMTP_CONFIG*)(*its).pConfigItem)->server, ((MAIL_SEND_INFO*)pItemData)->server);
							strcpy(((DD_SMTP_CONFIG*)(*its).pConfigItem)->password, ((MAIL_SEND_INFO*)pItemData)->passwd);
							strcpy(((DD_SMTP_CONFIG*)(*its).pConfigItem)->sendAddress, ((MAIL_SEND_INFO*)pItemData)->name);
							((DD_SMTP_CONFIG*)(*its).pConfigItem)->enableSSL = (unsigned short)((MAIL_SEND_INFO*)pItemData)->bSSL;
							break;
						}
					}
				}
				break;
			}
		case NCFG_ITEM_EMAIL_RECV_INFO:
			{
				int buffSize = 68;
				if (iteminfolist.empty())
				{
					ITEM_INFO item_info;
					item_info.itemID = DD_CONFIG_ITEM_NETWORK_SMTP;
					item_info.parserLen = sizeof(DD_SMTP_CONFIG);
					item_info.pConfigItem = (unsigned char *)new DD_SMTP_CONFIG;
					memset(item_info.pConfigItem, 0, sizeof(DD_SMTP_CONFIG));
					memcpy(((DD_SMTP_CONFIG*)item_info.pConfigItem)->receiveAddress[0],\
						((MAIL_RECV_INFO*)pItemData)->recvname, buffSize * sizeof(char));
					memcpy(((DD_SMTP_CONFIG*)item_info.pConfigItem)->receiveAddress[1], \
						((MAIL_RECV_INFO*)pItemData)->recvname + buffSize * sizeof(char), buffSize * sizeof(char));
					memcpy(((DD_SMTP_CONFIG*)item_info.pConfigItem)->receiveAddress[2], \
						((MAIL_RECV_INFO*)pItemData)->recvname + 2 * buffSize * sizeof(char), buffSize * sizeof(char));

					/*for (int i = 0; i < 3; i++)
					{
						if (strlen(((DD_SMTP_CONFIG*)item_info.pConfigItem)->receiveAddress[i]))
						{
							((DD_SMTP_CONFIG*)item_info.pConfigItem)->enableRecvAddrNum = i + 1;
						}
					}*/
					((DD_SMTP_CONFIG*)item_info.pConfigItem)->enableRecvAddrNum = 3;
					iteminfolist.push_back(item_info);
				}
				else
				{
					for (its = iteminfolist.begin(); its != iteminfolist.end(); its++)
					{
						if ((*its).itemID == DD_CONFIG_ITEM_NETWORK_SMTP)
						{
							memcpy(((DD_SMTP_CONFIG*)(*its).pConfigItem)->receiveAddress[0],\
								((MAIL_RECV_INFO*)pItemData)->recvname, buffSize * sizeof(char));
							memcpy(((DD_SMTP_CONFIG*)(*its).pConfigItem)->receiveAddress[1], \
								((MAIL_RECV_INFO*)pItemData)->recvname + buffSize * sizeof(char), buffSize * sizeof(char));
							memcpy(((DD_SMTP_CONFIG*)(*its).pConfigItem)->receiveAddress[2], \
								((MAIL_RECV_INFO*)pItemData)->recvname + 2 * buffSize * sizeof(char), buffSize * sizeof(char));

							/*for (int i = 0; i < 3; i++)
							{
								if (strlen(((DD_SMTP_CONFIG*)(*its).pConfigItem)->receiveAddress[i]))
								{
									((DD_SMTP_CONFIG*)(*its).pConfigItem)->enableRecvAddrNum = i + 1;
								}
							}*/
							((DD_SMTP_CONFIG*)(*its).pConfigItem)->enableRecvAddrNum = 3;
							break;
						}
					}

				}
			break;
			}
		case NCFG_ITEM_USER:
			{
				if (iteminfolist.empty())
				{
					ITEM_INFO item_info;
					for (int i = 0; i < itemHead.num; i++)
					{
						item_info.itemID = DD_CONFIG_ITEM_ACCOUNT;
						item_info.parserLen = sizeof(DD_ACCOUNT_CONFIG);
						item_info.pConfigItem = (unsigned char *)new DD_ACCOUNT_CONFIG;
						memset(item_info.pConfigItem, 0, sizeof(DD_ACCOUNT_CONFIG));
						((DD_ACCOUNT_CONFIG*)item_info.pConfigItem)->enable = true;
						memcpy(((DD_ACCOUNT_CONFIG*)item_info.pConfigItem)->MAC, ((NCFG_INFO_USER*)pItemData)->MAC, sizeof(unsigned char) * 8);
						strcpy(((DD_ACCOUNT_CONFIG*)item_info.pConfigItem)->name, ((NCFG_INFO_USER*)pItemData)->name);
						strcpy(((DD_ACCOUNT_CONFIG*)item_info.pConfigItem)->password, ((NCFG_INFO_USER*)pItemData)->password);
						((DD_ACCOUNT_CONFIG*)item_info.pConfigItem)->bingMAC = ((NCFG_INFO_USER*)pItemData)->bindMac;
						((DD_ACCOUNT_CONFIG*)item_info.pConfigItem)->group = ((NCFG_INFO_USER*)pItemData)->group;

	#define GET_BIT(x, y) ((x >> (y - 1)) & 0x1)
						
						DD_ACCOUNT_CONFIG * ac = ((DD_ACCOUNT_CONFIG*)item_info.pConfigItem);
						unsigned long temp = ((NCFG_INFO_USER*)pItemData)->authority;
						
						
						ac->authLive = GET_BIT(temp, 2);
						ac->authRecord = GET_BIT(temp, 3);
						ac->authPlayback = GET_BIT(temp, 4);
						ac->authBackup = GET_BIT(temp, 5);
						ac->authPTZ = GET_BIT(temp, 6);
						ac->netAuthView = GET_BIT(temp, 7);
						ac->logSearch = GET_BIT(temp, 8);
						ac->systemSetup = GET_BIT(temp, 9);
						ac->shutdown = GET_BIT(temp, 10);
						ac->twoWayAudio = GET_BIT(temp, 11);
						ac->fileManagement = GET_BIT(temp, 12);
						ac->diskManagement = GET_BIT(temp, 13);
						ac->remoteLogin = GET_BIT(temp, 14);
						ac->OnlineUserManagement = GET_BIT(temp, 15);
						
						memcpy((((DD_ACCOUNT_CONFIG*)item_info.pConfigItem)->authLiveCH), &((NCFG_INFO_USER*)pItemData)->authLiveCH, sizeof(ULONGLONG));
						memcpy((((DD_ACCOUNT_CONFIG*)item_info.pConfigItem)->authRecordCH), &((NCFG_INFO_USER*)pItemData)->authRecordCH, sizeof(ULONGLONG));
						memcpy((((DD_ACCOUNT_CONFIG*)item_info.pConfigItem)->authPlaybackCH), &((NCFG_INFO_USER*)pItemData)->authPlaybackCH, sizeof(ULONGLONG));
						memcpy((((DD_ACCOUNT_CONFIG*)item_info.pConfigItem)->authBackupCH), &((NCFG_INFO_USER*)pItemData)->authBackupCH, sizeof(ULONGLONG));
						memcpy((((DD_ACCOUNT_CONFIG*)item_info.pConfigItem)->authPTZCH), &((NCFG_INFO_USER*)pItemData)->authPTZCtrlCH, sizeof(ULONGLONG));
						memcpy((((DD_ACCOUNT_CONFIG*)item_info.pConfigItem)->netAuthViewCH), &((NCFG_INFO_USER*)pItemData)->authRemoteViewCH, sizeof(ULONGLONG));

						pItemData += sizeof(NCFG_INFO_USER);
						iteminfolist.push_back(item_info);
					}
				}
				else
				{
					for (its = iteminfolist.begin(); its != iteminfolist.end(); its++)
					{
						if ((*its).itemID == DD_CONFIG_ITEM_ACCOUNT)
						{
							((DD_ACCOUNT_CONFIG*)(*its).pConfigItem)->enable = true;
							memcpy(((DD_ACCOUNT_CONFIG*)(*its).pConfigItem)->MAC, ((NCFG_INFO_USER*)pItemData)->MAC, sizeof(unsigned char) * 8);
							strcpy(((DD_ACCOUNT_CONFIG*)(*its).pConfigItem)->name, ((NCFG_INFO_USER*)pItemData)->name);
							strcpy(((DD_ACCOUNT_CONFIG*)(*its).pConfigItem)->password, ((NCFG_INFO_USER*)pItemData)->password);
							((DD_ACCOUNT_CONFIG*)(*its).pConfigItem)->bingMAC = ((NCFG_INFO_USER*)pItemData)->bindMac;
							((DD_ACCOUNT_CONFIG*)(*its).pConfigItem)->group = ((NCFG_INFO_USER*)pItemData)->group;

#define GET_BIT(x, y) ((x >> (y - 1)) & 0x1)

							DD_ACCOUNT_CONFIG * ac = ((DD_ACCOUNT_CONFIG*)(*its).pConfigItem);
							unsigned long temp = ((NCFG_INFO_USER*)pItemData)->authority;


							ac->authLive = GET_BIT(temp, 2);
							ac->authRecord = GET_BIT(temp, 3);
							ac->authPlayback = GET_BIT(temp, 4);
							ac->authBackup = GET_BIT(temp, 5);
							ac->authPTZ = GET_BIT(temp, 6);
							ac->netAuthView = GET_BIT(temp, 7);
							ac->logSearch = GET_BIT(temp, 8);
							ac->systemSetup = GET_BIT(temp, 9);
							ac->shutdown = GET_BIT(temp, 10);
							ac->twoWayAudio = GET_BIT(temp, 11);
							ac->fileManagement = GET_BIT(temp, 12);
							ac->diskManagement = GET_BIT(temp, 13);
							ac->remoteLogin = GET_BIT(temp, 14);
							ac->OnlineUserManagement = GET_BIT(temp, 15);

							memcpy((((DD_ACCOUNT_CONFIG*)(*its).pConfigItem)->authLiveCH), &((NCFG_INFO_USER*)pItemData)->authLiveCH, sizeof(ULONGLONG));
							memcpy((((DD_ACCOUNT_CONFIG*)(*its).pConfigItem)->authRecordCH), &((NCFG_INFO_USER*)pItemData)->authRecordCH, sizeof(ULONGLONG));
							memcpy((((DD_ACCOUNT_CONFIG*)(*its).pConfigItem)->authPlaybackCH), &((NCFG_INFO_USER*)pItemData)->authPlaybackCH, sizeof(ULONGLONG));
							memcpy((((DD_ACCOUNT_CONFIG*)(*its).pConfigItem)->authBackupCH), &((NCFG_INFO_USER*)pItemData)->authBackupCH, sizeof(ULONGLONG));
							memcpy((((DD_ACCOUNT_CONFIG*)(*its).pConfigItem)->authPTZCH), &((NCFG_INFO_USER*)pItemData)->authPTZCtrlCH, sizeof(ULONGLONG));
							memcpy((((DD_ACCOUNT_CONFIG*)(*its).pConfigItem)->netAuthViewCH), &((NCFG_INFO_USER*)pItemData)->authRemoteViewCH, sizeof(ULONGLONG));

							pItemData += sizeof(NCFG_INFO_USER);
						}
					}
				}
				break;
			}
		case NCFG_ITEM_SENSOR_IN_ENABLE:
			{
				if (iteminfolist.empty())
				{
					ITEM_INFO item_info;

					pItemData += m_sensorInNum * sizeof(unsigned long);
					for (int i = m_sensorInNum; i < itemHead.num; i++)
					{
						item_info.itemID = DD_CONFIG_ITEM_SENSOR_SETUP;
						item_info.parserLen = sizeof(DD_SENSOR_CONFIG);
						item_info.pConfigItem = (unsigned char *)new DD_SENSOR_CONFIG;
						memset(item_info.pConfigItem, 0, sizeof(DD_SENSOR_CONFIG));
						((DD_SENSOR_CONFIG*)item_info.pConfigItem)->enable = *pItemData;

						iteminfolist.push_back(item_info);
						pItemData += sizeof(unsigned long);
					}
				}
				else
				{
					pItemData += m_sensorInNum * sizeof(unsigned long);
					for (its = iteminfolist.begin(); its != iteminfolist.end(); its++)
					{
						if ((*its).itemID == DD_CONFIG_ITEM_SENSOR_SETUP)
						{
							((DD_SENSOR_CONFIG*)(*its).pConfigItem)->enable = *pItemData;
							pItemData += sizeof(unsigned long);
						}
					}
				}
				break;
			}
		case NCFG_ITEM_SENSOR_IN_TYPE:
			{
				if (iteminfolist.empty())
				{
					ITEM_INFO item_info;

					pItemData += m_sensorInNum * sizeof(unsigned long);
					for (int i = m_sensorInNum; i < itemHead.num; i++)
					{
						item_info.itemID = DD_CONFIG_ITEM_SENSOR_SETUP;
						item_info.parserLen = sizeof(DD_SENSOR_CONFIG);
						item_info.pConfigItem = (unsigned char *)new DD_SENSOR_CONFIG;
						memset(item_info.pConfigItem, 0, sizeof(DD_SENSOR_CONFIG));
						((DD_SENSOR_CONFIG*)item_info.pConfigItem)->bNO = (*pItemData == 0);

						iteminfolist.push_back(item_info);
						pItemData += sizeof(unsigned long);
					}
				}
				else
				{
					pItemData += m_sensorInNum * sizeof(unsigned long);
					for (its = iteminfolist.begin(); its != iteminfolist.end(); its++)
					{
						if ((*its).itemID == DD_CONFIG_ITEM_SENSOR_SETUP)
						{
							((DD_SENSOR_CONFIG*)(*its).pConfigItem)->bNO = (*pItemData == 0);
							pItemData += sizeof(unsigned long);
						}
					}
				}
				break;
			}
		case NCFG_ITEM_SENSOR_IN_NAME:
			{
				if (iteminfolist.empty())
				{
					ITEM_INFO item_info;

					pItemData += m_sensorInNum * sizeof(NCFG_INFO_SHORT_NAME);
					for (int i = m_sensorInNum; i < itemHead.num; i++)
					{
						item_info.itemID = DD_CONFIG_ITEM_SENSOR_SETUP;
						item_info.parserLen = sizeof(DD_SENSOR_CONFIG);
						item_info.pConfigItem = (unsigned char *)new DD_SENSOR_CONFIG;
						memset(item_info.pConfigItem, 0, sizeof(DD_SENSOR_CONFIG));
						strcpy(((DD_SENSOR_CONFIG*)item_info.pConfigItem)->name, ((NCFG_INFO_SHORT_NAME*)pItemData)->name);

						iteminfolist.push_back(item_info);
						pItemData += sizeof(NCFG_INFO_SHORT_NAME);
					}
				}
				else
				{
					pItemData += m_sensorInNum * sizeof(NCFG_INFO_SHORT_NAME);
					for (its = iteminfolist.begin(); its != iteminfolist.end(); its++)
					{
						if ((*its).itemID == DD_CONFIG_ITEM_SENSOR_SETUP)
						{
							strcpy(((DD_SENSOR_CONFIG*)(*its).pConfigItem)->name, ((NCFG_INFO_SHORT_NAME*)pItemData)->name);
							pItemData += sizeof(NCFG_INFO_SHORT_NAME);
						}
					}
				}
				break;
			}
		case NCFG_ITEM_SENSOR_IN_HOLD_TIME:
			{
				if (iteminfolist.empty())
				{
					ITEM_INFO item_info;
					
					pItemData += m_sensorInNum * sizeof(unsigned long);
					for (int i = m_sensorInNum; i < itemHead.num; i++)
					{
						item_info.itemID = DD_CONFIG_ITEM_SENSOR_SETUP;
						item_info.parserLen = sizeof(DD_SENSOR_CONFIG);
						item_info.pConfigItem = (unsigned char *)new DD_SENSOR_CONFIG;
						memset(item_info.pConfigItem, 0, sizeof(DD_SENSOR_CONFIG));
						((DD_SENSOR_CONFIG*)item_info.pConfigItem)->holdTime = *((unsigned short*)pItemData);

						iteminfolist.push_back(item_info);
						pItemData += sizeof(unsigned long);
					}
				}
				else
				{
					pItemData += m_sensorInNum * sizeof(unsigned long);
					for (its = iteminfolist.begin(); its != iteminfolist.end(); its++)
					{
						if ((*its).itemID == DD_CONFIG_ITEM_SENSOR_SETUP)
						{
							((DD_SENSOR_CONFIG*)(*its).pConfigItem)->holdTime = *((unsigned short*)pItemData);
							pItemData += sizeof(unsigned long);
						}
					}
				}
				break;
			}
		case NCFG_ITEM_SENSOR_IN_SCH_WEEK:
			{
				if (iteminfolist.empty())
				{
					ITEM_INFO item_info;

					pItemData += m_sensorInNum * sizeof(WEEK_SCHEDULE);
					for (int i = m_sensorInNum; i < itemHead.num; i++)
					{
						item_info.itemID = DD_CONFIG_ITEM_SENSOR_SCHEDULE;
						item_info.parserLen = sizeof(DD_WEEK_SCHEDULE);
						item_info.pConfigItem = new unsigned char[item_info.parserLen];
						memset(item_info.pConfigItem, 0, item_info.parserLen);
						DD_DATE_SCHEDULE *pDD_Date = ((DD_WEEK_SCHEDULE*)item_info.pConfigItem)->week;
						DATE_SCHEDULE *pDate = ((WEEK_SCHEDULE*)pItemData)->week;
						for (int i = 0; i < 7; i++)
						{
							memcpy(pDD_Date->hour, pDate->hour, sizeof(unsigned long long) * 24);
							pDD_Date++;
							pDate++;
						}

						pItemData += sizeof(WEEK_SCHEDULE);
						iteminfolist.push_back(item_info);
					}
				}
				else
				{
					pItemData += m_sensorInNum * sizeof(WEEK_SCHEDULE);
					for (its = iteminfolist.begin(); its != iteminfolist.end(); its++)
					{
						if ((*its).itemID == DD_CONFIG_ITEM_SENSOR_SCHEDULE)
						{
							DD_DATE_SCHEDULE *pDD_Date = ((DD_WEEK_SCHEDULE*)(*its).pConfigItem)->week;
							DATE_SCHEDULE *pDate = ((WEEK_SCHEDULE*)pItemData)->week;
							for (int i = 0; i < 7; i++)
							{
								memcpy(pDD_Date->hour, pDate->hour, sizeof(unsigned long long) * 24);
								pDD_Date++;
								pDate++;
							}
							pItemData += sizeof(WEEK_SCHEDULE);
						}
					}
				}
				break;
			}
		case NCFG_ITEM_SENSOR_IN_BUZZ:
			{
				if (iteminfolist.empty())
				{
					ITEM_INFO item_info[DD_MAX_CAMERA_NUM];
					for (int i = 0; i < itemHead.num; i++)
					{
						item_info[i].itemID = DD_CONFIG_ITEM_SENSOR_ALARM_OUT;
						item_info[i].parserLen = sizeof(DD_TRIGGER_ALARM_OUT);
						item_info[i].pConfigItem = (unsigned char *)new DD_TRIGGER_ALARM_OUT;
						memset(item_info[i].pConfigItem, 0, sizeof(DD_TRIGGER_ALARM_OUT));
						((DD_TRIGGER_ALARM_OUT*)item_info[i].pConfigItem)->toBuzzer = *pItemData;

						iteminfolist.push_back(item_info[i]);
						pItemData += sizeof(unsigned long);
					}
				}
				else
				{
					for (its = iteminfolist.begin(); its != iteminfolist.end(); its++)
					{
						if ((*its).itemID == DD_CONFIG_ITEM_SENSOR_ALARM_OUT)
						{
							((DD_TRIGGER_ALARM_OUT*)(*its).pConfigItem)->toBuzzer = *pItemData;
							pItemData += sizeof(unsigned long);
						}
					}
				}
				break;
			}
		case NCFG_ITEM_SENSOR_IN_TO_ALARM_OUT:
			{
				if (iteminfolist.empty())
				{
					ITEM_INFO item_info[DD_MAX_CAMERA_NUM];
					for (int i = 0; i < itemHead.num; i++)
					{
						item_info[i].itemID = DD_CONFIG_ITEM_SENSOR_ALARM_OUT;
						item_info[i].parserLen = sizeof(DD_TRIGGER_ALARM_OUT);
						item_info[i].pConfigItem = (unsigned char *)new DD_TRIGGER_ALARM_OUT;
						memset(item_info[i].pConfigItem, 0, sizeof(DD_TRIGGER_ALARM_OUT));
						((DD_TRIGGER_ALARM_OUT*)item_info[i].pConfigItem)->toAlarmOut = *((unsigned long*)pItemData);

						iteminfolist.push_back(item_info[i]);
						pItemData += sizeof(unsigned long);
					}
				}
				else
				{
					for (its = iteminfolist.begin(); its != iteminfolist.end(); its++)
					{
						if ((*its).itemID == DD_CONFIG_ITEM_SENSOR_ALARM_OUT)
						{
							((DD_TRIGGER_ALARM_OUT*)(*its).pConfigItem)->toAlarmOut = *((unsigned long*)pItemData);
							pItemData += sizeof(unsigned long);
						}
					}
				}
				break;
			}
		case NCFG_ITEM_SENSOR_TO_BIG_VIEW:
			{
				if (iteminfolist.empty())
				{
					ITEM_INFO item_info[DD_MAX_CAMERA_NUM];
					for (int i = 0; i < itemHead.num; i++)
					{
						item_info[i].itemID = DD_CONFIG_ITEM_SENSOR_ALARM_OUT;
						item_info[i].parserLen = sizeof(DD_TRIGGER_ALARM_OUT);
						item_info[i].pConfigItem = (unsigned char *)new DD_TRIGGER_ALARM_OUT;
						memset(item_info[i].pConfigItem, 0, sizeof(DD_TRIGGER_ALARM_OUT));
						
						int j = 0;
						ULONGLONG temp = 0x1;
						ULONGLONG test = *((ULONGLONG *)pItemData);
						if (0 == test)
						{
							j = 0xff;
						}
						else
						{
							while (!(test & (temp << j)) && j < itemHead.num)
							{
								j ++;
							}
						}
						
						((DD_TRIGGER_ALARM_OUT*)item_info[i].pConfigItem)->ShowFullScreen = j;

						iteminfolist.push_back(item_info[i]);
						pItemData += sizeof(unsigned long long);
					}
				}
				else
				{
					for (its = iteminfolist.begin(); its != iteminfolist.end(); its++)
					{
						if ((*its).itemID == DD_CONFIG_ITEM_SENSOR_ALARM_OUT)
						{
							int j = 0;
							ULONGLONG temp = 0x1;
							ULONGLONG test = *((ULONGLONG *)pItemData);
							if (test == 0)
							{
								j = 0xff;
							}
							else
							{
								while (!(test & (temp << j)) && j < iteminfolist.size())
								{
									j ++;
								}
							}

							((DD_TRIGGER_ALARM_OUT*)(*its).pConfigItem)->ShowFullScreen = j;
							pItemData += sizeof(unsigned long long);
						}
					}
				}
				break;
			}
		case NCFG_ITEM_SENSOR_IN_TO_EMAIL_ENABLE:
			{
				if (iteminfolist.empty())
				{
					ITEM_INFO item_info[DD_MAX_CAMERA_NUM];
					for (int i = 0; i < itemHead.num; i++)
					{
						item_info[i].itemID = DD_CONFIG_ITEM_SENSOR_ALARM_OUT;
						item_info[i].parserLen = sizeof(DD_TRIGGER_ALARM_OUT);
						item_info[i].pConfigItem = (unsigned char *)new DD_TRIGGER_ALARM_OUT;
						memset(item_info[i].pConfigItem, 0, sizeof(DD_TRIGGER_ALARM_OUT));
						unsigned long test = *(unsigned long *)pItemData;
						((DD_TRIGGER_ALARM_OUT*)item_info[i].pConfigItem)->sendEmail = (bool)test;

						iteminfolist.push_back(item_info[i]);
						pItemData += sizeof(unsigned long);
					}
				}
				else
				{
					for (its = iteminfolist.begin(); its != iteminfolist.end(); its++)
					{
						if ((*its).itemID == DD_CONFIG_ITEM_SENSOR_ALARM_OUT)
						{
							unsigned long test = *(unsigned long *)pItemData;
							((DD_TRIGGER_ALARM_OUT*)(*its).pConfigItem)->sendEmail = (bool)test;
							pItemData += sizeof(unsigned long);
						}
					}
				}
				break;
			}
		case NCFG_ITEM_SENSOR_IN_TO_RECORD:
			{
				if (iteminfolist.empty())
				{
					ITEM_INFO item_info[DD_MAX_CAMERA_NUM];
					for (int i = 0; i < itemHead.num; i++)
					{
						item_info[i].itemID = DD_CONFIG_ITEM_SENSOR_TO_RECORD;
						item_info[i].parserLen = sizeof(DD_TRIGGER_RECORD);
						item_info[i].pConfigItem = (unsigned char *)new DD_TRIGGER_RECORD;
						memset(item_info[i].pConfigItem, 0, sizeof(DD_TRIGGER_RECORD));
						strcpy((char*)((DD_TRIGGER_RECORD*)item_info[i].pConfigItem)->recordCH, (char*)pItemData);

						iteminfolist.push_back(item_info[i]);
						pItemData += sizeof(unsigned long long);
					}
				}
				else
				{
					for (its = iteminfolist.begin(); its != iteminfolist.end(); its++)
					{
						if ((*its).itemID == DD_CONFIG_ITEM_SENSOR_TO_RECORD)
						{
							strcpy((char*)((DD_TRIGGER_RECORD*)(*its).pConfigItem)->recordCH, (char*)pItemData);
							pItemData += sizeof(unsigned long long);
						}
					}
				}
				break;
			}
		case NCFG_ITEM_SENSOR_IN_TO_SNAP:
			{
				if (iteminfolist.empty())
				{
					ITEM_INFO item_info[DD_MAX_CAMERA_NUM];
					for (int i = 0; i < itemHead.num; i++)
					{
						item_info[i].itemID = DD_CONFIG_ITEM_SENSOR_TO_RECORD;
						item_info[i].parserLen = sizeof(DD_TRIGGER_RECORD);
						item_info[i].pConfigItem = (unsigned char *)new DD_TRIGGER_RECORD;
						memset(item_info[i].pConfigItem, 0, sizeof(DD_TRIGGER_RECORD));
						strcpy((char*)((DD_TRIGGER_RECORD*)item_info[i].pConfigItem)->snapCH, (char*)pItemData);

						iteminfolist.push_back(item_info[i]);
						pItemData += sizeof(unsigned long long);
					}
				}
				else
				{
					for (its = iteminfolist.begin(); its != iteminfolist.end(); its++)
					{
						if ((*its).itemID == DD_CONFIG_ITEM_SENSOR_TO_RECORD)
						{
							strcpy((char*)((DD_TRIGGER_RECORD*)(*its).pConfigItem)->snapCH, (char*)pItemData);
							pItemData += sizeof(unsigned long long);
						}
					}
				}
				break;
			}
		case NCFG_ITEM_SENSOR_IN_TO_PTZ:
			{
				if (iteminfolist.empty())
				{
					ITEM_INFO item_info[DD_MAX_CAMERA_NUM];
					for (int i = 0; i < itemHead.num; i++)
					{
						item_info[i].itemID = DD_CONFIG_ITEM_SENSOR_TO_PTZ;
						item_info[i].parserLen = sizeof(DD_TRIGGER_PTZ) * m_deiviceInfo.videoInputNum;
						item_info[i].pConfigItem = (unsigned char *)new DD_TRIGGER_PTZ[m_deiviceInfo.videoInputNum];
						memset(item_info[i].pConfigItem, 0, sizeof(DD_TRIGGER_PTZ) * m_deiviceInfo.videoInputNum);
						for (int j = 0; j < m_deiviceInfo.videoInputNum; j++)
						{
							unsigned char &ptzType = ((DD_TRIGGER_PTZ*)item_info[i].pConfigItem)[j].toPTZType;
							unsigned char &toIndex = ((DD_TRIGGER_PTZ*)item_info[i].pConfigItem)[j].toIndex;
							unsigned char &backIndex = ((DD_TRIGGER_PTZ*)item_info[i].pConfigItem)[j].backIndex;
							
							if (((TO_PTZ*)pItemData)[j].cIsEnablePreset)
							{
								ptzType = 1;
								toIndex = ((TO_PTZ*)pItemData)[j].ucToPreset;
								backIndex = ((TO_PTZ*)pItemData)[j].ucBackPreset;
							}
							else if (((TO_PTZ*)pItemData)[j].cIsEnableCruise)
							{
								ptzType = 2;
								toIndex = ((TO_PTZ*)pItemData)[j].ucToCruise;
							}
							else if (((TO_PTZ*)pItemData)[j].cIsEnableTrack)
							{
								ptzType = 3;
								toIndex = ((TO_PTZ*)pItemData)[j].ucToTrack;
							}
							else
							{
								ptzType = 0;
							}

						}

						iteminfolist.push_back(item_info[i]);
						pItemData += sizeof(TO_PTZ);
					}
				}
				else
				{
					for (its = iteminfolist.begin(); its != iteminfolist.end(); its++)
					{
						if ((*its).itemID == DD_CONFIG_ITEM_SENSOR_TO_PTZ)
						{

							/*for (int i = 0; i < itemHead.num; i++)
							{*/
			
								for (int j = 0; j < m_deiviceInfo.videoInputNum; j++)
								{
									memset(((DD_TRIGGER_PTZ*)(*its).pConfigItem), 0, sizeof(DD_TRIGGER_PTZ) * m_deiviceInfo.videoInputNum);

									unsigned char &ptzType = ((DD_TRIGGER_PTZ*)(*its).pConfigItem)[j].toPTZType;
									unsigned char &toIndex = ((DD_TRIGGER_PTZ*)(*its).pConfigItem)[j].toIndex;
									unsigned char &backIndex = ((DD_TRIGGER_PTZ*)(*its).pConfigItem)[j].backIndex;

									if (((TO_PTZ*)pItemData)[j].cIsEnablePreset)
									{
										ptzType = 1;
										toIndex = ((TO_PTZ*)pItemData)[j].ucToPreset;
										backIndex = ((TO_PTZ*)pItemData)[j].ucBackPreset;
									}
									else if (((TO_PTZ*)pItemData)[j].cIsEnableCruise)
									{
										ptzType = 2;
										toIndex = ((TO_PTZ*)pItemData)[j].ucToCruise;
									}
									else if (((TO_PTZ*)pItemData)[j].cIsEnableTrack)
									{
										ptzType = 3;
										toIndex = ((TO_PTZ*)pItemData)[j].ucToTrack;
									}
									else
									{
										ptzType = 0;
									}

								}
							pItemData += sizeof(TO_PTZ);
							/*}*/
						}
					}
				}
				break;
			}
		case NCFG_ITEM_MOTION_ENABLE: 
			{
				if (iteminfolist.empty())
				{
					ITEM_INFO item_info[DD_MAX_CAMERA_NUM];
					for (int i = 0; i < itemHead.num; i++)
					{
						item_info[i].itemID = DD_CONFIG_ITEM_MOTION_SETUP;
						item_info[i].parserLen = sizeof(DD_MOTION_CONFIG);
						item_info[i].pConfigItem = new unsigned char[item_info[i].parserLen];
						memset(item_info[i].pConfigItem, 0, item_info[i].parserLen);
						((DD_MOTION_CONFIG*)item_info[i].pConfigItem)->enable = *pItemData;

						iteminfolist.push_back(item_info[i]);
						pItemData += sizeof(unsigned long);
					}
				}
				else
				{
					for (its = iteminfolist.begin(); its != iteminfolist.end(); its++)
					{
						if ((*its).itemID == DD_CONFIG_ITEM_MOTION_SETUP)
						{
							((DD_MOTION_CONFIG*)(*its).pConfigItem)->enable = *pItemData;
							pItemData += sizeof(unsigned long);
						}

					}
				}
				break;
			}
		case NCFG_ITEM_MOTION_AREA_INFO: 
			{
				if (iteminfolist.empty())
				{
					ITEM_INFO item_info[DD_MAX_CAMERA_NUM];
					for (int i = 0; i < itemHead.num; i++)
					{
						item_info[i].itemID = DD_CONFIG_ITEM_MOTION_SETUP;
						item_info[i].parserLen = sizeof(DD_MOTION_CONFIG);
						item_info[i].pConfigItem = (unsigned char *)new DD_MOTION_CONFIG;
						memset(item_info[i].pConfigItem, 0, sizeof(DD_MOTION_CONFIG));
						((DD_MOTION_CONFIG*)item_info[i].pConfigItem)->area.sensitivity = (((MOTION_DETECT_INFO*)pItemData)->sensitivity);
						((DD_MOTION_CONFIG*)item_info[i].pConfigItem)->area.hightNum = (((MOTION_DETECT_INFO*)pItemData)->areaInfo.gridHeight);
						((DD_MOTION_CONFIG*)item_info[i].pConfigItem)->area.widthNum = (((MOTION_DETECT_INFO*)pItemData)->areaInfo.gridWidth);
						/*strcpy((char*)((DD_MOTION_CONFIG*)item_info.pConfigItem)->area.area), (((char*)((MOTION_DETECT_INFO*)pItemData)->areaInfo.pData);*/
						memcpy(((DD_MOTION_CONFIG*)item_info[i].pConfigItem)->area.area, ((MOTION_DETECT_INFO*)pItemData)->areaInfo.pData, sizeof(((MOTION_DETECT_INFO*)pItemData)->areaInfo.pData));

						iteminfolist.push_back(item_info[i]);
						pItemData += sizeof(MOTION_DETECT_INFO);
					}
				}
				else
				{
					for (its = iteminfolist.begin(); its != iteminfolist.end(); its++)
					{
						if ((*its).itemID == DD_CONFIG_ITEM_MOTION_SETUP)
						{
							((DD_MOTION_CONFIG*)(*its).pConfigItem)->area.sensitivity = (((MOTION_DETECT_INFO*)pItemData)->sensitivity);
							((DD_MOTION_CONFIG*)(*its).pConfigItem)->area.hightNum = (((MOTION_DETECT_INFO*)pItemData)->areaInfo.gridHeight);
							((DD_MOTION_CONFIG*)(*its).pConfigItem)->area.widthNum = (((MOTION_DETECT_INFO*)pItemData)->areaInfo.gridWidth);
							/*strcpy((char*)((DD_MOTION_CONFIG*)(*its).pConfigItem)->area.area), (((char*)((MOTION_DETECT_INFO*)pItemData)->areaInfo.pData);*/
							memcpy(((DD_MOTION_CONFIG*)(*its).pConfigItem)->area.area, ((MOTION_DETECT_INFO*)pItemData)->areaInfo.pData, sizeof(((MOTION_DETECT_INFO*)pItemData)->areaInfo.pData));
							pItemData += sizeof(MOTION_DETECT_INFO);

						}
					}
				}
				break;
			}
		case NCFG_ITEM_MOTION_HOLD_TIME:
			{
				if (iteminfolist.empty())
				{
					ITEM_INFO item_info[DD_MAX_CAMERA_NUM];
					for (int i = 0; i < itemHead.num; i++)
					{
						item_info[i].itemID = DD_CONFIG_ITEM_MOTION_SETUP;
						item_info[i].parserLen = sizeof(DD_MOTION_CONFIG);
						item_info[i].pConfigItem = new unsigned char[item_info[i].parserLen];
						memset(item_info[i].pConfigItem, 0, item_info[i].parserLen);
						((DD_MOTION_CONFIG*)item_info[i].pConfigItem)->holdTime = *((unsigned short*)pItemData);

						iteminfolist.push_back(item_info[i]);
						pItemData += sizeof(unsigned long);
					}
				}
				else
				{
					for (its = iteminfolist.begin(); its != iteminfolist.end(); its++)
					{
						if ((*its).itemID == DD_CONFIG_ITEM_MOTION_SETUP)
						{
							((DD_MOTION_CONFIG*)(*its).pConfigItem)->holdTime = *((unsigned short*)pItemData);
							pItemData += sizeof(unsigned long);
						}
					}
				}
				break;
			}
		case NCFG_ITEM_MOTION_SCH_WEEK:
			{
				if (iteminfolist.empty())
				{
					ITEM_INFO item_info;
					for (int i = 0; i < itemHead.num; i++)
					{
						item_info.itemID = DD_CONFIG_ITEM_MOTION_SCHEDULE;
						item_info.parserLen = sizeof(DD_WEEK_SCHEDULE);
						item_info.pConfigItem = new unsigned char[item_info.parserLen];
						memset(item_info.pConfigItem, 0, item_info.parserLen);
						DD_DATE_SCHEDULE *pDD_Date = ((DD_WEEK_SCHEDULE*)item_info.pConfigItem)->week;
						DATE_SCHEDULE *pDate = ((WEEK_SCHEDULE*)pItemData)->week;
						for (int i = 0; i < 7; i++)
						{
							memcpy(pDD_Date->hour, pDate->hour, sizeof(unsigned long long) * 24);
							pDD_Date++;
							pDate++;
						}
						
						pItemData += sizeof(WEEK_SCHEDULE);
						iteminfolist.push_back(item_info);
					}
				}
				else
				{
					for (its = iteminfolist.begin(); its != iteminfolist.end(); its++)
					{
						if ((*its).itemID == DD_CONFIG_ITEM_MOTION_SCHEDULE)
						{
							DD_DATE_SCHEDULE *pDD_Date = ((DD_WEEK_SCHEDULE*)(*its).pConfigItem)->week;
							DATE_SCHEDULE *pDate = ((WEEK_SCHEDULE*)pItemData)->week;
							for (int i = 0; i < 7; i++)
							{
								memcpy(pDD_Date->hour, pDate->hour, sizeof(unsigned long long) * 24);
								pDD_Date++;
								pDate++;
							}
							pItemData += sizeof(WEEK_SCHEDULE);
						}
					}
				}
				break;
			}
		case NCFG_ITEM_MAX_PRE_ALARM_TIME:
			{
				if (iteminfolist.empty())
				{
					ITEM_INFO item_info;
					item_info.itemID = DD_CONFIG_ITEM_RECORD_MASK;
					item_info.parserLen = sizeof(DD_RECORD_CONFIG_MASK);
					item_info.pConfigItem = (unsigned char *)new DD_RECORD_CONFIG_MASK;
					memset(item_info.pConfigItem, 0, sizeof(DD_RECORD_CONFIG_MASK));
					DD_RECORD_CONFIG_MASK *rcm = (DD_RECORD_CONFIG_MASK*)item_info.pConfigItem;
					rcm->maxPreAlarmTime = *((unsigned long*)pItemData);
					iteminfolist.push_back(item_info);
				}
				else
				{
					for (its = iteminfolist.begin(); its != iteminfolist.end(); its++)
					{
						if ((*its).itemID == DD_CONFIG_ITEM_RECORD_MASK)
						{
							DD_RECORD_CONFIG_MASK *rcm = (DD_RECORD_CONFIG_MASK*)(*its).pConfigItem;
							rcm->maxPreAlarmTime = *((unsigned long*)pItemData);
							break;
						}
					}
				}
				break;
			}
		case NCFG_ITEM_MOTION_TO_BUZZ:
			{
				if (iteminfolist.empty())
				{
					ITEM_INFO item_info[DD_MAX_CAMERA_NUM];
					for (int i = 0; i < itemHead.num; i++)
					{
						item_info[i].itemID = DD_CONFIG_ITEM_MOTION_ALARM_OUT;
						item_info[i].parserLen = sizeof(DD_TRIGGER_ALARM_OUT);
						item_info[i].pConfigItem = (unsigned char *)new DD_TRIGGER_ALARM_OUT;
						memset(item_info[i].pConfigItem, 0, sizeof(DD_TRIGGER_ALARM_OUT));
						((DD_TRIGGER_ALARM_OUT*)item_info[i].pConfigItem)->toBuzzer = *pItemData;

						iteminfolist.push_back(item_info[i]);
						pItemData += sizeof(unsigned long);
					}
				}
				else
				{
					for (its = iteminfolist.begin(); its != iteminfolist.end(); its++)
					{
						if ((*its).itemID == DD_CONFIG_ITEM_MOTION_ALARM_OUT)
						{
							((DD_TRIGGER_ALARM_OUT*)(*its).pConfigItem)->toBuzzer = *pItemData;
							pItemData += sizeof(unsigned long);
						}
					}
				}
				break;
			}
		case NCFG_ITEM_MOTION_TO_ALARM_OUT:
			{
				if (iteminfolist.empty())
				{
					ITEM_INFO item_info[DD_MAX_CAMERA_NUM];
					for (int i = 0; i < itemHead.num; i++)
					{
						item_info[i].itemID = DD_CONFIG_ITEM_MOTION_ALARM_OUT;
						item_info[i].parserLen = sizeof(DD_TRIGGER_ALARM_OUT);
						item_info[i].pConfigItem = (unsigned char *)new DD_TRIGGER_ALARM_OUT;
						memset(item_info[i].pConfigItem, 0, sizeof(DD_TRIGGER_ALARM_OUT));
						((DD_TRIGGER_ALARM_OUT*)item_info[i].pConfigItem)->toAlarmOut = *((unsigned long*)pItemData);

						iteminfolist.push_back(item_info[i]);
						pItemData += sizeof(unsigned long long);
					}
				}
				else
				{
					for (its = iteminfolist.begin(); its != iteminfolist.end(); its++)
					{
						if ((*its).itemID == DD_CONFIG_ITEM_MOTION_ALARM_OUT)
						{
							((DD_TRIGGER_ALARM_OUT*)(*its).pConfigItem)->toAlarmOut = *((unsigned long*)pItemData);
							pItemData += sizeof(unsigned long long);
						}
					}
				}
				break;
			}
		case NCFG_ITEM_MOTION_TO_BIG_VIEW:
			{
				if (iteminfolist.empty())
				{
					ITEM_INFO item_info[DD_MAX_CAMERA_NUM];
					for (int i = 0; i < itemHead.num; i++)
					{
						item_info[i].itemID = DD_CONFIG_ITEM_MOTION_ALARM_OUT;
						item_info[i].parserLen = sizeof(DD_TRIGGER_ALARM_OUT);
						item_info[i].pConfigItem = (unsigned char *)new DD_TRIGGER_ALARM_OUT;
						memset(item_info[i].pConfigItem, 0, sizeof(DD_TRIGGER_ALARM_OUT));

						int j = 0;
						ULONGLONG temp = 0x1;
						ULONGLONG test = *((ULONGLONG *)pItemData);
						if (0 == test)
						{
							j = 0xff;
						}
						else
						{
							while (!(test & (temp << j)) && j < itemHead.num)
							{
								j ++;
							}
						}

						((DD_TRIGGER_ALARM_OUT*)item_info[i].pConfigItem)->ShowFullScreen = j;
						
						iteminfolist.push_back(item_info[i]);
						pItemData += sizeof(ULONGLONG);
					}
				}
				else
				{
					/*int i = 0;*/
					for (its = iteminfolist.begin(); its != iteminfolist.end(); its++)
					{
						if ((*its).itemID == DD_CONFIG_ITEM_MOTION_ALARM_OUT)
						{
							int j = 0;
							ULONGLONG temp = 0x1;
							ULONGLONG test = *((ULONGLONG *)pItemData);
							if (test == 0)
							{
								j = 0xff;
							}
							else
							{
								while (!(test & (temp << j)) && j < iteminfolist.size())
								{
									j ++;
								}
							}

							((DD_TRIGGER_ALARM_OUT*)(*its).pConfigItem)->ShowFullScreen = j;
							pItemData += sizeof(ULONGLONG);
							/*i++;*/
						}
					}
				}
				break;
			}
		case NCFG_ITEM_MOTION_TO_EMAIL_ENABLE:
			{
				if (iteminfolist.empty())
				{
					ITEM_INFO item_info[DD_MAX_CAMERA_NUM];
					for (int i = 0; i < itemHead.num; i++)
					{
						item_info[i].itemID = DD_CONFIG_ITEM_MOTION_ALARM_OUT;
						item_info[i].parserLen = sizeof(DD_TRIGGER_ALARM_OUT);
						item_info[i].pConfigItem = (unsigned char *)new DD_TRIGGER_ALARM_OUT;
						memset(item_info[i].pConfigItem, 0, sizeof(DD_TRIGGER_ALARM_OUT));
						unsigned long test = *(unsigned long *)pItemData;
						((DD_TRIGGER_ALARM_OUT*)item_info[i].pConfigItem)->sendEmail = test;

						iteminfolist.push_back(item_info[i]);
						pItemData += sizeof(unsigned long);
		
					}
				}
				else
				{
					for (its = iteminfolist.begin(); its != iteminfolist.end(); its++)
					{
						if ((*its).itemID == DD_CONFIG_ITEM_MOTION_ALARM_OUT)
						{
							unsigned long test = *(unsigned long *)pItemData;
							((DD_TRIGGER_ALARM_OUT*)(*its).pConfigItem)->sendEmail = test;
							pItemData += sizeof(unsigned long);
						}
					}
				}
				break;
			}
		case NCFG_ITEM_MOTION_TO_RECORD:
			{
				if (iteminfolist.empty())
				{
					ITEM_INFO item_info[DD_MAX_CAMERA_NUM];
					for (int i = 0; i < itemHead.num; i++)
					{
						item_info[i].itemID = DD_CONFIG_ITEM_MOTION_TO_RECORD;
						item_info[i].parserLen = sizeof(DD_TRIGGER_RECORD);
						item_info[i].pConfigItem = (unsigned char *)new DD_TRIGGER_RECORD;
						memset(item_info[i].pConfigItem, 0, sizeof(DD_TRIGGER_RECORD));
						strcpy((char*)((DD_TRIGGER_RECORD*)item_info[i].pConfigItem)->recordCH, (char*)pItemData);

						iteminfolist.push_back(item_info[i]);
						pItemData += sizeof(unsigned long long);
					}
				}
				else
				{
					for (its = iteminfolist.begin(); its != iteminfolist.end(); its++)
					{
						if ((*its).itemID == DD_CONFIG_ITEM_MOTION_TO_RECORD)
						{
							((DD_TRIGGER_ALARM_OUT*)(*its).pConfigItem)->sendEmail = *pItemData;
							pItemData += sizeof(unsigned long long);
						}
					}
				}
				break;
			}
		case NCFG_ITEM_MOTION_TO_SNAP:
			{
				if (iteminfolist.empty())
				{
					ITEM_INFO item_info[DD_MAX_CAMERA_NUM];
					for (int i = 0; i < itemHead.num; i++)
					{
						item_info[i].itemID = DD_CONFIG_ITEM_MOTION_TO_RECORD;
						item_info[i].parserLen = sizeof(DD_TRIGGER_RECORD);
						item_info[i].pConfigItem = (unsigned char *)new DD_TRIGGER_RECORD;
						memset(item_info[i].pConfigItem, 0, sizeof(DD_TRIGGER_RECORD));
						strcpy((char*)((DD_TRIGGER_RECORD*)item_info[i].pConfigItem)->snapCH, (char*)pItemData);

						iteminfolist.push_back(item_info[i]);
						pItemData += sizeof(unsigned long long);
					}
				}
				else
				{
					for (its = iteminfolist.begin(); its != iteminfolist.end(); its++)
					{
						if ((*its).itemID == DD_CONFIG_ITEM_MOTION_TO_RECORD)
						{
							strcpy((char*)((DD_TRIGGER_RECORD*)(*its).pConfigItem)->snapCH, (char*)pItemData);
							pItemData += sizeof(unsigned long long);
						}
					}
				}
				break;
			}
		case NCFG_ITEM_SHELTER_TO_BUZZ:
			{
				if (iteminfolist.empty())
				{
					ITEM_INFO item_info;
					item_info.itemID = DD_CONFIG_ITEM_SHELTER_ALARM_OUT;
					item_info.parserLen = sizeof(DD_TRIGGER_ALARM_OUT);
					item_info.pConfigItem = (unsigned char *)new DD_TRIGGER_ALARM_OUT;
					memset(item_info.pConfigItem, 0, sizeof(DD_TRIGGER_ALARM_OUT));
					((DD_TRIGGER_ALARM_OUT*)item_info.pConfigItem)->toBuzzer = *pItemData;

					iteminfolist.push_back(item_info);
				}
				else
				{
					for (its = iteminfolist.begin(); its != iteminfolist.end(); its++)
					{
						if ((*its).itemID == DD_CONFIG_ITEM_SHELTER_ALARM_OUT)
						{
							((DD_TRIGGER_ALARM_OUT*)(*its).pConfigItem)->toBuzzer = *pItemData;
							break;
						}
					}
				}
				break;
			}
		case NCFG_ITEM_SHELTER_TO_ALARM_OUT:
			{
				if (iteminfolist.empty())
				{
					ITEM_INFO item_info;
					item_info.itemID = DD_CONFIG_ITEM_SHELTER_ALARM_OUT;
					item_info.parserLen = sizeof(DD_TRIGGER_ALARM_OUT);
					item_info.pConfigItem = (unsigned char *)new DD_TRIGGER_ALARM_OUT;
					memset(item_info.pConfigItem, 0, sizeof(DD_TRIGGER_ALARM_OUT));
					((DD_TRIGGER_ALARM_OUT*)item_info.pConfigItem)->toAlarmOut = *((unsigned long*)pItemData);

					iteminfolist.push_back(item_info);
				}
				else
				{
					for (its = iteminfolist.begin(); its != iteminfolist.end(); its++)
					{
						if ((*its).itemID == DD_CONFIG_ITEM_SHELTER_ALARM_OUT)
						{
							((DD_TRIGGER_ALARM_OUT*)(*its).pConfigItem)->toAlarmOut = *((unsigned long*)pItemData);
							break;
						}
					}
				}
				break;
			}
		case NCFG_ITEM_SHELTER_TO_BIG_VIEW:
			{
				if (iteminfolist.empty())
				{
					ITEM_INFO item_info;
					item_info.itemID = DD_CONFIG_ITEM_SHELTER_ALARM_OUT;
					item_info.parserLen = sizeof(DD_TRIGGER_ALARM_OUT);
					item_info.pConfigItem = (unsigned char *)new DD_TRIGGER_ALARM_OUT;
					memset(item_info.pConfigItem, 0, sizeof(DD_TRIGGER_ALARM_OUT));
					((DD_TRIGGER_ALARM_OUT*)item_info.pConfigItem)->ShowFullScreen = *pItemData;

					iteminfolist.push_back(item_info);
				}
				else
				{
					for (its = iteminfolist.begin(); its != iteminfolist.end(); its++)
					{
						if ((*its).itemID == DD_CONFIG_ITEM_SHELTER_ALARM_OUT)
						{
							((DD_TRIGGER_ALARM_OUT*)(*its).pConfigItem)->ShowFullScreen = *pItemData;
							break;
						}
					}
				}
				break;
			}
		case NCFG_ITEM_SHELTER_TO_EMAIL_ADDR:
			{
				if (iteminfolist.empty())
				{
					ITEM_INFO item_info;
					item_info.itemID = DD_CONFIG_ITEM_SHELTER_ALARM_OUT;
					item_info.parserLen = sizeof(DD_TRIGGER_ALARM_OUT);
					item_info.pConfigItem = (unsigned char *)new DD_TRIGGER_ALARM_OUT;
					memset(item_info.pConfigItem, 0, sizeof(DD_TRIGGER_ALARM_OUT));
					((DD_TRIGGER_ALARM_OUT*)item_info.pConfigItem)->sendEmail = *pItemData;

					iteminfolist.push_back(item_info);
				}
				else
				{
					for (its = iteminfolist.begin(); its != iteminfolist.end(); its++)
					{
						if ((*its).itemID == DD_CONFIG_ITEM_SHELTER_ALARM_OUT)
						{
							((DD_TRIGGER_ALARM_OUT*)(*its).pConfigItem)->sendEmail = *pItemData;
							break;
						}
					}
				}
				break;
			}
		case NCFG_ITEM_SHELTER_TO_RECORD:
			{
				if (iteminfolist.empty())
				{
					ITEM_INFO item_info;
					item_info.itemID = DD_CONFIG_ITEM_SHELTER_TO_RECORD;
					item_info.parserLen = sizeof(DD_TRIGGER_RECORD);
					item_info.pConfigItem = (unsigned char *)new DD_TRIGGER_RECORD;
					memset(item_info.pConfigItem, 0, sizeof(DD_TRIGGER_RECORD));
					strcpy((char*)((DD_TRIGGER_RECORD*)item_info.pConfigItem)->recordCH, (char*)pItemData);

					iteminfolist.push_back(item_info);
				}
				else
				{
					for (its = iteminfolist.begin(); its != iteminfolist.end(); its++)
					{
						if ((*its).itemID == DD_CONFIG_ITEM_SHELTER_TO_RECORD)
						{
							strcpy((char*)((DD_TRIGGER_RECORD*)(*its).pConfigItem)->recordCH, (char*)pItemData);
							break;
						}
					}
				}
				break;
			}
		case NCFG_ITEM_SHELTER_TO_SNAP:
			{
				if (iteminfolist.empty())
				{
					ITEM_INFO item_info;
					item_info.itemID = DD_CONFIG_ITEM_SHELTER_TO_RECORD;
					item_info.parserLen = sizeof(DD_TRIGGER_RECORD);
					item_info.pConfigItem = (unsigned char *)new DD_TRIGGER_RECORD;
					memset(item_info.pConfigItem, 0, sizeof(DD_TRIGGER_RECORD));
					strcpy((char*)((DD_TRIGGER_RECORD*)item_info.pConfigItem)->snapCH, (char*)pItemData);

					iteminfolist.push_back(item_info);
				}
				else
				{
					for (its = iteminfolist.begin(); its != iteminfolist.end(); its++)
					{
						if ((*its).itemID == DD_CONFIG_ITEM_SHELTER_TO_RECORD)
						{
							strcpy((char*)((DD_TRIGGER_RECORD*)(*its).pConfigItem)->snapCH, (char*)pItemData);
							break;
						}
					}
				}
				break;
			}
		case NCFG_ITEM_VL_TO_BUZZ:
			{
				if (iteminfolist.empty())
				{
					ITEM_INFO item_info[DD_MAX_CAMERA_NUM];
					for (int i = 0; i < itemHead.num; i++)
					{
						item_info[i].itemID = DD_CONFIG_ITEM_VLOSS_ALARM_OUT;
						item_info[i].parserLen = sizeof(DD_TRIGGER_ALARM_OUT);
						item_info[i].pConfigItem = (unsigned char *)new DD_TRIGGER_ALARM_OUT;
						memset(item_info[i].pConfigItem, 0, sizeof(DD_TRIGGER_ALARM_OUT));
						((DD_TRIGGER_ALARM_OUT*)item_info[i].pConfigItem)->toBuzzer = *pItemData;

						iteminfolist.push_back(item_info[i]);
						pItemData += sizeof(unsigned long);
					}
				}
				else
				{
					for (its = iteminfolist.begin(); its != iteminfolist.end(); its++)
					{
						if ((*its).itemID == DD_CONFIG_ITEM_VLOSS_ALARM_OUT)
						{
							((DD_TRIGGER_ALARM_OUT*)(*its).pConfigItem)->toBuzzer = *pItemData;
							pItemData += sizeof(unsigned long);
						}
					}
				}
				break;
			}
		case NCFG_ITEM_VL_TO_ALARM_OUT:
			{
				if (iteminfolist.empty())
				{
					ITEM_INFO item_info[DD_MAX_CAMERA_NUM];
					for (int i = 0; i < itemHead.num; i++)
					{
						item_info[i].itemID = DD_CONFIG_ITEM_VLOSS_ALARM_OUT;
						item_info[i].parserLen = sizeof(DD_TRIGGER_ALARM_OUT);
						item_info[i].pConfigItem = (unsigned char *)new DD_TRIGGER_ALARM_OUT;
						memset(item_info[i].pConfigItem, 0, sizeof(DD_TRIGGER_ALARM_OUT));
						((DD_TRIGGER_ALARM_OUT*)item_info[i].pConfigItem)->toAlarmOut = *((unsigned long*)pItemData);
						iteminfolist.push_back(item_info[i]);
						pItemData += sizeof(unsigned long long);
					}
				}
				else
				{
					for (its = iteminfolist.begin(); its != iteminfolist.end(); its++)
					{
						if ((*its).itemID == DD_CONFIG_ITEM_VLOSS_ALARM_OUT)
						{
							((DD_TRIGGER_ALARM_OUT*)(*its).pConfigItem)->toAlarmOut = *((unsigned long*)pItemData);
							pItemData += sizeof(unsigned long long);
						}
					}
				}
				break;
			}
		case NCFG_ITEM_VL_TO_BIG_VIEW:
			{
				if (iteminfolist.empty())
				{
					ITEM_INFO item_info[DD_MAX_CAMERA_NUM];
					for (int i = 0; i < itemHead.num; i++)
					{
						item_info[i].itemID = DD_CONFIG_ITEM_VLOSS_ALARM_OUT;
						item_info[i].parserLen = sizeof(DD_TRIGGER_ALARM_OUT);
						item_info[i].pConfigItem = (unsigned char *)new DD_TRIGGER_ALARM_OUT;
						memset(item_info[i].pConfigItem, 0, sizeof(DD_TRIGGER_ALARM_OUT));
						
						int j = 0;
						ULONGLONG temp = 0x1;
						ULONGLONG test = *((ULONGLONG *)pItemData);
						if (0 == test)
						{
							j = 0xff;
						}
						else
						{
							while (!(test & (temp << j)) && j < itemHead.num)
							{
								j ++;
							}
						}

						((DD_TRIGGER_ALARM_OUT*)item_info[i].pConfigItem)->ShowFullScreen = j;

						iteminfolist.push_back(item_info[i]);
						pItemData += sizeof(unsigned long long);
					}
				}
				else
				{
					/*int i = 0;*/
					for (its = iteminfolist.begin(); its != iteminfolist.end(); its++)
					{
						if ((*its).itemID == DD_CONFIG_ITEM_VLOSS_ALARM_OUT)
						{
							int j = 0;
							ULONGLONG temp = 0x1;
							ULONGLONG test = *((ULONGLONG *)pItemData);
							if (test == 0)
							{
								j = 0xff;
							}
							else
							{
								while (!(test & (temp << j)) && j < iteminfolist.size())
								{
									j ++;
								}
							}
							((DD_TRIGGER_ALARM_OUT*)(*its).pConfigItem)->ShowFullScreen = j;
							pItemData += sizeof(unsigned long long);
							/*i++;*/
						}
					}
				}
				break;
			}
		case NCFG_ITEM_VL_TO_EMAIL_ENABLE:
			{
				if (iteminfolist.empty())
				{
					ITEM_INFO item_info[DD_MAX_CAMERA_NUM];
					for (int i = 0; i < itemHead.num; i++)
					{
						item_info[i].itemID = DD_CONFIG_ITEM_VLOSS_ALARM_OUT;
						item_info[i].parserLen = sizeof(DD_TRIGGER_ALARM_OUT);
						item_info[i].pConfigItem = (unsigned char *)new DD_TRIGGER_ALARM_OUT;
						memset(item_info[i].pConfigItem, 0, sizeof(DD_TRIGGER_ALARM_OUT));
						unsigned long test = *(unsigned long *)pItemData;
						((DD_TRIGGER_ALARM_OUT*)item_info[i].pConfigItem)->sendEmail = test;

						iteminfolist.push_back(item_info[i]);
						pItemData += sizeof(unsigned long);
					}
				}
				else
				{
					
					for (its = iteminfolist.begin(); its != iteminfolist.end(); its++)
					{
						if ((*its).itemID == DD_CONFIG_ITEM_VLOSS_ALARM_OUT)
						{
							unsigned long test = *(unsigned long *)pItemData;
							((DD_TRIGGER_ALARM_OUT*)(*its).pConfigItem)->sendEmail = test;
							pItemData += sizeof(unsigned long);
						}
					}
				}
				break;
			}
		case NCFG_ITEM_VL_TO_RECORD:
			{
				if (iteminfolist.empty())
				{
					ITEM_INFO item_info[DD_MAX_CAMERA_NUM];
					for (int i = 0; i < itemHead.num; i++)
					{
						item_info[i].itemID = DD_CONFIG_ITEM_VLOSS_TO_RECORD;
						item_info[i].parserLen = sizeof(DD_TRIGGER_RECORD);
						item_info[i].pConfigItem = (unsigned char *)new DD_TRIGGER_RECORD;
						memset(item_info[i].pConfigItem, 0, sizeof(DD_TRIGGER_RECORD));
						strcpy((char*)((DD_TRIGGER_RECORD*)item_info[i].pConfigItem)->recordCH, (char*)pItemData);

						iteminfolist.push_back(item_info[i]);
						pItemData += sizeof(unsigned long long);
					}
				}
				else
				{
					for (its = iteminfolist.begin(); its != iteminfolist.end(); its++)
					{
						if ((*its).itemID == DD_CONFIG_ITEM_VLOSS_TO_RECORD)
						{
							strcpy((char*)((DD_TRIGGER_RECORD*)(*its).pConfigItem)->recordCH, (char*)pItemData);
							pItemData += sizeof(unsigned long long);
						}
					}
				}
				break;
			}
		case NCFG_ITEM_VL_TO_SNAP:
			{
				if (iteminfolist.empty())
				{
					ITEM_INFO item_info[DD_MAX_CAMERA_NUM];
					for (int i = 0; i < itemHead.num; i++)
					{
						item_info[i].itemID = DD_CONFIG_ITEM_VLOSS_TO_RECORD;
						item_info[i].parserLen = sizeof(DD_TRIGGER_RECORD);
						item_info[i].pConfigItem = (unsigned char *)new DD_TRIGGER_RECORD;
						memset(item_info[i].pConfigItem, 0, sizeof(DD_TRIGGER_RECORD));
						strcpy((char*)((DD_TRIGGER_RECORD*)item_info[i].pConfigItem)->snapCH, (char*)pItemData);

						iteminfolist.push_back(item_info[i]);
						pItemData += sizeof(unsigned long long);
					}
				}
				else
				{
					for (its = iteminfolist.begin(); its != iteminfolist.end(); its++)
					{
						if ((*its).itemID == DD_CONFIG_ITEM_VLOSS_TO_RECORD)
						{
							strcpy((char*)((DD_TRIGGER_RECORD*)(*its).pConfigItem)->snapCH, (char*)pItemData);
							pItemData += sizeof(unsigned long long);
						}
					}
				}
				break;
			}
		case NCFG_ITEM_ALARM_OUT_NAME:
			{
				if (iteminfolist.empty())
				{
					ITEM_INFO item_info;
					item_info.itemID = DD_CONFIG_ITEM_RELAY_SETUP;
					item_info.parserLen = sizeof(DD_RELAY_CONFIG)*4;
					item_info.pConfigItem = (unsigned char *)new DD_RELAY_CONFIG[4];
					memset(item_info.pConfigItem, 0, sizeof(DD_RELAY_CONFIG)*4);
					for (int i = 0; i < m_deiviceInfo.sensorOutputNum; i++)
					{
						strcpy(((DD_RELAY_CONFIG*)(item_info.pConfigItem+i*sizeof(DD_RELAY_CONFIG)))->name, ((NCFG_INFO_SHORT_NAME*)pItemData)->name);
						pItemData += sizeof(((NCFG_INFO_SHORT_NAME*)pItemData)->name);
					}
					iteminfolist.push_back(item_info);
				}
				else
				{
					for (its = iteminfolist.begin(); its != iteminfolist.end(); its++)
					{
						if ((*its).itemID == DD_CONFIG_ITEM_RELAY_SETUP)
						{
							for (int i=0;i<m_deiviceInfo.sensorOutputNum; i++)
							{
								strcpy(((DD_RELAY_CONFIG*)((*its).pConfigItem+i*sizeof(DD_RELAY_CONFIG)))->name, ((NCFG_INFO_SHORT_NAME*)pItemData)->name);
								pItemData += sizeof(((NCFG_INFO_SHORT_NAME*)pItemData)->name);
							}
						}
					}
				}
				break;
			}
		case NCFG_ITEM_ALARM_OUT_HOLD_TIME:
			{
				if (iteminfolist.empty())
				{
					ITEM_INFO item_info;
					item_info.itemID = DD_CONFIG_ITEM_RELAY_SETUP;
					item_info.parserLen = sizeof(DD_RELAY_CONFIG)*4;
					item_info.pConfigItem = (unsigned char *)new DD_RELAY_CONFIG[4];
					memset(item_info.pConfigItem, 0, sizeof(DD_RELAY_CONFIG)*4);
					for (int i = 0; i < m_deiviceInfo.sensorOutputNum; i++)
					{
						((DD_RELAY_CONFIG*)item_info.pConfigItem)[i].holdTime = *((unsigned short*)pItemData);
						pItemData += sizeof(unsigned long);
					}
					iteminfolist.push_back(item_info);
				}
				else
				{
					for (its = iteminfolist.begin(); its != iteminfolist.end(); its++)
					{
						if ((*its).itemID == DD_CONFIG_ITEM_RELAY_SETUP)
						{
							for (int i = 0; i < m_deiviceInfo.sensorOutputNum; i++)
							{
								((DD_RELAY_CONFIG*)(*its).pConfigItem)[i].holdTime = *((unsigned short*)pItemData);
								pItemData += sizeof(unsigned long);
							}
						}
					}
				}
				break;
			}
		case NCFG_ITEM_BUZZER_ENABLE:
			{
				if (iteminfolist.empty())
				{
					ITEM_INFO item_info;
					item_info.itemID = DD_CONFIG_ITEM_BUZZER_SETUP;
					item_info.parserLen = sizeof(DD_BUZZER_CONFIG);
					item_info.pConfigItem = (unsigned char *)new DD_BUZZER_CONFIG;
					memset(item_info.pConfigItem, 0, sizeof(DD_BUZZER_CONFIG));
					((DD_BUZZER_CONFIG*)item_info.pConfigItem)->enable = *pItemData;
					
					iteminfolist.push_back(item_info);
				}
				else
				{
					for (its = iteminfolist.begin(); its != iteminfolist.end(); its++)
					{
						if ((*its).itemID == DD_CONFIG_ITEM_BUZZER_SETUP)
						{
							((DD_BUZZER_CONFIG*)(*its).pConfigItem)->enable = *pItemData;
							break;
						}
					}
				}
				break;
			}
		case NCFG_ITEM_BUZZER_HOLD_TIME:
			{
				if (iteminfolist.empty())
				{
					ITEM_INFO item_info;
					item_info.itemID = DD_CONFIG_ITEM_BUZZER_SETUP;
					item_info.parserLen = sizeof(DD_BUZZER_CONFIG);
					item_info.pConfigItem = (unsigned char *)new DD_BUZZER_CONFIG;
					memset(item_info.pConfigItem, 0, sizeof(DD_BUZZER_CONFIG));
					((DD_BUZZER_CONFIG*)item_info.pConfigItem)->holdTime = *((unsigned short*)pItemData);
					
					iteminfolist.push_back(item_info);
				}
				else
				{
					for (its = iteminfolist.begin(); its != iteminfolist.end(); its++)
					{
						if ((*its).itemID == DD_CONFIG_ITEM_BUZZER_SETUP)
						{
							((DD_BUZZER_CONFIG*)(*its).pConfigItem)->holdTime = *((unsigned short*)pItemData);
							break;
						}
					}
				}
				break;
			}
		case NCFG_ITEM_PTZ_SERIAL:
			{
				if (iteminfolist.empty())
				{
					ITEM_INFO item_info[DD_MAX_CAMERA_NUM];
					for (int i = 0; i < itemHead.num; i++)
					{
						item_info[i].itemID = DD_CONFIG_ITEM_PTZ_SETUP;
						item_info[i].parserLen = sizeof(DD_PTZ_CONFIG);
						item_info[i].pConfigItem = (unsigned char *)new DD_PTZ_CONFIG;
						memset(item_info[i].pConfigItem, 0, sizeof(DD_PTZ_CONFIG));
						((DD_PTZ_CONFIG*)item_info[i].pConfigItem)->enable = ((PTZ_SERIAL_INFO*)pItemData)->bEnable;
						((DD_PTZ_CONFIG*)item_info[i].pConfigItem)->protocol = (unsigned long)(((PTZ_SERIAL_INFO*)pItemData)->protocol);
						((DD_PTZ_CONFIG*)item_info[i].pConfigItem)->serial.baudRate = (unsigned long)(((PTZ_SERIAL_INFO*)pItemData)->baudRate);
						((DD_PTZ_CONFIG*)item_info[i].pConfigItem)->address = (unsigned long)(((PTZ_SERIAL_INFO*)pItemData)->devAddr);

						iteminfolist.push_back(item_info[i]);
						pItemData += sizeof(PTZ_SERIAL_INFO);
						
					}
				}
				else
				{
					for (its = iteminfolist.begin(); its != iteminfolist.end(); its++)
					{
						if ((*its).itemID == DD_CONFIG_ITEM_PTZ_SETUP)
						{
							((DD_PTZ_CONFIG*)(*its).pConfigItem)->enable = ((PTZ_SERIAL_INFO*)pItemData)->bEnable;
							((DD_PTZ_CONFIG*)(*its).pConfigItem)->protocol = (unsigned long)(((PTZ_SERIAL_INFO*)pItemData)->protocol);
							((DD_PTZ_CONFIG*)(*its).pConfigItem)->serial.baudRate = (unsigned long)(((PTZ_SERIAL_INFO*)pItemData)->baudRate);
							((DD_PTZ_CONFIG*)(*its).pConfigItem)->address = (unsigned long)(((PTZ_SERIAL_INFO*)pItemData)->devAddr);
							pItemData += sizeof(PTZ_SERIAL_INFO);
						}
					}
				}
				break;
			}
			if((itemHead.itemID >= NCFG_ITEM_PTZ_INFO_PRESET_INFO_CHBASE) && (itemHead.itemID < NCFG_ITEM_PTZ_INFO_PRESET_INFO_CHEND))
			{
				ITEM_INFO item_info;
				item_info.itemID = DD_CONFIG_ITEM_PTZ_PRESET;
				item_info.parserLen = sizeof(DD_PTZ_PRESET_CONFIG);
				item_info.pConfigItem = (unsigned char *)new DD_PTZ_PRESET_CONFIG;
				memset(item_info.pConfigItem, 0, sizeof(DD_PTZ_PRESET_CONFIG));
				DD_PTZ_PRESET_CONFIG *pChannelPreset = (DD_PTZ_PRESET_CONFIG*)(item_info.pConfigItem);

				PTZ_PRESET *pPtzPreset = (PTZ_PRESET *)pItemData;
				long presetNum = itemDataLen / sizeof(PTZ_PRESET);

				pChannelPreset->iSize = sizeof(DD_PTZ_PRESET_CONFIG);
				for(long i = 0; i < presetNum; i++)
				{
					if(pPtzPreset[i].enable)
					{
						pChannelPreset->enablePreset[pPtzPreset[i].index] = 1;
					}
				}

				iteminfolist.push_back(item_info);
			}
		default:
			{
				break;
			}
		}

		if((itemHead.itemID >= NCFG_ITEM_PTZ_INFO_PRESET_INFO_CHBASE) && (itemHead.itemID < NCFG_ITEM_PTZ_INFO_PRESET_INFO_CHEND))
		{//预置点配置
			ITEM_INFO item_info;
			item_info.itemID = DD_CONFIG_ITEM_PTZ_PRESET;
			item_info.parserLen = sizeof(DD_PTZ_PRESET_CONFIG);
			item_info.pConfigItem = (unsigned char *)new DD_PTZ_PRESET_CONFIG;
			memset(item_info.pConfigItem, 0, sizeof(DD_PTZ_PRESET_CONFIG));
			DD_PTZ_PRESET_CONFIG *pChannelPreset = (DD_PTZ_PRESET_CONFIG*)(item_info.pConfigItem);

			PTZ_PRESET *pPtzPreset = (PTZ_PRESET *)pItemData;
			long presetNum = itemDataLen / sizeof(PTZ_PRESET);

			pChannelPreset->iSize = sizeof(DD_PTZ_PRESET_CONFIG);
			for(long i = 0; i < presetNum; i++)
			{
				if(pPtzPreset[i].enable)
				{
					pChannelPreset->enablePreset[pPtzPreset[i].index] = 1;
				}
			}

			iteminfolist.push_back(item_info);
		}
		if((itemHead.itemID >= NCFG_ITEM_PTZ_INFO_CRUISE_INFO_CHBASE) && (itemHead.itemID < NCFG_ITEM_PTZ_INFO_CRUISE_INFO_CHEND))
		{   //巡航线配置（转换数据格式），ipc通过巡航线的序号，进行覆盖相同序号的巡航线.
			ITEM_INFO item_info;
			unsigned long channel = itemHead.itemID-NCFG_ITEM_PTZ_INFO_CRUISE_INFO_CHBASE;
			item_info.itemID = (enum dd_config_item_id)(DD_CONFIG_ITEM_PTZ_CRUISE + channel);
			item_info.parserLen = itemDataLen;
			if(itemDataLen > 0)
			{
				item_info.pConfigItem = new unsigned char [itemDataLen];
				memset(item_info.pConfigItem, 0,itemDataLen);

				NCFG_ITEM_HEAD *pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>((unsigned char *)pDeviceCfg + sizeof(NCFG_BLOCK_HEAD));
				//每个通道最多8条巡航线
				pItemHead->num =(pItemHead->num > CRUISE_NUMBER_MAX) ? CRUISE_NUMBER_MAX : pItemHead->num;
				unsigned int itemSum = pItemHead->num;
				unsigned char *pTemp = item_info.pConfigItem;//pItemData;
				NCFG_INFO_CRUISE_HEAD *pCuriseHead = NULL;
				PTZ_CURISE_POINT  *pPoint = NULL;
				CRUISE_INFO_HEAD *pCruiseHeadV3 = NULL;
				PTZ_CURISE_POINT *pPointV3 = NULL;

				unsigned int uPosPoint=0;
				unsigned int uCruise = 0;
				unsigned int offset =itemSum*sizeof(NCFG_INFO_CRUISE_HEAD);
				unsigned int offsetV3 = 0;

				for(unsigned int un=0; un<itemSum; un++)
				{  
					uCruise = un*sizeof(NCFG_INFO_CRUISE_HEAD);
					pCuriseHead = reinterpret_cast<NCFG_INFO_CRUISE_HEAD *>(pItemData + uCruise);

					pCruiseHeadV3 = reinterpret_cast<CRUISE_INFO_HEAD *>(pTemp);
					pCruiseHeadV3->cruiseFlag = 0;
					pCruiseHeadV3->cruiseIndex = pCuriseHead->index;
					pCruiseHeadV3->cruisepointnum = pCuriseHead->cruisepointnum;
					memcpy(pCruiseHeadV3->name,pCuriseHead->name,36);
					pCruiseHeadV3->cruiseIndex++;  //ipc巡航线序号是从1开始的，DVR是从0开始的

					uPosPoint = sizeof(PTZ_CURISE_POINT) * (pCuriseHead->cruisepointnum);
					pPoint = reinterpret_cast<PTZ_CURISE_POINT  *>(pItemData + offset);
					/*memcpy(pTemp, pPoint, uPosPoint);*/
					pTemp = pTemp+(sizeof(NCFG_INFO_CRUISE_HEAD));
					pPointV3 = reinterpret_cast<PTZ_CURISE_POINT *>(pTemp);
					//将预置点从0开始变成从1开始
					unsigned char * pTempConvert = NULL;
					for (unsigned int i=0; i<pCuriseHead->cruisepointnum; i++)
					{
				      	pPointV3->dwellSpeed = pPoint->dwellSpeed;
						pPointV3->dwellTime  = pPoint->dwellTime;
						pPointV3->preset     = pPoint->preset;
						pPointV3->preset++;
                        
						++pPoint;
						++pPointV3;
					}
					pTemp= pTemp+uPosPoint;
					offset += uPosPoint;
				}	
				iteminfolist.push_back(item_info);
			}
		}
	}
	//返回SDK配置数据
	long returnLen = sizeof(DD_CONFIG_ITEM_HEAD);
	int itemNum = 0;
	unsigned short sunlen = 0;
	unsigned short itemID = 0;
	for (its = iteminfolist.begin(); its != iteminfolist.end(); its++)
	{
		returnLen += (*its).parserLen;
		sunlen = (*its).parserLen;
		itemID = (*its).itemID;
		itemNum++;
	}
	
	if(pDDcfgHead)
	{
		pDDcfgHead->itemID = itemID;
		pDDcfgHead->num = itemNum;
		pDDcfgHead->subLen = sunlen;
		pDDcfgHead->len = itemNum * sunlen;
	}
	else
	{
		unsigned char *pTempBuf = new unsigned char[returnLen];

		DD_CONFIG_ITEM_HEAD item_head;
		item_head.itemID = itemID;
		item_head.num = itemNum;
		item_head.subLen = sunlen;
		item_head.len = itemNum * sunlen;
		memcpy(pTempBuf, &item_head, sizeof(DD_CONFIG_ITEM_HEAD));

		unsigned char *pTemp = pTempBuf + sizeof(DD_CONFIG_ITEM_HEAD);
		for (its = iteminfolist.begin(); its != iteminfolist.end(); its++)
		{
			memcpy(pTemp, (*its).pConfigItem, (*its).parserLen);
			pTemp += (*its).parserLen;
		}

		//nd_assert(returnLen <= sdkCfgLen, printf("returnLen = %d. sdkCfgLen = %d.\n", returnLen, sdkCfgLen));
		//memcpy(pSDKCfg, pTempBuf, returnLen);
		
		nd_assert(!pSDKCfg, printf("(%s.%d)pSDKCfg = %d. sdkCfgLen = %d.\n", __FILE__, __LINE__, pSDKCfg, sdkCfgLen));
		pSDKCfg = pTempBuf;
		sdkCfgLen = returnLen;
	}

	pConfigBlock.DestroyBlock();

	for (its = iteminfolist.begin(); its != iteminfolist.end(); its++)
	{
		delete [] (*its).pConfigItem;
		(*its).pConfigItem = NULL;
	}

	//delete [] pTempBuf;
	//pTemp = NULL;
	//pTempBuf = NULL;
	iteminfolist.clear();

	return returnLen;
}

long CConfigParser_V3::CfgSDK2CfgDevice(void *pSDKCfg, long sdkCfgLen, void *pDeviceCfg, long deviceCfgLen)
{
	NCFG_BLOCK_HEAD blockHead;
	blockHead.netcfgver = NCFG_VERSION;
	blockHead.ItemNum = 0;
	long datalen = 0;

	CConfigBlock_v4 pConfigBlock;
	pConfigBlock.CreateBlock((unsigned char *)pSDKCfg, sdkCfgLen);

	std::list<DD_CONFIG_ITEM_HEAD_EX> &configList = pConfigBlock.GetConfigItemList();
	unsigned char *pItemData = NULL;
	unsigned long itemDataLen;
	DD_CONFIG_ITEM_HEAD itemHead;
	
	std::list<ITEM_INFO> iteminfolist;
	std::list<ITEM_INFO>::iterator its;

	for(std::list<DD_CONFIG_ITEM_HEAD_EX>::iterator it = configList.begin(); it != configList.end(); it++)
	{
		itemHead = (*it).item_head;
		if(!pConfigBlock.GetConfig((DD_CONFIG_ITEM_ID)itemHead.itemID, &pItemData, itemDataLen))
		{
			assert(false);
			continue;
		}
		switch (itemHead.itemID)
		{
		case DD_CONFIG_ITEM_DEVICE_INFO:
			{
				unsigned long len = sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD)*2 + sizeof(unsigned long) + sizeof(NCFG_INFO_SHORT_NAME);
				unsigned char *pBuf = new unsigned char[len];
				memset(pBuf, 0 ,sizeof(unsigned char)*len);
				unsigned char *pTempCmdData = pBuf;

				blockHead.ItemNum = 2;
				memcpy(pTempCmdData, &blockHead, sizeof(NCFG_BLOCK_HEAD));
				pTempCmdData += sizeof(NCFG_BLOCK_HEAD);
				//填充配置项头
				NCFG_ITEM_HEAD item[2];
				NCFG_ITEM_HEAD *pHead = item;
				pHead->itemID = NCFG_ITEM_DEVICE_ID;
				pHead->len = sizeof(unsigned long);
				pHead->num = 1;
				pHead->subLen = sizeof(unsigned long);
				memcpy(pTempCmdData, pHead, sizeof(NCFG_ITEM_HEAD));
				pHead++;
				pTempCmdData += sizeof(NCFG_ITEM_HEAD);

				pHead->itemID = NCFG_ITEM_DEVICE_NAME;
				pHead->len = sizeof(NCFG_INFO_SHORT_NAME);
				pHead->num = 1;
				pHead->subLen = sizeof(NCFG_INFO_SHORT_NAME);
				memcpy(pTempCmdData, pHead, sizeof(NCFG_ITEM_HEAD));
				pHead++;
				pTempCmdData += sizeof(NCFG_ITEM_HEAD);

				DD_DEVICE_INFO* pDeviceInfo =  (DD_DEVICE_INFO*)pItemData;

				memcpy(pTempCmdData, &(pDeviceInfo->deviceID), sizeof(unsigned long));
				pTempCmdData += sizeof(unsigned long);

				NCFG_INFO_SHORT_NAME deviceName;
				strcpy(deviceName.name, pDeviceInfo->deviceName);
				memcpy(pTempCmdData, deviceName.name, sizeof(NCFG_INFO_SHORT_NAME));
				pTempCmdData += sizeof(NCFG_INFO_SHORT_NAME);

				ITEM_INFO item_info;
				item_info.itemID = DD_CONFIG_ITEM_DEVICE_INFO;
				item_info.parserLen = len;
				item_info.pConfigItem = (unsigned char *)pBuf;
				iteminfolist.push_back(item_info);
				datalen += len;
				break;
			}
		case DD_CONFIG_ITEM_SYSTEM_BASIC:
			{
				unsigned long len = sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD)*6 + sizeof(unsigned long)*6;
				unsigned char *pbuf = new unsigned char[len];
				memset(pbuf, 0 ,sizeof(unsigned char)*len);
				unsigned char *pTempCmdData = pbuf;

				blockHead.ItemNum = 6;
				memcpy(pTempCmdData, &blockHead, sizeof(NCFG_BLOCK_HEAD));
				pTempCmdData += sizeof(NCFG_BLOCK_HEAD);
				//填充配置项头
				NCFG_ITEM_HEAD item[6];
				NCFG_ITEM_HEAD *pHead = item;
				pHead->itemID = NCFG_ITEM_VIDEO_FORMAT;
				pHead->len = sizeof(unsigned long);
				pHead->num = 1;
				pHead->subLen = sizeof(unsigned long);
				memcpy(pTempCmdData, pHead, sizeof(NCFG_ITEM_HEAD));
				pHead++;
				pTempCmdData += sizeof(NCFG_ITEM_HEAD);

				pHead->itemID = NCFG_ITEM_AUTHORIZATION;
				pHead->len = sizeof(unsigned long);
				pHead->num = 1;
				pHead->subLen = sizeof(unsigned long);
				memcpy(pTempCmdData, pHead, sizeof(NCFG_ITEM_HEAD));
				pHead++;
				pTempCmdData += sizeof(NCFG_ITEM_HEAD);

				pHead->itemID = NCFG_ITEM_LANGUAGE;
				pHead->len = sizeof(unsigned long);
				pHead->num = 1;
				pHead->subLen = sizeof(unsigned long);
				memcpy(pTempCmdData, pHead, sizeof(NCFG_ITEM_HEAD));
				pHead++;
				pTempCmdData += sizeof(NCFG_ITEM_HEAD);

				pHead->itemID = NCFG_ITEM_SCRENN_SAVE_TIME;
				pHead->len = sizeof(unsigned long);
				pHead->num = 1;
				pHead->subLen = sizeof(unsigned long);
				memcpy(pTempCmdData, pHead, sizeof(NCFG_ITEM_HEAD));
				pHead++;
				pTempCmdData += sizeof(NCFG_ITEM_HEAD);

				pHead->itemID = NCFG_ITEM_REC_RECYCLE;
				pHead->len = sizeof(unsigned long);
				pHead->num = 1;
				pHead->subLen = sizeof(unsigned long);
				memcpy(pTempCmdData, pHead, sizeof(NCFG_ITEM_HEAD));
				pHead++;
				pTempCmdData += sizeof(NCFG_ITEM_HEAD);

				pHead->itemID = NCFG_ITEM_VGA_RESOLUTION;
				pHead->len = sizeof(unsigned long);
				pHead->num = 1;
				pHead->subLen = sizeof(unsigned long);
				memcpy(pTempCmdData, pHead, sizeof(NCFG_ITEM_HEAD));
				pHead++;
				pTempCmdData += sizeof(NCFG_ITEM_HEAD);

				DD_BASIC_CONFIG* pBasicConfig =  (DD_BASIC_CONFIG*)pItemData;

				memcpy(pTempCmdData, &(pBasicConfig->videoFormat), sizeof(unsigned long));
				pTempCmdData += sizeof(unsigned long);

				memcpy(pTempCmdData, &(pBasicConfig->passwordCheck), sizeof(unsigned long));
				pTempCmdData += sizeof(unsigned long);

				memcpy(pTempCmdData, &(pBasicConfig->deviceLanguage), sizeof(unsigned long));
				pTempCmdData += sizeof(unsigned long);

				memcpy(pTempCmdData, &(pBasicConfig->screensaver), sizeof(unsigned long));
				pTempCmdData += sizeof(unsigned long);

				memcpy(pTempCmdData, &(pBasicConfig->RecycleRecord), sizeof(unsigned long));
				pTempCmdData += sizeof(unsigned long);

				memcpy(pTempCmdData, &(pBasicConfig->videoOutResolution), sizeof(unsigned long));
				pTempCmdData += sizeof(unsigned long);

				ITEM_INFO item_info;
				item_info.itemID = DD_CONFIG_ITEM_SYSTEM_BASIC;
				item_info.parserLen = len;
				item_info.pConfigItem = (unsigned char *)pbuf;
				iteminfolist.push_back(item_info);
				datalen += len;
				break;
			}
		case DD_CONFIG_ITEM_DATE_TIME:
			{
				unsigned long len = sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD)*DD_DATE_TIME_CONFIG_NUM + sizeof(unsigned long)*(DD_DATE_TIME_CONFIG_NUM - 1) + sizeof(NCFG_INFO_LONG_NAME);
				unsigned char *pbuf = new unsigned char[len];
				memset(pbuf, 0 ,sizeof(unsigned char)*len);
				unsigned char *pTempCmdData = pbuf;

				blockHead.ItemNum = DD_DATE_TIME_CONFIG_NUM;
				memcpy(pTempCmdData, &blockHead, sizeof(NCFG_BLOCK_HEAD));
				pTempCmdData += sizeof(NCFG_BLOCK_HEAD);
				//填充配置项头
				NCFG_ITEM_HEAD item[DD_DATE_TIME_CONFIG_NUM];
				NCFG_ITEM_HEAD *pHead = item;
				pHead->itemID = NCFG_ITEM_DATE_FORMAT;
				pHead->len = sizeof(unsigned long);
				pHead->num = 1;
				pHead->subLen = sizeof(unsigned long);
				memcpy(pTempCmdData, pHead, sizeof(NCFG_ITEM_HEAD));
				pHead++;
				pTempCmdData += sizeof(NCFG_ITEM_HEAD);

				pHead->itemID = NCFG_ITEM_HOUR_FORMAT;
				pHead->len = sizeof(unsigned long);
				pHead->num = 1;
				pHead->subLen = sizeof(unsigned long);
				memcpy(pTempCmdData, pHead, sizeof(NCFG_ITEM_HEAD));
				pHead++;
				pTempCmdData += sizeof(NCFG_ITEM_HEAD);

				pHead->itemID = NCFG_ITEM_TIME_ZONE;
				pHead->len = sizeof(unsigned long);
				pHead->num = 1;
				pHead->subLen = sizeof(unsigned long);
				memcpy(pTempCmdData, pHead, sizeof(NCFG_ITEM_HEAD));
				pHead++;
				pTempCmdData += sizeof(NCFG_ITEM_HEAD);

				pHead->itemID = NCFG_ITEM_TIME_SYNC;
				pHead->len = sizeof(unsigned long);
				pHead->num = 1;
				pHead->subLen = sizeof(unsigned long);
				memcpy(pTempCmdData, pHead, sizeof(NCFG_ITEM_HEAD));
				pHead++;
				pTempCmdData += sizeof(NCFG_ITEM_HEAD);

				pHead->itemID = NCFG_ITEM_TIME_SERVER;
				pHead->len = sizeof(NCFG_INFO_LONG_NAME);
				pHead->num = 1;
				pHead->subLen = sizeof(NCFG_INFO_LONG_NAME);
				memcpy(pTempCmdData, pHead, sizeof(NCFG_ITEM_HEAD));
				pHead++;
				pTempCmdData += sizeof(NCFG_ITEM_HEAD);

				pHead->itemID = NCFG_ITEM_NTP_PORT;
				pHead->len = sizeof(unsigned long);
				pHead->num = 1;
				pHead->subLen = sizeof(unsigned long);
				memcpy(pTempCmdData, pHead, sizeof(NCFG_ITEM_HEAD));
				pTempCmdData += sizeof(NCFG_ITEM_HEAD);
				
				DD_DATE_TIME_CONFIG* pDateTimeConfig =  (DD_DATE_TIME_CONFIG*)pItemData;

				memcpy(pTempCmdData, &(pDateTimeConfig->dateFormat), sizeof(unsigned char));
				pTempCmdData += sizeof(unsigned long);
				memcpy(pTempCmdData, &(pDateTimeConfig->timeFormat), sizeof(unsigned char));
				pTempCmdData += sizeof(unsigned long);
				memcpy(pTempCmdData, &(pDateTimeConfig->timeZone), sizeof(unsigned char));
				pTempCmdData += sizeof(unsigned long);
				memcpy(pTempCmdData, &(pDateTimeConfig->enableNTP), sizeof(unsigned char));
				pTempCmdData += sizeof(unsigned long);

				NCFG_INFO_LONG_NAME info_long_name;
				strcpy(info_long_name.name, pDateTimeConfig->ntpServerAddr);
				memcpy(pTempCmdData, info_long_name.name, sizeof(NCFG_INFO_LONG_NAME));
				pTempCmdData += sizeof(NCFG_INFO_LONG_NAME);
				memcpy(pTempCmdData, &(pDateTimeConfig->ntpPort), sizeof(unsigned short));
				
				ITEM_INFO item_info;
				item_info.parserLen = len;
				item_info.pConfigItem = (unsigned char *)pbuf;
				iteminfolist.push_back(item_info);
				datalen += len;

				break;
			}
		case DD_CONFIG_ITEM_DAYLIGHT_INFO:
			{
				unsigned long len = sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD)*DD_DATE_TIME_CONFIG_NUM + sizeof(DAYLIGHT_INFO);
				unsigned char *pbuf = new unsigned char[len];
				memset(pbuf, 0 ,sizeof(unsigned char)*len);
				unsigned char *pTempCmdData = pbuf;

				blockHead.ItemNum = DD_DAYLIGHT_INFO_NUM;
				memcpy(pTempCmdData, &blockHead, sizeof(NCFG_BLOCK_HEAD));
				pTempCmdData += sizeof(NCFG_BLOCK_HEAD);
				//填充配置项头
				NCFG_ITEM_HEAD item[DD_DATE_TIME_CONFIG_NUM];
				NCFG_ITEM_HEAD *pHead = item;
				pHead->itemID = NCFG_ITEM_DAYLIGHT_INFO;
				pHead->len = sizeof(DAYLIGHT_INFO);
				pHead->num = 1;
				pHead->subLen = sizeof(DAYLIGHT_INFO);
				memcpy(pTempCmdData, pHead, sizeof(NCFG_ITEM_HEAD));
				pTempCmdData += sizeof(NCFG_ITEM_HEAD);

				DD_DAYLIGHT_INFO* pDayLight =  (DD_DAYLIGHT_INFO*)pItemData;
				DAYLIGHT_INFO daylight;
				daylight.enable = pDayLight->enable;
				daylight.InMday = pDayLight->InMday;
				daylight.InMonth = pDayLight->InMonth;
				daylight.InSecond = pDayLight->InSecond;
				daylight.InWday = pDayLight->InWday;
				daylight.InWeekIndex = pDayLight->InWeekIndex;
				daylight.InYear = pDayLight->InYear;
				daylight.offSet = pDayLight->offSet;
				daylight.OutMday = pDayLight->OutMday;
				daylight.OutMonth= pDayLight->OutMonth;
				daylight.OutSecond = pDayLight->OutSecond;
				daylight.OutWday = pDayLight->OutWday;
				daylight.OutWeekIndex = pDayLight->OutWeekIndex;
				daylight.OutYear = pDayLight->OutYear;
				daylight.type = pDayLight->type;
				memcpy(pTempCmdData, &daylight, sizeof(DAYLIGHT_INFO));

				ITEM_INFO item_info;
				item_info.parserLen = len;
				item_info.pConfigItem = (unsigned char *)pbuf;
				iteminfolist.push_back(item_info);
				datalen += len;
				
				break;
			}
		case DD_CONFIG_ITEM_LIVE_MAIN_CAMERA:
			{
				DD_LIVE_VIDEO_GROUP* plivegroup =  (DD_LIVE_VIDEO_GROUP*)pItemData;
				double cameraNum = m_deiviceInfo.videoInputNum;
				
				unsigned short splitmode = NCFG_ITEM_VIEW_SPLIT_1X1;
				unsigned long splitlen = 32*1*1+2;
				unsigned long totalGroupNum = 32;
				unsigned long validGroupNum = ceil(cameraNum / 1);

				if (DD_VIEW_SPLIT_1X1 == plivegroup->splitMode)
				{
					splitmode = NCFG_ITEM_VIEW_SPLIT_1X1;
					splitlen = 32*1*1+2;
					totalGroupNum = 32;
					validGroupNum = ceil(cameraNum / 1);
				}
				else if (DD_VIEW_SPLIT_2X2 == plivegroup->splitMode)
				{
					splitmode = NCFG_ITEM_VIEW_SPLIT_2X2;
					splitlen = 8*2*2+2;
					totalGroupNum = 8;
					validGroupNum = ceil(cameraNum / 4);
				}
				else if (DD_VIEW_SPLIT_2X3 == plivegroup->splitMode)
				{
					splitmode = NCFG_ITEM_VIEW_SPLIT_2X3;
					splitlen = 11*2*3+2;
					totalGroupNum = 11;
					validGroupNum = ceil(cameraNum / 6);
				}
				else if (DD_VIEW_SPLIT_3X3 == plivegroup->splitMode)
				{
					splitmode = NCFG_ITEM_VIEW_SPLIT_3X3;
					splitlen = 6*3*3+2;
					totalGroupNum = 6;
					validGroupNum = ceil(cameraNum / 9);
				}
				else if (DD_VIEW_SPLIT_4X4 == plivegroup->splitMode)
				{
					splitmode = NCFG_ITEM_VIEW_SPLIT_4X4;
					splitlen = 4*4*4+2;
					totalGroupNum = 4;
					validGroupNum = ceil(cameraNum / 16);
				}
				else if (DD_VIEW_SPLIT_5X5 == plivegroup->splitMode)
				{
					splitmode = NCFG_ITEM_VIEW_SPLIT_5X5;
					splitlen = 4*5*5+2;
					totalGroupNum = 4;
					validGroupNum = ceil(cameraNum / 20);
				}
				else if (DD_VIEW_SPLIT_6X6 == plivegroup->splitMode)
				{
					splitmode = NCFG_ITEM_VIEW_SPLIT_6X6;
					splitlen = 3*6*6+2;
					totalGroupNum = 3;
					validGroupNum = ceil(cameraNum / 36);
				}
				else
				{
					splitmode = NCFG_ITEM_VIEW_SPLIT_1X1;
					splitlen = 32*1*1+2;
					totalGroupNum = 32;
					validGroupNum = ceil(cameraNum / 1);
				}

				unsigned long len = sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD) * 3 + sizeof(unsigned long) + sizeof(unsigned long) + sizeof(unsigned long) * splitlen;
				unsigned char *pbuf = new unsigned char[len];
				memset(pbuf, 0 ,sizeof(unsigned char)*len);
				unsigned char *pTempCmdData = pbuf;
				blockHead.ItemNum = 3;
				memcpy(pTempCmdData, &blockHead, sizeof(NCFG_BLOCK_HEAD));
				pTempCmdData += sizeof(NCFG_BLOCK_HEAD);
				//填充配置项头
				NCFG_ITEM_HEAD item[3];
				NCFG_ITEM_HEAD *pHead = item;
				
				pHead->itemID = NCFG_ITEM_VIEW_SPLIT_MODE;
				pHead->len = sizeof(unsigned long);
				pHead->num = 1;
				pHead->subLen = sizeof(unsigned long);
				memcpy(pTempCmdData, pHead, sizeof(NCFG_ITEM_HEAD));
				pHead++;
				pTempCmdData += sizeof(NCFG_ITEM_HEAD);
				
				pHead->itemID = NCFG_ITEM_VIEW_DWELL_TIME;
				pHead->len = sizeof(unsigned long);
				pHead->num = 1;
				pHead->subLen = sizeof(unsigned long);
				memcpy(pTempCmdData, pHead, sizeof(NCFG_ITEM_HEAD));
				pHead++;
				pTempCmdData += sizeof(NCFG_ITEM_HEAD);

				pHead->itemID = splitmode;
				pHead->len = sizeof(unsigned long) * splitlen;
				pHead->num = 1;
				pHead->subLen = sizeof(unsigned long) * splitlen;
				memcpy(pTempCmdData, pHead, sizeof(NCFG_ITEM_HEAD));
				pHead++;
				pTempCmdData += sizeof(NCFG_ITEM_HEAD);
				
				memcpy(pTempCmdData, &plivegroup->splitMode, sizeof(unsigned long));
				pTempCmdData += sizeof(unsigned long);

				memcpy(pTempCmdData, &(plivegroup->holdTime), sizeof(unsigned short));
				pTempCmdData += sizeof(unsigned long);

				memcpy(pTempCmdData, &totalGroupNum, sizeof(unsigned long));
				pTempCmdData += sizeof(unsigned long);

				memcpy(pTempCmdData, &validGroupNum, sizeof(unsigned long));
				pTempCmdData += sizeof(unsigned long);

				unsigned long *data = (unsigned long *)pTempCmdData;
				memset(data, 0xffffffff, sizeof(unsigned long) * (splitlen - 2));
				for (int i = 0; i < validGroupNum; i++)
				{
					for (int j = 0; j < cameraNum; j++)
					{
						if (plivegroup[i].channel[j] != 0xff)
						{
							data[plivegroup[i].channel[j]] = j;
						}
					}
					data += DDSplitModeToNum((DD_VIEW_SPLIT_MODE)plivegroup->splitMode);
				}

				ITEM_INFO item_info;
				item_info.parserLen = len;
				item_info.pConfigItem = (unsigned char *)pbuf;
				iteminfolist.push_back(item_info);
				datalen += len;

				break;
			}
		case DD_CONFIG_ITEM_LIVE_SPOT_CAMERA:
			{
				DD_LIVE_VIDEO_GROUP* plivegroup =  (DD_LIVE_VIDEO_GROUP*)pItemData;
				double cameraNum = m_deiviceInfo.videoInputNum;
				
				unsigned short splitmode = NCFG_ITEM_SPOT_SPLIT_1X1;
				unsigned long splitlen = 32*1*1+2;
				unsigned long totalGroupNum = 32;
				unsigned long validGroupNum = ceil(cameraNum / 1);

				if (DD_VIEW_SPLIT_1X1 == plivegroup->splitMode)
				{
					splitmode = NCFG_ITEM_SPOT_SPLIT_1X1;
					splitlen = 32*1*1+2;
					totalGroupNum = 32;
					validGroupNum = ceil(cameraNum / 1);
				}
				else if (DD_VIEW_SPLIT_2X2 == plivegroup->splitMode)
				{
					splitmode = NCFG_ITEM_VIEW_SPLIT_2X2;
					splitlen = 8*2*2+2;
					totalGroupNum = 8;
					validGroupNum = ceil(cameraNum / 4);
				}
				else if (DD_VIEW_SPLIT_2X3 == plivegroup->splitMode)
				{
					splitmode = NCFG_ITEM_VIEW_SPLIT_2X3;
					splitlen = 11*2*3+2;
					totalGroupNum = 11;
					validGroupNum = ceil(cameraNum / 6);
				}
				else if (DD_VIEW_SPLIT_3X3 == plivegroup->splitMode)
				{
					splitmode = NCFG_ITEM_VIEW_SPLIT_3X3;
					splitlen = 6*3*3+2;
					totalGroupNum = 6;
					validGroupNum = ceil(cameraNum / 9);
				}
				else if (DD_VIEW_SPLIT_4X4 == plivegroup->splitMode)
				{
					splitmode = NCFG_ITEM_VIEW_SPLIT_4X4;
					splitlen = 4*4*4+2;
					totalGroupNum = 4;
					validGroupNum = ceil(cameraNum / 16);
				}
				else if (DD_VIEW_SPLIT_5X5 == plivegroup->splitMode)
				{
					splitmode = NCFG_ITEM_VIEW_SPLIT_5X5;
					splitlen = 4*5*5+2;
					totalGroupNum = 4;
					validGroupNum = ceil(cameraNum / 20);
				}
				else if (DD_VIEW_SPLIT_6X6 == plivegroup->splitMode)
				{
					splitmode = NCFG_ITEM_VIEW_SPLIT_6X6;
					splitlen = 3*6*6+2;
					totalGroupNum = 3;
					validGroupNum = ceil(cameraNum / 36);
				}
				else
				{
					splitmode = NCFG_ITEM_SPOT_SPLIT_1X1;
					splitlen = 32*1*1+2;
					totalGroupNum = 32;
					validGroupNum = ceil(cameraNum / 1);
				}

				unsigned long len = sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD) * 3 + sizeof(unsigned long) + sizeof(unsigned long) + sizeof(unsigned long) * splitlen;
				unsigned char *pbuf = new unsigned char[len];
				memset(pbuf, 0 ,sizeof(unsigned char)*len);
				unsigned char *pTempCmdData = pbuf;
				blockHead.ItemNum = 3;
				memcpy(pTempCmdData, &blockHead, sizeof(NCFG_BLOCK_HEAD));
				pTempCmdData += sizeof(NCFG_BLOCK_HEAD);
				//填充配置项头
				NCFG_ITEM_HEAD item[3];
				NCFG_ITEM_HEAD *pHead = item;

				pHead->itemID = NCFG_ITEM_SPOT_SPLIT_MODE;
				pHead->len = sizeof(unsigned long);
				pHead->num = 1;
				pHead->subLen = sizeof(unsigned long);
				memcpy(pTempCmdData, pHead, sizeof(NCFG_ITEM_HEAD));
				pHead++;
				pTempCmdData += sizeof(NCFG_ITEM_HEAD);

				pHead->itemID = NCFG_ITEM_SPOT_DWELL_TIME;
				pHead->len = sizeof(unsigned long);
				pHead->num = 1;
				pHead->subLen = sizeof(unsigned long);
				memcpy(pTempCmdData, pHead, sizeof(NCFG_ITEM_HEAD));
				pHead++;
				pTempCmdData += sizeof(NCFG_ITEM_HEAD);

				pHead->itemID = splitmode;
				pHead->len = sizeof(unsigned long) * splitlen;
				pHead->num = 1;
				pHead->subLen = sizeof(unsigned long) * splitlen;
				memcpy(pTempCmdData, pHead, sizeof(NCFG_ITEM_HEAD));
				pHead++;
				pTempCmdData += sizeof(NCFG_ITEM_HEAD);

				memcpy(pTempCmdData, &plivegroup->splitMode, sizeof(unsigned long));
				pTempCmdData += sizeof(unsigned long);

				memcpy(pTempCmdData, &(plivegroup->holdTime), sizeof(unsigned short));
				pTempCmdData += sizeof(unsigned long);

				memcpy(pTempCmdData, &totalGroupNum, sizeof(unsigned long));
				pTempCmdData += sizeof(unsigned long);

				memcpy(pTempCmdData, &validGroupNum, sizeof(unsigned long));
				pTempCmdData += sizeof(unsigned long);

				unsigned long *data = (unsigned long *)pTempCmdData;
				memset(data, 0xffffffff, sizeof(unsigned long) * (splitlen - 2));
				for (int i = 0; i < validGroupNum; i++)
				{
					for (int j = 0; j < cameraNum; j++)
					{
						if (plivegroup[i].channel[j] != 0xff)
						{
							data[plivegroup[i].channel[j]] = j;
						}
					}
					data += DDSplitModeToNum((DD_VIEW_SPLIT_MODE)plivegroup->splitMode);
				}

				ITEM_INFO item_info;
				item_info.parserLen = len;
				item_info.pConfigItem = (unsigned char *)pbuf;
				iteminfolist.push_back(item_info);
				datalen += len;
				break;
			}
		case DD_CONFIG_ITEM_LIVE_AUDIO:
			{
				unsigned long len = sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD)*DD_LIVE_AUDIO_GROUP_NUM + sizeof(unsigned long)*DD_LIVE_AUDIO_GROUP_NUM;
				unsigned char *pbuf = new unsigned char[len];
				memset(pbuf, 0 ,sizeof(unsigned char)*len);
				unsigned char *pTempCmdData = pbuf;

				blockHead.ItemNum = DD_LIVE_AUDIO_GROUP_NUM;
				memcpy(pTempCmdData, &blockHead, sizeof(NCFG_BLOCK_HEAD));
				pTempCmdData += sizeof(NCFG_BLOCK_HEAD);
				//填充配置项头
				NCFG_ITEM_HEAD item[DD_LIVE_AUDIO_GROUP_NUM];
				NCFG_ITEM_HEAD *pHead = item;
				pHead->itemID = NCFG_ITEM_LIVE_AUDIO_CHNN;
				pHead->len = sizeof(unsigned long);
				pHead->num = m_deiviceInfo.audioInputNum;
				pHead->subLen = sizeof(unsigned long);
				memcpy(pTempCmdData, pHead, sizeof(NCFG_ITEM_HEAD));
				pHead++;
				pTempCmdData += sizeof(NCFG_ITEM_HEAD);

				pHead->itemID = NCFG_ITEM_LIVE_AUDIO_VOLUME;
				pHead->len = sizeof(unsigned long);
				pHead->num = m_deiviceInfo.audioInputNum;
				pHead->subLen = sizeof(unsigned long);
				memcpy(pTempCmdData, pHead, sizeof(NCFG_ITEM_HEAD));
				pHead++;
				pTempCmdData += sizeof(NCFG_ITEM_HEAD);
				
				DD_LIVE_AUDIO_GROUP* plivegroup =  (DD_LIVE_AUDIO_GROUP*)pItemData;


				memcpy(pTempCmdData, &(plivegroup->channel), sizeof(unsigned char));
				pTempCmdData += sizeof(unsigned long);
				
				memcpy(pTempCmdData, &(plivegroup->volume), sizeof(unsigned char));
				pTempCmdData += sizeof(unsigned long);

				ITEM_INFO item_info;
				item_info.parserLen = len;
				item_info.pConfigItem = (unsigned char *)pbuf;
				iteminfolist.push_back(item_info);
				datalen += len;

				break;
			}
		case DD_CONFIG_ITEM_LIVE_DISPLAY:
			{
				unsigned long len = sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD)*DD_LIVE_DISPLAY_NUM + sizeof(unsigned long)*(DD_LIVE_DISPLAY_NUM - 1) + sizeof(OSD_INFO) + sizeof(unsigned long) * m_deiviceInfo
					.videoInputNum;
				unsigned char *pbuf = new unsigned char[len];
				memset(pbuf, 0 ,sizeof(unsigned char)*len);
				unsigned char *pTempCmdData = pbuf;

				blockHead.ItemNum = DD_LIVE_DISPLAY_NUM;
				memcpy(pTempCmdData, &blockHead, sizeof(NCFG_BLOCK_HEAD));
				pTempCmdData += sizeof(NCFG_BLOCK_HEAD);
				//填充配置项头
				NCFG_ITEM_HEAD item[DD_LIVE_DISPLAY_NUM];
				NCFG_ITEM_HEAD *pHead = item;
				pHead->itemID = NCFG_ITEM_LIVE_TIME_INFO;
				pHead->len = sizeof(OSD_INFO);
				pHead->num = 1;
				pHead->subLen = sizeof(OSD_INFO);
				memcpy(pTempCmdData, pHead, sizeof(NCFG_ITEM_HEAD));
				pHead++;
				pTempCmdData += sizeof(NCFG_ITEM_HEAD);

				pHead->itemID = NCFG_ITEM_LIVE_REC_STATUS;
				pHead->len = sizeof(unsigned long);
				pHead->num = 1;
				pHead->subLen = sizeof(unsigned long);
				memcpy(pTempCmdData, pHead, sizeof(NCFG_ITEM_HEAD));
				pTempCmdData += sizeof(NCFG_ITEM_HEAD);
				
				pHead->itemID = NCFG_ITEM_SHOW_CAM_NAME;
				pHead->len = sizeof(unsigned long) * m_deiviceInfo.videoInputNum;
				pHead->num = m_deiviceInfo.videoInputNum;
				pHead->subLen = sizeof(unsigned long);
				memcpy(pTempCmdData, pHead, sizeof(NCFG_ITEM_HEAD));
				pTempCmdData += sizeof(NCFG_ITEM_HEAD);

				DD_LIVE_DISPLAY* pLiveDisplay =  (DD_LIVE_DISPLAY*)pItemData;
				OSD_INFO osd_info;
				osd_info.bEnable = pLiveDisplay->showTime;
				memcpy(pTempCmdData, &osd_info, sizeof(OSD_INFO));
				pTempCmdData += sizeof(OSD_INFO);

				unsigned long status = pLiveDisplay->showRecordStatus[0];
				memcpy(pTempCmdData, &status, sizeof(unsigned long));
				pTempCmdData += sizeof(unsigned long);

				for (int i = 0; i < m_deiviceInfo.videoInputNum; i++)
				{
					memcpy(pTempCmdData, &pLiveDisplay->showCameraName[i], sizeof(unsigned char));
					pTempCmdData += sizeof(unsigned long);
				}
				
				ITEM_INFO item_info;
				item_info.parserLen = len;
				item_info.pConfigItem = (unsigned char *)pbuf;
				iteminfolist.push_back(item_info);
				datalen += len;
			
				break;
			}
		case DD_CONFIG_ITEM_CHNN_CONFIG:
			{
				unsigned long len = sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD)*DD_CHANNEL_CONFIG_NUM + sizeof(unsigned long)*m_deiviceInfo.videoInputNum + sizeof(NCFG_INFO_SHORT_NAME)*m_deiviceInfo.videoInputNum;
				unsigned char *pbuf = new unsigned char[len];
				memset(pbuf, 0 ,sizeof(unsigned char)*len);
				unsigned char *pTempCmdData = pbuf;

				blockHead.ItemNum = DD_CHANNEL_CONFIG_NUM;
				memcpy(pTempCmdData, &blockHead, sizeof(NCFG_BLOCK_HEAD));
				pTempCmdData += sizeof(NCFG_BLOCK_HEAD);
				//填充配置项头
				NCFG_ITEM_HEAD item[DD_CHANNEL_CONFIG_NUM];
				NCFG_ITEM_HEAD *pHead = item;
				pHead->itemID = NCFG_ITEM_CAMERA_NAME;
				pHead->len = sizeof(NCFG_INFO_SHORT_NAME)*m_deiviceInfo.videoInputNum;
				pHead->num = m_deiviceInfo.videoInputNum;
				pHead->subLen = sizeof(NCFG_INFO_SHORT_NAME);
				memcpy(pTempCmdData, pHead, sizeof(NCFG_ITEM_HEAD));
				pHead++;
				pTempCmdData += sizeof(NCFG_ITEM_HEAD);

				pHead->itemID = NCFG_ITEM_SHOW_CAM_NAME;
				pHead->len = sizeof(unsigned long)*m_deiviceInfo.videoInputNum;
				pHead->num = m_deiviceInfo.videoInputNum;
				pHead->subLen = sizeof(unsigned long);
				memcpy(pTempCmdData, pHead, sizeof(NCFG_ITEM_HEAD));
				pTempCmdData += sizeof(NCFG_ITEM_HEAD);

				DD_CHANNEL_CONFIG* pChannel =  (DD_CHANNEL_CONFIG*)pItemData;
				NCFG_INFO_SHORT_NAME short_name[DD_MAX_CAMERA_NUM];
				for (int i = 0; i < m_deiviceInfo.videoInputNum; i++)
				{	
					strcpy(short_name[i].name, pChannel[i].name);
					memcpy(pTempCmdData, &short_name[i], sizeof(NCFG_INFO_SHORT_NAME));
					pTempCmdData += sizeof(NCFG_INFO_SHORT_NAME);
					
				}
				for (int i = 0; i < m_deiviceInfo.videoInputNum; i++)
				{
					memcpy(pTempCmdData, &(pChannel[i].hide), sizeof(unsigned long));
					pTempCmdData += sizeof(unsigned long);
				}
				ITEM_INFO item_info;
				item_info.itemID = DD_CONFIG_ITEM_CHNN_CONFIG;
				item_info.parserLen = len;
				item_info.pConfigItem = (unsigned char *)pbuf;
				iteminfolist.push_back(item_info);
				datalen += len;

				break;
			}
		case DD_CONFIG_ITEM_VIDEO_COLOR:
			{
				break;
			}
		case DD_CONFIG_ITEM_VIDEO_OSD:
			{
				unsigned long len = sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD)*DD_VIDEO_OSD_CONFIG_NUM + sizeof(OSD_INFO)*(DD_VIDEO_OSD_CONFIG_NUM);
				unsigned char *pbuf = new unsigned char[len];
				memset(pbuf, 0 ,sizeof(unsigned char)*len);
				unsigned char *pTempCmdData = pbuf;

				blockHead.ItemNum = DD_VIDEO_OSD_CONFIG_NUM; 
				memcpy(pTempCmdData, &blockHead, sizeof(NCFG_BLOCK_HEAD));
				pTempCmdData += sizeof(NCFG_BLOCK_HEAD);
				//填充配置项头
				NCFG_ITEM_HEAD item[DD_VIDEO_OSD_CONFIG_NUM];
				NCFG_ITEM_HEAD *pHead = item;
				pHead->itemID = NCFG_ITEM_TIME_STAMP_INFO;
				pHead->len = sizeof(OSD_INFO);
				pHead->num = 1;
				pHead->subLen = sizeof(OSD_INFO);
				memcpy(pTempCmdData, pHead, sizeof(NCFG_ITEM_HEAD));
				pHead++;
				pTempCmdData += sizeof(NCFG_ITEM_HEAD);

				pHead->itemID = NCFG_ITEM_CAM_NAME_STAMP_INFO;
				pHead->len = sizeof(OSD_INFO);
				pHead->num = 1;
				pHead->subLen = sizeof(OSD_INFO);
				memcpy(pTempCmdData, pHead, sizeof(NCFG_ITEM_HEAD));
				pTempCmdData += sizeof(NCFG_ITEM_HEAD);

				DD_VIDEO_OSD_CONFIG* pLiveDisplay =  (DD_VIDEO_OSD_CONFIG*)pItemData;
				OSD_INFO time_osd_info;
				time_osd_info.x = pLiveDisplay->timeStamp.x;
				time_osd_info.y = pLiveDisplay->timeStamp.y;
				memcpy(pTempCmdData, &time_osd_info, sizeof(OSD_INFO));
				pTempCmdData += sizeof(OSD_INFO);
				OSD_INFO cam_osd_info;
				cam_osd_info.x = pLiveDisplay->cameraName.x;
				cam_osd_info.y = pLiveDisplay->cameraName.y;
				memcpy(pTempCmdData, &cam_osd_info, sizeof(OSD_INFO));

				ITEM_INFO item_info;
				item_info.parserLen = len;
				item_info.pConfigItem = (unsigned char *)pbuf;
				iteminfolist.push_back(item_info);
				datalen += len;

				break;
			}
		case DD_CONFIG_ITEM_ENCODE_MASK_MAJOR:
			{
				unsigned long len = sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD)*DD_ENCODE_CONFIG_NUM + sizeof(ENCODE_INFO)*(DD_ENCODE_CONFIG_NUM);
				unsigned char *pTempCmdData = new unsigned char[len];
				memset(pTempCmdData, 0 ,sizeof(unsigned char)*len);
				blockHead.ItemNum = DD_ENCODE_CONFIG_NUM;
				memcpy(pTempCmdData, &blockHead, sizeof(NCFG_BLOCK_HEAD));
				pTempCmdData += sizeof(NCFG_BLOCK_HEAD);
				//填充配置项头
				NCFG_ITEM_HEAD item[DD_ENCODE_CONFIG_NUM];
				NCFG_ITEM_HEAD *pHead = item;
				pHead->itemID = NCFG_ITEM_ENCODE_MAJOR_EX;
				pHead->len = sizeof(ENCODE_INFO_EX);
				pHead->num = 1;
				pHead->subLen = sizeof(ENCODE_INFO_EX);
				memcpy(pTempCmdData, pHead, sizeof(NCFG_ITEM_HEAD));
				pTempCmdData += sizeof(NCFG_ITEM_HEAD);

				DD_ENCODE_CONFIG* pEncodeConfig =  (DD_ENCODE_CONFIG*)pItemData;
				ENCODE_INFO_EX encode_info;
				encode_info.resolution = pEncodeConfig->resolution;
				encode_info.rate = pEncodeConfig->rate;
				encode_info.quality = pEncodeConfig->quality;
				encode_info.encodeType = pEncodeConfig->encodeType;
				encode_info.hBitStream = pEncodeConfig->maxBitrate;
				encode_info.lBitStream = pEncodeConfig->minBitrate;

				memcpy(pTempCmdData, &encode_info, sizeof(ENCODE_INFO_EX));

				ITEM_INFO item_info;
				item_info.parserLen = len;
				item_info.pConfigItem = (unsigned char *)pTempCmdData;
				iteminfolist.push_back(item_info);
				datalen += len;

				break;
			}
		case DD_CONFIG_ITEM_ENCODE_MASK_MINOR:
			{
				unsigned long len = sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD)*DD_ENCODE_CONFIG_NUM + sizeof(ENCODE_INFO)*(DD_ENCODE_CONFIG_NUM);
				unsigned char *pTempCmdData = new unsigned char[len];
				memset(pTempCmdData, 0 ,sizeof(unsigned char)*len);
				blockHead.ItemNum = DD_ENCODE_CONFIG_NUM;
				memcpy(pTempCmdData, &blockHead, sizeof(NCFG_BLOCK_HEAD));
				pTempCmdData += sizeof(NCFG_BLOCK_HEAD);
				//填充配置项头
				NCFG_ITEM_HEAD item[DD_ENCODE_CONFIG_NUM];
				NCFG_ITEM_HEAD *pHead = item;
				pHead->itemID = NCFG_ITEM_ENCODE_MINOR_EX;
				pHead->len = sizeof(ENCODE_INFO);
				pHead->num = 1;
				pHead->subLen = sizeof(ENCODE_INFO);
				memcpy(pTempCmdData, pHead, sizeof(NCFG_ITEM_HEAD));
				pTempCmdData += sizeof(NCFG_ITEM_HEAD);

				DD_ENCODE_CONFIG* pEncodeConfig =  (DD_ENCODE_CONFIG*)pItemData;
				ENCODE_INFO encode_info;
				encode_info.resolution = (unsigned char)pEncodeConfig->resolution;
				encode_info.rate = (unsigned char)pEncodeConfig->rate;
				encode_info.quality = (unsigned char)pEncodeConfig->quality;
				encode_info.encodeType = (unsigned char)pEncodeConfig->encodeType;
				encode_info.hBitStream = pEncodeConfig->maxBitrate;
				encode_info.lBitStream = pEncodeConfig->minBitrate;

				memcpy(pTempCmdData, &encode_info, sizeof(ENCODE_INFO));

				ITEM_INFO item_info;
				item_info.parserLen = len;
				item_info.pConfigItem = (unsigned char *)pTempCmdData;
				iteminfolist.push_back(item_info);
				datalen += len;

				break;
			}
		case DD_CONFIG_ITEM_ENCODE_SCHEDULE:
			{
				unsigned long len = sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD)*2 \
					+ sizeof(ENCODE_INFO) * m_localVideoNum \
					+ sizeof(ENCODE_INFO_EX) * m_deiviceInfo.videoInputNum;

				unsigned char *pbuf = new unsigned char[len];
				memset(pbuf, 0 ,sizeof(unsigned char)*len);
				unsigned char *pTempCmdData = pbuf;
				blockHead.ItemNum = 2;
				memcpy(pTempCmdData, &blockHead, sizeof(NCFG_BLOCK_HEAD));
				pTempCmdData += sizeof(NCFG_BLOCK_HEAD);

				NCFG_ITEM_HEAD itemHead[2];
				itemHead[0].itemID = NCFG_ITEM_ENCODE_MAJOR;
				itemHead[0].len = sizeof(ENCODE_INFO) * m_localVideoNum;
				itemHead[0].num = m_localVideoNum;
				itemHead[0].subLen = sizeof(ENCODE_INFO);
				itemHead[1].itemID = NCFG_ITEM_ENCODE_MAJOR_EX;
				itemHead[1].len = sizeof(ENCODE_INFO_EX) * m_deiviceInfo.videoInputNum;
				itemHead[1].num = m_deiviceInfo.videoInputNum;
				itemHead[1].subLen = sizeof(ENCODE_INFO_EX);
				memcpy(pTempCmdData, itemHead, sizeof(NCFG_ITEM_HEAD)*2);
				pTempCmdData += sizeof(NCFG_ITEM_HEAD)*2;

				ENCODE_INFO *ei = (ENCODE_INFO *)pTempCmdData;
				DD_ENCODE_CONFIG *ec = (DD_ENCODE_CONFIG *)pItemData;

				for (int i = 0; i < m_localVideoNum; i++)
				{	
					ei[i].encodeType = ec[i].encodeType;
					ei[i].quality = ec[i].quality;
					ei[i].rate = ec[i].rate;
					ei[i].resolution = DDVideoSizeToVideoSize(ec[i].resolution);
					ei[i].hBitStream = ec[i].maxBitrate;
					ei[i].lBitStream = ec[i].minBitrate;
				}

				ENCODE_INFO_EX *pEncodeInfoEx = (ENCODE_INFO_EX *)(pTempCmdData+sizeof(ENCODE_INFO)*m_localVideoNum);
				for(int i = 0; i < m_deiviceInfo.videoInputNum; i++)
				{
					pEncodeInfoEx[i].encodeType = ec[i].encodeType;
					pEncodeInfoEx[i].quality = ec[i].quality;
					pEncodeInfoEx[i].rate = ec[i].rate;
					pEncodeInfoEx[i].resolution = DDVideoSizeToVideoSize(ec[i].resolution);
					pEncodeInfoEx[i].hBitStream = ec[i].maxBitrate;
					pEncodeInfoEx[i].lBitStream = ec[i].minBitrate;
				}
				
				ITEM_INFO item_info;
				item_info.parserLen = len;
				item_info.pConfigItem = (unsigned char *)pbuf;
				iteminfolist.push_back(item_info);
				datalen += len;
			}
		case DD_CONFIG_ITEM_ENCODE_ALARM:
			{
				break;
			}
		case DD_CONFIG_ITEM_ENCODE_NETWORK:
			{
				unsigned long len = sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD)*DD_ENCODE_CONFIG_NUM + sizeof(ENCODE_INFO_EX) * m_deiviceInfo.videoInputNum;
				unsigned char *pbuf = new unsigned char[len];
				memset(pbuf, 0 ,sizeof(unsigned char)*len);
				unsigned char *pTempCmdData = pbuf;
				blockHead.ItemNum = DD_ENCODE_CONFIG_NUM;
				memcpy(pTempCmdData, &blockHead, sizeof(NCFG_BLOCK_HEAD));
				pTempCmdData += sizeof(NCFG_BLOCK_HEAD);

				NCFG_ITEM_HEAD item[DD_ENCODE_CONFIG_NUM];
				NCFG_ITEM_HEAD *pHead = item;
				pHead->itemID = NCFG_ITEM_ENCODE_MINOR_EX;
				pHead->len = sizeof(ENCODE_INFO_EX) * m_deiviceInfo.videoInputNum;
				pHead->num = 1;
				pHead->subLen = sizeof(ENCODE_INFO_EX);
				memcpy(pTempCmdData, pHead, sizeof(NCFG_ITEM_HEAD));
				pTempCmdData += sizeof(NCFG_ITEM_HEAD);

				ENCODE_INFO_EX *ei = (ENCODE_INFO_EX *)pTempCmdData;
				DD_ENCODE_CONFIG *ec = (DD_ENCODE_CONFIG *)pItemData;

				for (int i = 0; i < m_deiviceInfo.videoInputNum; i++)
				{	
					ei[i].encodeType = ec[i].encodeType;
					ei[i].quality = ec[i].quality;
					ei[i].rate = ec[i].rate;
					ei[i].resolution = ec[i].resolution;
					ei[i].hBitStream = ec[i].maxBitrate;
					ei[i].lBitStream = ec[i].minBitrate;
				}

				ITEM_INFO item_info;
				item_info.parserLen = len;
				item_info.pConfigItem = (unsigned char *)pbuf;
				iteminfolist.push_back(item_info);
				datalen += len;
				break;
			}
		case DD_CONFIG_ITEM_RECORD_MASK:
			{
				break;
			}
		case DD_CONFIG_ITEM_RECORD_SETUP:
			{
				unsigned long len = sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD)*DD_RECORD_CONFIG_NUM + sizeof(unsigned long)*(DD_RECORD_CONFIG_NUM-1) * m_deiviceInfo.videoInputNum + sizeof(unsigned long)* m_deiviceInfo.audioInputNum;
				unsigned char *pbuf = new unsigned char[len];
				memset(pbuf, 0 ,sizeof(unsigned char)*len);
				unsigned char *pTempCmdData = pbuf;

				blockHead.ItemNum = DD_RECORD_CONFIG_NUM;
				memcpy(pTempCmdData, &blockHead, sizeof(NCFG_BLOCK_HEAD));
				pTempCmdData += sizeof(NCFG_BLOCK_HEAD);
				//填充配置项头
				NCFG_ITEM_HEAD item[DD_RECORD_CONFIG_NUM];
				NCFG_ITEM_HEAD *pHead = item;
				pHead->itemID = NCFG_ITEM_REC_ENABLE;
				pHead->len = sizeof(unsigned long) * m_deiviceInfo.videoInputNum;
				pHead->num = m_deiviceInfo.videoInputNum;
				pHead->subLen = sizeof(unsigned long);
				memcpy(pTempCmdData, pHead, sizeof(NCFG_ITEM_HEAD));
				pHead++;
				pTempCmdData += sizeof(NCFG_ITEM_HEAD);

				pHead->itemID = NCFG_ITEM_PRE_ALARM_REC_TIME;
				pHead->len = sizeof(unsigned long) * m_deiviceInfo.videoInputNum;
				pHead->num = m_deiviceInfo.videoInputNum;
				pHead->subLen = sizeof(unsigned long);
				memcpy(pTempCmdData, pHead, sizeof(NCFG_ITEM_HEAD));
				pHead++;
				pTempCmdData += sizeof(NCFG_ITEM_HEAD);

				pHead->itemID = NCFG_ITEM_POST_ALARM_REC_TIME;
				pHead->len = sizeof(unsigned long)* m_deiviceInfo.videoInputNum;
				pHead->num = m_deiviceInfo.videoInputNum;
				pHead->subLen = sizeof(unsigned long);
				memcpy(pTempCmdData, pHead, sizeof(NCFG_ITEM_HEAD));
				pHead++;
				pTempCmdData += sizeof(NCFG_ITEM_HEAD);

				pHead->itemID = NCFG_ITEM_REC_HOLD_TIME;
				pHead->len = sizeof(unsigned long) * m_deiviceInfo.videoInputNum;
				pHead->num = m_deiviceInfo.videoInputNum;
				pHead->subLen = sizeof(unsigned long);
				memcpy(pTempCmdData, pHead, sizeof(NCFG_ITEM_HEAD));
				pHead++;
				pTempCmdData += sizeof(NCFG_ITEM_HEAD);

				pHead->itemID = NCFG_ITEM_REC_AUDIO;
				pHead->len = sizeof(unsigned long) * m_deiviceInfo.audioInputNum;
				pHead->num = m_deiviceInfo.audioInputNum;
				pHead->subLen = sizeof(unsigned long);
				memcpy(pTempCmdData, pHead, sizeof(NCFG_ITEM_HEAD));
				pHead++;
				pTempCmdData += sizeof(NCFG_ITEM_HEAD);

				pHead->itemID = NCFG_ITEM_REC_REDUNDANT;
				pHead->len = sizeof(unsigned long) * m_deiviceInfo.videoInputNum;
				pHead->num = m_deiviceInfo.videoInputNum;
				pHead->subLen = sizeof(unsigned long);
				memcpy(pTempCmdData, pHead, sizeof(NCFG_ITEM_HEAD));
				pTempCmdData += sizeof(NCFG_ITEM_HEAD);

				DD_RECORD_CONFIG* pRecordConfig =  (DD_RECORD_CONFIG*)pItemData;
				for (int i = 0; i < m_deiviceInfo.videoInputNum; i++)
				{
					memcpy(pTempCmdData, &(pRecordConfig[i].bOnlyVideo), sizeof(unsigned char));
					pTempCmdData += sizeof(unsigned long);
				}
				for (int i = 0; i < m_deiviceInfo.videoInputNum; i++)
				{
					memcpy(pTempCmdData, &(pRecordConfig[i].preAlarmTime), sizeof(unsigned short));
					pTempCmdData += sizeof(unsigned long);
				}
				for (int i = 0; i < m_deiviceInfo.videoInputNum; i++)
				{	
					memcpy(pTempCmdData, &(pRecordConfig[i].postAlarmTime), sizeof(unsigned short));
					pTempCmdData += sizeof(unsigned long);
				}
				for (int i = 0; i < m_deiviceInfo.videoInputNum; i++)
				{
					unsigned long temp = pRecordConfig[i].expired * 86400;  
					memcpy(pTempCmdData, &temp, sizeof(unsigned long));
					pTempCmdData += sizeof(unsigned long);
				}
				for (int i = 0; i < m_deiviceInfo.audioInputNum; i++)
				{
					memcpy(pTempCmdData, &(pRecordConfig[i].bWithAudio), sizeof(unsigned char));
					pTempCmdData += sizeof(unsigned long);
				}
				for (int i = 0; i < m_deiviceInfo.videoInputNum; i++)
				{
					memcpy(pTempCmdData, &(pRecordConfig[i].bRedundancy), sizeof(unsigned char));
					pTempCmdData += sizeof(unsigned long);
				}
				
				ITEM_INFO item_info;
				item_info.parserLen = len;
				item_info.pConfigItem = (unsigned char *)pbuf;
				iteminfolist.push_back(item_info);
				datalen += len;
			
				break;
			}
		case DD_CONFIG_ITEM_RECORD_SCHEDULE_SCHEDULE:
			{
				unsigned long len = sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD)*DD_WEEK_SCHEDULE_NUM + sizeof(WEEK_SCHEDULE)*DD_WEEK_SCHEDULE_NUM;
				unsigned char *pbuf = new unsigned char[len];
				memset(pbuf, 0 ,sizeof(unsigned char)*len);
				unsigned char *pTempCmdData = pbuf;

				blockHead.ItemNum = DD_WEEK_SCHEDULE_NUM;
				memcpy(pTempCmdData, &blockHead, sizeof(NCFG_BLOCK_HEAD));
				pTempCmdData += sizeof(NCFG_BLOCK_HEAD);
				//填充配置项头
				NCFG_ITEM_HEAD item[DD_WEEK_SCHEDULE_NUM];
				NCFG_ITEM_HEAD *pHead = item;
				pHead->itemID = NCFG_ITEM_REC_SCH_SCHEDULE_WEEK;
				pHead->len = sizeof(WEEK_SCHEDULE);
				pHead->num = 1;
				pHead->subLen = sizeof(WEEK_SCHEDULE);
				memcpy(pTempCmdData, pHead, sizeof(NCFG_ITEM_HEAD));
				pTempCmdData += sizeof(NCFG_ITEM_HEAD);
				
				DD_WEEK_SCHEDULE* pWeekSchedule =  (DD_WEEK_SCHEDULE*)pItemData;
				WEEK_SCHEDULE week_schedule;
				DATE_SCHEDULE *pDate = week_schedule.week;
				DD_DATE_SCHEDULE *pDD_Date = pWeekSchedule->week;
				for (int i = 0; i < 7; i++)
				{
					memcpy(pDate->hour, pDD_Date->hour, sizeof(unsigned long long) * 24);
					pDate++;
					pDD_Date++;
				}
				memcpy(pTempCmdData, &week_schedule, sizeof(WEEK_SCHEDULE));

				ITEM_INFO item_info;
				item_info.parserLen = len;
				item_info.pConfigItem = (unsigned char *)pbuf;
				iteminfolist.push_back(item_info);
				datalen += len;

				break;
			}
		case DD_CONFIG_ITEM_RECORD_SCHEDULE_MOTION:
			{
				unsigned long len = sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD)*DD_WEEK_SCHEDULE_NUM + sizeof(WEEK_SCHEDULE)*DD_WEEK_SCHEDULE_NUM;
				unsigned char *pbuf = new unsigned char[len];
				memset(pbuf, 0 ,sizeof(unsigned char)*len);
				unsigned char *pTempCmdData = pbuf;

				blockHead.ItemNum = DD_WEEK_SCHEDULE_NUM;
				memcpy(pTempCmdData, &blockHead, sizeof(NCFG_BLOCK_HEAD));
				pTempCmdData += sizeof(NCFG_BLOCK_HEAD);
				//填充配置项头
				NCFG_ITEM_HEAD item[DD_WEEK_SCHEDULE_NUM];
				NCFG_ITEM_HEAD *pHead = item;
				pHead->itemID = NCFG_ITEM_REC_SCH_MOTION_WEEK;
				pHead->len = sizeof(WEEK_SCHEDULE);
				pHead->num = 1;
				pHead->subLen = sizeof(WEEK_SCHEDULE);
				memcpy(pTempCmdData, pHead, sizeof(NCFG_ITEM_HEAD));
				pTempCmdData += sizeof(NCFG_ITEM_HEAD);

				DD_WEEK_SCHEDULE* pWeekSchedule =  (DD_WEEK_SCHEDULE*)pItemData;
				WEEK_SCHEDULE week_schedule;
				memcpy(week_schedule.week, pWeekSchedule->week, sizeof(DATE_SCHEDULE) * 7);
				memcpy(pTempCmdData, &week_schedule, sizeof(WEEK_SCHEDULE));

				ITEM_INFO item_info;
				item_info.parserLen = len;
				item_info.pConfigItem = (unsigned char *)pbuf;
				iteminfolist.push_back(item_info);
				datalen += len;

				break;
			}
		case DD_CONFIG_ITEM_RECORD_SCHEDULE_SENSOR:
			{
				unsigned long len = sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD)*DD_WEEK_SCHEDULE_NUM + sizeof(WEEK_SCHEDULE)*DD_WEEK_SCHEDULE_NUM;
				unsigned char *pbuf = new unsigned char[len];
				memset(pbuf, 0 ,sizeof(unsigned char)*len);
				unsigned char *pTempCmdData = pbuf;

				blockHead.ItemNum = DD_WEEK_SCHEDULE_NUM;
				memcpy(pTempCmdData, &blockHead, sizeof(NCFG_BLOCK_HEAD));
				pTempCmdData += sizeof(NCFG_BLOCK_HEAD);
				//填充配置项头
				NCFG_ITEM_HEAD item[DD_WEEK_SCHEDULE_NUM];
				NCFG_ITEM_HEAD *pHead = item;
				pHead->itemID = NCFG_ITEM_REC_SCH_SENSOR_WEEK;
				pHead->len = sizeof(WEEK_SCHEDULE);
				pHead->num = 1;
				pHead->subLen = sizeof(WEEK_SCHEDULE);
				memcpy(pTempCmdData, pHead, sizeof(NCFG_ITEM_HEAD));
				pTempCmdData += sizeof(NCFG_ITEM_HEAD);

				DD_WEEK_SCHEDULE* pWeekSchedule =  (DD_WEEK_SCHEDULE*)pItemData;
				WEEK_SCHEDULE week_schedule;
				memcpy(week_schedule.week, pWeekSchedule->week, sizeof(DATE_SCHEDULE) * 7);
				memcpy(pTempCmdData, &week_schedule, sizeof(WEEK_SCHEDULE));

				ITEM_INFO item_info;
				item_info.parserLen = len;
				item_info.pConfigItem = (unsigned char *)pbuf;
				iteminfolist.push_back(item_info);
				datalen += len;

				break;
			}
		case DD_CONFIG_ITEM_NETWORK_IP:
			{
				unsigned long len = sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD)*DD_NETWORK_IP_CONFIG_NUM + sizeof(PPPOE_INFO) + sizeof(NETWORK_ADDR_INFO) + sizeof(unsigned long)*(DD_NETWORK_IP_CONFIG_NUM - 2);
				unsigned char *pbuf = new unsigned char[len];
				memset(pbuf, 0 ,sizeof(unsigned char)*len);
				unsigned char *pTempCmdData = pbuf;

				blockHead.ItemNum = DD_NETWORK_IP_CONFIG_NUM;
				memcpy(pTempCmdData, &blockHead, sizeof(NCFG_BLOCK_HEAD));
				pTempCmdData += sizeof(NCFG_BLOCK_HEAD);
				//填充配置项头
				NCFG_ITEM_HEAD item[DD_NETWORK_IP_CONFIG_NUM];
				NCFG_ITEM_HEAD *pHead = item;
				pHead->itemID = NCFG_ITEM_NETWORK_ADDR_INFO;
				pHead->len = sizeof(NETWORK_ADDR_INFO);
				pHead->num = 1;
				pHead->subLen = sizeof(NETWORK_ADDR_INFO);
				memcpy(pTempCmdData, pHead, sizeof(NCFG_ITEM_HEAD));
				pHead++;
				pTempCmdData += sizeof(NCFG_ITEM_HEAD);

				pHead->itemID = NCFG_ITEM_DNS_1;
				pHead->len = sizeof(unsigned long);
				pHead->num = 1;
				pHead->subLen = sizeof(unsigned long);
				memcpy(pTempCmdData, pHead, sizeof(NCFG_ITEM_HEAD));
				pHead++;
				pTempCmdData += sizeof(NCFG_ITEM_HEAD);

				pHead->itemID = NCFG_ITEM_DNS_2;
				pHead->len = sizeof(unsigned long);
				pHead->num = 1;
				pHead->subLen = sizeof(unsigned long);
				memcpy(pTempCmdData, pHead, sizeof(NCFG_ITEM_HEAD));
				pHead++;
				pTempCmdData += sizeof(NCFG_ITEM_HEAD);

				pHead->itemID = NCFG_ITEM_PPPOE_INFO;
				pHead->len = sizeof(PPPOE_INFO);
				pHead->num = 1;
				pHead->subLen = sizeof(PPPOE_INFO);
				memcpy(pTempCmdData, pHead, sizeof(NCFG_ITEM_HEAD));
				pTempCmdData += sizeof(NCFG_ITEM_HEAD);

				DD_NETWORK_IP_CONFIG* pNetWork =  (DD_NETWORK_IP_CONFIG*)pItemData;
				
				NETWORK_ADDR_INFO networkaddr;
				networkaddr.bDHCP = pNetWork->useDHCP;
				networkaddr.gateway = pNetWork->gateway;
				networkaddr.netMask = pNetWork->subnetMask;
				networkaddr.staticIP = pNetWork->IP;
				memcpy(pTempCmdData, &networkaddr,  sizeof(NETWORK_ADDR_INFO));
				pTempCmdData += sizeof(NETWORK_ADDR_INFO);
				memcpy(pTempCmdData, &(pNetWork->preferredDNS), sizeof(unsigned long));
				pTempCmdData += sizeof(unsigned long);
				memcpy(pTempCmdData, &(pNetWork->alternateDNS), sizeof(unsigned long));
				pTempCmdData += sizeof(unsigned long);

				PPPOE_INFO pppoe_info;
				pppoe_info.bEnable = pNetWork->usePPPoE;
				strcpy(pppoe_info.password, pNetWork->password);
				strcpy(pppoe_info.user, pNetWork->account);
				memcpy(pTempCmdData, &pppoe_info, sizeof(PPPOE_INFO));

				ITEM_INFO item_info;
				item_info.parserLen = len;
				item_info.pConfigItem = (unsigned char *)pbuf;
				iteminfolist.push_back(item_info);
				datalen += len;

				break;
			}
		case DD_CONFIG_ITEM_NETWORK_ADVANCE:
			{
				unsigned long len = sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD)*3 + sizeof(unsigned long)*3;
				unsigned char *pbuf = new unsigned char[len];
				memset(pbuf, 0 ,sizeof(unsigned char)*len);
				unsigned char *pTempCmdData = pbuf;

				blockHead.ItemNum = 3;
				memcpy(pTempCmdData, &blockHead, sizeof(NCFG_BLOCK_HEAD));
				pTempCmdData += sizeof(NCFG_BLOCK_HEAD);
				//填充配置项头
				NCFG_ITEM_HEAD item[3];
				NCFG_ITEM_HEAD *pHead = item;
				pHead->itemID = NCFG_ITEM_MAX_CLIENT_NUM;
				pHead->len = sizeof(unsigned long);
				pHead->num = 1;
				pHead->subLen = sizeof(unsigned long);
				memcpy(pTempCmdData, pHead, sizeof(NCFG_ITEM_HEAD));
				pHead++;
				pTempCmdData += sizeof(NCFG_ITEM_HEAD);

				pHead->itemID = NCFG_ITEM_HTTP_PORT;
				pHead->len = sizeof(unsigned long);
				pHead->num = 1;
				pHead->subLen = sizeof(unsigned long);
				memcpy(pTempCmdData, pHead, sizeof(NCFG_ITEM_HEAD));
				pHead++;
				pTempCmdData += sizeof(NCFG_ITEM_HEAD);
				
				pHead->itemID = NCFG_ITEM_SERVER_PORT;
				pHead->len = sizeof(unsigned long);
				pHead->num = 1;
				pHead->subLen = sizeof(unsigned long);
				memcpy(pTempCmdData, pHead, sizeof(NCFG_ITEM_HEAD));
				pHead++;
				pTempCmdData += sizeof(NCFG_ITEM_HEAD);

				DD_NETWORK_ADVANCE_CONFIG* pNetAdvance =  (DD_NETWORK_ADVANCE_CONFIG*)pItemData;
				memcpy(pTempCmdData, &(pNetAdvance->OnlineUserNum), sizeof(unsigned short));
				pTempCmdData += sizeof(unsigned long);
				memcpy(pTempCmdData, &(pNetAdvance->httpPort), sizeof(unsigned short));
				pTempCmdData += sizeof(unsigned long);
				memcpy(pTempCmdData, &(pNetAdvance->datePort), sizeof(unsigned short));

				ITEM_INFO item_info;
				item_info.parserLen = len;
				item_info.pConfigItem = (unsigned char *)pbuf;
				iteminfolist.push_back(item_info);
				datalen += len;

				break;
			}
		case DD_CONFIG_ITEM_NETWORK_DDNS:
			{
				unsigned long len = sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD)*DD_DDNS_CONFIG_NUM + sizeof(DDNS_INFO) + sizeof(unsigned long)*(DD_DDNS_CONFIG_NUM - 1);
				unsigned char *pbuf = new unsigned char[len];
				memset(pbuf, 0 ,sizeof(unsigned char)*len);
				unsigned char *pTempCmdData = pbuf;

				blockHead.ItemNum = DD_DDNS_CONFIG_NUM;
				memcpy(pTempCmdData, &blockHead, sizeof(NCFG_BLOCK_HEAD));
				pTempCmdData += sizeof(NCFG_BLOCK_HEAD);
				//填充配置项头
				NCFG_ITEM_HEAD item[DD_DDNS_CONFIG_NUM];
				NCFG_ITEM_HEAD *pHead = item;
				pHead->itemID = NCFG_ITEM_DDNS_INFO;
				pHead->len = sizeof(DDNS_INFO);
				pHead->num = 1;
				pHead->subLen = sizeof(DDNS_INFO);
				memcpy(pTempCmdData, pHead, sizeof(NCFG_ITEM_HEAD));
				pHead++;
				pTempCmdData += sizeof(NCFG_ITEM_HEAD);

				pHead->itemID = NCFG_ITEM_DDNS_UPDATE_CYCLE;
				pHead->len = sizeof(unsigned long);
				pHead->num = 1;
				pHead->subLen = sizeof(unsigned long);
				memcpy(pTempCmdData, pHead, sizeof(NCFG_ITEM_HEAD));
				pTempCmdData += sizeof(NCFG_ITEM_HEAD);

				DD_DDNS_CONFIG* pDDns =  (DD_DDNS_CONFIG*)pItemData;
				DDNS_INFO ddns_info;
				ddns_info.bEnable = (unsigned char)pDDns->enable;
				ddns_info.type = (unsigned short)pDDns->useDDNSServer;
				ddns_info.bUseDomain = (unsigned char)pDDns->userHostDomain;
				strcpy(ddns_info.password, pDDns->password);
				strcpy(ddns_info.user, pDDns->userName);
				strcpy(ddns_info.domain, pDDns->hostDomain);
				memcpy(pTempCmdData, &ddns_info, sizeof(DDNS_INFO));
				pTempCmdData += sizeof(DDNS_INFO);
				memcpy(pTempCmdData, &(pDDns->interval), sizeof(unsigned short));

				ITEM_INFO item_info;
				item_info.parserLen = len;
				item_info.pConfigItem = (unsigned char *)pbuf;
				iteminfolist.push_back(item_info);
				datalen += len;

				break;
			}
		case DD_CONFIG_ITEM_NETWORK_SMTP:
			{
				unsigned long len = sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD)*DD_SMTP_CONFIG_NUM + sizeof(MAIL_SEND_INFO) + sizeof(MAIL_RECV_INFO) * DD_MAX_EMAIL_RECEIVE_ADDR_NUM;
				unsigned char *pbuf = new unsigned char[len];
				memset(pbuf, 0 ,sizeof(unsigned char)*len);
				unsigned char *pTempCmdData = pbuf;

				blockHead.ItemNum = DD_SMTP_CONFIG_NUM;
				memcpy(pTempCmdData, &blockHead, sizeof(NCFG_BLOCK_HEAD));
				pTempCmdData += sizeof(NCFG_BLOCK_HEAD);
				//填充配置项头
				NCFG_ITEM_HEAD item[DD_SMTP_CONFIG_NUM];
				NCFG_ITEM_HEAD *pHead = item;
				pHead->itemID = NCFG_ITEM_EMAIL_SEND_INFO;
				pHead->len = sizeof(MAIL_SEND_INFO);
				pHead->num = 1;
				pHead->subLen = sizeof(MAIL_SEND_INFO);
				memcpy(pTempCmdData, pHead, sizeof(NCFG_ITEM_HEAD));
				pHead++;
				pTempCmdData += sizeof(NCFG_ITEM_HEAD);

				pHead->itemID = NCFG_ITEM_EMAIL_RECV_INFO;
				pHead->len = sizeof(MAIL_RECV_INFO) * DD_MAX_EMAIL_RECEIVE_ADDR_NUM;
				pHead->num = DD_MAX_EMAIL_RECEIVE_ADDR_NUM;
				pHead->subLen = sizeof(MAIL_RECV_INFO);
				memcpy(pTempCmdData, pHead, sizeof(NCFG_ITEM_HEAD));
				pTempCmdData += sizeof(NCFG_ITEM_HEAD);

				DD_SMTP_CONFIG* pSmtp =  (DD_SMTP_CONFIG*)pItemData;
				MAIL_SEND_INFO send_info;
				send_info.bSSL = pSmtp->enableSSL;
				send_info.port = pSmtp->port;
				strcpy(send_info.name, pSmtp->sendAddress);
				strcpy(send_info.server, pSmtp->server);
				strcpy(send_info.passwd, pSmtp->password);
				memcpy(pTempCmdData, &send_info, sizeof(MAIL_SEND_INFO));
				pTempCmdData += sizeof(MAIL_SEND_INFO);
				
				MAIL_RECV_INFO recv_info[DD_MAX_EMAIL_RECEIVE_ADDR_NUM];
				for (int i = 0; i < DD_MAX_EMAIL_RECEIVE_ADDR_NUM; i++)
				{
					memset(&recv_info[i], 0, sizeof(MAIL_RECV_INFO));
					strcpy(recv_info[i].recvname, pSmtp->receiveAddress[i]);
					memcpy(pTempCmdData, &recv_info[i], sizeof(MAIL_RECV_INFO));
					pTempCmdData += sizeof(MAIL_RECV_INFO);
				}

				ITEM_INFO item_info;
				item_info.parserLen = len;
				item_info.pConfigItem = (unsigned char *)pbuf;
				iteminfolist.push_back(item_info);
				datalen += len;
				break;
			}
		case DD_CONFIG_ITEM_ACCOUNT:
			{
				unsigned long len = sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD) + sizeof(NCFG_INFO_USER) * itemHead.num;
				unsigned char *pbuf = new unsigned char[len];
				memset(pbuf, 0 ,sizeof(unsigned char)*len);
				unsigned char *pTempCmdData = pbuf;

				blockHead.ItemNum = DD_ACCOUNT_CONFIG_NUM;
				memcpy(pTempCmdData, &blockHead, sizeof(NCFG_BLOCK_HEAD));
				pTempCmdData += sizeof(NCFG_BLOCK_HEAD);
				//填充配置项头
				NCFG_ITEM_HEAD item[DD_ACCOUNT_CONFIG_NUM];
				NCFG_ITEM_HEAD *pHead = item;
				pHead->itemID = NCFG_ITEM_USER;
				pHead->len = sizeof(NCFG_INFO_USER) * itemHead.num;
				pHead->num = itemHead.num;
				pHead->subLen = sizeof(NCFG_INFO_USER);
				memcpy(pTempCmdData, pHead, sizeof(NCFG_ITEM_HEAD));
				pTempCmdData += sizeof(NCFG_ITEM_HEAD);
			
				DD_ACCOUNT_CONFIG* pAccout =  (DD_ACCOUNT_CONFIG*)pItemData;
				
				for (int i = 0; i < itemHead.num; i++)
				{	
					NCFG_INFO_USER user_info;
					
					memcpy(&user_info.authLiveCH, pAccout[i].authLiveCH, sizeof(unsigned long long));
					memcpy(&user_info.authBackupCH, pAccout[i].authBackupCH, sizeof(unsigned long long));
					memcpy(&user_info.authPlaybackCH, pAccout[i].authPlaybackCH, sizeof(unsigned long long));
					memcpy(&user_info.authPTZCtrlCH, pAccout[i].authPTZCH, sizeof(unsigned long long));
					memcpy(&user_info.authRecordCH, pAccout[i].authRecordCH, sizeof(unsigned long long));
					memcpy(&user_info.authRemoteViewCH, pAccout[i].netAuthViewCH, sizeof(unsigned long long));
				
					user_info.bindMac = pAccout[i].bingMAC;
					user_info.group = pAccout[i].group;
					memcpy(user_info.MAC, pAccout[i].MAC, sizeof(unsigned char) * 8); 
					strcpy(user_info.name, pAccout[i].name);
					strcpy(user_info.password, pAccout[i].password);

					unsigned long &authority = user_info.authority;
					
					if (pAccout[i].group == USER_GROUP_ADMIN)   // administrator
					{
						authority = 0xffffffff;
					}
					else // other user
					{
						authority = 0x1;
						unsigned long temp = 0;
						temp = pAccout[i].authLive;
						authority |=  temp << 1;
						temp =  pAccout[i].authRecord;
						authority |=  temp << 2;
						temp =  pAccout[i].authPlayback;
						authority |=  temp << 3;
						temp =  pAccout[i].authBackup;
						authority |=  temp << 4;
						temp = pAccout[i].authPTZ;
						authority |= temp << 5;
						temp = pAccout[i].netAuthView;
						authority |=  temp << 6;
						temp = pAccout[i].logSearch;
						authority |=  temp << 7;
						temp = pAccout[i].systemSetup;
						authority |= temp << 8;
						temp = pAccout[i].shutdown;
						authority |= temp << 9;
						temp = pAccout[i].twoWayAudio;
						authority |= temp << 10;
						temp = pAccout[i].fileManagement;
						authority |= temp << 11;
						temp = pAccout[i].diskManagement;
						authority |= temp << 12;
						temp = pAccout[i].remoteLogin;
						authority |= temp << 13;
						temp = pAccout[i].OnlineUserManagement;
						authority |= temp << 14;
					}

					memcpy(pTempCmdData, &user_info, sizeof(NCFG_INFO_USER));
					pTempCmdData += sizeof(NCFG_INFO_USER);
				}
			
				ITEM_INFO item_info;
				item_info.parserLen = len;
				item_info.pConfigItem = (unsigned char *)pbuf;
				iteminfolist.push_back(item_info);
				datalen += len;

				break;
			}
		case DD_CONFIG_ITEM_SENSOR_SETUP:
			{
				unsigned long len = sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD)*DD_SENSOR_CONFIG_NUM + (sizeof(NCFG_INFO_SHORT_NAME) + sizeof(unsigned long)*(DD_SENSOR_CONFIG_NUM - 1))*m_deiviceInfo.sensorInputNum;
				unsigned char *pbuf = new unsigned char[len];
				memset(pbuf, 0 ,sizeof(unsigned char)*len);
				unsigned char *pTempCmdData = pbuf;

				blockHead.ItemNum = DD_SENSOR_CONFIG_NUM;
				memcpy(pTempCmdData, &blockHead, sizeof(NCFG_BLOCK_HEAD));
				pTempCmdData += sizeof(NCFG_BLOCK_HEAD);
				//填充配置项头
				NCFG_ITEM_HEAD item[DD_SENSOR_CONFIG_NUM];
				NCFG_ITEM_HEAD *pHead = item;
				pHead->itemID = NCFG_ITEM_SENSOR_IN_ENABLE;
				pHead->len = sizeof(unsigned long) * m_deiviceInfo.sensorInputNum;
				pHead->num = m_deiviceInfo.sensorInputNum;
				pHead->subLen = sizeof(unsigned long);
				memcpy(pTempCmdData, pHead, sizeof(NCFG_ITEM_HEAD));
				pHead++;
				pTempCmdData += sizeof(NCFG_ITEM_HEAD);

				pHead->itemID = NCFG_ITEM_SENSOR_IN_TYPE;
				pHead->len = sizeof(unsigned long) * m_deiviceInfo.sensorInputNum;
				pHead->num = m_deiviceInfo.sensorInputNum;
				pHead->subLen = sizeof(unsigned long);
				memcpy(pTempCmdData, pHead, sizeof(NCFG_ITEM_HEAD));
				pHead++;
				pTempCmdData += sizeof(NCFG_ITEM_HEAD);

				pHead->itemID = NCFG_ITEM_SENSOR_IN_NAME;
				pHead->len = sizeof(NCFG_INFO_SHORT_NAME) * m_deiviceInfo.sensorInputNum;
				pHead->num = m_deiviceInfo.sensorInputNum;
				pHead->subLen = sizeof(NCFG_INFO_SHORT_NAME);
				memcpy(pTempCmdData, pHead, sizeof(NCFG_ITEM_HEAD));
				pHead++;
				pTempCmdData += sizeof(NCFG_ITEM_HEAD);

				pHead->itemID = NCFG_ITEM_SENSOR_IN_HOLD_TIME;
				pHead->len = sizeof(unsigned long) * m_deiviceInfo.sensorInputNum;
				pHead->num = m_deiviceInfo.sensorInputNum;
				pHead->subLen = sizeof(unsigned long);
				memcpy(pTempCmdData, pHead, sizeof(NCFG_ITEM_HEAD));
				pTempCmdData += sizeof(NCFG_ITEM_HEAD);

				DD_SENSOR_CONFIG* pSensor =  (DD_SENSOR_CONFIG*)pItemData;
				for (int i = 0; i < m_deiviceInfo.sensorInputNum; i++)
				{
					memcpy(pTempCmdData, &(pSensor[i].enable), sizeof(unsigned char));
					pTempCmdData += sizeof(unsigned long);
				}
				for (int i = 0; i < m_deiviceInfo.sensorInputNum; i++)
				{
					*((unsigned long *)pTempCmdData) = pSensor[i].bNO ? 0 : 1;
					memcpy(pTempCmdData, &(pSensor[i].bNO), sizeof(unsigned char));
					pTempCmdData += sizeof(unsigned long);
				}
				
				NCFG_INFO_SHORT_NAME short_name;
				memset(&short_name, 0, sizeof(NCFG_INFO_SHORT_NAME));
				for (int i = 0; i < m_deiviceInfo.sensorInputNum; i++)
				{	
					
					strcpy(short_name.name, pSensor[i].name);
					memcpy(pTempCmdData, &short_name, sizeof(NCFG_INFO_SHORT_NAME));
					pTempCmdData += sizeof(NCFG_INFO_SHORT_NAME);
				}
				for (int i = 0; i < m_deiviceInfo.sensorInputNum; i++)
				{
					unsigned long test = pSensor[i].holdTime;
					if (test == 0xffff)
					{
						test = 0xffffffff;
					}
					memcpy(pTempCmdData, &test, sizeof(unsigned long));
					pTempCmdData += sizeof(unsigned long);
				}
				ITEM_INFO item_info;
				item_info.parserLen = len;
				item_info.pConfigItem = (unsigned char *)pbuf;
				iteminfolist.push_back(item_info);
				datalen += len;

				break;
			}
		case DD_CONFIG_ITEM_SENSOR_SCHEDULE:
			{
				unsigned long len = sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD)*DD_SENSOR_SCHEDULE_NUM + sizeof(WEEK_SCHEDULE)*DD_SENSOR_SCHEDULE_NUM;
				unsigned char *pbuf = new unsigned char[len];
				memset(pbuf, 0 ,sizeof(unsigned char)*len);
				unsigned char *pTempCmdData = pbuf;

				blockHead.ItemNum = DD_SENSOR_SCHEDULE_NUM;
				memcpy(pTempCmdData, &blockHead, sizeof(NCFG_BLOCK_HEAD));
				pTempCmdData += sizeof(NCFG_BLOCK_HEAD);
				//填充配置项头
				NCFG_ITEM_HEAD item[DD_SENSOR_SCHEDULE_NUM];
				NCFG_ITEM_HEAD *pHead = item;
				pHead->itemID = NCFG_ITEM_SENSOR_IN_SCH_WEEK;
				pHead->len = sizeof(WEEK_SCHEDULE);
				pHead->num = 1;
				pHead->subLen = sizeof(WEEK_SCHEDULE);
				memcpy(pTempCmdData, pHead, sizeof(NCFG_ITEM_HEAD));
				pTempCmdData += sizeof(NCFG_ITEM_HEAD);

				DD_WEEK_SCHEDULE* pWeekSchedule =  (DD_WEEK_SCHEDULE*)pItemData;
				WEEK_SCHEDULE week_schedule;
				DATE_SCHEDULE *pDate = week_schedule.week;
				DD_DATE_SCHEDULE *pDD_Date = pWeekSchedule->week;
				for (int i = 0; i < 7; i++)
				{
					memcpy(pDate->hour, pDD_Date->hour, sizeof(unsigned long long) * 24);
					pDate++;
					pDD_Date++;
				}
				memcpy(pTempCmdData, &week_schedule, sizeof(WEEK_SCHEDULE));

				ITEM_INFO item_info;
				item_info.parserLen = len;
				item_info.pConfigItem = (unsigned char *)pbuf;
				iteminfolist.push_back(item_info);
				datalen += len;

				break;
			}
		case DD_CONFIG_ITEM_SENSOR_ALARM_OUT:
			{
				unsigned long len = sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD)*DD_TRIGGER_ALARM_OUT_NUM + (sizeof(unsigned long)*2 + sizeof(ULONGLONG)*2) * m_deiviceInfo.sensorInputNum;
				unsigned char *pbuf = new unsigned char[len];
				memset(pbuf, 0 ,sizeof(unsigned char)*len);
				unsigned char *pTempCmdData = pbuf;

				blockHead.ItemNum = DD_TRIGGER_ALARM_OUT_NUM;
				memcpy(pTempCmdData, &blockHead, sizeof(NCFG_BLOCK_HEAD));
				pTempCmdData += sizeof(NCFG_BLOCK_HEAD);
				//填充配置项头
				NCFG_ITEM_HEAD item[DD_TRIGGER_ALARM_OUT_NUM];
				NCFG_ITEM_HEAD *pHead = item;
				pHead->itemID = NCFG_ITEM_SENSOR_IN_BUZZ;
				pHead->len = sizeof(unsigned long) * m_deiviceInfo.sensorInputNum;
				pHead->num = m_deiviceInfo.sensorInputNum;
				pHead->subLen = sizeof(unsigned long);
				memcpy(pTempCmdData, pHead, sizeof(NCFG_ITEM_HEAD));
				pHead++;
				pTempCmdData += sizeof(NCFG_ITEM_HEAD);

				pHead->itemID = NCFG_ITEM_SENSOR_IN_TO_ALARM_OUT;
				pHead->len = sizeof(unsigned long) * m_deiviceInfo.sensorInputNum;
				pHead->num = m_deiviceInfo.sensorInputNum;
				pHead->subLen = sizeof(unsigned long);
				memcpy(pTempCmdData, pHead, sizeof(NCFG_ITEM_HEAD));
				pHead++;
				pTempCmdData += sizeof(NCFG_ITEM_HEAD);

				pHead->itemID = NCFG_ITEM_SENSOR_TO_BIG_VIEW;
				pHead->len = sizeof(ULONGLONG) * m_deiviceInfo.videoInputNum;
				pHead->num = m_deiviceInfo.videoInputNum;
				pHead->subLen = sizeof(ULONGLONG);
				memcpy(pTempCmdData, pHead, sizeof(NCFG_ITEM_HEAD));
				pHead++;
				pTempCmdData += sizeof(NCFG_ITEM_HEAD);

				pHead->itemID = NCFG_ITEM_SENSOR_IN_TO_EMAIL_ENABLE;
				pHead->len = sizeof(unsigned long) * m_deiviceInfo.sensorInputNum;
				pHead->num = m_deiviceInfo.sensorInputNum;
				pHead->subLen = sizeof(unsigned long);
				memcpy(pTempCmdData, pHead, sizeof(NCFG_ITEM_HEAD));
				pTempCmdData += sizeof(NCFG_ITEM_HEAD);

				DD_TRIGGER_ALARM_OUT* pTrigger =  (DD_TRIGGER_ALARM_OUT*)pItemData;
				for (int i = 0; i < m_deiviceInfo.sensorInputNum; i++)
				{
					memcpy(pTempCmdData, &(pTrigger[i].toBuzzer), sizeof(unsigned char));
					pTempCmdData += sizeof(unsigned long);
				}
				for (int i = 0; i < m_deiviceInfo.sensorInputNum; i++)
				{
					memcpy(pTempCmdData, &(pTrigger[i].toAlarmOut), sizeof(unsigned long));
					pTempCmdData += sizeof(unsigned long);
				}
				for (int i = 0; i < m_deiviceInfo.videoInputNum; i++)
				{
					unsigned long long *test= (unsigned long long *)pTempCmdData;
					
					if (pTrigger[i].ShowFullScreen == 0xff)
					{
						*test = 0;
					}
					else
					{
						*test = (0x1) << pTrigger[i].ShowFullScreen;
					}
					pTempCmdData += sizeof(unsigned long long);
				}

				for (int i = 0; i < m_deiviceInfo.sensorInputNum; i++)
				{
					*((unsigned long *)pTempCmdData) = pTrigger[i].sendEmail;
					//memcpy(pTempCmdData, &(pTrigger[i].sendEmail), sizeof(unsigned char));
					pTempCmdData += sizeof(unsigned long);
				}
				
				ITEM_INFO item_info;
				item_info.parserLen = len;
				item_info.pConfigItem = (unsigned char *)pbuf;
				iteminfolist.push_back(item_info);
				datalen += len;

				break;
			}
		case DD_CONFIG_ITEM_SENSOR_TO_RECORD:
			{
				unsigned long len = sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD)*DD_TRIGGER_RECORD_NUM + sizeof(ULONGLONG) * DD_TRIGGER_RECORD_NUM * m_deiviceInfo.sensorInputNum;
				unsigned char *pbuf = new unsigned char[len];
				memset(pbuf, 0 ,sizeof(unsigned char)*len);
				unsigned char *pTempCmdData = pbuf;

				blockHead.ItemNum = DD_TRIGGER_RECORD_NUM;
				memcpy(pTempCmdData, &blockHead, sizeof(NCFG_BLOCK_HEAD));
				pTempCmdData += sizeof(NCFG_BLOCK_HEAD);
				//填充配置项头
				NCFG_ITEM_HEAD item[DD_TRIGGER_RECORD_NUM];
				NCFG_ITEM_HEAD *pHead = item;
				pHead->itemID = NCFG_ITEM_SENSOR_IN_TO_RECORD;
				pHead->len = sizeof(ULONGLONG) * m_deiviceInfo.sensorInputNum;
				pHead->num = m_deiviceInfo.sensorInputNum;
				pHead->subLen = sizeof(ULONGLONG);
				memcpy(pTempCmdData, pHead, sizeof(NCFG_ITEM_HEAD));
				pHead++;
				pTempCmdData += sizeof(NCFG_ITEM_HEAD);

				pHead->itemID = NCFG_ITEM_SENSOR_IN_TO_SNAP ;
				pHead->len = sizeof(ULONGLONG) * m_deiviceInfo.sensorInputNum;
				pHead->num = m_deiviceInfo.sensorInputNum;
				pHead->subLen = sizeof(ULONGLONG);
				memcpy(pTempCmdData, pHead, sizeof(NCFG_ITEM_HEAD));
				pTempCmdData += sizeof(NCFG_ITEM_HEAD);

				DD_TRIGGER_RECORD* pTrigger =  (DD_TRIGGER_RECORD*)pItemData;
				
				for (int i = 0; i < m_deiviceInfo.sensorInputNum; i++)
				{
					memcpy(pTempCmdData, pTrigger[i].recordCH, sizeof(ULONGLONG));
					pTempCmdData += sizeof(ULONGLONG);
				}
				
				for (int i = 0; i < m_deiviceInfo.sensorInputNum; i++)
				{
					memcpy(pTempCmdData, pTrigger[i].snapCH, sizeof(ULONGLONG));
					pTempCmdData += sizeof(ULONGLONG);
				}

				ITEM_INFO item_info;
				item_info.parserLen = len;
				item_info.pConfigItem = (unsigned char *)pbuf;
				iteminfolist.push_back(item_info);
				datalen += len;

				break;
			}
		case DD_CONFIG_ITEM_MOTION_TO_RECORD:
			{
				unsigned long len = sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD)*DD_TRIGGER_RECORD_NUM + sizeof(ULONGLONG) * DD_TRIGGER_RECORD_NUM * m_deiviceInfo.videoInputNum;
				unsigned char *pbuf = new unsigned char[len];
				memset(pbuf, 0 ,sizeof(unsigned char)*len);
				unsigned char *pTempCmdData = pbuf;

				blockHead.ItemNum = DD_TRIGGER_RECORD_NUM;
				memcpy(pTempCmdData, &blockHead, sizeof(NCFG_BLOCK_HEAD));
				pTempCmdData += sizeof(NCFG_BLOCK_HEAD);
				//填充配置项头
				NCFG_ITEM_HEAD item[DD_TRIGGER_RECORD_NUM];
				NCFG_ITEM_HEAD *pHead = item;
				pHead->itemID = NCFG_ITEM_MOTION_TO_RECORD;
				pHead->len = sizeof(ULONGLONG) * m_deiviceInfo.videoInputNum;
				pHead->num = m_deiviceInfo.videoInputNum;
				pHead->subLen = sizeof(ULONGLONG);
				memcpy(pTempCmdData, pHead, sizeof(NCFG_ITEM_HEAD));
				pHead++;
				pTempCmdData += sizeof(NCFG_ITEM_HEAD);

				pHead->itemID = NCFG_ITEM_MOTION_TO_SNAP;
				pHead->len = sizeof(ULONGLONG) * m_deiviceInfo.videoInputNum;
				pHead->num = m_deiviceInfo.videoInputNum;
				pHead->subLen = sizeof(ULONGLONG);
				memcpy(pTempCmdData, pHead, sizeof(NCFG_ITEM_HEAD));
				pTempCmdData += sizeof(NCFG_ITEM_HEAD);

				DD_TRIGGER_RECORD* pTrigger = (DD_TRIGGER_RECORD*)pItemData;
				for (int i = 0; i < m_deiviceInfo.videoInputNum; i++)
				{
					memcpy(pTempCmdData, pTrigger[i].recordCH, sizeof(ULONGLONG));
					pTempCmdData += sizeof(ULONGLONG);
				}
				for (int i = 0; i < m_deiviceInfo.videoInputNum; i++)
				{
					memcpy(pTempCmdData, pTrigger[i].snapCH, sizeof(ULONGLONG));
					pTempCmdData += sizeof(ULONGLONG);
				}
				
				ITEM_INFO item_info;
				item_info.parserLen = len;
				item_info.pConfigItem = (unsigned char *)pbuf;
				iteminfolist.push_back(item_info);
				datalen += len;
				break;
			}
		case DD_CONFIG_ITEM_SHELTER_TO_RECORD:
			{
				unsigned long len = sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD)*DD_TRIGGER_RECORD_NUM + sizeof(ULONGLONG)*DD_TRIGGER_RECORD_NUM * m_deiviceInfo.videoInputNum;
				unsigned char *pbuf = new unsigned char[len];
				memset(pbuf, 0 ,sizeof(unsigned char)*len);
				unsigned char *pTempCmdData = pbuf;

				blockHead.ItemNum = DD_TRIGGER_RECORD_NUM;
				memcpy(pTempCmdData, &blockHead, sizeof(NCFG_BLOCK_HEAD));
				pTempCmdData += sizeof(NCFG_BLOCK_HEAD);
				//填充配置项头
				NCFG_ITEM_HEAD item[DD_TRIGGER_RECORD_NUM];
				NCFG_ITEM_HEAD *pHead = item;
				pHead->itemID = NCFG_ITEM_SHELTER_TO_RECORD;
				pHead->len = sizeof(ULONGLONG) * m_deiviceInfo.videoInputNum;
				pHead->num = m_deiviceInfo.videoInputNum;
				pHead->subLen = sizeof(ULONGLONG);
				memcpy(pTempCmdData, pHead, sizeof(NCFG_ITEM_HEAD));
				pHead++;
				pTempCmdData += sizeof(NCFG_ITEM_HEAD);

				pHead->itemID = NCFG_ITEM_SHELTER_TO_SNAP;
				pHead->len = sizeof(ULONGLONG) * m_deiviceInfo.videoInputNum;
				pHead->num = m_deiviceInfo.videoInputNum;
				pHead->subLen = sizeof(ULONGLONG);
				memcpy(pTempCmdData, pHead, sizeof(NCFG_ITEM_HEAD));
				pTempCmdData += sizeof(NCFG_ITEM_HEAD);

				DD_TRIGGER_RECORD* pTrigger = (DD_TRIGGER_RECORD*)pItemData;
				for (int i = 0; i < m_deiviceInfo.videoInputNum; i++)
				{
					ULONGLONG recordCH;
					recordCH = atol((char*)pTrigger[i].recordCH);
					memcpy(pTempCmdData, &recordCH, sizeof(ULONGLONG));
					pTempCmdData += sizeof(ULONGLONG);
				}
				for (int i = 0; i < m_deiviceInfo.videoInputNum; i++)
				{
					ULONGLONG snapCH;
					snapCH = atol((char*)pTrigger[i].snapCH);
					memcpy(pTempCmdData, &snapCH, sizeof(ULONGLONG));
					pTempCmdData += sizeof(ULONGLONG);
				}

				ITEM_INFO item_info;
				item_info.parserLen = len;
				item_info.pConfigItem = (unsigned char *)pbuf;
				iteminfolist.push_back(item_info);
				datalen += len;
				break;
			}
		case DD_CONFIG_ITEM_VLOSS_TO_RECORD:
			{
				unsigned long len = sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD)*DD_TRIGGER_RECORD_NUM + sizeof(ULONGLONG) * DD_TRIGGER_RECORD_NUM * m_deiviceInfo.videoInputNum;
				unsigned char *pbuf = new unsigned char[len];
				memset(pbuf, 0 ,sizeof(unsigned char)*len);
				unsigned char *pTempCmdData = pbuf;

				blockHead.ItemNum = DD_TRIGGER_RECORD_NUM;
				memcpy(pTempCmdData, &blockHead, sizeof(NCFG_BLOCK_HEAD));
				pTempCmdData += sizeof(NCFG_BLOCK_HEAD);
				//填充配置项头
				NCFG_ITEM_HEAD item[DD_TRIGGER_RECORD_NUM];
				NCFG_ITEM_HEAD *pHead = item;
				pHead->itemID = NCFG_ITEM_VL_TO_RECORD;
				pHead->len = sizeof(ULONGLONG) * m_deiviceInfo.videoInputNum;
				pHead->num = m_deiviceInfo.videoInputNum;
				pHead->subLen = sizeof(ULONGLONG);
				memcpy(pTempCmdData, pHead, sizeof(NCFG_ITEM_HEAD));
				pHead++;
				pTempCmdData += sizeof(NCFG_ITEM_HEAD);

				pHead->itemID = NCFG_ITEM_VL_TO_SNAP;
				pHead->len = sizeof(ULONGLONG) * m_deiviceInfo.videoInputNum;
				pHead->num = m_deiviceInfo.videoInputNum;
				pHead->subLen = sizeof(ULONGLONG);
				memcpy(pTempCmdData, pHead, sizeof(NCFG_ITEM_HEAD));
				pTempCmdData += sizeof(NCFG_ITEM_HEAD);

				DD_TRIGGER_RECORD* pTrigger = (DD_TRIGGER_RECORD*)pItemData;
				for (int i = 0; i < m_deiviceInfo.videoInputNum; i++)
				{
					memcpy(pTempCmdData, pTrigger[i].recordCH, sizeof(ULONGLONG));
					pTempCmdData += sizeof(ULONGLONG);
				}
				for (int i = 0; i < m_deiviceInfo.videoInputNum; i++)
				{
					memcpy(pTempCmdData, pTrigger[i].snapCH, sizeof(ULONGLONG));
					pTempCmdData += sizeof(ULONGLONG);
				}

				ITEM_INFO item_info;
				item_info.parserLen = len;
				item_info.pConfigItem = (unsigned char *)pbuf;
				iteminfolist.push_back(item_info);
				datalen += len;
				break;
			}
		case DD_CONFIG_ITEM_SENSOR_TO_PTZ:
			{
				unsigned long len = sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD)*DD_TRIGGER_PTZ_NUM + sizeof(TO_PTZ) * m_deiviceInfo.sensorInputNum * m_deiviceInfo.videoInputNum;
				unsigned char *pbuf = new unsigned char[len];
				memset(pbuf, 0 ,sizeof(unsigned char)*len);
				unsigned char *pTempCmdData = pbuf;

				blockHead.ItemNum = DD_TRIGGER_PTZ_NUM;
				memcpy(pTempCmdData, &blockHead, sizeof(NCFG_BLOCK_HEAD));
				pTempCmdData += sizeof(NCFG_BLOCK_HEAD);
				//填充配置项头
				NCFG_ITEM_HEAD item[DD_TRIGGER_PTZ_NUM];
				NCFG_ITEM_HEAD *pHead = item;
				pHead->itemID = NCFG_ITEM_SENSOR_IN_TO_PTZ;
				pHead->len = sizeof(TO_PTZ) * m_deiviceInfo.sensorInputNum * m_deiviceInfo.videoInputNum;
				pHead->num = m_deiviceInfo.sensorInputNum * m_deiviceInfo.videoInputNum;
				pHead->subLen = sizeof(TO_PTZ);
				memcpy(pTempCmdData, pHead, sizeof(NCFG_ITEM_HEAD));
				pTempCmdData += sizeof(NCFG_ITEM_HEAD);
				
				DD_TRIGGER_PTZ* pTrigger =  (DD_TRIGGER_PTZ*)pItemData;
				
				for (int i = 0; i < m_deiviceInfo.sensorInputNum; i++)
				{
					for (int j = 0; j < m_deiviceInfo.videoInputNum; j++)
					{
						TO_PTZ to_ptz;
						memset(&to_ptz, 0, sizeof(TO_PTZ));
						if (1 == pTrigger[i * m_deiviceInfo.sensorInputNum + j].toPTZType)
						{
							to_ptz.cIsEnablePreset = true;	
							to_ptz.ucToPreset = pTrigger[i * m_deiviceInfo.sensorInputNum + j].toIndex;
							to_ptz.ucBackPreset = pTrigger[i * m_deiviceInfo.sensorInputNum + j].backIndex;
						}
						else if (2 == pTrigger[i * m_deiviceInfo.sensorInputNum + j].toPTZType)
						{
							to_ptz.cIsEnableCruise = true;
							to_ptz.ucToCruise = pTrigger[i * m_deiviceInfo.sensorInputNum + j].toIndex;
						}
						else if (3 == pTrigger[i * m_deiviceInfo.sensorInputNum + j].toPTZType)
						{
							to_ptz.cIsEnableTrack = true;
							to_ptz.ucToTrack =  pTrigger[i * m_deiviceInfo.sensorInputNum + j].toIndex;
						}
						else
						{
						}
						memcpy(pTempCmdData, &to_ptz, sizeof(TO_PTZ));
						pTempCmdData += sizeof(TO_PTZ);
					}
				}
				
				ITEM_INFO item_info;
				item_info.parserLen = len;
				item_info.pConfigItem = (unsigned char *)pbuf;
				iteminfolist.push_back(item_info);
				datalen += len;
				break;
			}
		case DD_CONFIG_ITEM_MOTION_SETUP:
		case DD_CONFIG_ITEM_SHELTER_SETUP:
			{
				unsigned long len = sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD)*DD_MOTION_CONFIG_NUM + (sizeof(MOTION_DETECT_INFO) + sizeof(unsigned long)*(DD_MOTION_CONFIG_NUM - 1)) * m_deiviceInfo.videoInputNum;
				unsigned char *pbuf = new unsigned char[len];
				memset(pbuf, 0 ,sizeof(unsigned char)*len);
				unsigned char *pTempCmdData = pbuf;

				blockHead.ItemNum = DD_MOTION_CONFIG_NUM;
				memcpy(pTempCmdData, &blockHead, sizeof(NCFG_BLOCK_HEAD));
				pTempCmdData += sizeof(NCFG_BLOCK_HEAD);
				//填充配置项头
				NCFG_ITEM_HEAD item[DD_MOTION_CONFIG_NUM];
				NCFG_ITEM_HEAD *pHead = item;
				pHead->itemID = NCFG_ITEM_MOTION_ENABLE;
				pHead->len = sizeof(unsigned long) * m_deiviceInfo.videoInputNum;
				pHead->num = m_deiviceInfo.videoInputNum;
				pHead->subLen = sizeof(unsigned long);
				memcpy(pTempCmdData, pHead, sizeof(NCFG_ITEM_HEAD));
				pHead++;
				pTempCmdData += sizeof(NCFG_ITEM_HEAD);

				pHead->itemID = NCFG_ITEM_MOTION_AREA_INFO;
				pHead->len = sizeof(MOTION_DETECT_INFO) * m_deiviceInfo.videoInputNum;
				pHead->num = m_deiviceInfo.videoInputNum;
				pHead->subLen = sizeof(MOTION_DETECT_INFO);
				memcpy(pTempCmdData, pHead, sizeof(NCFG_ITEM_HEAD));
				pHead++;
				pTempCmdData += sizeof(NCFG_ITEM_HEAD);
				pHead->itemID = NCFG_ITEM_MOTION_HOLD_TIME;
				pHead->len = sizeof(unsigned long) * m_deiviceInfo.videoInputNum;
				pHead->num = m_deiviceInfo.videoInputNum;
				pHead->subLen = sizeof(unsigned long);
				memcpy(pTempCmdData, pHead, sizeof(NCFG_ITEM_HEAD));
				pTempCmdData += sizeof(NCFG_ITEM_HEAD);

				DD_MOTION_CONFIG* pSetup =  (DD_MOTION_CONFIG*)pItemData;
				for (int i = 0; i < m_deiviceInfo.videoInputNum; i++)
				{
					memcpy(pTempCmdData, &(pSetup[i].enable), sizeof(unsigned char));
					pTempCmdData += sizeof(unsigned long);
				}
				for (int i = 0; i < m_deiviceInfo.videoInputNum; i++)
				{
					MOTION_DETECT_INFO detect_info;
					detect_info.sensitivity = pSetup[i].area.sensitivity;
					detect_info.areaInfo.gridHeight = pSetup[i].area.hightNum;
					detect_info.areaInfo.gridWidth = pSetup[i].area.widthNum;
					//strcpy((char*)detect_info.areaInfo.pData, (char*)pSetup[i].area.area);
					memcpy(detect_info.areaInfo.pData, pSetup[i].area.area, sizeof(detect_info.areaInfo.pData));
					memcpy(pTempCmdData, &detect_info, sizeof(MOTION_DETECT_INFO));
					pTempCmdData += sizeof(MOTION_DETECT_INFO);
				}
				for (int i = 0; i < m_deiviceInfo.videoInputNum; i++)
				{
					unsigned long test = pSetup[i].holdTime;
					if (test == 0xffff)
					{
						test = 0xffffffff;
					}
					memcpy(pTempCmdData, &test, sizeof(unsigned long));
					pTempCmdData += sizeof(unsigned long);
				}

				ITEM_INFO item_info;
				item_info.parserLen = len;
				item_info.pConfigItem = (unsigned char *)pbuf;
				iteminfolist.push_back(item_info);
				datalen += len;
				break;
			}
		case DD_CONFIG_ITEM_MOTION_SCHEDULE:
			{
				unsigned long len = sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD)*DD_MOTION_SCHEDULE_NUM + sizeof(WEEK_SCHEDULE)*DD_MOTION_SCHEDULE_NUM;
				unsigned char *pbuf = new unsigned char[len];
				memset(pbuf, 0 ,sizeof(unsigned char)*len);
				unsigned char *pTempCmdData = pbuf;

				blockHead.ItemNum = DD_MOTION_SCHEDULE_NUM;
				memcpy(pTempCmdData, &blockHead, sizeof(NCFG_BLOCK_HEAD));
				pTempCmdData += sizeof(NCFG_BLOCK_HEAD);
				//填充配置项头
				NCFG_ITEM_HEAD item[DD_MOTION_SCHEDULE_NUM];
				NCFG_ITEM_HEAD *pHead = item;
				pHead->itemID = NCFG_ITEM_MOTION_SCH_WEEK;
				pHead->len = sizeof(WEEK_SCHEDULE);
				pHead->num = 1;
				pHead->subLen = sizeof(WEEK_SCHEDULE);
				memcpy(pTempCmdData, pHead, sizeof(NCFG_ITEM_HEAD));
				pTempCmdData += sizeof(NCFG_ITEM_HEAD);
			
				DD_WEEK_SCHEDULE* pWeekSchedule =  (DD_WEEK_SCHEDULE*)pItemData;
				WEEK_SCHEDULE week_schedule;
				DATE_SCHEDULE *pDate = week_schedule.week;
				DD_DATE_SCHEDULE *pDD_Date = pWeekSchedule->week;
				for (int i = 0; i < 7; i++)
				{
					memcpy(pDate->hour, pDD_Date->hour, sizeof(unsigned long long) * 24);
					pDate++;
					pDD_Date++;
				}
				memcpy(pTempCmdData, &week_schedule, sizeof(WEEK_SCHEDULE));

				ITEM_INFO item_info;
				item_info.parserLen = len;
				item_info.pConfigItem = (unsigned char *)pbuf;
				iteminfolist.push_back(item_info);
				datalen += len;
				break;
			}
		case DD_CONFIG_ITEM_MOTION_ALARM_OUT:
			{
				unsigned long len = sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD)*DD_TRIGGER_ALARM_OUT_NUM + (sizeof(ULONGLONG)*2 + sizeof(unsigned long)*2) * m_deiviceInfo.videoInputNum;
				unsigned char *pbuf = new unsigned char[len];
				memset(pbuf, 0 ,sizeof(unsigned char)*len);
				unsigned char *pTempCmdData = pbuf;

				blockHead.ItemNum = DD_TRIGGER_ALARM_OUT_NUM;
				memcpy(pTempCmdData, &blockHead, sizeof(NCFG_BLOCK_HEAD));
				pTempCmdData += sizeof(NCFG_BLOCK_HEAD);
				//填充配置项头
				NCFG_ITEM_HEAD item[DD_TRIGGER_ALARM_OUT_NUM];
				NCFG_ITEM_HEAD *pHead = item;
				pHead->itemID = NCFG_ITEM_MOTION_TO_ALARM_OUT;
				pHead->len = sizeof(unsigned long long) * m_deiviceInfo.videoInputNum;
				pHead->num = m_deiviceInfo.videoInputNum;
				pHead->subLen = sizeof(unsigned long long);
				memcpy(pTempCmdData, pHead, sizeof(NCFG_ITEM_HEAD));
				pHead++;
				pTempCmdData += sizeof(NCFG_ITEM_HEAD);

				pHead->itemID = NCFG_ITEM_MOTION_TO_BIG_VIEW;
				pHead->len = sizeof(ULONGLONG) * m_deiviceInfo.videoInputNum;
				pHead->num = m_deiviceInfo.videoInputNum;
				pHead->subLen = sizeof(ULONGLONG);
				memcpy(pTempCmdData, pHead, sizeof(NCFG_ITEM_HEAD));
				pHead++;
				pTempCmdData += sizeof(NCFG_ITEM_HEAD);

				pHead->itemID = NCFG_ITEM_MOTION_TO_EMAIL_ENABLE;
				pHead->len = sizeof(unsigned long) * m_deiviceInfo.videoInputNum;
				pHead->num = m_deiviceInfo.videoInputNum;
				pHead->subLen = sizeof(unsigned long);
				memcpy(pTempCmdData, pHead, sizeof(NCFG_ITEM_HEAD));
				pHead++;
				pTempCmdData += sizeof(NCFG_ITEM_HEAD);

				pHead->itemID = NCFG_ITEM_MOTION_TO_BUZZ;
				pHead->len = sizeof(unsigned long) * m_deiviceInfo.videoInputNum;
				pHead->num = 1;
				pHead->subLen = sizeof(unsigned long) * m_deiviceInfo.videoInputNum;
				memcpy(pTempCmdData, pHead, sizeof(NCFG_ITEM_HEAD));
				pTempCmdData += sizeof(NCFG_ITEM_HEAD);

				DD_TRIGGER_ALARM_OUT* pTrigger =  (DD_TRIGGER_ALARM_OUT*)pItemData;
				for (int i = 0; i < m_deiviceInfo.videoInputNum; i++)
				{
					memcpy(pTempCmdData, &(pTrigger[i].toAlarmOut), sizeof(unsigned long));
					pTempCmdData += sizeof(unsigned long long);
				}
				for (int i = 0; i < m_deiviceInfo.videoInputNum; i++)
				{
					unsigned long long *test= (unsigned long long *)pTempCmdData;

					if (pTrigger[i].ShowFullScreen == 0xff)
					{
						*test = 0;
					}
					else
					{
						*test = (0x1) << pTrigger[i].ShowFullScreen;
					}
					pTempCmdData += sizeof(unsigned long long);
				}

				for (int i = 0; i < m_deiviceInfo.videoInputNum; i++)
				{
					*((unsigned long *)pTempCmdData) = pTrigger[i].sendEmail;
					//memcpy(pTempCmdData, &(pTrigger[i].sendEmail), sizeof(unsigned char));
					pTempCmdData += sizeof(unsigned long);
				}

				for (int i = 0; i < m_deiviceInfo.videoInputNum; i++)
				{
					memcpy(pTempCmdData, &(pTrigger[i].toBuzzer), sizeof(unsigned char));
					pTempCmdData += sizeof(unsigned long);
				}

				ITEM_INFO item_info;
				item_info.parserLen = len;
				item_info.pConfigItem = (unsigned char *)pbuf;
				iteminfolist.push_back(item_info);
				datalen += len;
				break;
			}
		case DD_CONFIG_ITEM_SHELTER_ALARM_OUT:
			{
				unsigned long len = sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD)*DD_TRIGGER_ALARM_OUT_NUM + (sizeof(ULONGLONG)*2 + sizeof(unsigned long)*2) * m_deiviceInfo.videoInputNum;
				unsigned char *pbuf = new unsigned char[len];
				memset(pbuf, 0 ,sizeof(unsigned char)*len);
				unsigned char *pTempCmdData = pbuf;

				blockHead.ItemNum = DD_TRIGGER_ALARM_OUT_NUM;
				memcpy(pTempCmdData, &blockHead, sizeof(NCFG_BLOCK_HEAD));
				pTempCmdData += sizeof(NCFG_BLOCK_HEAD);
				//填充配置项头
				NCFG_ITEM_HEAD item[DD_TRIGGER_ALARM_OUT_NUM];
				NCFG_ITEM_HEAD *pHead = item;
				pHead->itemID = NCFG_ITEM_SHELTER_TO_ALARM_OUT;
				pHead->len = sizeof(ULONGLONG) * m_deiviceInfo.videoInputNum;
				pHead->num = m_deiviceInfo.videoInputNum;
				pHead->subLen = sizeof(ULONGLONG);
				memcpy(pTempCmdData, pHead, sizeof(NCFG_ITEM_HEAD));
				pHead++;
				pTempCmdData += sizeof(NCFG_ITEM_HEAD);

				pHead->itemID = NCFG_ITEM_SHELTER_TO_BIG_VIEW;
				pHead->len = sizeof(ULONGLONG) * m_deiviceInfo.videoInputNum;
				pHead->num = m_deiviceInfo.videoInputNum;
				pHead->subLen = sizeof(ULONGLONG);
				memcpy(pTempCmdData, pHead, sizeof(NCFG_ITEM_HEAD));
				pHead++;
				pTempCmdData += sizeof(NCFG_ITEM_HEAD);

				pHead->itemID = NCFG_ITEM_SHELTER_TO_EMAIL_ENABLE;
				pHead->len = sizeof(unsigned long) * m_deiviceInfo.videoInputNum;
				pHead->num = m_deiviceInfo.videoInputNum;
				pHead->subLen = sizeof(unsigned long);
				memcpy(pTempCmdData, pHead, sizeof(NCFG_ITEM_HEAD));
				pHead++;
				pTempCmdData += sizeof(NCFG_ITEM_HEAD);

				pHead->itemID = NCFG_ITEM_SHELTER_TO_BUZZ;
				pHead->len = sizeof(unsigned long) * m_deiviceInfo.videoInputNum;
				pHead->num = m_deiviceInfo.videoInputNum;
				pHead->subLen = sizeof(unsigned long);
				memcpy(pTempCmdData, pHead, sizeof(NCFG_ITEM_HEAD));
				pTempCmdData += sizeof(NCFG_ITEM_HEAD);

				DD_TRIGGER_ALARM_OUT* pTrigger =  (DD_TRIGGER_ALARM_OUT*)pItemData;
				for (int i = 0; i < m_deiviceInfo.videoInputNum; i++)
				{
					memcpy(pTempCmdData, &(pTrigger[i].toAlarmOut), sizeof(unsigned long));
					pTempCmdData += sizeof(ULONGLONG);
				}
				for (int i = 0; i < m_deiviceInfo.videoInputNum; i++)
				{
					memcpy(pTempCmdData, &(pTrigger[i].ShowFullScreen), sizeof(unsigned char));
					pTempCmdData += sizeof(ULONGLONG);
				}
				for (int i = 0; i < m_deiviceInfo.videoInputNum; i++)
				{
					memcpy(pTempCmdData, &(pTrigger[i].sendEmail), sizeof(unsigned char));
					pTempCmdData += sizeof(unsigned long);
				}
				for (int i = 0; i < m_deiviceInfo.videoInputNum; i++)
				{
					memcpy(pTempCmdData, &(pTrigger[i].toBuzzer), sizeof(unsigned char));
					pTempCmdData += sizeof(unsigned long);
				}

				ITEM_INFO item_info;
				item_info.parserLen = len;
				item_info.pConfigItem = (unsigned char *)pbuf;
				iteminfolist.push_back(item_info);
				datalen += len;
				break;
			}
		case DD_CONFIG_ITEM_VLOSS_ALARM_OUT:
			{
				unsigned long len = sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD)*DD_TRIGGER_ALARM_OUT_NUM + (sizeof(ULONGLONG)*2 + sizeof(unsigned long)*2) * m_deiviceInfo.videoInputNum;
				unsigned char *pbuf = new unsigned char[len];
				memset(pbuf, 0 ,sizeof(unsigned char)*len);
				unsigned char *pTempCmdData = pbuf;

				blockHead.ItemNum = DD_TRIGGER_ALARM_OUT_NUM;
				memcpy(pTempCmdData, &blockHead, sizeof(NCFG_BLOCK_HEAD));
				pTempCmdData += sizeof(NCFG_BLOCK_HEAD);
				//填充配置项头
				NCFG_ITEM_HEAD item[DD_TRIGGER_ALARM_OUT_NUM];
				NCFG_ITEM_HEAD *pHead = item;
				pHead->itemID = NCFG_ITEM_VL_TO_ALARM_OUT;
				pHead->len = sizeof(ULONGLONG) * m_deiviceInfo.videoInputNum;
				pHead->num = m_deiviceInfo.videoInputNum;
				pHead->subLen = sizeof(ULONGLONG);
				memcpy(pTempCmdData, pHead, sizeof(NCFG_ITEM_HEAD));
				pHead++;
				pTempCmdData += sizeof(NCFG_ITEM_HEAD);

				pHead->itemID = NCFG_ITEM_VL_TO_BIG_VIEW;
				pHead->len = sizeof(ULONGLONG) * m_deiviceInfo.videoInputNum;
				pHead->num = m_deiviceInfo.videoInputNum;
				pHead->subLen = sizeof(ULONGLONG);
				memcpy(pTempCmdData, pHead, sizeof(NCFG_ITEM_HEAD));
				pHead++;
				pTempCmdData += sizeof(NCFG_ITEM_HEAD);

				pHead->itemID = NCFG_ITEM_VL_TO_EMAIL_ENABLE;
				pHead->len = sizeof(unsigned long) * m_deiviceInfo.videoInputNum;
				pHead->num = m_deiviceInfo.videoInputNum;
				pHead->subLen = sizeof(unsigned long);
				memcpy(pTempCmdData, pHead, sizeof(NCFG_ITEM_HEAD));
				pHead++;
				pTempCmdData += sizeof(NCFG_ITEM_HEAD);

				pHead->itemID = NCFG_ITEM_VL_TO_BUZZ;
				pHead->len = sizeof(unsigned long) * m_deiviceInfo.videoInputNum;
				pHead->num = m_deiviceInfo.videoInputNum;
				pHead->subLen = sizeof(unsigned long);
				memcpy(pTempCmdData, pHead, sizeof(NCFG_ITEM_HEAD));
				pTempCmdData += sizeof(NCFG_ITEM_HEAD);

				DD_TRIGGER_ALARM_OUT* pTrigger =  (DD_TRIGGER_ALARM_OUT*)pItemData;
				for (int i = 0; i < m_deiviceInfo.videoInputNum; i++)
				{
					memcpy(pTempCmdData, &(pTrigger[i].toAlarmOut), sizeof(unsigned long));
					pTempCmdData += sizeof(ULONGLONG);
				}
				for (int i = 0; i < m_deiviceInfo.videoInputNum; i++)
				{
					unsigned long long *test= (unsigned long long *)pTempCmdData;

					if (pTrigger[i].ShowFullScreen == 0xff)
					{
						*test = 0;
					}
					else
					{
						*test = (0x1) << pTrigger[i].ShowFullScreen;
					}
					pTempCmdData += sizeof(unsigned long long);
				}
				for (int i = 0; i < m_deiviceInfo.videoInputNum; i++)
				{
					memcpy(pTempCmdData, &(pTrigger[i].sendEmail), sizeof(unsigned char));
					pTempCmdData += sizeof(unsigned long);
				}
				for (int i = 0; i < m_deiviceInfo.videoInputNum; i++)
				{
					memcpy(pTempCmdData, &(pTrigger[i].toBuzzer), sizeof(unsigned char));
					pTempCmdData += sizeof(unsigned long);
				}

				ITEM_INFO item_info;
				item_info.parserLen = len;
				item_info.pConfigItem = (unsigned char *)pbuf;
				iteminfolist.push_back(item_info);
				datalen += len;
				break;
			}
		case DD_CONFIG_ITEM_SHELTER_SCHEDULE:
			{
				break;
			}
		case DD_CONFIG_ITEM_VLOSS_SCHEDULE:
			{
				break;
			}
		case DD_CONFIG_ITEM_RELAY_SETUP:
			{
				unsigned long len = sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD)*DD_RELAY_CONFIG_NUM + (sizeof(NCFG_INFO_SHORT_NAME) + sizeof(unsigned long)) * m_deiviceInfo.sensorOutputNum;
				unsigned char *pbuf = new unsigned char[len];
				memset(pbuf, 0 ,sizeof(unsigned char)*len);
				unsigned char *pTempCmdData = pbuf;

				blockHead.ItemNum = DD_RELAY_CONFIG_NUM;
				memcpy(pTempCmdData, &blockHead, sizeof(NCFG_BLOCK_HEAD));
				pTempCmdData += sizeof(NCFG_BLOCK_HEAD);
				//填充配置项头
				NCFG_ITEM_HEAD item[DD_RELAY_CONFIG_NUM];
				NCFG_ITEM_HEAD *pHead = item;
				pHead->itemID = NCFG_ITEM_ALARM_OUT_NAME;
				pHead->len = sizeof(NCFG_INFO_SHORT_NAME) * m_deiviceInfo.sensorOutputNum;
				pHead->num = m_deiviceInfo.sensorOutputNum;
				pHead->subLen = sizeof(NCFG_INFO_SHORT_NAME);
				memcpy(pTempCmdData, pHead, sizeof(NCFG_ITEM_HEAD));
				pHead++;
				pTempCmdData += sizeof(NCFG_ITEM_HEAD);

				pHead->itemID = NCFG_ITEM_ALARM_OUT_HOLD_TIME;
				pHead->len = sizeof(unsigned long) * m_deiviceInfo.sensorOutputNum;
				pHead->num = m_deiviceInfo.sensorOutputNum;
				pHead->subLen = sizeof(unsigned long);
				memcpy(pTempCmdData, pHead, sizeof(NCFG_ITEM_HEAD));
				pTempCmdData += sizeof(NCFG_ITEM_HEAD);

				DD_RELAY_CONFIG* pRelay =  (DD_RELAY_CONFIG*)pItemData;
				NCFG_INFO_SHORT_NAME short_name;
				memset(&short_name, 0, sizeof(NCFG_INFO_SHORT_NAME));
				for (int i = 0; i < m_deiviceInfo.sensorOutputNum; i++)
				{
					strcpy(short_name.name, pRelay[i].name);
					memcpy(pTempCmdData, short_name.name, sizeof(NCFG_INFO_SHORT_NAME));
					pTempCmdData += sizeof(NCFG_INFO_SHORT_NAME);
				}
				for (int i = 0; i < m_deiviceInfo.sensorOutputNum; i++)
				{
					unsigned long test = pRelay[i].holdTime;
					if (test == 0xffff)
					{
						test = 0xffffffff;
					}
					memcpy(pTempCmdData, &test, sizeof(unsigned long));
					pTempCmdData += sizeof(unsigned long);
				}

				ITEM_INFO item_info;
				item_info.parserLen = len;
				item_info.pConfigItem = (unsigned char *)pbuf;
				iteminfolist.push_back(item_info);
				datalen += len;
				break;
			}
		case DD_CONFIG_ITEM_RELAY_SCHEDULE:
			{
				break;
			}
		case DD_CONFIG_ITEM_BUZZER_SETUP:
			{
				unsigned long len = sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD)*DD_BUZZER_CONFIG_NUM + sizeof(unsigned long)*DD_BUZZER_CONFIG_NUM;
				unsigned char *pbuf = new unsigned char[len];
				memset(pbuf, 0 ,sizeof(unsigned char)*len);
				unsigned char *pTempCmdData = pbuf;

				blockHead.ItemNum = DD_BUZZER_CONFIG_NUM;
				memcpy(pTempCmdData, &blockHead, sizeof(NCFG_BLOCK_HEAD));
				pTempCmdData += sizeof(NCFG_BLOCK_HEAD);
				//填充配置项头
				NCFG_ITEM_HEAD item[DD_BUZZER_CONFIG_NUM];
				NCFG_ITEM_HEAD *pHead = item;
				pHead->itemID = NCFG_ITEM_BUZZER_ENABLE;
				pHead->len = sizeof(unsigned long);
				pHead->num = 1;
				pHead->subLen = sizeof(unsigned long);
				memcpy(pTempCmdData, pHead, sizeof(NCFG_ITEM_HEAD));
				pHead++;
				pTempCmdData += sizeof(NCFG_ITEM_HEAD);

				pHead->itemID = NCFG_ITEM_BUZZER_HOLD_TIME;
				pHead->len = sizeof(unsigned long);
				pHead->num = 1;
				pHead->subLen = sizeof(unsigned long);
				memcpy(pTempCmdData, pHead, sizeof(NCFG_ITEM_HEAD));
				pTempCmdData += sizeof(NCFG_ITEM_HEAD);

				DD_BUZZER_CONFIG* pBuzzer =  (DD_BUZZER_CONFIG*)pItemData;
				memcpy(pTempCmdData, &(pBuzzer->enable), sizeof(unsigned char));
				pTempCmdData += sizeof(unsigned long);
				unsigned long test = pBuzzer->holdTime;
				if (test == 0xffff)
				{
					test = 0xffffffff;
				}
				memcpy(pTempCmdData, &test, sizeof(unsigned long));

				ITEM_INFO item_info;	
				item_info.parserLen = len;
				item_info.pConfigItem = (unsigned char *)pbuf;
				iteminfolist.push_back(item_info);
				datalen += len;
				break;
			}
		case DD_CONFIG_ITEM_BUZZER_SCHEDULE:
			{
				break;
			}
		case DD_CONFIG_ITEM_PTZ_SETUP:
			{
				unsigned long len = sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD)*DD_PTZ_CONFIG_NUM + sizeof(PTZ_SERIAL_INFO) * DD_PTZ_CONFIG_NUM * m_deiviceInfo.videoInputNum;
				unsigned char *pbuf = new unsigned char[len];
				memset(pbuf, 0 ,sizeof(unsigned char)*len);
				unsigned char *pTempCmdData = pbuf;

				blockHead.ItemNum = DD_PTZ_CONFIG_NUM;
				memcpy(pTempCmdData, &blockHead, sizeof(NCFG_BLOCK_HEAD));
				pTempCmdData += sizeof(NCFG_BLOCK_HEAD);
				//填充配置项头
				NCFG_ITEM_HEAD item[DD_PTZ_CONFIG_NUM];
				NCFG_ITEM_HEAD *pHead = item;
				pHead->itemID = NCFG_ITEM_PTZ_SERIAL;
				pHead->len = sizeof(PTZ_SERIAL_INFO) * m_deiviceInfo.videoInputNum;
				pHead->num = m_deiviceInfo.videoInputNum;
				pHead->subLen = sizeof(PTZ_SERIAL_INFO);
				memcpy(pTempCmdData, pHead, sizeof(NCFG_ITEM_HEAD));
				pTempCmdData += sizeof(NCFG_ITEM_HEAD);

				DD_PTZ_CONFIG* pPtz =  (DD_PTZ_CONFIG*)pItemData;
				PTZ_SERIAL_INFO serial_info;
				for (int i = 0; i < m_deiviceInfo.videoInputNum; i++)
				{
					serial_info.baudRate = (unsigned char)pPtz[i].serial.baudRate;
					serial_info.bEnable = pPtz[i].enable;
					serial_info.devAddr = pPtz[i].address;
					serial_info.protocol = (unsigned char)pPtz[i].protocol;
					memcpy(pTempCmdData, &serial_info, sizeof(PTZ_SERIAL_INFO));
					pTempCmdData += sizeof(PTZ_SERIAL_INFO);
				}
				
				ITEM_INFO item_info;
				item_info.parserLen = len;
				item_info.pConfigItem = (unsigned char *)pbuf;
				iteminfolist.push_back(item_info);
				datalen += len;
				break;
			}
		case DD_CONFIG_ITEM_PTZ_PRESET:
			{
				break;
			}
		case DD_CONFIG_ITEM_PTZ_CRUISE:
			{
				break;
			}
		}
	}
	unsigned char *pCfgBuf = new unsigned char[datalen];
	memset(pCfgBuf, 0, sizeof(unsigned char)*datalen);
	unsigned char *pTempBuf = pCfgBuf;
	for (its = iteminfolist.begin(); its != iteminfolist.end(); its++)
	{
		memcpy(pTempBuf, (*its).pConfigItem, (*its).parserLen);
		pTempBuf += (*its).parserLen;
		//delete[] (*its).pConfigItem;
	}

	memcpy(pDeviceCfg, pCfgBuf, datalen);
	deviceCfgLen = datalen;

	pConfigBlock.DestroyBlock();

	return deviceCfgLen;
}

void CConfigParser_V3::SetDeivceInfo(LPNET_SDK_DEVICEINFO deviceInfo)
{
	memcpy(&m_deiviceInfo, deviceInfo, sizeof(NET_SDK_DEVICEINFO));
}
long CConfigParser_V3::GetCfgItemLength(unsigned short itemNum, long cfgParamLen)
{
	long len = sizeof(NCFG_ITEM_HEAD) * itemNum;

	if (cfgParamLen < len)
	{
		assert(FALSE);
		return -1;
	}
	return len;
}

void CConfigParser_V3::SetCfgItemInfo(unsigned short itemNum, unsigned char *pBuf, unsigned short *pArray)
{
	for (int i = 0; i < itemNum; i++)
	{
		NCFG_ITEM_HEAD pTempItemHead;
		memset(&pTempItemHead, 0, sizeof(NCFG_ITEM_HEAD));
		pTempItemHead.itemID = pArray[i];
		memcpy(pBuf, &pTempItemHead, sizeof(NCFG_ITEM_HEAD));
		pBuf += sizeof(NCFG_ITEM_HEAD);
	}
}

unsigned long CConfigParser_V3::ToSDKResolution(unsigned long videoSize)
{
	unsigned long ret = 0;

	switch(videoSize)
	{
	case DVRVIDEO_SIZE_QCIF:
		ret = DD_VIDEO_SIZE_QCIF;
		break;
	case DVRVIDEO_SIZE_CIF:
		ret = DD_VIDEO_SIZE_CIF;
		break;
	case DVRVIDEO_SIZE_HD1:
		ret = DD_VIDEO_SIZE_HD1;
		break;
	case DVRVIDEO_SIZE_D1:
		ret = DD_VIDEO_SIZE_D1;
		break;
	case DVRVIDEO_SIZE_VGA:
		ret = DD_VIDEO_SIZE_VGA;
		break;
	case DVRVIDEO_SIZE_HD1080:
		ret = DD_VIDEO_SIZE_1080P;
		break;
	case DVRVIDEO_SIZE_HD720:
		ret = DD_VIDEO_SIZE_720P;
		break;


	case DVRVIDEO_SIZE_960P:
		ret = DD_VIDEO_SIZE_960P;
		break;
	case DVRVIDEO_SIZE_SXGA:
		ret = DD_VIDEO_SIZE_SXGA;
		break;
	case DVRVIDEO_SIZE_3M:
		ret = DD_VIDEO_SIZE_3M;
		break;
	case DVRVIDEO_SIZE_16_9_3M:
		ret = DD_VIDEO_SIZE_16_9_3M;
		break;
	case DVRVIDEO_SIZE_4M:
		ret = DD_VIDEO_SIZE_4M;
		break;
	case DVRVIDEO_SIZE_5M:
		ret = DD_VIDEO_SIZE_5M;
		break;
	default:
		break;
	}

	return ret;
}
