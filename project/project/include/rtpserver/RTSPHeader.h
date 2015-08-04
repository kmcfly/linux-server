////////////////////////////////////////////////////////////////////////////////////

/*
*
*相关定义参考rfc2326
*
*/

#ifndef _RTSP_HEADER_H_
#define _RTSP_HEADER_H_

/*
*RTSP连接相关的原则
*1:RTSP的传输方式需要支持三中连接
*	(1):用TCP一次连接，传输多个请求-应答消息
*	(2):每次TCP连接只传输一次请求-应答消息
*	(3):使用UDP方式，不进行连接
*2:RTSP要求服务器能向客户端发送请求
*3:对于同一个连接，请求和应答应当是有序对应的。如果不是广播请求，接收者必须应答收到的请求。
*	在一个RTT后如果没收到应答，就需要从新发送请求。如果使用TCP，就不需要重传请求，RTSP必须使用底层的传输来保证可靠性。
*	如果要重传，要使用相同的CSeq号
*4:RTSP默认使用554端口
*/

#include <string.h>

#if defined(WIN32)
#define strncasecmp _strnicmp
#endif

const unsigned short RTSP_PORT		= 554;
const unsigned short RTP_UDP_START_PORT		= 50000;

//rtsp的版本
const char RTSP_VERSION[]			= "RTSP/1.0";
const char LINE_TAIL_LABEL[]		= "\r\n";
const char HEADER_TAIL_LABEL[]		= "\r\n\r\n";

typedef enum _rtsp_session_status_
{
	RTSP_SESSION_START		= 0,
	RTSP_SESSION_SETUP		= 1,
	RTSP_SESSION_PLAY		= 2,
	RTSP_SESSION_PAUSE		= 3,
	RTSP_SESSION_TEARDOWN	= 4,
	RTSP_SESSION_END		= 5,
}RTSP_SESSION_STATUS;

//支持的方法
typedef enum _rtsp_method_type_
{
	RTSP_METHOD_STOP			= 0,
	RTSP_METHOD_OPTIONS			= 1,
	RTSP_METHOD_DESCRIBE		= 2,
	RTSP_METHOD_SETUP			= 3,
	RTSP_METHOD_PLAY			= 4,
	RTSP_METHOD_TEARDOWN		= 5,
	RTSP_METHOD_GETPARAMETER	= 6,
	RTSP_METHOD_END				= 7,
}RTSP_METHOD_TYPE;

const char RTSP_METHODS[][20] = 
{
	"",
	"OPTIONS",
	"DESCRIBE",
	"SETUP",
	"PLAY",
	"TEARDOWN",
	"GET_PARAMETER"
	"",
};

const char RTSP_CSEQ_LABELS[2][20] = 
{
	"CSeq",
	"",
};

const char RTSP_NETHODS[] = "Public: OPTIONS,DESCRIBE,SETUP,TEARDOWN,PALY,PAUSE,SCALE";


//RTSP应答

/*
* RTSP应答中的状态码有五类，和HTTP的类似
* 1xx: Informational - Request received, continuing process
* 2xx: Success - The action was successfully received, understood,and accepted
* 3xx: Redirection - Further action must be taken in order to complete the request
* 4xx: Client Error - The request contains bad syntax or cannot be fulfilled
* 5xx: Server Error - The server failed to fulfill an apparently valid request
*
*
*													状态			描述--使用在那些命令中			
*/

typedef enum _status_code_
{
	STATUS_CODE_CONTINUE							= 100,			//Continue--all

	STATUS_CODE_OK									= 200,			//OK--all
	STATUS_CODE_CREATED								= 201,			//Created--RECORD
	STATUS_CODE_LOW_ON_STORAGE_SPACE				= 250,			//Low on Storage Space--RECORD
	
	STATUS_CODE_MULTIPLE_CHOICES					= 300,			//Multiple Choices--all		
	STATUS_CODE_MOVED_PERMANENTLY					= 301,			//Moved Permanently--all
	STATUS_CODE_MOVED_TEMPORARILY					= 302,			//Moved Temporarily--all
	STATUS_CODE_SEE_OTHER							= 303,			//See Other--all
	STATUS_CODE_NOT_MODIFIED						= 304,			//Not Modified--all
	STATUS_CODE_USE_PROXY							= 305,			//Use Proxy--all
	STATUS_CODE_BAD_REQUEST							= 400,			//Bad Request--all
	STATUS_CODE_UNAUTHORIZED						= 401,			//Unauthorized--all
	STATUS_CODE_PAYMENT_REQUIRED					= 402,			//Payment Required--all
	STATUS_CODE_FORBIDDEN							= 403,			//Forbidden--all
	STATUS_CODE_NOT_FOUND							= 404,			//Not Found--all
	STATUS_CODE_METHOD_NOT_ALLOWED					= 405,			//Method Not Allowed--all
	STATUS_CODE_NOT_ACCEPTABLE						= 406,			//Not Acceptable--all
	STATUS_CODE_PROXY_AUTHENTICATION_REQUIRED		= 407,			//Proxy Authentication Required--all
	STATUS_CODE_REQUEST_TIME_OUT					= 408,			//Request Time-out--all
	STATUS_CODE_GONE								= 410,			//Gone--all
	STATUS_CODE_LENGTH_REQUIRED						= 411,			//Length Required--all
	STATUS_CODE_PRECONDITION_FAILED					= 412,			//Precondition Failed--DESCRIBE, SETUP
	STATUS_CODE_REQUEST_ENTITY_TOO_LARGE			= 413,			//Request Entity Too Large--all
	STATUS_CODE_REQUEST_URI_TOO_LARGE				= 414,			//Request-URI Too Large--all
	STATUS_CODE_UNSUPPORTED_MEDIA_TYPE				= 415,			//Unsupported Media Type--all
	STATUS_CODE_PARAMETER_NOT_UNDERSTOOD			= 451,			//Parameter Not Understood--SETUP
	STATUS_CODE_CONFERENCE_NOT_FOUND				= 452,			//Conference Not Found--SETUP
	STATUS_CODE_NOT_ENOUGH_BANDWIDTH				= 453,			//Not Enough Bandwidth--SETUP
	STATUS_CODE_SESSION_NOT_FOUND					= 454,			//Session Not Found--all
	STATUS_CODE_METHOD_NOT_VALID_IN_THIS_STATE		= 455,			//Method Not Valid in This State--all
	STATUS_CODE_HEADER_FIELD_NOT_VALID_FOR_RESOURCE = 456,			//Header Field Not Valid for Resource--all
	STATUS_CODE_INVALID_RANGE						= 457,			//Invalid Range--PALY
	STATUS_CODE_PARAMETER_IS_READ_ONLY				= 458,			//Parameter Is Read-Only--SET-PARAMETER
	STATUS_CODE_AGGREGATE_OPERATION_NOT_ALLOWED		= 459,			//Aggregate operation not allowed -- all
	STATUS_CODE_ONLY_AGGREGATE_OPERATION_ALLOWED	= 460,			//Only aggregate operation allowed --all
	STATUS_CODE_UNSUPPORTED_TRANSPORT				= 461,			//Unsupported transport --all
	STATUS_CODE_DESTINATION_UNREACHABLE				= 462,			//Destination unreachable --all

	STATUS_CODE_INTERNAL_SERVER_ERROR				= 500,			//Internal Server Error --all
	STATUS_CODE_NOT_IMPLEMENTED						= 501,			//Not Implemented --all
	STATUS_CODE_BAD_GATEWAY							= 502,			//Bad Gateway --all
	STATUS_CODE_SERVICE_UNAVAILABLE					= 503,			//Service Unavailable --all
	STATUS_CODE_GATEWAY_TIME_OUT					= 504,			//Gateway Time-out --all
	STATUS_CODE_RTSP_VERSION_NOT_SUPPORTED			= 505,			//RTSP Version not supported --all
	STATUS_CODE_OPTION_NOT_SUPPORTED				= 551,			//Option not supported	--all
}STATUS_CODE;

const char INTERLEAVED_DOLLAR_SIGN	= 0x24;	
typedef struct _interleaved_info_
{
	char mark;					//用来标记RTSP中携带RTCP数据包
	char channel;				//RTCP的类型，对同一个会话中，如果有音视频，则0和1代表音频RTP数据和音频RTCP数据，2和3代表视频RTP数据和视频RTCP数据信息
								//如果只有音频，则0和1代表音频RTP数据和音频RTCP数据
								//如果只有视频，则0和1代表视频RTP数据和视频RTCP数据
	unsigned short datalength;
}INTERLEAVED_INFO;


/*
下表概述了RTSP中的所有头区域
TYPE:
	g表示可以用在请求或者应答中的一般请求头
	R表示只能用在求头中
	r表示只能用在应答头中
	e表示只能用在实体头中
SUPPORT:
	req.:接收者对有改标记的头必须做出相应的处理。但是每个请求不一定要包含该头的区域
	opt.:接收者可以不关心这个头提供的信息
METHODS:
	entity	: 该头区域只能用于包含实体的消息中
	all		: 该头区域可用在所有方法中
	SETUP	: 该头区域只能用在SETUP方法中
	……
	……
	……
Header				type		support			methods
Accept				R			opt.			entity
Accept-Encoding		R			opt.			entity
Accept-Language		R			opt.			all
Allow				r			opt.			all
Authorization		R			opt.			all
Bandwidth			R			opt.			all
Blocksize			R			opt.			all but OPTIONS, TEARDOWN
Cache-Control		g			opt.			SETUP
Conference			R			opt.			SETUP
Connection			g			req.			all
Content-Base		e			opt.			entity
Content-Encoding	e			req.			SET_PARAMETER
Content-Encoding	e			req.			DESCRIBE, ANNOUNCE
Content-Language	e			req.			DESCRIBE, ANNOUNCE
Content-Length		e			req.			SET_PARAMETER, ANNOUNCE
Content-Length		e			req.			entity
Content-Location	e			opt.			entity
Content-Type		e			req.			SET_PARAMETER, ANNOUNCE
Content-Type		r			req.			entity
CSeq				g			req.			all
Date				g			opt.			all
Expires				e			opt.			DESCRIBE, ANNOUNCE
From				R			opt.			all
If-Modified-Since	R			opt.			DESCRIBE, SETUP
Last-Modified		e			opt.			entity
Proxy-Authenticate
Proxy-Require		R			req.			all
Public				r			opt.			all
Range				R			opt.			PLAY, PAUSE, RECORD
Range				r			opt.			PLAY, PAUSE, RECORD
Referer				R			opt.			all
Require				R			req.			all
Retry-After			r			opt.			all
RTP-Info			r			req.			PLAY
Scale				Rr			opt.			PLAY, RECORD
Session				Rr			req.			all but SETUP, OPTIONS
Server				r			opt.			all
Speed				Rr			opt.			PLAY
Transport			Rr			req.			SETUP
Unsupported			r			req.			all
User-Agent			R			opt.			all
Via					g			opt.			all
WWW-Authenticate	r			opt.			all
*/

typedef enum _header_fild_type_
{
	HEADER_FILD_ACCEPT				= 0,
	HEADER_FILD_ACCEPT_ENCODING		= 1,
	HEADER_FILD_ACCEPT_LANGUAGE		= 2,
	HEADER_FILD_ALLOW				= 3,
	HEADER_FILD_AUTHORIZATION		= 4,
	HEADER_FILD_BANDWIDTH			= 5,
	HEADER_FILD_BLOCKSIZE			= 6,
	HEADER_FILD_CACHE_CONTROL		= 7,
	HEADER_FILD_CONFERENCE			= 8,
	HEADER_FILD_CONNECTION			= 9,
	HEADER_FILD_CONTENT_BASE		= 10,
	HEADER_FILD_CONTENT_ENCONDING	= 11,
	HEADER_FILD_CONTENT_LANGUAGE	= 12,
	HEADER_FILD_CONTENT_LENGTH		= 13,
	HEADER_FILD_CONTENT_LOCATION	= 14,
	HEADER_FILD_CONTENT_TYPE		= 15,
	HEADER_FILD_CSEQ				= 16,
	HEADER_FILD_DATE				= 17,
	HEADER_FILD_EXPIRES				= 18,
	HEADER_FILD_FROM				= 19,
	HEADER_FILD_IF_MODIFIED_SINCE	= 20,
	HEADER_FILD_LAST_MODIFIED		= 21,
	HEADER_FILD_PROXY_AUTHENTICATE	= 22,
	HEADER_FILD_PROXY_REQUIRE		= 23,
	HEADER_FILD_PUBLIC				= 24,
	HEADER_FILD_RANGE				= 25,
	HEADER_FILD_REFERER				= 26,
	HEADER_FILD_REQUIRE				= 27,
	HEADER_FILD_RETRY_AFTER			= 28,
	HEADER_FILD_RTP_INFO			= 29,
	HEADER_FILD_SCALE				= 30,
	HEADER_FILD_SESSION				= 31,
	HEADER_FILD_SERVER				= 32,
	HEADER_FILD_SPEED				= 33,
	HEADER_FILD_TRANSPORT			= 34,
	HEADER_FILD_UNSUPPORTED			= 35,
	HEADER_FILD_USER_AGENT			= 36,
	HEADER_FILD_VIA					= 37,
	HEADER_FILD_WWW_AUTHENTICATE	= 38,
	HEADER_FILD_END					= 39,
}HEADER_FILD_TYPE;

const char RTSP_HEADER_FILD[40][20] = 
{
	"Accept",
	"Accept-Encoding",
	"Accept-Language",
	"Allow",
	"Authorization",
	"Bandwidth",
	"Blocksize",
	"Cache-Control",
	"Conference",
	"Connection",
	"Content-Base",
	"Content-Encoding",
	"Content-Language",
	"Content-Length",
	"Content-Location",
	"Content-Type",
	"CSeq",
	"Date",
	"Expires",
	"From",
	"If-Modified-Since",
	"Last-Modified",
	"Proxy-Authenticate",
	"Proxy-Require",
	"Public",
	"Range",
	"Referer",
	"Require",
	"Retry-After",
	"RTP-Info",
	"Scale",
	"Session",
	"Server",
	"Speed",
	"Transport",
	"Unsupported",
	"User-Agent",
	"Via",
	"WWW-Authenticate",
	"",
};

const bool METHOD_MUST_HAS_HEADER_FILDS[13][40] =
{
//	Accept				Accept-Encoding		Accept-Language		Allow			Authorization		Bandwidth
//	Blocksize			Cache-Control		Conference			Connection		Content-Base		Content-Encoding
//	Content-Language	Content-Length		Content-Location	Content-Type	CSeq				Date
//	Expires				From				If-Modified-Since	Last-Modified	Proxy-Authenticate	Proxy-Require
//	Public				Range				Referer				Require			Retry-After			RTP-Info
//	Scale				Session				Server				Speed			Transport			Unsupported
//	User-Agent			Via					WWW-Authenticate
	
	//DESCRIBE
	{
		true, false, false, false, false, false,
		false, false, false, false, false, false,
		false, false, false, false, false, false,
		false, false, false, false, false, false,
		false, false, false, false, false, false,
		false, false, false, false, false, false,
		false, false, false
	},

	//ANNOUNCE
	{
		false, false, false, false, false, false,
		false, false, false, false, false, false,
		false, false, false, false, false, false,
		false, false, false, false, false, false,
		false, false, false, false, false, false,
		false, false, false, false, false, false,
		false, false, false
	},

	//GET_PARAMETER
	{
		false, false, false, false, false, false,
		false, false, false, false, false, false,
		false, false, false, false, false, false,
		false, false, false, false, false, false,
		false, false, false, false, false, false,
		false, false, false, false, false, false,
		false, false, false
	},

	//OPTIONS
	{
		false, false, false, false, false, false,
		false, false, false, false, false, false,
		false, false, false, false, false, false,
		false, false, false, false, false, false,
		false, false, false, false, false, false,
		false, false, false, false, false, false,
		false, false, false
	},

	//PAUSE
	{
		false, false, false, false, false, false,
		false, false, false, false, false, false,
		false, false, false, false, false, false,
		false, false, false, false, false, false,
		false, false, false, false, false, false,
		false, false, false, false, false, false,
		false, false, false
	},

	//PLAY
	{
		false, false, false, false, false, false,
		false, false, false, false, false, false,
		false, false, false, false, false, false,
		false, false, false, false, false, false,
		false, false, false, false, false, false,
		false, false, false, false, false, false,
		false, false, false
	},

	//RECORD
	{
		false, false, false, false, false, false,
		false, false, false, false, false, false,
		false, false, false, false, false, false,
		false, false, false, false, false, false,
		false, false, false, false, false, false,
		false, false, false, false, false, false,
		false, false, false
	},

	//REDIRECT
	{
		false, false, false, false, false, false,
		false, false, false, false, false, false,
		false, false, false, false, false, false,
		false, false, false, false, false, false,
		false, false, false, false, false, false,
		false, false, false, false, false, false,
		false, false, false
	},

	//SETUP
	{
		false, false, false, false, false, false,
		false, false, false, false, false, false,
		false, false, false, false, false, false,
		false, false, false, false, false, false,
		false, false, false, false, false, false,
		false, false, false, false, false, false,
		false, false, false
	},

	//SET_PARAMETER
	{
		false, false, false, false, false, false,
		false, false, false, false, false, false,
		false, false, false, false, false, false,
		false, false, false, false, false, false,
		false, false, false, false, false, false,
		false, false, false, false, false, false,
		false, false, false
	},

	//TEARDOWN
	{
		false, false, false, false, false, false,
		false, false, false, false, false, false,
		false, false, false, false, false, false,
		false, false, false, false, false, false,
		false, false, false, false, false, false,
		false, false, false, false, false, false,
		false, false, false
	},

	//SCALE
	{
		false, false, false, false, false, false,
		false, false, false, false, false, false,
		false, false, false, false, false, false,
		false, false, false, false, false, false,
		false, false, false, false, false, false,
		false, false, false, false, false, false,
		false, false, false
	}
};


#endif /*_RTSP_HEADER_H_*/
////////////////////////////////////////////////////////////////////////////////////
