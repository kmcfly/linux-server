#ifndef _NET_CONFIG_DEFINE_V3_H_
#define _NET_CONFIG_DEFINE_V3_H_

namespace IPC_V3
{

//һ������²��ɸı��ֵ�������Ƿ�Ҫ���Ĵ�ֵ��Ҫ���ۺ�ȷ��
const unsigned long		NCFG_VERSION					=	0x01;

#pragma pack(4)

typedef struct _ncfg_item_head
{
	unsigned short	itemID;		//ID
	unsigned short	num;		//Ԫ����Ŀ��������ٸ�ͨ����ÿ��ͨ����Ӧһ��Ԫ��
	unsigned short	len;		//�����ܳ���
	unsigned short	subLen;		//ÿ��Ԫ�صĳ���
}NCFG_ITEM_HEAD;

typedef struct _ncfg_block_head
{
	unsigned long	biSize;		//���ṹ�峤��
	unsigned long	ItemNum;	//Ԫ����Ŀ
	unsigned long	netcfgver;	//��ǰ����İ汾�ţ��豸�˹̶�����NCFG_VERSION���ͻ��˸�����Ҫ��̬�ж�
}NCFG_BLOCK_HEAD;

typedef struct _ncfg_item_head_ex
{
	NCFG_ITEM_HEAD	item_head;
	unsigned long	start_pos;		//�ļ���ƫ��
}NCFG_ITEM_HEAD_EX;		

typedef struct _ncfg_block_info
{
	NCFG_ITEM_HEAD_EX itemHead;
	unsigned long	 off;				//������ƫ��
}NCFG_BLOCK_INFO;

typedef enum _config_page_index
{
	///////////ϵͳ����
	PAGE_BASENUM_SYS_CONFIG = 0x1000,
	PAGE_SYS_CONFIG ,  ///////ϵͳ���� ������
	PAGE_SYS_BASIC_CONFIG , /////ϵͳ�������ã� ������
	PAGE_SYS_TIME_CONFIG ,  ////////ϵͳʱ������
	PAGE_SYS_SDCARD_MANAGER_CONFIG,  ////SD������
	PAGE_SYS_DEFAULT_CONFIG , //////ϵͳ�ָ�ȱʡֵ������
	PAGE_SYS_DISK_MANAGER_CONFIG , ////���̹���
	PAGE_SYS_REBOOT , //////ϵͳ����
	PAGE_END_SYS_CONFIG ,


	////////////ͨ������
	PAGE_BASENUM_CAM_CONFIG = 0x2000,
	PAGE_CAM_CONFIG ,  //////ͨ������������
	PAGE_CAM_BASIC_CONFIG ,	 ///////////ͨ����������
	PAGE_CAM_IMAGE_CONFIG ,//////////ͨ��ͼ�����ã�ɫ�ʵ���
	PAGE_CAM_PTZ_CONFIG , //////////ͨ����̨����
	PAGE_CAM_MOTION_CONFIG , //////////ͨ��Motion��������
	PAGE_CAM_NET_IMAGE_CONFIG , //////////����ͼ������
	PAGE_CAM_RECORD_CONFIG ,//////////ͨ��¼������
	PAGE_CMA_RECORD_SCHEDULE_CONFIG, //////////¼���ų�����
	PAGE_CAM_MAP_REMOTE_CHNN_CONFIG, //////////ӳ��Զ���豸ͨ������
	PAGE_CAM_SET_OSD_MASK,	//////////��Ƶ�ڵ�������
	PAGE_CAM_AUDIO_PARA,    //////////��Ƶ����
	PAGE_CAM_VIDEO_ROI,     ////////// ROI����
	PAGE_CAM_CONFIG_END,  


	/////////��������
	PAGE_BASENUM_ALARM_CONFIG = 0x3000,
	PAGE_ALARM_CONFIG,  /////////��������������
	PAGE_ALARM_BASIC_CONFIG,//////////������������
	PAGE_ALARM_MOTION_CONFIG, //////////Motion��������
	PAGE_ALARM_MOTION_SCHEDULE_CONFIG, //////////Motion�ų�����
	PAGE_ALARM_SENSOR_CONFIG,  //////////Sensor��������
	PAGE_ALARM_SENSOR_SCHEDULE_CONFIG, //////////Sensor�ų�����
	PAGE_ALARM_IN_OTHER_CONFIG, //////������������
	PAGE_ALARM_IN_OTHER_SCHEDULE_CONFIG, ////�������������ų�
	PAGE_ALARM_OUT_CONFIG, //////////�����������
	PAGE_ALARM_OUT_SCHEDULE_CONFIG, //////////��������ų�����
	PAGE_ALARM_CONFIG_END,



	/////////////////��������
	PAGE_BASENUM_NET_CONFIG = 0x4000,
	PAGE_NET_CONFIG,  ///////////������
	PAGE_NET_BASIC_CONFIG, //////////��·��������
	PAGE_NET_IP_CONFIG, //////////IP����
	PAGE_NET_WIRELESS_CONFIG,//////////��������
	PAGE_NET_DDNS_CONFIG,//////////DDNS����
	PAGE_NET_IP_CHANGE_NOTIFY,//////////IP�仯֪ͨ
	PAGE_NET_RTSP_CONFIG,//////////RTSP����
	PAGE_NET_UPNP_CONFIG,//////////UPNP����
	PAGE_NET_CHUANTOU,	//���紩͸
	PAGE_NET_ECMS_INFO,
	PAGE_NET_BONJOUR_CONFIG,
	PAGE_NET_CONFIG_END,


	/////////////////�û�����
	PAGE_BASENUM_USER_CONFIG = 0x5000,
	PAGE_USER_CONFIG, ////������
	PAGE_USER_CONFIG_END,


	///////////////////֪ͨ����
	PAGE_BASENUM_NOTIFY_CONFIG = 0x6000,
	PAGE_NOTIFY_CONFIG, /////������
	PAGE_NOTIFY_FTP_CONFIG,//////////FTP����
	PAGE_NOTIFY_MAIL_CONFIG, //////////Mail����
	PAGE_NOTIFY_CONFIG_END,


	///////////////////////�߼�����
	PAGE_BASENUM_ADVANCED_CONFIG = 0x7000,
	PAGE_ADVANCED_CONFIG, /////������
	PAGE_ADVANCED_SAFE_CONFIG, //////////��ȫ����
	PAGE_ADVANCED_UPGRADE_CONFIG, //////����
	PAGE_SYS_IMPORT_EXPORT_CONFIG,///���뵼������
	PAGE_LOG_QUERY	,			//��־��ѯ
	PAGE_ONVIF_CONFIG,			//ONVIF�ͻ�������
	PAGE_SPVMN_CONFIG,
	PAGE_ADVANCED_CONFIG_END,
	//////////
	PAGE_BASENUM_PTZ_CONFIG	=0x8000,
	PAGE_PTZ_CONFIG,//������
	PAGE_PTZ_PRESET_CONFIG,//Ԥ�õ�����
	PAGE_PTZ_CRUISE_CONFIG,//Ѳ��������
	PAGE_PTZ_UPDATE_CONFIG, ////��̨����
	PAGE_BASENUM_PTZ_CONFIG_END,
}CONFIG_PAGE_INDEX;  /////////����

typedef enum cfg_item_id
{
	NCFG_ITEM_NULL=0x0000,		//�����������һЩ�����в������������

//�˷�ΧID�洢ϵͳ�������õ�����
	NCFG_ITEM_SYSTEM_BASE=0x100,		
	NCFG_ITEM_DEVICE_NAME,				//�豸���ƣ�NCFG_INFO_SHORT_NAME
	NCFG_ITEM_VIDEO_FORMAT,				//��Ƶ��ʽ��unsigned char ,��Ƶ��ʽ	
	NCFG_ITEM_DATE_FORMAT,				//���ڸ�ʽ��unsigned char
	NCFG_ITEM_TIME_SYNC,				//�Ƿ�����ʱ��ͬ����unsigned char
	NCFG_ITEM_TIME_ZONE,				//ʱ����long
	NCFG_ITEM_TIME_SERVER,				//����ʱ��ͬ����������ַ��LONG_NAME
	NCFG_ITEM_SEND_MAIL_SERVER,			//MAIL���ð�����������ַ�ͽ���MAIL��ַ�б�,N_EMAIL_INFO
	NCFG_ITEM_FTP_SERVER,               //FTP�������ṹ�����������MAIL�ķ�ʽ
	NCFG_ITEM_USER,						//�û���Ϣ��USER [MAX_USER_NUM]
	NCFG_ITEM_FTP_UPLOAD_PIC,			//����  FTP���������ϴ�һ��ͼƬ   ����FTP ����һ���ϴ�����ͼƬ  long
	NCFG_ITEM_LIVE_SUPPORT_PTZ,			//�ֳ��Ƿ�֧��PTZ������ƣ�unsigned char
	NCFG_ITEM_TIME_CITY,				//���ĸ����� int
	NCFG_ITEM_DAYLIGHT,					//�Ƿ�������ʱ unsigned char
	NCFG_ITEM_SHOW_DEV_NAME,//�Ƿ���ʾ�豸����.1 ��ʾ;0 ����ʾ  ,unsigned long
	NCFG_ITEM_DEV_NAME_POS,//�豸������ʾλ��  1 ���� 2 ���� 3 ���� 4 ����,  unsigned long
	NCFG_ITEM_TIME_SYNC_EX, // NCFG_SYNCHRONIZE_INFO
	NCFG_ITEM_TIME_ZONE_EX, // NCFG_TIMEZONE_INFO
	NCFG_ITEM_DEV_NAME_EX,  // NCFG_INFO_DEV_NAME_EX
	NCFG_ITEM_SYSTEM_END,
	
//ͨ������
	NCFG_ITEM_CHANNEL_BASE=0x200,
	NCFG_ITEM_PTZ_CONFIG,               //ÿ��ͨ������̨��Э�顢��ַ�������� NCFG_INFO_PTZ_CONFIG
	NCFG_ITEM_CHANNEL_BASE_END,

	NCFG_ITEM_CHANNEL_COLOR_BASE=0x250, //ͨ��ɫ������
	NCFG_ITEM_COLOR_BRIGHTNESS,          //����   unsigned char
	NCFG_ITEM_COLOR_CONTRAST,            //�Աȶ�  unsigned char
	NCFG_ITEM_COLOR_SATURATION,          //���Ͷ�  unsigned char
	NCFG_ITEM_COLOR_HUE,                 //�Ҷ�    unsigned char
	NCFG_ITEM_IMAGE_MIRROR,				//ͼ���� unsigned char
	NCFG_ITEM_IMAGE_FILP,				//ͼ��ת unsigned char
	NCFG_ITEM_WHITE_BALANCE,			//��ƽ��  NCFG_WHITE_BALANCE
	NCFG_ITEM_IMAGE_DYNAMIC_RANGE,		//��̬ NCFG_DYNAMIC_RANGE
	NCFG_ITEM_DENOISE,                     //����
	NCFG_ITEM_IRISTYPE,                     //�Զ��۽�
	NCFG_ITEM_SHARPEN,                      //��
	NCFG_ITEM_PICTURE_BIND_CHN,           //ͼƬ�󶨵��ĸ�ͨ��
	NCFG_ITEM_DAY_NIGHT_MODE,                  //IR-Cut ģʽ,unsigned long
	NCFG_ITEM_JPEG_LIVE_CHN,       // ͼƬ���󶨵��ĸ�ͨ��,char,0 CIF,1 D1
	NCFG_ITEM_CHANNEL_COLOR_END,

	NCFG_ITEM_CHANNEL_COLOR_EX_BASE=0xf30,    //panjy e00-f30 Լ304��
	NCFG_ITEM_WHITE_BALANCE_9321L,         //panjy 8126 9321L��ƽ��
	NCFG_ITEM_WHITE_BALANCE_9314,          //panjiyong 8126 9314��ƽ��
	NCFG_ITEM_IMAGE_DYNAMIC_RANGE_9314,    //panjy 8126 9314��̬
	NCFG_ITEM_DAY_NIGHT,                   //panjy ��ҹģʽ
    // huxinhua add expose set
    NCFG_ITEM_EXPOSE_TYPE,  // expose type : auto  manual   unsigned long
    NCFG_ITEM_EXPOSE_VALUE, // unsigned long
    NCFG_ITEM_VIDEO_ENCODE_TYPE,    //   unsigned long [VIDEO_INPUT_NUM] NCFG_ENCODE_H264 NCFG_ENCODE_MJPEG
    // huxinhua add end
	// ysj
	NCFG_ITEM_EXPOSURE_CTRL, 			// �ع����
	NCFG_ITEM_FOCUS_CTRL,				// �۽�����
	NCFG_ITEM_DIS,						// ���ַ��� Digital image stabilization
	NCFG_ITEM_Shading,					// ���ǲ���
	NCFG_ITEM_GAMMA,					// gamma
	NCFG_ITEM_ANTI_FLICKER,				// ����˸
	NCFG_ITEM_CRStrength,				// Crosstalk Removal ǿ��
	NCFG_ITEM_SlowFrameRate,			// ��֡��
	// ysj
	NCFG_ITEM_FOG_REDUCTION,			// ͸�� FOG_REDUCTION_CONFIG
	NCFG_ITEM_LENS_DISTORTION,			// ��ͷ���� LENS_DISTORTION_CONFIG
	NCFG_ITEM_BLC,                      // ���ⲹ�� unsigned char
	NCFG_ITEM_ANTI_OVEREXPOSURE,        // ������ unsigned char
	NCFG_ITEM_DAY_NIGHT_SENSITIVITY,// unsigned char
	NCFG_ITEM_NIGHT_TO_DAY,         // unsigned long
	NCFG_ITEM_DAY_TO_NIGHT,         // unsigned long
	NCFG_ITEM_ROI,                  // ROI���� 
	NCFG_ITEM_VIDEO_GAIN,           // ���� VIDEO_GAIN
	NCFG_ITEM_DIGITA_DENOISE,       // ���ֽ��� DIGITA_DENOISE
	NCFG_ITEM_IMAGE_ROTATO,         // ͼ����ת(����ģʽ) IMAGE_ROTATO
	NCFG_ITEM_DAY_TO_NIGHT_SENSITIVITY,	// unsigned char
	NCFG_ITEM_NIGHT_TO_DAY_SENSITIVITY,	// unsigned char
	NCFG_ITEM_TIME_FOCUS_MODE,				//�Խ�ģʽ unsigned char
	NCFG_ITEM_IRCHANGE_FOCUS,			//��ҹ�л�ʱ�Խ� unsigned char
	NCFG_ITEM_INTERVAL_TIME,			//�Խ����ʱ�� unsigned long
	NCFG_ITEM_CHANNEL_COLOR_EX_END,
	
	NCFG_ITEM_CHANNEL_TIME_STAMP_BASE=0x260,//ͨ��ʱ���
	NCFG_ITEM_TIME_STAMP_ENABLE,		//�Ƿ��ʱ���, unsigned char[VIDEO_INPUT_NUM]
	NCFG_ITEM_TIME_STAMP_POS,			//ʱ���λ��, NCFG_INFO_TIME_POS[VIDEO_INPUT_NUM]	
	NCFG_ITEM_VIDEO_OUTPUT_FORMAT,  //��Ƶ�����ʽ
	NCFG_ITEM_GET_MAX_OSD_REGIN_COUNT,         //OSD��������ʾ��������
	NCFG_ITEM_SET_OSD_CONTENT,                  //ÿ������ľ�������
	NCFG_ITEM_SET_OSD_MASK,                  // ������Ƶ�ڵ�������
	NCFG_ITEM_TIME_OSD_EX,  ///  NCFG_TIME_STAMP_CONFIG
	NCFG_ITEM_CHANNEL_TIME_STAMP_END,

	NCFG_ITEM_SCHEDULE_BASE=0x300,
	NCFG_ITEM_SENSOR_IN_SCH_WEEK,		//����������ܼƻ��� WEEK_SCHEDULE[SENSOR_INPUT_NUM]
	NCFG_ITEM_SENSOR_IN_SCH_HOLIDAY,	//�����������ռƻ�HOLIDAY_SCHEDULE [MAX_HOLIDAY_NUM][SENSOR_INPUT_NUM]
	NCFG_ITEM_MOTION_SCH_WEEK,			//�ƶ�����ܼƻ�WEEK_SCHEDULE[VIDEO_INPUT_NUM]
	NCFG_ITEM_MOTION_SCH_HOLIDAY,		//�ƶ������ռƻ�HOLIDAY_SCHEDULE [MAX_HOLIDAY_NUM][VIDEO_INPUT_NUM]
	NCFG_ITEM_SCHEDULE_END,

	NCFG_ITEM_AUDIO_BASE=0x350, 		// ��Ƶ����
	NCFG_ITEM_AUDIO_PARA_CAPACITY,		// �û���ȡ�豸��Ƶ����֧����
	NCFG_ITEM_AUDIO_PARA,
	NCFG_ITEM_AUDIO_ENCODE,				// ��Ƶ���룬unsigned char
	NCFG_ITEM_AUDIO_IN_TYPE,			// ��Ƶ�������ͣ�unsigned char
	NCFG_ITEM_AUDIO_END, 				// ��Ƶ����
	
//��ȫ���
	NCFG_ITEM_SECURITY_BASE=0x400,
	
	NCFG_ITEM_SECURITY_IPFILTER_BASE=0x410,//IP��ַ����	
	NCFG_ITEM_FILTER_IP_ENABLE,			//�Ƿ�����IP����, unsigned char
	NCFG_ITEM_FILTER_IP_MODE,			//����ģʽ���ܾ���Ҫ���˵ĵ�ַ�������ֻ������˵ĵ�ַ����, unsigned char
	NCFG_ITEM_FILTER_IP_LIST,			//��Ҫ���˵�IP��ַ, NCFG_INFO_FILT_IP [MAX_FILTER_IP_NUM]
	NCFG_ITEM_SECURITY_IPFILTER_END,

	NCFG_ITEM_SECURITY_MACFILTER_BASE=0x420,//MAC��ַ����
	NCFG_ITEM_FILTER_MAC_ENABLE,		//�Ƿ�����MAC����, unsigned char
	NCFG_ITEM_FILTER_MAC_MODE,			//����ģʽ���ܾ���Ҫ���˵ĵ�ַ�������ֻ������˵ĵ�ַ����, unsigned char
	NCFG_ITEM_FILTER_MAC_LIST,			//��Ҫ���˵�MAC��ַ, MAC [MAX_FILTER_MAC_NUM]
	NCFG_ITEM_SECURITY_MACFILTER_END,

	NCFG_ITEM_SECURITY_END,

//���繦��
	NCFG_ITEM_NET_BASE=0x500,
	NCFG_ITEM_UPNP_ENABLE,				//�Ƿ�����UPNP����, unsigned char
	NCFG_ITEM_UPNP_FRIENDLY_NAME,		//UPNP�豸��ʾ������, LONG_NAME
	NCFG_ITEM_HTTP_PORT,				//HTTP �˿�, unsigned short
	NCFG_ITEM_SERVER_PORT,				//�������˿ڣ� unsigned short	
	NCFG_ITEM_DNS_1,					//ULONGLONG
	NCFG_ITEM_DNS_2,					//ULONGLONG
	NCFG_ITEM_RTSP_PORT,				//unsigned short
	NCFG_ITEM_RTSP_ENABLE,				//unsigned char,�Ƿ�����RTSP����
	NCFG_ITEM_RTSP_ENABLE_ANON,			//unsigned char,RTSP�Ƿ��ܹ�������¼
	NCFG_ITEM_RTSP_STREAM_NAME,			//SHORT_NAME,RTSP��ַ����ı��
	NCFG_ITEM_RTSP_ENABLE_AUDIO,		//�Ƿ�Ҫ��������
	NCFG_ITEM_UPNP_ENABLE_MAP,			//�Ƿ�����UPNP����, unsigned char
	NCFG_ITEM_CHUANTOU_ALIAS,			//����ֻ���������ȡ������ͨ������������
	NCFG_ITEM_BONJOUR_ENABLE,			//�Ƿ�����Bonjour����, unsigned char
	NCFG_ITEM_BONJOUR_FRIENDLY_NAME,		//Bonjour�豸��ʾ������, LONG_NAME
	NCFG_ITEM_NET_BASE_END,

	NCFG_ITEM_DDNS_BASE=0x540,			//DDNS,���ܺ��滹��Ӽ�������
	NCFG_ITEM_DDNS_ENABLE,              //DDNS�Ƿ���, unsigned char
	NCFG_ITEM_DDNS_TYPE,				//DDNS���ͣ�unsigned char
	NCFG_ITEM_DDNS_USER_NAME,			//DDNS�û�����SHORT_NAME
	NCFG_ITEM_DDNS_PASSWORD,			//DDNS���룬SHOR T_NAME
	NCFG_ITEM_DDNS_SERVER_NAME,			//DDNS��������Ϣ��NCFG_INFO_LONG_NAME
	NCFG_ITEM_DDNS_DOMAIN_INFO,			//DDNS������Ϣ��NCFG_INFO_LONG_NAME
	NCFG_ITEM_DDNS_END,

	NCFG_ITEM_NET_WIRELESS_BASE=0x550,  //����������أ����ֻ�����һ��ID���ŵ�ǰ��Ϳ�����
	NCFG_ITEM_WIRELESS_ENABLE,			//�������� unsigned char
	NCFG_ITEM_WIRELESS_IPMODE,			//���IP��ģʽ,unsigned char
	NCFG_ITEM_WIRELESS_IP,              //����IP
	NCFG_ITEM_WIRELESS_SUB_NET_MASK,    //������������
	NCFG_ITEM_WIRELESS_GATEWAY,         //��������
	NCFG_ITEM_WIRELESS_DNS,             //����DNS
	NCFG_ITEM_WIRELESS_SSID,            //����SSID
	NCFG_ITEM_WIRELESS_AUTHENTICATION_MODE,//������֤ģʽ
	NCFG_ITEM_WIRELESS_ENCRYPT_TYPE,    //��������
	NCFG_ITEM_WIRELESS_CHANNEL,			//�ŵ�	unsigned char 1--11
	NCFG_ITEM_WIRELESS_WHICH_KEY,		// ʹ���ĸ���Կ unsigned char 1 2 3 4	
	NCFG_ITEM_WIRELESS_KEY1,            //��Կ1
	NCFG_ITEM_WIRELESS_KEY2,            //��Կ2
	NCFG_ITEM_WIRELESS_KEY3,            //��Կ3
	NCFG_ITEM_WIRELESS_KEY4,            //��Կ4
	NCFG_ITEM_WIRELESS_DNS2,            //����DNS
	NCFG_ITEM_NET_WIRELESS_END,
	
	NCFG_ITEM_NET_IPNOTIFY_BASE=0x570,  //����IP֪ͨ��ʽ
	NCFG_ITEM_IP_CHG_TO_EMAIL_ENABLE,	//IP�ı䴥���ʼ�(�˿ڱ仯һ������)��unsigned char
	NCFG_ITEM_IP_CHG_TO_EMAIL_ADDR,		//IP�ı䴥��MAIL��ַ�б�unsigned char [n],�����EMAIL��ID
	NCFG_ITEM_IP_CHG_TO_FTP_ENABLE,     //IP�ı䴥��FTP�ϴ���unsigned char
	NCFG_ITEM_IP_CHG_TO_FTP_ADDR,		//IP�ı䴥��FTP��ַ�б�unsigned char [n],�����FTP��ID
	NCFG_ITEM_IP_CHG_TO_EMAIL_ADDR_V2,
	NCFG_ITEM_NET_IPNOTIFY_END,

	NCFG_ITEM_OLD_BASE=0x5F0,
	NCFG_ITEM_GET_IP_MODE,				//��ȡIPģʽ��unsigned char
	NCFG_ITEM_STATIC_IP,				//��̬IP��ַ��ULONGLONG������64IP��
	NCFG_ITEM_GATEWAY,					//���أ�ULONGLONG
	NCFG_ITEM_SUB_NET_MASK,				//�������룬ULONGLONG
	NCFG_ITEM_PPPOE_ENABLE,				//PPPoE�Ƿ�������, unsigned char
	NCFG_ITEM_PPPOE_USER_NAME,			//PPPoE�û���,	LONG_NAME
	NCFG_ITEM_PPPOE_PASSWORD,			//PPPoE����,	SHORT_NAME
	NCFG_ITEM_START_AUTOSEARCH_ROUTE,	//�Զ�����·�������� unsigned char 0 �رգ���0 ����
	NCFG_ITEM_ENABLE_CHUANTOU,
	NCFG_ITEM_GET_DNS_MODE,
	NCFG_ITEM_SET_NIC_MODE,             // ������������ģʽ
	NCFG_ITEM_OLD_END,

	NCFG_ITEM_NET_END,
//������������
	NCFG_ITEM_ALARM_TRIGER_BASE=0x600,
	NCFG_ITEM_ALARM_OUT_NAME,			//�̵���������ƣ�SHORT_NAME[SENSOR_OUTPUT_NUM]
	NCFG_ITEM_ALARM_OUT_HOLD_TIME,		//�̵����������ʱ�䣬unsigned long [SENSOR_OUTPUT_NUM]
	NCFG_ITEM_ALARM_TRIGER_BASE_END,

	//������
	NCFG_ITEM_ALARM_TRIGER_SENSORBASE=0x620,
	NCFG_ITEM_SENSOR_IN_ENABLE,			//���������أ�unsigned char [SENSOR_INPUT_NUM]
	NCFG_ITEM_SENSOR_IN_TYPE,			//�������豸���ͣ�unsigned char [SENSOR_INPUT_NUM]
	NCFG_ITEM_SENSOR_IN_NAME,			//���������ƣ�SHORT_NAME [SENSOR_INPUT_NUM]

	NCFG_ITEM_SENSOR_IN_TO_ALARM_OUT,	//�������������������unsigned long [SENSOR_INPUT_NUM];
	NCFG_ITEM_SENSOR_IN_TO_EMAIL_ENABLE,//�������ܹ�����EMAIL��unsigned char
	NCFG_ITEM_SENSOR_IN_TO_FTP_ENABLE,	//��������������FTP��unsigned char
	NCFG_ITEM_SENSOR_IN_TO_FTP_ADDR,	//����������FTP��ַ�б���MAIL����ʽһ��unsigned long[n] 
	NCFG_ITEM_SENSOR_IN_TO_PTZ,			//������������̨Ԥ�Ƶ㣬TO_PTZ [VIDEO_INPUT_NUM][SENSOR_INPUT_NUM]
	NCFG_ITEM_SENSOR_IN_TO_SNAP,		//��������������SD��ͼƬ unsigned long[SENSORNUM]
	NCFG_ITEM_SENSOR_TO_FTP_SNAP,		//����������FTPץͼ��unsigned long[SENSORNUM]
	NCFG_ITEM_SENSOR_TO_EMAIL_SNAP,		//����������EMAILץͼ��unsigned long[SENSORNUM]
	NCFG_ITEM_SENSOR_IN_TO_EMAIL_SUBJECT, //�����������ʼ��������ʼ�����char[SMTP_SUBJECT_MAX_LEN]
	NCFG_ITEM_SENSOR_IN_TO_EMAIL_CONTENT, //�����������ʼ��������ʼ�����char[SMTP_CONTENT_MAX_LEN]
	NCFG_ITEM_SENSOR_TO_EMAIL_ADDR,
	NCFG_ITEM_SENSOR_IPHONE_PUSH_SWITCH, //����������IPHONE PUSH ����
	NCFG_ITEM_SENSOR_IPHONE_PUSH_CONTENT,//����
	NCFG_ITEM_SENSOR_HOLD_TIME,			// unsigned long
	NCFG_ITEM_SENSOR_IN_TO_SDRECORD_ENABLE,//����������SD��¼��
	NCFG_ITEM_ALARM_TRIGER_SENSOREND,

	//�ƶ����
	NCFG_ITEM_ALARM_TRIGER_MOTIONBASE=0x640,	
	NCFG_ITEM_MOTION_ENABLE,			//�ƶ���⿪�أ�unsigned char [VIDEO_INPUT_NUM]
	NCFG_ITEM_MOTION_AREA_INFO,         //�������Ϣ��NCFG_INFO_MOTION_INFO[VIDEO_INPUT_NUM]
	NCFG_ITEM_MOTION_HOLD_TIME_ALLCH,	//�ƶ���Ᵽ��ʱ��,unsigned long
	NCFG_ITEM_MOTION_TO_ALARM_OUT,		//�ƶ���ⴥ�����������unsigned long
	NCFG_ITEM_MOTION_TO_RECORD,			//�ƶ���ⴥ��¼��ULONGLONG [VIDEO_INPUT_NUM]
	NCFG_ITEM_MOTION_TO_EMAIL_ENABLE,	//�ƶ�����ܹ�����EMAIL��unsigned char
	NCFG_ITEM_MOTION_TO_PTZ,			//�ƶ���ⴥ����̨��TO_PTZ [VIDEO_INPUT_NUM][VIDEO_INPUT_NUM]
	NCFG_ITEM_MOTION_TO_FTP_ENABLE,		//�ƶ���ⴥ��FTP��unsigned char
	NCFG_ITEM_MOTION_TO_FTP_ADDR,		//�ƶ���ⴥ��FTP��ַ�б���MAIL����ʽһ��unsigned long[n] 
	NCFG_ITEM_MOTION_TO_BUZZ,           //�ƶ���ⴥ��BUZZ unsigned char[VIDEO_INPUT_NUM]
	NCFG_ITEM_MOTION_TO_SNAP,           //�ƶ���ⴥ��ץ�� unsigned long
	NCFG_ITEM_MOTION_TO_FTP_SNAP,		//�ƶ���ⴥ��FTPץͼ��unsigned long
	NCFG_ITEM_MOTION_TO_EMAIL_SNAP,		//�ƶ���ⴥ��EMAILץͼ��unsigned long
	NCFG_ITEM_MOTION_TO_MAIL_SUBJECT,	//�ƶ���ⴥ���ʼ����������� char[SMTP_SUBJECT_MAX_LEN]
	NCFG_ITEM_MOTION_TO_MAIL_CONTENT,	//�ƶ���ⴥ���ʼ����������� char[SMTP_CONTENT_MAX_LEN]
	NCFG_ITEM_MOTION_TO_EMAIL_ADDR,
	NCFG_ITEM_MOTION_IPHONE_PUSH_SWITCH, //�ƶ���ⴥ��IPHONE PUSH
	NCFG_ITEM_MOTION_IPHONE_PUSH_CONTENT, //����
	NCFG_ITEM_MOTION_HOLD_TIME,				// unsigned long
	NCFG_ITEM_MOTION_TO_SDRECORD_ENABLE,	//�ƶ���ⴥ��SD��¼��
	NCFG_ITEM_ALARM_TRIGER_MOTIONEND,
	
	NCFG_ITEM_ALARM_TRIGER_SHELTERBASE=0x670,
	NCFG_ITEM_SHELTER_TO_ALARM_OUT,		//�ڵ��������������ULONGLONG [VIDEO_INPUT_NUM]
	NCFG_ITEM_SHELTER_TO_RECORD,		//�ڵ�����¼��ULONGLONG [VIDEO_INPUT_NUM]
	NCFG_ITEM_SHELTER_TO_EMAIL_ENABLE,	//�ڵ��ܹ�����EMAIL��unsigned char
	NCFG_ITEM_SHELTER_TO_EMAIL_ADDR,	//�ڵ�����MAIL��ַ�б�unsigned long[n]
	NCFG_ITEM_SHELTER_TO_FTP_ENABLE,	//�ڵ�����FTP��unsigned char
	NCFG_ITEM_SHELTER_TO_FTP_ADDR,		//�ڵ�����FTP��ַ�б���MAIL����ʽһ��unsigned long[n] 
	NCFG_ITEM_SHELTER_TO_BUZZ,          //�ڵ�����BUZZ unsigned char[VIDEO_INPUT_NUM]
	NCFG_ITEM_SHELTER_TO_PTZ,			//�ڵ�������̨��TO_PTZ [VIDEO_INPUT_NUM][VIDEO_INPUT_NUM]
	NCFG_ITEM_ALARM_TRIGER_SHELTEREND,

	NCFG_ITEM_ALARM_TRIGER_END,


//����������ڲ�֧��NCFG_ITEM_ENCODE_ENCODE_TYPEʱĬ����֧��VBRģʽ
	NCFG_ITEM_ENCODE_BASE=0x700,		//ĿǰҪ�ϳ��ķ�ʽ�ŵ��˶�ID�����
	
	NCFG_ITEM_ENCODE_RESOLUTION_ALLCH,//����ͨ��һ���ֱ�ʱʹ�ô�ֵ
	NCFG_ITEM_ENCODE_RESOLUTION,		//���������ֱ��ʣ�unsigned char [VIDEO_INPUT_NUM]��ÿ��ͨ����ͬ
	NCFG_ITEM_ENCODE_VIDEO_QUALITY,		//������������, unsigned long [VIDEO_INPUT_NUM],VBRָ�ȼ�(����Խ����Խ��)CBRָ����
	NCFG_ITEM_ENCODE_FRAME_RATE,		//��������֡�ʣ�unsigned char [VIDEO_INPUT_NUM]
	NCFG_ITEM_ENCODE_ENCODE_TYPE,		//����������������unsigned char[VIDEO_INPUT_NUM]
	NCFG_ITEM_ENCODE_I_FRAME_INTERVAL,	// unsigned long
	NCFG_ITEM_ENCODE_ENCODE_LEVEL,		// unsigned long   NCFG_ENCODE_LEVEL
    // ����5����NVRʹ�õġ�
    NCFG_ITEM_ENCODE_RESOLUTION_V2,         // chn(0,1,2,3;4 byte) + �ֱ���(4 byte)
    NCFG_ITEM_ENCODE_VIDEO_QUALITY_V2,     // chn(0,1,2,3;4 byte) + ����(4 byte)
    NCFG_ITEM_ENCODE_FRAME_RATE_V2,        // chn(0,1,2,3;4 byte) + ֡��(4 byte)
    NCFG_ITEM_ENCODE_ENCODE_TYPE_V2,       // chn(0,1,2,3;4 byte) + ��������(4 byte)
    NCFG_ITEM_ENCODE_I_FRAME_INTERVAL_V2,// chn(0,1,2,3;4 byte) + I֡���(4 byte)
    
    // for api
    NCFG_ITEM_ENCODE_LEVEL_EX,   // �������� unsigned long [VIDEO_INPUT_NUM]
    NCFG_ITEM_ENCODE_SLICE_EX,   // �Ƿ��Ƭ unsigned long
	NCFG_ITEM_ENCODE_END,		
	
//��̨�������ã�Ѳ���ߣ�Ԥ�õ�ȣ�ͨ������ӵ���ID����˿ں�Э��ŵ���NCFG_ITEM_PTZ_CONFIG����
	NCFG_ITEM_PTZ_INFO_BASE=0x800,
	NCFG_ITEM_SPEEDDOME_PTZ, //����������ͨptz�͸�����ptz������
	NCFG_ITEM_PTZ_INFO_PRESET_INFO_CHBASE=0x810,//Ԥ�õ�����ͨ������������64��ͨ��ID��λ�ã�PTZ_PRESET_INFO
	NCFG_ITEM_PTZ_INFO_PRESET_INFO_CHEND =NCFG_ITEM_PTZ_INFO_PRESET_INFO_CHBASE+64,

	NCFG_ITEM_PTZ_INFO_CRUISE_INFO_CHBASE=NCFG_ITEM_PTZ_INFO_PRESET_INFO_CHEND,//Ѳ��������ͨ������������64��ͨ��ID��λ��
	NCFG_ITEM_PTZ_INFO_CRUISE_INFO_CHEND =NCFG_ITEM_PTZ_INFO_CRUISE_INFO_CHBASE+64,//PTZ_CRUISE_INFO_HEAD
	NCFG_ITEM_PTZ_INFO_END=0x8FF,
	
//�����������
	NCFG_ITEM_SPVMN_BASE=0x900,	
	NCFG_ITEM_SPVMN_ENABLE,				//���꿪�أ�unsigned char
	NCFG_ITEM_SPVMN_SERVERIP,			//sip������ip��ַ��unsigned long
	NCFG_ITEM_SPVMN_SERVERSIPPORT,		//sip�������Ự�˿� unsigned short
	NCFG_ITEM_SPVMN_IPCSIPPORT,			//IPC�Ự�˿�	unsigned short	
	NCFG_ITEM_SPVMN_REGPASSWD,			//ע������		char[36]
	NCFG_ITEM_SPVMN_SERVERDOMAIN,		//sip����������	char[36]
	NCFG_ITEM_SPVMN_SERVERID,			//sip������ID	char[36]
	NCFG_ITEM_SPVMN_IPCID,				//����ID		char[36]
	NCFG_ITEM_SPVMN_OWNER,				//�豸����		char[36]								
	NCFG_ITEM_SPVMN_CIVILCODE,			//��������		char[36]
	NCFG_ITEM_SPVMN_ADDRESS,			//��װ��ַ		char[36]
	NCFG_ITEM_SPVMN_END,

//����������
	NCFG_ITEM_DECODE_BASE=0xA00,
	NCFG_ITEM_OSD_LOCALIP,				//�����Ƿ���ʾ����IP, unsigned char
	NCFG_ITEM_CUR_QUADMODE,				//Ŀǰ�ָ�ģʽ,NCFG_INFO_QUADMODE
	NCFG_ITEM_MAP_REMOTE_CHNN,			//ӳ��Զ���豸ͨ��
	NCFG_ITEM_DECODE_END,
//IPCamera�豸������
	NCFG_ITEM_IPCAMERA_BASE=0xB00,
	NCFG_ITEM_NIGHT_VISION,				//ҹ�ӹ��ܿ���	unsigned char,NCFG_ENUM_NIGHT_VISION 
	NCFG_ITEM_FREQUENCY_HZ,				//��ͷ��ԴƵ��50/60����/����ģʽ	,unsinged char NCFG_ENUM_CAMERA_FREQ		
	NCFG_ITEM_IPCAMERA_END,

//SD��¼��������
	NCFG_ITEM_SDRECORD_BASE=0xC00,
	NCFG_ITEM_SDRECORD_STREAMINDEX,
	NCFG_ITEM_SDRECORD_NEEDAUDIO,
	NCFG_ITEM_SDRECORD_END,
	
	NCFG_ITEM_ECMS_INFO_BASE=0xD00,
	NCFG_ITEM_ECMS_INFO_SWITCH,			//�Ƿ�ҪIPCamera��������ECMS��unsigned char
	NCFG_ITEM_ECMS_INFO_ADDR,			// char[ECMS_ADDR_MAX_LEN],����������
	NCFG_ITEM_ECMS_INFO_PORT,			// unsigned short
	NCFG_ITEM_ECMS_INFO_ID,				//ID unsigned long
	NCFG_ITEM_ECMS_INFO_END,
	
///ֻ����Ϣ������������󳤶����ƣ�֧��ѡ��ķ�Χ,�汾��Ϣ��
	NCFG_ITEM_ONLYREAD_BASE=0xE00,
	NCFG_ITEM_DEVICE_NAME_MAX_LEN,      //�豸���Ƴ����޶�, unsigned char,[ֻ��]
	NCFG_ITEM_TIME_SERVER_MAX_LEN,      //����ʱ��ͬ����������ַ��󳤶�, unsigned char,[ֻ��]
	NCFG_ITEM_UPNP_FRIENDLY_NAME_MAX_LEN,//UPNP�豸��ʾ��������󳤶�, unsigned char,[ֻ��]
	NCFG_ITEM_CAMERA_NAME_MAX_LEN,      //ͨ��������󳤶�, unsigned char,[ֻ��]
	NCFG_ITEM_SENSOR_IN_NAME_MAX_LEN,   //������������󳤶�,unsigned char,[ֻ��]
	NCFG_ITEM_ALARM_OUT_NAME_MAX_LEN,   //�̵������������󳤶�, unsigned char,[ֻ��]
	NCFG_ITEM_PPPOE_USER_NAME_MAX_LEN,  //PPPoE�û�����󳤶ȣ�unsigned char,[ֻ��]
	NCFG_ITEM_PPPOE_PASSWORD_MAX_LEN,   //PPPoE������󳤶�, unsigned char,[ֻ��]
	NCFG_ITEM_DDNS_USER_NAME_MAX_LEN,   //DDNS�û�����󳤶�, unsigned char,[ֻ��]
	NCFG_ITEM_DDNS_PASSWORD_MAX_LEN,    //DDNS������󳤶�, unsigned char,[ֻ��]
	NCFG_ITEM_DDNS_DOMAIN_MAX_LEN,		//DDNS����������󳤶�, unsigned char,[ֻ��]
	NCFG_ITEM_WIRELESS_SSID_MAX_LEN,    //����SSID��󳤶�, unsigned char,[ֻ��]
	NCFG_ITEM_WIRELESS_KEY_MAX_LEN,     //��Կ��󳤶�, unsigned char,[ֻ��]
	NCFG_ITEM_SOFTWARE_VER,				//����汾��unsigned long,[ֻ��]
	NCFG_ITEM_SOFTWARE_BUILD_DATE,		//���Build���ڣ�unsigned long,[ֻ��]
	NCFG_ITEM_KERNEL_VER,				//�ں˰汾��unsigned long,[ֻ��]
	NCFG_ITEM_HARD_VER,					//Ӳ���汾��unsigned long,[ֻ��]
	NCFG_ITEM_DISK_INFO,				//���̻�SD����Ϣ��NCFG_INFO_DISK_INFO[n],[ֻ��]
	NCFG_ITEM_TOTAL_DISK_INFO,			//DVR����������Ϣ������ʹ�ðٷֱȣ�BYTE,[ֻ��]
	NCFG_ITEM_MAC_ADDRESS ,				//�豸MAC��ַ��ֻ������,[ֻ��]
	NCFG_ITEM_MAX_CLIENT_COUNT,         //���ӵ����ͻ�������  unsigned char,[ֻ��]
	NCFG_ITEM_PRESET_MAXNUM,            //֧�ֵ�Ԥ�õ��������� unsigned char,[ֻ��]
	NCFG_ITEM_CRUISE_MAXNUM,            //֧�ֵ�Ѳ���ߵ�������� unsigned char,[ֻ��]
	NCFG_ITEM_SUPPORT_PTZRETURN,		//֧�ֱ�����������Ԥ֪�㹦��,unsigned char,[ֻ��]
	NCFG_ITEM_TIME_STAMP_SUPPORT_POS,	//ʱ���֧�ֵ�λ�÷�ʽ��unsigned long ����ʱ�����ʹ�ô�ID(0x01 << pos��MASK),[ֻ��]
	NCFG_ITEM_SUPPORT_VIDEOFORMAT,		//֧�ֵ���Ƶ��ʽ,unsigned char,[ֻ��]
	NCFG_ITEM_SUPPORT_QUADMODE,			//֧�ֵķָ�ģʽ[ֻ��],�ڼ�λΪ1�ʹ���֧����һ���ָ�ģʽ��unsigned long,[ֻ��]
	NCFG_ITEM_SUPPORT_BITRATE_RANGE,	//CBRģʽ��֧�ֵ�������Χ��NCFG_INFO_BITRATE_RANG,[ֻ��]
	NCFG_ITEM_SUPPORT_VBR_BITRATE,		//VBRģʽ��ÿ�ֵ��εĻ���Ŀ��������NCFG_INFO_CBRBITRATE[n],[ֻ��]
	NCFG_ITEM_SUPPORT_VBR_LEVELNUM,		//VBR֧�ּ������εĻ��ʣ��ͻ���֧��5�ֵ��Σ�����������1-n������ȡ������,unsigned char,[ֻ��]
	NCFG_ITEM_SUPPORT_ENCODEMODE,		//֧�ֵı���ģʽ���Ǹ�VIDEO_ENCODE_MODE��MASK��unsigned char,[ֻ��]
	NCFG_ITEM_USER_MAXNUM,				//֧���û����������unsigned char[ֻ��]
	NCFG_ITEM_AUDIO_INPUT_NUM,			//��������ͨ��������unsigned char[ֻ��]
	NCFG_ITEM_HOLIDAY_MAX_NUM,			//����֧�ֵļ����ų����ڵ�����������ر��Ƿ�ѭ����ʽʱ���������ӣ�unsigned char
	NCFG_ITEM_DEVUSER_NAME_MAX_LEN,		//�豸�û�����󳤶ȣ�unsigned char[ֻ��]
	NCFG_ITEM_DEVUSER_PWD_MAX_LEN,		//�豸�û�������󳤶ȣ�unsigned char[ֻ��]
	NCFG_ITEM_DEVTYPE_INFO,				//�豸���ͣ�DEVICE_TYPE_DEF::NETDEVICE_TYPE_INFO��[ֻ��]
	NCFG_ITEM_PLAYBACK_MAX_NUM,			//ͬʱ�طŵ�ͨ����Ŀ��unsigned char[ֻ��]
	NCFG_ITEM_DEFAULT_COLOR_BRIGHTNESS, //ȱʡ������ֵ,unsigned char[ֻ��]
	NCFG_ITEM_DEFAULT_COLOR_CONTRAST,   //ȱʡ�ĶԱȶ�ֵ,unsigned char[ֻ��]
	NCFG_ITEM_DEFAULT_COLOR_SATURATION, //ȱʡ�ı��Ͷ�ֵ,unsigned char[ֻ��]
	NCFG_ITEM_DEFAULT_COLOR_HUE,		//ȱʡ�ĻҶ�ֵ,unsigned char[ֻ��]
	NCFG_ITEM_VGA_SUPPORT_RESOLUTION,	//VGAоƬ֧�ֵķֱ���MASK��unsigned long���ο�dvrdvsdef.h�е�VGA_RESOLUTIONö��[ֻ��]
	NCFG_ITEM_VGA_SUPPORT_REFRESH,		//VGA֧�ֵ���Ļˢ���ʣ�ÿ�ֱַ����¶�������ͬ[ֻ��]
	NCFG_ITEM_FILTER_IP_MAXNUM,			//֧��IP���˵����IP����unsigned char[ֻ��]
	NCFG_ITEM_FILTER_MAC_MAXNUM,		//֧��MAC���˵����MAC����unsigned char[ֻ��]
	NCFG_ITEM_DDNS_SERVER_INFO,			//DDNS�����б�֧�ֵ�DDNS����[ֻ��],DDNS_SERVER_INFO	
	NCFG_ITEM_LIVE_TIME_SUPPORT_POS,	//�ֳ�ʱ��֧�ֵ�λ�÷�ʽunsigned char[ֻ��]
	NCFG_ITEM_CRUISE_PRESET_MAXNUM,		//ÿ��Ѳ���ߵ����Ԥ�õ�����[ֻ��]
	NCFG_ITEM_VIDEO_INPUT_NUM,			//��Ƶ����ͨ������unsigned char[ֻ��]
	NCFG_ITEM_SENSOR_IN_NUM,			//������������������unsigned char[ֻ��]
	NCFG_ITEM_ALARM_OUT_NUM,			//�������������unsigned char[ֻ��]
	NCFG_ITEM_NOTIFY_FTP_NUM,			//FTP֪ͨ���������������unsigned char[ֻ��]
	NCFG_EMAIL_SUBJECT_MAX_LEN,			//EMAIL������󳤶�unsigned long[ֻ��]
	NCFG_EMAIL_CONTENT_MAX_LEN,			//EMAIL������󳤶�unsigned long[ֻ��]
	NCFG_FTP_SNAP_PIC_RAMES_MAX,		//FTP�ϴ�ͼƬ֧�ֵ�ÿ�����֡ȡֵö��unsigned long[ֻ��]����λ����
	NCFG_RTSP_STREAM_NAME_MAX_LEN,		//RTSP������ַ��󳤶�
	NCFG_PTZ_PRESET_NAME_MAX_LEN,		//PTZԤ�õ�������󳤶�
	NCFG_PTZ_CRUISE_NAME_MAX_LEN,		//PTZѲ����������󳤶�
	NCFG_PTZ_PROTOCOL_INFO,				//��̨֧�ֵ�Э��PTZ_PROTOCOL_INFO
	NCFG_PTZ_SUPPORT_BAUDRATE,			//��̨֧�ֵĲ�����PTZ_BAUDRATE_INFO
	NCFG_ITEM_ENCODE_SUPPORT_PROPERTY,//��ʾ֧�ֵ���������Щ	
	NCFG_ITEM_DEFAULT_IMAGE_MIRROR,			//ȱʡ�ľ���ͼ��,unsigned char[ֻ��]
	NCFG_ITEM_DEFAULT_IMAGE_FLIP,			//ȱʡ�ķ�תͼ��,unsigned char[ֻ��]
	NCFG_ITEM_LIGHT_HZ_SUPPORT,			//֧�ֵĵƹ�ģʽ
	NCFG_ITEM_IPCAMERA_NO,              //��ȡIPC���к� panjy	
	NCFG_ITEM_MAX_BRIGHT_VALUE,			//�������õ��������ֵ
	NCFG_ITEM_MAX_CONTRACT_VALUE,		//�������õĶԱȶ����ֵ
	NCFG_ITEM_MAX_SATURATION_VALUE,		//�������õı��Ͷ����ֵ
	NCFG_ITEM_MAX_HUE_VALUE,			//�������õ�ɫ�����ֵ

	NCFG_ITEM_MAX_WHITE_BALANCE_VALUE,	//�������õİ�ƽ������ֵ NCFG_MAX_WHITE_BALANCE
	NCFG_ITEM_MAX_DYNAMIC_RANGE,		//�������õĿ�̬��ֵ unsigned char

//	NCFG_ITEM_MAX_WHITE_BALANCE_VALUE,	//�������õİ�ƽ�����ֵ
	NCFG_ITEM_DEFAULT_WHITE_BALANCE, //ȱʡ�İ�ƽ��ֵ,NCFG_WHITE_BALANCE[ֻ��]
	NCFG_ITEM_DEFAULT_DYNAMIC_RANGE, //ȱʡ�Ŀ�̬,NCFG_DYNAMIC_RANGE[ֻ��]

	NCFG_ITEM_TRAVER_ONLINE,		//��͸�Ƿ�����
	NCFG_ITEM_DEFAULT_DENOISE,  //����Ĭ��ֵ
	NCFG_ITEM_MAX_DENOISE,      //�������ֵ
	NCFG_ITEM_DEFAULT_IRISTYPE,//�Զ���ȦĬ��ֵ
	NCFG_ITEM_MAX_IRISTYPE,     //�Զ���Ȧ���ֵ
	NCFG_ITEM_DEFAULT_SHARPEN,  //��Ĭ��ֵ
	NCFG_ITEM_MAX_SHARPEN,      //�����ֵ

	NCFG_ITEM_FTP_USR_MAX_LEN,
	NCFG_ITEM_FTP_PWD_MAX_LEN,

	NCFG_ITEM_CFG_IMAGE_STREAM_INDEX, //����ͼ��ʱ��������һ·���� unsigned char

	NCFG_ITEM_DEFAULT_VIDEO_OUTPUT_FORMAT,  //��Ƶ���Ĭ��ֵ
	NCFG_ITEM_MAX_VIDEO_OUTPUT_FORMAT,      //��Ƶ������ֵ

	NCFG_ITEM_IPHONE_PUSH_CONTENT_MAX,

    NCFG_ITEM_MAX_ENCODE_I_FRAME_INTERVAL,//I֡������ֵ
    NCFG_ITEM_MIN_ENCODE_I_FRAME_INTERVAL,//I֡�����Сֵ
    NCFG_ITEM_DEFAULT_ENCODE_I_FRAME_INTERVAL,//I ֡���Ĭ��ֵ

    NCFG_ITEM_DAY_NIGHT_MODE_DEFAULT,//IR-CUTĬ��ֵ

	NCFG_ITEM_PTZ_EXIST_DEFAULT,	//��̨�����Ƿ����
	NCFG_ITEM_ONVIF_CLIENT_INFO,
    // expose
    NCFG_ITEM_EXPOSE_TYPE_DEFAULT,
    NCFG_ITEM_EXPOSE_VALUE_MAX,
    NCFG_ITEM_EXPOSE_VALUE_DAFAULT,
    // gain
    //NCFG_ITEM_GAIN_VALUE_MAX,
    //NCFG_ITEM_GAIN_VALUE_DEFAULE,
    NCFG_ITEM_SUPPORT_VIDEO_ENCODE_TYPE, // unsigned long [VIDEO_INPUT_NUM] NCFG_ENCODE_H264 NCFG_ENCODE_MJPEG
    NCFG_ITEM_SUPPORT_ENCODE_LEVEL,      // unsigned long [VIDEO_INPUT_NUM] NCFG_ENCODE_LEVEL_BASELINE, NCFG_ENCODE_LEVEL_MAINPROFILE
	NCFG_ITEM_DEFAULT_FOG_REDUCTION,	// FOG_REDUCTION_CONFIG
	NCFG_ITEM_DEFAULT_LENS_DISTORTION,	// LENS_DISTORTION_CONFIG
	NCFG_ITEM_DEFAULT_BLC,	                // unsigned char
	NCFG_ITEM_DEFAULT_ANTI_OVEREXPOSURE,    // unsigned char
	NCFG_ITEM_DEFAULT_DAY_NIGHT_SENSITIVITY,// unsigned char
	NCFG_ITEM_DEFAULT_NIGHT_TO_DAY,         // unsigned long
	NCFG_ITEM_DEFAULT_DAY_TO_NIGHT,         // unsigned long
	NCFG_ITEM_DEFAULT_VIDEO_GAIN,           // ���� VIDEO_GAIN
	NCFG_ITEM_DEFAULT_DIGITA_DENOISE,       // ���ֽ��� DIGITA_DENOISE
	NCFG_ITEM_DEFAULT_IMAGE_ROTATO,         // ͼ����ת(����ģʽ) IMAGE_ROTATO
	NCFG_ITEM_DEFAULT_DAY_TO_NIGHT_SENSITIVITY,// unsigned char
	NCFG_ITEM_DEFAULT_NIGHT_TO_DAY_SENSITIVITY,// unsigned char
	NCFG_ITEM_ONLYREAD_END,	

	NCFG_ITEM_ONVIF_BASE=0xF00,
	NCFG_ONVIF_CLIENT_TYPE,
	NCFG_ONVIF_VERSION,	// unsigned long
	NCFG_ITEM_ONVIF_END,

///����ID���Ȳ��ǿ���������ȡ�ͱ����ID��Ҳ����ֻ��ID
	NCFG_ITEM_OTHER_BASE=0xFF0,
	NCFG_ITEM_MANUAL_SET_TIME_VALUE,	//����ʱ��
	NCFG_ITEM_BONJOUR_FRIENDLY_NAME_MAX_LEN,//BONJOUR�豸��ʾ��������󳤶�, unsigned char,[ֻ��]
	NCFG_ITEM_OTHER_END,
}NCFG_ITEM_ID;


enum NCFG_ENCODE_LEVEL
{
	NCFG_ENCODE_LEVEL_BASELINE,
	NCFG_ENCODE_LEVEL_MAINPROFILE,
	NCFG_ENCODE_LEVEL_HIGHPROFILE
};

//VGAÿ�ֱַ�����֧�ֵ���Ļˢ����
typedef struct _ncfg_info_vga_support_refreshhz
{
	unsigned long resolution;//�ֱ���
	unsigned char refreshhz[8];//���ֱַ�����֧�ֵ���Ļˢ����(��λhz)��8�֣�0��Ч
}
NCFG_INFO_VGA_SUPPORT_REFRSHHZ;

typedef struct _ncfg_whiteBalance
{
	unsigned char ucMode;// 0 �Զ� 1 �ֶ� 
	unsigned int red;	//��ɫ����
	unsigned int blue;	//��ɫ����
	//unsigned char unused;
}NCFG_WHITE_BALANCE;

typedef struct _ncfg_ptzSwitch
{
	unsigned long dwPtzSwitch;
}NCFG_PTZSWITCH;

typedef struct _ncfg_max_whiteBalance
{
	unsigned int redMax;	//��ɫ����
	unsigned int blueMax;	//��ɫ����
	//unsigned char unused[2];
}NCFG_MAX_WHITE_BALANCE;

typedef struct _ncfg_dynamic_range
{
	unsigned char ucMode;// 0 �ر� 1 ����
	unsigned char value;
	unsigned char unused[2];
}NCFG_DYNAMIC_RANGE;

typedef struct _ncfg_denoise
{
	unsigned char ucMode;// 0 �ر� 1 ����
	unsigned char value;
	unsigned char unused[2];
}NCFG_DENOISE;

typedef struct _ncfg_iristype
{
	unsigned char ucMode;// 0 �ر� 1 ����
	unsigned char unused[3];
}NCFG_IRISTYPE;

typedef struct _ncfg_sharpen
{
	unsigned char ucMode;// 0 �ر� 1 ����
	unsigned char value;
	unsigned char unused[2];
}NCFG_SHARPEN;

typedef struct _ncfg_day_night //panjy
{
	unsigned char ucMode; //color b/w auto
	unsigned char ucDelay; //autoģʽ�µ��ӳ�ʱ�� 0-255, default 4
	unsigned char ucDayTh; //����ֵ 0-255 default 5
	unsigned char ucNightTh; //0 - 255 default 3
}NCFG_DAY_NIGHT;

typedef enum _day_night_mode
{
	DN_AUTO  = 0x0,
	DN_COLOR = 0x1,
	DN_BW    = 0x2
}DAY_NIGHT_MODE;

typedef enum _white_balance_9314
{
	WHITE_BLANCE_9314_ATW  = 0x0,
	WHITE_BLANCE_9314_PUSH = 0x1,
	WHITE_BLANCE_9314_CRS  = 0x2
}WHITE_BALANCE_9314;

//������Ϣ���õ���Щ�Ͷ�����Щ���˴���������ʹ��
typedef struct _ncfg_info_ptz_config
{
	unsigned char	ucBaudrate;		//������0-13�ֱ��ʾ110��300����..921600�Ĳ�����
	unsigned char	ucProtocolid;	//Э��ID��0Ϊû��������̨
	unsigned char	ucDevaddr;		//��̨�豸ID
	unsigned char	ucNoUsed;		//û���ã�ֻ��Ϊ�˶���
}NCFG_INFO_PTZ_CONFIG;

//CBRģʽ�£�ÿ�ֱַ��ʵ�������Χ
typedef struct _ncfg_info_bitraterang
{
	unsigned long low;//�������
	unsigned long high;//�������
}
NCFG_INFO_BITRATE_RANG;

//VBRģʽ��ÿ�����εĻ��ʵ�Ŀ������
typedef struct _ncfg_info_vbrbitrate
{
	unsigned char resolution;//��Ƶ�ֱ���
	unsigned long targetbitrate[10];//���֧��10�����Σ����ܵĽ�������7�����ʻ����кܴ����
}
NCFG_INFO_VBRBITRATE;

//���������ã�ʵ��Ӧ�û�������,�����԰�����
const unsigned long  	N_USER_NAME_LEN				= 32;
const unsigned long  	N_PASSWORD_LEN				= 16;
typedef struct _ncfg_info_map_remote_chnn 
{
	char userName[N_USER_NAME_LEN];
	char password[N_PASSWORD_LEN];
	unsigned long    serverIP;
	unsigned short   serverPort;

	unsigned char localChnn;//����ͨ��
	unsigned char remoteChnn;//��Ӧ��Զ���豸ͨ��

	unsigned char osdchname;//ͨ��������ʾ����
	unsigned char osddevip;//�豸IP
	unsigned char osddevname;//�豸����	
    unsigned char resv[1];
}NCFG_INFO_MAP_REMOTE_CHNN;

//�û��ģ�Ŀǰ֧�ֳ������ӾͶ���������ӣ�Ȩ�޺�ͨ��ȥ����,����ı�ͻ�ID������ID��ʵ��
typedef struct _ncfg_info_user
{
	unsigned long   group;
	char            name[36];
	char            password[36];
	unsigned char   MAC[6];
	unsigned char   reserved;
	unsigned char   ucBindMac;	//0 ���� 1��
}NCFG_INFO_USER;

typedef struct _ncfg_info_short_name
{
	char	name [36];
}NCFG_INFO_SHORT_NAME;

typedef struct _ncfg_info_long_name
{
	char	name [132];
}NCFG_INFO_LONG_NAME;

typedef struct _ncfg_info_superlong_name
{
	char	name [1028];
}NCFG_INFO_SUPERLONG_NAME;

typedef struct _ncfg_info_dev_name_api_
{
    unsigned int		bSwitch;
	unsigned int		x;
	unsigned int		y;	
	char		szDevName[64];		//	�豸��
}NCFG_INFO_DEV_NAME_EX;

//����ʱ��������ͷ�ƶ�
typedef struct _ncfg_info_toptz
{
	unsigned char	ucToPreset;				//����ʱ���ƶ�����һ��Ԥ�Ƶ�0 -- 127	 0xff ��ʾ����Ҫ�ƶ�
	unsigned char	ucBackPreset;			//�������󷵻��ĸ�Ԥ�Ƶ�0 -- 127		 0xff ��ʾ������
	char            cIsEnableTo;            //���õ���ĳ��Ԥ�õ�
	char            cIsEnableBack;          //���ý���ʱ����ĳ����	
	unsigned char	ucToCruise;				//����ʱ,��������Ѳ����0 -- 127		 0xff ��ʾ������
	char            cIsEnableCruise;        //����Ѳ���߹���
	char			cIsEnableLocus;			//�����켣����
	unsigned char	ucNoUsed[1];
}NCFG_INFO_TOPTZ;


typedef struct _ncfg_info_motion_info_ex
{
	unsigned long motiontype;	//�ƶ��������ͣ�Ŀǰ�̶�Ϊ1,��������������Ҫ���⴦��ʱ+1
	unsigned long gridWidth;	//ÿ���ж��ٸ�ջ��
	unsigned long gridHeight;	//���ж�����ջ��
	unsigned long xPixel;		//ջ��ʼ����xλ��
	unsigned long yPixel;		//ջ��ʼ����yλ��
	unsigned long gridData[128];//��������
	unsigned long Sensitivity;  //�ƶ����������
}NCFG_INFO_MOTION_INFO;

typedef struct _ddns_server_info 
{
	unsigned char  ucDDNSID; /////��Ӧ�ķ�����ID   ID����0��Ӧ�ķ��������Ʋ���Ч
	unsigned char  supportproperty;//֧�ֵĳ��û�����������������Ե�MASK,NCFG_ENUM_DDNS_SUPPORT_PROPERTY
	unsigned char  noused[2];
	char           szDDNSServerName[64];  ////DDNS�������ĵ�ַ
}DDNS_SERVER_INFO, *LPDDNS_SERVER_INFO;


enum NCFG_ENUM_WIRELESS_AUTH_MODE
{
	NCFG_ENUM_WIRELESS_AUTH_MODE_OPEN,
	NCFG_ENUM_WIRELESS_AUTH_MODE_SHARED,
	NCFG_ENUM_WIRELESS_AUTH_MODE_WPA_PSK,
	NCFG_ENUM_WIRELESS_AUTH_MODE_WPA2_PSK,
};

enum NCFG_ENUM_WIRELESS_ENCRYPT_TYPE
{
	NCFG_ENUM_WIRELESS_ENCRYPT_TYPE_NONE,	//	CFG_WIRELESS_AUTH_MODE_OPEN, CFG_WIRELESS_AUTH_MODE_SHAREDʱ��
	NCFG_ENUM_WIRELESS_ENCRYPT_TYPE_WEP,	//	CFG_WIRELESS_AUTH_MODE_OPEN, CFG_WIRELESS_AUTH_MODE_SHAREDʱ��
	NCFG_ENUM_WIRELESS_ENCRYPT_TYPE_TKIP,	//	CFG_WIRELESS_AUTH_MODE_WPA_PSK, CFG_WIRELESS_AUTH_MODE_WPA2_PSK
	NCFG_ENUM_WIRELESS_ENCRYPT_TYPE_AES,	//	CFG_WIRELESS_AUTH_MODE_WPA_PSK, CFG_WIRELESS_AUTH_MODE_WPA2_PSK
};

enum NCFG_ENUM_DISK_TYPE
{
	NCFG_ENUM_DISK_TYPE_FIXEDDISK = 0,
	NCFG_ENUM_DISK_TYPE_REMOVABLE = 1,
	NCFG_ENUM_DISK_TYPE_CD        = 2,
	NCFG_ENUM_DISK_TYPE_ESATA     = 3,
	NCFG_ENUM_DISK_TYPE_SD		  = 4
};

//����MASK��ʽ,״̬��ʾ���ȷ�ʽ��������Ч-->�������߻�SD��������-->����δ��ʽ��-->�����쳣
enum NCFG_ENUM_DISK_STATUS
{
	NCFG_ENUM_DISK_STATUS_VALID			=0x01,		//���õ�,������Ч����ʹ��
	NCFG_ENUM_DISK_STATUS_UNFORMAT		=0x02,		//û�и�ʽ���Ĵ��̻�SD���ļ���ʽ�����ϱ�׼
	NCFG_ENUM_DISK_STATUS_INSERT_POPED	=0x04,		//���˿������Ǳ�������
	NCFG_ENUM_DISK_STATUS_SLEEPED		=0x08,		//����������
	NCFG_ENUM_DISK_STATUS_EXCEPTION		=0x10		//�쳣�����𻵴���
};

//�������̻�SD����Ϣ
typedef struct _ncfg_info_disk_info
{
	unsigned long   dwType ; /////DISK_TYPE
	unsigned long   dwIndex ;   ////������������ʽ�����̵�ʱ��Ҫ�ô˱�����Ϊ�������и�ʽ��, -1Ϊ��Ч
	unsigned long	dwDiskStatus;	//CFG_SD_STATUS
	unsigned long	capability; //���̻�SD��������������MByteΪ��λ
	unsigned long	Spaceused;	//ʹ���˵Ŀռ䣬��MByteΪ��λ
	unsigned long	SpaceSpare;	//ʣ��Ŀռ䣬��MByteΪ��λ
}NCFG_INFO_DISK_INFO, *LNCFG_INFO_PDISK_INFO;

typedef struct _ncfg_info_support_frame_rateEx
{
	unsigned long	dwStreamIndex;	//�ڼ�·����
	unsigned long	dwResolution;	//ʲô�ֱ���
	unsigned long	dwRate;			//֧����Щ֡��
	char			strResolutionName[16];//�ֱ�������
	unsigned short	supportDataRate[20];//֧�ֵ�����
}NCFG_INFO_SUPPORT_FRAME_RATE;

typedef struct _ncfg_info_quadmode
{
	unsigned long quadmode;			//�ָ�ģʽ��1��4��9��16����������
	unsigned long quadchannel;		//��ǰ�ָ�ģʽ��ͨ��
}NCFG_INFO_QUADMODE;

typedef struct _ncfg_info_time_pos
{
	unsigned long postype;
	unsigned long customdefine_X;
	unsigned long customdefine_Y;
}
NCFG_INFO_TIME_POS;

/// for api
typedef struct _ncfg_time_stamp_config
{
	unsigned int	bSwitch;			// 0 ��Ҫʱ���  1��Ҫʱ���
	unsigned int		x;					// ʱ���λ�� ������VGAΪ�ο�
	unsigned int		y;						
	unsigned int	format;				// ʱ���ʽ	DATE_MODE 0x01--������ģʽ  0x02--������ģʽ 0x03--������ģʽ
}NCFG_TIME_STAMP_CONFIG;

typedef enum _ncfg_enum_support_disp_pos
{
	NCFG_ENUM_DISP_SUPPORT_TOP_RIGHT		= 0x01,//����
	NCFG_ENUM_DISP_SUPPORT_TOP_LEFT			= 0x02,//����
	NCFG_ENUM_DISP_SUPPORT_BOTTOM_RIGHT		= 0x04,//����
	NCFG_ENUM_DISP_SUPPORT_BOTTOM_LEFT		= 0x08,//����
	NCFG_ENUM_DISP_SUPPORT_TOP_MIDDLE		= 0x10,//����
	NCFG_ENUM_DISP_SUPPORT_BOTTOM_MIDDLE	= 0x20,//����
	NCFG_ENUM_DISP_SUPPORT_CENTER_LEFT		= 0x40,//����
	NCFG_ENUM_DISP_SUPPORT_CENTER_MIDDLE	= 0x80,//����
	NCFG_ENUM_DISP_SUPPORT_CENTER_RIGHT		= 0x100,//����
	NCFG_ENUM_DISP_SUPPORT_CUSTOM_DEFINE	=0x80000000,//�Զ���λ��
}
NCFG_ENUM_DISP_POS;//

//֧����Щ��Ƶ��ʽ
typedef enum _ncfg_enum_support_video_format
{
	NCFG_ENUM_SUPPORT_VIDEOFORMAT_PAL=0x01,
	NCFG_ENUM_SUPPORT_VIDEOFORMAT_NTSC=0x02
}
NCFG_ENUM_SUPPORT_VIDEO_FORMAT;

//DDNS֧���������Ե�ȡֵ��Χ���������������Ͷ�����
typedef enum _ncfg_enum_ddns_support_property
{
	NCFG_ENUM_DDNS_SUPPORT_DOMAIN1	=0x01,//֧������1��Ҳ�п�����Ҫ֧����������	
	NCFG_ENUM_DDNS_SUPPORT_SERVERTYPE = 0x02,
}
NCFG_ENUM_DDNS_SUPPORT_PROPERTY;

//����IP�趨��Ϣ
typedef struct _ncfg_info_localnet_
{
	char getIPMode;
	unsigned char	ucNoUsed[3];
	unsigned long ipaddr;
	unsigned long netmask;
	unsigned long gateway;
}
NCFG_INFO_LOCALNET;

//PPPOE��Ϣ
typedef struct _ncfg_info_pppoe_
{
	unsigned char enable;
	unsigned char ucNoUsed[3];
	char username[132];
	char password[132];
}
NCFG_INFO_PPPOE;

typedef struct _ncfg_info_filt_ip
{
	unsigned long	dwIp;			//��Ҫ���˵�IP��ַ/����
	unsigned long	ipMask;			//���룬���ȫ0����IP��ַ�����������������ж����������
}NCFG_INFO_FILT_IP;


//�����ų�ͷ��Ϣ
typedef struct _ncfg_info_holiday_sche_head
{
	unsigned long holidaynum;//��������
	unsigned long offset;//������ÿ���ų�����Ե�һ��NCFG_INFO_HOLIDAY_SCHE_HEAD��ͷ��ƫ��
}
NCFG_INFO_HOLIDAY_SCHE_HEAD;

//ҹ�ӹ��ܶ���
typedef enum _night_vision_define
{
	NCFG_ENUM_NIGHT_VISION_AUTO=0,
	NCFG_ENUM_NIGHT_VISION_ON=1,
	NCFG_ENUM_NIGHT_VISION_OFF=2,
}
NCFG_ENUM_NIGHT_VISION;//

typedef struct _camera_freq_support
{
	unsigned long dwFreq;//���ֵƹ�ģʽ
	char ucName[16];//�ƹ�ģʽ������
}CAMERA_FREQ_SUPPORT;

typedef struct _ptz_protocol_info 
{
	unsigned char  ucprotocolID; /////��Ӧ��Э������ID
	unsigned char  supportproperty;//֧�ֵĳ�ID��������֮����������Ե�MASK
	unsigned char  noused[2];
	char           szProtocolName[64];  ////Э������
}PTZ_PROTOCOL_INFO, *LPPTZ_PROTOCOL_INFO;

typedef struct _ptz_baudrate_info 
{
	unsigned char  ucbaudrateID; /////��Ӧ�Ĳ�����ֵ
	unsigned char  noused[3];
	char           szbaudrateName[64];  ////��Ӧ�Ĳ���������
}PTZ_BAUDRATE_INFO, *LPPTZ_BAUDRATE_INFO;

/// to be remove
//��ģʽ TZ=CST+08:30:00CST+10:00:00,M3.1.0/02:0:0,M10.1.0/02:0:0
struct NCFG_DAYLIGHT_INFO_MODE_BY_WEEK	// ��ģʽ
{
	unsigned char	ucMonth;		//�ڼ��� 0 -- 11
	unsigned char	ucWeekOfMonth; //����µĵڼ��� 0 -- 4
	unsigned char	ucDayOfWeek;	//����ܵĵڼ���	0 -- 6	0��ʾ������
	unsigned char	ucHour;
	unsigned char	ucMinute;
	unsigned char	ucSecond;
	unsigned char	ucNoused[2];
};

//�ڰ����ϵĸ�ʽΪ TZ=PST-8:0:0PDT-7:0:0,J65/02:0:0,J270/02:0:0
struct NCFG_DAYLIGHT_INFO_MODE_BY_DAY	//����ģʽ
{
	unsigned char	ucMonth;		//�ڼ��� 0 -- 11
	unsigned char	ucDayOfMonth;		//����µĵڼ��� 0 -- 4	
	unsigned char	ucHour;
	unsigned char	ucMinute;
	unsigned char	ucSecond;
	unsigned char	ucNoused[3];
};

union NCFG_DAYLIGHT_MODE_INFO
{
	struct NCFG_DAYLIGHT_INFO_MODE_BY_WEEK	daylightModeByWeek;
	struct NCFG_DAYLIGHT_INFO_MODE_BY_DAY	daylightModeByDay;
};

struct NCFG_DAYLIGHT_INFO
{
	unsigned char		ucEnableDaylight;		//0 ������ʱ 1 ����ʱ		
	unsigned char		ucDayLightMode;			//0 ��ģʽ	 1 ����ģʽ
	unsigned short		nDisSecond;				//����
	NCFG_DAYLIGHT_MODE_INFO	daylightModeInfoStart;
	NCFG_DAYLIGHT_MODE_INFO	daylightModeInfoEnd;
};
///
/// for api
typedef struct
{	
	int	                dwSynchronizeType; 	// 0: �ֶ� 1: ntp������Уʱ  ��ʱ����
	char				szNtpServer[128];	//NTP��������ַ IP ���� host name
}NCFG_SYNCHRONIZE_INFO;

struct NCFG_TIMEZONE_INFO
{
	char 		    TimeZone[128];      /// ʱ���ַ���
	unsigned long	bDaylightSwitch;	/// 0 �ر�����ʱ 1 ��������ʱ 			
};

enum NCFG_ENCODE_VIDEO_ENCODE
{
    NCFG_ENCODE_H264 = 0x01,
//    NCFG_ENCODE_MJPEG= 0x02,
};

enum NCFG_ENCODE_AUDIO_ENCODE
{
    NCFG_ENCODE_G711A = 0,
	NCFG_ENCODE_G711U = 1,
};

struct NCFG_OSD_INFO_CONFIG
{
    unsigned int uShowOSDFlag;     //  1 show, 0 hide
    int iStartX;                   //  ��ʼλ��x������
    int iStartY;                   //  ��ʼλ��y������
    unsigned char ucOSDBuf[64];    //  gb2312����������
    unsigned int  uDataLen;         //  ucOSDBuf �ĳ��ȣ����ܳ���30,Ҳ���ǲ��ܳ���15���ַ�
    unsigned int  uShowLevel;      //  ��ʾ�ȼ�, 0 low, 1 middle, 2 high
    unsigned int  uFlicker;        //  �Ƿ���˸, 0  not flicker, 1 flicker
};


typedef struct _onvif_client_info 
{
	unsigned char  ucONVIFID; /////��Ӧ�ķ�����ID   ID����0��Ӧ�ķ��������Ʋ���Ч
	unsigned char  noused[3];
	char           szOnvifClientName[64];  ////ONVIF�ͻ��ʵ�����
}ONVIF_CLIENT_INFO, *LONVIF_CLIENT_INFO;



// --------------------------------------------------------
// ---------------------------------------��Ƶ�ڵ����START

// mask���ε�λ�úͳߴ� - Լ����VGAΪ�ο�
struct NCFG_RECT
{
    int 				s32X;
    int 				s32Y;
    unsigned int 		u32Width;
    unsigned int 		u32Height;
};

// ͸���� - ��˼������֧��
enum NCFG_FOSD_TRANSPARENT_e
{
	NCFG_FOSD_TRANSPARENT_PERCENT_0,
	NCFG_FOSD_TRANSPARENT_PERCENT_50,
	NCFG_FOSD_TRANSPARENT_PERCENT_75,
	NCFG_FOSD_TRANSPARENT_PERCENT_100,
};

typedef struct _ncfg_osd_mask_config
{
	bool 					bShow;
	NCFG_RECT 				rectang;
	unsigned int 			color; // 0x00RRGGBB
	NCFG_FOSD_TRANSPARENT_e	transparent;
}NCFG_OSD_MASK_CONFIG;
// ---------------------------------------��Ƶ�ڵ����END
// --------------------------------------------------------

typedef struct _nfcg_fog_reduction_config
{
	unsigned char  ucFogReduction;          // 0 no, 1 yes
	unsigned char  ucFogReductionValue;     // 0 - 255
	char            noUse[2];
}NCFG_FOG_REDUCTION_CONFIG;

typedef struct _nfcg_lens_distortion_config
{
	unsigned char  ucLensDistortion;        // 0 no, 1 yes
	unsigned char  ucLensDistortionValue;   // 0 - 255
	char            noUse[2];
}NCFG_LENS_DISTORTION_CONFIG;

enum AF_CTRL_TYPE
{
	ONETIME_FOCUS = 0,   // һ���Խ�
	MANUAL_FOCUS,        // �ֶ��Խ�
	MANUAL_ZOOM,         // �ֶ��䱶
	AUTO_FOCUS,          // �Զ�һֱ�Խ�
};
typedef struct _af_control_
{
	AF_CTRL_TYPE afType;
	int			  nValue;  //�ֶ�΢��ʱ��1����+��0����-��������������nValue�ʹ�0
}AF_CONTROL_;


typedef struct _nfcg_video_gain
{
    unsigned char  ucAnalogGainValue;
    unsigned char  ucDigitalGainValue;
    char           noUse[2];
}NCFG_VIDEO_GAIN;

typedef struct _nfcg_digita_denoise
{
    unsigned char uc2DValue;
    unsigned char uc3DValue;
    char           noUse[2];
}NCFG_DIGITA_DENOISE;

typedef struct
{
    unsigned int iEnable; //  0 close, 1 open
    int iChn;               // ������ 1--8
    char szAreaName[16];    // ��������
    unsigned int x;         // ��ʼ�������
    unsigned int y;
    unsigned int w;
    unsigned int h;
    unsigned int iLevel;  //  1 -- 10
}NCFG_VIDEO_CONFIG_ROI;


#if 0
// -----------------------------------------------------------
// ��Ƶ������� ----------------------------------------------
typedef enum _AUDIO_SMP_RATE_E 
{ 
    AUDIO_SMP_RATE_8000   = 8000,    /* 8K samplerate*/
    AUDIO_SMP_RATE_12000  = 12000,   /* 12K samplerate*/    
    AUDIO_SMP_RATE_11025  = 11025,   /* 11.025K samplerate*/
    AUDIO_SMP_RATE_16000  = 16000,   /* 16K samplerate*/
    AUDIO_SMP_RATE_22050  = 22050,   /* 22.050K samplerate*/
    AUDIO_SMP_RATE_24000  = 24000,   /* 24K samplerate*/
    AUDIO_SMP_RATE_32000  = 32000,   /* 32K samplerate*/
    AUDIO_SMP_RATE_44100  = 44100,   /* 44.1K samplerate*/
    AUDIO_SMP_RATE_48000  = 48000,   /* 48K samplerate*/
    AUDIO_SMP_RATE_BUTT,
} AUDIO_SMP_RATE_E; 

typedef enum _AUDIO_BITWIDTH_E
{
    AUDIO_BITWIDTH_8   = 0,   /* 8bit width */
    AUDIO_BITWIDTH_16  = 1,   /* 16bit width*/
    AUDIO_BITWIDTH_32  = 2,   /* 32bit width*/
    AUDIO_BITWIDTH_BUTT,
} AUDIO_BITWIDTH_E;

typedef struct _NCFG_AUDIO_CFG
{
	AUDIO_SMP_RATE_E	sample;			// ������
	unsigned char		ucEncodeType;	// ���뷽ʽ
	AUDIO_BITWIDTH_E	bitwidth;		// ������λ��
	unsigned char		channel;		// ��ͨ��
	unsigned char		volume;			// ���� [0, 100]
}NCFG_AUDIO_CFG;
// ��Ƶ������� ----------------------------------------------
// -----------------------------------------------------------
#endif
typedef enum _pic_stream_resolution_
{
	PIC_RESOLUTION_CIF = 0, 
	PIC_RESOLUTION_D1 = 1,
}ENUM_PIC_STREAM_RESOLUTION;


// isp -------------------------------------------------------
// case NCFG_ITEM_EXPOSURE_CTRL: 			// �ع����:
#if 0
typedef enum _EXP_OP_TYPE_E
{
	TYPE_AUTO	= 0,
	TYPE_MANUAL	= 1,
} EXP_OP_TYPE_E;

typedef struct _AE_ATTR_S
{
    /* base parameter */
    ISP_AE_MODE_E enAEMode;		/*AE mode(lownoise/framerate)(onvif)*/
    HI_U16 u16ExpTimeMax;       /*the exposure time's max and min value .(unit :line)*/
    HI_U16 u16ExpTimeMin; 
    HI_U16 u16DGainMax;         /* the Dgain's min and max value*/
    HI_U16 u16DGainMin;
    HI_U16 u16AGainMax;			/* the Again's min and max value*/
    HI_U16 u16AGainMin;
    
    HI_U8  u8ExpStep;			/*AE adjust step*/
    HI_S16 s16ExpTolerance;		/*AE adjust tolerance*/
    HI_U8  u8ExpCompensation;	/*AE compensation*/ 
    ISP_AE_FRAME_END_UPDATE_E  enFrameEndUpdateMode;
    HI_BOOL bByPassAE;
    /*AE weighting table*/
    HI_U8 u8Weight[WEIGHT_ZONE_ROW][WEIGHT_ZONE_COLUMN];
} AE_ATTR_S;
typedef struct _ME_ATTR_S
{
	HI_S32 s32AGain;       		/* sensor analog gain (unit: times) */
	HI_S32 s32DGain;       		/* sensor digital gain (unit: times) */
	HI_U32 u32ExpLine;			/* sensor exposure time (unit: line ) */

	HI_BOOL bManualExpLineEnable;
	HI_BOOL bManualAGainEnable;
	HI_BOOL bManualDGainEnable;
} ME_ATTR_S;
typedef struct _NCFG_EXPOSURE_CTRL
{
	OP_TYPE_E			ExpType;			// �عⷽʽ
	AE_ATTR_S stAEAttr;
	ME_ATTR_S stMEAttr;
}NCFG_EXPOSURE_CTRL;
#endif

// case NCFG_ITEM_FOCUS_CTRL:				// �۽�����:


// case NCFG_ITEM_DIS:						// ���ַ��� Digital image stabilization:


// case NCFG_ITEM_Shading:					// ���ǲ���:


// case NCFG_ITEM_GAMMA:					// gamma:


// case NCFG_ITEM_ANTI_FLICKER:				// ����˸:


// case NCFG_ITEM_CRStrength:				// Crosstalk Removal ǿ��


// case NCFG_ITEM_SlowFrameRate:			// ��֡��

// isp -------------------------------------------------------
// -----------------------------------------------------------




#pragma pack()

}
#endif //_NET_CONFIG_DEFINE_H_
