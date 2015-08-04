///////////////////////////////////////////////////////////////////////////

#ifndef __FTP_TYPE_H__
#define __FTP_TYPE_H__

const unsigned short FTP_SERVER_PORT	= 21;
const char		FTP_ANONYMOUS_NAME[]	= "anonymous";
const char		FTP_ANONYMOUS_PASS[]	= "";

const unsigned long MILLI_SECOND		= 1000;
const unsigned long SECOND				= MILLI_SECOND * MILLI_SECOND;
const unsigned long MSG_BUF_LEN			= 4096;
const char END_OF_LINE[]				= "\r\n";

typedef enum _ftp_status_code_
{
	FTP_STATUS_OPEN_DATA_PORT					=	150,//文件状态正常，开启数据连接端口
	FTP_STATUS_COMMAND_OK						=	200,//命令执行成功
	FTP_STATUS_NEW_CONNETCTION					=	220,// 新的联机服务ready
	FTP_STATUS_EXTENSION						=	211,//系统状态或是系统求助响应
	FTP_STATUS_SYS_TYPE							=	215,//名称系统类型
	FTP_STATUS_LOGOUT							=	221,//服务的控制连接埠关闭，可以注销
	FTP_STATUS_CLOSE_DATA_PORT					=	226,//关闭数据连接端口，请求的文件操作成功
	FTP_STATUS_PASSIV_MODE						=	227,//进入passive mode
	FTP_STATUS_USER_LOGIN						=	230,//使用者登入
	FTP_STATUS_AUTHORIZED_LOG					=	232,
	FTP_STATUS_SECURITY_AUTH					=	234,
	FTP_STATUS_SECURITY_AUTH_ADAT				=	235,
	
	FTP_STATUS_FILE_OPERATION					=	250,//请求的文件操作完成
	FTP_STATUS_PWD								=	257,//显示目前的路径名称
	FTP_STATUS_USERNAME_OK						=	331,//用户名称正确，需要密码
	FTP_STATUS_REQUIRE_SECURITY_DATA			=	334,
	FTP_STATUS_SERVER_OFF						=	421,//Server is going offline
	FTP_STATUS_SYNTAX_ERROR						=	500,
	FTP_STATUS_COMMAND_NOT_IMPLEMEMTED			=	502,
	FTP_STATUS_NOT_IMPLEMEMTED_FOR_PARA			=	504,
}FTP_STATUS_CODE;


////////////////////////////////////////////////////////////////////////////
//下面定义的参蕐和不同DVR版本相关联

#define TVT_DVR_3_0_VERSION 1

#if defined(TVT_DVR_3_0_VERSION)

#include "PUB_common.h"

#define TVT_FTP_MAX_CAMERA_NUM			(TVT_MAX_CAMERA_NUM + 4)
#define TVT_FTP_MAX_LANGUAGE_NUM		(TVT_MAX_LANGUAGE_NUM + 4)
#define TVT_FTP_MAX_TITLE_LEN			(TVT_MAX_TITLE_LEN + 4)
#define TVT_FTP_MAX_URL_LEN				(TVT_MAX_URL_LEN + 4)
#define TVT_FTP_MAX_TEXT_LEN			(TVT_MAX_TEXT_LEN + 4)
#define TVT_FTP_MAX_ACCOUNT_LEN			(TVT_MAX_ACCOUNT_LEN + 4)
#define TVT_FTP_MAX_PASSWORD_LEN		(TVT_MAX_PASSWORD_LEN + 4)

#define GetCurTime()	GetCurrTime32()

#else

#include "TVT_Config.h"

#define TVT_FTP_MAX_CAMERA_NUM			(TVT_MAX_CAMERA_NUM + 4)
#define TVT_FTP_MAX_LANGUAGE_NUM		(TVT_MAX_LANGUAGE_NUM + 4)
#define TVT_FTP_MAX_TITLE_LEN			(TVT_MAX_TITLE_LEN + 4)
#define TVT_FTP_MAX_URL_LEN				(TVT_MAX_URL_LEN + 4)
#define TVT_FTP_MAX_TEXT_LEN			(TVT_MAX_TEXT_LEN + 4)
#define TVT_FTP_MAX_ACCOUNT_LEN			(TVT_MAX_ACCOUNT_LEN + 4)
#define TVT_FTP_MAX_PASSWORD_LEN		(TVT_MAX_PASSWORD_LEN + 4)	

#define GetCurTime()	GetTime()

#endif

#endif /*__FTP_TYPE_H__*/

///////////////////////////////////////////////////////////////////////////
