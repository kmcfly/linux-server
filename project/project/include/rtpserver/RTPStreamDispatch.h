#ifndef __RTP_STREAM_DISPATCH
#define __RTP_STREAM_DISPATCH

#include "PUB_common.h"
#include "ServerRTPMan.h"
#include "RTSP_Common.h"


class CRTPStreamDispatch
{
public:
	CRTPStreamDispatch(void);
	~CRTPStreamDispatch(void);

	bool Initial();
	void Quit();

	bool Start();
	void Stop();

private:
	static PUB_THREAD_RESULT PUB_THREAD_CALL DispatchStreamThread(void *pParam);
	void DispatchStreamProc();

private:
//线程相关
	PUB_thread_t			m_hThread;
	bool					m_bRunning;

};
#endif
