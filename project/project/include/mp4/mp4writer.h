
#if !defined(_NS_VISION_MP4_WRITER_H)
#define _NS_VISION_MP4_WRITER_H

#include "mp4define.h"
#include "mp4common.h"
#include "mp4atom.h"
#include "mp4track.h"

///////////////////////////////////////////////////////////////////////////////
// Mp4Writer

enum NalType {
	NAL_TYPE_NON_IDR_SLICE		= 0x1,
	NAL_TYPE_DP_A_SLICE			= 0x2,
	NAL_TYPE_DP_B_SLICE			= 0x3,
	NAL_TYPE_DP_C_SLICE			= 0x4,
	NAL_TYPE_IDR_SLICE			= 0x5,
	NAL_TYPE_SEI				= 0x6,
	NAL_TYPE_SEQ_PARAM			= 0x7,
	NAL_TYPE_PIC_PARAM			= 0x8,
	NAL_TYPE_ACCESS_UNIT		= 0x9,
	NAL_TYPE_END_OF_SEQ			= 0xa,
	NAL_TYPE_END_OF_STREAM		= 0xb,
	NAL_TYPE_FILLER_DATA		= 0xc,
	NAL_TYPE_SEQ_EXTENSION		= 0xd
};

enum SampleFlags {
	SAMPLE_SYNC_POINT	= 0x01,		///< 这是一个同步点
	SAMPLE_START		= 0x10,		///< 这是这个 sample 的第一个数据包
	SAMPLE_END			= 0x20,		///< 这是这个 sample 的最后一个数据包
	SAMPLE_FRAGMENT		= 0x80,		///< 这个数据包是一个碎片
};

/** MP4 文件复用器/生成器. */
class Mp4Writer
{
public:
	Mp4Writer(unsigned int fileSize = DEFAULT_DATA_CNT,ENUM_MP4_WRITE_DESTYPE desType = ENUM_MP4_DESTYPE_LOCAL);
	virtual ~Mp4Writer();

// Attributes -------------------------------------------------
public:
	Mp4FilePtr GetFile() { return fMp4File; }
	bool IsOpen();

// Operations -------------------------------------------------
public:
	LRESULT Create(LPCSTR name);
	void Close();

	Mp4TrackPtr AddTrack(LPCSTR type, UINT timeScale = 1000);
	UINT AddAmrTrack(UINT timeScale, INT64 duration, BYTE framesPerSample, bool isAmrWB);
	UINT AddAudioTrack(UINT timeScale, INT64 duration, BYTE type);
	UINT AddVideoTrack(UINT timeScale, INT64 duration, WORD width, WORD height, BYTE type);

	LRESULT BeginWrite();
	LRESULT FinishWrite();

	UINT WriteAudioSampe(BYTE* sample, UINT size, INT64 timestamp, UINT timeScale = 1000);
	UINT WriteVideoSample(BYTE* sample, UINT size, INT64 timestamp, UINT isSyncSample, bool isEnd, UINT timeScale = 1000);

	Mp4TrackPtr GetTrack(int index);
	Mp4TrackPtr GetTrack(LPCSTR type);

	UINT GetDataCnt();

// Implementation ---------------------------------------------
protected:
	bool FlushVideoBuffer(Mp4TrackPtr track, INT64 timestamp, UINT timeScale);
	UINT FilterParamSets(Mp4TrackPtr track, BYTE* sample, UINT sampleSize);

// Data Members -----------------------------------------------
protected:	
	BYTE*	fVideoBuffer;			///< 视频缓存区
	UINT	fVideoBufferSize;		///< 视频缓存区的大小
	UINT	fVideoSampleSize;		///< 视频缓存区有效数据大小
	UINT    fDataCnt;               ///< 总共已写数据
	bool	fIsSyncSample;			///< 是否是同步 sample
	UINT	fSyncSampleCount;			///< 写入的关键帧的数目

	INT64	fLastVideoTimestamp;	///< 前一帧的时间戳
	INT64	fLastAudioTimestamp;	///< 前一帧的时间戳
	INT64	fStartTimestamp;		///< 开始录像的时间戳

	int		fVideoTrackIndex;		///< 视频 track 的索引值.
	char*	fFileName;				///< 文件的名称
	bool	fIsAACAudio;
	int		fSampleRate;

	Mp4AtomPtr		fRootAtom;		///< 根 ATOM
	Mp4FilePtr		fMp4File;		///< Mp4 文件
	Mp4TrackArray	fTracks;		///< Track 列表

	unsigned int    m_fileSize;     ///<写的mp4文件媒体数据大小 
	ENUM_MP4_WRITE_DESTYPE m_desType;///写到ftp服务器还是本地
};	

#endif // !defined(_NS_VISION_MP4_WRITER_H)
