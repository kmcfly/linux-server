#include "NetStreamAllotDevice.h"
#include "LocalDevice.h"
#include "MessageMan.h"


CNetStreamAllotDevice * CNetStreamAllotDevice::Instance()
{
	static CNetStreamAllotDevice netStreamAllotDevice;
	return &netStreamAllotDevice;
}

CNetStreamAllotDevice::CNetStreamAllotDevice()
{
	m_chnnNum = 0;				//�豸ͨ����
	m_autoAllotChnnCount = 0;	//���Զ������֡��������CIFΪ��λ
}

CNetStreamAllotDevice::~CNetStreamAllotDevice()
{

}

bool CNetStreamAllotDevice::Init(PRODUCT_TYPE productID, unsigned long chnnNum, unsigned long autoAllotChnnCount, unsigned long autoAllotD1ChnnCount, unsigned long limitD1ChnnCount)
{
	m_productID = productID;
	m_chnnNum = chnnNum;
	m_autoAllotChnnCount = autoAllotChnnCount;

	m_autoAllotRateCount = autoAllotD1ChnnCount * VIDEO_SIZE_D1 ;
	m_limitRateCount = limitD1ChnnCount * VIDEO_SIZE_D1;
	if (VIDEO_FORMAT_PAL == g_video_format)
	{
		m_autoAllotRateCount = m_autoAllotRateCount * 25;
		m_limitRateCount = m_limitRateCount * 25;
	}
	else
	{
		m_autoAllotRateCount = m_autoAllotRateCount * 30;
		m_limitRateCount = m_limitRateCount * 30;
	}

	m_maxNetStreamChnnCount = chnnNum;
	memset(m_secondStreamEncodeInfo, 0, sizeof(ENCODE_INFO) * MAX_CHNN_NUM);
	memset(m_chnnVideoInfo, 0, sizeof(m_chnnVideoInfo));
	return true;
}

void CNetStreamAllotDevice::Quit()
{

}


/*
*IN��
*	netChnn ��ǰ�����������ͨ��
*	mobileChnn ��ǰ�����������ͨ��
*	netStreamChnnCount ��ǰ������������
*OUT:
*	true ���Զ�����ģ���������������
*	false ���ⲿ�����������������
*/
bool CNetStreamAllotDevice::AllotNetStream(ULONGLONG netChnn, ULONGLONG mobileChnn, ULONGLONG mobileHigh, unsigned long netStreamChnnCount, bool bUseLimitRateCount)
{
	//��������Ʒ��֧���Զ�����ͷ���
	if (!CheckProduct())
	{
		return false;
	}

	//�����������
	unsigned long chnnCount = 0;
	for (int i=0; i<m_chnnNum; i++)
	{
		if ((netChnn >> i) & 0x01)
		{
			chnnCount++;
		}
	}

	//����ֻ�����
	unsigned long mobileCount = 0;
	for (int i=0; i<m_chnnNum; i++)
	{
		if ((mobileChnn >> i) & 0x01)
		{
			mobileCount++;
		}
	}

	//�������ֻ�����
	unsigned long mobileHighCount = 0;
	for (int i=0; i<m_chnnNum; i++)
	{
		if ((mobileHigh >> i) & 0x01)
		{
			mobileHighCount++;
		}
	}

	VIDEO_SIZE videoSize = DVRVIDEO_SIZE_CIF;
	unsigned long rate = 1;
	unsigned long mobileRate = 3;
	unsigned long bitStream = 512;

	if (0 < netStreamChnnCount - mobileCount)
	{
		//��ȡ�������ֱ���
		int size = m_autoAllotRateCount / (netStreamChnnCount - mobileCount);
		if (VIDEO_FORMAT_NTSC == g_video_format)
		{
			size /= 30;
		}
		else
		{
			size /= 25;
		}
		
		//printf("%s:%s:%d, netStreamChnnCount=%d, m_autoAllotChnnCount=%d, m_autoAllotRateCount=%d, size=%d\n", __FUNCTION__, __FILE__, __LINE__, netStreamChnnCount, m_autoAllotChnnCount, m_autoAllotRateCount, size);

// 		if (size / VIDEO_SIZE_D1)
// 		{
// 			videoSize = DVRVIDEO_SIZE_D1;
// 		}
// 
// 		if (size / VIDEO_SIZE_960H)
// 		{
// 			videoSize = DVRVIDEO_SIZE_960H;
// 		}

		if (size / VIDEO_SIZE_720P)
		{
			videoSize = DVRVIDEO_SIZE_HD720;
		}

		if (size / VIDEO_SIZE_1080P)
		{
			videoSize = DVRVIDEO_SIZE_HD1080;
		}

		//��ȡ������֡��
		unsigned long autoAllotRateCount = 0;
		if (bUseLimitRateCount)
		{
			autoAllotRateCount = m_limitRateCount;
		}
		else
		{
			autoAllotRateCount = m_autoAllotRateCount;
		}

		rate = (autoAllotRateCount) / (netStreamChnnCount - mobileCount);
		if (VIDEO_FORMAT_NTSC == g_video_format)
		{
			if (rate > 30)
			{
				rate = 30;
			}
		}
		else
		{
			if (rate > 25)
			{
				rate = 25;
			}
		}

		if (0 == rate)
		{
			rate = 1;
		}

		//��һ���ֻ�������ʱ��ֻ��3֡ͼƬ������һ��ͨ��ʱֻ��1֡ͼƬ
		if (1 < mobileCount)
		{
			mobileRate = 1;
		}
		else if (1 == mobileCount)
		{
			mobileRate = 3;
		}

		//��ȡ������������С
		switch (videoSize)
		{
		case DVRVIDEO_SIZE_CIF:
			bitStream = 768;
			break;
		case DVRVIDEO_SIZE_D1:
			bitStream = 1024;
			break;
		case DVRVIDEO_SIZE_HD1080:
			bitStream = 4 * 1024;
			break;
		case DVRVIDEO_SIZE_HD720:
			bitStream = 2048;
			break;
		case DVRVIDEO_SIZE_960H:
			bitStream = 1536;
			break;
		default:
			break;
		}
	}

	//����������Ϣ��SDK,���û�иı�����Ͳ�����SDK����ֹƵ����������ϵͳ����
	for (int chnn=0; chnn<m_chnnNum; chnn++)
	{
		if ((mobileChnn >> chnn) & 0x01)
		{
			//�ֻ�����Ĭ�Ϸֱ���ΪCIF
			if (DVRVIDEO_SIZE_CIF != m_chnnVideoInfo[chnn].videoSize)
			{
				m_chnnVideoInfo[chnn].videoSize = DVRVIDEO_SIZE_CIF;
				CLocalDevice::Instance()->SetVideoSize(chnn, DVRVIDEO_SIZE_CIF, false);
				CMessageMan::Instance()->RefreshOSDName(chnn);
			}

			if ((m_chnnVideoInfo[chnn].rate != mobileRate) || (m_chnnVideoInfo[chnn].encodeType != m_secondStreamEncodeInfo[chnn].encodeType) || 
				(m_chnnVideoInfo[chnn].lBitStream != m_secondStreamEncodeInfo[chnn].lBitStream) || (m_chnnVideoInfo[chnn].quality != m_secondStreamEncodeInfo[chnn].quality))
			{
				m_chnnVideoInfo[chnn].rate = mobileRate;
				m_chnnVideoInfo[chnn].encodeType = m_secondStreamEncodeInfo[chnn].encodeType;
				m_chnnVideoInfo[chnn].lBitStream = m_secondStreamEncodeInfo[chnn].lBitStream;
				m_chnnVideoInfo[chnn].quality	 = m_secondStreamEncodeInfo[chnn].quality;

				CLocalDevice::Instance()->SetFrameRateQuality(chnn, mobileRate, static_cast<VIDEO_ENCODE_MODE>(m_secondStreamEncodeInfo[chnn].encodeType), m_secondStreamEncodeInfo[chnn].lBitStream, m_secondStreamEncodeInfo[chnn].quality, false);
			}
		}
		else if ((mobileHigh >> chnn) & 0x01)
		{
			//�����ֻ���
			int rateTemp = rate;

			switch (videoSize)
			{
			case DVRVIDEO_SIZE_HD1080:
				{
					if (VIDEO_FORMAT_NTSC == g_video_format)
					{
						if (rateTemp > 3)
						{
							rateTemp = 3;
						}
					}
					else
					{
						if (rateTemp > 2)
						{
							rateTemp = 2;
						}
					}
				}
				break;
			case DVRVIDEO_SIZE_HD720:
			case DVRVIDEO_SIZE_960H:
				{
					if (VIDEO_FORMAT_NTSC == g_video_format)
					{
						if (rateTemp > 7)
						{
							rateTemp = 7;
						}
					}
					else
					{
						if (rateTemp > 6)
						{
							rateTemp = 6;
						}
					}
				}
				break;
			case DVRVIDEO_SIZE_D1:
				{
					if (VIDEO_FORMAT_NTSC == g_video_format)
					{
						if (rateTemp)
						{
							rateTemp = 15;
						}
					}
					else
					{
						if (rateTemp > 12)
						{
							rateTemp = 12;
						}
					}
				}
				break;
			default:
				break;
			}

			if (videoSize != m_chnnVideoInfo[chnn].videoSize)
			{
				m_chnnVideoInfo[chnn].videoSize = videoSize;
				CLocalDevice::Instance()->SetVideoSize(chnn, videoSize, false);
				CMessageMan::Instance()->RefreshOSDName(chnn);
			}


			if ((m_chnnVideoInfo[chnn].rate != rateTemp) || (m_chnnVideoInfo[chnn].encodeType != m_secondStreamEncodeInfo[chnn].encodeType) || 
				(m_chnnVideoInfo[chnn].lBitStream != bitStream) || (m_chnnVideoInfo[chnn].quality != m_secondStreamEncodeInfo[chnn].quality))
			{
				m_chnnVideoInfo[chnn].rate = rateTemp;
				m_chnnVideoInfo[chnn].encodeType = m_secondStreamEncodeInfo[chnn].encodeType;
				m_chnnVideoInfo[chnn].lBitStream = bitStream;
				m_chnnVideoInfo[chnn].quality	 = m_secondStreamEncodeInfo[chnn].quality;

				CLocalDevice::Instance()->SetFrameRateQuality(chnn, rateTemp, static_cast<VIDEO_ENCODE_MODE>(m_secondStreamEncodeInfo[chnn].encodeType), bitStream, m_secondStreamEncodeInfo[chnn].quality, false);
			}
		}
		else if ((netChnn >> chnn) & 0x01)
		{
			if (videoSize != m_chnnVideoInfo[chnn].videoSize)
			{
				m_chnnVideoInfo[chnn].videoSize = videoSize;
				CLocalDevice::Instance()->SetVideoSize(chnn, videoSize, false);
				CMessageMan::Instance()->RefreshOSDName(chnn);
			}

			if ((m_chnnVideoInfo[chnn].rate != rate) || (m_chnnVideoInfo[chnn].encodeType != m_secondStreamEncodeInfo[chnn].encodeType) || 
				(m_chnnVideoInfo[chnn].lBitStream != bitStream) || (m_chnnVideoInfo[chnn].quality != m_secondStreamEncodeInfo[chnn].quality))
			{
				m_chnnVideoInfo[chnn].rate = rate;
				m_chnnVideoInfo[chnn].encodeType = m_secondStreamEncodeInfo[chnn].encodeType;
				m_chnnVideoInfo[chnn].lBitStream = bitStream;
				m_chnnVideoInfo[chnn].quality	 = m_secondStreamEncodeInfo[chnn].quality;

				CLocalDevice::Instance()->SetFrameRateQuality(chnn, rate, static_cast<VIDEO_ENCODE_MODE>(m_secondStreamEncodeInfo[chnn].encodeType), bitStream, m_secondStreamEncodeInfo[chnn].quality, false);
			}
		}
		else
		{
			//Ĭ�Ϸֱ���ΪCIF

			if (DVRVIDEO_SIZE_CIF != m_chnnVideoInfo[chnn].videoSize)
			{
				m_chnnVideoInfo[chnn].videoSize = DVRVIDEO_SIZE_CIF;
				CLocalDevice::Instance()->SetVideoSize(chnn, DVRVIDEO_SIZE_CIF, false);
				CMessageMan::Instance()->RefreshOSDName(chnn);
			}
			
			if ((m_chnnVideoInfo[chnn].rate != 1) || (m_chnnVideoInfo[chnn].encodeType != m_secondStreamEncodeInfo[chnn].encodeType) || 
				(m_chnnVideoInfo[chnn].lBitStream != m_secondStreamEncodeInfo[chnn].lBitStream) || (m_chnnVideoInfo[chnn].quality != m_secondStreamEncodeInfo[chnn].quality))
			{
				m_chnnVideoInfo[chnn].rate = 1;
				m_chnnVideoInfo[chnn].encodeType = m_secondStreamEncodeInfo[chnn].encodeType;
				m_chnnVideoInfo[chnn].lBitStream = m_secondStreamEncodeInfo[chnn].lBitStream;
				m_chnnVideoInfo[chnn].quality	 = m_secondStreamEncodeInfo[chnn].quality;

				CLocalDevice::Instance()->SetFrameRateQuality(chnn, 1, static_cast<VIDEO_ENCODE_MODE>(m_secondStreamEncodeInfo[chnn].encodeType), bitStream, m_secondStreamEncodeInfo[chnn].quality, false);
			}
		}
	}

	return true;
}


bool CNetStreamAllotDevice::CheckProduct()
{
// #if !defined(__CUSTOM_IN__)
// 	return false;
// #endif

	if(TD_2716XD_M == m_productID)
	{
		//printf("%s:%s:%d, check true\n", __FUNCTION__, __FILE__, __LINE__);
		return true;
	}


	//printf("%s:%s:%d, check false, m_productID=%d, TD_2716XD_M=%d\n", __FUNCTION__, __FILE__, __LINE__, m_productID, TD_2716XD_M);

	return false;
}

void CNetStreamAllotDevice::SetChnnEncodeInfo(unsigned long chnn, const ENCODE_INFO &encodeInfo)
{
	m_secondStreamEncodeInfo[chnn] = encodeInfo;
}

void CNetStreamAllotDevice::GetUsedNetVideoSize(unsigned long &size)
{
	if (CheckProduct())
	{
		size = DVRVIDEO_SIZE_CIF;
	}
}


