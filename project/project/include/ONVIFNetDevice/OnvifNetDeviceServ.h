/*******************************************************************************
** Copyright (c) Tongwei Video Technology Co.,Ltd. All rights reserved.				
** AUTHOR		:	������	
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
   �ӿڹ�����������ʼ���豸�ӿڡ�
   �������������
   1��chnn��	��������豸��ͨ���ţ�
   2��IP��		���豸�������ַ��
   3��user��	���ڵ�¼���û����ƣ�
   4��netPort����¼�˿�
   5��httpPort��IE���Ӷ˿�
   6��ctrlPort�� ���ƶ˿�
   4��password�����ڵ�¼�����룻
   5��mac:		�����豸�����������ַ�����ڵ�¼��֤ʹ�á�
   ����ֵ���������سɹ�����ʧ�ܡ�
   ע: �ڸýӿ��ڲ�����Ҫ���Ե�¼��IP�豸�����ݵ�¼�Ľ�����ؽ����
   ע: �ڽӿ��ڲ�, �����ظ���ʼ��
   ע: �����豸�ӿ�, ������������״̬����Ӧ����
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
   �ӿڹ������� : ����ʼ��
   ************************************************************************/
   virtual void Quit() ;

   /************************************************************************
   �Ƿ���Ҫ������ʱ���������豸�����ò���
   ************************************************************************/
   //virtual bool IfNeedInitNetConfig(){return true;}

   /************************************************************************
   �ӿڹ������� : �������ݻ��������
   1����������NVR����.
   2���յ�����ͼ������(H264)��������.
   ************************************************************************/
   virtual void SetDataBufferMan(nd_in CIPDataBufferMan *pDataBufferMan);


   /***********************************************************************
   �޸��豸�󶨵�ͨ����
   ************************************************************************/
   virtual void SetBindChannel(nd_in long chnn);
   virtual long GetBindChannel() ;

   virtual void SetLocalVideoNum(long localNum){};
   virtual unsigned int GetLocalVideoNum(){ return 0;};

   /***********************************************************************
   �޸��豸�󶨵Ĵ�����ͨ����
   ************************************************************************/
   virtual void SetBindSensorChannel(nd_in long sensorChnn);
   virtual long GetBindSensorChannel();

   /***********************************************************************
   �õ��������豸��״̬,�Ͽ�������
   ************************************************************************/
   virtual long GetDevStatus() ;

	/***********************************************************************
	�鿴�����豸�Ƿ���������
	************************************************************************/
	virtual bool IsReConnectting();
	
   /************************************************************************
   �õ���ǰ����״̬, ND_SC_BUSY����, ��Ϊ���ܴ����������                                                                    
   ************************************************************************/
   virtual long IsReady() ;

   /************************************************************************
   �ӿڹ���������������Ƶ����Ƶ��������
   �������������
   1��video1��������MASK��
   2��video2��������MASK��
   3��video3���ֻ�����MASK��
   4��video4����Ƶ��MASK��
   ����ֵ���������سɹ�����ʧ�ܡ�
   ע����Ӧ��λΪ�㣬��ʾֹͣ��
   ************************************************************************/
   virtual bool RequestStream( nd_in ULONGLONG video1,
	   nd_in ULONGLONG video2,
	   nd_in ULONGLONG video3,
	   nd_in ULONGLONG audio) ;

   /************************************************************************
   �ӿڹ���������������Ƶ�ؼ�֡
   �������������
   1��video1��������MASK��
   2��video2��������MASK��
   3��video3���ֻ�����MASK��
   ����ֵ���������سɹ�����ʧ�ܡ�
   ע����Ӧ��λΪ�㣬��ʾֹͣ��
   ************************************************************************/
   virtual bool RequestKeyFrame(nd_in ULONGLONG video1, 
	   nd_in ULONGLONG video2,
	   nd_in ULONGLONG video3);

   /************************************************************************
   �ӿڹ�����������̨���ƽӿڡ�
   �������������
   ************************************************************************/
   virtual bool PTZControl(nd_in long chnn,
	   nd_in unsigned long cmdtype,
	   nd_in unsigned long ptzParam,
	   nd_in unsigned long speed,
	   nd_in unsigned long waiteTime) ;


   /************************************************************************
   �ӿڹ�����������ȡͼ��ɫ����Ϣ��
   ************************************************************************/
   virtual bool ColorControl(nd_in long chnn,
	   nd_in unsigned long cmdtype,
	   nd_in_out DD_VIDEO_COLOR& videoColor) ;

  /*
    function:
		���񣬷�ת������ģʽ������У�����Զ��Խ���ROI(����Ȥ��)
    input:
		pchar ���͵�����
	return:
	    �ɹ�true, ʧ�� false 
	*/

	virtual bool OtherControl(long chnn, unsigned long cmdtype, unsigned char *pChar);

   /************************************************************************
   �ӿڹ����������豸�ı�����Ϣ��
   �������������
   1��alarm��������Ϣ
   ����ֵ�������ޡ�
   ************************************************************************/
   virtual ULONGLONG GetMotionAlarm();
   virtual ULONGLONG GetSensorAlarm();
   virtual ULONGLONG GetVlossAlarm();
   virtual ULONGLONG GetOtherAlarm();


   /************************************************************************
   �ӿڹ������� : ��������
   1��
   ************************************************************************/
   virtual long SetConfig( nd_in long chnn,
	   nd_in unsigned long itemID,
	   nd_in const unsigned char* pData,
	   nd_in unsigned long dataLen,
	   nd_in bool bImmediately);

   virtual long FlushConfig();

   /************************************************************************
   ����ʱ��
   ************************************************************************/
   virtual long SetDateTime(nd_in const DD_TIME& ddTm);


   /************************************************************************
   ��ȡ�豸֧�ֲ�����Ϣ
   ************************************************************************/
   virtual long GetDevInfo(nd_in long videoIndex, nd_out ND_DEV_INFO& ndDevInfo);

   virtual long GetEncodeInfo(char * pVideoEncodeInfo, int iVideoEncodeInfoLen);

   /************************************************************************
   �ӿڹ������� : ������ 
   1��������Ϊ�������, ������Ϊ���ش���
   ************************************************************************/
   virtual bool GetLastError(nd_out std::string& strErr);
   virtual long GetLastError();
   virtual void SetLastError(nd_in long err);

   /************************************************************************
   ÿ���ӵ���һ�Σ�֪ͨ��ǰʱ�䣬ģ����ô���Ϊ��ʱ��
   ************************************************************************/
   virtual void OnTimer(unsigned long curTime);

   /***********************************************************************
   �������糬ʱʱ��,��λms
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

   //��ʱͬ��IPC��ʱ��
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

   bool							m_bPal;		//�Ƿ�ΪPAL�ƣ�true����PAL�ƣ�false����NTSC��

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

	//�ڶ�����ֻ������CIF
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