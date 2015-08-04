#include "IPCFrameEncode.h"

extern VIDEO_SIZE_BITRATE_INFO g_vsBitRateInfo[];


CIPCFrameEncode::CIPCFrameEncode(void)
{
	m_pValidBitRate = NULL;
	m_pFrameEncode = NULL;
	m_bLimitByBitRate = true;
}

CIPCFrameEncode::~CIPCFrameEncode(void)
{
}

bool CIPCFrameEncode::Initial(unsigned char chnnNum, unsigned long maxBitRateTotal, unsigned long maxFrameRateOnVGA, \
							  unsigned long *pValidBitRate, unsigned long bitRateCount, \
							  unsigned long defaVideoSize, unsigned long defaFrameRate, unsigned long defaBitRate)
{
	m_chnnNum = chnnNum;
	m_maxBitRateTotal = maxBitRateTotal;
	m_maxFrameRateOnVGA = maxFrameRateOnVGA;
	m_pValidBitRate = new unsigned long [bitRateCount];
	assert(NULL != m_pValidBitRate);
	memcpy(m_pValidBitRate, pValidBitRate, sizeof(unsigned long) * bitRateCount);
	m_bitRateCount = bitRateCount;
//	m_bLimitByBitRate = bLimitByBitRate;

	switch(defaVideoSize)
	{
	case DVRVIDEO_SIZE_D1:
	case DVRVIDEO_SIZE_VGA:
		{
			m_defaVideoSizeOfVGA = 1;
		}
		break;
	case DVRVIDEO_SIZE_HD720:
		{
			m_defaVideoSizeOfVGA = 3;
		}
		break;
	case DVRVIDEO_SIZE_HD1080:
		{
			m_defaVideoSizeOfVGA = 6;
		}
		break;
	case DVRVIDEO_SIZE_960P:
		{
			m_defaVideoSizeOfVGA = 4;
		}
		break;
	case DVRVIDEO_SIZE_SXGA:
		{
			m_defaVideoSizeOfVGA = 5;
		}
		break;
	case DVRVIDEO_SIZE_3M:
		{
			m_defaVideoSizeOfVGA = 8;
		}
		break;
	case DVRVIDEO_SIZE_16_9_3M:
		{
			m_defaVideoSizeOfVGA = 8;
		}
		break;
	case DVRVIDEO_SIZE_4M:
		{
			m_defaVideoSizeOfVGA = 12;
		}
		break;
	case DVRVIDEO_SIZE_5M:
		{
			m_defaVideoSizeOfVGA = 16;
		}
		break;
	default:
		{
			assert(false);
			m_defaVideoSizeOfVGA = 3;
		}
	}
	m_defaFrameRate = defaFrameRate;
	m_defaBitRate = defaBitRate;

	m_pFrameEncode = new FRAME_ENCODE[m_chnnNum];
	memset(m_pFrameEncode, 0, sizeof(FRAME_ENCODE) * m_chnnNum);

	m_bUseStaticVideoBitRate = CProduct::Instance()->IsUseStaticVideoBitRate();

	return true;
}

void CIPCFrameEncode::Quit()
{
	if(NULL != m_pValidBitRate)
	{
		delete [] m_pValidBitRate;
		m_pValidBitRate = NULL;
	}

	if(NULL != m_pFrameEncode)
	{
		delete [] m_pFrameEncode;
		m_pFrameEncode = NULL;
	}
}

//////////////////////////////////////////////////////////////////////////
void CIPCFrameEncode::AddEncode(unsigned char chnn, unsigned char frameRate, VIDEO_SIZE videoSize, unsigned long bitRate)
{
	assert (NULL != m_pFrameEncode);

	if(chnn >= m_chnnNum)
	{
		return;
	}

	m_pFrameEncode[chnn].frameRate	= frameRate;
	m_pFrameEncode[chnn].videoSize	= videoSize;
	m_pFrameEncode[chnn].bitRate	= bitRate;
}

bool CIPCFrameEncode::TryEncode(unsigned char chnn, unsigned char & frameRate, VIDEO_SIZE & videoSize, unsigned long &bitRate)
{
	assert(chnn < m_chnnNum);

	unsigned long usedBitRate = 0;
	unsigned long usedFrameRate = 0;
	for(unsigned char i = 0; i < m_chnnNum; i++)
	{
		if(chnn != i)
		{
			usedBitRate += m_pFrameEncode[i].bitRate;
			if(0 != m_pFrameEncode[i].frameRate)
			{
				usedFrameRate += ToCorrespondingVGARate(m_pFrameEncode[i].videoSize, m_pFrameEncode[i].frameRate);
			}
		}
	}

	//assert(usedBitRate < m_maxBitRateTotal);
	//assert(usedFrameRate < m_maxFrameRateOnVGA);
	if((usedBitRate >= m_maxBitRateTotal) || (usedFrameRate >= m_maxFrameRateOnVGA))
	{
		return false;
	}

	unsigned long leftBitRate = m_maxBitRateTotal - usedBitRate;
	leftBitRate = ToValidBitRate(leftBitRate, LOW_CLOSEST);

	bool bRet = true;

	if((videoSize != m_pFrameEncode[chnn].videoSize) \
		||(frameRate != m_pFrameEncode[chnn].frameRate))
	{
		//先调整帧率，后调整码率
		unsigned long leftFrameRate = m_maxFrameRateOnVGA - usedFrameRate;
		unsigned long realFrameRate;

		switch(videoSize)
		{
		case DVRVIDEO_SIZE_D1:
		case DVRVIDEO_SIZE_VGA:
			{
				realFrameRate = leftFrameRate;
				if(frameRate > realFrameRate)
				{
					frameRate = realFrameRate;
					bRet = false;
				}

				bitRate = m_defaBitRate * frameRate / m_defaFrameRate / m_defaVideoSizeOfVGA;
				bitRate = ToValidBitRate(bitRate, HIGH_CLOSEST);
				bitRate = (bitRate > MAX_BITRATE_ON_VGA) ? MAX_BITRATE_ON_VGA : bitRate;

				if(bitRate > leftBitRate)
				{
					bitRate = leftBitRate;
					bRet = false;
				}
			}
			break;
		case DVRVIDEO_SIZE_HD720:
			{
				realFrameRate = leftFrameRate/3;
				if(frameRate > realFrameRate)
				{
					frameRate = realFrameRate;
					frameRate = (0 == frameRate) ? 1 : frameRate;
					bRet = false;
				}

				bitRate = m_defaBitRate * frameRate * 3 / m_defaFrameRate / m_defaVideoSizeOfVGA;
				bitRate = ToValidBitRate(bitRate, HIGH_CLOSEST);
				bitRate = (bitRate > MAX_BITRATE_ON_HD720) ? MAX_BITRATE_ON_HD720 : bitRate;

				if(bitRate > leftBitRate)
				{
					bitRate = leftBitRate;
					bRet = false;
				}
			}
			break;
		case DVRVIDEO_SIZE_HD1080:
			{
				if(!m_bLimitByBitRate)
				{
					realFrameRate = leftFrameRate/3 * 2/5;
				}
				else
				{
					realFrameRate = leftFrameRate/6;
				}
				if(frameRate > realFrameRate)
				{
					frameRate = realFrameRate;
					frameRate = (0 == frameRate) ? 1 : frameRate;
					bRet = false;
				}

				bitRate = m_defaBitRate * frameRate * 6 / m_defaFrameRate / m_defaVideoSizeOfVGA;
				bitRate = ToValidBitRate(bitRate, HIGH_CLOSEST);
				bitRate = (bitRate > MAX_SUGGEST_BITRATE_ON_HD1080) ? MAX_SUGGEST_BITRATE_ON_HD1080: bitRate;

				if(bitRate > leftBitRate)
				{
					bitRate = leftBitRate;
					bRet = false;
				}
			}
			break;

		case DVRVIDEO_SIZE_960P:
			{
				if(!m_bLimitByBitRate)
				{
					realFrameRate = leftFrameRate*10/45;
				}
				else
				{
					realFrameRate = leftFrameRate/4;
				}
				if(frameRate > realFrameRate)
				{
					frameRate = realFrameRate;
					frameRate = (0 == frameRate) ? 1 : frameRate;
					bRet = false;
				}


				bitRate = m_defaBitRate * frameRate * 4 / m_defaFrameRate / m_defaVideoSizeOfVGA;
				bitRate = ToValidBitRate(bitRate, HIGH_CLOSEST);
				bitRate = (bitRate > MAX_SUGGEST_BITRATE_ON_960P) ? MAX_SUGGEST_BITRATE_ON_960P: bitRate;

				if(bitRate > leftBitRate)
				{
					bitRate = leftBitRate;
					bRet = false;
				}
			}
			break;

		case DVRVIDEO_SIZE_SXGA:
			{
				if(!m_bLimitByBitRate)
				{
					realFrameRate = leftFrameRate*10/55;
				}
				else
				{
					realFrameRate = leftFrameRate/5;
				}
				if(frameRate > realFrameRate)
				{
					frameRate = realFrameRate;
					frameRate = (0 == frameRate) ? 1 : frameRate;
					bRet = false;
				}


				bitRate = m_defaBitRate * frameRate * 5 / m_defaFrameRate / m_defaVideoSizeOfVGA;
				bitRate = ToValidBitRate(bitRate, HIGH_CLOSEST);
				bitRate = (bitRate > MAX_SUGGEST_BITRATE_ON_1D3M) ? MAX_SUGGEST_BITRATE_ON_1D3M: bitRate;

				if(bitRate > leftBitRate)
				{
					bitRate = leftBitRate;
					bRet = false;
				}
			}
			break;

		case DVRVIDEO_SIZE_3M:
		case DVRVIDEO_SIZE_16_9_3M:
			{
				if(!m_bLimitByBitRate)
				{
					realFrameRate = leftFrameRate*10/95;
				}
				else
				{
					realFrameRate = leftFrameRate/8;
				}
				if(frameRate > realFrameRate)
				{
					frameRate = realFrameRate;
					frameRate = (0 == frameRate) ? 1 : frameRate;
					bRet = false;
				}


				bitRate = m_defaBitRate * frameRate * 8 / m_defaFrameRate / m_defaVideoSizeOfVGA;
				bitRate = ToValidBitRate(bitRate, HIGH_CLOSEST);
				bitRate = (bitRate > MAX_SUGGEST_BITRATE_ON_3M) ? MAX_SUGGEST_BITRATE_ON_3M: bitRate;

				if(bitRate > leftBitRate)
				{
					bitRate = leftBitRate;
					bRet = false;
				}
			}
			break;

		case DVRVIDEO_SIZE_4M:
			{
				if(!m_bLimitByBitRate)
				{
					realFrameRate = leftFrameRate*10/95;
				}
				else
				{
					realFrameRate = leftFrameRate/12;
				}
				if(frameRate > realFrameRate)
				{
					frameRate = realFrameRate;
					frameRate = (0 == frameRate) ? 1 : frameRate;
					bRet = false;
				}


				bitRate = m_defaBitRate * frameRate * 12 / m_defaFrameRate / m_defaVideoSizeOfVGA;
				bitRate = ToValidBitRate(bitRate, HIGH_CLOSEST);
				bitRate = (bitRate > MAX_SUGGEST_BITRATE_ON_3M) ? MAX_SUGGEST_BITRATE_ON_3M: bitRate;

				if(bitRate > leftBitRate)
				{
					bitRate = leftBitRate;
					bRet = false;
				}
			}
			break;

		case DVRVIDEO_SIZE_5M:
			{
				if(!m_bLimitByBitRate)
				{
					realFrameRate = leftFrameRate*10/95;
				}
				else
				{
					realFrameRate = leftFrameRate/16;
				}
				if(frameRate > realFrameRate)
				{
					frameRate = realFrameRate;
					frameRate = (0 == frameRate) ? 1 : frameRate;
					bRet = false;
				}


				bitRate = m_defaBitRate * frameRate * 16 / m_defaFrameRate / m_defaVideoSizeOfVGA;
				bitRate = ToValidBitRate(bitRate, HIGH_CLOSEST);
				bitRate = (bitRate > MAX_SUGGEST_BITRATE_ON_3M) ? MAX_SUGGEST_BITRATE_ON_3M: bitRate;

				if(bitRate > leftBitRate)
				{
					bitRate = leftBitRate;
					bRet = false;
				}
			}
			break;

		default:
			{
				bRet = false;
			}
			assert(false);
		}

		if (m_bUseStaticVideoBitRate)
		{

			unsigned long defaBitRate = 0;
			for (int i=0; 0 != g_vsBitRateInfo[i].videoSize; i++)
			{
				if (g_vsBitRateInfo[i].videoSize == videoSize)
				{
					defaBitRate = g_vsBitRateInfo[i].bitRate;
					break;
				}
			}

			if (0 != defaBitRate)
			{
				unsigned long bitRateTemp = defaBitRate * frameRate / m_defaFrameRate;
				bitRateTemp = ToValidBitRate(bitRateTemp, HIGH_CLOSEST);
				
				if ((DVRVIDEO_SIZE_D1 == videoSize) || (DVRVIDEO_SIZE_VGA == videoSize))
				{
					bitRateTemp = (bitRateTemp > MAX_BITRATE_ON_VGA) ? MAX_BITRATE_ON_VGA : bitRateTemp;
				}
				else if (DVRVIDEO_SIZE_HD720 == videoSize)
				{
					bitRateTemp = (bitRateTemp > MAX_BITRATE_ON_HD720) ? MAX_BITRATE_ON_HD720 : bitRateTemp;
				}
				else if (DVRVIDEO_SIZE_HD1080 == videoSize)
				{
					bitRateTemp = (bitRateTemp > MAX_SUGGEST_BITRATE_ON_HD1080) ? MAX_SUGGEST_BITRATE_ON_HD1080: bitRateTemp;
				}
				else if (DVRVIDEO_SIZE_960P == videoSize)
				{
					bitRateTemp = (bitRateTemp > MAX_SUGGEST_BITRATE_ON_960P) ? MAX_SUGGEST_BITRATE_ON_960P: bitRateTemp;
				}
				else if (DVRVIDEO_SIZE_SXGA == videoSize)
				{
					bitRateTemp = (bitRateTemp > MAX_SUGGEST_BITRATE_ON_1D3M) ? MAX_SUGGEST_BITRATE_ON_1D3M: bitRateTemp;
				}
				else if (DVRVIDEO_SIZE_3M == videoSize || DVRVIDEO_SIZE_16_9_3M == videoSize || DVRVIDEO_SIZE_4M == videoSize || DVRVIDEO_SIZE_5M == videoSize)
				{
					bitRateTemp = (bitRateTemp > MAX_SUGGEST_BITRATE_ON_3M) ? MAX_SUGGEST_BITRATE_ON_3M: bitRateTemp;
				}
				else
				{
				}

				PRODUCT_TYPE productID = CProduct::Instance()->ProductType();
				
// 				if (TD_2716TS_PL == productID)
// 				{
// 					if (bitRate > bitRateTemp)
// 					{
// 						bitRate = bitRateTemp;
// 					}
// 
// 					if(bitRate > leftBitRate)
// 					{
// 						bitRate = leftBitRate;
// 						bRet = false;
// 					}
// 				}
// 				else
				{
					if (bitRate > bitRateTemp)
					{
						bitRate = bitRateTemp;
						//bRet = false;
					}
					else if(bitRate > leftBitRate)
					{
						bitRate = leftBitRate;
						//bRet = false;
					}
					else if (bitRateTemp <= leftBitRate)
					{
						bitRate = bitRateTemp;
					}
					else
					{
						//bRet = true;
					}
				}
			}
		}
	}
	else if(bitRate != m_pFrameEncode[chnn].bitRate)
	{
		if(leftBitRate < bitRate)
		{
			bitRate = leftBitRate;
			bRet = false;
		}
	}

	return bRet;
}

bool CIPCFrameEncode::ModifyEncode(unsigned char chnn, unsigned char frameRate, VIDEO_SIZE videoSize, unsigned long bitRate)
{
	assert(chnn < m_chnnNum);

	unsigned long usedBitRate = 0;
	for(unsigned char i = 0; i < m_chnnNum; i++)
	{
		if(chnn != i)
		{
			usedBitRate += m_pFrameEncode[i].bitRate;
		}
	}

	assert(usedBitRate < m_maxBitRateTotal);
	if(usedBitRate >= m_maxBitRateTotal)
	{
		return false;
	}

	if((usedBitRate + bitRate) > m_maxBitRateTotal)
	{
		return false;
	}

	m_pFrameEncode[chnn].videoSize = videoSize;
	m_pFrameEncode[chnn].frameRate = frameRate;
	m_pFrameEncode[chnn].bitRate = bitRate;

	return true;
}

unsigned long CIPCFrameEncode::RemainingFrameRate(VIDEO_SIZE videoSize)
{
	unsigned long usedBitRate = 0;
	unsigned long usedFrameRate = 0;
	for(unsigned char chnn = 0; chnn < m_chnnNum; chnn++)
	{
		usedBitRate += m_pFrameEncode[chnn].bitRate;
		if(0 != m_pFrameEncode[chnn].frameRate)
		{
			usedFrameRate += ToCorrespondingVGARate(m_pFrameEncode[chnn].videoSize, m_pFrameEncode[chnn].frameRate);
		}
	}

	if(usedBitRate > m_maxBitRateTotal)
	{
		assert(false);
		return 0;
	}

	if(usedFrameRate > m_maxFrameRateOnVGA)
	{
		assert(false);
		return 0;
	}

	unsigned long remainBitRate = m_maxBitRateTotal - usedBitRate;
	unsigned long remainFrameRate = m_maxFrameRateOnVGA - usedFrameRate;

	unsigned long remainByBitRate = 0;
	unsigned long remainByFrameRate = 0;
	switch(videoSize)
	{
	case DVRVIDEO_SIZE_D1:
	case DVRVIDEO_SIZE_VGA:
		{
			remainByBitRate = remainBitRate * m_maxFrameRateOnVGA / m_maxBitRateTotal;
			remainByFrameRate = remainFrameRate;

			return ((remainByFrameRate < remainByBitRate) ? remainByFrameRate : remainByBitRate);
		}
	case DVRVIDEO_SIZE_HD720:
		{
			remainByBitRate = remainBitRate * m_maxFrameRateOnVGA / (3 *m_maxBitRateTotal);
			remainByFrameRate = remainFrameRate / 3;

			return ((remainByFrameRate < remainByBitRate) ? remainByFrameRate : remainByBitRate);
		}
		break;
	case DVRVIDEO_SIZE_HD1080:
		{
			remainByBitRate = remainBitRate * m_maxFrameRateOnVGA/m_maxBitRateTotal/6;

			if(!m_bLimitByBitRate)
			{
				remainByFrameRate = remainFrameRate/3 * 2/5;
			}
			else
			{
				remainByFrameRate = remainFrameRate/6;
			}

			return ((remainByFrameRate < remainByBitRate) ? remainByFrameRate : remainByBitRate);
		}
		break;

	case DVRVIDEO_SIZE_960P:
		{
			remainByBitRate = remainBitRate * m_maxFrameRateOnVGA/m_maxBitRateTotal/6;

			if(!m_bLimitByBitRate)
			{
				remainByFrameRate = remainFrameRate*10/45;
			}
			else
			{
				remainByFrameRate = remainFrameRate/4;
			}

			return ((remainByFrameRate < remainByBitRate) ? remainByFrameRate : remainByBitRate);
		}
		break;
	case DVRVIDEO_SIZE_SXGA:
		{
			remainByBitRate = remainBitRate * m_maxFrameRateOnVGA/m_maxBitRateTotal/6;

			if(!m_bLimitByBitRate)
			{
				remainByFrameRate = remainFrameRate*10/55;
			}
			else
			{
				remainByFrameRate = remainFrameRate/5;
			}

			return ((remainByFrameRate < remainByBitRate) ? remainByFrameRate : remainByBitRate);
		}
		break;
	case DVRVIDEO_SIZE_3M:
	case DVRVIDEO_SIZE_16_9_3M:
		{
			remainByBitRate = remainBitRate * m_maxFrameRateOnVGA/m_maxBitRateTotal/6;

			if(!m_bLimitByBitRate)
			{
				remainByFrameRate = remainFrameRate*10/95;
			}
			else
			{
				remainByFrameRate = remainFrameRate/8;
			}

			return ((remainByFrameRate < remainByBitRate) ? remainByFrameRate : remainByBitRate);
		}
		break;
	case DVRVIDEO_SIZE_4M:
		{
			remainByBitRate = remainBitRate * m_maxFrameRateOnVGA/m_maxBitRateTotal/6;

			if(!m_bLimitByBitRate)
			{
				remainByFrameRate = remainFrameRate*10/95;
			}
			else
			{
				remainByFrameRate = remainFrameRate/12;
			}

			return ((remainByFrameRate < remainByBitRate) ? remainByFrameRate : remainByBitRate);
		}
		break;
	case DVRVIDEO_SIZE_5M:
		{
			remainByBitRate = remainBitRate * m_maxFrameRateOnVGA/m_maxBitRateTotal/6;

			if(!m_bLimitByBitRate)
			{
				remainByFrameRate = remainFrameRate*10/95;
			}
			else
			{
				remainByFrameRate = remainFrameRate/16;
			}

			return ((remainByFrameRate < remainByBitRate) ? remainByFrameRate : remainByBitRate);
		}
		break;
	default:
		assert(false);
	}

	return 0;
}

unsigned long CIPCFrameEncode::GetMaxFrameRate(VIDEO_SIZE videoSize)
{
	switch(videoSize)
	{
	case DVRVIDEO_SIZE_D1:
	case DVRVIDEO_SIZE_VGA:
		{
			return m_maxFrameRateOnVGA;
		}
	case DVRVIDEO_SIZE_HD720:
		{
			return m_maxFrameRateOnVGA / 3;
		}
		break;
	case DVRVIDEO_SIZE_HD1080:
		{
			if(!m_bLimitByBitRate)
			{
				return m_maxFrameRateOnVGA/3 * 2/5;
			}
			else
			{
				return m_maxFrameRateOnVGA/6;
			}
		}
		break;


	case DVRVIDEO_SIZE_960P:
		{
			if(!m_bLimitByBitRate)
			{
				return m_maxFrameRateOnVGA*10/45;
			}
			else
			{
				return m_maxFrameRateOnVGA/4;
			}
		}
		break;
	case DVRVIDEO_SIZE_SXGA:
		{
			if(!m_bLimitByBitRate)
			{
				return m_maxFrameRateOnVGA*10/55;
			}
			else
			{
				return m_maxFrameRateOnVGA/5;
			}
		}
		break;
	case DVRVIDEO_SIZE_3M:
	case DVRVIDEO_SIZE_16_9_3M:
		{
			if(!m_bLimitByBitRate)
			{
				return m_maxFrameRateOnVGA*10/95;
			}
			else
			{
				return m_maxFrameRateOnVGA/8;
			}
		}
		break;
	case DVRVIDEO_SIZE_4M:
		{
			if(!m_bLimitByBitRate)
			{
				return m_maxFrameRateOnVGA*10/95;
			}
			else
			{
				return m_maxFrameRateOnVGA/12;
			}
		}
		break;
	case DVRVIDEO_SIZE_5M:
		{
			if(!m_bLimitByBitRate)
			{
				return m_maxFrameRateOnVGA*10/95;
			}
			else
			{
				return m_maxFrameRateOnVGA/16;
			}
		}
		break;
	default:
		assert(false);
	}

	return 0;
}

unsigned long CIPCFrameEncode::GetSuggestBitRate(VIDEO_SIZE videoSize, unsigned char frameRate)
{
	unsigned long bitRate = 0;
	switch(videoSize)
	{
	case DVRVIDEO_SIZE_D1:
	case DVRVIDEO_SIZE_VGA:
		{
			bitRate = m_defaBitRate * frameRate / m_defaFrameRate / m_defaVideoSizeOfVGA;
			bitRate = ToValidBitRate(bitRate, HIGH_CLOSEST);
			bitRate = (bitRate > MAX_BITRATE_ON_VGA) ? MAX_BITRATE_ON_VGA : bitRate;
		}
		break;
	case DVRVIDEO_SIZE_HD720:
		{
			bitRate = m_defaBitRate * frameRate * 3 / m_defaFrameRate / m_defaVideoSizeOfVGA;
			bitRate = ToValidBitRate(bitRate, HIGH_CLOSEST);
			bitRate = (bitRate > MAX_BITRATE_ON_HD720) ? MAX_BITRATE_ON_HD720 : bitRate;
		}
		break;
	case DVRVIDEO_SIZE_HD1080:
		{
			bitRate = m_defaBitRate * frameRate * 6 / m_defaFrameRate / m_defaVideoSizeOfVGA;
			bitRate = ToValidBitRate(bitRate, HIGH_CLOSEST);
			bitRate = (bitRate > MAX_SUGGEST_BITRATE_ON_HD1080) ? MAX_SUGGEST_BITRATE_ON_HD1080 : bitRate;
		}
		break;
	case DVRVIDEO_SIZE_960P:
		{
			bitRate = m_defaBitRate * frameRate * 4 / m_defaFrameRate / m_defaVideoSizeOfVGA;
			bitRate = ToValidBitRate(bitRate, HIGH_CLOSEST);
			bitRate = (bitRate > MAX_SUGGEST_BITRATE_ON_HD1080) ? MAX_SUGGEST_BITRATE_ON_HD1080 : bitRate;
		}
		break;
	case DVRVIDEO_SIZE_SXGA:
		{
			bitRate = m_defaBitRate * frameRate * 5 / m_defaFrameRate / m_defaVideoSizeOfVGA;
			bitRate = ToValidBitRate(bitRate, HIGH_CLOSEST);
			bitRate = (bitRate > MAX_SUGGEST_BITRATE_ON_HD1080) ? MAX_SUGGEST_BITRATE_ON_HD1080 : bitRate;
		}
		break;
	case DVRVIDEO_SIZE_3M:
	case DVRVIDEO_SIZE_16_9_3M:
		{
			bitRate = m_defaBitRate * frameRate * 8 / m_defaFrameRate / m_defaVideoSizeOfVGA;
			bitRate = ToValidBitRate(bitRate, HIGH_CLOSEST);
			bitRate = (bitRate > MAX_SUGGEST_BITRATE_ON_HD1080) ? MAX_SUGGEST_BITRATE_ON_HD1080 : bitRate;
		}
		break;
	case DVRVIDEO_SIZE_4M:
		{
			bitRate = m_defaBitRate * frameRate * 12 / m_defaFrameRate / m_defaVideoSizeOfVGA;
			bitRate = ToValidBitRate(bitRate, HIGH_CLOSEST);
			bitRate = (bitRate > MAX_SUGGEST_BITRATE_ON_3M) ? MAX_SUGGEST_BITRATE_ON_3M : bitRate;
		}
		break;
	case DVRVIDEO_SIZE_5M:
		{
			bitRate = m_defaBitRate * frameRate * 16 / m_defaFrameRate / m_defaVideoSizeOfVGA;
			bitRate = ToValidBitRate(bitRate, HIGH_CLOSEST);
			bitRate = (bitRate > MAX_SUGGEST_BITRATE_ON_3M) ? MAX_SUGGEST_BITRATE_ON_3M : bitRate;
		}
		break;
	default:
		assert(false);
	}

	return bitRate;
}

unsigned long CIPCFrameEncode::ToValidBitRate(unsigned long bitRate, CLOSE_METHOD method)
{
	unsigned long lowCloseRate = bitRate;
	unsigned long highCloseRate = bitRate;

	while(true)
	{
		if(IsInBitRateRange(lowCloseRate))
		{
			if((LOW_CLOSEST == method) || (LOW_OR_HIGH_CLOSEST == method))
			{
				return lowCloseRate;
			}
		}
		else if(0 == lowCloseRate)
		{
	//		assert(false);
			return m_pValidBitRate[0];
		}
		else
		{
			lowCloseRate--;
		}

		if(IsInBitRateRange(highCloseRate))
		{
			if((HIGH_CLOSEST == method) || (LOW_OR_HIGH_CLOSEST == method))
			{
				return highCloseRate;
			}
		}
		else if(highCloseRate > m_pValidBitRate[m_bitRateCount-1])
		{
			return m_pValidBitRate[m_bitRateCount-1];
		}
		else
		{
			highCloseRate++;
		}
	}

	assert(false);
	return 0;
}

bool CIPCFrameEncode::IsInBitRateRange(unsigned long bitRate)
{
	if(bitRate == 0)
	{
		return false;
	}

	for (unsigned long i = 0; i< m_bitRateCount; ++i)
	{
		if (m_pValidBitRate[i] == bitRate)
		{
			return true;
		}
	}
	return false;
}

unsigned long CIPCFrameEncode::ToCorrespondingVGARate(unsigned long videoSize, unsigned char frameRate)
{
	switch(videoSize)
	{
	case DVRVIDEO_SIZE_D1:
	case DVRVIDEO_SIZE_VGA:
		{
			return frameRate;
		}
	case DVRVIDEO_SIZE_HD720:
		{
			return frameRate * 3;
		}
	case DVRVIDEO_SIZE_HD1080:
		{
			if(!m_bLimitByBitRate)
			{
				return frameRate * 3 * 5/2;
			}
			else
			{
				return frameRate * 6;
			}
		}
	case DVRVIDEO_SIZE_960P:
		{
			if(!m_bLimitByBitRate)
			{
				return frameRate * 35/10;
			}
			else
			{
				return frameRate * 4;
			}
		}
	case DVRVIDEO_SIZE_SXGA:
		{
			if(!m_bLimitByBitRate)
			{
				return frameRate * 45/10;
			}
			else
			{
				return frameRate * 5;
			}
		}
	case DVRVIDEO_SIZE_3M:
	case DVRVIDEO_SIZE_16_9_3M:
		{
			if(!m_bLimitByBitRate)
			{
				return frameRate * 85/10;
			}
			else
			{
				return frameRate * 8;
			}
		}
	case DVRVIDEO_SIZE_4M:
		{
			if(!m_bLimitByBitRate)
			{
				return frameRate * 85/10;
			}
			else
			{
				return frameRate * 12;
			}
		}
	case DVRVIDEO_SIZE_5M:
		{
			if(!m_bLimitByBitRate)
			{
				return frameRate * 85/10;
			}
			else
			{
				return frameRate * 16;
			}
		}
	default:
		assert(false);
	}

	return 0;
}

