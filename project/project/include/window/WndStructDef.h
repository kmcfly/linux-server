#ifndef _WND_STRUCT_DEF__
#define _WND_STRUCT_DEF__

#include "dvrdvsdef.h"
#include "Product.h"
#include "NetConfigDefine.h"

//////////////////////////////////////////////////////////////////////////
// ���á���ҳ��ר�ýṹ
//////////////////////////////////////////////////////////////////////////

#if defined(__ENVIRONMENT_WIN32__) || defined(__TDFH__) || defined(__TDNXP__) || defined(__CHIP3531__) || defined(__CHIP3535__) || defined(__CHIP3521__) || defined(__CHIP3520A__) || defined(__CHIPTI__) || defined(__CHIP3520D__) || defined(__CHIPGM__) 
#pragma pack(4)
#endif

//���б���������Ϣ =>> CfgTriggerDlg.h
typedef struct _alarm_trigger_info_
{
	unsigned short  Buzzer;					//����������
	unsigned short	PtzSafeChnn;			//Motion�¼�,���ܴ���ͨ��
	ULONGLONG		AlarmOut;				//�����������
	ULONGLONG		Record;					//����¼��
	ULONGLONG		FullScreen;				//�������汨��
	TO_PTZ			ToPtz[MAX_CHNN_NUM];	//������̨, ��������ͷ�ƶ�
	unsigned long   EmailEnable;			//�Ƿ񴥷�EMail ����
	unsigned long 	EmailAddr;
	char			AddrInfo[3][68];		//��ȡ������ǰ����EMail�ĵ�ַ����
	ULONGLONG		SnapCH;					//ץͼͨ��
	ULONGLONG		ftpVideo;				//����FTP��Ƶ�ϴ�
}ALARM_TRIGGER_INFO;

//EmailAddr:�û�ѡ���EMail���͵�ַ
//���λ��Ӧ���͵�ַ1,�ε�λ��Ӧ���͵�ַ2,�Դ�����,��Ϊ1ʱ��ʾ�û�ѡ�ô˵�ַ,Ϊ0��ʾû��ѡ��
//��:���EmailAddr = 0x03,��ʾ��ѡ���˵�һ������͵ڶ���������Ϊ������ķ��͵�ַ

typedef struct _wizard_info_ 
{
	//�������ò���
	unsigned long Modified;				//�Ƿ��޸���
	unsigned long DevMaxNameLen;		//�豸������
	unsigned long DateMode;				//���ڸ�ʽ
	unsigned long TimeMode;				//ʱ���ʽ
	unsigned long TimeZone;				//ʱ��
	unsigned long CurrTime;				//��ǰʱ��
	unsigned long LanguageNum;			//������Ŀ
	unsigned long CurrentLanguage;		//��ǰϵͳ����
	unsigned long CurrentResolution;	//��ǰ�ֱ���
	unsigned long ShowWzd;				//�´��Ƿ���ʾ��

	//�������ò���
	unsigned long Port;					//http�˿ں�
	unsigned long ServerPort;			//����˿ں�
	NETWORK_ADDR_INFO Addr_Info;		//��ַ����Ϣ
	unsigned long DNS_MainIP;			//��DNS����
	unsigned long DNS_SubIP;			//��DNS����
	
	//�������
	NCFG_INFO_SUPPORT_FRAME_RATE *pFrameRateData; //�������
	unsigned long ChannelNum;
	unsigned long defauQualityLevel;			//Ĭ������ˮƽ
	unsigned long QualityLevelNum;				//����λ��
	unsigned long videoEncodeMode;				//��Ƶģʽ
	unsigned long defaVideoEncodeMode;			//Ĭ����Ƶģʽ
	unsigned long bitStreamCount;				//λ������
	unsigned long defaBitStreamValue;			//Ĭ��λ��
	unsigned long bAllCH;
	unsigned long QualityLevelValue[32];		//��������
	unsigned long BitStreamValue[32];			//λ��ֵ
	NCFG_INFO_LANGUAGE Language[32];			//��ȡϵͳ����
	ENCODE_INFO_EX InfoEx[32];
	char DeviceName[36];						//�豸��

	NCFG_INFO_ENCODE_SPEC_FRAME_RATE FrameRateSpec; //����ķֱ�������
	CHANNEL_RECP * pChRecp;
	VS_SUPBRR *pVSBit;
}WIZARD_INFO;


typedef struct _wzd_US02_info_
{
	unsigned long		Modified;				//�Ƿ��޸���
	
	//�������
	unsigned long		LanguageNum;			//������Ŀ
	unsigned long		CurrentLanguage;		//��ǰϵͳ����
	
	//ʱ��
	unsigned long		DateMode;				//���ڸ�ʽ
	unsigned long		TimeMode;				//ʱ���ʽ
	unsigned long		TimeZone;				//ʱ��
	unsigned long		CurrTime;				//��ǰʱ��
	unsigned long		ShowWzd;				//�´��Ƿ���ʾ��
	unsigned long		sync;					//�Ƿ�ͷ�����ʱ��ͬ��
	unsigned long		httpPort;				//����˿�
	unsigned long		serverPort;				//����˿�
	NCFG_INFO_LONG_NAME timeServer;			    //ʱ���������ַ
	unsigned long		UpDateWzdTime;			//�Ƿ��ӳ���ʱ�䣬ʱ��ͬ����ʱ���޸�
	//����ʱ��DST��
	DAYLIGHT_INFO		dstInfo;				//����ʱ��Ϣ
	NETWORK_ADDR_INFO   Addr_Info;				//��ַ��Ϣ
	
	NCFG_INFO_LANGUAGE  Language[32];			//��ȡϵͳ֧������

	char				RouterIp[32];			//��ȡ��·��ip
	unsigned short		MobileDevice;			//ѡȡ���ƶ��豸�ͺ�
	//DDNS����
	unsigned long		maxUserLen;				//DDNS�û�����󳤶�
	unsigned long		maxPassLen;				//DDNS������󳤶�
	unsigned long		maxDomainLen;			//DDNS������󳤶�
	DDNS_INFO			ddnsInfo;				//DDNS����
	unsigned long		ddnsCycle;				//DDNSѭ������
	unsigned char		ddnsServerName[132];	//DDNS��������
	DDNS_SERVER_INFO*	pDdnsSeverInfo;			//�����е�DDNS_SERVER_INFO��Ϣ
	int					ddnsServerCount;		//�ܹ�ӵ�е�DDNS_SERVER_INFO����
}US02_WzdInfo;

typedef struct _nat_wizard_info_ 
{
	unsigned long		Modified;				//�Ƿ��޸���
	
	//�������
	unsigned long		LanguageNum;			//������Ŀ
	unsigned long		CurrentLanguage;		//��ǰϵͳ����
	
	//ʱ��
	unsigned long		DateMode;				//���ڸ�ʽ
	unsigned long		TimeMode;				//ʱ���ʽ
	unsigned long		TimeZone;				//ʱ��
	unsigned long		CurrTime;				//��ǰʱ��
	unsigned long		ShowWzd;				//�´��Ƿ���ʾ��
	unsigned long		sync;					//�Ƿ�ͷ�����ʱ��ͬ��
	unsigned long		httpPort;				//����˿�
	unsigned long		serverPort;				//����˿�
	NCFG_INFO_LONG_NAME timeServer;			    //ʱ���������ַ
	unsigned long		UpDateWzdTime;			//�Ƿ��ӳ���ʱ�䣬ʱ��ͬ����ʱ���޸�

	//�û���Ϣ���ֻ����Ŀ
	unsigned long		userNameMaxLen;			//��ȡ���û�����󳤶�
	unsigned long		passwordMaxLen;			//��ȡ��������󳤶�
	//����ʱ��DST��
	DAYLIGHT_INFO		dstInfo;				//����ʱ��Ϣ
	NETWORK_ADDR_INFO   Addr_Info;				//��ַ��Ϣ
	
	NCFG_INFO_LANGUAGE  Language[32];			//��ȡϵͳ֧������

	char				RouterIp[32];			//��ȡ��·��ip
	unsigned short		MobileDevice;			//ѡȡ���ƶ��豸�ͺ�
	NCFG_INFO_USER		userInfo[MAX_USER_NUM]; //��ȡ���е��û���Ϣ
	NAT_SERVER_INFO		natInfo;				//��͸����Ϣ
}NAT_WIZARD_INFO;

typedef struct _other_alarm_info_
{
	unsigned long	DiskAlarmFree;			//ʣ�������������

	unsigned long	Buzzer;					//����������
	unsigned long	Email;					//�Ƿ񴥷�EMail ����
	ULONGLONG		AlarmOut;				//�����������
}OTHER_ALARM_INFO;

typedef struct _wnd_startup_info_
{
	NCFG_INFO_TOOLBAR_POS	ToolbarPos;			//toolbar��λ��
	unsigned long			DoDwell;			//�Ƿ���̨
	unsigned long			SplitMode;			//��̨�ָ�ģʽ
	unsigned long			SplitData[8][128];	//�ָ�����
}WND_STARTUP_INFO;

typedef struct  _wnd_back_door_info_
{
	unsigned char BootImage;	//��������
	unsigned char Logo;			//��ʾLogo
	unsigned char DDNS;			//DDNS
	unsigned char Keyboard;		//����
	unsigned char Mac;			//MAC
	unsigned char Hardware;		//Ӳ���汾
	unsigned char CompanyID;	//����ID
	unsigned char Lang;			//����
	unsigned char PTZ;			//�޸�PTZ
	unsigned char CompanyName;	//��������
	unsigned char Reserve[2];	//����
}WND_BACK_DOOR_INFO;

#if defined(__ENVIRONMENT_WIN32__) || defined(__TDFH__) || defined(__TDNXP__) || defined(__CHIP3531__) || defined(__CHIP3535__) || defined(__CHIP3521__) || defined(__CHIP3520A__) || defined(__CHIPTI__) || defined(__CHIP3520D__) || defined(__CHIPGM__) 
#pragma pack()
#endif

#endif
