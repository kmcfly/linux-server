#include "StreamRecordManEx.h"

CStreamRecordManEx::CStreamRecordManEx()
{
	m_pRecordMan=NULL;

	m_videoInputNum	= 0;
	m_localVideoInputNum = 0;
	m_netVideoInputNum = 0;
	m_VirtualNetVideoInputNum = 0;
	m_audioInputNum	= 0;

}

CStreamRecordManEx::~CStreamRecordManEx()
{
	
}

CStreamRecordManEx*  CStreamRecordManEx::Instance()
{
	static CStreamRecordManEx s_liveStreamRecordMan;
	return &s_liveStreamRecordMan;
}

bool CStreamRecordManEx::IsSupportMinorStreamRecord()
{
	return m_bIsSupportMinorStreamRecord;
}

unsigned long CStreamRecordManEx::ChannelNumOfSupport1080pReplay()
{
	return m_channelNumOfSupport1080pReplay;
}



bool  CStreamRecordManEx::Initial(unsigned char videoInputNum, unsigned char netVideoInputNum,  unsigned char audioInputNum, CRecordMan *pRecordMan)
{
	m_pRecordMan = pRecordMan;

	assert ((0 < videoInputNum) && (videoInputNum <= MAX_CHNN_NUM));
	assert ((0 <= netVideoInputNum) && (netVideoInputNum <= MAX_CHNN_NUM));
	assert ((0 <= audioInputNum) && (audioInputNum <= MAX_CHNN_NUM));

	m_videoInputNum	= videoInputNum;
	m_localVideoInputNum = videoInputNum - netVideoInputNum;
	m_netVideoInputNum = netVideoInputNum;
	m_VirtualNetVideoInputNum = netVideoInputNum;
	m_audioInputNum	= audioInputNum;

//���ݲ�ͬƽ̨������С�����طŹ��ܿ���
//�Լ����֧�ֵ��������ط�·��N (�ط�·������N ��
//�����лط�ͨ������С����¼�����ݣ�����������������)

	m_bIsSupportMinorStreamRecord = false;
	m_channelNumOfSupport1080pReplay = 0;

#if defined(__CHIP3520D__)
	if (0 < m_netVideoInputNum &&  0 == m_localVideoInputNum)
	{
		m_bIsSupportMinorStreamRecord = true;
		if (m_bIsSupportMinorStreamRecord)
		{
			m_channelNumOfSupport1080pReplay = 1;
		}
		else
		{
			m_channelNumOfSupport1080pReplay = 0;
		}
	}
#endif
	return true;
}




RECORD_WRITE_STATUS CStreamRecordManEx::DateProc(CFrameData & frameData, bool & bKeyFrame)
{
	if(m_bIsSupportMinorStreamRecord)
	{
		unsigned long chnn = CHANNEL_ID(frameData.GetFrameInfo().channel);

		assert(chnn<m_videoInputNum+m_VirtualNetVideoInputNum);
		
		if(chnn>=m_localVideoInputNum && chnn<m_videoInputNum)
		{
			chnn += m_VirtualNetVideoInputNum;
		}

		FRAME_INFO_EX &frameInfo = frameData.GetFrameInfo();
		frameInfo.channel = chnn;
		
		return m_pRecordMan->DateProc(frameData,bKeyFrame);
	}

	return RECORD_WRITE_FAIL;
}

bool CStreamRecordManEx::Record(unsigned char chnn, RECORD_TYPE type)
{
	assert(chnn<m_videoInputNum+m_VirtualNetVideoInputNum);
	bool ret = false;

	//������¼��
	if(chnn>=m_videoInputNum && chnn<m_videoInputNum+m_VirtualNetVideoInputNum)
	{
		chnn -= m_VirtualNetVideoInputNum;
	}
	ret = m_pRecordMan->Record(chnn,type);

	//С����¼��С����¼��ͨ��Ϊ����ͨ���������Ҫת����ת��Ϊ����ͨ��
	if(chnn>=m_localVideoInputNum && chnn<m_videoInputNum)
	{
		chnn += m_VirtualNetVideoInputNum;
	}
	if(m_bIsSupportMinorStreamRecord)
	{
		ret = (ret && m_pRecordMan->Record(chnn,type));
	}
	return ret;

	
}


bool CStreamRecordManEx::StopRecord(unsigned char chnn, RECORD_TYPE type)
{
	assert(chnn<m_videoInputNum+m_VirtualNetVideoInputNum);
	bool ret = false;
	
	//ֹͣ������¼��
	if(chnn>=m_videoInputNum && chnn<m_videoInputNum+m_VirtualNetVideoInputNum)
	{
		chnn -= m_VirtualNetVideoInputNum;
	}
	ret = m_pRecordMan->StopRecord(chnn,type);

	//ֹͣС����¼��С����¼��ͨ��Ϊ����ͨ���������Ҫת����ת��Ϊ����ͨ��
	if(chnn>=m_localVideoInputNum && chnn<m_videoInputNum)
	{
		chnn += m_VirtualNetVideoInputNum;
	}
	if(m_bIsSupportMinorStreamRecord)
	{
		ret = (ret && m_pRecordMan->StopRecord(chnn,type));
	}
	return ret;

}


//�������ò���
//ֻȡ��Ƶ����Ƶ
void CStreamRecordManEx::SetRecEnable(FRAME_TYPE type, bool bEnable, unsigned char chnn)
{
	assert(chnn<m_videoInputNum+m_VirtualNetVideoInputNum);
	if(chnn>=m_videoInputNum && chnn<m_videoInputNum+m_VirtualNetVideoInputNum)
	{
		chnn -= m_VirtualNetVideoInputNum;
	}
	m_pRecordMan->SetRecEnable(type,bEnable,chnn);


	if(m_bIsSupportMinorStreamRecord)
	{
		if(chnn>=m_localVideoInputNum && chnn<m_videoInputNum)
		{
			chnn += m_VirtualNetVideoInputNum;
		}
		m_pRecordMan->SetRecEnable(type,bEnable,chnn);
	}

}

//ֻȡ�ų̡��ƶ���⡢����
void CStreamRecordManEx::SetWeekSchedule(RECORD_TYPE type, const WEEK_SCHEDULE & weekSch, unsigned char chnn)
{
	assert(chnn<m_videoInputNum+m_VirtualNetVideoInputNum);
	if(chnn>=m_videoInputNum && chnn<m_videoInputNum+m_VirtualNetVideoInputNum)
	{
		chnn -= m_VirtualNetVideoInputNum;
	}
	m_pRecordMan->SetWeekSchedule(type,weekSch,chnn);


	if(m_bIsSupportMinorStreamRecord)
	{
		if(chnn>=m_localVideoInputNum && chnn<m_videoInputNum)
		{
			chnn += m_VirtualNetVideoInputNum;
		}
		m_pRecordMan->SetWeekSchedule(type,weekSch,chnn);
	}
}

void CStreamRecordManEx::SetHolidaySchedule(RECORD_TYPE type, const CMyList <HOLIDAY_SCHEDULE> &holidayList, unsigned char chnn)
{
	assert(chnn<m_videoInputNum+m_VirtualNetVideoInputNum);
	if(chnn>=m_videoInputNum && chnn<m_videoInputNum+m_VirtualNetVideoInputNum)
	{
		chnn -= m_VirtualNetVideoInputNum;
	}
	m_pRecordMan->SetHolidaySchedule(type,holidayList,chnn);


	if(m_bIsSupportMinorStreamRecord)
	{
		if(chnn>=m_localVideoInputNum && chnn<m_videoInputNum)
		{
			chnn += m_VirtualNetVideoInputNum;
		}
		m_pRecordMan->SetHolidaySchedule(type,holidayList,chnn);
	}

}

void CStreamRecordManEx::SetHoldTime(unsigned long holdTime, unsigned char chnn)
{
	assert(chnn<m_videoInputNum+m_VirtualNetVideoInputNum);
	if(chnn>=m_videoInputNum && chnn<m_videoInputNum+m_VirtualNetVideoInputNum)
	{
		chnn -= m_VirtualNetVideoInputNum;
	}
	m_pRecordMan->SetHoldTime(holdTime,chnn);


	if(m_bIsSupportMinorStreamRecord)
	{
		if(chnn>=m_localVideoInputNum && chnn<m_videoInputNum)
		{
			chnn += m_VirtualNetVideoInputNum;
		}
		m_pRecordMan->SetHoldTime(holdTime,chnn);
	}

}

void CStreamRecordManEx::SetPreRecTime(unsigned long time, unsigned char chnn)
{
	assert(chnn<m_videoInputNum+m_VirtualNetVideoInputNum);
	if(chnn>=m_videoInputNum && chnn<m_videoInputNum+m_VirtualNetVideoInputNum)
	{
		chnn -= m_VirtualNetVideoInputNum;
	}
	m_pRecordMan->SetPreRecTime(time,chnn);


	if(m_bIsSupportMinorStreamRecord)
	{
		if(chnn>=m_localVideoInputNum && chnn<m_videoInputNum)
		{
			chnn += m_VirtualNetVideoInputNum;
		}
		m_pRecordMan->SetPreRecTime(time,chnn);
	}
}

void CStreamRecordManEx::SetPreRecDataLen(unsigned char chnn, unsigned long maxPreRecDataLen)
{
	assert(chnn<m_videoInputNum+m_VirtualNetVideoInputNum);
	if(chnn>=m_videoInputNum && chnn<m_videoInputNum+m_VirtualNetVideoInputNum)
	{
		chnn -= m_VirtualNetVideoInputNum;
	}
	m_pRecordMan->SetPreRecDataLen(chnn,maxPreRecDataLen);


	if(m_bIsSupportMinorStreamRecord)
	{
		if(chnn>=m_localVideoInputNum && chnn<m_videoInputNum)
		{
			chnn += m_VirtualNetVideoInputNum;
		}
		m_pRecordMan->SetPreRecDataLen(chnn,maxPreRecDataLen);
	}
}



void CStreamRecordManEx::SetRedundancyRecord(bool bEnable, unsigned char chnn)
{
	assert(chnn<m_videoInputNum+m_VirtualNetVideoInputNum);
	if(chnn>=m_videoInputNum && chnn<m_videoInputNum+m_VirtualNetVideoInputNum)
	{
		chnn -= m_VirtualNetVideoInputNum;
	}
	m_pRecordMan->SetRedundancyRecord(bEnable,chnn);


	if(m_bIsSupportMinorStreamRecord)
	{
		if(chnn>=m_localVideoInputNum && chnn<m_videoInputNum)
		{
			chnn += m_VirtualNetVideoInputNum;
		}
		m_pRecordMan->SetRedundancyRecord(bEnable,chnn);
	}
}

//�������ͨ����״̬�����Ƿ�����ֶ�¼�񼰼ƻ�¼��
void CStreamRecordManEx::SetChannelLive(unsigned char chnn, bool bLive)
{
	assert(chnn<m_videoInputNum+m_VirtualNetVideoInputNum);
	if(chnn>=m_videoInputNum && chnn<m_videoInputNum+m_VirtualNetVideoInputNum)
	{
		chnn -= m_VirtualNetVideoInputNum;
	}
	m_pRecordMan->SetChannelLive(chnn,bLive);


	if(m_bIsSupportMinorStreamRecord)
	{
		if(chnn>=m_localVideoInputNum && chnn<m_videoInputNum)
		{
			chnn += m_VirtualNetVideoInputNum;
		}
		m_pRecordMan->SetChannelLive(chnn,bLive);
	}
}








