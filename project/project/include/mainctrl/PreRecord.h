/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : 袁石维
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

	//预录像时，只能调用AddFrame，把数据保存到预录像模块
	bool AddFrame(CFrameData * pFrameData, bool & bKeyFrame, bool bDelete = true);

	//预录像结束后，要从预录像模块拿走数据，要先调用GetFrame,然后RemoveHead
	//下面这两个函数，只能在预录像结束后，要从预录像模块拿走数据时调用。其他情况下，不能调用
	CFrameData * GetFrame();
	CFrameData * RemoveHead();

	void RemoveAll();

	bool CurKeyFrame();
	unsigned long GetCount();
	void SetPreRecTime(unsigned long time);
	void RecoverPreRecTime();

	//重新设置文件保存的硬盘路径
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








