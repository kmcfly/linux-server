/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : ysw
** Date         : 2009-07-22
** Name         : LocalUIMan.h
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/
#ifndef _LOCAL_UI_MAN_H_
#define _LOCAL_UI_MAN_H_
#include "PUB_common.h"
#include "Product.h"
#include "Dialog.h"
#include "Disktop.h"
#include "StringTable.h"
#include "IniReader.h"

#ifdef __ENVIRONMENT_LINUX__
#include "LocalDevice.h"
#endif

using namespace GUI;

class CLocalUIMan
{
public:
	static CLocalUIMan * Instance();
	virtual ~CLocalUIMan();

#ifdef __ENVIRONMENT_WIN32__
	bool Initial(LANGUAGE language, unsigned short width, unsigned short height, unsigned long dateMode, unsigned long timeMode, const char *pFilePath, CDisplayCtrl *pDisplay);
#else
	bool Initial(LANGUAGE language, const char *pFilePath, unsigned short & width, unsigned short & height, unsigned long dateMode, unsigned long timeMode, TRANSPARENCY trans = TRANSPARENCY_0);
#endif
	void Quit();

	void SetProductInfo(const PRODUCT_INFO & productInfo);

	void SetResolution(unsigned short width, unsigned short height);
	bool Start();
	void Stop();

	void DoModal();
	//////////////////////////////////////////////////////////////////////////
protected:
private:
	CLocalUIMan();
	static PUB_THREAD_RESULT PUB_THREAD_CALL KeyProcThread(void *pParam);
	int KeyProc();

	bool LoadColor(const char* filepath);
	unsigned long GetColor(CIniReader& ini, const std::string& str_r, const std::string& str_g, const std::string& str_b) const;
	
private:
	//////////////////////////////////////////////////////////////////////////
	unsigned long m_lastKeyTime;	//表示上次操作的时间
	CDisktop*	m_pDisktop;

#ifdef __ENVIRONMENT_LINUX__
	CLocalDevice    *m_pLocalDeviceMan;
#endif
	//////////////////////////////////////////////////////////////////////////
	PUB_thread_t			m_keyProcID;
	bool					m_bKeyProc;
};

#endif //_LOCAL_UI_MAN_H_
