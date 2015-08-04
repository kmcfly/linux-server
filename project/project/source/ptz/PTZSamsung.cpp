#include "PTZSamsung.h"

#ifdef	LJ_DEBUG
#include "user_config.h"
#endif

CPTZSamsung::CPTZSamsung()
{
	m_Baudrate = SBR_9600;
}

CPTZSamsung::~CPTZSamsung()
{

}

/*  
 *  命令格式:  共有9Byte, COMMANDLEN = 9
 */

/*hby
 * 这里的m_lDevAddress最少必须要为1,当m_lDevAddress=1的时候,解码器的地址拨到地址0
 * 这里的m_ucCommand[8]好像没有起到作用.随便什么值都有动作.
 * m_ucCommand[8] 就应该是校验的数据.
 * 校验的方式为:m_ucCommand[i](i= 1,2,3,4,5,6)相加的和 再和 校验位相加等到一个0x100的倍数.
 */

bool CPTZSamsung::Stop()
{
	m_ucCommand[0] = 0xA0;
	m_ucCommand[1] = m_lDevAddress;	
	m_ucCommand[2] = m_lDevAddress;
	m_ucCommand[3] = 0x01;
	m_ucCommand[4] = 0x00;
	m_ucCommand[5] = 0x00;
	m_ucCommand[6] = 0xFF;
	m_ucCommand[7] = 0xFF;
	m_ucCommand[8] = GetCheckSum();
	return (CSerialPort::Instance())->WriteToPort(m_ucCommand,COMMANDLEN,m_Baudrate);
}

bool CPTZSamsung::Left()
{
	m_ucCommand[0] = 0xA0;
	m_ucCommand[1] = m_lDevAddress;
	m_ucCommand[2] = m_lDevAddress;
	m_ucCommand[3] = 0x01;
	m_ucCommand[4] = 0x00;
	m_ucCommand[5] = 0x01;			//Left
	m_ucCommand[6] = GetPanSpeed();
	m_ucCommand[7] = 0xFF;
	m_ucCommand[8] = GetCheckSum();
	return (CSerialPort::Instance())->WriteToPort(m_ucCommand,COMMANDLEN,m_Baudrate);
}

bool CPTZSamsung::Right()
{
	m_ucCommand[0] = 0xA0;
	m_ucCommand[1] = m_lDevAddress;
	m_ucCommand[2] = m_lDevAddress;
	m_ucCommand[3] = 0x01;
	m_ucCommand[4] = 0x00;
	m_ucCommand[5] = 0x02;			//Right
	m_ucCommand[6] = GetPanSpeed();
	m_ucCommand[7] = 0xFF;
	m_ucCommand[8] = GetCheckSum();
	return (CSerialPort::Instance())->WriteToPort(m_ucCommand,COMMANDLEN,m_Baudrate);
}

bool CPTZSamsung::Up()
{
	m_ucCommand[0] = 0xA0;
	m_ucCommand[1] = m_lDevAddress;
	m_ucCommand[2] = m_lDevAddress;
	m_ucCommand[3] = 0x01;
	m_ucCommand[4] = 0x00;
	m_ucCommand[5] = 0x04;			//Up
	m_ucCommand[6] = 0xFF;
	m_ucCommand[7] = GetTiltSpeed();
	m_ucCommand[8] = GetCheckSum();
	return (CSerialPort::Instance())->WriteToPort(m_ucCommand,COMMANDLEN,m_Baudrate);
}

bool CPTZSamsung::Down()
{
	m_ucCommand[0] = 0xA0;
	m_ucCommand[1] = m_lDevAddress;
	m_ucCommand[2] = m_lDevAddress;
	m_ucCommand[3] = 0x01;
	m_ucCommand[4] = 0x00;
	m_ucCommand[5] = 0x08;			//Down
	m_ucCommand[6] = 0xFF;
	m_ucCommand[7] = GetTiltSpeed();
	m_ucCommand[8] = GetCheckSum();
	return (CSerialPort::Instance())->WriteToPort(m_ucCommand,COMMANDLEN,m_Baudrate);
}

/*
 *hby 
 */
bool CPTZSamsung::LeftDown()
{
	m_ucCommand[0] = 0xA0;
	m_ucCommand[1] = m_lDevAddress;
	m_ucCommand[2] = m_lDevAddress;
	m_ucCommand[3] = 0x01;
	m_ucCommand[4] = 0x00;
	m_ucCommand[5] = 0x09;			//Right
	m_ucCommand[6] = GetPanSpeed();
	m_ucCommand[7] = GetTiltSpeed();
	m_ucCommand[8] = GetCheckSum();
	return (CSerialPort::Instance())->WriteToPort(m_ucCommand,COMMANDLEN,m_Baudrate);
}

bool CPTZSamsung::LeftUp()
{
	m_ucCommand[0] = 0xA0;
	m_ucCommand[1] = m_lDevAddress;
	m_ucCommand[2] = m_lDevAddress;
	m_ucCommand[3] = 0x01;
	m_ucCommand[4] = 0x00;
	m_ucCommand[5] = 0x05;			//Right
	m_ucCommand[6] = GetPanSpeed();
	m_ucCommand[7] = GetTiltSpeed();
	m_ucCommand[8] = GetCheckSum();
	return (CSerialPort::Instance())->WriteToPort(m_ucCommand,COMMANDLEN,m_Baudrate);
}

bool CPTZSamsung::RightDown()
{
	m_ucCommand[0] = 0xA0;
	m_ucCommand[1] = m_lDevAddress;
	m_ucCommand[2] = m_lDevAddress;
	m_ucCommand[3] = 0x01;
	m_ucCommand[4] = 0x00;
	m_ucCommand[5] = 0x0A;			//Right
	m_ucCommand[6] = GetPanSpeed();
	m_ucCommand[7] = GetTiltSpeed();
	m_ucCommand[8] = GetCheckSum();
	return (CSerialPort::Instance())->WriteToPort(m_ucCommand,COMMANDLEN,m_Baudrate);
}

bool CPTZSamsung::RightUp()
{
	m_ucCommand[0] = 0xA0;
	m_ucCommand[1] = m_lDevAddress;
	m_ucCommand[2] = m_lDevAddress;
	m_ucCommand[3] = 0x01;
	m_ucCommand[4] = 0x00;
	m_ucCommand[5] = 0x06;			
	m_ucCommand[6] = GetPanSpeed();
	m_ucCommand[7] = GetTiltSpeed();
	m_ucCommand[8] = GetCheckSum();
	return (CSerialPort::Instance())->WriteToPort(m_ucCommand,COMMANDLEN,m_Baudrate);
}

bool CPTZSamsung::Near()		//聚焦近 ====> 焦距+
{
	m_ucCommand[0] = 0xA0;
	m_ucCommand[1] = m_lDevAddress;
	m_ucCommand[2] = m_lDevAddress;
	m_ucCommand[3] = 0x01;
	m_ucCommand[4] = 0x73;
	m_ucCommand[5] = 0x0F;			
	m_ucCommand[6] = 0xFF;
	m_ucCommand[7] = 0xFF;
	m_ucCommand[8] = GetCheckSum();
	return (CSerialPort::Instance())->WriteToPort(m_ucCommand,COMMANDLEN,m_Baudrate);
}

bool CPTZSamsung::Far()		//聚焦远 =====> 焦距-
{
	m_ucCommand[0] = 0xA0;
	m_ucCommand[1] = m_lDevAddress;
	m_ucCommand[2] = m_lDevAddress;
	m_ucCommand[3] = 0x01;
	m_ucCommand[4] = 0x6B;
	m_ucCommand[5] = 0x0F;			
	m_ucCommand[6] = 0xFF;
	m_ucCommand[7] = 0xFF;
	m_ucCommand[8] = GetCheckSum();
	return (CSerialPort::Instance())->WriteToPort(m_ucCommand,COMMANDLEN,m_Baudrate);
}

bool CPTZSamsung::ZoomOut()		//特写 ====> 缩放+
{
	m_ucCommand[0] = 0xA0;
	m_ucCommand[1] = m_lDevAddress;
	m_ucCommand[2] = m_lDevAddress;
	m_ucCommand[3] = 0x01;
	m_ucCommand[4] = 0x5B;
	m_ucCommand[5] = 0x0F;			
	m_ucCommand[6] = 0xFF;
	m_ucCommand[7] = 0xFF;
	m_ucCommand[8] = GetCheckSum();
	return (CSerialPort::Instance())->WriteToPort(m_ucCommand,COMMANDLEN,m_Baudrate);
}

bool CPTZSamsung::ZoomIn()		//广角 =====> 缩放-
{
	m_ucCommand[0] = 0xA0;
	m_ucCommand[1] = m_lDevAddress;
	m_ucCommand[2] = m_lDevAddress;
	m_ucCommand[3] = 0x01;
	m_ucCommand[4] = 0x3B;
	m_ucCommand[5] = 0x0F;			
	m_ucCommand[6] = 0xFF;
	m_ucCommand[7] = 0xFF;
	m_ucCommand[8] = GetCheckSum();
	return (CSerialPort::Instance())->WriteToPort(m_ucCommand,COMMANDLEN,m_Baudrate);
}

bool CPTZSamsung::IrisOpen()		//光圈放大
{
	m_ucCommand[0] = 0xA0;
	m_ucCommand[1] = m_lDevAddress;
	m_ucCommand[2] = m_lDevAddress;
	m_ucCommand[3] = 0x01;
	m_ucCommand[4] = 0x73;
	m_ucCommand[5] = 0x0F;			
	m_ucCommand[6] = 0xFF;
	m_ucCommand[7] = 0xFF;
	m_ucCommand[8] = GetCheckSum();
	return (CSerialPort::Instance())->WriteToPort(m_ucCommand,COMMANDLEN,m_Baudrate);
}

bool CPTZSamsung::IrisClose()		//光圈缩小
{
	m_ucCommand[0] = 0xA0;
	m_ucCommand[1] = m_lDevAddress;
	m_ucCommand[2] = m_lDevAddress;
	m_ucCommand[3] = 0x01;
	m_ucCommand[4] = 0x6B;
	m_ucCommand[5] = 0x0F;			
	m_ucCommand[6] = 0xFF;
	m_ucCommand[7] = 0xFF;
	m_ucCommand[8] = GetCheckSum();
	return (CSerialPort::Instance())->WriteToPort(m_ucCommand,COMMANDLEN,m_Baudrate);
}

bool CPTZSamsung::PresetGo(BYTE ucPresetID)
{
	m_ucCommand[0] = 0xA0;
	m_ucCommand[1] = m_lDevAddress;
	m_ucCommand[2] = m_lDevAddress;
	m_ucCommand[3] = 0x03;
	m_ucCommand[4] = 0x50;		
	m_ucCommand[5] = ucPresetID+1;		
	m_ucCommand[6] = 0xFF;
	m_ucCommand[7] = 0xFF;
	m_ucCommand[8] = GetCheckSum();
	return (CSerialPort::Instance())->WriteToPort(m_ucCommand,COMMANDLEN,m_Baudrate);
}

//这个函数不做要求,直接调用预置点96.
bool CPTZSamsung::PresetPrepare()
{
	m_ucCommand[0] = 0xA0;
	m_ucCommand[1] = m_lDevAddress;
	m_ucCommand[2] = m_lDevAddress;
	m_ucCommand[3] = 0x03;
	m_ucCommand[4] = 0x50;		
	m_ucCommand[5] = 0x60;			
	m_ucCommand[6] = 0xFF;
	m_ucCommand[7] = 0xFF;
	m_ucCommand[8] = GetCheckSum();
	return (CSerialPort::Instance())->WriteToPort(m_ucCommand,COMMANDLEN,m_Baudrate);
}

bool CPTZSamsung::PresetSet(BYTE ucPresetID)
{
	//A0H	地址码	00H	03H	00H	预置点号	AFH	校验码
	m_ucCommand[0] = 0xA0;
	m_ucCommand[1] = m_lDevAddress;
	m_ucCommand[2] = m_lDevAddress;
	m_ucCommand[3] = 0x03;
	m_ucCommand[4] = 0x19;		
	m_ucCommand[5] = ucPresetID+1;			
	m_ucCommand[6] = 0xFF;
	m_ucCommand[7] = 0xFF;
	m_ucCommand[8] = GetCheckSum();
	return (CSerialPort::Instance())->WriteToPort(m_ucCommand,COMMANDLEN,m_Baudrate);
}

BYTE CPTZSamsung::GetCheckSum()
{
	BYTE CheckSum=0;
	for(int i=1;i<8;i++)
	{
		CheckSum += m_ucCommand[i];
	}
	CheckSum = 0x100 - ((CheckSum + 0x01) % 0x100);
	return CheckSum;
}

BYTE CPTZSamsung::GetPanSpeed()
{
//	printf("GetPanSpeed = %d\n",m_Speed);
	return m_Speed*8-1;//本来为0-63，这个结果为7-63(够用)
}

BYTE CPTZSamsung::GetTiltSpeed()
{
//	printf("GetTiltSpeed = %d\n",m_Speed);
	return m_Speed*8-1;//本来为0-63，这个结果为7-63(够用)
}

//所有的巡航开始都是这个命令
bool CPTZSamsung::RunCruise(BYTE uCruiseId)		//A0 01 01 03 13 01 FF FF E8
{
	uCruiseId = 0x01;
	
	m_ucCommand[0] = 0xA0;
	m_ucCommand[1] = m_lDevAddress;
	m_ucCommand[2] = m_lDevAddress;
	m_ucCommand[3] = 0x03;
	m_ucCommand[4] = 0x13;
	m_ucCommand[5] = uCruiseId;			
	m_ucCommand[6] = 0xFF;
	m_ucCommand[7] = 0xFF;
	m_ucCommand[8] = GetCheckSum();//0xE1;//
	return (CSerialPort::Instance())->WriteToPort(m_ucCommand,COMMANDLEN,m_Baudrate);
}

//所有的停止巡航命令都是这个 
bool CPTZSamsung::StopCruise()		//A0 01 01 01 00 00 FF FF FE
{
	m_ucCommand[0] = 0xA0;
	m_ucCommand[1] = m_lDevAddress;
	m_ucCommand[2] = m_lDevAddress;
	m_ucCommand[3] = 0x01;
	m_ucCommand[4] = 0x00;		
	m_ucCommand[5] = 0x00;			
	m_ucCommand[6] = 0xFF;
	m_ucCommand[7] = 0xFF;
	m_ucCommand[8] = GetCheckSum();//0xE1;//
	return (CSerialPort::Instance())->WriteToPort(m_ucCommand,COMMANDLEN,m_Baudrate);
}

//Samsung巡航线命令只有这个
bool CPTZSamsung::DeleteCruise(BYTE uCruiseId)
{
	uCruiseId = uCruiseId;
	
	m_ucCommand[0] = 0xA0;
	m_ucCommand[1] = m_lDevAddress;
	m_ucCommand[2] = m_lDevAddress;
	m_ucCommand[3] = 0x03;
	m_ucCommand[4] = 0x13;		
	m_ucCommand[5] = 0x01;			
	m_ucCommand[6] = 0xFF;
	m_ucCommand[7] = 0xFF;
	m_ucCommand[8] = GetCheckSum();
	return (CSerialPort::Instance())->WriteToPort(m_ucCommand,COMMANDLEN,m_Baudrate);
}

bool CPTZSamsung::EnterCruiseMode(BYTE uCruiseId)
{
	//A0 01 00 47 00 00 AF 49 
	//A0 01 00 47 00 02 AF 4B
	m_ucCommand[0] = 0xA0;
	m_ucCommand[1] = m_lDevAddress;
	m_ucCommand[2] = 0;
	m_ucCommand[3] = 0x47;
	m_ucCommand[4] = 0;
	m_ucCommand[5] = uCruiseId;
	m_ucCommand[6] = 0xAF;
	m_ucCommand[7] = GetCheckSum();
	return (CSerialPort::Instance())->WriteToPort(m_ucCommand,COMMANDLEN,m_Baudrate);
}

bool CPTZSamsung::ConfigCruise(BYTE ucPresetID, BYTE speed, BYTE waitTime)
{
	//A0 01 07 49 06 10 AF 56 
	//A0 01 10 49 00 20 AF 77 
	m_ucCommand[0] = 0xA0;
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
	m_ucCommand[4] = speed;
	m_ucCommand[5] = waitTime;
	m_ucCommand[6] = 0xAF;
	m_ucCommand[7] = GetCheckSum();
	return (CSerialPort::Instance())->WriteToPort(m_ucCommand,COMMANDLEN,m_Baudrate);
}

bool CPTZSamsung::LeaveCruiseMode(BYTE ucPresetID)
{
	//A0 01 00 4B 00 00 AF 45
	m_ucCommand[0] = 0xA0;
	m_ucCommand[1] = m_lDevAddress;
	m_ucCommand[2] = 0;
	m_ucCommand[3] = 0x4B;
	m_ucCommand[4] = 0;
	m_ucCommand[5] = 0;
	m_ucCommand[6] = 0xAF;
	m_ucCommand[7] = GetCheckSum();
	return (CSerialPort::Instance())->WriteToPort(m_ucCommand,COMMANDLEN,m_Baudrate);
}

bool CPTZSamsung::IsSupportCruise()
{
	printf("CPTZSamsung support cruise\n");
	return true;
}

bool CPTZSamsung::Record_Track()
{
	m_ucCommand[0] = 0xA0;
	m_ucCommand[1] = m_lDevAddress;
	m_ucCommand[2] = m_lDevAddress;
	m_ucCommand[3] = 0x03;
	m_ucCommand[4] = 0xFF;		
	m_ucCommand[5] = 0xFF;			
	m_ucCommand[6] = 0xFF;
	m_ucCommand[7] = 0xFF;
	m_ucCommand[8] = GetCheckSum();
	return (CSerialPort::Instance())->WriteToPort(m_ucCommand,COMMANDLEN,m_Baudrate);
}

//Record_Track 和 Stop_Record_Track 都是一个命令
bool CPTZSamsung::Stop_Record_Track()
{
	m_ucCommand[0] = 0xA0;
	m_ucCommand[1] = m_lDevAddress;
	m_ucCommand[2] = m_lDevAddress;
	m_ucCommand[3] = 0x03;
	m_ucCommand[4] = 0xFF;		
	m_ucCommand[5] = 0xFF;			
	m_ucCommand[6] = 0xFF;
	m_ucCommand[7] = 0xFF;
	m_ucCommand[8] = GetCheckSum();
	return (CSerialPort::Instance())->WriteToPort(m_ucCommand,COMMANDLEN,m_Baudrate);
}

//Samsung 的协议支持多条轨迹,但是我们只是用到一条,默认的轨迹为0x01
bool CPTZSamsung::Start_Track()
{
	m_ucCommand[0] = 0xA0;
	m_ucCommand[1] = m_lDevAddress;
	m_ucCommand[2] = m_lDevAddress;
	m_ucCommand[3] = 0x03;
	m_ucCommand[4] = 0x1B;		
	m_ucCommand[5] = 0x01;			
	m_ucCommand[6] = 0x01;
	m_ucCommand[7] = 0xFF;
	m_ucCommand[8] = GetCheckSum();
	return (CSerialPort::Instance())->WriteToPort(m_ucCommand,COMMANDLEN,m_Baudrate);
}

bool CPTZSamsung::Stop_Track()	//A0 03 03 01 00 00 FF FF FA 
{
	m_ucCommand[0] = 0xA0;
	m_ucCommand[1] = m_lDevAddress;
	m_ucCommand[2] = m_lDevAddress;
	m_ucCommand[3] = 0x01;
	m_ucCommand[4] = 0x00;		
	m_ucCommand[5] = 0x00;			
	m_ucCommand[6] = 0xFF;
	m_ucCommand[7] = 0xFF;
	m_ucCommand[8] = GetCheckSum();
	return (CSerialPort::Instance())->WriteToPort(m_ucCommand,COMMANDLEN,m_Baudrate);;
}

bool CPTZSamsung::Auto_Scan() //A0 03 03 03 19 62 FF FF 7D
{
	m_ucCommand[0] = 0xA0;
	m_ucCommand[1] = m_lDevAddress;
	m_ucCommand[2] = m_lDevAddress;
	m_ucCommand[3] = 0x03;
	m_ucCommand[4] = 0x19;		
	m_ucCommand[5] = 0x62;			
	m_ucCommand[6] = 0xFF;
	m_ucCommand[7] = 0xFF;
	m_ucCommand[8] = GetCheckSum();
	return (CSerialPort::Instance())->WriteToPort(m_ucCommand,COMMANDLEN,m_Baudrate);
}

bool CPTZSamsung::Stop_Auto_Scan()		//A0 03 03 01 00 00 FF FF FA
{
	m_ucCommand[0] = 0xA0;
	m_ucCommand[1] = m_lDevAddress;
	m_ucCommand[2] = m_lDevAddress;
	m_ucCommand[3] = 0x01;
	m_ucCommand[4] = 0x00;		
	m_ucCommand[5] = 0x00;			
	m_ucCommand[6] = 0xFF;
	m_ucCommand[7] = 0xFF;
	m_ucCommand[8] = GetCheckSum();
	return (CSerialPort::Instance())->WriteToPort(m_ucCommand,COMMANDLEN,m_Baudrate);
}

//查看海康的
//雨刷A0 01 01 FF FF FF FF FF 02 A0 01 01 FF FF FF FF FF 02 
//灯光A0 01 01 FF FF FF FF FF 02 A0 01 01 FF FF FF FF FF 02 
bool CPTZSamsung::SetAuxiliary(BYTE auxNum)
{
	if ((AUXILIARY_START >= auxNum) && (AUXILIARY_END <= auxNum))
	{
		assert(false);
		return false;
	}

	m_ucCommand[0] = 0xA0;
	m_ucCommand[1] = m_lDevAddress;
	m_ucCommand[2] = m_lDevAddress;
	m_ucCommand[3] = 0xff;
	m_ucCommand[4] = 0xff;		
	m_ucCommand[5] = 0xff;			
	m_ucCommand[6] = 0xff;
	m_ucCommand[7] = 0xff;
	m_ucCommand[8] = GetCheckSum();
	return (CSerialPort::Instance())->WriteToPort(m_ucCommand,COMMANDLEN,m_Baudrate);
}
bool CPTZSamsung::ClearAuxiliary(BYTE auxNum)
{
	if ((AUXILIARY_START >= auxNum) && (AUXILIARY_END <= auxNum))
	{
		assert(false);
		return false;
	}

	m_ucCommand[0] = 0xA0;
	m_ucCommand[1] = m_lDevAddress;
	m_ucCommand[2] = m_lDevAddress;
	m_ucCommand[3] = 0xff;
	m_ucCommand[4] = 0xff;		
	m_ucCommand[5] = 0xff;			
	m_ucCommand[6] = 0xff;
	m_ucCommand[7] = 0xff;
	m_ucCommand[8] = GetCheckSum();
	return (CSerialPort::Instance())->WriteToPort(m_ucCommand,COMMANDLEN,m_Baudrate);
}


