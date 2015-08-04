/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : 袁石维
** Date         : 2010-01-07
** Name         : FrameRate.cpp
** Version      : 1.0
** Description  :
** Modify Record:
1:
***********************************************************************/
#include "FrameRate.h"

CFrameRate::CFrameRate() : m_videoFormat(VIDEO_FORMAT_NTSC), m_chnnNum(0), m_maxRateTotal(0), m_maxRateCH(0)
{
	m_effectCH = INVALID_INDEX(ULONGLONG);
	m_pRateInfo = NULL;
}

CFrameRate::~CFrameRate()
{

}

bool CFrameRate::Initial(VIDEO_FORMAT videoFormat, unsigned char chnnNum, unsigned long maxRateTotal, unsigned long maxRateCH, ULONGLONG effectCH)
{
	assert ((0 <= chnnNum) && (chnnNum <= MAX_CHNN_NUM));

	//至少每个通道有一帧
	if (VIDEO_FORMAT_NTSC == videoFormat)
	{
		assert ((0 < maxRateCH) && (maxRateCH <= 600));
	}
	else
	{
		assert ((0 < maxRateCH) && (maxRateCH <= 500));
	}

	//总资源不能超出所有通道的最大值。
	//assert ((chnnNum <= maxRateTotal) && (maxRateTotal <= (chnnNum * maxRateCH)));

	m_videoFormat	= videoFormat;
	m_chnnNum		= chnnNum;
	m_maxRateTotal	= maxRateTotal;
	m_maxRateCH		= maxRateCH;
	m_effectCH		= effectCH;

	m_pRateInfo		= new RATE_INFO [m_chnnNum];
	assert (NULL != m_pRateInfo);

	return true;
}

void CFrameRate::Quit()
{
	if (NULL != m_pRateInfo)
	{
		delete [] m_pRateInfo;
		m_pRateInfo = NULL;
	}

	m_videoFormat	= VIDEO_FORMAT_NTSC;
	m_chnnNum		= 0;
	m_maxRateTotal	= 0;
	m_maxRateCH		= 0;
	m_effectCH		= INVALID_INDEX(ULONGLONG);
}

//////////////////////////////////////////////////////////////////////////
//把某个通道使用的资源情况加入到资源分配表中，用于初始化后使用。
void CFrameRate::AddRate(unsigned char chnn, unsigned char rate, VIDEO_SIZE videoSize)
{
	assert (NULL != m_pRateInfo);
//	assert (chnn < m_chnnNum);

	if(chnn >= m_chnnNum)
	{
		return;
	}

	if(!((m_effectCH >> chnn) & 0x1))
	{
		return;
	}

	if (VIDEO_FORMAT_NTSC == m_videoFormat)
	{
		assert ((0 < rate) && (rate <= 30));
	}
	else
	{
		assert ((0 < rate) && (rate <= 25));
	}

	m_pRateInfo[chnn].rate		= rate;
	m_pRateInfo[chnn].videoSize	= videoSize;
}

//检查给某个通道设置如此参数是否能成功，如果失败，则会返回建议值。
bool CFrameRate::TryRate(unsigned char chnn, unsigned char & rate, VIDEO_SIZE & videoSize)
{
	assert (NULL != m_pRateInfo);
	assert (chnn < m_chnnNum);
	if(!((m_effectCH >> chnn) & 0x1))
	{
		return false;
	}

	if (VIDEO_FORMAT_NTSC == m_videoFormat)
	{
		assert ((0 < rate) && (rate <= 30));
	}
	else
	{
		assert ((0 < rate) && (rate <= 25));
	}

	//获得已经使用的资源 = 总资源 - 该通道使用的资源。
	unsigned long totalRate = Rate() - UsedRate(m_pRateInfo[chnn].rate, static_cast<VIDEO_SIZE>(m_pRateInfo[chnn].videoSize));

	//不能超出对单个通道最大资源数目的限制。
	unsigned long lastRate = 0;
	if (m_maxRateTotal <= totalRate)
	{
		rate = m_pRateInfo[chnn].rate;
		videoSize = (VIDEO_SIZE)m_pRateInfo[chnn].videoSize;
		return false;
	}
	else
	{
		//存在下面的情况：比如系统的能力为480帧，当前配置到480帧，其中有1个通道为CIF，1帧.
		//当把这个通道的CIF，修成D1。就会出现系统的能力超出了480，就需要对这个通道降帧。 
		//而这个通道的帧率为1帧，所以不能再通过降低帧率的方法，使系统达到480。
		unsigned long lastRate  = ((m_maxRateTotal - totalRate) <= m_maxRateCH) ? (m_maxRateTotal - totalRate) : m_maxRateCH;

		lastRate = RealRate(lastRate, videoSize);
		
		if (rate <= lastRate)
		{
			return true;
		}
		else if (lastRate <= 0)
		{
			rate = m_pRateInfo[chnn].rate;
			videoSize = (VIDEO_SIZE)m_pRateInfo[chnn].videoSize;
			return false;
		}
		else
		{
			rate = static_cast<unsigned char >(lastRate);
			return false;
		}
	}
	
}

bool CFrameRate::TryRateEx(unsigned char chnn, unsigned char & rate, VIDEO_SIZE & videoSize)
{
	assert (NULL != m_pRateInfo);
	assert (chnn < m_chnnNum);
	if(!((m_effectCH >> chnn) & 0x1))
	{
		return false;
	}

	if (VIDEO_FORMAT_NTSC == m_videoFormat)
	{
		assert ((0 < rate) && (rate <= 30));
	}
	else
	{
		assert ((0 < rate) && (rate <= 25));
	}

	//获得已经使用的资源 = 总资源 - 该通道使用的资源。
	unsigned long totalRate = RateEx() - UsedRateEx(m_pRateInfo[chnn].rate, static_cast<VIDEO_SIZE>(m_pRateInfo[chnn].videoSize));

	//不能超出对单个通道最大资源数目的限制。
	unsigned long lastRate = 0;
	if (m_maxRateTotal <= totalRate)
	{
		rate = m_pRateInfo[chnn].rate;
		videoSize = (VIDEO_SIZE)m_pRateInfo[chnn].videoSize;
		return false;
	}
	else
	{
		//存在下面的情况：比如系统的能力为480帧，当前配置到480帧，其中有1个通道为CIF，1帧.
		//当把这个通道的CIF，修成D1。就会出现系统的能力超出了480，就需要对这个通道降帧。 
		//而这个通道的帧率为1帧，所以不能再通过降低帧率的方法，使系统达到480。
		unsigned long lastRate  = ((m_maxRateTotal - totalRate) <= m_maxRateCH) ? (m_maxRateTotal - totalRate) : m_maxRateCH;

		lastRate = RealRateEx(lastRate, videoSize);

		if (rate <= lastRate)
		{
			return true;
		}
		else if (lastRate <= 0)
		{
			rate = m_pRateInfo[chnn].rate;
			videoSize = (VIDEO_SIZE)m_pRateInfo[chnn].videoSize;
			return false;
		}
		else
		{
			rate = static_cast<unsigned char >(lastRate);
			return false;
		}
	}
}

//实际调整某个通道使用的数据，如果检查不过关，则恢复到之前的数据。并返回剩余资源数目（CIF）。
unsigned long CFrameRate::ModifyRate(unsigned char chnn, unsigned char rate, VIDEO_SIZE videoSize, bool bLimitForMaxRatCH)
{
	assert (NULL != m_pRateInfo);
	assert (chnn < m_chnnNum);

	if(!((m_effectCH >> chnn) & 0x1))
	{
		return false;
	}
	if (VIDEO_FORMAT_NTSC == m_videoFormat)
	{
		assert ((0 < rate) && (rate <= 30));
	}
	else
	{
		assert ((0 < rate) && (rate <= 25));
	}

	if (bLimitForMaxRatCH)
	{
		//不能超出单通道限制
		if (UsedRate(rate, videoSize) <= m_maxRateCH)
		{
			//记录下来原来的数据，用于设置不成功时恢复。
			unsigned long oldRate = m_pRateInfo[chnn].rate;
			VIDEO_SIZE oldSize =  static_cast<VIDEO_SIZE>(m_pRateInfo[chnn].videoSize);

			m_pRateInfo[chnn].rate		= rate;
			m_pRateInfo[chnn].videoSize = videoSize;

			//不能超出总资源的限制。
			if (m_maxRateTotal < Rate())
			{
				//恢复到之前的数据。
				m_pRateInfo[chnn].rate		= oldRate;
				m_pRateInfo[chnn].videoSize = oldSize;
			}
		}
	}
	else
	{
		m_pRateInfo[chnn].rate		= rate;
		m_pRateInfo[chnn].videoSize = videoSize;
	}
	
	

	return m_maxRateTotal - Rate();
}

unsigned long CFrameRate::ModifyRateEx(unsigned char chnn, unsigned char rate, VIDEO_SIZE videoSize, bool bLimitForMaxRatCH/* = true*/)
{
	assert (NULL != m_pRateInfo);
	assert (chnn < m_chnnNum);

	if(!((m_effectCH >> chnn) & 0x1))
	{
		return false;
	}

	if (VIDEO_FORMAT_NTSC == m_videoFormat)
	{
		assert ((0 < rate) && (rate <= 30));
	}
	else
	{
		assert ((0 < rate) && (rate <= 25));
	}

	if (bLimitForMaxRatCH)
	{
		//不能超出单通道限制
		if (UsedRateEx(rate, videoSize) <= m_maxRateCH)
		{
			//记录下来原来的数据，用于设置不成功时恢复。
			unsigned long oldRate = m_pRateInfo[chnn].rate;
			VIDEO_SIZE oldSize =  static_cast<VIDEO_SIZE>(m_pRateInfo[chnn].videoSize);

			m_pRateInfo[chnn].rate		= rate;
			m_pRateInfo[chnn].videoSize = videoSize;

			//不能超出总资源的限制。
			if (m_maxRateTotal < RateEx())
			{
				//恢复到之前的数据。
				m_pRateInfo[chnn].rate		= oldRate;
				m_pRateInfo[chnn].videoSize = oldSize;
			}
		}
	}
	else
	{
		m_pRateInfo[chnn].rate		= rate;
		m_pRateInfo[chnn].videoSize = videoSize;
	}



	return m_maxRateTotal - RateEx();
}

unsigned long CFrameRate::Remaining()
{
	return m_maxRateTotal - Rate();
}

unsigned long CFrameRate::RemainingEx()
{
	return m_maxRateTotal - RateEx();
}

//////////////////////////////////////////////////////////////////////////
//统计所有通道已经使用的资源数目，以CIF的帧率为标准返回。
unsigned long CFrameRate::Rate()
{
	unsigned long rate = 0;
	for (unsigned char i = 0; i < m_chnnNum; ++i)
	{
		if(!((m_effectCH >> i) & 0x1))
		{
			continue;
		}

		if (DVRVIDEO_SIZE_D1 == m_pRateInfo[i].videoSize || DVRVIDEO_SIZE_VGA == m_pRateInfo[i].videoSize )
		{
			rate += UsedRate(m_pRateInfo[i].rate, DVRVIDEO_SIZE_D1);//(m_pRateInfo[i].rate << 2);
		}
		else if (DVRVIDEO_SIZE_HD1 == m_pRateInfo[i].videoSize)
		{
			rate += UsedRate(m_pRateInfo[i].rate, DVRVIDEO_SIZE_HD1);//(m_pRateInfo[i].rate << 1);
		}
		else if ((DVRVIDEO_SIZE_HD1080 == m_pRateInfo[i].videoSize) || (DVRVIDEO_SIZE_HD720 == m_pRateInfo[i].videoSize)  || (DVRVIDEO_SIZE_960H == m_pRateInfo[i].videoSize))
		{
			rate +=  UsedRate(m_pRateInfo[i].rate, (VIDEO_SIZE)(m_pRateInfo[i].videoSize));//(m_pRateInfo[i].rate << 3);
		}
		else if ((DVRVIDEO_SIZE_3M == m_pRateInfo[i].videoSize) || (DVRVIDEO_SIZE_SXGA == m_pRateInfo[i].videoSize)  || (DVRVIDEO_SIZE_960P == m_pRateInfo[i].videoSize))
		{
			rate +=  UsedRate(m_pRateInfo[i].rate, (VIDEO_SIZE)(m_pRateInfo[i].videoSize));//(m_pRateInfo[i].rate << 3);
		}
		else if ((DVRVIDEO_SIZE_16_9_3M == m_pRateInfo[i].videoSize) || (DVRVIDEO_SIZE_4M == m_pRateInfo[i].videoSize)  || (DVRVIDEO_SIZE_5M == m_pRateInfo[i].videoSize))
		{
			rate +=  UsedRate(m_pRateInfo[i].rate, (VIDEO_SIZE)(m_pRateInfo[i].videoSize));//(m_pRateInfo[i].rate << 3);
		}
		else if ((DVRVIDEO_SIZE_CIF == m_pRateInfo[i].videoSize)
  			    ||(DVRVIDEO_SIZE_QCIF == m_pRateInfo[i].videoSize))
		{
			rate += (m_pRateInfo[i].rate);
		}
		else
		{
			assert (false);
		}
	}

	return rate;
}

unsigned long CFrameRate::RateEx()
{
	unsigned long rate = 0;
	for (unsigned char i = 0; i < m_chnnNum; ++i)
	{
		if(!((m_effectCH >> i) & 0x1))
		{
			continue;
		}

		rate += UsedRateEx(m_pRateInfo[i].rate, (VIDEO_SIZE)(m_pRateInfo[i].videoSize));
	}

	return rate;
}

//end
