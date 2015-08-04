////////////////////////////////////////////////////////////////////////////////////
#include "upnpSock.h"
#include "upnpType.h"
#include "uhttp.h"

class CSsdp
{
public:
	CSsdp();
	~CSsdp();

	bool Initial(unsigned short lport);
	void Quit();
	
	bool SetUserAgent(const char *pUserAgent);

	int SendSsdpMsg(SSDP_MSG_TYPE ssdpMsgType);
	const char * GetDeviceLocation(){return m_pDeviceLocation;}
protected:
	bool	m_hasInitial;
private:
	int GetDeviceInfo();

	CUpnpSock	*m_pUpnpSock;
	CUHttp		*m_pHttp;

	char		*m_pUserAgent;				//±¾µØ

	char		*m_pDeviceLocation;
	int			m_maxAge;
};
////////////////////////////////////////////////////////////////////////////////////


