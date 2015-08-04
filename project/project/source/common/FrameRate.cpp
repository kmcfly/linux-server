/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : Ԭʯά
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

	//����ÿ��ͨ����һ֡
	if (VIDEO_FORMAT_NTSC == videoFormat)
	{
		assert ((0 < maxRateCH) && (maxRateCH <= 600));
	}
	else
	{
		assert ((0 < maxRateCH) && (maxRateCH <= 500));
	}

	//����Դ���ܳ�������ͨ�������ֵ��
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
//��ĳ��ͨ��ʹ�õ���Դ������뵽��Դ������У����ڳ�ʼ����ʹ�á�
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

//����ĳ��ͨ��������˲����Ƿ��ܳɹ������ʧ�ܣ���᷵�ؽ���ֵ��
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

	//����Ѿ�ʹ�õ���Դ = ����Դ - ��ͨ��ʹ�õ���Դ��
	unsigned long totalRate = Rate() - UsedRate(m_pRateInfo[chnn].rate, static_cast<VIDEO_SIZE>(m_pRateInfo[chnn].videoSize));

	//���ܳ����Ե���ͨ�������Դ��Ŀ�����ơ�
	unsigned long lastRate = 0;
	if (m_maxRateTotal <= totalRate)
	{
		rate = m_pRateInfo[chnn].rate;
		videoSize = (VIDEO_SIZE)m_pRateInfo[chnn].videoSize;
		return false;
	}
	else
	{
		//������������������ϵͳ������Ϊ480֡����ǰ���õ�480֡��������1��ͨ��ΪCIF��1֡.
		//�������ͨ����CIF���޳�D1���ͻ����ϵͳ������������480������Ҫ�����ͨ����֡�� 
		//�����ͨ����֡��Ϊ1֡�����Բ�����ͨ������֡�ʵķ�����ʹϵͳ�ﵽ480��
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

	//����Ѿ�ʹ�õ���Դ = ����Դ - ��ͨ��ʹ�õ���Դ��
	unsigned long totalRate = RateEx() - UsedRateEx(m_pRateInfo[chnn].rate, static_cast<VIDEO_SIZE>(m_pRateInfo[chnn].videoSize));

	//���ܳ����Ե���ͨ�������Դ��Ŀ�����ơ�
	unsigned long lastRate = 0;
	if (m_maxRateTotal <= totalRate)
	{
		rate = m_pRateInfo[chnn].rate;
		videoSize = (VIDEO_SIZE)m_pRateInfo[chnn].videoSize;
		return false;
	}
	else
	{
		//������������������ϵͳ������Ϊ480֡����ǰ���õ�480֡��������1��ͨ��ΪCIF��1֡.
		//�������ͨ����CIF���޳�D1���ͻ����ϵͳ������������480������Ҫ�����ͨ����֡�� 
		//�����ͨ����֡��Ϊ1֡�����Բ�����ͨ������֡�ʵķ�����ʹϵͳ�ﵽ480��
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

//ʵ�ʵ���ĳ��ͨ��ʹ�õ����ݣ������鲻���أ���ָ���֮ǰ�����ݡ�������ʣ����Դ��Ŀ��CIF����
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
		//���ܳ�����ͨ������
		if (UsedRate(rate, videoSize) <= m_maxRateCH)
		{
			//��¼����ԭ�������ݣ��������ò��ɹ�ʱ�ָ���
			unsigned long oldRate = m_pRateInfo[chnn].rate;
			VIDEO_SIZE oldSize =  static_cast<VIDEO_SIZE>(m_pRateInfo[chnn].videoSize);

			m_pRateInfo[chnn].rate		= rate;
			m_pRateInfo[chnn].videoSize = videoSize;

			//���ܳ�������Դ�����ơ�
			if (m_maxRateTotal < Rate())
			{
				//�ָ���֮ǰ�����ݡ�
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
		//���ܳ�����ͨ������
		if (UsedRateEx(rate, videoSize) <= m_maxRateCH)
		{
			//��¼����ԭ�������ݣ��������ò��ɹ�ʱ�ָ���
			unsigned long oldRate = m_pRateInfo[chnn].rate;
			VIDEO_SIZE oldSize =  static_cast<VIDEO_SIZE>(m_pRateInfo[chnn].videoSize);

			m_pRateInfo[chnn].rate		= rate;
			m_pRateInfo[chnn].videoSize = videoSize;

			//���ܳ�������Դ�����ơ�
			if (m_maxRateTotal < RateEx())
			{
				//�ָ���֮ǰ�����ݡ�
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
//ͳ������ͨ���Ѿ�ʹ�õ���Դ��Ŀ����CIF��֡��Ϊ��׼���ء�
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
