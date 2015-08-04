#include "SnapCapabilityMan.h"

CSnapCapabilityMan *CSnapCapabilityMan::m_pSnapCapabilityMan = NULL;

CSnapCapabilityMan *CSnapCapabilityMan::Instance()
{
	if (NULL == m_pSnapCapabilityMan)
	{
		m_pSnapCapabilityMan = new CSnapCapabilityMan;
	}

	return m_pSnapCapabilityMan;
}

CSnapCapabilityMan::CSnapCapabilityMan()
{
	m_pChnnNoPhoneEableCount = NULL;
	m_pChnnPhoneEableCount = NULL;
}

CSnapCapabilityMan::~CSnapCapabilityMan()
{
	
}

bool CSnapCapabilityMan::Initial(int channelNum, int rate, int rateTime, VIDEO_SIZE videoSize)
{
	if (0 == channelNum)
	{
		m_chnnNum = 0;
		return true;
	}

	m_chnnNum = channelNum;
	m_snapRate = rate;
	m_snapRateTime = rateTime;
	m_snapVideSize = videoSize;
	m_jpegLevel = 3;
	m_phoneSnapRate = 2;
	m_phoneSnapRaiseChnn = -1;

	m_pChnnNoPhoneEableCount = new unsigned char [m_chnnNum];
	m_pChnnPhoneEableCount = new unsigned char [m_chnnNum];
	memset(m_pChnnNoPhoneEableCount, 0, m_chnnNum);
	memset(m_pChnnPhoneEableCount, 0, m_chnnNum);

	for (int chnn = 0; chnn < channelNum; ++chnn)
	{
		//printf("%s,%d,channel:%d, SetSnapEnable %d, %d, videoLevel:%d,videosize:%d,rateAndLevel:0\n",__FILE__, __LINE__,chnn, 0, 0, m_jpegLevel,videoSize);
		ext_set_snap_channel_enable(chnn, 0, 0, videoSize);
	}

	return true;
}

void CSnapCapabilityMan::Quit()
{
	if (NULL != m_pChnnPhoneEableCount)
	{
		delete [] m_pChnnPhoneEableCount;
		m_pChnnPhoneEableCount = NULL;
	}

	if (NULL != m_pChnnNoPhoneEableCount)
	{
		delete [] m_pChnnNoPhoneEableCount;
		m_pChnnNoPhoneEableCount = NULL;
	}

}

void CSnapCapabilityMan::StarSnapJpeg(int chnn, bool bPhone)
{
	if (chnn >= m_chnnNum)
	{
		return;
	}

	m_setSnapRateLock.Lock();

	int rate = 0;
	int rateTime = 0;
	int rateAndLevel = 0;
	
	bool bCurrHavePhone = false;
	for (int i = 0; i < m_chnnNum; ++i)
	{
		if (m_pChnnPhoneEableCount[i] > 0)
		{
			bCurrHavePhone = true;
			break;
		}
	}

	if (bPhone)
	{
		if (bCurrHavePhone)
		{
			if (0 == (m_pChnnPhoneEableCount[chnn] + m_pChnnNoPhoneEableCount[chnn]))
			{
				if (2 == m_snapRateTime)
				{
					rate = 1;
					rateTime = 3;
				}
				else if (1 == m_snapRateTime)
				{
					rate = 1;
					rateTime = 2;
				}
				else
				{
					rate = 1;
					rateTime = 3;
				}
				
#if defined(__SUPPORT_HONGDI__)
				rate = 3;
				rateTime  = 1;
#endif
				rateAndLevel = (m_jpegLevel << 16) + rate;

				//printf("%s,%d,channel:%d, have phone online, phone StarSnapJpeg, SetSnapEnable  %d, %d, videoLevel:%d,videosize:%d,rateAndLevel:%x\n",
				//	__FILE__, __LINE__,chnn, rate, rateTime, m_jpegLevel, m_snapVideSize, rateAndLevel);
				ext_set_snap_channel_enable(chnn, rateAndLevel, rateTime, m_snapVideSize);
			}
			else
			{
				//printf("%s,%d,channel:%d, have phone online, phone StarSnapJpeg, no SetSnapEnable,because of this channel is snaping\n",
				//	__FILE__,__LINE__,chnn);
			}
			m_pChnnPhoneEableCount[chnn]++;

		}
		else
		{
			{
				rate = m_phoneSnapRate;
				rateTime = 1;
#if defined(__SUPPORT_HONGDI__)
				rate = 3;
				rateTime  = 1;
#endif
				rateAndLevel = (m_jpegLevel << 16) + rate;
				//printf("%s,%d,channel:%d, no phone online, phone StarSnapJpeg, SetSnapEnable  %d, %d, videoLevel:%d,videosize:%d,rateAndLevel:%x\n",
				//	__FILE__, __LINE__,chnn, rate, rateTime, m_jpegLevel, m_snapVideSize, rateAndLevel);
				ext_set_snap_channel_enable(chnn, rateAndLevel, rateTime, m_snapVideSize);
				m_phoneSnapRaiseChnn = chnn;
			}

			{
				if (2 == m_snapRateTime)
				{
					rate = 1;
					rateTime = 3;
				}
				else if (1 == m_snapRateTime)
				{
					rate = 1;
					rateTime = 2;
				}
				else
				{
					rate = 1;
					rateTime = 3;
				}

#if defined(__SUPPORT_HONGDI__)
				rate = 3;
				rateTime  = 1;
#endif
				rateAndLevel = (m_jpegLevel << 16) + rate;

				for (int i = 0; i < m_chnnNum; ++i)
				{
					if (i != chnn)
					{
						if (((m_pChnnPhoneEableCount[i] + m_pChnnNoPhoneEableCount[i]) > 0))
						{
							//printf("%s,%d,chnn:%d no phone online, phone StarSnapJpeg, ChangeSnap,%d, %d, videoLevel:%d,videosize:%d,rateAndLevel:%x\n",
							//	__FILE__,__LINE__,i, rate, rateTime, m_jpegLevel, m_snapVideSize, rateAndLevel);
							ext_set_snap_channel_enable(i, rateAndLevel, rateTime, m_snapVideSize);
						}
						else
						{
							//printf("%s,%d, chnn:%d, no phone online, phone StarSnapJpeg, no snap %d,%d,videosize:%d\n",
							//	__FILE__,__LINE__,i, 0, 0, m_snapVideSize);
							ext_set_snap_channel_enable(i, 0, 0, m_snapVideSize);
						}
					}
				}

			}
			m_pChnnPhoneEableCount[chnn]++;
		}		
	}	
	else
	{		
		if (bCurrHavePhone)
		{
			//在有手机请求jpeg流的情况下
			if (0 == (m_pChnnPhoneEableCount[chnn] + m_pChnnNoPhoneEableCount[chnn]))
			{
				if (2 == m_snapRateTime)
				{
					rate = 1;
					rateTime = 3;
				}
				else if (1 == m_snapRateTime)
				{
					rate = 1;
					rateTime = 2;
				}
				else
				{
					rate = 1;
					rateTime = 3;
				}
#if defined(__SUPPORT_HONGDI__)
				rate = 3;
				rateTime  = 1;
#endif
				rateAndLevel = (m_jpegLevel << 16) + rate;

				//printf("%s,%d,channel:%d, have phone online, other StarSnapJpeg, SetSnapEnable  %d, %d, videoLevel:%d,videosize:%d,rateAndLevel:%x\n",
				//	 __FILE__, __LINE__,chnn, rate, rateTime, m_jpegLevel, m_snapVideSize, rateAndLevel);
				ext_set_snap_channel_enable(chnn, rateAndLevel, rateTime, m_snapVideSize);
			}
			else
			{
				//printf("%s,%d,channel:%d, have phone online, other StarSnapJpeg, no SetSnapEnable,because of this channel is snaping\n",
				//	__FILE__,__LINE__,chnn);
			}
			m_pChnnNoPhoneEableCount[chnn]++;
		}
		else
		{
			if (0 == (m_pChnnPhoneEableCount[chnn] + m_pChnnNoPhoneEableCount[chnn]))
			{
				rate = m_snapRate;
				rateTime = m_snapRateTime;	
#if defined(__SUPPORT_HONGDI__)
				rate = 3;
				rateTime  = 1;
#endif
				rateAndLevel = (m_jpegLevel << 16) + rate;
				//printf("%s,%d,channel:%d, no phone online, other StarSnapJpeg, SetSnapEnable  %d, %d, videoLevel:%d, videosize:%d,rateAndLevel:%x\n",
				//	__FILE__, __LINE__, chnn, rate, rateTime, m_jpegLevel, m_snapVideSize,rateAndLevel);
				ext_set_snap_channel_enable(chnn, rateAndLevel, rateTime, m_snapVideSize);
			}
			else
			{
				//printf("%s,%d, channel:%d, no phone online, other StarSnapJpeg, StarSnapJpeg, no SetSnapEnable,because of this channel is snaping\n",
				//	__FILE__,__LINE__,chnn);
			}
			
			m_pChnnNoPhoneEableCount[chnn]++;
		}
	}

	m_setSnapRateLock.UnLock();
}

void CSnapCapabilityMan::StopSnapJpeg(int chnn, bool bPhone)
{
	if (chnn >= m_chnnNum)
	{
		return;
	}

	m_setSnapRateLock.Lock();

	int rate = 0;
	int rateTime = 0;
	int rateAndLevel = 0;

	if (bPhone)
	{
		assert(m_pChnnPhoneEableCount[chnn] > 0);
		m_pChnnPhoneEableCount[chnn]--;
		if (0 == (m_pChnnPhoneEableCount[chnn]))
		{
			if (m_phoneSnapRaiseChnn == chnn)
			{
				int firstPhoneChannel = -1;
				for (int i = 0; i < m_chnnNum; ++i)
				{
					if (m_pChnnPhoneEableCount[i] > 0)
					{
						firstPhoneChannel = i;
						break;
					}
				}

				m_phoneSnapRaiseChnn = firstPhoneChannel;

				if (m_phoneSnapRaiseChnn > -1)
				{					

					{						
						rate = m_phoneSnapRate;
						rateTime = 1;
#if defined(__SUPPORT_HONGDI__)
						rate = 3;
						rateTime  = 1;
#endif
						rateAndLevel = (m_jpegLevel << 16) + rate;
						//printf("%s,%d,channel:%d,StopSnapJpeg phone.channel:%d is phone Raise channel, set snap:%d,%d.%x,%d\n",
						//	__FILE__,__LINE__,chnn,m_phoneSnapRaiseChnn,rate,rateTime,rateAndLevel,m_snapVideSize);
						ext_set_snap_channel_enable(m_phoneSnapRaiseChnn, rateAndLevel, rateTime, m_snapVideSize);
					}

					{
						if (2 == m_snapRateTime)
						{
							rate = 1;
							rateTime = 3;
						}
						else if (1 == m_snapRateTime)
						{
							rate = 1;
							rateTime = 2;
						}
						else
						{
							rate = 1;
							rateTime = 3;
						}
#if defined(__SUPPORT_HONGDI__)
						rate = 3;
						rateTime  = 1;
#endif
						rateAndLevel = (m_jpegLevel << 16) + rate;

						for (int i = 0; i < m_chnnNum; ++i)
						{
							if (i != m_phoneSnapRaiseChnn)
							{
								if (((m_pChnnPhoneEableCount[i] + m_pChnnNoPhoneEableCount[i]) > 0))
								{
									//printf("%s,%d,channel:%d,StopSnapJpeg phone.channel:%d set snap %d,%d,%x,%d\n",
									//	__FILE__,__LINE__,chnn,i,rate,rateTime,rateAndLevel,m_snapVideSize);
									ext_set_snap_channel_enable(i, rateAndLevel, rateTime, m_snapVideSize);
								}
								else
								{
									//printf("%s,%d,channel:%d,StopSnapJpeg phone.channel:%d set snap 0,0,0,%d\n",
									//	__FILE__,__LINE__,chnn,i,m_snapVideSize);
									ext_set_snap_channel_enable(i, 0, 0, m_snapVideSize);
								}
							}
						}

					}
				}
				else
				{					
					for (int i = 0; i < m_chnnNum; ++i)
					{
						if (((m_pChnnPhoneEableCount[i] + m_pChnnNoPhoneEableCount[i]) > 0))
						{
							rate = m_snapRate;
							rateTime = m_snapRateTime;
#if defined(__SUPPORT_HONGDI__)
							rate = 3;
							rateTime  = 1;
#endif
							rateAndLevel = (m_jpegLevel << 16) + rate;
							//printf("%s,%d,channel:%d,StopSnapJpeg phone.channel:%d set snap %d,%d,%x,%d\n",
							//	__FILE__,__LINE__,chnn,i,rate,rateTime,rateAndLevel,m_snapVideSize);
							ext_set_snap_channel_enable(i, rateAndLevel, rateTime, m_snapVideSize);
						}
						else
						{
							//printf("%s,%d,channel:%d,StopSnapJpeg phone.channel:%d set snap 0,0,0,%d\n",
							//	__FILE__,__LINE__,chnn,i,m_snapVideSize);
							ext_set_snap_channel_enable(i, 0, 0, m_snapVideSize);
						}
					}

				}
			}
			else if (0 == m_pChnnNoPhoneEableCount[chnn])
			{
				rate = 0;
				rateTime = 0;			
				rateAndLevel = 0;
				//printf("%s,%d,channel:%d,StopSnapJpeg phone.because of m_pChnnPhoneEableCount[%d]:%d,m_pChnnNoPhoneEableCount[%d]:%d,channel:%d,set snap %d,%d,%x,%d\n",
				//	   __FILE__,__LINE__,chnn,chnn,m_pChnnPhoneEableCount[chnn],chnn,m_pChnnNoPhoneEableCount[chnn],chnn,rate,rateTime,rateAndLevel,m_snapVideSize);
				ext_set_snap_channel_enable(chnn, rateAndLevel, rateTime, m_snapVideSize);
			}
			else
			{
				//printf("%s,%d, channel:%d,StopSnapJpeg phone,no setsnap ,because snaping,m_pChnnPhoneEableCount[%d]:%d,m_pChnnNoPhoneEableCount[%d]:%d,\n",
				//	__FILE__,__LINE__,chnn,chnn,m_pChnnPhoneEableCount[chnn],chnn,m_pChnnNoPhoneEableCount[chnn]);
			}
		}
		else
		{
			//printf("%s,%d, channel:%d,StopSnapJpeg phone,no setsnap ,because snaping,m_pChnnPhoneEableCount[%d]:%d,m_pChnnNoPhoneEableCount[%d]:%d,\n",
			//	__FILE__,__LINE__,chnn,chnn,m_pChnnPhoneEableCount[chnn],chnn,m_pChnnNoPhoneEableCount[chnn]);
		}
	}
	else
	{
		assert(m_pChnnNoPhoneEableCount[chnn] > 0);
		m_pChnnNoPhoneEableCount[chnn]--;

		if ((m_pChnnNoPhoneEableCount[chnn] + m_pChnnPhoneEableCount[chnn]) > 0)
		{
			//printf("%s,%d,channel:%d, StopSnapJpeg not phone, no SetSnapEnable,because of have snaping\n",
			//	   __FILE__,__LINE__,chnn);

		}
		else
		{
			rate = 0;
			rateTime = 0;
			rateAndLevel = 0;
			//printf("%s,%d,channel:%d StopSnapJpeg not phone, SetSnapEnable  %d, %d, videoLevel:%d, videosize:%d,rateAndLevel:%x\n",
			//	  __FILE__, __LINE__,chnn, rate, rateTime,m_jpegLevel,m_snapVideSize,rateAndLevel);
			ext_set_snap_channel_enable(chnn, rateAndLevel, rateTime, m_snapVideSize);
		}
	}

	m_setSnapRateLock.UnLock();
}


void CSnapCapabilityMan::ChangeJPEGVideoSize(VIDEO_SIZE videoSize)
{
	if (0 == m_chnnNum)
	{
		return;
	}

	m_setSnapRateLock.Lock();

	m_snapVideSize = videoSize;
#if defined(__CHIP3531__) || defined(__CHIPGM__) 
	int rate =0;
	int rateTime =0;
	int rateAndLevel = 0;
	for (int chnn = 0; chnn < m_chnnNum; ++chnn)
	{
		ext_set_snap_channel_enable(chnn, 0, 0, m_snapVideSize);
	}

	if (2 == m_snapRateTime)
	{
		rate = 1;
		rateTime = 3;
	}
	else if (1 == m_snapRateTime)
	{
		rate = 1;
		rateTime = 2;
	}
	else
	{
		rate = 1;
		rateTime = 3;
	}
	rateAndLevel = (m_jpegLevel << 16) + rate;
	for(int chnn=0;chnn<m_chnnNum;++chnn)
	{
		ext_set_snap_channel_enable(chnn, rateAndLevel, rateTime, m_snapVideSize);
	}
#else
	for (int chnn = 0; chnn < m_chnnNum; ++chnn)
	{
		ext_set_snap_channel_enable(chnn, 0, 0, m_snapVideSize);
	}
#endif
	m_setSnapRateLock.UnLock();
}

void CSnapCapabilityMan::ChangeJPEGVideoLevel(unsigned long videLevel)
{
	if (0 == m_chnnNum)
	{
		return;
	}

	m_setSnapRateLock.Lock();

	m_jpegLevel = videLevel;

	m_setSnapRateLock.UnLock();
}











