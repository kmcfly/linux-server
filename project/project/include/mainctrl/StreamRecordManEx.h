#ifndef __STREAM_RECORD_MANAGEMENT_EXTEND_H__
#define __STREAM_RECORD_MANAGEMENT_EXTEND_H__
#include "RecordMan.h"
class CStreamRecordManEx
{
public:
	~CStreamRecordManEx();
	static CStreamRecordManEx* Instance();
	bool Initial(unsigned char videoInputNum, unsigned char netVideoInputNum,  unsigned char audioInputNum, CRecordMan *pRecordMan);

	bool IsSupportMinorStreamRecord();
	unsigned long ChannelNumOfSupport1080pReplay();
	
	RECORD_WRITE_STATUS DateProc(CFrameData & frameData, bool & bKeyFrame);
	bool Record(unsigned char chnn, RECORD_TYPE type);
	bool StopRecord(unsigned char chnn, RECORD_TYPE type);


	//�������ò���
	//ֻȡ��Ƶ����Ƶ
	void SetRecEnable(FRAME_TYPE type, bool bEnable, unsigned char chnn);
	//ֻȡ�ų̡��ƶ���⡢����
	void SetWeekSchedule(RECORD_TYPE type, const WEEK_SCHEDULE & weekSch, unsigned char chnn);
	void SetHolidaySchedule(RECORD_TYPE type, const CMyList <HOLIDAY_SCHEDULE> &holidayList, unsigned char chnn);
	void SetHoldTime(unsigned long holdTime, unsigned char chnn);
	void SetPreRecTime(unsigned long time, unsigned char chnn);
	void SetPreRecDataLen(unsigned char chnn, unsigned long maxPreRecDataLen);


	void SetRedundancyRecord(bool bEnable, unsigned char chnn);
	//�������ͨ����״̬�����Ƿ�����ֶ�¼�񼰼ƻ�¼��
	void SetChannelLive(unsigned char chnn, bool bLive);
private:
	CStreamRecordManEx();

private:
	CRecordMan *m_pRecordMan;

	unsigned char	m_videoInputNum;
	unsigned char	m_netVideoInputNum;
	unsigned char	m_VirtualNetVideoInputNum;
	unsigned char	m_localVideoInputNum;
	unsigned char	m_audioInputNum;

	bool m_bIsSupportMinorStreamRecord;
	unsigned long m_channelNumOfSupport1080pReplay;
	
};


#endif