/*
2011-05-18 ���ʱ���޸��˵�Э���ں������ӳ� _V3
*/
#ifndef _NET_PROTOCOL_H_IPC_V3_
#define _NET_PROTOCOL_H_IPC_V3_
namespace IPC_V3
{

#include "Typedef.h"
#include "ipcamera_v3.h"

#pragma pack(4)

const int HEARTBEAT_TIMEOUT = 20;		//������ʱʱ��Ϊ20��

typedef struct _pack_cmd
{
	unsigned long cmdType;//�����ţ�ö�ٱ���NET_PROTOCOL�е�ֵ
	unsigned long cmdID;//ָ����ţ�ָ�����趨��һ����ֵ����Ӧ��ָ���ͷ�е�ָ�����������ȫ��ͬ
	//�㲥���û�������״̬�����IDԼ���̶�Ϊ0xffffffff���ڿͻ��˲�������IDʱ������ʹ�����ֵ��
	unsigned long cmdVer;//����汾������ֱ�Ӹ�������汾ȷ���������ĸ�ʽ
	unsigned long dataLen;//���ݳ���
}PACKCMD;

const int PACK_HEAD_LENGTH=sizeof(PACKCMD);
//ռ�����ֽ�
typedef enum _NET_PROTOCOL_H_
{
	//�ͻ��˵�¼
	CMD_BASENUM_LOGIN		= 0x0100,
	CMD_REQUEST_LOGIN,			//�ͻ��������¼,����CMD_REPLY_LONGIN_SUCC��CMD_REPLY_LONGIN_FAIL
	CMD_REQUEST_LOGOUT,			//�ͻ�������ǳ�
	CMD_REQUEST_STREAM_BRIEF,	//���û���¼ǰ��ȡ����������Ҫ��Ϣ�����治���ṹ��
	CMD_END_LOGIN,

	//�������˻ظ���¼��Ϣ
	CMD_BASENUM_REPLY_LOGIN	= 0x01000100,
	CMD_REPLY_LONGIN_SUCC,		//�����¼�������豸��Ϣ
	CMD_REPLY_LONGIN_FAIL,		//�ܾ���¼�����ߵ�¼ʧ��
	CMD_REPLY_STREAM_BRIEF,		//�ظ��û���·��������Ҫ��Ϣ�����ɸ�NET_GET_STREAM_BRIEF�ṹ�壬�м������Ը��ݳ��������
	CMD_END_REPLY_LOGIN,

	//�ͻ��˿��ƣ�ϵͳ����
	CMD_BASENUM_CTRL		= 0x0200,
	CMD_REQUEST_STREAM_START,	//����ʼ�����������縱��������Ƶ��
	CMD_REQUEST_STREAM_STOP,	//����ֹͣ�����������縱��������Ƶ��
	CMD_REQUEST_KEYFRAME,		//����ؼ�֡
	CMD_REQUEST_SHUTDOWN,		//����ػ����ɹ��������κ���Ϣ���豸��Ͽ���ʧ�ܷ��ش�����NET_REPLY_RESULT
	CMD_REQUEST_REBOOT,			//��������ϵͳ���ɹ��������κ���Ϣ���豸��Ͽ���ʧ�ܷ��ش�����NET_REPLY_RESULT
	CMD_REQUEST_CLIENT_INFO,	//�鿴����������DVS�Ͽͻ��ˣ��ظ�CMD_REPLY_CLIENT_INFO
	CMD_REQUEST_CHANGE_TIME,	//�޸��豸ʱ��////////////////////////////////////////
	CMD_REQUEST_SEARCH_WIFI,	//��������·����������CMD_REPLY_SEARCH_WIFI
	CMD_REQUEST_DATA_TO_SERVER,	//�����ݷ���NAT������	
	CMD_REQUEST_CHECK_WIFI,     //���wifi�Ƿ������� panjy
	CMD_REQUEST_NTP_IMMEDIATELY,//����NTPУʱ
	CMD_REQUEST_MAX_VIDEO_LEN,	//���øÿͻ��˿��Է��͵����һ֡��Ƶ�ĳ���
	CMD_END_CTRL,

	//�������˻ظ�����
	CMD_BASENUM_REPLY_CTRL	= 0x01000200,
	CMD_REPLY_CLIENT_INFO,		//���пͻ��˵���Ϣ
	CMD_REPLY_SEARCH_WIFI,		//�����������������б�NET_WIFI_MODEM
	CMD_REPLY_DATA_TO_CLIENT,
	CMD_REPLY_CHECK_WIFI,       //����Ƿ�������wifi
	CMD_END_REPLY_CTRL,

	//�ͻ���ͨ������
	CMD_BASENUM_CHNN_CTRL	= 0x0300,	
	CMD_REQUEST_COLOR_SET,		//����ĳͨ�����ֳ�ͼ�����ɫֵ
	CMD_REQUEST_COLOR_CANCEL,	//��ȡĳͨ�����ֳ�ͼ�����ɫֵ	
	CMD_REQUEST_SET_WHITE_BALANCE,		//���ð�ƽ��
	CMD_REQUEST_CANCE_WHITE_BALANCE,	//ȡ�����õİ�ƽ��
	CMD_REQUEST_SET_DYNANIC_RANGE,		//���ÿ�̬
	CMD_REQUEST_CANCE_DYNAMIC_RANGE,	//ȡ�����õĿ�̬
	CMD_REQUEST_SET_DENOISE,		//���ý���
	CMD_REQUEST_CANCE_DENOISE,	//ȡ�����ý���
	CMD_REQUEST_SET_IRISTYPE,		//�����Զ���Ȧ
	CMD_REQUEST_CANCE_IRISTYPE,	//ȡ�������Զ���Ȧ
	CMD_REQUEST_SET_SHARPEN,		//������
	CMD_REQUEST_CANCE_SHARPEN,	//ȡ��������
	CMD_REQUEST_BAD_PIXEL_CHECK,//����������
	CMD_REQUEST_DAY_NIGHT,      //9314������ҹģʽ panjy
	CMD_REQUEST_SET_IMAGE_MIRROR = 0x0319,	//ͼ���� unsigned char
	CMD_REQUEST_SET_IMAGE_FILP,				//ͼ��ת unsigned char
	CMD_REQUEST_SET_IMAGE_ROTATO,			//ͼ����ת unsigned long
	CMD_REQUEST_SET_AUTOFOCUS,				//�Զ��Խ�
	CMD_REQUEST_SET_IMAGE_DISTORTION,       //����У��
	CMD_END_CHNN_CTRL,

	//�������˻ظ�ͨ����Ϣ
	CMD_BASENUM_REPLY_CHNN_CTRL	= 0x01000300,
	CMD_REPLY_CHNN_CTRL_COLOR,	//�ظ���ĳ��ͨ������ɫֵ
	CMD_REPLY_BAD_PIXEL_CHECK,//����������ظ�
	CMD_END_REPLY_CHNN_CTRL,

	//�ͻ�������
	CMD_BASENUM_CFG			= 0x0400,
	CMD_REQUEST_CFG_DEFAULT,	//�ָ�Ĭ������
	CMD_REQUEST_CFG_NET,		//ͨ���ಥ����������صļ�������
	CMD_REQUEST_CFG_MAIL_TEST,	//����mail����
	CMD_REQUEST_CFG_FTP_TEST,	//����ftp����
	//���������ý��롢���󡢱��桢�뿪����
	CMD_REQUEST_CFG_ENTER,		//�������ã�ÿ��ֻ����һ���ͻ��˽������ã����пͻ�����������ʱ����ʧ��
	CMD_REQUEST_CFG_LEAVE,		//�뿪����
	CMD_REQUEST_CFG_GET,		//��������
	CMD_REQUEST_CFG_SET,		//��������
	CMD_REQUEST_CFG_NET_V2,		//ͨ���ಥ����������صļ�������
	CMD_REQUEST_CFG_ADD_IPHONE_TOKEN, //�ظ��� NET_CTRL_FAIL
	CMD_REQUEST_CFG_DEL_IPHONE_TOKEN, //�ظ��� NET_CTRL_FAIL
	CMD_REQUEST_CFG_SUPPORT_IPHONE_PUSH,//�Ƿ�֧��iphonepush,
	CMD_REQUEST_CFG_EXIST_IPHONE_TOKEN,//�豸���Ƿ��Ѿ������˸�����
	CMD_END_CFG,

	//�������˻ظ�������Ϣ
	CMD_BASENUM_REPLY_CFG		= 0x01000400,
	CMD_REPLY_CFG_SUCC,			//�����������ʱ�ظ��ɹ�����������ҳ�����ݣ���Ӧ��CMD_REQUEST_CFG_ENTER
	CMD_REPLY_CFG_FAIL,			//�ظ����ã�����ʧ���룬��Ӧ��CMD_REQUEST_CFG_ENTER
	CMD_REPLY_CFG_MAIL_TEST,	//����mail�Ľ��
	CMD_REPLY_CFG_FTP_TEST,		//����ftp�Ľ��
	CMD_REPLY_CFG_DATA,			//�ظ���������,��Ӧ��CMD_REQUEST_CFG_GET
	CMD_REPLY_CFG_DEFAULT,		//�ظ�����Ĭ�����������Ӧ��CMD_REQUEST_CFG_DEFAULT
	CMD_REPLY_CFG_ADD_IPHONE_TOKEN,
	CMD_REPLY_CFG_DEL_IPHONE_TOKEN,
	CMD_REPLY_CFG_SUPPORT_IPHONE_PUSH,
	CMD_REPLY_CFG_EXIST_IPHONE_TOKEN,
	CMD_END_REPLY_CFG,

	//SD���������
	CMD_BASENUM_DATA		= 0x0500,
	CMD_REQUEST_DATA_SD_ALL_DATE,		//SD������Щ��������
	CMD_REQUEST_DATA_SD_SEARCH_NAME,	//������ͨ����������SD���е����ݣ�����ͼƬ���ļ���
	CMD_REQUEST_DATA_SD_GET_PIC,		//��ȡָ����ͼƬ	
	CMD_REQUEST_DISK_FORMAT,		//��ʽ��ĳ������
	CMD_REQUEST_DISK_POP_SDCARD,	//����SD��
	CMD_END_DATA,

	//�������˻ظ�����
	CMD_BASENUM_REPLY_SEARCH	= 0x01000500,
	CMD_REPLY_DATA_SD_ALL_DATE,		//SD������Щ��������
	CMD_REPLY_DATA_SD_SEARCH_NAME,	//������ͨ����������SD���е����ݣ�����ͼƬ���ļ���
	CMD_REPLY_DATA_SD_GET_PIC,		//��ȡָ����ͼƬ
	CMD_REPLY_DISK_FAIL,		//�ظ����̹�������ʧ����
	CMD_REPLY_FORMAT_FAIL,		//��ʽ��ʧ��
	CMD_REPLY_FORMAT_PERCENT,	//��ʽ������
	CMD_REPLY_POP_SDCARD_SUCC,	//����SD�ɹ�
	CMD_REPLY_POP_SDCARD_FAIL,	//����SDʧ��
	CMD_END_REPLY_SEARCH,

	//�ͻ�����̨����
	CMD_BASENUM_PTZ			= 0x0600,
	CMD_REQUEST_PTZ_START,		//������̨����
	CMD_REQUEST_PTZ_MOVE,		//��̨��������
	CMD_REQUEST_PTZ_ADJUST,		//��̨preset��cruise����
	CMD_REQUEST_PTZ_STOP,		//������̨����
	CMD_REQUEST_PTZ_WRITECOMM,	//�ѿͻ��˴���������ֱ��д���ڿ�����̨��hebingxieyi
	CMD_END_PTZ,

	//�������ظ���̨Ԥ�õ㡢Ѳ����Ϣ
	CMD_BASENUM_REPLY_PTZ       = 0x01000600,
	CMD_REPLY_PTZ_PRESET_INFO,      //-----Ӧ����̨Ԥ�õ���Ϣ���ѷϳ�
	CMD_REPLY_PTZ_START_SUCC,       //����ʼ��̨���سɹ�
	CMD_REPLY_PTZ_START_FAIL,       //����ʼ��̨����ʧ�ܣ���������ԭ��
	CMD_END_REPLY_PTZ,

	//�Խ��������
	CMD_BASENUM_TALK = 0x0700,
	CMD_REQUEST_TALK_START,		    //����ʼ1v1�Խ�
	CMD_REQUEST_TALK_STREAM,		//�ͻ��˸�DVS���ͶԽ�����Ƶ����
	CMD_REQUEST_TALK_STOP,			//ֹͣ1v1�Խ�
	CMD_END_TALK,             

	//����������ػظ�
	CMD_BASENUM_REPLYY_TALK = 0x01000700,
	CMD_REPLY_TALK_START_SUCC,		    //�ظ�����ʼ1v1�Խ��ɹ�    
	CMD_REPLY_TALK_START_FAIL,		    //�ظ�����ʼ1v1�Խ�ʧ��    
	CMD_END_REPLY_TALK, 

	//�ͻ���ϵͳ�ļ���������
	CMD_BASENUM_SYSTEM_UPDATE		= 0x0800,
	CMD_REQUEST_SYSTEM_UPDATE_DATA,		//ϵͳ�ļ�����������	
	CMD_REQUEST_UBOOT_UPDATE_DATA,		//����uboot�ļ�
	CMD_END_SYSTEM_UPDATE,

	//�������˻ظ��ں�����
	CMD_BASENUM_REPLY_SYSTEM_UPDATE	= 0x01000800,
	CMD_REPLY_SYSTEM_UPDATE_FAIL,		//�ظ�ϵͳ�ļ�����ʧ����Ϣ
	CMD_REPLY_SYSTEM_UPDATE_PERCENT,	//�ظ�ϵͳ�ļ���������
	CMD_END_REPLY_SYSTEM_UPDATE,

	//�ͻ�����������
	CMD_BASENUM_UPDATE		= 0x0a00,
	CMD_REQUEST_UPDATE_START,	//�����������
	CMD_REQUEST_UPDATE_DATA,	//�������������
	CMD_REQUEST_UPDATE_END,		//��������
	CMD_END_UPDATE,

	//�������˻ظ�����
	CMD_BASENUM_REPLY_UPDATE	= 0x01000a00,
	CMD_REPLY_CAN_UPDATE,		//�ظ���ʾ��������
	CMD_REPLY_NOT_UPDATE,		//�ظ���ʾ��������
	CMD_REPLY_UPDATE_FAIL,		//�ظ�����ʧ����Ϣ
	CMD_REPLY_UPDATE_PERCENT,	//�ظ���������
	CMD_END_REPLY_UPDATE,

	//�������˷���״̬��Ϣ
	CMD_BASENUM_REPLY_STATUS	= 0x01000b00,
	CMD_REPLY_MOTION,			//����motion���״̬
	CMD_REPLY_SENSOR,			//����sensor���״̬	
	CMD_REPLY_CHNN_NAME,
	CMD_REPLY_PTZ_PRESET_NAME,	//����ĳ��ͨ��������Ԥ�õ�����
	CMD_REPLY_PTZ_CURISE_NAME,	//����ĳ��ͨ��������Ѳ��������
	CMD_REPLY_PTZ,				//��̨�Ƿ�������Э��
	CMD_REPLY_SD_INFO,			//����SD������Ϣ
	CMD_END_REPLY_STATUS,
	
	//�������˷�����Ƶ/��Ƶ����
	CMD_BASENUM_REPLY_STREAM	= 0x01000c00,	
	CMD_REPLY_LIVE_STREAM,		//�ֳ�����
	CMD_REPLY_PLAY_STREAM,		//�ط�����
	CMD_REPLY_TALK_STREAM,
	CMD_END_REPLY_STREAM,
}NET_PROTOCOL;

typedef enum _net_ctrl_fail
{
	NETERR_SUCCESS=0,				//�ɹ������еط��ĳɹ�������ʹ�ô�ֵ
	NETERR_FAIL=0xffffffff,			//��ͨʧ�ܷ���ֵ���ڲ����ԭ�������·��ش�ֵ
	NETERR_LOGIN_FAIL_VERSION_ERR=1,//��½ʱ�汾����
	NETERR_LOGIN_FAIL_USERPWD_ERR=2,//��½ʱ�û������������
	NETERR_LOGIN_FAIL_OVERFLOW	 =3,//��½���糬�أ������½����һ��������ֹͣlisten,�˴�������ò���
	NETERR_LOGIN_FAIL_REFUSE	 =4,//��½�ܾ��������Ǳ����˵�IP��ԭ��
	NETERR_OPERATE_FAIL_NOAUTHORITY=5,//������û��Ȩ�ޣ������á���̨���ط�
	NETERR_OPERATE_FAIL_OPERATING  =6,//ֻ��һ���˲���ʱ�����Ѿ������ڲ��������á���̨���ơ��ط�
	NETERR_NO_DISK	=7,				//�޴��̣��ڸ�ʽ������ѯ�¼��Ⱥʹ�����صĲ���ʱ��Ϊû�д��̶�ʧ��ʱ���ش�ֵ
	NETERR_NO_EVENT = 8,			//���¼�
	NETERR_FORMATERR_RECORDING	=9,	//���̸�ʽ����������¼��
	NETERR_NOTSUPPORT=10,			//��֧�ֵĲ���
	NETERR_MUST_ENTER_CFG=11,		//��û�н������õ�����½�����õ����������õĲ���ʱ���صĴ�����
	NETERR_DISK_IO_ERR=12,			//���̶�д��IO����
	NETERR_LOGIN_FAIL_MACERR	 =13,//MAC��ַ��ƥ�䣬�������Թ鵽NETERR_LOGIN_FAIL_REFUSE
	NETERR_LOGIN_FAIL_UNSUPPORT_CLIENT=14,//��½ʧ�ܣ�ԭʼ�汾ֻ֧��CMS��½
	NETERR_ADD_IPHONE_TOKEN_DUPLICATE=15, //IPHONE�������ظ���
	NETERR_DEL_IPHONE_TOKEN_NONE=16,			//û����ӹ�������
	NETERR_NO_EXIST_IPHONE_TOKEN = 17,
	NETERR_EXIST_IPHONE_TOKEN = 18,
}NET_CTRL_FAIL;

const unsigned long NET_PROTOCOL_VER	= 0x00000003;

struct  NET_GET_STREAM_BRIEF
{
	char	videostream[320];	
};

//�û���¼ʱ�õĽṹ��
typedef struct _net_login_info
{
	unsigned long	connectType;			//��������

	char			username[32];			//�û�����
	char			password[32];			//�û�����
	char			computerName[28];		//���������

	unsigned char   IP[8];					//�ͻ����������ӵ�IP��ַ,����ʹIPV6,��һλ��ʾIP��λ��4��6
	unsigned char   MAC[6];					//����Mac��ַ
	unsigned char	resv[2];//�����ֽڣ�������
	unsigned long	netprotocolver;			//�ͻ���Ŀǰʹ�õ�����Э��汾
}NET_LOGIN_INFO, *PNET_LOGIN_INFO;

typedef enum _enum_connect_type{
	CONNECTTYPE_CTRL_CENTER,
	CONNECTTYPE_MOBILE_PHONE,
	CONNECTTYPE_IE
}ENUM_CONNECT_TYPE;

//�û���¼�ɹ�ʱ���ظ��õĽṹ��
struct NET_LOGIN_SUCCESS_REPLY
{	
	unsigned long	ConfigDataLen;				//���ýṹ����

	unsigned long	PTZPresetNum;				//Ԥ�õ����
	unsigned long	PTZCruiseNum;				//Ѳ���߸���
	unsigned long   PTZPresetNumForCruise;		//Ѳ����֧�ֵ����Ԥ�õ����

	//Ԥ�õ�������󳤶ȣ�ÿ����Ʒ���ܲ�ͬ���ڱ༭�������,Ԥ�õ㱣��ͻ�ö���ͨ�����������ֻ�ܷŵ�����
	unsigned long	PresetNameMaxLen;

	//Ѳ��������󳤶ȣ�ÿ����Ʒ���ܲ�ͬ���ڱ༭�������,Ѳ�α���ͻ�ö���ͨ�����������ֻ�ܷŵ�����
	unsigned long	CruiseNameMaxLen;

	unsigned char	bSupportPTZ;				//�Ƿ�֧��PTZ����			1 ֧��  0 ��֧��
	unsigned char	videoFormat;				//��ǰ��ʽ   0 VIDEO_FORMAT_PAL  1 VIDEO_FORMAT_NTSC
	unsigned char	sensorInNum;				//Sensor In ����
	unsigned char	alarmOutNum ;				//�����������

	unsigned char	ucStreamCount;				//��Ƶ�м�·��	
	unsigned char	bSupportSnap;			//��Ʒ�Ƿ�֧��SD��ץͼ����  1 ֧��  0 ��֧��
	unsigned char	noused;
	unsigned char	ucLiveAudioStream;			//�Ƿ�����Ƶ���ֳ���	1 �У�0 û��

	unsigned char	ucTalkAudioStream;			//�Ƿ�����Ƶ�Խ�		1 �У�0 û��
	//IPCAMERA_AUDIO_ENCODE_TYPE_RAW   IPCAMERA_AUDIO_ENCODE_TYPE_G726
	unsigned char	audioEncodeType;			//��Ƶ�����ʽ	
	unsigned char	audioBitWidth;				//��Ƶλ��
	unsigned char	audioChannel;				//��ͨ��
	unsigned long	dwAudioSample;				//��Ƶ�Ĳ�����

	//USER_GROUP_NONE		USER_GROUP_ADMIN	USER_GROUP_ADVANCE	USER_GROUP_NORMAL
	unsigned long	UserRight;					//�û�����Ȩ�޻����û�����	
	unsigned long	softwareVer;				//Server������汾
	unsigned long	buildDate;					//Server�������������

	unsigned char	MAC[6];						//ipcamera�����������ַ
	char	        deviceName[34];				//�豸����

	unsigned long	nCustomerID;			//�ͻ��ţ����ݴ�ȷ���ͻ��˵Ŀͻ��������ԣ�0Ϊ����

	//�⼸����ɫ�Ǹ�NVR������
	unsigned char	defBrightness;
	unsigned char	defContrast;
	unsigned char 	defHue;
	unsigned char	defSaturation;

	unsigned char noused1[100];		//�Ժ��ܿ��ô�δ���ֽ�
};

//CMD_REQUEST_STREAM_START	CMD_REQUEST_STREAM_STOP
struct NET_REQUEST_STREAM_CTRL
{
	unsigned char	ucStreamType;	//�����ͣ���Ƶ ���� ͼƬ
	unsigned char	ucStreamIndex;	// ����ţ���0��ʼ
	unsigned char	noused[2];
};

//	CMD_REQUEST_KEYFRAME
struct NET_REQUEST_I_FRAME
{
	unsigned char	ucStreamIndex;	// ����ţ���0��ʼ
	unsigned char	noused[3];
};

typedef struct _net_chnn_color
{
	unsigned long brightness;
	unsigned long contrast;
	unsigned long saturation;
	unsigned long hue;
}NET_CHNN_COLOR;

typedef struct _net_chnn_white_balance
{
	unsigned char ucMode;// 0 �Զ� 1 �ֶ� 2 ���� 3 ����
	unsigned int red;	//��ɫ����
	unsigned int blue;	//��ɫ����	
}NET_CHNN_WHITE_BALANCE;

typedef struct _net_chnn_dynamic_range
{
	unsigned char ucChn;
	unsigned char ucMode;// 0 �ر� 1 ����
	unsigned char ucValue;
	unsigned char unused;
}NET_CHNN_DYNAMIC_RANGE;

typedef struct _net_denoise
{
	unsigned char ucMode;// 0 �ر� 1 ����
	unsigned char value;
	unsigned char unused[2];
}NET_DENOISE;

typedef struct _net_iristype
{
	unsigned char ucMode;// 0 �ر� 1 ����
	unsigned char unused[3];
}NET_IRISTYPE;

typedef struct _net_sharpen
{
	unsigned char ucMode;// 0 �ر� 1 ����
	unsigned char value;
	unsigned char unused[2];
}NET_SHARPEN;

typedef struct _net_day_night //panjy
{
	unsigned char ucMode; //color b/w auto
	unsigned char ucDelay; //autoģʽ�µ��ӳ�ʱ�� 0-255, default 4
	unsigned char ucDayTh; //����ֵ 0-255 default 5
	unsigned char ucNightTh; //0 - 255 default 3
}NET_DAY_NIGHT;

typedef struct _net_chnn_name
{
	unsigned long	nameLen;
	char			name [36];
}NET_CHNN_NAME;

typedef struct _net_cfg_info
{
	unsigned long	len;			//�������ݵĳ���
	unsigned long	cfgType;		//��������
}NET_CFG_INFO;

typedef struct _net_data_search
{
	unsigned long		searchType;	//��������
	unsigned long		streamID;	//Ҫ���͵�stream ID
	ULONGLONG       	videoCH;	//��Ƶͨ������
	ULONGLONG           audioCH;    //��Ƶͨ������
	LONGLONG			startTime;	//��ʼʱ��
	LONGLONG			endTime;	//����ʱ��

	unsigned long		dataType; //�������� �����ڻطŻ��Ǳ��ݣ�NET_DATA_SEARCH_TYPE
	unsigned char		RecordTypeMASK;//¼���������
	unsigned char		reserveData[3];//�����4BYTE
}NET_DATA_SEARCH;

typedef struct _net_data_info
{
	unsigned long	dataType;		//��������
	unsigned long	end;			//1 ��ʾ���һ��
	unsigned long   index;          //����
	unsigned long	dataNum;		//���ݸ���
}NET_DATA_INFO;

typedef struct _net_ptz_ctrl
{
	unsigned long	speed;			//�����ٶ�
	unsigned long	cmdType;		//��������
	unsigned long	cmdPara;		//�������
}NET_PTZ_CTRL;

typedef struct _net_ptz_info
{
	unsigned long   presetNum;
	unsigned long   presetNameLen;
	unsigned long   curiseNum;
	unsigned long   curiseNameLen;
	unsigned long	curisePointNum;
}NET_PTZ_INFO;

typedef struct _net_status
{
	unsigned long chnn;		
	unsigned long status;//��λ��ʾ��ͨ����״̬
}NET_STATUS;

typedef struct _net_reply
{
	DWORD	dwResult;
}
NET_REPLY_RESULT;

typedef struct _net_reply_ex
{
	DWORD dwResult;
	char	szMessage[1020];
}
NET_REPLY_RESULT_EX;

//dwOtherStatus
const unsigned long STATUS_CONFIG_BASE = 0x00000001;	// �������û�������
const unsigned long STATUS_TALKING = 0x00000002;		// ���ڶԽ�
const unsigned long STATUS_CONFIG_PTZ = 0x00000004;		// ����������̨
const unsigned long STATUS_OPERATE_SD = 0x00000008;		// ���ڲ���SD��
const unsigned long STATUS_UPDATING = 0x00000010;		// ��������

typedef struct _net_echo_client_info
{
	unsigned long	dwIP;				//���ӵĿͻ��˵�IP��ַ

	unsigned short	wPort;				//�ͻ���Զ�̻��Ķ˿ںţ������ֽ���
	unsigned short	Unused;				

	//	char			szUserName[DVS_USER_NAME_LEN];	//�û���
	char			szUserName[16];	//�û���
	unsigned long	dwUserType;	
	unsigned long	dwOtherStatus;		//	STATUS_CONFIG_BASE  STATUS_TALKING  STATUS_CONFIG_PTZ 
}NET_ECHO_CLIENT_INFO;

//CMD_REQUEST_CLIENT_INFO	 CMD_REPLY_ALL_CLIENT_INFO
typedef struct _net_all_client_info
{
	unsigned long			dwCount;
	//	NET_ECHO_CLIENT_INFO	clientInfo[MAX_CLIENT_LINK];
	NET_ECHO_CLIENT_INFO	clientInfo[8];
}NET_ALL_CLIENT_INFO;

typedef struct _net_sdcard_info
{
	unsigned long	hasSDCard;	//0 û��SD����1 ��SD��,����δ��ʼ�� 2 ��SD����������ʹ��
	unsigned long	capability; //SD��������������MByteΪ��λ
	unsigned long	usedRec;	//ʹ���˵�¼��ռ䣬��MByteΪ��λ
	unsigned long	spareRec;	//ʣ���¼��ռ䣬��MByteΪ��λ
	unsigned long	usedPic;	//ʹ���˵�ͼƬ�ռ䣬��MByteΪ��λ
	unsigned long	sparePic;	//ʣ���ͼƬ�ռ䣬��MByteΪ��λ
}NET_SDCARD_INFO;

//CMD_REQUEST_PTZ_WRITECOMM
typedef struct _net_write_comm
{
	unsigned char			byChnn;
	unsigned char			Unused[3];
	unsigned long			dwLen;		//���������4�������������ֻ��������
	unsigned char			szContent[1];		//�������ʹ��4�������������ֻ��������
}NET_WRITE_COMM;

//CMD_REQUEST_CHANGE_TIME
typedef struct _net_change_time
{		
	LONGLONG		llTime;
}NET_CHANGE_TIME;

typedef struct _net_automail_info
{
	char	szDestMail[64];				//Ŀ������ĵ�ַ ���磺aaa@hotmail.com
	char	szSrcMail[64];				//��������ĵ�ַ ���磺bbb@163.com
	char	szUserName[64];				//�����û���
	char	szMailPwd[64];				//��������
	char	szMailServer[64];			//�����������ַ ���� smtp.163.com
	unsigned char ucSSL;
	char	noUsed;
	unsigned short nPort;
}NET_AUTOMAIL_INFO;

typedef struct _net_ftp_info
{
	char		szHostName[64];			//��������ַ��IP
	unsigned short  nPort;				//FTP����˿ں�,һ��Ϊ21
	char		szUnused[2];
	char		szFtpUserName[64];		//FTP�û���
	char		szFtpPasswd[64];		//FTP�û�����
	char		szPath[64];
}NET_FTP_INFO;

typedef struct _net_cfg_page_info
{
	DWORD  pageIndex;  //////////��Ӧ��ҳ������Ϣ
}NET_CFG_PAGE_INFO;

typedef struct _net_pic_name
{
	char	name[40];
}NET_PIC_NAME;

typedef struct _net_manual_rec_info
{
	unsigned long bStartRec; //0 �ر��ֶ�¼�񣬷�0�����ֶ�¼��
	unsigned long dwChannel; //�����ر��ֶ�¼���ͨ����
}NET_MANUAL_REC_INFO;

typedef struct _net_iphone_push
{
        unsigned long ifsupport;//0��֧��iphonepush,    1֧��
        unsigned long type;//0λ��ʾ�ƶ����,��1λ��ʾ������

}NET_IPHONE_PUSH;

#pragma pack()

}

#endif //_NET_PROTOCOL_H_
