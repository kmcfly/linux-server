#include "TvtAlarm.h"
#include "MessageMan.h"
#include "NetServerSDK.h"

CTvtAlarm::CTvtAlarm()
{
	m_ullMotionOld = 0;
	m_ullSensorOld = 0;
	m_bTvtAlarmRun = false;
	m_TvtAlarmID =PUB_THREAD_ID_NOINIT;

}

CTvtAlarm::~CTvtAlarm()
{

}
CTvtAlarm* CTvtAlarm::Instance()
{
	static CTvtAlarm s_pTvtAlarm;
	return &s_pTvtAlarm;
}


void CTvtAlarm::CheckProc()
{
	ULONGLONG ullMotionState;
	ULONGLONG ullSensorState;
	ULONGLONG ullVideoLossState;
	while(m_bTvtAlarmRun)
	{
		CMessageMan::Instance()->GetCHStatus(CHNN_STATUS_MOTION, ullMotionState);
		CMessageMan::Instance()->GetCHStatus(CHNN_STATUS_SENSOR_IN, ullSensorState);
		CMessageMan::Instance()->GetCHStatus(CHNN_STATUS_VIDEO_LOSS, ullVideoLossState);
		ULONGLONG ullMotionNew = ullMotionState;
		ULONGLONG ullSensorNew = ullSensorState;
		ULONGLONG ullVideoLossNew = ullVideoLossState;

		if(ullMotionNew != m_ullMotionOld)
		{
			tuint32 n = 0;
			while(m_ullMotionOld != ullMotionNew)
			{
				if(m_ullMotionOld%2 != ullMotionNew%2)
				{
					NET_SERVER_SetMotionStatus(n, ullMotionNew%2);
					m_ullMotionOld = m_ullMotionOld/2;
					ullMotionNew = ullMotionNew/2;
				}
				else
				{
					m_ullMotionOld = m_ullMotionOld/2;
					ullMotionNew = ullMotionNew/2;
				}
				n++;
			}
			m_ullMotionOld = ullMotionState;
		}
		if(ullSensorNew != m_ullSensorOld)
		{
			tuint32 n = 0;
			while(m_ullSensorOld != ullSensorNew)
			{
				if(m_ullSensorOld%2 != ullSensorNew%2)
				{
					NET_SERVER_SetSensorStatus(n, ullSensorNew%2);
					m_ullSensorOld = m_ullSensorOld/2;
					ullSensorNew = ullSensorNew/2;
				}
				else
				{
					m_ullSensorOld = m_ullSensorOld/2;
					ullSensorNew = ullSensorNew/2;
				}
				n++;
			}
			m_ullSensorOld = ullSensorState;
		}

		if(ullVideoLossNew != m_ullVideoLossOld)
		{
			tuint32 n = 0;
			while(m_ullVideoLossOld != ullVideoLossNew)
			{
				if(m_ullVideoLossOld%2 != ullVideoLossNew%2)
				{
					NET_SERVER_SetVideoLossStatus(n, ullVideoLossNew%2);
					m_ullVideoLossOld = m_ullVideoLossOld/2;
					ullVideoLossNew = ullVideoLossNew/2;
				}
				else
				{
					m_ullVideoLossOld = m_ullVideoLossOld/2;
					ullVideoLossNew = ullVideoLossNew/2;
				}
				n++;
			}
			m_ullVideoLossOld = ullVideoLossState;
		}

		PUB_Sleep(1000);
	}
}

int CTvtAlarm::Start()
{
		if(!m_bTvtAlarmRun)
		{
			m_TvtAlarmID = PUB_CreateThread(TvtAlarmThread, this, &m_bTvtAlarmRun);
			if(m_TvtAlarmID == PUB_CREATE_THREAD_FAIL)
			{
				PUB_PrintError(__FILE__,__LINE__);
				return -1;
			}
		}
		return 0;
}

int CTvtAlarm::Stop()
{
	if(m_bTvtAlarmRun)
	{
		PUB_ExitThread(&m_TvtAlarmID, &m_bTvtAlarmRun);
	}
	return 0;
}

PUB_THREAD_RESULT PUB_THREAD_CALL CTvtAlarm::TvtAlarmThread(void *pParam)
{
#ifdef __ENVIRONMENT_LINUX__
	pid_t pid = getpid();
	pid_t tid = syscall(__NR_gettid);
	printf("%s, %d, pid = %d, tid=%d\n", __FILE__, __LINE__, pid, tid);
#endif

	CTvtAlarm *pThis = reinterpret_cast<CTvtAlarm *>(pParam);
	pThis->CheckProc();
	return 0;
}