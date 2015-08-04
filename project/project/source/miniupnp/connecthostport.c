/* $Id: connecthostport.c,v 1.13 2014/03/31 12:36:36 nanard Exp $ */
/* Project : miniupnp
 * Author : Thomas Bernard
 * Copyright (c) 2010-2014 Thomas Bernard
 * This software is subject to the conditions detailed in the
 * LICENCE file provided in this distribution. */

/* use getaddrinfo() or gethostbyname()
 * uncomment the following line in order to use gethostbyname() */
#ifdef NO_GETADDRINFO
#define USE_GETHOSTBYNAME
#endif

#include <string.h>
#include <stdio.h>
#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#include <io.h>
#define MAXHOSTNAMELEN 64
#define snprintf _snprintf
#define herror
#define socklen_t int
#else /* #ifdef _WIN32 */
#include <unistd.h>
#include <sys/param.h>
#include <sys/select.h>
#include <errno.h>
#define closesocket close
#include <netdb.h>
#include <netinet/in.h>
#include <fcntl.h>
/* defining MINIUPNPC_IGNORE_EINTR enable the ignore of interruptions
 * during the connect() call */
#define MINIUPNPC_IGNORE_EINTR
#ifndef USE_GETHOSTBYNAME
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#endif /* #ifndef USE_GETHOSTBYNAME */
#endif /* #else _WIN32 */

/* definition of PRINT_SOCKET_ERROR */
#ifdef _WIN32
#define PRINT_SOCKET_ERROR(x)    printf("Socket error: %s, %d\n", x, WSAGetLastError());
#else
#define PRINT_SOCKET_ERROR(x) perror(x)
#endif

#if defined(__amigaos__) || defined(__amigaos4__)
#define herror(A) printf("%s\n", A)
#endif

#include "connecthostport.h"

#ifndef MAXHOSTNAMELEN
#define MAXHOSTNAMELEN 64
#endif

#ifndef UPNP_SOCK_TIMEOUT
#define UPNP_SOCK_TIMEOUT		2000  // ms
#endif//UPNP_SOCK_TIMEOUT

#define MINIUPNPC_SET_SOCKET_TIMEOUT

static int SetSockNonBlock(int sock, int nonBlock)
{
#ifdef WIN32
	unsigned long dwVal = nonBlock; // not 0 is NON_BLOCK mode
	ioctlsocket(sock, FIONBIO, &dwVal);
#else
	int flags = fcntl(sock,F_GETFL);
	if (nonBlock)
	{
		flags |= O_NONBLOCK;
	}
	else
	{
		flags &= ~O_NONBLOCK;
	}
	fcntl(sock, F_SETFL, flags);
#endif
	return 0;
}

static int AsyncConnect(int sock, const struct sockaddr *pServAddr, socklen_t len, int iTimeOut)
{
	// the sock must has been set to NON_BLOCK mode
	int ret = 0;
	if( connect(sock, pServAddr, len) != 0 )
	{
#ifdef WIN32
		int iErrno = WSAGetLastError();
		if (WSAEISCONN  == iErrno) 
		{
			//WSASetLastError(iErrno);
			ret = 0;
		}
		
		//多判断WSAEINVAL的理由如下
		//Note In order to preserve backward compatibility, this error is reported as WSAEINVAL to 
		//Windows Sockets 1.1 applications that link to either Winsock.dll or Wsock32.dll.
		else if((WSAEWOULDBLOCK != iErrno) && (WSAEALREADY != iErrno) && (WSAEINVAL != iErrno))
		{
			//PRINT_SOCKET_ERROR("connect");
			//WSASetLastError(iErrno);
			ret = -1;
		}	
#else// for linux / unix
		if (EISCONN == errno) 
		{
			ret = 0;
		}
		else if((EINPROGRESS != errno) && (EWOULDBLOCK != errno) && (EAGAIN != errno) && (EALREADY != errno))
		{
			ret = -1;
		}
#endif
	}
	if(ret == 0)
	{
		//检查sock是否连接上
		int error;
		socklen_t error_len;
		struct timeval	struTimeout;

		int selectRet;
		fd_set  writeFds;
		FD_ZERO(&writeFds);
		FD_SET(sock, &writeFds);

		if ( 0 != iTimeOut)
		{
			struTimeout.tv_sec = iTimeOut / 1000;  //取秒
			struTimeout.tv_usec = (iTimeOut % 1000 ) * 1000; //取微妙
			selectRet = select(sock + 1, NULL, &writeFds, NULL, &struTimeout);
		}
		else
		{
			struTimeout.tv_sec = 0;
			struTimeout.tv_usec = 0;
			selectRet = select(sock + 1, NULL, &writeFds, NULL, &struTimeout);
		}



		if ( selectRet <= 0 ) //异常或者超时
		{
			PRINT_SOCKET_ERROR("connect select");
			ret = -1;
		}
		else
		{
			if(FD_ISSET(sock, &writeFds))
			{
				error_len = sizeof(error);
				if (getsockopt(sock, SOL_SOCKET, SO_ERROR, (char *)&error, &error_len) < 0)
				{
					ret = -1;
				}
				else if(error != 0)
				{
					ret = -1;
				}
				else
				{
					ret = 0;
				}
			}
			else
			{
//				assert(false);
				ret = -1;
			}
		}
	}

	return ret;
}

/* connecthostport()
 * return a socket connected (TCP) to the host and port
 * or -1 in case of error */
int connecthostport(const char * host, unsigned short port,
                    unsigned int scope_id)
{
	int s, n;
#ifdef USE_GETHOSTBYNAME
	struct sockaddr_in dest;
	struct hostent *hp;
#else /* #ifdef USE_GETHOSTBYNAME */
	char tmp_host[MAXHOSTNAMELEN+1];
	char port_str[8];
	struct addrinfo *ai, *p;
	struct addrinfo hints;
#endif /* #ifdef USE_GETHOSTBYNAME */
#ifdef MINIUPNPC_SET_SOCKET_TIMEOUT
	struct timeval timeout;
#endif /* #ifdef MINIUPNPC_SET_SOCKET_TIMEOUT */

#ifdef USE_GETHOSTBYNAME
	hp = gethostbyname(host);
	if(hp == NULL)
	{
		herror(host);
		return -1;
	}
	memcpy(&dest.sin_addr, hp->h_addr, sizeof(dest.sin_addr));
	memset(dest.sin_zero, 0, sizeof(dest.sin_zero));
	s = socket(PF_INET, SOCK_STREAM, 0);
	if(s < 0)
	{
		PRINT_SOCKET_ERROR("socket");
		return -1;
	}
#ifdef MINIUPNPC_SET_SOCKET_TIMEOUT
	/* setting a 3 seconds timeout for the connect() call */
	timeout.tv_sec = UPNP_SOCK_TIMEOUT / 1000;
	timeout.tv_usec = (UPNP_SOCK_TIMEOUT % 1000) * 1000;
	if(setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(struct timeval)) < 0)
	{
		PRINT_SOCKET_ERROR("setsockopt");
	}
	timeout.tv_sec = UPNP_SOCK_TIMEOUT / 1000;
	timeout.tv_usec = (UPNP_SOCK_TIMEOUT % 1000) * 1000;
	if(setsockopt(s, SOL_SOCKET, SO_SNDTIMEO, &timeout, sizeof(struct timeval)) < 0)
	{
		PRINT_SOCKET_ERROR("setsockopt");
	}
#endif /* #ifdef MINIUPNPC_SET_SOCKET_TIMEOUT */
	dest.sin_family = AF_INET;
	dest.sin_port = htons(port);
//	n = connect(s, (struct sockaddr *)&dest, sizeof(struct sockaddr_in));		

	SetSockNonBlock(s, 1);
	if(AsyncConnect(s, (struct sockaddr *)&dest, sizeof(struct sockaddr_in), UPNP_SOCK_TIMEOUT) == 0)
	{
		n = 0;
	}
	else
	{
		n = -1;
	}
	SetSockNonBlock(s, 0);
#if 0 MINIUPNPC_IGNORE_EINTR
	/* EINTR The system call was interrupted by a signal that was caught
	 * EINPROGRESS The socket is nonblocking and the connection cannot
	 *             be completed immediately. */
	while(n < 0 && (errno == EINTR || errno = EINPROGRESS))
	{
		socklen_t len;
		fd_set wset;
		int err;
		FD_ZERO(&wset);
		FD_SET(s, &wset);
		if((n = select(s + 1, NULL, &wset, NULL, NULL)) == -1 && errno == EINTR)
			continue;
		/*len = 0;*/
		/*n = getpeername(s, NULL, &len);*/
		len = sizeof(err);
		if(getsockopt(s, SOL_SOCKET, SO_ERROR, &err, &len) < 0) {
			PRINT_SOCKET_ERROR("getsockopt");
			closesocket(s);
			return -1;
		}
		if(err != 0) {
			errno = err;
			n = -1;
		}
	}
#endif /* #ifdef MINIUPNPC_IGNORE_EINTR */
	if(n<0)
	{
		PRINT_SOCKET_ERROR("connect");
		closesocket(s);
		return -1;
	}
#else /* #ifdef USE_GETHOSTBYNAME */
	/* use getaddrinfo() instead of gethostbyname() */
	memset(&hints, 0, sizeof(hints));
	/* hints.ai_flags = AI_ADDRCONFIG; */
#ifdef AI_NUMERICSERV
	hints.ai_flags = AI_NUMERICSERV;
#endif
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_family = AF_UNSPEC; /* AF_INET, AF_INET6 or AF_UNSPEC */
	/* hints.ai_protocol = IPPROTO_TCP; */
	snprintf(port_str, sizeof(port_str), "%hu", port);
	if(host[0] == '[')
	{
		/* literal ip v6 address */
		int i, j;
		for(i = 0, j = 1; host[j] && (host[j] != ']') && i < MAXHOSTNAMELEN; i++, j++)
		{
			tmp_host[i] = host[j];
			if(0 == memcmp(host+j, "%25", 3))	/* %25 is just url encoding for '%' */
				j+=2;							/* skip "25" */
		}
		tmp_host[i] = '\0';
	}
	else
	{
		strncpy(tmp_host, host, MAXHOSTNAMELEN);
	}
	tmp_host[MAXHOSTNAMELEN] = '\0';
	n = getaddrinfo(tmp_host, port_str, &hints, &ai);
	if(n != 0)
	{
#ifdef _WIN32
		fprintf(stderr, "getaddrinfo() error : %d\n", n);
#else
		fprintf(stderr, "getaddrinfo() error : %s\n", gai_strerror(n));
#endif
		return -1;
	}
	s = -1;
	for(p = ai; p; p = p->ai_next)
	{
		s = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
		if(s < 0)
			continue;
		if(p->ai_addr->sa_family == AF_INET6 && scope_id > 0) {
			struct sockaddr_in6 * addr6 = (struct sockaddr_in6 *)p->ai_addr;
			addr6->sin6_scope_id = scope_id;
		}
#ifdef MINIUPNPC_SET_SOCKET_TIMEOUT
		/* setting a 3 seconds timeout for the connect() call */
		timeout.tv_sec = UPNP_SOCK_TIMEOUT / 1000;
		timeout.tv_usec = (UPNP_SOCK_TIMEOUT % 1000) * 1000;
		if(setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(struct timeval)) < 0)
		{
			PRINT_SOCKET_ERROR("setsockopt");
		}
		timeout.tv_sec = UPNP_SOCK_TIMEOUT / 1000;
		timeout.tv_usec = (UPNP_SOCK_TIMEOUT % 1000) * 1000;
		if(setsockopt(s, SOL_SOCKET, SO_SNDTIMEO, &timeout, sizeof(struct timeval)) < 0)
		{
			PRINT_SOCKET_ERROR("setsockopt");
		}
#endif /* #ifdef MINIUPNPC_SET_SOCKET_TIMEOUT */
//		n = connect(s, p->ai_addr, p->ai_addrlen);

		SetSockNonBlock(s, 1);
		if(AsyncConnect(s, p->ai_addr, (socklen_t)p->ai_addrlen, UPNP_SOCK_TIMEOUT) == 0)
		{
			n = 0;
		}
		else
		{
			n = -1;
		}
		SetSockNonBlock(s, 0);
#if 0 //MINIUPNPC_IGNORE_EINTR
		/* EINTR The system call was interrupted by a signal that was caught
		 * EINPROGRESS The socket is nonblocking and the connection cannot
		 *             be completed immediately. */
		while(n < 0 && (errno == EINTR || errno == EINPROGRESS))
		{
			socklen_t len;
			fd_set wset;
			int err;
			FD_ZERO(&wset);
			FD_SET(s, &wset);
			if((n = select(s + 1, NULL, &wset, NULL, NULL)) == -1 && errno == EINTR)
				continue;
			/*len = 0;*/
			/*n = getpeername(s, NULL, &len);*/
			len = sizeof(err);
			if(getsockopt(s, SOL_SOCKET, SO_ERROR, &err, &len) < 0) {
				PRINT_SOCKET_ERROR("getsockopt");
				closesocket(s);
				freeaddrinfo(ai);
				return -1;
			}
			if(err != 0) {
				errno = err;
				n = -1;
			}
		}
#endif /* #ifdef MINIUPNPC_IGNORE_EINTR */
		if(n < 0)
		{
			closesocket(s);
			continue;
		}
		else
		{
			break;
		}
	}
	freeaddrinfo(ai);
	if(s < 0)
	{
		PRINT_SOCKET_ERROR("socket");
		return -1;
	}
	if(n < 0)
	{
		PRINT_SOCKET_ERROR("connect");
		return -1;
	}
#endif /* #ifdef USE_GETHOSTBYNAME */
	return s;
}

