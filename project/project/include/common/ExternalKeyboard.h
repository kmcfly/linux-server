/*****************************************************************
Copyright (C),2009-2012, tongwei video Tech. Co., Ltd.

File name:ExternalKeyboard.h

Author	: ZHL

Version :	

Date	:	2009-10-15

Description:
	�����ⲿ���̣�485�ӿڵļ��̣������롣
	1���ýӿ�SetExKeyboardType( MESSAGE_TYPE ExKeyboard )
	   �����ò�ͬ�����485����
	2���ýӿ�bool ReceiveExternalKey( KEY_VALUE_INFO *pKeyInfo )����
	   �ⲿ���̵�����
	3�����ݲ�ͬ���͵ļ��̽����Ӵ��ڽ��յ�������ΪKEY_VALUE_INFO����

Others:

Function List:

History:

1.Date:

Author:

Modification:	
2.
******************************************************************/
#ifndef _EXTERNAL_KEYBOARD_H
#define _EXTERNAL_KEYBOARD_H


#include "dvrdvsdef.h"
#include "PUB_common.h"
#include "PTZ.h"


//�ⲿ�������ýṹ
typedef struct _external_keyboard_cfg
{
	MESSAGE_TYPE ExKeyboardType;		//�ⲿ��������
	unsigned char	name[32];					//��������
	
	unsigned char	baudrate;					//������
	unsigned char	databit;					//����λ
	unsigned char	stopbit;					//ֹͣλ
	unsigned char	checkbit;					//��żУ��λ�� 0����У��λ��1:��У�飬2��żУ��

	unsigned char	commandlen;					//485���̷���һ��������ֽ�����������ʱһ��Ҫ�����������ֽ�
	unsigned char	notused[3];

	
}EXTERNAL_KEYBOARD_CFG;

class CExternalKeyboard
{
public:

	static CExternalKeyboard *Instance();
	void SetExKeyboardType( KEYBOARD_TYPE ExKeyboardType );
	bool GetExKeyboardValue( KEY_VALUE_INFO *pkeyValue );
	void SetDVRID( unsigned long localDVRID ){ m_uclocalDVRID = localDVRID;}
	
	//����һ���߳�ר���������ⲿ����������
	bool Start();
	void Stop();
	
	~CExternalKeyboard(void);
	
private:
	CExternalKeyboard(void);

	static PUB_THREAD_RESULT PUB_THREAD_CALL ExBoardThread(void *pParam);
	void AddExKeyboardProc();
	bool ReceiveExKeyboardInput( KEY_VALUE_INFO *pKeyInfo );

	bool TranslateTaiwanOEM( KEY_VALUE_INFO *pKeyInfo, const BYTE *buf );
	bool TranslateMY1003( KEY_VALUE_INFO *pKeyInfo, const BYTE *buf );
	bool TranslateBlack( KEY_VALUE_INFO *pKeyInfo, const BYTE *buf );
	bool TranslatePL14NVKBD30_70(KEY_VALUE_INFO *pKeyInfo, const BYTE *buf);
	bool TranslateTWSY( KEY_VALUE_INFO *pKeyInfo, const BYTE *buf);

	//����ⲿ485��������
	EXTERNAL_KEYBOARD_CFG m_ExKeyboardCfg;
	//���ץȡ���ļ�ֵ
	std::list<KEY_VALUE_INFO> m_keyList;

	PUB_thread_t		m_ExKeyboardProcID;
	bool				m_bExKeyboardProc;
	CPUB_Lock			m_keyListLock;

	unsigned char		m_uclocalDVRID;								//��������DVR ID ��unsigned long ���ͣ� ����������ܲ���unsigned char ��DVR ID
														
};


#endif


