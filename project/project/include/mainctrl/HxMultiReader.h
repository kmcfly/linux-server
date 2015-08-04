
#ifndef __HX_MULTI_READER_H__
#define __HX_MULTI_READER_H__

#include <vector>

#include "OneChnnReader.h"
#include "HxCycleBuffer.h"

enum hx_read_ret
{
	HX_READ_SUCCESS,
	HX_READ_ERR_NO_DATA,
	HX_READ_ERR_NO_BUFFER,
};

typedef struct  
{
	COneChnnReader			*pReader;
	CHxCycleBuffer			*pBuffer;
	unsigned long			streamID;
	unsigned long			startTime;
	unsigned long			stopTime;
	unsigned long			playEnd;
	unsigned long			chnnNum;
}HX_READER;

class CHxMultiReader
{
public:
	CHxMultiReader(void);
	~CHxMultiReader(void);

	bool Initial(CReclogManEX *pRecLogManEx, const char *pFilePath);
	void Quit();

	bool Start(unsigned long streamID, unsigned long startTime, unsigned long stopTime, long channel, long diskOwner);
	void Stop(unsigned long streamID);

	long GetNextFrame(FRAME_INFO_EX &frameInfo);
	void ReleaseFrame(const FRAME_INFO_EX &frameInfo);

private:
	std::vector<HX_READER>	m_readers;
	long					m_readerIndex;

	CReclogManEX			*m_pRecLogMan;

	CPUB_Lock				m_lock;
	char					*m_pFilePath;
};

#endif
