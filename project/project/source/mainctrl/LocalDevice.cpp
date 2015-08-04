/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : 
** Date         : 2008-12-25
** Name         : LocalDevice.cpp
** Version      : 1.0
** Description  :
** Modify Record:
1:
***********************************************************************/
#include "LocalDevice.h"
#include <ctype.h>
#ifdef __ENVIRONMENT_LINUX__
#include "RemoteControl.h"
#else
#include "DecodeDisplay2.h"
#include "DevControlVirtual.h"
#endif
#include "UserMan.h"
#include "MessageMan.h"

#include "EncodeCheck.h"

#ifdef __DVR_ULTIMATE__
#include "IPCDecDisplayMan.h"
#include "NetDeviceManager.h"
#endif

#define FLASH_PARAM_MAC_ADDR       0
#define FLASH_PARAM_HD_VERSION     1
#define FLASH_PARAM_SUB_VER        2
#define FLASH_PARAM_OUT_DEVICE     3
#define FLASH_PARAM_VIDEO_FORMAT   4
#define FLASH_PARAM_DDNS_TYPE      5
#define FLASH_PARAM_KEYBOARD_NAME  6
#define FLASH_PARAM_FUNC_CUSTOM    7
#define FLASH_PARAM_OUT_RESOLUTION 8
#define FLASH_PARAM_EXTERNAL_KEYBOARD  9
#define FLASH_PARAM_NETDEVICE_TYPE 10   //a -- 百兆 其他0 -- 千兆  NetType.dat
#define FLASH_PARAM_TIME_ZONE		11
#define FLASH_PARAM_ENCODE_SIZE		12	//编码最大帧率
#define FLASH_PARAM_COMPANY_NAME	13
#define FLASH_PARAM_SN             14
#define FLASH_PARAM_KERNELVERSION  4096


#define HDVR_DECODE_CHIP_TYPE_TW2864   0x1001
#define HDVR_DECODE_CHIP_TYPE_NVP1104A 0x2001
#define HDVR_DECODE_CHIP_TYPE_CX25838 0x3001
#define HDVR_DECODE_CHIP_TYPE_NVP1108 0x4001
#define HDVR_DECODE_CHIP_TYPE_NVP1918 0x5001
#define HDVR_DECODE_CHIP_TYPE_TW2960  0xb001


const unsigned int CMD_RTC_WRITE	= 0xFFFF;
const unsigned int CMD_RTC_READ		= 0xFFFE;
const unsigned int CMD_RTC_RESET	= 0xFFFD;

extern unsigned char g_coverAreaCountLimit;
extern VIDEO_FORMAT g_video_format;
extern bool g_bCVBSChangeResolution;
extern VGA_RESOLUTION g_resolution;

CPUB_Lock CLocalDevice::m_keyLock;
std::list<KEY_VALUE_INFO> CLocalDevice::m_keyValueList;
#ifdef __ENVIRONMENT_LINUX__
ext_dvr_product_info *g_pCryptoInfo;
#endif

extern ULONGLONG g_net_channel;
bool g_bRu042304SEMETo2504HE_M = false;
int g_Ru04OldProductType = 0;
bool g_b2316SETo2316ME = false;


bool CLocalDevice::m_bUse1104A1108Color   = false;
bool CLocalDevice::m_b2316SSNewPCB = false;


VIDEO_FORMAT	CLocalDevice::m_videoFormatFlash	= VIDEO_FORMAT_NONE;
VIDEO_OUT_TYPE	CLocalDevice::m_videoOutTypeFlash	= VIDEO_OUT_NONE;
VGA_RESOLUTION	CLocalDevice::m_outResolutionFlash	= VGA_RESOLUTION_NONE;
int CLocalDevice::m_encode960H = -1;

bool g_ext_init_interface = false;
//////////////////////////////////////////////////////////////////////////
CLocalDevice *CLocalDevice::Instance()
{
	static CLocalDevice s_LocalDevice;
	return &s_LocalDevice;
}
//////////////////////////////////////////////////////////////////////////
#ifdef __ENVIRONMENT_LINUX__
bool CLocalDevice::Initial(ext_dvr_input_param & parm, int videoOutputNum, int netVideoInputNum, DWORD & sensorInputType, bool bMCU/* = true*/, unsigned char* pCvbs/* = NULL*/)
{
	memset(m_bSensorOutStatus, 0, sizeof(unsigned char )*16);
	memset(m_SenorOutOpenCout, 0, sizeof(int)*16);
	m_bCamVisible = true;
	m_bSupportMCU = bMCU;
	m_videoFormat = static_cast<VIDEO_FORMAT>(parm.videoFormat);
	m_localVideoInputNum = parm.videoChnnNum;
	m_netVideoInputNum	= netVideoInputNum;
	m_videoInputNum	= m_localVideoInputNum + m_netVideoInputNum;
	m_videoOutputNum = videoOutputNum;

	m_pLocalVideoSize = new VIDEO_SIZE[m_localVideoInputNum];
	m_pLocalNetVideoSize = new VIDEO_SIZE[m_localVideoInputNum];	
	for (unsigned char chnn = 0; chnn < m_localVideoInputNum; ++chnn)
	{
		m_pLocalVideoSize[chnn] = static_cast<VIDEO_SIZE>(parm.videoLocalSize[chnn]);
		m_pLocalNetVideoSize[chnn] = static_cast<VIDEO_SIZE>(parm.videoNetSize[chnn]);
	}
	
	//初始化MCU
	if(m_bSupportMCU && !CMcu8952::Instance()->Initial( sensorInputType ))
	{
		printf("Initial MCU fail\n");
#if !defined(__X_LINUX__)
		return false;
#endif
	}
	
	//启动单片机校验线程
	//把开启单片线程从LocalDevice只提到此处。解决：单片机获取版本号不正确的问题。
	if(m_bSupportMCU && !CMcu8952::Instance()->Start())
	{
		PUB_PrintError(__FILE__,__LINE__);
#if !defined(__X_LINUX__)
		return false;
#endif
	}
	m_waveFormat = WAVE_FORMAT_DVI_ADPCM;
#ifdef __NETSERVER_HXHT__	
	ext_change_boot_audio_format(19);
	m_waveFormat = WAVE_FORMAT_ALAW;
#endif

#ifdef __DVR_ULTIMATE__
	ext_change_boot_audio_format(19);
	m_waveFormat = WAVE_FORMAT_ALAW;
//	ext_change_boot_audio_format(20);
//	m_waveFormat = WAVE_FORMAT_MULAW;
#endif

#ifdef __GB28181_SERVER__
	ext_change_boot_audio_format(19);
	m_waveFormat = WAVE_FORMAT_ALAW;
#endif

#ifdef __CHIPTI__ 
	ext_change_boot_audio_format(19);
	m_waveFormat = WAVE_FORMAT_ALAW;
#endif

	{
#if defined(__GB28181_SERVER__)
		ext_set_venc_profile(0);
#endif
	}

	int  iParam=0;
#if defined(__CHIP3520A__) || defined(__CHIP3520D__) || defined(__CHIP3521__)
	NCFG_CVBS_INFO * pFrameRateData = NULL;
#if __ENVIRONMENT_WIN32__
	BYTE* pTempData = NULL;
	unsigned long tempDataLen = 0;
	CCfgDlg::GetConfig( NCFG_ITEM_CVBS_TO_SPOT, &pTempData, tempDataLen);
	assert(tempDataLen > 0);
	pFrameRateData = (reinterpret_cast<NCFG_CVBS_INFO *>(pTempData));
#endif	

#if __ENVIRONMENT_LINUX__
	pFrameRateData = (reinterpret_cast<NCFG_CVBS_INFO *>(pCvbs));
	if (pFrameRateData == NULL)
	{
		printf("pFrameRateData is NULL\n");
	}else{
		printf("pFrameRateData ----------------- %d,%d\n",pFrameRateData->current,pFrameRateData->last);
	}
#endif	
	iParam = pFrameRateData->current;
	if ( (0 == iParam) || (1 == iParam) )
	{
		iParam = 0;  //支持CVBS ,SPOT
	} 
	else if ( 2 == iParam )
	{
		iParam = 1;
	}
#endif

#ifdef __ENVIRONMENT_LINUX__
#if defined(__CHIP3520A__) || defined(__CHIP3520D__) || defined(__CHIP3521__)
	if(0 != ext_set_sdk_param(EXT_SET_SDK_PARAM_CVBS_SUPPORT,&iParam))
	{
		printf("%s,%d,ext_device_init error\n",__FILE__,__LINE__);
		return false;
	}
#endif
#endif

	if (0 != ext_device_init(&parm))
	{
		printf("%s,%d,ext_device_init error\n",__FILE__,__LINE__);
		return false;
	}
	g_ext_init_interface = true;
	printf("%s,%d,ext_device_init \n",__FILE__,__LINE__);

	/*对于rtc时钟的控制有下面的SDK控制
	//打开时钟芯片设备
	m_rtcFd = open("/dev/ds1307", O_RDONLY|O_NONBLOCK);
	if (m_rtcFd <= 0)
	{
		printf("Open rtc driver file fail\n");
		return 0;
	}
	*/

	//先初始化时间
	if (!InitSystemTime())
	{
		printf("Initial system time fail\n");
#if !defined(__X_LINUX__)
		return false;
#endif
	}

	m_preLiveAudioChnn = -1;
	m_bOpenAudio = false;
	m_bAudioAuto = false;
	m_bLiveOneChnn = -1;
	m_bTalking = false;

	m_pOSDInfo = new OSD_INFO [m_localVideoInputNum];
	memset(&m_timeStampAttrib, 0, sizeof(OSD_ATTRIB));
	m_timeStampAttrib.pData = new unsigned char [512];

	if (m_bSupportMCU)
	{
		CMcu8952::Instance()->Operation(SERIAL_LAMP, SERIAL_LAMP2_OFF);
		CMcu8952::Instance()->Operation(SERIAL_LAMP, SERIAL_LAMP3_OFF);
		CMcu8952::Instance()->Operation(SERIAL_LAMP, SERIAL_LAMP4_OFF);
		CMcu8952::Instance()->Operation(SERIAL_LAMP, SERIAL_LAMP5_OFF);
		CMcu8952::Instance()->Operation(SERIAL_LAMP, SERIAL_LAMP6_OFF);
	}
	else
	{
		ext_led_set(SERIAL_LAMP, SERIAL_LAMP2_OFF);
		ext_led_set(SERIAL_LAMP, SERIAL_LAMP3_OFF);
		ext_led_set(SERIAL_LAMP, SERIAL_LAMP4_OFF);
		ext_led_set(SERIAL_LAMP, SERIAL_LAMP5_OFF);
		ext_led_set(SERIAL_LAMP, SERIAL_LAMP6_OFF);
	}

	g_pCryptoInfo = NULL;

	m_bPlaybacking = false;

	
	
	{
		m_bUse1104A1108Color   = false;
		m_b2316SSNewPCB = false;
		unsigned int type0 = 0;
		unsigned int type1 = 0;
		unsigned int type2 = 0;
		unsigned int type3 = 0;

		PRODUCT_TYPE productTypeTemp = CProduct::Instance()->ProductType();
		ext_get_board_used_chip_type(&type0, &type1, &type2, &type3);

		printf("##read chip type:%d ",type0);
		if (HDVR_DECODE_CHIP_TYPE_TW2864 == type0)
		{
			printf(" is 2864\n");
			m_bUse1104A1108Color = false;
		}
		else if (HDVR_DECODE_CHIP_TYPE_NVP1104A == type0)
		{
			printf(" is 1104\n");
			m_bUse1104A1108Color = true;			
		}
		else if (HDVR_DECODE_CHIP_TYPE_CX25838 == type0)
		{
			printf(" is 25838\n");
			m_bUse1104A1108Color = false;
		}
		else if (HDVR_DECODE_CHIP_TYPE_NVP1108 == type0)
		{
			printf(" is 1108\n");
			m_bUse1104A1108Color = true;			
			if (productTypeTemp == TD_2316CDS)
			{
				m_b2316SSNewPCB = true;
			}
		}
		else if (HDVR_DECODE_CHIP_TYPE_TW2960 == type0)
		{
			printf(" is 2960 \n");
			m_bUse1104A1108Color = false;
		}
		else if (HDVR_DECODE_CHIP_TYPE_NVP1918 == type0)
		{
			printf("%s:%s:%d, DEcode is nvp 1918\n", __FUNCTION__, __FILE__, __LINE__);
			m_bUse1104A1108Color = false;
		}
		else
		{
			printf(" is other chip\n");
			m_bUse1104A1108Color = true;
		}	
	}
	//初始化485接口	
	bool bCtrl485CS = false;
	{
		PRODUCT_TYPE productTypeTemp = CProduct::Instance()->ProductType();
		if ((productTypeTemp == TD_2304CDS) || (productTypeTemp == TD_2304HS) || m_b2316SSNewPCB ||
			(productTypeTemp == TD_2604LE) || (productTypeTemp == TD_2608LE) || (productTypeTemp == TD_2616LE) || 
			(productTypeTemp == TD_2516HE_B_N100) || (productTypeTemp == TD_2524HE_B) || (productTypeTemp == TD_2532HE_B) ||
			(productTypeTemp == TD_2704XE_S) || (productTypeTemp == TD_2708XE_S) ||
			(productTypeTemp == TD_2704XS_SL) || (productTypeTemp == TD_2708XS_SL) 
			|| (productTypeTemp == TD_2816NS_C) || (productTypeTemp == TD_2804NS_C) || (productTypeTemp == TD_2808NS_C)
			|| (productTypeTemp == TD_2808NE_C) || (productTypeTemp == TD_2816NE_C) || (productTypeTemp == TD_2832NE_C)
			|| (productTypeTemp == TD_2832PD_C16)
			|| (productTypeTemp == TD_2816ND_C) || (productTypeTemp == TD_2832ND_C)
			|| (productTypeTemp == TD_2804PS_C4) || (productTypeTemp == TD_2808PS_C4) 
			|| (productTypeTemp == TD_2808PS_C8) || (productTypeTemp == TD_2816PS_C8) 
			|| (productTypeTemp == TD_2816PE_C8) || (productTypeTemp == TD_2832PE_C8)
			|| (productTypeTemp == TD_2704AS_SL) || (productTypeTemp == TD_2704AS_SL_A) || (productTypeTemp == TD_2704AS_PL) || (productTypeTemp == TD_2708AS_SL) || (productTypeTemp == TD_2716AS_SL)
			|| (productTypeTemp == TD_2704AS_SL_A) || (productTypeTemp == TD_2708AS_SL_A) || (productTypeTemp == TD_2716AS_SL_A)
			|| (productTypeTemp == TD_2704AE_SL) || (productTypeTemp == TD_2708AE_SL) || (productTypeTemp == TD_2716AE_SL)
			|| (productTypeTemp == TD_2704AS_S) || (productTypeTemp == TD_2708AS_S) || (productTypeTemp == TD_2708AS_S_A) || (productTypeTemp == TD_2716AS_S)
			|| (productTypeTemp == TD_2704AS_S_A) || (productTypeTemp == TD_2708AS_S_A) || (productTypeTemp == TD_2716AS_S_A)
			|| (productTypeTemp == TD_2704TS_S) || (productTypeTemp == TD_2708TS_S) || (productTypeTemp == TD_2716TS_S) 
			|| (TD_2704TS_PL == productTypeTemp) || (TD_2708TS_PL == productTypeTemp) || (TD_2716TS_PL == productTypeTemp) //TD_2308CDS不用片) //TD_2308CDS不用片选;TD_2304CDS需要片选
			|| (TD_2704AE_PL == productTypeTemp) || (TD_2708AE_PL == productTypeTemp) || (TD_2716AE_PL == productTypeTemp)
			|| (TD_2704AC_PL == productTypeTemp) || (TD_2708AC_PL == productTypeTemp) || (TD_2716AC_PL == productTypeTemp)
			|| (TD_2704AS_PL == productTypeTemp) || (TD_2708AS_PL == productTypeTemp) || (TD_2716AS_PL == productTypeTemp))
		{
			bCtrl485CS = true;
			printf("###%s,%d,485CS\n",__FILE__,__LINE__);
		}
	}

	if( !CSerialPort::Instance()->Initial(bCtrl485CS) )
	{
		printf("Initial serial fail\n");
#if !defined(__X_LINUX__)
		return false;
#endif
	}

	if (bCtrl485CS)
	{
		printf("####################3%s,%d CLocalDevice::Initial\n,",__FILE__,__LINE__); 
		ext_led_set(9, 0);
	}

	m_productType = CProduct::Instance()->ProductType();

	m_imageDefaultColor = 0;
	m_bright = 0;
	m_hue = 0;
	m_sat = 0;
	m_contrast = 0;

	m_bInitial = true;

	return true;
}

//使用GMT标准时间(即日历时间)进行设入,所以本地时间转换.
bool CLocalDevice::SetSystemTime(unsigned long time, bool bSetOS/* = true*/)
{
#if defined(__X_LINUX__)
	return true;
#endif
	ext_dvr_rtc_time  struTimeNow;
	time_t t = time;
	struct tm tmTime;
	
	memset(&tmTime, 0, sizeof(tmTime));
	gmtime_r(&t, &tmTime);

	struTimeNow.second	= static_cast<unsigned int>(tmTime.tm_sec);
	struTimeNow.minute	= static_cast<unsigned int>(tmTime.tm_min);
	struTimeNow.hour	= static_cast<unsigned int>(tmTime.tm_hour);
	struTimeNow.dayofweek = static_cast<unsigned int>(tmTime.tm_wday + 1);//此处有错误0-6（周天为0),但是刘志高修改了底层，所以这里不动
	struTimeNow.day		= static_cast<unsigned int>(tmTime.tm_mday);
	struTimeNow.month	= static_cast<unsigned int>(tmTime.tm_mon + 1);
	struTimeNow.year	= static_cast<unsigned int>(tmTime.tm_year - 100);

	if (bSetOS)
	{
		printf("Set RTC Time:%d-%d-%d %d:%d:%d\n", struTimeNow.year, struTimeNow.month, struTimeNow.day, struTimeNow.hour, struTimeNow.minute, struTimeNow.second);
		printf("RTC dayofweek %d\n",struTimeNow.dayofweek);
	}

	ext_rtc_time_system(&struTimeNow, 1);

	if (bSetOS)
	{
		struct timeval tv;
		struct timezone tz;

		gettimeofday(&tv, &tz);

		tv.tv_sec = t;
		tv.tv_usec = 0;

		settimeofday(&tv, &tz);
		ext_chagne_system_time_flag();
	}
	
	return true;
}

bool CLocalDevice::InitSystemTime()
{
#if defined(__X_LINUX__)
	return true;
#endif
	ext_dvr_rtc_time time;
	memset(&time, 0, sizeof(ext_dvr_rtc_time));
	
	ext_rtc_time_system(&time, 0);
	
	struct tm tmTime;
	struct timeval tv;
	struct timezone tz;

	tmTime.tm_sec	= static_cast<int>(time.second); 
	tmTime.tm_min	= static_cast<int>(time.minute);
	tmTime.tm_hour	= static_cast<int>(time.hour);
	tmTime.tm_mday	= static_cast<int>(time.day);
	tmTime.tm_mon	= static_cast<int>(time.month - 1); 
	tmTime.tm_year	= static_cast<int>(time.year + 100);
	tmTime.tm_wday	= static_cast<int>(time.dayofweek - 1);

	printf("RTC Time:%d-%d-%d %d:%d:%d\n", time.year, time.month, time.day, time.hour, time.minute, time.second);

	struct tm buildTime;
	memset(&buildTime, 0, sizeof(tm));
	time_t	t = GetBuildDate();	//获取的为GMT标准时间,所以必须用标准时间转换
	gmtime_r(&t, &buildTime );

	//如果RTC时钟不正常,则使用版本编译时间初始化系统时间.
	if ((tmTime.tm_year < 109) || (135 < tmTime.tm_year))
	{
		tmTime = buildTime;

		//重新设入RTC芯片
		time.second		= static_cast<unsigned int>(tmTime.tm_sec);
		time.minute		= static_cast<unsigned int>(tmTime.tm_min);
		time.hour		= static_cast<unsigned int>(tmTime.tm_hour);
		time.dayofweek	= static_cast<unsigned int>(tmTime.tm_wday + 1);
		time.day		= static_cast<unsigned int>(tmTime.tm_mday);
		time.month		= static_cast<unsigned int>(tmTime.tm_mon + 1);
		time.year		= static_cast<unsigned int>(tmTime.tm_year - 100);

		printf("Build Time to RTC Time:%d-%d-%d %d:%d:%d\n", time.year, time.month, time.day, time.hour, time.minute, time.second);

		ext_rtc_time_system(&time, 1);
	}

	time_t lTime = timegm(&tmTime);
	
	gettimeofday(&tv, &tz);

	tv.tv_sec = lTime;
	tv.tv_usec = 0;

	settimeofday(&tv, &tz);

	ext_chagne_system_time_flag();
	return true;
}

bool CLocalDevice::FeedDog()
{
#if defined(__DVR_DEBUG__)
	return true;
#endif

	if (m_bSupportMCU)
	{
		if (!CMcu8952::Instance()->Operation(SERIAL_FEEDDOG, SERIAL_FEEDDOG_ON))
		{
			//printf("#############################################feed dog error.time:");
			//PrintfBit32Time(GetCurrTime32());
			return false;
		}
	}
	else
	{
		ext_wdg_set(1);
	}
	return true;
}

void CLocalDevice::KillDog()
{
#if defined(__DVR_DEBUG__)
	return;
#endif

	if (m_bSupportMCU)
	{
		CMcu8952::Instance()->Operation(SERIAL_KILLDOG, SERIAL_FEEDDOG_ON);
	}
	else
	{
		ext_wdg_set(0);
	}
}
#endif

#ifdef __ENVIRONMENT_WIN32__
bool CLocalDevice::Initial(CDecodeDisplay2 *pDecodeDisplay, HWND hPlayWnd, int localVideoInputNum, int videoOutputNum, int netVideoInputNum)
{
	m_pDataDisplay = pDecodeDisplay;
	m_pDataDisplay->Initial(25, hPlayWnd);
	m_hPlaybackWnd = hPlayWnd;

	m_localVideoInputNum = localVideoInputNum;
	m_netVideoInputNum	= netVideoInputNum;
	m_videoInputNum	= m_localVideoInputNum + m_netVideoInputNum;
	m_videoOutputNum = videoOutputNum;

	m_bUse1104A1108Color   = false;
	m_b2316SSNewPCB = false;

	m_bCamVisible = true;

	m_imageDefaultColor = 0;
	m_bright = 0;
	m_hue = 0;
	m_sat = 0;
	m_contrast = 0;

	m_bInitial = true;

	return true;
}
#endif


void CLocalDevice::Quit()
{
#ifdef __ENVIRONMENT_LINUX__
	if (m_bSupportMCU)
	{
		CMcu8952::Instance()->Operation(SERIAL_LAMP, SERIAL_LAMP2_OFF);
		CMcu8952::Instance()->Operation(SERIAL_LAMP, SERIAL_LAMP3_OFF);
		CMcu8952::Instance()->Operation(SERIAL_LAMP, SERIAL_LAMP4_OFF);
		CMcu8952::Instance()->Operation(SERIAL_LAMP, SERIAL_LAMP5_OFF);
		CMcu8952::Instance()->Operation(SERIAL_LAMP, SERIAL_LAMP6_OFF);

		//关掉所有的VIDEO LOSS  指示灯
		CMcu8952::Instance()->Operation(SERIAL_SETVIDEOLOSSLAMP,  0xffffffff);
		CMcu8952::Instance()->Operation(SERIAL_SETVIDEOLOSSLAMP_H,0xffffffff);

		CMcu8952::Instance()->Quit();
	}
	else
	{
		ext_led_set(SERIAL_LAMP, SERIAL_LAMP2_OFF);
		ext_led_set(SERIAL_LAMP, SERIAL_LAMP3_OFF);
		ext_led_set(SERIAL_LAMP, SERIAL_LAMP4_OFF);
		ext_led_set(SERIAL_LAMP, SERIAL_LAMP5_OFF);
		ext_led_set(SERIAL_LAMP, SERIAL_LAMP6_OFF);
	}
	
	CSerialPort::Instance()->Quit();

	ext_device_quit(0);
	/*
	close(m_rtcFd);
	m_rtcFd = -1;
	*/

	delete [] m_pOSDInfo;
	m_pOSDInfo = NULL;

	delete [] m_timeStampAttrib.pData;
	m_timeStampAttrib.pData = NULL;
#else
	m_pDataDisplay->Quit();
#endif

#ifdef __ENVIRONMENT_LINUX__
	if (g_pCryptoInfo != NULL)
	{
		delete g_pCryptoInfo;
		g_pCryptoInfo = NULL;
	}

	if (NULL != m_pLocalVideoSize)
	{
		delete [] m_pLocalVideoSize;
		m_pLocalVideoSize = NULL;
	}
	if (NULL != m_pLocalNetVideoSize)
	{
		delete [] m_pLocalNetVideoSize;
		m_pLocalNetVideoSize = NULL;
	}
#endif
}

bool CLocalDevice::Start()
{
#ifdef __ENVIRONMENT_LINUX__
	//在创建鼠标线程前，要初始化好分辨率
	assert(m_displayHeight != 0);
	assert(m_displayWidth != 0);

//#if defined(__CHIPTI__)
#if 0
	m_mouseProcID = PUB_CreateThread(MouseThread, this, &m_bMouseProc);
	if(m_mouseProcID == PUB_CREATE_THREAD_FAIL)
	{
		PUB_PrintError(__FILE__,__LINE__);
		return false;
	}
#endif
	
	//下面函数返回的m_bitmapInfo，m_waveFormat有问题
	if (0 != ext_capture_start(VIDEO_STREAM_TYPE_PRIMARY))
	{
		PUB_PrintError(__FILE__,__LINE__);
		return false;
	}
	

	if (0 != ext_capture_start(VIDEO_STREAM_TYPE_NETWORK))
	{
		PUB_PrintError(__FILE__,__LINE__);
		return false;
	}


	if (0 != ext_capture_start(VIDEO_STREAM_TYPE_AUDIO))
	{
		PUB_PrintError(__FILE__,__LINE__);
		return false;
	}

	CExternalKeyboard *pExternalKeyboard = CExternalKeyboard::Instance();
	//把外接485键盘的类型设入
	pExternalKeyboard->SetExKeyboardType( static_cast<KEYBOARD_TYPE>(CProduct::Instance()->External485KeyboardType()) );

	if( !pExternalKeyboard->Start())
	{
		PUB_PrintError(__FILE__,__LINE__);
		return false;
	}

	return true;
#endif

	return true;
}

void CLocalDevice::Stop()
{
#ifdef __ENVIRONMENT_LINUX__
	CExternalKeyboard::Instance()->Stop();

	if (m_bSupportMCU)
	{
		CMcu8952::Instance()->Stop();
	}

	/*if (PUB_THREAD_ID_NOINIT != m_mouseProcID)
	{		
		PUB_ExitThread(&m_mouseProcID, &m_bMouseProc);
	}*/
	/*printf("%s,%d\n",__FILE__,__LINE__);
	ext_capture_stop(VIDEO_STREAM_TYPE_PRIMARY);
	ext_capture_stop(VIDEO_STREAM_TYPE_NETWORK);
	ext_capture_stop(VIDEO_STREAM_TYPE_AUDIO);*/
#endif
}

typedef struct _display_attrib_ex
{
	unsigned short bShow;	//0表示不显示，1表示显示现场，2表示显示回放
	unsigned short chnn;	//通道号
	unsigned short xPos;	//左上角横坐标
	unsigned short yPos;	//左上角纵坐标
	unsigned short cx;		//显示区域宽度（像素）
	unsigned short cy;		//显示区域高度（像素）
}DISPLAY_ATTRIB_EX;

void CLocalDevice::ChangeDisplayMode(const DISPLAY_DEV_INFO* pDispDevInfo, DISPLAY_ATTRIB *pAttrib, unsigned char chnnNum, unsigned long* pSplitMode, int splitNum, unsigned char* pVisibleChnn, unsigned char bFilter)
{
	m_dispChannelNum = chnnNum;
	
#ifdef __DVR_ULTIMATE__
	bool bPlayback = false; //回放时,不需要开启网络流
	int ipc_width = 0;
	int ipc_height = 0;
	bool b3MBigImageUsedD1Show = false;
	bool bVoOutType = false;
	unsigned long skSplitm = pSplitMode[DISPLAY_LAYER_NET];

	if (VIDEO_OUT_VGA == pDispDevInfo->videoType || VIDEO_OUT_HDMI == pDispDevInfo->videoType)
	{
		bVoOutType = true;
	}

	if( bVoOutType )
	{
		if( skSplitm <= VIEW_SPLIT_2X2 )
		{
			CNetDeviceManager::Instance()->SetCurDisplaySpiltMode( 1 );
		}
		else
		{
			CNetDeviceManager::Instance()->SetCurDisplaySpiltMode( 0 );
		}

		CNetDeviceManager::Instance()->SetDispModeBigImageUsedD1( 0xffff, false );
		if( VIEW_SPLIT_1X1 == skSplitm || VIEW_SPLIT_2X2 == skSplitm 
			|| VIEW_SPLIT_3X3 == skSplitm || VIEW_SPLIT_4X4 == skSplitm
			|| VIEW_SPLIT_5X5 == skSplitm || VIEW_SPLIT_6X6 == skSplitm )
		{
			b3MBigImageUsedD1Show = false;
		}
		else
		{
			b3MBigImageUsedD1Show = false;
		}
	}


	ULONGLONG netCH = 0, netCHMinor = 0;
	for(int ch = 0; ch < chnnNum; ch++)
	{
		if(DISPLAY_NET == pAttrib[ch].bShow)
		{
			if(CProduct::Instance()->IPCSupportDisplayMinor())
			{
				if(pAttrib[ch].cx >= (m_displayWidth*2)/3)
				{
					if( b3MBigImageUsedD1Show )
					{
						ipc_width = 0;
						ipc_height = 0;
						CNetDeviceManager::Instance()->GetIPCCurrentResolution(pAttrib[ch].chnn + m_localVideoInputNum, &ipc_width, &ipc_height );
						if( ipc_width*ipc_height <= 0 )
						{
							netCHMinor |= (static_cast<ULONGLONG>(0x1) << pAttrib[ch].chnn);
							CNetDeviceManager::Instance()->SetDispModeBigImageUsedD1( pAttrib[ch].chnn, true );
						}
						else if( ipc_width <= 1920 && ipc_height <= 1088 )
						{
							netCH |= (static_cast<ULONGLONG>(0x1) << pAttrib[ch].chnn);
						}
						else
						{
							netCHMinor |= (static_cast<ULONGLONG>(0x1) << pAttrib[ch].chnn);
							CNetDeviceManager::Instance()->SetDispModeBigImageUsedD1( pAttrib[ch].chnn, true );
						}

					}
					else
					{
						ipc_width = 0;
						ipc_height = 0;
						CNetDeviceManager::Instance()->GetIPCCurrentResolution(pAttrib[ch].chnn + m_localVideoInputNum, &ipc_width, &ipc_height);
						if((ipc_width*ipc_height > 1920 * 1088) && (VIEW_SPLIT_1X1 != skSplitm) || ((ipc_width*ipc_height > 1280 * 960) && (TD_2708AS_S == m_productType || TD_2708AS_S_A == m_productType)))
						{

							netCHMinor |= (static_cast<ULONGLONG>(0x1) << pAttrib[ch].chnn);
							if(b3MBigImageUsedD1Show)
							{
								CNetDeviceManager::Instance()->SetDispModeBigImageUsedD1( pAttrib[ch].chnn, true );
							}
						}
						else
						{
							netCH |= (static_cast<ULONGLONG>(0x1) << pAttrib[ch].chnn);
						}
					}
				}
				else if(pAttrib[ch].cx >= (m_displayWidth)/2)
				{
					ipc_width = 0;
					ipc_height = 0;

					CNetDeviceManager::Instance()->GetIPCCurrentResolution(pAttrib[ch].chnn + m_localVideoInputNum, &ipc_width, &ipc_height );

					if( ipc_width*ipc_height <= 0 )
					{
						netCHMinor |= (static_cast<ULONGLONG>(0x1) << pAttrib[ch].chnn);
						if( b3MBigImageUsedD1Show )
						{
							CNetDeviceManager::Instance()->SetDispModeBigImageUsedD1( pAttrib[ch].chnn, true );
						}
					}
					else if( ipc_width*ipc_height > 1920*1088 )
					{
						netCHMinor |= (static_cast<ULONGLONG>(0x1) << pAttrib[ch].chnn);
						if( b3MBigImageUsedD1Show )
						{
							CNetDeviceManager::Instance()->SetDispModeBigImageUsedD1( pAttrib[ch].chnn, true );
						}
					}
#if defined(__CHIP3520D__) || defined(__CHIP3521__)
					else if ((ipc_width * ipc_height > 1280 * 720) && (0 < m_localVideoInputNum))
					{
						netCHMinor |= (static_cast<ULONGLONG>(0x1) << pAttrib[ch].chnn);
					}
					else if (((pAttrib[ch].cx == (m_displayWidth)/2) && (0 == m_localVideoInputNum) && (ipc_width * ipc_height > 960 * 480)) || (TD_2304SS_C == m_productType)|| (TD_2308SE_C == m_productType)|| (TD_2308SS_C == m_productType))
					{
						netCHMinor |= (static_cast<ULONGLONG>(0x1) << pAttrib[ch].chnn);
					}
#elif defined(__CHIP3531__) && defined(__HISI_SDK_0BX__) || defined(__CHIP3520A__)
					else if ((ipc_width * ipc_height > 960 * 480) && (0 < m_localVideoInputNum))
					{
						netCHMinor |= (static_cast<ULONGLONG>(0x1) << pAttrib[ch].chnn);
					}
#elif defined(__CHIP3531__)
					else if ((pAttrib[ch].cx == m_displayWidth / 2) && (0 < m_localVideoInputNum))
					{
						//3531做的混合型设备，只有4分割不显示主码流
						netCHMinor |= (static_cast<ULONGLONG>(0x1) << pAttrib[ch].chnn);
					}
#endif
					else
					{
						netCH |= (static_cast<ULONGLONG>(0x1) << pAttrib[ch].chnn);
					}
				}
				else
				{
					netCHMinor |= (static_cast<ULONGLONG>(0x1) << pAttrib[ch].chnn);
				}
			}
			else
			{
				netCH |= (static_cast<ULONGLONG>(0x1) << pAttrib[ch].chnn);
			}
		}
		else if(DISPLAY_PLAYBACK == pAttrib[ch].bShow)
		{
			//只要一路回放通道,即认为处于回放
			bPlayback = true;
		}
	}
#endif

	DISPLAY_ATTRIB *pAttribTemp = new DISPLAY_ATTRIB[chnnNum];
	memcpy(pAttribTemp, pAttrib, sizeof(DISPLAY_ATTRIB) * chnnNum);

#if defined(__TDFH__) || defined(__TDNXP__) || defined(__CHIPTI__)
	DISPLAY_ATTRIB_EX* pDispEx = new DISPLAY_ATTRIB_EX[36];	
	memset(pDispEx, 0, sizeof(DISPLAY_ATTRIB_EX) * 36);
#ifdef __ENVIRONMENT_LINUX__	
	ext_zorder *pZorder = new ext_zorder[36];
	memset(pZorder, 0, sizeof(ext_zorder) * 36);
#endif

	//用于将画面数据补齐
	bool bUsedIndex[36] = {false};
	unsigned long splitMode = 0;

	for(int i = 0; i < chnnNum; i++)
	{
		pDispEx[i].bShow = (pAttrib[i].bShow == DISPLAY_NET) ? DISPLAY_PLAYBACK : pAttrib[i].bShow;
		pDispEx[i].chnn = pAttrib[i].chnn;
		pDispEx[i].xPos = pAttrib[i].xPos;
		pDispEx[i].yPos = pAttrib[i].yPos;
		pDispEx[i].cx = pAttrib[i].cx;
		pDispEx[i].cy = pAttrib[i].cy;
	
#if defined(__TDFH__) || defined(__CHIPTI__)
		if (pDispDevInfo->width < (pDispEx[i].xPos + pDispEx[i].cx + 10))
		{
			pDispEx[i].cx = (pDispDevInfo->width - pDispEx[i].xPos);
		}
#endif
#ifdef __ENVIRONMENT_LINUX__
		pZorder[i].chnn = pAttrib[i].chnn;
		if (pAttrib[i].zOrder >= 4)
		{
			pZorder[i].zorder = 2;
		}
		else
		{
			pZorder[i].zorder = 1;
			bUsedIndex[pAttrib[i].index] = true;
		}
#endif
		if( (DISPLAY_LAYER_NULL != pAttrib[i].zOrder) &&
			(DISPLAY_LAYER_MINI != pAttrib[i].zOrder) &&
			(0 == splitMode))
		{
			//找到主分隔模式
			splitMode = pSplitMode[pAttrib[i].zOrder];
		}
	}
#else
	DISPLAY_ATTRIB_EX* pDispEx = new DISPLAY_ATTRIB_EX[chnnNum + 1];	
	memset(pDispEx, 0, sizeof(DISPLAY_ATTRIB_EX) * (chnnNum + 1));
#ifdef __ENVIRONMENT_LINUX__
	ext_zorder *pZorder = new ext_zorder [chnnNum + 1];
	memset(pZorder, 0, sizeof(ext_zorder) * (chnnNum+1));
#endif

	for(int i = 0; i < chnnNum; i++)
	{
		if(pAttribTemp[i].bShow == DISPLAY_NET)
		{
			pAttribTemp[i].chnn += m_localVideoInputNum;
		}

		pDispEx[i].bShow = pAttrib[i].bShow;
		pDispEx[i].chnn = pAttrib[i].chnn;
		pDispEx[i].xPos = pAttrib[i].xPos;
		pDispEx[i].yPos = pAttrib[i].yPos;
		pDispEx[i].cx = pAttrib[i].cx;
		pDispEx[i].cy = pAttrib[i].cy;
#ifdef __ENVIRONMENT_LINUX__
		pZorder[i].chnn = pAttrib[i].chnn;
		if (pAttrib[i].zOrder >= 4)
		{
			pZorder[i].zorder = 2;
		}
		else
		{
			pZorder[i].zorder = 1;
		}
#endif
	}
#endif

	ULONGLONG displayCH = 0, displayCHMinor = 0;

	//不检查任何参数，直接显示，仅用在开机启动后第一次设置分割模式
	if(bFilter)
	{
		assert (NULL != pAttrib);
		if (DISPLAY_DEV_MAJOR == pDispDevInfo->devMode)
		{
			for (unsigned char i=0; i<chnnNum; ++i)
			{
				if ((DISPLAY_LIVE == pAttribTemp[i].bShow) || (DISPLAY_NET == pAttribTemp[i].bShow))	//现场的才检查
				{
					pDispEx[i].bShow = (pDispEx[i].bShow == DISPLAY_NET) ? DISPLAY_PLAYBACK : pDispEx[i].bShow;
				}
			}

#ifdef __DVR_ULTIMATE__

			if(!bPlayback && (DISPLAY_DEV_MAJOR == pDispDevInfo->devMode))
			{
				printf("netCH = %x, netCHMinor = %x, %s, %d\n", (unsigned long)netCH, (unsigned long)netCHMinor, __FILE__, __LINE__);
				CIPCDecDisplayMan::Instance()->SetDisplayCH(netCH, netCHMinor);
			}
#endif

#ifdef __ENVIRONMENT_LINUX__
			if (VIDEO_OUT_CVBS == pDispDevInfo->videoType)
			{
				ext_change_display_slave((DISPLAY_ATTRIB*)pDispEx, chnnNum);
			}
			else
			{
				ext_change_display_zorder(pZorder, chnnNum);						
				ext_change_display_format((DISPLAY_ATTRIB*)pDispEx, chnnNum);
			}
#endif
			GUI::CFBInterface::Instance()->SetDisplayAttrib(pAttribTemp, chnnNum, pSplitMode, splitNum);
		}
		else
		{
			for (unsigned char i=0; i<chnnNum; ++i)
			{
				if ((DISPLAY_LIVE == pAttribTemp[i].bShow) || (DISPLAY_NET == pAttribTemp[i].bShow))	//现场的才检查
				{
					pDispEx[i].bShow = (pAttribTemp[i].bShow == DISPLAY_NET) ? DISPLAY_PLAYBACK : pAttribTemp[i].bShow;
				}
			}

#ifdef __DVR_ULTIMATE__
			if(!bPlayback && (DISPLAY_DEV_MAJOR == pDispDevInfo->devMode))
			{
				printf("netCH = %x, netCHMinor = %x, %s, %d\n", (unsigned long)netCH, (unsigned long)netCHMinor, __FILE__, __LINE__);
				CIPCDecDisplayMan::Instance()->SetDisplayCH(netCH, netCHMinor);
			}
#endif

#ifdef __ENVIRONMENT_LINUX__
			if (VIDEO_OUT_CVBS == pDispDevInfo->videoType)
			{
				ext_change_display_slave((DISPLAY_ATTRIB*)pDispEx, chnnNum);
			}
			else
			{
				ext_change_display_zorder(pZorder, chnnNum);
				ext_change_display_format((DISPLAY_ATTRIB*)pDispEx, chnnNum);
			}
#endif
		}
	}
	else
	{
		assert (NULL != pAttrib);
		if (DISPLAY_DEV_MAJOR == pDispDevInfo->devMode)
		{
			//2010-02-04 10:32:00 YSW
			//在此检查权限设置，然后根据权限设定来把某些通道的标记修改过来
			ULONGLONG authCH = CUserMan::Instance()->CheckAuthority(AUTH_LIVE);

			int liveNum = 0;
			int audioChannel = 0;
			for (unsigned char i=0; i<chnnNum; ++i)
			{
				if ((DISPLAY_LIVE == pAttribTemp[i].bShow) || (DISPLAY_NET == pAttribTemp[i].bShow))	//现场的才检查
				{
					if((pAttribTemp[i].chnn < m_videoInputNum) && pVisibleChnn && (pVisibleChnn[pAttribTemp[i].chnn] <= 0))
					{
						if(DISPLAY_NET == pAttribTemp[i].bShow)
						{
#ifdef __DVR_ULTIMATE__
							netCH &= ~((ULONGLONG)0x01 << pAttrib[i].chnn);
							netCHMinor &= ~((ULONGLONG)0x01 << pAttrib[i].chnn);
#endif
						}
						pAttribTemp[i].bShow = DISPLAY_INVISIBLE;//表示隐藏,设置不显示
						pDispEx[i].bShow = DISPLAY_HIDE;
					}
					else if (0 == (authCH & ((ULONGLONG)(0x01) << pAttribTemp[i].chnn)))
					{
						if(DISPLAY_NET == pAttribTemp[i].bShow)
						{
#ifdef __DVR_ULTIMATE__
							netCH &= ~((ULONGLONG)0x01 << pAttrib[i].chnn);
							netCHMinor &= ~((ULONGLONG)0x01 << pAttrib[i].chnn);
#endif
						}
						pAttribTemp[i].bShow = DISPLAY_HIDE;//表示隐藏
						pDispEx[i].bShow = DISPLAY_HIDE;
					}
					else if((0 == (g_net_channel & ((ULONGLONG)(0x01) << pAttribTemp[i].chnn))) \
						&& (DISPLAY_NET == pAttribTemp[i].bShow))
					{
#ifdef __DVR_ULTIMATE__
						netCH &= ~((ULONGLONG)0x01 << pAttrib[i].chnn);
						netCHMinor &= ~((ULONGLONG)0x01 << pAttrib[i].chnn);
#endif
						pAttribTemp[i].bShow = DISPLAY_INVISIBLE;
						pDispEx[i].bShow = DISPLAY_HIDE;
					}
					else
					{
						liveNum++;
						audioChannel = pAttribTemp[i].chnn;
						pDispEx[i].bShow = (pDispEx[i].bShow == DISPLAY_NET) ? DISPLAY_PLAYBACK : pDispEx[i].bShow;
					}
					if (!m_bCamVisible)
					{
						if(DISPLAY_NET == pAttribTemp[i].bShow)
						{
#ifdef __DVR_ULTIMATE__
							netCH &= ~((ULONGLONG)0x01 << pAttrib[i].chnn);
							netCHMinor &= ~((ULONGLONG)0x01 << pAttrib[i].chnn);
#endif
						}
						pAttribTemp[i].bShow = DISPLAY_HIDE;
						pDispEx[i].bShow = DISPLAY_HIDE;
					}
				}
			}

#ifdef __DVR_ULTIMATE__
			if(!bPlayback && (DISPLAY_DEV_MAJOR == pDispDevInfo->devMode))
			{
				printf("netCH = %x, netCHMinor = %x, %s, %d\n", (unsigned long)netCH, (unsigned long)netCHMinor, __FILE__, __LINE__);
				CIPCDecDisplayMan::Instance()->SetDisplayCH(netCH, netCHMinor);
			}
#endif

			if (1 == liveNum)
			{
				m_bLiveOneChnn = audioChannel;
			}
			else
			{
				m_bLiveOneChnn = -1;
			}

			printf("liveNum:%d,m_bOpenAudio:%d,m_bAudioAuto:%d,audioChannel:%d\n",
				liveNum,m_bOpenAudio,m_bAudioAuto,audioChannel);
			if (m_bAudioAuto && m_bOpenAudio)
			{
				if ((1 == liveNum) && ((audioChannel < CProduct::Instance()->AudioInputNum()) || (audioChannel >= m_localVideoInputNum)))
				{
					if (m_preLiveAudioChnn != audioChannel)
					{
						printf("%s,%d,open chnn:%d live audio, ", __FILE__,__LINE__,audioChannel);
						if ((!m_bTalking) && (!m_bPlaybacking))
						{
							printf("ok\n");
#ifdef __ENVIRONMENT_LINUX__
							if(audioChannel < m_localVideoInputNum)
							{
								if(m_preLiveAudioChnn >= m_localVideoInputNum)
								{
#ifdef __DVR_ULTIMATE__
									ext_ip_in_audio_stop();
#endif

								}
								ext_set_audio_live_channel(1, audioChannel);
							}
							else
							{
								if(m_preLiveAudioChnn < m_localVideoInputNum)
								{
									ext_set_audio_live_channel(0, 0);
								}
#ifdef __DVR_ULTIMATE__
								ext_ip_in_audio_start(audioChannel-m_localVideoInputNum);
#endif
							}
#endif
						}
						else
						{
							printf("error,because of playbacking or talking\n");
						}

						m_preLiveAudioChnn = audioChannel;

						CMessageMan::Instance()->SetAudioChnn(m_preLiveAudioChnn);
					}		
				}
			}
			
#if defined(__TDFH__) || defined(__TDNXP__) || defined(__CHIPTI__)
			//补齐画面
			int chnnIndex = chnnNum;
			unsigned char totalNum = SplitModeToNum(static_cast<VIEW_SPLIT_MODE>(splitMode));
			DISPLAY_ATTRIB attrib;
			memset(&attrib, 0, sizeof(DISPLAY_ATTRIB));
			for (int i = 0; i < totalNum; i++)
			{
				if(!bUsedIndex[i])
				{
					AreaInfo(pDispDevInfo->width, pDispDevInfo->height, static_cast<VIEW_SPLIT_MODE>(splitMode), attrib, i);
				
					attrib.index = i;

					pZorder[chnnIndex].zorder = 1;
					//pDispEx[chnnIndex] = attrib;

					pDispEx[chnnIndex].bShow = 0;
					pDispEx[chnnIndex].chnn = ~static_cast<unsigned short>(0);
					pDispEx[chnnIndex].xPos = attrib.xPos;
					pDispEx[chnnIndex].yPos = attrib.yPos;
					pDispEx[chnnIndex].cx = attrib.cx;
					pDispEx[chnnIndex].cy = attrib.cy;

#if defined(__TDFH__) || defined(__CHIPTI__)
					if (pDispDevInfo->width < (pDispEx[chnnIndex].xPos + pDispEx[chnnIndex].cx + 10))
					{
						pDispEx[chnnIndex].cx = (pDispDevInfo->width - pDispEx[chnnIndex].xPos);
					}
#endif
					chnnIndex++;
				}
			}
#endif

#ifdef __ENVIRONMENT_LINUX__
			if (VIDEO_OUT_CVBS == pDispDevInfo->videoType)
			{
#if defined(__TDFH__) || defined(__TDNXP__)
				ext_change_display_slave((DISPLAY_ATTRIB*)pDispEx, chnnIndex);
#else
				ext_change_display_slave((DISPLAY_ATTRIB*)pDispEx, chnnNum);
#endif
			}
			else
			{
#if defined(__TDFH__) || defined(__TDNXP__)
				ext_change_display_zorder(pZorder, chnnIndex);
				ext_change_display_format((DISPLAY_ATTRIB*)pDispEx, chnnIndex);
#else
				ext_change_display_zorder(pZorder, chnnNum);	
				ext_change_display_format((DISPLAY_ATTRIB*)pDispEx, chnnNum);
#endif
			}
#endif
			GUI::CFBInterface::Instance()->SetDisplayAttrib(pAttribTemp, chnnNum, pSplitMode, splitNum);
		}
		else
		{
			ULONGLONG authCH = CUserMan::Instance()->CheckAuthority(AUTH_LIVE);		
			for (unsigned char i=0; i<chnnNum; ++i)
			{
				if ((DISPLAY_LIVE == pAttribTemp[i].bShow) || (DISPLAY_NET == pAttribTemp[i].bShow))	//现场的才检查
				{
					if((pAttribTemp[i].chnn < m_videoInputNum) && pVisibleChnn && (pVisibleChnn[pAttribTemp[i].chnn] <= 0))
					{
						if(DISPLAY_NET == pAttribTemp[i].bShow)
						{
#ifdef __DVR_ULTIMATE__
							netCH &= ~((ULONGLONG)0x01 << pAttrib[i].chnn);
							netCHMinor &= ~((ULONGLONG)0x01 << pAttrib[i].chnn);
#endif
						}
						pAttribTemp[i].bShow = DISPLAY_INVISIBLE;//表示隐藏,设置不显示
						pDispEx[i].bShow = DISPLAY_HIDE;
					}
					else if (0 == (authCH & ((ULONGLONG)(0x01) << pAttribTemp[i].chnn)))
					{
						if(DISPLAY_NET == pAttribTemp[i].bShow)
						{
#ifdef __DVR_ULTIMATE__
							netCH &= ~((ULONGLONG)0x01 << pAttrib[i].chnn);
							netCHMinor &= ~((ULONGLONG)0x01 << pAttrib[i].chnn);
#endif
						}
						pAttribTemp[i].bShow = DISPLAY_HIDE;//表示隐藏
						pDispEx[i].bShow = DISPLAY_HIDE;
					}
					else if((0 == (g_net_channel & ((ULONGLONG)(0x01) << pAttribTemp[i].chnn))) \
						&& (DISPLAY_NET == pAttribTemp[i].bShow))
					{
#ifdef __DVR_ULTIMATE__
						netCH &= ~((ULONGLONG)0x01 << pAttrib[i].chnn);
						netCHMinor &= ~((ULONGLONG)0x01 << pAttrib[i].chnn);
#endif
						pAttribTemp[i].bShow = DISPLAY_INVISIBLE;
						pDispEx[i].bShow = DISPLAY_HIDE;
					}
					else
					{
						pDispEx[i].bShow = (pAttribTemp[i].bShow == DISPLAY_NET) ? DISPLAY_PLAYBACK : pAttribTemp[i].bShow;
					}
					if (!m_bCamVisible)
					{
						if(DISPLAY_NET == pAttribTemp[i].bShow)
						{
#ifdef __DVR_ULTIMATE__
							netCH &= ~((ULONGLONG)0x01 << pAttrib[i].chnn);
							netCHMinor &= ~((ULONGLONG)0x01 << pAttrib[i].chnn);
#endif
						}
						pAttribTemp[i].bShow = DISPLAY_HIDE;
						pDispEx[i].bShow = DISPLAY_HIDE;
					}
				}
			}

#ifdef __DVR_ULTIMATE__
			if(!bPlayback && (DISPLAY_DEV_MAJOR == pDispDevInfo->devMode))
			{
				CIPCDecDisplayMan::Instance()->GetDisplayCH(displayCH, displayCHMinor);
				if((displayCH != netCH) || (displayCHMinor != netCHMinor))
				{
					printf("netCH = %x, netCHMinor = %x, %s, %d\n", (unsigned long)netCH, (unsigned long)netCHMinor, __FILE__, __LINE__);
					CIPCDecDisplayMan::Instance()->SetDisplayCH(netCH, netCHMinor);
				}
			}
#endif

#if defined(__TDFH__) || defined(__TDNXP__)
			//补齐画面
			int chnnIndex = chnnNum;
			unsigned char totalNum = SplitModeToNum(static_cast<VIEW_SPLIT_MODE>(splitMode));
			DISPLAY_ATTRIB attrib;
			memset(&attrib, 0, sizeof(DISPLAY_ATTRIB));
			for (int i = 0; i < totalNum; i++)
			{
				if(!bUsedIndex[i])
				{
					AreaInfo(pDispDevInfo->width, pDispDevInfo->height, static_cast<VIEW_SPLIT_MODE>(splitMode), attrib, i);

					attrib.index = i;

					pZorder[chnnIndex].zorder = 1;
					//pDispEx[chnnIndex] = attrib;

					pDispEx[chnnIndex].bShow = 0;
					pDispEx[chnnIndex].chnn = ~static_cast<unsigned short>(0);
					pDispEx[chnnIndex].xPos = attrib.xPos;
					pDispEx[chnnIndex].yPos = attrib.yPos;
					pDispEx[chnnIndex].cx = attrib.cx;
					pDispEx[chnnIndex].cy = attrib.cy;

					chnnIndex++;
				}
			}
#endif

#ifdef __ENVIRONMENT_LINUX__
			if (VIDEO_OUT_CVBS == pDispDevInfo->videoType)
			{
				ext_change_display_slave((DISPLAY_ATTRIB*)pDispEx, chnnNum);
			}
			else
			{
				ext_change_display_zorder(pZorder, chnnNum);
				ext_change_display_format((DISPLAY_ATTRIB*)pDispEx, chnnNum);
			}
#endif
		}
	}
	
	delete [] pDispEx;
#ifdef __ENVIRONMENT_LINUX__
	delete [] pZorder;
#endif
	delete [] pAttribTemp;
}

bool CLocalDevice::HaveSpot()
{
	return CProduct::Instance()->HaveSpot();
}

void CLocalDevice::ChangeDisplaySpot(int chnn)
{
#ifdef __ENVIRONMENT_LINUX__
	if (CProduct::Instance()->UseMCUSpot())
	{
		if(m_bSupportMCU)
		{
			CMcu8952::Instance()->Operation(SERIAL_SPOT, chnn);
		}
	}
	else
	{
		ext_change_spot_display(chnn);
	}
#endif
}

void CLocalDevice::SetAudioLiveChannel(bool bOpen, unsigned char chnn)
{
// #ifdef __NETSERVER_HXHT__
// 	bOpen = false;
// #endif

	if (bOpen)
	{
		if (m_preLiveAudioChnn != chnn)
		{
			printf("%s,%d,open chnn:%d live audio,", __FILE__, __LINE__,chnn);	
			if (!m_bPlaybacking && !m_bTalking)
			{
				printf("ok\n");
#ifdef __ENVIRONMENT_LINUX__
				if(chnn < m_localVideoInputNum)
				{
					if(m_preLiveAudioChnn >= m_localVideoInputNum)
					{
#ifdef __DVR_ULTIMATE__
						ext_ip_in_audio_stop();
#endif
					}
					ext_set_audio_live_channel(1, chnn);
				}
				else
				{
					if(m_preLiveAudioChnn < m_localVideoInputNum)
					{
						ext_set_audio_live_channel(0, 0);
					}
#ifdef __DVR_ULTIMATE__
					ext_ip_in_audio_start(chnn - m_localVideoInputNum);
#endif
				}
#endif
				m_preLiveAudioChnn = chnn;
				m_bOpenAudio = true;
			}
			else
			{
				printf("error.because of playbacking or talking\n");
			}
		}
	}
	else
	{
		if (m_bOpenAudio)
		{
			printf("%s,%d,close live audio\n",__FILE__,__LINE__);
#ifdef __ENVIRONMENT_LINUX__
			if(m_preLiveAudioChnn >= m_localVideoInputNum)
			{
#ifdef __DVR_ULTIMATE__
				ext_ip_in_audio_stop();
#endif
			}
			else
			{
				ext_set_audio_live_channel(0, chnn);
			}
#endif
		}
		else
		{
			printf("%s,%d,no need close live audio.\n",__FILE__,__LINE__);
		}
		m_bOpenAudio = false;
		m_preLiveAudioChnn = -1;
	}

	CMessageMan::Instance()->SetAudioChnn(m_preLiveAudioChnn);
	CMessageMan::Instance()->RefreshLiveTrue();
}

void CLocalDevice::SetAudioVolume(int volume)
{
#ifdef __ENVIRONMENT_LINUX__
	ext_set_audio_output_volume((15*volume)/100);
#endif 
}

void CLocalDevice::RecoverAudioLiveChannel()
{
	if ((!m_bPlaybacking) && (!m_bTalking))
	{
		if (-1 == m_preLiveAudioChnn)
		{
			printf("%s,%d, recover to close live audio\n",__FILE__,__LINE__);
#ifdef __ENVIRONMENT_LINUX__
			ext_set_audio_live_channel(0, 0);
#endif
			m_bOpenAudio = false;
		}
		else
		{
			printf("%s,%d,recover to open chnn:%d live audio\n", __FILE__,__LINE__,m_preLiveAudioChnn);
#ifdef __ENVIRONMENT_LINUX__
			if(m_preLiveAudioChnn < m_localVideoInputNum)
			{
				ext_set_audio_live_channel(1, m_preLiveAudioChnn);
			}
			else
			{
#ifdef __DVR_ULTIMATE__
				ext_ip_in_audio_start(m_preLiveAudioChnn - m_localVideoInputNum);
#endif
			}
#endif
			m_bOpenAudio = true;
		}

		CMessageMan::Instance()->SetAudioChnn(m_preLiveAudioChnn);
	}
	else
	{
		printf("%s,%d,no recover live aduio,because of playbacking = %d or talking=%d\n",__FILE__,__LINE__, m_bPlaybacking, m_bTalking);
	}
}

void CLocalDevice::SetAudioAuto(bool bOpen)
{
	m_bAudioAuto = bOpen;
	printf("setAudioAuto:%d\n",m_bAudioAuto);
	
	//修改问题：声音自动功能在配置后，关闭配置界面后没有马上有效
	if (m_bAudioAuto && m_bOpenAudio)
	{
		if ( (-1 != m_bLiveOneChnn)&& \
			((m_bLiveOneChnn < CProduct::Instance()->AudioInputNum()) \
			|| (m_bLiveOneChnn > m_localVideoInputNum)) )
		{
			if (m_preLiveAudioChnn != m_bLiveOneChnn)
			{
				printf("%s,%d,set audioauto to open chnn:%d live audio.", __FILE__,__LINE__,m_bLiveOneChnn);
				if ((!m_bTalking) && (!m_bPlaybacking))
				{
					printf("ok\n");
#ifdef __ENVIRONMENT_LINUX__
					if(m_bLiveOneChnn < m_localVideoInputNum)
					{
						if(m_preLiveAudioChnn >= m_localVideoInputNum)
						{
#ifdef __DVR_ULTIMATE__
							ext_ip_in_audio_stop();
#endif
						}
						ext_set_audio_live_channel(1, m_bLiveOneChnn);
					}
					else
					{
						if(m_preLiveAudioChnn < m_localVideoInputNum)
						{
							ext_set_audio_live_channel(0, 0);
						}
#ifdef __DVR_ULTIMATE__
						ext_ip_in_audio_start(m_bLiveOneChnn - m_localVideoInputNum);
#endif
					}
#endif
				}
				else
				{
					printf("error.because of playbacking or talking\n");
				}

				m_preLiveAudioChnn = m_bLiveOneChnn;

				CMessageMan::Instance()->SetAudioChnn(m_preLiveAudioChnn);
			}		
		}
	}
}

void CLocalDevice::SetLEDStatus(DEVICE_LED_STATUS status, bool bOn)
{
#ifdef __ENVIRONMENT_LINUX__
	if (m_bSupportMCU)
	{
		if (LED_STATUS_HDD == status)
		{
			CMcu8952::Instance()->Operation(SERIAL_LAMP, bOn ? SERIAL_LAMP2_ON : SERIAL_LAMP2_OFF);
		}

		if (LED_STATUS_RECORD == status)
		{
			CMcu8952::Instance()->Operation(SERIAL_LAMP, bOn ? SERIAL_LAMP3_ON : SERIAL_LAMP3_OFF);
		}

		if (LED_STATUS_PLAY == status)
		{
			CMcu8952::Instance()->Operation(SERIAL_LAMP, bOn ? SERIAL_LAMP4_ON : SERIAL_LAMP4_OFF);
		}

		if (LED_STATUS_NET == status)
		{
			CMcu8952::Instance()->Operation(SERIAL_LAMP, bOn ? SERIAL_LAMP5_ON : SERIAL_LAMP5_OFF);
		}

		if (LED_STATUS_BACKUP == status)
		{
			CMcu8952::Instance()->Operation(SERIAL_LAMP, bOn ? SERIAL_LAMP6_ON : SERIAL_LAMP6_OFF);
		}
	}
	else
	{
		if (LED_STATUS_HDD == status)
		{
			ext_led_set(SERIAL_LAMP, bOn ? SERIAL_LAMP2_ON : SERIAL_LAMP2_OFF);
		}

		if (LED_STATUS_RECORD == status)
		{
			ext_led_set(SERIAL_LAMP, bOn ? SERIAL_LAMP3_ON : SERIAL_LAMP3_OFF);
		}

		if (LED_STATUS_PLAY == status)
		{
			ext_led_set(SERIAL_LAMP, bOn ? SERIAL_LAMP4_ON : SERIAL_LAMP4_OFF);
		}

		if (LED_STATUS_NET == status)
		{
			ext_led_set(SERIAL_LAMP, bOn ? SERIAL_LAMP5_ON : SERIAL_LAMP5_OFF);
		}

		if (LED_STATUS_BACKUP == status)
		{
			ext_led_set(SERIAL_LAMP, bOn ? SERIAL_LAMP6_ON : SERIAL_LAMP6_OFF);
		}
	}
#endif 
}

void CLocalDevice::SetLEDStatusEx(DEVICE_LED_STATUS status, unsigned long statusValue)
{
#ifdef __ENVIRONMENT_LINUX__
	if (m_bSupportMCU)
	{
		if (LED_STATUS_VIDEO_LOSS == status)
		{
			statusValue |= (~((1 << m_videoInputNum) - 1));

			CMcu8952::Instance()->Operation(SERIAL_SETVIDEOLOSSLAMP, statusValue);
			if (m_videoInputNum > 16)
			{
				CMcu8952::Instance()->Operation(SERIAL_SETVIDEOLOSSLAMP_H, statusValue);
			}
		}
	}
	else
	{

	}
#endif 
}

bool CLocalDevice::ChangeOutpuDevice(int device, int width, int height)
{
#ifdef __ENVIRONMENT_LINUX__
	if (0 == ext_change_output_device(&device, &width, &height))
	{
		return true;
	}
	else
	{
		return false;
	}
#else
	return true;
#endif
}

void CLocalDevice::SetCurMainDevice(int devColor)
{
	m_curDevice = devColor;

	m_curDevColorForPlayBack = GetCurMainDeviceDefaColor();

#ifdef __ENVIRONMENT_LINUX__
	printf("%s,%d, device=%d ext_set_voutdev_color,br:%d,co:%d,sa:%d,hu:%d\n",__FILE__,__LINE__, m_curDevice,
		((m_curDevColorForPlayBack & 0xff000000) >> 24),((m_curDevColorForPlayBack & 0x00ff0000) >> 16),((m_curDevColorForPlayBack & 0x0000ff00) >> 8),((m_curDevColorForPlayBack & 0x000000ff) >> 0));
	ext_set_voutdev_brightness(m_curDevice, ((m_curDevColorForPlayBack & 0xff000000) >> 24));
	ext_set_voutdev_contrast(m_curDevice, ((m_curDevColorForPlayBack & 0x00ff0000) >> 16));
	ext_set_voutdev_saturation(m_curDevice, ((m_curDevColorForPlayBack & 0x0000ff00) >> 8));
#ifndef __TDFH__
	ext_set_voutdev_hue(m_curDevice, ((m_curDevColorForPlayBack & 0x000000ff) >> 0));
#endif

#endif
}


int CLocalDevice::GetCurMainDevice()
{
	return m_curDevice;
}

unsigned long CLocalDevice::GetCurMainDeviceColor( )
{
	return m_curDevColorForPlayBack;
}

unsigned long CLocalDevice::GetCurMainDeviceDefaColor()
{
	unsigned long ulDeviceColor = 0;
	
	if (m_curDevice == VIDEO_OUT_HDMI)
	{
		ulDeviceColor = DEFAULT_HDMI_DEV_COLOR;
	}
	else if (m_curDevice == VIDEO_OUT_CVBS)
	{
		ulDeviceColor = DEFAULT_CVBS_DEV_COLOR;
	}
	else
	{
		ulDeviceColor = DEFAULT_VGA_DEV_COLOR;	
	}
	
#if defined(__CHIP3531__)
	if ((0 < m_localVideoInputNum) && (0 < m_netVideoInputNum))
	{
		if (m_curDevice == VIDEO_OUT_HDMI)
		{
			ulDeviceColor = ((96<<24) + (148<<16) + (122<<8) + 128);
		}
		else if (m_curDevice == VIDEO_OUT_CVBS)
		{
			ulDeviceColor = ((96<<24) + (148<<16) + (122<<8) + 128);
		}
		else
		{
			ulDeviceColor = ((96<<24) + (148<<16) + (122<<8) + 128);
		}
	}
#elif defined(__CHIP3520A__) || defined(__CHIP3520D__)
	if ((0 < m_localVideoInputNum) && (0 < m_netVideoInputNum))
	{
		if (m_curDevice == VIDEO_OUT_HDMI)
		{
			ulDeviceColor = ((128<<24) + (128<<16) + (128<<8) + 128);
		}
		else if (m_curDevice == VIDEO_OUT_CVBS)
		{
			ulDeviceColor = ((128<<24) + (128<<16) + (128<<8) + 128);
		}
		else
		{
			ulDeviceColor = ((128<<24) + (128<<16) + (128<<8) + 128);
		}
	}
#elif defined(__CHIP3521__)
	if ((0 < m_localVideoInputNum) && (0 < m_netVideoInputNum))
	{
		if (m_curDevice == VIDEO_OUT_HDMI)
		{
			ulDeviceColor = ((160<<24) + (139<<16) + (142<<8) + 126);
		}
		else if (m_curDevice == VIDEO_OUT_CVBS)
		{
			ulDeviceColor = ((160<<24) + (139<<16) + (142<<8) + 126);
		}
		else
		{
			ulDeviceColor = ((160<<24) + (142<<16) + (126<<8) + 128);
		}
	}
#endif

	return ulDeviceColor;
}

void CLocalDevice::SetMainDeviceColor(unsigned long devColor)
{
	if (devColor != m_curDevColorForPlayBack)
	{
		m_curDevColorForPlayBack = devColor;
#ifdef __ENVIRONMENT_LINUX__
		printf("%s,%d, ext_set_voutdev_color,dev=%d, br:%d,co:%d,sa:%d,hu:%d\n",__FILE__,__LINE__, m_curDevice,
			  ((devColor & 0xff000000) >> 24),((devColor & 0x00ff0000) >> 16),((devColor & 0x0000ff00) >> 8),((devColor & 0x000000ff) >> 0));
		ext_set_voutdev_brightness(m_curDevice, ((devColor & 0xff000000) >> 24));
		ext_set_voutdev_contrast(m_curDevice, ((devColor & 0x00ff0000) >> 16));
		ext_set_voutdev_saturation(m_curDevice, ((devColor & 0x0000ff00) >> 8));
#ifndef __TDFH__
		ext_set_voutdev_hue(m_curDevice, ((devColor & 0x000000ff) >> 0));
#endif

#endif
	}
}

void CLocalDevice::SetMainDeviceColor()
{
	bool bSet = false;
	unsigned long ulColor = GetCurMainDeviceDefaColor();

	if (m_curDevColorForPlayBack != ulColor)
	{
#ifdef __ENVIRONMENT_LINUX__
		printf("%s,%d, ext_set_voutdev_color,br:%d,co:%d,sa:%d,hu:%d\n",__FILE__,__LINE__,
			((m_curDevColorForPlayBack & 0xff000000) >> 24),((m_curDevColorForPlayBack & 0x00ff0000) >> 16),((m_curDevColorForPlayBack & 0x0000ff00) >> 8),((m_curDevColorForPlayBack & 0x000000ff) >> 0));

		ext_set_voutdev_brightness(m_curDevice, ((m_curDevColorForPlayBack & 0xff000000) >> 24));
		ext_set_voutdev_contrast(m_curDevice, ((m_curDevColorForPlayBack & 0x00ff0000) >> 16));
		ext_set_voutdev_saturation(m_curDevice, ((m_curDevColorForPlayBack & 0x0000ff00) >> 8));
#ifndef __TDFH__
		ext_set_voutdev_hue(m_curDevice, ((m_curDevColorForPlayBack & 0x000000ff) >> 0));
#endif
#endif
	}
}

void CLocalDevice::RecoverMainDeviceColor()
{
	unsigned long color = GetCurMainDeviceDefaColor();

	if (m_curDevColorForPlayBack != color)
	{
#ifdef __ENVIRONMENT_LINUX__
		printf("%s,%d, ext_set_voutdev_color,br:%d,co:%d,sa:%d,hu:%d\n",__FILE__,__LINE__,
			((color & 0xff000000) >> 24),((color & 0x00ff0000) >> 16),((color & 0x0000ff00) >> 8),((color & 0x000000ff) >> 0));

		ext_set_voutdev_brightness(m_curDevice, ((color & 0xff000000) >> 24));
		ext_set_voutdev_contrast(m_curDevice, ((color & 0x00ff0000) >> 16));
		ext_set_voutdev_saturation(m_curDevice, ((color & 0x0000ff00) >> 8));
#ifndef __TDFH__
		ext_set_voutdev_hue(m_curDevice, ((color & 0x000000ff) >> 0));
#endif
#endif
	}
}

#ifdef __ENVIRONMENT_LINUX__
extern bool g_bUse1104A1108ChipColor;
#endif

unsigned long CLocalDevice::GetImageDefaultColor()
{
	if ((0 != m_imageDefaultColor) && (VIDEO_FORMAT_NONE != m_videoFormatFlash))
	{
		return m_imageDefaultColor;
	}

	VIDEO_FORMAT videoFormatTemp = (VIDEO_FORMAT)GetVideoFormatFromFlash();

#ifdef __ENVIRONMENT_LINUX__
#if defined(__SPECIAL_COLOR)
	m_imageDefaultColor = (VIDEO_FORMAT_NTSC == videoFormatTemp) ? DEFAULT_COLOR_NTSC_25838 : DEFAULT_COLOR_PAL_25838;
#else
	if (g_bUse1104A1108ChipColor)
	{
		m_imageDefaultColor = (VIDEO_FORMAT_NTSC == videoFormatTemp) ? DEFAULT_COLOR_NTSC_1104 : DEFAULT_COLOR_PAL_1104;
	}
	else
	{
		m_imageDefaultColor = (VIDEO_FORMAT_NTSC == videoFormatTemp) ? DEFAULT_COLOR_NTSC_2864 : DEFAULT_COLOR_PAL_2864;
	}
#endif

#if defined(__CHIP3520A__) || defined(__CHIP3521__) || defined(__HISI_SDK_0BX__) || defined(__CHIPGM__) || defined(__CHIP3520D__)
	if (true == m_bInitial)
	{
#if defined(__CHIPGM__)
		if (0 == ext_get_video_Def_Color(&m_bright, &m_hue, &m_sat, &m_contrast))
#else
		m_bright = 128;
		m_hue = 128;
		m_sat = 128;
		m_contrast =128;
#endif
		{
			m_imageDefaultColor = (m_bright << 24) + (m_contrast << 16) + (m_sat << 8) + m_hue;
		}
	}
#endif
#else
	m_imageDefaultColor = (VIDEO_FORMAT_NTSC == videoFormatTemp) ? DEFAULT_COLOR_NTSC_2864 : DEFAULT_COLOR_PAL_2864;
#endif

	m_bright	= (m_imageDefaultColor >> 24);
	m_contrast	= (m_imageDefaultColor >> 16);
	m_sat		= (m_imageDefaultColor >> 8);
	m_hue		= m_imageDefaultColor;


	return m_imageDefaultColor;
}


unsigned char CLocalDevice::GetRealImageColor(unsigned char defaultColor, unsigned char setToColor)
{
	if (128 > setToColor)
	{
		return defaultColor * setToColor / 128;
	}
	else if (128 == setToColor)
	{
		return defaultColor;
	}
	else
	{
		return ((254 - defaultColor) * (setToColor - 129)) / 126 + defaultColor + 1;
	}
}



void CLocalDevice::KeyFrame(unsigned char chnn, bool bLocal)
{
#ifdef __ENVIRONMENT_LINUX__
	if (bLocal)
	{
		if (0 != ext_set_local_keyframe((int)chnn))
		{
			/*PUB_PrintError(__FILE__,__LINE__);
			return;*/
		}
	}
	else
	{
		if (0 != ext_set_net_keyframe((int)chnn))
		{
			/*PUB_PrintError(__FILE__,__LINE__);
			return;*/
		}
	}
#endif

}
//////////////////////////////////////////////////////////////////////////
bool CLocalDevice::GetVideoFormat(unsigned char chnn, BITMAPINFOHEADER &bitmap, bool bLocal /* = true */)
{
#ifdef __ENVIRONMENT_WIN32__
	return CDevControlVirtual::Instance()->GetVideoFormat(bitmap);
#endif

#ifdef __ENVIRONMENT_LINUX__
	if (bLocal)
	{
		if (0 != ext_get_video_format(m_pLocalVideoSize[chnn], &bitmap))
		{
			PUB_PrintError(__FILE__, __LINE__);
			return false;
		}
	}
	else
	{
		if (0 != ext_get_video_format(m_pLocalNetVideoSize[chnn], &bitmap))
		{
			PUB_PrintError(__FILE__,__LINE__);
			return false;
		}

	}
	return true;
#endif
}

bool CLocalDevice::GetAudioFormat(WAVEFORMATEX &wave)
{
#ifdef __ENVIRONMENT_WIN32__
	return CDevControlVirtual::Instance()->GetAudioFormat(wave);
#endif

#ifdef __ENVIRONMENT_LINUX__
	if (0 != ext_get_audio_format(&wave))
	{
		PUB_PrintError(__FILE__,__LINE__);
		return false;
	}
#if !defined(__CHIPGM__)
	wave.wFormatTag = m_waveFormat;
#else
	if (TD_2704AS_SL != CProduct::Instance()->ProductType())
	{
		wave.wFormatTag = m_waveFormat;
	}
#endif
	return true;
#endif
}


bool CLocalDevice::GetKeyInput(KEY_VALUE_INFO &keyValue)
{
#ifdef __ENVIRONMENT_LINUX__
	//2010-01-29 10:04:00 YSW
	//先检查鼠标，然后检查面板按键和遥控器按键

//#ifndef __CHIPTI__
#if 1
	if (0 == ext_mouse_get_status(&keyValue))
	{
		return true;
 	}

#else

	m_keyLock.Lock();
	if (m_keyValueList.size())
	{
		keyValue = m_keyValueList.front();
		m_keyValueList.pop_front();
		m_keyLock.UnLock();
		return true;
	}
	m_keyLock.UnLock();

#endif

	//读面板按键和遥控器按键值
	if (m_bSupportMCU)
	{
		if(CMcu8952::Instance()->GetMCUKeyInfo( &keyValue))
		{
			//如果是遥控器，则需要送入遥控器处理模块
			if( KEY_TYPE_REMOTE == keyValue.type || KEY_TYPE_REMOTE_TR12 == keyValue.type)
			{
				return CRemoteControl::Instance()->RemoteControlDVR( keyValue );
			}
			else
			{
				return true;
			}
		}
	}
	else
	{
		if(0 == ext_keyboard_value( &keyValue))
		{
			//如果是遥控器，则需要送入遥控器处理模块
			if(KEY_TYPE_REMOTE == keyValue.type)
			{
#if defined(__TW01_RILI__)
				return false;
#endif
				#ifdef __REMOTE_TR12__
					keyValue.type = KEY_TYPE_REMOTE_TR12;
					keyValue.value= (keyValue.value & 0xffffff7f);
				#endif
				return CRemoteControl::Instance()->RemoteControlDVR( keyValue );
			}
			else
			{
				return true;
			}
		}
	}

	//最后检查串口控制设备的输入信息
	if(CExternalKeyboard::Instance()->GetExKeyboardValue( &keyValue ))
	{
		return true;
	}

	return false;
#endif
	return false;
}

void CLocalDevice::SetResolution(unsigned short width, unsigned short height)
{
	m_displayWidth = width;
	m_displayHeight = height;
}

int CLocalDevice::GetVideo_F(BYTE chnn, FRAME_INFO_EX &frameInfo)
{
#ifdef __ENVIRONMENT_LINUX__
	int ret = ext_get_primary_video_stream((int)chnn, &frameInfo);
	if (110 == ret)
	{
		for (unsigned char chnn = 0; chnn < m_localVideoInputNum; ++chnn)
		{
			ext_change_local_video_size((int)chnn, m_pLocalVideoSize[chnn]);
		}		
	}

	if (0 == ret)
	{
		return GET_FRAME_SUCC;
	}
	else if (3 == ret)
	{
		return FRAME_BUF_FULL;
	}
	else
	{
		return GET_FRAME_FAIL;
	}
#endif

#ifdef __ENVIRONMENT_WIN32__
	return CDevControlVirtual::Instance()->GetPrimaryVideoStream(chnn, frameInfo) ? GET_FRAME_SUCC : GET_FRAME_FAIL;
#endif
}

int CLocalDevice::GetVideo_S(BYTE chnn, FRAME_INFO_EX &frameInfo)
{
#ifdef __ENVIRONMENT_LINUX__
	int ret = ext_get_network_video_stream((int)chnn, &frameInfo);
	if (110 == ret)
	{
		for (unsigned char chnn = 0; chnn < m_localVideoInputNum; ++chnn)
		{
			ext_change_net_video_size((int)chnn, m_pLocalNetVideoSize[chnn]);
		}		
	}

	if (0 == ret)
	{
		return GET_FRAME_SUCC;
	}
	else if (3 == ret)
	{
		return FRAME_BUF_FULL;
	}
	else
	{
		return GET_FRAME_FAIL;
	}
#else
	return GET_FRAME_FAIL;
#endif
}

int CLocalDevice::GetJPEGStream(FRAME_INFO_EX &frameInfo)
{
#ifdef __ENVIRONMENT_LINUX__
	int ret = ext_get_jpeg_stream(&frameInfo);

	if (0 == ret)
	{
		return GET_FRAME_SUCC;
	}	
	else
	{
		return GET_FRAME_FAIL;
	}   
#else	//__ENVIRONMENT_WIN32__
	return GET_FRAME_FAIL; 	
#endif

}

int CLocalDevice::GetAudio(BYTE chnn, FRAME_INFO_EX &frameInfo)
{
#ifdef __ENVIRONMENT_LINUX__
	int ret = ext_get_audio_stream(chnn, &frameInfo);

	if (0 == ret)
	{
		return GET_FRAME_SUCC;
	}
	else if (3 == ret)
	{
		return FRAME_BUF_FULL;
	}
	else
	{
		return GET_FRAME_FAIL;
	}   
#else	//__ENVIRONMENT_WIN32__
	//return CDevControlVirtual::Instance()->GetAudioStream(chnn, frameInfo) ? GET_FRAME_SUCC : GET_FRAME_FAIL;
	return GET_FRAME_FAIL;
#endif
}
//////////////////////////////////////////////////////////////////////////
void CLocalDevice::ReleaseBuf(FRAME_INFO_EX &frameInfo)
{
	
#ifdef __ENVIRONMENT_LINUX__
	if (0 != ext_release_buf(&frameInfo))
	{
		assert(false);
	}
#endif

#ifdef __ENVIRONMENT_WIN32__
	CDevControlVirtual::Instance()->ReleaseFrame(frameInfo);
#endif
}


void CLocalDevice::ReleaseJPEGBuf(FRAME_INFO_EX &frameInfo)
{
#ifdef __ENVIRONMENT_LINUX__
	if (0 != ext_release_jpeg_buf(&frameInfo))
	{
		assert(false);
	}
#endif
}

int CLocalDevice::ClearBuffer()
{
#ifdef __ENVIRONMENT_LINUX__
	return ext_clear_buffer();
#else
	return 0;
#endif
}


//////////////////////////////////////////////////////////////////////////
void CLocalDevice::GetMotionStatus(ULONGLONG &status)
{
#ifdef __ENVIRONMENT_LINUX__
	ext_get_video_md_alarm(&status);
#else
	status = 0;//ALL_CHNN_MASK(m_localVideoInputNum);
#endif
}

void CLocalDevice::GetVideoLossStatus(ULONGLONG &status)
{
#ifdef __ENVIRONMENT_LINUX__
	ext_get_video_loss_status(&status);

	if(0 == m_localVideoInputNum)
	{
		return;
	}

	unsigned long value = (unsigned long )(status & 0x00000000ffffffff);
	value |= (~((1 << m_localVideoInputNum) - 1));

	if( m_videoLossStatus != value )
	{
		//视频丢失状态改变的时候从新设置指示灯状态
		SetLEDStatusEx(LED_STATUS_VIDEO_LOSS, value);
		m_videoLossStatus = value;
	}

	//2304SE_C做的HYBIRD，数字通道一直显示视频丢失
	status = (status & ((1 << m_localVideoInputNum) - 1));
#else
	status = 0;
#endif
}

void CLocalDevice::GetSensorInStatus(ULONGLONG &status)
{
#ifdef __ENVIRONMENT_LINUX__
	if (m_bSupportMCU)
	{
		status = CMcu8952::Instance()->GetAlarm();
	}
	else
	{
		ext_alarm_status_get(&status);
	}
#else
	status = ALL_CHNN_MASK(m_localVideoInputNum);
#endif

	
	 //2704特有
	if (TD_2704HD == m_productType)
	{
		ULONGLONG temp = status >> 8;
		status = 0xffffffff & temp;	
	}	

}

void CLocalDevice::SetSensorOutStatus(unsigned char chnn, bool bOpen)
{
#ifdef __ENVIRONMENT_LINUX__
#if defined(__CUSTOM_TW01__) || defined(__CUSTOM_TW01TS__) || defined(__CUSTOM_CBC__)
	m_sensorStatLock.Lock();
	bool bIsCtrlSensor = false;
	if (m_bSensorOutStatus[chnn])
	{
		if (bOpen)
		{
			if (m_SenorOutOpenCout[chnn] > 10)
			{
				m_SenorOutOpenCout[chnn] = 0;				
				bIsCtrlSensor = true;
			}
			else
			{
				m_SenorOutOpenCout[chnn]++;
				bIsCtrlSensor = false;
			}			
		}
		else
		{
			m_bSensorOutStatus[chnn] = 0;
			m_SenorOutOpenCout[chnn] = 0;
			bIsCtrlSensor = true;		
		}
	}
	else
	{
		if (bOpen)
		{
			m_bSensorOutStatus[chnn] = 1;
			m_SenorOutOpenCout[chnn] = 1;
			bIsCtrlSensor = true;					
		}
		else
		{			
			m_SenorOutOpenCout[chnn] = 0;
			bIsCtrlSensor = true;
		}
	}
	m_sensorStatLock.UnLock();

	if (bIsCtrlSensor)
	{
		//printf("!!!%s,%d, sensor:%d, %d\n",__FILE__,__LINE__,chnn,bOpen);
		if (m_bSupportMCU)
		{
			BYTE relayCtrl = (1 << chnn);
			if( !bOpen )
			{
				relayCtrl = (( ~relayCtrl ) & 0x8f);
			}
			CMcu8952::Instance()->Operation(SERIAL_RELAY, relayCtrl);
		}
		else
		{
			ext_alarm_output_set(chnn, bOpen ? 1 : 0);
		}
	}


	//m_sensorStatLock.Lock();
	//if (m_bSensorOutStatus[chnn])
	//{
	//	if (bOpen)
	//	{
	//		if (m_SenorOutOpenCout[chnn] > 10)
	//		{
	//			m_SenorOutOpenCout[chnn] = 0;
	//			printf("!!!!!!!!!%s,%d,open again :chnn:%d\n",__FILE__,__LINE__,chnn);
	//			if (m_bSupportMCU)
	//			{		
	//				BYTE relayCtrl = (1 << chnn);				
	//				CMcu8952::Instance()->Operation(SERIAL_RELAY, relayCtrl);
	//			}
	//			else
	//			{
	//				ext_alarm_output_set(chnn, bOpen ? 1 : 0);
	//			}	
	//		}
	//		m_SenorOutOpenCout[chnn]++;
	//	}
	//	else
	//	{
	//		m_bSensorOutStatus[chnn] = 0;
	//		m_SenorOutOpenCout[chnn] = 0;
	//		printf("!!!!!!!!!!%s,%d,close:chnn:%d\n",__FILE__,__LINE__,chnn);
	//		if (m_bSupportMCU)
	//		{						
	//			BYTE relayCtrl = (1 << chnn);
	//			relayCtrl = (( ~relayCtrl ) & 0x8f);				
	//			CMcu8952::Instance()->Operation(SERIAL_RELAY, relayCtrl);
	//		}
	//		else
	//		{
	//			ext_alarm_output_set(chnn, bOpen ? 1 : 0);
	//		}
	//	}
	//}
	//else
	//{
	//	if (bOpen)
	//	{
	//		m_bSensorOutStatus[chnn] = 1;
	//		m_SenorOutOpenCout[chnn] = 1;
	//		printf("!!!!!!!!!%s,%d,open:chnn:%d\n",__FILE__,__LINE__,chnn);
	//		if (m_bSupportMCU)
	//		{		
	//			BYTE relayCtrl = (1 << chnn);				
	//			CMcu8952::Instance()->Operation(SERIAL_RELAY, relayCtrl);
	//		}
	//		else
	//		{
	//			ext_alarm_output_set(chnn, bOpen ? 1 : 0);
	//		}			
	//	}
	//	else
	//	{
	//		printf("!!!!!!!!!!%s,%d,close again:chnn:%d\n",__FILE__,__LINE__,chnn);
	//		m_SenorOutOpenCout[chnn] = 0;
	//		if (m_bSupportMCU)
	//		{						
	//			BYTE relayCtrl = (1 << chnn);
	//			relayCtrl = (( ~relayCtrl ) & 0x8f);				
	//			CMcu8952::Instance()->Operation(SERIAL_RELAY, relayCtrl);
	//		}
	//		else
	//		{
	//			ext_alarm_output_set(chnn, bOpen ? 1 : 0);
	//		}

	//	}

	//}
	//m_sensorStatLock.UnLock();
#else
	if (m_bSupportMCU)
	{
		BYTE relayCtrl = (1 << chnn);
		if( !bOpen )
		{
			relayCtrl = (( ~relayCtrl ) & 0x8f);
		}
		CMcu8952::Instance()->Operation(SERIAL_RELAY, relayCtrl);
	}
	else
	{
		ext_alarm_output_set(chnn, bOpen ? 1 : 0);
	}
#endif
#endif
}

void CLocalDevice::SetBuzzerStatus(bool bOpen)
{
#ifdef __ENVIRONMENT_LINUX__
	if (m_bSupportMCU)
	{
		CMcu8952::Instance()->Operation(SERIAL_BUZZER, bOpen ? SERIAL_BUZZER_ON : SERIAL_BUZZER_OFF);
	}
	else
	{
#if defined(__ZNV_SC__)
		ext_buzzer_set(bOpen ? 2 : 0);
#else
		ext_buzzer_set(bOpen ? 1 : 0);
#endif
	}
#endif
}

void CLocalDevice::SetVideoFormat(VIDEO_FORMAT videoFormat)
{
#ifdef __ENVIRONMENT_LINUX__

#endif
}

void CLocalDevice::SetOtherAlarm(OTHER_ALARM_TYPE type, bool bOpen)
{
#ifdef __ENVIRONMENT_LINUX__
	if (bOpen)
	{
		m_otherAlarmStatus |= static_cast<ULONGLONG>(0x01) << type;
	}
	else
	{
		m_otherAlarmStatus &= ~(static_cast<ULONGLONG>(0x01) << type);
	}
#endif
}

void CLocalDevice::GetOtherAlarm(ULONGLONG & status)
{
#ifdef __ENVIRONMENT_LINUX__
	status = m_otherAlarmStatus;
#else
	status = 0;
#endif
}
//////////////////////////////////////////////////////////////////////////
void CLocalDevice::SetVideoSize(unsigned char chnn, unsigned long size, bool bLocal/* = true*/)
{
	if (bLocal)
	{
		CEncodeCheck::Instance()->SetChannelRes(chnn, size);
		CCalculateSubEncode::Instance()->SetChannelRes(chnn, size);
	}

#ifdef __ENVIRONMENT_LINUX__
	if (bLocal)
	{
		assert(m_pLocalVideoSize != NULL);
		assert(chnn < m_localVideoInputNum);
		m_pLocalVideoSize[chnn] = (VIDEO_SIZE)size;
		if (0 != ext_change_local_video_size(chnn, size))
		{
			PUB_PrintError(__FILE__,__LINE__);
		}
		//printf("set chnn:%02d local video size:%d\n", chnn, size);
	}
	else
	{
		assert(m_pLocalNetVideoSize != NULL);
		assert(chnn < m_localVideoInputNum);
		if (0 != ext_change_net_video_size(chnn, size))
		{
			printf("%s:%s:%d, Change video size %d error\n", __FUNCTION__, __FILE__, __LINE__, size);
			PUB_PrintError(__FILE__,__LINE__);
		}
		else
		{
			m_pLocalNetVideoSize[chnn] = (VIDEO_SIZE)size;
		}
		//printf("set chnn:%02d net    video size:%d\n", chnn, size);
	}
#endif
}

unsigned long CLocalDevice::GetVideoSize(unsigned char chnn, bool bLocal)
{
#ifdef __ENVIRONMENT_LINUX__
	if (bLocal)
	{
		if ((m_pLocalVideoSize != NULL) && (chnn < m_localVideoInputNum))
		{
			return m_pLocalVideoSize[chnn];
		}
		else
		{
			assert(m_pLocalVideoSize != NULL);
			assert(chnn < m_localVideoInputNum);
			return 0;
		}
	}
	else
	{
		if ((m_pLocalNetVideoSize != NULL) && (chnn < m_localVideoInputNum))
		{
			return m_pLocalNetVideoSize[chnn];
		}
		else
		{
			assert(m_pLocalNetVideoSize != NULL);
			assert(chnn < m_localVideoInputNum);
			return 0;
		}
	}
#else
	return 0;
#endif
}

//void CLocalDevice::SetFrameRate(unsigned char chnn, unsigned char rate, bool bLocal/* = true*/)
//{
//#ifdef __ENVIRONMENT_LINUX__
//	if (bLocal)
//	{
//
//		if (0 != ext_change_local_frame_rate(chnn, rate))
//		{
//			PUB_PrintError(__FILE__,__LINE__);
//		}
//		//printf("set chnn:%02d local frame rate:%d\n", chnn, rate);
//	}
//	else
//	{
//		if (0 != ext_change_net_frame_rate(chnn, rate))
//		{
//			PUB_PrintError(__FILE__,__LINE__);
//		}
//		//printf("set chnn:%02d net    frame rate:%d\n", chnn, rate);
//	}
//#endif
//}
//
//void CLocalDevice::SetVideoQuality(unsigned char chnn, VIDEO_ENCODE_MODE type, int bitStream, int quality, bool bLocal/* = true*/)
//{
//#ifdef __ENVIRONMENT_LINUX__
//	if (bLocal)
//	{
//		if (0 != ext_change_local_img_quality(chnn, type, bitStream, quality))
//		{
//			PUB_PrintError(__FILE__,__LINE__);
//		}
//		//printf("set chnn:%02d local encode:%d, bitstream:%d, quality:%d\n", 
//		//	 chnn, type, bitStream, quality);
//	} 
//	else
//	{
//		if (0 != ext_change_net_img_quality(chnn, type, bitStream, quality))
//		{
//			PUB_PrintError(__FILE__,__LINE__);
//		}
//		//printf("set chnn:%02d net    encode:%d, bitstream:%d, quality:%d\n", 
//		//	chnn, type, bitStream, quality);
//	}
//#endif
//}

void CLocalDevice::SetFrameRateQuality(unsigned char chnn, unsigned char rate,VIDEO_ENCODE_MODE type, int bitStream, int quality, bool bLocal /* = true */)
{
#ifdef __ENVIRONMENT_LINUX__
#ifdef __TD2316SES_SPEC__
	type = VIDEO_ENCODE_MODE_CBR;
#endif
	if (bLocal)
	{
		if (0 != ext_change_local_frame_quality(chnn, rate, type, bitStream, quality))
		{
			PUB_PrintError(__FILE__,__LINE__);
		}
		printf("set chnn:%02d local rate:%d, encode:%d, bitstream:%d, quality:%d\n", 
			 chnn, rate, type, bitStream, quality);
	} 
	else
	{
		if (0 != ext_change_net_frame_quality(chnn, rate, type, bitStream, quality))
		{
			printf("set chnn:%02d net    rate:%d, encode:%d, bitstream:%d, quality:%d\n", chnn, rate, type, bitStream, quality);
			PUB_PrintError(__FILE__,__LINE__);
		}
	}
#endif

}

void CLocalDevice::SetMotionInfo(unsigned char chnn, const MOTION_DETECT_INFO *pInfo)
{
#ifdef __ENVIRONMENT_LINUX__
	ext_set_motion_sensitive(chnn, pInfo->sensitivity);

	ext_set_motion_area(chnn, (MOTION_AREA_INFO *)(&pInfo->areaInfo));
#endif
}

void CLocalDevice::SetImageColor(unsigned char chnn, unsigned long color)
{
	if ((0 < m_localVideoInputNum) && (0 < m_netVideoInputNum))
	{
		GetImageDefaultColor();

#ifdef __ENVIRONMENT_LINUX__
		unsigned char rColor = GetRealImageColor(m_bright, (color & 0xff000000) >> 24);
		ext_set_video_brightness(chnn, rColor);

		rColor = GetRealImageColor(m_contrast, (color & 0x00ff0000) >> 16);
		ext_set_video_contrast(chnn, rColor);
		
		rColor = GetRealImageColor(m_sat, (color & 0x0000ff00) >> 8);
		ext_set_video_saturation(chnn, rColor);
		
		rColor = GetRealImageColor(m_hue, (color & 0x000000ff) >> 0);
		ext_set_video_hue(chnn, rColor);
#else
		char rColor = GetRealImageColor(m_bright, (color & 0xff000000) >> 24);
		rColor = GetRealImageColor(m_bright, (color & 0x00ff0000) >> 16);
		rColor = GetRealImageColor(m_bright, (color & 0x0000ff00) >> 8);
		rColor = GetRealImageColor(m_bright, (color & 0x000000ff) >> 0);
#endif
	}
	else
	{
#ifdef __ENVIRONMENT_LINUX__
		ext_set_video_brightness(chnn, ((color & 0xff000000) >> 24));
		ext_set_video_contrast(chnn, ((color & 0x00ff0000) >> 16));
		ext_set_video_saturation(chnn, ((color & 0x0000ff00) >> 8));
		ext_set_video_hue(chnn, ((color & 0x000000ff) >> 0));
#endif
	}
}


void CLocalDevice::InitOSDTimeStamp(OSD_ATTRIB *pAttrib/* = NULL*/)
{
#ifdef __ENVIRONMENT_LINUX__
		assert (NULL != pAttrib);

		//下面是打印0到10的点阵图
		/*{
			unsigned char *pStr = pAttrib->pData ;

			unsigned short FONT_INFO_BUF_LEN = 4 + ((60 * 20) >> 3);

			for (int i = 0; i < 10; i++)
			{
				unsigned long fontInfo = 0;
				memcpy(&fontInfo, pStr, sizeof(unsigned long));

				unsigned short cy = static_cast<unsigned short>(fontInfo & 0x000000ff); 
				unsigned short cx = static_cast<unsigned short>(fontInfo & 0x0000ff00) >> 8;
				unsigned long charID = (fontInfo & 0xffff0000) >> 16;
				printf("!!!!!!!!charID:%d,cx:%d,cy:%d\n",charID,cx,cy);

				unsigned char *pFontStyle = pStr + sizeof(unsigned long);


				bool bBord = false;
				unsigned long index = 0;
				unsigned char style = 0;

				for (short y=0; y<cy; ++y)
				{
					for (short x=0; x<cx; ++x)
					{
						index = y*cx+x;
						style = pFontStyle[index >> 3];
						if (style & (0x01 << (index & 0x0007)))
						{
							printf("1");
						}
						else
						{
							printf(" ");
						}
					}
					printf("\n");
				}


				pStr+=FONT_INFO_BUF_LEN;
			}
		}*/

		ext_rec_osd_print(0, OSD_TIME_STAMP, pAttrib);
#endif
}

void CLocalDevice::RefreshTimeStamp(const std::string & strTime)
{
#ifdef __ENVIRONMENT_LINUX__
	for (unsigned char i=0; i<m_localVideoInputNum; ++i)
	{
		if (m_pOSDInfo[i].bEnable)
		{
			m_timeStampAttrib.cy = 20;
			m_timeStampAttrib.cx = 20 * strTime.length();
			m_timeStampAttrib.xPos = m_pOSDInfo[i].x;
			m_timeStampAttrib.yPos = m_pOSDInfo[i].y;
			m_timeStampAttrib.length = strTime.length();
			strcpy((char *)m_timeStampAttrib.pData, strTime.c_str());
			ULONGLONG curTime = GetCurrTime();

			unsigned long * pTime = (unsigned long *)(m_timeStampAttrib.pData + ((m_timeStampAttrib.length + 7) & (~7)));
			
			pTime++;
			* pTime = curTime / 1000000;
			pTime++;
			* pTime = curTime % 1000000;

			ext_rec_osd_print(i, OSD_TIME_STAMP, &m_timeStampAttrib);
		}
		else
		{
			ext_rec_osd_clean(i, OSD_TIME_STAMP);
		}
	}
#endif
}

void CLocalDevice::SetOSDTimeStamp(unsigned char chnn, const OSD_INFO *pOSDInfo)
{
#ifdef __ENVIRONMENT_LINUX__
	assert (chnn < m_localVideoInputNum);
	assert (NULL != pOSDInfo);
	assert (NULL != m_pOSDInfo);

	m_pOSDInfo [chnn] = *pOSDInfo;
#endif
}

void CLocalDevice::SetOSDCamName(unsigned char chnn, bool bShow, OSD_ATTRIB *pAttrib/* = NULL*/)
{
#ifdef __ENVIRONMENT_LINUX__
	if (bShow)
	{
		assert (NULL != pAttrib);
		ext_rec_osd_print(chnn, OSD_CAMERA_NAME, pAttrib);
	}
	else
	{
		ext_rec_osd_clean(chnn, OSD_CAMERA_NAME);
	}
#endif
}

void CLocalDevice::SetCamCover(unsigned char chnn, const CAMERA_COVER_INFO *pCoverInfo)
{
#ifdef __ENVIRONMENT_LINUX__
	assert (NULL != pCoverInfo);
	assert (chnn < m_localVideoInputNum);

	int areaMax = g_coverAreaCountLimit;

	//先清除
	for (unsigned char i=0; i<areaMax; ++i)
	{
		ext_clean_cover_area(chnn, i);
	}

	//再设置
	for (unsigned char i=0; i<areaMax; ++i)
	{
		if (pCoverInfo->area[i].bCover)
		{
			ext_set_cover_area(chnn, i, pCoverInfo->area[i].x, pCoverInfo->area[i].y, pCoverInfo->area[i].cx, pCoverInfo->area[i].cy, pCoverInfo->area[i].color);
		}
	}
#endif
}

void CLocalDevice::Zoom(bool bZoom, int x/* = 0*/, int y/* = 0*/, int w/* = 0*/, int h/* = 0*/)
{
#ifdef __ENVIRONMENT_LINUX__
	ext_vidoe_image_zoom(bZoom ? 1 : 0, x, y, w, h);
#endif
}

void CLocalDevice::SetWaterMark(int chnn, unsigned char* key, int key_len, unsigned char* user_info, int user_len)
{
#ifdef __ENVIRONMENT_LINUX__
	ext_rec_water_mark_set(chnn, key, key_len, user_info, user_len);
#endif
}

void CLocalDevice::CleanWaterMark(int chnn)
{
#ifdef __ENVIRONMENT_LINUX__
	ext_rec_water_mask_clean(chnn);
#endif
}

void CLocalDevice::SetDeviceNomenuImage(unsigned char *pData, unsigned long width, unsigned long height)
{
#ifdef __ENVIRONMENT_LINUX__
	int imgbits   = 2;		
	ext_set_output_nomenu_image((char*)pData, width, height, imgbits);		
#endif
}

void CLocalDevice::PlaybackStop(ULONGLONG stopCH)
{
#ifdef __ENVIRONMENT_LINUX__
	//printf("PlaybackStop %s,%d\n", __FILE__,__LINE__);
	ext_playback_stop();
	m_bPlaybacking = false;

#else
	ULONGLONG channelMask = 0x01;
	for(long channel = 0; (channel < 64) && ((channelMask << channel) <= m_pbChannelBits); channel++)
	{
		if((channelMask << channel) & m_pbChannelBits & stopCH)
		{
			m_pDataDisplay->Stop(channel);
		}
	}
#endif
}
bool CLocalDevice::PlaybackStart(ULONGLONG chnnBits,bool bRestart)
{
#ifdef __ENVIRONMENT_LINUX__
	if (m_bOpenAudio)
	{
		printf("%s,%d,playback to close audio\n",__FILE__,__LINE__);
		if (m_preLiveAudioChnn < CProduct::Instance()->AudioInputNum() && (0 != ext_set_audio_live_channel(0, 0)))
		{
//			return false;
		}
#ifdef __DVR_ULTIMATE__
		if (m_preLiveAudioChnn >= CProduct::Instance()->LocalVideoInputNum() && (0 != ext_ip_in_audio_stop()))
		{
//			return false;
		}
#endif

		m_bOpenAudio = false;
	}
	else
	{
		printf("%s,%d, live audio is closed\n",__FILE__,__LINE__);
	}
	
	int iRet = 0;
	if (bRestart)
	{
		iRet = ext_playback_restart(chnnBits);
	}
	else
	{
		iRet = ext_playback_start(chnnBits);
	}
	
	if (0 != iRet)
	{
		printf("%s:%s:%d, Playback start error, return:%d.\n", __FUNCTION__, __FILE__, __LINE__, iRet);
		return false;
	}
	else
	{
		printf("%s:%s:%d, Playback start OK.\n", __FUNCTION__, __FILE__, __LINE__);
	}

	m_bPlaybacking = true;

	return true;

#else

	ULONGLONG channelMask = 0x01;
	int startCh = -1;
	int endCh = 0;
	int count = 0;
    int windowNum = 0;
	//
	m_pbChannelBits = chnnBits;
	for(long channel = 0; (channel < 64) && ((channelMask << channel) <= chnnBits); channel++)
	{
		if((channelMask << channel) & chnnBits)
		{
			if(startCh == -1)
			{
				startCh = channel;
			}
			endCh = channel;
		}
	}

	channelMask = 0;
	for(long channel = startCh; channel <= endCh; channel++)
	{
		channelMask |= ((ULONGLONG)0x01 << channel);
	}

	count = endCh - startCh + 1;
	
	if (1 == count)
	{
		windowNum =1;
	}
	else if (count <= 4)
	{
		windowNum = 4;
	}
	else if (count <= 9)
	{
		windowNum = 9;
	}
	else if (count <= 16)
	{
		windowNum = 16;
	}
	else if(count < 25)
	{
		windowNum = 25;
	}
	else
	{
		assert(false);
	}

	m_pDataDisplay->SetDivision(windowNum, channelMask);

	channelMask = 0x01;

	for(long channel = 0; (channel < 64) && ((channelMask << channel) <= chnnBits); channel++)
	{
		if((channelMask << channel) & chnnBits)
		{
			m_pDataDisplay->Start(channel);
		}
	}

	return true;
#endif
}

bool CLocalDevice::PlaybackPause(bool bPause)
{
#if defined(__ENVIRONMENT_LINUX__)
	if (bPause)
	{
		for (int i=0; i<m_videoInputNum; i++)
		{
#if  !defined(__CHIPGM__)
			ext_playback_pause(i, 1);
#endif
		}
	}
	else
	{
		for (int i=0; i<m_videoInputNum; i++)
		{
#if  !defined(__CHIPGM__)
			ext_playback_pause(i, 0);
#endif
		}
	}
#endif

	return true;
}

bool CLocalDevice::PlaybackPause(unsigned long ulChnn, bool bPause)
{
#if defined(__ENVIRONMENT_LINUX__)
	if (bPause)
	{
#if !defined(__CHIPGM__)
		ext_playback_pause(ulChnn, 1);
#endif
	}
	else
	{
#if !defined(__CHIPGM__)
		ext_playback_pause(ulChnn, 0);
#endif
	}
#endif

	return true;
}

bool CLocalDevice::PlaybackSetBuf(FRAME_INFO_EX *pFrameInfo)
{
#ifdef __ENVIRONMENT_LINUX__
	if (pFrameInfo->frameType == FRAME_TYPE_VIDEO_FORMAT)
	{
		delete [] pFrameInfo->pData;
		pFrameInfo->pData = NULL;
		return true;
	}
	else if (pFrameInfo->frameType == FRAME_TYPE_AUDIO_FORMAT)
	{
		delete [] pFrameInfo->pData;
		pFrameInfo->pData = NULL;
		return true;
	}
	else
	{
		if (0 != ext_playback_set_buf(pFrameInfo))
		{
			return false;
		}
		else
		{
			return true;
		}
	}
#else
	ENCODE_FRAME_INFO encodeFrameInfo;
	encodeFrameInfo.dwLen = pFrameInfo->length;
	encodeFrameInfo.pData = pFrameInfo->pData;
	encodeFrameInfo.ucChannel = (unsigned char)pFrameInfo->channel;
	encodeFrameInfo.llRateTime = pFrameInfo->time;
	encodeFrameInfo.llRealTime = pFrameInfo->time;
	encodeFrameInfo.ucKeyFrame = (unsigned char)pFrameInfo->keyFrame;
	if(pFrameInfo->frameType == FRAME_TYPE_VIDEO_FORMAT)
	{
		encodeFrameInfo.ucFrameType = DD_FRAME_VIDEO_FORMAT;
	}
	else if(pFrameInfo->frameType == FRAME_TYPE_AUDIO_FORMAT)
	{
		encodeFrameInfo.ucFrameType = DD_FRAME_AUDIO_FORMAT;
	}
	else if(pFrameInfo->frameType == FRAME_TYPE_VIDEO)
	{
		encodeFrameInfo.ucFrameType = DD_FRAME_VIDEO;
	}
	else if(pFrameInfo->frameType == FRAME_TYPE_AUDIO)
	{
		encodeFrameInfo.ucFrameType = DD_FRAME_AUDIO;
	}
	else
	{
		//assert(false);
		return false;
	}

	if(pFrameInfo->frameAttrib & PLAY_FRAME_NO_SHOW)
	{
		encodeFrameInfo.ucSkipFlag |= DD_FRAME_NO_SHOW;
	}
	else if(pFrameInfo->frameAttrib & PLAY_FRAME_SEC_END)
	{
		encodeFrameInfo.ucSkipFlag |= DD_FRAME_SEC_END;
	}

	m_pDataDisplay->PlaySetBuf(&encodeFrameInfo);

	if (pFrameInfo->frameType == FRAME_TYPE_VIDEO_FORMAT)
	{
		delete [] pFrameInfo->pData;
		pFrameInfo->pData = NULL;
	}
	else if (pFrameInfo->frameType == FRAME_TYPE_AUDIO_FORMAT)
	{
		delete [] pFrameInfo->pData;
		pFrameInfo->pData = NULL;
	}

	return true;
#endif
}
bool CLocalDevice::PlaybackGetBuf(FRAME_INFO_EX *pFrameInfo)
{
#ifdef __ENVIRONMENT_LINUX__
	if (pFrameInfo->frameType == FRAME_TYPE_VIDEO_FORMAT)
	{
		pFrameInfo->pData = new unsigned char [512];
		return true;
	}
	else if (pFrameInfo->frameType == FRAME_TYPE_AUDIO_FORMAT)
	{
		pFrameInfo->pData = new unsigned char [512];
		return true;
	}
	else
	{
		if (0 != ext_playback_get_buf(pFrameInfo))
		{
			return false;
		}
		else
		{
			return true;
		}
	}
#else
	ENCODE_FRAME_INFO encodeFrameInfo;
	if(pFrameInfo->frameType == FRAME_TYPE_VIDEO)
	{
		encodeFrameInfo.ucFrameType = DD_FRAME_VIDEO;
	}
	else if(pFrameInfo->frameType == FRAME_TYPE_AUDIO)
	{
		encodeFrameInfo.ucFrameType = DD_FRAME_AUDIO;
	}
	else if (pFrameInfo->frameType == FRAME_TYPE_VIDEO_FORMAT)
	{
		pFrameInfo->pData = new unsigned char [512];
		return true;
	}
	else if (pFrameInfo->frameType == FRAME_TYPE_AUDIO_FORMAT)
	{
		pFrameInfo->pData = new unsigned char [512];
		return true;
	}
	else
	{
		assert (false);
	}
	encodeFrameInfo.ucChannel = pFrameInfo->channel;

	bool bRet = !!m_pDataDisplay->PlayGetBuf(&encodeFrameInfo);
	if(bRet)
	{
		pFrameInfo->pData = encodeFrameInfo.pData;
		pFrameInfo->length = encodeFrameInfo.dwLen;
	}

	return bRet;
#endif
}

int CLocalDevice::CheckDecodeError()
{
#ifdef __ENVIRONMENT_LINUX__
	int ret = ext_err_get_last_value();	
	if (ret != 0)
	{
		printf("XXXXXXXXXXXXXXXXXXXXXXXdecode error.curtime:%d ", ret);
		PrintfBit64(GetCurrTime(),false);	
	}
	return ret;	
#endif
	return 0;
}

//////////////////////////////////////////////////////////////////////////
CLocalDevice::CLocalDevice()
{
#ifdef __ENVIRONMENT_WIN32__
	m_dataLen	= 320*1024;
	m_pData		= new unsigned char [m_dataLen];
	m_channl	= 0;
	memset(m_index, 0, sizeof(m_index));
	memset(m_preFrameTime, 0, sizeof(m_preFrameTime));
	memset(m_preFrameTime, 0, sizeof(m_preFrameTime));
#endif

#ifdef __ENVIRONMENT_LINUX__
	m_displayWidth = 720;
	m_displayHeight = 480;
	m_mouseProcID = PUB_THREAD_ID_NOINIT;
	m_bMouseProc = false;

	m_pOSDInfo = NULL;

	m_videoLossStatus = 0xffffffff;
#endif

	m_bright = 0;
	m_hue = 0;
	m_sat = 0;
	m_contrast = 0;

	m_imageDefaultColor = 0;
	m_bInitial = 0;
	
	m_dispChannelNum = 0;
}

CLocalDevice::~CLocalDevice()
{
#ifdef __ENVIRONMENT_WIN32__
	delete [] m_pData;
	m_pData		= NULL;
	m_channl	= 0;
	m_dataLen	= 0;
	memset(m_index, 0, sizeof(m_index));
#endif
}

#ifdef __ENVIRONMENT_LINUX__
PUB_THREAD_RESULT PUB_THREAD_CALL CLocalDevice::MouseThread(void *pParam)
{
#ifdef __ENVIRONMENT_LINUX__
	pid_t pid = getpid();
	pid_t tid = syscall(__NR_gettid);
	printf("%s, %d, pid = %d, tid=%d\n", __FILE__, __LINE__, pid, tid);
#endif

	CLocalDevice *pThis = reinterpret_cast<CLocalDevice *>(pParam);
	pThis->MouseProc();

	return 0;
}

int CLocalDevice::MouseProc()
{
	int  mouse_fd = -1;
	int bytes = 0;
	signed char buf [32] = {0};
	short dx = m_displayWidth/2, dy = m_displayHeight/2, bp = 1;
	static   unsigned   char   IMPS2_Param   []   =   {243,200,243,100,243,80};
	fd_set fd_read;
	struct timeval tv;
	KEY_VALUE_INFO keyValue;
	keyValue.type = KEY_TYPE_MOUSE;

	do 
	{
		//保持经常初始化鼠标
		if (bytes >= 0)
		{
#ifndef __CHIPTI__
			mouse_fd = open("/dev/mice",  O_RDWR);
#else
			mouse_fd = open("/dev/input/mice",  O_RDWR);
#endif
			if (mouse_fd < 0)
			{
				printf("I can not open device %d\n", mouse_fd);
				usleep(1000000);
				continue;
			}
			else
			{
				printf("Open device success %d!\n", mouse_fd);
				write   (mouse_fd,   IMPS2_Param,   sizeof   (IMPS2_Param));

				FD_ZERO(&fd_read);
				FD_SET(mouse_fd, &fd_read);
				tv.tv_sec = 5;
				tv.tv_usec=0;
			}
		}
		else
		{
			close(mouse_fd);
			mouse_fd=0;
			bytes = 0;
			continue;
		}

		while (m_bMouseProc && (bytes >= 0))
		{
			switch (select(mouse_fd+1, &fd_read, NULL, NULL, &tv))
			{
			case -1:
				printf("system device err\n");
				break;
			case 0:
				//printf("select wait timeouted\n");
				FD_SET(mouse_fd, &fd_read);
				tv.tv_sec = 5;
				tv.tv_usec=0;
				continue;
			default:
				if (FD_ISSET(mouse_fd, &fd_read))
				{		
					bytes = read(mouse_fd,  buf, 32);
				}
				else
				{	
					bytes = -1;
				}

				if( bytes < 0 )
				{
					printf(" mouse read error!\n");
					break;
				}

				//printf("buf[0]:%x, buf[1]:%d, buf[2]:%d, buf[3]:%d, dx:%d, dy:%d\n", buf[0], buf[1], buf[2], buf[3], dx, dy);

				dx += buf[1];
				dy += (-buf[2]);

				if (0 > dx)
				{
					dx = 0;
				}
				else if (dx >= m_displayWidth)
				{
					dx = m_displayWidth - 1;
				}

				if (0 > dy)
				{
					dy = 0;
				}
				else if (dy >= m_displayHeight)
				{
					dy = m_displayHeight - 1;
				}

				keyValue.xPos = dx;
				keyValue.yPos = dy;
				keyValue.type = KEY_TYPE_MOUSE;

				GUI::CFBInterface::Instance()->DrawMice(dx, dy);

				if (0x9 == buf[0])
				{
					keyValue.isDown = true;
					keyValue.value = MOUSE_KEY_LEFT_DOWN;
				}
				else if (0xa == buf[0])
				{
					keyValue.isDown = true;
					keyValue.value = MOUSE_KEY_RIGHT_DOWN;
				}
				else
				{
					keyValue.isDown = false;

					if (buf[3] < 0)//前
					{
						keyValue.value = MOUSE_KEY_WHEEL_FORWARD;
					}
					else if (buf[3] > 0)
					{
						keyValue.value = MOUSE_KEY_WHEEL_BACK;
					}
					else
					{
						keyValue.value = MOUSE_KEY_MOUSE_MOVE;
					}
				}

				m_keyLock.Lock();
				m_keyValueList.push_back(keyValue);
				m_keyLock.UnLock();

				tv.tv_sec = 5;
				tv.tv_usec=0;

				PUB_Sleep(5);
			}
		}
	} while(m_bMouseProc);

	close(mouse_fd);

	return 0;
}
#endif
//////////////////////////////////////////////////////////////////////////
int CLocalDevice::GetTalkWay(FRAME_INFO_EX &frameInfo)
{
#ifdef __ENVIRONMENT_LINUX__
	if(0 != ext_get_audio_speek_stream(&frameInfo))
	{
		return GET_FRAME_FAIL;
	}
	else
	{
		return GET_FRAME_SUCC;
	}
#else
	return GET_FRAME_FAIL;
#endif
}

void CLocalDevice::ReleaseTalkWayBuf(FRAME_INFO_EX &frameInfo)
{
#ifdef __ENVIRONMENT_LINUX__
	ext_release_audio_speek_buf(&frameInfo);
#endif
}

void CLocalDevice::TalkWayStop()
{
#ifdef __ENVIRONMENT_LINUX__
	ext_audio_speek_stop();
	m_bTalking = false;
	
#endif
} 

bool CLocalDevice::TalkWayStart()
{
#ifdef __ENVIRONMENT_LINUX__
	//如果在现场并且有现场声音预览，则先关闭现场声音
	if(m_bOpenAudio)
	{

		if (m_preLiveAudioChnn < CProduct::Instance()->AudioInputNum() && (0 != ext_set_audio_live_channel(0, 0)))
		{
			//关闭现场声音失败，不进入对讲
			return false;
		}

#ifdef __DVR_ULTIMATE__
		if (m_preLiveAudioChnn >= CProduct::Instance()->LocalVideoInputNum() && (0 != ext_ip_in_audio_stop()))
		{
			//关闭现场声音失败，不进入对讲
			return false;
		}
#endif
		m_bOpenAudio = false;
		printf("%s,%d,talk to close audio\n",__FILE__,__LINE__);
	}
	else
	{
		printf("%s,%d, live audio is closed\n",__FILE__,__LINE__);
	}

	ext_audio_speek_start();
	m_bTalking = true;

	return true;
#else
	return false;
#endif
}

bool CLocalDevice::TalkWaySetBuf(FRAME_INFO_EX *pFrameInfo)
{
#ifdef __ENVIRONMENT_LINUX__
	if(0 != ext_audio_speek_set_buf(pFrameInfo))
	{
		return false;
	}

	return true;
#else
	return false;
#endif
}

bool CLocalDevice::TalkWayGetBuf(FRAME_INFO_EX *pFrameInfo)
{
#ifdef __ENVIRONMENT_LINUX__
	if(0 != ext_audio_speek_get_buf(pFrameInfo))
	{
		return false;
	}

	return true;
#else
	return false;
#endif
}

//#ifdef __ENVIRONMENT_WIN32__
#define DEC_CHIP_NUM	1
//#endif

long CLocalDevice::GetIPCDecChipNum()
{
#ifdef __DVR_ULTIMATE__
#ifdef __ENVIRONMENT_LINUX__
	ext_dvr_ip_in_buf_addr ipInBuffer, ipInBufferMinor;
	ext_ip_in_get_buf_addr(&ipInBuffer, &ipInBufferMinor);
	return ipInBuffer.buf_number;
#else
	return DEC_CHIP_NUM;
#endif
#endif
	return DEC_CHIP_NUM;
}

bool CLocalDevice::GetIPCBuffer(long chipId, unsigned char *&pBuffer, unsigned long &bufferSize, unsigned char *&pMinorBuffer, unsigned long &minorBufSize)
{
#ifdef __DVR_ULTIMATE__
#ifdef __ENVIRONMENT_LINUX__
	ext_dvr_ip_in_buf_addr ipInBuffer, ipInBufferMinor;
	ext_ip_in_get_buf_addr(&ipInBuffer, &ipInBufferMinor);
	
	pBuffer = ipInBuffer.buf_vir_addr[chipId];
	bufferSize = ipInBuffer.buf_vir_len[chipId];

	pMinorBuffer = ipInBufferMinor.buf_vir_addr[chipId];
	minorBufSize = ipInBufferMinor.buf_vir_len[chipId];


	printf("buffersize[%d] = %x, %d, minorBufSize[%d] = %x, %d %s, %d\n", chipId, pBuffer, bufferSize, chipId, pMinorBuffer, minorBufSize, __FILE__, __LINE__);
	assert(bufferSize > 1536*1024);
	assert((minorBufSize == 0) || (minorBufSize > 512 * 1024));

	return true;
#else
	static unsigned char *pIPBuffer[DEC_CHIP_NUM] = {NULL};
	static unsigned char *pIPMinorBuffer[DEC_CHIP_NUM] = {NULL};
	bufferSize = 28*1024*1024;
	minorBufSize = 3 * 1024 * 1024;
	if(NULL == pIPBuffer[0])
	{
		for(int i = 0; i < DEC_CHIP_NUM; i++)
		{
			pIPBuffer[i] = new unsigned char[bufferSize];
			pIPMinorBuffer[i] = new unsigned char[minorBufSize];
		}
	}

	pBuffer = pIPBuffer[chipId];
	pMinorBuffer = pIPMinorBuffer[chipId];
	
	return true;
#endif
#endif
	return false;
}

bool CLocalDevice::IPCStart(unsigned long channelInfo, unsigned long minorChannelInfo, unsigned int* chip_id)
{
#ifdef __DVR_ULTIMATE__
#ifdef __ENVIRONMENT_LINUX__
	ext_ip_in_start(channelInfo, minorChannelInfo, chip_id);
#else
	for(int i = 0; i < 8; i++)
	{
		chip_id[i] = 0;
	}
#endif
#endif

	return true;
}

bool CLocalDevice::IPCStop()
{
#ifdef __DVR_ULTIMATE__
#ifdef __ENVIRONMENT_LINUX__
	ext_ip_in_stop();
#else
#endif
#endif
	return true;
}

bool CLocalDevice::IPCSetBufferData(ext_dvr_ip_in_buffer_param *pIPBufferParam)
{
	bool bRet = true;
#ifdef __DVR_ULTIMATE__
#ifdef __ENVIRONMENT_LINUX__
	bRet = (0 == ext_ip_in_set_buffer_data(pIPBufferParam));
#else
	*pIPBufferParam->pStatus = 0;
#endif
#endif

	return bRet;
}

bool CLocalDevice::IPCMinorSetBufferData(ext_dvr_ip_in_buffer_param *pIPBufferParam)
{
	bool bRet = true;
#ifdef __DVR_ULTIMATE__
#ifdef __ENVIRONMENT_LINUX__
	bRet = (0 == ext_ip_in_small_set_buffer_data(pIPBufferParam));
#else
	*pIPBufferParam->pStatus = 0;
#endif
#endif

	return bRet;
}

int CLocalDevice::GetDecLeftFrame(int channel)
{
	int frameCount = 0;
#ifdef __DVR_ULTIMATE__
#ifdef __ENVIRONMENT_LINUX__
	ext_get_vdec_left_frame_cnt(channel, &frameCount);
#endif
#endif
	return frameCount;
}

//void CLocalDevice::SetSnapTime(int sec_tm)
//{
//#ifdef __ENVIRONMENT_LINUX__
//	 ext_set_snap_image_time(sec_tm );
//#endif
//}

//
//void CLocalDevice::ChangeJPEGVideoSize(VIDEO_SIZE videoSize)
//{
//	m_setSnapRateLock.Lock();
//
//	m_snapVideSize = videoSize;
//
//	m_setSnapRateLock.UnLock();
//}
//
//void CLocalDevice::ChangeJPEGVideoLevel(unsigned long videLevel)
//{
//	m_setSnapRateLock.Lock();
//	m_jpegLevel = videLevel;
//
//	m_setSnapRateLock.UnLock();
//}
//
//void CLocalDevice::ChangeJPEGRate(unsigned long rate)
//{
//	m_setSnapRateLock.Lock();
//	m_phoneRaiseChnnRate = rate;
//	m_setSnapRateLock.UnLock();
//}

//void CLocalDevice::SetSnapCapability(int channelNum, int rate, int rateTime, VIDEO_SIZE videoSize)
//{
//
//#ifdef __ENVIRONMENT_LINUX__
//	m_setSnapRateLock.Lock();
//	
//	m_snapRate = rate;
//	m_snapRateTime = rateTime;
//	m_snapVideSize = videoSize;
//	m_phoneSnapCH = 0;
//	m_emailSnapCH = 0;
//	m_phoneRaiseSnapChnn = -1;
//	m_phoneRaiseChnnRate = 2;
//	m_jpegLevel = 3;
//	for (int chnn = 0; chnn < channelNum; ++chnn)
//	{
//		printf("%s,%d,channel:%d, SetSnapEnable %d, %d, videoLevel:%d,videosize:%d,rateAndLevel:0\n",__FILE__, __LINE__,chnn, 0, 0, m_jpegLevel,videoSize);
//		ext_set_snap_channel_enable(chnn, 0, 0, videoSize);
//	}
//	m_setSnapRateLock.UnLock();
//#endif
//}

//
//void CLocalDevice::SetEmailSnapEnable(int channel, bool bEnable)
//{
//#ifdef __ENVIRONMENT_LINUX__
//	m_setSnapRateLock.Lock();
//	int rate = 0;
//	int rateTime = 0;
//
//	if (0 == m_phoneSnapCH)
//	{
//		//在没有手机请求jpeg流的情况下
//		int rateAndLevel = 0;
//		if (bEnable)
//		{
//			rate = m_snapRate;
//			rateTime = m_snapRateTime;
//			m_emailSnapCH |= ((ULONGLONG)0x01 << channel);
//			rateAndLevel = (m_jpegLevel << 16) + rate;
//		}
//		else
//		{
//			rate = 0;
//			rateTime = 0;
//			m_emailSnapCH &= (~((ULONGLONG)0x01 << channel));
//			rateAndLevel = 0;
//		}
//		printf("%s,%d,channel:%d, no phone,bEmailEnable:%d, SetSnapEnable  %d, %d, videoLevel:%d, videosize:%d,rateAndLevel:%x\n",__FILE__, __LINE__,channel, bEnable,rate, rateTime,m_jpegLevel,m_snapVideSize,rateAndLevel);
//
//		ext_set_snap_channel_enable(channel, rateAndLevel, rateTime, m_snapVideSize);
//	}
//	else
//	{
//		//在有手机请求jpeg流的情况下
//		if (m_phoneSnapCH & ((ULONGLONG)0x1 << channel))
//		{
//			printf("%s,%d, chanel:%d close emailSnap, but have phone,so not setsanp \n",__FILE__,__LINE__,channel);
//		}
//		else
//		{
//			if (bEnable)
//			{
//				if (2 == m_snapRateTime)
//				{
//					rate = 1;
//					rateTime = 3;
//				}
//				else if (1 == m_snapRateTime)
//				{
//					rate = 1;
//					rateTime = 2;
//				}
//				else
//				{
//					rate = 1;
//					rateTime = 3;
//				}
//				m_emailSnapCH |= ((ULONGLONG)0x01 << channel);
//			}
//			else
//			{
//				rate = 0;
//				rateTime = 0;
//				m_emailSnapCH &= (~((ULONGLONG)0x01 << channel));
//			}
//
//			int rateAndLevel = (m_jpegLevel << 16) + rate;
//			printf("%s,%d,channel:%d, have phone,bEmailEnable:%d, SetSnapEnable  %d, %d, videoLevel:%d,videosize:%d,rateAndLevel:%x\n",__FILE__, __LINE__,channel, bEnable,rate, rateTime,m_jpegLevel,m_snapVideSize, rateAndLevel);
//			
//			ext_set_snap_channel_enable(channel, rateAndLevel, rateTime, m_snapVideSize);
//		}		
//
//	}
//
//	m_setSnapRateLock.UnLock();
//#endif
//}
//
//void CLocalDevice::SetPhoneSnapChannel(int channel, bool bEnable)
//{
//#ifdef __ENVIRONMENT_LINUX__
//	m_setSnapRateLock.Lock();
//
//	int rate = 0;
//	int rateTime = 0;
//	int phoneChannelNum = 0;
//	int firstPhoneChannel = -1;
//
//	if (bEnable)
//	{
//		m_phoneSnapCH |= ((ULONGLONG)0x01 << channel);		
//	}
//	else
//	{
//		m_phoneSnapCH &= (~((ULONGLONG)0x01 << channel));		
//	}
//
//	for (int chnn = 0; chnn < m_localVideoInputNum; ++chnn)
//	{
//		if (m_phoneSnapCH & ((ULONGLONG)0x1 << chnn))
//		{
//			firstPhoneChannel = chnn;
//			break;
//		}
//	}
//
//	if (m_phoneRaiseSnapChnn < 0)
//	{
//		if (firstPhoneChannel > -1)
//		{
//			m_phoneRaiseSnapChnn = firstPhoneChannel;
//			int rateAndLevel = (m_jpegLevel << 16) + m_phoneRaiseChnnRate;
//			printf("%s,%d, set channel:%d, snap %d,%d,videoLevel:%d,videosize:%d,rateAndLevel:%x\n",__FILE__,__LINE__,m_phoneRaiseSnapChnn, m_phoneRaiseChnnRate,1,m_jpegLevel,m_snapVideSize,rateAndLevel);
//			ext_set_snap_channel_enable(m_phoneRaiseSnapChnn, rateAndLevel, 1, m_snapVideSize);
//		}		
//	}
//	else
//	{
//		if (m_phoneSnapCH & ((ULONGLONG)0x1 << m_phoneRaiseSnapChnn))
//		{
//
//		}
//		else
//		{
//			m_phoneRaiseSnapChnn = firstPhoneChannel;
//			if (m_phoneRaiseSnapChnn > -1)
//			{
//				int rateAndLevel = (m_jpegLevel << 16) + m_phoneRaiseChnnRate;
//				printf("%s,%d, set channel:%d, snap %d,%d,videlLevel:%d,videosize:%d,rateAndLevel:%x\n",__FILE__,__LINE__,m_phoneRaiseSnapChnn, m_phoneRaiseChnnRate,1,m_jpegLevel,m_snapVideSize,rateAndLevel);
//				ext_set_snap_channel_enable(m_phoneRaiseSnapChnn, rateAndLevel, 1, m_snapVideSize);
//			}			
//		}
//	}
//	
//
//	if (m_phoneRaiseSnapChnn > -1)
//	{
//		if (2 == m_snapRateTime)
//		{
//			rate = 1;
//			rateTime = 3;
//		}
//		else if (1 == m_snapRateTime)
//		{
//			rate = 1;
//			rateTime = 2;
//		}
//		else
//		{
//			rate = 1;
//			rateTime = 3;
//		}
//	}
//	else
//	{
//		rate = m_snapRate;
//		rateTime = m_snapRateTime;
//	}
//
//	for (int chnn = 0; chnn < m_localVideoInputNum; ++chnn)
//	{
//		if (chnn != m_phoneRaiseSnapChnn)
//		{
//			if ((m_phoneSnapCH & ((ULONGLONG)0x1 << chnn)) || (m_emailSnapCH & ((ULONGLONG)0x1 << chnn))) 
//			{
//				int rateAndLevel = (m_jpegLevel << 16) + rate;
//				printf("%s,%d, set channel:%d, snap %d,%d,videlLevel:%d,videosize:%d,rateAndLevel:%x\n",__FILE__,__LINE__,chnn, rate,rateTime,m_jpegLevel,m_snapVideSize,rateAndLevel);
//				ext_set_snap_channel_enable(chnn, rateAndLevel, rateTime, m_snapVideSize);
//			}
//			else
//			{
//				printf("%s,%d, set channel:%d, snap %d,%d,videosize:%d\n",__FILE__,__LINE__,chnn, 0,0,m_snapVideSize);
//				ext_set_snap_channel_enable(chnn, 0, 0, m_snapVideSize);
//			}
//
//		}
//	}
//
//	m_setSnapRateLock.UnLock();
//#endif
//}

//////////////////////////////////////////////////////////////////////////
void CLocalDevice::SetLcdScreemStatus(bool bOn)
{
#ifdef __ENVIRONMENT_LINUX__
	if (bOn)
	{
		ext_lcd_screen_onoff(1);
	}
	else
	{
		ext_lcd_screen_onoff(0);
	}
#endif	
}



//////////////////////////////////////////////////////////////////////////
bool CLocalDevice::GetMCUVersion(char *pVersion, int len)
{
#ifdef __ENVIRONMENT_LINUX__
	assert(pVersion != NULL);
	assert(len >= 9);

	if (m_bSupportMCU)
	{
		CMcu8952::Instance()->Operation(SERIAL_GETVERSION, 0);
		DVRSleep(50);
		CMcu8952::Instance()->GetMCUVersion(pVersion);
	}
	else
	{
		strcpy(pVersion,"---");
	}
#endif
	return true;	
}

bool CLocalDevice::GetKernelVersion(char *pVersion, int len)
{
#ifdef __ENVIRONMENT_LINUX__
	/*
	assert(pVersion != NULL);
	assert(len >= 11);
	FILE* pfd = fopen("/proc/sys/kernel/version","r");
	if ( NULL == pfd )
	{
		return false;
	}

	char str[80] = {0};
	fgets(str,80,pfd);
	printf("kernel verions is: %s\n", str);
	fclose(pfd);
	snprintf(pVersion, 11, "%s", str+1);
	*/

	if (0 != ext_get_flash_param(FLASH_PARAM_KERNELVERSION, pVersion, len))
	{
		return false;
	}
#endif	
	return true;
}

bool CLocalDevice::GetMacFromFlash(char *pMac, int len)
{
#ifdef __ENVIRONMENT_LINUX__
	assert(pMac != NULL);
	assert(len >= 17);
	if (0 != ext_get_flash_param(FLASH_PARAM_MAC_ADDR, pMac, 17))
	{
		return false;
	}
#endif
	return true;
}

bool CLocalDevice::WriteMacToFlash(char *pMac, int len)
{
#ifdef __ENVIRONMENT_LINUX__
	assert(pMac != NULL);
	assert(len >= 17);
	if (0 != ext_set_flash_param(FLASH_PARAM_MAC_ADDR, pMac, 17))
	{
		return false;
	}
	
	//设置到flash成功后，然后在设置到文件里
#if defined(__TDFH__) || defined(__TDNXP__) || defined(__CHIPGM__)
	FILE * pFile = fopen("/mnt/mtd/mac.dat", "wb");
#else
	FILE * pFile = fopen("/etc/init.d/mac.dat", "wb");
#endif
	if (NULL == pFile)
	{
		return false;
	}
	if (1 != fwrite(pMac, 17, 1, pFile))
	{
		fclose(pFile);
		return false;
	}
	sync();
	fclose(pFile);		
#endif
	return true;
}

bool CLocalDevice::GetHardWareVersion(char *pVersion, int len)
{
#ifdef __ENVIRONMENT_LINUX__
	assert(pVersion != NULL);
	assert(len >= 6);
	if (0 != ext_get_flash_param(FLASH_PARAM_HD_VERSION, pVersion, 6))
	{
		return false;
	}
#endif
	
	pVersion[5] = '\0';

	return true;
}

bool CLocalDevice::WriteHardWareVersion(char *pVersion, int len)
{
#ifdef __ENVIRONMENT_LINUX__
	assert(pVersion != NULL);
	assert(len >= 6);
	if (0 != ext_set_flash_param(FLASH_PARAM_HD_VERSION, pVersion, 6))
	{
		return false;
	}
#endif
	return true;
}

unsigned long CLocalDevice::GetProductID()
{
#ifdef __ENVIRONMENT_LINUX__
	int retProductID = 0;
	if (NULL == g_pCryptoInfo)
	{
		g_pCryptoInfo = new ext_dvr_product_info;
		if (NULL == g_pCryptoInfo)
		{
			return ext_crypto_check();
		}

		ext_crypto_check_ex(g_pCryptoInfo);

		retProductID = g_pCryptoInfo->product_id;
		//2316SE产品强制转成2316ME
		if (TD_2316SE == retProductID)
		{
			printf("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@%s,%d,2316SE to 2316ME\n",__FILE__,__LINE__);
			retProductID = TD_2316ME;
			g_b2316SETo2316ME = true;
		}
#ifdef __CUSTOM_RU04__
		if ((retProductID == TD_2304SE) || (retProductID == TD_2304ME))
		{
			g_Ru04OldProductType = retProductID;
			retProductID = TD_2504HE_M;
			g_bRu042304SEMETo2504HE_M = true;
			printf("!!!!!!!!!!!111change 2304se to 2504HE_M\n");
		}
#endif

#ifdef __CUSTOM_US02__
		if ((TD_2516HE_B_N100 == retProductID))
		{
			retProductID = TD_2616LE;
			printf("Change 2516HE_B_N100 to 22516HE_C");
		}
		else if ((TD_2708XE_M == retProductID) || (TD_2708XE_MH == retProductID))
		{
			retProductID = TD_2708XE_P;
			printf("Change 2708XE_M/H to 2708XE_P\n");
		}
		else if (TD_2716XD_M == retProductID)
		{
			retProductID = TD_2716XD_P;
			printf("Change TD_2716XD_M to 2716XD_P\n");
		}
		else if (TD_2316SS_B == retProductID)
		{
			retProductID = TD_2316SS_C;
			printf("Change TD_2316SS_B to 2316SS_C\n");
		}
#endif
		return retProductID;
	}
	else
	{
		retProductID = g_pCryptoInfo->product_id;
		//2316SE产品强制转成2316ME
		if (TD_2316SE == retProductID)
		{
			printf("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@%s,%d,2316SE to 2316ME\n",__FILE__,__LINE__);
			retProductID = TD_2316ME;
			g_b2316SETo2316ME = true;
		}
#ifdef __CUSTOM_RU04__
		if ((retProductID == TD_2304SE) || (retProductID == TD_2304ME))
		{
			g_Ru04OldProductType = retProductID;
			retProductID = TD_2504HE_M;
			g_bRu042304SEMETo2504HE_M = true;
			printf("!!!!!!!!!!!222change 2304se to 2504HE_M\n");
		}
#endif

#ifdef __CUSTOM_US02__
		if ((TD_2516HE_B_N100 == retProductID))
		{
			retProductID = TD_2616LE;
			printf("Change 2516HE_B_N100 to 22516HE_C");
		}
		else if ((TD_2708XE_M == retProductID) || (TD_2708XE_MH == retProductID))
		{
			retProductID = TD_2708XE_P;
			printf("Change 2708XE_M/H to 2708XE_P\n");
		}
		else if (TD_2716XD_M == retProductID)
		{
			retProductID = TD_2716XD_P;
			printf("Change TD_2716XD_M to 2716XD_P\n");
		}
		else if (TD_2316SS_B == retProductID)
		{
			retProductID = TD_2316SS_C;
			printf("Change TD_2316SS_B to 2316SS_C\n");
		}
#endif

		return retProductID;
	}	
#else
	return 0;
#endif
}

unsigned long CLocalDevice::GetProductSubID()
{
#ifdef __ENVIRONMENT_LINUX__
	char version[12] = {0};
	if (0 != ext_get_flash_param(FLASH_PARAM_SUB_VER, version, 6))
	{
		return 0;
	}
	return atoi(version);
#else
	return 0;
#endif	
}
bool CLocalDevice::WriteProductSubID(char *pVersion, int len)
{
#ifdef __ENVIRONMENT_LINUX__
	assert(pVersion != NULL);
	printf("pVersion:%s\n",pVersion);
	if (strlen(pVersion) > 2)
	{
		return false;
	}
	if (0 != ext_set_flash_param(FLASH_PARAM_SUB_VER, pVersion, 6))
	{
		return false;
	}
#endif
	return true;
}

unsigned long CLocalDevice::GetDisplayModFromFlash( )
{
#ifdef __ENVIRONMENT_LINUX__
	if (VIDEO_OUT_NONE == m_videoOutTypeFlash)
	{
		char str[8] = {0};
		if (0 != ext_get_flash_param(FLASH_PARAM_OUT_DEVICE, str, 1))
		{
			return 0;
		}

		if(strcmp(str,"1")==0)
		{
			m_videoOutTypeFlash = VIDEO_OUT_HDMI;
		}
		else if(strcmp(str,"2")==0)
		{
			m_videoOutTypeFlash = VIDEO_OUT_VGA;
		}
		else if(strcmp(str,"3")==0)
		{
			m_videoOutTypeFlash = VIDEO_OUT_CVBS;
		}
		else
		{
			return 0;
		}
	}

	return m_videoOutTypeFlash;
#else
	return 0;
#endif
}

bool CLocalDevice::WriteDisplayModToFlash(char *pDisplay, int len)
{
#ifdef __ENVIRONMENT_LINUX__
	assert(pDisplay != NULL);
	if (1 != strlen(pDisplay))
	{
		return false;
	}

	VIDEO_OUT_TYPE vot = VIDEO_OUT_NONE;
	if(strcmp(pDisplay,"1")==0)
	{
		vot = VIDEO_OUT_HDMI;
	}
	else if(strcmp(pDisplay,"2")==0)
	{
		vot = VIDEO_OUT_VGA;
	}
	else if(strcmp(pDisplay,"3")==0)
	{
		vot = VIDEO_OUT_CVBS;
	}
	else
	{
		return false;
	}

	//如果要写到flash的参数和当前flash中的不一样就需要重写参数到flash,否则不重写
	if (vot != m_videoOutTypeFlash)
	{
		m_videoOutTypeFlash = VIDEO_OUT_NONE;

		if (0 != ext_set_flash_param(FLASH_PARAM_OUT_DEVICE, pDisplay, 1))
		{
			return false;
		}
	}
#endif
	return true;
}
bool CLocalDevice::WriteDisplayResToFlash(char *pRes, int len)
{
#ifdef __ENVIRONMENT_LINUX__
	if (4 != strlen(pRes))
	{
		return false;
	}

	//把16进制字符串转为10进制数
	//判断数组str的前4位是不是
	for(int i=0;i<4;i++)
	{
		if(isdigit(pRes[i]))
		{

		}
		else
		{
			//有不是数字的部分
			printf("no all digit\n");
			return false;
		}
	}

	unsigned long ret=(pRes[0]-48)*16*16*16+(pRes[1]-48)*16*16+(pRes[2]-48)*16+(pRes[3]-48);

	VGA_RESOLUTION vgaResTemp = VGA_RESOLUTION_NONE;
	switch(ret)
	{
	case VGA_640X480:
		{
			vgaResTemp = VGA_640X480;
			break;
		}
	case VGA_720X480:
		{
			vgaResTemp = VGA_720X480;
			break;
		}
	case VGA_720X576:
		{
			vgaResTemp = VGA_720X576;
			break;
		}
	case VGA_800X600:
		{
			vgaResTemp = VGA_800X600;
			break;
		}
	case VGA_1024X768:
		{
			vgaResTemp = VGA_1024X768;
			break;
		}
	case VGA_1280X960:
		{
			vgaResTemp = VGA_1280X960;
			break;
		}
	case VGA_1280X1024:
		{
			vgaResTemp = VGA_1280X1024;
			break;
		}
	case VGA_1920X1080:
		{
			vgaResTemp = VGA_1920X1080;
			break;
		}
	case VGA_1440X960:
		{
			vgaResTemp = VGA_1440X960;
			break;
		}
	case VGA_1440X1152:
		{
			vgaResTemp = VGA_1440X1152;
			break;
		}
	default:
		{
			return false;
			break;
		}
	}

	if (vgaResTemp != m_outResolutionFlash)
	{
		m_outResolutionFlash = VGA_RESOLUTION_NONE;

		if (0 != ext_set_flash_param(FLASH_PARAM_OUT_RESOLUTION, pRes, 4))
		{
			return false;
		}
	}
#endif
		return true;

}

unsigned long CLocalDevice::GetDisplayResFromFlash()
{
#ifdef __ENVIRONMENT_LINUX__

	if (VGA_RESOLUTION_NONE == m_outResolutionFlash)
	{
		char str[8] = {0};
		if (0 != ext_get_flash_param(FLASH_PARAM_OUT_RESOLUTION, str, 4))
		{
			return 0;
		}

		//把16进制字符串转为10进制数
		//判断数组str的前4位是不是
		for(int i=0;i<4;i++)
		{
			if(isdigit(str[i]))
			{

			}
			else
			{
				//有不是数字的部分
				printf("no all digit\n");
				return 0;
			}
		}

		unsigned long ret=(str[0]-48)*16*16*16+(str[1]-48)*16*16+(str[2]-48)*16+(str[3]-48);

		switch(ret)
		{
		case VGA_640X480:
			{
				m_outResolutionFlash = VGA_640X480;
				break;
			}
		case VGA_720X480:
			{
				m_outResolutionFlash = VGA_720X480;
				break;
			}
		case VGA_720X576:
			{
				m_outResolutionFlash = VGA_720X576;
				break;
			}
		case VGA_800X600:
			{
				m_outResolutionFlash = VGA_800X600;
				break;
			}
		case VGA_1024X768:
			{
				m_outResolutionFlash = VGA_1024X768;
				break;
			}
		case VGA_1280X960:
			{
				m_outResolutionFlash = VGA_1280X960;
				break;
			}
		case VGA_1280X1024:
			{
				m_outResolutionFlash = VGA_1280X1024;
				break;
			}
		case VGA_1920X1080:
			{
				m_outResolutionFlash = VGA_1920X1080;
				break;
			}
		case VGA_1440X960:
			{
				m_outResolutionFlash = VGA_1440X960;
				break;
			}
		case VGA_1440X1152:
			{
				m_outResolutionFlash = VGA_1440X1152;
				break;
			}
		default:
			{
				return 0;
				break;
			}
		}
	}
	
	return m_outResolutionFlash;

#else
	return 0;
#endif
	
}
unsigned long CLocalDevice::GetVideoFormatFromFlash( )
{
#ifdef __ENVIRONMENT_LINUX__
#if defined(__CUSTOM_RU04__)
	return VIDEO_FORMAT_PAL;
#else
#if defined(__CUSTOM_YUANZHENG__)
	return VIDEO_FORMAT_PAL;
#else
#if defined(__TW01_RILI__)
	return VIDEO_FORMAT_NTSC;
#else
	if (VIDEO_FORMAT_NONE == m_videoFormatFlash)
	{
		printf("%s:%s:%d, Get video format from flash\n", __FUNCTION__, __FILE__, __LINE__);
		char str[8] = {0};
		if (0 != ext_get_flash_param(FLASH_PARAM_VIDEO_FORMAT, str, 1))
		{
			return 0;
		}
		m_videoFormatFlash = (0 == atoi(str)) ? VIDEO_FORMAT_NTSC : VIDEO_FORMAT_PAL;
	}

	return m_videoFormatFlash;
#endif
#endif
#endif
#else
	return VIDEO_FORMAT_NTSC;
#endif
}

bool CLocalDevice::WriteTimeZoneToFlash(char *pTimeZone, int len)
{
#ifdef __ENVIRONMENT_LINUX__
	assert(pTimeZone != NULL);
	printf("pTimeZone:%s\n",pTimeZone);

	if (0 != ext_set_flash_param(FLASH_PARAM_TIME_ZONE, pTimeZone, len))
	{
		printf("%s %d Set TimeZone failed,, TimeZone= %s\n", __FILE__, __LINE__, pTimeZone);
		return false;
	}
#endif	
	printf("%s %d Set TimeZone Succ, TimeZone= %s\n", __FILE__, __LINE__, pTimeZone);
	return true;
}

unsigned long CLocalDevice::GetTimeZoneFromFlash()
{
#ifdef __ENVIRONMENT_LINUX__
	char name[5] = {0};
	if (0 != ext_get_flash_param(FLASH_PARAM_TIME_ZONE, name, sizeof(name) - 1))
	{
		printf("%s %d Get TimeZone failed\n", __FILE__, __LINE__);
#if defined (__CUSTOM_BR27__)
		return TIME_ZONE_GMT_D3;
#else 
#if defined (__CUSTOM_CN__)|| defined(__CUSTOM_SHANGHAI__)
		return TIME_ZONE_GMT_A8;
#else
		return TIME_ZONE_GMT;
#endif
#endif
	}
	else
	{
		//读取flash中数据成功
		printf("%s %d Get TimeZone Succ, Timezone = %s\n", __FILE__, __LINE__, name);
		if (name[0] != 'G' || name[1] != 'T')
		{
#if defined (__CUSTOM_BR27__)
			return TIME_ZONE_GMT_D3;
#else 
#if defined (__CUSTOM_CN__)|| defined(__CUSTOM_SHANGHAI__)
			return TIME_ZONE_GMT_A8;
#else
			return TIME_ZONE_GMT;
#endif
#endif
		}
		if (name[2] == '0')
		{
			return atoi(const_cast<const char*>(&name[3]));
		}
		else
		{
			return atoi(name + 2);
		}
	}

#endif
	return TIME_ZONE_GMT;
}

bool CLocalDevice::WriteVideoFormatToFlash(char *pFormat, int len)
{
#ifdef __ENVIRONMENT_LINUX__
	assert(pFormat != NULL);
	if (1 != strlen(pFormat))
	{
		return false;
	}

	m_videoFormatFlash = VIDEO_FORMAT_NONE;

	if (0 != ext_set_flash_param(FLASH_PARAM_VIDEO_FORMAT, pFormat, 1))
	{
		return false;
	}	

	//设置完到flash中，再写到/etc/init.d/VideoFormat.dat中
#if defined(__TDFH__) || defined(__TDNXP__) || defined(__CHIPGM__)
	FILE *fp = fopen("/mnt/mtd/VideoFormat.dat", "wb");
#else
	FILE *fp = fopen("/etc/init.d/VideoFormat.dat", "wb");
#endif
	if (NULL == fp)
	{
		return false;
	}
	if (0 != fwrite(pFormat, len, 1, fp))
	{
		fclose(fp);
		return false;
	}
	sync();
	fclose(fp);

#endif
	return true;
}

bool CLocalDevice::WriteDDNSTypeToFlash(char *pDDNSName, int len)
{
#ifdef __ENVIRONMENT_LINUX__
	assert(pDDNSName != NULL);
	printf("pDDNSName:%s\n",pDDNSName);
	if (strlen(pDDNSName) > 3)
	{
		return false;
	}
	if (0 != ext_set_flash_param(FLASH_PARAM_DDNS_TYPE, pDDNSName, 6))
	{
		return false;
	}
#endif
	return true;

}

unsigned long CLocalDevice::GetDDNSTypeFromFlash()
{
#ifdef __ENVIRONMENT_LINUX__
	char name[12] = {0};
	if (0 != ext_get_flash_param(FLASH_PARAM_DDNS_TYPE, name, 6))
	{
		return 0;
	}
	return atoi(name);
#else
	return 0;
#endif	
}

bool CLocalDevice::SetBootImage(char *pFileName)
{
#ifdef __ENVIRONMENT_LINUX__
	if (ext_set_boot_image(pFileName) != 0)
	{
		return false;
	}
#endif
	return true;
}

bool CLocalDevice::WriteCryptoInfo(char *pType, int len)
{
#ifdef __ENVIRONMENT_LINUX__
	assert(pType != NULL);
	assert(len == 5);	
	printf("cryto:%c%c%c%c%c\n",pType[0],pType[1],pType[2],pType[3],pType[4]);
	if (0 != ext_crypto_write_data((unsigned char*)pType, 5))
	{
		return false;
	}	
#endif
	return true;	
}

int CLocalDevice::GetSupportLanguage( )
{
#ifdef __ENVIRONMENT_LINUX__
	if (NULL == g_pCryptoInfo)
	{
		g_pCryptoInfo = new ext_dvr_product_info;
		if (NULL == g_pCryptoInfo)
		{
			return 0;
		}
		ext_crypto_check_ex(g_pCryptoInfo);
		return g_pCryptoInfo->product_language;
	}
	else
	{
		return g_pCryptoInfo->product_language;
	}	
#else
	return 0;
#endif
}

bool CLocalDevice::WriteKeyBoardNameToFlash(char *pName, int len)
{
#ifdef __ENVIRONMENT_LINUX__
	assert(pName != NULL);
	printf("%s,%d,WriteKeyBoardNameToFlash:%s\n",__FILE__,__LINE__,pName);
	if ((strlen(pName) > 6) || (strlen(pName) <= 0))
	{
		return false;
	}
	if (CheckKeyBoardName(pName) < 0)
	{
		return false;
	}
	if (0 != ext_set_flash_param(FLASH_PARAM_KEYBOARD_NAME, pName, 6))
	{
		return false;
	}
#endif
	return true;
}

int CLocalDevice::GetKeyBoardNameFromFlash(char *pName, int len)
{
#ifdef __ENVIRONMENT_LINUX__
	assert(pName != NULL);
	assert(len > 6);
	memset(pName, 0, len);
	if (0 != ext_get_flash_param(FLASH_PARAM_KEYBOARD_NAME, pName, 6))
	{
		printf("(%s,%d)GetKeyBoardNameFromFlash error\n", __FILE__, __LINE__);
		return -1;
	}
	printf("%s,%d,GetKeyBoardNameFromFlash:%s\n",__FILE__,__LINE__,pName);
	if ((strlen(pName) <= 0) || (strlen(pName) > 6))
	{
		return -1;
	}
	
	return CheckKeyBoardName(pName);
#endif
	return -1;
}

bool CLocalDevice::WriteCrytoInfoNew(unsigned long languageBits, unsigned char productSubID)
{
#ifdef __ENVIRONMENT_LINUX__
	unsigned char info[5] = {0};
	info[3] =  (languageBits & 0xff);
	info[2] =  ((languageBits >> 8) & 0xff);
	info[1] =  ((languageBits >> 16) & 0xff);
	info[0] =  ((languageBits >> 24) & 0xff);
	info[0] &= 0x0f;
	info[0] |= ((CRYTOINFO_NEW_FLAG)<<4);
	
	info[4] =  productSubID;

	if (0 != ext_crypto_write_data(info, 5))
	{
		return false;
	}	
#endif
	
	return true;
}

CRYTOINFO_STATUS CLocalDevice::CrytoInfoStatus()
{
	CRYTOINFO_STATUS status = CRYTOINFO_STATUS_NULL;
#ifdef __ENVIRONMENT_LINUX__	
	assert(NULL == g_pCryptoInfo);
	if (NULL == g_pCryptoInfo)
	{
		g_pCryptoInfo = new ext_dvr_product_info;
	}
	memset(g_pCryptoInfo, 0, sizeof(ext_dvr_product_info));
	ext_crypto_check_ex(g_pCryptoInfo);
	printf("product_id = %d\n", g_pCryptoInfo->product_id);
	unsigned char flage = (g_pCryptoInfo->product_language & 0xf0);
	flage >>= 4;
	if ((flage) == CRYTOINFO_NEW_FLAG)
	{
		status = CRYTOINFO_STATUS_NEW;
		printf("%s,%d,new crytoinfo\n",__FILE__,__LINE__);
	}
	else if ((flage) == CRYTOINFO_OLD_FLAG)
	{
		status = CRYTOINFO_STATUS_OLD;
		printf("%s,%d,old crytoinfo\n",__FILE__,__LINE__);
	}
	else
	{
		status = CRYTOINFO_STATUS_NULL;
		printf("%s,%d,no write crytoinfo\n",__FILE__,__LINE__);
	}	
#endif
	return status;
}

unsigned long CLocalDevice::GetSupportLanguageNew()
{
#ifdef __ENVIRONMENT_LINUX__	
	assert(NULL != g_pCryptoInfo);
	unsigned long languageBits = 0;
	unsigned long temp;

	if (NULL == g_pCryptoInfo)
	{
		g_pCryptoInfo = new ext_dvr_product_info;
		memset(g_pCryptoInfo, 0, sizeof(ext_dvr_product_info));
		ext_crypto_check_ex(g_pCryptoInfo);
	}

	temp = ((g_pCryptoInfo->resv1) & 0xff);
	languageBits += (unsigned long)temp;

	temp = ((g_pCryptoInfo->resv0) & 0xff) << 8;
	languageBits += (unsigned long)temp;

	temp = ((g_pCryptoInfo->product_type) & 0xff) << 16;
	languageBits += (unsigned long)temp;

	temp = ((g_pCryptoInfo->product_language) & 0xff) << 24;
	languageBits += (unsigned long)temp;

	return languageBits;
#else
	return 0;
#endif	
}

unsigned long CLocalDevice::GetProductSubIDNew()
{
#ifdef __ENVIRONMENT_LINUX__
	assert(NULL != g_pCryptoInfo);
	unsigned long temp;

	if (NULL == g_pCryptoInfo)
	{
		g_pCryptoInfo = new ext_dvr_product_info;
		memset(g_pCryptoInfo, 0, sizeof(ext_dvr_product_info));
		ext_crypto_check_ex(g_pCryptoInfo);
	}
    temp = g_pCryptoInfo->resv2 & 0xff;

	return temp;
#else
	return 0;
#endif
}

bool CLocalDevice::WriteFuncCustomToFlash(char* pFunc, int len)
{
	//校验位 pFunc[0] = 0xC3, pFunc[5] = 0x3C
	//中间[1],[2],[3],[4] 有效
	assert(pFunc != NULL);
	assert(len >= 6);
	pFunc[0] = 0xC3;
	pFunc[5] = 0x3C;

#ifdef __ENVIRONMENT_LINUX__
	if (0 != ext_set_flash_param(FLASH_PARAM_FUNC_CUSTOM, pFunc, 7))
	{
		return false;
	}
	return true;
#else
	FILE* file = fopen("flash.dat", "wb+");
	if(file)
	{
		fseek(file, 0, SEEK_SET);
		fwrite(pFunc, sizeof(char), len, file);
		fclose(file);
		return true;
	}
	return false;
#endif
}

bool CLocalDevice::GetFuncCustomFromFlash(char* pFunc, int len)
{
	assert(pFunc != NULL);
	assert(len >= 6);
#ifdef __ENVIRONMENT_LINUX__
	if (0 != ext_get_flash_param(FLASH_PARAM_FUNC_CUSTOM, pFunc, 7))
	{
		return false;
	}
	return true;
#else
	FILE* file = fopen("flash.dat", "rb");
	if(file)
	{
		fseek(file, 0, SEEK_SET);
		fread(pFunc, sizeof(char), len, file);
		fclose(file);
		return true;
	}
	return false;
#endif
}

bool CLocalDevice::WriteCompanyNameToFlash(char *pCompanyName, int len)
{
#ifdef __ENVIRONMENT_LINUX__
	assert(pCompanyName != NULL);
	assert(len < 32);
	if(len >= 32)
	{
		return false;
	}
	if (0 != ext_set_flash_param(FLASH_PARAM_COMPANY_NAME, pCompanyName, len))
	{
		return false;
	}
#endif
	return true;
}
bool CLocalDevice::GetCompanyNameFromFlash(char *pCompanyName, int len)
{
#ifdef __ENVIRONMENT_LINUX__
	assert(pCompanyName != NULL);
	if (0 != ext_get_flash_param(FLASH_PARAM_COMPANY_NAME, pCompanyName, len))
	{
		return false;
	}

	//把字符串后面的非0字符全部变为0，这样可以解决字符串没有结束符的问题
	for(int i = len-1 ; i >= 0; i--)
	{
		if(0 == pCompanyName[i])
		{
			break;
		}
		else
		{
			pCompanyName[i] = 0;
		}
	}

	//如果是空字符串，则给定默认的厂商名TVT
	bool bUseDefaultName = false;

	if(0 == pCompanyName[0])
	{
		bUseDefaultName = true;
	}
	else
	{
		for (int i=0; i<strlen(pCompanyName); i++)
		{
			if ((0 > pCompanyName[i]) || (127 < pCompanyName[i]))
			{
				bUseDefaultName = true;
				break;
			}
		}
	}

	if (bUseDefaultName)
	{
		strcpy(pCompanyName, "IP Camera");
	}
#else
	strcpy(pCompanyName, "IP Camera");
#endif
	
	return true;
}


bool CLocalDevice::IsUse1104A1108Color()
{
	return m_bUse1104A1108Color;
}

bool CLocalDevice::Is2316SSNewPCB()
{
	return m_b2316SSNewPCB;
}


int CLocalDevice::CheckKeyBoardName(char *pName)
{
	//下面的顺序不能修改。
	int keyboardType = -1;

	if (0 == strcmp(pName, "Q1"))        //对应于1UA按键板
	{
		keyboardType = KEYBOARD_1UA;
	}
	else if (0 == strcmp(pName, "Q2"))   //对应于1UB按键板
	{
		keyboardType = KEYBOARD_1UA;
	}
	else if (0 == strcmp(pName, "Q3"))  //对应于1UC按键板
	{
		keyboardType = KEYBOARD_1UA;
	}
	else if (0 == strcmp(pName, "Q4"))  //对应于1UD按键板
	{
		keyboardType = KEYBOARD_1UA;
	}
	else if (0 == strcmp(pName, "Q5"))  //对应于1UE按键板
	{
		keyboardType = KEYBOARD_1UA;
	}
	else if (0 == strcmp(pName, "Q6"))  //对应于1UF按键板
	{
		keyboardType = KEYBOARD_1UA;
	}
	else if (0 == strcmp(pName, "Q7"))  //对应于1UG按键板
	{
		keyboardType = KEYBOARD_1UA;
	}
	else if (0 == strcmp(pName, "Q8"))  //对应于1US(旧版)按键板
	{
		keyboardType = KEYBOARD_1US;
	}
	else if (0 == strcmp(pName, "Q9"))  //对应于1UT按键板
	{
		keyboardType = KEYBOARD_1UA;
	}
	else if (0 == strcmp(pName, "Q10")) //对应于SUA按键板
	{
		keyboardType = KEYBOARD_1UA;
	}
	else if (0 == strcmp(pName, "Q11")) //对应于SUB按键板
	{
		keyboardType = KEYBOARD_1UA;
	}
	else if (0 == strcmp(pName, "Q12")) //对应于SUC按键板
	{
		keyboardType = KEYBOARD_1UA;
	}
	else if (0 == strcmp(pName, "Q13")) //对应于1UTW按键板
	{
		keyboardType = KEYBOARD_1UTW;
	}
	else if (0 == strcmp(pName, "Q14")) //对应于2UA按键板
	{
		keyboardType = KEYBOARD_2UA;
	}
	else if (0 == strcmp(pName, "Q15")) //对应于2UB按键板
	{
		keyboardType = KEYBOARD_2UA;
	}
	else if (0 == strcmp(pName, "Q16")) //对应于2UC按键板
	{
		keyboardType = KEYBOARD_2UA;
	}
	else if (0 == strcmp(pName, "Q17")) //对应于2UD按键板
	{
		keyboardType = KEYBOARD_2UD;
	}
	else if (0 == strcmp(pName, "Q18")) //对应于1US(新版)按键板
	{
		keyboardType = KEYBOARD_1UA;
	}
	else if (0 == strcmp(pName, "Q19")) //对应于1.5U的按键板
	{
		keyboardType = KEYBOARD_2UD;
	}
	else if (0 == strcmp(pName, "Q20")) //对应于1UV的按键板
	{
		keyboardType = KEYBOARD_1UA;
	}
	else if (0 == strcmp(pName, "Q21")) //对应于1UH的按键板
	{
		keyboardType = KEYBOARD_2UD;
	}
	else if (0 == strcmp(pName, "Q22")) //对应于1UI的按键板
	{
		keyboardType = KEYBOARD_2UD;
	}
	else if (0 == strcmp(pName, "Q23")) //光面板使用1UA
	{
		keyboardType = KEYBOARD_1UA;
	}


	return keyboardType;
}


void CLocalDevice::SetCamVisible(bool bVisible)
{
	m_bCamVisible = bVisible;
}


bool CLocalDevice::SetTviPtzDeviceCtrl(int chn, BYTE* data, int len )
{
#ifdef __ENVIRONMENT_LINUX__
#if defined(__CHIP3531__) || defined(__CHIP3521__) 
	assert((chn >= 0) && (chn <m_localVideoInputNum));
	assert(data != NULL);
	assert(len == 8);

	int *pDataSend = new int[len];
	for(int i=0;i<len;i++)
	{
		pDataSend[i] = data[i];
	}
	if (0 != ext_set_ptz_device_control(chn, pDataSend, len))
	{
//		printf("set tvi ptz fail! len:%d data:0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x\n",\
//			len,pDataSend[0],pDataSend[1],pDataSend[2],pDataSend[3],pDataSend[4],pDataSend[5],pDataSend[6],pDataSend[7]);
		return false;
	}
//	printf("set tvi ptz ok! len:%d data:0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x\n",\
//			len,pDataSend[0],pDataSend[1],pDataSend[2],pDataSend[3],pDataSend[4],pDataSend[5],pDataSend[6],pDataSend[7]);

	delete []pDataSend;
#endif
#endif
	return true;
}

bool CLocalDevice::GetTviPtzDeviceCtrl(int chn, BYTE* data, int *p_len )
{
#ifdef __ENVIRONMENT_LINUX__
#if defined(__CHIP3531__) || defined(__CHIP3521__)
	assert((chn >= 0) && (chn <m_localVideoInputNum));
	assert(data != NULL);
	assert(p_len != NULL);

	int pDataGet[256];
	int len = 0;
	if (0 != ext_get_ptz_device_control(chn, (int*)pDataGet, &len))
	{
//		printf("get tvi ptz fail! len:%d data:0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x\n",\
//			len,pDataGet[0],pDataGet[1],pDataGet[2],pDataGet[3],pDataGet[4],pDataGet[5],pDataGet[6],pDataGet[7]);
		return false;
	}
	assert(len != 0 && len<=256);
	
//	printf("get tvi ptz fail! len:%d data:0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x\n",\
//			len,pDataGet[0],pDataGet[1],pDataGet[2],pDataGet[3],pDataGet[4],pDataGet[5],pDataGet[6],pDataGet[7]);
	for(int i=0;i<len;i++)
	{
		data[i] = (BYTE)(pDataGet[i]);
	}
	*p_len = len;
#endif
#endif
	return true;

}

int CLocalDevice::GetHdChipType()
{
#ifdef __ENVIRONMENT_LINUX__
	unsigned int hdtype=0;
#ifndef __PACKID_9__
	ext_get_hd_chip_type(&hdtype);
	printf("hdtype=%d\n",hdtype);	
#endif
	return hdtype;
#else
	return 0;
#endif
}

bool CLocalDevice::WriteNICTypeToFlash(char *pNICType, int len)
{
#ifdef __ENVIRONMENT_LINUX__
	assert(pNICType != NULL);
	printf("nicType:%s\n",pNICType);
	if (strlen(pNICType) > 2)
	{
		return false;
	}

	if (0 != ext_set_flash_param(FLASH_PARAM_NETDEVICE_TYPE, pNICType, 6))
	{
		return false;
	}
#endif
	return true;
}

int  CLocalDevice::GetNICTypeFromFlash()
{
#ifdef __ENVIRONMENT_LINUX__
	char name[12] = {0};

	if (0 != ext_get_flash_param(FLASH_PARAM_NETDEVICE_TYPE, name, 6))
	{

		return 0;
	}

	
	return atoi(name);
#else
	return 0;
#endif
}

void CLocalDevice::AdjustCVBSVo(unsigned long adjust)
{
// #if defined(__CHIP3531__) || defined(__CHIP3521__) || defined(__CHIP3520A__)
// 	int dispW = 640;
// #else
// 	int dispW = 640;
// #endif
// 	int dispH = 420;
// 	unsigned short w = 0;
// 	unsigned short h = 0;
// 
// 	if (VIDEO_FORMAT_PAL == g_video_format)
// 	{
// 		GetVGAResolution(VGA_720X576, w, h);
// 	}
// 	else
// 	{
// 		GetVGAResolution(VGA_720X480, w, h);
// 	}
// 	
// 	if (adjust)
// 	{
// 		if (516 < h)
// 		{
// 			dispH = 516;
// 		}
// 	}
// 	else
// 	{
// 		dispW = w;
// 		dispH = h;
// 	}

#if defined(__CHIP3531__) || defined(__CHIP3535__) || defined(__CHIP3521__) || defined(__CHIP3520A__) || defined(__CHIP3520D__) || defined(__CHIPGM__) 
	unsigned short w = 0;
	unsigned short h = 0;
	GetCVBSResolution(g_video_format, w, h, adjust);
	

	int dispW = w;
	int dispH = h;
	
	printf("%s:%s:%d, dislW=%d, dispH=%d\n", __FUNCTION__, __FILE__, __LINE__, dispW, dispH);
#ifndef WIN32
	ext_change_cvbs_output_size(dispW, dispH);
#endif
#endif
}

#ifdef __SUPPORT_OSDTWOLINES__
void CLocalDevice::GetVideoSize(int chnn, int &width, int &height)
{
#ifdef __ENVIRONMENT_LINUX__
	if (VIDEO_FORMAT_NTSC == m_videoFormat)
	{
		if (DVRVIDEO_SIZE_CIF == m_pLocalVideoSize[chnn])
		{
			width  = 352;
			height = 240;
		}
		else
		{
			width =  704;
			height = 480;
		}
	}
	else
	{
		if (DVRVIDEO_SIZE_CIF == m_pLocalVideoSize[chnn])
		{
			width  = 352;
			height = 288;
		}
		else
		{
			width =  704;
			height = 576;
		}
	}

#endif

}
#endif


void CLocalDevice::SetChosenPos(unsigned long x,unsigned long y)
{
	GUI::CFBInterface::Instance()->SetClickPos(x,y);
}
void CLocalDevice::SendInitChnn(unsigned long chnn)
{
	GUI::CFBInterface::Instance()->SendInitChnn(chnn);
}

void CLocalDevice::SetEncodeSize(int encodeSize)
{
#ifndef WIN32
	char strEncodeSize[6] = {0};

#if defined(__LANGUAGE_JAPANESE__)
	if (0 == encodeSize)
	{
		encodeSize = 1;
	}
	else
	{
		encodeSize = 0;
	}
#endif

	snprintf(strEncodeSize, sizeof(strEncodeSize), "%d", encodeSize);
	ext_set_flash_param(FLASH_PARAM_ENCODE_SIZE, strEncodeSize, 6);
#else
	FILE * pFlash = fopen("encodeType.txt", "wb+");
	if (NULL != pFlash)
	{
		fwrite(&encodeSize, sizeof(unsigned long), 1, pFlash);
		fclose(pFlash);
	}
#endif

	m_encode960H = -1;
}

int CLocalDevice::GetEncodeSize()
{
	if (-1 == m_encode960H)
	{
		m_encode960H = 0;

#ifndef WIN32
		char strEncodeSize[6] = {0};
		if (0 == ext_get_flash_param(FLASH_PARAM_ENCODE_SIZE, strEncodeSize, 6))
		{
			if (isdigit((int)strEncodeSize[0]))
			{
				m_encode960H = atoi(&strEncodeSize[0]);
			}

			if ((0 != m_encode960H) && (1 != m_encode960H))
			{
				m_encode960H = 0;
			}
		}

#if defined(__LANGUAGE_JAPANESE__)
		if (0 == m_encode960H)
		{
			m_encode960H = 1;
		}
		else
		{
			m_encode960H = 0;
		}
#endif
#else
		FILE * pFlash = fopen("encodeType.txt", "rb+");
		if (NULL != pFlash)
		{
			fread(&m_encode960H, sizeof(unsigned long), 1, pFlash);
			fclose(pFlash);
		}
#endif
	}

	return m_encode960H;
}

//////////////////////////////////////////////////////////////////////////



bool CLocalDevice::GetSNFromFlash(char *pSN, int len)
{
#ifdef __ENVIRONMENT_LINUX__
	assert(pSN != NULL);
	assert(len <= 32);
	if (0 != ext_get_flash_param(FLASH_PARAM_SN, pSN, len))
	{
		return false;
	}
#endif
	return true;
}

bool CLocalDevice::GetSNFromFlash(char *pSN)
{
#ifdef __ENVIRONMENT_LINUX__

	printf("getSNFromFlash()\n");
	if (0 != ext_get_flash_param(FLASH_PARAM_SN, pSN, 32))
	{
		return false;
	}
	printf("pSN->%s\n", pSN);
#endif
	return true;
}

bool CLocalDevice::WriteSNToFlash(char *pSN, int len)
{
#ifdef __ENVIRONMENT_LINUX__
	assert(pSN != NULL);
	assert(len <= 32);
	if (0 != ext_set_flash_param(FLASH_PARAM_SN, pSN, len))
	{
		return false;
	}
#endif
	return true;
}
