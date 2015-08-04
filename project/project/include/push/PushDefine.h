/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : 
** Date         : 2013-09-10
** Version      : 1.0
** Description  : ��Ϣ����
** Modify Record:
1:
***********************************************************************/
#ifndef __PUSH_DEFINE_H__
#define __PUSH_DEFINE_H__

#include "dvrdvsdef.h"
#include "PUB_common.h"

#pragma pack(4)

const unsigned long		IPHONE_TOKEN_LEN = 32;
const unsigned short	PNTS_SERVER_PORT = 6036;
const unsigned long		MAX_MSG_LEN = 50;
const unsigned long		MAX_IP_LEN = 25;
const unsigned long     MAX_PROVIDER_ID_LEN = 20; //������Ϊ30���������ʵ���������

const char DVR_PUSH_VERSION[8] = "1.0.0";

/////������������
typedef enum __push_data_type__
{
	DATA_TYPE_ALARM		= 0,		//������Ϣ
	DATA_TYPE_OPERATE	= 1,		//������Ϣ
	DATA_TYPE_FILE		= 2,		//�ļ�
}PUSH_DATA_TYPE;


//���ͱ������Ͷ���
typedef enum __push_alarm_type__
{
	PUSH_ALARM_VIDEOLOSS,		//��Ƶ��ʧ����
	PUSH_ALARM_MOTION,			//��̬��ⱨ��
	PUSH_ALARM_SENSOR,			//����������
	PUSH_ALARM_DISKFULL,		//����������
	PUSH_ALARM_DISKWARNING,		//����˥������
}PUSH_ARARM_TYPE;

//�����¼����Ͷ���
typedef enum __push_operate_type__
{
	PUSH_OPERATE_LOGIN,			//��½
	PUSH_OPERATE_FORMAT_DISK,	//��ʽ��Ӳ��
	PUSH_OPERATE_DELETE_FILE,	//ɾ�ļ�
	PUSH_OPERATE_CHANGE_PASS,	//�޸�����
	PUSH_OPERATE_BACKUP,		//����
}PUSH_OPERATE_TYPE;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////DVR���ֻ��ͻ��˺�PNTS/////////////////////////////////////////
/*
�������͹��ܵ��ֻ��ͻ���ID
*/
typedef enum __push_client_type__
{
    PUSH_CLIENT_TVT_BEGIN = 0,
	PUSH_CLIENT_IOS_TVT_FREE,				//SuperCam
	PUSH_CLIENT_IOS_TVT_PRO,				//SuperCamPro
	PUSH_CLIENT_IOS_TVT_PROHD,			//SuperCamHD
	PUSH_CLIENT_IOS_TVT_CMS_HD,          //SuperCamCMS
	PUSH_CLIENT_ANDROID_TVT_PRO = 50,	//NONE
	PUSH_CLIENT_TVT_END,
    
	PUSH_CLIENT_PENGANTAI_BEGIN = 100,
	PUSH_CLIENT_IOS_PENGANTAI_FREE,				//SuperLive
	PUSH_CLIENT_IOS_PENGANTAI_IPC,				//SuperLiveIPC
	PUSH_CLIENT_IOS_PENGANTAI_PRO,				//SuperLivePro
	PUSH_CLIENT_IOS_PENGANTAI_HD,				//SuperLiveHD
	PUSH_CLIENT_IOS_PENGANTAI_CMS,				//SuperCMS
	PUSH_CLIENT_IOS_PENGANTAI_SUPERCLIENT,    //SuperClient
	PUSH_CLIENT_IOS_PENGANTAI_SUPERCLIENTM,	//SuperClientM
	PUSH_CLIENT_IOS_PENGANTAI_NVMS,				//NVMS
	PUSH_CLIENT_ANDROID_PENGANTAI_PRO = 150, //SuperLivePro
	PUSH_CLIENT_ANDROID_PENGANTAI_HD,         //SuperLiveHD
	PUSH_CLIENT_ANDROID_PENGANTAI_CMS_HD,   //SuperCMS
	PUSH_CLIENT_ANDROID_PENGANTAI_IPC,				//SuperLiveIPC
	PUSH_CLIENT_ANDROID_PENGANTAI_SUPERCLIENT,    //SuperClient
	PUSH_CLIENT_ANDROID_PENGANTAI_SUPERCLIENTM,	//SuperClientM
	PUSH_CLIENT_ANDROID_PENGANTAI_NVMS,				//NVMS
	PUSH_CLIENT_PENGANTAI_END,
    
	PUSH_CLIENT_US02_BEGIN = 200,
    PUSH_CLIENT_IOS_US02_PRO,					//QTView
	PUSH_CLIENT_IOS_US02_HD,					//QTViewHD
	PUSH_CLIENT_ANDROID_US02_PRO = 250,		//QTView
	PUSH_CLIENT_ANDROID_US02_HD,				//QTViewHD
	PUSH_CLIENT_US02_END,
    
	PUSH_CLIENT_US09_BEGIN = 300,
	PUSH_CLIENT_IOS_US09_PRO,					//EZLIVE
	PUSH_CLIENT_ANDROID_US09_PRO = 350,		//EZLIVE
	PUSH_CLIENT_US09_END,
    
	PUSH_CLIENT_US61_BEGIN = 400,
	PUSH_CLIENT_IOS_US61_PRO,						//BroView
	PUSH_CLIENT_IOS_US61_CMS_HD,					//BroCMS
    PUSH_CLIENT_ANDROID_US61_PRO = 450,			//BroView
	PUSH_CLIENT_US61_END,
    
	PUSH_CLIENT_RU04_BEGIN = 500,
	PUSH_CLIENT_IOS_RU04_PRO,					//VideoPhone
	PUSH_CLIENT_ANDROID_RU04_PRO = 550,		//GrizzlyVideoPhone
	PUSH_CLIENT_RU04_END,
    
	PUSH_CLIENT_IL03_BEGIN = 600,
	PUSH_CLIENT_IOS_IL03_PRO,					//Provison Cam
	PUSH_CLIENT_IOS_IL03_HD,					//Provison PAD
	PUSH_CLIENT_ANDROID_IL03_PRO=650,		//Provison Cam
	PUSH_CLIENT_ANDROID_IL03_HD,				//Provison PAD
	PUSH_CLIENT_IL03_END,
}PUSH_CLIENT_TYPE;

typedef enum __client_os_type__
{
	CLIENT_OS_IOS		= 0,			//ƻ���ͻ���
	CLIENT_OS_ANDROID	= 1,			//��׿�ͻ���
}CLIENT_OS_TYPE;

//DVR返回客户端是否接收到tokenId
typedef enum _send_token_info__
{
    SEND_TOKEN_SUCCED
}SEND_TOKEN_INFO;


///////////////////////////////////////////////DVR�ͺ�PNTS/////////////////////////////////////////


/////provider推送数据的回复
typedef enum __push_data_reply__
{
	PUSH_SUCCESS		= 0,		//���ͳɹ�
	PUSH_FAIL	= 1,				//����ʧ��

}PUSH_DATA_REPLY;


/////PNTS���͸�provider������
typedef enum __push_msg_type__
{
	PUSH_CMD_REQUEST_PUSH_MSG		= 0x100,				//provider����PNTS��������Ϣ��DVR_PUSH_MSG
	PUSH_CMD_REQUEST_PROVIDER_INFO,							//����provider����Ϣ


	PUSH_CMD_PUSH_MSG		= 0x200,					//������Ϣ��ֻ����PROVIDER--��PNTS
	PUSH_CMD_PROVIDER_INFO,								//PROVIDER�豸�����Ϣ,PROVIDER_TYPE_INFO PROVIDER--��PNTS

	PUSH_DEVICE_TOKEN_INFO = 0x300,							//iosÿ�����ӵ�provider��provider��������PNTS,DEVICE_TOKEN_INFO_PNTS
    PUSH_OFFLINE_TOKEN_INFO,                                //��ios�˵��ߺ�provider��������PNTS,DEVICE_TOKEN_INFO_PNTS
}PUSH_MSG_TYPE;

/*
provider���͸�PNTS��������������Ϣ��ʽ:PUSH_MSG_HEADER|PUSH_MSG_INFO|DVR_PUSH_MSG

PNTS��������provider������Ϣ�Ļظ���Ϣ��ʽ:PUSH_MSG_HEADER|PUSH_MSG_INFO|PUSH_DATA_REPLY(int��)

PNTS��������provider������Ϣ��ʽ:PUSH_MSG_HEADER|PUSH_MSG_INFO
provider��PNTS������������Ϣ�Ļظ���Ϣ��ʽ:PUSH_MSG_HEADER|PUSH_MSG_INFO|(ĳ���ṹ��)

provider��PNTS֮�����Ϣ�������̣�
1. provider��PNTS���Ӻ�PNTS����PUSH_CMD_REQUEST_PROVIDER_INFO������Ϣ��provider��provider�ظ�PUSH_CMD_REPLY_PROVIDER_INFO
   �����PROVIDER_TYPE_INFO�ṹ���PNTS��

2. ios�ͻ���ÿ�����ӵ�provider��provider����PUSH_DEVICE_TOKEN_INFO�����DEVICE_TOKEN_INFO_PNTS�ṹ���PNTS.
3. ios�ͻ��˵��ߺ�provider����PUSH_OFFLINE_TOKEN_INFO�����DEVICE_TOKEN_INFO_PNTS�ṹ���PNTS��
4. provider��PNTS֮����������ƣ�
   ÿ��20s����һ��������Ϣ��
 
 
 1��DVR���ߺ��ϴ��豸������Ϣ���������кš�����ID��IP�ȣ�DVR״̬�仯ʱ��Ҫ�ϴ��µ��豸����
 2��ʵʱ����ǰ��token���״̬
 3�������ӷ���������Ϣ��ÿ�����Ӻ�Ҫ����һ��ʱ�䡢������������֮����һ�����
 4����Ϣ�в������豸���������Ϣ������˵IP�����кţ�
 5����ʾ�ַ�������50��
 6����������ַ���䣨��ѡ��
 7�����豸����������������͹��ܣ���Ӧ�������͹��ܣ�>>>>>>����
 8������US02��֧��

*/

typedef struct __push_msg_header__
{
	char startFlag[4];
	unsigned long dataLen;
}PUSH_MSG_HEADER;



typedef struct __push_msg_info__
{
	unsigned long isRequest;	//Ϊ1��ʾ����Ϊ0��ʾ��Ӧ
	unsigned long msgType;		//PUSH_MSG_TYPE
	unsigned long msgLen;		//
}PUSH_MSG_INFO;

/*
�豸���ƽṹ�嶨�壬���ڿͻ��������ӵ�ʱ���͸��豸��
*/
typedef struct __device_token_info__
{
	unsigned long		length;				//DEVICE_TOKEN_INFO���ݳ���
	char				version[8];			//�汾
	CLIENT_OS_TYPE		clientOSType;		//�ͻ��˲���ϵͳ����
	PUSH_CLIENT_TYPE	pushClientType;		//�ͻ�������
	unsigned long		tokenLen;
	char  				token[68];			//�豸����
	char                OSVersion[8];    //ϵͳ�汾��
	char                softVersion[8];   //����汾��
	char				language[16];		//ϵͳ����
	char				Locale[16];			//����
	char			    latitude[16];			//γ��
	char				longitude[16];			//����
}DEVICE_TOKEN_INFO;

/*
设备令牌扩展结构体定义，用于客户端在连接到provider时，provider发给PNTS
*/
typedef struct __device_token_info_pnts__
{
	DEVICE_TOKEN_INFO deviceTokenInfo;
	char providerId[MAX_PROVIDER_ID_LEN];				//provider�ı�ʶId
    char userName[36];                                  //�û���
    //int  sound;                                         //������Ϣ������
}DEVICE_TOKEN_INFO_PNTS;

/*
provider������Ϣ�ṹ�嶨�壬����provider�����ӵ�PNTSʱ��PNTS����provider����Ϣ
*/

typedef struct _provider_type_info
{
    //unsigned long headFlag; ////ȡֵΪHEADFLAG
    unsigned long devicetype; /////////�豸���� DVS  IPCAMERA  DVR  SUPERDVR
    unsigned long producttype;  /////��Ʒ����
    unsigned long netprotrocolver; ///////////�豸��Ӧ������Э��汾
    unsigned long configver; ///////////�豸��Ӧ�����ð汾
    char providerId[MAX_PROVIDER_ID_LEN]; ////////provider�ı�ʶId
    unsigned long encryptType;	//���ݼ��ܷ�ʽ��0 ��ʾû�м��ܷ�ʽ
    unsigned long encryptParam;
    unsigned long ulReserve[6];				/////������Ϣ���Ա��Ժ�����
    unsigned long dwLG;					//֧����Щ���� 0 �������� + Ӣ��  1 �������� + Ӣ��
    unsigned long dwUDPDefaultVideoStream;	//���ڴ�͸Ĭ�ϵ���
	unsigned char companyInfo[32];      //��Ӧ�̱�ʶ
	unsigned long areaId;         //���ڵ���
    char serverIP[MAX_IP_LEN];				//provider��IP
	int  serverPort;				        //provider��port

}PROVIDER_TYPE_INFO, *LPPROVIDER_TYPE_INFO;

/*
������Ϣ�ṹ�嶨�壬provider��Ҫ������Ϣʱ����PNTS
*/
typedef struct __provider_push_msg__
{
	//char providerId[MAX_PROVIDER_ID_LEN];       //provider�ı�ʶId
	char msg[MAX_MSG_LEN];					//��Ϣ��
	int  msgLen;					//��Ϣ��ĳ���
    int  badage;                   
	//char serverIP[MAX_IP_LEN];				//provider��IP
	//int  serverPort;				//provider��port
    //char userName[36];                //�û���
    //char password[36];                //����
	int  msgType;					//������Ϣ����(PUSH_DATA_TYPE,0,1,2)
	int  msgValue;					//��Ϣ�����ֵ(PUSH_ARARM_TYPE/PUSH_OPERATE_TYPE)
    int  paramLen;                  //������Ϣ�����ĳ��ȣ�Ӧ������msgType��msgValue���������ĺ���
	unsigned long time;				//UTCʱ��
}PROVIDER_PUSH_MSG, *LPPROVIDER_PUSH_MSG;


/*
{
	"aps":
	{
		"alert":"2011/9/15 02:00:55 Motion Alarm",
		"sound":"default",
        "badage":"1"
	},
	"URL":"192.168.3.154:80"
	"UTC":"1316052055",
	"T":"1"							//PUSH_DATA_TYPE,0,1,2
	"V":"3"							//�����ֵ
	"bHParam":"len"                  //���������ĳ���
	"P":"0x123456"					//�����������ö����Ʊ�ʾ����Ƶ��ʧ�����������ƶ����ʱ�������ͨ����Ϣ��������ʱ������Ŵ���ʣ��ռ䣬˥��ʱ���������˥�������Ϣ������ʱ������Ų����ĸ�������
    "PSN":"providerId"              //providerId,����MAX_PROVIDER_ID_LEN
    "AU":"user"
    
}
*/
//IPPORT&USER&PASS&UTC&MAC&T&V&HP&P
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma pack()

#endif

/*
CMD_REQUEST_CFG_ADD_IPHONE_TOKEN			//����豸����
����������豸���ƽṹ�� DEVICE_TOKEN_INFO
�������ظ�CMD_REPLY_CFG_ADD_IPHONE_TOKEN	����������豸���ƽ��ADD_TOKEN_STATUS
*/
