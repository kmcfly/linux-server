#include "SerialPort.h"
#include "dvr_func.h"

CSerialPort * CSerialPort::Instance()
{
	static CSerialPort s_SerialPort;
	return &s_SerialPort;
}

CSerialPort::CSerialPort()
{
	m_fdCom = -1;
	m_fp = NULL;
	m_SetBaudrate = 0;
	m_writeflag = 0;
	m_bCtrl485CS = false;

}

CSerialPort::~CSerialPort()
{
	
}

bool CSerialPort::Initial(bool bCtrl485CS)
{
#if defined(__CHIPTI__)
	return true;
#endif
	bool retval;

//��ʼ������ttys1ʱ�����
	while(-1 == m_fdCom) 
	{
		m_fdCom = open(dev2,  O_RDWR|O_NOCTTY);
		assert (-1 != m_fdCom);
		if( -1 == m_fdCom )
		{
			printErrorLocation("open the /dev/ttys1 error");
			return false;
		}

		m_fp = fdopen(m_fdCom, "r+b");
		if( NULL == m_fp )
		{
			Quit();
			printErrorLocation("fdopen() the ttys1 error");
			return false;
		}

	//Ĭ�ϴ��ڲ�������Ϊ:
	//8λ����λ��1λֹͣλ����У��λ��������9600��
	//Ĭ��Ϊ������
		retval = SetSerialParity(8, 1, 'N');
		retval &= SetSerialSpeed(SBR_9600);
		if( false == retval )
		{
			Quit();
			printErrorLocation("set parameter of ttys1 error");
			return false;
		}
		m_SetBaudrate = SBR_9600;
		m_writeflag = 0;

	}

	m_bCtrl485CS = bCtrl485CS;

	if (m_bCtrl485CS)
	{
		ext_led_set(9,0);
	}

	return true;
}


void CSerialPort::Quit()
{

#if defined(__CHIPTI__)
	return;
#endif

	int retval = -1;

	while(-1 != m_fdCom) 
	{
		retval = close(m_fdCom);
		if( -1 == retval )
		{
		//�ر��ļ�����
			if( EINTR == errno )
				continue;
			if( EBADF == errno )
			{
				printErrorLocation("close the ttys1 error,the file descriptor is wrong");
				assert(0);
			}
		}
		m_fdCom = -1;
	}

	m_bCtrl485CS = false;
}

bool CSerialPort::SetSerialParity(int databits, int stopbits, int parity)
{
#if defined(__CHIPTI__)
	return true;
#endif

	struct termios options;
	if  ( tcgetattr( m_fdCom,&options)  !=  0)
	{
		printf("%s():%s:%d, the m_fdCom = %d\n", __FUNCTION__, __FILE__, __LINE__, m_fdCom);
		perror("SetupSerial 1");
		return false;
	}

//��������λ
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
			options.c_cflag &= ~PARENB;   /* Clear parity enable */
			options.c_iflag &= ~INPCK;     /* Enable parity checking */
			break;
		case 'o':
		case 'O':
			options.c_cflag |= (PARODD | PARENB); /* ����Ϊ��У��*/
			options.c_iflag |= INPCK;             /* Disnable parity checking */
			break;
		case 'e':
		case 'E':
			options.c_cflag |= PARENB;     /* Enable parity */
			options.c_cflag &= ~PARODD;   /* ����ΪżУ��*/
			options.c_iflag |= INPCK;       /* Disnable parity checking */
			break;
		case 'S':
		case 's':  /*as no parity*/
			options.c_cflag &= ~PARENB;
			options.c_cflag &= ~CSTOPB;
			break;
		default:
			printErrorLocation("Unsupported pariy");
			return false;
	}

	// ����ֹͣλ
	switch (stopbits)
	{
		case 1:
			options.c_cflag &= ~CSTOPB;
			break;
		case 2:
			options.c_cflag |= CSTOPB;
			break;
		default:
			printErrorLocation("Unsupported stop bits");
			return false;
	}

	/* Set input parity option */
	if (parity != 'n')
		options.c_iflag |= INPCK;

	//mj ������ǿ����ն�֮��Ļ�,ֻ�Ǵ����������,������Ҫ����������,
	//mj ��ôʹ��ԭʼģʽ(Raw Mode)��ʽ��ͨѶ,���÷�ʽ����: 
	options.c_lflag  &= ~(ICANON | ECHO | ECHOE | ISIG); // Input   
	options.c_oflag  &= ~OPOST;                          // Output
	options.c_iflag &= ~(ICRNL | IXON); //debug 0x0d,0x11?0x13

	//options.c_lflag  &= ~(ICANON | ECHO | ECHOE | ISIG|ECHONL); // Input   
	//options.c_oflag  &= ~OPOST;									// Output
	//options.c_iflag &= ~(ICRNL|IXON|IXOFF|IXANY|INLCR);				//debug 0x0d,0x11?0x13
	//options.c_iflag &= ~IGNPAR;									//����У�����
	//options.c_iflag |=	IGNCR;									//�����Իس����з�
	//options.c_cflag |=	CREAD;									//�������
	//options.c_cflag |=	CRTSCTS;									//������
	

	tcflush(m_fdCom,TCIFLUSH);
	options.c_cc[VTIME] = 10; 		/* ���ó�ʱ10 seconds*/
	options.c_cc[VMIN] = 5; 		/* ��������ֽڴ��ھ͸����ϲ�Ӧ�ó����豸�ɶ� */

	if (tcsetattr(m_fdCom,TCSANOW,&options) != 0)
	{
		printErrorLocation("SetupSerial 3");
		return false;
	}

	return true;
}


//������ʱ���øú���
bool CSerialPort::SetSerialParityRead(int readlength, int stopbits, int parity,BYTE ttydatalength)
{
#if defined(__CHIPTI__)
	return true;
#endif

	struct termios options;

	if( ( 7 != ttydatalength) && ( 8 != ttydatalength))
	assert(0);

	if  ( tcgetattr( m_fdCom,&options)  !=  0)
	{
		perror("SetupSerial 1");
		return false;
	}

	options.c_cflag &= ~CSIZE;
	switch (ttydatalength) /*��������λ��*/
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
#if 1
	switch (parity)
	{
	case 'n':
	case 'N':
		options.c_cflag &= ~PARENB;   /* Clear parity enable */
		options.c_iflag &= ~INPCK;     /* Enable parity checking */
		break;
	case 'o':
	case 'O':
		options.c_cflag |= (PARODD | PARENB); /* ����Ϊ��У��*/
		options.c_iflag |= INPCK;             /* Disnable parity checking */
		break;
	case 'e':
	case 'E':
		options.c_cflag |= PARENB;     /* Enable parity */
		options.c_cflag &= ~PARODD;   /* ����ΪżУ��*/
		options.c_iflag |= INPCK;       /* Disnable parity checking */
		break;
	case 'S':
	case 's':  /*as no parity*/
		options.c_cflag &= ~PARENB;
		options.c_cflag &= ~CSTOPB;
	break;
	default:
		printErrorLocation("Unsupported parity");
		return false;
	}
#endif

	/* ����ֹͣλ*/
	switch (stopbits)
	{
	case 1:
		options.c_cflag &= ~CSTOPB;
		break;
	case 2:
		options.c_cflag |= CSTOPB;
		break;
	default:
		printErrorLocation("Unsupported stop bits\n");
		return false;
	}

	/* Set input parity option */
	if (parity != 'n')
		options.c_iflag |= INPCK;

	//mj ������ǿ����ն�֮��Ļ�,ֻ�Ǵ����������,������Ҫ����������,
	//mj ��ôʹ��ԭʼģʽ(Raw Mode)��ʽ��ͨѶ,���÷�ʽ����: 
	options.c_lflag  &= ~(ICANON | ECHO | ECHOE | ISIG); // Input   
	options.c_oflag  &= ~OPOST;                          // Output

	tcflush(m_fdCom,TCIFLUSH);
	options.c_cc[VTIME] =0; 		
	options.c_cc[VMIN] = 1; 		/*��һ���ֽھͱ���������*/

	if (tcsetattr(m_fdCom,TCSANOW,&options) != 0)
	{
		printErrorLocation("SetupSerial 3");
		return false;
	}

	return true;
}

bool CSerialPort::SetSerialSpeed(PORT_BAUDRATE bandrate)
{
#if defined(__CHIPTI__)
	return true;
#endif
	
	int   status;
	unsigned long rate;
	struct termios   Opt;

	//m_PortLock.Lock();
	
	if ( m_SetBaudrate!=bandrate )
	{	
		if (-1 == m_fdCom) 
		{
			if (!Initial(m_bCtrl485CS))
			{
				return false;
			}
		}

		assert (-1 != m_fdCom);

		rate = GetBaudRate(bandrate);
		tcgetattr(m_fdCom, &Opt);
		tcflush(m_fdCom, TCIOFLUSH);
		cfsetispeed(&Opt, rate);
		cfsetospeed(&Opt, rate);
		status = tcsetattr(m_fdCom, TCSANOW, &Opt);
		if  (status != 0)
		{
			printErrorLocation("tcsetattr fd1");
			//m_PortLock.UnLock();
			
			return false;
		}
		tcflush(m_fdCom,TCIOFLUSH);

		//���óɹ����´��ڲ�����
		m_SetBaudrate = bandrate;
		//m_PortLock.UnLock();
		
		return true;
	}
	else
	{
		//m_PortLock.UnLock();
		
		return true;
	}
}


bool CSerialPort::WriteToPort(const BYTE *pBuf,int len,PORT_BAUDRATE bandrate)
{
#if defined(__CHIPTI__)
	return true;
#endif
	
	//�鿴���뻺�����Ƿ������ݣ� �����ݾ�Ҫ�ȴ�����ſ���
	//���ǲ��ܱ�֤���е����ݶ���ץ��
	int hasInput = 0;
	if ((0 <= ioctl(m_fdCom, FIONREAD, &hasInput)) && (0 < hasInput))
	{
#if defined(__PTZ_GE__)
		PUB_Sleep(5);
#else
		PUB_Sleep(10);
#endif
	}
	
	m_PortLock.Lock();

	if (m_bCtrl485CS)
	{
		ext_led_set(9,1);
	}
	

	bool res = SetSerialSpeed(bandrate);

	//��Щ����û�ı䣬����Ҫ���� zhl 10-06-05
	//SetSerialParity(8, 1, 'N');			
		
	if (!res)
	{
		if (m_bCtrl485CS)
		{
			ext_led_set(9,0);
		}		
		m_PortLock.UnLock();
		return false;
	}
#if defined(__PTZ_GE__)
	int iSleepTime = 5; //����Ϊ��λ��������̵��Ȼ��˷���10����
#else
	int iSleepTime = 10; //����Ϊ��λ��������̵��Ȼ��˷���10����
#endif
	
	switch(bandrate)
	{
	case SBR_110:
		iSleepTime += len * 1000 * 8 / 110 ;
		break;
	case SBR_300:
		iSleepTime += len * 1000 * 8 / 300 ;
		break;
	case SBR_600:
		iSleepTime += len * 1000 * 8 / 600 ;
		break;
	case SBR_1200:
		iSleepTime += len * 1000 * 8 / 1200 ;
		break;
	case SBR_2400:
		iSleepTime += len * 1000 * 8 / 2400 ;
		break;
	case SBR_4800:
		iSleepTime += len * 1000 * 8 / 4800 ;
		break;
	case SBR_9600:
		iSleepTime += len * 1000 * 8 / 9600 ;
		break;
	case SBR_19200:
		iSleepTime += len * 1000 * 8 / 19200 ;
		break;
	case SBR_38400:
		iSleepTime += len * 1000 * 8 / 38400 ;
		break;
	case SBR_57600:
		iSleepTime += len * 1000 * 8 / 57600 ;
		break;
	case SBR_115200:
		iSleepTime += len * 1000 * 8 / 115200 ;
		break;
	case SBR_230400:
		iSleepTime += len * 1000 * 8 / 230400 ;
		break;
	case SBR_460800:
		iSleepTime += len * 1000 * 8 / 460800 ;
		break;
	case SBR_921600:
		iSleepTime += len * 1000 * 8 / 921600 ;
		break;
	default :
		iSleepTime += len * 1000 * 8 / 110 ;
		break;
	}
	int iSave = fcntl(m_fdCom,F_GETFL);
	fcntl(m_fdCom, F_SETFL, iSave & (~O_NONBLOCK));
	int ret1 = write(m_fdCom, pBuf, len);
#if defined(__TDFH__)
	//�ȴ����ݷ������
	tcdrain(m_fdCom);
#endif
	m_writeflag = 1;

	if (ret1==len) 
	{
		PUB_Sleep(iSleepTime);
		if (m_bCtrl485CS)
		{
			ext_led_set(9,0);
		}		
		m_PortLock.UnLock();
		return true;
	}
	else
	{
		PUB_Sleep(10);
		int ret2 = write(m_fdCom, pBuf+ret1, len-ret1);
#if defined(__TDFH__)
		//�ȴ����ݷ������
		tcdrain(m_fdCom);
#endif
		PUB_Sleep(iSleepTime);
		if (m_bCtrl485CS)
		{
			ext_led_set(9,0);
		}
		m_PortLock.UnLock();
		
		if (ret2==len-ret1)
		{
			return true;
		}
		return false;
	}
	
}

#ifdef SUPPORT_PORTCTRL_DVR
 
//��������
//����:��������BUFFER,�����ݴ�С,ʵ�ʶ��������ݴ�С
int CSerialPort::ReadPort(BYTE *pBuf,int len,PORT_BAUDRATE baudrate)
{
#if defined(__CHIPTI__)
	return 0;
#endif
	if (-1 == m_fdCom)
	{
		return 0;
	}

	fd_set serial;
	int retval = 0;
	int ret1 = 0;
	bool ret =true;

	if( NULL == m_fp )
	{
		printf("%s():%s:%d, the ttys1 m_fp is NULL��have not initial the ttys1 device\n", __FUNCTION__, __FILE__, __LINE__);
		return 0;
	}

	//������Ϊ��ģʽ
	m_PortLock.Lock();
	//�����ʻ��߶�д��ʽ�ı�ʹ������ô��ڲ���
	if((((PORT_BAUDRATE)m_SetBaudrate) != baudrate) || (m_writeflag == 1) )
	{
		ret 	= SetSerialSpeed(baudrate);

		//��Щ����û�ı䣬����Ҫ����zhl 10-06-05
		//if( true == ret )
		//{
		//	ret &=SetSerialParity( 8, 1,  'N');
		//}
	}	

	if( !ret  )
	{
		m_PortLock.UnLock();
		printErrorLocation("Set 485 error");
		return 0; 
	}
	m_writeflag = 0;

	//����485�ӿڴ���������
	memset((char *)pBuf, 0, len);	
	while( len != ret1)
	{
		//ÿ��selectǰ��Ҫ�������ó�ʱʱ��
		struct timeval timeout = {0,20000};   
		FD_ZERO(&serial);
		FD_SET(m_fdCom, &serial);
		if ((retval = select(m_fdCom+1,  &serial, NULL, NULL,&timeout)) <=0)
		{
			m_PortLock.UnLock();			
			return 0;	
		}
		//������дBUFFER
		if(FD_ISSET(m_fdCom,&serial)!=0)
		{
			ret1 += read( m_fdCom, &pBuf[ret1], len-ret1);
		}
	}

	m_PortLock.UnLock();	
	retval = ret1;
	ret1 = 0;
	//int i;
	//for( i=0; i<retval; i++)
	//printf("%02x    ", pBuf[i]);
	//printf("\n");

	return retval;
}

#endif


