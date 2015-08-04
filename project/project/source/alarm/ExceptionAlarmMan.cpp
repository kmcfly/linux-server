/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : LH
** Date         : 2013-07-10
** Name         : ExcetionAlarm.cpp
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/

#include "ExceptionAlarmMan.h"
#include "LocalDevice.h"

CExceptionAlarmMan::CExceptionAlarmMan()
{
	m_prevChnnBits = 0;
}

CExceptionAlarmMan::~CExceptionAlarmMan()
{

}

void CExceptionAlarmMan::AlarmIn(ULONGLONG chnnBits)
{
	if(chnnBits != 0)
	{
		CLocalDevice::Instance()->SetBuzzerStatus(true);
	}
	else
	{
		if(m_prevChnnBits != 0)
		{
			CLocalDevice::Instance()->SetBuzzerStatus(false);
		}
	}

	m_prevChnnBits = chnnBits;
}
