
#if !defined(_NS_VISION_MP4_READER_H_)
#define _NS_VISION_MP4_READER_H_

#include "mp4common.h"
#include "mp4atom.h"
#include "mp4track.h"

///////////////////////////////////////////////////////////////////////////////
// Mp4Reader

/** MP4 文件解复用器(Reader). 主要用于读取 MP4 文件的内容. */
class Mp4Reader
{
public:
	Mp4Reader();
	virtual ~Mp4Reader();

// Attributes -------------------------------------------------
public:
	UINT GetWidth();
	UINT GetHeight();
	UINT GetFrameRate(bool isVideo = TRUE);
	UINT GetBitrate(bool isVideo = TRUE);
	UINT GetSampleCount(bool isVideo = TRUE);

	UINT GetDuration();
	UINT GetTimeScale();

	INT64 GetPosition() { return fPosition; }
	void SetPosition(INT64 posotion);

	Mp4FilePtr GetFile() { return fMp4File; }
	Mp4AtomPtr GetRootAtom() { return fRootAtom; }
	int GetTrackCount() { return fTracks.GetCount(); }

// Operations -------------------------------------------------
public:
	virtual LRESULT Open(LPCSTR name);
	virtual void Close();
	void Seek(INT64 time);

	Mp4TrackPtr GetTrack(int index);
	Mp4TrackPtr GetTrack(LPCSTR type);

	bool GetNextVideoSample(BYTE* buf, UINT* length, INT64* timestamp, INT64* timeScale);
	bool GetNextAudioSample(BYTE* buf, UINT* length, INT64* timestamp, INT64* timeScale);

// Implementation ---------------------------------------------
protected:
	int GenerateTracks();

// Data Members -----------------------------------------------
protected:	
	BYTE*	fVideoBuffer;		///< 视频数据缓存区
	UINT	fVideoOffset;		///< 下一次要返回给应用程序的视频数据的开始位置
	UINT	fVideoBufferSize;	///< 视频数据缓存区有效数据的长度
	UINT	fMaxBufferSize;		///< 视频数据缓存区的最大长度
	INT64	fTimestamp;			///< 最后读取的视频的时间戳
	UINT	fNaluIndex;			///< 下一次要返回给应用程序的视频 NALU 的索引在当前帧数
	bool	fIsSyncSample;		///< 最后读取的视频是否是关键帧
	UINT	fVideoSampleId;		///< 下一次要读取的视频帧的 ID.
	INT64	fPosition;			///< 当前播放的位置
	UINT	fAudioSampleId;		///< 下一次要读取的音频帧的 ID.
	int		fVideoTrackIndex;	///< 视频 Track 的索引值
	int		fAudioTrackIndex;
	char*	fFileName;			///< 文件的名称

	Mp4AtomPtr		fRootAtom;	///< 根 ATOM
	Mp4FilePtr		fMp4File;	///< MP4 文件
	Mp4TrackArray	fTracks;	///< Track 列表

};

#endif // !defined(_NS_VISION_MP4_READER_H_)
