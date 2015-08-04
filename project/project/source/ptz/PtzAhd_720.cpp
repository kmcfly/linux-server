/*
* Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
* Author       : xianshiwei
* Date         : 2015-05-27
* Name         : PtzAhd_720.cpp
* Version      : 1.0
* Description  : 

* Modify Record:

*/
#include "PtzAhd_720.h"
#include "LocalDevice.h"

CPtzAhd720::CPtzAhd720()
{
	m_chnn=0;
}

CPtzAhd720::CPtzAhd720(unsigned long chnn)
{
	printf("CPtzAhd720 chnn:%d\n",chnn);
	m_chnn = (int)chnn;
}


CPtzAhd720::~CPtzAhd720()
{
	
}

bool CPtzAhd720::SetCurChnn(unsigned long chnn)
{
	m_chnn = chnn;
}

unsigned long CPtzAhd720::GetCurChnn()
{
	return m_chnn;
}


bool CPtzAhd720::Stop()
{
	//m_ucCommand[0] = 0xB5;
	//m_ucCommand[1] = m_lDevAddress;
	//m_ucCommand[2] = 0x14;
	//m_ucCommand[3] = 0;
	////m_ucCommand[4] = 0;
	////m_ucCommand[5] = 0;
	////m_ucCommand[6] = 0;
	////m_ucCommand[7] = GetCheckSum();
	//return CLocalDevice::Instance()->SetTviPtzDeviceCtrl(m_chnn,m_ucCommand,COMMANDLEN_AHD);
	return true;
}

bool CPtzAhd720::Left()
{
	m_ucCommand[0] = 0x00;
	m_ucCommand[1] = 0x20;
	m_ucCommand[2] = 0x20;
	m_ucCommand[3] = 0x00;
	/*m_ucCommand[4] = 0;
	m_ucCommand[5] = 0;
	m_ucCommand[6] = 0;
	m_ucCommand[7] = GetCheckSum();*/

	return CLocalDevice::Instance()->SetTviPtzDeviceCtrl(m_chnn,m_ucCommand,COMMANDLEN_AHD);
}

bool CPtzAhd720::Right()
{
	m_ucCommand[0] = 0x00;
	m_ucCommand[1] = 0x40;
	m_ucCommand[2] = 0x40;
	m_ucCommand[3] = 0x00;
	/*m_ucCommand[4] = 0;
	m_ucCommand[5] = 0;
	m_ucCommand[6] = 0;
	m_ucCommand[7] = GetCheckSum();*/

	return CLocalDevice::Instance()->SetTviPtzDeviceCtrl(m_chnn,m_ucCommand,COMMANDLEN_AHD);	
}

bool CPtzAhd720::Up()
{
	m_ucCommand[0] = 0x00;
	m_ucCommand[1] = 0x10;
	m_ucCommand[2] = 0x10;
	m_ucCommand[3] = 0x4C;
	/*m_ucCommand[4] = 0;
	m_ucCommand[5] = 0;
	m_ucCommand[6] = 0;
	m_ucCommand[7] = GetCheckSum();*/

	return CLocalDevice::Instance()->SetTviPtzDeviceCtrl(m_chnn,m_ucCommand,COMMANDLEN_AHD);
}

bool CPtzAhd720::Down()
{
	m_ucCommand[0] = 0x00;
	m_ucCommand[1] = 0x08;
	m_ucCommand[2] = 0x08;
	m_ucCommand[3] = 0x4C;
	/*m_ucCommand[4] = 0;
	m_ucCommand[5] = 0;
	m_ucCommand[6] = 0;
	m_ucCommand[7] = GetCheckSum();*/

	return CLocalDevice::Instance()->SetTviPtzDeviceCtrl(m_chnn,m_ucCommand,COMMANDLEN_AHD);
}

bool CPtzAhd720::Near()
{
	m_ucCommand[0] = 0x80;
	m_ucCommand[1] = 0x00;
	m_ucCommand[2] = 0x00;
	m_ucCommand[3] = 0x00;
	/*m_ucCommand[4] = 0;
	m_ucCommand[5] = 0;
	m_ucCommand[6] = 0;
	m_ucCommand[7] = GetCheckSum();*/

	return CLocalDevice::Instance()->SetTviPtzDeviceCtrl(m_chnn,m_ucCommand,COMMANDLEN_AHD);
}

bool CPtzAhd720::Far()
{
	m_ucCommand[0] = 0x00;
	m_ucCommand[1] = 0x01;
	m_ucCommand[2] = 0x01;
	m_ucCommand[3] = 0x00;
	/*m_ucCommand[4] = 0;
	m_ucCommand[5] = 0;
	m_ucCommand[6] = 0;
	m_ucCommand[7] = GetCheckSum();*/

	return CLocalDevice::Instance()->SetTviPtzDeviceCtrl(m_chnn,m_ucCommand,COMMANDLEN_AHD);
}

bool CPtzAhd720::ZoomOut()
{
	m_ucCommand[0] = 0x00;
	m_ucCommand[1] = 0x02;
	m_ucCommand[2] = 0x02;
	m_ucCommand[3] = 0x00;
	/*m_ucCommand[4] = 0;
	m_ucCommand[5] = 0;
	m_ucCommand[6] = 0;
	m_ucCommand[7] = GetCheckSum();*/

	return CLocalDevice::Instance()->SetTviPtzDeviceCtrl(m_chnn,m_ucCommand,COMMANDLEN_AHD);
}

bool CPtzAhd720::ZoomIn()
{
	m_ucCommand[0] = 0x00;
	m_ucCommand[1] = 0x04;
	m_ucCommand[2] = 0x04;
	m_ucCommand[3] = 0x00;	
	/*m_ucCommand[4] = 0;
	m_ucCommand[5] = 0;
	m_ucCommand[6] = 0;
	m_ucCommand[7] = GetCheckSum();*/

	return CLocalDevice::Instance()->SetTviPtzDeviceCtrl(m_chnn,m_ucCommand,COMMANDLEN_AHD);
}

bool CPtzAhd720::IrisOpen()
{
	m_ucCommand[0] = 0x40;
	m_ucCommand[1] = 0x00;
	m_ucCommand[2] = 0x00;
	m_ucCommand[3] = 0x00;
	/*m_ucCommand[4] = 0;
	m_ucCommand[5] = 0;
	m_ucCommand[6] = 0;
	m_ucCommand[7] = GetCheckSum();*/

	return CLocalDevice::Instance()->SetTviPtzDeviceCtrl(m_chnn,m_ucCommand,COMMANDLEN_AHD);
}

bool CPtzAhd720::IrisClose()
{
	m_ucCommand[0] = 0x20;
	m_ucCommand[1] = 0x00;
	m_ucCommand[2] = 0x00;
	m_ucCommand[3] = 0x00;
	/*m_ucCommand[4] = 0;
	m_ucCommand[5] = 0;
	m_ucCommand[6] = 0;
	m_ucCommand[7] = GetCheckSum();*/

	return CLocalDevice::Instance()->SetTviPtzDeviceCtrl(m_chnn,m_ucCommand,COMMANDLEN_AHD);
}

/*
 * hby �����ĸ�����
 */
bool CPtzAhd720::LeftDown()
{
	/*m_ucCommand[0] = 0xB5;
	m_ucCommand[1] = m_lDevAddress;
	m_ucCommand[2] = 0x0B;
	m_ucCommand[3] = GetTiltSpeed();
	m_ucCommand[4] = GetPanSpeed();
	m_ucCommand[5] = 0;
	m_ucCommand[6] = 0;
	m_ucCommand[7] = GetCheckSum();
	return CLocalDevice::Instance()->SetTviPtzDeviceCtrl(m_chnn,m_ucCommand,COMMANDLEN_AHD);*/
	return true;
}
bool CPtzAhd720::LeftUp()
{
	//m_ucCommand[0] = 0xB5;
	//m_ucCommand[1] = m_lDevAddress;
	//m_ucCommand[2] = 0x0A;
	//m_ucCommand[3] = GetTiltSpeed();
	///*m_ucCommand[4] = GetPanSpeed();
	//m_ucCommand[5] = 0;
	//m_ucCommand[6] = 0;
	//m_ucCommand[7] = GetCheckSum();*/
	//return CLocalDevice::Instance()->SetTviPtzDeviceCtrl(m_chnn,m_ucCommand,COMMANDLEN_AHD);
	return true;
}
bool CPtzAhd720::RightDown()
{
	//m_ucCommand[0] = 0xB5;
	//m_ucCommand[1] = m_lDevAddress;
	//m_ucCommand[2] = 0x0D;
	//m_ucCommand[3] = GetTiltSpeed();
	///*m_ucCommand[4] = GetPanSpeed();
	//m_ucCommand[5] = 0;
	//m_ucCommand[6] = 0;
	//m_ucCommand[7] = GetCheckSum();*/
	//return CLocalDevice::Instance()->SetTviPtzDeviceCtrl(m_chnn,m_ucCommand,COMMANDLEN_AHD);
	return true;
}
bool CPtzAhd720::RightUp()
{
	//m_ucCommand[0] = 0xB5;
	//m_ucCommand[1] = m_lDevAddress;
	//m_ucCommand[2] = 0x0C;
	//m_ucCommand[3] = GetTiltSpeed();
	///*m_ucCommand[4] = GetPanSpeed();
	//m_ucCommand[5] = 0;
	//m_ucCommand[6] = 0;
	//m_ucCommand[7] = GetCheckSum();*/
	//return CLocalDevice::Instance()->SetTviPtzDeviceCtrl(m_chnn,m_ucCommand,COMMANDLEN_AHD);
	return true;
}

bool CPtzAhd720::PresetGo(BYTE ucPresetID)
{
	if(0x5E != ucPresetID)
	{
		return false;
	}
	m_ucCommand[0] = 0x40;
	m_ucCommand[1] = 0x00;
	m_ucCommand[2] = 0x00;
	m_ucCommand[3] = 0x00;
	printf("the preset value 720P \n");
	/*m_ucCommand[4] = 0;
	m_ucCommand[5] = 0;
	m_ucCommand[6] = 0;
	m_ucCommand[7] = GetCheckSum();*/
	return CLocalDevice::Instance()->SetTviPtzDeviceCtrl(m_chnn,m_ucCommand,COMMANDLEN_AHD);
}

bool CPtzAhd720::PresetSet(BYTE ucPresetID)
{
	//m_ucCommand[0] = 0xB5;
	//m_ucCommand[1] = m_lDevAddress;
	//m_ucCommand[2] = 0x15;
	//m_ucCommand[3] = ucPresetID+1;
	///*m_ucCommand[4] = 0;
	//m_ucCommand[5] = 0;
	//m_ucCommand[6] = 0;
	//m_ucCommand[7] = GetCheckSum();*/
	//return CLocalDevice::Instance()->SetTviPtzDeviceCtrl(m_chnn,m_ucCommand,COMMANDLEN_AHD);
	return true;
}

bool CPtzAhd720::PresetDelete(BYTE ucPresetID)
{
	//m_ucCommand[0] = 0xB5;
	//m_ucCommand[1] = m_lDevAddress;
	//m_ucCommand[2] = 0x16;
	//m_ucCommand[3] = ucPresetID+1;
	///*m_ucCommand[4] = 0;
	//m_ucCommand[5] = 0;
	//m_ucCommand[6] = 0;
	//m_ucCommand[7] = GetCheckSum();*/
	//return CLocalDevice::Instance()->SetTviPtzDeviceCtrl(m_chnn,m_ucCommand,COMMANDLEN_AHD);
	return true;
}


bool CPtzAhd720::MotorStop()
{
	//m_ucCommand[0] = 0xB5;
	//m_ucCommand[1] = m_lDevAddress;
	//m_ucCommand[2] = 0x14;
	//m_ucCommand[3] = 0;
	///*m_ucCommand[4] = 0;
	//m_ucCommand[5] = 0;
	//m_ucCommand[6] = 0;
	//m_ucCommand[7] = GetCheckSum();*/
	//return CLocalDevice::Instance()->SetTviPtzDeviceCtrl(m_chnn,m_ucCommand,COMMANDLEN_AHD);
	return true;
}

bool CPtzAhd720::SetCameraStatus(BYTE camStatus,BYTE camVersion)
{
	//m_ucCommand[0] = 0xB5;
	//m_ucCommand[1] = m_lDevAddress;
	//m_ucCommand[2] = 0x87;
	//m_ucCommand[3] = 0;
	///*m_ucCommand[4] = 0;
	//m_ucCommand[5] = camStatus;
	//m_ucCommand[6] = camVersion;
	//m_ucCommand[7] = GetCheckSum();*/
	//return CLocalDevice::Instance()->SetTviPtzDeviceCtrl(m_chnn,m_ucCommand,COMMANDLEN_AHD);
	return true;
}

bool CPtzAhd720::GetCameraStatus(BYTE* pCamStatus)
{
	//BYTE pData[256] = {0};
	//int len = 0;
	//CLocalDevice::Instance()->GetTviPtzDeviceCtrl(m_chnn,(BYTE*)pData,&len);
	//assert(len==COMMANDLEN_AHD);
	//memcpy(m_ucCommand,pData,COMMANDLEN_AHD);
	//
	//assert(m_ucCommand[0]==0xB5);
	//assert(m_ucCommand[1]==m_lDevAddress);
	//assert(m_ucCommand[2]==0x88);
	//assert(m_ucCommand[3]==0);
	//assert(m_ucCommand[4]==0);
	//assert(m_ucCommand[5]==0);
	////assert(p_data[6]==0xB5);
	//*pCamStatus = m_ucCommand[6];
	//assert(m_ucCommand[7]==GetCheckSum());

	return true;
}

BYTE CPtzAhd720::GetCheckSum()
{
	/*BYTE CheckSum=0;
	for(int i=0;i<7;i++)
	{
		CheckSum += m_ucCommand[i];
	}
	return CheckSum;*/
	return 0;
}

BYTE CPtzAhd720::GetPanSpeed()
{
	return m_Speed*8-1;//����Ϊ0-63��������Ϊ7-63(����)
}

BYTE CPtzAhd720::GetTiltSpeed()
{
	return m_Speed*8-1;//����Ϊ0-63��������Ϊ7-63(����)
}


bool CPtzAhd720::StopCruise()
{
	return true;
// 	printf("ptz tvi 24\n");
// 	//A0 01 00 51 00 00 AF 5F 
// 	m_ucCommand[0] = 0xB5;
// 	m_ucCommand[1] = m_lDevAddress;
// 	m_ucCommand[2] = 0;
// 	m_ucCommand[3] = 0x51;
// 	m_ucCommand[4] = 0;
// 	m_ucCommand[5] = 0;
// 	m_ucCommand[6] = 0xAF;
// 	m_ucCommand[7] = GetCheckSum();
// 	return CLocalDevice::Instance()->SetTviPtzDeviceCtrl(m_chnn,m_ucCommand,COMMANDLEN);
}

//TVI ͬ�����Э����ʱ��֧�����¹��ܡ�
#if 0

bool CPtzAhd720::RunCruise(BYTE uCruiseId)
{
	printf("ptz tvi 23\n");
	//A0 01 00 4F 01 00 AF 40 
	//A0 01 00 4F 01 01 AF 41 
	//A0 01 00 4F 01 02 AF 42 
	m_ucCommand[0] = 0xB5;
	m_ucCommand[1] = m_lDevAddress;
	m_ucCommand[2] = 0;
	m_ucCommand[3] = 0x4F;
	m_ucCommand[4] = 1;
	m_ucCommand[5] = uCruiseId;
	m_ucCommand[6] = 0xAF;
	m_ucCommand[7] = GetCheckSum();
	return CLocalDevice::Instance()->SetTviPtzDeviceCtrl(m_chnn,m_ucCommand,COMMANDLEN);
}

bool CPtzAhd720::StopCruise()
{
	printf("ptz tvi 24\n");
	//A0 01 00 51 00 00 AF 5F 
	m_ucCommand[0] = 0xB5;
	m_ucCommand[1] = m_lDevAddress;
	m_ucCommand[2] = 0;
	m_ucCommand[3] = 0x51;
	m_ucCommand[4] = 0;
	m_ucCommand[5] = 0;
	m_ucCommand[6] = 0xAF;
	m_ucCommand[7] = GetCheckSum();
	return CLocalDevice::Instance()->SetTviPtzDeviceCtrl(m_chnn,m_ucCommand,COMMANDLEN);
}

bool CPtzAhd720::DeleteCruise(BYTE uCruiseId)
{
	printf("ptz tvi 25\n");
	//A0 01 00 4D 00 00 AF 43 
	//A0 01 00 4D 00 01 AF 42 
	m_ucCommand[0] = 0xB5;
	m_ucCommand[1] = m_lDevAddress;
	m_ucCommand[2] = 0;
	m_ucCommand[3] = 0x4D;
	m_ucCommand[4] = 0;
	m_ucCommand[5] = uCruiseId;
	m_ucCommand[6] = 0xAF;
	m_ucCommand[7] = GetCheckSum();
	return CLocalDevice::Instance()->SetTviPtzDeviceCtrl(m_chnn,m_ucCommand,COMMANDLEN);
}

bool CPtzAhd720::EnterCruiseMode(BYTE uCruiseId)
{
	printf("ptz tvi 26\n");
	//A0 01 00 47 00 00 AF 49 
	//A0 01 00 47 00 02 AF 4B
	m_ucCommand[0] = 0xB5;
	m_ucCommand[1] = m_lDevAddress;
	m_ucCommand[2] = 0;
	m_ucCommand[3] = 0x47;
	m_ucCommand[4] = 0;
	m_ucCommand[5] = uCruiseId;
	m_ucCommand[6] = 0xAF;
	m_ucCommand[7] = GetCheckSum();
	return CLocalDevice::Instance()->SetTviPtzDeviceCtrl(m_chnn,m_ucCommand,COMMANDLEN);
}

bool CPtzAhd720::ConfigCruise(BYTE ucPresetID, BYTE speed, BYTE waitTime)
{
	printf("ptz tvi 27\n");
	//A0 01 07 49 06 10 AF 56 
	//A0 01 10 49 00 20 AF 77 
	m_ucCommand[0] = 0xB5;
	m_ucCommand[1] = m_lDevAddress;
	if ( (0==ucPresetID) && (0==speed) && (0==waitTime)) 
	{
		m_ucCommand[2] = ucPresetID;
	}
	else
	{
		m_ucCommand[2] = ucPresetID+1;
	}

	m_ucCommand[3] = 0x49;
	m_ucCommand[4] = 9 - speed;
	m_ucCommand[5] = waitTime;
	m_ucCommand[6] = 0xAF;
	m_ucCommand[7] = GetCheckSum();
	return CLocalDevice::Instance()->SetTviPtzDeviceCtrl(m_chnn,m_ucCommand,COMMANDLEN);
}

bool CPtzAhd720::LeaveCruiseMode(BYTE ucPresetID)
{
	printf("ptz tvi 28\n");
	//A0 01 00 4B 00 00 AF 45
	m_ucCommand[0] = 0xB5;
	m_ucCommand[1] = m_lDevAddress;
	m_ucCommand[2] = 0;
	m_ucCommand[3] = 0x4B;
	m_ucCommand[4] = 0;
	m_ucCommand[5] = 0;
	m_ucCommand[6] = 0xAF;
	m_ucCommand[7] = GetCheckSum();
	return CLocalDevice::Instance()->SetTviPtzDeviceCtrl(m_chnn,m_ucCommand,COMMANDLEN);
}

bool CPtzAhd720::IsSupportCruise()
{
	printf("CPtzAhd720 support cruise\n");
	return true;
}

bool CPtzAhd720::Record_Track()
{
	printf("ptz tvi 29\n");
	//A0 01 00 4B 00 00 AF 45
	//printf("Here record the track,FILE:%s,LINE:%d\n",__FILE__,__LINE__);
	m_ucCommand[0] = 0xB5;
	m_ucCommand[1] = m_lDevAddress;
	m_ucCommand[2] = 0;
	m_ucCommand[3] = 0x1f;
	m_ucCommand[4] = 0;
	m_ucCommand[5] = 0;
	m_ucCommand[6] = 0xAF;
	m_ucCommand[7] = GetCheckSum();
	return CLocalDevice::Instance()->SetTviPtzDeviceCtrl(m_chnn,m_ucCommand,COMMANDLEN);


}

bool CPtzAhd720::Stop_Record_Track()
{
	printf("ptz tvi 30\n");
	//printf("Here stop the track,FILE:%s,LINE:%d\n",__FILE__,__LINE__);
	//A0 01 00 21 00 00 AF 45
	m_ucCommand[0] = 0xB5;
	m_ucCommand[1] = m_lDevAddress;
	m_ucCommand[2] = 0;
	m_ucCommand[3] = 0x21;
	m_ucCommand[4] = 0;
	m_ucCommand[5] = 0;
	m_ucCommand[6] = 0xAF;
	m_ucCommand[7] = GetCheckSum();
	return CLocalDevice::Instance()->SetTviPtzDeviceCtrl(m_chnn,m_ucCommand,COMMANDLEN);


}
bool CPtzAhd720::Start_Track()
{
	printf("ptz tvi 31\n");
	//A0 01 00 23 00 00 AF 45
	m_ucCommand[0] = 0xB5;
	m_ucCommand[1] = m_lDevAddress;
	m_ucCommand[2] = 0;
	m_ucCommand[3] = 0x23;
	m_ucCommand[4] = 0;
	m_ucCommand[5] = 0;
	m_ucCommand[6] = 0xAF;
	m_ucCommand[7] = GetCheckSum();
	return CLocalDevice::Instance()->SetTviPtzDeviceCtrl(m_chnn,m_ucCommand,COMMANDLEN);
	
}

bool CPtzAhd720::Stop_Track()
{
	printf("ptz tvi 32\n");
	m_ucCommand[0] = 0xB5;
	m_ucCommand[1] = m_lDevAddress;
	m_ucCommand[2] = 0;
	m_ucCommand[3] = 0x04;					//ʹ��������ֹͣtrack
	m_ucCommand[4] = 0;
	m_ucCommand[5] = 0;
	m_ucCommand[6] = 0xAF;
	m_ucCommand[7] = GetCheckSum();	
	return CLocalDevice::Instance()->SetTviPtzDeviceCtrl(m_chnn,m_ucCommand,COMMANDLEN);;
}


bool CPtzAhd720::Auto_Scan()
{
	printf("ptz tvi 33\n");
	bool retval;
	
	//A0 01 00 07 00 97 AF 45   ��ʼ�ź�(����AO)����������ַ������byte1,����byte2,����byte1������byte2�����������־��У���ֽ�
	//�Զ�ɨ�費������ʱ������Ԥ�õ�99��ʵ���Զ�ɨ�裬����Ԥ�õ�96����ʵ��ֹͣɨ�裬PELCO-P��PELCO-DЭ��
	//99 Begin Auto-scan
	//98 Begin Frame Scan
	//97 Begin Random Scan
	//96 stop auto-scan
		m_ucCommand[0] = 0xB5;
		m_ucCommand[1] = m_lDevAddress;
		m_ucCommand[2] = 0;
		m_ucCommand[3] = 0x07;
		m_ucCommand[4] = 0;
		m_ucCommand[5] = 97;
		//printf("the preset value %d\n", (int)m_ucCommand[5]);
	
		m_ucCommand[6] = 0xAF;
		m_ucCommand[7] = GetCheckSum();
		retval = CLocalDevice::Instance()->SetTviPtzDeviceCtrl(m_chnn,m_ucCommand,COMMANDLEN);
		//printf("Running= %d, FILE:%s,LINE:%d\n", Running, __FILE__, __LINE__);
		return retval;
}

bool CPtzAhd720::Stop_Auto_Scan()
{
		printf("ptz tvi 34\n");
		bool retval;
	//printf("Here start the auto scan function,FILE:%s, LINE:%d\n", __FILE__, __LINE__);
	//A0 01 00 0x07 00 0x60 AF 45   ��ʼ�ź�(����AO)����������ַ������byte1,����byte2,����byte1������byte2�����������־��У���ֽ�
	//�Զ�ɨ�費������ʱ������Ԥ�õ�99��ʵ���Զ�ɨ�裬����Ԥ�õ�96����ʵ��ֹͣɨ�裬PELCO-P��PELCO-DЭ��

		m_ucCommand[0] = 0xB5;
		m_ucCommand[1] = m_lDevAddress;
		m_ucCommand[2] = 0;
		m_ucCommand[3] = 0x07;
		m_ucCommand[4] = 0;
		m_ucCommand[5] = 96;
		//printf("the preset value %d\n", (int)m_ucCommand[5]);
		m_ucCommand[6] = 0xAF;
		m_ucCommand[7] = GetCheckSum();
		retval = CLocalDevice::Instance()->SetTviPtzDeviceCtrl(m_chnn,m_ucCommand,COMMANDLEN);

		return retval;
}



bool CPtzAhd720::SetAuxiliary(BYTE auxNum)
{
	printf("ptz tvi 35\n");
	if ((AUXILIARY_START >= auxNum) && (AUXILIARY_END <= auxNum))
	{
		assert(false);
		return false;
	}

	m_ucCommand[0] = 0xB5;
	m_ucCommand[1] = m_lDevAddress;
	m_ucCommand[2] = 0;
	m_ucCommand[3] = 0x09;
	m_ucCommand[4] = 0;
	m_ucCommand[5] = auxNum;
	m_ucCommand[6] = 0xAF;
	m_ucCommand[7] = GetCheckSum();
	return CLocalDevice::Instance()->SetTviPtzDeviceCtrl(m_chnn,m_ucCommand,COMMANDLEN);
}

bool CPtzAhd720::ClearAuxiliary(BYTE auxNum)
{
	printf("ptz tvi 36\n");
	if ((AUXILIARY_START >= auxNum) && (AUXILIARY_END <= auxNum))
	{
		assert(false);
		return false;
	}

	m_ucCommand[0] = 0xB5;
	m_ucCommand[1] = m_lDevAddress;
	m_ucCommand[2] = 0;
	m_ucCommand[3] = 0x0B;
	m_ucCommand[4] = 0;
	m_ucCommand[5] = auxNum;
	m_ucCommand[6] = 0xAF;
	m_ucCommand[7] = GetCheckSum();
	return CLocalDevice::Instance()->SetTviPtzDeviceCtrl(m_chnn,m_ucCommand,COMMANDLEN);
}


bool CPtzAhd720::MenuOn()
{
	m_ucCommand[0] = 0xB5;
	m_ucCommand[1] = m_lDevAddress;
	m_ucCommand[2] = 0x17;
	m_ucCommand[3] = 0x5f;
	m_ucCommand[4] = 0;
	m_ucCommand[5] = 0;
	m_ucCommand[6] = 0;
	m_ucCommand[7] = GetCheckSum();
	return CLocalDevice::Instance()->SetTviPtzDeviceCtrl(m_chnn,m_ucCommand,COMMANDLEN);
}

bool CPtzAhd720::MenuLeft()
{
	m_ucCommand[0] = 0xB5;
	m_ucCommand[1] = m_lDevAddress;
	m_ucCommand[2] = 0x09;
	m_ucCommand[3] = 0;
	m_ucCommand[4] = 0;
	m_ucCommand[5] = 0;
	m_ucCommand[6] = 0;
	m_ucCommand[7] = GetCheckSum();
	return CLocalDevice::Instance()->SetTviPtzDeviceCtrl(m_chnn,m_ucCommand,COMMANDLEN);
}

bool CPtzAhd720::MenuRight()
{
	m_ucCommand[0] = 0xB5;
	m_ucCommand[1] = m_lDevAddress;
	m_ucCommand[2] = 0x08;
	m_ucCommand[3] = 0;
	m_ucCommand[4] = 0;
	m_ucCommand[5] = 0;
	m_ucCommand[6] = 0;
	m_ucCommand[7] = GetCheckSum();
	return CLocalDevice::Instance()->SetTviPtzDeviceCtrl(m_chnn,m_ucCommand,COMMANDLEN);
	
}

bool CPtzAhd720::MenuUp()
{
	m_ucCommand[0] = 0xB5;
	m_ucCommand[1] = m_lDevAddress;
	m_ucCommand[2] = 0x06;
	m_ucCommand[3] = GetTiltSpeed();
	m_ucCommand[4] = 0;
	m_ucCommand[5] = 0;
	m_ucCommand[6] = 0;
	m_ucCommand[7] = GetCheckSum();
	return CLocalDevice::Instance()->SetTviPtzDeviceCtrl(m_chnn,m_ucCommand,COMMANDLEN);
}

bool CPtzAhd720::MenuDown()
{
	m_ucCommand[0] = 0xB5;
	m_ucCommand[1] = m_lDevAddress;
	m_ucCommand[2] = 0x07;
	m_ucCommand[3] = GetTiltSpeed();
	m_ucCommand[4] = 0;
	m_ucCommand[5] = 0;
	m_ucCommand[6] = 0;
	m_ucCommand[7] = GetCheckSum();
	return CLocalDevice::Instance()->SetTviPtzDeviceCtrl(m_chnn,m_ucCommand,COMMANDLEN);
}
#endif

