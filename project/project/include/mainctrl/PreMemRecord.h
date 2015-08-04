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
#ifndef _PRE_MEM_RECORD_
#define _PRE_MEM_RECORD_

#include "FrameData.h"

class  CPreMemRecord
{
public:
	//interface
	CPreMemRecord();
	virtual ~CPreMemRecord();

	bool AddFrame(CFrameData * pFrameData, bool & bKeyFrame);
	CFrameData * GetFrame();

	CFrameData * RemoveHead();
	void RemoveAll();
	bool RemoveSection();

	unsigned long GetCount();
	void SetPreRecTime(unsigned long time);
	void SetPreRecDataLen(unsigned long maxPreRecDataLen);
protected:
	//methods

	//variable

private:
	//methods
	bool AddToList(CFrameData * pFrameData);
	void RemoveFrame();
	void RefreshFirstTime();

	//variable
	CMyList <CFrameData *> m_frameList;

	unsigned long		m_dataLen;
	unsigned long		m_maxDataLen;
	LONGLONG			m_fristFrameTime;
	LONGLONG			m_maxDataTime; //sec
};

#endif //_PRE_MEM_RECORD_








