/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : 袁石维
** Date         : 2009-07-16
** Name         : FBInterface.cpp
** Version      : 1.0
** Description  :
** Modify Record:
1:
***********************************************************************/
#ifdef __ENVIRONMENT_WIN32__
#else
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
//#include <asm/page.h>
#endif

#include "FBInterface.h"
#include "MessageMan.h"
#include "StringTable.h"
#include "Thai.h"
#include "Arabic.h"

#ifdef __ENVIRONMENT_LINUX__
#include "dvr_func.h"
#endif

extern bool g_langRightOrder;

const unsigned short FONT_INFO_BUF_LEN = 4 + ((60 * 20) >> 3);

#if defined(__TDFH__) || defined(__CHIPGM__)
#define PAGE_MASK (0xfffff000)
#endif

#ifdef __CHIPTI__ 
#define PAGE_MASK (0xfffff000)
//#define FB_MOUSE 0
#endif

#ifdef FB_MOUSE
unsigned long g_fb_bk [16*20] = {0};
unsigned char CurseValue[]= /*width = 32, height = 32*/
{
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x1, 0x1, 0x1, 0x0, 0x1, 0x1, 0x1, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x1, 0x1, 0x1, 0x0, 0xff, 0xff, 0xff, 0x0, 0x1, 0x1, 0x1, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x1, 0x1, 0x1, 0x0, 0xff, 0xff, 0xff, 0x0, 0xff, 0xff, 0xff, 0x0, 
	0x1, 0x1, 0x1, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x1, 0x1, 0x1, 0x0, 0xff, 0xff, 0xff, 0x0, 0xff, 0xff, 0xff, 0x0, 
	0xff, 0xff, 0xff, 0x0, 0x1, 0x1, 0x1, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x1, 0x1, 0x1, 0x0, 0xff, 0xff, 0xff, 0x0, 0xff, 0xff, 0xff, 0x0, 
	0xff, 0xff, 0xff, 0x0, 0xff, 0xff, 0xff, 0x0, 0x1, 0x1, 0x1, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x1, 0x1, 0x1, 0x0, 0xff, 0xff, 0xff, 0x0, 0xff, 0xff, 0xff, 0x0, 
	0xff, 0xff, 0xff, 0x0, 0xff, 0xff, 0xff, 0x0, 0xff, 0xff, 0xff, 0x0, 0x1, 0x1, 0x1, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x1, 0x1, 0x1, 0x0, 0xff, 0xff, 0xff, 0x0, 0xff, 0xff, 0xff, 0x0, 
	0xff, 0xff, 0xff, 0x0, 0xff, 0xff, 0xff, 0x0, 0xff, 0xff, 0xff, 0x0, 0xff, 0xff, 0xff, 0x0, 
	0x1, 0x1, 0x1, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x1, 0x1, 0x1, 0x0, 0xff, 0xff, 0xff, 0x0, 0xff, 0xff, 0xff, 0x0, 
	0xff, 0xff, 0xff, 0x0, 0xff, 0xff, 0xff, 0x0, 0xff, 0xff, 0xff, 0x0, 0xff, 0xff, 0xff, 0x0, 
	0xff, 0xff, 0xff, 0x0, 0x1, 0x1, 0x1, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x1, 0x1, 0x1, 0x0, 0xff, 0xff, 0xff, 0x0, 0xff, 0xff, 0xff, 0x0, 
	0xff, 0xff, 0xff, 0x0, 0xff, 0xff, 0xff, 0x0, 0xff, 0xff, 0xff, 0x0, 0xff, 0xff, 0xff, 0x0, 
	0xff, 0xff, 0xff, 0x0, 0xff, 0xff, 0xff, 0x0, 0x1, 0x1, 0x1, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x1, 0x1, 0x1, 0x0, 0xff, 0xff, 0xff, 0x0, 0xff, 0xff, 0xff, 0x0, 
	0xff, 0xff, 0xff, 0x0, 0xff, 0xff, 0xff, 0x0, 0xff, 0xff, 0xff, 0x0, 0xff, 0xff, 0xff, 0x0, 
	0xff, 0xff, 0xff, 0x0, 0xff, 0xff, 0xff, 0x0, 0xff, 0xff, 0xff, 0x0, 0x1, 0x1, 0x1, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x1, 0x1, 0x1, 0x0, 0xff, 0xff, 0xff, 0x0, 0xff, 0xff, 0xff, 0x0, 
	0xff, 0xff, 0xff, 0x0, 0xff, 0xff, 0xff, 0x0, 0xff, 0xff, 0xff, 0x0, 0xff, 0xff, 0xff, 0x0, 
	0x1, 0x1, 0x1, 0x0, 0x1, 0x1, 0x1, 0x0, 0x1, 0x1, 0x1, 0x0, 0x1, 0x1, 0x1, 0x0, 
	0x1, 0x1, 0x1, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x1, 0x1, 0x1, 0x0, 0xff, 0xff, 0xff, 0x0, 0xff, 0xff, 0xff, 0x0, 
	0xff, 0xff, 0xff, 0x0, 0x1, 0x1, 0x1, 0x0, 0xff, 0xff, 0xff, 0x0, 0xff, 0xff, 0xff, 0x0, 
	0x1, 0x1, 0x1, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x1, 0x1, 0x1, 0x0, 0xff, 0xff, 0xff, 0x0, 0xff, 0xff, 0xff, 0x0, 
	0x1, 0x1, 0x1, 0x0, 0x1, 0x1, 0x1, 0x0, 0xff, 0xff, 0xff, 0x0, 0xff, 0xff, 0xff, 0x0, 
	0x1, 0x1, 0x1, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x1, 0x1, 0x1, 0x0, 0xff, 0xff, 0xff, 0x0, 0x1, 0x1, 0x1, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x1, 0x1, 0x1, 0x0, 0xff, 0xff, 0xff, 0x0, 
	0xff, 0xff, 0xff, 0x0, 0x1, 0x1, 0x1, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x1, 0x1, 0x1, 0x0, 0x1, 0x1, 0x1, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x1, 0x1, 0x1, 0x0, 0xff, 0xff, 0xff, 0x0, 
	0xff, 0xff, 0xff, 0x0, 0x1, 0x1, 0x1, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x1, 0x1, 0x1, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x1, 0x1, 0x1, 0x0, 
	0xff, 0xff, 0xff, 0x0, 0xff, 0xff, 0xff, 0x0, 0x1, 0x1, 0x1, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x1, 0x1, 0x1, 0x0, 
	0xff, 0xff, 0xff, 0x0, 0xff, 0xff, 0xff, 0x0, 0x1, 0x1, 0x1, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x1, 0x1, 0x1, 0x0, 0xff, 0xff, 0xff, 0x0, 0xff, 0xff, 0xff, 0x0, 0x1, 0x1, 0x1, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x1, 0x1, 0x1, 0x0, 0xff, 0xff, 0xff, 0x0, 0xff, 0xff, 0xff, 0x0, 0x1, 0x1, 0x1, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x1, 0x1, 0x1, 0x0, 0x1, 0x1, 0x1, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0
};

CPUB_Lock g_mouse_lock;
#endif
//////////////////////////////////////////////////////////////////////////
GUI::CFBInterface * GUI::CFBInterface::Instance()
{
	static GUI::CFBInterface s_FBInterface;
	return &s_FBInterface;
}

#ifdef __ENVIRONMENT_WIN32__
GUI::CFBInterface::CFBInterface() : m_chnnNum(0), m_pFrameBuffer(NULL), m_pDrawBuf(NULL), m_cx(0), m_cy(0), m_width(0), m_bufLen(0), m_transparency(TRANSPARENCY_0)
{
	m_pAudioImage = NULL;
	m_pMotionImage = NULL;
	m_pPTZImage = NULL;
	m_pLogoImage = NULL;
	m_pWaitImage = NULL;
	m_pSnapSuccImage = NULL;
	m_pSnapFailImage = NULL;

	m_logoWidth = 0;
	m_logoHeight = 0;
	m_logoLen = 0;

	memset(m_liveSplitMode, 0 , sizeof(unsigned long) * 8);
	memset(m_displayAttrib, 0, MAX_CHNN_NUM*sizeof(DISPLAY_ATTRIB));

	m_showRecStatusCH = INVALID_INDEX(unsigned long);


	m_pos_x = INVALID_INDEX(unsigned long);
	m_pos_y = INVALID_INDEX(unsigned long);
	m_initchnn =INVALID_INDEX(unsigned long);

}
#else
GUI::CFBInterface::CFBInterface() : m_pFrameBuffer(NULL), m_pDrawBuf(NULL), m_cx(0), m_cy(0), m_width(0), m_bufLen(0), m_fb_fd(-1), m_transparency(TRANSPARENCY_0)
{
	m_showRecStatusCH = INVALID_INDEX(unsigned long);

	memset(&m_fb_fix, 0, sizeof(fb_fix_screeninfo));
	memset(&m_fb_var, 0, sizeof(fb_var_screeninfo));

	memset(&m_miceInfo , 0, sizeof(MICE_INFO));

	m_pWaitImage = NULL;
#ifdef __TDHISI__
	m_fb_buf_addr_phy = 0;
#endif
}
#endif

GUI::CFBInterface::~CFBInterface()
{}

#ifdef __ENVIRONMENT_WIN32__
bool GUI::CFBInterface::Initial(const char *pFilePath, CDisplayCtrl *pDisplay, unsigned char localVideoInputNum, unsigned char videoInputNum, unsigned char videoOutNum, unsigned char netVideoInputNum)
{
	assert (NULL != pFilePath);

	m_font.Initial(pFilePath);
	
	m_pDisplay = pDisplay;

	m_localVideoInputNum = localVideoInputNum;
	m_videoInputNum = videoInputNum;
	m_videoOutNum = videoOutNum;
	m_netVideoInputNum = netVideoInputNum;

    m_pos_x = INVALID_INDEX(unsigned long);
	m_pos_y = INVALID_INDEX(unsigned long);
	m_initchnn =INVALID_INDEX(unsigned long);

	return true;
}
#else
bool GUI::CFBInterface::Initial(const char *pFilePath, unsigned short & width, unsigned short & height, unsigned char localVideoInputNum, unsigned char videoInputNum, unsigned char videoOutNum, unsigned char netVideoInputNum)
{
	assert (NULL != pFilePath);


	m_pos_x = INVALID_INDEX(unsigned long);
	m_pos_y = INVALID_INDEX(unsigned long);
	m_initchnn =INVALID_INDEX(unsigned long);

#ifdef __TDHISI__
	ext_dvr_fb_screen_info fb_info;
	memset(&fb_info, 0, sizeof(ext_dvr_fb_screen_info));
	
	if (0 != ext_get_fb_info(1, &fb_info))
	{
		printf("Error opening /dev/fb, Check kernel config\n");
		return false;
	}
	else
	{
		//map physics address to virtual address
		m_pFrameBuffer = fb_info.p_begin_addr;

		m_fb_fix.smem_len = fb_info.stride*fb_info.height;
		m_fb_fix.line_length = fb_info.stride;
		
		m_fb_var.xres = fb_info.width;
		m_fb_var.yres = fb_info.height;

		m_fb_var.bits_per_pixel = (fb_info.bytes * 8);

		printf("The mem is :%d\n", m_fb_fix.smem_len);
		printf("The line_length is :%d\n", m_fb_fix.line_length);
		printf("The xres is :%d\n", m_fb_var.xres);
		printf("The yres is :%d\n", m_fb_var.yres);
		printf("bits_per_pixel is :%d\n", m_fb_var.bits_per_pixel);

		width = m_fb_var.xres;
		height = m_fb_var.yres;

		m_fb_buf_addr_phy = fb_info.phy_address;
	}
#else
#if defined(__TDFH__)  || defined(__CHIPTI__)
	m_fb_fd = open("/dev/fb0", O_RDWR);
#else
	m_fb_fd = open("/dev/fb1", O_RDWR);
#endif
	if(m_fb_fd  < 0)
	{
		printf("Error opening /dev/fb. Check kernel config\n");
		return false;
	}

	if (-1 == ioctl(m_fb_fd, FBIOGET_VSCREENINFO, &m_fb_var))
	{
		printf("ioctl FBIOGET_VSCREENINFO\n");
		return false;
	}

	if (-1 == ioctl(m_fb_fd, FBIOGET_FSCREENINFO, &m_fb_fix))
	{
		printf("ioctl FBIOGET_FSCREENINFO\n");
		return false;
	}

	//map physics address to virtual address
// 	m_pFrameBuffer = (unsigned char *)mmap(NULL, \
// 		m_fb_fix.smem_len + ((unsigned long)(m_fb_fix.smem_start) & (~PAGE_MASK)), \
// 		PROT_READ | PROT_WRITE, MAP_SHARED, m_fb_fd, 0);

	//映射时使用的长度为fb实际内存区域的长度
	m_pFrameBuffer = (unsigned char *)mmap(NULL, m_fb_fix.smem_len, PROT_READ | PROT_WRITE, MAP_SHARED, m_fb_fd, 0);
	if (-1L == (long)m_pFrameBuffer)
	{
		printf("mmap error! mem:%d offset:%d\n", m_pFrameBuffer, (unsigned long)(m_fb_fix.smem_start) & (~PAGE_MASK));
		return false;
	}
	else
	{
		printf("mem:%d offset:%d\n", m_pFrameBuffer, (unsigned long)(m_fb_fix.smem_start) & (~PAGE_MASK));
	}

	printf("The mem is :%d\n", m_fb_fix.smem_len);
	printf("The line_length is :%d\n", m_fb_fix.line_length);
	printf("The xres is :%d\n", m_fb_var.xres);
	printf("The yres is :%d\n", m_fb_var.yres);
	printf("bits_per_pixel is :%d\n", m_fb_var.bits_per_pixel);

	width = m_fb_var.xres;
	height = m_fb_var.yres;
#endif

	if (!m_font.Initial(pFilePath))
	{
		return false;
	}
	m_bEnableRefreshLive = false;

	memset(m_liveSplitMode, 0 , sizeof(unsigned long) * 8);
	memset(m_displayAttrib, 0, MAX_CHNN_NUM*sizeof(DISPLAY_ATTRIB));

	m_localVideoInputNum = localVideoInputNum;
	m_videoInputNum = videoInputNum;
	m_videoOutNum = videoOutNum;
	m_netVideoInputNum = netVideoInputNum;

	return true;
}
#endif

void GUI::CFBInterface::Quit()
{
#ifdef __ENVIRONMENT_WIN32__
	m_pDrawBuf	= NULL;
	m_bufLen	= 0;
#else

#ifndef __TDHISI__

	if (NULL != m_pFrameBuffer)
	{
		//FH平台FB的地址需要unmap
		if (munmap(m_pFrameBuffer, m_fb_fix.smem_len))
		{
			printf("%s:%s:%d, mumap error, m_pFrameBuffer=0x%08x, smem_len=0x%08x\n", __FUNCTION__, __FILE__, __LINE__, m_pFrameBuffer, m_fb_fix.smem_len);
		}
		
	}
	if (NULL != m_pDrawBuf)
	{
		delete [] m_pDrawBuf;
		m_pDrawBuf	= NULL;
		m_bufLen	= 0;
	}

	close(m_fb_fd);
	m_fb_fd = -1;
#endif

#endif

	m_cx = 0;
	m_cy = 0;
	m_width = 0;

	m_font.Quit();

	m_pAudioImage = NULL;
	m_pMotionImage = NULL;
	m_pPTZImage = NULL;

	m_pFrameBuffer = NULL;
	m_bEnableRefreshLive = false;

	memset(&m_liveSplitMode, 0 , sizeof(unsigned long) * 8);
	memset(m_displayAttrib, 0, MAX_CHNN_NUM*sizeof(DISPLAY_ATTRIB));
}

void GUI::CFBInterface::SetImageAddr(unsigned char *pAudio, unsigned char *pMotion, unsigned char *pPTZ, unsigned char *pWait, unsigned char* pSnapSuccImage, unsigned char* pSnapFailImage, unsigned char* pLogo, int logoWidth, int logoHeight, int logoLen)
{
	m_pAudioImage = pAudio;
	m_pMotionImage = pMotion;
	m_pPTZImage = pPTZ;
	m_pWaitImage = pWait;
	m_pLogoImage = pLogo;
	m_pSnapSuccImage = pSnapSuccImage;
	m_pSnapFailImage = pSnapFailImage;

	m_logoWidth = logoWidth;
	m_logoHeight = logoHeight;
	m_logoLen = logoLen;
}

void GUI::CFBInterface::WaitProc(bool bShow)
{
	if (NULL == m_pWaitImage)
	{
		return;
	}

	if (bShow)
	{
		unsigned long x = m_cx/2 - 16;
		unsigned long y = m_cy/2 - 16;

#ifdef __ENVIRONMENT_WIN32__
		unsigned short *pBuf = NULL, *pSrcBuf = reinterpret_cast<unsigned short *>(m_pWaitImage);

		for (unsigned short h=0; h<32; ++h)
		{
			pBuf = reinterpret_cast<unsigned short *>(m_pDrawBuf + m_width*(y + h) + x*FRAME_BUFFER_PIXSEL_NUM);

			for (unsigned short w=0; w<32; ++w)
			{
				if (0x8000 & pSrcBuf[0])
				{
					pBuf[0] = pSrcBuf[0];
				}
			
				++ pSrcBuf;
				++ pBuf;
			}
		}

		m_pDisplay->Refresh(x, y, 32, 32);
#else
		unsigned short *pBuf = NULL, *pSrcBuf = reinterpret_cast<unsigned short *>(m_pWaitImage);

#ifdef __TDHISI__
		ext_tde_begin_job();
#endif

#ifdef __TDHISI__
		unsigned char *pDec = NULL, *pSrc = NULL;

		for (unsigned short h=0; h<32; ++h)
		{
			pBuf = reinterpret_cast<unsigned short *>(m_pFrameBuffer + m_fb_fix.line_length*(y + h) + x*FRAME_BUFFER_PIXSEL_NUM);

			for (unsigned short w=0; w<32; ++w)
			{
				if (0x8000 & pSrcBuf[0])
				{
					pDec = reinterpret_cast<unsigned char *>(pBuf);
					pSrc = reinterpret_cast<unsigned char *>(pSrcBuf);

					ext_fb_copy(m_fb_buf_addr_phy+(pDec - m_pFrameBuffer), m_app_buf_addr_phy+(pSrc - m_pAppBuf), 32, m_fb_var.yres, 1, 1, 0x11);
				}

				++ pSrcBuf;
				++ pBuf;
			}
		}
#else
		for (unsigned short h=0; h<32; ++h)
		{
			pBuf = reinterpret_cast<unsigned short *>(m_pFrameBuffer + m_width*(y + h) + x*FRAME_BUFFER_PIXSEL_NUM);

			for (unsigned short w=0; w<32; ++w)
			{
				if (0x8000 & pSrcBuf[0])
				{
					pBuf[0] = pSrcBuf[0];
				}

				++ pSrcBuf;
				++ pBuf;
			}
		}
#endif

#ifdef __TDHISI__
		ext_tde_end_job();
#endif

#endif
	}
}

void GUI::CFBInterface::SetResolution(unsigned short cx, unsigned short cy)
{
	assert (cx > 0);
	assert (cy > 0);


#ifdef __ENVIRONMENT_WIN32__
	m_bufLen = cx*cy*FRAME_BUFFER_PIXSEL_NUM;
	m_pDisplay->GetBuffer(&m_pDrawBuf);
	assert (NULL != m_pDrawBuf);
	memset(m_pDrawBuf, CLEAR_COLOR_VALUE, m_bufLen);
#else

#ifdef __TDHISI__
	m_bufLen = cx*cy*FRAME_BUFFER_PIXSEL_NUM;
	m_pDrawBuf = (m_pFrameBuffer+m_fb_fix.smem_len);

	assert (NULL != m_pDrawBuf);
	memset(m_pDrawBuf, 0, m_bufLen);
#else
	if (NULL != m_pDrawBuf)
	{
		delete [] m_pDrawBuf;
		m_pDrawBuf = NULL;
		m_bufLen = 0;
	}

	m_bufLen = cx*cy*FRAME_BUFFER_PIXSEL_NUM;
	m_pDrawBuf = new unsigned char [m_bufLen];
	assert (NULL != m_pDrawBuf);
	memset(m_pDrawBuf, 0, m_bufLen);
#endif

#endif

	m_cx = cx;
	m_cy = cy;
	m_width = m_cx*FRAME_BUFFER_PIXSEL_NUM;
}

void GUI::CFBInterface::Refresh(unsigned short x, unsigned short y, unsigned short cx, unsigned short cy)
{
#ifdef __ENVIRONMENT_WIN32__
	m_pDisplay->Refresh(x, y, cx, cy);
#else
	unsigned char *pSrc = m_pDrawBuf;
	unsigned char *pDec = m_pFrameBuffer;

#ifdef FB_MOUSE
	g_mouse_lock.Lock();
#endif

#ifdef __TDHISI__
	ext_tde_begin_job();
#endif

#ifdef __TDHISI__
	pSrc = m_pDrawBuf + (m_width * y) + (x*FRAME_BUFFER_PIXSEL_NUM);
	pDec = m_pFrameBuffer + (m_fb_fix.line_length * y) + (x*2);

	ext_fb_copy(m_fb_buf_addr_phy+(pDec - m_pFrameBuffer), m_fb_buf_addr_phy+(pSrc - m_pFrameBuffer), m_fb_var.xres, m_fb_var.yres, cx, cy, 0x11);
#else
#ifdef __CHIPTI__
	if(m_fb_var.xres==704)//SD
	{
		unsigned short* pwdSrcPre;
		unsigned short* pwdSrcCur;
		unsigned short* pwdDstPre;
		unsigned short* pwdDstCur;
		int pr,pg,pb;
		int cr,cg,cb;
		int tr,tg,tb;
		int value;
		
		if(y&1)
		{
			y = y - 1;
			cy += 1;
		}
		if(cy&1)
		{
			cy += 1;
		}
		
		for (int i=0; i<cy; i+=2)
		{
			pwdSrcPre = (unsigned short*)(m_pDrawBuf + (m_width * (i + y)) + (x*FRAME_BUFFER_PIXSEL_NUM));
			pwdSrcCur = (unsigned short*)(m_pDrawBuf + (m_width * (i + 1 + y)) + (x*FRAME_BUFFER_PIXSEL_NUM));;
			pwdDstPre = (unsigned short*)(m_pFrameBuffer + (m_fb_fix.line_length * (i + y)) + (x*FRAME_BUFFER_PIXSEL_NUM));
			pwdDstCur = (unsigned short*)(m_pFrameBuffer + (m_fb_fix.line_length * (i + 1 + y)) + (x*FRAME_BUFFER_PIXSEL_NUM));
			for(int j=0;j<cx;j++)
			{
				unsigned short DataPre = *pwdSrcPre;
				unsigned short DataCur = *pwdSrcCur;
				pr = (DataPre&0x7c00)>>10;
				pg = (DataPre&0x03e0)>>5;
				pb = (DataPre&0x001f);

				cr = (DataCur&0x7c00)>>10;
				cg = (DataCur&0x03e0)>>5;
				cb = (DataCur&0x001f);
				
				if(pr>cr)
				{
					tr = pr - cr;
				}else
				{
					tr = cr - pr;
				}
				if(pg>cg)
				{
					tg = pg - cg;
				}else
				{
					tg = cg - pg;
				}
				if(pb>cb)
				{
					tb = pb - cb;
				}else
				{
					tb = cb - pb;
				}
#define RGB_DETA 15
				if( 1 || tr>RGB_DETA||tg>RGB_DETA||tb>RGB_DETA)
				{
					value  = (((5*pr+3*cr)>>3) & 0x001f)<<10;
					value |= (((5*pg+3*cg)>>3) & 0x001f)<<5;
					value |= (((5*pb+3*cb)>>3) & 0x001f);
					value |= DataPre & 0x8000;
					*pwdDstPre = value;

					value  = (((5*cr+3*pr)>>3) & 0x001f)<<10;
					value |= (((5*cg+3*pg)>>3) & 0x001f)<<5;
					value |= (((5*cb+3*pb)>>3) & 0x001f);
					value |= DataCur & 0x8000;
					*pwdDstCur = value;
				}
				else
				{
					*pwdDstPre = DataPre;
					*pwdDstCur = DataCur;
				}
				pwdSrcPre ++;
				pwdSrcCur ++;  
				pwdDstPre ++;  
				pwdDstCur ++;  
			} 
		}  
	}
	else
	{
		for (int i=0; i<cy; ++i)
		{
			pSrc = m_pDrawBuf + (m_width * (i + y)) + (x*FRAME_BUFFER_PIXSEL_NUM);
			pDec = m_pFrameBuffer + (m_fb_fix.line_length * (i + y)) + (x*FRAME_BUFFER_PIXSEL_NUM);
	
			memcpy(pDec, pSrc, cx*FRAME_BUFFER_PIXSEL_NUM);
		}
	}
#else
	for (int i=0; i<cy; ++i)
	{
		pSrc = m_pDrawBuf + (m_width * (i + y)) + (x*FRAME_BUFFER_PIXSEL_NUM);
		pDec = m_pFrameBuffer + (m_fb_fix.line_length * (i + y)) + (x*FRAME_BUFFER_PIXSEL_NUM);

		memcpy(pDec, pSrc, cx*FRAME_BUFFER_PIXSEL_NUM);
	}
#endif
#endif

#ifdef __TDHISI__
	ext_tde_end_job();
#endif

#ifdef FB_MOUSE
	//只要鼠标区域的四个角的点，最多有一个在改变的区域内，就要进行刷新。
	if ( ((x <= m_miceInfo.x) && (m_miceInfo.x < (x+cx)) && (y <= m_miceInfo.y) && (m_miceInfo.y < (y+cy))) || \
		 ((x < (m_miceInfo.x+16)) && ((m_miceInfo.x+16) < (x+cx)) && (y < (m_miceInfo.y+20)) && ((m_miceInfo.y+20) < (y+cy))) || \
		 ((x < (m_miceInfo.x+16)) && ((m_miceInfo.x+16) < (x+cx)) && (y < m_miceInfo.y) && (m_miceInfo.y < (y+cy))) || \
		 ((x < m_miceInfo.x) && (m_miceInfo.x < (x+cx)) && (y < (m_miceInfo.y+20)) && ((m_miceInfo.y+20) < (y+cy))) )
	{
#ifndef __CHIPTI__
		//需要把新刷入的图片放入鼠标背景区
		unsigned long *pBuf = (unsigned long*)(m_pDrawBuf + m_width*m_miceInfo.y + m_miceInfo.x*FRAME_BUFFER_PIXSEL_NUM);
		unsigned long *pBK = (unsigned long *)(g_fb_bk);

		unsigned char yNum = ((m_miceInfo.y + 20) < m_cy) ? 20 : (m_cy - m_miceInfo.y);
		unsigned char xNum = ((m_miceInfo.x + 16) < m_cx) ? 16 : (m_cx - m_miceInfo.x);
		for (unsigned char h=0; h<yNum; ++h)
		{
			memcpy(pBK, pBuf, xNum*FRAME_BUFFER_PIXSEL_NUM);
			pBuf += m_cx;
			pBK += 16;
		}

		//绘制光标
		pBuf = (unsigned long*)(m_pFrameBuffer + m_width*m_miceInfo.y + m_miceInfo.x*FRAME_BUFFER_PIXSEL_NUM);
		pBK = (unsigned long*)(CurseValue);
		for (unsigned char h=0; h<yNum; ++h)
		{
			for (unsigned char w=0; w<xNum; ++w)
			{
				if (0 != pBK[w])
				{
					pBuf[w] = (pBK[w] | 0xff000000);
				}
			}

			pBuf += m_cx;
			pBK += 32;
		}
#else
		//需要把新刷入的图片放入鼠标背景区
		unsigned short *pBuf = (unsigned short*)(m_pDrawBuf + m_width*m_miceInfo.y + m_miceInfo.x*FRAME_BUFFER_PIXSEL_NUM);
		unsigned short *pBK = (unsigned short *)(g_fb_bk);

		unsigned char yNum = ((m_miceInfo.y + 20) < m_cy) ? 20 : (m_cy - m_miceInfo.y);
		unsigned char xNum = ((m_miceInfo.x + 16) < m_cx) ? 16 : (m_cx - m_miceInfo.x);
		for (unsigned char h=0; h<yNum; ++h)
		{
			memcpy(pBK, pBuf, xNum*FRAME_BUFFER_PIXSEL_NUM);
			pBuf += m_cx;
			pBK += 16;
		}

		//绘制光标
		pBuf = (unsigned short*)(m_pFrameBuffer + m_width*m_miceInfo.y + m_miceInfo.x*FRAME_BUFFER_PIXSEL_NUM);
		unsigned long * pBK1 = (unsigned long*)(CurseValue);
		for (unsigned char h=0; h<yNum; ++h)
		{
			for (unsigned char w=0; w<xNum; ++w)
			{
				if (0 != pBK1[w])
				{
					pBuf[w] = ((pBK1[w]&0x7fff) | 0x8000);
				}
			}

			pBuf += m_cx;
			pBK1 += 32;
		}
#endif	
	}

	g_mouse_lock.UnLock();
#endif

#endif
}

void GUI::CFBInterface::Clear(unsigned short x, unsigned short y, unsigned short cx, unsigned short cy)
{
	assert (x < m_cx);
	assert (y < m_cy);
	assert ((0 < cx) && (cx <= m_cx));
	assert ((0 < cy) && (cy <= m_cy));

	assert (NULL != m_pDrawBuf);

	//把第一行设成需要的颜色值，然后一行一行地把后面的设定
	unsigned char *pData = m_pDrawBuf + m_width*y + x*FRAME_BUFFER_PIXSEL_NUM;
	unsigned long len = cx*FRAME_BUFFER_PIXSEL_NUM;

	//进行尺寸修正，以免超出范围
	cx = ((x+cx) > m_cx) ? (m_cx-x) : cx;
	cy = ((y+cy) > m_cy) ? (m_cy-y) : cy;

#ifdef __TDHISI__
	ext_tde_begin_job();
	ext_fb_quick_fill(pData, 0, cx, cy, 1);
	ext_tde_end_job();
#else
	for (unsigned short h=0; h<cy; ++h)
	{
#ifdef __ENVIRONMENT_WIN32__
		memset(pData, CLEAR_COLOR_VALUE, len);
#else
		memset(pData, 0, len);
#endif

		pData += m_width;
	}
#endif
}

void GUI::CFBInterface::DrawRect(unsigned short x, unsigned short y, unsigned short cx, unsigned short cy, unsigned long color)
{
	assert (NULL != m_pDrawBuf);

	//进行尺寸修正，以免超出范围
	cx = ((x+cx) > m_cx) ? (m_cx-x) : cx;
	cy = ((y+cy) > m_cy) ? (m_cy-y) : cy;

	color = (color & 0x00ffffff) + (m_transparency << 24);

	if( 2 == FRAME_BUFFER_PIXSEL_NUM)
	{
		color = GUI_RGB32_TO_RGB1555(color);
	}

	//把第一行设成需要的颜色值，然后一行一行地把后面的设定
	unsigned char *pData = m_pDrawBuf + m_width*y + x*FRAME_BUFFER_PIXSEL_NUM;

#ifdef __TDHISI__
	ext_tde_begin_job();
	ext_fb_quick_fill(pData, color, cx, cy, 1);
	ext_tde_end_job();
#else
	for (unsigned short w=0; w<cx; ++w)
	{
		memcpy(pData, &color, FRAME_BUFFER_PIXSEL_NUM);
		pData += FRAME_BUFFER_PIXSEL_NUM;
	}

	pData = m_pDrawBuf + m_width*y + x*FRAME_BUFFER_PIXSEL_NUM;

	int w = cx * FRAME_BUFFER_PIXSEL_NUM;
	for (unsigned short h=1; h<cy; ++h)
	{
		memcpy(pData+m_width, pData, w);
		pData += m_width;
	}
#endif
}

void GUI::CFBInterface::DrawRectBorder(unsigned short x, unsigned short y, unsigned short cx, unsigned short cy, unsigned long color, unsigned short height, unsigned short width)
{
	//进行尺寸修正，以免超出范围
	cx = ((x+cx) > m_cx) ? (m_cx-x) : cx;
	cy = ((y+cy) > m_cy) ? (m_cy-y) : cy;

	unsigned short x2 = (x + cx - 1);
	unsigned short y2 = (y + cy - 1);

	//上横
	DrawLine(x, y, x2, y, color, height);
	//下横
	DrawLine(x, y2-height+1, x2, y2-height+1, color, height);

#ifdef __TDHISI__
	//左竖
	DrawLine(x, y, x, y2, (COLOR_LINE_WHITE == color) ? GUI_RGB(128, 128, 128) : color, width);
	//右竖
	DrawLine(x2-width+1, y, x2-width+1, y2, (COLOR_LINE_WHITE == color) ? GUI_RGB(128, 128, 128) : color, width);
#else
	//左竖
	DrawLine(x, y, x, y2, color, width);
	//右竖
	DrawLine(x2-width+1, y, x2-width+1, y2, color, width);
#endif
}

void GUI::CFBInterface::DrawLine(unsigned short x1, unsigned short y1, unsigned short x2, unsigned short y2, unsigned long color, unsigned short width)
{
	x1 = (m_cx <= x1) ? (m_cx - 1) : x1;
	y1 = (m_cy <= y1) ? (m_cy - 1) : y1;
	x2 = (m_cx <= x2) ? (m_cx - 1) : x2;
	y2 = (m_cy <= y2) ? (m_cy - 1) : y2;

#ifdef __TDHISI__
	color = (COLOR_LINE_WHITE == color) ? GUI_RGB(128, 128, 128) : color;
#endif

	if( 2 == FRAME_BUFFER_PIXSEL_NUM)
	{
		color = GUI_RGB32_TO_RGB1555(color);
	}

	if (x1 == x2)//竖线
	{
		unsigned short cy = (y2 > y1) ? (y2 - y1 + 1) : (y1 - y2 + 1);
		unsigned short x = x1;
		unsigned short y = (y2 > y1) ? y1 : y2;
		unsigned char *pData = m_pDrawBuf + m_width*y + x*FRAME_BUFFER_PIXSEL_NUM;

#ifdef __TDHISI__
		ext_tde_begin_job();
		ext_fb_quick_fill(pData, color, width, cy, 1);
		ext_tde_end_job();
#else
		unsigned char *pLine = new unsigned char [width * FRAME_BUFFER_PIXSEL_NUM];
		for (unsigned short i = 0; i < width * FRAME_BUFFER_PIXSEL_NUM; i += FRAME_BUFFER_PIXSEL_NUM)
		{
			memcpy(pLine + i * sizeof(char), &color, FRAME_BUFFER_PIXSEL_NUM);
		}

		for (unsigned short h=0; h<cy; ++h)
		{
			memcpy(pData, pLine, width*FRAME_BUFFER_PIXSEL_NUM);
			pData += m_width;
		}

		delete [] pLine;
#endif
	} 
	else if (y1 == y2)
	{
		unsigned short cx = (x2 > x1) ? (x2 - x1 + 1) : (x1 - x2 + 1);
		unsigned short x = (x2 > x1) ? x1 : x2;
		unsigned short y = y1;
		unsigned char *pData = m_pDrawBuf + m_width*y + x*FRAME_BUFFER_PIXSEL_NUM;

#ifdef __TDHISI__
		ext_tde_begin_job();
		ext_fb_quick_fill(pData, color, cx, width, 1);
		ext_tde_end_job();
#else
		unsigned char *pLine = new unsigned char [cx * FRAME_BUFFER_PIXSEL_NUM];
		for (unsigned short i=0; i< cx * FRAME_BUFFER_PIXSEL_NUM; i += FRAME_BUFFER_PIXSEL_NUM)
		{
			memcpy(pLine + i * sizeof(char), &color, FRAME_BUFFER_PIXSEL_NUM);
		}

		for (unsigned short h=0; h<width; ++h)
		{
			memcpy(pData, pLine, cx * FRAME_BUFFER_PIXSEL_NUM);
			pData += m_width;
		}

		delete [] pLine;
#endif
	}
	else
	{
		assert(false);
	}
}

unsigned long GUI::CFBInterface::DrawText(const char *pStr, unsigned long len, unsigned short x, unsigned short y, unsigned short width, unsigned short height, unsigned long color, unsigned char zoom/* = 1*/, bool bForceDrawBorder, bool bResort)
{
	assert (NULL != m_pDrawBuf);
	assert (x < m_cx);
	assert (y < m_cy);

	//把第一行设成需要的颜色值，然后一行一行地把后面的设定
	unsigned char *pData = m_pDrawBuf + m_width*y + x*FRAME_BUFFER_PIXSEL_NUM;
	unsigned short cx = 0, cy = 0;

	unsigned short *pWord = NULL;

	unsigned long i = 0;

#if 0

	//// gb2312
	do
	{
		if ((0 <= static_cast<unsigned char>(pStr[i])) && (static_cast<unsigned char>(pStr[i]) < 0x21))
		{
			//do nothing
			if (width < 8)
			{
				break;
			}
			else
			{
				cx = (8 * zoom);
				++i;
			}
		}
		else if ((0x21 <= static_cast<unsigned char>(pStr[i])) && (static_cast<unsigned char>(pStr[i]) < 0x7f))	//english char
		{
			cx = width;
			cy = height;

			if (!m_font.GetBitmapBits(pData, m_cx, cx, cy, pStr[i], color, FRAME_BUFFER_PIXSEL_NUM, zoom))
			{
				break;
			}

			++ i;
		}
		else if(0x7f <= static_cast<unsigned char>(pStr[i]))
		{
			cx = width;
			cy = height;

			//双字节字符,只能显示gb2312定义部分
			if ((0x7f <= static_cast<unsigned char>(pStr[i])) && (static_cast<unsigned char>(pStr[i]) < 0xa1))
			{
				if (width < 8)
				{
					break;
				}
				else
				{
					m_font.GetBitmapBits(pData, m_cx, cx, cy, '?', color, FRAME_BUFFER_PIXSEL_NUM, zoom);
					//cx = 16;
					i += 2;
				}
			}
			else if (!((0xa9 < static_cast<unsigned char>(pStr[i])) && (static_cast<unsigned char>(pStr[i]) < 0xb0)) && (static_cast<unsigned char>(pStr[i]) < 0xf8))
			{
				pWord = (unsigned short *)(pStr+i);
				//cx = width;
				//cy = height;

				if ((0xa0 < static_cast<unsigned char>(pStr[i+1])) && (static_cast<unsigned char>(pStr[i+1]) < 0xff))
				{
					if (!m_font.GetBitmapBits(pData, m_cx, cx, cy, *pWord, color, FRAME_BUFFER_PIXSEL_NUM, zoom))
					{
						break;
					}

					i += 2;
				}
				else
				{
					if (width < 8)
					{
						break;
					}
					else
					{
						m_font.GetBitmapBits(pData, m_cx, cx, cy, '?', color, FRAME_BUFFER_PIXSEL_NUM, zoom);
						//cx = 16;
						i += 2;
					}
				}
			}
			else
			{
				if (width < 8)
				{
					break;
				}
				else
				{
					m_font.GetBitmapBits(pData, m_cx, cx, cy, '?', color, FRAME_BUFFER_PIXSEL_NUM, zoom);
					//cx = 16;
					i += 2;
				}
			}
		}
		else
		{
			cx = width;
			cy = height;

			assert(false);
			m_font.GetBitmapBits(pData, m_cx, cx, cy, '?', color, FRAME_BUFFER_PIXSEL_NUM, zoom);
			//cx = 8;
			++ i;
		}

		pData += (cx * FRAME_BUFFER_PIXSEL_NUM);
		width -= cx;
	}while((0 < width) && (width < m_cx) && (i < len));
#else
	
	if(len <= 0)
	{
		return 0;
	}


	if(!g_langRightOrder)
	{
		//// utf-8 [ucs-2. ISO 10646-1:1993]
		char* utf8Ptr = const_cast<char*>(pStr);

		unsigned long pre_code = 0;
		unsigned long pre_cx = 0;

		do
		{
			cx = width;
			cy = height;

			unsigned long charID = 0;
			int size = 0;

			GUI_UTF8_TO_UCS2(utf8Ptr, charID, size);

			utf8Ptr += size;
			i += size;

			unsigned char* pShowData = pData;

			int ret_thai_check = THAI_RULE_N;
			if((charID >= 0x0E00) && (charID <= 0x0E7F))
			{
				ret_thai_check = ThaiCharRule(charID, pre_code);

				if(THAI_RULE_C == ret_thai_check)
				{
					unsigned short char_cx = 0, char_cy = 0;
					m_font.GetFontSize(char_cx, char_cy, charID, zoom);

					int char_lv = ThaiCharLevel(charID);
					int pre_char_lv = ThaiCharLevel(pre_code);

					if( ((THAI_LV_TOP == char_lv) || (THAI_LV_ABOVE == char_lv)) && 
						((THAI_LV_TOP == pre_char_lv) || (THAI_LV_ABOVE == pre_char_lv)))
					{
						pShowData = pData - char_cx * FRAME_BUFFER_PIXSEL_NUM - m_width * 4;
					}
					else
					{
						pShowData = pData - char_cx * FRAME_BUFFER_PIXSEL_NUM;
					}

					cx += char_cx;
				}
				else if((THAI_RULE_N == ret_thai_check) && (charID == 0x0E33))
				{
					pShowData = pData - 4 * FRAME_BUFFER_PIXSEL_NUM;
				}
			}

			if(cx <= 0)
			{
				/*
					[2011-06-22 L]泰语在宽width=0时, 并不为结束, 后续可能有和前面处于
				同一区域的字符
				*/
				break;
			}

			if(THAI_RULE_X != ret_thai_check)
			{
				int ret = m_font.GetBitmapBits(pShowData, m_cx, cx, cy, charID, color, FRAME_BUFFER_PIXSEL_NUM, zoom, bForceDrawBorder);
				if(ret < 0)
				{
					//字体库中不存在或没创建的字符,使用?替代
					charID = FONT_ERR_CHAR_REPLACE;	//'?'
					m_font.GetBitmapBits(pShowData, m_cx, cx, cy, charID, color, FRAME_BUFFER_PIXSEL_NUM, zoom, bForceDrawBorder);
				}
				else if(ret == CFont::FONT_ERR_OVERFLOW)
				{
					//如果越界,并没有输出字符,需回退
					i -= size;
					break;
				}
			}

			if(THAI_RULE_N == ret_thai_check)
			{
				if((charID == 0x0E33) && (cx >= 4))
				{
					cx -= 4;
				}

				pData += (cx * FRAME_BUFFER_PIXSEL_NUM);
				width -= cx;
				pre_cx = cx;
			}

			pre_code = charID;

		}while((0 <= width) && (width < m_cx) && (i < len));
	}
	else
	{
		pData = m_pDrawBuf + m_width*y + x*FRAME_BUFFER_PIXSEL_NUM + width * FRAME_BUFFER_PIXSEL_NUM;

		//// utf-8 [ucs-2. ISO 10646-1:1993]

		char* pNewStr = NULL;
		char* utf8Ptr = NULL;
		if(bResort)
		{
			pNewStr = new char[len + 1];
			memset(pNewStr, 0, len + 1);
			ReSortText(pNewStr, pStr, len);
			utf8Ptr = pNewStr;
		}
		else
		{
			utf8Ptr = const_cast<char*>(pStr);
		}

		unsigned long pre_code = 0;
		bool bIgnoreNext = false;
		
		do
		{
			cx = width;
			cy = height;

			unsigned long charID = 0;
			int size = 0;

			GUI_UTF8_TO_UCS2(utf8Ptr, charID, size);

			utf8Ptr += size;
			i += size;

			 unsigned long oldID = charID;

			bool bIgnore = false;
			if((charID >= 0x0600) && (charID <= 0x06FF))
			{
				//阿拉伯语
				unsigned long next_code = 0;

				if(i < len)
				{
					int tmp_size = 0;

					GUI_UTF8_TO_UCS2(utf8Ptr, next_code, tmp_size);
				}

				charID = ArabicCharRule(pre_code, charID, next_code, bIgnore);
			}

			if(!bIgnoreNext)
			{
				unsigned short chcx = 0, chcy = 0;
				if(m_font.GetFontSize(chcx, chcy, charID, zoom))
				{
					pData -= chcx * FRAME_BUFFER_PIXSEL_NUM;
				}
				int ret = m_font.GetBitmapBits(pData, m_cx, cx, cy, charID, color, FRAME_BUFFER_PIXSEL_NUM, zoom, bForceDrawBorder);
				if(ret < 0)
				{
					charID = FONT_ERR_CHAR_REPLACE;	//'?'
					m_font.GetFontSize(chcx, chcy, charID, zoom);
					pData -= chcx * FRAME_BUFFER_PIXSEL_NUM;
					m_font.GetBitmapBits(pData, m_cx, cx, cy, charID, color, FRAME_BUFFER_PIXSEL_NUM, zoom, bForceDrawBorder);
				}
				else if(ret == CFont::FONT_ERR_OVERFLOW)
				{
					i -= size;
					break;
				}

				width -= cx;
			}
			else
			{
				bIgnoreNext = false;
			}

			if(bIgnore)
			{
				bIgnoreNext = true;
			}

			pre_code = oldID;

		}while((0 < width) && (width < m_cx) && (i < len));

		if(NULL != pNewStr)
		{
			delete [] pNewStr;
		}
	}

#endif

	return i;
}

void GUI::CFBInterface::DrawImage(const unsigned char *pImage, unsigned long len, unsigned short x, unsigned short y, unsigned short width, unsigned short cx, unsigned short cy)
{
	assert (NULL != m_pDrawBuf);

	//进行尺寸修正，以免超出范围
	cx = ((x+cx) > m_cx) ? (m_cx-x) : cx;
	cy = ((y+cy) > m_cy) ? (m_cy-y) : cy;

	unsigned char *pBuf = NULL, *pSrcBuf = NULL;

#ifdef __TDHISI__
	ext_tde_begin_job();
	pBuf = m_pDrawBuf + m_width*y + x*FRAME_BUFFER_PIXSEL_NUM;
	pSrcBuf = (unsigned char *)(pImage);

	ext_fb_copy(m_fb_buf_addr_phy+(pBuf-m_pFrameBuffer), m_app_buf_addr_phy+(pSrcBuf-m_pAppBuf), width, m_fb_var.yres, cx, cy, 0x11);
	ext_tde_end_job();
#else
	for (unsigned short h=0; h<cy; ++h)
	{
		pBuf = m_pDrawBuf + m_width*(y + h) + x*FRAME_BUFFER_PIXSEL_NUM;
		pSrcBuf = (unsigned char *)(pImage + width*h*FRAME_BUFFER_PIXSEL_NUM);

		memcpy(pBuf, pSrcBuf, cx*FRAME_BUFFER_PIXSEL_NUM);
	}
#endif
}

void GUI::CFBInterface::DrawMice(unsigned short x, unsigned short y)
{
#ifdef FB_MOUSE
	if ((NULL != m_pDrawBuf) && (0 < m_cx) && (0 < m_cy))
	{
		assert ((0 <= x) && (x < m_cx));
		assert ((0 <= y) && (y < m_cy));

		g_mouse_lock.Lock();

		unsigned char yNum = ((m_miceInfo.y + 20) < m_cy) ? 20 : (m_cy - m_miceInfo.y);
		unsigned char xNum = ((m_miceInfo.x + 16) < m_cx) ? 16 : (m_cx - m_miceInfo.x);

#ifndef __CHIPTI__


		unsigned long *pBuf = (unsigned long*)(m_pFrameBuffer + m_width*m_miceInfo.y + m_miceInfo.x*FRAME_BUFFER_PIXSEL_NUM);
		unsigned long *pSrc = (unsigned long *)(g_fb_bk);

		//把原来位置的数据写回去
		for (unsigned char h=0; h<yNum; ++h)
		{
			memcpy(pBuf, pSrc, xNum*FRAME_BUFFER_PIXSEL_NUM);
			pBuf += m_cx;
			pSrc += 16;
		}

		m_miceInfo.x = x;
		m_miceInfo.y = y;

		//要根据新的位置来定区域。
		yNum = ((m_miceInfo.y + 20) < m_cy) ? 20 : (m_cy - m_miceInfo.y);
		xNum = ((m_miceInfo.x + 16) < m_cx) ? 16 : (m_cx - m_miceInfo.x);

		//备份新位置的数据
		pBuf = (unsigned long*)(m_pFrameBuffer + m_width*m_miceInfo.y + m_miceInfo.x*FRAME_BUFFER_PIXSEL_NUM);
		pSrc = (unsigned long *)(g_fb_bk);

		for (unsigned char h=0; h<yNum; ++h)
		{
			memcpy(pSrc, pBuf, xNum*FRAME_BUFFER_PIXSEL_NUM);
			pBuf += m_cx;
			pSrc += 16;
		}

		//绘制光标
		pBuf = (unsigned long*)(m_pFrameBuffer + m_width*m_miceInfo.y + m_miceInfo.x*FRAME_BUFFER_PIXSEL_NUM);
		pSrc = (unsigned long*)(CurseValue);
		for (unsigned char h=0; h<yNum; ++h)
		{
			for (unsigned char w=0; w<xNum; ++w)
			{
				if (0 != pSrc[w])
				{
					pBuf[w] = (pSrc[w] | 0xff000000);
				}
			}
			
			pBuf += m_cx;
			pSrc += 32;
		}


#else
		unsigned short *pBuf = (unsigned short*)(m_pFrameBuffer + m_width*m_miceInfo.y + m_miceInfo.x*FRAME_BUFFER_PIXSEL_NUM);
		unsigned long *pSrc = (unsigned long *)(g_fb_bk);

		//把原来位置的数据写回去
		for (unsigned char h=0; h<yNum; ++h)
		{
			memcpy(pBuf, pSrc, xNum*FRAME_BUFFER_PIXSEL_NUM);
			pBuf += m_cx;
			pSrc += 16;
		}

		m_miceInfo.x = x;
		m_miceInfo.y = y;

		//要根据新的位置来定区域。
		yNum = ((m_miceInfo.y + 20) < m_cy) ? 20 : (m_cy - m_miceInfo.y);
		xNum = ((m_miceInfo.x + 16) < m_cx) ? 16 : (m_cx - m_miceInfo.x);

		//备份新位置的数据
		pBuf = (unsigned short*)(m_pFrameBuffer + m_width*m_miceInfo.y + m_miceInfo.x*FRAME_BUFFER_PIXSEL_NUM);
		pSrc = (unsigned long *)(g_fb_bk);

		for (unsigned char h=0; h<yNum; ++h)
		{
			memcpy(pSrc, pBuf, xNum*FRAME_BUFFER_PIXSEL_NUM);
			pBuf += m_cx;
			pSrc += 16;
		}

		//绘制光标
		pBuf = (unsigned short*)(m_pFrameBuffer + m_width*m_miceInfo.y + m_miceInfo.x*FRAME_BUFFER_PIXSEL_NUM);
		unsigned long * pBK1 = (unsigned long*)(CurseValue);
		for (unsigned char h=0; h<yNum; ++h)
		{
			for (unsigned char w=0; w<xNum; ++w)
			{
				if (0 != pBK1[w])
				{
					pBuf[w] = ((pBK1[w]&0x7fff) | 0x8000);
				}
			}

			pBuf += m_cx;
			pBK1 += 32;
		}
#endif
		g_mouse_lock.UnLock();
	}
#endif
}

bool GUI::CFBInterface::TextSize(const char *pStr, unsigned long len, unsigned short &cx, unsigned short &cy, unsigned char zoom/* = 1*/)
{
#if 0
	//// gb2312
	cx = 0;
	cy = 0;

	unsigned short offsetCx = 9;
	unsigned short offsetCy = 20;
	m_font.GetFontSize(offsetCx, offsetCy, '?', zoom);


	unsigned short *pWord = NULL, xSize = 0, ySize = 0;
	long i = 0;
	do
	{
		if ((0 <= static_cast<unsigned char>(pStr[i])) && (static_cast<unsigned char>(pStr[i]) < 0x21))
		{
			//do nothing
			++ i;
			xSize = (8 * zoom);
		}
		else if ((0x21 <= static_cast<unsigned char>(pStr[i])) && (static_cast<unsigned char>(pStr[i]) < 0x7f))	//english char
		{
			m_font.GetFontSize(xSize, ySize, pStr[i++], zoom);
		}
		else if(0x7f <= static_cast<unsigned char>(pStr[i]))
		{
			//双字节字符
			if ((0x7f <= static_cast<unsigned char>(pStr[i])) && (static_cast<unsigned char>(pStr[i]) < 0xa1))
			{
				xSize = offsetCx;
				i += 2;
			}
			else if (!((0xa9 < static_cast<unsigned char>(pStr[i])) && (static_cast<unsigned char>(pStr[i]) < 0xb0)) && (static_cast<unsigned char>(pStr[i]) < 0xf8))
			{
				if ((0xa0 < static_cast<unsigned char>(pStr[i+1])) && (static_cast<unsigned char>(pStr[i+1]) < 0xff))
				{
					pWord = (unsigned short *)(pStr+i);
					m_font.GetFontSize(xSize, ySize, *pWord, zoom);

					i += 2;
				}
				else
				{
					xSize = offsetCx;
					i += 2;
				}
			}
			else
			{
				xSize = offsetCx;
				i += 2;
			}
		}
		else
		{
			xSize = offsetCx;
			++ i;
		}

		cx += xSize;
		cy = ySize;
	}while(i < len);

#else

	//// uft-8 [ucs-2]
	cx = 0;
	cy = 0;

	if(len <= 0)
	{
		return false;
	}

	unsigned short offsetCx = 9;
	unsigned short offsetCy = 20;
	m_font.GetFontSize(offsetCx, offsetCy, FONT_ERR_CHAR_REPLACE, zoom);//'?'

	if(!g_langRightOrder)
	{
		unsigned short *pWord = NULL, xSize = 0, ySize = 0;
		long i = 0;
		char* utf8Ptr = const_cast<char*>(pStr);

		unsigned long pre_code = 0;

		do
		{
			unsigned long charID = 0;
			int size = 0;
			GUI_UTF8_TO_UCS2(utf8Ptr, charID, size);

			i += size;
			utf8Ptr += size;

			if(!m_font.GetFontSize(xSize, ySize, charID, zoom))
			{
				xSize = offsetCx;
				ySize = offsetCy;
			}

			if(charID >= 0x0E00 && charID <= 0x0E7F)
			{
				int ret = ThaiCharRule(charID, pre_code);
				switch (ret)
				{
				case THAI_RULE_N:
					{
						cx += xSize;
						cy = ySize;
					}
					break;
				}
			}
			else
			{
				cx += xSize;
				cy = ySize;
			}

			pre_code = charID;
		}while(i < len);
	}
	else
	{
		bool bResort = true;
		char* pNewStr = NULL;
		char* utf8Ptr = NULL;
		if(bResort)
		{
			pNewStr = new char[len + 1];
			memset(pNewStr, 0, len + 1);
			ReSortText(pNewStr, pStr, len);
			utf8Ptr = pNewStr;
		}
		else
		{
			utf8Ptr = const_cast<char*>(pStr);
		}

		unsigned short *pWord = NULL, xSize = 0, ySize = 0;
		long i = 0;
		unsigned long pre_code = 0;
		bool bIgnoreNext = false;

		do
		{
			unsigned long charID = 0;
			int size = 0;

			GUI_UTF8_TO_UCS2(utf8Ptr, charID, size);

			utf8Ptr += size;
			i += size;

			unsigned long oldID = charID;

			bool bIgnore = false;
			if((charID >= 0x0600) && (charID <= 0x06FF))
			{
				//阿拉伯语
				unsigned long next_code = 0;

				if(i < len)
				{
					int tmp_size = 0;

					GUI_UTF8_TO_UCS2(utf8Ptr, next_code, tmp_size);
				}

				charID = ArabicCharRule(pre_code, charID, next_code, bIgnore);
			}

			if(!bIgnoreNext)
			{
				if(!m_font.GetFontSize(xSize, ySize, charID, zoom))
				{
					xSize = offsetCx;
					ySize = offsetCy;
				}
			}
			else
			{
				bIgnoreNext = false;
			}

			if(bIgnore)
			{
				bIgnoreNext = true;
			}

			cx += xSize;
			cy = ySize;

			pre_code = oldID;
		}while(i < len);

		if(NULL != pNewStr)
		{
			delete [] pNewStr;
		}
	}

#endif

	return true;
}

#ifdef __TDHISI__
const unsigned long MAX_FB_BUF_LEN = 6*1024*1024;
const unsigned long MAX_FB_BUF_APP_OFFSET = MAX_FB_BUF_LEN - (1*1024*1024);
//0:代表放置不变化图形的区域，1：代表放置变化的图形区域，该区域重复使用。
unsigned char *GUI::CFBInterface::ImageBufAddr(unsigned char bufIndex)
{
	m_app_buf_addr_phy = 0;
	m_pAppBuf = NULL;
	int len = 0;

	if (0 == ext_app_buf_addr(&m_pAppBuf, &m_app_buf_addr_phy, &len))
	{
		if (0 == bufIndex)
		{
			return m_pAppBuf;	//可用空间为MAX_FB_BUF_LEN
		}
		else
		{
			return (m_pAppBuf + MAX_FB_BUF_APP_OFFSET);	//前面MAX_FB_BUF_APP_OFFSET不能使用，可用空间为1MB
		}
	}
	else
	{
		assert (false);
	}
}
#endif

//////////////////////////////////////////////////////////////////////////
void GUI::CFBInterface::RefreshLive()
{
	if (!m_bEnableRefreshLive)
	{
		return;
	}

	//首先进行清屏动作。
	Clear(0, 0, m_cx, m_cy);

	ULONGLONG videoLoss = 0, motion = 0, netDeviceStatus = 0, poeOnlineStatus = 0;

	std::string name;
	CMessageMan *pMsgMan = CMessageMan::Instance();

	const char *pNoAuth = CStringTable::Instance()->String(E_STR_ID_NO_AHTHORITY);
#if defined(__TW01_RILI__)
	const char *pVideoLoss = " ";
#else
	const char *pVideoLoss = CStringTable::Instance()->String(E_STR_ID_VIDEO_LOSS);
#endif
	const char *pDisconnected = CStringTable::Instance()->String(E_STR_ID_IPCAM_CONNECTING);

	unsigned short str_cx_a = 0, str_cy_a = 0, str_len_a = strlen(pNoAuth);
	unsigned short str_cx = 0, str_cy = 0, str_len = strlen(pVideoLoss);

	TextSize(pNoAuth, str_len_a, str_cx_a, str_cy_a);
	TextSize(pVideoLoss, str_len, str_cx, str_cy);

	unsigned short x_off = 20, y_off = 20, height = str_cy;

	unsigned long recType = RECORD_TYPE_NULL;
	unsigned long recColor = COLOR_SELECT;

	pMsgMan->GetCHStatus(CHNN_STATUS_VIDEO_LOSS, videoLoss);
	pMsgMan->GetCHStatus(CHNN_STATUS_MOTION, motion);
	netDeviceStatus = pMsgMan->GetNetDeviceStatus();
	poeOnlineStatus = pMsgMan->GetPoeOnlineStatus();

	//unsigned char index = 0;
	bool bDrawBorder[36] = {false};
	unsigned long splitMode = 0;

	unsigned short str_cx_cam = 0, str_cy_cam = 0;
	//然后绘制需要显示的内容。
	unsigned char realChannel;
	for (unsigned char i = 0; i < m_chnnNum; ++i)
	{
		if(DISPLAY_NONE != m_displayAttrib[i].bShow)
		{
			//现场和回放都需要画分割线
			//2010-02-03 18:16:00 YSW
			//当输出设备为CVBS时，横线几乎没有，竖线都看不清楚，所以在此做了特殊处理。
			if (m_displayAttrib[i].zOrder >= 4)
			{
				//下面是对画中画的边框对特殊的处理
				if (m_cx >= 800)
				{
					DrawRectBorder(m_displayAttrib[i].xPos - 1, m_displayAttrib[i].yPos - 1, m_displayAttrib[i].cx + 2, m_displayAttrib[i].cy + 2, \
						GUI_RGB(96, 96, 96), (m_cx < 800) ? 2 : 1, 1);
				}
			
				DrawRectBorder(m_displayAttrib[i].xPos, m_displayAttrib[i].yPos, m_displayAttrib[i].cx, m_displayAttrib[i].cy, \
					GUI_RGB(128, 128, 128), (m_cx < 800) ? 2 : 1, 1);
			}
			else
			{
				DISPLAY_ATTRIB attrib;
				memcpy(&attrib, &m_displayAttrib[i], sizeof(DISPLAY_ATTRIB));
#if defined(__TDFH__) || defined(__CHIPTI__)
				if (m_cx < (attrib.xPos + attrib.cx + 10))
				{
					attrib.cx = (m_cx - attrib.xPos );
					if (m_cx < 800)
					{
						attrib.cx -= 1;
					}
				}
#endif
				DrawRectBorder(attrib.xPos, attrib.yPos, attrib.cx, attrib.cy, \
					GUI_RGB(128, 128, 128), (m_cx < 800) ? 2 : 1, 1);

			}
			
			x_off = 20, y_off = 5, height = str_cy;

			//如果是现场则根据配置显示通道名称，而回放则需要总是显示通道名称
			if ((DISPLAY_LIVE == m_displayAttrib[i].bShow) || (DISPLAY_HIDE == m_displayAttrib[i].bShow)\
				|| (DISPLAY_NET == m_displayAttrib[i].bShow) || (DISPLAY_INVISIBLE == m_displayAttrib[i].bShow))
			{
				realChannel = m_displayAttrib[i].chnn;
				if (pMsgMan->ShowName(realChannel) && pMsgMan->GetCamName(realChannel, name))
				{
#ifdef __SUPPORT_OSDTWOLINES__
					//2011-06-15 ZYX
					unsigned long xPos, yPos;
					pMsgMan->GetLiveCamPos(realChannel, xPos, yPos);
					int firstLineNum = name.find('/');
					if (-1 == firstLineNum)
					{
						TextSize(name.c_str(), strlen(name.c_str()), str_cx_cam, str_cy_cam);
					}
					else
					{
						TextSize(name.c_str(), firstLineNum, str_cx_cam, str_cy_cam);
						unsigned short new_str_cx=0, new_str_cy=0;
						TextSize(name.c_str()+firstLineNum+1, strlen(name.c_str())-firstLineNum-1, new_str_cx, new_str_cy);
						if (new_str_cx > str_cx_cam)
						{
							str_cx_cam = new_str_cx;
						}
						str_cy_cam += str_cy_cam;
					}
					if (m_displayAttrib[i].cx - 2 < str_cx_cam)
					{
						str_cx_cam = m_displayAttrib[i].cx - 2;
					}
					x_off = xPos*m_cx*(m_displayAttrib[i].cx - str_cx_cam)/100/(m_cx - str_cx_cam);
					y_off = yPos*m_cy*(m_displayAttrib[i].cy - str_cy_cam)/100/(m_cy - str_cy_cam);
					if (x_off + str_cx_cam + 2 > m_displayAttrib[i].cx)
					{
						x_off = m_displayAttrib[i].cx - str_cx_cam - 2;
					}
					if (y_off + str_cy_cam + 2 > m_displayAttrib[i].cy)
					{
						y_off = m_displayAttrib[i].cy - str_cy_cam - 2;
					}
					//
#endif
					//2010-02-03 17:40:00 YSW
					//最上面一行的通道名称显示在图像底部
					if ((m_cx < 800) && (0 == m_displayAttrib[i].yPos) && (m_displayAttrib[i].cy < m_cy))
					{
						y_off = m_displayAttrib[i].cy - height - y_off;
					}
					else if ((m_cx < 800) && (0 == m_displayAttrib[i].yPos))
					{
						//大画面且为监视器则应该做些偏移
						y_off += 20;
					}

					if ((m_cx < 800) && (0 == m_displayAttrib[i].xPos))
					{
						x_off += 20;
					}

					if (m_cx < 800)
					{
#ifdef __SUPPORT_OSDTWOLINES__
						//2011-06-15 ZYX
						if (-1 == firstLineNum)
						{
							TextSize(name.c_str(), strlen(name.c_str()), str_cx_cam, str_cy_cam);
							DrawRect(m_displayAttrib[i].xPos+x_off, m_displayAttrib[i].yPos+y_off, str_cx_cam, str_cy_cam, 0xff282828);
						}
						else
						{
							TextSize(name.c_str(), firstLineNum, str_cx_cam, str_cy_cam);
							DrawRect(m_displayAttrib[i].xPos+x_off, m_displayAttrib[i].yPos+y_off, str_cx_cam, str_cy_cam, 0xff282828);
							TextSize(name.c_str()+firstLineNum+1, strlen(name.c_str())-firstLineNum-1, str_cx_cam, str_cy_cam);
							DrawRect(m_displayAttrib[i].xPos+x_off, m_displayAttrib[i].yPos+y_off+str_cy_cam, str_cx_cam, str_cy_cam, 0xff282828);
						}
#else
						TextSize(name.c_str(), strlen(name.c_str()), str_cx_cam, str_cy_cam);
						DrawRect(m_displayAttrib[i].xPos+x_off, m_displayAttrib[i].yPos+y_off, str_cx_cam, str_cy_cam, 0xff282828);
#endif
					}
					
#ifdef __SUPPORT_OSDTWOLINES__
					if (-1 == firstLineNum)
					{
						DrawText(name.c_str(), name.length(), m_displayAttrib[i].xPos+x_off, m_displayAttrib[i].yPos+y_off, m_displayAttrib[i].cx-x_off-2, height, COLOR_LINE_LIVE, 1, true);
					}
					else
					{
						DrawText(name.c_str(), firstLineNum, m_displayAttrib[i].xPos+x_off, m_displayAttrib[i].yPos+y_off, m_displayAttrib[i].cx-x_off-2, height, COLOR_LINE_LIVE, 1, true);
						DrawText(name.c_str()+firstLineNum+1, name.length()-firstLineNum-1, m_displayAttrib[i].xPos+x_off, m_displayAttrib[i].yPos+y_off+str_cy_cam/2, m_displayAttrib[i].cx-x_off-2, height, COLOR_LINE_LIVE, 1, true);
					}
#else
					DrawText(name.c_str(), name.length(), m_displayAttrib[i].xPos+x_off, m_displayAttrib[i].yPos+y_off, m_displayAttrib[i].cx-x_off-2, height, COLOR_LINE_LIVE, 1, true);

#endif
					//
				}
				
			}
			else////回放总是显示通道名称和快进快退速度
			{
#if defined(__ASYN_PLAY__)
				if(DISPLAY_PLAYBACK == m_displayAttrib[i].bShow)
				{
					unsigned long speed = pMsgMan->GetShowSpeed(m_displayAttrib[i].chnn);
					char str[64], tmp[64];
					memset(str, 0, 64);
					memset(tmp, 0, 64);

					if( (speed & (~(0x1 << 16)))!=PLAY_SPEED_1X)
					{
						if (speed >> 16)
						{
							snprintf(str, 64, ">>");
						}
						else
						{
							snprintf(str, 64, "<<");
						}
					}
					else
					{
						snprintf(str, 64, ">>");
					}
					speed &= (~(0x1 << 16));
					switch(speed)
					{
					case    PLAY_SPEED_1X:
					case	PLAY_SPEED_2X:
					case	PLAY_SPEED_4X:
					case	PLAY_SPEED_8X:
					case	PLAY_SPEED_16X:
					case	PLAY_SPEED_32X:
						speed = 0x1 << (speed - 5);
						snprintf(tmp, 64, "%s%dX", str, speed);
						break;
					case	PLAY_SPEED_1_32X:
					case	PLAY_SPEED_1_16X:
					case	PLAY_SPEED_1_8X:
					case	PLAY_SPEED_1_4X:
					case	PLAY_SPEED_1_2X:
						speed = 32 / (0x1 << speed);
						snprintf(tmp, 64, "%s1/%dX", str, speed);
						break;
					case PLAY_SPEED_NONE:
						speed =0;
						memset(tmp,0,sizeof(tmp));
						break;
					case PLAY_SPEED_PAUSE:
						speed=0;
						snprintf(tmp, 64, "||");
						break;
					default:
						break;
					}

					unsigned short speed_x_pos, speed_y_pos, speed_str_length, speed_str_height;
					TextSize(tmp, strlen(tmp), speed_str_length, speed_str_height);
					speed_x_pos = m_displayAttrib[i].xPos+m_displayAttrib[i].cx -speed_str_length -x_off;
					speed_y_pos = m_displayAttrib[i].yPos+y_off;
					DrawText(tmp, strlen(tmp), speed_x_pos, speed_y_pos, speed_str_length, speed_str_height, COLOR_LINE_LIVE, 1, true);
				}
				
#endif				
				long channel = m_displayAttrib[i].chnn;
				channel += (channel < m_netVideoInputNum) ? m_localVideoInputNum : -m_netVideoInputNum;
				if (pMsgMan->PBShowCamName(channel) && pMsgMan->GetCamName(channel, name))
				{
#ifdef __SUPPORT_OSDTWOLINES__
					//2011-06-15 ZYX
					unsigned long xPos, yPos;
					pMsgMan->GetLiveCamPos(realChannel, xPos, yPos);
					int firstLineNum = name.find('/');
					if (-1 == firstLineNum)
					{
						TextSize(name.c_str(), strlen(name.c_str()), str_cx_cam, str_cy_cam);
					}
					else
					{
						TextSize(name.c_str(), firstLineNum, str_cx_cam, str_cy_cam);
						unsigned short new_str_cx=0, new_str_cy=0;
						TextSize(name.c_str()+firstLineNum+1, strlen(name.c_str())-firstLineNum-1, new_str_cx, new_str_cy);
						if (new_str_cx > str_cx_cam)
						{
							str_cx_cam = new_str_cx;
						}
						str_cy_cam += str_cy_cam;
					}
					if (m_displayAttrib[i].cx - 2 < str_cx_cam)
					{
						str_cx_cam = m_displayAttrib[i].cx - 2;
					}
					x_off = xPos*m_cx*(m_displayAttrib[i].cx - str_cx_cam)/100/(m_cx - str_cx_cam);
					y_off = yPos*m_cy*(m_displayAttrib[i].cy - str_cy_cam)/100/(m_cy - str_cy_cam);
					if (x_off + str_cx_cam + 2 > m_displayAttrib[i].cx)
					{
						x_off = m_displayAttrib[i].cx - str_cx_cam - 2;
					}
					if (y_off + str_cy_cam + 2 > m_displayAttrib[i].cy)
					{
						y_off = m_displayAttrib[i].cy - str_cy_cam - 2;
					}
					//
#endif
					//2010-02-03 17:40:00 YSW
					//最上面一行的通道名称显示在图像底部
					if ((m_cx < 800) && (0 == m_displayAttrib[i].yPos) && (m_displayAttrib[i].cy < m_cy))
					{
						y_off = m_displayAttrib[i].cy - height - y_off;
					}
					else if ((m_cx < 800) && (0 == m_displayAttrib[i].yPos))
					{
						//大画面且为监视器则应该做些偏移
						y_off += 20;
					}

					if ((m_cx < 800) && (0 == m_displayAttrib[i].xPos))
					{
						x_off += 20;
					}

					if (m_cx < 800)
					{
#ifdef __SUPPORT_OSDTWOLINES__
						//2011-06-15 ZYX
						if (-1 == firstLineNum)
						{
							TextSize(name.c_str(), strlen(name.c_str()), str_cx_cam, str_cy_cam);
							DrawRect(m_displayAttrib[i].xPos+x_off, m_displayAttrib[i].yPos+y_off, str_cx_cam, str_cy_cam, 0xff282828);
						}
						else
						{
							TextSize(name.c_str(), firstLineNum, str_cx_cam, str_cy_cam);
							DrawRect(m_displayAttrib[i].xPos+x_off, m_displayAttrib[i].yPos+y_off, str_cx_cam, str_cy_cam, 0xff282828);
							TextSize(name.c_str()+firstLineNum+1, strlen(name.c_str())-firstLineNum-1, str_cx_cam, str_cy_cam);
							DrawRect(m_displayAttrib[i].xPos+x_off, m_displayAttrib[i].yPos+y_off+str_cy_cam, str_cx_cam, str_cy_cam, 0xff282828);
						}
#else
						TextSize(name.c_str(), strlen(name.c_str()), str_cx_cam, str_cy_cam);
						DrawRect(m_displayAttrib[i].xPos+x_off, m_displayAttrib[i].yPos+y_off, str_cx_cam, str_cy_cam, 0xff282828);
#endif
					}
#ifdef __SUPPORT_OSDTWOLINES__
					if (-1 == firstLineNum)
					{
						DrawText(name.c_str(), name.length(), m_displayAttrib[i].xPos+x_off, m_displayAttrib[i].yPos+y_off, m_displayAttrib[i].cx-x_off-2, height, COLOR_LINE_LIVE, 1, true);
					}
					else
					{
						DrawText(name.c_str(), firstLineNum, m_displayAttrib[i].xPos+x_off, m_displayAttrib[i].yPos+y_off, m_displayAttrib[i].cx-x_off-2, height, COLOR_LINE_LIVE, 1, true);
						DrawText(name.c_str()+firstLineNum+1, name.length()-firstLineNum-1, m_displayAttrib[i].xPos+x_off, m_displayAttrib[i].yPos+y_off+str_cy_cam/2, m_displayAttrib[i].cx-x_off-2, height, COLOR_LINE_LIVE, 1, true);
					}
#else
					DrawText(name.c_str(), name.length(), m_displayAttrib[i].xPos+x_off, m_displayAttrib[i].yPos+y_off, m_displayAttrib[i].cx-x_off-2, height, COLOR_LINE_LIVE, 1, true);
#endif
					//
				}
			}

			//如果是现场显示，则需要刷新录像状态和视频丢失信号
			if ((DISPLAY_LIVE == m_displayAttrib[i].bShow) || (DISPLAY_HIDE == m_displayAttrib[i].bShow) \
				 || (DISPLAY_NET == m_displayAttrib[i].bShow))
			{
				realChannel = m_displayAttrib[i].chnn;

				x_off = 20; y_off = 28; height = 10;

				//2010-02-03 19:15:00 YSW
				//报警录像和手动/定时录像的标记颜色不一致。
				recType = pMsgMan->GetRecStatus(realChannel);
				if (RECORD_TYPE_NULL != recType)
				{
					//2010-02-03 17:40:00 YSW
					//最上面一行的通道录像状态显示在图像底部
					//如果是大画面，则应该在上部
					if ((m_cx < 800) && (0 == m_displayAttrib[i].yPos) && (m_displayAttrib[i].cy < m_cy))
					{
						y_off = m_displayAttrib[i].cy - str_cy - 15;
					}
					else if ((m_cx < 800) && (0 == m_displayAttrib[i].yPos))
					{
						//大画面且为监视器则应该做些偏移
						y_off += 20;
					}

					if ((m_cx < 800) && (0 == m_displayAttrib[i].xPos))
					{
						x_off += 20;
					}

					if (RECORD_TYPE_SENSOR & recType)
					{
#if defined(__TW01_RILI__)
						recColor = GUI_RGB(0, 255, 0);
#else
						recColor = GUI_RGB(255, 0, 0);
#endif
					}
					else if (RECORD_TYPE_MOTION & recType)
					{
						recColor = GUI_RGB(255, 255, 0);
					}
					else if (RECORD_TYPE_MANUAL & recType)
					{
#if defined(__TW01_RILI__)
						recColor = GUI_RGB(255, 0, 0);
#else
						recColor = GUI_RGB(0, 255, 0);
#endif
					}
					else
					{
						recColor = GUI_RGB(0, 0, 255);
					}

					if (0 != m_showRecStatusCH)
					{
						DrawRect(m_displayAttrib[i].xPos+x_off, m_displayAttrib[i].yPos+y_off, height, height, recColor);
					}					
				}

				//因为现场音频、云台控制等信息和录像状态在一行，所以紧跟其后
				//省去再次计算坐标的步骤。
				x_off += 20;
				y_off -= 3;
				height = 16;
				unsigned long dataLen = 16*16*FRAME_BUFFER_PIXSEL_NUM;

				////提示现场音频
				if (pMsgMan->AudioChnn() == realChannel)
				{
					DrawImage(m_pAudioImage, dataLen, m_displayAttrib[i].xPos+x_off, m_displayAttrib[i].yPos+y_off, height, height, height);
				}

				//提示云台控制
				x_off += 20;
				if (pMsgMan->CurrPTZChnn() == realChannel)
				{
					DrawImage(m_pPTZImage, dataLen, m_displayAttrib[i].xPos+x_off, m_displayAttrib[i].yPos+y_off, height, height, height); 
				}

				//视频丢失信息要居中摆放，重新计算指标位置。
				x_off = (m_displayAttrib[i].cx >> 1)-(str_cx >> 1);
				y_off = (m_displayAttrib[i].cy >> 1)-(str_cy >> 1);
				height = str_cy;

				//优先显示视频丢失信息，次优显示无权限信息
				if ((DISPLAY_HIDE != m_displayAttrib[i].bShow) && (videoLoss & ((ULONGLONG)0x01 << realChannel)))
				{
					//如果有logo,显示logo; logo图的宽度可能在小屏下过界
					if(m_pLogoImage)
					{
#if defined(__CUSTOM_US02__) || defined(__TD32SPEC__)
						if (16 < m_chnnNum)
						{
							DrawText(pVideoLoss, str_len, m_displayAttrib[i].xPos+x_off, m_displayAttrib[i].yPos+y_off, m_displayAttrib[i].cx-x_off-2, height, COLOR_LINE_LIVE, 1, true);
						}
						else
						{
							int x_logo = (m_displayAttrib[i].cx > (m_logoWidth + 4)) ? (m_displayAttrib[i].xPos + (m_displayAttrib[i].cx >> 1) - (m_logoWidth >> 1)) : (m_displayAttrib[i].xPos + 2);
							int cx_logo = (m_displayAttrib[i].cx > (m_logoWidth + 4)) ? m_logoWidth : (m_displayAttrib[i].cx - 4);
							int y_logo = m_displayAttrib[i].yPos + (m_displayAttrib[i].cy >> 1) - (m_logoHeight >> 1);
							DrawImage(m_pLogoImage, m_logoLen, x_logo, y_logo, m_logoWidth, cx_logo, m_logoHeight);
							DrawText(pVideoLoss, str_len, m_displayAttrib[i].xPos+x_off, y_logo + m_logoHeight, m_displayAttrib[i].cx-x_off-2, height, COLOR_LINE_LIVE, 1, true);
						}
#else
						PRODUCT_TYPE productType = CProduct::Instance()->ProductType();
						if ((TD_2532HD_C == productType) || (TD_2524HD_C == productType))
						{
							DrawText(pVideoLoss, str_len, m_displayAttrib[i].xPos+x_off, m_displayAttrib[i].yPos+y_off, m_displayAttrib[i].cx-x_off-2, height, COLOR_LINE_LIVE, 1, true);
						}
						else
						{
							int x_logo = (m_displayAttrib[i].cx > (m_logoWidth + 4)) ? (m_displayAttrib[i].xPos + (m_displayAttrib[i].cx >> 1) - (m_logoWidth >> 1)) : (m_displayAttrib[i].xPos + 2);
							int cx_logo = (m_displayAttrib[i].cx > (m_logoWidth + 4)) ? m_logoWidth : (m_displayAttrib[i].cx - 4);
							int y_logo = m_displayAttrib[i].yPos + (m_displayAttrib[i].cy >> 1) - (m_logoHeight >> 1);
							DrawImage(m_pLogoImage, m_logoLen, x_logo, y_logo, m_logoWidth, cx_logo, m_logoHeight);
							DrawText(pVideoLoss, str_len, m_displayAttrib[i].xPos+x_off, y_logo + m_logoHeight, m_displayAttrib[i].cx-x_off-2, height, COLOR_LINE_LIVE, 1, true);
						}
#endif
					}
					else
					{
						DrawText(pVideoLoss, str_len, m_displayAttrib[i].xPos+x_off, m_displayAttrib[i].yPos+y_off, m_displayAttrib[i].cx-x_off-2, height, COLOR_LINE_LIVE, 1, true);
					}
				}
				else if((DISPLAY_HIDE != m_displayAttrib[i].bShow) && (poeOnlineStatus & netDeviceStatus & ((ULONGLONG)0x01 << realChannel)))
				{
					DrawText(pDisconnected, strlen(pDisconnected), m_displayAttrib[i].xPos+x_off, m_displayAttrib[i].yPos+y_off, m_displayAttrib[i].cx-x_off-2, height, COLOR_LINE_LIVE, 1, true);
				}
				else if (DISPLAY_HIDE == m_displayAttrib[i].bShow)
				{
					x_off = (m_displayAttrib[i].cx >> 1)-(str_cx_a >> 1);
					y_off = (m_displayAttrib[i].cy >> 1)-(str_cy_a >> 1);
					height = str_cy_a;

					DrawText(pNoAuth, str_len_a, m_displayAttrib[i].xPos+x_off, m_displayAttrib[i].yPos+y_off, m_displayAttrib[i].cx-x_off-2, height, COLOR_LINE_LIVE, 1, true);
				}
			}

			//取分割模式
			if( (DISPLAY_LAYER_NULL != m_displayAttrib[i].zOrder) &&
				(DISPLAY_LAYER_MINI != m_displayAttrib[i].zOrder) &&
				(0 == splitMode))
			{
				splitMode = m_liveSplitMode[m_displayAttrib[i].zOrder];
			}

			//标记已经绘制分割线
			bDrawBorder[m_displayAttrib[i].index] = true;
		}
	}

	//[2010-08-23 L] 补齐分割线,
	//bDrawBorder[]记录屏幕上已经绘制了的分割线区域
	unsigned char totalNum = SplitModeToNum(static_cast<VIEW_SPLIT_MODE>(splitMode));
	for (int i = 0; i < totalNum; i++)
	{
		if(!bDrawBorder[i])
		{
			DISPLAY_ATTRIB attrib;
			AreaInfo(m_cx, m_cy, static_cast<VIEW_SPLIT_MODE>(splitMode), attrib, i);
#if defined(__TDFH__) || defined(__CHIPTI__)
			if (m_cx < (attrib.xPos + attrib.cx + 10))
			{
 				attrib.cx = (m_cx - attrib.xPos);
 				if (m_cx < 800)
 				{
 					attrib.cx -= 1;
 				}
			}
#endif
			DrawRectBorder(attrib.xPos, attrib.yPos, attrib.cx, attrib.cy, GUI_RGB(128, 128, 128), (m_cx < 800) ? 2 : 1, 1);
		}

		if(m_initchnn!=INVALID_INDEX(unsigned long))
		{
			int index=AreaIndex(m_cx, m_cy, static_cast<VIEW_SPLIT_MODE>(splitMode),m_pos_x,m_pos_y,1,1);			
			DISPLAY_ATTRIB attribleft;
			if(index !=-1)
			{
				AreaInfo(m_cx, m_cy, static_cast<VIEW_SPLIT_MODE>(splitMode), attribleft,index);
			}
			else
			{
				AreaInfo(m_cx, m_cy, static_cast<VIEW_SPLIT_MODE>(splitMode), attribleft, 0);
			}
			DrawRectBorder(attribleft.xPos, attribleft.yPos, attribleft.cx, attribleft.cy, GUI_RGB(255, 0, 0), (m_cx < 800) ? 2 : 1, 1);
		}


	}

	//绘制抓图提示图片
	unsigned long x_snap_tip = 0, y_snap_tip = 0;
	bool bShowSnapTip = false, bSuccSnapTip = true;

	pMsgMan->ShowSnapTip(bShowSnapTip, bSuccSnapTip);
	if(bShowSnapTip)
	{
		pMsgMan->GetSnapTipPos(x_snap_tip, y_snap_tip);
		
		unsigned long height_snap_tip = 32;
		unsigned long datalen_snap_tip = 32 * 32 * FRAME_BUFFER_PIXSEL_NUM;

		x_snap_tip = ((x_snap_tip + height_snap_tip) > m_cx) ? (m_cx - height_snap_tip - 1) : x_snap_tip;
		y_snap_tip = ((y_snap_tip + height_snap_tip) > m_cy) ? (m_cy - height_snap_tip - 1) : y_snap_tip;

		if(bSuccSnapTip)
		{
			DrawImage(m_pSnapSuccImage, datalen_snap_tip, x_snap_tip, y_snap_tip, height_snap_tip, height_snap_tip, height_snap_tip);
		}
		else
		{
			DrawImage(m_pSnapFailImage, datalen_snap_tip, x_snap_tip, y_snap_tip, height_snap_tip, height_snap_tip, height_snap_tip);
		}
	}
}

void GUI::CFBInterface::SetDisplayAttrib(DISPLAY_ATTRIB *pDisplayAttrib, unsigned char chnnNum, unsigned long* pSplitMode, int splitNum)
{
	assert (NULL != pDisplayAttrib);
	assert (chnnNum <= MAX_CHNN_NUM);
	
	if(chnnNum==0)
	{
		m_bEnableRefreshLive = false;
		return;
	}
	m_bEnableRefreshLive = true;
	
	memcpy(m_displayAttrib, pDisplayAttrib, chnnNum * sizeof(DISPLAY_ATTRIB));
	m_chnnNum	= chnnNum;
	memcpy(m_liveSplitMode, pSplitMode, splitNum * sizeof(unsigned long));
	m_bEnableRefreshLive = true;
}

bool GUI::CFBInterface::GetFont(unsigned char *pFontBuf, unsigned long & bufLen, unsigned short charID)
{
	/*
	if ((0 <= charID) && (charID < 33))
	{
		unsigned long fontInfo;
		fontInfo = (0x20 << 16) + (8 << 8) + 20;

		unsigned long len = sizeof(unsigned long) + ((8*20) >> 2);
		assert (len <= bufLen);

		memcpy(pFontBuf, &fontInfo, sizeof(unsigned long));
		memset(pFontBuf+sizeof(unsigned long), 0, len-sizeof(unsigned long));

		bufLen = len;

		return true;
	}
	else if ((33 <= charID) && (charID < 127))	//english char
	{
		return m_font.GetFont(pFontBuf, bufLen, charID);
	}
	else
	{
		//须与字符存储的顺序一致
		unsigned char hi = charID & 0xFF;
		unsigned char low = (charID >> 8) & 0xFF;

		//双字节字符,只能显示gb2312定义部分
		if((0x7f <= hi) && (hi < 0xa1))
		{
			//占位,非gb2312双字节部分
		}
		else if (!((0xa9 < hi) && (hi < 0xb0)) && (hi < 0xf8))
		{
			if ((0xa0 < low) && (low < 0xff))
			{
				return m_font.GetFont(pFontBuf, bufLen, charID);
			}
		}
		else
		{
			//占位
		}

		return m_font.GetFont(pFontBuf, bufLen, '?');
	}
	*/
	return true;
}

bool GUI::CFBInterface::GetFont(unsigned char *pFontBuf, unsigned long & bufLen, const char *pStr, unsigned long length)
{
	assert (NULL != pFontBuf);
	assert (bufLen > 0);
	assert (NULL != pStr);
	//assert (length > 0);

	memset(pFontBuf, 0, bufLen);

	if(length <= 0)
	{
		bufLen = 0;
		return true;
	}

	unsigned char *pFont = pFontBuf;
	unsigned short *pWord = NULL;
	unsigned long len = 0;
	unsigned long subLen = 0;

	if(!g_langRightOrder)
	{
		char* pUtf8Str = (char*)pStr;
		unsigned long i = 0;
		do
		{
			len = (bufLen - subLen);

			unsigned long charcode = 0;
			int size = 0;

			GUI_UTF8_TO_UCS2(pUtf8Str, charcode, size);

			pUtf8Str += size;
			i += size;

			if(!m_font.GetFont(pFont, len, charcode))
			{
				m_font.GetFont(pFont, len, FONT_ERR_CHAR_REPLACE);
			}

			subLen += FONT_INFO_BUF_LEN;
			pFont += FONT_INFO_BUF_LEN;

		}while(i < length);
	}
	else
	{
		//// utf-8 [ucs-2. ISO 10646-1:1993]
		char* pNewStr = new char[length + 1];
		memset(pNewStr, 0, len + 1);
		ReSortText(pNewStr, pStr, length);

		int chNum = 0;

		//测算一共有多少个字符
		char* pUtf8Str = (char*)pStr;
		unsigned long i = 0;
		do 
		{
			unsigned long charcode = 0;
			int size = 0;

			GUI_UTF8_TO_UCS2(pUtf8Str, charcode, size);

			pUtf8Str += size;
			i += size;

			chNum += 1;
		} while (i < length);

		pFont = pFontBuf + FONT_INFO_BUF_LEN * (chNum - 1);
		pUtf8Str = (char*)pNewStr;
		i = 0;
		do
		{
			len = (bufLen - subLen);

			unsigned long charcode = 0;
			int size = 0;

			GUI_UTF8_TO_UCS2(pUtf8Str, charcode, size);

			pUtf8Str += size;
			i += size;

			if(!m_font.GetFont(pFont, len, charcode))
			{
				m_font.GetFont(pFont, len, FONT_ERR_CHAR_REPLACE);
			}

			subLen += FONT_INFO_BUF_LEN;
			pFont -= FONT_INFO_BUF_LEN;

		}while(i < length);

		delete [] pNewStr;
	}

	if (0 == subLen)
	{
		memset(pFontBuf, 0, bufLen);
	}

	bufLen = subLen;

	return (bufLen > 0) ? true : false;
}

bool GUI::CFBInterface::ReSortText(char* pText, const char* pSor, int len)
{
	bool ret = false;

	if(len <= 0)
	{
		return ret;
	}

	memcpy(pText, pSor, len);

	char* pSortBuff = new char[len + 1];

	int i = 0;
	char* utf8Ptr = const_cast<char*>(pSor);
	do
	{
		unsigned long charID = 0;
		int size = 0;

		int direct = 0;
		int tmpLen = 0;
		char* pTmp = utf8Ptr;

		unsigned long lastCharID = 0;
		int lastSize = 0;

		do 
		{
			GUI_UTF8_TO_UCS2(pTmp, charID, size);

			//右序语言区
			if((0x0590 <= charID) && (0x077F >= charID))
			{
				if(0 == direct)
				{
					direct = -1;
				}

				if(direct > 0)
				{
					if((0x30 > lastCharID)||
						((0x39 < lastCharID) && (0x41 > lastCharID)) ||
						((0x5A < lastCharID) && (0x61 > lastCharID)) ||
						((0x7A < lastCharID) && (0x80 > lastCharID)) )
					{
						tmpLen -= lastSize;
					}
					break;
				}
			}
			else if((0x30 > charID)||
					((0x39 < charID) && (0x41 > charID)) ||
					((0x5A < charID) && (0x61 > charID)) ||
					((0x7A < charID) && (0x80 > charID)) )
			{
				switch (charID)
				{
				case 0x28:
					pText[i + tmpLen] = 0x29;
					break;
				case 0x29:
					pText[i + tmpLen] = 0x28;
					break;
				case 0x3C:
					pText[i + tmpLen] = 0x3E;
					break;
				case 0x3E:
					pText[i + tmpLen] = 0x3C;
					break;
				case 0x5B:
					pText[i + tmpLen] = 0x5D;
					break;
				case 0x5D:
					pText[i + tmpLen] = 0x5B;
					break;
				case 0x7B:
					pText[i + tmpLen] = 0x7D;
					break;
				case 0x7D:
					pText[i + tmpLen] = 0x7B;
					break;
				default:
					break;
				}

				if(0 == direct)
				{
					direct = -1;
				}

				if(direct > 0)
				{
					memcpy(pSortBuff + len - tmpLen - size, pTmp, size);
				}

				if((direct > 0) && (i + tmpLen >= len - 1))
				{
					break;
				}
			}
			else
			{
				if(0 == direct)
				{
					direct = 1;
				}

				if(direct < 0)
				{
					break;
				}
				memcpy(pSortBuff + len - tmpLen - size, pTmp, size);
			}

			pTmp += size;
			tmpLen += size;

			lastCharID = charID;
			lastSize = size;

		} while (i + tmpLen < len);

		if((direct > 0) && (tmpLen > 0))
		{

			memcpy(pText + i, pSortBuff + len - tmpLen, tmpLen);
			ret = true;
		}

		utf8Ptr += tmpLen;
		i += tmpLen;
	}while(i < len);

	if(NULL != pSortBuff)
	{
		delete [] pSortBuff;
	}

	return ret;
}


void GUI::CFBInterface::SetClickPos(unsigned long x,unsigned long y)
{
	m_pos_x = x;
	m_pos_y = y;
}
void GUI::CFBInterface::SendInitChnn(unsigned long chnn)
{
	m_initchnn = chnn;
}


//////////////////////private////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
