#ifndef _IP_DATA_BUFFER_MAN_H
#define _IP_DATA_BUFFER_MAN_H

#include "IPDataBufferDefine.h"
const unsigned long MAX_DATA_BLOCK_LEN	= 512 * 1024;
const unsigned long BLOCK_NUM			= 500;

class CIPDataBufferMan
{
	class CPUB_Lock_IPD
	{
	public:
		CPUB_Lock_IPD();
		~CPUB_Lock_IPD();

		void Lock();										//加锁
		void UnLock();										//解锁
#ifdef	__ENVIRONMENT_LINUX__
		void CondWait();
		void CondSignal();
		int TimeLock(int iMicrosecond);
		int TryLock();
#endif
	private:
#ifdef	__ENVIRONMENT_LINUX__
		pthread_cond_t		m_cond;
		typedef  pthread_mutex_t			PUB_lock_t_IPD;		//锁
#else
		typedef  CRITICAL_SECTION			PUB_lock_t_IPD; //锁
#endif
		PUB_lock_t_IPD		m_Lock;
	};

	typedef struct _one_data_block
	{
		long			dataLen;
		unsigned char	*pData;
	}ONE_DATA_BLOCK;

	typedef struct _ip_data_buffer_info
	{
		long					bufferSize;
		long					dataLen;
		long					freeLen;
		long					readOffset;
		long					writeOffset;
		long					freeOffset;
		long					tailOffset;
		long					bBufferFull;
		unsigned char			*pBuffer;
		CPUB_Lock_IPD			*pLock;

		ONE_DATA_BLOCK			dataBlocks[BLOCK_NUM];

		unsigned long			recvLenOneSecond[5];
		unsigned long			curTime;
	}IP_DATA_BUFFER_INFO;

	typedef struct _channel_stream_info
	{
		long		width;
		long		height;
		bool		bSupport;
	}CHANNEL_STREAM_INFO;

public:
	CIPDataBufferMan(void);
	~CIPDataBufferMan(void);

	bool Initial(NET_DEVICE_STREAM_TYPE streamType);
	void Quit();

	/************************************************************************
	设置每个解码芯片所对应的数字通道掩码
	************************************************************************/
	void SetDecodeChipNum(long decodeChipNum);
	void SetChannelDecChip(long channelNum, long *pDecChip);
	void SetChipBuffer(long chipId, unsigned char *pDataBuffer, unsigned long bufSize);
	void SetSupportVideoSize(unsigned long *pSupportVideoSize, long channelNum);

	/************************************************************************
	设置一个Block的大小
	************************************************************************/
	void SetOneBlockSize(unsigned long blockSize);

	/************************************************************************
	得到当前有几个512K的Buffer已填充了数据
	************************************************************************/
	long GetBufferedNum(long chipId);

	/************************************************************************
	读取某个解码芯片所对应的一块数据
	************************************************************************/
	bool GetDataBuffer(long chipId, unsigned char *&pBuffer, unsigned long &dataLen);

	/************************************************************************
	释放一块数据区
	************************************************************************/
	void ReleaseDataBuffer(long chipId, unsigned char *pBuffer, unsigned long dataLen);

	/************************************************************************
	向缓冲区中放入一帧数据
	************************************************************************/
	bool PushData(long channel, const DVR_IP_IN_FRM_INFO &ipFrameInfo, const unsigned char *pData);

	void Action(bool bAction);

	void ClearDataBuffer();

	//统计网络上平均码流速度
	unsigned long GetAverageRate();

	//判断某个CHIP的缓冲区是否已经满了
	bool IsBufferFull(long chipId);

	bool PrintBufferInfo(long chipId);

private:
	long GetDecodeChipId(long channel);
	
	unsigned long GetCurrTime32();
#ifdef __ENVIRONMENT_WIN32__
	unsigned long TimeToDVRTime32(FILETIME DVRtm);
#else
	unsigned long TimeToDVRTime32(timeval  DVRtm);
#endif
	void CheckVideoSize(long channel, long width, long height);

private:
	long						m_channelNum;
	long						m_decChipNum;
	long						*m_pChannelDecChip;
	IP_DATA_BUFFER_INFO			*m_pDataBufferInfo;

	bool						*m_pbNeedKeyFrame;

	bool						m_bAction;

	unsigned long				*m_pSupportVideoSize;

	CHANNEL_STREAM_INFO			*m_pStreamInfo;

	CPUB_Lock_IPD				m_averageRateLock;

	CPUB_Lock_IPD				m_blockSizeLock;
	unsigned long				m_blockSize;

	NET_DEVICE_STREAM_TYPE		m_streamType;
};

#endif
