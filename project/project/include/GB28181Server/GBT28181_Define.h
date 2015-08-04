/******************************************************************************

filename:GBT28181_Define.h

describe:���ֶ���ͷ�ļ�

copyright:

author:����

date:2013/03/11

******************************************************************************/
#pragma once
#include "dvrdvsdef.h"
#include <deque>
using namespace std;

#define WAITTIME		50				//SIP�¼�����ʱ����
#define LIVETIME		(90 * 1000)		//����ʱ��
#define HEARTBEATTIME	(20 * 1000)		//�������<30s

/************************************************************************/
/*                                 xml                                  */
/************************************************************************/

enum XML_PROTOCOL_TYPE
{
	PROTOCOL_INVALID = -1,

	PROTOCOL_BASIC = 0x00000000,

	PROTOCOL_REQUEST = 0x00810000,							//��������
	PROTOCOL_CONTROL = 0x00818100,							//CONTROL����
	PROTOCOL_QUERY = 0x00818200,							//QUERY��������	
	PROTOCOL_NOTIFY = 0x00818300,							//NOTIFY����
	PROTOCOL_RESPONSE = 0x00820000,							//Ӧ������

	PROTOCOL_CONTROL_PTZ     = 0x00818101,                   //��̨����
	PROTOCOL_CONTROL_TELEBOOT = 0x00818102,                  //Զ������
	PROTOCOL_CONTROL_RECORD  =   0x00818103,                  //¼��
	PROTOCOL_CONTROL_GUARD  = 0x00818104,                     //����
	PROTOCOL_CONTROL_ALARM  =  0x00818105,                     //�澯��λ

	PROTOCOL_QUERY_DEVICESTATUS = 0x00818201,				//�豸״̬��ѯ����
	PROTOCOL_QUERY_CATALOG = 0x00818202,					//�豸Ŀ¼��Ϣ��ѯ����
	PROTOCOL_QUERY_DEVICEINFO = 0x00818203,					//�豸��Ϣ��ѯ����
	PROTOCOL_QUERY_RECORDINFO = 0x00818204,					//�ļ�Ŀ¼��������
	PROTOCOL_QUERY_ALARM = 0x00818205,						//������ѯ

	PROTOCOL_NOTIFY_KEEPALIVE = 0x00818301,					//״̬��Ϣ����
	PROTOCOL_NOTIFY_ALARM = 0x00818302,						//����֪ͨ
	PROTOCOL_NOTIFY_FILENED = 0x00818303,                   //�طŻ������ؽ����ϱ�

	PROTOCOL_RESPONSE_DEVICECONTROL = 0x00820001,			//�豸����Ӧ��
	PROTOCOL_RESPONSE_ALARM = 0x00820002,					//����֪ͨӦ��
	PROTOCOL_RESPONSE_CATALOG = 0x00820003,					//�豸Ŀ¼��Ϣ��ѯӦ��
	PROTOCOL_RESPONSE_DEVICEINFO = 0x00820004,				//�豸��Ϣ��ѯӦ��
	PROTOCOL_RESPONSE_DEVICESTATUS = 0x00820005,			//�豸״̬��Ϣ��ѯӦ��
	PROTOCOL_RESPONSE_RECORDINFO = 0x00820006,				//�ļ�Ŀ¼����Ӧ��
};

class CBasicProtocol
{
public:
	CBasicProtocol() : m_ProtocolType(PROTOCOL_BASIC)
	{
		m_dwSN=0;
		m_ProtocolType=PROTOCOL_BASIC;
	};

	/*virtual*/ ~CBasicProtocol(){};
	void Clear(){m_ProtocolType = PROTOCOL_BASIC;};

	XML_PROTOCOL_TYPE GetProtocolType(){return m_ProtocolType;};

public:
	XML_PROTOCOL_TYPE m_ProtocolType;

	DWORD m_dwSN;						//<!-- �������кţ���ѡ�� -->
};

/************************************************************************/
/*                                 ����                                 */
/************************************************************************/
class CControlProtocol : public CBasicProtocol
{
public:
	CControlProtocol()
	{
		memset(m_szDeviceID,0,sizeof(m_szDeviceID));
		memset(m_szPTZCmd,0,sizeof(m_szPTZCmd));
		m_bTeleBoot	  =  false;
		m_bRecord	  =  false;
		m_bGuard	  =  false;
		m_bResetAlarm =  false;
		m_ProtocolType = PROTOCOL_CONTROL;
	};
	/*virtual*/ ~CControlProtocol(){};
	void Clear()
	{
		memset(m_szDeviceID,0,sizeof(m_szDeviceID));
		memset(m_szPTZCmd,0,sizeof(m_szPTZCmd));
		m_bTeleBoot	  =  false;
		m_bRecord	  =  false;
		m_bGuard	  =  false;
		m_bResetAlarm =  false;
		CBasicProtocol::Clear();
		m_ProtocolType = PROTOCOL_CONTROL;
	};

	CControlProtocol(const CControlProtocol& Protocol)
	{
		Clear();
		memcpy(m_szDeviceID, Protocol.m_szDeviceID,min(sizeof(m_szDeviceID)-1, strlen(Protocol.m_szDeviceID)));
		memcpy(m_szPTZCmd, Protocol.m_szPTZCmd, min(sizeof(m_szPTZCmd)-1,strlen(Protocol.m_szPTZCmd)));

		m_bTeleBoot = Protocol.m_bTeleBoot;
		m_bRecord = Protocol.m_bRecord;
		m_bGuard = Protocol.m_bGuard;
		m_bResetAlarm = Protocol.m_bResetAlarm;
		m_ControlType = Protocol.m_ControlType;
	}

	CControlProtocol& operator=(const CControlProtocol& Protocol)
	{
		if (this == &Protocol)
		{
			return *this;
		}

		Clear();
		memcpy(m_szDeviceID, Protocol.m_szDeviceID, min(sizeof(m_szDeviceID)-1,strlen(Protocol.m_szDeviceID)));
		memcpy(m_szPTZCmd, Protocol.m_szPTZCmd, min(sizeof(m_szPTZCmd)-1,strlen(Protocol.m_szPTZCmd)));

		m_bTeleBoot = Protocol.m_bTeleBoot;
		m_bRecord = Protocol.m_bRecord;
		m_bGuard = Protocol.m_bGuard;
		m_bResetAlarm = Protocol.m_bResetAlarm;
		m_ControlType = Protocol.m_ControlType;
		return *this;
	}
 /*
	enum CONTROL_TYPE
	{
		CONTROL_TYPE_PTZCmd = 0x01,
		CONTROL_TYPE_TeleBoot = 0x02,
		CONTROL_TYPE_RecordCmd = 0x04,
		CONTROL_TYPE_GuardCmd = 0x08,
		CONTROL_TYPE_AlarmCmd = 0x10,
	};
  */
public:
	char m_szDeviceID[32];				//<!-- Ŀ���豸���루��ѡ�� -->
	char m_szPTZCmd[32];				//<!-- ���/��̨���������ѡ��������Ӧ���ϸ�¼A�е�A.3�еĹ涨) -->
	bool m_bTeleBoot;					//<!-- Զ���������������ѡ�� -->
	bool m_bRecord;						//<!-- ¼����������ѡ�� -->
	bool m_bGuard;						//<!-- ��������/���������ѡ�� -->
	bool m_bResetAlarm;					//<!-- ������λ�����ѡ�� -->
	//list m_listInfo					//<!-- ��չ��Ϣ���ɶ��� -->

	XML_PROTOCOL_TYPE m_ControlType;
};

/************************************************************************/
/*                                 ��ѯ                                 */
/************************************************************************/
//�豸״̬��ѯ����
class CQueryDeviceStatusProtocol : public CBasicProtocol
{
public:
	CQueryDeviceStatusProtocol()
	{
		memset(m_szDeviceID,0,sizeof(m_szDeviceID)); 
		m_ProtocolType = PROTOCOL_QUERY_DEVICESTATUS;
	};

	/*virtual*/ ~CQueryDeviceStatusProtocol(){};

	void Clear()
	{
		memset(m_szDeviceID,0,sizeof(m_szDeviceID));
		CBasicProtocol::Clear();
		m_ProtocolType = PROTOCOL_QUERY_DEVICESTATUS;
	};

	CQueryDeviceStatusProtocol(const CQueryDeviceStatusProtocol& Protocol)
	{
		Clear();
		memcpy(m_szDeviceID, Protocol.m_szDeviceID, min(sizeof(m_szDeviceID)-1,strlen(Protocol.m_szDeviceID)));
	}

	CQueryDeviceStatusProtocol& operator=(const CQueryDeviceStatusProtocol& Protocol)
	{
		if (this == &Protocol)
		{
			return *this;
		}

		Clear();
		memcpy(m_szDeviceID, Protocol.m_szDeviceID, min(sizeof(m_szDeviceID)-1,strlen(Protocol.m_szDeviceID)));
		return *this;
	}

public:
	char m_szDeviceID[32];				//<!-- Ŀ���豸���豸/���򡢰�ȫ������Ƶ�������ϵͳ���루��ѡ�� -->
};

//�豸Ŀ¼��Ϣ��ѯ����
class CQueryCatalogProtocol : public CBasicProtocol
{
public:
	CQueryCatalogProtocol()
	{
		memset(m_szDeviceID,0,sizeof(m_szDeviceID));
		memset(m_szStartTime,0,sizeof(m_szStartTime));
		memset(m_szEndTime,0,sizeof(m_szEndTime));

		m_ProtocolType = PROTOCOL_QUERY_CATALOG;
	};

	/*virtual*/ ~CQueryCatalogProtocol(){};
	void Clear()
	{
		memset(m_szDeviceID,0,sizeof(m_szDeviceID));
		memset(m_szStartTime,0,sizeof(m_szStartTime));
		memset(m_szEndTime,0,sizeof(m_szEndTime));

		CBasicProtocol::Clear(); 
		m_ProtocolType = PROTOCOL_QUERY_CATALOG;
	};

	CQueryCatalogProtocol(const CQueryCatalogProtocol& Protocol)
	{
		Clear();

		memcpy(m_szDeviceID, Protocol.m_szDeviceID, min(sizeof(m_szDeviceID)-1,strlen(Protocol.m_szDeviceID)));
		memcpy(m_szStartTime, Protocol.m_szStartTime, min(sizeof(m_szStartTime)-1,strlen(Protocol.m_szStartTime)));
		memcpy(m_szEndTime, Protocol.m_szEndTime, min(sizeof(m_szEndTime)-1,strlen(Protocol.m_szEndTime)));
	}

	CQueryCatalogProtocol& operator=(const CQueryCatalogProtocol& Protocol)
	{
		if (this == &Protocol)
		{
			return *this;
		}

		Clear();

		memcpy(m_szDeviceID, Protocol.m_szDeviceID, min(sizeof(m_szDeviceID)-1,strlen(Protocol.m_szDeviceID)));
		memcpy(m_szStartTime, Protocol.m_szStartTime, min(sizeof(m_szStartTime)-1,strlen(Protocol.m_szStartTime)));
		memcpy(m_szEndTime, Protocol.m_szEndTime, min(sizeof(m_szEndTime)-1,strlen(Protocol.m_szEndTime)));
		return *this;
	}

public:
	char m_szDeviceID[32];				//<!-- Ŀ���豸���豸/����/��ȫ������Ƶ�������ϵͳ/������루��ѡ�� -->
	char m_szStartTime[64];				//<!-- �����豸����ʼʱ�䣨��ѡ���ձ�ʾ���� -->
	char m_szEndTime[64];				//<!-- �����豸����ֹʱ�䣨��ѡ���ձ�ʾ����ǰʱ��-->
};

//�豸��Ϣ��ѯ����
class CQueryDeviceInfoProtocol : public CBasicProtocol
{
public:
	CQueryDeviceInfoProtocol()
	{
		memset(m_szDeviceID,0,sizeof(m_szDeviceID));
		m_ProtocolType = PROTOCOL_QUERY_DEVICEINFO;
	};

	/*virtual*/ ~CQueryDeviceInfoProtocol(){};

	void Clear()
	{
		memset(m_szDeviceID,0,sizeof(m_szDeviceID));
		CBasicProtocol::Clear(); 
		m_ProtocolType = PROTOCOL_QUERY_DEVICEINFO;
	};

	CQueryDeviceInfoProtocol(const CQueryDeviceInfoProtocol& Protocol)
	{
		Clear();
		memcpy(m_szDeviceID, Protocol.m_szDeviceID, min(sizeof(m_szDeviceID)-1,strlen(Protocol.m_szDeviceID)));
	}

	CQueryDeviceInfoProtocol& operator=(const CQueryDeviceInfoProtocol& Protocol)
	{
		if (this == &Protocol)
		{
			return *this;
		}

		Clear();
		memcpy(m_szDeviceID, Protocol.m_szDeviceID, min(sizeof(m_szDeviceID)-1,strlen(Protocol.m_szDeviceID)));
		return *this;
	}

public:
	char m_szDeviceID[32];				//<!-- Ŀ���豸���豸���루��ѡ�� -->
	         
};

//�ļ�Ŀ¼��������
class CQueryRecordInfoProtocol : public CBasicProtocol
{
public:
	CQueryRecordInfoProtocol()
	{	
		memset(m_szDeviceID,0,sizeof(m_szDeviceID));
		memset(m_szStartTime,0,sizeof(m_szStartTime));
		memset(m_szEndTime,0,sizeof(m_szEndTime));
		memset(m_szFilePath,0,sizeof(m_szFilePath));
		memset(m_szAddress,0,sizeof(m_szAddress));
		memset(m_szType,0,sizeof(m_szType));
		memset(m_szRecorderID,0,sizeof(m_szRecorderID))	;
		m_iSecrecy = 0;

		m_ProtocolType = PROTOCOL_QUERY_RECORDINFO;
	};

	/*virtual*/ ~CQueryRecordInfoProtocol(){};

	void Clear()
	{
		memset(m_szDeviceID,0,sizeof(m_szDeviceID));
		memset(m_szStartTime,0,sizeof(m_szStartTime));
		memset(m_szEndTime,0,sizeof(m_szEndTime));
		memset(m_szFilePath,0,sizeof(m_szFilePath));
		memset(m_szAddress,0,sizeof(m_szAddress));
		memset(m_szType,0,sizeof(m_szType));
		memset(m_szRecorderID,0,sizeof(m_szRecorderID))	;
		m_iSecrecy = 0;
		
		CBasicProtocol::Clear();
		m_ProtocolType = PROTOCOL_QUERY_RECORDINFO;
	};

	CQueryRecordInfoProtocol(const CQueryRecordInfoProtocol& Protocol)
	{
		Clear();


		memcpy(m_szDeviceID, Protocol.m_szDeviceID, min(sizeof(m_szDeviceID)-1,strlen(Protocol.m_szDeviceID)));
		memcpy(m_szStartTime, Protocol.m_szStartTime, min(sizeof(m_szStartTime)-1,strlen(Protocol.m_szStartTime)));
		memcpy(m_szEndTime, Protocol.m_szEndTime, min(sizeof(m_szEndTime)-1,strlen(Protocol.m_szEndTime)));
		memcpy(m_szFilePath, Protocol.m_szFilePath, min(sizeof(m_szFilePath)-1,strlen(Protocol.m_szFilePath)));
		memcpy(m_szAddress, Protocol.m_szAddress, min(sizeof(m_szAddress)-1,strlen(Protocol.m_szAddress)));
		memcpy(m_szType, Protocol.m_szType, min(sizeof(m_szType)-1,strlen(Protocol.m_szType)));
		memcpy(m_szRecorderID, Protocol.m_szRecorderID, min(sizeof(m_szRecorderID)-1,strlen(Protocol.m_szRecorderID)));

		m_iSecrecy = Protocol.m_iSecrecy;
	
	}

	CQueryRecordInfoProtocol& operator=(const CQueryRecordInfoProtocol& Protocol)
	{
		if (this == &Protocol)
		{
			return *this;
		}

		Clear();

		memcpy(m_szDeviceID, Protocol.m_szDeviceID, min(sizeof(m_szDeviceID)-1,strlen(Protocol.m_szDeviceID)));
		memcpy(m_szStartTime, Protocol.m_szStartTime, min(sizeof(m_szStartTime)-1,strlen(Protocol.m_szStartTime)));
		memcpy(m_szEndTime, Protocol.m_szEndTime, min(sizeof(m_szEndTime)-1,strlen(Protocol.m_szEndTime)));
		memcpy(m_szFilePath, Protocol.m_szFilePath, min(sizeof(m_szFilePath)-1,strlen(Protocol.m_szFilePath)));
		memcpy(m_szAddress, Protocol.m_szAddress, min(sizeof(m_szAddress)-1,strlen(Protocol.m_szAddress)));
		memcpy(m_szType, Protocol.m_szType, min(sizeof(m_szType)-1,strlen(Protocol.m_szType)));
		memcpy(m_szRecorderID, Protocol.m_szRecorderID, min(sizeof(m_szRecorderID)-1,strlen(Protocol.m_szRecorderID)));

		m_iSecrecy = Protocol.m_iSecrecy;

		return *this;
	}

public:
	char m_szDeviceID[32];				//<!-- Ŀ¼�豸/��ȫ������Ƶ�������ϵͳ/������루��ѡ�� -->
	char m_szStartTime[64];				//<!-- ¼����ʼʱ�䣨��ѡ���ձ�ʾ���� -->
	char m_szEndTime[64];				//<!-- ����¼����ֹʱ�䣨��ѡ���ձ�ʾ����ǰʱ��-->
	char m_szFilePath[256];				//<!-- �ļ�·���� ����ѡ��-->
	char m_szAddress[256];				//<!-- ¼���ַ����ѡ ֧�ֲ���ȫ��ѯ�� -->
	int m_iSecrecy;						//<!-- �������ԣ���ѡ��ȱʡΪ0��0�������ܣ�1������-->
	char m_szType[10];					//<!-- ¼��������ͣ���ѡ��time��alarm��manual��all -->
	char m_szRecorderID[32];			//<!-- ¼�񴥷���ID����ѡ��-->
};

//������ѯ
class CQueryAlarmProtocol : public CBasicProtocol
{
public:
	CQueryAlarmProtocol()
	{
		memset(m_szDeviceID,0,sizeof(m_szDeviceID));
		memset(m_szStartAlarmPriority,0,sizeof(m_szStartAlarmPriority));
		memset(m_szEndAlarmPriority,0,sizeof(m_szEndAlarmPriority));
		memset(m_szAlarmMethod,0,sizeof(m_szAlarmMethod));
		memset(m_szStartAlarmTime,0,sizeof(m_szStartAlarmTime));
		memset(m_szEndAlarmTime,0,sizeof(m_szEndAlarmTime));

		m_ProtocolType = PROTOCOL_QUERY_ALARM;
	};
	/*virtual*/ ~CQueryAlarmProtocol(){};
	void Clear()
	{
		memset(m_szDeviceID,0,sizeof(m_szDeviceID));
		memset(m_szStartAlarmPriority,0,sizeof(m_szStartAlarmPriority));
		memset(m_szEndAlarmPriority,0,sizeof(m_szEndAlarmPriority));
		memset(m_szAlarmMethod,0,sizeof(m_szAlarmMethod));
		memset(m_szStartAlarmTime,0,sizeof(m_szStartAlarmTime));
		memset(m_szEndAlarmTime,0,sizeof(m_szEndAlarmTime));

		CBasicProtocol::Clear(); 
		m_ProtocolType = PROTOCOL_QUERY_ALARM;
	};

	CQueryAlarmProtocol(const CQueryAlarmProtocol& Protocol)
	{
		Clear();


		memcpy(m_szDeviceID, Protocol.m_szDeviceID, min(sizeof(m_szDeviceID)-1,strlen(Protocol.m_szDeviceID)));
		memcpy(m_szStartAlarmPriority, Protocol.m_szStartAlarmPriority, min(sizeof(m_szStartAlarmPriority)-1,strlen(Protocol.m_szStartAlarmPriority)));
		memcpy(m_szEndAlarmPriority, Protocol.m_szEndAlarmPriority, min(sizeof(m_szEndAlarmPriority)-1,strlen(Protocol.m_szEndAlarmPriority)));
		memcpy(m_szAlarmMethod, Protocol.m_szAlarmMethod, min(sizeof(m_szAlarmMethod)-1,strlen(Protocol.m_szAlarmMethod)));
		memcpy(m_szStartAlarmTime, Protocol.m_szStartAlarmTime, min(sizeof(m_szStartAlarmTime)-1,strlen(Protocol.m_szStartAlarmTime)));
		memcpy(m_szEndAlarmTime, Protocol.m_szEndAlarmTime, min(sizeof(m_szEndAlarmTime)-1,strlen(Protocol.m_szEndAlarmTime)));


	}

	CQueryAlarmProtocol& operator=(const CQueryAlarmProtocol& Protocol)
	{
		if (this == &Protocol)
		{
			return *this;
		}

		Clear();

		memcpy(m_szDeviceID, Protocol.m_szDeviceID, min(sizeof(m_szDeviceID)-1,strlen(Protocol.m_szDeviceID)));
		memcpy(m_szStartAlarmPriority, Protocol.m_szStartAlarmPriority, min(sizeof(m_szStartAlarmPriority)-1,strlen(Protocol.m_szStartAlarmPriority)));
		memcpy(m_szEndAlarmPriority, Protocol.m_szEndAlarmPriority, min(sizeof(m_szEndAlarmPriority)-1,strlen(Protocol.m_szEndAlarmPriority)));
		memcpy(m_szAlarmMethod, Protocol.m_szAlarmMethod, min(sizeof(m_szAlarmMethod)-1,strlen(Protocol.m_szAlarmMethod)));
		memcpy(m_szStartAlarmTime, Protocol.m_szStartAlarmTime, min(sizeof(m_szStartAlarmTime)-1,strlen(Protocol.m_szStartAlarmTime)));
		memcpy(m_szEndAlarmTime, Protocol.m_szEndAlarmTime, min(sizeof(m_szEndAlarmTime)-1,strlen(Protocol.m_szEndAlarmTime)));
		return *this;
	}

public:
	//char m_szDeviceID[11];				//<!-- �����豸����򱨾����ı��루10λ������ѡ��-->
	char m_szDeviceID[32];
	char m_szStartAlarmPriority[10];	//<!-- ������ʼ���𣨿�ѡ����0Ϊȫ����1Ϊһ�����飬2Ϊ�������飬3Ϊ�������飬4Ϊ�ļ�����-->
	char m_szEndAlarmPriority[10];		//<!-- ������ֹ���𣨿�ѡ����0Ϊȫ����1Ϊһ�����飬2Ϊ�������飬3Ϊ�������飬4Ϊ�ļ�����-->
	char m_szAlarmMethod[10];			//<!-- ������ʽ��������ѡ����ȡֵ0Ϊȫ����1Ϊ�绰������2Ϊ�豸������3Ϊ���ű�����4ΪGPS������
										//5Ϊ��Ƶ������6Ϊ�豸���ϱ�����7��������������Ϊֱ�������12Ϊ�绰�������豸����-->
	char m_szStartAlarmTime[64];		//<!-- �����豸����ʼʱ�䣨��ѡ���ձ�ʾ���� -->
	char m_szEndAlarmTime[64];			//<!-- �����豸����ֹʱ�䣨��ѡ���ձ�ʾ����ǰʱ��-->
};

/************************************************************************/
/*                                 ֪ͨ                                 */
/************************************************************************/
//״̬��Ϣ����
class CNotifyKeepAliveProtocol : public CBasicProtocol
{
public:
	CNotifyKeepAliveProtocol()
	{
		memset(m_szDeviceID,0,sizeof(m_szDeviceID));
		m_bStatus = 0;
		m_ProtocolType = PROTOCOL_NOTIFY_KEEPALIVE;
	};

	/*virtual*/ ~CNotifyKeepAliveProtocol(){};
	void Clear()
	{
		memset(m_szDeviceID,0,sizeof(m_szDeviceID));
		m_bStatus = 0;

		CBasicProtocol::Clear(); 
		m_ProtocolType = PROTOCOL_NOTIFY_KEEPALIVE;
	};

	CNotifyKeepAliveProtocol(const CNotifyKeepAliveProtocol& Protocol)
	{
		Clear();

		memcpy(m_szDeviceID, Protocol.m_szDeviceID, min(sizeof(m_szDeviceID)-1,strlen(Protocol.m_szDeviceID)));

		m_bStatus = Protocol.m_bStatus;
	}

	CNotifyKeepAliveProtocol& operator=(const CNotifyKeepAliveProtocol& Protocol)
	{
		if (this == &Protocol)
		{
			return *this;
		}

		Clear();

		memcpy(m_szDeviceID, Protocol.m_szDeviceID, min(sizeof(m_szDeviceID)-1,strlen(Protocol.m_szDeviceID)));
		m_bStatus = Protocol.m_bStatus;
		return *this;
	}

public:
	char m_szDeviceID[32];				//<!-- Դ�豸���豸/ϵͳ���루��ѡ�� -->
	bool m_bStatus;						//<!-- �Ƿ�������������ѡ�� -->
};

//����֪ͨ
class CNotifyAlarmProtocol : public CBasicProtocol
{
public:
	CNotifyAlarmProtocol()
	{
		memset(m_szDeviceID,0,sizeof(m_szDeviceID));
		memset(m_szAlarmPriority,0,sizeof(m_szAlarmPriority));
		memset(m_szAlarmMethod,0,sizeof(m_szAlarmMethod));
		memset(m_szAlarmTime,0,sizeof(m_szAlarmTime));
		memset(m_szAlarmDescription,0,sizeof(m_szAlarmDescription));

		m_fLongitude =0.0;
		m_fLatitude =0.0;

		m_ProtocolType = PROTOCOL_NOTIFY_ALARM;
	};

	/*virtual*/ ~CNotifyAlarmProtocol(){};
	void Clear()
	{
		memset(m_szDeviceID,0,sizeof(m_szDeviceID));
		memset(m_szAlarmPriority,0,sizeof(m_szAlarmPriority));
		memset(m_szAlarmMethod,0,sizeof(m_szAlarmMethod));
		memset(m_szAlarmTime,0,sizeof(m_szAlarmTime));
		memset(m_szAlarmDescription,0,sizeof(m_szAlarmDescription));
		m_fLongitude =0.0;
		m_fLatitude =0.0;

		CBasicProtocol::Clear();
		m_ProtocolType = PROTOCOL_NOTIFY_ALARM;
	};

	CNotifyAlarmProtocol(const CNotifyAlarmProtocol& Protocol)
	{
		Clear();

		memcpy(m_szDeviceID, Protocol.m_szDeviceID, min(sizeof(m_szDeviceID)-1,strlen(Protocol.m_szDeviceID)));
		memcpy(m_szAlarmPriority, Protocol.m_szAlarmPriority, min(sizeof(m_szAlarmPriority)-1,strlen(Protocol.m_szAlarmPriority)));
		memcpy(m_szAlarmMethod, Protocol.m_szAlarmMethod, min(sizeof(m_szAlarmMethod)-1,strlen(Protocol.m_szAlarmMethod)));

		memcpy(m_szAlarmTime, Protocol.m_szAlarmTime, min(sizeof(m_szAlarmTime)-1,strlen(Protocol.m_szAlarmTime)));
		memcpy(m_szAlarmDescription, Protocol.m_szAlarmDescription, min(sizeof(m_szAlarmDescription)-1,strlen(Protocol.m_szAlarmDescription)));
	

		m_fLongitude = Protocol.m_fLongitude;
		m_fLatitude = Protocol.m_fLatitude;
	}

	CNotifyAlarmProtocol& operator=(const CNotifyAlarmProtocol& Protocol)
	{
		if (this == &Protocol)
		{
			return *this;
		}

		Clear();

		memcpy(m_szDeviceID, Protocol.m_szDeviceID, min(sizeof(m_szDeviceID)-1,strlen(Protocol.m_szDeviceID)));
		memcpy(m_szAlarmPriority, Protocol.m_szAlarmPriority, min(sizeof(m_szAlarmPriority)-1,strlen(Protocol.m_szAlarmPriority)));
		memcpy(m_szAlarmMethod, Protocol.m_szAlarmMethod, min(sizeof(m_szAlarmMethod)-1,strlen(Protocol.m_szAlarmMethod)));

		memcpy(m_szAlarmTime, Protocol.m_szAlarmTime, min(sizeof(m_szAlarmTime)-1,strlen(Protocol.m_szAlarmTime)));
		memcpy(m_szAlarmDescription, Protocol.m_szAlarmDescription, min(sizeof(m_szAlarmDescription)-1,strlen(Protocol.m_szAlarmDescription)));

		m_fLongitude = Protocol.m_fLongitude;
		m_fLatitude = Protocol.m_fLatitude;
		return *this;
	}

public:
	//char m_szDeviceID[11];				//<!-- �����豸����򱨾����ı��루10λ������ѡ��-->
	char m_szDeviceID[32];
	char m_szAlarmPriority[10];		//<!-- �������𣨱�ѡ����1Ϊһ�����飬2Ϊ�������飬3Ϊ�������飬4Ϊ�ļ�����-->
	char m_szAlarmMethod[10];			//<!-- ������ʽ����ѡ����ȡֵ1Ϊ�绰������2Ϊ�豸������3Ϊ���ű�����4ΪGPS������5Ϊ��Ƶ������6Ϊ�豸���ϱ�����7��������-->
		
	char	m_szAlarmTime[64];           //<!--����ʱ�䣨��ѡ��-->
	char m_szAlarmDescription[512];		//<!--����������������ѡ��-->
	double m_fLongitude;				//<!-- ��γ����Ϣ��ѡ -->
	double m_fLatitude;					//<!-- ��γ����Ϣ��ѡ -->
	//list m_listInfo					//<!-- ��չ��Ϣ���ɶ��� -->
};


//״̬��Ϣ����
class CNotifyFileEndProtocol : public CBasicProtocol
{
public:
	CNotifyFileEndProtocol()
	{
		memset(m_szDeviceID,0,sizeof(m_szDeviceID));
		m_ProtocolType = PROTOCOL_NOTIFY_FILENED;
	};

	/*virtual*/ ~CNotifyFileEndProtocol(){};
	void Clear()
	{
		memset(m_szDeviceID,0,sizeof(m_szDeviceID));
		memset(m_szNotifyType,0,sizeof(m_szNotifyType));
		CBasicProtocol::Clear(); 
		m_ProtocolType = PROTOCOL_NOTIFY_KEEPALIVE;
	};

	CNotifyFileEndProtocol(const CNotifyFileEndProtocol& Protocol)
	{
		Clear();

		memcpy(m_szDeviceID, Protocol.m_szDeviceID, min(sizeof(m_szDeviceID)-1,strlen(Protocol.m_szDeviceID)));
	    memcpy(m_szNotifyType, Protocol.m_szNotifyType, min(sizeof(m_szNotifyType)-1,strlen(Protocol.m_szNotifyType)));
	}

	CNotifyFileEndProtocol& operator=(const CNotifyFileEndProtocol& Protocol)
	{
		if (this == &Protocol)
		{
			return *this;
		}

		Clear();

		memcpy(m_szDeviceID, Protocol.m_szDeviceID, min(sizeof(m_szDeviceID)-1,strlen(Protocol.m_szDeviceID)));
		memcpy(m_szNotifyType, Protocol.m_szNotifyType, min(sizeof(m_szNotifyType)-1,strlen(Protocol.m_szNotifyType)));
		return *this;
	}

public:
	char m_szDeviceID[32];				//<!-- Դ�豸���豸/ϵͳ���루��ѡ�� -->
	char m_szNotifyType[8];					//<!-- ֪ͨ���� -->
};


/************************************************************************/
/*                                 Ӧ��                                 */
/************************************************************************/
//�豸����Ӧ��
class CResponseDeviceControlProtocol : public CBasicProtocol
{
public:
	CResponseDeviceControlProtocol()
	{
		memset(m_szDeviceID,0,sizeof(m_szDeviceID));
		m_bResult = 0;
		m_ProtocolType = PROTOCOL_RESPONSE_DEVICECONTROL;
	};

	/*virtual*/ ~CResponseDeviceControlProtocol(){};

	void Clear()
	{
		memset(m_szDeviceID,0,sizeof(m_szDeviceID));
		m_bResult = 0;

		CBasicProtocol::Clear(); 
		m_ProtocolType = PROTOCOL_RESPONSE_DEVICECONTROL;
	};

	CResponseDeviceControlProtocol(const CResponseDeviceControlProtocol& Protocol)
	{
		Clear();
		memcpy(m_szDeviceID, Protocol.m_szDeviceID, min(sizeof(m_szDeviceID)-1,strlen(Protocol.m_szDeviceID)));
		m_bResult = Protocol.m_bResult;
	}

	CResponseDeviceControlProtocol& operator=(const CResponseDeviceControlProtocol& Protocol)
	{
		if (this == &Protocol)
		{
			return *this;
		}

		Clear();
		memcpy(m_szDeviceID, Protocol.m_szDeviceID, min(sizeof(m_szDeviceID)-1,strlen(Protocol.m_szDeviceID)));
		m_bResult = Protocol.m_bResult;
		return *this;
	}

public:
	char m_szDeviceID[32];				//<!-- Ŀ���豸/����/ϵͳ���루��ѡ�� -->
	bool m_bResult;						//<!-- ִ�н����־����ѡ�� -->
};

//����֪ͨӦ��
class CResponseAlarmProtocol : public CBasicProtocol
{
public:
	CResponseAlarmProtocol()
	{
		memset(m_szDeviceID,0,sizeof(m_szDeviceID));
		m_bResult = 0;

		m_ProtocolType = PROTOCOL_RESPONSE_ALARM;
	};

	/*virtual*/ ~CResponseAlarmProtocol(){};

	void Clear()
	{
		memset(m_szDeviceID,0,sizeof(m_szDeviceID));
		m_bResult = 0;

		CBasicProtocol::Clear();
		m_ProtocolType = PROTOCOL_RESPONSE_ALARM;
	};

	CResponseAlarmProtocol(const CResponseAlarmProtocol& Protocol)
	{
		Clear();
		memcpy(m_szDeviceID, Protocol.m_szDeviceID, min(sizeof(m_szDeviceID)-1,strlen(Protocol.m_szDeviceID)));
		m_bResult = Protocol.m_bResult;
	}

	CResponseAlarmProtocol& operator=(const CResponseAlarmProtocol& Protocol)
	{
		if (this == &Protocol)
		{
			return *this;
		}

		Clear();
		memcpy(m_szDeviceID, Protocol.m_szDeviceID, min(sizeof(m_szDeviceID)-1,strlen(Protocol.m_szDeviceID)));
		m_bResult = Protocol.m_bResult;
		return *this;
	}

public:
	char m_szDeviceID[32];				//<!-- Ŀ���豸/����/ϵͳ���루��ѡ�� -->
	bool m_bResult;						//<!-- ִ�н����־����ѡ�� -->
};

//�豸Ŀ¼��Ϣ��ѯӦ��
class CCatalogItem
{
public:
	CCatalogItem()
	{
		Clear();
	};
	/*virtual*/ ~CCatalogItem(){};
	
	void Clear()
	{
		memset(m_szDeviceID,0,sizeof(m_szDeviceID));

		memset(m_szName,0,sizeof(m_szName));
		memset(m_szManufacturer,0,sizeof(m_szManufacturer));
		memset(m_szModel,0,sizeof(m_szModel));
		memset(m_szOwner,0,sizeof(m_szOwner));

		memset(m_szCivilCode,0,sizeof(m_szCivilCode));
		memset(m_szBlock,0,sizeof(m_szBlock));
		memset(m_szAddress,0,sizeof(m_szAddress));
		memset(m_szParental,0,sizeof(m_szParental));

		memset(m_szParentID,0,sizeof(m_szParentID));
		memset(m_szSafetyWay,0,sizeof(m_szSafetyWay));
		memset(m_szRegisterWay,0,sizeof(m_szRegisterWay));
		memset(m_szCertNum,0,sizeof(m_szCertNum));

		memset(m_szCertifiable,0,sizeof(m_szCertifiable));
		memset(m_szErrCode,0,sizeof(m_szErrCode));
		memset(m_szEndTime,0,sizeof(m_szEndTime));
		memset(m_szIPAddress,0,sizeof(m_szIPAddress));

		memset(m_szPassword,0,sizeof(m_szPassword));

		m_bSecrecy=false;
		m_bStatus =false;
		m_dwPort  = 0;
		m_fLongitude = 0.0;
		m_fLatitude = 0.0;
	
	}
	CCatalogItem(const CCatalogItem& Protocol)
	{
		Clear();	//push_back

		memcpy(m_szDeviceID, Protocol.m_szDeviceID, min(sizeof(m_szDeviceID)-1,strlen(Protocol.m_szDeviceID)));
		memcpy(m_szName, Protocol.m_szName, min(sizeof(m_szName)-1,strlen(Protocol.m_szName)));
		memcpy(m_szManufacturer, Protocol.m_szManufacturer, min(sizeof(m_szManufacturer)-1,strlen(Protocol.m_szManufacturer)));
		memcpy(m_szModel, Protocol.m_szModel, min(sizeof(m_szModel)-1,strlen(Protocol.m_szModel)));
		memcpy(m_szOwner, Protocol.m_szOwner, min(sizeof(m_szOwner)-1,strlen(Protocol.m_szOwner)));
		memcpy(m_szCivilCode, Protocol.m_szCivilCode, min(sizeof(m_szCivilCode)-1,strlen(Protocol.m_szCivilCode)));
		memcpy(m_szBlock, Protocol.m_szBlock, min(sizeof(m_szBlock)-1,strlen(Protocol.m_szBlock)));
		memcpy(m_szAddress, Protocol.m_szAddress, min(sizeof(m_szAddress)-1,strlen(Protocol.m_szAddress)));
		memcpy(m_szParental, Protocol.m_szParental, min(sizeof(m_szParental)-1,strlen(Protocol.m_szParental)));
		memcpy(m_szSafetyWay, Protocol.m_szSafetyWay, min(sizeof(m_szSafetyWay)-1,strlen(Protocol.m_szSafetyWay)));
		memcpy(m_szRegisterWay, Protocol.m_szRegisterWay, min(sizeof(m_szRegisterWay)-1,strlen(Protocol.m_szRegisterWay)));
		memcpy(m_szCertNum, Protocol.m_szCertNum, min(sizeof(m_szCertNum)-1,strlen(Protocol.m_szCertNum)));
		memcpy(m_szCertifiable, Protocol.m_szCertifiable, min(sizeof(m_szCertifiable)-1,strlen(Protocol.m_szCertifiable)));
		memcpy(m_szErrCode, Protocol.m_szErrCode, min(sizeof(m_szErrCode)-1,strlen(Protocol.m_szErrCode)));
		memcpy(m_szEndTime, Protocol.m_szEndTime, min(sizeof(m_szEndTime)-1,strlen(Protocol.m_szEndTime)));
		memcpy(m_szIPAddress, Protocol.m_szIPAddress, min(sizeof(m_szIPAddress)-1,strlen(Protocol.m_szIPAddress)));
		memcpy(m_szPassword, Protocol.m_szPassword, min(sizeof(m_szPassword)-1,strlen(Protocol.m_szPassword)));

		m_bSecrecy = Protocol.m_bSecrecy;
		m_dwPort = Protocol.m_dwPort;
		m_bStatus = Protocol.m_bStatus;
		m_fLongitude = Protocol.m_fLongitude;
		m_fLatitude = Protocol.m_fLatitude;
	}

	CCatalogItem& operator=(const CCatalogItem& Protocol)
	{
		if (this == &Protocol)
		{
			return *this;
		}

		Clear();

		memcpy(m_szDeviceID, Protocol.m_szDeviceID, min(sizeof(m_szDeviceID)-1,strlen(Protocol.m_szDeviceID)));
		memcpy(m_szName, Protocol.m_szName, min(sizeof(m_szName)-1,strlen(Protocol.m_szName)));
		memcpy(m_szManufacturer, Protocol.m_szManufacturer, min(sizeof(m_szManufacturer)-1,strlen(Protocol.m_szManufacturer)));
		memcpy(m_szModel, Protocol.m_szModel, min(sizeof(m_szModel)-1,strlen(Protocol.m_szModel)));
		memcpy(m_szOwner, Protocol.m_szOwner, min(sizeof(m_szOwner)-1,strlen(Protocol.m_szOwner)));
		memcpy(m_szCivilCode, Protocol.m_szCivilCode, min(sizeof(m_szCivilCode)-1,strlen(Protocol.m_szCivilCode)));
		memcpy(m_szBlock, Protocol.m_szBlock, min(sizeof(m_szBlock)-1,strlen(Protocol.m_szBlock)));
		memcpy(m_szAddress, Protocol.m_szAddress, min(sizeof(m_szAddress)-1,strlen(Protocol.m_szAddress)));
		memcpy(m_szParental, Protocol.m_szParental, min(sizeof(m_szParental)-1,strlen(Protocol.m_szParental)));
		memcpy(m_szSafetyWay, Protocol.m_szSafetyWay, min(sizeof(m_szSafetyWay)-1,strlen(Protocol.m_szSafetyWay)));
		memcpy(m_szRegisterWay, Protocol.m_szRegisterWay, min(sizeof(m_szRegisterWay)-1,strlen(Protocol.m_szRegisterWay)));
		memcpy(m_szCertNum, Protocol.m_szCertNum, min(sizeof(m_szCertNum)-1,strlen(Protocol.m_szCertNum)));
		memcpy(m_szCertifiable, Protocol.m_szCertifiable, min(sizeof(m_szCertifiable)-1,strlen(Protocol.m_szCertifiable)));
		memcpy(m_szErrCode, Protocol.m_szErrCode, min(sizeof(m_szErrCode)-1,strlen(Protocol.m_szErrCode)));
		memcpy(m_szEndTime, Protocol.m_szEndTime, min(sizeof(m_szEndTime)-1,strlen(Protocol.m_szEndTime)));
		memcpy(m_szIPAddress, Protocol.m_szIPAddress, min(sizeof(m_szIPAddress)-1,strlen(Protocol.m_szIPAddress)));
		memcpy(m_szPassword, Protocol.m_szPassword, min(sizeof(m_szPassword)-1,strlen(Protocol.m_szPassword)));

		m_bSecrecy = Protocol.m_bSecrecy;
		m_dwPort = Protocol.m_dwPort;
		m_bStatus = Protocol.m_bStatus;
		m_fLongitude = Protocol.m_fLongitude;
		m_fLatitude = Protocol.m_fLatitude;
		return *this;
	}

public:
	char m_szDeviceID[32];				//<!-- �豸/����/ϵͳ���루��ѡ�� -->
	char m_szName[128];					//<!-- �豸/����/ϵͳ���ƣ���ѡ�� -->
	char m_szManufacturer[64];			//<!-- ��Ϊ�豸ʱ���豸���̣���ѡ�� -->
	char m_szModel[64];					//<!-- ��Ϊ�豸ʱ���豸�ͺţ���ѡ�� -->
	char m_szOwner[64];					//<!-- ��Ϊ�豸ʱ���豸��������ѡ�� -->
	char m_szCivilCode[64];				//<!-- �������򣨱�ѡ�� -->
	char m_szBlock[64];					//<!-- ��������ѡ�� -->
	char m_szAddress[64];				//<!-- ��Ϊ�豸ʱ����װ��ַ����ѡ�� -->
	char m_szParental[10];				//<!-- ��Ϊ�豸ʱ���Ƿ������豸����ѡ��1�У�0û�� -->
	char m_szParentID[32];				//<!-- ���豸/����/ϵͳID����ѡ���и��豸��Ҫ��д�� -->
	char m_szSafetyWay[10];				//<!-- ���ȫģʽ����ѡ��ȱʡΪ0�� 0�������ã�2��S/MIMEǩ����ʽ��3��S/MIME����ǩ��ͬʱ���÷�ʽ��4������ժҪ��ʽ-->
	char m_szRegisterWay[10];			//<!-- ע�᷽ʽ����ѡ��ȱʡΪ1�� 1�� ����sip3261��׼����֤ע��ģʽ��2�� ���ڿ����˫����֤ע��ģʽ��3�� ��������֤���˫����֤ע��ģʽ-->
	char m_szCertNum[64];				//<!-- ֤�����кţ���֤����豸��ѡ�� -->
	char m_szCertifiable[10];			//<!-- ֤����Ч��ʶ����֤����豸��ѡ��ȱʡΪ0��֤����Ч��ʶ�� 0����Ч 1����Ч-->
	char m_szErrCode[32];				//<!-- ��Чԭ���루��֤����֤����Ч���豸��ѡ�� -->
	char m_szEndTime[64];				//<!-- ֤����ֹ��Ч�ڣ���֤����豸��ѡ�� -->
	bool m_bSecrecy;					//<!-- �������ԣ���ѡ��ȱʡΪ0��0�������ܣ�1������-->
	char m_szIPAddress[16];				//<!-- �豸/����/ϵͳIP��ַ����ѡ��-->
	DWORD m_dwPort;						//<!-- �豸/����/ϵͳ�˿ڣ���ѡ��-->
	char m_szPassword[64];				//<!-- �豸�����ѡ��-->
	bool m_bStatus;						//<!-- �豸״̬����ѡ�� -->
	double m_fLongitude;				//<!-- ���ȣ���ѡ�� -->
	double m_fLatitude;					//<!-- γ�ȣ���ѡ�� -->
};
class CResponseCatalogProtocol : public CBasicProtocol
{
public:
	CResponseCatalogProtocol()
	{
		memset(m_szDeviceID,0,sizeof(m_szDeviceID));
		memset(m_szSumNum,0,sizeof(m_szSumNum));
		m_bResult  =false;

		m_ResponseCatalogType = RESPONSECATALOG_TYPE_1;
		m_ProtocolType = PROTOCOL_RESPONSE_CATALOG;
	};
	/*virtual*/ ~CResponseCatalogProtocol(){};

	void Clear()
	{
		m_deqItem.clear(); 

		memset(m_szDeviceID,0,sizeof(m_szDeviceID));
		memset(m_szSumNum,0,sizeof(m_szSumNum));
		m_bResult  =false;

		CBasicProtocol::Clear(); 
		m_ProtocolType = PROTOCOL_RESPONSE_CATALOG;
	};
	void Delete()
	{
		deque<CCatalogItem*>::iterator it = m_deqItem.begin();
		for (; it != m_deqItem.end(); it++)
		{
			CCatalogItem* pItem = *it;
			if (pItem)
			{
				delete pItem;
				pItem = NULL;
			}
		}
		m_deqItem.clear();
	};

	CResponseCatalogProtocol(const CResponseCatalogProtocol& Protocol)
	{
		Clear();
		
		memcpy(m_szDeviceID, Protocol.m_szDeviceID, min(sizeof(m_szDeviceID)-1,strlen(Protocol.m_szDeviceID)));
		memcpy(m_szSumNum, Protocol.m_szSumNum, min(sizeof(m_szSumNum)-1,strlen(Protocol.m_szSumNum)));

		m_deqItem = Protocol.m_deqItem;
		m_bResult = Protocol.m_bResult;
		m_ResponseCatalogType = Protocol.m_ResponseCatalogType;
	}

	CResponseCatalogProtocol& operator=(const CResponseCatalogProtocol& Protocol)
	{
		if (this == &Protocol)
		{
			return *this;
		}

		Clear();

		memcpy(m_szDeviceID, Protocol.m_szDeviceID, min(sizeof(m_szDeviceID)-1,strlen(Protocol.m_szDeviceID)));
		memcpy(m_szSumNum, Protocol.m_szSumNum, min(sizeof(m_szSumNum)-1,strlen(Protocol.m_szSumNum)));

		m_deqItem = Protocol.m_deqItem;
		m_bResult = Protocol.m_bResult;
		m_ResponseCatalogType = Protocol.m_ResponseCatalogType;
		return *this;
	}

	enum RESPONSECATALOG_TYPE
	{
		RESPONSECATALOG_TYPE_1,								//�豸Ŀ¼��Ϣ��ѯӦ��
		RESPONSECATALOG_TYPE_2,								//Ŀ¼��Ϣ��ѯ�յ�Ӧ��
		RESPONSECATALOG_TYPE_3 = RESPONSECATALOG_TYPE_2		//Ŀ¼�յ�Ӧ��
	};

public:
	char m_szDeviceID[32];				//<!-- Ŀ���豸/����/ϵͳ�ı��루��ѡ�� -->
	char m_szSumNum[10];				//<!-- ��ѯ�����������ѡ�� -->
	deque<CCatalogItem*> m_deqItem;		//<!-- �豸Ŀ¼���б�,Num��ʾĿ¼����� -->
	//list m_listInfo					//<!-- ��չ��Ϣ���ɶ��� -->
	bool m_bResult;						//<!-- ִ�н����־����ѡ�� -->

	RESPONSECATALOG_TYPE m_ResponseCatalogType;
};

//�豸��Ϣ��ѯӦ��
class CResponseDeviceInfoProtocol : public CBasicProtocol
{
public:
	CResponseDeviceInfoProtocol()
	{
		memset(m_szDeviceID,0,sizeof(m_szDeviceID));
		memset(m_szManufacturer,0,sizeof(m_szManufacturer));
		memset(m_szModel,0,sizeof(m_szModel));
		memset(m_szFirmware,0,sizeof(m_szFirmware));
		memset(MaxCamera,0,sizeof(MaxCamera));
		memset(MaxALarm,0,sizeof(MaxALarm));
		memset(BuildTime,0,sizeof(BuildTime));
		memset(deviceName,0,sizeof(deviceName));

		m_bResult = false;
		m_dwChannel = 0;

		m_ProtocolType = PROTOCOL_RESPONSE_DEVICEINFO;
	};

	/*virtual*/ ~CResponseDeviceInfoProtocol(){};

	void Clear()
	{
		memset(m_szDeviceID,0,sizeof(m_szDeviceID));
		memset(m_szManufacturer,0,sizeof(m_szManufacturer));
		memset(m_szModel,0,sizeof(m_szModel));
		memset(m_szFirmware,0,sizeof(m_szFirmware));
		memset(MaxCamera,0,sizeof(MaxCamera));
		memset(MaxALarm,0,sizeof(MaxALarm));
		memset(BuildTime,0,sizeof(BuildTime));
		memset(deviceName,0,sizeof(deviceName));

		m_bResult = false;
		m_dwChannel = 0;

		CBasicProtocol::Clear();
		m_ProtocolType = PROTOCOL_RESPONSE_DEVICEINFO;
	};

	CResponseDeviceInfoProtocol(const CResponseDeviceInfoProtocol& Protocol)
	{
		Clear();
	
		m_bResult = Protocol.m_bResult;

		memcpy(m_szDeviceID, Protocol.m_szDeviceID, min(sizeof(m_szDeviceID)-1,strlen(Protocol.m_szDeviceID)));
		memcpy(m_szManufacturer, Protocol.m_szManufacturer, min(sizeof(m_szManufacturer)-1,strlen(Protocol.m_szManufacturer)));
		memcpy(m_szModel, Protocol.m_szModel, min(sizeof(m_szModel)-1,strlen(Protocol.m_szModel)));
		memcpy(m_szFirmware, Protocol.m_szFirmware, min(sizeof(m_szFirmware)-1,strlen(Protocol.m_szFirmware)));

		m_dwChannel = Protocol.m_dwChannel;
	}

	CResponseDeviceInfoProtocol& operator=(const CResponseDeviceInfoProtocol& Protocol)
	{
		if (this == &Protocol)
		{
			return *this;
		}

		Clear();

		m_bResult   = Protocol.m_bResult;
		m_dwChannel = Protocol.m_dwChannel;

		memcpy(m_szDeviceID, Protocol.m_szDeviceID, min(sizeof(m_szDeviceID)-1,strlen(Protocol.m_szDeviceID)));
		memcpy(m_szManufacturer, Protocol.m_szManufacturer, min(sizeof(m_szManufacturer)-1,strlen(Protocol.m_szManufacturer)));
		memcpy(m_szModel, Protocol.m_szModel, min(sizeof(m_szModel)-1,strlen(Protocol.m_szModel)));
		memcpy(m_szFirmware, Protocol.m_szFirmware, min(sizeof(m_szFirmware)-1,strlen(Protocol.m_szFirmware)));
		
		return *this;
	}

public:
	char m_szDeviceID[32];				//<!-- Ŀ���豸/����/ϵͳ�ı��루��ѡ�� -->
	bool m_bResult;						//<!-- ��ѯ�������ѡ�� -->
	char m_szManufacturer[64];			//<!-- �豸�����̣���ѡ�� -->
	char m_szModel[64];					//<!-- �豸�ͺţ���ѡ�� -->
	char m_szFirmware[64];				//<!-- �豸�̼��汾����ѡ�� -->
	DWORD m_dwChannel;					//<!-- ��Ƶ����ͨ��������ѡ�� -->

	char	MaxCamera[8];        //���֧���������Ŀ
	char	MaxALarm[8];         //���澯·��
	char 	DeviceType[16];        //�豸���� dvr
	char	BuildTime[32];              //�汾����ʱ��
	char			deviceName [32];       //�豸����
	//list m_listInfo					//<!-- ��չ��Ϣ���ɶ��� -->
};

//�豸״̬��Ϣ��ѯӦ��
class CDeviceStatusItem
{
public:
	CDeviceStatusItem()
	{
		memset(m_szDeviceID,0,sizeof(m_szDeviceID));
		memset(m_szStatus,0,sizeof(m_szStatus));
		m_lDevicetype = 0;
	};

	/*virtual*/ ~CDeviceStatusItem(){};

	CDeviceStatusItem(const CDeviceStatusItem& Protocol)
	{
		memset(m_szDeviceID,0,sizeof(m_szDeviceID));
		memset(m_szStatus,0,sizeof(m_szStatus));

		memcpy(m_szDeviceID, Protocol.m_szDeviceID, min(sizeof(m_szDeviceID)-1,strlen(Protocol.m_szDeviceID)));
		memcpy(m_szStatus, Protocol.m_szStatus, min(sizeof(m_szStatus)-1,strlen(Protocol.m_szStatus)));

	}

	CDeviceStatusItem& operator=(const CDeviceStatusItem& Protocol)
	{
		if (this == &Protocol)
		{
			return *this;
		}

		memset(m_szDeviceID,0,sizeof(m_szDeviceID));
		memset(m_szStatus,0,sizeof(m_szStatus));

		memcpy(m_szDeviceID, Protocol.m_szDeviceID, min(sizeof(m_szDeviceID)-1,strlen(Protocol.m_szDeviceID)));
		memcpy(m_szStatus, Protocol.m_szStatus, min(sizeof(m_szStatus)-1,strlen(Protocol.m_szStatus)));

		return *this;
	}

public:
	char m_szDeviceID[32];				//<!-- �����豸���루��ѡ�� -->
	char m_szStatus[10];				//<!-- �����豸״̬����ѡ�� -->
	int  m_lDevicetype;                 //<-- 1����Ƶͨ��  2�������豸
};
class CResponseDeviceStatusProtocol : public CBasicProtocol
{
public:
	CResponseDeviceStatusProtocol()
	{
		memset(m_szDeviceID,0,sizeof(m_szDeviceID));
		memset(m_szReason,0,sizeof(m_szReason));
		memset(m_szDeviceTime,0,sizeof(m_szDeviceTime));

		m_bResult =false;
		m_bOnline = false;
		m_bEncode =false;
		m_bStatus =false;
		m_bRecord =false;
		
		m_deqItem.clear();
		m_ProtocolType = PROTOCOL_RESPONSE_DEVICESTATUS;
	};

	/*virtual*/ ~CResponseDeviceStatusProtocol(){};

	void Clear()
	{
		m_deqItem.clear();
	
		memset(m_szDeviceID,0,sizeof(m_szDeviceID));
		memset(m_szReason,0,sizeof(m_szReason));
		memset(m_szDeviceTime,0,sizeof(m_szDeviceTime));

		m_bResult =false;
		m_bOnline = false;
		m_bEncode =false;
		m_bStatus =false;
		m_bRecord =false;

		CBasicProtocol::Clear(); 
		m_ProtocolType = PROTOCOL_RESPONSE_DEVICESTATUS;
	};

	void Delete()
	{
		deque<CDeviceStatusItem*>::iterator it = m_deqItem.begin();
		for (; it != m_deqItem.end(); it++)
		{
			CDeviceStatusItem* pItem = *it;
			if (pItem)
			{
				delete pItem;
				pItem = NULL;
			}
		}
		m_deqItem.clear();
	};

	CResponseDeviceStatusProtocol(const CResponseDeviceStatusProtocol& Protocol)
	{
		Clear();

		m_bResult = Protocol.m_bResult;
		m_bOnline = Protocol.m_bOnline;
		m_bStatus = Protocol.m_bStatus;
		m_bEncode = Protocol.m_bEncode;
		m_bRecord = Protocol.m_bRecord;
		m_deqItem = Protocol.m_deqItem;
		m_StatusType = Protocol.m_StatusType;

		memcpy(m_szDeviceID, Protocol.m_szDeviceID, min(sizeof(m_szDeviceID)-1,strlen(Protocol.m_szDeviceID)));
		memcpy(m_szReason, Protocol.m_szReason, min(sizeof(m_szReason)-1,strlen(Protocol.m_szReason)));
		memcpy(m_szDeviceTime, Protocol.m_szDeviceTime, min(sizeof(m_szDeviceTime)-1,strlen(Protocol.m_szDeviceTime)));
	}

	CResponseDeviceStatusProtocol& operator=(const CResponseDeviceStatusProtocol& Protocol)
	{
		if (this == &Protocol)
		{
			return *this;
		}

		Clear();

		m_bResult = Protocol.m_bResult;
		m_bOnline = Protocol.m_bOnline;
		m_bStatus = Protocol.m_bStatus;
		m_bEncode = Protocol.m_bEncode;
		m_bRecord = Protocol.m_bRecord;
		m_deqItem = Protocol.m_deqItem;
		m_StatusType = Protocol.m_StatusType;

		memcpy(m_szDeviceID, Protocol.m_szDeviceID, min(sizeof(m_szDeviceID)-1,strlen(Protocol.m_szDeviceID)));
		memcpy(m_szReason, Protocol.m_szReason, min(sizeof(m_szReason)-1,strlen(Protocol.m_szReason)));
		memcpy(m_szDeviceTime, Protocol.m_szDeviceTime, min(sizeof(m_szDeviceTime)-1,strlen(Protocol.m_szDeviceTime)));
		return *this;
	}

	enum STATUS_TYPE
	{
		STATUS_TYPE_Encode = 0x01,
		STATUS_TYPE_Record = 0x02
	};

public:
	char m_szDeviceID[32];				//<!-- Ŀ���豸/����/ϵͳ�ı��루��ѡ�� -->
	bool m_bResult;						//<!-- ��ѯ�����־����ѡ�� -->
	bool m_bOnline;						//<!-- �Ƿ����ߣ���ѡ�� -->
	bool m_bStatus;						//<!-- �Ƿ�������������ѡ�� -->
	char m_szReason[512];				//<!-- ����������ԭ�򣨿�ѡ�� -->
	bool m_bEncode;						//<!-- �Ƿ���루��ѡ�� -->
	bool m_bRecord;						//<!-- �Ƿ�¼�񣨿�ѡ�� -->
	char m_szDeviceTime[64];			//<!-- �豸ʱ������ڣ���ѡ�� -->
	deque<CDeviceStatusItem*> m_deqItem;//<!-- �����豸״̬�б�,num��ʾ�б����������ѡ�� -->
	//list m_listInfo					//<!-- ��չ��Ϣ���ɶ��� -->

	STATUS_TYPE m_StatusType;
};

//�ļ�Ŀ¼����Ӧ��
class CRecordInfoItem
{
public:
	CRecordInfoItem()
	{
		Clear();
	};
	/*virtual*/ ~CRecordInfoItem(){};
	
	CRecordInfoItem(const CRecordInfoItem& Protocol)
	{
		Clear();

		m_bSecrecy = Protocol.m_bSecrecy;

		memcpy(m_szDeviceID, Protocol.m_szDeviceID, min(sizeof(m_szDeviceID)-1,strlen(Protocol.m_szDeviceID)));
		memcpy(m_szName, Protocol.m_szName, min(sizeof(m_szName)-1,strlen(Protocol.m_szName)));
		memcpy(m_szFilePath, Protocol.m_szFilePath, min(sizeof(m_szFilePath)-1,strlen(Protocol.m_szFilePath)));
		memcpy(m_szAddress, Protocol.m_szAddress, min(sizeof(m_szAddress)-1,strlen(Protocol.m_szAddress)));
		memcpy(m_szStartTime, Protocol.m_szStartTime, min(sizeof(m_szStartTime)-1,strlen(Protocol.m_szStartTime)));
		memcpy(m_szEndTime, Protocol.m_szEndTime, min(sizeof(m_szEndTime)-1,strlen(Protocol.m_szEndTime)));
		memcpy(m_szType, Protocol.m_szType, min(sizeof(m_szType)-1,strlen(Protocol.m_szType)));
		memcpy(m_szRecorderID, Protocol.m_szRecorderID, min(sizeof(m_szRecorderID)-1,strlen(Protocol.m_szRecorderID)));
		
	}

	void Clear()
	{
		memset(m_szDeviceID,0,sizeof(m_szDeviceID));
		memset(m_szName,0,sizeof(m_szName));
		memset(m_szFilePath,0,sizeof(m_szFilePath));
		memset(m_szAddress,0,sizeof(m_szAddress));
		memset(m_szStartTime,0,sizeof(m_szStartTime));
		memset(m_szEndTime,0,sizeof(m_szEndTime));
		memset(m_szType,0,sizeof(m_szType));
		memset(m_szRecorderID,0,sizeof(m_szRecorderID));

		m_bSecrecy =false;

	}

	CRecordInfoItem& operator=(const CRecordInfoItem& Protocol)
	{
		if (this == &Protocol)
		{
			return *this;
		}

		Clear();

		m_bSecrecy = Protocol.m_bSecrecy;

		memcpy(m_szDeviceID, Protocol.m_szDeviceID, min(sizeof(m_szDeviceID)-1,strlen(Protocol.m_szDeviceID)));
		memcpy(m_szName, Protocol.m_szName, min(sizeof(m_szName)-1,strlen(Protocol.m_szName)));
		memcpy(m_szFilePath, Protocol.m_szFilePath, min(sizeof(m_szFilePath)-1,strlen(Protocol.m_szFilePath)));
		memcpy(m_szAddress, Protocol.m_szAddress, min(sizeof(m_szAddress)-1,strlen(Protocol.m_szAddress)));
		memcpy(m_szStartTime, Protocol.m_szStartTime, min(sizeof(m_szStartTime)-1,strlen(Protocol.m_szStartTime)));
		memcpy(m_szEndTime, Protocol.m_szEndTime, min(sizeof(m_szEndTime)-1,strlen(Protocol.m_szEndTime)));
		memcpy(m_szType, Protocol.m_szType, min(sizeof(m_szType)-1,strlen(Protocol.m_szType)));
		memcpy(m_szRecorderID, Protocol.m_szRecorderID, min(sizeof(m_szRecorderID)-1,strlen(Protocol.m_szRecorderID)));

		return *this;
	}

public:
	char m_szDeviceID[32];				//<!-- �豸/������루��ѡ�� -->
	char m_szName[128];					//<!-- �豸/�������ƣ���ѡ�� -->
	char m_szFilePath[256];				//<!-- �ļ�·���� ����ѡ��-->
	char m_szAddress[256];				//<!-- ¼���ַ����ѡ�� -->
	char m_szStartTime[64];				//<!-- ¼��ʼʱ�䣨��ѡ��-->
	char m_szEndTime[64];				//<!-- ¼�����ʱ�䣨��ѡ��-->
	bool m_bSecrecy;					//<!-- �������ԣ���ѡ��ȱʡΪ0��0�������ܣ�1������-->
	char m_szType[10];					//<!-- ¼��������ͣ���ѡ��time��alarm��manual��all -->
	char m_szRecorderID[32];			//<!-- ¼�񴥷���ID����ѡ��-->
};
class CResponseRecordInfoProtocol : public CBasicProtocol
{
public:
	CResponseRecordInfoProtocol()
	{
		memset(m_szDeviceID,0,sizeof(m_szDeviceID));
		memset(m_szName,0,sizeof(m_szName));
		memset(m_szSumNum,0,sizeof(m_szSumNum));
		m_deqItem.clear();

		m_ProtocolType = PROTOCOL_RESPONSE_RECORDINFO;
	};

	/*virtual*/ ~CResponseRecordInfoProtocol(){};

	void Clear()
	{
		m_deqItem.clear(); 
		memset(m_szDeviceID,0,sizeof(m_szDeviceID));
		memset(m_szName,0,sizeof(m_szName));
		memset(m_szSumNum,0,sizeof(m_szSumNum));
		
		CBasicProtocol::Clear(); 
		m_ProtocolType = PROTOCOL_RESPONSE_RECORDINFO;
	};

	void Delete()
	{
		deque<CRecordInfoItem*>::iterator it = m_deqItem.begin();
		for (; it != m_deqItem.end(); it++)
		{
			CRecordInfoItem* pItem = *it;
			if (pItem)
			{
				delete pItem;
				pItem = NULL;
			}
		}
		m_deqItem.clear();
	};

	CResponseRecordInfoProtocol(const CResponseRecordInfoProtocol& Protocol)
	{
		Clear();
		
		memcpy(m_szDeviceID, Protocol.m_szDeviceID, min(sizeof(m_szDeviceID)-1,strlen(Protocol.m_szDeviceID)));
		memcpy(m_szName, Protocol.m_szName, min(sizeof(m_szName)-1,strlen(Protocol.m_szName)));
		memcpy(m_szSumNum, Protocol.m_szSumNum, min(sizeof(m_szSumNum)-1,strlen(Protocol.m_szSumNum)));

		m_deqItem = Protocol.m_deqItem;
	}

	CResponseRecordInfoProtocol& operator=(const CResponseRecordInfoProtocol& Protocol)
	{
		if (this == &Protocol)
		{
			return *this;
		}

		Clear();
		memcpy(m_szDeviceID, Protocol.m_szDeviceID, min(sizeof(m_szDeviceID)-1,strlen(Protocol.m_szDeviceID)));
		memcpy(m_szName, Protocol.m_szName, min(sizeof(m_szName)-1,strlen(Protocol.m_szName)));
		memcpy(m_szSumNum, Protocol.m_szSumNum, min(sizeof(m_szSumNum)-1,strlen(Protocol.m_szSumNum)));
		m_deqItem = Protocol.m_deqItem;
		return *this;
	}

public:
	char m_szDeviceID[32];				//<!-- �豸/������루��ѡ�� -->
	char m_szName[128];					//<!-- �豸/�������ƣ���ѡ�� -->
	char m_szSumNum[10];				//<!-- ��ѯ�����������ѡ�� -->
	deque<CRecordInfoItem*> m_deqItem;	//<!-- �ļ�Ŀ¼���б�,Num��ʾĿ¼����� -->
	//list m_listInfo					//<!-- ��չ��Ϣ���ɶ��� -->
};