////////////////////////////////////////////////////////////////////////////////////////
/*
*
*����RTPʱ�����RTCPʱ�����ϵͳʱ��֮��Ĺ�ϵ
*
*/
#ifndef _RTP_TIMESTAMP_H_
#define _RTP_TIMESTAMP_H_

//#include "TVT_PubCom.h"			//3.0������   3.0���DD_Public
#include "PUB_common.h"
#include "RTSP_Common.h"

class CRTPTimestamp
{
public:
	CRTPTimestamp();
	~CRTPTimestamp();

	void SetStreamInfo(unsigned long capFre, unsigned long frameRate);
	void SetStreamInfo(unsigned long frameRate);
	unsigned long GetAndIncRTPTs();
	unsigned long GetRTPTimestamp();
	RTSP_DATE_TIME GetLastNTPTime();
	unsigned long GetRTCPTimestamp();
	void ChangeFrameRate(unsigned long frameRate);

	unsigned long GetFrameRate();
private:
	unsigned long m_capFre;						//����Ƶ��
	unsigned long m_frameRate;					//֡��
	unsigned long m_tsUnit;						//ÿһ֡���ӵ�ʱ�����С

	unsigned long m_curRTPTs;					//RTPʱ���
	unsigned long m_curRTCPTs;					//RTCPʱ���

	RTSP_DATE_TIME m_curDVRTime;

	unsigned long m_frameInterval;
	CPUB_Lock	  m_lock;
};
#endif /*_RTP_TIMESTAMP_H_*/
////////////////////////////////////////////////////////////////////////////////////////

