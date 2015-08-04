#include "RTPTimestamp.h"
#include "RTPHeader.h"
#include "RTSP_Common.h"
//#include "Profile.h"

#ifdef _DEBUG
#define  new DEBUG_NEW
#endif


CRTPTimestamp::CRTPTimestamp()
{
	m_capFre			= 0;
	m_frameRate			= 0;

	m_tsUnit		= 0;
	m_curRTPTs	= 0;
	m_curRTCPTs	= 0;

	//m_curDVRTime = 0;
}

CRTPTimestamp::~CRTPTimestamp()
{

}

void CRTPTimestamp::SetStreamInfo(unsigned long capFre, unsigned long frameRate)
{
	m_lock.Lock();
	m_capFre	= capFre;
	m_frameRate	= frameRate;
	m_tsUnit	= m_capFre / frameRate;//时间戳增量
	m_curRTPTs	= rand();
	m_curRTCPTs	= m_curRTPTs;
	m_lock.UnLock();
	m_curDVRTime = RTSPGetCurrTime();
}

void CRTPTimestamp::SetStreamInfo(unsigned long frameRate)
{
	m_lock.Lock();
	m_tsUnit = m_capFre / frameRate;
	m_lock.UnLock();
}

unsigned long CRTPTimestamp::GetAndIncRTPTs()
{
	m_lock.Lock();
	m_curRTPTs += m_tsUnit;
	unsigned long ret = m_curRTPTs;
	m_lock.UnLock();

	m_curDVRTime = RTSPGetCurrTime();//这里由创建RTP数据包时调用，保存上一次发送RTP数据时的时间

	return ret;
}

unsigned long CRTPTimestamp::GetRTPTimestamp()
{
	m_lock.Lock();
	unsigned long ret = m_curRTPTs;
	m_lock.UnLock();
	return ret;
}


unsigned long CRTPTimestamp::GetRTCPTimestamp()
{
	//这里由发送SR要取得时间戳时调用，计算与上一次发送RTP数据的时间差
	RTSP_DATE_TIME CurTime = RTSPGetCurrTime();
	ULONGLONG offset = (CurTime.seconds - m_curDVRTime.seconds)*1000000 + (CurTime.microsecond - m_curDVRTime.microsecond);//偏移了多少微秒
	double timeIncFrac = (double)m_capFre/1000000;//每微秒时间戳的增量
	unsigned long RTPInc = offset * timeIncFrac;//总的增量
	m_lock.Lock();
	m_curRTCPTs =  RTPInc + m_curRTPTs;
	unsigned long ret = m_curRTCPTs;
	m_lock.UnLock();

	return ret;
}

unsigned long CRTPTimestamp::GetFrameRate()
{
	return m_frameRate;
}

RTSP_DATE_TIME CRTPTimestamp::GetLastNTPTime()
{
	return m_curDVRTime;
}

void CRTPTimestamp::ChangeFrameRate(unsigned long frameRate)
{
	m_frameRate = frameRate;
	m_tsUnit	= m_capFre / frameRate;
}