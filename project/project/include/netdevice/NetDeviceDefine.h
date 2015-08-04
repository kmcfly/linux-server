#ifndef _NET_DEVICE_DEFINE_
#define _NET_DEVICE_DEFINE_

/*
 * ���ڶ��� Net Device ��
 * ��ǰ׺ ND_
 * NVR�汾���� : ��׺ _NVR_V3 , _NVR_V4
 * IPC�汾���� : ��׺ _IPC_V3 , _IPC_V4
 * NVR�����������..\pluginĿ¼, ���ض�̬��, ��������֧�ֵ�IPC
 * �����������е�IPC�����ӻỰ, ����Ƿ�Ϊ֧�ֵ�IPC����
*/

#ifdef __ENVIRONMENT_WIN32__
#pragma pack(4)
#endif

const char MAIN_ETHER[] = "eth0";
const char POE_ETHER[] = "eth1";

enum NET_DEVICE_STREAM_TYPE
{
	NET_DEVICE_MAIN_STREAM,		//������
	NET_DEVICE_SUB_STREAM,		//������
	NET_DEVICE_MINOR_STREAM,	//�ֳ�С����
	NET_DEVICE_JPEG_STREAM,		//ͼƬ��
	NET_DEVICE_MAX_STREAM,
};

enum MANUFACTURE_ID
{
	MID_UNKNOWN,
	MID_TVT,
	MID_ONVIF,
	MID_COUNT,
};

enum ONVIF_MANU_ID
{
	MANU_ONVIF_NORMAL,
	MANU_ONVIF_HIKVISION,
	MANU_ONVIF_DAHUA,
	MANU_ONVIF_XIONGMAI,
	MANU_ONVIF_ZNV,
	MANU_ONVIF_ZHONGWEI,
	MANU_ONVIF_TIANSHITONG,
	MANU_ONVIF_LONGPLUS,
	MANU_ONVIF_TVT,
	MANU_ONVIF_QSEE,
	MANU_ONVIF_XTS,
	MANU_ONVIF_KEC,
	MANU_ONVIF_NVSIP,
	MANU_ONVIF_STENLEY,
	MANU_ONVIF_SUNELL,
};

//lib��̬��Ļ�����Ϣ
typedef struct _nd_lib_info 
{
	unsigned int	manufacturerId;		//����ID������MANUFACTURE_ID
	char			version[28];		//��汾��Ϣ ����"v2.0.1"
	unsigned char	bNeedHttpPort;		//�Ƿ���ҪHTTP�˿ڽ���ͨ��
	unsigned char	bNeedCtrlPort;		//�Ƿ���Ҫר�ŵĿ��ƶ˿�ͨ��
	unsigned char	resv[2];
}ND_LIB_INFO;

//MAC
typedef struct _nd_mac
{
	unsigned char mac[8];
}ND_MAC;

//���������� �õ��������豸��Ϣ
typedef struct _nd_device_info
{
	char			szAddr[64];
	unsigned long	netmask;
	unsigned long	gateway;
	unsigned short	netport;
	unsigned short	nHttpPort;
	unsigned short	nCtrlPort;
	char			szID[64];	//�����豸��ʶ
	char			firmwareVersion[16];
	unsigned long	manufacturerId;
	char			manufacturerName[36];
	char			productModel[36];
	char			szEther[8];
}ND_DEVICE_INFO;

typedef struct _nd_device_network_info_
{
	unsigned long	curIpAddr;
	unsigned long	newIpaddr;
	unsigned long	newNetmask;
	unsigned long	newGateway;
	char			userName[36];
	char			password[28];
//	unsigned char	MacAddr[6];
	char			szID[64];	//�����豸��ʶ(����MAC��ַ)
	unsigned short	notused;
}ND_DEVICE_NETWORK_INFO;

//�����豸֧�ֵĲ�����Ϣ
typedef struct _nd_dev_info
{
	unsigned long	chnn;			//ͨ��
	unsigned long	videoSize;		//֧�ֵ���Ļ�ֱ��� DD_VIDEO_SIZE, ���� �Ƿ�֧��D1: (videoSize & (0x1 << 3))
	unsigned char	ucRate[32];		//ÿ�ֱַ��ʶ�Ӧ�����֡��, ���� D1��Ӧ���֡��: ucRate[3]
	unsigned long	maxBitRate[32];	//ÿ�ֱַ��ʶ�Ӧ��������ʣ����� D1��Ӧ�������: maxBitRate[3]
}ND_DEV_INFO;

//�����豸������Ϣ
typedef struct _nd_alarm_info
{
#ifdef	__ENVIRONMENT_WIN32__

	ULONGLONG	sensor;				//����������
	ULONGLONG	motion;				//�ƶ����
	ULONGLONG	videoLoss;			//��Ƶ��ʧ
	ULONGLONG   other;				//��������
#else
	unsigned long long 	sensor;				//����������
	unsigned long long 	motion;				//�ƶ����
	unsigned long long 	videoLoss;			//��Ƶ��ʧ
	unsigned long long  other;				//��������
#endif
}ND_ALARM_INFO;

#ifdef	__ENVIRONMENT_WIN32__
#pragma pack()
#endif

#endif
//end
