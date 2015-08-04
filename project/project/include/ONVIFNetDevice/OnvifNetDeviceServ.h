/*******************************************************************************
** Copyright (c) Tongwei Video Technology Co.,Ltd. All rights reserved.				
** AUTHOR		:	骆名猛	
** DATE			:	2013-02-21
** FILENAME		:	OnvifNetDeviceServ.h
** Version		:	ONVIF 2.1
** DESCRIPTION	:
** Modify Record:
*******************************************************************************/
#ifndef _OnvifNetDeviceServ_H_
#define _OnvifNetDeviceServ_H_

#include "OnvifDevMgtServ.h"
#include "OnvifDevConfServ.h"
#include "OnvifDevCtlServ.h"
#include "OnvifVideoServ.h"
//#include "OnvifDevAlarmServ.h"

#include "ClientRTPMain.h"

#ifdef WIN32
#include "mmreg.h"
#else
#include "lvfw.h"
#endif

#define ONVIF_VER40_ALARM_EVENT
#if defined(__AP_AU27__)
#define ONVIF_SNAP_ACTIVE   //snap control macor
#else
//#define ONVIF_SNAP_ACTIVE   //snap control macor
#endif

//#define ONVIF_WRITE_FILE_DEBUG

#define ONVIF_AUDIO_G711_FRM_LEN 320
#define ONVIF_AUDIO_G711_HEADER_FRM_LEN 324
#define ONVIF_AUDIO_FRAME_BUF_MAX_LEN 8192
#define ONVIF_AUDIO_MAX_FRM_COUNT ((ONVIF_AUDIO_FRAME_BUF_MAX_LEN-320)/ONVIF_AUDIO_G711_HEADER_FRM_LEN)
#define ONVIF_AUDIO_AVILD_FRMS_LEN (ONVIF_AUDIO_MAX_FRM_COUNT*ONVIF_AUDIO_G711_FRM_LEN)

class COnvifNetDevice:public  CNetDevice
{
public:
   //the second and third stream is the same video stream source

   enum EStreamType
   {
       T_MAJORSSTREAM = 0,
	   T_SUBSTREAM,
	   T_THIRDSTREAM,
	   T_ENDSTREAM,
   };
	typedef struct OnvifNetDevParam
	{
		//ip data operation
		CIPDataBufferMan		*m_pMainDataBufferMan;//main stream
		CIPDataBufferMan		*m_pSecDataBufferMan; //second stream
       CIPDataBufferMan        *m_pThirdDataBufferMan; //third stream
	   CIPDataBufferMan        *m_pSnapJpegBufferMan;

		BITMAPINFOHEADER		m_mainVideoFormat;
		BITMAPINFOHEADER		m_secVideoFormat; 
       BITMAPINFOHEADER       m_thirdVideoFormat;

		WAVEFORMATEX           m_mainAudioFormat;
		//WAVEFORMATEX           m_secAudioFormat;
		bool					bWaitKeyFrame;

	}ONVIFNETDEVPARAM;

	typedef struct OnvifIpcResolution
	{
		int  width;
		int  height;
		int  minFrameRate;
		int  maxFrameRate;
		//bool bUsed;
	}ONVIFIPCRESOLUTION;
   
	typedef struct OnvifResoStruc
	{
		int resoSize;
		OnvifIpcResolution *resoContent;
	};

	typedef struct OnvifValueRange
	{
		int min;
		int max;
	}ONVIFValueRange;
  
	typedef struct OnvifH264ParameterRange
	{
		OnvifResoStruc       ipcReso;   
		OnvifValueRange      encodeType;
		OnvifValueRange      qualityRange;
		OnvifValueRange      govLenRange;
		OnvifValueRange      bitRateRange;
	}ONVIFH264ParameterRange;

	typedef struct OnvifVideoUri
	{
       bool bVideo;
	   char  Uri[256];
	};

	typedef struct _EncParamRangeInfo
	{
		int width;
		int height;
		int max_frame;
		int min_frame;
	}ENC_PARAM_RANG_INFO;

public:
   COnvifNetDevice();
   ~COnvifNetDevice();

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
   virtual bool Initial(nd_in long deviceId,						//
	   nd_in long chnn,							//
	   nd_in char *pServer,				//
	   nd_in unsigned short netPort,				//
	   nd_in unsigned short httpPort,				//
	   nd_in unsigned short ctrlPort,				//
	   nd_in const std::string & user,			//
	   nd_in const std::string & password,		//
	   nd_in const ND_MAC & mac,
	   nd_in const char* pEther="eth0",
	   nd_in long sensorNum=0);				//

   /************************************************************************
   接口功能描述 : 反初始化
   ************************************************************************/
   virtual void Quit() ;

   /************************************************************************
   是否需要在连接时设置网络设备的配置参数
   ************************************************************************/
   //virtual bool IfNeedInitNetConfig(){return true;}

   /************************************************************************
   接口功能描述 : 设置数据缓存管理器
   1、本函数由NVR调用.
   2、收到网络图像数据(H264)后对其填充.
   ************************************************************************/
   virtual void SetDataBufferMan(nd_in CIPDataBufferMan *pDataBufferMan);


   /***********************************************************************
   修改设备绑定的通道号
   ************************************************************************/
   virtual void SetBindChannel(nd_in long chnn);
   virtual long GetBindChannel() ;

   virtual void SetLocalVideoNum(long localNum){};
   virtual unsigned int GetLocalVideoNum(){ return 0;};

   /***********************************************************************
   修改设备绑定的传感器通道号
   ************************************************************************/
   virtual void SetBindSensorChannel(nd_in long sensorChnn);
   virtual long GetBindSensorChannel();

   /***********************************************************************
   得到该网络设备的状态,断开或正常
   ************************************************************************/
   virtual long GetDevStatus() ;

	/***********************************************************************
	查看网络设备是否处于重连中
	************************************************************************/
	virtual bool IsReConnectting();
	
   /************************************************************************
   得到当前连接状态, ND_SC_BUSY以下, 认为不能处理后续命令                                                                    
   ************************************************************************/
   virtual long IsReady() ;

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
   virtual bool RequestStream( nd_in ULONGLONG video1,
	   nd_in ULONGLONG video2,
	   nd_in ULONGLONG video3,
	   nd_in ULONGLONG audio) ;

   /************************************************************************
   接口功能描述：请求视频关键帧
   输入参数描述：
   1、video1：主码流MASK。
   2、video2：次码流MASK。
   3、video3：手机码流MASK。
   返回值描述：返回成功或者失败。
   注：对应的位为零，表示停止。
   ************************************************************************/
   virtual bool RequestKeyFrame(nd_in ULONGLONG video1, 
	   nd_in ULONGLONG video2,
	   nd_in ULONGLONG video3);

   /************************************************************************
   接口功能描述：云台控制接口。
   输入参数描述：
   ************************************************************************/
   virtual bool PTZControl(nd_in long chnn,
	   nd_in unsigned long cmdtype,
	   nd_in unsigned long ptzParam,
	   nd_in unsigned long speed,
	   nd_in unsigned long waiteTime) ;


   /************************************************************************
   接口功能描述：获取图像色彩信息。
   ************************************************************************/
   virtual bool ColorControl(nd_in long chnn,
	   nd_in unsigned long cmdtype,
	   nd_in_out DD_VIDEO_COLOR& videoColor) ;

  /*
    function:
		镜像，翻转，走廊模式，畸变校正，自动对焦，ROI(感兴趣点)
    input:
		pchar 发送的内容
	return:
	    成功true, 失败 false 
	*/

	virtual bool OtherControl(long chnn, unsigned long cmdtype, unsigned char *pChar);

   /************************************************************************
   接口功能描述：设备的报警信息。
   输出参数描述：
   1、alarm：报警信息
   返回值描述：无。
   ************************************************************************/
   virtual ULONGLONG GetMotionAlarm();
   virtual ULONGLONG GetSensorAlarm();
   virtual ULONGLONG GetVlossAlarm();
   virtual ULONGLONG GetOtherAlarm();


   /************************************************************************
   接口功能描述 : 设置配置
   1、
   ************************************************************************/
   virtual long SetConfig( nd_in long chnn,
	   nd_in unsigned long itemID,
	   nd_in const unsigned char* pData,
	   nd_in unsigned long dataLen,
	   nd_in bool bImmediately);

   virtual long FlushConfig();

   /************************************************************************
   设置时间
   ************************************************************************/
   virtual long SetDateTime(nd_in const DD_TIME& ddTm);


   /************************************************************************
   获取设备支持参数信息
   ************************************************************************/
   virtual long GetDevInfo(nd_in long videoIndex, nd_out ND_DEV_INFO& ndDevInfo);

   virtual long GetEncodeInfo(char * pVideoEncodeInfo, int iVideoEncodeInfoLen);

   /************************************************************************
   接口功能描述 : 错误处理 
   1、正整型为常规错误, 负整数为严重错误
   ************************************************************************/
   virtual bool GetLastError(nd_out std::string& strErr);
   virtual long GetLastError();
   virtual void SetLastError(nd_in long err);

   /************************************************************************
   每秒钟调用一次，通知当前时间，模块可用此作为定时器
   ************************************************************************/
   virtual void OnTimer(unsigned long curTime);

   /***********************************************************************
   设置网络超时时间,单位ms
   ***********************************************************************/
   virtual void SetNetTimeout(unsigned long msTimeout);

   virtual unsigned long GetDevResolutionVal( int* width, int* height );
   
   static  int  GetRtpVideoData(RTSP_DATA_FRAME *pFrameData, int SeesionID, void *pParam); 
   static  int  GetIpcUsedInfo(RTSP_GET_IPC_USED_INFO* pIpcUsedInfo, void *pParam);
   static  int  FuncProcCallback(RTSP_FUNC_PROC_CALLBACK_INFO* p_func_info, void *pParam);

   void GetRtspRestart(bool &bMajorNeed, bool &bSubNeed,bool &bThirdNeed, bool &bIpcHasThirdStram);
   void RestartRtsp(unsigned char aStreamType); //0:major stream,1:sub stream,2:third stream,others end

   void CheckRtpConnectTimeout(unsigned long curTime);
   void GetIpcEventInfo(unsigned long curTime);
   int  ResyncIpcTimes(void);

   void PrintRunPos();

   void PtzInfoHanlde(const unsigned char* pData,int tDataLen);
  /* bool GetPtzInfo(void *pData,int tDataLen,int dataNum,int sDataLen);*/

   //定时同步IPC的时间
   virtual int ResyncIPCSystemTime(int sync_flag);

protected:


private:
	int		ProcessRTSPData(RTSP_DATA_FRAME *pFrameData, int SeesionID);
	int     ProcessAudioData(RTSP_DATA_FRAME *aFrameData,WAVEFORMATEX	   *aAudioInfo,CIPDataBufferMan	*aDataBufferMan,long channelNum);
	int     ProcessVideoData(RTSP_DATA_FRAME *aFrameData,BITMAPINFOHEADER	*aBitmapInfo,CIPDataBufferMan	*aDataBufferMan,long channelNum);
	int     ProcessSnapData(RTSP_DATA_FRAME* aFrameData,CIPDataBufferMan	*aDataBufferMan,long channelNum);

	void    InitNetDevParamer(void);
	void    QuitNetDev(void);
	void	InitIpcResolution(unsigned int streamNum,unsigned int streamType, bool &bHasLessD1);//OnvifH264ParameterRange *aParamRange);
	void	InitIpcColorRange(void);
	void    InitH264ParameterRange(OnvifH264ParameterRange *aRangeParam);
    void   GetIpcEncoderParameter(int aStreamNum,OnvifVideoEncoder *encoderParam);
    void   GetIpcVideoUri(unsigned int aVideoType,unsigned int aProfileNum);

	bool    CheckOnlyOneStreamMasterSize(OnvifH264ParameterRange* pH264ParamRange);
	bool    CheckStreamIsInThirdSize(OnvifH264ParameterRange* pH264ParamRange);
	
     int   RequestVideoStream(unsigned int aVideoType);
	 bool   StopVideoStream(int aSessionID, int lines );

	int		CheckH264Profile(OnvifMediaProfile *mediaProfile);
	int		GetSubStreamToken(void);
	int		GetMajorStreamToken(void);
	int		GetThirdStreamToken(void);

	int		CheckDvrResolution(int width,int height);

	int    RegetSubStreamResolution(int stream_type, OnvifH264ParameterRange *aRangeParam, unsigned short in_devResolution, 
			int in_width, int in_height, ENC_PARAM_RANG_INFO* p_enc_range );
	
	void	GetDvrResolution(unsigned short devResolution,int *width,int *height);
	void	GetDvrQuality(unsigned int profileNum,unsigned short dvrQua,unsigned short encodeType,int *devQua,int *devEnType);
	int		VideoEncoderHandle(unsigned long profileNum,OnvifVideoEncoder *aEncoderSet,unsigned long dataLen,unsigned const char *pData);
	int		SetDevIpConf(unsigned long datalen,const unsigned char *data);

	int     ChangeSecondStreamParam(unsigned long dataLen,unsigned const char *pData);
	int     ChangeThirdStreamParam(unsigned long dataLen,unsigned const char *pData);

	void    AlarmMotionHandle(int aStreamNum,unsigned const char *pData,unsigned long dataLen,bool bImmediately,int prod_id);
	int     SnapPictureHandle(int aStreamNum, unsigned const char *pData,unsigned long dataLen,bool bImmediately,int cap_num);

	int 	NtpDateTimeHandle(const void *pData,int dataLen, bool bNotifyRtsp = true );
	int    DaylightHandle(const void *pData,int dataLen);

	void   GetTZTabName( char *aTZTab, unsigned int ipcManuID );
	void   GetTzDstTab(char *aTzTab,char *aDstTab);
	void   GetTimeZoneTime(unsigned int aTimeZone,int *aHour,int *aMinute,bool *aBehindUtc);
	
	void   QuitDevInfo(void);

	void   UpdateIPAddr(char *oldAddr,char *newAddr,int port);
	void   UpdateRtspIPAddr(char *oldAddr, int oldlen, char *newAddr);

	int   CurIpcInfoCmd(RTSP_GET_IPC_USED_INFO* pIpcUsedInfo);
	int   FuncProcTypeCmd(RTSP_FUNC_PROC_CALLBACK_INFO* p_func_info);

	void  InitObjectValiant();

private:
   OnvifDevCapaServ				m_capaServ;
   OnvifMediaProfile			m_profileServ;
   
   //major stream RTSP service url
   OnvifVideoUri				m_majorVideo;
   //sub stream RTSP service url
   OnvifVideoUri				m_subVideo;
   OnvifVideoUri                m_thirdVideoUri;

   //H264 parameter
   OnvifH264ParameterRange		m_majorParamRange;
   OnvifH264ParameterRange		m_subParamRange;
   OnvifH264ParameterRange      m_thirdParamRange;

   OnvifVideoEncoder			m_encoderMajorPara;
   OnvifVideoEncoder			m_encoderSubPara;
   OnvifVideoEncoder			m_encoderThirdPara;

   bool                          m_bFirstSetNetEncCfg;

   OnvifDevColorRange			m_imageParaRange;

    char						m_serverAddr[128];
   //username and password
   char							m_userName[32];
   char							m_passKey[32];

   //time zone and Daylight saving time
   unsigned int					m_timeZone;
   bool							m_bDst;  //DST time setting
   bool							m_bNtpSet; //NTP time setting

   char							m_dstTz[96];
   char							m_tzTab[32];
   char							m_dstTab[32];
      
   //device parameter
   OnvifNetDevParam             stOnvifNetParamVal;
   OnvifNetDevParam				*m_pOnvifNetDev;

   //audio buffer 
   char							*m_pAudioBuffer;
   unsigned     int			m_audioBufferLen;
   
   //device relate information
   CPUB_Lock					m_channelLock;
   CPUB_Lock					m_rtspLock;
   CPUB_Lock                    lock_onvif_ctrl_op;
   CPUB_Lock					m_devStatusLock;


   NET_DEVICE_STATUS_CONNECT	m_ctrlStatus;
   NET_DEVICE_STATUS			m_deviceStatus; //device connect status

   int  						m_rtspMajorID; //the major stream ID
   int							m_rtspSubID;  //the sub  stream ID
   int							m_rtspThirdID;
  
   bool							m_bRtspMajorNeedRestart;
   bool							m_bRtspSubNeedRestart;
   bool                         m_bRtspThirdNeedRestart;

   int                          m_iRtspMajorDiscnnCnt;
   int                          m_iRtspSubDiscnnCnt;
   int                          m_iRtspThirdDiscnnCnt;

   long							m_channelNum; //the device binding channel
   int                          m_manu_prod_id;
   int                          m_ipc_timezone_mode;
   long							m_sensorNum;

   bool							m_bPal;		//是否为PAL制，true：是PAL制，false：是NTSC制

   unsigned long				m_majorVideoCurrCount;
   unsigned long				m_subVideoCurrCount;
   unsigned long				m_thirdVideoCurrCount;

   unsigned long				m_majorVideoSaveCount;
   unsigned long				m_subVideoSaveCount;
   unsigned long				m_thirdVideoSaveCount;

   unsigned long                m_lTestFrameMasterCnt;
   unsigned long                m_lTestFrameSecondCnt;
   unsigned long                m_lTestFrameThirdCnt;

   unsigned long                m_saveCheckTimes;

   unsigned long				m_currTime;
   unsigned long				m_netTimeout;
   unsigned int                 m_fastCheckNetTimeNumber;
    
   //third stream parameter
   bool                         m_bMajorRequest;
   bool							m_bSecRequest;
   bool							m_bThirdRequest;

   bool                         m_bDevicePtzCtlActive;

#ifdef   ONVIF_WRITE_FILE_DEBUG
    FILE						*m_file;
	FILE						*m_audioFile;
#endif

	int                         m_snapImageID;


//config change
	DD_ENCODE_CONFIG            m_stMajorEncCfg;
	DD_ENCODE_CONFIG            m_stSubEncCfg;
	DD_ENCODE_CONFIG            m_stThirdEncCfg;

	int                        m_bMajorResolutionWidth;
	int                        m_bMajorResolutionHeight;

	DD_DATE_TIME_CONFIG        m_stSaveDatatimeCfg;

	bool                       m_bIpcOnlyMasterStream;
	unsigned int               m_iIpcOnyMsMaxWidth;
	unsigned int               m_iIpcOnyMsMaxHeight;

	bool                       m_bIpcHasThirdStream;
	bool                       m_bHasRequestSecondStream;
	bool                       m_bHasRequestThirdStream;

	//第二码流只能设置CIF
	bool                       m_bSecondThirdSteamOnlyCif;

	unsigned long              m_ulPrevGetIpcEventTime;

	bool                       m_eventEnable;

	unsigned long              times_for_resync_time;

	unsigned long              m_ulOnvifLastError;
	unsigned long              m_ulMajorLastError;
	unsigned long              m_ulSubLastError;
	unsigned long              m_ulThirdLastError;

	unsigned int               m_iIpcCtrlUsedType;
	char                       m_strSaveServUri[128];

	bool                       m_bOnvifCtrlEnable;

	int                        m_run_lines;
	int                        m_2run_lines;

	bool                       m_bFastCheckVideoLoss;
	int                        m_iFastCheckMasterCount;
	int                        m_iFastCheckMastrePrevCount;
	unsigned int               m_iFastCheckPrevTime;
};

#endif