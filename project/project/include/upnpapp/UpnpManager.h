// UpnpManager.h: interface for the CUpnpManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_UPNPMANAGER_H__22A64128_851B_4E46_9D5B_84F87C298D75__INCLUDED_)
#define AFX_UPNPMANAGER_H__22A64128_851B_4E46_9D5B_84F87C298D75__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <list>

//#include "PUB_common.h"
//#include "PUB_Define.h"
extern "C"{
#include "upnp.h"
#include "ixml.h"
}
#include "PUB_common.h"

#define		UPNP_XML_FILE_SAMPLE	"/mnt/mtd/WebServer/upnpsample.xml"
#define		UPNP_ETH0_DEFAULT_FILE_NAME		"tvtupnp_eth0.xml"
#define		UPNP_RAUSB0_DEFAULT_FILE_NAME	"tvtupnp_rausb0.xml"
#define		DIR_PATH_WEBPAGE		"/mnt/mtd/WebServer/"


typedef struct _upnp_recent_event_info
{
	unsigned long	dwIP;
	char			szEventType[128];
	unsigned long	dwLastTick;
}UPNP_RECENT_EVENT_INFO;

typedef struct _upnp_share_memory_info
{
	char			szIFName[12];
	char			szFriendlyName[128];
	unsigned short	nHttpPort;
	unsigned short	nDataPort;
	unsigned short	nRtspPort;
	unsigned short	nUsed;
	unsigned long	dwExternalIp;
}UPNP_SHARE_MEMORY_INFO;

IXML_Node* AddElemtNode(IXML_Document *pDoc, IXML_Node* pNode, const char *pTag, const char *pText);
int GetValueByTag(IXML_Node *pNodeIn, const char *pTag, const char **ppValue, int iIndex, IXML_Node **ppNodeOut = NULL);

class CUpnpManager  
{
public:
	static CUpnpManager* Instance();
	~CUpnpManager();
	bool Start();
	void Stop();
	BOOL IsStart();
	void SetUpnpInfo(const char *pFriendlyName, unsigned short nHttpPort,unsigned short nDataPort, unsigned short nRtspPort);
	BOOL IsChange(const char *pFriendlyName, unsigned short nHttpPort,unsigned short nDataPort, unsigned short nRtspPort);

protected:
	CUpnpManager();

private:
	void UpnpThreadProcess(void);
	static RESULT WINAPI UpnpThread(void*  lpParameter);
	int StartUpnp(const char *pFriendlyName, unsigned short nHttpPort, unsigned short nDataPort, unsigned short nRtspPort);

	int DeviceStart(const char *pFriendlyName, unsigned short nHttpPort, unsigned short nDataPort,
		unsigned short nRtspPort, const char *pIfName = NULL, unsigned short nPort = 0, 
		const char *pWebFileName = NULL, const char *pWebDirPath = NULL);
	int DeviceStop();
	int Run();

	int RefreshXMLFile(const char *pFilePath, const char *pFriendlyName, const char *pIp);
	int HandleAddPortMapping(const struct Upnp_Discovery *pDiscovery);

	int GetActionUrl(const struct Upnp_Discovery *pDiscovery, char *pActionTypeUrl);
	int CheckMappingPort(const struct Upnp_Discovery *pDiscovery, const char *pActionTypeUrl, 
		std::list<unsigned short> *pPortList);
	int DeleteMappingPort(const char *pActionTypeUrl, unsigned short nPort);
	int AddMappingPort(const char *pActionTypeUrl, unsigned short nPort);

	//static	void* HandleSendADThread(void *pParam);
	//int HandleSendADRun();

	static int UpnpCallback(Upnp_EventType EventType, void* pEvent, void* pCookie);	

	//0 map里面没有这个命令，或者有这个命令，并且已经超过了指定的时间
	//-1 map里有这个命令，没有超过指定的时间
	int CheckEventList(const struct Upnp_Discovery *pDiscovery, unsigned long dwTimeout);
protected:
	char		m_szFriendlyName[128];	
	unsigned short	m_nHttpPort;
	unsigned short  m_nDataPort;
	unsigned short  m_nRtspPort;
private:
	bool		m_bStart;	
	char		m_szIp[20];
	char		m_szMac[36];

	PUB_thread_t		m_upnpThread_ID;
	UpnpDevice_Handle	*m_pUpnpDeviceHandle;
	UpnpClient_Handle	*m_pClientHandle;

	unsigned long m_dwRouter;
	std::list<unsigned short>			m_portList;
	std::list<UPNP_RECENT_EVENT_INFO*>	m_recentEventList;

};

#endif // !defined(AFX_UPNPMANAGER_H__22A64128_851B_4E46_9D5B_84F87C298D75__INCLUDED_)
