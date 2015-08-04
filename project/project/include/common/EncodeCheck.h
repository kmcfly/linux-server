#ifndef __FRAME_CHECK_H__
#define __FRAME_CHECK_H__

#include "dvrdvsdef.h"

// typedef struct __support_enc_info__
// {
// 	unsigned long	ulRes;			//�ֱ���
// 	unsigned short	usMaxRate;		//���֡��
// 	unsigned short	usMaxBitrate;	//������ʣ���ǰ��ʹ�ã������ϵķ�ʽ������
// 	unsigned long	ulDefBitrate;	//Ĭ�����ʣ���ǰ��ʹ�ã������ϵķ�ʽ������
// }SUPPORT_ENC_INFO;

typedef struct __device_enc_info__
{
	unsigned short		usTotalRate;		//��֡��
	unsigned long		ulRes;				//�ֱ���
	unsigned short		usTotalBitrate;		//������
	unsigned short		usBCurrentRes[32];	//�Ƿ��ǵ�ǰ�õķֱ���
	SUPPORT_ENC_INFO *	pSupportEncInfo;	//ÿ��ͨ���÷ֱ��ʵ���Ϣ, ����Ϊsizeof(ENC_INFO) * channelNum
}DEVICE_ENC_INFO;


class CEncodeCheck
{
public:
	~CEncodeCheck();

	static CEncodeCheck * Instance();

	bool Initial(unsigned long ulChnn);
	void Quit();

	int AddEncodeInfo(unsigned long ulRes, unsigned short usTotalRate, unsigned short usMaxRate, unsigned short usTotalBitrate);

	int GetEncodeInfo(SUPPORT_ENC_INFO * pSupportEncInfo, unsigned long & dataLen);
	bool CheckEncodeParam(unsigned long ulChnn, CHECK_ENC_INFO * pCheckEncInfo);
	void SetChannelRes(unsigned long ulChnn, unsigned long ulRes);
	void ClearData();
protected:
	CEncodeCheck();
private:
	unsigned long GetDefBitrate(unsigned long ulRes);
	
private:
	static CEncodeCheck	* m_pEncodeCheck;
	unsigned long		m_channel;
	unsigned long		m_resNum;

	DEVICE_ENC_INFO		m_deviceEncInfo[128];
	bool				m_bSourceSelf;

	bool				m_bHaveInitial;
};

#endif

