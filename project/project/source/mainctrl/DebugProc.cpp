/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : YSW
** Date         : 2010-02-03
** Name         : DebugProc.cpp
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/
#include "DebugProc.h"
#include "KeyProc.h"
#include "Dialog.h"

CDebugProc * CDebugProc::Instance()
{
	static CDebugProc s_debugProc;
	return & s_debugProc;
}

CDebugProc::~CDebugProc()
{

}

bool CDebugProc::Initial()
{
	return false;
}

void CDebugProc::Quit()
{

}

//void CDebugProc::Runing(bool bRuning)
//{
//	m_bRuning = bRuning;
//}

bool CDebugProc::Start()
{
	m_debugProcID = PUB_CreateThread(DebugProcThread, this, &m_bDebugProc);
	if(m_debugProcID == PUB_CREATE_THREAD_FAIL)
	{
		PUB_PrintError(__FILE__,__LINE__);
		return false;
	}

	return true;
}

void CDebugProc::Stop()
{
	if (PUB_THREAD_ID_NOINIT != m_debugProcID)
	{		
		PUB_ExitThread(&m_debugProcID, &m_bDebugProc);
	}
}

void CDebugProc::SetRect(unsigned short width, unsigned short height)
{}

void CDebugProc::SetDebug(DEBUG_TYPE type, bool bOpen, unsigned long time/* = 0*/, unsigned long times/* = 0*/)
{
	if (DEBUG_LIVE == type)
	{
		m_infoLive.bOpen	= bOpen;
		m_infoLive.time		= time;
		m_infoLive.times	= times;
	}

	if (DEBUG_RECORD == type)
	{
		m_infoRecord.bOpen	= bOpen;
		m_infoRecord.time	= time;
		m_infoRecord.times	= times;
	}

	if (DEBUG_SEARCH == type)
	{
		m_infoSearch.bOpen	= bOpen;
		m_infoSearch.time	= time;
		m_infoSearch.times	= times;
	}

	if (DEBUG_PLAYBACK == type)
	{
		m_infoPlayback.bOpen	= bOpen;
		m_infoPlayback.time		= time;
		m_infoPlayback.times	= times;
	}

	if (DEBUG_INFORMATION == type)
	{
		m_infoInformation.bOpen	= bOpen;
		m_infoInformation.time	= time;
		m_infoInformation.times	= times;
	}
}
//////////////////////////////////////////////////////////////////////////
CDebugProc::CDebugProc() : m_width(1280), m_height(1024), m_debugProcID(PUB_THREAD_ID_NOINIT), m_bDebugProc(false), m_bRuning(false)
{
#if !defined(__CHIP3520__) && !defined(__CHIP3515__) && !defined(__CHIP3531__) && !defined(__CHIP3535__) && !defined(__CHIP3521__) && !defined(__CHIP3520A__) && !defined(__CHIP3520D__) && !defined(__CHIPGM__) 
	m_width	= 800;
	m_height = 600;
#endif

	memset(&m_infoLive, 0, sizeof(DEBUG_INFO));
	memset(&m_infoRecord, 0, sizeof(DEBUG_INFO));
	memset(&m_infoSearch, 0, sizeof(DEBUG_INFO));
	memset(&m_infoPlayback, 0, sizeof(DEBUG_INFO));
	memset(&m_infoInformation, 0, sizeof(DEBUG_INFO));
}

PUB_THREAD_RESULT PUB_THREAD_CALL CDebugProc::DebugProcThread(void *pParam)
{
#ifdef __ENVIRONMENT_LINUX__
	pid_t pid = getpid();
	pid_t tid = syscall(__NR_gettid);
	printf("%s, %d, pid = %d, tid=%d\n", __FILE__, __LINE__, pid, tid);
#endif

	CDebugProc *pThis = reinterpret_cast<CDebugProc *>(pParam);
	pThis->DebugProc();

	return 0;
}

int CDebugProc::DebugProc()
{
	KEY_VALUE_INFO keyValue;
	keyValue.type	= KEY_TYPE_MOUSE;
	keyValue.isDown	= true;
	keyValue.value	= E_MOUSE_KEY_LEFT_DOWN;

	while (m_bDebugProc)
	{
		LiveProc(keyValue);

		PUB_Sleep(10);
	}

	return 0;
}

void CDebugProc::LiveProc(KEY_VALUE_INFO & keyValue)
{
	if (m_infoLive.bOpen && m_bRuning)
	{
		for (unsigned long i=0; i<m_infoLive.times; ++i)
		{
			keyValue.xPos	= (rand() % m_width);
			keyValue.yPos	= (rand() % m_height);

			if (m_bRuning)
			{
				GUI::CDialog::AddKeyValue(keyValue);
			}
			else
			{
				break;
			}

			PUB_Sleep(5000);
		}
	}
}

void CDebugProc::RecordProc(KEY_VALUE_INFO & keyValue)
{
	if (m_infoRecord.bOpen && m_bRuning)
	{

	}
}

void CDebugProc::SearchProc(KEY_VALUE_INFO & keyValue)
{
	if (m_infoSearch.bOpen && m_bRuning)
	{

	}
}

void CDebugProc::PlabybackProc(KEY_VALUE_INFO & keyValue)
{
	if (m_infoPlayback.bOpen && m_bRuning)
	{

	}
}

void CDebugProc::InfoProc(KEY_VALUE_INFO & keyValue)
{
	if (m_infoInformation.bOpen && m_bRuning)
	{

	}
}
//end
