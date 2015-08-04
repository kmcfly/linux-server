/*
* Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
* Author       : xianshiwei
* Date         : 2015-05-27
* Name         : PtzAux.cpp
* Version      : 1.0
* Description  : 

* Modify Record:

*/
#include"PtzAux.h"
#include"PTZCoaxialCreate.h"
#include "dvr_func.h"


CPtzAux * CPtzAux::m_pPtzAux = 0;

CPtzAux::CPtzAux()
{
	for(unsigned int i=0;i<128;++i)
	{
		m_resoType[i]=0;
	}
}

CPtzAux * CPtzAux::Instance()
{
	if (0 == m_pPtzAux)
	{
		//lock
		if (0 == m_pPtzAux)
		{
			m_pPtzAux = new CPtzAux;
		}
		//lock
	}
	return m_pPtzAux;
}

long CPtzAux::GetResolutionType(int chnn) const
{
	if (chnn < 128)
	{
		return m_resoType[chnn];
	}
	return -1;
}

void CPtzAux::SetResolutionType(int chnn,long resoType)
{
	if ( chnn<128 )
	{
		m_resoType[chnn] = resoType;
		printf("SetResolutionType---------------m_resoType:%d\n",m_resoType[chnn]);
	}
	
	/*if (
		(VIDEO_TYPE_720P60 == m_resoType[chnn])||(VIDEO_TYPE_720P50 == m_resoType[chnn])||\
		(VIDEO_TYPE_1080P30 == m_resoType[chnn])||(VIDEO_TYPE_1080P25 == m_resoType[chnn])||\
		(VIDEO_TYPE_720P30 == m_resoType[chnn])||(VIDEO_TYPE_720P25 == m_resoType[chnn])||\
		(VIDEO_TYPE_960H30 == m_resoType[chnn])||(VIDEO_TYPE_960H25 == m_resoType[chnn])||\
		(VIDEO_TYPE_HDCIF30 == m_resoType[chnn])||(VIDEO_TYPE_HDCIF25 == m_resoType[chnn]) 
		)
	{
		m_stutas = TVI;
	}
	if ( (VIDEO_TYPE_1080P30_AHD == m_resoType[chnn])||(VIDEO_TYPE_1080P25_AHD == m_resoType[chnn]) )
	{
		m_stutas = AHD_1080;
	}
	if (
		(VIDEO_TYPE_720P60_AHD == m_resoType[chnn])||(VIDEO_TYPE_720P50_AHD == m_resoType[chnn])||\
		(VIDEO_TYPE_720P30_AHD == m_resoType[chnn])||(VIDEO_TYPE_720P25_AHD == m_resoType[chnn])
		)
	{
		m_stutas = AHD_720;
	}
	if ((VIDEO_TYPE_960H30_AHD == m_resoType[chnn])||(VIDEO_TYPE_960H25_AHD == m_resoType[chnn]))
	{
		m_stutas = AHD_960;
	}*/
}

CPTZProtocol * CPtzAux::GetPtzPoint(int chnn)
{
	CPTZCoaxialCreate * pCreate = NULL;
	CPTZProtocol * pPro = NULL;

	if (
		(VIDEO_TYPE_720P60 == m_resoType[chnn])||(VIDEO_TYPE_720P50 == m_resoType[chnn])||\
		(VIDEO_TYPE_1080P30 == m_resoType[chnn])||(VIDEO_TYPE_1080P25 == m_resoType[chnn])||\
		(VIDEO_TYPE_720P30 == m_resoType[chnn])||(VIDEO_TYPE_720P25 == m_resoType[chnn])||\
		(VIDEO_TYPE_960H30 == m_resoType[chnn])||(VIDEO_TYPE_960H25 == m_resoType[chnn])||\
		(VIDEO_TYPE_HDCIF30 == m_resoType[chnn])||(VIDEO_TYPE_HDCIF25 == m_resoType[chnn]) 
		)
	{
		std::map<STUTAS_COAXIAL,CPTZProtocol *>::iterator iter = m_PtzPoint.find(TVI);
		if ( (m_PtzPoint.end() != iter)&&( TVI ==static_cast<STUTAS_COAXIAL>(iter->first)) )
		{
				pPro = iter->second;
				((CPTZTvi *)pPro)->SetCurChnn(chnn);
				printf("get tvi object!____________________________\n");
		}else{
			pCreate = new CPTZCoaxialCreateTVI;
			pPro = pCreate->GetCoaxialProtocol(chnn);
			m_PtzPoint.insert(std::make_pair(TVI,pPro));
			printf("create tvi object!____________________________\n");
		}
	}

	if ( (VIDEO_TYPE_1080P30_AHD == m_resoType[chnn])||(VIDEO_TYPE_1080P25_AHD == m_resoType[chnn]) )
	{
		std::map<STUTAS_COAXIAL,CPTZProtocol *>::iterator iter = m_PtzPoint.find(AHD_1080);
		if ( (m_PtzPoint.end() != iter)&&( AHD_1080 ==static_cast<STUTAS_COAXIAL>(iter->first)) )
		{
			pPro = iter->second;
			((CPtzAhd1080 *)pPro)->SetCurChnn(chnn);
			printf("get 1080 object!____________________________\n");
		}else{
			pCreate = new CPTZCoaxialCreateAHD1080;
			pPro =pCreate->GetCoaxialProtocol(chnn);
			m_PtzPoint.insert(std::make_pair(AHD_1080,pPro));
			printf("create 1080 object!____________________________\n");
		}
	}

	if (
		(VIDEO_TYPE_720P60_AHD == m_resoType[chnn])||(VIDEO_TYPE_720P50_AHD == m_resoType[chnn])||\
		(VIDEO_TYPE_720P30_AHD == m_resoType[chnn])||(VIDEO_TYPE_720P25_AHD == m_resoType[chnn])
		)
	{
		std::map<STUTAS_COAXIAL,CPTZProtocol *>::iterator iter = m_PtzPoint.find(AHD_720);
		if ( (m_PtzPoint.end() != iter)&&( AHD_720 ==static_cast<STUTAS_COAXIAL>(iter->first)) )
		{
			pPro = iter->second;
			((CPtzAhd720 *)pPro)->SetCurChnn(chnn);
			printf("get 720 object!____________________________\n");
		}else{
			pCreate = new CPTZCoaxialCreateAHD720;
			pPro = pCreate->GetCoaxialProtocol(chnn);
			m_PtzPoint.insert(std::make_pair(AHD_720,pPro));
			printf("create 720 object!____________________________\n");
		}
	}

	if ( (VIDEO_TYPE_960H30_AHD == m_resoType[chnn])||(VIDEO_TYPE_960H25_AHD == m_resoType[chnn]) )
	{
		std::map<STUTAS_COAXIAL,CPTZProtocol *>::iterator iter = m_PtzPoint.find(AHD_960);
		if ( (m_PtzPoint.end() != iter)&&( AHD_960 ==static_cast<STUTAS_COAXIAL>(iter->first)) )
		{
			pPro = iter->second;
			((CPtzAhd960 *)pPro)->SetCurChnn(chnn);
			printf("get 960 object!____________________________\n");
		}else{
			pCreate = new CPTZCoaxialCreateAHD960;
			pPro = pCreate->GetCoaxialProtocol(chnn);
			m_PtzPoint.insert(std::make_pair(AHD_960,pPro));
			printf("create 960 object!____________________________\n");
		}
	}

	if ( (VIDEO_TYPE_HDCIF30_AHD == m_resoType[chnn])||(VIDEO_TYPE_HDCIF25_AHD == m_resoType[chnn]) )
	{
		 pCreate = NULL;
	}
	
	if (NULL != pCreate)
	{
		delete pCreate;
		//return NULL;
	}

	return pPro;
}
