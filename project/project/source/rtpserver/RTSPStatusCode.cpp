#include "RTSPStatusCode.h"
#include "RTSPHeader.h"

#ifdef _DEBUG
#define  new DEBUG_NEW
#endif


CRTSPStatusCode::CRTSPStatusCode()
{
	m_hasInitial = false;
	m_statusInfoList.clear();
}

CRTSPStatusCode::~CRTSPStatusCode()
{

}

CRTSPStatusCode *CRTSPStatusCode::Instance()
{
	static CRTSPStatusCode rtspStatusCode;
	return &rtspStatusCode;
}

bool CRTSPStatusCode::Initial()
{
	//if (m_hasInitial)
	//{
	//	assert(false);
	//	return false;
	//}

	m_statusInfoList.clear();

	STATUS_CODE_INFO statusCodeInfo;

	//100-Continue
	statusCodeInfo.statusCode	= STATUS_CODE_CONTINUE;
	statusCodeInfo.reason		= "Continue";
	m_statusInfoList.push_back(statusCodeInfo);
	
	//200-OK
	statusCodeInfo.statusCode	= STATUS_CODE_OK;
	statusCodeInfo.reason		= "OK";
	m_statusInfoList.push_back(statusCodeInfo);
	
	//201-Created
	statusCodeInfo.statusCode	= STATUS_CODE_CREATED;
	statusCodeInfo.reason		= "Created";
	m_statusInfoList.push_back(statusCodeInfo);

	//250-Low on Storage Space
	statusCodeInfo.statusCode	= STATUS_CODE_LOW_ON_STORAGE_SPACE;
	statusCodeInfo.reason		= "Low on Storage Space";
	m_statusInfoList.push_back(statusCodeInfo);

	//300-Multiple Choices
	statusCodeInfo.statusCode	= STATUS_CODE_MULTIPLE_CHOICES;
	statusCodeInfo.reason		= "Multiple Choices";
	m_statusInfoList.push_back(statusCodeInfo);
	
	//301-Moved Permanently
	statusCodeInfo.statusCode	= STATUS_CODE_MOVED_PERMANENTLY;
	statusCodeInfo.reason		= "Moved Permanently";
	m_statusInfoList.push_back(statusCodeInfo);

	//302-Moved Temporarily
	statusCodeInfo.statusCode	= STATUS_CODE_MOVED_TEMPORARILY;
	statusCodeInfo.reason		= "Moved Temporarily";
	m_statusInfoList.push_back(statusCodeInfo);

	//303-See Other
	statusCodeInfo.statusCode	= STATUS_CODE_SEE_OTHER;
	statusCodeInfo.reason		= "See Other";
	m_statusInfoList.push_back(statusCodeInfo);

	//304-Not Modified
	statusCodeInfo.statusCode	= STATUS_CODE_NOT_MODIFIED;
	statusCodeInfo.reason		= "Not Modified";
	m_statusInfoList.push_back(statusCodeInfo);

	//305-Use Proxy
	statusCodeInfo.statusCode	= STATUS_CODE_USE_PROXY;
	statusCodeInfo.reason		= "Use Proxy";
	m_statusInfoList.push_back(statusCodeInfo);

	//400-Bad Request
	statusCodeInfo.statusCode	= STATUS_CODE_BAD_REQUEST;
	statusCodeInfo.reason		= "Bad Request";
	m_statusInfoList.push_back(statusCodeInfo);

	//401-Unauthorized
	statusCodeInfo.statusCode	= STATUS_CODE_UNAUTHORIZED;
	statusCodeInfo.reason		= "Unauthorized";
	m_statusInfoList.push_back(statusCodeInfo);

	//402-Payment Required
	statusCodeInfo.statusCode	= STATUS_CODE_PAYMENT_REQUIRED;
	statusCodeInfo.reason		= "Payment Required";
	m_statusInfoList.push_back(statusCodeInfo);

	//403-Forbidden
	statusCodeInfo.statusCode	= STATUS_CODE_USE_PROXY;
	statusCodeInfo.reason		= "Forbidden";
	m_statusInfoList.push_back(statusCodeInfo);

	//404-Not Found
	statusCodeInfo.statusCode	= STATUS_CODE_NOT_FOUND;
	statusCodeInfo.reason		= "Use Proxy";
	m_statusInfoList.push_back(statusCodeInfo);

	//405-Method Not Allowed
	statusCodeInfo.statusCode	= STATUS_CODE_METHOD_NOT_ALLOWED;
	statusCodeInfo.reason		= "Method Not Allowed";
	m_statusInfoList.push_back(statusCodeInfo);

	//406-Not Acceptable
	statusCodeInfo.statusCode	= STATUS_CODE_NOT_ACCEPTABLE;
	statusCodeInfo.reason		= "Not Acceptable";
	m_statusInfoList.push_back(statusCodeInfo);

	//407-Proxy Authentication Required
	statusCodeInfo.statusCode	= STATUS_CODE_PROXY_AUTHENTICATION_REQUIRED;
	statusCodeInfo.reason		= "Proxy Authentication Required";
	m_statusInfoList.push_back(statusCodeInfo);

	//408-Request Time-out
	statusCodeInfo.statusCode	= STATUS_CODE_REQUEST_TIME_OUT;
	statusCodeInfo.reason		= "Request Time-out";
	m_statusInfoList.push_back(statusCodeInfo);

	//410-Gone
	statusCodeInfo.statusCode	= STATUS_CODE_GONE;
	statusCodeInfo.reason		= "Gone";
	m_statusInfoList.push_back(statusCodeInfo);

	//411-Length Required
	statusCodeInfo.statusCode	= STATUS_CODE_LENGTH_REQUIRED;
	statusCodeInfo.reason		= "Length Required";
	m_statusInfoList.push_back(statusCodeInfo);

	//412-Precondition Failed
	statusCodeInfo.statusCode	= STATUS_CODE_PRECONDITION_FAILED;
	statusCodeInfo.reason		= "Precondition Failed";
	m_statusInfoList.push_back(statusCodeInfo);

	//413-Request Entity Too Large
	statusCodeInfo.statusCode	= STATUS_CODE_REQUEST_ENTITY_TOO_LARGE;
	statusCodeInfo.reason		= "Request Entity Too Large";
	m_statusInfoList.push_back(statusCodeInfo);

	//414-Request-URI Too Large
	statusCodeInfo.statusCode	= STATUS_CODE_REQUEST_URI_TOO_LARGE;
	statusCodeInfo.reason		= "Request-URI Too Large";
	m_statusInfoList.push_back(statusCodeInfo);

	//415-Unsupported Media Type
	statusCodeInfo.statusCode	= STATUS_CODE_UNSUPPORTED_MEDIA_TYPE;
	statusCodeInfo.reason		= "Unsupported Media Type";
	m_statusInfoList.push_back(statusCodeInfo);

	//451-Parameter Not Understood
	statusCodeInfo.statusCode	= STATUS_CODE_PARAMETER_NOT_UNDERSTOOD;
	statusCodeInfo.reason		= "Parameter Not Understood";
	m_statusInfoList.push_back(statusCodeInfo);

	//452-Conference Not Found
	statusCodeInfo.statusCode	= STATUS_CODE_CONFERENCE_NOT_FOUND;
	statusCodeInfo.reason		= "Conference Not Found";
	m_statusInfoList.push_back(statusCodeInfo);

	//453-Not Enough Bandwidth
	statusCodeInfo.statusCode	= STATUS_CODE_NOT_ENOUGH_BANDWIDTH;
	statusCodeInfo.reason		= "Not Enough Bandwidth";
	m_statusInfoList.push_back(statusCodeInfo);

	//454-Session Not Found
	statusCodeInfo.statusCode	= STATUS_CODE_SESSION_NOT_FOUND;
	statusCodeInfo.reason		= "Session Not Found";
	m_statusInfoList.push_back(statusCodeInfo);

	//455-Method Not Valid in This State
	statusCodeInfo.statusCode	= STATUS_CODE_METHOD_NOT_VALID_IN_THIS_STATE;
	statusCodeInfo.reason		= "Method Not Valid in This State";
	m_statusInfoList.push_back(statusCodeInfo);

	//456-Header Field Not Valid for Resource
	statusCodeInfo.statusCode	= STATUS_CODE_HEADER_FIELD_NOT_VALID_FOR_RESOURCE;
	statusCodeInfo.reason		= "Header Field Not Valid for Resource";
	m_statusInfoList.push_back(statusCodeInfo);

	//457-Invalid Range
	statusCodeInfo.statusCode	= STATUS_CODE_INVALID_RANGE;
	statusCodeInfo.reason		= "Invalid Range";
	m_statusInfoList.push_back(statusCodeInfo);

	//458-Parameter Is Read-Only
	statusCodeInfo.statusCode	= STATUS_CODE_PARAMETER_IS_READ_ONLY;
	statusCodeInfo.reason		= "Parameter Is Read-Only";
	m_statusInfoList.push_back(statusCodeInfo);

	//459-Aggregate operation not allowed
	statusCodeInfo.statusCode	= STATUS_CODE_AGGREGATE_OPERATION_NOT_ALLOWED;
	statusCodeInfo.reason		= "Aggregate operation not allowed";
	m_statusInfoList.push_back(statusCodeInfo);

	//460-Only aggregate operation allowed
	statusCodeInfo.statusCode	= STATUS_CODE_ONLY_AGGREGATE_OPERATION_ALLOWED;
	statusCodeInfo.reason		= "Only aggregate operation allowed";
	m_statusInfoList.push_back(statusCodeInfo);

	//461-Unsupported transport
	statusCodeInfo.statusCode	= STATUS_CODE_UNSUPPORTED_TRANSPORT;
	statusCodeInfo.reason		= "Unsupported transport";
	m_statusInfoList.push_back(statusCodeInfo);

	//462-Destination unreachable
	statusCodeInfo.statusCode	= STATUS_CODE_DESTINATION_UNREACHABLE;
	statusCodeInfo.reason		= "Destination unreachable";
	m_statusInfoList.push_back(statusCodeInfo);

	//500-Internal Server Error
	statusCodeInfo.statusCode	= STATUS_CODE_INTERNAL_SERVER_ERROR;
	statusCodeInfo.reason		= "Internal Server Error";
	m_statusInfoList.push_back(statusCodeInfo);

	//501-Not Implemented
	statusCodeInfo.statusCode	= STATUS_CODE_NOT_IMPLEMENTED;
	statusCodeInfo.reason		= "Not Implemented";
	m_statusInfoList.push_back(statusCodeInfo);

	//502-Bad Gateway
	statusCodeInfo.statusCode	= STATUS_CODE_BAD_GATEWAY;
	statusCodeInfo.reason		= "Bad Gateway";
	m_statusInfoList.push_back(statusCodeInfo);

	//503-Service Unavailable
	statusCodeInfo.statusCode	= STATUS_CODE_SERVICE_UNAVAILABLE;
	statusCodeInfo.reason		= "Service Unavailable";
	m_statusInfoList.push_back(statusCodeInfo);

	//504-Gateway Time-out
	statusCodeInfo.statusCode	= STATUS_CODE_GATEWAY_TIME_OUT;
	statusCodeInfo.reason		= "Gateway Time-out";
	m_statusInfoList.push_back(statusCodeInfo);

	//505-RTSP Version not supported
	statusCodeInfo.statusCode	= STATUS_CODE_RTSP_VERSION_NOT_SUPPORTED;
	statusCodeInfo.reason		= "RTSP Version not supported";
	m_statusInfoList.push_back(statusCodeInfo);

	//551-Option not supported
	statusCodeInfo.statusCode	= STATUS_CODE_OPTION_NOT_SUPPORTED;
	statusCodeInfo.reason		= "Option not supported";
	m_statusInfoList.push_back(statusCodeInfo);

	m_hasInitial = true;

	return true;
}

void CRTSPStatusCode::Quit()
{
	m_hasInitial = false;
	m_statusInfoList.clear();
}

const char *CRTSPStatusCode::GetStatusCodeReason(int statusCode)
{
	if (!m_hasInitial)
	{
		assert(false);
		return NULL;
	}

	for (list<STATUS_CODE_INFO>::iterator iter = m_statusInfoList.begin(); iter != m_statusInfoList.end(); iter++)
	{
		if (statusCode == iter->statusCode)
		{
			return iter->reason.c_str();
		}
	}

	assert(false);

	return NULL;
}

