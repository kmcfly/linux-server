/*
* Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
* Author       : xianshiwei
* Date         : 2015-05-27
* Name         : PTZCoaxialCreate.h
* Version      : 1.0
* Description  : 

* Modify Record:

*/
#ifndef __PTZ_COAXIAL_CREATE__
#define __PTZ_COAXIAL_CREATE__

#include"PTZProtocol.h"
#include"PTZTvi.h"
#include "PTZCoaxial.h"
#include "PtzAhd.h"
#include "PtzAhd_1080.h"
#include "PtzAhd_720.h"
#include "PtzAhd_960.h"


class CPTZCoaxialCreate{
public:
	CPTZCoaxialCreate();
	virtual ~CPTZCoaxialCreate();
	
	virtual CPTZProtocol * GetCoaxialProtocol(int chnn, long resolution=0)=0;	
};

class CPTZCoaxialCreateTVI
	:public CPTZCoaxialCreate{

public:
	CPTZCoaxialCreateTVI();
	virtual ~CPTZCoaxialCreateTVI();

	CPTZProtocol * GetCoaxialProtocol(int chnn, long resolution=0);
};

class CPTZCoaxialCreateAHD
	:public CPTZCoaxialCreate{

public:
	CPTZCoaxialCreateAHD();
	virtual ~CPTZCoaxialCreateAHD();	

	virtual CPTZProtocol * GetCoaxialProtocol(int chnn, long resolution=0);
};

class CPTZCoaxialCreateAHD1080
	:public CPTZCoaxialCreateAHD{

public:
	CPTZCoaxialCreateAHD1080();
	virtual ~CPTZCoaxialCreateAHD1080();	

	CPTZProtocol * GetCoaxialProtocol(int chnn, long resolution=0);
};

class CPTZCoaxialCreateAHD720
	:public CPTZCoaxialCreateAHD{

public:
	CPTZCoaxialCreateAHD720();
	virtual ~CPTZCoaxialCreateAHD720();	

	 CPTZProtocol * GetCoaxialProtocol(int chnn, long resolution=0);
};

class CPTZCoaxialCreateAHD960
	:public CPTZCoaxialCreateAHD{

public:
	CPTZCoaxialCreateAHD960();
	virtual ~CPTZCoaxialCreateAHD960();	

	CPTZProtocol * GetCoaxialProtocol(int chnn, long resolution=0);
};

#endif
