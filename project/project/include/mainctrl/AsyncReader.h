/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : xzw
** Date         : 2011-6-28
** Name         : AsyncReader.h
** Version      : 3.1.9
** Description  :
** Modify Record:
1:
***********************************************************************/
   
#ifndef _ASYNC_READER_H__
#define _ASYNC_READER_H__

#include "OneChnnReader.h"

typedef CMyList<unsigned long> CHANNEL_LIST;


class CAsyncReader  
{
public:
	CAsyncReader();
	virtual ~CAsyncReader();
	

	bool  Initial(long maxCHNum, CReclogManEX *pRecLogManEx, unsigned long diskOwnerIndex);
	void Quit();

	long StartRead(FILE_INFO_EX_LIST *pFileList, ULONGLONG & outCH, const char *pFilePath = NULL);
	long StartRead(unsigned long startTime, unsigned long endTime, \
		const CHANNEL_LIST &chnnList, ULONGLONG & outCH, const char *pFilePath = NULL);
	long StartRead(unsigned long startTime, const CHANNEL_LIST &chnnList, ULONGLONG & outCH, const char *pFilePath);
	void StopRead();

	long SetCurTime(bool bForward, unsigned long time, ULONGLONG & outCH, unsigned long chnn);
	long SetCurChannel(ULONGLONG chnnBits, ULONGLONG & outCH);

	void SetForward(unsigned long chnn, bool bforward);
	void SetJumping(unsigned long chnn , bool bJumping);
	void SetPlaySpeed(unsigned long chnn, unsigned short speed);
	void SetPause(unsigned long chnn, bool bPause);
	void SetPlayNext(unsigned long chnn, bool bPlayNext);	
	long GetNextFrameType(FRAME_INFO_EX &frameInfo, unsigned long &readerID,bool &bneedchange);	
	long GetFrame(FRAME_INFO_EX &frameinfo, unsigned long &readerID);
	ULONGLONG GetShowTime(){return m_showTime;}
	void SetBaseShowTime(unsigned long readerID, LONGLONG baseShowTime);
	void SetBaseFrameTime(unsigned long readerID, LONGLONG baseFrameTime);	
	void SetLastReadFrameTime(unsigned long readerID, LONGLONG lastReadFrameTime);
	LONGLONG GetBaseFrameTime(unsigned long readerID);
	
	bool HasAudioData(long channel);
	bool CheckSleep();
	bool SetRealPause(bool brealPause){m_bRealPause = brealPause;};
protected:
	//函数功能：得到下一个要读取数据的通道(依照时间顺序)
	//返回值：找到下一个要读取的通道，返回通道号，否则返回-1
	//函数参数：
	//bool &bneedchange--是否跳帧
	//int &chnnstatus--返回的状态值，例如MR_RET_ALLCHNN_PAUSE
	int GetNextReadChannel(bool &bneedchange,int &chnnstatus);
	long GetNextFrame(FRAME_INFO_EX &frameInfo, unsigned long &readerID);
	long GetKeyFrame(FRAME_INFO_EX &frameInfo, long nStepFrame, bool bForward, unsigned long &readerID);	
public:
	enum ASYNC_READER_RET
	{
		MR_RET_OK,					//成功
		MR_RET_TOO_MANY_CHANNELS,	//回放通道太多，超过了初始化时所设置的最大通道数
		MR_RET_NOTHING_TO_READ,		//没有东西可读
		MR_RET_PARAM_ERROR,			//参数错误
		MR_RET_UNKNOWN_ERROR,		//未知错误
		MR_RET_ALLCHNN_PAUSE        //返回所有通道都暂停
	};
private:
	enum READ_TYPE
	{
		READ_TYPE_NULL,
		READ_BY_FILE_LIST,
		READ_BY_START_END,
		READ_BY_START,
	};
	typedef struct _local_reader
	{
		unsigned long	chnn;
		unsigned long	bSwitch;
		unsigned short	speed;
		bool			bforward;
		bool			bJumping;
		bool			bPause;
		bool			bPlayNext;
		LONGLONG		time;
		LONGLONG		baseShowTime;
		LONGLONG		baseFrameTime;//时间校验基础帧
		LONGLONG        lastReadFrameTime;//上一帧的读取时间
		COneChnnReader	reader;
	}LOCAL_READER, *LPLOCAL_READER;

	READ_TYPE				m_readType;//表示读取类型
	char					*m_pFilePath;
	LPLOCAL_READER			m_pReader;
	long					m_maxCHNum;

	LONGLONG				m_startTime;
	LONGLONG				m_endTime;
	
	ULONGLONG				m_chnnBits;
	int						m_currReaderID;
	ULONGLONG				m_showTime;

	bool					m_bRealPause;//是否是真正需要停止画面的暂停
};

#endif // _MULTI_READER_H__
