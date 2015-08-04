#include "OnvifDevCtlServ.h"

//#include "OnvifDevConfStub.h"
//#include "deviceInputAndOutputStub.h"

COnvifDevCtl::COnvifDevCtl()
{
	memset(m_PTZServAddr,'\0',sizeof(m_PTZServAddr));
	memset(m_devIOServAddr,'\0',sizeof(m_devIOServAddr));
	memset(m_imagingServAddr,'\0',sizeof(m_imagingServAddr));

	memset(m_userName,'\0',sizeof(m_userName));
	memset(m_passWord,'\0',sizeof(m_passWord));
	//m_Iris = -10;

	memset( &stOnvifInputParam, 0, sizeof( ONVIF_INIT_INPUT_PARAM));

}

COnvifDevCtl::~COnvifDevCtl()
{
}

void COnvifDevCtl::InitPTZServAddr(char *PTZServ,char *userName,char *passKey)
{
	if(PTZServ == NULL || userName == NULL || passKey == NULL)
	{
		return;
	}
	strcpy(m_PTZServAddr,PTZServ);
	strcpy(m_userName,userName);
	strcpy(m_passWord,passKey);
}

void COnvifDevCtl::InitImagingServAddr(char *imagingServ,char *userName,char *passKey)
{
    if(imagingServ == NULL || userName == NULL || passKey == NULL)
	{
		return;
	}
	strcpy(m_imagingServAddr,imagingServ);
	strcpy(m_userName,userName);
	strcpy(m_passWord,passKey);
}

void COnvifDevCtl::InitDevUtcTimes( bool bClearUtcTime, char* aServAddr, unsigned int ipcCtrlType )
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

void COnvifDevCtl::InitDevIOServAddr(char *devIOServAddr,char *userName,char *passKey)
{
	if(devIOServAddr == NULL || userName == NULL || passKey == NULL)
	{
		return;
	}
	strcpy(m_devIOServAddr,devIOServAddr);
	strcpy(m_userName,userName);
	strcpy(m_passWord,passKey);
}

//device IO control
int COnvifDevCtl::GetRelayOutputs(struct OnvifRelayOutputs *relayResp,unsigned int aDigest)
{
	int result = -1;
	enum EResultType eResult= OTHER_FAULT;
	struct soap *pSoap=NULL;
    struct SOAP_ENV__Header *pHeader =NULL;

	struct _tds__GetRelayOutputs outputs;
	struct _tds__GetRelayOutputsResponse response;
	int i = 0;
	int size = 0;
	struct tt__RelayOutputSettings *deviceTmp =NULL;
	struct OnvifRelayOutputSet  *appTmp=NULL;

	outputs.dummy = 0;
	response.RelayOutputs = NULL;

	if(m_devIOServAddr[0] == '0')
	{
		return SERVER_ADDR_FAULT;
	}
	if(relayResp == NULL)
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


	result=soap_call___tds__GetRelayOutputs(pSoap,m_devIOServAddr,NULL,&outputs,&response);

	if(SOAP_OK == result)
	{
		size = response.__sizeRelayOutputs;
		if(size > 0)
		{
			deviceTmp = response.RelayOutputs->Properties;
			relayResp->sizeRelayOutputs = size;
			relayResp->RelayOutputs = (struct OnvifRelayOutput *)malloc(size*sizeof(struct OnvifRelayOutput));
			if(relayResp->RelayOutputs == NULL)
			{
				return OPERATE_FAILED;
			}
			appTmp = &(relayResp->RelayOutputs->Properties);
			while(i < size)
			{
				strcpy(relayResp->RelayOutputs->token,response.RelayOutputs->token);
				GetAppRelayOutputSettings(appTmp,deviceTmp);
				i++;
				appTmp ++;
				deviceTmp++;
			}
		}
		else
		{
			return -1;
		}
		appTmp = NULL;
		deviceTmp=NULL;
		response.RelayOutputs =NULL;
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

int COnvifDevCtl::SetRelayOutput(char *relayToken,struct OnvifRelayOutputSet *relatSettingData,unsigned int aDigest)
{
	int result = -1;
	enum EResultType eResult= OTHER_FAULT;
	struct soap *pSoap=NULL;
	struct SOAP_ENV__Header *pHeader =NULL;

	struct _tds__SetRelayOutputSettings outputSettings;
	struct _tds__SetRelayOutputSettingsResponse response;
	struct tt__RelayOutputSettings Properties;

	Properties.DelayTime =NULL;
	outputSettings.RelayOutputToken =NULL;
	outputSettings.Properties = NULL;
	response.dummy = 0;
	if(m_devIOServAddr[0] == '\0')
	{
		return SERVER_ADDR_FAULT;
	}
	if(relayToken == NULL || relatSettingData == NULL)
	{
		return PARAMETER_FAULT;
	}

	SetDevRelayOutputSettings(&Properties,relatSettingData);

	outputSettings.Properties = &Properties;
	outputSettings.RelayOutputToken = relayToken;

	pSoap = onvif_init_soap(TDEVICE_MGT,false);
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


	result =soap_call___tds__SetRelayOutputSettings(pSoap,m_devIOServAddr,NULL,&outputSettings,&response);
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

int COnvifDevCtl::TriggerRelay(char *relayToken,enum EOnvifRelayLogicalState logicalState,unsigned int aDigest)
{
	int result = -1;
	enum EResultType eResult= OTHER_FAULT;
	struct soap *pSoap=NULL;
    struct SOAP_ENV__Header *pHeader =NULL;

	struct _tds__SetRelayOutputState outputState;
	struct _tds__SetRelayOutputStateResponse response;

	outputState.RelayOutputToken =NULL;
	response.dummy = 0;

	if(m_devIOServAddr[0] == '\0')
	{
		return SERVER_ADDR_FAULT;
	}
	if(relayToken == NULL)
	{
		return PARAMETER_FAULT;
	}

	outputState.RelayOutputToken = relayToken;
	if(logicalState == T_RelayLogicalStateActive)
	{
		outputState.LogicalState = tt__RelayLogicalState__active;
	}

	if(logicalState == T_RelayLogicalStateInactive)
	{
		outputState.LogicalState = tt__RelayLogicalState__inactive;
	}

	pSoap = onvif_init_soap(TDEVICE_MGT,false);
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


	result =soap_call___tds__SetRelayOutputState(pSoap,m_devIOServAddr,NULL, &outputState, &response);
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

int COnvifDevCtl::SendAuxilaryCommand(char *sendCommand,char *commandResp,unsigned int aDigest)
{
	int result = -1;
	enum EResultType eResult= OTHER_FAULT;
	struct soap *pSoap=NULL;
    struct SOAP_ENV__Header *pHeader =NULL;

	struct _tds__SendAuxiliaryCommand sendAuxiliaryCommand;
	struct _tds__SendAuxiliaryCommandResponse response;

	sendAuxiliaryCommand.AuxiliaryCommand = NULL;
	response.AuxiliaryCommandResponse =NULL;

	if(m_devIOServAddr[0] == '\0')
	{
		return SERVER_ADDR_FAULT;
	}
	if(sendCommand  == NULL  || commandResp == NULL)
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


	result = soap_call___tds__SendAuxiliaryCommand(pSoap,m_devIOServAddr,NULL,&sendAuxiliaryCommand,&response);

	if(SOAP_OK == result)
	{
		strcpy(commandResp,response.AuxiliaryCommandResponse);
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


//imaging control
int COnvifDevCtl::GetImagingSetting(char *videoSourceToken,struct OnvifDevColor *colorResp,unsigned int aDigest)
{
	int result = -1;
    enum EResultType eResult= OTHER_FAULT;
	struct soap *pSoap=NULL;
    struct SOAP_ENV__Header *pHeader =NULL;

	struct _timg__GetImagingSettings settings;
	struct _timg__GetImagingSettingsResponse response;
	struct tt__ImagingSettings20 *devImagingSet = NULL;

	settings.VideoSourceToken = NULL;
	response.ImagingSettings = NULL;

	if(m_imagingServAddr[0] == '\0')
	{
		return SERVER_ADDR_FAULT;
	}
	if(videoSourceToken == NULL || colorResp == NULL)
	{
		return PARAMETER_FAULT;
	}

	settings.VideoSourceToken = videoSourceToken;

	pSoap = onvif_init_soap(TDEVICE_IMAGING,false);
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


	result =soap_call___timg__GetImagingSettings(pSoap, m_imagingServAddr, NULL,&settings,&response);
	if(result == SOAP_OK)
	{
		devImagingSet = response.ImagingSettings;
       if(devImagingSet->Brightness != NULL)
	   {
		   colorResp->Brightness = *(devImagingSet->Brightness);
	   }
	   else
	   {
		   colorResp->Brightness = 0;
	   }
	   if(devImagingSet->ColorSaturation != NULL)
	   {
			colorResp->ColorSaturation = *(devImagingSet->ColorSaturation);
	   }
	   else
	   {
		   colorResp->ColorSaturation = 0;
	   }
	   if(devImagingSet->Contrast != NULL)
	   {
			colorResp->Contrast = *(devImagingSet->Contrast);
	   }
	   else
	   {
		   colorResp->Contrast = 0;
	   }
       
	   if(devImagingSet->Sharpness != NULL)
	   {
			colorResp->Sharpness = *(devImagingSet->Sharpness);	
	   }
	   else
	   {
		   colorResp->Sharpness = 0;
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

int COnvifDevCtl::SetImagingParameter(char *videoSourceToken,bool bModifyPersistence,struct OnvifDevColor *colorSet,unsigned int aDigest)
{
	int result = -1;
	enum EResultType eResult= OTHER_FAULT;
	struct soap *pSoap=NULL;
    struct SOAP_ENV__Header *pHeader =NULL;

	struct _timg__SetImagingSettings settings; 
	struct _timg__SetImagingSettingsResponse response;
	xsd__boolean_  tmpbool = _true;
#if 0
	struct tt__BacklightCompensation20 BacklightCompensation;
	struct tt__Exposure20 Exposure;
	struct tt__FocusConfiguration20 Focus;	
	struct tt__WideDynamicRange20 WideDynamicRange;
	struct tt__WhiteBalance20 WhiteBalance;
#endif
	struct tt__ImagingSettings20 devImagSettings;	

	settings.VideoSourceToken = NULL;
	settings.ImagingSettings = NULL;
	settings.ForcePersistence_x0020 = NULL;
	response.dummy = 0;

	if(m_imagingServAddr[0] == '\0')
	{
		return SERVER_ADDR_FAULT;
	}

	if(colorSet == NULL)
	{
		return PARAMETER_FAULT;
	}
    if(bModifyPersistence)
	{
		tmpbool = _true;
	}
	else
	{
		tmpbool = _false;
	}

	devImagSettings.Brightness = &(colorSet->Brightness);
	devImagSettings.ColorSaturation = &(colorSet->ColorSaturation);
	devImagSettings.Contrast = &(colorSet->Contrast);

	devImagSettings.IrCutFilter = NULL;
	devImagSettings.Sharpness = &(colorSet->Sharpness);
	devImagSettings.Extension = NULL;
	devImagSettings.__anyAttribute = NULL;

	devImagSettings.BacklightCompensation = NULL;
	devImagSettings.Exposure = NULL;
	devImagSettings.Focus = NULL;
	devImagSettings.WideDynamicRange = NULL;
	devImagSettings.WhiteBalance = NULL;

	settings.VideoSourceToken = videoSourceToken;
	settings.ImagingSettings = &devImagSettings;
	settings.ForcePersistence_x0020 = NULL;//&tmpbool;

	pSoap = onvif_init_soap(TDEVICE_IMAGING,false);
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


	result =soap_call___timg__SetImagingSettings(pSoap, m_imagingServAddr, NULL,&settings,&response );
	
	if(result == SOAP_OK)
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

int COnvifDevCtl::GetImagingParameterRange(char *videoSourceToken,struct OnvifDevColorRange *colorRangeResp,unsigned int aDigest)
{
	int result = -1;
	enum EResultType eResult= OTHER_FAULT;
	struct soap *pSoap=NULL;
    struct SOAP_ENV__Header *pHeader =NULL;

	int i = 0;
	int size = 0;
	struct _timg__GetOptions options;
	struct _timg__GetOptionsResponse response;

	options.VideoSourceToken = NULL;
	response.ImagingOptions = NULL;
	if(m_imagingServAddr[0] == '\0')
	{
		return -1;
	}
	if(videoSourceToken == NULL || colorRangeResp == NULL)
	{
		return -1;
	}
   
	options.VideoSourceToken = videoSourceToken;

	pSoap = onvif_init_soap(TDEVICE_IMAGING,false);
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


	result = soap_call___timg__GetOptions(pSoap, m_imagingServAddr, NULL,&options,&response);
	if(result == SOAP_OK && response.ImagingOptions )
	{
		SetAppFloatRange(&(colorRangeResp->Brightness),response.ImagingOptions->Brightness);		
		SetAppFloatRange(&(colorRangeResp->ColorSaturation),response.ImagingOptions->ColorSaturation);		
		SetAppFloatRange(&(colorRangeResp->Contrast),response.ImagingOptions->Contrast);	

		SetAppFloatRange(&(colorRangeResp->Sharpness),response.ImagingOptions->Sharpness);		
       if(response.ImagingOptions->Exposure != NULL)
	   {
         if(response.ImagingOptions->Exposure->Iris != NULL)
		  {
		   colorRangeResp->bIrisUse = true;
		   colorRangeResp->Iris.max = response.ImagingOptions->Exposure->Iris->Max;
		   colorRangeResp->Iris.min = response.ImagingOptions->Exposure->Iris->Min;
		  }
		  else
		  {
			  colorRangeResp->bIrisUse = false;
			  colorRangeResp->Iris.max = 0.0;
			  colorRangeResp->Iris.min = 0.0;
		  }
	   }

       
	   if(response.ImagingOptions->Focus != NULL)
	   {
          colorRangeResp->bFocusUse = true;
		  if(response.ImagingOptions->Focus->NearLimit != NULL)
		  {
			  colorRangeResp->focusNear.max = response.ImagingOptions->Focus->NearLimit->Max;
		      colorRangeResp->focusNear.min = response.ImagingOptions->Focus->NearLimit->Min;
		  }
		  if(response.ImagingOptions->Focus->FarLimit != NULL)
		  {
		    colorRangeResp->focusFar.max = response.ImagingOptions->Focus->FarLimit->Max;
		    colorRangeResp->focusFar.min = response.ImagingOptions->Focus->FarLimit->Min; 
		  }
		  if(response.ImagingOptions->Focus->DefaultSpeed != NULL)
		  {
			  colorRangeResp->focusSpeed.max = response.ImagingOptions->Focus->DefaultSpeed->Max;
		      colorRangeResp->focusSpeed.min = response.ImagingOptions->Focus->DefaultSpeed->Min;
		  }
	   }
	   else
	   {
		   colorRangeResp->bFocusUse = false;
		   colorRangeResp->focusNear.max = 0;
		   colorRangeResp->focusNear.min = 0;
		   colorRangeResp->focusFar.max = 0;
		   colorRangeResp->focusFar.min = 0; 
		   colorRangeResp->focusSpeed.max = 0;
		   colorRangeResp->focusSpeed.min = 0;
	   }
		
		eResult= OPERATE_SUCCESS;
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

//PTZ control
void COnvifDevCtl::InitPTZProfileToken(char *ProfileToken)
{
	if(ProfileToken == NULL)
	{
		return;
	}
    strncpy( p_profileToken,ProfileToken,sizeof(p_profileToken));
}


int COnvifDevCtl::GetNode(char *NodeToken,OnvifDevPTZNode *PTZNodeResp,unsigned int aDigest)
{
	int result = -1;
	enum EResultType eResult= OTHER_FAULT;
	struct SOAP_ENV__Header *pHeader =NULL;
	struct soap *pSoap=NULL;

	struct _tptz__GetNode GetNode;
	struct _tptz__GetNodeResponse Response;
	struct tt__PTZNode *PTZNode;
    GetNode.NodeToken = NodeToken;
	//memset(&Response,'\0',sizeof( _tptz__GetNodeResponse));
    Response.PTZNode = NULL;
	if(m_PTZServAddr[0] == '\0')
	{
		return SERVER_ADDR_FAULT;
	}

	if(PTZNodeResp== NULL)
	{
		return PARAMETER_FAULT;
	}

	pSoap = onvif_init_soap(TDEVICE_PTZ,false);
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
    result = soap_call___tptz__GetNode(pSoap, m_PTZServAddr, NULL, &GetNode,&Response);
	PTZNode = Response.PTZNode;
	if(SOAP_OK== result)
	{
		if(Response.PTZNode != NULL)
	   {
            PTZNodeResp->HomeSupported = PTZNode->HomeSupported;	   
	        PTZNodeResp->MaximumNumberOfPresets = PTZNode->MaximumNumberOfPresets;
	        //PTZNodeResp->AuxiliaryCommands = PTZNode->AuxiliaryCommands;
			if(Response.PTZNode->SupportedPTZSpaces != NULL)
			{
				if(Response.PTZNode->SupportedPTZSpaces->AbsolutePanTiltPositionSpace != NULL)
				{
	                 PTZNodeResp->SupportedPTZSpaces->AbsolutePanTiltPositionSpace[0][0] = PTZNode->SupportedPTZSpaces->AbsolutePanTiltPositionSpace->XRange->Max;
	                 PTZNodeResp->SupportedPTZSpaces->AbsolutePanTiltPositionSpace[0][1] = PTZNode->SupportedPTZSpaces->AbsolutePanTiltPositionSpace->XRange->Min;						
                     PTZNodeResp->SupportedPTZSpaces->AbsolutePanTiltPositionSpace[1][0] = PTZNode->SupportedPTZSpaces->AbsolutePanTiltPositionSpace->YRange->Max;
	                 PTZNodeResp->SupportedPTZSpaces->AbsolutePanTiltPositionSpace[1][1] = PTZNode->SupportedPTZSpaces->AbsolutePanTiltPositionSpace->YRange->Min;
				}
				if(Response.PTZNode->SupportedPTZSpaces->AbsoluteZoomPositionSpace != NULL)
				{
	                PTZNodeResp->SupportedPTZSpaces->AbsoluteZoomPositionSpace[0] = PTZNode->SupportedPTZSpaces->AbsoluteZoomPositionSpace->XRange->Max;
	                PTZNodeResp->SupportedPTZSpaces->AbsoluteZoomPositionSpace[1] = PTZNode->SupportedPTZSpaces->AbsoluteZoomPositionSpace->XRange->Min;				
				}
				if(Response.PTZNode->SupportedPTZSpaces->RelativePanTiltTranslationSpace != NULL)
				{
		            PTZNodeResp->SupportedPTZSpaces->RelativePanTiltTranslationSpace[0][0] = PTZNode->SupportedPTZSpaces->RelativePanTiltTranslationSpace->XRange->Max;
	                PTZNodeResp->SupportedPTZSpaces->RelativePanTiltTranslationSpace[0][1] = PTZNode->SupportedPTZSpaces->RelativePanTiltTranslationSpace->XRange->Min;
                    PTZNodeResp->SupportedPTZSpaces->RelativePanTiltTranslationSpace[1][0] = PTZNode->SupportedPTZSpaces->RelativePanTiltTranslationSpace->YRange->Max;
                    PTZNodeResp->SupportedPTZSpaces->RelativePanTiltTranslationSpace[1][1] = PTZNode->SupportedPTZSpaces->RelativePanTiltTranslationSpace->YRange->Min;			
				}
				if(Response.PTZNode->SupportedPTZSpaces->RelativeZoomTranslationSpace != NULL)
				{
	                PTZNodeResp->SupportedPTZSpaces->RelativeZoomTranslationSpace[0] = PTZNode->SupportedPTZSpaces->RelativeZoomTranslationSpace->XRange->Max;
	                PTZNodeResp->SupportedPTZSpaces->RelativeZoomTranslationSpace[1] = PTZNode->SupportedPTZSpaces->RelativeZoomTranslationSpace->XRange->Min;
				}
				if(Response.PTZNode->SupportedPTZSpaces->ContinuousPanTiltVelocitySpace != NULL)
				{
                    PTZNodeResp->SupportedPTZSpaces->ContinuousPanTiltVelocitySpace[0][0] = PTZNode->SupportedPTZSpaces->ContinuousPanTiltVelocitySpace->XRange->Max;
	                PTZNodeResp->SupportedPTZSpaces->ContinuousPanTiltVelocitySpace[0][1] = PTZNode->SupportedPTZSpaces->ContinuousPanTiltVelocitySpace->XRange->Min;
	                PTZNodeResp->SupportedPTZSpaces->ContinuousPanTiltVelocitySpace[1][0] = PTZNode->SupportedPTZSpaces->ContinuousPanTiltVelocitySpace->YRange->Max;
	                PTZNodeResp->SupportedPTZSpaces->ContinuousPanTiltVelocitySpace[1][1] = PTZNode->SupportedPTZSpaces->ContinuousPanTiltVelocitySpace->YRange->Min;				
				}
				if(Response.PTZNode->SupportedPTZSpaces->ContinuousZoomVelocitySpace != NULL)
				{
		            PTZNodeResp->SupportedPTZSpaces->ContinuousZoomVelocitySpace[0] = PTZNode->SupportedPTZSpaces->ContinuousZoomVelocitySpace->XRange->Max;
	                PTZNodeResp->SupportedPTZSpaces->ContinuousZoomVelocitySpace[1] = PTZNode->SupportedPTZSpaces->ContinuousZoomVelocitySpace->XRange->Min;			
				}
				if(Response.PTZNode->SupportedPTZSpaces->PanTiltSpeedSpace != NULL)
				{
					PTZNodeResp->SupportedPTZSpaces->PanTiltSpeedSpace[0] = PTZNode->SupportedPTZSpaces->PanTiltSpeedSpace->XRange->Max;
	                PTZNodeResp->SupportedPTZSpaces->PanTiltSpeedSpace[1] = PTZNode->SupportedPTZSpaces->PanTiltSpeedSpace->XRange->Min;
				}
				if(Response.PTZNode->SupportedPTZSpaces->ZoomSpeedSpace)
				{
		            PTZNodeResp->SupportedPTZSpaces->ZoomSpeedSpace[0] = PTZNode->SupportedPTZSpaces->ZoomSpeedSpace->XRange->Max;
	                PTZNodeResp->SupportedPTZSpaces->ZoomSpeedSpace[1] = PTZNode->SupportedPTZSpaces->ZoomSpeedSpace->XRange->Min;			
				}
			}
	   }

       eResult = OPERATE_SUCCESS;
	} 
	else
	{
		eResult = OPERATE_FAILED;
	}
	onvif_free_soap_header(pHeader);
	onvif_free_soap(pSoap);
	pSoap = NULL;
	return eResult;
}

int COnvifDevCtl::ContinuousMove(char *timeout,DevPosition *position,unsigned int aDigest)
{
	int result = -1;
	enum EResultType eResult= OTHER_FAULT;
	struct SOAP_ENV__Header *pHeader =NULL;
	struct soap *pSoap=NULL;

	struct _tptz__ContinuousMove *ContinuousMove = NULL ;
	struct _tptz__ContinuousMoveResponse ContinuousMoveResponse;
	ContinuousMove = new _tptz__ContinuousMove;
	ContinuousMove->Velocity = new tt__PTZSpeed;
	ContinuousMove->Velocity->PanTilt = new tt__Vector2D;
	ContinuousMove->Velocity->Zoom = new tt__Vector1D;
	//ContinuousMove->Velocity->PanTilt->space = new char[128];
	//ContinuousMove->Velocity->Zoom->space = new char[128];

	do 
	{
		if(ContinuousMove == NULL)
		{
			result = OPERATE_FAILED;
			break;
		}
		if(position == NULL || timeout == NULL)
		{
			result = OPERATE_FAILED;
			break;
		}

		ContinuousMoveResponse.dummy = 0;
		ContinuousMove->ProfileToken = p_profileToken;
		ContinuousMove->Timeout = timeout;
		ContinuousMove->Velocity->PanTilt->space = position->space[2];
		ContinuousMove->Velocity->Zoom->space = position->space[3];
		ContinuousMove->Velocity->PanTilt->x = position->position_speed[0];
		ContinuousMove->Velocity->PanTilt->y = position->position_speed[1];
		ContinuousMove->Velocity->Zoom->x = position->position_speed[2];

		if(m_PTZServAddr[0] == '\0')
		{
			result = SERVER_ADDR_FAULT;
			break;
		}

		pSoap = onvif_init_soap(TDEVICE_PTZ,false);
		if(pSoap == NULL)
		{
			result =  SOAP_INIT_FAULT;
			break;
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
			result =  SOAP_INIT_FAULT;
			break;
		}
		pSoap->header = pHeader;
		result = soap_call___tptz__ContinuousMove(pSoap, m_PTZServAddr, NULL,ContinuousMove,&ContinuousMoveResponse);
		if(SOAP_OK == result)
		{
			eResult = OPERATE_SUCCESS;
		}
		else
		{
			eResult = OPERATE_FAILED;
		}
	} while (0);

	if( SOAP_OK != result )
	{
		eResult = OPERATE_FAILED;
	}
	
	//delete [] ContinuousMove->Velocity->Zoom->space;
	//delete [] ContinuousMove->Velocity->PanTilt->space;
	delete ContinuousMove->Velocity->Zoom;
	delete ContinuousMove->Velocity->PanTilt;
	delete ContinuousMove->Velocity;
    delete ContinuousMove;

	if( pHeader)
	{
		onvif_free_soap_header(pHeader);
	}
	pHeader = NULL;

	if( pSoap )
	{
		onvif_free_soap(pSoap);
	}
	pSoap = NULL;

	return eResult;
}

void COnvifDevCtl::SetPTZSpeed(float speed_in,float speed_max,float PtzSpeed_max,float *speed_out)
 {
    float m = 0;
	m= PtzSpeed_max/speed_max;
	*speed_out = m*speed_in;

 }

int COnvifDevCtl::PTZMove(PTZMoveDirection MoveDirection,char *timeout,char *Panaddr,char *Zoomaddr,float PTZSpeed)
{
     DevPosition position;
	 char *Timeout = NULL; 
	 if(timeout == NULL || Panaddr == NULL || Zoomaddr == NULL)
	 {
	     return OPERATE_FAILED;
	 }
     switch(MoveDirection)
     {
       case Right:
          position.position_speed[0] = PTZSpeed;
          position.position_speed[1] = 0;
	      position.position_speed[2] = 0;
	      break;
	   case Left:
          position.position_speed[0] = -PTZSpeed;
          position.position_speed[1] = 0;
	      position.position_speed[2] = 0;
	      break;
	   case Up:
          position.position_speed[0] = 0;
          position.position_speed[1] = PTZSpeed;
	      position.position_speed[2] = 0;
	      break;
	   case Down:
          position.position_speed[0] = 0;
          position.position_speed[1] = -PTZSpeed;
	      position.position_speed[2] = 0;
	      break;
	   case Right_Up:
          position.position_speed[0] = PTZSpeed;
          position.position_speed[1] = PTZSpeed;
	      position.position_speed[2] = 0;
	      break;
	   case Right_Down:
          position.position_speed[0] = PTZSpeed;
          position.position_speed[1] = -PTZSpeed;
	      position.position_speed[2] = 0;
	      break;
	   case Left_Up:
          position.position_speed[0] = -PTZSpeed;
          position.position_speed[1] = PTZSpeed;
	      position.position_speed[2] = 0;
	      break;
	   case Left_Down:
          position.position_speed[0] = -PTZSpeed;
          position.position_speed[1] = -PTZSpeed;
	      position.position_speed[2] = 0;
	      break;
	   default:
		   break;

     }
	 strncpy(position.space[2],Panaddr,sizeof(position.space[2]));
	 strncpy(position.space[3],Zoomaddr,sizeof(position.space[3]));
	 Timeout = timeout;
	 int result = ContinuousMove(Timeout,&position,1);
	 return result;

}

int COnvifDevCtl::PTZZoom(DevPtzChange PtzChange,char *timeout,char *Panaddr,char *Zoomaddr,float PTZSpeed)
{
     DevPosition position;
	 char *Timeout = NULL; 
	 if(timeout == NULL || Panaddr == NULL || Zoomaddr == NULL)
	 {
	     return OPERATE_FAILED;
	 }
	 switch(PtzChange)
	 {
		 case _add:position.position_speed[0] = 0;
                   position.position_speed[1] = 0;
	               position.position_speed[2] = PTZSpeed;
				   break;
		 case _decrease:position.position_speed[0] = 0;
                   position.position_speed[1] = 0;
	               position.position_speed[2] = -PTZSpeed;
				   break;
         default:
		           break;
	 }
	 strncpy(position.space[2],Panaddr,sizeof(position.space[2]));
	 strncpy(position.space[3],Zoomaddr,sizeof(position.space[3]));
	 Timeout = timeout;
	 int result = ContinuousMove(Timeout,&position,1);
	 return result;
}

int COnvifDevCtl::PtzMoveStop(unsigned int aDigest)
{
	int result = -1;
	enum EResultType eResult= OTHER_FAULT;
	struct SOAP_ENV__Header *pHeader =NULL;
	struct soap *pSoap=NULL;

	struct _tptz__Stop *Stop;
	struct _tptz__StopResponse StopResponse;
    Stop = new _tptz__Stop;
	//Stop->ProfileToken = new xsd__boolean_;
	Stop->PanTilt = new xsd__boolean_;
	Stop->Zoom = new xsd__boolean_;
	Stop->ProfileToken = p_profileToken;
	*(Stop->PanTilt) = _true;
	*(Stop->Zoom) = _true;

	//memcpy(Stop->PanTilt,&p_pan,sizeof(Stop->PanTilt));
    //memcpy(Stop->Zoom,&p_zoom,sizeof(Stop->Zoom));

	StopResponse.dummy = 0;

	do 
	{
		if( &Stop == NULL)
		{
			result = OPERATE_FAILED;
			break;
		}

		if(m_PTZServAddr[0] == '\0')
		{
			result = SERVER_ADDR_FAULT;
			break;
		}
		pSoap = onvif_init_soap(TDEVICE_PTZ,false);
		if(pSoap == NULL)
		{
			result = SOAP_INIT_FAULT;
			break;
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
			result = SOAP_INIT_FAULT;
			break;
		}
		pSoap->header = pHeader;
		result = soap_call___tptz__Stop(pSoap, m_PTZServAddr, NULL,Stop,&StopResponse);
		if(SOAP_OK == result)
		{
			eResult = OPERATE_SUCCESS;
		}
		else
		{
			eResult = OPERATE_FAILED;
		}
	} while (0);

	if( SOAP_OK != result )
	{
		eResult = OPERATE_FAILED;
	}
  
	delete Stop->PanTilt;
	delete Stop->Zoom;
	delete Stop;

	if( pHeader)
	{
		onvif_free_soap_header(pHeader);
	}
	pHeader = NULL;

	if( pSoap )
	{
		onvif_free_soap(pSoap);
	}
	pSoap = NULL;

	return eResult;

}

int COnvifDevCtl::GetAllPresets(char *aProfileToken,struct PresetsStruct *PresetsResp,int aDigest)
{
	int result = -1;
	enum EResultType eResult= OTHER_FAULT;
	struct SOAP_ENV__Header *pHeader =NULL;
	struct soap *pSoap=NULL;
    int i= 0,j=0;
	struct _tptz__GetPresets GetPresets;
	struct _tptz__GetPresetsResponse GetPresetsResp;

	GetPresets.ProfileToken = aProfileToken;
   GetPresetsResp.__sizePreset = 0;
   GetPresetsResp.Preset =NULL;
   
	if(m_PTZServAddr[0] == '\0' || PresetsResp == NULL)
	{
		return SERVER_ADDR_FAULT;
	}
	pSoap = onvif_init_soap(TDEVICE_PTZ,false);
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

	result = soap_call___tptz__GetPresets(pSoap,m_PTZServAddr,NULL,&GetPresets,&GetPresetsResp);

	if(SOAP_OK == result)
	{
		for(i=0;i<GetPresetsResp.__sizePreset;i++)
		{
			if(GetPresetsResp.Preset != NULL)
			{
				if(GetPresetsResp.Preset[i].token != NULL)
				{
					j = atoi(GetPresetsResp.Preset[i].token);//在token中找，预置点号
					char * pTemp = NULL;
					if (0 == j)
					{
						unsigned int tempLenToken = strlen(GetPresetsResp.Preset[i].token);
						for (--tempLenToken;tempLenToken>0;--tempLenToken)
						{
							//pTemp = reinterpret_cast<char *>(GetPresetsResp.Preset[i].token+tempLenToken);
							j = atoi( GetPresetsResp.Preset[i].token+tempLenToken );
							if (0 == j)
							{
								break;
							}
						}
						j = atoi( GetPresetsResp.Preset[i].token+(++tempLenToken) );//以上均为在token中找，预置点号

						if (0 == j)//如果以上步骤，在token中没有找到，则以下在name中找
						{	
							j = atoi(GetPresetsResp.Preset[i].Name);
							if (0 == j)
							{
								unsigned int tempLenName = strlen(GetPresetsResp.Preset[i].token);
								for (--tempLenName;tempLenName>0;--tempLenName)
								{
									j = atoi( GetPresetsResp.Preset[i].token+tempLenName );
									if (0 == j)
									{
										break;
									}
								}
								j = atoi( GetPresetsResp.Preset[i].token+(++tempLenName) );
							}
						}//在name中找，到处结束
					}
					strcpy(PresetsResp->presetToken[j],GetPresetsResp.Preset[i].token);
				}
			}
		}
		eResult = OPERATE_SUCCESS;
	}
	else
	{
		eResult = OPERATE_FAILED;
	}

	onvif_free_soap_header(pHeader);
	onvif_free_soap(pSoap);
	pSoap = NULL;
	return eResult; 
}


int COnvifDevCtl::SetPreset(unsigned int PresetNum)
{
	int result = -1;
	int ret = -1;
	char PresetsToken[16] = {0};
	/*char Preset[8] = {0};
	char PresetName[16] = {0};*/
	int flag = 0;
	unsigned int j = 0;
	//	struct PtzPresesData PresesData;
	enum EResultType eResult= OTHER_FAULT;
	COnvifSoap soap;

	if(m_PTZServAddr[0] == '\0' || p_profileToken[0] == '\0')
	{
		return SERVER_ADDR_FAULT;
	}

	sprintf(PresetsToken,"%d",PresetNum+1);
	/*strcpy(PresetsToken,"preset");
	sprintf(Preset,"%d",PresetNum+1);
	strcat(PresetsToken,Preset);

	strcpy(PresetName,PresetsToken);*/
	struct _tptz__SetPreset SetPreset;
	struct _tptz__SetPresetResponse PresetResponse;

	SetPreset.ProfileToken = p_profileToken;
	/*SetPreset.PresetName = PresetName;*/
	SetPreset.PresetName = NULL;
	PresetResponse.PresetToken = NULL;

#if 0
	GetPresets(&PresesData);

	for(int i = 0 ;i < PresesData.SizePresets ;i++)
	{
		if(!strcmp(PresesData.PresetsToken[i],PresetsToken))
		{
			flag = 1;
			break;
		}
	}

	if(!flag)
	{
		for(j= 0;j<(PresetNum+1);j++)
		{
			// CreatePreset();
		}
	}
#endif

	SetPreset.PresetToken = PresetsToken;
	if(m_PTZServAddr[0] == '\0')
	{
		return SERVER_ADDR_FAULT;
	}
	//ret = soap.ProduceSoapInfo(TDEVICE_PTZ,1,m_userName,m_passKey);
	//if(ret != SOAP_INIT_SUCCESS)
	//{
	//   return SOAP_INIT_FAULT;
	//}

	ret = soap.ProduceSoapInfo(TDEVICE_PTZ,true,m_userName,m_passWord,&stOnvifInputParam.stDevUtcTime);
	if(ret != SOAP_INIT_SUCCESS)
	{
		return SOAP_INIT_FAULT;
	}
	result = soap_call___tptz__SetPreset(soap.m_pSoap, m_PTZServAddr, NULL,&SetPreset,&PresetResponse);
	if(result != SOAP_OK)
	{
		soap.ExitSoap();
		ret = soap.ProduceSoapInfo(TDEVICE_PTZ,false,m_userName,m_passWord);
		if(ret != SOAP_INIT_SUCCESS)
		{
			return SOAP_INIT_FAULT;
		}
		result = soap_call___tptz__SetPreset(soap.m_pSoap, m_PTZServAddr, NULL,&SetPreset,&PresetResponse);
	}
	if(SOAP_OK == result)
	{
		//strncpy(PresetsToken,PresetResponse.PresetToken,sizeof(PresetsToken));
		eResult = OPERATE_SUCCESS;
	}
	else
	{
		eResult = OPERATE_FAILED;
	}
	soap.ExitSoap();
	return eResult;
}

int COnvifDevCtl::SetPreset(char *PresetsName, char *PresetsToken,unsigned int aDigest)
{
	int result = -1;
	enum EResultType eResult= OTHER_FAULT;
	struct SOAP_ENV__Header *pHeader =NULL;
	struct soap *pSoap=NULL;

	struct _tptz__SetPreset SetPreset;
	struct _tptz__SetPresetResponse PresetResponse;
	SetPreset.ProfileToken = p_profileToken;
	SetPreset.PresetName = PresetsName;
	SetPreset.PresetToken = PresetsToken;

	if(m_PTZServAddr[0] == '\0')
	{
		return SERVER_ADDR_FAULT;
	}
	pSoap = onvif_init_soap(TDEVICE_PTZ,false);
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
	result = soap_call___tptz__SetPreset(pSoap, m_PTZServAddr, NULL,&SetPreset,&PresetResponse);
	if(SOAP_OK == result)
	{
		//strncpy(PresetsToken,PresetResponse.PresetToken,sizeof(PresetsToken));
		eResult = OPERATE_SUCCESS;
	}
	else
	{
		eResult = OPERATE_FAILED;
	}
	onvif_free_soap_header(pHeader);
	onvif_free_soap(pSoap);
	pSoap = NULL;
	return eResult;


}

int COnvifDevCtl::GotoPreset(char *PresetsToken,DevPosition *position,unsigned int aDigest)
{
	int result = -1;
	enum EResultType eResult= OTHER_FAULT;
	struct SOAP_ENV__Header *pHeader =NULL;
	struct soap *pSoap=NULL;

	struct _tptz__GotoPreset *GotoPreset;
	struct _tptz__GotoPresetResponse GotoPresetResponse;
	GotoPreset = new _tptz__GotoPreset;
	GotoPreset->Speed = new tt__PTZSpeed;
	GotoPreset->Speed->PanTilt = new tt__Vector2D;
	GotoPreset->Speed->Zoom = new tt__Vector1D;
	//GotoPreset->Speed->PanTilt->space = new char[128];
	//GotoPreset->Speed->Zoom->space = new char[128];

	do 
	{
		if(GotoPreset == NULL)
		{
			result = OPERATE_FAILED;
			break;
		}
		if(position == NULL || PresetsToken == NULL)
		{
			result = OPERATE_FAILED;
			break;
		}
		GotoPreset->ProfileToken = p_profileToken;
		GotoPreset->PresetToken = PresetsToken;
		GotoPreset->Speed->PanTilt->space = position->space[2];
		GotoPreset->Speed->Zoom->space = position->space[3];
		GotoPreset->Speed->PanTilt->x = position->position_speed[0];
		GotoPreset->Speed->PanTilt->y = position->position_speed[1];
		GotoPreset->Speed->Zoom->x = position->position_speed[2];

		if(m_PTZServAddr[0] == '\0')
		{
			result = SERVER_ADDR_FAULT;
			break;
		}
		pSoap = onvif_init_soap(TDEVICE_PTZ,false);
		if(pSoap == NULL)
		{
			result = SOAP_INIT_FAULT;
			break;
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
			result = SOAP_INIT_FAULT;
			break;
		}
		pSoap->header = pHeader;
		result = soap_call___tptz__GotoPreset(pSoap, m_PTZServAddr, NULL,GotoPreset,&GotoPresetResponse);
		if(SOAP_OK == result)
		{
			eResult = OPERATE_SUCCESS;
		}
		else
		{
			eResult = OPERATE_FAILED;
		}
	} while (0);

	if( SOAP_OK != result )
	{
		eResult = OPERATE_FAILED;
	}

//	delete [] GotoPreset->Speed->Zoom->space;
//	delete [] GotoPreset->Speed->PanTilt->space;
	delete GotoPreset->Speed->Zoom;
    delete GotoPreset->Speed->PanTilt;
	delete GotoPreset->Speed;
	delete GotoPreset;

	if( pHeader)
	{
		onvif_free_soap_header(pHeader);
	}
	pHeader = NULL;

	if( pSoap )
	{
		onvif_free_soap(pSoap);
	}
	pSoap = NULL;

	return eResult;


}

int COnvifDevCtl::RemovePreset(char *PresetsToken,unsigned int aDigest)
{
	int result = -1;
	enum EResultType eResult= OTHER_FAULT;
	struct SOAP_ENV__Header *pHeader =NULL;
	struct soap *pSoap=NULL;

	struct _tptz__RemovePreset RemovePreset;
	struct _tptz__RemovePresetResponse RemovePresetResponse;

	if(PresetsToken == NULL)
	{
	   return OPERATE_FAILED;
	}
   
	RemovePreset.ProfileToken = p_profileToken;
	RemovePreset.PresetToken = PresetsToken;
	if(m_PTZServAddr[0] == '\0')
	{
		return SERVER_ADDR_FAULT;
	}
	pSoap = onvif_init_soap(TDEVICE_PTZ,false);
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
	result = soap_call___tptz__RemovePreset(pSoap, m_PTZServAddr, NULL,&RemovePreset,&RemovePresetResponse);
	if(SOAP_OK == result)
	{
		eResult = OPERATE_SUCCESS;
	}
	else
	{
		eResult = OPERATE_FAILED;
	}
	onvif_free_soap_header(pHeader);
	onvif_free_soap(pSoap);
	pSoap = NULL;
	return eResult;

}

int COnvifDevCtl::OperatePresetTour(char *PresetTourToken,DevPTZPresetTourOperation *PresetTourOperation,unsigned int aDigest)
{
	int result = -1;
	enum EResultType eResult= OTHER_FAULT;
	struct SOAP_ENV__Header *pHeader =NULL;
	struct soap *pSoap=NULL;

	if(PresetTourToken == NULL || PresetTourOperation == NULL)
	{
	    return PARAMETER_FAULT;
	}

	if(m_PTZServAddr[0] == '\0')
	{
		return SERVER_ADDR_FAULT;
	}
    
	struct _tptz__OperatePresetTour PresetTour;
	struct _tptz__OperatePresetTourResponse PresetTourResponse;
	PresetTour.ProfileToken = p_profileToken;
	PresetTour.PresetTourToken = PresetTourToken;
	memcpy(&(PresetTour.Operation),PresetTourOperation,sizeof(&(PresetTour.Operation)));
	PresetTourResponse.dummy = 0;

	pSoap = onvif_init_soap(TDEVICE_PTZ,false);
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
	result = soap_call___tptz__OperatePresetTour(pSoap, m_PTZServAddr, NULL,&PresetTour,&PresetTourResponse);
	if(SOAP_OK == result)
	{
		eResult = OPERATE_SUCCESS;
	}
	else
	{
		eResult = OPERATE_FAILED;
	}
	onvif_free_soap_header(pHeader);
	onvif_free_soap(pSoap);
	pSoap = NULL;
	return eResult;


}

int COnvifDevCtl::RemovePresetTour(char *PresetTourToken,unsigned int aDigest)
{
	int result = -1;
	enum EResultType eResult= OTHER_FAULT;
	struct SOAP_ENV__Header *pHeader =NULL;
	struct soap *pSoap=NULL;

	if(PresetTourToken == NULL)
	{
	    return PARAMETER_FAULT;
	}

	if(m_PTZServAddr[0] == '\0')
	{
		return SERVER_ADDR_FAULT;
	}

	struct _tptz__RemovePresetTour RemovePresetTour;
    struct _tptz__RemovePresetTourResponse RemovePresetTourResponse;
	RemovePresetTour.ProfileToken = p_profileToken;
	RemovePresetTour.PresetTourToken = PresetTourToken;
	RemovePresetTourResponse.dummy = 0;

	pSoap = onvif_init_soap(TDEVICE_PTZ,false);
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
	result = soap_call___tptz__RemovePresetTour(pSoap, m_PTZServAddr, NULL,&RemovePresetTour,&RemovePresetTourResponse);
	if(SOAP_OK == result)
	{
		eResult = OPERATE_SUCCESS;
	}
	else
	{
		eResult = OPERATE_FAILED;
	}
	onvif_free_soap_header(pHeader);
	onvif_free_soap(pSoap);
	pSoap = NULL;
	return eResult;
}


int COnvifDevCtl::GetImagingIrisRange(char *videoSourceToken,ImgGetOptions *imgGetOptions,unsigned int aDigest)
{
	int result = -1;
	enum EResultType eResult= OTHER_FAULT;
	struct SOAP_ENV__Header *pHeader =NULL;
	struct soap *pSoap=NULL;

	struct _timg__GetOptions TimgGetOptions;
	struct _timg__GetOptionsResponse GetOptionsResponse;
	TimgGetOptions.VideoSourceToken = videoSourceToken;
    
	if(m_imagingServAddr[0] == '\0')
	{
		return SERVER_ADDR_FAULT;
	}
	if(videoSourceToken == NULL || imgGetOptions == NULL)
	{
	     return PARAMETER_FAULT;
	}
	pSoap = onvif_init_soap(TDEVICE_IMAGING,false);
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
	result = soap_call___timg__GetOptions(pSoap, m_imagingServAddr, NULL,&TimgGetOptions,&GetOptionsResponse);
	if(SOAP_OK == result)
	{
		if(&GetOptionsResponse != NULL)
		{
			if(GetOptionsResponse.ImagingOptions->Exposure != NULL)
			{

   			     if(GetOptionsResponse.ImagingOptions->Exposure->Iris != NULL)
			     {
				    imgGetOptions->IrisMax = GetOptionsResponse.ImagingOptions->Exposure->Iris->Max;
				    imgGetOptions->IrisMin = GetOptionsResponse.ImagingOptions->Exposure->Iris->Min;
					//m_Iris = GetOptionsResponse.ImagingOptions->Exposure->Iris->Max;
			     }
			}

		}
		eResult = OPERATE_SUCCESS;
	}
	else
	{
		eResult = OPERATE_FAILED;
	}
	onvif_free_soap_header(pHeader);
	onvif_free_soap(pSoap);
	pSoap = NULL;
	return eResult;
}

int COnvifDevCtl::GetImagingIris(char *videoSourceToken,float *Iris,unsigned int aDigest)
{
	int result = -1;
	enum EResultType eResult= OTHER_FAULT;
	struct SOAP_ENV__Header *pHeader =NULL;
	struct soap *pSoap=NULL;

	struct _timg__GetImagingSettings GetImagingSettings;
	struct _timg__GetImagingSettingsResponse GetImagingSettingsResponse;
	GetImagingSettings.VideoSourceToken = videoSourceToken;

	if(m_imagingServAddr[0] == '\0')
	{
		return SERVER_ADDR_FAULT;
	}
	pSoap = onvif_init_soap(TDEVICE_IMAGING,false);
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
	result = soap_call___timg__GetImagingSettings(pSoap,m_imagingServAddr, NULL,&GetImagingSettings,&GetImagingSettingsResponse);
	if(SOAP_OK == result)
	{

		if(&GetImagingSettingsResponse != NULL)
		{
			if(GetImagingSettingsResponse.ImagingSettings->Exposure != NULL)
			{
				if(GetImagingSettingsResponse.ImagingSettings->Exposure->Iris != NULL)
			    {
				    *Iris = *GetImagingSettingsResponse.ImagingSettings->Exposure->Iris;
		        }
			}
		}
		eResult = OPERATE_SUCCESS;

	}
	else
	{
		eResult = OPERATE_FAILED;
	}
	onvif_free_soap_header(pHeader);
	onvif_free_soap(pSoap);
	pSoap = NULL;
	return eResult;
}

int COnvifDevCtl::SetImagingSettings(char *videoSourceToken,DevImgSetDate *SetDate,unsigned int aDigest)
{
	int result = -1;
	enum EResultType eResult= OTHER_FAULT;
	struct SOAP_ENV__Header *pHeader =NULL;
	struct soap *pSoap=NULL;
	
	if( SetDate == NULL )
	{
		return PARAMETER_FAULT;
	}

	struct _timg__SetImagingSettings *SetImagingSettings = NULL;
	struct _timg__SetImagingSettingsResponse SetImagingSettingsResponse;
    SetImagingSettings = new _timg__SetImagingSettings;
	SetImagingSettings->ForcePersistence_x0020 = new  xsd__boolean_;
    SetImagingSettings->ImagingSettings = new tt__ImagingSettings20;
	SetImagingSettings->ImagingSettings->Exposure = new tt__Exposure20;
	SetImagingSettings->ImagingSettings->Focus = new tt__FocusConfiguration20;

	do 
	{
		if(SetDate->boolean != NULL)
		{
			memcpy( SetImagingSettings->ForcePersistence_x0020,&(SetDate->boolean),sizeof( SetImagingSettings->ForcePersistence_x0020));
		}
		else
		{
			SetImagingSettings->ForcePersistence_x0020 = NULL;
		}
		if(SetDate->FocusMode != NULL )
		{
			memcpy(&(SetImagingSettings->ImagingSettings->Focus->AutoFocusMode),SetDate->FocusMode,sizeof(&(SetImagingSettings->ImagingSettings->Focus->AutoFocusMode)));
			SetImagingSettings->ImagingSettings->Focus->__anyAttribute = NULL;
			SetImagingSettings->ImagingSettings->Focus->DefaultSpeed = NULL;
			SetImagingSettings->ImagingSettings->Focus->Extension = NULL;
			SetImagingSettings->ImagingSettings->Focus->FarLimit = NULL;
			SetImagingSettings->ImagingSettings->Focus->NearLimit = NULL;

		}
		else
		{
			SetImagingSettings->ImagingSettings->Focus = NULL;
		}
		if(SetDate->IrisDate != NULL)

		{
			memcpy(&(SetImagingSettings->ImagingSettings->Exposure->Mode),&(SetDate->IrisDate->Mode),sizeof(&(SetImagingSettings->ImagingSettings->Exposure->Mode)));

		}

		SetImagingSettings->ImagingSettings->Exposure->Iris = &(SetDate->IrisDate->iris);
		SetImagingSettings->VideoSourceToken = videoSourceToken;
		SetImagingSettings->ImagingSettings->Exposure->ExposureTime = NULL;
		SetImagingSettings->ImagingSettings->Exposure->Gain = NULL;
		SetImagingSettings->ImagingSettings->Exposure->MaxExposureTime = NULL;
		SetImagingSettings->ImagingSettings->Exposure->MaxGain = NULL;
		SetImagingSettings->ImagingSettings->Exposure->MaxIris = NULL;
		SetImagingSettings->ImagingSettings->Exposure->MinExposureTime = NULL;
		SetImagingSettings->ImagingSettings->Exposure->MinGain = NULL;
		SetImagingSettings->ImagingSettings->Exposure->MinIris =NULL;
		SetImagingSettings->ImagingSettings->Exposure->Priority = NULL;
		SetImagingSettings->ImagingSettings->Exposure->Window = NULL;
		SetImagingSettings->ImagingSettings->__anyAttribute = NULL;
		SetImagingSettings->ImagingSettings->BacklightCompensation = NULL;
		SetImagingSettings->ImagingSettings->Brightness = NULL;
		SetImagingSettings->ImagingSettings->ColorSaturation = NULL;
		SetImagingSettings->ImagingSettings->Contrast = NULL;
		SetImagingSettings->ImagingSettings->Extension = NULL;
		SetImagingSettings->ImagingSettings->Sharpness = NULL;
		SetImagingSettings->ImagingSettings->WhiteBalance = NULL;
		SetImagingSettings->ImagingSettings->WideDynamicRange = NULL;
		SetImagingSettings->ImagingSettings->IrCutFilter = NULL;

		if(m_imagingServAddr[0] == '\0')
		{
			result = SERVER_ADDR_FAULT;
			break;
		}
		pSoap = onvif_init_soap(TDEVICE_IMAGING,false);
		if(pSoap == NULL)
		{
			result = SOAP_INIT_FAULT;
			break;
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
			result = SOAP_INIT_FAULT;
			break;
		}
		pSoap->header = pHeader;

		result = soap_call___timg__SetImagingSettings(pSoap, m_imagingServAddr, NULL,SetImagingSettings,&SetImagingSettingsResponse);
		if(SOAP_OK == result)
		{
			eResult = OPERATE_SUCCESS;
		}
		else
		{
			eResult = OPERATE_FAILED;
		}
	} while (0);

	if( SOAP_OK != result )
	{
		eResult = OPERATE_FAILED;
	}

	delete SetImagingSettings->ImagingSettings->Focus;
	delete SetImagingSettings->ImagingSettings->Exposure;
	delete SetImagingSettings->ImagingSettings;
	delete SetImagingSettings->ForcePersistence_x0020;
	delete SetImagingSettings;

	if( pHeader)
	{
		onvif_free_soap_header(pHeader);
	}
	pHeader = NULL;

	if( pSoap )
	{
		onvif_free_soap(pSoap);
	}
	pSoap = NULL;

	return eResult;

}

int COnvifDevCtl::PTZFocusMove(char *videoSourceToken,DevPtzChange PtzChange,PTZMoveOptions *MoveOptions,float FocusSpeed,unsigned int aDigest)
{
	int result = -1;
	enum EResultType eResult= OTHER_FAULT;
	struct SOAP_ENV__Header *pHeader =NULL;
	struct soap *pSoap=NULL;

	struct _timg__Move *timgMove = NULL;
	struct _timg__MoveResponse MoveResponse;
	timgMove = new _timg__Move;
	timgMove->Focus = new tt__FocusMove;
	timgMove->Focus->Continuous = new tt__ContinuousFocus;
	timgMove->Focus->Absolute = new tt__AbsoluteFocus;
	timgMove->Focus->Relative = new tt__RelativeFocus;
	timgMove->VideoSourceToken = videoSourceToken;

	do 
	{
		if(videoSourceToken == NULL || MoveOptions == NULL )
		{
			result = PARAMETER_FAULT;
			break;
		}
		if(MoveOptions->Continuous != NULL)
		{
			if(FocusSpeed > MoveOptions->Continuous[0] || FocusSpeed < MoveOptions->Continuous[1])
			{
				result = PARAMETER_FAULT;
				break;
			}
			else
			{
	     		switch (PtzChange)
				{
					 case _add:
								timgMove->Focus->Continuous->Speed = FocusSpeed;
								break;
					case _decrease:
								timgMove->Focus->Continuous->Speed = -FocusSpeed;
								 break;
						default:
								   break;	
				}	
				  //timgMove->Focus->Continuous->Speed = FocusSpeed;
			}
		}

			timgMove->Focus->Absolute = NULL;
			timgMove->Focus->Relative = NULL;

		if(m_imagingServAddr[0] == '\0')
		{
			result = SERVER_ADDR_FAULT;
			break;
		}
		pSoap = onvif_init_soap(TDEVICE_IMAGING,false);
		if(pSoap == NULL)
		{
			result = SOAP_INIT_FAULT;
			break;
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
			result = SOAP_INIT_FAULT;
			break;
		}
		pSoap->header = pHeader;
		result = soap_call___timg__Move(pSoap, m_imagingServAddr, NULL,timgMove,&MoveResponse);
		if(SOAP_OK == result)
		{
			eResult = OPERATE_SUCCESS;
		}
		else
		{
			eResult = OPERATE_FAILED;
		}
	} while (0);

	if( SOAP_OK != result )
	{
		eResult = OPERATE_FAILED;
	}

	delete timgMove->Focus->Relative;
	delete timgMove->Focus->Absolute;
	delete timgMove->Focus->Continuous;
	delete timgMove->Focus;
	delete timgMove;

	if( pHeader)
	{
		onvif_free_soap_header(pHeader);
	}
	pHeader = NULL;

	if( pSoap )
	{
		onvif_free_soap(pSoap);
	}
	pSoap = NULL;

	return eResult;
}

int COnvifDevCtl::GetPTZFocusOptions(char *videoSourceToken,PTZMoveOptions *MoveOptions,unsigned int aDigest)
{
	int result = -1;
	enum EResultType eResult= OTHER_FAULT;
	struct SOAP_ENV__Header *pHeader =NULL;
	struct soap *pSoap=NULL;

	struct _timg__GetMoveOptions GetMoveOptions;
	struct _timg__GetMoveOptionsResponse GetMoveOptionsResponse;
	GetMoveOptions.VideoSourceToken = videoSourceToken;

	if(m_imagingServAddr[0] == '\0')
	{
		return SERVER_ADDR_FAULT;
	}
	pSoap = onvif_init_soap(TDEVICE_IMAGING,false);
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
	result = soap_call___timg__GetMoveOptions(pSoap, m_imagingServAddr, NULL,&GetMoveOptions,&GetMoveOptionsResponse);
	if(SOAP_OK == result)
	{
		if(GetMoveOptionsResponse.MoveOptions != NULL)
		{
			if(GetMoveOptionsResponse.MoveOptions->Absolute  != NULL)
			{  
				if(GetMoveOptionsResponse.MoveOptions->Absolute->Position != NULL)
				{
				    MoveOptions->Absolute[0] = GetMoveOptionsResponse.MoveOptions->Absolute->Position->Max;
				    MoveOptions->Absolute[1] = GetMoveOptionsResponse.MoveOptions->Absolute->Position->Min;		
				}

				if(GetMoveOptionsResponse.MoveOptions->Absolute->Speed != NULL)
				{
				    MoveOptions->Absolute[2] = GetMoveOptionsResponse.MoveOptions->Absolute->Speed->Max;
					MoveOptions->Absolute[3] = GetMoveOptionsResponse.MoveOptions->Absolute->Speed->Min;
				}
				
				
			}
			if(GetMoveOptionsResponse.MoveOptions->Relative != NULL)
			{
				if(GetMoveOptionsResponse.MoveOptions->Relative->Distance != NULL)
				{
				   MoveOptions->Relative[0] = GetMoveOptionsResponse.MoveOptions->Relative->Distance->Max;
				   MoveOptions->Relative[1] = GetMoveOptionsResponse.MoveOptions->Relative->Distance->Min;
				}
				if(GetMoveOptionsResponse.MoveOptions->Relative->Speed != NULL)
				{
				   MoveOptions->Relative[2] = GetMoveOptionsResponse.MoveOptions->Relative->Speed->Max;
                   MoveOptions->Relative[3] = GetMoveOptionsResponse.MoveOptions->Relative->Speed->Min;
				}

			}
			if(GetMoveOptionsResponse.MoveOptions->Continuous != NULL)
			{
				MoveOptions->Continuous[0] = GetMoveOptionsResponse.MoveOptions->Continuous->Speed->Max;
				MoveOptions->Continuous[1] = GetMoveOptionsResponse.MoveOptions->Continuous->Speed->Min;
			}
		
		}

		eResult = OPERATE_SUCCESS;
	}
	else
	{
		eResult = OPERATE_FAILED;
	}
	onvif_free_soap_header(pHeader);
	onvif_free_soap(pSoap);
	pSoap = NULL;
	return eResult;
}

int COnvifDevCtl::CreatePresetTour(char *profileToken,unsigned int aDigest)
{
	int result = -1;
	enum EResultType eResult= OTHER_FAULT;
	struct SOAP_ENV__Header *pHeader =NULL;
	struct soap *pSoap=NULL;

	struct _tptz__CreatePresetTour CreatePresetTour;
	struct _tptz__CreatePresetTourResponse CreatePresetTourResponse;

	CreatePresetTour.ProfileToken = profileToken;
	CreatePresetTourResponse.PresetTourToken = NULL;
	if(m_PTZServAddr[0] == '\0')
	{
		return SERVER_ADDR_FAULT;
	}
	pSoap = onvif_init_soap(TDEVICE_PTZ,false);
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
	result = soap_call___tptz__CreatePresetTour(pSoap, m_PTZServAddr, NULL,&CreatePresetTour,&CreatePresetTourResponse);
	if(SOAP_OK == result)
	{
		eResult = OPERATE_SUCCESS;
	}
	else
	{
		eResult = OPERATE_FAILED;
	}
	onvif_free_soap_header(pHeader);
	onvif_free_soap(pSoap);
	pSoap = NULL;
	return eResult;
}

int COnvifDevCtl::GetPresetTours(char *profileToken,struct PTZPresetTours *PresetTours,unsigned int aDigest)
{
	int result = -1;
	enum EResultType eResult= OTHER_FAULT;
	struct SOAP_ENV__Header *pHeader =NULL;
	struct soap *pSoap=NULL;

	struct _tptz__GetPresetTours GetPresetTours;
	struct _tptz__GetPresetToursResponse GetPresetToursResponse;

	if(profileToken == NULL)
	{
		return PARAMETER_FAULT;
	}

	GetPresetTours.ProfileToken = profileToken;
	GetPresetToursResponse.__sizePresetTour = 0;
	GetPresetToursResponse.PresetTour = NULL;

	pSoap = onvif_init_soap(TDEVICE_PTZ,false);
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
	result = soap_call___tptz__GetPresetTours(pSoap, m_PTZServAddr, NULL,&GetPresetTours,&GetPresetToursResponse);
	if(SOAP_OK == result)
	{
		if(&GetPresetToursResponse != NULL)
		{
			PresetTours->SizePresetTour =  GetPresetToursResponse.__sizePresetTour;
			if(GetPresetToursResponse.PresetTour != NULL)
			{
				for(int i = 0;i<PresetTours->SizePresetTour;i++)
				{
					if(GetPresetToursResponse.PresetTour->token != NULL)
					{
						strcpy(PresetTours->PresetTourToken[i],GetPresetToursResponse.PresetTour->token);
					}
					GetPresetToursResponse.PresetTour++;
				}
			}
		}
		eResult = OPERATE_SUCCESS;
	}
	else
	{
		eResult = OPERATE_FAILED;
	}
	onvif_free_soap_header(pHeader);
	onvif_free_soap(pSoap);
	pSoap = NULL;
	return eResult;
}

//
void COnvifDevCtl::GetAppRelayOutputSettings(struct OnvifRelayOutputSet *appParam,struct tt__RelayOutputSettings *deviceParam)
{
	if(appParam == NULL || deviceParam == NULL)
	{
		return;
	}
	if(deviceParam->Mode == tt__RelayMode__Monostable )
	{
		appParam->Mode = T_RelayModeMonostable;
	}
	if(deviceParam->Mode == tt__RelayMode__Bistable)
	{
		appParam->Mode = T_RelayModeBistable;
	}
	strcpy(appParam->DelayTime, deviceParam->DelayTime);
	if(deviceParam->IdleState == tt__RelayIdleState__closed)
	{
		appParam->IdleState = T_RelayIdleStateClosed;
	}
	if( deviceParam->IdleState == tt__RelayIdleState__open)
	{
		appParam->IdleState = T_RelayIdleStateOpen;
	}
}

void COnvifDevCtl::SetDevRelayOutputSettings(struct tt__RelayOutputSettings *deviceParam,struct OnvifRelayOutputSet *appParam)
{
	if(deviceParam == NULL || appParam == NULL)
	{
		return;
	}

	if( appParam->Mode == T_RelayModeMonostable)
	{	
		deviceParam->Mode = tt__RelayMode__Monostable;
	}
	if(appParam->Mode == T_RelayModeBistable )
	{
		deviceParam->Mode = tt__RelayMode__Bistable;
	}
	deviceParam->DelayTime=appParam->DelayTime;
	if(appParam->IdleState == T_RelayIdleStateClosed )
	{
		deviceParam->IdleState = tt__RelayIdleState__closed;
	}
	if( appParam->IdleState == T_RelayIdleStateOpen )
	{
		deviceParam->IdleState = tt__RelayIdleState__open;
	}
}

void COnvifDevCtl::SetAppFloatRange(struct OnvifFloatRange *appFloatRange,struct tt__FloatRange *devFloatRange)
{
	if(appFloatRange == NULL)
	{
		return;   
	}
	if(devFloatRange == NULL)
	{
		appFloatRange->min  =0.0;
		appFloatRange->max = 0.0;
		return;
	}
	appFloatRange->min  =devFloatRange->Min;
	appFloatRange->max = devFloatRange->Max;
}

int COnvifDevCtl::ModifyPresetInTour(struct OnvifModifyPresetTour *PresetTour, unsigned int aDigest)
{
//3?ê??ˉ
	int result = -1;
	int ret = -1;
	int size = 0;
	int i = 0;
	int flag = -1;
	enum EResultType eResult= OTHER_FAULT;
	//COnvifSoap soap;
	struct SOAP_ENV__Header *pHeader =NULL;
    struct soap *pSoap=NULL;
	struct _tptz__ModifyPresetTour ModifyPresetTour;
	struct _tptz__ModifyPresetTourResponse ModifyPresetTourResponse;

	struct tt__PTZPresetTourStatus Status;
	struct tt__PTZPresetTourStartingCondition StartingCondition;

	ModifyPresetTour.ProfileToken = NULL;
	ModifyPresetTour.PresetTour = NULL;
	ModifyPresetTourResponse.dummy = 0;

	if(m_PTZServAddr[0] == '\0')
	{
		return SERVER_ADDR_FAULT;
	}

	if(PresetTour == NULL)
	{
		return PARAMETER_FAULT;
	}

	pSoap = onvif_init_soap(TDEVICE_PTZ,false);
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

	struct tt__PresetTour *PtzPresetTour = NULL;
	PtzPresetTour = new tt__PresetTour;
	if(PtzPresetTour == NULL)
	{
		return OPERATE_FAILED;
	}
//ì?3?2?êy
	size = PresetTour->SizePreset;

	PtzPresetTour->__anyAttribute = NULL;
	PtzPresetTour->__sizeTourSpot = PresetTour->SizePreset;
	PtzPresetTour->AutoStart = _true;
	PtzPresetTour->Extension = NULL;
	PtzPresetTour->Name = NULL;
	PtzPresetTour->StartingCondition = NULL;
	PtzPresetTour->Status = NULL;
	PtzPresetTour->token = PresetTour->TourToken;

	struct tt__PTZPresetTourSpot *PTZPresetTourSpot = NULL;
	PTZPresetTourSpot = new tt__PTZPresetTourSpot[size];
	if(PTZPresetTourSpot == NULL)
	{
		delete PtzPresetTour;
		PtzPresetTour = NULL;
		return OPERATE_FAILED;
	}

	struct tt__PTZPresetTourPresetDetail *PresetDetail = NULL;
	PresetDetail = new tt__PTZPresetTourPresetDetail[size];
	if(PresetDetail == NULL)
	{
		delete PtzPresetTour;
		PtzPresetTour = NULL;

		delete [] PTZPresetTourSpot;
		PTZPresetTourSpot = NULL;
		return OPERATE_FAILED;
	}

	struct tt__PTZSpeed *Speed = NULL;
	Speed = new tt__PTZSpeed[size];
	if(Speed == NULL) 
	{
		delete PtzPresetTour;
		PtzPresetTour = NULL;

		delete [] PTZPresetTourSpot;
		PTZPresetTourSpot = NULL;

		delete [] PresetDetail;
		PresetDetail = NULL;
		return OPERATE_FAILED;   
	}

	struct tt__Vector2D *PanTilt;
	PanTilt = new tt__Vector2D[size];
	if(PanTilt == NULL)
	{
		delete PtzPresetTour;
		PtzPresetTour = NULL;

		delete [] PTZPresetTourSpot;
		PTZPresetTourSpot = NULL;

		delete [] PresetDetail;
		PresetDetail = NULL;

		delete [] Speed;
		Speed = NULL;
		return OPERATE_FAILED;   
	}

	for(i = 0 ;i<size;i++)
	{
		PanTilt[i].space = PresetTour->TourSpeedSpace;
		PanTilt[i].x     = PresetTour->TourXSpeed[i];
		PanTilt[i].y     = PresetTour->TourYSpeed[i];
		Speed[i].PanTilt = &(PanTilt[i]);
		Speed[i].Zoom    = NULL;

		PresetDetail[i].__union_PTZPresetTourPresetDetail			= 1;
		PresetDetail[i].union_PTZPresetTourPresetDetail.PresetToken = PresetTour->PresetsToken[i];	
		PresetDetail[i].__size										= 0;
		PresetDetail[i].__any										= NULL;
		PresetDetail[i].__anyAttribute								= NULL;

		PTZPresetTourSpot[i].PresetDetail	= PresetDetail + i;	   
		PTZPresetTourSpot[i].Speed			= Speed + i;
		PTZPresetTourSpot[i].StayTime		= PresetTour->StayTime[i];

		PTZPresetTourSpot[i].Extension		= NULL;

		PTZPresetTourSpot[i].__anyAttribute = NULL;
	}

	Status.State						= tt__PTZPresetTourState__Idle;
	Status.CurrentTourSpot				= NULL;
	Status.Extension					= NULL;
	Status.__anyAttribute				= NULL;

	StartingCondition.RecurringTime		= NULL;
	StartingCondition.RecurringDuration = NULL;
	StartingCondition.Direction			= NULL;
	StartingCondition.Extension			= NULL;
	StartingCondition.__anyAttribute    = NULL;

	if(PtzPresetTour != NULL)
	{
		PtzPresetTour->Name					= NULL;
	}
	if(PtzPresetTour != NULL)
	{
		PtzPresetTour->Status				= &Status;
	}
	if(PtzPresetTour != NULL)
	{
		PtzPresetTour->AutoStart			= _false;
	}
	if(PtzPresetTour != NULL)
	{
		PtzPresetTour->StartingCondition	= &StartingCondition;
	}
	if(PtzPresetTour != NULL)
	{
		PtzPresetTour->__sizeTourSpot		= PresetTour->SizePreset;
	}
	if(PtzPresetTour != NULL)
	{
		PtzPresetTour->TourSpot				= PTZPresetTourSpot;
	}
	if(PtzPresetTour != NULL)
	{
		PtzPresetTour->Extension			= NULL;
	}
	if(PtzPresetTour != NULL)
	{
		PtzPresetTour->token				= PresetTour->TourToken;
	}
	if(PtzPresetTour != NULL)
	{
		PtzPresetTour->__anyAttribute		= NULL;
	}
	ModifyPresetTour.PresetTour = PtzPresetTour;
	ModifyPresetTour.ProfileToken = p_profileToken;
//μ÷ó?onvif?ó?ú
	result = soap_call___tptz__ModifyPresetTour(pSoap, m_PTZServAddr, NULL,&ModifyPresetTour,&ModifyPresetTourResponse);
	if(result != SOAP_OK)
	{
		onvif_free_soap_header(pHeader);
		onvif_free_soap(pSoap);
		pSoap = NULL;

		pSoap = onvif_init_soap(TDEVICE_PTZ,false);
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

		if(ret == SOAP_INIT_SUCCESS)
		{
			result = soap_call___tptz__ModifyPresetTour(pSoap, m_PTZServAddr, NULL,&ModifyPresetTour,&ModifyPresetTourResponse);
		}
	}

	if(SOAP_OK == result)
	{
		eResult = OPERATE_SUCCESS;
	}
	else
	{
		eResult = OPERATE_FAILED;
	}

	ModifyPresetTour.ProfileToken = NULL;
	ModifyPresetTour.PresetTour = NULL;

	if(Speed != NULL)
	{
		delete [] Speed;
		Speed = NULL;
	}

	if(PanTilt != NULL)
	{
		delete [] PanTilt;
		PanTilt = NULL;
	}

	if(PresetDetail != NULL)
	{
		delete [] PresetDetail;
		PresetDetail = NULL;
	}

	if(PTZPresetTourSpot != NULL)
	{
		delete [] PTZPresetTourSpot;
		PTZPresetTourSpot = NULL;
	}

	if(PtzPresetTour != NULL)
	{
		delete PtzPresetTour;
		PtzPresetTour = NULL;
	}

	onvif_free_soap_header(pHeader);
	onvif_free_soap(pSoap);
	pSoap = NULL;

	return eResult;
}
