#ifndef __FTP_LOAD_PIC__
#define __FTP_LOAD_PIC__

#include "FtpLoad.h"
#include <list>

class CFtpLoadPic : public CFtpLoad
{
public:
	CFtpLoadPic();
	~CFtpLoadPic();

	bool Start();
	void Stop();
	int	UpLoad(const char*pFileName, CFrameData *pFrameData);
	void SetBufLen(unsigned long BufLen) {m_BufLen = BufLen;}
	CFtpPI* GetFtpPI() {return &m_FtpPI;}
	
protected:
	static	PUB_THREAD_RESULT PUB_THREAD_CALL FtpDataProcThread(void *pParam);
	void FtpDataProc();
	int	RealSend(CFrameData *pFrameData, unsigned int SendPos);
	int	ClearAllData();
	bool NextUpLoad();

protected:
	std::list<char*>			m_listFileName;
	std::list<CFrameData*>		m_listFrameData;

	unsigned long				m_FrameDataLen;
	unsigned long				m_BufLen;
};


#endif
