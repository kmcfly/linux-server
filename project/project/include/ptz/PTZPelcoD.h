

#ifndef PTZPELCOD_H
#define PTZPELCOD_H



#include "PTZProtocol.h"
#undef  COMMANDLEN
#define COMMANDLEN 7

class CPTZPelcoD : public CPTZProtocol  
{
public:

	CPTZPelcoD();
	virtual ~CPTZPelcoD();
	void AutoScan(int nGroupID);

	bool ZoomIn();
	bool ZoomOut();
	bool Near();
	bool Far();
	bool IrisClose();
	bool IrisOpen();
	bool Up();
	bool Down();
	bool Right();
	bool Stop();
	BYTE m_ucCommand[COMMANDLEN];
	BYTE GetCheckSum();
	bool Left();

	//hby �����ĸ�����
	bool LeftDown();
	bool LeftUp();
	bool RightDown();
	bool RightUp();
	
	//hby ʹ��PELCO_D �Լ���Ѳ������
//	bool Command(BYTE type,BYTE cmd,BYTE ptzaddr,int baudrate,BYTE speed,BYTE waitTime);

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

	//���Ƹ�������
	bool SetAuxiliary(BYTE auxNum);
	bool ClearAuxiliary(BYTE auxNum);

private:
	BYTE GetTiltSpeed();
	BYTE GetPanSpeed();
};

#endif

