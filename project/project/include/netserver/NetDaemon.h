#ifndef _NET_DAEMON_H_
#define _NET_DAEMON_H_

#include "PUB_common.h"

class CNetDaemon
{
public:
	CNetDaemon(void);
	~CNetDaemon(void);

	bool Start();
	void Stop();

private:
	static PUB_THREAD_RESULT PUB_THREAD_CALL DaemonThread(void *pParam);

private:
	PUB_thread_t		m_daemonHandle;
	bool				m_bRunning;
};

#endif
