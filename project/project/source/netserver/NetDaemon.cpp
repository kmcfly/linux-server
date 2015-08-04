#include "NetDaemon.h"

#ifndef __ENVIRONMENT_WIN32__
#include "dvr_func.h"
#include "NetInterface.h"
#endif

#ifdef __DVR_ULTIMATE__
#include "NetDeviceManager.h"
#endif

CNetDaemon::CNetDaemon(void)
{
	m_bRunning = false;
	m_daemonHandle = PUB_THREAD_ID_NOINIT;
}

CNetDaemon::~CNetDaemon(void)
{
}

bool CNetDaemon::Start()
{
	if(m_bRunning)
	{
		return true;
	}

	m_daemonHandle = PUB_CreateThread(DaemonThread, this, &m_bRunning);

	if(PUB_CREATE_THREAD_FAIL == m_daemonHandle)
	{
		return false;
	}

	return true;
}

void CNetDaemon::Stop()
{
	if(m_bRunning)
	{
		PUB_ExitThread(&m_daemonHandle, &m_bRunning);
	}
}

PUB_THREAD_RESULT PUB_THREAD_CALL CNetDaemon::DaemonThread(void *pParam)
{
	CNetDaemon *pThis = (CNetDaemon *)pParam;

	while(pThis->m_bRunning)
	{
#if defined(__DVR_ULTIMATE__) && !defined(__ENVIRONMENT_WIN32__)
		int needRestart = 0;
#ifdef __CHIP3531__
		ext_get_network_restart_flag(&needRestart);
#endif
		
		if(1 == needRestart)
		{
			CNetDeviceManager::Instance()->OnNetworkRestart();
			unsigned long ip;
			CNetInterface::Instance()->GetIPAddress(ip);
			DVRSystem("/mnt/mtd/eth_restart.sh");
			printf("restart the network.................\n");
			PUB_Sleep(2000);
			CNetInterface::Instance()->SetIP(ip);
		}
#endif

		PUB_Sleep(1000);
	}
	return 0;
}
