
#ifndef __HX_NET_READER_H__
#define __HX_NET_READER_H__

#include "FrameData.h"
#include "HxMultiReader.h"
#include "MessageMan.h"

typedef struct _client_frame_temp_
{
	unsigned long clientID;
	std::list<CFrameData *> *pFrameList;
}CLIENT_FRAME_TEMP;

class CHxNetReader
{
public:
	CHxNetReader(void);
	~CHxNetReader(void);
	static CHxNetReader * Instance();

	bool Initial(CReclogManEX *pReclogManEx, CMessageMan *pMsgMan, const char *pFilePath = NULL);
	void Quit();

	bool Start();
	void Stop();

	bool StartRead(unsigned long clientID, long channel, unsigned long startTime, unsigned long endTime, unsigned long diskOwnerIndex);
	void StopRead(unsigned long clientID);

private:
	static	RESULT WINAPI ReadThread(LPVOID lpParam);

	bool CreateReadThd();
	void CloseReadThd();

	void ReadProc();

	bool AddNewFrame(unsigned long clientID, CFrameData * pFrameData);
	bool DelUsedFrame();
private:
	static CHxNetReader *		m_pHxNetReader;

	HANDLE						m_hReadThread;
	bool						m_bReading;

	CHxMultiReader				m_reader;

	CPUB_Lock					m_frameTempListLock;
	std::list<CLIENT_FRAME_TEMP> m_clientFrameTempList;

	CReclogManEX				*m_pReclogMan;
	CMessageMan					*m_pMsgMan;

	char						*m_pFilePath;
};

#endif
