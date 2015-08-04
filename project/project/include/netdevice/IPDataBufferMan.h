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

		void Lock();										//����
		void UnLock();										//����
#ifdef	__ENVIRONMENT_LINUX__
		void CondWait();
		void CondSignal();
		int TimeLock(int iMicrosecond);
		int TryLock();
#endif
	private:
#ifdef	__ENVIRONMENT_LINUX__
		pthread_cond_t		m_cond;
		typedef  pthread_mutex_t			PUB_lock_t_IPD;		//��
#else
		typedef  CRITICAL_SECTION			PUB_lock_t_IPD; //��
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
	����ÿ������оƬ����Ӧ������ͨ������
	************************************************************************/
	void SetDecodeChipNum(long decodeChipNum);
	void SetChannelDecChip(long channelNum, long *pDecChip);
	void SetChipBuffer(long chipId, unsigned char *pDataBuffer, unsigned long bufSize);
	void SetSupportVideoSize(unsigned long *pSupportVideoSize, long channelNum);

	/************************************************************************
	����һ��Block�Ĵ�С
	************************************************************************/
	void SetOneBlockSize(unsigned long blockSize);

	/************************************************************************
	�õ���ǰ�м���512K��Buffer�����������
	************************************************************************/
	long GetBufferedNum(long chipId);

	/************************************************************************
	��ȡĳ������оƬ����Ӧ��һ������
	************************************************************************/
	bool GetDataBuffer(long chipId, unsigned char *&pBuffer, unsigned long &dataLen);

	/************************************************************************
	�ͷ�һ��������
	************************************************************************/
	void ReleaseDataBuffer(long chipId, unsigned char *pBuffer, unsigned long dataLen);

	/************************************************************************
	�򻺳����з���һ֡����
	************************************************************************/
	bool PushData(long channel, const DVR_IP_IN_FRM_INFO &ipFrameInfo, const unsigned char *pData);

	void Action(bool bAction);

	void ClearDataBuffer();

	//ͳ��������ƽ�������ٶ�
	unsigned long GetAverageRate();

	//�ж�ĳ��CHIP�Ļ������Ƿ��Ѿ�����
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
