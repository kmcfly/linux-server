#ifndef __TVT_DEV_INTERFACE_H__
#define __TVT_DEV_INTERFACE_H__

#include "PUB_common.h"
#include "DevInterface.h"
#include "APIMsgDistributor.h"
#include "MarkupSTL.h"
#include <map>
#include <vector>
#include <string>
#include <map>

class CTvtDevInterface : public CDevInterface, public CAPIMsgDistributorInterface
{
public:
	CTvtDevInterface();
	~CTvtDevInterface();
/*
	*  该接口用来网络库不能够处理的http命令
	*/
	virtual tint32 WebProcExt( const std::string& strWebContent, NETNS::HTTP_STATUS_CODE& status, std::string& strWebReponse) ;
	virtual tint32 NetProcExt( const char* pContent, tint32 dwLen, char *& pszRespXml, tint32& nRespLen, NETNS::HTTP_STATUS_CODE& status);
	virtual void  ParseFailedResource( const char* pUrl, char*& pResourceExt, tuint32 &dwRetExtLen);
	virtual void DelResponseBuff(char *&pszRespXml);
	virtual NETNS::DEV_RET GetChnList( const char* pUser, tuint32* pChnID, tuint32 dwChnSize, tuint32& dwRetNeedSize);
	/*
	* support function
	*/
	virtual NETNS::DEV_RET SupportIntegratedPTZ( bool &bSupport);
	virtual NETNS::DEV_RET SupportRS485PTZ( bool &bSupport);
	virtual NETNS::DEV_RET SupportSDCard( bool &bSupport);

	/*
	* system
	*/
	virtual NETNS::DEV_RET GetUserInfo( NETNS::DEV_USER_INFO *pCfg, tuint32 nSize, tuint32& nNeedSize) ;
	virtual NETNS::DEV_RET GetDeviceInfo( const char* pUser, NETNS::DEV_INFO& info) ;
	virtual NETNS::DEV_RET GetSystemTime( const char* pUser, NETNS::DEV_SYSTIME& sysTime) ;
	virtual NETNS::DEV_RET SetSystemTime( const char* pUser, const NETNS::DEV_SYSTIME& sysTime) ;

	virtual NETNS::DEV_RET SetCurTime( const char* pUser, const NETNS::DEV_DATETIME& datetime) ;

	/*
	* Stream Capabilities
	*/
	// camara
	virtual NETNS::DEV_RET GetImageCaps( const char* pUser, tuint32 nChnID, NETNS::DEV_CAMERA_PARAM_CAPS &caps) ;
	virtual NETNS::DEV_RET GetImageParam( const char* pUser, tuint32 nChnID, NETNS::DEV_CAMERA_PARAM &param) ;
	virtual NETNS::DEV_RET SetImageParam( const char* pUser, tuint32 nChnID, NETNS::DEV_CAMERA_PARAM &param) ;
	virtual NETNS::DEV_RET GetSnapshot( const char* pUser, tuint32 nChnID, char*& pPic, tuint32& nPicSize) ;
	 

	// stream
	virtual NETNS::DEV_RET GetStreamCaps( const char* pUser, tuint32 nChnID, tuint32 nStreamIdx, NETNS::DEV_SREAM_CAPS& caps) ;
	virtual NETNS::DEV_RET GetStreamName( const char* pUser, tuint32 nChnID, tuint32 nStreamIdx, char* pStreamName, tuint32 nNameLen, tuint32& nNeedNameLen) ;
	virtual tuint32 GetStreamCnt( tuint32 nChnID ) ;
	virtual tint32 SupportAudioEncode( tuint32 nChnID, NETNS::DEV_AUDIO_ENCODE encode) ;
	virtual NETNS::DEV_RET SetAudioEncode(  const char* pUser, tuint32 nChnID, const NETNS::DEV_AUDIO_ENCODE encode) ;
	virtual NETNS::DEV_RET GetVideoStream( const char* pUser, tuint32 nChnID, tuint32 nStreamIdx, NETNS::DEV_SREAM_CFG &cfg) ;
	virtual NETNS::DEV_RET SetVideoStream( const char* pUser, tuint32 nChnID, tuint32 nStreamIdx, const NETNS::DEV_SREAM_CFG& cfg) ;
	NETNS::DEV_RET RequestKeyFrame( const char* pUser, tuint32 nChnID);
	virtual bool RtspNeedAuthorization();

	// OSD
	virtual NETNS::DEV_RET GetOsdTimestamp( const char* pUser, tuint32 nChnID, NETNS::DEV_OSD_TIMESTAMP& cfg) ;
	virtual NETNS::DEV_RET GetOsdChnName( const char* pUser, tuint32 nChnID,  NETNS::DEV_OSD_DEVICE_NAME& cfg) ;
	virtual NETNS::DEV_RET SetOsdTimestamp( const char* pUser,  tuint32 nChnID, const NETNS::DEV_OSD_TIMESTAMP cfg) ;
	virtual NETNS::DEV_RET SetOsdChnName( const char* pUser, tuint32 nChnID,  const NETNS::DEV_OSD_DEVICE_NAME cfg) ;

	// privacy mask
	virtual NETNS::DEV_RET GetPrivacyMaskCfg( const char* pUser, tuint32 nChnID, NETNS::DEV_OSD_PRIVACY_MASK* pCfg, tuint32 nSize, tuint32& nNeedSize);
	virtual NETNS::DEV_RET SetPrivacyMaskCfg( const char* pUser, tuint32 nChnID, NETNS::DEV_OSD_PRIVACY_MASK* pCfg, tuint32 nSize) ;

	/*
	* protocol
	*/
	virtual NETNS::DEV_RET PtzGetCaps( const char* pUser, tuint32 nChnID, NETNS::DEV_PTZ_CAPS& cfg) ;
	virtual NETNS::DEV_RET PtzGetCaps( const char* pUser, tuint32 nChnID, NETNS::DEV_DOM_PTZ_CAPS& cfg) ;
	virtual NETNS::DEV_RET PtzGetProtocolCfg( const char* pUser, tuint32 nChnID, NETNS::DEV_PTZ_PROTO& cfg) ;
	virtual NETNS::DEV_RET PtzSetProtocolCfg( const char* pUser, tuint32 nChnID, const NETNS::DEV_PTZ_PROTO& cfg);

	// ptz ctrol
	virtual NETNS::DEV_RET PtzCtrl( const char* pUser, tuint32 nChnID, NETNS::DEV_PTZ_CTRL_TYPE action, tuint32 nSpeed) ;

	// preset
	virtual NETNS::DEV_RET PtzGotoPreset( const char* pUser, tuint32 nChnID, tuint32 nPresetID) ;
	virtual NETNS::DEV_RET PtzGetPresets( const char* pUser, tuint32 nChnID, NETNS::DEV_PTZ_PRESET* pPresets, tuint32 nNum, tuint32& nNeedSize);
	virtual tuint32 PtzGetPresetsMaxNum( tuint32 nChnID ) ;
	virtual NETNS::DEV_RET PtzAddPreset( const char* pUser, tuint32 nChnID, const char* pName) ;
	virtual NETNS::DEV_RET PtzModifyPresetName( const char* pUser, tuint32 nChnID, tuint32 nPresetID, const char* pName) ;
	virtual NETNS::DEV_RET PtzDeletePreset( const char* pUser, tuint32 nChnID, tuint32 nPresetID) ;
	virtual NETNS::DEV_RET PtzModifyPresetPosition( const char* pUser, tuint32 nChnID, tuint32 nPresetID) ;

	// cruise
	virtual NETNS::DEV_RET PtzGetCruises( const char* pUser, tuint32 nChnID, NETNS::DEV_PTZ_CRUISE* pCruises, tuint32 nSize, tuint32& nNeedSize);
	virtual NETNS::DEV_RET PtzGetCruise( const char* pUser, tuint32 nChnID, tuint32 nCruiseID, NETNS::DEV_PTZ_PRESET* pPreset, tuint32 nSize, tuint32& nNeedSize) ;
	virtual NETNS::DEV_RET PtzAddCruise( const char* pUser, tuint32 nChnID, const char* pCruiseName,  NETNS::DEV_PTZ_PRESET* pPreset, tuint32 nSize) ;
	virtual NETNS::DEV_RET PtzModifyCruise( const char* pUser, tuint32 nChnID, tuint32 nCruiseID, const char* pCruiseName,  NETNS::DEV_PTZ_PRESET* pPreset, tuint32 nSize ) ;
	virtual NETNS::DEV_RET PtzDeleteCruise( const char* pUser, tuint32 nChnID, tuint32 nCruiseID) ;
	virtual NETNS::DEV_RET PtzRunCruise( const char* pUser, tuint32 nChnID, tuint32 nCruiseID) ;
	virtual NETNS::DEV_RET PtzStopCruise( const char* pUser, tuint32 nChnID) ;

	/*
	* Motion Detection
	*/
	virtual NETNS::DEV_RET  GetMotionCfg( const char* pUser, tuint32 nChnID, NETNS::DEV_MOTION_CFG& cfg) ;
	virtual NETNS::DEV_RET  GetMotionSensitivity( const char* pUser, tuint32 nChnID, NETNS::DEV_RANGE& sen) ;
	virtual NETNS::DEV_RET  SetMotionCfg( const char* pUser, tuint32 nChnID, const NETNS::DEV_MOTION_CFG &cfg) ; 

	/*
	* Alarm
	*/
	virtual NETNS::DEV_RET GetAlarmInCfg( const char* pUser, tuint32 nChnID, NETNS::DEV_SENSOR_CFG &cfg) ;
	virtual NETNS::DEV_RET SetAlarmInCfg( const char* pUser, tuint32 nChnID, const NETNS::DEV_SENSOR_CFG &cfg) ;
	virtual NETNS::DEV_RET ManualAlarmOut( const char* pUser, tuint32 nChnID, bool bStatus) ;
	virtual NETNS::DEV_RET GetAlarmoutCfg( const char* pUser, tuint32 nChnID, NETNS::DEV_ALARMOUT_CFG &cfg) ;
	virtual NETNS::DEV_RET SetAlarmoutCfg( const char* pUser, tuint32 nChnID, const NETNS::DEV_ALARMOUT_CFG &Cfg) ;

	/*
	* TCP/IPV4
	*/
	virtual NETNS::DEV_RET GetNetBasicCfg( const char* pUser, NETNS::DEV_IPV4_CFG& cfg) ;
	virtual NETNS::DEV_RET SetNetBasicCfg( const char* pUser, const NETNS::DEV_IPV4_CFG& cfg) ;
	virtual NETNS::DEV_RET GetNetPppoeConfig( const char* pUser, NETNS::DEV_PPPOE_CFG& cfg) ;
	virtual NETNS::DEV_RET SetNetPppoeConfig( const char* pUser, const NETNS::DEV_PPPOE_CFG& cfg) ;

	virtual NETNS::DEV_RET GetPortCfg( const char* pUser, NETNS::DEV_NET_PORT& cfg) ;
	virtual NETNS::DEV_RET SetPortCfg( const char* pUser, const NETNS::DEV_NET_PORT& cfg) ;
	virtual NETNS::DEV_RET GetDdnsCfg( const char* pUser, NETNS::DEV_DDNS_CFG& cfg) ;
	virtual NETNS::DEV_RET SetDdnsCfg( const char* pUser, const NETNS::DEV_DDNS_CFG& cfg) ;

	/*
	* User Management
	*/
	virtual NETNS::DEV_RET ModifyPassword( const char* pUser, const char* pNewPassword ) ;

	/*
	*
	*/
	virtual NETNS::DEV_RET RebootDevice( const char* pUser) ;
	// param: dwRetNeedSize 返回实际通道数量

	/*
	*   Playback Management
	*/
	virtual NETNS::DEV_RET RecSearchByDay( const char* pUser, NETNS::DEV_DATE* pDateList, tuint32 dwListSize, tuint32& dwRetNeedSize) ;
	virtual NETNS::DEV_RET RecSearchByTime( const char* pUser, const NETNS::DEV_DATETIME& searchStart, const NETNS::DEV_DATETIME& searchStop
		, NETNS::DEV_REC_SEARCH* pRecList, tuint32 dwListSize, tuint32& dwRetNeedSize) ;

	/*
	*  smart interface                                                                    
	*/
	virtual tuint32 GetDevAttriSaveLen( NETNS::DEV_ATTRIBUTE_LEN type) ;

	void SetMaxChnnel( int maxChnn );

public:
	virtual int GetMessage(MESSAGE_DATA &msgData);
private:
	void ClearMsg(int msgType);
	bool WaitingForMsg(int msgType, int timeOut = 1000);
	bool WaitingForMsg(std::vector<int> &msgType, int timeOut = 1000);
	void GetResolution(int resolution, NETNS::DEV_RESOLUTION *res, unsigned int size, unsigned int &validSize, bool bPal);
	bool GetResolutionFromWH(unsigned long width, unsigned long height, int resolution);
	bool PtzCmdConvert(NETNS::DEV_PTZ_CTRL_TYPE ptzCtrlType, int &cmd);
	bool SaveConfig(const unsigned char *pData, unsigned long dataLen);
	bool CheckUserPass(const char *pXmlInfo);
	bool ParseXmlUserInfo(const char *pData, std::string &username, std::string &passwd);
	bool DaylightInfoToStr(DAYLIGHT_INFO *daylightInfo, char *pOut, unsigned int len);
	bool StrToDaylightInfo(const char *pIn, unsigned int len, DAYLIGHT_INFO *daylightInfo, char *timeZone);
	bool CalcTimeZoneIndex(std::string *pSrcTimeZone, int size, const char *pPattern, int &index);

private:
	CPUB_Lock				m_Lock;
	CPUB_Lock               m_langLock;

	CMarkupSTL				m_XmlParser;
	std::map<int, char *>	m_mapMsg;
	std::map<std::string, std::string>	m_AuthedUser;

	unsigned long			m_RuningCruiseID;

	int                     m_iMaxChannels;
};


#endif