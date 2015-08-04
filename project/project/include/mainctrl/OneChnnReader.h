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

	//��reclogManע���ע�������ļ�������
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
	int					m_readerType;		//			��¼�ļ�����
	FILE_INFO_EX_LIST	m_fileList;			//	0		��Ű�ʱ�������������ļ��б�
	bool				m_bOpenFile;		//	false	��¼�Ƿ����ļ���.
	bool				m_bReVideoFmat;		//	false	��¼�Ƿ����µ���Ƶ��ʽ
	bool				m_bReAudioFmat; 	//	false	��¼�Ƿ����µ���Ƶ��ʽ
	BITMAPINFOHEADER	m_bitmapInfo;		//	0		���������Ƶ��ʽ
	WAVEFORMATEX		m_waveFormat;		//	0		���������Ƶ��ʽ
	READ_STATE			m_readState;		//	READER_STATE_STOP	��¼״̬,�Ա���������ʱ����
	unsigned long		m_channel;			//	0		��¼ͨ����
	POS					m_currPos;			//	NULL	��¼��ǰ�ļ���λ��
	char				m_filePath[256];	//	0		��¼��ǰ�ļ���·��
	char				m_recPath[256];

	bool				m_bDynSearch;		//false		true��ʾΪ��̬�����ļ��б�

	unsigned long       m_diskOwnerIndex;

	bool                m_bRegister;
	FILE_INFO_EX        m_readFileInfo;

	LONGLONG			m_videoTime;
};

#endif




