// MultiReader.h: interface for the CMultiReader class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _MULTI_READER_H__
#define _MULTI_READER_H__

#include "OneChnnReader.h"

enum MULTI_READER_RET
{
	MR_RET_OK,					//�ɹ�
	MR_RET_TOO_MANY_CHANNELS,	//�ط�ͨ��̫�࣬�����˳�ʼ��ʱ�����õ����ͨ����
	MR_RET_NOTHING_TO_READ,		//û�ж����ɶ�
	MR_RET_PARAM_ERROR,			//��������
	MR_RET_UNKNOWN_ERROR		//δ֪����
};

typedef CMyList<unsigned long> CHANNEL_LIST;

typedef struct _local_reader
{
	unsigned long	chnn;
	unsigned long	bSwitch;
	LONGLONG		time;
	COneChnnReader	reader;
}LOCAL_READER, *LPLOCAL_READER;

class CMultiReader  
{
public:
	CMultiReader();
	virtual ~CMultiReader();
	

	bool  Initial(long maxCHNum, CReclogManEX *pRecLogManEx, unsigned long diskOwnerIndex);
	void Quit();

	long StartRead(FILE_INFO_EX_LIST *pFileList, ULONGLONG & outCH, const char *pFilePath = NULL);
	long StartRead(unsigned long startTime, unsigned long endTime, \
		const CHANNEL_LIST &chnnList, ULONGLONG & outCH, const char *pFilePath = NULL);
	long StartRead(unsigned long startTime, const CHANNEL_LIST &chnnList, ULONGLONG & outCH, const char *pFilePath);
	void StopRead();

	long SetCurTime(bool bForward, unsigned long time, ULONGLONG & outCH);
	long SetCurChannel(ULONGLONG chnnBits, ULONGLONG & outCH);

	long GetNextFrameType(FRAME_INFO_EX &frameInfo, bool bForward = true);
	long GetNextFrame(FRAME_INFO_EX &frameInfo, bool bForward = true, bool bVideoOnly = false);
	long GetKeyFrame(FRAME_INFO_EX &frameInfo, long nStepFrame = 1, bool bForward = true);

	bool HasAudioData(long channel);

	bool CheckSleep();
protected:
	int GetNextReadChannel(bool bForwardRead);

private:
	enum READ_TYPE
	{
		READ_TYPE_NULL,
		READ_BY_FILE_LIST,
		READ_BY_START_END,
		READ_BY_START,
	};

	READ_TYPE				m_readType;//��ʾ��ȡ����
	char					*m_pFilePath;
	LPLOCAL_READER			m_pReader;
	long					m_maxCHNum;

	LONGLONG				m_startTime;
	LONGLONG				m_endTime;
	LONGLONG				m_lastTime;
	ULONGLONG				m_chnnBits;
};

#endif // _MULTI_READER_H__
