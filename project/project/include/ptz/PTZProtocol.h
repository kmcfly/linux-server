#ifndef PTZPROTOCOL_H
#define PTZPROTOCOL_H

//#include "Config.h"
#include "PTZ.h"
#include "Typedef.h"
#include "dvrdvsdef.h"
#include "PUB_common.h"
#include "SerialPort.h"
//#include "..\RECORD\avitypes.h"	// Added by ClassView
class CPTZProtocol  
{
public:

	CPTZProtocol();
	virtual ~CPTZProtocol();

	/////////////
	virtual bool Command(BYTE cmd,BYTE ptzaddr,int baudrate,BYTE speed, unsigned long ptzPram, BYTE waitTime = 0);
	bool SetCurisePoint(BYTE ptzaddr,int baudrate, BYTE uCruiseId,PTZ_CURISE_POINT *pCurisePoint,BYTE curisePointNum);
	void GetDevAddrRange(int &min,int &max){min = 0;max = 255;}

	bool WriteBuffer(const BYTE* pBuffer, int len);
	
	//��ѯ�Ƿ�֧��Ѳ��
	virtual bool IsSupportCruise();
protected:
	virtual bool Left()	= 0;
	virtual bool Right()	= 0;
	virtual bool Up()	= 0;
	virtual bool Down()	= 0;
	virtual bool LeftDown() = 0;
	virtual bool LeftUp() = 0;
	virtual bool RightDown() = 0;
	virtual bool RightUp() = 0;

	virtual bool Near()	= 0;
	virtual bool Far()	= 0;
	virtual bool ZoomOut()	= 0;
	virtual bool ZoomIn()	= 0;
	virtual bool IrisOpen()	= 0;
	virtual bool IrisClose()= 0;

	virtual bool Stop() = 0;

#if 1
	virtual bool PresetGo(BYTE ucPresetID) = 0;		//��ĳ��Ԥ�õ�
	virtual bool PresetSet(BYTE ucPresetID) = 0;	//��Ŀǰλ������ΪucPresetID��Ԥ�õ�
	virtual bool PresetClear(BYTE ucPresetID);  //���ĳ��Ԥ�õ㡣
#endif
	//zhoufeng 2007-12-11 ���Ѳ������
	virtual bool RunCruise(BYTE uCruiseId);		//����Ѳ��
	virtual bool StopCruise();					//ֹͣ����Ѳ��
	virtual bool DeleteCruise(BYTE uCruiseId);		//ɾ��Ѳ���˹켣��
	virtual bool EnterCruiseMode(BYTE uCruiseId);							//����Ѳ������ģʽ
	virtual bool ConfigCruise(BYTE ucPresetID, BYTE speed, BYTE waitTime);	//����Ѳ���ٶ�
	virtual bool LeaveCruiseMode(BYTE uCruiseId);							//�˳�Ѳ������ģʽ
	virtual bool ConfigureCruiseDisable();


	//zhl113  2009-07-07 �����̨�켣���Զ�ɨ�蹦��
	virtual bool Record_Track();
	virtual bool Stop_Record_Track();
	virtual bool Start_Track();
	virtual bool Stop_Track();

	//zhl-113 2009-07-9 ����Զ�ɨ�蹦��
	virtual bool Auto_Scan();
	virtual bool Stop_Auto_Scan();

	//��ӵ�����̨�Դ�ϵͳ�˵�����
	virtual bool PopPTZMenu();
	
	//���Ƹ������أ� ������������������ˢ���ƹ⡢������ͷ��������
	virtual bool SetAuxiliary(BYTE auxNum);
	virtual bool ClearAuxiliary(BYTE auxNum);

	PORT_BAUDRATE	m_Baudrate;//������
	long			m_Speed;
	DWORD			m_dwLastState;
	long			m_lDevAddress;
	
private:
	BYTE		m_LastCmd;
	LONGLONG	m_LastTime;
};

#endif

