/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : zhl
** Date         : 2010-07-22
** Name         : HxConnectMan.h
** Version      : 1.0
** Description  :
					��Ҫ��ɵ�����
					1���������ݽ��պͷ��͡�
					2��DVR����ά��
					3���յ�DVRЭ�̺�����Ƶ�����������Ƶ����
					
					DVRҪ�������׽ӿڽ�����Ϣ
					1������������ĻỰ�˿�6004
					2������������Ĵ洢��������˿�6005
					3��������������Ƶʱ�ͽ���������Ự�˿�Э�̺õ����������豸�˿ڣ�����HXHT�ĸ��ַ��������ͻ��ˡ�������IP�Ͷ˿�ͨ��Э�̾����ģ���Ҫ��ʱ�����׽ӿ��ٴ�������,
					   �����׽ӿ���Ҫ��������ƵЭ�̹�����DVR������Ƶ������Կ(ticket) ��ȷ�ϡ���Э�̺����ݴ������׽ӿھ�ֻ��������Ƶ���书�ܣ�
					   ��ʱ��Ӧ���ڸ����й���Ҫ��������Ƶ����ģ��������
					4:��������ļ����˿ڣ�������Ƶ���Ӷ˿ڡ��������Ӷ˿ڡ������˿ڡ�WEB���ʶ˿�

				   ���ݴ���ķ�ʽΪ��
				    1����ÿ���׽ӿڴ���һ��SEND_DATA_BUF�����Ҫ���͵����׽ӿڵ����ݣ�ÿ����SEND_DATA_BUF�д��һ��HXHT��������ݡ�
				    2�������ݴ���Ϣ����ģ�鷢�͵�����ʱ���Ȳ鿴���׽ӿڵ�SEND_DATA_BUF�Ƿ�Ϊ�գ���Ϊ�վ͵ȴ�SEND_DATA_BUF�����ݷ�����Ϻ���ܰ����ݷŵ�SEND_DATA_BUF��
				    3������ĳ���׽ӿڣ����SEND_DATA_BUF�������ݾͷ��͵����磬SEND_DATA_BUF�е����ݿ��Զ�η���
					4����ʱ�������׽ӿڵ�SEND_DATA_BUF���Ա���ʱ���٣������׽ӿڲ�һ�����ر�
** Modify Record:

***********************************************************************/

#ifndef _DATA_COMMUNICATE_MAN_H_
#define _DATA_COMMUNICATE_MAN_H_

#include "PUB_common.h"
#include "HxGlobal.h"
#include "ReqDVRProcess.h"
#include "SWL_Public.h"
#include "SWL_ListenProcEx.h"
#include "PfHxDefine.h"
#include "HxAVDataSendMan.h"
#include "MessageMan.h"


const int REGISTER_WAIT_TIME	= 30;				//ע��ʱ�ȴ�30sû��Ӧ�����ע��
const int SOCKET_SRBUF_LEN		= 512 * 1024;		//
class CReqDVRProcess;
class CHxConnectMan
{
public:
	static CHxConnectMan *Instance();

	//��ʼ���ͽ���������ĻỰ�˿ں�����Ƶ�����˿�
	bool Initial(HX_NET_INIT_INFO hxNetInitInfo);	
	void Quit();

	bool Start();
	void Stop();

	//ɾ�����л����֡	
	void ClearAllFrameData();

	//��������HXHT����ģ��
	bool Restart(const HXHT_NET_PARAM_INFO *pNewParam, bool bLocalIPChanged);

	~CHxConnectMan();
protected:
private:
	CHxConnectMan();


	//���̸߳���������Ϣ
	static PUB_THREAD_RESULT PUB_THREAD_CALL HxConnectManThread(void *pParam);
	void HxConnectManProc();
	//���̸߳��𱻶���Ϣ
	static PUB_THREAD_RESULT PUB_THREAD_CALL NetServerThread(void *pParam);
	void NetServerProc();

	//�����׽ӿڣ��ڽ���������Ƶ����ʱ��Ҫ
	static int AcceptLinkCallback(CLIENT_SOCK_INFO *pClientSockInfo, void *pParam1, void *pParam2);
	bool InitMsgSockInfo(CLIENT_SOCK_INFO *pClientSockInfo);
	void QuitMsgSockInfo();	
	inline std::list<HX_CMD_DATA *>::iterator DelMsgBuf(SWL_socket_t sockfd);


	//������������������
	bool AddReqToCS(TVTHX_MSG_CMD hxCmdNum, SWL_socket_t clientSockfd, char *pParam, unsigned long paramLen);
	//��ȡһ������������ÿ�������������ģ�Ҳ��ÿ����Ϣ�������Ӧ����������
	bool DecReqToCS(HX_DVR_REQ_CMD_INFO &dvrReqCmdInfo);
	//���������Ϣ����	
	void ClearReqCmdList();
	bool SendRequest(TVTHX_MSG_CMD requestCmd, SWL_socket_t toSockFd, const char *pParam=NULL);		//����������ע��,����������״̬�����
	bool GetVideoHead(HX_CMD_DATA &hxCmdData);
	bool GetAVData(HX_CMD_DATA &sockCmdBuf);

	int SendOneCmdData(HX_CMD_DATA &hxCmdData);
	int SendDataNoblock(HX_CMD_DATA &hxCmdData);


	int RecvData(SWL_socket_t fd, char *pData, int dataLen);
	int BlockRecvData(SWL_socket_t fd, char *pData, int dataLen);
	int GetOneCmdData(HX_CMD_DATA &hxRepCmdData);
	int GetOneCmdDataNoblock(HX_CMD_DATA &hxRepCmdData);

	inline int SelectFd(fd_set * pFdSet, bool bRead);
	bool GetReplayFromNet(HX_CMD_DATA &replayCmdData,  const char *pParam=NULL);

	
	bool ChangeVideoStatus(HX_CMD_DATA &hxCmdData);
	SWL_socket_t ConnectHxNetDev(char *pIP, unsigned short port);

	void GetFrame();
	void GetPBFrame();
	void StartStreamOK(const START_STREAM_PARAM *pStartStreamParam);
	void StopStreamOK(const START_STREAM_PARAM *pStartStreamParam);

	//��ⱨ��״̬�Ƿ�ı�
	bool CheckAlarm(ALARM_INFO &alarmInfo);
	void SendAlarmStatus();

	void SendFaultStatus();
	bool CheckVLFault();
	bool CheckDiskFullFault();

	void GetRebootTime(const char *pRebootTime, int rebootCycle);
	void ChangeSysTime(const char *pCSTime);

	PUB_thread_t		m_HxConnectManID;
	volatile bool		m_bHxConnectManProc;

	PUB_thread_t		m_netServerID;
	bool				m_bNetServerProc;

	SWL_socket_t		m_storeSockfd;						//����������ע�������
	SWL_socket_t		m_registerSockfd;					//��洢������ע�������

	SWL_socket_t		m_maxMsgSockfd;						//��¼���Ż�ͨ�ͻ�������ʱ����������ļ�������
	unsigned long		m_sockCount;
	fd_set				m_msgSockSet;						
	std::list<SWL_socket_t> m_msgSockfdList;
	CPUB_Lock			m_msgSockSetLock;



	HX_NET_INIT_INFO	m_HxNetInitInfo;

	volatile int		m_haveRegisted;
	volatile int		m_heartCycle;
	volatile int		m_haveSRegisted;					//�洢ע��
	volatile int		m_SHeartCycle;						//�洢ע����������

	char				*m_pMessageID;
	unsigned long		m_lastKeepLiveTime;
	unsigned long		m_lastRegisteTime;
	unsigned long		m_lastSRegisteTime;

	CSWL_ListenProcEx	*m_pHxListenProc;
	CReqDVRProcess		*m_pReqDVRProcess;
	CMessageMan			*m_pMsgMan;

	CPUB_Lock			m_dvrReqCmdListLock;
	std::list<HX_DVR_REQ_CMD_INFO> m_dvrReqCmdList;

	CPUB_Lock			m_msgBufListLock;
	std::list<HX_CMD_DATA *> m_msgBufList;				//ÿ�����ӵ���Ϣ�˿ڵ�socket����һ�������������ա�������� �ڽ���һ���׽ӿڵ�����ʱ��������������
	
	
	TVTHX_STREAM_HEAD	*m_pStreamHead;						//�ֳ�����Ƶͷ��ÿ��ͨ��һ��ͷ��Ϣ�� �ڻ�ȡ����Ƶ��ʽ�����
	TVTHX_STREAM_HEAD	*m_pPBStreamHead;					//�ط�����Ƶͷ��ÿ��ͨ��һ��ͷ��Ϣ�� �ڻ�ȡ����Ƶ��ʽ�����	

	ULONGLONG			m_ullStatusMotion;
	ULONGLONG			m_ullStatusSensorIn;
	ULONGLONG			m_ullStatusVideoLoss;
	ULONGLONG			m_ullStatusAlarmOut;

	unsigned long		m_sysRebootTime;					//ϵͳ�������
	bool				m_bHaveSynTime;

	FAULT_INFO			*m_pVLFaultInfo;					//��Ƶ��ʧ֪ͨ��Ϣ
	FAULT_INFO			m_diskFaultInfo;					//Ӳ������֪ͨ��Ϣ

	bool				m_bHasStoreServer;					//
};
#endif /*_DATA_COMMUNICATE_MAN_H_*/
