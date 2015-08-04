
#ifndef _STREAM_DEAMON_H_
#define _STREAM_DEAMON_H_

#include "DDPublic.h"
#include "OnvifNetDeviceServ.h"
#include <set>


//#define RUN_CHECK_THREAD_PROC 1

using std::set;

class CStreamDeamon
{
public:
	static CStreamDeamon *Instance();
	bool Start();
	bool Stop();

	void AddDevice(COnvifNetDevice *pNetDevice);
	void DelDevice(COnvifNetDevice *pNetDevice);

private:
	CStreamDeamon(void);
	~CStreamDeamon(void);

	static PUB_THREAD_RESULT PUB_THREAD_CALL DeamonThread(void *pParam);
	void DeamonProc();

#ifdef RUN_CHECK_THREAD_PROC
	static PUB_THREAD_RESULT PUB_THREAD_CALL CheckThread(void *pParam);
	void CheckProc();
#endif

private:
	static CStreamDeamon			*m_pInstance;
	set<COnvifNetDevice *>			m_netDeviceSet;
	CPUB_Lock						m_deamonLock;

	PUB_thread_t					m_deamonThreadId;
	bool							m_bDeamon;

#ifdef RUN_CHECK_THREAD_PROC
	PUB_thread_t                    m_checkThread;
	bool                            m_bCheckFlag;
#endif
};

#endif
