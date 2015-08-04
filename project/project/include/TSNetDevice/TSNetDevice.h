// ���� ifdef ���Ǵ���ʹ�� DLL �������򵥵�
// ��ı�׼�������� DLL �е������ļ��������������϶���� TSNETDEVICE_EXPORTS
// ���ű���ġ���ʹ�ô� DLL ��
// �κ�������Ŀ�ϲ�Ӧ����˷��š�������Դ�ļ��а������ļ����κ�������Ŀ���Ὣ
// TSNETDEVICE_API ������Ϊ�Ǵ� DLL ����ģ����� DLL ���ô˺궨���
// ������Ϊ�Ǳ������ġ�
#ifndef _TS_NETDEVICE_H_
#define _TS_NETDEVICE_H_

#include "NetDevicePlatform.h"
#include "SWL_MultiNetComm.h"

#ifdef __ENVIRONMENT_LINUX__
#include "lvfw.h"
#else
#include <vfw.h>
#endif

#ifdef TEST_DISCONNECT
const int MAX_KEEP_TIME = 4000;			//��������ʱ��(ms)
#else
const int MAX_KEEP_TIME = 20000;			//��������ʱ��(ms)
#endif

const long	SOCK_TIME_OUT = 10000;		//���ͽ��ճ�ʱʱ��(ms)
const long REPLY_DATA_TIME_OUT = 10000;		//����ظ����ݳ�ʱʱ��(ms)

const unsigned long SWL_HEADFLAG = mmioFOURCC('h', 'e', 'a', 'd');

const long			RECV_BUFFER_SIZE = 256*1024;

const long			INVALID_CMD_ID = -1;

typedef struct _reply_cmd_data
{
	long			requestCmdID;
	long			replyCmdID;
	unsigned char	*pData;
	long			dataLen;
}REPLY_CMD_DATA;

typedef struct _net_combined_info_ex
{
	unsigned	long		dwUsed;			//�Ƿ�����ʹ��	0 û���� 1 ʹ����
	unsigned	long		dwDataID;		//��������ݵ�ID������������ÿ����������ID��ͬ
	unsigned	long		dwTotalPack;	//һ���м�����
	unsigned	long		dwTotalLen;		//���е���Ч���ݵ��ܳ���
	unsigned	long		dwLen;			//�Ѿ�����/�����˶��ٳ���
	unsigned	char		*pBuffer;		//ֻ�ڽ��յ�ʱ������
	long					bufferLen;

	unsigned long		streamId;
}NET_COMBINED_INFO_EX;

//��ΪSWL_COMBINATION_DATA����ʱ����ṹ��
typedef struct swl_combined_data
{
	unsigned	long	dwDataID;		//��������ݵ�ID������������ÿ����������ID��ͬ
	unsigned	long	dwTotalPack;	//һ���м�����
	unsigned	long	dwTotalLen;		//���е���Ч���ݵ��ܳ���
	unsigned	long	dwIndex;		//�ڼ�������������1��ʼ
	unsigned	long	dwLen;			//��������
	unsigned	long	dwPadding;		//Ϊ������һ�汾�����紫��ģ�鸽��
}NET_COMBINED_DATA;

typedef struct _comm_pack_head_
{
	char		headFlag[4];
	long		dataLen;
}COMM_PACK_HEAD;

enum RECV_DATA_TYPE
{
	DATA_TYPE_TRANS			= 0x0,	//͸������
	DATA_TYPE_PACK_INFO		= 0x1
};

// �����Ǵ� TSNetDevice.dll ������

struct _ts_netdevice_param;
class CTSCfgToIpc;
class CTSNdParam;

class CTSNetDevice : public CNetDevice
{
	friend class CTSCfgToIpc;
	friend class CTSNdParam;
public:
	CTSNetDevice();
	virtual ~CTSNetDevice();

	virtual bool Initial(long deviceId, long chnn, char *pServer, unsigned short netPort, unsigned short httpPort, \
		unsigned short ctrlPort, const std::string & user, const std::string & password, const ND_MAC & mac, const char *pEther="eth0", long sensorNum=0);
	//����ʼ���豸���ͷ���Դ���ǳ���
	virtual void Quit();

	virtual bool IfNeedInitNetConfig();

	virtual void SetDataBufferMan(CIPDataBufferMan *pDataBufferMan);

	/***********************************************************************
	�޸��豸�󶨵�ͨ����
	************************************************************************/
	virtual void SetBindChannel(long chnn);
	virtual long GetBindChannel();
	virtual void SetLocalVideoNum(long localNum);
	virtual unsigned int GetLocalVideoNum(){ return m_localVideoNum;};

	/***********************************************************************
	�õ��������豸��״̬,�Ͽ�������
	************************************************************************/
	virtual long GetDevStatus();


	/***********************************************************************
	�鿴�����豸�Ƿ���������
	************************************************************************/
	virtual bool IsReConnectting();
	
	
	/***********************************************************************
	�޸��豸�󶨵Ĵ�����ͨ����
	************************************************************************/
	virtual void SetBindSensorChannel(long sensorChnn);
	virtual long GetBindSensorChannel();

	/*
	�õ���ǰ���ӿ���״̬
	*/
	virtual long IsReady();

	/************************************************************************
	�ӿڹ���������������Ƶ����Ƶ��������
	�������������
	1��video1��������MASK��
	2��video2��������MASK��
	3��video3���ֻ�����MASK��
	4��audio����Ƶ��MASK��
	����ֵ���������سɹ�����ʧ�ܡ�
	ע����Ӧ��λΪ�㣬��ʾֹͣ��
	************************************************************************/
	virtual bool RequestStream(ULONGLONG video1, ULONGLONG video2, ULONGLONG video3, ULONGLONG audio);


	virtual bool RequestJpegStream(ULONGLONG channlMask);
	/************************************************************************
	�ӿڹ���������������Ƶ�ؼ�֡
	�������������
	1��video1��������MASK��
	2��video2��������MASK��
	3��video3���ֻ�����MASK��
	����ֵ���������سɹ�����ʧ�ܡ�
	ע����Ӧ��λΪ�㣬��ʾֹͣ��
	************************************************************************/
	virtual bool RequestKeyFrame(ULONGLONG video1, ULONGLONG video2, ULONGLONG video3);

	/************************************************************************
	�ӿڹ�����������̨���ƽӿڡ�
	************************************************************************/
	virtual bool PTZControl(long chnn, unsigned long cmdtype, unsigned long ptzParam, unsigned long speed, unsigned long waiteTime);

	/************************************************************************
	�ӿڹ�����������ȡͼ��ɫ����Ϣ��
	�������������
	1��ctrlInfo��������Ϣ�������PTZ_CTRL�ṹ�嶨�塣
	����ֵ���������سɹ�����ʧ�ܡ�
	************************************************************************/
	virtual bool ColorControl(long chnn, unsigned long cmdtype, DD_VIDEO_COLOR& videoColor);

	/*
    function:
		���񣬷�ת������ģʽ������У�����Զ��Խ���ROI(����Ȥ��)
    input:
		pchar ���͵�����
	return:
	    �ɹ�true, ʧ�� false 
	*/

	virtual bool OtherControl(long chnn, unsigned long cmdtype, unsigned char *pChar);

	/************************************************************************
	�ӿڹ����������豸�ı�����Ϣ��
	�������������
	����ֵ�������ޡ�
	************************************************************************/
	virtual ULONGLONG GetMotionAlarm();
	virtual ULONGLONG GetSensorAlarm();
	virtual ULONGLONG GetVlossAlarm();
	virtual ULONGLONG GetOtherAlarm();

	//
	virtual long SetConfig(long chnn, unsigned long itemID, const unsigned char* pData, unsigned long dataLen, bool bImmediately = true);
	virtual long FlushConfig();

	/************************************************************************
	����ʱ��
	************************************************************************/
	virtual long SetDateTime(const DD_TIME& ddTm);

	/************************************************************************
	��ȡ�豸֧�ֲ�����Ϣ, videoIndexȡֵ0��1��2��ʾ�ĸ�����
	************************************************************************/
	virtual long GetDevInfo(long videoIndex, ND_DEV_INFO& ndDevInfo);
	virtual long GetEncodeInfo(char * pVideoEncodeInfo, int iVideoEncodeInfoLen);

	virtual bool GetLastError(std::string& strErr){return true;};
	virtual long GetLastError() {return 0;};
	virtual void SetLastError(long err){};

	virtual void OnTimer(unsigned long curTime);

	/***********************************************************************
	�������糬ʱʱ��,��λms
	***********************************************************************/
	virtual void SetNetTimeout(unsigned long msTimeout);
	virtual unsigned long GetDevResolutionVal( int* width, int* height );

	//��ʱͬ��IPC��ʱ��
	virtual int ResyncIPCSystemTime(int sync_flag);

protected:

protected:
	bool InitParam();
	void QuitParam();
	//���ݴ���ɹ�����0�����򷵻�-1����ʱ���������״̬
	static int ReceiveCallback(long clientID, void* pParam, RECV_DATA_BUFFER *pDataBuffer);
	int ProcessData(RECV_DATA_BUFFER *pDataBuffer);
	void ProcRecvCmd(unsigned long cmdType, unsigned char *pData, unsigned long datalen);
	
	bool ProcessFrame(const unsigned char *pBuf, int iLen);
	bool CombineFrame(const unsigned char *pBuf, int iLen);
	bool ProcessWaitFrame();

	bool WaitReplyData();

	bool PostCmd(unsigned long iCmdType, unsigned char* pData, unsigned long dataLen);
	bool SendCmd(unsigned long iCmdType, unsigned char* pData, unsigned long dataLen);

	void EncryptLoginInfo(void *pLoginInfo);

	bool SetInitConfig();

	unsigned long DDPtzToIpcPTZ(unsigned long ddPtzCmd);

	bool WaitForReady();

protected:
	_ts_netdevice_param*		m_pTSNetDeviceParam;

	CTSCfgToIpc*				m_pTsCfgToIpc;
	CTSNdParam*					m_pTsNdParam;

	int                         m_iIpcUsedChipFlag;

	int                         m_iAudioFrameMaxCnt;

	int                         m_localVideoNum;
};

#endif