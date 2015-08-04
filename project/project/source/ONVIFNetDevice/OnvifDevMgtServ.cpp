#include "OnvifDevMgtServ.h"

#include "OnvifSoapStub.h"

const int gMonth[][12]={
	{31,28,31,30,31,30,31,31,30,31,30,31},
	{31,29,31,30,31,30,31,31,30,31,30,31}
};


int Onvif_GetDevUtc(struct OnvifDateTime *dateTimeResp, char* serAddr )
{
	enum  EResultType eResult = OTHER_FAULT;
	struct soap *pSoap = NULL;
	struct SOAP_ENV__Header *pHeader =NULL;

	char *tmp1 = NULL;
	char *tmp2 = NULL;

	int result = -1;
	struct _tds__GetSystemDateAndTime sysDateAndTime;
	struct _tds__GetSystemDateAndTimeResponse response;
	struct tt__SystemDateTime *deviceSystemDateAndTime =NULL;

	sysDateAndTime.dummy = 0;
	response.SystemDateAndTime =NULL;

	if(dateTimeResp == NULL)
	{
		return PARAMETER_FAULT;
	}

	pSoap = onvif_init_soap(TDEVICE_MGT,false);
	if(pSoap == NULL)
	{
		return SOAP_INIT_FAULT;
	}

	result =soap_call___tds__GetSystemDateAndTime(pSoap, serAddr, NULL,&sysDateAndTime,&response);
	if(SOAP_OK == result)
	{
		deviceSystemDateAndTime = response.SystemDateAndTime;
		if(deviceSystemDateAndTime != NULL)
		{
			if(deviceSystemDateAndTime->DateTimeType == tt__SetDateTimeType__Manual)
			{
				dateTimeResp->dateTimeType = TSetManual;
			}
			if(deviceSystemDateAndTime->DateTimeType == tt__SetDateTimeType__NTP)
			{
				dateTimeResp->dateTimeType = TSetNTP;
			}

			if(deviceSystemDateAndTime->DaylightSavings == _true)
			{
				dateTimeResp->bDaylightSave = true;
			}
			else
			{
				dateTimeResp->bDaylightSave = false;
			}

			if(deviceSystemDateAndTime->UTCDateTime != NULL)
			{
				if(deviceSystemDateAndTime->UTCDateTime->Date != NULL)
				{
					dateTimeResp->date.Year = deviceSystemDateAndTime->UTCDateTime->Date->Year;
					dateTimeResp->date.Month = deviceSystemDateAndTime->UTCDateTime->Date->Month;
					dateTimeResp->date.Day = deviceSystemDateAndTime->UTCDateTime->Date->Day;
				}

				if(deviceSystemDateAndTime->UTCDateTime->Time != NULL)
				{
					dateTimeResp->time.Hour = deviceSystemDateAndTime->UTCDateTime->Time->Hour;
					dateTimeResp->time.Minute = deviceSystemDateAndTime->UTCDateTime->Time->Minute;
					dateTimeResp->time.Second = deviceSystemDateAndTime->UTCDateTime->Time->Second;
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

int Onvif_GetDev_UtcTime( char* servAddr, ONVIF_INIT_INPUT_PARAM* pstInputParam )
{
	int ret;
	struct OnvifDateTime stUtcTime;

	memset( &stUtcTime, 0, sizeof( struct OnvifDateTime) );
	ret = Onvif_GetDevUtc( &stUtcTime, servAddr );

	if( OPERATE_SUCCESS == ret )
	{
		pstInputParam->stDevUtcTime.uYear = stUtcTime.date.Year;
		pstInputParam->stDevUtcTime.uMonth = stUtcTime.date.Month;
		pstInputParam->stDevUtcTime.uDay = stUtcTime.date.Day;
		pstInputParam->stDevUtcTime.uHour = stUtcTime.time.Hour;
		pstInputParam->stDevUtcTime.uMointue = stUtcTime.time.Minute;
		pstInputParam->stDevUtcTime.uSeconds = stUtcTime.time.Second;

		return 0;
	}

	return -1;
}

COnvifDevMgt::COnvifDevMgt()
{

   memset(m_servAddr,'\0',sizeof(m_servAddr));
   memset(m_userName,'\0',sizeof(m_userName));
   memset(m_passKey,'\0',sizeof(m_passKey));
   

   //memset(m_timeZone,'\0',sizeof(m_timeZone));
   //strcpy(m_timeZone,"UTC8");
   //strcpy(m_timeZone,"PST10PDT,M3.2.0/2,M11.1.0/2"); 
   //strcpy(m_timeZone,"GMT8");
   //strcpy(m_timeZone,"STWT8");

   //network
   memset(m_ipSetToken,'\0',sizeof(m_ipSetToken));  

   memset( &stOnvifInputParam, 0, sizeof( ONVIF_INIT_INPUT_PARAM));

}

COnvifDevMgt::~COnvifDevMgt()
{

}
//device capability
int COnvifDevMgt::GetDevCapability(char *aDevServAddr,enum EOnvifCapabilityType capabilityType,struct OnvifDevCapaServ *capaResp,unsigned int aDigest)
{
	int result = -1;
	enum EResultType eResult = OTHER_FAULT;
	struct soap *pSoap =NULL;
	struct SOAP_ENV__Header *pHeader =NULL;

	struct _tds__GetCapabilities GetCapabilities;
	struct _tds__GetCapabilitiesResponse  getCapaResp;

	enum tt__CapabilityCategory category;

	switch(capabilityType)
	{
		case TAll:
			category = tt__CapabilityCategory__All;
			break;
		case TAnalytics:
			category = tt__CapabilityCategory__Analytics;
			break;
		case TDeviceMgt:
			category = tt__CapabilityCategory__Device;
			break;
		case TEvents:
			category = tt__CapabilityCategory__Events;
			break;
		case TImaging:
			category = tt__CapabilityCategory__Imaging;
			break;
		case TMedia:
			category = tt__CapabilityCategory__Media;
			break;
		case TPTZ:
			category = tt__CapabilityCategory__PTZ;
			break;
		default:
			return -1;
	}

	GetCapabilities.__sizeCategory = 1;
	GetCapabilities.Category = &category;
	getCapaResp.Capabilities =NULL;

    //printf("***enter capability!\n");
	if(aDevServAddr == NULL)
	{
		return SERVER_ADDR_FAULT;
	}
	if(capaResp == NULL)
	{
		return PARAMETER_FAULT;
	}
	pSoap = onvif_init_soap(TDEVICE_MGT,false);
	if(pSoap == NULL)
	{
		printf("***capability init soap failed!\n");
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
		printf("***init header failed!\n");
		onvif_free_soap(pSoap);
		return SOAP_INIT_FAULT;
	}

	pSoap->header = pHeader;

	result = soap_call___tds__GetCapabilities((soap *)pSoap,aDevServAddr, NULL,&GetCapabilities,&getCapaResp);

	capaResp->bAnalyticsEnable = false;
	capaResp->bDevMgtEnable = false;
	capaResp->bEventEnable = false;
	capaResp->bImagingEnable = false;
	capaResp->bImagingEnable = false;
	capaResp->bMediaEnable = false;
	capaResp->bPTZEnable = false;
	capaResp->maxMediaProfileNum = 0;

	if(SOAP_OK == result)
	{
		if(getCapaResp.Capabilities->Analytics != NULL)
		{
			strcpy(capaResp->analyticsServ,getCapaResp.Capabilities->Analytics->XAddr);
           capaResp->bAnalyticsEnable = true;
		}
		if(getCapaResp.Capabilities->Device != NULL)
		{
			strcpy(capaResp->devMgtServ,getCapaResp.Capabilities->Device->XAddr);
			capaResp->bDevMgtEnable = true;
		}

		if(getCapaResp.Capabilities->Events != NULL)
		{
			strcpy(capaResp->eventsServ,getCapaResp.Capabilities->Events->XAddr);
			capaResp->bEventEnable = true;
		}  
		if(getCapaResp.Capabilities->Imaging != NULL)
		{
			strcpy(capaResp->imagingServ,getCapaResp.Capabilities->Imaging->XAddr);
			capaResp->bImagingEnable =true;
		}

		if(getCapaResp.Capabilities->Media != NULL)
		{
			strcpy(capaResp->mediaServ,getCapaResp.Capabilities->Media->XAddr);
			capaResp->bMediaEnable = true;
          // if(getCapaResp.Capabilities->Media->Extension->ProfileCapabilities != NULL)
			 // capaResp->maxMediaProfileNum = getCapaResp.Capabilities->Media->Extension->ProfileCapabilities->MaximumNumberOfProfiles;
		   //else
			//   capaResp->maxMediaProfileNum = 0;
		}

		if(getCapaResp.Capabilities->PTZ != NULL)
		{
			strcpy(capaResp->PTZServ,getCapaResp.Capabilities->PTZ->XAddr);
			capaResp->bPTZEnable = true;
		}
       //extension handle

		eResult = OPERATE_SUCCESS;	
	}
	else
	{ 
		eResult = OPERATE_FAILED;	
	}

	getCapaResp.Capabilities = NULL;

	if(pSoap->header != NULL)
	{
		pSoap->header = NULL;
	}
	onvif_free_soap_header(pHeader);

	pHeader = NULL;
	onvif_free_soap(pSoap);
	//printf("***exit capability!\n");
	return eResult;
}

int COnvifDevMgt::GetDevUtcTimeCapability(char *aDevServAddr,enum EOnvifCapabilityType capabilityType,struct OnvifDevCapaServ *capaResp,unsigned int aDigest)
{
	int eResult;

	Onvif_GetDev_UtcTime(aDevServAddr,&stOnvifInputParam);
	eResult = GetDevCapability(aDevServAddr,capabilityType,capaResp,aDigest);
	memset( &stOnvifInputParam, 0, sizeof( ONVIF_INIT_INPUT_PARAM));

	return eResult;
};

//register device
//check the user if the authenticate
//in:deviceMgtServ,this parameter is not allow invalid string,device management service address,get it from the function GetDevCapability()
//in:username,the check user name .this parameter is not allow invalid string.such as username[0] = '\0';
//in:password,this parameter is not allow invalid string.
//return: true,the user is exit;false,the user is not correctted.
bool COnvifDevMgt::RegisterUser(char *deviceMgtServ,char *userName,char *password,unsigned int aDigest)
{
   int result = -1; 
	if(deviceMgtServ == NULL || userName == NULL || password == NULL)
	{
		return false;
	}
    strcpy(m_servAddr,deviceMgtServ);
    strcpy(m_userName,userName);
	strcpy(m_passKey,password);
   result = SetHostName("TVT-8888",aDigest);
   if(result == OPERATE_SUCCESS)
   {
	   return true;
   }
   m_servAddr[0] = '\0';
   m_userName[0] = '\0';
   m_passKey[0] = '\0';
	return false;
}

//system manage
void COnvifDevMgt::InitDevMgtParameter(char *aServAddr,char *aUserName,char *aPasskey)
{
	if(aServAddr == NULL || aUserName == NULL || aPasskey == NULL)
	{
		m_servAddr[0] = '\0';
		m_userName[0] = '\0';
		m_passKey[0] = '\0';
		return;
	}
	strcpy(m_servAddr,aServAddr);
	strcpy(m_userName,aUserName);
	strcpy(m_passKey,aPasskey);

}

void COnvifDevMgt::InitDevUtcTimes( bool bClearUtcTime, char* aServAddr, unsigned int ipcCtrlType )
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


int COnvifDevMgt::GetDevInfo(struct OnvifDevInfo *aDevInfoResp,unsigned int aDigest)
{
	int result = -1;
	struct _tds__GetDeviceInformation information;
	struct _tds__GetDeviceInformationResponse response;
   
	enum  EResultType eResult = OTHER_FAULT;
    struct soap *pSoap = NULL;
	struct SOAP_ENV__Header *pHeader =NULL;

	information.dummy = 0;
	if(m_servAddr[0] == '\0')
	{
		return SERVER_ADDR_FAULT;
	}

	if(aDevInfoResp == NULL)
	{
		return PARAMETER_FAULT;
	}
    
	pSoap = onvif_init_soap(TDEVICE_MGT,false);
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

	result = soap_call___tds__GetDeviceInformation(pSoap, m_servAddr,NULL,&information,&response);
	if(SOAP_OK == result)
	{
		strcpy(aDevInfoResp->Manufacturer,response.Manufacturer);
		strcpy(aDevInfoResp->Model,response.Model);
		strcpy(aDevInfoResp->FirmwareVersion,response.FirmwareVersion);
		strcpy(aDevInfoResp->SerialNumber,response.SerialNumber);
		strcpy(aDevInfoResp->HardwareId,response.Manufacturer);
		eResult = OPERATE_SUCCESS;
	}
	else
	{
		return OPERATE_FAILED;
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

int COnvifDevMgt::GetDevDateTime(struct OnvifDateTime *dateTimeResp,unsigned int aDigest)
{
	enum  EResultType eResult = OTHER_FAULT;
	struct soap *pSoap = NULL;
	struct SOAP_ENV__Header *pHeader =NULL;
    
	char *tmp1 = NULL;
	char *tmp2 = NULL;

	int result = -1;
	struct _tds__GetSystemDateAndTime sysDateAndTime;
	struct _tds__GetSystemDateAndTimeResponse response;
	struct tt__SystemDateTime *deviceSystemDateAndTime =NULL;

	sysDateAndTime.dummy = 0;
	response.SystemDateAndTime =NULL;

	if(m_servAddr[0] == '\0')
	{
		return SERVER_ADDR_FAULT;
	}

	if(dateTimeResp == NULL)
	{
		return PARAMETER_FAULT;
	}

	pSoap = onvif_init_soap(TDEVICE_MGT,false);
	if(pSoap == NULL)
	{
		return SOAP_INIT_FAULT;
	}

	result =soap_call___tds__GetSystemDateAndTime(pSoap, m_servAddr,NULL,&sysDateAndTime,&response);
	if(SOAP_OK != result)
	{
		onvif_free_soap(pSoap);
		pSoap = NULL;

		pSoap = onvif_init_soap(TDEVICE_MGT,false);
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

		result =soap_call___tds__GetSystemDateAndTime(pSoap, m_servAddr,NULL,&sysDateAndTime,&response);

	}
	
	if(SOAP_OK == result)
	{
		deviceSystemDateAndTime = response.SystemDateAndTime;
		if(deviceSystemDateAndTime != NULL)
		{
			if(deviceSystemDateAndTime->DateTimeType == tt__SetDateTimeType__Manual)
			{
				dateTimeResp->dateTimeType = TSetManual;
			}
			if(deviceSystemDateAndTime->DateTimeType == tt__SetDateTimeType__NTP)
			{
				dateTimeResp->dateTimeType = TSetNTP;
			}

			if(deviceSystemDateAndTime->DaylightSavings == _true)
			{
				dateTimeResp->bDaylightSave = true;
			}
			else
			{
				dateTimeResp->bDaylightSave = false;
			}

          if(deviceSystemDateAndTime->TimeZone != NULL)
		   {
	
			   if(deviceSystemDateAndTime->TimeZone->TZ != NULL)
				{
					strcpy(dateTimeResp->timeZone,deviceSystemDateAndTime->TimeZone->TZ);
					//get timezone tab 
					tmp1 = dateTimeResp->timeZone;
					tmp2  = strstr(tmp1,"T");
					int lens_tmp = 0;
					if(tmp2 != NULL)
					{
						tmp2++;
						while(*tmp2 >= 'A' && *tmp2 <='Z')
						{
							tmp2++;
						}
						lens_tmp = tmp2 - tmp1;
						if( lens_tmp > 31 )
						{
							lens_tmp = 31;
						}
						strncpy(dateTimeResp->TZTab,tmp1,lens_tmp);

						//Get DST tab
						tmp1 = tmp2;
						while(*tmp1 != '\0')
						{
							if(*tmp1 >= 'A' && *tmp1 <='Z')
							{
								break;
							}
							tmp1++;
						}

						if(*tmp1 != '\0')
						{
							tmp2 = strstr(tmp1,"T");

							if(tmp2 != NULL)
							{
								tmp2++;
								while(*tmp2 >= 'A' && *tmp2 <='Z')
								{
									tmp2++;
								}

								lens_tmp = tmp2 - tmp1;
								if( lens_tmp > 31 )
								{
									lens_tmp = 31;
								}
								strncpy(dateTimeResp->DSTTab,tmp1,lens_tmp );
							}
						}
					}
					else
					{
						dateTimeResp->timeZone[0] = '\0';
						dateTimeResp->TZTab[0] = '\0';
						dateTimeResp->DSTTab[0] = '\0';
					}
				}
				else
				{
					dateTimeResp->timeZone[0] = '\0';
				}

		   }

			if(deviceSystemDateAndTime->UTCDateTime != NULL)
			{
			  if(deviceSystemDateAndTime->UTCDateTime->Date != NULL)
			  {
					dateTimeResp->date.Year = deviceSystemDateAndTime->UTCDateTime->Date->Year;
				    dateTimeResp->date.Month = deviceSystemDateAndTime->UTCDateTime->Date->Month;
				    dateTimeResp->date.Day = deviceSystemDateAndTime->UTCDateTime->Date->Day;
			  }
			  if(deviceSystemDateAndTime->UTCDateTime->Time != NULL)
			  {
              dateTimeResp->time.Hour = deviceSystemDateAndTime->UTCDateTime->Time->Hour;
              dateTimeResp->time.Minute = deviceSystemDateAndTime->UTCDateTime->Time->Minute;
              dateTimeResp->time.Second = deviceSystemDateAndTime->UTCDateTime->Time->Second;
			  }
			 }

			if(deviceSystemDateAndTime->LocalDateTime != NULL)
			{
				if(deviceSystemDateAndTime->LocalDateTime->Date != NULL)
				{
					dateTimeResp->date.Year = deviceSystemDateAndTime->LocalDateTime->Date->Year;
					dateTimeResp->date.Month = deviceSystemDateAndTime->LocalDateTime->Date->Month;
					dateTimeResp->date.Day = deviceSystemDateAndTime->LocalDateTime->Date->Day;
			    }
               
				if(deviceSystemDateAndTime->LocalDateTime->Time != NULL)
				{
					dateTimeResp->time.Hour = deviceSystemDateAndTime->LocalDateTime->Time->Hour;
					dateTimeResp->time.Minute = deviceSystemDateAndTime->LocalDateTime->Time->Minute;
					dateTimeResp->time.Second = deviceSystemDateAndTime->LocalDateTime->Time->Second;
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


int COnvifDevMgt::FormatTimeZoneString(char* timeBuf, struct OnvifDateTime *dateTimeSet, unsigned int ipcManuID)
{
	if( ONVIF_TIMEZONE_SET_DAHUA_MODE == ipcManuID )
	{
		if(dateTimeSet->bBehindUtc && (0 != dateTimeSet->deltaHour || 0 != dateTimeSet->deltaMinute) )
		{
			sprintf(timeBuf,"%s-%02d:%02d",dateTimeSet->TZTab,dateTimeSet->deltaHour,dateTimeSet->deltaMinute);
		}
		else
		{
			sprintf(timeBuf,"%s+%02d:%02d",dateTimeSet->TZTab,dateTimeSet->deltaHour,dateTimeSet->deltaMinute);
		}
	}
	else
	{
		int len = strlen( dateTimeSet->TZTab );
		if( len > 3 )
		{
			sprintf( timeBuf, "%s", dateTimeSet->TZTab );
			return 0;
		}


		if(dateTimeSet->bBehindUtc)
		{
			sprintf(timeBuf,"%s+%02d:%02d",dateTimeSet->TZTab,dateTimeSet->deltaHour,dateTimeSet->deltaMinute);
		}
		else
		{
			sprintf(timeBuf,"%s-%02d:%02d",dateTimeSet->TZTab,dateTimeSet->deltaHour,dateTimeSet->deltaMinute);
		}
	}
	return 0;
}

int COnvifDevMgt::SetDevDateTime(struct OnvifDateTime *dateTimeSet,unsigned int aDigest, unsigned int ipcManuID)
{
	enum  EResultType eResult = OTHER_FAULT;
	struct soap *pSoap = NULL;
	struct SOAP_ENV__Header *pHeader =NULL;

	int result = -1;
	struct _tds__SetSystemDateAndTime  setsysDateAndTime;
	struct _tds__SetSystemDateAndTimeResponse response;

	struct tt__TimeZone devTimeZone;	
	struct tt__DateTime devUTCDateTime;		
	struct tt__Time deTime;	
	struct tt__Date deDate;	

	setsysDateAndTime.TimeZone = NULL;
	setsysDateAndTime.UTCDateTime  = NULL;
	response.dummy = 0;
	devUTCDateTime.Date = NULL;
	devUTCDateTime.Time =NULL;
	devTimeZone.TZ =NULL;

	struct OnvifDevDate       newDate;
	struct OnvifDevTime       newTime;
	memset(&newDate,0,sizeof(OnvifDevDate));
	memset(&newTime,0,sizeof(OnvifDevTime));

	char timeBuf[64];
	int inDay=0,outDay=0 ,i=0; 

	if(m_servAddr[0] == '\0')
	{
		return SERVER_ADDR_FAULT;
	}

	if(dateTimeSet == NULL)
	{
		return PARAMETER_FAULT;
	}
    
	memset(timeBuf,'\0',sizeof(timeBuf));

#if 1 //process timezone information
	//format the timezone
	FormatTimeZoneString( timeBuf, dateTimeSet, ipcManuID );
	strcpy(dateTimeSet->timeZone,timeBuf);
#else

	strcat(dateTimeSet.timeZone,m_tzTab);
	sprintf(timeBuf,"%02d:%02d",0,0);
	strcat(dateTimeSet.timeZone,timeBuf);
#endif

	if(dateTimeSet->dateTimeType == TSetManual)
	{
		setsysDateAndTime.DateTimeType = tt__SetDateTimeType__Manual;
		


		LocalToUTC(dateTimeSet,newDate,newTime);

		deDate.Year = newDate.Year;
		deDate.Month = newDate.Month;
		deDate.Day = newDate.Day;
		devUTCDateTime.Date = &deDate;


		deTime.Hour = newTime.Hour;
		deTime.Minute = newTime.Minute;
		deTime.Second  = newTime.Second;
		devUTCDateTime.Time = &deTime;
		//printf("Manual UTC time:%d:%d:%d,%d:%d:%d\n",newDate.Year,newDate.Month,newDate.Day,newTime.Hour,newTime.Minute,newTime.Second);
		setsysDateAndTime.UTCDateTime = &devUTCDateTime;
	}

	if(dateTimeSet->bDaylightSave)
	{
		setsysDateAndTime.DaylightSavings = _true;

       strcat(dateTimeSet->timeZone,dateTimeSet->dstInfo);
	}
	else
	{
		setsysDateAndTime.DaylightSavings = _false;
	}

	if(dateTimeSet->timeZone[0] != '\0')
	{
		devTimeZone.TZ = dateTimeSet->timeZone;
		setsysDateAndTime.TimeZone = &devTimeZone;
	}
	else
	{
		setsysDateAndTime.TimeZone = NULL;
	}

	pSoap = onvif_init_soap(TDEVICE_MGT,false);
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

	result =soap_call___tds__SetSystemDateAndTime(pSoap, m_servAddr,NULL,&setsysDateAndTime,&response);

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

int COnvifDevMgt::SetDstActive(struct OnvifDateTime *dateTimeSet,unsigned int aDigest, unsigned int ipcManuID)
{
	enum  EResultType eResult = OTHER_FAULT;
	struct soap *pSoap = NULL;
	struct SOAP_ENV__Header *pHeader =NULL;

	int result = -1;
	struct _tds__SetSystemDateAndTime  setsysDateAndTime;
	struct _tds__SetSystemDateAndTimeResponse response;

	struct tt__TimeZone devTimeZone;	
	
	setsysDateAndTime.TimeZone = NULL;
	setsysDateAndTime.UTCDateTime  = NULL;
	response.dummy = 0;
	devTimeZone.TZ =NULL;

	char timeBuf[64];
	int inDay=0,outDay=0 ,i=0; 

	if(m_servAddr[0] == '\0')
	{
		return SERVER_ADDR_FAULT;
	}

	if(dateTimeSet == NULL)
	{
		return PARAMETER_FAULT;
	}

	memset(timeBuf,'\0',sizeof(timeBuf));

#if 1 //process timezone information
	//format the timezone
	FormatTimeZoneString( timeBuf, dateTimeSet, ipcManuID );
	strcpy(dateTimeSet->timeZone,timeBuf);
#else

	strcat(dateTimeSet.timeZone,m_tzTab);
	sprintf(timeBuf,"%02d:%02d",0,0);
	strcat(dateTimeSet.timeZone,timeBuf);
#endif

	if(dateTimeSet->dateTimeType == TSetManual)
	{
		setsysDateAndTime.DateTimeType = tt__SetDateTimeType__Manual;
	}

	if(dateTimeSet->dateTimeType == TSetNTP)
	{
		setsysDateAndTime.DateTimeType = tt__SetDateTimeType__NTP;
	}

	if(dateTimeSet->bDaylightSave)
	{
		setsysDateAndTime.DaylightSavings = _true;
		{
			memset(timeBuf,'\0',sizeof(timeBuf));
			if(dateTimeSet->bWeekType == false) //the date format
			{
				for(i=1;i<dateTimeSet->beginDate.Month + 1;i++)
				{
					inDay+= gMonth[0][i-1];
				}
				inDay += dateTimeSet->beginDate.Day;

				for(i=1;i<dateTimeSet->endDate.Month+1;i++)
				{
					outDay += gMonth[0][i-1];
				}
				outDay += dateTimeSet->endDate.Day;;
				if(dateTimeSet->DSTTab[0] != 0)
				{
					sprintf(timeBuf,"%s%d,J%d/%d:%d:%d,J%d/%d:%d:%d",dateTimeSet->DSTTab,dateTimeSet->tOffset,\
						inDay,dateTimeSet->beginTime.Hour,dateTimeSet->beginTime.Minute,dateTimeSet->beginTime.Second, \
						outDay,dateTimeSet->endTime.Hour,dateTimeSet->endTime.Minute,dateTimeSet->endTime.Second); 
				}
				else
				{
					sprintf(timeBuf,"DST%d,J%d/%d:%d:%d,J%d/%d:%d:%d",dateTimeSet->tOffset,\
						inDay,dateTimeSet->beginTime.Hour,dateTimeSet->beginTime.Minute,dateTimeSet->beginTime.Second, \
						outDay,dateTimeSet->endTime.Hour,dateTimeSet->endTime.Minute,dateTimeSet->endTime.Second); 
				}
			}
			else //the week format
			{
				if(dateTimeSet->DSTTab[0] != 0)
				{
					sprintf(timeBuf,"%s%d,M%d.%d.%d/%d:%d:%d,M%d.%d.%d/%d:%d:%d",dateTimeSet->DSTTab,dateTimeSet->tOffset,\
						dateTimeSet->beginDate.Month+1,dateTimeSet->beginWeekIndex+1,dateTimeSet->beginWday, \
						dateTimeSet->beginTime.Hour,dateTimeSet->beginTime.Minute,dateTimeSet->beginTime.Second, \
						dateTimeSet->endDate.Month+1,dateTimeSet->endWeekIndex+1,dateTimeSet->endWday, \
						dateTimeSet->endTime.Hour,dateTimeSet->endTime.Minute,dateTimeSet->endTime.Second); 
				}
				else
				{
					sprintf(timeBuf,"DST%d,M%d.%d.%d/%d:%d:%d,M%d.%d.%d/%d:%d:%d",dateTimeSet->tOffset,\
						dateTimeSet->beginDate.Month+1,dateTimeSet->beginWeekIndex+1,dateTimeSet->beginWday, \
						dateTimeSet->beginTime.Hour,dateTimeSet->beginTime.Minute,dateTimeSet->beginTime.Second, \
						dateTimeSet->endDate.Month+1,dateTimeSet->endWeekIndex+1,dateTimeSet->endWday, \
						dateTimeSet->endTime.Hour,dateTimeSet->endTime.Minute,dateTimeSet->endTime.Second); 
				}

			}

			strcat(dateTimeSet->timeZone,timeBuf);
			strcpy(dateTimeSet->dstInfo,timeBuf);

			printf("****DST Info:%s\n",dateTimeSet->timeZone);
		}
	}
	else
	{
		setsysDateAndTime.DaylightSavings = _false;
	}

	if(dateTimeSet->timeZone[0] != '\0')
	{
		devTimeZone.TZ = dateTimeSet->timeZone;
		setsysDateAndTime.TimeZone = &devTimeZone;
	}
	else
	{
		devTimeZone.TZ = NULL;
		setsysDateAndTime.TimeZone = NULL;
	}

	pSoap = onvif_init_soap(TDEVICE_MGT,false);
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

	result =soap_call___tds__SetSystemDateAndTime(pSoap, m_servAddr,NULL,&setsysDateAndTime,&response);

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

int COnvifDevMgt::SetNtpActive(struct OnvifDateTime *dateTimeSet,unsigned int aDigest, unsigned int ipcManuID)
{
	enum  EResultType eResult = OTHER_FAULT;
	struct soap *pSoap = NULL;
	struct SOAP_ENV__Header *pHeader =NULL;

	int result = -1;
	struct _tds__SetSystemDateAndTime  setsysDateAndTime;
	struct _tds__SetSystemDateAndTimeResponse response;

	char timeBuf[64];
	struct tt__DateTime devUTCDateTime;		
	struct tt__Time deTime;	
	struct tt__Date deDate;	
	struct tt__TimeZone devTimeZone;	

	struct OnvifDevDate       newDate;
	struct OnvifDevTime       newTime;
	memset(&newDate,0,sizeof(OnvifDevDate));
	memset(&newTime,0,sizeof(OnvifDevTime));

	devTimeZone.TZ = NULL;
	setsysDateAndTime.TimeZone = NULL;
	setsysDateAndTime.UTCDateTime  = NULL;
	response.dummy = 0;
    
	memset(&deDate,0,sizeof(tt__Date));
	memset(&deTime,0,sizeof(tt__Time));
   devUTCDateTime.Date = NULL;
   devUTCDateTime.Time = NULL;

	if(m_servAddr[0] == '\0')
	{
		return SERVER_ADDR_FAULT;
	}

	if(dateTimeSet == NULL)
	{
		return PARAMETER_FAULT;
	}

	if(dateTimeSet->dateTimeType == TSetManual)
	{
		setsysDateAndTime.DateTimeType = tt__SetDateTimeType__Manual;
	}
	if(dateTimeSet->dateTimeType == TSetNTP)
	{
		setsysDateAndTime.DateTimeType = tt__SetDateTimeType__NTP;
		printf("NTP is active!\n");
	}

	{
		memset(timeBuf,'\0',sizeof(timeBuf));

		//format the timezone
		FormatTimeZoneString( timeBuf, dateTimeSet, ipcManuID );
		strcpy(dateTimeSet->timeZone,timeBuf);

		//printf("NTP TIME ZONE:%s\n",timeBuf);

	    LocalToUTC(dateTimeSet,newDate,newTime);

		deDate.Year = newDate.Year;
		deDate.Month = newDate.Month;
		deDate.Day = newDate.Day;
		devUTCDateTime.Date = &deDate;

		deTime.Hour = newTime.Hour;
		deTime.Minute = newTime.Minute;
		deTime.Second  = newTime.Second;
		devUTCDateTime.Time = &deTime;
		//printf("NTP UTC time:%d:%d:%d,%d:%d:%d\n",newDate.Year,newDate.Month,newDate.Day,newTime.Hour,newTime.Minute,newTime.Second);
		setsysDateAndTime.UTCDateTime = &devUTCDateTime;
	}



	if(dateTimeSet->bDaylightSave)
	{
		setsysDateAndTime.DaylightSavings = _true;
       strcat(dateTimeSet->timeZone,dateTimeSet->dstInfo);
	}
	else
	{
		setsysDateAndTime.DaylightSavings = _false;
	}

	if(dateTimeSet->timeZone[0] != '\0')
	{
		devTimeZone.TZ = dateTimeSet->timeZone;
		setsysDateAndTime.TimeZone = &devTimeZone;
	}
	else
	{
		setsysDateAndTime.TimeZone = NULL;
	}

	pSoap = onvif_init_soap(TDEVICE_MGT,false);
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

	result =soap_call___tds__SetSystemDateAndTime(pSoap, m_servAddr,NULL,&setsysDateAndTime,&response);

	if(SOAP_OK == result)
	{
		eResult = OPERATE_SUCCESS;
		//printf("Set NTP time success!\n");
	}
	else
	{
		eResult = OPERATE_FAILED;
		//printf("Set NTP time failed!\n");
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


int COnvifDevMgt::RebootDev(char *devResp,unsigned int aDigest)
{
	enum  EResultType eResult = OTHER_FAULT;
	struct soap *pSoap = NULL;
	struct SOAP_ENV__Header *pHeader =NULL;

	int result = -1;
	struct _tds__SystemReboot reboot;
	struct _tds__SystemRebootResponse response;

	reboot.dummy = 0;
	response.Message = NULL;
	if(m_servAddr[0] == '\0')
	{
		return SERVER_ADDR_FAULT;
	}

	if(devResp == NULL)
	{
		return PARAMETER_FAULT;
	}
	pSoap = onvif_init_soap(TDEVICE_MGT,false);
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

	result =soap_call___tds__SystemReboot(pSoap, m_servAddr,NULL,&reboot,&response);

	if(SOAP_OK == result)
	{
		strcpy(devResp,response.Message);
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



int COnvifDevMgt::SetDevFactoryDefault(bool bSoftDefault,unsigned int aDigest)
{
	enum  EResultType eResult = OTHER_FAULT;
	struct soap *pSoap = NULL;
	struct SOAP_ENV__Header *pHeader =NULL;

	int result = -1;
	struct _tds__SetSystemFactoryDefault fDefault;
	struct _tds__SetSystemFactoryDefaultResponse response;

	if(m_servAddr[0] == '\0')
	{
		return SERVER_ADDR_FAULT;
	}

	if(bSoftDefault)
	{
		fDefault.FactoryDefault = tt__FactoryDefaultType__Soft;
	}
	else
	{
		fDefault.FactoryDefault = tt__FactoryDefaultType__Hard;        
	}

	pSoap = onvif_init_soap(TDEVICE_MGT,false);
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

	result =soap_call___tds__SetSystemFactoryDefault(pSoap, m_servAddr,NULL,&fDefault,&response);
	if(SOAP_OK == result)
	{
		return OPERATE_SUCCESS;
	}
	else
	{
		return OPERATE_FAILED;
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

//user
int COnvifDevMgt::GetDevUsers(struct OnvifUsersList *usersResp,unsigned int aDigest)
{
	enum  EResultType eResult = OTHER_FAULT;
	struct soap *pSoap = NULL;
	struct SOAP_ENV__Header *pHeader =NULL;

	int result = -1;
	struct _tds__GetUsers users;
	struct _tds__GetUsersResponse response;
	int i = 0;
	int size = 0;
	struct tt__User *devUser = NULL;
	struct OnvifUserInfo *appUser =NULL;

	users.dummy =0;
	response.__sizeUser = 0;
	response.User =NULL;
	if(m_servAddr[0]  == '\0')
	{
		return SERVER_ADDR_FAULT;
	}

	if(usersResp  == NULL)
	{
		return PARAMETER_FAULT;
	}

	pSoap = onvif_init_soap(TDEVICE_MGT,false);
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

	result = soap_call___tds__GetUsers(pSoap, m_servAddr, NULL,&users,&response);
	if(SOAP_OK == result)
	{
		size = response.__sizeUser;
		if(size > 0)
		{
			devUser =response.User;
			usersResp->User = (struct OnvifUserInfo *)malloc(size *sizeof(struct OnvifUserInfo));
			
			if(usersResp->User == NULL)
			{
				return OPERATE_FAILED;
			}

			appUser = usersResp->User;

			while(i < size)
			{
				GetAppUserFromDevice(appUser,devUser);
				i++;
				appUser++;
				devUser++;
			}
		}
		appUser =NULL;
		devUser =NULL;
		response.User =NULL;
		eResult =OPERATE_SUCCESS;
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

int COnvifDevMgt::AddUser(struct OnvifUsersList *addUsers,unsigned int aDigest)
{
	enum  EResultType eResult = OTHER_FAULT;
	struct soap *pSoap = NULL;
	struct SOAP_ENV__Header *pHeader =NULL;

	int result = -1;
	struct _tds__CreateUsers users;
	struct _tds__CreateUsersResponse response;
	int i = 0;
	int size = 0;
	struct tt__User *devUser = NULL;
	struct OnvifUserInfo *appUser =NULL;

	users.__sizeUser = 0;
	users.User =NULL;
	response.dummy =0;

	if(m_servAddr[0]  == '0')
	{
		return SERVER_ADDR_FAULT;
	}

	if(addUsers == NULL)
	{
		return PARAMETER_FAULT;
	}

	size = addUsers->sizeUser;
	appUser= addUsers->User;

	users.__sizeUser = size;
	users.User  = (struct tt__User *)malloc(size *sizeof(struct tt__User));
	if(users.User == NULL)
	{
		return OPERATE_FAILED;
	}
   memset(users.User,0,size *sizeof(struct tt__User));
	while(i < size)
	{
		GetDevUserFromApp(devUser,appUser);
		i++;
		appUser++;
		devUser++;
	}

	pSoap = onvif_init_soap(TDEVICE_MGT,false);
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

	result = soap_call___tds__CreateUsers(pSoap, m_servAddr, NULL,&users,&response);

	appUser =NULL;
	devUser =NULL;
	free(users.User);
	users.User = NULL;
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

int COnvifDevMgt::DeleteUser(struct OnvifDeleteUsers *delUsers,unsigned int aDigest)
{
	enum  EResultType eResult = OTHER_FAULT;
	struct soap *pSoap = NULL;
	struct SOAP_ENV__Header *pHeader =NULL;

	int result = -1;
	struct _tds__DeleteUsers users;
	struct _tds__DeleteUsersResponse response;

	users.__sizeUsername = 0;
	users.Username =NULL;
	response.dummy = 0;

	if(m_servAddr[0]  == '\0')
	{
		return SERVER_ADDR_FAULT;
	}

	if(delUsers == NULL)
	{
		return PARAMETER_FAULT;
	}

	users.__sizeUsername = delUsers->sizeUsername;
	users.Username = delUsers->Username;

	pSoap = onvif_init_soap(TDEVICE_MGT,false);
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

	result = soap_call___tds__DeleteUsers(pSoap, m_servAddr, NULL,&users,&response);

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

int COnvifDevMgt::SetUserCode(struct OnvifUsersList *setUsers,unsigned int aDigest)
{
	enum  EResultType eResult = OTHER_FAULT;
	struct soap *pSoap = NULL;
	struct SOAP_ENV__Header *pHeader =NULL;

	int result = -1;
	struct _tds__SetUser user;
	struct _tds__SetUserResponse response;
	int i = 0;
	int size = 0;
	struct tt__User *devUser = NULL;
	struct OnvifUserInfo *appUser =NULL;

	user.__sizeUser = 0;
	user.User =NULL;
	response.dummy =0;

	if( m_servAddr[0]  == '\0')
	{
		return SERVER_ADDR_FAULT;
	}

	if(setUsers == NULL)
	{
		return PARAMETER_FAULT;
	}

	size = setUsers->sizeUser;
	appUser= setUsers->User;

	user.__sizeUser = size;
	user.User  = (struct tt__User *)malloc(size *sizeof(struct tt__User));
	if(user.User == NULL)
	{
		return OPERATE_FAILED;
	}
	memset(user.User,0,size *sizeof(struct tt__User));
	while(i < size)
	{
		GetDevUserFromApp(devUser,appUser);
		i++;
		appUser++;
		devUser++;
	}
    
	pSoap = onvif_init_soap(TDEVICE_MGT,false);
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

	result = soap_call___tds__SetUser(pSoap, m_servAddr, NULL,&user,&response);

	appUser =NULL;
	devUser =NULL;
	free(user.User);
    user.User=NULL;
	if(SOAP_OK == result)
	{
		eResult = OPERATE_SUCCESS;
	}
	else
	{
		eResult =OPERATE_FAILED;
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

#if 0
void COnvifDevMgt::SetUserName(void)
{
   
}
#endif

//network
void COnvifDevMgt::GetNetworkParameter(struct OnvifNwParameter *nwParameterResp,unsigned int aDigest)
{
    int result = -1; 
	if(nwParameterResp == NULL)
	{
		return ;
	}
	nwParameterResp->ipSuccess = false;
	//nwParameterResp->portSuccess = false;
	nwParameterResp->dnsSuccess = false;
	nwParameterResp->gatewaySuccess = false;

	result = GetDevIpAddr(&(nwParameterResp->ipAddr),aDigest);
	if(result == OPERATE_SUCCESS)
	{
		nwParameterResp->ipSuccess = true;
	}
#if 0
	result = -1;
	result = GetDevPort(&(nwParameterResp->portData));
	if(result == OPERATE_SUCCESS)
	{
		nwParameterResp->portSuccess = true;
	}
#endif

	result = -1;
	result = GetDevDns(&(nwParameterResp->dnsData),aDigest);
	if(result == OPERATE_SUCCESS)
	{
		nwParameterResp->dnsSuccess = true;
	}
	result = -1;
	result = GetDefaultGateway(&(nwParameterResp->gatewayData),aDigest);
	if(result == OPERATE_SUCCESS)
	{
		nwParameterResp->gatewaySuccess = true;
	}

}
void COnvifDevMgt::SetNetworkParameter(struct OnvifNwParameter *nwParameterSet,unsigned int aDigest)
{
	int result = -1; 
	if(nwParameterSet == NULL)
	{
		return ;
	}
	nwParameterSet->ipSuccess = false;
	//nwParameterSet->portSuccess = false;
	nwParameterSet->dnsSuccess = false;
	nwParameterSet->gatewaySuccess = false;
	result = SetDevIpAddr(&(nwParameterSet->ipAddr),aDigest);
	if(result == OPERATE_SUCCESS)
	{
		nwParameterSet->ipSuccess = true;
	}

#if 0
	result = -1;
	result = SetDevPort(&(nwParameterSet->portData));
	if(result == OPERATE_SUCCESS)
	{
		nwParameterSet->portSuccess = true;
	}
#endif
	result = -1;
	result =  SetDevDns(&(nwParameterSet->dnsData),aDigest);
	if(result == OPERATE_SUCCESS)
	{
		nwParameterSet->dnsSuccess = true;
	}
	result = -1;
	if(nwParameterSet->gatewayData.ipv4Enable)
	{
		result = SetDefaultGateway(nwParameterSet->gatewayData.ipv4,true,aDigest);
	}
	if(nwParameterSet->gatewayData.ipv6Enable)
	{
		result = SetDefaultGateway(nwParameterSet->gatewayData.ipv6,false,aDigest);
	}
	if(result == OPERATE_SUCCESS)
	{
		nwParameterSet->gatewaySuccess = true;
	}
}



int COnvifDevMgt::GetDevIpAddr(struct OnvifDevIpAddr *devIpResp,unsigned int aDigest)
{
	enum  EResultType eResult = OTHER_FAULT;
	struct soap *pSoap = NULL;
	struct SOAP_ENV__Header *pHeader =NULL;

    int i= 0;
	int result = -1;
	struct _tds__GetNetworkInterfaces GetNetworkInterfaces;
	struct _tds__GetNetworkInterfacesResponse GetNetworkInterfacesResp;

    struct tt__NetworkInterface *tmpNetwork = NULL; 

	GetNetworkInterfaces.dummy =0;
	GetNetworkInterfacesResp.__sizeNetworkInterfaces = 0;
	GetNetworkInterfacesResp.NetworkInterfaces =NULL;

	if(m_servAddr[0] == '\0')
	{
		return SERVER_ADDR_FAULT;
	}

	if(devIpResp == NULL)
	{
		return PARAMETER_FAULT;
	}

	pSoap = onvif_init_soap(TDEVICE_MGT,false);
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

	result=soap_call___tds__GetNetworkInterfaces(pSoap, m_servAddr,NULL,&GetNetworkInterfaces,&GetNetworkInterfacesResp);

	if(SOAP_OK == result)
	{
		while(i < GetNetworkInterfacesResp.__sizeNetworkInterfaces)
		{
		    tmpNetwork = GetNetworkInterfacesResp.NetworkInterfaces;
			if(tmpNetwork->Enabled)
			{
				strcpy(m_ipSetToken,tmpNetwork->token);
				//get hardware address
				if(tmpNetwork->Info->HwAddress != NULL)
				{
					strcpy(devIpResp->hardwareAddr,tmpNetwork->Info->HwAddress);
				}

				//ipv4 configuration
				if(tmpNetwork->IPv4)
				{
					if(tmpNetwork->IPv4->Config->__sizeManual > 0)
					{				
						devIpResp->ipv4Enable = true;
						devIpResp->ipv4.ipSetType = TIPManual;
						strcpy(devIpResp->ipv4.ipAddr,tmpNetwork->IPv4->Config->Manual->Address);
						devIpResp->ipv4.prefixLength = tmpNetwork->IPv4->Config->Manual->PrefixLength;			    
					}

					if(tmpNetwork->IPv4->Config->DHCP)
					{
						devIpResp->ipv4Enable = true;
						devIpResp->ipv4.ipSetType = TIPFromDHCP;
						strcpy(devIpResp->ipv4.ipAddr,tmpNetwork->IPv4->Config->FromDHCP->Address);
						devIpResp->ipv4.prefixLength = tmpNetwork->IPv4->Config->FromDHCP->PrefixLength;	
					}

					if(tmpNetwork->IPv4->Config->LinkLocal)
					{
						devIpResp->ipv4Enable = true;
						devIpResp->ipv4.ipSetType = TIPLinkLocal;
						strcpy(devIpResp->ipv4.ipAddr,tmpNetwork->IPv4->Config->LinkLocal->Address);
						devIpResp->ipv4.prefixLength = tmpNetwork->IPv4->Config->LinkLocal->PrefixLength;
					}
				}
				//ipv6 configuration
				if(tmpNetwork->IPv6 != NULL)
				{
					if(tmpNetwork->IPv6->Enabled)
					{ 						
						devIpResp->ipv6Enable = true;
						if(tmpNetwork->IPv6->Config->DHCP == tt__IPv6DHCPConfiguration__Auto)
						{
							devIpResp->ipv6.ipv6DhcpConf = TDHCPAuto;
						}
						if(tmpNetwork->IPv6->Config->DHCP == tt__IPv6DHCPConfiguration__Stateful)
						{
							devIpResp->ipv6.ipv6DhcpConf = TDHCPStateful;
						}
						if(tmpNetwork->IPv6->Config->DHCP == tt__IPv6DHCPConfiguration__Stateless)
						{
							devIpResp->ipv6.ipv6DhcpConf = TIPv6DHCPStateless;
						}
						if(tmpNetwork->IPv6->Config->DHCP == tt__IPv6DHCPConfiguration__Off)
						{
							devIpResp->ipv6.ipv6DhcpConf = TIPv6DHCPOff;
						}

						if(tmpNetwork->IPv6->Config->Manual)
						{
							devIpResp->ipv6.ipSetType = TIPManual;
							strcpy(devIpResp->ipv6.ipv6Addr,tmpNetwork->IPv6->Config->Manual->Address);
							devIpResp->ipv6.prefixLength = tmpNetwork->IPv6->Config->Manual->PrefixLength;
						}
						if(tmpNetwork->IPv6->Config->LinkLocal)
						{
							devIpResp->ipv6.ipSetType = TIPLinkLocal;
							strcpy(devIpResp->ipv6.ipv6Addr,tmpNetwork->IPv6->Config->LinkLocal->Address);
							devIpResp->ipv6.prefixLength = tmpNetwork->IPv6->Config->LinkLocal->PrefixLength;
						}
						if(tmpNetwork->IPv6->Config->FromDHCP)
						{
							devIpResp->ipv6.ipSetType = TIPFromDHCP;
							strcpy(devIpResp->ipv6.ipv6Addr,tmpNetwork->IPv6->Config->FromDHCP->Address);
							devIpResp->ipv6.prefixLength = tmpNetwork->IPv6->Config->FromDHCP->PrefixLength;
						}
						if(tmpNetwork->IPv6->Config->FromRA)
						{
							devIpResp->ipv6.ipSetType = TIPFromRA;
							strcpy(devIpResp->ipv6.ipv6Addr,tmpNetwork->IPv6->Config->FromRA->Address);
							devIpResp->ipv6.prefixLength = tmpNetwork->IPv6->Config->FromRA->PrefixLength;
						}
					}
					else
					{
						devIpResp->ipv6Enable = false;
					}
				}
				else
				{
					devIpResp->ipv6Enable = false;
				}

				break;
			}
			else
			{
				i++;
				tmpNetwork++;
			}
		}

       tmpNetwork = NULL;
		GetNetworkInterfacesResp.__sizeNetworkInterfaces = 0;
		GetNetworkInterfacesResp.NetworkInterfaces =NULL;
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

int COnvifDevMgt::SetDevIpAddr(struct OnvifDevIpAddr *setParam,unsigned int aDigest)
{
	int result = -1;
	enum EResultType eResult= OTHER_FAULT;
	struct soap *pSoap=NULL;
	struct SOAP_ENV__Header *pHeader =NULL;

	xsd__boolean_ enable = _true;
	xsd__boolean_ ipv4Enable = _true;
	xsd__boolean_ ipv4Dhcp = _true;

	xsd__boolean_ ipv6Enable = _true;
	xsd__boolean_ ipv6AcceptRouterAdvert = _false;
	enum tt__IPv6DHCPConfiguration ipv6DHCP;

	struct tt__NetworkInterfaceSetConfiguration network_inter;

	struct tt__IPv4NetworkInterfaceSetConfiguration ipv4;
	struct tt__PrefixedIPv4Address manual_ip4;

	struct tt__IPv6NetworkInterfaceSetConfiguration ipv6;
	struct tt__PrefixedIPv6Address manual_ip6;


	struct _tds__SetNetworkInterfaces SetInterfaces;
	struct _tds__SetNetworkInterfacesResponse Resp;

	if(m_servAddr[0] == '\0')
	{
		return SERVER_ADDR_FAULT;
	}

	if(setParam == NULL)
	{
		return PARAMETER_FAULT;
	}
		
	enable = _true;
	
	if(setParam->ipv4Enable)
	{

		manual_ip4.Address = setParam->ipv4.ipAddr;
		manual_ip4.PrefixLength = setParam->ipv4.prefixLength;

		ipv4Enable = _true;

		if(setParam->ipv4.dhcp)
		{
			ipv4Dhcp = _true;
		}
		else
		{
			ipv4Dhcp = _false;
		}
		ipv4.Enabled = &ipv4Enable;
		ipv4.DHCP = &ipv4Dhcp;
		ipv4.__sizeManual = 1;
		ipv4.Manual = &manual_ip4;
		network_inter.IPv4 = &ipv4;
	}
	else
	{
		network_inter.IPv4 = NULL;
	}

	if(setParam->ipv6Enable)
	{

	   ipv6Enable = _true;
       
		manual_ip6.Address = setParam->ipv6.ipv6Addr;
		manual_ip6.PrefixLength = setParam->ipv6.prefixLength;

		switch(setParam->ipv6.ipv6DhcpConf)
		{
		case TDHCPAuto:
			ipv6DHCP = tt__IPv6DHCPConfiguration__Auto;
			break;
		case TDHCPStateful:
			ipv6DHCP = tt__IPv6DHCPConfiguration__Stateful;			
			break;
		case TIPv6DHCPStateless:
			ipv6DHCP = tt__IPv6DHCPConfiguration__Stateless;		
			break;
		case TIPv6DHCPOff:
			ipv6DHCP = tt__IPv6DHCPConfiguration__Off;			
			break;
		default :
			break;
		}
#if 0
		if(setParam->ipv6Set.acceptRouterAdvert)
		{
			ipv6AcceptRouterAdvert = _true;
		}
		else
		{

			ipv6AcceptRouterAdvert = _false;
		}
#endif
		ipv6.Enabled = &ipv6Enable;
		ipv6.AcceptRouterAdvert = NULL;//&ipv6AcceptRouterAdvert;
		ipv6.DHCP = &ipv6DHCP;
		ipv6.__sizeManual = 1;
		if(setParam->ipv6.ipSetType == TIPManual)
			ipv6.Manual = &manual_ip6;
		else
			ipv6.Manual = NULL;

		network_inter.IPv6 = &ipv6;
	}
	else
	{
		network_inter.IPv6 = NULL;
	}

	network_inter.Enabled = &enable;
	network_inter.__anyAttribute = NULL;
	network_inter.Extension = NULL;
	network_inter.Link = NULL;
	network_inter.MTU = NULL;

	SetInterfaces.InterfaceToken = m_ipSetToken;
	SetInterfaces.NetworkInterface =&network_inter;

	pSoap = onvif_init_soap(TDEVICE_MGT,false);
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

	result = soap_call___tds__SetNetworkInterfaces(pSoap, m_servAddr, NULL,&SetInterfaces,&Resp);

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

int COnvifDevMgt::GetDevPort(struct OnvifProtocolProt *getPortParam,unsigned int aDigest)
{
	int result = -1;
	enum EResultType eResult= OTHER_FAULT;
	struct soap *pSoap=NULL;
	struct SOAP_ENV__Header *pHeader =NULL;
	int i = 0;
	int size = 0;
	struct _tds__GetNetworkProtocols GetNetworkProtocols;
	struct _tds__GetNetworkProtocolsResponse GetNetworkProtocolsResp;
	struct tt__NetworkProtocol *devProtocol =NULL;

	if(m_servAddr[0] == '\0')
	{
		return SERVER_ADDR_FAULT;
	}

	if(getPortParam == NULL)
	{
		return PARAMETER_FAULT;
	}

	GetNetworkProtocols.dummy = 0;

	pSoap = onvif_init_soap(TDEVICE_MGT,false);
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

	result =soap_call___tds__GetNetworkProtocols(pSoap, m_servAddr, NULL,&GetNetworkProtocols,&GetNetworkProtocolsResp);

	if(SOAP_OK == result)
	{	
		size = GetNetworkProtocolsResp.__sizeNetworkProtocols;
		if(size > 0)
		{
			devProtocol = GetNetworkProtocolsResp.NetworkProtocols;
			while(i <size)
			{

				if(devProtocol->Name == tt__NetworkProtocolType__HTTP)
				{
					getPortParam->httpPort = *(devProtocol->Port);
					if(devProtocol->Enabled)
					{
                       getPortParam->bHttpEnable = true;
					}
					else
					{
                       getPortParam->bHttpEnable = false;
					}
				}

				if(devProtocol->Name == tt__NetworkProtocolType__HTTPS)
				{
					getPortParam->httpsPort = *(devProtocol->Port);
					if(devProtocol->Enabled)
					{
						getPortParam->bHttpsEnable = true;
					}
					else
					{
						getPortParam->bHttpsEnable = false;
					}
				}

				if(devProtocol->Name == tt__NetworkProtocolType__RTSP)
				{
					getPortParam->rtspPort = *(devProtocol->Port);
					if(devProtocol->Enabled)
					{
						getPortParam->bRtspEnable = true;
					}
					else
					{
						getPortParam->bRtspEnable = false;
					}
				}
				i++;
				devProtocol++;

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

int COnvifDevMgt::SetDevPort(struct OnvifProtocolProt *portSetParam,unsigned int aDigest)
{
	int result = -1;
	enum EResultType eResult= OTHER_FAULT;
	struct soap *pSoap=NULL;
	struct SOAP_ENV__Header *pHeader =NULL;
	int size =0;
	int i= 0;
	int httpPort = 0,httpsPort = 0,rtspPort=0;
	struct _tds__SetNetworkProtocols protocol;	
	struct _tds__SetNetworkProtocolsResponse protocolResp;
	struct tt__NetworkProtocol *devProtocol = NULL;

	if(portSetParam == NULL)
	{
		return PARAMETER_FAULT;
	}

	if(m_servAddr[0] == '\0')
	{
		return SERVER_ADDR_FAULT;
	}
	size = 3;
	if(size > 0)
	{
		protocol.__sizeNetworkProtocols = size;
		protocol.NetworkProtocols = (struct tt__NetworkProtocol *)malloc(size *sizeof(struct tt__NetworkProtocol));
		if(protocol.NetworkProtocols == NULL)
		{
			return OPERATE_FAILED;
		}
		devProtocol = protocol.NetworkProtocols;

		if(portSetParam->bHttpEnable)
		{
			devProtocol->Enabled = _true;
		}
		else
		{
			devProtocol->Enabled = _false;
		}
		httpPort = portSetParam->httpPort;
		devProtocol->Name = tt__NetworkProtocolType__HTTP; 
		devProtocol->__sizePort = 1;
		devProtocol->Port = &httpPort;
		devProtocol->__anyAttribute = NULL;
		devProtocol->Extension = NULL;
		devProtocol++;
			
		if(portSetParam->bHttpsEnable)
		{
          devProtocol->Enabled = _true;
		}
		else
		{
			devProtocol->Enabled = _false;
		}
		httpsPort = portSetParam->httpsPort;
		devProtocol->Name = tt__NetworkProtocolType__HTTPS;
		devProtocol->__sizePort = 1;
		devProtocol->Port = &httpsPort;
		devProtocol->__anyAttribute = NULL;
		devProtocol->Extension = NULL;
		devProtocol++;

		if(portSetParam->bRtspEnable)
		{
          devProtocol->Enabled = _true;
		}
		else
		{
			devProtocol->Enabled = _false;
		}
		rtspPort = portSetParam->rtspPort;
		devProtocol->Name = tt__NetworkProtocolType__RTSP;
		devProtocol->__sizePort = 1;
		devProtocol->Port = &rtspPort;
		devProtocol->__anyAttribute = NULL;
		devProtocol->Extension = NULL;
		devProtocol++;	
	}
    devProtocol= NULL;
	pSoap = onvif_init_soap(TDEVICE_MGT,false);
	if(pSoap == NULL)
	{
		if(protocol.NetworkProtocols != NULL)
		{
			free(protocol.NetworkProtocols);		
		}
		protocol.NetworkProtocols = NULL;
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

	result =soap_call___tds__SetNetworkProtocols(pSoap,m_servAddr,NULL,&protocol,&protocolResp);

	if(protocol.NetworkProtocols != NULL)
	{
		free(protocol.NetworkProtocols);		
	}
	protocol.NetworkProtocols = NULL;

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

int COnvifDevMgt::GetDevDns(struct OnvifDnsData *dnsResp,unsigned int aDigest)
{
	enum EResultType eResult= OTHER_FAULT;
	struct soap *pSoap=NULL;
	struct SOAP_ENV__Header *pHeader =NULL;
	struct tt__DNSInformation *p_DNSInformation =NULL;
	struct tt__IPAddress *ipaddr=NULL;

	struct _tds__GetDNS GetDNS;
	struct _tds__GetDNSResponse GetDNSResponse;

	int result =-1;
	int i =0;
	GetDNS.dummy = 0;
	GetDNSResponse.DNSInformation = NULL;

	if(m_servAddr[0] == '\0')
	{
		return SERVER_ADDR_FAULT;
	}
	if(dnsResp == NULL)
	{
		return PARAMETER_FAULT;
	}
	pSoap = onvif_init_soap(TDEVICE_MGT,false);
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

	result = soap_call___tds__GetDNS(pSoap, m_servAddr, NULL,&GetDNS,&GetDNSResponse);

	if(SOAP_OK == result)
	{
		p_DNSInformation = GetDNSResponse.DNSInformation;
		if(p_DNSInformation->FromDHCP)
		{
			dnsResp->ipsource = TDhcpType;
			dnsResp->dnsIpSize = p_DNSInformation->__sizeDNSFromDHCP;
			ipaddr = p_DNSInformation->DNSFromDHCP;
		}
		else if(p_DNSInformation->__sizeDNSManual >0)
		{
			dnsResp->ipsource = TManualType;
			dnsResp->dnsIpSize = p_DNSInformation->__sizeDNSManual;
			ipaddr = p_DNSInformation->DNSManual;
		}

		dnsResp->dnsIp = (struct OnvifIpAddr *)malloc(sizeof(struct OnvifIpAddr)*(dnsResp->dnsIpSize));
		if(dnsResp->dnsIp == NULL)
		{
			return OPERATE_FAILED;
		}

		while(i < dnsResp->dnsIpSize)
		{
			if(ipaddr->Type)
			{
				dnsResp->dnsIp->ipv4Type = false;
				strcpy(dnsResp->dnsIp->ipAddress,*(ipaddr->IPv6Address));
			}
			else
			{
				dnsResp->dnsIp[i].ipv4Type = true;
				strcpy(dnsResp->dnsIp[i].ipAddress,*(ipaddr->IPv4Address));
			}
			ipaddr++;
			i++;
		}

		GetDNSResponse.DNSInformation = NULL;
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

int COnvifDevMgt::SetDevDns(struct OnvifDnsData *setDnsParam,unsigned int aDigest)
{
	int result =-1;
	enum EResultType eResult= OTHER_FAULT;
	struct soap *pSoap=NULL;
	struct SOAP_ENV__Header *pHeader =NULL;
	int i = 0;
	char *addressPtr=NULL;
	struct tt__IPAddress *dnsIP=NULL;
	struct _tds__SetDNS SetDNS;
	struct _tds__SetDNSResponse SetDNSResponse;

	SetDNSResponse.dummy = 0;
	if(m_servAddr[0] == '\0')
	{
		return SERVER_ADDR_FAULT;
	}

	if(setDnsParam == NULL)
	{
		return PARAMETER_FAULT;
	}
	if(setDnsParam->ipsource == TDhcpType)
	{
		SetDNS.FromDHCP = _true;
	}
	else
	{
		SetDNS.FromDHCP = _false;
	}

	SetDNS.__sizeDNSManual = setDnsParam->dnsIpSize;
	SetDNS.DNSManual = (struct tt__IPAddress *)malloc(sizeof(struct tt__IPAddress) * SetDNS.__sizeDNSManual);
	if(SetDNS.DNSManual == NULL)
	{
		return PARAMETER_FAULT;
	}
	dnsIP = SetDNS.DNSManual;
	while(i < setDnsParam->dnsIpSize)
	{
		addressPtr = setDnsParam->dnsIp[i].ipAddress;
		if(setDnsParam->dnsIp[i].ipv4Type)
		{
			dnsIP->Type = tt__IPType__IPv4;
			dnsIP->IPv4Address =&addressPtr;
			dnsIP->IPv6Address  = NULL;
		}
		else
		{
			dnsIP->Type = tt__IPType__IPv6;
			dnsIP->IPv6Address = &addressPtr;
			dnsIP->IPv4Address  = NULL; 
		}
		dnsIP++;
		i++;
	}
	SetDNS.__sizeSearchDomain = 0;
	SetDNS.SearchDomain =NULL;

	pSoap = onvif_init_soap(TDEVICE_MGT,false);
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

	result = soap_call___tds__SetDNS(pSoap,m_servAddr,NULL,&SetDNS,&SetDNSResponse);
	if(SetDNS.DNSManual != NULL)
	{
		free(SetDNS.DNSManual);
	}

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

int COnvifDevMgt::GetDefaultGateway(struct OnvifGateWayIP *gatewayResp,unsigned int aDigest)
{
	int result = -1;
	enum EResultType eResult= OTHER_FAULT;
	struct soap *pSoap=NULL;
	struct SOAP_ENV__Header *pHeader =NULL;
	struct _tds__GetNetworkDefaultGateway GetNetworkDefaultGateway;
	struct _tds__GetNetworkDefaultGatewayResponse GetNetworkDefaultGatewayResp;

	if(m_servAddr[0] == '\0')
	{
		return SERVER_ADDR_FAULT;
	}
	if(gatewayResp == NULL)
	{
		return PARAMETER_FAULT;
	}
	GetNetworkDefaultGateway.dummy = 0;

	pSoap = onvif_init_soap(TDEVICE_MGT,false);
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

	result =soap_call___tds__GetNetworkDefaultGateway(pSoap, m_servAddr,NULL,&GetNetworkDefaultGateway,&GetNetworkDefaultGatewayResp);
	if(SOAP_OK == result)
	{
		if(GetNetworkDefaultGatewayResp.NetworkGateway->IPv4Address)
		{
			gatewayResp->ipv4Enable  = true;
			strcpy(gatewayResp->ipv4,*(GetNetworkDefaultGatewayResp.NetworkGateway->IPv4Address));
		}
		else
		{
          gatewayResp->ipv4Enable = false;
		   gatewayResp->ipv4[0] = '\0';
		}
		
		if(GetNetworkDefaultGatewayResp.NetworkGateway->IPv6Address)
		{
			gatewayResp->ipv6Enable  = true;
			strcpy(gatewayResp->ipv6,*(GetNetworkDefaultGatewayResp.NetworkGateway->IPv6Address));
		}
		else
		{
           gatewayResp->ipv6Enable = false;
		    gatewayResp->ipv6[0] = '\0';
		}
		GetNetworkDefaultGatewayResp.NetworkGateway = NULL;
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

int COnvifDevMgt::SetDefaultGateway(char *gateWayIp,bool ipv4Add,unsigned int aDigest)
{
	int result = -1;
	enum EResultType eResult= OTHER_FAULT;
	struct soap *pSoap=NULL;
	struct SOAP_ENV__Header *pHeader =NULL;
	struct _tds__SetNetworkDefaultGateway SetNetworkDefaultGateway;
	struct _tds__SetNetworkDefaultGatewayResponse SetNetworkDefaultGatewayResp;

	SetNetworkDefaultGateway.__sizeIPv4Address = 0;
	SetNetworkDefaultGateway.__sizeIPv6Address = 0;
	SetNetworkDefaultGateway.IPv4Address =NULL;
	SetNetworkDefaultGateway.IPv6Address =NULL;
	SetNetworkDefaultGatewayResp.dummy = 0;

	if(m_servAddr[0] == '\0')
	{
		return SERVER_ADDR_FAULT;
	}
	if(gateWayIp == NULL)
	{
		return PARAMETER_FAULT;
	}

	if(ipv4Add)
	{
		SetNetworkDefaultGateway.__sizeIPv4Address = 1;
		SetNetworkDefaultGateway.IPv4Address = (char **)&gateWayIp;
	}
	else 
	{
		SetNetworkDefaultGateway.__sizeIPv6Address = 1;
		SetNetworkDefaultGateway.IPv6Address =(char **)&gateWayIp;
	}

	pSoap = onvif_init_soap(TDEVICE_MGT,false);
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

	result =soap_call___tds__SetNetworkDefaultGateway(pSoap, m_servAddr, NULL, &SetNetworkDefaultGateway,&SetNetworkDefaultGatewayResp);
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

int COnvifDevMgt::GetNtpSetting(struct OnvifNTPData *getNtpData,unsigned int aDigest)
{
	enum EResultType eResult= OTHER_FAULT;
	struct soap *pSoap=NULL;
	struct SOAP_ENV__Header *pHeader =NULL;
	int result = -1;
	int size= 0;
	int i = 0;
	struct tt__NTPInformation *p_NTPInformation =NULL;
	struct tt__NetworkHost *NTPData=NULL;
	struct _tds__GetNTP GetNTP;
	struct _tds__GetNTPResponse GetNTPResponse;

	GetNTP.dummy = 0;
	GetNTPResponse.NTPInformation = NULL;
	if(m_servAddr[0] == '\0')
	{
		return SERVER_ADDR_FAULT;
	}
	if(getNtpData == NULL)
	{
		return PARAMETER_FAULT;
	}
	pSoap = onvif_init_soap(TDEVICE_MGT,false);
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

	result =soap_call___tds__GetNTP(pSoap, m_servAddr, NULL,&GetNTP,&GetNTPResponse);

	if(SOAP_OK == result)
	{
		p_NTPInformation = GetNTPResponse.NTPInformation;

		if(p_NTPInformation->FromDHCP)
		{
			size = p_NTPInformation->__sizeNTPFromDHCP;
			getNtpData->FromDHCP = true;
			getNtpData->addrSize = size;
			NTPData = p_NTPInformation->NTPFromDHCP;
		}
		else
		{
			size = p_NTPInformation->__sizeNTPManual;
			getNtpData->FromDHCP = false;
			getNtpData->addrSize = size;
			NTPData = p_NTPInformation->NTPManual;
		}

		getNtpData->addr = (struct OnvifNTPAddr *)malloc(sizeof(struct OnvifNTPAddr)*size);
		if(getNtpData->addr == NULL)
		{
			return -1;
		}
		while(i < size)
		{
			if(NTPData->Type == tt__NetworkHostType__IPv4)
			{
				getNtpData->addr->type = TIpv4;
				strcpy(getNtpData->addr[i].ipAddr.ipAddress,*(NTPData->IPv4Address));
			}
			else if(NTPData->Type == tt__NetworkHostType__IPv6)
			{
				getNtpData->addr->type = TIpv6;
				strcpy(getNtpData->addr[i].ipAddr.ipAddress,*(NTPData->IPv6Address));
			}
			else if(NTPData->Type == tt__NetworkHostType__DNS)
			{
				getNtpData->addr->type = TDNSName;
				strcpy(getNtpData->addr[i].DNSName,*(NTPData->DNSname));
			}
			NTPData++;
			i++;
		}

		NTPData = NULL;
		p_NTPInformation = NULL;
		GetNTPResponse.NTPInformation =NULL;
		eResult=OPERATE_SUCCESS;
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

int COnvifDevMgt::SetNtpSetting(struct OnvifNTPData *setNtpData,unsigned int aDigest)
{
	enum EResultType eResult= OTHER_FAULT;
	struct soap *pSoap=NULL;
	struct SOAP_ENV__Header *pHeader =NULL;
	int result = -1;
	int i =0;
	int size = 0;
	char *tmp=NULL;
	struct tt__NetworkHost *NTPManual = NULL;
	struct _tds__SetNTP SetNTP;
	struct _tds__SetNTPResponse SetNTPResponse;
    
	SetNTP.__sizeNTPManual = 0;
	SetNTP.NTPManual = NULL;
	SetNTP.FromDHCP  = _false;
	SetNTPResponse.dummy = 0;

	if(m_servAddr[0] == '\0')
	{
		return SERVER_ADDR_FAULT;
	}
	if(setNtpData == NULL)
	{
		return PARAMETER_FAULT;
	}
	if(setNtpData->FromDHCP == true)
	{
       SetNTP.FromDHCP = _true;
	}
	else
	{
		SetNTP.FromDHCP = _false;
	}
	size = setNtpData->addrSize;
	if(size > 0)
	{
		SetNTP.__sizeNTPManual = size;
		SetNTP.NTPManual = (struct tt__NetworkHost *)malloc(sizeof(struct tt__NetworkHost)*size);
		if(SetNTP.NTPManual == NULL)
		{
			return -1;
		}
	}

	NTPManual = SetNTP.NTPManual;
	while(i< size)
	{
		if(setNtpData->addr->type == TIpv4)
		{
			NTPManual->Type = tt__NetworkHostType__IPv4;
			tmp = (setNtpData->addr[i].ipAddr).ipAddress;
			NTPManual->IPv4Address = &tmp;
			NTPManual->DNSname = NULL;
			NTPManual->IPv6Address = NULL;
		}
		if(setNtpData->addr->type == TIpv6)
		{
			NTPManual->Type = tt__NetworkHostType__IPv6;
			tmp = setNtpData->addr[i].ipAddr.ipAddress;
			NTPManual->IPv6Address = &tmp;
			NTPManual->DNSname = NULL;
			NTPManual->IPv4Address = NULL;
		}
		if(setNtpData->addr->type == TDNSName)
		{
			NTPManual->Type = tt__NetworkHostType__DNS;
			tmp = setNtpData->addr[i].DNSName;
			NTPManual->DNSname = &tmp;
			NTPManual->IPv4Address = NULL;
			NTPManual->IPv6Address = NULL;
		}
		NTPManual->__anyAttribute = NULL;
		NTPManual->Extension = NULL;
		NTPManual ++;
		i++;
	}

	pSoap = onvif_init_soap(TDEVICE_MGT,false);
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

	result =soap_call___tds__SetNTP(pSoap, m_servAddr, NULL,&SetNTP,&SetNTPResponse);
	NTPManual = NULL;

	if(SetNTP.NTPManual != NULL)
	{
		free(SetNTP.NTPManual);
	}

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

int COnvifDevMgt::GetDynamicDnsSetting(struct OnvifDDnsStruc *getDDnsData,unsigned int aDigest)
{
	enum EResultType eResult= OTHER_FAULT;
	struct soap *pSoap=NULL;
	struct SOAP_ENV__Header *pHeader =NULL;
	int result = -1;

	struct _tds__GetDynamicDNS GetDynamicDNS;
	struct _tds__GetDynamicDNSResponse GetDynamicDNSResp;

	GetDynamicDNS.dummy = 1;
	GetDynamicDNSResp.DynamicDNSInformation =NULL;

	if(m_servAddr[0] == '0')
	{
		return SERVER_ADDR_FAULT;
	}
	if(getDDnsData == NULL)
	{
		return PARAMETER_FAULT;
	}
	pSoap = onvif_init_soap(TDEVICE_MGT,false);
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

	result = soap_call___tds__GetDynamicDNS(pSoap, m_servAddr, NULL,&GetDynamicDNS,&GetDynamicDNSResp);


	if(SOAP_OK == result)
	{
		if(GetDynamicDNSResp.DynamicDNSInformation->Type == tt__DynamicDNSType__NoUpdate)
		{
			getDDnsData->Type  = TDDNSNoUpdate;
		}
		if(GetDynamicDNSResp.DynamicDNSInformation->Type == tt__DynamicDNSType__ClientUpdates)
		{
			getDDnsData->Type  = TDDNSClientUpdates;
		}
		if(GetDynamicDNSResp.DynamicDNSInformation->Type == tt__DynamicDNSType__ServerUpdates)
		{
			getDDnsData->Type  = TDDNSServerUpdates;
		}
		strcpy(getDDnsData->Name,*(GetDynamicDNSResp.DynamicDNSInformation->Name));
		strcpy(getDDnsData->TTL,GetDynamicDNSResp.DynamicDNSInformation->TTL);
		GetDynamicDNSResp.DynamicDNSInformation =NULL;
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

int COnvifDevMgt::SetDynamicDnsSetting(struct OnvifDDnsStruc *setDDnsData,unsigned int aDigest)
{
	enum EResultType eResult= OTHER_FAULT;
	struct soap *pSoap=NULL;
	struct SOAP_ENV__Header *pHeader =NULL;
	int result = -1;
	char *tmp=NULL;
	struct _tds__SetDynamicDNS SetDynamicDNS;
	struct _tds__SetDynamicDNSResponse SetDynamicDNSResp;

	SetDynamicDNSResp.dummy = 0;
	if(m_servAddr[0] == '\0')
	{
		return SERVER_ADDR_FAULT;
	}
	if(setDDnsData == NULL)
	{
		return PARAMETER_FAULT;
	}

	if(setDDnsData->Type == TDDNSNoUpdate)
	{
		SetDynamicDNS.Type = tt__DynamicDNSType__NoUpdate;
	}
	if(setDDnsData->Type == TDDNSClientUpdates)
	{
		SetDynamicDNS.Type = tt__DynamicDNSType__ClientUpdates;
	}
	if(setDDnsData->Type == TDDNSServerUpdates)
	{
		SetDynamicDNS.Type = tt__DynamicDNSType__ServerUpdates;
	}
	tmp= setDDnsData->Name;
	SetDynamicDNS.Name = &tmp;
	SetDynamicDNS.TTL = setDDnsData->TTL;

	pSoap = onvif_init_soap(TDEVICE_MGT,false);
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

	result = soap_call___tds__SetDynamicDNS(pSoap, m_servAddr, NULL,&SetDynamicDNS,&SetDynamicDNSResp);
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

int COnvifDevMgt::GetHostName(char *hostNameParam,int size,unsigned int aDigest)
{
	enum EResultType eResult= OTHER_FAULT;
	struct soap *pSoap=NULL;
	struct SOAP_ENV__Header *pHeader =NULL;

	int result = -1;
	int len = 0;
	struct _tds__GetHostname GetHostname;
	struct _tds__GetHostnameResponse GetHostnameResponse;

	GetHostname.dummy  = 0;
	GetHostnameResponse.HostnameInformation =NULL;
	if(m_servAddr[0] == '\0')
	{
		return SERVER_ADDR_FAULT;
	}
	if(hostNameParam == NULL)
	{
		return PARAMETER_FAULT;
	}

	pSoap = onvif_init_soap(TDEVICE_MGT,false);
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

	result = soap_call___tds__GetHostname(pSoap, m_servAddr, NULL, &GetHostname,&GetHostnameResponse);
	if(SOAP_OK == result)
	{      

		len = strlen(GetHostnameResponse.HostnameInformation->Name);
		if(len > size)
		{
			GetHostnameResponse.HostnameInformation = NULL;
			return OPERATE_FAILED;
		}
		strcpy(hostNameParam,GetHostnameResponse.HostnameInformation->Name);
		GetHostnameResponse.HostnameInformation = NULL;
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

int COnvifDevMgt::SetHostName(char *setHostName,unsigned int aDigest)
{
	enum EResultType eResult= OTHER_FAULT;
	struct soap *pSoap=NULL;
	struct SOAP_ENV__Header *pHeader =NULL;

	int result = -1;
	struct _tds__SetHostname SetHostname;
	struct _tds__SetHostnameResponse SetHostnameResponse;

	if(m_servAddr[0] == '\0')
	{
		return SERVER_ADDR_FAULT;
	}
	if(setHostName == NULL)
	{
		return PARAMETER_FAULT;
	}
	SetHostname.Name = setHostName;
	SetHostnameResponse.dummy = 0;

	pSoap = onvif_init_soap(TDEVICE_MGT,false);
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

	result = soap_call___tds__SetHostname(pSoap, m_servAddr, NULL,&SetHostname,&SetHostnameResponse);

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

int COnvifDevMgt::SetHostNameDHCP(bool fromDhcpParam,unsigned int aDigest)
{
	enum EResultType eResult= OTHER_FAULT;
	struct soap *pSoap=NULL;
	struct SOAP_ENV__Header *pHeader =NULL;
	int result =-1;
	struct _tds__SetHostnameFromDHCP SetHostnameFromDHCP;
	struct _tds__SetHostnameFromDHCPResponse SetHostnameFromDHCPResponse;

	if(m_servAddr[0] == '\0')
	{
		return SERVER_ADDR_FAULT;
	}
	if(fromDhcpParam)
	{
		SetHostnameFromDHCP.FromDHCP = _true;
	}
	else
	{
		SetHostnameFromDHCP.FromDHCP = _false;
	}
	pSoap = onvif_init_soap(TDEVICE_MGT,false);
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

	result = soap_call___tds__SetHostnameFromDHCP(pSoap, m_servAddr, NULL,&SetHostnameFromDHCP,&SetHostnameFromDHCPResponse);
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

//ip filter
int COnvifDevMgt::GetIpAddressFilter(struct OnvifIpAddrFilter *ipListParam,unsigned int aDigest) 
{
	int result = -1;
	int i =0;
	int size = 0;
	enum EResultType eResult= OTHER_FAULT;
	struct soap *pSoap=NULL;
	struct SOAP_ENV__Header *pHeader =NULL;
	struct _tds__GetIPAddressFilter Filter;
	struct _tds__GetIPAddressFilterResponse Resp;
	struct tt__IPAddressFilter *IPAddressFilter =NULL;

	struct tt__PrefixedIPv4Address *IPv4Address = NULL;
	struct tt__PrefixedIPv6Address *IPv6Address = NULL;

	Filter.dummy =0;
	Resp.IPAddressFilter = NULL;

	if(m_servAddr[0] == '\0')
	{
		return SERVER_ADDR_FAULT;
	}
	if(ipListParam == NULL)
	{
		return PARAMETER_FAULT;
	}

	pSoap = onvif_init_soap(TDEVICE_MGT,false);
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

	result =soap_call___tds__GetIPAddressFilter(pSoap, m_servAddr, NULL,&Filter,&Resp);

	if(SOAP_OK == result)
	{
		if(Resp.IPAddressFilter->Type == tt__IPAddressFilterType__Allow)
		{
			ipListParam->filterType = FilterType__Allow;
		}
		if(Resp.IPAddressFilter->Type == tt__IPAddressFilterType__Deny)
		{
			ipListParam->filterType = FilterType__Deny;
		}

		if(Resp.IPAddressFilter->__sizeIPv4Address > 0)
		{		
			size = Resp.IPAddressFilter->__sizeIPv4Address;
			ipListParam->ipv4Size = size;
			ipListParam->ipv4Addr = (struct OnvifIpFilter *)malloc(sizeof(struct OnvifIpFilter)*size);
			if(ipListParam->ipv4Addr == NULL)
			{
				return OPERATE_FAILED;
			}

			//IPv4Address = GetIPAddressFilterResp.IPAddressFilter->IPv4Address;
			i = 0;
			while(i < size)
			{               
				strcpy(ipListParam->ipv4Addr[i].ip,Resp.IPAddressFilter->IPv4Address[i].Address);
				ipListParam->ipv4Addr[i].prefixlength = Resp.IPAddressFilter->IPv4Address[i].PrefixLength;
				IPv4Address++;
				i++;
			}
		}

		if(Resp.IPAddressFilter->__sizeIPv6Address > 0)
		{
			size = Resp.IPAddressFilter->__sizeIPv6Address;
			ipListParam->ipv6Size = size;

			ipListParam->ipv6Addr = (struct OnvifIpFilter *)malloc(sizeof(struct OnvifIpFilter)*size);
			if(ipListParam->ipv6Addr == NULL)
			{
				return OPERATE_FAILED;
			}
			i =0;
			while(i<ipListParam->ipv6Size)
			{
				strcpy(ipListParam->ipv6Addr[i].ip, Resp.IPAddressFilter->IPv6Address[i].Address);
				ipListParam->ipv6Addr[i].prefixlength = Resp.IPAddressFilter->IPv6Address[i].PrefixLength;
				i++;
			}
		}

		Resp.IPAddressFilter = NULL;
		eResult = OPERATE_SUCCESS;
	}
	else
	{
		eResult = OPERATE_FAILED;
	}

	IPAddressFilter =NULL;

	IPv4Address = NULL;
	IPv6Address = NULL;

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

void COnvifDevMgt::InitDevFilterAddr(struct tt__IPAddressFilter *devFilter,struct OnvifIpAddrFilter *appFilter)
{
	int i= 0;
	int size = 0;
	if(devFilter == NULL)
	{
		return;
	}

	if(appFilter == NULL)
	{
		devFilter->__sizeIPv4Address = 0;
		devFilter->IPv4Address =NULL;
		devFilter->__sizeIPv6Address = 0;
		devFilter->IPv6Address = NULL;
		devFilter->Type = tt__IPAddressFilterType__Allow;
		devFilter->Extension = NULL;
		devFilter->__anyAttribute = NULL;
		return;
	}

	if(appFilter->filterType == FilterType__Allow)
	{
		devFilter->Type = tt__IPAddressFilterType__Allow;
	}

	if(appFilter->filterType == FilterType__Deny)
	{
		devFilter->Type = tt__IPAddressFilterType__Deny;
	}

	if(appFilter->ipv4Size > 0)
	{
		size = appFilter->ipv4Size;
		devFilter->__sizeIPv4Address = size;
		if(size > 0)
		{
			devFilter->IPv4Address = (struct tt__PrefixedIPv4Address *)malloc(sizeof(struct tt__PrefixedIPv4Address)*size);
			if(devFilter->IPv4Address  == NULL)
			{
				return;
			}
			for(i= 0;i<size;i++)
			{
				devFilter->IPv4Address[i].PrefixLength = appFilter->ipv4Addr[i].prefixlength;
				devFilter->IPv4Address[i].Address = appFilter->ipv4Addr[i].ip;
			}
		}

	}
	if(appFilter->ipv6Size > 0)
	{
		size = appFilter->ipv6Size;
		devFilter->__sizeIPv6Address = size;
		if(devFilter->__sizeIPv6Address > 0)
		{
			devFilter->IPv6Address = (struct tt__PrefixedIPv6Address *)malloc(sizeof(struct tt__PrefixedIPv6Address)*size);
			if(devFilter->IPv6Address  == NULL)
			{
				return;
			}
			for(i= 0;i<devFilter->__sizeIPv6Address;i++)
			{
				devFilter->IPv6Address[i].PrefixLength = appFilter->ipv6Addr[i].prefixlength;
				devFilter->IPv6Address[i].Address = appFilter->ipv6Addr[i].ip;
			}
		}
	} 
	devFilter->Extension = NULL;
	devFilter->__anyAttribute = NULL;
}

int COnvifDevMgt::SetIpAddressFilter(struct OnvifIpAddrFilter *setFilter,unsigned int aDigest) 
{
	int result = -1;
	enum EResultType eResult= OTHER_FAULT;
	struct soap *pSoap=NULL;
	struct SOAP_ENV__Header *pHeader =NULL;
	struct tt__IPAddressFilter filter;
	struct _tds__SetIPAddressFilter SetIPAddressFilter;
	struct _tds__SetIPAddressFilterResponse SetIPAddressFilterResp;

	if(m_servAddr[0] == '\0')
	{
		return SERVER_ADDR_FAULT;
	}
	if(setFilter == NULL)
	{
		return PARAMETER_FAULT;
	}

	InitDevFilterAddr(&filter,setFilter);
	SetIPAddressFilter.IPAddressFilter = &filter;

	pSoap = onvif_init_soap(TDEVICE_MGT,false);
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

	result =soap_call___tds__SetIPAddressFilter(pSoap, m_servAddr, NULL,&SetIPAddressFilter,&SetIPAddressFilterResp);

	if(filter.IPv4Address != NULL)
	{
		free(filter.IPv4Address);
	}
	filter.IPv4Address = NULL;

	if(filter.IPv6Address != NULL)
	{
		free(filter.IPv6Address);
	}
	filter.IPv6Address = NULL;

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

int COnvifDevMgt::AddIpAddressFilter(struct OnvifIpAddrFilter *addIpFilterParam,unsigned int aDigest) 
{
	int result = -1;
	int i = 0;
	enum EResultType eResult= OTHER_FAULT;
	struct soap *pSoap=NULL;
	struct SOAP_ENV__Header *pHeader =NULL;
	struct tt__IPAddressFilter IPAddressFilter;

	struct _tds__AddIPAddressFilter AddIPAddress;
	struct _tds__AddIPAddressFilterResponse AddIPAddressResp;

	if(m_servAddr[0] == '\0')
	{
		return SERVER_ADDR_FAULT;
	}
	if(addIpFilterParam == NULL)
	{
		return PARAMETER_FAULT;
	}

	InitDevFilterAddr(&IPAddressFilter,addIpFilterParam);
	AddIPAddress.IPAddressFilter = &IPAddressFilter;
	AddIPAddressResp.dummy = 0;

	pSoap = onvif_init_soap(TDEVICE_MGT,false);
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

	result =soap_call___tds__AddIPAddressFilter(pSoap, m_servAddr, NULL,&AddIPAddress,&AddIPAddressResp);

	if(IPAddressFilter.IPv4Address !=NULL)
	{
		free(IPAddressFilter.IPv4Address);
		IPAddressFilter.IPv4Address =NULL;
	}

	if(IPAddressFilter.IPv6Address != NULL)
	{
		free(IPAddressFilter.IPv6Address);
		IPAddressFilter.IPv6Address =NULL;
	}	
	AddIPAddress.IPAddressFilter = NULL;

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

int COnvifDevMgt::RemoveIpAddressFilter(struct OnvifIpAddrFilter *removeFilter,unsigned int aDigest) 
{
	int result = -1;
	enum EResultType eResult= OTHER_FAULT;
	struct soap *pSoap=NULL;
	struct SOAP_ENV__Header *pHeader =NULL;
	struct tt__IPAddressFilter IPAddressFilter;
	struct _tds__RemoveIPAddressFilter RemoveIPAddressFilter;
	struct _tds__RemoveIPAddressFilterResponse RemoveIPAddressFilterResp;

	if(m_servAddr[0] == '\0')
	{
		return SERVER_ADDR_FAULT;
	}
	if(removeFilter == NULL)
	{
		return PARAMETER_FAULT;
	}

	InitDevFilterAddr(&IPAddressFilter,removeFilter);
	RemoveIPAddressFilter.IPAddressFilter =&IPAddressFilter;
	RemoveIPAddressFilterResp.dummy = 0;

	pSoap = onvif_init_soap(TDEVICE_MGT,false);
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

	result = soap_call___tds__RemoveIPAddressFilter(pSoap, m_servAddr, NULL,&RemoveIPAddressFilter,&RemoveIPAddressFilterResp);

	if(IPAddressFilter.IPv4Address !=NULL)
	{
		free(IPAddressFilter.IPv4Address);
		IPAddressFilter.IPv4Address =NULL;
	}

	if(IPAddressFilter.IPv6Address != NULL)
	{
		free(IPAddressFilter.IPv6Address);
		IPAddressFilter.IPv6Address =NULL;
	}	
	RemoveIPAddressFilter.IPAddressFilter = NULL;

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



//others
void COnvifDevMgt::GetDevUserFromApp(struct tt__User *devUser,struct OnvifUserInfo *appUser)
{
	if(devUser == NULL)
	{
		return;
	}
	if(appUser == NULL)
	{
		devUser->Username = NULL;
		devUser->Password = NULL;
		devUser->UserLevel = tt__UserLevel__Extended;
		devUser->Extension = NULL;
		devUser->__anyAttribute =NULL;
		return;
	}

	devUser->Username = appUser->Username;
	devUser->Password = appUser->Password;
	if(appUser->UserLevel == TAdministrator)
	{
		devUser->UserLevel = tt__UserLevel__Administrator;       
	}
	if(appUser->UserLevel == TOperator)
	{
		devUser->UserLevel = tt__UserLevel__Operator;       
	}
	if(appUser->UserLevel == TUser)
	{
		devUser->UserLevel = tt__UserLevel__User;       
	}
	if(appUser->UserLevel == TAnonymous)
	{
		devUser->UserLevel = tt__UserLevel__Anonymous;       
	}
	if(appUser->UserLevel == TExtended)
	{
		devUser->UserLevel = tt__UserLevel__Extended;       
	}
	devUser->Extension = NULL;
	devUser->__anyAttribute =NULL;

}

void COnvifDevMgt::GetAppUserFromDevice(struct OnvifUserInfo *appUser,struct tt__User *devUser)
{
	if(appUser == NULL)
	{
		return;
	}
	if(devUser == NULL)
	{
		appUser->Username[0] = '\0';
		appUser->Password[0] = '\0';
		appUser->UserLevel = TExtended;
	}
	strcpy(appUser->Username,devUser->Username);
	if(devUser->Password != NULL)
	{
		strcpy(appUser->Password,devUser->Password);
	}

	if(devUser->UserLevel == tt__UserLevel__Administrator)
	{
		appUser->UserLevel = TAdministrator;       
	}
	if(devUser->UserLevel == tt__UserLevel__Operator)
	{
		appUser->UserLevel = TOperator;       
	}
	if(devUser->UserLevel == tt__UserLevel__User)
	{
		appUser->UserLevel = TUser;       
	}
	if(devUser->UserLevel == tt__UserLevel__Anonymous)
	{
		appUser->UserLevel = TAnonymous;       
	}
	if(devUser->UserLevel == tt__UserLevel__Extended)
	{
		appUser->UserLevel = TExtended;       
	}
}
//parameter:
//timezone: time zone
//utctime: in and out,put in local time,and the transform result is also in the utctime
void COnvifDevMgt::LocalToUTC(OnvifDateTime *localTime,struct OnvifDevDate &UTCDate,struct OnvifDevTime &UTCTime)
{
	int hour= 0,minute= 0;
	bool behindUtc=false;
	//char  tmpTZ[64]={'\0'};

	if(localTime == NULL)
	{
		return;
	}
	//GetTimeZoneTime(timeZone,&hour,&minute,&behindUtc);
	hour = localTime->deltaHour;
	minute = localTime->deltaMinute;
    behindUtc = localTime->bBehindUtc;

	if(hour >= 24 || minute >= 60)
	{
		return;
	}

	if(behindUtc)
	{
		//if needed in the application,transform this time to UTC time
		AddTime(localTime,UTCDate,UTCTime);	//modified by luomm in 2013.04.10	    
	}
	else
	{
		//if needed in the application,transform this time to UTC time
		SubtractTime(localTime,UTCDate,UTCTime); //modified by luomm in 2013.04.10		
	}
}

void COnvifDevMgt::UTCToLocal(OnvifDateTime *localTime,struct OnvifDevDate &UTCDate,struct OnvifDevTime &UTCTime)
{
	int hour= 0,minute= 0;
	bool behindUtc=false;

	if(localTime == NULL)
	{
		return;
	}
	//GetTimeZoneTime(timeZone,&hour,&minute,&behindUtc);

	hour = localTime->deltaHour;
	minute = localTime->deltaMinute;
	behindUtc = localTime->bBehindUtc;

	if(hour >= 24 || minute >= 60)
	{
		return;
	}
	if(behindUtc)
	{
		SubtractTime(localTime,UTCDate,UTCTime);     
	}
	else
	{
		AddTime(localTime,UTCDate,UTCTime);
	}
}

bool COnvifDevMgt::leap(int year)
{

	if(!(year % 400))
	{
		return true;
	}

	if(!(year % 4))
	{
		if(year % 100)
		{
			return true;
		}
	}
	return false;
}

void COnvifDevMgt::AddTime(OnvifDateTime *Time,struct OnvifDevDate &UTCDate,struct OnvifDevTime &UTCTime)
{

	int tYear,tMonth,tDay;
	int tHour,tMinute,tSec;
	bool lYear = false;

	tYear	= Time->date.Year;
	tMonth	= Time->date.Month;
	tDay	= Time->date.Day;

	tHour   = Time->time.Hour;
	tMinute = Time->time.Minute;
	tSec	= Time->time.Second;

	tHour += Time->deltaHour;
	tMinute += Time->deltaMinute;

	if(tMinute > 60)
	{		  
		tHour++;
	}
	tMinute = tMinute % 60;
	if(tHour >= 24)
	{
		tDay++;
		lYear = leap(tYear);
		if(tDay > gMonth[lYear][tMonth -1])
		{
			tMonth++;
			tDay= 1;
			if(tMonth >= 12)
			{
				tYear++;
				tMonth = 1;
			}
		}
	}
	tHour = tHour % 24;

	UTCDate.Year = tYear;
	UTCDate.Month = tMonth;
	UTCDate.Day  = tDay;

	UTCTime.Hour = tHour;
	UTCTime.Minute = tMinute;
	UTCTime.Second = tSec;
}

void COnvifDevMgt::SubtractTime(OnvifDateTime *Time,struct OnvifDevDate &UTCDate,struct OnvifDevTime &UTCTime)
{
	int tYear = 0, tMonth = 0,tDay = 0;
	int tHour= 0, tMinute=0,tSec = 0;
	bool lYear = false;

	if(Time == NULL)
	{
		return;
	}

	tYear  = Time->date.Year;
	tMonth = Time->date.Month;
	tDay   = Time->date.Day;

	tHour   = Time->time.Hour;
	tMinute = Time->time.Minute;
	tSec    = Time->time.Second;

	//subtract time
	tMinute -=  Time->deltaMinute;
	tHour   -=  Time->deltaHour;

	if(tMinute < 0)
	{
		tHour--;
	}
    tMinute += 60;
	tMinute %= 60;

	if(tHour < 0)
	{
		tDay--;
		if(tDay <= 0)
		{
			tMonth--;
			if(tMonth <= 0)
			{
				tYear--;
				tMonth = 12;
			}
			lYear = leap(tYear);
			tDay = gMonth[lYear][tMonth - 1];
		}
	}

	tHour += 24;
	tHour %= 24;

	//time change
	UTCDate.Year  = tYear;
	UTCDate.Month = tMonth;
	UTCDate.Day   = tDay;

    UTCTime.Hour  = tHour;
	UTCTime.Minute = tMinute;
	UTCTime.Second = tSec;
}

#if 0
void COnvifDevMgt::InitLocalTZ(void)
{

}

bool COnvifDevMgt::leap(int year)
{

	if(!(year % 400))
	{
		return true;
	}

	if(!(year % 4))
	{
		if(year % 100)
		{
			return true;
		}
	}
	return false;
}


void COnvifDevMgt::AddTime(struct OnvifDevDate *date,struct OnvifDevTime *time)
{
	int year = 0,month = 0,day = 0;
	int hour= 0, minute=0,second = 0;
	bool lYear = false;

	if(date == NULL || time == NULL)
	{
		return;
	}

	//if(Tzone < 0 || Tzone >= 45)
	//{
	//	return;
	//}

	year = date->Year;
	month = date->Month;
	day = date->Day;

	hour = time->Hour;
	minute = time->Minute;
	second = time->Second;

	//add time
	//minute += m_localTz[Tzone].minute;
	//hour += m_localTz[Tzone].hour;

	if(minute > 60)
	{		  
		hour ++;
	}
	minute = minute % 60;
	if(hour >= 24)
	{
		day ++;
		lYear = leap(year);
		if(day > gMonth[lYear][month -1])
		{
			month ++;
			day= 1;
			if(month >= 12)
			{
				year ++;
				month = 1;
			}
		}
	}
	hour = hour % 24;

	//time change
	date->Year =year;
	date->Month = month;
	date->Day =day ;

	time->Hour =hour ;
	time->Minute = minute;
	time->Second = second;
}

void COnvifDevMgt::SubtractTime(struct Date *date,struct Time *time)
{
	int year = 0,month = 0,day = 0;
	int hour= 0, minute=0,second = 0;
	bool lYear = false;

	if(date == NULL || time == NULL)
	{
		return;
	}

	//if(Tzone < 0 || Tzone >= 45)
	//{
	//	return;
//	}

	year = date->Year;
	month = date->Month;
	day = date->Day;

	hour = time->Hour;
	minute = time->Minute;
	second = time->Second;

	//subtract time
	//minute -= m_localTz[Tzone].minute;
	//hour -= m_localTz[Tzone].hour;

	if(minute < 0)
	{
		hour --;
		if(hour < 0)
		{
			day --;
			if(day <= 0)
			{
				month --;
				if(month <= 0)
				{
					year --;
					month = 12;
				}
				lYear = leap(year);
				day = gMonth[lYear][month - 1];
			}
		}
	}
	minute += 60;
	minute %= 60;
	hour += 24;
	hour %= 24;

	//time change
	date->Year =year;
	date->Month = month;
	date->Day =day ;

	time->Hour =hour ;
	time->Minute = minute;
	time->Second = second;
}

void COnvifDevMgt::LocalTime2UTC(struct DateTime *UTCTime,struct DateTime *localTime)
{
	struct Date date;
	struct Time time;
	bool lYear = false;

	if(UTCTime == NULL || localTime == NULL)
	{
		return;
	}

	//if(TZone < 0 || TZone >= 45)
	//{
	//	return;
	//}

	date.Year = localTime->datereq.Year;
	date.Month = localTime->datereq.Month;
	date.Day = localTime->datereq.Day;

	time.Hour = localTime->timereq.Hour;
	time.Minute = localTime->timereq.Minute;
	time.Second = localTime->timereq.Second;


	//if(m_localTz[TZone].behindUTC) //local behind UTC time
	{
		AddTime(&date,&time,m_timeZoneNum);
	}
	//else
	{
		SubtractTime(&date,&time,m_timeZoneNum);
	}
	UTCTime->datereq.Year = date.Year;
	UTCTime->datereq.Month = date.Month;
	UTCTime->datereq.Day = date.Day;

	UTCTime->timereq.Hour = time.Hour;
	UTCTime->timereq.Minute = time.Minute;
	UTCTime->timereq.Second = time.Second; 
}

void COnvifDevMgt::UTC2LocalTime(struct OnvifDevDate *date,struct OnvifDevTime *time,struct tt__DateTime *UTCTime)
{
	struct OnvifDevDate tDate;
	struct OnvifDevTime tTime;

	bool lYear = false;

	if(UTCTime == NULL || date == NULL || time == NULL)
	{
		return;
	}

	//if(TZone < 0 || TZone >= 45)
	//{
	//	return;
	//}

	tDate.Year = UTCTime->Date->Year;
	tDate.Month = UTCTime->Date->Month;
	tDate.Day = UTCTime->Date->Day;

	tTime.Hour = UTCTime->Time->Hour;
	tTime.Minute = UTCTime->Time->Minute;
	tTime.Second = UTCTime->Time->Second;


	//if(m_localTz[TZone].behindUTC) //local behind UTC time
	{		
		SubtractTime(&tDate,&tTime,m_timeZoneNum);
	}
	//else
	{
		AddTime(&tDate,&tTime,m_timeZoneNum);	
	}

	date->Year = date.Year;
	date->Month = date.Month;
	date->Day = date.Day;

	time->Hour = time.Hour;
	time->Minute = time.Minute;
	time->Second = time.Second; 
}
#endif

