
#include "NetDevicePlatform.h"
#include "NetDevice.h"
#include "OnvifNetDeviceServ.h"
#include "IPDataBufferMan.h"
#include "DigestGen.h"
#include "OnvifEvent.h"
#include "OnvifDevDiscoverServ.h"

//#define ONVIF_DEBUG
#define ONVIF_SECOND_STREAM_FIRST_CHN_MAX_WIDTH 1280
#define ONVIF_SECOND_STREAM_FIRST_CHN_MAX_HEIGHT 960

#define ONVIF_SECOND_STREAM_SIZE_MAX_WIDHT 720
#define ONVIF_SECOND_STREAM_SIZE_MAX_HEIGHT 576

#define ONVIF_SECOND_STREAM_SIZE_MIN_WIDHT 240
#define ONVIF_SECOND_STREAM_SIZE_MIN_HEIGHT 200

#define ONVIF_ONLY_ONE_STREAM_SIZE1_WIDTH 704
#define ONVIF_ONLY_ONE_STREAM_SIZE1_HEIGH_PAL 576
#define ONVIF_ONLY_ONE_STREAM_SIZE1_HEIGH_NTSC 480
#define ONVIF_ONLY_ONE_STREAM_SIZE2_WIDTH 640
#define ONVIF_ONLY_ONE_STREAM_SIZE2_HEIGH 480
#define ONVIF_ONLY_ONE_STREAM_SIZE3_WIDTH 1280
#define ONVIF_ONLY_ONE_STREAM_SIZE3_HEIGH 720
#define ONVIF_ONLY_ONE_STREAM_SIZE4_WIDTH 1280
#define ONVIF_ONLY_ONE_STREAM_SIZE4_HEIGH 960

#define ONVIF_MAX_IPC_RESOLUTION_COUNT 10

typedef struct _TVT_TIME_ZONE_INFO
{
	char zone_info[64];
	int  zone_index;
}TVT_TIME_ZONE_INFO;

TVT_TIME_ZONE_INFO g_time_zone_info[] = {
	{"GMT+12", 0},
	{"GMT+11", 1},
	{"HAST10HADT,M3.2.0,M11.1.0", 2},
	{"AKST9AKDT,M3.2.0,M11.1.0", 3},
	{"PST8PDT,M3.2.0,M11.1.0", 4},
	{"MST7MDT,M3.2.0,M11.1.0", 5},
	{"CST6CDT,M3.2.0,M11.1.0",6},
	{"CST6CDT,M4.1.0,M10.5.0", 6},
	{"CST5CDT,M3.2.0/0,M11.1.0/1", 7},
	{"EST5EDT,M3.2.0,M11.1.0", 7},
	{"VET4:30", 8},
	{"CST4CDT,M3.2.0,M11.1.0", 9},
	{"AMT4AMST,M10.3.0/0,M2.3.0/0", 9},
	{"AST4", 9},
	{"PYT4PYST,M10.1.0/0,M4.2.0/0", 9},
	{"CLT4CLST,M10.2.0/0,M3.2.0/0", 9},
	{"NST3:30NDT,M3.2.0,M11.1.0", 10},
	{"BRT3BRST,M10.3.0/0,M2.3.0/0", 11},
	{"ART3", 11},
	{"WGT3WGST,M3.5.6/22,M10.5.6/23", 11},
	{"UYT3UYST,M10.1.0,M3.2.0", 11},
	{"FNT2", 12},
	{"AZOT1AZOST,M3.5.0/0,M10.5.0/1", 13},
	{"GMT0BST,M3.5.0/1,M10.5.0", 14},
	{"CET-1CEST,M3.5.0,M10.5.0/3", 15},
	{"WAT1WAST,M9.1.0,M4.1.0", 15},
	{"EET-2EEST,M3.5.0/3,M10.5.0/4", 16},
	{"EET-2", 16},
	{"IST-2IDT,M3.5.5/2,M10.5.0/2", 16},
	{"SAST-2", 16},
	{"EET-2EEST,M3.5.0/0,M10.5.0/0", 16},
	{"EET-2EEST,M3.5.5/0,M10.5.5/0", 16},
	{"AST-3", 17},
	{"IRST-3:30IRDT-4:30,J80/0,J264/0", 18},
	{"AZT-4AZST,M3.5.0/4,M10.5.0/5", 19},
	{"GST4", 19},
	{"MSK-4", 19},
	{"AFT-4:30", 20},
	{"PKT-5", 21},
	{"IST-5:30", 22},
	{"NPT-5:45", 23},
	{"ALMT-6", 24},
	{"MMT-6:30", 25},
	{"WIT-7", 26},
	{"CST-8", 27},
	{"WST-8", 27},
	{"JST-9", 28},
	{"CST-9:30", 29},
	{"CST-9:30CST,M10.1.0,M4.1.0/3", 29},
	{"YAKT-10", 30},
	{"EST-10EST,M10.1.0,M4.1.0/3", 30},
	{"PST-10:0", 30},
	{"SBT-11", 31},
	{"NFT-11:30", 31},
	{"NZST-12NZDT,M9.5.0,M4.1.0/3", 32},
	{"FJT-12,M10.5.0,M1.3.0/3", 32},
	{"PETT-12PETST,M3.5.0,M10.5.0/3", 32},
	{"MHT-12", 32},
	{"CHAST-12:45CHADT,M9.5.0/2:45,M4.1.0/3:45", 33},
	{"TOT-13", 33},
	{"", 100000},
};


#ifndef mmioFOURCC1
#define mmioFOURCC1( ch0, ch1, ch2, ch3 ) \
	( (unsigned long)(unsigned char)(ch0) | ( (unsigned long)(unsigned char)(ch1) << 8 ) |	\
	( (unsigned long)(unsigned char)(ch2) << 16 ) | ( (unsigned long)(unsigned char)(ch3) << 24 ) )
#endif

const int TVT_MAX_CRUISE_NUM = 16;
COnvifNetDevice::COnvifNetDevice()
{
	InitObjectValiant();
}

COnvifNetDevice::~COnvifNetDevice()
{
	QuitNetDev();

#ifdef ONVIF_WRITE_FILE_DEBUG
   if(m_file != NULL)
   {
	   fclose(m_file);
   }
   if(m_audioFile != NULL)
   {
	   fclose(m_audioFile);
   }
#endif

   //free audio memory space
   if(m_pAudioBuffer != NULL)
   {
	   delete[] m_pAudioBuffer;
   }
   m_pAudioBuffer = NULL;

   if(m_majorParamRange.ipcReso.resoContent != NULL)
   {
	   delete[] m_majorParamRange.ipcReso.resoContent;
   }
   m_majorParamRange.ipcReso.resoContent = NULL;

   if(m_subParamRange.ipcReso.resoContent != NULL)
   {
	   delete[] m_subParamRange.ipcReso.resoContent;
   }
   m_subParamRange.ipcReso.resoContent = NULL;

   if(m_thirdParamRange.ipcReso.resoContent != NULL)
   {
	   delete[] m_thirdParamRange.ipcReso.resoContent;
   }
   m_thirdParamRange.ipcReso.resoContent = NULL;

   if( m_profileServ.profile )
   {
	   delete[] m_profileServ.profile;
   }
   m_profileServ.profile = NULL;
   m_profileServ.profileSize = 0;
}


void COnvifNetDevice::InitObjectValiant()
{
	memset(&m_capaServ,0,sizeof(struct OnvifDevCapaServ));

	m_profileServ.profileSize = 0;
	m_profileServ.profile = new struct OnvifMediaProData[ONVIF_MAX_PROFILE_NUMBER];

	m_userName[0] = '\0';
	m_passKey[0]  = '\0'; 

	memset(&m_encoderMajorPara,0,sizeof(struct OnvifVideoEncoder));
	memset(&m_encoderSubPara,0,sizeof(struct OnvifVideoEncoder));
	memset(&m_encoderThirdPara,0,sizeof(struct OnvifVideoEncoder));

	m_timeZone = DD_TIME_ZONE_NUM;//DD_TIME_ZONE_GMT;
	m_bDst = false;
	m_bNtpSet = false;
	memset(m_dstTz,'\0',sizeof(m_dstTz));
	memset(m_tzTab,'\0',sizeof(m_tzTab));
	memset(m_dstTab,'\0',sizeof(m_dstTab));
	strcpy(m_tzTab,"CST");
	strcpy(m_dstTab,"DST");

	//net device parameter
	InitNetDevParamer();

	InitH264ParameterRange(&m_majorParamRange);
	InitH264ParameterRange(&m_subParamRange);
	InitH264ParameterRange(&m_thirdParamRange);

	m_majorVideo.bVideo = false;
	m_majorVideo.Uri[0] = '\0';

	m_subVideo.bVideo = false;
	m_subVideo.Uri[0] = '\0';

	m_thirdVideoUri.bVideo = false;
	m_thirdVideoUri.Uri[0] = '\0';

	//device relate information init
	m_ctrlStatus   = ND_SC_ERROR;
	m_deviceStatus = ND_STATUS_DISCONNECT;
	m_rtspMajorID = -1;
	m_rtspSubID  = -1;
	m_channelNum  = -1;

	m_bRtspMajorNeedRestart	= false;
	m_bRtspSubNeedRestart	= false;
	m_bRtspThirdNeedRestart = false;
	//m_currTime = GetTickCount();

	m_iRtspMajorDiscnnCnt = 0;
	m_iRtspSubDiscnnCnt = 0;
	m_iRtspThirdDiscnnCnt = 0;

	m_majorVideoCurrCount = 0;
	m_subVideoCurrCount = 0;
	m_thirdVideoCurrCount = 0;

	m_majorVideoSaveCount = 0;
	m_subVideoSaveCount = 0;
	m_thirdVideoSaveCount = 0;

	m_saveCheckTimes = 0;

	m_rtspThirdID = -1;
	m_bMajorRequest = false;
	m_bSecRequest = false;
	m_bThirdRequest = false;

	m_bDevicePtzCtlActive  = false;

	m_ulOnvifLastError = 0;
	m_ulMajorLastError = 0;
	m_ulSubLastError = 0;
	m_ulThirdLastError = 0;

#ifdef ONVIF_WRITE_FILE_DEBUG
	m_file = fopen("F://tvt_ipc_in_buffer.dat","wb");
	m_audioFile = fopen("F://audio_in_buffer.dat","wb");
#endif

	m_netTimeout = 10000;
	m_fastCheckNetTimeNumber = m_netTimeout;

	m_pAudioBuffer = NULL;

	m_snapImageID = RTSP_STREAMID_FOR_SNAP_VAL;

	memset( &m_stMajorEncCfg, 0, sizeof(DD_ENCODE_CONFIG) );
	memset( &m_stSubEncCfg, 0, sizeof(DD_ENCODE_CONFIG) );
	memset( &m_stThirdEncCfg, 0, sizeof(DD_ENCODE_CONFIG) );

	m_bFirstSetNetEncCfg = false;

	m_bMajorResolutionWidth = 0;
	m_bMajorResolutionHeight = 0;

	memset( &m_stSaveDatatimeCfg, 0, sizeof(DD_DATE_TIME_CONFIG) );

	m_bIpcHasThirdStream = false;
	m_bHasRequestSecondStream = false;
	m_bHasRequestThirdStream = false;

	m_bIpcOnlyMasterStream = false;

	m_ulPrevGetIpcEventTime = 0;

	m_eventEnable = false;

	m_iIpcCtrlUsedType = ONVIF_CTRL_DEV_USED_NORMAL;
	memset( m_strSaveServUri, 0, sizeof(m_strSaveServUri) );

	m_bOnvifCtrlEnable = false;

	//audio memory space
	m_pAudioBuffer = new char[ONVIF_AUDIO_FRAME_BUF_MAX_LEN];
	if(m_pAudioBuffer != NULL)
	{
		memset(m_pAudioBuffer,0,sizeof(char)*ONVIF_AUDIO_FRAME_BUF_MAX_LEN);
	}

	m_run_lines = 0;
	m_2run_lines = 0;

	m_manu_prod_id = MANU_ONVIF_NORMAL;
	m_ipc_timezone_mode = ONVIF_TIMEZONE_SET_NORMAL_MODE;

	m_iIpcOnyMsMaxWidth = ONVIF_SECOND_STREAM_SIZE_MAX_WIDHT;
	m_iIpcOnyMsMaxHeight = ONVIF_SECOND_STREAM_SIZE_MAX_HEIGHT;


	m_bSecondThirdSteamOnlyCif = false;

	m_bFastCheckVideoLoss = false;
	m_iFastCheckMasterCount = 0;
	m_iFastCheckMastrePrevCount = 0;
}

void COnvifNetDevice::PrintRunPos()
{
#ifdef __ENVIRONMENT_LINUX__
	if( -1 != access( "/mnt/mtd/ts_pri", F_OK ) )
#else
	if( 0 )
#endif
	{
		printf("m_run_lines onvif ctrl=%d,%d--%d\n", m_run_lines, m_2run_lines, m_channelNum );
	}
}

//bool COnvifNetDevice::GetPtzInfo(void *pData,int tDataLen,int dataNum,int sDataLen)
//{
//	TVT_PTZ_ADVANCE_CONFIG *pCfgData	= NULL;
//	int m								= 0;
//
//	char proToken[64];
//	char  nodeToken[32];
//
//	int cruiseNum						= 0;
//	int  presetNum						= 0;
//	int  minPreset						= 0;
//	int  add							= 0;
//	int totalCruiseNum					= 0;
//	int chnn                            = -1;
//
//	COnvifDevCtl OnvifCtrl;
//	PtzNodeData PTZNodeResp;
//	PtzPresesData PresesData;
//
//	PtzPresetTours Tours;
//	PtzPresetTour PresetTour;
//
//	bool  bCruiseInipc[TVT_MAX_CRUISE_NUM]; //mark the cruise in device
//
//	char tourToken[8]; //tour token
//
//	memset(tourToken,0,sizeof(tourToken));
//
//	memset(bCruiseInipc,0,sizeof(bCruiseInipc));
//
//	memset(proToken,0,sizeof(proToken));
//	memset(nodeToken,0,sizeof(nodeToken));
//
//	memset(&PTZNodeResp,0,sizeof(PtzNodeData));
//	memset(&PresesData,0,sizeof(PtzPresesData));
//
//	memset(&Tours,0,sizeof(PtzPresetTours));
//	memset(&PresetTour,0,sizeof(PtzPresetTour));
//
//	if(pData == NULL || tDataLen == 0)
//	{
//		return false;
//	}
//
//	if(!m_devCapa.bPTZEnable)
//	{
//		return false;
//	}
//
//	if(m_devCapa.PTZServ[0] == '\0')
//	{
//		return false;
//	}
//
//	pCfgData = (TVT_PTZ_ADVANCE_CONFIG *)pData;
//
//	pCfgData->supportTrack       = 0;
//	pCfgData->supportTrack       = 0;
//
//	m_channelLock.Lock();
//	chnn = m_dwDeviceID;
//	m_channelLock.UnLock();
//
//    lock_onvif_ctrl_op.Lock();
//	if ( NULL != m_profileServ.profile)
//	{
//		strcpy(proToken,m_profileServ.profile->profileToken);
//		if (NULL != m_profileServ.profile->ptzDefaultInfo)
//		{
//			strcpy(nodeToken,m_profileServ.profile->ptzDefaultInfo->nodeToken);
//		}
//	}
//    lock_onvif_ctrl_op.UnLock();
//	
//	OnvifCtrl.InitPTZServAddr(m_capaServ.PTZServ, m_userName, m_passKey);
//	OnvifCtrl.InitPTZProfileToken(proToken);
//	int ret1 = OnvifCtrl.GetNode(nodeToken,&PTZNodeResp,1);
//
//	if(ret1 != OPERATE_SUCCESS)
//	{
//		printf("channel = %d,ONVIF get PTZ node failed!\n",chnn);
//		pCfgData = NULL;
//		return false;
//	}
//	//judge the max cruise num
//	if(PTZNodeResp.MaxPresetTours > 0)  
//	{
//		pCfgData->cruise.enableCruise = 1;
//		pCfgData->cruise.maxCruiseNum = PTZNodeResp.MaxPresetTours;
//	}
//	else 
//	{
//		pCfgData->cruise.enableCruise = 1; 
//		pCfgData->cruise.maxCruiseNum = TVT_MAX_CRUISE_NUM;
//	} 
//	bool bPresetZeroBegin = false;
//	if(PTZNodeResp.MaximumNumberOfPresets > 0)
//	{
//		pCfgData->preset.supportPreset = 1;
//		pCfgData->preset.maxPresetNum  = PTZNodeResp.MaximumNumberOfPresets;
//
//		ret1 = 0;
//		ret1 = OnvifCtrl.GetPresets(&PresesData);
//
//		if(ret1 == OPERATE_SUCCESS)
//		{
//			for(int i= 0,j=0;i< PresesData.SizePresets;i++)
//			{
//				j = atoi(PresesData.PresetsToken[i]);
//				//assert(j > 0 && j <= TVT_MAX_PRESET_NUM);
//				if(j ==0)
//				{
//					bPresetZeroBegin = true;
//				}
//
//				//预置点的个数从0开始计数
//				if(bPresetZeroBegin) 
//				{
//					if(j >= 0 && j < TVT_MAX_PRESET_NUM)
//					{
//						pCfgData->preset.enablePreset[j] = 1;   
//					}
//				}
//				else  //预置点个数从非0开始计数
//				{
//					if(j > 0 && j <= TVT_MAX_PRESET_NUM)
//					{
//						pCfgData->preset.enablePreset[j-1] = 1;
//					}
//				}
//			}
//		}
//	}
//
//	ret1 = 0;
//	ret1 = OnvifCtrl.GetTours(proToken,&Tours);
//
//	if(Tours.SizeTour > 0 && ret1 == OPERATE_SUCCESS) //cruise num > 0
//	{
//		cruiseNum = atoi(Tours.TourToken[0]);
//		if(cruiseNum == 0)
//		{
//			add = 1;
//			m_bPtzCruiseZeroBegin = true;
//		}
//		else
//		{
//			add = 0;
//			m_bPtzCruiseZeroBegin = false;
//		}
//
//		totalCruiseNum = TVT_MAX_CRUISE_NUM < Tours.SizeTour ? TVT_MAX_CRUISE_NUM:Tours.SizeTour;
//		for(i = 0; i < totalCruiseNum; i++)
//		{
//			cruiseNum = atoi(Tours.TourToken[i]);
//			cruiseNum += add;
//			assert(cruiseNum > 0 && cruiseNum <= TVT_MAX_CRUISE_NUM);
//
//			bCruiseInipc[cruiseNum -1]	= true;
//
//			// get the preset in cruise
//
//			ret1=OnvifCtrl.GetPresetsInTour(Tours.TourToken[i],&PresetTour);
//
//			pCfgData->cruise.cruise[cruiseNum-1].maxPointNum   = TVT_MAX_CRUISE_PRESET_NUM;//255;
//			pCfgData->cruise.cruise[cruiseNum-1].validPointNum = PresetTour.SizeTourPresets;
//			pCfgData->cruise.cruise[cruiseNum-1].maxPresetNum  = PTZNodeResp.MaximumNumberOfPresets;
//
//			if(PresetTour.SizeTourPresets > 0) //preset in cruise
//			{
//				minPreset = TVT_MAX_CRUISE_PRESET_NUM < PresetTour.SizeTourPresets ? TVT_MAX_CRUISE_PRESET_NUM:PresetTour.SizeTourPresets;
//
//				for(m = 0;m < minPreset;m++)
//				{
//					presetNum = atoi(PresetTour.PresetToken[m]);
//					pCfgData->cruise.cruise[cruiseNum-1].point[m].presetNo = presetNum -1;
//
//					if(m_PresetTourData.TourStayTime[cruiseNum-1][m] != 0) 
//					{
//						pCfgData->cruise.cruise[cruiseNum-1].point[m].times = m_PresetTourData.TourStayTime[cruiseNum-1][m];
//					}
//					else
//					{
//						pCfgData->cruise.cruise[cruiseNum-1].point[m].times = 5;
//					}
//
//					if(m_PresetTourData.TourSpeed[cruiseNum-1][m] != 0)
//					{
//						pCfgData->cruise.cruise[cruiseNum-1].point[m].speed = m_PresetTourData.TourSpeed[cruiseNum-1][m];
//					}
//					else
//					{
//						pCfgData->cruise.cruise[cruiseNum-1].point[m].speed = 1;
//					}
//				}//for preset num
//				m_bPresetInCruise[i]		= true;	//this cruise have preset
//			} 
//			else
//			{
//				m_bPresetInCruise[i]		= false; //this cruise have  no preset
//			}//if
//		}//for
//	}
//
//	int cResult = -1;
//	for(i= 0;i<TVT_MAX_CRUISE_NUM;i++)
//	{
//		if(!bCruiseInipc[i]) //this cruise is not exist and create cruise
//		{
//			cResult = -1;
//			memset(tourToken,0,sizeof(tourToken));
//
//			cResult = OnvifCtrl.CreatePresetTour(proToken,tourToken);
//
//			if(cResult == OPERATE_SUCCESS)
//			{
//				if(i==0) //the ipc cruise token from 0
//				{
//					cruiseNum = atoi(tourToken);
//					if(cruiseNum == 0)
//					{
//						m_bPtzCruiseZeroBegin = true;
//					}
//					else
//					{
//						m_bPtzCruiseZeroBegin = false;
//					}
//				}
//			}
//			else
//			{
//				break;
//			}
//
//			pCfgData->cruise.cruise[i].maxPointNum   = TVT_MAX_CRUISE_PRESET_NUM;//255;
//			pCfgData->cruise.cruise[i].validPointNum = 0;
//			pCfgData->cruise.cruise[i].maxPresetNum  = PTZNodeResp.MaximumNumberOfPresets;
//			//this cruise have no preset
//			m_bPresetInCruise[i] = false;
//		}
//	} 
//	pCfgData = NULL;
//	return true;
//}

void COnvifNetDevice::PtzInfoHanlde(const unsigned char* pData,int tDataLen)
{
    //首先怎么去取出数据相应的数据（条巡航线）

	//这里主要是对添加和修改

    //添加一个函数判断m_bPtzCruiseZeroBegin（是从0开始，还是从1开始）
	if( (pData == NULL)||( tDataLen <= 0) )
	{
		return;
	}
	char StayTimeBuf[16];
	char TourBuf[16];
	char PtzPresetTour[16];

	char proToken[64];
	char nodeToken[32];
	char speedPTSpace[128];
	char tourToken[8];
	COnvifDevCtl onvifCtrl;
	
	int majorNum = GetMajorStreamToken();
	if(majorNum <= 0)
	{
		return;
	}
	if(!m_capaServ.bPTZEnable)
	{
		return;
	}
	if(m_capaServ.PTZServ[0] == '\0')
	{
		return;
	}

	OnvifPTZDevInfo defaultPTZInfo;
    OnvifModifyPresetTour PresetTour;
	OnvifDevPTZNode PTZNodeResp;

	memset(&PresetTour,  0, sizeof(OnvifModifyPresetTour));
	memset(&PTZNodeResp, 0, sizeof(OnvifDevPTZNode));

	memset(tourToken,0,sizeof(tourToken));

	memset(StayTimeBuf,0,sizeof(StayTimeBuf));
	memset(TourBuf,0,sizeof(TourBuf));
	memset(PtzPresetTour,0,sizeof(PtzPresetTour));

	memset(proToken,0,sizeof(proToken));
	memset(nodeToken,0,sizeof(nodeToken));
	memset(speedPTSpace,0,sizeof(speedPTSpace));

    DevPTZSpaces * pdevPTZSpaces = new DevPTZSpaces;
	PTZNodeResp.SupportedPTZSpaces = pdevPTZSpaces;

	lock_onvif_ctrl_op.Lock();
	strcpy(proToken,m_profileServ.profile[majorNum-1].profileToken);
    if(m_profileServ.profile[majorNum-1].ptzDefaultInfo != NULL)
	{						         memcpy(&defaultPTZInfo,m_profileServ.profile[majorNum-1].ptzDefaultInfo,sizeof(OnvifPTZDevInfo));
	}
	strcpy(nodeToken,defaultPTZInfo.nodeToken);
	strcpy(speedPTSpace,defaultPTZInfo.speedPTSpace);
	lock_onvif_ctrl_op.UnLock();

	int ret = -1;
	onvifCtrl.InitPTZServAddr(m_capaServ.PTZServ, m_userName, m_passKey);
	onvifCtrl.InitPTZProfileToken(proToken);
	/*ret = onvifCtrl.GetPtzNode(nodeToken,&PTZNodeResp);*/
	ret = onvifCtrl.GetNode(nodeToken,&PTZNodeResp,1);

	if(ret != OPERATE_SUCCESS)
	{
		delete pdevPTZSpaces;
		return;
	}

	CruiseInfo * pCruiseHeader =  NULL;
	CruisePresetInfo * pCruisePreset = NULL;
    unsigned int offset = 0;
	pCruiseHeader = (CruiseInfo *)pData;
    for(unsigned int i = 0; tDataLen > offset;i++)
	{
		PresetTour.SizePreset = pCruiseHeader->cruisepointnum;
		if( 1 )   //现在假设是从1 开始的，以后做一个获取ptzInfo 的 接口来设置这些相关的信息 m_bPtzCruiseZeroBegin
		{		
			sprintf(PresetTour.TourToken,"%d",i+1);
		}
		else
		{
			sprintf(PresetTour.TourToken,"%d",i);
		}	

		offset += sizeof(CruiseInfo);
		pCruisePreset = (CruisePresetInfo *)(pData+offset);
		for(unsigned int j = 0;j<PresetTour.SizePreset;j++)
		{
			sprintf(PresetTour.StayTime[j],"PT%dS",pCruisePreset->dwellTime);
			sprintf(PresetTour.PresetsToken[j],"%d",pCruisePreset->preset);
			

			if(pCruisePreset->dwellSpeed > 4)
			{
				PresetTour.TourXSpeed[j] = PTZNodeResp.SupportedPTZSpaces->ContinuousPanTiltVelocitySpace[0][0];
				PresetTour.TourYSpeed[j] = (float)(5*(pCruisePreset->dwellSpeed-4))/16;
				if(PresetTour.TourYSpeed[j] >PTZNodeResp.SupportedPTZSpaces->ContinuousPanTiltVelocitySpace[1][0])
				{
					PresetTour.TourYSpeed[j] = PTZNodeResp.SupportedPTZSpaces->ContinuousPanTiltVelocitySpace[1][0];
				}
			}
			else
			{
				PresetTour.TourXSpeed[j] = (float)(5*pCruisePreset->dwellSpeed)/16;
				PresetTour.TourYSpeed[j] = 0;
				if(PresetTour.TourXSpeed[j] > PTZNodeResp.SupportedPTZSpaces->ContinuousPanTiltVelocitySpace[0][0])
				{
					PresetTour.TourXSpeed[j] = PTZNodeResp.SupportedPTZSpaces->ContinuousPanTiltVelocitySpace[0][0];
				}
			}
			strncpy(PresetTour.TourSpeedSpace,speedPTSpace,sizeof(PresetTour.TourSpeedSpace)); 
			pCruisePreset++;
		}
		offset += (pCruiseHeader->cruisepointnum) * sizeof(CruisePresetInfo);
		int reselt = onvifCtrl.ModifyPresetInTour(&PresetTour,1);
		printf("%s,%d,%d",__FILE__,__LINE__,reselt);
		pCruiseHeader = (CruiseInfo *)(pData + offset);
	}
	delete pdevPTZSpaces;
    return;
}

//////////////////////////////////////////////////////////////////////////
/************************************************************************
接口功能描述：初始化设备接口。
输入参数描述：
1、chnn：	分配给该设备的通道号；
2、IP：		该设备的网络地址；
3、user：	用于登录的用户名称；
4、netPort：登录端口
5、httpPort：IE连接端口
6、ctrlPort： 控制端口
4、password：用于登录的密码；
5、mac:		本地设备的网络物理地址，用于登录验证使用。
返回值描述：返回成功或者失败。
注: 在该接口内部，需要尝试登录到IP设备，根据登录的结果返回结果。
注: 在接口内部, 处理重复初始化
注: 各个设备接口, 必须依赖连接状态做相应处理
************************************************************************/
bool COnvifNetDevice::Initial(nd_in long deviceId,						//
					 nd_in long chnn,							//
					 nd_in char *pServer,				//
					 nd_in unsigned short netPort,				//
					 nd_in unsigned short httpPort,				//
					 nd_in unsigned short ctrlPort,				//
					 nd_in const std::string & user,			//
					 nd_in const std::string & password,		//
					 nd_in const ND_MAC & mac,					//
					 nd_in const char* pEther,					//
					 nd_in long sensorNum)						//
{
	m_bPal = false;
    int result = -1;
    bool regResult = false;
	int mediaResult = -1;
	char userName[32] = {'\0'};
	char userPassword[32] = {'\0'};
	char servUri[128] = {'\0'};
    int  majorStreamNum = 0;
	int  subStreamNum  = 0;
	int thirdStreamNum = 0;
    int tLen = 0;
	COnvifDevMgt devmgt;
    COnvifDevConf devConf;

	m_lTestFrameMasterCnt = 0;
	m_lTestFrameSecondCnt = 0;
	m_lTestFrameThirdCnt = 0;

	m_iFastCheckMasterCount = 0;
	m_iFastCheckMastrePrevCount = 0;
	m_iFastCheckPrevTime = 0;
    
	//初始化的时候清理数据格式参数
	m_pOnvifNetDev->m_mainVideoFormat.biWidth = 0;
	m_pOnvifNetDev->m_mainVideoFormat.biHeight = 0;

	m_pOnvifNetDev->m_secVideoFormat.biWidth = 0;
	m_pOnvifNetDev->m_secVideoFormat.biHeight = 0;

	m_pOnvifNetDev->m_thirdVideoFormat.biWidth = 0;
	m_pOnvifNetDev->m_thirdVideoFormat.biHeight = 0;

	memset( &m_stSaveDatatimeCfg, 0, sizeof(DD_DATE_TIME_CONFIG) );


	m_channelLock.Lock();
	m_channelNum  = chnn;
	m_sensorNum = sensorNum;
	m_channelLock.UnLock();

	//printf( "get dev init channel %d\n", m_channelNum );

	lock_onvif_ctrl_op.Lock();
	m_majorParamRange.ipcReso.resoSize = 0;
	m_subParamRange.ipcReso.resoSize = 0;
	m_thirdParamRange.ipcReso.resoSize = 0;
	lock_onvif_ctrl_op.UnLock();

	m_iIpcOnyMsMaxWidth = ONVIF_SECOND_STREAM_SIZE_MAX_WIDHT;
	m_iIpcOnyMsMaxHeight = ONVIF_SECOND_STREAM_SIZE_MAX_HEIGHT;

	tLen = user.copy(userName,user.length());
    userName[tLen] = '\0';
	tLen =password.copy(userPassword,password.length());
	userPassword[tLen] = '\0';
   
    memset(servUri,'\0',sizeof(servUri));
	sprintf(servUri,"%s%s:%d","http://",pServer,netPort);
    strcat(servUri,"/onvif/device_service");
  
	//printf( "get dev capability channel %d\n", m_channelNum );
	//lock_onvif_ctrl_op.Lock();
	devmgt.InitDevMgtParameter(servUri,userName,userPassword);
	devmgt.InitDevUtcTimes(false,servUri,m_iIpcCtrlUsedType);
	result = devmgt.GetDevCapability(servUri,TAll,&m_capaServ,1);
	if(result != OPERATE_SUCCESS)
	{
		result = devmgt.GetDevCapability(servUri,TAll,&m_capaServ,0);
	}

	if( OPERATE_SUCCESS != result )
	{
		result = devmgt.GetDevUtcTimeCapability(servUri,TAll,&m_capaServ,1);
		if( OPERATE_SUCCESS == result )
		{
			m_iIpcCtrlUsedType = ONVIF_CTRL_DEV_USED_UTC_TIEM;
		}
	}
	//lock_onvif_ctrl_op.UnLock();

	if(result != OPERATE_SUCCESS)
	{
		#ifdef ONVIF_DEBUG
			printf("****device:%s,get capability failed!\n",servUri);	
		#endif

		m_ulOnvifLastError |= ONVIF_ERROR_USER_PSW;

		return false;
	}

	if(!m_capaServ.bDevMgtEnable)
	{
		m_ulOnvifLastError |= ONVIF_ERROR_MGTDEV;
		return false;
	}

	//register user 
	regResult = true;//devmgt.RegisterUser(m_capaServ.devMgtServ,userName,userPassword,1);
    if(!regResult)
    {
		m_userName[0] = '\0';
		m_passKey[0]  = '\0';
		return false;
	}

	  strcpy(m_userName,userName);
	  strcpy(m_passKey,userPassword);
	  strcpy(m_strSaveServUri,servUri);
     
	  if(m_capaServ.bAnalyticsEnable)
		  UpdateIPAddr(m_capaServ.analyticsServ,pServer,netPort);
	  if(m_capaServ.bDevMgtEnable)
		  UpdateIPAddr(m_capaServ.devMgtServ,pServer,netPort);
	  if(m_capaServ.bImagingEnable)
		  UpdateIPAddr(m_capaServ.imagingServ,pServer,netPort);
	  if(m_capaServ.bMediaEnable)
		  UpdateIPAddr(m_capaServ.mediaServ,pServer,netPort);
	  if(m_capaServ.bPTZEnable)
		  UpdateIPAddr(m_capaServ.PTZServ,pServer,netPort);
	  if(m_capaServ.bEventEnable)
		  UpdateIPAddr(m_capaServ.eventsServ,pServer,netPort);

      memset(m_serverAddr,0,sizeof(m_serverAddr));
	  strcpy(m_serverAddr,pServer);

	  if( !m_profileServ.profile )
	  {
		  printf( "current profile addr error %d,%s,%d\n", m_channelNum, __FILE__, __LINE__ );
		  return false;
	  }
	  m_profileServ.profileSize = 0;

	  //printf( "get dev begingetprofile channel %d\n", m_channelNum );
	  //get media profile token.include video encoder,video source.
	  //lock_onvif_ctrl_op.Lock();
	  devConf.InitDevMediaParameter(m_capaServ.mediaServ,m_userName,m_passKey);
	  devConf.InitDevUtcTimes( false, m_strSaveServUri, m_iIpcCtrlUsedType );
	  mediaResult = devConf.GetMediaProfiles(&m_profileServ,1);
	  if(mediaResult != OPERATE_SUCCESS)
	  {
			mediaResult = devConf.GetMediaProfiles(&m_profileServ,0);
	  }

	  if( OPERATE_SUCCESS != mediaResult )
	  {
		  devConf.InitDevUtcTimes( false, m_strSaveServUri, ONVIF_CTRL_DEV_USED_UTC_TIEM );
		  mediaResult = devConf.GetMediaProfiles(&m_profileServ,1);
		  if( OPERATE_SUCCESS == mediaResult )
		  {
			  m_iIpcCtrlUsedType = ONVIF_CTRL_DEV_USED_UTC_TIEM;
		  }
	  }

	  //printf( "get dev profile channel %d\n", m_channelNum );
	  //lock_onvif_ctrl_op.UnLock();

	  if(mediaResult != OPERATE_SUCCESS)
	  {
#ifdef ONVIF_DEBUG
		  printf("***Get profiles failed!\n");
#endif
		  m_ulOnvifLastError |= ONVIF_ERROR_GET_PROFILE;

		  return false;
	  }

	  //lock_onvif_ctrl_op.Lock();
	  m_bOnvifCtrlEnable = true;
	  //lock_onvif_ctrl_op.UnLock();


      struct OnvifVideoEncoder    tmpencoderSubPara;
	  majorStreamNum = GetMajorStreamToken();
	  subStreamNum   = GetSubStreamToken();
	  thirdStreamNum = GetThirdStreamToken();
	  //ipc have more two profile,but the sub profile is not H264,set the two profile
	  if( 0 ) //subStreamNum == 0 && m_profileServ.profileSize >1)
	  {
		  memset(&tmpencoderSubPara,0,sizeof(struct OnvifVideoEncoder));

		  GetIpcEncoderParameter(majorStreamNum+1,&tmpencoderSubPara);
		  tmpencoderSubPara.encoderType = TPRO_H264; 

		  //lock_onvif_ctrl_op.Lock();
		  devConf.InitDevMediaParameter(m_capaServ.mediaServ,m_userName,m_passKey);
		  devConf.InitDevUtcTimes(false,m_strSaveServUri,m_iIpcCtrlUsedType);
		  result = devConf.SetVideoEncoder(m_profileServ.profile[majorStreamNum].videoEncoderConfToken,true,&tmpencoderSubPara,1); 
          if(result != OPERATE_SUCCESS)
		  {
           result = devConf.SetVideoEncoder(m_profileServ.profile[majorStreamNum].videoEncoderConfToken,true,&tmpencoderSubPara,0); 
		  }
          //lock_onvif_ctrl_op.UnLock();

		  if(result == OPERATE_SUCCESS)
		  {
			  subStreamNum = majorStreamNum+1;
			  m_profileServ.profile[majorStreamNum].profileType = TPRO_H264;
		  }
	  }

	  if( majorStreamNum <= 0 )
	  {
		  m_ulOnvifLastError |= ONVIF_ERROR_NO_SUB_STR;

		  //lock_onvif_ctrl_op.Lock();
		  m_bOnvifCtrlEnable = false;
		  //lock_onvif_ctrl_op.UnLock();
		  return false;
	  }

	  if( subStreamNum <= 0 )
	  {
		  m_bIpcOnlyMasterStream = true;
	  }
      
	  //只有在通道0的时候，只有一个码流的IPC可以支持1280x960
	  if( m_bIpcOnlyMasterStream && 0 == m_channelNum )
	  {
		  m_iIpcOnyMsMaxWidth = ONVIF_SECOND_STREAM_FIRST_CHN_MAX_WIDTH;
		  m_iIpcOnyMsMaxHeight = ONVIF_SECOND_STREAM_FIRST_CHN_MAX_HEIGHT;
	  }
	  else
	  {
		  m_iIpcOnyMsMaxWidth = ONVIF_SECOND_STREAM_SIZE_MAX_WIDHT;
		  m_iIpcOnyMsMaxHeight = ONVIF_SECOND_STREAM_SIZE_MAX_HEIGHT;
	  }

	  bool bHasLessD1Stream = false;
	  if(majorStreamNum > 0)
	  {
		  GetIpcVideoUri(T_MAJORSSTREAM,majorStreamNum);
	  }
	  InitIpcResolution(majorStreamNum,T_MAJORSSTREAM, bHasLessD1Stream);
	  if( m_bIpcOnlyMasterStream )
	  {
		  //当前只有一个码流,需要小于d1 的分辨率的码流
		  if( !bHasLessD1Stream )
		  {
			  m_ulOnvifLastError |= ONVIF_ERROR_NO_SUB_STR;

			  //lock_onvif_ctrl_op.Lock();
			  m_bOnvifCtrlEnable = false;
			  //lock_onvif_ctrl_op.UnLock();
			  return false;
		  }
		  else
		  {
			  if( !CheckOnlyOneStreamMasterSize(&m_majorParamRange) )
			  {
				  m_ulOnvifLastError |= ONVIF_ERROR_NO_SUB_STR;
				  m_bOnvifCtrlEnable = false;
				  return false;
			  }
		  }
	  }

	  if(subStreamNum != 0)
	  {
		  bHasLessD1Stream = false;
		  InitIpcResolution(subStreamNum,T_SUBSTREAM, bHasLessD1Stream);

		  if( !bHasLessD1Stream )
		  {
			  m_ulOnvifLastError |= ONVIF_ERROR_NO_SUB_STR;
		  }
		  GetIpcVideoUri(T_SUBSTREAM,subStreamNum );
	  }

	  if( 0 )
	  {
		  if( thirdStreamNum > 0 )
		  {
			  m_bIpcHasThirdStream = false;
			  InitIpcResolution(thirdStreamNum,T_THIRDSTREAM, bHasLessD1Stream);
			  GetIpcVideoUri(T_THIRDSTREAM,thirdStreamNum);
		  }
	  }

	  //printf( "get dev uri and resolution channel %d\n", m_channelNum );
	  bool bGetSerialFlag = false;
	  m_manu_prod_id = COnvifDevDiscover::Instance()->GetIPCDevviceSerial( servUri, userName, userPassword, 2, bGetSerialFlag );
	  if( !bGetSerialFlag && 0 == m_manu_prod_id )
	  {
		  m_manu_prod_id = COnvifDevDiscover::Instance()->GetIPCDevviceSerial( servUri, userName, userPassword, 1, bGetSerialFlag );
		  if( !bGetSerialFlag && 0 == m_manu_prod_id )
		  {
			  m_manu_prod_id = COnvifDevDiscover::Instance()->GetIPCDevviceSerial( servUri, userName, userPassword, 0, bGetSerialFlag );
		  }
	  }

	  if( MANU_ONVIF_XIONGMAI == m_manu_prod_id )
	  {
		  m_bSecondThirdSteamOnlyCif = true;
	  }

	  InitIpcColorRange();

	  GetTzDstTab(m_tzTab,m_dstTab);

	  COnvifDevCtl  devCtl;
	  if(m_capaServ.bPTZEnable)
	  {					
		  struct PTZPresetTours PresetTours;
		  PresetTours.SizePresetTour = 0;
		  memset(&PresetTours,0,sizeof(struct PTZPresetTours));
		  
		  //lock_onvif_ctrl_op.Lock();
		  devCtl.InitPTZServAddr(m_capaServ.PTZServ,userName,userPassword);
		  devCtl.InitDevUtcTimes(false,m_strSaveServUri,m_iIpcCtrlUsedType);
		  devCtl.GetPresetTours(m_profileServ.profile[majorStreamNum-1].profileToken,&PresetTours,1);
		  if(PresetTours.SizePresetTour < 8)
		  {
			  for(int i = 0;i<8-PresetTours.SizePresetTour;i++)
			  {

				  devCtl.CreatePresetTour(m_profileServ.profile[majorStreamNum-1].profileToken,1);
			  }
		  }
		  //lock_onvif_ctrl_op.UnLock();
	  }

	  //printf( "get dev ptz channel %d\n", m_channelNum );

#ifdef ONVIF_SNAP_ACTIVE
	  COnvifSnapJpeg::Instance()->AddDevSnap(m_channelNum);
#endif


#ifdef ONVIF_VER40_ALARM_EVENT   
	  //event
	  bool bEventProd = false;
	  bool bCanProcPullMsg = false;

	  m_ipc_timezone_mode = ONVIF_TIMEZONE_SET_NORMAL_MODE;
	  if( MANU_ONVIF_DAHUA == m_manu_prod_id 
		  || MANU_ONVIF_LONGPLUS == m_manu_prod_id )
	  {
		  m_ipc_timezone_mode = ONVIF_TIMEZONE_SET_DAHUA_MODE;
	  }

	  if( MANU_ONVIF_XIONGMAI == m_manu_prod_id )
	  {
		  //m_capaServ.bEventEnable = false;
		  //bEventProd = false;

		  bEventProd = true;
	  }
	  else
	  {
		  bEventProd = true;
	  }

	  if( MANU_ONVIF_DAHUA == m_manu_prod_id 
		  || MANU_ONVIF_HIKVISION == m_manu_prod_id 
		  || MANU_ONVIF_QSEE == m_manu_prod_id  
		  || MANU_ONVIF_KEC == m_manu_prod_id 
//		  || MANU_ONVIF_SUNELL == m_manu_prod_id
		  || MANU_ONVIF_XIONGMAI == m_manu_prod_id )
	  {
		  bCanProcPullMsg = true;
	  }

	  if(bEventProd && m_capaServ.bEventEnable)
	  {
		  //lock_onvif_ctrl_op.Lock();
		  devConf.InitDevEventParameter(m_capaServ.eventsServ,m_userName,m_passKey);
		  devConf.InitDevUtcTimes(false,m_strSaveServUri,m_iIpcCtrlUsedType);
		  devConf.GetEventProperty(m_eventEnable, 1, m_manu_prod_id);
		  if( !m_eventEnable )
		  {
				devConf.GetEventProperty(m_eventEnable, 1, m_manu_prod_id);
		  }
		  //lock_onvif_ctrl_op.UnLock();

		  if(m_eventEnable)
		  {
			  OnvifEventChnnInfo stOnvifEvnetInfo;
			  memset( &stOnvifEvnetInfo, 0, sizeof(OnvifEventChnnInfo) );

#ifdef WIN32			
			  COnvifEvent::EventInstance()->SetListenAddr("192.168.3.53",50099);
			  strcpy( stOnvifEvnetInfo.localIpAddr, "192.168.3.53" );
#else
			  //get the alarm server address
			  //COnvifEvent::EventInstance()->SetListenAddrByManual("192.168.3.18",50099);
			  COnvifEvent::EventInstance()->GetListenAddress( stOnvifEvnetInfo.localIpAddr, 96, pEther, strlen(pEther));
			  printf( "current ipc event server ip addr : %s,%s, %d\n", stOnvifEvnetInfo.localIpAddr, pEther, m_channelNum );
#endif

			  strcpy( stOnvifEvnetInfo.userName, m_userName );
			  strcpy( stOnvifEvnetInfo.passKey, m_passKey );
			  strcpy( stOnvifEvnetInfo.servAddr, m_capaServ.eventsServ );
			  stOnvifEvnetInfo.iIpcCtrlType = m_iIpcCtrlUsedType;
			  strcpy( stOnvifEvnetInfo.devUtcServAddr,m_strSaveServUri );
			  stOnvifEvnetInfo.bCanProcessPullMsg = bCanProcPullMsg;
			  COnvifEvent::EventInstance()->AddChnnEvent(m_channelNum,&stOnvifEvnetInfo);	    
			  //event
			  COnvifEvent::EventInstance()->ResetChnnEvent(m_channelNum,m_serverAddr); 
		  }
	  }
#endif

	  //printf( "get dev event channel %d\n", m_channelNum );

   //device active label
	m_bDevicePtzCtlActive = true;
	m_bFirstSetNetEncCfg = true;

	//m_reConnectCount++;

	return true;
}

/************************************************************************
接口功能描述 : 反初始化
************************************************************************/
void COnvifNetDevice::Quit()
{  
   int  rtspMajorID= -1;
   int  rtspSubID  = -1;
   int rtspThirdID = -1;

   if( m_channelNum < 0 )
   {
	   printf( "current dev no init %d,%s\n", __LINE__, __FILE__ );
	   return;
   }

   m_devStatusLock.Lock();
   m_deviceStatus = ND_STATUS_DISCONNECT;
   m_devStatusLock.UnLock();

	m_rtspLock.Lock();
    rtspMajorID  = m_rtspMajorID;
	rtspSubID	  = m_rtspSubID;
    rtspThirdID	  =  m_rtspThirdID;
	m_rtspLock.UnLock();

	//stop RTSP
	if(rtspMajorID > 0)
	{
		StopVideoStream(rtspMajorID,__LINE__);
	}
	if(rtspSubID > 0)
	{
		StopVideoStream(rtspSubID,__LINE__);
	}
	//third stream parameter
	if(rtspThirdID > 0)
	{
		StopVideoStream(rtspThirdID,__LINE__);
	}

#ifdef ONVIF_SNAP_ACTIVE
	if( m_channelNum >= 0 )
	{
		COnvifSnapJpeg::Instance()->DelDevSnap(m_channelNum);
	}
#endif

#ifdef ONVIF_VER40_ALARM_EVENT
	if(m_eventEnable && m_channelNum >= 0 )
	{
		COnvifEvent::EventInstance()->DelChnnEvent(m_channelNum);
	}
#endif

	m_rtspLock.Lock();

	m_rtspMajorID = -1;
	m_rtspSubID = -1;
	m_rtspThirdID = -1;

	m_bRtspMajorNeedRestart = false;
	m_bRtspSubNeedRestart = false;
    m_bRtspThirdNeedRestart = false;

	m_iRtspMajorDiscnnCnt = 0;
	m_iRtspSubDiscnnCnt = 0;
	m_iRtspThirdDiscnnCnt = 0;

	//third stream parameter
	m_bMajorRequest = false;
	m_bThirdRequest = false;
	m_bSecRequest = false;

	m_rtspLock.UnLock();

	lock_onvif_ctrl_op.Lock();
	
	m_majorParamRange.ipcReso.resoSize = 0;
	m_subParamRange.ipcReso.resoSize = 0;
	m_thirdParamRange.ipcReso.resoSize = 0;

	QuitDevInfo();
	printf( "QuitDevInfo done Current Channel %d lasterror %x,%x,%x\n", m_channelNum, 
		m_ulOnvifLastError | m_ulMajorLastError, m_ulSubLastError, m_ulThirdLastError );
	//m_onvifLock.Lock();
	memset(&m_majorVideo,0,sizeof(OnvifVideoUri));
	memset(&m_subVideo,0,sizeof(OnvifVideoUri));
	//m_onvifLock.UnLock();

	m_bOnvifCtrlEnable = false;

	memset(&m_encoderMajorPara,0,sizeof(OnvifVideoEncoder));
	memset(&m_encoderSubPara,0,sizeof(OnvifVideoEncoder));
	memset(&m_encoderThirdPara,0, sizeof(OnvifVideoEncoder));

	lock_onvif_ctrl_op.UnLock();

	m_channelLock.Lock();
	m_channelNum = -1;
	m_channelLock.UnLock();

 }

/************************************************************************
接口功能描述 : 设置数据缓存管理器
1、本函数由NVR调用.
2、收到网络图像数据(H264)后对其填充.
************************************************************************/
 void COnvifNetDevice::SetDataBufferMan(nd_in CIPDataBufferMan *pDataBufferMan)
 {
#ifdef ONVIF_DEBUG
	 printf("######SetDataBufferMan!\n");
#endif
	m_rtspLock.Lock();

	if(m_pOnvifNetDev != NULL)
	{
		m_pOnvifNetDev->m_pMainDataBufferMan = pDataBufferMan; //main stream buffer
		m_pOnvifNetDev->m_pSecDataBufferMan  = pDataBufferMan + 1; //sub stream buffer
		m_pOnvifNetDev->m_pThirdDataBufferMan = pDataBufferMan + 2; //third stream buffer
		m_pOnvifNetDev->m_pSnapJpegBufferMan = pDataBufferMan + 3; //jpeg stream buffer
	}

    m_rtspLock.UnLock();

 }

/***********************************************************************
修改设备绑定的通道号
************************************************************************/
 void COnvifNetDevice::SetBindChannel(nd_in long chnn)
 {  
    
	 printf("######channel:old %d- new %d,SetBindChannel!\n",m_channelNum, chnn );

	 m_channelLock.Lock();
	 m_channelNum = chnn;
	 m_channelLock.UnLock();
 }

 long COnvifNetDevice::GetBindChannel()
 {
	 long channel = 0;    

	 m_channelLock.Lock();
     channel = m_channelNum;
     m_channelLock.UnLock();

	 return channel;
 }

/***********************************************************************
修改设备绑定的传感器通道号
************************************************************************/
void COnvifNetDevice::SetBindSensorChannel(nd_in long sensorChnn)
{
	m_sensorNum = sensorChnn;
}

long COnvifNetDevice::GetBindSensorChannel()
{
	return m_sensorNum;
}



/***********************************************************************
得到该网络设备的状态,断开或正常
************************************************************************/
 long COnvifNetDevice::GetDevStatus()
 {
    NET_DEVICE_STATUS        devStatus;

	m_devStatusLock.Lock();
	devStatus = m_deviceStatus;
	m_devStatusLock.UnLock();
    
	return devStatus;
 }
 
bool COnvifNetDevice::IsReConnectting()
{
	NET_DEVICE_STATUS netDeviceStatus = (NET_DEVICE_STATUS)GetDevStatus();

	if ((0 <= m_reConnectCount) && (ND_STATUS_OK != netDeviceStatus))
	{
		return true;
	}
	else
	{
		return false;
	}
}


/************************************************************************
得到当前连接状态, ND_SC_BUSY以下, 认为不能处理后续命令                                                                    
************************************************************************/
 long COnvifNetDevice::IsReady()
 {
	 enum NET_DEVICE_STATUS_CONNECT ctrStatus;  

	 m_devStatusLock.Lock();

	 if(m_deviceStatus == ND_STATUS_OK)
	 {
		 ctrStatus = m_ctrlStatus;
         m_devStatusLock.UnLock();
		 return ctrStatus;
	 }
	 m_devStatusLock.UnLock();

	 return ND_SC_ERROR;
 }

/************************************************************************
接口功能描述：请求视频及音频数据流。
输入参数描述：
1、video1：主码流MASK。
2、video2：次码流MASK。
3、video3：手机码流MASK。
4、video4：音频流MASK。
返回值描述：返回成功或者失败。
注：对应的位为零，表示停止。
************************************************************************/
bool COnvifNetDevice::RequestStream( nd_in ULONGLONG video1,
	nd_in ULONGLONG video2,
	nd_in ULONGLONG video3,
	nd_in ULONGLONG audio)
{
	
	int  majorStreamNum = 0;
	int  subStreamNum  = 0;
	int  thirdStreamNum = 0;
    
	int majorId = -1;
	int subId   = -1;
    int thirdId = -1;
    long chnn = 128;

    bool bRequestMajor = false;
	bool bRequestSub   = false;
	bool bRequestThird = false;

	if( !m_bOnvifCtrlEnable )
	{
		printf("******ONVIF ++++++++++++++ request video:(%d)video1 = %llu,video2= %llu,video3 = %llu !\n",m_channelNum, video1,video2,video3);
		return false;
	}
	else
	{
		printf("******ONVIF request video:(%d)video1 = %llu,video2= %llu,video3 = %llu !\n",m_channelNum, video1,video2,video3);
	}
	//printf("video1=");
	//PrintfBit64(video1);

	//printf("video2=");
	//PrintfBit64(video2);

	//printf("video3=");
	//PrintfBit64(video3);

	m_rtspLock.Lock();
	majorId = m_rtspMajorID;
	subId   = m_rtspSubID;
	thirdId = m_rtspThirdID;
	m_rtspLock.UnLock();
  
	m_channelLock.Lock();
	chnn = m_channelNum;
	m_channelLock.UnLock();

	if(video1 == 0)
	{	
		m_devStatusLock.Lock();
		m_deviceStatus  = ND_STATUS_DISCONNECT;
		m_devStatusLock.UnLock();

		StopVideoStream(majorId,__LINE__);

		m_rtspLock.Lock();
		m_rtspMajorID = -1;
		m_bRtspMajorNeedRestart = false;
		m_bMajorRequest  = false;
		m_rtspLock.UnLock();


	}
	else
	{	
		if(majorId <= 0) 
		{
			bRequestMajor = true;
			majorStreamNum = GetMajorStreamToken();

			if(majorStreamNum > 0)//major stream is exist
			{
				//GetIpcVideoUri(T_MAJORSSTREAM,majorStreamNum);
				majorId = RequestVideoStream(T_MAJORSSTREAM);
				if(majorId > 0)
				{

				}
				else
				{
					printf( "current channel %d, requeststram major error %s,%d\n",
						m_channelNum, __FILE__, __LINE__ );
				}
			}

			m_rtspLock.Lock();
			m_majorVideoCurrCount = 0;
			m_bRtspMajorNeedRestart = false;
			m_rtspMajorID = majorId;
			m_bMajorRequest = true;
			m_rtspLock.UnLock();
		}
	}

	//当前IPC只有一个主码流
	if( m_bIpcOnlyMasterStream )
	{
		if( 0 == video2 )
		{
			m_rtspLock.Lock();
			m_rtspSubID = -1;
			m_bRtspSubNeedRestart = false;
			m_bSecRequest  = false;
			m_rtspLock.UnLock();
		}
		else
		{
			bRequestSub = true;

			m_rtspLock.Lock();
			m_subVideoCurrCount = 0;
			m_bRtspSubNeedRestart = false;
			m_rtspSubID = m_rtspMajorID;
			m_bSecRequest = true;
			m_rtspLock.UnLock();
		}

		if( 0 == video3 )
		{
			m_rtspLock.Lock();
			m_rtspThirdID = -1;
			m_bRtspThirdNeedRestart = false;
			m_bThirdRequest  = false;
			m_rtspLock.UnLock();
		}
		else
		{
			bRequestThird = true;

			m_rtspLock.Lock();
			m_thirdVideoCurrCount = 0;
			m_bRtspThirdNeedRestart = false;
			m_rtspThirdID = m_rtspMajorID;
			m_bThirdRequest = true;
			m_rtspLock.UnLock();
		}
	}
	else
	{
		//当前IPC有三个码流
		if( m_bIpcHasThirdStream )
		{
			if( 0 == video2 )
			{
				if( subId > 0 )
				{
					StopVideoStream(subId,__LINE__);
				}
				m_rtspLock.Lock();
				m_rtspSubID = -1;
				m_bRtspSubNeedRestart = false;
				m_bSecRequest  = false;
				m_rtspLock.UnLock();
			}
			else
			{
				if(subId <= 0) 
				{
					bRequestSub = true;
					subStreamNum = GetSubStreamToken();

					if(subStreamNum > 0)//major stream is exist
					{
						//GetIpcVideoUri(T_MAJORSSTREAM,majorStreamNum);
						subId = RequestVideoStream(T_SUBSTREAM);
						if(subId > 0)
						{
						}
						else
						{
							printf( "current channel %d, requeststram sub error %s,%d\n",
								m_channelNum, __FILE__, __LINE__ );
						}
					}

					m_rtspLock.Lock();
					m_subVideoCurrCount = 0;
					m_bRtspSubNeedRestart = false;
					m_rtspSubID = subId;
					m_bSecRequest = true;
					m_rtspLock.UnLock();
				}
			}

			//控制第三个码流
			if( 0 == video3 )
			{
				if( thirdId > 0 )
				{
					StopVideoStream(thirdId,__LINE__);
				}
				m_rtspLock.Lock();
				m_rtspThirdID = -1;
				m_bRtspThirdNeedRestart = false;
				m_bThirdRequest  = false;
				m_rtspLock.UnLock();
			}
			else
			{
				if(thirdId <= 0) 
				{
					bRequestThird = true;
					thirdStreamNum = GetThirdStreamToken();

					if(thirdStreamNum > 0)//major stream is exist
					{
						//GetIpcVideoUri(T_MAJORSSTREAM,majorStreamNum);
						thirdId = RequestVideoStream(T_THIRDSTREAM);
						if(thirdId > 0)
						{
						}
						else
						{
							printf( "current channel %d, requeststram third error %s,%d\n",
								m_channelNum, __FILE__, __LINE__ );
						}
					}

					m_rtspLock.Lock();
					m_thirdVideoCurrCount = 0;
					m_bRtspThirdNeedRestart = false;
					m_rtspThirdID = thirdId;
					m_bThirdRequest = true;
					m_rtspLock.UnLock();
				}
			}

		}
		else
		{
			//当前IPC有两个码流

			if( 0 == video2 && 0 == video3 )
			{
				if( subId > 0 )
				{
					StopVideoStream(subId,__LINE__);
				}
				else if( thirdId > 0 )
				{
					StopVideoStream(thirdId,__LINE__);
				}

				m_rtspLock.Lock();
				m_rtspSubID = -1;
				m_bRtspSubNeedRestart = false;
				m_bSecRequest  = false;

				m_rtspThirdID = -1;
				m_bRtspThirdNeedRestart = false;
				m_bThirdRequest  = false;

				m_rtspLock.UnLock();
			}
			else 
			{
				int tmpStreamID = -1;

				if( subId <= 0 && thirdId <= 0 )
				{
					if( 1 == video2 )
					{
						bRequestSub = true;
					}

					if( 1 == video3 )
					{
						bRequestThird = true;
					}

					subStreamNum = GetSubStreamToken();
					if(subStreamNum > 0)//major stream is exist
					{
						//GetIpcVideoUri(T_MAJORSSTREAM,majorStreamNum);
						tmpStreamID = RequestVideoStream(T_SUBSTREAM);
						if(tmpStreamID > 0)
						{
						}
						else
						{
							printf( "current channel %d, requeststram sub and third error %s,%d\n",
								m_channelNum, __FILE__, __LINE__ );
						}
					}
				}
				else 
				{
					if( subId > 0 )
					{
						tmpStreamID = subId;
					}
					else if( thirdId > 0 )
					{
						tmpStreamID = thirdId;
					}
				}

				if( tmpStreamID > 0 )
				{
					if( 0 == video2 )
					{
						m_rtspLock.Lock();
						m_rtspSubID = -1;
						m_bRtspSubNeedRestart = false;
						m_bSecRequest  = false;

						m_bHasRequestSecondStream = false;
						m_rtspLock.UnLock();
					}
					else
					{
						m_rtspLock.Lock();
						m_subVideoCurrCount = 0;
						m_bRtspSubNeedRestart = false;
						m_rtspSubID = tmpStreamID;
						m_bSecRequest = true;

						m_bHasRequestSecondStream = true;

						m_rtspLock.UnLock();
					}

					if( 0 == video3 )
					{
						m_rtspLock.Lock();
						m_rtspThirdID = -1;
						m_bRtspThirdNeedRestart = false;
						m_bThirdRequest  = false;

						m_bHasRequestThirdStream = false;

						m_rtspLock.UnLock();

					}
					else
					{
						m_rtspLock.Lock();
						m_thirdVideoCurrCount = 0;
						m_bRtspThirdNeedRestart = false;
						m_rtspThirdID = tmpStreamID;
						m_bThirdRequest = true;

						m_bHasRequestThirdStream = true;

						m_rtspLock.UnLock();

					}
				}
				else
				{
					m_rtspLock.Lock();

					m_rtspSubID = -1;
					m_bRtspSubNeedRestart = false;
					m_bSecRequest  = false;

					m_rtspThirdID = -1;
					m_bRtspThirdNeedRestart = false;
					m_bThirdRequest  = false;

					m_rtspLock.UnLock();
				}
			}
		}

	}

    bool bDevDisconnect =  false;

	m_rtspLock.Lock();
	if((bRequestMajor && m_rtspMajorID < 0) 
		|| (bRequestSub && m_rtspSubID < 0) 
		|| (bRequestThird && m_rtspThirdID < 0))
	{
         bDevDisconnect = true;
	}
	m_rtspLock.UnLock();

	m_devStatusLock.Lock();
	if(bDevDisconnect)
	{
		m_deviceStatus  = ND_STATUS_DISCONNECT;
		m_ulOnvifLastError |= ONVIF_ERROR_REQUEST_STR;
	}
	else
	{
		m_reConnectCount++;
		m_deviceStatus  = ND_STATUS_OK;
	}
	m_devStatusLock.UnLock();

	return true;
 }

/************************************************************************
接口功能描述：请求视频关键帧
输入参数描述：
1、video1：主码流MASK。
2、video2：次码流MASK。
3、video3：手机码流MASK。
返回值描述：返回成功或者失败。
注：对应的位为零，表示停止。
************************************************************************/
bool COnvifNetDevice::RequestKeyFrame(ULONGLONG video1, ULONGLONG video2, ULONGLONG video3)
{
	return true;
}

/************************************************************************
接口功能描述：云台控制接口。
输入参数描述：
************************************************************************/
 bool COnvifNetDevice::PTZControl(nd_in long chnn,
						nd_in unsigned long cmdtype,
						nd_in unsigned long ptzParam,
						nd_in unsigned long speed,
						nd_in unsigned long waiteTime)
 {
	//OnvifPTZDevInfo *defaultPTZInfo= NULL;
	int majorNum = 0;

	if( !m_bOnvifCtrlEnable )
	{
		return false;
	}

#ifdef ONVIF_DEBUG
	printf("#####channel:%d,PTZControl",chnn);
#endif
     int result				= -1;
	 int num				= 0;
	 COnvifDevCtl DevPtzCtl; 
	 OnvifDevPTZNode DevPTZNodeResp;
     DevPTZSpaces PTZSpaces;

	 DevPTZNodeResp.HomeSupported = 0;
	 DevPTZNodeResp.MaximumNumberOfPresets = 0;

	 DevPTZNodeResp.SupportedPTZSpaces = &PTZSpaces;

	 DevPosition Position;
	 char *timeout = NULL; 
	 PTZMoveDirection MoveDirection;
	 char *Panaddr = NULL;
	 char *Zoomaddr = NULL;
	 static float PTZspeed = 0;
	 float speed_max = 0;
	 float speed_out = 0;
	 speed_max = 10;

	 char *PresetsToken = NULL;
	 char *PresetsName = NULL;
	 char buf[32] = {'\0'};

	 struct PresetsStruct PresetsResp;

	 char *PresetTourToken = NULL;
	 DevPTZPresetTourOperation PresetTourOperation;

     DevImgSetDate ImgSetDate;
     ImgIrisdata Irisdata;
	 ImgGetOptions imgGetOptions;
	 float Iris = 0;
	 static float m_Iris = 0;
	 ImgSetDate.boolean = _true_;
	 ImgSetDate.FocusMode = NULL;
	 ImgSetDate.IrisDate = NULL;
	 imgGetOptions.IrisMax = 0;
	 imgGetOptions.IrisMin = 0;
	 

	 if(!m_bDevicePtzCtlActive)
	 {
		 return false;
	 }

	 PTZMoveOptions MoveOptions;
	 float FocusSpeed = 0;
	 DevPtzChange PtzChange;
	 PtzChange = _add;

     memset(&MoveOptions,'\0',sizeof(MoveOptions));
	 memset(&PTZSpaces,'\0',sizeof(PTZSpaces));

	 memset(&PresetsResp,0,sizeof(struct PresetsStruct));
	 majorNum = GetMajorStreamToken();
	 if(majorNum <= 0)
	 {
		 return false;
	 }
	 if(!m_capaServ.bPTZEnable)
	 {
		 return false;
	 }
     OnvifPTZDevInfo defaultPTZInfo;
	 char proToken[64];
	 char vsourceToken[64];
      
	 memset(proToken,0,sizeof(proToken));
	 memset(vsourceToken,0,sizeof(vsourceToken));
     memset(&defaultPTZInfo,0,sizeof(OnvifPTZDevInfo));

	 //defaultPTZInfo = m_profileServ.profile[majorNum-1].ptzDefaultInfo;
	 lock_onvif_ctrl_op.Lock();
     strcpy(proToken,m_profileServ.profile[majorNum-1].profileToken);
	 strcpy(vsourceToken,m_profileServ.profile[majorNum-1].videoSourceToken);
	 if(m_profileServ.profile[majorNum-1].ptzDefaultInfo != NULL)
	 {
		 memcpy(&defaultPTZInfo,m_profileServ.profile[majorNum-1].ptzDefaultInfo,sizeof(OnvifPTZDevInfo));
	 }
	 lock_onvif_ctrl_op.UnLock();
	 

	 if(m_userName[0] == '\0' || m_passKey[0] == '\0')
	 {
		 return false;
	 }

	 //lock_onvif_ctrl_op.Lock();
	 if( m_bOnvifCtrlEnable ) 
	 {
		 DevPtzCtl.InitPTZServAddr(m_capaServ.PTZServ,m_userName,m_passKey);
		 DevPtzCtl.InitDevUtcTimes(false,m_strSaveServUri,m_iIpcCtrlUsedType);
		 DevPtzCtl.InitPTZProfileToken(proToken);
		 if(defaultPTZInfo.nodeToken[0] != '\0')
		 {
			 DevPtzCtl.GetNode(defaultPTZInfo.nodeToken,&DevPTZNodeResp,1);
		 }
		 PTZspeed = defaultPTZInfo.defaultXSpeed;//初始化速度
		 DevPtzCtl.SetPTZSpeed(speed,speed_max,DevPTZNodeResp.SupportedPTZSpaces->ContinuousPanTiltVelocitySpace[0][0],&speed_out);//速度转化
		 
		 PTZspeed = speed_out;
		 timeout = defaultPTZInfo.defaultTimeOut;
		 Panaddr = defaultPTZInfo.continusePTVSpace;
		 Zoomaddr = defaultPTZInfo.continuseZVSpace;
		 //DevPtzCtl.GetImagingIris(m_profileServ.profile[majorNum-1].videoSourceToken,&Iris,1);
		 //m_Iris = Iris;
		 switch(cmdtype)
		 {
			  case PTZ_CMD_LEFT:   
				  MoveDirection = Left;
				  DevPtzCtl.PTZMove(MoveDirection,timeout,Panaddr,Zoomaddr,PTZspeed);
				  break;
			  case PTZ_CMD_RIGHT:  
				  MoveDirection = Right;
				 DevPtzCtl.PTZMove(MoveDirection,timeout,Panaddr,Zoomaddr,PTZspeed);
				  break;
			  case PTZ_CMD_UP:     
				  MoveDirection = Up;
				  DevPtzCtl.PTZMove(MoveDirection,timeout,Panaddr,Zoomaddr,PTZspeed);
				  break;
			  case PTZ_CMD_DOWN:   
				  MoveDirection = Down;
				  DevPtzCtl.PTZMove(MoveDirection,timeout,Panaddr,Zoomaddr,PTZspeed);
				  break;
			  case PTZ_CMD_LEFT_UP:
				  MoveDirection = Left_Up;
				                   
				  DevPtzCtl.PTZMove(MoveDirection,timeout,Panaddr,Zoomaddr,PTZspeed);
				  break;
			  case PTZ_CMD_LEFT_DOWN:
				  MoveDirection = Left_Down;
				  DevPtzCtl.PTZMove(MoveDirection,timeout,Panaddr,Zoomaddr,PTZspeed); 
				  break;
			  case PTZ_CMD_RIGHT_UP:
				  MoveDirection = Right_Up;
				  DevPtzCtl.PTZMove(MoveDirection,timeout,Panaddr,Zoomaddr,PTZspeed);
				  break;
			  case PTZ_CMD_RIGHT_DOWN:
				  MoveDirection = Right_Down;
				  DevPtzCtl.PTZMove(MoveDirection,timeout,Panaddr,Zoomaddr,PTZspeed);
				  break;
			  case PTZ_CMD_ZOOM_OUT:
				  PtzChange = _add;
				  DevPtzCtl.PTZZoom(PtzChange,timeout,Panaddr,Zoomaddr,PTZspeed);							 
				  break;
			  case PTZ_CMD_ZOOM_IN: 
				  PtzChange = _decrease;
				  DevPtzCtl.PTZZoom(PtzChange,timeout,Panaddr,Zoomaddr,PTZspeed);
				  break;
			  case PTZ_CMD_PRESET_SET: 
				  //PresetsToken = "1";
				  //sprintf(buf,"%d",ptzParam +1);
				  //PresetsToken  = buf ;
				  //取消对这句话的注释     交换前两个参数的位置，
				  //DevPtzCtl.SetPreset(PresetsName,PresetsToken,1);
				  DevPtzCtl.SetPreset(ptzParam);
				  break;			                        
			  case PTZ_CMD_PRESET_GO:
				  {
				      DevPtzCtl.GetAllPresets(proToken,&PresetsResp,1);
				  /*if(defaultPTZInfo.limitPTSpace[0] != '\0')
				  {
					  Position.position_speed[0] = defaultPTZInfo.defaultXSpeed;
					  Position.position_speed[1] = defaultPTZInfo.defaultYSpeed;
					  Position.position_speed[2] = defaultPTZInfo.defaultZSpeed;
					  strncpy(Position.space[2],defaultPTZInfo.limitPTSpace,sizeof(Position.space[2]));
					  strncpy(Position.space[3],defaultPTZInfo.limitZSpace,sizeof(Position.space[3]));
					  sprintf(buf,"%d",ptzParam +1);
					  PresetsToken  = buf ;
					  DevPtzCtl.GotoPreset(PresetsToken,&Position,1);
				  }else{*/
					  Position.position_speed[0] = defaultPTZInfo.defaultXSpeed;
					  Position.position_speed[1] = defaultPTZInfo.defaultYSpeed;
					  Position.position_speed[2] = defaultPTZInfo.defaultZSpeed;
					  strncpy(Position.space[2],defaultPTZInfo.speedPTSpace,sizeof(Position.space[2]));
					  strncpy(Position.space[3],defaultPTZInfo.speedZSpace,sizeof(Position.space[3]));
					 
					  strcpy(buf,PresetsResp.presetToken[ptzParam+1]);
					  PresetsToken = buf;
					  DevPtzCtl.GotoPreset(PresetsToken,&Position,1);
				  }
				  break;
			  case PTZ_CMD_PRESET_DEL: 
				  PresetsToken = "2";
				  //取消对这句话的注释
				  DevPtzCtl.RemovePreset(PresetsToken,1);
				  break;
			  case PTZ_CMD_STOP :
				  DevPtzCtl.PtzMoveStop(1);
				  break;

			  case PTZ_CMD_IRIS_OPEN:
				  DevPtzCtl.InitImagingServAddr(m_capaServ.imagingServ,m_userName,m_passKey);
				  if(vsourceToken[0] != '\0')
				  {
					  DevPtzCtl.GetImagingIrisRange(vsourceToken,&imgGetOptions,1);

					  DevPtzCtl.GetImagingIris(vsourceToken,&Iris,1);
					 m_Iris = Iris;
					   m_Iris = m_Iris + 2;
					   if(m_Iris < imgGetOptions.IrisMin)
					   {
						   m_Iris = imgGetOptions.IrisMin;
					   }
					   ImgSetDate.boolean = _true_;
					   ImgSetDate.FocusMode = NULL;
					   ImgSetDate.IrisDate = &Irisdata;
					   ImgSetDate.IrisDate->iris = m_Iris;								   
					   ImgSetDate.IrisDate->Mode = ExposureMode__MANUAL;
					   DevPtzCtl.SetImagingSettings(vsourceToken,&ImgSetDate,1);
				  }
				  break;

			  case PTZ_CMD_IRIS_CLOSE:
				  DevPtzCtl.InitImagingServAddr(m_capaServ.imagingServ,m_userName,m_passKey);
				  if(vsourceToken[0] != '\0')
				  {
					  DevPtzCtl.GetImagingIrisRange(vsourceToken,&imgGetOptions,1);

					  DevPtzCtl.GetImagingIris(vsourceToken,&Iris,1);
					   m_Iris = Iris;
					   m_Iris = m_Iris - 2;
					   if(m_Iris < imgGetOptions.IrisMin)
					   {
						   m_Iris = imgGetOptions.IrisMin;
					   }
					   ImgSetDate.boolean = _true_;
					   ImgSetDate.FocusMode = NULL;
					   ImgSetDate.IrisDate = &Irisdata;
					   ImgSetDate.IrisDate->iris = m_Iris;								   
					   ImgSetDate.IrisDate->Mode = ExposureMode__MANUAL;
					   DevPtzCtl.SetImagingSettings(vsourceToken,&ImgSetDate,1);
				  }
				   //DevPtzCtl.PTZIris(m_profileServ.profile[majorNum-1].videoSourceToken,&Irisdata);
				   break;
			  case PTZ_CMD_CRUISE_SET:
				  break;
			  case PTZ_CMD_CRUISE_RUN: 
				  sprintf(buf,"%d",ptzParam+1);
				   PresetTourToken  = buf ;
					PresetTourOperation = start;
					DevPtzCtl.OperatePresetTour(PresetTourToken,&PresetTourOperation,1);
					break;
			  case PTZ_CMD_CRUISE_STOP:
				   sprintf(buf,"%d",ptzParam+1);
				   PresetTourToken  = buf ;
				   PresetTourOperation = stop;
				  DevPtzCtl.OperatePresetTour(PresetTourToken,&PresetTourOperation,1);
				   break;
			  case PTZ_CMD_CRUISE_DEL: 
				  sprintf(buf,"%d",ptzParam+1);
				  PresetTourToken  = buf;
				  DevPtzCtl.RemovePresetTour(PresetTourToken,1);
				  break;
			  case PTZ_CMD_FAR :
				  DevPtzCtl.InitImagingServAddr(m_capaServ.imagingServ,m_userName,m_passKey);
				  if(vsourceToken[0] != '\0')
				  {
					  DevPtzCtl.GetImagingIrisRange(vsourceToken,&imgGetOptions,1);

					  PtzChange = _add;
					   ImgSetDate.boolean = _true_;
					   Exposure_Mode FocusMode;
					   ImgSetDate.FocusMode = &FocusMode;
					  *(ImgSetDate.FocusMode) = ExposureMode__MANUAL;
					  ImgSetDate.IrisDate = NULL;
					   DevPtzCtl.SetImagingSettings(vsourceToken,&ImgSetDate,1);
					   DevPtzCtl.GetPTZFocusOptions(vsourceToken,&MoveOptions,1);
					  if(MoveOptions.Continuous[0] != 0)
					  {
						  FocusSpeed = MoveOptions.Continuous[0];
					  }
					  DevPtzCtl.PTZFocusMove(vsourceToken,PtzChange,&MoveOptions,FocusSpeed,1);
				  }
				  break;
			  case PTZ_CMD_NEAR :
				  DevPtzCtl.InitImagingServAddr(m_capaServ.imagingServ,m_userName,m_passKey);
				  if(vsourceToken[0] != '\0')
				  {
					  DevPtzCtl.GetImagingIrisRange(vsourceToken,&imgGetOptions,1);

					  PtzChange = _decrease;
					  ImgSetDate.boolean = _true_;
					  Exposure_Mode FocusMode1;
					  ImgSetDate.FocusMode = &FocusMode1;
					  *(ImgSetDate.FocusMode) = ExposureMode__MANUAL;
					  ImgSetDate.IrisDate = NULL;
					  DevPtzCtl.SetImagingSettings(vsourceToken,&ImgSetDate,1);
					  DevPtzCtl.GetPTZFocusOptions(vsourceToken,&MoveOptions,1);
					  if(MoveOptions.Continuous[0] != 0)
					  {
						  FocusSpeed = MoveOptions.Continuous[0];
					  }
					  DevPtzCtl.PTZFocusMove(vsourceToken,PtzChange,&MoveOptions,FocusSpeed,1);
				  }
				 break;
			  default:
				  break;
		 }
	 }

	 //lock_onvif_ctrl_op.UnLock();
   
	 return true;
 }


/************************************************************************
接口功能描述：获取图像色彩信息。
************************************************************************/
 bool COnvifNetDevice::ColorControl(nd_in long chnn,
						  nd_in unsigned long cmdtype,
						  nd_in_out DD_VIDEO_COLOR& videoColor)
 {
     int result				= -1;
	 int num				= 0;
     
	 int dvrMax				= 255;
	 float brightRate		= 0.0;
	 float satRate			= 0.0;
	 float contrastRate		= 0.0;
	 float sharpnessRate	= 0.0;
    
	 DD_VIDEO_COLOR			dvrColor;
	 OnvifDevColor			colorResp;
     COnvifDevCtl			DevColorCtrl;
	 NET_DEVICE_CTRL_COLOR  colorCtrl = ND_CTRL_COLOR_NULL;
	 char  vsourceToken[64];

	 if( !m_bOnvifCtrlEnable )
	 {
		 return false;
	 }

	 memset(vsourceToken,0,sizeof(vsourceToken));
#ifdef ONVIF_DEBUG
	 printf("######channel:%d,ColorControl!\n",chnn);
#endif
	 memset(&dvrColor, 1,sizeof(DD_VIDEO_COLOR));
	 memset(&colorResp,0,sizeof(struct OnvifDevColor));

	 if(!m_capaServ.bImagingEnable)
	 {
		 return false;
	 }
	 if(m_userName[0] == '\0' || m_passKey[0] == '\0')
	 {
		 return false;
	 }

	 
	 num	= GetMajorStreamToken();
	 if(num <= 0)
	 {
		 return false;
	 }
     
	 lock_onvif_ctrl_op.Lock();
	 strcpy(vsourceToken,m_profileServ.profile[num-1].videoSourceToken);
	 lock_onvif_ctrl_op.UnLock();

	 brightRate = m_imageParaRange.Brightness.max/dvrMax;
	 satRate   =  m_imageParaRange.ColorSaturation.max/dvrMax;
	 contrastRate = m_imageParaRange.Contrast.max/dvrMax;
	 sharpnessRate = m_imageParaRange.Sharpness.max/dvrMax;

	 DevColorCtrl.InitImagingServAddr(m_capaServ.imagingServ,m_userName,m_passKey);

	 colorCtrl = (enum NET_DEVICE_CTRL_COLOR)cmdtype;
   
	 switch(colorCtrl)
	 {
		 case ND_CTRL_COLOR_GET:
              //lock_onvif_ctrl_op.Lock();
			  if( m_bOnvifCtrlEnable )
			  {
				  DevColorCtrl.InitDevUtcTimes(false,m_strSaveServUri,m_iIpcCtrlUsedType);
				  result = DevColorCtrl.GetImagingSetting(vsourceToken,&colorResp,1);
				  if(result != OPERATE_SUCCESS)
				  {
					  result = DevColorCtrl.GetImagingSetting(vsourceToken,&colorResp,0);
				  }
			  }
			  else
			  {
				  result = OPERATE_FAILED;
			  }
			  //lock_onvif_ctrl_op.UnLock();

		      if(result== OPERATE_SUCCESS)
			  {
                dvrColor.startTime = 8;
				if(brightRate > 0.0)
				{
				  dvrColor.brightness = (char)colorResp.Brightness/brightRate;
				}

				if(contrastRate > 0.0)
				{
				  dvrColor.contrast   = (char)colorResp.Contrast/contrastRate;
				}

				if(sharpnessRate > 0.0)
				{
				  dvrColor.hue        = (char)colorResp.Sharpness/sharpnessRate;
				}
				if(satRate > 0.0)
				{
					dvrColor.saturation  = (char)colorResp.ColorSaturation/satRate;
				}
              videoColor.brightness = dvrColor.brightness;
			   videoColor.contrast   = dvrColor.contrast;
			   videoColor.hue        = dvrColor.hue;  
              videoColor.saturation  = dvrColor.saturation;
              videoColor.startTime   = dvrColor.startTime;
			  }
			  else
			  {
                return false;
			  }
			break;//获取
		 case ND_CTRL_COLOR_SAVE://设置并保存到配置
		 case ND_CTRL_COLOR_SET://设置[只针对图像]

			 if(videoColor.brightness == 0)
			 {
			   colorResp.Brightness = m_imageParaRange.Brightness.min;
			 }
			 else
			 {
			   colorResp.Brightness	 = (float)videoColor.brightness *brightRate;
			 }
              
			 if(videoColor.saturation == 0)
			 {
              colorResp.ColorSaturation  = m_imageParaRange.ColorSaturation.min;
			 }
			 else
			 {
			    colorResp.ColorSaturation  = (float)videoColor.saturation * satRate;
			 }
            
			 if(videoColor.contrast == 0)
			 {
				 colorResp.Contrast = m_imageParaRange.Contrast.min;
			 }
			 else
			 {
			     colorResp.Contrast	 = (float)videoColor.contrast * contrastRate;
			 }
            //if(videoColor.hue == 0)
			 {
               colorResp.Sharpness = m_imageParaRange.Sharpness.min;
			 }
			 //else
			 //{
			 //  colorResp.Sharpness	= (float)videoColor.hue * sharpnessRate;
			 //}

			 //lock_onvif_ctrl_op.Lock();
			 if( m_bOnvifCtrlEnable )
			 {
				 DevColorCtrl.InitDevUtcTimes(false,m_strSaveServUri,m_iIpcCtrlUsedType);
				 result = DevColorCtrl.SetImagingParameter(vsourceToken,true,&colorResp,1);
				 if(result != OPERATE_SUCCESS)
				 {
					 result = DevColorCtrl.SetImagingParameter(vsourceToken,true,&colorResp,0);
				 }

			 }
			 else
			 {
				 result = OPERATE_FAILED;
			 }
			 //lock_onvif_ctrl_op.UnLock();

			 if(result != OPERATE_SUCCESS)
			 {
				 return false;
			 }

			break;

		 case ND_CTRL_COLOR_GET_DEFAULT://获取默认

			  dvrColor.startTime = 8;
			  if(m_imageParaRange.Brightness.max > m_imageParaRange.Brightness.min)
			  {
					dvrColor.brightness = (char)(m_imageParaRange.Brightness.max + m_imageParaRange.Brightness.min)/(2*brightRate);
			  }
			  if(m_imageParaRange.Contrast.max > m_imageParaRange.Contrast.min)
			  {
					dvrColor.contrast   = (char)(m_imageParaRange.Contrast.max + m_imageParaRange.Contrast.min)/(2*contrastRate);
			  }

			  if(m_imageParaRange.Sharpness.max > m_imageParaRange.Sharpness.min)
			  {
					dvrColor.hue  = (char)(m_imageParaRange.Sharpness.max + m_imageParaRange.Sharpness.min)/(2*sharpnessRate);
			  }
             if(m_imageParaRange.ColorSaturation.max > m_imageParaRange.ColorSaturation.min)
			  {
				    dvrColor.saturation  = (char)(m_imageParaRange.ColorSaturation.max + m_imageParaRange.ColorSaturation.min)/(2*satRate);
			  }
			 videoColor.brightness = dvrColor.brightness;
			 videoColor.contrast   = dvrColor.contrast;
			 videoColor.hue        = dvrColor.hue;  
			 videoColor.saturation  = dvrColor.saturation;
			 videoColor.startTime   = dvrColor.startTime;
			  
			 break;
		 case ND_CTRL_COLOR_SET_DEFAULT://设置默认

			  colorResp.Brightness       = (float)(m_imageParaRange.Brightness.max + m_imageParaRange.Brightness.min)/2;
			  colorResp.Contrast         = (float)(m_imageParaRange.Contrast.max + m_imageParaRange.Contrast.min)/2;
			  colorResp.Sharpness        = (float)(m_imageParaRange.Sharpness.max + m_imageParaRange.Sharpness.min)/2;
			  colorResp.ColorSaturation   = (float)(m_imageParaRange.ColorSaturation.max + m_imageParaRange.ColorSaturation.min)/2;
             
			  //lock_onvif_ctrl_op.Lock();
			  if( m_bOnvifCtrlEnable )
			  {
				  DevColorCtrl.InitDevUtcTimes(false,m_strSaveServUri,m_iIpcCtrlUsedType);
				  result = DevColorCtrl.SetImagingParameter(vsourceToken,true,&colorResp,1);
				  if(result != OPERATE_SUCCESS)
				  {
					  result = DevColorCtrl.SetImagingParameter(vsourceToken,true,&colorResp,0);
				  }

			  }
			  else
			  {
				  result = OPERATE_FAILED;
			  }
			  //lock_onvif_ctrl_op.UnLock();
			  if(result != OPERATE_SUCCESS)
			  {
				 return false;
			  }
			 break;
		 default:
			 break;
	 }
	 return true;
 }
 bool COnvifNetDevice:: OtherControl(long chnn, unsigned long cmdtype, unsigned char *pChar)
{
	return true;
}


/************************************************************************
接口功能描述：设备的报警信息。
输出参数描述：
1、alarm：报警信息
返回值描述：无。
************************************************************************/
 ULONGLONG COnvifNetDevice::GetMotionAlarm()
 {
	 unsigned int alarm = 0;
	 
	 long channel = 32;

#ifdef ONVIF_VER40_ALARM_EVENT	 
	 channel = m_channelNum;
	 alarm	= COnvifEvent::EventInstance()->GetDevAlarm(channel,TEVENT_ALARM_MOTION);
#endif
	 return alarm > 0 ? 1 : 0 ;
 }

 ULONGLONG COnvifNetDevice::GetSensorAlarm()
 {
	 unsigned int alarm = 0;
	 long channel = 32;

#ifdef ONVIF_VER40_ALARM_EVENT	 
	 channel = m_channelNum;
	 alarm	= COnvifEvent::EventInstance()->GetDevAlarm(channel,TEVENT_ALARM_SENSOR);
#endif


	 return alarm > 0 ? 1 : 0 ;
 }

 ULONGLONG COnvifNetDevice::GetVlossAlarm()
 {
	 unsigned int alarm = 0;
	 long channel = 32;

#ifdef ONVIF_VER40_ALARM_EVENT	 
	 channel = m_channelNum;
	 alarm	= COnvifEvent::EventInstance()->GetDevAlarm(channel,TEVENT_ALARM_VIDEO_LOSS);
#endif

	 return alarm > 0 ? 1 : 0 ;
 }

 ULONGLONG COnvifNetDevice::GetOtherAlarm()
 {
	 unsigned int alarm = 0;
	 long channel = 32;

#ifdef ONVIF_VER40_ALARM_EVENT	 
	 channel = m_channelNum;
	 alarm	= COnvifEvent::EventInstance()->GetDevAlarm(channel,TEVENT_ALARM_OTHER);
#endif

	 return alarm > 0 ? 1 : 0 ;
 }


 int COnvifNetDevice::ChangeSecondStreamParam(unsigned long dataLen,unsigned const char *pData)
 {
	 int result = -1;
	 int  subStreamNum  = 0;
	 int setResult =-1;

	 subStreamNum   = GetSubStreamToken();
	 if(subStreamNum != 0)
	 {
		 {
			 memset(&m_encoderSubPara,0,sizeof(m_encoderSubPara));
			 GetIpcEncoderParameter(subStreamNum,&m_encoderSubPara);
		 }

		 result = -1;
		 result = VideoEncoderHandle(T_SUBSTREAM,&m_encoderSubPara,dataLen,pData);
		 if(result >= 0)
		 {
			 setResult = 0;
		 }
		 else
		 {
			 m_ulOnvifLastError |= ONVIF_ERROR_CFG_SUB_ENC;
		 }
	 }

	 return setResult;
 }


 int COnvifNetDevice::ChangeThirdStreamParam(unsigned long dataLen,unsigned const char *pData)
 {
	 int result = -1;
	 int  thirdStreamNum  = 0;
	 int setResult =-1;

	 //当前如果不是三码流的IPC
	 if( !m_bIpcHasThirdStream )
	 {
		 return 0;
	 }

	 thirdStreamNum   = GetThirdStreamToken();
	 if(thirdStreamNum != 0)
	 {
		 {
			 memset(&m_encoderThirdPara,0,sizeof(m_encoderThirdPara));
			 GetIpcEncoderParameter(thirdStreamNum,&m_encoderThirdPara);
		 }

		 result = -1;
		 result = VideoEncoderHandle(T_THIRDSTREAM,&m_encoderThirdPara,dataLen,pData);
		 if(result >= 0)
		 {
			 setResult = 0;
		 }
		 else
		 {
			 m_ulOnvifLastError |= ONVIF_ERROR_CFG_THIRD_ENC;
		 }
	 }

	 return setResult;
 }

/************************************************************************
接口功能描述 : 设置配置
1、
************************************************************************/
 long COnvifNetDevice::SetConfig( nd_in long chnn,
					   nd_in unsigned long itemID,
					   nd_in const unsigned char* pData,
					   nd_in unsigned long dataLen,
					   nd_in bool bImmediately)
 {
    enum dd_config_item_id confItemID  = DD_CONFIG_ITEM_SYSTEM_BASE;
    int result = -1;
    int setResult =-1;
	int tmpId1 = -1;
	int tmpId2 = -1;
	int tmpId3 = -1;
	int  majorStreamNum = 0;
	int  subStreamNum  = 0;

    struct _dd_sensor_config_ *pSensor =NULL;
	struct _dd_motion_config_        *pMotion=NULL;
    long  tmpChann = 0;

	if(dataLen == 0 || pData == NULL)
	{
		return -1;
	}

	if( !m_bOnvifCtrlEnable )
	{
		return -1;
	}

	confItemID = (enum dd_config_item_id)itemID;
	switch(confItemID)
	{
		case DD_CONFIG_ITEM_DEVICE_INFO: //DD_DEVICE_INFO

			break;
		case DD_CONFIG_ITEM_SYSTEM_BASIC:			//DD_BASIC_CONFIG
		   
			break;		
		case DD_CONFIG_ITEM_DATE_TIME:				//DD_DATE_TIME_CONFIG	      
#ifdef ONVIF_DEBUG
			printf("######channel:%d,NTP Set!\n",chnn);
#endif
			setResult = NtpDateTimeHandle(pData,dataLen);

			memcpy( &m_stSaveDatatimeCfg, pData, sizeof(DD_DATE_TIME_CONFIG) );
			m_stSaveDatatimeCfg.recv = 1;

			break;

		case DD_CONFIG_ITEM_DAYLIGHT_INFO:			//DD_DAYLIGHT_INFO 
#ifdef ONVIF_DEBUG
			printf("######channel:%d,Daylight Set!\n",chnn);
#endif		
          setResult = DaylightHandle(pData,dataLen);

			break;
		case DD_CONFIG_ITEM_LIVE_AUDIO:				//DD_LIVE_AUDIO_GROUP * AUDIO_INPUT_NUM
           
			break;
		case DD_CONFIG_ITEM_ENCODE_MASK_MAJOR:		//DD_ENCODE_CONFIG
		    break;
		case DD_CONFIG_ITEM_ENCODE_MASK_MINOR:		//DD_ENCODE_CONFIG
			 break;
		case DD_CONFIG_ITEM_ENCODE_SCHEDULE:		//DD_ENCODE_CONFIG * VIDEO_INPUT_NUM
#ifdef ONVIF_DEBUG
			printf("######channel:%d,major encoder Set!\n",chnn);
#endif

			{
				majorStreamNum = GetMajorStreamToken();
				memset(&m_encoderMajorPara,0,sizeof(OnvifVideoEncoder));
				GetIpcEncoderParameter(majorStreamNum,&m_encoderMajorPara);
			}

			result = -1;	
			result = VideoEncoderHandle(T_MAJORSSTREAM,&m_encoderMajorPara,dataLen,pData);
			if(result  >= 0 )
			{
				setResult = 0;
			}
			else
			{
				m_ulOnvifLastError |= ONVIF_ERROR_CFG_MAJOR_ENC;
			}

			// 初始化网络配置为最小配置, 只有一个主码流时候不需要配置
			if( result >= 0 && m_bFirstSetNetEncCfg && !m_bIpcOnlyMasterStream )
			{
				DD_ENCODE_CONFIG* pTmpEncCfg = new DD_ENCODE_CONFIG[m_channelNum+1];
				if( pTmpEncCfg )
				{
					DD_ENCODE_CONFIG* pItemCfg = pTmpEncCfg + m_channelNum;
					int itemslens = sizeof(DD_ENCODE_CONFIG)*(m_channelNum+1);
					
					pItemCfg->iSize = 0;
					pItemCfg->resolution = 2;
					pItemCfg->rate = 30;
					pItemCfg->encodeType = 2;
					pItemCfg->quality = 0;
					pItemCfg->minBitrate = 32;
					pItemCfg->maxBitrate = 384;


					if( !m_bIpcHasThirdStream )
					{
						//当前只有两个码流，子码流和第三码流是同一份
						result = ChangeSecondStreamParam( itemslens, (unsigned const char*)pTmpEncCfg );
					}
					else
					{
						//当前IPC有三码流，设置第三码流
						result = ChangeThirdStreamParam( itemslens, (unsigned const char*)pTmpEncCfg );
					}

					if( result >= 0 )
					{
						setResult = 0;
					}
					else
					{
						setResult = -1;
					}


					delete [] pTmpEncCfg;
				}
				m_bFirstSetNetEncCfg = false;
			}

			break;
		case DD_CONFIG_ITEM_ENCODE_ALARM:			//DD_ENCODE_CONFIG * VIDEO_INPUT_NUM		
		   break;

		case DD_CONFIG_ITEM_ENCODE_THIRD_STREAM:
			//配置有两个以上的码流的IPC
			if( !m_bIpcOnlyMasterStream )
			{
				if( !m_bIpcHasThirdStream )
				{
					//当前只有两个码流，子码流和第三码流是同一份
					setResult = ChangeSecondStreamParam( dataLen, pData );
				}
				else
				{
					//当前IPC有三码流，设置第三码流
					setResult = ChangeThirdStreamParam( dataLen, pData );
				}
			}
			break;

		case DD_CONFIG_ITEM_ENCODE_NETWORK:			//DD_ENCODE_CONFIG * VIDEO_INPUT_NUM
#ifdef ONVIF_DEBUG
			printf("######channel:%d,sub encoder Set!\n",chnn);
			printf("Before sub setting:width=%d,height= %d\n",m_encoderSubPara.resolution.width,m_encoderSubPara.resolution.height);
#endif
			//当期如果没有第三码流，则不能配置网络码流
			if( !m_bIpcHasThirdStream )
			{
				break;
			}

			//如果当前只有主码流，则不能配置网络码流
			if( m_bIpcOnlyMasterStream )
			{
				break;
			}

			setResult = ChangeSecondStreamParam( dataLen, pData );

			break;

		case DD_CONFIG_ITEM_NETWORK_IP:				//DD_NETWORK_IP_CONFIG
	#ifdef ONVIF_DEBUG
			printf("######Channel:%d,nvr ip address modify!\n",chnn);
	#endif

			 m_rtspLock.Lock();
			 tmpId1 = m_rtspMajorID;
			 tmpId2 = m_rtspSubID;
			 tmpId3 = m_rtspThirdID;
			 m_rtspLock.UnLock();

			 StopVideoStream(tmpId1,__LINE__);

			 m_rtspLock.Lock();
			 m_rtspMajorID = -1;
			 m_rtspLock.UnLock();

			 m_devStatusLock.Lock();
			 m_deviceStatus = ND_STATUS_DISCONNECT;
			 m_devStatusLock.UnLock();

			 //如果当前IPC只有一个主码流
			 if( m_bIpcOnlyMasterStream )
			 {
				 m_rtspLock.Lock();
				 m_rtspSubID = -1;
				 m_rtspThirdID = -1;
				 m_rtspLock.UnLock();
			 }
			 else
			 {
				 //当前IPC有三个码流
				 if( m_bIpcHasThirdStream )
				 {
					 StopVideoStream(tmpId2,__LINE__);
					 StopVideoStream(tmpId3,__LINE__);

					 m_rtspLock.Lock();
					 m_rtspSubID = -1;
					 m_rtspThirdID = -1;
					 m_rtspLock.UnLock();

				 }
				 else
				 {
					 //当期IPC只有两个码流
					 if( tmpId2 > 0 )
					 {
						 StopVideoStream(tmpId2,__LINE__);
					 }
					 else if( tmpId3 > 0 )
					 {
						 StopVideoStream(tmpId3,__LINE__);
					 }
					 m_rtspLock.Lock();
					 m_rtspSubID = -1;
					 m_rtspThirdID = -1;
					 m_rtspLock.UnLock();

				 }

			 }

#ifdef WIN32			
			 COnvifEvent::EventInstance()->SetListenAddr("192.168.3.53",50099);
#else
			 //get the alarm server address
			 //COnvifEvent::EventInstance()->SetListenAddrByManual("192.168.3.18",50099);
#endif

			setResult = 0;
			break;
		case DD_CONFIG_ITEM_NETWORK_ADVANCE:			//DD_NETWORK_ADVANCE_CONFIG
			break;
		case DD_CONFIG_ITEM_NETWORK_DDNS:			//DD_DDNS_CONFIG
			break;
		case DD_CONFIG_ITEM_DDNS_SERVER_INFO:		//DD_DDNS_SERVER_INFO
			break;
		case DD_CONFIG_ITEM_NETWORK_SMTP:			//DD_SMTP_CONFIG
			
			break;
		case DD_CONFIG_ITEM_SENSOR_SETUP:

			break;
		case DD_CONFIG_ITEM_SENSOR_SCHEDULE:
			break;
		case DD_CONFIG_ITEM_SENSOR_ALARM_OUT:
			break;
		case DD_CONFIG_ITEM_SENSOR_TO_RECORD:
#ifdef ONVIF_SNAP_ACTIVE
			SnapPictureHandle(GetSubStreamToken(),pData,dataLen,bImmediately,4);
#endif
			break;
		case DD_CONFIG_ITEM_SENSOR_TO_PTZ:
			break;


		case DD_CONFIG_ITEM_MOTION_SETUP:
#ifdef ONVIF_VER40_ALARM_EVENT
			majorStreamNum = GetMajorStreamToken();
			AlarmMotionHandle(majorStreamNum,pData,dataLen,bImmediately,m_manu_prod_id);
#endif
			break;
		case	DD_CONFIG_ITEM_MOTION_SCHEDULE:

			break;
		case DD_CONFIG_ITEM_MOTION_ALARM_OUT:

			break;
		case DD_CONFIG_ITEM_MOTION_TO_RECORD:
#ifdef ONVIF_SNAP_ACTIVE
			SnapPictureHandle(GetSubStreamToken(),pData,dataLen,bImmediately,4);
#endif
           break;

		case DD_CONFIG_ITEM_SHELTER_SETUP:

			break;
		case DD_CONFIG_ITEM_SHELTER_SCHEDULE:	
			break;
		case DD_CONFIG_ITEM_SHELTER_ALARM_OUT:
			break;
		case DD_CONFIG_ITEM_SHELTER_TO_RECORD:
			break;

		case DD_CONFIG_ITEM_VLOSS_SCHEDULE:
	       break; 
       case DD_CONFIG_ITEM_VLOSS_ALARM_OUT:	
		   break;
       case DD_CONFIG_ITEM_VLOSS_TO_RECORD:		
#ifdef ONVIF_SNAP_ACTIVE
		   SnapPictureHandle(GetSubStreamToken(),pData,dataLen,bImmediately,4);
#endif
		   break;
	   case DD_CONFIG_ITEM_ENCODE_JPEG:
#ifdef ONVIF_SNAP_ACTIVE
		   SnapPictureHandle(GetSubStreamToken(),pData,dataLen,bImmediately,SNAP_JPEG_CAPTRUE_USED_DEFAULT);
#endif
		   break;
		default :
			break;

	}
	if ((DD_CONFIG_ITEM_PTZ_CRUISE <= confItemID)&&(DD_CONFIG_ITEM_PTZ_CRUISE_END > confItemID))
	{
	    PtzInfoHanlde(pData,dataLen);
	}

	return setResult;
 }

 long COnvifNetDevice::FlushConfig()
 {
#ifdef ONVIF_DEBUG
	 printf("######FlushConfig!\n");
#endif
	 return 0;
 }

/************************************************************************
设置时间
************************************************************************/
 long COnvifNetDevice::SetDateTime(nd_in const DD_TIME& ddTm)
 {
    COnvifDevMgt devMgt;
	OnvifDateTime dateTimeSet;
	int result = -1;
    int tMajorId = -1;
	int  tSubId =-1;
	int hour= 0,minute= 0;
	bool behindUtc=false;

	if( !m_bOnvifCtrlEnable )
	{
		return -1;
	}

#ifdef ONVIF_DEBUG
	printf("***CNetDevice:SetDateTime!\n");
#endif

	//需要在NTP对时函数之后调用
    if(m_timeZone == DD_TIME_ZONE_NUM)
	{
#ifdef ONVIF_DEBUG
		printf("***the time zone is DD_TIME_ZONE_NUM!\n");
#endif
		return -1;
	}
    
    m_rtspLock.Lock();
	tMajorId =m_rtspMajorID;
	tSubId   = m_rtspSubID;
	m_rtspLock.UnLock();

    if(tMajorId > 0)
 	{
 		CClientRTPMain::Instance()->HandleSystemTimeChanged(tMajorId);
 	}
 	if(tSubId > 0)
 	{
 		CClientRTPMain::Instance()->HandleSystemTimeChanged(tSubId);
 	}
	if( m_rtspThirdID > 0 )
	{
		CClientRTPMain::Instance()->HandleSystemTimeChanged(m_rtspThirdID);
	}


	if((m_capaServ.bDevMgtEnable == true) 
		|| m_userName[0] != '\0' 
		|| m_passKey[0] != '\0')
	{

//		 if(m_bNtpSet)
//		 {
//			dateTimeSet.dateTimeType  = TSetNTP;
//
//#ifdef ONVIF_DEBUG
//			printf("***Device %s:dateTimeType is NTP!\n",m_capaServ.devMgtServ);
//#endif
//			return 0;
//		 }
//		 else
		 {
			dateTimeSet.dateTimeType  = TSetManual;	
#ifdef ONVIF_DEBUG
			printf("***Device %s:dateTimeType is Manual!\n",m_capaServ.devMgtServ);
			printf("***set time:%d:%d:%d,%d:%d:%d\n",ddTm.year,ddTm.month+1,ddTm.mday,ddTm.hour,ddTm.minute,ddTm.second);
#endif
		 }
        //m_bNtpSet = false;
        
		 memset(&dateTimeSet,0,sizeof(OnvifDateTime));

		 dateTimeSet.date.Year     = ddTm.year;
		 dateTimeSet.date.Month    = ddTm.month + 1;
		 dateTimeSet.date.Day      = ddTm.mday;
		 dateTimeSet.time.Hour     = ddTm.hour;
		 dateTimeSet.time.Minute   = ddTm.minute;
		 dateTimeSet.time.Second   = ddTm.second;
        
		 if( strlen( m_dstTab ) )
		 {
			 strcpy(dateTimeSet.DSTTab,m_dstTab);
		 }
		 GetTZTabName( dateTimeSet.TZTab, m_ipc_timezone_mode );

		 memset(dateTimeSet.timeZone,'\0',sizeof(dateTimeSet.timeZone));

        GetTimeZoneTime(m_timeZone,&hour,&minute,&behindUtc);
        dateTimeSet.deltaHour = hour;
		 dateTimeSet.deltaMinute = minute;
		 dateTimeSet.bBehindUtc  = behindUtc;

		 printf( "current SetDateTime channel %d, tzTab dstTab %s,%s-----%s,%d\n", m_channelNum, 
			 m_tzTab, m_dstTab, __FILE__, __LINE__ );
		 printf( "current SetDateTime channel %d, TimeZome %d,(%d,%d,%d)--%s,%d\n", m_channelNum,
			m_timeZone, hour, minute, behindUtc, __FILE__, __LINE__ );

		 if(m_bDst)
		 {
			dateTimeSet.bDaylightSave = true;

			strcpy(dateTimeSet.dstInfo,m_dstTz);
		 }
		 else
		 {
			dateTimeSet.bDaylightSave = false;
		 }

		 //lock_onvif_ctrl_op.Lock();
		 if( m_bOnvifCtrlEnable )
		 {
			 int ipcTimeZoneMode = m_ipc_timezone_mode;
			 devMgt.InitDevMgtParameter(m_capaServ.devMgtServ,m_userName,m_passKey);
			 devMgt.InitDevUtcTimes(false,m_strSaveServUri,m_iIpcCtrlUsedType);
			 result = devMgt.SetDevDateTime(&dateTimeSet,1, ipcTimeZoneMode );
			 if(result != OPERATE_SUCCESS)
			 {
				 result = devMgt.SetDevDateTime(&dateTimeSet,0, ipcTimeZoneMode);
			 }
		 }
		 else
		 {
			 result = OPERATE_FAILED;
		 }
		 //lock_onvif_ctrl_op.UnLock();

	     if(result == OPERATE_SUCCESS)
		 {
			 return 0;
		 }
		 else
		 {
			 printf("***Device %d,%s:set time failed!\n",m_channelNum,m_capaServ.devMgtServ);
			 return -1;
		 }
	}
	else
	{
		return -1;
	}

	return 0;
 }

/************************************************************************
获取设备支数信息
************************************************************************/
 long COnvifNetDevice::GetDevInfo(nd_in long videoIndex, nd_out ND_DEV_INFO& ndDevInfo)
 {
     
	 int i= 0;
	 int dvrResolutionNum = 0; 
	 OnvifH264ParameterRange *tmpRange=NULL;
     unsigned long	tmpVideoSize = 0;
	 long chnn = 0;
     
	 bool  bFindReso = false;

	 if( !m_bOnvifCtrlEnable )
	 {
		 return -1;
	 }

#ifdef ONVIF_DEBUG
	 printf("***GetDevInfo!\n");
#endif
	 m_channelLock.Lock();
	 chnn=m_channelNum;
	 m_channelLock.UnLock();


	 if(videoIndex == 0) //major video stream parameter
	 {
		 tmpRange = &m_majorParamRange;
	 }
	 else
	 {
		 if( !m_bIpcOnlyMasterStream )
		 {
			 if( m_bIpcHasThirdStream )
			 {
				 if(videoIndex == 1)//sub video stream parameter
				 {
					 tmpRange = &m_subParamRange;
				 }
				 else if( videoIndex == 2 )
				 {
					 tmpRange = &m_thirdParamRange;
				 }
			 }
			 else
			 {
				 if(videoIndex == 1)
				 {
					 tmpRange = &m_subParamRange;
				 }
				 else if(videoIndex == 2)
				 {
					 tmpRange = &m_subParamRange;
				 }
			 }
		 }
		 else
		 {
			 tmpRange = &m_majorParamRange;
		 }
	 }

	 if( 0 != videoIndex )
	 {
		 int maxCurFrm = 30;
		 for(i = 0; i < tmpRange->ipcReso.resoSize; i++ )
		 {
			 if( 0 == i )
			 {
				 maxCurFrm = tmpRange->ipcReso.resoContent[i].maxFrameRate;
			 }
			 else
			 {
				 if( maxCurFrm < tmpRange->ipcReso.resoContent[i].maxFrameRate )
				 {
					 maxCurFrm = tmpRange->ipcReso.resoContent[i].maxFrameRate;
				 }
			 }
			
		 }

		 if (30 != maxCurFrm)
		 {
			 maxCurFrm = 25;
		 }

		 //如果不是主码流获取信息，则固定设置为CIF大小
		 ndDevInfo.videoSize = 0x0002;
		 ndDevInfo.ucRate[2-1] = maxCurFrm;
		 ndDevInfo.maxBitRate[2-1] = 512;

		 return 0;
	 }


	 if( tmpRange->ipcReso.resoSize <= 0 
		 || tmpRange->ipcReso.resoSize > ONVIF_MAX_IPC_RESOLUTION_COUNT )
	 {
		 printf( "current channel ipcReso size %d--channel %d\n", 
			 tmpRange->ipcReso.resoSize, m_channelNum );
		 return -1;
	 }


	 lock_onvif_ctrl_op.Lock();
	 unsigned long ulImageSize = 0;
	 unsigned long ulImageTmp = 0;
	 bool bImageSizeDec = false;
	 bool bCheckImageSize = false;
	 if( tmpRange->ipcReso.resoSize > 2 )
	 {
			ulImageSize = tmpRange->ipcReso.resoContent[0].width*tmpRange->ipcReso.resoContent[0].height;
			ulImageTmp = tmpRange->ipcReso.resoContent[1].width*tmpRange->ipcReso.resoContent[1].height;
			if( ulImageSize >= ulImageTmp )
			{
				bImageSizeDec = true;
			}

			bCheckImageSize = true;
			ulImageSize = ulImageTmp;
	 }

	 ndDevInfo.chnn = chnn;
	 for(i = 0; i < tmpRange->ipcReso.resoSize; i++ )
	 {
		 dvrResolutionNum = CheckDvrResolution(tmpRange->ipcReso.resoContent[i].width,tmpRange->ipcReso.resoContent[i].height);
		 if( m_bIpcOnlyMasterStream )
		 {
				if( tmpRange->ipcReso.resoContent[i].width <= m_iIpcOnyMsMaxWidth 
					&& tmpRange->ipcReso.resoContent[i].height <= m_iIpcOnyMsMaxHeight 
					&& tmpRange->ipcReso.resoContent[i].width >= ONVIF_SECOND_STREAM_SIZE_MIN_WIDHT
					&& tmpRange->ipcReso.resoContent[i].height >= ONVIF_SECOND_STREAM_SIZE_MIN_HEIGHT )
				{
					//avild
				}
				else
				{
					dvrResolutionNum = 0;
				}
		 }

		 if( bCheckImageSize && i >= 2 )
		 {
			 ulImageTmp = tmpRange->ipcReso.resoContent[i].width*tmpRange->ipcReso.resoContent[i].height;
			 if( bImageSizeDec )
			 {
				 if( ulImageTmp > ulImageSize )
				 {
					 dvrResolutionNum = 0;
				 }
				 else
				 {
					 ulImageSize = ulImageTmp;
				 }
			 }
			 else
			 {
				 if( ulImageTmp < ulImageSize )
				 {
					 dvrResolutionNum = 0;
				 }
				 else
				 {
					 ulImageSize = ulImageTmp;
				 }
			 }
		 }

		 if(dvrResolutionNum >0)
		 {
			 tmpVideoSize = tmpVideoSize | (0x0001<<(dvrResolutionNum-1));
			 ndDevInfo.ucRate[dvrResolutionNum -1] = tmpRange->ipcReso.resoContent[i].maxFrameRate;
			 bFindReso = true;
		 }
	 }

     ndDevInfo.videoSize = tmpVideoSize;

     lock_onvif_ctrl_op.UnLock();
    
	int streamIndex = -1;
	struct OnvifVideoEncoder curEncoderPara;
    
	//no public resolution
	//只有主码流需要显示当前分辨率
	if( 0 == videoIndex )
	{
		memset(&curEncoderPara,0,sizeof(OnvifVideoEncoder));
		streamIndex = GetMajorStreamToken();
		if(streamIndex > 0)
		{
			GetIpcEncoderParameter(streamIndex,&curEncoderPara);
			dvrResolutionNum = CheckDvrResolution(curEncoderPara.resolution.width,curEncoderPara.resolution.height);
			if(dvrResolutionNum > 0 && (0 == (tmpVideoSize | (0x0001<<(dvrResolutionNum-1)))))
			{
				tmpVideoSize = tmpVideoSize | (0x0001<<(dvrResolutionNum-1));
				if( curEncoderPara.frameRate > 25 )
				{
					ndDevInfo.ucRate[dvrResolutionNum -1] = 30; //default the frame rate is 25
				}
				else
				{
					ndDevInfo.ucRate[dvrResolutionNum -1] = 25; //default the frame rate is 25
				}
				ndDevInfo.videoSize |= tmpVideoSize;
			}
		}
	}

	//最大码流
	for (int i=0; i<32; i++)
	{
		if ((ndDevInfo.videoSize >> i) & 0x01)
		{
			switch(0x01 << i)
			{
			case DD_VIDEO_SIZE_720P:
				ndDevInfo.maxBitRate[i] = 6*1024;
				break;
			case DD_VIDEO_SIZE_1080P:
				ndDevInfo.maxBitRate[i] = 8*1024;
				break;
			case DD_VIDEO_SIZE_960P:
				ndDevInfo.maxBitRate[i] = 6*1024;
				break;
			case DD_VIDEO_SIZE_SXGA:
				ndDevInfo.maxBitRate[i] = 6*1024;
				break;
			case DD_VIDEO_SIZE_3M:
				ndDevInfo.maxBitRate[i] = 12*1024;
				break;
			case DD_VIDEO_SIZE_16_9_3M:
				ndDevInfo.maxBitRate[i] = 12*1024;
				break;
			case DD_VIDEO_SIZE_4M:
				ndDevInfo.maxBitRate[i] = 12*1024;
				break;
			case DD_VIDEO_SIZE_5M:
				ndDevInfo.maxBitRate[i] = 12*1024;
				break;
			default:
				ndDevInfo.maxBitRate[i] = 2*1024;
			}
		}
	}

	//如果这个产品支持VGA又支持D1，只保留D1的分辨率
	if ((DD_VIDEO_SIZE_VGA & ndDevInfo.videoSize) && (DD_VIDEO_SIZE_D1 & ndDevInfo.videoSize))
	{
		ndDevInfo.videoSize = (ndDevInfo.videoSize & ~DD_VIDEO_SIZE_VGA);
	}

	return 0;
 }

long COnvifNetDevice::GetEncodeInfo(char * pVideoEncodeInfo, int iVideoEncodeInfoLen)
{
	return -1;
}

/************************************************************************
接口功能描述 : 错误处理 
1、正整型为常规错误, 负整数为严重错误
************************************************************************/
 bool COnvifNetDevice::GetLastError(nd_out std::string& strErr)
 {
    return true;
 }
 long COnvifNetDevice::GetLastError()
 {
	 return 0;
 }
 void COnvifNetDevice::SetLastError(nd_in long err)
 {
	 return;
 }

/************************************************************************
每秒钟调用一次，通知当前时间，模块可用此作为定时器
************************************************************************/
void COnvifNetDevice::OnTimer(unsigned long curTime)
{
	if( ++times_for_resync_time > 3*60 )
	{
		ResyncIpcTimes();
		times_for_resync_time = 0;
	}
}

void COnvifNetDevice::GetIpcEventInfo(unsigned long curTime)
{

}

 void COnvifNetDevice::CheckRtpConnectTimeout(unsigned long curTime)
 {
	 unsigned long  timerLostTime; 
	 bool  bUnlink = false;

	 bool  bMajorUnlink = false;
	 bool  bSubUnlink   = false;
	 bool  bThirdUnlink = false;

     int channel = 0;

	 m_channelLock.Lock();
	 channel = m_channelNum;
	 m_channelLock.UnLock();

	 if( 0 == m_saveCheckTimes || channel < 0 )
	 {
		 m_saveCheckTimes = curTime;

		 return;
	 }

	 m_devStatusLock.Lock();
	 if( ND_STATUS_OK != m_deviceStatus )
	 {
		 m_saveCheckTimes = curTime;

		 m_devStatusLock.UnLock();
		 return;
	 }
	 m_devStatusLock.UnLock();

	if( curTime > m_saveCheckTimes )
	{
		timerLostTime = curTime - m_saveCheckTimes;

		if( !m_bFastCheckVideoLoss )
		{
			if( timerLostTime < m_netTimeout )
			{
				return;
			}

			m_saveCheckTimes = curTime;
		}
		else
		{
			if( (curTime - m_iFastCheckPrevTime ) < m_fastCheckNetTimeNumber )
			{
				return;
			}

			//printf( "sh version %d, %d,%d\n", (curTime - m_iFastCheckPrevTime), m_iFastCheckMastrePrevCount, m_iFastCheckMasterCount );
			m_iFastCheckPrevTime = curTime;

			if( 0 != m_iFastCheckMasterCount || m_iFastCheckMastrePrevCount != 0 )
			{
				if( m_iFastCheckMastrePrevCount == m_iFastCheckMasterCount )
				{
					bMajorUnlink = true;
				}
			}
			m_iFastCheckMastrePrevCount = m_iFastCheckMasterCount;

			if( !bMajorUnlink )
			{
				if( timerLostTime < m_netTimeout )
				{
					return;
				}

				m_saveCheckTimes = curTime;
			}

		}
	}
	else
	{
		m_saveCheckTimes = curTime;

		return;
	}

    m_rtspLock.Lock();

     if(m_rtspMajorID >0 || m_bMajorRequest)
	 {
		 if( m_majorVideoCurrCount == m_majorVideoSaveCount )
		 {
			 m_iRtspMajorDiscnnCnt += 1;
			 if( m_iRtspMajorDiscnnCnt > 2 )
			 {
				 bMajorUnlink = true;
				 m_bRtspMajorNeedRestart = false;
				 m_iRtspMajorDiscnnCnt = 0;
			 }
			 else
			 {
				 m_bRtspMajorNeedRestart = true;
			 }

			 //printf("Disconnect![%d,%d] major0 interval time =%d ,m_bMajorReauest = %d, %d\n", 
			 //	 channel, m_iRtspMajorDiscnnCnt, m_netTimeout,m_bMajorRequest, m_deviceStatus);
		 }

		 m_majorVideoSaveCount = m_majorVideoCurrCount;
	 }

	 // 只有主码流正常，才检测子码流
	 if( !bMajorUnlink )
	 {
		 if(m_rtspSubID > 0 || m_bSecRequest)
		 {
			 if( m_subVideoCurrCount == m_subVideoSaveCount )
			 {
				 m_iRtspSubDiscnnCnt += 1;
				 if( m_iRtspSubDiscnnCnt > 5 )
				 {
					 bSubUnlink = true;
					 m_bRtspSubNeedRestart = false;
					 m_iRtspSubDiscnnCnt = 0;
				 }
				 else
				 {
					 m_bRtspSubNeedRestart = true;
				 }
				 //printf("Disconnect![%d,%d] sub stream.m_bSecRequest = %d,%d\n", 
				//	 channel,m_iRtspThirdDiscnnCnt, m_bSecRequest, m_deviceStatus);
			 }
			 m_subVideoSaveCount = m_subVideoCurrCount;
		 }

		 if(m_rtspThirdID > 0 || m_bThirdRequest) 
		 {
			 if( m_thirdVideoCurrCount == m_thirdVideoSaveCount )
			 {
				 m_iRtspThirdDiscnnCnt += 1;
				 if( m_iRtspThirdDiscnnCnt > 5 )
				 {
					 bThirdUnlink = true;
					 m_bRtspThirdNeedRestart = false;
					 m_iRtspThirdDiscnnCnt = 0;
				 }
				 else
				 {
					 m_bRtspThirdNeedRestart = true;
				 }
				 //printf("Disconnect![%d,%d] third stream. m_bThirdRequest = %d %d\n", 
				//	 channel,m_iRtspThirdDiscnnCnt,m_bThirdRequest, m_deviceStatus);
			 }
			 m_thirdVideoSaveCount = m_thirdVideoCurrCount;
		 }
	 }

	 m_rtspLock.UnLock();

	 m_devStatusLock.Lock();
	 if(bMajorUnlink || bSubUnlink || bThirdUnlink) 
	 {
		 m_deviceStatus = ND_STATUS_DISCONNECT;
		 printf("^^^^^^^GetTickCount() %d^^^^^^device status is Disconnect %d,%d,%d!\n",
			 m_channelNum, bMajorUnlink, bSubUnlink, bThirdUnlink);
	 }
	 else
	 {
		 m_deviceStatus = ND_STATUS_OK;
	 }
	 m_devStatusLock.UnLock();

	 return;
 }

 void COnvifNetDevice::SetNetTimeout(unsigned long msTimeout)
 {
	 if( 4000 == msTimeout )
	 {
		 //上海版本处理
		 m_bFastCheckVideoLoss = true;
		 m_fastCheckNetTimeNumber = 1000;
		 m_netTimeout = msTimeout;
	 }
	 else
	 {
		 m_bFastCheckVideoLoss = false;
		 m_fastCheckNetTimeNumber = msTimeout;
		 m_netTimeout = msTimeout;
	 }

 }

 unsigned long COnvifNetDevice::GetDevResolutionVal( int* width, int* height )
 {
	 *width = m_bMajorResolutionWidth;
	 *height = m_bMajorResolutionHeight;
	 return 0;
 }

int  COnvifNetDevice::GetRtpVideoData(RTSP_DATA_FRAME *pFrameData, int SeesionID, void *pParam)
{

	COnvifNetDevice		*pThis  = (COnvifNetDevice *)pParam;
  
	if(pThis == NULL)
	{
#ifdef ONVIF_DEBUG
		printf("$$$$$$GetRtpVideoData:pThis is NULL!\n");
#endif
		return -1;
	}

	return pThis->ProcessRTSPData(pFrameData,SeesionID);

}

int COnvifNetDevice::FuncProcCallback(RTSP_FUNC_PROC_CALLBACK_INFO* p_func_info, void *pParam)
{
	COnvifNetDevice		*pThis  = (COnvifNetDevice *)pParam;

	if(pThis == NULL)
	{
		return -1;
	}

	return pThis->FuncProcTypeCmd( p_func_info );
}

int COnvifNetDevice::GetIpcUsedInfo(RTSP_GET_IPC_USED_INFO* pIpcUsedInfo, void *pParam)
{
	COnvifNetDevice		*pThis  = (COnvifNetDevice *)pParam;

	if(pThis == NULL)
	{
		return -1;
	}

	return pThis->CurIpcInfoCmd(pIpcUsedInfo);
}

void COnvifNetDevice::GetRtspRestart(bool &bMajorNeed, bool &bSubNeed,bool &bThirdNeed, bool &bIpcHasThirdStram)
{
	m_devStatusLock.Lock();
	if( ND_STATUS_OK == m_deviceStatus )
	{
		bMajorNeed = m_bRtspMajorNeedRestart;
		bSubNeed = m_bRtspSubNeedRestart;
		bThirdNeed = m_bRtspThirdNeedRestart;

		bIpcHasThirdStram = m_bIpcHasThirdStream;
	}
	else
	{
		bMajorNeed = false;
		bSubNeed = false;
		bThirdNeed = false;

		bIpcHasThirdStram = false;
	}
	m_devStatusLock.UnLock();
}

void COnvifNetDevice::RestartRtsp(unsigned char aStreamType)
{
	int majorId = -1;
    int subId  = -1;
	int thirdId = -1;

	if( !m_bOnvifCtrlEnable )
	{
		return;
	}

	m_rtspLock.Lock();
	majorId = m_rtspMajorID;
	subId   = m_rtspSubID;
	thirdId = m_rtspThirdID;
	m_rtspLock.UnLock();

	//restart major stream
	if(aStreamType == 0 && m_bRtspMajorNeedRestart && majorId > 0  )
	{
		if(majorId > 0)
		{
			StopVideoStream(majorId,__LINE__);
		}

		int majorStreamNum = GetMajorStreamToken();
		GetIpcVideoUri(T_MAJORSSTREAM,majorStreamNum);
		majorId = RequestVideoStream(T_MAJORSSTREAM);

		m_rtspLock.Lock();
		m_rtspMajorID = majorId;
		m_majorVideoCurrCount = 0;
		m_bMajorRequest = true;

		m_bRtspMajorNeedRestart = false;

		//只有一个码流的时候，子次码流使用主码流一样的数据
		if( m_bIpcOnlyMasterStream )
		{
			m_rtspSubID = subId;
			m_subVideoCurrCount = 0;
			m_bSecRequest = true;
			m_bRtspSubNeedRestart = false;


			m_rtspThirdID = thirdId;
			m_thirdVideoCurrCount = 0;
			m_bThirdRequest = true;
			m_bRtspThirdNeedRestart = false;
		}

		m_rtspLock.UnLock();

		return;
	}

	//只有一个码流的时候，直接返回
	if( m_bIpcOnlyMasterStream )
	{
		return;
	}

	if( m_bIpcHasThirdStream )
	{
		if( 1 == aStreamType && m_bRtspSubNeedRestart && subId > 0 )
		{
			if(subId > 0)
			{
				StopVideoStream(subId,__LINE__);
			}

			int subStreamNum = GetSubStreamToken();
			GetIpcVideoUri(T_SUBSTREAM,subStreamNum);
			subId = RequestVideoStream(T_SUBSTREAM);

			m_rtspLock.Lock();
			m_rtspSubID = subId;
			m_subVideoCurrCount = 0;
			m_bSecRequest = true;

			m_bRtspSubNeedRestart = false;
			m_rtspLock.UnLock();

			return;
		}

		if( 2 == aStreamType && m_bRtspThirdNeedRestart && thirdId > 0 )
		{
			if(thirdId > 0)
			{
				StopVideoStream(thirdId,__LINE__);
			}

			int thirdStreamNum = GetThirdStreamToken();
			GetIpcVideoUri(T_THIRDSTREAM,thirdStreamNum);
			thirdId = RequestVideoStream(T_THIRDSTREAM);

			m_rtspLock.Lock();
			m_rtspThirdID = thirdId;
			m_thirdVideoCurrCount = 0;
			m_bThirdRequest = true;

			m_bRtspThirdNeedRestart = false;
			m_rtspLock.UnLock();

			return;
		}
	}
	else
	{
		int tmpStreamID = -1;

		if( 1 != aStreamType && 2 != aStreamType )
		{
			return;
		}

		if( !m_bRtspSubNeedRestart && !m_bRtspThirdNeedRestart )
		{
			return;
		}

		if( subId > 0 )
		{
			tmpStreamID = subId;
		}
		else if( thirdId > 0 )
		{
			tmpStreamID = thirdId;
		}

		if( tmpStreamID > 0 )
		{
			StopVideoStream( tmpStreamID,__LINE__ );
		}
		else
		{
			//直接退出
			return;
		}

		int subStreamNum = GetSubStreamToken();
		GetIpcVideoUri(T_SUBSTREAM,subStreamNum);
		tmpStreamID = RequestVideoStream(T_SUBSTREAM);

		if( m_bHasRequestSecondStream )
		{
			m_rtspLock.Lock();
			m_rtspSubID = tmpStreamID;
			m_subVideoCurrCount = 0;
			m_bSecRequest = true;
			m_rtspLock.UnLock();
		}

		if( m_bHasRequestThirdStream )
		{
			m_rtspLock.Lock();
			m_rtspThirdID = tmpStreamID;
			m_thirdVideoCurrCount = 0;
			m_bThirdRequest = true;
			m_rtspLock.UnLock();
		}

		m_rtspLock.Lock();
		m_bRtspSubNeedRestart = false;
		m_bRtspThirdNeedRestart = false;
		m_rtspLock.UnLock();

		return;
	}    

}

int COnvifNetDevice::FuncProcTypeCmd(RTSP_FUNC_PROC_CALLBACK_INFO* p_func_info)
{
	switch( p_func_info->func_type )
	{
	case RTSP_FUNC_PROC_TYPE_REF_COUNT:
		if( 0 == p_func_info->sub_channel )
		{
			m_iFastCheckMasterCount = p_func_info->ref_count;
		}
		break;

	default:
		break;
	}
	return 0;
}

int COnvifNetDevice::CurIpcInfoCmd(RTSP_GET_IPC_USED_INFO* pIpcUsedInfo)
{
	if( 0 == pIpcUsedInfo->cmd )
	{
		COnvifDevMgt devmgt;
		ONVIF_PUBLIC_UTC_TIME stUtcTm;

		memset( &stUtcTm, 0, sizeof(ONVIF_PUBLIC_UTC_TIME));
		//lock_onvif_ctrl_op.Lock();
		if( m_bOnvifCtrlEnable )
		{
			devmgt.InitDevUtcTimes(false, m_strSaveServUri, m_iIpcCtrlUsedType );
			devmgt.GetExistUtcTm( &stUtcTm );
		}
		//lock_onvif_ctrl_op.UnLock();

		pIpcUsedInfo->uYear = stUtcTm.uYear;
		pIpcUsedInfo->uMonth = stUtcTm.uMonth;
		pIpcUsedInfo->uDay = stUtcTm.uDay;
		pIpcUsedInfo->uHour = stUtcTm.uHour;
		pIpcUsedInfo->uMointue = stUtcTm.uMointue;
		pIpcUsedInfo->uSeconds = stUtcTm.uSeconds;
	}

	return 0;
}

 //private function
int COnvifNetDevice::ProcessRTSPData(RTSP_DATA_FRAME *pFrameData, int SeesionID)
{
	BITMAPINFOHEADER	*pBitmapInfo = NULL;
	WAVEFORMATEX       *pAudioInfo = NULL;
	CIPDataBufferMan	*pDataBufferMan = NULL;
    long   channel = 0;
    int    proResult = -1;
    int    mId = -1,sId = -1,tId = -1;
	int    snap_ID = SNAP_JPEG_STREAM_ID_VALUE;

	m_channelLock.Lock();
	channel = m_channelNum;
	m_channelLock.UnLock();

	if(pFrameData == NULL)
	{
		m_rtspLock.Lock();
		if(m_rtspMajorID == SeesionID)
		{
			//printf("!**! chnn:%d, major sessionID = %d\n", channel, m_rtspMajorID);
			m_majorVideoSaveCount = m_majorVideoCurrCount;
		}

		if(m_rtspSubID == SeesionID)
		{
			//printf("!**! chnn:%d, sub sessionID = %d\n", channel, m_rtspSubID);
			m_subVideoSaveCount = m_subVideoCurrCount;
		}

		//third stream video
		if(m_rtspThirdID == SeesionID)
		{
			//printf("!**! chnn:%d, third sessionID = %d\n", channel, m_rtspThirdID);
			m_thirdVideoSaveCount = m_thirdVideoCurrCount;
		}
		m_rtspLock.UnLock();

		return 0;
	}
    
	//m_devStatusLock.Lock();
	//m_deviceStatus = ND_STATUS_OK;
	//m_devStatusLock.UnLock();

	m_rtspLock.Lock();
	//m_currTime = GetTickCount();
	mId = m_rtspMajorID;
	sId = m_rtspSubID;
	tId = m_rtspThirdID;
	if(pFrameData->streamID == mId)
	{
		m_bMajorRequest  = false;
		if(m_pOnvifNetDev == NULL)
		{
			printf("******m_pOnvifNetDev == NULL!\n");
			m_rtspLock.UnLock();
			return 0;
		}

		m_majorVideoCurrCount++;
		m_iRtspMajorDiscnnCnt = 0;

		pBitmapInfo = &m_pOnvifNetDev->m_mainVideoFormat;
		pAudioInfo  = &m_pOnvifNetDev->m_mainAudioFormat;
		pDataBufferMan = m_pOnvifNetDev->m_pMainDataBufferMan;
		//process the video data
		if(pFrameData->frame.streamType == RTSP_STREAM_TYPE_VIDEO_1
			|| pFrameData->frame.streamType == RTSP_STREAM_TYPE_VIDEO_2
			|| pFrameData->frame.streamType == RTSP_STREAM_TYPE_VIDEO_3)
		{
			proResult =  ProcessVideoData(pFrameData,pBitmapInfo,pDataBufferMan,channel);
			if( 0 == proResult )
			{
				m_lTestFrameMasterCnt++;
				if( 0 == (m_lTestFrameMasterCnt & 0x3fff))
				{
					printf("current channel %d,send frame number %d,%d,%d\n", 
						m_channelNum, m_lTestFrameMasterCnt, m_lTestFrameSecondCnt, m_lTestFrameThirdCnt );
				} 
			}
		}


		//process audio data
		if(pFrameData->frame.streamType == RTSP_STREAM_TYPE_AUDIO)
		{
			if(pFrameData->streamID == mId)
			{
				proResult =  ProcessAudioData(pFrameData,pAudioInfo,pDataBufferMan,channel); 
			}
			else
			{
				proResult = 0;
			}

			//第三码流的音频与主码流的音频一样，所以这里直接使用主码流音频
			//复制一份数据到第三码流的内存管理器2014.12.6
			CIPDataBufferMan	*pDataBufferManThird = m_pOnvifNetDev->m_pThirdDataBufferMan;
			int proAudioResult =  ProcessAudioData(pFrameData,pAudioInfo,pDataBufferManThird,channel); 
			if( 0 != proAudioResult )
			{
				proResult = proAudioResult;
			}
		}
	}

	if(pFrameData->streamID == sId)
	{
		m_bSecRequest = false;
		if(m_pOnvifNetDev == NULL)
		{
			printf("******m_pOnvifNetDev == NULL!\n");
			m_rtspLock.UnLock();
			return 0;
		}

	    m_subVideoCurrCount++;
		m_iRtspSubDiscnnCnt = 0;

		pBitmapInfo = &m_pOnvifNetDev->m_secVideoFormat;
		//pAudioInfo  = &m_pOnvifNetDev->m_secAudioFormat;
		pDataBufferMan = m_pOnvifNetDev->m_pSecDataBufferMan;


		//process the video data
		if(pFrameData->frame.streamType == RTSP_STREAM_TYPE_VIDEO_1
			|| pFrameData->frame.streamType == RTSP_STREAM_TYPE_VIDEO_2
			|| pFrameData->frame.streamType == RTSP_STREAM_TYPE_VIDEO_3)
		{
			proResult =  ProcessVideoData(pFrameData,pBitmapInfo,pDataBufferMan,channel);
			if( 0 == proResult )
			{
				m_lTestFrameSecondCnt++;
			}
		}
	}

	if(pFrameData->streamID == tId)
	{
		m_bThirdRequest = false;
		if(m_pOnvifNetDev == NULL)
		{
			printf("******m_pOnvifNetDev == NULL!\n");
			m_rtspLock.UnLock();
			return 0;
		}

		m_thirdVideoCurrCount++;
		m_iRtspThirdDiscnnCnt = 0;

		pBitmapInfo = &m_pOnvifNetDev->m_thirdVideoFormat;
		pDataBufferMan = m_pOnvifNetDev->m_pThirdDataBufferMan;

		//process the video data
		if(pFrameData->frame.streamType == RTSP_STREAM_TYPE_VIDEO_1
			|| pFrameData->frame.streamType == RTSP_STREAM_TYPE_VIDEO_2
			|| pFrameData->frame.streamType == RTSP_STREAM_TYPE_VIDEO_3)
		{
			proResult =  ProcessVideoData(pFrameData,pBitmapInfo,pDataBufferMan,channel);
			if( 0 == proResult )
			{
				m_lTestFrameThirdCnt++;
			}
		}
	}

	if(pFrameData->streamID == snap_ID )
	{
		if( RTSP_STREAM_TYPE_PICTURE == pFrameData->frame.streamType)
		{
			pDataBufferMan = m_pOnvifNetDev->m_pSnapJpegBufferMan;
			proResult = ProcessSnapData(pFrameData,pDataBufferMan,channel);
		}
	}
	m_rtspLock.UnLock();

	return proResult;
}

/* copy from CCITT G.711 specifications */
unsigned char u2a[128] = {			/* u- to A-law conversions */
	1,	1,	2,	2,	3,	3,	4,	4,
	5,	5,	6,	6,	7,	7,	8,	8,
	9,	10,	11,	12,	13,	14,	15,	16,
	17,	18,	19,	20,	21,	22,	23,	24,
	25,	27,	29,	31,	33,	34,	35,	36,
	37,	38,	39,	40,	41,	42,	43,	44,
	46,	48,	49,	50,	51,	52,	53,	54,
	55,	56,	57,	58,	59,	60,	61,	62,
	64,	65,	66,	67,	68,	69,	70,	71,
	72,	73,	74,	75,	76,	77,	78,	79,
	/* corrected:
	81,	82,	83,	84,	85,	86,	87,	88, 
	should be: */
	80,	82,	83,	84,	85,	86,	87,	88,
	89,	90,	91,	92,	93,	94,	95,	96,
	97,	98,	99,	100,	101,	102,	103,	104,
	105,	106,	107,	108,	109,	110,	111,	112,
	113,	114,	115,	116,	117,	118,	119,	120,
	121,	122,	123,	124,	125,	126,	127,	128
};

/* u-law to A-law conversion */
static int ulaw2alaw (int	uval)
{
	uval &= 0xff;
	return ((uval & 0x80) ? (0xD5 ^ (u2a[0xFF ^ uval] - 1)) :
		  (0x55 ^ (u2a[0x7F ^ uval] - 1)));
}

int COnvifNetDevice::ProcessAudioData(RTSP_DATA_FRAME *aFrameData,WAVEFORMATEX	   *aAudioInfo,CIPDataBufferMan	*aDataBufferMan,long channelNum)
{
	bool				bPush   = false;
	int					result  = -1;
	int					virLen  = 0;
	int                cLen   = 0;
	DVR_IP_IN_FRM_INFO	dvrFrmData;
	unsigned char *tmpData=NULL;

	if(aFrameData == NULL || aAudioInfo == NULL || aDataBufferMan == NULL)
	{
		return -1;
	}

	if(!m_pAudioBuffer)
	{
		return -1;
	}

	//加入音频格式
	if(0 == aAudioInfo->nAvgBytesPerSec)
	{
		aAudioInfo->wFormatTag = WAVE_FORMAT_ALAW;
		aAudioInfo->nSamplesPerSec = 8000;
		aAudioInfo->nChannels = 1;
		aAudioInfo->wBitsPerSample = 8;
		aAudioInfo->nBlockAlign = 1;
		aAudioInfo->nAvgBytesPerSec = 8000;

		DVR_IP_IN_FRM_INFO frameInfo;
		memset(&frameInfo, 0, sizeof(DVR_IP_IN_FRM_INFO));
		frameInfo.channel = (int)channelNum;
		frameInfo.data_type = COM_FRAME_TYPE_AUDIO_FORMAT;
		frameInfo.len = sizeof(WAVEFORMATEX);
		frameInfo.vir_len = 0x1000 - sizeof(DVR_IP_IN_FRM_INFO);
		while(!aDataBufferMan->PushData((long)frameInfo.channel, frameInfo, (unsigned char *)aAudioInfo))
		{
			m_devStatusLock.Lock();
			if(m_deviceStatus != ND_STATUS_OK)
			{
				m_devStatusLock.UnLock();
				break;
			}
			m_devStatusLock.UnLock();

			PUB_Sleep(10);
		}
	}

    if(aFrameData->frame.encodeType == RTP_PAYLOAD_TYPE_G711_U)
	{
		tmpData = aFrameData->pData + sizeof(RTSP_DATA_FRAME); 
		for(int i = 0;i<aFrameData->frame.length;i++)
		{
			if(tmpData != NULL)
			{
              *tmpData = ulaw2alaw(*tmpData);
			}
			tmpData++;
		}
	}
    
	//m_quitLock.Lock();
	//audio data process
    unsigned int audioNum = 0;
	char *bufferInData = NULL;
	char *audioData = NULL;
	//int datalen = 0;
	int left = 0;
	int destlen = 0;
	int reSetAudioDataLen = aFrameData->frame.length;

	//如果声音的帧长度超出，则只保存有效长度。
	if(aFrameData->frame.length > ONVIF_AUDIO_AVILD_FRMS_LEN ) 
	{
		audioNum = ONVIF_AUDIO_MAX_FRM_COUNT;
		reSetAudioDataLen = ONVIF_AUDIO_MAX_FRM_COUNT*ONVIF_AUDIO_G711_FRM_LEN;
	}

	bufferInData = m_pAudioBuffer;
	audioData= (char *)(aFrameData->pData + sizeof(RTSP_DATA_FRAME));
    left = reSetAudioDataLen;

	while(left >= ONVIF_AUDIO_G711_FRM_LEN)
	{
		bufferInData[0] = 0;
		bufferInData[1] = 0x01;
		bufferInData[2] = 160;
		bufferInData[3] = 0;
       
		bufferInData += (ONVIF_AUDIO_G711_HEADER_FRM_LEN-ONVIF_AUDIO_G711_FRM_LEN);
		memcpy(bufferInData,audioData,ONVIF_AUDIO_G711_FRM_LEN*sizeof(char));
		audioData += ONVIF_AUDIO_G711_FRM_LEN;
		left -= ONVIF_AUDIO_G711_FRM_LEN;
		bufferInData += ONVIF_AUDIO_G711_FRM_LEN;
		destlen += ONVIF_AUDIO_G711_HEADER_FRM_LEN;
	}

	if (left > 0)
	{
		long len = 0;
		bufferInData[0] = 0;
		bufferInData[1] = 0x01;
		bufferInData[2] = left/2;
		bufferInData[3] = 0;
       
		bufferInData += (ONVIF_AUDIO_G711_HEADER_FRM_LEN-ONVIF_AUDIO_G711_FRM_LEN);
		memcpy(bufferInData,audioData,left*sizeof(char));
		destlen += (left + (ONVIF_AUDIO_G711_HEADER_FRM_LEN-ONVIF_AUDIO_G711_FRM_LEN));
	}


	cLen= (destlen + sizeof(DVR_IP_IN_FRM_INFO)) % 0x1000;
	virLen = destlen + ((cLen == 0) ? 0 : (0x1000 - cLen));

	if(aFrameData->length <= aFrameData->frame.length || aFrameData->pData == NULL)
	{
		return -1;
	}

	memset(&dvrFrmData,0,sizeof(struct _dvr_ip_in_frm_info));
	dvrFrmData.channel			= (int)channelNum;
	dvrFrmData.enc_stream_type	= WAVE_FORMAT_ALAW;
	dvrFrmData.data_type		= COM_FRAME_TYPE_AUDIO;
	dvrFrmData.keyframe			= (int)aFrameData->frame.bKeyFrame;
	dvrFrmData.len				= destlen;
	dvrFrmData.vir_len			= virLen; 
	dvrFrmData.cur_time			= (unsigned long long)aFrameData->frame.time.seconds*1000000 + (unsigned long long)aFrameData->frame.time.microsecond;
	dvrFrmData.relative_time	= dvrFrmData.cur_time;
    
	if(aDataBufferMan != NULL)
	{
		bPush = aDataBufferMan->PushData(channelNum,dvrFrmData,(const unsigned char *)m_pAudioBuffer);//aFrameData->pData + sizeof(RTSP_DATA_FRAME));
	}
   //m_quitLock.UnLock();

	if(bPush)
	{
		result = 0;
	}
	else
	{
#ifdef ONVIF_DEBUG
		printf("######channel:%d,lost audio frame!\n",channelNum);
#endif
		result = -1;
	}

    return result;
}

int COnvifNetDevice::ProcessVideoData(RTSP_DATA_FRAME *aFrameData,BITMAPINFOHEADER	*aBitmapInfo,CIPDataBufferMan	*aDataBufferMan,long channelNum)
{
	bool				bPush   = false;
	int					result  = -1;
	int					virLen  = 0;
	int                cLen   = 0;
	DVR_IP_IN_FRM_INFO	dvrFrmData;

	if(aFrameData == NULL || aBitmapInfo == NULL || aDataBufferMan == NULL)
	{
		printf( "current input param error %d, %s,%d\n", channelNum, __FILE__, __LINE__ );
		return -1;
	}

	//更新视频格式
	if((aBitmapInfo->biWidth != aFrameData->frame.width) 
		|| (aBitmapInfo->biHeight != aFrameData->frame.height))
	{

		aBitmapInfo->biWidth = aFrameData->frame.width;
		aBitmapInfo->biHeight = aFrameData->frame.height;

		//把视频格式放入缓冲列表
		DVR_IP_IN_FRM_INFO frameInfo;
		memset(&frameInfo, 0, sizeof(DVR_IP_IN_FRM_INFO));
		frameInfo.channel = (int)channelNum;
		frameInfo.width = aFrameData->frame.width;
		frameInfo.height = aFrameData->frame.height;
		frameInfo.data_type = COM_FRAME_TYPE_VIDEO_FORMAT;
		frameInfo.len = sizeof(BITMAPINFOHEADER);
		frameInfo.vir_len = 0x1000 - sizeof(DVR_IP_IN_FRM_INFO);

		while(!aDataBufferMan->PushData((long)frameInfo.channel, frameInfo, (unsigned char *)aBitmapInfo))
		{
			m_devStatusLock.Lock();
			if(m_deviceStatus != ND_STATUS_OK)
			{
			    m_devStatusLock.UnLock();
				break;
			}
			m_devStatusLock.UnLock();

			PUB_Sleep(10);
		}
	}

	cLen= (aFrameData->frame.length + sizeof(DVR_IP_IN_FRM_INFO)) % 0x1000;
	virLen = aFrameData->frame.length + ((cLen == 0) ? 0 : (0x1000 - cLen)); 

	if(aFrameData->length <= aFrameData->frame.length || aFrameData->pData == NULL)
	{
		printf( "current frame length error %d,%d,%x  %s,%d\n", 
			aFrameData->length, aFrameData->frame.length, (int)aFrameData->pData,
			__FILE__, __LINE__ );
		return -1;
	}
   
	memset(&dvrFrmData,0,sizeof(struct _dvr_ip_in_frm_info));
	dvrFrmData.channel			= (int)channelNum;
	dvrFrmData.enc_stream_type	= (int)aFrameData->frame.encodeType;

	dvrFrmData.data_type		= COM_FRAME_TYPE_VIDEO;

	dvrFrmData.width			= aFrameData->frame.width;
	dvrFrmData.height			= aFrameData->frame.height;
	dvrFrmData.keyframe			= (int)aFrameData->frame.bKeyFrame;
	dvrFrmData.len				= aFrameData->frame.length;
	dvrFrmData.vir_len			= virLen; 

	dvrFrmData.cur_time			= (unsigned long long)aFrameData->frame.time.seconds*1000000 + (unsigned long long)aFrameData->frame.time.microsecond;
	dvrFrmData.relative_time	= dvrFrmData.cur_time;


	if(aDataBufferMan != NULL)
	{

		if((m_pOnvifNetDev->bWaitKeyFrame && aFrameData->frame.bKeyFrame) //the key frame lost and this frame is key frame
			|| !m_pOnvifNetDev->bWaitKeyFrame)
		{
			bPush = aDataBufferMan->PushData(channelNum,dvrFrmData,aFrameData->pData + sizeof(RTSP_DATA_FRAME));
		}
		
	}

	if(bPush)
	{
		result = 0;
		m_pOnvifNetDev->bWaitKeyFrame = false;
	}
	else
	{
	   result =-1;
	   //printf("#####channel:%d,lost video frame,%s,%d!\n",channelNum, __FILE__, __LINE__ );
	   m_pOnvifNetDev->bWaitKeyFrame = true;		
	}

    return result;
}


int COnvifNetDevice::ProcessSnapData(RTSP_DATA_FRAME* aFrameData,CIPDataBufferMan	*aDataBufferMan,long channelNum)
{
	bool				bPush   = false;
	int					virLen  = 0;
	int                cLen   = 0;
	DVR_IP_IN_FRM_INFO	dvrFrmData;

	if(aFrameData == NULL)
	{
		return -1;
	}

	cLen= (aFrameData->frame.length + sizeof(DVR_IP_IN_FRM_INFO)) % 0x1000;
	virLen = aFrameData->frame.length + ((cLen == 0) ? 0 : (0x1000 - cLen)); 

	if(aFrameData->length <= aFrameData->frame.length || aFrameData->pData == NULL)
	{
		return -1;
	}

	memset(&dvrFrmData,0,sizeof(struct _dvr_ip_in_frm_info));
	dvrFrmData.channel			= (int)channelNum;
	dvrFrmData.enc_stream_type	= (int)aFrameData->frame.encodeType;

	dvrFrmData.data_type		= COM_FRAME_TYPE_JPEG;

	dvrFrmData.width			= aFrameData->frame.width;
	dvrFrmData.height			= aFrameData->frame.height;
	dvrFrmData.keyframe			= (int)aFrameData->frame.bKeyFrame;
	dvrFrmData.len				= aFrameData->frame.length;
	dvrFrmData.vir_len			= virLen; 

	dvrFrmData.cur_time			= (unsigned long long)aFrameData->frame.time.seconds*1000000 + (unsigned long long)aFrameData->frame.time.microsecond;
	dvrFrmData.relative_time	= dvrFrmData.cur_time;


	if(aDataBufferMan != NULL)
	{
		bPush = aDataBufferMan->PushData(channelNum,dvrFrmData,aFrameData->pData + sizeof(RTSP_DATA_FRAME));
	}

	return bPush ? 0 : -1;
}



void COnvifNetDevice::InitNetDevParamer(void)
{
	m_pOnvifNetDev = &stOnvifNetParamVal;
	if(m_pOnvifNetDev == NULL)
	{
		return ;
	}

	m_pOnvifNetDev->m_pMainDataBufferMan = NULL;
	m_pOnvifNetDev->m_pSecDataBufferMan  = NULL;
	m_pOnvifNetDev->m_pThirdDataBufferMan = NULL;
	m_pOnvifNetDev->m_pSnapJpegBufferMan = NULL;

	memset(&m_pOnvifNetDev->m_mainVideoFormat,0,sizeof(BITMAPINFOHEADER));
	memset(&m_pOnvifNetDev->m_secVideoFormat, 0,sizeof(BITMAPINFOHEADER));
	memset(&m_pOnvifNetDev->m_thirdVideoFormat,0,sizeof(BITMAPINFOHEADER));

	memset(&m_pOnvifNetDev->m_mainAudioFormat,0,sizeof(WAVEFORMATEX));
	//memset(&m_pOnvifNetDev->m_secAudioFormat,0,sizeof(WAVEFORMATEX));

	m_pOnvifNetDev->m_mainVideoFormat.biSize = sizeof(BITMAPINFOHEADER);
	m_pOnvifNetDev->m_mainVideoFormat.biCompression = mmioFOURCC1('H','2','6','4');
	m_pOnvifNetDev->m_mainVideoFormat.biPlanes = 1;

	m_pOnvifNetDev->m_secVideoFormat  = m_pOnvifNetDev->m_mainVideoFormat;
	m_pOnvifNetDev->m_thirdVideoFormat = m_pOnvifNetDev->m_mainVideoFormat;

	m_pOnvifNetDev->bWaitKeyFrame  = true; //wait for the key video frame

}


void COnvifNetDevice::QuitNetDev(void)
{
	//m_onvifLock.Lock();
	m_pOnvifNetDev->m_pMainDataBufferMan = NULL;
	m_pOnvifNetDev->m_pSecDataBufferMan = NULL;
	m_pOnvifNetDev->m_pThirdDataBufferMan = NULL;
	m_pOnvifNetDev->m_pSnapJpegBufferMan = NULL;

	m_pOnvifNetDev = NULL;
   // m_onvifLock.UnLock();
}

bool COnvifNetDevice::CheckStreamIsInThirdSize(OnvifH264ParameterRange* pH264ParamRange)
{
	OnvifIpcResolution *reso_Content = NULL;
	int i;
	int resuNum;
	bool bHasOnlyStreamSize = false;

	i= 0;
	resuNum = pH264ParamRange->ipcReso.resoSize;
	reso_Content = pH264ParamRange->ipcReso.resoContent;
	while( i < resuNum )
	{
		if( ONVIF_SECOND_STREAM_SIZE_MAX_WIDHT >= reso_Content[i].width )
		{
			if( ONVIF_SECOND_STREAM_SIZE_MAX_HEIGHT >= reso_Content[i].height )
			{
				bHasOnlyStreamSize = true;
				break;
			}
		}

		i++;
	}

	return bHasOnlyStreamSize;
}

bool COnvifNetDevice::CheckOnlyOneStreamMasterSize(OnvifH264ParameterRange* pH264ParamRange)
{
	OnvifIpcResolution *reso_Content = NULL;
	int i;
	int resuNum;
	bool bHasOnlyStreamSize = false;

	i= 0;
	resuNum = pH264ParamRange->ipcReso.resoSize;
	reso_Content = pH264ParamRange->ipcReso.resoContent;
	while( i < resuNum )
	{
		if( ONVIF_ONLY_ONE_STREAM_SIZE1_WIDTH == reso_Content[i].width )
		{
			if( ONVIF_ONLY_ONE_STREAM_SIZE1_HEIGH_PAL == reso_Content[i].height )
			{
				bHasOnlyStreamSize = true;
			}
			else if( ONVIF_ONLY_ONE_STREAM_SIZE1_HEIGH_NTSC == reso_Content[i].height )
			{
				bHasOnlyStreamSize = true;
			}
		}
		else if( ONVIF_ONLY_ONE_STREAM_SIZE2_WIDTH == reso_Content[i].width )
		{
			if( ONVIF_ONLY_ONE_STREAM_SIZE2_HEIGH == reso_Content[i].height )
			{
				bHasOnlyStreamSize = true;
			}
		}
		else if( m_iIpcOnyMsMaxWidth > ONVIF_SECOND_STREAM_SIZE_MAX_WIDHT )
		{
			if( ONVIF_ONLY_ONE_STREAM_SIZE3_WIDTH == reso_Content[i].width )
			{
				if( ONVIF_ONLY_ONE_STREAM_SIZE3_HEIGH == reso_Content[i].height )
				{
					bHasOnlyStreamSize = true;
				}
			}
			else if( ONVIF_ONLY_ONE_STREAM_SIZE4_WIDTH == reso_Content[i].width )
			{
				if( ONVIF_ONLY_ONE_STREAM_SIZE4_HEIGH == reso_Content[i].height )
				{
					bHasOnlyStreamSize = true;
				}
			}
		}

		i++;
	}

	return bHasOnlyStreamSize;
}

void COnvifNetDevice::InitIpcResolution(unsigned int streamNum,unsigned int streamType, bool &bHasLessD1)//OnvifH264ParameterRange *aParamRange)
{
	COnvifDevConf devConf;
	OnvifVideoEncoderConfOptions confOptions;
	int maxFrameRate =0,minFrameRate = 0;
	int result = -1,resuNum = 0;
	int i= 0;
	struct OnvifJpegOptions *tmpJpegSet =NULL;
    char token[64];
	char eToken[64];

	memset(&confOptions,0,sizeof(struct OnvifVideoEncoderConfOptions));
    memset(token,0,sizeof(token));
    memset(eToken,0,sizeof(eToken));

	if(streamNum <= 0)
	{
		return;
	}

	lock_onvif_ctrl_op.Lock();
	strcpy(token,m_profileServ.profile[streamNum-1].profileToken);
	strcpy(eToken,m_profileServ.profile[streamNum-1].videoEncoderConfToken);
	lock_onvif_ctrl_op.UnLock();

	devConf.InitDevMediaParameter(m_capaServ.mediaServ,m_userName,m_passKey);
	devConf.InitDevUtcTimes(false,m_strSaveServUri,m_iIpcCtrlUsedType);
	result =devConf.GetVideoEncoderRangeParameter(token,eToken,&confOptions,1);
	if(result != OPERATE_SUCCESS)
	{
		result =devConf.GetVideoEncoderRangeParameter(token,eToken,&confOptions,0);
	}

	if(result == OPERATE_SUCCESS)
	{
#ifdef ONVIF_DEBUG
		printf("***device %s:init ipc resolution success!\n",m_capaServ.devMgtServ);
#endif
		if(confOptions.bH264Enable)
		{
			maxFrameRate = confOptions.H264.JpegSetting.FrameRateRange.max;  
			minFrameRate = confOptions.H264.JpegSetting.FrameRateRange.min;

			tmpJpegSet = &confOptions.H264.JpegSetting;

			resuNum = tmpJpegSet->__sizeResolutionsAvailable;
			if(resuNum > 0)
			{
				OnvifIpcResolution *reso_Content = NULL;

				lock_onvif_ctrl_op.Lock();

				if(streamType == T_MAJORSSTREAM)
				{
					reso_Content = m_majorParamRange.ipcReso.resoContent;
					m_majorParamRange.ipcReso.resoSize = 0;
				}
				else if(streamType == T_SUBSTREAM)
				{
					reso_Content = m_subParamRange.ipcReso.resoContent;
					m_subParamRange.ipcReso.resoSize = 0;
				}
				else if(streamType == T_SUBSTREAM)
				{
					reso_Content = m_thirdParamRange.ipcReso.resoContent;
					m_thirdParamRange.ipcReso.resoSize = 0;
				}

				if(reso_Content)
				{			
					if( resuNum > ONVIF_MAX_IPC_RESOLUTION_COUNT )
					{
						resuNum = ONVIF_MAX_IPC_RESOLUTION_COUNT;
					}

					i= 0;
					while( i < resuNum )
					{
						reso_Content[i].height = tmpJpegSet->ResolutionsAvailable[i].height;
						reso_Content[i].width  = tmpJpegSet->ResolutionsAvailable[i].width;
						reso_Content[i].minFrameRate = minFrameRate;
						reso_Content[i].maxFrameRate = maxFrameRate;

						if( reso_Content[i].height <= m_iIpcOnyMsMaxHeight 
							&& reso_Content[i].width <= m_iIpcOnyMsMaxWidth 
							&& reso_Content[i].height >= ONVIF_SECOND_STREAM_SIZE_MIN_HEIGHT 
							&& reso_Content[i].width >= ONVIF_SECOND_STREAM_SIZE_MIN_WIDHT )
						{
							bHasLessD1 = true;
						}

						i++;
					}

					if(streamType == T_MAJORSSTREAM)
				    {
						m_majorParamRange.ipcReso.resoSize = resuNum;

						m_majorParamRange.encodeType.min = confOptions.H264.JpegSetting.EncodingIntervalRange.min;
						m_majorParamRange.encodeType.max = confOptions.H264.JpegSetting.EncodingIntervalRange.max;

						m_majorParamRange.qualityRange.min = confOptions.QualityRange.min;
						m_majorParamRange.qualityRange.max = confOptions.QualityRange.max;

						if(confOptions.bExtensionEnable == true)
						{
							m_majorParamRange.bitRateRange.min = confOptions.Extension.H264.JpegSetting.BitrateRange.min;
							m_majorParamRange.bitRateRange.max = confOptions.Extension.H264.JpegSetting.BitrateRange.max;

							m_majorParamRange.govLenRange.min  = confOptions.Extension.H264.GovLengthRange.min;
							m_majorParamRange.govLenRange.max  = confOptions.Extension.H264.GovLengthRange.max;
						} 
				    }
				    
				  if(streamType == T_SUBSTREAM)
				  {
					  m_subParamRange.ipcReso.resoSize = resuNum;

					  m_subParamRange.encodeType.min = confOptions.H264.JpegSetting.EncodingIntervalRange.min;
					  m_subParamRange.encodeType.max = confOptions.H264.JpegSetting.EncodingIntervalRange.max;

					  m_subParamRange.qualityRange.min = confOptions.QualityRange.min;
					  m_subParamRange.qualityRange.max = confOptions.QualityRange.max;

					  if(confOptions.bExtensionEnable == true)
					  {
						  m_subParamRange.bitRateRange.min = confOptions.Extension.H264.JpegSetting.BitrateRange.min;
						  m_subParamRange.bitRateRange.max = confOptions.Extension.H264.JpegSetting.BitrateRange.max;

						  m_subParamRange.govLenRange.min  = confOptions.Extension.H264.GovLengthRange.min;
						  m_subParamRange.govLenRange.max  = confOptions.Extension.H264.GovLengthRange.max;
					  } 
				  }


				  if(streamType == T_THIRDSTREAM)
				  {
					  m_thirdParamRange.ipcReso.resoSize = resuNum;

					  m_thirdParamRange.encodeType.min = confOptions.H264.JpegSetting.EncodingIntervalRange.min;
					  m_thirdParamRange.encodeType.max = confOptions.H264.JpegSetting.EncodingIntervalRange.max;

					  m_thirdParamRange.qualityRange.min = confOptions.QualityRange.min;
					  m_thirdParamRange.qualityRange.max = confOptions.QualityRange.max;

					  if(confOptions.bExtensionEnable == true)
					  {
						  m_thirdParamRange.bitRateRange.min = confOptions.Extension.H264.JpegSetting.BitrateRange.min;
						  m_thirdParamRange.bitRateRange.max = confOptions.Extension.H264.JpegSetting.BitrateRange.max;

						  m_thirdParamRange.govLenRange.min  = confOptions.Extension.H264.GovLengthRange.min;
						  m_thirdParamRange.govLenRange.max  = confOptions.Extension.H264.GovLengthRange.max;
					  } 
				  }
				}

				lock_onvif_ctrl_op.UnLock();

			}        
		}

	}
	tmpJpegSet = NULL;
}

void COnvifNetDevice::InitIpcColorRange(void)
{
	int result			= -1;
	int num				= 0;
	COnvifDevCtl			DevColorCtrl;
    char   sToken[64];

	memset(&m_imageParaRange,0,sizeof(struct OnvifDevColorRange));
    memset(sToken,0,sizeof(sToken));
	if(!m_capaServ.bImagingEnable)
	{
		return;
	}
	if(m_userName[0] == '\0' || m_passKey[0] == '\0')
	{
		return;
	}

	num	= GetMajorStreamToken();
	if(num <= 0)
	{
		return;
	}

	lock_onvif_ctrl_op.Lock();
    strcpy(sToken,m_profileServ.profile[num-1].videoSourceToken);
	lock_onvif_ctrl_op.UnLock();

	if( m_bOnvifCtrlEnable )
	{
		DevColorCtrl.InitImagingServAddr(m_capaServ.imagingServ,m_userName,m_passKey);
		DevColorCtrl.InitDevUtcTimes(false,m_strSaveServUri,m_iIpcCtrlUsedType);
		result = DevColorCtrl.GetImagingParameterRange(sToken,&m_imageParaRange,1);
		if(result != OPERATE_SUCCESS)
		{
			result = DevColorCtrl.GetImagingParameterRange(sToken,&m_imageParaRange,0);
		}
	}
	else
	{
		result = OPERATE_FAILED;
	}

	if(OPERATE_SUCCESS == result)
	{
#ifdef ONVIF_DEBUG
		printf("***Device %s:init color success!\n",m_capaServ.devMgtServ);
#endif
   		
	}
}

void    COnvifNetDevice::InitH264ParameterRange(OnvifH264ParameterRange *aRangeParam)
{
     if(aRangeParam == NULL)
	 {
		 return;
	 }
	 aRangeParam->encodeType.max = 1;
	 aRangeParam->encodeType.min = 0;
	 aRangeParam->qualityRange.max = 25;
	 aRangeParam->qualityRange.min = 0;

	 aRangeParam->govLenRange.min = 1;
	 aRangeParam->govLenRange.max = 10000;
	 aRangeParam->bitRateRange.min = 256;
	 aRangeParam->bitRateRange.max  = 10240;

	 aRangeParam->ipcReso.resoSize = 0;
	 aRangeParam->ipcReso.resoContent = new OnvifIpcResolution[ONVIF_MAX_IPC_RESOLUTION_COUNT];
	 if(aRangeParam->ipcReso.resoContent != NULL)
	 {
		 aRangeParam->ipcReso.resoContent[0].height = 720;
		 aRangeParam->ipcReso.resoContent[0].width  = 1280;
		 aRangeParam->ipcReso.resoContent[0].minFrameRate = 1;
		 aRangeParam->ipcReso.resoContent[0].maxFrameRate = 25;
	 }
}

void   COnvifNetDevice::GetIpcEncoderParameter(int aStreamNum, OnvifVideoEncoder *encoderParam)
{

	int width = 0,height = 0,devQua = 0,devEnType = 0;
	COnvifDevConf devConf;
    char proToken[64];

	int result = -1;
   if(aStreamNum <= 0 || encoderParam == NULL)
   {
	   return;
   }
   memset(proToken,0,sizeof(proToken));

    lock_onvif_ctrl_op.Lock();
    strcpy(proToken,m_profileServ.profile[aStreamNum-1].videoEncoderConfToken);
	lock_onvif_ctrl_op.UnLock();

	if(m_bOnvifCtrlEnable )
	{
		devConf.InitDevMediaParameter(m_capaServ.mediaServ,m_userName,m_passKey);
		devConf.InitDevUtcTimes(false, m_strSaveServUri, m_iIpcCtrlUsedType);
		result = devConf.GetVideoEncoder(proToken,encoderParam,1);
		if(result != OPERATE_SUCCESS)
		{
			result = devConf.GetVideoEncoder(proToken,encoderParam,0);
		}
	}
	else
	{
		result = OPERATE_FAILED;
	}

	if(result == OPERATE_SUCCESS)
	{	
		return;
	}
	else
	{
		encoderParam->encoderType = TPRO_END;
	}

}

void  COnvifNetDevice::GetIpcVideoUri(unsigned int aVideoType,unsigned int aProfileNum)
{
    enum EStreamType type = T_ENDSTREAM;
    OnvifVideoUri *tmpVideo =NULL;
	COnvifVideo videoSev;
    char tmpUri[256];
    char proToken[64];

	int result = -1;
    
	memset(tmpUri,0,sizeof(tmpUri));
    memset(proToken,0,sizeof(proToken));

    //printf("enter GetIpcVideoUri !\n");
	type = (enum EStreamType)aVideoType;
    
	lock_onvif_ctrl_op.Lock();
	strcpy(proToken,m_profileServ.profile[aProfileNum-1].profileToken);
	lock_onvif_ctrl_op.UnLock();

   // printf("before GetStreamUri !\n");
	if( m_bOnvifCtrlEnable )
	{
		videoSev.InitServParameter(m_capaServ.mediaServ,m_userName,m_passKey);	
		videoSev.InitDevUtcTimes(false,m_strSaveServUri,m_iIpcCtrlUsedType);
		result = videoSev.GetStreamUri(proToken,tmpUri,1);
		if(result != OPERATE_SUCCESS)
		{
			result = videoSev.GetStreamUri(proToken,tmpUri,0);
		}
	}
	else
	{
		result = OPERATE_FAILED;
	}
   //printf("after GetStreamUri !\n");

	if(type == T_MAJORSSTREAM)
	{
		tmpVideo = &m_majorVideo;
	}
	else if(type == T_SUBSTREAM)
	{
		tmpVideo = &m_subVideo;
	}
	else if( type == T_THIRDSTREAM )
	{
		tmpVideo = &m_thirdVideoUri;
	}
	
	if(result == OPERATE_SUCCESS
		&& strlen(tmpUri) < 255 )
	{
#ifdef ONVIF_DEBUG
		printf("***before RTSP service Uri:%s!\n",tmpUri);
#endif
		strcpy(tmpVideo->Uri,tmpUri);
		tmpVideo->bVideo = true;
		UpdateRtspIPAddr(tmpVideo->Uri,sizeof(tmpVideo->Uri), m_serverAddr);

#ifdef ONVIF_DEBUG
		printf("***after RTSP service Uri:%s!\n",tmpVideo->Uri);
#endif
	}
	else
	{
		tmpVideo->bVideo = false;

#ifdef ONVIF_DEBUG
		printf("***Get RTSP service Uri failed!\n");
#endif
	}
	tmpVideo = NULL;


    //printf("exit GetStreamUri !\n");

	return;
}

int COnvifNetDevice::RequestVideoStream(unsigned int aVideoType)
{
   enum EStreamType tmpType = T_ENDSTREAM;
   int rtpID = -1;
   char uri[256];
   bool bAudioEnable = false;
   unsigned long* pErrCallBack = NULL;

   memset(uri,0,sizeof(uri));

   printf("enter RequestVideoStream %d,%d !\n", m_channelNum, aVideoType );

   int ret_val = 0;

   lock_onvif_ctrl_op.Lock();

   tmpType = (enum EStreamType)aVideoType;
   if(tmpType == T_MAJORSSTREAM)
   {
	   if(m_majorVideo.bVideo == false)
	   {
		   printf("***Device %s:m_majorVideo.bVideo %d== false!\n",m_capaServ.devMgtServ, m_channelNum);
		   ret_val = -1;
	   }
	   else
		   strcpy(uri,m_majorVideo.Uri);

	   bAudioEnable = true;
	   pErrCallBack = &m_ulMajorLastError;
   }
   else if(tmpType == T_SUBSTREAM)
   {
	   if(!m_bIpcHasThirdStream)
	   {
		   if( !CheckStreamIsInThirdSize( &m_subParamRange ) )
		   {
			   printf("***Device %s:m_subParamRange.size d1 == %d false!\n",m_capaServ.devMgtServ, m_channelNum);
			   ret_val = -1;
		   }
	   }

	   if(m_subVideo.bVideo == false)
	   {
		   printf("***Device %s:m_subVideo.bVideo == %d false!\n",m_capaServ.devMgtServ, m_channelNum);
		   ret_val = -1;
	   }
	   else
		   strcpy(uri,m_subVideo.Uri);

	   bAudioEnable = false;
	   pErrCallBack = &m_ulSubLastError;
   }
   else if( tmpType == T_THIRDSTREAM )
   {
	   if( !CheckStreamIsInThirdSize( &m_thirdParamRange ) )
	   {
		   printf("***Device %s:m_thirdParamRange.size d1 == %d false!\n",m_capaServ.devMgtServ, m_channelNum);
		   ret_val = -1;
	   }	   
	   
	   if( m_thirdVideoUri.bVideo == false )
	   {
		   ret_val = -1;
	   }
	   else
	   {
		   strcpy( uri, m_thirdVideoUri.Uri );
	   }
	   bAudioEnable = false;
	   pErrCallBack = &m_ulThirdLastError;
   }
   lock_onvif_ctrl_op.UnLock();

   if( 0 != ret_val )
   {
	   return -1;
   }

   //printf("after lock in RequestVideoStream %d,%d!\n", m_channelNum, aVideoType );

   rtpID = CClientRTPMain::Instance()->StartASession(uri,m_userName,m_passKey,GetRtpVideoData,this, GetIpcUsedInfo,
	   m_channelNum, tmpType, bAudioEnable, pErrCallBack, FuncProcCallback );
   //printf("exit RequestVideoStream !\n");
   
   if(rtpID <= 0)
   {
	   //printf( "start session error %d rtpID = %d\n", m_channelNum, rtpID );
	   if( -3 == rtpID )
	   {
		   m_ulOnvifLastError |= ONVIF_ERROR_RTP_SOCK_CREATE;
	   }
	   return -1;
   }

   return rtpID;
}


bool  COnvifNetDevice::StopVideoStream(int aSeesionID, int lines )
{
  
	printf( "stop vidoe stream chanel %d, %d--lines %d\n", m_channelNum, aSeesionID, lines );
	if(CClientRTPMain::Instance()->StopASession(aSeesionID))
	{
       return true;
	}
	else
	{
		return false;
	}
   	
}

int COnvifNetDevice::CheckH264Profile(OnvifMediaProfile *mediaProfile)
{
	int num = 0; 
	if(mediaProfile == NULL)
	{
		return 0;
	}
	for(int i= 0;i<mediaProfile->profileSize;i++)
	{
		if(mediaProfile->profile->profileType == TPRO_H264)
		{
			num++;
		}
	}
	return num;
}

int COnvifNetDevice::GetMajorStreamToken(void)
{
	unsigned int num =0;
	int i = 0;
	bool bH264Find= false;
	struct OnvifMediaProData *tmpProfile = NULL;

	lock_onvif_ctrl_op.Lock();
	tmpProfile = m_profileServ.profile;
	while(i < m_profileServ.profileSize)
	{
		num++;
		if((tmpProfile->profileType == TPRO_H264))// && (tmpProfile->bFixed == true))
		{
			bH264Find = true;
			break;
		}
		i++;
		tmpProfile++;
	}
    lock_onvif_ctrl_op.UnLock();

	tmpProfile= NULL;
	if(bH264Find)
	{
		return num;
	}

	return 0;
}

int COnvifNetDevice::GetSubStreamToken(void)
{
	unsigned int num =0;
	unsigned int h264Num = 0;
	int i = 0;
	bool bH264Find= false;
	struct OnvifMediaProData *tmpProfile = NULL;

	lock_onvif_ctrl_op.Lock();
	tmpProfile = m_profileServ.profile;
	while(i < m_profileServ.profileSize)
	{
		num++;
		if((tmpProfile->profileType == TPRO_H264))
		{
			h264Num ++;
			if(h264Num >1)
			{
				bH264Find = true;			    
				break;
			}
		}
		i++;
		tmpProfile++;
	}
    lock_onvif_ctrl_op.UnLock();

	tmpProfile= NULL;
	if(bH264Find)
	{
		return num;
	}
	return 0;
}

int COnvifNetDevice::GetThirdStreamToken(void)
{
	unsigned int num =0;
	unsigned int h264Num = 0;
	int i = 0;
	bool bH264Find= false;
	struct OnvifMediaProData *tmpProfile = NULL;

	lock_onvif_ctrl_op.Lock();
	tmpProfile = m_profileServ.profile;
	while(i < m_profileServ.profileSize)
	{
		num++;
		if((tmpProfile->profileType == TPRO_H264))
		{
			h264Num ++;
			if(h264Num >2)
			{
				bH264Find = true;			    
				break;
			}
		}
		i++;
		tmpProfile++;
	}
	lock_onvif_ctrl_op.UnLock();

	tmpProfile= NULL;
	if(bH264Find)
	{
		return num;
	}
	return 0;
}


int COnvifNetDevice::CheckDvrResolution(int width,int height)
{
	if(((width == 176)&&(height == 144))
		|| ((width == 176)&&(height == 120)))
	{
		if(144 == height)
		{
			m_bPal = true;
		}
		return 1; //DD_VIDEO_SIZE_QCIF;
	}

	if(((width == 352)&&(height == 288))
		|| ((width == 352)&&(height == 240)))
	{
		if(288 == height)
		{
#ifdef ONVIF_DEBUG
			printf("352 and 288!\n");
#endif
			m_bPal = true;
		}

		return 2; //DD_VIDEO_SIZE_CIF;
	}

	if(((width == 704)&&(height == 288))
		|| ((width == 704)&&(height == 240)))
	{
		if(288 == height)
		{
			m_bPal = true;
		}

		return 3; //DD_VIDEO_SIZE_HD1;
	}

	if(((width == 704)&&(height == 576)) \
		|| ((width == 704)&&(height == 480)) \
		|| ((width == 720)&&(height == 576)) \
		|| ((width == 720)&&(height == 480)))
	{
		if(576 == height)
		{
#ifdef ONVIF_DEBUG
			printf("704 and 576!\n");
#endif
			m_bPal = true;
		}

		return 4; //DD_VIDEO_SIZE_D1;
	}

	if((width == 320)&&(height == 240))
	{
		return 5; //DD_VIDEO_SIZE_QVGA;
	}

	if((width == 640)&&(height == 480))
	{
		return 6; //DD_VIDEO_SIZE_VGA;
	}

	//if((width == 1024)&&(height == 768))
	//{
	//	return 7; //DD_VIDEO_SIZE_XVGA;
	//}

	if((width == 160)&&(height == 120))
	{
		return 8; //DD_VIDEO_SIZE_QQVGA;
	}

	if((width == 960)&&(height == 480))
	{
		return 9; //DD_VIDEO_SIZE_960H;
	}
	
	if((width == 1280)&&(height == 720))
	{
		return 10; //DD_VIDEO_SIZE_720P;
	}
	else if((width == 1280)&&(height == 960))
	{
		return 14; //DD_VIDEO_SIZE_960P;
	}
	else if((width >1200 && width < 1300) && (height >700 && height < 1100))
	{
		return 10; //DD_VIDEO_SIZE_720P;
	}
	//modify 2013.11.25,when the value near the 720P,return the 720P
	//if((width == 1216 && height == 768)
	//	|| (width == 1216 && height == 1024))
	//{
	//	return 10;
	//}
 
	if((width == 1920)&&(height == 1080))
	{
		return 11; //DD_VIDEO_SIZE_1080P;
	}

 //   //modify 2013.11.25,when the value near the 1080p,return the 1080p
	//if((width > 1900 && width < 2000) && (height >1000 && height <1100))
	//{
	//	return 11;
	//}

	//if((width == 800)&&(height == 600))
	//{
	//	return 12; //DD_VIDEO_SIZE_SVGA;
	//}

	//if((width == 1280)&&(height == 1024))
	//{
	//	return 13; //DD_VIDEO_SIZE_SXGA;
	//}

	if((width == 1280)&&(height == 960))
	{
		return 14; //DD_VIDEO_SIZE_960P;
	}

	//if((width == 1600)&&(height == 1200))
	//{
	//	return 15;
	//}

	if((width == 2048)&&(height == 1536))
	{
		return 16; //DD_VIDEO_SIZE_3M;
	}

	if((width == 2304)&&(height == 1296))
	{
		return 15; //DD_VIDEO_SIZE_16_9_3M;
	}

	if((width == 2688)&&(height == 1520))
	{
		return 12; //DD_VIDEO_SIZE_4M;
	}

	if((width == 2592)&&(height == 1944))
	{
		return 7; //DD_VIDEO_SIZE_5M;
	}

	//printf("\n\n#################width:%d, height:%d\n\n", width, height);

	return 0;
}

void COnvifNetDevice::GetDvrResolution(unsigned short devResolution,int *width,int *height)
{
	DD_VIDEO_SIZE resoType;
	resoType = (DD_VIDEO_SIZE)devResolution;
	switch(resoType)
	{
	case DD_VIDEO_SIZE_QCIF:	//QCIF(176*144(PAL)/176*120(N))
		*width = 176;
		*height = m_bPal ? 144 : 120;
		break;
	case DD_VIDEO_SIZE_CIF:	//CIF(352*288(PAL)/352*240(N))
		*width = 352;
		*height = m_bPal ? 288 : 240;
		break;
	case DD_VIDEO_SIZE_HD1:	//HD1(704*288(PAL)/(704*240))
		*width = 704;
		*height =  m_bPal ? 288 : 240;
		break;
	case DD_VIDEO_SIZE_D1:	//D1(704*576(PAL)/704*480(N))
		*width = 704;
		*height =  m_bPal ? 576 : 480;
		break;

	case DD_VIDEO_SIZE_QVGA:	//QVGA(320*240)
		*width = 320;
		*height = 240;
		break;
	case DD_VIDEO_SIZE_VGA:	//VGA(640*480)
		*width = 640;
		*height = 480;
		break;
	//case DD_VIDEO_SIZE_XVGA:	//XVGA(1024*768)
	//	*width = 1024;
	//	*height = 768;
	//	break;
	case DD_VIDEO_SIZE_QQVGA:	//QQVGA(160*120)
		*width = 160;
		*height = 120;
		break;

	//case DD_VIDEO_SIZE_960H:	//960H(960*480)
	//	*width = 960;
	//	*height = 480;
	//	break;
	case   DD_VIDEO_SIZE_720P:	//720P(1280*720)
		*width = 1280;
		*height = 720;
		break;
	case DD_VIDEO_SIZE_1080P:	//1080P(1920*1080)
		*width = 1920;
		*height = 1080;
		break;
	//case DD_VIDEO_SIZE_SVGA:   //SVGA(800*600)
	//	*width = 800;
	//	*height = 600;
	//	break;
	case DD_VIDEO_SIZE_SXGA:   //SXGA(1280*1024)
		*width = 1280;
		*height = 1024;
		break;
	case DD_VIDEO_SIZE_960P:   //
		*width = 1280;
		*height = 960;
		break;
	case DD_VIDEO_SIZE_3M:   //
		*width = 2048;
		*height = 1536;
		break;

	case DD_VIDEO_SIZE_16_9_3M:   //
		*width = 2304;
		*height = 1296;
		break;

	case DD_VIDEO_SIZE_4M:   //
		*width = 2688;
		*height = 1520;
		break;

	case DD_VIDEO_SIZE_5M:   //
		*width = 2592;
		*height = 1944;
		break;

	default:
		*width = 0;
		*height = 0;
		break;
	}
}

int COnvifNetDevice::RegetSubStreamResolution(int stream_type, OnvifH264ParameterRange *aRangeParam, unsigned short in_devResolution, 
								 int in_width, int in_height, ENC_PARAM_RANG_INFO* p_enc_range )
{
	int i;
	int prev_width = 0;
	int prev_height = 0;
	int min_width = 0;
	int min_height = 0;
	int tmp_width;
	int tmp_height;
	long l_wxh_tmp = 0;
	long l_wxh_cur = 0;
	long l_wxh_min = 0;
	long l_wxh_dec = 0;

	int img_max_width = m_iIpcOnyMsMaxWidth;
	int img_max_height = m_iIpcOnyMsMaxHeight;
	int img_min_width = ONVIF_SECOND_STREAM_SIZE_MIN_WIDHT;
	int img_min_height = ONVIF_SECOND_STREAM_SIZE_MIN_HEIGHT;
	bool bMustLessD1 = true;

	int max_frame = 0;
	int min_frame = 0;

	if( T_MAJORSSTREAM == stream_type )
	{
		for( i = 0; i < aRangeParam->ipcReso.resoSize; i++ )
		{
			tmp_width = aRangeParam->ipcReso.resoContent[i].width;
			tmp_height = aRangeParam->ipcReso.resoContent[i].height;

			max_frame = aRangeParam->ipcReso.resoContent[i].maxFrameRate;
			min_frame = aRangeParam->ipcReso.resoContent[i].minFrameRate;

			if( tmp_width == in_width && tmp_height == in_height )
			{
				p_enc_range->width = prev_width;
				p_enc_range->height = prev_height;
				p_enc_range->max_frame = max_frame;
				p_enc_range->min_frame = min_frame;

				return 0;
			}
		}

		return -1;
	}

	if( m_bIpcHasThirdStream )
	{
		if( T_SUBSTREAM == stream_type )
		{
			bMustLessD1 = false;
		}
	}

	for( i = 0; i < aRangeParam->ipcReso.resoSize; i++ )
	{
		tmp_width = aRangeParam->ipcReso.resoContent[i].width;
		tmp_height = aRangeParam->ipcReso.resoContent[i].height;

		if( tmp_width > 0 && tmp_height > 0 )
		{
			if( bMustLessD1 )
			{
				if( tmp_width <= img_max_width && tmp_height <= img_max_height 
					&& tmp_width >= img_min_width && tmp_height >= img_min_height )
				{
					l_wxh_tmp = tmp_width*tmp_height;
					if( 352 == in_width )
					{
						if( 0 == l_wxh_min
							|| l_wxh_min > l_wxh_tmp )
						{
							prev_width = tmp_width;
							prev_height = tmp_height;

							max_frame = aRangeParam->ipcReso.resoContent[i].maxFrameRate;
							min_frame = aRangeParam->ipcReso.resoContent[i].minFrameRate;

							l_wxh_min = l_wxh_tmp;
						}
					}
					else
					{
						if( 0 == l_wxh_min 
							|| l_wxh_min < l_wxh_tmp )
						{
							prev_width = tmp_width;
							prev_height = tmp_height;

							max_frame = aRangeParam->ipcReso.resoContent[i].maxFrameRate;
							min_frame = aRangeParam->ipcReso.resoContent[i].minFrameRate;

							l_wxh_min = l_wxh_tmp;
						}
					}
				}
			}
			else
			{
				l_wxh_tmp = tmp_width*tmp_height;
				l_wxh_cur = in_width*in_height;
				l_wxh_dec = (l_wxh_tmp > l_wxh_cur) ? (l_wxh_tmp - l_wxh_cur) : (l_wxh_cur - l_wxh_tmp);
				if( 0 == l_wxh_min 
					|| l_wxh_min > l_wxh_dec )
				{
					prev_width = tmp_width;
					prev_width = tmp_height;

					max_frame = aRangeParam->ipcReso.resoContent[i].maxFrameRate;
					min_frame = aRangeParam->ipcReso.resoContent[i].minFrameRate;

					l_wxh_min = l_wxh_dec;
				}
			}
		}
	}

	if( bMustLessD1 )
	{
		if( 0 == prev_width || 0 == prev_height )
		{
			return -1;
		}
	}

	if( 0 == prev_width || 0 == prev_height )
	{
		i = 0;
		prev_width = aRangeParam->ipcReso.resoContent[i].width;
		prev_height = aRangeParam->ipcReso.resoContent[i].height;
	}

	p_enc_range->width = prev_width;
	p_enc_range->height = prev_height;
	p_enc_range->max_frame = max_frame;
	p_enc_range->min_frame = min_frame;

	return 0;
}

void COnvifNetDevice::GetDvrQuality(unsigned int profileNum,unsigned short dvrQua,unsigned short encodeType,int *devQua,int *devEnType)
{
   DD_IMAGE_QUALITY iQua;
   DD_VIDEO_ENCODE_MODE enMode;
   
   OnvifH264ParameterRange *tmpRange =NULL;

   lock_onvif_ctrl_op.Lock();
   if((enum EStreamType)profileNum == T_MAJORSSTREAM)
   {
      tmpRange = &m_majorParamRange;
   }
   if((enum EStreamType)profileNum == T_SUBSTREAM)
   {
	   tmpRange = &m_subParamRange;
   }
   if((enum EStreamType)profileNum == T_THIRDSTREAM)
   {
	   tmpRange = &m_thirdParamRange;
   }
   
   enMode = (DD_VIDEO_ENCODE_MODE)encodeType;
   if(enMode == DD_VIDEO_ENCODE_MODE_VBR)
   {
      *devEnType = tmpRange->encodeType.min;
   }
   else
   {
	  *devEnType = tmpRange->encodeType.max;
   }
   
   iQua = (DD_IMAGE_QUALITY)dvrQua;
   switch(iQua)
   {
	   case DD_IMAGE_QUALITY_LOWEST:
			*devQua = 1;
		   break;
	   case   DD_IMAGE_QUALITY_LOWER:
		   *devQua = 2;
		   break;
	   case  DD_IMAGE_QUALITY_LOW:
		   *devQua = 3;
		   break;
	   case   DD_IMAGE_QUALITY_MEDIUM:
         *devQua = (int)(tmpRange->qualityRange.max + tmpRange->qualityRange.min)/2;
		   break;
	   case  DD_IMAGE_QUALITY_HEIGHTER:
          *devQua = (int)tmpRange->qualityRange.max - 1;
		   break;
	   case DD_IMAGE_QUALITY_HEIGHTEST:
          *devQua = tmpRange->qualityRange.max;
		   break;
	   default :
		   break;
   }
   lock_onvif_ctrl_op.UnLock();
}

int	 COnvifNetDevice::VideoEncoderHandle(unsigned long aProfileNum,OnvifVideoEncoder *aEncoderSet,unsigned long dataLen,unsigned const char *pData)
{
	DD_ENCODE_CONFIG *pEncodeConf = NULL;
	//struct OnvifVideoEncoder vEncoderSet;
	int width = 0,height = 0,devQua = 0,devEnType = 0;
	int in_width;
	int in_height;
	COnvifDevConf devConf;
    
	enum EStreamType streamNum = T_ENDSTREAM;
	int tmpNum =0,subNum = 0;
	int result = -1;
	int eResult = -1;
    int maxBitRate=0;
	pEncodeConf = (DD_ENCODE_CONFIG *)pData;
    
	char videoEncoderConfToken[64];
	memset(videoEncoderConfToken,0,sizeof(videoEncoderConfToken));

	if(m_capaServ.bMediaEnable == false)
	{
		return -1;
	}
	m_channelLock.Lock();
	pEncodeConf += m_channelNum;
	m_channelLock.UnLock();
   
	if(pEncodeConf == NULL)
	{
		return -2;
	}
	if(pEncodeConf->iSize >dataLen)
	{
		return -3;
	}
    if(aEncoderSet == NULL)
	{
		return -4;
	}

	streamNum = (enum EStreamType)aProfileNum;

	if( T_MAJORSSTREAM == streamNum )
	{
		if( 0 == memcmp( pEncodeConf, &m_stMajorEncCfg, sizeof(DD_ENCODE_CONFIG) ) )
		{
			return 0;
		}
	}
	else if( T_SUBSTREAM == streamNum )
	{
		if( m_bSecondThirdSteamOnlyCif )
		{
			pEncodeConf->resolution = DD_VIDEO_SIZE_CIF;
		}

		if( 0 == memcmp( pEncodeConf, &m_stSubEncCfg, sizeof(DD_ENCODE_CONFIG) ) )
		{
			return 0;
		}
	}
	else if( T_THIRDSTREAM == streamNum )
	{
		if( m_bSecondThirdSteamOnlyCif )
		{
			pEncodeConf->resolution = DD_VIDEO_SIZE_CIF;
		}

		if( 0 == memcmp( pEncodeConf, &m_stThirdEncCfg, sizeof(DD_ENCODE_CONFIG) ) )
		{
			return 0;
		}
	}

	GetDvrResolution(pEncodeConf->resolution,&width,&height);
	if(width == 0 || height == 0 || pEncodeConf->rate == 0)
	{
		return -5;
	}

	ENC_PARAM_RANG_INFO stEncRangeInof;

	memset( &stEncRangeInof, 0, sizeof(ENC_PARAM_RANG_INFO));

	if(streamNum == T_MAJORSSTREAM)
	{
		in_width = width;
		in_height = height;
		tmpNum = GetMajorStreamToken();
		if(tmpNum <= 0)
		{
			return -6;
		}
		
		lock_onvif_ctrl_op.Lock();
		result = RegetSubStreamResolution( T_MAJORSSTREAM, &m_majorParamRange, 
			pEncodeConf->resolution, in_width, in_height, &stEncRangeInof );
		lock_onvif_ctrl_op.UnLock();
		if( result < 0 )
		{
			return -7;
		}

		m_bMajorResolutionWidth = width;
		m_bMajorResolutionHeight = height;


	}
	else if(streamNum == T_SUBSTREAM)
	{
		in_width = width;
		in_height = height;
		lock_onvif_ctrl_op.Lock();
		result = RegetSubStreamResolution( T_SUBSTREAM, &m_subParamRange, 
			pEncodeConf->resolution, in_width, in_height, &stEncRangeInof );
		lock_onvif_ctrl_op.UnLock();
		if( result < 0 )
		{
			return -8;
		}
		subNum = GetSubStreamToken();
		if(subNum <= 0)
		{
			return -9;
		}

		width = stEncRangeInof.width;
		height = stEncRangeInof.height;
	}
	else if( streamNum == T_THIRDSTREAM)
	{
		in_width = width;
		in_height = height;
		lock_onvif_ctrl_op.Lock();
		result = RegetSubStreamResolution( T_THIRDSTREAM, &m_thirdParamRange, 
			pEncodeConf->resolution, in_width, in_height, &stEncRangeInof );
		lock_onvif_ctrl_op.UnLock();
		if( result < 0 )
		{
			return -10;
		}
		subNum = GetThirdStreamToken();
		if(subNum <= 0)
		{
			return -11;
		}
		width = stEncRangeInof.width;
		height = stEncRangeInof.height;

	}

    lock_onvif_ctrl_op.Lock();
	if(streamNum == T_MAJORSSTREAM)
	{
	    maxBitRate  = m_majorParamRange.bitRateRange.max;
		strcpy(videoEncoderConfToken,m_profileServ.profile[tmpNum-1].videoEncoderConfToken);
	}
	else if(streamNum == T_SUBSTREAM)
	{
		maxBitRate  = m_subParamRange.bitRateRange.max;
       	strcpy(videoEncoderConfToken,m_profileServ.profile[subNum-1].videoEncoderConfToken); 
	}
	else if( streamNum == T_THIRDSTREAM )
	{
		maxBitRate  = m_thirdParamRange.bitRateRange.max;
		strcpy(videoEncoderConfToken,m_profileServ.profile[subNum-1].videoEncoderConfToken); 
	}
    lock_onvif_ctrl_op.UnLock();

	GetDvrQuality(streamNum,pEncodeConf->quality,pEncodeConf->encodeType,&devQua,&devEnType);
   
	int cur_frame_rate = pEncodeConf->rate;
	if( cur_frame_rate > stEncRangeInof.max_frame )
	{
		cur_frame_rate = stEncRangeInof.max_frame;
	}
	else if( cur_frame_rate < stEncRangeInof.min_frame )
	{
		cur_frame_rate = stEncRangeInof.min_frame;
	}

	if(aEncoderSet->encoderType != TPRO_END)
	{
		if(!aEncoderSet->bNearReso)
		{
			aEncoderSet->resolution.width  = width;
			aEncoderSet->resolution.height = height;
		}

		aEncoderSet->frameRate		   = cur_frame_rate;
		//aEncoderSet->quality		   = (float)devQua;
		//aEncoderSet->encodingInterval  = devEnType;

		if(pEncodeConf->maxBitrate > maxBitRate)
		{
			aEncoderSet->bitrate		   = maxBitRate;
		}
		else
		{
			aEncoderSet->bitrate		   = pEncodeConf->maxBitrate;
		}

		if(streamNum == T_SUBSTREAM)
		{
			aEncoderSet->h264Conf.gopSize = pEncodeConf->rate;
		}
		else if( streamNum == T_THIRDSTREAM )
		{
			aEncoderSet->h264Conf.gopSize = pEncodeConf->rate;
		}

		result = -1;
		//lock_onvif_ctrl_op.Lock();
		if( m_bOnvifCtrlEnable )
		{
			devConf.InitDevMediaParameter(m_capaServ.mediaServ,m_userName,m_passKey);
			devConf.InitDevUtcTimes(false,m_strSaveServUri,m_iIpcCtrlUsedType);
			result = devConf.SetVideoEncoder(videoEncoderConfToken,true,aEncoderSet,1);
			if(result != OPERATE_SUCCESS)
			{
				result = devConf.SetVideoEncoder(videoEncoderConfToken,true,aEncoderSet,0);
			}
		}
		else
		{
			result = OPERATE_FAILED;
		}
		//lock_onvif_ctrl_op.UnLock();

		if(result == OPERATE_SUCCESS)
		{
#ifdef ONVIF_DEBUG
			printf("***Device %s:set encoder successfully!\n",m_capaServ.devMgtServ);
#endif
			eResult = 0;
		}
#ifdef ONVIF_DEBUG
		else
		{
			printf("***Device %s:set encoder failed!\n",m_capaServ.devMgtServ);
		}
#endif
	}
    
	if( T_MAJORSSTREAM == streamNum )
	{
		memcpy( &m_stMajorEncCfg, pEncodeConf, sizeof(DD_ENCODE_CONFIG) );
	}
	else if( T_SUBSTREAM == streamNum )
	{
		memcpy( &m_stSubEncCfg, pEncodeConf, sizeof(DD_ENCODE_CONFIG) );
	}
	else if( T_THIRDSTREAM == streamNum )
	{
		memcpy( &m_stThirdEncCfg, pEncodeConf, sizeof(DD_ENCODE_CONFIG) );
	}

	return eResult;
}

void COnvifNetDevice::AlarmMotionHandle(int aStreamNum, unsigned const char *pData,unsigned long dataLen,bool bImmediately,int prod_id)
{
	DD_MOTION_CONFIG  *pMotionData =NULL;
	COnvifDevConf devConf;
	char anaToken[64];
	CPackBitsCompress	packbits;
	CBase64				base64;
	OnvifAnaMotionRule	rulePara;
	OnvifAnaModule		anaPara;
	
	int					row = 0;
	int					col  = 0;

	unsigned char		*activeCell = NULL;

	int					ByteNum = 0;
	int					DeltaByteNum = 0;

	pMotionData = (DD_MOTION_CONFIG  *)pData;

	m_channelLock.Lock();
	pMotionData += m_channelNum;
	m_channelLock.UnLock();

	if( 1 != aStreamNum )
	{
		return;
	}

	if(pMotionData == NULL)
	{
		return;
	}

	if(pMotionData->iSize > dataLen)
	{
		return;
	}

	memset(anaToken,0,sizeof(anaToken));

	memset(&rulePara,0,sizeof(OnvifAnaMotionRule));
	memset(&anaPara,0,sizeof(OnvifAnaModule));

	lock_onvif_ctrl_op.Lock();
	strcpy(anaToken,m_profileServ.profile[aStreamNum-1].analyticsToken);
	if( MANU_ONVIF_STENLEY == m_manu_prod_id )
	{
		row = 22; //pMotionData->area.widthNum;
		col = 18; //pMotionData->area.hightNum;
	}
	else
	{
		row = m_profileServ.profile[aStreamNum-1].motionRow;
		col = m_profileServ.profile[aStreamNum-1].motionCol;
	}
	lock_onvif_ctrl_op.UnLock();

	if(row == 0 || col ==0)
	{
		pMotionData = NULL;
		return;
	}

	DeltaByteNum		= (row * col)%8;
	ByteNum			= (row * col)/8;  	  
	if(DeltaByteNum)
	{
		ByteNum		+= 1;
	}

	activeCell = new unsigned char[ByteNum];
	if(activeCell == NULL)
	{
		return;
	}
	memset(activeCell,0,sizeof(unsigned char)*(ByteNum));

	//lock_onvif_ctrl_op.Lock();
	if( m_bOnvifCtrlEnable )
	{
		devConf.InitDevMediaParameter(m_capaServ.mediaServ,m_userName,m_passKey);
		devConf.InitDevUtcTimes(false,m_strSaveServUri,m_iIpcCtrlUsedType);
		devConf.GetAnaPara(anaToken,&anaPara);
		devConf.GetRulePara(anaToken,&rulePara);

		//set Sensitivity
		anaPara.sensitivetyValue = pMotionData->area.sensitivity*100 / 8;
		devConf.SetAnalyticsSensitivety(anaToken,&anaPara);
	}
	//lock_onvif_ctrl_op.UnLock();

	unsigned char cellMask[8] = {0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01};
	unsigned char packDest[ONVIF_MAX_MOTION_AREA_LEN];
	int packLen = 0;
	char dest64[ONVIF_MAX_MOTION_AREA_LEN];
	int value = 0;
	int bRow = 0;
	int eRow = 0;
	int bCol = 0;
	int eCol = 0;
	unsigned char* pMotionMskVal = (unsigned char*)pMotionData->area.area;
	unsigned char cmsk_val = 0;

	//set active cell value
	if(pMotionData->enable)
	{
		for( bCol = 0; bCol < col; bCol++ )
		{
			for( bRow = 0; bRow < row; bRow++ )
			{
				eRow = (bRow*pMotionData->area.widthNum)/row;
				eCol = (bCol*pMotionData->area.hightNum)/col;

				cmsk_val = pMotionMskVal[ eCol*((pMotionData->area.widthNum+7)>>3) + (eRow>>3)];
				if( cmsk_val & (1<<(eRow%8)))
				{
					value = bCol*row + bRow;
					activeCell[value>>3] |= cellMask[value%8]; 
				}
			}
		}
	}

	memset(packDest,0,sizeof(unsigned char)*ONVIF_MAX_MOTION_AREA_LEN);
	packLen  = packbits.Encode((const unsigned char *)activeCell,ByteNum,packDest);
	memset(dest64,0,sizeof(dest64));

	base64.Encode((const unsigned char *)packDest,packLen,(char *)dest64,ONVIF_MAX_MOTION_AREA_LEN);
	strcpy((char *)rulePara.area,dest64);
	//lock_onvif_ctrl_op.Lock();
	if( m_bOnvifCtrlEnable )
	{
		devConf.SetAnalyticsArea(anaToken,&rulePara);
	}
	//lock_onvif_ctrl_op.UnLock();


	if(activeCell != NULL)
	{
		delete[] activeCell;
	}
	activeCell  = NULL;

	pMotionData = NULL;

	return;

}

int COnvifNetDevice::SnapPictureHandle(int aStreamNum, unsigned const char *pData,unsigned long dataLen,bool bImmediately,int cap_num)
{
	DD_TRIGGER_RECORD *pCfgData = (DD_TRIGGER_RECORD *)pData;
	DD_ENCODE_CONFIG *pJpegCfg = (DD_ENCODE_CONFIG *)pData;
	unsigned  long		deviceID			= 0; 
	bool              bFind             = false;
	COnvifVideo videoSev;
	char    uriResp[256] = {0};
	char  profileToken[64]={0};
	int sret;
	
	m_channelLock.Lock();
	deviceID = m_channelNum;
	pCfgData += m_channelNum;
	m_channelLock.UnLock();

	lock_onvif_ctrl_op.Lock();
	strcpy(profileToken,m_profileServ.profile[aStreamNum-1].profileToken);
	lock_onvif_ctrl_op.UnLock();

	if( m_bOnvifCtrlEnable )
	{
		videoSev.InitDevUtcTimes(false,m_strSaveServUri,m_iIpcCtrlUsedType);
		videoSev.InitServParameter(m_capaServ.mediaServ,m_userName,m_passKey);
		sret = videoSev.GetSnapshotUri(profileToken,uriResp,1);
	}
	else
	{
		sret = OPERATE_FAILED;
	}

	if(OPERATE_SUCCESS == sret)
	{

		if( SNAP_JPEG_CAPTRUE_USED_DEFAULT == cap_num )
		{
			COnvifSnapJpeg::Instance()->SetSnapPeriodParam(m_channelNum,5,2,cap_num);
		}
		else
		{
			COnvifSnapJpeg::Instance()->SetSnapPeriodParam(m_channelNum,3,3,cap_num);
		}

		if(COnvifSnapJpeg::Instance()->StartSnap(m_channelNum,uriResp,GetRtpVideoData,this, m_serverAddr) >= 0)
		{
		}
	}


	return 1;
}



int COnvifNetDevice::SetDevIpConf(unsigned long datalen,const unsigned char *data)
{
	DD_NETWORK_IP_CONFIG *dvrIPData = NULL;
    COnvifDevMgt devMgt;
    struct OnvifNwParameter nwSet;

	memset(&nwSet,0,sizeof(struct OnvifNwParameter));
	if(datalen == 0 || data == NULL)
	{
		return 0;
	}
    
	dvrIPData = (DD_NETWORK_IP_CONFIG *)data;
	if((dvrIPData == NULL) || (dvrIPData->iSize > datalen))
	{
		return 0;
	}
	//memcpy(ipAddr,&(dvrIPData->IP),sizeof(unsigned long));

	//lock_onvif_ctrl_op.Lock();
	if( m_bOnvifCtrlEnable )
	{
		devMgt.InitDevMgtParameter(m_capaServ.devMgtServ,m_userName,m_passKey);
		devMgt.InitDevUtcTimes(false,m_strSaveServUri,m_iIpcCtrlUsedType);
		devMgt.GetNetworkParameter(&nwSet,0);   
		devMgt.SetNetworkParameter(&nwSet,0);
	}
	//lock_onvif_ctrl_op.UnLock();
	return 1;
}



int COnvifNetDevice::ResyncIpcTimes(void)
{
	int tMajorId = -1; //modify the time
	bool bResynTime = false;

	if( !m_bOnvifCtrlEnable )
	{
		return 0;
	}

	m_rtspLock.Lock();
	tMajorId = m_rtspMajorID;
	m_rtspLock.UnLock();

	if( tMajorId > 0 )
	{
		bResynTime = CClientRTPMain::Instance()->GetIsMustResyncSystem(tMajorId);
	}

	if( bResynTime )
	{
		DD_DATE_TIME_CONFIG stDataTimeCfg;

		if( 1 == m_stSaveDatatimeCfg.recv )
		{
			memcpy( &stDataTimeCfg, &m_stSaveDatatimeCfg, sizeof(DD_DATE_TIME_CONFIG) );
			NtpDateTimeHandle( &stDataTimeCfg, sizeof(DD_DATE_TIME_CONFIG) );
		}
	}

	return 0;
}

int COnvifNetDevice::ResyncIPCSystemTime(int sync_flag)
{
	int tMajorId = -1; //modify the time
	if( !m_bOnvifCtrlEnable )
	{
		return 0;
	}

	m_rtspLock.Lock();
	tMajorId = m_rtspMajorID;
	m_rtspLock.UnLock();

	if( tMajorId > 0 )
	{
		DD_DATE_TIME_CONFIG stDataTimeCfg;

		if( 1 == m_stSaveDatatimeCfg.recv )
		{
			memcpy( &stDataTimeCfg, &m_stSaveDatatimeCfg, sizeof(DD_DATE_TIME_CONFIG) );
			NtpDateTimeHandle( &stDataTimeCfg, sizeof(DD_DATE_TIME_CONFIG), false );
		}
	}
}


void COnvifNetDevice::GetTZTabName( char *aTZTab, unsigned int ipcManuID )
{
	int b_find_zone_time = 0;
	int idx = 0;
	int zcnt = 200;

	if( ONVIF_TIMEZONE_SET_DAHUA_MODE == ipcManuID )
	{
		if( strlen( m_tzTab ) )
		{
			strcpy(aTZTab,m_tzTab);
		}

		return;
	}


	while( (zcnt--) > 0 )
	{
		if( g_time_zone_info[idx].zone_index >= 100000 )
		{
			break;
		}

		if( m_timeZone == g_time_zone_info[idx].zone_index )
		{
			strcpy( aTZTab, g_time_zone_info[idx].zone_info );
			b_find_zone_time = 1;
			break;
		}

		idx++;
	}

	if( !b_find_zone_time )
	{
		if( strlen( m_tzTab ) )
		{
			strcpy(aTZTab,m_tzTab);
		}
	}
}

int COnvifNetDevice::NtpDateTimeHandle(const void *pData,int dataLen, bool bNotifyRtsp)
{
	COnvifDevMgt devMgt;
	struct OnvifNTPData setNtpData;
	struct OnvifNTPAddr ntpAddr;           
	DD_DATE_TIME_CONFIG *dvrNtpData =NULL;
  	struct OnvifDateTime dateTimeSet;

	int tMajorId = -1; //modify the time
	int tSubId   = -1;
	
	int hour= 0,minute= 0;
	bool behindUtc=false;
   // long long absoluteTime = 0; 
	
	int digestResult = -1;

	int result =-1;
	int  setResult =-1;

	if(pData == NULL || dataLen <= 0)
	{
		return -1;
	}


	dvrNtpData =(DD_DATE_TIME_CONFIG *)pData;
	if(dvrNtpData->iSize > dataLen)
	{
		return -1;
	}

	m_timeZone = (enum _dd_time_zone_name_)dvrNtpData->timeZone;
    memset(&dateTimeSet,0,sizeof(struct OnvifDateTime));


	m_bNtpSet = false;
	dateTimeSet.dateTimeType = TSetManual;

	//lock_onvif_ctrl_op.Lock();
	if( m_bOnvifCtrlEnable )
	{
		devMgt.InitDevMgtParameter(m_capaServ.devMgtServ,m_userName,m_passKey);
		devMgt.InitDevUtcTimes(false,m_strSaveServUri,m_iIpcCtrlUsedType);

		bool bEnableNTPbyManu = false;
		//如果不是海康，则不NTP对时
		if( MANU_ONVIF_HIKVISION == m_manu_prod_id )
		{
			bEnableNTPbyManu = true;
		}

		// 暂时关闭NTP校正时间
		if( bEnableNTPbyManu && dvrNtpData->enableNTP > 0)
		{
			memset(&ntpAddr,0,sizeof(struct OnvifNTPAddr));
			ntpAddr.type = TDNSName;
			strncpy(ntpAddr.DNSName,dvrNtpData->ntpServerAddr,sizeof(ntpAddr.DNSName));
			setNtpData.addrSize = 1;
			setNtpData.addr = &ntpAddr;
			setNtpData.FromDHCP = false;

			result = devMgt.SetNtpSetting(&setNtpData,1);
			if(result != OPERATE_SUCCESS)
			{
				result = devMgt.SetNtpSetting(&setNtpData ,0);
			}

			if(result == OPERATE_SUCCESS)
			{
			   m_bNtpSet = true;
				dateTimeSet.dateTimeType = TSetNTP;
			}
			//NTP's service address is not setting success sometimes,but we will allow update the 
			//the time from network service address which is default value in ipc.
			//m_bNtpSet = true;
			//dateTimeSet.dateTimeType = TSetNTP;
		}
	}
	//lock_onvif_ctrl_op.UnLock();

	{
		m_rtspLock.Lock();
		tMajorId = m_rtspMajorID;
		tSubId   = m_rtspSubID;
		m_rtspLock.UnLock();

		if( bNotifyRtsp )
		{
			if(tMajorId > 0)
			{
				CClientRTPMain::Instance()->HandleSystemTimeChanged(tMajorId);
			}
			if(tSubId > 0)
			{
				CClientRTPMain::Instance()->HandleSystemTimeChanged(tSubId);
			}
			if( m_rtspThirdID > 0 )
			{
				CClientRTPMain::Instance()->HandleSystemTimeChanged(m_rtspThirdID);
			}
		}

		if( strlen( m_dstTab ) )
		{
			strcpy(dateTimeSet.DSTTab,m_dstTab);
		}

		GetTZTabName( dateTimeSet.TZTab, m_ipc_timezone_mode );

		memset(dateTimeSet.timeZone,'\0',sizeof(dateTimeSet.timeZone));

		GetTimeZoneTime(m_timeZone,&hour,&minute,&behindUtc);
		dateTimeSet.deltaHour = hour;
		dateTimeSet.deltaMinute = minute;
		dateTimeSet.bBehindUtc  = behindUtc;

		printf( "current NtpDateTimeHandle channel %d, tzTab dstTab %s,%s-----%s,%d\n", m_channelNum, 
			m_tzTab, m_dstTab, __FILE__, __LINE__ );
		printf( "current NtpDateTimeHandle channel %d, TimeZome %d,(%d,%d,%d)--%s,%d\n", m_channelNum,
			m_timeZone, hour, minute, behindUtc, __FILE__, __LINE__ );

	   	tm nowTm = DVRTimeToTm(GetCurrTime());
		dateTimeSet.date.Year = nowTm.tm_year + 1900;
		dateTimeSet.date.Month = nowTm.tm_mon + 1;
		dateTimeSet.date.Day = nowTm.tm_mday;
		dateTimeSet.time.Hour = nowTm.tm_hour;
		dateTimeSet.time.Minute = nowTm.tm_min;
		dateTimeSet.time.Second = nowTm.tm_sec;
#ifdef ONVIF_DEBUG
		printf("**NTP Time set:%d:%d:%d,%d:%d:%d\n",nowTm.tm_year,nowTm.tm_mon +1,nowTm.tm_mday,nowTm.tm_hour,nowTm.tm_min,nowTm.tm_sec);
#endif
	}
    
	if(m_bDst)
	{
		dateTimeSet.bDaylightSave= true;
		strcpy(dateTimeSet.dstInfo,m_dstTz);
	}
	else
	{
		dateTimeSet.bDaylightSave= false;
	}


	//lock_onvif_ctrl_op.Lock();
	if( m_bOnvifCtrlEnable )
	{
		int ipcTimeZoneMode = m_ipc_timezone_mode;
		devMgt.InitDevUtcTimes(false,m_strSaveServUri,m_iIpcCtrlUsedType);
		digestResult = devMgt.SetNtpActive(&dateTimeSet,1,ipcTimeZoneMode);
		if(digestResult != OPERATE_SUCCESS)
		{
			digestResult = devMgt.SetNtpActive(&dateTimeSet,0,ipcTimeZoneMode);
		}
	}
	else
	{
		digestResult = OPERATE_FAILED;
	}
	//lock_onvif_ctrl_op.UnLock();

	if(digestResult == OPERATE_SUCCESS)
	{
		setResult = 0;
	}
	else
	{
		printf("Set NTP time failed!---channel %d\n", m_channelNum );
	}
	dvrNtpData =NULL;

   return setResult;
}

int COnvifNetDevice::DaylightHandle(const void *pData,int dataLen)
{
	int result = -1,re = -1;
	int hour= 0,minute = 0;
	bool bBehindUtc= false;
    int tOffset = 0 ;
    char timeBuf[64];

	int inHour= 0,inMinute=0,inSec=0;
	int outHour= 0,outMinute= 0,outSec=0;
    
	int inDay=0,outDay=0 ,i=0; 

    OnvifDateTime    dateTimeSet;
	COnvifDevMgt     devMgt;
    DD_DAYLIGHT_INFO *pDstData = NULL;
	char DstTab[8]= {0};
	char TzTab[8] = {0};

	if(pData == NULL || dataLen <= 0)
	{
		return -1;
	}

    pDstData = (DD_DAYLIGHT_INFO *)pData;
   
    if(sizeof(pDstData) > dataLen)
	{
		return -1;
	}

	//这里的时区是在NTP对时设置的。
	if(m_timeZone == DD_TIME_ZONE_NUM)
	{
		return -1;
	}

	GetTimeZoneTime(m_timeZone,&hour,&minute,&bBehindUtc);
	if(hour>=24 || minute >= 60)
	{
		return -1;
	}

	memset(&dateTimeSet,0,sizeof(OnvifDateTime));
	memset(timeBuf,'\0',sizeof(timeBuf));
	memset(dateTimeSet.timeZone,'\0',sizeof(dateTimeSet.timeZone));
    
	if( strlen( m_dstTab ) )
	{
		strcpy(dateTimeSet.DSTTab,m_dstTab);
	}
	GetTZTabName( dateTimeSet.TZTab, m_ipc_timezone_mode );

    dateTimeSet.deltaHour = hour;
	dateTimeSet.deltaMinute = minute;
	dateTimeSet.bBehindUtc = bBehindUtc;

	printf( "current DaylightHandle channel %d, tzTab dstTab %s,%s-----%s,%d\n", m_channelNum, 
		m_tzTab, m_dstTab, __FILE__, __LINE__ );
	printf( "current DaylightHandle channel %d, TimeZome %d,(%d,%d,%d)--%s,%d\n", m_channelNum,
		m_timeZone, hour, minute, bBehindUtc, __FILE__, __LINE__ );

   if(pDstData->enable)
   {
      dateTimeSet.bDaylightSave = true;
	  dateTimeSet.tOffset = pDstData->offSet/3600;
      
	  dateTimeSet.beginDate.Year = pDstData->InYear;
	  dateTimeSet.beginDate.Month = pDstData->InMonth;
	  dateTimeSet.beginDate.Day   = pDstData->InMday;

	  dateTimeSet.endDate.Year   = pDstData->OutYear;
	  dateTimeSet.endDate.Month  = pDstData->OutMonth;
	  dateTimeSet.endDate.Day    = pDstData->OutMday;

	  dateTimeSet.beginTime.Hour = pDstData->InSecond /3600;
	  dateTimeSet.beginTime.Minute = (pDstData->InSecond % 3600)/60;
	  dateTimeSet.beginTime.Second = pDstData->InSecond % 60;

	  dateTimeSet.endTime.Hour = pDstData->OutSecond /3600;
	  dateTimeSet.endTime.Minute = (pDstData->OutSecond % 3600)/60;
	  dateTimeSet.endTime.Second = pDstData->OutSecond % 60;
      
	  if(pDstData->type == 0)//the week format
	  {
        dateTimeSet.bWeekType = true;
	  }
	  else //the date format
	  {
        dateTimeSet.bWeekType = false;
	  }

	  dateTimeSet.beginWday = pDstData->InWday;
	  dateTimeSet.beginWeekIndex = pDstData->InWeekIndex;

	  dateTimeSet.endWday  = pDstData->OutWday;
	  dateTimeSet.endWeekIndex = pDstData->OutWeekIndex;

   }
   else
   {
	   m_bDst = false;
	   dateTimeSet.bDaylightSave = false;
   }
    
   if(m_bNtpSet)
   {
      dateTimeSet.dateTimeType = TSetNTP;
   }
   else
   {
	  dateTimeSet.dateTimeType = TSetManual;
   }
   
  
    //lock_onvif_ctrl_op.Lock();
	if( m_bOnvifCtrlEnable )
	{
		int ipcTimeZoneMode = m_ipc_timezone_mode;
		devMgt.InitDevMgtParameter(m_capaServ.devMgtServ,m_userName,m_passKey);
		devMgt.InitDevUtcTimes(false,m_strSaveServUri,m_iIpcCtrlUsedType);
		result = devMgt.SetDstActive(&dateTimeSet,1,ipcTimeZoneMode);
		if(result != OPERATE_SUCCESS)
		{
			result = devMgt.SetDstActive(&dateTimeSet,0,ipcTimeZoneMode);
		}
	}
	else
	{
		result = OPERATE_FAILED;
	}
	//lock_onvif_ctrl_op.UnLock();

	if(result == OPERATE_SUCCESS)
	{
		if(dateTimeSet.bDaylightSave == true)
		{
			m_bDst = true;
			memset(m_dstTz,'\0',sizeof(m_dstTz));
			strcpy(m_dstTz,dateTimeSet.dstInfo);
		}
		re = 0;
	}	
	else
	{
		printf("***DST Set error channel %d!\n", m_channelNum );
	}
	return re;
}

void COnvifNetDevice::GetTzDstTab(char *aTzTab,char *aDstTab)
{
   int result = -1;
   COnvifDevMgt     devMgt;
   struct OnvifDateTime dateTimeResp;
   char *tmp1 = NULL;
   char *tmp2 = NULL;

	if(aTzTab == NULL || aDstTab == NULL)
   {
	   return;
   }
	memset(&dateTimeResp,0,sizeof(struct OnvifDateTime));

	//lock_onvif_ctrl_op.Lock();
	if( m_bOnvifCtrlEnable )
	{
		devMgt.InitDevMgtParameter(m_capaServ.devMgtServ,m_userName,m_passKey);
		devMgt.InitDevUtcTimes(false,m_strSaveServUri,m_iIpcCtrlUsedType);
		result = devMgt.GetDevDateTime(&dateTimeResp,1);
		if(result != OPERATE_SUCCESS)
		{
			result = devMgt.GetDevDateTime(&dateTimeResp,0);
		}
	}
	else
	{
		result = OPERATE_FAILED;
	}
	//lock_onvif_ctrl_op.UnLock();

   if(result == OPERATE_SUCCESS)
   {
	   //get timezone tab  
      if(dateTimeResp.TZTab[0] != '\0')
	  {
		  strcpy(aTzTab,dateTimeResp.TZTab);
	  }
       //Get DST tab
	   if(dateTimeResp.DSTTab[0] != '\0')
	   {	
		   strcpy(aDstTab,dateTimeResp.DSTTab);
	   }		   	  
   }

}

//get the hour and minute value which it is in west or east  of the UTC time
//input:timezone
//output: hour,minute and bool value behindUtc
void COnvifNetDevice::GetTimeZoneTime(unsigned int aTimeZone,int *aHour,int *aMinute,bool *aBehindUtc)
{
	enum _dd_time_zone_name_ tzone = (enum _dd_time_zone_name_)aTimeZone;
	switch(tzone)
	{
	case DD_TIME_ZONE_GMT_D12:
		*aHour = 12;
		*aMinute = 0;
		*aBehindUtc = true;
		break;

	case DD_TIME_ZONE_GMT_D11:
		*aHour = 11;
		*aMinute = 0;
		*aBehindUtc = true;
		break;
	case DD_TIME_ZONE_GMT_D10:
		*aHour = 10;
		*aMinute = 0;
		*aBehindUtc = true;
		break;
	case DD_TIME_ZONE_GMT_D9:
		*aHour = 9;
		*aMinute = 0;
		*aBehindUtc = true;
		break;
	case DD_TIME_ZONE_GMT_D8:
		*aHour = 8;
		*aMinute = 0;
		*aBehindUtc = true;
		break;
	case DD_TIME_ZONE_GMT_D7:
		*aHour = 7;
		*aMinute = 0;
		*aBehindUtc = true;
		break;
	case DD_TIME_ZONE_GMT_D6:
		*aHour = 6;
		*aMinute = 0;
		*aBehindUtc = true;
		break;
	case DD_TIME_ZONE_GMT_D5:
		*aHour = 5;
		*aMinute = 0;
		*aBehindUtc = true;
		break;
	case DD_TIME_ZONE_GMT_D4_30:
		*aHour = 4;
		*aMinute = 30;
		*aBehindUtc = true;
		break;
	case DD_TIME_ZONE_GMT_D4:
		*aHour = 4;
		*aMinute = 0;
		*aBehindUtc = true;
		break;
	case DD_TIME_ZONE_GMT_D3_30:
		*aHour = 3;
		*aMinute = 30;
		*aBehindUtc = true;
		break;
	case DD_TIME_ZONE_GMT_D3:
		*aHour = 3;
		*aMinute = 0;
		*aBehindUtc = true;
		break;
	case DD_TIME_ZONE_GMT_D2:
		*aHour = 2;
		*aMinute = 0;
		*aBehindUtc = true;
		break;
	case DD_TIME_ZONE_GMT_D1:
		*aHour = 1;
		*aMinute = 0;
		*aBehindUtc = true;
		break;
	case DD_TIME_ZONE_GMT:
		*aHour = 0;
		*aMinute = 0;
		*aBehindUtc = true;
		break;
	case DD_TIME_ZONE_GMT_A1:
		*aHour = 1;
		*aMinute = 0;
		*aBehindUtc = false;
		break;
	case DD_TIME_ZONE_GMT_A2:
		*aHour = 2;
		*aMinute = 0;
		*aBehindUtc = false;
		break;
	case DD_TIME_ZONE_GMT_A3:
		*aHour = 3;
		*aMinute = 0;
		*aBehindUtc = false;
		break;
	case DD_TIME_ZONE_GMT_A3_30:
		*aHour = 3;
		*aMinute = 30;
		*aBehindUtc = false;
		break;
	case DD_TIME_ZONE_GMT_A4:
		*aHour = 4;
		*aMinute = 0;
		*aBehindUtc = false;
		break;
	case DD_TIME_ZONE_GMT_A4_30:
		*aHour = 4;
		*aMinute = 30;
		*aBehindUtc = false;
		break;
	case DD_TIME_ZONE_GMT_A5:
		*aHour = 5;
		*aMinute = 0;
		*aBehindUtc = false;
		break;
	case DD_TIME_ZONE_GMT_A5_30:
		*aHour = 5;
		*aMinute = 30;
		*aBehindUtc = false;
		break;
	case DD_TIME_ZONE_GMT_A5_45:
		*aHour = 5;
		*aMinute = 45;
		*aBehindUtc = false;
		break;
	case DD_TIME_ZONE_GMT_A6:
		*aHour = 6;
		*aMinute = 0;
		*aBehindUtc = false;
		break;
	case DD_TIME_ZONE_GMT_A6_30:
		*aHour = 6;
		*aMinute = 30;
		*aBehindUtc = false;
		break;
	case DD_TIME_ZONE_GMT_A7:
		*aHour = 7;
		*aMinute = 0;
		*aBehindUtc = false;
		break;
	case DD_TIME_ZONE_GMT_A8:
		*aHour = 8;
		*aMinute = 0;
		*aBehindUtc = false;
		break;
	case DD_TIME_ZONE_GMT_A9:
		*aHour = 9;
		*aMinute = 0;
		*aBehindUtc = false;
		break;
	case DD_TIME_ZONE_GMT_A9_30:
		*aHour = 9;
		*aMinute = 30;
		*aBehindUtc = false;
		break;
	case DD_TIME_ZONE_GMT_A10:
		*aHour = 10;
		*aMinute = 0;
		*aBehindUtc = false;
		break;
	case DD_TIME_ZONE_GMT_A11:
		*aHour = 11;
		*aMinute = 0;
		*aBehindUtc = false;
		break;
	case DD_TIME_ZONE_GMT_A12:
		*aHour = 12;
		*aMinute = 0;
		*aBehindUtc = false;
		break;
	case DD_TIME_ZONE_GMT_A13:
		*aHour = 13;
		*aMinute = 0;
		*aBehindUtc = false;
		break;
	default:
		*aHour = 24;
		*aMinute = 60;
		break;
	} 
}

void COnvifNetDevice::QuitDevInfo(void)
{
	//memset(&m_capaServ,0,sizeof(struct OnvifDevCapaServ));  
	m_profileServ.profileSize = 0;

	memset(&m_majorVideo,0,sizeof(OnvifVideoUri));
	memset(&m_subVideo,0,sizeof(OnvifVideoUri));

	memset(&m_encoderMajorPara,0,sizeof(OnvifVideoEncoder));
	memset(&m_encoderSubPara,0,sizeof(OnvifVideoEncoder));
	memset(&m_encoderThirdPara,0,sizeof(OnvifVideoEncoder));

}

void   COnvifNetDevice::UpdateIPAddr(char *oldAddr,char *newAddr,int port)
{
      char *pos1= NULL;
	  char *pos2 =NULL;
	  char *pos3 = NULL;
	  char tBuf[256];

	  memset(tBuf,0,sizeof(tBuf));
      if(oldAddr == NULL || newAddr == NULL)
	  {
		  return;
	  }
	  pos1 = strstr(oldAddr,"http://");
     if(pos1 != NULL)
	 {
		 pos1 += 7;
		 pos2 = strstr(pos1,":");

		 if(pos2 != NULL)
		 {
			 sprintf(tBuf,"http://%s",newAddr);
			 strcat(tBuf,pos2);
			 strncpy(oldAddr,tBuf,strlen(tBuf));
		 }
		 else
		 {
			 pos2= strstr(pos1,"/");
			 if(pos2 != NULL)
			 {
				 sprintf(tBuf,"http://%s:%d",newAddr,port);
				 strcat(tBuf,pos2);
				 strncpy(oldAddr,tBuf,strlen(tBuf));
			 }
		 }
		 return;
	 }     
}

void   COnvifNetDevice::UpdateRtspIPAddr(char *oldAddr, int oldlen, char *newAddr)
{
	char *pos1= NULL;
	char *pos2 =NULL;
	char *pos3 = NULL;
	char *pos4 = NULL;
	char tBuf[256];

	memset(tBuf,0,sizeof(tBuf));
	if(oldAddr == NULL || newAddr == NULL)
	{
		return;
	}

	pos1 = NULL;
	pos1 = strstr(oldAddr,"rtsp://");
	if(pos1 != NULL)
	{
		pos1 += 7;
		pos2 = strstr(pos1,":");
		if(pos2 != NULL)
		{
			while(1)
			{
				pos4 = strstr( pos2+1, ":" );
				if( NULL == pos4 )
				{
					break;
				}

				pos2 = pos4;
			}

			sprintf(tBuf,"rtsp://%s",newAddr);
			strcat(tBuf,pos2);
			memset( oldAddr, 0, oldlen );
			strncpy(oldAddr,tBuf ,strlen(tBuf));
		}
		else
		{
		   pos2 = NULL;
		   pos2 = strstr(pos1,"/");
		   if(pos2 != NULL)
		   {
			   sprintf(tBuf,"rtsp://%s",newAddr);
			   strcat(tBuf,pos2);
			   memset( oldAddr, 0, oldlen );
			   strncpy(oldAddr,tBuf ,strlen(tBuf));
		   }
		}
	}
}