#ifndef _COMMON_H_
#define _COMMON_H_
/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : 
** Date         : 2008-11-11
** Name         : common.h
** Version      : 1.0
** Description  :
** Modify Record:
1:
***********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#ifndef WIN32
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/socket.h>

#else
#include "SWL_TCPLink.h"
#endif

#include <assert.h>

const unsigned long MAXLINE = 3*1024;
const unsigned long RIO_READER_BUF_LEN = 8*1024;

#ifdef WIN32

typedef struct {
    CSWL_TCPLink *pTcpLink;
    int			cnt;
    char		*pHead;
    char		*buf;
	int			bufSize;
}RIO_READER;

void rio_readinitb(RIO_READER &reader, CSWL_TCPLink *pTcpLink);
int rio_readlineb(RIO_READER &reader, char *pBuf, size_t len);
int rio_readnb(RIO_READER &reader, char *pBuf, size_t num);

int rio_writen(CSWL_TCPLink *pTcpLink, char *pBuf, size_t num);

#else

typedef struct {
    int fd;
    int cnt;
    char *pHead;
    char buf[RIO_READER_BUF_LEN];
}RIO_READER;

typedef struct sockaddr SA;
int rio_readn(int fd, char *pBuf, size_t num);
int rio_writen(int fd, char *pBuf, size_t num);

void rio_readinitb(RIO_READER &reader, int fd);
int rio_read(RIO_READER &reader, char *pBuf, size_t num);
int rio_readlineb(RIO_READER &reader, char *pBuf, size_t len);
int rio_readnb(RIO_READER &reader, char *pBuf, size_t num);

int open_listenfd(int port);
int OpenListenfdv6(unsigned short port);

#endif

char *str_skip_sp(char *strSrc);


#ifdef __ENVIRONMENT_WIN32__
int strcasecmp(const char *str1, const char *str2);
int strncasecmp(const char *str1, const char *str2, size_t count);
#endif

#endif // _COMMON_H_
