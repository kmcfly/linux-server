#ifndef _ONVIF_ALARM_H_
#define _ONVIF_ALARM_H_

#include "DDPublic.h"
#include "soapServ.h"
#include "eventH.h"

enum EOnvifEventType
{
	TONVIF_EVENT_MOTION=0,
	TONVIF_EVENT_SERSOR,
	TONVIF_EVENT_VIDEOLOSS,
	TONVIF_EVENT_SHELDER,
	TONVIF_EVENT_OTHER,
	TONVIF_EVENT_END
};

enum EOnvifEventAction
{
	TONVIF_BASE_ACTION=0,
	TONVIF_PULLPOINT_ACTION,
	TONVIF_END_ACTION
};

#endif