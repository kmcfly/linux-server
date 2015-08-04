////////////////////////////////////////////////////////////////////////////////////////
/*
*
*计算RTP时间戳和RTCP时间戳和系统时间之间的关系
*
*/
#ifndef _RTP_TIMESTAMP_H_
#define _RTP_TIMESTAMP_H_

//#include "TVT_PubCom.h"			//3.0不包含   3.0添加DD_Public
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
	unsigned long m_capFre;						//采样频率
	unsigned long m_frameRate;					//帧率
	unsigned long m_tsUnit;						//每一帧增加的时间戳大小

	unsigned long m_curRTPTs;					//RTP时间戳
	unsigned long m_curRTCPTs;					//RTCP时间戳

	ULONGLONG	  m_offsetRTPWithCap;			//发送RTP数据到采样开始的偏移时间
	RTSP_DATE_TIME m_curDVRTime;

	unsigned long m_frameInterval;
	CPUB_Lock	  m_lock;
};
#endif /*_RTP_TIMESTAMP_H_*/
////////////////////////////////////////////////////////////////////////////////////////

