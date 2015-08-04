
#ifndef _MULTICAST_HEADER_
#define _MULTICAST_HEADER_

const unsigned long MULTICAST_PORT =23456;
const char * const MULTICAST_IPTOOL_SEND_IP   = "234.55.55.55";
const char * const MULTICAST_IPTOOL_RECV_IP   = "234.55.55.56";

const char * const MULTICAST_IP   = "234.55.55.55";

const unsigned long DEFAULT_LOCAL_PORT	= 9407;


#define mmioFOURCC1( ch0, ch1, ch2, ch3 ) \
	( (unsigned long)(unsigned char)(ch0) | ( (unsigned long)(unsigned char)(ch1) << 8 ) |	\
	( (unsigned long)(unsigned char)(ch2) << 16 ) | ( (unsigned long)(unsigned char)(ch3) << 24 ) )

const unsigned long FLAG_MULTIHEAD		=		mmioFOURCC1('M', 'H', 'E', 'D');

//0x10000-0x10001 �豸���Ʊ�����ANSI��ΪUTF8
//0x10001-0x10002 ֧�����ô�͸�汾���кŹ���
//0x10002-0x10003 ֧��DNS��Ϣ���ܼ�����DNS������˿ڡ�HTTP�˿ڡ�RTSP�˿ڹ���
//0x10003-0x10004 IPCamera��һ����ַ���鲥���ظ���������һ���鲥
//0x10004-0x10005 IPCamera������DHCP���Զ�����������
//0x10005-0x10006 �������޸�IP�лظ�
//0x10006-0x10007	�����������豸��ظ���Ʒ�ͺš�Ĭ����ʽ���ͻ��汾������Ϣ���鲥�ظ����б�ʾ�豸�������,
//					�����˽ṹ�� MULTICAST_INFO_V14
//					����mac��ַ�����˻ظ�
const unsigned long MULTICAST_VER			= 0x10000;
const unsigned long	MULTICAST_10002_VERSION = 0x10002;
const unsigned long	MULTICAST_10003_VERSION = 0x10003;
const unsigned long	MULTICAST_10004_VERSION = 0x10004;
const unsigned long	MULTICAST_10005_VERSION = 0x10005;
const unsigned long	MULTICAST_10006_VERSION = 0x10006;
const unsigned long	MULTICAST_10007_VERSION = 0x10007;
const unsigned long MULTICAST_CURRENT_VERSION	= MULTICAST_10007_VERSION;

const unsigned long DEVICE_NAME_MAX_LEN_MULTI	= 20;		//�豸����󳤶�		
const unsigned long MULTICAST_CMD_SEARCH = 1; //������������������ָ��
const unsigned long MULTICAST_CMD_INFO	  =2; //��DVS��������Ӧָ������Լ�����Ϣ
const unsigned long MULTICAST_CMD_SET	  =3; //������������������ָ�����IP��ַ���豸����
const unsigned long MULTICAST_CMD_SET_MAC =4; //������������������ָ�����MAC��ַ
const unsigned long MULTICAST_CMD_SET_DEFAULT_VIDEOFORMAT = 5; //������������������ָ�����Ĭ����ʽ��IPCamera��ʽ������̶���
const unsigned long MULTICAST_CMD_SET_DEV_VER=6;//������������������ָ������豸Ӳ���汾��
const unsigned long MULTICAST_CMD_SET_CUSTOMERID=7;//������������������ָ����ö��ƿͻ��ţ�ʹ��inputformֵ������ֵ�ο�����Э���еĶ���
const unsigned long MULTICAST_CMD_SET_NATSERVER=8;
const unsigned long MULTICAST_CMD_SET_SEQUENCE_NO=9;//�������к�
const unsigned long MULTICAST_CMD_SET_AUTOSEARCH_SERVER=10;//�Զ�����·����ʱʹ�õķ������ĵ�ַ
//huxinhua add begin
const unsigned long MULTICAST_CMD_SET_IPC_TYPE = 11; //����IPC���ͺ�
//huxinhua add end
const unsigned long MULTICAST_CMD_SET_RESET_9411 = 12; //9411�ָ���������

const unsigned long MULTICAST_CMD_SEARCH_ONE_DEVICE = 13; // ͨ�����������������ָ�����豸
const unsigned long MULTICAST_CMD_SEARCH_ONE_DEVICE_REPLY = 14;

const unsigned long MULTICAST_CMD_SET_LANGUAGE = 15;

const unsigned long MULTICAST_CMD_SET_REPLY = 16;	//����IP��ַ�Ļظ�

const unsigned long MULTICAST_CMS_SET_IPC_ATTRIBUTES = 17;

const unsigned long MULTICAST_CMS_SET_IPC_PRODUCT_MANUFACTURERY = 18;

typedef struct MultiCastHead
{
	ULONG			head;		//��FLAG_MULTIHEAD
	ULONG			ver;		//�ṹ��Ϣ�汾
	ULONG			cmd;
}MULTICASTHEAD;

typedef struct MultiCastInfo
{	
	MULTICASTHEAD			mulHead;
	char			DevName[DEVICE_NAME_MAX_LEN_MULTI];//����˱����ʽΪUTF8�뽫�汾������Ϊ0x10001
	BYTE			MacAddr[6];
	unsigned short	netport;
	ULONG			ipaddr;
	ULONG			netmask;
	ULONG			route;	
	unsigned long	softver;			
	unsigned long	softbuilddate;	
	unsigned short	nHttpPort;
	char			NoUsed[2];
	unsigned long	deviceType;
	unsigned long	productType;	
	unsigned long	kernelVersion;
	unsigned long	VideoFormat;
	unsigned long	devVer;//�豸Ӳ���汾�� 
	char			szPasswd[28];//����IP����������ȷ��admin���룬����base64����
	BYTE			Resved[28];//�Ժ����ӹ��ܾ�������Щ�ֽ�
}MULTICAST_INFO;

struct  MULTI_SET_MAC
{
	MULTICASTHEAD	mulHead;
	//���DevNameû�ã�ֻ��Ϊ�˼�����ǰ��
	char			DevName[DEVICE_NAME_MAX_LEN_MULTI];//����˱����ʽΪUTF8�뽫�汾������Ϊ0x10001
	BYTE			dstMac[6];
	unsigned char	noused[2];
	BYTE			srcMac[6];
	unsigned char	noused1[126];
};

typedef struct MultiCastInfo_v13
{
	MULTICASTHEAD			mulHead;
	char			DevName[DEVICE_NAME_MAX_LEN_MULTI];//����˱����ʽΪUTF8�뽫�汾������Ϊ0x10001
	BYTE			MacAddr[6];
	unsigned short	netport;
	ULONG			ipaddr;
	ULONG			netmask;
	ULONG			route;	
	unsigned long	softver;			
	unsigned long	softbuilddate;	
	unsigned short	nHttpPort;
	unsigned short	nRTSPPort;
	unsigned long	deviceType;
	unsigned long	productType;	
	unsigned long	kernelVersion;
	unsigned long	VideoFormat;		//��Ƶ��ʽ
	unsigned long	devVer;				//�豸Ӳ���汾�� 
	char			szPasswd[28];		//����IP����������ȷ��admin���룬����base64����
	unsigned long	dwDNSServer1;		//��̬IP�趨ʱ��DNS��ַ1
	unsigned long	dwDNSServer2;		//��̬IP�趨ʱ��DNS��ַ2
	unsigned long	dwIPCameraNo;		//IPCamera���к�
	BYTE			bTraversOnline;		//IPCamera�봩͸������ע������	
	BYTE			ucIsWIFI;			//0 ���� 1 wifi
	BYTE			ucStartAutoSearch;	//0 û�п��Զ����� 1 �����Զ�����
	BYTE			ucSupportAutoSearch; //0 ��֧���Զ����� 1 ֧���Զ�����	
	unsigned long	dwSecondIP;
	unsigned long	dwSecondMask;
	unsigned char   ucDhcpcdStatus;		// �ظ�״̬ʱ��0 δ���� 1 �������� 2 �ɹ� 3 ʧ��
										// IPTOOL ����IPCameraʱ�� 0 �ر� 1����
	unsigned char	ucLanguage;
	unsigned char   ucIPMode;			//0 ��̬IP 1 dhcp
	unsigned char	ucStartPPPOE;		//0 δ���� 1 �ѿ��� pppoe����	
}MULTICAST_INFO_V13;


typedef struct MultiCastInfo_v14
{
	MULTICASTHEAD			mulHead;
	char			DevName[DEVICE_NAME_MAX_LEN_MULTI];//����˱����ʽΪUTF8�뽫�汾������Ϊ0x10001
	BYTE			MacAddr[6];
	unsigned short	netport;
	ULONG			ipaddr;
	ULONG			netmask;
	ULONG			route;	
	unsigned long	softver;			
	unsigned long	softbuilddate;	
	unsigned short	nHttpPort;
	unsigned short	nRTSPPort;
	unsigned long	deviceType;
	unsigned long	productType;	
	unsigned long	kernelVersion;
	unsigned long	VideoFormat;		//��Ƶ��ʽ
	unsigned long	devVer;				//�豸Ӳ���汾�� 
	char			szPasswd[28];		//����IP����������ȷ��admin���룬����base64����
	unsigned long	dwDNSServer1;		//��̬IP�趨ʱ��DNS��ַ1
	unsigned long	dwDNSServer2;		//��̬IP�趨ʱ��DNS��ַ2
	unsigned long	dwIPCameraNo;		//IPCamera���к�
	BYTE			bTraversOnline;		//IPCamera�봩͸������ע������	
	BYTE			ucIsWIFI;			//0 ���� 1 wifi
	BYTE			ucStartAutoSearch;	//0 û�п��Զ����� 1 �����Զ�����
	BYTE			ucSupportAutoSearch; //0 ��֧���Զ����� 1 ֧���Զ�����	
	unsigned long	dwSecondIP;
	unsigned long	dwSecondMask;
	unsigned char   ucDhcpcdStatus;		// �ظ�״̬ʱ��0 δ���� 1 �������� 2 �ɹ� 3 ʧ��
										// IPTOOL ����IPCameraʱ�� 0 �ر� 1����
	unsigned char	ucLanguage;
	unsigned char   ucIPMode;			//0 ��̬IP 1 dhcp
	unsigned char	ucStartPPPOE;		//0 δ���� 1 �ѿ��� pppoe����

	char			szProduct[24];
	char			szCustomer[24];
	char			defaultInputNorm;
	char			softReset;			//1 ��Ҫ�����λ
	char			noused1[2];
	int				iRand;
    
    char            szProductType[16];//   ���������Ǹ�NVR��ʾ��
    char            szManufacturery[16]; //    
	char			noused2[12];
}MULTICAST_INFO_V14;

/********************	����NATSERVER IP  ********************/
typedef struct Multi_NatServer
{
	bool			bUsed;		//�Ƿ������������ַ
	bool			noused;		//û��
	unsigned short	nPort;
	char			szServer[64];//Nat�������ĵ�ַ	
}MULTI_NATSERVER;

// MULTICAST_CMD_SET_REPLY
typedef struct MultiSetIpReply
{
	MULTICASTHEAD		mulHead;
	DWORD				ipAddress;
	unsigned short		nPort;
	unsigned short		nResult; //0 �ɹ� 1 ʧ��
	BYTE				Resved[120];
}MULTI_SETIP_REPLY;

typedef struct Multi_NatServer_Info
{
	unsigned long		dwReLoginTime;
	MULTI_NATSERVER		natServer[10];	
}MULTI_NATSERVER_INFO;

typedef struct MultiSetNatServer
{
	MULTICASTHEAD			mulHead;
	BYTE					MacAddr[8];	//ָ���޸�˭��NatServer
	MULTI_NATSERVER_INFO	mulNatServerInfo;
}MULTI_SETNATSERVER;
/********************	����NATSERVER IP  ********************/

/********************	���� IPC ���к�  ********************/
typedef struct MultiSetSeqNo
{
	MULTICASTHEAD			mulHead;
	BYTE					MacAddr[8];	//ָ���޸�˭�����к�
	unsigned long			dwIPCameraNo;
}MULTI_SET_SEQ_NO;
/********************	���� IPC ���к�  ********************/

/********************	���� �Զ�����·�����ķ�����  ********************/
struct MULTI_AUTO_SEARCH_SERVER
{
	bool	bUsed;
	char	noused;
	unsigned short	nPort;
	unsigned long	dwIP;
};

struct MULTI_SET_AUTO_SEARCH_SERVER
{
	MULTICASTHEAD			mulHead;
	BYTE					MacAddr[8];	//ָ���޸�˭��NatServer
	MULTI_AUTO_SEARCH_SERVER	servers[10];
};
/********************	���� �Զ�����·�����ķ�����   ********************/

/********************	9411 �ָ���������  ********************/
struct MULTI_SET_RESET_9411	
{
	MULTICASTHEAD			mulHead;
	char					password[16];
};

struct MULTI_SET_RESET
{
	MULTICASTHEAD			mulHead;
	char					password[16];
	unsigned char			szMac[8];
};
/********************	9411 �ָ���������  ********************/

//MULTICAST_CMD_SEARCH_ONE_DEVICE
struct MULTICAST_INFO_ONE_DEVICE
{
	MULTICASTHEAD	mulHead;
	char			szSeqOrAlias[32];
};

//MULTICAST_CMD_SEARCH_ONE_DEVICE_REPLY
struct MULTICAST_INFO_ONE_DEVICE_REPLY
{
	MULTICASTHEAD			mulHead;
	char			szSeqOrAlias[32];
	char			DevName[DEVICE_NAME_MAX_LEN_MULTI];//����˱����ʽΪUTF8�뽫�汾������Ϊ0x10001
	BYTE			MacAddr[6];
	unsigned short	netport;
	ULONG			ipaddr;
	ULONG			netmask;
	ULONG			route;	
	unsigned long	softver;			
	unsigned long	softbuilddate;	
	unsigned short	nHttpPort;
	unsigned short	nRTSPPort;
	unsigned long	deviceType;
	unsigned long	productType;	
	unsigned long	kernelVersion;
	unsigned long	VideoFormat;		//��Ƶ��ʽ
	unsigned long	devVer;				//�豸Ӳ���汾�� 
	char			szPasswd[28];		//����IP����������ȷ��admin���룬����base64����
	unsigned long	dwDNSServer1;		//��̬IP�趨ʱ��DNS��ַ1
	unsigned long	dwDNSServer2;		//��̬IP�趨ʱ��DNS��ַ2
	unsigned long	dwIPCameraNo;		//IPCamera���к�
	BYTE			bTraversOnline;		//IPCamera�봩͸������ע������	
	BYTE			ucIsWIFI;			//0 ���� 1 wifi 10 û�д�IP
	BYTE			ucStartAutoSearch;	//0 û�п��Զ����� 1 �����Զ�����
	BYTE			Resved1[1];			//�Ժ����ӹ��ܾ�������Щ�ֽ�
	unsigned long	dwSecondIP;
	unsigned long	dwSecondMask;
	unsigned char   ucDhcpcdStatus;		// �ظ�״̬ʱ��0 δ���� 1 �������� 2 �ɹ� 3 ʧ��
										// IPTOOL ����IPCameraʱ�� 0 �ر� 1����
	unsigned char	ucLanguage;
	BYTE			Resved2[2];			//�Ժ����ӹ��ܾ�������Щ�ֽ�
};

//MULTICAST_CMD_SET_LANGUAGE
struct MULTI_SET_LANGUAE
{
	MULTICASTHEAD		mulHead;
	unsigned long		dwLanguage;	 //֧����Щ���� 0 �������� + Ӣ��  1 �������� + Ӣ��
};

//MULTICAST_CMS_SET_IPC_ATTRIBUTES
struct IPC_ATTRIBUTES
{
	MULTICASTHEAD		mulHead;
	unsigned long		dwDefaultInput;
	unsigned long		dwProductType;
	unsigned long		dwCustomerType;
	unsigned long		dwLanguage;
	unsigned long		noused[50];
};

#endif //_MULTICAST_HEADER_

