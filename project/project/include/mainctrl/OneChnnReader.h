//OneChnnReader.h

#ifndef ONE_CHNN_READER_H
#define ONE_CHNN_READER_H

#if defined(__PRIMARY_WRDISK__)
#include "PrimaryRecReader.h"
#else
#include "RecReader.h"
#endif

#include "AVIReaderEx.h"
#include "mylist.cpp"
#include "ReclogManEx.h"

typedef enum _read_state_
{
	READ_STATE_STOP,
	READ_STATE_FORWARD_SLEEP,
	READ_STATE_BEHIND_SLEEP,
	READ_STATE_ACTION
}READ_STATE;

enum 
{
	SEEK_FRAME_OK,
	SEEK_FRAME_FAIL,
	SEEK_FRAME_SLEEP
};

enum
{
	ONE_CHNN_READ_FAIL,
	ONE_CHNN_READ_SUC,
	ONE_CHNN_READ_SLEEP
};

enum
{
	FILE_TYPE_FTVT,
	FILE_TYPE_AVI
};

class COneChnnReader
{
public:
	COneChnnReader();
	~COneChnnReader();


	bool Initial(CReclogManEX *pReclogManEx, unsigned long diskOwnerIndex);
	void Quit();
	long StartRead(unsigned long startTime, unsigned long endTime, unsigned long chnn, const char *pRecPath = NULL);
	long StartRead(const FILE_INFO_EX_LIST & fileList, const char *pRecPath = NULL);
	long StartRead(unsigned long playTime, unsigned long chnn, const char *pRecPath = NULL);
	void StopRead();
	long GetFrame(FRAME_INFO_EX &frameInfo);
	LONGLONG GetFrameTime();
	FRAME_TYPE GetFrameType();
	long SeekFrame(SEEK_MODE mode, unsigned long num = 1);
	long GetFrameLen();
#if 0
	long SeekFrameToTime(bool bFuture, LONGLONG time);
#else
	long SeekFrameToTime(bool bForward, unsigned long time);
#endif

	long GetReadState();
	long WakeUp ();

protected:
	bool GetFilePathEx(POS filePos);
	long OpenFile(unsigned long startTime,unsigned long channel);
	void CloseFile();
	long GetPrevFile();
	long GetNextFile();
	void RemoveFileList();
	unsigned long SeekVideoFrame(SEEK_MODE mode, unsigned long seekNum = 1);
	unsigned long SeekKeyFrameToTime(unsigned long time);
	bool GetVideoInfo(BITMAPINFOHEADER &bitmapInfo);
	bool GetAudioInfo(WAVEFORMATEX &waveFormat);
	unsigned long GetCurrFrame(FRAME_INFO_EX &frameInfo);

	//向reclogMan注册和注销被读文件的索引
	void RegisterReadFileIndex( );
	void CancelReadFileIndex( );

protected:
	CReclogManEX		*m_pReclogMan;
	CAVIReader			m_cAviReader;
#if defined(__PRIMARY_WRDISK__)
	CPrimaryRecReader	m_cRecReader;
#else
	CRecReader			m_cRecReader;
#endif
	int					m_readerType;		//			记录文件类型
	FILE_INFO_EX_LIST	m_fileList;			//	0		存放按时间搜索而来的文件列表。
	bool				m_bOpenFile;		//	false	记录是否有文件打开.
	bool				m_bReVideoFmat;		//	false	记录是否获得新的视频格式
	bool				m_bReAudioFmat; 	//	false	记录是否获得新的音频格式
	BITMAPINFOHEADER	m_bitmapInfo;		//	0		存放最新视频格式
	WAVEFORMATEX		m_waveFormat;		//	0		存放最新音频格式
	READ_STATE			m_readState;		//	READER_STATE_STOP	记录状态,以便有数据随时唤醒
	unsigned long		m_channel;			//	0		记录通道号
	POS					m_currPos;			//	NULL	记录当前文件的位置
	char				m_filePath[256];	//	0		记录当前文件的路径
	char				m_recPath[256];

	bool				m_bDynSearch;		//false		true表示为动态搜索文件列表

	unsigned long       m_diskOwnerIndex;

	bool                m_bRegister;
	FILE_INFO_EX        m_readFileInfo;

	LONGLONG			m_videoTime;
};

#endif




