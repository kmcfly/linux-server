
#ifndef PTZMINIKING_H
#define PTZMINIKING_H


#include "PTZProtocol.h"
#undef COMMANDLEN
#define COMMANDLEN 11

class CPTZMinking : public CPTZProtocol  
{
public:
	virtual bool PresetSet(BYTE ucPresetID);
	virtual bool PresetGo(BYTE ucPresetID);
	CPTZMinking();
	virtual ~CPTZMinking();

	bool Stop();
	
	bool IrisOpen();
	bool IrisClose();
	bool ZoomOut();
	bool ZoomIn();
	bool Far();
	bool Near();
	bool Down();
	bool Up();
	bool Right();
	bool Left();

	//hby �����ĸ�����
	bool LeftDown();
	bool LeftUp();
	bool RightDown();
	bool RightUp();
	
	BYTE GetCheckSum();
	BYTE ucCommand[COMMANDLEN];
	virtual void GetDevAddrRange(int* min, int *max){*min = 0x00, *max = 0x1F;}

	//zhoufeng 2007-12-11 ���Ѳ������
	bool RunCruise(BYTE uCruiseId);		//����Ѳ��
	bool StopCruise();					//ֹͣ����Ѳ��
	bool DeleteCruise(BYTE uCruiseId);  //ɾ��Ѳ���˹켣��
	bool EnterCruiseMode(BYTE uCruiseId);							//����Ѳ������ģʽ
	bool ConfigCruise(BYTE ucPresetID, BYTE speed, BYTE waitTime);	//����Ѳ���ٶ�
	bool LeaveCruiseMode(BYTE uCruiseId);							//�˳�Ѳ������ģʽ

	//��ѯ�Ƿ�֧��Ѳ��
	bool IsSupportCruise();

	//�������ؿ���
	bool SetAuxiliary(BYTE auxNum);
	bool ClearAuxiliary(BYTE auxNum);

private:
	BYTE GetTiltSpeed();
	BYTE GetPanSpeed();
	BYTE m_CurrentCruiseId;
};

#endif

