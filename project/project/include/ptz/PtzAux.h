/*
* Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
* Author       : xianshiwei
* Date         : 2015-05-27
* Name         : PtzAux.h
* Version      : 1.0
* Description  : 

* Modify Record:

*/
#ifndef __PTZ_AUX___
#define __PTZ_AUX___

#include <map>
#ifdef __ENVIRONMENT_LINUX__
#include "PTZProtocol.h"
#endif


class CPtzAux
{
	typedef enum __video_stutas_coaxial_
	{
		EMPTY =0,
		TVI,		//tvi ״̬
		AHD_1080,
		AHD_720,
		AHD_960,
	}STUTAS_COAXIAL;
public:
	static CPtzAux * Instance();
protected:
	CPtzAux();
	~CPtzAux();

public:
	long GetResolutionType(int chnn) const;
	void SetResolutionType(int chnn,long resoType);

	CPTZProtocol* GetPtzPoint(int chnn);
private:
	long m_resoType[128];
	static CPtzAux* m_pPtzAux;
	std::map<STUTAS_COAXIAL,CPTZProtocol *> m_PtzPoint; 
};
#endif