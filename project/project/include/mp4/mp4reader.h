
#if !defined(_NS_VISION_MP4_READER_H_)
#define _NS_VISION_MP4_READER_H_

#include "mp4common.h"
#include "mp4atom.h"
#include "mp4track.h"

///////////////////////////////////////////////////////////////////////////////
// Mp4Reader

/** MP4 �ļ��⸴����(Reader). ��Ҫ���ڶ�ȡ MP4 �ļ�������. */
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
	BYTE*	fVideoBuffer;		///< ��Ƶ���ݻ�����
	UINT	fVideoOffset;		///< ��һ��Ҫ���ظ�Ӧ�ó������Ƶ���ݵĿ�ʼλ��
	UINT	fVideoBufferSize;	///< ��Ƶ���ݻ�������Ч���ݵĳ���
	UINT	fMaxBufferSize;		///< ��Ƶ���ݻ���������󳤶�
	INT64	fTimestamp;			///< ����ȡ����Ƶ��ʱ���
	UINT	fNaluIndex;			///< ��һ��Ҫ���ظ�Ӧ�ó������Ƶ NALU �������ڵ�ǰ֡��
	bool	fIsSyncSample;		///< ����ȡ����Ƶ�Ƿ��ǹؼ�֡
	UINT	fVideoSampleId;		///< ��һ��Ҫ��ȡ����Ƶ֡�� ID.
	INT64	fPosition;			///< ��ǰ���ŵ�λ��
	UINT	fAudioSampleId;		///< ��һ��Ҫ��ȡ����Ƶ֡�� ID.
	int		fVideoTrackIndex;	///< ��Ƶ Track ������ֵ
	int		fAudioTrackIndex;
	char*	fFileName;			///< �ļ�������

	Mp4AtomPtr		fRootAtom;	///< �� ATOM
	Mp4FilePtr		fMp4File;	///< MP4 �ļ�
	Mp4TrackArray	fTracks;	///< Track �б�

};

#endif // !defined(_NS_VISION_MP4_READER_H_)
