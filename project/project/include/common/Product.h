/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : Ԭʯά
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

const unsigned long		MAX_PLAYBACK_NUM			= 1;		//ͬʱ�طŵ�ͨ����Ŀ
const unsigned long		MAX_DEVICE_NAME_LEN			= 36 -4;	//�豸���Ƴ����޶�,�洢�ռ�36���ֽ�
const unsigned long		MAX_TIME_SERVER_LEN			= 132-4;	//����ʱ��ͬ����������ַ��󳤶�, �洢�ռ�132���ֽ�
const unsigned long		MAX_UPNP_FRIENDLY_NAME_LEN	= 132-4;	//UPNP�豸��ʾ��������󳤶�, �洢�ռ�132���ֽ�

#if defined(__CHIPGM__)
const unsigned long		MAX_CAMERA_NAME_LEN			= 32-4; 		//ͨ��������󳤶�,�洢�ռ�36���ֽ�
#else
const unsigned long             MAX_CAMERA_NAME_LEN                     = 36-4;
#endif

const unsigned long		MAX_SENSOR_IN_NAME_LEN		= 36-4;		//������������󳤶�,�洢�ռ�36���ֽ�
const unsigned long		MAX_ALARM_OUT_NAME_LEN		= 36-4;		//�̵������������󳤶�, �洢�ռ�36���ֽ�
const unsigned long		MAX_PPPOE_USER_NAME_LEN		= 132-4;	//PPPoE�û�����󳤶ȣ��洢�ռ�132���ֽ�
const unsigned long		MAX_PPPOE_PASSWORD_LEN		= 36-4;		//PPPoE������󳤶�, �洢�ռ�36���ֽ�
const unsigned long		MAX_DDNS_USER_NAME_LEN		= 36-4;		//DDNS�û�����󳤶�, �洢�ռ�36���ֽ�
const unsigned long		MAX_DDNS_PASSWORD_LEN		= 36-4;		//DDNS������󳤶�, �洢�ռ�36���ֽ�
const unsigned long		MAX_DDNS_DOMAIN_LEN			= 132-4;	//DDNS����������󳤶�, �洢�ռ�132���ֽ�
const unsigned long		MAX_WIRELESS_SSID_LEN		= 36-4;		//����SSID��󳤶�, �洢�ռ�36���ֽ�

const unsigned long		MAX_BUFFER_LEN				= 512*1024;

extern unsigned long	MAX_VIDEO_FRAME_LEN;

const unsigned long		MAX_SPEED_VALUE_LEN			= 12;
const unsigned char		MAX_WIFI_CFG_NUM			= 5;		//��������ά��
const unsigned char		MAX_MAIL_RECV_NUM			= 3;		//�ʼ������˵������Ŀ
const unsigned char     ALARM_EMAIL_MAX_SNAP_IMAGE_COUNT = 3;   //�ʼ���ͼƬ�����������
const unsigned char     MAX_BAN_ALLOW_LIST_NUM		= 20;		//�ڰ����������Ŀ
const unsigned long		MAX_DAILY_SNAP_NUM			= 100;		//�ճ�ץͼ��������

//const std::string       g_BitRateStr[]   = {"64 Kbs","128 Kbs","256 Kbs","512 Kbs","768 Kbs","1 Mbs", "2 Mbs"};
//const unsigned long     g_MajorBitRateValue[] = {256, 384, 512, 768, 1024, 1280, 1538};//��������ѡ��ķ�Χ����λkbs
//const unsigned long     g_MajorBitRateValue[] = {64, 128, 256, 512, 768, 1024, 2048};//��������ѡ��ķ�Χ����λkbs

//((brightness<<24) + (contrast<<16) + (saturation<<8) + hue)
#if defined(__CHIP3520__) || defined(__CHIP3515__)
#ifdef __SPECIAL_COLOR   //2316se_s���õ�ɫ��������ͬ�ú�˼оƬ�Ĳ�Ʒ��һ��
const unsigned long DEFAULT_COLOR_NTSC_25838 = ((80 << 24) + (110 << 16) + (140 << 8) + 128);  //C25838ʹ�õ�ɫ�ʣ� 2316SS������ʹ��C25838
const unsigned long DEFAULT_COLOR_PAL_25838 =  ((80 << 24) + (110 << 16) + (140 << 8) + 128);  
const unsigned long DEFAULT_COLOR_NTSC_1108 = ((102 << 24) + (130 << 16) + (132 << 8) + 128);   //1108ʹ�õ�ɫ�ʣ�2316SS������ʹ��1108
const unsigned long DEFAULT_COLOR_PAL_1108  = ((102 << 24) + (130 << 16) + (132 << 8) + 128);  
#elif defined(__PACKID_13__) //3520 960��Ʒ 2616LDS ��ɫ��ֵ ͼ��ȱʡ��ɫ�޸�Ϊ��brightness 119��hue 128��sat 128��con 106
const unsigned long DEFAULT_COLOR_NTSC_2864 = ((100 << 24) + (103 << 16) + (128 << 8) + 128);   //��������Ե�ɫ�ʣ�C25838,2864,2960
const unsigned long DEFAULT_COLOR_PAL_2864  = ((104 << 24) + (98 << 16) + (128 << 8) + 128);
const unsigned long DEFAULT_COLOR_NTSC_1104 = ((102 << 24) + (130 << 16) + (132 << 8) + 128);   //1104 1108ʹ��
const unsigned long DEFAULT_COLOR_PAL_1104  = ((102 << 24) + (130 << 16) + (132 << 8) + 128); 
#else
const unsigned long DEFAULT_COLOR_NTSC_2864 = ((100 << 24) + (100 << 16) + (140 << 8) + 128);   //��������Ե�ɫ�ʣ�C25838,2864
const unsigned long DEFAULT_COLOR_PAL_2864  = ((100 << 24) + (100 << 16) + (140 << 8) + 128);
const unsigned long DEFAULT_COLOR_NTSC_1104 = ((102 << 24) + (130 << 16) + (132 << 8) + 128);   //1104 1108ʹ��
const unsigned long DEFAULT_COLOR_PAL_1104  = ((102 << 24) + (130 << 16) + (132 << 8) + 128); 
#endif
#else
#if defined(__TDFH__)
//���ȡ��Աȶȡ����Ͷȡ�ɫ�ȵ�˳�������N��  91��111��100��131    P��104��111��100��131
const unsigned long DEFAULT_COLOR_NTSC_2864 = ((100 << 24) + (100 << 16) + (140 << 8) + 128);
const unsigned long DEFAULT_COLOR_PAL_2864  = ((100 << 24) + (100 << 16) + (140 << 8) + 128);
//const unsigned long DEFAULT_COLOR_NTSC_1104 = ((104 << 24) + (111 << 16) + (120 << 8) + 128);
//const unsigned long DEFAULT_COLOR_PAL_1104  = ((104 << 24) + (111 << 16) + (146 << 8) + 128);
//���� 122   ɫ�� 128   ���Ͷ� 120   �Աȶ� 83
const unsigned long DEFAULT_COLOR_NTSC_1104 = ((122 << 24) + (83 << 16) + (120 << 8) + 128);
const unsigned long DEFAULT_COLOR_PAL_1104  = ((122 << 24) + (83 << 16) + (120 << 8) + 128);
#else
#if defined(__CHIP3531__) || defined(__CHIP3535__)
//brightness 115 contrast 122 saturation 148 hue 128 1108оƬ
//p��ʽ ����136 �Ҷ� 128 ���Ͷ�128 �Աȶ�107 1918оƬ
//N��ʽ ����120 �Ҷ� 128 ���Ͷ�128 �Աȶ�118 1918оƬ
//3531     PAL  ����132  �Ҷ�128  ���Ͷ�128 �Աȶ�120
//3531     NTSC ����128  �Ҷ�128  ���Ͷ�148 �Աȶ�120

const unsigned long DEFAULT_COLOR_NTSC_2864 = ((128 << 24) + (120 << 16) + (148 << 8) + 128);
const unsigned long DEFAULT_COLOR_PAL_2864  = ((132 << 24) + (120 << 16) + (128 << 8) + 128);
//1104 1108
//				
//3531     PAL        ����128 �Ҷ�128    ���Ͷ�96  �Աȶ�120
//3531     NTSC       ����120 �Ҷ�128    ���Ͷ�96  �Աȶ�120
const unsigned long DEFAULT_COLOR_NTSC_1104 = ((120 << 24) + (120 << 16) + (96 << 8) + 128);
const unsigned long DEFAULT_COLOR_PAL_1104  = ((128 << 24) + (120 << 16) + (96 << 8) + 128);
#else
#if defined(__CHIP3520A__) || defined(__CHIP3520D__)
//����115 ɫ��128 ���Ͷ�90 �Աȶ�102
//����108 ɫ��130 ���Ͷ�148 �Աȶ�130
//3520a    PAL        ����132  ɫ��130  ���Ͷ�148  �Աȶ�120
//3520a    NTSC       ����128  ɫ��130  ���Ͷ�148  �Աȶ�120
#if defined(__TW01_RILI__)
//TW01�����汾��ɫ��Ĭ��ֵΪ����108 ��ɫ��130�����Ͷ�148 ���Աȶ�130
const unsigned long DEFAULT_COLOR_NTSC_2864 = ((108 << 24) + (130 << 16) + (148 << 8) + 130);
#else
const unsigned long DEFAULT_COLOR_NTSC_2864 = ((128 << 24) + (120 << 16) + (148 << 8) + 130);
#endif
const unsigned long DEFAULT_COLOR_PAL_2864  = ((132 << 24) + (120 << 16) + (148 << 8) + 130);
//3520a    PAL        ����128  ɫ��130  ���Ͷ�120 �Աȶ�120
//3520a    NTSC       ����120  ɫ��130  ���Ͷ�88  �Աȶ�120
#if defined(__TW01_RILI__)
//TW01�����汾��ɫ��Ĭ��ֵΪ����108 ��ɫ��130�����Ͷ�148 ���Աȶ�130
const unsigned long DEFAULT_COLOR_NTSC_1104 = ((108 << 24) + (130 << 16) + (148 << 8) + 130);
#else
const unsigned long DEFAULT_COLOR_NTSC_1104 = ((120 << 24) + (120 << 16) + (88 << 8) + 130);
#endif
const unsigned long DEFAULT_COLOR_PAL_1104  = ((128 << 24) + (120 << 16) + (120 << 8) + 130);



#else
#if defined(__CHIP3521__)
//����115 ɫ��128 ���Ͷ�90 �Աȶ�102
//����108 ɫ��130 ���Ͷ�148 �Աȶ�130
//3521     PAL        ����128  ɫ��130  ���Ͷ�120  �Աȶ�120
//3521     NTSC       ����128  ɫ��130  ���Ͷ�148  �Աȶ�120
const unsigned long DEFAULT_COLOR_NTSC_2864 = ((128 << 24) + (120 << 16) + (148 << 8) + 130);
const unsigned long DEFAULT_COLOR_PAL_2864  = ((128 << 24) + (120 << 16) + (120 << 8) + 130);
//3521     PAL        ����128  ɫ��130  ���Ͷ�120  �Աȶ�120
//3521     NTSC       ����120  ɫ��130  ���Ͷ�88   �Աȶ�120
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
//���ȡ��Աȶȡ����Ͷȡ�ɫ�ȵ�
const unsigned long DEFAULT_HDMI_DEV_COLOR = ((137<<24) + (128<<16) + (128<<8) + 139);
const unsigned long DEFAULT_VGA_DEV_COLOR  = ((137<<24) + (128<<16) + (128<<8) + 139);
const unsigned long DEFAULT_CVBS_DEV_COLOR = ((160<<24) + (128<<16) + (126<<8) + 139);
#elif defined(__CHIP3531__) || defined(__CHIP3535__)
//br:123 hue:104 sat:116 con:120
//���� 96 hue 128 sat 122 con 148
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
//���� 160 �Աȶ� 139  ���Ͷ� 98  �Ҷ� 100
const unsigned long DEFAULT_HDMI_DEV_COLOR = ((160<<24) + (139<<16) + (98<<8) + 100);
const unsigned long DEFAULT_VGA_DEV_COLOR  = ((160<<24) + (139<<16) + (98<<8) + 100);
const unsigned long DEFAULT_CVBS_DEV_COLOR = ((160<<24) + (142<<16) + (126<<8) + 128);
#elif defined(__CHIP3520A__) || defined(__CHIP3520D__)
//���� 128 �Աȶ� 128  ���Ͷ� 128  �Ҷ� 128
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
//���� 128 �Աȶ� 128  ���Ͷ� 128  �Ҷ� 128
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

	unsigned long max_rate_total;		//�ܹ����������KPS��
	unsigned long max_rate_chnn;		//
}FRAME_RATE;

typedef struct _ipc_frame_rate_
{
	unsigned long	max_rate_total_on_vga;	//PAL����֧�ֵ�����ͨ����D1��֡��
	unsigned long	max_rate_chnn_on_vga;	//PAL����֧�ֵĵ�ͨ�����D1֡��
	unsigned long	max_rate_chnn;			//PAL����֧�ֵĵ�ͨ�����֡��
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
	//�ɱ����
	unsigned char	localVideoInputNum;		//������Ƶ����ͨ����Ŀ
	unsigned char	audioInputNum;			//��Ƶ����ͨ����Ŀ
	unsigned char	sensorInputNum;			//����������ͨ����Ŀ
	unsigned char	relayOutputNum;			//�̵��������Ŀ
	unsigned long   displayResolutionMask;  //��������ѡ��ķֱ���

	unsigned char	videoOuputNum;			//��Ƶ�����Ŀ����֧�ֻط����ͨ������
	unsigned char	netVideoOutputNum;		//����ط����ͨ����Ŀ
	unsigned char	netVideoInputNum;		//�����źŽ���ͨ����Ŀ
	unsigned char	IVSNum;					//���ܷ���ͨ����Ŀ

	unsigned char	presetNumOneCH;			//ÿ��ͨ��Ԥ�õ���Ŀ
	unsigned char	cruiseNumOneCH;			//ÿ��ͨ��Ѳ������Ŀ
	unsigned char	presetNumOneCruise;		//ÿ��Ѳ���ߵ�Ԥ�õ���Ŀ
	unsigned char	trackNumOneCH;			//ÿ��ͨ���켣��Ŀ

	unsigned char	userNum;				//�û���Ŀ 
	unsigned char	netClientNum;			//���ͻ�����Ŀ
	unsigned char	netFirstStreamNum;		//�����������ͨ�������Ŀ����ͬʱ�����м����ͻ��˲鿴������
	unsigned char	deviceType;				//�豸����

	unsigned char	doblueStream;			//�Ƿ����ṩ˫����
	unsigned char	audioStream;			//�Ƿ����ṩ��Ƶ��
	unsigned char	talkAudio;				//�Ƿ��жԽ�����: 1��ʾ�жԽ�����;0��ʾû��
	unsigned char	bPasswordCheck;			//�����Ƿ�Ҫ��������

	unsigned char	defBrightness;			//ȱʡ����
	unsigned char	defContrast;			//ȱʡ�Աȶ�
	unsigned char	defSaturation;			//ȱʡ���Ͷ�
	unsigned char	defHue;					//ȱʡɫ��

	unsigned short	videoInputNum;			//��Ƶ����ͨ����Ŀ�����ؼ����磩
	unsigned short  deviceID;				//�豸ID��
    unsigned long   videoFormat;            //ϵͳ��ǰ��ʽ

	//������FUNC_REMOTE_UPGRADE��Ӧ�Ĺ��ܣ���ô��FUNC_REMOTE_UPGRADE������Ϊ1������Ϊ�㡣
	unsigned long	function[8];			//��������

	unsigned long	deviceIP;				//�豸�����ַ
	unsigned char	deviceMAC[8];			//�豸�����ַ

	unsigned long	buildDate;				//��������:year<<16 + month<<8 + mday
	unsigned long	buildTime;				//����ʱ��:hour<<16 + min<<8 + sec

	char			deviceName[36];			//�豸����

	char	firmwareVersion[36];			//�̼��汾
	char	kernelVersion[64];				//�ں˰汾
	char	hardwareVersion[36];			//Ӳ���汾
	char	MCUVersion[36];					//��Ƭ���汾
}PRODUCT_INFO;

typedef struct __channel_rec_param__
{
	unsigned long	supVS;	//��֧�ֱַ���
	unsigned long	dfVS;	//Ĭ�Ϸֱ���
}CHANNEL_RECP;

typedef struct __VSSupBrr__
{
	unsigned long resolution;	//��Ӧ�ķֱ���
	unsigned long vsforbrr[20];//ÿ���ֱ���֧����Щ����
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
#elif defined(__CHIP3531__)//3531ȫ��֧��d1��cifץͼ
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



	//���ڱ��أ����磩֡�� ,�����ͷֱ���
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
	unsigned long  writeDefaultResolution();//дĬ�Ϸֱ��ʵ�flash
	
		//�����ź���ز����ӿ�
	unsigned long *GetDigitChannelSupportVideoSize(){return m_pSupportVideoSize;}
	unsigned long GetDigitMaxTotalBitRate(){return m_ipcMaxTotalBitRate;}
	unsigned long IPCIsLimitByBitRate(){return m_bLimitByBitRate;}

	unsigned long IPCMaxVideoOutNum(){return m_ipcMaxVideoOutNum;}

	unsigned long DefaLiveAudioState(){return m_defaLiveAudioState;}

	//���dvr��ؽӿ�
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

	void SetHDParam();//���ø�����Ƶ����

	void AdjustParam();
	void SetChannelRecp(unsigned long ch, VIDEO_SIZE supVS, VIDEO_SIZE defVS, CHANNEL_RECP * pChBrr);
	void SetVSFOrBrr(unsigned long productID,unsigned long resolution);//����ÿ���ֱ��ʶ�Ӧ��������Ŀǰ�����ڻ������

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
	unsigned long   m_preRecordType; //Ԥ¼������ͣ����ڴ淽ʽ����Ӳ�̷�ʽ
	unsigned long	m_preAlarmTime;
	unsigned char	m_External485KeyboardType;
	unsigned char	m_notuse[3];
	unsigned char   m_maxFFSpeed;             //��ͨ�������������֡���롣 
	                                          //�����128����ʾ��Ʒ�������Լ����
	                                          //���ǲ���Ĭ�ϣ����ܼ�·�طţ����ֻ������4������֡���롣
	unsigned char   m_chnnSendFirstStreamMaxNum; //ÿ��ͨ��������������������ÿ��ͨ�������������ķ����������ƣ�����ÿ��ͨ���ܹ����Է��ͼ�����������
	                                             //������������������ֳ���������Զ�̻ط�������Զ�̱�������.��ĿǰΪ�������Ϊ1.

	bool            m_bLimitFirstStreamSendNum; //�Ƿ����ƶ�����DVR�����������ķ��������ơ�ͨ������������������͹ر��������
	                                            //���������m_netVideoOutputNum�����ǣ�m_netVideoOutputNum��ʾ�ͻ���֧�ּ�·�ԱȻطš�	
	                                            //Ŀǰֻ��2508FD��2516FD������
	unsigned char   m_sendFirstStreamMaxNum;    //����DVR�����������ķ���.������������ֵΪm_chnnSendFirstStreamMaxNum*m_localVideoInputNum
	                                            //һ�㲻ҪС�ڣ�m_netVideoOutputNum

	//////////////////////////////////////////////////////////////////////////
	bool            m_bSupportRedunRec;
	bool			m_bSupportESATA;
	unsigned long   m_eSataIndex;       //m_eSataIndex��ʾ�ڼ���sata����esata����1��ʼ����
	unsigned char   m_pciToEsataNum;     //���������ʾ��PCIת��esata��������2011-1-8Ϊֹ��ֻ��2432MD��2424MDʹ�õ���pciתesata
	unsigned char   *m_pPciToEsataIndex;  //PCIת��esata�ı�ţ���2011-1-8Ϊֹ�������9,10����1��ʼ������
	bool            m_bHaveUsbToEsata;       //���������ʾ�Ƿ���usbת��esata��
	char            m_szUsbToEsataPath[128]; //���������ʾ��ת��esata��usb��·��
	bool			m_bDoDiskIODetect;
	bool			m_bAudioAction;
	bool			m_bSupportMCU;
	bool            m_bHaveSpot;
	bool            m_bUseMCUSpot;     //�Ƿ�ʹ�õ�Ƭ����spot��Ŀǰֻ��24XXMDϵ�в�Ʒ��ʹ�õ�Ƭ����spot����
	unsigned char   m_speekType;    //�Խ�����.0-û�жԽ���1-�ж����ĶԽ����롣2-��ͨ��0���Խ����롣
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
	unsigned long   m_maxStreamIDNumForPlayOrBackup;//DVR���֧�ֶ��ٸ��ͻ��������streamID��

	unsigned long   m_netSendCopies;             //�����Է����������ķ�������Ҫ����2516FD����Ϊ2516FD���͵���ʵʱ��������������������
												 //���Զ�2516FD�������ƣ������Ϊ24��������ƷΪ�����ƣ�ΪINVALID_INDEX(type)
	unsigned long   m_netlimitSendCopies;           //��ĳЩ����������������֡�����ƶ��١�2516FD�ڱ���ʱ�����ĵ����ܲ��ϴ�
												 //����ʱ�ڷ���ʵʱ��������,�������޷��е���
												 //���Զ�2516FD�������ƣ����Ƶ�5��������ƷΪ�����ƣ�ΪINVALID_INDEX(type)

	bool            m_bLimitSecondStreamForFirstStreamNum;
	unsigned long   m_firstStreamNumForLimitSecondStream;
	unsigned long   m_limitTypeForFirstStreamNum;
	unsigned long   m_netLimitRatForFirstStreamNum;
	unsigned long   m_netLimitAllRatForFirstStreamNum;
	unsigned long	m_firstStreamLimitBitRate;


	//���ڱ��أ����磩֡�� ,�����ͷֱ���
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

	unsigned long   m_bLocalVideoSizeSpecial;     ////�Ƿ�֧�������¼������
	ULONGLONG       m_localVideoSizeCIFChnnBits;  //�������ó�CIF��ͨ������
	ULONGLONG       m_localVideoSizeD1ChnnBits;    //�������ó�D1��ͨ������

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
	bool			m_bHardwareS7;				//�Ƿ�Ӳ���汾�к���"S7�����ߡ�s7��
	//
	unsigned long  m_productSubIDInHardVerion;  //��ʾ��Ӳ���汾�ŵĻ����Ӱ汾�ţ�һ���ǵ���m_productSubID

	bool m_b2704HDNewHardWare; //�Ƿ�2708XDF��Ҳ����2704HD����������

	//�����ź���صĲ���
	unsigned long	m_ipcVideoDefaSize;
	unsigned long	m_ipcDefaChnnRate;	//PAL���µ�ֵ
	unsigned long	m_ipcDefaBitRate;
	bool m_bIPCUseStaticBitRate;
	unsigned long	m_ipcDefaVideoEncodeMode;
	unsigned long	m_ipcNetVideoDefaSize;
	unsigned long	m_ipcNetDefaChnnRate;	//PAL���µ�ֵ
	unsigned long	m_ipcNetDefaBitRate;
	unsigned long	m_ipcNetDefaVideoEncodeMode;
	IPC_FRAME_RATE	m_ipcFrameRate;		//PAL���µ�ֵ
	unsigned long	*m_pSupportVideoSize;
	unsigned long	m_ipcMaxTotalBitRate;
	unsigned long	m_bLimitByBitRate;	//��NVR����������оƬCPU����ʱ��NVR�����ܾ�ȡ�������ʵĴ�С��
										//��NVR����������оƬ�Ľ�������ʱ��NVR�����ܾ�ȡ����֡�ʣ���Ϊtrueʱ��ÿ��ͨ���ķֱ���֡��Ϊƽ�����ֵ
	unsigned long	m_ipcMaxVideoOutNum;	//IPC�ط����ͨ����

	unsigned long	m_ipcTransferChnnNum;	//����ͨ������ת�����ͻ��˵�ͨ��·��

	unsigned long	m_defaLiveAudioState;

	unsigned long	m_pbChannelNumForLimitFirstStream;

	bool			m_bSupportDisplayMinor;	//NVR�Ƿ�֧���ֳ���ʾС����

	unsigned char	m_supportPrimaryStreamReplayNum; //�豸��֧��ͬʱ�ط��������������Ŀ

	CEncodeCheck	* m_pEncodeCheck;
	CCalculateSubEncode * m_pSubEncodeCheck;
	VS_SUPBRR *m_pvsBrr;//9���ֱ��ʣ�ÿ���ֱ��ʶ�Ӧ������,����ΪQCIF��CIF,HD1,D1,HD1080,HD720,960H,VGA,QVGA
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

