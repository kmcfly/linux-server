/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : L
** Date         : 2011-05-05
** Name         : NdLibMan.cpp
** Version      : 1.0
** Description  :
** Modify Record:
1:
***********************************************************************/

#include "NdLibMan.h"
#include <algorithm>

#ifdef __ENVIRONMENT_WIN32__
#include "io.h"
#else
#include <sys/stat.h>
#include <dirent.h>
#endif

CNdLibMan::CNdLibMan()
{
	m_pFilePath = NULL;
}

CNdLibMan::~CNdLibMan()
{

}

CNdLibMan* CNdLibMan::Instance()
{
	static CNdLibMan s_nd_lib_man;

	return &s_nd_lib_man;
}

bool CNdLibMan::Initial(const char* filepath)
{
	m_pFilePath = new char[512];
	memset(m_pFilePath,0,512);

	if (NULL != filepath)
	{
		//����linux�²����ļ�·��,ĩ�˶�û��'\'��'/'
		strncpy(m_pFilePath, filepath, 512);
		int len = strlen(m_pFilePath);
#ifdef __ENVIRONMENT_WIN32__
		if ('/' == m_pFilePath[len - 1] || '\\' == m_pFilePath[len - 1] )
		{
			m_pFilePath[len - 1] = '\0';
		}
		strcat(m_pFilePath, "\\plugin");
#else
		if ('/' == m_pFilePath[len - 1])
		{
			m_pFilePath[len - 1] = '\0';
		}
		strcat(m_pFilePath, "/plugin");
#endif
	}
	else
	{
#ifdef __ENVIRONMENT_WIN32__
		strncpy(m_pFilePath, "plugin", 512);
#else
		strcpy(m_pFilePath, "plugin");
#endif
	}

	return Search();
}

void CNdLibMan::Quit()
{
	if(m_pFilePath)
	{
		delete [] m_pFilePath;
		m_pFilePath = NULL;
	}

	//����ռ��ؿ���Ϣ
	ND_DLL_SUPPORT_VEC::iterator iter;
	for(iter = m_dllSupportVec.begin(); iter != m_dllSupportVec.end(); iter++)
	{
		if((*iter)->pDllProcAddress->p_quit != NULL)
		{
			(*iter)->pDllProcAddress->p_quit();
		}

		CDynamicLinkLib* pDll = (*iter)->pDllProcAddress->p_dll;

		if(pDll->IsOpen())
		{
			pDll->Close();
		}


		delete pDll;
		delete (*iter)->pDllProcAddress;
		delete (*iter);
	}

	assert(m_objNetDeviceMap.size() <= 0);
}

CNetDevice* CNdLibMan::CreateNetDevice(unsigned long manufacturerId)
{
	CNetDevice* pNewNetDevice = NULL;

	//���Ҷ�Ӧ�Ŀ���Ϣ
	ND_DLL_SUPPORT_VEC::iterator iter;
	for(iter = m_dllSupportVec.begin(); iter != m_dllSupportVec.end(); iter++)
	{
		if((*iter)->libInfo.manufacturerId == manufacturerId)
		{
			break;
		}
	}
	if(iter != m_dllSupportVec.end())
	{
		ND_CREATE p_create = (*iter)->pDllProcAddress->p_create;
		
		//��������
		pNewNetDevice = p_create();

		//����Ѿ������Ķ����б�, ���ܴ������ɹ�
		if(pNewNetDevice)
		{
			m_objNetDeviceMap.insert(std::make_pair((unsigned long)(pNewNetDevice), manufacturerId));
		}
		else
		{
			//�������ɹ�������, �������� �� ���product name�������
			printf("(%s.%d)Create net device error.manufacturer id=%d.\n", __FILE__, __LINE__, manufacturerId);
		}
	}

	return pNewNetDevice;
}

bool CNdLibMan::DestroyNetDevice(CNetDevice* pNetDevice)
{
	assert(pNetDevice);

	//�����Ѿ������Ķ����б�
	std::map<unsigned long, unsigned long>::iterator iterKey = m_objNetDeviceMap.find((unsigned long)(pNetDevice));

	if(iterKey != m_objNetDeviceMap.end())
	{
		//���Ҷ�Ӧ�Ŀ���Ϣ
		ND_DLL_SUPPORT_VEC::iterator iter;
		for(iter = m_dllSupportVec.begin(); iter != m_dllSupportVec.end(); iter++)
		{
			if((*iter)->libInfo.manufacturerId == iterKey->second)
			{
				break;
			}
		}
		if(iter != m_dllSupportVec.end())
		{
			ND_DESTROY p_destroy = (*iter)->pDllProcAddress->p_destroy;

			//ɾ������
			p_destroy(pNetDevice);

			//�Ӷ������ɾ��
			m_objNetDeviceMap.erase(iterKey);

			return true;
		}
	}		

	printf("Error.(%s.%d)Destroy net device. Binding channel=%d.\n", __FILE__, __LINE__, pNetDevice->GetBindChannel());
	assert(false); //�������, ��Ӧ��ɾ�����ɹ�
	return false;
}

long CNdLibMan::GetSupportManufacturer(std::vector<NVR_SUPPORT_MANUFACTURER>& vecManufacturer)
{
	long count = 0;

	ND_DLL_SUPPORT_VEC::iterator iter;
	for(iter = m_dllSupportVec.begin(); iter != m_dllSupportVec.end(); iter++)
	{
		NVR_SUPPORT_MANUFACTURER supportManufacturer;
		supportManufacturer.manufacturerId = (*iter)->libInfo.manufacturerId;
		supportManufacturer.bNeedCtrlPort = (*iter)->libInfo.bNeedCtrlPort;
		supportManufacturer.bNeedHttpPort = (*iter)->libInfo.bNeedHttpPort;
		vecManufacturer.push_back(supportManufacturer);
		count++;
	}

	return count;
}

void CNdLibMan::OnTimer(unsigned long curTime)
{
	ND_DLL_SUPPORT_VEC::iterator iter;
	for(iter = m_dllSupportVec.begin(); iter != m_dllSupportVec.end(); iter++)
	{
		(*iter)->pDllProcAddress->p_on_timer(curTime);
	}
}

int CNdLibMan::StartSearchIPCDevice()
{
	ND_DLL_SUPPORT_VEC::iterator iter;
	for(iter = m_dllSupportVec.begin(); iter != m_dllSupportVec.end(); iter++)
	{
		(*iter)->pDllProcAddress->p_get_discovered_device_info( NULL, 0 );
	}

	return 0;
}

int CNdLibMan::GetDiscoveredDeviceNum()
{
	StartSearchIPCDevice();

	int deviceNum = 0;
	ND_DLL_SUPPORT_VEC::iterator iter;
	for(iter = m_dllSupportVec.begin(); iter != m_dllSupportVec.end(); iter++)
	{
		deviceNum += (*iter)->pDllProcAddress->p_get_discovered_device_num();
	}

	return deviceNum;
}

int CNdLibMan::SetCurDevicePoeChnnNum( int poe_chnn_num )
{
	ND_DLL_SUPPORT_VEC::iterator iter;
	ND_DEVICE_INFO stDevInfo;

	// ���õ����豸��poeͨ����Ŀ
	// ��Ϊ�ӿ���ͳһ�ģ����ӽӿڱȽ��鷳������ʹ��ͬһ�ӿڣ����ݲ�����һ������ȡ��ͬ�Ĺ��ܡ�
	memset( &stDevInfo, 0, sizeof(ND_DEVICE_INFO) );
	strcpy( stDevInfo.productModel, "5b5b5b5b" );
	stDevInfo.gateway = poe_chnn_num;
	for(iter = m_dllSupportVec.begin(); iter != m_dllSupportVec.end(); iter++)
	{
		if(MID_ONVIF == (*iter)->libInfo.manufacturerId)
		{
			(*iter)->pDllProcAddress->p_get_discovered_device_info(&stDevInfo, 0xffff);
			break;
		}
	}

	return 0;
}


int CNdLibMan::GetOnvifIpcManufactID(ND_DEVICE_INFO* pDevInfo)
{
	ND_DLL_SUPPORT_VEC::iterator iter;
	ND_DEVICE_INFO stDevInfo;
	int prod_id = MANU_ONVIF_NORMAL;

	// ��ȡIPC�ĳ�������
	// ��Ϊ�ӿ���ͳһ�ģ����ӽӿڱȽ��鷳������ʹ��ͬһ�ӿڣ����ݲ�����һ������ȡ��ͬ�Ĺ��ܡ�
	memset( &stDevInfo, 0, sizeof(ND_DEVICE_INFO) );
	strcpy( stDevInfo.szAddr, pDevInfo->szAddr );
	strcpy( stDevInfo.productModel, "5a5a5a5a" );
	for(iter = m_dllSupportVec.begin(); iter != m_dllSupportVec.end(); iter++)
	{
		if(MID_ONVIF == (*iter)->libInfo.manufacturerId)
		{
			(*iter)->pDllProcAddress->p_get_discovered_device_info(&stDevInfo, 0xffff);
			//��ȡ�豸��������
			prod_id = stDevInfo.netmask;
			break;
		}
	}

	return prod_id;
}

int CNdLibMan::GetDiscoveredDeviceInfo(ND_DEVICE_INFO *pDeviceInfo, int deviceNum)
{
	int retNum = 0;
	int tvtIpcNum = 0;

	ND_DLL_SUPPORT_VEC::iterator iter;
	for(iter = m_dllSupportVec.begin(); iter != m_dllSupportVec.end(); iter++)
	{
		if(MID_TVT == (*iter)->libInfo.manufacturerId)
		{
			tvtIpcNum = (*iter)->pDllProcAddress->p_get_discovered_device_info(pDeviceInfo, deviceNum);
			retNum = tvtIpcNum;
		}
		else
		{
			if(deviceNum > retNum)
			{
				ND_DEVICE_INFO *pTempDeviceInfo = pDeviceInfo+retNum;
				int tempNum = (*iter)->pDllProcAddress->p_get_discovered_device_info(pTempDeviceInfo, deviceNum-retNum);
				for(int i = 0; i < tempNum; i++)
				{
					int j = 0;
					for(; j < tvtIpcNum; j++)
					{
						//ONVIFЭ���������TVTЭ��������ͬΪ��IPCAM�ˣ�����Ե�
						if(strcmp(pTempDeviceInfo[i].szAddr, pDeviceInfo[j].szAddr) == 0)
						{
							break;
						}
					}
					if(j == tvtIpcNum)
					{
						pDeviceInfo[retNum++] = pTempDeviceInfo[i];
					}
				}
			}
			else
			{
				break;
			}
		}
	}

	return retNum;
}

void CNdLibMan::SetDiscoveredDeviceInfo(ND_DEVICE_NETWORK_INFO *pDeviceNetworkInfo)
{
	ND_DLL_SUPPORT_VEC::iterator iter;
	for(iter = m_dllSupportVec.begin(); iter != m_dllSupportVec.end(); iter++)
	{
		if (NULL != (*iter)->pDllProcAddress->p_set_discovered_device_info)
		{
			(*iter)->pDllProcAddress->p_set_discovered_device_info(pDeviceNetworkInfo);
		}
	}
}

bool CNdLibMan::Search()
{
	//����ռ��ؿ���Ϣ
	ND_DLL_SUPPORT_VEC::iterator iter;
	for(iter = m_dllSupportVec.begin(); iter != m_dllSupportVec.end(); iter++)
	{
		CDynamicLinkLib* pDll = (*iter)->pDllProcAddress->p_dll;
		
		if(pDll->IsOpen())
		{
			pDll->Close();
		}

		delete pDll;
		delete (*iter)->pDllProcAddress;
		delete (*iter);
	}

	//�������������п�
	return SearchDir(m_pFilePath);
}

bool CNdLibMan::SearchDir(const char* path)
{
	bool flag = false;

#ifdef __ENVIRONMENT_WIN32__
	char filepath[512];
	strcpy(filepath, path);
	strcat(filepath, "\\*.dll");

	//���Ȳ���dir�з���Ҫ����ļ�
	long hFile;
	struct _finddata_t fileinfo;
	if((hFile = _findfirst(filepath, &fileinfo)) != -1)
	{
		do
		{
			//����ǲ���Ŀ¼
			//�������,����д���
			if (!(fileinfo.attrib & _A_SUBDIR))
			{
				snprintf(filepath, 512, "%s\\%s", path, fileinfo.name);
				if(AddDll(filepath))
				{
					flag = true;	//�����ҵ�һ���򷵻�true
				}
			}
		} while(_findnext(hFile, &fileinfo) == 0);
		_findclose(hFile);
	}
#else
	struct dirent* ptr=NULL;	
	DIR* dir = NULL;

	dir=opendir(path); 
	if(NULL == dir)
	{
		return false;
	}

	while(NULL != (ptr = readdir(dir)))
	{
		if((0 == strcmp(ptr->d_name, "."))||(0 == strcmp(ptr->d_name, "..")))
		{
			continue;
		}
		char filepath[512] = {0};
		snprintf(filepath, 512, "%s/%s", path, ptr->d_name);
		if(AddDll(filepath))
		{
			flag = true;
		}		
	}
	closedir(dir);
#endif

	return flag;
}

bool CNdLibMan::AddDll(const char* filename)
{
#ifndef __ENVIRONMENT_WIN32__
	//����׺��.so
	char ext[16] = {0};
	strncpy(ext, filename + strlen(filename) - 3, 3);
	if(0 != strcmp(ext, ".so"))
	{
		return false;
	}
#endif

	bool flag = false;

	CDynamicLinkLib* pNewDll = new CDynamicLinkLib();
	
	ND_DLL_PROC_ADDRESS* pProcAddress = new ND_DLL_PROC_ADDRESS;
	memset(pProcAddress, 0, sizeof(ND_DLL_PROC_ADDRESS));

	ND_DLL_SUPPORT_INFO* pDllSupportInfo = new ND_DLL_SUPPORT_INFO;
	memset(&(pDllSupportInfo->libInfo), 0, sizeof(ND_LIB_INFO));
	pDllSupportInfo->pDllProcAddress = pProcAddress;

	//��ȡ������ַ
	if(pNewDll->Open(filename))
	{
		pProcAddress->p_dll = pNewDll;

		pProcAddress->p_init = (ND_INIT)pNewDll->GetProcAddress(ND_INIT_NAME);
		pProcAddress->p_quit = (ND_QUIT)pNewDll->GetProcAddress(ND_QUIT_NAME);
		pProcAddress->p_get_info = (ND_GET_INFO)pNewDll->GetProcAddress(ND_GET_INFO_NAME);
		pProcAddress->p_get_discovered_device_num = (ND_GET_DISCOVERED_DEVICE_NUM)pNewDll->GetProcAddress(ND_GET_DISCOVERED_DEVICE_NUM_NAME);
		pProcAddress->p_get_discovered_device_info = (ND_GET_DISCOVERED_DEVICE_INFO)pNewDll->GetProcAddress(ND_GET_DISCOVERED_DEVICE_INFO_NAME);
		pProcAddress->p_set_discovered_device_info = (ND_SET_DISCOVERED_DEVICE_INFO)pNewDll->GetProcAddress(ND_SET_DISCOVERED_DEVICE_INFO_NAME);

		pProcAddress->p_on_timer = (ND_ON_TIMER)pNewDll->GetProcAddress(ND_ON_TIMER_NAME);
		pProcAddress->p_create = (ND_CREATE)pNewDll->GetProcAddress(ND_CREATE_NAME);
		pProcAddress->p_destroy = (ND_DESTROY)pNewDll->GetProcAddress(ND_DESTROY_NAME);
	
		if( pProcAddress->p_init && 
			pProcAddress->p_quit &&
			pProcAddress->p_get_info &&
			pProcAddress->p_get_discovered_device_num &&
			pProcAddress->p_get_discovered_device_info &&
			pProcAddress->p_on_timer &&
			pProcAddress->p_create && 
			pProcAddress->p_destroy)
		{
			//���ʼ��
			if(0 == pProcAddress->p_init())
			{
				if(0 == pProcAddress->p_get_info(&(pDllSupportInfo->libInfo)))
				{
					if(MID_UNKNOWN == pDllSupportInfo->libInfo.manufacturerId)
					{
						//��˾���Ʋ�����, ��Ϊ����ʧ��
						pProcAddress->p_quit();
					}
					else
					{
						flag = true;
					}
				}
			}
		}
	}

	if(flag)
	{
		printf("Ok. (%s.%d)Load dynamic link lib ok.manufacturer=\"%d\",path=%s.\n", __FILE__, __LINE__, pDllSupportInfo->libInfo.manufacturerId, filename);

		if(MID_TVT == pDllSupportInfo->libInfo.manufacturerId)
		{
			m_dllSupportVec.insert(m_dllSupportVec.begin(), pDllSupportInfo);
		}
		else
		{
			m_dllSupportVec.push_back(pDllSupportInfo);
		}
	}
	else
	{
		printf("Error. (%s.%d)Load dynamic link lib error.manufacturer=\"%d\",.path=%s.\n", __FILE__, __LINE__, pDllSupportInfo->libInfo.manufacturerId, filename);

		if(pNewDll->IsOpen())
		{
			pNewDll->Close();
		}

		delete pNewDll;
		pNewDll = NULL;

		delete pProcAddress;
		pProcAddress = NULL;

		delete pDllSupportInfo;
		pDllSupportInfo = NULL;
	}

	return flag;
}

//end
