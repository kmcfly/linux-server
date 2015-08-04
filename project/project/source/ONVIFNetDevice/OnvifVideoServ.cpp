#include "OnvifVideoServ.h"



COnvifVideo::COnvifVideo()
{ 

	memset(m_servAddr,'\0',sizeof(m_servAddr));

	memset(m_userName,'\0',sizeof(m_userName));
	memset(m_passKey,'\0',sizeof(m_passKey));


	m_streamType = TStream_RTPUnicast;
	m_transportProtocolType = T_TransportProtocolRTSP;

	memset( &stOnvifInputParam, 0, sizeof( ONVIF_INIT_INPUT_PARAM));

}

COnvifVideo::~COnvifVideo()
{

}

void COnvifVideo::InitServParameter(char *servAddr,char *userName,char *password)
{
   if(servAddr == NULL || userName == NULL || password == NULL)
   {
	   return;
   }
   strcpy(m_servAddr,servAddr);

   strcpy(m_userName,userName);
   strcpy(m_passKey,password);
   
}

void COnvifVideo::InitDevUtcTimes( bool bClearUtcTime, char* aServAddr, unsigned int ipcCtrlType )
{
	if( bClearUtcTime )
	{
		memset( &stOnvifInputParam, 0, sizeof( ONVIF_INIT_INPUT_PARAM));
		return;
	}

	if( ONVIF_CTRL_DEV_USED_UTC_TIEM == ipcCtrlType )
	{
		Onvif_GetDev_UtcTime(aServAddr,&stOnvifInputParam);
	}
}


void COnvifVideo::SetStreamSetup(enum EOnvifStreamType aStreamType,enum EOnvifTransportProtocolType aTransportType)
{
	if(aStreamType >= TStream_End)
	{
		return;
	}

	if(aTransportType >= T_TransportProtocolEnd)
	{
		return;
	}

	m_streamType = aStreamType;
	m_transportProtocolType = aTransportType;
}

int  COnvifVideo::GetStreamUri(char *profileToken,char *aUriResp,unsigned int aDigest)
{
	int result = -1;

	enum EResultType eResult= OTHER_FAULT;
	struct soap *pSoap=NULL;
    struct SOAP_ENV__Header *pHeader =NULL;

	struct _trt__GetStreamUri GetStreamUri; 
	struct _trt__GetStreamUriResponse response;

	struct tt__StreamSetup deviceStreamSetup;

	struct tt__Transport  devTransport;

	deviceStreamSetup.Transport = NULL;
	deviceStreamSetup.__size = 0;
	deviceStreamSetup.__any =NULL;
	deviceStreamSetup.__anyAttribute = NULL;
	devTransport.Tunnel = NULL;

	if(m_servAddr[0] == '\0')
	{
		return SERVER_ADDR_FAULT;
	}

	if(profileToken[0] == '\0' || profileToken == NULL)
	{
		return PARAMETER_FAULT;
	}

	if(aUriResp == NULL)
	{
		return PARAMETER_FAULT;
	}

	if(m_streamType == TStream_RTPUnicast)
	{
		deviceStreamSetup.Stream = tt__StreamType__RTP_Unicast;
	}

	if(m_streamType == TStream_RTPMulticast)
	{
		deviceStreamSetup.Stream = tt__StreamType__RTP_Multicast;
	}

	if(m_transportProtocolType == T_TransportProtocolUDP)
	{
		devTransport.Protocol = tt__TransportProtocol__UDP;
	}
	if(m_transportProtocolType == T_TransportProtocolTCP)
	{
		devTransport.Protocol = tt__TransportProtocol__TCP;
	}
	if(m_transportProtocolType == T_TransportProtocolRTSP)
	{
		devTransport.Protocol = tt__TransportProtocol__RTSP;
	}
	if(m_transportProtocolType == T_TransportProtocolHTTP)
	{
		devTransport.Protocol = tt__TransportProtocol__HTTP;
	}

	deviceStreamSetup.Transport = &devTransport;

	GetStreamUri.StreamSetup = &deviceStreamSetup;
	GetStreamUri.ProfileToken = profileToken;
	response.MediaUri =NULL;

	pSoap = onvif_init_soap(TDEVICE_MEDIA,false);
	if(pSoap == NULL)
	{
		return SOAP_INIT_FAULT;
	}
	if(aDigest == 0)
	{
		pHeader = onvif_init_soap_header(pSoap,false,NULL,m_userName,m_passKey,false,&stOnvifInputParam);
	}
	else
	{
		pHeader = onvif_init_soap_header(pSoap,false,NULL,m_userName,m_passKey,true,&stOnvifInputParam);
	}
	if(pHeader == NULL)
	{
		onvif_free_soap(pSoap);
		return SOAP_INIT_FAULT;
	}
	pSoap->header = pHeader;

	result = soap_call___trt__GetStreamUri(pSoap, m_servAddr, NULL,&GetStreamUri,&response);

	if(SOAP_OK == result)
	{
		if(response.MediaUri != NULL)
		{
			if(response.MediaUri->Uri != NULL)
			{
				strcpy(aUriResp,response.MediaUri->Uri);
			}

			SetAppStreamUri(&m_mediaUriResp,response.MediaUri);
		}
		eResult = OPERATE_SUCCESS;		
	}
	else
	{
		eResult = OPERATE_FAILED;		
	}
    
	pSoap->header = NULL;
	onvif_free_soap_header(pHeader);
	pHeader = NULL;

	onvif_free_soap(pSoap);
	pSoap =NULL;
	return eResult; 
}

int COnvifVideo::GetSnapshotUri(char *profileToken,char *aSnapUriResp,unsigned int aDigest)
{
	int result = -1;
	enum EResultType eResult= OTHER_FAULT;
	struct soap *pSoap=NULL;
    struct SOAP_ENV__Header *pHeader =NULL;

	struct _trt__GetSnapshotUri SnapshotUri;
	struct _trt__GetSnapshotUriResponse response;

	if(m_servAddr[0] == '\0')
	{
		return SERVER_ADDR_FAULT;
	}
	if(profileToken[0] == '\0'|| aSnapUriResp == NULL|| profileToken == NULL)
	{
		return PARAMETER_FAULT;
	}

	SnapshotUri.ProfileToken = profileToken;
	response.MediaUri = NULL;

	pSoap = onvif_init_soap(TDEVICE_MEDIA,false);
	if(pSoap == NULL)
	{
		return SOAP_INIT_FAULT;
	}

	if(aDigest == 0)
	{
		pHeader = onvif_init_soap_header(pSoap,false,NULL,m_userName,m_passKey,false,&stOnvifInputParam);
	}
	else
	{
		pHeader = onvif_init_soap_header(pSoap,false,NULL,m_userName,m_passKey,true,&stOnvifInputParam);
	}
	if(pHeader == NULL)
	{
		onvif_free_soap(pSoap);
		return SOAP_INIT_FAULT;
	}
	pSoap->header = pHeader;

	result =soap_call___trt__GetSnapshotUri(pSoap, m_servAddr, NULL,&SnapshotUri,&response);
	if(SOAP_OK == result)
	{
		if(response.MediaUri->Uri != NULL)
		{
			strcpy(aSnapUriResp,response.MediaUri->Uri);
		}
		SetAppStreamUri(&m_mediaUriResp,response.MediaUri);
		eResult = OPERATE_SUCCESS;
	}
	else
	{
		eResult = OPERATE_FAILED;
	}
	onvif_free_soap_header(pHeader);
	pHeader = NULL;
	onvif_free_soap(pSoap);
	pSoap =NULL;
	return eResult; 
}

int COnvifVideo::StartMulticast(char *profileToken,unsigned int aDigest)
{
	int result = -1;

	enum EResultType eResult= OTHER_FAULT;
	struct soap *pSoap=NULL;
    struct SOAP_ENV__Header *pHeader =NULL;

	struct _trt__StartMulticastStreaming streaming;
	struct _trt__StartMulticastStreamingResponse response;

	if(m_servAddr[0] == '\0')
	{
		return SERVER_ADDR_FAULT;
	}
	if(profileToken[0] == '\0'|| profileToken == NULL)
	{
		return PARAMETER_FAULT;
	}

	streaming.ProfileToken = profileToken;
	response.dummy = 0;

	pSoap = onvif_init_soap(TDEVICE_MEDIA,false);
	if(pSoap == NULL)
	{
		return SOAP_INIT_FAULT;
	}
	if(aDigest == 0)
	{
		pHeader = onvif_init_soap_header(pSoap,false,NULL,m_userName,m_passKey,false,&stOnvifInputParam);
	}
	else
	{
		pHeader = onvif_init_soap_header(pSoap,false,NULL,m_userName,m_passKey,true,&stOnvifInputParam);
	}
	if(pHeader == NULL)
	{
		onvif_free_soap(pSoap);
		return SOAP_INIT_FAULT;
	}
	pSoap->header = pHeader;

	result =soap_call___trt__StartMulticastStreaming(pSoap, m_servAddr, NULL,&streaming,&response);
	if(SOAP_OK == result)
	{
		eResult = OPERATE_SUCCESS;
	}
	else
	{
		eResult = OPERATE_FAILED;
	}
	onvif_free_soap_header(pHeader);
	pHeader = NULL;
	onvif_free_soap(pSoap);
	pSoap =NULL;
	return eResult;
}

int COnvifVideo::StopMulticast(char *profileToken,unsigned int aDigest)
{
	int result = -1;
	enum EResultType eResult= OTHER_FAULT;
	struct soap *pSoap=NULL;
    struct SOAP_ENV__Header *pHeader =NULL;

	struct _trt__StopMulticastStreaming streaming;
	struct _trt__StopMulticastStreamingResponse response;

	if(m_servAddr[0] == '\0')
	{
		return SERVER_ADDR_FAULT;
	}
	if(profileToken[0] == '\0'|| profileToken == NULL)
	{
		return PARAMETER_FAULT;
	}

	streaming.ProfileToken = profileToken;

	pSoap = onvif_init_soap(TDEVICE_MEDIA,false);
	if(pSoap == NULL)
	{
		return SOAP_INIT_FAULT;
	}
	if(aDigest == 0)
	{
		pHeader = onvif_init_soap_header(pSoap,false,NULL,m_userName,m_passKey,false,&stOnvifInputParam);
	}
	else
	{
		pHeader = onvif_init_soap_header(pSoap,false,NULL,m_userName,m_passKey,true,&stOnvifInputParam);
	}
	if(pHeader == NULL)
	{
		onvif_free_soap(pSoap);
		return SOAP_INIT_FAULT;
	}
	pSoap->header = pHeader;
	response.dummy = 0;

	result =soap_call___trt__StopMulticastStreaming(pSoap, m_servAddr,NULL,&streaming,&response);
	if(SOAP_OK == result)
	{
		eResult = OPERATE_SUCCESS;
	}
	else
	{
		eResult = OPERATE_FAILED;
	}
	onvif_free_soap_header(pHeader);
	pHeader = NULL;
	onvif_free_soap(pSoap);
	pSoap =NULL;
	return eResult; 
}

int COnvifVideo::SetSynchronizationPoint(char *profileToken,unsigned int aDigest)
{
	int result = -1;

	enum EResultType eResult= OTHER_FAULT;
	struct soap *pSoap=NULL;
    struct SOAP_ENV__Header *pHeader =NULL;

	struct _trt__SetSynchronizationPoint point;
	struct _trt__SetSynchronizationPointResponse response;

	point.ProfileToken =NULL;
	response.dummy = 0;
	if(m_servAddr[0] == '\0')
	{
		return SERVER_ADDR_FAULT;
	}
	if(profileToken[0] == '\0'|| profileToken == NULL)
	{
		return PARAMETER_FAULT;
	}

	point.ProfileToken = profileToken;
	response.dummy = 0;

	pSoap = onvif_init_soap(TDEVICE_MEDIA,false);
	if(pSoap == NULL)
	{
		return SOAP_INIT_FAULT;
	}
	if(aDigest == 0)
	{
		pHeader = onvif_init_soap_header(pSoap,false,NULL,m_userName,m_passKey,false,&stOnvifInputParam);
	}
	else
	{
		pHeader = onvif_init_soap_header(pSoap,false,NULL,m_userName,m_passKey,true,&stOnvifInputParam);
	}
	if(pHeader == NULL)
	{
		onvif_free_soap(pSoap);
		return SOAP_INIT_FAULT;
	}
	pSoap->header = pHeader;

	result =soap_call___trt__SetSynchronizationPoint(pSoap, m_servAddr,NULL,&point,&response);

	if(SOAP_OK == result)
	{
		eResult = OPERATE_SUCCESS;
	}
	else
	{
		eResult = OPERATE_FAILED;
	}

	onvif_free_soap_header(pHeader);
	pHeader = NULL;
	onvif_free_soap(pSoap);
	pSoap =NULL;
	return eResult; 
}

//other function

void COnvifVideo::SetAppStreamUri(struct MediaUri *appMediaUri,struct tt__MediaUri *deviceMediaUri)
{
	if(appMediaUri == NULL ||  deviceMediaUri == NULL)
	{
		return;
	}
	if(deviceMediaUri->Uri != NULL)
	{
		strcpy(appMediaUri->Uri,deviceMediaUri->Uri);
	}
	if(deviceMediaUri->InvalidAfterConnect == _true)
	{
		appMediaUri->InvalidAfterConnect = true;
	}
	else
	{
		appMediaUri->InvalidAfterConnect = false;
	}
	if(deviceMediaUri->InvalidAfterReboot == _true)
	{
		appMediaUri->InvalidAfterReboot = true;
	}
	else
	{
		appMediaUri->InvalidAfterReboot = false;
	}
	if(deviceMediaUri->Timeout != NULL)
	{
		strcpy(appMediaUri->Timeout,deviceMediaUri->Timeout);
	}
	else
	{
		appMediaUri->Timeout[0] = '\0';
	}
}