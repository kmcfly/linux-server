//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef __FAKE_CURISE_H__
#define __FAKE_CURISE_H__

#include "PTZProtocol.h"
#include "Product.h"

const BYTE UNUSED_PRESET	= 0xff;
const BYTE MAX_DWELL_TIME	= 60  + 10;

class CFakeCurise
{
public:
	static CFakeCurise * Instance();
	~CFakeCurise();
	
	bool Initial(unsigned long channelNum);
	void Quit();

	bool Start();
	void Stop();

	bool PreAction(unsigned long channel, CPTZProtocol *pPTZProtocol, unsigned char cmdtype, unsigned long ptzParam);

	//
	bool InitChnnFakeCurise(unsigned long channel, const PTZ_CURISE* pCurise, const PTZ_CURISE_POINT* pCurisePt);
	
	void SetCuriseType(const unsigned long *pIsFakeCurise, unsigned long len);
	void SetSerialInfo(unsigned long channel, unsigned char enable, unsigned char addr, unsigned char protocol, unsigned char rate);

typedef struct _chnn_curise_info_
{
	unsigned long		curSetCurise;			//当前设置的与置点
	unsigned long		curSetPoint;			//当前要设置的与置点

	unsigned long		curRunCurise;			//当前运行的巡航线
	unsigned long		curRunPoint;			//当前运行的与置点

	unsigned long		nextRunPointTime;		//下一个预置点运行的时间
	PTZ_CURISE_POINT	chnnCurisePoint[MAX_CRUISE_NUM][MAX_CRUISE_PRESET_NUM];	//改通道的巡航线信息
}CHNN_CURISE_INFO;

private:
	CFakeCurise();
	static	PUB_THREAD_RESULT PUB_THREAD_CALL FakeCuriseThread(void *pParam);
	void FakeCuriseProc();

 	bool EnterFakeCuriseMode(unsigned long channel, BYTE uCruiseId);
 	bool FakeCuriseSet(unsigned long channel, BYTE ucPresetID, BYTE speed, BYTE waitTime);
	bool LeaveFakeCuriseMode(unsigned long channel);
	bool RunFakeCurise(unsigned long channel, BYTE uCruiseId);
	bool StopFakeCurise(unsigned long channel);
	bool DelFakeCruise(unsigned long channel, BYTE uCruiseId);

	unsigned long GetPreValidPresetPos(unsigned long channel, unsigned long curiseID, unsigned long presetPos);
	bool ChnnCuriseIsRunning(unsigned long channel)
	{
		bool bIsRunning = false;
		m_lock.Lock();
		if ((m_curRunCuriseChnn >> channel ) & 0x01)
		{
			bIsRunning = true;
		}
		else
		{
			bIsRunning = false;
		}
		m_lock.UnLock();

		return bIsRunning;
	}

	void SetChnnCuriseStatus(unsigned long channel, bool bIsRunning)
	{
		m_lock.Lock();
		if (bIsRunning)
		{
			//标记为开始巡航
			m_curRunCuriseChnn |= (0x01 << channel);
		}
		else
		{
			//标记为停止巡航
			m_curRunCuriseChnn &= (~(0x01 << channel));
		}
		m_lock.UnLock();
	}

	bool IsUnUsedPreset(BYTE presetNum)
	{
		if (UNUSED_PRESET == presetNum)
		{
			return true;
		} 
		else
		{
			return false;
		}
	}

	bool IsFakeCurise(unsigned long channel)
	{
		if (channel > m_channelNum)
		{
			assert(false);
			return false;
		}

		bool bFakeCurise = false;

		m_lock.Lock();
		if (0 != m_ISFakeCurise[channel])
		{
			bFakeCurise = true;
		}
		m_lock.UnLock();

		return bFakeCurise;
	}


	unsigned long		m_channelNum;							//记录通道个数
	PUB_thread_t		m_fakeCuriseProcID;
	bool				m_bFakeCuriseProc;
	
	CPUB_Lock			m_lock;
	unsigned long		m_ISFakeCurise[64];						//记录各个通道是否是真巡航
	ULONGLONG			m_curRunCuriseChnn;						//当前运行巡航线的通道

	CHNN_CURISE_INFO	m_chnnCuriseInfo[64];					//每个通道8条巡航线，每个巡航线16个与置点

	PTZ_SERIAL_INFO		m_ptzSerialInfo[64];					//当前通道的串口配置信息				
	CPTZProtocol		*m_pPtzProtocl[64];
};
#endif

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////


