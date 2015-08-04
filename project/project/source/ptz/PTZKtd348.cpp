#include "PTZKtd348.h"

#ifdef __DEBUG_NEW__
#include "debug_new.h"
#define new DEBUG_NEW
#endif


CPTZKtd348::CPTZKtd348()
{	
	memset(m_ucCommand, 0, sizeof(m_ucCommand));
	for (int i = 0; i < 6; ++i)
	{
		m_ucCommand[i*3] = 0x80;
	}
	m_ucCommand[10] = 0x18;

	m_lock.Lock();
	m_bSendCmd = false;
	m_lock.UnLock();
	
	m_bWriteProc = false;

	m_thread = PUB_CreateThread(WriteCmdThread, (void*)this, &m_bWriteProc);
	if (PUB_CREATE_THREAD_FAIL == m_thread)
	{
		printf("%s:%s:%d, create thread error!\n", __FUNCTION__, __FILE__,__LINE__);
	}
}

CPTZKtd348::~CPTZKtd348()
{
	if ( PUB_THREAD_ID_NOINIT != m_thread )
	{		
		PUB_ExitThread(&m_thread, &m_bWriteProc);
		printf("%s thread exit!m_bWriteProc=[%d]\n", __FUNCTION__, m_bWriteProc);
	}
}

PUB_THREAD_RESULT PUB_THREAD_CALL CPTZKtd348::WriteCmdThread(void * pParam)
{
#ifndef	WIN32
	pid_t pid = getpid();
	pid_t tid = syscall(__NR_gettid);
	printf("%s, %d, Ptz ktd348 pid = %d, tid=%d\n", __FILE__, __LINE__, pid, tid);
#endif

	CPTZKtd348 * pPTZKtd348 = reinterpret_cast<CPTZKtd348 *>(pParam);
	pPTZKtd348->WriteCmdProc();

	return 0;
}

void CPTZKtd348::WriteCmdProc()
{
	ULONGLONG lastSendCmdTime = GetCurrTime();
	bool bPause = true;
	
	while (m_bWriteProc)
	{
		m_lock.Lock();
		bPause = m_bSendCmd;
		m_lock.UnLock();

		if (bPause)
		{
			if ((GetCurrTime() - lastSendCmdTime) >= 12000)
			{
				lastSendCmdTime = GetCurrTime();
				(CSerialPort::Instance())->WriteToPort(m_ucCommand, COMMANDLEN, m_Baudrate);
			}
			else
			{
				PUB_Sleep(1);
			}
		}
		else
		{
			PUB_Sleep(50);
			continue;
		}
	}
}

bool CPTZKtd348::Left()
{
	m_lock.Lock();
	m_ucCommand[8]  = m_lDevAddress;
	m_ucCommand[9]  = 0x91;
	m_bSendCmd = true;
	m_lock.UnLock();

	return true;
}

bool CPTZKtd348::Stop()
{
	m_lock.Lock();
	m_bSendCmd = false;
	m_lock.UnLock();

	return true;
}

bool CPTZKtd348::Right()
{
	m_lock.Lock();
	m_ucCommand[8]  = m_lDevAddress;
	m_ucCommand[9]  = 0x81;
	m_bSendCmd = true;
	m_lock.UnLock();

	return true;
}


bool CPTZKtd348::Down()
{
	m_lock.Lock();
	m_ucCommand[8]  = m_lDevAddress;
	m_ucCommand[9]  = 0x8A;
	m_bSendCmd = true;
	m_lock.UnLock();

	return true;
}


bool CPTZKtd348::Up()
{
	m_lock.Lock();
	m_ucCommand[8]  = m_lDevAddress;
	m_ucCommand[9]  = 0x88;
	m_bSendCmd = true;
	m_lock.UnLock();

	return true;
}

bool CPTZKtd348::IrisOpen()
{
	m_lock.Lock();
	m_ucCommand[8]  = m_lDevAddress;
	m_ucCommand[9]  = 0xAA;
	m_bSendCmd = true;
	m_lock.UnLock();

	return true;
}


bool CPTZKtd348::IrisClose()
{
	m_lock.Lock();
	m_ucCommand[8]  = m_lDevAddress;
	m_ucCommand[9]  = 0xA2;
	m_bSendCmd = true;
	m_lock.UnLock();

	return true;
}

bool CPTZKtd348::Far()
{
	m_lock.Lock();
	m_ucCommand[8]  = m_lDevAddress;
	m_ucCommand[9]  = 0xA1;
	m_bSendCmd = true;
	m_lock.UnLock();

	return true;
}

bool CPTZKtd348::Near()
{
	m_lock.Lock();
	m_ucCommand[8]  = m_lDevAddress;
	m_ucCommand[9]  = 0xA9;
	m_bSendCmd = true;
	m_lock.UnLock();

	return true;
}

bool CPTZKtd348::ZoomOut()
{
	m_lock.Lock();
	m_ucCommand[8]  = m_lDevAddress;
	m_ucCommand[9]  = 0xA8;
	m_bSendCmd = true;
	m_lock.UnLock();

	return true;
}


bool CPTZKtd348::ZoomIn()
{
	m_lock.Lock();
	m_ucCommand[8]  = m_lDevAddress;
	m_ucCommand[9]  = 0xA0;
	m_bSendCmd = true;
	m_lock.UnLock();
	return true;
}

/*
unsigned char CPTZKtd348::0
{
	return m_Speed*8-1;//本来为0-63,这个结果为7-63(够用)
}

unsigned char CPTZKtd348::0
{
	return m_Speed*8-1;//本来为0-63,这个结果为7-63(够用)
}
*/
