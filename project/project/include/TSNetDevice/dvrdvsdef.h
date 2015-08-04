
#ifndef _DVRDVSDEFINE_
#define _DVRDVSDEFINE_
#include "Typedef.h"
#include "mylist.h"

#ifdef __ENVIRONMENT_WIN32__
#pragma pack(4)
#endif

//2010-01-26 14:59:00 YSW
//��֤���ļ���Ľṹ�屣��4�ֽڶ���
#include "device.h"

typedef enum _dvr_system_status
{
	DVR_SYSTEM_RUNING,		//������
	DVR_SYSTEM_REBOOT,		//ϵͳ����
	DVR_SYSTEM_REDRAW,		//���³�ʼ������
	DVR_SYSTEM_LOGOUT,		//��Ҫ�˳�����¼����
	DVR_SYSTEM_EXIT			//�˳�ϵͳ
}DVR_SYSTEM_STATUS;

//��Ƶ������,Ҫ���˸ĵ�
typedef enum _TVT_ENUM_VIDEO_STREAM_TYPE
{
	VIDEO_STREAM_NONE		= 0x00000000,		//������
	VIDEO_STREAM_SLOW_NET	= 0x00000001,		//������
	VIDEO_STREAM_FAST_NET	= 0x00000002,		//������
	VIDEO_STREAM_PLAYBACK	= 0x00000004,		//�ط���
}TVT_ENUM_VIDEO_STREAM_TYPE;

typedef enum _DDNS_SERVER_TYPE
{
	DDNS_NONE =0,
	DDNS_88IP,
	DDNS_DNS2P,
	DDNS_MEIBU,
	DDNS_DYNDNS,
	DDNS_NOIPDNS,
	DDNS_DVRDYDNS,
	DDNS_MINTDNS,
	DDNS_MYQSEE, //ר����US02��DDNS,����MINTDNS,������myq-see.com
	DDNS_DVRLIS, 
	DDNS_EASTERNDNS,
	DDNS_NEWDDNS,//ר����US09��DDNS,����MINTDNS,������www.newddns.com
	DDNS_VISIONICA,
	DDNS_TVT_IP_SERVER,////ͬΪ��˾��IP Server������ 
	DDNS_END,
}DDNS_SERVER_TYPE;

//�ⲿ��������
typedef enum _keyboard_type
{
	KEYBOARD_1UA	= 0x00, 
	KEYBOARD_1UB,
	KEYBOARD_1UC,
	KEYBOARD_1UD,
	KEYBOARD_1UE,
	KEYBOARD_1UF,
	KEYBOARD_1UG,
	KEYBOARD_1US,  //1US�ɰ�
	KEYBOARD_1UT,
	KEYBOARD_SUA,
	KEYBOARD_SUB,
	KEYBOARD_SUC,
	KEYBOARD_1UTW,
	KEYBOARD_1USS,  //1US�°� 
	KEYBOARD_15U,  //1.5U������ 
	KEYBOARD_1UV,
	KEYBOARD_1UH,
	KEYBOARD_1UI,	
	KEYBOARD_2UA  = 0x30,
	KEYBOARD_2UB,
	KEYBOARD_2UC,
	KEYBOARD_2UD,
	KEYBOARD_TWOEM = 0x60,
	KEYBOARD_MY1003K,
}KEYBOARD_TYPE;

typedef enum _video_out_device_type
{
	VIDEO_OUT_DEVICE_HDMI	= 0x0001,
	VIDEO_OUT_DEVICE_VGA	= 0x0002,
	VIDEO_OUT_DEVICE_CVBS	= 0x0004
}VIDEO_OUT_DEVICE_TYPE;

typedef enum _transparency
{
	TRANSPARENCY_100	= 0x00,
	TRANSPARENCY_75		= 0x40,
	TRANSPARENCY_50		= 0x80,
	TRANSPARENCY_25		= 0xdc,
	TRANSPARENCY_0		= 0xff,
}TRANSPARENCY;

typedef enum _play_speed
{
	PLAY_SPEED_1_32X = 0,
	PLAY_SPEED_1_16X,
	PLAY_SPEED_1_8X,	//1/8
	PLAY_SPEED_1_4X,	//1/4
	PLAY_SPEED_1_2X,	//1/2
	PLAY_SPEED_1X,		//1
	PLAY_SPEED_2X,
	PLAY_SPEED_4X,
	PLAY_SPEED_8X,
	PLAY_SPEED_16X,
	PLAY_SPEED_32X,
	PLAY_SPEED_NONE,
}PLAY_SPEED;

typedef enum _image_quality
{
	IMAGE_QUALITY_LOWEST	= 0x01,
	IMAGE_QUALITY_LOWER		= 0x02,
	IMAGE_QUALITY_LOW		= 0x03,
	IMAGE_QUALITY_MEDIUM	= 0x04,
	IMAGE_QUALITY_HEIGHTER	= 0x05,
	IMAGE_QUALITY_HEIGHTEST	= 0x06
}IMAGE_QUALITY;

//VGA�ֱ��ʣ��ȿ������ڱ���Ҳ��������֧�ֹ��ܲ���,'
//ע�⣺Ҫ��λ��ʾ
typedef enum _vga_resolution
{
	VGA_640X480		= 0x0001,//(640<<16)+(480),
	VGA_720X480		= 0x0002,//(720<<16)+(480),
	VGA_720X576		= 0x0004,//(720<<16)+(576),
	VGA_800X600		= 0x0008,//(800<<16)+(600),
	VGA_1024X768	= 0x0010,//(1024<<16)+(768),
	VGA_1280X960	= 0x0020,//(1280<<16)+(960),
	VGA_1280X1024	= 0x0040,//(1280<<16)+(1024),
	VGA_1920X1080	= 0x0080,//(1920<<16)+(1080)
}VGA_RESOLUTION;

typedef struct _mac
{
	unsigned char mac [8];
}MAC;

//��¼�����յĽṹ��
typedef struct _dvr_date
{
	unsigned char  mday;
	unsigned char  month;
	unsigned short year;
	unsigned long  time;
}DVR_DATE;

//��ʾ�����յ�ģʽ
typedef enum _date_mode
{
	DATE_MODE_YMD	= 0x01,		//������ģʽ
	DATE_MODE_MDY	= 0x02,		//������ģʽ
	DATE_MODE_DMY	= 0x03		//������ģʽ
}DATE_MODE;

//��ʾʱ���ʽ
typedef enum _time_mode
{
	TIME_MODE_12	= 0x01,		//12Сʱ
	TIME_MODE_24	= 0x02,		//24Сʱ
}TIME_MODE;

typedef enum _daylight_type
{
	DAYLIGHT_TYPE_WEEK,
	DAYLIGHT_TYPE_DAY,
}DAYLIGHT_TYPE;


typedef enum _time_zone_name
{
	TIME_ZONE_GMT_D12=0,
	TIME_ZONE_GMT_D11,
	TIME_ZONE_GMT_D10,
	TIME_ZONE_GMT_D9,
	TIME_ZONE_GMT_D8,
	TIME_ZONE_GMT_D7,
	TIME_ZONE_GMT_D6,
	TIME_ZONE_GMT_D5,
	TIME_ZONE_GMT_D4_30,
	TIME_ZONE_GMT_D4,
	TIME_ZONE_GMT_D3_30,
	TIME_ZONE_GMT_D3,
	TIME_ZONE_GMT_D2,
	TIME_ZONE_GMT_D1,
	TIME_ZONE_GMT,
	TIME_ZONE_GMT_A1,
	TIME_ZONE_GMT_A2,
	TIME_ZONE_GMT_A3,
	TIME_ZONE_GMT_A3_30,
	TIME_ZONE_GMT_A4,
	TIME_ZONE_GMT_A4_30,
	TIME_ZONE_GMT_A5,
	TIME_ZONE_GMT_A5_30,
	TIME_ZONE_GMT_A5_45,
	TIME_ZONE_GMT_A6,
	TIME_ZONE_GMT_A6_30,
	TIME_ZONE_GMT_A7,
	TIME_ZONE_GMT_A8,
	TIME_ZONE_GMT_A9,	
	TIME_ZONE_GMT_A9_30,
	TIME_ZONE_GMT_A10,
	TIME_ZONE_GMT_A11,
	TIME_ZONE_GMT_A12,
	TIME_ZONE_GMT_A13,
}TIMEZOMENAME;

typedef struct _osd_info
{
	unsigned short x;			//
	unsigned short y;			//
	unsigned long	bEnable;	//�Ƿ���ʾ��
}OSD_INFO;

typedef struct _cover_info
{
	unsigned short x;
	unsigned short y;
	unsigned short cx;
	unsigned short cy;

	unsigned long color;
	unsigned long bCover;
}COVER_AREA;

const unsigned char MAX_COVER_AREA_NUM = 3;
typedef struct _camera_cover_info
{
	COVER_AREA area[MAX_COVER_AREA_NUM];
}CAMERA_COVER_INFO;

typedef enum _display_dev_mode
{
	DISPLAY_DEV_MINOR = 0,
	DISPLAY_DEV_MAJOR,
}DISPLAY_DEV_MODE;

typedef struct _dispaly_dev_info 
{
	unsigned short  devId;			//�豸���
	unsigned short  videoType;		//�豸����VIDEO_OUT_TYPE

	unsigned short	bValid;			//����ʾ�豸�Ƿ����
	unsigned short	devMode;		//��������������DISPLAY_DEV_MODE

	unsigned short	width;			//�豸�ķֱ���
	unsigned short	height;			//
}DISPLAY_DEV_INFO;

typedef struct _daylight_info
{
	unsigned char InMonth;		//�ڼ��½���DST
	unsigned char InMday;		//�ڼ��ս���DST�����ڷ�ʽ��Ч��
	unsigned char OutMonth;		//�ڼ��³�DST
	unsigned char OutMday;		//�ڼ��ճ�DST�����ڷ�ʽ��Ч��

	unsigned char InWeekIndex;	//�ڼ��ܽ���DST���ܷ�ʽ��Ч��
	unsigned char InWday;		//���ڼ�����DST���ܷ�ʽ��Ч��
	unsigned char OutWeekIndex;	//�ڼ��ܳ�DST���ܷ�ʽ��Ч��
	unsigned char OutWday;		//���ڼ���DST���ܷ�ʽ��Ч��

	unsigned short InYear;		//����DST����ݣ��������ã����Ƿ��ڴˣ��պÿ��Զ���ṹ�壬���Ա�����
	unsigned short OutYear;		//��DST����ݣ��������ã����Ƿ��ڴˣ��պÿ��Զ���ṹ�壬���Ա�����

	unsigned short enable;		//�Ƿ�������ʱ����
	unsigned short type;		//����ʱ���÷�ʽ���ܻ������ڷ�ʽ

	unsigned long InSecond;		//����DSTһ���ڵ���ƫ�ƣ�0��86399������ת����ʱ����
	unsigned long OutSecond;	//��DSTһ���ڵ���ƫ�ƣ�0��86399������ת����ʱ����

	unsigned long offSet;		//����ʱ��ƫ������0��86399���롣

}DAYLIGHT_INFO;

typedef struct _zone_time_info
{
	unsigned short UseZone; //�Ƿ�ʹ��ʱ��,ʱ��
	unsigned short UseTime;

	unsigned char TimeZone; //ʱ��
	unsigned char hour;
	unsigned char min;
	unsigned char sec;

	unsigned char mday;
	unsigned char month;
	unsigned short year;//2008~2025
}ZONE_TIME_INFO;

typedef enum _view_split_mode
{
	VIEW_SPLIT_1X1	= 0x00010000,	//1X1

	VIEW_SPLIT_2X2	= 0x00020000,	//2X2
	
	VIEW_SPLIT_1A2	= 0x00040000,	//2X3
	VIEW_SPLIT_2X3,

	VIEW_SPLIT_1A5	= 0x00080000,	//3X3
	VIEW_SPLIT_3X3,
	
	VIEW_SPLIT_1A7	= 0x00100000,	//4X4
	VIEW_SPLIT_1A12,
	VIEW_SPLIT_4X4,

	VIEW_SPLIT_2A6	= 0x00200000,	//4X6
	VIEW_SPLIT_4X6,

	VIEW_SPLIT_1A9	= 0x00400000,	//5X5
	VIEW_SPLIT_4A9,
	VIEW_SPLIT_1A16,
	VIEW_SPLIT_4A16,
	VIEW_SPLIT_5X5,

	VIEW_SPLIT_1A11	= 0x00800000,	//6X6
	VIEW_SPLIT_1A20,
	VIEW_SPLIT_4A20,
	VIEW_SPLIT_6X6,
}VIEW_SPLIT_MODE;

inline unsigned char SplitModeToNum(VIEW_SPLIT_MODE mode)
{
	unsigned char num = 1;
	switch(mode)
	{
	case VIEW_SPLIT_2X2:
		num = 4;
		break;
	case VIEW_SPLIT_2X3:
		num = 6;
		break;
	case VIEW_SPLIT_1A5:
		num = 6;
		break;
	case VIEW_SPLIT_3X3:
		num = 9;
		break;
	case VIEW_SPLIT_1A7:
		num = 8;
		break;
	case VIEW_SPLIT_1A12:
		num = 13;
		break;
	case VIEW_SPLIT_4X4:
		num = 16;
		break;
	case VIEW_SPLIT_4X6:
		num = 24;
		break;
	case VIEW_SPLIT_5X5:
		num = 25;
		break;
	case VIEW_SPLIT_6X6:
		num = 36;
		break;
	default:
		num = 1;
		break;
	}

	return num;
}

typedef struct _zoom_info
{
	unsigned short x;
	unsigned short y;
	unsigned short width;
	unsigned short height;
}ZOOM_INFO;

//��������
typedef enum _alarm_type
{
	ALARM_TYPE_NULL         = 0x0000,
	ALARM_TYPE_MONTION      = 0x0001,
	ALARM_TYPE_SENSOR       = 0x0002,
	ALARM_TYPE_VIDEOLOSS    = 0x0004,
	ALARM_TYPE_SPEED        = 0x0008,

	ALARM_TYPE_DISK_FULL    = 0x0010,
	ALARM_TYPE_IP_CONFLICT  = 0x0020,
	ALARM_TYPE_DISCONNET    = 0x0040,
	ALARM_TYPE_DISK_WARNING   = 0x0080,

	ALARM_TYPE_TEST         = 0xffffffff,
}ALARM_TYPE;

//ͼƬ������
typedef struct _snap_pic_prority
{
	unsigned short  diskIndex;
	unsigned short  nLogPos;

	unsigned long   nPos;
	LONGLONG        snapTime;
	
	unsigned short  nChannel;   //ͨ����
	unsigned short  nType;      //��������	
	unsigned short  bLock;      //�Ƿ�����
	unsigned short  bFill;      //�Ƿ������
	unsigned long   nLen;
}SNAP_PIC_PRORITY;

//�������������Ĵ���ʽ����ͨ�ģ����Բ���ģ��ͨ����������ͳһ����
//�����Ϳ���ʹ��AlarmIn��Ϊ���࣬������һ����������������Ϳ����ˡ�
//�����������ͱ��������������ܼ�ϣ�����������Ҳֻ�ܴ�β�����롣
typedef enum _other_alarm_type
{
	OTHER_ALARM_TYPE_DISK_FULL,
	OTHER_ALARM_TYPE_IP_CONFLICT,
	OTHER_ALARM_TYPE_DISCONNET,
	OTHER_ALARM_TYPE_DISK_WARNING,
	OTHER_ALARM_TYPE_NUM
}OTHER_ALARM_TYPE;

//¼����������
typedef enum _record_type
{
	RECORD_TYPE_NULL		= 0x0000,

	RECORD_TYPE_MANUAL		= 0x0001,		//�ֶ�¼��
	RECORD_TYPE_SCHEDULE	= 0x0002,		//�ų�¼��
	RECORD_TYPE_MOTION		= 0x0004,		//�ƶ����¼��
	RECORD_TYPE_SENSOR		= 0x0008,		//����������¼��

	RECORD_TYPE_END			= 0x0010,		//��ʾ���������������
	RECORD_TYPE_ALL			= 0xffff
}RECORD_TYPE;

//FILE_INFO_EX��RECLOG_INFO_EX�е�type�Ĵ�0-16λ���ڼ�¼¼�����͡�
//��32λ���ڼ�¼�Ƿ�������
//��31λ���ڱ�ʾ��¼ֻ�����ԣ�ע�⣺ֻ������������ʱ����ֻ�����������Ľ����¼�У�����λ����һ�������塣
//��30λ���ڱ�ʾ��¼����д��ע�⣺ֻ������������ʱ����ֻ�����������Ľ����¼�У�����λ����һ�������塣
//��29λ���ڱ�ʾ��¼���ڱ�����ע�⣺ֻ������������ʱ����ֻ�����������Ľ����¼�У�����λ����һ�������塣
//��28λ���ڱ�ʾ��¼����ʽ��ע�⣺ֻ������������ʱ����ֻ�����������Ľ����¼�У�����λ����һ�������塣

#define RECORD_FILE_IS_LOCK(type)       (0x80000000 & (type))
#define RECORD_FILE_EN_LOCK(type)       ((type) |= (0x80000000))
#define RECORD_FILE_EN_UNLOCK(type)     ((type) &= (0x7fffffff))

#define RECORD_FILE_IS_ONLYREAD(type)   (0x40000000 & (type))
#define RECORD_FILE_EN_ONLYREAD(type)   ((type) |= (0x40000000))

#define RECORD_FILE_IS_WRITING(type)    (0x20000000 & (type))
#define RECORD_FILE_EN_WRITING(type)    ((type) |= (0x20000000))

#define RECORD_FILE_IS_READING(type)    (0x10000000 & (type))
#define RECORD_FILE_EN_READING(type)    ((type) |= (0x10000000))

#define RECORD_FILE_IS_UNOFFICAL(type)    (0x08000000 & (type))
#define RECORD_FILE_EN_UNOFFICAL(type)    ((type) |= (0x08000000))

#define RECORD_TYPE_IS_NORMAL(type) (0x0f & type)

//��ȡ¼�����͵�����
inline unsigned char RecTypeNum()
{
	for (unsigned char i=0; i<16; ++i)
	{
		if ((0x01 << i) & RECORD_TYPE_END)
		{
			return i;
		}
	}

	return 0;
}

//��ȡĳ��¼�����͵��������
inline unsigned char RecTypeIndex(RECORD_TYPE type)
{
	unsigned char num = RecTypeNum();

	for (unsigned char i=0; i<num; ++i)
	{
		if ((0x01 << i) & type)
		{
			return i;
		}
	}

	return num;
}

typedef struct _encode_info
{
	unsigned char	resolution;		//ͼ��ֱ���
	unsigned char	rate;			//����֡��
	unsigned char	encodeType;		//�������ͣ��������ͱ�����
	unsigned char	quality;		//��������
	unsigned long	lBitStream;		//������������
	unsigned long	hBitStream;		//������������
}ENCODE_INFO;

//	����������չ�ṹ�嶨��Ӧ����XDVR3.0�汾�ϵġ�
typedef struct _file_info_ex
{
	unsigned short	deviceID;		//�����豸ID
	unsigned short	cameraID;		//�����豸ͨ��ID

	unsigned short	fileIndex;		//�ļ����
	unsigned short	diskIndex;		//�������

	unsigned long	type;			//�������
	unsigned long	chnn;			//ͨ����
	unsigned long	startTime;		//��ʼʱ��
	unsigned long	endTime;		//����ʱ��
}FILE_INFO_EX;

typedef struct _reclog_info_ex 
{
	unsigned short	deviceID;		//�����豸ID
	unsigned short	cameraID;		//�����豸ͨ��ID

	unsigned short	fileIndex;		//�ļ����
	unsigned short	diskIndex;		//�������

	unsigned long	type;			//�������
	unsigned long	chnn;			//ͨ����
	unsigned long	startTime;		//��ʼʱ��
	unsigned long	endTime;		//����ʱ��
}RECLOG_INFO_EX;

typedef struct _section_info_ex
{
	unsigned long	startTime;		//��ʼʱ��
	unsigned long	endTime;		//����ʱ��
}SECTION_INFO_EX;

typedef CMyList <RECLOG_INFO_EX>		RECLOG_INFO_EX_LIST;
typedef CMyList <FILE_INFO_EX>			FILE_INFO_EX_LIST;
typedef CMyList <SECTION_INFO_EX>		SECTION_INFO_EX_LIST;
typedef CMyList <DVR_DATE>				DVR_DATA_LIST;

//

//�¼���־
typedef enum _event_type
{
    EVENT_TYPE_MOTION   = 0x0001,		//�ƶ����
    EVENT_TYPE_SENSOR   = 0x0002,		//����������
    EVENT_TYPE_V_LOSS   = 0x0004,		//��Ƶ��ʧ
	EVENT_TYPE_V_COVER	= 0x0008,		//��Ƶ�ڵ�
}EVENT_TYPE;

typedef struct _event_log
{
	unsigned short chnn;			//�¼���Ӧ��ͨ��
	unsigned short type;			//�¼����ͣ�EVENT_TYPE

	unsigned long startTime;		//�¼������Ŀ�ʼʱ��
	unsigned long endTime;			//�¼��Ľ���ʱ��
}EVENT_LOG, *P_EVENT_LOG;

typedef CMyList <EVENT_LOG> EVENT_LOT_LIST;

typedef struct _operator_log
{
	unsigned short	contentID;		//��־����ID
	unsigned short	type;			//��־�����ͣ�LOG_TYPE

	unsigned long	IP;				//��������־ʱ�û�IP
	unsigned long	time;			//����־����ʱ��
	char			name [36];		//����־��Ӧ���û�����
}OPERATOR_LOG, *P_OPERATOR_LOG;

typedef CMyList <OPERATOR_LOG> OPERATOR_LOG_LIST;

//��λ���ֽڶ�ӦcontentID����λ���ֽڶ�Ӧtype��
typedef enum _log_type
{
	//ϵͳ����(Control)
	LOG_TYPE_SYSTEM_CTRL		= 0x00010000,
	LOG_TYPE_BOOT,							//ϵͳ����
	LOG_TYPE_SHUTDOWN,						//ϵͳ�ػ�
	LOG_TYPE_REBOOT,						//ϵͳ����
	LOG_TYPE_FORMAT_SUCC,					//��ʽ�����̳ɹ�
	LOG_TYPE_FORMAT_FAIL,					//��ʽ������ʧ��
	LOG_TYPE_UPGRADE_SUCC,					//�����ɹ�
	LOG_TYPE_UPGRADE_FAIL,					//����ʧ��
	LOG_TYPE_CLEAR_ALARM,					//�������
	LOG_TYPE_OPEN_ALARM,					//��������
	LOG_TYPE_MANUAL_START,					//�����ֶ�¼��
	LOG_TYPE_MANUAL_STOP,					//ֹͣ�ֶ�¼��
	LOG_TYPE_PTZ_ENTER,						//��ʼ��̨����
	LOG_TYPE_PTZ_CTRL,						//��̨����
	LOG_TYPE_PTZ_EXIT,						//������̨����
	LOG_TYPE_AUDIO_CH_CHANGE,				//�ı��ֳ���Ƶͨ��
	LOG_TYPE_VOLUME_ADJUST,					//��������
	LOG_TYPE_MUTE_ENABLE,					//��������
	LOG_TYPE_MUTE_DISENABLE,				//�رվ���
	LOG_TYPE_DWELL_ENABLE,					//������ѭ
	LOG_TYPE_DWELL_DISENABLE,				//�ر���ѭ
	LOG_TYPE_LOG_IN,						//��¼
	LOG_TYPE_LOG_OFF,						//�ǳ�
	LOG_TYPE_CHANGE_TIME,					//�޸�ϵͳʱ��
	LOG_TYPE_MANUAL_SNAP_SUCC,               //�ֶ�ץͼ�ɹ�
	LOG_TYPE_MANUAL_SNAP_FAIL,               //�ֶ�ץͼʧ��

	//ϵͳ����(Setup)
	LOG_TYPE_CONFIG			= 0x00020000,
	LOG_TYPE_CHGE_VIDEO_FORMAT,				//�ı���Ƶ��ʽ
	LOG_TYPE_CHGE_VGA_RESOLUTION,			//�ı���ʾ���ֱ���
	LOG_TYPE_CHGE_LANGUAGE,					//��������
	LOG_TYPE_CHGE_NET_USER_NUM,				//���������û���Ŀ
	LOG_TYPE_CHGE_TIME_ZONE,				//����ʱ��
	LOG_TYPE_NTP_MANUAL,					//�ֶ�����Уʱ
	LOG_TYPE_NTP_ON,						//�����Զ�����Уʱ
	LOG_TYPE_NTP_OFF,						//�ر��Զ�����Уʱ
	LOG_TYPE_CHGE_NTP_SERVER,				//�޸�����ʱ���������ַ
	LOG_TYPE_CHGE_DST,						//��������ʱ����
	LOG_TYPE_PASSWD_ON,						//������������
	LOG_TYPE_PASSWD_OFF,					//�رղ�������

	LOG_TYPE_CHGE_CAM_NAME,					//����ͨ������
	LOG_TYPE_MODIFY_COLOR,					//����ͼ��ɫ��
	LOG_TYPE_CHGE_HOST_MONITOR,				//��������������������
	LOG_TYPE_CHGE_SPOT,						//�������������������
	LOG_TYPE_CHGE_OSD,						//�����ַ���������

	LOG_TYPE_CHGE_LOCAL_ENCODE,				//����¼�����������
	LOG_TYPE_CHGE_REC_VIDEO_SWITCH,			//����¼�񿪹�����
	LOG_TYPE_CHGE_REC_AUDIO_SWITCH,			//����¼����Ƶ��������
	LOG_TYPE_CHGE_REC_REDU_SWITCH,			//��������¼�񿪹�����
	LOG_TYPE_CHGE_REC_PRE_TIME,				//������ǰ¼��ʱ��
	LOG_TYPE_CHGE_REC_POST_TIME,			//��������¼��ʱ��
	LOG_TYPE_CHGE_REC_HOLD_TIME,			//����¼�����ݹ���ʱ��

	LOG_TYPE_CHGE_SCH_SCHEDULE,				//������ʱ¼��ƻ�
	LOG_TYPE_CHGE_SCH_MOTION,				//�����ƶ����¼��ƻ�
	LOG_TYPE_CHGE_SCH_ALARM,				//��������¼��ƻ�

	LOG_TYPE_CHGE_SENSOR_SWITCH,			//�����������뿪������
	LOG_TYPE_CHGE_SENSOR_TYPE,				//�������������豸����
	LOG_TYPE_CHGE_SENSOR_TRIGGER,			//�����������봦��ʽ����
	LOG_TYPE_CHGE_SENSOR_SCH,				//���������������ƻ�

	LOG_TYPE_CHGE_MOTION_SWITCH,			//�����ƶ���⿪������
	LOG_TYPE_CHGE_MOTION_SENS,				//�����ƶ����������
	LOG_TYPE_CHGE_MOTION_AREA,				//�����ƶ������������
	LOG_TYPE_CHGE_MOTION_TRIGGER,			//�����ƶ���⴦��ʽ
	LOG_TYPE_CHGE_MOTION_SCH,				//�����ƶ����ƻ�

	LOG_TYPE_CHGE_VL_TRIGGER,				//������Ƶ��ʧ����ʽ����

	LOG_TYPE_CHGE_RELAY_SWITCH,				//�������������������
	LOG_TYPE_CHGE_RELAY_SCH,				//������������ƻ�

	LOG_TYPE_BUZZER_ON,						//�������������豸
	LOG_TYPE_BUZZER_OFF,					//�ر����������豸
	LOG_TYPE_CHGE_BUZZER_SCH,				//�������������ƻ�

	LOG_TYPE_CHGE_HTTP_PORT,				//�޸�HTTP�������˿�
	LOG_TYPE_CHGE_SER_PORT,					//�޸�����������˿�
	LOG_TYPE_CHGE_IP,						//���������ַ
	LOG_TYPE_DHCP_SUCC,						//�Զ���ȡ�����ַ�ɹ�
	LOG_TYPE_DHCP_FAIL,						//�Զ���ȡ�����ַʧ��
	LOG_TYPE_CHGE_PPPOE,					//����PPPoE
	LOG_TYPE_CHGE_DDNS,						//����DDNS
	LOG_TYPE_NET_STREAM_CFG,				//������������������
	
	LOG_TYPE_CHGE_SERIAL,					//������̨��������
	LOG_TYPE_PRESET_MODIFY,					//����Ԥ�õ�
	LOG_TYPE_CRUISE_MODIFY,					//����Ѳ����
	LOG_TYPE_TRACK_MODIFY,					//�����켣

	LOG_TYPE_USER_ADD,						//�����û�
	LOG_TYPE_USER_MODIFY,					//�����û�Ȩ��
	LOG_TYPE_USER_DELETE,					//ɾ���û�
	LOG_TYPE_CHANGE_PASSWD,					//�޸��û�����

	LOG_TYPE_LOAD_DEFAULT,					//�ָ�Ĭ������
	LOG_TYPE_IMPORT_CONFIG,					//��������
	LOG_TYPE_EXPORT_CONFIG,					//��������

	LOG_TYPE_CHGE_IMAGE_MASK,				//ͼ���ڵ�
	LOG_TYPE_RECYCLE_REC_ON,				//����ѭ��¼��
	LOG_TYPE_RECYCLE_REC_OFF,				//�ر�ѭ��¼��
	LOG_TYPE_CHGE_DISK_ALARM,				//�������̱����ռ�

	LOG_TYPE_CHGE_SEND_EMAIL,				//����Email ��������Ϣ
	LOG_TYPE_CHGE_RECV_EMAIL,				//����Email ��������Ϣ
	LOG_TYPE_CHGE_SNAP_SETTING,             //����ץͼ����

	//¼��ط�(Playback)
	LOG_TYPE_PLAYBACK		= 0x00040000,
	LOG_TYPE_PLAYBACK_PLAY,					//����
	LOG_TYPE_PLAYBACK_PAUSE,				//��ͣ
	LOG_TYPE_PLAYBACK_RESUME,				//�ָ�����
	LOG_TYPE_PLAYBACK_FF,					//���
	LOG_TYPE_PLAYBACK_REW,					//����
	LOG_TYPE_PLAYBACK_STOP,					//ֹͣ
	LOG_TYPE_PLAYBACK_NEXT_SECTION,			//��һ��
	LOG_TYPE_PLAYBACK_PREV_SECTION,			//��һ��

	//���ݱ���(Backup)
	LOG_TYPE_BACKUP			= 0x00080000,
	LOG_TYPE_BACKUP_START,					//��ʼ����
	LOG_TYPE_BACKUP_COMPLETE,				//�������
	LOG_TYPE_BACKUP_CANCEL,					//��������
	LOG_TYPE_BACKUP_FAIL,					//����ʧ��

	//¼�����(Search)
	LOG_TYPE_SEARCH			= 0x00100000,
	LOG_TYPE_SEARCH_TIME,					//��ʱ�����
	LOG_TYPE_SEARCH_EVENT,					//���¼�����
	LOG_TYPE_SEARCH_FILE_MAN,				//�ļ�����
	LOG_TYPE_DELETE_FILE,					//ɾ���ļ�
	LOG_TYPE_LOCK_FILE,						//�����ļ�
	LOG_TYPE_UNLOCK_FILE,					//�����ļ�
	LOG_TYPE_DELETE_PICTURE,                //ɾ��ͼƬ
	LOG_TYPE_LOCK_PICTURE,                  //����ͼƬ
	LOG_TYPE_UNLOCK_PICTURE,                //����ͼƬ


	//�鿴��Ϣ(View information)
	LOG_TYPE_VIEW_INFO		= 0x00200000,
	LOG_TYPE_VIEW_SYSTEM,					//�鿴ϵͳ��Ϣ
	LOG_TYPE_VIEW_EVENT,					//�鿴�¼�
	LOG_TYPE_VIEW_LOG,						//�鿴��־
	LOG_TYPE_VIEW_NETWORK,					//�鿴����״̬
	LOG_TYPE_VIEW_ONLINE_USER,				//�鿴�����û�
	LOG_TYPE_VIEW_EXPORT_LOG,				//������־
	LOG_TYPE_VIEW_EXPORT_EVENT,				//�����¼�

	//�쳣��Ϣ(Error)
	LOG_TYPE_ERROR_INFO		= 0x00400000,
	LOG_TYPE_IP_CONFLICT,					//�����ַ��ͻ
	LOG_TYPE_NETWORK_ERR,					//�����쳣
	LOG_TYPE_DDNS_ERR,						//DDNS����
	LOG_TYPE_DISK_IO_ERR,					//���̶�д����
	LOG_TYPE_UNKNOWN_OFF,					//�쳣�ϵ�
	LOG_TYPE_UNKNOWN_ERR,					//δ֪����	
	LOG_TYPE_ERR_9A9A9A9A,
	LOG_TYPE_ERR_9A000001,
	LOG_TYPE_ERR_9A000002,
	LOG_TYE_DISK_WARNING,                  //����˥������
}LOG_TYPE;

inline unsigned short GetContentID(LOG_TYPE type)
{
	return (type & 0xffff0000) >> 16;
}

//��ʾλ����ͨ�����ơ��ֳ�ʱ�䡢ʱ�����ͼ���е�λ��
typedef enum _disp_pos
{
	DISP_TOP_RIGHT		= 1,	//����
	DISP_TOP_LEFT		= 2,	//����
	DISP_BOTTOM_RIGHT	= 3,	//����
	DISP_BOTTOM_LEFT	= 4,	//����
	DISP_TOP_MIDDLE		= 5,	//����
	DISP_BOTTOM_MIDDLE	= 6,	//����
	DISP_CENTER_LEFT	= 7,	//����
	DISP_CENTER_MIDDLE	= 8,	//����
	DISP_CENTER_RIGHT	= 9,	//����
	DISP_CUSTOM_DEFINE	=32,	//�Զ���λ��
}DISP_POS;//

//���ų̽ṹ��24����24Сʱ��ULONGLONG��ÿһλ����ÿһ���ӵ�״̬
typedef struct _date_schedule
{
	ULONGLONG hour [24];
}DATE_SCHEDULE;

//���ų̽ṹ��7��ʾÿ��7����ų�
typedef struct _week_schedule
{
	DATE_SCHEDULE week[7];
}WEEK_SCHEDULE;

//�����ų̣�Ŀǰ������ʵ�ַ�ʽ��һ����ÿ���ظ��ļ����ų̷�ʽ��һ����ָ�����ڵļ����ų̷�ʽ,year��ǰһ�ַ�ʽ����
typedef struct _holiday_schedule
{
	unsigned char		month;
	unsigned char		day;
	unsigned short		year;	
	DATE_SCHEDULE		date;  //��һ����ļƻ���24Сʱ��ÿ��Сʱÿһ������һ��bit��ʾ
}HOLIDAY_SCHEDULE;


//�����û�Ȩ�ޣ�ÿ���û�����Ĭ��Ȩ�ޣ����ǿ������µ��ھ���Ȩ�ޣ��������ϵ��ڣ���
typedef enum _user_group_
{
	USER_GROUP_NONE		= 0x00,	//
	USER_GROUP_ADMIN	= 0x01,	//����Ա��ӵ�����е�Ȩ��
	USER_GROUP_ADVANCE	= 0x02,	//�߼��û���Ĭ�Ͼ��У�������¼�����á��طš����ݡ����ݹ������̹�����̨���ƣ�Զ�̵�¼����ȫͨ��Ȩ��
	USER_GROUP_NORMAL	= 0x04	//һ���û���Ĭ�Ͼ��У�������¼�񡢻طš����ݡ���̨���ơ�Զ�̵�¼����ȫͨ��Ȩ��
}USER_GROUP;

typedef enum _user_authority
{
	AUTH_BASIC			= 0x0001,		//����Ȩ��

	//ͨ��Ȩ��
	AUTH_LIVE			= 0x0002,
	AUTH_RECORD			= 0x0004,		//¼��Ȩ�ޣ���������ֹͣ�ֶ�¼���Ȩ��
	AUTH_PLAYBACK		= 0x0008,		//�ط�Ȩ�ޣ��������ݼ������طſ���
	AUTH_BACKUP			= 0x0010,		//����Ȩ�ޣ�ÿ��ͨ���������ơ�
	AUTH_PTZ_CTRL		= 0x0020,		//��̨����Ȩ�ޣ�����̨����Ȩ��������
	AUTH_REMOTE_VIEW	= 0x0040,		//Զ��Ԥ������Զ�̲鿴��ͨ�����ֳ����档

	//ϵͳȨ��
	AUTH_LOG_VIEW		= 0x0080,		//�鿴ϵͳ��־
	AUTH_SYS_SETUP		= 0x0100,		//ϵͳ����
	AUTH_SHUTDOWN		= 0x0200,		//�ػ���������
	AUTH_AUDIO_TALK		= 0x0400,		//�����Խ�
	AUTH_DATA_MAN		= 0x0800,		//���ݹ�����������������ɾ��¼���ļ�
	AUTH_DISK_MAN		= 0x1000,		//���̹��������趨�������ԣ����Ը�ʽ�����̡�
	AUTH_REMOTE_LOGON	= 0x2000,		//Զ�̵�¼
	AUTH_ADVANCE_MAN	= 0x4000,		//�߼����������û������ظ����á�������ߵ������õ�
	AUTH_NET_DEVICE_MAN	= 0x8000,		//

	AUTH_ALL			= 0xffffffff	//ȫ��Ȩ��
}USER_AUTHORITY;


//unsigned long �еĺ�28λ�����ڼ�¼���ԡ���λ����¼����.����ֻ����28������
typedef enum _language_
{
	LANGUAGE_ENGLISH       = 0x0000001,  
	LANGUAGE_CHINESE_S     = 0x0000002,
	LANGUAGE_CHINESE_B     = 0x0000004,
	LANGUAGE_PORTUGUESE    = 0x0000008,
	LANGUAGE_GREECE        = 0x0000010,
	LANGUAGE_SPAISH        = 0x0000020,
	LANGUAGE_RUSSIAN       = 0x0000040,	
	LANGUAGE_NORWAY        = 0x0000080,
	LANGUAGE_TURKEY        = 0x0000100,
	LANGUAGE_ITALY         = 0x0000200,
	LANGUAGE_CZECH         = 0x0000400,
	LANGUAGE_GERMAN        = 0x0000800,
	LANGUAGE_HEBREW        = 0x0001000,
	LANGUAGE_JAPANESE      = 0x0002000,	
	LANGUAGE_FRENCH        = 0x0004000,
	LANGUAGE_POLISH        = 0x0008000,
	LANGUAGE_BULGARIAN     = 0x0010000,
	LANGUAGE_INDONESIA     = 0x0020000,
	LANGUAGE_RUSSIAN_D     = 0x0040000,
	LANGUAGE_THAI          = 0x0080000,

	LANGUAGE_ALL           = 0xFFFFFFF,
}LANGUAGE;
#define  MAX_LANGUAGE_PACK_NUM 28


typedef enum _getip_mode
{
	GETIP_MODE_STATICIP = 0,	//��̬IP	
	GETIP_MODE_DHCP		= 1,	//��̬IP
}GETIP_MODE;

typedef enum _client_status
{
	CLIENT_STATUS_NULL		= 0x0,
	CLIENT_STATUS_LIVE		= 0x0001,
	CLIENT_STATUS_PTZ		= 0x0002,
	CLIENT_STATUS_SEARCH	= 0x0004,
	CLIENT_STATUS_PB		= 0x0008,
	CLIENT_STATUS_BACKUP	= 0x0010,
	CLIENT_STATUS_INFO		= 0x0020,
	CLIENT_STATUS_SETUP		= 0x0040,
}CLIENT_STATUS;

typedef struct _ddns_info
{
	unsigned char	bEnable;	//�Ƿ�����DDNS����
	unsigned char	bUseDomain;	//�Ƿ�ʹ��������Ϣ��
	unsigned short	type;		//ʹ������Э��

	char	user[132];			//�û����ƣ�
	char	password[132];		//�û�����
	char	domain[132];		//��Ӧ��������Ϣ��
}DDNS_INFO;

typedef struct _pppoe_info
{
	unsigned long	bEnable;	//�Ƿ�����PPPoE����
	char	user[132];			//�û����ƣ�
	char	password[132];		//�û�����
}PPPOE_INFO;

typedef struct _network_addr_info
{
	unsigned long bDHCP;	//
	unsigned long staticIP;	//
	unsigned long gateway;	//
	unsigned long netMask;	//
}NETWORK_ADDR_INFO;

typedef struct _hxht_net_param_info_
{
	char videoDeviceID[132];		//DVR�ڽ���������е��豸ID,�ɽ�����������䣬DVR����������ע��ʱʹ��
	unsigned short	usVDevDataPort;	//DVR�����ݶ˿ڣ�������Ƶ����Ƶ����ʷ��Ƶ���Ự�˿ڵ�
	unsigned short	notUsed;

	unsigned long	csIP;			//�����������IP
	unsigned short	csMessagePort;	//�����������Ϣ�˿� Ĭ��ʹ��6005
	unsigned short	csStortPort;	//����������洢ע��˿� Ĭ��ʹ��6004
}HXHT_NET_PARAM_INFO;

typedef struct _motion_detect_info
{
	unsigned long sensitivity;  //�ƶ����������
	MOTION_AREA_INFO areaInfo;	//�ƶ�����������Ϣ��
}MOTION_DETECT_INFO;

typedef struct hxht_motion_rect
{
	unsigned short		xStart;
	unsigned short		yStart;
	unsigned short		width;
	unsigned short		height;
	unsigned char		bEnable;
	unsigned char		resv[3];
}HXHT_MOTION_RECT;

typedef struct _hxht_motion_area_info
{
	unsigned long sensitivity;  //�ƶ����������
	HXHT_MOTION_RECT	areaRect[32];
}HXHT_MOTION_AREA_INFO;

//����ʱ��������ͷ�ƶ�
typedef struct _to_ptz
{
	unsigned char	ucToPreset;				//����ʱ���ƶ�����һ��Ԥ�Ƶ�0 -- 127	 0xff ��ʾ����Ҫ�ƶ�
	unsigned char	ucBackPreset;			//�������󷵻��ĸ�Ԥ�Ƶ�0 -- 127		 0xff ��ʾ������
	char            cIsEnablePreset;        //���õ���ĳ��Ԥ�õ�
	char            cIsEnableBack;          //���ý���ʱ����ĳ����	
	unsigned char	ucToCruise;				//����ʱ,��������Ѳ����0 -- 16		 0xff ��ʾ������
	char            cIsEnableCruise;        //����Ѳ���߹���
	char			cIsEnableTrack;			//�����켣����
	unsigned char	ucToTrack;				//����ʱ,��������Ѳ����0 -- 3		 0xff ��ʾ������
}TO_PTZ;
//////////////////////////////////////////////////////////////////////////
typedef struct _net_client_info
{
	unsigned short	port;			//�ͻ���Զ�̻��Ķ˿ںţ������ֽ���
	unsigned short	status;			//��ʾ���û���ǰ�Ĳ���״̬

	unsigned long	clientType;		//�ͻ��˵����ͣ��������ġ��ֻ���IE��
	unsigned long	IP;				//���ӵĿͻ��˵�IP��ַ

	char			User[36];		//�û��� 
}NET_CLIENT_INFO;

//2010-06-10 10:15:00 YSW
//Ϊ�˿ͻ��˶��¾ɰ汾�ļ��ݣ�����������ṹ�壬�ɿͻ����ж�����������ʹ��
//NET_CLIENT_INFO����NET_CLIENT_INFO_EX�ṹ�塣
typedef struct _net_client_info_ex
{
	unsigned short	port;			//�ͻ���Զ�̻��Ķ˿ںţ������ֽ���
	unsigned short	status;			//��ʾ���û���ǰ�Ĳ���״̬

	unsigned long	clientType;		//�ͻ��˵����ͣ��������ġ��ֻ���IE��
	unsigned long	IP;				//���ӵĿͻ��˵�IP��ַ

	char			User[36];		//�û��� 
	unsigned long	clientID;		//�ͻ���ID��Ϊ�˺���ǰ�ͻ��˼��ݣ��ط������
}NET_CLIENT_INFO_EX;

typedef struct _stream_info
{
	unsigned char	bAudio;			//�Ƿ������Ƶ
	unsigned char	bCameraName;	//�Ƿ����ͨ������
	unsigned char	bTimeStamp;		//�Ƿ����ʱ���
	unsigned char	bVBR;			//
	
	unsigned char	frameRate;		//֡��
	unsigned char	quality;		//����
	unsigned char	resolution;		//�ֱ���
	unsigned char	bKeepRate;		//����������

	unsigned short	timeStampXPos;
	unsigned short	timeStampYPos;
	unsigned short	cameraNameXPos;
	unsigned short	cameraNameYPos;

	unsigned short	preRecTime;
	unsigned short	postRecTime;
	unsigned short	dataHoldTime;
	unsigned short	unused;
}STREAM_INFO;

typedef struct _network_status_info
{
	unsigned char	bDHCP;			//�Ƿ��Ƕ�̬��ȡ�����ַ��0��ʾ��̬IP
	unsigned char	bPPPoE;			//�Ƿ�����PPPoE
	unsigned char	bDDNS;			//�Ƿ�����DDNS, 0x1��ʾ������DDNS���ܵ���һ���÷������ã� 0x11��ʾ��ʱ��������������DVR��
	unsigned char	bWiFi;			//�Ƿ�������������

	unsigned short	httpPort;		//HTTP�������˿�	
	unsigned short	serverPort;		//ҵ��������˿�

	unsigned long	IP;				//��ǰ�����ַ
	unsigned long	subMask;		//��������
	unsigned long	gateway;		//����
	unsigned long	DNS1;			//��DNS������
	unsigned long	DNS2;			//��DNS������

	unsigned long   netstatus;		//�����״̬��0��ʾû�����ӵ�internet��1��ʾ���ӵ���internet��
									//Ϊ1��ʱ���ʾ���ڴ���up״̬���Ҳ��������ߣ����������Ϊ0

	char	MAC[8];			//�豸�����ַ
}NETWORK_STATUS_INFO;

typedef enum _ban_allow_type
{
	BAN_ALLOW_TYPE_BAN,
	BAN_ALLOW_TYPE_ALLOW
}BAN_ALLOW_TYPE;

typedef struct _ip_section
{
	unsigned long enable;		//���ʾ�˶���Ч
	unsigned long start;		//��ʼ��ַ
	unsigned long end;			//������ַ
}IP_SECTION;

//�������
typedef enum _call_back_module
{
	CALL_BACK_MODULE_DISK_MAN = 1,
	CALL_BACK_MODULE_BACKUP_MAN = 2,

}CALL_BACK_MODULE;


typedef enum _call_back_msg
{
	CALL_BACK_MSG_PROGRESS = 1,
	CALL_BACK_MSG_FINISH   = 2,
	CALL_BACK_MSG_FAIL     = 3,
}CALL_BACK_MSG;

//�뱸�����
typedef enum _backup_data_type
{
	NET_BACKUP_DATA_TYPE_DVR,
	NET_BACKUP_DATA_TYPE_AVI,	
}BACKUP_DATA_TYPE;

///////////////////////////////Ӳ�����/////////////////////////////////////////
typedef enum _disk_num_status
{
	NET_DISK_HAVE_READEWRITE_DISK = 0x01,
	NET_DISK_HAVE_REDUN_DISK      = 0x02,
	NET_DISK_HAVE_ONLYREADE_DISK  = 0x04,
	NET_DISK_HAVE_UNFORMATED_DISK = 0x08,
}DISK_NUM_STATUS;

typedef enum _disk_owner
{
	NET_DISK_OWNED_BY_THIS    = 0,
	NET_DISK_OWNED_BY_OTHER_1 = 1,
}DISK_OWNER;

//�豸����
typedef enum _disk_type
{
	NET_DISK_TYPE_FIXEDDISK = 0,     //�̶�¼��Ӳ�̣���֧���Ȱγ�
	NET_DISK_TYPE_REMOVEABLE = 1,    //�Ȱβ��豸
	NET_DISK_TYPE_CDDVD,			 //��������֧���Ȱγ�
}DISK_TYPE;

//�豸�ӿ�����
typedef enum _disk_interface_type
{
	NET_DISK_IDE_INTERFACE = 0,
	NET_DISK_SATA_INTERFACE,
	NET_DISK_USB_INTERFACE,
	NET_DISK_REMDISK_INTERFACE,
	NET_DISK_ESATA_INETERFACE,
	NET_DISK_SD_INTERFACE,
	NET_DISK_CDDVD_INTERFACE,
}DISK_INTERFACE_TYPE;

//¼��Ӳ�̻��õ�:NET_DISK_NORMAL,   NET_DISK_NO_FORMAT
//�ֱ��ʾ��     ����������ʹ�ã���      ��Ӳ��
//�������õ���NET_DISK_NORMAL��NET_DISK_NO_CD
//�ֱ��ʾ��  ������            û�зŹ���
//�Ȱβ��豸���õ���NET_DISK_IS_MOUNT�� NET_DISK_NO_MOUNT
//�ֱ��ʾ��        �ɹ����벢������ʹ�ã�  �ɹ����뵫�ǻ�û��ʹ��
typedef enum _disk_status
{	
	NET_DISK_NORMAL,				//���� 
	NET_DISK_NO_FORMAT,				//δ��ʽ��
	NET_DISK_NO_CD,                 //û�зŹ���
	NET_DISK_IS_MOUNT,              //���Ȱβ��豸��˵����ʾ�Ѿ����أ�������ʹ����
	NET_DISK_NO_MOUNT,              //���Ȱβ��豸��˵��û�й��أ���ʾ�ɹ����뵫�ǻ�û�б�ʹ��
}DISK_STATUS;

typedef enum _disk_property
{
	NET_DISK_READ_WRITE = 0,  //��������д
	NET_DISK_ONLY_READ,   //ֻ�ܶ�������д
	NET_DISK_REDUNDANCY   //����
}DISK_PROPERTY;

//�ýṹ�������ڱ���Ӳ�̵�������Ϣ��
typedef struct _dvr_disk_info
{
	unsigned char   diskOwnedBy;    //�����ڱ�DVR����������DVR
	unsigned char	diskType;		//�������ͣ�USB��CD/DVD��¼��Ӳ�� ��Ӧ��DISK_TYPE
	unsigned char   diskInterface;  //���̽ӿ����ͣ�SATA,IDE,USB, CD/DVD ������DISK_INTERFACE_TYPE
	unsigned char	diskStatus;		//����״̬��no format��normal����������DISK_STATUS��
	unsigned char   diskProperty;   //
    char            diskname[36];   //���̵�������/dev/sda
	char            mountdir[36];   //���̹���Ŀ¼

	unsigned long	totalSpace;		//��������������λΪ M
	unsigned long	freeSpace;		//ʣ��ռ䣬��λΪ M
	unsigned char   partitionNum;   //������
	unsigned long   diskpara;       //��ͬ�豸����������岻ͬ��Ŀǰ�����¼��Ӳ�̣������Ӳ�̵�Normal����ô�������������ǵ�һ�������ĵ�һ������Ŀ¼
	unsigned char   disktemp;       //��ʱ����
	unsigned char	reserved;
}DVR_DISK_INFO;

typedef CMyList<DVR_DISK_INFO> DVR_DISK_INFO_LIST;

//�����豸���е�Ӳ����������ЩDVR
typedef struct _dvr_disk_owner
{
	BYTE  dvrIndex;        //
	BYTE  dvrId[8];        //
	BYTE  dvrPara;         //
	BYTE  diskNum;
}DVR_DISK_OWNER;
typedef CMyList<DVR_DISK_OWNER> DVR_DISK_OWNER_LIST;

typedef struct _net_smart_info_item_
{
	unsigned char  nIndex;            //ID
	unsigned char  nCurrent;          //��ǰֵ
	unsigned char  nWorest;           //�ֵ
	unsigned char  nThreshold;        //��ֵ
	unsigned long  nRawValue;         //����
	unsigned long  nStatus;           //0-ok,1-warning,2-critical
}SMART_INFO_ITEM;

typedef struct _disk_smart_info_
{
	unsigned char   szModelNumber[48];
	unsigned char   szSerialNumber[48];
	unsigned char   szFirmwareRev[48];
	SMART_INFO_ITEM smartInfo[20];
	unsigned long   diskHealth;
}NET_DISK_SMART_INFO;


typedef struct _disk_health
{
	char                szDiskName[48];
	unsigned char       bSleep;
	NET_DISK_SMART_INFO diskSmartInfo;  
}DISK_HEALTH_INFO;
typedef CMyList<DISK_HEALTH_INFO> DISK_HEALTH_INFO_LIST;

//������־ģ���reclogģ��ĳ�ʼ��
typedef struct _disk_log_info
{
	unsigned char diskOwnedBy;
	unsigned char diskProperty;
	unsigned char diskIndex;
}DISK_LOG_INFO;
///////////////////////////////Ӳ�����/////////////////////////////////////////

///////////////////////////////��̨���/////////////////////////////////////////
//��̨Ѳ����
typedef struct _ptz_curise_point
{
	unsigned char   preset;		//Ԥ�õ����
	unsigned char   dwellSpeed;	//��ת�ٶ�
	unsigned short  dwellTime;	//ͣ��ʱ�䣨�룩
}PTZ_CURISE_POINT;

typedef struct _ptz_curise
{
	unsigned short	index;		//���
	unsigned short	presetNum;	//Ԥ�õ���Ŀ
	char			name[36];	//Ѳ���ߵ�����
}PTZ_CURISE;

typedef struct _ptz_preset
{
	unsigned short		index;		//Ԥ�õ����
	unsigned short		enable;		//Ԥ�õ��Ƿ��Ѿ������ã� ����Ϊ�Ѿ�������
	char				name[36];	//Ԥ�õ�����
}PTZ_PRESET;

typedef struct _ptz_ctrl
{
	unsigned long	chnn;			//�ĸ�ͨ��
	unsigned long	speed;			//�����ٶ�
	unsigned long	cmdType;		//��������
	unsigned long	cmdValue;		//���������Ԥ�õ���ţ�Ѳ������ţ��켣��ŵ�
}PTZ_CTRL;

typedef struct _net_ptz_info
{
	unsigned long   chnn;
	unsigned long   presetNum;
	unsigned long   curiseNum;
	unsigned long	curisePointNum;
}NET_PTZ_INFO;

//��̨������Ϣ
typedef struct _ptz_serial_info
{
	unsigned char	baudRate;		//������0-13�ֱ��ʾ110��300����..921600�Ĳ�����
	unsigned char	protocol;	//Э��ID��0Ϊû��������̨
	unsigned char	devAddr;		//��̨�豸ID
	unsigned char	bEnable;		//��ʾ�Ƿ�������̨����
}PTZ_SERIAL_INFO;
///////////////////////////////��̨���/////////////////////////////////////////

//Sensro ����
typedef enum _SENSOR_TYPE
{
	SENSOR_TYPE_NO,	
	SENSOR_TYPE_NC,				
}SENSOR_STATUS;

//�ʼ����� ��������Ϣ�ṹ
typedef struct _mail_send_info
{
	unsigned long	bSSL;
	unsigned long	port;
	char			server[68];
	char			name[68];
	char			passwd[36];
}MAIL_SEND_INFO;

//�ʼ����� ��������Ϣ�ṹ
typedef struct _mail_recv_info 
{
	char recvname[68];
}MAIL_RECV_INFO;
///////////////////////////////�������/////////////////////////////////////////
typedef enum _speed_source_type
{
	SPEED_SOURCE_GPS = 1,	//GPS
	SPEED_SOURCE_CAR		//����
}SPEED_SOURCE_TYPE;

typedef enum _speed_unit_type
{
	SPEED_UNIT_KM_H = 1,	//ǧ��/ʱ
	SPEED_UNIT_MN_H			//����/ʱ
}SPEED_UNIT_TYPE;

typedef struct _mdvr_speed
{
	char speed [12];		//��ʾ�ٶȣ���Ϊ������С�������ؼ��ϲ�������С�������Բ����ַ����İ취��
}MDVR_SPEED;

//��������õĹ���״̬
typedef struct _mdvr_wallop_state
{
	unsigned long	enable;			//�Ƿ�����Ĭ�ϲ�����0:������1:����
	float			threshold;		//������ֵ
	double 			reference;		//��ο�ֵ
}MDVR_WALLOP_STATE;

typedef enum _wifi_ip_mode
{
	WIFI_IP_STATIC,
	WIFI_IP_DECP
}WIFI_IP_MODE;

typedef enum _wifi_auth_mode
{
	WIFI_AUTH_OPEN = 1,
	WIFI_AUTH_SHARE,
	WIFI_AUTH_WPA2_PSK,
	WIFI_AUTH_WPA_PSK
}WIFI_AUTH_MODE;

typedef enum _wifi_encryp_type
{
	WIFI_ENCRYP_TKIP = 1,
	WIFI_ENCRYP_AES,
	WIFI_ENCRYP_NONE,
	WIFI_ENCRYP_WEP,
}WIFI_ENCRYP_TYPE;

//wifi ����  //����һ������

typedef struct _wifi_config
{
	unsigned long	enable;				//�������翪��
	unsigned long	IPMode;				//0	�Ǿ�̬IP  1 ��ʾ��̬IP

	unsigned long	staticIP;			//��������ip
	unsigned long	subMask;			//��������
	unsigned long	gateway;			//
	unsigned long	DNSServer;			//

	unsigned long	channel;			//�ŵ�	0--11 0��ʾ�Զ�

	unsigned long	authMode;			//��֤ģʽ1:OPEN 2:SHARE 3:WPA2-PSK 4:WPA-PSK
	unsigned long	encrypType;			//��������1:TKIP 2:AES 3:NONE 4:WEP 
	unsigned long	defaultKeyID;		//������

	unsigned long	rate;				//Ƶ��
	unsigned long	mode;				//ģʽ

	char			ssid[32];			//SSID

	char			key1[36];			//����1
	//char			key2[36];			//����2

}WIFI_CONFIG;

//�������������
typedef struct mdvr_netserver_cfg
{	
	unsigned long   serverIP;		//������IP
	unsigned long	serverPort;		//�������˿�
}MDVR_NETSERVER_CFG;

//���������в��Ų�������
typedef struct mdvr_dialup_cfg
{
	char	points[132];			//�κŽ����
	char	name[132];				//�û���
	char	password[36];			//����
	char	serviceNumber[36];		//����ҵ���
}MDVR_DIALUP_CFG;

typedef enum _wireless_type
{
	WIRELESS_TYPE_EVDO = 1,
	WIRELESS_TYPE_GPRS
}WIRELESS_TYPE;

typedef enum _wireless_conn_mode
{
	WIRELESS_CONN_LONG = 1,
	WIRELESS_CONN_ACTIVE
}WIRELESS_CONN_MODE;

//������������
typedef struct mdvr_wireless_cfg
{
	unsigned short	enable;			//�Ƿ����������繦�� 1:������0���ر�
	unsigned short 	type;			//����ģ������1:EVDO 2:GPRS
	unsigned short 	mode;			//����ģʽ1:����ģʽ2:����ģʽ
	unsigned short  reserved;		//Ԥ��

	char			number1[16];	//����1
	char			number2[16];	//����2
	char			number3[16];	//����3

	MDVR_DIALUP_CFG	dialup;			//��������
}MDVR_WIRELESS_CFG;

typedef struct _net_wifi_modem
{
	unsigned long	dwChannel;		//�ŵ�	
	char			ssid[32];		//ssid
	unsigned long	dwAuthMode;
	unsigned long   dwEncrypType;
	unsigned long	dwSignal;		//0:���ź� 1:���� 2:�� 3:һ�� 4:�� 5:�ܺ�
	unsigned long	dwSignalUseState; //�ź���û��ʹ�ù� 0:û��ʹ�� 1:����ʹ�� 2:����ʹ��
}NET_WIFI_MODEM;

///////////////////////////////�������/////////////////////////////////////////

//����ö���˹�������������Щ����֧�ֿ��ܲ�֧�ֵĹ��ܡ�
//ע�⣬��������µ��ֻ�ܽ����������ӣ�������ֵ������ֵ�ĵ�����
typedef enum _func_type
{
	FUNC_REMOTE_UPGRADE,			//Զ������
	FUNC_MANUAL_ALARM,				//�ֶ�����
	FUNC_VIEW_VERSION,				//�鿴ϵͳ�汾��Ϣ
	FUNC_VIEW_EVENT,				//�鿴�¼�
	FUNC_VIEW_LOG,					//�鿴��־
	FUNC_VIEW_NETWORK_STATUS,		//�鿴����״̬
	FUNC_VIEW_ONLINE_USER,			//�鿴�����û�
	FUNC_VIEW_DISK_INFO,			//�鿴������Ϣ
	FUNC_DISK_MAN,					//���̹���
	FUNC_DISK_GROUP,				//���̷��鹦��
	FUNC_EXPORT_LOG,				//������־
	FUNC_IMPORT_SETUP,				//��������
	FUNC_EXPROT_SETUP,				//��������
	FUNC_PTZ_CTRL,					//��̨����
	FUNC_TIME_SEARCH,				//��ʱ������
	FUNC_EVENT_SEARCH,				//���¼�����
	FUNC_FILE_MAN,					//�ļ�����
	FUNC_EVICT_SOMEONE,				//������
	FUNC_TALK_WAY,					//�����Խ�
	FUNC_MANUAL_REC,				//�ֶ�¼��Զ�̿��ƣ�
	FUNC_REMOTE_REBOOT,				//Զ������
	FUNC_REMOTE_SHUTDOWN,			//Զ�̹ػ�
	FUNC_SETUP,						//Զ������
	FUNC_BACKUP,					//Զ�̱���
	FUNC_PLAYBACK,					//Զ�̻ط�
}_FUNC_TYPE;

//ע��һ�º��channe����Ϊ32λ��
//��ƷID��������ӵ��豸ȡ��ID��
#define	PRODUCT_ID(channel) (((channel) & 0xffff0000) >> 16)

//�����豸��cameraͨ�������豸�ϵ�ʵ��ͨ��
#define	CAMERA_ID(channel) (((channel) & 0x0000ff00) >> 8)

//�߼�ͨ�������籾��cameraռ��0-7ͨ�����������豸ռ��8-11ͨ��
#define	CHANNEL_ID(channel) ((channel) & 0x000000ff)

//����ͨ����Ŀ������ȫͨ��ģ��
#define ALL_CHNN_MASK(chnnNum) (((ULONGLONG)0x01 << (chnnNum)) - 1)

//��Ч����
#define INVALID_INDEX(type) (static_cast<type>(~(type)0x0))

//������־������
#define LOG_CONTENT_MASK(type) (type & 0xffff0000)

//��ͨ������ѭ������
#define CH_ROL(X, MASK, mask, n) \
	X <<= n; \
	if (0 == (X & MASK)) \
	{ \
		X = mask; \
	}
//��ͨ������ѭ������
#define CH_LOL(X, MASK, mask, n) \
	X >>= n; \
	if (0 == (X & MASK)) \
	{ \
		X = mask; \
	}

#define IP_ADDRESS(ip1, ip2, ip3, ip4) ((ip1) + (ip2<<8) + (ip3<<16) + (ip4<<24))

const unsigned long ONE_DAY_SECOND_NUM	= 86400;
const unsigned long INVALID_CLIENT_ID	= (~0x0);
const unsigned long LOCAL_DEVICE_ID		= 0;
const unsigned long NET_DEVICE_ID		= 0xffff;	//����������ͨ��ʱ�������豸ͳһ�����ID
const unsigned long LOCAL_CLIENT_ID		= 0;
const unsigned long HOLDTIME_ALWAYS		= (~0x0);
const ULONGLONG		MAX_TIME			= ((LONGLONG)0x7fffffff<<32) + (0xffffffff);

//[��(unsigned char)] [�ź���ʽ(unsigned char)] [������(unsigned char)] [ͨ��(unsigned char)]
#define DISPLAY_PACK(LAYER_TYPE, SIGNAL_FORMAT, DISP_INDEX, CHNN_VALUE) (static_cast<unsigned long>(((LAYER_TYPE & 0xFF) << 24) | ((SIGNAL_FORMAT & 0xFF) << 16) | ((DISP_INDEX & 0xFF) << 8) |(CHNN_VALUE & 0xFF)))

//[unsigned long]
#define DISPLAY_GET_LAYER(DISP_VALUE) (static_cast<unsigned short>((DISP_VALUE >> 24) & 0xFF))
#define DISPLAY_GET_SIGNAL(DISP_VALUE) (static_cast<unsigned short>((DISP_VALUE >> 16) & 0xFF))
#define DISPLAY_GET_INDEX(DISP_VALUE) (static_cast<unsigned char>((DISP_VALUE >> 8) & 0xFF))
#define DISPLAY_GET_CHNN(DISP_VALUE) (static_cast<unsigned char>((DISP_VALUE & 0xFF)))



#define SNAP_PIC_IS_ONLYREAD(type)   (0x8000 & (type))
#define SNAP_PIC_EN_ONLYREAD(type)   ((type) |= (0x8000))

#ifndef NEED_VIDEO_ENCODE_INFO
#define NEED_VIDEO_ENCODE_INFO
typedef struct _chn_video_encode_info_
{
	unsigned int	channel;					//ͨ����
	unsigned long	iLength;					//��Ƶ�������ݳ���
	unsigned char	*pVideoEncodeInfo;			//��ǰ�ı�����Ϣ��ַ
	unsigned char	bValid;						//�Ƿ�����޸�
	unsigned char	reserver[7];				//����
}CHN_VIDEO_ENCODE_INFO, *PCHN_VIDEO_ENCODE_INFO;


typedef struct _chn_video_encode_info_detail_
{
	unsigned int	stream;					//0:������ 1:������
	unsigned char	resolution[12];			//��ȡʱ��ʾ��ǰ�ֱ����ַ���������ʱ��ʾ��Ҫ���õķֱ����ַ�����������352*288��Ҫ�н�����
	unsigned long	lBitRateStream[20];		//��ȡʱ��ʾ��ǰ�����������飬����ʱ��ֻ��lBitStream[0]��Ч��������CBR
	unsigned int	rate;					//��ȡʱ��ʾΪ��ǰ�ֱ��ʵ�֡�����ֵ������ʱ��ʾ��Ҫ���õ�֡��
	unsigned int	encodeType;				//��ȡʱ��ʾ��ǰ�ֱ��ʵı�������1����������2������ 3��������/��������֧��
	//����ʱ��ʾ��ǰ�ֱ��ʵı�������1����������2������
	unsigned int	quality;				//��ȡʱ��ʾ��ǰ���������ķ�������������ʱΪ��Χ�ڵ�һ���̶�ֵ��������VBR
	unsigned char	bCrrentUse;				//��ȡʱ��ʾ��ǰ�Ƿ�Ϊ��ǰʹ�ã�����ʱ����������
	unsigned char	reserver[3];
}CHN_VIDEO_ENCODE_INFO_DETAIL, *PCHN_VIDEO_ENCODE_INFO_DETAIL;

#endif

#if defined(__CHIP3520A__) || defined(__CHIP3535__)
#if defined(__ENVIRONMENT_LINUX__) && !defined(__X_LINUX__)
#include "hi_mem.h"
#endif
#endif

#ifdef	__ENVIRONMENT_WIN32__
#pragma pack()
#endif

#endif //_DVRDVSDEFINE_
