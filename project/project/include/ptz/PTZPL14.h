#ifndef _PTZPL14_H_
#define _PTZPL14_H_

#include "PTZProtocol.h"
#undef  COMMANDLEN
#define COMMANDLEN 10
#define PL14_OPEN_MENU_PRESET_NUM 95
#define PL14_CLOSE_MENU_PRESET_NUM 96

class CPTZPL14 : public CPTZProtocol  
{
public:

	CPTZPL14();
	virtual ~CPTZPL14();
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
	BYTE GetCheckSum();
	BYTE GetSpeed();
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
	virtual bool PresetClear(BYTE ucPresetID);

	//zhoufeng 2007-12-11 ���Ѳ������
	bool EnterCruiseMode(BYTE uCruiseId);
	bool ConfigCruise(BYTE ucPresetID, BYTE speed, BYTE waitTime);
	bool LeaveCruiseMode(BYTE uCruiseId);
	bool RunCruise(BYTE uCruiseId);		//����Ѳ��
	bool StopCruise();					//ֹͣ����Ѳ��

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
	//������̨�Դ�ϵͳ�˵�
	bool PopPTZMenu();
	bool ClosePTZMenu();
protected:
	BYTE m_ucCommand[COMMANDLEN];

	
};


#endif

