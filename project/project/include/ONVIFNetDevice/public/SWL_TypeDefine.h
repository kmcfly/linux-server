#ifndef  __SWL_TYPEDEFINE_H__
#define  __SWL_TYPEDEFINE_H__

#ifdef __ENVIRONMENT_WIN32__

/****************************************** windows ******************************/
//#include <Winsock2.h>		//liujiang
#include <afxsock.h>		//liuhao

typedef  SOCKET     SWL_socket_t;
#define  SWL_INVALID_SOCKET  INVALID_SOCKET
typedef  int        SWL_socklen_t;

//函数返回值
#define  SWL_SOCKET_ERROR  SOCKET_ERROR

//调用SWL_Shutdown时，iHow可以填的三个值
#define SWL_SHUT_RD    SD_RECEIVE      
#define SWL_SHUT_WR    SD_SEND         
//#define SWL_SHUT_BOTH  SD_BOTH		//liujiang
#define SWL_SHUT_BOTH	2				//liuhao
/****************************************** windows ******************************/

#elif defined  __ENVIRONMENT_LINUX__

/****************************************** linux ******************************/
#include <sys/types.h>
#include <sys/socket.h>
#include <net/if.h>
#include <netinet/in.h>
#include <net/if_arp.h>
#include <arpa/inet.h>

typedef  int        SWL_socket_t;
#define  SWL_INVALID_SOCKET   -1
typedef  socklen_t  SWL_socklen_t;

//函数返回值
#define  SWL_SOCKET_ERROR  -1

//调用SWL_Shutdown时，iHow可以填的三个值
#define SWL_SHUT_RD    SHUT_RD      
#define SWL_SHUT_WR    SHUT_WR         
#define SWL_SHUT_BOTH  SHUT_RDWR 
/****************************************** linux ******************************/


#else
	#error  UNKNOWN ENVIRONMENT	
#endif

#endif

