#pragma once

#include <list>
#include "PUB_common.h"

const unsigned long	MAX_NET_DEVICE_NUM = 128;
const unsigned long NETDEVICE_LOG_HEAD_FLAG = mmioFOURCC('N', 'D', 'L', 'H');

typedef struct _net_device_link_info
{
	unsigned long	deviceID;	//�����豸ID����1��ʼ

	unsigned char	enabled;	//��ǰ�����豸�Ƿ���ã�������������豸��
	unsigned char	channel;	//�������豸�ڱ��ط����ͨ��
	
	char			szEtherName[16];		//���Ϊ�գ�Ĭ��Ϊeth0
	char			szServer[64];	//�����豸IP��ַ
	unsigned short	nPort;		//�����豸�˿ں�
	unsigned short	nHttpPort;	//HTTP�˿�
	unsigned short	nCtrlPort;	//���ƶ˿ڣ�һ����nPort��ͬ
	char			szID[64];	//�����豸��ʶ������MAC��ַ��
	char			username[36];//ע�ᵽ�����豸��ʱ���õ��û���
	char			password[36];//ע�ᵽ�����豸��ʱ���õ�����

	unsigned long	manufacturerId;	//�豸����
	char 			manufacturerName[36];
	char			productModel[36];//��Ʒ�ͺ�

	unsigned char	bUseDefaultCfg;
	unsigned char	resv[3];
}NET_DEVICE_LINK_INFO;

typedef struct _netdevice_log_header
{
	unsigned long		headFlag;	//
	unsigned long		deviceTotalNum;	//�ļ���¼�������ɵ��豸�ܸ���
	unsigned long		deviceUsedNum;	//�ļ���¼��ʹ�õļ�¼����
	unsigned long		itemLen;	//ÿһ���豸��ĳ���
}NETDEVICE_LOG_HEADER;

typedef struct _nedevice_ext_func_flag
{
	unsigned long      functins[4];
	unsigned long      flag_poeIsPtoP;
	unsigned long      resv[11];
}NEDEVICE_EXT_FUNC_FLAG;

class CRegNetDeviceLog
{
public:
	static CRegNetDeviceLog *Instance();
	
	bool Initial(const char *pLogFilePath);
	void Quit();
	inline long GetClientID(){return m_clientID;}
	void EnterIPCamConfig(unsigned long clientID = LOCAL_DEVICE_ID);
	void ExitIPCamConfig();

	//��ȡ����POEʹ�õ�ģʽ
	bool GetPoeUsedPtoPMode(){return m_bPoeUsedPtoPFunc;}
	void SetPoeUsedPtoPMode( bool bPoeUsedModePtoP );
	
	/*********************************************************************************
	�õ����ɽ��ܵĽ����豸��
	*********************************************************************************/
	unsigned long GetMaxDeviceCount() const;
	
	/*********************************************************************************
	���ý���������豸����
	**********************************************************************************/
	bool SetNetDevice(NET_DEVICE_LINK_INFO *pNetDevice, unsigned long num);
	
	/*********************************************************************************
	�����������������������豸��һ������
	**********************************************************************************/
	unsigned long GetNetDeviceCount() const;
	unsigned long GetNetDevice(NET_DEVICE_LINK_INFO *pNetDevice, unsigned long num);
	
	/*********************************************************************************
	���ص������豸���б�
	**********************************************************************************/
	void GetChBindNetDevice(std::list<NET_DEVICE_LINK_INFO> &list);
	
	///*********************************************************************************
	//Ϊָ���������豸����ͨ����ֻ�з��䵽����ͨ�����豸�Żᱻ����
	//**********************************************************************************/
	//bool AllocChannelForDevice(unsigned short deviceID, unsigned char channel);	
	
private:
	CRegNetDeviceLog(void);
	~CRegNetDeviceLog(void);
	
	bool AddOneDevice(NET_DEVICE_LINK_INFO &netDevice);

private:
	char						*m_pFileLogName;
	NET_DEVICE_LINK_INFO				*m_pNetDeviceInfo;
	NETDEVICE_LOG_HEADER		m_logHeader;
	CPUB_Lock					m_fileLock;
	long						m_clientID;
	std::list<long>				m_IDList;

	//�Ƿ�ʹ��һ��һģʽ
	bool                        m_bPoeUsedPtoPFunc;
};
