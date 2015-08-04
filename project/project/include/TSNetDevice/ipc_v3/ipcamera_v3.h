#ifndef _DVRDVSDEFINE_
#define _DVRDVSDEFINE_
#include "Typedef.h"

#pragma pack(4)


const char * const		DEFAULT_MAIL_SUBJECT = "AUTOMAIL IP NOTICE";	//Ĭ��AUTOMAIL���ʼ�����
const char * const		DEFAULT_MAIL_TEST_SUBJECT = "AUTOMAIL TEST MAIL";	
const char * const 		DEFAULT_MAIL_TEST_CONTENT = "Automail test";
const char * const		VLOSS_ALARM_MAIL_SUBJECT = "VIDEO LOSS ALARM NOTICE";
const char * const		MOTION_ALARM_MAIL_SUBJECT = "MOTION ALARM NOTICE";
const char * const		SENSOR_ALARM_MAIL_SUBJECT = "SENSOR ALARM NOTICE";

//���ֱ���ʱ��ģ�"����"�Ķ��壬��ǰ����0��0xffffffff����ֵ������ͳһ
const unsigned long HOLDTIME_ALWAYS=0xffffffff;
//�豸�û�����������󳤶ȣ�ĿǰDVR DVS �ͻ��� CMS���� �������������붼���ô�ֵ����Ҫ���Ĵ�ֵ,Ҫ���ľ���ͳһ����
const unsigned long  DEV_USER_NAME_LEN      = 16;
const unsigned long  DEV_PASSWORD_LEN       = 16;

enum IPCAMERA_ENCODE_LEVEL
{
	IPCAMERA_BASELINE,
	IPCAMERA_MAINPROFILE,
	IPCAMERA_HIGHPROFILE
};
//��Ƶ��ʽ����
typedef enum _video_format
{
	VIDEO_FORMAT_NTSC	= 0x00,
	VIDEO_FORMAT_PAL	= 0x01
}VIDEO_FORMAT;

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
	DDNS_MYQSEE,			 //ר����US02��DDNS,����MINTDNS,������www.myq-see.com
	DDNS_EASTERNDNS,		//ר����US09��DDNS,����MINTDNS,������www.easterndns.com
	DDNS_3322				//ϣ��������www.3322.org
}DDNS_SERVER_TYPE;

typedef enum _dvr_video_size
{
	DVRVIDEO_SIZE_NONE	= 0x0000,	//NONE
	DVRVIDEO_SIZE_QCIF	= 0x0001,	//QCIF
	DVRVIDEO_SIZE_CIF	= 0x0002,	//CIF
	DVRVIDEO_SIZE_HD1	= 0x0003,	//HD1
	DVRVIDEO_SIZE_D1	= 0x0004,	//D1

	DVRVIDEO_SIZE_QQVGA	= 0x0005,	//QQVGA 
	DVRVIDEO_SIZE_QVGA	= 0x0006,	//QVGA
	DVRVIDEO_SIZE_VGA	= 0x0007,	//VGA
	DVRVIDEO_SIZE_XVGA	= 0x0008,	//XVGA 1280*960

	DVRVIDEO_SIZE_720P	= 0x0009,  	//1280*720
	DVRVIDEO_SIZE_1080P = 0x000A,	//1920*1080
	
	DVRVIDEO_SIZE_UXVGA = 0x000B,	//UXVGA 1600*1200

	DVRVIDEO_SIZE_NVR_INNER = 0x000C,//  NVR inner use 640*480
	DVRVIDEO_SIZE_14M = 0x000D,      //   1280*1024 1.4M
	DVRVIDEO_SIZE_QXGA  = 0x000E,    // 2048*1536
	DVRVIDEO_SIZE_8M  = 0x000F,    // 3264*2448

	DVRVIDEO_SIZE_16_9_3M = 0x0010,  //  2304*1296
	DVRVIDEO_SIZE_5M = 0x0011, //  2592*1944
	DVRVIDEO_SIZE_4M = 0x0012, //  2688*1520
	DVRVIDEO_SIZE_2K = 0x0013, //  2560*1440

}DVR_VIDEO_SIZE;

const ULONGLONG MAX_TIME		= ((LONGLONG)0x7fffffff<<32) + (0xffffffff);

typedef struct _mac
{
	unsigned char mac [8];
}MAC;


//ucFrameType
const char	IPCAMERA_FRAME_TYPE_VIDEO	= 0;
const char	IPCAMERA_FRAME_TYPE_AUDIO	= 1;
const char	IPCAMERA_FRAME_TYPE_PIC	= 2;

//ucEncodeType
const char	IPCAMERA_VIDEO_ENCODE_TYPE_H264 = 0;
const char	IPCAMERA_VIDEO_ENCODE_TYPE_MGP4 = 1;
const char	IPCAMERA_AUDIO_ENCODE_TYPE_RAW = 32;
const char	IPCAMERA_AUDIO_ENCODE_TYPE_G726 = 33;
const char	IPCAMERA_AUDIO_ENCODE_TYPE_G711 = 34;
const char	IPCAMERA_AUDIO_ENCODE_TYPE_ADPCM = 35;
const char	IPCAMERA_PIC_ENCODE_TYPE_JPG = 64;
const char	IPCAMERA_PIC_ENCODE_TYPE_BMP = 65;

struct IPCAMERA_FRAME_INFO
{
	//����  ��Ƶ  ͼƬ    IPCAMERA_FRAME_TYPE_LIVE_VIDEO
	unsigned char		ucFrameType;
	//���뷽ʽ����������Ƶ��ͼƬ�Ķ���һ��
	unsigned char		ucEncodeType;
	//����ţ���0��ʼ
	unsigned char		ucStreamIndex;
	//0 ���ǹؼ�֡  1 �ǹؼ�֡
	unsigned char		ucKeyFrame;	
	
	//֡��ţ�һֱ�������ӵ�����ٴ�0��ʼ����
	unsigned long		frameIndex;

	unsigned long       width;
	unsigned long       height;
	
	//IPCamera�ı���ʱ�䣨��IPCamera����ĳʱ����ʱ���룩���ͻ����ô���UTCʱ��ĺ��������õ���ʱ����ֱ����ʾ��
	LONGLONG			time;	  

	//���ʱ�䣬�ڸı��豸ʱ��ʱ����仯����һֱ������ʱ��
	LONGLONG			relativeTime;

	unsigned long		length;
	unsigned char      *pData;
};	//���ڱ���ʹ��

//��¼�����յĽṹ��
typedef struct _dvr_date
{
	unsigned short year;
	unsigned char  month;
	unsigned char  mday;
}DVR_DATE;
//��ʾ�����յ�ģʽ
typedef enum _date_mode
{
	DATE_MODE_YMD	= 0x01,		//������ģʽ
	DATE_MODE_MDY	= 0x02,		//������ģʽ
	DATE_MODE_DMY	= 0x03		//������ģʽ
}DATE_MODE;

//��ʾλ����ͨ�����ơ��ֳ�ʱ�䡢ʱ�����ͼ���е�λ��
typedef enum _disp_pos
{
	DISP_TOP_RIGHT		= 1,	//����
	DISP_TOP_LEFT		= 2,	//����
	DISP_BOTTOM_RIGHT	= 3,	//����
	DISP_BOTTOM_LEFT	= 4,	//����
	DISP_CUSTOM_DEFINE	= 32,	//�Զ���
}DISP_POS;

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
typedef struct _n_holiday_schedule
{
	unsigned char		month;
	unsigned char		day;
	unsigned short		year;	
	DATE_SCHEDULE		date;  //��һ����ļƻ���24Сʱ��ÿ��Сʱÿһ������һ��bit��ʾ
}N_HOLIDAY_SCHEDULE;

const unsigned long MAX_MAIL_RECV_CNT         = 8;

typedef struct _email_recv_item
{	
	unsigned long  dwIndex;
	char  szRecvAddr[64];
}EMAIL_RECV_ITEM;

typedef struct _email_info
{
	unsigned long	authMethod;
	unsigned short	nPort;
	unsigned char	chSSL;	//SSL_MODE_NONE ����ҪSSL  SSL_MODE_SSL SSL  SSL_MODE_TLS TLS
	unsigned char	noused[1];
	char server		[132];
	char address	[132];
	char userName	[132];
	char password	[36];
	EMAIL_RECV_ITEM stRecvList[MAX_MAIL_RECV_CNT];
}EMAIL_INFO;

const unsigned char	SSL_MODE_NONE = 0;	
const unsigned char	SSL_MODE_SSL = 1;
const unsigned char	SSL_MODE_TLS = 2;

enum VIDEO_ENCODE_MODE
{
	VIDEO_ENCODE_MODE_VBR	=0x01,//������
	VIDEO_ENCODE_MODE_CBR =0x02//�̶�����
};
//�����û�Ȩ��
typedef enum _user_group_
{
	USER_GROUP_NONE		= 0x00,
	USER_GROUP_ADMIN	= 0x01,
	USER_GROUP_ADVANCE	= 0x02,
	USER_GROUP_NORMAL	= 0x04
}USER_GROUP;

typedef enum _getip_mode
{
	GETIP_MODE_STATICIP = 0,	//��̬IP	
	GETIP_MODE_DHCP		= 1,	//��̬IP
}
GETIP_MODE;

typedef enum _image_quality
{
	IMAGE_QUALITY_LOWEST	= 0x01,
	IMAGE_QUALITY_LOWER		= 0x02,
	IMAGE_QUALITY_MEDIUM	= 0x03,
	IMAGE_QUALITY_HEIGHTER	= 0x04,
	IMAGE_QUALITY_HEIGHTEST	= 0x05
}IMAGE_QUALITY;

typedef struct _net_wifi_modem
{
	unsigned long	dwChannel;
	char			ssid[36];//��󳤶�32
	unsigned long	dwAuthMode;
	unsigned long   dwEncrypType;
	unsigned long	dwSignal;		// 0 ���ź� 1 ���� 2 ��  3 һ�� 4 ��  5 �ܺ�
}NET_WIFI_MODEM;

const unsigned long		SMTP_SUBJECT_MAX_LEN		= 64;	//�ʼ��������󳤶�
const unsigned long		SMTP_CONTENT_MAX_LEN		= 256;	//�ʼ����ݵ���󳤶�

const unsigned long     FTP_SERVER_NAME_MAX_LEN      = 32;   //FTP���������ֵ���󳤶�
const unsigned long     FTP_SERVER_HOST_NAME_MAX_LEN = 128;   //FTP��������ַ����󳤶�
const unsigned long     FTP_USERNAME_MAX_LEN    = 64;   //FTP�û�������󳤶�
const unsigned long     FTP_PASSWD_MAX_LEN      = 64;   //FTP�û��������󳤶�
const unsigned long		FTP_PATH_LEN			= 128;	//FTP����ļ���·��

typedef struct _ftp_server
{
	unsigned long	dwServerID;									//����Ψһ��ʾ����һ��FTP������ 0 ��ʾû������
	char			szServerName[FTP_SERVER_NAME_MAX_LEN+4];	//FTP���������֣��û������Լ��üǾ���
	char			szHostName[FTP_SERVER_HOST_NAME_MAX_LEN+4];	//��������ַ��Ҳ������IP��ַ
	unsigned short  nPort;										//FTP����˿ں�,һ��Ϊ21
	char			szUnused[2];
	char			szFtpUserName[FTP_USERNAME_MAX_LEN+4];		//FTP�û���
	char			szFtpPasswd[FTP_PASSWD_MAX_LEN+4];			//FTP�û�����
	char			szPath[FTP_PATH_LEN+4];						//����ļ���Ŀ¼
}FTP_SERVER;

const BYTE PRESET_NAME_MAX_LEN =32;

//Ԥ�õ���Ϣ��ֻ����һ��ͨ����
typedef struct _ptz_preset_info
{
	BYTE preseIndex;//Ԥ�õ�λ�����
	BYTE noused[3];//������
	char  name[PRESET_NAME_MAX_LEN+4]; //Ԥ�õ�����
}
PRESET_INFO;

const BYTE CRUISE_NAME_MAX_LEN =32;
//Ѳ������Ϣͷ��ֻ����һ��ͨ����
typedef struct _ptz_cruise_info_head
{
	unsigned long cruiseFlag;//��ʼֹͣ��� 
	unsigned long cruisepointnum;//Ѳ��������
	unsigned long cruiseIndex;//Ѳ�������
	char  name[CRUISE_NAME_MAX_LEN+4];
}
CRUISE_INFO_HEAD;

typedef	struct _ptz_cruise_point
{
	unsigned char	cruisePoint;	//Ԥ�õ�
	unsigned char	cruiseSpeed;	//Ѳ��ʱ��ÿ��Ԥ֧����ٶ�(1-8)
	unsigned short	cruiseStopTime;	//Ѳ��·����ÿ����Ԥ�õ��ͣ��ʱ��(1-999��)
}PTZ_CRUISE_POINT;

typedef enum _camera_freq_define
{
	ENUM_CAMERA_FREQ_50HZ=0,
	ENUM_CAMERA_FREQ_60HZ=1,
	NCFG_ENUM_CAMERA_FREQ_OUTDOOR=2
}NCFG_ENUM_CAMERA_FREQ;//

#pragma pack()

#endif
