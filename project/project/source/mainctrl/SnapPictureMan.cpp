/////////
//#include "PUB_common.h"
#include "Product.h"
//#include "NetServer.h"
#include "dvr_func.h"
#include "SnapPictureMan.h"
#include "SnapMan.h"
#include "SnapCapabilityMan.h"

CSnapPictureMan *CSnapPictureMan::m_Instance = NULL;

CSnapPictureMan::CSnapPictureMan(void)
{
	m_pChnnStatus = NULL;
	m_pChnnCount = NULL;
	m_pLastTime = NULL;
	m_pChannType = NULL;
}

CSnapPictureMan::~CSnapPictureMan(void)
{
	Quite();
}

bool CSnapPictureMan::Initial(unsigned char chnnNum)
{
	//printf("Initial =========\n");
	int channelNum = chnnNum;

	m_nMaxSnap = 3;
	m_nMaxTime = 2;

	if (NULL == m_pChnnStatus)
	{
		m_pChnnStatus = new int [channelNum];
		memset(m_pChnnStatus, 0, sizeof(int)*channelNum);
	}
	
	if (NULL == m_pChnnCount)
	{
		m_pChnnCount = new int [channelNum];
		memset(m_pChnnCount, 0, sizeof(int)*channelNum);
	}
	
	if (NULL == m_pLastTime)
	{
		m_pLastTime = new LONGLONG [channelNum];
		memset(m_pLastTime, 0, sizeof(LONGLONG)*channelNum);
	}

	if (NULL == m_pChannType)
	{
		m_pChannType = new unsigned long[channelNum];
		/*for (int i =0; i< channelNum; i++)
		{
			m_pChannType[i] = ALARM_TYPE_NULL;
		}*/
		memset(m_pChannType, 0, sizeof(unsigned long)*channelNum);
	}

	return true;
}

bool CSnapPictureMan::Quite()
{
	if (m_pChnnStatus != NULL)
	{
		delete [] m_pChnnStatus;
		m_pChnnStatus = NULL;
	}

	if (m_pChnnCount != NULL)
	{
		delete [] m_pChnnCount;
		m_pChnnCount = NULL;
	}

	if (m_pLastTime != NULL)
	{
		delete [] m_pLastTime;
		m_pLastTime = NULL;
	}

	if (NULL != m_pChannType)
	{
		delete []m_pChannType;
		m_pChannType = NULL;
	}
	return true;
}

CSnapPictureMan* CSnapPictureMan::GetInstance()
{
	if (NULL == m_Instance)
	{
		m_Instance = new CSnapPictureMan;
	}
	return m_Instance;
}

bool CSnapPictureMan::SetSnapPicture(unsigned long chnnl, unsigned long type)
{
	m_lock.Lock();

	if (0 == m_nMaxSnap)
	{
		m_lock.UnLock();
		return false;
	}


	if (m_pChannType[chnnl] != 0)
	{
		//printf("======%d\n", m_pChannType[chnnl]);
		m_lock.UnLock();
		return true;
	}
	
	m_pChannType[chnnl] = type;

	if (!m_pChnnStatus[chnnl])
	{
		m_pChnnStatus[chnnl] = 1;
		m_nCurSnapNum = m_nMaxSnap;
	}

	//调用打开抓图通道。
	{
		//printf("begine snap picture channel = %02d! file = %s, line = %d\n", chnnl, __FILE__, __LINE__);
#if !defined(__SUPPORT_HONGDI__)
		CSnapCapabilityMan::Instance()->StarSnapJpeg(chnnl, false);
#endif

	}

	m_lock.UnLock();
	return true;
}

bool CSnapPictureMan::SnapPicture(void *buf, int nLen, int nChannel, LONGLONG time , unsigned short width, unsigned short height, bool bLock)
{
	m_lock.Lock();

	if (!m_pChnnStatus[nChannel])
	{
		m_lock.UnLock();
		return true;
	}
	

	if (m_pChnnCount[nChannel] < m_nCurSnapNum)
	{
		bool bSnap = false;
		if (time <= m_pLastTime[nChannel])
		{
			bSnap = true;
		}
		else if(((time-m_pLastTime[nChannel])/1000000) >= m_nMaxTime)
		{
			bSnap = true;
		}
		else
		{
			bSnap = false;
		}
		if (bSnap)
		{
			CSnapMan::GetInstance()->SaveSnapPicture(buf, nLen, m_pChannType[nChannel], nChannel, time, width, height, bLock);
			m_pChnnCount[nChannel]++;
			m_pLastTime[nChannel] = time;
		}
		
	}

	if (m_pChnnCount[nChannel] >= m_nCurSnapNum)
	{
		m_pChnnStatus[nChannel] = 0;
		m_pChnnCount[nChannel] = 0;
		m_pChannType[nChannel] = 0;
		m_pLastTime[nChannel] = 0;

		//printf("end snap pictrue channel = %02d! file = %s, line = %d\n", nChannel, __FILE__, __LINE__);
		//调用stop。
#if defined(__SUPPORT_HONGDI__)
		CSnapCapabilityMan::Instance()->StopSnapJpeg(nChannel, false);
#endif

	}

	m_lock.UnLock();
	return true;
}

void CSnapPictureMan::SetSnapTimeS(const unsigned long timeS)
{
	m_lock.Lock();
	m_nMaxTime = timeS;
	m_lock.UnLock();
}

void CSnapPictureMan::SetSnapImgNum(const unsigned long imgNum)
{
	m_lock.Lock();

	m_nMaxSnap = imgNum;
	m_lock.UnLock();
}

unsigned long CSnapPictureMan::GetSnapTime()
{
	m_lock.Lock();
	if ((0 == m_nMaxTime) || (0 == m_nMaxSnap))
	{
		m_lock.UnLock();
		return 0;
	}

	unsigned long retTime = m_nMaxTime * m_nMaxSnap;

	if (retTime > 20)
	{
		retTime = 20;
	}

	m_lock.UnLock();
	return retTime;
	
}

