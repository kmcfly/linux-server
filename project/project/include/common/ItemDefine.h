/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : 袁石维
** Date         : 2009-05-09
** Name         : ItemDefine.h
** Version      : 1.0
** Description  :
** Modify Record:
1:
***********************************************************************/
#ifndef _ITEM_DEFINE_H_
#define _ITEM_DEFINE_H_

//产品名
char PRODUCT_NAME_START		[] = "<PRODUCT_NAME>";
char PRODUCT_NAME			[] = "PRODUCT_NAME";
char PRODUCT_NAME_END		[] = "</PRODUCT_NAME>";

//产品配置组
char PRODUCT_COLLOCATE		[] = "<PRODUCT_COLLOCATE>";
char VIDEO_INPUT_NUM		[] = "VIDEO_INPUT_NUM";
char AUDIO_INPUT_NUM		[] = "AUDIO_INPUT_NUM";
char SENSOR_INPUT_NUM		[] = "SENSOR_INPUT_NUM";
char VIDEO_OUT_NUM			[] = "VIDEO_OUT_NUM";
char SENSOR_OUT_NUM			[] = "SENSOR_OUT_NUM";
char KEY_BOARD_TYPE			[] = "KEY_BOARD_TYPE";
char NET_VIDEO_INPUT_NUM	[] = "NET_VIDEO_INPUT_NUM";
char MAX_PRE_ALARM_TIME		[] = "MAX_PRE_ALARM_TIME";
char PRODUCT_COLLOCATE_END	[] = "</PRODUCT_COLLOCATE>";

//功能配置组
char SUPPORT_COLLOCATE		[] = "<SUPPORT_COLLOCATE>";
char SUPPORT_ESATA			[] = "SUPPORT_ESATA";
char ESATA_INDEX            [] = "ESATA_INDEX";
char DO_DISK_IO_DETECT		[] = "DO_DISK_IO_DETECT";
char AUDIO_ACTION			[] = "AUDIO_ACTION";
char SUPPORT_REDUNDANCY_REC [] = "SUPPORT_REDUNDANCY_REC";
char SUPPORT_MCU			[] = "SUPPORT_MCU";
char USE_MCU_SPOT           [] = "USE_MCU_SPOT";
char SPEEK_TYPE             [] = "SPEEK_TYPE";
char NETWORK_WIFI			[] = "NETWORK_WIFI";
char NETWORK_3G				[] = "NETWORK_3G";
char ORIENTATE_GPS			[] = "ORIENTATE_GPS";
char MAX_CLIENT_NUM			[] = "MAX_CLIENT_NUM";
char VIDEO_OUT_DEVICE		[] = "VIDEO_OUT_DEVICE";
char RESOLUTION_MASK		[] = "RESOLUTION_MASK";
char SUPPORT_COLLOCATE_END	[] = "</SUPPORT_COLLOCATE>";

//帧率参数组
char LOCAL_FRAME_RATE_TABLE	[] = "<LOCAL_FRAME_RATE_TABLE>";
char NET_FRAME_RATE_TABLE	[] = "<NET_FRAME_RATE_TABLE>";
char RATE_SIZE_ALL_CH       [] = "RATE_SIZE_ALL_CH";
char VIDEO_SIZE_MASK		[] = "VIDEO_SIZE_MASK";
char VIDEO_DEFA_SIZE		[] = "VIDEO_DEFA_SIZE";
char VIDEO_ENCODEMODE		[] = "VIDEO_ENCODEMODE";
char RATE_TOTAL_PAL_ON_CIF  [] = "RATE_TOTAL_PAL_ON_CIF";
char RATE_TOTAL_NTSC_ON_CIF [] = "RATE_TOTAL_NTSC_ON_CIF";
char MAX_RATE_TOTAL			[] = "MAX_RATE_TOTAL";
char MAX_RATE_CHNN			[] = "MAX_RATE_CHNN";
char BIT_RATE_VALUE         [] = "BIT_RATE_VALUE";
char BIT_RATE_DEFA_VALUE    [] = "BIT_RATE_DEFA_VALUE";
char FRAME_RATE_TABLE_END	[] = "</FRAME_RATE_TABLE>";

//码流表参数组
char LOCAL_BIT_STREAM_TABLE [] = "<LOCAL_BIT_STREAM_TABLE>";
char NET_BIT_STREAM_TABLE	[] = "<NET_BIT_STREAM_TABLE>";
char BIT_STREAM_LOWEST		[] = "BIT_STREAM_LOWEST";
char BIT_STREAM_LOWER		[] = "BIT_STREAM_LOWER";
char BIT_STREAM_LOW			[] = "BIT_STREAM_LOW";
char BIT_STREAM_MEDIUM		[] = "BIT_STREAM_MEDIUM";
char BIT_STREAM_HIGH		[] = "BIT_STREAM_HIGH";
char BIT_STREAM_HIGHEST		[] = "BIT_STREAM_HIGHEST";
char BIT_STREAM_TABLE_END	[] = "</BIT_STREAM_TABLE>";

//图像参数组
char IMAGE_PARAM_TABLE		[] = "<IMAGE_PARAM_TABLE>";
char IMAGE_PARAM_PAL		[] = "IMAGE_PARAM_PAL";
char IMAGE_PARAM_NTSC		[] = "IMAGE_PARAM_NTSC";
char IMAGE_PARAM_TABLE_END	[] = "</IMAGE_PARAM_TABLE>";
#endif //_ITEM_DEFINE_H_
