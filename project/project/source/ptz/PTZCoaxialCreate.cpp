/*
* Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
* Author       : xianshiwei
* Date         : 2015-05-27
* Name         : PTZCoaxialCreate.cpp
* Version      : 1.0
* Description  : 

* Modify Record:

*/
#include"PTZCoaxialCreate.h"


CPTZCoaxialCreate::CPTZCoaxialCreate()
{
}

CPTZCoaxialCreate::~CPTZCoaxialCreate()
{
}



CPTZCoaxialCreateTVI::CPTZCoaxialCreateTVI()
{
}

CPTZCoaxialCreateTVI::~CPTZCoaxialCreateTVI()
{
}

CPTZProtocol * CPTZCoaxialCreateTVI::GetCoaxialProtocol(int chnn,long resolution)
{
	CPTZProtocol * pTvi=new CPTZTvi(chnn);
	return pTvi;
}

//CPTZProtocol * CPTZCoaxialCreateAHD1080::GetCoaxialProtocol(int chnn, long resolution)
//{
//	CPtzAhd * pAhd=NULL;
//	if ( (VIDEO_TYPE_1080P30_AHD == resolution)||(VIDEO_TYPE_1080P25_AHD == resolution) )
//	{
//		pAhd= new CPtzAhd1080;
//
//	}else if ( (VIDEO_TYPE_720P60_AHD == resolution)||(VIDEO_TYPE_720P50_AHD == resolution)||\
//		(VIDEO_TYPE_720P30_AHD == resolution)||(VIDEO_TYPE_720P25_AHD == resolution) )
//	{
//		pAhd = new CPtzAhd720;
//	} 
//	else if( (VIDEO_TYPE_960H30_AHD == resolution)||(VIDEO_TYPE_960H25_AHD == resolution) )
//	{
//		pAhd = new CPtzAhd960;
//
//	}else if ((VIDEO_TYPE_HDCIF30_AHD == resolution)||(VIDEO_TYPE_HDCIF25_AHD == resolution) )
//	{
//		pAhd = NULL;
//	}
//	
//	return pAhd;
//}

CPTZCoaxialCreateAHD::CPTZCoaxialCreateAHD()
{
}

CPTZCoaxialCreateAHD::~CPTZCoaxialCreateAHD()
{
}

CPTZProtocol * CPTZCoaxialCreateAHD::GetCoaxialProtocol(int chnn, long resolution)
{
	return NULL;
}

CPTZCoaxialCreateAHD1080::CPTZCoaxialCreateAHD1080()
{
}

CPTZCoaxialCreateAHD1080::~CPTZCoaxialCreateAHD1080()
{
}

CPTZProtocol * CPTZCoaxialCreateAHD1080::GetCoaxialProtocol(int chnn, long resolution)
{
	CPTZProtocol * pAhd=new CPtzAhd1080(chnn);
	return pAhd;
}


CPTZCoaxialCreateAHD720::CPTZCoaxialCreateAHD720()
{
}

CPTZCoaxialCreateAHD720::~CPTZCoaxialCreateAHD720()
{
}

CPTZProtocol * CPTZCoaxialCreateAHD720::GetCoaxialProtocol(int chnn, long resolution)
{
	CPTZProtocol * pAhd=new CPtzAhd720(chnn);
	return pAhd;
}


CPTZCoaxialCreateAHD960::CPTZCoaxialCreateAHD960()
{
}

CPTZCoaxialCreateAHD960::~CPTZCoaxialCreateAHD960()
{
}

CPTZProtocol * CPTZCoaxialCreateAHD960::GetCoaxialProtocol(int chnn, long resolution)
{
	CPTZProtocol * pAhd=new CPtzAhd960(chnn);
	return pAhd;
}