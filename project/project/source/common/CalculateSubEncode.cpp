/*
* Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
* Author       : xian shi wei
* Date         : 2015-01-05
* Name         : EncodeSubCalculate.cpp
* Version      : 1.0
* Description  : 

* Modify Record:

*/

#include "CalculateSubEncode.h"

#include <string.h>
#include <assert.h>
#include <stdio.h>

CCalculateSubEncode * CCalculateSubEncode::m_pSubEncode = NULL;

CCalculateSubEncode::CCalculateSubEncode()
{
	m_channel = 0;
	m_resNum  = 0;
	memset(m_deviceEncInfo, 0, sizeof(m_deviceEncInfo));
	m_bSourceSelf = true;
	m_bHaveInitial = false;
}

CCalculateSubEncode::~CCalculateSubEncode()
{
	NULL != m_pChannelResMask?
		delete [] m_pChannelResMask,
		m_pChannelResMask = NULL:
		NULL;
}

CCalculateSubEncode * CCalculateSubEncode::Instance()
{
	if (NULL == m_pSubEncode)
	{
        //¼ÓËø
		if (NULL == m_pSubEncode)
		{
			m_pSubEncode = new CCalculateSubEncode;
		}
	}

	return m_pSubEncode;
}

bool CCalculateSubEncode::Initial(unsigned long ulChnn)
{
	m_channel = ulChnn;
	m_bHaveInitial = true;
	//m_pChannelInfo = new SUPPORT_SUB_ENC_INFO__ [ulChnn];
	m_pChannelResMask = new unsigned long [ulChnn];
	memset(m_pChannelResMask,0,ulChnn * sizeof(unsigned long));
	return true;
}


int CCalculateSubEncode::AddEncodeInfo(unsigned long ulRes, unsigned short usTotalRate, unsigned short usMaxRate, unsigned short usTotalBitrate)
{
	if (m_resNum >= RESOLUTION_TOTAL_NUM_)
	{
		return -1;
	}
	SUPPORT_SUB_ENC_INFO * pSupportEncInfo = new SUPPORT_SUB_ENC_INFO [m_channel];
	memset(pSupportEncInfo, 0, sizeof(SUPPORT_SUB_ENC_INFO) * m_channel);

	m_deviceEncInfo[m_resNum].pSupportEncInfo = pSupportEncInfo;

	m_deviceEncInfo[m_resNum].usTotalRate = usTotalRate;
	m_deviceEncInfo[m_resNum].usTotalBitrate = usTotalBitrate;
	m_deviceEncInfo[m_resNum].ulRes = ulRes;
    
	for (int i=0; i< m_channel; i++)
	{
		pSupportEncInfo[i].chnn             = i;
		pSupportEncInfo[i].ulRes			= ulRes;
		pSupportEncInfo[i].usMaxRate		= usMaxRate;
		pSupportEncInfo[i].usMaxBitrate	= 16 * 1024;
		pSupportEncInfo[i].ulDefBitrate	= GetDefBitrate(ulRes);
	}

	m_resNum++;
	return m_resNum;
}



int CCalculateSubEncode::GetEncodeInfo(SUPPORT_SUB_ENC_INFO * pSupportEncInfo, unsigned long & dataLen)
{
	if (NULL == pSupportEncInfo)
	{
		return -1;
	}

	if (dataLen < sizeof(SUPPORT_SUB_ENC_INFO) * m_channel)
	{
		return -1;
	}

	for (int iChnn=0; iChnn<m_channel; iChnn++)
	{
		for (int i=0; i<m_resNum; i++)
		{
			if (1 == m_deviceEncInfo[i].usBCurrentRes[iChnn])
			{
				memcpy(pSupportEncInfo + iChnn, m_deviceEncInfo[i].pSupportEncInfo + iChnn, sizeof(SUPPORT_SUB_ENC_INFO));
				break;
			}
		}
	}

	dataLen = sizeof(SUPPORT_SUB_ENC_INFO) * m_channel;

	return 0;
}

int CCalculateSubEncode::GetEncodeAllInfo(SUPPORT_SUB_ENC_INFO * pSupportEncInfo, unsigned long & dataLen)
{
	if (NULL == pSupportEncInfo)
	{
		return -1;
	}

	for (int iChnn=0; iChnn<m_channel; iChnn++)
	{
		for (int i=0; i<m_resNum; i++)
		{
			SUPPORT_SUB_ENC_INFO * pTemp = m_deviceEncInfo[i].pSupportEncInfo + iChnn;
			memcpy( pSupportEncInfo+(iChnn*m_resNum + i), pTemp, sizeof(SUPPORT_SUB_ENC_INFO));
			if ( m_pChannelResMask[iChnn] & (pTemp->ulRes) ){
				break;		
			}
		}
	}

	dataLen = sizeof(SUPPORT_SUB_ENC_INFO) * m_channel * m_resNum;

	return 0;
}

int CCalculateSubEncode::SetChannelResMask(unsigned long ulChnn, unsigned long vedioSize)
{
	if (ulChnn < m_channel)
	{
		m_pChannelResMask[ulChnn] = 0;
		m_pChannelResMask[ulChnn] |= vedioSize;
		return 0;
	}
	return -1;
}

bool CCalculateSubEncode::CheckEncodeParam(unsigned long ulChnn, CHECK_ENC_INFO * pCheckEncInfo)
{
	if (m_bSourceSelf)
	{
		if (ulChnn < m_channel)
		{
			unsigned long maxRate = 0;
			for (int j=0; j<m_resNum; j++)
			{
				if (pCheckEncInfo[ulChnn].ulRes == m_deviceEncInfo[j].ulRes)
				{
					maxRate = m_deviceEncInfo[j].pSupportEncInfo[ulChnn].usMaxRate;
					break;
				}
			}

			if (maxRate < pCheckEncInfo[ulChnn].usRate)
			{
				pCheckEncInfo[ulChnn].usRate = maxRate;
			}
		}
		else
		{
			assert(false);
		}
	}
	else
	{
		if (ulChnn < m_channel)
		{
			unsigned long usedRate = 0;
			for (int i=0; i<m_channel; i++)
			{
				if (ulChnn == i)
				{
					continue;
				}

				for (int j=0; j<m_resNum; j++)
				{
					if (pCheckEncInfo[i].ulRes == m_deviceEncInfo[j].ulRes)
					{
						usedRate += (pCheckEncInfo[i].usRate * 10000) / m_deviceEncInfo[j].usTotalRate;
						break;
					}
				}
			}

			unsigned long lastRate = 0;

			for (int j=0; j<m_resNum; j++)
			{
				if (pCheckEncInfo[ulChnn].ulRes == m_deviceEncInfo[j].ulRes)
				{
					lastRate = (m_deviceEncInfo[j].usTotalRate * (10000 - usedRate)) / 10000;
					break;
				}
			}

			if (lastRate < pCheckEncInfo[ulChnn].usRate)
			{
				pCheckEncInfo[ulChnn].usRate = lastRate;
			}
		}
		else if (0xffffffff == ulChnn)
		{
			unsigned long useRate = 0;
			unsigned long ulRes = 0;
			unsigned long ulTotalRate = 0;

			if (0 != m_deviceEncInfo[0].ulRes)
			{
				ulRes = m_deviceEncInfo[0].ulRes;
				ulTotalRate = m_deviceEncInfo[0].usTotalRate;
				useRate = m_deviceEncInfo[0].pSupportEncInfo[0].usMaxRate;
			}

			for (int j=0; j<m_resNum; j++)
			{
				if (pCheckEncInfo[0].ulRes == m_deviceEncInfo[j].ulRes)
				{
					ulTotalRate = m_deviceEncInfo[j].usTotalRate;

					useRate = pCheckEncInfo[0].usRate;
					ulRes = pCheckEncInfo[0].ulRes;

					break;
				}
			}

			if (pCheckEncInfo[0].usRate * m_channel > ulTotalRate)
			{
				useRate = ulTotalRate / m_channel;
			}

			for (int i=0; i<m_channel; i++)
			{
				pCheckEncInfo[i].ulRes = ulRes;
				pCheckEncInfo[i].usRate = useRate;
			}
		}
		else
		{
			unsigned long useRate = 0;
			unsigned long ulRes = 0;
			unsigned long useBitrate = 0;

			if (0 != m_deviceEncInfo[0].ulRes)
			{
				ulRes = m_deviceEncInfo[0].ulRes;
				useBitrate = m_deviceEncInfo[0].pSupportEncInfo[0].ulDefBitrate;
				useRate = m_deviceEncInfo[0].pSupportEncInfo[0].usMaxRate;
			}

			for (int i=0; i<m_channel; i++)
			{
				pCheckEncInfo[i].ulRes = ulRes;
				pCheckEncInfo[i].usRate = useRate;
				pCheckEncInfo[i].usBitrate = useBitrate;
			}

			return false;
		}

	}

	return true;
}

void CCalculateSubEncode::SetChannelRes(unsigned long ulChnn, unsigned long ulRes)
{
	if (m_bHaveInitial)
	{
		int iPos = -1;

		for (int i=0; i<m_resNum; i++)
		{
			if (ulRes == m_deviceEncInfo[i].ulRes)
			{
				iPos = i;
				break;
			}
		}

		if (-1 != iPos)
		{
			for (int i=0; i<m_resNum; i++)
			{
				if (iPos == i)
				{
					m_deviceEncInfo[i].usBCurrentRes[ulChnn] = 1;
				}
				else
				{
					m_deviceEncInfo[i].usBCurrentRes[ulChnn] = 0;
				}
			}
		}
	}
}

unsigned long CCalculateSubEncode::GetDefBitrate(unsigned long ulRes)
{
	switch (ulRes)
	{
	case DVRVIDEO_SIZE_QCIF:
		return 256;
	case DVRVIDEO_SIZE_CIF:
		return 512;
	case DVRVIDEO_SIZE_HD1:
		return 512;
	case DVRVIDEO_SIZE_D1:
		return 1024;
	case DVRVIDEO_SIZE_HD1080:
		return 4096;
	case DVRVIDEO_SIZE_HD720:
		return 2048;
	case DVRVIDEO_SIZE_960H:
		return 1536;
	case DVRVIDEO_SIZE_VGA:
		return 512;
	case DVRVIDEO_SIZE_QVGA:
		return 512;
	case DVRVIDEO_SIZE_960P:
		return 2048;
	case DVRVIDEO_SIZE_SXGA:
		return 2048;
	case DVRVIDEO_SIZE_3M:
		return 4096;
	case DVRVIDEO_SIZE_16_9_3M:
		return 8192;
	case DVRVIDEO_SIZE_4M:
		return 8192;
	case DVRVIDEO_SIZE_5M:
		return 8192;
	default:
		return 512;
	}
}

void CCalculateSubEncode::ClearData()
{
	memset(m_deviceEncInfo,0,sizeof(DEVICE_SUB_ENC_INFO)*RESOLUTION_TOTAL_NUM_ );
}