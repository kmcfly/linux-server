/////////////////////////////////////////////////////////////////////////////////////
/***********************************************************************
** ����ͬΪ����Ƽ����޹�˾��������Ȩ����
** ����			: zhl
** ����         : 2011-12-22
** ����         : UpnpMan.h
** �汾��		: 1.0
** ����			: Upnp����
				1: UPNP��ʼ��ʱ��Ҫ�����ӿڸ����������ӿ�������Ϣ
				2: ���ĳ���˿��Ƿ��Ѿ����������ӵ�·������ӳ��
				3: ���������ӵ�·�����ϵ�ĳ���˿�ӳ��ɾ��
				4: ���������ӵ�·���������һ���˿�ӳ��
				5: ��ȡ�������ӵ�·�����Ϲ��ڱ������Ķ˿�ӳ��״̬
				6: ��ȡ�������ڹ�����(WAN)�ϵ�IP��Ϣ
				7: ���Ѿ�ӳ��Ķ˿���Ҫ��ʱ��·����ˢ��ӳ�䣬��ֹ·����������ӳ�䱻����
** �޸ļ�¼		:
***********************************************************************/
#ifndef _UPNP_MAN_H_
#define _UPNP_MAN_H_

#include "ssdp.h"
#include "upnpDevice.h"

typedef enum _port_map_status_
{
	PORT_MAP_NONE		= 0,		//�����ɹ������(��ǰ�˿�û�б�ӳ��)��ɾ������Ӷ˿�ӳ��ɹ�
	PORT_MAP_EXIST		= 1,		//��ǰ�˿��Ѿ�ӳ����·������
	PORT_MAP_ADD_FAIL	= 2,		//���ӳ��ʧ��
	PORT_MAP_DEL_FAIL	= 3,		//ɾ��ӳ��ʧ��
}PORT_MAP_STATUS;

typedef enum _port_type_
{
	PORT_TYPE_NONE	= 0,			//��ͨ�˿�
	PORT_TYPE_DATA	= 1,			//���ݶ˿�
	PORT_TYPE_WEB	= 2,			//HTTP�˿�
}PORT_TYPE;


/*
*����Ѿ�ӳ�䵽·�ɵĶ˿���Ϣ���ڶ˿�ӳ���߳�Ҫ��ʱ�鿴��Щ�˿��Ƿ��Ѿ�������ӳ��(������·��������)��
*�����IPӳ��������˿�(��·������������IP�Ƚ�����ӳ�����)������Ҫ����ӳ��˿ڣ�����ǵ�ǰ�˿���·�����ϵ�ӳ��״̬
*/
typedef struct _port_map_info_
{
	char szNicName[20];				//�ӿ�����
	unsigned short	port;			//�˿�
	unsigned short	notUsed;		//δ��
	unsigned long	mapStatus;		//�˿ڵ�ǰ״̬��ӳ���߳�ÿ����·��������ӳ���ˢ�����״̬��
									//���ӳ��ʧ�ܻ���ΪPORT_MAP_ADD_FAIL, ����˿��Ѿ���ӳ����ΪPORT_MAP_EXIST������ΪPORT_MAP_NONE
	PORT_TYPE		portType;
	unsigned long	localIP;
}PORT_MAP_INFO;

class CUpnpMan 
{
public:
	static CUpnpMan * Instance();
	~CUpnpMan();
	
//	bool Initial(std::list<char *> nicNameList);
	bool Initial();
	void Quit();

	void EnableUpnp(bool bUseUpnp);
	void SetDeviceName(const char * pDeviceName);
	void IPChanged(const char * pNicName);

	bool Start();
	void Stop();
	
	//�鿴����pNicName�Ķ˿�port�Ƿ��Ѿ�ӳ����·������
	unsigned long CheckPortMap(const char * pNicName, unsigned short port);
	//US02��ʹ�ã�����dvrip�鿴��Ӧupnp�Ƿ�ӳ��ɹ�
	
	int CheckPortMap(unsigned long localIP);
	
	//��������pNicName�Ķ˿�port��·�����ϵ�ӳ��
	unsigned long DeletePortMap(const char * pNicName, unsigned short port);
	unsigned long DeletePortMap();

	//������pNicName�Ķ˿�port��ӵ�·�����ϵ�ӳ����
	unsigned long AddPortMap(const char * pNicName, unsigned short port, PORT_TYPE portType);
	void AddPort(const char * pNicName, unsigned short port, PORT_TYPE portType);

	//��ȡ����pNicName��·�����ϵĶ˿�ӳ����Ϣ
	unsigned long GetNicMapInfo(const char * pNicName, std::list<PORT_MAP_INFO> & portMapInfoList);
	//��ȡ����pNicName������IP��ַ.ʹ��·������UPNP���ܻ�ȡ������豸ͨ��pppoe��ʽ������Ҫͨ��������ʽ��ȡIP��ַ
	int GetWanIP(char * pNicName);

private:
	CUpnpMan();

	unsigned long AddPortMap(unsigned long localIp, unsigned short port);
	int AddPortMap(unsigned long localIp, unsigned short port, unsigned long &mapStatus);
	unsigned long DelPortMap(unsigned short port);
	void PouseUpnp(bool bPouse);

	static PUB_THREAD_RESULT PUB_THREAD_CALL PortMapThread(void * pParam);
	void PortMapProc();

	int GetExtIp(char *pIp);
private:
	bool						m_bHasInitial;
	static CUpnpMan *			m_pUpnpMan;

	bool						m_bUseUpnp;
	bool						m_bPouse;
	CPUB_Lock					m_useUpnpLock;

	//std::list<CSsdp *>		m_ssdpList;
	CSsdp						m_ssdp;
	//std::list<char *>			m_nicNameList;

	PUB_thread_t				m_portMapProcID;
	bool						m_bPortMapProc;

	std::list<PORT_MAP_INFO>	m_portMapInfoList;
	CPUB_Lock					m_listLock;

	char						m_deviceName[256];
};
#endif	/*_UPNP_MAN_H_*/

/////////////////////////////////////////////////////////////////////////////////////

