#ifndef ENTITYDEF_H
#define ENTITYDEF_H

#define CHECKDEV_PERIOD 5
#define DEV_TIMEOUT 20
#define SERV_PORT 8874

//��������
typedef enum __packet_type__
{
	HEATBEAT_PACKET_TYPE, //dvr -> ipserver      �� ע���
	REQUEST_PACKET_TYPE,  //cms -> ipserver      �� �����
	ANSWER_PACKET_TYPE,   //ipserver ->dvr ��cms �� Ӧ���.
}PACKET_TYPE;

//Ӧ������ؽ��������
typedef enum __query_result__
{
	QUERY_SUCCESS = 1,		//��ѯ�ɹ�
	UPDATE_SUCCESS,			//���³ɹ�
	DEVICE_NOT_EXIST,		//�豸������
	DEVICE_NOT_ONLINE,		//�豸������
	DEVICE_DISABLED,		//�豸�ѽ���
}QUERY_RESULT;


//dvr �� ipserver ����ע���ʱ�Ľṹ��  //////////////////////////////
typedef struct _server_msg_ 
{
	PACKET_TYPE type;          //�����HEATBEAT_PACKET_TYPE
	unsigned char msg[84];
}SERVER_MSG;
typedef struct _heat_beat_packet_
{
	PACKET_TYPE		 type;
	char	 		 deviceName[68];	//�豸����
	unsigned long	 deviceIP;			//�豸IP
	unsigned short   port;				//�豸�˿�  
	unsigned char    mac[6];			//Mac��ַ
	unsigned char    resver[4];         //�����ֽڶ���
}HEAT_BEAT_PACKET;
////////////////////////////////////////////////////////////////////////



//cms �� ipserver ���������ʱ�Ľṹ��//////////////////////////////////
typedef struct _request_packet_
{
	PACKET_TYPE		type;                //�����REQUEST_PACKET_TYPE
	char 			queryInfo[68];		 //��ѯ�豸����
}REQUST_PACKET;
///////////////////////////////////////////////////////////////////////


//ipserver �ظ�DVR��cms�� Ӧ��� ʹ�õĽṹ��////////////////////////
typedef struct _answer_packet_
{
	PACKET_TYPE			type;                //�����ANSWER_PACKET_TYPE
	QUERY_RESULT 	  	result;				//��ѯ���
	char	 		  	deviceName[68];		//�豸����
	unsigned long   	deviceIP;			//�豸ip
	unsigned short 	    port;				//�豸�˿�
	unsigned char   	mac[6];				//Mac��ַ
}ANSWER_PACKET;





#endif
