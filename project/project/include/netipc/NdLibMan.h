/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : L
** Date         : 2011-05-05
** Name         : NdLibMan.h
** Version      : 1.0
** Description  : ��������IPC��
** Modify Record: 
1:
***********************************************************************/

#ifndef _ND_LIB_MAN_H_
#define _ND_LIB_MAN_H_

#include "DynamicLinkLib.h"
#include "NetDevicePlatform.h"
#include "NetProtocol.h"

#include <string>
#include <map>
#include <vector>

//ND�⵼����������
typedef int			(*ND_INIT)();
typedef int			(*ND_QUIT)();
typedef int			(*ND_GET_INFO)(ND_LIB_INFO*);
typedef int			(*ND_GET_DISCOVERED_DEVICE_NUM)();
typedef int			(*ND_GET_DISCOVERED_DEVICE_INFO)(ND_DEVICE_INFO*, int);
typedef void		(*ND_SET_DISCOVERED_DEVICE_INFO)(ND_DEVICE_NETWORK_INFO *);
typedef int			(*ND_ON_TIMER)(unsigned long);
typedef CNetDevice* (*ND_CREATE)();
typedef void		(*ND_DESTROY)(CNetDevice*);

#define ND_INIT_NAME						"nd_init"
#define	ND_QUIT_NAME						"nd_quit"
#define ND_GET_INFO_NAME					"nd_get_info"
#define ND_GET_DISCOVERED_DEVICE_NUM_NAME	"nd_get_discovered_device_num"
#define ND_GET_DISCOVERED_DEVICE_INFO_NAME	"nd_get_discovered_device_info"
#define ND_SET_DISCOVERED_DEVICE_INFO_NAME  "nd_set_discovered_device_info"
#define	ND_ON_TIMER_NAME					"nd_on_timer"
#define	ND_CREATE_NAME						"nd_create"
#define ND_DESTROY_NAME						"nd_destroy"

typedef struct _nd_dll_proc_address
{
	CDynamicLinkLib*				p_dll;			//��

	ND_INIT							p_init;
	ND_QUIT							p_quit;

	ND_GET_INFO						p_get_info;

	ND_GET_DISCOVERED_DEVICE_NUM	p_get_discovered_device_num;
	ND_GET_DISCOVERED_DEVICE_INFO	p_get_discovered_device_info;
	ND_SET_DISCOVERED_DEVICE_INFO	p_set_discovered_device_info;

	ND_ON_TIMER						p_on_timer;

	ND_CREATE						p_create;
	ND_DESTROY						p_destroy;
}ND_DLL_PROC_ADDRESS;

typedef struct _nd_dll_support_info
{
	ND_DLL_PROC_ADDRESS*		pDllProcAddress;//���ؿ��ַ

	ND_LIB_INFO					libInfo;		//��̬������ȡ�Ŀ���Ϣ
}ND_DLL_SUPPORT_INFO;

typedef std::vector<ND_DLL_SUPPORT_INFO*>			ND_DLL_SUPPORT_VEC;	//�洢���ؽ����Ŀ��ļ�
typedef std::map<unsigned long, ND_DLL_SUPPORT_INFO*>	ND_DLL_SUPPORT_MAP;	//����

class CNdLibMan
{
public:
	virtual ~CNdLibMan();
	static CNdLibMan* Instance();

	bool Initial(const char* filepath);
	void Quit();

	//��ȡ�ӿڶ���, ͨ�� [������, ��Ʒ�ͺ�]
	CNetDevice* CreateNetDevice(unsigned long manufacturerId);
	
	//ɾ���ӿڶ���
	bool DestroyNetDevice(CNetDevice* pNetDevice);

	//��ȡ֧�ֵ����г���
	long GetSupportManufacturer(std::vector<NVR_SUPPORT_MANUFACTURER>& vecManufacturer);

	void OnTimer(unsigned long curTime);

	int StartSearchIPCDevice();
	int GetDiscoveredDeviceNum();
	int GetDiscoveredDeviceInfo(ND_DEVICE_INFO *pDeviceInfo, int deviceNum);
	void SetDiscoveredDeviceInfo(ND_DEVICE_NETWORK_INFO *pDeviceNetworkInfo);

	int GetOnvifIpcManufactID(ND_DEVICE_INFO* pDevInfo);
	int SetCurDevicePoeChnnNum( int poe_chnn_num );

	//��������Ŀ��Ŀ¼, �����nd��
	bool Search();

protected:
	CNdLibMan();

	bool SearchDir(const char* path);
	bool AddDll(const char* filename);

protected:
	char*									m_pFilePath;		//nd����·��
	ND_DLL_SUPPORT_VEC						m_dllSupportVec;	//���е������
	std::map<unsigned long, unsigned long>	m_objNetDeviceMap;	//�Ѿ�������NetDevice������Ϣ
};

#endif

//end
