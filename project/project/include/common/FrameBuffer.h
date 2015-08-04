/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : Ô¬Ê¯Î¬
** Date         : 2007-12-18
** Name         : FrameBuffer.h
** Version      : 1.0
** Description  :
** Modify Record:
1:
***********************************************************************/
#ifndef _FRAME_BUFFER_H_
#define _FRAME_BUFFER_H_

#include "FrameData.h"
#include "mylist.h"

typedef struct _frame_data_info
{
	CFrameData		*pFrameData;
	unsigned long	dataID;
}FRAME_DATA_INFO;

typedef enum _net_stream_type
{
	NET_STREAM_VIDEO,
	NET_STREAM_AUDIO,
	NET_STREAM_PLAYBACK,
}NET_STREAM_TYPE;

class CFrameBuffer
{
public:
	//interface
	CFrameBuffer (NET_STREAM_TYPE type, ULONG len, ULONG time);
	~CFrameBuffer();

	bool AddFrame(CFrameData *pFrameData, bool & bKeyFrame);
	bool GetFrame(unsigned long & dataID, FRAME_INFO_EX **ppFrameInfo, BYTE *pData, ULONG &bufLen, ULONG offset, bool & bSameID);
	void RemoveAll();


protected:
	//methods

	//variable
private:
	//methods
	bool AddToList(CFrameData *pFrameData, bool &bKeyFrame);
	void RemoveFrame();

	void RefreshFirstTime();

	//variable
	bool			m_bKeyFrame;
	NET_STREAM_TYPE	m_type;
	unsigned long	m_dataLen;
	unsigned long	m_maxDataLen;
	LONGLONG		m_maxDataTime; //Ãë
	LONGLONG		m_fristFrameTime;
	
	CPUB_Lock			m_listLock;

	CMyList <FRAME_DATA_INFO> m_frameList;

	static unsigned long	m_currIndex;
	static CPUB_Lock		m_indexLock;
};
#endif //_FRAME_BUFFER_H_
