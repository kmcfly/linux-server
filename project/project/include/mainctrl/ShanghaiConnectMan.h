/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : zxx
** Date         : 2012-11-22
** Name         : ShanghaiConnectMan.h
** Version      : 1.0
** Description  :
** Modify Record:
***********************************************************************/

#ifndef _SHANGHAI_CONNECTMAN_H_
#define _SHANGHAI_CONNECTMAN_H_

#include "PUB_common.h"
#include "SWL_Public.h"
#include "SWL_ListenProcEx.h"
#include "tinyxml.h" 
#include "DiskManager.h"
#include "ReclogManEx.h"
#include "ShanghaiCmd.h"
#include "NetProtocol.h"
#include "LogMan.h"
#include <list>
#include <string>
#include <map>
using namespace std;

const int  recvSendBufLen = 1024;
const char pShanghaiXmltitle[] = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\r\n";
const unsigned long maxShanghaiDataSize = 2048;  
const unsigned long maxCmdCount = 100;//��ౣ��100��ָ�� 
#pragma pack(1)
typedef struct msgHead
{
	char heart[6];//QWCMD
	unsigned long headlen;//��Ϣ���ȣ���ͷ

	char resver[10];//�����ֽ�
}MSG_HEAD;
#pragma pack()

typedef struct _delay_dailyinfo_
{
	unsigned long		operType;//��Ϣ�Ĳ�������
	unsigned long		chnn;	//��Ϣͨ��
	char			    triggerTime[20];//����ʱ��
}DELAY_DAILYINFO;

typedef struct _heart_info_
{
	ULONGLONG TotalSpace;//���д��̼��������������ͣ���GΪ��λ
	ULONGLONG FreeSpace;//���д���ʣ���������ܺͣ���GΪ��λ
}HEARTINFO;

typedef enum net_err
{
	ERR_SUCC,
	ERR_OPERTYPE,
	ERR_PICTOOBIG,
	ERR_NETERR,
	ERR_PTENABLE,//���ò���ƽ̨�ر�
}NET_ERRINFO;

class CShanghaiConnectMan
{
public:
	static CShanghaiConnectMan *Instance();
	~CShanghaiConnectMan();

	void Initial();
	void Quit();

	bool Start();
	void Stop();

	void SetPTEnabel(bool benable);//����ƽ̨
	bool GetPTEnable();
	void SetMotionEnable(bool benable);//�ƶ��������
	void SetStartHeartProc();//������������

	void SetAgentId(const char *pAgentId);//���ô���id
	void Setport(unsigned long port);//���÷������˿�
	void SetIp(const char *pIp);//���ô���ip

	void GetHeartInfo(CReclogManEX	*pReclogMan);//ÿ��������ȥ��ȡ������Ϣ��dvr��ʶ
	//��ȡ��־�Ĳ���,���������б�����
	void GetLogCmd(unsigned long type,unsigned long chnn,char *ptext,int textLen,void *pPicBuf,int picBufLen,ULONGLONG time);
	
	void GetMotionAlarmSnap(OPER_INFO snapInfo);//��ȡ�ƶ���ⱨ����ͼ
	void GetSensorAlarmSnap(OPER_INFO snapInfo);//��ȡ�ⲿ����������ͼ

	//Base64�㷨
	int to64frombits(unsigned char *out, const unsigned char *in, int inlen);

	void SetMotionHead(bool bhead);//����ƶ�����ͷ
	void SetSensorHead(bool bhead);//����ⲿ������ͷ
	bool GetMotionHead();
	bool GetSensorHead();

	void SetMotionAlarmStartTime(unsigned long time);
	void SetSensorAlarmStartTime(unsigned long time);

	LONGLONG GetTriggerTime();
        LONGLONG GetChannelBits();
	
protected:
private:
	CShanghaiConnectMan();
	/****************************�̲߳���*****************************************/
	//���̸߳�����������Ϣ��ͼƬ�ķ���
	static PUB_THREAD_RESULT PUB_THREAD_CALL HeartThread(void *pParam);
	void HeartProc();

	/****************************�̲߳���*****************************************/

	/*********************************xml��������*******************************/
	//����Agent
	TiXmlElement * CreateAgent(TiXmlDocument *document,char *pdvrType);
	//������ʱ����heart
	void CreateShanghaiHeartXml(TiXmlElement *pElement,char *pdvrType,HEARTINFO hear_info);
	//����opercmd
	TiXmlElement * AddOperXml(TiXmlElement *pElement,OPER_INFO operInfo);
	/*********************************xml��������*******************************/

	/***********************************socket����******************************/
	bool Connectheart();//��������������
	
	//������ȡ���յ���xml�ķ�����ʱ����´ν���������ʱ����
	bool GetHeartRecvTimeAndLeap(char *pSrc,int &TimeValue);
	
	void ProcessGetImageXml(char *xmlbuf);

	//���ͺͽ���socket��Ϣ
	bool SendAndRecvHeart(char *pXmlbuf,int &recvTimeLeap,bool bheart);
	/***********************************socket����******************************/

	/****************************************��غ���******************************/
	bool GetDYRSysErrExitLog();//�����ϴιػ��Ƿ��쳣���쳣Ϊtrue,����Ϊfalse
	
	void gettmreciveTime(tm &serverTm);//��ȡ���յ���ʱ��
	void ChangeSystime();//�ı�ϵͳʱ��
	void GetOperCmd(OPER_INFO Opercmd);//��ò�������Ϣ����
	/****************************************��غ���******************************/

	/*****************************************�̲߳�������**************************/
	int HeartOnlyFunction(int &recvTimeLeap);//��������
	int CmdOnlyFunction(int type);//����ָ���ͼƬ

	void CMDMotionAlarm();//�ƶ���ⱨ������
	void CMDSensorAlarm();//�ⲿ������������
	/*****************************************�̲߳�������**************************/
private:
	PUB_thread_t		m_heartID;
	bool				m_bheartProc;
	SWL_socket_t		m_heartsock;//ֻ��һ���׽ӿ�
	
	PUB_thread_t		m_cmdID;
	bool				m_bCmdProc;

	std::list<OPER_INFO> m_listCmd;				//��������б���ֶ��Լ��ճ�ͼƬ
	std::list<OPER_INFO> m_listMotionPic;		//�ƶ����ͼƬ�б�
	std::list<OPER_INFO> m_listsensorPic;		//�ⲿ��������������ͼƬ�б�
		
	HEARTINFO m_hear_info;//
	
	char m_recvTime[256];
	
	CReclogManEX	*m_pReclogMan;//�ô���ָ���Ǵ�MainFrame��ȡ������Ŀ����Ϊ�˻�ȡ����ʱ������Ҫ��ʣ���������
	
	int m_DVrStart;		   //��־�����������
	int m_recvTimeLeap;//�������յ���ʱ����
	unsigned long m_heartTime;//����������ʱ��

	//MOtion�б��ͷ��Ϣ������xmlʱ����Ҫͷ+ͼƬ+ͼƬ������ͼƬ������ϲ��ܹرն�Ӧ�׽ӿ�
	bool	  m_bmotionHead;//�Ƿ����б�ͷ

	//SENSOR�б��ͷ��Ϣ,����xmlʱ����Ҫͷ+ͼƬ+ͼƬ������ͼƬ������ϲ��ܹرն�Ӧ�׽ӿ�
	bool	  m_bsensorHead;
	
	enum cmd_type
	{
		CMD_OPER=0,
		CMD_MOTION,
		CMD_SENSOR,
	};


	bool m_benable;//ƽ̨�Ƿ�����
	bool m_bmotionEnable;//�ƶ���ⱨ���ϴ��Ƿ�����
	bool m_bstartHeart;//�Ƿ�����������

	char m_agentId[13];	//����id
	unsigned long m_port;//����˿�
	char m_ip[20];//����ip
	
	CPUB_Lock m_PTLock;//ͣ�õ�ʱ��Ҫɾ���б��ڵ��������ݣ�����ͬ��ͣ�úͷ���ʱ����б�
	unsigned long m_sensorpicCount;//���ʹ�����ͼƬ����
	unsigned long m_motionpicCount;//�����ƶ����ͼƬ����
	unsigned long m_sensorAlarmStartTime;//��������������ץͼ��ʼʱ��
	unsigned long m_motionAlarmStartTime;//�ƶ���ⱨ������ץͼ��ʼʱ��
	unsigned long m_maxpicdelayTime;//ͼƬ�����ӳ����ʱ��,��ǰʱ��-��������ʱ��>�����ӳ����ʱ����ֹͣ����

	LONGLONG m_triggerTime;
	LONGLONG m_chnnBits;
};


#endif

