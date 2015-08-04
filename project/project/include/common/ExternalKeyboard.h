/*****************************************************************
Copyright (C),2009-2012, tongwei video Tech. Co., Ltd.

File name:ExternalKeyboard.h

Author	: ZHL

Version :	

Date	:	2009-10-15

Description:
	接收外部键盘（485接口的键盘）的输入。
	1：用接口SetExKeyboardType( MESSAGE_TYPE ExKeyboard )
	   来设置不同的外接485键盘
	2：用接口bool ReceiveExternalKey( KEY_VALUE_INFO *pKeyInfo )来读
	   外部键盘的输入
	3：根据不同类型的键盘解析从串口接收到的数据为KEY_VALUE_INFO类型

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


//外部键盘配置结构
typedef struct _external_keyboard_cfg
{
	MESSAGE_TYPE ExKeyboardType;		//外部键盘类型
	unsigned char	name[32];					//键盘名字
	
	unsigned char	baudrate;					//波特率
	unsigned char	databit;					//数据位
	unsigned char	stopbit;					//停止位
	unsigned char	checkbit;					//奇偶校验位， 0：无校验位，1:奇校验，2：偶校验

	unsigned char	commandlen;					//485键盘发出一条命令的字节数，读串口时一次要读这样长的字节
	unsigned char	notused[3];

	
}EXTERNAL_KEYBOARD_CFG;

class CExternalKeyboard
{
public:

	static CExternalKeyboard *Instance();
	void SetExKeyboardType( KEYBOARD_TYPE ExKeyboardType );
	bool GetExKeyboardValue( KEY_VALUE_INFO *pkeyValue );
	void SetDVRID( unsigned long localDVRID ){ m_uclocalDVRID = localDVRID;}
	
	//创建一个线程专门来处理外部按键的输入
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

	//存放外部485键盘类型
	EXTERNAL_KEYBOARD_CFG m_ExKeyboardCfg;
	//存放抓取到的键值
	std::list<KEY_VALUE_INFO> m_keyList;

	PUB_thread_t		m_ExKeyboardProcID;
	bool				m_bExKeyboardProc;
	CPUB_Lock			m_keyListLock;

	unsigned char		m_uclocalDVRID;								//在配置中DVR ID 是unsigned long 类型， 名扬键盘智能操作unsigned char 的DVR ID
														
};


#endif


