#ifndef __TVT_IOTC_DEVICE__
#define __TVT_IOTC_DEVICE__

#ifndef DVR_3_0
#define  DVR_3_0
#endif
#ifdef DVR_3_0
#include "PUB_common.h"
#include "ConfigEx.h"
#include "ReclogManEx.h"
#else
#include "TVT_PubCom.h"
#endif
#include "IOTCAPIs.h"
//#include "RDTAPIs.h"
#include "AVAPIs.h"
#include "AVIOCTRLDEFs.h"
#include "AVFRAMEINFO.h"
#include "FrameData.h"
#include <string>
#include <map>
#include <list>
#include <bitset>


const unsigned int IOTC_MAX_CLIENT_NUM = 64;
const unsigned int SERVTYPE_STREAM_SERVER = 16;
typedef enum
{
	IOTC_STREAM_TYPE_VIDEO = 0,
	IOTC_STREAM_TYPE_AUDIO = 1,
}IOTC_STREAM_TYPE;

typedef enum
{
	IOTC_QUALITY_UNKNOWN	=	0x00,
	IOTC_QUALITY_MAX		=	0x01,// > 512kbps
	IOTC_QUALITY_HIGH		=	0x02,// 256 - 512
	IOTC_QUALITY_MIDDLE		=	0x03,// 128 - 256
	IOTC_QUALITY_LOW		=	0x04,// 64 - 128
	IOTC_QUALITY_MIN		=	0x05,// < 64
}IOTC_QUALITY_LEVEL;



typedef struct
{
	unsigned int	resolution;
	unsigned short	frameRate;
}QUALITY_LEVEL;

const QUALITY_LEVEL cQualityLevel[6] =
{
	{0, 0},
	{1, 30},
	{2, 15},
	{3, 15},
	{4, 10},
	{0, 0}
};

const char IOTC_USER_NAME[] = {"admin"};

typedef struct _iotc_session_key_
{
	int		SID;
	int		avChannel;
	_iotc_session_key_()
	{
		SID = 0;
		avChannel = 0;
	}
	_iotc_session_key_(int sid, int avChann)
	{
		SID = sid;
		avChannel = avChann;
	}
	bool operator < (const _iotc_session_key_ &sessionKey) const
	{
		if (SID == sessionKey.SID)
		{
			return avChannel < sessionKey.avChannel;
		}
		return SID < sessionKey.SID;
	}
	bool operator == (const _iotc_session_key_ & sessionKey) const
	{
		printf("IOTC_SESSION_KEY %d %d   --  %d %d\n", SID, avChannel, sessionKey.SID, sessionKey.avChannel);
		return (SID == sessionKey.SID && avChannel == sessionKey.avChannel);
	}

}IOTC_SESSION_KEY;

typedef struct _iotc_cmd_key_
{
	int		SID;
	int		avChannel;
	int		IOCtrlType;
	_iotc_cmd_key_()
	{
		SID = 0;
		avChannel = 0;
		IOCtrlType = 0;
	}
	_iotc_cmd_key_(int sid, int avChann, int ioCtrlT)
	{
		SID = sid;
		avChannel = avChann;
		IOCtrlType = ioCtrlT;
	}
	bool operator < (const _iotc_cmd_key_ &CMDKey) const
	{
		if (SID == CMDKey.SID)
		{
			if (avChannel == CMDKey.avChannel)
			{
				return IOCtrlType < CMDKey.IOCtrlType;
			}
			else
			{
				return avChannel < CMDKey.avChannel;
			}
		}
		else
		{
			return SID < CMDKey.SID;
		}
	}

}IOTC_CMD_KEY;

class CTVTIOTCDevice
{
public:
	struct RECV_IOCMD_PARAM
	{
		CTVTIOTCDevice *pClass;
		PUB_thread_t	hIORecvTheadID;
		bool			bIORecvThreadRun;
		int SID;//需要作为参数传入线程，所以即使是map的key，也在这里添加了
		int iotcChannel[IOTC_MAX_CLIENT_NUM];
		int avChannel[IOTC_MAX_CLIENT_NUM];
		int tvtChannel[IOTC_MAX_CLIENT_NUM];//不好的设计:把tvtChannel和avChannel绑定在一起，
											//因为客户端传过来的SMsgAVIoctrlAVStream是无效的，没有传有效的channel值
	};
	struct SEND_IOCMD_PARAM
	{
		CTVTIOTCDevice *pClass;
		PUB_thread_t	hIOSendTheadID;
		bool			bIOSendThreadRun;
		int				avChannelID;
		unsigned int	IOCtrlType;
		const char		*pIOCtrlData;
		int				IOCtrlDataSize;
	};
	struct AVCHANNEL_START_PARAM
	{
		CTVTIOTCDevice *pClass;
		PUB_thread_t	hAVStartTheadID;
		bool			bAVStartThreadRun;
		int				SID;
		int				iotcChannel;
		int				clientChannel;
	};
	struct IOTC_SESSION_PARAM
	{
		int				rdtChannel;
		int				avChannel;
		int				tvtChannel[2];//video audio;
		bool			bNeedWaiKeyFrame;
	};
	
public:
	static CTVTIOTCDevice *Instance();
	~CTVTIOTCDevice();
	bool Initial(const char *pUID, const char *pUserName, const char *pPassWd);
	bool Start();
	bool Stop();
	int GetFrameData(CFrameData *pFrameData);
	void ClearData(unsigned long dwStreamID, unsigned long dwChannel);
#ifdef DVR_3_0
	void SetReclogMan(CReclogManEX *pReclogMan);
#endif

private:
	CTVTIOTCDevice();
	bool StartLoginProc();
	bool StartListenProc();
	bool StartRecvIOCmdProc(struct RECV_IOCMD_PARAM *pParam);
	bool StartFrameProc();
	bool StartSendIOCmdProc(struct SEND_IOCMD_PARAM *pParam);
	bool StartAVStartProc(struct AVCHANNEL_START_PARAM *pParam);
	bool StartRecycleProc();
	bool StopRecycleProc();
	bool StopListenProc();
	bool StopRecvIOCmdProc();
	bool StopRecvIOCmdProc(int SID);
	bool StopFrameProc();
	bool StopAVStart(int SID);

	// create thread to login because without WAN still can work on LAN
	static PUB_THREAD_RESULT PUB_THREAD_CALL LoginThread(void *pParam);
	void LoginProc();

	static PUB_THREAD_RESULT PUB_THREAD_CALL RecycleThread(void *pParam);
	void RecycleProc();

	static PUB_THREAD_RESULT PUB_THREAD_CALL ListenThread(void *pParam);
	void ListenProc();

	static PUB_THREAD_RESULT PUB_THREAD_CALL RecvIOCmdThread(void *pParam);
	void RecvIOCmdProc(struct RECV_IOCMD_PARAM *pParam);

	static PUB_THREAD_RESULT PUB_THREAD_CALL FrameThread(void *pParam);
	void FrameProc();

	static PUB_THREAD_RESULT PUB_THREAD_CALL SendIOCmdThread(void *pParam);
	void SendIOCmdProc(struct SEND_IOCMD_PARAM *pParam);

	static PUB_THREAD_RESULT PUB_THREAD_CALL AVStartThread(void *pParam);
	void AVStartProc(struct AVCHANNEL_START_PARAM *pParam);

	static int __stdcall AuthCallBackFunc(char *pUser, char *pPass);
	void HandleIOCtrlCmd(int SID, int avChannel, const char *pBuf, int type);
	void SaveFrame(CFrameData *pFrame);

	void InitialFuncMap();
	int IOCtrlIPCamStart(int SID, int avChannel, const char *pBuf);
	int IOCtrlIPCamStop(int SID, int avChannel, const char *pBuf);
	int IOCtrlIPCamAudioStart(int SID, int avChannel, const char *pBuf);
	int IOCtrlIPCamAudioStop(int SID, int avChannel, const char *pBuf);
	int IOCtrlIPCamDevInfoReq(int SID, int avChannel, const char *pBuf);
	int IOCtrlIPCamSetStreamCtrlReq(int SID, int avChannel, const char *pBuf);
	int IOCtrlIPCamDevInfoResp(int SID, int avChannel, const char *pBuf);
	int IOCtrlIPCamSetStreamCtrlResp(int SID, int avChannel, const char *pBuf);
	int IOCtrlGetSupportStreamReq(int SID, int avChannel, const char *pBuf);
	int IOCtrlSetPassWdReq(int SID, int avChannel, const char *pBuf);
	int IOCtrlPtz(int SID, int avChannel, const char *pBuf);

#ifdef DVR_3_0
	unsigned int GetCurrTs(FRAME_INFO_EX *pFrame);
#else
	unsigned int GetCurrTs(TVT_DATA_FRAME *pFrame);
#endif
	void StopTransFrame(int SID);
	void StopTransFrame(IOTC_SESSION_KEY sessionKey);
	void StopTransFrame(IOTC_SESSION_KEY sessionKey, IOTC_STREAM_TYPE streamType);
	int SendIOCmd(int SID, int avChannel, unsigned int nIOCtrlType, const char *pIOCtrlData, int nIOCtrlDataSize);
	bool CheckProcessProc(int SID);
	int SendFrameData(std::list<CFrameData*>* pFrameList, FRAMEINFO_t &frameInfo, int avChannel, int &sendLen);
	int ChangeQuality(unsigned int resolution, unsigned short rate, int channel);
	void Recycle();
	static bool __stdcall CheckPassWd(const char *pPassWd, unsigned int passLen);
	//void WriteSysLog(unsigned long clientID, LOG_TYPE type);
	int GetProductInfo();
	bool EnterConfig();
	bool ChangePassWd(const char *pOldPass, unsigned int oldPassLen, const char *pNewPass, unsigned int newPassLen);
	int GetTVTChannel(int SID, int avChannel);

	bool CheckDropFrame(IOTC_SESSION_KEY sessionKey, std::list<CFrameData*>* pFrameList, CFrameData *pFrameData);
	bool DropFrame(std::list<CFrameData*>* pFrameList, unsigned int &dropedSize );
private:

	bool				m_bInit;

	PUB_thread_t		m_hLoginThreadID;
	bool				m_bLoginThreadProc;
	PUB_thread_t		m_hListenThreadID;
	bool				m_bListenThreadProc;
	PUB_thread_t		m_hFrameThreadID;
	bool				m_bFrameThreadProc;
	PUB_thread_t		m_hRecycleThreadID;
	bool				m_bRecycleThreadProc;

	CPUB_Lock			m_lockMapSession;;
	CPUB_Lock			m_lockMapFrame;
	CPUB_Lock			m_lockMapRecvIOCmdParam;
	CPUB_Lock			m_lockMapSendIOCmdParam;
	CPUB_Lock			m_lockLoginProc;
	CPUB_Lock			m_lockListAVStartParam;

	bool				m_bIsLogin;

	std::string			m_UID;
	std::string			m_UserName;
	std::string			m_PassWd;

	unsigned int		m_MaxChannel;
	int					m_CLientCount;
	int					m_MaxRDTChannel;

	unsigned long		m_ClientID;
#ifdef DVR_3_0
	PRODUCT_INFO		m_Productinfo;
	ENCODE_INFO			m_EncodeInfo;
	CReclogManEX		*m_pReclogMan;
#endif
	unsigned int		m_MaxFrameBufferSize;
	unsigned int		m_MaxFrameBufferTime;

	unsigned int												m_TVTChannelRefCount[2][TVT_MAX_CAMERA_NUM];
	std::map<IOTC_SESSION_KEY, std::list<CFrameData*>*>			m_mapFrameList;
	std::map<IOTC_SESSION_KEY, unsigned long>					m_mapFrameSize;

	//一个session表示一个avChannel，即对应的一次avServStart3, Session不是指一个客户端发起连接建立的SID
	std::map<IOTC_SESSION_KEY, IOTC_SESSION_PARAM>				m_mapSessionParam;

	std::map<int, RECV_IOCMD_PARAM*>							m_mapRecvIOCmdParam;//key == SID
	std::map<IOTC_CMD_KEY, SEND_IOCMD_PARAM*>					m_mapSendIOCmdParam;//

	std::list<AVCHANNEL_START_PARAM*>							m_listAVStartParam;
//
	typedef int (CTVTIOTCDevice::*IOCtrlFunc)(int SID, int avChannel, const char *pBuf);
	std::map<int, IOCtrlFunc>					m_mapIOCtrlFunc;
};


#endif