
#ifndef _PTZ_MAN_H_
#define _PTZ_MAN_H_

#include "PUB_common.h"
#include "dvrdvsdef.h"
#include "PTZ.h"
#include <map>

#include "Product.h"
#ifdef __ENVIRONMENT_WIN32__
#include "PtzProtocolWin32.h"
#else
#include "PTZProtocol.h"
#include "FakeCurise.h"
#endif

//////////////////////////////////////////////////////////////////////////
// * [2/4/2010]
// * ��̨����
//////////////////////////////////////////////////////////////////////////
// * ���³���������Ҫ���ڵ������в�Ʒ֧�ֵ�������������
const unsigned long PTZ_MAX_PRESET_NUM = 128;
const unsigned long PTZ_MAX_CURISE_PT_NUM = 32;
const unsigned long PTZ_MAX_CURISE_NUM = 8;
//////////////////////////////////////////////////////////////////////////

class CPtzMan
{
public:
	~CPtzMan ();
	static CPtzMan* Instance();
	bool Initial();
	void Quit();

	enum PtzCmdState
	{
		PTZ_CMD_WAITING = 0,
		PTZ_CMD_SUCC,
		PTZ_CMD_FAIL,
	};

	struct PtzCmdPack
	{
		unsigned long channel;			//��������Ƶ�ͨ��
		unsigned long cmd_type;			//��������
		unsigned long speed;			//�ٶ�
		unsigned long wait_time;		//��̨������ɺ�,��̨��Ҫ�ȴ�ʱ��
		unsigned long param;			//�������
		unsigned long start_time;		//������ʱ���
		unsigned long end_time;			//������ɺ��ʱ���
		unsigned long process_state;	//������״̬:������,�����,����ʧ��
		unsigned char* ptr_param_ex;	//��չ�������,ĳЩ�ض�������Ҫ���ݶ�������
	};

public:
	//�������� [ͨ�����������͡�������������ʡ��ȴ�ʱ��] [���ش������]
	unsigned long Action(unsigned long channel, unsigned char cmdtype, unsigned long ptzParam, unsigned char speed = 5, unsigned char waiteTime = 0, const unsigned char* ptrParamEx = NULL);

	// * �������ö�ȡ���,����ʼ���������ļ�����Ϣ����һ��

	//��̨���ô�����Ϣ [ͨ�����Ƿ����á���ַ��Э�顢�����ʡ�����]
	void SetSerialInfo(unsigned long channel, unsigned char enable, unsigned char addr, unsigned char protocol, unsigned char rate, unsigned long speed = 5);

	//Ԥ�õ����� [ͨ����Ԥ�õ㡢Ԥ�õ����]
	void SetPresetConfig(unsigned long channel, const PTZ_PRESET* pPreset, unsigned long count);
	//��ȡԤ�õ����� [ͨ�������Ԥ�õ㡢���Ԥ�õ����] [���ش������]
	unsigned long GetPresetConfig(unsigned long channel, PTZ_PRESET** pOutPreset, unsigned long& outCount);
	bool PresetIsEnable( unsigned long chnn, unsigned int presetNum );
	//Ѳ�������� [ͨ����Ѳ���߱�š�Ѳ����ͷ��Ѳ���ߵ㡢Ѳ���ߵ����] 
	void SetCruiseConfig(unsigned long channel, unsigned long index, const PTZ_CURISE* pCurise, const PTZ_CURISE_POINT* pCurisePt, unsigned long count);
	//��ȡѲ�������� [ͨ����Ѳ���߱�š����Ѳ������Ϣ�����Ѳ���ߵ���Ϣ�����Ѳ�������] [���ش������]
	unsigned long GetCruiseConfig(unsigned long channel, unsigned long index, PTZ_CURISE** pOutCurise, PTZ_CURISE_POINT** pOutCurisePt, unsigned long& outCount);

	//��ȡĳͨ����̨�Ƿ�����
	bool GetPtzEnable(unsigned long channel);
	//��ȡĳͨ����̨״̬ [ͨ��] [����ͨ��״̬]
	unsigned long GetPtzState(unsigned long channel);
	//����ĳͨ����̨״̬ [ͨ����״̬]
	void SetPtzState(unsigned long channel, unsigned long state);

	//���á���ȡ����
	bool IsPtzLock(unsigned long channel);
	void SetPtzLock(unsigned long channel, bool lock);
	
	//����Ѳ����Ԥ����Ԥ�õ���Ϣ����̨
	bool SetCruisePreviewInfo( const unsigned char *pData );
	//�ָ�ͨ��chnnԤ����Ѳ����Ԥ�õ���Ϣ
	bool RecoverCruisePreviewInfo( unsigned long chnn );

	//��̨����
	void EnterPtz(unsigned long clientID = LOCAL_DEVICE_ID);
	void ExitPtz();
	unsigned long PtzClient();
	
	bool WantToConfigPTZ(unsigned char cmdtype);

protected:
	CPtzMan();

protected:
	//��������
	unsigned long ProcessAction(unsigned long channel, unsigned char cmdtype, unsigned long ptzParam, unsigned char speed = 5, unsigned char waiteTime = 0, unsigned char* ptrParamEx = NULL);

	//��������
	unsigned long ProcessCommand(PtzCmdPack& cmd);

	//���ĳ��ͨ��,�Ƿ�����ٴ�ִ������ [ͨ��] [��ǰʱ��]
	unsigned long CanChnnProcess(unsigned long chnn, unsigned long timetip);

	//���ĳ�����Ƿ���Ա�ִ��
	unsigned long CanProcess(const PtzCmdPack& cmd);

	//��ȡ����Ĵ����
	unsigned long GetCmdType(unsigned char cmdtype, unsigned long param, bool& bEnd);

	//ֹͣ��̨�ĵ�ǰ״̬
	bool ResetPtz(unsigned long chnn, unsigned long& curState);

protected:
	//��ȡĳЭ�鴦�������
	CPTZProtocol* GetPtzProtocol(unsigned long protocol,unsigned long chnn);
	
	//�ָ�һ��ͨ����Ԥ������Ѳ����
	bool RecovreOneChnnCurisePreview( unsigned long chnn );
	
	//�ͻ����쳣�˳�ʱ���PTZ����״̬
	void StopPTZConfig();

protected:
	// * �ڲ�ʹ�ýṹ�嶨��

	//��̨�������á�Э��������
	struct PtzSerialInfo
	{
		unsigned char enable;				//��ʾ�Ƿ�������̨����
		unsigned char addr;					//��̨�豸ID
		unsigned char protocol;				//Э��ID��0Ϊû��������̨
		unsigned char baud_rate;			//������0-13�ֱ��ʾ110��300����..921600�Ĳ�����
		unsigned long channel;				//ͨ��
		unsigned long speed;				//��̨Ĭ���ٶ�
		unsigned long last_cmd_type;		//���һ��ִ�е���������
		unsigned long last_process_time;	//���һ��ִ�������ʱ��
		unsigned long wait_time;			//���һ��ִ��������Ҫ�ȴ���ʱ��
	};

	struct PtzPreset
	{
		unsigned long channel;					//ͨ��
		unsigned long count;					//Ԥ�õ�ʹ�ø���,���PTZ_MAX_PRESET_NUM��
		PTZ_PRESET preset[PTZ_MAX_PRESET_NUM];	//Ԥ�õ�����
	};

	struct PtzOneCurise
	{
		unsigned long symbol;			//�Ǳ�Ѳ���߱�� : λ��ʾ[1�Ƿ������˴�Ѳ���ߡ�2�Ƿ����ñ�Ѳ���ߡ�3�Ƿ�Ϊ��ʵѲ����]
		unsigned long count;			//Ѳ����ʹ�ø���,���PTZ_MAX_CURISE_PT_NUM��
		PTZ_CURISE header;				//Ѳ����ͷ��Ϣ
		PTZ_CURISE_POINT curise_pt[PTZ_MAX_CURISE_PT_NUM]; //Ѳ���ߵ���Ϣ
	};

	struct PtzCurise
	{
		unsigned long channel;					//ͨ��
		unsigned long count;					//ͨ��Ѳ����ʹ������,���PTZ_MAX_CURISE_NUM��
		PtzOneCurise curise[PTZ_MAX_CURISE_NUM];//ÿ��Ѳ������Ϣ
	};

	typedef std::map<unsigned long, PtzSerialInfo>		PtzSerialInfoMap;	//[ͨ��,����������Ϣ]
	typedef std::map<unsigned long, PtzPreset*>			PtzPresetMap;		//[ͨ��,Ԥ�õ�������Ϣ]
	typedef std::map<unsigned long, PtzCurise*>			PtzCuriseMap;		//[ͨ��,Ѳ����������Ϣ]
	typedef std::map<unsigned long, CPTZProtocol*>		PtzProtocolMap;		//[Э����,Э�������]

protected:

	bool						m_bInit;						//�Ƿ��ʼ������

	std::map<unsigned long, unsigned long> m_ptzChnnState;		//�������ͨ�������ڵ�״̬ [ͨ��,״̬���]
	PtzSerialInfoMap			m_cfgSerialInfoMap;				//�������ͨ���Ĵ�����Ϣ [ͨ��,����������Ϣ]
	PtzPresetMap				m_cfgPresetMap;					//���ô洢��Ԥ�õ���Ϣ [ͨ��,Ԥ�õ�������Ϣ]
	PtzCuriseMap				m_cfgCuriseMap;					//���ô洢��Ѳ������Ϣ [ͨ��,Ѳ����������Ϣ]
	unsigned char				m_curiseIsPreview[MAX_CHNN_NUM][PTZ_MAX_CURISE_NUM];		//���ĳͨ����Ѳ�����Ƿ�Ԥ����,Ϊ0��ʾδ��Ԥ������Ϊ0��ʾ��Ԥ����	
	PtzProtocolMap				m_protocolMap;					//Э����֧�ֵĿ��Ʒ�ʽ [Э����,Э�������]

	//
	CPUB_Lock					m_lock;
	unsigned long				m_idPtzCtrl;		//�������������̨����
#ifdef __ENVIRONMENT_LINUX__
	CFakeCurise					*m_pFakeCurise;
#endif
};

#endif

