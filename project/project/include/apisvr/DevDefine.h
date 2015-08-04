#ifndef __DEV_DEFINE_H__
#define __DEV_DEFINE_H__

#include "base_type_define.h"
namespace NETNS
{

#pragma pack( 4)

// Onvif�ӿڷ�������ö�ٶ���
enum ONVIF_RET
{
	ONVIF_RET_ok,
	ONVIF_RET_fault,
	ONVIF_RET_noSupported,
	ONVIF_RET_serviceNotSupported,
	ONVIF_RET_networkErr,
	ONVIF_RET_unkown,
	ONVIF_RET_typeErr,

	//�û������й�
	ONVIF_RET_anonymousNotAllowed,
	ONVIF_RET_usernameTooShort,
	ONVIF_RET_usernameTooLong,
	ONVIF_RET_passwordTooWeak,
	ONVIF_RET_usernameClash,

	// �������
	ONVIF_RET_invalidHostname,
	ONVIF_RET_invalidIPv4Address,
	ONVIF_RET_invalidIPv6Address,
	ONVIF_RET_invalidDnsName,
	ONVIF_RET_invalidGatewayAddress,
	ONVIF_RET_IPFilterListIsFull,
	ONVIF_RET_noIPv4Address,

	// ϵͳ
	ONVIF_RET_tooManyScopes,
	ONVIF_RET_scopeOverWriteProhibited,	
	ONVIF_RET_fixedScope,
	ONVIF_RET_noScope,

	// ý��
	ONVIF_RET_noSource,
	ONVIF_RET_settingsInvalid,



	// ͼ��

	// ��̨
	ONVIF_RET_presetExist,
	ONVIF_RET_invalidPresetName,
	ONVIF_RET_tooManyPresets,
	ONVIF_RET_noToken,

	//Generic faults
	ONVIF_RET_INVALID_ARGS, //���зǷ�����
	ONVIF_RET_INVALID_ARGS_VAL, //�������зǷ�ֵ

	//����
	ONVIF_BUFF_SIZE_NOT_ENOUGH, //��ȡʱ�������С����
	ONVIF_ERROR_PARAM, //��������

	ONVIF_USER_PASSWORD_ERROR=401,
};

enum  DEV_RET
{
	DEV_STATUS_SUCCESS,
	DEV_STATUS_PERMISSION_DENIED ,   
	DEV_STATUS_UNAUTHORIZED,   
	DEV_STATUS_PARAM_ERR,      // ���ݷǷ�
	DEV_STATUS_SYSTEM_BUSY,   //ϵͳæ
};

enum HTTP_STATUS_CODE
{
	HTTP_STATUS_OK = 200,
	HTTP_STATUS_MOVED_TEMPORARILY = 302,
	HTTP_STATUS_BAD_REQUEST = 400,			// malformed syntax 
	HTTP_STATUS_UNAUTHORIZED = 401,			// authentication needed, respond with auth hdr
	HTTP_STATUS_NOT_FOUND = 404,
	HTTP_STATUS_FORBIDDEN = 403,
	HTTP_STATUS_REQUEST_TIMEOUT = 408,
	HTTP_STATUS_NOT_IMPLEMENTED = 501,			// used for unrecognized requests 
	HTTP_STATUS_INTERNAL_SERVER_ERROR = 500,
} ;

////////////////////system
enum DEV_USER_GROUP
{
	USER_GROUP_NONE		= 0x00,
	USER_GROUP_ADMIN	= 0x01,
	USER_GROUP_ADVANCE	= 0x02,
	USER_GROUP_NORMAL	= 0x04
};

const tuint32 DEV_USER_NAME_LEN = 20;
const tuint32 DEV_USER_PWD_LEN  = 20;
struct DEV_USER_INFO
{
	DEV_USER_GROUP	dwUserType;		   
	char szUserName[DEV_USER_NAME_LEN];
	char szPassword[DEV_USER_PWD_LEN];	 
};

enum DEV_SYNCHRONIZE_TYPE
{
	DATETIME_MANUAL =  0,
	DATETIME_NTP ,
};

struct DEV_DATE
{
	// 2014-8-20 7:10:20
	tuint16 wYear;    
	tuint16 wMonth; 
	tuint16 wDay; 
};

struct DEV_TIME
{
	tuint16 wHour;    
	tuint16 wMinute; 
	tuint16 wSecond; 
};

struct DEV_DATETIME
{
	DEV_DATE date;
	DEV_TIME time;
};

struct DEV_REC_SEARCH
{
	DEV_DATETIME startTime;
	tuint32		 lastTime;
};

const tuint32 DEV_MAX_TIME_ZOME_LEN  = 128;
const tuint32 DEV_NTP_SERVER_MAX_LEN = 128;  
struct DEV_TIMEZONE
{
	char szTimeZone[ DEV_MAX_TIME_ZOME_LEN];
	bool			bDaylightSwitch;
};

struct DEV_ADJUST_TIME
{
	DEV_SYNCHRONIZE_TYPE synchronizeType;  
	char szNtpServer[DEV_NTP_SERVER_MAX_LEN];	//NTP��������ַ IP ���� host name
};

struct DEV_SYSTIME
{
	DEV_ADJUST_TIME adjustTime;
	DEV_TIMEZONE    timezone;
};

typedef tuint8 DEV_MAC[6];

struct DEV_INFO
{
	char szMode[32];
	char szBrand[32];
	char szDevDescription[32];
	char szSoftwareVersion[32];
	DEV_DATE softwareBuildDate;
	char szKernelVersion[32];
	char szHardwareVersion[32]; 
	DEV_MAC macAddr;
	tint16 wAudioInCount;
	tint16 wAudioOutCount;
	tint16 wAlarmInCount;
	tint16 wAlarmOutCount;


};


///////////////////////////stream info
enum DEV_VIDEO_ENCODING
{
	VIDEO_ENCODING_MJPEG,
	VIDEO_ENCODING_MPEG4,
	VIDEO_ENCODING_H264,
};

enum DEV_VIDEO_PROFILE
{
	H264_PROFILE_NULL,
	H264_PROFILE_BASELINE,
	H264_PROFILE_MAIN,
	H264_PROFILE_EXTENDED,
	H264_PROFILE_HIGH,
	MPEG_PROFILE_SP,
	MPEG_PROFILE_ASP,
};


struct DEV_TWO_DIMENSION
{
	tint32 nWidth;
	tint32 nHeight;
};

typedef DEV_TWO_DIMENSION DEV_RESOLUTION ;

struct DEV_RANGE
{
	tint32 nMin;
	tint32 nMax;
};

enum DEV_QUALITY_TYPE
{
	DEV_QUALITY_NULL,
	DEV_QUALITY_LOWEST,
	DEV_QUALITY_LOWER,
	DEV_QUALITY_MEDIUM,
	DEV_QUALITY_HIGHER,
	DEV_QUALITY_HIGHEST,
};

struct DEV_SREAM_CFG 
{
	DEV_RESOLUTION  res; 
	tuint32  nFrmRate;  
	tuint32  nBitRate;	
	DEV_QUALITY_TYPE   quality;
	DEV_VIDEO_ENCODING encode;
	DEV_VIDEO_PROFILE  profile;
	tint32   nIFrameInterval;

};	

struct DEV_SREAM_CAPS
{
	DEV_VIDEO_ENCODING  encodeType[8];
	tuint32				encodeTypeSize;
	DEV_RESOLUTION      resolutions[8];
	tuint32			    resolutionsSize;
	DEV_RANGE frmRate;
	DEV_RANGE IFrmInterval;
	DEV_VIDEO_PROFILE  profiles[8];
	tuint32			   profilesSize;
	DEV_RANGE bitrate;
};





// �����������Χ

enum DEV_WHITEBALANCE_MODE
{
	WHITE_BALANCE_AUTO,
	WHITE_BALANCE_MANUAL,
	WHITE_BALANCE_OUTDOOR,
	WHITE_BALANCE_INDOOR,
	WHITE_BALANCE_ATW,
	WHITE_BALANCE_NALAMP,
};

enum DEV_IRCUT_MODE
{
	IRCUT_AUTO,
	IRCUT_DAY,
	IRCUT_NIGHT,
	IRCUT_TIMER,
};

struct DEV_RANGE_3DIMESION
{
	tint32 nMin;
	tint32 nMax;
	tint32 nDef; //ȱʡֵ
};

struct DEV_CAMERA_PARAM_CAPS 
{
	tuint8  ucDefFrequency;
	DEV_RANGE_3DIMESION bright;
	DEV_RANGE_3DIMESION contrast;
	DEV_RANGE_3DIMESION saturation;
	DEV_RANGE_3DIMESION hue;
	bool bDefMirrorSwitch;
	bool bDefFlipSwitch;
	tuint8 ucDefWDR;
	bool bDefWDRSwitch;

	DEV_WHITEBALANCE_MODE  defWhiteBalanceMode;
	DEV_RANGE_3DIMESION   whiteBalanceYrGain;
	DEV_RANGE_3DIMESION   whiteBalanceYbGain;

	tuint8 ucDefDenoise;
	bool bDefDenoiseSwitch;
	bool bDefIrisSwitch;
	tuint8 ucDefSharpen;
	bool bDefDefSharpenSwitch;

	DEV_IRCUT_MODE defIRcut;
};   


// ���������
struct DEV_CAMERA_PARAM   
{
	tuint8  ucFrequency; 
	tuint8	ucBright;						  	//����
	tuint8	ucContrast;							//�Աȶ�
	tuint8	ucHue;							    //ɫ��
	tuint8	ucSaturation;						//���϶�
	bool    bMirrorSwitch;
	bool    bFlipSwitch;
	bool    bIrisSwitch;
	bool    bSharpenSwtich;                    //��
	tuint8	ucSharpenValue;                     //��ʱ��Ĭ��ֵ
	tuint8  ucDenoise;
	bool	bDenoiseSwitch;
	DEV_WHITEBALANCE_MODE  ucWhiteBalanceMode;		
	tuint32	dwWhiteBalanceRed;					//��ɫ����
	tuint32	dwWhiteBalanceBlue;					//��ɫ����

	bool    bWDRSwitch;				     
	tuint8  ucWDRValue;				   //��̬�򿪵�ʱ��ĵȼ�

	DEV_IRCUT_MODE  IRCutMode;  
};   


enum DEV_AUDIO_ENCODE
{
	AUDIO_ENCODE_G711A,
	AUDIO_ENCODE_G711U ,
};




// OSD
const tuint32 DVSCFG_DEVICE_NAME_MAX_LEN = 64;
//��ʾ�����յ�ģʽ
enum DEV_DATE_MODE
{
	DATE_MODE_YMD	= 0x01,		//������ģʽ
	DATE_MODE_MDY	= 0x02,		//������ģʽ
	DATE_MODE_DMY	= 0x03		//������ģʽ
};

struct DEV_OSD_TIMESTAMP
{
	bool   	bSwitch;			// 0 ��Ҫʱ���  1��Ҫʱ���
	tuint32		x;					// ʱ���λ�� ������VGAΪ�ο�
	tuint32		y;						
	DEV_DATE_MODE	format;				// ʱ���ʽ	DATE_MODE
};
struct DEV_OSD_DEVICE_NAME
{
	bool		bSwitch;
	tuint32		x;
	tuint32		y;	
	char		szDevName[DVSCFG_DEVICE_NAME_MAX_LEN];		//	�豸��
};

struct DEV_RECT
{
	tint32 			x;
	tint32 			y;
	tuint32 		width;
	tuint32 		height;
};

enum DEV_COLOR
{
	DEV_COLOR_BLACK,
	DEV_COLOR_WHITE,
	DEV_COLOR_GRAY,
};

struct DEV_OSD_PRIVACY_MASK
{
	bool 				bShow;
	DEV_RECT 	  rect;
	DEV_COLOR 		color; // 0x00RRGGBB - RGB need turing to YUV on GM soc.
};


//////////PTZ
struct DEV_PTZ_CAPS
{
	DEV_RANGE ctrlSpeed;
	tuint32   nPresetMaxCnt;
};

struct DEV_DOM_PTZ_CAPS
{
	tuint32  nCruiseMaxCnt;
	DEV_RANGE  cruisePresetSpeed;
	tuint32    cruisePresetMaxHoldTime;
	tuint32    cruisePresetMaxCnt;
};


enum DEV_PTZ_PROTO_TYPE
{

	PTZ_PELCOP,
	PTZ_PELCOD,
	PTZ_LILIN,
	PTZ_MINKING,
	PTZ_NEON,
	PTZ_STAR,
	PTZ_VIDO,
	PTZ_DSCP,
	PTZ_VISCA,


};

struct DEV_PTZ_PROTO
{
	tuint32	baudrate;		//������0-13�ֱ��ʾ110��300����..921600�Ĳ�����
	DEV_PTZ_PROTO_TYPE type;	//Э��ID��0Ϊû��������̨
	tuint8	addr;		//��̨�豸ID

};


enum DEV_PTZ_CTRL_TYPE
{
	PTZ_CTRL_UP,
	PTZ_CTRL_DOWN,
	PTZ_CTRL_LEFT,
	PTZ_CTRL_RIGHT,
	PTZ_CTRL_LEFTUP,
	PTZ_CTRL_LEFTDOWN,
	PTZ_CTRL_RIGHTUP,
	PTZ_CTRL_RIGHTDOWN,
	PTZ_CTRL_NEAR,
	PTZ_CTRL_FAR,
	PTZ_CTRL_ZOOMIN,
	PTZ_CTRL_ZOOMOUT,
	PTZ_CTRL_IRISOPEN,
	PTZ_CTRL_IRISCLOSE,
	PTZ_CTRL_STOP,
};

const tuint32 DEV_PRESET_NAME_MAX_LEN = 11;
const tuint32 DEV_CRUISE_NAME_MAX_LEN = 31;

struct DEV_PTZ_PRESET
{
	tuint8 preseIndex;   //Ԥ�õ�λ�����
	char  name[DEV_PRESET_NAME_MAX_LEN + 1]; //Ԥ�õ�����
	tuint8 speed;
	tuint8 holdtime;

};

struct DEV_PTZ_CRUISE
{
	tuint8 ucCuiseIndex;
	char szName[DEV_CRUISE_NAME_MAX_LEN];
};

const tuint32 DEV_ALARMOUT_MAX_COUNT = 32;
const tuint32 MAX_MOTION_HEIGHT = 32;

////////////motion
struct DEV_MOTION_CFG
{
	bool	bSwitch;
	tint32	nAlarmHoldTime;
	tint32	nMotionSen;						 
	tuint64	ullMotionArea[MAX_MOTION_HEIGHT];	//�ƶ���������
	tint32	nMotionAreaWidth;
	tint32  nMotionAreaHeight;
    //tuint64 nAlarmoutCh;	    //�����ļ���ͨ�����������ÿλ��ʾһ��ͨ��
    tuint32     nAlarmoutCh[DEV_ALARMOUT_MAX_COUNT];
    tuint32     nAlarmoutCount;
};


///////////////alarm
enum DEV_ALARMIN_VOLTAGE
{
	VOL_NO,
	VOL_NC,
};

const tuint32 DEV_SENSOR_NAME_LEN = 128;


struct DEV_SENSOR_CFG
{
	tuint32   nSensorIdx;										  //������ID
	char 			szSensorName[DEV_SENSOR_NAME_LEN];	//���������ơ�
	bool		  bSwitch;				              	//Sensor���أ����ͨ���رգ���Sensor�ĵ�ƽ�仯��Ч 0 �� 1 ��
	DEV_ALARMIN_VOLTAGE 	nVoltage;				    //Sensor�ı�����ƽ������/���գ���0 ���� 1 ����

	tuint32	  dwAlarmHoldTime;			        	//Sensor��������ʱ��
    //tuint32   	nAlarmoutCh;	    //�����ļ���ͨ�����������ÿλ��ʾһ��ͨ��
    tuint32     nAlarmoutCh[DEV_ALARMOUT_MAX_COUNT];
    tuint32     nAlarmoutCount;
};

const tuint32 DEV_ALARMOUT_NAME_LEN = 128;

struct DEV_ALARMOUT_CFG
{
	tuint32  nAlarmIdx;	
	char 		 szAlarmoutName[DEV_ALARMOUT_NAME_LEN];	//���ơ�	
	tuint32	  dwAlarmHoldTime;		    //��������ʱ��	        	
};



////////////////////network
enum DEV_IP_SETTING_MODE
{
	IP_SETTING_MODE_STATICIP,
	IP_SETTING_MODE_DHCP,
};

struct DEV_IPV4_CFG
{
	DEV_IP_SETTING_MODE mode;
	char szStaticIP[16];
	char szStaticIPRoute[16];
	char szStaticIPMask[16];
	bool bDnsFromDhcpSwitch;
	char szDnsServer1[16];
	char szDnsServer2[16];
};

struct DEV_PPPOE_CFG
{
	bool bSwitch;
	char szUsername[64];
	char szPassword[64];
};

struct DEV_NET_PORT
{
	tuint16 wWebPort;
	tuint16 wNetPort;
	tuint16 wRtspPort;

};

struct DEV_DDNS_CFG
{
	bool bSwitch;
	char szDdnsType[64];
	char szUsername[64];
	char szPassword[64];
	char szDomainName[64];
	char szServerName[64];
};

enum DEV_ATTRIBUTE_LEN
{
	DEV_ATTRI_NTP_SERVER_MAX_LEN,
};



/*
* RTSP �ṹ�嶨��
*/


enum RTSP_NETPROTOCOL
{
	RTSP_UDP,
	RTSP_TCP,
};

enum RTSP_MEDIADATA_TYPE
{
	RTSP_VIDEO,
	RTSP_AUDIO,
	RTSP_VIDEO_AND_ADUIO,
	RTSP_TALK,
};

enum RTSP_FRAME_TYPE
{
	RTSP_FRMAE_TYPE_VIDEO,
	RTSP_FRMAE_TYPE_AUDIO,
	RTSP_FRMAE_TYPE_PIC,
};

enum RTSP_VIDEO_ENCODE_TYPE
{
	RTSP_VIDEO_ENCODE_TYPE_H264,
	RTSP_VIDEO_ENCODE_TYPE_MPG4,
	RTSP_VIDEO_ENCODE_TYPE_MJPEG,
	RTSP_VIDEO_ENCODE_TYPE_H265,
	RTSP_VIDEO_ENCODE_TYPE_JPG,

};

enum RTSP_AUDIO_ENCODE_TYPE
{
	RTSP_AUDIO_ENCODE_TYPE_RAW,
	RTSP_AUDIO_ENCODE_TYPE_G726,
	RTSP_AUDIO_ENCODE_TYPE_G711,
	RTSP_AUDIO_ENCODE_TYPE_ADPCM,
	RTSP_AUDIO_ENCODE_TYPE_G711U,
	RTSP_AUDIO_ENCODE_TYPE_LPCM,
};

enum RTSP_PIC_ENCODE_TYPE
{
	RTSP_PIC_ENCODE_TYPE_BMP,
};

union RTSP_ENCODE_TYPE
{
	RTSP_VIDEO_ENCODE_TYPE videoEnc;
	RTSP_AUDIO_ENCODE_TYPE  audioEnc;
	RTSP_PIC_ENCODE_TYPE  picEnc;
};

struct RTSP_FRAME
{
	RTSP_FRAME_TYPE	frameType;   //�Ƿ�Ϊ��Ƶ������Ϊ��Ƶ
	RTSP_ENCODE_TYPE encodeType;//�������ͣ�������Ƶ����һ��ö��
	bool	bKeyFrame;        
	tint32  streamIndex;      
	tuint32 dwFrameRate;        //֡��
	tuint32 dwWidth;			//��Ƶ�Ŀ�
	tuint32 dwHeight;			//��Ƶ�ĸ�
	tuint64 ullFrametime;		//���ƫ��ʱ��
	tuint32 dwHz;				//����������,��ƵƵ��
	tuint32 dwBitwidth;			//����λ��
	tuint32 dwFrameLen;			//�����֡���ݳ���
};

enum DEV_TYPE
{
	DEVICE_IPC = 0,
	DEVICE_DVR,
};

#pragma pack()
}
#endif