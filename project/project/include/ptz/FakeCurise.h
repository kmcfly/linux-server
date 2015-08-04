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
	unsigned long		curSetCurise;			//��ǰ���õ����õ�
	unsigned long		curSetPoint;			//��ǰҪ���õ����õ�

	unsigned long		curRunCurise;			//��ǰ���е�Ѳ����
	unsigned long		curRunPoint;			//��ǰ���е����õ�

	unsigned long		nextRunPointTime;		//��һ��Ԥ�õ����е�ʱ��
	PTZ_CURISE_POINT	chnnCurisePoint[MAX_CRUISE_NUM][MAX_CRUISE_PRESET_NUM];	//��ͨ����Ѳ������Ϣ
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
			//���Ϊ��ʼѲ��
			m_curRunCuriseChnn |= (0x01 << channel);
		}
		else
		{
			//���ΪֹͣѲ��
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


	unsigned long		m_channelNum;							//��¼ͨ������
	PUB_thread_t		m_fakeCuriseProcID;
	bool				m_bFakeCuriseProc;
	
	CPUB_Lock			m_lock;
	unsigned long		m_ISFakeCurise[64];						//��¼����ͨ���Ƿ�����Ѳ��
	ULONGLONG			m_curRunCuriseChnn;						//��ǰ����Ѳ���ߵ�ͨ��

	CHNN_CURISE_INFO	m_chnnCuriseInfo[64];					//ÿ��ͨ��8��Ѳ���ߣ�ÿ��Ѳ����16�����õ�

	PTZ_SERIAL_INFO		m_ptzSerialInfo[64];					//��ǰͨ���Ĵ���������Ϣ				
	CPTZProtocol		*m_pPtzProtocl[64];
};
#endif

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////


