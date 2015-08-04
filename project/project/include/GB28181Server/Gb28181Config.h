#pragma once
#include <stdio.h>
#include <map>
#include <utility>
#include <string>

#include "GBT28181_Define.h"

#define  GB_FAIL  0xFFFF

enum RELOAD_TYPE
{
	SIPSERVER,		//sip server ����
	CHANNELID,		//ͨ��id����
	ALL,			//ȫ������
};
typedef struct _gb28181_init_info
{
	unsigned long	dwMaxVideo;
	unsigned long   dwMaxALarm;
	unsigned long	dwMaxAudio;
	unsigned long	dwDeviceType;

	PRODUCT_TYPE	dwProductType;
	unsigned long	dwProductID;
	unsigned long	dwProductSubID;

	unsigned long	dwDevVer;
	unsigned long	dwSoftVer;
	unsigned long	dwBuildTime;
	char			deviceName [20];
}Gb28181_INIT_INFO;
#ifndef  __ENVIRONMENT_LINUX__
#include<WinSock2.h>
#pragma comment(lib,"WS2_32")
class CInitSock
{
public:
	CInitSock(BYTE minorVer=2,BYTE majorVer=2)
	{
		WSADATA wsaData;
		WORD sockVersion=MAKEWORD(minorVer,majorVer);
		if(WSAStartup(sockVersion,&wsaData)!=0)
		{
			exit(0);
		}
	}
	~CInitSock()
	{
		WSACleanup();
	}
};
#endif
class GB28181_CONFIG
{
public:

	static GB28181_CONFIG* Instance();

	GB28181_CONFIG();

	~GB28181_CONFIG();

	//ɾ�������豸
	void Delete();

	 void SetGB28181Config(const GB28181_SIP_SERVER & Gb28181SipServer);
	
	 void SetGB28181InfoConfig(const Gb28181_INIT_INFO & Gb28181Info);

	//ͨ��ID ��ȡDeviceID
	std::string GetDvrID(unsigned short uiChannel); 
	
	//ͨ��DeviceID ��ȡͨ��ID
	unsigned short GetChannel(const string &strDvrID); 

    void InsertData(const string & strDvrID,unsigned short uiChannel);

	void InsertAlarmData(const string & strAlarmID,unsigned short uiAlarmChannel);

	//��ȡ�澯�豸��ͨ��ID
	unsigned short GetAlarmChannel(const string &strAlarmID);

	//��ȡ�澯�豸ID
	std::string GetAlarmID(unsigned short uiAlarmChannel);


	std::string GetProductName(PRODUCT_TYPE type);

	//�������б仯ʱ�����������ͨ�����ݣ����²���
	void ClearData();

	//�������б仯ʱ�������������Alarm���ݣ����²���
	void ClearAlarmData();

	void ClearDvrIDsData();
	
	void ClearDvrAlarmIDsData();

	//���¼�������
	bool ReloadConf(const enum RELOAD_TYPE){return true;};

	bool SetSipServerIP(unsigned long ulServerIP);

	bool SetDvrIP(unsigned long uldvrIP);
	
	#ifndef __ENVIRONMENT_LINUX__
	bool SetDvrWindowsIP();
	#endif
public:
	char m_wSipserverIP[32];				//sip������IP��ַ
	char m_SipServerId[32];					//sip������ID
	char m_Dvrip[32];						//DVR ip
	char m_DvrId[32];						//�豸Id
	char m_UserID[32];						//�û���
	char m_RegisterPasswd[32];				//ע������

	char m_szOwner[64];					//<!-- ��Ϊ�豸ʱ���豸��������ѡ�� -->
	char m_szCivilCode[64];				//<!-- �������򣨱�ѡ�� -->
	char m_szBlock[64];					//<!-- ��������ѡ�� -->

	unsigned short	m_wSipserverPort;		//sip�������Ự�˿�
	unsigned short	m_wSipDvrPort;			//dvr sip�Ự�˿�
	unsigned int	m_expires;				//����ʱ��

	unsigned char	m_dwMaxCamera;          //���֧�ֵ��������Ŀ
	unsigned char	m_dwMaxALarm;            //���֧�ֵĸ澯��
	unsigned long	m_dwDevVer;             //Ӳ���汾
	unsigned long	m_dwSoftVer;            //����汾
	unsigned long	m_dwBuildTime;          //�汾����ʱ��
	char			m_deviceName [20];       //�豸����
	PRODUCT_TYPE    m_productType;           //��Ʒ����

	deque<CCatalogItem*> m_deqItem;		//<!-- �豸Ŀ¼���б�,Num��ʾĿ¼����� -->
	deque<CDeviceStatusItem*> m_deqAlarmItem;  //!--�����豸Ŀ¼

	//std::map<string,unsigned short> dvrIDs;
	//std::map<string,unsigned short> dvrAlarmIDs;

	std::map<unsigned short,string> dvrIDs;
	std::map<unsigned short,string> dvrAlarmIDs;
};
