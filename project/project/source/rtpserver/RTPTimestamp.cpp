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
	m_tsUnit	= m_capFre / frameRate;//ʱ�������
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

	m_curDVRTime = RTSPGetCurrTime();//�����ɴ���RTP���ݰ�ʱ���ã�������һ�η���RTP����ʱ��ʱ��

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
	//�����ɷ���SRҪȡ��ʱ���ʱ���ã���������һ�η���RTP���ݵ�ʱ���
	RTSP_DATE_TIME CurTime = RTSPGetCurrTime();
	ULONGLONG offset = (CurTime.seconds - m_curDVRTime.seconds)*1000000 + (CurTime.microsecond - m_curDVRTime.microsecond);//ƫ���˶���΢��
	double timeIncFrac = (double)m_capFre/1000000;//ÿ΢��ʱ���������
	unsigned long RTPInc = offset * timeIncFrac;//�ܵ�����
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