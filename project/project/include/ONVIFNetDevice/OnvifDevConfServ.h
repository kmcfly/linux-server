/*******************************************************************************
** Copyright (c) Tongwei Video Technology Co.,Ltd. All rights reserved.				
** AUTHOR		:	ÂæÃûÃÍ	
** DATE			:	2013-02-21
** FILENAME		:	OnvifDevConfServ.h 
** Version		:	ONVIF 2.1
** DESCRIPTION	:
** Modify Record:
*******************************************************************************/

#ifndef _OnvifDevConfServ_H_
#define _OnvifDevConfServ_H_

#include "soapServ.h"
#include "eventH.h"

const int ONVIF_CONF_LEN = 32;

#define ONVIF_MAX_PROFILE_NUMBER 10
#define ONVIF_MAX_RESOLUT_COUNT 30
#define ONVIF_MAX_MOTION_AREA_LEN 1024

enum EOnvifMediaProfile
{
	TPRO_H264= 0,
	TPRO_MPEG4,
	TPRO_JPEG,
	TPRO_END
};

enum EOnvifMpeg4ProfileType
{
	TOfMpeg4_SP = 0, 
	TOfMpeg4_ASP = 1,
	TOfMpeg4_END
};

enum EOnvifH264ProfileType
{
	TOfH264_Baseline = 0, 
	TOfH264_Main = 1,
	TOfH264_High = 2,
	TOfH264_Extended = 3, 
	TOfH264_END
};

struct OnvifPTZDevInfo
{
	char confToken[32];
	char nodeToken[32];
	char name[64];
	char absolutePTSpace[128];
	char absoluteZoomSpace[128];
	char relativePTSpace[128];
	char relativeZoomSpace[128];
	char continusePTVSpace[128];
	char continuseZVSpace[128];
    char defaultTimeOut[32]; 
    char limitPTSpace[128];
	char  limitZSpace[128];

	float defaultXSpeed;
	float defaultYSpeed;
	char  speedPTSpace[128];
	float defaultZSpeed;
	char  speedZSpace[128];
};

struct OnvifMediaProData
{
	enum EOnvifMediaProfile profileType;
	bool bFixed;
	char profileToken[64];
	char videoEncoderConfToken[64];
	char videoSourceConfToken[64];
	char videoSourceToken[64];

	bool bPTZ;
    OnvifPTZDevInfo *ptzDefaultInfo;

	char analyticsToken[64];
	int motionRow;
	int motionCol;

	OnvifPTZDevInfo stPtzDevInfo;
};

struct OnvifMediaProfile
{
	int profileSize;
	struct OnvifMediaProData *profile;
};

struct OnvifResolution
{
	int width;
	int height;
};

struct OnvifH264Conf
{
	int gopSize;
    enum EOnvifH264ProfileType profile;
};

struct OnvifMpeg4Conf
{
	int gopSize;
	enum EOnvifMpeg4ProfileType profile;
};

struct OnvifMulticastConf
{
	//struct OnvifMediaIPAddr Address;	/* required element of type tt:IPAddress */
	bool bIpv4;
	char ipAddr[64];
	int Port;	/* required element of type xsd:int */
	int TTL;	/* required element of type xsd:int */
	bool autoStart;	/* required element of type xsd:bool */
};

struct OnvifVideoEncoder
{
	enum EOnvifMediaProfile encoderType;
	bool bNearReso;
	struct OnvifResolution resolution;
	char  encoderName[64];
	int   useCount;
	float quality;
	int   frameRate;
	int   encodingInterval;
	int   bitrate;
    union 
	{
       struct OnvifH264Conf   h264Conf;
		struct OnvifMpeg4Conf  mpeg4Conf;
	};
	struct OnvifMulticastConf muticastConf;
	char sessionTimeout[64];
};


struct OnvifRange
{
	int min;
	int max;
};

struct OnvifJpegOptions
{
	int __sizeResolutionsAvailable;	/* sequence of elements <ResolutionsAvailable> */
	struct OnvifResolution *ResolutionsAvailable;	/* required element of type tt:VideoResolution */
	struct OnvifRange FrameRateRange;	/* required element of type tt:IntRange */
	struct OnvifRange EncodingIntervalRange;	/* required element of type tt:IntRange */
};

struct OnvifMpeg4Options
{
	struct OnvifJpegOptions JpegSetting;
	struct OnvifRange      GovLengthRange;	/* required element of type Range */
	bool Mpeg4Profile_ASP;/* required element */
	bool Mpeg4Profile_SP;/* required element */
};

struct OnvifH264Options
{
	struct OnvifJpegOptions JpegSetting;
	struct OnvifRange GovLengthRange;	/* required element of type Range */
	bool H264Profile_Baseline;
	bool H264Profile_Main;
	bool H264Profile_Extended;
	bool H264Profile_High;
};

struct OnvifJpegOptions2
{
	struct OnvifJpegOptions jpeg;
	struct OnvifRange       BitrateRange;	/* required element of type Range */
};

struct OnvifMpeg4Options2
{
	struct OnvifJpegOptions2 JpegSetting;
	struct OnvifRange   GovLengthRange;	/* required element of type Range */
	bool Mpeg4Profile_ASP;/* required element */
	bool Mpeg4Profile_SP;/* required element */
};

struct OnvifH264Options2
{
	struct OnvifJpegOptions2 JpegSetting;
	struct OnvifRange       GovLengthRange;	/* required element of type Range */
	bool H264Profile_Baseline;
	bool H264Profile_Main;
	bool H264Profile_Extended;
	bool H264Profile_High;
};

struct OnvifVideoEncoderOptionsExtension
{
	struct OnvifJpegOptions2  JPEG;	/* optional element of type JpegOptions2 */
	struct OnvifMpeg4Options2 MPEG4;	/* optional element of type Mpeg4Options2 */
	struct OnvifH264Options2  H264;	/* optional element of type H264Options2 */
};

struct OnvifVideoEncoderConfOptions
{
	struct OnvifRange QualityRange;	/* required element of type Range */
	bool   bJpegEnable;
	struct OnvifJpegOptions JPEG;	/* optional element of type JpegOptions */
	bool   bMpegEnable;
	struct OnvifMpeg4Options MPEG4;	/* optional element of type Mpeg4Options */
	bool   bH264Enable;
	struct OnvifH264Options H264;	/* optional element of type H264Options */
	bool   bExtensionEnable;
	struct OnvifVideoEncoderOptionsExtension Extension;	/* optional element of type VideoEncoderOptionsExtension */

	struct OnvifResolution stJpegResolu[ONVIF_MAX_RESOLUT_COUNT];
	struct OnvifResolution stMpeg4Resolu[ONVIF_MAX_RESOLUT_COUNT];
	struct OnvifResolution stH264Resolu[ONVIF_MAX_RESOLUT_COUNT];

	struct OnvifResolution stExtJpegResolu[ONVIF_MAX_RESOLUT_COUNT];
	struct OnvifResolution stExtMpeg4Resolu[ONVIF_MAX_RESOLUT_COUNT];
	struct OnvifResolution stExtH264Resolu[ONVIF_MAX_RESOLUT_COUNT];
};

//analytics struct
struct OnvifAnaMotionRule
{
	char type[64];
	char name[64];
	int   arealen;
	unsigned char area[ONVIF_MAX_MOTION_AREA_LEN]; //if the 128 is smaller,can add
};

struct OnvifAnaModule
{
	char type[64];
	char name[64];
	unsigned int  sensitivetyValue;
};


class COnvifDevConf
{
public:
  COnvifDevConf();
  COnvifDevConf(char *userName,char *passkey);
  ~COnvifDevConf();
   
  void InitDevMediaParameter(char *mediaAddr,char *userName,char *passKey);
  void InitDevEventParameter(char *eventAddr,char *userName,char *passKey);
  void InitDevUtcTimes( bool bClearUtcTime, char* aServAddr, unsigned int ipcCtrlType );

  int  GetEventProperty(bool &eventEnable,unsigned int aDigest,int manuProc =-1);
  //media configuration
  int GetMediaProfiles(OnvifMediaProfile *profileResp,unsigned int aDigest);

  int GetVideoEncoder(char *encoderToken,struct OnvifVideoEncoder *videoEncoderResp,unsigned int aDigest);
  int SetVideoEncoder(char *encoderToken,bool persistenceModifyParam,struct OnvifVideoEncoder *videoEncoderSet,unsigned int aDigest);

  int GetVideoEncoderRangeParameter(char *profileToken,char *encoderToken,struct OnvifVideoEncoderConfOptions *confOptionsResp,unsigned int aDigest);

  //alarm configuration

  //analytics interface
  int GetRulePara(char *analyticsToken,OnvifAnaMotionRule *ruleRespPara);
  int GetAnaPara(char *analyticsToken,OnvifAnaModule *anaRespPara);

  int SetAnalyticsArea(char *analyticsToken,OnvifAnaMotionRule *rulePara);
  int SetAnalyticsSensitivety(char *analyticsToken,OnvifAnaModule *anaParam);

protected:
	int SetAppJpegOptions(struct tt__JpegOptions *deviceJPEG,struct OnvifJpegOptions *appJPEG);
	int SetAppMpeg4Options(struct tt__Mpeg4Options *deviceMPEG4,struct OnvifMpeg4Options *appMPEG4);
	int SetAppH264Options(struct tt__H264Options *deviceH264,struct OnvifH264Options *appH264);
	int SetAppJpegOptions2(struct tt__JpegOptions2 *deviceJPEG,struct OnvifJpegOptions2 *appJPEG);
	int SetAppMpeg4Options2(struct tt__Mpeg4Options2 *deviceMPEG4,struct OnvifMpeg4Options2 *appMPEG4);
	int SetAppH264Options2(struct tt__H264Options2 *deviceH264,struct OnvifH264Options2 *appH264);
	int SetAppOptionsExtension(struct tt__VideoEncoderOptionsExtension *deviceExtension,struct OnvifVideoEncoderOptionsExtension *appExtension);

private:

	char m_mediaServAddr[128];
	char m_eventServAddr[128];
	
	char m_userName[ONVIF_CONF_LEN];
	char m_passWord[ONVIF_CONF_LEN];


	//modify
	//char m_h264ProfileToken[ONVIF_CONF_LEN*2];
	//char m_mpeg4ProfileToken[ONVIF_CONF_LEN*2];
	//char m_jpegProfileToken[ONVIF_CONF_LEN*2];

	//char m_h264EncoderToken[ONVIF_CONF_LEN*2];
	//char m_mpeg4EncoderToken[ONVIF_CONF_LEN*2];
	//char m_jpegEncoderToken[ONVIF_CONF_LEN*2];

	ONVIF_INIT_INPUT_PARAM stOnvifInputParam;


};


#endif