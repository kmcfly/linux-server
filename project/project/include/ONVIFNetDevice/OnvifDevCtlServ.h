/*******************************************************************************
** Copyright (c) Tongwei Video Technology Co.,Ltd. All rights reserved.				
** AUTHOR		:	������	
** DATE			:	2013-02-21
** FILENAME		:	OnvifDevCtlServ.h 
** Version		:	ONVIF 2.1
** DESCRIPTION	:
** Modify Record:
*******************************************************************************/
#ifndef OnvifDevCtlServ_H_
#define OnvifDevCtlServ_H_

#include "soapServ.h"

enum EOnvifRelayMode 
{
	T_RelayModeMonostable = 0, 
	T_RelayModeBistable = 1
};

enum EOnvifRelayIdleState 
{
	T_RelayIdleStateClosed = 0, 
	T_RelayIdleStateOpen = 1
};

enum EOnvifRelayLogicalState 
{
	T_RelayLogicalStateActive = 0, 
	T_RelayLogicalStateInactive = 1
};

//PTZ
enum Dev_boolean_ 
{
	_false_ = 0, 
	_true_ = 1
};

enum DevPTZPresetTourOperation
{
	start = 0,
	stop = 1,
	Pause = 2,
	Extended = 3
};

enum Exposure_Mode
{
	ExposureMode__AUTO = 0, 
	ExposureMode__MANUAL = 1
};//ͼ�񲿷��ֶ������Զ�

struct OnvifRelayOutputSet
{
	enum EOnvifRelayMode Mode;	/* required element of type tt:RelayMode */
	char DelayTime[64];	/* required element of type xsd:string */
	enum EOnvifRelayIdleState IdleState;	/* required element of type tt:RelayIdleState */
};

struct OnvifRelayOutput
{
	char token[64];	/* required attribute of type tt:ReferenceToken */
	struct OnvifRelayOutputSet Properties;	/* required element of type tt:RelayOutputSettings */
};

struct OnvifRelayOutputs
{
	int sizeRelayOutputs;	/* SOAP 1.2 RPC return element (when namespace qualified) */	/* sequence of elements <RelayOutputs> */
	struct OnvifRelayOutput *RelayOutputs;	/* optional element of type tt:RelayOutput */
};


//imaging
struct OnvifDevColor
{
	float Brightness;
	float ColorSaturation;
	float Contrast;
	float Sharpness;
};

struct OnvifFloatRange
{
	float min;
	float max;
};

struct OnvifDevColorRange
{
  OnvifFloatRange  Brightness;
  OnvifFloatRange  ColorSaturation;
  OnvifFloatRange  Contrast;
  OnvifFloatRange  Sharpness;

  bool            bIrisUse;
  OnvifFloatRange  Iris;

  bool            bFocusUse;
  OnvifFloatRange  focusSpeed;
  OnvifFloatRange  focusNear;
  OnvifFloatRange  focusFar;

};

struct OnviDevfPTZNodes
{
	int nodeSize;
	struct OnvifDevPTZNodeData *PTZNode;
};//ptz���нڵ�

struct OnvifDevPTZNodeData
{
	 char token[64];
	 char Name[64];
	 char *anyAttribute;
};

struct DevPTZSpaces
{   
	//char PositionSpace[8][128];
	float AbsolutePanTiltPositionSpace[2][2];
	float AbsoluteZoomPositionSpace[2];
    float RelativePanTiltTranslationSpace[2][2];
    float RelativeZoomTranslationSpace[2];
	float ContinuousPanTiltVelocitySpace[2][2];
	float ContinuousZoomVelocitySpace[2];
	float PanTiltSpeedSpace[2];
    float ZoomSpeedSpace[2];
};//ptz�ڵ�ת���Ĳ�����Χ

struct OnvifDevPTZNode
{
	struct DevPTZSpaces *SupportedPTZSpaces;
	int  MaximumNumberOfPresets;
	bool HomeSupported;
	//char **AuxiliaryCommands;
};

struct DevPTZGetConfigurations
{
	int __sizePTZConfiguration;
	int UseCount;
	char token[64];
};//PTZ���нڵ������

struct OnvifModifyPresetTour
{
	char TourToken[16];
	int SizePreset;
	char PresetsToken[256][16];
	char StayTime[256][16];
	float TourXSpeed[256];
	float TourYSpeed[256];
	char TourSpeedSpace[128];
};

const unsigned char CRUISE_NAME_MAX_LEN =32;
struct CruiseInfo{
	unsigned long cruiseFlag;//��ʼֹͣ��� 
	unsigned long cruisepointnum;//Ѳ��������
	unsigned long cruiseIndex;//Ѳ�������
	char  name[CRUISE_NAME_MAX_LEN+4];
};
struct CruisePresetInfo{
	unsigned char   preset;		//Ԥ�õ����
	unsigned char   dwellSpeed;	//��ת�ٶ�
	unsigned short  dwellTime;	//ͣ��ʱ�䣨�룩
};



struct DevPTZGetConfiguration
{
    char DefaultAbsolutePantTiltPositionSpace[128];	
	char DefaultAbsoluteZoomPositionSpace[128];	
	char DefaultRelativePanTiltTranslationSpace[128];	
	char DefaultRelativeZoomTranslationSpace[128];	
	char DefaultContinuousPanTiltVelocitySpace[128];//����ת���е�PanTilt��ַ
	char DefaultContinuousZoomVelocitySpace[128];//����ת����Zoom��ַ
	char DefaultPanTiltSpeedSpace[128];
	char DefaultZoomSpeedSpace[128];
	float PanTilt_Zoom[3];//Ĭ�ϵ��ٶȲ�������
	char DefaultPTZTimeout[64];//Ĭ�ϵ�timeout
};//GetConfiguration�з��ص�Ĭ�ϵ�ַ��Timeout�Լ�Ĭ�ϵ��ٶȲ���

struct DevPosition
{
	char space[4][128];//��ַ
	float position_pan[3];//�����ƶ�������ƶ��д洢PanTil��Zoom����
    float position_speed[3];//�����ƶ�������ƶ���speed�е�PanTil��Zoom����
};



struct ImgIrisdata
{
    float iris;
	float iris_max;
	float iris_min;
	enum Exposure_Mode Mode;
};//��Ȧ����

struct DevImgSetDate
{
    enum Dev_boolean_ boolean;
    enum Exposure_Mode *FocusMode;
	struct ImgIrisdata *IrisDate;
};//���õ��ڹ�Ȧ��С����



struct PTZMoveOptions
{
	//char MoveName[3][32];
	float Absolute[4];
	float Relative[4];
	float Continuous[2];
};//ptz�۽���ز���

enum DevPtzChange
{
   _add = 1,
   _decrease
};

enum PTZMoveDirection
{
   Right = 1,
   Left ,
   Up   ,
   Down ,
   Right_Up,
   Right_Down,
   Left_Up,
   Left_Down
};//PTZ�ƶ�����

struct ImgGetOptions
{
	float IrisMax;
	float IrisMin;
};//��Ȧ��С��Χ

struct PTZPresetTours
{
	int SizePresetTour;
	char PresetTourToken[16][16];
};//Ѳ��������
struct PresetsStruct
{
	char presetToken[257][16];
};

class COnvifDevCtl
{
public:
	COnvifDevCtl();
    ~COnvifDevCtl();
    
	void InitPTZServAddr(char *PTZServ,char *userName,char *passKey);
	void InitImagingServAddr(char *imagingServ,char *userName,char *passKey);
	void InitDevIOServAddr(char *devIOServAddr,char *userName,char *passKey);
	void InitDevUtcTimes( bool bClearUtcTime, char* aServAddr, unsigned int ipcCtrlType );

	//device IO control
	//if aDigest > 0 ,the username and password is encryption
	int GetRelayOutputs(struct OnvifRelayOutputs *relayResp,unsigned int aDigest);
	int SetRelayOutput(char *relayToken,struct OnvifRelayOutputSet *relatSettingData,unsigned int aDigest);
	int TriggerRelay(char *relayToken,enum EOnvifRelayLogicalState logicalState,unsigned int aDigest);
	int SendAuxilaryCommand(char *sendCommand,char *commandResp,unsigned int aDigest);

	//imaging control
	int GetImagingSetting(char *videoSourceToken,struct OnvifDevColor *colorResp,unsigned int aDigest);
	int SetImagingParameter(char *videoSourceToken,bool bModifyPersistence,struct OnvifDevColor *colorSet,unsigned int aDigest);
    int GetImagingParameterRange(char *videoSourceToken,struct OnvifDevColorRange *colorRangeResp,unsigned int aDigest);

	//PTZ control
	void InitPTZProfileToken(char *ProfileToken);
	int GetNode(char *NodeToken,OnvifDevPTZNode *PTZNodeResp,unsigned int aDigest);
    int ContinuousMove(char *timeout,DevPosition *position,unsigned int aDigest);
	int PTZMove(PTZMoveDirection MoveDirection,char *timeout,char *Panaddr,char *Zoomaddr,float PTZSpeed);
	int PTZZoom(DevPtzChange PtzChange,char *timeout,char *Panaddr,char *Zoomaddr,float PTZSpeed);
	void SetPTZSpeed(float speed_in,float speed_max,float PtzSpeed_max,float *speed_out);
	int PtzMoveStop(unsigned int aDigest);
	int GetAllPresets(char *aProfileToken,struct PresetsStruct *PresetsResp,int aDigest);
	int SetPreset(char *PresetsName, char *PresetsToken,unsigned int aDigest);
	int SetPreset(unsigned int PresetNum);
	int GotoPreset(char *PresetsToken,DevPosition *position,unsigned int aDigest);
	int RemovePreset(char *PresetsToken,unsigned int aDigest); 
    int OperatePresetTour(char *PresetTourToken,DevPTZPresetTourOperation *PresetTourOperation,unsigned int aDigest);
	int RemovePresetTour(char *PresetTourToken,unsigned int aDigest);
	int GetImagingIrisRange(char *videoSourceToken,ImgGetOptions *imgGetOptions,unsigned int aDigest);
	int GetImagingIris(char *videoSourceToken,float *Iris,unsigned int aDigest);
	int SetImagingSettings(char *videoSourceToken,DevImgSetDate *SetDate,unsigned int aDigest);//����ͼ�����
    int GetPTZFocusOptions(char *videoSourceToken,PTZMoveOptions *MoveOptions,unsigned int aDigest);
	int PTZFocusMove(char *videoSourceToken,DevPtzChange PtzChange,PTZMoveOptions *MoveOptions,float FocusSpeed,unsigned int aDigest);
    int CreatePresetTour(char *profileToken,unsigned int aDigest);
	int GetPresetTours(char *profileToken,struct PTZPresetTours *PresetTours,unsigned int aDigest);
	
	int ModifyPresetInTour(struct OnvifModifyPresetTour *PresetTour,unsigned int aDigest);
protected:

	void GetAppRelayOutputSettings(struct OnvifRelayOutputSet *appParam,struct tt__RelayOutputSettings *deviceParam);
	void SetDevRelayOutputSettings(struct tt__RelayOutputSettings *deviceParam,struct OnvifRelayOutputSet *appParam);
	void SetAppFloatRange(struct OnvifFloatRange *appFloatRange,struct tt__FloatRange *devFloatRange);
private:

   char m_PTZServAddr[128];
   char m_devIOServAddr[128];
   char m_imagingServAddr[128];
   //float m_Iris;
   char p_profileToken[64];

   char m_userName[32];
   char m_passWord[32];

   ONVIF_INIT_INPUT_PARAM stOnvifInputParam;


};


#endif