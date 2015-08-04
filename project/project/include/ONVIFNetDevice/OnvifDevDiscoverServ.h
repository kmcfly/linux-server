/*******************************************************************************
** Copyright (c) Tongwei Video Technology Co.,Ltd. All rights reserved.				
** AUTHOR		:	������	
** DATE			:	2013-02-21
** FILENAME		:	OnvifDevDiscoverServ.h
** Version		:	ONVIF 2.1
** DESCRIPTION	:
** Modify Record:
*******************************************************************************/
#ifndef _OnvifDevDiscoverServ_H_
#define _OnvifDevDiscoverServ_H_

#include "DDPublic.h"
#include "NetDevicePlatform.h"
#include "soapServ.h"
#include "OnvifDevMgtServ.h"

class COnvifDevDiscover
{
public:
  COnvifDevDiscover();
  ~COnvifDevDiscover();
  
  //return device discover instance,this instance is a static variable
  static COnvifDevDiscover *Instance(void);
  //��ʼ����
  void StartSearch(void);
  //ֹͣ����
  void StopSearch(void);

  //function
  //��ȡ���������豸��Ϣ���豸�������豸�б���
  //input:������豸�б�
  //deviceNum:��ȡ���豸��Ŀ
  //return:���浽�豸��Ϣ���е��豸����
  int  GetDeviceInfo(nd_in ND_DEVICE_INFO* p_device_info, nd_in int deviceNum, bool bGetIpcInfo );
  int  GetDeviceNum(void);

  int GetIPCDevviceSerial(char *servAddr, char *aUserName, char *aPassKey, int aDigest_type, bool& bGetSerialFlag );
  int SetCurDevicePoeChnnCnt( int poe_chnn_cnt );

protected:
	typedef struct OnvifDisDevInfo
	{
		char Addr[128];
		char endPointReference[64];
		char type[128];
		char szEther[8];
		bool active;
		int  noActiveNum;
		bool bIsTvtIpc;
		char Manufacturer[32];
		char firwareVersion[32];
		unsigned int manu_id_val;
		ULONGLONG ullSearchTimes;
		unsigned long	manufacturerId;
		struct OnvifDisDevInfo *next;
	}ONVIF_DEV_INFO;


	typedef struct OnvifSecondDisDevInfo
	{
		char Addr[128];
		char endPointReference[32];
		char Manufacturer[32];
		char szEther[8];
		char manu_id_val;
		ULONGLONG ullSearchTimes;
		unsigned long	manufacturerId;
	}ONVIF_SECOND_DEV_INFO;

	struct OnvifDisDevInfo *m_pNewHead;
	struct OnvifDisDevInfo *m_pOldHead;
	
	int DiscoveryStart(const char *soap_endpoint, const char *soap_action, struct d__ProbeType *d__Probe, const char *pEther);
	int DiscoveryRecv(struct d__ProbeMatchesType *d__ProbeMatches);
	int DiscoveryStop(void);

	static PUB_THREAD_RESULT PUB_THREAD_CALL DeviceDiscovery(void *param);
	PUB_THREAD_RESULT DeviceDiscoveryProc(void);

	PUB_thread_t		m_threadID;
	bool				m_bDiscovering;

	CPUB_Lock          m_lock;
	CWaitEvent         m_updataEvent;

private:
	void UpdateDevInfoList(void);
	bool GenNewDevList(struct OnvifDisDevInfo *newDev);
	void DeleteNoActiveDevice(void);
	void SetDeviceNoActive(void);
	void FreeDeviceInfo(OnvifDisDevInfo *aListHead);
	int  GetListNodeNum(OnvifDisDevInfo *aListHead);
    int  GetDevSerial(char *servAddr,char *serialNum,int &serialLen, int &product_id );

	int SecondGetDevSerial(char *servAddr,ONVIF_SECOND_DEV_INFO* pOvifInfo );
	int GetIPCProductIDNum(char* ManufacStr,char* ProdMode,char* firwareversion);
	bool SecondCheckDevhas(ONVIF_SECOND_DEV_INFO* pDevInfo );
	bool SecondRegetIpcProdInfo(const unsigned char* szIpAddr, ONVIF_SECOND_DEV_INFO& stOnvifInfo);
	void SecondDeleteTimerOld( ULONGLONG ullTimes );
	int SecondDiscoveryStart(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct d__ProbeType *d__Probe, const char *pEther);
	int SecondDiscoveryRecv(struct soap *soap, struct d__ProbeMatchesType *d__ProbeMatches);
	int SecondDevicesDiscover(int i_eth_pos);
	int SecondAllEthDeviceDiscover();


	int SetONVIFDeviceIPAddr(char* search_onvif_ip_addr);

	inline char* strlwr(char* str)
	{
		char* orig = str;
		for (; *str != '\0'; str++)
		{
			*str = tolower(*str);
		}
		return orig;
	}

    struct soap *m_pSoap;
	struct SOAP_ENV__Header m_header; 
	d__ProbeType			m_discoverReq;
	d__ProbeMatchesType     m_discoverResp;
	d__ScopesType          m_discoverScope;

	char					*m_pTmp;

	char					m_uuid[64];
	OnvifDisDevInfo			*m_pDevInfo;
	OnvifDisDevInfo			m_devInfoBuffer;

	std::list<ONVIF_SECOND_DEV_INFO>		m_listOnvifDevice;
	int                             m_iStartSearchCount;

	bool                    m_bHasMustSearchEth1Dev;

};

#endif