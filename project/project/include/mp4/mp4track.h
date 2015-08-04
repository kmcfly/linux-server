
#if !defined(_NS_VISION_MP4_TRACK_H)
#define _NS_VISION_MP4_TRACK_H

#include "mp4common.h"
#include "mp4atom.h"

///////////////////////////////////////////////////////////////////////////////
// Mp4Track

/** 
 * 代表 MP4 文件的一个轨道(Track). 
 * Track 是 MP4 文件的主要组成部分, 代表了一路音频或视频等类型的媒体数据以
 * 及相关参数.
 */
class Mp4Track : public BaseObject
{
public:
	Mp4Track();
	virtual ~Mp4Track();
	DECLARE_BASEREF_METHODS(Mp4Atom);

// Attributes -------------------------------------------------
public:
	UINT GetTrackId();
	void SetTrackId(UINT id);

	LPCSTR GetType();
	void SetType(LPCSTR type);

	Mp4AtomPtr GetTrackAtom() { return fTrackAtom; }
	void SetTrackAtom(Mp4Atom* track);

	void SetDuration(INT64 duration);
	INT64 GetDuration();

	void SetTimeScale(UINT timeScale);
	UINT GetTimeScale();

	UINT GetNumberOfSamples();
	UINT GetNumberOfChunks();

	UINT GetWidth();
	UINT GetHeight();

	void SetFixedSampleDuration(INT64 duration);

	/** 设置每一个 Chunk 最多包括的 sample 的数量. */
	void SetSampleCountPerChunk(int count) { fSampleCountPerChunk = count; }

// Operations -------------------------------------------------
public:
	LRESULT ReadSample(Mp4File *file, UINT sampleId, BYTE* data, UINT* numBytes, 
		INT64* timestamp, INT64 *duration, bool* isSyncSample);

	LRESULT WriteSample(Mp4File *file, BYTE* data, UINT numBytes, 
		INT64 duration, bool isSyncSample);

	LRESULT FinishWrite(Mp4File *file);

	UINT GetSampleSize(UINT sampleId);
	UINT GetMaxSampleSize();
	INT64 GetTotalOfSampleSizes();

	UINT GetAvgBitrate();	// in bps
	UINT GetMaxBitrate();	// in bps

	void GetSampleTimes(UINT sampleId, INT64* startTime, INT64* duration);
	UINT GetSampleIdFromTime(INT64 when, bool wantSyncSample = false);
	
	bool IsSyncSample(UINT sampleId);
	UINT GetNextSyncSample(UINT sampleId);

	INT64 GetSampleFileOffset(UINT sampleId);
	INT64 GetChunkTime(int chunkId);

	Mp4AtomPtr GetAvcCAtom();
	void Clear();
	void Init();

// Implementation ---------------------------------------------
protected:
	UINT GetSampleStscIndex(UINT sampleId);	
	LRESULT FlushChunkBuffer(Mp4File *file);
	bool IsChunkFull(UINT sampleId);

	void UpdateChunkOffsets(INT64 chunkOffset);
	void UpdateSampleToChunk(UINT sampleId, UINT chunkId, UINT samplesPerChunk);
	void UpdateSampleSizes(UINT sampleId, UINT numBytes);
	void UpdateSampleTimes(INT64 duration);
	void UpdateSyncSamples(UINT sampleId, bool isSyncSample);
	void UpdateDurations(INT64 duration);
	void Reset();

// Data Members -----------------------------------------------
protected:
	UINT  fChunkCount;				///< Chunk 的数量
	UINT  fWriteSampleId;			///< 下一个要写入的 sample 的 ID.
	UINT  fFixedSampleSize;			///< 固定的 sample 的时间长度
	BYTE* fChunkBuffer;				///< 区块 chunk 缓存区
	UINT  fChunkBufferSize;			///< 当前区块 chunk 缓存区的大小
	UINT  fChunkSamples;			///< 当前区块 chunk 缓存区中总共 sample 的数目
	INT64 fChunkDuration;			///< 当前 Chunk 的时间长度

	// controls for chunking
	UINT  fSampleCountPerChunk;		///< 每一个区块的 sample 的数目
	INT64 fDurationPerChunk;		///< 每一个区块的时间长度
	UINT  fBytesPerChunk;			///< 每一个 sample 的大小.

	Mp4AtomPtr	fTrackAtom;			///< 根 ATOM

	Mp4PropertyPtr fTrackType;		///< 这个 Track 的类型.
	Mp4PropertyPtr fTrackId;		///< 这个 Track 的 ID.
	Mp4PropertyPtr fTimeScale;		///< Time scale
	Mp4PropertyPtr fTrackDuration;	///< Track Duration
	Mp4PropertyPtr fMediaDuration;	///< Media Duration

	Mp4ArrayPropertyPtr fSamplesSize;		///< Sample 大小表
	Mp4ArrayPropertyPtr fChunksOffset;		///< Chunk 文件偏移位置表
	Mp4ArrayPropertyPtr fSyncSamples;		///< 同步 Sample ID 表

	// stts
	Mp4ArrayPropertyPtr fSampleCount;		///< stts 有相同时间的 sample 的数量
	Mp4ArrayPropertyPtr fSampleDelta;		///< stts sample 的时间长度

	// stsc
	Mp4ArrayPropertyPtr fFirstChunk;		///< 第一个 chunk 的 ID
	Mp4ArrayPropertyPtr fSamplesPerChunk;	///< 这些 chunk 包含的 sample 数
	Mp4ArrayPropertyPtr fFirstSample;		///< 第一个 sample 的 ID.
	Mp4ArrayPropertyPtr fSampleDescIndex;	///< 
};

typedef SmartPtr<Mp4Track> Mp4TrackPtr;
typedef Mp4Array<Mp4TrackPtr> Mp4TrackArray;

#endif // !defined(_NS_VISION_MP4_TRACK_H)
