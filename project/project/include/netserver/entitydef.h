#ifndef ENTITYDEF_H
#define ENTITYDEF_H

#define CHECKDEV_PERIOD 5
#define DEV_TIMEOUT 20
#define SERV_PORT 8874

//包的类型
typedef enum __packet_type__
{
	HEATBEAT_PACKET_TYPE, //dvr -> ipserver      的 注册包
	REQUEST_PACKET_TYPE,  //cms -> ipserver      的 请求包
	ANSWER_PACKET_TYPE,   //ipserver ->dvr 或cms 的 应答包.
}PACKET_TYPE;

//应答包返回结果的类型
typedef enum __query_result__
{
	QUERY_SUCCESS = 1,		//查询成功
	UPDATE_SUCCESS,			//更新成功
	DEVICE_NOT_EXIST,		//设备不存在
	DEVICE_NOT_ONLINE,		//设备不在线
	DEVICE_DISABLED,		//设备已禁用
}QUERY_RESULT;


//dvr 向 ipserver 发送注册包时的结构体  //////////////////////////////
typedef struct _server_msg_ 
{
	PACKET_TYPE type;          //这个填HEATBEAT_PACKET_TYPE
	unsigned char msg[84];
}SERVER_MSG;
typedef struct _heat_beat_packet_
{
	PACKET_TYPE		 type;
	char	 		 deviceName[68];	//设备名称
	unsigned long	 deviceIP;			//设备IP
	unsigned short   port;				//设备端口  
	unsigned char    mac[6];			//Mac地址
	unsigned char    resver[4];         //用于字节对齐
}HEAT_BEAT_PACKET;
////////////////////////////////////////////////////////////////////////



//cms 向 ipserver 发送请求包时的结构体//////////////////////////////////
typedef struct _request_packet_
{
	PACKET_TYPE		type;                //这个填REQUEST_PACKET_TYPE
	char 			queryInfo[68];		 //查询设备名称
}REQUST_PACKET;
///////////////////////////////////////////////////////////////////////


//ipserver 回复DVR或cms的 应答包 使用的结构体////////////////////////
typedef struct _answer_packet_
{
	PACKET_TYPE			type;                //这个填ANSWER_PACKET_TYPE
	QUERY_RESULT 	  	result;				//查询结果
	char	 		  	deviceName[68];		//设备名称
	unsigned long   	deviceIP;			//设备ip
	unsigned short 	    port;				//设备端口
	unsigned char   	mac[6];				//Mac地址
}ANSWER_PACKET;





#endif
