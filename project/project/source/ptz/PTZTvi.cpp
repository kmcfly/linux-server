
#include "PTZTvi.h"
#include "LocalDevice.h"

CPTZTvi::CPTZTvi()
{
	m_Baudrate = SBR_9600; //TVI同轴控制下此参数无用
	m_chnn=0;
}

CPTZTvi::CPTZTvi(unsigned long chnn)
{
	printf("CPTZTvi chnn:%d\n",chnn);
	m_Baudrate = SBR_9600; //TVI同轴控制下此参数无用
	m_chnn = (int)chnn;
}


CPTZTvi::~CPTZTvi()
{
	
}

bool CPTZTvi::SetCurChnn(unsigned long chnn)
{
	m_chnn = chnn;
}

unsigned long CPTZTvi::GetCurChnn()
{
	return m_chnn;
}


bool CPTZTvi::Stop()
{
	m_ucCommand[0] = 0xB5;
	m_ucCommand[1] = m_lDevAddress;
	m_ucCommand[2] = 0x14;
	m_ucCommand[3] = 0;
	m_ucCommand[4] = 0;
	m_ucCommand[5] = 0;
	m_ucCommand[6] = 0;
	m_ucCommand[7] = GetCheckSum();
	return CLocalDevice::Instance()->SetTviPtzDeviceCtrl(m_chnn,m_ucCommand,COMMANDLEN);
}

bool CPTZTvi::Left()
{
	m_ucCommand[0] = 0xB5;
	m_ucCommand[1] = m_lDevAddress;
	m_ucCommand[2] = 0x09;
	m_ucCommand[3] = 0;
	m_ucCommand[4] = GetPanSpeed();
	m_ucCommand[5] = 0;
	m_ucCommand[6] = 0;
	m_ucCommand[7] = GetCheckSum();
	return CLocalDevice::Instance()->SetTviPtzDeviceCtrl(m_chnn,m_ucCommand,COMMANDLEN);
}

bool CPTZTvi::Right()
{
	m_ucCommand[0] = 0xB5;
	m_ucCommand[1] = m_lDevAddress;
	m_ucCommand[2] = 0x08;
	m_ucCommand[3] = 0;
	m_ucCommand[4] = GetPanSpeed();
	m_ucCommand[5] = 0;
	m_ucCommand[6] = 0;
	m_ucCommand[7] = GetCheckSum();
	return CLocalDevice::Instance()->SetTviPtzDeviceCtrl(m_chnn,m_ucCommand,COMMANDLEN);
	
}

bool CPTZTvi::Up()
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

bool CPTZTvi::Down()
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

bool CPTZTvi::Near()
{
	m_ucCommand[0] = 0xB5;
	m_ucCommand[1] = m_lDevAddress;
	m_ucCommand[2] = 0x10;
	m_ucCommand[3] = 0;
	m_ucCommand[4] = 0;
	m_ucCommand[5] = 0;
	m_ucCommand[6] = 0;
	m_ucCommand[7] = GetCheckSum();
	return CLocalDevice::Instance()->SetTviPtzDeviceCtrl(m_chnn,m_ucCommand,COMMANDLEN);
}

bool CPTZTvi::Far()
{
	m_ucCommand[0] = 0xB5;
	m_ucCommand[1] = m_lDevAddress;
	m_ucCommand[2] = 0x11;
	m_ucCommand[3] = 0;
	m_ucCommand[4] = 0;
	m_ucCommand[5] = 0;
	m_ucCommand[6] = 0;
	m_ucCommand[7] = GetCheckSum();
	return CLocalDevice::Instance()->SetTviPtzDeviceCtrl(m_chnn,m_ucCommand,COMMANDLEN);
}

bool CPTZTvi::ZoomOut()
{
	m_ucCommand[0] = 0xB5;
	m_ucCommand[1] = m_lDevAddress;
	m_ucCommand[2] = 0x13;
	m_ucCommand[3] = 0;
	m_ucCommand[4] = 0;
	m_ucCommand[5] = 0;
	m_ucCommand[6] = 0;
	m_ucCommand[7] = GetCheckSum();
	return CLocalDevice::Instance()->SetTviPtzDeviceCtrl(m_chnn,m_ucCommand,COMMANDLEN);
}

bool CPTZTvi::ZoomIn()
{
	m_ucCommand[0] = 0xB5;
	m_ucCommand[1] = m_lDevAddress;
	m_ucCommand[2] = 0x12;
	m_ucCommand[3] = 0;	
	m_ucCommand[4] = 0;
	m_ucCommand[5] = 0;
	m_ucCommand[6] = 0;
	m_ucCommand[7] = GetCheckSum();
	
	return CLocalDevice::Instance()->SetTviPtzDeviceCtrl(m_chnn,m_ucCommand,COMMANDLEN);
}

bool CPTZTvi::IrisOpen()
{
	m_ucCommand[0] = 0xB5;
	m_ucCommand[1] = m_lDevAddress;
	m_ucCommand[2] = 0x0F;
	m_ucCommand[3] = 0;
	m_ucCommand[4] = 0;
	m_ucCommand[5] = 0;
	m_ucCommand[6] = 0;
	m_ucCommand[7] = GetCheckSum();
	return CLocalDevice::Instance()->SetTviPtzDeviceCtrl(m_chnn,m_ucCommand,COMMANDLEN);
}

bool CPTZTvi::IrisClose()
{
	m_ucCommand[0] = 0xB5;
	m_ucCommand[1] = m_lDevAddress;
	m_ucCommand[2] = 0x0E;
	m_ucCommand[3] = 0;
	m_ucCommand[4] = 0;
	m_ucCommand[5] = 0;
	m_ucCommand[6] = 0;
	m_ucCommand[7] = GetCheckSum();
	return CLocalDevice::Instance()->SetTviPtzDeviceCtrl(m_chnn,m_ucCommand,COMMANDLEN);
}

/*
 * hby 增加四个方向
 */
bool CPTZTvi::LeftDown()
{
	m_ucCommand[0] = 0xB5;
	m_ucCommand[1] = m_lDevAddress;
	m_ucCommand[2] = 0x0B;
	m_ucCommand[3] = GetTiltSpeed();
	m_ucCommand[4] = GetPanSpeed();
	m_ucCommand[5] = 0;
	m_ucCommand[6] = 0;
	m_ucCommand[7] = GetCheckSum();
	return CLocalDevice::Instance()->SetTviPtzDeviceCtrl(m_chnn,m_ucCommand,COMMANDLEN);
}
bool CPTZTvi::LeftUp()
{
	m_ucCommand[0] = 0xB5;
	m_ucCommand[1] = m_lDevAddress;
	m_ucCommand[2] = 0x0A;
	m_ucCommand[3] = GetTiltSpeed();
	m_ucCommand[4] = GetPanSpeed();
	m_ucCommand[5] = 0;
	m_ucCommand[6] = 0;
	m_ucCommand[7] = GetCheckSum();
	return CLocalDevice::Instance()->SetTviPtzDeviceCtrl(m_chnn,m_ucCommand,COMMANDLEN);
}
bool CPTZTvi::RightDown()
{
	m_ucCommand[0] = 0xB5;
	m_ucCommand[1] = m_lDevAddress;
	m_ucCommand[2] = 0x0D;
	m_ucCommand[3] = GetTiltSpeed();
	m_ucCommand[4] = GetPanSpeed();
	m_ucCommand[5] = 0;
	m_ucCommand[6] = 0;
	m_ucCommand[7] = GetCheckSum();
	return CLocalDevice::Instance()->SetTviPtzDeviceCtrl(m_chnn,m_ucCommand,COMMANDLEN);
}
bool CPTZTvi::RightUp()
{
	m_ucCommand[0] = 0xB5;
	m_ucCommand[1] = m_lDevAddress;
	m_ucCommand[2] = 0x0C;
	m_ucCommand[3] = GetTiltSpeed();
	m_ucCommand[4] = GetPanSpeed();
	m_ucCommand[5] = 0;
	m_ucCommand[6] = 0;
	m_ucCommand[7] = GetCheckSum();
	return CLocalDevice::Instance()->SetTviPtzDeviceCtrl(m_chnn,m_ucCommand,COMMANDLEN);
}

bool CPTZTvi::PresetGo(BYTE ucPresetID)
{
	m_ucCommand[0] = 0xB5;
	m_ucCommand[1] = m_lDevAddress;
	m_ucCommand[2] = 0x17;
	m_ucCommand[3] = ucPresetID+1;
	//printf("the preset value:%d\n", (int)m_ucCommand[3]);
	m_ucCommand[4] = 0;
	m_ucCommand[5] = 0;
	m_ucCommand[6] = 0;
	m_ucCommand[7] = GetCheckSum();
	return CLocalDevice::Instance()->SetTviPtzDeviceCtrl(m_chnn,m_ucCommand,COMMANDLEN);
}

bool CPTZTvi::PresetSet(BYTE ucPresetID)
{
	m_ucCommand[0] = 0xB5;
	m_ucCommand[1] = m_lDevAddress;
	m_ucCommand[2] = 0x15;
	m_ucCommand[3] = ucPresetID+1;
	m_ucCommand[4] = 0;
	m_ucCommand[5] = 0;
	m_ucCommand[6] = 0;
	m_ucCommand[7] = GetCheckSum();
	return CLocalDevice::Instance()->SetTviPtzDeviceCtrl(m_chnn,m_ucCommand,COMMANDLEN);
}

bool CPTZTvi::PresetDelete(BYTE ucPresetID)
{
	m_ucCommand[0] = 0xB5;
	m_ucCommand[1] = m_lDevAddress;
	m_ucCommand[2] = 0x16;
	m_ucCommand[3] = ucPresetID+1;
	m_ucCommand[4] = 0;
	m_ucCommand[5] = 0;
	m_ucCommand[6] = 0;
	m_ucCommand[7] = GetCheckSum();
	return CLocalDevice::Instance()->SetTviPtzDeviceCtrl(m_chnn,m_ucCommand,COMMANDLEN);
}


bool CPTZTvi::MotorStop()
{
	m_ucCommand[0] = 0xB5;
	m_ucCommand[1] = m_lDevAddress;
	m_ucCommand[2] = 0x14;
	m_ucCommand[3] = 0;
	m_ucCommand[4] = 0;
	m_ucCommand[5] = 0;
	m_ucCommand[6] = 0;
	m_ucCommand[7] = GetCheckSum();
	return CLocalDevice::Instance()->SetTviPtzDeviceCtrl(m_chnn,m_ucCommand,COMMANDLEN);
}

bool CPTZTvi::SetCameraStatus(BYTE camStatus,BYTE camVersion)
{
	m_ucCommand[0] = 0xB5;
	m_ucCommand[1] = m_lDevAddress;
	m_ucCommand[2] = 0x87;
	m_ucCommand[3] = 0;
	m_ucCommand[4] = 0;
	m_ucCommand[5] = camStatus;
	m_ucCommand[6] = camVersion;
	m_ucCommand[7] = GetCheckSum();
	return CLocalDevice::Instance()->SetTviPtzDeviceCtrl(m_chnn,m_ucCommand,COMMANDLEN);
}

bool CPTZTvi::GetCameraStatus(BYTE* pCamStatus)
{
	BYTE pData[256] = {0};
	int len = 0;
	CLocalDevice::Instance()->GetTviPtzDeviceCtrl(m_chnn,(BYTE*)pData,&len);
	assert(len==COMMANDLEN);
	memcpy(m_ucCommand,pData,COMMANDLEN);
	
	assert(m_ucCommand[0]==0xB5);
	assert(m_ucCommand[1]==m_lDevAddress);
	assert(m_ucCommand[2]==0x88);
	assert(m_ucCommand[3]==0);
	assert(m_ucCommand[4]==0);
	assert(m_ucCommand[5]==0);
	//assert(p_data[6]==0xB5);
	*pCamStatus = m_ucCommand[6];
	assert(m_ucCommand[7]==GetCheckSum());

	return true;
}

BYTE CPTZTvi::GetCheckSum()
{
	BYTE CheckSum=0;
	for(int i=0;i<7;i++)
	{
		CheckSum += m_ucCommand[i];
	}
	return CheckSum;
}

BYTE CPTZTvi::GetPanSpeed()
{
	return m_Speed*8-1;//本来为0-63，这个结果为7-63(够用)
}

BYTE CPTZTvi::GetTiltSpeed()
{
	return m_Speed*8-1;//本来为0-63，这个结果为7-63(够用)
}

bool CPTZTvi::StopCruise()
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

//TVI 同轴控制协议暂时不支持以下功能。
#if 0
bool CPTZTvi::RunCruise(BYTE uCruiseId)
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

bool CPTZTvi::StopCruise()
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

bool CPTZTvi::DeleteCruise(BYTE uCruiseId)
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

bool CPTZTvi::EnterCruiseMode(BYTE uCruiseId)
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

bool CPTZTvi::ConfigCruise(BYTE ucPresetID, BYTE speed, BYTE waitTime)
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

bool CPTZTvi::LeaveCruiseMode(BYTE ucPresetID)
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

bool CPTZTvi::IsSupportCruise()
{
	printf("CPTZTvi support cruise\n");
	return true;
}

bool CPTZTvi::Record_Track()
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

bool CPTZTvi::Stop_Record_Track()
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
bool CPTZTvi::Start_Track()
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

bool CPTZTvi::Stop_Track()
{
	printf("ptz tvi 32\n");
	m_ucCommand[0] = 0xB5;
	m_ucCommand[1] = m_lDevAddress;
	m_ucCommand[2] = 0;
	m_ucCommand[3] = 0x04;					//使用向左来停止track
	m_ucCommand[4] = 0;
	m_ucCommand[5] = 0;
	m_ucCommand[6] = 0xAF;
	m_ucCommand[7] = GetCheckSum();	
	return CLocalDevice::Instance()->SetTviPtzDeviceCtrl(m_chnn,m_ucCommand,COMMANDLEN);;
}


bool CPTZTvi::Auto_Scan()
{
	printf("ptz tvi 33\n");
	bool retval;
	
	//A0 01 00 07 00 97 AF 45   开始信号(都是AO)，解码器地址，命令byte1,命令byte2,数据byte1，数据byte2，结束传输标志，校验字节
	//自动扫描不可以用时，调用预置点99能实现自动扫描，调用预置点96可以实现停止扫描，PELCO-P和PELCO-D协议
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

bool CPTZTvi::Stop_Auto_Scan()
{
		printf("ptz tvi 34\n");
		bool retval;
	//printf("Here start the auto scan function,FILE:%s, LINE:%d\n", __FILE__, __LINE__);
	//A0 01 00 0x07 00 0x60 AF 45   开始信号(都是AO)，解码器地址，命令byte1,命令byte2,数据byte1，数据byte2，结束传输标志，校验字节
	//自动扫描不可以用时，调用预置点99能实现自动扫描，调用预置点96可以实现停止扫描，PELCO-P和PELCO-D协议

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



bool CPTZTvi::SetAuxiliary(BYTE auxNum)
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

bool CPTZTvi::ClearAuxiliary(BYTE auxNum)
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
#endif

bool CPTZTvi::MenuOn()
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

bool CPTZTvi::MenuLeft()
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

bool CPTZTvi::MenuRight()
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

bool CPTZTvi::MenuUp()
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

bool CPTZTvi::MenuDown()
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


