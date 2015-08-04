
#include "SpeedMan.h"
#include "AlarmOutMan.h"

CSpeedMan*CSpeedMan::m_pInstance = NULL;

CSpeedMan::CSpeedMan() : CAlarmIn()
{
	m_speedUnit = 0;
	m_speedSource = 0;
	memset(m_speedValue, 0, sizeof(m_speedValue));
}

CSpeedMan::~CSpeedMan()
{
}

CSpeedMan* CSpeedMan::Instance()
{
	if(m_pInstance == NULL)
	{
		m_pInstance = new CSpeedMan;
		assert(m_pInstance != NULL);
	}
	return m_pInstance;
}

void CSpeedMan::InitialStatus()
{
	for (unsigned char i = 0; i < m_chnnNum; i++)
	{
		m_pAlarmInStatus[i] = ALARMIN_STATUS_NO;
		m_pAlarmInAttrib[i].timerType = DVR_TIMER_SENSOR_IN;
	}
}


RECORD_TYPE CSpeedMan::GetRecType()
{
	//return RECORD_TYPE_SPEED;
	return RECORD_TYPE_SENSOR;
}

void CSpeedMan::OpenAlarm(unsigned long chnn)
{
	memset(&m_pEventInfo[chnn], 0, sizeof(EVENT_LOG));
	m_pEventInfo[chnn].chnn = (unsigned short)chnn;
	m_pEventInfo[chnn].startTime = GetCurrTime32();
	m_pEventInfo[chnn].type = EVENT_TYPE_SENSOR;

	Record(chnn);
	AlarmOut(chnn);
	toPTZ(chnn);
	BigView(chnn);
}

void CSpeedMan::CloseAlarm(unsigned long chnn)
{
	StopRecord(chnn);
	StopPTZ(chnn);

	m_pEventInfo[chnn].endTime = GetCurrTime32();
	m_pLogMan->WriteEventLog(m_pEventInfo[chnn]);
	memset(&m_pEventInfo[chnn], 0, sizeof(EVENT_LOG));
}

void CSpeedMan::SetSpeedUnit(unsigned long SpeedUnit)
{
	m_speedUnit = SpeedUnit;
}

void CSpeedMan::SetSpeedSource(unsigned long SpeedSource)
{
	m_speedSource = SpeedSource;
}

void CSpeedMan::SetSpeedValue(char *speedValue, unsigned int ch)
{
	memcpy(m_speedValue[ch], speedValue, 12);	
}

BYTE CSpeedMan::CompareSpeed(void)
{
	char speedtmp[10]={0};
	BYTE ret = 0;
	double speedcurr = 0;

	/*
	m_lock.Lock();	

	GPS_DATA gpsdate;
	CGPSCtrl *pgpsctrl= CGPSCtrl::Instance();
	if(pgpsctrl->GetGPSState())
	{
		pgpsctrl->GetGPSData(&gpsdate);
		sprintf(speedtmp,"%d.%d",gpsdate.speed.hightcell,gpsdate.speed.lowcell);

		//千米/小时
		if(m_speedunit == 1)
		{
			speedcurr = (atof(speedtmp))*1.852;
		}
		//海里/小时
		else if(m_speedunit == 2)
		{
			speedcurr = atof(speedtmp);
		}

		if(speedcurr>atof(m_speedTmp[0]))
		{
			ret |= 0x01;
		}
		else if(speedcurr<atof(m_speedTmp[1]))
		{
			ret |=0x02;
		}

		//printf("speedcurr=%f,m_speedTmp[0]=%f,speedTmp[1]=%f,ret=%x\n", speedcurr, atof(m_speedTmp[0]), atof(m_speedTmp[1]),ret);
	}

	m_lock.UnLock(); 
	*/
	
	ret = 0x01;
	return ret;

}
