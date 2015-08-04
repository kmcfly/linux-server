#ifndef __TUTK_STREAM_DISPATCH__
#define __TUTK_STREAM_DISPATCH__

#include "PUB_common.h"
#include "TVT_IOTCDevice.h"

class CTUTKStreamDispatch
{
public:
	CTUTKStreamDispatch(void);
	~CTUTKStreamDispatch(void);

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
