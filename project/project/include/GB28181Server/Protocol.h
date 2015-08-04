/******************************************************************************

filename:Protocol.h

describe:sip body������ͷ�ļ�

copyright:

author:����

date:2013/04/10

******************************************************************************/
#pragma once

#include "MarkupSTL.h"
#include "GBT28181_Define.h"

class CProtocol
{
public:
	CProtocol(void);
	virtual ~CProtocol(void);

	//�˳�ǰ��Ҫ���ã��������ڴ�й¶
	static void Release()
	{
		m_ResponseCatalogProtocol.Delete();
		m_ResponseDeviceStatusProtocol.Delete();
		m_ResponseRecordInfoProtocol.Delete();
	}

	/************************************************************************/
	/*                                 ���                                 */
	/************************************************************************/
	//�����������(Control/Query/Notify/Response)
	static bool Pack_Protocol(const CBasicProtocol* pBasicProtocol, string& strXml);

	/************************************************************************/
	/*                                 ���                                 */
	/************************************************************************/
	//������������(Control/Query/Notify/Response)
	static bool Unpack_Protocol(const char* szDoc, CBasicProtocol* &pBasicProtocol);

protected:
	static bool Unpack_ControlProtocol();
	static bool Unpack_QueryDeviceStatusProtocol();
	static bool Unpack_QueryCatalogProtocol();
	static bool Unpack_QueryDeviceInfoProtocol();
	static bool Unpack_QueryRecordInfoProtocol();
	static bool Unpack_QueryAlarmProtocol();
	static bool Unpack_NotifyKeepAliveProtocol();
	static bool Unpack_NotifyAlarmProtocol();
	static bool Unpack_ResponseDeviceControlProtocol();
	static bool Unpack_ResponseAlarmProtocol();
	static bool Unpack_ResponseCatalogProtocol();
	static bool Unpack_ResponseDeviceInfoProtocol();
	static bool Unpack_ResponseDeviceStatusProtocol();
	static bool Unpack_ResponseRecordInfoProtocol();

	static bool Pack_ControlProtocol(const char* szSN, const CBasicProtocol* pBasicProtocol, string& strXml);
	static bool Pack_QueryDeviceStatusProtocol(const char* szSN, const CBasicProtocol* pBasicProtocol, string& strXml);
	static bool Pack_QueryCatalogProtocol(const char* szSN, const CBasicProtocol* pBasicProtocol, string& strXml);
	static bool Pack_QueryDeviceInfoProtocol(const char* szSN, const CBasicProtocol* pBasicProtocol, string& strXml);
	static bool Pack_QueryRecordInfoProtocol(const char* szSN, const CBasicProtocol* pBasicProtocol, string& strXml);
	static bool Pack_QueryAlarmProtocol(const char* szSN, const CBasicProtocol* pBasicProtocol, string& strXml);
	static bool Pack_NotifyKeepAliveProtocol(const char* szSN, const CBasicProtocol* pBasicProtocol, string& strXml);
	static bool Pack_NotifyFileEndProtocol(const char* szSN, const CBasicProtocol* pBasicProtocol, string& strXml);
	static bool Pack_NotifyAlarmProtocol(const char* szSN, const CBasicProtocol* pBasicProtocol, string& strXml);
	static bool Pack_ResponseDeviceControlProtocol(const char* szSN, const CBasicProtocol* pBasicProtocol, string& strXml);
	static bool Pack_ResponseAlarmProtocol(const char* szSN, const CBasicProtocol* pBasicProtocol, string& strXml);
	static bool Pack_ResponseCatalogProtocol(const char* szSN, const CBasicProtocol* pBasicProtocol, string& strXml);
	static bool Pack_ResponseDeviceInfoProtocol(const char* szSN, const CBasicProtocol* pBasicProtocol, string& strXml);
	static bool Pack_ResponseDeviceStatusProtocol(const char* szSN, const CBasicProtocol* pBasicProtocol, string& strXml);
	static bool Pack_ResponseRecordInfoProtocol(const char* szSN, const CBasicProtocol* pBasicProtocol, string& strXml);

private:
	static CMarkupSTL* m_pMarkupSTL;

	static CControlProtocol m_ControlProtocol;
	static CQueryDeviceStatusProtocol m_QueryDeviceStatusProtocol;
	static CQueryCatalogProtocol m_QueryCatalogProtocol;
	static CQueryDeviceInfoProtocol m_QueryDeviceInfoProtocol;
	static CQueryRecordInfoProtocol m_QueryRecordInfoProtocol;
	static CQueryAlarmProtocol m_QueryAlarmProtocol;
	static CNotifyKeepAliveProtocol m_NotifyKeepAliveProtocol;
	static CNotifyAlarmProtocol m_NotifyAlarmProtocol;
	static CResponseDeviceControlProtocol m_ResponseDeviceControlProtocol;
	static CResponseAlarmProtocol m_ResponseAlarmProtocol;
	static CResponseCatalogProtocol m_ResponseCatalogProtocol;
	static CResponseDeviceInfoProtocol m_ResponseDeviceInfoProtocol;
	static CResponseDeviceStatusProtocol m_ResponseDeviceStatusProtocol;
	static CResponseRecordInfoProtocol m_ResponseRecordInfoProtocol;
};
