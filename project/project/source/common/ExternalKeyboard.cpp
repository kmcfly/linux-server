#include "ExternalKeyboard.h"
#include "PL14_NVKBD30_70.h"
#include "SerialPort.h"


CExternalKeyboard * CExternalKeyboard::Instance()
{
	static CExternalKeyboard ExternalKeyboard;
	return &ExternalKeyboard;
}

bool CExternalKeyboard::GetExKeyboardValue( KEY_VALUE_INFO *pkeyValue )
{
	bool retval = false;
	
	m_keyListLock.Lock();

	if( 0 < m_keyList.size() )
	{
		*pkeyValue = m_keyList.front();
		m_keyList.pop_front();
		retval = true;
	}
	m_keyListLock.UnLock();
	//PUB_Sleep( 10 );
	return retval;
}

void CExternalKeyboard::SetExKeyboardType( KEYBOARD_TYPE ExKeyboardType )
{
	if( KEYBOARD_TWOEM == ExKeyboardType )
	{
		m_ExKeyboardCfg.ExKeyboardType	= KEY_TYPE_TWOEM;

		m_ExKeyboardCfg.baudrate		= SBR_9600;
		m_ExKeyboardCfg.databit			= 8;
		m_ExKeyboardCfg.stopbit			= 1;
		m_ExKeyboardCfg.checkbit		= 0;
		
		m_ExKeyboardCfg.commandlen		= 6;

		strcpy( ( char *)( m_ExKeyboardCfg.name ), "TVT_TAIWAN_OEM" );
	}
	else if( KEYBOARD_MY1003K == ExKeyboardType )
	{
		m_ExKeyboardCfg.ExKeyboardType = KEY_TYPE_MY1003;

		m_ExKeyboardCfg.baudrate		= SBR_9600;
		m_ExKeyboardCfg.databit			= 8;
		m_ExKeyboardCfg.stopbit			= 1;
		m_ExKeyboardCfg.checkbit		= 0;

		m_ExKeyboardCfg.commandlen		= 5;
		
		strcpy( ( char *)( m_ExKeyboardCfg.name ), "TVT_MY1003" );
	}
	/*
	else if( KEY_TYPE_BLACK == ExKeyboardType )
	{
		m_ExKeyboardCfg.ExKeyboardType = KEY_TYPE_BLACK;

		m_ExKeyboardCfg.baudrate		= SBR_9600;
		m_ExKeyboardCfg.databit			= 8;
		m_ExKeyboardCfg.stopbit			= 1;
		m_ExKeyboardCfg.checkbit		= 0;

		m_ExKeyboardCfg.commandlen		= 8;

		strcpy( ( char *)( m_ExKeyboardCfg.name ), "TVT_BLACK" );
	}
	*/
	else if (KEYBOARD_PL14_NVKBD == ExKeyboardType)
	{
		m_ExKeyboardCfg.ExKeyboardType = KEY_TYPE_PL14_NVKBD30_70;

		m_ExKeyboardCfg.baudrate		= SBR_9600;
		m_ExKeyboardCfg.databit			= 8;
		m_ExKeyboardCfg.stopbit			= 1;
		m_ExKeyboardCfg.checkbit		= 0;

		m_ExKeyboardCfg.commandlen		= 10;

		strcpy( ( char *)( m_ExKeyboardCfg.name ), "PL14_NVKBD30_70" );
	}
	else if (KEYBOARD_TWSY == ExKeyboardType)
	{
		
		m_ExKeyboardCfg.ExKeyboardType = KEY_TYPE_TWSY;

		m_ExKeyboardCfg.baudrate		= SBR_9600;
		m_ExKeyboardCfg.databit			= 8;
		m_ExKeyboardCfg.stopbit			= 1;
		m_ExKeyboardCfg.checkbit		= 0;

		m_ExKeyboardCfg.commandlen		= 5;

		strcpy(( char *)( m_ExKeyboardCfg.name), "TVT_TWSY");
		printf("%s:%s:%d, external keyboard TVT_TWSY\n", __FUNCTION__, __FILE__, __LINE__);
	}
	else
	{
		assert( false );
		printf("%s:%s:%d, no external keyboard\n", __FUNCTION__, __FILE__, __LINE__);
	}

	printf("%s:%s:%d, The external 485 keyboard is %s\n", __FUNCTION__, __FILE__, __LINE__, m_ExKeyboardCfg.name);
}


bool CExternalKeyboard::Start()
{
	bool retval = true;

	assert( 0 == m_ExKeyboardProcID );
	m_ExKeyboardProcID = PUB_CreateThread( ExBoardThread, (void *)this, &m_bExKeyboardProc );
	if( PUB_CREATE_THREAD_FAIL == m_ExKeyboardProcID )
	{
		PUB_PrintError(__FILE__,__LINE__);
		retval = false;
	}
	
	return retval;
}

void CExternalKeyboard::Stop()
{
	PUB_ExitThread( &m_ExKeyboardProcID, &m_bExKeyboardProc );
}

PUB_THREAD_RESULT PUB_THREAD_CALL CExternalKeyboard::ExBoardThread(void *pParam)
{
#ifdef __ENVIRONMENT_LINUX__
	pid_t pid = getpid();
	pid_t tid = syscall(__NR_gettid);

	printf("%s, %d, pid = %d, tid=%d\n", __FILE__, __LINE__, pid, tid);
#endif

	assert(NULL != pParam);
	CExternalKeyboard* pOperator = (CExternalKeyboard*)pParam;
	pOperator->AddExKeyboardProc();

	return 0;
}

void CExternalKeyboard::AddExKeyboardProc()
{
	KEY_VALUE_INFO keyInfo;

	while( m_bExKeyboardProc )
	{
		if( ReceiveExKeyboardInput( &keyInfo ) )
		{
			m_keyListLock.Lock();

			if( 2 < m_keyList.size() )
			{
				m_keyList.pop_front();
			}

			m_keyList.push_back( keyInfo );
			m_keyListLock.UnLock();
			memset( &keyInfo, 0, sizeof( KEY_VALUE_INFO ) );
			
			//PUB_Sleep(10);
			
		}
		else
		{
			PUB_Sleep(30);
		}

	}
}


bool CExternalKeyboard::ReceiveExKeyboardInput( KEY_VALUE_INFO *pKeyInfo )
{
	bool retval = false;
	BYTE pData[10] = {0};
	CSerialPort *pSerialPort = CSerialPort::Instance();

	assert( NULL != pKeyInfo );

	if( m_ExKeyboardCfg.commandlen == ( pSerialPort->ReadPort( pData, m_ExKeyboardCfg.commandlen, (PORT_BAUDRATE)m_ExKeyboardCfg.baudrate ) ) )
	{
		//读到所需要的数据字节长度
		
		switch( m_ExKeyboardCfg.ExKeyboardType )
		{
		case KEY_TYPE_TWOEM:
		{
			retval = TranslateTaiwanOEM( pKeyInfo, pData );
			break;
		}
		case KEY_TYPE_MY1003:
		{
			retval = TranslateMY1003( pKeyInfo, pData );
			break;

		}
		/*case KEY_TYPE_BLACK:
		{
				retval = TranslateBlack( pKeyInfo, pData );
				break;
		}*/
		case KEY_TYPE_PL14_NVKBD30_70:
		{
			retval = TranslatePL14NVKBD30_70( pKeyInfo, pData );
			break;
		}
		case KEY_TYPE_TWSY:
		{
			retval = TranslateTWSY( pKeyInfo, pData );
			break;
		}
		default:
			break;
		}
	}
	
	return retval; 
}


CExternalKeyboard::CExternalKeyboard()
{


}


CExternalKeyboard::~CExternalKeyboard()
{

}

bool CExternalKeyboard::TranslateTaiwanOEM( KEY_VALUE_INFO *pKeyInfo, const BYTE *buf )
{
	if( (NULL == pKeyInfo) || (NULL == buf) )
	{
		assert(false);
		return false;
	}

	if( (buf[2] == 0xb0)&&( (buf[0] - (BYTE)207) == m_uclocalDVRID) )
	{
		KEY_VALUE_INFO KeyValue;
		KeyValue.isDown = 0;
		KeyValue.xPos	= 0;
		KeyValue.yPos	= 0;
		KeyValue.type	= m_ExKeyboardCfg.ExKeyboardType;

		unsigned int datetemp = buf[3];
		KeyValue.value = (datetemp<<8)|buf[4];
		
		*pKeyInfo = KeyValue;

		return true;
	}
	else
	{
		if(buf[2] != 0xb0)
		{
			printf("%s:%s:%d, TranslateTaiwanOEM() error:buf[2]=0x%02x is not 0xb0. \n", __FUNCTION__, __FILE__, __LINE__, buf[2]);
		}
		else
		{
			printf("%s:%s:%d, TranslateTaiwanOEM() error:DVR ID=%d, (buf[0] - (BYTE)207)=(%d - 207)=%d, are not same.\n", __FUNCTION__, __FILE__, __LINE__, m_uclocalDVRID, buf[0], buf[0] - 207);
		}

		return false;
	}

}
bool CExternalKeyboard::TranslateMY1003( KEY_VALUE_INFO *pKeyInfo, const BYTE *buf )
{
	assert( NULL != pKeyInfo );
	memset( pKeyInfo, 0, sizeof( KEY_VALUE_INFO ) );
	assert( NULL != buf );
	bool retval = false;
	unsigned char XorValue;
	unsigned long dataTemp;
	unsigned char DeviceID = buf[1];
	CSerialPort *pSerialPort = CSerialPort::Instance();

	//起始位和校验位错误就刷新串口缓存
	XorValue = ( buf[0] ^ buf[1]) ^( buf[2] ^ buf[3]);
	if( ( 0xff != buf[0] ) || ( XorValue != buf[4]) )
	{
		int fdCom = pSerialPort->GetfdCom();
		assert(-1 < fdCom);
		tcflush(fdCom,TCIFLUSH);
		return retval;

	}

	//检查该DVR是不是受空设备
	if( m_uclocalDVRID != DeviceID )
	{
		return false;
	}

	KEY_VALUE_INFO KeyValue;
	KeyValue.isDown = 0;
	KeyValue.xPos	= 0;
	KeyValue.yPos	= 0;
	KeyValue.type	= m_ExKeyboardCfg.ExKeyboardType;

	switch( buf[2] )
	{
	case 0x10:
	case 0x11:
	case 0x12:
	case 0x13:
	case 0x14:
	case 0x15:
	case 0x16:
	case 0x17:
	case 0x18:
	case 0x1a:
	case 0x1b:
	case 0x1c:
	case 0x1d:
	case 0x1e:
	case 0x1f:
	case 0x20:
	case 0x21:
	case 0x22:
	case 0x23:
	case 0x24:
	case 0x25:
	case 0x26:
	case 0x27:
	case 0x28:
	case 0x29:
	case 0x2a:
	case 0x2b:
	case 0x39:
	case 0x3a:
	case 0x3b:
		//这些按键在按下和弹起时都发送码值，按下的时候才有效,buf[3]=0x00
		if( 0x00 == buf[3] )
		{
			KeyValue.value = ( unsigned long )buf[2];
			retval = true;
		}
		else
		{
			return false;
		}

		break;
	case 0x19:
	case 0x31:
	case 0x32:
	case 0x34:
	case 0x35:
	case 0x37:
	case 0x38:
	case 0x40:
	case 0x41:
	case 0x42:
	case 0x43:
	case 0x44:
	case 0x45:
	case 0x46:
	case 0x47:
	case 0x48:
	case 0x49:
		//这些按键（包括摇杆）每次操作（按下和弹起）只发一组码值，buf[3]中存放操作的参数
		dataTemp = buf[3];
		KeyValue.value = ( ( dataTemp << 8 ) | buf[2] );
		retval = true;
		break;
	case 0x30:
	case 0x33:
	case 0x36:
		dataTemp = buf[3];
		KeyValue.value = ( ( dataTemp << 8 ) | buf[2] );
		retval = true;
		break;
	case 0x00:
		KeyValue.value = 0xff;
		retval = true;
			break;
	default:
		break;
	}

	*pKeyInfo = KeyValue;

	return retval;
}
bool CExternalKeyboard::TranslateBlack( KEY_VALUE_INFO *pKeyInfo, const BYTE *buf )
{
	bool retval =false;

	return retval;
}
bool CExternalKeyboard::TranslateTWSY( KEY_VALUE_INFO *pKeyInfo, const BYTE *buf)
{
	
	CSerialPort *pSerialPort = CSerialPort::Instance();
	if( (NULL == pKeyInfo) || (NULL == buf) )
	{
		assert(false);
		return false;
	}
	if( (buf[0] != 0xff) || (buf[4] != buf[0] ^ buf[1] ^ buf[2] ^ buf[3]) )
	{
		int fdCom = pSerialPort->GetfdCom();
		assert(-1 < fdCom);
		tcflush(fdCom,TCIFLUSH);
	}
	if (m_uclocalDVRID != buf[1])
	{
		return false;
	}
	if ((buf[0] == 0xff) && (buf[4] == buf[0] ^ buf[1] ^ buf[2] ^ buf[3]))
	{
		if(buf[3] == 0)
		{
			KEY_VALUE_INFO KeyValue;
			KeyValue.isDown = buf[3];
			KeyValue.xPos = 0;
			KeyValue.yPos = 0;
			KeyValue.type = m_ExKeyboardCfg.ExKeyboardType;
			KeyValue.value = buf[2];

			*pKeyInfo = KeyValue;
			return true;
		}
		else if(buf[3] == 0x01 && buf[2] >= 0x50 && buf[2] <= 0x57)
		{
			KEY_VALUE_INFO KeyValue;
			KeyValue.isDown = buf[3];
			KeyValue.xPos = 0;
			KeyValue.yPos = 0;
			KeyValue.type = m_ExKeyboardCfg.ExKeyboardType;
			KeyValue.value = buf[2];

			*pKeyInfo = KeyValue;
			return true;
		}
		else
		{
			return false;
		}
	}
	else 
	{
		return false;
	}
}


bool CExternalKeyboard::TranslatePL14NVKBD30_70(KEY_VALUE_INFO *pKeyInfo, const BYTE *buf)
{
	if (NULL == pKeyInfo || NULL == buf)
	{
		printf("%s:%s:%d, Running at here\n", __FUNCTION__, __FILE__, __LINE__);
		assert(false);
		return false;
	}
	else
	{
		memset( pKeyInfo, 0, sizeof( KEY_VALUE_INFO ) );
	}

	unsigned char XorValue;
	unsigned long dataTemp;
	//起始位和校验位错误就刷新串口缓存
	XorValue = buf[0] + buf[1] + buf[2] + buf[3] + buf[4] + buf[5] + buf[6] + buf[7] + buf[8];
	if( ( PL14_NVKBD_START_CHAR != buf[0] ) || ( XorValue != buf[9]) )
	{
		int fdCom = CSerialPort::Instance()->GetfdCom();
		assert(-1 < fdCom);
		tcflush(fdCom,TCIFLUSH);
		printf("%s:%s:%d, Running at here, xorVal = 0x%02x, buf[9]=0x%02x\n", __FUNCTION__, __FILE__, __LINE__, XorValue, buf[9]);
		return false;
	}

	//检查该DVR是不是受空设备
	if( m_uclocalDVRID != buf[DEVICE_ID_POS] )
	{
		printf("%s:%s:%d, The device id = %d, control device id = %d\n", __FUNCTION__, __FILE__, __LINE__, m_uclocalDVRID, buf[DEVICE_ID_POS]);
		return false;
	}

	KEY_VALUE_INFO KeyValue;
	memset(&KeyValue, 0, sizeof(KEY_VALUE_INFO));

	KeyValue.isDown = 0;
	KeyValue.xPos	= 0;
	KeyValue.yPos	= 0;
	KeyValue.type	= m_ExKeyboardCfg.ExKeyboardType;
	KeyValue.value	= buf[3];
	KeyValue.value	= ( KeyValue.value << 8 )| buf[4];
	KeyValue.value	= ( KeyValue.value << 8 )| buf[5];

	*pKeyInfo = KeyValue;
	return true;
}



