/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : 袁石维
** Date         : 2009-05-08
** Name         : Product.h
** Version      : 1.0
** Description  :
** Modify Record:
1:
***********************************************************************/
#ifndef _PRODUCT_H
#define _PRODUCT_H

#include <iostream>
#include <fstream>
#include <string.h>
#include <stdio.h>
using namespace std;

#include "PUB_common.h"
#include "Version.h"
#include "EncodeCheck.h"
#include "CalculateSubEncode.h"
extern unsigned long    MAX_RECORD_FILE_LEN_M;
extern VIDEO_FORMAT	g_video_format;
///////////////////////////////////////////////////////////////////////////////////////////
const unsigned char		MAX_CHNN_NUM = 64;
const unsigned char		MAX_GROUP_ITEM_COUNT = 24;

const unsigned long  	USER_NAME_LEN				= 32; //
const unsigned long  	PASSWORD_LEN				= 32;//
const unsigned long		MAX_PTZ_CH					= 64;
const unsigned long  	MAX_PRESET_NUM				= 128;//
const unsigned long  	MAX_CRUISE_NUM				= 8;//
const unsigned long  	MAX_CRUISE_PRESET_NUM		= 16;
const unsigned long		MAX_TRACK_NUM				= 4;
const unsigned long  	MAX_USER_NUM				= 64;//
const unsigned long  	MAX_HOLIDAY_NUM				= 30;//
const unsigned long		MAX_EMAIL_NUM				= 16;
const unsigned long		MAX_FTP_NUM					= 8;
const unsigned long		MAX_FILTER_IP_NUM			= 30;//
const unsigned long		MAX_FILTER_MAC_NUM			= 30;//
const unsigned long		MAX_MOTION_AREA_LEN			= (720/8)*(576/8);
const unsigned long		MAX_WIRELESS_KEY_LEN		= 256/8;

const unsigned long		MAX_PLAYBACK_NUM			= 1;		//同时回放的通道数目
const unsigned long		MAX_DEVICE_NAME_LEN			= 36 -4;	//设备名称长度限定,存储空间36个字节
const unsigned long		MAX_TIME_SERVER_LEN			= 132-4;	//网络时间同步服务器地址最大长度, 存储空间132个字节
const unsigned long		MAX_UPNP_FRIENDLY_NAME_LEN	= 132-4;	//UPNP设备显示的名字最大长度, 存储空间132个字节

#if defined(__CHIPGM__)
const unsigned long		MAX_CAMERA_NAME_LEN			= 32-4; 		//通道名称最大长度,存储空间36个字节
#else
const unsigned long             MAX_CAMERA_NAME_LEN                     = 36-4;
#endif

const unsigned long		MAX_SENSOR_IN_NAME_LEN		= 36-4;		//传感器名称最大长度,存储空间36个字节
const unsigned long		MAX_ALARM_OUT_NAME_LEN		= 36-4;		//继电器输出名称最大长度, 存储空间36个字节
const unsigned long		MAX_PPPOE_USER_NAME_LEN		= 132-4;	//PPPoE用户名最大长度，存储空间132个字节
const unsigned long		MAX_PPPOE_PASSWORD_LEN		= 36-4;		//PPPoE密码最大长度, 存储空间36个字节
const unsigned long		MAX_DDNS_USER_NAME_LEN		= 36-4;		//DDNS用户名最大长度, 存储空间36个字节
const unsigned long		MAX_DDNS_PASSWORD_LEN		= 36-4;		//DDNS密码最大长度, 存储空间36个字节
const unsigned long		MAX_DDNS_DOMAIN_LEN			= 132-4;	//DDNS域名名称最大长度, 存储空间132个字节
const unsigned long		MAX_WIRELESS_SSID_LEN		= 36-4;		//无线SSID最大长度, 存储空间36个字节

const unsigned long		MAX_BUFFER_LEN				= 512*1024;

extern unsigned long	MAX_VIDEO_FRAME_LEN;

const unsigned long		MAX_SPEED_VALUE_LEN			= 12;
const unsigned char		MAX_WIFI_CFG_NUM			= 5;		//数组的最大维数
const unsigned char		MAX_MAIL_RECV_NUM			= 3;		//邮件接收人的最大数目
const unsigned char     ALARM_EMAIL_MAX_SNAP_IMAGE_COUNT = 3;   //邮件中图片附件最大张数
const unsigned char     MAX_BAN_ALLOW_LIST_NUM		= 20;		//黑白名单最大数目
const unsigned long		MAX_DAILY_SNAP_NUM			= 100;		//日常抓图的最多份数

//const std::string       g_BitRateStr[]   = {"64 Kbs","128 Kbs","256 Kbs","512 Kbs","768 Kbs","1 Mbs", "2 Mbs"};
//const unsigned long     g_MajorBitRateValue[] = {256, 384, 512, 768, 1024, 1280, 1538};//码流可以选择的范围。单位kbs
//const unsigned long     g_MajorBitRateValue[] = {64, 128, 256, 512, 768, 1024, 2048};//码流可以选择的范围。单位kbs

//((brightness<<24) + (contrast<<16) + (saturation<<8) + hue)
#if defined(__CHIP3520__) || defined(__CHIP3515__)
#ifdef __SPECIAL_COLOR   //2316se_s采用的色彩与其他同用海思芯片的产品不一致
const unsigned long DEFAULT_COLOR_NTSC_25838 = ((80 << 24) + (110 << 16) + (140 << 8) + 128);  //C25838使用的色彩， 2316SS旧主板使用C25838
const unsigned long DEFAULT_COLOR_PAL_25838 =  ((80 << 24) + (110 << 16) + (140 << 8) + 128);  
const unsigned long DEFAULT_COLOR_NTSC_1108 = ((102 << 24) + (130 << 16) + (132 << 8) + 128);   //1108使用的色彩，2316SS新主板使用1108
const unsigned long DEFAULT_COLOR_PAL_1108  = ((102 << 24) + (130 << 16) + (132 << 8) + 128);  
#elif defined(__PACKID_13__) //3520 960产品 2616LDS 的色彩值 图像缺省颜色修改为。brightness 119，hue 128，sat 128，con 106
const unsigned long DEFAULT_COLOR_NTSC_2864 = ((100 << 24) + (103 << 16) + (128 << 8) + 128);   //这个是中性的色彩，C25838,2864,2960
const unsigned long DEFAULT_COLOR_PAL_2864  = ((104 << 24) + (98 << 16) + (128 << 8) + 128);
const unsigned long DEFAULT_COLOR_NTSC_1104 = ((102 << 24) + (130 << 16) + (132 << 8) + 128);   //1104 1108使用
const unsigned long DEFAULT_COLOR_PAL_1104  = ((102 << 24) + (130 << 16) + (132 << 8) + 128); 
#else
const unsigned long DEFAULT_COLOR_NTSC_2864 = ((100 << 24) + (100 << 16) + (140 << 8) + 128);   //这个是中性的色彩，C25838,2864
const unsigned long DEFAULT_COLOR_PAL_2864  = ((100 << 24) + (100 << 16) + (140 << 8) + 128);
const unsigned long DEFAULT_COLOR_NTSC_1104 = ((102 << 24) + (130 << 16) + (132 << 8) + 128);   //1104 1108使用
const unsigned long DEFAULT_COLOR_PAL_1104  = ((102 << 24) + (130 << 16) + (132 << 8) + 128); 
#endif
#else
#if defined(__TDFH__)
//亮度、对比度、饱和度、色度的顺序给出；N制  91、111、100、131    P制104、111、100、131
const unsigned long DEFAULT_COLOR_NTSC_2864 = ((100 << 24) + (100 << 16) + (140 << 8) + 128);
const unsigned long DEFAULT_COLOR_PAL_2864  = ((100 << 24) + (100 << 16) + (140 << 8) + 128);
//const unsigned long DEFAULT_COLOR_NTSC_1104 = ((104 << 24) + (111 << 16) + (120 << 8) + 128);
//const unsigned long DEFAULT_COLOR_PAL_1104  = ((104 << 24) + (111 << 16) + (146 << 8) + 128);
//亮度 122   色度 128   饱和度 120   对比度 83
const unsigned long DEFAULT_COLOR_NTSC_1104 = ((122 << 24) + (83 << 16) + (120 << 8) + 128);
const unsigned long DEFAULT_COLOR_PAL_1104  = ((122 << 24) + (83 << 16) + (120 << 8) + 128);
#else
#if defined(__CHIP3531__) || defined(__CHIP3535__)
//brightness 115 contrast 122 saturation 148 hue 128 1108芯片
//p制式 亮度136 灰度 128 饱和度128 对比度107 1918芯片
//N制式 亮度120 灰度 128 饱和度128 对比度118 1918芯片
//3531     PAL  亮度132  灰度128  饱和度128 对比度120
//3531     NTSC 亮度128  灰度128  饱和度148 对比度120

const unsigned long DEFAULT_COLOR_NTSC_2864 = ((128 << 24) + (120 << 16) + (148 << 8) + 128);
const unsigned long DEFAULT_COLOR_PAL_2864  = ((132 << 24) + (120 << 16) + (128 << 8) + 128);
//1104 1108
//				
//3531     PAL        亮度128 灰度128    饱和度96  对比度120
//3531     NTSC       亮度120 灰度128    饱和度96  对比度120
const unsigned long DEFAULT_COLOR_NTSC_1104 = ((120 << 24) + (120 << 16) + (96 << 8) + 128);
const unsigned long DEFAULT_COLOR_PAL_1104  = ((128 << 24) + (120 << 16) + (96 << 8) + 128);
#else
#if defined(__CHIP3520A__) || defined(__CHIP3520D__)
//亮度115 色度128 饱和度90 对比度102
//亮度108 色度130 饱和度148 对比度130
//3520a    PAL        亮度132  色度130  饱和度148  对比度120
//3520a    NTSC       亮度128  色度130  饱和度148  对比度120
#if defined(__TW01_RILI__)
//TW01日立版本，色彩默认值为明度108 ，色度130，饱和度148 ，对比度130
const unsigned long DEFAULT_COLOR_NTSC_2864 = ((108 << 24) + (130 << 16) + (148 << 8) + 130);
#else
const unsigned long DEFAULT_COLOR_NTSC_2864 = ((128 << 24) + (120 << 16) + (148 << 8) + 130);
#endif
const unsigned long DEFAULT_COLOR_PAL_2864  = ((132 << 24) + (120 << 16) + (148 << 8) + 130);
//3520a    PAL        亮度128  色度130  饱和度120 对比度120
//3520a    NTSC       亮度120  色度130  饱和度88  对比度120
#if defined(__TW01_RILI__)
//TW01日立版本，色彩默认值为明度108 ，色度130，饱和度148 ，对比度130
const unsigned long DEFAULT_COLOR_NTSC_1104 = ((108 << 24) + (130 << 16) + (148 << 8) + 130);
#else
const unsigned long DEFAULT_COLOR_NTSC_1104 = ((120 << 24) + (120 << 16) + (88 << 8) + 130);
#endif
const unsigned long DEFAULT_COLOR_PAL_1104  = ((128 << 24) + (120 << 16) + (120 << 8) + 130);



#else
#if defined(__CHIP3521__)
//亮度115 色度128 饱和度90 对比度102
//亮度108 色度130 饱和度148 对比度130
//3521     PAL        亮度128  色度130  饱和度120  对比度120
//3521     NTSC       亮度128  色度130  饱和度148  对比度120
const unsigned long DEFAULT_COLOR_NTSC_2864 = ((128 << 24) + (120 << 16) + (148 << 8) + 130);
const unsigned long DEFAULT_COLOR_PAL_2864  = ((128 << 24) + (120 << 16) + (120 << 8) + 130);
//3521     PAL        亮度128  色度130  饱和度120  对比度120
//3521     NTSC       亮度120  色度130  饱和度88   对比度120
const unsigned long DEFAULT_COLOR_NTSC_1104 = ((120 << 24) + (120 << 16) + (88 << 8) + 130);
const unsigned long DEFAULT_COLOR_PAL_1104  = ((128 << 24) + (120 << 16) + (120 << 8) + 130);
#else
#if defined(__CHIPTI__)
const unsigned long DEFAULT_COLOR_NTSC_2864 = ((140 << 24) + (110 << 16) + (140 << 8) + 131);
const unsigned long DEFAULT_COLOR_PAL_2864  = ((140 << 24) + (100 << 16) + (140 << 8) + 130);
const unsigned long DEFAULT_COLOR_NTSC_1104 = ((140 << 24) + (110 << 16) + (140 << 8) + 131); 
const unsigned long DEFAULT_COLOR_PAL_1104  = ((140 << 24) + (100 << 16) + (140 << 8) + 130);
#else
const unsigned long DEFAULT_COLOR_NTSC_2864 = ((100 << 24) + (100 << 16) + (140 << 8) + 128);
const unsigned long DEFAULT_COLOR_PAL_2864  = ((100 << 24) + (100 << 16) + (140 << 8) + 128);
const unsigned long DEFAULT_COLOR_NTSC_1104 = ((102 << 24) + (130 << 16) + (132 << 8) + 128); 
const unsigned long DEFAULT_COLOR_PAL_1104  = ((102 << 24) + (130 << 16) + (132 << 8) + 128);
#endif
#endif
#endif
#endif
#endif
#endif

#if defined(__TDFH__)
//亮度、对比度、饱和度、色度的
const unsigned long DEFAULT_HDMI_DEV_COLOR = ((137<<24) + (128<<16) + (128<<8) + 139);
const unsigned long DEFAULT_VGA_DEV_COLOR  = ((137<<24) + (128<<16) + (128<<8) + 139);
const unsigned long DEFAULT_CVBS_DEV_COLOR = ((160<<24) + (128<<16) + (126<<8) + 139);
#elif defined(__CHIP3531__) || defined(__CHIP3535__)
//br:123 hue:104 sat:116 con:120
//亮度 96 hue 128 sat 122 con 148
#ifdef __DVR_ULTIMATE__
const unsigned long DEFAULT_HDMI_DEV_COLOR = ((128<<24) + (138<<16) + (128<<8) + 128);
const unsigned long DEFAULT_VGA_DEV_COLOR  = ((128<<24) + (138<<16) + (128<<8) + 128);
const unsigned long DEFAULT_CVBS_DEV_COLOR = ((128<<24) + (138<<16) + (128<<8) + 128);
#else
const unsigned long DEFAULT_HDMI_DEV_COLOR = ((96<<24) + (148<<16) + (122<<8) + 128);
const unsigned long DEFAULT_VGA_DEV_COLOR  = ((96<<24) + (148<<16) + (122<<8) + 128);
const unsigned long DEFAULT_CVBS_DEV_COLOR = ((96<<24) + (148<<16) + (122<<8) + 128);
#endif
#elif defined(__PACKID_9__) || defined(__PACKID_14__)
//亮度 160 对比度 139  饱和度 98  灰度 100
const unsigned long DEFAULT_HDMI_DEV_COLOR = ((160<<24) + (139<<16) + (98<<8) + 100);
const unsigned long DEFAULT_VGA_DEV_COLOR  = ((160<<24) + (139<<16) + (98<<8) + 100);
const unsigned long DEFAULT_CVBS_DEV_COLOR = ((160<<24) + (142<<16) + (126<<8) + 128);
#elif defined(__CHIP3520A__) || defined(__CHIP3520D__)
//亮度 128 对比度 128  饱和度 128  灰度 128
#ifdef __DVR_ULTIMATE__
const unsigned long DEFAULT_HDMI_DEV_COLOR = ((128<<24) + (138<<16) + (128<<8) + 128);
const unsigned long DEFAULT_VGA_DEV_COLOR  = ((128<<24) + (138<<16) + (128<<8) + 128);
const unsigned long DEFAULT_CVBS_DEV_COLOR = ((128<<24) + (138<<16) + (128<<8) + 128);
#else
const unsigned long DEFAULT_HDMI_DEV_COLOR = ((128<<24) + (128<<16) + (128<<8) + 128);
const unsigned long DEFAULT_VGA_DEV_COLOR  = ((128<<24) + (128<<16) + (128<<8) + 128);
const unsigned long DEFAULT_CVBS_DEV_COLOR = ((128<<24) + (128<<16) + (128<<8) + 128);
#endif
#elif defined(__CHIPTI__)
//亮度 128 对比度 128  饱和度 128  灰度 128
const unsigned long DEFAULT_HDMI_DEV_COLOR = ((128<<24) + (128<<16) + (128<<8) + 128);
const unsigned long DEFAULT_VGA_DEV_COLOR  = ((128<<24) + (128<<16) + (128<<8) + 128);
const unsigned long DEFAULT_CVBS_DEV_COLOR = ((128<<24) + (128<<16) + (128<<8) + 128);
#else
#ifdef __DVR_ULTIMATE__
const unsigned long DEFAULT_HDMI_DEV_COLOR = ((128<<24) + (138<<16) + (128<<8) + 128);
const unsigned long DEFAULT_VGA_DEV_COLOR  = ((128<<24) + (138<<16) + (128<<8) + 128);
const unsigned long DEFAULT_CVBS_DEV_COLOR = ((128<<24) + (138<<16) + (128<<8) + 128);
#else
const unsigned long DEFAULT_HDMI_DEV_COLOR = ((160<<24) + (139<<16) + (142<<8) + 126);
const unsigned long DEFAULT_VGA_DEV_COLOR  = ((160<<24) + (139<<16) + (142<<8) + 126);
const unsigned long DEFAULT_CVBS_DEV_COLOR = ((160<<24) + (142<<16) + (126<<8) + 128);
#endif
#endif


const unsigned long SNAP_PIC_MAX_NUM  = 3; 
extern const unsigned long g_resolutionNum;
typedef struct _frame_rate
{
	unsigned long  rate_size_all_ch;

	unsigned short rate_total_pal_on_cif;
	unsigned short rate_chnn_pal_on_cif;

	unsigned short rate_total_ntsc_on_cif;
	unsigned short rate_chnn_ntsc_on_cif;

	unsigned long max_rate_total;		//总共最大码流（KPS）
	unsigned long max_rate_chnn;		//
}FRAME_RATE;

typedef struct _ipc_frame_rate_
{
	unsigned long	max_rate_total_on_vga;	//PAL制下支持的所有通道的D1总帧率
	unsigned long	max_rate_chnn_on_vga;	//PAL制下支持的单通道最大D1帧率
	unsigned long	max_rate_chnn;			//PAL制下支持的单通道最大帧率
}IPC_FRAME_RATE;

typedef struct _img_bit_stream
{
	unsigned long	lowest;
	unsigned long	lower;
	unsigned long	low;
	unsigned long	medium;
	unsigned long	high;
	unsigned long	highest;
}IMG_BIT_STREAM;

typedef struct _image_param
{
	unsigned short cif_width;
	unsigned short cif_height;
	unsigned short hd1_width;
	unsigned short hd1_height;
	unsigned short d1_width;
	unsigned short d1_height;
}IMAGE_PARAM;

typedef struct _product_info
{
	//可变参数
	unsigned char	localVideoInputNum;		//本地视频输入通道数目
	unsigned char	audioInputNum;			//音频输入通道数目
	unsigned char	sensorInputNum;			//传感器输入通道数目
	unsigned char	relayOutputNum;			//继电器输出数目
	unsigned long   displayResolutionMask;  //监视器可选择的分辨率

	unsigned char	videoOuputNum;			//视频输出数目（及支持回放最大通道数）
	unsigned char	netVideoOutputNum;		//网络回放最大通道数目
	unsigned char	netVideoInputNum;		//数字信号接入通道数目
	unsigned char	IVSNum;					//智能分析通道数目

	unsigned char	presetNumOneCH;			//每个通道预置点数目
	unsigned char	cruiseNumOneCH;			//每个通道巡航线数目
	unsigned char	presetNumOneCruise;		//每个巡航线的预置点数目
	unsigned char	trackNumOneCH;			//每个通道轨迹数目

	unsigned char	userNum;				//用户数目 
	unsigned char	netClientNum;			//最多客户端数目
	unsigned char	netFirstStreamNum;		//主码流传输的通道最大数目，即同时可以有几个客户端查看主码流
	unsigned char	deviceType;				//设备类型

	unsigned char	doblueStream;			//是否有提供双码流
	unsigned char	audioStream;			//是否有提供音频流
	unsigned char	talkAudio;				//是否有对讲功能: 1表示有对讲功能;0表示没有
	unsigned char	bPasswordCheck;			//操作是否要输入密码

	unsigned char	defBrightness;			//缺省亮度
	unsigned char	defContrast;			//缺省对比度
	unsigned char	defSaturation;			//缺省饱和度
	unsigned char	defHue;					//缺省色调

	unsigned short	videoInputNum;			//视频输入通道数目（本地加网络）
	unsigned short  deviceID;				//设备ID号
    unsigned long   videoFormat;            //系统当前制式

	//假如是FUNC_REMOTE_UPGRADE对应的功能，那么第FUNC_REMOTE_UPGRADE个比特为1，否则为零。
	unsigned long	function[8];			//功能描述

	unsigned long	deviceIP;				//设备网络地址
	unsigned char	deviceMAC[8];			//设备物理地址

	unsigned long	buildDate;				//创建日期:year<<16 + month<<8 + mday
	unsigned long	buildTime;				//创建时间:hour<<16 + min<<8 + sec

	char			deviceName[36];			//设备名称

	char	firmwareVersion[36];			//固件版本
	char	kernelVersion[64];				//内核版本
	char	hardwareVersion[36];			//硬件版本
	char	MCUVersion[36];					//单片机版本
}PRODUCT_INFO;

typedef struct __channel_rec_param__
{
	unsigned long	supVS;	//可支持分辨率
	unsigned long	dfVS;	//默认分辨率
}CHANNEL_RECP;

typedef struct __VSSupBrr__
{
	unsigned long resolution;	//对应的分辨率
	unsigned long vsforbrr[20];//每个分辨率支持哪些码流
}VS_SUPBRR;



class CProduct
{
public:
	~CProduct();
	static CProduct * Instance();
	bool Initial(const char * pFilePath, unsigned long productID, unsigned long subProductID, unsigned long sdkProductID=0);
	void SetProductSubIDInHardware(unsigned long productSubIDInHardware);
	void Quit();
	void ShowProduct();
	int GetDeviceType()
	{
		if (0 == m_netVideoInputNum)
		{
			return 0;
		}
		else if (0 == m_localVideoInputNum)
		{
			return 1;
		}
		else
		{
			return 2;
		}
	}
	
	void GetProductIDName(std::string & productName);

	void SetDeviceInfo(const char *pDeviceName, unsigned short deviceID, unsigned long videoFormat);
	void SetDeviceID(unsigned short deviceID){m_deviceID = deviceID;}
	void SetDeviceName(const char *pDeviceName){m_productName = pDeviceName;}
	
	void ChangeDeviceID(unsigned short deviceID) {m_deviceID = deviceID;};
	void ChangeDeviceName(const char *pDeviceName) {m_productName = pDeviceName;};
	void ChangeKeyBoardType(KEYBOARD_TYPE type, char *pName);
	void ChangePreRecordTime(unsigned long timeS) {m_preAlarmTime = timeS;};
	void GetProductInfo(PRODUCT_INFO & info, char *pszMAC, char *pszHardWare, char *pszKernel, char *pszMCU);
	inline unsigned char GetSpeekType() { return m_speekType;};
	void SetFuncCustomInfo(char* pInfo, int offset, int len);
	//////////////////////////////////////////////////////////////////////////
	inline PRODUCT_TYPE ProductType(){return m_productType;};
	inline unsigned long ProductSubID(){return m_productSubID;};	
    void GetHardwarePreVersion(char *pPreVersion, int len);
	const char * GetFirmwareVersion(char * pFV, int len);
	inline unsigned long GetProductSubIDInHardware() {return m_productSubIDInHardVerion;};
	//////////////////////////////////////////////////////////////////////////
	inline unsigned char VideoInputNum(){return (m_localVideoInputNum + m_netVideoInputNum);};
	inline unsigned char VideoOutputNum(){return m_videoOutputNum;};
	inline unsigned char NetVideoOutPutNum(){return m_netVideoOutputNum;};
	inline unsigned char AudioInputNum(){return m_audioInputNum;};
	inline unsigned char SensorInputNum(){return m_sensorInputNum;};
	inline unsigned char SensorOutputNum(){return m_sensorOutputNum;};
	inline unsigned char NetVideoInputNum(){return m_netVideoInputNum;};
	inline unsigned char PoeInputNum(){return m_poeInputNum;}
	inline unsigned char LocalVideoInputNum(){return m_localVideoInputNum;};
	inline unsigned long MaxPreAlarmTime(){return m_preAlarmTime;};
	inline unsigned long GetPreRecordType(){return m_preRecordType;};
	inline unsigned char MaxFFSpeed(){return m_maxFFSpeed;};	
	//////////////////////////////////////////////////////////////////////////
	inline bool SupportRedunRec() {return m_bSupportRedunRec;};
	inline bool SupportESATA(){return m_bSupportESATA;};
	inline unsigned long GetESATAIndex(){return m_eSataIndex;};
	bool        GetUseEsata();
	void        SetUseEsata(bool bUse);
	inline unsigned char GetPciToEsataNum(){return m_pciToEsataNum;};
	unsigned char GetPciToEsataIndex(unsigned char index);
	inline bool SupportUsbToEsata() { return m_bHaveUsbToEsata;};
	void GetUsbToEsataPath(char *pPath);
	inline bool DoDiskIODetect(){return m_bDoDiskIODetect;};
	inline bool AudioAction(){return m_bAudioAction;};
	inline bool SupportMCU(){return m_bSupportMCU;};
	inline bool UseMCUSpot(){return m_bUseMCUSpot;};
	inline bool HaveSpot(){return m_bHaveSpot;};
	inline bool NetworkWiFi(){return m_bNetworkWiFi;};
	inline bool OrientateGPS(){return m_bOrientateGPS;};
	inline bool AutoReport(){return m_bAutoReport;};

	inline unsigned char LocalKeyboardType(){return m_localKeyboardType;};
	inline unsigned char External485KeyboardType(){return m_External485KeyboardType;};
	inline unsigned char MaxNetClientNum(){return m_maxNetClientNum;};	
	inline unsigned long VideoOutDeviceMask(){return m_videoOutDevice;};
	inline unsigned long DisplayResolutionMask(){return m_displayResolutionMask;};
	inline unsigned long DefDispResolutionMask(){return m_defDispResolutionMask;};

	inline unsigned long SnapJpegRate() {return m_snapJpegRate;};
	inline unsigned long SnapJpegRateTime() {return m_snapJpegRateTime;};
	unsigned long SnapTimeIntervalCount();
	unsigned long SnapTimeIntervalValue(int index);
	unsigned long SnapDefaTimeIntervalValue() {return m_SnapDefaTimeIntervalValue;};
	unsigned long SnapQuiltyLevelNum();
	unsigned long SnapQuiltyLevelValue(int index);
	unsigned long SnapDefQuiltyValue();
	unsigned long SnapPictureSizeMask()
	{
#ifdef __DVR_ULTIMATE__
		
#if defined(__CHIP3521__) || defined(__CHIP3520A__) || defined(__CHIP3520D__)
		return DVRVIDEO_SIZE_CIF;
#else
		return ( DVRVIDEO_SIZE_CIF | DVRVIDEO_SIZE_D1);
#endif
		
#elif defined(__PACKID_14__) || defined(__PACKID_27__)
#if defined(__SUPPORT_HONGDI__)
		return (DVRVIDEO_SIZE_HD1080 | DVRVIDEO_SIZE_CIF | DVRVIDEO_SIZE_D1);
#else
		return ( DVRVIDEO_SIZE_CIF | DVRVIDEO_SIZE_D1);
#endif
#elif defined(__CHIP3531__)//3531全部支持d1和cif抓图
		return ( DVRVIDEO_SIZE_CIF | DVRVIDEO_SIZE_D1);
#else
		return DVRVIDEO_SIZE_CIF;
#endif
	};
	unsigned long SnapDefPictureSize()
	{
#if (defined(__PACKID_14__) || defined(__PACKID_27__)) && defined(__SUPPORT_HONGDI__)
		return DVRVIDEO_SIZE_HD1080;
#else
		return DVRVIDEO_SIZE_CIF;
#endif
	};
	unsigned long GetSplitMode(unsigned char chnnNum);
	inline bool SupportDoubleStream(){return m_bSupportDoblueStream;};
	inline unsigned char GetChnnSendFirstStreamNum(){return m_chnnSendFirstStreamMaxNum;};
	inline bool SupportLimitFirstStreamSendNum() {return m_bLimitFirstStreamSendNum;};
	inline unsigned char GetDVRSendFirstStreamNum(){return m_sendFirstStreamMaxNum;};	
	inline unsigned long GetDVRMaxStreamIDNum(){return m_maxStreamIDNumForPlayOrBackup;};
	inline bool SupportSnapPic(){return m_bSupportSnapPic;};

	inline unsigned long GetSecondStreamCopies() {return m_netSendCopies;};
	inline unsigned long GetNetLimitMaxRate() {return m_netlimitSendCopies;};
	inline bool IsLimitSecondStreamForFirstStreamNum() {return m_bLimitSecondStreamForFirstStreamNum;};
	inline unsigned long LimitSecondStreamTypeForFirstStreamNum() {return m_limitTypeForFirstStreamNum;};
	inline unsigned long FirstStreamNumForLimitSecondStream(){return m_firstStreamNumForLimitSecondStream;};
	inline unsigned long LimitSecondRateForFirstStreamNum(){return m_netLimitRatForFirstStreamNum;};
	inline unsigned long LimitSecondAllRateForFirstStreamNum(){return m_netLimitAllRatForFirstStreamNum;};
	inline unsigned long FirstStreamLimitBitRate(){return m_firstStreamLimitBitRate;}



	//关于本地（网络）帧率 ,码流和分辨率
	inline unsigned long LocalRateSizeAllCH(){return m_local_rate.rate_size_all_ch;};
	inline unsigned long LocalVideoSizeMask(){return m_localVideoSize;};
	unsigned long LocalDefaVideoSize();
	inline unsigned long LocalTotalRatePal() {return m_local_rate.rate_total_pal_on_cif;};
	inline unsigned long LocalTotalRateNtsc() {return m_local_rate.rate_total_ntsc_on_cif;};
	inline unsigned long LocalMaxChannelRatePal() {return m_local_rate.rate_chnn_pal_on_cif;};
	inline unsigned long LocalMaxChannelRateNtsc() {return m_local_rate.rate_chnn_ntsc_on_cif;};
	unsigned long LocalDefaChnnRateNumPal();
	unsigned long LocalDefaChnnRateNumNtsc();
	unsigned long LocalDefChnnRateNum();

	unsigned long        LocalQuiltyLevelNum();
	unsigned long        LocalQuiltyLevelValue(int index);
	unsigned long        LocalDefQuiltyValue();
	inline unsigned long LocalVideoEncodeModeMask() {return m_localVideoEncodeModeMask;}
	inline unsigned long LocalDefauVideoEncodeMode()
	{
#if defined(__TW01_RILI__)
		return VIDEO_ENCODE_MODE_VBR;
#else
		return m_localDefaVideoEncodeMode;
#endif
	}
	unsigned long        LocalBitRateCount();
	unsigned long        LocalBitRateValue(int index);
	unsigned long        LocalDefaBitRateValue();
	inline unsigned long NetRateSizeAllCH() {return m_net_rate.rate_size_all_ch;};
	inline unsigned long NetVideoSizeMask(){return m_netVideoSize;};
	inline unsigned long NetDefaVideoSize() {return m_netVideoDefaSize;};
	inline unsigned long NetTotalRatePal() {return m_net_rate.rate_total_pal_on_cif;};
	inline unsigned long NetTotalRateNtsc() {return m_net_rate.rate_total_ntsc_on_cif;};
	inline unsigned long NetMaxChannelRatePal() {return m_net_rate.rate_chnn_pal_on_cif;};
	inline unsigned long NetMaxChannelRateNtsc() {return m_net_rate.rate_chnn_ntsc_on_cif;};
	inline unsigned long NetDefaChnnRateNumPal() 
	{
#if defined(__CUSTOM_IL03__)
		if (6 < m_netDefaChnnRateNumPal)
		{
			return  6;
		}
		else
		{
			return m_netDefaChnnRateNumPal;
		}
#else
		return m_netDefaChnnRateNumPal;
#endif
	}

	inline unsigned long NetDefaChnnRateNumNtsc()
	{
#if defined(__CUSTOM_IL03__)
		if (7 < m_netDefaChnnRateNumNtsc)
		{
			return 7;
		}
		else
		{
			return m_netDefaChnnRateNumNtsc;
		}
#else
		return m_netDefaChnnRateNumNtsc;
#endif
	}
	unsigned long        NetQuiltyLevelNum();
	unsigned long        NetQuiltyLevelValue(int index);
	unsigned long        NetDefQuiltyValue();
	inline unsigned long NetVideoEncodeModeMask() {return m_netVideoEncodeModeMask;}
	inline unsigned long NetDefauVideoEncodeMode()
	{
#if defined(__CUSTOM_IL03__)
		return  VIDEO_ENCODE_MODE_VBR;
#else
		return m_netDefaVideoEncodeMode;
#endif
	};
	unsigned long        NetBitRateCount();
	unsigned long        NetBitRateValue(int index);
	unsigned long        NetDefaBitRateValue();
	inline unsigned long IPCDefVideoSize() {return m_ipcVideoDefaSize;}
	inline unsigned long IPCDefaChnnRate() 
	{
		return m_ipcDefaChnnRate;
	}
	unsigned long        IPCDefaBitRateValue(){return m_ipcDefaBitRate;}
	inline unsigned long IPCDefauVideoEncodeMode() {return m_ipcDefaVideoEncodeMode;}
	inline unsigned long IPCNetDefVideoSize() {return m_ipcNetVideoDefaSize;}
	inline unsigned long IPCNetDefaChnnRate() {return m_ipcNetDefaChnnRate;}
	unsigned long        IPCNetDefaBitRateValue(){return m_ipcNetDefaBitRate;}
	inline unsigned long IPCNetDefauVideoEncodeMode() {return m_ipcNetDefaVideoEncodeMode;}
	inline unsigned long IPCMaxChnnRate()
	{
		return m_ipcFrameRate.max_rate_chnn;
	}
	inline unsigned long IPCMaxChnnRateOnVGA()
	{
		return m_ipcFrameRate.max_rate_chnn_on_vga;
	}
	
	inline unsigned long IPCMaxTotalRateOnVGA()
	{
		return m_ipcFrameRate.max_rate_total_on_vga;
	}
	inline bool			 IPCSupportDisplayMinor(){return m_bSupportDisplayMinor;}
	//inline void LocalBitStream(IMG_BIT_STREAM & bitStream){bitStream = m_local_BitStream;};
	inline void NetworkBitStream(IMG_BIT_STREAM & bitStream){bitStream = m_net_BitStream;};

	inline unsigned long LocalVideoSizeSpecial() {return m_bLocalVideoSizeSpecial;};
	inline unsigned long LocalVideoSizeCIFChnnBits() {return m_localVideoSizeCIFChnnBits;};
	inline unsigned long LocalVideoSizeD1ChnnBits(){return m_localVideoSizeD1ChnnBits;};
	
	inline bool          CanChangLocalVideoInfo() {return m_bCanChangLocalVideoInfo;};
	bool CanChangLocalVideoSize();
	inline unsigned long PBChannelNumForLimitFirstStream(){return m_pbChannelNumForLimitFirstStream;}

	//////////////////////////////////////////////////////////////////////////
	inline IMAGE_PARAM ImageParamPAL(){return m_imageParam_PAL;};
	inline IMAGE_PARAM ImageParamNTSC(){return m_imageParam_NTSC;};
	//////////////////////////////////////////////////////////////////////////

	bool IsHardwareHasS7() const {return m_bHardwareS7;};
	inline bool Is2704HDNewHarWare() {return m_b2704HDNewHardWare;};

	void Chang2316SSSpot();
	unsigned long GetDefualtResolution();
	unsigned long  writeDefaultResolution();//写默认分辨率到flash
	
		//数字信号相关参数接口
	unsigned long *GetDigitChannelSupportVideoSize(){return m_pSupportVideoSize;}
	unsigned long GetDigitMaxTotalBitRate(){return m_ipcMaxTotalBitRate;}
	unsigned long IPCIsLimitByBitRate(){return m_bLimitByBitRate;}

	unsigned long IPCMaxVideoOutNum(){return m_ipcMaxVideoOutNum;}

	unsigned long DefaLiveAudioState(){return m_defaLiveAudioState;}

	//混合dvr相关接口
	VIDEO_SIZE RealVideoSize(VIDEO_SIZE videoSize);
	bool GetChannelRecp(unsigned long ch, CHANNEL_RECP * pChBrr);
	void GetVSforBrr(VS_SUPBRR *pVS)
	{
		assert(pVS);
		if(NULL != m_pvsBrr)
		{
			memcpy(pVS,m_pvsBrr,sizeof(VS_SUPBRR)*g_resolutionNum);
		}
		
	}

	bool IsUseStaticVideoBitRate(){return m_bIPCUseStaticBitRate;}
	bool IsDispD1Device();
	bool CVBS_ChangeParam(unsigned long productID,unsigned long status);
protected:
private:
	CProduct();
	//bool CheckSoftLevel(unsigned long productID);
	bool GetParam(string &str);

	bool GetProductType(ifstream & product);
	bool GetProductCollocate(ifstream & product);
	bool GetSupportCollocate(ifstream & product);
	bool GetFrameTable(ifstream & product, FRAME_RATE &frameRate, 
					   unsigned long &videoSizeMask, unsigned long &videoDefaSize,
					   unsigned long &defaChnnRateNumPal, unsigned long &defaChnnRateNumNtsc,
					   unsigned long &videoEncodeModeMask, unsigned long &defaVideoEncodeMode, 
					   unsigned long &bitRateCount, unsigned long **pBitRateRange, 
					   unsigned long &defBitRateValue, bool bLocal = true);
	bool GetBitStream(ifstream & product, IMG_BIT_STREAM & bitStream);
	bool GetImageParam(ifstream & product);

	bool GetVideoSize(string &str, unsigned long & videoSize);
	bool GetVideoDefaSizeAndRate(string &str, unsigned long &videoSize, unsigned long &videoRate1, unsigned long &videoRate2);
	bool GetVideoEncodeMode(string &str, unsigned long &encodeMode, unsigned long &defaEncodeMode, bool bLocal);
	bool GetVideoDevice(string &str);
	bool GetResolution(string &str);
	bool GetFrameRate(string &str, unsigned short &rate_total, unsigned short &rate_chnn);
    bool GetBitRateRange(string &str, unsigned long &bitRateCount, unsigned long **pBitRateRange);
	bool GetImageParam(string &str, IMAGE_PARAM & imageParam);
	//////////////////////////////////////////////////////////////////////////
	PRODUCT_TYPE	ProductType(string &str);

	void SetHDParam();//设置高清视频参数

	void AdjustParam();
	void SetChannelRecp(unsigned long ch, VIDEO_SIZE supVS, VIDEO_SIZE defVS, CHANNEL_RECP * pChBrr);
	void SetVSFOrBrr(unsigned long productID,unsigned long resolution);//设置每个分辨率对应的码流，目前先用在混合型上

	void SetStaticVideoBitrate(VIDEO_SIZE vs, unsigned long ulBitrate);
	//////////////////////////////////////////////////////////////////////////
	unsigned short	m_deviceID;
	PRODUCT_TYPE	m_productType;
	PRODUCT_TYPE	m_sdkProductType;
	unsigned long   m_productSubID;
	std::string		m_productName;
	unsigned long   m_videoFormat;
	//////////////////////////////////////////////////////////////////////////
	unsigned char	m_localVideoInputNum;
	unsigned char	m_audioInputNum;
	unsigned char	m_sensorInputNum;
	unsigned char	m_videoOutputNum;
	unsigned char	m_netVideoOutputNum;
	unsigned char	m_sensorOutputNum;
	unsigned char	m_localKeyboardType;
	unsigned char	m_netVideoInputNum;
	unsigned char	m_poeInputNum;
	unsigned long   m_preRecordType; //预录像的类型，是内存方式还是硬盘方式
	unsigned long	m_preAlarmTime;
	unsigned char	m_External485KeyboardType;
	unsigned char	m_notuse[3];
	unsigned char   m_maxFFSpeed;             //单通道最大允许几倍逐帧解码。 
	                                          //如果是128，表示产品不受这个约束。
	                                          //就是采用默认，不管几路回放，最大只能做到4倍数逐帧解码。
	unsigned char   m_chnnSendFirstStreamMaxNum; //每个通道发送主码流个数。对每个通道发送主码流的份数做了限制（即：每个通道总共可以发送几个主码流）
	                                             //这里的主码流包括：现场主码流，远程回放码流，远程备份码流.到目前为，这个域为1.

	bool            m_bLimitFirstStreamSendNum; //是否限制对整个DVR发送主码流的份数做限制。通过这个变量可以启动和关闭这个功能
	                                            //这个变量与m_netVideoOutputNum区别是：m_netVideoOutputNum表示客户端支持几路对比回放。	
	                                            //目前只有2508FD和2516FD做限制
	unsigned char   m_sendFirstStreamMaxNum;    //整个DVR发送主码流的份数.这个变量的最大值为m_chnnSendFirstStreamMaxNum*m_localVideoInputNum
	                                            //一般不要小于：m_netVideoOutputNum

	//////////////////////////////////////////////////////////////////////////
	bool            m_bSupportRedunRec;
	bool			m_bSupportESATA;
	unsigned long   m_eSataIndex;       //m_eSataIndex表示第几个sata口做esata。从1开始计数
	unsigned char   m_pciToEsataNum;     //这个变量表示：PCI转的esata个数，到2011-1-8为止，只有2432MD，2424MD使用的是pci转esata
	unsigned char   *m_pPciToEsataIndex;  //PCI转的esata的编号，到2011-1-8为止，编号是9,10。从1开始计数。
	bool            m_bHaveUsbToEsata;       //这个变量表示是否由usb转的esata；
	char            m_szUsbToEsataPath[128]; //这个变量表示：转成esata的usb的路径
	bool			m_bDoDiskIODetect;
	bool			m_bAudioAction;
	bool			m_bSupportMCU;
	bool            m_bHaveSpot;
	bool            m_bUseMCUSpot;     //是否使用单片机的spot。目前只有24XXMD系列产品，使用单片机的spot功能
	unsigned char   m_speekType;    //对讲类型.0-没有对讲。1-有独立的对讲输入。2-用通道0做对讲输入。
	bool			m_bNetworkWiFi;
	bool			m_bNetwork3G;
	bool			m_bOrientateGPS;
	bool			m_bAutoReport;
	unsigned char	m_maxNetClientNum;	
	
	unsigned long	m_videoOutDevice;
	unsigned long	m_displayResolutionMask;
	unsigned long   m_defDispResolutionMask;
	
	unsigned long   m_snapJpegRate;
	unsigned long   m_snapJpegRateTime;
    unsigned long   m_emailSnapTimeInterValNum;
	unsigned long   *m_pEmailSnapTimeInterVal;
	unsigned long   m_SnapDefaTimeIntervalValue;
	bool            m_bSupportDoblueStream;
	bool            m_bSupportSnapPic;
	unsigned long   m_maxStreamIDNumForPlayOrBackup;//DVR最大支持多少个客户端请求的streamID数

	unsigned long   m_netSendCopies;             //最大可以发送子码流的份数。主要用在2516FD，因为2516FD发送的是实时的子码流，而其他厂房
												 //所以对2516FD进行限制，其最大为24；其他产品为不限制，为INVALID_INDEX(type)
	unsigned long   m_netlimitSendCopies;           //在某些情况下子码流的最大帧率限制多少。2516FD在备份时，消耗的性能不较大，
												 //而这时在发送实时的子码流,机器就无法承担，
												 //所以对2516FD进行限制，限制到5；其他产品为不限制，为INVALID_INDEX(type)

	bool            m_bLimitSecondStreamForFirstStreamNum;
	unsigned long   m_firstStreamNumForLimitSecondStream;
	unsigned long   m_limitTypeForFirstStreamNum;
	unsigned long   m_netLimitRatForFirstStreamNum;
	unsigned long   m_netLimitAllRatForFirstStreamNum;
	unsigned long	m_firstStreamLimitBitRate;


	//关于本地（网络）帧率 ,码流和分辨率
	FRAME_RATE		m_local_rate;	
	unsigned long   m_localDefaChnnRateNumPal;
	unsigned long   m_localDefaChnnRateNumNtsc;
	unsigned long   m_localDefaChnnRateNumPal960H;
	unsigned long   m_localDefaChnnRateNumNtsc960H;
	unsigned long	m_localVideoSize;
	unsigned long   m_localVideoDefaSize;
	unsigned long   m_localVideoEncodeModeMask;
	unsigned long   m_localDefaVideoEncodeMode;
	unsigned long   m_localBitRateCount;
	unsigned long   *m_pLocalBitRateRange;
	CHANNEL_RECP	*m_pChRECP;
	unsigned long   m_localDefaBitRateValue;
	FRAME_RATE		m_net_rate;
	unsigned long   m_netDefaChnnRateNumPal;
	unsigned long   m_netDefaChnnRateNumNtsc;
	unsigned long	m_netVideoSize;
	unsigned long   m_netVideoDefaSize;
	unsigned long   m_netVideoEncodeModeMask;
	unsigned long   m_netDefaVideoEncodeMode;
	unsigned long   m_netBitRateCount;
	unsigned long   *m_pNetBitRateRange;
	unsigned long   m_netDefaBitRateValue;

	unsigned long   m_bLocalVideoSizeSpecial;     ////是否支持特殊的录像配置
	ULONGLONG       m_localVideoSizeCIFChnnBits;  //可以配置成CIF的通道掩码
	ULONGLONG       m_localVideoSizeD1ChnnBits;    //可以配置成D1的通道掩码

	bool            m_bCanChangLocalVideoInfo;
	//////////////////////////////////////////////////////////////////////////	
	IMG_BIT_STREAM	m_local_BitStream;
	IMG_BIT_STREAM	m_net_BitStream;
	//////////////////////////////////////////////////////////////////////////
	IMAGE_PARAM		m_imageParam_PAL;
	IMAGE_PARAM		m_imageParam_NTSC; 

	//
	char           m_szKeyBoardName[16];
	char		   m_szExternal485KeyBoardName[16];

	unsigned char  m_funcCustomInfo[32];
	bool			m_bHardwareS7;				//是否硬件版本中含有"S7“或者”s7“
	//
	unsigned long  m_productSubIDInHardVerion;  //显示在硬件版本号的机器子版本号，一般是等于m_productSubID

	bool m_b2704HDNewHardWare; //是否2708XDF（也就是2704HD）的新主板

	//数字信号相关的参数
	unsigned long	m_ipcVideoDefaSize;
	unsigned long	m_ipcDefaChnnRate;	//PAL制下的值
	unsigned long	m_ipcDefaBitRate;
	bool m_bIPCUseStaticBitRate;
	unsigned long	m_ipcDefaVideoEncodeMode;
	unsigned long	m_ipcNetVideoDefaSize;
	unsigned long	m_ipcNetDefaChnnRate;	//PAL制下的值
	unsigned long	m_ipcNetDefaBitRate;
	unsigned long	m_ipcNetDefaVideoEncodeMode;
	IPC_FRAME_RATE	m_ipcFrameRate;		//PAL制下的值
	unsigned long	*m_pSupportVideoSize;
	unsigned long	m_ipcMaxTotalBitRate;
	unsigned long	m_bLimitByBitRate;	//当NVR性能受限于芯片CPU能力时，NVR的性能就取决于码率的大小；
										//当NVR性能受限于芯片的解码能力时，NVR的性能就取决于帧率，当为true时，每个通道的分辨率帧率为平均最大值
	unsigned long	m_ipcMaxVideoOutNum;	//IPC回放最大通道数

	unsigned long	m_ipcTransferChnnNum;	//数字通道数据转发给客户端的通道路数

	unsigned long	m_defaLiveAudioState;

	unsigned long	m_pbChannelNumForLimitFirstStream;

	bool			m_bSupportDisplayMinor;	//NVR是否支持现场显示小码流

	unsigned char	m_supportPrimaryStreamReplayNum; //设备能支持同时回放主码流的最大数目

	CEncodeCheck	* m_pEncodeCheck;
	CCalculateSubEncode * m_pSubEncodeCheck;
	VS_SUPBRR *m_pvsBrr;//9个分辨率，每个分辨率对应的码流,依次为QCIF，CIF,HD1,D1,HD1080,HD720,960H,VGA,QVGA
private:
	enum VS_ALL
	{
		VS_QCIF	= 0,
		VS_CIF,	
		VS_HD1,	
		VS_D1,
		VS_HD1080,
		VS_HD720,
		VS_960H,  
		VS_VGA,   
		VS_QVGA, 
	};
	
};
#endif//

