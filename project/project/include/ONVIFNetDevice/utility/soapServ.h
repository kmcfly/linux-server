#ifndef _SOAPSERV_H_
#define _SOAPSERV_H_

#include "OnvifSoapH.h"
#include "OnvifPtzStub.h"//tqh 2013-6-19
#include "RTSP_Common.h"

#ifdef __cplusplus
extern "C"
{
#endif

//typedef  bool boolean;
//#define bool boolean

//typedef enum  EResultType
//{
//	SOAP_INIT_FAULT = 0,
//	SERVER_ADDR_FAULT = 1,
//	PARAMETER_FAULT  = 2,
//	OPERATE_SUCCESS,
//	OPERATE_FAILED,
//	OTHER_FAULT
//};

typedef enum  EResultType
{
	SOAP_INIT_FAULT   = 0,
	SOAP_INIT_SUCCESS = 1,
	SERVER_ADDR_FAULT = 2,
	PARAMETER_FAULT   = 3,
	OPERATE_SUCCESS,
	OPERATE_FAILED,
	OTHER_FAULT
};


enum ENAMESPACETYPE
{
	TDEVICE_DISCOVERY = 0,
	TDEVICE_REMOTE_DISCOVERY,
	TDEVICE_MGT,
	TDEVICE_IMAGING,
	TDEVICE_MEDIA,
	TDEVICE_EVENT,
	TDEVICE_PTZ,
	TDEVICE_ANALYTICS,
	TDEVICE_END,
};

//function declare
extern struct soap *onvif_init_soap(enum ENAMESPACETYPE aNameType,bool bDiscover);
extern void onvif_free_soap(struct soap *in_soap);
extern struct SOAP_ENV__Header *onvif_init_soap_header(struct soap *aSoap,bool bDiscover,char *messageID,char *aUserName,char *aPassKey,bool bDigest,ONVIF_INIT_INPUT_PARAM* pInputParam);
extern struct SOAP_ENV__Header *onvif_init_discover_soap_header(char *messageID);
extern void onvif_free_soap_header(struct SOAP_ENV__Header *aHeader);
extern bool onvif_soap_generate_uuid(char *out_uuid);

extern int Onvif_GetDev_UtcTime( char* servAddr, ONVIF_INIT_INPUT_PARAM* pstInputParam );

#ifdef __cplusplus
}
#endif


class COnvifSoap
{
public:
	COnvifSoap();
	~COnvifSoap();

	int ProduceSoapInfo(enum ENAMESPACETYPE aNameType,bool bWsse,char *aUser,char *aPassKey); 
	int ProduceSoapInfo(enum ENAMESPACETYPE aNameType,bool bWsse,char *aUser,char *aPassKey,ONVIF_PUBLIC_UTC_TIME * aDevUtcTime);

	void ExitSoap(void); 

	int InitSoap(enum ENAMESPACETYPE aNameType,bool bDiscover);
	int InitSoap(enum ENAMESPACETYPE aNameType,bool bDiscover,ONVIF_PUBLIC_UTC_TIME *aDevUtcTime);
	void InitUtcTimeByExist(ONVIF_INIT_INPUT_PARAM* pstDevTime);
	void InitDevUtcTimes( bool bClearUtcTime, char* aServAddr, unsigned int ipcCtrlType );

	void FreeSoap(void);
	int InitSoapHeader(bool bDiscover = false);
	void FreeSoapHeader(void);

	void AddHeaderToSoap(void);

	void AddUsernamePassKey(int bWsse,char *aUsername,char *aPassKey);
	void AddUsernamePassKey(int bWsse,char *aUsername,char *aPassKey,bool bLocalTime);
	void AddMessageId(void);
	void AddMessageId(bool bWsa);
	void AddAction(char *aAction);
	void AddActionTo(char *aActionTo);
	void AddAction(char *aAction,bool bWsa);
	void AddActionTo(char *aActionTo,bool bWsa);

	void AddUserIdPasswd(char *aUserName,char *aPasswd);

public:   
	struct soap *m_pSoap;
	struct SOAP_ENV__Header *m_pHeader;
protected:
private:
	ONVIF_INIT_INPUT_PARAM stOnvifInputParam;

};


#endif