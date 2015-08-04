
#ifndef _MULTICAST_HEADER_
#define _MULTICAST_HEADER_

const unsigned long MULTICAST_PORT =23456;
const char * const MULTICAST_IP   ="234.55.55.55";

const unsigned long DEFAULT_LOCAL_PORT	=7838;

#define mmioFOURCC1( ch0, ch1, ch2, ch3 ) \
	( (unsigned long)(unsigned char)(ch0) | ( (unsigned long)(unsigned char)(ch1) << 8 ) |	\
	( (unsigned long)(unsigned char)(ch2) << 16 ) | ( (unsigned long)(unsigned char)(ch3) << 24 ) )

const unsigned long FLAG_MULTIHEAD		=		mmioFOURCC1('M', 'H', 'E', 'D');
const unsigned long MULTICAST_VER	=0x10001; 

const unsigned long DEVICE_NAME_MAX_LEN_MULTI	= 20;		//�豸����󳤶�		
const unsigned long MULTICAST_CMD_SEARCH		= 1; //������������������ָ��
const unsigned long MULTICAST_CMD_INFO			=2; //��DVS��������Ӧָ������Լ�����Ϣ
const unsigned long MULTICAST_CMD_SET			=3; //������������������ָ�����IP��ַ���豸����
const unsigned long MULTICAST_CMD_SET_MAC		=4; //������������������ָ�����MAC��ַ
const unsigned long MULTICAST_CMD_SET_DEFAULT_INPUTFORM = 5; //������������������ָ�����Ĭ����ʽ��IPCamera��ʽ������̶���
const unsigned long MULTICAST_CMD_SET_DEV_VER	=6;//������������������ָ������豸Ӳ���汾��

typedef struct MultiCastInfo
{
	ULONG			head;		//��FLAG_MULTIHEAD
	ULONG			ver;		//�ṹ��Ϣ�汾
	ULONG			cmd;
	char			DevName[DEVICE_NAME_MAX_LEN_MULTI];
	BYTE			MacAddr[6];
	unsigned short	netport;
	ULONG			ipaddr;
	ULONG			netmask;
	ULONG			route;	
	unsigned long	softver;			
	unsigned long	softbuilddate;	//û������
	unsigned short	nHttpPort;
	char			NoUsed[2];
	unsigned long	deviceType;
	unsigned long	productType;	
	unsigned long	kernelVersion;
	unsigned long	inputform;
	unsigned long	devVer;//�豸Ӳ���汾�� 
	char			szPasswd[28];//����IP����������ȷ��admin���룬����base64����
	BYTE			Resved[28];//�Ժ����ӹ��ܾ�������Щ�ֽ�
}MULTICAST_INFO;

//////////////////////////////////////////////////////////////////////////
//extral data(����Resved[28])

//MULTICAST_CMD_SET
typedef struct _hxht_access_info
{
	char				deviceSerialNo[20];		//�豸���к�
	unsigned long		dwAccessServerIP;			//���������IP
	unsigned short		wMsgPort;					//�����������Ϣ�˿�
	unsigned short		wStoreRegPort;				//�洢ע��˿�
}HXHT_ACCESS_INFO;

#endif //_MULTICAST_HEADER_
