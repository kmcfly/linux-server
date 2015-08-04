#include <sys/types.h>
#include <dirent.h>
#include <asm/unistd.h>
#include "TvtDevInterface.h"
#include "Product.h"
#include "LocalDevice.h"
#include "ConfigEx.h"
#include "dvrdvsconfig.h"
#include "PtzMan.h"
#include "ReadDdns.h"
#include "APIStreamDispatch.h"

extern std::string g_zoneName[];

CTvtDevInterface::CTvtDevInterface()
{
	m_iMaxChannels = 0;
}

CTvtDevInterface::~CTvtDevInterface()
{

}

tint32 CTvtDevInterface::WebProcExt( const std::string& strWebContent, NETNS::HTTP_STATUS_CODE& status, std::string& strWebReponse) 
{
	status = NETNS::HTTP_STATUS_NOT_FOUND;
	strWebReponse = "NotFound!";
	return NETNS::DEV_STATUS_SUCCESS;
}


/*
* support function
*/
NETNS::DEV_RET CTvtDevInterface::SupportIntegratedPTZ(bool &bSupport) 
{
	bSupport = true;
	return NETNS::DEV_STATUS_SUCCESS;
}
NETNS::DEV_RET CTvtDevInterface::SupportRS485PTZ(bool &bSupport) 
{
	bSupport = true;
	return NETNS::DEV_STATUS_SUCCESS;
}
NETNS::DEV_RET CTvtDevInterface::SupportSDCard(bool &bSupport) 
{
	bSupport = true;
	return NETNS::DEV_STATUS_SUCCESS;
}


NETNS::DEV_RET CTvtDevInterface::GetUserInfo( NETNS::DEV_USER_INFO *pCfg, tuint32 nSize, tuint32& nNeedSize) 
{
	unsigned char *pData = NULL;
	unsigned long dataLen = 0;
	CMyList <NCFG_ITEM_ID> itemList;
	itemList.AddTail(NCFG_ITEM_USER);
	if (!CConfigEx::Instance()->GetConfigEx(itemList, &pData, dataLen))
	{
		return NETNS::DEV_STATUS_SYSTEM_BUSY;
	}
	if (dataLen < sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD) + MAX_USER_NUM*sizeof(NCFG_INFO_USER))
	{
		delete []pData;
		return NETNS::DEV_STATUS_PARAM_ERR;
	}
	NCFG_INFO_USER *UserInfo = (NCFG_INFO_USER*)(pData + sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD));
	int i=0;
	for (; i<nSize && i<MAX_USER_NUM; i++)
	{
		if (strlen(UserInfo[i].name) <= 0)
		{
			break;
		}
		snprintf(pCfg[i].szUserName, sizeof(pCfg[i].szUserName), "%s", UserInfo[i].name);
		snprintf(pCfg[i].szPassword, sizeof(pCfg[i].szPassword), "%s", UserInfo[i].password);

		//USER_GROUP == NETNS::DEV_USER_GROUP
		pCfg[i].dwUserType = (NETNS::DEV_USER_GROUP)(UserInfo[i].group);
	}
	nNeedSize = i;
	delete []pData;

	return NETNS::DEV_STATUS_SUCCESS;
}

NETNS::DEV_RET CTvtDevInterface::GetDeviceInfo( const char* pUser, NETNS::DEV_INFO &info) 
{
	if (!CheckUserPass(pUser))
	{
		return NETNS::DEV_STATUS_PERMISSION_DENIED;
	}
	NETNS::DEV_RET ret = NETNS::DEV_STATUS_SUCCESS;

	PRODUCT_INFO product;
	CProduct::Instance()->GetProductInfo(product, "", "", "", "");

	snprintf(info.szMode, sizeof(info.szMode), "%s", product.deviceName);
	snprintf(info.szBrand, sizeof(info.szBrand), "%s", product.deviceName);
	snprintf(info.szDevDescription, sizeof(info.szDevDescription), "%s", product.deviceName);
	snprintf(info.szSoftwareVersion, sizeof(info.szSoftwareVersion), "%s", product.firmwareVersion);
	info.softwareBuildDate.wYear = (product.buildDate >> 16) & 0xffff;
	info.softwareBuildDate.wMonth = (product.buildDate >> 8) & 0xff;
	info.softwareBuildDate.wDay = product.buildDate & 0xff;;
	snprintf(info.szKernelVersion, sizeof(info.szKernelVersion), "%s", product.kernelVersion);
	snprintf(info.szHardwareVersion, sizeof(info.szHardwareVersion), "%s", product.hardwareVersion);
	memcpy(info.macAddr, product.deviceMAC, sizeof(info.macAddr));	
	info.wAudioInCount = product.audioInputNum + product.netVideoInputNum;
	//audioOut number无法获取
	info.wAudioOutCount = product.localVideoInputNum + product.netVideoInputNum;
	info.wAlarmInCount = product.sensorInputNum;
	info.wAlarmOutCount = product.relayOutputNum;

	return ret;
}

NETNS::DEV_RET CTvtDevInterface::GetSystemTime( const char* pUser, NETNS::DEV_SYSTIME &sysTime) 
{
	if (!CheckUserPass(pUser))
	{
		return NETNS::DEV_STATUS_PERMISSION_DENIED;
	}

	CMyList <NCFG_ITEM_ID> itemList;
	itemList.AddTail(NCFG_ITEM_TIME_SYNC);
	unsigned char *pData = NULL;
	unsigned long dataLen = 0;
	if (!CConfigEx::Instance()->GetConfigEx(itemList, &pData, dataLen))
	{
		return NETNS::DEV_STATUS_SYSTEM_BUSY;
	}
	if (dataLen < sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD) + sizeof(unsigned long))
	{
		printf("%s %d err\n", __FILE__, __LINE__);
		delete []pData;
		return NETNS::DEV_STATUS_SYSTEM_BUSY;
	}
	sysTime.adjustTime.synchronizeType = *((unsigned long*)(pData + sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD))) > 0 ? NETNS::DATETIME_NTP : NETNS::DATETIME_MANUAL;
	delete []pData;
//
	itemList.RemoveAll();
	itemList.AddTail(NCFG_ITEM_TIME_SERVER);
	if (!CConfigEx::Instance()->GetConfigEx(itemList, &pData, dataLen))
	{
		return NETNS::DEV_STATUS_SYSTEM_BUSY;
	}
	if (dataLen < sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD) + sizeof(NCFG_INFO_LONG_NAME))
	{
		delete []pData;
		return NETNS::DEV_STATUS_SYSTEM_BUSY;
	}
	strncpy(sysTime.adjustTime.szNtpServer, (const char*)(pData + sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD)), sizeof(sysTime.adjustTime.szNtpServer));
	delete []pData;
//
	itemList.RemoveAll();
	itemList.AddTail(NCFG_ITEM_TIME_ZONE);
	if (!CConfigEx::Instance()->GetConfigEx(itemList, &pData, dataLen))
	{
		return NETNS::DEV_STATUS_SYSTEM_BUSY;
	}
	if (dataLen < sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD) + sizeof(long))
	{
		delete []pData;
		return NETNS::DEV_STATUS_SYSTEM_BUSY;
	}
	char timeZone[128] = {0};
	int i = *(long*)(pData + sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD));
	if (i < 0 || i > 34)
	{
		delete []pData;
		return NETNS::DEV_STATUS_SYSTEM_BUSY;
	}
	strncpy(timeZone, g_zoneName[i].c_str(), sizeof(timeZone));
	delete []pData;
//	
	itemList.RemoveAll();
	itemList.AddTail(NCFG_ITEM_DAYLIGHT_INFO);
	if (!CConfigEx::Instance()->GetConfigEx(itemList, &pData, dataLen))
	{
		return NETNS::DEV_STATUS_SYSTEM_BUSY;
	}
	if (dataLen < sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD) + sizeof(DAYLIGHT_INFO))
	{
		delete []pData;
		return NETNS::DEV_STATUS_SYSTEM_BUSY;
	}
	DaylightInfoToStr((DAYLIGHT_INFO*)(pData + sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD)), timeZone+strlen(timeZone), sizeof(timeZone)-strlen(timeZone));
	strncpy(sysTime.timezone.szTimeZone, timeZone, sizeof(sysTime.timezone.szTimeZone));
	sysTime.timezone.bDaylightSwitch = ((DAYLIGHT_INFO*)(pData + sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD)))->enable > 0;
	delete []pData;

	return NETNS::DEV_STATUS_SUCCESS;
}

NETNS::DEV_RET CTvtDevInterface::SetSystemTime( const char* pUser, const NETNS::DEV_SYSTIME& sysTime) 
{
	if (!CheckUserPass(pUser))
	{
		return NETNS::DEV_STATUS_PERMISSION_DENIED;
	}

	CMyList <NCFG_ITEM_ID> itemList;
	itemList.AddTail(NCFG_ITEM_TIME_SYNC);
	unsigned char *pData = NULL;
	unsigned long dataLen = 0;
	if (!CConfigEx::Instance()->GetConfigEx(itemList, &pData, dataLen))
	{
		return NETNS::DEV_STATUS_SYSTEM_BUSY;
	}
	if (dataLen < sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD) + sizeof(unsigned long))
	{
		printf("%s %d err\n", __FILE__, __LINE__);
		delete []pData;
		return NETNS::DEV_STATUS_SYSTEM_BUSY;
	}
	unsigned long *syncronize = (reinterpret_cast<unsigned long*>(pData + sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD)));
	*syncronize = sysTime.adjustTime.synchronizeType == NETNS::DATETIME_NTP ? 1 : 0;
	if (!SaveConfig(pData, dataLen))
	{
		delete []pData;
		return NETNS::DEV_STATUS_SYSTEM_BUSY;
	}
	delete []pData;
//
	itemList.RemoveAll();
	itemList.AddTail(NCFG_ITEM_TIME_SERVER);
	if (!CConfigEx::Instance()->GetConfigEx(itemList, &pData, dataLen))
	{
		return NETNS::DEV_STATUS_SYSTEM_BUSY;
	}
	if (dataLen < sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD) + sizeof(NCFG_INFO_LONG_NAME))
	{
		delete []pData;
		return NETNS::DEV_STATUS_SYSTEM_BUSY;
	}
	strncpy((char*)(pData + sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD)), sysTime.adjustTime.szNtpServer, sizeof(sysTime.adjustTime.szNtpServer));
	if (!SaveConfig(pData, dataLen))
	{
		delete []pData;
		return NETNS::DEV_STATUS_SYSTEM_BUSY;
	}
	delete []pData;
//
	itemList.RemoveAll();
	itemList.AddTail(NCFG_ITEM_TIME_ZONE);
	if (!CConfigEx::Instance()->GetConfigEx(itemList, &pData, dataLen))
	{
		return NETNS::DEV_STATUS_SYSTEM_BUSY;
	}
	if (dataLen < sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD) + sizeof(long))
	{
		delete []pData;
		return NETNS::DEV_STATUS_SYSTEM_BUSY;
	}

	DAYLIGHT_INFO daylightInfo = {0};
	char timeZone[16] = {0};
	if (!StrToDaylightInfo(sysTime.timezone.szTimeZone, sizeof(sysTime.timezone.szTimeZone), &daylightInfo, timeZone))
	{
		delete []pData;
		return NETNS::DEV_STATUS_SYSTEM_BUSY;
	}
	//timeZone转换成g_zoneName下标
	int iTimeZone = 0;
	if (!CalcTimeZoneIndex(g_zoneName, 34, timeZone, iTimeZone))
	{
		delete []pData;
		return NETNS::DEV_STATUS_SYSTEM_BUSY;
	}

	*((long*)(pData + sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD))) = iTimeZone;
	if (!SaveConfig(pData, dataLen))
	{
		delete []pData;
		return NETNS::DEV_STATUS_SYSTEM_BUSY;
	}
	delete []pData;
//
	itemList.RemoveAll();
	itemList.AddTail(NCFG_ITEM_DAYLIGHT_INFO);
	if (!CConfigEx::Instance()->GetConfigEx(itemList, &pData, dataLen))
	{
		return NETNS::DEV_STATUS_SYSTEM_BUSY;
	}
	if (dataLen < sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD) + sizeof(DAYLIGHT_INFO))
	{
		delete []pData;
		return NETNS::DEV_STATUS_SYSTEM_BUSY;
	}
	memcpy((DAYLIGHT_INFO*)(pData + sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD)), &daylightInfo, sizeof(DAYLIGHT_INFO));
	((DAYLIGHT_INFO*)(pData + sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD)))->enable = sysTime.timezone.bDaylightSwitch ? 1 : 0;
	if (!SaveConfig(pData, dataLen))
	{
		delete []pData;
		return NETNS::DEV_STATUS_SYSTEM_BUSY;
	}
	delete []pData;
	return NETNS::DEV_STATUS_SUCCESS;
}

NETNS::DEV_RET CTvtDevInterface::SetCurTime( const char* pUser, const NETNS::DEV_DATETIME& datetime)
{
	if (!CheckUserPass(pUser))
	{
		return NETNS::DEV_STATUS_PERMISSION_DENIED;
	}

	unsigned long time = 0;
	TimeConvert(datetime.date, datetime.time, time);
	CMessageMan::Instance()->PostMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_L_CHANGE_TIME, (unsigned char*)&time, sizeof(unsigned long));

	return NETNS::DEV_STATUS_SUCCESS;
}

NETNS::DEV_RET CTvtDevInterface::GetImageCaps( const char* pUser, tuint32 nChnID, NETNS::DEV_CAMERA_PARAM_CAPS &caps) 
{
	if (!CheckUserPass(pUser))
	{
		return NETNS::DEV_STATUS_PERMISSION_DENIED;
	}

	unsigned long defaultColor = CLocalDevice::Instance()->GetImageDefaultColor();
	caps.bright.nMin = 0;
	caps.contrast.nMin = 0;
	caps.saturation.nMin = 0;
	caps.hue.nMin = 0;
	caps.bright.nMax = 255;
	caps.contrast.nMax = 255;
	caps.saturation.nMax = 255;
	caps.hue.nMax = 255;
	caps.bright.nDef = (unsigned char)(defaultColor>>24);
	caps.contrast.nDef = (unsigned char)(defaultColor>>16);
	caps.saturation.nDef = (unsigned char)(defaultColor>>8);
	caps.hue.nDef = (unsigned char)(defaultColor);

	return  NETNS::DEV_STATUS_SUCCESS;
}

NETNS::DEV_RET CTvtDevInterface::GetImageParam( const char* pUser, tuint32 nChnID, NETNS::DEV_CAMERA_PARAM &param) 
{
	if (!CheckUserPass(pUser))
	{
		return NETNS::DEV_STATUS_PERMISSION_DENIED;
	}

	CMyList <NCFG_ITEM_ID> itemList;
	itemList.AddTail(NCFG_ITEM_VIDEO_COLOR);
	unsigned char *pData = NULL;
	unsigned long dataLen = 0;
	if (!CConfigEx::Instance()->GetConfigEx(itemList, &pData, dataLen))
	{
		return NETNS::DEV_STATUS_SYSTEM_BUSY;
	}
	if (dataLen < sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD) + (nChnID+1) * sizeof(unsigned long))
	{
		delete []pData;
		return NETNS::DEV_STATUS_PARAM_ERR;
	}
	unsigned long *color = (unsigned long*)(pData + sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD) + nChnID * sizeof(unsigned long));

	param.ucBright = (unsigned char)(*color>>24);
	param.ucContrast = (unsigned char)(*color>>16);
	param.ucSaturation = (unsigned char)(*color>>8);
	param.ucHue = (unsigned char)(*color);
	delete []pData;
//
/*Get不到
	itemList.RemoveAll();
	itemList.AddTail(NCFG_ITEM_CAM_MIRROR);
	if (!CConfigEx::Instance()->GetConfigEx(itemList, &pData, dataLen))
	{
		return NETNS::DEV_STATUS_SYSTEM_BUSY;
	}
	if (dataLen < sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD) + sizeof(unsigned char))
	{
		delete []pData;
		return NETNS::DEV_STATUS_PARAM_ERR;
	}
	param.bMirrorSwitch = *pData > 0;
	delete []pData;
*/
//

	return NETNS::DEV_STATUS_SUCCESS;
}

NETNS::DEV_RET CTvtDevInterface::SetImageParam( const char* pUser, tuint32 nChnID, NETNS::DEV_CAMERA_PARAM &param) 
{
	if (!CheckUserPass(pUser))
	{
		return NETNS::DEV_STATUS_PERMISSION_DENIED;

	}
	CMyList <NCFG_ITEM_ID> itemList;
	itemList.AddTail(NCFG_ITEM_VIDEO_COLOR);
	unsigned char *pData = NULL;
	unsigned long dataLen = 0;
	if (!CConfigEx::Instance()->GetConfigEx(itemList, &pData, dataLen))
	{
		return NETNS::DEV_STATUS_SYSTEM_BUSY;
	}
	if (dataLen < sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD) + (nChnID+1) * sizeof(unsigned long))
	{
		delete []pData;
		return NETNS::DEV_STATUS_PARAM_ERR;
	}
	unsigned long *color = (unsigned long*)(pData + sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD) + nChnID * sizeof(unsigned long));
	*color = 0;
	*color |= param.ucBright << 24;
	*color |= param.ucContrast << 16;
	*color |= param.ucSaturation << 8;
	*color |= param.ucHue;

	if (!SaveConfig(pData, dataLen))
	{
		delete []pData;
		return NETNS::DEV_STATUS_SYSTEM_BUSY;
	}
	
	delete []pData;

	return NETNS::DEV_STATUS_SUCCESS;
}	

NETNS::DEV_RET CTvtDevInterface::GetSnapshot( const char* pUser, tuint32 nChnID, char*& pPic, tuint32& nPicSize) 
{
	if (!CheckUserPass(pUser))
	{
		return NETNS::DEV_STATUS_PERMISSION_DENIED;
	}

	//TODO
	nPicSize = 0;
	FRAME_INFO_EX frame;
	int countDown = 100;
	while(countDown-- > 0 && 0 == CLocalDevice::Instance()->GetJPEGStream(frame))
	{
		if (frame.channel == nChnID)
		{
			nPicSize = frame.length;
			pPic = new char[nPicSize];
			memcpy(pPic, frame.pData, nPicSize);	
			countDown = 0;
		}

		if (frame.pData)
		{
			delete []frame.pData;
		}
	}

	if (nPicSize == 0)
	{
		return NETNS::DEV_STATUS_SYSTEM_BUSY;
	}

	return NETNS::DEV_STATUS_SUCCESS;
}

// stream
NETNS::DEV_RET CTvtDevInterface::GetStreamCaps( const char* pUser, tuint32 nChnID, tuint32 nStreamIdx, NETNS::DEV_SREAM_CAPS &caps)
{
	if (!CheckUserPass(pUser))
	{
		return NETNS::DEV_STATUS_PERMISSION_DENIED;
	}

	unsigned char *pData = NULL;
	unsigned long dataLen = 0;
	CMyList <NCFG_ITEM_ID> itemList;

	if (nStreamIdx == 0)
	{
		itemList.AddTail(NCFG_ITEM_ENCODE_MAJOR_EX);
	}
	else if(nStreamIdx == 1)
	{
		itemList.AddTail(NCFG_ITEM_ENCODE_MINOR_EX);
	}
	else
	{
		return NETNS::DEV_STATUS_PARAM_ERR;
	}
	if (!CConfigEx::Instance()->GetConfigEx(itemList, &pData, dataLen))
	{
		return NETNS::DEV_STATUS_SYSTEM_BUSY;
	}
	if (dataLen < sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD) + (nChnID+1) * sizeof(ENCODE_INFO_EX))
	{
		delete []pData;
		return NETNS::DEV_STATUS_SYSTEM_BUSY;
	}
	ENCODE_INFO_EX *pEncodeInfo = (ENCODE_INFO_EX *)(pData + sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD) + nChnID * sizeof(ENCODE_INFO_EX));

	caps.bitrate.nMin = pEncodeInfo->hBitStream;
	caps.bitrate.nMax = pEncodeInfo->hBitStream;
	caps.encodeType[0] = NETNS::VIDEO_ENCODING_H264;
	caps.encodeTypeSize = 1;

	caps.frmRate.nMin = 1;
	caps.frmRate.nMax = pEncodeInfo->rate;

	delete []pData;
//
	itemList.RemoveAll();
	if (nStreamIdx == 0)
	{
		itemList.AddTail(NCFG_ITEM_ENCODE_MAJOR_SUPPORT_PROPERTY);
	}
	else if(nStreamIdx == 1)
	{
		itemList.AddTail(NCFG_ITEM_ENCODE_MINOR_SUPPORT_PROPERTY);
	}
	else
	{
		return NETNS::DEV_STATUS_PARAM_ERR;
	}
	if (!CConfigEx::Instance()->GetConfigEx(itemList, &pData, dataLen))
	{
		return NETNS::DEV_STATUS_SYSTEM_BUSY;
	}

	if (dataLen >= sizeof(NCFG_INFO_SUPPORT_FRAME_RATE) + sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD))
	{
		NCFG_INFO_SUPPORT_FRAME_RATE *pConf = reinterpret_cast<NCFG_INFO_SUPPORT_FRAME_RATE*>(pData + sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD));
		GetResolution(pConf->dwResolution, caps.resolutions, sizeof(caps.resolutions)/sizeof(NETNS::DEV_RESOLUTION), caps.resolutionsSize, g_video_format == VIDEO_FORMAT_PAL);
		printf("GetResolution %d  %d\n", dataLen, pConf->dwResolution);
	}
	delete []pData;
//
	caps.profiles[0] = NETNS::H264_PROFILE_BASELINE;
	caps.profiles[1] = NETNS::H264_PROFILE_MAIN;
	caps.profiles[2] = NETNS::H264_PROFILE_EXTENDED;
	caps.profiles[3] = NETNS::H264_PROFILE_HIGH;
	caps.profilesSize = 4;

	caps.IFrmInterval.nMax = 300;
	caps.IFrmInterval.nMin = 20;
	
	return NETNS::DEV_STATUS_SUCCESS;
}

NETNS::DEV_RET CTvtDevInterface::GetStreamName( const char* pUser, tuint32 nChnID, tuint32 nSreamIdx,  char* pStreamName, tuint32 nNameLen, tuint32& nNeedNameLen)
{
	if (!CheckUserPass(pUser))
	{
		return NETNS::DEV_STATUS_PERMISSION_DENIED;
	}

	if (nSreamIdx == 0)
	{
		snprintf(pStreamName, nNameLen, "chID=%d&streamType=main&linkType=tcp", nChnID);
	}
	else if(nSreamIdx == 1)
	{
		snprintf(pStreamName, nNameLen, "chID=%d&streamType=sub&linkType=tcp", nChnID);
	}
	else
	{
		return NETNS::DEV_STATUS_PARAM_ERR;
	}

	nNeedNameLen = strlen(pStreamName);

	return NETNS::DEV_STATUS_SUCCESS;
}


tuint32 CTvtDevInterface::GetStreamCnt(  tuint32 nChnID ) 
{
	return 2;
}

tint32 CTvtDevInterface::SupportAudioEncode(  tuint32 nChnID, NETNS::DEV_AUDIO_ENCODE encode) 
{
	if (NETNS::AUDIO_ENCODE_G711A == encode)
	{
		return true;
	}
	return true;
}

NETNS::DEV_RET CTvtDevInterface::SetAudioEncode( const char* pUser, tuint32 nChnID, const NETNS::DEV_AUDIO_ENCODE encode) 
{
	if (!CheckUserPass(pUser))
	{
		return NETNS::DEV_STATUS_PERMISSION_DENIED;
	}
	
	if (NETNS::AUDIO_ENCODE_G711A == encode)
	{
		return NETNS::DEV_STATUS_SUCCESS;
	}
	
	return NETNS::DEV_STATUS_PARAM_ERR;
}

NETNS::DEV_RET CTvtDevInterface::GetVideoStream( const char* pUser, tuint32 nChnID,  tuint32 nSteamIdx, NETNS::DEV_SREAM_CFG &cfg) 
{
	if (!CheckUserPass(pUser))
	{
		return NETNS::DEV_STATUS_PERMISSION_DENIED;
	}

	unsigned char *pData = NULL;
	unsigned long dataLen = 0;
	CMyList <NCFG_ITEM_ID> itemList;
	if (nSteamIdx == 0)
	{
		itemList.AddTail(NCFG_ITEM_ENCODE_MAJOR_EX);
	}
	else if(nSteamIdx == 1)
	{
		itemList.AddTail(NCFG_ITEM_ENCODE_MINOR_EX);
	}
	else
	{
		return NETNS::DEV_STATUS_PARAM_ERR;
	}
	if (!CConfigEx::Instance()->GetConfigEx(itemList, &pData, dataLen))
	{
		return NETNS::DEV_STATUS_SYSTEM_BUSY;
	}
	if (dataLen < sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD) + (nChnID+1) * sizeof(ENCODE_INFO_EX))
	{
		delete []pData;
		return NETNS::DEV_STATUS_SYSTEM_BUSY;
	}
	ENCODE_INFO_EX *pEncodeInfo = (ENCODE_INFO_EX *)(pData + sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD) + nChnID * sizeof(ENCODE_INFO_EX));

	cfg.encode = NETNS::VIDEO_ENCODING_H264;
	cfg.nBitRate = pEncodeInfo->hBitStream;
	cfg.nFrmRate = pEncodeInfo->rate;
	cfg.nIFrameInterval = 100;
	cfg.profile = NETNS::H264_PROFILE_HIGH;//TODO
	if (pEncodeInfo->resolution <= DVRVIDEO_SIZE_QCIF)
	{
		cfg.quality = NETNS::DEV_QUALITY_LOWEST;
	}
	else if(pEncodeInfo->resolution <= DVRVIDEO_SIZE_HD1 || pEncodeInfo->resolution == DVRVIDEO_SIZE_QVGA)
	{
		cfg.quality = NETNS::DEV_QUALITY_LOWER;
	}
	else if(pEncodeInfo->resolution <= DVRVIDEO_SIZE_D1 || pEncodeInfo->resolution == DVRVIDEO_SIZE_VGA)
	{
		cfg.quality = NETNS::DEV_QUALITY_MEDIUM;
	}
	else if(pEncodeInfo->resolution <= DVRVIDEO_SIZE_960P || pEncodeInfo->resolution == DVRVIDEO_SIZE_HD1080)
	{
		cfg.quality = NETNS::DEV_QUALITY_HIGHER;
	}
	else
	{
		cfg.quality = NETNS::DEV_QUALITY_HIGHEST;
	}

	unsigned int size = 0;
	GetResolution(pEncodeInfo->resolution, &(cfg.res), 1, size, g_video_format == VIDEO_FORMAT_PAL);

	delete []pData;

	return NETNS::DEV_STATUS_SUCCESS;
}

NETNS::DEV_RET CTvtDevInterface::SetVideoStream( const char* pUser, tuint32 nChnID,  tuint32 nSteamIdx, const NETNS::DEV_SREAM_CFG& cfg) 
{
	if (!CheckUserPass(pUser))
	{
		return NETNS::DEV_STATUS_PERMISSION_DENIED;
	}

	unsigned char *pData = NULL;
	unsigned long dataLen = 0;
	CMyList <NCFG_ITEM_ID> itemList;
	if (nSteamIdx == 0)
	{
		itemList.AddTail(NCFG_ITEM_ENCODE_MAJOR);
	}
	else if(nSteamIdx == 1)
	{
		itemList.AddTail(NCFG_ITEM_ENCODE_MINOR);
	}
	else
	{
		return NETNS::DEV_STATUS_PARAM_ERR;
	}
	if (cfg.encode != NETNS::VIDEO_ENCODING_H264)
	{
		return NETNS::DEV_STATUS_PARAM_ERR;
	}

	if (!CConfigEx::Instance()->GetConfigEx(itemList, &pData, dataLen))
	{
		return NETNS::DEV_STATUS_SYSTEM_BUSY;
	}
	if (dataLen < sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD) + (nChnID+1) * sizeof(ENCODE_INFO))
	{
		delete []pData;
		return NETNS::DEV_STATUS_PARAM_ERR;
	}
	ENCODE_INFO *pEncodeInfo = (ENCODE_INFO *)(pData + sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD) + nChnID * sizeof(ENCODE_INFO));

	pEncodeInfo->encodeType = cfg.encode;
	pEncodeInfo->hBitStream = cfg.nBitRate;
	pEncodeInfo->rate = cfg.nFrmRate;
	pEncodeInfo->quality = 0;//TODO
	if (!GetResolutionFromWH(cfg.res.nWidth, cfg.res.nHeight, pEncodeInfo->resolution))
	{
		delete []pData;
		return NETNS::DEV_STATUS_PARAM_ERR;
	}

	if (!SaveConfig(pData, dataLen))
	{
		delete []pData;
		return NETNS::DEV_STATUS_SYSTEM_BUSY;
	}
	delete []pData;

	return NETNS::DEV_STATUS_SUCCESS;
}

NETNS::DEV_RET CTvtDevInterface::RequestKeyFrame( const char* pUser, tuint32 nChnID ) 
{
	if (!CheckUserPass(pUser))
	{
		return NETNS::DEV_STATUS_PERMISSION_DENIED;
	}

	CLocalDevice::Instance()->KeyFrame(nChnID);

	return NETNS::DEV_STATUS_SUCCESS;
}

bool CTvtDevInterface::RtspNeedAuthorization()
{
	return true;
}

// OSD
NETNS::DEV_RET CTvtDevInterface::GetOsdTimestamp( const char* pUser, tuint32 nChnID, NETNS::DEV_OSD_TIMESTAMP& cfg) 
{
	if (!CheckUserPass(pUser))
	{
		return NETNS::DEV_STATUS_PERMISSION_DENIED;
	}

	//不支持时间格式
	unsigned char *pData = NULL;
	unsigned long dataLen = 0;
	CMyList <NCFG_ITEM_ID> itemList;
	itemList.AddTail(NCFG_ITEM_TIME_STAMP_INFO);
	if (!CConfigEx::Instance()->GetConfigEx(itemList, &pData, dataLen))
	{
		return NETNS::DEV_STATUS_SYSTEM_BUSY;
	}
	if (dataLen < sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD) + (nChnID+1) * sizeof(OSD_INFO))
	{
		delete []pData;
		return NETNS::DEV_STATUS_PARAM_ERR;
	}
	OSD_INFO *osd = (OSD_INFO*)(pData + sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD) + nChnID * sizeof(OSD_INFO));

	cfg.bSwitch = osd->bEnable;
	cfg.x = osd->x * 6.4;
	cfg.y = osd->y * 4.8;

	delete []pData;

	return NETNS::DEV_STATUS_SUCCESS;
}

NETNS::DEV_RET CTvtDevInterface::GetOsdChnName( const char* pUser,  tuint32 nChnID,  NETNS::DEV_OSD_DEVICE_NAME& cfg) 
{
	if (!CheckUserPass(pUser))
	{
		return NETNS::DEV_STATUS_PERMISSION_DENIED;
	}

	unsigned char *pData = NULL;
	unsigned long dataLen = 0;
	CMyList <NCFG_ITEM_ID> itemList;
	itemList.AddTail(NCFG_ITEM_CAM_NAME_STAMP_INFO);
	if (!CConfigEx::Instance()->GetConfigEx(itemList, &pData, dataLen))
	{
		return NETNS::DEV_STATUS_SYSTEM_BUSY;
	}
	if (dataLen < sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD) + (nChnID+1) * sizeof(OSD_INFO))
	{
		delete []pData;
		return NETNS::DEV_STATUS_PARAM_ERR;
	}
	OSD_INFO *osd = (OSD_INFO*)(pData + sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD) + nChnID * sizeof(OSD_INFO));
	cfg.bSwitch = osd->bEnable;
	cfg.x = osd->x * 6.4;
	cfg.y = osd->y * 4.8;

	delete []pData;

//
	itemList.RemoveAll();
	itemList.AddTail(NCFG_ITEM_CAMERA_NAME);
	if (!CConfigEx::Instance()->GetConfigEx(itemList, &pData, dataLen))
	{
		return NETNS::DEV_STATUS_SYSTEM_BUSY;
	}
	if (dataLen < sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD) + (nChnID+1) * sizeof(NCFG_INFO_SHORT_NAME))
	{
		delete []pData;
		return NETNS::DEV_STATUS_PARAM_ERR;
	}
	NCFG_INFO_SHORT_NAME *name = (NCFG_INFO_SHORT_NAME *)(pData + sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD) + nChnID * sizeof(NCFG_INFO_SHORT_NAME));
	strncpy(cfg.szDevName, name->name, sizeof(NCFG_INFO_SHORT_NAME));

	delete []pData;
	return NETNS::DEV_STATUS_SUCCESS;
}

NETNS::DEV_RET CTvtDevInterface::SetOsdTimestamp( const char* pUser, tuint32 nChnID, const NETNS::DEV_OSD_TIMESTAMP cfg) 
{
	if (!CheckUserPass(pUser))
	{
		return NETNS::DEV_STATUS_PERMISSION_DENIED;
	}

	unsigned char *pData = NULL;
	unsigned long dataLen = 0;
	CMyList <NCFG_ITEM_ID> itemList;
	itemList.AddTail(NCFG_ITEM_TIME_STAMP_INFO);
	if (!CConfigEx::Instance()->GetConfigEx(itemList, &pData, dataLen))
	{
		return NETNS::DEV_STATUS_SYSTEM_BUSY;
	}
	if (dataLen < sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD) + (nChnID+1) * sizeof(OSD_INFO))
	{
		delete []pData;
		return NETNS::DEV_STATUS_PARAM_ERR;
	}
	OSD_INFO *osd = (OSD_INFO*)(pData + sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD) + nChnID * sizeof(OSD_INFO));

	osd->bEnable = cfg.bSwitch;
	osd->x = cfg.x / 6.4;
	osd->y = cfg.y / 4.8;

	if (!SaveConfig(pData, dataLen))
	{
		delete []pData;
		return NETNS::DEV_STATUS_SYSTEM_BUSY;
	}
	delete []pData;

	return NETNS::DEV_STATUS_SUCCESS;
}

NETNS::DEV_RET CTvtDevInterface::SetOsdChnName(const char* pUser,  tuint32 nChnID,  const NETNS::DEV_OSD_DEVICE_NAME cfg) 
{
	if (!CheckUserPass(pUser))
	{
		return NETNS::DEV_STATUS_PERMISSION_DENIED;
	}

	unsigned char *pData = NULL;
	unsigned long dataLen = 0;
	CMyList <NCFG_ITEM_ID> itemList;
	itemList.AddTail(NCFG_ITEM_CAM_NAME_STAMP_INFO);
	if (!CConfigEx::Instance()->GetConfigEx(itemList, &pData, dataLen))
	{
		return NETNS::DEV_STATUS_SYSTEM_BUSY;
	}
	if (dataLen < sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD) + (nChnID+1) * sizeof(OSD_INFO))
	{
		delete []pData;
		return NETNS::DEV_STATUS_PARAM_ERR;
	}
	OSD_INFO *osd = (OSD_INFO*)(pData + sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD) + nChnID * sizeof(OSD_INFO));
	osd->bEnable = cfg.bSwitch;
	osd->x = cfg.x / 6.4;
	osd->y = cfg.y / 4.8;

	if (!SaveConfig(pData, dataLen))
	{
		delete []pData;
		return NETNS::DEV_STATUS_SYSTEM_BUSY;
	}
	delete []pData;
//
	itemList.RemoveAll();
	itemList.AddTail(NCFG_ITEM_CAMERA_NAME);
	if (!CConfigEx::Instance()->GetConfigEx(itemList, &pData, dataLen))
	{
		return NETNS::DEV_STATUS_SYSTEM_BUSY;
	}
	if (dataLen < sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD) + (nChnID+1) * sizeof(NCFG_INFO_SHORT_NAME))
	{
		delete []pData;
		return NETNS::DEV_STATUS_PARAM_ERR;
	}
	NCFG_INFO_SHORT_NAME *name = (NCFG_INFO_SHORT_NAME *)(pData + sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD) + nChnID * sizeof(NCFG_INFO_SHORT_NAME));
	strncpy(name->name, cfg.szDevName, sizeof(NCFG_INFO_SHORT_NAME));
	
	if (!SaveConfig(pData, dataLen))
	{
		delete []pData;
		return NETNS::DEV_STATUS_SYSTEM_BUSY;
	}
	delete []pData;

	return NETNS::DEV_STATUS_SUCCESS;
}

// privacy mask
NETNS::DEV_RET CTvtDevInterface::GetPrivacyMaskCfg( const char* pUser, tuint32 nChnID, NETNS::DEV_OSD_PRIVACY_MASK* pCfg, tuint32 nSize, tuint32& nNeedSize) 
{
	if (!CheckUserPass(pUser))
	{
		return NETNS::DEV_STATUS_PERMISSION_DENIED;
	}

	unsigned char *pData = NULL;
	unsigned long dataLen = 0;
	CMyList <NCFG_ITEM_ID> itemList;
	itemList.AddTail(NCFG_ITEM_CAM_COVER_INFO);
	if (nSize < 1)
	{
		return NETNS::DEV_STATUS_PARAM_ERR;
	}
	if (!CConfigEx::Instance()->GetConfigEx(itemList, &pData, dataLen))
	{
		return NETNS::DEV_STATUS_SYSTEM_BUSY;
	}
	if (dataLen < sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD) + (nChnID+1) * sizeof(CAMERA_COVER_INFO))
	{
		delete []pData;
		return NETNS::DEV_STATUS_PARAM_ERR;
	}
	CAMERA_COVER_INFO* mask = (CAMERA_COVER_INFO*)(pData + sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD) + nChnID * sizeof(CAMERA_COVER_INFO));
	nNeedSize = 0;
	while(nNeedSize < nSize && nNeedSize < MAX_COVER_AREA_NUM)
	{
		pCfg[nNeedSize].bShow = mask->area->bCover;
		//pCfg[0].color = mask->area->color;//NCFG_OSD_MASK_CONFIG.color == DEV_OSD_PRIVACY_MASK.color
		pCfg[nNeedSize].color = NETNS::DEV_COLOR_GRAY;
		pCfg[nNeedSize].rect.x = mask->area->x * 6.4;
		pCfg[nNeedSize].rect.y = mask->area->y * 4.8;
		pCfg[nNeedSize].rect.width = mask->area->cx * 6.4;
		pCfg[nNeedSize].rect.height = mask->area->cy * 4.8;
		nNeedSize++;
	}

	delete []pData;

	return NETNS::DEV_STATUS_SUCCESS;
}

NETNS::DEV_RET CTvtDevInterface::SetPrivacyMaskCfg( const char* pUser, tuint32 nChnID, NETNS::DEV_OSD_PRIVACY_MASK* pCfg, tuint32 nSize) 
{
	if (!CheckUserPass(pUser))
	{
		return NETNS::DEV_STATUS_PERMISSION_DENIED;
	}

	unsigned char *pData = NULL;
	unsigned long dataLen = 0;
	CMyList <NCFG_ITEM_ID> itemList;
	itemList.AddTail(NCFG_ITEM_CAM_COVER_INFO);
	if (nSize < 1)
	{
		return NETNS::DEV_STATUS_PARAM_ERR;
	}
	if (!CConfigEx::Instance()->GetConfigEx(itemList, &pData, dataLen))
	{
		return NETNS::DEV_STATUS_SYSTEM_BUSY;
	}
	if (dataLen < sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD) + (nChnID+1) * sizeof(CAMERA_COVER_INFO))
	{
		delete []pData;
		return NETNS::DEV_STATUS_PARAM_ERR;
	}
	CAMERA_COVER_INFO* mask = (CAMERA_COVER_INFO*)(pData + sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD) + nChnID * sizeof(CAMERA_COVER_INFO));
	mask->area->bCover = pCfg[0].bShow;
	mask->area->color = pCfg[0].color;
	mask->area->x = pCfg[0].rect.x / 6.4;
	mask->area->y = pCfg[0].rect.y / 4.8;
	mask->area->cx = pCfg[0].rect.width / 6.4;
	mask->area->cy = pCfg[0].rect.height / 4.8;

	if (!SaveConfig(pData, dataLen))
	{
		delete []pData;
		return NETNS::DEV_STATUS_SYSTEM_BUSY;
	}
	delete []pData;

	return NETNS::DEV_STATUS_SUCCESS;
}

/*
* protocol
*/
NETNS::DEV_RET CTvtDevInterface::PtzGetCaps( const char* pUser, tuint32 nChnID, NETNS::DEV_PTZ_CAPS& cfg) 
{
	if (!CheckUserPass(pUser))
	{
		return NETNS::DEV_STATUS_PERMISSION_DENIED;
	}

	cfg.ctrlSpeed.nMax = 8;
	cfg.ctrlSpeed.nMin = 1;
	cfg.nPresetMaxCnt = DD_MAX_PRESET_NUM;

	return NETNS::DEV_STATUS_SUCCESS;
}

NETNS::DEV_RET CTvtDevInterface::PtzGetCaps( const char* pUser, tuint32 nChnID, NETNS::DEV_DOM_PTZ_CAPS& cfg) 
{
	if (!CheckUserPass(pUser))
	{
		return NETNS::DEV_STATUS_PERMISSION_DENIED;
	}

	cfg.cruisePresetMaxCnt = DD_MAX_PRESET_NUM;
	cfg.cruisePresetSpeed.nMax = 8;
	cfg.cruisePresetSpeed.nMin = 1;
	cfg.nCruiseMaxCnt = DD_MAX_CRUISE_NUM;
	cfg.cruisePresetMaxHoldTime = 0xffffffff;

	return NETNS::DEV_STATUS_SUCCESS;
}

NETNS::DEV_RET CTvtDevInterface::PtzGetProtocolCfg( const char* pUser, tuint32 nChnID, NETNS::DEV_PTZ_PROTO& cfg) 
{
	if (!CheckUserPass(pUser))
	{
		return NETNS::DEV_STATUS_PERMISSION_DENIED;
	}

	unsigned char *pData = NULL;
	unsigned long dataLen = 0;
	CMyList <NCFG_ITEM_ID> itemList;
	itemList.AddTail(NCFG_ITEM_PTZ_SERIAL);
	if (!CConfigEx::Instance()->GetConfigEx(itemList, &pData, dataLen))
	{
		return NETNS::DEV_STATUS_SYSTEM_BUSY;
	}
	if (dataLen < sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD) + (nChnID+1) * sizeof(PTZ_SERIAL_INFO))
	{
		delete []pData;
		return NETNS::DEV_STATUS_PARAM_ERR;
	}
	PTZ_SERIAL_INFO *protocol = (PTZ_SERIAL_INFO*)(pData + sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD) + nChnID * sizeof(PTZ_SERIAL_INFO));
	cfg.addr = protocol->devAddr;
	cfg.baudrate = protocol->baudRate;
	cfg.type = (NETNS::DEV_PTZ_PROTO_TYPE)(protocol->protocol - 1);//PROTOCOL_TYPE -> NETNS::DEV_PTZ_PROTO_TYPE

	delete []pData;

	return NETNS::DEV_STATUS_SUCCESS;
}

NETNS::DEV_RET CTvtDevInterface::PtzSetProtocolCfg( const char* pUser, tuint32 nChnID, const NETNS::DEV_PTZ_PROTO& cfg) 
{
	if (!CheckUserPass(pUser))
	{
		return NETNS::DEV_STATUS_PERMISSION_DENIED;
	}

	unsigned char *pData = NULL;
	unsigned long dataLen = 0;
	CMyList <NCFG_ITEM_ID> itemList;
	itemList.AddTail(NCFG_ITEM_PTZ_SERIAL);
	if (!CConfigEx::Instance()->GetConfigEx(itemList, &pData, dataLen))
	{
		return NETNS::DEV_STATUS_SYSTEM_BUSY;
	}
	if (dataLen < sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD) + (nChnID+1) * sizeof(PTZ_SERIAL_INFO))
	{
		delete []pData;
		return NETNS::DEV_STATUS_PARAM_ERR;
	}
	PTZ_SERIAL_INFO *protocol = (PTZ_SERIAL_INFO*)(pData + sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD) + nChnID * sizeof(PTZ_SERIAL_INFO));
	protocol->devAddr = cfg.addr;
	protocol->baudRate = cfg.baudrate;
	protocol->protocol = cfg.type + 1;//PROTOCOL_TYPE -> NETNS::DEV_PTZ_PROTO_TYPE
	
	if (!SaveConfig(pData, dataLen))
	{
		delete []pData;
		return NETNS::DEV_STATUS_SYSTEM_BUSY;
	}
	delete []pData;

	return NETNS::DEV_STATUS_SUCCESS;
}

// ptz ctrol
NETNS::DEV_RET CTvtDevInterface::PtzCtrl( const char* pUser, tuint32 nChnID, NETNS::DEV_PTZ_CTRL_TYPE action, tuint32 nSpeed) 
{
	if (!CheckUserPass(pUser))
	{
		return NETNS::DEV_STATUS_PERMISSION_DENIED;
	}

	NETNS::DEV_RET ret = NETNS::DEV_STATUS_PARAM_ERR;
	if (nSpeed > 8 || nSpeed < 1)
	{
		nSpeed = 8;
	}
	int cmd = 0;
	if (!PtzCmdConvert(action, cmd))
	{
		return ret;
	}

	//不知道是用CPtzMan还是使用MsgMan
	//CPtzMan::Instance()->Action(nChnID, cmd, 0, nSpeed);

	PTZ_CTRL ptzCtrl;
	ptzCtrl.chnn = nChnID;
	ptzCtrl.cmdType = cmd;
	ptzCtrl.speed = nSpeed;

	MESSAGE_DATA msgData;
	memset(&msgData, 0, sizeof(MESSAGE_DATA));

	//这里直接用了LOCAL_CLIENT_ID 因为界面个远程操作同时控制云台也没用
	CMessageMan::Instance()->SendMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_PTZ_CTRL, (unsigned char *)&ptzCtrl, sizeof(PTZ_CTRL), msgData);
	PACKCMD *pPackCmd = reinterpret_cast<PACKCMD*>(msgData.pData);
//  	if (pPackCmd->cmdType == CMD_REPLY_PTZ_SET_SUCC)
//  	{
//  		ret = NETNS::DEV_STATUS_SUCCESS;
//  	}
//  	else
//  	{
//  		ret = NETNS::DEV_STATUS_SYSTEM_BUSY;
//  	}
	CMessageMan::Instance()->ReleaseMsgBuff(msgData);

	return NETNS::DEV_STATUS_SUCCESS;
}

// preset
NETNS::DEV_RET CTvtDevInterface::PtzGotoPreset( const char* pUser,  tuint32 nChnID, tuint32 nPresetID) 
{
	if (!CheckUserPass(pUser))
	{
		return NETNS::DEV_STATUS_PERMISSION_DENIED;
	}

	//CPtzMan::Instance()->Action(nChnID, PTZ_CMD_PRESET_GO, nPresetID);
	NETNS::DEV_RET ret = NETNS::DEV_STATUS_SUCCESS;
	PTZ_CTRL ptzCtrl;
	ptzCtrl.chnn = nChnID;
	ptzCtrl.cmdType = PTZ_CMD_PRESET_GO;

	MESSAGE_DATA msgData;
	memset(&msgData, 0, sizeof(MESSAGE_DATA));
	CMessageMan::Instance()->SendMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_PTZ_CTRL, (unsigned char *)&ptzCtrl, sizeof(PTZ_CTRL), msgData);
	PACKCMD *pPackCmd = reinterpret_cast<PACKCMD*>(msgData.pData);
// 	if (pPackCmd->cmdType == CMD_REPLY_PTZ_SEND_CMD_SUC)
// 	{
// 		ret = NETNS::DEV_STATUS_SUCCESS;
// 	}
// 	else
// 	{
// 		ret = NETNS::DEV_STATUS_SYSTEM_BUSY;
// 	}
	CMessageMan::Instance()->ReleaseMsgBuff(msgData);

	return ret;
}

NETNS::DEV_RET CTvtDevInterface::PtzGetPresets( const char* pUser,  tuint32 nChnID, NETNS::DEV_PTZ_PRESET* pPresets, tuint32 nNum, tuint32& nNeedSize) 
{
	if (!CheckUserPass(pUser))
	{
		return NETNS::DEV_STATUS_PERMISSION_DENIED;
	}

//MsgMan的方式获取 CMD_REQUEST_PTZ_PRESET_INFO  根本就没实现

	NETNS::DEV_RET ret = NETNS::DEV_STATUS_SUCCESS;

	//获取配置NCFG_ITEM_PTZ_INFO_PRESET_INFO_CHBASE的方式

	if (nChnID > NCFG_ITEM_PTZ_INFO_PRESET_INFO_CHEND - NCFG_ITEM_PTZ_INFO_PRESET_INFO_CHBASE)
	{
		return NETNS::DEV_STATUS_PARAM_ERR;
	}
	unsigned char *pData = NULL;
	unsigned long dataLen = 0;
	CMyList <NCFG_ITEM_ID> itemList;
	itemList.AddTail((NCFG_ITEM_ID)(NCFG_ITEM_PTZ_INFO_PRESET_INFO_CHBASE + nChnID));
	if (!CConfigEx::Instance()->GetConfigEx(itemList, &pData, dataLen))
	{
		return NETNS::DEV_STATUS_SYSTEM_BUSY;
	}
	if (dataLen < sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD) + sizeof(PTZ_PRESET))
	{
		delete []pData;
		return NETNS::DEV_STATUS_PARAM_ERR;
	}

	nNeedSize = 0;
	dataLen -= sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD);
	PTZ_PRESET* preset = (PTZ_PRESET*)(pData + sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD));
	while(dataLen > 0 && nNeedSize < nNum)
	{
		strncpy(pPresets[nNeedSize].name, preset->name, sizeof(pPresets->name));
		pPresets[nNeedSize].preseIndex = preset->index;
		dataLen -= sizeof(PTZ_PRESET);
		preset++;
		nNeedSize++;
	}

	delete []pData;

	return NETNS::DEV_STATUS_SUCCESS;
}

tuint32 CTvtDevInterface::PtzGetPresetsMaxNum( tuint32 nChnID ) 
{
	return MAX_PRESET_NUM;
}

NETNS::DEV_RET CTvtDevInterface::PtzAddPreset( const char* pUser,tuint32 nChnID, const char* pName) 
{
	if (!CheckUserPass(pUser))
	{
		return NETNS::DEV_STATUS_PERMISSION_DENIED;
	}

	return NETNS::DEV_STATUS_SUCCESS;
}

NETNS::DEV_RET CTvtDevInterface::PtzModifyPresetName( const char* pUser, tuint32 nChnID, tuint32 nPresetID, const char* pName) 
{
	if (!CheckUserPass(pUser))
	{
		return NETNS::DEV_STATUS_PERMISSION_DENIED;
	}

	if (nChnID > NCFG_ITEM_PTZ_INFO_PRESET_INFO_CHEND - NCFG_ITEM_PTZ_INFO_PRESET_INFO_CHBASE || nPresetID >= MAX_PRESET_NUM)
	{
		return NETNS::DEV_STATUS_PARAM_ERR;
	}
	unsigned char *pData = NULL;
	unsigned long dataLen = 0;
	CMyList <NCFG_ITEM_ID> itemList;
	itemList.AddTail((NCFG_ITEM_ID)(NCFG_ITEM_PTZ_INFO_PRESET_INFO_CHBASE + nChnID));
	if (!CConfigEx::Instance()->GetConfigEx(itemList, &pData, dataLen))
	{
		return NETNS::DEV_STATUS_SYSTEM_BUSY;
	}
	if (dataLen < sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD) + (nPresetID+1)*sizeof(PTZ_PRESET))
	{
		delete []pData;
		return NETNS::DEV_STATUS_PARAM_ERR;
	}

	unsigned long oriLen = dataLen;
	dataLen -= sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD);
	PTZ_PRESET* preset = (PTZ_PRESET*)(pData + sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD));
	while(dataLen > 0)
	{
		if (preset->index == nPresetID)
		{
			strncpy(preset->name, pName, sizeof(preset->name));
			break;
		}
		dataLen -= sizeof(PTZ_PRESET);
		preset++;
	}

	if (!SaveConfig(pData, oriLen))
	{
		delete []pData;
		return NETNS::DEV_STATUS_SYSTEM_BUSY;
	}
	delete []pData;

	return NETNS::DEV_STATUS_SUCCESS;
}

NETNS::DEV_RET CTvtDevInterface::PtzDeletePreset( const char* pUser, tuint32 nChnID, tuint32 nPresetID) 
{
	if (!CheckUserPass(pUser))
	{
		return NETNS::DEV_STATUS_PERMISSION_DENIED;
	}

	NETNS::DEV_RET ret = NETNS::DEV_STATUS_SUCCESS;
	PTZ_CTRL ptzCtrl;
	ptzCtrl.chnn = nChnID;
	ptzCtrl.cmdValue = nPresetID;
	ptzCtrl.cmdType = PTZ_CMD_PRESET_DEL;

	MESSAGE_DATA msgData;
	memset(&msgData, 0, sizeof(MESSAGE_DATA));
	CMessageMan::Instance()->SendMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_PTZ_CTRL, (unsigned char *)&ptzCtrl, sizeof(PTZ_CTRL), msgData);
	PACKCMD *pPackCmd = reinterpret_cast<PACKCMD*>(msgData.pData);
// 	if (pPackCmd->cmdType == CMD_REPLY_PTZ_SEND_CMD_SUC)
// 	{
// 		ret = NETNS::DEV_STATUS_SUCCESS;
// 	}
// 	else
// 	{
// 		ret = NETNS::DEV_STATUS_SYSTEM_BUSY;
// 	}
	CMessageMan::Instance()->ReleaseMsgBuff(msgData);

	return ret;
}

NETNS::DEV_RET CTvtDevInterface::PtzModifyPresetPosition( const char* pUser, tuint32 nChnID, tuint32 nPresetID) 
{
	if (!CheckUserPass(pUser))
	{
		return NETNS::DEV_STATUS_PERMISSION_DENIED;
	}

	NETNS::DEV_RET ret = NETNS::DEV_STATUS_SUCCESS;
	PTZ_CTRL ptzCtrl;
	ptzCtrl.chnn = nChnID;
	ptzCtrl.cmdValue = nPresetID;
	ptzCtrl.cmdType = PTZ_CMD_PRESET_SET;

	MESSAGE_DATA msgData;
	memset(&msgData, 0, sizeof(MESSAGE_DATA));
	CMessageMan::Instance()->SendMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_PTZ_CTRL, (unsigned char *)&ptzCtrl, sizeof(PTZ_CTRL), msgData);
	PACKCMD *pPackCmd = reinterpret_cast<PACKCMD*>(msgData.pData);
// 	if (pPackCmd->cmdType == CMD_REPLY_PTZ_SEND_CMD_SUC)//搞不清楚应该是那种返回值
// 	{
// 		ret = NETNS::DEV_STATUS_SUCCESS;
// 	}
// 	else
// 	{
// 		ret = NETNS::DEV_STATUS_SYSTEM_BUSY;
// 	}
	CMessageMan::Instance()->ReleaseMsgBuff(msgData);

	return ret;
}

// cruise
NETNS::DEV_RET CTvtDevInterface::PtzGetCruises( const char* pUser, tuint32 nChnID, NETNS::DEV_PTZ_CRUISE* pCruises, tuint32 nSize, tuint32& nNeedSize) 
{
	if (!CheckUserPass(pUser))
	{
		return NETNS::DEV_STATUS_PERMISSION_DENIED;
	}

	NETNS::DEV_RET ret = NETNS::DEV_STATUS_SUCCESS;

	if (nChnID > NCFG_ITEM_PTZ_INFO_CRUISE_INFO_CHEND - NCFG_ITEM_PTZ_INFO_CRUISE_INFO_CHBASE)
	{
		return NETNS::DEV_STATUS_PARAM_ERR;
	}
	unsigned char *pData = NULL;
	unsigned long dataLen = 0;
	CMyList <NCFG_ITEM_ID> itemList;
	itemList.AddTail((NCFG_ITEM_ID)(NCFG_ITEM_PTZ_INFO_CRUISE_INFO_CHBASE + nChnID));
	if (!CConfigEx::Instance()->GetConfigEx(itemList, &pData, dataLen))
	{
		return NETNS::DEV_STATUS_SYSTEM_BUSY;
	}
	if (dataLen < sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD) + sizeof(PTZ_CURISE))
	{
		if (dataLen == sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD))
		{
			nNeedSize = 0;
			return NETNS::DEV_STATUS_SUCCESS;
		}
		delete []pData;
		return NETNS::DEV_STATUS_PARAM_ERR;
	}

	nNeedSize = 0;
	dataLen -= sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD);
	PTZ_CURISE* cuise = (PTZ_CURISE*)(pData + sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD));
	while(dataLen > 0 && nNeedSize < nSize)
	{
		strncpy(pCruises[nNeedSize].szName, cuise->name, sizeof(pCruises->szName));
		pCruises[nNeedSize].ucCuiseIndex = cuise->index;
		dataLen -= sizeof(PTZ_PRESET);
		cuise++;
		nNeedSize++;
	}
	delete []pData;

	return NETNS::DEV_STATUS_SUCCESS;
}

NETNS::DEV_RET CTvtDevInterface::PtzGetCruise( const char* pUser, tuint32 nChnID, tuint32 nCruiseID, NETNS::DEV_PTZ_PRESET* pPreset, tuint32 nSize, tuint32& nNeedSize) 
{
	if (!CheckUserPass(pUser))
	{
		return NETNS::DEV_STATUS_PERMISSION_DENIED;
	}

	//TODO获取不到巡航线中的预置点信息

 	NETNS::DEV_RET ret = NETNS::DEV_STATUS_SUCCESS;
 	do 
 	{
 		if (nChnID > NCFG_ITEM_PTZ_INFO_CRUISE_INFO_CHEND - NCFG_ITEM_PTZ_INFO_CRUISE_INFO_CHBASE)
 		{
 			ret = NETNS::DEV_STATUS_PARAM_ERR;
 			break;
 		}
 		unsigned char *pData = NULL;
 		unsigned long dataLen = 0;
 		CMyList <NCFG_ITEM_ID> itemList;
 		itemList.AddTail((NCFG_ITEM_ID)(NCFG_ITEM_PTZ_INFO_CRUISE_INFO_CHBASE + nChnID));
 		if (!CConfigEx::Instance()->GetConfigEx(itemList, &pData, dataLen))
 		{
 			return NETNS::DEV_STATUS_SYSTEM_BUSY;
 		}
 		if (dataLen < sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD) + (nCruiseID+1)*sizeof(PTZ_CURISE))
 		{
			if (dataLen == sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD))
			{
				nNeedSize = 0;
				return NETNS::DEV_STATUS_SUCCESS;
			}
 			delete []pData;
 			return NETNS::DEV_STATUS_PARAM_ERR;
 		}
 
 		PTZ_CURISE* cuise = (PTZ_CURISE*)(pData + sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD) + nCruiseID*sizeof(PTZ_CURISE));
 		nNeedSize = 0;
 		while(nNeedSize < nSize)
 		{
 			//pPreset[nNeedSize].preseIndex = cuise->presetNum	
 			nNeedSize++;
 		}
 
 	} while(0);
 
 	return ret;

	
}

NETNS::DEV_RET CTvtDevInterface::PtzAddCruise( const char* pUser, tuint32 nChnID, const char* pCruiseName,  NETNS::DEV_PTZ_PRESET* pPreset, tuint32 nSize) 
{
	if (!CheckUserPass(pUser))
	{
		return NETNS::DEV_STATUS_PERMISSION_DENIED;

	}
	return NETNS::DEV_STATUS_SUCCESS;
}

NETNS::DEV_RET CTvtDevInterface::PtzModifyCruise( const char* pUser, tuint32 nChnID, tuint32 nCruiseID, const char* pCruiseName,  NETNS::DEV_PTZ_PRESET* pPreset, tuint32 nSize ) 
{
	if (!CheckUserPass(pUser))
	{
		return NETNS::DEV_STATUS_PERMISSION_DENIED;
	}

	NETNS::DEV_RET ret = NETNS::DEV_STATUS_SYSTEM_BUSY;
	unsigned char *pCmdBuf = new unsigned char[sizeof(unsigned long)*2 + nSize*sizeof(PTZ_CURISE_POINT)];
	unsigned long *pIndex = (unsigned long*)(pCmdBuf);
	unsigned long *pNum = (unsigned long*)(pCmdBuf + sizeof(unsigned long));
	PTZ_CURISE_POINT *cruise = (PTZ_CURISE_POINT*)(pCmdBuf + sizeof(unsigned long)*2);
	for (int i=0; i<nSize; i++)
	{
		cruise->preset = pPreset[i].preseIndex;
		cruise->dwellSpeed = pPreset[i].speed;
		cruise->dwellTime = pPreset[i].holdtime;
	}

	if (0 == CPtzMan::Instance()->Action(nChnID, PTZ_CMD_CRUISE_CFG, 0, 0, 0, pCmdBuf))
	{
		unsigned char *pData = NULL;
		unsigned long dataLen = 0;
		CMyList <NCFG_ITEM_ID> itemList;
		itemList.AddTail((NCFG_ITEM_ID)(NCFG_ITEM_PTZ_INFO_CRUISE_INFO_CHBASE + nChnID));
		if (!CConfigEx::Instance()->GetConfigEx(itemList, &pData, dataLen))
		{
			return NETNS::DEV_STATUS_SYSTEM_BUSY;
		}
		if (dataLen < sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD) + (nCruiseID+1)*sizeof(PTZ_CURISE))
		{
			delete []pData;
			return NETNS::DEV_STATUS_PARAM_ERR;
		}

		PTZ_CURISE *cruise = (PTZ_CURISE*)(pData + sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD) + nCruiseID*sizeof(PTZ_CURISE));
		strncpy(cruise->name, pCruiseName, sizeof(cruise->name));

		if (!SaveConfig(pData, dataLen))
		{
			delete []pData;
			return NETNS::DEV_STATUS_SYSTEM_BUSY;
		}
		delete []pData;
		ret = NETNS::DEV_STATUS_SUCCESS;
	}

	delete []pCmdBuf;

	return ret;
}

NETNS::DEV_RET CTvtDevInterface::PtzDeleteCruise( const char* pUser, tuint32 nChnID, tuint32 nCruiseID) 
{
	if (!CheckUserPass(pUser))
	{
		return NETNS::DEV_STATUS_PERMISSION_DENIED;
	}

	NETNS::DEV_RET ret = NETNS::DEV_STATUS_SUCCESS;
	PTZ_CTRL ptzCtrl;
	ptzCtrl.chnn = nChnID;
	ptzCtrl.cmdValue = nCruiseID;
	ptzCtrl.cmdType = PTZ_CMD_CRUISE_DEL;

	MESSAGE_DATA msgData;
	memset(&msgData, 0, sizeof(MESSAGE_DATA));
	CMessageMan::Instance()->SendMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_PTZ_CTRL, (unsigned char *)&ptzCtrl, sizeof(PTZ_CTRL), msgData);
	PACKCMD *pPackCmd = reinterpret_cast<PACKCMD*>(msgData.pData);
// 	if (pPackCmd->cmdType == CMD_REPLY_PTZ_CURISE_INFO)
// 	{
// 		ret = NETNS::DEV_STATUS_SUCCESS;
// 	}
// 	else
// 	{
// 		ret = NETNS::DEV_STATUS_SYSTEM_BUSY;
// 	}
	CMessageMan::Instance()->ReleaseMsgBuff(msgData);

	return ret;
}

NETNS::DEV_RET CTvtDevInterface::PtzRunCruise( const char* pUser, tuint32 nChnID, tuint32 nCruiseID) 
{
	if (!CheckUserPass(pUser))
	{
		return NETNS::DEV_STATUS_PERMISSION_DENIED;
	}

	NETNS::DEV_RET ret = NETNS::DEV_STATUS_SUCCESS;
	PTZ_CTRL ptzCtrl;
	ptzCtrl.chnn = nChnID;
	ptzCtrl.cmdValue = nCruiseID;
	ptzCtrl.cmdType = PTZ_CMD_CRUISE_RUN;
	m_RuningCruiseID = nCruiseID;

	MESSAGE_DATA msgData;
	memset(&msgData, 0, sizeof(MESSAGE_DATA));
	CMessageMan::Instance()->SendMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_PTZ_CTRL, (unsigned char *)&ptzCtrl, sizeof(PTZ_CTRL), msgData);
	PACKCMD *pPackCmd = reinterpret_cast<PACKCMD*>(msgData.pData);

	CMessageMan::Instance()->ReleaseMsgBuff(msgData);

	return ret;
}

NETNS::DEV_RET CTvtDevInterface::PtzStopCruise( const char* pUser, tuint32 nChnID) 
{
	if (!CheckUserPass(pUser))
	{
		return NETNS::DEV_STATUS_PERMISSION_DENIED;
	}

	NETNS::DEV_RET ret = NETNS::DEV_STATUS_SUCCESS;
	PTZ_CTRL ptzCtrl;
	ptzCtrl.chnn = nChnID;
	ptzCtrl.cmdValue = m_RuningCruiseID;
	ptzCtrl.cmdType = PTZ_CMD_CRUISE_STOP;

	MESSAGE_DATA msgData;
	memset(&msgData, 0, sizeof(MESSAGE_DATA));
	CMessageMan::Instance()->SendMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_PTZ_CTRL, (unsigned char *)&ptzCtrl, sizeof(PTZ_CTRL), msgData);
	PACKCMD *pPackCmd = reinterpret_cast<PACKCMD*>(msgData.pData);

	CMessageMan::Instance()->ReleaseMsgBuff(msgData);

	return ret;
}

/*
* Motion Detection
*/
NETNS::DEV_RET CTvtDevInterface:: GetMotionCfg( const char* pUser, tuint32 nChnID, NETNS::DEV_MOTION_CFG& cfg) 
{
	if (!CheckUserPass(pUser))
	{
		return NETNS::DEV_STATUS_PERMISSION_DENIED;
	}

	unsigned char *pData = NULL;
	unsigned long dataLen = 0;
	CMyList <NCFG_ITEM_ID> itemList;
	itemList.AddTail(NCFG_ITEM_MOTION_AREA_INFO);
	if (!CConfigEx::Instance()->GetConfigEx(itemList, &pData, dataLen))
	{
		return NETNS::DEV_STATUS_SYSTEM_BUSY;
	}
	if (dataLen < sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD) + (nChnID+1)*sizeof(MOTION_DETECT_INFO))
	{
		delete []pData;
		return NETNS::DEV_STATUS_PARAM_ERR;
	}
	MOTION_DETECT_INFO *motion = (MOTION_DETECT_INFO*)(pData + sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD) + nChnID*sizeof(MOTION_DETECT_INFO));
	cfg.nMotionSen = motion->sensitivity;
	//MOTION_AREA_INFO.pData comment is err
	cfg.nMotionAreaWidth = motion->areaInfo.gridWidth;
	cfg.nMotionAreaHeight = motion->areaInfo.gridHeight;
	for (int i=0, m=0, n=0; i<NETNS::MAX_MOTION_HEIGHT; i++)
	{
		for (int j=0; j<64; j++)
		{
			if (motion->areaInfo.pData[m] & (0x01<<n))
			{
				cfg.ullMotionArea[i] |= 0x01 << j;
			}
			if (++n == 8)
			{
				n=0;
				m++;
			}
		}
	}

	delete []pData;
//
	itemList.RemoveAll();
	itemList.AddTail(NCFG_ITEM_MOTION_HOLD_TIME);
	if (!CConfigEx::Instance()->GetConfigEx(itemList, &pData, dataLen))
	{
		return NETNS::DEV_STATUS_SYSTEM_BUSY;
	}
	if (dataLen < sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD) + (nChnID+1)*sizeof(unsigned long))
	{
		delete []pData;
		return NETNS::DEV_STATUS_PARAM_ERR;
	}
	unsigned long *holdTime = (unsigned long*)(pData + sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD) + nChnID*sizeof(unsigned long));
	cfg.nAlarmHoldTime = *holdTime;
	delete []pData;
//
	itemList.RemoveAll();
	itemList.AddTail(NCFG_ITEM_MOTION_TO_ALARM_OUT);
	if (!CConfigEx::Instance()->GetConfigEx(itemList, &pData, dataLen))
	{
		return NETNS::DEV_STATUS_SYSTEM_BUSY;
	}
	if (dataLen < sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD) + (nChnID+1)*sizeof(ULONGLONG))
	{
		delete []pData;
		return NETNS::DEV_STATUS_PARAM_ERR;
	}
	ULONGLONG *alarmOut = (ULONGLONG*)(pData + sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD) + nChnID*sizeof(ULONGLONG));
	int tmpx = 0;
	for( tmpx = 0; tmpx < sizeof(ULONGLONG)*8; tmpx++ )
	{
		cfg.nAlarmoutCh[tmpx] = ((*alarmOut)>>tmpx) & 0x1;
	}
	delete []pData;
//
	itemList.RemoveAll();
	itemList.AddTail(NCFG_ITEM_MOTION_ENABLE);
	if (!CConfigEx::Instance()->GetConfigEx(itemList, &pData, dataLen))
	{
		return NETNS::DEV_STATUS_SYSTEM_BUSY;
	}
	if (dataLen < sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD) + (nChnID+1)*sizeof(unsigned long))
	{
		delete []pData;
		return NETNS::DEV_STATUS_PARAM_ERR;
	}
	unsigned long *enable = (unsigned long*)(pData + sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD) + nChnID*sizeof(unsigned long));
	cfg.bSwitch = *enable;
	
	if (!SaveConfig(pData, dataLen))
	{
		delete []pData;
		return NETNS::DEV_STATUS_SYSTEM_BUSY;
	}
	delete []pData;

	return NETNS::DEV_STATUS_SUCCESS;
}

NETNS::DEV_RET CTvtDevInterface:: GetMotionSensitivity( const char* pUser, tuint32 nChnID, NETNS::DEV_RANGE& sen) 
{
	if (!CheckUserPass(pUser))
	{
		return NETNS::DEV_STATUS_PERMISSION_DENIED;
	}

	sen.nMin = 1;
	sen.nMax = 8;
	return NETNS::DEV_STATUS_SUCCESS;
}

NETNS::DEV_RET CTvtDevInterface:: SetMotionCfg( const char* pUser, tuint32 nChnID, const NETNS::DEV_MOTION_CFG &cfg) 
{
	if (!CheckUserPass(pUser))
	{
		return NETNS::DEV_STATUS_PERMISSION_DENIED;
	}

	unsigned char *pData = NULL;
	unsigned long dataLen = 0;
	CMyList <NCFG_ITEM_ID> itemList;
	itemList.AddTail(NCFG_ITEM_MOTION_AREA_INFO);
	if (!CConfigEx::Instance()->GetConfigEx(itemList, &pData, dataLen))
	{
		return NETNS::DEV_STATUS_SYSTEM_BUSY;
	}
	if (dataLen < sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD) + (nChnID+1)*sizeof(MOTION_DETECT_INFO))
	{
		delete []pData;
		return NETNS::DEV_STATUS_PARAM_ERR;
	}
	MOTION_DETECT_INFO *motion = (MOTION_DETECT_INFO*)(pData + sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD) + nChnID*sizeof(MOTION_DETECT_INFO));
	motion->sensitivity = cfg.nMotionSen;
	motion->areaInfo.gridWidth = cfg.nMotionAreaWidth;
	motion->areaInfo.gridHeight = cfg.nMotionAreaHeight;
	for (int i=0, m=0, n=0; i<NETNS::MAX_MOTION_HEIGHT; i++)
	{
		for (int j=0; j<64; j++)
		{
			if (cfg.ullMotionArea[i] & (0x01 << j))
			{
				motion->areaInfo.pData[m] |= 0x01 << n;
			}
			if (++n == 8)
			{
				n=0;
				m++;
			}
		}
	}

	if (!SaveConfig(pData, dataLen))
	{
		delete []pData;
		return NETNS::DEV_STATUS_SYSTEM_BUSY;
	}	
	delete []pData;
//
	itemList.RemoveAll();
	itemList.AddTail(NCFG_ITEM_MOTION_HOLD_TIME);
	if (!CConfigEx::Instance()->GetConfigEx(itemList, &pData, dataLen))
	{
		return NETNS::DEV_STATUS_SYSTEM_BUSY;
	}
	if (dataLen < sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD) + (nChnID+1)*sizeof(unsigned long))
	{
		delete []pData;
		return NETNS::DEV_STATUS_PARAM_ERR;
	}
	unsigned long *holdTime = (unsigned long*)(pData + sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD) + nChnID*sizeof(unsigned long));
	*holdTime = cfg.nAlarmHoldTime;
	
	if (!SaveConfig(pData, dataLen))
	{
		delete []pData;
		return NETNS::DEV_STATUS_SYSTEM_BUSY;
	}
	delete []pData;
//
	itemList.RemoveAll();
	itemList.AddTail(NCFG_ITEM_MOTION_TO_ALARM_OUT);
	if (!CConfigEx::Instance()->GetConfigEx(itemList, &pData, dataLen))
	{
		return NETNS::DEV_STATUS_SYSTEM_BUSY;
	}
	if (dataLen < sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD) + (nChnID+1)*sizeof(ULONGLONG))
	{
		delete []pData;
		return NETNS::DEV_STATUS_PARAM_ERR;
	}
	ULONGLONG *alarmOut = (ULONGLONG*)(pData + sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD) + nChnID*sizeof(ULONGLONG));
	int tmpx = 0;
	ULONGLONG ullAlarmOutTmp = 0;
	for( tmpx = 0; tmpx < sizeof(ULONGLONG)*8; tmpx++ )
	{
		if( cfg.nAlarmoutCh[tmpx] )
		{
			ullAlarmOutTmp |= (1<<tmpx);
		}
		//cfg.nAlarmoutCh[tmpx] = ((*alarmOut)>>tmpx) & 0x1;
	}
	*alarmOut = ullAlarmOutTmp; //cfg.nAlarmoutCh;
	
	if (!SaveConfig(pData, dataLen))
	{
		delete []pData;
		return NETNS::DEV_STATUS_SYSTEM_BUSY;
	}
	delete []pData;
//
	itemList.RemoveAll();
	itemList.AddTail(NCFG_ITEM_MOTION_ENABLE);
	if (!CConfigEx::Instance()->GetConfigEx(itemList, &pData, dataLen))
	{
		return NETNS::DEV_STATUS_SYSTEM_BUSY;
	}
	if (dataLen < sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD) + (nChnID+1)*sizeof(unsigned long))
	{
		delete []pData;
		return NETNS::DEV_STATUS_PARAM_ERR;
	}
	unsigned long *enable = (unsigned long*)(pData + sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD) + nChnID*sizeof(unsigned long));
	*enable = cfg.bSwitch;
	
	if (!SaveConfig(pData, dataLen))
	{
		delete []pData;
		return NETNS::DEV_STATUS_SYSTEM_BUSY;
	}
	delete []pData;

	return NETNS::DEV_STATUS_SUCCESS;
}

/*
* Alarm
*/
NETNS::DEV_RET CTvtDevInterface::GetAlarmInCfg( const char* pUser, tuint32 nChnID, NETNS::DEV_SENSOR_CFG &cfg) 
{
	if (!CheckUserPass(pUser))
	{
		return NETNS::DEV_STATUS_PERMISSION_DENIED;
	}

	//TODO  sensor in 的个数 不一定是通道数

	unsigned char *pData = NULL;
	unsigned long dataLen = 0;
	CMyList <NCFG_ITEM_ID> itemList;
	itemList.AddTail(NCFG_ITEM_SENSOR_IN_ENABLE);
	if (!CConfigEx::Instance()->GetConfigEx(itemList, &pData, dataLen))
	{
		return NETNS::DEV_STATUS_SYSTEM_BUSY;
	}
	if (dataLen < sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD) + (nChnID+1)*sizeof(unsigned long))
	{
		delete []pData;
		return NETNS::DEV_STATUS_PARAM_ERR;
	}
	unsigned long *enable = (unsigned long*)(pData + sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD) + nChnID*sizeof(unsigned long));
	cfg.bSwitch = *enable;
	delete []pData;
//
	itemList.RemoveAll();
	itemList.AddTail(NCFG_ITEM_SENSOR_IN_NAME);
	if (!CConfigEx::Instance()->GetConfigEx(itemList, &pData, dataLen))
	{
		return NETNS::DEV_STATUS_SYSTEM_BUSY;
	}
	if (dataLen < sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD) + (nChnID+1)*sizeof(NCFG_INFO_SHORT_NAME))
	{
		delete []pData;
		return NETNS::DEV_STATUS_PARAM_ERR;
	}
	NCFG_INFO_SHORT_NAME *name = (NCFG_INFO_SHORT_NAME*)(pData + sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD) + nChnID*sizeof(NCFG_INFO_SHORT_NAME));
	strncpy(cfg.szSensorName, name->name, sizeof(cfg.szSensorName));
	delete []pData;
//
	itemList.RemoveAll();
	itemList.AddTail(NCFG_ITEM_SENSOR_IN_HOLD_TIME);
	if (!CConfigEx::Instance()->GetConfigEx(itemList, &pData, dataLen))
	{
		return NETNS::DEV_STATUS_SYSTEM_BUSY;
	}
	if (dataLen < sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD) + (nChnID+1)*sizeof(unsigned long))
	{
		delete []pData;
		return NETNS::DEV_STATUS_PARAM_ERR;
	}
	unsigned long *holdTime = (unsigned long*)(pData + sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD) + nChnID*sizeof(unsigned long));
	cfg.dwAlarmHoldTime = *holdTime;
	delete []pData;
//
	itemList.RemoveAll();
	itemList.AddTail(NCFG_ITEM_SENSOR_IN_TO_ALARM_OUT);
	if (!CConfigEx::Instance()->GetConfigEx(itemList, &pData, dataLen))
	{
		return NETNS::DEV_STATUS_SYSTEM_BUSY;
	}
	if (dataLen < sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD) + (nChnID+1)*sizeof(ULONGLONG))
	{
		delete []pData;
		return NETNS::DEV_STATUS_PARAM_ERR;
	}
	ULONGLONG *alarmOut = (ULONGLONG*)(pData + sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD) + nChnID*sizeof(ULONGLONG));
	int tmpx = 0;
	for( tmpx = 0; tmpx < sizeof(ULONGLONG)*8; tmpx++ )
	{
		cfg.nAlarmoutCh[tmpx] = ((*alarmOut)>>tmpx) & 0x1;
	}
	delete []pData;
//
	itemList.RemoveAll();
	itemList.AddTail(NCFG_ITEM_SENSOR_IN_TYPE);
	if (!CConfigEx::Instance()->GetConfigEx(itemList, &pData, dataLen))
	{
		return NETNS::DEV_STATUS_SYSTEM_BUSY;
	}
	if (dataLen < sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD) + (nChnID+1)*sizeof(unsigned long))
	{
		delete []pData;
		return NETNS::DEV_STATUS_PARAM_ERR;
	}
	unsigned long *sensorInType = (unsigned long*)(pData + sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD) + nChnID*sizeof(unsigned long));
	cfg.nVoltage = (NETNS::DEV_ALARMIN_VOLTAGE)(*sensorInType);
	delete []pData;

	return NETNS::DEV_STATUS_SUCCESS;
}

NETNS::DEV_RET CTvtDevInterface::SetAlarmInCfg( const char* pUser, tuint32 nChnID, const NETNS::DEV_SENSOR_CFG &cfg) 
{
	if (!CheckUserPass(pUser))
	{
		return NETNS::DEV_STATUS_PERMISSION_DENIED;
	}

	unsigned char *pData = NULL;
	unsigned long dataLen = 0;
	CMyList <NCFG_ITEM_ID> itemList;
	itemList.AddTail(NCFG_ITEM_SENSOR_IN_ENABLE);
	if (!CConfigEx::Instance()->GetConfigEx(itemList, &pData, dataLen))
	{
		return NETNS::DEV_STATUS_SYSTEM_BUSY;
	}
	if (dataLen < sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD) + (nChnID+1)*sizeof(unsigned long))
	{
		delete []pData;
		return NETNS::DEV_STATUS_PARAM_ERR;
	}
	unsigned long *enable = (unsigned long*)(pData + sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD) + nChnID*sizeof(unsigned long));
	*enable = cfg.bSwitch;
	
	if (!SaveConfig(pData, dataLen))
	{
		delete []pData;
		return NETNS::DEV_STATUS_SYSTEM_BUSY;
	}
	delete []pData;
//
	itemList.RemoveAll();
	itemList.AddTail(NCFG_ITEM_SENSOR_IN_NAME);
	if (!CConfigEx::Instance()->GetConfigEx(itemList, &pData, dataLen))
	{
		return NETNS::DEV_STATUS_SYSTEM_BUSY;
	}
	if (dataLen < sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD) + (nChnID+1)*sizeof(NCFG_INFO_SHORT_NAME))
	{
		delete []pData;
		return NETNS::DEV_STATUS_PARAM_ERR;
	}
	NCFG_INFO_SHORT_NAME *name = (NCFG_INFO_SHORT_NAME*)(pData + sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD) + nChnID*sizeof(NCFG_INFO_SHORT_NAME));
	strncpy(name->name, cfg.szSensorName, sizeof(cfg.szSensorName));
	
	if (!SaveConfig(pData, dataLen))
	{
		delete []pData;
		return NETNS::DEV_STATUS_SYSTEM_BUSY;
	}
	delete []pData;
//
	itemList.RemoveAll();
	itemList.AddTail(NCFG_ITEM_SENSOR_IN_HOLD_TIME);
	if (!CConfigEx::Instance()->GetConfigEx(itemList, &pData, dataLen))
	{
		return NETNS::DEV_STATUS_SYSTEM_BUSY;
	}
	if (dataLen < sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD) + (nChnID+1)*sizeof(unsigned long))
	{
		delete []pData;
		return NETNS::DEV_STATUS_PARAM_ERR;
	}
	unsigned long *holdTime = (unsigned long*)(pData + sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD) + nChnID*sizeof(unsigned long));
	*holdTime = cfg.dwAlarmHoldTime;
	
	if (!SaveConfig(pData, dataLen))
	{
		delete []pData;
		return NETNS::DEV_STATUS_SYSTEM_BUSY;
	}
	delete []pData;
//
	itemList.RemoveAll();
	itemList.AddTail(NCFG_ITEM_SENSOR_IN_TO_ALARM_OUT);
	if (!CConfigEx::Instance()->GetConfigEx(itemList, &pData, dataLen))
	{
		return NETNS::DEV_STATUS_SYSTEM_BUSY;
	}
	if (dataLen < sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD) + (nChnID+1)*sizeof(ULONGLONG))
	{
		delete []pData;
		return NETNS::DEV_STATUS_PARAM_ERR;
	}
	ULONGLONG *alarmOut = (ULONGLONG*)(pData + sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD) + nChnID*sizeof(ULONGLONG));
	int tmpx = 0;
	ULONGLONG ullAlarmOutTmp = 0;
	for( tmpx = 0; tmpx < sizeof(ULONGLONG)*8; tmpx++ )
	{
		if( cfg.nAlarmoutCh[tmpx] )
		{
			ullAlarmOutTmp |= (1<<tmpx);
		}
		//cfg.nAlarmoutCh[tmpx] = ((*alarmOut)>>tmpx) & 0x1;
	}
	*alarmOut = ullAlarmOutTmp; //cfg.nAlarmoutCh;
	
	if (!SaveConfig(pData, dataLen))
	{
		delete []pData;
		return NETNS::DEV_STATUS_SYSTEM_BUSY;
	}
	delete []pData;
//
	itemList.RemoveAll();
	itemList.AddTail(NCFG_ITEM_SENSOR_IN_TYPE);
	if (!CConfigEx::Instance()->GetConfigEx(itemList, &pData, dataLen))
	{
		return NETNS::DEV_STATUS_SYSTEM_BUSY;
	}
	if (dataLen < sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD) + (nChnID+1)*sizeof(unsigned long))
	{
		delete []pData;
		return NETNS::DEV_STATUS_PARAM_ERR;
	}
	unsigned long *sensorInType = (unsigned long*)(pData + sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD) + nChnID*sizeof(unsigned long));
	*sensorInType = cfg.nVoltage;
	
	if (!SaveConfig(pData, dataLen))
	{
		delete []pData;
		return NETNS::DEV_STATUS_SYSTEM_BUSY;
	}
	delete []pData;

	return NETNS::DEV_STATUS_SUCCESS;
}

NETNS::DEV_RET CTvtDevInterface::ManualAlarmOut( const char* pUser, tuint32 nChnID, bool bStatus) 
{
	if (!CheckUserPass(pUser))
	{
		return NETNS::DEV_STATUS_PERMISSION_DENIED;
	}

	ULONGLONG alarm = 0;
	alarm |= 0x01<<nChnID;

	if (bStatus)
	{
		CMessageMan::Instance()->PostMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_OPEN_ALARM, (unsigned char *)&alarm, sizeof(ULONGLONG));
	}
	else
	{
		CMessageMan::Instance()->PostMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_CLEAN_ALARM, (unsigned char *)&alarm, sizeof(ULONGLONG));
	}

	return NETNS::DEV_STATUS_SUCCESS;
}

//SENSOR_OUTPUT_NUM与通道的概念不一样，待解决
NETNS::DEV_RET CTvtDevInterface::GetAlarmoutCfg(  const char* pUser,tuint32 nChnID, NETNS::DEV_ALARMOUT_CFG &cfg) 
{
	if (!CheckUserPass(pUser))
	{
		return NETNS::DEV_STATUS_PERMISSION_DENIED;
	}

	unsigned char *pData = NULL;
	unsigned long dataLen = 0;
	CMyList <NCFG_ITEM_ID> itemList;
	itemList.AddTail(NCFG_ITEM_ALARM_OUT_NAME);
	if (!CConfigEx::Instance()->GetConfigEx(itemList, &pData, dataLen))
	{
		return NETNS::DEV_STATUS_SYSTEM_BUSY;
	}
	if (dataLen < sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD) + (nChnID+1)*sizeof(NCFG_INFO_SHORT_NAME))
	{
		delete []pData;
		return NETNS::DEV_STATUS_PARAM_ERR;
	}
	NCFG_INFO_SHORT_NAME *name = (NCFG_INFO_SHORT_NAME*)(pData + sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD) + nChnID*sizeof(NCFG_INFO_SHORT_NAME));
	strncpy(cfg.szAlarmoutName, name->name, sizeof(cfg.szAlarmoutName));
	delete []pData;
//
	itemList.RemoveAll();
	itemList.AddTail(NCFG_ITEM_ALARM_OUT_HOLD_TIME);
	if (!CConfigEx::Instance()->GetConfigEx(itemList, &pData, dataLen))
	{
		return NETNS::DEV_STATUS_SYSTEM_BUSY;
	}
	if (dataLen < sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD) + (nChnID+1)*sizeof(unsigned long))
	{
		delete []pData;
		return NETNS::DEV_STATUS_PARAM_ERR;
	}
	unsigned long *holdTime = (unsigned long*)(pData + sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD) + nChnID*sizeof(unsigned long));
	cfg.dwAlarmHoldTime = *holdTime;
	delete []pData;
//
	cfg.nAlarmIdx = nChnID;

	return NETNS::DEV_STATUS_SUCCESS;
}

NETNS::DEV_RET CTvtDevInterface::SetAlarmoutCfg( const char* pUser,tuint32 nChnID, const NETNS::DEV_ALARMOUT_CFG &Cfg) 
{
	if (!CheckUserPass(pUser))
	{
		return NETNS::DEV_STATUS_PERMISSION_DENIED;
	}

	nChnID = Cfg.nAlarmIdx;//assume AlarmIndex is equal to ChannelID
	unsigned char *pData = NULL;
	unsigned long dataLen = 0;
	CMyList <NCFG_ITEM_ID> itemList;
	itemList.AddTail(NCFG_ITEM_ALARM_OUT_NAME);
	if (!CConfigEx::Instance()->GetConfigEx(itemList, &pData, dataLen))
	{
		return NETNS::DEV_STATUS_SYSTEM_BUSY;
	}
	if (dataLen < sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD) + (nChnID+1)*sizeof(NCFG_INFO_SHORT_NAME))
	{
		delete []pData;
		return NETNS::DEV_STATUS_PARAM_ERR;
	}
	NCFG_INFO_SHORT_NAME *name = (NCFG_INFO_SHORT_NAME*)(pData + sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD) + nChnID*sizeof(NCFG_INFO_SHORT_NAME));
	strncpy(name->name, Cfg.szAlarmoutName, sizeof(Cfg.szAlarmoutName));
	
	if (!SaveConfig(pData, dataLen))
	{
		delete []pData;
		return NETNS::DEV_STATUS_SYSTEM_BUSY;
	}
	delete []pData;
//
	itemList.RemoveAll();
	itemList.AddTail(NCFG_ITEM_ALARM_OUT_HOLD_TIME);
	if (!CConfigEx::Instance()->GetConfigEx(itemList, &pData, dataLen))
	{
		return NETNS::DEV_STATUS_SYSTEM_BUSY;
	}
	if (dataLen < sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD) + (nChnID+1)*sizeof(unsigned long))
	{
		delete []pData;
		return NETNS::DEV_STATUS_PARAM_ERR;
	}
	unsigned long *holdTime = (unsigned long*)(pData + sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD) + nChnID*sizeof(unsigned long));
	*holdTime = Cfg.dwAlarmHoldTime;
	
	if (!SaveConfig(pData, dataLen))
	{
		delete []pData;
		return NETNS::DEV_STATUS_SYSTEM_BUSY;
	}
	delete []pData;
//

	return NETNS::DEV_STATUS_SUCCESS;
}

/*
* TCP/IPV4
*/
NETNS::DEV_RET CTvtDevInterface::GetNetBasicCfg(const char* pUser,  NETNS::DEV_IPV4_CFG& cfg) 
{
	if (!CheckUserPass(pUser))
	{
		return NETNS::DEV_STATUS_PERMISSION_DENIED;
	}

	unsigned char *pData = NULL;
	unsigned long dataLen = 0;
	CMyList <NCFG_ITEM_ID> itemList;
	itemList.AddTail(NCFG_ITEM_NETWORK_ADDR_INFO);
	if (!CConfigEx::Instance()->GetConfigEx(itemList, &pData, dataLen))
	{
		return NETNS::DEV_STATUS_SYSTEM_BUSY;
	}
	if (dataLen < sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD) + sizeof(NETWORK_ADDR_INFO))
	{
		delete []pData;
		return NETNS::DEV_STATUS_PARAM_ERR;
	}

	NETWORK_ADDR_INFO *pAddressInfo = reinterpret_cast<NETWORK_ADDR_INFO*>(pData + sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD));
	struct in_addr addr;
	cfg.mode = pAddressInfo->bDHCP > 0 ? NETNS::IP_SETTING_MODE_DHCP : NETNS::IP_SETTING_MODE_STATICIP;

	addr.s_addr = pAddressInfo->staticIP;
	snprintf(cfg.szStaticIP, sizeof(cfg.szStaticIP), "%s", inet_ntoa(addr));

	addr.s_addr = pAddressInfo->gateway;
	snprintf(cfg.szStaticIPRoute, sizeof(cfg.szStaticIP), "%s", inet_ntoa(addr));

	addr.s_addr = pAddressInfo->netMask;
	snprintf(cfg.szStaticIPMask, sizeof(cfg.szStaticIPMask), "%s", inet_ntoa(addr));

	delete []pData;

//
	itemList.RemoveAll();
	itemList.AddTail(NCFG_ITEM_DNS_1);
	if (!CConfigEx::Instance()->GetConfigEx(itemList, &pData, dataLen))
	{
		return NETNS::DEV_STATUS_SYSTEM_BUSY;
	}
	if (dataLen < sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD) + sizeof(unsigned long))
	{
		delete []pData;
		return NETNS::DEV_STATUS_PARAM_ERR;
	}
	unsigned long *dns1 = reinterpret_cast<unsigned long *>(pData + sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD));

	addr.s_addr = *dns1;
	snprintf(cfg.szDnsServer1, sizeof(cfg.szDnsServer1), "%s", inet_ntoa(addr));
	delete []pData;
//
	itemList.RemoveAll();
	itemList.AddTail(NCFG_ITEM_DNS_2);
	if (!CConfigEx::Instance()->GetConfigEx(itemList, &pData, dataLen))
	{
		return NETNS::DEV_STATUS_SYSTEM_BUSY;
	}
	if (dataLen < sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD) + sizeof(unsigned long))
	{
		delete []pData;
		return NETNS::DEV_STATUS_PARAM_ERR;
	}
	unsigned long *dns2 = reinterpret_cast<unsigned long *>(pData + sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD));
	addr.s_addr = *dns2;
	snprintf(cfg.szDnsServer2, sizeof(cfg.szDnsServer1), "%s", inet_ntoa(addr));
	delete []pData;

	return NETNS::DEV_STATUS_SUCCESS;
}

NETNS::DEV_RET CTvtDevInterface::SetNetBasicCfg( const char* pUser,const NETNS::DEV_IPV4_CFG& cfg) 
{
	if (!CheckUserPass(pUser))
	{
		return NETNS::DEV_STATUS_PERMISSION_DENIED;
	}

	unsigned char *pData = NULL;
	unsigned long dataLen = 0;
	CMyList <NCFG_ITEM_ID> itemList;
	itemList.AddTail(NCFG_ITEM_NETWORK_ADDR_INFO);
	if (!CConfigEx::Instance()->GetConfigEx(itemList, &pData, dataLen))
	{
		return NETNS::DEV_STATUS_SYSTEM_BUSY;
	}
	if (dataLen < sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD) + sizeof(NETWORK_ADDR_INFO))
	{
		delete []pData;
		return NETNS::DEV_STATUS_PARAM_ERR;
	}
	NETWORK_ADDR_INFO *networkAddrInfo = (NETWORK_ADDR_INFO*)(pData + sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD));
	networkAddrInfo->bDHCP = (cfg.mode == NETNS::IP_SETTING_MODE_DHCP) ? 1 : 0;
	networkAddrInfo->staticIP = inet_addr(cfg.szStaticIP);
	networkAddrInfo->netMask = inet_addr(cfg.szStaticIPMask);
	networkAddrInfo->gateway = inet_addr(cfg.szStaticIPRoute);
	if (!SaveConfig(pData, dataLen))
	{
		delete []pData;
		return NETNS::DEV_STATUS_SYSTEM_BUSY;
	}
	delete []pData;
//
	itemList.RemoveAll();
	itemList.AddTail(NCFG_ITEM_DNS_1);
	if (!CConfigEx::Instance()->GetConfigEx(itemList, &pData, dataLen))
	{
		return NETNS::DEV_STATUS_SYSTEM_BUSY;
	}
	if (dataLen < sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD) + sizeof(unsigned long))
	{
		delete []pData;
		return NETNS::DEV_STATUS_PARAM_ERR;
	}
	unsigned long *dns1 = (unsigned long*)(pData + sizeof(NCFG_BLOCK_HEAD) + sizeof(unsigned long));
	*dns1 = inet_addr(cfg.szDnsServer1);
	if (!SaveConfig(pData, dataLen))
	{
		delete []pData;
		return NETNS::DEV_STATUS_SYSTEM_BUSY;
	}
	delete []pData;
//
	itemList.RemoveAll();
	itemList.AddTail(NCFG_ITEM_DNS_2);
	if (!CConfigEx::Instance()->GetConfigEx(itemList, &pData, dataLen))
	{
		return NETNS::DEV_STATUS_SYSTEM_BUSY;
	}
	if (dataLen < sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD) + sizeof(unsigned long))
	{
		delete []pData;
		return NETNS::DEV_STATUS_PARAM_ERR;
	}
	unsigned long *dns2 = (unsigned long*)(pData + sizeof(NCFG_BLOCK_HEAD) + sizeof(unsigned long));
	*dns2 = inet_addr(cfg.szDnsServer2);
	if (!SaveConfig(pData, dataLen))
	{
		delete []pData;
		return NETNS::DEV_STATUS_SYSTEM_BUSY;
	}
	delete []pData;

	return NETNS::DEV_STATUS_SUCCESS;
}

NETNS::DEV_RET CTvtDevInterface::GetNetPppoeConfig(const char* pUser,  NETNS::DEV_PPPOE_CFG& cfg) 
{
	if (!CheckUserPass(pUser))
	{
		return NETNS::DEV_STATUS_PERMISSION_DENIED;
	}

	unsigned char *pData = NULL;
	unsigned long dataLen = 0;
	CMyList <NCFG_ITEM_ID> itemList;
	itemList.AddTail(NCFG_ITEM_PPPOE_INFO);
	if (!CConfigEx::Instance()->GetConfigEx(itemList, &pData, dataLen))
	{
		return NETNS::DEV_STATUS_SYSTEM_BUSY;
	}
	if (dataLen < sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD) + sizeof(PPPOE_INFO))
	{
		delete []pData;
		return NETNS::DEV_STATUS_PARAM_ERR;
	}
	PPPOE_INFO *pppoe = (PPPOE_INFO*)(pData + sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD));
	cfg.bSwitch = pppoe->bEnable > 0;
	snprintf(cfg.szUsername, sizeof(cfg.szUsername), "%s", pppoe->user);
	snprintf(cfg.szPassword, sizeof(cfg.szPassword), "%s", pppoe->password);

	delete []pData;

	return NETNS::DEV_STATUS_SUCCESS;
}

NETNS::DEV_RET CTvtDevInterface::SetNetPppoeConfig(const char* pUser, const NETNS::DEV_PPPOE_CFG& cfg) 
{
	if (!CheckUserPass(pUser))
	{
		return NETNS::DEV_STATUS_PERMISSION_DENIED;
	}

	unsigned char *pData = NULL;
	unsigned long dataLen = 0;
	CMyList <NCFG_ITEM_ID> itemList;
	itemList.AddTail(NCFG_ITEM_PPPOE_INFO);
	if (!CConfigEx::Instance()->GetConfigEx(itemList, &pData, dataLen))
	{
		return NETNS::DEV_STATUS_SYSTEM_BUSY;
	}
	if (dataLen < sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD) + sizeof(PPPOE_INFO))
	{
		delete []pData;
		return NETNS::DEV_STATUS_PARAM_ERR;
	}
	PPPOE_INFO *pppoe = (PPPOE_INFO*)(pData + sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD));
	pppoe->bEnable = cfg.bSwitch ? 1 : 0;
	snprintf(pppoe->user, sizeof(pppoe->user), "%s", cfg.szUsername);
	snprintf(pppoe->password, sizeof(pppoe->password), "%s", cfg.szPassword);

	if (!SaveConfig(pData, dataLen))
	{
		delete []pData;
		return NETNS::DEV_STATUS_SYSTEM_BUSY;
	}
	delete []pData;

	return NETNS::DEV_STATUS_SUCCESS;
}

NETNS::DEV_RET CTvtDevInterface::GetPortCfg( const char* pUser, NETNS::DEV_NET_PORT& cfg) 
{
	if (!CheckUserPass(pUser))
	{
		return NETNS::DEV_STATUS_PERMISSION_DENIED;
	}

	unsigned char *pData = NULL;
	unsigned long dataLen = 0;
	CMyList <NCFG_ITEM_ID> itemList;
	itemList.AddTail(NCFG_ITEM_HTTP_PORT);
	if (!CConfigEx::Instance()->GetConfigEx(itemList, &pData, dataLen))
	{
		return NETNS::DEV_STATUS_SYSTEM_BUSY;
	}
	if (dataLen < sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD) + sizeof(unsigned long))
	{
		delete []pData;
		return NETNS::DEV_STATUS_PARAM_ERR;
	}
	unsigned long *httpPort = (unsigned long*)(pData + sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD));
	cfg.wWebPort = *httpPort;
	delete []pData;
//
	itemList.RemoveAll();
	itemList.AddTail(NCFG_ITEM_SERVER_PORT);
	if (!CConfigEx::Instance()->GetConfigEx(itemList, &pData, dataLen))
	{
		return NETNS::DEV_STATUS_SYSTEM_BUSY;
	}
	if (dataLen < sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD) + sizeof(unsigned long))
	{
		delete []pData;
		return NETNS::DEV_STATUS_PARAM_ERR;
	}
	unsigned long *serverPort = (unsigned long*)(pData + sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD));
	cfg.wNetPort = *serverPort;
	delete []pData;
//
	cfg.wRtspPort = 554;
	
	return NETNS::DEV_STATUS_SUCCESS;
}

NETNS::DEV_RET CTvtDevInterface::SetPortCfg( const char* pUser, const NETNS::DEV_NET_PORT& cfg) 
{
	if (!CheckUserPass(pUser))
	{
		return NETNS::DEV_STATUS_PERMISSION_DENIED;
	}

	unsigned char *pData = NULL;
	unsigned long dataLen = 0;
	CMyList <NCFG_ITEM_ID> itemList;
	itemList.AddTail(NCFG_ITEM_HTTP_PORT);
	if (!CConfigEx::Instance()->GetConfigEx(itemList, &pData, dataLen))
	{
		return NETNS::DEV_STATUS_SYSTEM_BUSY;
	}
	if (dataLen < sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD) + sizeof(unsigned long))
	{
		delete []pData;
		return NETNS::DEV_STATUS_PARAM_ERR;
	}
	unsigned long *httpPort = (unsigned long*)(pData + sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD));
	*httpPort = cfg.wWebPort;
	if (!SaveConfig(pData, dataLen))
	{
		delete []pData;
		return NETNS::DEV_STATUS_SYSTEM_BUSY;
	}
	delete []pData;
//
	itemList.RemoveAll();
	itemList.AddTail(NCFG_ITEM_SERVER_PORT);
	if (!CConfigEx::Instance()->GetConfigEx(itemList, &pData, dataLen))
	{
		return NETNS::DEV_STATUS_SYSTEM_BUSY;
	}
	if (dataLen < sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD) + sizeof(unsigned long))
	{
		delete []pData;
		return NETNS::DEV_STATUS_PARAM_ERR;
	}
	unsigned long *serverPort = (unsigned long*)(pData + sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD));
	*serverPort = cfg.wNetPort;
	if (!SaveConfig(pData, dataLen))
	{
		delete []pData;
		return NETNS::DEV_STATUS_SYSTEM_BUSY;
	}
	delete []pData;
//
	CAPIStreamDispatch::Instance()->Restart(cfg.wWebPort, cfg.wRtspPort);

	return NETNS::DEV_STATUS_SUCCESS;
}

NETNS::DEV_RET CTvtDevInterface::GetDdnsCfg( const char* pUser, NETNS::DEV_DDNS_CFG& cfg) 
{
	if (!CheckUserPass(pUser))
	{
		return NETNS::DEV_STATUS_PERMISSION_DENIED;
	}

	unsigned char *pData = NULL;
	unsigned long dataLen = 0;
	CMyList <NCFG_ITEM_ID> itemList;
	itemList.AddTail(NCFG_ITEM_DDNS_INFO);
	if (!CConfigEx::Instance()->GetConfigEx(itemList, &pData, dataLen))
	{
		return NETNS::DEV_STATUS_SYSTEM_BUSY;
	}
	if (dataLen < sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD) + sizeof(DDNS_INFO))
	{
		delete []pData;
		return NETNS::DEV_STATUS_PARAM_ERR;
	}
	DDNS_INFO *ddns = (DDNS_INFO*)(pData + sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD));
	cfg.bSwitch = ddns->bEnable > 0;
	DDNS_SERVER_INFO ddnsServerInfo;
// 	if (CReadDdns::Instance()->GetDddnsServerInfo(ddns->type, &ddnsServerInfo))
// 	{
// 		snprintf(cfg.szDdnsType, sizeof(cfg.szDdnsType), "%s", ddnsServerInfo.szDDNSServerName);
// 		snprintf(cfg.szServerName, sizeof(cfg.szServerName), "%s", ddnsServerInfo.szDDNSServerName);
// 	}
// 	snprintf(cfg.szUsername, sizeof(cfg.szUsername), "%s", ddns->user);
// 	snprintf(cfg.szPassword, sizeof(cfg.szPassword), "%s", ddns->password);
// 	snprintf(cfg.szDomainName, sizeof(cfg.szDomainName), "%s", ddns->domain);

	delete []pData;

	return NETNS::DEV_STATUS_SUCCESS;
}

NETNS::DEV_RET CTvtDevInterface::SetDdnsCfg( const char* pUser, const NETNS::DEV_DDNS_CFG& cfg) 
{
	if (!CheckUserPass(pUser))
	{
		return NETNS::DEV_STATUS_PERMISSION_DENIED;
	}

	unsigned char *pData = NULL;
	unsigned long dataLen = 0;
	CMyList <NCFG_ITEM_ID> itemList;
	itemList.AddTail(NCFG_ITEM_DDNS_INFO);
	if (!CConfigEx::Instance()->GetConfigEx(itemList, &pData, dataLen))
	{
		return NETNS::DEV_STATUS_SYSTEM_BUSY;
	}
	if (dataLen < sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD) + sizeof(DDNS_INFO))
	{
		delete []pData;
		return NETNS::DEV_STATUS_PARAM_ERR;
	}
	DDNS_INFO *ddns = (DDNS_INFO*)(pData + sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD));
// 	ddns->bEnable = cfg.bSwitch ? 1 : 0;
// 	DDNS_SERVER_INFO ddnsServerInfo;
// 	int count = CReadDdns::Instance()->GetDddnsCount();
// 	int type = -1;
// 	for (int i=0; i<count; i++)
// 	{
// 		if (!CReadDdns::Instance()->GetDddnsServerInfo(i, &ddnsServerInfo))
// 		{
// 			break;
// 		}
// 		if (0 == strncmp(cfg.szServerName, ddnsServerInfo.szDDNSServerName, sizeof(cfg.szServerName)))
// 		{
// 			type = i;
// 			break;
// 		}
// 	}
// 	if(type > 0)
// 	{
// 		ddns->type = type;
// 	}
// 	snprintf(ddns->domain, sizeof(ddns->domain), "%s", cfg.szDomainName);
// 	snprintf(ddns->user, sizeof(ddns->user), "%s", cfg.szUsername);
// 	snprintf(ddns->password, sizeof(ddns->password), "%s", cfg.szPassword);

	if (!SaveConfig(pData, dataLen))
	{
		delete []pData;
		return NETNS::DEV_STATUS_SYSTEM_BUSY;
	}
	delete []pData;

	return NETNS::DEV_STATUS_SUCCESS;
}

/*
* User Management
*/
NETNS::DEV_RET CTvtDevInterface::ModifyPassword( const char* pUser, const char* pNewPassword ) 
{
	if (!CheckUserPass(pUser))
	{
		return NETNS::DEV_STATUS_PERMISSION_DENIED;
	}

	unsigned char *pData = NULL;
	unsigned long dataLen = 0;
	CMyList <NCFG_ITEM_ID> itemList;
	itemList.AddTail(NCFG_ITEM_USER);
	if (!CConfigEx::Instance()->GetConfigEx(itemList, &pData, dataLen))
	{
		return NETNS::DEV_STATUS_SYSTEM_BUSY;
	}
	if (dataLen < sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD) + MAX_USER_NUM*sizeof(NCFG_INFO_USER))
	{
		delete []pData;
		return NETNS::DEV_STATUS_PARAM_ERR;
	}
	NCFG_INFO_USER *UserInfo = (NCFG_INFO_USER*)(pData + sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD));
	for (int i=0; i<MAX_USER_NUM; i++)
	{
		if( USER_GROUP_NONE != UserInfo[i].group && strlen(pUser) == strlen(UserInfo[i].name) &&\
			0 == strncmp(pUser, UserInfo[i].name, sizeof(UserInfo[i].name)) )
		{
			snprintf(UserInfo[i].password, sizeof(UserInfo[i].password), "%s", pNewPassword);
			break;
		}
	}
	if (!SaveConfig(pData, dataLen))
	{
		delete []pData;
		return NETNS::DEV_STATUS_SYSTEM_BUSY;
	}
	delete []pData;

	return NETNS::DEV_STATUS_SUCCESS;
}

/*
*
*/
NETNS::DEV_RET CTvtDevInterface::RebootDevice( const char* pUser) 
{
	if (!CheckUserPass(pUser))
	{
		return NETNS::DEV_STATUS_PERMISSION_DENIED;
	}

	PACKCMD *pCmd = NULL;
	CMessageMan::Instance()->PostMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_REBOOT, NULL, 0);

	return NETNS::DEV_STATUS_SUCCESS;
}


NETNS::DEV_RET CTvtDevInterface::RecSearchByDay( const char* pUser, NETNS::DEV_DATE* pDateList, tuint32 dwListSize, tuint32& dwRetNeedSize)
{
	if (!CheckUserPass(pUser))
	{
		return NETNS::DEV_STATUS_PERMISSION_DENIED;
	}

	NETNS::DEV_RET ret = NETNS::DEV_STATUS_PERMISSION_DENIED;
	MESSAGE_DATA msgData;
	int channel = 0;

	ClearMsg(CMD_REPLY_DATA_INFO);
	ClearMsg(CMD_REPLY_DATA_NULL);

	PackCommonPlaybackMsg(msgData, channel, 0, CMD_REQUEST_DATA_SEARCH, DATA_SEARCH_TYPE_YEAR, NET_DATA_SEARCH_PLAYBACK);
	CMessageMan::Instance()->PutMsgToServer(msgData, API_SERVER);

	std::vector<int> msgType;
	msgType.push_back(CMD_REPLY_DATA_INFO);
	msgType.push_back(CMD_REPLY_DATA_NULL);

	if (WaitingForMsg(msgType, 1000))
	{
		m_Lock.Lock();
		if (m_mapMsg.count(CMD_REPLY_DATA_INFO) > 0)
		{
			if (m_mapMsg[CMD_REPLY_DATA_INFO])
			{
				PACKCMD *packCmd = (PACKCMD *)(m_mapMsg[CMD_REPLY_DATA_INFO]);
				NET_DATA_INFO *pDataInfo = (NET_DATA_INFO *)(m_mapMsg[CMD_REPLY_DATA_INFO] + sizeof(PACKCMD));
				if (pDataInfo->dataType == DATA_SEARCH_TYPE_YEAR)
				{
					//sizeof(PACKCMD) + sizeof(NET_DATA_INFO) + sizeof(DVR_DATE)*dateInfoList.GetCount()
					unsigned long len = packCmd->dataLen - sizeof(PACKCMD) -sizeof(NET_DATA_INFO);
					int i=0;
					DVR_DATE *pDvrDate = (DVR_DATE *)(m_mapMsg[CMD_REPLY_DATA_INFO] + sizeof(PACKCMD) + sizeof(NET_DATA_INFO));;
					while(len > 0 && i < dwListSize)
					{
						pDateList[i].wYear = pDvrDate->year;
						pDateList[i].wMonth = pDvrDate->month;
						pDateList[i].wDay = pDvrDate->mday;
						i++;
						len -= sizeof(DVR_DATE);
						pDvrDate++;
					}
					dwRetNeedSize = i;

					ret = NETNS::DEV_STATUS_SUCCESS;
				}

				delete []m_mapMsg[CMD_REPLY_DATA_INFO];
			}
			m_mapMsg.erase(CMD_REPLY_DATA_INFO);
		}
		else if(m_mapMsg.count(CMD_REPLY_DATA_NULL) > 0)
		{
			m_mapMsg.erase(CMD_REPLY_DATA_NULL);

			ret = NETNS::DEV_STATUS_SUCCESS;
		}
		m_Lock.UnLock();

	}

	return ret;
}

NETNS::DEV_RET CTvtDevInterface::RecSearchByTime( const char* pUser, const NETNS::DEV_DATETIME& searchStart, const NETNS::DEV_DATETIME& searchStop
	, NETNS::DEV_REC_SEARCH* pRecList, tuint32 dwListSize, tuint32& dwRetNeedSize) 
{
	if (!CheckUserPass(pUser))
	{
		return NETNS::DEV_STATUS_PERMISSION_DENIED;
	}

	NETNS::DEV_RET ret = NETNS::DEV_STATUS_PERMISSION_DENIED;
	unsigned long startTime = 0, endTime = 0;
	if (!TimeConvert(searchStart.date, searchStart.time, startTime))
	{
		return ret;
	}
	if (!TimeConvert(searchStop.date, searchStop.time, endTime))
	{
		return ret;
	}
	
	MESSAGE_DATA msgData;
	int channel = 0;

	ClearMsg(CMD_REPLY_DATA_INFO);
	ClearMsg(CMD_REPLY_DATA_NULL);

	PackCommonPlaybackMsg(msgData, channel, 0, CMD_REQUEST_DATA_SEARCH, DATA_SEARCH_TYPE_TIME, NET_DATA_SEARCH_PLAYBACK, startTime, endTime);
	CMessageMan::Instance()->PutMsgToServer(msgData, API_SERVER);

	std::vector<int> msgType;
	msgType.push_back(CMD_REPLY_DATA_INFO);
	msgType.push_back(CMD_REPLY_DATA_NULL);

	if (WaitingForMsg(msgType, 1000))
	{
		m_Lock.Lock();
		if (m_mapMsg.count(CMD_REPLY_DATA_INFO) > 0)
		{
			if (m_mapMsg[CMD_REPLY_DATA_INFO])
			{
				PACKCMD *packCmd = (PACKCMD *)(m_mapMsg[CMD_REPLY_DATA_INFO]);
				NET_DATA_INFO *pDataInfo = (NET_DATA_INFO *)(m_mapMsg[CMD_REPLY_DATA_INFO] + sizeof(PACKCMD));
				if (pDataInfo->dataType == DATA_SEARCH_TYPE_TIME /* && channel == channel*/)
				{
					//sizeof(PACKCMD) + sizeof(NET_DATA_INFO) + sizeof(DVR_DATE)*dateInfoList.GetCount()
					unsigned long len = packCmd->dataLen - sizeof(PACKCMD) -sizeof(NET_DATA_INFO);
					int i=0;
					NET_SECTION_INFO *sectionInfo = (NET_SECTION_INFO *)(m_mapMsg[CMD_REPLY_DATA_INFO] + sizeof(PACKCMD) + sizeof(NET_DATA_INFO));;
					while(len > 0 && i < dwListSize)
					{
						ConvertTime(sectionInfo->starTime, pRecList[i].startTime.date, pRecList[i].startTime.time);
						pRecList[i].lastTime = sectionInfo->endTime - sectionInfo->starTime;
						len -= sizeof(NET_SECTION_INFO);
						sectionInfo++;
						i++;
					}
					dwRetNeedSize = i;

					ret = NETNS::DEV_STATUS_SUCCESS;
				}

				delete []m_mapMsg[CMD_REPLY_DATA_INFO];
			}
			m_mapMsg.erase(CMD_REPLY_DATA_INFO);
		}
		else if(m_mapMsg.count(CMD_REPLY_DATA_NULL) > 0)
		{
			m_mapMsg.erase(CMD_REPLY_DATA_NULL);

			ret = NETNS::DEV_STATUS_SUCCESS;
		}
		m_Lock.UnLock();

	}

	return ret;


	return NETNS::DEV_STATUS_SUCCESS;
}

tuint32 CTvtDevInterface::GetDevAttriSaveLen( NETNS::DEV_ATTRIBUTE_LEN type)
{
	tuint32 nLen = 0;
	switch( type)
	{
	case NETNS::DEV_ATTRI_NTP_SERVER_MAX_LEN:
		{
			nLen = 225;
		}	
		break;
	}

	return nLen;
}

int CTvtDevInterface::GetMessage( MESSAGE_DATA &msgData )
{
	if (!msgData.pData || msgData.dataLen <= 0)
	{
		return 0;
	}

	PACKCMD *pCmd = reinterpret_cast<PACKCMD *>(msgData.pData);

	m_Lock.Lock();
	if (m_mapMsg.count(pCmd->cmdType) > 0)
	{
		if (m_mapMsg[pCmd->cmdType])
		{
			delete []m_mapMsg[pCmd->cmdType];
		}
	}
	char *pData = new char[msgData.dataLen];
	memcpy(pData, msgData.pData, msgData.dataLen);
	m_mapMsg[pCmd->cmdType] = pData;
	m_Lock.UnLock();

	return 0;
}

bool CTvtDevInterface::WaitingForMsg( std::vector<int> &msgType, int timeOut )
{
	bool ret = false;
	do 
	{
		m_Lock.Lock();
		for (int i=0; i<msgType.size(); i++)
		{
			if (m_mapMsg.count(msgType[i]) > 0)
			{
				if (m_mapMsg[msgType[i]])
				{
					ret = true;
				}
			}
		}
		m_Lock.UnLock();
		if (!ret)
		{
			PUB_Sleep(1);
		}
		timeOut -= 1;
	} while(timeOut > 0 && !ret);

	return ret;
}

bool CTvtDevInterface::WaitingForMsg(int msgType, int timeOut)
{
	bool ret = false;
	do 
	{
		m_Lock.Lock();
		if (m_mapMsg.count(msgType) > 0)
		{
			if (m_mapMsg[msgType])
			{
				ret = true;
			}
		}
		m_Lock.UnLock();
		if (!ret)
		{
			PUB_Sleep(1);
		}
		timeOut -= 1;
	} while(timeOut > 0 && !ret);

	return ret;
}

void CTvtDevInterface::ClearMsg( int msgType )
{
	m_Lock.Lock();
	if (m_mapMsg.count(msgType) > 0)
	{
		m_mapMsg.erase(msgType);
	}
	m_Lock.UnLock();
}

void CTvtDevInterface::GetResolution( int resolution, NETNS::DEV_RESOLUTION *res, unsigned int size, unsigned int &validSize, bool bPal )
{
	int pos = 0;
	do 
	{
		if (resolution & DVRVIDEO_SIZE_QCIF)
		{
			if (pos >= size)
			{
				break;
			}
			if (bPal)
			{
				res[pos].nWidth = 176;
				res[pos].nHeight = 144;
			}
			else
			{
				res[pos].nWidth = 176;
				res[pos].nHeight = 120;
			}
			pos++;
		}
		if (resolution & DVRVIDEO_SIZE_CIF)
		{
			if (pos >= size)
			{
				break;
			}
			if (bPal)
			{
				res[pos].nWidth = 352;
				res[pos].nHeight = 288;
			}
			else
			{
				res[pos].nWidth = 352;
				res[pos].nHeight = 240;
			}
			pos++;
		}
		if (resolution & DVRVIDEO_SIZE_HD1)
		{
			if (pos >= size)
			{
				break;
			}
			if (bPal)
			{
				res[pos].nWidth = 704;
				res[pos].nHeight = 288;
			}
			else
			{
				res[pos].nWidth = 704;
				res[pos].nHeight = 240;
			}
			pos++;
		}
		if (resolution & DVRVIDEO_SIZE_D1)
		{
			if (pos >= size)
			{
				break;
			}
			if (bPal)
			{
				res[pos].nWidth = 704;
				res[pos].nHeight = 576;
			}
			else
			{
				res[pos].nWidth = 704;
				res[pos].nHeight = 480;
			}
			pos++;
		}
		if (resolution & DVRVIDEO_SIZE_HD1080)
		{
			if (pos >= size)
			{
				break;
			}
			res[pos].nWidth = 1920;
			res[pos].nHeight = 1080;
			pos++;
		}
		if (resolution & DVRVIDEO_SIZE_HD720)
		{
			if (pos >= size)
			{
				break;
			}
			res[pos].nWidth = 1280;
			res[pos].nHeight = 720;
			pos++;
		}
		if (resolution & DVRVIDEO_SIZE_960H)
		{
			if (pos >= size)
			{
				break;
			}
			if (bPal)
			{
				res[pos].nWidth = 960;
				res[pos].nHeight = 576;
			}
			else
			{
				res[pos].nWidth = 960;
				res[pos].nHeight = 480;
			}
			pos++;
		}
		if (resolution & DVRVIDEO_SIZE_VGA)
		{
			if (pos >= size)
			{
				break;
			}
			res[pos].nWidth = 640;
			res[pos].nHeight = 480;
			pos++;
		}
		if (resolution & DVRVIDEO_SIZE_QVGA)
		{
			if (pos >= size)
			{
				break;
			}
			res[pos].nWidth = 320;
			res[pos].nHeight = 240;
			pos++;
		}
		if (resolution & DVRVIDEO_SIZE_960P)
		{
			if (pos >= size)
			{
				break;
			}
			res[pos].nWidth = 1280;
			res[pos].nHeight = 960;
			pos++;
		}
		if (resolution & DVRVIDEO_SIZE_SXGA)
		{
			if (pos >= size)
			{
				break;
			}
			res[pos].nWidth = 1280;
			res[pos].nHeight = 1024;
			pos++;
		}
		if (resolution & DVRVIDEO_SIZE_3M)
		{
			if (pos >= size)
			{
				break;
			}
			res[pos].nWidth = 2048;
			res[pos].nHeight = 1536;
			pos++;
		}

		if (resolution & DVRVIDEO_SIZE_16_9_3M)
		{
			if (pos >= size)
			{
				break;
			}
			res[pos].nWidth = 2304;
			res[pos].nHeight = 1296;
			pos++;
		}
		if (resolution & DVRVIDEO_SIZE_4M)
		{
			if (pos >= size)
			{
				break;
			}
			res[pos].nWidth = 2688;
			res[pos].nHeight = 1520;
			pos++;
		}
		if (resolution & DVRVIDEO_SIZE_5M)
		{
			if (pos >= size)
			{
				break;
			}
			res[pos].nWidth = 2592;
			res[pos].nHeight = 1944;
			pos++;
		}
	} while(0);

	validSize = pos;
}

bool CTvtDevInterface::GetResolutionFromWH( unsigned long width, unsigned long height, int resolution )
{
	bool ret = true;

	if((width == 176 && height == 144) || (width == 176 && height == 120))
	{
		resolution = DVRVIDEO_SIZE_QCIF;
	}
	else if((width == 352 && height == 288) || (width == 352 && height == 240))
	{
		resolution = DVRVIDEO_SIZE_CIF;
	}
	else if((width == 704 && height == 288) || (width == 704 && height == 240))
	{
		resolution = DVRVIDEO_SIZE_HD1;
	}
	else if((width == 704 && height == 576) || (width == 704 && height == 480))
	{
		resolution = DVRVIDEO_SIZE_D1;
	}
	else if(width == 1920 && height == 1080)
	{
		resolution = DVRVIDEO_SIZE_HD1080;
	}
	else if(width == 1280 && height == 720)
	{
		resolution = DVRVIDEO_SIZE_HD720;
	}
	else if((width == 960 && height == 576) || (width == 960 && height == 480))
	{
		resolution = DVRVIDEO_SIZE_960H;
	}
	else if(width == 640 && height == 480)
	{
		resolution = DVRVIDEO_SIZE_VGA;
	}
	else if(width == 320 && height == 240)
	{
		resolution = DVRVIDEO_SIZE_QVGA;
	}
	else if(width == 1280 && height == 960)
	{
		resolution = DVRVIDEO_SIZE_960P;
	}
	else if(width == 1280 && height == 1024)
	{
		resolution = DVRVIDEO_SIZE_SXGA;
	}
	else if(width == 2048 && height == 1536)
	{
		resolution = DVRVIDEO_SIZE_3M;
	}
	else if(width == 2304 && height == 1296)
	{
		resolution = DVRVIDEO_SIZE_16_9_3M;
	}
	else if(width == 2688 && height == 1520)
	{
		resolution = DVRVIDEO_SIZE_4M;
	}
	else if(width == 2592 && height == 1944)
	{
		resolution = DVRVIDEO_SIZE_5M;
	}
	else
	{
		ret = false;
	}
	return ret;
}

bool CTvtDevInterface::PtzCmdConvert( NETNS::DEV_PTZ_CTRL_TYPE ptzCtrlType, int &cmd )
{
	switch(ptzCtrlType)
	{
	case NETNS::PTZ_CTRL_UP:
		cmd = PTZ_CMD_UP;
		break;
	case NETNS::PTZ_CTRL_DOWN:
		cmd = PTZ_CMD_DOWN;
		break;
	case NETNS::PTZ_CTRL_LEFT:
		cmd = PTZ_CMD_LEFT;
		break;
	case NETNS::PTZ_CTRL_RIGHT:
		cmd = PTZ_CMD_RIGHT;
		break;
	case NETNS::PTZ_CTRL_LEFTUP:
		cmd = PTZ_CMD_LEFT_UP;
		break;
	case NETNS::PTZ_CTRL_LEFTDOWN:
		cmd = PTZ_CMD_LEFT_DOWN;
		break;
	case NETNS::PTZ_CTRL_RIGHTUP:
		cmd = PTZ_CMD_RIGHT_UP;
		break;
	case NETNS::PTZ_CTRL_RIGHTDOWN:
		cmd = PTZ_CMD_RIGHT_DOWN;
		break;
	case NETNS::PTZ_CTRL_NEAR:
		cmd = PTZ_CMD_NEAR;
		break;
	case NETNS::PTZ_CTRL_FAR:
		cmd = PTZ_CMD_FAR;
		break;
	case NETNS::PTZ_CTRL_ZOOMIN:
		cmd = PTZ_CMD_ZOOM_IN;
		break;
	case NETNS::PTZ_CTRL_ZOOMOUT:
		cmd = PTZ_CMD_ZOOM_OUT;
		break;
	case NETNS::PTZ_CTRL_IRISOPEN:
		cmd = PTZ_CMD_IRIS_OPEN;
		break;
	case NETNS::PTZ_CTRL_IRISCLOSE:
		cmd = PTZ_CMD_IRIS_CLOSE;
		break;
	case NETNS::PTZ_CTRL_STOP:
		cmd = PTZ_CMD_STOP;
		break;
	default:
		return false;
	}
	return true;
}

bool CTvtDevInterface::SaveConfig( const unsigned char *pData, unsigned long dataLen )
{

	return CConfigEx::Instance()->SetConfig(pData, dataLen);

/*
	MESSAGE_DATA msgData;
	PACKCMD *pCmd = NULL;
	CMessageMan::Instance()->SendMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_CFG_ENTER, NULL, 0, msgData);
	pCmd = reinterpret_cast<PACKCMD *>(msgData.pData);
	if (CMD_REPLY_CFG_SUCC != static_cast<NET_PROTOCOL>(pCmd->cmdType))
	{
		CMessageMan::Instance()->ReleaseMsgBuff(msgData);
		return false;
	}
	CMessageMan::Instance()->ReleaseMsgBuff(msgData);

	//保存信息
	CMessageMan::Instance()->SendMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_CFG_SET, (unsigned char*)pData, dataLen, msgData);
	pCmd = reinterpret_cast<PACKCMD *>(msgData.pData);
	if (CMD_REPLY_CFG_SUCC != static_cast<NET_PROTOCOL>(pCmd->cmdType))
	{
		ret = false;
	}
	CMessageMan::Instance()->ReleaseMsgBuff(msgData);

	//退出配置
	CMessageMan::Instance()->PostMsgDataToServer(m_ClientID, LOCAL_DEVICE_ID, CMD_REQUEST_CFG_LEAVE, NULL, 0);

	return ret;
*/
}

bool CTvtDevInterface::CheckUserPass( const char *pXmlInfo )
{
	bool ret = false;
	
	std::string username;
	std::string passwd;
	if (!ParseXmlUserInfo(pXmlInfo, username, passwd))
	{
		return false;
	}

	if (m_AuthedUser.count(username) > 0)
	{
		if (0 == m_AuthedUser[username].compare(passwd))
		{
			return true;
		}
	}

	unsigned char *pData = NULL;
	unsigned long dataLen = 0;
	CMyList <NCFG_ITEM_ID> itemList;
	itemList.AddTail(NCFG_ITEM_USER);
	if (!CConfigEx::Instance()->GetConfigEx(itemList, &pData, dataLen))
	{
		return false;
	}
	if (dataLen < sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD) + MAX_USER_NUM*sizeof(NCFG_INFO_USER))
	{
		delete []pData;
		return false;
	}
	NCFG_INFO_USER *UserInfo = (NCFG_INFO_USER*)(pData + sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD));
	for (int i=0; i<MAX_USER_NUM; i++)
	{
		if( USER_GROUP_NONE != UserInfo[i].group && username.length() == strlen(UserInfo[i].name) &&\
			0 == username.compare(UserInfo[i].name) )
		{
			if (passwd.length() == strlen(UserInfo[i].password) && 0 == passwd.compare(UserInfo[i].password))
			{
				m_AuthedUser[username] = passwd;
				ret = true;
			}
			break;
		}
	}
	delete []pData;

	return ret;
}

bool CTvtDevInterface::ParseXmlUserInfo( const char *pData, std::string &username, std::string &passwd )
{
	m_XmlParser.SetDoc(pData);
	do 
	{
		if (!m_XmlParser.FindChildElem("username"))
		{
			break;
		}
		m_XmlParser.IntoElem();
		username = m_XmlParser.GetData();
		m_XmlParser.OutOfElem();
		if (!m_XmlParser.FindChildElem("password"))
		{
			break;
		}
		m_XmlParser.IntoElem();
		passwd = m_XmlParser.GetData();
		return true;
	} while(0);

	return false;
}

bool CTvtDevInterface::DaylightInfoToStr( DAYLIGHT_INFO *daylightInfo, char *pOut, unsigned int len )
{
	if (!daylightInfo->enable)
	{
		return true;
	}
	char startRule[32] = {0};
	char stopRule[32] = {0};
	char hhmmss[16] = {0};

	if (daylightInfo->InSecond % 3600 == 0)
	{
		snprintf(hhmmss, sizeof(hhmmss), "%d", daylightInfo->InSecond / 3600);
	}
	else
	{
		snprintf(hhmmss, sizeof(hhmmss), "%d:%d:%d", daylightInfo->InSecond / 3600, (daylightInfo->InSecond % 3600) / 60, \
			daylightInfo->InSecond % 60);
	}
	snprintf(startRule, sizeof(startRule), "M%d.%d.%d/%s", daylightInfo->InMonth+1, daylightInfo->InWeekIndex+1, \
		daylightInfo->InWday, hhmmss);
//
	if (daylightInfo->OutSecond % 3600 == 0)
	{
		snprintf(hhmmss, sizeof(hhmmss), "%d", daylightInfo->OutSecond / 3600);
	}
	else
	{
		snprintf(hhmmss, sizeof(hhmmss), "%d:%d:%d", daylightInfo->OutSecond / 3600, (daylightInfo->OutSecond % 3600) / 60, \
			daylightInfo->OutSecond % 60);
	}	
	snprintf(stopRule, sizeof(stopRule), "M%d.%d.%d/%s", daylightInfo->OutMonth+1, daylightInfo->OutWeekIndex+1, \
		daylightInfo->OutWday, hhmmss);
//
	char dstOffset[16] = {0};
	if (daylightInfo->offSet % 3600 == 0)
	{
		snprintf(hhmmss, sizeof(hhmmss), "%d", daylightInfo->offSet / 3600);
	}
	else
	{
		snprintf(hhmmss, sizeof(hhmmss), "%d:%d:%d", daylightInfo->offSet / 3600, (daylightInfo->offSet % 3600) / 60, \
			daylightInfo->offSet % 60);
	}
	snprintf(pOut, len, "%s%s,%s,%s", "DST", dstOffset, startRule, stopRule);

	return true;
}

bool CTvtDevInterface::StrToDaylightInfo( const char *pIn, unsigned int len, DAYLIGHT_INFO *daylightInfo, char *timeZone )
{
	char name[7] = {0};
	char dstName[7] = {0};
	sscanf(pIn, "%[a-zA-Z]%[0-9:+-]%[a-zA-Z+-]%d", \
		name, timeZone, dstName, &(daylightInfo->offSet));//这个offset没有负值..

	int h = 0, m = 0, s = 0, month = 0, week = 0, wDay = 0;
	sscanf(pIn, "%*[^,],M%d.%d.%d/%d:%d:%d", &month, &week, &wDay, &h, &m, &s);
	daylightInfo->InSecond = h * 3600 + m * 60 + s;

	daylightInfo->InMonth = month;
	daylightInfo->InWeekIndex = week;
	daylightInfo->InWday = wDay;
//
	h = 0;
	m = 0;
	s = 0;
	month = 0;
	week = 0;
	wDay = 0;
	sscanf(pIn, "%*[^,],%*[^,],M%d.%d.%d/%d:%d:%d", &month, &week, &wDay, &h, &m, &s);
	daylightInfo->OutMonth = month;
	daylightInfo->OutWeekIndex = week;
	daylightInfo->OutWday = wDay;

	daylightInfo->OutSecond = h * 3600 + m * 60 + s;

	if (daylightInfo->OutSecond == 0)//by default
	{
		daylightInfo->OutSecond = 7200;
	}
	daylightInfo->type = 0;//周方式

//随便填非0的数值 防止断言
	daylightInfo->InMday = 1;
	daylightInfo->OutMday = 1;

//一些值在内部是从0开始的
	if (daylightInfo->InMonth <= 0 || daylightInfo->OutMonth <= 0 || \
		daylightInfo->InWeekIndex <= 0 || daylightInfo->OutWeekIndex <= 0)
	{
		return false;
	}
	daylightInfo->InMonth -= 1;
	daylightInfo->OutMonth -= 1;
	daylightInfo->InWeekIndex -= 1;
	daylightInfo->OutWeekIndex -= 1;

	return true;
}

bool CTvtDevInterface::CalcTimeZoneIndex( std::string *pSrcTimeZone, int size, const char *pPattern, int &index )
{
	if (!pPattern || strlen(pPattern) <= 3)
	{
		return false;
	}
	int h1=0, m1=0, d1=0;
	int h2=0, m2=0, d2=0;
	char sign1[2] = {0};
	char sign2[2] = {0};
	index = -1;

	sscanf(pPattern, "%[+-]%d:%d:%d", sign2, &h2, &m2, &d2);
	for (int i=0; i<size; i++)
	{
		sscanf(pSrcTimeZone[i].c_str(), "GMT%[+-]%d:%d", sign1, &h1, &m1);
		if (0 == strcmp(sign1, sign2) && h1 == h2 && m1== m2 && d1 == d2)
		{
			index = i;
			break;
		}
	}

	if (index < 0)
	{
		return false;
	}

	return true;
}

tint32 CTvtDevInterface::NetProcExt( const char* pContent, tint32 dwLen, char *& pszRespXml, tint32& nRespLen, NETNS::HTTP_STATUS_CODE& status)
{
	printf( "current function no impl ---------------------------------- %s,%d\n", __FILE__, __LINE__ );
	return 0;
}

void CTvtDevInterface::ParseFailedResource( const char* pUrl, char*& pResourceExt, tuint32 &dwRetExtLen)
{
#define API_NEW_URL_MAX_LENGTH 256
#define API_NEW_URL_MAXLINE 260
#define API_NEW_URL_MAX_EXIST_NUM 5

	//printf( "ParseFaile Resource pUrl = %s\n", pUrl );

	m_langLock.Lock();

	char* pBufNewUrl = new char[API_NEW_URL_MAX_LENGTH];
	char *lang = NULL;

	do 
	{
		lang = strstr(pUrl, "./WebServer/language/");
		if (NULL == lang)
		{
			//printf( "no path web language exist\n" );
			break;
		}

		if( 0 != access("/mnt/mtd/WebServer/language.tar.gz", F_OK ))
		{
			//printf( "no file language.tar.gz exist\n" );
			break;
		}

		lang += strlen("./WebServer/language/");
		sprintf(pBufNewUrl, "/nfsdir/language/%s", lang);
		lang = pBufNewUrl + strlen("/nfsdir/language/");
		char *q = NULL;
		char langName[36] = {0};
		char langPath[API_NEW_URL_MAXLINE] = {0};
		//printf("request language!\n");

		q = strchr(lang, '/');
		memcpy(langName, lang, q - lang);
		memcpy(langPath, pBufNewUrl, q - pBufNewUrl);
		
		//printf("language: %s\n", langName);
		//printf("path: %s\n", langPath);

		if (0 == access(langPath, F_OK))
		{
			//printf( " langpath ==== %s\n", langPath );
			break;
		}

		//printf("language not exist!\n");
		struct stat statbuf = {0};
		struct dirent * de = NULL;
		char tempPath[API_NEW_URL_MAXLINE] = {0};
		DIR *dir = NULL;
		bool bDeleteDir = false;
		int num  = 0;
		char cmd[API_NEW_URL_MAXLINE] = {0};
		//当前存在的language数是否过多
		dir = opendir("/nfsdir/language/");
		while ((de = readdir(dir)) != NULL)
		{
			if (strcmp(de->d_name, ".") == 0|| strcmp(de->d_name, "..") == 0)
			{
				continue;
			}
			sprintf(tempPath, "/nfsdir/language/%s", de->d_name);
			lstat(tempPath, &statbuf);
			if (S_ISDIR(statbuf.st_mode))
			{
				if (bDeleteDir)
				{
					//过多则删除
					printf("remove dir: %s\n", de->d_name);
					sprintf(cmd, "rm -rf %s ", tempPath);
					DVRSystem(cmd);
				}
				else
				{
					num++;
					if (num >= API_NEW_URL_MAX_EXIST_NUM - 1)
					{
						bDeleteDir = true;
					}							
				}
			}
		}
		closedir(dir);
		//解压
		sprintf(cmd, "tar -zxf /mnt/mtd/WebServer/language.tar.gz %s/ -C /nfsdir/language/ ", langName);
		
		//printf("extract language packet!\n");
		DVRSystem(cmd);
		printf("extract language packet cmd %s!\n", cmd );

	} while (0);


	memset( pBufNewUrl, 0, API_NEW_URL_MAX_LENGTH );
	strncpy( pBufNewUrl, pUrl, API_NEW_URL_MAX_LENGTH );

	dwRetExtLen = API_NEW_URL_MAX_LENGTH;
	pResourceExt = pBufNewUrl;

	//printf( "alloc buffer addr = %x\n", (int)pBufNewUrl );

	m_langLock.UnLock();

}

void CTvtDevInterface::DelResponseBuff(char *&pszRespXml)
{
	m_langLock.Lock();
	if( pszRespXml )
	{
		//printf( "release buffer addr %x\n", (int)pszRespXml );
		delete[] pszRespXml;
	}
	m_langLock.UnLock();
}

NETNS::DEV_RET CTvtDevInterface::GetChnList( const char* pUser, tuint32* pChnID, tuint32 dwChnSize, tuint32& dwRetNeedSize)
{	
	int i;
	int count = 0;

	if (!CheckUserPass(pUser))
	{
		return NETNS::DEV_STATUS_PERMISSION_DENIED;
	}

	if( m_iMaxChannels > dwChnSize )
	{
		count = dwChnSize;
	}
	else
	{
		count = m_iMaxChannels;
	}

	if( count < 0 )
	{
		count = 0;
	}

	for( i = 0; i < count; i++ )
	{
		pChnID[i] = i;
	}

	dwRetNeedSize = count;

	return NETNS::DEV_STATUS_SUCCESS;
}

void CTvtDevInterface::SetMaxChnnel( int maxChnn )
{
	m_iMaxChannels = maxChnn;
}
