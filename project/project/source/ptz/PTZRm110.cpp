#include "PTZRm110.h"

#ifdef	LJ_DEBUG
#include "user_config.h"
#endif

CPTZRm110::CPTZRm110()
{
	m_Baudrate = SBR_9600;
}

CPTZRm110::~CPTZRm110()
{

}

/*  hby
 *  命令格式:  共有9Byte, COMMANDLEN = 7  特别注意停止命令的不同
 *  从HIK的DVR捕获的停止命令的数据有两种
 * 其中 m_ucCommand[1] = 0x48的时候是 上,下,左,右的停止命令
 *	    m_ucCommand[1] = 0x4A的时候是 关于摄像头的控制的停止命令
 *    但是,测试的时候发现0x48和0x4A都可以作为上下左右的停止命令,
 *    所以,停止命令就为0x4A.
 
 *   注意2:
 *      RM110协议不能够调整速度.
 */

bool CPTZRm110::Stop()		// F6 4A 00 01 00 01 4C 
{
	m_ucCommand[0] = 0xF6;
	m_ucCommand[1] = 0x4A;
	m_ucCommand[2] = 0x00;
	m_ucCommand[3] = m_lDevAddress;
	m_ucCommand[4] = 0x00;			
	m_ucCommand[5] = 0x01;			//Stop
	m_ucCommand[6] = GetCheckSum();
	return (CSerialPort::Instance())->WriteToPort(m_ucCommand,COMMANDLEN,m_Baudrate);
}

bool CPTZRm110::Left()			// F6 45 00 01 09 01 50
{
	m_ucCommand[0] = 0xF6;
	m_ucCommand[1] = 0x45;
	m_ucCommand[2] = 0x00;
	m_ucCommand[3] = m_lDevAddress;
	m_ucCommand[4] = 0x09;			
	m_ucCommand[5] = 0x01;			//Stop
	m_ucCommand[6] = GetCheckSum();
	return (CSerialPort::Instance())->WriteToPort(m_ucCommand,COMMANDLEN,m_Baudrate);
}

bool CPTZRm110::Right()		//F6 46 00 01 09 01 51
{
	m_ucCommand[0] = 0xF6;
	m_ucCommand[1] = 0x46;
	m_ucCommand[2] = 0x00;
	m_ucCommand[3] = m_lDevAddress;
	m_ucCommand[4] = 0x09;			
	m_ucCommand[5] = 0x01;			//Stop
	m_ucCommand[6] = GetCheckSum();
	return (CSerialPort::Instance())->WriteToPort(m_ucCommand,COMMANDLEN,m_Baudrate);
}

bool CPTZRm110::Up()			//F6 47 00 01 09 01 52
{
	m_ucCommand[0] = 0xF6;
	m_ucCommand[1] = 0x47;
	m_ucCommand[2] = 0x00;
	m_ucCommand[3] = m_lDevAddress;
	m_ucCommand[4] = 0x09;			
	m_ucCommand[5] = 0x01;			//Stop
	m_ucCommand[6] = GetCheckSum();
	return (CSerialPort::Instance())->WriteToPort(m_ucCommand,COMMANDLEN,m_Baudrate);
}

bool CPTZRm110::Down()		//F6 48 00 01 09 01 53
{
	m_ucCommand[0] = 0xF6;
	m_ucCommand[1] = 0x48;
	m_ucCommand[2] = 0x00;
	m_ucCommand[3] = m_lDevAddress;
	m_ucCommand[4] = 0x09;			
	m_ucCommand[5] = 0x01;			//Stop
	m_ucCommand[6] = GetCheckSum();
	return (CSerialPort::Instance())->WriteToPort(m_ucCommand,COMMANDLEN,m_Baudrate);
}

bool CPTZRm110::Near()	////聚焦近 ====> 焦距+  F6 4E 00 02 09 01 5A
{
	m_ucCommand[0] = 0xF6;
	m_ucCommand[1] = 0x4E;
	m_ucCommand[2] = 0x00;
	m_ucCommand[3] = m_lDevAddress;
	m_ucCommand[4] = 0x09;			
	m_ucCommand[5] = 0x01;			//Stop
	m_ucCommand[6] = GetCheckSum();
	return (CSerialPort::Instance())->WriteToPort(m_ucCommand,COMMANDLEN,m_Baudrate);
}

bool CPTZRm110::Far()		//聚焦远 =====> 焦距-  F6 4D 00 02 09 01 59
{
	m_ucCommand[0] = 0xF6;
	m_ucCommand[1] = 0x4D;
	m_ucCommand[2] = 0x00;
	m_ucCommand[3] = m_lDevAddress;
	m_ucCommand[4] = 0x09;			
	m_ucCommand[5] = 0x01;			//Stop
	m_ucCommand[6] = GetCheckSum();
	return (CSerialPort::Instance())->WriteToPort(m_ucCommand,COMMANDLEN,m_Baudrate);
}

bool CPTZRm110::ZoomOut()//特写 ====> 缩放+  F6 4B 00 02 09 01 57
{
	m_ucCommand[0] = 0xF6;
	m_ucCommand[1] = 0x4B;
	m_ucCommand[2] = 0x00;
	m_ucCommand[3] = m_lDevAddress;
	m_ucCommand[4] = 0x09;			
	m_ucCommand[5] = 0x01;			//Stop
	m_ucCommand[6] = GetCheckSum();
	return (CSerialPort::Instance())->WriteToPort(m_ucCommand,COMMANDLEN,m_Baudrate);
}

bool CPTZRm110::ZoomIn()//广角 =====> 缩放-  F6 4C 00 02 09 01 58
{
	m_ucCommand[0] = 0xF6;
	m_ucCommand[1] = 0x4C;
	m_ucCommand[2] = 0x00;
	m_ucCommand[3] = m_lDevAddress;
	m_ucCommand[4] = 0x09;			
	m_ucCommand[5] = 0x01;			//Stop
	m_ucCommand[6] = GetCheckSum();
	return (CSerialPort::Instance())->WriteToPort(m_ucCommand,COMMANDLEN,m_Baudrate);
}

bool CPTZRm110::IrisOpen()//光圈放大  F6 49 00 02 09 01 55
{
	m_ucCommand[0] = 0xF6;
	m_ucCommand[1] = 0x49;
	m_ucCommand[2] = 0x00;
	m_ucCommand[3] = m_lDevAddress;
	m_ucCommand[4] = 0x09;			
	m_ucCommand[5] = 0x01;			//Stop
	m_ucCommand[6] = GetCheckSum();
	return (CSerialPort::Instance())->WriteToPort(m_ucCommand,COMMANDLEN,m_Baudrate);
}

bool CPTZRm110::IrisClose()//光圈缩小 F6 4A 00 02 09 01 56
{
	m_ucCommand[0] = 0xF6;
	m_ucCommand[1] = 0x4A;
	m_ucCommand[2] = 0x00;
	m_ucCommand[3] = m_lDevAddress;
	m_ucCommand[4] = 0x09;			
	m_ucCommand[5] = 0x01;			//Stop
	m_ucCommand[6] = GetCheckSum();
	return (CSerialPort::Instance())->WriteToPort(m_ucCommand,COMMANDLEN,m_Baudrate);
}

/*
 * hby 增加四个方向
 */
bool CPTZRm110::LeftDown()
{
	return true;
}

bool CPTZRm110::LeftUp()
{
	return true;
}

bool CPTZRm110::RightDown()
{
	return true;
}

bool CPTZRm110::RightUp()
{
	return true;
}


bool CPTZRm110::PresetGo(BYTE ucPresetID)
{
	return true;
}

bool CPTZRm110::PresetSet(BYTE ucPresetID)
{
	return true;
}

BYTE CPTZRm110::GetCheckSum()
{
	BYTE CheckSum=0;
	for(int i=1;i<6;i++)
	{
		CheckSum += m_ucCommand[i];
	}
	return CheckSum;
}

// 雨刮 开
// F6 5E 00 01 01 01 61 
// 雨刮 关
// F6 5E 00 01 01 00 60 
// 
// 灯 开
// F6 5E 00 01 01 00 60 
// 灯 关
// F6 5E 00 01 01 00 60
//海康定义如下
//雨刷F6 00 00 01 00 01 02 F6 00 00 01 00 01 02 
//灯光F6 00 00 01 00 01 02 F6 00 00 01 00 01 02 
bool CPTZRm110::SetAuxiliary(BYTE auxNum)
{
	if ((AUXILIARY_START >= auxNum) && (AUXILIARY_END <= auxNum))
	{
		assert(false);
		return false;
	}

	m_ucCommand[0] = 0xF6;
	switch(auxNum)
	{
	case AUXILIARY_BRUSH:
		{
			m_ucCommand[1] = 0x00;
			m_ucCommand[5] = 0x01;
		}
		break;
	case AUXILIARY_LAMP:
		{
			m_ucCommand[1] = 0x00;
			m_ucCommand[5] = 0x01;
		}
		break;
	//case AUXILIARY_VIDEO_POWER:
	//	break;
	default:
		{
			return false;
		}
		break;
	}
	m_ucCommand[2] = 0x00;
	m_ucCommand[3] = m_lDevAddress;
	m_ucCommand[4] = 0x00;
	m_ucCommand[6] = GetCheckSum();
	return (CSerialPort::Instance())->WriteToPort(m_ucCommand,COMMANDLEN,m_Baudrate);
}

bool CPTZRm110::ClearAuxiliary(BYTE auxNum)
{
	if ((AUXILIARY_START >= auxNum) && (AUXILIARY_END <= auxNum))
	{
		assert(false);
		return false;
	}

	m_ucCommand[0] = 0xF6;
	switch(auxNum)
	{
	case AUXILIARY_BRUSH:
		{
			m_ucCommand[1] = 0x00;
			m_ucCommand[5] = 0x01;
		}
		break;
	case AUXILIARY_LAMP:
		{
			m_ucCommand[1] = 0x00;
			m_ucCommand[5] = 0x01;
		}
		break;
		//case AUXILIARY_VIDEO_POWER:
		//	break;
	default:
		{
			return false;
		}
		break;
	}
	
	m_ucCommand[2] = 0x00;
	m_ucCommand[3] = m_lDevAddress;
	m_ucCommand[4] = 0x00;			
	m_ucCommand[6] = GetCheckSum();
	return (CSerialPort::Instance())->WriteToPort(m_ucCommand,COMMANDLEN,m_Baudrate);
}

