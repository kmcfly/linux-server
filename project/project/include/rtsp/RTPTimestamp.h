////////////////////////////////////////////////////////////////////////////////////////
/*
*
*����RTPʱ�����RTCPʱ�����ϵͳʱ��֮��Ĺ�ϵ
*
*/
#ifndef _RTP_TIMESTAMP_H_
#define _RTP_TIMESTAMP_H_

//#include "TVT_PubCom.h"			//3.0������   3.0���DD_Public
#include "DDPublic.h"
#include "RTSP_Common.h"

class CRTPTimestamp
{
public:
	CRTPTimestamp();
	~CRTPTimestamp();

	void SetStreamInfo(unsigned long capFre, unsigned long frameRate);

	unsigned long GetRTPTimestamp(ULONGLONG capTime);
	unsigned long GetRTPTimestamp();
	unsigned long UpdateRTPTS();

	unsigned long GetRTCPTimestamp();

	//void SetFrameRate(unsigned long frameRate);
	unsigned long GetFrameRate();
private:
	unsigned long m_capFre;						//����Ƶ��
	unsigned long m_frameRate;					//֡��
	unsigned long m_tsUnit;						//ÿһ֡���ӵ�ʱ�����С

	unsigned long m_curRTPTs;					//RTPʱ���
	unsigned long m_curRTCPTs;					//RTCPʱ���

	ULONGLONG	  m_offsetRTPWithCap;			//����RTP���ݵ�������ʼ��ƫ��ʱ��
	RTSP_DATE_TIME m_curDVRTime;

	unsigned long m_frameInterval;
	CPUB_Lock	  m_lock;
};
#endif /*_RTP_TIMESTAMP_H_*/
////////////////////////////////////////////////////////////////////////////////////////

