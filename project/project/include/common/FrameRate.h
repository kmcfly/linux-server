/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : 袁石维
** Date         : 2010-01-07
** Name         : (
** Version      : 1.0
** Description  :
** Modify Record:
1:
***********************************************************************/
#ifndef _FRAME_RATE_H_
#define _FRAME_RATE_H_

#include "Product.h"

class CFrameRate
{
public:
	CFrameRate();
	~CFrameRate();

	bool Initial(VIDEO_FORMAT videoFormat, unsigned char chnnNum, unsigned long maxRateTotal, unsigned long maxRateCH, ULONGLONG effectCH);
	void Quit();

	//////////////////////////////////////////////////////////////////////////
	void AddRate(unsigned char chnn, unsigned char rate, VIDEO_SIZE videoSize);
	bool TryRate(unsigned char chnn, unsigned char & rate, VIDEO_SIZE & videoSize);
	bool TryRateEx(unsigned char chnn, unsigned char & rate, VIDEO_SIZE & videoSize);
	unsigned long ModifyRate(unsigned char chnn, unsigned char rate, VIDEO_SIZE videoSize, bool bLimitForMaxRatCH = true);
	unsigned long ModifyRateEx(unsigned char chnn, unsigned char rate, VIDEO_SIZE videoSize, bool bLimitForMaxRatCH = true);
	unsigned long Remaining();
	unsigned long RemainingEx();
protected:

private:
	unsigned long Rate();
	unsigned long RateEx();
	//////////////////////////////////////////////////////////////////////////
	typedef struct _rate_info
	{
		unsigned long	rate;
		unsigned long	videoSize;
	}RATE_INFO;

	VIDEO_FORMAT	m_videoFormat;
	unsigned long	m_chnnNum;
	unsigned long	m_maxRateTotal;
	unsigned long	m_maxRateCH;
	
	ULONGLONG		m_effectCH;			//有效数据通道

	RATE_INFO		*m_pRateInfo;
};
#endif	//_FRAME_RATE_H_

