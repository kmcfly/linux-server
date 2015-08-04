/*******************************************************************************
** Copyright (c) Tongwei Video Technology Co.,Ltd. All rights reserved.				
** AUTHOR		:	ÂæÃûÃÍ	
** DATE			:	2013-02-21
** FILENAME		:	OnvifVideoServ.h
** Version		:	ONVIF 2.1
** DESCRIPTION	:
** Modify Record:
*******************************************************************************/
#ifndef _OnvifVideoServ_H_
#define _OnvifVideoServ_H_


#include "soapServ.h"


const int ONVIF_STREAM_STR_LEN = 128;

//stream
enum EOnvifStreamType
{
	TStream_RTPUnicast = 0, 
	TStream_RTPMulticast = 1,
	TStream_End
};
enum EOnvifTransportProtocolType
{
	T_TransportProtocolUDP = 0, 
	T_TransportProtocolTCP = 1, 
	T_TransportProtocolRTSP = 2, 
	T_TransportProtocolHTTP = 3,
	T_TransportProtocolEnd
};

class COnvifVideo
{
public:
  COnvifVideo();
  ~COnvifVideo();
   
   void InitServParameter(char *servAddr,char *userName,char *password);
   void InitDevUtcTimes( bool bClearUtcTime, char* aServAddr, unsigned int ipcCtrlType );

   void SetStreamSetup(enum EOnvifStreamType aStreamType,enum EOnvifTransportProtocolType aTransportType);

   int  GetStreamUri(char *profileToken,char *aUriResp,unsigned int aDigest);
   int  GetSnapshotUri(char *profileToken,char *aSnapUriResp,unsigned int aDigest);
   int  SetSynchronizationPoint(char *profileToken,unsigned int aDigest); 

   int  StartMulticast(char *profileToken,unsigned int aDigest);
   int  StopMulticast(char *profileToken,unsigned int aDigest);

protected:
	struct MediaUri
	{
		char Uri[256];	/* required element of type xsd:string */
		bool InvalidAfterConnect;	/* required element of type xsd:boolean */
		bool InvalidAfterReboot;	/* required element of type xsd:boolean */
		char Timeout[ONVIF_STREAM_STR_LEN];	/* required element of type xsd:string */
	};

	void SetAppStreamUri(struct MediaUri *appMediaUri,struct tt__MediaUri *deviceMediaUri);
private:

	char m_servAddr[ONVIF_STREAM_STR_LEN];
    
	char m_userName[32];
	char m_passKey[32];

	//char m_profileToken[ONVIF_STREAM_STR_LEN]; 

	struct MediaUri m_mediaUriResp;

	enum EOnvifStreamType m_streamType;
	enum EOnvifTransportProtocolType m_transportProtocolType;

	ONVIF_INIT_INPUT_PARAM stOnvifInputParam;
};


#endif