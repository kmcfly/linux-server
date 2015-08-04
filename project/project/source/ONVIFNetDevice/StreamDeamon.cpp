
#include "NetDevicePlatform.h"
#include "NetDevice.h"

#include "StreamDeamon.h"

CStreamDeamon *CStreamDeamon::m_pInstance = NULL;

CStreamDeamon::CStreamDeamon(void)
{
	m_deamonThreadId = PUB_THREAD_ID_NOINIT;
	m_bDeamon = false;

#ifdef RUN_CHECK_THREAD_PROC
	m_checkThread = PUB_THREAD_ID_NOINIT;
	m_bCheckFlag = false;
#endif
}

CStreamDeamon::~CStreamDeamon(void)
{
}

CStreamDeamon *CStreamDeamon::Instance()
{
	if(NULL == m_pInstance)
	{
		m_pInstance = new CStreamDeamon;
	}

	return m_pInstance;
}

bool CStreamDeamon::Start()
{
	if(PUB_THREAD_ID_NOINIT != m_deamonThreadId)
	{
		return true;
	}

	m_deamonThreadId = PUB_CreateThread(DeamonThread, this, &m_bDeamon);
	if(PUB_CREATE_THREAD_FAIL == m_deamonThreadId)
	{
		assert(false);
		return false;
	}

#ifdef RUN_CHECK_THREAD_PROC
	m_checkThread = PUB_CreateThread(CheckThread, this, &m_bCheckFlag);
	if(PUB_CREATE_THREAD_FAIL == m_checkThread)
	{
		assert(false);
		return false;
	}
#endif

	return true;
}

bool CStreamDeamon::Stop()
{
	if(PUB_THREAD_ID_NOINIT != m_deamonThreadId)
	{
		PUB_ExitThread(&m_deamonThreadId, &m_bDeamon);
	}

#ifdef RUN_CHECK_THREAD_PROC
	if(PUB_THREAD_ID_NOINIT != m_checkThread)
	{
		PUB_ExitThread(&m_checkThread, &m_bCheckFlag);
	}
#endif

	return true;
}

void CStreamDeamon::AddDevice(COnvifNetDevice *pNetDevice)
{
	m_deamonLock.Lock();
	m_netDeviceSet.insert(pNetDevice);
	m_deamonLock.UnLock();
}

void CStreamDeamon::DelDevice(COnvifNetDevice *pNetDevice)
{
	m_deamonLock.Lock();
	m_netDeviceSet.erase(pNetDevice);
	m_deamonLock.UnLock();
}

#ifdef RUN_CHECK_THREAD_PROC
PUB_THREAD_RESULT PUB_THREAD_CALL CStreamDeamon::CheckThread(void *pParam)
{
	CStreamDeamon *pThis = (CStreamDeamon *)pParam;
	pThis->CheckProc();

	return 0;
}

void CStreamDeamon::CheckProc()
{
	set<COnvifNetDevice *>::iterator it;
	COnvifNetDevice *pNetDevice = NULL;
	while( m_bCheckFlag )
	{
		CClientRTPMain::Instance()->PrintRunPos();
		for(it = m_netDeviceSet.begin(); it != m_netDeviceSet.end(); it++)
		{
			pNetDevice = *it;
			pNetDevice->PrintRunPos();
		}
		PUB_Sleep(30000);
	}
}
#endif


PUB_THREAD_RESULT PUB_THREAD_CALL CStreamDeamon::DeamonThread(void *pParam)
{
	CStreamDeamon *pThis = (CStreamDeamon *)pParam;
	pThis->DeamonProc();

	return 0;
}

void CStreamDeamon::DeamonProc()
{
	bool bMajorNeedRestart, bSubNeedRestart,bThirdNeedRestart;
	bool bHasThirdStream;
	unsigned long curTimes;
	unsigned long times_for_chk_rertp = 0;
	
	while(m_bDeamon)
	{
		curTimes = GetTickCount();

		m_deamonLock.Lock();
		set<COnvifNetDevice *>::iterator it;
		for(it = m_netDeviceSet.begin(); it != m_netDeviceSet.end(); it++)
		{
			COnvifNetDevice *pNetDevice = *it;

			//if( ++times_for_chk_rertp > 0 )
			{
				pNetDevice->CheckRtpConnectTimeout( curTimes );
				//times_for_chk_rertp = 0;
			}



			pNetDevice->GetRtspRestart(bMajorNeedRestart, bSubNeedRestart,bThirdNeedRestart, bHasThirdStream);

			if(bMajorNeedRestart)
			{
				pNetDevice->RestartRtsp(0);
			}

			if( bHasThirdStream )
			{
				if(bSubNeedRestart)
				{
					pNetDevice->RestartRtsp(1);
				}

				if(bThirdNeedRestart)
				{
					pNetDevice->RestartRtsp(2);
				}

			}
			else
			{
				if(bSubNeedRestart)
				{
					pNetDevice->RestartRtsp(1);
				}
				else if(bThirdNeedRestart)
				{
					pNetDevice->RestartRtsp(2);
				}
			}
		}
		m_deamonLock.UnLock();

		PUB_Sleep(500);
	}
}

