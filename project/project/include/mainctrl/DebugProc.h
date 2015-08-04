/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : YSW
** Date         : 2010-02-03
** Name         : DebugProc.h
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/
#ifndef _DEBUG_PROC_H_
#define _DEBUG_PROC_H_

#include "MessageMan.h"

class CDebugProc
{
public:
	//////////////////////////////////////////////////////////////////////////
	typedef enum _debug_type
	{
		DEBUG_LIVE,
		DEBUG_RECORD,
		DEBUG_SEARCH,
		DEBUG_PLAYBACK,
		DEBUG_INFORMATION
	}DEBUG_TYPE;
	//////////////////////////////////////////////////////////////////////////
	static CDebugProc * Instance();
	virtual ~CDebugProc();

	bool Initial();
	void Quit();

	bool Start();
	void Stop();

	inline void Runing(bool bRuning){m_bRuning = bRuning;}

	void SetRect(unsigned short width, unsigned short height);
	void SetDebug(DEBUG_TYPE type, bool bOpen, unsigned long time = 60, unsigned long times = 10);
protected:
private:
	CDebugProc();
	static PUB_THREAD_RESULT PUB_THREAD_CALL DebugProcThread(void *pParam);
	int DebugProc();

	//
	void LiveProc(KEY_VALUE_INFO & keyValue);
	void RecordProc(KEY_VALUE_INFO & keyValue);
	void SearchProc(KEY_VALUE_INFO & keyValue);
	void PlabybackProc(KEY_VALUE_INFO & keyValue);
	void InfoProc(KEY_VALUE_INFO & keyValue);
	//////////////////////////////////////////////////////////////////////////
	typedef struct _debug_info
	{
		unsigned long	bOpen;
		unsigned long	time;
		unsigned long	times;
	}DEBUG_INFO;

	unsigned short			m_width;
	unsigned short			m_height;

	PUB_thread_t			m_debugProcID;
	bool					m_bDebugProc;
	bool					m_bRuning;

	DEBUG_INFO				m_infoLive;
	DEBUG_INFO				m_infoRecord;
	DEBUG_INFO				m_infoSearch;
	DEBUG_INFO				m_infoPlayback;
	DEBUG_INFO				m_infoInformation;
};
#endif //_DEBUG_PROC_H_
