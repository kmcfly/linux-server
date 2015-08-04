#include "IPCDecDisplayMan.h"
#include "IPDataBufferDefine.h"
#include "LocalDevice.h"

extern bool g_bLogin;
extern NET_LOGIN_SUCCESS_REPLY *g_p_login_succ;
CIPCDecDisplayMan::CIPCDecDisplayMan(void)
{
	m_bDisplaying			= false;
	m_displayCH				= 0;
	m_pLocalDevice			= CLocalDevice::Instance();
}

CIPCDecDisplayMan::~CIPCDecDisplayMan(void)
{
}

CIPCDecDisplayMan *CIPCDecDisplayMan::Instance()
{
	static CIPCDecDisplayMan ipcDecDisplay;
	return &ipcDecDisplay;
}

bool CIPCDecDisplayMan::Start()
{
	m_lock.Lock();
	m_bDisplaying = true;
	m_lock.UnLock();

	printf("%s, %s, %d\n\n", __FUNCTION__, __FILE__, __LINE__);

	return true;
}

void CIPCDecDisplayMan::Stop()
{
	m_lock.Lock();
	m_bDisplaying = false;

	if((m_displayCH != 0) || (m_displayCHMinor != 0))
	{
		printf("IPCStop : displayCH = %x, displayCHMinor = %x, %s, %d\n", (unsigned long)m_displayCH, (unsigned long)m_displayCHMinor, __FILE__, __LINE__);
		m_pLocalDevice->IPCStop();
		m_displayCH = 0;
		m_displayCHMinor = 0;
	}
	m_lock.UnLock();
}

void CIPCDecDisplayMan::SetDisplayCH(ULONGLONG dislayCH, ULONGLONG displayCHMinor)
{
	if(g_bLogin)
	{
		dislayCH &= (g_p_login_succ->authLiveCH >> g_p_login_succ->productInfo.localVideoInputNum);
		displayCHMinor &= (g_p_login_succ->authLiveCH >> g_p_login_succ->productInfo.localVideoInputNum);
	}
	if((dislayCH == m_displayCH) && (displayCHMinor == m_displayCHMinor))
	{
		return;
	}

	m_lock.Lock();

	if((m_displayCH != 0) || (m_displayCHMinor != 0))
	{
		printf("IPCStop : displayCH = %x, displayCHMinor = %x, %s, %d\n", (unsigned long)m_displayCH, (unsigned long)m_displayCHMinor, __FILE__, __LINE__);
		m_pLocalDevice->IPCStop();
	}

	m_displayCH = dislayCH;
	m_displayCHMinor = displayCHMinor;
	if((m_displayCH != 0) || (m_displayCHMinor != 0))
	{
		unsigned int chipId[EXT_DVR_MAX_CHANNEL];
		printf("IPCStart : displayCH = %x, displayCHMinor = %x, %s, %d\n", (unsigned long)m_displayCH, (unsigned long)m_displayCHMinor, __FILE__, __LINE__);
		m_pLocalDevice->IPCStart(m_displayCH, m_displayCHMinor, chipId);
	}

	m_lock.UnLock();
}

void CIPCDecDisplayMan::GetDisplayCH(ULONGLONG &displayCH, ULONGLONG &displayCHMinor)
{
	m_lock.Lock();
	displayCH = m_displayCH;
	displayCHMinor = m_displayCHMinor;
	m_lock.UnLock();
}

bool CIPCDecDisplayMan::SetDataBuffer(ext_dvr_ip_in_buffer_param *pIPBufferParam)
{
	bool bRet = true;

	m_lock.Lock();
	if(((m_displayCH | m_displayCHMinor) != 0) && m_bDisplaying)
	{
		bRet = m_pLocalDevice->IPCSetBufferData(pIPBufferParam);
	}
	else
	{
		*pIPBufferParam->pStatus = IP_BUFFER_FREE;
	}
	m_lock.UnLock();

	return bRet;
}

bool CIPCDecDisplayMan::SetMinorDataBuffer(ext_dvr_ip_in_buffer_param *pIPBufferParam)
{
	bool bRet = true;

	m_lock.Lock();
	if((m_displayCHMinor != 0) && m_bDisplaying)
	{
		bRet = m_pLocalDevice->IPCMinorSetBufferData(pIPBufferParam);
	}
	else
	{
		*pIPBufferParam->pStatus = IP_BUFFER_FREE;
	}
	m_lock.UnLock();

	return bRet;
}

