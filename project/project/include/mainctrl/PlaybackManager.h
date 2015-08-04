// PlaybackManager.h: interface for the CPlaybackManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PLAYBACKMANAGER_H__DB3693C7_9CFE_4CF9_B6FB_53A6C4409370__INCLUDED_)
#define AFX_PLAYBACKMANAGER_H__DB3693C7_9CFE_4CF9_B6FB_53A6C4409370__INCLUDED_

#include "StreamRecordManEx.h"
#define ADAPT_TO_ACTUAL_OUTPUT_ULL_CHNN(outCH) \
	if(CStreamRecordManEx::Instance()->IsSupportMinorStreamRecord())\
		{\
		ULONGLONG tmep = outCH;\
		unsigned char localVideoInputNum = CProduct::Instance()->LocalVideoInputNum();\
		unsigned char netVideoInputNum = CProduct::Instance()->NetVideoInputNum();\
		tmep &= (~((static_cast<ULONGLONG>(0x1) <<(localVideoInputNum+netVideoInputNum))-1));\
		tmep = tmep >> netVideoInputNum;\
		outCH &= ((static_cast<ULONGLONG>(0x1) <<(localVideoInputNum+netVideoInputNum))-1);\
		outCH |= tmep;\
		}

#define ADAPT_TO_VIRTUAL_ULL_WINDOW_MASK(ullWindowMask) \
	if(CStreamRecordManEx::Instance()->IsSupportMinorStreamRecord())\
			{\
				unsigned long support1080pNum=CStreamRecordManEx::Instance()->ChannelNumOfSupport1080pReplay();\
				unsigned char localVideoInputNum = CProduct::Instance()->LocalVideoInputNum();\
				unsigned char netVideoInputNum = CProduct::Instance()->NetVideoInputNum();\
				printf("1 ullWindowMask:0x%llx\n",ullWindowMask);\
				unsigned long retain1080pNum = 0;\
				for(unsigned long index=0;index<netVideoInputNum;index++)\
				{\
				if((((ULONGLONG)(0x1) <<(index+localVideoInputNum))&ullWindowMask)!=0)\
					{\
						retain1080pNum++;\
					}\
				}\
				if(retain1080pNum>support1080pNum)\
					{\
					for(unsigned long index=0;index<netVideoInputNum;index++)\
						{\
							ullWindowMask |= ((ullWindowMask&((ULONGLONG)(0x1) <<(index+localVideoInputNum)))<<netVideoInputNum);\
							ullWindowMask &= (~((ULONGLONG)(0x1) <<(index+localVideoInputNum)));\
							}\
							}\
				printf("2 ullWindowMask:0x%llx\n",ullWindowMask);\
			}\

#define ADAPT_TO_VIRTUAL_PLAYBACK_ULL_CHNN_BITS(chnnBits) \
						if(CStreamRecordManEx::Instance()->IsSupportMinorStreamRecord())\
								{\
									unsigned long support1080pNum=CStreamRecordManEx::Instance()->ChannelNumOfSupport1080pReplay();\
									unsigned char localVideoInputNum = CProduct::Instance()->LocalVideoInputNum();\
									unsigned char netVideoInputNum = CProduct::Instance()->NetVideoInputNum();\
									printf("1 chnnBits:%llx\n",chnnBits);\
									unsigned long retain1080pNum = 0;\
									for(unsigned long index=0;index<netVideoInputNum;index++)\
									{\
									if((((ULONGLONG)(0x1) <<(index+localVideoInputNum))&chnnBits)!=0)\
										{\
											retain1080pNum++;\
										}\
									}\
									if(retain1080pNum>support1080pNum)\
										{\
										for(unsigned long index=0;index<netVideoInputNum;index++)\
											{\
											chnnBits |= ((chnnBits&((ULONGLONG)(0x1) <<(index+localVideoInputNum)))<<netVideoInputNum);\
											chnnBits &= (~((ULONGLONG)(0x1) <<(index+localVideoInputNum)));\
											}\
											}\
											printf("2 chnnBits:%llx\n",chnnBits);\
											}\


#define ADAPT_TO_ACTUAL_CHANNEL(channel) \
	if(CStreamRecordManEx::Instance()->IsSupportMinorStreamRecord())\
		{\
		unsigned char netChnnNum = CProduct::Instance()->NetVideoInputNum();\
		unsigned char localChnnNum = CProduct::Instance()->LocalVideoInputNum();\
		if(channel>=(netChnnNum+localChnnNum)){channel = channel-netChnnNum;}\
		}

#define ADAPT_TO_VIRTUAL_CHANNEL(channel) \
	if(CStreamRecordManEx::Instance()->IsSupportMinorStreamRecord())\
		{\
		unsigned char netChnnNum = CProduct::Instance()->NetVideoInputNum();\
		unsigned char localChnnNum = CProduct::Instance()->LocalVideoInputNum();\
		if(m_chnnNum>(netChnnNum+localChnnNum)&&channel<(netChnnNum+localChnnNum))\
			{\
			channel = channel+netChnnNum;\
			}\
		}

#define ADAPT_TO_ACTUAL_CHNN_NUM(chnnNum,chnnNumAdapt) \
	if(CStreamRecordManEx::Instance()->IsSupportMinorStreamRecord())\
		{\
		unsigned char netChnnNum = CProduct::Instance()->NetVideoInputNum();\
		unsigned char localChnnNum = CProduct::Instance()->LocalVideoInputNum();\
		if(chnnNum>(netChnnNum+localChnnNum))\
			{\
			chnnNumAdapt = chnnNum-netChnnNum;\
			}\
			else\
				{\
				chnnNumAdapt=chnnNum;\
				}\
				}\
				else\
					{\
					chnnNumAdapt=chnnNum;\
					}


#define ADAPT_TO_VIRTUAL_CHNN_NUM(chnnNum,chnnNumAdapt) \
	if(CStreamRecordManEx::Instance()->IsSupportMinorStreamRecord())\
		{\
		unsigned char netChnnNum = CProduct::Instance()->NetVideoInputNum();\
		unsigned char localChnnNum = CProduct::Instance()->LocalVideoInputNum();\
		if(chnnNum<=(netChnnNum+localChnnNum))\
			{\
			chnnNumAdapt = chnnNum+netChnnNum;\
			}\
			else\
				{\
				chnnNumAdapt=chnnNum;\
				}\
				}\
				else\
					{\
					chnnNumAdapt=chnnNum;\
					}

#define SET_PRIMARY_STREAM_CHNN(setDisplayIndex,ullWindowMask) \
	if(CStreamRecordManEx::Instance()->IsSupportMinorStreamRecord())\
		{\
			printf("1-set primary chnn: 0x%llx\n",ullWindowMask);\
			unsigned char localVideoInputNum = CProduct::Instance()->LocalVideoInputNum();\
			unsigned char netVideoInputNum = CProduct::Instance()->NetVideoInputNum();\
			unsigned long displayNum = 0;\
			for(unsigned long index=0;index<netVideoInputNum;index++)\
			{\
				if((((ULONGLONG)(0x1) <<(index+localVideoInputNum))&ullWindowMask)!=0 \
					|| (((ULONGLONG)(0x1) <<(index+localVideoInputNum+netVideoInputNum))&ullWindowMask)!=0)\
				{\
					displayNum++;\
					if(displayNum==setDisplayIndex+1)\
					{\
						ullWindowMask |= ((ULONGLONG)0x1<<(index+localVideoInputNum));\
						ullWindowMask &= (~((ULONGLONG)(0x1) <<(index+localVideoInputNum+netVideoInputNum)));\
						break;\
					}\
				}\
			}\
			printf("2-set primary chnn: 0x%llx\n",ullWindowMask);\
		}\



#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MultiReader.h"

#define PLAYBACK_SPS_MAX_CHN_NUM 128

//#ifndef __CHIP3531__
#ifndef __NEW_PLAYBACK__

class CPlaybackManager  
{
public:
	CPlaybackManager();
	virtual ~CPlaybackManager();

	bool Initial(unsigned char chnnNum, unsigned char videoOutputNum, unsigned char maxFFSpeed, const char *pFilePath, CReclogManEX *pReclogManEx, unsigned long diskOwnerIndex);
	void Quit();

	//回放控制
	//开启准备回放
	bool Play(FILE_INFO_EX_LIST *pFileList, unsigned long curTime,MESSAGE_CALLBACK callBack, void *pCallBackObj, ULONGLONG &outCH);
	bool Play(unsigned long startTime, unsigned long endTime, unsigned long curTime, \
		ULONGLONG chnnBits,MESSAGE_CALLBACK callBack, void *pCallBackObj, ULONGLONG &outCH);

	//开始送数据
	void Run();

	void Stop();
	void Pause();
	void Resume();
	bool SetCurTime(unsigned long curTime, ULONGLONG & outCH);
	bool SetCurChannelMask(ULONGLONG ullWindowMask, ULONGLONG & outCH);
	void SetPlaySpeed(bool bForward, PLAY_SPEED playSpeed);
	void PlayFF();
	void PlayRew();
	void PlayNextFrame(bool bForward);

	inline unsigned long GetPlayStatus() {return m_nPlayStatus;};

	//通道属性函数接口
	void SetAudioChannel(unsigned long channel);
	inline bool IsAudioSupported(long iWindow) {return true;};

protected:
	static RESULT WINAPI PlayProc(void *pParam);
	void PlayProc();

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

	unsigned char			m_forwardType;
	unsigned char			m_frameSpeedStatus;
	unsigned char			m_chnnNum;
	unsigned char			m_localChnnNum;
	unsigned char			m_videoOutputNum;
	unsigned char           m_maxFFSpeed;
	unsigned char			m_activeChnnNum;
	char					*m_pFileName;

	//读文件控制
	PUB_thread_t			m_playThread;
	CWaitEvent				m_evtReadOnce;
	PLAY_STATE				m_nPlayStatus;
	bool					m_bPlayNext;
	bool					m_bPlayPrev;
	PLAY_SPEED				m_playSpeed;
	float					m_playRate[11];

	CLock					m_playLock;
	SPEED_CTRL				m_speedCtrl;

	bool					m_bSetTimeFinish;//时间重定位辅助标识

	//帧信息
	FRAME_INFO_EX			m_frameInfo;
	long					m_bufSize;

	CMultiReader			m_reader;

	//当回放模块想要传给外部消息时，调用此回调函数
	//pObject	注册回调函数的时候传进来的参数
	//iMsg		消息名
	//pParam1	参数1
	//pParam2	参数2
	MESSAGE_CALLBACK  m_pMsgCallBack;
	void                       *m_pCallBackObj;

	CHANNEL_LIST             m_channelList;

	ULONGLONG				m_prePlayChannels;

	//音频输出通道。INVALID_INDEX表示静音。
	unsigned long           m_audioChannel; 

	//
	bool                    m_bRun;
};
#else //__CHIP3531__
class CPlaybackManager  
{
public:
	CPlaybackManager();
	virtual ~CPlaybackManager();

	bool Initial(unsigned char chnnNum, unsigned char videoOutputNum, unsigned char maxFFSpeed, const char *pFilePath, CReclogManEX *pReclogManEx, unsigned long diskOwnerIndex);
	void Quit();

	//回放控制
	//开启准备回放
	bool Play(FILE_INFO_EX_LIST *pFileList, unsigned long curTime,MESSAGE_CALLBACK callBack, void *pCallBackObj, ULONGLONG &outCH);
	bool Play(unsigned long startTime, unsigned long endTime, unsigned long curTime, \
		      ULONGLONG chnnBits,MESSAGE_CALLBACK callBack, void *pCallBackObj, ULONGLONG &outCH);
	
	//开始送数据
	void Run();

	void Stop();
	void Pause();
	void Resume();
	bool SetCurTime(unsigned long curTime, ULONGLONG & outCH);
	bool SetCurChannelMask(ULONGLONG ullWindowMask, ULONGLONG & outCH);
	void SetSpecSplit(bool bIsSepcSplit, unsigned long primaryStreamDisplayIndex);
	void SetPlaySpeed(bool bForward, PLAY_SPEED playSpeed);
	void PlayFF();
	void PlayRew();
	void PlayNextFrame(bool bForward);

	inline unsigned long GetPlayStatus() {return m_nPlayStatus;};

	//通道属性函数接口
	void SetAudioChannel(unsigned long channel);
	inline bool IsAudioSupported(long iWindow) {return true;};

protected:
	static RESULT WINAPI PlayProc(void *pParam);
	void PlayProc();

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

	unsigned char			m_forwardType;
	unsigned char			m_frameSpeedStatus;
	unsigned char			m_chnnNum;
	unsigned char			m_localChnnNum;
	unsigned char			m_videoOutputNum;
	unsigned char           m_maxFFSpeed;
	unsigned char			m_activeChnnNum;
	char					*m_pFileName;
	
	int                     m_playImageChannWidth[PLAYBACK_SPS_MAX_CHN_NUM];
	int                     m_playImageChannHeight[PLAYBACK_SPS_MAX_CHN_NUM];

	int                     m_playImageChannFrames[PLAYBACK_SPS_MAX_CHN_NUM];

	int                     m_playImageChannTmCount[PLAYBACK_SPS_MAX_CHN_NUM];
	LONGLONG                m_playImageChannPreTms[PLAYBACK_SPS_MAX_CHN_NUM];

	//读文件控制
	PUB_thread_t			m_playThread;
	CWaitEvent				m_evtReadOnce;
	PLAY_STATE				m_nPlayStatus;
	bool					m_bPlayNext;
	bool					m_bPlayPrev;
	PLAY_SPEED				m_playSpeed;
	float					m_playRate[11];

	CPUB_Lock				m_playLock;
	SPEED_CTRL				m_speedCtrl;
	
	bool					m_bSetTimeFinish;//时间重定位辅助标识

	//帧信息
	FRAME_INFO_EX			m_frameInfo;
	long					m_bufSize;
	
	CMultiReader			m_reader;

	//当回放模块想要传给外部消息时，调用此回调函数
	//pObject	注册回调函数的时候传进来的参数
	//iMsg		消息名
	//pParam1	参数1
	//pParam2	参数2
	MESSAGE_CALLBACK  m_pMsgCallBack;
	void                       *m_pCallBackObj;

	CHANNEL_LIST             m_channelList;

	ULONGLONG				m_prePlayChannels;

	//音频输出通道。INVALID_INDEX表示静音。
	unsigned long           m_audioChannel; 

	//
	bool                    m_bRun;

	bool					m_bFrameReadFromFile;

	bool m_bIsSepcSplit; //是否奇异分割
	unsigned long m_primaryStreamDisplayIndex; //奇异分割时须设置大画面码流为大码流的画面序号
};
#endif //__CHIP3531__
#endif // !defined(AFX_PLAYBACKMANAGER_H__DB3693C7_9CFE_4CF9_B6FB_53A6C4409370__INCLUDED_)
