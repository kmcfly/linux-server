
#ifndef _CONFIG_PARSER_H_
#define _CONFIG_PARSER_H_

#include "dvrdvsconfig.h"
//#include "DVR_NET_SDK.h"

typedef struct _net_sdk_deviceinfo
{
	long videoInputNum;
	long netVideoNum;
	long sensorOutputNum;
	long audioInputNum;
	long sensorInputNum;
}NET_SDK_DEVICEINFO, *LPNET_SDK_DEVICEINFO;

class CConfigParser
{
public:
	CConfigParser(void){};
	virtual ~CConfigParser(void){};

	virtual void SetVideoInputNum(long localVideoNum, long netVideoNum) = 0;
	virtual void SetSensorInNum(long sensorInNum) = 0;

	//��SDK�ӿ����͵�������ת��Ϊ�豸Э��֧�ֵ��������ݸ�ʽ(��������ʱ��)
	virtual long GetCfgRequestParam(DD_CONFIG_ITEM_ID configItem, void *pCfgParam, long cfgParamLen) = 0;

	//���豸���ص�������Ϣת��ΪSDK����ʶ���������Ϣ(��������ʱ��)
	//�������Ϊ�豸������Ϣ�����ȡ�����ת����SDK�����õ�ָ�뼰�ռ䳤��
	//����ת�����SDK���������ֽڳ���
	virtual long CfgDevice2CfgSDK(void *pDeviceCfg, long deviceCfgLen, unsigned char*& pSDKCfg, long& sdkCfgLen, DD_CONFIG_ITEM_HEAD* pDDcfgHead) = 0;

	//��SDK��װ��������Ϣת��Ϊ�豸��ʶ���������Ϣ(��������ʱ��)
	//�������ΪSDK������Ϣ�����ȡ�����ת�����豸���õ�ָ�뼰�ռ䳤��
	//����ת������豸���������ֽڳ���
	virtual long CfgSDK2CfgDevice(void *pSDKCfg, long sdkCfgLen, void *pDeviceCfg, long deviceCfgLen) = 0;

	//��½���netdeivce���л�ȡ���豸����Ϣ
	virtual void SetDeivceInfo(LPNET_SDK_DEVICEINFO deviceInfo) = 0;
};

#endif
