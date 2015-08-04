#ifndef _IPC_DECDISPLAY_MAN_H_
#define _IPC_DECDISPLAY_MAN_H_

#include "LocalDevice.h"
#include "PUB_common.h"

class CIPCDecDisplayMan
{
public:
	static CIPCDecDisplayMan *Instance();

	bool Start();
	void Stop();

	void SetDisplayCH(ULONGLONG displayCH, ULONGLONG displayCHMinor);
	void GetDisplayCH(ULONGLONG &displayCH, ULONGLONG &displayCHMinor);

	bool SetDataBuffer(ext_dvr_ip_in_buffer_param *pIPBufferParam);
	bool SetMinorDataBuffer(ext_dvr_ip_in_buffer_param *pIPBufferParam);

private:
	CIPCDecDisplayMan(void);
	~CIPCDecDisplayMan(void);

private:
	bool				m_bDisplaying;
	ULONGLONG			m_displayCH;
	ULONGLONG			m_displayCHMinor;

	CPUB_Lock			m_lock;
	CLocalDevice		* m_pLocalDevice;
};

#endif