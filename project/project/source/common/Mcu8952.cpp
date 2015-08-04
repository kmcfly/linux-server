/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : �ܷ�
** Date         : 2007-09-3
** Name         : Keyboard.cpp
** Version      : 1.0
** Description  :
** Modify Record:
1:
***********************************************************************/
#include "Mcu8952.h"
#include "mylist.cpp"
#include "PUB_common.h"

CMcu8952* CMcu8952::m_pInstance = NULL;

CMcu8952* CMcu8952::Instance ()
{
	if(NULL == m_pInstance)
	{
		m_pInstance = new CMcu8952;
	}

	assert(NULL != m_pInstance);	
	
	return m_pInstance;
}

CMcu8952::CMcu8952()
{	
	m_LampState = 0;
	m_Delay = 0;
	m_fd = -1;
	m_ThreadHandle = 0;
	m_ThreadFlag = true;
	m_Alarm_get_flag = 0;
	m_ListLock;
//	m_Alarm = 1<<SENSOR_INPUT_NUM;
	m_Reply = 0xff;
	m_VersionChip = (0x0a<<9) | (0x04<<5) | (0x17); //��Ƭ���汾Ϊ 10-04-23	
	m_VersionHard = 0x0102;	//Ӳ���汾Ĭ��Ϊ: 1.2
	pthread_mutex_init( &m_com_mutex, NULL );
}

CMcu8952::~CMcu8952()
{
	//Quit();
	pthread_mutex_destroy( &m_com_mutex );
}

bool CMcu8952::Operation(SERIAL_OPERATION opType, unsigned long opeValue)
{
	bool ret = false;

	switch(opType) 
	{
	case SERIAL_LAMP:		
		if ( opeValue & 0x80 ) 
		{
			m_LampState = m_LampState & ( opeValue & 0x7C);
		}
		else
		{
			m_LampState = m_LampState | ( opeValue & 0x7C);
		}	
		ret = WriteSerial(SERIAL_LAMP, m_LampState);
		break;

	case SERIAL_RELAY:
		if ( opeValue & 0x80 ) 
		{
			m_Delay = m_Delay & ( opeValue & 0x0F );
		}
		else
		{
			m_Delay = m_Delay | ( opeValue & 0x0F );			
		}

		ret = WriteSerial(SERIAL_RELAY, m_Delay);
		break;

	case SERIAL_BUZZER:
		ret = WriteSerial(SERIAL_BUZZER, opeValue);
		break;

	case SERIAL_SPOT:
		ret = WriteSerial(SERIAL_SPOT, opeValue);
		break;

	case SERIAL_FEEDDOG:
		ret = WriteSerial(SERIAL_FEEDDOG, opeValue);
		break;
		
	case SERIAL_KILLDOG:
		ret = WriteSerial(SERIAL_KILLDOG, opeValue);
		break;

	case SERIAL_CTRL485:
		ret = WriteSerial(SERIAL_CTRL485, opeValue?1:0);
		break;

	case SERIAL_GETVERSION:
		ret = WriteSerial(SERIAL_GETVERSION, 0);
		break;

	case SERIAL_SETVIDEOLOSSLAMP:
		ret = WriteSerialEx(SERIAL_SETVIDEOLOSSLAMP,   opeValue&0xff, (opeValue>>8)&0xff);
		break;

	case SERIAL_SETVIDEOLOSSLAMP_H:
		ret = WriteSerialEx(SERIAL_SETVIDEOLOSSLAMP_H, (opeValue>>16)&0xff, (opeValue>>24)&0xff);
		break;
	default:
		break;
	}
	
	//���ӳټ�С��������ֱ�������رղ���������Ϳ��ܺ������ӳ�̫����ء�
	PUB_Sleep(1);
	return ret;
}

bool CMcu8952::WriteSerial(BYTE eventType, BYTE eventValue)
{
	BYTE buffer[5] = {0};	
	int sendTime = 0;
	
	buffer[0] = 0xa0;
	buffer[1] = eventType;
	buffer[2] = eventValue;
	buffer[3] = 0;
	buffer[4] = (BYTE)(buffer[0]+buffer[1]+buffer[2]);

	m_Reply = 0xff;
	
	if (-1 == m_fd)
	{
		return true;
	}
	
	while (sendTime < 5)
	{
		pthread_mutex_lock( &m_com_mutex );
		//printf("%s():%s:%d, write the MCU serial port\n", __FUNCTION__, __FILE__, __LINE__);
//		printf("%s():%s:%d, the m_fd =%d\n", __FUNCTION__, __FILE__, __LINE__,  m_fd );
		BYTE ret = write(m_fd, &buffer, 5);
#if defined(__TDFH__)
		//�ȴ����ݷ������
		tcdrain(m_fd);
#endif

//		printf("%s():%s:%d, the ret  = %d\n", __FUNCTION__, __FILE__, __LINE__, ret );
		PUB_Sleep(2);
		pthread_mutex_unlock( &m_com_mutex );

		PUB_Sleep(20);

//		printf( "%s():%s:%d, the m_Reply = %d\n", __FUNCTION__, __FILE__, __LINE__, m_Reply );
		if ((5 == ret) && (m_Reply == eventType))
		//if( 5 == ret )
		{		
			return true;
		}

		++ sendTime;
	}

//	printf("%s():%s:%d, sendTime = %d\n", __FUNCTION__, __FILE__, __LINE__, sendTime);
	return false;
}
	
bool CMcu8952::WriteSerialEx(BYTE eventType1, BYTE eventValue2, BYTE eventValue3)
{
	BYTE buffer[5] = {0};	
	int sendTime = 0;
	
	//printf("%s,%d:%x,%x,%x\n",__FILE__,__LINE__,eventType1,eventValue2,eventValue3);

	buffer[0] = 0xa0;
	buffer[1] = eventType1;
	buffer[2] = eventValue2;
	buffer[3] = eventValue3;
	buffer[4] = (BYTE)(buffer[0]+buffer[1]+buffer[2]+buffer[3]);

	m_Reply = 0xff;

	if (-1 == m_fd)
	{
		return true;
	}
	
	while (sendTime < 5)
	{
		pthread_mutex_lock( &m_com_mutex );
		BYTE ret = write(m_fd, &buffer, 5);
#if defined(__TDFH__)
		//�ȴ����ݷ������
		tcdrain(m_fd);
#endif

		PUB_Sleep(2);
		pthread_mutex_unlock( &m_com_mutex );

		PUB_Sleep(20);
		
		if ((5 == ret) && (m_Reply == eventType1))
		{	
			//printf("%s():%s:%d, write MCU serial OK and get the serial replay\n", __FUNCTION__, __FILE__, __LINE__);
			return true;
		}

		++ sendTime;
	}
	
	return false;

}

bool CMcu8952::SetSerialParity(int databits, int stopbits, int parity)
{
	struct termios options;
	
	if  ( tcgetattr( m_fd,&options)  !=  0)
	{
		perror("SetupSerial 1");
		return false;
	}

	//PrintMCUSerialConfig( &options );


//��������λ��
	options.c_cflag &= ~CSIZE;
	switch (databits)
	{
	case 7:
		options.c_cflag |= CS7;
		break;
	case 8:
		options.c_cflag |= CS8;
		break;
	default:
		printErrorLocation("Unsupported data size");
		return false;
	}

//������żУ��λ
	switch (parity)
	{
		case 'n':
		case 'N':
			options.c_cflag &= ~PARENB;   /* �ر����ʱ������żУ������ʱ������żУ�� */
			options.c_iflag &= ~INPCK;     /* ��ֹ����ʱ��żУ��*/
			break;
		case 'o':
		case 'O':
			options.c_cflag |= (PARODD | PARENB); /* ����Ϊ��Ч���������ʱ������żУ������ʱ������żУ��*/
			options.c_iflag |= INPCK;           	
			break;
		case 'e':
		case 'E':
			options.c_cflag |= PARENB;     /* Enable parity */
			options.c_cflag &= ~PARODD;   /* ת��ΪżЧ��*/
			options.c_iflag |= INPCK;       /* Disnable parity checking */
			break;
		case 'S':
		case 's':  /*as no parity*/
		    	options.c_cflag &= ~PARENB;
			options.c_cflag &= ~CSTOPB;
			break;
		default:
			perror("Unsupported parity\n");
			return false;
	}

// ����ֹͣλ����
	switch (stopbits)
	{
		case 1:
			options.c_cflag &= ~CSTOPB; /*����һ��ֹͣλ*/
			break;
		case 2:
			options.c_cflag |= CSTOPB; /*��������ֹͣλ*/
		   break;
		default:
			 perror("Unsupported stop bits\n");
			 return false;
	}

	/* Set input parity option */
	//if (parity != 'n')
	//	options.c_iflag |= INPCK;

	//mj ������ǿ����ն�֮��ģ�ֻ�Ǵ��ڴ������ݣ�������Ҫ����������
	//mj ��ôʹ��ԭʼģʽ(Raw Mode)��ʽ��ͨѶ�����÷�ʽ���£� 
	options.c_lflag  &= ~(ICANON | ECHO | ECHOE | ISIG); // Input   
	options.c_oflag  &= ~OPOST;              				/*��ִ���������*/
	options.c_iflag &= ~(ICRNL | IXON); 					/*������ʱ�����س�ת��Ϊ���У�������ʼ��ֹͣ�������*/

	tcflush(m_fd,TCIFLUSH);							/*����յ������ݵ��������巢�Ѿ�д�����ݵ�����*/
	
	//options.c_cc[VTIME] = 150; 		/* ���ó�ʱ15 seconds*/
	options.c_cc[VTIME] = 5;
	options.c_cc[VMIN] = 0; 		/* Update the options and do it NOW */

	if (tcsetattr(m_fd,TCSANOW,&options) != 0)
	{
		perror("SetupSerial 3");
		return false;
	}

	memset( &options, 0, sizeof( struct termios ) );
	if  ( tcgetattr( m_fd,&options)  !=  0)
	{
		perror("SetupSerial 1");
		return false;
	}

	//PrintMCUSerialConfig( &options );
	return true;
}

bool CMcu8952::SetSerialSpeed(PORT_BAUDRATE baudrate)
{
	unsigned long BaudRate;
	
	assert (-1 != m_fd);	
	
	struct termios   options;

	unsigned rate = GetBaudRate(baudrate);

	memset( &options, 0, sizeof( struct termios ) );
	if( tcgetattr(m_fd, &options)  != 0 )
	{
		printErrorLocation("tcgetattr error");
		return false;
	}
	//PrintMCUSerialConfig( &options );

	tcflush(m_fd, TCIOFLUSH);
	cfsetispeed(&options, rate);
	cfsetospeed(&options, rate);
	
	if  (	tcsetattr(m_fd, TCSANOW, &options) != 0)
	{
		printErrorLocation("tcsetattr fd1");
		return false;
	}
	tcflush(m_fd,TCIOFLUSH);

	GetBaudRate( baudrate, &BaudRate);
//	printf("%s():%s:%d, set MCU serial bandrate=%d OK\n", __FUNCTION__, __FILE__, __LINE__, BaudRate );
	
	return true;
}

bool CMcu8952::Initial( DWORD &sensorInputType )
{
	m_fd = open(dev1,  O_RDWR );
	assert (-1 != m_fd);
	if ( -1 == m_fd )
	{
		printErrorLocation("open ttys2 error");
		return false;
	}

//	printf("%s():%s:%d , the MCU serial port file descriptor is :%d\n", __FUNCTION__, __FILE__, __LINE__, m_fd );
	//���ô����ٶ�
	if (!SetSerialSpeed(SBR_9600)) 
	{
		//fprintf (stdout,"(%s  %d): SetSpeed  execute failed!\n");
		return false;
	}

	//���ô�����żУ��λ
	if (!SetSerialParity(8, 1, 'N')) 
	{
		//fprintf (stdout,"(%s  %d): SetParity execute failed!\n");
		return false;
	}

	m_Alarm = sensorInputType;
	//Start();	//kg_20080310


	//���Ը�����
	/*Operation(SERIAL_LAMP,SERIES_LAMP2_ON);
	Operation(SERIAL_LAMP,SERIES_LAMP3_ON);
	Operation(SERIAL_LAMP,SERIES_LAMP4_ON);
	Operation(SERIAL_LAMP,SERIES_LAMP5_ON);
	Operation(SERIAL_LAMP,SERIES_LAMP6_ON);
	
	Operation(SERIAL_LAMP,SERIES_LAMP6_OFF);
	Operation(SERIAL_LAMP,SERIES_LAMP5_OFF);
	Operation(SERIAL_LAMP,SERIES_LAMP4_OFF);
	Operation(SERIAL_LAMP,SERIES_LAMP3_OFF);
	Operation(SERIAL_LAMP,SERIES_LAMP2_OFF);
	*/

	//�ص����е�VIDEO LOSS  ָʾ��
	Operation(SERIAL_SETVIDEOLOSSLAMP,  0xffffffff);
	Operation(SERIAL_SETVIDEOLOSSLAMP_H,0xffffffff);
	
	return true;
}

void CMcu8952::Quit()
{
	//Stop();
	
	if (m_fd!=-1) 
	{
		close(m_fd);
		m_fd = -1;
	}
}

RESULT WINAPI CMcu8952::ThreadKeyboardProc(LPVOID lpParame)
{
#ifdef __ENVIRONMENT_LINUX__
	pid_t pid = getpid();
	printf("%s, %d, pid = %d\n", __FILE__, __LINE__, pid);
#endif

	CMcu8952 * pKeyboard = static_cast<CMcu8952 *>(lpParame);
	pKeyboard->ReadSerial();	
	return 0L;
}

#if defined(__TDFH__)
void CMcu8952::ReadSerial()
{
	int size = 0;
	char buffer[5] = {0};
	char crc = 0;
	unsigned long eventValue=0;
	fd_set   rdfd;
	int readsize = 0;

//���ó�ʱΪ2��
	struct timeval timeout;	
	timeout.tv_sec = 2;
	timeout.tv_usec = 0;
	
	int ret = 0;
	memset(buffer, 0, 5);

	while(m_ThreadFlag)
	{	
		//printf("%s():%s:%d, running at the MCU8952 thread\n", __FUNCTION__, __FILE__, __LINE__);
		FD_ZERO(&rdfd);
		FD_SET(m_fd, &rdfd);

		ret = select(m_fd+1, &rdfd, NULL, NULL, &timeout);	

		if(ret > 0)
		{
			pthread_mutex_lock( &m_com_mutex );
			size += read(m_fd, buffer+size, 5 - size);
			if ( (0x0a != buffer[0]) && (0xa0 != buffer[0]) ) 
			{
				pthread_mutex_unlock( &m_com_mutex );
				size = 0;
				ret = 0;
				continue;
			}

			if ( 5 > size)
			{
				//�������5���ֽڵ�����
				pthread_mutex_unlock( &m_com_mutex );
				continue;
				//size = read(m_fd,buffer+size, 5-size);
			}

			pthread_mutex_unlock( &m_com_mutex );

			//����У��ֵ
			crc = buffer[0] + buffer[1] + buffer[2] + buffer[3];

			//�յ�MCU�ϱ����¼�
			if ((0x0a ==buffer[0]) && (buffer[4] == crc)) 
			{
				switch( buffer[1])
				{
					//�ϱ�������״̬
					case 5:
						if( m_Alarm_get_flag)
						{
							m_Alarm = ( buffer[3] | ( buffer[2] << 8 ) );
							m_Alarm_get_flag = 0;	
						}
						break;
					//�ϱ�MCU �汾
					case 6:
						m_VersionChip = ( (buffer[3]<<8) | buffer[2] );
//						printf("%s():%s:%d, get the MCU version\n", __FUNCTION__, __FILE__, __LINE__);
						break;		
					//�ϱ�Ӳ���汾
					case 7:
						m_VersionHard = ( (buffer[2]<<8) | buffer[3] );	
//						printf("%s():%s:%d, get the hardware version\n", __FUNCTION__, __FILE__, __LINE__);						
						break;
					//ң�ػ�����尴��������
					default:
						eventValue = ((buffer[3] << 16) | (buffer[2] << 8) | buffer[1]);
						//printf("%s():%s:%d, get eventValue=0x%08x\n", __FUNCTION__, __FILE__, __LINE__, eventValue );
						AddEventList(eventValue);
						break;
						
				}

			}
		//�յ�MCU��д���ڵĻ�Ӧ
			else if ((0xa0 ==buffer[0]) && (buffer[4] == crc)) 
			{
				//printf("Receive a write reply: %d\n", buffer[1]);
				m_Reply = buffer[1];
			}

			size = 0;
			ret = 0;
		}
		PUB_Sleep(10);
	}
}
#else

void CMcu8952::ReadSerial()
{
	int size = 0;
	char buffer[5] = {0};
	char crc = 0;
	unsigned long eventValue=0;
	fd_set   rdfd;
	int readsize = 0;

	//���ó�ʱΪ2��
	struct timeval timeout;	
	timeout.tv_sec = 2;
	timeout.tv_usec = 0;

	int ret = 0;
	while(m_ThreadFlag)
	{
		//printf("%s():%s:%d, running at the MCU8952 thread\n", __FUNCTION__, __FILE__, __LINE__);
		memset(buffer, 0, 5);
		FD_ZERO(&rdfd);
		FD_SET(m_fd, &rdfd);

		ret = select(m_fd+1, &rdfd, NULL, NULL, &timeout);	

		if(ret > 0)
		{
			pthread_mutex_lock( &m_com_mutex );
			size += read(m_fd, buffer+size, 5-size);
			PUB_Sleep(20);
			if ( (0x0a != buffer[0]) && (0xa0 != buffer[0]) ) 
			{
				pthread_mutex_unlock( &m_com_mutex );
				size = 0;
				ret = 0;
				continue;
			}

			if ( 5 != size) 
			{
				//�������5���ֽڵ�����
				pthread_mutex_unlock( &m_com_mutex );
				continue;
			}

			pthread_mutex_unlock( &m_com_mutex );

			//����У��ֵ
			crc = buffer[0] + buffer[1] + buffer[2] + buffer[3];

			//�յ�MCU�ϱ����¼�
			if ((0x0a ==buffer[0]) && (buffer[4] == crc)) 
			{
				switch( buffer[1])
				{
					//�ϱ�������״̬
				case 5:
					if( m_Alarm_get_flag)
					{
						m_Alarm = ( buffer[3] | ( buffer[2] << 8 ) );
						m_Alarm_get_flag = 0;	
					}
					break;
					//�ϱ�MCU �汾
				case 6:
					m_VersionChip = ( (buffer[3]<<8) | buffer[2] );
					//printf("%s():%s:%d, get the MCU version\n", __FUNCTION__, __FILE__, __LINE__);
					break;		
					//�ϱ�Ӳ���汾
				case 7:
					m_VersionHard = ( (buffer[2]<<8) | buffer[3] );	
					//printf("%s():%s:%d, get the hardware version\n", __FUNCTION__, __FILE__, __LINE__);						
					break;
					//ң�ػ�����尴��������
				default:
					eventValue = ((buffer[3] << 16) | (buffer[2] << 8) | buffer[1]);
					//printf("%s():%s:%d, get eventValue=0x%08x\n", __FUNCTION__, __FILE__, __LINE__, eventValue );
					AddEventList(eventValue);
					break;

				}

			}
			//�յ�MCU��д���ڵĻ�Ӧ
			else if ((0xa0 ==buffer[0]) && (buffer[4] == crc)) 
			{
				//printf("Receive a write reply: %d\n", buffer[1]);
				m_Reply = buffer[1];
			}

			size = 0;
			ret = 0;
		}
		PUB_Sleep(10);
	}
}


#endif

void CMcu8952::AddEventList(unsigned long value)
{
	m_ListLock.Lock();

	{
		//���ڷ�����ڵ�Ƭ���л���ܶ�ֵ��Ϊ�˱ȽϺõĿ���Ч��
		//�ڷ����µķ���ֵʱ��ֱ�Ӱѻ���ļ�ֵȫ����ա�
		if ((0x50 <= value) && (value <= 0x61))
		{
			m_MEventList.RemoveAll();
		}
	}

	while(m_MEventList.GetCount() >= LIST_KEYVALUE_NUM)
	{
		m_MEventList.RemoveHead();
	}

	m_MEventList.AddTail(value);
	m_ListLock.UnLock();
}

bool CMcu8952::GetMCUKeyInfo(KEY_VALUE_INFO *pKeyInfo)
{
	unsigned long keyValue = 0xFF;
	BYTE tmpValue = 0;

	m_ListLock.Lock();
	if(!m_MEventList.IsEmpty())
	{
		keyValue = m_MEventList.RemoveHead();
	}
	else
	{
		m_ListLock.UnLock();
		return false;
	}
	m_ListLock.UnLock();

	//��0-7λ���¼�����, 8-15λ���¼�ֵ, 16-23λ�Ǳ�ʾ�Ƿ񳤰���
	pKeyInfo->isDown	= (keyValue >> 16) & 0xFF;
	pKeyInfo->value		= (keyValue >> 8) & 0xFF;
	pKeyInfo->xPos		= 0;
	pKeyInfo->yPos		= 0;

	tmpValue = keyValue & 0xFF;
	
	//������Ϣ
	if ((1 == tmpValue) || (3 == tmpValue)) 
	{
		pKeyInfo->type = KEY_TYPE_LOCAL;
	}

	//ң����Ϣ
	if (2 == tmpValue) 
	{
#ifdef __REMOTE_TR12__
		pKeyInfo->type = KEY_TYPE_REMOTE_TR12;
#else
		pKeyInfo->value |= 0x80;
		pKeyInfo->type = KEY_TYPE_REMOTE;
#endif
	}
	
	return  true;
}

bool CMcu8952::Start()
{
#if defined(__X_LINUX__)
	return true;
#endif

	m_ThreadHandle = PUB_CreateThread(ThreadKeyboardProc, (void *)this, &m_ThreadFlag);
	if( PUB_CREATE_THREAD_FAIL == m_ThreadHandle )
	{
		PUB_PrintError(__FILE__,__LINE__);
		return false;
	}

	bool retval = Operation(SERIAL_CTRL485,0);
	if( false == retval )
	{
		printErrorLocation("can not start the MCU serial port.")
	}

	return true;
}

void CMcu8952::Stop()
{
#if defined(__X_LINUX__)
	return;
#endif

	int iCount = 5;
	m_ThreadFlag = 0;

	while( iCount-- )
	{
		usleep(200);
	}
	
	if(PUB_THREAD_ID_NOINIT != m_ThreadHandle)
	{
		PUB_ExitThread(&m_ThreadHandle, &m_ThreadFlag);
	}
}


	//��ȡMCU �����������
int CMcu8952::GetMCUSerialConfig( struct termios *pOptions)
{

}

	//��ӡMCU����������Ϣ
void CMcu8952::PrintMCUSerialConfig(struct termios *pOptions)
{
	int i;
	
	printf("%s():%s:%d, get the MCU serial configuration\n", __FUNCTION__, __FILE__, __LINE__);

	if( NULL != pOptions )
	{
		printf("c_iflag = 0x%08x     c_oflag = 0x%08x\n", pOptions->c_iflag, pOptions->c_oflag );
		printf("c_cflag = 0x%08x     c_lflag = 0x%08x\n", pOptions->c_cflag, pOptions->c_lflag );
		printf("c_line = 0x%02x \n", pOptions->c_line );
	}

	for( i=0; i<NCCS; i++ )
	{
		if( 0 == ( i%5 ) )
			printf("\n");
		
		printf("c_cc[%d] = 0x%02x    ",i,pOptions->c_cc[i]);
	}
	printf("\n");

	printf("%s():%s:%d, get the MCU serial configuration OK\n", __FUNCTION__, __FILE__, __LINE__);
	
}

//end


