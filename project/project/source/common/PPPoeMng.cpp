//////FileManager.cpp

#include "PPPoeMng.h"


CPPPOEMng::CPPPOEMng()
{
	m_ThreadHandle = 0;
	m_IsAutoLogin = false;
	m_HasLonginSuc = false;
	m_WaiteStop.CreateWaitEvent();

	m_pIfName=new char[MaxLengthIfName];
	assert( NULL!= m_pIfName);
	memset(m_pIfName,0,MaxLengthIfName);
}

CPPPOEMng::~CPPPOEMng()
{
	if( NULL != m_pIfName )
	{
		delete [] m_pIfName;
		m_pIfName = NULL;
	}
}

CPPPOEMng* CPPPOEMng::Instance()
{

	static CPPPOEMng s_CPppoEMng;
	
	return &s_CPppoEMng;
}


bool CPPPOEMng::SetUserInfo(const char *userName, const char *passWord)
{	
	char strCommand[300] = {0};				
	sprintf(strCommand,"%s %s %s",pppoe_setup,userName,passWord);

	m_MuxAcessFile.Lock();

	int result=DVRSystem(strCommand);

	m_MuxAcessFile.UnLock();

	if ( 0!=result)
	{
		return false;
	}

	return true;
}

bool CPPPOEMng::SetIfName()
{
	char path[100];
	memset(path,0,100);

	m_MuxAcessFile.Lock();
	FILE *pFIfName= fopen(ifName,"r");
	if( NULL != pFIfName )
	{
		fgets(path,100,pFIfName);
		fclose(pFIfName);
	}
	m_MuxAcessFile.UnLock();

	if (NULL == pFIfName) 
	{
		return false;
	}

	path[99]='\0';
	if ( path[0]=='\0' ) 
	{
		return false;
	}

	char *p=strstr(path,"ppp");
	if ( NULL==p )
	{
		return false;
	}

	//if last word is space, change it to be '\0'
	if(isspace(*(p+strlen(p)-1)))
	{
		*(p+strlen(p)-1) = 0;
	}
	memset(m_pIfName,0,MaxLengthIfName);
	memcpy(m_pIfName,p,strlen(p));
	return true;
}

char* CPPPOEMng::GetIfName() const
{
	if (!m_HasLonginSuc) 
	{
		return NULL;
	}
	return m_pIfName;
}


void CPPPOEMng::Initial()
{
	//杀掉所有PPPOE相关的进程
	killProcessByName(const_cast<char *>("/bin/sh"),"/usr/sbin/pppoe-start");
	killProcessByName(const_cast<char *>("/bin/sh"), "/usr/sbin/pppoe-connect");
	killProcessByName(const_cast<char *>("/use/sbin/pppd"), NULL);
	killProcessByName(const_cast<char *>("/usr/sbin/pppoe"), NULL);
	//删除/var/run中的所有文件
#if defined(__X_LINUX__)
	DVRSystem("rm -rf /var/run/*");
#endif
}

bool CPPPOEMng::Login()
{
	m_MuxAcessFile.Lock();

	int result=DVRSystem(pppoe_start);

	m_MuxAcessFile.UnLock();

	if ( 0!=result)
	{
		return false;
	}
	
	

	return true;
}
bool CPPPOEMng::LoginBG()
{

	m_MuxAcessFile.Lock();

	int result=DVRSystem(pppoe_start_bg);

	m_MuxAcessFile.UnLock();

	if ( 0!=result)
	{
		return false;
	}

	return true;
}

bool CPPPOEMng::Logout()
{
	m_MuxAcessFile.Lock();

	int result=DVRSystem(pppoe_stop);

	m_MuxAcessFile.UnLock();

	if ( 0!=result)
	{
		return false;
	}

	return true;
}

bool  CPPPOEMng::StartAutoLogin()
{
	//create a thread to check PPP connection, if  it is down, connect it again!
	m_ThreadHandle = PUB_CreateThread(AutoLogin, (void *)this, &m_IsAutoLogin);	
	if( PUB_CREATE_THREAD_FAIL == m_ThreadHandle )
	{
		PUB_PrintError(__FILE__,__LINE__);
		return false;
	}
	return true;
}

void* CPPPOEMng::AutoLogin(LPVOID lpParameter)
{
	PUB_PRINTF_INFO("%s %d pid = %d\n",__FILE__, __LINE__, getpid());
	CPPPOEMng* pInstance=(CPPPOEMng*)lpParameter;
	pInstance->LoginBG();
	pInstance->m_WaiteStop.WaitForCond(INFINITE);
	return 0;
}

bool CPPPOEMng::Start(const char *userName, const char *passWord)
{
	//save user name and password
	if( !SetUserInfo(userName,passWord) )
	{
		return false;		
	}

	if(!StartAutoLogin())
	{
		return false;
	}
	
	return true;	
}

bool  CPPPOEMng::Stop()
{
	Logout();
	Initial();

	//if (Logout()) 
	{
		if (m_IsAutoLogin) 
		{
			m_WaiteStop.SetEvent();

			if (PUB_THREAD_ID_NOINIT != m_ThreadHandle)
			{		
				PUB_ExitThread(&m_ThreadHandle, &m_IsAutoLogin);	
			}
		}
		return true;
	}
	return false;
}

int CPPPOEMng::TestPppoe( const char *userName, const char *passWord )
{	
	if( !SetUserInfo(userName,passWord) )
	{
		return 0;		
	}

	Login();
	
	int retval = IsPppoeRunning();

	Logout();

	Initial();
	
	return retval;
}


/***************************************************

判断PPPOE是否正在运行中。
	查看调用pppoe-status后的输出文件中的信息可以判断
PPPOE是否在运行中
zhl

***************************************************/

int CPPPOEMng::IsPppoeRunning()
{
	DVRSystem(pppoe_status);

	FILE *pPPPStatus = fopen( pppoe_status_file, "r" );
	if( NULL == pPPPStatus )
	{
		perror("fopen()");
		assert( false );
		return PPPOE_STATUS_UNKNOW ;
	}
	
	fseek( pPPPStatus, 0, SEEK_END );
	long fileLen = ftell( pPPPStatus );
	char *pFileContents = (char *)malloc( fileLen + 10 );
	if( NULL != pFileContents )
	{
		memset( pFileContents, 0, fileLen + 10 );
	}
	else
	{
		assert( false );
		fclose( pPPPStatus );
		return PPPOE_STATUS_UNKNOW;
	}
	
	char *pstatus = NULL;
	char *pConnect = NULL;

	int isConnected=-1;
	
	fseek( pPPPStatus, 0, SEEK_SET );
	if( 1 != fread( pFileContents, fileLen, 1, pPPPStatus ) )
	{
		ferror(pPPPStatus);
		if( NULL != pFileContents )
		{
			free(pFileContents);
			pFileContents = NULL;
		}
		assert( false );
		fclose( pPPPStatus );
		return PPPOE_STATUS_UNKNOW;
	}
	fclose(pPPPStatus);

	pstatus = strstr( pFileContents, "pppoe-status:");
	if( NULL == pstatus )
	{
		assert( false );
		if( NULL != pFileContents )
		{

			free(pFileContents);
			pFileContents = NULL;
		}
		return PPPOE_STATUS_UNKNOW;
	}

	pConnect = strstr( pFileContents, "$?=");
	if( NULL != pConnect )
	{
		if( 1 != sscanf( pConnect ,"$?=%d", &isConnected ) )
		{
			assert( false );
			if( NULL != pFileContents )
			{
				free(pFileContents);
				pFileContents = NULL;
			}
			return PPPOE_STATUS_UNKNOW;
		}
		
	}
	else
	{
		isConnected = 1;
	}

	//没有运行pppoe
	if( NULL != strstr( pstatus, "Link is down (can't read pppoe PID file" ) )
	{
		if( NULL != pFileContents )
		{
			free(pFileContents);
			pFileContents = NULL;
		}
		return PPPOE_STATUS_STOP;
	}

	//运行了PPPOE
	if( !isConnected )
	{
		if( NULL != pFileContents )
		{
			free(pFileContents);
			pFileContents = NULL;
		}
		return PPPOE_STATUS_VALID;
	}
	else
	{
		if( NULL != strstr( pstatus ,"attached to ppp0") )
		{
			if( NULL != pFileContents )
			{
				free(pFileContents);
				pFileContents = NULL;
			}
			return PPPOE_STATUS_RUNNING;
		}

		if( NULL != strstr( pstatus, "Link is up" ) )
		{
			if( NULL != pFileContents )
			{
				free(pFileContents);
				pFileContents = NULL;
			}
			return PPPOE_STATUS_VALID;
		}

		if( NULL != strstr( pstatus, "Link is down -- could not find interface corresponding") )
		{
			if( NULL != pFileContents )
			{
				free(pFileContents);
				pFileContents = NULL;
			}
			return PPPOE_STATUS_RUNNING;
		}
	}

	assert( false );
	
	if( NULL != pFileContents )
	{
		free(pFileContents);
		pFileContents = NULL;
	}

	return PPPOE_STATUS_UNKNOW;
}

