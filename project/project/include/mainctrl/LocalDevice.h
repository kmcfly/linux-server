/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : Ԭʯά
** Date         : 2008-12-25
** Name         : LocalDevice.h
** Version      : 1.0
** Description  :
** Modify Record:
1:
***********************************************************************/
#ifdef __ENVIRONMENT_VIDEO_PLAY__
#include "VideoLocalDevice.h"
#else
#ifndef _LOCAL_DEVICE_H_
#define _LOCAL_DEVICE_H_

#include "Product.h"
#include "ConfigEx.h"
#include "KeyProc.h"

#ifdef __ENVIRONMENT_LINUX__
#include "FBInterface.h"
#include "Mcu8952.h"
#include "ExternalKeyboard.h"
#include "SerialPort.h"
#include "dvr_func.h"
#else
#include "FBInterface.h"
#endif

#ifdef __ENVIRONMENT_WIN32__
class CDecodeDisplay2;

typedef struct _ext_dvr_ip_in_buffer_param
{
	long			chipId;
	unsigned long	dataLen;
	long			*pStatus;
	unsigned char	*pData;
}ext_dvr_ip_in_buffer_param;

#define EXT_DVR_MAX_CHANNEL 128

#endif

/*
typedef struct _rtc_time
{
	unsigned int second;
	unsigned int minute;
	unsigned int hour;
	unsigned int dayofweek;
	unsigned int day;
	unsigned int month;
	unsigned int year;
}RTC_TIME;
*/

//ÿ��dvr����һ���������е��źţ�����Ӳ��ȷ��
typedef enum _device_led_status_
{
	LED_STATUS_HDD		= 0x00000001,	//����װ��Ӳ��
	LED_STATUS_RECORD	= 0x00000002,	//���κ����ʵ�¼��ָʾ����
	LED_STATUS_PLAY		= 0x00000004,	//������ڻط�ͼ��ָʾ����
	LED_STATUS_NET		= 0x00000008,	//����пͻ�������ָʾ����
	LED_STATUS_BACKUP	= 0x00000010,	//������κα�������ʱָʾ����
	LED_STATUS_VIDEO_LOSS = 0x00000011, //2U����и���ͨ��VIDEO LOSS ָʾ��
}DEVICE_LED_STATUS;

//
//typedef enum _flash_param_type
//{
//	FLASH_PARAM_MAC = 0,          //��flash�ж�дmac��ַ
//	FLASH_PARAM_HARDWARE = 1,   //��flash�ж�дӲ�����汾��
//	FLASH_PARAM_PRODUCT_B = 2,       //��flash�ж�д��Ʒ�Ӱ汾�š�����Ʒ��ID�Ӽ����ж�ȡ��
//	FLASH_PARAM_DISPLAY = 3,      //��flash�ж�д��ʾģʽ��
//}FLASH_PARAM_TYPE;

#define CRYTOINFO_OLD_FLAG 0x3
#define CRYTOINFO_NEW_FLAG 0xa

typedef enum _crytoinfo_status
{
	CRYTOINFO_STATUS_NULL,
	CRYTOINFO_STATUS_OLD,
	CRYTOINFO_STATUS_NEW,
} CRYTOINFO_STATUS;

class CLocalDevice
{
public:
	enum
	{
		GET_FRAME_SUCC,		//��ȡ���ݳɹ�
		GET_FRAME_FAIL,		//��ȡ����ʧ��
		FRAME_BUF_FULL,		//��������
	};

	~CLocalDevice();
	static CLocalDevice * Instance();
	//////////////////////////////////////////////////////////////////////////
#ifdef __ENVIRONMENT_LINUX__
	bool Initial(ext_dvr_input_param & parm, int videoOutputNum, int netVideoInputNum, DWORD & sensorInputType, bool bMCU = true,unsigned char* pCvbs=NULL);
	bool SetSystemTime(unsigned long time, bool bSetOS = true);
	bool FeedDog();
	void KillDog();
#else
	bool Initial(CDecodeDisplay2 *pDecodeDisplay, HWND hPlayWnd, int localVideoInputNum, int videoOutputNum, int netVideoInputNum);
#endif

	void Quit();
	//////////////////////////////////////////////////////////////////////////
	bool Start();
	void Stop();
	//////////////////////////////////////////////////////////////////////////
	//�л��ֳ����ط���ʾģʽ���絥���棬4����
	void ChangeDisplayMode(const DISPLAY_DEV_INFO* pDispDevInfo, DISPLAY_ATTRIB *pAttrib, unsigned char chnnNum, unsigned long* pSplitMode, int splitNum, unsigned char* pVisibleChnn, unsigned char bFilter = false);
	bool HaveSpot();
	void ChangeDisplaySpot(int chnn); //�л�SPOT
	void KeyFrame(unsigned char chnn, bool bLocal = true);
	void SetAudioLiveChannel(bool bOpen, unsigned char chnn); //�����ֳ���Ƶͨ��
	void SetAudioVolume(int volume);
	void RecoverAudioLiveChannel(); //�ڽ���ط�ʱ����ر��ֳ��������ô˺������Իָ���ԭ����״̬��
	void SetAudioAuto(bool bOpen);
	void SetLEDStatus(DEVICE_LED_STATUS status, bool bOn);
	void SetLEDStatusEx(DEVICE_LED_STATUS status, unsigned long statusValue);

	bool ChangeOutpuDevice(int device, int width, int height);

	void SetCurMainDevice(int devColor);
	int GetCurMainDevice();
	unsigned long GetCurMainDeviceColor( );
	unsigned long GetCurMainDeviceDefaColor();
	void SetMainDeviceColor(unsigned long devColor);
	void SetMainDeviceColor();
	void RecoverMainDeviceColor();

	unsigned long GetImageDefaultColor();
	unsigned char GetRealImageColor(unsigned char defaultColor, unsigned char setToColor);

	//////////////////////////////////////////////////////////////////////////
	bool GetVideoFormat(unsigned char chnn, BITMAPINFOHEADER &bitmap, bool bLocal = true);
	bool GetAudioFormat(WAVEFORMATEX &wave);
	bool GetKeyInput(KEY_VALUE_INFO &keyValue);
	void SetResolution(unsigned short width, unsigned short height);

	int GetVideo_F(BYTE chnn, FRAME_INFO_EX &frameInfo);
	int GetVideo_S(BYTE chnn, FRAME_INFO_EX &frameInfo);
	int GetJPEGStream(FRAME_INFO_EX &frameInfo);
	int GetAudio(BYTE chnn, FRAME_INFO_EX &frameInfo);
	void ReleaseBuf(FRAME_INFO_EX &frameInfo);
	void ReleaseJPEGBuf(FRAME_INFO_EX &frameInfo);
	int  ClearBuffer();
	//////////////////////////////////////////////////////////////////////////
	void GetMotionStatus(ULONGLONG &status);
	void GetVideoLossStatus(ULONGLONG &status);
	unsigned long GetVideoLossStatus()
	{
		if (0 < m_localVideoInputNum)
		{
			unsigned long vl = m_videoLossStatus;
			vl &= ((0x01 << m_localVideoInputNum) - 1);
			vl |= ~((0x01 << (m_localVideoInputNum + m_netVideoInputNum)) - 1);
			return vl ;
		}
		else
		{
			return	0;
		}
	}

	void GetSensorInStatus(ULONGLONG &status);
	void SetSensorOutStatus(unsigned char chnn, bool bOpen);
	void SetBuzzerStatus(bool bOpen);
	void SetVideoFormat(VIDEO_FORMAT videoFormat);
	void SetOtherAlarm(OTHER_ALARM_TYPE type, bool bOpen);
	void GetOtherAlarm(ULONGLONG & status);
	//////////////////////////////////////////////////////////////////////////
	void SetVideoSize(unsigned char chnn, unsigned long size, bool bLocal = true);
	unsigned long GetVideoSize(unsigned char chnn, bool bLocal = true);
	//void SetFrameRate(unsigned char chnn, unsigned char rate, bool bLocal = true);
	//void SetVideoQuality(unsigned char chnn, VIDEO_ENCODE_MODE type, int bitStream, int quality, bool bLocal = true);
	void SetFrameRateQuality(unsigned char chnn, unsigned char rate,VIDEO_ENCODE_MODE type, int bitStream, int quality, bool bLocal = true);
	void SetMotionInfo(unsigned char chnn, const MOTION_DETECT_INFO *pInfo);
	void SetImageColor(unsigned char chnn, unsigned long color);
	void InitOSDTimeStamp(OSD_ATTRIB *pAttrib);
	void RefreshTimeStamp(const std::string & strTime);
	void SetOSDTimeStamp(unsigned char chnn, const OSD_INFO *pOSDInfo);
	void SetOSDCamName(unsigned char chnn, bool bShow, OSD_ATTRIB *pAttrib = NULL);
	void SetCamCover(unsigned char chnn, const CAMERA_COVER_INFO *pCoverInfo);
	void Zoom(bool bZoom, int x = 0, int y = 0, int w = 0, int h = 0);
	void SetWaterMark(int chnn, unsigned char* key, int key_len, unsigned char* user_info, int user_len);
	void CleanWaterMark(int chnn);
	void SetDeviceNomenuImage(unsigned char *pData, unsigned long width, unsigned long height);
	//////////////////////////////////////////////////////////////////////////
    void	PlaybackStop(ULONGLONG stopCH = (ULONGLONG)-1);          //�ú���ֻ��ֹͣ�طţ�û���л����ֳ�ģʽ���ܡ�
	bool	PlaybackStart(ULONGLONG chnnBits, bool bRestart=false); //�ú��������طţ�����û���Զ��л����ط�ģʽ�Ĺ���
	bool	PlaybackPause(bool bPause);
	bool	PlaybackPause(unsigned long ulChnn, bool bPause);

	bool	PlaybackSetBuf(FRAME_INFO_EX *pFrameInfo);
	bool	PlaybackGetBuf(FRAME_INFO_EX *pFrameInfo);
	int     CheckDecodeError( );
	//////////////////////////////////////////////////////////////////////////
	int		GetTalkWay(FRAME_INFO_EX &frameInfo);	//��ȡ�Խ���������
	void	ReleaseTalkWayBuf(FRAME_INFO_EX &frameInfo);	//�ͷŶԽ���������
	void	TalkWayStop();	//ֹͣ�����Խ�
	bool	TalkWayStart(); //���������Խ�
	bool	TalkWaySetBuf(FRAME_INFO_EX *pFrameInfo);	//���������Խ�����
	bool	TalkWayGetBuf(FRAME_INFO_EX *pFrameInfo);	//��ȡ�����Խ����ݻ�����
	//////////////////////////////////////////////////////////////////////////
	//�ṩ��Hybrid
	long GetIPCDecChipNum();
	bool GetIPCBuffer(long chipId, unsigned char *&pBuffer, unsigned long &bufferSize, unsigned char *&pMinorBuffer, unsigned long &minorBufSize);
	bool IPCStart(unsigned long channelInfo, unsigned long minorChannelInfo, unsigned int* chip_id);
	bool IPCStop();
	bool IPCSetBufferData(ext_dvr_ip_in_buffer_param *pIPBufferParam);
	bool IPCMinorSetBufferData(ext_dvr_ip_in_buffer_param *pIPBufferParam);
	int GetDecLeftFrame(int channel);
	//////////////////////////////////////////////////////////////////////////
	//void SetSnapTime(int sec_tm);

	//void SetSnapCapability(int channelNum, int rate, int rateTime, VIDEO_SIZE videoSize); //����ץJPEG����
	//void SetEmailSnapEnable(int channel, bool bEnable);                                        //����ĳ��ͨ���Ƿ�ץJPEG
	//void SetPhoneSnapChannel(int channel, bool bEnable);                                   //�����Ƿ����ĳ��ͨ����ץjpeg����
	//void ChangeJPEGVideoSize(VIDEO_SIZE videoSize);
	//void ChangeJPEGVideoLevel(unsigned long videLevel);
	//void ChangeJPEGRate(unsigned long rate);


	//////////////////////////////////////////////////////////////////////////
	void    SetLcdScreemStatus(bool bOn);

	void	SetCamVisible(bool bVisible);

	bool SetTviPtzDeviceCtrl(int chn, BYTE* data, int len );
	bool GetTviPtzDeviceCtrl(int chn, BYTE* data, int *p_len );


	bool        GetMCUVersion(char *pVersion, int len);    //mcu�汾��
	static bool GetKernelVersion(char *pVersion, int len); //��ȡ����汾��
	static bool GetMacFromFlash(char *pMac, int len);      //��flash��mac��ַ
	static bool WriteMacToFlash(char *pMac, int len);      //��flashдmac��ַ
	
	static bool GetSNFromFlash(char *pSN, int len);//
	static bool GetSNFromFlash(char *pSN);
	static bool WriteSNToFlash(char *pSN, int len);//

	static bool GetHardWareVersion(char *pVersion, int len); //��flash��Ӳ���汾��
	static bool WriteHardWareVersion(char *pVersion, int len); // ��flashдӲ���汾��
	static unsigned long GetProductID();      //��ȡ����оƬ�еĲ�Ʒ���汾��
	static unsigned long GetProductSubID();   //��flash�ж�ȡ��Ʒ�Ӱ汾��
	static bool WriteProductSubID(char *pVersion, int len); //��flashд��Ʒ�Ӱ汾��
	static unsigned long GetDisplayModFromFlash( ); //��ȡflash�е���Ƶ���ģʽ
	static bool WriteDisplayModToFlash(char *pDisplay, int len); //��flashд��Ƶ���ģʽ
	static unsigned long GetVideoFormatFromFlash( ); //��ȡflashд��Ƶ��ʽ

	//zxx�¼�
	static bool WriteDisplayResToFlash(char *pRes, int len);//��flash��д�ֱ���
	static unsigned long GetDisplayResFromFlash();//��flash�ж�ȡ�ֱ���

	static bool WriteTimeZoneToFlash(char *pTimeZone, int len);//��flash��д��ʱ��
	static unsigned long GetTimeZoneFromFlash();		//��flash�ж���ʱ��

	static bool WriteVideoFormatToFlash(char *pFormat, int len); //��flashд��Ƶ��ʽ
	static bool WriteDDNSTypeToFlash(char *pDDNSName, int len); //��flashдDDNS�����
	static unsigned long GetDDNSTypeFromFlash( );//��ȡflashDDNS�����
	static bool SetBootImage(char *pFileName);
	static bool WriteCryptoInfo(char *pType, int len); //д֧�ֵ����԰�
	static int  GetSupportLanguage( ); //��ȡ֧�ֵ����԰�
	static bool WriteKeyBoardNameToFlash(char *pName, int len);
	static int  GetKeyBoardNameFromFlash(char *pName, int len);
    static bool WriteCrytoInfoNew(unsigned long languageBits, unsigned char productSubID);
	static CRYTOINFO_STATUS  CrytoInfoStatus();
	static unsigned long GetSupportLanguageNew();
	static unsigned long GetProductSubIDNew();
	static bool WriteFuncCustomToFlash(char* pFunc, int len);	//��flashд֧�ֵĹ���
	static bool	GetFuncCustomFromFlash(char* pFunc, int len);	//��ȡ֧�ֵĹ���
	static bool WriteCompanyNameToFlash(char *pCompanyName, int len);
	static bool GetCompanyNameFromFlash(char *pCompanyName, int len);
    
	static bool IsUse1104A1108Color();
	static int GetHdChipType();
	static bool Is2316SSNewPCB();

	static bool WriteNICTypeToFlash(char *pNICType, int len);
	static int  GetNICTypeFromFlash();
	static void AdjustCVBSVo(unsigned long adjust);

	static void SetEncodeSize(int encodeSize);
	static int GetEncodeSize();

	void SetChosenPos(unsigned long x,unsigned long y);
	void SendInitChnn(unsigned long chnn);

#ifdef __SUPPORT_OSDTWOLINES__
	void GetVideoSize(int chnn, int &width, int &height);
#endif

	unsigned long CurrentDispChannelNum(){return m_dispChannelNum;}

protected:
private:
	CLocalDevice();
#ifdef __ENVIRONMENT_LINUX__
	static PUB_THREAD_RESULT PUB_THREAD_CALL MouseThread(void *pParam);
	int		MouseProc();
	bool InitSystemTime();
#endif

	static int CheckKeyBoardName(char *pName);


	//////////////////////////////////////////////////////////////////////////
	PRODUCT_TYPE    m_productType;
	unsigned char	m_localVideoInputNum;
	unsigned char	m_netVideoInputNum;
	unsigned char	m_videoInputNum;
	unsigned char	m_videoOutputNum;

	//////////////////////////////////////////////////////////////////////////
	CConfigEx		*m_pConfig;
	//////////////////////////////////////////////////////////////////////////
	ULONGLONG		m_motionStatus;
	ULONGLONG		m_sensorInStatus;
	ULONGLONG		m_otherAlarmStatus;
	//////////////////////////////////////////////////////////////////////////
#ifdef __ENVIRONMENT_LINUX__
	VIDEO_FORMAT		m_videoFormat;       //��ʽ
	VIDEO_SIZE			*m_pLocalVideoSize;  //����ͨ���ֱ��ʣ�ÿ��ͨ�����Բ�ͬ
	VIDEO_SIZE			*m_pLocalNetVideoSize; //����ͨ�����紫��ֱ��ʣ�ͨ��ͨ�����Բ�ͬ��˵����ͨ����D1������������Դ�CIF��Ҳ���Դ�D1
	PUB_thread_t		m_mouseProcID;
	bool				m_bMouseProc;
	bool				m_bSupportMCU;

	//int					m_rtcFd;

	OSD_INFO			*m_pOSDInfo;
	OSD_ATTRIB			m_timeStampAttrib;
	
#else
	unsigned char	*m_pData;
	unsigned char	m_channl;
	unsigned long	m_dataLen;
	unsigned long	m_index[MAX_CHNN_NUM];
	LONGLONG		m_preFrameTime[MAX_CHNN_NUM];
	//	CVideoEncode	m_videoEncoder;
	unsigned char	*m_pVideoData;
	long			m_videoSize;
	CDecodeDisplay2	*m_pDataDisplay;
	ULONGLONG		m_pbChannelBits;	
	HWND            m_hPlaybackWnd;
#endif

	unsigned long		m_videoLossStatus;
	static CPUB_Lock m_keyLock;
	static std::list<KEY_VALUE_INFO> m_keyValueList;
	unsigned short m_displayWidth;   
	unsigned short m_displayHeight;

	int m_preLiveAudioChnn;  //���ڷ�ֹ��ο���ͬһ��ͨ������Ƶ��
	bool m_bOpenAudio;        //��־�Ƿ�������Ƶ���ܡ�
	bool m_bAudioAuto;
	int  m_bLiveOneChnn;      //���ڼ�¼�����ֳ���ͨ����ʾ������£���ʾ���ǵڼ�ͨ����
	                          //-1��ʾ�����ֳ���ͨ��������ʾ���ڻطŽ��档
	bool m_bTalking;			//��־�Ƿ������˶Խ�����������˶Խ������ֳ��������ָܻ�


	//int m_snapRate;       //��ץͼ�й�ϵ�ı���
	//int m_snapRateTime;
	//VIDEO_SIZE m_snapVideSize;
    //ULONGLONG  m_phoneSnapCH;
	//ULONGLONG  m_emailSnapCH;
	//int m_phoneRaiseSnapChnn;
	//unsigned long m_phoneRaiseChnnRate;
	//unsigned long m_jpegLevel;
	//CPUB_Lock  m_setSnapRateLock;


#ifdef __ENVIRONMENT_LINUX__
	static ext_dvr_product_info *m_pCryptoInfo;
#endif

	bool  m_bPlaybacking;

	int           m_curDevice;
	unsigned long m_curDevColorForPlayBack;

	bool		m_bCamVisible;

	unsigned short	m_waveFormat;
	static bool m_bUse1104A1108Color;
	static bool m_b2316SSNewPCB;

	CPUB_Lock     m_sensorStatLock;
	unsigned char m_bSensorOutStatus[16];
	int           m_SenorOutOpenCout[16];

	//���flash��ʱ����

	static VIDEO_FORMAT		m_videoFormatFlash;
	static VIDEO_OUT_TYPE	m_videoOutTypeFlash;
	static VGA_RESOLUTION	m_outResolutionFlash;
	static int				m_encode960H;
	char					m_bright;
	char					m_hue;
	char					m_sat;
	char					m_contrast;

	unsigned long			m_imageDefaultColor;
	bool					m_bInitial;
	unsigned long			m_dispChannelNum;
};
#endif //_LOCAL_DEVICE_H_

#endif

//end
