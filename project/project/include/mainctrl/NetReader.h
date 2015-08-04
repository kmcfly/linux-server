/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : 袁石维
** Date         : 2007-12-26
** Name         : NetReader.cpp
** Version      : 1.0
** Description  :
** Modify Record:
1:
***********************************************************************/
#ifndef _NET_READER_H_
#define _NET_READER_H_
#include "MultiReader.h"
//#include "devicecontrol.h"
#include "FrameData.h"
#include "NetProtocol.h"
#include "NetServer.h"

//const unsigned char NET_DATA_BUF_NUM = 16;

typedef enum _net_data_status
{
	DATA_NULL,
	DATA_READING,
	DATA_PAUSE,
	DATA_FF,
	DATA_REW,
}NET_DATA_STATUS;

typedef struct _net_data_buf
{
	unsigned char	*pBuf;
	unsigned long	bufLen;
	unsigned long	bUsed;
}NET_DATA_BUF;

typedef struct _net_data_reader
{
	unsigned long		clientID;
	unsigned long		streamID;
	unsigned long		dataType;	//NET_DATA_SEARCH_TYPE

	unsigned long		channelNum;
	unsigned long		diskOwnerIndex;

	CMultiReader		*pReader;
	CMyList <CFrameData *> *pDataList;

	unsigned long		status;		//NET_DATA_STATUS
	unsigned long		bAction;
	unsigned long		speed;		//NET_PLAY_SPEED
	unsigned long       kfOffSetNum;

	unsigned long		sendIndex;		//即将发送的帧index
	unsigned long		receiveIndex;	//已经处理的帧index

	unsigned long		startTime;
	unsigned long		endTime;
	ULONGLONG			videoCH;
	ULONGLONG			audioCH;
	CHANNEL_LIST        videoChannelList;
	unsigned long       channelForSendEndFrame;

	NET_DATA_BUF		*pDataBuf;
	unsigned long		bufNum;

	unsigned long		videoTime;
	unsigned long		sendVideoTime;

	unsigned long		curPlayChannelNum;
}NET_DATA_READRE;

class CNetReader
{
public:
	//interface
	CNetReader();
	~CNetReader();

	//bool Initail(unsigned char videoInputNum, unsigned char netVideoInputNum, char *pFilePath, CReclogManEX *pReclogManEx, CMessageMan *pMsgMan);
    bool Initail(char *pFilePath, CReclogManEX *pReclogManEx, CMessageMan *pMsgMan);
    void Quit();

	bool Start ();
	void Stop();

	void Action(bool bAction);

	//当客户端断开连接时，需停止该客户端的所有流
	void Stop(unsigned long clientID);
	
	bool StartRead(unsigned long clientID, unsigned long streamID, unsigned long dataType, \
		ULONGLONG videoCH, ULONGLONG audioCH, unsigned long startTime, unsigned long endTime, \
		ULONGLONG & outCH, unsigned long channelNum, unsigned long diskOwnerIndex);
	int Stop(unsigned long clientID, unsigned long streamID);
	bool Play(unsigned long clientID, unsigned long streamID);
	void Pause(unsigned long clientID, unsigned long streamID, ULONGLONG videoCH);
	bool FF(unsigned long clientID, unsigned long streamID, ULONGLONG videoCH, unsigned long speed);
	bool REW(unsigned long clientID, unsigned long streamID, ULONGLONG videoCH, unsigned long speed);
	void ChangeAudio(unsigned long clientID, unsigned long streamID, ULONGLONG audioCH);

//	bool ChangeCH(unsigned long clientID, unsigned long streamID, ULONGLONG videoCH, ULONGLONG audioCH, ULONGLONG & outCH);
//	bool ChangeTime(unsigned long clientID, unsigned long streamID, LONGLONG startTime, LONGLONG endTime, ULONGLONG & outCH);

	//用于接收回复的帧index
	void RelayIndex(unsigned long clientID, unsigned long streamID, unsigned long index);
protected:
	//methods

	//variable

private:
	//methods
	static	RESULT WINAPI ReadThread(LPVOID lpParam);
	
	bool CreateReadThd();
	void CloseReadThd();
	
	void ReadProc();

	bool HasStream(unsigned long clientID, unsigned long streamID);
	NET_DATA_READRE * CreateStream(unsigned long clientID, unsigned long streamID, unsigned long dataType, \
		ULONGLONG videoCH, ULONGLONG audioCH, unsigned long startTime, unsigned long endTime, \
		unsigned long channelNum, unsigned long diskOwnerIndex);
	void DestroyStream(NET_DATA_READRE *pNode);

	unsigned long GetBufIndex(NET_DATA_READRE *pNode);
	void ReleaseBuf (NET_DATA_READRE *pNode, unsigned long index);

	bool NextRead(NET_DATA_READRE* pNode, FRAME_INFO_EX & frameInfo);

	unsigned long GetStreamType(unsigned long clientID, unsigned long streamID);

	//variable
	HANDLE			m_hRead;
	bool			m_bRead;
	
	CMyList <NET_DATA_READRE *> m_readerList;
	CMyList <CFrameData *>		m_tempFrameList;

	CPUB_Lock	m_readLock;

	//CNetServer	*m_pNetServer;

	enum
	{
		LIVE_STREAM_ID = 0,
		PLAYBACK_STREAM_ID = 1,
		BACKUP_STREAM_ID = 2,
	};

#ifdef WIN32
	char *m_pBKPath;
#endif

	//unsigned char m_videoInputNum;
	//unsigned char m_netVideoInputNum;
	CReclogManEX  *m_pReclogMan;
	char          *m_pFilePath;
	CMessageMan   *m_pMsgMan;

	bool          m_bAction;
};
#endif //_NET_READER_H_



