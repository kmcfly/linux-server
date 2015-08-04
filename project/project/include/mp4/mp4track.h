
#if !defined(_NS_VISION_MP4_TRACK_H)
#define _NS_VISION_MP4_TRACK_H

#include "mp4common.h"
#include "mp4atom.h"

///////////////////////////////////////////////////////////////////////////////
// Mp4Track

/** 
 * ���� MP4 �ļ���һ�����(Track). 
 * Track �� MP4 �ļ�����Ҫ��ɲ���, ������һ·��Ƶ����Ƶ�����͵�ý��������
 * ����ز���.
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

	/** ����ÿһ�� Chunk �������� sample ������. */
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
	UINT  fChunkCount;				///< Chunk ������
	UINT  fWriteSampleId;			///< ��һ��Ҫд��� sample �� ID.
	UINT  fFixedSampleSize;			///< �̶��� sample ��ʱ�䳤��
	BYTE* fChunkBuffer;				///< ���� chunk ������
	UINT  fChunkBufferSize;			///< ��ǰ���� chunk �������Ĵ�С
	UINT  fChunkSamples;			///< ��ǰ���� chunk ���������ܹ� sample ����Ŀ
	INT64 fChunkDuration;			///< ��ǰ Chunk ��ʱ�䳤��

	// controls for chunking
	UINT  fSampleCountPerChunk;		///< ÿһ������� sample ����Ŀ
	INT64 fDurationPerChunk;		///< ÿһ�������ʱ�䳤��
	UINT  fBytesPerChunk;			///< ÿһ�� sample �Ĵ�С.

	Mp4AtomPtr	fTrackAtom;			///< �� ATOM

	Mp4PropertyPtr fTrackType;		///< ��� Track ������.
	Mp4PropertyPtr fTrackId;		///< ��� Track �� ID.
	Mp4PropertyPtr fTimeScale;		///< Time scale
	Mp4PropertyPtr fTrackDuration;	///< Track Duration
	Mp4PropertyPtr fMediaDuration;	///< Media Duration

	Mp4ArrayPropertyPtr fSamplesSize;		///< Sample ��С��
	Mp4ArrayPropertyPtr fChunksOffset;		///< Chunk �ļ�ƫ��λ�ñ�
	Mp4ArrayPropertyPtr fSyncSamples;		///< ͬ�� Sample ID ��

	// stts
	Mp4ArrayPropertyPtr fSampleCount;		///< stts ����ͬʱ��� sample ������
	Mp4ArrayPropertyPtr fSampleDelta;		///< stts sample ��ʱ�䳤��

	// stsc
	Mp4ArrayPropertyPtr fFirstChunk;		///< ��һ�� chunk �� ID
	Mp4ArrayPropertyPtr fSamplesPerChunk;	///< ��Щ chunk ������ sample ��
	Mp4ArrayPropertyPtr fFirstSample;		///< ��һ�� sample �� ID.
	Mp4ArrayPropertyPtr fSampleDescIndex;	///< 
};

typedef SmartPtr<Mp4Track> Mp4TrackPtr;
typedef Mp4Array<Mp4TrackPtr> Mp4TrackArray;

#endif // !defined(_NS_VISION_MP4_TRACK_H)
