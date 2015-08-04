
#ifdef   __ENVIRONMENT_LINUX__
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#endif

#include <stdio.h>
#include <errno.h>
#include <string.h>
#include "SWL_Public.h"
#include <assert.h>

#ifdef __ENVIRONMENT_WIN32__
void PrintDebugInfo(const char *pText, DWORD dwError)
{
#if defined(_DEBUG)
	if(dwError == 0)
	{
		return;
	}
	FILE *pFile = fopen("C:\\DebugInfo.log", "ab+");
	char szText[256];
	sprintf(szText, "%s %d: ",pText, dwError);
	fwrite(szText, strlen(szText)+1, 1, pFile);
	LPVOID lpMsgBuf;
	int nLength = FormatMessage( 
		FORMAT_MESSAGE_ALLOCATE_BUFFER | 
		FORMAT_MESSAGE_FROM_SYSTEM | 
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		dwError,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
		(LPTSTR) &lpMsgBuf,
		0,
		NULL 
		);
	fwrite((LPCTSTR)lpMsgBuf, nLength+1, 1, pFile);
	// Free the buffer.
	LocalFree( lpMsgBuf );
	strcpy(szText, "\r\n");
	fwrite(szText, strlen(szText)+1, 1, pFile);
	fclose(pFile);

#endif /////#if defined(_DEBUG)  end
}
#endif

static unsigned long s_dwInit = 0;

//return value: 0 �ɹ� 1 ʧ��
int SWL_Init()
{
	if(s_dwInit > 0)
	{
		++s_dwInit;
		return 0;
	}

#ifdef  __ENVIRONMENT_WIN32__
	//windows����Ҫ��WS2_32.DLL����г�ʼ��
	WSADATA wsaData;
	if(SOCKET_ERROR == WSAStartup(0x202,&wsaData)) {
		printf("%s %d last errno = %d\n", __FILE__, __LINE__, WSAGetLastError());
		return 1;
	}
#elif  defined  __ENVIRONMENT_LINUX__
	signal(SIGPIPE, SIG_IGN);
#endif
	++s_dwInit;
	return 0;
}

int SWL_Quit()
{
	if (s_dwInit > 0)
	{
		--s_dwInit;
	}
	else
	{
		return 0;
	}
#ifdef  __ENVIRONMENT_WIN32__
	//windows����Ҫ���WS2_32.DLL��
	if(SOCKET_ERROR == WSACleanup())
	{
		printf("%s %d last errno = %d\n", __FILE__, __LINE__, WSAGetLastError());
		return 1;
	}
#endif	
	return 0;
}

void SWL_PrintError(const char* pFile, int iLine)
{
#ifdef  __ENVIRONMENT_LINUX__
	char szErrorPos[256] = {0};
	sprintf(szErrorPos, "%s %d ", pFile, iLine);
	perror(szErrorPos);
#elif defined  __ENVIRONMENT_WIN32__
	int iErrno = WSAGetLastError();
	printf("%s %d Errno = %d\n", pFile, iLine, iErrno);
#endif 
}
 
//return value: �ɹ�����SWL_UNINVALID_SOCKET  
//			    ʧ�ܣ�SWL_UNINVALID_SOCKET  
//���ʧ�ܣ��򲻻�ı� *pSocket��ֵ
SWL_socket_t SWL_CreateSocket(int iDomain, int iType, int iProtocol)
{
	SWL_socket_t sock = socket(iDomain, iType, iProtocol);
	if (SWL_INVALID_SOCKET == sock) 
	{
#ifdef __ENVIRONMENT_WIN32__
		PrintDebugInfo("SWL_socket_t sock = socket(iDomain, iType, iProtocol)", WSAGetLastError());
#endif
		return SWL_INVALID_SOCKET;
	}
	int  opt = 1;
	if(0 != setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(int)))
	{
		SWL_PrintError(__FILE__, __LINE__);
#ifdef __ENVIRONMENT_WIN32__
		PrintDebugInfo("setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(int))", WSAGetLastError());
#endif
		return SWL_INVALID_SOCKET;
	}

#ifdef  __ENVIRONMENT_LINUX__
	int iSave = fcntl(sock,F_GETFL);
	fcntl(sock, F_SETFL, iSave | O_NONBLOCK);
#elif defined  __ENVIRONMENT_WIN32__
	unsigned long dwVal = 1; //��0Ϊ������ģʽ
	ioctlsocket(sock, FIONBIO, &dwVal);
#endif
	return sock;
}

//return value: 0 �ɹ� -1 ʧ��
int SWL_CloseSocket(SWL_socket_t sock)
{
#ifdef        __ENVIRONMENT_WIN32__
	return closesocket(sock);
#elif defined __ENVIRONMENT_LINUX__
	return close(sock);
#endif
}

//�ر��Ѿ������ϵ�socket
//return value: 0 �ɹ� -1
//���������iHow��	SWL_SHUT_RD �ر�ʱ�������Ķ�ȡ��������ֹ
//					SWL_SHUT_WR	�ر�ʱ�������ķ��Ͳ�������ֹ
//					SWL_SHUT_BOTH �ر�ʱ�������Ķ�ȡ�ͷ��Ͳ�������ֹ
//ֻҪ������shutdown������iHow���ĸ����������Ӷ����Ͽ���
//BUGS:���iHowΪboth��recv�ķ���ֵ�Ƿ���Ϊ0��
//��ʵ������shutdownҲ���ԣ�close�ˣ��Է���Ȼ��֪����ֻ������TCP��һ��ȫ˫��
//��ͨ�����ر�ʱѡ��ص�send��ͨ���ȣ����Ա�֤���еķ��ͳ�ȥ�˵����ݶ��յ��ˣ�
//Ȼ���ٹر�
int SWL_Shutdown(SWL_socket_t sSock, int iHow)
{
	return shutdown(sSock, iHow);
}

//��socket
//return valude: 0 �ɹ� 
//				 SWL_SOCKET_ERROR ʧ��
int SWL_Bind(SWL_socket_t sock, const struct sockaddr *pInfo, SWL_socklen_t len)
{
	return bind(sock, pInfo, len);
}

//����socket
//return value: 0 �ɹ� 
//				SWL_SOCKET_ERROR ʧ��
int SWL_Listen(SWL_socket_t sock, int iBacklog)
{
	return listen(sock, iBacklog);
}

//return value: �ɹ�������socket
//				ʧ�ܣ�SWL_INVALID_SOCKET
int SWL_Accept(SWL_socket_t sock, struct sockaddr *pAddr, SWL_socklen_t *pLen)
{
	return accept(sock, pAddr, pLen);
}

//��������
//return value: >=0 ���ͳ������ݳ��ȣ� 
//              SWL_SOCKET_ERROR ʧ��
//��������� pBuf Ҫ���͵����ݵ�ָ��  iLen Ҫ���͵����ݵĳ���  iFlags ������
//BUGS: linux�µ�sendԭ��Ϊ	  ssize_t send(int s, const void *buf, size_t len, int flags);
//		windows�µ�sendԭ��Ϊ int send(SOCKET s, const char FAR *buf,   int len,  int flags);
//      ����û�а�windows��linux�µ���������ͳһ������ֱ�Ӻ��������ͣ�ʹ����int
// int SWL_Send(SWL_socket_t sock, const void* pBuf, int iLen, int iFlags)
// {
// #ifdef            __ENVIRONMENT_WIN32__
// 	return send(sock, static_cast<const char*>(pBuf), iLen, 0);
// #elif defined     __ENVIRONMENT_LINUX__
// 	return static_cast<int>(send(sock, pBuf, static_cast<size_t>(iLen), MSG_DONTWAIT | MSG_NOSIGNAL));
// #endif
// }

//��������
//return value: >0 �ɹ��յ��������� SWL_SOCKET_ERROR �д����� 0 �Է�socket�����ر�
//BUGS:linux��ԭ��Ϊ ssize_t recv(int s, void *buf, size_t len, int flags);
//     windows��ԭ�� int recv(SOCKET s, char FAR *buf, int len, int flags);
//      ����û�а�windows��linux�µ���������ͳһ������ֱ�Ӻ��������ͣ�ʹ����int
// int SWL_Recv(SWL_socket_t sock, void* pBuf, int iLen, int iFlags)
// {
// #ifdef            __ENVIRONMENT_WIN32__
// 	return recv(sock, static_cast<char*>(pBuf), iLen, 0);
// #elif defined     __ENVIRONMENT_LINUX__
// 	return static_cast<int>(recv(sock, pBuf, static_cast<size_t>(iLen), MSG_DONTWAIT));
// #endif
// }

//�첽����
//iTimeOut ��Ϊ����Ϊ��λ������connect���Ƿ�ȴ���Ϊ0����������
int SWL_Connect(SWL_socket_t sock, const struct sockaddr *pServAddr, SWL_socklen_t len, int iTimeOut)
{
	//�����棬��������socket�Ѿ����ó����첽�������ֻ�Ǵ���һ�����Ӷ���
	//��û�е���������ȥ����
	if(0 != connect(sock, pServAddr, len))
	{
#ifdef         __ENVIRONMENT_WIN32__
		int iErrno = WSAGetLastError();
		if (WSAEISCONN  == iErrno) 
		{
			WSASetLastError(iErrno);
			return 0;
		}
		//���ж�WSAEINVAL���������£�
		//Note In order to preserve backward compatibility, this error is reported as WSAEINVAL to 
		//Windows Sockets 1.1 applications that link to either Winsock.dll or Wsock32.dll.
		else if((WSAEWOULDBLOCK != iErrno) && (WSAEALREADY != iErrno) && (WSAEINVAL != iErrno))
		{
			PrintDebugInfo("connect(sock, pServAddr, len)", WSAGetLastError());
			WSASetLastError(iErrno);
			return SWL_SOCKET_ERROR;
		}	
#elif defined  __ENVIRONMENT_LINUX__
		if (EISCONN == errno) 
		{
			return 0;
		}
		else if((EINPROGRESS != errno) && (EWOULDBLOCK != errno) && (EAGAIN != errno) && (EALREADY != errno))
		{
			return SWL_SOCKET_ERROR;
		}
#endif
	}

	//���sock�Ƿ�������
	int error;
	SWL_socklen_t error_len;

	int iRet = 0;
	fd_set  writeFds;
	FD_ZERO(&writeFds);
	FD_SET(sock, &writeFds);

	struct timeval	struTimeout;
	if ( 0 != iTimeOut)
	{
		struTimeout.tv_sec = iTimeOut / 1000;  //ȡ��
		struTimeout.tv_usec = (iTimeOut % 1000 ) * 1000; //ȡ΢��
		iRet = select(sock + 1, NULL, &writeFds, NULL, &struTimeout);
	}
	else
	{
		struTimeout.tv_sec = 0;
		struTimeout.tv_usec = 0;
		iRet = select(sock + 1, NULL, &writeFds, NULL, &struTimeout);
	}
		
	if ( 0 >= iRet ) //�쳣���߳�ʱ
	{
#ifdef __ENVIRONMENT_WIN32__
		PrintDebugInfo("select(sock + 1, NULL, &writeFds, NULL, &struTimeout);", WSAGetLastError());
#endif
		return SWL_SOCKET_ERROR;
	}
	else
	{
		if(FD_ISSET(sock, &writeFds))
		{
			error_len = sizeof(error);
			if (getsockopt(sock, SOL_SOCKET, SO_ERROR, (char *)&error, &error_len) < 0)
			{
				return SWL_SOCKET_ERROR;
			}

			if(error != 0)
			{
				return SWL_SOCKET_ERROR;
			}
			return 0;	//��Ϊֻ��һ��sock�����ж��Ƿ���writeFds����
		}
		else
		{
			assert(false);
			return SWL_SOCKET_ERROR;
		}
	}
}

//��The socket is marked as nonblocking and no connections are present to be accepted
//��������socket����ʱ��һ�Ṧ����û�����ݹ���
//��ʱ�򷵻�true
bool SWL_EWOULDBLOCK()
{
#ifdef            __ENVIRONMENT_WIN32__
	if( WSAEWOULDBLOCK == WSAGetLastError())
#elif defined     __ENVIRONMENT_LINUX__
	if ( (EWOULDBLOCK == errno) || (EAGAIN == errno) )
#endif
	{
		return true;
	}
	return false;
}
