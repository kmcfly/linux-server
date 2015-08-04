// PlaybackManager.h: interface for the CPlaybackManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(__PLAYBACKMAG_ASYN__)
#define __PLAYBACKMAG_ASYN__
#include "MultiReader.h"
#include "AsyncReader.h"
#include "LocalDevice.h"

#define  PLAYBACK_ALL_CHNN	(~((unsigned long)0))

//#ifndef __CHIP3531__
#ifndef __NEW_PLAYBACK__
class CPlaybackManagerAsyn  
{
public:
	CPlaybackManagerAsyn();
	virtual ~CPlaybackManagerAsyn();

	bool Initial(unsigned char chnnNum, unsigned char videoOutputNum, unsigned char maxFFSpeed, const char *pFilePath, CReclogManEX *pReclogManEx, unsigned long diskOwnerIndex, bool bSync);
	void Quit();

	//回放控制
	//开启准备回放
	//bool Play(FILE_INFO_EX_LIST *pFileList, unsigned long curTime,MESSAGE_CALLBACK callBack, void *pCallBackObj, ULONGLONG &outCH);
	bool Play(unsigned long startTime, unsigned long endTime, unsigned long curTime, \
		ULONGLONG chnnBits,MESSAGE_CALLBACK callBack, void *pCallBackObj, ULONGLONG &outCH);

	//开始送数据
	void Run();

	void Stop();
	//函数功能：暂停某个通道
	//函数参数：chnn--通道，brealPause--是否是按下了暂停按钮，因为快进时按上下段也会发暂停
	void Pause(unsigned long chnn,bool brealPause);
	void Resume(unsigned long chnn);
	bool SetCurTime(unsigned long curTime, ULONGLONG & outCH, unsigned long chnn);
	bool SetCurChannelMask(ULONGLONG ullWindowMask, ULONGLONG & outCH);
	void SetPlaySpeed(bool bForward, PLAY_SPEED playSpeed, unsigned long chnn);
	void PlayNextFrame(bool bForward, unsigned long chnn);

	//inline unsigned long GetPlayStatus(unsigned long chnn) {return m_nPlayStatus[chnn];};

	//通道属性函数接口
	void SetAudioChannel(unsigned long channel);
	inline bool IsAudioSupported(long iWindow) {return true;};

protected:
	static RESULT WINAPI SyncPlayProc(void *pParam);
	static RESULT WINAPI AsyncPlayProc(void *pParam);
	void AsyncPlayProc();
	void SyncPlayProc();

protected:
	//当前播放状态
	enum PLAY_STATE
	{
		N_PS_STOP = 1, 
		N_PS_PLAY, 
		N_PS_PAUSE, 
		N_PS_FF,
		N_PS_REW,
		N_PS_CHANGING_TIME
	};

	typedef struct _speed_ctrl
	{
		long			bFirstFrame;
		LONGLONG		lastFrameTime;
		LONGLONG		lastShowTime;
	}SPEED_CTRL;

	enum
	{
		FF_TYPE_SMOOTHLY,
		FF_TYPE_JUMPING
	};

	unsigned char			*m_pforwardType;
	unsigned char			*m_pframeSpeedStatus;
	unsigned char			m_forwardType;
	unsigned char			m_frameSpeedStatus;
	unsigned char			m_chnnNum;
	unsigned char			m_videoOutputNum;
	unsigned char           m_maxFFSpeed;
	unsigned char			m_activeChnnNum;
	char					*m_pFileName;

	//读文件控制
	PUB_thread_t			m_playThread;
	PLAY_STATE				*m_pPlayStatus;
	PLAY_STATE				m_nPlayStatus;
	bool					m_bFirstFrame[32];
	bool					m_bPlayNext;
	bool					m_bPlayPrev;
	PLAY_SPEED				m_playSpeed;
	unsigned short			m_playRate[11];

	CLock					m_playLock;
	SPEED_CTRL				m_speedCtrl;

	bool					m_bSetTimeFinish;//时间重定位辅助标识

	//帧信息
	FRAME_INFO_EX			m_frameInfo;
	long					m_bufSize;

	CAsyncReader			m_asyncReader;
	CMultiReader			m_syncReader;

	//当回放模块想要传给外部消息时，调用此回调函数
	//pObject	注册回调函数的时候传进来的参数
	//iMsg		消息名
	//pParam1	参数1
	//pParam2	参数2
	MESSAGE_CALLBACK			m_pMsgCallBack;
	void                       *m_pCallBackObj;

	CHANNEL_LIST             m_channelList;

	ULONGLONG				m_prePlayChannels;

	//音频输出通道。INVALID_INDEX表示静音。
	unsigned long           m_audioChannel; 

	//
	bool                    m_bRun;
	bool					m_bSync;
};
#else //__CHIP3531__
class CPlaybackManagerAsyn  
{
public:
	CPlaybackManagerAsyn();
	virtual ~CPlaybackManagerAsyn();

	bool Initial(unsigned char chnnNum, unsigned char videoOutputNum, unsigned char maxFFSpeed, const char *pFilePath, CReclogManEX *pReclogManEx, unsigned long diskOwnerIndex, bool bSync);
	void Quit();

	//回放控制
	//开启准备回放
	//bool Play(FILE_INFO_EX_LIST *pFileList, unsigned long curTime,MESSAGE_CALLBACK callBack, void *pCallBackObj, ULONGLONG &outCH);
	bool Play(unsigned long startTime, unsigned long endTime, unsigned long curTime, \
		ULONGLONG chnnBits,MESSAGE_CALLBACK callBack, void *pCallBackObj, ULONGLONG &outCH);

	//开始送数据
	void Run();

	void Stop();
	//函数功能：暂停某个通道
	//函数参数：chnn--通道，brealPause--是否是按下了暂停按钮，因为快进时按上下段也会发暂停
	void Pause(unsigned long chnn,bool brealPause);
	void Resume(unsigned long chnn);
	bool SetCurTime(unsigned long curTime, ULONGLONG & outCH, unsigned long chnn);
	bool SetCurChannelMask(ULONGLONG ullWindowMask, ULONGLONG & outCH);
	void SetPlaySpeed(bool bForward, PLAY_SPEED playSpeed, unsigned long chnn);
	void PlayNextFrame(bool bForward, unsigned long chnn);

	//inline unsigned long GetPlayStatus(unsigned long chnn) {return m_nPlayStatus[chnn];};

	//通道属性函数接口
	void SetAudioChannel(unsigned long channel);
	inline bool IsAudioSupported(long iWindow) {return true;};

protected:
	static RESULT WINAPI SyncPlayProc(void *pParam);
	static RESULT WINAPI AsyncPlayProc(void *pParam);
	void AsyncPlayProc();
	void SyncPlayProc();

	void ReleaseReadedBuf()
	{
		if(m_bFrameReadFromFile)
		{
			CLocalDevice::Instance()->PlaybackSetBuf(&m_frameInfo);
			m_bFrameReadFromFile = false;
		}
	}
protected:
	//当前播放状态
	enum PLAY_STATE
	{
		N_PS_STOP = 1, 
		N_PS_PLAY, 
		N_PS_PAUSE, 
		N_PS_FF,
		N_PS_REW,
		N_PS_CHANGING_TIME
	};

	typedef struct _speed_ctrl
	{
		long			bFirstFrame;
		LONGLONG		lastFrameTime;
		LONGLONG		lastShowTime;
	}SPEED_CTRL;

	enum
	{
		FF_TYPE_SMOOTHLY,
		FF_TYPE_JUMPING
	};

	unsigned char			*m_pforwardType;
	unsigned char			*m_pframeSpeedStatus;
	unsigned char			m_forwardType;
	unsigned char			m_frameSpeedStatus;
	unsigned char			m_chnnNum;
	unsigned char			m_videoOutputNum;
	unsigned char           m_maxFFSpeed;
	unsigned char			m_activeChnnNum;
	char					*m_pFileName;

	//读文件控制
	PUB_thread_t			m_playThread;
	PLAY_STATE				*m_pPlayStatus;
	PLAY_STATE				m_nPlayStatus;
	bool					m_bFirstFrame[32];
	bool					m_bPlayNext;
	bool					m_bPlayPrev;
	PLAY_SPEED				m_playSpeed;
	unsigned short			m_playRate[11];

	CPUB_Lock				m_playLock;
	SPEED_CTRL				m_speedCtrl;

	bool					m_bSetTimeFinish;//时间重定位辅助标识

	//帧信息
	FRAME_INFO_EX			m_frameInfo;
	long					m_bufSize;

	CAsyncReader			m_asyncReader;
	CMultiReader			m_syncReader;

	//当回放模块想要传给外部消息时，调用此回调函数
	//pObject	注册回调函数的时候传进来的参数
	//iMsg		消息名
	//pParam1	参数1
	//pParam2	参数2
	MESSAGE_CALLBACK			m_pMsgCallBack;
	void                       *m_pCallBackObj;

	CHANNEL_LIST             m_channelList;

	ULONGLONG				m_prePlayChannels;

	//音频输出通道。INVALID_INDEX表示静音。
	unsigned long           m_audioChannel; 

	//
	bool                    m_bRun;
	bool					m_bSync;
	bool					m_bFrameReadFromFile;
};
#endif //__CHIP3531__

#endif 
