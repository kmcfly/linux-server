
#ifndef PTZPELCOP_H
#define PTZPELCOP_H

#include "PTZProtocol.h"
#undef	COMMANDLEN
#define COMMANDLEN 8

class CPTZPelcoP : public CPTZProtocol
{
public:
	bool PresetPrepare();
	CPTZPelcoP();
	virtual ~CPTZPelcoP();

	BYTE m_ucCommand[COMMANDLEN];
	BYTE GetCheckSum();
	
	bool Stop();
	bool Left();
	bool Right();
	bool Up();
	bool Down();
	bool Near();
	bool Far();
	bool ZoomOut();
	bool ZoomIn();
	bool IrisOpen();
	bool IrisClose();

//hby �����ĸ�����
	bool LeftDown();
	bool LeftUp();
	bool RightDown();
	bool RightUp();

	//��ȡ����̨Э���ַ��Χ <1>
	virtual void GetDevAddrRange(int* min, int *max){*min = 0x00, *max = 0x1F;}

	virtual bool PresetGo(BYTE ucPresetID);
	virtual bool PresetSet(BYTE ucPresetID);

	//zhoufeng 2007-12-11 ���Ѳ������
	bool RunCruise(BYTE uCruiseId);		//����Ѳ��
	bool StopCruise();					//ֹͣ����Ѳ��
	bool DeleteCruise(BYTE uCruiseId);  //ɾ��Ѳ���˹켣��
	bool EnterCruiseMode(BYTE uCruiseId);							//����Ѳ������ģʽ
	bool ConfigCruise(BYTE ucPresetID, BYTE speed, BYTE waitTime);	//����Ѳ���ٶ�
	bool LeaveCruiseMode(BYTE ucPresetID);							//�˳�Ѳ������ģʽ
//zhl113 2009-07-08�����̨�켣���Զ�Ѳ������
	bool Record_Track();
	bool Stop_Record_Track();
	bool Start_Track();
	bool Stop_Track();
//zhl-113 2009--7-9 ����Զ�ɨ�蹦��
	bool Auto_Scan();
	bool Stop_Auto_Scan();

	//��ѯ�Ƿ�֧��Ѳ��
	bool IsSupportCruise();

	bool SetAuxiliary(BYTE auxNum);
	bool ClearAuxiliary(BYTE auxNum);

protected:
	
private:
	BYTE GetTiltSpeed();
	BYTE GetPanSpeed();
};

#endif

