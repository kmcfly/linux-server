/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : Ԭʯά
** Date         : 2007-10-11
** Name         : PreRecord.h
** Version      : 1.0
** Description  :
** Modify Record:
1:
***********************************************************************/
#ifndef _PRE_RECORD_
#define _PRE_RECORD_
#include "PreRecordDataImp.h"
#include "FrameData.h"


class  CPreRecord
{
public:
	//interface
	CPreRecord();
	virtual ~CPreRecord();

	//
	void Initial(unsigned char channel, char *pPath);

	bool IsCanPreRecord();

	//Ԥ¼��ʱ��ֻ�ܵ���AddFrame�������ݱ��浽Ԥ¼��ģ��
	bool AddFrame(CFrameData * pFrameData, bool & bKeyFrame, bool bDelete = true);

	//Ԥ¼�������Ҫ��Ԥ¼��ģ���������ݣ�Ҫ�ȵ���GetFrame,Ȼ��RemoveHead
	//����������������ֻ����Ԥ¼�������Ҫ��Ԥ¼��ģ����������ʱ���á���������£����ܵ���
	CFrameData * GetFrame();
	CFrameData * RemoveHead();

	void RemoveAll();

	bool CurKeyFrame();
	unsigned long GetCount();
	void SetPreRecTime(unsigned long time);
	void RecoverPreRecTime();

	//���������ļ������Ӳ��·��
	void ResetPreRecordPath(char * path, bool &bPreRecord);
protected:
	//methods

	//variable

private:
	//methods
	bool AddToList(CFrameData * pFrameData, bool & bKeyFrame);
	void RemoveFrame();
	void RefreshFirstTime();

	//variable
	CMyList <CFrameData *> m_frameList;

	unsigned long		m_dataLen;
	unsigned long		m_maxDataLen;
	LONGLONG			m_fristFrameTime;
	LONGLONG			m_maxDataTime; //sec
	LONGLONG            m_oldMaxDataTime;

	CPreRecordDataImp   m_prerecordDataImp;
	bool                m_bCanRecord;

};

#endif //_PRE_RECORD_








