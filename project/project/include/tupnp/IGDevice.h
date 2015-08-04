////////////////////////////////////////////////////////////////////////
#ifndef _IG_DEVICE_H_
#define _IG_DEVICE_H_

#include <list>
#include "SWL_TypeDefine.h"

#include "tinyxml.h"

typedef struct _element_
{
	char	name[320];
	char	value[320];
}ELEMENT;

typedef struct _argument_
{
	char name[320];
	bool bDirectionIn;
	char relatedStateVariable[320];
	struct _argument_ *pNextArg;
}ARGUMENT;

typedef struct _action_
{
	char name[320];
	ARGUMENT	*pArgument;
	struct _action_ *pNextAction;
}ACTION;

typedef struct _service_
{
	ELEMENT type;
	ELEMENT id;
	ELEMENT crlUrl;
	ELEMENT eventUrl;
	ELEMENT scpdUrl;
	ACTION	*pAction;
	struct _service_ *pNextService;
}SERVICE;


class CIGDevice
{
public:
	CIGDevice();
	~CIGDevice();

	bool Initial(const char *pDeviceXmlInfo);
	void Quit();

	bool Parser(const TiXmlElement *pRootDevice);
	bool ISSupportService(const char *pServiceName);
	const char *GetServiceScpdUrl(const char *pServiceName);

	bool SetScpdInfo(const char *pServiceName, const char *pScpdXmlInfo);

	const char * GetSpecificPortMappingEntry(char *pXML, unsigned long xmlLen, unsigned long remoteHost, unsigned short externalPort, const char *pProtocol);
	const char * AddPortMapping(char *pXML, unsigned long xmlLen, unsigned long remoteHos,unsigned short externelPort, const char *pProtocol, unsigned short internalPort, unsigned long internalClient, bool bEnable, const char *pDescription);
	const char * DeletePortMapping(char *pXML, unsigned long xmlLen, unsigned long remoteHost, unsigned short externalPort, const char *pProtocol);

	const char * GetControlUrl(const char *pServiceName);

	bool GetExtIpEnable();//ªÒ»°Õ‚Õ¯ip
private:
	bool ParserScpdInfo(SERVICE *pService, const char *pScpdXmlInfo);

	SERVICE *GetService(const char *pServiceName);
	ACTION *GetAction(SERVICE *pService, const char *pActionName);

	void ClearElementList();
	void ClearServiceInfo();
private:
	std::list<ELEMENT *>	m_elementList;

	SERVICE					*m_pService;
	CIGDevice *				m_pNextDevice;
};
#endif /*_IG_DEVICE_H_*/
////////////////////////////////////////////////////////////////////////
