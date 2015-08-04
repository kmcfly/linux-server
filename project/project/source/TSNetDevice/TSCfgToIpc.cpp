/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : L
** Date         : 2011-04-12
** Name         : TSCfgToIpc.cpp
** Version      : 1.0
** Description  : 向IPC配置转换
** Modify Record:
1:创建
***********************************************************************/
#include "TSCfgToIpc.h"
#include "TSNetDevice.h"
#include "TSNdParam.h"

CTSCfgToIpc::CTSCfgToIpc()
{
	m_pNetDevice = NULL;
	m_pNdParam = NULL;

	m_pIpcBuff = NULL;
	m_ipcDataLen = 0;
	m_ipcBuffLen = 0;

	m_ipcCurresolution = DD_VIDEO_SIZE_720P;
}

CTSCfgToIpc::~CTSCfgToIpc()
{

}

bool CTSCfgToIpc::Initial(CTSNetDevice* pNetDevice, CTSNdParam* pNdParam)
{
	assert(pNetDevice);
	m_pNetDevice = pNetDevice;
	m_pNdParam = pNdParam;
	m_videoStreamIndex = -1;

	m_timeZone[DD_TIME_ZONE_GMT_D12]	= 720;
	m_timeZone[DD_TIME_ZONE_GMT_D11]	= 660;
	m_timeZone[DD_TIME_ZONE_GMT_D10]	= 600;
	m_timeZone[DD_TIME_ZONE_GMT_D9]		= 540;
	m_timeZone[DD_TIME_ZONE_GMT_D8]		= 480;
	m_timeZone[DD_TIME_ZONE_GMT_D7]		= 420;
	m_timeZone[DD_TIME_ZONE_GMT_D6]		= 360;
	m_timeZone[DD_TIME_ZONE_GMT_D5]		= 300;
	m_timeZone[DD_TIME_ZONE_GMT_D4_30]	= 270;
	m_timeZone[DD_TIME_ZONE_GMT_D4]		= 240;
	m_timeZone[DD_TIME_ZONE_GMT_D3_30]	= 210;
	m_timeZone[DD_TIME_ZONE_GMT_D3]		= 180;
	m_timeZone[DD_TIME_ZONE_GMT_D2]		= 120;
	m_timeZone[DD_TIME_ZONE_GMT_D1]		= 60;
	m_timeZone[DD_TIME_ZONE_GMT]		= 0;
	m_timeZone[DD_TIME_ZONE_GMT_A1]		= -60;
	m_timeZone[DD_TIME_ZONE_GMT_A2]		= -120;
	m_timeZone[DD_TIME_ZONE_GMT_A3]		= -180;
	m_timeZone[DD_TIME_ZONE_GMT_A3_30]	= -210;
	m_timeZone[DD_TIME_ZONE_GMT_A4]		= -240;
	m_timeZone[DD_TIME_ZONE_GMT_A4_30]	= -270;
	m_timeZone[DD_TIME_ZONE_GMT_A5]		= -300;
	m_timeZone[DD_TIME_ZONE_GMT_A5_30]	= -330;
	m_timeZone[DD_TIME_ZONE_GMT_A5_45]	= -345;
	m_timeZone[DD_TIME_ZONE_GMT_A6]		= -360;
	m_timeZone[DD_TIME_ZONE_GMT_A6_30]	= -390;
	m_timeZone[DD_TIME_ZONE_GMT_A7]		= -420;
	m_timeZone[DD_TIME_ZONE_GMT_A8]		= -480;
	m_timeZone[DD_TIME_ZONE_GMT_A9]		= -540;
	m_timeZone[DD_TIME_ZONE_GMT_A9_30]	= -570;
	m_timeZone[DD_TIME_ZONE_GMT_A10]	= -600;
	m_timeZone[DD_TIME_ZONE_GMT_A11]	= -660;
	m_timeZone[DD_TIME_ZONE_GMT_A12]	= -720;
	m_timeZone[DD_TIME_ZONE_GMT_A13]	= -780;

	return true;
}

void CTSCfgToIpc::Quit()
{
	ClearCfgList();
	ClearIpcList();

	if(m_pIpcBuff)
	{
		delete [] m_pIpcBuff;
		m_pIpcBuff = NULL;
	}
	m_ipcDataLen = 0;
	m_ipcBuffLen = 0;

	m_pNetDevice = NULL;
	m_pNdParam = NULL;
}

long CTSCfgToIpc::SetConfig(unsigned long itemID, const unsigned char* pData, unsigned long dataLen)
{
	TS_CFG_ITEM cfgItem;
	memset(&cfgItem, 0, sizeof(TS_CFG_ITEM));

	cfgItem.dataLen = dataLen;
	cfgItem.itemID = itemID;

	if(cfgItem.dataLen > 0)
	{
		cfgItem.pData = new unsigned char[cfgItem.dataLen];
		memcpy(cfgItem.pData, pData, cfgItem.dataLen);
		
		m_cfgItemList.push_back(cfgItem);
	}

	return 0;
}

unsigned long CTSCfgToIpc::FlushConfig()
{
	//清空发送给IPC的列表
	ClearIpcList();

	//转换配置数据
	ConvertConfig();

	//打包要发送的配置数据
	PackCmd();

	//返回 > 0 说明有数据需要发送
	return m_ipcDataLen;
}

void CTSCfgToIpc::ClearCfgList()
{
	TS_CFG_ITEM_LIST::iterator iter;
	for(iter = m_cfgItemList.begin(); iter != m_cfgItemList.end(); iter++)
	{
		if(iter->pData)
		{
			delete [] (iter->pData);
		}
	}

	m_cfgItemList.clear();
}

bool CTSCfgToIpc::GetBuff(unsigned char*& pBuff, unsigned long& dataLen)
{
	if(m_ipcDataLen > 0)
	{
		pBuff = m_pIpcBuff;
		dataLen = m_ipcDataLen;
		return true;
	}

	return false;
}

void CTSCfgToIpc::ClearIpcList()
{
	TS_CFG_LIST_IPC_V2::iterator iter;
	for(iter = m_ipcItemList_v2.begin(); iter != m_ipcItemList_v2.end(); iter++)
	{
		if(iter->pData)
		{
			delete [] (iter->pData);
		}
	}
	m_ipcItemList_v2.clear();
}

unsigned long CTSCfgToIpc::PackCmd()
{
	m_ipcDataLen = 0;

	if(m_ipcItemList_v2.size() > 0)
	{
		//填充头
		IPC_V3::NCFG_BLOCK_HEAD* pBlockHead = reinterpret_cast<IPC_V3::NCFG_BLOCK_HEAD*>(m_pIpcBuff);
		pBlockHead->biSize = sizeof(IPC_V3::NCFG_BLOCK_HEAD);
		pBlockHead->ItemNum = m_ipcItemList_v2.size();
		pBlockHead->netcfgver = IPC_V3::NCFG_VERSION;

		//填充索引, 数据
		IPC_V3::NCFG_ITEM_HEAD* pItemHead = reinterpret_cast<IPC_V3::NCFG_ITEM_HEAD*>(m_pIpcBuff + sizeof(IPC_V3::NCFG_BLOCK_HEAD));

		unsigned long index = 0;
		unsigned long offset = 0;
		unsigned long offsetHead = sizeof(IPC_V3::NCFG_BLOCK_HEAD) + pBlockHead->ItemNum * sizeof(IPC_V3::NCFG_ITEM_HEAD);
		TS_CFG_LIST_IPC_V2::iterator iter;
		for(iter = m_ipcItemList_v2.begin(); iter != m_ipcItemList_v2.end(); iter++)
		{
			memcpy(&(pItemHead[index]), &(iter->itemHead), sizeof(IPC_V3::NCFG_ITEM_HEAD));
			memcpy(m_pIpcBuff + offsetHead + offset, iter->pData, iter->itemHead.len);

			offset += iter->itemHead.len;
			index++;
		}

		m_ipcDataLen = sizeof(IPC_V3::NCFG_BLOCK_HEAD) + pBlockHead->ItemNum * sizeof(IPC_V3::NCFG_ITEM_HEAD) + offset;
	}

	return m_ipcDataLen;
}

#define TS_CC_CFG_TO_IPC_V2(TS_CFG_ID, TS_CFG_SUBLEN, TS_CFG_NUM) \
{ \
	cfg_item_v3.itemHead.itemID = TS_CFG_ID; \
	cfg_item_v3.itemHead.subLen = TS_CFG_SUBLEN; \
	cfg_item_v3.itemHead.num = TS_CFG_NUM; \
	cfg_item_v3.itemHead.len = (cfg_item_v3.itemHead.subLen) * (cfg_item_v3.itemHead.num); \
	cfg_item_v3.pData = new unsigned char[cfg_item_v3.itemHead.len]; \
	memset(cfg_item_v3.pData, 0, sizeof(unsigned char) * cfg_item_v3.itemHead.len); \
	m_ipcItemList_v2.push_back(cfg_item_v3); \
	cfg_item_num++; \
	cfg_data_len_v2 += cfg_item_v3.itemHead.len; \
	m_pNdParam->SetModify(cfg_item_v3.itemHead.itemID); \
}

long CTSCfgToIpc::ConvertConfig()
{
	unsigned long cfg_item_num = 0;
	unsigned long cfg_data_len_v2 = 0;
	unsigned int chnnIndex = m_pNetDevice->GetBindChannel();
	unsigned int chnnLocal = m_pNetDevice->GetLocalVideoNum();
	unsigned int currChnn=0;

	TS_CFG_ITEM_LIST::iterator iter;
	for(iter = m_cfgItemList.begin(); iter != m_cfgItemList.end(); iter++)
	{
		TS_CFG_IPC_V3 cfg_item_v3;

		switch(iter->itemID)
		{
		case DD_CONFIG_ITEM_SYSTEM_BASIC:
			{
				assert(iter->dataLen == sizeof(DD_BASIC_CONFIG));
				DD_BASIC_CONFIG* pBasicCfg = reinterpret_cast<DD_BASIC_CONFIG*>(iter->pData);

				{
					TS_CC_CFG_TO_IPC_V2(IPC_V3::NCFG_ITEM_FREQUENCY_HZ, sizeof(unsigned char), 1);
					unsigned char* pIpcVideoFormat = cfg_item_v3.pData;

					*pIpcVideoFormat = (DD_VIDEO_FORMAT_PAL == pBasicCfg->videoFormat) ? IPC_V3::ENUM_CAMERA_FREQ_50HZ : IPC_V3::ENUM_CAMERA_FREQ_60HZ;
				}
			}
			break;
		case DD_CONFIG_ITEM_DATE_TIME:
			{
				assert(iter->dataLen == sizeof(DD_DATE_TIME_CONFIG));
				DD_DATE_TIME_CONFIG* pDataTime = reinterpret_cast<DD_DATE_TIME_CONFIG*>(iter->pData);
				
				{
					TS_CC_CFG_TO_IPC_V2(IPC_V3::NCFG_ITEM_DATE_FORMAT, sizeof(unsigned char), 1);
					unsigned char* pIpcDateFormat = cfg_item_v3.pData;
					
					*pIpcDateFormat = pDataTime->dateFormat;
				}
				//时区
				{
					TS_CC_CFG_TO_IPC_V2(IPC_V3::NCFG_ITEM_TIME_ZONE, sizeof(long), 1);
					long* pIpcTimeZone = reinterpret_cast<long*>(cfg_item_v3.pData);
					
					*pIpcTimeZone = m_timeZone[pDataTime->timeZone];
				}
			}
			break;
		case DD_CONFIG_ITEM_DAYLIGHT_INFO:
			{
				assert(iter->dataLen == sizeof(DD_DAYLIGHT_INFO));
				DD_DAYLIGHT_INFO* pDaylightInfo = reinterpret_cast<DD_DAYLIGHT_INFO*>(iter->pData);

				{
					TS_CC_CFG_TO_IPC_V2(IPC_V3::NCFG_ITEM_DAYLIGHT, sizeof(IPC_V3::NCFG_DAYLIGHT_INFO), 1);
					IPC_V3::NCFG_DAYLIGHT_INFO* pIpcDaylightInfo = (IPC_V3::NCFG_DAYLIGHT_INFO* )cfg_item_v3.pData;

					pIpcDaylightInfo->ucEnableDaylight = pDaylightInfo->enable;
					pIpcDaylightInfo->ucDayLightMode = pDaylightInfo->type;//DAYLIGHT_TYPE
					pIpcDaylightInfo->nDisSecond = pDaylightInfo->offSet;

					if(pIpcDaylightInfo->ucDayLightMode == 0)//周模式
					{
						IPC_V3::NCFG_DAYLIGHT_INFO_MODE_BY_WEEK *pWeek = \
							&pIpcDaylightInfo->daylightModeInfoStart.daylightModeByWeek;
						pWeek->ucMonth = pDaylightInfo->InMonth;
						pWeek->ucWeekOfMonth = pDaylightInfo->InWeekIndex;
						pWeek->ucDayOfWeek = pDaylightInfo->InWday;
						pWeek->ucHour = pDaylightInfo->InSecond/3600;
						pWeek->ucMinute = pDaylightInfo->InSecond/60%60;
						pWeek->ucSecond = pDaylightInfo->InSecond%60;

						pWeek = &pIpcDaylightInfo->daylightModeInfoEnd.daylightModeByWeek;
						pWeek->ucMonth = pDaylightInfo->OutMonth;
						pWeek->ucWeekOfMonth = pDaylightInfo->OutWeekIndex;
						pWeek->ucDayOfWeek = pDaylightInfo->OutWday;
						pWeek->ucHour = pDaylightInfo->OutSecond/3600;
						pWeek->ucMinute = pDaylightInfo->OutSecond/60%60;
						pWeek->ucSecond = pDaylightInfo->OutSecond%60;
					}
					else
					{
						IPC_V3::NCFG_DAYLIGHT_INFO_MODE_BY_DAY *pDayTime = \
							&pIpcDaylightInfo->daylightModeInfoStart.daylightModeByDay;
						pDayTime->ucMonth = pDaylightInfo->InMonth;
						pDayTime->ucDayOfMonth = pDaylightInfo->InMday;
						pDayTime->ucHour = pDaylightInfo->InSecond/3600;
						pDayTime->ucMinute = pDaylightInfo->InSecond/60%60;
						pDayTime->ucSecond = pDaylightInfo->InSecond%60;

						pDayTime = &pIpcDaylightInfo->daylightModeInfoEnd.daylightModeByDay;
						pDayTime->ucMonth = pDaylightInfo->OutMonth;
						pDayTime->ucDayOfMonth = pDaylightInfo->OutMday;
						pDayTime->ucHour = pDaylightInfo->OutSecond/3600;
						pDayTime->ucMinute = pDaylightInfo->OutSecond/60%60;
						pDayTime->ucSecond = pDaylightInfo->OutSecond%60;
					}
				}
			}
			break;
		case DD_CONFIG_ITEM_VIDEO_OSD:
			{
				DD_VIDEO_OSD_CONFIG* pVideoOsdConfigALL = reinterpret_cast<DD_VIDEO_OSD_CONFIG*>(iter->pData);
				DD_VIDEO_OSD_CONFIG* pVideoOsdConfig = &(pVideoOsdConfigALL[chnnIndex]);

				//时间戳
				{
					TS_CC_CFG_TO_IPC_V2(IPC_V3::NCFG_ITEM_TIME_OSD_EX, sizeof(IPC_V3::NCFG_TIME_STAMP_CONFIG), 1);
					IPC_V3::NCFG_TIME_STAMP_CONFIG* pTimeStamp = reinterpret_cast<IPC_V3::NCFG_TIME_STAMP_CONFIG*>(cfg_item_v3.pData);

					m_pNdParam->GetTimeStamp(*pTimeStamp);
					//是否打时间戳
					pTimeStamp->bSwitch = pVideoOsdConfig->enableTimeStamp;
					pTimeStamp->x = pVideoOsdConfig->timeStamp.x * 100;
					pTimeStamp->y = pVideoOsdConfig->timeStamp.y*100;
				}

				//通道名称
				{
					TS_CC_CFG_TO_IPC_V2(IPC_V3::NCFG_ITEM_DEV_NAME_EX, sizeof(IPC_V3::NCFG_INFO_DEV_NAME_EX), 1);
					IPC_V3::NCFG_INFO_DEV_NAME_EX *pCamName =reinterpret_cast<IPC_V3::NCFG_INFO_DEV_NAME_EX *>(cfg_item_v3.pData);

					m_pNdParam->GetCamName(*pCamName);
					pCamName->bSwitch = pVideoOsdConfig->enableCameraName;
					pCamName->x = pVideoOsdConfig->cameraName.x * 100;
					pCamName->y = pVideoOsdConfig->cameraName.y * 100;
				}

				//区域遮挡
				{
					int idx;
					// 暂时固定定义为4
					int osd_mark_count = 4;
					int xwd = 640;
					int yht = 480;

					TS_CC_CFG_TO_IPC_V2(IPC_V3::NCFG_ITEM_SET_OSD_MASK, sizeof(IPC_V3::NCFG_OSD_MASK_CONFIG), osd_mark_count);
					IPC_V3::NCFG_OSD_MASK_CONFIG* pIpcOSDMark = 
						reinterpret_cast<IPC_V3::NCFG_OSD_MASK_CONFIG*>(cfg_item_v3.pData);

					for( idx = 0; idx < osd_mark_count; idx++ )
					{
						memset( pIpcOSDMark, 0, sizeof(IPC_V3::NCFG_OSD_MASK_CONFIG) );
						if( idx < DD_MAX_VIDEO_COVER_NUM && pVideoOsdConfig->cover[idx].enable )
						{
							pIpcOSDMark->bShow = true;
							pIpcOSDMark->rectang.s32X = (xwd*pVideoOsdConfig->cover[idx].area.x)/100;
							pIpcOSDMark->rectang.s32Y = (yht*pVideoOsdConfig->cover[idx].area.y)/100;
							pIpcOSDMark->rectang.u32Width = (xwd*pVideoOsdConfig->cover[idx].area.cx)/100;
							pIpcOSDMark->rectang.u32Height = (yht*pVideoOsdConfig->cover[idx].area.cy)/100;
						}

						pIpcOSDMark++;
					}

				}
			}
			break;
		case DD_CONFIG_ITEM_CHNN_CONFIG:
			{
				DD_CHANNEL_CONFIG *pAllChannlConfig = reinterpret_cast<DD_CHANNEL_CONFIG *>(iter->pData);
				DD_CHANNEL_CONFIG *pChannlConfig = pAllChannlConfig+chnnIndex;

				TS_CC_CFG_TO_IPC_V2(IPC_V3::NCFG_ITEM_DEVICE_NAME, sizeof(IPC_V3::NCFG_INFO_SHORT_NAME), 1);
				IPC_V3::NCFG_INFO_SHORT_NAME *pCameraName = reinterpret_cast<IPC_V3::NCFG_INFO_SHORT_NAME *>(cfg_item_v3.pData);

				unsigned long len = sizeof(pCameraName->name);
				strncpy(pCameraName->name,pChannlConfig->name,len -1);
			}
			break;
		case DD_CONFIG_ITEM_VIDEO_COLOR:
			{
				DD_VIDEO_COLOR_CONFIG* pVideoColorAll = reinterpret_cast<DD_VIDEO_COLOR_CONFIG*>(iter->pData);
				DD_VIDEO_COLOR &videoColor = pVideoColorAll[chnnIndex].videoColor[0];

				IPC_V3::NET_CHNN_COLOR maxColorVal;
				m_pNdParam->GetMaxColorValue(maxColorVal);
				IPC_V3::NET_CHNN_COLOR defaultColor;
				m_pNdParam->GetDefaColorValue(defaultColor);

				{
					TS_CC_CFG_TO_IPC_V2(IPC_V3::NCFG_ITEM_COLOR_BRIGHTNESS, sizeof(unsigned char), 1);
					unsigned char* pIpcBrightness = cfg_item_v3.pData;

					*pIpcBrightness = DVRColorToIPCColor(videoColor.brightness, DVR_DEFAULT_COLOR_BRIGHT, \
						defaultColor.brightness, maxColorVal.brightness);
				}

				{
					TS_CC_CFG_TO_IPC_V2(IPC_V3::NCFG_ITEM_COLOR_CONTRAST, sizeof(unsigned char), 1);
					unsigned char* pIpcContrast = cfg_item_v3.pData;

					*pIpcContrast = DVRColorToIPCColor(videoColor.contrast, DVR_DEFAULT_COLOR_CONTRAST, \
						defaultColor.contrast, maxColorVal.contrast);
				}

				{
					TS_CC_CFG_TO_IPC_V2(IPC_V3::NCFG_ITEM_COLOR_SATURATION, sizeof(unsigned char), 1);
					unsigned char* pIpcSaturation = cfg_item_v3.pData;

					*pIpcSaturation = DVRColorToIPCColor(videoColor.saturation, DVR_DEFAULT_COLOR_SATURATION, \
						defaultColor.saturation, maxColorVal.saturation);
				}

				{
					TS_CC_CFG_TO_IPC_V2(IPC_V3::NCFG_ITEM_COLOR_HUE, sizeof(unsigned char), 1);
					unsigned char* pIpcHue = cfg_item_v3.pData;

					*pIpcHue = DVRColorToIPCColor(videoColor.hue, DVR_DEFAULT_COLOR_HUE, \
						defaultColor.hue, maxColorVal.hue);
				}
			}
			break;
		case DD_CONFIG_ITEM_VIDEO_ROI:
			{
				DD_VIDEO_ROI_CONFIG* pRoiAll = reinterpret_cast<DD_VIDEO_ROI_CONFIG*>(iter->pData);
				if ((chnnIndex-chnnLocal < 0)||(chnnIndex-chnnLocal >127)){
					break;
				}
				DD_VIDEO_ROI_CONFIG &Roi    = pRoiAll[chnnIndex-chnnLocal];
				TS_CC_CFG_TO_IPC_V2(IPC_V3::NCFG_ITEM_ROI,sizeof(DD_VIDEO_ROI_CONFIG),1);
				DD_VIDEO_ROI_CONFIG * pIpcRoi = reinterpret_cast<DD_VIDEO_ROI_CONFIG *>(cfg_item_v3.pData);

				memcpy(pIpcRoi,&Roi,sizeof(DD_VIDEO_ROI_CONFIG));
 
				int xwd = 640;
				int yht = 480;				
				for (unsigned int size = 0; size<DD_MAX_VIDEO_ROI_NUM;size++)
				{
					if (1 == pIpcRoi->roi[size].iEnable){
						pIpcRoi->roi[size].x = (100 * Roi.roi[size].x);
						pIpcRoi->roi[size].y = (100 * Roi.roi[size].y);
						pIpcRoi->roi[size].w = (100 * Roi.roi[size].w);
						pIpcRoi->roi[size].h = (100 * Roi.roi[size].h);
					}
				}
                break;
			}
		case DD_CONFIG_ITEM_CAM_MIRROR:
			{
                unsigned char *pMirrorAll = reinterpret_cast<unsigned char *> (iter->pData);
				currChnn = chnnIndex-chnnLocal;
				if ((currChnn < 0 )||(currChnn >127)){
					break;
				}
				unsigned char &Mirror    = pMirrorAll[currChnn];

				TS_CC_CFG_TO_IPC_V2(IPC_V3::NCFG_ITEM_IMAGE_MIRROR,sizeof(unsigned char),1);
				unsigned char * pIpcMirror = reinterpret_cast<unsigned char *>(cfg_item_v3.pData);

				*pIpcMirror = Mirror;
				break;
			}
		case DD_CONFIG_ITEM_CAM_FILP:
			{
				unsigned char *pFlipAll = reinterpret_cast<unsigned char *> (iter->pData);
				currChnn = chnnIndex-chnnLocal;
				if ((currChnn < 0 )||(currChnn >127)){
					break;
				}
				unsigned char &Flip    = pFlipAll[currChnn];

				TS_CC_CFG_TO_IPC_V2(IPC_V3::NCFG_ITEM_IMAGE_FILP,sizeof(unsigned char),1);
				unsigned char * pIpcFlip = reinterpret_cast<unsigned char *>(cfg_item_v3.pData);

				*pIpcFlip = Flip;
				break;
			}
		case DD_CONFIG_ITEM_CAM_ROTATO:
			{
				unsigned long *pRotatoAll = reinterpret_cast<unsigned long *> (iter->pData);
				currChnn = chnnIndex-chnnLocal;
				if ((currChnn < 0 )||(currChnn >127)){
					break;
				}
				unsigned long &Rotato    = pRotatoAll[currChnn];

				TS_CC_CFG_TO_IPC_V2(IPC_V3::NCFG_ITEM_IMAGE_ROTATO,sizeof(unsigned long),1);
				unsigned long * pIpcRotato = reinterpret_cast<unsigned long *>(cfg_item_v3.pData);

				*pIpcRotato = Rotato;
				break;
			}
		case DD_CONFIG_ITEM_CAM_LENS_DISTORTION:
			{
				DD_LENS_DISTORTION_CONFIG *pDistortionAll = reinterpret_cast< DD_LENS_DISTORTION_CONFIG *>(iter->pData);
				currChnn = chnnIndex-chnnLocal;
				if ((currChnn < 0 )||(currChnn >127)){
					break;
				}
				DD_LENS_DISTORTION_CONFIG &Distortion    = pDistortionAll[currChnn];

				TS_CC_CFG_TO_IPC_V2(IPC_V3::NCFG_ITEM_LENS_DISTORTION,sizeof(DD_LENS_DISTORTION_CONFIG),1);
				DD_LENS_DISTORTION_CONFIG * pIpcDistortoin = reinterpret_cast<DD_LENS_DISTORTION_CONFIG *>(cfg_item_v3.pData);

				pIpcDistortoin->ucLensDistortion = Distortion.ucLensDistortion;
				pIpcDistortoin->ucLensDistortionValue = Distortion.ucLensDistortionValue;
				pIpcDistortoin->noUse[0] = Distortion.noUse[0];
				pIpcDistortoin->noUse[1] = Distortion.noUse[1];
				break;
			}
		case DD_CONFIG_ITEM_CAM_TIME_FOCUS_MODE:
			{
				unsigned char *pFocusAll = reinterpret_cast<unsigned char *> (iter->pData);
				currChnn = chnnIndex-chnnLocal;
				if ((currChnn < 0 )||(currChnn >127)){
					break;
				}
				unsigned char &Focus    = pFocusAll[currChnn];

				TS_CC_CFG_TO_IPC_V2(IPC_V3::NCFG_ITEM_TIME_FOCUS_MODE,sizeof(unsigned char),1);
				unsigned char * pIpcFocus = reinterpret_cast<unsigned char *>(cfg_item_v3.pData);

				*pIpcFocus = Focus;
				break;
			}
		case DD_CONFIG_ITEM_CAM_IRCHANGE_FOCUS:
			{
				unsigned char *pIrFocusAll = reinterpret_cast<unsigned char *> (iter->pData);
				currChnn = chnnIndex-chnnLocal;
				if ((currChnn < 0 )||(currChnn >127)){
					break;
				}
				unsigned char &IrFocus    = pIrFocusAll[currChnn];

				TS_CC_CFG_TO_IPC_V2(IPC_V3::NCFG_ITEM_IRCHANGE_FOCUS,sizeof(unsigned char),1);
				unsigned char * pIpcIrFocus = reinterpret_cast<unsigned char *>(cfg_item_v3.pData);

				*pIpcIrFocus = IrFocus;
				break;
			}
		case DD_CONFIG_ITEM_CAM_INTERVAL_TIME:
			{
				unsigned long *pIntervalTimeAll = reinterpret_cast<unsigned long *> (iter->pData);
				currChnn = chnnIndex-chnnLocal;
				if ((currChnn < 0 )||(currChnn >127)){
					break;
				}
				unsigned long &IntervalTime    = pIntervalTimeAll[currChnn];

				TS_CC_CFG_TO_IPC_V2(IPC_V3::NCFG_ITEM_INTERVAL_TIME,sizeof(unsigned long),1);
				unsigned long * pIpcIntervalTime = reinterpret_cast<unsigned long *>(cfg_item_v3.pData);

				*pIpcIntervalTime = IntervalTime;
				break;
			}
		case DD_CONFIG_ITEM_ENCODE_SCHEDULE:
			{
				DD_ENCODE_CONFIG* pEncodeCfgAll = reinterpret_cast<DD_ENCODE_CONFIG*>(iter->pData);
				DD_ENCODE_CONFIG* pEncodeCfg = &(pEncodeCfgAll[chnnIndex]);

				int encodeIndex = 0;
				//分辨率
				{
					unsigned long* pEncodeIndex = NULL;
					unsigned long* pIpcResolution = NULL;

					TS_CC_CFG_TO_IPC_V2(IPC_V3::NCFG_ITEM_ENCODE_RESOLUTION_V2, 2*sizeof(unsigned long), 1);
					pEncodeIndex = reinterpret_cast<unsigned long*>(cfg_item_v3.pData);
					pIpcResolution = pEncodeIndex+1;

					*pEncodeIndex = encodeIndex;

					m_ipcCurresolution = pEncodeCfg->resolution;

					switch(pEncodeCfg->resolution)
					{
					case DD_VIDEO_SIZE_QCIF:
						*pIpcResolution = IPC_V3::DVRVIDEO_SIZE_QCIF;
						break;
					case DD_VIDEO_SIZE_CIF:
						*pIpcResolution = IPC_V3::DVRVIDEO_SIZE_CIF;
						break;
					case DD_VIDEO_SIZE_HD1:
						*pIpcResolution = IPC_V3::DVRVIDEO_SIZE_HD1;
						break;
					case DD_VIDEO_SIZE_D1:
						*pIpcResolution = IPC_V3::DVRVIDEO_SIZE_D1;
						break;
					case DD_VIDEO_SIZE_QVGA:
						*pIpcResolution = IPC_V3::DVRVIDEO_SIZE_QVGA;
						break;
					case DD_VIDEO_SIZE_VGA:
						*pIpcResolution = IPC_V3::DVRVIDEO_SIZE_VGA;
						break;
					//case DD_VIDEO_SIZE_XVGA:
					//	*pIpcResolution = IPC_V3::DVRVIDEO_SIZE_XVGA;
					//	break;
					case DD_VIDEO_SIZE_QQVGA:
						*pIpcResolution = IPC_V3::DVRVIDEO_SIZE_QQVGA;
						break;
					case DD_VIDEO_SIZE_720P:
						*pIpcResolution = IPC_V3::DVRVIDEO_SIZE_720P;
						break;
					case DD_VIDEO_SIZE_1080P:
						*pIpcResolution = IPC_V3::DVRVIDEO_SIZE_1080P;
						break;
					case DD_VIDEO_SIZE_960P:
						*pIpcResolution = IPC_V3::DVRVIDEO_SIZE_XVGA;
						break;
					case DD_VIDEO_SIZE_SXGA:
						*pIpcResolution = IPC_V3::DVRVIDEO_SIZE_14M;
						break;
					case DD_VIDEO_SIZE_3M:
						*pIpcResolution = IPC_V3::DVRVIDEO_SIZE_QXGA;
						break;
					case DD_VIDEO_SIZE_16_9_3M:
						*pIpcResolution = IPC_V3::DVRVIDEO_SIZE_16_9_3M;
						break;
					case DD_VIDEO_SIZE_4M:
						*pIpcResolution = IPC_V3::DVRVIDEO_SIZE_4M;
						break;
					case DD_VIDEO_SIZE_5M:
						*pIpcResolution = IPC_V3::DVRVIDEO_SIZE_5M;
						break;
					default:
						assert(false); //error
						*pIpcResolution = IPC_V3::DVRVIDEO_SIZE_CIF;
						break;
					}
				}

				int resolutionIndex = 0;
				for(resolutionIndex = 0; resolutionIndex < 32; resolutionIndex++)
				{
					if((0x01 << resolutionIndex) & pEncodeCfg->resolution)
					{
						break;
					}
				}
				ND_DEV_INFO devInfo;
				m_pNdParam->GetDevInfo(encodeIndex, devInfo);

				//画面质量
				{
					unsigned long* pEncodeIndex = NULL;
					unsigned long* pIpcQuality = NULL;

					TS_CC_CFG_TO_IPC_V2(IPC_V3::NCFG_ITEM_ENCODE_VIDEO_QUALITY_V2, 2*sizeof(unsigned long), 1);
					pEncodeIndex = reinterpret_cast<unsigned long*>(cfg_item_v3.pData);
					pIpcQuality = pEncodeIndex+1;

					*pEncodeIndex = encodeIndex;

/*
					if(pEncodeCfg->encodeType != DD_VIDEO_ENCODE_MODE_CBR)
					{
						switch(pEncodeCfg->quality)
						{
						case DD_IMAGE_QUALITY_LOWEST:
							*pIpcQuality = IPC_V3::IMAGE_QUALITY_LOWEST;
							break;
						case DD_IMAGE_QUALITY_LOWER:
							*pIpcQuality = IPC_V3::IMAGE_QUALITY_LOWER;
							break;
						case DD_IMAGE_QUALITY_LOW:
							*pIpcQuality = IPC_V3::IMAGE_QUALITY_LOWER;
							break;
						case DD_IMAGE_QUALITY_MEDIUM:
							*pIpcQuality = IPC_V3::IMAGE_QUALITY_MEDIUM;
							break;
						case DD_IMAGE_QUALITY_HEIGHTER:
							*pIpcQuality = IPC_V3::IMAGE_QUALITY_HEIGHTER;
							break;
						case DD_IMAGE_QUALITY_HEIGHTEST:
							*pIpcQuality = IPC_V3::IMAGE_QUALITY_HEIGHTEST;
							break;
						default:
							assert(false); //error
							*pIpcQuality = IPC_V3::IMAGE_QUALITY_MEDIUM;
							break;
						}
					}
					else
*/
					{
						*pIpcQuality = (pEncodeCfg->maxBitrate > devInfo.maxBitRate[resolutionIndex]) ? devInfo.maxBitRate[resolutionIndex] : pEncodeCfg->maxBitrate;
					}
				}

				//帧率
				{
					unsigned long* pEncodeIndex = NULL;
					unsigned long* pIpcFrameRate = NULL;

					TS_CC_CFG_TO_IPC_V2(IPC_V3::NCFG_ITEM_ENCODE_FRAME_RATE_V2,2*sizeof(unsigned long),1);

					pEncodeIndex = reinterpret_cast<unsigned long*>(cfg_item_v3.pData);
					pIpcFrameRate = pEncodeIndex+1;

					*pEncodeIndex = encodeIndex;
					*pIpcFrameRate = (pEncodeCfg->rate > devInfo.ucRate[resolutionIndex]) ? devInfo.ucRate[resolutionIndex] : pEncodeCfg->rate;
				}

				//码流类型
				{
					unsigned long* pEncodeIndex = NULL;
					unsigned long* pIpcEncodeType = NULL;

					TS_CC_CFG_TO_IPC_V2(IPC_V3::NCFG_ITEM_ENCODE_ENCODE_TYPE_V2, 2*sizeof(unsigned long), 1);

					pEncodeIndex = reinterpret_cast<unsigned long*>(cfg_item_v3.pData);
					pIpcEncodeType = pEncodeIndex+1;

					*pEncodeIndex = encodeIndex;

					switch(pEncodeCfg->encodeType)
					{
					case DD_VIDEO_ENCODE_MODE_VBR:
						*pIpcEncodeType = IPC_V3::VIDEO_ENCODE_MODE_VBR;
						break;
					case DD_VIDEO_ENCODE_MODE_CBR:
						*pIpcEncodeType = IPC_V3::VIDEO_ENCODE_MODE_CBR;
						break;
					default:
						//error
						assert(false);
						*pIpcEncodeType = IPC_V3::VIDEO_ENCODE_MODE_VBR;
						break;
					}
				}
			}
			break;
		case DD_CONFIG_ITEM_ENCODE_NETWORK:
		case DD_CONFIG_ITEM_ENCODE_THIRD_STREAM:
			{
				DD_ENCODE_CONFIG* pEncodeCfgAll = reinterpret_cast<DD_ENCODE_CONFIG*>(iter->pData);
				DD_ENCODE_CONFIG* pEncodeCfg = &(pEncodeCfgAll[chnnIndex]);

				int encodeIndex = 1;

				if( DD_CONFIG_ITEM_ENCODE_THIRD_STREAM == iter->itemID )
				{
					if( m_pNdParam && m_pNdParam->GetNvrShowStreamNum() >= 2 )
					{
						encodeIndex = m_pNdParam->GetNvrShowStreamNum();
					}
					else
					{
						break;
					}
				}

				//分辨率
				{
					unsigned long* pEncodeIndex = NULL;
					unsigned long* pIpcResolution = NULL;

					TS_CC_CFG_TO_IPC_V2(IPC_V3::NCFG_ITEM_ENCODE_RESOLUTION_V2, 2*sizeof(unsigned long), 1);
					pEncodeIndex = reinterpret_cast<unsigned long*>(cfg_item_v3.pData);
					pIpcResolution = pEncodeIndex+1;


					*pEncodeIndex = encodeIndex;

					switch(pEncodeCfg->resolution)
					{
					case DD_VIDEO_SIZE_QCIF:
						*pIpcResolution = IPC_V3::DVRVIDEO_SIZE_QCIF;
						break;
					case DD_VIDEO_SIZE_CIF:
						*pIpcResolution = IPC_V3::DVRVIDEO_SIZE_CIF;
						if( DD_CONFIG_ITEM_ENCODE_THIRD_STREAM == iter->itemID )
						{
							*pIpcResolution = IPC_V3::DVRVIDEO_SIZE_NVR_INNER;
						}
						break;
					case DD_VIDEO_SIZE_HD1:
						*pIpcResolution = IPC_V3::DVRVIDEO_SIZE_HD1;
						break;
					case DD_VIDEO_SIZE_D1:
						*pIpcResolution = IPC_V3::DVRVIDEO_SIZE_D1;
						break;
					case DD_VIDEO_SIZE_QVGA:
						*pIpcResolution = IPC_V3::DVRVIDEO_SIZE_QVGA;
						break;
					case DD_VIDEO_SIZE_VGA:
						*pIpcResolution = IPC_V3::DVRVIDEO_SIZE_VGA;
						break;
					//case DD_VIDEO_SIZE_XVGA:
					//	*pIpcResolution = IPC_V3::DVRVIDEO_SIZE_XVGA;
					//	break;
					case DD_VIDEO_SIZE_QQVGA:
						*pIpcResolution = IPC_V3::DVRVIDEO_SIZE_QQVGA;
						break;
					case DD_VIDEO_SIZE_720P:
						*pIpcResolution = IPC_V3::DVRVIDEO_SIZE_720P;
						break;
					case DD_VIDEO_SIZE_1080P:
						*pIpcResolution = IPC_V3::DVRVIDEO_SIZE_1080P;
						break;
					case DD_VIDEO_SIZE_960P:
						*pIpcResolution = IPC_V3::DVRVIDEO_SIZE_XVGA;
						break;
					case DD_VIDEO_SIZE_SXGA:
						*pIpcResolution = IPC_V3::DVRVIDEO_SIZE_14M;
						break;
					case DD_VIDEO_SIZE_3M:
						*pIpcResolution = IPC_V3::DVRVIDEO_SIZE_QXGA;
						break;
					case DD_VIDEO_SIZE_16_9_3M:
						*pIpcResolution = IPC_V3::DVRVIDEO_SIZE_16_9_3M;
						break;
					case DD_VIDEO_SIZE_4M:
						*pIpcResolution = IPC_V3::DVRVIDEO_SIZE_4M;
						break;
					case DD_VIDEO_SIZE_5M:
						*pIpcResolution = IPC_V3::DVRVIDEO_SIZE_5M;
						break;
					default:
						assert(false); //error
						*pIpcResolution = IPC_V3::DVRVIDEO_SIZE_CIF;
						break;
					}
				}

				ND_DEV_INFO devInfo;
				m_pNdParam->GetDevInfo(encodeIndex, devInfo);

				int resolutionIndex = 0;
				for(resolutionIndex = 0; resolutionIndex < 32; resolutionIndex++)
				{
					if((0x01 << resolutionIndex) & pEncodeCfg->resolution)
					{
						break;
					}
				}

				//画面质量
				{
					unsigned long* pEncodeIndex = NULL;
					unsigned long* pIpcQuality = NULL;

					TS_CC_CFG_TO_IPC_V2(IPC_V3::NCFG_ITEM_ENCODE_VIDEO_QUALITY_V2, 2*sizeof(unsigned long), 1);
					pEncodeIndex = reinterpret_cast<unsigned long*>(cfg_item_v3.pData);
					pIpcQuality = pEncodeIndex+1;

					*pEncodeIndex = encodeIndex;

/*
					if(pEncodeCfg->encodeType != DD_VIDEO_ENCODE_MODE_CBR)
					{
						switch(pEncodeCfg->quality)
						{
						case DD_IMAGE_QUALITY_LOWEST:
							*pIpcQuality = IPC_V3::IMAGE_QUALITY_LOWEST;
							break;
						case DD_IMAGE_QUALITY_LOWER:
							*pIpcQuality = IPC_V3::IMAGE_QUALITY_LOWER;
							break;
						case DD_IMAGE_QUALITY_LOW:
							*pIpcQuality = IPC_V3::IMAGE_QUALITY_LOWER;
							break;
						case DD_IMAGE_QUALITY_MEDIUM:
							*pIpcQuality = IPC_V3::IMAGE_QUALITY_MEDIUM;
							break;
						case DD_IMAGE_QUALITY_HEIGHTER:
							*pIpcQuality = IPC_V3::IMAGE_QUALITY_HEIGHTER;
							break;
						case DD_IMAGE_QUALITY_HEIGHTEST:
							*pIpcQuality = IPC_V3::IMAGE_QUALITY_HEIGHTEST;
							break;
						default:
							assert(false); //error
							*pIpcQuality = IPC_V3::IMAGE_QUALITY_MEDIUM;
							break;
						}
					}
					else
*/
					{
						*pIpcQuality = (pEncodeCfg->maxBitrate > devInfo.maxBitRate[resolutionIndex]) ? devInfo.maxBitRate[resolutionIndex] : pEncodeCfg->maxBitrate;
					}
				}

				//帧率
				{
					unsigned long* pEncodeIndex = NULL;
					unsigned long* pIpcFrameRate = NULL;

					TS_CC_CFG_TO_IPC_V2(IPC_V3::NCFG_ITEM_ENCODE_FRAME_RATE_V2,2*sizeof(unsigned long),1);

					pEncodeIndex = reinterpret_cast<unsigned long*>(cfg_item_v3.pData);
					pIpcFrameRate = pEncodeIndex+1;

					*pEncodeIndex = encodeIndex;
					*pIpcFrameRate = (pEncodeCfg->rate > devInfo.ucRate[resolutionIndex]) ? devInfo.ucRate[resolutionIndex] : pEncodeCfg->rate;
				}

				//码流类型
				{
					unsigned long* pEncodeIndex = NULL;
					unsigned long* pIpcEncodeType = NULL;

					TS_CC_CFG_TO_IPC_V2(IPC_V3::NCFG_ITEM_ENCODE_ENCODE_TYPE_V2, 2*sizeof(unsigned long), 1);

					pEncodeIndex = reinterpret_cast<unsigned long*>(cfg_item_v3.pData);
					pIpcEncodeType = pEncodeIndex+1;

					*pEncodeIndex = encodeIndex;

					switch(pEncodeCfg->encodeType)
					{
					case DD_VIDEO_ENCODE_MODE_VBR:
						*pIpcEncodeType = IPC_V3::VIDEO_ENCODE_MODE_VBR;
						break;
					case DD_VIDEO_ENCODE_MODE_CBR:
						*pIpcEncodeType = IPC_V3::VIDEO_ENCODE_MODE_CBR;
						break;
					default:
						//error
						assert(false);
						*pIpcEncodeType = IPC_V3::VIDEO_ENCODE_MODE_VBR;
						break;
					}
				}
			}
			break;
		case DD_CONFIG_ITEM_MOTION_SETUP:
			{
				DD_MOTION_CONFIG* pMotionCfgAll = reinterpret_cast<DD_MOTION_CONFIG*>(iter->pData);
				DD_MOTION_CONFIG* pMotionCfg = &(pMotionCfgAll[chnnIndex]);

				//移动侦测 使能
				{
					TS_CC_CFG_TO_IPC_V2(IPC_V3::NCFG_ITEM_MOTION_ENABLE, sizeof(unsigned char), 1);
					unsigned char* pIpcEnable = reinterpret_cast<unsigned char*>(cfg_item_v3.pData);

					*pIpcEnable = pMotionCfg->enable;
				}

				//移动侦测 区域
				{
					TS_CC_CFG_TO_IPC_V2(IPC_V3::NCFG_ITEM_MOTION_AREA_INFO, sizeof(IPC_V3::NCFG_INFO_MOTION_INFO), 1);
					IPC_V3::NCFG_INFO_MOTION_INFO* pIpcMotionInfo = reinterpret_cast<IPC_V3::NCFG_INFO_MOTION_INFO*>(cfg_item_v3.pData);

					//.widthNum = 22
					//.hightNum = 15
					assert(pMotionCfg->area.widthNum == 22);
					assert(pMotionCfg->area.hightNum == 15);
					assert(sizeof(pIpcMotionInfo->gridData) < sizeof(pMotionCfg->area.area));

					pIpcMotionInfo->motiontype = 1;
					pIpcMotionInfo->Sensitivity = pMotionCfg->area.sensitivity;
					//不同分辨率IPC的Motion区域不一样
					m_pNdParam->GetMotionArea(pIpcMotionInfo->xPixel, pIpcMotionInfo->yPixel, pIpcMotionInfo->gridWidth, pIpcMotionInfo->gridHeight);
					
					//IPC中以 的宽高, 不固定有分辨率不定
					//而DVR则 全部使用
					memset(pIpcMotionInfo->gridData, 0, sizeof(pIpcMotionInfo->gridData));
					for(int j = 0; j < pIpcMotionInfo->gridHeight; j++)
					{
						for(int i = 0; i < pIpcMotionInfo->gridWidth; i++)
						{
							int col = ((double)(i * pMotionCfg->area.widthNum) + 0.5) / (double)(pIpcMotionInfo->gridWidth);
							int row = ((double)(j * pMotionCfg->area.hightNum) + 0.5) / (double)(pIpcMotionInfo->gridHeight);
							unsigned char* pMotionArea = reinterpret_cast<unsigned char*>(pMotionCfg->area.area);
							if(pMotionArea[(pMotionCfg->area.widthNum / 8 + 1) * row + col / 8] & (0x01 << (col%8)))
							{
								pIpcMotionInfo->gridData[pIpcMotionInfo->yPixel + j] |= ((unsigned long)(0x1) << (pIpcMotionInfo->yPixel + i));
							}
						}
					}
				}

				//移动侦测报警保持时间
				{
					TS_CC_CFG_TO_IPC_V2(IPC_V3::NCFG_ITEM_MOTION_HOLD_TIME, sizeof(unsigned long), 1);
					unsigned long* pIpcTime = reinterpret_cast<unsigned long*>(cfg_item_v3.pData);

					*pIpcTime = pMotionCfg->holdTime;
				}
			}
			break;
		case DD_CONFIG_ITEM_SENSOR_SETUP:
			{
				chnnIndex = m_pNetDevice->GetBindSensorChannel();
				DD_SENSOR_CONFIG* pSensorAll = reinterpret_cast<DD_SENSOR_CONFIG*>(iter->pData);
				DD_SENSOR_CONFIG* pSensor = &(pSensorAll[chnnIndex]);
				int sensorInNum = m_pNdParam->GetSensorInNum();
				//传感器 使能
				{
					TS_CC_CFG_TO_IPC_V2(IPC_V3::NCFG_ITEM_SENSOR_IN_ENABLE, sizeof(unsigned char), sensorInNum);
					unsigned char* pIpcEnable = reinterpret_cast<unsigned char*>(cfg_item_v3.pData);
					memset(pIpcEnable, 0, sizeof(unsigned char)*sensorInNum);

					*pIpcEnable = pSensor->enable;
				}

				//传感器 类型
				{
					TS_CC_CFG_TO_IPC_V2(IPC_V3::NCFG_ITEM_SENSOR_IN_TYPE, sizeof(unsigned char), sensorInNum);
					unsigned char* pIpcType = reinterpret_cast<unsigned char*>(cfg_item_v3.pData);
					memset(pIpcType, 0, sizeof(unsigned char)*sensorInNum);

					*pIpcType = pSensor->bNO ? 0 : 1;
				}
				//传感器 名称
				{
					TS_CC_CFG_TO_IPC_V2(IPC_V3::NCFG_ITEM_SENSOR_IN_NAME, sizeof(IPC_V3::NCFG_INFO_SHORT_NAME), sensorInNum);
					IPC_V3::NCFG_INFO_SHORT_NAME* pIpcName = reinterpret_cast<IPC_V3::NCFG_INFO_SHORT_NAME*>(cfg_item_v3.pData);
					memset(pIpcName, 0, sizeof(IPC_V3::NCFG_INFO_SHORT_NAME)*sensorInNum);

					memcpy(pIpcName, pSensor->name, sizeof(IPC_V3::NCFG_INFO_SHORT_NAME));
				}
				//传感器保持时间
				//{
				//	TS_CC_CFG_TO_IPC_V2(IPC_V3::NCFG_ITEM_SENSOR_HOLD_TIME, sizeof(unsigned long), 1);
				//	unsigned long* pIpcTime = reinterpret_cast<unsigned long*>(cfg_item_v3.pData);

				//	*pIpcTime = pSensor->holdTime;
				//}
			}
			break;
		case DD_CONFIG_ITEM_SENSOR_ALARM_OUT:
			{
				chnnIndex = m_pNetDevice->GetBindSensorChannel();
				DD_TRIGGER_ALARM_OUT* pSensorAll = reinterpret_cast<DD_TRIGGER_ALARM_OUT*>(iter->pData);
				DD_TRIGGER_ALARM_OUT* pSensor = &(pSensorAll[chnnIndex]);
				int sensorInNum = m_pNdParam->GetSensorInNum();

				//传感器触发报警输出
				{
				TS_CC_CFG_TO_IPC_V2(IPC_V3::NCFG_ITEM_SENSOR_IN_TO_ALARM_OUT, sizeof(unsigned long), sensorInNum);
				unsigned long* pIpcAlarmOut = reinterpret_cast<unsigned long*>(cfg_item_v3.pData);
				memset(pIpcAlarmOut, 0, sizeof(unsigned long) * sensorInNum);

				*pIpcAlarmOut = pSensor->toAlarmOut;
				}
				//传感器能够触发EMAIL
				{
					TS_CC_CFG_TO_IPC_V2(IPC_V3::NCFG_ITEM_SENSOR_IN_TO_EMAIL_ENABLE, sizeof(unsigned char), sensorInNum);
					unsigned char* pIpcEmailEnable = reinterpret_cast<unsigned char*>(cfg_item_v3.pData);
					memset(pIpcEmailEnable, 0, sizeof(unsigned char) * sensorInNum);

					*pIpcEmailEnable = pSensor->sendEmail;
				}
			}
			break;
		//case DD_CONFIG_ITEM_SENSOR_TO_RECORD:
		//	{
		//		DD_TRIGGER_RECORD* pSensorAll = reinterpret_cast<DD_TRIGGER_RECORD*>(iter->pData);
		//		DD_TRIGGER_RECORD* pSensor = &(pSensorAll[chnnIndex]);

		//		//传感器触发抓图
		//		{
		//			TS_CC_CFG_TO_IPC_V2(IPC_V3::NCFG_ITEM_SENSOR_IN_TO_SNAP, sizeof(ULONGLONG), 1);
		//			ULONGLONG* pIpcToSnap = reinterpret_cast<ULONGLONG*>(cfg_item_v3.pData);

		//			memcpy(pIpcToSnap, pSensor->snapCH, sizeof(ULONGLONG));
		//		}
		//	}
		//	break;
		//case DD_CONFIG_ITEM_SENSOR_TO_PTZ:
		//	{
		//		DD_TRIGGER_PTZ* pSensorAll = reinterpret_cast<DD_TRIGGER_PTZ*>(iter->pData);
		//		DD_TRIGGER_PTZ* pSensor = &(pSensorAll[chnnIndex]);

		//		//传感器触发云台预制点
		//		{
		//			TS_CC_CFG_TO_IPC_V2(IPC_V3::NCFG_ITEM_SENSOR_IN_TO_PTZ, sizeof(IPC_V3::NCFG_INFO_TOPTZ), 1);
		//			IPC_V3::NCFG_INFO_TOPTZ* pIpcToPtz = reinterpret_cast<IPC_V3::NCFG_INFO_TOPTZ*>(cfg_item_v3.pData);

		//			if (pSensor->toPTZType == 1)//预置点
		//			{
		//				pIpcToPtz->ucToPreset = pSensor->toIndex;
		//				pIpcToPtz->ucBackPreset = pSensor->backIndex;
		//				pIpcToPtz->cIsEnableBack = 1;
		//				pIpcToPtz->cIsEnableTo = 1;
		//			}
		//			else if (pSensor->toPTZType == 2)//巡航线
		//			{
		//				pIpcToPtz->cIsEnableCruise = 1;
		//				pIpcToPtz->ucToCruise = pSensor->toIndex;
		//			}
		//		}
		//	}
		//	break;
		case DD_CONFIG_ITEM_SENSOR_SCHEDULE:
			{
				chnnIndex = m_pNetDevice->GetBindSensorChannel();
				DD_WEEK_SCHEDULE* pSensorWeekAll = reinterpret_cast<DD_WEEK_SCHEDULE*>(iter->pData);
				int sensorInNum = m_pNdParam->GetSensorInNum();

			    //这里出错可能是因为对于sensor的网络通道没开，导致路数不够，访问越界
                if ( NULL != pSensorWeekAll )
                {
					DD_WEEK_SCHEDULE* pSensorWeek = &(pSensorWeekAll[chnnIndex]);
					//传感器周计划
					{
						TS_CC_CFG_TO_IPC_V2(IPC_V3::NCFG_ITEM_SENSOR_IN_SCH_WEEK, sizeof(IPC_V3::WEEK_SCHEDULE), sensorInNum);
						IPC_V3::WEEK_SCHEDULE* pIpcWeek = reinterpret_cast<IPC_V3::WEEK_SCHEDULE*>(cfg_item_v3.pData);
						memset(pIpcWeek, 0, sizeof(IPC_V3::WEEK_SCHEDULE)*sensorInNum);
						if (NULL != pIpcWeek)
						{
							memcpy(pIpcWeek, pSensorWeek, sizeof(IPC_V3::WEEK_SCHEDULE));
						}						
					}
                }
			}
			break;
		case DD_CONFIG_ITEM_MOTION_SCHEDULE:
			{
				chnnIndex = m_pNetDevice->GetBindSensorChannel();
				DD_WEEK_SCHEDULE* pWeekScheduleAll = reinterpret_cast<DD_WEEK_SCHEDULE*>(iter->pData);
				DD_WEEK_SCHEDULE* pWeekSchedule = &(pWeekScheduleAll[chnnIndex]);

				//移动侦测 排程
				{
					TS_CC_CFG_TO_IPC_V2(IPC_V3::NCFG_ITEM_MOTION_SCH_WEEK, sizeof(IPC_V3::WEEK_SCHEDULE), 1);
					IPC_V3::WEEK_SCHEDULE* pIpcSchedule = reinterpret_cast<IPC_V3::WEEK_SCHEDULE*>(cfg_item_v3.pData);

					assert(sizeof(DD_WEEK_SCHEDULE) == sizeof(IPC_V3::WEEK_SCHEDULE));
					memcpy(pIpcSchedule, pWeekSchedule, sizeof(IPC_V3::WEEK_SCHEDULE));
				}
			}
			break;
		case DD_CONFIG_ITEM_MOTION_ALARM_OUT:
			{
				DD_TRIGGER_ALARM_OUT* pMotionAll = reinterpret_cast<DD_TRIGGER_ALARM_OUT*>(iter->pData);
				DD_TRIGGER_ALARM_OUT* pMontion = &(pMotionAll[chnnIndex]);

				//移动侦测触发报警输出
				{
					TS_CC_CFG_TO_IPC_V2(IPC_V3::NCFG_ITEM_MOTION_TO_ALARM_OUT, sizeof(ULONGLONG), 1);
					ULONGLONG* pIpcAlarmOut = reinterpret_cast<ULONGLONG*>(cfg_item_v3.pData);

					*pIpcAlarmOut = pMontion->toAlarmOut;
				}
				//移动侦测能够触发EMAIL
				{
					TS_CC_CFG_TO_IPC_V2(IPC_V3::NCFG_ITEM_MOTION_TO_EMAIL_ENABLE, sizeof(unsigned char), 1);
					unsigned char* pIpcEmailEnable = reinterpret_cast<unsigned char*>(cfg_item_v3.pData);

					*pIpcEmailEnable = pMontion->sendEmail;
				}
			}
			break;
		//case DD_CONFIG_ITEM_MOTION_TO_RECORD:
		//	{
		//		DD_TRIGGER_RECORD* pMontionAll = reinterpret_cast<DD_TRIGGER_RECORD*>(iter->pData);
		//		DD_TRIGGER_RECORD* pMontion = &(pMontionAll[chnnIndex]);

		//		//移动侦测触发抓图
		//		{
		//			TS_CC_CFG_TO_IPC_V2(IPC_V3::NCFG_ITEM_MOTION_TO_SNAP, sizeof(ULONGLONG), 1);
		//			ULONGLONG* pIpcToSnap = reinterpret_cast<ULONGLONG*>(cfg_item_v3.pData);

		//			memcpy(pIpcToSnap, pMontion->snapCH, sizeof(ULONGLONG));
		//		}
		//	}
		//	break;
		case DD_CONFIG_ITEM_RELAY_SETUP:
			{
// 				DD_RELAY_CONFIG* pAlarmOutALL = reinterpret_cast<DD_RELAY_CONFIG*>(iter->pData);
// 				DD_RELAY_CONFIG* pAlarmOut = &(pAlarmOutALL[chnnIndex]);
// 
// 				//输出名称
// 				{
// 					TS_CC_CFG_TO_IPC_V2(IPC_V3::NCFG_ITEM_ALARM_OUT_NAME, sizeof(IPC_V3::NCFG_INFO_SHORT_NAME), 1);
// 					IPC_V3::NCFG_INFO_SHORT_NAME* pIpcAlarmName = reinterpret_cast<IPC_V3::NCFG_INFO_SHORT_NAME*>(cfg_item_v3.pData);
// 
// 					//strncpy(pIpcAlarmName, pAlarmOut->name, sizeof(IPC_V3::NCFG_INFO_SHORT_NAME));
// 				}
// 				//输出保持时间
// 				{
// 					TS_CC_CFG_TO_IPC_V2(IPC_V3::NCFG_ITEM_ALARM_OUT_HOLD_TIME, sizeof(unsigned long), 1);
// 					unsigned long* pIpcHoldTime = reinterpret_cast<unsigned long*>(cfg_item_v3.pData);
// 
// 					*pIpcHoldTime = pAlarmOut->holdTime;
// 				}
			}
			break;
		case DD_CONFIG_ITEM_PTZ_SETUP:
			{
				DD_PTZ_CONFIG* pPtzAll = reinterpret_cast<DD_PTZ_CONFIG*>(iter->pData);
				DD_PTZ_CONFIG* pPtz = &(pPtzAll[chnnIndex]);
				//云台的协议、地址、波特率
				{
					TS_CC_CFG_TO_IPC_V2(IPC_V3::NCFG_ITEM_PTZ_CONFIG, sizeof(IPC_V3::NCFG_INFO_PTZ_CONFIG), 1);
					IPC_V3::NCFG_INFO_PTZ_CONFIG* pIpcConfig = reinterpret_cast<IPC_V3::NCFG_INFO_PTZ_CONFIG*>(cfg_item_v3.pData);

					pIpcConfig->ucBaudrate = pPtz->serial.baudRate;
					pIpcConfig->ucDevaddr = pPtz->address;
					pIpcConfig->ucProtocolid = pPtz->protocol;
				}
			}
			break;
		case DD_CONFIG_ITEM_PTZ_PRESET:
			{
				DD_PTZ_PRESET_CONFIG* pPtzPresetAll = reinterpret_cast<DD_PTZ_PRESET_CONFIG*>(iter->pData);
				DD_PTZ_PRESET_CONFIG* pPtzPreset = &(pPtzPresetAll[chnnIndex]);
	
				//云台参数配置预置点
				{
					int presetNum = 0;
					for (int i=0; i<DD_MAX_PRESET_NUM; i++)
					{
						if (pPtzPreset->enablePreset[i])
						{
							presetNum++;
						}
					}
					TS_CC_CFG_TO_IPC_V2(IPC_V3::NCFG_ITEM_PTZ_INFO_PRESET_INFO_CHBASE+chnnIndex, sizeof(IPC_V3::PRESET_INFO), presetNum);
					IPC_V3::PRESET_INFO* pIpcPreset = reinterpret_cast<IPC_V3::PRESET_INFO*>(cfg_item_v3.pData);

					presetNum = 0;
					for (int i=0; i<DD_MAX_PRESET_NUM; i++)
					{
						if (pPtzPreset->enablePreset[i])
						{
							char nameTemp[36];
							snprintf(nameTemp, 36, "preset %d", i);
							memcpy(pIpcPreset->name, nameTemp, sizeof(char)*(36));
							pIpcPreset->preseIndex = presetNum++;
						}
					}
				}
			}
			break;
		case DD_CONFIG_ITEM_ENCODE_JPEG:
			{
				DD_ENCODE_CONFIG *pEncodeConfigAll = reinterpret_cast<DD_ENCODE_CONFIG *>(iter->pData);
				DD_ENCODE_CONFIG *pEncodeConfig = &pEncodeConfigAll[chnnIndex];
				char temp = IPC_V3::PIC_RESOLUTION_D1;
				switch(pEncodeConfig->resolution)
				{
				case DD_VIDEO_SIZE_CIF:
					temp = IPC_V3::PIC_RESOLUTION_CIF;
					break;
				case DD_VIDEO_SIZE_D1:
					temp = IPC_V3::PIC_RESOLUTION_D1;
					break;
				default:
					temp = IPC_V3::PIC_RESOLUTION_D1;
					break;
				}

				TS_CC_CFG_TO_IPC_V2(IPC_V3::NCFG_ITEM_JPEG_LIVE_CHN,sizeof(char),1);
				char *pJpegRes = reinterpret_cast<char *>(cfg_item_v3.pData);
				*pJpegRes = temp;
			}
			break;
		case DD_CONFIG_ITEM_PTZ_PROTOCOL_INFO:
			{
				assert(iter->dataLen == sizeof(DD_PTZ_PROTOCOL_INFO));
				DD_PTZ_PROTOCOL_INFO* pPtzProtocol = reinterpret_cast<DD_PTZ_PROTOCOL_INFO*>(iter->pData);
				
				//云台支持的协议
				{
					TS_CC_CFG_TO_IPC_V2(IPC_V3::NCFG_PTZ_PROTOCOL_INFO, sizeof(IPC_V3::PTZ_PROTOCOL_INFO), 1);
					IPC_V3::PTZ_PROTOCOL_INFO* pIpcProtocolInfo = reinterpret_cast<IPC_V3::PTZ_PROTOCOL_INFO*>(cfg_item_v3.pData);

					pIpcProtocolInfo->ucprotocolID = pPtzProtocol->protocolID;
					pIpcProtocolInfo->supportproperty = pPtzProtocol->pportproperty;
					memcpy(pIpcProtocolInfo->szProtocolName, pPtzProtocol->ProtocolName, sizeof(char)*64);
				}
			}
			break;
		default:
			break;
		}

		if ((DD_CONFIG_ITEM_PTZ_CRUISE <= iter->itemID)&&(DD_CONFIG_ITEM_PTZ_CRUISE_END > iter->itemID))
		{
				//巡航线信息头，只含有一个通道的
				//因为每个通道可能有多条巡航线，而每条巡航线可能是中不一样的，		
				//NCFG_ITEM_PTZ_INFO_CRUISE_INFO_CHBASE   这个地方不能用0xf850 要用 0x0850,ipc那边是这个值2128
				TS_CC_CFG_TO_IPC_V2(2128,iter->dataLen,1);
				unsigned char * pTempSrc = reinterpret_cast<unsigned char *>(iter->pData);

				unsigned char * pTempDes = cfg_item_v3.pData;

				memcpy(pTempDes,pTempSrc,iter->dataLen);

				break;
		}
	}

	//计算容量
	unsigned long cfg_len = cfg_item_num * sizeof(IPC_V3::NCFG_ITEM_HEAD) + sizeof(IPC_V3::NCFG_BLOCK_HEAD) + cfg_data_len_v2;
	if(cfg_len > m_ipcBuffLen)
	{
		if(m_pIpcBuff)
		{
			delete [] m_pIpcBuff;
		}
		m_ipcBuffLen = cfg_len + 1024;
		m_pIpcBuff = new unsigned char[m_ipcBuffLen];
	}

	return 0;
}

//end
