#include "OnvifDevConfServ.h"
//#include "NetDeviceDefine.h"
//#include "OnvifDevConfStub.h"

COnvifDevConf::COnvifDevConf()
{
   m_userName[0] ='\0';
   m_passWord[0] = '\0';

   m_mediaServAddr[0] = '\0';
   m_eventServAddr[0] = '\0';

   memset( &stOnvifInputParam, 0, sizeof( ONVIF_INIT_INPUT_PARAM));

}

COnvifDevConf::COnvifDevConf(char *userName,char *passkey)
{
    if(userName == NULL || passkey == NULL)
	{
		m_userName[0] = '\0';
		m_passWord[0] = '\0';
		return;
	}
	strcpy(m_userName,userName);
	strcpy(m_passWord,passkey);

	memset( &stOnvifInputParam, 0, sizeof( ONVIF_INIT_INPUT_PARAM));
}

COnvifDevConf::~COnvifDevConf()
{
}


void COnvifDevConf::InitDevMediaParameter(char *mediaAddr,char *userName,char *passKey)
{
	if(userName == NULL || passKey == NULL)
	{
		m_userName[0] = '\0';
		m_passWord[0] = '\0';
		return;
	}
	strcpy(m_userName,userName);
	strcpy(m_passWord,passKey);

	if(mediaAddr == NULL)
	{
		m_mediaServAddr[0] = '\0';
		return;
	}
	strcpy(m_mediaServAddr,mediaAddr);

}

void COnvifDevConf::InitDevEventParameter(char *eventAddr,char *userName,char *passKey)
{
	if(userName == NULL || passKey == NULL)
	{
		m_userName[0] = '\0';
		m_passWord[0] = '\0';
		return;
	}

	strcpy(m_userName,userName);
	strcpy(m_passWord,passKey);

	if(eventAddr == NULL)
	{
		m_eventServAddr[0] = '\0';
		return;
	}
	strcpy(m_eventServAddr,eventAddr);
}

void COnvifDevConf::InitDevUtcTimes( bool bClearUtcTime, char* aServAddr, unsigned int ipcCtrlType )
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


int  COnvifDevConf::GetEventProperty(bool &eventEnable,unsigned int aDigest,int manuProc)
{
	int result = -1;
	enum EResultType eResult= OTHER_FAULT;
	struct soap *pSoap=NULL;
	struct SOAP_ENV__Header *pHeader =NULL;


	struct _tev__GetEventProperties EventProperties;
	struct _tev__GetEventPropertiesResponse eventPropertiesResp;
    
	EventProperties.dummy = 0;
	memset(&eventPropertiesResp,0,sizeof(struct _tev__GetEventPropertiesResponse));
	
	if(m_eventServAddr[0] == '\0')
	{
		return SERVER_ADDR_FAULT;
	}

	pSoap = onvif_init_soap(TDEVICE_EVENT,false);
	if(pSoap == NULL)
	{
		return SOAP_INIT_FAULT;
	}
	if(aDigest == 0)
	{
		pHeader = onvif_init_soap_header(pSoap,false,NULL,m_userName,m_passWord,false,&stOnvifInputParam);
	}
	else
	{
		pHeader = onvif_init_soap_header(pSoap,false,NULL,m_userName,m_passWord,true,&stOnvifInputParam);
	}
	if(pHeader == NULL)
	{
		onvif_free_soap(pSoap);
		return SOAP_INIT_FAULT;
	}
	pSoap->header = pHeader;
    
	result = soap_call___dee__GetEventProperties(pSoap,m_eventServAddr,NULL,&EventProperties,&eventPropertiesResp);

  	if(SOAP_OK == result)
	{
		if( eventPropertiesResp.wsnt__FixedTopicSet == _true )        
		{
			eventEnable = true;
		}
		else
		{	
			if (NULL != eventPropertiesResp.wstop__TopicSet)
			{
				eventEnable = true;
			}else{
				eventEnable = false;
			}
		}
		////这个值，海康 和 景阳处理的不一样，景阳为false,特殊处理下
		//if (MANU_ONVIF_SUNELL == manuProc)
		//{
		//	if( eventPropertiesResp.wsnt__FixedTopicSet == _false )        
		//	{
		//		eventEnable = true;
		//	}
		//}
		eResult = OPERATE_SUCCESS;
	}
	else
	{
		eResult = OPERATE_FAILED;
	}

	if(pSoap->header != NULL)
	{
		pSoap->header = NULL;
	}

	onvif_free_soap_header(pHeader);
	pHeader = NULL;
	onvif_free_soap(pSoap);
	pSoap =NULL;
	return eResult;
 }

//media configuration
int COnvifDevConf::GetMediaProfiles(OnvifMediaProfile *profileResp,unsigned int aDigest)
{
	int result = -1;
	enum EResultType eResult= OTHER_FAULT;
	struct soap *pSoap=NULL;
	struct SOAP_ENV__Header *pHeader =NULL;

	int i = 0;
	int j = 0;
	unsigned len = 0;

	char *tmp1 = NULL;
	char *tmp2  = NULL;
	char tmpBuf[6];
   
	int profileNum = 0;

	struct _trt__GetProfiles GetProfiles;
	struct _trt__GetProfilesResponse Resp;
	struct tt__Profile *Profiles = NULL;
	struct OnvifMediaProData *appProfile = NULL;
	
	GetProfiles.dummy = 0;
	Resp.__sizeProfiles = 0;
	Resp.Profiles = NULL;

	if(m_mediaServAddr[0] == '\0')
	{
		return SERVER_ADDR_FAULT;
	}
   
	if(profileResp == NULL)
	{
		return PARAMETER_FAULT;
	}

	pSoap = onvif_init_soap(TDEVICE_MEDIA,false);
	if(pSoap == NULL)
	{
		return SOAP_INIT_FAULT;
	}

	if(aDigest == 0)
	{
		pHeader = onvif_init_soap_header(pSoap,false,NULL,m_userName,m_passWord,false,&stOnvifInputParam);
	}
	else
	{
		pHeader = onvif_init_soap_header(pSoap,false,NULL,m_userName,m_passWord,true,&stOnvifInputParam);
	}
	if(pHeader == NULL)
	{
		onvif_free_soap(pSoap);
		return SOAP_INIT_FAULT;
	}
	pSoap->header = pHeader;


	result = soap_call___trt__GetProfiles(pSoap, m_mediaServAddr, NULL, &GetProfiles,&Resp);

	if(SOAP_OK == result)
	{	
		if(Resp.__sizeProfiles > 0)
		{
			profileNum = Resp.__sizeProfiles;
			Profiles = Resp.Profiles;
			if( profileNum > ONVIF_MAX_PROFILE_NUMBER )
			{
				profileNum = ONVIF_MAX_PROFILE_NUMBER;
			}
			profileResp->profileSize = profileNum;
			memset(profileResp->profile,0,profileNum * sizeof(struct OnvifMediaProData));
			appProfile = profileResp->profile;
			i = 0;
			while(i < profileResp->profileSize)
			{			
				appProfile->profileType = TPRO_END;
				if(Profiles->VideoEncoderConfiguration != NULL)
				{
					if(Profiles->VideoEncoderConfiguration->Encoding == tt__VideoEncoding__H264)
					{
						appProfile->profileType = TPRO_H264;
					}

					if(Profiles->VideoEncoderConfiguration->Encoding == tt__VideoEncoding__MPEG4)
					{
						appProfile->profileType = TPRO_MPEG4;
					}

					if(Profiles->VideoEncoderConfiguration->Encoding == tt__VideoEncoding__JPEG)
					{
						appProfile->profileType = TPRO_JPEG;
					}
					if(Profiles->VideoEncoderConfiguration->token != NULL)
					{
						strcpy(appProfile->videoEncoderConfToken,Profiles->VideoEncoderConfiguration->token);
					}
				}

				if(Profiles->VideoSourceConfiguration != NULL)
				{
					if(Profiles->VideoSourceConfiguration->token != NULL)
					{
						strcpy(appProfile->videoSourceConfToken,Profiles->VideoSourceConfiguration->token);
					}

					if(Profiles->VideoSourceConfiguration->SourceToken != NULL)
					{
						strcpy(appProfile->videoSourceToken,Profiles->VideoSourceConfiguration->SourceToken);
					}
					else
					{
						strcpy(appProfile->videoSourceToken,appProfile->videoSourceConfToken);
					}
				}
				else
				{
					appProfile->videoSourceToken[0] = '\0';
                  appProfile->videoSourceConfToken[0] = '\0';
				}
				strcpy(appProfile->profileToken,Profiles->token);


				//analytics parameter
				if(Profiles->VideoAnalyticsConfiguration != NULL)
				{
					if(Profiles->VideoAnalyticsConfiguration->token != NULL)
					{
						strcpy(appProfile->analyticsToken,Profiles->VideoAnalyticsConfiguration->token);
					}

					if(Profiles->VideoAnalyticsConfiguration->AnalyticsEngineConfiguration != NULL && Profiles->VideoAnalyticsConfiguration->AnalyticsEngineConfiguration->AnalyticsModule!= NULL)
					{
						if(Profiles->VideoAnalyticsConfiguration->AnalyticsEngineConfiguration->AnalyticsModule->Parameters != NULL)
						{
							if(Profiles->VideoAnalyticsConfiguration->AnalyticsEngineConfiguration->AnalyticsModule->Parameters->__sizeElementItem >0)
							{
								j = 0;
								while( j< Profiles->VideoAnalyticsConfiguration->AnalyticsEngineConfiguration->AnalyticsModule->Parameters->__sizeElementItem)
								{
									if(Profiles->VideoAnalyticsConfiguration->AnalyticsEngineConfiguration->AnalyticsModule->Parameters->ElementItem != NULL 
										|| Profiles->VideoAnalyticsConfiguration->AnalyticsEngineConfiguration->AnalyticsModule->Parameters->ElementItem->__any != NULL)
									{
										if(NULL != strstr(Profiles->VideoAnalyticsConfiguration->AnalyticsEngineConfiguration->AnalyticsModule->Parameters->ElementItem->__any,"CellLayout"))  
										{
											tmp1 = strstr(Profiles->VideoAnalyticsConfiguration->AnalyticsEngineConfiguration->AnalyticsModule->Parameters->ElementItem->__any,"\"");
											if(tmp1 != NULL)
											{
												tmp1++;
												tmp2 = strstr(tmp1,"\"");
												if(tmp2 != NULL)
												{
													strncpy(tmpBuf,tmp1,tmp2 -tmp1);
													appProfile->motionRow  = atoi(tmpBuf);
												}

												tmp1=NULL;
												memset(tmpBuf,0,sizeof(tmpBuf));
												tmp2++;
												tmp1 = strstr(tmp2,"\"");
												if(tmp1 != NULL)
												{
													tmp1++;
													tmp2 = NULL;
													tmp2 = strstr(tmp1,"\"");
													if(tmp2 != NULL)
													{
														strncpy(tmpBuf,tmp1,tmp2-tmp1);
														appProfile->motionCol  = atoi(tmpBuf);
													}
												}
											} //tmp1 != NULL
											tmp1=NULL;
											tmp2 = NULL;
										}
									}
									j++;
								}
							}
						}

					}
					else
					{
						if( Profiles->VideoAnalyticsConfiguration->__size > 0 )
						{
							for( j = 0; j < Profiles->VideoAnalyticsConfiguration->__size; j++ )
							{
								char *pAnalyticsInfo = Profiles->VideoAnalyticsConfiguration->__any[j];

								tmp1 = strstr( pAnalyticsInfo, "Columns=");
								if( tmp1 )
								{
									tmp1 += 1+strlen("Columns=");
									tmp2 = strstr( tmp1, "Rows=");
									if( tmp2 )
									{
										int xcl = 0;
										int xrw = 0;

										tmp2 += 1+strlen("Rows=");

										sscanf(tmp1, "%d", &xcl);
										sscanf(tmp2, "%d", &xrw);
										
										appProfile->motionRow  = xcl;
										appProfile->motionCol  = xrw;

									}
								}

							}
						}
					}
				} //if analytics

               
				if(Profiles->PTZConfiguration != NULL)
				{
					appProfile->bPTZ = true;
				     appProfile->ptzDefaultInfo = &appProfile->stPtzDevInfo;
					 memset(appProfile->ptzDefaultInfo,0,sizeof(OnvifPTZDevInfo));
					 memset(appProfile->ptzDefaultInfo->nodeToken,'\0',sizeof(appProfile->ptzDefaultInfo->nodeToken));
					 if(Profiles->PTZConfiguration->token != NULL)
						strcpy(appProfile->ptzDefaultInfo->confToken,Profiles->PTZConfiguration->token);
					 if(Profiles->PTZConfiguration->NodeToken != NULL)
					 {
						 strcpy(appProfile->ptzDefaultInfo->nodeToken,Profiles->PTZConfiguration->NodeToken);
					 }
					 if(Profiles->PTZConfiguration->Name != NULL)
						strcpy(appProfile->ptzDefaultInfo->name,Profiles->PTZConfiguration->Name);
                   if(Profiles->PTZConfiguration->DefaultAbsolutePantTiltPositionSpace != NULL)
						 strcpy(appProfile->ptzDefaultInfo->absolutePTSpace,Profiles->PTZConfiguration->DefaultAbsolutePantTiltPositionSpace);
				     if(Profiles->PTZConfiguration->DefaultAbsoluteZoomPositionSpace != NULL)
						strcpy(appProfile->ptzDefaultInfo->absoluteZoomSpace,Profiles->PTZConfiguration->DefaultAbsoluteZoomPositionSpace);
					 if(Profiles->PTZConfiguration->DefaultRelativePanTiltTranslationSpace != NULL)
						strcpy(appProfile->ptzDefaultInfo->relativePTSpace,Profiles->PTZConfiguration->DefaultRelativePanTiltTranslationSpace);
					 if(Profiles->PTZConfiguration->DefaultRelativeZoomTranslationSpace != NULL)
						strcpy(appProfile->ptzDefaultInfo->relativeZoomSpace,Profiles->PTZConfiguration->DefaultRelativeZoomTranslationSpace);
                   if(Profiles->PTZConfiguration->DefaultContinuousPanTiltVelocitySpace != NULL)
						strcpy(appProfile->ptzDefaultInfo->continusePTVSpace,Profiles->PTZConfiguration->DefaultContinuousPanTiltVelocitySpace);
				     if(Profiles->PTZConfiguration->DefaultContinuousZoomVelocitySpace != NULL) 
						strcpy(appProfile->ptzDefaultInfo->continuseZVSpace,Profiles->PTZConfiguration->DefaultContinuousZoomVelocitySpace);
					  if(Profiles->PTZConfiguration->DefaultPTZTimeout != NULL)
						strcpy(appProfile->ptzDefaultInfo->defaultTimeOut,Profiles->PTZConfiguration->DefaultPTZTimeout);
					  if(Profiles->PTZConfiguration->PanTiltLimits != NULL)
					  {
						  if(Profiles->PTZConfiguration->PanTiltLimits->Range != NULL)
						  {
							  if(Profiles->PTZConfiguration->PanTiltLimits->Range->URI != NULL)
								strcpy(appProfile->ptzDefaultInfo->limitPTSpace,Profiles->PTZConfiguration->PanTiltLimits->Range->URI);
							  if(Profiles->PTZConfiguration->ZoomLimits->Range->URI != NULL)
							   strcpy(appProfile->ptzDefaultInfo->limitZSpace,Profiles->PTZConfiguration->ZoomLimits->Range->URI);
						  }  
					  }
					  if(Profiles->PTZConfiguration->DefaultPTZSpeed != NULL)
				     {
						if(Profiles->PTZConfiguration->DefaultPTZSpeed->PanTilt != NULL)
						{
						  appProfile->ptzDefaultInfo->defaultXSpeed = Profiles->PTZConfiguration->DefaultPTZSpeed->PanTilt->x;
						  appProfile->ptzDefaultInfo->defaultYSpeed = Profiles->PTZConfiguration->DefaultPTZSpeed->PanTilt->y;
						  if(Profiles->PTZConfiguration->DefaultPTZSpeed->PanTilt->space != NULL)
							strcpy(appProfile->ptzDefaultInfo->speedPTSpace,Profiles->PTZConfiguration->DefaultPTZSpeed->PanTilt->space);
						}
						if(Profiles->PTZConfiguration->DefaultPTZSpeed->Zoom != NULL)
						{
							appProfile->ptzDefaultInfo->defaultZSpeed  = Profiles->PTZConfiguration->DefaultPTZSpeed->Zoom->x;
							if(Profiles->PTZConfiguration->DefaultPTZSpeed->Zoom->space != NULL)
								strcpy(appProfile->ptzDefaultInfo->speedZSpace,Profiles->PTZConfiguration->DefaultPTZSpeed->Zoom->space);
						}
				     }
				 }
				else
				{
					appProfile->bPTZ  =false;
					appProfile->ptzDefaultInfo = NULL;
				}
				if(Profiles->fixed != NULL)
				{
					if(*(Profiles->fixed))
					{
						appProfile->bFixed = true;
					}
					else
					{
						appProfile->bFixed = false;
					}
				}
				appProfile++;
				Profiles++;
				i++;
			}
		}
		appProfile = NULL;
		Profiles = NULL;
		Resp.Profiles =NULL;
		eResult = OPERATE_SUCCESS;
	}
	else
	{
		eResult = OPERATE_FAILED;
	}
    
    if(pSoap->header != NULL)
	{
		pSoap->header = NULL;
	}
	onvif_free_soap_header(pHeader);
	pHeader = NULL;
	onvif_free_soap(pSoap);
	pSoap =NULL;
	return eResult;
}


int COnvifDevConf::GetVideoEncoder(char *encoderToken,struct OnvifVideoEncoder *videoEncoderResp,unsigned int aDigest)
{
	int result = -1;
	enum EResultType eResult= OTHER_FAULT;
	struct soap *pSoap=NULL;
    struct SOAP_ENV__Header *pHeader =NULL;

	struct _trt__GetVideoEncoderConfiguration Configuration;
	struct _trt__GetVideoEncoderConfigurationResponse Resp;

	struct tt__VideoEncoderConfiguration *confResp =NULL;

	Resp.Configuration = NULL;

	if(m_mediaServAddr[0] == '\0')
	{
		return SERVER_ADDR_FAULT;
	}
	if(encoderToken == NULL || videoEncoderResp == NULL)
	{
		return PARAMETER_FAULT;
	}

	Configuration.ConfigurationToken=encoderToken;

	pSoap = onvif_init_soap(TDEVICE_MEDIA,false);
	if(pSoap == NULL)
	{
		return SOAP_INIT_FAULT;
	}

	if(aDigest == 0)
	{
		pHeader = onvif_init_soap_header(pSoap,false,NULL,m_userName,m_passWord,false,&stOnvifInputParam);
	}
	else
	{
		pHeader = onvif_init_soap_header(pSoap,false,NULL,m_userName,m_passWord,true,&stOnvifInputParam);
	}
	if(pHeader == NULL)
	{
		onvif_free_soap(pSoap);
		return SOAP_INIT_FAULT;
	}
	pSoap->header = pHeader;

	result = soap_call___trt__GetVideoEncoderConfiguration(pSoap, m_mediaServAddr, NULL,&Configuration,&Resp);

	if(SOAP_OK == result)
	{
		confResp = Resp.Configuration;
		strcpy(videoEncoderResp->encoderName,confResp->Name);
		videoEncoderResp->useCount = confResp->UseCount;
		//strcpy(videoEncodingRespParam->token,confResp->token);
		if(confResp->SessionTimeout != NULL)
		{	
			strcpy(videoEncoderResp->sessionTimeout,confResp->SessionTimeout);
		}

		videoEncoderResp->quality = confResp->Quality;
       
		if(confResp->Resolution != NULL)
		{
			videoEncoderResp->resolution.height =confResp->Resolution->Height;
		    videoEncoderResp->resolution.width = confResp->Resolution->Width;
			//modify the value,when the value is near 720p or 1080p
			//near the 1280*720
			//if((videoEncoderResp->resolution.width  == 1216 && videoEncoderResp->resolution.height == 768)
			//	|| (videoEncoderResp->resolution.width  == 1216 && videoEncoderResp->resolution.height == 1024))
			//{
			//		videoEncoderResp->bNearReso = true;
			//}
			if((videoEncoderResp->resolution.width  > 1200 && videoEncoderResp->resolution.width <1300)
				&& (videoEncoderResp->resolution.height  > 700 && videoEncoderResp->resolution.height <= 1024))
			{
				if((videoEncoderResp->resolution.width == 1280 && videoEncoderResp->resolution.height== 720)
                  || (videoEncoderResp->resolution.width == 1280 && videoEncoderResp->resolution.height== 1024)
				  || (videoEncoderResp->resolution.width == 1280 && videoEncoderResp->resolution.height== 960))
				{
					videoEncoderResp->bNearReso = false;
				}
				else
				{
					videoEncoderResp->bNearReso = true;
				}
			}
			else
			{
				videoEncoderResp->bNearReso = false;
			}
			////near the 1920*1080
			//if(videoEncoderResp->resolution.width != 1920)
			//{
			//	if(videoEncoderResp->resolution.width >1900 && videoEncoderResp->resolution.width <2000)
			//	{
			//		videoEncoderResp->bNearReso = true;
			//	}
			//}
		}

		if(confResp->RateControl != NULL)
		{
			videoEncoderResp->bitrate = confResp->RateControl->BitrateLimit;
			videoEncoderResp->encodingInterval = confResp->RateControl->EncodingInterval;
			videoEncoderResp->frameRate = confResp->RateControl->FrameRateLimit;
		}
		if(confResp->Encoding == tt__VideoEncoding__JPEG)
		{
			videoEncoderResp->encoderType = TPRO_JPEG;
		} 
		else if(confResp->Encoding ==tt__VideoEncoding__MPEG4)
		{
			videoEncoderResp->encoderType = TPRO_MPEG4;
			if(confResp->MPEG4 != NULL)
			{
				videoEncoderResp->mpeg4Conf.gopSize = confResp->MPEG4->GovLength;
				if(confResp->MPEG4->Mpeg4Profile == tt__Mpeg4Profile__SP)
				{
					videoEncoderResp->mpeg4Conf.profile = TOfMpeg4_SP;
				}
				else if(confResp->MPEG4->Mpeg4Profile == tt__Mpeg4Profile__ASP)
				{
					videoEncoderResp->mpeg4Conf.profile = TOfMpeg4_ASP;
				}
			}
		}
		else if(confResp->Encoding ==tt__VideoEncoding__H264)
		{
			videoEncoderResp->encoderType = TPRO_H264;
			if(confResp->H264 != NULL)
			{
				videoEncoderResp->h264Conf.gopSize = confResp->H264->GovLength;
				if(confResp->H264->H264Profile == tt__H264Profile__Baseline)
				{
					videoEncoderResp->h264Conf.profile = TOfH264_Baseline;
				}
				if(confResp->H264->H264Profile == tt__H264Profile__Main)
				{
					videoEncoderResp->h264Conf.profile = TOfH264_Main;
				}
				if(confResp->H264->H264Profile == tt__H264Profile__Extended)
				{
					videoEncoderResp->h264Conf.profile = TOfH264_Extended;
				}
				if(confResp->H264->H264Profile == tt__H264Profile__High)
				{
					videoEncoderResp->h264Conf.profile = TOfH264_High;
				}
			}
		}

		if(confResp->Multicast != NULL)
		{
			videoEncoderResp->muticastConf.Port = confResp->Multicast->Port;
			videoEncoderResp->muticastConf.TTL = confResp->Multicast->TTL;
			if(confResp->Multicast->AutoStart == _true)
			{
				videoEncoderResp->muticastConf.autoStart = true;
			}
			else
			{
				videoEncoderResp->muticastConf.autoStart = false;
			}

			if(confResp->Multicast->Address->Type == tt__IPType__IPv4)
			{
				if(confResp->Multicast->Address->IPv4Address != NULL)
				{
					videoEncoderResp->muticastConf.bIpv4 = true;			
					strcpy(videoEncoderResp->muticastConf.ipAddr,*(confResp->Multicast->Address->IPv4Address));
				}
			}
			else
			{
				
				if(confResp->Multicast->Address->IPv6Address != NULL)
				{
					videoEncoderResp->muticastConf.bIpv4 = false;
				    strcpy(videoEncoderResp->muticastConf.ipAddr,*(confResp->Multicast->Address->IPv6Address));
				}
			}

		}
		eResult = OPERATE_SUCCESS;
	}
	else
	{
		eResult = OPERATE_FAILED;
	}
	if(pSoap->header != NULL)
	{
		pSoap->header = NULL;
	}

	onvif_free_soap_header(pHeader);
	pHeader = NULL;
	onvif_free_soap(pSoap);
	pSoap =NULL;
	return eResult;
}

int COnvifDevConf::SetVideoEncoder(char *encoderToken,bool persistenceModifyParam,struct OnvifVideoEncoder *videoEncoderSet,unsigned int aDigest)
{
	int result = -1;
	enum EResultType eResult= OTHER_FAULT;
	struct soap *pSoap=NULL;
   	struct SOAP_ENV__Header *pHeader =NULL;

	char *tmpAddr=NULL;
	struct _trt__SetVideoEncoderConfiguration Configuration;
	struct _trt__SetVideoEncoderConfigurationResponse Response;

	struct tt__VideoEncoderConfiguration confTmp;

	struct tt__VideoResolution Resolution;	
	struct tt__VideoRateControl RateControl;	
	struct tt__Mpeg4Configuration devMPEG4;	
	struct tt__H264Configuration devH264;	
	struct tt__MulticastConfiguration devMulticast;
	struct tt__IPAddress devAddress;

	memset(&devMulticast,0,sizeof(struct tt__MulticastConfiguration));

	Configuration.Configuration = NULL;
	Configuration.ForcePersistence = _false;
	Response.dummy = 0;

	if(m_mediaServAddr[0] == '\0')
	{
		return SERVER_ADDR_FAULT;
	}
    
	if(encoderToken == NULL || videoEncoderSet == NULL)
	{
		return PARAMETER_FAULT;
	}
	confTmp.Name    = videoEncoderSet->encoderName;
	confTmp.UseCount = videoEncoderSet->useCount;
	confTmp.token    =  encoderToken;
    
	Resolution.Height = videoEncoderSet->resolution.height;
	Resolution.Width  = videoEncoderSet->resolution.width;

	confTmp.Quality   = videoEncoderSet->quality;
	
	RateControl.FrameRateLimit    = videoEncoderSet->frameRate;
	RateControl.EncodingInterval  =  videoEncoderSet->encodingInterval;
	RateControl.BitrateLimit      = videoEncoderSet->bitrate;
    
	if(videoEncoderSet->encoderType == TPRO_H264)
	{
		confTmp.Encoding = tt__VideoEncoding__H264;
		devH264.GovLength = videoEncoderSet->h264Conf.gopSize;

		if(videoEncoderSet->h264Conf.profile == TOfH264_Baseline)
		{
			devH264.H264Profile= tt__H264Profile__Baseline;
		}
		else if(videoEncoderSet->h264Conf.profile == TOfH264_Main)
		{
			devH264.H264Profile= tt__H264Profile__Main;	
		}
		else if(videoEncoderSet->h264Conf.profile == TOfH264_Extended)
		{
			devH264.H264Profile= tt__H264Profile__Extended;
		}
		else if(videoEncoderSet->h264Conf.profile == TOfH264_High)
		{
			devH264.H264Profile= tt__H264Profile__High;
		} 
	}
	else if(videoEncoderSet->encoderType == TPRO_MPEG4)
	{
		confTmp.Encoding = tt__VideoEncoding__MPEG4;
		devMPEG4.GovLength = videoEncoderSet->mpeg4Conf.gopSize;
		if(videoEncoderSet->mpeg4Conf.profile == TOfMpeg4_SP)
		{
			devMPEG4.Mpeg4Profile = tt__Mpeg4Profile__SP;
		}
		if(videoEncoderSet->mpeg4Conf.profile == TOfMpeg4_ASP)
		{
			devMPEG4.Mpeg4Profile = tt__Mpeg4Profile__ASP;
		}  
	}
	else if(videoEncoderSet->encoderType == TPRO_JPEG)
	{
		confTmp.Encoding = tt__VideoEncoding__JPEG;
	}

	tmpAddr = videoEncoderSet->muticastConf.ipAddr;
	if(videoEncoderSet->muticastConf.bIpv4)
	{
		devAddress.Type = tt__IPType__IPv4;
		devAddress.IPv4Address =&tmpAddr;
		devAddress.IPv6Address =NULL;
	}
	else
	{
		devAddress.Type = tt__IPType__IPv6;
		devAddress.IPv6Address =&tmpAddr;
		devAddress.IPv4Address =NULL;
	}

	devMulticast.Port = videoEncoderSet->muticastConf.Port;
	devMulticast.TTL = videoEncoderSet->muticastConf.TTL;
	if(videoEncoderSet->muticastConf.autoStart)
	{
		devMulticast.AutoStart = _true;
	}
	else
	{
		devMulticast.AutoStart = _false;
	}
	devMulticast.__any = NULL;
	devMulticast.__anyAttribute = NULL;
	devMulticast.__size = 0;   
	devMulticast.Address = &devAddress;

	confTmp.Resolution = &Resolution;
	confTmp.RateControl = &RateControl;
	if(videoEncoderSet->encoderType == TPRO_MPEG4)
	{
		confTmp.MPEG4 = &devMPEG4;
		confTmp.H264 = NULL;
	}

	if(videoEncoderSet->encoderType == TPRO_H264)
	{
		confTmp.MPEG4= NULL;
		confTmp.H264= &devH264;
	}

	if(videoEncoderSet->encoderType == TPRO_JPEG)
	{
		confTmp.MPEG4 = NULL;
		confTmp.H264 = NULL;
	}

	confTmp.Multicast = &devMulticast;
	confTmp.SessionTimeout = videoEncoderSet->sessionTimeout;
	confTmp.__size = 0;	
	confTmp.__any =NULL;
	confTmp.__anyAttribute = NULL;

	Configuration.Configuration = &confTmp;
	if(persistenceModifyParam)
	{
		Configuration.ForcePersistence = _true;
	}
	else
	{
		Configuration.ForcePersistence = _false;
	}

	pSoap = onvif_init_soap(TDEVICE_MEDIA,false);
	if(pSoap == NULL)
	{
		return SOAP_INIT_FAULT;
	}
	if(aDigest == 0)
	{
		pHeader = onvif_init_soap_header(pSoap,false,NULL,m_userName,m_passWord,false,&stOnvifInputParam);
	}
	else
	{
		pHeader = onvif_init_soap_header(pSoap,false,NULL,m_userName,m_passWord,true,&stOnvifInputParam);
	}
	if(pHeader == NULL)
	{
		onvif_free_soap(pSoap);
		return SOAP_INIT_FAULT;
	}
	pSoap->header = pHeader;

	result =soap_call___trt__SetVideoEncoderConfiguration(pSoap, m_mediaServAddr, NULL,&Configuration,&Response);

	if(SOAP_OK == result)
	{
		eResult = OPERATE_SUCCESS;
	}
	else
	{
		eResult = OPERATE_FAILED;
	}
	if(pSoap->header != NULL)
	{
		pSoap->header = NULL;
	}
	onvif_free_soap_header(pHeader);
	pHeader = NULL;
	onvif_free_soap(pSoap);
	pSoap =NULL;
	return eResult;
}

int COnvifDevConf::GetVideoEncoderRangeParameter(char *profileToken,char *encoderToken,struct OnvifVideoEncoderConfOptions *confOptionsResp,unsigned int aDigest)
{
	int result = -1;
	enum EResultType eResult= OTHER_FAULT;
	struct soap *pSoap=NULL;
	struct SOAP_ENV__Header *pHeader =NULL;

	struct _trt__GetVideoEncoderConfigurationOptions Options;
	struct _trt__GetVideoEncoderConfigurationOptionsResponse OptionsResponse;

	struct tt__VideoEncoderConfigurationOptions *optTmp =NULL;

	Options.ConfigurationToken = NULL;
	Options.ProfileToken = NULL;
	OptionsResponse.Options = NULL;

	if(m_mediaServAddr[0] == '0')
	{
		return SERVER_ADDR_FAULT;
	}

	if(profileToken == NULL || encoderToken == NULL || confOptionsResp == NULL)
	{
		return PARAMETER_FAULT;
	}

	Options.ConfigurationToken = encoderToken;
	Options.ProfileToken = profileToken;

	pSoap = onvif_init_soap(TDEVICE_MEDIA,false);
	if(pSoap == NULL)
	{
		return SOAP_INIT_FAULT;
	}
	if(aDigest == 0)
	{
		pHeader = onvif_init_soap_header(pSoap,false,NULL,m_userName,m_passWord,false,&stOnvifInputParam);
	}
	else
	{
		pHeader = onvif_init_soap_header(pSoap,false,NULL,m_userName,m_passWord,true,&stOnvifInputParam);
	}
	if(pHeader == NULL)
	{
		onvif_free_soap(pSoap);
		return SOAP_INIT_FAULT;
	}
	pSoap->header = pHeader;

	result = soap_call___trt__GetVideoEncoderConfigurationOptions(pSoap, m_mediaServAddr, NULL,&Options,&OptionsResponse);
    
	confOptionsResp->bJpegEnable = false;
	confOptionsResp->bMpegEnable = false;
	confOptionsResp->bH264Enable = false;
	confOptionsResp->bExtensionEnable = false;

	if(SOAP_OK == result)
	{
		optTmp = OptionsResponse.Options;
		if(optTmp->QualityRange != NULL)
		{
			confOptionsResp->QualityRange.max = optTmp->QualityRange->Max;
			confOptionsResp->QualityRange.min = optTmp->QualityRange->Min;
		}

		if(optTmp->JPEG != NULL)
		{
			confOptionsResp->bJpegEnable = true;
			confOptionsResp->JPEG.ResolutionsAvailable = confOptionsResp->stJpegResolu;
			result = SetAppJpegOptions(optTmp->JPEG,&confOptionsResp->JPEG);
		}

		if(optTmp->MPEG4 != NULL)
		{
			confOptionsResp->bMpegEnable = true;
			confOptionsResp->MPEG4.JpegSetting.ResolutionsAvailable = confOptionsResp->stMpeg4Resolu;
			SetAppMpeg4Options(optTmp->MPEG4,&confOptionsResp->MPEG4);
		}

       
		if(optTmp->H264 != NULL)
		{
			confOptionsResp->bH264Enable = true;
			confOptionsResp->H264.JpegSetting.ResolutionsAvailable = confOptionsResp->stH264Resolu;
			SetAppH264Options(optTmp->H264,&confOptionsResp->H264);
		}


		if(optTmp->Extension != NULL)
		{
			confOptionsResp->bExtensionEnable = true;
			confOptionsResp->Extension.JPEG.jpeg.ResolutionsAvailable = confOptionsResp->stExtJpegResolu;
			confOptionsResp->Extension.MPEG4.JpegSetting.jpeg.ResolutionsAvailable = confOptionsResp->stExtMpeg4Resolu;
			confOptionsResp->Extension.H264.JpegSetting.jpeg.ResolutionsAvailable = confOptionsResp->stExtH264Resolu;
			SetAppOptionsExtension(optTmp->Extension,&(confOptionsResp->Extension));
		}

		optTmp = NULL;
		OptionsResponse.Options = NULL;
		eResult = OPERATE_SUCCESS;
	}
	else
	{
		eResult = OPERATE_FAILED;
	}
	if(pSoap->header != NULL)
	{
		pSoap->header = NULL;
	}

	onvif_free_soap_header(pHeader);
	pHeader = NULL;
	onvif_free_soap(pSoap);
	pSoap =NULL;
	return eResult;
}


int COnvifDevConf::SetAppJpegOptions(struct tt__JpegOptions *deviceJPEG,struct OnvifJpegOptions *appJPEG)
{
	int size =0;
	int i = 0;
	struct OnvifResolution *appResolution=NULL;
	struct tt__VideoResolution * deviceResolution=NULL;

	if(deviceJPEG == NULL || appJPEG == NULL)
	{
		return -1;
	}
	if(deviceJPEG->EncodingIntervalRange != NULL)
	{
		appJPEG->EncodingIntervalRange.max = deviceJPEG->EncodingIntervalRange->Max;
		appJPEG->EncodingIntervalRange.min = deviceJPEG->EncodingIntervalRange->Min;
    }
    
	if(deviceJPEG->FrameRateRange != NULL)
	{
		appJPEG->FrameRateRange.max = deviceJPEG->FrameRateRange->Max;
		appJPEG->FrameRateRange.min = deviceJPEG->FrameRateRange->Min;
    }
	size =  deviceJPEG->__sizeResolutionsAvailable;
	if(size > 0)
	{
		if( size > ONVIF_MAX_RESOLUT_COUNT )
		{
			size = ONVIF_MAX_RESOLUT_COUNT;
		}
		appJPEG->__sizeResolutionsAvailable =size;

		i =0;
		appResolution =appJPEG->ResolutionsAvailable;
		deviceResolution = deviceJPEG->ResolutionsAvailable;
		while(i < size)
		{
			appResolution->height = deviceResolution->Height;
			appResolution->width = deviceResolution->Width;
			deviceResolution++;
			appResolution++;
			i++;
		}
	}

	appResolution = NULL;
	deviceResolution = NULL;
	return 1;
}

int COnvifDevConf::SetAppMpeg4Options(struct tt__Mpeg4Options *deviceMPEG4,struct OnvifMpeg4Options *appMPEG4)
{
	struct OnvifResolution *appResolution=NULL;
	struct tt__VideoResolution *deviceResolution =NULL;
	enum tt__Mpeg4Profile *profile = NULL;
	int size = 0;
	int i = 0;

	if(deviceMPEG4 == NULL || appMPEG4 == NULL)
	{
		return -1;
	}
	if(deviceMPEG4->GovLengthRange != NULL)
	{
		appMPEG4->GovLengthRange.max = deviceMPEG4->GovLengthRange->Max;
		appMPEG4->GovLengthRange.min  = deviceMPEG4->GovLengthRange->Min;
	}
	appMPEG4->JpegSetting.__sizeResolutionsAvailable = deviceMPEG4->__sizeResolutionsAvailable;
	if(deviceMPEG4->EncodingIntervalRange != NULL)
	{
		appMPEG4->JpegSetting.EncodingIntervalRange.max = deviceMPEG4->EncodingIntervalRange->Max;
		appMPEG4->JpegSetting.EncodingIntervalRange.min = deviceMPEG4->EncodingIntervalRange->Min;
	}
	if(deviceMPEG4->FrameRateRange != NULL)
	{
		appMPEG4->JpegSetting.FrameRateRange.max = deviceMPEG4->FrameRateRange->Max;
		appMPEG4->JpegSetting.FrameRateRange.min = deviceMPEG4->FrameRateRange->Min;
    }
	i = 0;
	size = deviceMPEG4->__sizeMpeg4ProfilesSupported;
	profile = deviceMPEG4->Mpeg4ProfilesSupported;
	while(i < size)
	{
		if(*profile == tt__Mpeg4Profile__SP)
		{
			appMPEG4->Mpeg4Profile_SP = true;
		}
		else
		{
			appMPEG4->Mpeg4Profile_SP = false;
		}

		if(*profile == tt__Mpeg4Profile__ASP)
		{
			appMPEG4->Mpeg4Profile_ASP = true;
		}
		else
		{        
			appMPEG4->Mpeg4Profile_ASP = false;	
		}
		i++;
		profile++;
	}
    size = 0;
	size = deviceMPEG4->__sizeResolutionsAvailable;
	if(size > 0)
	{
		if( size > ONVIF_MAX_RESOLUT_COUNT )
		{
			size = ONVIF_MAX_RESOLUT_COUNT;
		}

		appMPEG4->JpegSetting.__sizeResolutionsAvailable = size;
		appResolution = appMPEG4->JpegSetting.ResolutionsAvailable;
		deviceResolution = deviceMPEG4->ResolutionsAvailable;
		i =0;
		while(i < size)
		{
			appResolution->height = deviceResolution->Height;
			appResolution->width  = deviceResolution->Width;
			appResolution++;
			deviceResolution++;
			i++;

		}
	}

	appResolution = NULL;
	deviceResolution = NULL;
	return 1;
}

int COnvifDevConf::SetAppH264Options(struct tt__H264Options *deviceH264,struct OnvifH264Options *appH264)
{
	struct OnvifResolution *appResolution=NULL;
	struct tt__VideoResolution *deviceResolution =NULL;
	enum tt__H264Profile *profile = NULL;
	int size = 0;
	int i = 0;

	if(deviceH264 == NULL || appH264 == NULL)
	{
		return -1;
	}
	if(deviceH264->GovLengthRange != NULL)
	{
		appH264->GovLengthRange.max = deviceH264->GovLengthRange->Max;
		appH264->GovLengthRange.min  = deviceH264->GovLengthRange->Min;
	}
	appH264->JpegSetting.__sizeResolutionsAvailable = deviceH264->__sizeResolutionsAvailable;
	if(deviceH264->EncodingIntervalRange != NULL)
	{
		appH264->JpegSetting.EncodingIntervalRange.max = deviceH264->EncodingIntervalRange->Max;
		appH264->JpegSetting.EncodingIntervalRange.min = deviceH264->EncodingIntervalRange->Min;
	}
	if(deviceH264->FrameRateRange != NULL)
	{
		appH264->JpegSetting.FrameRateRange.max = deviceH264->FrameRateRange->Max;
		appH264->JpegSetting.FrameRateRange.min = deviceH264->FrameRateRange->Min;
	}
	i = 0;
	size = deviceH264->__sizeH264ProfilesSupported;
	profile = deviceH264->H264ProfilesSupported;
	while(i < size)
	{
		if(*profile == tt__H264Profile__Baseline)
		{
			appH264->H264Profile_Baseline = true;
		}
		else
		{
			appH264->H264Profile_Baseline = false;
		}

		if(*profile == tt__H264Profile__Main)
		{
			appH264->H264Profile_Main = true;
		}
		else
		{
			appH264->H264Profile_Main = false;
		}
		if(*profile == tt__H264Profile__Extended)
		{
			appH264->H264Profile_Extended = true;
		}
		else
		{
			appH264->H264Profile_Extended = false;
		}

		if(*profile == tt__H264Profile__High)
		{
			appH264->H264Profile_High = true;
		}
		else
		{
			appH264->H264Profile_High = false;
		}

		i++;
		profile++;
	}

	size = deviceH264->__sizeResolutionsAvailable;
	if(size > 0)
	{
		if( size > ONVIF_MAX_RESOLUT_COUNT )
		{
			size = ONVIF_MAX_RESOLUT_COUNT;
		}

		appH264->JpegSetting.__sizeResolutionsAvailable = size;
		appResolution = appH264->JpegSetting.ResolutionsAvailable;
		deviceResolution = deviceH264->ResolutionsAvailable;
		i =0;
		while(i < size)
		{
			appResolution->height = deviceResolution->Height;
			appResolution->width  = deviceResolution->Width;
			appResolution++;
			deviceResolution++;
			i++;

		}
	}

	appResolution = NULL;
	deviceResolution = NULL;

	return 1;
}
int COnvifDevConf::SetAppJpegOptions2(struct tt__JpegOptions2 *deviceJPEG,struct OnvifJpegOptions2 *appJPEG)
{
	int size =0;
	int i = 0;
	struct OnvifResolution *appResolution=NULL;
	struct tt__VideoResolution * deviceResolution=NULL;

	if(deviceJPEG == NULL || appJPEG == NULL)
	{
		return -1;
	}
	appJPEG->jpeg.EncodingIntervalRange.max = deviceJPEG->EncodingIntervalRange->Max;
	appJPEG->jpeg.EncodingIntervalRange.min = deviceJPEG->EncodingIntervalRange->Min;
	appJPEG->jpeg.FrameRateRange.max = deviceJPEG->FrameRateRange->Max;
	appJPEG->jpeg.FrameRateRange.min = deviceJPEG->FrameRateRange->Min;
	appJPEG->BitrateRange.max= deviceJPEG->BitrateRange->Max;
	appJPEG->BitrateRange.min = deviceJPEG->BitrateRange->Min;
	//


	size =  deviceJPEG->__sizeResolutionsAvailable;
	if(size > 0)
	{
		if( size > ONVIF_MAX_RESOLUT_COUNT )
		{
			size = ONVIF_MAX_RESOLUT_COUNT;
		}
		appJPEG->jpeg.__sizeResolutionsAvailable =size;
		i=0;
		appResolution =appJPEG->jpeg.ResolutionsAvailable;
		deviceResolution = deviceJPEG->ResolutionsAvailable;
		while(i < size)
		{
			appResolution->height = deviceResolution->Height;
			appResolution->width = deviceResolution->Width;
			deviceResolution++;
			appResolution++;
			i++;
		}
	}


	appResolution = NULL;
	deviceResolution = NULL;
	return 1;
}

int COnvifDevConf::SetAppMpeg4Options2(struct tt__Mpeg4Options2 *deviceMPEG4,struct OnvifMpeg4Options2 *appMPEG4)
{
	struct OnvifResolution *appResolution=NULL;
	struct tt__VideoResolution *deviceResolution =NULL;
	enum tt__Mpeg4Profile *profile = NULL;
	int size = 0;
	int i = 0;

	if(deviceMPEG4 == NULL || appMPEG4 == NULL)
	{
		return -1;
	}

	if( !deviceMPEG4->GovLengthRange || !deviceMPEG4->EncodingIntervalRange 
		|| !deviceMPEG4->FrameRateRange || !deviceMPEG4->BitrateRange )
	{
		return -1;
	}

	appMPEG4->GovLengthRange.max = deviceMPEG4->GovLengthRange->Max;
	appMPEG4->GovLengthRange.min  = deviceMPEG4->GovLengthRange->Min;
	appMPEG4->JpegSetting.jpeg.__sizeResolutionsAvailable = deviceMPEG4->__sizeResolutionsAvailable;
	appMPEG4->JpegSetting.jpeg.EncodingIntervalRange.max = deviceMPEG4->EncodingIntervalRange->Max;
	appMPEG4->JpegSetting.jpeg.EncodingIntervalRange.min = deviceMPEG4->EncodingIntervalRange->Min;
	appMPEG4->JpegSetting.jpeg.FrameRateRange.max = deviceMPEG4->FrameRateRange->Max;
	appMPEG4->JpegSetting.jpeg.FrameRateRange.min = deviceMPEG4->FrameRateRange->Min;
	appMPEG4->JpegSetting.BitrateRange.max = deviceMPEG4->BitrateRange->Max;
	appMPEG4->JpegSetting.BitrateRange.min = deviceMPEG4->BitrateRange->Min;

	i = 0;
	size = deviceMPEG4->__sizeMpeg4ProfilesSupported;
	profile = deviceMPEG4->Mpeg4ProfilesSupported;
	while(i < size)
	{
		if(*profile == tt__Mpeg4Profile__SP)
		{
			appMPEG4->Mpeg4Profile_SP = true;
		}
		else
		{
			appMPEG4->Mpeg4Profile_SP = false;
		}

		if(*profile == tt__Mpeg4Profile__ASP)
		{
			appMPEG4->Mpeg4Profile_ASP = true;
		}
		else
		{        
			appMPEG4->Mpeg4Profile_ASP = false;	
		}
		i++;
		profile++;
	}
    size = 0;
	size = deviceMPEG4->__sizeResolutionsAvailable;
	if(size > 0)
	{
		if( size > ONVIF_MAX_RESOLUT_COUNT )
		{
			size = ONVIF_MAX_RESOLUT_COUNT;
		}

		appMPEG4->JpegSetting.jpeg.__sizeResolutionsAvailable = size;
		appResolution = appMPEG4->JpegSetting.jpeg.ResolutionsAvailable;
		deviceResolution = deviceMPEG4->ResolutionsAvailable;
		i =0;
		while(i < size)
		{
			appResolution->height = deviceResolution->Height;
			appResolution->width  = deviceResolution->Width;
			appResolution++;
			deviceResolution++;
			i++;

		}
	}

	appResolution = NULL;
	deviceResolution = NULL;
	return 1;
}

int COnvifDevConf::SetAppH264Options2(struct tt__H264Options2 *deviceH264,struct OnvifH264Options2 *appH264)
{
	struct OnvifResolution *appResolution=NULL;
	struct tt__VideoResolution *deviceResolution =NULL;
	enum tt__H264Profile *profile = NULL;
	int size = 0;
	int i = 0;

	if(deviceH264 == NULL || appH264 == NULL)
	{
		return -1;
	}
	if(deviceH264->GovLengthRange != NULL)
	{
		appH264->GovLengthRange.max = deviceH264->GovLengthRange->Max;
		appH264->GovLengthRange.min  = deviceH264->GovLengthRange->Min;
	}
	appH264->JpegSetting.jpeg.__sizeResolutionsAvailable = deviceH264->__sizeResolutionsAvailable;
	if(deviceH264->EncodingIntervalRange != NULL)
	{
		appH264->JpegSetting.jpeg.EncodingIntervalRange.max = deviceH264->EncodingIntervalRange->Max;
	    appH264->JpegSetting.jpeg.EncodingIntervalRange.min = deviceH264->EncodingIntervalRange->Min;
	}
	if(deviceH264->FrameRateRange != NULL)
	{
		appH264->JpegSetting.jpeg.FrameRateRange.max = deviceH264->FrameRateRange->Max;
		appH264->JpegSetting.jpeg.FrameRateRange.min = deviceH264->FrameRateRange->Min;
	}
	if(deviceH264->BitrateRange != NULL)
	{
		appH264->JpegSetting.BitrateRange.max = deviceH264->BitrateRange->Max;
		appH264->JpegSetting.BitrateRange.min = deviceH264->BitrateRange->Min;
	}

	i = 0;
	size = deviceH264->__sizeH264ProfilesSupported;
	profile = deviceH264->H264ProfilesSupported;
	while(i < size)
	{
		if(*profile == tt__H264Profile__Baseline)
		{
			appH264->H264Profile_Baseline = true;
		}
		else
		{
			appH264->H264Profile_Baseline = false;
		}

		if(*profile == tt__H264Profile__Main)
		{
			appH264->H264Profile_Main = true;
		}
		else
		{
			appH264->H264Profile_Main = false;
		}
		if(*profile == tt__H264Profile__Extended)
		{
			appH264->H264Profile_Extended = true;
		}
		else
		{
			appH264->H264Profile_Extended = false;
		}

		if(*profile == tt__H264Profile__High)
		{
			appH264->H264Profile_High = true;
		}
		else
		{
			appH264->H264Profile_High = false;
		}

		i++;
		profile++;
	}

	size = deviceH264->__sizeResolutionsAvailable;
	if(size > 0)
	{
		if( size > ONVIF_MAX_RESOLUT_COUNT )
		{
			size = ONVIF_MAX_RESOLUT_COUNT;
		}
		appH264->JpegSetting.jpeg.__sizeResolutionsAvailable = size;
		appResolution = appH264->JpegSetting.jpeg.ResolutionsAvailable;
		deviceResolution = deviceH264->ResolutionsAvailable;
		i =0;
		while(i < size)
		{
			appResolution->height = deviceResolution->Height;
			appResolution->width  = deviceResolution->Width;
			appResolution++;
			deviceResolution++;
			i++;

		}
	}

	appResolution = NULL;
	deviceResolution = NULL;

	return 1;
}

int COnvifDevConf::SetAppOptionsExtension(struct tt__VideoEncoderOptionsExtension *deviceExtension,struct OnvifVideoEncoderOptionsExtension *appExtension)
{
	struct OnvifJpegOptions2 *appJPEG=NULL;	
	struct OnvifMpeg4Options2 *appMPEG4 = NULL;	
	struct OnvifH264Options2 *appH264 =NULL;

	struct tt__JpegOptions2 *deviceJPEG =NULL;	
	struct tt__Mpeg4Options2 *deviceMPEG4 =NULL;	
	struct tt__H264Options2 *deviceH264 =NULL;	

	int result = -1;

	appJPEG= &appExtension->JPEG;
	appMPEG4= &appExtension->MPEG4;
	appH264 = &appExtension->H264;

	deviceH264=deviceExtension->H264;
	deviceJPEG = deviceExtension->JPEG;
	deviceMPEG4 = deviceExtension->MPEG4;

	result = SetAppH264Options2(deviceH264,appH264);

	result = SetAppJpegOptions2(deviceJPEG,appJPEG);

	result = SetAppMpeg4Options2(deviceMPEG4,appMPEG4);


	appJPEG=NULL;	
	appMPEG4 = NULL;	
	appH264 =NULL;

	deviceJPEG =NULL;	
	deviceMPEG4 =NULL;	
	deviceH264 =NULL;	
	return result;
}


/////////////////////////////////////////////////////////////////////////////////
//analytics interface
int  COnvifDevConf::GetRulePara(char *analyticsToken,OnvifAnaMotionRule *ruleRespPara)
{
	enum EResultType eResult= OTHER_FAULT;
	COnvifSoap soapApp;

	int result = -1;
	int  i = 0,j=0;

	struct _tan__GetRules GetRules;
	struct _tan__GetRulesResponse GetRulesResp;

	memset(&GetRules,0,sizeof(_tan__GetRules));
	memset(&GetRulesResp,0,sizeof(_tan__GetRulesResponse));
	soapApp.InitUtcTimeByExist(&stOnvifInputParam);

	GetRules.ConfigurationToken = NULL;
	GetRulesResp.__sizeRule = 0;
	GetRulesResp.Rule      = NULL;
	if(m_mediaServAddr[0] == '\0')
	{
		return SERVER_ADDR_FAULT;
	}
	if(analyticsToken == NULL || ruleRespPara == NULL || analyticsToken[0] == '\0')
	{
		return PARAMETER_FAULT;
	}

	if(soapApp.ProduceSoapInfo(TDEVICE_ANALYTICS,true,m_userName,m_passWord) != SOAP_INIT_SUCCESS)
	{
		return eResult;
	}

	GetRules.ConfigurationToken = analyticsToken;

	result = soap_call___dare__GetRules(soapApp.m_pSoap, m_mediaServAddr, NULL,&GetRules,&GetRulesResp);
	if(result != SOAP_OK)
	{
		soapApp.ExitSoap();
		if(soapApp.ProduceSoapInfo(TDEVICE_ANALYTICS,false,m_userName,m_passWord) != SOAP_INIT_SUCCESS)
		{
			return eResult;
		}
		result = soap_call___dare__GetRules(soapApp.m_pSoap, m_mediaServAddr, NULL,&GetRules,&GetRulesResp);
	}

	if(SOAP_OK == result)
	{
		if(GetRulesResp.__sizeRule > 0)
		{
			while(i < GetRulesResp.__sizeRule)
			{
				//get the motion parameter
				if(strstr(GetRulesResp.Rule[i].Type,"CellMotionDetector") == NULL)
				{
					continue;
				}
				strcpy(ruleRespPara->type,GetRulesResp.Rule[i].Type);
				strcpy(ruleRespPara->name,GetRulesResp.Rule[i].Name);
				if(GetRulesResp.Rule[i].Parameters == NULL)
				{
					continue;
				}

				j= 0;
				while(j < GetRulesResp.Rule[i].Parameters->__sizeSimpleItem)
				{
					if(GetRulesResp.Rule[i].Parameters->SimpleItem[i].Name != NULL)
					{
						if(NULL != strstr(GetRulesResp.Rule[i].Parameters->SimpleItem[j].Name,"ActiveCells"))
						{
							if(GetRulesResp.Rule[i].Parameters->SimpleItem[j].Value != NULL)
							{
								ruleRespPara->arealen  = (int)strlen(GetRulesResp.Rule[i].Parameters->SimpleItem[j].Value);
								strcpy((char *)ruleRespPara->area,GetRulesResp.Rule[i].Parameters->SimpleItem[j].Value);
							}
							break;
						}
					}
					j++;
				}
				break;			
				i++;
			}
		}
		eResult = OPERATE_SUCCESS;
	}
	else
	{
		eResult = OPERATE_FAILED;
	}
	soapApp.ExitSoap();
	return eResult;   
}


int  COnvifDevConf::GetAnaPara(char *analyticsToken,OnvifAnaModule *anaRespPara)
{
	enum EResultType eResult= OTHER_FAULT;
	COnvifSoap soapApp;

	int i = 0;
	int result = -1;
	struct _tan__GetAnalyticsModules GetAnalyticsModules;
	struct _tan__GetAnalyticsModulesResponse GetAnalyticsModulesResp;


	GetAnalyticsModules.ConfigurationToken		  = NULL;
	GetAnalyticsModulesResp.__sizeAnalyticsModule = 0;
	GetAnalyticsModulesResp.AnalyticsModule       = NULL;

	if(m_mediaServAddr[0] == '\0')
	{
		return SERVER_ADDR_FAULT;
	}

	if(analyticsToken == NULL || anaRespPara == NULL || analyticsToken[0] == '\0')
	{
		return PARAMETER_FAULT;
	}

	if(soapApp.ProduceSoapInfo(TDEVICE_ANALYTICS,true,m_userName,m_passWord) != SOAP_INIT_SUCCESS)
	{
		return eResult;
	}

	soapApp.InitUtcTimeByExist(&stOnvifInputParam);

	GetAnalyticsModules.ConfigurationToken = analyticsToken;
	result = soap_call___daae__GetAnalyticsModules(soapApp.m_pSoap, m_mediaServAddr, NULL,&GetAnalyticsModules,&GetAnalyticsModulesResp);
	if(result != SOAP_OK)
	{
		soapApp.ExitSoap();
		if(soapApp.ProduceSoapInfo(TDEVICE_ANALYTICS,false,m_userName,m_passWord) != SOAP_INIT_SUCCESS)
		{
			return eResult;
		}
		result = soap_call___daae__GetAnalyticsModules(soapApp.m_pSoap, m_mediaServAddr, NULL,&GetAnalyticsModules,&GetAnalyticsModulesResp);
	}

	if(SOAP_OK == result)
	{

		if(GetAnalyticsModulesResp.__sizeAnalyticsModule > 0)
		{
			while( i < GetAnalyticsModulesResp.__sizeAnalyticsModule)
			{
				if(strstr(GetAnalyticsModulesResp.AnalyticsModule[i].Type,"tt:CellMotionEngine") != NULL)
				{
					strcpy(anaRespPara->type,GetAnalyticsModulesResp.AnalyticsModule[i].Type);
					strcpy(anaRespPara->name,GetAnalyticsModulesResp.AnalyticsModule[i].Name);

					if(GetAnalyticsModulesResp.AnalyticsModule[i].Parameters != NULL)
					{
						if(GetAnalyticsModulesResp.AnalyticsModule[i].Parameters->SimpleItem != NULL)
						{
							if(GetAnalyticsModulesResp.AnalyticsModule[i].Parameters->SimpleItem->Name != NULL)
							{
								if(strstr(GetAnalyticsModulesResp.AnalyticsModule[i].Parameters->SimpleItem->Name,"Sensitivity") != NULL)
								{
									if(GetAnalyticsModulesResp.AnalyticsModule[i].Parameters->SimpleItem->Value != NULL)
									{
										anaRespPara->sensitivetyValue = atoi((const char*)GetAnalyticsModulesResp.AnalyticsModule[i].Parameters->SimpleItem->Value);
									}
								}
							}
						}
					}
					break;
				}
				i++;
			}
		}
		eResult = OPERATE_SUCCESS;
	}
	else
	{
		eResult = OPERATE_FAILED;
	}
	soapApp.ExitSoap();
	return eResult;   
}

int COnvifDevConf::SetAnalyticsArea(char *analyticsToken,OnvifAnaMotionRule *rulePara)
{
	enum EResultType eResult= OTHER_FAULT;
	COnvifSoap soapApp;

	int result = -1;
	struct _tan__ModifyRules ModifyRules;
	struct _tan__ModifyRulesResponse ModifyRulesResp;

	struct _tt__ItemList_SimpleItem SimpleItem[4];	
	struct tt__ItemList Parameters;
	struct tt__Config Rule;

	memset(&ModifyRules,0,sizeof(_tan__ModifyRules));
	memset(&ModifyRulesResp,0,sizeof(_tan__ModifyRulesResponse));

	memset(&SimpleItem,0,sizeof(_tt__ItemList_SimpleItem));
	memset(&Parameters,0,sizeof(tt__ItemList));
	memset(&Rule,0,sizeof(tt__Config));

	if(m_mediaServAddr[0] == '\0')
	{
		return SERVER_ADDR_FAULT;
	}
	if(analyticsToken == NULL || rulePara == NULL || analyticsToken[0] == '\0')
	{
		return PARAMETER_FAULT;
	}

	if(soapApp.ProduceSoapInfo(TDEVICE_ANALYTICS,true,m_userName,m_passWord) != SOAP_INIT_SUCCESS)
	{
		return eResult;
	}

	soapApp.InitUtcTimeByExist(&stOnvifInputParam);

	SimpleItem[0].Name				= "MinCount";
	SimpleItem[0].Value				= "5";

	SimpleItem[1].Name				= "AlarmOnDelay";
	SimpleItem[1].Value				= "100";

	SimpleItem[2].Name				= "AlarmOffDelay";
	SimpleItem[2].Value				= "100";

	SimpleItem[3].Name				= "ActiveCells";
	SimpleItem[3].Value				= (char *)rulePara->area;

	Parameters.__sizeSimpleItem		= 4;
	Parameters.SimpleItem			= SimpleItem;

	Rule.Parameters					= &Parameters;
	Rule.Name						= rulePara->name;
	Rule.Type						= rulePara->type;

	ModifyRules.ConfigurationToken	= analyticsToken;
	ModifyRules.__sizeRule			= 1;
	ModifyRules.Rule				= &Rule;
	ModifyRulesResp.dummy			= 0;

	result = soap_call___dare__ModifyRules(soapApp.m_pSoap, m_mediaServAddr, NULL,&ModifyRules,&ModifyRulesResp);
	if(result != SOAP_OK)
	{
		soapApp.ExitSoap();
		if(soapApp.ProduceSoapInfo(TDEVICE_ANALYTICS,false,m_userName,m_passWord) != SOAP_INIT_SUCCESS)
		{
			return eResult;
		}
		result = soap_call___dare__ModifyRules(soapApp.m_pSoap, m_mediaServAddr, NULL,&ModifyRules,&ModifyRulesResp);
	}

	if(SOAP_OK == result)
	{
		eResult = OPERATE_SUCCESS;
	}
	else
	{
		eResult = OPERATE_FAILED;
	}
	soapApp.ExitSoap();
	return eResult;    
}

int COnvifDevConf::SetAnalyticsSensitivety(char *analyticsToken,OnvifAnaModule *anaParam)
{
	enum EResultType eResult= OTHER_FAULT;
	COnvifSoap soapApp;
	char valueBuf[6];

	int result = -1;
	struct _tan__ModifyAnalyticsModules ModifyAnalyticsModules;
	struct _tan__ModifyAnalyticsModulesResponse ModifyAnalyticsModulesResp;

	struct _tt__ItemList_SimpleItem SimpleItem;	
	struct tt__ItemList Parameters;
	struct tt__Config AnalyticsModule;

	memset(&ModifyAnalyticsModules,0,sizeof(_tan__ModifyAnalyticsModules));
	memset(&ModifyAnalyticsModulesResp,0,sizeof(_tan__ModifyAnalyticsModulesResponse));

	memset(&SimpleItem,0,sizeof(_tt__ItemList_SimpleItem));
	memset(&Parameters,0,sizeof(tt__ItemList));
	memset(&AnalyticsModule,0,sizeof(tt__Config));

	soapApp.InitUtcTimeByExist(&stOnvifInputParam);

	if(m_mediaServAddr[0] == '\0')
	{
		return SERVER_ADDR_FAULT;
	}
	if(analyticsToken == NULL || anaParam == NULL || analyticsToken[0] == '\0')
	{
		return PARAMETER_FAULT;
	}

	if(soapApp.ProduceSoapInfo(TDEVICE_ANALYTICS,true,m_userName,m_passWord) != SOAP_INIT_SUCCESS)
	{
		return eResult;
	}

	memset(valueBuf,0,sizeof(valueBuf));
	sprintf(valueBuf,"%d",anaParam->sensitivetyValue);

	SimpleItem.Name								 = "Sensitivity";
	SimpleItem.Value							 = valueBuf;

	Parameters.__sizeSimpleItem					 = 1;
	Parameters.SimpleItem						 = &SimpleItem;

	AnalyticsModule.Parameters					 = &Parameters;
	AnalyticsModule.Name						 = anaParam->name;
	AnalyticsModule.Type						 = anaParam->type;

	ModifyAnalyticsModules.ConfigurationToken	 = analyticsToken;
	ModifyAnalyticsModules.__sizeAnalyticsModule = 1;
	ModifyAnalyticsModules.AnalyticsModule		 = &AnalyticsModule;

	result = soap_call___daae__ModifyAnalyticsModules(soapApp.m_pSoap, m_mediaServAddr, NULL,&ModifyAnalyticsModules,&ModifyAnalyticsModulesResp);
	if(result != SOAP_OK)
	{
		soapApp.ExitSoap();
		if(soapApp.ProduceSoapInfo(TDEVICE_ANALYTICS,false,m_userName,m_passWord) != SOAP_INIT_SUCCESS)
		{
			return eResult;
		}
		result = soap_call___daae__ModifyAnalyticsModules(soapApp.m_pSoap, m_mediaServAddr, NULL,&ModifyAnalyticsModules,&ModifyAnalyticsModulesResp);
	}

	if(SOAP_OK == result)
	{
		eResult = OPERATE_SUCCESS;
	}
	else
	{
		eResult = OPERATE_FAILED;
	}
	soapApp.ExitSoap();
	return eResult;    
}

