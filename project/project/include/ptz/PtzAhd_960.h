/*
* Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
* Author       : xianshiwei
* Date         : 2015-05-27
* Name         : PtzAhd_960.h
* Version      : 1.0
* Description  : 

* Modify Record:

*/
#ifndef __PTZ_AHD_960___
#define __PTZ_AHD_960___

#include "PtzAhd.h"

class CPtzAhd960
	:public CPtzAhd{

public:
	CPtzAhd960();
	CPtzAhd960(unsigned long chnn);
	virtual ~CPtzAhd960();

	bool SetCurChnn(unsigned long chnn);
	unsigned long GetCurChnn();

	BYTE m_ucCommand[COMMANDLEN_AHD];
	int  m_chnn;
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

	bool PresetDelete(BYTE ucPresetID);

	//��ȡ����̨Э���ַ��Χ <1>
	virtual void GetDevAddrRange(int* min, int *max){*min = 0x00, *max = 0x1F;}

	virtual bool PresetGo(BYTE ucPresetID);
	virtual bool PresetSet(BYTE ucPresetID);


	bool MotorStop();
	bool SetCameraStatus(BYTE camStatus,BYTE camVersion);
	bool GetCameraStatus(BYTE* pCamStatus);

	bool StopCruise();					//ֹͣ����Ѳ��
#if 0	
	bool RunCruise(BYTE uCruiseId);		//����Ѳ��
	bool StopCruise();					//ֹͣ����Ѳ��
	bool DeleteCruise(BYTE uCruiseId);  //ɾ��Ѳ���˹켣��
	bool EnterCruiseMode(BYTE uCruiseId);							//����Ѳ������ģʽ
	bool ConfigCruise(BYTE ucPresetID, BYTE speed, BYTE waitTime);	//����Ѳ���ٶ�
	bool LeaveCruiseMode(BYTE ucPresetID);							//�˳�Ѳ������ģʽ

	bool Record_Track();
	bool Stop_Record_Track();
	bool Start_Track();
	bool Stop_Track();
	bool Auto_Scan();
	bool Stop_Auto_Scan();

	//��ѯ�Ƿ�֧��Ѳ��
	bool IsSupportCruise();

	bool SetAuxiliary(BYTE auxNum);
	bool ClearAuxiliary(BYTE auxNum);


	//OSD�˵�����
	bool MenuOn();
	bool MenuLeft();
	bool MenuRight();
	bool MenuUp();
	bool MenuDown();
#endif
private:
	BYTE GetTiltSpeed();
	BYTE GetPanSpeed();

};

#endif